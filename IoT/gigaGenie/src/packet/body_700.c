
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
#include "packet/body_700.h"


/*******************************/
#if 0

"cmdDataInfoVOs[0].dataTypeCd" = "Temp"

IF-711-req - serialized_string:
{
     "extrSysId": "HOME_IOT",
     "inclDevIds": ["W_085DDD27FB7507"],
     "excluDevIds": [],
     "cmdDataInfoVOs": [{
         "dataTypeCd": "2502",
         "cmdData": []
     }],
     "msgHeadVO": {
         "mapHeaderExtension": {}
     }
 } 

IF-711-rsp - serialized_string:
{
    "extrSysId": "HOME_IOT",
    "cmdDataInfoVOs": [{
        "dataTypeCd": "2502",
        "cmdData": []
    }],
    "devColecDataVOs": [{
        "devId": "W_085DDD27FB7507",
        "devTrtSttusCd": "01",
        "colecRowVOs": [{
            "occDt": "2016-06-0814:44:48.00",
            "binDataInfoVOs": [{
                "dataTypeCd": "2503",
                "binData": [0]
            }]
        }]
    }]
} 
#endif



int im_if711_init_resp_body (IMPacketBodyPtr pbodyResp, IMPacketBodyPtr pbodyRqst)
{
	if ( pbodyRqst->root == NULL ){
		return -1;
	}

	pbodyResp->root = json_value_init_object();

	JSON_Value* tmpVal;

	tmpVal = jbody_value_get_value(pbodyRqst->root, "extrSysId");
	jbody_value_set_value(pbodyResp->root, "extrSysId", json_value_deep_copy(tmpVal));

	tmpVal = jbody_value_get_value(pbodyRqst->root, "cmdDataInfoVOs");
	jbody_value_set_value(pbodyResp->root, "cmdDataInfoVOs", json_value_deep_copy(tmpVal));

	return (pbodyResp->root != NULL)?0:-1;
}

int im_if711_body_get_inclDevId_with_index (IMPacketBodyPtr body, int idx, char **o_inclDevId)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* inclDevId = jbody_value_get_arrItem_with_index(body->root, "inclDevIds", idx);
	if ( inclDevId == NULL )	{
		return -1;
	}

	*o_inclDevId = jbody_value_to_string(inclDevId);

	return 0;
}


int im_if711_body_get_cmdDataInfo_with_index (IMPacketBodyPtr body, int idx, char **o_cmdType)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* cmdDataInfoVO = jbody_value_get_arrItem_with_index(body->root, "cmdDataInfoVOs", idx);
	if ( cmdDataInfoVO == NULL )	{
		return -1;
	}

	JSON_Value* dataTypeCd = jbody_value_get_value(cmdDataInfoVO, "dataTypeCd");

	*o_cmdType = jbody_value_to_string(dataTypeCd);

	return 0;
}


static JSON_Value* _curr_colecRowVO;
int im_if711_devColecDataVO_append (IMPacketBodyPtr body, char* devId)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* colecRowVO = jbody_colecRowVO_init(); 
	JSON_Value* devColecDataVO = jbody_devColecDataVO_init(devId); 

	jbody_devColecDataVO_append_colecRowVO(devColecDataVO, colecRowVO);

	jbody_value_append_arr_value(body->root, "devColecDataVOs", devColecDataVO);

	_curr_colecRowVO = colecRowVO;
	return 0;
}

int im_if711_devColecDataVO_add_numdata(IMPacketBodyPtr body, char* dataTypeCd, double snsnVal)
{
	JSON_Value* snsnDataInfoVO = jbody_snsnDataInfoVO_init(dataTypeCd, snsnVal); 
	jbody_colecRowVO_append_snsnDataInfoVO(_curr_colecRowVO, snsnDataInfoVO);
	return 0;
}

int im_if711_devColecDataVO_add_strdata(IMPacketBodyPtr body, char* dataTypeCd, char* snsnVal)
{
	JSON_Value* strDataInfoVO = jbody_strDataInfoVO_init(dataTypeCd, snsnVal); 
	jbody_colecRowVO_append_strDataInfoVO(_curr_colecRowVO, strDataInfoVO);
	return 0;
}

int im_if711_devColecDataVO_add_bindata(IMPacketBodyPtr body, char* dataTypeCd, char* binVal, int binValLen)
{
	JSON_Value* binDataInfoVO = jbody_binDataInfoVO_init(dataTypeCd, binVal, binValLen); 
	jbody_colecRowVO_append_binDataInfoVO(_curr_colecRowVO, binDataInfoVO);
	return 0;
}
