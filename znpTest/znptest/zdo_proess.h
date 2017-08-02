/*
 * zdo_proess.h
 *
 *  Created on: Jul 24, 2017
 *      Author: kon
 */

#ifndef ZDO_PROESS_H_
#define ZDO_PROESS_H_
#include "ktype.h"


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

void AF_RegisterAppEndpointDescription(SimpleDescriptionFormat_t *pSimpleDesFormat, int serial);

u8 ZDO_RegisterForZDOMsgCB(u16 ClusterID, int serial);
#endif /* ZDO_PROESS_H_ */
