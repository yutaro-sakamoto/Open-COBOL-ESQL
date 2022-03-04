
       IDENTIFICATION              DIVISION.
      ******************************************************************
       PROGRAM-ID.                 prog.
      ******************************************************************
       DATA                        DIVISION.
      ******************************************************************
       WORKING-STORAGE             SECTION.

       01  IDX                     PIC  9(02).
       01 LOG-COUNT PIC 9999 VALUE 1.

OCESQL*EXEC SQL BEGIN DECLARE SECTION END-EXEC.
       01  DBNAME                  PIC  X(30) VALUE SPACE.
       01  USERNAME                PIC  X(30) VALUE SPACE.
       01  PASSWD                  PIC  X(10) VALUE SPACE.

       01  EMP-REC-VARS.
         03  EMP-NO                PIC S9(04) VALUE ZERO.
         03  EMP-NAME              PIC  X(20) .
         03  EMP-SALARY            PIC S9(04) VALUE ZERO.
OCESQL*EXEC SQL END DECLARE SECTION END-EXEC.

OCESQL*EXEC SQL INCLUDE SQLCA END-EXEC.
OCESQL     copy "sqlca.cbl".
      ******************************************************************
OCESQL*
OCESQL 01  SQ0001.
OCESQL     02  FILLER PIC X(024) VALUE "DROP TABLE IF EXISTS EMP".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0002.
OCESQL     02  FILLER PIC X(135) VALUE "CREATE TABLE EMP ( EMP_NO NUME"
OCESQL  &  "RIC(4, 0) NOT NULL, EMP_NAME CHAR(20), EMP_SALARY NUMERIC("
OCESQL  &  "4, 0), CONSTRAINT IEMP_0 PRIMARY KEY (EMP_NO) )".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0003.
OCESQL     02  FILLER PIC X(024) VALUE "DROP TABLE IF EXISTS EMP".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
OCESQL 01  SQ0004.
OCESQL     02  FILLER PIC X(014) VALUE "DISCONNECT ALL".
OCESQL     02  FILLER PIC X(1) VALUE X"00".
OCESQL*
       PROCEDURE                   DIVISION.
      ******************************************************************
       MAIN-RTN.

       PERFORM CONNECT-DB.

OCESQL*    EXEC SQL
OCESQL*        DROP TABLE IF EXISTS EMP
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0001
OCESQL     END-CALL.
           PERFORM OUTPUT-RETURN-CODE-TEST.

OCESQL*    EXEC SQL
OCESQL*         CREATE TABLE EMP
OCESQL*         (
OCESQL*             EMP_NO     NUMERIC(4,0) NOT NULL,
OCESQL*             EMP_NAME   CHAR(20),
OCESQL*             EMP_SALARY NUMERIC(4,0),
OCESQL*             CONSTRAINT IEMP_0 PRIMARY KEY (EMP_NO)
OCESQL*         )
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0002
OCESQL     END-CALL.
           PERFORM OUTPUT-RETURN-CODE-TEST.

OCESQL*    EXEC SQL
OCESQL*        DROP TABLE IF EXISTS EMP
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE SQ0003
OCESQL     END-CALL.
           PERFORM OUTPUT-RETURN-CODE-TEST.

       PERFORM DISCONNECT-DB.

      *    END
           STOP RUN.

      ******************************************************************
       CONNECT-DB.
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

      ******************************************************************
       DISCONNECT-DB.
      ******************************************************************

OCESQL*    EXEC SQL
OCESQL*        DISCONNECT ALL
OCESQL*    END-EXEC.
OCESQL     CALL "OCESQLDisconnect" USING
OCESQL          BY REFERENCE SQLCA
OCESQL     END-CALL.

      ******************************************************************
       OUTPUT-RETURN-CODE-TEST.
      ******************************************************************
           IF  SQLCODE = ZERO
             THEN

               DISPLAY LOG-COUNT " <log> success test_return_code"

             ELSE
               DISPLAY LOG-COUNT " <log> fail test_return_code    "
                   NO ADVANCING
               DISPLAY "SQLCODE=" SQLCODE " ERRCODE="  SQLSTATE " "
                   NO ADVANCING
               EVALUATE SQLCODE
                  WHEN  +10
                     DISPLAY "Record_not_found"
                  WHEN  -01
                     DISPLAY "Connection_falied"
                  WHEN  -20
                     DISPLAY "Internal_error"
                  WHEN  -30
                     DISPLAY "PostgreSQL_error" NO ADVANCING
                     DISPLAY SQLERRMC
                  *> TO RESTART TRANSACTION, DO ROLLBACK.
OCESQL*              EXEC SQL
OCESQL*                  ROLLBACK
OCESQL*              END-EXEC
OCESQL     CALL "OCESQLStartSQL"
OCESQL     END-CALL
OCESQL     CALL "OCESQLExec" USING
OCESQL          BY REFERENCE SQLCA
OCESQL          BY REFERENCE "ROLLBACK" & x"00"
OCESQL     END-CALL
OCESQL     CALL "OCESQLEndSQL"
OCESQL     END-CALL
                  WHEN  OTHER
                     DISPLAY "Undefined_error" NO ADVANCING
                     DISPLAY SQLERRMC
               END-EVALUATE.

           ADD 1 TO LOG-COUNT.
      ******************************************************************



