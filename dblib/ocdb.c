#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ocdblog.h"
#include "ocdb.h"
#include "ocdbutil.h"

static struct conn_list _conn_lists = {{-1,NULL,0,0,0,0,NULL,NULL},NULL};
static int _next_conn_id = DEFAULT_NEXT_CONN_ID;
static int dummy_conn_id = 0;

/* connection list */
static struct s_conn *look_up_conn_lists(int);
static struct conn_list *create_conn(int, char *, int);
static int add_conn_lists(int, char *, int);
static void free_conn_lists(int);

/*
 * OCDBCheckType
 * @dbtype
 * return value
 *   success: RESULT_SUCCESS
 *   failed: RESULT_FAILED
 */
int
OCDBCheckType(int dbtype){
	int retval = RESULT_FAILED;

	// check
	if(1)
	{
		retval = RESULT_SUCCESS;
	}

	return retval;
}

/*
 * OCDBConnect
 * @conninfo:  * failure: -1
 */
int
OCDBConnect(int dbtype, char *conninfo, char *connname, int autocommit, char *cencoding){
	int retval = OCDB_CONN_NOT_CONNECT;
	unsigned long connaddr;
#ifdef PGSQL_MODE_ON
	connaddr = OCDB_PGConnect(conninfo, autocommit, cencoding);
#endif

	if(connaddr == OCDB_CONN_FAIL_CONNECT){
		return OCDB_CONN_FAIL_CONNECT;
	}

	retval = add_conn_lists(dbtype, connname, connaddr);
	LOG("connid=%d\n", retval);
	return retval;
}

/*
 * OCDBStatus
 * @id connction id
 * return
 *  success: OCDB_CONN_CONNECT_OK
 *  failure: OCDB_CONN_FAIL_CONNECT
 */
int
OCDBStatus(int id){
	struct s_conn *p_conn;
	int retval = OCDB_CONN_FAIL_CONNECT;

	p_conn = look_up_conn_lists(id);

	if(p_conn == NULL) return retval;

#ifdef PGSQL_MODE_ON
	retval = OCDB_PGstatus(p_conn->connaddr);
#endif

	return retval;
}

/*
 * OCDBErrorMessage
 * @id connction id
 * success: string
 * failure: NULL
 */
char *
OCDBErrorMessage(int id){
	struct s_conn *p_conn;
	char *retstr;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return NULL;
	}

#ifdef PGSQL_MODE_ON
	retstr = OCDB_PGErrorMessage(p_conn->connaddr);
#endif

	return retstr;
}

/*
 * OCDBExec
 * @id connction id
 * @query SQL query
 */
void
OCDBExec(int id, char *query){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}
	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGExec(p_conn->connaddr, query);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBExecParams
 * @id connction id
 * @query SQL query
 */
void
OCDBExecParams(int id, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	LOG("id=%d,p_conn=%d\n", id, p_conn);
	if(p_conn == NULL){
		return;
	}


	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGExecParams(p_conn->connaddr, query, nParams, paramsTypes, paramValues,
			paramLengths, paramFormats, resultFormat);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBCursorDeclare
 * @id connction id
 * @cname cursor name
 * @query SQL query
 * @with_hold flag for Cursor Definition "WITH HOLD"
 */
void
OCDBCursorDeclare(int id, char *cname, char *query, int with_hold){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGCursorDeclare(p_conn->connaddr, cname, query, with_hold);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBCursorDeclareParams
 * @id connction id
 * @query SQL query
 * @with_hold flag for Cursor Definition "WITH HOLD"
 */
void
OCDBCursorDeclareParams(int id, char *cname, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat, int with_hold){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGCursorDeclareParams(p_conn->connaddr, cname, query, nParams,
			paramsTypes, paramValues, paramLengths,
			paramFormats, resultFormat, with_hold);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

int
OCDBCountRow(int id, char *query){
	struct s_conn *p_conn;
	int rows;
	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return -1;
	}
	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
		p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	}
#endif
	rows = -1;
#ifdef PGSQL_MODE_ON
	rows = OCDB_PGCountRow(p_conn->connaddr, query);
#endif
	return rows;
}

int
OCDBCountRowParams(int id, char *query, int nParams,
		const int *paramsTypes, const char * const *paramValues,
		const int *paramLengths, const int *paramFormats,
		int resultFormat){
	struct s_conn *p_conn;
	int rows;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return -1;
	}
	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
		p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	}
#endif
	rows = -1;
#ifdef PGSQL_MODE_ON
	rows = OCDB_PGCountRowParams(p_conn->connaddr, query, nParams,
			paramsTypes, paramValues, paramLengths,
			paramFormats, resultFormat);
