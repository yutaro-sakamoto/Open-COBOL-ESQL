
       IDENTIFICATION              DIVISION.
      ******************************************************************
       PROGRAM-ID.                 prog.
      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.
       01 V PIC X(10).

       01  TEST-DATA.
         03 FILLER  PIC X(10) VALUE "xxxxxxxxxx".
         03 FILLER  PIC X(10) VALUE "abcdefghij".
         03 FILLER  PIC X(10) VALUE "1234567890".
         03 FILLER  PIC X(10) VALUE "abc_______".
         03 FILLER  PIC X(10) VALUE "??????????".
         03 FILLER  PIC X(10) VALUE "{}^|!%#$()".
         03 FILLER  PIC X(10) VALUE "‚ ‚¢‚¤‚¦‚¨".
         03 FILLER  PIC X(10) VALUE "“ú–{Œêaaaa".
         03 FILLER  PIC X(10) VALUE "‚P‚Q‚R‚S‚T".
         03 FILLER  PIC X(10) VALUE "ƒvƒƒOƒ‰ƒ€".

       01  TEST-DATA-R   REDEFINES TEST-DATA.
         03  TEST-TBL    OCCURS  10.
           05  D             PIC X(10).

       01  IDX                     PIC  S9(02) .
       01 LOG-COUNT PIC 9999 VALUE 1.

       01 READ-DATA-TBL.
         03  READ-TBL    OCCURS  10.
           05  READ-DATA     PIC X(10).

OCESQL*EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.
OCESQL*EXEC SQL END DECLARE SECTION END-EXEC.

OCESQL*EXEC SQL INCLUDE SQLCA END-EXEC.
OCESQL     copy "sqlca.cbl".
      ******************************************************************
OCESQL*
OCESQL 01  SQ0001.
OCESQL     02  FILLER PIC X(038) VALUE "SELECT FIELD FROM TESTTABLE OR"
OCESQL  &  "DER BY N".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0002.
OCESQL     02  FILLER PIC X(030) VALUE "DROP TABLE IF EXISTS TESTTABLE".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0003.
OCESQL     02  FILLER PIC X(067) VALUE "CREATE TABLE TESTTABLE ( N NUM"
OCESQL  &  "ERIC(2, 0) NOT NULL, FIELD CHAR(10) )".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0004.
OCESQL     02  FILLER PIC X(039) VALUE "INSERT INTO TESTTABLE VALUES ("
OCESQL  &  " $1, $2 )".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0005.
OCESQL     02  FILLER PIC X(030) VALUE "DROP TABLE IF EXISTS TESTTABLE".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0006.
OCESQL     02  FILLER PIC X(014) VALUE "DISCONNECT ALL".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.

       PERFORM SETUP-DB.

      *    SHOW RESULT
OCESQL*    EXEC SQL
OCESQL*        SELECT FIELD INTO :READ-TBL FROM TESTTABLE ORDER BY N
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 10
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE READ-DATA(1)
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetHostTable" USING
OCESQL          BY VALUE 10
OCESQL          BY VALUE 10
OCESQL          BY VALUE 1
OCESQL     END-CALL
OCESQL     CALL "OCESQLExecSelectIntoOccurs" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0001
OCESQL          BY VALUE 0
OCESQL          BY VALUE 1
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.

           PERFORM VARYING IDX FROM 1 BY 1 UNTIL IDX > 10
               DISPLAY READ-DATA(IDX)
           END-PERFORM.

       PERFORM CLEANUP-DB.

      *    END
           STOP RUN.

      ******************************************************************
       SETUP-DB.
      ******************************************************************

      *    SERVER
           MOVE  "testdb@localhost:5432"
             TO DBNAME.
           MOVE  "main_user"
             TO USERNAME.
           MOVE  "password"
             TO PASSWD.

OCESQL*    EXEC SQL
OCESQL*        CONNECT :USERNAME IDENTIFIED BY :PASSWD USING :DBNAME
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLConnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE USERNAME
OCESQL          BY VALUE 30
OCESQL          BY REFERENCE PASSWD
OCESQL          BY VALUE 10
OCESQL          BY REFERENCE DBNAME
OCESQL          BY VALUE 30
OCESQL     END-CALL.

OCESQL*    EXEC SQL
OCESQL*        DROP TABLE IF EXISTS TESTTABLE
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0002
OCESQL     END-CALL.

OCESQL*    EXEC SQL
OCESQL*         CREATE TABLE TESTTABLE
OCESQL*         (
OCESQL*             N         NUMERIC(2,0) NOT NULL,
OCESQL*             FIELD     CHAR(10)
OCESQL*         )
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0003
OCESQL     END-CALL.

      *    INSERT ROWS USING HOST VARIABLE
           PERFORM VARYING IDX FROM 1 BY 1 UNTIL IDX > 10
              MOVE D(IDX)     TO  V
OCESQL*       EXEC SQL
OCESQL*          INSERT INTO TESTTABLE VALUES (:IDX, :V)
OCESQL*       END-EXEC
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetSQLParams" USING
OCESQL          BY VALUE 3
OCESQL          BY VALUE 2
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE IDX
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetSQLParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 10
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE V
OCESQL     END-CALL
OCESQL     CALL "OCESQLExecParams" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0004
OCESQL          BY VALUE 2
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL
           END-PERFORM.

      *    COMMIT
OCESQL*    EXEC SQL
OCESQL*        COMMIT WORK
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "COMMIT" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.

      ******************************************************************
       CLEANUP-DB.
      ******************************************************************
OCESQL*    EXEC SQL
OCESQL*        DROP TABLE IF EXISTS TESTTABLE
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0005
OCESQL     END-CALL.

OCESQL*    EXEC SQL
OCESQL*        DISCONNECT ALL
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLDisconnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL     END-CALL.




