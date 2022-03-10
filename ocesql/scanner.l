%option 8bit
%option caseless
%option noyywrap
%option never-interactive
%option yylineno
%option stack

%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "define.h"
#include "parser.h"

int startlineno = 0;
int endlineno = 0;
int hostlineno = 0;
int period = 0;
int conn_use_other_db = 0;
int command_putother = 0;
struct cb_exec_list *exec_list = NULL;
struct cb_hostreference_list *host_reference_list;
struct cb_res_hostreference_list *res_host_reference_list;
struct cb_sql_list *sql_list;

char dbname[BUFFSIZE];
char prepname[BUFFSIZE];
char commandname[BUFFSIZE];
char cursorname[BUFFSIZE];
char sqlname[BUFFSIZE];
int sqlnum = 0;
char incfilename[BUFFSIZE];
struct cb_field *var_varying;
struct cb_field *var_sqlbytea;

int flag_insqlstring = 0;
int flag_selectcommand = 0;

#define YY_INPUT(buf,result,max_size) result = yyinput (buf, max_size);
static int
yyinput (char *buf, int max_size);

#define SET_LOCATION(x)				\
  (x)->source_file = (unsigned char *)cb_source_file;		\

%}

%s PICTURE_STATE WORKING_STATE

%x EXEC_STATE ESQL_WORK_STATE ESQL_FUNC_STATE ESQL_INCLUDE_STATE ESQL_SELECT_STATE ESQL_STATE ESQL_DBNAME_STATE WHERE_CURRENT_OF PROCEDURE_STATE

