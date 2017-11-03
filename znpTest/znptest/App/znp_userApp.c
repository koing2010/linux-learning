/*
 * znp_userset.c
 *
 *  Created on: Nov 2, 2017
 *      Author: kon
 */

#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <termios.h>
#include "ktype.h"
#include "znp_struct.h"
#include "string.h"

#include "time.h"
#include "znp_userApp.h"
#include "zdo_process.h"
#include "zcl.h"

//start device
u8 zb_start_req[3] =
  {
	0x00, 0x26, 0x00
  };

SimpleDescriptionFormat_t SimpleDesc =
  {
	ZNP_END_POINT, ZCL_HA_PROFILE_ID, ZCL_HA_DEVICEID_ON_OFF_SWITCH, 0, //app version
	NO_LATENCY, 0, //in cluster list
	NULL, 0, NULL
  };

/*
 * fl : serial port file number
 * return: the send status
 */
u8 UerRegister(int fl)
  {
	u8 status = FAILURE;
	printf("device open success!\n");

	//send start znp device requesting
	if ((status = znp_dev_pack(zb_start_req, sizeof(zb_start_req), fl)) != SUCCESS)
	  {
		return status;
	  }
	sleep(1);

	if ((status = ZDO_RegisterForZDOMsgCB(Match_Desc_req, fl)) != SUCCESS)
	  {
		return status;
	  }
	sleep(1);
	if ((status = AF_RegisterAppEndpointDescription(&SimpleDesc, fl)) != SUCCESS)
	  {
		return status;
	  }

	return status;
  }
