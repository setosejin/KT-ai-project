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

#ifndef IOTMAKERS_PACKET_H
#define IOTMAKERS_PACKET_H

#include "packet/head.h"
#include "packet/body.h"

typedef struct {
	unsigned int	len;
	IMPacketHead		head;
	IMPacketBody		body;

} IMPacket, *IMPacketPtr;

typedef void (*IMCbPktHndl)(IMPacketPtr pkt);
typedef void (*IMCbPktBodyHndl)(IMPacketBodyPtr body);

#ifdef __cplusplus
extern "C"
{
#endif

int im_packet_init(IMPacketPtr pkt) ;
int im_packet_release(IMPacketPtr pkt) ;
int im_packet_init_for_request(IMPacketPtr pkt) ;
int im_packet_init_for_response(IMPacketPtr pkt) ;
int im_packet_init_for_report(IMPacketPtr pkt) ;

IMPacketHeadPtr im_packet_get_head(IMPacketPtr pkt) ;
IMPacketBodyPtr im_packet_get_body(IMPacketPtr pkt) ;
int im_packet_get_len(IMPacketPtr pkt) ;
int im_packet_copy_body(IMPacketPtr pkt, IMPacketBodyPtr i_body) ;

int im_packet_add_headExt_nvp(IMPacketPtr pkt, char *key, char *val) ;
void im_packet_set_head_msgType_as_REQUEST(IMPacketPtr pkt) ;
void im_packet_set_head_msgType_as_RESPONSE(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_ATHN_COMMCHATHN_DEV_TCP(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_KEEP_ALIVE_COMMCHATHN_TCP(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_COLEC_ITGDATA_RECV(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_CONTL_ITGCNVY_DATA(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_INITA_DEV_UDATERPRT(IMPacketPtr pkt) ;
void im_packet_set_head_methodType_as_LAST_VAL_QUERY(IMPacketPtr pkt) ;


void im_packet_set_head_trmTransacId(IMPacketPtr pkt, unsigned long long trxid);

int im_packet_get_serialized_to_buff(IMPacketPtr pkt, char *o_buff, unsigned int o_buff_len);
int im_packet_get_deserialized_from_buff(IMPacketPtr pkt, char *i_buff, unsigned int i_buff_len);

#ifdef __cplusplus
}
#endif

#endif


