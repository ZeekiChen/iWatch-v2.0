#ifndef _OLED_LOG_H
#define _OLED_LOG_H

#include "oled_conf.h"

#if (OLED_USE_LOG == 1)

#include <stdio.h>

#ifdef	_OLED_LOG_C
#define	LOG_EXTERN
#else
#define	LOG_EXTERN	extern
#endif

LOG_EXTERN int16_t log_cnt;
#ifdef	_OLED_LOG_C
int16_t log_cnt = 1;
#endif

#define	OG_PRINTERROR(...)		\
do{															\
	printf("error%d: "__VA_ARGS__" at [file:%s, line:%d]\n", log_cnt++, __FILE__, __LINE__);	\
}while(0)
#define	OG_LOG(...)	\
do{										\
	printf(__VA_ARGS__);\
}while(0)
#endif
#endif

