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
#include <stdlib.h>
#include <string.h>

#if !defined(WIN32)
#include <arpa/inet.h>
#endif

#include "util/log.h"
#include "util/util.h"
#include "packet/head.h"

static unsigned long long get_new_tranx_id() 
{
	return (unsigned long long)im_util_get_unique_number();
}


void im_pktHead_init(IMPacketHeadPtr head)
{
	IMHeadRawPtr hraw = &(head->headRaw);

	memset(head, 0x00, sizeof(IMPacketHead));

	hraw->protoMainVer		= IOTMAKERS_PROTOCOL_VER_MAJOR;	// 프로토콜 메인버전
	hraw->protoSubVer		= IOTMAKERS_PROTOCOL_VER_MINOR;	// 프로토콜 하위버전
	hraw->hdrType			= HdrType_BASIC;				// 헤더타입
	hraw->hdrLen			= htons(IOTMAKERS_DEF_HEAD_LEN);	// 헤더 길이

	im_pktHead_set_msgType(head, MsgType_REQUEST);			// 메시지 유형
	im_pktHead_set_msgExchPtrn(head, MsgExchPtrn_NONE);		// 메시지 교환 패턴
	im_pktHead_set_methodType(head, MthdType_NONE);			// 기능유형

	hraw->trmTransacId		= get_new_tranx_id();			// 트랜잭션아이디
	
	memcpy(hraw->commChAthnNo, (char*)im_base_get_commChAthnNo(), IOTMAKERS_STR_16_BYTE_LEN);



	hraw->encryUsage		= UseYn_NO;						// 암호화사용여부
	hraw->encryType			= EcodType_AES_128;				// 암호화유형
	
	hraw->compUsage			= UseYn_NO;						// 압축사용여부
	hraw->compType			= CmpreType_HUFFMAN;			// 압축유형
	
	hraw->encodType			= EncdngType_JSON;				// 인코딩유형
	hraw->rsltCd			= RespCd_NONE;					// 결과코드

	head->ext_cnt			= 0;		// 헤더확장필드수
	return;
}

void im_pktHead_release(IMPacketHeadPtr head)
{
	IMHeadExtPtr ext = &(head->ext);
	im_headExt_release(ext);
}

void im_pktHead_copy_trmTransacId(IMPacketHeadPtr headDest, IMPacketHeadPtr headSrc)
{
	headDest->headRaw.trmTransacId = headSrc->headRaw.trmTransacId;
}

/* =====================================
head get/set
====================================== */
void im_pktHead_set_msgType(IMPacketHeadPtr head, MsgType type)
{
	head->headRaw.ugly.msgInfo.msgType = type;		// 메시지 유형
}
MsgType im_pktHead_get_msgType(IMPacketHeadPtr head)
{
	return (MsgType)head->headRaw.ugly.msgInfo.msgType;		// 메시지 유형
}
void im_pktHead_set_msgExchPtrn(IMPacketHeadPtr head, MsgExchPtrn type)
{
	head->headRaw.ugly.msgInfo.msgExchPtrn = type;		// 메시지 교환 패턴
}
MsgExchPtrn im_pktHead_get_msgExchPtrn(IMPacketHeadPtr head)
{
	return (MsgExchPtrn)head->headRaw.ugly.msgInfo.msgExchPtrn;		// 메시지 교환 패턴
}
void im_pktHead_set_methodType(IMPacketHeadPtr head, MthdType type)
{
#if defined(REVERSED)
	head->headRaw.ugly.methodType |=  (htons(type)& 0x0FFF) ;	
#else
	// mask least 12bits(0xFF0F) in network byte order of 16 bits
	head->headRaw.ugly.methodType |=  (htons(type)& 0xFF0F) ;	
#endif
}
MthdType im_pktHead_get_methodType(IMPacketHeadPtr head)
{
#if defined(REVERSED)
	return (MthdType)ntohs(head->headRaw.ugly.methodType & 0x0FFF) ;	
#else
	// mask least 12bits(0xFF0F) in network byte order of 16 bits
	return (MthdType)ntohs(head->headRaw.ugly.methodType & 0xFF0F) ;	
#endif
}

