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

#ifndef IOTMAKERS_NETIO_SOCK_H
#define IOTMAKERS_NETIO_SOCK_H


#ifdef __cplusplus
extern "C"
{
#endif

void im_mqtt_set_cb_recv_handler(void* cb);
int im_mqtt_connect(MQTTClient *pclient, char *remoteip, int port, char *clientId) ;
int im_mqtt_disconnect(MQTTClient *pclient);
int im_mqtt_subscribe_topic(MQTTClient *pclient, char *topic);
int im_mqtt_unsubscribe_topic(MQTTClient *pclient, char *topic);
int im_mqtt_pub(MQTTClient *pclient, char *topic,  unsigned char *data, int data_len);


#ifdef __cplusplus
}
#endif


#endif
