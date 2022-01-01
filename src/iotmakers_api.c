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
#include <sys/time.h>
#include <unistd.h>

#include "im_common.h"
#include "base/base.h"
#include "util/thread.h"
#include "util/util.h"
#include "util/log.h"
#include "packet/packet.h"
#include "packet/body_400.h"
#include "action/action.h"

#if  defined (_IM_C_SOCK_) 
#include "netio/sock.h"
#elif defined (_IM_C_MQTT_) 
#include "netio/mqtt.h"
#endif

static	long	g_keepalive_sec;
mutex_type		g_sock_mutex;

void im_error_handler(int errcode)
{
	IMCbErrorHndl cb_error_handler = im_base_get_cb_error_hndler();
	if ( cb_error_handler != NULL )	{
		cb_error_handler(errcode);
	}
}

/*
 * im_init
 */
int im_init(char *ip, int port, char *deviceId, char *athnRqtNo, char *extrSysId, char *log_file_name)
{
	int rc = 0;
	
	rc = im_base_init(ip, port, extrSysId, deviceId, athnRqtNo, log_file_name);
	if ( rc < 0 )	{
		return -1;
	}

	im_log_init((char*)im_base_get_log_file_name());
	im_log_set_level(LOG_LEVEL_ERROR);
	
	return 0;
}

int im_init_with_config_file(char *fname)
{
	char			ip[IOTMAKERS_STR_64_BYTE_LEN + 1];
	unsigned short	port;
	char			extrSysId[IOTMAKERS_STR_30_BYTE_LEN + 1];
	char			deviceId[IOTMAKERS_STR_64_BYTE_LEN + 1];
	char			athnRqtNo[IOTMAKERS_STR_64_BYTE_LEN + 1];
	char			log_file_name[IOTMAKERS_STR_128_BYTE_LEN + 1];

	int rc = im_util_read_config(fname,
			ip, &port,
			extrSysId,
			deviceId,
			athnRqtNo, log_file_name);

	printf("ip: %s\n", ip);
	printf("port: %d\n", port);
	printf("deviceId: %s\n", deviceId);
	printf("devicePasswd: %s\n", athnRqtNo);
	printf("gatewayId: %s\n", extrSysId);
	printf("log_file_name: %s\n", log_file_name);

	if (rc < 0)	{
		return -1;
	}

	return im_init(ip, port, deviceId, athnRqtNo, extrSysId, log_file_name);
}

void im_release() 
{
	im_base_release();
	im_log_close();
	return;
}

int im_get_LastErrCode()
{
	return im_base_get_lastErrorCode();
}

void im_set_loglevel(int loglevel)
{
	if ( loglevel <= LOG_LEVEL_ERROR )	{
		im_log_set_level(LOG_LEVEL_ERROR);
	} else if ( loglevel >= LOG_LEVEL_DEBUG )	{
		im_log_set_level(LOG_LEVEL_DEBUG);
	} else {
		im_log_set_level(loglevel);
	}
}

#if  defined (_IM_C_SOCK_) 
void im_set_socktimeout_sec(int sec)
{
	im_base_set_sockTimeoutSec(sec);
}
#endif


/*
 * im_settings callback - user data
 */
void im_set_numdata_handler(IMCbTagidNumDataHndl cb_proc)
{
	im_base_set_cb_numdata_handler(cb_proc);
}
void im_set_strdata_handler(IMCbTagidStrDataHndl cb_proc)
{
	im_base_set_cb_strdata_handler(cb_proc);
}
void im_set_dataresp_handler(IMCbDataRespHndl cb_proc)
{
	im_base_set_cb_dataresp_handler(cb_proc);
}
void im_set_error_handler(IMCbErrorHndl cb_proc)
{
	im_base_set_cb_error_hndler(cb_proc);
}
void im_set_if711_handler(IMCbIF711Hndl cb_proc)
{
	im_base_set_cb_if711_hndler(cb_proc);
}
void im_set_if333_handler(IMCbIF333Hndl cb_proc)
{
	im_base_set_cb_if333_hndler(cb_proc);
}


/*
 * im_connect
 */
void im_disconnect()
{
#if  defined (_IM_C_MQTT_) 
	im_mqtt_disconnect(im_base_get_mqttc());
#elif defined (_IM_C_SOCK_) 
	im_sock_disconnect(im_base_get_sock());
	im_base_set_sock(-1);
#endif
	if ( g_sock_mutex != NULL ){
		im_thread_destroy_mutex(g_sock_mutex);
		g_sock_mutex = NULL;
	}
}


