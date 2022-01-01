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

#include "base/base.h"
#include "util/log.h"
#include "packet/packet.h"
#include "action/action.h"
#include "packet/body_700.h"


static unsigned long long g_trxID;

/* =====================================
EXTR-IF-711
====================================== */
static void __query_request_body_handler(IMPacketBodyPtr pbodyResp, IMPacketBodyPtr pbodyRqst)
{
	int idx = 0;
	int rc;
	char *inclDevId;
	char *cmdType;

	IMCbIF711Hndl if711_handler= (IMCbIF711Hndl)im_base_get_cb_if711_hndler();

	rc = im_if711_body_get_cmdDataInfo_with_index(pbodyRqst, 0, &cmdType);

	if ( if711_handler != NULL )	
	{
		for (idx=0; idx<IOTMAKERS_MAX_CONTROL_DATA; idx++)	{
			rc = im_if711_body_get_inclDevId_with_index(pbodyRqst, idx, &inclDevId);
			if ( rc < 0 )	{
				break;
			}
			
			im_if711_devColecDataVO_append(pbodyResp, inclDevId);

			if711_handler(pbodyRqst, inclDevId, cmdType);
		}

	} else {
		INFO_LOG("No if711_handler set");
	}

/*
	im_pktBody_print_serialized_string(pbodyRqst);
	im_pktBody_print_serialized_string(pbodyResp);
*/
}


static int im_action_query_send_rsp(IMPacketPtr pktRqst)
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketHeadPtr phead = im_packet_get_head(ppkt);
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	IMPacketBodyPtr pbodyRqst = im_packet_get_body(pktRqst);

	im_packet_init_for_response(ppkt);
	im_packet_set_head_methodType_as_LAST_VAL_QUERY(ppkt);

	im_pktHead_copy_trmTransacId(&(pkt.head), &(pktRqst->head));

	im_if711_init_resp_body(pbody, pbodyRqst);	

	__query_request_body_handler(pbody, pbodyRqst);
	
	rc = im_action_send_packet(ppkt);

	im_packet_release(ppkt);
	return rc;
}

int im_action_query_last_req_handler(IMPacketPtr reqPkt)
{
	int rc = 0;
	IMPacketHeadPtr head = im_packet_get_head(reqPkt);
	IMPacketBodyPtr body = im_packet_get_body(reqPkt);

	g_trxID = im_pktHead_get_trmTransacId(head);

	rc = im_action_query_send_rsp(reqPkt);

	return rc;
}


