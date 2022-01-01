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

#if defined (_IM_C_SOCK_) 
#include "iotmakers_api.h"
#endif

/* =====================================
EXTR-IF-224
====================================== */
int im_action_authDevChannel_send_req()
{
	int rc = 0;
	IMPacket pkt;
	IMPacketPtr ppkt = &pkt;
	IMPacketHeadPtr phead = im_packet_get_head(ppkt);
	IMPacketBodyPtr pbody = im_packet_get_body(ppkt);

	im_packet_init_for_request(ppkt);
	im_packet_set_head_methodType_as_ATHN_COMMCHATHN_DEV_TCP(ppkt);

	im_if224_build_body(pbody, im_base_get_extrSysId(), im_base_get_deviceId(), im_base_get_athnRqtNo());	

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


int im_action_authDevChannel_resp_handler(IMPacketPtr respPkt)
{
	IMPacketBodyPtr body = im_packet_get_body(respPkt);
	int rc;
	char *athnNo = NULL;

	athnNo = (char*)im_pktBody_get_strval(body, EXTR_IF_BODY_ATHN_athnNo);
	if ( athnNo == NULL ){
		ERROR_LOG("no name[%s] in the body", EXTR_IF_BODY_ATHN_athnNo);
		return -1;
	}

	rc = im_base_set_commChAthnNo(athnNo);
	if ( rc < 0 )	{
		ERROR_LOG("invalid athnNo=[%s] received", athnNo);
		return -1;
	}

	INFO_LOG("The device channel has been authenticated. [%s]", (char*)im_base_get_commChAthnNo());

	return 0;
}
