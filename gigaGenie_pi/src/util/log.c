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

#if defined(ARDUINO_ARCH_AVR)
#include <Arduino.h>
#elif defined(_LINUX_) || defined(WIN32)
#include <fcntl.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <ctype.h>

#include "util/log.h"
#include "util/util.h"

int __bas_util_log_level;

#if defined(_LINUX_) || defined(WIN32)
static char g_fname_log[128];
#endif

static void _shurink_logfile(const char* log_file_name)
{
#if defined(_LINUX_) || defined(WIN32)
printf("log_file_name=[%s]\n", log_file_name);	
	int fd = open(log_file_name, O_CREAT|O_WRONLY|O_TRUNC, 0);
printf("fd=[%d]\n", fd);	

	close(fd);
#endif
}

void im_log_close() 
{
	return;
}

int im_log_init(const char* log_file_name)
{
	__bas_util_log_level = LOG_LEVEL_ERROR;

	if (log_file_name == NULL || strlen(log_file_name) <= 0) {
		g_fname_log[0] = '\0';
		return -1;
	}

	strcpy(g_fname_log, log_file_name);

	_shurink_logfile(g_fname_log);

	return 0;
}


void im_log_set_level(int level) 
{
	__bas_util_log_level = level;
	return;
}


void im_log(char *fmt, ...)
{
	va_list vlst;
	char    ebuf[IOTMAKERS_MAX_LOG_MSG_LEN];

#if defined(ARDUINO)
  va_start (vlst, fmt );
  vsnprintf(ebuf, 128, fmt, vlst);
  va_end (vlst);
  Serial.print(ebuf);
#elif defined(_LINUX_) || defined(WIN32)
	char    buf[256], logfile[256];
	char    *logPtr;
	int		fd, ret;

	logPtr = ebuf;
	im_util_strftime_now(buf, "%H:%M:%S");
	logPtr += snprintf(logPtr, 256, "%s ", buf);

	va_start(vlst, fmt);
	logPtr += vsnprintf(logPtr, sizeof(ebuf)-8, fmt, vlst);
	va_end(vlst);
	logPtr += sprintf(logPtr, "\n");

	if ( strlen(g_fname_log) <= 0){
		printf("%s", ebuf);
		return ;
	}


	sprintf(logfile, "%s", (char*)im_util_strftime_now(buf, g_fname_log));

#ifdef WIN32
	if( (fd=open(logfile, O_WRONLY|O_CREAT|O_APPEND, 0)) < 0){
		perror(logfile); 
		return;
	};
#else
	if( (fd=open(logfile, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0){
		perror(logfile); 
		return;
	};
#endif

	if( (ret=write(fd, ebuf, strlen(ebuf))) < 0){
		perror(logfile);
		close(fd);
		return;
	}
	close(fd);
#endif

	return;
}

void im_log_write_hex(unsigned char* data, int size)
{
    int i, rowsize, offset;
    char *bufpos, buffer[80];	/* XXX handle buffer overflow */
  
	if (size<=0){
		return;
	}

    for( offset = 0; size;  )
	{
		rowsize = ( size > 16 ) ? 16 : size;
		sprintf( buffer, "%04x: ", offset );
		bufpos = buffer + strlen( buffer );
	
		if ( rowsize >= 8 ) {
			for( i = 0; i < 8; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
			strcat( bufpos, "| " ); bufpos += 2;
			for( ; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}else{
			for( i = 0; i < rowsize; i++ ) {
				sprintf( bufpos, "%02x ", data[i] );
				bufpos += 3;
			};
		}

		for( ; i < 16; i++ ) {
		    strcat( buffer, "   " );
		};

		strcat( buffer, "  " );
		bufpos = buffer + strlen( buffer );
		for( i = 0; i < rowsize; i++, bufpos++ ) {
			sprintf( bufpos, "%c", ( isprint(data[i]) && (data[i]!='%') ) ? data[i] : '.' );
		};

		size -= rowsize;
		data += rowsize;
		offset += rowsize;
	
		*bufpos = '\0';
		DEBUG_LOG("%s", buffer);
	}
}
