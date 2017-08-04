/*
 * znp_struct.h
 *
 *  Created on: Jul 23, 2017
 *      Author: root
 */

#ifndef ZNP_STRUCT_H_
#define ZNP_STRUCT_H_
#include "ktype.h"

#define SUBSYSTEM_MASK      0x1F
#define SYS_MAX             10

typedef struct TM_CMD_
 {
 u8 LEN;
 u8 CMD0;
 u8 CMD1;
 u8 data[];
 } TM_CMD, *pTM_CMD;

 typedef u8 (*mtProcessMsg_t)(pTM_CMD);

void print_msg(u8* msg, u8 lenth);
u8 calcFCS(u8 *pMsg, u8 MsgLen);
u8 znp_dev_pack(u8 *msg, u8 msg_len, int fd);
void ParseCmd(pTM_CMD msg_pkt);
#endif /* ZNP_STRUCT_H_ */
