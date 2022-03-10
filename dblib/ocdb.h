#ifndef OCDB_H
#define OCDB_H

#define PGSQL_MODE_ON

typedef struct lock_conn {
	char *db;
	char *schema;
	char *table;
	int lockconnid;
	char *pid;
}LOCK_CONN;

#ifdef PGSQL_MODE_ON
#include "ocpgsql.h"
#endif

#define RESULT_FAILED -1
#define RESULT_SUCCESS 0
#define RESULT_ERROR -2

// default resource address
#define OCDB_RES_DEFAULT_ADDRESS 0

#define DEFAULT_NEXT_CONN_ID 1
#define INVALID_CONN_ID -1
#define OCDB_INVALID_NUMBER -1
#define OCDB_INVALID_STRING (NULL)
#define OCDB_CONN_CONNECT_OK 1
#define OCDB_CONN_NOT_CONNECT 0
#define OCDB_CONN_FAIL_CONNECT (-1)

//file mode
#define OCDB_FILEMODE_INPUT	1
#define OCDB_FILEMODE_OUTPUT	2
#define OCDB_FILEMODE_I_O	3
#define OCDB_FILEMODE_EXTEND	4

//lock mode
#define OCDB_LOCK_EXCLUSIVE	1
#define OCDB_LOCK_MANUAL	2
#define OCDB_LOCK_AUTOMATIC	4
#define OCDB_LOCK_MULTIPLE	8
#define OCDB_LOCK_MASK	0x7

//wait for locks
#define OCDB_LOCK_WAIT		1
#define OCDB_LOCK_NOWAIT	0

// autocommit
#define OCDB_AUTOCOMMIT_OFF 0
#define OCDB_AUTOCOMMIT_ON 1

// cursor with hold
#define OCDB_CURSOR_WITH_HOLD_OFF 0
#define OCDB_CURSOR_WITH_HOLD_ON 1

// Lcok table response
#define OCDB_RESLOCK_COMMAND_OK	0
#define OCDB_RESLOCK_TABLE_NOLOCK	10
#define OCDB_RESLOCK_TABLE_LOCKED	11
#define OCDB_RESLOCK_TABLE_LOCKED_OWN	12
#define OCDB_RESLOCK_RECORD_NOLOCK	20
#define OCDB_RESLOCK_RECORD_LOCKED	21
#define OCDB_RESLOCK_RECORD_LOCKED_OWN	22
#define OCDB_RESLOCK_ERROR_NOTCONNECT	90
#define OCDB_RESLOCK_ERROR_NODATABASE	91
#define OCDB_RESLOCK_ERROR_NOTABLE	92
#define OCDB_RESLOCK_ERROR_SEGFAULT	98
#define OCDB_RESLOCK_ERROR_FATAL	99

// direction of FetchOne
enum {
	OCDB_READ_NEXT = 1,     // READ NEXT
	OCDB_READ_PREVIOUS,     // READ PREVIOUS
	OCDB_READ_CURRENT       // READ CURRENT CURSOR
};

