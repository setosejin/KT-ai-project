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

#ifndef IOTMAKERS_PKTHEAD_H
#define IOTMAKERS_PKTHEAD_H

#include "im_common.h"
#include "packet/head_ext.h"

#define IOTMAKERS_PROTOCOL_VER_MAJOR		1
#define IOTMAKERS_PROTOCOL_VER_MINOR		1

#define IOTMAKERS_DEF_HEAD_LEN			35

/*
 * ********************************************************************************
 * packet/comn.h
 * ********************************************************************************
 */

typedef enum {
	HdrType_BASIC				= 1
	, HdrType_LIGHT_WEIGHT		= 2
} HdrType;

typedef enum {
	MsgType_NONE				= 0
	, MsgType_REQUEST			= 1
	, MsgType_RESPONSE			= 2
	, MsgType_REPORT			= 3
} MsgType;

typedef enum {
	MsgExchPtrn_NONE			= 0
	, MsgExchPtrn_ONE_WAY		= 1
	, MsgExchPtrn_ONE_WAY_ACK	= 2
	, MsgExchPtrn_THREE_WAY		= 3
} MsgExchPtrn;

typedef enum {
	UseYn_NO					= 0
	, UseYn_YES					= 1
} UseYn;

typedef enum {
	EncdngType_USER_DEFINED		= 0x01
	, EncdngType_XML			= 0x02
	, EncdngType_JSON			= 0x03
	, EncdngType_GPB			= 0x10
	, EncdngType_THRIFT			= 0x11
	, EncdngType_AVRO			= 0x12
	, EncdngType_PCRE			= 0x13
} EncdngType;

typedef enum {
	EcodType_AES_128			= 0x01
	, EcodTypeAES_256			= 0x02
	, EcodType_DES_64			= 0x11
	, EcodType_DES_EDE_192		= 0x12
	, EcodType_SEED_128			= 0x21
	, EcodType_ARIA_128			= 0x31
	, EcodType_ARIA_192			= 0x32
	, EcodType_ARIA_256			= 0x33
} EcodType;

typedef enum {
	CmpreType_HUFFMAN = 1
	, CmpreType_RUN_LENGTH = 2
	, CmpreType_SHANNON_FANO = 3
} CmpreType;


typedef enum {
	MthdType_NONE							= 0
	/** 플랫폼 및 어댑터 버전 조회 */
	, MthdType_VER_GWVER_RQT				= 111
	/** 외부시스템 TCP 로그인  */
	, MthdType_ATHN_LOGINATHN_EXTRSYS_TCP	= 212
	/** 장치 TCP 로그인 */
	, MthdType_ATHN_LOGINATHN_DEV_TCP		= 214
	/** 외부시스템 TCP 채널 인증 */
	, MthdType_ATHN_COMMCHATHN_EXTRSYS_TCP	= 222
	/** 장치 TCP 채널 인증 */
	, MthdType_ATHN_COMMCHATHN_DEV_TCP 		= 224
	/** KeepAlive TCP 채널 */
	, MthdType_KEEP_ALIVE_COMMCHATHN_TCP	= 231
	/** 데이터유형 조회 */
	, MthdType_INITA_DATATYPE_RETV			= 311
	/** 외부시스템정보 조회 */
	, MthdType_INITA_EXTRSYS_RETV			= 321
	/** 외부시스템정보 갱신보고 */
	, MthdType_INITA_EXTRSYS_UDATERPRT		= 322
	/** 장치정보 조회 */
	, MthdType_INITA_DEV_RETV				= 331
	/** 장치정보 갱신보고 */
	, MthdType_INITA_DEV_UDATERPRT			= 332
	/** 장치정보 조회(플랫폼) */
	, MthdType_INITA_DEV_INFO				= 333
	/** 수집설정 필터링조건조회 */
	, MthdType_COLEC_SETUP_FLTRCONDRETV		= 341
	/** 수집설정 필터링조건해지 */
	, MthdType_COLEC_SETUP_FLTRCONDTRMN		= 343
	/** 수집설정 이벤트조건조회 */
	, MthdType_COLEC_SETUP_EVCONDRETV		= 344
	/** 수집설정 이벤트조건해지 */
	, MthdType_COLEC_SETUP_EVCONDTRMN		= 346
	/** 수집 통합데이터 */
	, MthdType_COLEC_ITGDATA_RECV			= 411
	/** 수집 센싱데이터 */
	, MthdType_COLEC_SNSNDATA_RECV			= 421
	/** 수집 위치데이터 */
	, MthdType_COLEC_LODATA_RECV			= 431
	/** 수집 상태데이터 */
	, MthdType_COLEC_STTUSDATA_RECV			= 441
	/** 수집 이진데이터 */
	, MthdType_COLEC_BINDATA_RECV			= 451
	/** 수집 이벤트 */
	, MthdType_COLEC_EVDATA_RECV			= 461
	/** 시스템제어 실시간제어 */
	, MthdType_CONTL_SYS_RTIMECONTL			= 511
	/** 시스템제어 설정변경 */
	, MthdType_CONTL_SYS_SETUPCHG			= 512
	/** 장치제어 실시간제어 */
	, MthdType_CONTL_DEV_RTIMECONTL			= 521
	/** 장치제어 설정변경 */
	, MthdType_CONTL_DEV_SETUPCHG			= 522
	/** 장치제어 펌웨어업데이트 */
	, MthdType_CONTL_DEV_FRMWRUDATE			= 523
	/** 통합데이터 전달 */
	, MthdType_CONTL_ITGCNVY_DATA			= 525
	/** 시스템점검 체크패킷수신 */
	, MthdType_CHK_SYS_CHKPACKTRCV			= 612
	/** 장치점검 체크패킷수신 */
	, MthdType_CHK_DEV_CHKPACKTRCV			= 622

	// (TODO)
	// 임시로 만들었음...
	, MthdType_LAST_VAL_QUERY				= 711

	, MthdType_FRMWR_INFO					= 811
	, MthdType_FRMWR_UPDATE_NTFY			= 812
	, MthdType_FRMWR_UPDATE_STTUS_TRM		= 813
	, MthdType_FRMWR_DATA_TCP				= 816
	, MthdType_PKG_DATA_TCP					= 817
	,
} MthdType;

