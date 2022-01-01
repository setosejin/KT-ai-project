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
#include "packet/body_300.h"

#if 0

"devBasVOs[0].extrSysId" = ""

{
	"InfoUpdTypeCd": "13"
	"devBasVOs": [{
		"extrSysId": "",
		"devId": "",
		"upExtrSysId": "",
		"upDevId": "",

		"devNm": "",
		"ipAdr": "",
		"sttusCd": "",
		"makrCd": "",
		"modelCd": "",
		"frmwrVerNo": "",

		"useYn": "",
		"delYn": "",
		"lastMtnDt": ""
	}]
}

#endif

/*
obj: set/get
arr: append/get_idx
*/

static int im_if332_init_body(IMPacketBodyPtr body, char* extrSysId, char* devId)
{
	body->root = json_value_init_object();

	jbody_value_set_string(body->root, "infoUpdTypeCd", "13");

	JSON_Value *devBasVO = jbody_devBasVO_init(extrSysId, devId);

	jbody_value_append_arr_value(body->root, "devBasVOs", devBasVO);
	return 0;
}

static JSON_Value* jsonwrap_if332_get_devBasVOs_item(JSON_Value *json_val)
{
	JSON_Array  *devBasVOs = json_object_get_array(json_value_get_object(json_val), EXTR_IF_BODY_300_devBasVOs);
	JSON_Value *devBasVOs_0 = json_array_get_value(devBasVOs, 0);
	return devBasVOs_0;
}

/* =====================================
if332 - complex
====================================== */
int im_if332_data_init (IMPacketBodyPtr body, char* extrSysId, char* devId)
{
	int rc = 0;
	im_pktBody_init(body);
	im_if332_init_body(body, extrSysId, devId);
	return rc;
}


int im_if332_set_device_info (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo)
{
	JSON_Value *devBasVO = jbody_value_get_arrItem_with_index(body->root, "devBasVOs", 0);
	if ( devBasVO == NULL )	{
		return -1;
	}

	if ( devId != NULL ) jbody_value_set_string(devBasVO, "devId", devId);
	if ( devNm != NULL ) jbody_value_set_string(devBasVO, "devNm", devNm);
	if ( makrCd != NULL ) jbody_value_set_string(devBasVO, "makrCd", makrCd);
	if ( modelCd != NULL ) jbody_value_set_string(devBasVO, "modelCd", modelCd);
	if ( ipAdr != NULL ) jbody_value_set_string(devBasVO, "ipAdr", ipAdr);
	if ( sttusCd != NULL ) jbody_value_set_string(devBasVO, "sttusCd", sttusCd);
	if ( frmwrVerNo != NULL ) jbody_value_set_string(devBasVO, "frmwrVerNo", frmwrVerNo);

	return 0;
}

int im_if332_set_device_info_insert (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo)
{
	jbody_value_set_string(body->root, "infoUpdTypeCd", "01");
	im_if332_set_device_info(body, devId, devNm, makrCd, modelCd, ipAdr, sttusCd, frmwrVerNo);
	return 0;
}
int im_if332_set_device_info_delete (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo)
{
 	jbody_value_set_string(body->root, "infoUpdTypeCd", "21");
	im_if332_set_device_info(body, devId, devNm, makrCd, modelCd, ipAdr, sttusCd, frmwrVerNo);
	return 0;
}
int im_if332_set_device_info_update_all (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo)
{
 	jbody_value_set_string(body->root, "infoUpdTypeCd", "11");
	im_if332_set_device_info(body, devId, devNm, makrCd, modelCd, ipAdr, sttusCd, frmwrVerNo);
	return 0;
}
int im_if332_set_device_info_update (IMPacketBodyPtr body, char* devId, char* devNm, char* makrCd, char* modelCd, char* ipAdr, char* sttusCd, char* frmwrVerNo)
{
  	jbody_value_set_string(body->root, "infoUpdTypeCd", "13");
	im_if332_set_device_info(body, devId, devNm, makrCd, modelCd, ipAdr, sttusCd, frmwrVerNo);
	return 0;
}

int im_if332_set_device_info_avp (IMPacketBodyPtr body, char* attr, char* val)
{
	JSON_Value *devBasVO = jbody_value_get_arrItem_with_index(body->root, "devBasVOs", 0);
	if ( devBasVO == NULL )	{
		return -1;
	}

	if ( attr != NULL && val != NULL) jbody_value_set_string(devBasVO, attr, val);
	return 0;
}

