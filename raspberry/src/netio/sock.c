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
#include <errno.h>
#include <unistd.h>

#if defined(_IM_C_SOCK_)

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#endif


#include "util/log.h"
#include "base/base.h"
#include "im_common.h"


typedef void (*IMCbRecvMsgHandler)(char *data, int data_len);

static IMCbRecvMsgHandler _cb_onMsgarrvd_hndl = NULL;;

void im_sock_set_cb_recv_handler(void* cb)
{
	_cb_onMsgarrvd_hndl = (IMCbRecvMsgHandler)cb;
}


static int _net_sock_getaddr (struct sockaddr *addr, int sock_type, const char *host, int port_num)
{
    struct addrinfo hints, *res;
	char	port[32];
 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = sock_type;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_PASSIVE;
 
	sprintf(port, "%d", port_num);
    if (getaddrinfo(host, port, &hints, &res))
        return -1;
 
    if (res == NULL)
        return -1;
 
    memcpy(addr, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    return 0;
}

static int _net_sock_getaddr_udp (struct sockaddr *addr, const char *host, int port_num)
{
	return _net_sock_getaddr(addr, SOCK_DGRAM, host, port_num);
}
static int _net_sock_getaddr_tcp (struct sockaddr *addr, const char *host, int port_num)
{
	return _net_sock_getaddr(addr, SOCK_STREAM, host, port_num);
}




int im_sock_set_read_timeout(int sock, int sec)
{
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	if (setsockopt (sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		INFO_LOG("fail setsockopt SO_RCVTIMEO, sock=[%d]", sock);

	return 0;
}

int im_sock_set_write_timeout(int sock, int sec)
{
	struct timeval timeout;      
	timeout.tv_sec = sec;
	timeout.tv_usec = 0;

	if (setsockopt (sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
		INFO_LOG("fail setsockopt SO_SNDTIMEO, sock=[%d]", sock);
	
	return 0;
}

int im_sock_set_timeout(int sock, int sec)
{
	im_sock_set_read_timeout(sock, sec);
	im_sock_set_write_timeout(sock, sec);

	return 0;
}

int im_sock_connect_timeout(char *remoteip, int port, int timeout_sec) 
{
	int sock = -1;
	struct sockaddr_in remote_addr;

#ifdef WIN32
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);

	WSADATA wsa_data;

	WSAStartup(wVersionRequested, &wsa_data);
#endif

	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if ( sock < 0 ) {
		ERROR_LOG("fail socket()");
		return -1;
	}

	im_sock_set_timeout(sock, timeout_sec);

    if ( _net_sock_getaddr_tcp((struct sockaddr *)&remote_addr, remoteip, port) < 0 ) {
		ERROR_LOG("fail net_sock_getaddr_tcp(%s:%d)", remoteip, port);
		return -1;
	}

	INFO_LOG("connect(%s:%d)", remoteip, port);
	int rc = connect(sock, (const struct sockaddr *)&remote_addr, sizeof(remote_addr));
	if ( rc ) {
		ERROR_LOG("fail connect(%s:%d)", remoteip, port);
		return -1;
	}

	return sock;
}

int im_sock_connect(char *ip, int port) 
{
	return im_sock_connect_timeout(ip, port, 0);
}


int im_sock_disconnect(int sock)
{
	if ( sock <= 0 )	{
		return -1;
	}

#ifdef WIN32
	closesocket(sock); 
	WSACleanup();
#else
	close(sock);
#endif

	return 0;
}

int im_sock_send(int sock, char *data, int data_len)
{
	if ( sock < 0 || sock > 32767 )	{
		INFO_LOG("invalid sock=[%d]", sock);
		sleep(1);
		return -1;
	}

	int nsend = send(sock, data, data_len, 0);
	if ( nsend < 0 )	{
		if (errno == EWOULDBLOCK || errno == EINTR) {
			return 0;
		} else {
			ERROR_LOG("no data sent; nsend=[%d]", nsend);
			im_base_set_lastErrorCode(IM_ErrCode_SOCK_SEND_FAIL);
			return -1;
		}
	} else if ( nsend == 0 ) {
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_CONNECTION_LOSS);
		ERROR_LOG("remote host may be closed");
		return 0;
	}
	
	return nsend;
}

/* ===============================
 * sock_available
 * > 0 : on read occured
 * = 0 : timeout
 * < 0 : select error
 * =============================== */
int im_sock_available(int sockfd, int timeout_sec)
{
    struct timeval tv;
    fd_set readfds;
    int state;

	if (sockfd < 0) return -1;


	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);

	tv.tv_sec = timeout_sec;
	tv.tv_usec = 0;

	if ( tv.tv_sec <= 0 && tv.tv_usec <= 0) {
		state = select(sockfd+1, &readfds, (fd_set *)0, (fd_set *)0, (struct timeval*)0);
	}else{
		state = select(sockfd+1, &readfds, (fd_set *)0, (fd_set *)0, &tv);
	};

	if ( state < 0 ) {
		/* error on select */
		ERROR_LOG("sock_availavle=[%d]", state);
		perror("sock_availavle():");
		return -1;
	} else 	if ( state == 0 ){
		/* read timeout */
		return 0;
	};

	if ( FD_ISSET(sockfd, &readfds)){
		/* read event may occured */
		return state;
	} else {
		return -1;
	};
}


int im_sock_recv(int sock, char *o_buff, int buff_len)
{
	if ( sock < 0 || sock > 32767 )	{
		INFO_LOG("invalid sock=[%d]", sock);
		sleep(1);
		return -1;
	}

	int nread = recv(sock, o_buff, buff_len, 0);
	if ( nread < 0 )	{
		if (errno == EWOULDBLOCK || errno == EINTR) {
			return 0;
		} else {
			//DEBUG_LOG("no data received; nread=[%d]", nread);
			im_base_set_lastErrorCode(IM_ErrCode_SOCK_RECV_FAIL);
			return -1;
		}
	} else if ( nread == 0 ) {
		ERROR_LOG("remote host may be closed");
		im_base_set_lastErrorCode(IM_ErrCode_SOCK_CONNECTION_LOSS);
		return -1;
	}
	
	o_buff[nread] = '\0';
	return nread;
}

////////////////////////////////////////////////////////////
static char g_buff_recv[IOTMAKERS_MAX_SEND_MSG_LEN];

int im_sock_recv_data(int sock) 
{
	int recv_len;
	int read_timeout = (1);

	if ( im_sock_available(sock, read_timeout) <= 0 )	{
		return 0;
	}

	recv_len = im_sock_recv(sock, g_buff_recv, IOTMAKERS_MAX_SEND_MSG_LEN);
	if ( recv_len <= 0 )	{
		return -1;
	}

	if ( _cb_onMsgarrvd_hndl != NULL )	{
		_cb_onMsgarrvd_hndl(g_buff_recv, recv_len);
	}

	return recv_len;
}

int im_sock_set_nonblocking(int sockfd)
{
	if (sockfd < 0) return -1;
#ifdef WIN32
   unsigned long blocking = 0;
   return (ioctlsocket(sockfd, FIONBIO, &blocking) == SOCKET_ERROR) ? -1 : 0;
#else
   int flags = fcntl(sockfd, F_GETFL, 0);
   if (flags < 0) return -1;
   flags = (flags&~O_NONBLOCK);
   return (fcntl(sockfd, F_SETFL, flags) != 0) ? -1 : 0;
#endif
}

#endif //_IM_C_SOCK_