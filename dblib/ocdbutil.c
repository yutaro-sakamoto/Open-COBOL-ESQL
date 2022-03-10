#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <math.h>
#include "ocdbutil.h"
#include "ocdblog.h"

char type_tc_negative_final_number[] =
{
	'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y'
//     '{', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R'
};
static int type_tc_negative_final_number_len =
	sizeof(type_tc_negative_final_number)/sizeof(type_tc_negative_final_number[0]);

/*
 * <Function name>
 *   insert_decimal_point
 *
 * <Outline>
 *   powerで指定した位置に小数点を挿入する
 *
 * <Input>
 *   @data: 挿入対象
 *   @data_size: dataに割り当てられたサイズ(バイト単位)
 *   @power: 小数点以下の桁数(負の値)
 */
void insert_decimal_point(char *data, int data_size, int power){
	int before_length, after_length;
	before_length = strlen(data);
	after_length = strlen(data) + 1;

	int n_decimal_places = -power;

	// check size of data
	//LOG("%d %d %d %d\n",data_size, before_length, after_length, n_decimal_places);
	if(data_size < after_length){
		return;
	} else if(n_decimal_places <= 0 || n_decimal_places >= before_length){
		return;
	}

	memmove(data + (after_length-n_decimal_places), data + (before_length-n_decimal_places),
			n_decimal_places * sizeof(char));
	data[before_length - n_decimal_places] = '.';
}

/*
 * <Function name>
 *   type_tc_is_positive
 *
 * <Outline>
 *   OCDB_TYPE_SIGNED_NUMBER_TCのデータが正負であるかを判別し、
 *   負の値の場合は符号を取り除いた数値で引数を上書きする
 *   もし該当する数値が存在しない場合は、0をセットした上でtrueを返す
 *
 * <Input>
 *   @lastchar: 判別対象の文字
 *
 * <Output>
 *   判別対象が正 : true
 *   判別対象が負 : false
 *
 */
int type_tc_is_positive(char *lastchar){
	int i;

	if(*lastchar >= '0' &&  *lastchar <= '9')
		return true;

	for(i=0; i<type_tc_negative_final_number_len; i++){
		if(*lastchar == type_tc_negative_final_number[i]){
			char tmp[2];
			sprintf(tmp, "%d", i);
			*lastchar = tmp[0];
			return false;
		}
	}

	LOG("no final_number found: %c\n", *lastchar);
	*lastchar = 0;
	return true;
}

/*
 * <Function name>
 *   ocdb_getenv
 *
 * <Outline>
 *   環境変数から値を取得する。ない場合はエラーログを残した上でNULLを返す
 *
 * <Input>
 *   @param: パラメータ名
 *   @def  : default value
 *
 * <Output>
 *   success: パラメータの値
 *   failure: default value
 */
char *ocdb_getenv(char *param, char *def){
	char *env;

	if(param == NULL){
		ERRLOG("parameter is NULL\n");
		return def;
	}

	env = getenv(param);
	if(env == NULL){
		LOG("param '%s' is not set. set default value. \n", param);
		return def;
	} else {
		LOG("param '%s' is %s. \n", param, env);
	}
	return env;
}



char *
uint_to_str(int i){
	int tmp = i;
	int dig = 0;
	char *ret;
	int len;

	if(i < 0) return NULL;
	do{
		dig++;
		tmp = tmp/10;
	}while(tmp > 0);

	len = dig+TERMINAL_LENGTH;
	if((ret = (char *)calloc(len,sizeof(char))) ==NULL){
		return NULL;
	}
	sprintf(ret,"%d",i);
	return ret;
}

char *
oc_strndup(char *src, int n){
	char *ret;

	if(n < 0){
		return NULL;
	}
	ret = (char *)malloc(sizeof(char) * (n + 1));
	if(!src){
		return NULL;
	}

	memcpy(ret,src,n);
	ret[n] = '\0';

	return ret;
}

