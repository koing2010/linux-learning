/*
 * znptest.h
 *
 *  Created on: Jul 24, 2017
 *      Author: kon
 */

#ifndef ZNPTEST_H_
#define ZNPTEST_H_

#define ZNP_END_POINT                                           0x01
#define ZNP_DEVICE_VERSION                                      0
#define ZCL_HA_PROFILE_ID                                       0x0104



// Generic Device IDs
#define ZCL_HA_DEVICEID_ON_OFF_SWITCH                           0x0000
#define ZCL_HA_DEVICEID_LEVEL_CONTROL_SWITCH                    0x0001
#define ZCL_HA_DEVICEID_ON_OFF_OUTPUT                           0x0002
#define ZCL_HA_DEVICEID_LEVEL_CONTROLLABLE_OUTPUT               0x0003
#define ZCL_HA_DEVICEID_SCENE_SELECTOR                          0x0004


// ZDO Cluster IDs

#define ZDO_RESPONSE_BIT        ((u16)0x8000)

#define Simple_Desc_req         ((u16)0x0004)
#define Active_EP_req           ((u16)0x0005)
#define Match_Desc_req          ((u16)0x0006)
#define Simple_Desc_rsp         (Simple_Desc_req | ZDO_RESPONSE_BIT)
#define Active_EP_rsp           (Active_EP_req | ZDO_RESPONSE_BIT)
#define Match_Desc_rsp          (Match_Desc_req | ZDO_RESPONSE_BIT)

#define Device_annce            ((u16)0x0013)

#endif /* ZNPTEST_H_ */
