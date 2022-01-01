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
#include <stdlib.h>

#include "iotmakers_api.h"
#include "base/base.h"
#include "util/log.h"
#include "packet/packet.h"
#include "action/action.h"

#if  defined (_IM_C_SOCK_) 
#include "netio/sock.h"
#endif

static void im_action_recv_packet_handler(IMPacketPtr pkt)
{
	IMPacketHeadPtr head = im_packet_get_head(pkt);
	MthdType methodType = (MthdType)im_pktHead_get_methodType(head);
	switch (methodType)
	{
	case MthdType_ATHN_COMMCHATHN_DEV_TCP:		// 224
		im_action_authDevChannel_resp_handler(pkt);
		break;
#if  defined (_IM_C_SOCK_) 
	case MthdType_KEEP_ALIVE_COMMCHATHN_TCP:	// 231
		im_action_authKeepalive_resp_handler(pkt);
		break;
#endif
	case MthdType_INITA_DEV_UDATERPRT:			// 332
		im_action_if332_resp_handler(pkt);
		break;
	case MthdType_COLEC_ITGDATA_RECV:			// 411
		im_action_collectionData_resp_handler(pkt);
		break;
	case MthdType_CONTL_ITGCNVY_DATA:			// 525
		im_action_control_req_handler(pkt);
		break;
	case MthdType_LAST_VAL_QUERY:			// 711
		im_action_query_last_req_handler(pkt);
		break;
	case MthdType_INITA_DEV_INFO:			// 333
		im_action_if333_req_handler(pkt);
		break;
	
	default:
		ERROR_LOG("Unknown methodType[%d]", methodType);
		im_base_set_lastErrorCode(IM_ErrCode_PACKET_UNKNOWN);
		im_error_handler(IM_ErrCode_PACKET_UNKNOWN);
	}
}

static unsigned long __read_length()
{
	unsigned long pktLen;
	if ( im_sock_recv((char*)&pktLen, 4) < 0 )	{
		return -1;
	};
	pktLen = ntohl(pktLen);
	return pktLen;
}


int im_action_recv_data_handler(char *data, int datalen) 
{
	int rc;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;

	if ( datalen <= 0 )	{
		ERROR_LOG("no data to cook");
		return -1;
	}
DEBUG_LOG("{{{");
	DEBUG_LOG("[%d]bytes received", datalen);
im_log_write_hex((unsigned char*)data, datalen);	

	im_packet_init(ppkt);
	rc = im_packet_get_deserialized_from_buff(ppkt, data, datalen);
	if ( rc < 0 )	{
		im_packet_release(ppkt);
		ERROR_LOG("fail im_packet_get_deserialized_from_buff()");
		return -1;
	}

	im_base_inc_pktCountRecv();

	im_action_recv_packet_handler(ppkt);

	im_packet_release(ppkt);
DEBUG_LOG("}}}");

	return 0;
}


int im_action_recv_packet()
{
	int rc = 0;

#if defined(_IM_C_SOCK_)
	char buff[2048];
	int readLen;
	int packetLength = __read_length();
	DEBUG_LOG("packetLength=[%d].", packetLength);
	if ( packetLength < 35 || packetLength > 2048)	{
		ERROR_LOG("pkt_size:[%ld]", packetLength);
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_RECV_FAIL);
		im_sock_flush();
		return -1;
	}

	readLen = im_sock_recv(buff, packetLength);
	if ( readLen != packetLength )	{
		ERROR_LOG("pkt_size:[%ld]", packetLength);
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_RECV_FAIL);
		im_sock_flush();
		return -1;
	}

	im_action_recv_data_handler(buff, packetLength);
#endif

	return rc;
}

