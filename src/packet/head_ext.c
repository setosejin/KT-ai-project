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
#include "packet/head_ext.h"


/* =====================================
pHExt init
====================================== */
int im_headExt_init(IMHeadExtPtr pHExt) 
{
	pHExt->root_val = json_value_init_object();
	pHExt->root_obj = json_value_get_object(pHExt->root_val);
	pHExt->serialized_string = NULL;
	return 0;
}
int im_headExt_init_with_string(IMHeadExtPtr pHExt, char *json_str) 
{
	pHExt->root_val = json_parse_string(json_str);
	if ( pHExt->root_val == NULL )	{
		return -1;
	}
	pHExt->root_obj = json_value_get_object(pHExt->root_val);
	pHExt->serialized_string = NULL;
	return 0;
}
void im_headExt_release(IMHeadExtPtr pHExt) 
{
	if ( pHExt->serialized_string != NULL ){
		json_free_serialized_string(pHExt->serialized_string);
		pHExt->serialized_string = NULL;
	}

	if ( pHExt->root_val != NULL ){
		json_value_free(pHExt->root_val);
		pHExt->root_val = NULL;
	}
}


/* =====================================
key-value-pair
====================================== */
int im_headExt_put_kvp_str(IMHeadExtPtr pHExt, char *key, char *val) 
{
	pHExt->json_status = json_object_set_string(pHExt->root_obj, key, val);
	return (pHExt->json_status == JSONSuccess)?0:-1;
}
char* im_headExt_get_kvp_str(IMHeadExtPtr pHExt, char *key) 
{
	return (char*)json_object_get_string(pHExt->root_obj, key);
}
char* im_headExt_get_kvp_name_with_index(IMHeadExtPtr pHExt, int idx) 
{
	return (char*)json_object_get_name(pHExt->root_obj, idx);
}


/* =====================================
serialize
====================================== */
int im_packet_head_ext_get_serialized_strlen(IMHeadExtPtr pHExt) 
{
	//!!!
	//!!! json_serialization_size() counts the End Of String('\0') for string length.
	//!!!
	return json_serialization_size(pHExt->root_val) - 1;
}
char* im_packet_head_ext_get_serialized_str(IMHeadExtPtr pHExt) 
{
	if ( pHExt->serialized_string != NULL ){
		json_free_serialized_string(pHExt->serialized_string);
	}
	pHExt->serialized_string = json_serialize_to_string(pHExt->root_val);
	return (char*)pHExt->serialized_string;
}



/* =====================================
debug
====================================== */
void jsonwrap_print_value_serialized(JSON_Value* json_val)
{
    char *serialized_string = NULL;
	serialized_string = json_serialize_to_string_pretty(json_val);
	printf("serialized_string:\n%s\n", serialized_string);
    json_free_serialized_string(serialized_string);
}