JPNWORD [\xA0-\xDF]|([\x81-\x9F\xE0-\xFC][\x40-\x7E\x80-\xFC])
DIGIT [0-9]
WORD ([A-Za-z\+\-0-9_]|[(]|[)]|[\'])
INCFILE [A-Za-z0-9_\+\-]+
FILENAME [A-Za-z0-9_\+\-\.]+
STRVALUE ("\""[^\"]+"\""|"\'"[^\']+"\'")
HEXVALUE "X"("\""[^\"]+"\""|"\'"[^\']+"\'")
SELF [,()\[\].;\:\+\-\*\/\%\^\<\>\=]
OP_CHARS [\~\!\@\#\^\&\|\`\?\+\-\*\/\%\<\>\=]
OPERATOR {OP_CHARS}+
COMPARISON "="|"<>"|"<"|">"|"<="|">="
COMMA ","
HOSTWORD ":"([A-Za-z\-0-9_]*([\xA0-\xDF]|([\x81-\x9F\xE0-\xFC][\x40-\x7E\x80-\xFC]))*[A-Za-z\-0-9_]*)

INT_CONSTANT {digit}+

%%

"WORKING-STORAGE"[ ]+"SECTION"[ ]*"." {
	BEGIN WORKING_STATE;
	startlineno = yylineno;
	endlineno = yylineno;
	host_reference_list = NULL;
	res_host_reference_list = NULL;
	memset(dbname,0,sizeof(dbname));
	memset(prepname,0,sizeof(prepname));
	memset(commandname,0,sizeof(commandname));
	strcpy(commandname,"WORKING_BEGIN");
	memset(cursorname,0,sizeof(cursorname));
	memset(sqlname,0,sizeof(sqlname));
	memset(incfilename,0,sizeof(incfilename));
	hostreferenceCount = 0;
	conn_use_other_db = 0;
	command_putother = 0;
	sql_list = NULL;
	var_varying = NULL;
	var_sqlbytea = NULL;
	return WORKINGBEGIN;
}

"PROCEDURE"[ ]+"DIVISION"[^\.]*"." {
	BEGIN PROCEDURE_STATE;
	startlineno = yylineno;
	endlineno = yylineno;
	host_reference_list = NULL;
	res_host_reference_list = NULL;
	memset(dbname,0,sizeof(dbname));
	memset(prepname,0,sizeof(prepname));
	if(strcmp(commandname,"WORKING_END")!= 0){
		strcpy(commandname,"WORKING_END");
	} else {
		strcpy(commandname,"PROCEDURE_BEGIN");
	}
	memset(cursorname,0,sizeof(cursorname));
	memset(sqlname,0,sizeof(sqlname));
	memset(incfilename,0,sizeof(incfilename));
	hostreferenceCount = 0;
	conn_use_other_db = 0;
	command_putother = 0;
	sql_list = NULL;
	var_varying = NULL;
	var_sqlbytea = NULL;
	return PROCEDUREBEGIN;
}

<WORKING_STATE>{
	"EXEC" {
		startlineno = yylineno;
		return EXEC;
	}

	"SQL" {
		yy_push_state(ESQL_WORK_STATE);
		return SQL;
	}

	"." {
		return '.';
	}

	"COPY"[ ]+"\"".+"\""([ ]+("==".*"=="|[^\.]*))*"." {}

	"COPY"[ ]+[^\.]+([ ]+("==".*"=="|[^\.]*))*"." {}

	("66"|"77"|"78"|"88")[^\.]*"." {}

	"OBJECT-STORAGE"[ ]+"SECTION"[ ]*"." |
	"LOCAL-STORAGE"[ ]+"SECTION"[ ]*"." |
	"LINKAGE"[ ]+"SECTION"[ ]*"." |
	"COMMUNICATION"[ ]+"SECTION"[ ]*"." |
	"REPORT"[ ]+"SECTION"[ ]*"." |
	"SCREEN"[ ]+"SECTION"[ ]*"." {
		startlineno = yylineno;
		endlineno = yylineno;
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"WORKING_END");
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		BEGIN INITIAL;
		return WORKINGEND;
	}


	"PIC" |
	"PICTURE" {
		yy_push_state(PICTURE_STATE);
	}

	([0-9]+)|([0-9]+\.[0-9]+) {
		sscanf(yytext, "%ld", &yylval.ld);
		return NUMERIC;
	}

	"OCCURS" {
		return OCCURS;
	}

	"USAGE" {
		return USAGE;
	}

	"COMP-1" {
		return  COMP_1;
	}

	"COMP-2" {
		return  COMP_2;
	}

	"COMP-3" {
		return COMP_3;
	}

	"SIGN"  { return SIGN ;}

	"LEADING" { return LEADING;}

	"SEPARATE" { return SEPARATE; }

	"TRAILING" { return TRAILING; }

	"EXTERNAL"  { return EXTERNAL;}

	"VALUE"|"VALUES" { return VALUE;}

	"VARYING"  {
		startlineno = yylineno;
		endlineno = yylineno;
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"VARYING_PARAM");
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		return VARYING;
	}

	"SQL-BYTEA"  {
		startlineno = yylineno;
		endlineno = yylineno;
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"SQL_BYTEA_PARAM");
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_sqlbytea = NULL;
		return SQL_BYTEA;
	}

	"TIMES"  { return TIMES;}

	"ALL"  { return ALL ;}

	{STRVALUE}|{HEXVALUE} { return CONST;}

	([A-Za-z\-0-9_]|{JPNWORD})+ {
		yylval.s = strdup(yytext);
		return WORD;
	}

	(\r\n|\n) { }

	. { }
}

<ESQL_WORK_STATE>{
	"INCLUDE" {
		period = 0;
		startlineno = yylineno;
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		yy_push_state(ESQL_INCLUDE_STATE);
		return INCLUDE;
	}

	"BEGIN" {
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"HOST_BEGIN");
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		return _BEGIN;
	}

	"END" {
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"HOST_END");
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		return END;
	}

	"DECLARE" {
		return DECLARE;
	}

	"SECTION" {
		return SECTION;
	}

	"END-EXEC"[ \r\n]*"." {
		endlineno = yylineno;
		yy_pop_state();
		return END_EXEC;
	}

	"END-EXEC" {
		endlineno = yylineno;
		yy_pop_state();
		return END_EXEC;
	}
}

<PICTURE_STATE>{
	"IS" {
		/* ignore */
	}

	[^ \r\n;\.]+(\.[^ \r\n;\.]+)* {
		yylval.s = strdup(yytext);
		yy_pop_state();
		return PICTURE;
	}
}

<PROCEDURE_STATE>{
	"EXEC" {
		startlineno = yylineno;
		return EXEC;
	}

	"SQL" {
		yy_push_state(ESQL_FUNC_STATE);
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(commandname,0,sizeof(commandname));
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		period = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		return SQL;
	}
}

<ESQL_FUNC_STATE>{
	"SELECT" {
		yy_push_state(ESQL_STATE);
		flag_insqlstring = 1;
		flag_selectcommand = 1;
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);
		sqlnum++;
		memset(sqlname, 0, sizeof(sqlname));
		sprintf(sqlname, "SQ%04d", sqlnum);
		return SELECT;
	}

	"INSERT" {
		yy_push_state(ESQL_STATE);
		flag_insqlstring = 1;
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);
		sqlnum++;
		memset(sqlname, 0, sizeof(sqlname));
		sprintf(sqlname, "SQ%04d", sqlnum);
		return INSERT;
	}

	"DELETE" {
		yy_push_state(ESQL_STATE);
		flag_insqlstring = 1;
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);
		sqlnum++;
		memset(sqlname, 0, sizeof(sqlname));
		sprintf(sqlname, "SQ%04d", sqlnum);
		return DELETE;
	}

	"CONNECT" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"CONNECT");
		return CONNECT;
	}

	"DISCONNECT" {
		yy_push_state(ESQL_STATE);
		flag_insqlstring = 1;
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);
		sqlnum++;
		memset(sqlname, 0, sizeof(sqlname));
		sprintf(sqlname, "SQ%04d", sqlnum);
		return DISCONNECT;
	}

	"UPDATE" {
		yy_push_state(ESQL_STATE);
		flag_insqlstring = 1;
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);
		sqlnum++;
		memset(sqlname, 0, sizeof(sqlname));
		sprintf(sqlname, "SQ%04d", sqlnum);
		return UPDATE;
	}

	"DECLARE" {
		yy_push_state(ESQL_STATE);
		return DECLARE;
	}

	"OPEN" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"OPEN");
		return OPEN;
	}

	"PREPARE" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"PREPARE");
		return PREPARE;
	}

	"EXECUTE" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"EXECUTE");
		return EXECUTE;
	}

	"CLOSE" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"CLOSE");
		return CLOSE;
	}

	"FETCH" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"FETCH");
		return FETCH;
	}

	"COMMIT"[ ]+"WORK"+[ ]+"RELEASE" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"COMMIT_RELEASE");
		return COMMIT_WORK;
	}

	"COMMIT"[ ]+"WORK"+[ ]+"WITH"+[ ]+"RELEASE" {
		yy_push_state(ESQL_STATE);
		strcpy(commandname,"COMMIT_RELEASE");
		return COMMIT_WORK;
	}

	"COMMIT"[ ]+"WORK" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"COMMIT");
		return COMMIT_WORK;
	}

	"COMMIT" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"COMMIT");
		return COMMIT_WORK;
	}

	"ROLLBACK"[ ]+"WORK"+[ ]+"RELEASE" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"ROLLBACK_RELEASE");
		return ROLLBACK_WORK;
	}

	"ROLLBACK"[ ]+"WORK"+[ ]+"WITH"+[ ]+"RELEASE" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"ROLLBACK_RELEASE");
		return ROLLBACK_WORK;
	}

	"ROLLBACK"[ ]+"WORK" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"ROLLBACK");
		return ROLLBACK_WORK;
	}

	"ROLLBACK" {
		yy_push_state(ESQL_STATE);

		strcpy(commandname,"ROLLBACK");
		return ROLLBACK_WORK;
	}

	"AT" {
		if(flag_insqlstring || conn_use_other_db){
			yylval.s = strdup (yytext);
			return TOKEN;
		}

		yy_push_state(ESQL_DBNAME_STATE);
		conn_use_other_db = 1;
		return AT;
	}

	"INCLUDE" {
		period = 0;
		startlineno = yylineno;
		host_reference_list = NULL;
		res_host_reference_list = NULL;
		memset(dbname,0,sizeof(dbname));
		memset(prepname,0,sizeof(prepname));
		memset(cursorname,0,sizeof(cursorname));
		memset(sqlname,0,sizeof(sqlname));
		memset(incfilename,0,sizeof(incfilename));
		hostreferenceCount = 0;
		conn_use_other_db = 0;
		command_putother = 0;
		sql_list = NULL;
		var_varying = NULL;
		var_sqlbytea = NULL;
		yy_push_state(ESQL_INCLUDE_STATE);
		return INCLUDE;
	}

	({WORD}|{JPNWORD})+ {
		yy_push_state(ESQL_STATE);
		yylval.s = strdup (yytext);
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,yylval.s);

		sqlnum++;
     		memset(sqlname, 0, sizeof(sqlname));
     		sprintf(sqlname, "SQ%04d", sqlnum);

		command_putother = 1;
		return OTHERFUNC;
	}
}

