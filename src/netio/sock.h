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

#ifndef IOTMAKERS_NETIO_SOCK_H
#define IOTMAKERS_NETIO_SOCK_H

#define IM_SOCKET_TIMEOUT	3

#ifdef __cplusplus
extern "C"
{
#endif

/*
void im_sock_set_cb_recv_handler(void* cb);
int im_sock_set_read_timeout(int sock, int sec);
int im_sock_set_write_timeout(int sock, int sec);
int im_sock_set_timeout(int sock, int sec);
int im_sock_connect_timeout(char *remoteip, int port, int timeout_sec) ;
int im_sock_connect(char *ip, int port) ;
int im_sock_disconnect(int sock);
int im_sock_send(int sock, char *data, int data_len);
int im_sock_available(int sockfd, int timeout_sec);
int im_sock_recv(int sock, char *o_buff, int buff_len);
int im_sock_recv_data(int sock) ;
*/

int im_sock_connect(char *ip, unsigned short port) ;
int im_sock_disconnect();

void im_sock_set_cb_recv_handler(void* cb);
int im_sock_set_read_timeout(int sec);
int im_sock_set_write_timeout(int sec);
int im_sock_set_timeout(int sec);

int im_sock_connected();
void im_sock_flush();
int im_sock_available();

int im_sock_send(char *data, int data_len);
int im_sock_recv(char *o_buff, int buff_len);





#ifdef __cplusplus
}
#endif


#endif