typedef enum {
	RespCd_NONE = 0
	/** 정상 */
	, RespCd_NORMALITY = 100
	/** 일반오류 */
	, RespCd_GENERAL_ERROR = 200
	/** 구현오류 */
	, RespCd_IMPLEMENTATION_ERROR = 201
	/** 패킷 푸시 오류 */
	, RespCd_PACKET_PUSH_ERROR = 202
	/** 복호화 오류 */
	, RespCd_DECRYPTION_ERROR = 203
	/** 패킷파싱 오류 */
	, RespCd_PASSING_ERROR = 204
	/** 인증 오류 */
	, RespCd_CERTIFICATION_ERROR = 205
	/** 응답 오류 */
	, RespCd_ACK_ERROR = 206
	/** 통신채널인증오류 */
	, RespCd_COMM_CH_ATHN_ERROR = 207
	/** 요청정보오류 */
	, RespCd_REQUEST_INFO_ERROR = 208
} RespCd;


/*
 * ********************************************************************************
 * 연동하는 메시지들이 공통적으로 사용하는 메시지를 정의한다.
 *
 * 중요한 항목으로는 메시지헤더VO(IMPacketHead), 공통요청VO(ComnReqVO), 공통응답VO(ComnRespVO), 공통보고VO(ComnRprt) 등이 있다.
 * 또한 주요 메시지들 내에서 공통으로 사용되는 데이터 구조체(DevColecDataVO, SnsnDataInfoVO, StrDataInfoVO)들을 정의한다.
 *
 * 예를들어, 만약 통합수집데이터VO를 생성하려고 한다면
 * packet/400.h 에 정의된 통합수집데이터VO(ItgColecDataVO)를 생성하고, 해당 VO내에 장비수집데이터VO(DevColecDataVO)를
 * 추가하기 위해서는 packet/comn.h에 정의된 함수들을 사용해야 한다.
 * 장비수집데이터VO는 다른 메시지에서도 사용되는 구조체이기 때문에 packet/comn.h에 정의되어 있다.
 * ********************************************************************************
 */

/*
 * 메시지헤더VO(IMPacketHead)를 정의한다.
 * 메시지헤더는 모든 메시지(요청/응답/보고)에 공통적으로 포함되는 기본 속성이다.
 *
 * KT표준프로토콜의 구조는 [TCP Packet Length (4 Byte)] + [Message Header(35 Byte)] + [Message Body]로 구성되는데,
 * 메시지헤더VO는 [Message Header]를 의미한다.
 *
 * 그리고 메시지헤더VO가 전송될 때, 35 Byte로 변환되어 전송되고, (메시지헤더확장VO 제외하고, 35 Byte)

 0000: 00 00 00 87 11 01 00 23 | 60 e0 24 12 62 57 00 00 
 ..................==.==.=====...-====.,,,,,,,,,,,,,,,,,
 ................................msgType:2--MsgType_REQUEST(1) 01
 ................................msgExchPtrn:2--MsgExchPtrn_ONE_WAY_ACK(2) 10
 ................................methodType:12--MthdType_ATHN_COMMCHATHN_DEV_TCP(224)
 ................................01100000 1110000

60e0 :x86 0xFF0F (OK)
60e0 :mips(R) 0x0FFF (OK)
 */

