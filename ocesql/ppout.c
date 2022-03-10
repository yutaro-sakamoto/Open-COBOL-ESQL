#include "define.h"
#include <stdio.h>
#include <stdlib.h>

char inbuff[256];
char out[256];
struct cb_exec_list *head;
char *outbuff;
FILE *outfile;
int EOFflg = 0;
int EOFFLG = 0;
int lineNUM = 0;

int japflg = 0;
int charcount = 0;

void readline(FILE *readfile){
	char ipchar;
	int n;

	if (readfile){
		ipchar = ' ';

		for (n = 0;ipchar != '\n';n++) {
			ipchar = fgetc(readfile);
			if (ipchar==EOF){
				EOFflg = 1;
				break;}
			inbuff[n] = ipchar;

		}
		inbuff[n] = '\0';
		lineNUM++;
	}
}
char *SQcount(int i){
	char NUM[4];
	NUM[0] = '0';
	NUM[1] = '0';
	NUM[2] = '0';
	NUM[3] = 0;
	if(i<10)
		NUM[2] = i+48;
	if(i>=10&&i<=99){
		NUM[1] = (i/10)+48;
		NUM[2] = (i%10)+48;
	}
	if(i>=100&&i<=999){
		NUM[0] = (i/100)+48;
		NUM[1] = (i%100/10)+48;
		NUM[2] = (i%100%10)+48;
	}
	return strdup(NUM);
}

char *substring(int len,char *wk_str,int flag_end){
	char wkstr[70];
	int n = 0;
	int k;
	int dex;
	int h;

	for(n=0;n<70;n++){
		wkstr[n]='\0';
	}
	dex = len;
	if(len >= strlen(wk_str)){
		EOFFLG = 1;
		dex = strlen(wk_str);
	}
	h=1;
	if(!EOFFLG){
		for(h=1;h<dex;h++){
			if(!check_Dchar(wk_str[dex-h])){
				break;
			}
		}
	}
	if(h>=2){
		if(!(h%2)){
			if(flag_end==1){
				dex--;
			}else{
				dex++;
			}
		}
	}

	for(n = 0;n<dex;n++){
		wkstr[n] = wk_str[n];
	}
	for( k=n;wk_str[k]!='\0';k++){
		wk_str[k-n]=wk_str[k];
	}
	wk_str[k-n]='\0';
	return strdup(wkstr);
}

void sql_string(struct cb_exec_list *wk_text){
	char *intNUM;
	char sqlstr[5][256];
	char compsql[15] = "OCESQL  &  \"\".";
	char terminal[13] = "OCESQL     .";

	char *sqlloop;
	int sqlloop_len;

	struct cb_sql_list *wk_sql;
	int len;

	wk_sql = wk_text->sql_list;

	sqlloop_len = 0;
	for(;wk_sql->next!=NULL;){
		sqlloop_len += strlen(wk_sql->sqltext);
		if(strcmp(wk_sql->next->sqltext,",")!=0){
			sqlloop_len += strlen(" ");
		}
		wk_sql = wk_sql->next;
	}
	sqlloop_len += strlen(wk_sql->sqltext);

	sqlloop = (char *)malloc((sqlloop_len + 1) * sizeof(char));
	if(sqlloop == NULL){
		_printlog("memory allocation failed.\n");
		return;
	}
	memset(sqlloop, 0, sqlloop_len + 1);

	wk_sql = wk_text->sql_list;
	for(;wk_sql->next!=NULL;){
		strcat(sqlloop,wk_sql->sqltext);
		if(strcmp(wk_sql->next->sqltext,",")!=0){
			strcat(sqlloop," ");
		}
		wk_sql = wk_sql->next;
	}
	strcat(sqlloop,wk_sql->sqltext);

	for(;;){
		charcount=0;
		strcpy(sqlstr[0],"OCESQL     02  FILLER PIC X(000) VALUE \"");
		strcpy(sqlstr[1],"OCESQL  &  \"");
		strcpy(sqlstr[2],"OCESQL  &  \"");
		strcpy(sqlstr[3],"OCESQL  &  \"");
		strcpy(sqlstr[4],"OCESQL  &  \"");
		len = strlen(sqlloop);
		if(len<256){
			intNUM = SQcount(len);
			sqlstr[0][28] = intNUM[0];
			sqlstr[0][29] = intNUM[1];
			sqlstr[0][30] = intNUM[2];
		}else{
			sqlstr[0][28] = '2';
			sqlstr[0][29] = '5';
			sqlstr[0][30] = '6';
		}
		strcat(sqlstr[0],substring(30,sqlloop,0));
		strcat(sqlstr[0],"\"");
		if((strlen(sqlstr[0]) == 72)){
			charcount++;
		}
		sqlstr[0][72]='\0';
		if(EOFFLG == 1){
			strcat(sqlstr[0],".");
		}
		if(!EOFFLG){
			strcat(sqlstr[1],substring(58,sqlloop,0));
			strcat(sqlstr[1],"\"");
			if((strlen(sqlstr[1]) == 72)){
				charcount++;
			}

			sqlstr[1][72]='\0';

			if(EOFFLG == 1){
				strcat(sqlstr[1],".");
			}
		}
		if(!EOFFLG){
			strcat(sqlstr[2],substring(58,sqlloop,0));
			strcat(sqlstr[2],"\"");
			if((strlen(sqlstr[2]) == 72)){
				charcount++;
			}

			sqlstr[2][72]='\0';

			if(EOFFLG == 1){
				strcat(sqlstr[2],".");
			}
		}
		if(!EOFFLG){
			strcat(sqlstr[3],substring(58,sqlloop,0));
			strcat(sqlstr[3],"\"");
			if((strlen(sqlstr[3]) == 72)){
				charcount++;
			}

			sqlstr[3][72]='\0';

			if(EOFFLG == 1){
				strcat(sqlstr[3],".");
			}
		}
		if(!EOFFLG){
			strcat(sqlstr[4],substring(52-charcount,sqlloop,1));
			strcat(sqlstr[4],"\"");
			sqlstr[4][72]='\0';

			if(strlen(sqlstr[4]) != ( 65 - charcount ) && !EOFFLG){
				sqlstr[0][28] = '2';
				sqlstr[0][29] = '5';
				sqlstr[0][30] = '5';
			}

			strcat(sqlstr[4],".");
		}

		strcpy(out,sqlstr[0]);
		outwrite();
		if(strncmp(compsql, sqlstr[1], 14) == 0){
			strcpy(out,terminal);
			outwrite();
		} else if(sqlstr[1][12] !='\0'){
			strcpy(out,sqlstr[1]);
			outwrite();
		}
		if(strncmp(compsql, sqlstr[2], 14) == 0){
			strcpy(out,terminal);
			outwrite();
		} else if(sqlstr[2][12] !='\0'){
			strcpy(out,sqlstr[2]);
			outwrite();
		}
		if(strncmp(compsql, sqlstr[3], 14) == 0){
			strcpy(out,terminal);
			outwrite();
		} else if(sqlstr[3][12] !='\0'){
			strcpy(out,sqlstr[3]);
			outwrite();
		}
		if(strncmp(compsql, sqlstr[4], 14) == 0){
			strcpy(out,terminal);
			outwrite();
		} else if(sqlstr[4][12] !='\0'){
			strcpy(out,sqlstr[4]);
			outwrite();
		}
		if(EOFFLG == 1){
			EOFFLG = 0;
			strcpy(out,"OCESQL     02  FILLER PIC X(1) VALUE X\"00\".");
			outwrite();
			break;
		}
	}
	free(sqlloop);
}

