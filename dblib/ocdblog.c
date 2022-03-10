#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "ocdblog.h"

void OCLOG(const char *file, const char *func, const char *format, ...){
	if(!loglevel){
		get_loglevel();
		get_logfile();
	}

	if(loglevel == LOG_OUTPUT_DEBUG){
		// output log
		time_t clock;
		FILE *fp;
		va_list args;
		time(&clock);

		fp = fopen(logfile, "a");
		if(fp != NULL){
			fprintf(fp, "[%s]#DEBUG# %s:%s(): ", strtok(ctime(&clock), "\r\n"), file, func);
			va_start(args, format);
			vfprintf(fp, format, args);
			va_end(args);
			fclose(fp);
		}
	}
}

void OCERRLOG(const char *file, const char *func, const char *format, ...){
	if(!loglevel){
		get_loglevel();
		get_logfile();
	}

	if(loglevel >= LOG_OUTPUT_ERR){
		time_t clock;
		FILE *fp;
		va_list args;
		time(&clock);

		fp = fopen(logfile, "a");
		if(fp != NULL){
			fprintf(fp, "[%s]#ERROR# %s:%s(): ", strtok(ctime(&clock), "\r\n"), file, func);
			va_start(args, format);
			vfprintf(fp, format, args);
			va_end(args);
			fclose(fp);
		}
	}
}

static void get_loglevel(){
	char *strenv;

	char *tmp = getenv("OCDB_LOGLEVEL");
	if(tmp != NULL){
		strenv = strdup(tmp);
		if(strenv != NULL){
			if(!strcmp(strenv, "NOLOG") || !strcmp(strenv, "nolog")){
				loglevel = LOG_OUTPUT_NOTHING;
			} else if(!strcmp(strenv, "ERR") || !strcmp(strenv, "err")){
				loglevel = LOG_OUTPUT_ERR;
			} else if(!strcmp(strenv, "DEBUG") || !strcmp(strenv, "debug")){
				loglevel = LOG_OUTPUT_DEBUG;
			}
			free(strenv);
		}else{
			loglevel = LOG_OUTPUT_NOTHING;
		}
	} else {
		loglevel = LOG_OUTPUT_NOTHING;
	}
}

static void get_logfile(){
	char *strenv;
	const char defpath[] = "/tmp/ocesql.log";

	if(logfile != NULL){
		return;
	}

	char *tmp = getenv("OCDB_LOGFILE");
	if(tmp != NULL){
		strenv = strdup(tmp);
	} else {
		strenv = strdup(defpath);
	}
	logfile = strenv;
}
