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

#ifndef IOTMAKERS_PKTBODY_400_H
#define IOTMAKERS_PKTBODY_400_H

#include "packet/body.h"


#define EXTR_IF_BODY_DATA_devColecDataVOs		"devColecDataVOs"
#define EXTR_IF_BODY_DATA_colecRowVOs			"colecRowVOs"




#ifdef __cplusplus
extern "C"
{
#endif

int im_if411_build_numdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, double snsnVal);
int im_if411_build_strdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal);

int im_if411_complexdata_init (IMPacketBodyPtr body, char* extrSysId, char* devId);
void im_if411_complexdata_release(IMPacketBodyPtr body);
int im_if411_complexdata_add_numdata(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal);
int im_if411_complexdata_add_strdata(IMPacketBodyPtr body, char* dataTypeCd, char* snsnVal);

#ifdef __cplusplus
}
#endif




#endif
