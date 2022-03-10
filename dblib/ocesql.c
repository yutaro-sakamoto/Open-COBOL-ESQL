#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <malloc.h>
#include "ocdblog.h"
#include "ocdbutil.h"
#include "ocesql.h"
#include "ocdb.h"

typedef struct sql_var {
	int type; // set OCDB_TYPE_*
	int length; // size
	int power; // power
	void *addr; // address of variable
	char *realdata; // realdata
} SQLVAR;

typedef struct sql_var_list {
	SQLVAR sv;
	struct sql_var_list *next;
} SQLVARLIST;

struct query_info {
	char *pname;  // default
	char *query;
	int nParams;
};

typedef struct prepare_list {
	struct query_info sq;
	struct prepare_list *next;
} PREPARELIST;

typedef struct cursor_list {
	int connid; // connection id
	char *cname; // default NULL
	struct prepare_list *sp;
	char *query; // default NULL
	int nParams; // params for query
	int isOpened; // open flag
	int tuples; // fetched rows
	SQLVARLIST *plist; // parameter list
	struct cursor_list *next;
} CURSORLIST;



static PREPARELIST _prepare_list = {{NULL, NULL, 0}, NULL};
static CURSORLIST _cursor_list = {0, NULL, NULL, NULL, 0, 0, 0, NULL, NULL};
static SQLVARLIST *_sql_var_lists = NULL;
static SQLVARLIST *_sql_res_var_lists = NULL;
static int _var_lists_length = 0;
static int _res_var_lists_length = 0;
static int _occurs_length = 0;
static int _occurs_iter = 0;
static int _occurs_is_parent = 0;


static void sqlca_initialize(struct sqlca_t *);

/* sql var list */
static void init_sql_var_list(void);
static void reset_sql_var_list(void);
static SQLVARLIST * new_sql_var_list(void);
static SQLVARLIST * add_sql_var_list(int, int, int, void *);
static SQLVARLIST * add_sql_res_var_list(int, int, int, void *);
static void clear_sql_var_list(SQLVARLIST *);
static void show_sql_var_list(SQLVARLIST *);
static PREPARELIST * new_prepare_list(void);
static PREPARELIST * add_prepare_list(char *, char *, int);

static void create_realdata(SQLVAR *, int);
static void create_coboldata_lowvalue(SQLVAR *, int);
static void create_coboldata(SQLVAR *, int, char *);
static int get_varchar_length(char *);
static void set_varchar_length(int, char *);
static int get_sqlvar_byte_length(SQLVAR *);
static int is_group_occurs_param_set(SQLVARLIST *, int);

/* cursor list */
static CURSORLIST *new_cursor_list(void);
static int add_cursor_list(int, char *, char *, int);
static int add_cursor_list_with_prepare(int, char *, PREPARELIST *);
static void remove_cursor_list(char *);

//static CURSORLIST * add_prepared_cursor_list(int, int, int, void *);
static void clear_cursor_list(CURSORLIST *, int);
static void _clear_cursor_list(CURSORLIST *, int);
static void show_cursor_list(CURSORLIST *);
static CURSORLIST * get_cursor_from_list(char *);
static PREPARELIST * get_prepare_from_list(char *);


static void _ocesqlExec(struct sqlca_t *, int , char *);
static void _ocesqlExecParams(struct sqlca_t *, int, char *, int);
static void _ocesqlExecParamsOccurs(struct sqlca_t *, int, char *, int);
static void _ocesqlCursorDeclare(struct sqlca_t *, int, char *, char *, int);
static void _ocesqlPreparedCursorDeclare(struct sqlca_t *, int, char *, char *);
static int _ocesqlExecPrepare(struct sqlca_t *, int, char *, int);
static int _ocesqlExecPrepareOccurs(struct sqlca_t *, int, char *, int);
static void _ocesqlExecSelectIntoOne(struct sqlca_t *, int, char *, int, int);
static void _ocesqlExecSelectIntoOccurs(struct sqlca_t *, int, char *, int, int);
static int _ocesqlConnectMain(struct sqlca_t *, char *, char *, char *, char *);
static int _ocesqlConnect(struct sqlca_t *, char *, int, char *, int, char *, int, char *);
static int _ocesqlConnectInformal(struct sqlca_t *, char *, int, char *);
static void _ocesqlDisconnect(struct sqlca_t *, int); // mada
static void _ocesqlResultStatus(int, struct sqlca_t *);
static void _ocesqlReleaseConnection(int, void *);

static int _ocesqlResolveCONNID(struct sqlca_t *, char *, int);


static void
sqlca_initialize(struct sqlca_t * sqlca){
	memcpy((char *) sqlca, (char *) &sqlca_init, sizeof(struct sqlca_t));
}

/*
 * <Function name>
 *   OCESQLConnect
 *
 * <Outline>
 *   $B%G!<%?%Y!<%9@\B3$r;n$_$k!#@.8y$7$?$i%3%M%/%7%g%s(BID$B$rH/9T$7$FJV$9(B
 *   return $B$9$kA0$K(B st.sqlcode $B$KCM$r%;%C%H$9$k$3$H!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   //@name: database name
 *   //@user: user name
 *   //@passwd: password
 *
 * <Output>
 *   success : ConnectionId
 *   failure ; OCESQL_NO_CONNECTION
 */
int
OCESQLConnect(struct sqlca_t *st, char *user, int userlen, char *passwd, int passwdlen, char *name, int namelen){
	LOG("OCESQLConnect start\n");
	return _ocesqlConnect(st, user, userlen, passwd, passwdlen, name, namelen, NULL);
}

/*
 * <Function name>
 *   OCESQLIDConnect
 *
 * <Outline>
 *   $B%G!<%?%Y!<%9@\B3$r;n$_$k!#0z?t$K@\B3@h>pJs$r;XDj$N=q<0$G%;%C%H$9$k!#(B
 *   $B@.8y$7$?$i%3%M%/%7%g%s(BID$B$rH/9T$7$FJV$9!#(B
 *   return $B$9$kA0$K(B st.sqlcode $B$KCM$r%;%C%H$9$k$3$H!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   //@name: database name
 *   //@user: user name
 *   //@passwd: password
 *
 * <Output>
 *   success : ConnectionId
 *   failure ; OCESQL_NO_CONNECTION
 */
