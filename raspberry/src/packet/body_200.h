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

#ifndef IOTMAKERS_PKTBODY_200_H
#define IOTMAKERS_PKTBODY_200_H

#include "packet/body.h"


#define EXTR_IF_BODY_ATHN_athnRqtNo		"athnRqtNo"
#define EXTR_IF_BODY_ATHN_athnNo		"athnNo"


#ifdef __cplusplus
extern "C"
{
#endif

int im_if224_build_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* athnRqtNo);


#ifdef __cplusplus
}
#endif


#endif
