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
#include "packet/body_400.h"
#include "action/action.h"


/* =====================================
EXTR-IF-411: COLEC_ITGDATA_RECV
====================================== */
static int __init_packet_for_request_411(IMPacketPtr ppkt, long trxid)
{
	IMPacketHeadPtr phead = im_packet_get_head(ppkt);
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	im_packet_init_for_request(ppkt);
	im_packet_set_head_methodType_as_COLEC_ITGDATA_RECV(ppkt);

	if ( trxid != 0 )	{
		im_packet_set_head_trmTransacId(ppkt, trxid);
	}

	return 0;
}
static void __release_packet(IMPacketPtr ppkt)
{
	im_packet_release(ppkt);
}





int im_action_collectionNumData_send_req_with_trxid(char* extrSysId, char* devId, char* dataTypeCd, double snsnVal, long trxid)
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	__init_packet_for_request_411(ppkt, trxid);

	im_if411_build_numdata_body(pbody, extrSysId, devId, dataTypeCd, snsnVal);	
	rc = im_action_send_packet(ppkt);
    if (rc < 0 ) {
		__release_packet(ppkt);
		ERROR_LOG("fail im_action_send_packet(), rc=[%d]", rc);
		return -1;
	}
	__release_packet(ppkt);
	

#if defined (_IM_C_SOCK_) 
	if ( !im_base_is_ServiceMode() )	{
		rc = im_action_recv_packet();
	}
#endif

	return rc;
}

int im_action_collectionStrData_send_req_with_trxid(char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal, long trxid)
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	__init_packet_for_request_411(ppkt, trxid);

	im_if411_build_strdata_body(pbody, extrSysId, devId, dataTypeCd, snsnVal);	
	rc = im_action_send_packet(ppkt);
    if (rc < 0 ) {
		__release_packet(ppkt);
		ERROR_LOG("fail im_action_send_packet(), rc=[%d]", rc);
		return -1;
	}
	__release_packet(ppkt);
	
#if defined (_IM_C_SOCK_) 
	if ( !im_base_is_ServiceMode() )	{
		rc = im_action_recv_packet();
	}
#endif

	return rc;
}

int im_action_collectionComplexData_send_req_with_trxid(IMPacketBodyPtr body, long trxid)
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	__init_packet_for_request_411(ppkt, trxid);

	im_packet_copy_body(ppkt, body);
	rc = im_action_send_packet(ppkt);
    if (rc < 0 ) {
		__release_packet(ppkt);
		ERROR_LOG("fail im_action_send_packet(), rc=[%d]", rc);
		return -1;
	}
	__release_packet(ppkt);
	
#if defined (_IM_C_SOCK_) 
	if ( !im_base_is_ServiceMode() )	{
		rc = im_action_recv_packet();
	}
#endif

	return rc;
}



int im_action_collectionData_resp_handler(IMPacketPtr respPkt)
{
	IMPacketHeadPtr head = im_packet_get_head(respPkt);
	IMPacketBodyPtr body = im_packet_get_body(respPkt);

	IMCbDataRespHndl cb_dataresp_handler = (IMCbDataRespHndl)im_base_get_cb_dataresp_handler();;

	if ( cb_dataresp_handler != NULL )	{
		long long trxid = im_pktHead_get_trmTransacId(head);
		char *respCode = (char*)im_pktBody_get_respCd(body);
		cb_dataresp_handler(trxid, respCode);
	} else {
		// no operation
		;
	}

	return 0;
}


