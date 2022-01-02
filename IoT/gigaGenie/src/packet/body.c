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

#include "packet/body.h"


/* =====================================
body init
====================================== */
int im_pktBody_init(IMPacketBodyPtr body) 
{
	body->json_status = JSONSuccess;
	body->serialized_string = NULL;
	body->root = NULL;
	return 0;
}
int im_pktBody_init_with_string(IMPacketBodyPtr body, char *json_str) 
{
	body->json_status = JSONSuccess;
	body->serialized_string = NULL;

	body->root = json_parse_string(json_str);
	if ( body->root == NULL )	{
		return -1;
	}
	return 0;
}
void im_pktBody_release(IMPacketBodyPtr body) 
{
	if ( body->serialized_string != NULL ){
		json_free_serialized_string(body->serialized_string);
		body->serialized_string = NULL;
	}

	if ( body->root != NULL ){
		json_value_free(body->root);
		body->root = NULL;
	}
}

/* =====================================
simple key-value-pair
====================================== */
char* im_pktBody_get_strval(IMPacketBodyPtr body, char *key) 
{
	return (char*)json_object_dotget_string(json_value_get_object(body->root), key);
}

char* im_pktBody_get_respCd(IMPacketBodyPtr body) 
{
	return im_pktBody_get_strval(body, EXTR_IF_BODY_COMMON_respCd);
}
int im_pktBody_get_respCd_as_int(IMPacketBodyPtr body) 
{
	char *respCd = im_pktBody_get_strval(body, EXTR_IF_BODY_COMMON_respCd);
	if ( respCd == NULL )	{
		return -1;
	}
	return atoi(respCd);
}
char* im_pktBody_get_respMsg(IMPacketBodyPtr body) 
{
	return im_pktBody_get_strval(body, EXTR_IF_BODY_COMMON_respMsg);
}


/* =====================================
serialize
====================================== */
int im_pktBody_get_serialized_strlen(IMPacketBodyPtr body) 
{
	//
	//!!! json_serialization_size() counts '\0' for string length.
	//
	return json_serialization_size(body->root) - 1;
}
char* im_pktBody_get_serialized_string(IMPacketBodyPtr body) 
{
	if ( body->serialized_string != NULL ){
		json_free_serialized_string(body->serialized_string);
	}
	body->serialized_string = json_serialize_to_string(body->root);
	return (char*)body->serialized_string;
}

int im_pktBody_get_serialized_to_buff(IMPacketBodyPtr body, char *o_buff, int o_buff_len)
{
	int len = im_pktBody_get_serialized_strlen(body);
	if ( len <= 0 || o_buff_len < len )	{
		return -1;
	}

	strcpy(o_buff, im_pktBody_get_serialized_string(body));
	return len;
}

int im_pktBody_get_deserialized_from_buff(IMPacketBodyPtr body, char *i_buff)
{
	// Just in case...
	im_pktBody_release(body);

	return im_pktBody_init_with_string(body, i_buff);
}

/* =====================================
debug
====================================== */
void im_pktBody_print_serialized_string(IMPacketBodyPtr body)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(body->root);
	printf("serialized_string:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