<ESQL_DBNAME_STATE>{
	{HOSTWORD} {
		yylval.s = strdup (yytext + 1);
		hostlineno = yylineno;
		yy_pop_state();
		return HOSTTOKEN;
	}
}

<ESQL_STATE>{
	{COMMA}   {
	          yylval.s = strdup (yytext);
	          return TOKEN;
	          }
	(\r\n|\n) {   }



	[;]?(\r\n|\n)		{
				ECHO;
				}
	"AT" {
			if(flag_insqlstring || conn_use_other_db){
				yylval.s = strdup (yytext);
				return TOKEN;
			}

			yy_push_state(ESQL_DBNAME_STATE);
                        conn_use_other_db = 1;
			return AT;
	}

	"SELECT" {
			if(flag_insqlstring){
	      			yylval.s = strdup (yytext);
	      			return TOKEN;
			}
			flag_insqlstring = 1;
			yylval.s = strdup (yytext);
			memset(commandname,0,sizeof(commandname));
			strcpy(commandname,yylval.s);

			sqlnum++;
     			memset(sqlname, 0, sizeof(sqlname));
     			sprintf(sqlname, "SQ%04d", sqlnum);

			return SELECT;
	}

	"FROM" {
			if(flag_insqlstring){
	      			yylval.s = strdup (yytext);
				if(!flag_selectcommand){
		      			return TOKEN;
				} else {
					flag_selectcommand = 0;
		      			return SELECTFROM;
				}
			}
			return FROM;
	}

	"CURSOR" {
			if(flag_insqlstring){
	      			yylval.s = strdup (yytext);
	      			return TOKEN;
			}
		       return CURSOR;
	 }

	"FOR" {
			if(flag_insqlstring){
				yylval.s = strdup (yytext);
				return TOKEN;
			}
			return FOR;
	}

	"IDENTIFIED"[ ]+"BY" {
			if(flag_insqlstring){
	      			yylval.s = strdup (yytext);
	      			return TOKEN;
			}
			return IDENTIFIED_BY;
	}

	"USING" {
			if(flag_insqlstring){
	      			yylval.s = strdup (yytext);
	      			return TOKEN;
			}
			return USING;
	}

	"INTO" {
			if(flag_insqlstring && !flag_selectcommand){
	      			yylval.s = strdup (yytext);
	      			return TOKEN;
			}
		     	yylval.s = strdup (yytext);
		     	return INTO;
	}

	"WHERE"[ \r\n]+"CURRENT"[ \r\n]+"OF" {
			yy_push_state(WHERE_CURRENT_OF);
			return WHERECURRENTOF;
	}

	{OPERATOR} {
			yylval.s = strdup (yytext);
			return TOKEN;
	}

	{HOSTWORD} {
			yylval.s = strdup (yytext + 1);
			hostlineno = yylineno;

			return HOSTTOKEN;
	}

	"END-EXEC"[ \r\n]*"." {
			flag_insqlstring = 0;
			flag_selectcommand = 0;
			period = 1;
			endlineno = yylineno;
			yy_pop_state();
			yy_pop_state();
			return END_EXEC;
	}

	"END-EXEC" {
			flag_insqlstring = 0;
			flag_selectcommand = 0;
			endlineno = yylineno;
			yy_pop_state();
			yy_pop_state();
			return END_EXEC;
	}

	"\""({WORD}|{JPNWORD}|{SELF}|[ ])+"\"" {
			yylval.s = (char *)calloc(strlen(yytext)+3,sizeof(char));
			strcat(yylval.s,"\"");
			strcat(yylval.s,yytext);
			strcat(yylval.s,"\"");
			return TOKEN;
	}

	"'"({WORD}|{JPNWORD}|{SELF}|[ ])+"'" {
			yylval.s = strdup (yytext);
			return TOKEN;
	}

	({WORD}|{JPNWORD})+("."("*"|({WORD}|{JPNWORD})+))? {
			yylval.s = strdup (yytext);
			return TOKEN;
	}

	{SELF} {
			yylval.s = strdup (yytext);
			return TOKEN;
	}
}

