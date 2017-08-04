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

#define znp_dev_path "/dev/ttyACM2"

static int openSerial(char *cSerialName);
void ParseRxMsg(u8 *Msg, u8 lenth);
void *ListeningUsart(void *serial_fd);

u8 zb_start_req[3] =
 {
 0x00, 0x26, 0x00
 };

SimpleDescriptionFormat_t SimpleDesc =
 {
 ZNP_END_POINT, ZCL_HA_PROFILE_ID, ZCL_HA_DEVICEID_ON_OFF_SWITCH, 0, //app version
   NO_LATENCY,
   0, //in cluster list
   NULL,
   0,
   NULL
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
 open sriral port
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
u8 rxbuf[256];
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

   //sleep(1);
   /* read MAX_READ_SIZE bytes data value from serial port */
   int read_len = read(ser_fd, rxbuf, sizeof(rxbuf));

	if (read_len > 0)
	 {
	 unsigned char i = 0;
	 print_msg(rxbuf, read_len);
	 printf("\nread_len = %d\n", read_len);
	 ParseRxMsg(rxbuf, read_len);
	 }
   }
  //pthread_mutex_unlock(&uart_mutex);

  }
 pthread_exit(NULL);

 }

void ParseRxMsg(u8 *Msg, u8 lenth)
 {
 u8 n = lenth;
 u8 LEN = 0;
 u8 FSC = 0;

//looking for start code
 while (*Msg != 0xFE && n >= 5)
  {
  Msg++;
  n--;
  }

 if (n < 5)
  {
  printf("RxMsg Lenth ERRO!\n");
  return;
  }

 Msg ++;
 LEN = *Msg;//this format data len
 if ((LEN + 3) > n)
  {
  printf("RxMsg format ERRO!\n");
  }
 FSC = Msg[LEN + 3];
 if( FSC == calcFCS(Msg, LEN + 3) )
  {
   ParseCmd( Msg );// force turning format
  }

 }