static void __imcb_recv_data_handler(char *data, int data_len)
{
	im_action_recv_data_handler(data, data_len);
}


int im_connect()
{
	int conn_timeout_sec = (3);

#if  defined (_IM_C_MQTT_) 
	int rc;
	rc = im_mqtt_connect_timeout(im_base_get_mqttc(), im_base_get_ip(), im_base_get_port(), 
		im_base_get_deviceId(), conn_timeout_sec);
	if ( rc < 0 )	{
		ERROR_LOG("fail im_mqtt_connect()");
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_CONNECTION_FAIL);
		im_error_handler(IM_ErrCode_SOCK_CONNECTION_FAIL);
		return -1;
	}
	im_mqtt_set_cb_recv_handler(__imcb_recv_data_handler);

#elif defined (_IM_C_SOCK_) 
	int sock = -1;

	if ( im_base_get_sock() > 0 )	{
		im_disconnect();
	}

	sock = im_sock_connect_timeout(im_base_get_ip(), im_base_get_port(), conn_timeout_sec);
	if ( sock <= 0 )	{
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_CONNECTION_FAIL);
		im_error_handler(IM_ErrCode_SOCK_CONNECTION_FAIL);
		return -1;
	}
	im_base_set_sock(sock);
	im_sock_set_timeout(im_base_get_sockTimeoutSec());
	im_sock_set_cb_recv_handler((void*)__imcb_recv_data_handler);

#endif

	g_sock_mutex = im_thread_create_mutex();
	return 0;
}

int im_connected()
{
	return (im_sock_connected() >= 1)?1:0;
}

/*
 * im_auth_device
 */
int im_auth_device()
{
	im_base_set_lastErrorCode(IM_ErrCode_SUCCESS);
	
	int rc = im_action_authDevChannel_send_req();
	if ( rc < 0 )	{
		return -1;
	}

	int authWaitSec = 3;
	while ( im_base_is_ChAthnSuccess() != (1) )	{
		if ( authWaitSec-- <= 0 )	{
			ERROR_LOG("fail im_base_is_ChAthnSuccess()");
			return -1;
		}
		INFO_LOG("wating the device authenticated.");
		sleep(1);
	}
	INFO_LOG("device authenticated.");

	return 0;
}


/*
 * im_send_keepalive
 */
#if defined (_IM_C_SOCK_) 
int im_send_keepalive()
{
	int pktCountRecvPrev = im_base_get_pktCountRecv();
	int pktCountRecvNow = pktCountRecvPrev;

	int rc = im_action_authKeepalive_send_req();
	if ( rc < 0 )	{
		im_error_handler(IM_ErrCode_SOCK_SEND_FAIL);
		return -1;
	}

/*
	pktCountRecvNow = im_base_get_pktCountRecv();
	if ( pktCountRecvPrev == pktCountRecvNow )	{
		sleep(1);
		pktCountRecvNow = im_base_get_pktCountRecv();
	}

	if ( pktCountRecvPrev == pktCountRecvNow ) 	{
		sleep(1);
		pktCountRecvNow = im_base_get_pktCountRecv();
	}

	if ( pktCountRecvPrev == pktCountRecvNow )	{
		im_error_handler(IM_ErrCode_SOCK_SEND_FAIL);
	}
*/
	return 0;
}
#endif



int im_loop()
{
#if defined (_IM_C_SOCK_)
	if ( im_util_gettimeofday_as_sec() - g_keepalive_sec >  IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC)	{
		g_keepalive_sec = im_util_gettimeofday_as_sec(); 
		im_send_keepalive();
	}

	while ( im_sock_available() >= (1) )
	{
		if ( im_action_recv_packet() < 0 )		{
			return -1;
		};
	}
#endif
	DEBUG_LOG("sent=[%ld]pkts, recv=[%ld]pkts", im_base_get_pktCountSent(), im_base_get_pktCountRecv());
	return 0;
}



/*
 * loop 
 */
