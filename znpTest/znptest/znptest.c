/*
 * znptest.c
 *
 *  Created on: Jul 23, 2017
 *      Author: kon
 */

#include "stdio.h"
#include "stdlib.h"
#include <fcntl.h>
#include <termios.h>
#include "pthread.h"
#include "ktype.h"
#include "zdo_proess.h"
#include "znptest.h"
#include "znp_struct.h"
#include "time.h"
#include "string.h"

unsigned char znp_dev_path[] = "/dev/ttyACM";

static int openSerial(char *cSerialName);
void *ListeningUsart(void *serial_fd);
u8 PaserUartdata(int serial_fd);


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

int main(void)
  {
	pthread_t thread;

	unsigned char ser_cnt = 0;
	unsigned char *s_path = NULL;
	int s_fd = 0; //serial path
	//void *th_return;
	//locking the serial port which is cached the ZNP device

	s_path = malloc(sizeof(znp_dev_path) + 3);
	memcpy(s_path, znp_dev_path, sizeof(znp_dev_path));
	u8 Find = 0;
	for (ser_cnt = 1; ser_cnt < 20; ser_cnt++)
	  {
		memset(&s_path[sizeof(znp_dev_path)], 0, 3);
		sprintf ( &s_path[sizeof(znp_dev_path)-1], "%d", ser_cnt);//-1 means 0x00 of string

		printf("try open serial port path = %s\n", s_path);
		s_fd = openSerial(s_path);
		if (s_fd > 0)
		  {
			//send polling
			u8 sys_ping[3] =
			  {
			  0x00, 0x21, 0x01
			  };
			if (znp_dev_pack(sys_ping, sizeof(sys_ping), s_fd) != SUCCESS)
			  {
				continue;
			  }
			sleep(1);

			/* parse the data in rxbufer */
			u8 i = 0;
			for ( i = 0; i < 20; i++)//try for 20 times
			  {
				if (0xFE == PaserUartdata(s_fd)) // sys_ping parser function return 0xFE
				  {
					Find = 1;
					break;
				  }
				else if(i == 20)// if do not find response . this COM not be the target device.
				  {
					close(s_fd);
				  }
			  }

			if(Find)//have find the device
			  {
				break;//break for(ser_cnt.....
			  }

		  }
		else
		  {
			continue;//check net serial file.
		  }
	  } //end for(ser_cnt.....


	if(Find == 0)//do not find the serial port
	  {
		printf("***** check device failed *******!\n")
		return 0;
	  }


	if (s_fd > 0)
	  {
		/** create thread 1 **/
		pthread_create(&thread, NULL, ListeningUsart, (void*) s_fd);
	  }

	if (s_fd > 0)
	  {
		printf("device open success!\n");
		znp_dev_pack(zb_start_req, sizeof(zb_start_req), s_fd);
		sleep(1);
		ZDO_RegisterForZDOMsgCB(Match_Desc_req, s_fd);
		sleep(1);
		AF_RegisterAppEndpointDescription(&SimpleDesc, s_fd);
	  }

	printf("znptest processor start !\n");
	pthread_join(thread, NULL);

	close(s_fd);
	return 0;
  }

/*****************************************************************************
 open serial port
 return: the file of port
 *****************************************************************************/
static int openSerial(char *cSerialName)
  {
	int iFd;

	struct termios opt;

	iFd = open(cSerialName, O_RDWR | O_NOCTTY);
	if (iFd < 0)
	  {
		perror(cSerialName);
		return -1;
	  }

	tcgetattr(iFd, &opt);
	//cfsetispeed(&opt, B57600);
	//cfsetospeed(&opt, B57600);

	cfsetispeed(&opt, B115200);
	cfsetospeed(&opt, B115200);

	/*
	 * raw mode
	 */
	opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	opt.c_oflag &= ~(OPOST);
	opt.c_cflag &= ~(CSIZE | PARENB);
	opt.c_cflag |= CS8;

	/*
	 * 'DATA_LEN' bytes can be read by serial
	 */
	opt.c_cc[VMIN] = 1; //
	opt.c_cc[VTIME] = 1; //wait time for reading once

	if (tcsetattr(iFd, TCSANOW, &opt) < 0)
	  {
		return -1;
	  }

	return iFd;
  }
