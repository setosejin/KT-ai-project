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

#ifndef IOTMAKERS_PKTBODY_700_H
#define IOTMAKERS_PKTBODY_700_H

#include "packet/body.h"




#ifdef __cplusplus
extern "C"
{
#endif

int im_if711_init_resp_body (IMPacketBodyPtr pbodyResp, IMPacketBodyPtr pbodyRqst);
int im_if711_body_get_inclDevId_with_index (IMPacketBodyPtr body, int idx, char **o_inclDevId);
int im_if711_devColecDataVO_append (IMPacketBodyPtr body, char* devId);
int im_if711_devColecDataVO_add_numdata(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal);
int im_if711_devColecDataVO_add_strdata(IMPacketBodyPtr body, char* dataTypeCd, char* snsnVal);
int im_if711_devColecDataVO_add_bindata(IMPacketBodyPtr body, char* dataTypeCd, char* binVal, int binValLen);

#ifdef __cplusplus
}
#endif


#endif