static thread_return_type WINAPI __th_recv_loop(void *arg)
{
	int rc = 0;

	im_base_set_recvLoopStart();
	im_base_set_ServiceModeStart();

	g_keepalive_sec = im_util_gettimeofday_as_sec(); 
	while ( im_base_get_isRecvLoopStop() != (1) )
	{
		if ( im_loop() < 0 )		{
			im_sock_disconnect();
			break;
		}
		// to avoid busy_wait
		sleep(1);
	}

	im_base_set_ServiceModeStop();
	INFO_LOG("done recv_loop.");
}

static int __im_start_recv_loop_thread()
{
	int rc;
    int thr_id  = (int)im_thread_start(__th_recv_loop, (void *)NULL);

	return 0;
}

static void __im_stop_recv_loop()
{
	im_base_set_recvLoopStop();
	im_base_set_ServiceModeStop();
}

int im_start_service()
{
	int rc;

	// 1. connect
	rc = im_connect();
	if ( rc < 0  )	{
        ERROR_LOG("fail im_connect()");
		return -1;
	}

#if  defined (_IM_C_MQTT_) 
	im_base_set_mqttTopic4Sub(im_base_get_deviceId());
	im_base_set_mqttTopic4Pub(im_base_get_deviceId());
	rc = im_mqtt_subscribe_topic(im_base_get_mqttc(), im_base_get_mqttTopic4Sub());
	if ( rc < 0)	{
		ERROR_LOG("fail im_mqtt_subscribe_topic(), topic=[%d]", im_base_get_mqttTopic4Sub());
		goto exit_on_error;
	}
#endif
 
	// 2. do looper
	im_base_set_ServiceModeStart();
	rc = __im_start_recv_loop_thread();
	if ( rc < 0 )	{
        ERROR_LOG("fail __im_start_recv_loop_thread()");
		goto exit_on_error;
	}

	// 3. do im_auth_device
	rc = im_auth_device();
	if ( rc < 0 )	{
        ERROR_LOG("fail im_auth_device()");
		goto exit_on_error;
	}

#if  defined (_IM_C_MQTT_) 
	im_mqtt_unsubscribe_topic(im_base_get_mqttc(), im_base_get_mqttTopic4Sub());
	im_base_set_mqttTopic4Sub(im_base_get_commChAthnNo());
	im_base_set_mqttTopic4Pub(im_base_get_commChAthnNo());
	rc = im_mqtt_subscribe_topic(im_base_get_mqttc(), im_base_get_mqttTopic4Sub());
	if ( rc < 0)	{
		ERROR_LOG("fail im_mqtt_subscribe_topic(), topic=[%d]", im_base_get_commChAthnNo());
		goto exit_on_error;
	}
#endif	
	return 0;

exit_on_error:
	__im_stop_recv_loop();	
	im_disconnect();
	return -1;
}

void im_stop_service()
{
	__im_stop_recv_loop();
	im_disconnect();
}



/*
 * im_send_data
 */
long long im_get_new_trxid()
{
	return (long long)im_util_get_unique_number();
}

static int im_send_numdata_with_trxid(char* extrSysId, char* devId, char *tagid, double val, long long trxid)
{
	im_base_set_lastErrorCode(IM_ErrCode_SUCCESS);

	int rc = im_action_collectionNumData_send_req_with_trxid(extrSysId, devId, tagid, val, trxid);
	if ( rc < 0 )	{
		im_error_handler(im_base_get_lastErrorCode());
		return -1;
	}

	return 0;
}

static int im_send_strdata_with_trxid(char* extrSysId, char* devId, char *tagid, char *val, long long trxid)
{
	im_base_set_lastErrorCode(IM_ErrCode_SUCCESS);

	int rc = im_action_collectionStrData_send_req_with_trxid(extrSysId, devId, tagid, val, trxid);
	if ( rc < 0 )	{
		im_error_handler(im_base_get_lastErrorCode());
		return -1;
	}

	return 0;
}


/*
 * im_send_complexdata
 */
static	IMPacketBody	g_if411_body;
int im_init_complexdata()
{
	im_pktBody_release(&g_if411_body);
	im_if411_complexdata_init(&g_if411_body, im_base_get_extrSysId(), im_base_get_deviceId());
	return 0;
}
int im_init_complexdata_with_devid(char *devId)
{
	im_pktBody_release(&g_if411_body);
	im_if411_complexdata_init(&g_if411_body, im_base_get_extrSysId(), devId);
	return 0;
}


