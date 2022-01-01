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

#ifndef IOTMAKERS_BODY_UTIL_H
#define IOTMAKERS_BODY_UTIL_H

#include "parson/parson.h"

#ifdef __cplusplus
extern "C"
{
#endif

JSON_Array* jbody_value_check_array(JSON_Value* val, char* arrname);
JSON_Value* jbody_value_set_string(JSON_Value* jval, char* name, char* val);
JSON_Value* jbody_value_set_number(JSON_Value* jval, char* name, double val);
JSON_Value* jbody_value_set_value(JSON_Value* jval, char* name, JSON_Value* val);
JSON_Value* jbody_value_append_arr_value(JSON_Value* jval, char* arrname, JSON_Value* val);
JSON_Value* jbody_value_get_value(JSON_Value* jval, char* name);
JSON_Value* jbody_value_get_arrItem_with_index(JSON_Value* jval, char* arrname, int idx);
char* jbody_value_to_string(JSON_Value* jVal);
double jbody_value_to_number(JSON_Value* jVal);

JSON_Value* jbody_snsnDataInfoVO_init(char *dataTypeCd, double snsnVal);
JSON_Value* jbody_strDataInfoVO_init(char *dataTypeCd, char *strVal);
JSON_Value* jbody_binDataInfoVO_init(char *dataTypeCd, char *binVal, int binValLen);
JSON_Value* jbody_binSetupDataInfoVO_init(char *snsnTagCd, char *setupVal, int binValLen);

JSON_Value* jbody_colecRowVO_init();
JSON_Value* jbody_colecRowVO_append_strDataInfoVO(JSON_Value* colecRowVO, JSON_Value* strDataInfoVO );
JSON_Value* jbody_colecRowVO_append_snsnDataInfoVO(JSON_Value* colecRowVO, JSON_Value* snsnDataInfoVO );
JSON_Value* jbody_colecRowVO_append_binDataInfoVO(JSON_Value* colecRowVO, JSON_Value* binDataInfoVO );

JSON_Value* jbody_devColecDataVO_init(char* devId);
JSON_Value* jbody_devColecDataVO_append_colecRowVO(JSON_Value* devColecDataVO, JSON_Value* colecRowVO );
JSON_Value* jbody_cnvyRowVO_init();
JSON_Value* jbody_cnvyRowVO_append_strDataInfoVO(JSON_Value* cnvyRowVO, JSON_Value* strDataInfoVO );
JSON_Value* jbody_cnvyRowVO_append_snsnDataInfoVO(JSON_Value* cnvyRowVO, JSON_Value* snsnDataInfoVO );
JSON_Value* jbody_devCnvyDataVO_init(char* devId);
JSON_Value* jbody_devCnvyDataVO_append_colecRowVO(JSON_Value* devColecDataVO, JSON_Value* cnvyRowVO );

JSON_Value* jbody_devBasVO_init(char* extrSysId, char* devId);
JSON_Value* jbody_devDtlVO_init(char* atribNm, char* atribVal);

#ifdef __cplusplus
}
#endif



#endif