#endif
	return rows;
}

/*
 * OCDBCursorOpen
 * @id connction id
 * @cname cursor name
 */
void
OCDBCursorOpen(int id, char *cname){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	// nothing to do
	return;
#endif

	return;
}

/*
 * OCDBCursorFetchOne
 * @id connction id
 * @cname cursor name
 */
void
OCDBCursorFetchOne(int id, char *cname, int fetchmode){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	LOG("addr:%d, cname:%s, mode:%d\n",p_conn->connaddr, cname, fetchmode);
	p_conn->resaddr = OCDB_PGCursorFetchOne(p_conn->connaddr, cname, fetchmode);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBCursorFetchOccurs
 * @id connction id
 * @cname cursor name
 * @count fetch data count
 */
void
OCDBCursorFetchOccurs(int id, char *cname, int fetchmode, int count){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	LOG("addr:%d, cname:%s, mode:%d, count:%d\n",p_conn->connaddr, cname, fetchmode, count);
	p_conn->resaddr = OCDB_PGCursorFetchOccurs(p_conn->connaddr, cname, fetchmode, count);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBCursorClose
 * @id connction id
 * @cname cursor name
 */
void
OCDBCursorClose(int id, char *cname){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}

	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	p_conn->result = RESULT_SUCCESS;

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGCursorClose(p_conn->connaddr, cname);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

/*
 * OCDBResultStatus
 * @id connction id
 * return
 *  statuscode(PREFIX: OCDBRES_)
 */
int
OCDBResultStatusConvert(int id){
	struct s_conn *p_conn;
	int retval;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RES_FATAL_ERROR;
	}

	if(p_conn->resaddr == OCDB_RES_DEFAULT_ADDRESS){
		// 結果リソースが無いため成功で返す
		return OCDB_RES_COMMAND_OK;
	}

#ifdef PGSQL_MODE_ON
	switch(OCDB_PGResultStatus(p_conn->resaddr)){
	case PGRES_EMPTY_QUERY:
		retval = OCDB_RES_EMPTY_QUERY;
		break;
	case PGRES_COMMAND_OK:
		retval = OCDB_RES_COMMAND_OK;
		break;
	case PGRES_TUPLES_OK:
		if(OCDB_PGntuples(p_conn->resaddr) > 0){
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
		if(strcmp(OCDB_PGResultErrorField(p_conn->resaddr),"55P03")==0){
			retval = OCDB_RES_LOCKED_RECORD;
			break;
		}
		/* no break */
	default:
		retval = OCDB_RES_FATAL_ERROR;
		break;
	}
#endif
	return retval;
}

int
OCDBResultStatus(int id){
	struct s_conn *p_conn;
	int retval;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RES_FATAL_ERROR;
	}

	if(p_conn->resaddr == OCDB_RES_DEFAULT_ADDRESS){
		// 結果リソースが無いため成功で返す
		return OCDB_RES_COMMAND_OK;
	}

#ifdef PGSQL_MODE_ON
	switch(OCDB_PGResultStatus(p_conn->resaddr)){
	case PGRES_COMMAND_OK:
	case PGRES_TUPLES_OK:
	case PGRES_COPY_OUT:
	case PGRES_COPY_IN:
		retval = OCDB_RES_COMMAND_OK;
		break;
	case PGRES_EMPTY_QUERY:
	case PGRES_BAD_RESPONSE:
	case PGRES_NONFATAL_ERROR:
	case PGRES_FATAL_ERROR:
	default:
		retval = OCDB_RES_FATAL_ERROR;
		break;
	}
#endif
	return retval;
}


/*
 * OCDBResultErrorMessage
 * @id connction id
 * return value
 *   errorMessage(or NULL)
 */
char *
OCDBResultErrorMessage(int id){
	struct s_conn *p_conn;

	LOG("id=%d\n", id);
	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RES_NOCONNECT_ERRORMSG;
	}

	// initialize
	p_conn->errorMessage = NULL;
#ifdef PGSQL_MODE_ON
	p_conn->errorMessage = OCDB_PGResultErrorMessage(p_conn->resaddr);
#endif
	return p_conn->errorMessage;
}


char *
OCDBResultErrorField(int id){
	struct s_conn *p_conn;
	char *errfield;

	LOG("id=%d\n", id);
	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RES_NOCONNECT_ERRORMSG;
	}

	// initialize
	errfield = NULL;
#ifdef PGSQL_MODE_ON
	errfield = OCDB_PGResultErrorField(p_conn->resaddr);
#endif
	return errfield;
}