void outsqlfiller(struct cb_exec_list *wk_head_p){
	int i;
	i = 0;
	strcpy(out,"OCESQL*");
	outwrite();
	for(;wk_head_p;wk_head_p = wk_head_p->next){

		if (wk_head_p->sql_list){

			i++;
			strcpy(out,"OCESQL 01  ");

			strcat(out,wk_head_p->sqlName);
			strcat(out,".");
			outwrite();

			sql_string(wk_head_p);

			strcpy(out,"OCESQL*");
			outwrite();
		}

	}

	return;
}


void ppoutputendcall(struct cb_exec_list *list){
	char buff[256];
	if( list == NULL)
		return ;

	memset(buff, 0, sizeof(buff));
	if( list->period)
		sprintf(buff, "OCESQL%5sEND-CALL.\n", " ");
	else
		sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
	fputs(buff, outfile);
	return ;

}

void ppoutputopen(struct cb_exec_list *list){
	struct cb_hostreference_list *wk_host;
	struct cb_exec_list *l;
	char str_type[BUFFSIZE];

	l = list;

	if(l->hostreferenceCount != 0){
		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		strcat(out,"\"OCESQLStartSQL\"");
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strend);
		outwrite();
		wk_host = l->host_list;
		int count = 0;
		for(; wk_host ; wk_host = wk_host->next){
			count += ppoutputparam(wk_host,0);
		}

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		strcat(out,"\"OCESQLCursorOpenParams\"");
		strcat(out,strusing);
		outwrite();
		_printlog("Generate:OCESQLCursorOpenParams");

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strsqlca);
		outwrite();

		if( list->cursorName == NULL)
			return ;
		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strreference);
		strcat(out,"\"");
		strcat(out,list->cursorName);
		strcat(out,"\"");
		strcat(out," & x\"00\"");
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strbyvalue);
		sprintf(str_type,"%d",l->hostreferenceCount);
		strcat(out,str_type);
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strend);
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		strcat(out,"\"OCESQLEndSQL\"");
		outwrite();

	} else {
		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		strcat(out,"\"OCESQLCursorOpen\"");
		strcat(out,strusing);
		outwrite();
		_printlog("Generate:OCESQLCursorOpen");

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strsqlca);
		outwrite();

		if( list->cursorName == NULL)
			return ;
		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strreference);
		strcat(out,"\"");
		strcat(out,list->cursorName);
		strcat(out,"\"");
		strcat(out," & x\"00\"");
		outwrite();
	}

	ppoutputendcall(list);

	return ;

}

void ppoutputconnect(struct cb_exec_list *list){
	char buff[256];
	struct cb_hostreference_list *host_list;
	int l,m,n;
	int iret;
	int list_count = 0;

	host_list = list->host_list;
	while(host_list)
	{
		list_count++;
		host_list = host_list->next;
	}
	if(list_count == 1){
		if(list->conn_use_other_db){
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDConnectInformal\" USING\n"," ");
			fputs(buff, outfile);
			_printlog("Generate:OCESQLIDConnectInformal");
		} else {
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%5sCALL \"OCESQLConnectInformal\" USING\n"," ");
			fputs(buff, outfile);
			_printlog("Generate:OCESQLConnectInformal");
		}
	} else {
		if(list->conn_use_other_db){
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDConnect\" USING\n"," ");
			fputs(buff, outfile);
			_printlog("Generate:OCESQLIDConnect");
		} else {
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%5sCALL \"OCESQLConnect\" USING\n"," ");
			fputs(buff, outfile);
			_printlog("Generate:OCESQLConnect");
		}
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		iret = gethostvarianttype(list->dbName,&l,&m,&n);
		if(iret != 0){
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", m);
		fputs(buff, outfile);
	}

	host_list = list->host_list;
	while(host_list)
	{
		iret = gethostvarianttype( host_list->hostreference,&l,&m,&n);
		if(iret!= 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(host_list->hostreference, host_list->lineno, buff, errorfilename);
			return;
		}
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ",host_list->hostreference);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ",m);
		fputs(buff, outfile);
		host_list = host_list->next;
	}
	ppoutputendcall(list);
	return ;

}

void _ppoutputparam(char *varface, int type, int digits, int scale, int iteration){
	char buff[256];

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLSetSQLParams\" USING\n" ," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", type);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ",digits );
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ",scale );
	fputs(buff, outfile);
	memset(buff, 0, sizeof(buff));
	if(iteration>0){
		sprintf(buff, "OCESQL%10sBY REFERENCE %s(1)\n"," ",varface);
	}else{
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ",varface);
	}
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));

	sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
	fputs(buff, outfile);
	return ;
}

int ppoutputparam(struct cb_hostreference_list *host_list, int iteration){
	char buff[256];
	int type, digits, scale ;
	int iret;
	int count = 0;

	if( host_list == NULL )
		return count;
	iret = gethostvarianttype( host_list->hostreference,&type,&digits,&scale);
	if(iret  != 0)
	{
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "E%03d",iret);
		printerrormsg(host_list->hostreference, host_list->lineno, buff, errorfilename);
		return 0;
	}

	if(type == HVARTYPE_GROUP){
		struct cb_field *f;

		f = getfieldbyname(host_list->hostreference);
		if(f == NULL){
			printf("%s:%d\n", host_list->hostreference, ERR_NOTDEF_WORKING);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
			printerrormsg(host_list->hostreference, host_list->lineno,
					buff, errorfilename);
			return count;
		}

		f = f->children;

		while(f){
			iret = gethostvarianttype(f->sname,&type,&digits,&scale);
			if(iret  != 0)
			{
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(f->sname, host_list->lineno, buff, errorfilename);
				return count;
			}
			_ppoutputparam(f->sname, type, digits, scale, iteration);
			count++;
			f = f->sister;
		}
	}else{
		_ppoutputparam(host_list->hostreference, type, digits, scale, iteration);
		count++;
	}

	return count;
}

void ppoutputresparam(char *varface, int type, int digits, int scale, int iteration){
	char buff[256];

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLSetResultParams\" USING\n" ," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", type);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ",digits);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ",scale);
	fputs(buff, outfile);
	memset(buff, 0, sizeof(buff));
	if(iteration > 0){
		sprintf(buff, "OCESQL%10sBY REFERENCE %s(1)\n"," ",varface);
	}else{
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ",varface);
	}
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));

	sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
	fputs(buff, outfile);
	return ;
}

void ppoutputresgroup(struct cb_field *cf, int lineno, int iteration){
	char buff[256];
	int type, digits, scale;
	int iret;

	if(cf == NULL)
		return;

	iret = gethostvarianttype(cf->sname, &type, &digits, &scale);
	if(iret != 0){
		printf("%s:%d\n", cf->sname, iret);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "E%03d",iret);
		printerrormsg(cf->sname, lineno, buff, errorfilename);
		return;
	}

	if(type == HVARTYPE_GROUP){
		ppoutputresgroup(cf->children, lineno, iteration);
	} else {
		ppoutputresparam(cf->sname, type, digits, scale, iteration);
	}

	if(cf->sister != NULL){
		ppoutputresgroup(cf->sister, lineno, iteration);
	}

	return ;
}

