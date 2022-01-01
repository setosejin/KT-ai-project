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

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif


#if defined(_IM_C_MQTT_)
#include "org.eclipse.paho.mqtt.c/src/MQTTClient.h"
#include "org.eclipse.paho.mqtt.c/src/Log.h"

#include "util/log.h"
#include "im_common.h"

typedef void (*IMCbRecvMsgHandler)(char *data, int data_len);

static IMCbRecvMsgHandler _cb_onMsgarrvd_hndl = NULL;;

void im_mqtt_set_cb_recv_handler(void* cb)
{
	_cb_onMsgarrvd_hndl = (IMCbRecvMsgHandler)cb;
}




static void im_mqtt_onConnlost(void *context, char *cause)
{
	INFO_LOG("Connection lost; cause=[%s]", cause);
}
static int im_mqtt_onMsgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	INFO_LOG("Message arrived, topic=[%s]", topicName);

	if ( _cb_onMsgarrvd_hndl != NULL )	{
		_cb_onMsgarrvd_hndl(message->payload, message->payloadlen);
	}

	MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}
static void im_mqtt_onDelivered(void *context, MQTTClient_deliveryToken dt)
{
	INFO_LOG("Message with token value [%d] delivery confirmed", dt);
}


int im_mqtt_connect_timeout(MQTTClient *pclient, char *remoteip, int port, char *clientId, int timeout_sec) 
{
	char  mqttAddr[160];
	int rc = 0;

#ifdef XXX_XWIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsa_data;

	WSAStartup(wVersionRequested, &wsa_data);
#endif


//// set MQTT_C_CLIENT_TRACE=ON
//	Log_initialize(NULL);
//	Log_setTraceLevel(LOG_ERROR);
////
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	if ( timeout_sec > 0 )	{
		conn_opts.connectTimeout = timeout_sec;
	}

	snprintf(mqttAddr, sizeof(mqttAddr), "tcp://%s:%d", remoteip, port);
    rc = MQTTClient_create(pclient, mqttAddr, clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    if ( rc < 0) {
		ERROR_LOG("fail MQTTClient_create(), rc=[%d]", rc);
		return -1;
	}

	conn_opts.keepAliveInterval = IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC;
    conn_opts.cleansession = 1;

    MQTTClient_setCallbacks(*pclient, NULL, im_mqtt_onConnlost, im_mqtt_onMsgarrvd, im_mqtt_onDelivered);

	DEBUG_LOG("MQTTClient_connect(); addr=[%s], cid=[%s]", mqttAddr, clientId);
	rc = MQTTClient_connect(*pclient, &conn_opts);
    if (rc < 0 ) {
		ERROR_LOG("fail MQTTClient_connect(), rc=[%d]", rc);
		return -1;
	}

	return 0;
}

int im_mqtt_connect(MQTTClient *pclient, char *remoteip, int port, char *clientId) 
{
	return im_mqtt_connect_timeout(pclient, remoteip, port, clientId, (5)); 
}

int im_mqtt_disconnect(MQTTClient *pclient)
{
	int timeout = 5000;
    MQTTClient_disconnect(*pclient, timeout);
    MQTTClient_destroy(pclient);
	return 0;
}

int im_mqtt_subscribe_topic(MQTTClient *pclient, char *topic)
{	
	int rc;
	static int QOS = 1;

	DEBUG_LOG("Subscribe topic=[%s]", topic);

	rc = MQTTClient_subscribe(*pclient, topic, QOS);
    if (rc < 0 ) {
		ERROR_LOG("fail MQTTClient_subscribe(), rc=[%d]", rc);
		return -1;
	}

	return 0;
}
int im_mqtt_unsubscribe_topic(MQTTClient *pclient, char *topic)
{	
	int rc;

	DEBUG_LOG("Unsubscribe topic=[%s]", topic);
	rc = MQTTClient_unsubscribe(*pclient, topic);
    if (rc < 0 ) {
		ERROR_LOG("fail MQTTClient_unsubscribe(), rc=[%d]", rc);
		return -1;
	}

	return 0;
}


int im_mqtt_pub(MQTTClient *pclient, char *topic,  unsigned char *data, int data_len)
{	
	int rc;
	static int timeout_msec = 2000L;
	static int QOS = 1;

    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

	pubmsg.payload = data;
	pubmsg.payloadlen = data_len;
	pubmsg.qos = QOS;
	pubmsg.retained = 0;

    DEBUG_LOG("Message with topic [%s]", topic);
	rc = MQTTClient_publishMessage(*pclient, topic, &pubmsg, &token);
	if ( rc < 0 ){
		ERROR_LOG("fail MQTTClient_publishMessage(), rc=[%d]", rc);
		return -1;
	}

	/*
#if 0
	DEBUG_LOG("Waiting for up to %d seconds for publication of %s on topic %s for client", 
		(int)(timeout_msec/1000), data, topic);

	rc = MQTTClient_waitForCompletion(*pclient, token, timeout_msec);
	if ( rc < 0 ){
		ERROR_LOG("fail MQTTClient_waitForCompletion(), rc=[%d], token=[%d]", rc, (int)token);
		return -1;
	}

	DEBUG_LOG("Message with delivery token [%d] delivered.", token);
#endif
	*/
	return data_len;
}

#endif