int
OCDBCmdTuples(int id){
	struct s_conn *p_con;
	int retval;

	p_con = look_up_conn_lists(id);
	if(p_con == NULL){
		return OCDB_INVALID_NUMBER;
	}

#ifdef PGSQL_MODE_ON
	retval = OCDB_PGcmdtuples(p_con->resaddr);
#endif

	return retval;
}


int
OCDBNtuples(int id){
	struct s_conn *p_con;
	int retval;

	p_con = look_up_conn_lists(id);
	if(p_con == NULL){
		return OCDB_INVALID_NUMBER;
	}

#ifdef PGSQL_MODE_ON
	retval = OCDB_PGntuples(p_con->resaddr);
#endif

	return retval;
}

int
OCDBNfields(int id){
	struct s_conn *p_conn;
	int retval;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_INVALID_NUMBER;
	}

#ifdef PGSQL_MODE_ON
	retval = OCDB_PGnfields(p_conn->resaddr);
#endif

	return retval;
}

char *
OCDBFname(int id, int index){
	struct s_conn *p_conn;
	char *retstr;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_INVALID_STRING;
	}

#ifdef PGSQL_MODE_ON
	retstr = OCDB_PGfname(p_conn->resaddr, index);
#endif

	return retstr;
}

int
OCDBFnumber(int id, const char *fname){
	struct s_conn *p_conn;
	int retval;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_INVALID_NUMBER;
	}

#ifdef PGSQL_MODE_ON
	retval = OCDB_PGfnumber(p_conn->resaddr, fname);
#endif

	return retval;
}

char *
OCDBGetvalue(int id, int row, int cnum){
	struct s_conn *p_conn;
	char *retstr;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return NULL;
	}

#ifdef PGSQL_MODE_ON
	retstr = OCDB_PGgetvalue(p_conn->resaddr, row, cnum);
	LOG("retstr:%s\n",retstr);
#endif

	return retstr;
}


void
OCDBDropTable(int id, char *tname){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}
	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGDropTable(p_conn->connaddr, tname);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

void
OCDBDeleteTable(int id, char *tname){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return;
	}
	// initialize
#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		// release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif

#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGDeleteTable(p_conn->connaddr, tname);
	if(!p_conn->resaddr){
		ERRLOG("PostgreSQL Result is NULL");
	}
#endif
	return;
}

int
OCDBIsExistTable(int id, char *tname, char *user, int viewflag){
	struct s_conn *p_conn;
	int result;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return RESULT_FAILED;
	}

#ifdef PGSQL_MODE_ON
	result = OCDB_PGIsExistTable(p_conn->connaddr, tname, user, viewflag);
#endif

	return result;
}

COLUMN_INFO_LIST *
OCDBGetTableInfo(int id, const char *dbname, const char *tablename){
	struct s_conn *p_conn;
	COLUMN_INFO_LIST *info_list;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		LOG("no connection id %d found.\n", id);
		return;
	}

#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr){
		OCDB_PGClear(p_conn->resaddr);
		p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
	}
	info_list = OCDB_PGGetTableInfo(p_conn->connaddr, dbname, tablename);
#endif
	LOG("get table info was successful.\n");
	return info_list;
}

void
OCDBFinish(int id){
	struct s_conn *p_conn;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		LOG("no connection id %d found.\n", id);
		return;
	}

#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr){
		OCDB_PGClear(p_conn->resaddr);
	}
	OCDB_PGFinish(p_conn->connaddr);
#endif
	LOG("connection id %d released.\n", id);
	free_conn_lists(id);
	return;
}

/*
 * <Function name>
 *   OCDBResolveCONNID
 *
 * <Outline>
 *   接続識別子から接続IDを探し出す
 *
 * <Input>
 *  cid : 接続識別子
 *
 * <Output>
 *   success : 接続ID
 *   failure : -1(RESULT_FAILED)
 */
int
OCDBResolveCONNID(char *cid){
	struct conn_list *tmp = &_conn_lists;

	while(tmp->next != NULL){
		tmp = tmp->next;
		LOG("#%d#%s#search:%s#\n", tmp->sc.id, tmp->sc.cid, cid);
		if(strcmp(cid, tmp->sc.cid) == 0){
			LOG("return connid=%d\n", tmp->sc.id);
			return tmp->sc.id;
			break;
		}
	}

	return RESULT_FAILED;
}

/* NATIVEロック用API */
int
OCDBIsLockTable(int id, char *tname, int lock_mode, int file_mode){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RES_BAD_RESPONSE;
	}

	res = OCDB_RES_FATAL_ERROR;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGIsLockTable(p_conn->connaddr, tname, lock_mode, file_mode);
#endif
	return res;
}