void ppoutputexecprepare(struct cb_exec_list *list){
	char buff[256];
	struct cb_hostreference_list *host_list;
	int type, digits, scale;
	int iret;
	char str_type[BUFFSIZE];

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLStartSQL\"\nOCESQL%5sEND-CALL\n"," "," ");
	fputs(buff, outfile);

	int length = 0;
	int iteration = 0;
	int occurs_is_parent = 0;

	// occurs check
	{
		struct cb_hostreference_list *res_host_list;
		int iret;
		if(list->host_list == NULL){
                  goto exit_execute_occurs_check;
		}

		res_host_list = list->host_list;
		struct cb_field *parent, *child, *current;
		current = getfieldbyname(res_host_list->hostreference);
		if(current == NULL){
			printf("%s:%d\n", res_host_list->hostreference, ERR_NOTDEF_WORKING);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
			printerrormsg(res_host_list->hostreference, res_host_list->lineno,
						  buff, errorfilename);
			return;
		}
		parent = current->parent;
		if(parent == NULL){
			goto exit_execute_occurs_check;
		}

		child = parent->children;
		if(parent->occurs){
			iteration = parent->occurs;
			occurs_is_parent = 1;

			iret = get_host_group_length(child, &length);
			if(iret != 0){
				goto exit_execute_occurs_check;
			}
		} else {
			iteration = -1;
			occurs_is_parent = 0;

			iret = get_host_group_table_info(child, &iteration, &length);
			if(iret != 0){
				goto exit_execute_occurs_check;
			}
		}
	}
exit_execute_occurs_check:
	host_list = list->host_list;
	int count = 0;
	if(host_list){
		iret = gethostvarianttype(host_list->hostreference, &type, &digits, &scale);
		if(iret != 0){
			printf("%s:%d\n", host_list->hostreference,iret);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(host_list->hostreference, host_list->lineno,
					buff, errorfilename);
			return;
		}

		while(host_list){
			count += ppoutputparam(host_list,iteration);
			host_list = host_list->next;
		}
	}
	if(iteration>0){
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sCALL \"OCESQLSetHostTable\" USING\n" ," ");
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", iteration);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", length);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", occurs_is_parent);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
		fputs(buff, outfile);
		if(!list->conn_use_other_db){
			sprintf(buff, "OCESQL%5sCALL \"OCESQLExecPrepareOccurs\" USING\n" ," ");
			_printlog("Generate:OCESQLExecPrepareOccurs");
		} else {
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecPrepareOccurs\" USING\n" ," ");
			_printlog("Generate:OCESQLIDExecPrepareOccurs");
		}
	}else{
		if(!list->conn_use_other_db){
			sprintf(buff, "OCESQL%5sCALL \"OCESQLExecPrepare\" USING\n" ," ");
			_printlog("Generate:OCESQLExecPrepare");
		} else {
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecPrepare\" USING\n" ," ");
			_printlog("Generate:OCESQLIDExecPrepare");
		}
	}

	fputs(buff, outfile);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		int iret;
		int var_type;
		int var_len;
		int var_scale;

		iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
		if(iret != 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
		fputs(buff, outfile);
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE \"%s\" & x\"00\"\n"," ",list->prepareName);
	fputs(buff, outfile);

	strcpy(out,"OCESQL ");
	strcat(out,"       ");
	strcat(out,strbyvalue);
	sprintf(str_type,"%d",count);
	strcat(out,str_type);
	outwrite();

	strcpy(out,"OCESQL ");
	strcat(out,"   ");
	strcat(out,strend);
	outwrite();

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLEndSQL\"\n"," ");
	fputs(buff, outfile);

	ppoutputendcall(list);

}

void ppoutputfetch(struct cb_exec_list *list){
	char buff[256];
	struct cb_res_hostreference_list *res_host_list;
	int type, digits, scale;
	int iret;

 	int occurs_is_parent = 0;
	int length = 0;
	int iteration = 0;

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLStartSQL\"\nOCESQL%5sEND-CALL\n"," "," ");
	fputs(buff, outfile);

	res_host_list = list->res_host_list;

	iret = gethostvarianttype(res_host_list->hostreference, &type, &digits, &scale);
	if(iret != 0){
		printf("%s:%d\n", res_host_list->hostreference,iret);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "E%03d",iret);
		printerrormsg(res_host_list->hostreference, res_host_list->lineno,
					  buff, errorfilename);
		return;
	}

	if(type == HVARTYPE_GROUP){
		struct cb_field *parent, *child;

		parent = getfieldbyname(res_host_list->hostreference);
		if(parent == NULL){
			printf("%s:%d\n", res_host_list->hostreference, ERR_NOTDEF_WORKING);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
			printerrormsg(res_host_list->hostreference, res_host_list->lineno,
						  buff, errorfilename);
			return;
		}

		child = parent->children;
		if(parent->occurs){
			iteration = parent->occurs;
			occurs_is_parent = 1;
			ppoutputresgroup(child, res_host_list->lineno, iteration);
			iret = get_host_group_length(child, &length);
			if(iret != 0){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(res_host_list->hostreference, res_host_list->lineno, buff, errorfilename);
				return;
			}
		} else {
			iteration = -1;
			occurs_is_parent = 0;

			iret = get_host_group_table_info(child, &iteration, &length);
			ppoutputresgroup(child, res_host_list->lineno, iteration);
			if(iret != 0){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(res_host_list->hostreference, res_host_list->lineno, buff, errorfilename);
				return;
			}
		}
	} else {
		while(res_host_list)
		{
			iret = gethostvarianttype(res_host_list->hostreference, &type, &digits, &scale);
			if(iret != 0){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(res_host_list->hostreference, res_host_list->lineno,
							  buff, errorfilename);
				return;
			}
			ppoutputresparam(res_host_list->hostreference, type, digits, scale,iteration);
			res_host_list = res_host_list->next;
		}
	}

	if(iteration){
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sCALL \"OCESQLSetHostTable\" USING\n" ," ");
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", iteration);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", length);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", occurs_is_parent);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sCALL \"OCESQLCursorFetchOccurs\" USING\n" ," ");
		fputs(buff, outfile);
		_printlog("Generate:OCESQLCursorFetchOccurs");
	} else {
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sCALL \"OCESQLCursorFetchOne\" USING\n" ," ");
		fputs(buff, outfile);
		_printlog("Generate:OCESQLCursorFetchOne");
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if( list->cursorName == NULL )
		return ;

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE \"%s\" & x\"00\"\n"," ",list->cursorName);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLEndSQL\"\n"," ");
	fputs(buff, outfile);

	ppoutputendcall(list);

}

void ppoutputcommit(struct cb_exec_list *list){
	char buff[256];

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLStartSQL\"\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	if(!list->conn_use_other_db){
		sprintf(buff, "OCESQL%5sCALL \"OCESQLExec\" USING\n", " ");
		_printlog("Generate:COMMIT");
	} else {
		sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExec\" USING\n", " ");
		_printlog("Generate:COMMITuseDBNAME");
	}
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		int iret;
		int var_type;
		int var_len;
		int var_scale;

		iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
		if(iret != 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
		fputs(buff, outfile);
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE \"COMMIT\" & x\"00\"\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLEndSQL\"\n"," ");
	fputs(buff, outfile);

	ppoutputendcall(list);
	return ;
}


