/*
 * znp_struct.h
 *
 *  Created on: Jul 23, 2017
 *      Author: root
 */

#ifndef ZNP_STRUCT_H_
#define ZNP_STRUCT_H_
#include "ktype.h"

void print_msg(u8* msg, u8 lenth);
u8 calcFCS(u8 *pMsg,u8 MsgLen);
u8 znp_dev_pack(u8 *msg, u8 msg_len, int fd);
#endif /* ZNP_STRUCT_H_ */
