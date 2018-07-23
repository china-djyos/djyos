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
#ifndef __USBH_CUSTOM_H
#define __USBH_CUSTOM_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "../../../core/inc/usbh_core.h"



#define USB_CUSTOM_CLASS                                        0xFF

//
typedef enum
{
    CUSTOM_SEND_DATA = 0,
    CUSTOM_SEND_DATA_WAIT,
    CUSTOM_RECEIVE_DATA,
    CUSTOM_RECEIVE_DATA_WAIT,
}
CUSTOM_DataStateTypeDef;

//
typedef enum
{
    CUSTOM_INIT = 0,
    CUSTOM_IDLE,
    CUSTOM_TRANSFER_AT,
    CUSTOM_TRANSFER_MODEM,
    CUSTOM_ERROR_STATE,
}
CUSTOM_StateTypeDef;

// Structure for CUSTOM interface
typedef struct
{
    uint8_t              InPipe;
    uint8_t              OutPipe;
    uint8_t              OutEp;
    uint8_t              InEp;
    uint16_t             OutEpSize;
    uint16_t             InEpSize;
    uint8_t              IntPipe;
    uint8_t              IntEp;
    uint16_t             IntEpSize;
}
CUSTOM_InterfaceTypedef;

//
// Structure for CUSTOM process
typedef struct _CUSTOM_Process
{
    CUSTOM_InterfaceTypedef         AT_Interface;
    CUSTOM_InterfaceTypedef         MODEM_Interface;
    CUSTOM_InterfaceTypedef         DEBUG_Interface;
    CUSTOM_StateTypeDef             eState;
    void                               *pAT_RxPipe;
    void                               *pAT_TxPipe;
    void                            *pAT_IntPipe;
    void                               *pMODEM_RxPipe;
    void                               *pMODEM_TxPipe;
    void                            *pMODEM_IntPipe;
    void                               *pDEBUG_RxPipe;
    void                               *pDEBUG_TxPipe;
    CUSTOM_DataStateTypeDef         eAT_RxState;
    CUSTOM_DataStateTypeDef         eAT_TxState;
    CUSTOM_DataStateTypeDef         eAT_IntState;
    CUSTOM_DataStateTypeDef         eMODEM_RxState;
    CUSTOM_DataStateTypeDef         eMODEM_TxState;
    CUSTOM_DataStateTypeDef         eMODEM_IntState;
    CUSTOM_DataStateTypeDef         eDEBUG_RxState;
    CUSTOM_DataStateTypeDef         eDEBUG_TxState;
    char                             *pInfos;
    u32                             dwMode; // 0 -- 正常模式；1 -- 睡眠模式；
                                    //（在睡眠模式下，系统可能会进入到STOP模式，因此模块的处理过程不能存在能够进入idle的阻塞）
}
CUSTOM_HandleTypeDef;

//
extern USBH_ClassTypeDef     CUSTOM_Class;
#define USBH_CUSTOM_CLASS    &CUSTOM_Class

USBH_StatusTypeDef  USBH_CUSTOM_AT_Transmit (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

USBH_StatusTypeDef  USBH_CUSTOM_AT_Receive (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

USBH_StatusTypeDef  USBH_CUSTOM_MODEM_Transmit (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

USBH_StatusTypeDef  USBH_CUSTOM_MODEM_Receive (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

USBH_StatusTypeDef  USBH_CUSTOM_DEBUG_Transmit (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

USBH_StatusTypeDef  USBH_CUSTOM_DEBUG_Receive (USBH_HandleTypeDef *pHost,
        uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout);

#ifdef __cplusplus
}
#endif

#endif // __USBH_CUSTOM_H