void ppoutputrollback(struct cb_exec_list *list){
	char buff[256];

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLStartSQL\"\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	if(!list->conn_use_other_db){
		sprintf(buff, "OCESQL%5sCALL \"OCESQLExec\" USING\n", " ");
		_printlog("Generate:ROLLBACK");
	} else {
		sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExec\" USING\n", " ");
		_printlog("Generate:ROLLBACKuseDBNAME");
	}
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		int iret;
		int var_type;
		int var_len;
		int var_scale;

		iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
		if(iret != 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
		fputs(buff, outfile);
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE \"ROLLBACK\" & x\"00\"\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n"," ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLEndSQL\"\n"," ");
	fputs(buff, outfile);

	ppoutputendcall(list);

}

void ppoutputprepare(struct cb_exec_list *list){
	char buff[256];
	int l,m,n;
	int iret;
	struct cb_field *parent, *child;
	char *comp_varname;

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLPrepare\" USING\n", " ");
	fputs(buff, outfile);
	_printlog("Generate:OCESQLPrepare");

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if( list->cursorName == NULL)
		return ;
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE \"%s\" & x\"00\"\n"," ",list->prepareName);
	fputs(buff, outfile);

	iret = gethostvarianttype(list->host_list->hostreference,&l,&m,&n);
	if(iret!= 0)
	{
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "E%03d",iret);
		printerrormsg(list->host_list->hostreference, list->host_list->lineno, buff, errorfilename);
		return;
	} else if(l != HVARTYPE_GROUP && l != HVARTYPE_ALPHANUMERIC_VARYING){
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "E%03d",ERR_PREPARE_ISNT_GROUP);
		printerrormsg(list->host_list->hostreference, list->host_list->lineno, buff, errorfilename);
		return;
	}

	parent = getfieldbyname(list->host_list->hostreference);
	if(parent == NULL){
	     printf("%s:%d\n", list->host_list->hostreference, ERR_NOTDEF_WORKING);
	     memset(buff, 0, sizeof(buff));
	     sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
	     printerrormsg(list->host_list->hostreference, list->host_list->lineno,
			   buff, errorfilename);
	     return;
	}

	child = parent->children;
	comp_varname = (char *)malloc((strlen(parent->sname) + 4) * sizeof(char));
	if(comp_varname == NULL){
	     return;
	}
	sprintf(comp_varname, "%s-LEN", parent->sname);
	if(strcmp(comp_varname, child->sname) != 0 ||
	   child->sister == NULL){
	     memset(buff, 0, sizeof(buff));
	     sprintf(buff, "E%03d",ERR_PREPARE_INVALID_PARAM);
	     printerrormsg(list->host_list->hostreference, list->host_list->lineno,
			   buff, errorfilename);
	     free(comp_varname);
	     return;
	}

	sprintf(comp_varname, "%s-ARR", parent->sname);
	if(strcmp(comp_varname, child->sister->sname) != 0){
	     memset(buff, 0, sizeof(buff));
	     sprintf(buff, "E%03d",ERR_PREPARE_INVALID_PARAM);
	     printerrormsg(list->host_list->hostreference, list->host_list->lineno,
			   buff, errorfilename);
	     free(comp_varname);
	     return;
	}
	free(comp_varname);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ",child->sister->sname);
	fputs(buff, outfile);
	sprintf(buff, "OCESQL%10sBY VALUE %s\n"," ",child->sname);
	fputs(buff, outfile);

	ppoutputendcall(list);
}

void ppoutputdisconnect(struct cb_exec_list *list){
	char buff[256];

	memset(buff, 0, sizeof(buff));
	if(!list->conn_use_other_db){
		sprintf(buff, "OCESQL%5sCALL \"OCESQLDisconnect\" USING\n", " ");
		_printlog("Generate:OCESQLDisconnect");
	} else {
		sprintf(buff, "OCESQL%5sCALL \"OCESQLIDDisconnect\" USING\n", " ");
		_printlog("Generate:OCESQLDisconnect");
	}
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		int iret;
		int var_type;
		int var_len;
		int var_scale;

		iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
		if(iret != 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
		fputs(buff, outfile);
	}

	ppoutputendcall(list);
}


void ppoutputother(struct cb_exec_list *list){
	char buff[256];
	struct cb_hostreference_list *host_list;

	if( list->hostreferenceCount ==  0)
	{
		memset(buff, 0, sizeof(buff));
		if(!list->conn_use_other_db){
			sprintf(buff, "OCESQL%5sCALL \"OCESQLExec\" USING\n", " ");
			_printlog("Generate:OCESQLExec");
		} else {
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExec\" USING\n", " ");
			_printlog("Generate:OCESQLIDExec");
		}
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
		fputs(buff, outfile);

		if(list->conn_use_other_db){
			int iret;
			int var_type;
			int var_len;
			int var_scale;

			iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
			if(iret != 0)
			{
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(list->dbName, lineNUM, buff, errorfilename);
				return;
			}

			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
			fputs(buff, outfile);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
			fputs(buff, outfile);
		}

		if( list->sqlName == NULL )
			return ;
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->sqlName);
		fputs(buff, outfile);

		ppoutputendcall(list);
		return ;
	}

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLStartSQL\"\nOCESQL%5sEND-CALL\n"," "," ");
	fputs(buff, outfile);

	int length = 0;
	int iteration = 0;
	int occurs_is_parent = 0;

	if((strcasecmp(list->commandName,"INSERT")==0) ||
			(strcasecmp(list->commandName,"DELETE")==0) ||
			(strcasecmp(list->commandName,"UPDATE")==0)){
		struct cb_hostreference_list *res_host_list;
		int iret;

		res_host_list = list->host_list;
		struct cb_field *parent, *child, *current;
		current = getfieldbyname(res_host_list->hostreference);
		if(current == NULL){
			printf("%s:%d\n", res_host_list->hostreference, ERR_NOTDEF_WORKING);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
			printerrormsg(res_host_list->hostreference, res_host_list->lineno,
						  buff, errorfilename);
			return;
		}
		parent = current->parent;
		if(parent == NULL){
			goto exit_occurs_check;
		}

		child = parent->children;
		if(parent->occurs){
			iteration = parent->occurs;
			occurs_is_parent = 1;

			iret = get_host_group_length(child, &length);
			if(iret != 0){
				goto exit_occurs_check;
			}
		} else {
			iteration = -1;
			occurs_is_parent = 0;

			iret = get_host_group_table_info(child, &iteration, &length);
			if(iret != 0){
				goto exit_occurs_check;
			}
		}
	}
