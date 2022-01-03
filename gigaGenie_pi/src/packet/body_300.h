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

#ifndef IOTMAKERS_PKTBODY_300_H
#define IOTMAKERS_PKTBODY_300_H

#include "packet/body.h"


#define EXTR_IF_BODY_300_devBasVOs			"devBasVOs"


#ifdef __cplusplus
extern "C"
{
#endif

int im_if332_data_init (IMPacketBodyPtr body, char* extrSysId, char* devId);
int im_if332_set_device_info (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo);
int im_if332_set_device_info_insert (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo);
int im_if332_set_device_info_delete (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo);
int im_if332_set_device_info_update (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo);
int im_if332_set_device_info_update_all (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo);
int im_if332_set_device_info_avp (IMPacketBodyPtr body, char* attr, char* val);

void im_if332_data_release(IMPacketBodyPtr body);

#ifdef __cplusplus
}
#endif




#endif