int
OCDBLockTable(int id, char *tname, int lock_mode, int file_mode, int waitforlocks){
	struct s_conn *p_conn;
	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return RESULT_ERROR;
	}

#ifdef PGSQL_MODE_ON
	if(p_conn->resaddr != OCDB_RES_DEFAULT_ADDRESS){
		//release resource
		OCDB_PGClear(p_conn->resaddr);
	}
#endif
	p_conn->resaddr = OCDB_RES_DEFAULT_ADDRESS;
#ifdef PGSQL_MODE_ON
	p_conn->resaddr = OCDB_PGLockTable(p_conn->connaddr,tname,lock_mode,file_mode,waitforlocks);
	if(p_conn->resaddr){
		p_conn->result = OCDB_PGResultStatusConvert(p_conn->resaddr);
		if(p_conn->result==0){
			return RESULT_SUCCESS;
		} else {
			p_conn->errorMessage = OCDB_PGResultErrorMessage(p_conn->resaddr);
		}
	}
	return RESULT_FAILED;
#endif
}


/* ロック管理テーブル用API */

int
OCDBConnectLockDB(char *host, char *port){
	int res;

	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGConnectLockDB(host, port);
#endif
	return res;
}

int
OCDBCreateLockData(int id, LOCK_CONN *lockcon, char *table){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGCreateLockData(p_conn->connaddr, lockcon, table);
	p_conn->pid = strdup(lockcon->pid);
#endif
	return res;
}

int
OCDBCheckLockTable(int id, LOCK_CONN *lockcon,char *key_record){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGCheckLockTable(lockcon, key_record);
#endif
	return res;
}

int
OCDBWriteLockTable(int id, LOCK_CONN *lockcon, char *key_record){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGWriteLockTable(lockcon, key_record);
#endif
	return res;
}

int
OCDBDeleteLockTable(int id, LOCK_CONN *lockcon, char *key_record){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGDeleteLockTable(lockcon, key_record);
#endif
	return res;
}

int
OCDBDeleteLockTableAll(int id){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn == NULL){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	if(p_conn->pid){
		res = OCDB_PGDeleteLockTableAll(p_conn->pid);
	}else{
		res = OCDB_RESLOCK_COMMAND_OK;
	}
#endif
	return res;
}

int
OCDBCheckOpenTable(int id, LOCK_CONN *lockcon, int openmode, int lockmode){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn->id == INVALID_CONN_ID){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGCheckOpenTable(lockcon,openmode,lockmode);
#endif
	return res;
}

int
OCDBWriteOpenTable(int id, LOCK_CONN *lockcon, int openmode, int lockmode){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn->id == INVALID_CONN_ID){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGWriteOpenTable(lockcon,openmode,lockmode);
#endif
	return res;
}

int
OCDBDeleteOpenTable(int id, LOCK_CONN *lockcon){
	struct s_conn *p_conn;
	int res;

	p_conn = look_up_conn_lists(id);
	if(p_conn->id == INVALID_CONN_ID){
		return OCDB_RESLOCK_ERROR_NOTCONNECT;
	}
	res = OCDB_RESLOCK_ERROR_FATAL;
#ifdef PGSQL_MODE_ON
	res = OCDB_PGDeleteOpenTable(lockcon);
#endif
	return res;
}

/*
 * <Function name>
 *   look_up_conn_lists
 *
 * <Outline>
 *   接続IDの情報を接続リストから捜し出す
 *
 * <Input>
 *  id : 接続ID
 *
 * <Output>
 *   success : 接続リソース構造体(struct s_conn *)
 *   failure : NULL
 */
static struct s_conn *
look_up_conn_lists(int id){
	struct s_conn *retval = NULL;
	struct conn_list *tmp = &_conn_lists;

	while(tmp->next != NULL){
		tmp = tmp->next;
		if(tmp->sc.id == id){
			retval = &(tmp->sc);
			break;
		}
	}
	return retval;
}

/*
 * <Function name>
 *   create_conn
 *
 * <Outline>
 *   接続用リストの要素を新規に作成する
 *
 * <Input>
 *   dbtype : データベースのタイプ
 *   connname : 接続時に指定した識別子(指定が無い場合はNULL)
 *   connaddr : コネクションリソースのアドレス(PostgresならPGconn *と同等)
 *
 * <Output>
 *   success : struct conn_list *
 *   failure ; NULL
 */