exit_occurs_check:
	host_list = list->host_list;
	int count = 0;
	while( host_list)
	{
		count += ppoutputparam(host_list,iteration);
		host_list = host_list->next;
	}

	memset(buff, 0, sizeof(buff));
	if(iteration>0){
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sCALL \"OCESQLSetHostTable\" USING\n" ," ");
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", iteration);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", length);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", occurs_is_parent);
		fputs(buff, outfile);

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
		fputs(buff, outfile);
		if(!list->conn_use_other_db){
			sprintf(buff, "OCESQL%5sCALL \"OCESQLExecParamsOccurs\" USING\n" ," ");
			_printlog("Generate:OCESQLExecParamsOccurs");
		} else {
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecParamsOccurs\" USING\n" ," ");
			_printlog("Generate:OCESQLIDExecParamsOccurs");
		}
	}else{
		if(!list->conn_use_other_db){
			sprintf(buff, "OCESQL%5sCALL \"OCESQLExecParams\" USING\n" ," ");
			_printlog("Generate:OCESQLExecParams");
		} else {
			sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecParams\" USING\n" ," ");
			_printlog("Generate:OCESQLIDExecParams");
		}
	}

	fputs(buff, outfile);
	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE SQLCA\n"," ");
	fputs(buff, outfile);

	if(list->conn_use_other_db){
		int iret;
		int var_type;
		int var_len;
		int var_scale;

		iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
		if(iret != 0)
		{
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "E%03d",iret);
			printerrormsg(list->dbName, lineNUM, buff, errorfilename);
			return;
		}

		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", list->dbName);
		fputs(buff, outfile);
		memset(buff, 0, sizeof(buff));
		sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
		fputs(buff, outfile);
	}

	if( list->sqlName == NULL )
		return ;

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ",list->sqlName);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%10sBY VALUE %d\n", " ", count);
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
	fputs(buff, outfile);

	memset(buff, 0, sizeof(buff));
	sprintf(buff, "OCESQL%5sCALL \"OCESQLEndSQL\"\n"," ");
	fputs(buff, outfile);

	ppoutputendcall(list);
}

int ppoutcontext(struct cb_exec_list *list){
	if( list  == NULL)
		return 1;

	if(  strcmp(list->commandName,"CONNECT")==0){

		ppoutputconnect(list);
		return 1;
	}

	if(  strcmp(list->commandName,"OPEN")==0){

		ppoutputopen(list);
		return 1;
	}

	if(  strcmp(list->commandName,"EXECUTE")==0){

		ppoutputexecprepare(list);
		return 1;
	}

	if(  strcmp(list->commandName,"FETCH")==0){

		ppoutputfetch(list);
		return 1;
	}

	if(  strcmp(list->commandName,"COMMIT")==0){

		ppoutputcommit(list);
		return 1;
	}

	if(  strcmp(list->commandName,"COMMIT_RELEASE")==0){

		ppoutputcommit(list);
		ppoutputdisconnect(list);
		return 1;
	}

	if(  strcmp(list->commandName,"ROLLBACK")==0){

		ppoutputrollback(list);
		return 1;
	}
	if(  strcmp(list->commandName,"ROLLBACK_RELEASE")==0){

		ppoutputrollback(list);
		ppoutputdisconnect(list);
		return 1;
	}
	if(  strcmp(list->commandName,"INSERT")==0){

		ppoutputother(list);
		return 1;
	}
	if(  strcmp(list->commandName,"UPDATE")==0){

		ppoutputother(list);
		return 1;
	}
	if(  strcmp(list->commandName,"DELETE")==0){

		ppoutputother(list);
		return 1;
	}
	if(  strcmp(list->commandName,"PREPARE")==0){

		ppoutputprepare(list);
		return 1;
	}
	if(  strcmp(list->commandName,"DISCONNECT")==0){

		ppoutputdisconnect(list);
		return 1;
	}
	if(  list->command_putother){

		ppoutputother(list);
		return 1;
	}

	return 0;
}

