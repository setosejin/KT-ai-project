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

#ifndef IOTMAKERS_ACTION_H
#define IOTMAKERS_ACTION_H

#ifdef __cplusplus
extern "C"
{
#endif

int im_action_recv_data_handler(char *data, int datalen) ;
int im_action_recv_packet() ;
int im_action_send_packet(IMPacketPtr ppkt) ;

int im_action_authDevChannel_send_req();
int im_action_authDevChannel_resp_handler(IMPacketPtr respPkt);
int im_action_authKeepalive_send_req();
int im_action_authKeepalive_resp_handler(IMPacketPtr respPkt);

int im_action_collectionNumData_send_req_with_trxid(char* extrSysId, char* devId, char* dataTypeCd, double snsnVal, long trxid);
int im_action_collectionStrData_send_req_with_trxid(char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal, long trxid);
int im_action_collectionComplexData_send_req_with_trxid(IMPacketBodyPtr body, long trxid);
int im_action_collectionData_resp_handler(IMPacketPtr respPkt);

int im_action_control_send_rsp(IMPacketPtr pktRqst, char *respCd, char *respMsg);
int im_action_control_req_handler(IMPacketPtr reqPkt);

#ifdef __cplusplus
}
#endif

#endif


