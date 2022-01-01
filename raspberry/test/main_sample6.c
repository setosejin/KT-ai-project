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

static int send_device_update_info()
{
	int rc;

	// 1. im_init_if332
	im_init_if332();

	// 2. im_set_if332(devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	im_set_if332_update("testDevName2", NULL, NULL, NULL, NULL, NULL);
	// 3. im_send_if332()
	if ( (rc=im_send_if332(0)) < 0 )	{
		printf("fail im_send_if332() =[%d]\n", rc);
		return -1;
	};

	return 0;
}
static int send_device_update_all_info()
{
	int rc;

	// 1. im_init_if332
	im_init_if332();

	// 2. im_set_if332(devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	im_set_if332_update_all("testNewDevId", "testDevName2", "testMakerCd", "testModelCd", "10.0.0.1", "0001", "1.0.0");
	// 3. im_send_if332()
	if ( (rc=im_send_if332(0)) < 0 )	{
		printf("fail im_send_if332() =[%d]\n", rc);
		return -1;
	};

	return 0;
}

static int send_device_insert_info()
{
	int rc;

	// 1. im_init_if332
	im_init_if332();

	// 2. im_set_if332(devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	im_set_if332_insert("testNewDevId", "testDevName", "testMakerCd", "testModelCd", "10.0.0.1", "0001", "1.0.0");
	// 3. im_send_if332()
	if ( (rc=im_send_if332(0)) < 0 )	{
		printf("fail im_send_if332() =[%d]\n", rc);
		return -1;
	};

	return 0;
}

static int send_device_delete_info()
{
	int rc;

	// 1. im_init_if332
	im_init_if332();

	// 2. im_set_if332(devId, devName, makerCd, modelCd, ipAddr, statCd, frmwrVer);
	im_set_if332_delete("testNewDevId", "testDevName", "testMakerCd", "testModelCd", "10.0.0.1", "0001", "1.0.0");
	// 3. im_send_if332()
	if ( (rc=im_send_if332(0)) < 0 )	{
		printf("fail im_send_if332() =[%d]\n", rc);
		return -1;
	};

	return 0;
}


static void mycb_numdata_handler(char *tagid, double numval)
{
	// !!! USER CODE HERE
	printf("tagid=[%s], val=[%f]\n", tagid, numval);

	int rc = im_send_report_strdata("testReport", "OK");
	// 	int rc = im_send_report_numdata("testReportCode", 100);
	printf("im_send_report_strdata=[%d]\n", rc);

}
static void mycb_strdata_handler(char *tagid, char *strval)
{
	// !!! USER CODE HERE
	printf("tagid=[%s], val=[%s]\n", tagid, strval);

	int rc = im_send_report_strdata("testReport", "OK");
	// 	int rc = im_send_report_numdata("testReportCode", 100);
	printf("im_send_report_strdata=[%d]\n", rc);
}

/* ============================
main_sample4.c

- sending report
	im_send_report_strdata("testReport", "OK");
	im_send_report_numdata("testReportCode", 100);

- sending device_update, IF332
	im_init_if332();
	im_set_if332_insert(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
	im_set_if332_delete(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
	im_set_if332_update(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
	im_set_if332_update_all(char* devId, char* devName, char* makerCd, char* modelCd, char* ipAddr, char* statCd, char* frmwrVer);
	im_send_if332()
	
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
