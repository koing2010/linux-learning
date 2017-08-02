/*
 * zdo_process.c
 *
 *  Created on: 2017-
 *      Author: kon
 */
#include "stdio.h"
#include "ktype.h"
#include "znp_struct.h"
#include "zdo_proess.h"

/*
 * register
 */
void AF_RegisterAppEndpointDescription(
  SimpleDescriptionFormat_t *pSimpleDesFormat, int serial)
 {
 u8* RegDes = NULL;
 u8* BackRegDes = NULL;
 u8 i = 0;
 u16 *pList = NULL;
 u8 number = 9 + pSimpleDesFormat->AppNumInClusters
   + pSimpleDesFormat->AppNumOutClusters + 3; //3:len + cmd0 + cmd1
 RegDes = malloc(number);
 if (RegDes != NULL)
  BackRegDes = RegDes; //save the start situation
 *RegDes++ = 9 + pSimpleDesFormat->AppNumInClusters
   + pSimpleDesFormat->AppNumOutClusters;
 *RegDes++ = 0x24;
 *RegDes++ = 0x00;
 *RegDes++ = pSimpleDesFormat->EndPoint;
 *RegDes++ = (u8) pSimpleDesFormat->AppProfId;
 *RegDes++ = (u8) (pSimpleDesFormat->AppProfId >> 8);
 *RegDes++ = (u8) pSimpleDesFormat->AppDeviceId;
 *RegDes++ = (u8) (pSimpleDesFormat->AppDeviceId >> 8);
 *RegDes++ = (u8) pSimpleDesFormat->AppDevVer;
 *RegDes++ = (u8) pSimpleDesFormat->LatencyReq;
 *RegDes++ = (u8) pSimpleDesFormat->AppNumInClusters;
 pList = pSimpleDesFormat->pAppInClusterList;
 for (i = 0; i < pSimpleDesFormat->AppNumInClusters; i++)
  {
  *RegDes++ = (u8) *pList;
  *RegDes++ = (u8) (*pList >> 8);
  pList++;
  }
 *RegDes++ = pSimpleDesFormat->AppNumOutClusters;
 pList = pSimpleDesFormat->pAppOutClusterList;
 for (i = 0; i < pSimpleDesFormat->AppNumOutClusters; i++)
  {
  *RegDes++ = (u8) *pList;
  *RegDes++ = (u8) (*pList >> 8);
  pList++;
  }

 print_msg(BackRegDes, number);
 if (znp_dev_pack(BackRegDes, number, serial) == SUCCESS)
  {
  printf("Has Send Register Msg\n");
  }
 else
  {
  printf("Register Erro\n");
  }
 free(BackRegDes);

 }

/*
 * call this fucntion ro register of an imcoming over the air zdo message probably a response message but
 *  requests can also be received
 */
u8 ZDO_RegisterForZDOMsgCB(u16 ClusterID, int serial)
 {
 u8 code[3] =
  {
  0x02, 0x25, 0x3E
  };
 u8 *msg = NULL;
 u8 *BackMsg = NULL;
 u8 reStatus = FAILURE;
 u8 number = sizeof(code) + sizeof(ClusterID);
 msg = malloc(number);
 if (msg != NULL)
  {
  BackMsg = msg;
  memcpy(msg, code, sizeof(code));
  msg += sizeof(code);
  *msg++ = (u8) ClusterID;
  *msg = (u8) (ClusterID >> 8);

  if (znp_dev_pack(BackMsg, number, serial) == SUCCESS)
   {
   reStatus = SUCCESS;
   printf("ZDO %X Cluster Register SUCCESS\n", ClusterID);
   }
  else
   {
   printf("ZDO %X Cluster Register Erro\n", ClusterID);
   }
  free(BackMsg);
  }

 return reStatus;

 }
