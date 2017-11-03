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
/*
 * This interface allows the tester to interact with the Application Framework layer (AF)
 */
//MT_AF Commands
#define AF_REGISTER      0x00
#define AF_DATA_REQUEST  0x01 //This command is used by the tester to build and send a message through AF layer.
//MT_AF Callbacks
#define AF_DATA_CONFIRM     0x80
#define AF_REFLECT_ERROR    0x83
#define AF_INCOMING_MSG     0x81


/*
 * This interface allows the tester to interact with the target at system level such as reset, read/write
 * memory, read/write extended address…etc.
 */
//MT_SYS Commands
#define SYS_RESET_REQ    0x00
#define SYS_PING         0x01
#define SYS_VERSION      0x02
#define SYS_SET_EXTADDR  0x03
#define SYS_GET_EXTADDR  0x04


/*
 * This interface allows the tester to issue commands to the ZDO layer in the target and receive
 * responses. Each of these messages has a corresponding message that is returned by the target. The
 * response message only indicates that the command message was received and executed. The result of
 * the command execution will be conveyed to the tester via a callback message interface.
 */
#define ZDO_NWK_ADDR_REQ      0x00
#define ZDO_IEEE_ADDR_REQ     0x01
#define ZDO_NODE_DESC_REQ     0x02
#define ZDO_POWER_DESC_REQ    0x03
#define ZDO_SIMPLE_DESC_REQ   0x04
#define ZDO_ACTIVE_EP_REQ     0x05
#define ZDO_MATCH_DESC_REQ    0x06
#define ZDO_COMPLEX_DESC_REQ  0x07
#define ZDO_USER_DESC_REQ     0x08
#define ZDO_END_DEVICE_ANNCE  0x0A
#define ZDO_USER_DESC_SET     0x0B

#define ZDO_MGMT_LQI_REQ              0x31
#define ZDO_MGMT_LEAVE_REQ            0x34
#define ZDO_MGMT_PERMIT_JOIN_REQ      0x36
#define ZDO_MGMT_NWK_UPDATE_REQ       0x37
#define ZDO_MSG_CB_REGISTER           0x3E
#define ZDO_MSG_CB_REMOVE             0x3F

/*
 *MT_ZDO Callbacks
 */
#define ZDO_NWK_ADDR_RSP          0x80
#define ZDO_IEEE_ADDR_RSP         0x81
#define ZDO_NODE_DESC_RSP         0x82
#define ZDO_SIMPLE_DESC_RSP       0x84
#define ZDO_ACTIVE_EP_RSP         0x85
#define ZDO_MATCH_DESC_RSP        0x86
#define ZDO_USER_DESC_RSP         0x87
#define ZDO_MGMT_PERMIT_JOIN_RSP  0xB6

#define ZDO_STATE_CHANGE_IND      0xC0
#define ZDO_END_DEVICE_ANNCE_IND  0xC1
#define ZDO_LEAVE_IND             0xC9
#define ZDO_MSG_CB_INCOMING       0xFF


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
u8 ParseCmd(pTM_CMD msg_pkt);
#endif /* ZNP_STRUCT_H_ */