void im_pktHead_set_trmTransacId(IMPacketHeadPtr head, unsigned long long trxid)
{
	head->headRaw.trmTransacId	= trxid;
}
unsigned long long im_pktHead_get_trmTransacId(IMPacketHeadPtr head)
{
	return head->headRaw.trmTransacId;
}

void im_pktHead_set_respCd(IMPacketHeadPtr head, RespCd rcode)
{
	head->headRaw.rsltCd = htons(rcode);
}
RespCd im_pktHead_get_respCd(IMPacketHeadPtr head)
{
	return (RespCd)ntohs(head->headRaw.rsltCd);
}

void im_pktHead_set_hdrLen(IMPacketHeadPtr head, unsigned short len)
{
	DEBUG_LOG("len=[%d]", len);
	head->headRaw.hdrLen = htons(len);
}
unsigned short im_pktHead_get_hdrLen(IMPacketHeadPtr head)
{
	return ntohs(head->headRaw.hdrLen);
}

MthdType im_pktHead_look_MethType(char* recvBuf, int recvLen)
{
	int index = 0;
	IMPacketHeadPtr head  = (IMPacketHeadPtr)&recvBuf[IOTMAKERS_TCP_HEAD_4_BYTE_LEN];

	if ( recvLen < IOTMAKERS_DEF_HEAD_LEN  )	{
		return MthdType_NONE;
	}

	return (MthdType)im_pktHead_get_methodType(head);
}

/* =====================================
head -> extension
====================================== */
int im_pktHeadExt_get_count(IMPacketHeadPtr head)
{
	return head->ext_cnt;
}

int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val)
{
	IMHeadExtPtr ext = &(head->ext);
	if ( head->ext_cnt == 0 )	{
		im_headExt_init(ext);
	}

	if ( im_headExt_put_kvp_str(ext, key, val) < 0 )	{
		if ( head->ext_cnt == 0 ){
			im_headExt_release(ext);
		}
		return -1;
	}
	head->ext_cnt++;
	return 0;
}
char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key)
{
	IMHeadExtPtr ext = &(head->ext);
	if ( head->ext_cnt == 0 ){
		return NULL;
	}
	return (char*)im_headExt_get_kvp_str(ext, key);
}

