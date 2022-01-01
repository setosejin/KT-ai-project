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
#include <stdlib.h>
#include <string.h>

#include "im_common.h"
#include "base/base.h"

static IMBasePtr g_im_base = NULL;

static IMBasePtr g_get_imbase()
{
	if (g_im_base == NULL) {
		g_im_base = (IMBasePtr)malloc(sizeof(IMBase));
		if (!g_im_base) {
			printf("%s(): fail malloc()\n", __FUNCTION__);
			return NULL;
		}
		memset(g_im_base, 0x00, sizeof(IMBase));
	}
	return g_im_base;
}

int im_base_init(char *ip, int port, char *extrSysId, char *deviceId, char *athnRqtNo, char *log_file_name)
{
	IMBasePtr base = g_get_imbase();
	if (!base) {
		return -1;
	}
	strncpy(base->ip, ip, IOTMAKERS_STR_64_BYTE_LEN);
	base->port = port;
	strncpy(base->extrSysId, extrSysId, IOTMAKERS_STR_30_BYTE_LEN);
	strncpy(base->deviceId, deviceId, IOTMAKERS_STR_64_BYTE_LEN);
	strncpy(base->athnRqtNo, athnRqtNo, IOTMAKERS_STR_64_BYTE_LEN);
	strncpy(base->log_file_name, log_file_name, IOTMAKERS_STR_128_BYTE_LEN);

	base->commChAthnNo[0] = '\0';
	base->isChAthnSuccess = 0;

#ifdef _IM_C_MQTT_
	base->mqttTopic4Pub[0] = '\0';
	base->mqttTopic4Sub[0] = '\0';
#else
	base->sock = -1;
	base->keepAliveExpireTime = IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC;
	base->sock_timeout_sec = IOTMAKERS_SOCKET_TIMEOUT_SEC;
#endif

	base->pktCountRecv = 0;
	base->pktCountSent = 0;

	base->isServiceMode = 0;
	base->isRecvLoopStop = (1);
	base->lastErrorCode = 0;


	base->cb_error_hndl = NULL;

	base->cb_numdata_hndl = NULL;
	base->cb_strdata_hndl = NULL;
	base->cb_dataresp_hndl = NULL;

	return 0;
}

void im_base_release() 
{
	IMBasePtr base = g_get_imbase();

	if (g_im_base != NULL){
		free(g_im_base);
		g_im_base = NULL;
	}

	return;
}


char* im_base_get_ip() 
{ 
	return g_get_imbase()->ip; 
}
int im_base_get_port() 
{
	return g_get_imbase()->port; 
}


char* im_base_get_extrSysId() 
{
	return g_get_imbase()->extrSysId; 
}
char* im_base_get_deviceId() 
{ 
	return g_get_imbase()->deviceId; 
}
char* im_base_get_athnRqtNo() 
{ 
	return g_get_imbase()->athnRqtNo; 
}
char* im_base_get_log_file_name() 
{
	return g_get_imbase()->log_file_name; 
}

char* im_base_get_commChAthnNo() 
{ 
	return g_get_imbase()->commChAthnNo; 
}
int im_base_set_commChAthnNo(char *respAthnNo) 
{
#if 1
	strncpy(g_get_imbase()->commChAthnNo, respAthnNo, IOTMAKERS_STR_64_BYTE_LEN+1);
#else
	char tmpAthnNo[IOTMAKERS_STR_64_BYTE_LEN + 1] = "\0";
	char* athnNo = g_get_imbase()->commChAthnNo;
	int i = 0;

	char* tok = strtok(respAthnNo, "-");
	if (tok == NULL) {
		return -1;
	}
	strcpy(tmpAthnNo, tok);

	while (tok != NULL) {
		tok = strtok(NULL, "-");
		if (tok == NULL)
			break;

		strcat(tmpAthnNo, tok);
	}

	int tmpAthnNoLen = strlen(tmpAthnNo);

	char buf[3] = "\0";
	unsigned char ch = 0x00;

	for (i = 0; i < tmpAthnNoLen; i++) {
		if (i % 2 == 0) {
			memcpy(&buf[0], &tmpAthnNo[i], 2);
			ch = strtol(buf, NULL, 16);

			memcpy(&athnNo[i/2], &ch, 1);
		}
	}
#endif
	g_get_imbase()->isChAthnSuccess = (1);
	return 0;
}

int im_base_is_ChAthnSuccess()
{
	return g_get_imbase()->isChAthnSuccess; 
}




#ifdef _IM_C_MQTT_
void* im_base_get_mqttc() 
{
	return &(g_get_imbase()->mqttClient); 
}
void im_base_set_mqttc() 
{
}

