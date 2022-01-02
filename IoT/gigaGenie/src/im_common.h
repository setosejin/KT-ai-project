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

#ifndef IOTMAKERS_COMMON_H
#define IOTMAKERS_COMMON_H

/*
 * ********************************************************************************
 * im_common.h
 * ********************************************************************************
 */
#define IOTMAKERS_SDK_C_VERSION				"2.2.0"

#define IOTMAKERS_KEEPALIVE_EXPIRE_TIME_SEC		50
#define IOTMAKERS_SOCKET_TIMEOUT_SEC			2
#define IOTMAKERS_CONFIG_FNAME				"./config.txt"

#if defined(ARDUINO_ARCH_AVR)
#define IOTMAKERS_MAX_SEND_MSG_LEN			64
#define IOTMAKERS_MAX_CONTROL_DATA			16
#elif defined(_LINUX_)
#define IOTMAKERS_MAX_SEND_MSG_LEN			4096
#define IOTMAKERS_MAX_CONTROL_DATA			64
#else
#define IOTMAKERS_MAX_SEND_MSG_LEN			4096
#define IOTMAKERS_MAX_CONTROL_DATA			64
#endif


#define IOTMAKERS_MAX(a, b)             ((a) > (b) ? (a) : (b))

#define IOTMAKERS_1_BIT_MASKING				0x00000001
#define IOTMAKERS_2_BIT_MASKING				0x00000003
#define IOTMAKERS_4_BIT_MASKING				0x0000000F
#define IOTMAKERS_7_BIT_MASKING				0x0000007F
#define IOTMAKERS_8_BIT_MASKING				0x000000FF
#define IOTMAKERS_12_BIT_MASKING			0x00000FFF
#define IOTMAKERS_16_BIT_MASKING			0x0000FFFF
#define IOTMAKERS_32_BIT_MASKING			0xFFFFFFFF
#define IOTMAKERS_64_BIT_MASKING			0xFFFFFFFFFFFFFFF

#define IOTMAKERS_TCP_HEAD_1_BYTE_LEN		1
#define IOTMAKERS_TCP_HEAD_2_BYTE_LEN		2
#define IOTMAKERS_TCP_HEAD_4_BYTE_LEN		4
#define IOTMAKERS_TCP_HEAD_8_BYTE_LEN		8
#define IOTMAKERS_TCP_HEAD_16_BYTE_LEN		16

#define IOTMAKERS_STR_1_BYTE_LEN			1
#define IOTMAKERS_STR_2_BYTE_LEN			2
#define IOTMAKERS_STR_4_BYTE_LEN			4
#define IOTMAKERS_STR_5_BYTE_LEN			5
#define IOTMAKERS_STR_8_BYTE_LEN			8
#define IOTMAKERS_STR_10_BYTE_LEN			10
#define IOTMAKERS_STR_16_BYTE_LEN			16
#define IOTMAKERS_STR_20_BYTE_LEN			20
#define IOTMAKERS_STR_30_BYTE_LEN			30
#define IOTMAKERS_STR_32_BYTE_LEN			32
#define IOTMAKERS_STR_39_BYTE_LEN			39
#define IOTMAKERS_STR_60_BYTE_LEN			60
#define IOTMAKERS_STR_64_BYTE_LEN			64
#define IOTMAKERS_STR_100_BYTE_LEN		100
#define IOTMAKERS_STR_128_BYTE_LEN		100
#define IOTMAKERS_STR_1024_BYTE_LEN		1024


typedef enum {
	IM_ErrCode_ERROR					= -1
	, IM_ErrCode_SUCCESS				= 0
	, IM_ErrCode_UNKNOWN				= 100

	, IM_ErrCode_CONFIG_PARS_ERROR		= 201
	, IM_ErrCode_MEM_ALLOC_ERROR		= 202

	, IM_ErrCode_HEAD_PARSE_ERROR		= 301
	, IM_ErrCode_BODY_PARSE_ERROR		= 302
	, IM_ErrCode_BODY_BUILD_ERROR		= 303
	, IM_ErrCode_PACKET_UNKNOWN			= 304

	, IM_ErrCode_SOCK_SEND_FAIL			= 401
	, IM_ErrCode_SOCK_RECV_FAIL			= 402
	, IM_ErrCode_SOCK_CONNECTION_FAIL	= 403
	, IM_ErrCode_SOCK_CONNECTION_LOSS	= 404

} IM_ErrCode;


typedef void (*IMCbTagidNumDataHndl)(char *tagid, double val);
typedef void (*IMCbTagidStrDataHndl)(char *tagid, char *val);
typedef void (*IMCbDataRespHndl)(long long trxid, char *respCode);
typedef void (*IMCbErrorHndl)(int errCode);
typedef void (*IMCbIF711Hndl)(void *pVO, char *devId, char* cmdType);
typedef void (*IMCbIF333Hndl)(void *pVO, char *devId, char* cmdType);
#endif