<WHERE_CURRENT_OF>{
	(\r\n|\n) {  }

	({WORD}|{JPNWORD})+ {
			yylval.s = strdup (yytext);
			yy_pop_state();
			return CURNAME;
	}
}

<ESQL_INCLUDE_STATE>{
	(\r\n|\n) {  }

	"SQLCA" {
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"INCLUDE");
		return INCLUDE_SQLCA;
	}

	"END-EXEC"[ \r\n]*"." {
		period = 1;
		endlineno = yylineno;
		yy_pop_state();
		yy_pop_state();
		return END_EXEC;
	}

	"END-EXEC" {
		endlineno = yylineno;
		yy_pop_state();
		yy_pop_state();
		return END_EXEC;
	}

	{INCFILE} {
		memset(commandname,0,sizeof(commandname));
		strcpy(commandname,"INCFILE");
		yylval.s = strdup (yytext);
		strcpy(incfilename,yylval.s);
		return INCLUDE_FILE;
	}
}

<*>{
	(\r\n|\n) { }
	[ \t]+ { }
	([A-Z0-9]|{JPNWORD})(([_A-Z0-9-]|{JPNWORD})*([A-Z0-9]|{JPNWORD}+))? { }
	\".*\" { }
	'.*'   { }
	. { }

	<<EOF>> {
		BEGIN INITIAL;
		yyterminate ();
	}
}
%%

