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

#include <stdio.h>
#include <string.h>

#include "packet/body_util.h"
#include "packet/body_400.h"

#if 0

"extrSysId" = "extrSysId"

"devColecDataVOs[0].colecRowVOs[0].snsnDataInfoVOs[0].dataTypeCd" = "aaa"
"devColecDataVOs[0].colecRowVOs[0].snsnDataInfoVOs[0].snsnVal" = 100

"devColecDataVOs[0].colecRowVOs[0].snsnDataInfoVOs[1].dataTypeCd" = "bbb"
"devColecDataVOs[0].colecRowVOs[0].snsnDataInfoVOs[1].snsnVal" = 100

"devColecDataVOs[0].colecRowVOs[0].strDataInfoVOs[0].snsnTagCd" = "aaa"
"devColecDataVOs[0].colecRowVOs[0].strDataInfoVOs[0].strVal" = "xxx"


{
    "extrSysId": "extrSysId",
    "devColecDataVOs": [
        {
            "devId": "devId",
            "colecRowVOs": [
                {
					"occDt": "2014-02-28 17:59:17.517",
                    "snsnDataInfoVOs": [
                        {
                            "dataTypeCd": "aaa",
                            "snsnVal": 100
                        },
                        {
                            "dataTypeCd": "bbb",
                            "snsnVal": 100
                        },
                        {
                            "dataTypeCd": "ccc",
                            "snsnVal": 100
                        }
                    ],
                    "strDataInfoVOs": [
                        {
                            "snsnTagCd": "aaa",
                            "strVal": "xxx"
                        },
                        {
                            "snsnTagCd": "bbb",
                            "strVal": "yyy"
                        },
                        {
                            "snsnTagCd": "ccc",
                            "strVal": "zzz"
                        }
                    ]
                }
            ]
        }
    ]
}

#endif


/* =====================================
if411 - simple
====================================== */
int im_if411_build_numdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, double snsnVal)
{
	int rc = 0;

	body->root = json_value_init_object();
	jbody_value_set_string(body->root, "extrSysId", extrSysId);

	JSON_Value* snsnDataInfoVO = jbody_snsnDataInfoVO_init(dataTypeCd, snsnVal); 
	
	JSON_Value* colecRowVO = jbody_colecRowVO_init(); 
	jbody_colecRowVO_append_snsnDataInfoVO(colecRowVO, snsnDataInfoVO);

	JSON_Value* devColecDataVO = jbody_devColecDataVO_init(devId); 
	jbody_devColecDataVO_append_colecRowVO(devColecDataVO, colecRowVO);

	jbody_value_append_arr_value(body->root, "devColecDataVOs", devColecDataVO);

	return rc;
}

int im_if411_build_strdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal)
{
	int rc = 0;

	body->root = json_value_init_object();
	jbody_value_set_string(body->root, "extrSysId", extrSysId);

	JSON_Value* strDataInfoVO = jbody_strDataInfoVO_init(dataTypeCd, snsnVal); 
	
	JSON_Value* colecRowVO = jbody_colecRowVO_init(); 
	jbody_colecRowVO_append_strDataInfoVO(colecRowVO, strDataInfoVO);

	JSON_Value* devColecDataVO = jbody_devColecDataVO_init(devId); 
	jbody_devColecDataVO_append_colecRowVO(devColecDataVO, colecRowVO);

	jbody_value_append_arr_value(body->root, "devColecDataVOs", devColecDataVO);

	return rc;
}



/* =====================================
if411 - complex
====================================== */
int im_if411_complexdata_init (IMPacketBodyPtr body, char* extrSysId, char* devId)
{
	int rc = 0;
	im_pktBody_init(body);

	body->root = json_value_init_object();
	jbody_value_set_string(body->root, "extrSysId", extrSysId);

	JSON_Value* colecRowVO = jbody_colecRowVO_init(); 
	JSON_Value* devColecDataVO = jbody_devColecDataVO_init(devId); 

	jbody_devColecDataVO_append_colecRowVO(devColecDataVO, colecRowVO);

	jbody_value_append_arr_value(body->root, "devColecDataVOs", devColecDataVO);

	return rc;
}
void im_if411_complexdata_release(IMPacketBodyPtr body)
{
	im_pktBody_release(body);
}

int im_if411_complexdata_add_numdata(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal)
{
	int rc = 0;

	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devColecDataVO = jbody_value_get_arrItem_with_index(body->root, "devColecDataVOs", 0);
	if ( devColecDataVO == NULL )	{
		return -1;
	}
	JSON_Value* colecRowVO = jbody_value_get_arrItem_with_index(devColecDataVO, "colecRowVOs", 0);
	if ( colecRowVO == NULL )	{
		return -1;
	}

	JSON_Value* snsnDataInfoVO = jbody_snsnDataInfoVO_init(dataTypeCd, snsnVal); 
	jbody_colecRowVO_append_snsnDataInfoVO(colecRowVO, snsnDataInfoVO);

	return rc;
}

int im_if411_complexdata_add_strdata(IMPacketBodyPtr body, char* dataTypeCd, char* snsnVal)
{
	int rc = 0;

	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devColecDataVO = jbody_value_get_arrItem_with_index(body->root, "devColecDataVOs", 0);
	if ( devColecDataVO == NULL )	{
		return -1;
	}
	JSON_Value* colecRowVO = jbody_value_get_arrItem_with_index(devColecDataVO, "colecRowVOs", 0);
	if ( colecRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jbody_strDataInfoVO_init(dataTypeCd, snsnVal); 
	jbody_colecRowVO_append_strDataInfoVO(colecRowVO, strDataInfoVO);

	
	return rc;
}