static struct conn_list *
create_conn(int dbtype, char *connname, int connaddr){
	// initialize
	int newId;
	struct conn_list *p;

	p = (struct conn_list *)malloc(sizeof(struct conn_list));
	if(p != NULL){
		newId = _next_conn_id;
		_next_conn_id++;

		p->next = NULL;
		p->sc.id = newId;
		p->sc.cid = strdup(connname);
		p->sc.dbtype = dbtype;
		p->sc.connaddr = connaddr;
		p->sc.resaddr = OCDB_RES_DEFAULT_ADDRESS;
		p->sc.result = RESULT_SUCCESS;;
		p->sc.errorMessage = NULL;
		p->sc.pid = NULL;
	}

	return p;
}

/*
 * <Function name>
 *   create_conn
 *
 * <Outline>
 *   接続用リストの要素をコネクションリストに追加する
 *
 * <Input>
 *   dbtype : データベースのタイプ
 *   connname : 接続時に指定した識別子(指定が無い場合はNULL)
 *   connaddr : コネクションリソースのアドレス(PostgresならPGconn *と同等)
 *
 * <Output>
 *   success : craete_connで発行された接続ID
 *   failure : INVALID_CONN_ID
 */
static int
add_conn_lists(int dbtype, char *connname, int connaddr){
	// add list and return unique ID(start: 1)
	struct conn_list *index = &_conn_lists;

	while(index->next != NULL){
		index = index->next;
		if(index->sc.cid != NULL &&
				strcmp(index->sc.cid, connname) == 0){
			ERRLOG("connection id %s is already registered.\n", connname);
			return INVALID_CONN_ID;
		}
		if(index->sc.cid != NULL &&
				strcmp(index->sc.cid, connname) == 0){
			return index->sc.id;
		}
		/*--------------------------*/
	}

	// insert
	index->next = create_conn(dbtype, connname, connaddr);
	if(index->next == NULL){
		return INVALID_CONN_ID;
	}

	return index->next->sc.id;
}

/*
 * <Function name>
 *   free_conn_lists
 *
 * <Outline>
 *   接続IDに対応する接続を終了し、リストから消去する
 *  (未実装)
 *
 * <Input>
 *  id : 接続ID
 *
 * <Output>
 *  なし
 */
static void
free_conn_lists(int id){
	struct conn_list *tmp = &_conn_lists;
	struct conn_list *tmp_prev = NULL;

	while(tmp->next != NULL){
		tmp_prev = tmp;
		tmp = tmp->next;
		if(tmp->sc.id == id){
			tmp_prev->next = tmp->next;
			if(tmp->sc.cid) free(tmp->sc.cid);
			if(tmp->sc.pid) free(tmp->sc.pid);
			free(tmp);
			break;
		}
	}
}

/*
 * common APIs
 */
char *
_alloc(long size)
{
	char *new = (char *) calloc(1L, size);
	return (new);
}

char *
_strdup(const char *string)
{
	char *new;

	if (string == NULL)
		return NULL;

	new = strdup(string);
	return (new);
}

COLUMN_INFO_LIST *
info_list_generate(void)
{
	COLUMN_INFO_LIST *head;
	head = (COLUMN_INFO_LIST *)calloc(1, sizeof(COLUMN_INFO_LIST));
	return head;
}

COLUMN_INFO_LIST *
add_info_list(COLUMN_INFO_LIST *current, char* name, int type, int length, int precision, int scale)
{
	COLUMN_INFO_LIST *new_list;
	new_list = (COLUMN_INFO_LIST *)calloc(1, sizeof(COLUMN_INFO_LIST));
	current->next = new_list;
	new_list->column_info = (COLUMN_INFO *)calloc(1, sizeof(COLUMN_INFO));
	new_list->column_info->name = strdup(name);
	new_list->column_info->type = type;
	new_list->column_info->character_length = length;
	new_list->column_info->numeric_precision = precision;
	new_list->column_info->numeric_scale = scale;
	
	return new_list;
}

void
info_list_finish(COLUMN_INFO_LIST *list)
{
	if(list == NULL){
		return;
	}
	info_list_finish(list->next);
	if(list->column_info){
		if(list->column_info->name){
			free(list->column_info->name);
		}
		free(list->column_info);
	}
	free(list);
}



/*
 *	last_dir_separator
 *
 * Find the location of the last directory separator, return
 * NULL if not found.
 */
char *
last_dir_separator(const char *filename)
{
	const char *p,
	*ret = NULL;

	for (p = skip_drive(filename); *p; p++)
		if (IS_DIR_SEP(*p))
			ret = p;
	return (char *) ret;
}

int
strlen_or_null(const char *string)
{
	if (!string)
		return 0;
	return (strlen(string));
}

