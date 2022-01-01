/*
 * GiGA IoT Platform version 2.0
 *
 *  Copyright ¨Ï 2015 kt corp. All rights reserved.
 *
 *  This is a proprietary software of kt corp, and you may not use this file except in
 *  compliance with license agreement with kt corp. Any redistribution or use of this
 *  software, with or without modification shall be strictly prohibited without prior written
 *  approval of kt corp, and the copyright notice above does not evidence any actual or
 *  intended publication of such software.
 */

#ifndef IOTMAKERS_H
#define IOTMAKERS_H

#define LOG_LEVEL_ERROR   1                    /* Log level  : Error               */
#define LOG_LEVEL_INFO    2                    /* Log level  : Information         */
#define LOG_LEVEL_DEBUG   3                    /* Log level  : Debug               */

typedef void (*IMCbTagidNumDataHndl)(char *tagid, double val);
typedef void (*IMCbTagidStrDataHndl)(char *tagid, char *val);
typedef void (*IMCbDataRespHndl)(long long trxid, char *respCode);
typedef void (*IMCbErrorHndl)(int errCode);

typedef void (*IMCbIF711Hndl)(void *pVO, char *devId, char* cmdType);
typedef void (*IMCbIF333Hndl)(void *pVO, char *devId, char* cmdType);


#ifdef __cplusplus
extern "C"
{
#endif

int im_init(char *ip, int port, char *deviceId, char *athnRqtNo, char *extrSysId, char *log_file_name);
int im_init_with_config_file(char *fname);
void im_release() ;

int im_get_LastErrCode();
void im_set_loglevel(int loglevel);

#if  defined (_IM_C_SOCK_) 
void im_set_socktimeout_sec(int sec);
#endif

void im_set_numdata_handler(IMCbTagidNumDataHndl cb_proc);
void im_set_strdata_handler(IMCbTagidStrDataHndl cb_proc);
void im_set_dataresp_handler(IMCbDataRespHndl cb_proc);
void im_set_error_handler(IMCbErrorHndl cb_proc);
void im_set_if711_handler(IMCbIF711Hndl cb_proc);

void im_disconnect();
int im_connect();
int im_connected();

int im_auth_device();

#if defined (_IM_C_SOCK_) 
int im_send_keepalive();
int im_recv_control_packet(int timeout_sec);
#endif

int im_start_service();
void im_stop_service();

long long im_get_new_trxid();

int im_init_complexdata();
int im_init_complexdata_with_devid(char *devId);
int im_add_numdata_to_complexdata(char *tagid, double val);
int im_add_strdata_to_complexdata(char *tagid, char *val);
int im_send_complexdata(long long trxid);

int im_send_numdata(char *tagid, double val, long long trxid);
int im_send_strdata(char *tagid, char *val, long long trxid);

int im_send_numdata_with_devid(char *devId, char *tagid, double val, long long trxid);
int im_send_strdata_with_devid(char *devId, char *tagid, char *val, long long trxid);


int im_send_numdata(char *tagid, double val, long long trxid);
int im_send_strdata(char *tagid, char *val, long long trxid);

void im_error_handler(int errcode);

int im_send_report_numdata(char *tagid, double val);
int im_send_report_strdata(char *tagid, char *val);

int im_init_if332();
int im_set_if332_insert(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
int im_set_if332_delete(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
int im_set_if332_update(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
int im_set_if332_update_all(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
int im_set_if332_avp (char* attr, char* val);
int im_send_if332(long long trxid);


int im_add_numdata_for_if711(void* body, char *tagid, double val);
int im_add_strdata_for_if711(void* body, char *tagid, char *val);
int im_add_bindata_for_if711(void* body, char *tagid, char *val, int valLen);



#ifdef __cplusplus
}
#endif

#endif
