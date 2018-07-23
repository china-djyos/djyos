/**
  ******************************************************************************
  * @file    usbh_cdc.h
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contains all the prototypes for the usbh_cdc.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_HUB_H
#define __USBH_HUB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../../../core/inc/usbh_core.h"



#define USB_HUB_CLASS                                       0x09

#define HUB_GET_STATUS                                      0x0
#define HUB_CLEAR_FEATURE                                   0x1
#define HUB_SET_FEATURE                                     0x3
#define HUB_GET_DESCRIPTOR                                  0x6
#define HUB_SET_DESCRIPTOR                                  0x7
#define HUB_CLEAR_TT_BUFFER                                 0x8
#define HUB_RESET_TT                                        0x9
#define HUB_GET_TT_STATE                                    0xA
#define HUB_STOP_TT                                         0xB

#define C_HUB_LOCAL_POWER                                   0
#define C_HUB_OVER_CURRENT                                  1
#define PORT_CONNECTION                                     0
#define PORT_ENABLE                                         1
#define PORT_SUSPEND                                        2
#define PORT_OVER_CURRENT                                   3
#define PORT_RESET                                          4
#define PORT_POWER                                          8
#define PORT_LOW_SPEED                                      9
#define C_PORT_CONNECTION                                   16
#define C_PORT_ENABLE                                       17
#define C_PORT_SUSPEND                                      18
#define C_PORT_OVER_CURRENT                                 19
#define C_PORT_RESET                                        20
#define PORT_TEST                                           21
#define PORT_INDICATOR                                      22

//
typedef struct _HUB_Description{
    u8 bLen;
    u8 bType;
    u8 bPorts;
    u16 wCharacteristics;
    u8 bPwrOn2PwrGood;
    u8 bContrCurrent;
    u8 bData[258];
}tagDescHUB;

//
typedef union _PortStatus{
    u32    dw;
    struct {
        u32 connect:1;
        u32 enabled:1;
        u32 suspend:1;
        u32 overCurrent:1;
        u32 reset:1;
        u32 reserved0:3;
        u32 power:1;
        u32 lowDevice:1;
        u32 highDevice:1;
        u32 test:1;
        u32 indicator:1;
        u32 reserved1:3;
        u32 connectionChange:1; // port change
        u32 enableChange:1;
        u32 suspendChange:1;
        u32 overCurrentChange:1;
        u32 resetChange:1;
        u32 reserved2:11;
    }bit;
}tagPortStatus;

//
typedef union _HubStatus{
    u32    dw;
    struct {
        u32 localPower:1;
        u32 overCurrent:1;
        u32 reserved0:14;
        u32 localPowerChange:1;
        u32 overCurrentChange:1;
        u32 reserved1:14;
    }bit;
}tagHubStatus;

//
 typedef enum _HUB_ProcessState
 {
     HUB_DEBUG,
     HUB_INIT,
     HUB_IDLE,
     HUB_PORT_SCAN,
     HUB_PORT_CHANGE,
     HUB_PORT_ABORT,
 }
 tagHUB_State;

 //
typedef enum _HUB_PortStatus
{
    NOCHANGE,
    CONNECT,
    DISCONNECT,
}
tagHUB_PortStatus;

//
typedef enum _HUB_ChangeReqState
{
    REQ,
    REQ_WAIT,
    REQ_ERROR
}
tagHUB_ChangeReqState;

//
typedef struct _HUB_StatusChange
{
    u8 bPipe;
    u8 bEp;
    u16 wEpSize;
    tagHUB_ChangeReqState state;
    u8 *pStatus;
    u8 bBytes;
    u8 bLen;
    u8 *pBuf;
    tagHUB_PortStatus status;
}
tagHUB_StatusChange;

// Structure for HUB process
typedef struct _HUB_Process
{
    tagHUB_State state;
    u8 bPorts;
    tagHUB_StatusChange change;
    CMD_StateTypeDef  requestState;
    USBH_HandleTypeDef **pPort; //
}
tagHUB_Handle;

//
extern USBH_ClassTypeDef            HUB_Class;
#define USBH_HUB_CLASS              &HUB_Class

USBH_StatusTypeDef USBH_HUB_ClearHubFeature(USBH_HandleTypeDef *pHost, u16 wFeature);
USBH_StatusTypeDef USBH_HUB_SetHubFeature(USBH_HandleTypeDef *pHost, u16 wFeature);
USBH_StatusTypeDef USBH_HUB_GetHubStatus(USBH_HandleTypeDef *pHost, tagHubStatus *pState);
USBH_StatusTypeDef USBH_HUB_GetPortStatus(USBH_HandleTypeDef *pHost, u16 wPort, tagPortStatus *pStatus);
USBH_StatusTypeDef HUB_StatusChange(USBH_HandleTypeDef *pHost, u8 *pBuf, u8 bLen);
USBH_StatusTypeDef USBH_HUB_ClearPortFeature(USBH_HandleTypeDef *pHost, u16 wPort, u16 wFeature);
USBH_StatusTypeDef USBH_HUB_SetPortFeature(USBH_HandleTypeDef *pHost, u16 wPort, u16 wFeature);
USBH_StatusTypeDef USBH_HUB_GetDescriptor(USBH_HandleTypeDef *pHost, u8 *pBuf, u8 bLen);

#ifdef __cplusplus
}
#endif

#endif // __USBH_HUB_H