void ppbuff(struct cb_exec_list *list){
	int var_type;
	int var_len;
	int var_scale;
	int count;
	struct cb_exec_list *wk_head;
	struct cb_hostreference_list *wk_host;
	char str_type[BUFFSIZE];
	struct cb_exec_list *l;
	int iret;
	char buff[10];

	l = list;
	if(ppoutcontext(list) == 1 )
		return ;

	if(strcmp(l->commandName,"INCLUDE")==0){
		strcpy(out,"OCESQL ");
		strcat(out,"    ");
		strcat(out,"copy ");
		strcat(out,"\"");
		strcat(out,copypath);
		strcat(out,"\".");
		outwrite();
	}
	if(strcmp(l->commandName,"INCFILE")==0){
		return;
	}
	if(strcmp(l->commandName,"HOST_BEGIN")==0 ||
	   strcmp(l->commandName,"HOST_END")==0 ||
	   strcmp(l->commandName,"WORKING_BEGIN")==0){
		return;
	}

	if(strcmp(l->commandName,"WORKING_END")==0){
		wk_head = l;
		outsqlfiller(wk_head);
	}

	if(strcmp(l->commandName,"VARYING_PARAM")==0){
	     // modify cb_field
	     struct cb_field *vp_parent, *vp_len, *vp_arr;
	     int pstart;
	     int istart;
	     char vtmp[256];
	     vp_parent = l->varname;

	     parameter_split(vp_parent,l->commandName);

	     vp_len = vp_parent->children;
	     vp_arr = vp_len->sister;

	     // get start position
	     for(pstart=6;inbuff[pstart]!='\0';pstart++){
		  if(inbuff[pstart] != ' ')
		       break;
	     }

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     sprintf(vtmp, "%02d", vp_parent->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_parent->sname);
	     strcat(out, ".");
	     outwrite();

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     strcat(out,"  ");
	     sprintf(vtmp, "%02d", vp_len->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_len->sname);
	     strcat(out, " PIC S9(8) COMP-5.");
	     outwrite();

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     strcat(out,"  ");
	     sprintf(vtmp, "%02d", vp_arr->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_arr->sname);
	     if(vp_arr->pictype == PIC_NATIONAL){
	     	strcat(out, " PIC N(");
	     } else {
	     	strcat(out, " PIC X(");
	     }
	     sprintf(vtmp, "%d", vp_arr->picnsize);
	     strcat(out, vtmp);
	     strcat(out, ").");
	     outwrite();
	}

	if(strcmp(l->commandName,"SQL_BYTEA_PARAM")==0){

	     // modify cb_field
	     struct cb_field *vp_parent, *vp_len, *vp_arr;
	     int pstart;
	     int istart;
	     char vtmp[256];
	     vp_parent = l->sqlbyteaname;

	     parameter_split(vp_parent,l->commandName);

	     vp_len = vp_parent->children;
	     vp_arr = vp_len->sister;

	     // get start position
	     for(pstart=6;inbuff[pstart]!='\0';pstart++){
		  if(inbuff[pstart] != ' ')
		       break;
	     }

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     sprintf(vtmp, "%02d", vp_parent->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_parent->sname);
	     strcat(out, ".");
	     outwrite();

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     strcat(out,"  ");
	     sprintf(vtmp, "%02d", vp_len->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_len->sname);
	     strcat(out, " PIC S9(8) COMP-5.");
	     outwrite();

	     strcpy(out,"OCESQL ");
	     for(istart=7; istart<pstart; istart++){
		  strcat(out," ");
	     }
	     strcat(out,"  ");
	     sprintf(vtmp, "%02d", vp_arr->level);
	     strcat(out, vtmp);
	     strcat(out," ");
	     strcat(out, vp_arr->sname);
	     strcat(out, " PIC X(");
	     sprintf(vtmp, "%d", vp_arr->picnsize);
	     strcat(out, vtmp);
	     strcat(out, ").");
	     outwrite();
	}

	if(strcmp(l->commandName,"SELECT")==0){
		if(l->res_host_list == NULL){
			if(l->hostreferenceCount != 0){
				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strcall);
				strcat(out,"\"OCESQLStartSQL\"");
				outwrite();

				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strend);
				outwrite();
				wk_host = l->host_list;
				count = 0;
				for(; wk_host ; wk_host = wk_host->next){
					count += ppoutputparam(wk_host,0);
				}

				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strcall);
				if(strlen(l->cursorName)>0){
					if(l->conn_use_other_db){
						strcat(out,"\"OCESQLIDCursorDeclareParams\"");
						_printlog("Generate:OCESQLIDCursorDeclareParams");
					} else {
						strcat(out,"\"OCESQLCursorDeclareParams\"");
						_printlog("Generate:OCESQLCursorDeclareParams");
					}
				} else{
					if(l->conn_use_other_db){
						strcat(out,"\"OCESQLIDExecParams\"");
						_printlog("Generate:OCESQLIDExecParams");
					} else {
						strcat(out,"\"OCESQLExecParams\"");
						_printlog("Generate:OCESQLExecParams");
					}
				}
				strcat(out,strusing);

			} else {
				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strcall);
				if(strlen(l->cursorName)>0){
					if(l->conn_use_other_db){
						strcat(out,"\"OCESQLIDCursorDeclare\"");
						_printlog("Generate:OCESQLIDCursorDeclare");
					} else {
						strcat(out,"\"OCESQLCursorDeclare\"");
						_printlog("Generate:OCESQLCursorDeclare");
					}
				} else {
					if(l->conn_use_other_db){
						strcat(out,"\"OCESQLIDExec\"");
						_printlog("Generate:OCESQLIDExec");
					} else {
						strcat(out,"\"OCESQLExec\"");
						_printlog("Generate:OCESQLExec");
					}
				}
				strcat(out,strusing);
			}
			outwrite();

			strcpy(out,"OCESQL ");
			strcat(out,"       ");
			strcat(out,strsqlca);
			outwrite();

			if(list->conn_use_other_db){
				iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
				if(iret != 0)
				{
					memset(buff, 0, sizeof(buff));
					sprintf(buff, "E%03d",iret);
					printerrormsg(list->dbName, lineNUM, buff, errorfilename);
					return;
				}

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", l->dbName);
				fputs(buff, outfile);
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
				fputs(buff, outfile);
			}

			if(strlen(l->cursorName)>0){
				strcpy(out,"OCESQL ");
				strcat(out,"       ");
				strcat(out,strreference);
				strcat(out,"\"");
				strcat(out,l->cursorName);
				strcat(out,"\"");
				strcat(out," & x\"00\"");
				outwrite();
			}

			strcpy(out,"OCESQL ");
			strcat(out,"       ");
			strcat(out,strreference);
			strcat(out,l->sqlName);
			outwrite();
			if(l->hostreferenceCount != 0){
				strcpy(out,"OCESQL ");
				strcat(out,"       ");
				strcat(out,strbyvalue);
				sprintf(str_type,"%d",count);
				strcat(out,str_type);
				outwrite();

				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strend);
				outwrite();

				strcpy(out,"OCESQL ");
				strcat(out,"   ");
				strcat(out,strcall);
				strcat(out,"\"OCESQLEndSQL\"");
				outwrite();
			}
			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strend);
			if( l->period)
				strcat(out,".");

			outwrite();
		} else { // SELECT INTO
			struct cb_res_hostreference_list *wk_res_host;
			int length = 0;
			int iteration = 0;
			int reshostreferenceCount = 0;
			int occurs_is_parent = 0;

			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strcall);
			strcat(out,"\"OCESQLStartSQL\"");
			outwrite();

			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strend);
			outwrite();

			wk_res_host = l->res_host_list;
			iret = gethostvarianttype(wk_res_host->hostreference ,
									  &var_type, &var_len, &var_scale);

			if(iret != 0){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(wk_res_host->hostreference, wk_res_host->lineno,
							  buff, errorfilename);
				return;
			}

			if(var_type == HVARTYPE_GROUP){
				struct cb_field *parent, *child;

				parent = getfieldbyname(wk_res_host->hostreference);
				if(parent == NULL){
					printf("%s:%d\n", wk_res_host->hostreference, ERR_NOTDEF_WORKING);
					memset(buff, 0, sizeof(buff));
					sprintf(buff, "E%03d",ERR_NOTDEF_WORKING);
					printerrormsg(wk_res_host->hostreference, wk_res_host->lineno,
								  buff, errorfilename);
					return;
				}

				child = parent->children;

				if(parent->occurs){
					iteration = parent->occurs;
					occurs_is_parent = 1;
					iret = get_host_group_length(child, &length);
					if(iret != 0){
						memset(buff, 0, sizeof(buff));
						sprintf(buff, "E%03d",iret);
						printerrormsg(wk_res_host->hostreference, wk_res_host->lineno,
									  buff, errorfilename);
						return;
					}
				} else {
					occurs_is_parent = 0;
					iteration = -1;

					iret = get_host_group_table_info(child, &iteration, &length);
					if(iret != 0){
						memset(buff, 0, sizeof(buff));
						sprintf(buff, "E%03d",iret);
						printerrormsg(wk_res_host->hostreference, wk_res_host->lineno,
									  buff, errorfilename);
						return;
					}
				}

				while(child != NULL){
					iret = gethostvarianttype(child->sname, &var_type, &var_len, &var_scale);
					if(iret != 0){
						memset(buff, 0, sizeof(buff));
						sprintf(buff, "E%03d",iret);
						printerrormsg(child->sname, wk_res_host->lineno, buff, errorfilename);
						return;
					}
					ppoutputresparam(child->sname, var_type, var_len, var_scale,iteration);
					child = child->sister;
					reshostreferenceCount++;
				}
			} else {
				while(wk_res_host)
				{
					iret = gethostvarianttype(wk_res_host->hostreference,
											  &var_type, &var_len, &var_scale);
					if(iret != 0){
						memset(buff, 0, sizeof(buff));
						sprintf(buff, "E%03d",iret);
						printerrormsg(wk_res_host->hostreference, wk_res_host->lineno,
									  buff, errorfilename);
						return;
					}

					ppoutputresparam(wk_res_host->hostreference, var_type, var_len, var_scale, 0);
					reshostreferenceCount++;
					wk_res_host = wk_res_host->next;
				}
			}
			count = 0;
			wk_host = l->host_list;
			for(; wk_host ; wk_host = wk_host->next){
				count += ppoutputparam(wk_host,iteration);
			}

			if(iteration){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%5sCALL \"OCESQLSetHostTable\" USING\n" ," ");
				fputs(buff, outfile);

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", iteration);
				fputs(buff, outfile);

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", length);
				fputs(buff, outfile);

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", occurs_is_parent);
				fputs(buff, outfile);

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%5sEND-CALL\n", " ");
				fputs(buff, outfile);

				memset(buff, 0, sizeof(buff));
				if(l->conn_use_other_db){
					sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecSelectIntoOccurs\" USING\n" ," ");
					_printlog("Generate:OCESQLIDExecSelectIntoOccurs");
				} else {
					sprintf(buff, "OCESQL%5sCALL \"OCESQLExecSelectIntoOccurs\" USING\n" ," ");
					_printlog("Generate:OCESQLExecSelectIntoOccurs");
				}
				fputs(buff, outfile);
			} else {
				memset(buff, 0, sizeof(buff));
				if(l->conn_use_other_db){
					sprintf(buff, "OCESQL%5sCALL \"OCESQLIDExecSelectIntoOne\" USING\n" ," ");
					_printlog("Generate:OCESQLIDExecSelectIntoOne");
				} else {
					sprintf(buff, "OCESQL%5sCALL \"OCESQLExecSelectIntoOne\" USING\n" ," ");
					_printlog("Generate:OCESQLExecSelectIntoOne");
				}
				fputs(buff, outfile);
			}

			strcpy(out,"OCESQL ");
			strcat(out,"       ");
			strcat(out,strsqlca);
			outwrite();

			if(list->conn_use_other_db) {
				iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
				if(iret != 0)
				{
					memset(buff, 0, sizeof(buff));
					sprintf(buff, "E%03d",iret);
					printerrormsg(list->dbName, lineNUM, buff, errorfilename);
					return;
				}

				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", l->dbName);
				fputs(buff, outfile);
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
				fputs(buff, outfile);
			}

			strcpy(out,"OCESQL ");
			strcat(out,"       ");
			strcat(out,strreference);
			strcat(out,l->sqlName);
			outwrite();

			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY VALUE %d\n", " ", count);
			fputs(buff, outfile);

			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY VALUE %d\n", " ", reshostreferenceCount);
			fputs(buff, outfile);

			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strend);
			outwrite();

			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strcall);
			strcat(out,"\"OCESQLEndSQL\"");
			outwrite();

			strcpy(out,"OCESQL ");
			strcat(out,"   ");
			strcat(out,strend);
			if( l->period)
				strcat(out,".");

			outwrite();
		}
	} else if(l->prepareName[0] != '\0'){
		// DECLARE cursor for prepare
		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		if(l->conn_use_other_db){
			strcat(out,"\"OCESQLIDPreparedCursorDeclare\"");
			_printlog("Generate:OCESQLIDPreparedCursorDeclare");
		} else {
			strcat(out,"\"OCESQLPreparedCursorDeclare\"");
			_printlog("Generate:OCESQLPreparedCursorDeclare");
		}
		strcat(out,strusing);
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strsqlca);
		outwrite();

		if(list->conn_use_other_db){
			iret = gethostvarianttype(list->dbName,&var_type, &var_len, &var_scale);
			if(iret != 0)
			{
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",iret);
				printerrormsg(list->dbName, lineNUM, buff, errorfilename);
				return;
			}

			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY REFERENCE %s\n"," ", l->dbName);
			fputs(buff, outfile);
			memset(buff, 0, sizeof(buff));
			sprintf(buff, "OCESQL%10sBY VALUE %d\n"," ", var_len);
			fputs(buff, outfile);
		}

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strreference);
		strcat(out,"\"");
		strcat(out,l->cursorName);
		strcat(out,"\"");
		strcat(out," & x\"00\"");
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strreference);
		strcat(out,"\"");
		strcat(out,l->prepareName);
		strcat(out,"\"");
		strcat(out," & x\"00\"");
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strend);
		if( l->period)
			strcat(out,".");
		outwrite();
	}

	if(strcmp(l->commandName, "CLOSE")==0){


		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		strcat(out,strcall);
		strcat(out,"\"OCESQLCursorClose\" ");
		strcat(out,strusing);
		outwrite();
		_printlog("Generate:OCESQLCursorClose");

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strsqlca);
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"       ");
		strcat(out,strreference);
		strcat(out,"\"");
		strcat(out,l->cursorName);
		strcat(out,"\"");
		strcat(out," & x\"00\"");
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(outbuff,"   ");
		strcat(outbuff,strend);
		outwrite();

		strcpy(out,"OCESQL ");
		strcat(out,"   ");
		if( l->period)
			strcat(out,".");
		outwrite();
	}
	return;
}

