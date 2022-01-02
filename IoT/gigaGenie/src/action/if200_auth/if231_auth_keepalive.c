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
#include "packet/body_200.h"
#include "action/action.h"

/* =====================================
EXTR-IF-231
====================================== */
int im_action_authKeepalive_send_req()
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketHeadPtr phead = im_packet_get_head(ppkt);
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	im_packet_init_for_request(ppkt);
	im_packet_set_head_methodType_as_KEEP_ALIVE_COMMCHATHN_TCP(ppkt);

	rc = im_action_send_packet(ppkt);
    if (rc < 0 ) {
		im_packet_release(ppkt);
		ERROR_LOG("fail im_action_send_packet(), rc=[%d]", rc);
		return -1;
	}
	im_packet_release(ppkt);
	

#if defined (_IM_C_SOCK_) 
	if ( !im_base_is_ServiceMode() )	{
		rc = im_action_recv_packet();
	}
#endif

	return rc;
}


int im_action_authKeepalive_resp_handler(IMPacketPtr respPkt)
{
	IMPacketBodyPtr body = im_packet_get_body(respPkt);
	// no operation
	return 0;
}

