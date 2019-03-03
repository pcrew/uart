#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <errno.h>
#include <string.h>

#define __log_msg(TIMESTAMP, LOG, ...)					\
{									\
	fprintf(stdout, TIMESTAMP LOG __VA_ARGS__);			\
	fflush(stdout);							\
} while (0);	

#define log_dbg_msg(...) __log_msg( __TIMESTAMP__ , " DBG: ", __VA_ARGS__)
#define log_err_msg(...) __log_msg( , "ERR: ", __VA_ARGS__)
#define log_sys_msg(...) __log_msg( , "SYS: ", __VA_ARGS__)
#define log_inf_msg(...) __log_msg( , "INF: ", __VA_ARGS__)
#define log_wrn_msg(...) __log_msg( , "WRN: ", __VA_ARGS__)
#endif