/* =====================================
head -> extension -> (de)serialize
====================================== */
int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) 
{
	IMHeadExtPtr ext = &(head->ext);
	int index;
	int i;

	if ( head->ext_cnt <= 0 ) {
		return 0;
	}

	index = 0;

	for (i=0; i<head->ext_cnt; i++)
	{
		char *key = (char*)im_headExt_get_kvp_name_with_index(ext, i);
		if (key == NULL)	{
			break;
		}
		char *val = (char*)im_headExt_get_kvp_str(ext, key);

		// 1.key_len: 1BYTE
		unsigned char key_len = (unsigned char)strlen(key);
		key_len &= IOTMAKERS_8_BIT_MASKING;

		memcpy(&o_buff[index], &key_len, IOTMAKERS_TCP_HEAD_1_BYTE_LEN);
		index += IOTMAKERS_TCP_HEAD_1_BYTE_LEN;
		if ( index > o_buff_len ){
			ERROR_LOG("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 2. key: n BYTE
		memcpy(&o_buff[index], key, strlen(key));
		index += strlen(key);
		if ( index > o_buff_len ){
			ERROR_LOG("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 3. val_len: 2BYTE
		unsigned short val_len = (unsigned char)strlen(val);
		val_len = htons(val_len) & IOTMAKERS_16_BIT_MASKING;

		memcpy(&o_buff[index], &val_len, IOTMAKERS_TCP_HEAD_2_BYTE_LEN);
		index += IOTMAKERS_TCP_HEAD_2_BYTE_LEN;
		if ( index > o_buff_len ){
			ERROR_LOG("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}

		// 4. val: n BYTE
		memcpy(&o_buff[index], val, strlen(val));
		index += strlen(val);
		if ( index > o_buff_len ){
			ERROR_LOG("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
			return -1;
		}
	}

	return index;
}

int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) 
{
	int ext_len;
	int index;
	
	if ( im_pktHead_get_hdrLen(head) <= IOTMAKERS_DEF_HEAD_LEN ) {
		return 0;
	}

	ext_len = im_pktHead_get_hdrLen(head) - IOTMAKERS_DEF_HEAD_LEN;
	for (index = 0; index < ext_len; )
	{
		unsigned char key_len = 0;
		memcpy(&key_len, &buf[index], IOTMAKERS_TCP_HEAD_1_BYTE_LEN);
		index += IOTMAKERS_TCP_HEAD_1_BYTE_LEN;

		char *key = (char*)malloc(key_len+1);
		memcpy(key, &buf[index], key_len);
		key[key_len] = '\0';
		index += key_len;

		unsigned short val_len = 0;
		memcpy(&val_len, &buf[index], IOTMAKERS_TCP_HEAD_2_BYTE_LEN);
		val_len = ntohs(val_len) & IOTMAKERS_16_BIT_MASKING;
		index += IOTMAKERS_TCP_HEAD_2_BYTE_LEN;

		char *val = (char*)malloc(val_len+1);
		memcpy(val, &buf[index], val_len);
		val[val_len] = '\0';
		index += val_len;

		im_pktHeadExt_add_val(head, key, val);

		free(key);
		free(val);
	}

	return head->ext_cnt;
}


/* =====================================
 head (de)serialize
====================================== */
int im_pktHead_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len)
{
	int hdlen = 0;
	int hdextlen = 0;

	hdlen = IOTMAKERS_DEF_HEAD_LEN;

	if ( o_buff_len < hdlen )	{
		ERROR_LOG("o_buff is too small ; o_buff_len=[%d]", o_buff_len);
		return -1;
	}

	if ( head->ext_cnt > 0 )	
	{
		hdextlen = im_pktHeadExt_get_serialized_to_buff(head, 
			&o_buff[IOTMAKERS_DEF_HEAD_LEN], o_buff_len-IOTMAKERS_DEF_HEAD_LEN);
		if ( hdextlen <= 0 ){
			ERROR_LOG("fail im_pktHeadExt_get_serialized_to_buff()");
			return -1;
		}
		hdlen = IOTMAKERS_DEF_HEAD_LEN + hdextlen;
		im_pktHead_set_hdrLen(head, hdlen);
	}

	memcpy(&o_buff[0], (char*)&(head->headRaw), IOTMAKERS_DEF_HEAD_LEN);
	
	return hdlen;
}


int im_pktHead_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff)
{
	int hdlen = 0;
	int ext_cnt = 0;

	im_pktHead_init(head);

	hdlen = IOTMAKERS_DEF_HEAD_LEN;
	memcpy((char*)&(head->headRaw), &i_buff[0], hdlen);

	if ( im_pktHead_get_hdrLen(head) > hdlen ) {
		ext_cnt = im_pktHeadExt_get_deserialize_from_buff(head, &i_buff[hdlen]);
		if ( ext_cnt <= 0 ){
			ERROR_LOG("fail im_pktHeadExt_get_deserialize_from_buff()");
			return -1;
		}
	}

	return im_pktHead_get_hdrLen(head);
}




/*************************************/
/*************************************/
/*************************************/
/*************************************/
/*************************************/

static char* im_pktHead_get_string_RsltCd(RespCd respCd) 
{
	switch (respCd) {
	/** 정상 */
	case RespCd_NORMALITY:
		return "RespCd_NORMALITY";
		break;
	/** 일반오류 */
	case RespCd_GENERAL_ERROR:
		return "RespCd_GENERAL_ERROR";
		break;
	/** 구현오류 */
	case RespCd_IMPLEMENTATION_ERROR:
		return "RespCd_IMPLEMENTATION_ERROR";
		break;
	/** 패킷 푸시 오류 */
	case RespCd_PACKET_PUSH_ERROR:
		return "RespCd_PACKET_PUSH_ERROR";
		break;
	/** 복호화 오류 */
	case RespCd_DECRYPTION_ERROR:
		return "RespCd_DECRYPTION_ERROR";
		break;
	/** 패킷파싱 오류 */
	case RespCd_PASSING_ERROR:
		return "RespCd_PASSING_ERROR";
		break;
	/** 인증 오류 */
	case RespCd_CERTIFICATION_ERROR:
		return "RespCd_CERTIFICATION_ERROR";
		break;
	/** 응답 오류 */
	case RespCd_ACK_ERROR:
		return "RespCd_ACK_ERROR";
		break;
	/** 통신채널인증오류 */
	case RespCd_COMM_CH_ATHN_ERROR:
		return "RespCd_COMM_CH_ATHN_ERROR";
		break;
	/** 요청정보오류 */
	case RespCd_REQUEST_INFO_ERROR:
		return "RespCd_REQUEST_INFO_ERROR";
		break;
	default:
		break;
	}

	return "RespCd_UNKNOWN";
}

static char* im_pktHead_get_string_HdrType(HdrType hdrType)
{
	switch (hdrType) {
	case HdrType_BASIC:
		return "HdrType_BASIC";
		break;
	case HdrType_LIGHT_WEIGHT:
		return "HdrType_LIGHT_WEIGHT";
		break;
	default:
		break;
	}

	return "UNKNWON";
}

static char* im_pktHead_get_string_MsgType(MsgType msgType)
{
	switch (msgType) {
	case MsgType_REQUEST:
		return "MsgType_REQUEST";
		break;
	case MsgType_RESPONSE:
		return "MsgType_RESPONSE";
		break;
	case MsgType_REPORT:
		return "MsgType_REPORT";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_MsgExchPtrn(MsgExchPtrn msgExchPtrn) 
{
	switch (msgExchPtrn) {
	case MsgExchPtrn_ONE_WAY:
		return "MsgExchPtrn_ONE_WAY";
		break;
	case MsgExchPtrn_ONE_WAY_ACK:
		return "MsgExchPtrn_ONE_WAY_ACK";
		break;
	case MsgExchPtrn_THREE_WAY:
		return "MsgExchPtrn_THREE_WAY";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_MthdType(MthdType mthdType)
{
	switch (mthdType) {
	case MthdType_VER_GWVER_RQT:
		return "MthdType_VER_GWVER_RQT";
		break;
	case MthdType_ATHN_LOGINATHN_EXTRSYS_TCP:
		return "MthdType_ATHN_LOGINATHN_EXTRSYS_TCP";
		break;
	case MthdType_ATHN_LOGINATHN_DEV_TCP:
		return "MthdType_ATHN_LOGINATHN_DEV_TCP";
		break;
	case MthdType_ATHN_COMMCHATHN_EXTRSYS_TCP:
		return "MthdType_ATHN_COMMCHATHN_EXTRSYS_TCP";
		break;
	case MthdType_ATHN_COMMCHATHN_DEV_TCP:
		return "MthdType_ATHN_COMMCHATHN_DEV_TCP";
		break;
	case MthdType_KEEP_ALIVE_COMMCHATHN_TCP:
		return "MthdType_KEEP_ALIVE_COMMCHATHN_TCP";
		break;
	case MthdType_INITA_DATATYPE_RETV:
		return "MthdType_INITA_DATATYPE_RETV";
		break;
	case MthdType_INITA_EXTRSYS_RETV:
		return "MthdType_INITA_EXTRSYS_RETV";
		break;
	case MthdType_INITA_EXTRSYS_UDATERPRT:
		return "MthdType_INITA_EXTRSYS_UDATERPRT";
		break;
	case MthdType_INITA_DEV_RETV:
		return "MthdType_INITA_DEV_RETV";
		break;
	case MthdType_INITA_DEV_UDATERPRT:
		return "MthdType_INITA_DEV_UDATERPRT";
		break;
	case MthdType_COLEC_SETUP_FLTRCONDRETV:
		return "MthdType_COLEC_SETUP_FLTRCONDRETV";
		break;
	case MthdType_COLEC_SETUP_FLTRCONDTRMN:
		return "MthdType_COLEC_SETUP_FLTRCONDTRMN";
		break;
	case MthdType_COLEC_SETUP_EVCONDRETV:
		return "MthdType_COLEC_SETUP_EVCONDRETV";
		break;
	case MthdType_COLEC_SETUP_EVCONDTRMN:
		return "MthdType_COLEC_SETUP_EVCONDTRMN";
		break;
	case MthdType_COLEC_ITGDATA_RECV:
		return "MthdType_COLEC_ITGDATA_RECV";
		break;
	case MthdType_COLEC_SNSNDATA_RECV:
		return "MthdType_COLEC_SNSNDATA_RECV";
		break;
	case MthdType_COLEC_LODATA_RECV:
		return "MthdType_COLEC_LODATA_RECV";
		break;
	case MthdType_COLEC_STTUSDATA_RECV:
		return "MthdType_COLEC_STTUSDATA_RECV";
		break;
	case MthdType_COLEC_BINDATA_RECV:
		return "MthdType_COLEC_BINDATA_RECV";
		break;
	case MthdType_COLEC_EVDATA_RECV:
		return "MthdType_COLEC_EVDATA_RECV";
		break;
	case MthdType_CONTL_SYS_RTIMECONTL:
		return "MthdType_CONTL_SYS_RTIMECONTL";
		break;
	case MthdType_CONTL_SYS_SETUPCHG:
		return "MthdType_CONTL_SYS_SETUPCHG";
		break;
	case MthdType_CONTL_DEV_RTIMECONTL:
		return "MthdType_CONTL_DEV_RTIMECONTL";
		break;
	case MthdType_CONTL_DEV_SETUPCHG:
		return "MthdType_CONTL_DEV_SETUPCHG";
		break;
	case MthdType_CONTL_DEV_FRMWRUDATE:
		return "MthdType_CONTL_DEV_FRMWRUDATE";
		break;
	case MthdType_CONTL_ITGCNVY_DATA:
		return "MthdType_CONTL_ITGCNVY_DATA";
		break;
	case MthdType_CHK_SYS_CHKPACKTRCV:
		return "MthdType_CHK_SYS_CHKPACKTRCV";
		break;
	case MthdType_CHK_DEV_CHKPACKTRCV:
		return "MthdType_CHK_DEV_CHKPACKTRCV";
		break;
	case MthdType_LAST_VAL_QUERY:
		return "MthdType_LAST_VAL_QUERY";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_UseYn(UseYn useYn) 
{
	switch (useYn) {
	case UseYn_NO:
		return "UseYn_NO";
		break;
	case UseYn_YES:
		return "UseYn_YES";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_EncdngType(EncdngType encdngType) 
{
	switch (encdngType) {
	case EncdngType_USER_DEFINED:
		return "EncdngType_USER_DEFINED";
		break;
	case EncdngType_XML:
		return "EncdngType_XML";
		break;
	case EncdngType_JSON:
		return "EncdngType_JSON";
		break;
	case EncdngType_GPB:
		return "EncdngType_GPB";
		break;
	case EncdngType_THRIFT:
		return "EncdngType_THRIFT";
		break;
	case EncdngType_AVRO:
		return "EncdngType_AVRO";
		break;
	case EncdngType_PCRE:
		return "EncdngType_PCRE";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_EcodType(EcodType ecodType)
{
	switch (ecodType) {
	case EcodType_AES_128:
		return "EcodType_AES_128";
		break;
	case EcodTypeAES_256:
		return "EcodTypeAES_256";
		break;
	case EcodType_DES_64:
		return "EcodType_DES_64";
		break;
	case EcodType_DES_EDE_192:
		return "EcodType_DES_EDE_192";
		break;
	case EcodType_SEED_128:
		return "EcodType_SEED_128";
		break;
	case EcodType_ARIA_128:
		return "EcodType_ARIA_128";
		break;
	case EcodType_ARIA_192:
		return "EcodType_ARIA_192";
		break;
	case EcodType_ARIA_256:
		return "EcodType_ARIA_256";
		break;
	default:
		break;
	}
	return "UNKNWON";
}

static char* im_pktHead_get_string_CmpreType(CmpreType cmpreType) 
{
	switch (cmpreType) {
	case CmpreType_HUFFMAN:
		return "CmpreType_HUFFMAN";
		break;
	case CmpreType_RUN_LENGTH:
		return "CmpreType_RUN_LENGTH";
		break;
	case CmpreType_SHANNON_FANO:
		return "CmpreType_SHANNON_FANO";
		break;
	default:
		break;
	}
	return "UNKNWON";
}


static char dump_msg_buf[IOTMAKERS_MAX_LOG_MSG_LEN + 1] = "\0";

void im_pktHead_print(IMPacketHeadPtr head) 
{
	IMHeadRawPtr hraw = &(head->headRaw);

	switch (im_pktHead_get_msgType(head))
	{
	case MsgType_REQUEST:
		DEBUG_LOG("%s", "[REQUEST]"); break;
	case MsgType_RESPONSE:
		DEBUG_LOG("%s", "[RESPONSE]"); break;
	case MsgType_REPORT:
		DEBUG_LOG("%s", "[REPORT]"); break;
	default:
		DEBUG_LOG("%s", "[UNKNOWN]"); break;
	}

	DEBUG_LOG("\tprotoMainVer: [%d]", hraw->protoMainVer);
	DEBUG_LOG("\tprotoSubVer: [%d]", hraw->protoSubVer);
	DEBUG_LOG("\thdrType: [%s]", im_pktHead_get_string_HdrType((HdrType)hraw->hdrType));
	DEBUG_LOG("\thdrLen: [%d]", im_pktHead_get_hdrLen(head));
	DEBUG_LOG("\tmsgType: [%s]", im_pktHead_get_string_MsgType(im_pktHead_get_msgType(head)) );
	DEBUG_LOG("\tmsgExchPtrn: [%s]", im_pktHead_get_string_MsgExchPtrn(im_pktHead_get_msgExchPtrn(head)));
	DEBUG_LOG("\tmthdType: [%s]", im_pktHead_get_string_MthdType(im_pktHead_get_methodType(head)) );
	DEBUG_LOG("\ttrmTransacId: [%lld]", hraw->trmTransacId);

	int i = 0;
	int idx = 0;
	int athnNoLen = strlen(hraw->commChAthnNo);
	for (i = 0; i < athnNoLen; i++) {
		idx += snprintf(&dump_msg_buf[idx], IOTMAKERS_MAX_LOG_MSG_LEN, "%02X ", (unsigned char)hraw->commChAthnNo[i]);
	}
	DEBUG_LOG( "\tcommChAthnNo:%s", dump_msg_buf);

	DEBUG_LOG( "\tencryUsage: [%s]", im_pktHead_get_string_UseYn((UseYn)hraw->encryUsage));
	DEBUG_LOG( "\tencryType: [%s]", im_pktHead_get_string_EcodType((EcodType)hraw->encryType));
	DEBUG_LOG( "\tcompUsage: [%s]", im_pktHead_get_string_UseYn((UseYn)hraw->compUsage));
	DEBUG_LOG( "\tcompType: [%s]", im_pktHead_get_string_CmpreType((CmpreType)hraw->compType));
	DEBUG_LOG( "\tencodType: [%s]", im_pktHead_get_string_EncdngType((EncdngType)hraw->encodType));
	DEBUG_LOG( "\trsltCd: [%d]", im_pktHead_get_respCd(head));

	return;
}