static int
yyinput(char *buff,int max_size)
{
	char *bp;
	char *comment;

	memset(buff,0,max_size);
	while(fgets(buff,max_size,yyin))
	{
		if(strlen(buff) > 7)
		{
			bp = buff + 7;

			switch (buff[6]) {
			case ' ':
				break;
			case '-':
				break;

			case '\r':
			case '\n':
			case '\0':
				/* ignore line */
				strcpy(buff,"\n");
	    			return strlen(buff);

			case '*':
				/* comment line */
				strcpy(buff,"\n");
	    			return strlen(buff);

			case '/':
				/* comment line */
				strcpy(buff,"\n");
    				return strlen(buff);

			case 'D':
				/* comment line */
				strcpy(buff,"\n");
    				return strlen(buff);

			case 'd':
				/* comment line */
				strcpy(buff,"\n");
    				return strlen(buff);

			case '$':
				/* comment line */
				strcpy(buff,"\n");
    				return strlen(buff);

			default:
	                        printf("EOF:%s\n", buff);
				return YY_NULL;
			}
			if(strlen(buff) > 72){
				memmove(buff, bp, 65);
				strcpy(buff + 65,"\n");
			}else{
				memmove(buff,bp,strlen(bp) + 1);
			}

			comment = strstr(buff, "*>");
                        if(comment) strcpy(comment, "\n");
			return strlen(buff);
	    	}
	    	strcpy(buff,"\n");
	    	return strlen(buff);
	}

	return 0;

}

