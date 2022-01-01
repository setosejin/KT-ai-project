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

#include <string.h>

#if !defined(WIN32)
#include <arpa/inet.h>
#endif

#include "util/util.h"
#include "packet/packet.h"
#include "packet/head.h"

/* =====================================
packet - init
====================================== */
int im_packet_init(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	IMPacketBodyPtr body = &(pkt->body);

	pkt->len = 0;
	im_pktHead_init(head);
	im_pktBody_init(body);
	return 0;
}
int im_packet_release(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	IMPacketBodyPtr body = &(pkt->body);

	pkt->len = 0;
	im_pktHead_release(head);
	im_pktBody_release(body);
	return 0;
}

int im_packet_init_for_request(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_packet_init(pkt);

	im_pktHead_set_msgType(head, MsgType_REQUEST);
	im_pktHead_set_msgExchPtrn(head, MsgExchPtrn_ONE_WAY_ACK);
	return 0;
}

int im_packet_init_for_report(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_packet_init(pkt);

	im_pktHead_set_msgType(head, MsgType_REPORT);
	im_pktHead_set_msgExchPtrn(head, MsgExchPtrn_ONE_WAY);
	return 0;
}

int im_packet_init_for_response(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_packet_init(pkt);

	im_pktHead_set_msgType(head, MsgType_RESPONSE);
	return 0;
}

IMPacketHeadPtr im_packet_get_head(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	return head;
}

IMPacketBodyPtr im_packet_get_body(IMPacketPtr pkt) 
{
	IMPacketBodyPtr body = &(pkt->body);
	return body;
}

int im_packet_get_len(IMPacketPtr pkt) 
{
	return pkt->len;
}

int im_packet_copy_body(IMPacketPtr pkt, IMPacketBodyPtr i_body) 
{
	IMPacketBodyPtr body = &(pkt->body);
	im_pktBody_release(body);
	return im_pktBody_init_with_string(body, im_pktBody_get_serialized_string(i_body));
}



/* =====================================
packet - head_ext
====================================== */
int im_packet_get_headExt_count(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	return im_pktHeadExt_get_count(head);
}

int im_packet_put_headExt_val(IMPacketPtr pkt, char *key, char *val) 
{
	IMPacketHeadPtr head = &(pkt->head);
	return im_pktHeadExt_add_val(head, key, val);
}

char *im_packet_get_headExt_val(IMPacketPtr pkt, char *key) 
{
	IMPacketHeadPtr head = &(pkt->head);
	return im_pktHeadExt_get_val(head, key);
}


/* =====================================
packet - head
====================================== */
void im_packet_set_head_msgType_as_REQUEST(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_msgType(head, MsgType_REQUEST);
}
void im_packet_set_head_msgType_as_RESPONSE(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_msgType(head, MsgType_RESPONSE);
}


void im_packet_set_head_methodType_as_ATHN_COMMCHATHN_DEV_TCP(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_ATHN_COMMCHATHN_DEV_TCP);
}
void im_packet_set_head_methodType_as_KEEP_ALIVE_COMMCHATHN_TCP(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_KEEP_ALIVE_COMMCHATHN_TCP);
}
void im_packet_set_head_methodType_as_COLEC_ITGDATA_RECV(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_COLEC_ITGDATA_RECV);
}
void im_packet_set_head_methodType_as_CONTL_ITGCNVY_DATA(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_CONTL_ITGCNVY_DATA);
}
void im_packet_set_head_methodType_as_INITA_DEV_UDATERPRT(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_INITA_DEV_UDATERPRT);
}
// 2016-06-02
void im_packet_set_head_methodType_as_LAST_VAL_QUERY(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_LAST_VAL_QUERY);
}
//2016-06-14
void im_packet_set_head_methodType_as_INITA_DEV_INFO(IMPacketPtr pkt) 
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_methodType(head, MthdType_INITA_DEV_INFO);
}


void im_packet_set_head_trmTransacId(IMPacketPtr pkt, unsigned long long trxid)
{
	IMPacketHeadPtr head = &(pkt->head);
	im_pktHead_set_trmTransacId(head, trxid);
}

/* =====================================
packet - (de)serialize
====================================== */
int im_packet_get_serialized_to_buff(IMPacketPtr pkt, char *o_buff, unsigned int o_buff_len)
{
	IMPacketHeadPtr head = &(pkt->head);
	IMPacketBodyPtr body = &(pkt->body);

	int head_len = 0;
	int body_len = 0;

	head_len = im_pktHead_get_serialized_to_buff(head, 
		&o_buff[IOTMAKERS_TCP_HEAD_4_BYTE_LEN], o_buff_len-IOTMAKERS_TCP_HEAD_4_BYTE_LEN);
	if ( head_len < 0 )	{
		//ERROR_LOG("fail im_pktHead_get_serialized_to_buff()");
		return -1;
	}

	if ( body->root != NULL )	{
		body_len = im_pktBody_get_serialized_to_buff(body, 
			&o_buff[IOTMAKERS_TCP_HEAD_4_BYTE_LEN+head_len], o_buff_len-IOTMAKERS_TCP_HEAD_4_BYTE_LEN-head_len);
	}

	pkt->len = head_len + body_len;

	unsigned int packetLen = htonl(pkt->len) & IOTMAKERS_32_BIT_MASKING;
	memcpy(&o_buff[0], &packetLen, IOTMAKERS_TCP_HEAD_4_BYTE_LEN);

	return pkt->len + IOTMAKERS_TCP_HEAD_4_BYTE_LEN;
}

#if 0
int im_packet_get_deserialized_from_buff(IMPacketPtr pkt, char *i_buff, unsigned int i_buff_len)
{
	IMPacketHeadPtr head = &(pkt->head);
	IMPacketBodyPtr body = &(pkt->body);

	int head_len = 0;
	int body_len = 0;

	unsigned long pktLen = 0;

	memcpy((char*)&pktLen, &i_buff[0], IOTMAKERS_STR_4_BYTE_LEN);
	pkt->len = ntohl(pktLen);

	pkt->len = i_buff_len;
	if ( i_buff_len < pkt->len ) {
		return -1;
	}

	i_buff[IOTMAKERS_STR_4_BYTE_LEN + pkt->len] = '\0';

	head_len = im_pktHead_get_deserialized_from_buff(head, &i_buff[IOTMAKERS_STR_4_BYTE_LEN]);
	if ( head_len < 0 )	{
		return -1;
	}

	body_len = im_pktBody_get_deserialized_from_buff(body, &i_buff[IOTMAKERS_STR_4_BYTE_LEN + head_len]);
	if ( body_len < 0 )	{
		return -1;
	}

	return (head_len + body_len);
}
#else
int im_packet_get_deserialized_from_buff(IMPacketPtr pkt, char *i_buff, unsigned int i_buff_len)
{
	IMPacketHeadPtr head = &(pkt->head);
	IMPacketBodyPtr body = &(pkt->body);

	int head_len = 0;
	int body_len = 0;

	unsigned long pktLen = 0;

	pkt->len = i_buff_len;

	i_buff[pkt->len] = '\0';

	head_len = im_pktHead_get_deserialized_from_buff(head, &i_buff[0]);
	if ( head_len < 0 )	{
		return -1;
	}

	body_len = im_pktBody_get_deserialized_from_buff(body, &i_buff[head_len]);
	if ( body_len < 0 )	{
		return -1;
	}

	return (head_len + body_len);
}

#endif