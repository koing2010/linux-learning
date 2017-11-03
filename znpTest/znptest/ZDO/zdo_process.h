/*
 * zdo_prcoess.h
 *
 *  Created on: Jul 24, 2017
 *      Author: kon
 */

#ifndef ZDO_PROCESS_H_
#define ZDO_PROCESS_H_
#include "ktype.h"

// ZDO Cluster IDs

#define ZDO_RESPONSE_BIT        ((u16)0x8000)

#define Simple_Desc_req         ((u16)0x0004)
#define Active_EP_req           ((u16)0x0005)
#define Match_Desc_req          ((u16)0x0006)
#define Simple_Desc_rsp         (Simple_Desc_req | ZDO_RESPONSE_BIT)
#define Active_EP_rsp           (Active_EP_req | ZDO_RESPONSE_BIT)
#define Match_Desc_rsp          (Match_Desc_req | ZDO_RESPONSE_BIT)

#define Device_annce            ((u16)0x0013)



typedef u16  cId_t;
// Simple Description Format Structure
typedef struct
{
  u8          EndPoint;
  u16         AppProfId;
  u16         AppDeviceId;
  u8          AppDevVer;
  u8          LatencyReq;
  u8          AppNumInClusters;
  cId_t         *pAppInClusterList;
  u8          AppNumOutClusters;
  cId_t         *pAppOutClusterList;
} SimpleDescriptionFormat_t;

#define  NO_LATENCY             0x00
#define  FAST_BEACONS           0x01
#define  SLOW_BEACONS           0x02
//0x00-No latency
//0x01-fast beacons
//0x02-slow beacons

u8 AF_RegisterAppEndpointDescription(SimpleDescriptionFormat_t *pSimpleDesFormat, int serial);
u8 ZDO_ActiveEPReq(u16 DstAddr, u16 NWKAddrOfInterest, int serial);
u8 ZDO_RegisterForZDOMsgCB(u16 ClusterID, int serial);
#endif /* ZDO_PROESS_H_ */