int set_coboldata(COBOLFIELD *field, char *real_data){

	//バッファの準備
	static char *cobol_data_buffer = NULL;
	static int current_buffer_len = 0;
	if(cobol_data_buffer == NULL){
		cobol_data_buffer = (char *)calloc(OCDB_COBOL_BUFFER_SIZE, sizeof(char));
		current_buffer_len = OCDB_COBOL_BUFFER_SIZE;
	}else if(current_buffer_len <= strlen(real_data)){
		current_buffer_len = strlen(real_data);
		free(cobol_data_buffer);
		cobol_data_buffer = (char *)calloc(current_buffer_len + TERMINAL_LENGTH, sizeof(char));
	}

	int disp_length = 0;

	if(field->type <= OCDB_TYPE_UNSINGED_BINARY_NATIVE){
		//数値型
		int i;
		int buf_i;
		int is_negative;
		int integer_digit;
		int decimal_digit;
		char *tmp_data;
		char  *dppos;

		if(real_data[0] == '-'){
			is_negative = 1;
			real_data = real_data + 1;
		}else{
			is_negative = 0;
		}

		dppos = strchr(real_data, '.');
		if(dppos){
			integer_digit = (int)(dppos - real_data);
			decimal_digit = strlen(real_data) - integer_digit - 1;
		}else{
			integer_digit  = strlen(real_data);
			decimal_digit = 0;
		}
		buf_i = 0;
		for(i = 0; i < field->length - integer_digit + field->power; i++){
			cobol_data_buffer[buf_i] = '0';
			buf_i++;
		}
		for(i = 0; i < strlen(real_data); i++){
			if(real_data[i] != '.'){
				cobol_data_buffer[buf_i] = real_data[i];
				buf_i++;
			}
		}
		for(i = 0; i < -(field->power) - decimal_digit; i++){
			cobol_data_buffer[buf_i] = '0';
			buf_i++;
		}
		cobol_data_buffer[buf_i] = '\0';


		switch(field->type){
		case OCDB_TYPE_UNSIGNED_NUMBER:
		{
			memcpy(field->addr, cobol_data_buffer, field->length);
			break;
		}
		case OCDB_TYPE_SIGNED_NUMBER_TC:
		{
			if(is_negative){
				int index = cobol_data_buffer[buf_i - 1] - '0';
				cobol_data_buffer[buf_i - 1] = type_tc_negative_final_number[index];
			}
			memcpy(field->addr, cobol_data_buffer, field->length);
			break;
		}
		case OCDB_TYPE_SIGNED_NUMBER_LS:
		{
			if(is_negative){
				memset((char *)field->addr, '-', 1);
			}else{
				memset((char *)field->addr, '+', 1);
			}
			memcpy(field->addr + 1, cobol_data_buffer, field->length);
			break;
		}
		case OCDB_TYPE_UNSIGNED_NUMBER_PD:
		case OCDB_TYPE_SIGNED_NUMBER_PD:
		{
			int use_first_bit = 0;
			int cobol_byte_length = 0;
			int real_i = 0;
			unsigned char ubit = 0xF0;
			unsigned char lbit = 0x0F;
			unsigned char vubit = 0;
			unsigned char vlbit = 0;

			cobol_byte_length = (field->length / 2) + 1;
			use_first_bit = (field->length) % 2;

			for(i = 0; i < cobol_byte_length; i++){
				if(i!=0 || use_first_bit){
					vubit = (cobol_data_buffer[real_i]) << 4;
					vubit = vubit & ubit;
					real_i++;
				}
				if(i != cobol_byte_length - 1){
					vlbit = cobol_data_buffer[real_i];
					vlbit = vlbit & lbit;
					real_i++;
				}else if(field->type == OCDB_TYPE_SIGNED_NUMBER_PD){
					if(is_negative){
						vlbit = 0x0D;
					}else{
						vlbit = 0x0C;
					}

				}else{
					vlbit = 0x0F;
				}
				((char *)field->addr)[i] = vubit | vlbit;
			}
			break;
		default :
			ERRLOG("UNSUPPORTED TYPE");
			break;
		}

		}
		disp_length = field->length;
	}else{
		//文字列
		int tmp_len;
		int i;
		char *tmp;
		int field_byte;
		switch(field->type){
		case OCDB_TYPE_ALPHANUMERIC:
			if(strlen(real_data) >= field->length){
				memcpy(field->addr, real_data, field->length);
			}else{
				memset(field->addr,' ',field->length);
				memcpy(field->addr,real_data,strlen(real_data));
			}
			disp_length = field->length;
			break;
		case OCDB_TYPE_JAPANESE:
			field_byte = field->length * 2;
			if(strlen(real_data) >= field_byte){
				memcpy(field->addr, real_data, field_byte);
			}else{
				tmp = (char *)field->addr;
				memset(field->addr, ' ', field_byte);
				memcpy(field->addr,real_data,strlen(real_data));
			}
			tmp = (char *)field->addr;
			for(i=field_byte-1;i>=1;i-=2){
				if(tmp[i] == ' ' && tmp[i-1] == ' '){
					tmp[i-1] = 0x81;
					tmp[i] = 0x40;
				}
			}
			disp_length = field_byte;
			break;
		case OCDB_TYPE_ALPHANUMERIC_VARYING:
		case OCDB_TYPE_BYTEA:
			if(strlen(real_data) >= field->length){
				tmp_len = field->length;
			} else {
				tmp_len = strlen(real_data);
			}
			memcpy(field->addr, &tmp_len, OCDB_VARCHAR_HEADER_BYTE);
			memset((char *)field->addr + OCDB_VARCHAR_HEADER_BYTE,' ',field->length);

			if (field->type == OCDB_TYPE_BYTEA){
				oc_memrep(real_data, tmp_len, 0);
				memcpy((char *)field->addr,real_data,tmp_len);
			} else {
				memcpy((char *)field->addr + OCDB_VARCHAR_HEADER_BYTE,real_data, tmp_len);
			}
			LOG("VARYING-LEN:%d\n",tmp_len);
			disp_length = field->length;
			break;
		case OCDB_TYPE_JAPANESE_VARYING:
			field_byte = field->length * 2;
			if(strlen(real_data) >= (field->length * 2)){
				tmp_len = field->length;
			} else {
				tmp_len =  field_byte / 2;
			}
			memcpy(field->addr, &tmp_len, OCDB_VARCHAR_HEADER_BYTE);
			tmp = (char *)((char *)field->addr+OCDB_VARCHAR_HEADER_BYTE);
			for(i=0;i+1<field_byte;i=i+2){
				tmp[i] = 0x81;
				tmp[i+1] = 0x40;
			}
			memcpy((char *)field->addr + OCDB_VARCHAR_HEADER_BYTE,real_data,strlen(real_data));
			LOG("VARYING-LEN:%d\n",field_byte);
			disp_length = field_byte;
			break;
		default:
			ERRLOG("UNSUPPORTED TYPE");
			break;
		}
	}
#ifndef NDEBUG
	if(field->type == OCDB_TYPE_JAPANESE_VARYING ||
	   field->type == OCDB_TYPE_ALPHANUMERIC_VARYING ||
	   field->type == OCDB_TYPE_BYTEA){
		LOG("%d %d#%s#%.*s#\n", field->type, disp_length, real_data, disp_length, 
		    (char *)field->addr + OCDB_VARCHAR_HEADER_BYTE);
	} else {
		LOG("%d %d#%s#%.*s#\n", field->type, disp_length, real_data, disp_length, field->addr);
	}
#endif
	return RESULT_SUCCESS;
}


