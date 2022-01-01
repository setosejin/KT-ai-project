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

#include <string.h>

#include "im_common.h"
#include "util/log.h"
#include "packet/packet.h"
#include "base/base.h"
#include "util/thread.h"

#if  defined (_IM_C_SOCK_) 
#include "netio/sock.h"
#endif

extern mutex_type g_sock_mutex;

static char g_buff_send[IOTMAKERS_MAX_SEND_MSG_LEN];

#if defined(_IM_C_MQTT_)
static int im_action_send_packet_via_mqtt(void *pMqttClient, char *data, int data_len) 
{
	int sent_len = im_mqtt_pub(pMqttClient, im_base_get_mqttTopic4Pub(), data, data_len);
	if ( sent_len !=  data_len) {
		ERROR_LOG("fail im_mqtt_pub()");
		return -1;
	}
	DEBUG_LOG("[%d]bytes sent", sent_len);

	return 0;
}
#elif defined(_IM_C_SOCK_)
static int im_action_send_packet_via_socket(int sock, char *data, int data_len)
{
//	int sent_len = im_sock_send(sock, data, data_len);
	int sent_len = im_sock_send(data, data_len);
	if ( sent_len !=  data_len) {
		ERROR_LOG("fail im_sock_send()");
		return -1;
	}
	DEBUG_LOG("[%d]bytes sent", sent_len);

	return 0;
}
#endif


int im_action_send_packet(IMPacketPtr ppkt) 
{
	int rc = 0;

	im_thread_lock_mutex(g_sock_mutex);

DEBUG_LOG("[[[");
	int len_serialized = im_packet_get_serialized_to_buff(ppkt, g_buff_send, IOTMAKERS_MAX_SEND_MSG_LEN);
	if ( len_serialized < 0 )	{
		ERROR_LOG("fail im_packet_get_serialized_to_buff()");
		im_thread_unlock_mutex(g_sock_mutex);
		return -1;
	}
	DEBUG_LOG("len_serialized=[%d]", len_serialized);
im_log_write_hex((unsigned char*)g_buff_send, len_serialized);	

#if defined(_IM_C_MQTT_)
	rc = im_action_send_packet_via_mqtt((void *)im_base_get_mqttc(), &g_buff_send[0], len_serialized);
#elif defined(_IM_C_SOCK_)
	rc = im_action_send_packet_via_socket(im_base_get_sock(), &g_buff_send[0], len_serialized);
#endif
DEBUG_LOG("]]]");

	if ( rc < 0 )	{
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_SEND_FAIL);
		im_thread_unlock_mutex(g_sock_mutex);
		return -1;
	}

	im_base_inc_pktCountSent();

	im_thread_unlock_mutex(g_sock_mutex);
	return rc;
}