// 型情報
//#define OCDB_TYPE_FORMATTED_NUMBER 0        // 数字編集
#define OCDB_TYPE_UNSIGNED_NUMBER 1         // 符号無数字
//#define OCDB_TYPE_SIGNED_NUMBER_TS 2        // 符号付数字(trailing separate)
#define OCDB_TYPE_SIGNED_NUMBER_TC 3        // 符号付数字(trailing combined)
#define OCDB_TYPE_SIGNED_NUMBER_LS 4        // 符号付数字(leading separete)
//#define OCDB_TYPE_SIGNED_NUMBER_LC 5        // 符号付数字(leading combined)
//#define OCDB_TYPE_SIGNED_CALC 6             // 符号付計算
//#define OCDB_TYPE_UNSIGNED_CALC 7           // 符号無計算
#define OCDB_TYPE_UNSIGNED_NUMBER_PD 8        // 正のパック10進数
#define OCDB_TYPE_SIGNED_NUMBER_PD 9      // 符号付パック10進数
//#define OCDB_TYPE_COMP6 10                  // COMP-6
//#define OCDB_TYPE_SINGED_BINARY 11          // 符号付2進数
//#define OCDB_TYPE_UNSINGED_BINARY 12        // 符号無2進数
#define OCDB_TYPE_SINGED_BINARY_NATIVE 13   // 符号付2進数(native-order)
#define OCDB_TYPE_UNSINGED_BINARY_NATIVE 14 // 符号無2進数(native-order)
#define OCDB_TYPE_ALPHANUMERIC 16           // 英数字
//#define OCDB_TYPE_ALPHANUMERIC_J 17         // 英数字(桁寄せ)
//#define OCDB_TYPE_ALPHABETICAL 18           // 英字
//#define OCDB_TYPE_ALPHABETICAL_J 19         // 英字(桁寄せ)
//#define OCDB_TYPE_FORMATTED_ALPHANUMERIC 20 // 英数字編集
#define OCDB_TYPE_GROUP 22                  // 集団
//#define OCDB_TYPE_FLOATING_NUMBER 23        // Float or Double
#define OCDB_TYPE_JAPANESE 24               // 日本語
//#define OCDB_TYPE_JAPANESE_J 25             // 日本語(桁寄せ)
//#define OCDB_TYPE_FORMATTED_JAPANESE 26     // 日本語編集
//#define OCDB_TYPE_WIDE 27                   // wide
//#define OCDB_TYPE_WIDE_J 28                 // wide(桁寄せ)
//#define OCDB_TYPE_FORMATTED_WIDE 29         // wide
#define OCDB_TYPE_ALPHANUMERIC_VARYING 30     // VARYING(PIC X)
#define OCDB_TYPE_JAPANESE_VARYING 31         // VARYING(PIC N)
#define OCDB_TYPE_BYTEA 32                    // BYTEA
#define OCDB_TYPE_MIN 0                       // 型下限
//#define OCDB_TYPE_MAX 32                      // 型上限
#define OCDB_TYPE_MAX 33                      // 型上限

#define OCDB_TYPE_OPTION_DATE 1
#define OCDB_TYPE_OPTION_BINARY 2
#define OCDB_TYPE_OPTION_VARCHAR 3

#define OCDB_VARCHAR_HEADER_BYTE 4 // size of PIC S9(8) COMP-5

//SQL exec status(SQLCODE)
// over 0: NO ERROR
// OCDB_RES_COMMAND_OK(0):
//   - INSERT, UPDATEなど行を返さないSQLの成功
//   - SELECT, FETCHの結果が1行以上ある場合
// OCDB_RES_TUPLES_NODATA:
//   - SELECT, FETCHの結果が0行の場合
// OCDB_RES_TUPLES_NODATA:
//   - KEY指定時にSELECT, FETCHの結果が0行の場合
// (-10)-(-19): WARNING
// (-20)-(-29): ERROR
// (-30)-(-39): FATAL ERROR
// (-40)-(-49): LOCK STATUS
#define OCDB_RES_EMPTY_QUERY (-10)
#define OCDB_RES_COMMAND_OK 0
#define OCDB_RES_TUPLES_NODATA 10
#define OCDB_RES_INVALID_KEY 11
#define OCDB_RES_COPY_OUT 2
#define OCDB_RES_COPY_IN 3
#define OCDB_RES_BAD_RESPONSE (-20)
#define OCDB_RES_NONFATAL_ERROR (-21)
#define OCDB_RES_ARGUMENT_ERROR (-22)
#define OCDB_RES_ALLOCATE_ERROR (-23)
#define OCDB_RES_UNDEFINED_CONNECTION (-24)
#define OCDB_RES_UNDEFINED_PREPARE (-25)
#define OCDB_RES_UNDEFINED_CURSOR (-26)
#define OCDB_RES_OVERWRITE_OPENED_CURSOR (-27)
#define OCDB_RES_FATAL_ERROR (-30)
#define OCDB_RES_LOCKED_RECORD (-40)
#define OCDB_RES_LOCKED_TABLE (-41)

#define OCDB_RES_NOCONNECT_ERRORMSG "Connection is not opened."

#define OCDB_COBOL_BUFFER_SIZE 1024
#define OCDB_OCCURS_MAX_TIMES 32000

struct s_conn {
	int id;
	char *cid;
	int dbtype;
	unsigned long connaddr;
	unsigned long resaddr;
	int result;
	char *errorMessage;
	char *pid;
};

struct conn_list {
	struct s_conn sc;
	struct conn_list *next;
};