void ppbuff_incfile(struct cb_exec_list *list){
	struct cb_exec_list *l;
	int eof_flag;
	char buff[10];

	l = list;

	if(strcmp(l->commandName,"INCFILE")==0){
		char filename[512];
		FILE *incf;
		char incf_buff[BUFFSIZE + 1];
		//int retcode;
		char *retcode;

		memset(filename, 0, 512);

		if(include_path){
			sprintf(filename, "%s/", include_path);
		}
		strcat(filename, l->incfileName);

 		incf = fopen_or_die(filename, "r");
		eof_flag = 0;
		while(1){
			memset(incf_buff, 0, BUFFSIZE + 1);
			fgets(incf_buff, BUFFSIZE, incf);
			if(feof(incf)) {
				eof_flag = 1;
				//break;
			}
			if(strlen(incf_buff) > MAX_LINESIZE){
				memset(buff, 0, sizeof(buff));
				sprintf(buff, "E%03d",ERR_EXCEED_LIMIT_LINE_LENGTH);
				printerrormsg("", lineNUM, buff, errorfilename);
			}
			strcpy(out,"OCESQL");
			strcat(out, incf_buff + strlen("OCESQL"));
			//retcode = strlen(incf_buff);
			//if(retcode > strlen("OCESQL")){
			//	out[retcode-1] = '\0';
			//}
			retcode = strstr(out, "\n");
			if(retcode != NULL && retcode != incf_buff){
				*retcode = '\0';
			}
			
			outwrite();
			if(eof_flag){
				break;
			}
		}
		return;
	}
	return;
}

void outwrite(){
	size_t len;

	outbuff = out;
	len = strlen(outbuff);
	fwrite (outbuff ,len, 1 , outfile );
	fputc('\n',outfile);
}

void ppoutput(char *ppin,char *ppout,struct cb_exec_list *head){
	FILE *readfile;
	size_t len;

	struct cb_exec_list *l;
	l = head;
	lineNUM = 0;
	EOFflg = 0;

 	readfile = fopen_or_die(ppin,"r");
 	outfile  = fopen_or_die(ppout,"w");

	EOFFLG = 0;
	if (readfile && outfile){
		for(;EOFflg != 1;){
			readline(readfile);
			if(head){
				if (l->startLine<= lineNUM && l->endLine>=lineNUM){
					if(strcmp(l->commandName,"WORKING_END")==0){
						ppbuff(l);
					}

					if(strcmp(l->commandName,"WORKING_BEGIN")!=0 &&
					   strcmp(l->commandName,"WORKING_END") != 0){
						inbuff[0] = 'O';
						inbuff[1] = 'C';
						inbuff[2] = 'E';
						inbuff[3] = 'S';
						inbuff[4] = 'Q';
						inbuff[5] = 'L';
						inbuff[6] = '*';
					}

					outbuff = inbuff;
					len = strlen(outbuff);
					fwrite (outbuff ,len, 1 , outfile );

					if (EOFflg == 1){
						fputc('\n',outfile);

					}
				}
				else{
					if(lineNUM - l->endLine == 1){
						if(strcmp(l->commandName,"WORKING_END")){
							ppbuff(l);
						}
						if (l->next != NULL)
							l = l->next;

						if(l->startLine<= lineNUM && l->endLine>=lineNUM &&
						   (strcmp(l->commandName,"WORKING_BEGIN")!=0 &&
						    strcmp(l->commandName,"WORKING_END")!=0)){
							inbuff[0] = 'O';
							inbuff[1] = 'C';
							inbuff[2] = 'E';
							inbuff[3] = 'S';
							inbuff[4] = 'Q';
							inbuff[5] = 'L';
							inbuff[6] = '*';
						}
						outbuff = inbuff;
						len = strlen(outbuff);
						fwrite (outbuff ,len, 1 , outfile );
					}else{
						outbuff = inbuff;
						len = strlen(outbuff);
						fwrite (outbuff ,len, 1 , outfile );

					}
				}
			}else{
				outbuff = inbuff;
				len = strlen(outbuff);
				fwrite (outbuff ,len, 1 , outfile );
			}
		}
	}

	fclose(readfile);
	fclose(outfile);
}

