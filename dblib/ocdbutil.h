#ifndef OCDBUTIL_H
#define OCDBUTIL_H

#define SIGN_LENGTH 1
#define TERMINAL_LENGTH 1
#define DECIMAL_LENGTH 1

#define BIGENDIAN 1
#define LITTLEENDIAN 2

#define MEM_FREE(x) {free(x); x=NULL;}

#define BUFFSIZE 256

extern char type_tc_negative_final_number[];

void insert_decimal_point(char *, int, int);
int type_tc_is_positive(char *);
char *ocdb_getenv(char *, char *);
char *uint_to_str(int);
char *oc_strndup(char *, int);
char *get_str_without_after_space(char *);
char *get_str_replace_hostvalue(char *, int *);
int get_endian();
int is_low_value(char *, int);
char *ocdb_itoa(const char *, int);

void oc_memrep(char *, int , int);

#endif