int im_add_numdata_to_complexdata(char *tagid, double val)
{
	im_if411_complexdata_add_numdata(&g_if411_body, tagid, val);
	return 0;
}
int im_add_strdata_to_complexdata(char *tagid, char *val)
{
	im_if411_complexdata_add_strdata(&g_if411_body, tagid, val);
	return 0;
}
static int im_send_complexdata_with_trxid(long long trxid)
{
	im_base_set_lastErrorCode(IM_ErrCode_SUCCESS);

	int rc = im_action_collectionComplexData_send_req_with_trxid(&g_if411_body, trxid);
	im_if411_complexdata_release(&g_if411_body);

	if ( rc < 0 )	{
		im_error_handler(im_base_get_lastErrorCode());
		return -1;
	}
	return 0;
}
int im_send_complexdata(long long trxid)
{
	return im_send_complexdata_with_trxid(trxid);
}


int im_send_numdata(char *tagid, double val, long long trxid)
{
	return im_send_numdata_with_trxid(im_base_get_extrSysId(), im_base_get_deviceId(), tagid, val, trxid);
}
int im_send_strdata(char *tagid, char *val, long long trxid)
{
	return im_send_strdata_with_trxid(im_base_get_extrSysId(), im_base_get_deviceId(), tagid, val, trxid);
}

int im_send_numdata_with_devid(char *devId, char *tagid, double val, long long trxid)
{
	return im_send_numdata_with_trxid(im_base_get_extrSysId(), devId, tagid, val, trxid);
}
int im_send_strdata_with_devid(char *devId, char *tagid, char *val, long long trxid)
{
	return im_send_strdata_with_trxid(im_base_get_extrSysId(), devId, tagid, val, trxid);
}


int im_send_report_numdata(char *tagid, double val)
{
	return im_action_controlReportNumData_send_req_with_trxid(tagid, val, 0);
}
int im_send_report_strdata(char *tagid, char *val)
{
	return im_action_controlReportStrData_send_req_with_trxid(tagid, val, 0);
}

/*
 * IF-332
 */
static	IMPacketBody	g_if332_body;
int im_init_if332()
{
	im_pktBody_release(&g_if332_body);
	im_if332_data_init(&g_if332_body, im_base_get_extrSysId(), im_base_get_deviceId());
	return 0;
}
int im_set_if332_insert(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer)
{
	im_if332_set_device_info_insert(&g_if332_body, devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	return 0;
}
int im_set_if332_delete(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer)
{
	im_if332_set_device_info_delete(&g_if332_body, devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	return 0;
}
int im_set_if332_update_all(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer)
{
	im_if332_set_device_info_update_all(&g_if332_body, devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	return 0;
}
int im_set_if332_update(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer)
{
	im_if332_set_device_info_update(&g_if332_body, devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	return 0;
}
int im_set_if332_avp (char* attr, char* val){
	im_if332_set_device_info_avp(&g_if332_body, attr, val);
	return 0;
}
static int im_send_if332_with_trxid(long long trxid)
{
	im_base_set_lastErrorCode(IM_ErrCode_SUCCESS);
	int rc = im_action_if332_send_req_with_trxid(&g_if332_body, trxid);
	im_if332_data_release(&g_if332_body);
	if ( rc < 0 )	{
		im_error_handler(im_base_get_lastErrorCode());
		return -1;
	}
	return 0;
}
int im_send_if332(long long trxid)
{
	return im_send_if332_with_trxid(trxid);
}



/*
 * IF-711
 */
int im_add_numdata_for_if711(void* body, char *tagid, double val)
{
	return im_if711_devColecDataVO_add_numdata(body, tagid, (double)val);
}
int im_add_strdata_for_if711(void* body, char *tagid, char *val)
{
	return im_if711_devColecDataVO_add_strdata(body, tagid, val);
}
int im_add_bindata_for_if711(void* body, char *tagid, char *val, int valLen)
{
	return im_if711_devColecDataVO_add_bindata(body, tagid, val, valLen);
}

/*
 * IF-333
 */
int im_if333_set_attr(void* body, char *attr, char *val)
{
	return im_if333_devBasVO_set_attr(body, attr, val);
}
int im_if333_add_devDtlVO(void* body, char *attr, char *val)
{
	return im_if333_devBasVO_add_devDtlVO(body, attr, val);
}

int im_if333_add_binSetup(void* body, char *tagid, char *val, int valLen)
{
	return im_if333_devBasVO_add_binSetupDataInfoVO(body, tagid, val, valLen);
}