void ppoutput_incfile(char *ppin,char *ppout,struct cb_exec_list *head){
	FILE *readfile;
	size_t len;

	struct cb_exec_list *l;
	l = head;

 	readfile = fopen_or_die(ppin,"r");
 	outfile  = fopen_or_die(ppout,"w");

	EOFFLG = 0;
	if (readfile && outfile){
		for(;EOFflg != 1;){
			readline(readfile);
			if(head){
				if (l->startLine<= lineNUM && l->endLine>=lineNUM){
					if (strcmp(l->commandName, "INCFILE") == 0){
						inbuff[0] = 'O';
						inbuff[1] = 'C';
						inbuff[2] = 'E';
						inbuff[3] = 'S';
						inbuff[4] = 'Q';
						inbuff[5] = 'L';
						inbuff[6] = '*';
					}

					outbuff = inbuff;
					len = strlen(outbuff);
					fwrite (outbuff ,len, 1 , outfile );

					if (EOFflg == 1){
						fputc('\n',outfile);

					}
				}
				else{
					if(lineNUM - l->endLine == 1){
						if(strcmp(l->commandName,"INCFILE")==0){
							ppbuff_incfile(l);
						}
						if (l->next != NULL)
							l = l->next;

						if(l->startLine<= lineNUM && l->endLine>=lineNUM &&
						   (strcmp(l->commandName, "INCFILE") == 0)){
							inbuff[0] = 'O';
							inbuff[1] = 'C';
							inbuff[2] = 'E';
							inbuff[3] = 'S';
							inbuff[4] = 'Q';
							inbuff[5] = 'L';
							inbuff[6] = '*';
						}
						outbuff = inbuff;
						len = strlen(outbuff);
						fwrite (outbuff ,len, 1 , outfile );
					}else{
						outbuff = inbuff;
						len = strlen(outbuff);
						fwrite (outbuff ,len, 1 , outfile );

					}
				}
			}else{
				outbuff = inbuff;
				len = strlen(outbuff);
				fwrite (outbuff ,len, 1 , outfile);
			}
		}
	}
	fclose(readfile);
	fclose(outfile);
}

int check_Dchar(char c){
	unsigned char uc = (unsigned char)c;
	if(uc >= 0x81 && uc <= 0x9f){
		return 1;
	}else if(uc >= 0xe0 && uc <= 0xef){
		return 1;
	}
	return 0;
}

int get_host_group_length(struct cb_field *field, int *length){
	if(field == NULL) return 0;


	if((field->pictype == PIC_NATIONAL) ||
	   (field->pictype == PIC_NATIONAL_VARYING)){
		*length += field->picnsize * 2;
	}else{
		if(field->usage == USAGE_PACKED){
			*length += field->picnsize / 2 + 1;
		}else{
			*length += field->picnsize;
		}
	}

	return get_host_group_length(field->sister, length);
}

int get_host_group_table_info(struct cb_field *field, int *iteration, int *length){
	if(field == NULL) return 0;

	if(field->occurs){
		if(*iteration == -1 || field->occurs < *iteration){
			*iteration = field->occurs;
		}
	} else {
		*iteration = 0;
	}
	if((field->pictype == PIC_NATIONAL) ||
	   (field->pictype == PIC_NATIONAL_VARYING)){
		*length += field->picnsize * 2;
	}else{
		*length += field->picnsize;
	}

	return get_host_group_table_info(field->sister, iteration, length);
}

void parameter_split(struct cb_field *vp_parent,char *commandName){
	struct cb_field *vp_len, *vp_arr;
	char *basename;
	int varlen;
	int vartype;

	vp_len = malloc(sizeof(struct cb_field));
	vp_arr = malloc(sizeof(struct cb_field));
	if( vp_len == NULL || vp_arr == NULL){
	     printf("parameter_split: memory allocation for cb_field failed.\n");
	     goto die_parameter_split;
	     return;
	}

	memset(vp_len, 0 ,sizeof(struct cb_field));
	memset(vp_arr, 0 ,sizeof(struct cb_field));

	basename = vp_parent->sname;
	varlen = vp_parent->picnsize;
	vartype = vp_parent->pictype;

	// vp_len
	vp_len->sname = (char *)malloc((strlen(basename) + strlen("-LEN") + TERMINAL_LENGTH) * sizeof(char));
	if(vp_len->sname == NULL){
	     printf("parameter_split: memory allocation for vp_len->sname failed.\n");
	     goto die_parameter_split;
	     return;
	}
	sprintf(vp_len->sname, "%s-LEN", basename);
	vp_len->level = vp_parent->level + 1;
	vp_len->parent = vp_parent;
	vp_len->pictype = PIC_NUMERIC;
	vp_len->usage = USAGE_BINARY_NATIVE;
	vp_len->have_sign = 1;
	vp_len->picnsize = 10;
	vp_parent->children = vp_len;

	// vp_arr
	vp_arr->sname = (char *)malloc((strlen(basename) + strlen("-ARR") + TERMINAL_LENGTH) * sizeof(char));
	if(vp_arr->sname == NULL){
	     printf("parameter_split: memory allocation for vp_arr->sname failed.\n");
	     goto die_parameter_split;
	     return;
	}
	sprintf(vp_arr->sname, "%s-ARR", basename);
	vp_arr->level = vp_parent->level + 1;
	vp_arr->pictype = vartype;
	vp_arr->picnsize = varlen;
	vp_arr->parent = vp_parent;
	vp_len->sister = vp_arr;

    // bytea
    if(strcmp(commandName,"SQL_BYTEA_PARAM")==0){
		vp_parent->pictype = PIC_SQL_BYTEA;
	}
	else{
		// vp_parent
		if(vp_parent->pictype == PIC_NATIONAL){
		     vp_parent->pictype = PIC_NATIONAL_VARYING;
		} else {
		     vp_parent->pictype = PIC_ALPHANUMERIC_VARYING;
		}
    }
    
	return;

die_parameter_split:
	if(vp_len){
	     if(vp_len->sname) free(vp_len->sname);
	     free(vp_len);
	}
	if(vp_arr){
	     if(vp_arr->sname) free(vp_arr->sname);
	     free(vp_arr);
	}
	exit(-1);
	return;
}



FILE* fopen_or_die(char *filename, const char *mode){
	FILE* retval;
	retval = fopen(filename, mode);

	if(retval == NULL){
		perror(filename);
		exit(-1);
	}

	return retval;
}

void _printlog(char *msg){
	printf("%s\n", msg);
}