void im_if332_data_release(IMPacketBodyPtr body)
{
	im_pktBody_release(body);
}


/*******************************************************************************/


#if 0

IF-333-req - serialized_string:
{
    "extrSysId": "HOME_IOT",
    "inclDevIds": ["W_085DDD531F7F0D"],
    "excluDevIds": [],
    "cmdDataInfoVOs": [{
        "dataTypeCd": "31000008",
        "cmdData": []
    }],
    "msgHeadVO": {
        "mapHeaderExtension": {}
    }
}

IF-333-rsp - serialized_string:
{
    "cmdDataInfoVOs": [{
        "dataTypeCd": "31000008",
        "cmdData": []
    }],
    "devBasVOs": [{
        "extrSysId": "HOME_IOT",
        "devId": "W_085DDD531F7F0D",
        "devTrtSttusCd": "00",
        "upDevId": "C_085DDD531F7F",
        "modelNm": "018c00420005",
        "frmwrVerNo": "39.18",
        "athnFormlCd": "0004",
        "athnRqtNo": "F21898613CE9955CE3F0DD9DA30FF30C",
        "athnNo": "00000000-3B9B-840B-0000-000000000001",
        "devDtlVOs": [{
            "atribNm": "GEN_DEV_CLASS",
            "atribVal": "10"
        }, {
            "atribNm": "SPCF_DEV_CLASS",
            "atribVal": "01"
        }, {
            "atribNm": "HW_VER",
            "atribVal": "11"
        }, {
            "atribNm": "IST_ICON_TY",
            "atribVal": "0700"
        }, {
            "atribNm": "USER_ICON_TY",
            "atribVal": "0700"
        }],
        "binSetupDataInfoVOs": [{
            "snsnTagCd": "82000001",
            "setupVal": [1]
        }]
    }]
} 
#endif



int im_if333_init_resp_body (IMPacketBodyPtr pbodyResp, IMPacketBodyPtr pbodyRqst)
{
	if ( pbodyRqst->root == NULL ){
		return -1;
	}

	pbodyResp->root = json_value_init_object();

	JSON_Value* tmpVal;

	tmpVal = jbody_value_get_value(pbodyRqst->root, "cmdDataInfoVOs");
	jbody_value_set_value(pbodyResp->root, "cmdDataInfoVOs", json_value_deep_copy(tmpVal));

	return (pbodyResp->root != NULL)?0:-1;
}

int im_if333_body_get_extrSysId (IMPacketBodyPtr body, char **o_extrSysId)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value* extrSysId = jbody_value_get_value(body->root, "extrSysId");
	if ( extrSysId == NULL )	{
		return -1;
	}

	*o_extrSysId = jbody_value_to_string(extrSysId);
	return 0;
}

int im_if333_body_get_inclDevId_with_index (IMPacketBodyPtr body, int idx, char **o_inclDevId)
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

int im_if333_body_get_cmdDataInfo_with_index (IMPacketBodyPtr body, int idx, char **o_cmdType)
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

static JSON_Value* _curr_devBasVO;
int im_if333_devBasVO_append_init (IMPacketBodyPtr body, char* extrSysId, char* devId)
{
	if ( body->root == NULL ){
		return -1;
	}

	JSON_Value *devBasVO = jbody_devBasVO_init(extrSysId, devId);

	jbody_value_append_arr_value(body->root, "devBasVOs", devBasVO);

	_curr_devBasVO = devBasVO;
	return 0;
}

int im_if333_devBasVO_set_attr(IMPacketBodyPtr body, char* attr, char* val)
{
	jbody_value_set_string(_curr_devBasVO, attr, val);
	return 0;
}

int im_if333_devBasVO_add_devDtlVO(IMPacketBodyPtr body, char* attr, char* val)
{
	JSON_Value *devDtlVO = jbody_devDtlVO_init(attr, val);
	jbody_value_append_arr_value(_curr_devBasVO, "devDtlVOs", devDtlVO);

	return 0;
}

int im_if333_devBasVO_add_binSetupDataInfoVO(IMPacketBodyPtr body, char* dataTypeCd, char* binVal, int binValLen)
{
	JSON_Value *binSetupDataInfoVO = jbody_binSetupDataInfoVO_init(dataTypeCd, binVal, binValLen);
	jbody_value_append_arr_value(_curr_devBasVO, "binSetupDataInfoVOs", binSetupDataInfoVO);

	return 0;
}