typedef struct cobolfield{
	int type;
	int length;
	int power;
	void *addr;
} COBOLFIELD;

#define OCDB_COLUMN_TYPE_NUMERIC 0
#define OCDB_COLUMN_TYPE_CHARACTER 1
#define OCDB_COLUMN_TYPE_UNKNOWN -1

typedef struct column_info{
	char *name;
	int type;
	int character_length;
	int numeric_precision;
	int numeric_scale;
} COLUMN_INFO;

typedef struct column_info_list{
	struct column_info *column_info;
	struct column_info_list *next;
} COLUMN_INFO_LIST;

enum DBTYPE{
#ifdef PGSQL_MODE_ON
	USE_PGSQL = 0,
#endif
#ifdef MYSQL_MODE_ON
	USE_MYSQL = 1,
#endif
#ifdef ORACLE_MODE_ON
	USE_ORACLE = 2,
#endif
	DUMMY = -1,
};

/* method */
int OCDBCheckType(int);
int OCDBConnect(int, char *, char *, int, char *);
int OCDBStatus(int);
char *OCDBErrorMessage(int);

void OCDBExec(int, char *);
void OCDBExecParams(int, char *, int, const int *, const char * const *,
		const int *, const int *, int);
void OCDBCursorDeclare(int, char *, char *, int);
void OCDBCursorDeclareParams(int, char *, char *, int, const int *,
		const char * const *, const int *, const int *, int, int);
int OCDBCountRow(int, char *);
int OCDBCountRowParams(int, char *, int, const int *,
		const char * const *, const int *, const int *, int);
void OCDBCursorOpen(int, char *);
void OCDBCursorFetchOne(int, char *, int);
void OCDBCursorFetchOccurs(int, char *, int, int);
void OCDBCursorClose(int, char *);
int OCDBResultStatusConvert(int);
int OCDBResultStatus(int);
char *OCDBResultErrorMessage(int);
char *OCDBResultErrorField(int);
int OCDBCmdTuples(int);
int OCDBNtuples(int);
int OCDBNfields(int);
char *OCDBFname(int, int);
int OCDBFnumber(int, const char *);
char *OCDBGetvalue(int, int, int);
void OCDBDropTable(int, char *);
void OCDBDeleteTable(int, char *);
int OCDBIsExistTable(int, char *, char *, int);
struct column_info_list *OCDBGetTableInfo(int, const char *, const char *);
void OCDBFinish(int);

int OCDBResolveCONNID(char *);

int OCDBConnectLockDB(char *, char *);
int OCDBCreateLockData(int, LOCK_CONN *, char *);
int OCDBCheckLockTable(int, LOCK_CONN *, char *);
int OCDBWriteLockTable(int, LOCK_CONN *, char *);
int OCDBDeleteLockTable(int, LOCK_CONN *, char *);
int OCDBDeleteLockTableAll(int);
int OCDBCheckOpenTable(int, LOCK_CONN *, int, int);
int OCDBWriteOpenTable(int, LOCK_CONN *, int, int);
int OCDBDeleteOpenTable(int, LOCK_CONN *);
int OCDBIsLockTable(int, char *, int, int);
int OCDBLockTable(int, char *, int, int, int);

/* common APIs */
char *_alloc(long);
char *_strdup(const char *);
char *last_dir_separator(const char *);
int strlen_or_null(const char *);
int set_coboldata(COBOLFIELD *, char *);
char *set_realdata(COBOLFIELD *);
struct column_info_list *info_list_generate(void);
struct column_info_list *add_info_list(struct column_info_list *, char *, int, int, int, int);
void info_list_finish(struct column_info_list *);

#define skip_drive(path)	(path)

#ifndef WIN32
#define IS_DIR_SEP(ch)	((ch) == '/')

#define is_absolute_path(filename)		\
		(						\
				IS_DIR_SEP((filename)[0])		\
		)
#else
#define IS_DIR_SEP(ch)	((ch) == '/' || (ch) == '\\')

/* See path_is_relative_and_below_cwd() for how we handle 'E:abc'. */
#define is_absolute_path(filename)					\
		(									\
				IS_DIR_SEP((filename)[0]) ||					\
				(isalpha((unsigned char) ((filename)[0])) && (filename)[1] == ':' && \
						IS_DIR_SEP((filename)[2]))					\
		)
#endif

#endif
