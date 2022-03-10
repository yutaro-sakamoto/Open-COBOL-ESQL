#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "ocpgsql.h"
#include "ocdblog.h"
#include "ocdbutil.h"

static char sql_savepoint[]          = "SAVEPOINT oc_save";
static char sql_release_savepoint[]  = "RELEASE SAVEPOINT oc_save";
static char sql_rollback_savepoint[] = "ROLLBACK TO oc_save";

//PostgreSQL ErrorCode
static const char errcode_violate_unique_constraint[] = "23505";

//TODOリスト化予定
static unsigned long PGLockConnId = OCDB_CONN_FAIL_CONNECT;

//単文ロールバックフラグ
static int rollback_one_mode = 0;

static PGresult *rollback_to_savepoint(unsigned long, PGresult *);

unsigned long
OCDB_PGConnect(char *conninfo, int autocommit, char *cencoding){
	PGconn *conn;
	PGresult *res = NULL;
	char *env_rollback_mode;
	char *env_cursor_withhold_mode;

	conn = PQconnectdb(conninfo);

	if(conn == NULL){
		return OCDB_CONN_FAIL_CONNECT;
	} else if(PQstatus(conn) != CONNECTION_OK){
		ERRLOG("%s\n",PQerrorMessage(conn));
		PQfinish(conn);
		return OCDB_CONN_FAIL_CONNECT;
	}

	PQsetClientEncoding(conn, cencoding);

	env_rollback_mode = ocdb_getenv("OCDB_PG_IGNORE_ERROR", NULL);
	if(env_rollback_mode != NULL && strcmp(env_rollback_mode, "Y")==0){
		rollback_one_mode = 1;
	}else{
		rollback_one_mode = 0;
	}

	return (unsigned long)conn;
}

int
OCDB_PGstatus(unsigned long connadr){
	ConnStatusType stat;

	stat = PQstatus((PGconn *)connadr);

	if(stat == CONNECTION_OK){
		return OCDB_CONN_CONNECT_OK;
	}

	return OCDB_CONN_FAIL_CONNECT;
}

char *
OCDB_PGErrorMessage(unsigned long connadr){
	PGconn *conn;
	conn = (PGconn *)connadr;
	return  PQerrorMessage(conn);
}

void
OCDB_PGClear(unsigned long resaddr){
	PGresult *res;

	res = (PGresult *)resaddr;
	PQclear(res);
	return;
}

unsigned long
OCDB_PGExec(unsigned long connadr, char *query){
	PGconn *conn;
	PGresult *res;
	PGresult *save_res;
	int status;
	int tmp_rollback_one_mode=0;

	LOG("CONNADDR: %d, EXEC SQL: %s\n", connadr, query);
	conn = (PGconn *)connadr;
	if(rollback_one_mode){
		tmp_rollback_one_mode = 1;
		if(strcmp(query, "BEGIN") == 0 ||
				strcmp(query, "COMMIT") == 0 ||
				strcmp(query, "ROLLBACK") == 0){
			tmp_rollback_one_mode = 0;
		}
		if(tmp_rollback_one_mode){
			save_res = PQexec(conn, sql_savepoint);
			PQclear(save_res);
		}
	}
	res = PQexec(conn, query);
	if(tmp_rollback_one_mode){
		status = PQresultStatus(res);
		
		if(status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK ||
				status == PGRES_COPY_OUT || status == PGRES_COPY_IN){
			save_res = PQexec(conn, sql_release_savepoint);
			PQclear(save_res);
		} else {
			// Exec ROLLBACK and update PGresult
			if(res){
				char *sqlstate = PQresultErrorField(res, PG_DIAG_SQLSTATE);
				if(sqlstate == NULL || 
				   strcmp(sqlstate, errcode_violate_unique_constraint) != 0){
					save_res = PQexec(conn, sql_release_savepoint);
					PQclear(save_res);
				} else {
					res = rollback_to_savepoint(connadr, res);
				}
			} else {
				res = rollback_to_savepoint(connadr, res);
			}
		}
	}
	return (unsigned long)res;
}

unsigned long
OCDB_PGExecParams(unsigned long connadr, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat){
	PGconn *conn;
	PGresult *res;
	PGresult *save_res;
	int status;

	LOG("CONNADDR: %d, EXEC SQL: %s\n", connadr, query);
	conn = (PGconn *)connadr;
	if(rollback_one_mode){
		save_res = PQexec(conn, sql_savepoint);
		PQclear(save_res);
	}
	res = PQexecParams(conn, query, nParams, (const Oid *)paramsTypes, paramValues,
			paramLengths, paramFormats, resultFormat);
	conn = (PGconn *)connadr;
	if(rollback_one_mode){
		status = PQresultStatus(res);
		if(status == PGRES_COMMAND_OK || status == PGRES_TUPLES_OK ||
				status == PGRES_COPY_OUT || status == PGRES_COPY_IN){
			save_res = PQexec(conn, sql_release_savepoint);
			PQclear(save_res);
		} else {
			// Exec ROLLBACK and update PGresult
			if(res){
				char *sqlstate = PQresultErrorField(res, PG_DIAG_SQLSTATE);
				if(sqlstate == NULL || 
				   strcmp(sqlstate, errcode_violate_unique_constraint) != 0){
					save_res = PQexec(conn, sql_release_savepoint);
					PQclear(save_res);
				} else {
					res = rollback_to_savepoint(connadr, res);
				}
			} else {
				res = rollback_to_savepoint(connadr, res);
			}
		}
	}
	return (unsigned long)res;
}