char* set_realdata(COBOLFIELD *field){
	char *tmpdata;
	char *resultdata;
	int cobol_byte_length = 0;
	int realdata_length = 0;

	switch(field->type){
	case OCDB_TYPE_UNSIGNED_NUMBER:
	case OCDB_TYPE_SIGNED_NUMBER_TC:
	case OCDB_TYPE_SIGNED_NUMBER_LS:
	{
		int start = 0;
		cobol_byte_length = field->length;
		realdata_length = field->length;
		//LOW-VALUE判定
		if(is_low_value(field->addr, cobol_byte_length)){
			LOG("type:%d coboldata is low-value.\n", field->type);
			resultdata = (char *)calloc(1 + TERMINAL_LENGTH, sizeof(char));
			resultdata[0] = '0';
			return resultdata;
		}
		// 小数点
		if(field->power < 0){
			realdata_length++;
		}
		if (field->type == OCDB_TYPE_SIGNED_NUMBER_TC){
			start = 1;
			realdata_length += SIGN_LENGTH;
		}else if (field->type == OCDB_TYPE_SIGNED_NUMBER_LS){
			realdata_length += SIGN_LENGTH;
			cobol_byte_length++;
		}

		resultdata = (char *)calloc(realdata_length + TERMINAL_LENGTH, sizeof(char));
		memcpy(resultdata + start, field->addr, cobol_byte_length);

		if (field->type == OCDB_TYPE_SIGNED_NUMBER_TC){
			if(type_tc_is_positive(resultdata + SIGN_LENGTH + cobol_byte_length - 1)){
				resultdata[0] = '+';
			} else {
				resultdata[0] = '-';
			}
		}

		if(field->power < 0){
			insert_decimal_point(resultdata, realdata_length, field->power);
		}

		break;
	}
	case OCDB_TYPE_UNSIGNED_NUMBER_PD:
	case OCDB_TYPE_SIGNED_NUMBER_PD:
	{
		int use_first_bit = 0;
		int i,real_i;
		char real_val[2];
		unsigned char *current;
		unsigned char ubit = 0xF0;
		unsigned char lbit = 0x0F;
		cobol_byte_length = (field->length / 2) + 1;
		realdata_length = field->length;
		use_first_bit = (field->length) % 2;
		real_i = 0;
		//LOW-VALUE判定
		if(is_low_value(field->addr, cobol_byte_length)){
			LOG("type:%d coboldata is low-value.\n", field->type);
			resultdata = (char *)calloc(1 + TERMINAL_LENGTH, sizeof(char));
			resultdata[0] = '0';
			return resultdata;
		}
		// 小数点
		if(field->power < 0){
			realdata_length++;
		}
		// 符号部分
		if(field->type == OCDB_TYPE_SIGNED_NUMBER_PD){
			realdata_length += SIGN_LENGTH;
			real_i++;
		}

		resultdata = (char *)calloc(realdata_length + TERMINAL_LENGTH, sizeof(char));

		for(i = 0; i < cobol_byte_length; i++){
			current = (char *)(field->addr + i * sizeof(char));
			if(i!=0 || use_first_bit){
				sprintf(real_val, "%1d", (*current & ubit) >> 4);
				resultdata[real_i] = real_val[0];
				real_i++;
			}

			if(i != cobol_byte_length - 1){
				sprintf(real_val, "%1d", *current & lbit);
				resultdata[real_i] = real_val[0];
				real_i++;
			}else if(field->type == OCDB_TYPE_SIGNED_NUMBER_PD){
				if((*current & lbit) == 0x0B || (*current & lbit) == 0x0D ){
					resultdata[0] = '-';
				}else{
					resultdata[0] = '+';
				}
			}
		}
		if(field->power < 0){
			insert_decimal_point(resultdata, realdata_length, field->power);
		}
		break;
	}
	case OCDB_TYPE_BYTEA:
	{
		cobol_byte_length = field->length;
		LOG("VARYING-LEN:%d\n",cobol_byte_length);
		resultdata = (char *)calloc(cobol_byte_length + TERMINAL_LENGTH, sizeof(char));
		oc_memrep(field->addr, cobol_byte_length, 1);
		memcpy(resultdata, field->addr, cobol_byte_length);
		realdata_length = cobol_byte_length;
		break;
	}
	case OCDB_TYPE_JAPANESE_VARYING:
	case OCDB_TYPE_ALPHANUMERIC_VARYING:
	{
		cobol_byte_length = 0;
		memcpy(&cobol_byte_length, field->addr, OCDB_VARCHAR_HEADER_BYTE);
		if(field->type == OCDB_TYPE_JAPANESE_VARYING){
			cobol_byte_length = cobol_byte_length * 2;
		}
		LOG("VARYING-LEN:%d\n",cobol_byte_length);

		resultdata = (char *)calloc(cobol_byte_length + TERMINAL_LENGTH, sizeof(char));
		memcpy(resultdata, (char *)((char *)field->addr + OCDB_VARCHAR_HEADER_BYTE), cobol_byte_length);
		realdata_length = cobol_byte_length;
		break;
	}
	case OCDB_TYPE_JAPANESE:
	case OCDB_TYPE_ALPHANUMERIC:
	default:
	{
		if(field->type == OCDB_TYPE_ALPHANUMERIC){
			cobol_byte_length = field->length;
		} else if(field->type == OCDB_TYPE_JAPANESE){
			cobol_byte_length = field->length * 2;
		} else {
			cobol_byte_length = field->length;
			ERRLOG("Unsupported type\n");
		}
		//LOW-VALUE判定
		if(is_low_value(field->addr, cobol_byte_length)){
			LOG("type:%d coboldata is low-value.\n", field->type);
			resultdata = (char *)calloc(1 + TERMINAL_LENGTH, sizeof(char));
			resultdata[0] = ' ';
			return resultdata;
		}
		resultdata = (char *)calloc(cobol_byte_length + TERMINAL_LENGTH, sizeof(char));
		memcpy(resultdata, field->addr, cobol_byte_length);
		realdata_length = cobol_byte_length;
		break;
	}
	}
	LOG("type:%d #coboldata:%.*s/length:%d#realdata:%s/length:%d\n",
			field->type,
			cobol_byte_length, field->addr,
			field->length,
			resultdata,
			realdata_length);

	return resultdata;
}
