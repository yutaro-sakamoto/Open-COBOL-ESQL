#ifndef OCPGSQL_H
#define OCPGSQL_H
#include <libpq-fe.h>
#include "ocdb.h"
/* method */
unsigned long OCDB_PGConnect(char *, int, char *);
int OCDB_PGstatus(unsigned long);
char *OCDB_PGErrorMessage(unsigned long);

void OCDB_PGClear(unsigned long);
unsigned long OCDB_PGExec(unsigned long, char *);
unsigned long OCDB_PGExecParams(unsigned long, char *, int,
                    const int *, const char * const *,
                    const int *, const int *, int);
unsigned long OCDB_PGCursorDeclare(unsigned long, char *, char *, int);
unsigned long OCDB_PGCursorDeclareParams(unsigned long, char *, char *, int,
                    const int *, const char * const *,
     	  	    const int *, const int *, int, int);
int OCDB_PGCountRow(unsigned long, char *);
int OCDB_PGCountRowParams(unsigned long, char *, int, const int *,
                    const char * const *, const int *, const int *, int);
unsigned long OCDB_PGCursorFetchOne(unsigned long, char *, int);
unsigned long OCDB_PGCursorFetchOccurs(unsigned long, char *, int, int);
unsigned long OCDB_PGCursorClose(unsigned long, char *);
int OCDB_PGResultStatus(unsigned long);
int OCDB_PGcmdtuples(unsigned long);
int OCDB_PGntuples(unsigned long);
char *OCDB_PGResultErrorMessage(unsigned long);
char *OCDB_PGResultErrorField(unsigned long);
int OCDB_PGnfields(unsigned long);
char *OCDB_PGfname(unsigned long, int);
int OCDB_PGfnumber(unsigned long, const char *);
char *OCDB_PGgetvalue(unsigned long, int, int);
unsigned long OCDB_PGDropTable(unsigned long, char *);
struct column_info_list *OCDB_PGGetTableInfo(unsigned long, const char *, const char *);
void OCDB_PGFinish(unsigned long);

#endif
