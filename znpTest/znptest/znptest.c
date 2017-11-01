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
#include "time.h"

#define znp_dev_path "/dev/ttyACM3"

static int openSerial(char *cSerialName);
void *ListeningUsart(void *serial_fd);

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
	int status;
	//void *th_return;

	int s_fd = openSerial(znp_dev_path);
	if (s_fd > 0)
	  {
		/** create thread 1 **/
		status = pthread_create(&thread, NULL, ListeningUsart, (void*) s_fd);
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
u8 state;
u8 CMD_Token[2];
u8 LEN_Token;
u8 FSC_Token;
u8 tempDataLen;
u8 ch;
u8* pBuf = NULL;
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

			whlie(1 == read(ser_fd, ch, 1))
			  {
				switch (ch)
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
					  state = LEN_STATE;
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
				  int bytesInRxBuffer = read(ser_fd, &pBuf[MT_RPC_FRAME_HDR_SZ + tempDataLen],
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
					  ParseCmd(pBuf);
					}

				  /* free the pBuf */
				  free(pBuf);

				  /* Reset the state, send or discard the buffers at this point */
				  state = SOP_STATE;

				  break;

				default:
				  break;
				  }
			  }

		  }
		//pthread_mutex_unlock(&uart_mutex);

	  }
	pthread_exit(NULL);

  }

