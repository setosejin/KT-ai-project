#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include "iotmakers.h"

#define LED1 5 // BCM_GPIO 24
#define TC1 4  // BCM_GPIO 23
#define MAXTIMINGS 85
#define DHTPIN 7
int dht11_dat[5] = {0, 0, 0, 0};

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
	// LED on-off ...
	pinMode(LED1, OUTPUT);
	if(!strcmp(tagid,"LED"))
	{
		if(!strcmp(strval,"ON"))
		{
			printf("tagid=[%s], val=[%s]\n", tagid, strval);
			digitalWrite(LED1, 1); // On
		}
		else if(!strcmp(strval,"OFF"))
		{
			printf("tagid=[%s], val=[%s]\n", tagid, strval);
		    digitalWrite(LED1, 0); // Off
		}
	}
}

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
		return -1;
	}

	// check flag 1 보냄
	im_send_numdata("check", 1, 0); 

	printf("im_stop_service()\n");
	im_stop_service();

	printf("im_release()\n");
	im_release();

	return 0;
}