/*
 * ListeningUsart
 */
/* 1st byte is the length of the data field, 2nd/3rd bytes are command field. */
#define MT_RPC_FRAME_HDR_SZ   3

/* Start-of-frame delimiter for UART transport */
#define MT_UART_SOF                     0xFE

/* UART frame overhead for SOF and FCS bytes */
#define MT_UART_FRAME_OVHD              2
/* State values for ZTool protocal */
#define SOP_STATE      0x00
#define CMD_STATE1     0x01
#define CMD_STATE2     0x02
#define LEN_STATE      0x03
#define DATA_STATE     0x04
#define FCS_STATE      0x05

/* ZTool protocal parameters */
u8 state = LEN_STATE;
u8 CMD_Token[2];
u8 LEN_Token;
u8 FSC_Token;
u8 tempDataLen;
u8 ch;
u8* pBuf = NULL;
u8 PaserUartdata(int serial_fd)
  {
	u8 status = 0;
	while(1 == read(serial_fd, &ch, 1))
	  {
		switch (state)
		  {
		case SOP_STATE:
		  if (ch == MT_UART_SOF)
			state = LEN_STATE;
		  break;

		case LEN_STATE:
		  LEN_Token = ch;
		  tempDataLen = 0;
		  pBuf = malloc(LEN_Token + MT_RPC_FRAME_HDR_SZ);
		  if (pBuf)
			{
			  pBuf[0] = LEN_Token;
			  state = CMD_STATE1;
			}
		  else
			{
			  state = SOP_STATE;
			}
		  break;
		case CMD_STATE1:
		  pBuf[1] = ch;
		  state = CMD_STATE2;
		  break;

		case CMD_STATE2:
		  pBuf[2] = ch;
		  /* If there is no data, skip to FCS state */
		  if (LEN_Token)
			{
			  state = DATA_STATE;
			}
		  else
			{
			  state = FCS_STATE;
			}
		  break;
		case DATA_STATE:
		  /* Fill in the buffer the first byte of the data */
		  pBuf[MT_RPC_FRAME_HDR_SZ + tempDataLen++] = ch;

		  /* Check number of bytes left in the Rx buffer */
		  int bytesInRxBuffer = read(serial_fd, &pBuf[MT_RPC_FRAME_HDR_SZ + tempDataLen],
			  LEN_Token - tempDataLen);
		  if (bytesInRxBuffer > 0)
			{
			  tempDataLen += bytesInRxBuffer;
			}

		  /* If number of bytes read is equal to data length, time to move on to FCS */
		  if (tempDataLen == LEN_Token)
			state = FCS_STATE;
		  break;

		case FCS_STATE:

		  FSC_Token = ch;

		  /* Make sure it's correct */
		  if (FSC_Token == calcFCS(pBuf, MT_RPC_FRAME_HDR_SZ + LEN_Token))
			{
			  status = ParseCmd( (pTM_CMD)pBuf );
			}

		  /* free the pBuf */
		  free(pBuf);

		  /* Reset the state, send or discard the buffers at this point */
		  state = SOP_STATE;

		  return status;

		default:
		  break;
		  }
	  }
	return status;
  }

void *ListeningUsart(void *serial_fd)
  {
	int ser_fd = (long) serial_fd;
	printf("thread start");
	fd_set readfds;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000; //100ms
	/**whe**/

	printf("thread_file_id= %d\n", ser_fd);
	while (1)
	  {
		//pthread_mutex_lock(&uart_mutex);
		FD_ZERO(&readfds);
		FD_SET(ser_fd, &readfds);
		int ret = select(ser_fd + 1, &readfds, NULL, NULL, &tv);
		if (ret > 0)
		  {
			PaserUartdata(ser_fd);
		  }
		//pthread_mutex_unlock(&uart_mutex);

	  }
	pthread_exit(NULL);

  }

