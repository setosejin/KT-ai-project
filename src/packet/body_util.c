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

#include "packet/body.h"

#if 0
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
						}                    ],
                    "strDataInfoVOs": [
                        {
                            "snsnTagCd": "aaa",
                            "strVal": "xxx"
                        },
                        {
                            "snsnTagCd": "bbb",
                            "strVal": "yyy"
                        }
                    ]
                }
            ]
        }
    ]
}

#endif


/*
IF json_object_get_array is NULL THEN json_value_init_array
*/
JSON_Array* jbody_value_check_array(JSON_Value* val, char* arrname)
{
	JSON_Array* arr = json_object_get_array  (json_value_get_object(val), arrname);
	if ( arr == NULL )	{
		json_object_set_value(json_value_get_object(val), arrname, json_value_init_array());
	}
	arr = json_object_get_array  (json_value_get_object(val), arrname);
	if ( arr == NULL )	{
		return NULL;
	}
	return arr;
}

JSON_Value* jbody_value_set_string(JSON_Value* jval, char* name, char* val)
{
	json_object_set_string(json_value_get_object(jval), name, val);
	return jval;
}

JSON_Value* jbody_value_set_number(JSON_Value* jval, char* name, double val)
{
	json_object_set_number(json_value_get_object(jval), name, val);
	return jval;
}

JSON_Value* jbody_value_set_value(JSON_Value* jval, char* name, JSON_Value* val)
{
	json_object_set_value(json_value_get_object(jval), name, val);
	return jval;
}

JSON_Value* jbody_value_append_arr_value(JSON_Value* jval, char* arrname, JSON_Value* val)
{
	json_array_append_value(jbody_value_check_array(jval, arrname), val);
	return jval;
}


JSON_Value* jbody_value_get_value(JSON_Value* jval, char* name)
{
	return json_object_get_value(json_value_get_object(jval), name);
}

JSON_Value* jbody_value_get_arrItem_with_index(JSON_Value* jval, char* arrname, int idx)
{
	return  json_array_get_value(json_object_get_array(json_value_get_object(jval), arrname), idx);
}


char* jbody_value_to_string(JSON_Value* jVal)
{
	return (char*)json_string(jVal);
}
double jbody_value_to_number(JSON_Value* jVal)
{
	return json_number(jVal);
}


/**********************  "snsnDataInfoVOs"  *****************************
{
    "dataTypeCd": "aaa",
    "snsnVal": 100
}
*/
JSON_Value* jbody_snsnDataInfoVO_init(char *dataTypeCd, double snsnVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jbody_value_set_string(json_val, "dataTypeCd", dataTypeCd);
	jbody_value_set_number(json_val, "snsnVal", snsnVal);
	return json_val;
}

/**********************  "strDataInfoVOs"  *****************************
{
    "snsnTagCd": "aaa",
    "strVal": "100"
}
*/
JSON_Value* jbody_strDataInfoVO_init(char *dataTypeCd, char *strVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jbody_value_set_string(json_val, "snsnTagCd", dataTypeCd);
	jbody_value_set_string(json_val, "strVal", strVal);
	return json_val;
}

/**********************  "binDataInfoVOs"  *****************************
{
    "dataTypeCd": "aaa",
    "binData": [1,2,255, 0]
}
*/
JSON_Value* jbody_binDataInfoVO_init(char *dataTypeCd, char *binVal, int binValLen)
{
	int i;
	JSON_Value *json_val = json_value_init_object();
	JSON_Value *json_arr_val = json_value_init_array();
	
	jbody_value_set_string(json_val, "dataTypeCd", dataTypeCd);
	jbody_value_set_value(json_val, "binData", json_arr_val);

	JSON_Array  *json_arr  = json_value_get_array  (json_arr_val);
	for ( i=0 ; i<binValLen; i++)
	{
		unsigned char ch = binVal[i];
		json_array_append_number(json_arr, (double)ch);
	}
	
	return json_val;
}

/**********************  "binSetupDataInfoVOs"  *****************************
{
            "snsnTagCd": "82000001",
            "setupVal": [1,22,33,44]
}
*/
JSON_Value* jbody_binSetupDataInfoVO_init(char *snsnTagCd, char *setupVal, int binValLen)
{
	int i;
	JSON_Value *json_val = json_value_init_object();
	JSON_Value *json_arr_val = json_value_init_array();
	
	jbody_value_set_string(json_val, "snsnTagCd", snsnTagCd);
	jbody_value_set_value(json_val, "setupVal", json_arr_val);

	JSON_Array  *json_arr  = json_value_get_array  (json_arr_val);
	for ( i=0 ; i<binValLen; i++)
	{
		unsigned char ch = setupVal[i];
		json_array_append_number(json_arr, (double)ch);
	}
	
	return json_val;
}


