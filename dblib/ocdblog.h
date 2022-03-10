#ifndef OCDBLOG_H /* OCDBLOG_H */
#define OCDBLOG_H

#define LOG_OUTPUT_NOTHING 1
#define LOG_OUTPUT_ERR 2
#define LOG_OUTPUT_DEBUG 3

// load parameter for debug
static int loglevel = 0;
static char* logfile = NULL;

#define LOG(...) {OCLOG(__FILE__, __FUNCTION__,__VA_ARGS__);}
#define ERRLOG(...) {OCERRLOG(__FILE__, __FUNCTION__,__VA_ARGS__);}

void OCLOG(const char *, const char *, const char *, ...);
void OCERRLOG(const char *, const char *, const char *, ...);
static void get_loglevel();
static void get_logfile();

#endif