int
OCESQLIDConnect(struct sqlca_t *st, char *atdb, int atdblen, char *user, int userlen, char *passwd, int passwdlen, char *name, int namelen){
	char *atdbbuf;
	LOG("OCESQLIDConnect start\n");
	atdbbuf = get_str_without_after_space(oc_strndup(atdb, atdblen));
	if((!atdbbuf) || (*atdbbuf == '\0')){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	_ocesqlConnect(st, user, userlen, passwd, passwdlen, name, namelen, atdbbuf);

	free(atdbbuf);

	return 0;
}

static int
_ocesqlConnect(struct sqlca_t *st, char *user, int userlen, char *passwd, int passwdlen, char *name, int namelen, char *atdb){
	char *dbuser, *dbpasswd, *dbname;
	char *tmpuser, *tmppasswd, *tmpname;
	int ret;

	tmpname   = get_str_without_after_space(oc_strndup(name, namelen));
	tmpuser   = get_str_without_after_space(oc_strndup(user, userlen));
	tmppasswd = get_str_without_after_space(oc_strndup(passwd, passwdlen));

	if(!tmpname){
		dbname = strdup(ocdb_getenv("OCDB_DB_NAME", NULL));
	}else if(*tmpname == '\0'){
		free(tmpname);
		tmpname = ocdb_getenv("OCDB_DB_NAME", NULL);
		if(tmpname)
			dbname = strdup(tmpname);
		else
			dbname = NULL;
	}else{
		dbname = tmpname;
	}

	if(!tmpuser){
		dbuser = strdup(ocdb_getenv("OCDB_DB_USER", NULL));
	}else if(*tmpuser == '\0'){
		free(tmpuser);
		tmpuser = ocdb_getenv("OCDB_DB_USER", NULL);
		if(tmpuser)
			dbuser = strdup(tmpuser);
		else
			dbuser = NULL;
	}else{
		dbuser = tmpuser;
	}

	if(!tmppasswd){
		dbpasswd = strdup(ocdb_getenv("OCDB_DB_PASS", NULL));
	}else if(*tmppasswd == '\0'){
		free(tmppasswd);
		tmppasswd = ocdb_getenv("OCDB_DB_PASS", NULL);
		if(tmppasswd)
			dbpasswd = strdup(tmppasswd);
		else
			dbpasswd = NULL;
	}else{
		dbpasswd = tmppasswd;
	}

	if(atdb){
		ret = _ocesqlConnectMain(st, dbname, dbuser, dbpasswd, atdb);
	}else{
		ret = _ocesqlConnectMain(st, dbname, dbuser, dbpasswd, OCESQL_DEFAULT_DBNAME);
	}

	if(dbname) free(dbname);
	if(dbuser) free(dbuser);
	if(dbpasswd) free(dbpasswd);

	return ret;
}

/*
 * <Function name>
 *   OCESQLConnectInformal
 *
 * <Outline>
 *   $B%G!<%?%Y!<%9@\B3$r;n$_$k!#0z?t$K@\B3@h>pJs$r;XDj$N=q<0$G%;%C%H$9$k!#(B
 *   $B@.8y$7$?$i%3%M%/%7%g%s(BID$B$rH/9T$7$FJV$9(B
 *   return $B$9$kA0$K(B st.sqlcode $B$KCM$r%;%C%H$9$k$3$H!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @conninfo: Connection Info (USER/PASSWD@DBNAME)
 *   @conninfolen: length of conninfo
 *
 * <Output>
 *   success : ConnectionId
 *   failure ; OCESQL_NO_CONNECTION
 */
int
OCESQLConnectInformal(struct sqlca_t *st, char *conninfo, int conninfolen){
	LOG("OCESQLConnectInformal start\n");
	return _ocesqlConnectInformal(st, conninfo, conninfolen, NULL);
}

/*
 * <Function name>
 *   OCESQLIDConnectInformal
 *
 * <Outline>
 *   $B%G!<%?%Y!<%9@\B3$r;n$_$k!#0z?t$K@\B3@h>pJs$r;XDj$N=q<0$G%;%C%H$9$k!#(B
 *   $B@.8y$7$?$i!"(BATDBNAME$B$H(BCONNECTION ID$B$N%;%C%H$rEPO?$9$k!#(B
 *   return $B$9$kA0$K(B st.sqlcode $B$KCM$r%;%C%H$9$k$3$H!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @conninfo: Connection Info (USER/PASSWD@DBNAME)
 *   @conninfolen: length of conninfo
 *
 * <Output>
 *   success : ConnectionId
 *   failure ; OCESQL_NO_CONNECTION
 */
int
OCESQLIDConnectInformal(struct sqlca_t *st, char *atdb, int atdblen, char *conninfo, int conninfolen){
	LOG("OCESQLIDConnectInformal start\n");
	char *atdbbuf;
	atdbbuf = get_str_without_after_space(oc_strndup(atdb, atdblen));
	if((!atdbbuf) || (*atdbbuf == '\0')){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	_ocesqlConnectInformal(st, conninfo, conninfolen, atdbbuf);

	free(atdbbuf);

	return 0;
}

static int
_ocesqlConnectInformal(struct sqlca_t *st, char *conninfo, int conninfolen, char *atdb){

	char *conninfobuf,*dsn,*passwd;
	char *dbname, *dbuser, *dbpasswd;
	char *tmpname, *tmpuser, *tmppasswd;
	int ret;

	conninfobuf = get_str_without_after_space(oc_strndup(conninfo,conninfolen));
	if(!conninfobuf){
		ERRLOG("Connection information is NULL\n",conninfobuf);
		return -1;
	}
	LOG("%s\n",conninfobuf);
	dsn = strchr(conninfobuf,'@');
	if(dsn){
		*dsn='\0';
		dsn++;
	}

	passwd = strchr(conninfobuf,'/');
	if(passwd){
		*passwd='\0';
		passwd++;
	}


	tmpname   = get_str_without_after_space(dsn);
	tmpuser   = get_str_without_after_space(conninfobuf);
	tmppasswd = get_str_without_after_space(passwd);

	if(!tmpname || *tmpname == '\0'){
		tmpname = ocdb_getenv("OCDB_DB_NAME", NULL);
		if(tmpname)
			dbname = strdup(tmpname);
		else
			dbname = NULL;
	}else{
		dbname = strdup(tmpname);
	}

	if(!tmpuser || *tmpuser == '\0'){
		tmpuser = ocdb_getenv("OCDB_DB_USER", NULL);
		if(tmpuser)
			dbuser = strdup(tmpuser);
		else
			dbuser = NULL;
	}else{
		dbuser = strdup(tmpuser);
	}

	if(!tmppasswd || *tmppasswd == '\0'){
		tmppasswd = ocdb_getenv("OCDB_DB_PASS", NULL);
		if(tmppasswd)
			dbpasswd = strdup(tmppasswd);
		else
			dbpasswd = NULL;
	}else{
		dbpasswd = strdup(tmppasswd);
	}

	if(atdb){
		ret = _ocesqlConnectMain(st, dbname, dbuser, dbpasswd, atdb);
	}else{
		ret = _ocesqlConnectMain(st, dbname, dbuser, dbpasswd, OCESQL_DEFAULT_DBNAME);
	}

	if(conninfobuf) free(conninfobuf);
	if(dbname) free(dbname);
	if(dbuser) free(dbuser);
	if(dbpasswd) free(dbpasswd);

	return ret;
}

static int
_ocesqlConnectMain(struct sqlca_t *st, char *name, char *user, char *passwd, char *conndbname){
	int connectId;
	int autocommit;
	int dbtype = USE_PGSQL; // aiming at a specific target

	char *cencoding = strdup(ocdb_getenv("OCDB_DB_CHAR", "SJIS"));

	char *dbname = name ? _strdup(name) : NULL;
	char *dbhost = NULL;
	char *dbport = NULL;
	char *real_dbname = NULL;
	char *connstr = NULL;
	int connlen = 0;

	LOG("dbname   = %s\n",name);
	LOG("user     = %s\n",user);
	LOG("password = %s\n",passwd);
	LOG("connname = %s\n",conndbname);

	sqlca_initialize(st);

	// dbtype check
	if(OCDBCheckType(dbtype)){
		st->sqlcode = OCDB_CONN_FAIL_CONNECT;
		ERRLOG("dbtype invalid: %d.\n", dbtype);
		return 1;
	}

	int id = OCDBResolveCONNID(conndbname);
	if(id > 0){
		if(OCDBStatus(id) == OCDB_CONN_CONNECT_OK){
			LOG("connection cid %s is already connected.\n", conndbname);
			if (dbname) free(dbname);
			return 0;
		}else{
			OCDBFinish(id);
		}
	}

	if (dbname != NULL){
 		char *tmpstr;

		tmpstr = strrchr(dbname, ':');
		if (tmpstr != NULL){
			dbport = _strdup(tmpstr + 1);
			*tmpstr = '\0';
		}
		tmpstr = strrchr(dbname, '@');
		if (tmpstr != NULL){
			dbhost = _strdup(tmpstr + 1);
			*tmpstr = '\0';
		}

		if(strlen(dbname) > 0){
			real_dbname = _strdup(dbname);
		}
	}

	connlen = strlen_or_null(real_dbname) + strlen_or_null(dbhost) +
		strlen_or_null(dbport) + strlen_or_null(user) +
		strlen_or_null(passwd) + sizeof(" host = port = dbname = user = password =");
	connstr = _alloc(connlen);

	sprintf(connstr, "%s%s %s%s %s%s %s%s %s%s",
			real_dbname ? "dbname=" : "", real_dbname ? real_dbname : "",
			dbhost ? "host=" : "", dbhost ? dbhost : "",
			dbport ? "port=" : "", dbport ? dbport : "",
			(user && strlen(user) > 0) ? "user=" : "", user ? user : "",
			(passwd && strlen(passwd) > 0) ? "password=" : "", passwd ? passwd : "");

	autocommit = OCDB_AUTOCOMMIT_ON;

	connectId = OCDBConnect(dbtype, connstr, conndbname, autocommit, cencoding);
	LOG("publish connectId: %d.\n", connectId);

	if (dbhost) free(dbhost);
	if (dbport) free(dbport);
	if (dbname) free(dbname);
	if (real_dbname) free(real_dbname);
	if (cencoding) free(cencoding);

	if (connectId == OCDB_CONN_FAIL_CONNECT){
		ERRLOG("connection failed. connect params is :%s\n",connstr);
		free(connstr);
		return 1;
	}else if(connectId == INVALID_CONN_ID){
		ERRLOG("create connect data error\n");
		free(connstr);
		return 1;
	}

	free(connstr);

	OCDBExec(connectId, "BEGIN");
	st->sqlcode = OCESQL_CONN_CONNECT_OK;

	LOG("connection success. connectId = %d, dbname = %s\n", connectId, conndbname);
	return 0;
}

/*
 * <Function name>
 *   OCESQLPrepare
 *
 * <Outline>
 *   SQL$BJ8$r=`Hw$9$k(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @sname: SQL name
 *   @query: query
 *   @querylen: length of query
 */
int
OCESQLPrepare(struct sqlca_t *st, char *sname, char *query, int querylen){
	LOG("OCESQLPrepare start\n");
	char *querybuf, *pquery;
	int nParams;

	querybuf = oc_strndup(query, querylen);
	pquery = get_str_replace_hostvalue(querybuf, &nParams);

	if(querybuf)
		free(querybuf);
	LOG("Add prepare: sname:%s, nParams:%d, query:'%s'\n",sname,nParams,pquery);
	if(add_prepare_list(sname, pquery, nParams) ==NULL){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
	}

	return 0;
}

/*
 * <Function name>
 *   OCESQLExec
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExec$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 */
int
OCESQLExec(struct sqlca_t *st, char *query){
	LOG("OCESQLExec start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExec(st, id, query);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDExec
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlExec$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @query: SQL query
 */
int
OCESQLIDExec(struct sqlca_t *st, char *atdb, int atdblen, char *query){
	LOG("OCESQLIDExec start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExec(st, id, query);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlExec
 *
 * <Outline>
 *   SQL$B%/%(%j$N<B9T(B
 *   COMMIT, ROLLBACK $B$N;~$O(B BEGIN $B$b9T$&!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 */
static void
_ocesqlExec(struct sqlca_t *st, int id, char *query){
	sqlca_initialize(st);

	// check argument
	if(query == NULL || strlen(query) == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	OCDBExec(id, query);
	_ocesqlResultStatus(id,st);
	if(st->sqlcode < 0){
		return;
	}

	if(strcmp(query, "COMMIT") == 0|| strcmp(query, "ROLLBACK") == 0){
		clear_cursor_list(&_cursor_list, id);
		OCDBExec(id, "BEGIN");
	}

	return;
}

/*
 * <Function name>
 *   OCESQLExecParams
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecParams$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLExecParams(struct sqlca_t *st, char *query, int nParams){
	LOG("OCESQLExecParams start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecParams(st, id, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDExecParams
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlExecParams$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecParams(struct sqlca_t *st, char *atdb, int atdblen, char *query, int nParams){
	LOG("OCESQLIDExecParams start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	LOG("id=%d\n", id);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecParams(st, id, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlExecParams
 *
 * <Outline>
 *   $BKd$a9~$_%Q%i%a!<%?IU$-(BSQL$B%/%(%j$N<B9T(B
 *   COMMIT, ROLLBACK $B$N;~$O(B BEGIN $B$b9T$&!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
static void
_ocesqlExecParams(struct sqlca_t *st, int id, char *query, int nParams){
	int i;
	char **arr;
	SQLVARLIST *p = _sql_var_lists;

	sqlca_initialize(st);

	// check argument
	if(query == NULL || strlen(query) == 0 || nParams == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}
	arr = NULL;
	if((arr = (char **)malloc(sizeof(char *) * nParams)) == NULL){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
		return;
	}

	SQLVARLIST *p_res = _sql_res_var_lists;
	int inpatambytea = 0;
	int outpatambytea = 0;
	
	int *paramLengths;
	int *paramFormats;
	
	paramLengths = NULL;
	paramFormats = NULL;
	if((paramLengths = (int *)malloc(sizeof(int *) * nParams)) == NULL){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
		return;
	}
	if((paramFormats = (int *)malloc(sizeof(int *) * nParams)) == NULL){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
		return;
	}

	// set parameters
	for(i=0; i<_var_lists_length; i++, p=p->next){
		//LOG("set parameter p->sv.realdata [%s]\n", p->sv.realdata);
		arr[i] = p->sv.realdata;

		paramLengths[i] = p->sv.length;
		if(p->sv.type == OCDB_TYPE_BYTEA) {
			paramFormats[i] = 1;
			inpatambytea = 1;
		} else {
			paramFormats[i] = 0;
		}

	}

	for(i=0; i<_res_var_lists_length; i++, p_res=p_res->next){
		if(p_res->sv.type == OCDB_TYPE_BYTEA) {
			outpatambytea = 1;
		}
	}

	if (outpatambytea == 0) {
		if (inpatambytea == 1) {
			LOG("inpatambytea2 = 1 outpatambytea = 0 \n");
			OCDBExecParams(id, query, nParams, NULL,
				   (const char * const *)arr, (const int *)paramLengths, (const int *)paramFormats, 0);
		} else {
			LOG("inpatambytea2 = 0 outpatambytea = 0 \n");
			OCDBExecParams(id, query, nParams, NULL,
				   (const char * const *)arr, NULL, NULL, 0);
		} 
	} else {
		if (inpatambytea == 1) {
			LOG("inpatambytea2 = 1 outpatambytea = 1 \n");
			OCDBExecParams(id, query, nParams, NULL,
				   (const char * const *)arr, (const int *)paramLengths, (const int *)paramFormats, 1);
		} else {
			LOG("inpatambytea2 = 0 outpatambytea = 1 \n");
			OCDBExecParams(id, query, nParams, NULL,
				   (const char * const *)arr, NULL, NULL, 1);
		}
	}

	_ocesqlResultStatus(id,st);
	if(arr != NULL){
		free(arr);
		free(paramLengths);
		free(paramFormats);
	}
	if(st->sqlcode < 0){
		return;
	}

	if(strcmp(query, "COMMIT") == 0|| strcmp(query, "ROLLBACK") == 0){
		clear_cursor_list(&_cursor_list, id);
		OCDBExec(id, "BEGIN");
	}
	return;
}

/*
 * <Function name>
 *   OCESQLExecParamsOccurs
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecParamsOccurs$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLExecParamsOccurs(struct sqlca_t *st, char *query, int nParams){
	LOG("OCESQLExecParamsOccurs start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecParamsOccurs(st, id, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDExecParamsOccurs
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlExecParamsOccurs$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecParamsOccurs(struct sqlca_t *st, char *atdb, int atdblen, char *query, int nParams){
	LOG("OCESQLIDExecParamsOccurs start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecParamsOccurs(st, id, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlExecParamsOccurs
 *
 * <Outline>
 *   $BKd$a9~$_%Q%i%a!<%?IU$-(BSQL$B%/%(%j$N<B9T(B
 *   $B%Q%i%a!<%?$O$O(BSetHostTable$B$GDj5A$7$?7+$jJV$72s?t$H0l<~$N%P%$%H?t$K(B
 *   $B=>$$%;%C%H$5$l$k(B
 *   $BBP1~$9$k(BSQL$B$O(BINSERT$BJ8(B,UPDATE$BJ8(B,DELETE$BJ8$N$_!#(B
 *
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
static void
_ocesqlExecParamsOccurs(struct sqlca_t *st, int id, char *query, int nParams){
	int i, index;
	char **arr;
	SQLVARLIST *p;

	sqlca_initialize(st);

	// check argument
	if(query == NULL || strlen(query) == 0 || nParams == 0 || _occurs_iter > OCDB_OCCURS_MAX_TIMES){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}
	arr = NULL;
	if((arr = (char **)malloc(sizeof(char *) * nParams)) == NULL){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
		return;
	}

	for(index=0; index<_occurs_iter; index++){
		// set parameters
		p = _sql_var_lists;

		// check
		if(is_group_occurs_param_set(_sql_var_lists, index) == RESULT_SUCCESS){
			for(i=0; i<_var_lists_length; i++, p=p->next){
				//LOG("set parameter p->sv.realdata [%s]\n", p->sv.realdata);
				create_realdata(&p->sv, index);
				arr[i] = p->sv.realdata;
			}

			OCDBExecParams(id, query, nParams, NULL,
					(const char * const *)arr, NULL, NULL, 0);
			_ocesqlResultStatus(id,st);
			if(st->sqlcode < 0){
				break;
			}
		}
	}
	if(arr != NULL){
		free(arr);
	}

	return;
}

/*
 * <Function name>
 *   OCESQLCursorDeclare
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlCursorDeclare$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: cursor name
 *   @query: SQL query
 */
int
OCESQLCursorDeclare(struct sqlca_t *st, char *cname, char *query){
	LOG("OCESQLCursorDeclare start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlCursorDeclare(st, id, cname, query, 0);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDCursorDeclare
 *
 * <Outline>
 *   $B%+!<%=%k@k8@(B
 *   CONNECTION ID$B$NBX$o$j$K@\B3;R$rMxMQ$9$k!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @cname: cursor name
 *   @query: SQL query
 */
int
OCESQLIDCursorDeclare(struct sqlca_t *st, char *atdb, int atdblen, char *cname, char *query){
	LOG("OCESQLIDCursorDeclare start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlCursorDeclare(st, id, cname, query, 0);
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorDeclareParams
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlCursorDeclareParams$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: cursor name
 *   @query: SQL query
 */
int
OCESQLCursorDeclareParams(struct sqlca_t *st, char *cname, char *query, int nParams){
	LOG("OCESQLCursorDeclareParams start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	// check argument
	if(nParams == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	_ocesqlCursorDeclare(st, id, cname, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorDeclareParams
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlCursorDeclareParams$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDCursorDeclareParams(struct sqlca_t *st, char *atdb, int atdblen, char *cname, char *query, int nParams){
	LOG("OCESQLIDCursorDeclareParams start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	// check argument
	if(nParams == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}
	_ocesqlCursorDeclare(st, id, cname, query, nParams);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlCursorDeclareParams
 *
 * <Outline>
 *   $BKd$a9~$_%Q%i%a!<%?IU$-%+!<%=%k@k8@(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: Cursor name
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
static void
_ocesqlCursorDeclare(struct sqlca_t *st, int id, char *cname, char *query, int nParams){
	int res;

	sqlca_initialize(st);

	// check argument
	if(cname == NULL || strlen(cname) == 0 ||
	   query == NULL || strlen(query) == 0 ){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	res = add_cursor_list(id, cname, query, nParams);

	if(res == RESULT_FAILED){
		st->sqlcode = OCDB_RES_OVERWRITE_OPENED_CURSOR;
	}else if(res == RESULT_ERROR){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
	}
	return;
}

/*
 * <Function name>
 *   OCESQLPreparedCursorDeclare
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlPreparedCursorDeclare$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: Cursor name
 *   @sname: Prepared SQLID
 */
int
OCESQLPreparedCursorDeclare(struct sqlca_t *st, char *cname, char *sname){
	LOG("OCESQLPreparedCursorDeclare start\n");
	int id;
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlPreparedCursorDeclare(st, id, cname, sname);
	return 0;
}


/*
 * <Function name>
 *   OCESQLIDPreparedCursorDeclare
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlPreparedCursorDeclare$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @cname: Cursor name
 *   @sname: Prepared SQLID
 */
int
OCESQLIDPreparedCursorDeclare(struct sqlca_t *st, char *atdb, int atdblen, char *cname, char *sname){
	LOG("OCESQLIDPreparedCursorDeclare start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlPreparedCursorDeclare(st, id, cname, sname);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlPreparedCursorDeclare
 *
 * <Outline>
 *   $BKd$a9~$_%Q%i%a!<%?IU$-%+!<%=%k@k8@(B
 *   SQL$BJ8$K$O(BOCESQLPrepare$B$K$FDj5A$5$l$?<1JL;R$r;H$&!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: Cursor name
 *   @sname: Prepared SQLID
 */
static void
_ocesqlPreparedCursorDeclare(struct sqlca_t *st, int id, char *cname, char *sname){
	int res;
	PREPARELIST *prepare;

	sqlca_initialize(st);

	// check argument
	if(cname == NULL || strlen(cname) == 0 ||
	   sname == NULL || strlen(sname) == 0 ){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	LOG("Declare Cursor with prepare: id:%d, cname:%s, sname:%s\n",id,cname,sname);

	// search prepare
	prepare = get_prepare_from_list(sname);
	if(prepare == NULL){
		ERRLOG("prepare %s not registered.\n", sname);
					st->sqlcode = OCDB_RES_UNDEFINED_PREPARE;
		return;
	}

	if((res = add_cursor_list_with_prepare(id, cname, prepare)) == RESULT_FAILED){
		st->sqlcode = OCDB_RES_OVERWRITE_OPENED_CURSOR;
	}else if((res = add_cursor_list_with_prepare(id, cname, prepare)) == RESULT_ERROR){
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
	}
}

/*
 * <Function name>
 *   OCESQLExecPrepare
 *
 * <Outline>
 *   OCESQLPrepare$BJ8$G=`Hw$5$l$?(BSQL$B$r<B9T$9$k!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @sname: Prepared SQLID
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */

int
OCESQLExecPrepare(struct sqlca_t *st, char *sname, int nParams){
	LOG("OCESQLExecPrepared start\n");
	int id;
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, strlen(OCESQL_DEFAULT_DBNAME));
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}

	return _ocesqlExecPrepare(st, id, sname, nParams);
}

/*
 * <Function name>
 *   OCESQLIDExecPrepare
 *
 * <Outline>
 *   OCESQLPrepare$BJ8$G=`Hw$5$l$?(BSQL$B$r<B9T$9$k!#(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @sname: Prepared SQLID
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecPrepare(struct sqlca_t *st, char *atdb, int atdblen, char *sname, int nParams){
	LOG("OCESQLExecPrepared start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}

	return _ocesqlExecPrepare(st, id, sname, nParams);
}


static int
_ocesqlExecPrepare(struct sqlca_t *st, int id, char *sname, int nParams){
	int i;
	char **arr;
	char *query;
	SQLVARLIST *p = _sql_var_lists;
	PREPARELIST *prepare;

	sqlca_initialize(st);

	// search prepare
	prepare = get_prepare_from_list(sname);
	if(prepare == NULL){
		ERRLOG("prepare %s not registered.\n", sname);
		st->sqlcode = OCDB_RES_UNDEFINED_PREPARE;
		return 1;
	}

	query = prepare->sq.query;

	// check argument
	if(query == NULL || strlen(query) == 0 || nParams != prepare->sq.nParams){
		ERRLOG("prepare %s argument error.\n", sname);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	if(nParams > 0){
		if(prepare->sq.nParams != nParams){
			ERRLOG("A number of parameters(%d) and prepared sql parameters(%d) is unmatch.\n",nParams,prepare->sq.nParams);
					st->sqlcode =  OCDB_RES_ARGUMENT_ERROR;
					strcpy(st->sqlerrm.sqlerrmc,"A number of parameters and prepared sql parameters is unmatch.");
					st->sqlerrm.sqlerrml = strlen("A number of parameters and prepared sql parameters is unmatch.");
					return 1;
		}
		if((arr = (char **)malloc(sizeof(char *) * nParams)) == NULL){
			st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
			return 1;
		}

		// set parameters
		for(i=0; i<_var_lists_length; i++, p=p->next){
			//LOG("set parameter p->sv.realdata [%s]\n", p->sv.realdata);
			arr[i] = p->sv.realdata;
		}

		OCDBExecParams(id, query, nParams, NULL,
				(const char * const *)arr, NULL, NULL, 0);
		free(arr);
	}else{
		OCDBExec(id, query);
	}

	_ocesqlResultStatus(id,st);
	if(st->sqlcode < 0){
		return 1;
	}

	if(strcmp(query, "COMMIT") == 0|| strcmp(query, "ROLLBACK") == 0){
		clear_cursor_list(&_cursor_list, id);
		OCDBExec(id, "BEGIN");
	}
	return 0;
}

/*
 * <Function name>
 *   OCESQLExecPrepareOccurs
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecPrepareOccurs$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @sname: Prepared SQLID
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */

int
OCESQLExecPrepareOccurs(struct sqlca_t *st, char *sname, int nParams){
	LOG("OCESQLExecPrepared start\n");
	int id;
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, strlen(OCESQL_DEFAULT_DBNAME));
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}

	return _ocesqlExecPrepareOccurs(st, id, sname, nParams);
}

/*
 * <Function name>
 *   OCESQLIDExecPrepare
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecPrepareOccurs$B$K$F(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @sname: Prepared SQLID
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecPrepareOccurs(struct sqlca_t *st, char *atdb, int atdblen, char *sname, int nParams){
	LOG("OCESQLExecPrepared start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}

	return _ocesqlExecPrepareOccurs(st, id, sname, nParams);
}

/*
 * <Function name>
 *   _ocesqlExecPrepareOccurs
 *
 * <Outline>
 *   $BKd$a9~$_%Q%i%a!<%?IU$-(BSQL$B%/%(%j$N<B9T(B
 *   $B%Q%i%a!<%?$O$O(BSetHostTable$B$GDj5A$7$?7+$jJV$72s?t$H0l<~$N%P%$%H?t$K(B
 *   $B=>$$%;%C%H$5$l$k(B
 *
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @sname: Prepared SQLID
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
static int
_ocesqlExecPrepareOccurs(struct sqlca_t *st, int id, char *sname, int nParams){
	int i, index;
	char **arr;
	char *query;
	SQLVARLIST *p;
	PREPARELIST *prepare;

	sqlca_initialize(st);

	// search prepare
	prepare = get_prepare_from_list(sname);
	if(prepare == NULL){
		ERRLOG("prepare %s not registered.\n", sname);
		st->sqlcode = OCDB_RES_UNDEFINED_PREPARE;
		return 1;
	}

	query = prepare->sq.query;

	// check argument
	if(query == NULL || strlen(query) == 0 || _occurs_iter > OCDB_OCCURS_MAX_TIMES){
		ERRLOG("prepare %s argument error.\n", sname);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	if(nParams > 0){
		if(prepare->sq.nParams != nParams){
			ERRLOG("A number of parameters(%d) and prepared sql parameters(%d) is unmatch.\n",nParams,prepare->sq.nParams);
					st->sqlcode =  OCDB_RES_ARGUMENT_ERROR;
					strcpy(st->sqlerrm.sqlerrmc,"A number of parameters and prepared sql parameters is unmatch.");
					st->sqlerrm.sqlerrml = strlen("A number of parameters and prepared sql parameters is unmatch.");
					return 1;
		}
		if((arr = (char **)malloc(sizeof(char *) * nParams)) == NULL){
			st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
			return 1;
		}

		for(index=0; index<_occurs_iter; index++){
			// set parameters
			p = _sql_var_lists;

			// check
			if(is_group_occurs_param_set(_sql_var_lists, index) == RESULT_SUCCESS){
				for(i=0; i<_var_lists_length; i++, p=p->next){
					//LOG("set parameter p->sv.realdata [%s][index=%d]\n", p->sv.realdata, index);
					create_realdata(&p->sv, index);
					arr[i] = p->sv.realdata;
				}
			
				OCDBExecParams(id, query, nParams, NULL,
					       (const char * const *)arr, NULL, NULL, 0);
				_ocesqlResultStatus(id,st);
				if(st->sqlcode < 0){
					break;
				}
			}
		}
		if(arr != NULL){
			free(arr);
		}
	}else{
		OCDBExec(id, query);
		_ocesqlResultStatus(id,st);
	}

	if(st->sqlcode < 0){
		return 1;
	}

	if(strcmp(query, "COMMIT") == 0|| strcmp(query, "ROLLBACK") == 0){
		clear_cursor_list(&_cursor_list, id);
		OCDBExec(id, "BEGIN");
	}
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorOpen
 *
 * <Outline>
 *   $B%+!<%=%k%*!<%W%s(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @cname: cursor name
 */
int
OCESQLCursorOpen(struct sqlca_t *st, char *cname){
	LOG("OCESQLCursorOpen start\n");
	CURSORLIST *cursor;

	sqlca_initialize(st);

	// check argument
	LOG("cname=#%s#\n", cname);
	if(cname == NULL || strlen(cname) == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	// search cursor
	cursor = get_cursor_from_list(cname);
	if(cursor == NULL){
		ERRLOG("cursor %s not registered.\n", cname);
		st->sqlcode = OCDB_RES_UNDEFINED_CURSOR;
		return 1;
	}

	if(cursor->isOpened){
		LOG("cursor %s alredy opened.\n", cname);
		OCDBCursorClose(cursor->connid, cname);
		_ocesqlResultStatus(cursor->connid,st);
		if(st->sqlcode < 0){
			ERRLOG("cursor %s close failed.\n", cname);
			return 1;
		}
		cursor->isOpened = 0;
	}

	// DECLARE CURSOR
	if(cursor->nParams > 0){
		int i;
		char **arr;
		SQLVARLIST *p = cursor->plist;

		if((arr = (char **)malloc(sizeof(char *) * cursor->nParams)) == NULL){
			ERRLOG("memory allocation failed.\n");
			st->sqlcode =OCDB_RES_ALLOCATE_ERROR;
			return 1;
		}

		// set parameters
		for(i=0; i<cursor->nParams; i++, p=p->next){
			//LOG("set parameter p->sv.realdata [%s]\n", p->sv.realdata);
			create_realdata(&p->sv,0);
			arr[i] = p->sv.realdata;
			LOG("params[%d]:#%s#\n",i, p->sv.realdata);
		}

		OCDBCursorDeclareParams(cursor->connid, cursor->cname, cursor->query,
					cursor->nParams, NULL, (const char * const *)arr,
					NULL, NULL, 0, OCDB_CURSOR_WITH_HOLD_OFF);
		free(arr);
	} else if(cursor->sp){
		LOG("with prepare: sname:%s, query:%s\n",cursor->sp->sq.pname, cursor->sp->sq.query);
		OCDBCursorDeclare(cursor->connid, cursor->cname,
				cursor->sp->sq.query, OCDB_CURSOR_WITH_HOLD_OFF);
	}else {
		OCDBCursorDeclare(cursor->connid, cursor->cname,
				  cursor->query, OCDB_CURSOR_WITH_HOLD_OFF);
	}
	_ocesqlResultStatus(cursor->connid,st);
	if(st->sqlcode < 0){
		return 1;
	}

	// OPEN CURSOR
	OCDBCursorOpen(cursor->connid, cursor->cname);
	_ocesqlResultStatus(cursor->connid,st);
	if(st->sqlcode < 0){
		return 1;
	}
	cursor->isOpened = 1;
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorOpenParams -- $B;H$o$J$$$+$b(B
 *
 * <Outline>
 *   $B%+!<%=%k%*!<%W%s(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @cname: cursor name
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLCursorOpenParams(struct sqlca_t *st, char *cname, int nParams){
	CURSORLIST *cursor;
	LOG("OCESQLCursorOpenParams start\n");
	sqlca_initialize(st);

	// check argument
	LOG("cname=#%s#\n", cname);
	if(cname == NULL || strlen(cname) == 0){

		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}

	// search cursor
	cursor = get_cursor_from_list(cname);
	if(cursor == NULL){
		ERRLOG("cursor %s not registered.\n", cname);
		st->sqlcode = OCDB_RES_UNDEFINED_CURSOR;
		return 1;
	}
	if(cursor->sp == NULL){
		ERRLOG("prepare sql in cursor '%s' not registered.\n", cname);
		st->sqlcode = OCDB_RES_UNDEFINED_PREPARE;
		return 1;
	}
	if(cursor->sp->sq.nParams != nParams){
		ERRLOG("A number of parameters(%d) and prepared sql parameters(%d) is unmatch.\n",nParams,cursor->sp->sq.nParams);
		st->sqlcode =  OCDB_RES_ARGUMENT_ERROR;
		strcpy(st->sqlerrm.sqlerrmc,"A number of parameters and prepared sql parameters is unmatch.");
		st->sqlerrm.sqlerrml = strlen("A number of parameters and prepared sql parameters is unmatch.");
		return 1;
	}
	if(cursor->isOpened){
		LOG("cursor %s alredy opened.\n", cname);
		OCDBCursorClose(cursor->connid, cname);
		_ocesqlResultStatus(cursor->connid,st);
		if(st->sqlcode < 0){
			ERRLOG("cursor %s close failed.\n", cname);
			strncpy(st->sqlerrm.sqlerrmc, OCDBResultErrorMessage(cursor->connid), SQLERRMC_LEN - 1);
			return 1;
		}
		cursor->isOpened = 0;
	}

	int i;
	char **arr;
	SQLVARLIST *p = _sql_var_lists;

	if((arr = (char **)malloc(sizeof(char *) * nParams)) == NULL){
		ERRLOG("memory allocation failed.\n");
		st->sqlcode = OCDB_RES_ALLOCATE_ERROR;
		return 1;
	}

	// set parameters
	for(i=0; i<_var_lists_length; i++, p=p->next){
		//LOG("set parameter p->sv.realdata [%s]\n", p->sv.realdata);
		arr[i] = p->sv.realdata;
		LOG("params[%d]:#%s#\n",i, p->sv.realdata);
	}
	LOG("with prepare: sname:%s, query:%s\n",cursor->sp->sq.pname, cursor->sp->sq.query);
	OCDBCursorDeclareParams(cursor->connid, cursor->cname, cursor->sp->sq.query,
			cursor->sp->sq.nParams, NULL, (const char * const *)arr,
					NULL, NULL, 0, OCDB_CURSOR_WITH_HOLD_OFF);
	_ocesqlResultStatus(cursor->connid,st);
	free(arr);
	if(st->sqlcode < 0){
		return 1;
	}

	// OPEN CURSOR
	OCDBCursorOpen(cursor->connid, cursor->cname);
	_ocesqlResultStatus(cursor->connid,st);
	if(st->sqlcode < 0){
		return 1;
	}
	cursor->isOpened = 1;
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorFetchOne
 *
 * <Outline>
 *   $B%+!<%=%k%U%'%C%A(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *   OCDBGetValue$B$GJV$5$l$kCM$O:85M$a$K$J$C$F$$$k$3$H$rA0Ds$H$7$F$$$k!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: cursor name
 */
int
OCESQLCursorFetchOne(struct sqlca_t *st, char *cname){
 	LOG("OCESQLCursorFetchOne start\n");
	int i;
	int id;
	CURSORLIST *cursor;
	SQLVARLIST *p = _sql_res_var_lists;

	sqlca_initialize(st);

	// check argument
	if(cname == NULL || strlen(cname) == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}
	LOG("cname:%s\n",cname);
	cursor = get_cursor_from_list(cname);
	if(cursor == NULL){
		ERRLOG("cursor %s not registered.\n", cname);
		st->sqlcode =  OCDB_RES_UNDEFINED_CURSOR;
		return 1;
	}
	id = cursor->connid;

	// exec sql
	OCDBCursorFetchOne(id, cname, OCDB_READ_NEXT);
	_ocesqlResultStatus(id,st);
	if(st->sqlcode < 0){
		return 1;
	}

	if(OCDBNfields(id) != _res_var_lists_length){
		ERRLOG("A number of parameters(%d) and results(%d) is unmatch.\n",_res_var_lists_length,OCDBNfields(id));
		st->sqlcode =  OCDB_RES_ARGUMENT_ERROR;
		strcpy(st->sqlerrm.sqlerrmc,"A number of parameters and results is unmatch.");
		st->sqlerrm.sqlerrml = strlen("A number of parameters and results is unmatch.");
		return 1;
	}

	// check numtuples
	if(OCDBNtuples(id) < 1){
		st->sqlcode = OCDB_RES_TUPLES_NODATA;
		st->sqlerrd[2] = cursor->tuples;
		LOG("TUPLES NODATA\n");
		return 1;
	} else {
		// set params
		char *retstr;
		for(i=0; i< _res_var_lists_length; i++, p = p->next){
			//LOG("var_list(%d) %d %d %d\n", i, p->sv.type, p->sv.length, p->sv.addr);
			retstr = OCDBGetvalue(id, 0, i);
			//LOG("retstr [%s]\n", retstr);
			create_coboldata(&p->sv, 0, retstr);
		}
	}
	cursor->tuples += st->sqlerrd[2];
	st->sqlerrd[2] = cursor->tuples;
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorFetchOccurs
 *
 * <Outline>
 *   $B%+!<%=%k%U%'%C%A(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *   OCDBGetValue$B$GJV$5$l$kCM$O:85M$a$K$J$C$F$$$k$3$H$rA0Ds$H$7$F$$$k!#(B
 *
 *   $B%U%'%C%A$7$?CM$O(BSetHostTable$B$GDj5A$7$?7+$jJV$72s?t$H0l<~$N%P%$%H?t$K(B
 *   $B=>$$%;%C%H$5$l$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: cursor name
 */
int
OCESQLCursorFetchOccurs(struct sqlca_t *st, char *cname){
	LOG("OCESQLCursorFetchOccurs start\n");
	int i,j;
	int id;
	int tuples = 0;
	int fields = 0;
	CURSORLIST *cursor;
	SQLVARLIST *p = _sql_res_var_lists;

	sqlca_initialize(st);

	// check argument
	if(cname == NULL || strlen(cname) == 0 || _occurs_iter > OCDB_OCCURS_MAX_TIMES){
		ERRLOG("_occurs_iter:%d\n", _occurs_iter);
		ERRLOG("cname:%s\n", cname);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}
	LOG("cname:%s\n",cname);
	cursor = get_cursor_from_list(cname);
	if(cursor == NULL){
		ERRLOG("cursor %s not registered.\n", cname);
		st->sqlcode = OCDB_RES_UNDEFINED_CURSOR;
		return 1;
	}
	id = cursor->connid;

	// exec sql
	OCDBCursorFetchOccurs(id, cname, OCDB_READ_NEXT, _occurs_iter);

	if(st->sqlcode < 0){
		return 1;
	}

	fields = OCDBNfields(id);

	if(fields != _res_var_lists_length){
		ERRLOG("A number of parameters(%d) and results(%d) is unmatch.\n",_res_var_lists_length, fields);
		st->sqlcode =  OCDB_RES_ARGUMENT_ERROR;
		strcpy(st->sqlerrm.sqlerrmc,"A number of parameters and results is unmatch.");
		st->sqlerrm.sqlerrml = strlen("A number of parameters and results is unmatch.");
		return 1;
	}


	tuples = OCDBNtuples(id);
	if(tuples < 1){
		st->sqlcode = OCDB_RES_TUPLES_NODATA;
		LOG("TUPLES NODATA\n");
		return 0;
	}
	if(tuples > _occurs_iter){
		LOG("Data was taken more than a specified. OCCURS:%d, TUPLES:%d.\n",_occurs_iter,tuples);
		tuples = _occurs_iter;
	}
	for(j=0; j<tuples; j++){
		SQLVARLIST *p = _sql_res_var_lists;
		// set params
		char *retstr;
		for(i=0; i< _res_var_lists_length; i++, p = p->next){
			if(i>=fields)
				break;
			//LOG("var_list(%d) %d %d %d\n", i, p->sv.type, p->sv.length, p->sv.addr);
			retstr = OCDBGetvalue(id, j, i);
			//LOG("retstr [%s]\n", retstr);
			create_coboldata(&p->sv, j, retstr);
		}
		cursor->tuples += 1;
	}

	if(tuples < _occurs_iter){
		for(;j<_occurs_iter;j++){
			SQLVARLIST *p = _sql_res_var_lists;
			// set zero
			for(i=0; i< _res_var_lists_length; i++, p = p->next){
				create_coboldata_lowvalue(&p->sv, j);
			}
		}

	}

	st->sqlerrd[2] = cursor->tuples;
	return 0;
}

/*
 * <Function name>
 *   OCESQLCursorClose
 *
 * <Outline>
 *   $B%+!<%=%k%/%m!<%:(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @cname: cursor name
 */
int
OCESQLCursorClose(struct sqlca_t *st, char *cname){
 	LOG("OCESQLCursorClose start\n");
	CURSORLIST *cursor;
	int id;

	sqlca_initialize(st);

	// check argument
	if(cname == NULL || strlen(cname) == 0){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return 1;
	}
	LOG("Cursor Name:%s\n",cname);

	cursor = get_cursor_from_list(cname);
	if(cursor == NULL){
		ERRLOG("cursor %s not registered.\n", cname);
		st->sqlcode = OCDB_RES_UNDEFINED_CURSOR;
		return 1;
	}
	if(!cursor->isOpened){
		LOG("cursor %s not opened.\n", cname);
		//remove_cursor_list(cname);
		return 0;
	}

	id = cursor->connid;
	LOG("Connect ID:%d\n",id);

	OCDBCursorClose(id, cname);
	_ocesqlResultStatus(id,st);
	if(st->sqlcode < 0){
		return 1;
	}

	cursor->isOpened = 0;

	return 0;
}

/*
 * <Function name>
 *   OCESQLExecSelectIntoOne
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecSelectIntoOne$B$G(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLExecSelectIntoOne(struct sqlca_t *st, char *query, int nParams, int nResParams){
	LOG("OCESQLExecSelectIntoOne start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecSelectIntoOne(st, id, query, nParams, nResParams);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDExecSelectIntoOne
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlExecSelectIntoOne$B$G(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecSelectIntoOne(struct sqlca_t *st, char *atdb, int atdblen,
						  char *query, int nParams, int nResParams){
	LOG("OCESQLIDExecSelectIntoOne start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecSelectIntoOne(st, id, query, nParams, nResParams);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlExecSelectIntoOne
 *
 * <Outline>
 *   SELECT INTO $B$N<B9T(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *   OCDBGetValue$B$GJV$5$l$kCM$O:85M$a$K$J$C$F$$$k$3$H$rA0Ds$H$7$F$$$k!#(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
static void
_ocesqlExecSelectIntoOne(struct sqlca_t *st, int id, char *query, int nParams, int nResParams){

	int i;
	int fields = 0;
	SQLVARLIST *p = _sql_res_var_lists;

	// check argument
	if(query == NULL || strlen(query) == 0){
		ERRLOG("ARGUMENT ERROR\n");
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	if(nParams > 0){
		_ocesqlExecParams(st, id, query, nParams);
	}else{
		_ocesqlExec(st, id, query);
	}

	if(st->sqlcode < 0){
		return;
	}

	fields = OCDBNfields(id);
	if(fields != nResParams){
		ERRLOG("ResParams(%d) and fields(%d) are different\n",nResParams, fields);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	// check numtuples
	if(OCDBNtuples(id) < 1){
		st->sqlcode = OCDB_RES_TUPLES_NODATA;
		LOG("TUPLES NODATA\n");
	} else {
		// set params
		char *retstr;
		for(i=0; i< _res_var_lists_length; i++, p = p->next){
			if(i>=fields)
				break;
			//LOG("var_list(%d) %d %d %d\n", i, p->sv.type, p->sv.length, p->sv.addr);
			retstr = OCDBGetvalue(id, 0, i);
			//LOG("retstr [%s]\n", retstr);
			create_coboldata(&p->sv, 0, retstr);
		}
	}
	return;
}

/*
 * <Function name>
 *   OCESQLExecSelectIntoOccurs
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlExecSelectIntoOccurs$B$G(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLExecSelectIntoOccurs(struct sqlca_t *st, char *query, int nParams, int nResParams){
	LOG("OCESQLExecSelectIntoOccurs start\n");
	int id;
	LOG("SQL:#%s#\n", query);
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecSelectIntoOccurs(st, id, query, nParams, nResParams);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDExecSelectIntoOccurs
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlExecSelectIntoOccurs$B$G(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
int
OCESQLIDExecSelectIntoOccurs(struct sqlca_t *st, char *atdb, int atdblen, char *query, int nParams, int nResParams){
 	LOG("OCESQLIDExecSelectIntoOccurs start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlExecSelectIntoOccurs(st, id, query, nParams, nResParams);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlExecSelectIntoOccurs
 *
 * <Outline>
 *   SELECT INTO $B$N<B9T(B
 *   $B<B9T7k2L(B(OCDB_RES_* -> ocdb.h $B;2>H(B)$B$O(BSQLCA$B$K3JG<$9$k(B
 *   OCDBGetValue$B$GJV$5$l$kCM$O:85M$a$K$J$C$F$$$k$3$H$rA0Ds$H$7$F$$$k!#(B
 *
 *   SELECT$B$7$?CM$O(BSetHostTable$B$GDj5A$7$?7+$jJV$72s?t$H0l<~$N%P%$%H?t$K(B
 *   $B=>$$%;%C%H$5$l$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *   @query: SQL query
 *   @nParams: $B%Q%i%a!<%?$N8D?t(B
 *   @nResParams: $B%Q%i%a!<%?$N8D?t(B
 */
static void
_ocesqlExecSelectIntoOccurs(struct sqlca_t *st, int id, char *query, int nParams, int nResParams){

	int i, j;
	int tuples = 0;
	int fields = 0;
	SQLVARLIST *p = _sql_res_var_lists;


	// check argument
	if(query == NULL || strlen(query) == 0 || _occurs_iter > OCDB_OCCURS_MAX_TIMES){
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	if(nParams > 0){
		_ocesqlExecParams(st, id, query, nParams);
	}else{
		_ocesqlExec(st, id, query);
	}

	if(st->sqlcode < 0){
		return;
	}

	fields = OCDBNfields(id);
	if(fields != nResParams){
		ERRLOG("ResParams(%d) and fields(%d) are different\n",nResParams, fields);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		return;
	}

	tuples = OCDBNtuples(id);
	if(tuples < 1){
		st->sqlcode = OCDB_RES_TUPLES_NODATA;
		LOG("TUPLES NODATA\n");
		return;
	}
	if(tuples > _occurs_iter){
		tuples = _occurs_iter;
	}
	for(j=0; j<tuples; j++){
		SQLVARLIST *p = _sql_res_var_lists;
		// set params
		char *retstr;
		for(i=0; i< _res_var_lists_length; i++, p = p->next){
			if(i>=fields)
				break;
			//LOG("var_list(%d) %d %d %d\n", i, p->sv.type, p->sv.length, p->sv.addr);
			retstr = OCDBGetvalue(id, j, i);
			//LOG("retstr [%s]\n", retstr);
			create_coboldata(&p->sv, j, retstr);
		}
	}

	if(tuples < _occurs_iter){
		for(;j<_occurs_iter;j++){
			SQLVARLIST *p = _sql_res_var_lists;
			// set zero
			for(i=0; i< _res_var_lists_length; i++, p = p->next){
				create_coboldata_lowvalue(&p->sv, j);
			}
		}

	}
	return;
}

/*
 * <Function name>
 *   OCESQLNtuples
 *
 * <Outline>
 *   $B7k2L%j%=!<%9$+$i9T?t$r<hF@$9$k(B
 *
 * <Input>
 *   @id: ConnectionId
 *
 * <Output>
 *   success: $B9T?t(B
 *   failure: OCDB_INVALID_NUMBER
 */
int
OCESQLNtuples(int id){
	return OCDBNtuples(id);
}

/*
 * <Function name>
 *   OCESQLNfields
 *
 * <Outline>
 *   $B7k2L%j%=!<%9$+$i%U%#!<%k%I?t$r<hF@$9$k(B
 *
 * <Input>
 *   @id: ConnectionId
 *
 * <Output>
 *   success: $B%U%#!<%k%I?t(B
 *   failure: OCDB_INVALID_NUMBER
 */
int
OCESQLNfields(int id){
	return OCDBNfields(id);
}

/*
 * <Function name>
 *   OCESQLGetvalue
 *
 * <Outline>
 *   $B7k2L%j%=!<%9$N(B1$B9TL\$N;XDj$7$?%U%#!<%k%I$+$i%G!<%?$rJ8;zNs7?$G<hF@$9$k(B
 *
 * <Input>
 *   @id: ConnectionId
 *   @index: $B%U%#!<%k%IHV9f(B(0$B$+$i%9%?!<%H(B)
 *
 * <Output>
 *   success: $B%G!<%?J8;zNs(B
 *   failure: OCDB_INVALID_NUMBER
 */
char *
OCESQLGetvalue(int id, int index){
	return OCDBGetvalue(id, 0, index);
}

/*
 * <Function name>
 *   OCESQLResultErrorMessage
 *
 * <Outline>
 *   SQLCA$B$+$i%(%i!<%a%C%;!<%8$r<hF@$9$k(B
 *
 * <Input>
 *   @st: SQLCA
 *
 * <Output>
 *   success: $B%(%i!<%a%C%;!<%8(B
 *   failure: NULL
 */
char *
OCESQLResultErrorMessage(struct sqlca_t *st){
	return st->sqlerrm.sqlerrmc;
}

/*
 * <Function name>
 *   OCESQLDisconnect
 *
 * <Outline>
 *   $B<B=hM}$O(B_ocesqlDisconnect
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 */
int
OCESQLDisconnect(struct sqlca_t *st){
	LOG("OCESQLDisconnect start\n");
	int id;
	id = _ocesqlResolveCONNID(st, OCESQL_DEFAULT_DBNAME, OCESQL_DEFAULT_DBLENG);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlDisconnect(st, id);
	return 0;
}

/*
 * <Function name>
 *   OCESQLIDDisconnect
 *
 * <Outline>
 *   $B$3$3$G$O@\B3(BID$B<hF@$N$_$r<B;\(B
 *   $B<B=hM}$O(B_ocesqlDisconnect
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @atdb: Connection Identifier
 *   @atdblen: length of atdb
 */
int
OCESQLIDDisconnect(struct sqlca_t *st, char *atdb, int atdblen){
 	LOG("OCESQLIDDisconnect start\n");
	int id;
	id = _ocesqlResolveCONNID(st, atdb, atdblen);
	if(id == RESULT_FAILED){
		ERRLOG("connection id is not found.\n");
		st->sqlcode = OCDB_RES_UNDEFINED_CONNECTION;
		return 1;
	}
	_ocesqlDisconnect(st, id);
	return 0;
}

/*
 * <Function name>
 *   _ocesqlDisonnect
 *
 * <Outline>
 *   $B%G!<%?%Y!<%9$+$i@ZCG$9$k(B
 *
 * <Input>
 *   @st: SQLCA pointer
 *   @id: ConnectionId
 *
 */
static void
_ocesqlDisconnect(struct sqlca_t *st, int id){
	OCDBFinish(id);
	//LOG("disconnect id=%d\n", id);
	//LOG("dummy: skip disconnect id=%d\n", id);
}

/*
 * <Function name>
 *   OCESQLStartSQL
 *
 * <Outline>
 *   SQL$B%/%(%j:n@.3+;O=hM}(B
 *   _sql_var_lists $B$N%/%j%"$r9T$&(B
 *   _var_lists_length $B$N%/%j%"$r9T$&(B
 *
 */
int
OCESQLStartSQL(void){
	LOG("#begin\n");
	init_sql_var_list();
	LOG("#end\n");
	return 0;
}

/*
 * <Function name>
 *   OCESQLSetSQLParams
 *
 * <Outline>
 *   $B;XDj$5$l$?%Q%i%a!<%?$rKd$a9~$_(BSQL$B%j%9%H$KDI2C$9$k(B
 *
 * <Input>
 *   type : $BKd9~JQ?t$N7?(B
 *   length : $BKd9~JQ?t$N(Bsize
 *   scale : $B%9%1!<%k(B(10$B$N%Q%o!<$H$7$FI=8=$5$l$?%U%#!<%k%I$N%9%1!<%k(B)
 *   addr : $BJQ?t$N%"%I%l%9(B
 *
 * < Output>
 *   $B2?$b$7$J$$(B
 *
 */
int
OCESQLSetSQLParams(int type, int length, int scale, void *addr){
	if(type < OCDB_TYPE_MIN || type > OCDB_TYPE_MAX){
		ERRLOG("invalide arugument 'type': %d\n", type);
		return 1;
	}

	if(length < 0){ // $BD9$5$,$J$$>l9g(B
		ERRLOG("invalide argument 'length': %d\n", length);
		return 1;
	}

	if(!addr){
		ERRLOG("invalide argument addr is NULL\n");
		return 1;
	}

	if(add_sql_var_list(type ,length, scale, addr) == NULL){
		ERRLOG("fail to add SQLVARLIST\n");
		return 1;
	}
	return 0;
}

/*
 * <Function name>
 *   OCESQLSetSQLParams
 *
 * <Outline>
 *   $B;XDj$5$l$?%Q%i%a!<%?$r(BSQL$B7k2L3JG<%j%9%H$KDI2C$9$k(B
 *
 * <Input>
 *   type : $BKd9~JQ?t$N7?(B
 *   length : $BKd9~JQ?t$N(Bsize
 *   scale : $B%9%1!<%k(B(10$B$N%Q%o!<$H$7$FI=8=$5$l$?%U%#!<%k%I$N%9%1!<%k(B)
 *   addr : $BJQ?t$N%"%I%l%9(B
 *
 * < Output>
 *   $B2?$b$7$J$$(B
 *
 */
int
OCESQLSetResultParams(int type, int length, int scale, void *addr){
	if(type < OCDB_TYPE_MIN || type > OCDB_TYPE_MAX){
		ERRLOG("invalide arugument 'type': %d\n", type);
		return 1;
	}

	if(length < 0){ // $BD9$5$,$J$$>l9g(B
		ERRLOG("invalide argument 'length': %d\n", length);
		return 1;
	}

	if(!addr){
		ERRLOG("invalide argument addr is NULL\n");
		return 1;
	}

	if(add_sql_res_var_list(type ,length, scale, addr) == NULL){
		ERRLOG("fail to add SQLVARLIST\n");
		return 1;
	}
	return 0;
}

/*
 * <Function name>
 *   OCESQLSetHostTable
 *
 * <Outline>
 *   OCCURS$BJQ?t$,%[%9%HJQ?t$K;XDj$5$l$F$$$k;~$K7+$jJV$72s?t$H(B
 *   $B0l<~$N%P%$%H?t$r%;%C%H$9$k(B
 *
 * <Input>
 *   iter: iteration count
 *   length: byte length of host variable
 *
 * < Output>
 *   $B2?$b$7$J$$(B
 *
 */
int
OCESQLSetHostTable(int iter, int length, int is_parent){
	if(iter < 0){
		ERRLOG("invalide arugument 'iter': %d\n", iter);
		return 1;
	}
	if(length < 0){
		ERRLOG("invalide arugument 'length': %d\n", length);
		return 1;
	}
	_occurs_iter = iter;
	_occurs_length = length;
	_occurs_is_parent = is_parent;
	return 0;
}

/*
 * <Function name>
 *   OCESQLEndSQL
 *
 * <Outline>
 *   SQL$B%/%(%j=*N;=hM}(B
 *   _sql_var_lists $B$N%/%j%"$r9T$&(B
 *   _var_lists_length $B$N%/%j%"$r9T$&(B
 */
int
OCESQLEndSQL(void){
	LOG("#debug start dump var_list\n");
	show_sql_var_list(_sql_var_lists);
	LOG("#debug start dump res_list\n");
	show_sql_var_list(_sql_res_var_lists);
	LOG("#debug end dump list\n");

	clear_sql_var_list(_sql_var_lists);
	clear_sql_var_list(_sql_res_var_lists);
	reset_sql_var_list();
	return 0;
}

/*
 * <Function name>
 *   init_sql_var_list
 *
 * <Outline>
 *   $BKd$a9~$_(BSQL$B%j%9%H=i4|2=(B
 *   $BI,MW$K1~$8$F(Bclear_sql_var_list$B$b<B9T$9$k(B
 */
static void
init_sql_var_list(void){
	if(_sql_var_lists != NULL){
		clear_sql_var_list(_sql_var_lists);
	}
	if(_sql_res_var_lists != NULL){
		clear_sql_var_list(_sql_res_var_lists);
	}
	reset_sql_var_list();

	if((_sql_var_lists = new_sql_var_list()) == NULL){
		ERRLOG("cannot initialize SQLVARLIST\n");
		return;
	}

	if((_sql_res_var_lists = new_sql_var_list()) == NULL){
		ERRLOG("cannot initialize SQLVARLIST\n");
		return;
	}

	return;
}

/*
 * <Function name>
 *   reset_sql_var_list
 *
 * <Outline>
 *   $B%0%m!<%P%kJQ?t=i4|2=(B
 *   - _sql_var_lists
 *   - _var_lists_length
 */
static void
reset_sql_var_list(void){
	_sql_var_lists = NULL;
	_sql_res_var_lists = NULL;
	_var_lists_length = 0;
	_res_var_lists_length = 0;
	_occurs_length = 0;
	_occurs_iter = 0;
	_occurs_is_parent = 0;
	return;
}

/*
 * <Function name>
 *   new_sql_var_list
 *
 * <Outline>
 *   $BKd$a9~$_(BSQL$B%j%9%H@8@.(B
 */
static SQLVARLIST *
new_sql_var_list(void){
	SQLVARLIST *p;
	p = (SQLVARLIST *)malloc(sizeof(SQLVARLIST));
	if(p != NULL){
		// initialize
		p->sv.type = 0;
		p->sv.length = 0;
		p->sv.power = 0;
		p->sv.addr = NULL;
		p->sv.realdata = NULL;
		p->next = NULL;
	}

	return p;
}

/*
 * <Function name>
 *   add_sql_var_list
 *
 * <Outline>
 *   SQL$BKd9~JQ?t%j%9%H$NMWAG$r:n@.$7DI2C$9$k(B
 *   value $B$K$O!"(Blength + 1$B$NJ8;zNs$r=`Hw$9$k(B
 *   addr$B$N%G!<%?$r7?$ND9$5$K9g$o$;$F(Bdata$B$K3JG<$9$k(B
 *   addr$B$N%G!<%?$H7?>pJs$r$b$H$K@07A$7$?%G!<%?$O(Brealdata$B$K3JG<$9$k(B
 *
 * <Input>
 *   type : $BKd9~JQ?t$N7?(B
 *   length : $BKd9~JQ?t$N(Bsize
 *   power : $BKd9~JQ?t$N(Bpower
 *   addr : $BJQ?t$N%"%I%l%9(B
 *
 */
static SQLVARLIST *
add_sql_var_list(int type , int length, int power, void *addr){

	SQLVARLIST *p = _sql_var_lists;

	if(_sql_var_lists == NULL){
		ERRLOG("_sql_var_lists has not been initialized\n");
		return NULL;
	}

	while(p->next != NULL){
		p = p->next;
	}

	if((p->next = new_sql_var_list()) == NULL){
		ERRLOG("cannot generate new SQLVARLIST\n");
		return NULL;
	}

	p->sv.type = type;
	p->sv.length = length;
	p->sv.power = power;
	p->sv.addr = addr;


	create_realdata(&p->sv, 0);
	_var_lists_length++;

	return p;
}

/*
 * <Function name>
 *   add_sql_var_list
 *
 * <Outline>
 *   SQL$BKd9~JQ?t%j%9%H$NMWAG$r:n@.$7DI2C$9$k(B
 *   value $B$K$O!"(Blength + 1$B$NJ8;zNs$r=`Hw$9$k(B
 *   addr$B$N%G!<%?$r7?$ND9$5$K9g$o$;$F(Bdata$B$K3JG<$9$k(B
 *   addr$B$N%G!<%?$H7?>pJs$r$b$H$K@07A$7$?%G!<%?$O(Brealdata$B$K3JG<$9$k(B
 *
 * <Input>
 *   type : $BKd9~JQ?t$N7?(B
 *   length : $BKd9~JQ?t$N(Bsize
 *   power : $BKd9~JQ?t$N(Bpower
 *   addr : $BJQ?t$N%"%I%l%9(B
 *
 */
static SQLVARLIST *
add_sql_res_var_list(int type , int length, int power, void *addr){
	SQLVARLIST *p = _sql_res_var_lists;

	if(_sql_res_var_lists == NULL){
		ERRLOG("_sql_var_lists has not been initialized\n");
		return NULL;
	}

	while(p->next != NULL){
		p = p->next;
	}

	if((p->next = new_sql_var_list()) == NULL){
		ERRLOG("cannot generate new SQLVARLIST\n");
		return NULL;
	}

	p->sv.type = type;
	p->sv.length = length;
	p->sv.power = power;
	p->sv.addr = addr;


	//create_realdata(&p->sv, 0);
	_res_var_lists_length++;

	return p;
}

static void
create_realdata(SQLVAR *sv,int index){

	COBOLFIELD field;
	field.addr = sv->addr;
	if(_occurs_is_parent){
		field.addr += index * _occurs_length;
	}else{
		field.addr += index * get_sqlvar_byte_length(sv);
	}
	field.type = sv->type;
	field.length = sv->length;
	field.power = sv->power;
	sv->realdata = set_realdata(&field);
}

static void
create_coboldata_lowvalue(SQLVAR *sv, int index){
	void *addr = sv->addr;
	char *caddr = addr;
	if(_occurs_is_parent){
		caddr += index * _occurs_length;
	}else{
		caddr += index * get_sqlvar_byte_length(sv);
	}
	addr = caddr;

	memset(addr,0,get_sqlvar_byte_length(sv));

	return;
}
static void
create_coboldata(SQLVAR *sv, int index, char *retstr){
	COBOLFIELD field;
	field.addr = sv->addr;
	if(_occurs_is_parent){
		field.addr += index * _occurs_length;
	}else{
		field.addr += index * get_sqlvar_byte_length(sv);
	}
	field.type = sv->type;
	field.length = sv->length;
	field.power = sv->power;

	set_coboldata(&field, retstr);
}

static int
get_varchar_length(char *hexval){
	int i, shift;
	int retval = 0;
	unsigned char tmp1, tmp2, tmp;
	if(get_endian() == LITTLEENDIAN){
		int is = 0;
		for(i=0; i<OCDB_VARCHAR_HEADER_BYTE; i+=2){
			tmp1 = tolower(hexval[i]);
			if(isdigit(tmp1)){
				tmp1 -= '0';
			} else {
				tmp1 -= ('a' - 10);
			}

			tmp2 = tolower(hexval[i+1]);
			if(isdigit(tmp2)){
				tmp2 -= '0';
			} else {
				tmp2 -= ('a' - 10);
			}
			tmp = (tmp1<<4) + tmp2;
			if(tmp == 0) is++;
			shift = i/2 - is;
			retval += (tmp<<(shift*8));
		}
	} else {
		for(i=0; i<OCDB_VARCHAR_HEADER_BYTE; i+=2){
			tmp1 = tolower(hexval[i]);
			if(isdigit(tmp1)){
				tmp1 -= '0';
			} else {
				tmp1 -= ('a' - 10);
			}

			tmp2 = tolower(hexval[i+1]);
			if(isdigit(tmp2)){
				tmp2 -= '0';
			} else {
				tmp2 -= ('a' - 10);
			}
			tmp = (tmp1<<4) + tmp2;
			shift = (OCDB_VARCHAR_HEADER_BYTE - i)/2 - 1;
			retval += (tmp<<(shift*8));
		}
	}

	return retval;
}

static void
set_varchar_length(int len, char *dest){
	int i;
	char c;

	if(get_endian() == LITTLEENDIAN){
		for(i=OCDB_VARCHAR_HEADER_BYTE-1 ; i>0; i-=2){
			c = 0xf & (len >> ((i-1)*4));
			dest[i] = (c>9) ? (c+'A'-10) : (c+'0');
			c = 0xf & (len >> (i*4));
			dest[i+1] = (c>9) ? (c+'A'-10) : (c+'0');
		}
	} else {
		for(i=0; i<OCDB_VARCHAR_HEADER_BYTE; i++){
			c = 0xf & (len >> (i*4));
			dest[OCDB_VARCHAR_HEADER_BYTE-i-1] =
			     (c>9) ? (c+'A'-10) : (c+'0');
		}
	}
	return;
}

static void show_sql_var_list(SQLVARLIST *p){
	if(p != NULL){
		LOG("%p %d %d %d %p %p\n", p, p->sv.type, p->sv.length, p->sv.power,
			p->sv.addr, p->next);
		show_sql_var_list(p->next);
	}
}


/*
 * <Function name>
 *   clear_sql_var_list
 *
 * <Outline>
 *   $BKd$a9~$_(BSQL$B%j%9%H$N%a%b%j2rJ|(B
 *
 * <Input>
 *   @SQLVARLIST *
 */
static void
clear_sql_var_list(SQLVARLIST *p){
	if(p != NULL){
		clear_sql_var_list(p->next);
		if(p->sv.realdata)
			free(p->sv.realdata);
		free(p);
	}
}

static int
OCESQLResultStatus(int id){
	//dummy
	return 0;
}

/*
 * <Function name>
 *   _ocesqlResultStatus
 *
 * <Outline>
 *   $B7k2L$r<hF@$7!"(BESQL$B$NJV$jCM$KK]Lu$7$FJV$9(B
 *
 * <Input>
 *   @id: connectId
 *
 * <Output>
 *   status
 */
static void
_ocesqlResultStatus(int id, struct sqlca_t *st){
	char* state;
	char* errmsg;
	int statelen,errlen;

	st->sqlcode = OCDBResultStatus(id);
	LOG("SQLCODE:%d\n",st->sqlcode);
	state = OCDBResultErrorField(id);
	if(state){
		LOG("STATE:%s\n",state);
		statelen = strlen(state);
		if(statelen >= SQLSTATE_LEN) statelen = SQLSTATE_LEN;
		strncpy(st->sqlsubcode, state, statelen);
	}
	errmsg = OCDBResultErrorMessage(id);
	if(errmsg){
		LOG("MESSAGE:%s\n",errmsg);
		errlen = strlen(errmsg);
		if(errlen >= SQLERRMC_LEN) errlen = SQLERRMC_LEN;
		strncpy(st->sqlerrm.sqlerrmc, errmsg, errlen);
		st->sqlerrm.sqlerrml = errlen;

	}

	st->sqlerrd[2] = OCDBCmdTuples(id);

	if(st->sqlcode < 0){
		ERRLOG("STATE:%s, MESSAGE:%s\n",state,errmsg);
	}
}

static void
_ocesqlReleaseConnection(int status, void *arg){
	int connectId = *(int *)arg;
	OCDBFinish(connectId);
}

static int
_ocesqlResolveCONNID(struct sqlca_t *st, char *atdb, int atdblen){
	char *cid = NULL;
	int id;

	cid = get_str_without_after_space(oc_strndup(atdb, atdblen));
	if(cid == NULL){
		return RESULT_FAILED;
	}
	id = OCDBResolveCONNID(cid);
	if(id == RESULT_FAILED){
		ERRLOG("connection name %s is not found in connection list.\n", cid);
		st->sqlcode = OCDB_RES_ARGUMENT_ERROR;
		free(cid);
		return RESULT_FAILED;
	}
	free(cid);

	return id;
}

/*
 * <Function name>
 *   new_cursor_list
 *
 * <Outline>
 *   CURSOR$B%j%9%H@8@.(B
 */
static CURSORLIST *
new_cursor_list(void){
	CURSORLIST *p;
	p = (CURSORLIST *)malloc(sizeof(CURSORLIST));
	if(p != NULL){
		// initialize
		p->connid = 0;
		p->cname = NULL;
		p->sp = NULL;
		p->query = NULL;
		p->nParams = 0;
		p->isOpened = 0;
		p->tuples = 0;
		p->next = NULL;
	}

	return p;
}

/*
 * <Function name>
 *   add_cursor_list
 *
 * <Outline>
 *   DeclareCursor$B$GDj5A$5$l$?(BCURSOR$B$r%j%9%H$KEPO?$9$k(B
 *
 * <Input>
 *   id : $B@\B3(BID
 *   cname : CURSOR NAME
 *   query : query
 *   nParams : number of parameters
 *
 * <Output>
 *   success: RESULT_SUCCESS
 *   failure: RESULT_FAILED
 */
static int
add_cursor_list(int id, char *cname, char *query, int nParams){
	CURSORLIST *p = &_cursor_list;
	int isExist = 0;

	while(p->next != NULL){
		p = p->next;
		// duplication check
		if(strcmp(cname, p->cname) == 0){
			if(p->isOpened){
				ERRLOG("cursor name '%s' already registered and opened.\n", cname);
				return RESULT_FAILED;
			}
			isExist = 1;
			LOG("cursor name '%s' already registered.\n", cname);
			if(p->query){
				free(p->query);
			}
			if(p->plist){
				clear_sql_var_list(p->plist);
			}
			break;
		}
	}
	if(!isExist){
		if((p->next = new_cursor_list()) == NULL){
			ERRLOG("cannot generate new CURSORLIST\n");
			return RESULT_ERROR;
		}
		p = p->next;
		p->cname = _strdup(cname);
	}
	p->connid = id;
	p->query = _strdup(query);
	p->nParams = nParams;
	p->plist = NULL;
	p->isOpened = 0;
	p->tuples = 0;

	if(nParams > 0){
		// set parameter list
		SQLVARLIST *sp = _sql_var_lists;
		SQLVARLIST *current;
		if(_sql_var_lists == NULL){
			ERRLOG("_sql_var_lists has not been initialized\n");
			return RESULT_ERROR;
		}

		if((p->plist = new_sql_var_list()) == NULL){
			ERRLOG("cannot initialize SQLVARLIST\n");
			return RESULT_ERROR;
		}
		current = p->plist;

		while(sp->next != NULL){
			if((current->next = new_sql_var_list()) == NULL){
				ERRLOG("cannot initialize SQLVARLIST\n");
				return RESULT_ERROR;
			}

			// copy
			current->sv.type = sp->sv.type;
			LOG("current->sv.type=#%d#\n", current->sv.type);
			current->sv.length = sp->sv.length;
			current->sv.power = sp->sv.power;
			current->sv.addr = sp->sv.addr;

			current->sv.realdata = _strdup(sp->sv.realdata);

			sp = sp->next;
			current = current->next;
		}
	}

	return RESULT_SUCCESS;
}


static int
add_cursor_list_with_prepare(int id, char *cname, PREPARELIST *prepare){
	CURSORLIST *p = &_cursor_list;
	int isExist=0;
	while(p->next != NULL){
		p = p->next;
		// duplication check
		if(strcmp(cname, p->cname) == 0){
			if(p->isOpened){
				ERRLOG("cursor name '%s' already registered and opened.\n", cname);
				return RESULT_FAILED;
			}
			isExist = 1;
			LOG("cursor name '%s' already registered.\n", cname);
			break;
		}
	}
	if(!isExist){
		if((p->next = new_cursor_list()) == NULL){
			ERRLOG("cannot generate new CURSORLIST\n");
			return RESULT_ERROR;
		}
		p = p->next;
		p->cname = strdup(cname);
	}
	p->connid = id;
	p->sp = prepare;
	p->isOpened = 0;
	p->tuples = 0;


	return RESULT_SUCCESS;
}

/*
 * <Function name>
 *   remove_cursor_list
 *
 * <Outline>
 *   DeclareCursor$B$GDj5A$5$l$?(BCURSOR$B$r%j%9%H$KEPO?$9$k(B
 *
 * <Input>
 *   cname : CURSOR NAME
 */
static void
remove_cursor_list(char *cname){
	CURSORLIST *p, *prev;

	p = &_cursor_list;

	while(p->next != NULL){
		prev = p;
		p = p->next;
		if(strcmp(cname, p->cname) == 0){
			prev->next = p->next;
			if(p->cname)
				free(p->cname);
			if(p->query)
				free(p->query);
			free(p);
			LOG("remove cursor %s.\n", cname);
			return;
		}
	}
}

static void show_cursor_list(CURSORLIST *p){
	if(p != NULL){
		LOG("%d %s\n", p->connid, p->cname);
		show_cursor_list(p->next);
	}
}

/*
 * <Function name>
 *   clear_sql_var_list
 *
 * <Outline>
 *   CURSOR$B%j%9%H$N%a%b%j2rJ|(B
 *
 * <Input>
 *   @CURSORLIST *
 */
static void
clear_cursor_list(CURSORLIST *list, int id){
	_clear_cursor_list(list->next, id);
}

static void
_clear_cursor_list(CURSORLIST *p, int id){
	if(p != NULL){
		_clear_cursor_list(p->next, id);
		if(p->connid == id)
			p->isOpened = 0;
	}
}

/*
 * <Function name>
 *   get_cursor_list_connid
 *
 * <Outline>
 *   $B0z?t$N(Bcname$B$H0lCW$9$k(BCURSOR$B>pJs$rJV$9(B
 *
 * <Input>
 *   cname : CURSOR NAME
 *
 * <Output>
 *   success : CURSORLIST$BJQ?t(B
 *   failure : RESULT_FAILED
 */
static CURSORLIST *
get_cursor_from_list(char *cname){
	CURSORLIST *p = &_cursor_list;
	CURSORLIST *ret = NULL;
	LOG("#target:%s#\n", cname);
	while(p->next != NULL){
		p = p->next;
		LOG("#search:%s#\n", p->cname);
		if(strcmp(cname, p->cname) == 0){
			ret = p;
			break;
		}
	}

	if(ret == NULL){
		ERRLOG("cursor name '%s' is not found in cursor list.\n", cname);
		return NULL;
	}
	LOG("#return:%s#\n", ret->cname);
	return ret;
}

static void
show_prepare_list(){
	PREPARELIST *p =&_prepare_list;
	while(p != NULL){
		if(p->sq.pname){
			LOG("#sname:%s#\n", p->sq.pname);
		}else{
			LOG("#sname:NULL#\n");
		}
		p = p->next;
	}
}

static PREPARELIST*
new_prepare_list(){
	PREPARELIST *p;
	p = (PREPARELIST *)malloc(sizeof(PREPARELIST));
	if(p != NULL){
		// initialize
//		p->sq = (struct query_info *)malloc(sizeof(struct query_info));
		p->sq.pname = NULL;
		p->sq.query = NULL;
		p->sq.nParams = 0;
		p->next = NULL;
	}
	return p;

}

static PREPARELIST*
add_prepare_list(char *sname, char *query, int nParams){
	int isExist = 0;
	PREPARELIST *p =&_prepare_list;

	if(p == NULL){
		ERRLOG("_prepare_list has not been initialized\n");
		return NULL;
	}

	while(p->next != NULL){
		p = p->next;
		if(strcmp(p->sq.pname, sname)==0){
			LOG("prepare name '%s' already registered\n", sname);
			isExist =1;
			if(p->sq.query){
				free(p->sq.query);
				p->sq.query=NULL;
			}
			break;
		}
	}

	if(!isExist){
		if((p->next = new_prepare_list()) == NULL){
				ERRLOG("cannot generate new SQLVARLIST\n");
				return NULL;
		}
		p = p->next;
		p->sq.pname = strdup(sname);
	}
	p->sq.query = query;
	p->sq.nParams = nParams;

		show_prepare_list();

	return p;
}

static PREPARELIST *
get_prepare_from_list(char *sname){

	PREPARELIST *p =&_prepare_list;
	PREPARELIST *ret = NULL;

	if(p == NULL){
		ERRLOG("_prepare_list has not been initialized\n");
		return NULL;
	}
	LOG("#tartget:%s#\n", sname);
	while(p->next != NULL){
		p = p->next;
		if(p->sq.pname == NULL){
			break;
		}
		LOG("#search:%s#\n", p->sq.pname);
		if(strcmp(p->sq.pname, sname) == 0){
			ret = p;
			break;
		}
	}

	if(ret == NULL){
		ERRLOG("prepare name '%s' is not found in prepare list.\n", sname);
		show_prepare_list();
		return NULL;
	}
	LOG("#return:%s#\n", ret->sq.pname);
	return ret;
}

static int
get_sqlvar_byte_length(SQLVAR *sv){
	if(sv == NULL) return 0;
	
	if(sv->type == OCDB_TYPE_JAPANESE ||
	   sv->type == OCDB_TYPE_JAPANESE_VARYING){
		return sv->length * 2;
	} else if(sv->type == OCDB_TYPE_UNSIGNED_NUMBER_PD ||
		  sv->type == OCDB_TYPE_SIGNED_NUMBER_PD){
		return (sv->length / 2) + 1;
	}

	return sv->length;
}

/*
 * <Function name>
 *   is_group_occurs_param_set
 *
 * <Outline>
 *   $B7+JV$79`L\$KCM$,@_Dj$5$l$F$$$k$+$I$&$+$r%A%'%C%/(B
 *   ($B=8CD9`L\(B / _occurs_is_parent = true $B$N$_BP>]$H$9$k(B)
 *
 * <Input>
 *   p : SQLVARLIST$B7?$NJQ?t(B
 *   index : $B7+$jJV$72s?t(B
 *
 * <Output>
 *   $BCM$,@_Dj$5$l$F$$$k(B,$BBP>]30(B : RESULT_SUCCESS
 *   $BCM$,@_Dj$5$l$F$$$J$$(B(LOW-VALUE) : RESULT_FAILED
 */
static int is_group_occurs_param_set(SQLVARLIST *psvl, int index){
	LOG("#start is_group_occurs_param_set\n");

	COBOLFIELD start;
	if(_occurs_is_parent){
		LOG("#_occurs_is_parent: true\n");
		start.addr = psvl->sv.addr;
		start.addr += index * _occurs_length;

		if(is_low_value((char *)start.addr, _occurs_length)){
			LOG("#failed: low_value\n");
			return RESULT_FAILED;
		}
	} else {
		LOG("#_occurs_is_parent: false\n");	  

		SQLVARLIST *p = psvl;
		int i;
		for(i=0; i<_var_lists_length; i++, p=p->next){
			start.addr = p->sv.addr;

			int len = get_sqlvar_byte_length(&p->sv);
			start.addr += index * len;

			if(is_low_value((char *)start.addr, len)){
				LOG("#failed: low_value\n");
				return RESULT_FAILED;
			}
		}
	}

	LOG("#success: not low_value\n");
	return RESULT_SUCCESS;
}
