#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "iotmakers.h"

static void mycb_dataresp_handler(long long trxid, char *respCode)
{
	printf("trxid=[%lld], respCode=[%s]\n", trxid, respCode);
}

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

/* ============================
main_sample3.c

- Sending the collection data with transaction id
	im_set_dataresp_handler(mycb_dataresp_handler);

============================ */
int main()
{
	int i;
	int rc;
	double val = 0.0;

	set_SigHandler();

	printf("im_init()\n");
	rc = im_init_with_config_file("./config.txt");
	if ( rc < 0  )	{
		printf("fail im_init()\n");
		return -1;
	}

	im_set_loglevel(LOG_LEVEL_DEBUG);

	im_set_dataresp_handler(mycb_dataresp_handler);

	printf("im_start_service()...\n");
	rc = im_start_service();
	if ( rc < 0  )	{
		printf("fail im_start_service()\n");
		im_release();
		return -1;
	}

	local_loop = (1);
	val = 0;
	while (local_loop == (1))
	{
		long long trxid = im_get_new_trxid();
		printf("trxid=[%lld]\n", trxid);
		rc = im_send_numdata("timer", val++, trxid);
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
