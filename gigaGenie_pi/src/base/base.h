/*
 * GiGA IoTMakers version 1.0
 *
 *  Copyright ¨Ï 2016 kt corp. All rights reserved.
 *
 *  This is a proprietary software of kt corp, and you may not use this file except in
 *  compliance with license agreement with kt corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of kt corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 */

#ifndef IOTMAKERS_BASE_H
#define IOTMAKERS_BASE_H

#include "im_common.h"
#include "packet/packet.h"

#if defined(_IM_C_MQTT_)
#include "org.eclipse.paho.mqtt.c/src/MQTTClient.h"
#endif

/*
 * IoTMakers Client Instance
 *
 */
typedef struct IMBase_t {
	char			ip[IOTMAKERS_STR_64_BYTE_LEN + 1];
	unsigned short	port;

	char			extrSysId[IOTMAKERS_STR_30_BYTE_LEN + 1];
	char			deviceId[IOTMAKERS_STR_64_BYTE_LEN + 1];
	char			athnRqtNo[IOTMAKERS_STR_64_BYTE_LEN + 1];
	char			log_file_name[IOTMAKERS_STR_128_BYTE_LEN + 1];

	char			commChAthnNo[IOTMAKERS_STR_64_BYTE_LEN + 1];
	int				isChAthnSuccess;

#if defined(_IM_C_MQTT_)
	MQTTClient		mqttClient;
	char			mqttTopic4Pub[IOTMAKERS_STR_64_BYTE_LEN + 1];
	char			mqttTopic4Sub[IOTMAKERS_STR_64_BYTE_LEN + 1];
#elif defined(_IM_C_SOCK_)
	int				sock;
	int				sock_timeout_sec;
	int				keepAliveExpireTime;
#endif

	unsigned long	pktCountRecv;
	unsigned long	pktCountSent;

	int				isServiceMode;
	int				isRecvLoopStop;
	int				lastErrorCode;


	IMCbTagidNumDataHndl	cb_numdata_hndl;
	IMCbTagidStrDataHndl	cb_strdata_hndl;
	IMCbDataRespHndl		cb_dataresp_hndl;
	IMCbErrorHndl			cb_error_hndl;
	IMCbIF711Hndl			cb_if711_hndl;
	IMCbIF333Hndl			cb_if333_hndl;

} IMBase, *IMBasePtr;

/*
 * ********************************************************************************
 * base.h
 * ********************************************************************************
 */

#ifdef __cplusplus
extern "C"
{
#endif

int im_base_init(char *ip, int port, char *extrSysId, char *deviceId, char *athnRqtNo, char *log_file_name);
void im_base_release() ;
char* im_base_get_ip() ;
int im_base_get_port() ;
char* im_base_get_extrSysId() ;
char* im_base_get_deviceId() ;
char* im_base_get_athnRqtNo() ;
char* im_base_get_log_file_name() ;
char* im_base_get_commChAthnNo() ;
int im_base_set_commChAthnNo(char *respAthnNo) ;
int im_base_is_ChAthnSuccess();

void* im_base_get_mqttc() ;
void im_base_set_mqttc() ;
char* im_base_get_mqttTopic4Pub();
void im_base_set_mqttTopic4Pub(char *topic);
char* im_base_get_mqttTopic4Sub();
void im_base_set_mqttTopic4Sub(char *topic);

int im_base_get_sock() ;
void im_base_set_sock(int sock) ;
int im_base_get_sockTimeoutSec() ;
int im_base_set_sockTimeoutSec(int sec);
int im_base_get_keepAliveExpireTimeSec() ;
int im_base_set_keepAliveExpireTimeSec(int sec) ;

unsigned long im_base_get_pktCountRecv() ;
void im_base_inc_pktCountRecv() ;
unsigned long im_base_get_pktCountSent() ;
void im_base_inc_pktCountSent() ;

void im_base_set_ServiceModeStart();
void im_base_set_ServiceModeStop() ;
int im_base_is_ServiceMode();

void im_base_set_recvLoopStart() ;
void im_base_set_recvLoopStop() ;
int im_base_get_isRecvLoopStop() ;

int im_base_get_lastErrorCode() ;
void im_base_set_lastErrorCode(IM_ErrCode lastErrorCode) ;

IMCbPktBodyHndl im_base_get_cb_keepalived_body_handler();
void im_base_set_cb_keepalived_body_handler(IMCbPktBodyHndl cb_fp) ;
IMCbPktBodyHndl im_base_get_cb_collection_body_handler();
void im_base_set_cb_collection_body_handler(IMCbPktBodyHndl cb_fp) ;
IMCbPktBodyHndl im_base_get_cb_control_body_handler();
void im_base_set_cb_control_body_handler(IMCbPktBodyHndl cb_fp) ;
IMCbTagidNumDataHndl im_base_get_cb_numdata_handler();
void im_base_set_cb_numdata_handler(IMCbTagidNumDataHndl cb_fp) ;
IMCbTagidStrDataHndl im_base_get_cb_strdata_handler();
void im_base_set_cb_strdata_handler(IMCbTagidStrDataHndl cb_fp) ;
IMCbDataRespHndl im_base_get_cb_dataresp_handler();
void im_base_set_cb_dataresp_handler(IMCbDataRespHndl cb_fp) ;
IMCbErrorHndl im_base_get_cb_error_hndler();
void im_base_set_cb_error_hndler(IMCbErrorHndl cb_fp) ;
IMCbIF711Hndl im_base_get_cb_if711_hndler();
void im_base_set_cb_if711_hndler(IMCbIF711Hndl cb_fp) ;


#ifdef __cplusplus
}
#endif


#endif
