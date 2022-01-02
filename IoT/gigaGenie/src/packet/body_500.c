
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
#include <stdio.h>

#include "packet/body_util.h"
#include "packet/body_500.h"

int im_if525_build_resp_body (IMPacketBodyPtr body, char* respCd, char* respMsg)
{
	body->root = json_value_init_object();

	jbody_value_set_string(body->root, "respCd", respCd);
	jbody_value_set_string(body->root, "respMsg", respMsg);

	return (body->root != NULL)?0:-1;
}

/************************** PARS CONTROL  ******************************/
#if 0

"devCnvyDataVOs[0].cnvyRowVOs[0].snsnDataInfoVOs[0].dataTypeCd" = "controll01"
"devCnvyDataVOs[0].cnvyRowVOs[0].snsnDataInfoVOs[0].snsnVal" = 6

control data - serialized_string:
{
    "mapHeaderExtension": {},
    "devCnvyDataVOs": [
        {
            "devId": "river4D1450836201263",
            "cnvyRowVOs": [
                {
                    "snsnDataInfoVOs": [
                        {
                            "dataTypeCd": "controll01",
                            "snsnVal": 6
                        }
                    ],
                    "sttusDataInfoVOs": [],
                    "contlDataInfoVOs": [],
                    "cmdDataInfoVOs": [],
                    "binDataInfoVOs": [],
                    "strDataInfoVOs": [],
                    "dtDataInfoVOs": [],
                    "genlSetupDataInfoVOs": [],
                    "sclgSetupDataInfoVOs": [],
                    "binSetupDataInfoVOs": [],
                    "mapRowExtension": {}
                }
            ]
        }
    ],
    "msgHeadVO": {
        "mapHeaderExtension": {}
    }
}


#endif

int im_if525_body_get_numdata_with_index (IMPacketBodyPtr body, int idx, char **o_tagid, double *o_val)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jbody_value_get_arrItem_with_index(body->root, "devCnvyDataVOs", 0);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jbody_value_get_arrItem_with_index(devCnvyDataVO, "cnvyRowVOs", 0);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jbody_value_get_arrItem_with_index(cnvyRowVO, "snsnDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}


	*o_tagid = jbody_value_to_string(jbody_value_get_value(strDataInfoVO, "dataTypeCd"));
	*o_val = json_number(jbody_value_get_value(strDataInfoVO, "snsnVal"));

	return 0;
}
int im_if525_body_get_strdata_with_index (IMPacketBodyPtr body, int idx, char **o_tagid, char **o_val)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* devCnvyDataVO = jbody_value_get_arrItem_with_index(body->root, "devCnvyDataVOs", 0);
	if ( devCnvyDataVO == NULL )	{
		return -1;
	}

	JSON_Value* cnvyRowVO = jbody_value_get_arrItem_with_index(devCnvyDataVO, "cnvyRowVOs", 0);
	if ( cnvyRowVO == NULL )	{
		return -1;
	}

	JSON_Value* strDataInfoVO = jbody_value_get_arrItem_with_index(cnvyRowVO, "strDataInfoVOs", idx);
	if ( strDataInfoVO == NULL )	{
		return -1;
	}


	*o_tagid = jbody_value_to_string(jbody_value_get_value(strDataInfoVO, "snsnTagCd"));
	*o_val = jbody_value_to_string(jbody_value_get_value(strDataInfoVO, "strVal"));

	return 0;
}




/************************** BUILD REPORT  ******************************/
#if 0
report data - serialized_string:
{
	"extrSysId": "EXAMPLE_LOWSYSTEM",
	"devCnvyDataVOs": [{
		"m2mSvcNo": 0,
		"devId": "D901CCTV01",
		"cnvyRowVOs": [{
			"occDt": "2014-02-2817:59:17.517",
			"snsnDataInfoVOs": [{
				"dataTypeCd": "10001003",
				"snsnVal": 0.7
			}],
			"strDataInfoVOs": [{
				"snsnTagCd": "60001003",
				"strVal": "EXAMPLE_STRING"
			}]
		}]
	}]
}

#endif



/* =====================================
if525r - simple
im_if525_build_report_numdata_body
im_if525_build_report_strdata_body
====================================== */
int im_if525_build_report_numdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, double snsnVal)
{
	int rc = 0;

	body->root = json_value_init_object();
	jbody_value_set_string(body->root, "extrSysId", extrSysId);

	JSON_Value* snsnDataInfoVO = jbody_snsnDataInfoVO_init(dataTypeCd, snsnVal); 
	
	JSON_Value* cnvyRowVO = jbody_cnvyRowVO_init(); 
	jbody_cnvyRowVO_append_snsnDataInfoVO(cnvyRowVO, snsnDataInfoVO);

	JSON_Value* devCnvyDataVO = jbody_devCnvyDataVO_init(devId); 
	jbody_devCnvyDataVO_append_colecRowVO(devCnvyDataVO, cnvyRowVO);

	jbody_value_append_arr_value(body->root, "devCnvyDataVOs", devCnvyDataVO);

	return rc;
}

int im_if525_build_report_strdata_body (IMPacketBodyPtr body, char* extrSysId, char* devId, char* dataTypeCd, char* snsnVal)
{
	int rc = 0;

	body->root = json_value_init_object();
	jbody_value_set_string(body->root, "extrSysId", extrSysId);

	JSON_Value* strDataInfoVO = jbody_strDataInfoVO_init(dataTypeCd, snsnVal); 

	JSON_Value* cnvyRowVO = jbody_cnvyRowVO_init(); 
	jbody_cnvyRowVO_append_snsnDataInfoVO(cnvyRowVO, strDataInfoVO);

	JSON_Value* devCnvyDataVO = jbody_devCnvyDataVO_init(devId); 
	jbody_devCnvyDataVO_append_colecRowVO(devCnvyDataVO, cnvyRowVO);

	jbody_value_append_arr_value(body->root, "devCnvyDataVOs", devCnvyDataVO);

	return rc;
}

