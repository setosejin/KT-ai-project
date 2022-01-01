
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
#include "packet/body_util.h"
#include "packet/body_200.h"

#if 0
req:
{
    "extrSysId": "OPEN_TCP_001PTL001_100000XXXX",
    "devId": "XXXXr4D1450836201263",
    "athnRqtNo": "XXXX2p55o"
}

rsp:
{
    "athnRqtNo": "XXXX2p55o",
    "athnNo": "000000003B9ACFFB000000003B9CXXXX",
    "respCd": "100",
    "respMsg": "SUCCESS"
}
#endif



/*
 * ********************************************************************************
 * [1. MSG] DevCommChAhtnRqtVO를 생성하기 위한 함수
 * ********************************************************************************
 */

// ATHN_COMMCHATHN_DEV_TCP
int im_if224_build_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* athnRqtNo)
{
	body->root = json_value_init_object();

	jbody_value_set_string(body->root, "extrSysId", extrSysId);
	jbody_value_set_string(body->root, "devId", devId);
	jbody_value_set_string(body->root, "athnRqtNo", athnRqtNo);

	return (body->root != NULL)?0:-1;
}
