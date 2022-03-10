#ifndef OCESQL_H
#define OCESQL_H

#define OCESQL_CONN_CONNECT_OK 0
#define OCESQL_CONN_FAIL_CONNECT (-1)

#define OCESQL_NO_CONNECTION (-1)

#define SQLERRMC_LEN	150
#define SQLSTATE_LEN	8

// SQL連絡領域 - only sqlcode and sqlerrm are used at the moment
struct sqlca_t
{
	char		sqlcaid[8];
	int		sqlabc;
	int		sqlcode; // error code
	char		sqlsubcode[8]; // error code 2
	struct
	{
		short		sqlerrml;
		char		sqlerrmc[SQLERRMC_LEN];
	}			sqlerrm; // error message
	char		sqlerrp[8];
	int		sqlerrd[6];
	char		sqlwarn[8];
	char		sqlstate[SQLSTATE_LEN];
} sqlca;

static struct sqlca_t sqlca_init =
{
	{
		'S', 'Q', 'L', 'C', 'A', ' ', ' ', ' '
	},
	sizeof(struct sqlca_t),
	0,
	{
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	},
	{
		0,
		{
			0
		}
	},
	{
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	},
	{
		0, 0, 0, 0, 0, 0
	},
	{
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	},
	{
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
	}
};


#define OCESQL_TYPE_PD_POSITIVE 'C'
#define OCESQL_TYPE_PD_NEGATIVE 'D'

#define OCESQL_OK 0
#define OCESQL_NORECORD 1
#define OCESQL_TIMEOUT 2

#define OCESQL_DEFAULT_DBNAME "OCDB_DEFAULT_DBNAME"
#define OCESQL_DEFAULT_DBLENG strlen(OCESQL_DEFAULT_DBNAME)

int OCESQLConnect(struct sqlca_t *, char *, int, char *, int, char *, int);
int OCESQLConnectInformal(struct sqlca_t *, char *, int);
int OCESQLIDConnect(struct sqlca_t *, char *, int, char *, int, char *, int, char *, int);
int OCESQLIDConnectInformal(struct sqlca_t *, char *, int, char *, int);
int OCESQLDisconnect(struct sqlca_t *);
int OCESQLIDDisconnect(struct sqlca_t *, char*, int);

int OCESQLPrepare(struct sqlca_t *, char *, char *, int);
int OCESQLExec(struct sqlca_t *, char *);
int OCESQLIDExec(struct sqlca_t *, char *, int, char *);
int OCESQLExecParams(struct sqlca_t *, char *, int);
int OCESQLIDExecParams(struct sqlca_t *, char *, int, char *, int);
int OCESQLExecParamsOccurs(struct sqlca_t *, char *, int);
int OCESQLIDExecParamsOccurs(struct sqlca_t *, char *, int, char *, int);
int OCESQLCursorDeclare(struct sqlca_t *, char *, char *);
int OCESQLIDCursorDeclare(struct sqlca_t *, char *, int, char *, char *);
int OCESQLCursorDeclareParams(struct sqlca_t *, char *, char *, int);
int OCESQLIDCursorDeclareParams(struct sqlca_t *, char *, int, char *, char *, int);
int OCESQLPreparedCursorDeclare(struct sqlca_t *, char *, char *);
int OCESQLIDPreparedCursorDeclare(struct sqlca_t *, char *, int, char *, char *);
int OCESQLExecPrepare(struct sqlca_t *, char *, int);
int OCESQLIDExecPrepare(struct sqlca_t *, char *, int,  char *, int);
int OCESQLExecPrepareOccurs(struct sqlca_t *, char *, int);
int OCESQLIDExecPrepareOccurs(struct sqlca_t *, char *, int,  char *, int);
int OCESQLCursorOpen(struct sqlca_t *, char *);
int OCESQLCursorOpenParams(struct sqlca_t *, char *, int);
int OCESQLCursorFetchOne(struct sqlca_t *, char *);
int OCESQLCursorFetchOccurs(struct sqlca_t *, char *);
int OCESQLCursorClose(struct sqlca_t *, char *);
int OCESQLExecSelectIntoOne(struct sqlca_t *, char *, int, int);
int OCESQLIDExecSelectIntoOne(struct sqlca_t *, char *, int, char *, int, int);
int OCESQLExecSelectIntoOccurs(struct sqlca_t *, char *, int, int);
int OCESQLIDExecSelectIntoOccurs(struct sqlca_t *, char *, int, char *, int, int);
int OCESQLNtuples(int);
int OCESQLNfields(int);
char *OCESQLGetvalue(int, int);
char *OCESQLResultErrorMessage(struct sqlca_t *);

int OCESQLStartSQL(void);
int OCESQLSetSQLParams(int, int, int, void *);
int OCESQLSetResultParams(int, int, int, void *);
int OCESQLSetHostTable(int, int, int);
int OCESQLEndSQL(void);

#endif