char* im_base_get_mqttTopic4Pub()
{
	return g_get_imbase()->mqttTopic4Pub;
}
void im_base_set_mqttTopic4Pub(char *topic)
{
	sprintf(g_get_imbase()->mqttTopic4Pub, "in/%s", topic);
}

char* im_base_get_mqttTopic4Sub()
{
	return g_get_imbase()->mqttTopic4Sub;
}
void im_base_set_mqttTopic4Sub(char *topic)
{
	sprintf(g_get_imbase()->mqttTopic4Sub, "out/%s", topic);
}
#else
int im_base_get_sock() 
{
	return g_get_imbase()->sock; 
}
void im_base_set_sock(int sock) 
{
	g_get_imbase()->sock = sock; 
}

int im_base_get_sockTimeoutSec() 
{ 
	return g_get_imbase()->sock_timeout_sec; 
}
int im_base_set_sockTimeoutSec(int sec)
{
	g_get_imbase()->sock_timeout_sec = sec; 
}

int im_base_get_keepAliveExpireTimeSec() 
{
	return g_get_imbase()->keepAliveExpireTime; 
}
int im_base_set_keepAliveExpireTimeSec(int sec) 
{
	g_get_imbase()->keepAliveExpireTime = sec; 
}
#endif



unsigned long im_base_get_pktCountRecv() 
{ 
	return g_get_imbase()->pktCountRecv; 
}
void im_base_inc_pktCountRecv() 
{
	g_get_imbase()->pktCountRecv++;
}
unsigned long im_base_get_pktCountSent() 
{ 
	return g_get_imbase()->pktCountSent; 
}
void im_base_inc_pktCountSent() 
{
	g_get_imbase()->pktCountSent++;
}


void im_base_set_ServiceModeStart()
{
	g_get_imbase()->isServiceMode = (1); 
}
void im_base_set_ServiceModeStop() 
{
	g_get_imbase()->isServiceMode = (0); 
}
int im_base_is_ServiceMode()
{
	return g_get_imbase()->isServiceMode; 
}

void im_base_set_recvLoopStart() 
{
	g_get_imbase()->isRecvLoopStop = (0); 
}
void im_base_set_recvLoopStop() 
{
	g_get_imbase()->isRecvLoopStop = (1); 
}
int im_base_get_isRecvLoopStop() 
{
	return g_get_imbase()->isRecvLoopStop; 
}

int im_base_get_lastErrorCode() 
{ 
	return g_get_imbase()->lastErrorCode; 
}
void im_base_set_lastErrorCode(IM_ErrCode lastErrorCode) 
{
	g_get_imbase()->lastErrorCode = lastErrorCode;
}


// callback for user
IMCbTagidNumDataHndl im_base_get_cb_numdata_handler()
{
	return g_get_imbase()->cb_numdata_hndl; 
}
void im_base_set_cb_numdata_handler(IMCbTagidNumDataHndl cb_fp) 
{
	g_get_imbase()->cb_numdata_hndl = cb_fp; 
}

IMCbTagidStrDataHndl im_base_get_cb_strdata_handler()
{
	return g_get_imbase()->cb_strdata_hndl; 
}
void im_base_set_cb_strdata_handler(IMCbTagidStrDataHndl cb_fp) 
{
	g_get_imbase()->cb_strdata_hndl = cb_fp; 
}

IMCbDataRespHndl im_base_get_cb_dataresp_handler()
{
	return g_get_imbase()->cb_dataresp_hndl; 
}
void im_base_set_cb_dataresp_handler(IMCbDataRespHndl cb_fp) 
{
	g_get_imbase()->cb_dataresp_hndl = cb_fp; 
}

IMCbErrorHndl im_base_get_cb_error_hndler()
{
	return g_get_imbase()->cb_error_hndl; 
}
void im_base_set_cb_error_hndler(IMCbErrorHndl cb_fp) 
{
	g_get_imbase()->cb_error_hndl = cb_fp; 
}


IMCbIF711Hndl im_base_get_cb_if711_hndler()
{
	return g_get_imbase()->cb_if711_hndl; 
}
void im_base_set_cb_if711_hndler(IMCbIF711Hndl cb_fp) 
{
	g_get_imbase()->cb_if711_hndl = cb_fp; 
}

IMCbIF333Hndl im_base_get_cb_if333_hndler()
{
	return g_get_imbase()->cb_if333_hndl; 
}
void im_base_set_cb_if333_hndler(IMCbIF333Hndl cb_fp) 
{
	g_get_imbase()->cb_if333_hndl = cb_fp; 
}

