#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "iotmakers.h"

static int local_loop = (0);

static void SigHandler(int sig)
{
	switch(sig)
	{
		case SIGTERM :
		case SIGINT :
			printf("accept signal SIGINT[%d]\n", sig);
			im_stop_service();
			local_loop = (0);
			break;
		default :
			;
	};
	return;
} 
static void set_SigHandler()
{
	signal(SIGINT,   SigHandler);	
	signal(SIGTERM,  SigHandler);	
}

static void mycb_error_handler(int lastErrCode)
{
	printf("lastErrCode=[%d]\n", lastErrCode);
	switch ( lastErrCode )
	{
	case 401:	//IM_ErrCode_SOCK_SEND_FAIL
	case 402:	//IM_ErrCode_SOCK_RECV_FAIL
	case 403:	//IM_ErrCode_SOCK_CONNECTION_FAIL
	case 404:	//IM_ErrCode_SOCK_CONNECTION_LOSS
		break;
	}
}



/* ============================
main_sample5.c

- Error event handler
	im_set_error_handler(mycb_error_handler);
============================ */
int main()
{
	int i;
	int rc;

	set_SigHandler();

	printf("im_init()\n");
	rc = im_init_with_config_file("./config.txt");
	if ( rc < 0  )	{
		printf("fail im_init()\n");
		return -1;
	}

	im_set_loglevel(LOG_LEVEL_DEBUG);

	im_set_error_handler(mycb_error_handler);

	printf("im_start_service()...\n");
	rc = im_start_service();
	if ( rc < 0  )	{
		printf("fail im_start_service()\n");
		im_release();
		return -1;
	}

	local_loop = (1);
	while (local_loop == (1)) 
	{
		printf("user looper...\n");

		printf("im_send_strdata()...\n");
		rc = im_send_strdata("switch", "ON", 0);
		if ( rc < 0  )	{
			printf("ErrCode[%d]\n", im_get_LastErrCode());
			break;
		}

		sleep(5);
	}

	printf("im_stop_service()\n");
	im_stop_service();

	printf("im_release()\n");
	im_release();

	return 0;
}
