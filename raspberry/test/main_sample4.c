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

static void mycb_numdata_handler(char *tagid, double numval)
{
	// !!! USER CODE HERE
	printf("tagid=[%s], val=[%f]\n", tagid, numval);
}
static void mycb_strdata_handler(char *tagid, char *strval)
{
	// !!! USER CODE HERE
	printf("tagid=[%s], val=[%s]\n", tagid, strval);
}

/* ============================
main_sample4.c

- Handling a control data
	im_set_numdata_handler(mycb_numdata_handler);
	im_set_strdata_handler(mycb_strdata_handler);
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

	im_set_numdata_handler(mycb_numdata_handler);
	im_set_strdata_handler(mycb_strdata_handler);

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
		printf("waiting control data...\n");
		sleep(1);
	}

	printf("im_stop_service()\n");
	im_stop_service();

	printf("im_release()\n");
	im_release();

	return 0;
}