unsigned long
OCDB_PGCursorDeclare(unsigned long connadr, char *cname, char *query, int with_hold){
	const char *query_part_with_hold_on[] = {"DECLARE ", " CURSOR WITH HOLD FOR "};
	const char *query_part[] = {"DECLARE ", " CURSOR FOR "};
	char *true_query;
	int true_query_size;
	unsigned long res;

	if(with_hold == OCDB_CURSOR_WITH_HOLD_ON){
		true_query_size = strlen(query_part_with_hold_on[0]) + strlen(cname) +
			strlen(query_part_with_hold_on[1]) +  strlen(query) + 1;
		true_query = (char *)malloc(true_query_size * sizeof(char));
		if(true_query == NULL){
			return OCDB_RES_DEFAULT_ADDRESS;
		}

		// build query
		sprintf(true_query, "%s%s%s%s",  query_part_with_hold_on[0], cname,
				query_part_with_hold_on[1], query);
	} else {
		true_query_size = strlen(query_part[0]) + strlen(cname) +
			strlen(query_part[1]) + strlen(query) + 1;
		true_query = (char *)malloc(true_query_size * sizeof(char));
		if(true_query == NULL){
			return OCDB_RES_DEFAULT_ADDRESS;
		}

		// build query
		sprintf(true_query, "%s%s%s%s",  query_part[0], cname, query_part[1], query);
	}

	res = (unsigned long)OCDB_PGExec(connadr, true_query);
	if(res){
		// カーソルを確定させるためにCOMMITを入れる
		//OCDB_PGExec(connadr, "COMMIT");
		//OCDB_PGExec(connadr, "BEGIN");
	}
	free(true_query);
	return res;
}

unsigned long
OCDB_PGCursorDeclareParams(unsigned long connadr, char *cname, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat, int with_hold){
	const char *query_part_with_hold_on[] = {"DECLARE ", " CURSOR WITH HOLD FOR "};
	const char *query_part[] = {"DECLARE ", " CURSOR FOR "};
	char *true_query;
	int true_query_size;
	unsigned long res;

	if(with_hold == OCDB_CURSOR_WITH_HOLD_ON){
		true_query_size = strlen(query_part_with_hold_on[0]) + strlen(cname) +
			strlen(query_part_with_hold_on[1]) + strlen(query) + 1;
		true_query = (char *)malloc(true_query_size * sizeof(char));
		if(true_query == NULL){
			return OCDB_RES_DEFAULT_ADDRESS;
		}

		// build query
		sprintf(true_query, "%s%s%s%s",  query_part_with_hold_on[0], cname,
				query_part_with_hold_on[1], query);
	} else {
		true_query_size = strlen(query_part[0]) + strlen(cname) +
			strlen(query_part[1]) + strlen(query) + 1;
		true_query = (char *)malloc(true_query_size * sizeof(char));
		if(true_query == NULL){
			return OCDB_RES_DEFAULT_ADDRESS;
		}

		// build query
		sprintf(true_query, "%s%s%s%s",  query_part[0], cname, query_part[1], query);
	}

	res = (unsigned long)OCDB_PGExecParams(connadr, true_query, nParams,
			paramsTypes, paramValues,
			paramLengths, paramFormats, resultFormat);
	if(res){
		// カーソルを確定させるためにCOMMITを入れる
		//OCDB_PGExec(connadr, "COMMIT");
	}
	free(true_query);
	return res;
}

