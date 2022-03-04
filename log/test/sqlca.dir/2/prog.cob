
       IDENTIFICATION              DIVISION.
      ******************************************************************
       PROGRAM-ID.                 prog.
      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.
       01 TEST-DATA.
         03 FILLER PIC X(9) VALUE "0001____1".
         03 FILLER PIC X(9) VALUE "0002____2".
         03 FILLER PIC X(9) VALUE "0003____3".
         03 FILLER PIC X(9) VALUE "0004____4".
         03 FILLER PIC X(9) VALUE "0005____5".

       01 TEST-DATA-R REDEFINES TEST-DATA.
         03 TEST-TBL OCCURS 5.
           05 TEST-ID PIC 9(4).
           05 TEST-V  PIC X(5).

       01 IDX PIC 9.

       01 READ-DATA.
         03  READ-TBL    OCCURS  1.
           05  READ-V PIC X(5).

OCESQL*EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01 DATA-ID PIC 9(4).
       01 DATA-V PIC X(5).
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.
OCESQL*EXEC SQL END DECLARE SECTION END-EXEC.

OCESQL*EXEC SQL INCLUDE SQLCA END-EXEC.
OCESQL     copy "sqlca.cbl".

      ******************************************************************
OCESQL*
OCESQL 01  SQ0001.
OCESQL     02  FILLER PIC X(035) VALUE "SELECT V FROM TESTTABLE ORDER "
OCESQL  &  "BY ID".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0002.
OCESQL     02  FILLER PIC X(030) VALUE "DROP TABLE IF EXISTS TESTTABLE".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0003.
OCESQL     02  FILLER PIC X(048) VALUE "CREATE TABLE TESTTABLE ( ID in"
OCESQL  &  "teger, V char(5) )".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0004.
OCESQL     02  FILLER PIC X(039) VALUE "INSERT INTO TESTTABLE VALUES ("
OCESQL  &  " $1, $2 )".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0005.
OCESQL     02  FILLER PIC X(014) VALUE "DISCONNECT ALL".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.

       PERFORM SETUP-DB.

OCESQL*EXEC SQL
OCESQL*  DECLARE C CURSOR FOR
OCESQL*    SELECT V
OCESQL*    FROM TESTTABLE
OCESQL*    ORDER BY ID
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLCursorDeclare" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_C" & x"00"
OCESQL          BY REFERENCE SQ0001
OCESQL     END-CALL.

OCESQL*EXEC SQL
OCESQL*  OPEN C
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLCursorOpen" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_C" & x"00"
OCESQL     END-CALL.
       PERFORM SHOW-STATUS.

OCESQL*EXEC SQL
OCESQL*  OPEN CC
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLCursorOpen" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_CC" & x"00"
OCESQL     END-CALL.
       PERFORM SHOW-STATUS.

OCESQL*EXEC SQL
OCESQL*  FETCH C INTO :DATA-V
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 5
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE DATA-V
OCESQL     END-CALL
OCESQL     CALL "OCESQLCursorFetchOne" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_C" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.
       PERFORM SHOW-STATUS.

OCESQL*EXEC SQL
OCESQL*  FETCH CC INTO :DATA-V
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetResultParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 5
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE DATA-V
OCESQL     END-CALL
OCESQL     CALL "OCESQLCursorFetchOne" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_CC" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL.
       PERFORM SHOW-STATUS.

OCESQL*EXEC SQL
OCESQL*  CLOSE C
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLCursorClose"  USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_C" & x"00"
OCESQL     END-CALL
OCESQL    .
       PERFORM SHOW-STATUS.

OCESQL*EXEC SQL
OCESQL*  CLOSE CC
OCESQL*END-EXEC.
OCESQL     CALL "OCESQLCursorClose"  USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "prog_CC" & x"00"
OCESQL     END-CALL
OCESQL    .
       PERFORM SHOW-STATUS.

       PERFORM CLEANUP-DB.

       STOP RUN.

      ******************************************************************
       SETUP-DB.
      ******************************************************************

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
OCESQL*        CREATE TABLE TESTTABLE
OCESQL*        (
OCESQL*          ID integer,
OCESQL*          V  char(5)
OCESQL*        )
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0003
OCESQL     END-CALL.


           PERFORM VARYING IDX FROM 1 BY 1 UNTIL IDX > 5
             MOVE TEST-ID(IDX) TO DATA-ID
             MOVE TEST-V(IDX) TO DATA-V
OCESQL*      EXEC SQL
OCESQL*        INSERT INTO TESTTABLE VALUES
OCESQL*          (:DATA-ID, :DATA-V)
OCESQL*      END-EXEC
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetSQLParams" USING
OCESQL          BY VALUE 1
OCESQL          BY VALUE 4
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE DATA-ID
OCESQL     END-CALL
OCESQL     CALL "OCESQLSetSQLParams" USING
OCESQL          BY VALUE 16
OCESQL          BY VALUE 5
OCESQL          BY VALUE 0
OCESQL          BY REFERENCE DATA-V
OCESQL     END-CALL
OCESQL     CALL "OCESQLExecParams" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0004
OCESQL          BY VALUE 2
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL
           END-PERFORM.

      ******************************************************************
       CLEANUP-DB.
      ******************************************************************

OCESQL*    EXEC SQL
OCESQL*        DISCONNECT ALL
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLDisconnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL     END-CALL.

      ******************************************************************
       SHOW-STATUS.
      ******************************************************************
           DISPLAY SQLCODE.
           DISPLAY SQLSTATE.



