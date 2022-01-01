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

#ifndef IOTMAKERS_PKTBODY_500_H
#define IOTMAKERS_PKTBODY_500_H

#include "packet/body.h"

#define EXTR_IF_BODY_CTRL_devCnvyDataVOs		"devCnvyDataVOs"
#define EXTR_IF_BODY_CTRL_cnvyRowVOs			"cnvyRowVOs"



#ifdef __cplusplus
extern "C"
{
#endif

int im_if525_build_resp_body (IMPacketBodyPtr body, char* respCd, char* respMsg);

int im_if525_body_get_numdata_with_index (IMPacketBodyPtr body, int idx, char **o_tagid, double *o_val);
int im_if525_body_get_strdata_with_index (IMPacketBodyPtr body, int idx, char **o_tagid, char **o_val);


int im_if525_build_report_numdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, double snsnVal);
int im_if525_build_report_strdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal);


#ifdef __cplusplus
}
#endif


#endif
