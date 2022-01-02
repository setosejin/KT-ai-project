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

#ifndef IOTMAKERS_PKTBODY_H
#define IOTMAKERS_PKTBODY_H

#include "parson/parson.h"

#define EXTR_IF_BODY_COMMON_respCd		"respCd"
#define EXTR_IF_BODY_COMMON_respMsg		"respMsg"

#define EXTR_IF_BODY_COMMON_extrSysId	"extrSysId"
#define EXTR_IF_BODY_COMMON_m2mSvcNo	"m2mSvcNo"

#define EXTR_IF_BODY_COMMON_occDt				"occDt"
#define EXTR_IF_BODY_COMMON_snsnDataInfoVOs		"snsnDataInfoVOs"
#define EXTR_IF_BODY_COMMON_strDataInfoVO		"strDataInfoVOs"
#define EXTR_IF_BODY_COMMON_dataTypeCd			"dataTypeCd"
#define EXTR_IF_BODY_COMMON_snsnVal				"snsnVal"
#define EXTR_IF_BODY_COMMON_snsnTagCd			"snsnTagCd"
#define EXTR_IF_BODY_COMMON_strVal				"strVal"



#if _IM_C_MQTT_
#define EXTR_IF_BODY_COMMON_deviceId	"devId"
#else
#define EXTR_IF_BODY_COMMON_deviceId	"deviceId"
#define EXTR_IF_BODY_COMMON_devId		"devId"
#endif

typedef struct {
	JSON_Status		json_status;
    char			*serialized_string;
    JSON_Value		*root;
} IMPacketBody, *IMPacketBodyPtr;


#ifdef __cplusplus
extern "C"
{
#endif

int im_pktBody_init(IMPacketBodyPtr body) ;
int im_pktBody_init_with_string(IMPacketBodyPtr body, char *json_str) ;
void im_pktBody_release(IMPacketBodyPtr body) ;
char* im_pktBody_get_strval(IMPacketBodyPtr body, char *key) ;
char* im_pktBody_get_respCd(IMPacketBodyPtr body) ;
int im_pktBody_get_respCd_as_int(IMPacketBodyPtr body) ;
char* im_pktBody_get_respMsg(IMPacketBodyPtr body) ;
int im_pktBody_get_serialized_strlen(IMPacketBodyPtr body) ;
char* im_pktBody_get_serialized_string(IMPacketBodyPtr body) ;
int im_pktBody_get_serialized_to_buff(IMPacketBodyPtr body, char *o_buff, int o_buff_len);
int im_pktBody_get_deserialized_from_buff(IMPacketBodyPtr body, char *i_buff);
void im_pktBody_print_serialized_string(IMPacketBodyPtr body);


#ifdef __cplusplus
}
#endif



#endif



