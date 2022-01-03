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

/* ============================
main_sample2.c

- Sending the complex collection data
	im_init_complexdata();
	im_add_numdata_to_complexdata();
	im_send_complexdata();
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
		rc = im_init_complexdata();
		if ( rc < 0  )	{
			printf("ErrCode[%d]\n", im_get_LastErrCode());
			break;
		}

		rc = im_add_numdata_to_complexdata("Latitude", (double) 35.460670);
		if ( rc < 0  )	{
			printf("ErrCode[%d]\n", im_get_LastErrCode());
			break;
		}

		rc = im_add_numdata_to_complexdata("Longitude", (double) 126.914063);
		if ( rc < 0  )	{
			printf("ErrCode[%d]\n", im_get_LastErrCode());
			break;
		}

		printf("im_send_complexdata()...\n");
		rc = im_send_complexdata(0);
		if ( rc < 0  )	{
			printf("ErrCode[%d]\n", im_get_LastErrCode());
			return -1;
		}

		sleep(5);
	}


	printf("im_stop_service()\n");
	im_stop_service();

	printf("im_release()\n");
	im_release();

	return 0;
}