/**********************  "colecRowVOs"  *****************************
 {
	"occDt": "2014-02-28 17:59:17.517",
     "snsnDataInfoVOs": [
         {}
     ],
     "strDataInfoVOs": [
         {}
     ],
     "binDataInfoVOs": [
         {}
     ]
 }
*/

JSON_Value* jbody_colecRowVO_init()
{
	JSON_Value *json_val = json_value_init_object();
	
	char timebuff[64];
	im_util_strftime_now(timebuff, "%Y-%m-%d %H:%M:%S.");
	sprintf(timebuff+strlen(timebuff), "%03d", im_util_gettime_now_msec());
	jbody_value_set_string(json_val, "occDt", timebuff);
	return json_val;
}

JSON_Value* jbody_colecRowVO_append_strDataInfoVO(JSON_Value* colecRowVO, JSON_Value* strDataInfoVO )
{
	jbody_value_append_arr_value(colecRowVO, "strDataInfoVOs", strDataInfoVO);
	return colecRowVO;
}

JSON_Value* jbody_colecRowVO_append_snsnDataInfoVO(JSON_Value* colecRowVO, JSON_Value* snsnDataInfoVO )
{
	jbody_value_append_arr_value(colecRowVO, "snsnDataInfoVOs", snsnDataInfoVO);
	return colecRowVO;
}

JSON_Value* jbody_colecRowVO_append_binDataInfoVO(JSON_Value* colecRowVO, JSON_Value* binDataInfoVO )
{
	jbody_value_append_arr_value(colecRowVO, "binDataInfoVOs", binDataInfoVO);
	return colecRowVO;
}

/**********************  "devColecDataVOs"  *****************************
{
    "devId": "devId",
    "colecRowVOs": [
        {}
    ]
}
*/
JSON_Value* jbody_devColecDataVO_init(char* devId)
{
	JSON_Value *json_val = json_value_init_object();
	
	jbody_value_set_string(json_val, "devId", devId);
	return json_val;
}

JSON_Value* jbody_devColecDataVO_append_colecRowVO(JSON_Value* devColecDataVO, JSON_Value* colecRowVO )
{
	jbody_value_append_arr_value(devColecDataVO, "colecRowVOs", colecRowVO);
	return devColecDataVO;
}

/**********************  "cnvyRowVOs"  *****************************
 {
	"occDt": "2014-02-28 17:59:17.517",
     "snsnDataInfoVOs": [
         {}
     ],
     "strDataInfoVOs": [
         {}
     ]
 }
*/

JSON_Value* jbody_cnvyRowVO_init()
{
	return jbody_colecRowVO_init();
}

JSON_Value* jbody_cnvyRowVO_append_strDataInfoVO(JSON_Value* cnvyRowVO, JSON_Value* strDataInfoVO )
{
	jbody_value_append_arr_value(cnvyRowVO, "strDataInfoVOs", strDataInfoVO);
	return cnvyRowVO;
}

JSON_Value* jbody_cnvyRowVO_append_snsnDataInfoVO(JSON_Value* cnvyRowVO, JSON_Value* snsnDataInfoVO )
{
	jbody_value_append_arr_value(cnvyRowVO, "snsnDataInfoVOs", snsnDataInfoVO);
	return cnvyRowVO;
}

/**********************  "devCnvyDataVOs"  *****************************
{
    "devId": "devId",
    "cnvyRowVOs": [
        {}
    ]
}
*/
JSON_Value* jbody_devCnvyDataVO_init(char* devId)
{
	return jbody_devColecDataVO_init(devId);
}

JSON_Value* jbody_devCnvyDataVO_append_colecRowVO(JSON_Value* devColecDataVO, JSON_Value* cnvyRowVO )
{
	jbody_value_append_arr_value(devColecDataVO, "cnvyRowVOs", cnvyRowVO);
	return devColecDataVO;
}



/**********************  "devBasVOs"  *****************************
{
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

{
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


*/

JSON_Value* jbody_devBasVO_init(char* extrSysId, char* devId)
{
	JSON_Value *json_val = json_value_init_object();
	
	jbody_value_set_string(json_val, "extrSysId", extrSysId);
	jbody_value_set_string(json_val, "devId", devId);
	return json_val;
}

JSON_Value* jbody_devDtlVO_init(char* atribNm, char* atribVal)
{
	JSON_Value *json_val = json_value_init_object();
	
	jbody_value_set_string(json_val, "atribNm", atribNm);
	jbody_value_set_string(json_val, "atribVal", atribVal);
	return json_val;
}

