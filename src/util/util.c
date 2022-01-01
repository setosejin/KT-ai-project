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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>


#if defined(_LINUX_)
#include <sys/time.h>
#include <time.h>
#elif defined(WIN32)
#include <windows.h>
#include <sys/timeb.h>

int gettimeofday (struct timeval *tv, void* tz)
{
    struct _timeb timebuffer;
    
    _ftime(&timebuffer);
    tv->tv_sec = timebuffer.time;
    tv->tv_usec = timebuffer.millitm * 1000;
	return (0);
} 
#endif


#include "im_common.h"

#if defined(ARDUINO)

#endif

#if defined(_LINUX_) || defined(WIN32)
static struct timeval g_time_of_day;
#endif


long im_util_gettimeofday_as_sec()
{   
#if defined(_LINUX_) || defined(WIN32)
	gettimeofday(&g_time_of_day, NULL);
	return (time_t)g_time_of_day.tv_sec;
#endif
	return 0;
}

long im_util_gettimeofday_as_usec()
{   
#if defined(_LINUX_) || defined(WIN32)
	gettimeofday(&g_time_of_day, NULL);
	return (time_t)g_time_of_day.tv_usec;
#endif
	return 0;
}


int im_util_strftime_on_sec(long sec, char *strdate, char *fmt)
{
#if defined(_LINUX_) || defined(WIN32)
	struct tm *tm;
	tm = localtime((time_t*)&sec);
	return (strftime(strdate, strlen(fmt)+32, fmt, tm));
#endif
	return 0;
}

char *im_util_strftime_now(char *buff, char *fmt)
{
	im_util_strftime_on_sec(im_util_gettimeofday_as_sec(), buff, fmt);
	return buff;
}
int im_util_gettime_now_msec()
{
	long usec = im_util_gettimeofday_as_usec();
	return (int)usec/1000;
}


unsigned long im_util_get_secs()
{
#if defined(ARDUINO)
	return (unsigned long)(millis()/1000);
#else
	return im_util_gettimeofday_as_sec();
#endif
}

long long im_util_get_unique_number() 
{
	static unsigned int counter = 0;
#if defined(_LINUX_) || defined(WIN32)
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return  (long long)(tv.tv_sec+counter++);
#endif
	return (long long)counter++;
}


int im_util_str_trim(char *szStr, int nLen)
{
	char *ch, *pcStart;
	int	 i, nChLen = 0;
	
	if ( szStr[0] == '\0'){
		return 0;
	};

	ch = szStr;

	for (i = 0; i < nLen; i++){
		if ( !isspace(*ch) ){
			pcStart = ch;
			break;
		};
		ch++;
	};

	nChLen = 0;
	for (; i < nLen; i++){
		if ( isspace(*ch) ){
			break;
		};
		ch++;
		nChLen++;
	};

	memmove(szStr, pcStart, nChLen);
	szStr[nChLen] = '\0';

	return nChLen;
}


int im_util_read_config(const char* config, char* ip, unsigned short* port, 
	char* extrSysId, char* deviceId, char* athnRqtNo, char* log_file_name)
{
#if defined(_LINUX_) || defined(WIN32)
	char line_buff[1024];
	char* tmp = NULL;

	int chk_ip = 0;
	int chk_port = 0;
	int chk_log_file_name = 0;
	int chk_extr_sys_id = 0;
	int chk_dev_id = 0;
	int chk_dev_pw = 0;

	ip[0] = '\0';
	*port = 0;
	extrSysId[0] = '\0';
	deviceId[0] = '\0';
	athnRqtNo[0] = '\0';
	log_file_name[0] = '\0';

	FILE* fp = fopen(config, "r");
	if (fp == NULL) {
		printf("config(%s) file not found\n", config);
		return -1;
	}

	while (	fgets(line_buff, sizeof(line_buff), fp)) {
		if (line_buff[0] == '#')
			continue;

		im_util_str_trim(line_buff, strlen(line_buff));

		if(strncmp(line_buff, "SERVER_IP", strlen("SERVER_IP")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL, "=");
			if (tmp) {
				snprintf(ip, IOTMAKERS_STR_64_BYTE_LEN + 1, "%s", tmp);
				chk_ip++;
			}
			continue;
		}

		if(strncmp(line_buff, "SERVER_PORT", strlen("SERVER_PORT")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL,"=");
			if (tmp) {
				*port = atoi(tmp);
				chk_port++;
			}
			continue;
		}

		if(strncmp(line_buff, "LOG_FILE_NAME", strlen("LOG_FILE_NAME")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL, "=");
			if (tmp) {
				snprintf(log_file_name, IOTMAKERS_STR_128_BYTE_LEN + 1, "%s", tmp);
				chk_log_file_name++;
			}
			continue;
		}

		if(strncmp(line_buff, "GATEWAY_ID", strlen("GATEWAY_ID")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL, "=");
			if (tmp) {
				snprintf(extrSysId, IOTMAKERS_STR_30_BYTE_LEN + 1, "%s", tmp);
				chk_extr_sys_id++;
			}
			continue;
		}

		if(strncmp(line_buff, "DEV_ID", strlen("DEV_ID")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL, "=");
			if (tmp) {
				snprintf(deviceId, IOTMAKERS_STR_64_BYTE_LEN + 1, "%s", tmp);
				chk_dev_id++;
			}
			continue;
		}

		if(strncmp(line_buff, "DEV_PW", strlen("DEV_PW")) == 0) {
			strtok(line_buff, "=");
			tmp = strtok(NULL, "=");
			if (tmp) {
				snprintf(athnRqtNo, IOTMAKERS_STR_64_BYTE_LEN + 1, "%s", tmp);
				chk_dev_pw++;
			}
			continue;
		}
	}

	fclose(fp);

	if (chk_ip != 1 || chk_port != 1 
			|| chk_extr_sys_id != 1 || chk_dev_id != 1 || chk_dev_pw != 1) {
		printf("config(%s) file seems incorrect\n", config);
		printf("example: \n");
		printf("SERVER_IP=192.168.0.1\n");
		printf("SERVER_PORT=9075\n");
		printf("LOG_FILE_NAME=libim_log.txt\n");
		printf("GATEWAY_ID=IOT_CCEI_001PTL002_8000000075\n");
		printf("DEV_ID=csdkTestDevId1\n");
		printf("DEV_PW=new123456\n");
		return -1;
	}
#endif

	return 0;
}


#if 0
int is_avail_send_without_athn(MthdType mthdType)
{
	int rtn = 0;

	switch (mthdType) {
	// 인증을하지 않아도 전송할 수 있는 메시지들을 정의한다.
	case MthdType_VER_GWVER_RQT:
	case MthdType_ATHN_LOGINATHN_DEV_TCP:
	case MthdType_ATHN_COMMCHATHN_DEV_TCP:
		rtn = 1;
		break;
	}
	return rtn;
}
#endif