char *
get_str_without_after_space(char *target){
	char *pos;

	if(!target){
		return NULL;
	}

//	pos = target + strlen(target) - 1;
//	for(;pos >= target;pos--){
//		if(*pos != ' ')
//			break;
//		*pos = '\0';
//	}
	for(pos = target; *pos; pos++){
		if(*pos == ' '){
			*pos = '\0';
			break;
		}
	}

	return target;
}

typedef struct sql_str_list{
	char *tok;
	struct sql_str_list *next;
} SQLSTRLIST;

SQLSTRLIST *
new_sql_list(){
	SQLSTRLIST *p;
	p = (SQLSTRLIST *)malloc(sizeof(SQLSTRLIST));
	if(!p){
		return NULL;
	}
	p->tok = NULL;
	p->next = NULL;

	return p;
}

void
add_sql_list(SQLSTRLIST *list, char *tok){
	SQLSTRLIST *p = list;

	while(p->next != NULL){
		p = p->next;
	}

	p->next = new_sql_list();
	p->tok = tok;
}

void
clear_sql_list(SQLSTRLIST *list){
	SQLSTRLIST *p = list;
	SQLSTRLIST *p_next;

	while(p != NULL){
		p_next = p->next;
		if(p->tok) free(p->tok);
		free(p);
		p = p_next;
	}
}

char *
get_str_replace_hostvalue(char *target, int *nParams){
	char *tmptarget, *space, *start, *now, *rep;
	int length;
	int hoststat;
	char *retstr;
	char buff[BUFFSIZE];

	LOG("src:'%s'\n",target);

	SQLSTRLIST *slist = new_sql_list();
	SQLSTRLIST *p;

	*nParams = 0;
	length = 0;

	if(slist == NULL){
		return NULL;
	}

	tmptarget = strdup(target);

	now = tmptarget;
	start = now;
	hoststat = 0;
	while(*now != '\0'){
		if(hoststat){
			if((*now == ',') ||
				(*now == ')') ||
				(*now == ' ')
			){
				*nParams = *nParams + 1;
				sprintf(buff,"$%d\0",*nParams);
				rep = strdup(buff);
				length += strlen(rep);
				add_sql_list(slist, rep);
				start = now;
				hoststat = 0;
			}
		}else{
			if(*now == ':'){
				rep = oc_strndup(start,now - start);
				length += strlen(rep);
				add_sql_list(slist, rep);
				start = now;
				hoststat = 1;
			}
		}
		now++;
	}
	if(now != start){
		if(hoststat){
			*nParams = *nParams + 1;
			sprintf(buff,"$%d\0",*nParams);
			rep = strdup(buff);
		}else{
			rep = strdup(start);
		}
		length += strlen(rep);
		add_sql_list(slist, rep);
	}


	retstr = (char *)calloc(length + 1,sizeof(char));

	p = slist;
	while(p->next != NULL){
		strcat(retstr, p->tok);
		p = p->next;
	}

	clear_sql_list(slist);
	free(tmptarget);
	LOG("dest:'%s'\n",retstr);
	return retstr;
}

int get_endian(){
	int x=1;
	int retval = BIGENDIAN;

	if(*(char *)&x == 1) retval = LITTLEENDIAN;

	return retval;
}

int is_low_value(char *target, int size){
	int i;
	for(i=0;i<size;i++){
		if(target[i] != '\0'){
			return 0;
		}
	}
	return 1;
}


char *ocdb_itoa(const char *format, int i){
	static a[32];
	memset(a, 0, 32);
	sprintf(a, format, i);
	return a;
}


void oc_memrep(char *src, int n, int mode){
	int i;
	int y;

    for(i=0;i<=n;i++){
      if(mode == 1) {
        if(src[i] == 0x00) {
          src[i] = 0x7f;
        }
      } else {
        if(src[i] == 0x7f) {
          src[i] = 0x00;
        }
      }

	}

}

