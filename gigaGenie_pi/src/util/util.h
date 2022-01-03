/*
 * GiGA IoTMakers version 1.0
 *
 *  Copyright (c) 2016 kt corp. All rights reserved.
 *
 *  This is a proprietary software of kt corp, and you may not use this file except in
 *  compliance with license agreement with kt corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of kt corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 */

#ifndef IOTMAKERS_UTIL_H
#define IOTMAKERS_UTIL_H


/*
 * ********************************************************************************
 * util.h
 * ********************************************************************************
 */

#if defined(WIN32)
#include <Windows.h>

#define sleep(n) Sleep(n*1000)

#undef htons
#undef htonl
#undef ntohs
#undef ntohl

#define HTONS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))
#define NTOHS(n) (((((unsigned short)(n) & 0xFF)) << 8) | (((unsigned short)(n) & 0xFF00) >> 8))

#define HTONL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define NTOHL(n) (((((unsigned long)(n) & 0xFF)) << 24) | \
                  ((((unsigned long)(n) & 0xFF00)) << 8) | \
                  ((((unsigned long)(n) & 0xFF0000)) >> 8) | \
                  ((((unsigned long)(n) & 0xFF000000)) >> 24))

#define htons HTONS 
#define htonl HTONL 
#define ntohs NTOHS 
#define ntohl NTOHL 
#endif

#ifdef __cplusplus
extern "C"
{
#endif

long im_util_gettimeofday_as_sec();
long im_util_gettimeofday_as_usec();
int im_util_strftime_on_sec(long sec, char *strdate, char *fmt);
char *im_util_strftime_now(char *buff, char *fmt);
int im_util_gettime_now_msec();
long long im_util_get_unique_number() ;
int im_util_str_trim(char *szStr, int nLen);
int im_util_read_config(const char* config, char* ip, unsigned short* port, 
	char* extrSysId, char* deviceId, char* athnRqtNo, char* log_file_name);

unsigned long im_util_get_secs();


#ifdef __cplusplus
}
#endif


#endif
