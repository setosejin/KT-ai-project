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

#ifndef IOTMAKERS_LOG_H
#define IOTMAKERS_LOG_H

#if defined(ARDUINO)
#define IOTMAKERS_MAX_LOG_MSG_LEN			80
#elif defined(_LINUX_)
#define IOTMAKERS_MAX_LOG_MSG_LEN			4096
#else
#define IOTMAKERS_MAX_LOG_MSG_LEN			4096
#endif

#define LOG_LEVEL_ERROR   1                    /* Log level  : Error               */
#define LOG_LEVEL_INFO    2                    /* Log level  : Information         */
#define LOG_LEVEL_DEBUG   3                    /* Log level  : Debug               */

extern int __bas_util_log_level;

// filename, line, function
//#define BAS_LOG_PREFIX_FRM		"%-12.12s %04d %-10.10s"
#define BAS_LOG_PREFIX_FRM		"%s %04d %-10.10s"

#define ERROR_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_ERROR ) im_log("ERR "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#define INFO_LOG(format,...)  do{if( __bas_util_log_level >= LOG_LEVEL_INFO  ) im_log("INF "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#define DEBUG_LOG(format,...) do{if( __bas_util_log_level >= LOG_LEVEL_DEBUG ) im_log("DBG "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)
#define LOG(format, ...)      do{if( __bas_util_log_level >= LOG_LEVEL_DEBUG ) im_log("TRA "BAS_LOG_PREFIX_FRM": "format"", __FILE__,__LINE__,__FUNCTION__,  ##__VA_ARGS__);}while(0)


#ifdef __cplusplus
extern "C"
{
#endif

void im_log_close() ;
int im_log_init(const char* log_file_name) ;
void im_log_set_level(int level) ;
void im_log(char *fmt, ...) ;
void im_log_write_hex(unsigned char* data, int size) ;

#ifdef __cplusplus
}
#endif



#endif