#pragma pack(1)
typedef struct {
	unsigned char			protoMainVer:4;			// 프로토콜 상위버전		(4bit)
	unsigned char			protoSubVer:4;			// 프로토콜 하위버전		(4bit)
	unsigned char			hdrType;				// 헤더타입					(8bit)
	unsigned short			hdrLen;					// 헤더 길이				(16bit)
	// 4 bytes

	union ugly {
		unsigned char __pad [2];
#if defined(REVERSED)
		struct ugly2 {
			unsigned char	msgType:2;				// 메시지 유형				(최상위2bit)
			unsigned char	msgExchPtrn:2;			// 메시지 교환 패턴			(상위2bit)
			unsigned char	__dummy:4;
		}msgInfo;
#else
		struct ugly2 {
			unsigned char	__dummy:4;
			unsigned char	msgExchPtrn:2;			// 메시지 교환 패턴			(상위2bit)
			unsigned char	msgType:2;				// 메시지 유형				(최상위2bit)
		}msgInfo;
#endif
		unsigned short		methodType;				// 기능유형					(하위12bit)
	}ugly;
	// 2 bytes

	unsigned long long		trmTransacId;			// 트랜잭션아이디			(64bit)
	// 8 bytes

	char					commChAthnNo[IOTMAKERS_STR_16_BYTE_LEN];		// 통신채널인증토큰	(128bit)
	// 16 bytes

	unsigned char			encryType:7;			// 암호화유형				(하위7bit)
	unsigned char			encryUsage:1;			// 암호화사용여부			(상위1bit)
	// 1 byte
	
	unsigned char			compType:7;				// 압축유형					(하위7bit)
	unsigned char			compUsage:1;			// 압축사용여부				(상위1bit)
	// 1 byte
	
	unsigned char			encodType;				// 인코딩유형				(8bit)
	// 1 byte
	
	unsigned short			rsltCd;					// 결과코드					(16bit)
	// 2 bytes
} IMHeadRaw, *IMHeadRawPtr;


#pragma pack(0)
typedef struct {
	IMHeadRaw	headRaw;
	IMHeadExt	ext;
	int			ext_cnt;	// 헤더확장필드수

} IMPacketHead, *IMPacketHeadPtr;


#ifdef __cplusplus
extern "C"
{
#endif

void im_pktHead_init(IMPacketHeadPtr head);
void im_pktHead_release(IMPacketHeadPtr head);

void im_pktHead_copy_trmTransacId(IMPacketHeadPtr headDest, IMPacketHeadPtr headSrc);

void im_pktHead_set_msgType(IMPacketHeadPtr head, MsgType type);
MsgType im_pktHead_get_msgType(IMPacketHeadPtr head);

void im_pktHead_set_msgExchPtrn(IMPacketHeadPtr head, MsgExchPtrn type);
MsgExchPtrn im_pktHead_get_msgExchPtrn(IMPacketHeadPtr head);

void im_pktHead_set_methodType(IMPacketHeadPtr head, MthdType type);
MthdType im_pktHead_get_methodType(IMPacketHeadPtr head);

void im_pktHead_set_trmTransacId(IMPacketHeadPtr head, unsigned long long trxid);
unsigned long long im_pktHead_get_trmTransacId(IMPacketHeadPtr head);

void im_pktHead_set_respCd(IMPacketHeadPtr head, RespCd rcode);
RespCd im_pktHead_get_respCd(IMPacketHeadPtr head);

void im_pktHead_set_hdrLen(IMPacketHeadPtr head, unsigned short len);
unsigned short im_pktHead_get_hdrLen(IMPacketHeadPtr head);

MthdType im_pktHead_look_MethType(char* recvBuf, int recvLen);
int im_pktHeadExt_get_count(IMPacketHeadPtr head);

int im_pktHeadExt_add_val(IMPacketHeadPtr head, char *key, char *val);
char* im_pktHeadExt_get_val(IMPacketHeadPtr head, char *key);

int im_pktHeadExt_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len) ;
int im_pktHeadExt_get_deserialize_from_buff(IMPacketHeadPtr head, char *buf) ;
int im_pktHead_get_serialized_to_buff(IMPacketHeadPtr head, char *o_buff, int o_buff_len);
int im_pktHead_get_deserialized_from_buff(IMPacketHeadPtr head, char* i_buff);
void im_pktHead_print(IMPacketHeadPtr head) ;

#ifdef __cplusplus
}
#endif






#endif