int
OCDB_PGCountRow(unsigned long connadr, char *query){
	const char *query_part[] = {"SELECT COUNT(*) FROM ( ", " ) OCDB_COUNT"};
	char *true_query;
	char *result_value;
	int true_query_size;
	unsigned long res;
	int result;

	true_query_size = strlen(query_part[0]) + strlen(query) + strlen(query_part[1])  + 1;
	true_query = (char *)malloc(true_query_size * sizeof(char));
	if(true_query == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	// build query
	sprintf(true_query, "%s%s%s",  query_part[0], query, query_part[1]);

	res = (unsigned long)OCDB_PGExec(connadr, true_query);
	free(true_query);
	if(PQresultStatus(res) != PGRES_TUPLES_OK){
		ERRLOG("Count Row Error\n");
		ERRLOG("code:%d\n", PQresStatus(PQresultStatus(res)));
		ERRLOG("status:%s\n", PQresultErrorField(res, PG_DIAG_SQLSTATE));
		ERRLOG("message:%s\n", PQresultErrorMessage(res));
		return -1;
	}
	result_value = PQgetvalue(res, 0, 0);
	if(!result_value){
		return -1;
	}
	result = strcmp(result_value, "0");
	PQclear(res);
	return result;
}

int
OCDB_PGCountRowParams(unsigned long connadr, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat){
	const char *query_part[] = {"SELECT COUNT(*) FROM ( ", " ) OCDB_COUNT"};
	char *true_query;
	char *result_value;
	int true_query_size;
	unsigned long res;
	int result;


	true_query_size = strlen(query_part[0]) + strlen(query) + strlen(query_part[1])  + 1;
	true_query = (char *)malloc(true_query_size * sizeof(char));
	if(true_query == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	// build query
	sprintf(true_query, "%s%s%s",  query_part[0], query, query_part[1]);

	res = (unsigned long)OCDB_PGExecParams(connadr, true_query, nParams,
			paramsTypes, paramValues,
			paramLengths, paramFormats, resultFormat);
	free(true_query);
	if(PQresultStatus(res) != PGRES_TUPLES_OK){
		ERRLOG("Count Row Error\n");
		ERRLOG("code:%d\n", PQresStatus(PQresultStatus(res)));
		ERRLOG("status:%s\n", PQresultErrorField(res, PG_DIAG_SQLSTATE));
		ERRLOG("message:%s\n", PQresultErrorMessage(res));
		return -1;
	}

	result_value = PQgetvalue(res, 0, 0);
	if(!result_value){
		return -1;
	}
	result = strcmp(result_value, "0");
	PQclear(res);
	return result;
}

unsigned long
OCDB_PGCursorFetchOne(unsigned long connadr, char *cname, int fetchmode){
	//TODO RELATIVEはPostgreSQLでは無意味な単語なため、FORWORD/BACKWORDに変更する
	const char *query_part[] = {"FETCH ", " RELATIVE ", " FROM "};
	const char next[] = "1";
	const char current[] = "0";
	const char previous[] = "-1";
	char *true_query;
	int true_query_size;
	unsigned long res;

	true_query_size = strlen(query_part[0]) + strlen(query_part[1]) + 1 +
		strlen(query_part[2]) + strlen(cname) + 1;
	true_query = (char *)malloc(true_query_size * sizeof(char));
	if(true_query == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	// build query
	if(fetchmode == OCDB_READ_CURRENT){
		sprintf(true_query, "%s%s%s%s%s",
				query_part[0], query_part[1], current, query_part[2], cname);
	} else if(fetchmode == OCDB_READ_PREVIOUS){
		sprintf(true_query, "%s%s%s%s%s",
				query_part[0], query_part[1], previous, query_part[2], cname);
	} else { // NEXT
		sprintf(true_query, "%s%s%s%s%s",
				query_part[0], query_part[1], next, query_part[2], cname);
	}
	res = (unsigned long)OCDB_PGExec(connadr, true_query);
	free(true_query);
	return res;
}

unsigned long
OCDB_PGCursorFetchOccurs(unsigned long connadr, char *cname, int fetchmode, int fetchcount){

	const char *query_part[] = {"FETCH ", " FORWARD ", " BACKWARD " , " FROM "};
	char *true_query;
	int true_query_size;
	char *str_fetchcount;
	int str_fetchcount_size;
	char *str_read_mode;
	unsigned long res;

	//カレントはエラーとする
	if(fetchmode == OCDB_READ_CURRENT){
		return OCDB_RES_DEFAULT_ADDRESS;
	}

	//読み込み件数文字列を作成
	str_fetchcount_size = (int)log10(fetchcount) + 1 + 1;
	str_fetchcount = (char *)calloc(str_fetchcount_size, sizeof(char));
	if(str_fetchcount == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	//TODO 桁数上限の確認
	sprintf(str_fetchcount,"%d",fetchcount);

	if(fetchmode == OCDB_READ_PREVIOUS){
		str_read_mode = query_part[2];
	}else{
		str_read_mode = query_part[1];
	}

	true_query_size = strlen(query_part[0]) + strlen(str_read_mode) + strlen(str_fetchcount) +
		strlen(query_part[3]) + strlen(cname) + 1;
	true_query = (char *)malloc(true_query_size * sizeof(char));
	if(true_query == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	// build query
	sprintf(true_query, "%s%s%s%s%s",
			query_part[0], str_read_mode, str_fetchcount, query_part[3], cname);

	res = (unsigned long)OCDB_PGExec(connadr, true_query);
	free(true_query);
	free(str_fetchcount);
	return res;
}

unsigned long
OCDB_PGCursorClose(unsigned long connadr, char *cname){
	const char *query_part[] = {"CLOSE "};
	char *true_query;
	int true_query_size;
	unsigned long res;

	true_query_size = strlen(query_part[0]) + strlen(cname) + 1;
	true_query = (char *)malloc(true_query_size * sizeof(char));
	if(true_query == NULL){
		return OCDB_RES_DEFAULT_ADDRESS;
	}

	// build query
	sprintf(true_query, "%s%s",  query_part[0], cname);
	res = (unsigned long)OCDB_PGExec(connadr, true_query);
	free(true_query);
	return res;
}

int
OCDB_PGResultStatus(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	return PQresultStatus(res);
}

int
OCDB_PGResultStatusConvert(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	int tmpval = PQresultStatus(res);
	int retval;
	int ntuples;
	// Convert Status
	switch(tmpval){
	case PGRES_EMPTY_QUERY:
		retval = OCDB_RES_EMPTY_QUERY;
		break;
	case PGRES_COMMAND_OK:
		retval = OCDB_RES_COMMAND_OK;
		break;
	case PGRES_TUPLES_OK:
		// check tuples(オーバーフローの可能性あり)
		ntuples = PQntuples(res);
		if(ntuples > 0){
			retval = OCDB_RES_COMMAND_OK;
		} else {
			retval = OCDB_RES_TUPLES_NODATA;
		}
		break;
	case PGRES_COPY_OUT:
		retval = OCDB_RES_COPY_OUT;
		break;
	case PGRES_COPY_IN:
		retval = OCDB_RES_COPY_IN;
		break;
	case PGRES_BAD_RESPONSE:
		retval = OCDB_RES_BAD_RESPONSE;
		break;
	case PGRES_NONFATAL_ERROR:
		retval = OCDB_RES_NONFATAL_ERROR;
		break;
	case PGRES_FATAL_ERROR:
		if(strcmp(PQresultErrorField(res,PG_DIAG_SQLSTATE),"55P03")==0){
			retval = OCDB_RES_LOCKED_RECORD;
			break;
		}
		/* no break */
	default:
		retval = OCDB_RES_FATAL_ERROR;
		break;
	}

	return retval;
}

char *
OCDB_PGResultErrorField(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	return  PQresultErrorField(res,PG_DIAG_SQLSTATE);
}

char *
OCDB_PGResultErrorMessage(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	return PQresultErrorMessage(res);
}

int
OCDB_PGcmdtuples(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	char *result = PQcmdTuples(res);
	if((result != NULL) && (strlen(result) > 0)){
		return  atoi(result);
	}
	return 0;
}

int
OCDB_PGntuples(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	return  PQntuples(res);
}

int
OCDB_PGnfields(unsigned long connres){
	PGresult *res = (PGresult *)connres;
	return  PQnfields(res);
}

char *
OCDB_PGfname(unsigned long connres, int index){
	PGresult *res = (PGresult *)connres;
	return  PQfname(res, index);
}

int
OCDB_PGfnumber(unsigned long connres, const char *fname){
	PGresult *res = (PGresult *)connres;
	return  PQfnumber(res, fname);
}

char *
OCDB_PGgetvalue(unsigned long connres, int row, int cnum){
	PGresult *res = (PGresult *)connres;
	return  PQgetvalue(res, row, cnum);
}

unsigned long
OCDB_PGDropTable(unsigned long connaddr, char *tname){
	PGconn *conn;
	char *query;
	int query_size;
	unsigned long retval;
	const char *constr[] = {"DROP TABLE IF EXISTS "};

	query_size = strlen(constr[0]) + strlen(tname) + TERMINAL_LENGTH;
	query = (char *)calloc(query_size, sizeof(char));
	if(query == NULL){
		ERRLOG("memory allocation failed.\n");
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	sprintf(query, "%s%s", constr[0], tname);
	conn = (PGconn *)connaddr;
	retval = (unsigned long)OCDB_PGExec(connaddr, query);
	free(query);
	return retval;
};

unsigned long
OCDB_PGDeleteTable(unsigned long connaddr, char *tname){
	PGconn *conn;
	char *query;
	int query_size;
	unsigned long retval;
	const char *constr[] = {"DELETE FROM "};

	query_size = strlen(constr[0]) + strlen(tname) + TERMINAL_LENGTH;
	query = (char *)calloc(query_size, sizeof(char));
	if(query == NULL){
		ERRLOG("memory allocation failed.\n");
		return OCDB_RES_DEFAULT_ADDRESS;
	}
	sprintf(query, "%s%s", constr[0], tname);
	conn = (PGconn *)connaddr;
	retval = (unsigned long)OCDB_PGExec(connaddr, query);
	free(query);
	return retval;
};

int
OCDB_PGIsExistTable(unsigned long connaddr, char *tname, char *user, int viewflag){
	PGconn *conn;
	PGresult *res;
	int tuplesnum;
	int result;
	char *params[2];
	const char *contstr[] = {"SELECT TABLENAME FROM PG_TABLES WHERE LOWER(TABLENAME) = LOWER($1) AND LOWER(TABLEOWNER) = LOWER($2)"};
	const char *convstr[] = {"SELECT VIEWNAME FROM PG_VIEWS WHERE LOWER(VIEWNAME) = LOWER($1) AND LOWER(VIEWOWNER) = LOWER($2)"};

	conn = (PGconn *)connaddr;
	params[0] = tname;
	params[1] = user;
	if(!viewflag){
		res = PQexecParams(conn, contstr[0], 2, NULL, (const char * const *)params, NULL, NULL, 0);
	}else{
		res = PQexecParams(conn, convstr[0], 2, NULL, (const char * const *)params, NULL, NULL, 0);
	}
	result = OCDB_RES_BAD_RESPONSE;
	if(res != NULL){
		//system table check
		if(PQresultStatus(res) == PGRES_TUPLES_OK){
			tuplesnum = PQntuples(res);
			if(tuplesnum==1){
				result = OCDB_RES_COMMAND_OK;
			}else if(tuplesnum == 0){
				result = OCDB_RES_TUPLES_NODATA;
			}
		}else{
			char *msg = PQresultErrorMessage(res);
			ERRLOG("%s\n",msg);
			result = OCDB_RES_NONFATAL_ERROR;
		}
		PQclear(res);
	}else{
		ERRLOG("SELECT PG_TABLES IS FATAL ERROR\n");
		result = OCDB_RES_FATAL_ERROR;
	}
	return result;
}

int
OCDB_PGIsLockTable(unsigned long connaddr, char *tname, int lock_mode, int file_mode){
	PGconn *conn;
	PGresult *res;
	char *lockstr;
	const char *constr[] = {"select a.mode from pg_locks a LEFT OUTER JOIN pg_class b ON a.relation=b.relfilenode where b.relname= lower($1)"};
	char *params[1];
	int tuples;
	int i;
	int retval;

	conn = (PGconn *)connaddr;

	params[0] = tname;
	res = PQexecParams(conn,constr[0],1,NULL,(const char * const *)params,NULL,NULL,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RES_FATAL_ERROR;
	}else{
		switch(PQresultStatus(res)){
		case PGRES_TUPLES_OK:
			tuples = PQntuples(res);
			retval = OCDB_RES_COMMAND_OK;
			if(tuples>=1){
				if((file_mode == OCDB_FILEMODE_OUTPUT) || (lock_mode == OCDB_LOCK_EXCLUSIVE)){
					retval = OCDB_RES_LOCKED_TABLE;
				}else{
					for(i=0;i<tuples;i++){
						lockstr = PQgetvalue(res,i,0);
						if((strcmp(lockstr,"AccessExclusiveLock")==0) || (strcmp(lockstr,"ExclusiveLock")==0)){
							retval = OCDB_RES_LOCKED_TABLE;
							break;
						}
					}
				}
			}
			break;
		default:
			ERRLOG("%s\n",PQresultErrorMessage(res));
			retval = OCDB_RES_FATAL_ERROR;
			break;
		}
	}
	PQclear(res);
	return retval;
}


COLUMN_INFO_LIST *
OCDB_PGGetTableInfo(unsigned long connaddr, const char *dbname, const char *tablename){
	PGconn *conn;
	PGresult *res;
	int tuples;
	int i;
	COLUMN_INFO_LIST *head;
	COLUMN_INFO_LIST *current;
	char *type_str;
	int type;
	
	const char constr[] = 
		"SELECT "
		" column_name, "
		" data_type, "
		" character_maximum_length, "
		" numeric_precision, "
		" numeric_scale "
		"FROM "
		" information_schema.columns "
		"WHERE "
		" table_catalog = $1 and "
		" LOWER(table_name) = LOWER($2) "
		"ORDER BY "
		" ordinal_position";
	conn = (PGconn *)connaddr;
	char *params[2] = {dbname, tablename};
	head = NULL;
	res = PQexecParams(conn, constr, 2, NULL, (const char * const *)params, NULL, NULL, 0);
	if(res != NULL){
		if(PQresultStatus(res) == PGRES_TUPLES_OK){
			head = info_list_generate();
			current = head;
			tuples = PQntuples(res);
			for(i=0; i<tuples; i++){
				type_str = PQgetvalue(res,i,1);
				if(strstr(type_str, "numeric") != NULL){
					type = OCDB_COLUMN_TYPE_NUMERIC;
				}else if(strstr(type_str, "character") != NULL){
					type = OCDB_COLUMN_TYPE_CHARACTER;
				}else{
					type = OCDB_COLUMN_TYPE_UNKNOWN;
				}
				current = add_info_list(current, PQgetvalue(res,i,0), type,
					atoi(PQgetvalue(res,i,2)),
					atoi(PQgetvalue(res,i,3)),
					atoi(PQgetvalue(res,i,4)));
			}
		}else{
			ERRLOG("%s\n",PQresultErrorMessage(res));
		}
		PQclear(res);
	}else{
		ERRLOG("SELECT PG_TABLES IS FATAL ERROR\n");
	}
	return head;
};

void
OCDB_PGFinish(unsigned long connaddr){
	PGconn *conn;

	conn = (PGconn *)connaddr;
	PQfinish(conn);
	if(PGLockConnId != OCDB_CONN_FAIL_CONNECT){
		PQfinish((PGconn *)PGLockConnId);
		PGLockConnId = OCDB_CONN_FAIL_CONNECT;
	}
};

/*NATIVEロック用API*/

unsigned long
OCDB_PGLockTable(unsigned long connaddr, char *tname, int lock_mode, int file_mode, int waitforlocks){
	PGconn *conn;
	PGresult *res;
	char *query;
	int query_size;
	const char *constr[] = {"LOCK TABLE ", " IN ", " MODE", " NOWAIT"};
	const char *lckstr[] = {"ROW SHARE", "EXCLUSIVE", "ACCESS EXCLUSIVE"};
	int lock_type=0;
	conn = (PGconn *)connaddr;
	LOG("lock_mode = %d, file_mode = %d\n",lock_mode,file_mode);
	if(file_mode == OCDB_FILEMODE_OUTPUT){
		lock_type = 2;
	}else if((file_mode == OCDB_FILEMODE_I_O) && ((lock_mode & OCDB_LOCK_EXCLUSIVE) != 0)){
		lock_type = 2;
	}else if((file_mode == OCDB_FILEMODE_INPUT) && ((lock_mode & OCDB_LOCK_EXCLUSIVE) != 0)){
		lock_type = 1;
	}else{
		lock_type = 0;
	}

	query = NULL;
	if(waitforlocks==OCDB_LOCK_WAIT){
		query_size = strlen(constr[0])+strlen(tname)+strlen(constr[1])+strlen(lckstr[lock_type])+
			strlen(constr[2])+TERMINAL_LENGTH;
		if((query = (char *)calloc(query_size,sizeof(char))) == NULL){
			return RESULT_ERROR;
		}
		sprintf(query,"%s%s%s%s%s",constr[0],tname,constr[1],lckstr[lock_type],constr[2]);
	}else{
		query_size = strlen(constr[0])+strlen(tname)+strlen(constr[1])+strlen(lckstr[lock_type])+
			strlen(constr[2])+strlen(constr[3])+TERMINAL_LENGTH;
		if((query = (char *)calloc(query_size,sizeof(char))) == NULL){
			return RESULT_ERROR;
		}
		sprintf(query,"%s%s%s%s%s%s",constr[0],tname,constr[1],lckstr[lock_type],constr[2],constr[3]);
	}

	LOG("lock query = %s\n",query);
	res = PQexec(conn,query);
	free(query);
	return (unsigned long)res;
}

/*LOCKDB用API*/

int
OCDB_PGConnectLockDB(const char *host,const char *port){
	//LOCKDBとのコネクションがなければ作成する
	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		char *query;
		int length=0;
		const char *constr[] = {"dbname=oclockdb user=oclockadmin", " host=", " port="};

		length = strlen(constr[0])+TERMINAL_LENGTH;
		if(host!=NULL){
			length += strlen(constr[1])+strlen(host);
		}
		if(port!=NULL){
			length += strlen(constr[1])+strlen(port);
		}
		if((query = (char *)calloc(length,sizeof(char))) == NULL){
			return OCDB_RESLOCK_ERROR_SEGFAULT;
		}
		sprintf(query,"%s",constr[0]);
		if(host!=NULL){
			sprintf(query,"%s%s%s",query,constr[1],host);
		}
		if(port!=NULL){
			sprintf(query,"%s%s%s",query,constr[2],port);
		}
		PGLockConnId = OCDB_PGConnect(query,OCDB_AUTOCOMMIT_ON,"SJIS");
		if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
			return OCDB_RESLOCK_ERROR_NOTCONNECT;
		}
	}
	return OCDB_RESLOCK_COMMAND_OK;
}

int
OCDB_PGCreateLockData(unsigned long connaddr, LOCK_CONN *lockcon, const char *table){
	PGconn *conn;
	char *tmpdb;
	int dblen, tablelen, schemalen;

	conn = (PGconn *)connaddr;

	tmpdb = PQdb(conn);
	dblen = strlen(tmpdb)+TERMINAL_LENGTH;
	if((lockcon->db = (char *)calloc(dblen,sizeof(char))) == NULL){
		return OCDB_RESLOCK_ERROR_SEGFAULT;
	}
	sprintf(lockcon->db,"%s",tmpdb);

	tablelen = strlen(table)+TERMINAL_LENGTH;
	if((lockcon->table = (char *)calloc(tablelen,sizeof(char))) == NULL){
		return OCDB_RESLOCK_ERROR_SEGFAULT;
	}
	sprintf(lockcon->table,"%s",table);
	//TODO
	schemalen = strlen("")+TERMINAL_LENGTH;
	if((lockcon->schema = (char *)calloc(schemalen,sizeof(char))) == NULL){
		return OCDB_RESLOCK_ERROR_SEGFAULT;
	}
	sprintf(lockcon->schema,"%s","");

	lockcon->pid = uint_to_str(PQbackendPID(conn));

	return OCDB_RESLOCK_COMMAND_OK;
}

int
OCDB_PGCheckLockTable(LOCK_CONN *lockcon, char *key_record){
	char *constr[] ={"select lockpid, keyvalue from oclocks where dbname = $1 and schema = $2  and tablename = $3 and keyvalue = $4"};
	char *params[4];
	int paramLengths[4] = {0,0,0,0};
	int paramFormats[4] = {0,0,0,0};
	PGconn *conn;
	PGresult *res;
	int tuples, retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;
	params[3] = key_record;
	paramLengths[3] = strlen(key_record);
	paramFormats[3] = 1;

	res = PQexecParams(conn, constr[0], 4, NULL, (const char * const *)params, paramLengths, paramFormats, 0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGCheckLockTable_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_TUPLES_OK:
		tuples = PQntuples(res);
		if(tuples == 0){
			retval = OCDB_RESLOCK_RECORD_NOLOCK;
		}else if(tuples == 1){
			if(strcmp(lockcon->pid,PQgetvalue(res,0,0))==0){
				retval = OCDB_RESLOCK_RECORD_LOCKED_OWN;
			}else{
				retval = OCDB_RESLOCK_RECORD_LOCKED;
			}
		}else{
			ERRLOG("There is more than one of the same record locking.");
			retval = OCDB_RESLOCK_ERROR_FATAL;
		}
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGCheckLockTable_Exit:
	if(res) PQclear(res);
	return retval;
}

int
OCDB_PGWriteLockTable(LOCK_CONN *lockcon, char *key_record){
	char *constr[] ={"insert into oclocks values ( $1 , $2 , $3 , $4 , $5)"};
	char *params[5];
	int paramLengths[5] = {0,0,0,0,0};
	int paramFormats[5] = {0,0,0,0,0};
	PGconn *conn;
	PGresult *res;
	int retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;
	params[3] = lockcon->pid;
	params[4] = key_record;
	paramLengths[4] = strlen(key_record);
	paramFormats[4] = 1;


	res = PQexecParams(conn,constr[0],5,NULL,(const char * const *)params,paramLengths,paramFormats,0);
	if(res==NULL){
		ERRLOG("fatal error");
		return OCDB_RESLOCK_ERROR_FATAL;
	}
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK:
		return OCDB_RESLOCK_COMMAND_OK;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		return OCDB_RESLOCK_ERROR_FATAL;
	}

	if(res) PQclear(res);
	return retval;
}

int
OCDB_PGDeleteLockTable(LOCK_CONN *lockcon, char *key_record){
	char *constr[] ={"delete from oclocks where dbname = $1 and schema = $2 and tablename = $3 and lockpid = $4 and keyvalue = $5"};
	char *params[5];
	int paramLengths[5] = {0,0,0,0,0};
	int paramFormats[5] = {0,0,0,0,0};
	PGconn *conn;
	PGresult *res;
	int retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;
	params[3] = lockcon->pid;
	params[4] = key_record;
	paramLengths[4] = strlen(key_record);
	paramFormats[4] = 1;

	res = PQexecParams(conn,constr[0],5,NULL,(const char * const *)params,paramLengths,paramFormats,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGDeleteLockTable_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK:
		retval = OCDB_RESLOCK_COMMAND_OK;
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGDeleteLockTable_Exit:
	if(res) PQclear(res);
	return retval;
}

int
OCDB_PGDeleteLockTableAll(char *pid){
	char *constr[] ={"delete from oclocks where lockpid = $1"};
	char *params[1];
	PGconn *conn;
	conn = (PGconn *)PGLockConnId;
	PGresult *res;
	int retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = pid;

	res = PQexecParams(conn,constr[0],1,NULL,(const char * const *)params,NULL,NULL,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGDeleteLockTableAll_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK:
		retval = OCDB_RESLOCK_COMMAND_OK;
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGDeleteLockTableAll_Exit:
	if(res) PQclear(res);
	return retval;
}

int
OCDB_PGCheckOpenTable(LOCK_CONN *lockcon, int openmode, int lockmode){
	char *constr[] ={"select openpid, openmode, lockmode from ocopentables where dbname = $1 and schema = $2 and tablename = $3"};
	char *params[3];
	PGconn *conn;
	PGresult *res;
	int i, tuples, retval;
	int omode, lmode;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;

	res = PQexecParams(conn,constr[0],3,NULL,(const char * const *)params,NULL,NULL,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGCheckOpenTable_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_TUPLES_OK:
		tuples = PQntuples(res);
		for(i=0;i<tuples;i++){
			omode = atoi(PQgetvalue(res,i,1));
			lmode = atoi(PQgetvalue(res,i,2));

			if((omode==OCDB_FILEMODE_OUTPUT) || (openmode==OCDB_FILEMODE_OUTPUT)){
				retval = OCDB_RESLOCK_TABLE_LOCKED;
				goto OCDB_PGCheckOpenTable_Exit;
			}

			if((lmode == OCDB_LOCK_EXCLUSIVE) && (lockmode == OCDB_LOCK_EXCLUSIVE)){
				retval = OCDB_RESLOCK_TABLE_LOCKED;
				goto OCDB_PGCheckOpenTable_Exit;
			}

			if((lmode == OCDB_LOCK_EXCLUSIVE) || (lockmode == OCDB_LOCK_EXCLUSIVE)){
				if(omode == OCDB_FILEMODE_INPUT & openmode == OCDB_FILEMODE_INPUT){
					continue;
				}
				retval = OCDB_RESLOCK_TABLE_LOCKED;
				goto OCDB_PGCheckOpenTable_Exit;
			}
		}
		retval = OCDB_RESLOCK_TABLE_NOLOCK;
		break;
	case OCDB_RES_TUPLES_NODATA:
		retval = OCDB_RESLOCK_TABLE_NOLOCK;
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGCheckOpenTable_Exit:
	if(res)  PQclear(res);
	return retval;
}

int
OCDB_PGWriteOpenTable(LOCK_CONN *lockcon, int openmode, int lockmode){
	char *constr[] ={"insert into ocopentables ( dbname, schema, tablename, openpid, openmode, lockmode ) values ( $1 , $2 , $3 , $4 , $5 , $6)"};
	char *params[6];
	PGconn *conn;
	PGresult *res;
	int retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;
	params[3] = lockcon->pid;
	params[4] = uint_to_str(openmode);
	params[5] = uint_to_str(lockmode);

	res = PQexecParams(conn,constr[0],6,NULL,(const char * const *)params,NULL,NULL,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval = OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGWriteOpenTable_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK:
		retval = OCDB_RESLOCK_COMMAND_OK;
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGWriteOpenTable_Exit:
	if(params[4]) free(params[4]);
	if(params[5]) free(params[5]);
	if(res) PQclear(res);
	return retval;
}

int
OCDB_PGDeleteOpenTable(LOCK_CONN *lockcon){
	char *constr[] ={"delete from ocopentables where dbname = $1 and schema = $2 and tablename = $3 and openpid = $4"};
	char *params[4];
	PGconn *conn;
	PGresult *res;
	int retval;

	if(PGLockConnId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection to oclockdb not found");
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}

	conn = (PGconn *)PGLockConnId;

	params[0] = lockcon->db;
	params[1] = lockcon->schema;
	params[2] = lockcon->table;
	params[3] = lockcon->pid;

	res = PQexecParams(conn,constr[0],4,NULL,(const char * const *)params,NULL,NULL,0);
	if(res==NULL){
		ERRLOG("fatal error");
		retval =  OCDB_RESLOCK_ERROR_FATAL;
		goto OCDB_PGDeleteOpenTable_Exit;
	}
	switch(PQresultStatus(res)){
	case PGRES_COMMAND_OK:
		retval = OCDB_RESLOCK_COMMAND_OK;
		break;
	default:
		ERRLOG("%s\n",PQresultErrorMessage(res));
		retval = OCDB_RESLOCK_ERROR_FATAL;
		break;
	}
	OCDB_PGDeleteOpenTable_Exit:
	if(res) PQclear(res);
	return retval;
}

static PGresult *
rollback_to_savepoint(unsigned long connadr, PGresult *res){
	PGconn *conn;
	PGresult *save_res;

	conn = (PGconn *)connadr;

	// Exec ROLLBACK and update PGresult
	if(res){
		ERRLOG("SQL Failed.\n");
		ERRLOG("code:%s\n", PQresStatus(PQresultStatus(res)));
		ERRLOG("status:%s\n", PQresultErrorField(res, PG_DIAG_SQLSTATE));
		ERRLOG("message:%s\n", PQresultErrorMessage(res));
		PQclear(res);
	} else {
		ERRLOG("SQL Failed, but log is nothing.\n");
	}
	LOG("CONNADDR: %d, EXEC SQL: ROLLBACK TO SAVEPOINT\n", connadr);
	res = PQexec(conn, sql_rollback_savepoint);

	save_res = PQexec(conn, sql_release_savepoint);
	PQclear(save_res);

	return res;
}
