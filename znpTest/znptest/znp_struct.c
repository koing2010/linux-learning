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

static u8 SYS_CMDProccessing(pTM_CMD Msg);
static u8 MAC_CMDProccessing(pTM_CMD Msg);
static u8 NWK_CMDProccessing(pTM_CMD Msg);
static u8 AF_CMDProccessing(pTM_CMD Msg);
static u8 ZDO_CMDProccessing(pTM_CMD Msg);
static u8 SAPI_CMDProccessing(pTM_CMD Msg);
static u8 UTIL_CMDProccessing(pTM_CMD Msg);
static u8 DEBUG_CMDProccessing(pTM_CMD Msg);
static u8 APP_CMDProccessing(pTM_CMD Msg);

mtProcessMsg_t mtProcessIncoming[] =
  {
	  NULL,
	  SYS_CMDProccessing,
	  MAC_CMDProccessing,
	  NWK_CMDProccessing,
	  AF_CMDProccessing,
	  ZDO_CMDProccessing,
	  SAPI_CMDProccessing,
	  UTIL_CMDProccessing
  };
/*
 * XOR of all the bytes
 */
u8 calcFCS(u8 *pMsg, u8 MsgLen)
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
		if ((*msg & 0xF0) == 0x00)
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
		int re = write(fd, backpMsg, msg_len + 2);
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

u8 ParseCmd(pTM_CMD msg_pkt)
  {
	mtProcessMsg_t ProcessFunc;
	if ((msg_pkt->CMD0 & SUBSYSTEM_MASK) < SYS_MAX)
	  {
		ProcessFunc = mtProcessIncoming[msg_pkt->CMD0 & SUBSYSTEM_MASK];
		if (ProcessFunc)
		  {
			/* execute processing function */
			return (*ProcessFunc)(msg_pkt);
		  }
		else
		  {
			printf("NO Process Function!\n");
		  }
	  }
	return 0;
  }

static u8 SYS_CMDProccessing(pTM_CMD Msg)
  {
	printf("SYS_CMD_INCOMING\n");
	switch (Msg->CMD1)
	  {
	case SYS_PING:
	  printf("SYS_PING RSP\n");
	  return 0xFE;

	  }

	return 0;
  }
static u8 MAC_CMDProccessing(pTM_CMD Msg)
  {
	printf("MAC_CMD_INCOMING\n");
	return 0;
  }
static u8 NWK_CMDProccessing(pTM_CMD Msg)
  {
	printf("NWK_CMD_INCOMING\n");
	return 0;
  }
static u8 AF_CMDProccessing(pTM_CMD Msg)
  {
	printf("AF_CMD_INCOMING\n");
	return 0;
  }
static u8 ZDO_CMDProccessing(pTM_CMD Msg)
  {
	printf("ZDO_CMD_INCOMING\n");
	return 0;
  }
static u8 SAPI_CMDProccessing(pTM_CMD Msg)
  {
	printf("SAPI_CMD_INCOMING\n");
	return 0;
  }
static u8 UTIL_CMDProccessing(pTM_CMD Msg)
  {
	printf("UTIL_CMD_INCOMING\n");
	return 0;
  }
static u8 DEBUG_CMDProccessing(pTM_CMD Msg)
  {
	printf("DEBUG_CMD_INCOMING\n");
	return 0;
  }
static u8 APP_CMDProccessing(pTM_CMD Msg)
  {
	printf("APP_CMD_INCOMING\n");
	return 0;
  }
