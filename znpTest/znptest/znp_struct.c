/*
 * znp_struct.c
 *
 *  Created on: Jul 23, 2017
 *      Author: kon
 */

#include "stdio.h"
#include "stdlib.h"
#include "ktype.h"
#include "znp_struct.h"
#include "string.h"

/*
 * XOR of all the bytes
 */
u8 calcFCS(u8 *pMsg,u8 MsgLen)
 {
 u8 result = 0;
 while (MsgLen--)
  {
  result ^= *pMsg++;
  }
 return result;
 }

/*
 * print the hex message
 */
void print_msg(u8* msg, u8 lenth)
 {
 printf("Hex: ");
 do
  {
  if((*msg & 0xF0) == 0x00)
   {
   printf("0");
   }
   printf("%X ", *msg++);
  }
 while (--lenth);
 printf("\n");
 }
/*
 * pack the msg and write into serial filed
 */
u8 znp_dev_pack(u8 *msg, u8 msg_len, int fd)
 {
 u8 *pMsg = malloc(msg_len + 2); // start code 0xFE end FCS
 u8 *backpMsg = pMsg;
 u8 status = FAILURE;

 //add header and fcs
 *pMsg++ = 0xFE;
 memcpy(pMsg, msg, msg_len);
 pMsg += msg_len;
 *pMsg = calcFCS(msg, msg_len);

 print_msg(backpMsg, msg_len + 2);
 if (backpMsg != NULL)
  {
  int re =  write(fd, backpMsg, msg_len + 2);
  if (re > 0)
   {
   status = SUCCESS;
   printf("znp_dev_pack write SUCCESS!\n");
   }
  else
   {
   printf("znp_dev_pack write FAILURE!\n");
   }
  free(backpMsg);
  }
 return status;
 }

