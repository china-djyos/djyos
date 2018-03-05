//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include "../inc/usbh_custom.h"
#include "../../../../usbh_lowlevel.h"
#include "../../../../usb.h"
#include <lock.h>
#include <systime.h>

//
//
//
#define DEFAULT_WAIT_TIME          50 // 50us
#define NOT_SUPPORT                0xFF


static USBH_StatusTypeDef USBH_CUSTOM_InterfaceInit  (USBH_HandleTypeDef *pHost);

static USBH_StatusTypeDef USBH_CUSTOM_InterfaceDeInit  (USBH_HandleTypeDef *pHost);

static USBH_StatusTypeDef USBH_CUSTOM_Process(USBH_HandleTypeDef *pHost);

static USBH_StatusTypeDef USBH_CUSTOM_SOFProcess(USBH_HandleTypeDef *pHost);

static USBH_StatusTypeDef USBH_CUSTOM_ClassRequest (USBH_HandleTypeDef *pHost);

static USBH_StatusTypeDef __AT_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength);

static USBH_StatusTypeDef __AT_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout);

static USBH_StatusTypeDef __AT_ProcessInt(USBH_HandleTypeDef *pHost, uint8_t *pRxData, uint32_t *pRxDataLength);

static USBH_StatusTypeDef __MODEM_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength);

static USBH_StatusTypeDef __MODEM_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout);

static USBH_StatusTypeDef __MODEM_ProcessInt(USBH_HandleTypeDef *pHost, uint8_t *pRxData, uint32_t *pRxDataLength);

static USBH_StatusTypeDef __DEBUG_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength);

static USBH_StatusTypeDef __DEBUG_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout);

extern void USBH_CUSTOM_AT_TransmitCallback(USBH_HandleTypeDef *pHost);

extern void USBH_CUSTOM_AT_ReceiveCallback(USBH_HandleTypeDef *pHost);

extern void USBH_CUSTOM_AT_IntReceiveCallback(USBH_HandleTypeDef *pHost);

extern void USBH_CUSTOM_MODEM_TransmitCallback(USBH_HandleTypeDef *pHost);

extern void USBH_CUSTOM_MODEM_ReceiveCallback(USBH_HandleTypeDef *pHost);

extern void USBH_CUSTOM_MODEM_IntReceiveCallback(USBH_HandleTypeDef *pHost);

extern u32 USBH_Fetch(u8 bPipe, u8 *pBuffer, u32 dwLen);
extern s32 USBH_OpenAsync(USBH_HandleTypeDef *pHost, u8 bPipe);
extern s32 USBH_CloseAsync(USBH_HandleTypeDef *pHost, u8 bPipe);
extern void USBH_Wait(u8 bPipe, u32 dwTimeout);

USBH_ClassTypeDef  CUSTOM_Class =
{
    "CUSTOM",
    USB_CUSTOM_CLASS,
    USBH_CUSTOM_InterfaceInit,
    USBH_CUSTOM_InterfaceDeInit,
    USBH_CUSTOM_ClassRequest,
    USBH_CUSTOM_Process,
    USBH_CUSTOM_SOFProcess,
    NULL,
};

struct __DeviceInfomation
{
    uint16_t wPID;
    uint16_t wVID;
    uint8_t  bAT;
    uint8_t  bMODEM;
    uint8_t  bDebug;
    char *pDescription;
}
DEVICE_TABLE[] =
{
    {0xF601, 0x05C6, 0x2,  0x1, NOT_SUPPORT, "FORGE, SLM730"},       // FORGE
    {0x9025, 0x05C6, 0x2,  0x1, NOT_SUPPORT, "FORGE, SLM630"},       // FORGE
    {0x9B3C, 0x1C9E, 0x2,  0x1, NOT_SUPPORT, "LOONSONG, u9300C"},    // LONGSONG，do not work
    {0x9B05, 0x1C9E, 0x2,  0x1, 0x0,         "LOONSONG, U8300C"},    // LONGSONG
    {0x15C1, 0x12D1, 0x2,  0x0, NOT_SUPPORT, "HUAWEI, ME909"},       // HUAWEI,modem口未知
    {0x0125, 0x2C7C, 0x2,  0x3, NOT_SUPPORT, "QUECTEL, EC20 V2.0"},  // QUECTEL
    {0x9001, 0x1E0E, 0x2,  0x3, 0x0,         "SIMCOM, 7600C/E"},     // SIMCOM

    {0,      0,      0,      0,  0,          "end of table"},        // end of array
};

// ============================================================================
// 功能：获取custom设备的描述信息
// 参数：
// 返回：
// 备注：
// ============================================================================
static char *__GetDescription(USBH_HandleTypeDef *pHost)
{
    uint8_t i = 0;
    char *ret = NULL;

    for(;;)
    {
        if((pHost->device.DevDesc.idProduct == DEVICE_TABLE[i].wPID) &&
           (pHost->device.DevDesc.idVendor == DEVICE_TABLE[i].wVID))
        {
            ret = DEVICE_TABLE[i].pDescription;
            break;
        }

        if(!strcmp(DEVICE_TABLE[i].pDescription, "end of table"))
            break;
        i++;
    }

    return (ret);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __AT_Interface(USBH_HandleTypeDef *pHost, uint8_t *pInterface)
{
    uint8_t i = 0;

    *pInterface = 0x2; // 默认使用#2, 因为各厂家都是用这个

    for(;;)
    {
        if((pHost->device.DevDesc.idProduct == DEVICE_TABLE[i].wPID) &&
           (pHost->device.DevDesc.idVendor == DEVICE_TABLE[i].wVID))
        {
            *pInterface = DEVICE_TABLE[i].bAT;
            break;
        }

        if(!strcmp(DEVICE_TABLE[i].pDescription, "end of table"))
            break;
        i++;
    }

    if(NOT_SUPPORT == *pInterface)
        return (USBH_NOT_SUPPORTED);

    USBH_SelectInterface (pHost, *pInterface);
    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __DEBUG_Interface(USBH_HandleTypeDef *pHost, uint8_t *pInterface)
{
    uint8_t i = 0;

    *pInterface = 0x2; // 默认使用#2, 因为各厂家都是用这个

    for(;;)
    {
        if((pHost->device.DevDesc.idProduct == DEVICE_TABLE[i].wPID) &&
           (pHost->device.DevDesc.idVendor == DEVICE_TABLE[i].wVID))
        {
            *pInterface = DEVICE_TABLE[i].bDebug;
            break;
        }

        if(!strcmp(DEVICE_TABLE[i].pDescription, "end of table"))
            break;
        i++;
    }

    if(NOT_SUPPORT == *pInterface)
        return (USBH_NOT_SUPPORTED);

    USBH_SelectInterface (pHost, *pInterface);
    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __MODEM_Interface(USBH_HandleTypeDef *pHost, uint8_t *pInterface)
{
    uint8_t i = 0;

    *pInterface = 0x1; // 默认使用#2, 因为很多厂家都是用这个

    for(;;)
    {
        if((pHost->device.DevDesc.idProduct == DEVICE_TABLE[i].wPID) &&
           (pHost->device.DevDesc.idVendor == DEVICE_TABLE[i].wVID))
        {
            *pInterface = DEVICE_TABLE[i].bMODEM;
            break;
        }

        if(!strcmp(DEVICE_TABLE[i].pDescription, "end of table"))
            break;
        i++;
    }

    if(NOT_SUPPORT == *pInterface)
        return (USBH_NOT_SUPPORTED);

    USBH_SelectInterface (pHost, *pInterface);
    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static void *__CreatePipe(char *bName)
{
    return ((void*)Lock_MutexCreate(bName));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static void __DestroyPipe(void *pPipe)
{
    Lock_MutexDelete(pPipe);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __AcquirePipe(void *pPipe, uint32_t *pTimeout)
{
    s64 log;

    log = DjyGetSysTime();
    if(FALSE == Lock_MutexPend(pPipe, *pTimeout))
        return (USBH_BUSY); // timeout

    if(CN_TIMEOUT_FOREVER != *pTimeout)
    {
        log = DjyGetSysTime() - log;

        if(log >= 0 && log < *pTimeout)
            *pTimeout -= log; // time consumed
        else
            *pTimeout = 0; // timeout
    }

    return (USBH_OK);
}

//-----------------------------------------------------------------------------
//功能：
//参数：
//返回：
//备注：
// ============================================================================
static void __ReleasePipe(void *pPipe)
{
    Lock_MutexPost(pPipe);
}

/**
  * @brief  USBH_CUSTOM_InterfaceInit
  *         The function init the CDC class.
  * @param  pHost: Host handle
  * @retval USBH status
  */
static USBH_StatusTypeDef USBH_CUSTOM_InterfaceInit (USBH_HandleTypeDef *pHost)
{    
    uint8_t interfaceIndex, endpoints;
    CUSTOM_HandleTypeDef *chandle;
    CUSTOM_InterfaceTypedef *interface;
    uint8_t  transferType;
    uint8_t i;
    extern void USBH_SetCUSTOM(USBH_HandleTypeDef *pHost);

    pHost->pActiveClass->ClassState = USBH_BUSY;

    if(!pHost->pActiveClass->pData)
    {
        pHost->pActiveClass->pData = (CUSTOM_HandleTypeDef *)
                USBH_malloc (sizeof(CUSTOM_HandleTypeDef));

        if(!pHost->pActiveClass->pData)
            return (USBH_FAIL);

        USBH_memset(pHost->pActiveClass->pData, 0x0, sizeof(CUSTOM_HandleTypeDef));
    }

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    chandle->pInfos = __GetDescription(pHost); // 获取设备信息
    if(USBH_OK == __AT_Interface(pHost, &interfaceIndex))
    {
        endpoints = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].bNumEndpoints;
        interface = &chandle->AT_Interface;

        for(i = 0; i < endpoints; i++)
        {
            transferType = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bmAttributes;
            if(USB_EP_TYPE_BULK == transferType)
            {
                if(pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress & 0x80) // IN
                {
                    interface->InEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->InEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    /*Allocate the length for host channel number in*/
                    chandle->pAT_RxPipe = __CreatePipe("USB in pipe(AT) lock");
                    if(!chandle->pAT_RxPipe)
                        goto FAIL;

                    interface->InPipe = USBH_AllocPipe(pHost, interface->InEp);
                    if(USBH_OpenAsync(pHost, interface->InPipe))
                        goto FAIL;
                    /* Open channel for IN endpoint */
                    USBH_OpenPipe (pHost,
                                  interface->InPipe,
                                  interface->InEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->InEpSize);

                    USBH_LL_SetToggle  (pHost, interface->InPipe,0);
                }
                else // OUT
                {
                    interface->OutEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->OutEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    chandle->pAT_TxPipe = __CreatePipe("USB out pipe(AT) lock");
                    if(!chandle->pAT_TxPipe)
                        goto FAIL;

                    interface->OutPipe = USBH_AllocPipe(pHost, interface->OutEp);

                    USBH_OpenPipe (pHost,
                                  interface->OutPipe,
                                  interface->OutEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->OutEpSize);

                    USBH_LL_SetToggle (pHost, chandle->AT_Interface.OutPipe,0);
                }
            }
            else if(USB_EP_TYPE_INTR == transferType)
            {
                if(pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress & 0x80)
                {
                    interface->IntEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->IntEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                }

                chandle->pAT_IntPipe = __CreatePipe( "USB interrupt pipe(AT) lock");
                if(!chandle->pAT_IntPipe)
                    goto FAIL;

                interface->IntPipe = USBH_AllocPipe(pHost, interface->IntEp);

                USBH_OpenPipe (pHost,
                               interface->IntPipe,
                               interface->IntEp,
                               pHost->device.address,
                               pHost->device.speed,
                               USB_EP_TYPE_INTR,
                               interface->IntEpSize);

                USBH_LL_SetToggle (pHost, interface->IntPipe, 0);
            }
        }
    }

    if(USBH_OK == __MODEM_Interface(pHost, &interfaceIndex))
    {
        endpoints = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].bNumEndpoints;
        interface = &chandle->MODEM_Interface;

        for(i = 0; i < endpoints; i++)
        {
            transferType = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bmAttributes;
            if(USB_EP_TYPE_BULK == transferType)
            {
                if(pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress & 0x80) // IN
                {
                    interface->InEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->InEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    chandle->pMODEM_RxPipe = __CreatePipe("USB in pipe(MODEM) lock");
                    if(!chandle->pMODEM_RxPipe)
                        goto FAIL;

                    interface->InPipe = USBH_AllocPipe(pHost, interface->InEp);
                    if(USBH_OpenAsync(pHost, interface->InPipe))
                        goto FAIL;

                    USBH_OpenPipe (pHost,
                                  interface->InPipe,
                                  interface->InEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->InEpSize);

                    USBH_LL_SetToggle  (pHost, interface->InPipe,0);
                }
                else // OUT
                {
                    interface->OutEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->OutEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    chandle->pMODEM_TxPipe = __CreatePipe("USB out pipe(MODEM) lock");
                    if(!chandle->pMODEM_TxPipe)
                        goto FAIL;

                    interface->OutPipe = USBH_AllocPipe(pHost, interface->OutEp);

                    USBH_OpenPipe (pHost,
                                  interface->OutPipe,
                                  interface->OutEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->OutEpSize);

                    USBH_LL_SetToggle (pHost, chandle->AT_Interface.OutPipe,0);
                }
            }
            else if(USB_EP_TYPE_INTR == transferType)
            {
                if(pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress & 0x80)
                {
                    interface->IntEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->IntEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                }
                else
                {
                    goto FAIL; // unkown end point description
                }

                chandle->pMODEM_IntPipe = __CreatePipe("USB interrupt pipe(MODEM) lock");
                if(!chandle->pMODEM_IntPipe)
                    goto FAIL;

                interface->IntPipe = USBH_AllocPipe(pHost, interface->IntEp);

                /* Open pipe for Notification endpoint */
                USBH_OpenPipe (pHost,
                               interface->IntPipe,
                               interface->IntEp,
                               pHost->device.address,
                               pHost->device.speed,
                               USB_EP_TYPE_INTR,
                               interface->IntEpSize);

                USBH_LL_SetToggle (pHost, interface->IntPipe, 0);
            }
        }
    }

    if(USBH_OK == __DEBUG_Interface(pHost, &interfaceIndex))
    {
        endpoints = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].bNumEndpoints;
        interface = &chandle->DEBUG_Interface;

        for(i = 0; i < endpoints; i++)
        {
            transferType = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bmAttributes;
            if(USB_EP_TYPE_BULK == transferType)
            {
                if(pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress & 0x80) // IN
                {
                    interface->InEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->InEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    /*Allocate the length for host channel number in*/
                    chandle->pDEBUG_RxPipe = __CreatePipe("USB in pipe(DEBUG) lock");
                    if(!chandle->pDEBUG_RxPipe)
                        goto FAIL;

                    interface->InPipe = USBH_AllocPipe(pHost, interface->InEp);
                    if(USBH_OpenAsync(pHost, interface->InPipe))
                        goto FAIL;
                    /* Open channel for IN endpoint */
                    USBH_OpenPipe (pHost,
                                  interface->InPipe,
                                  interface->InEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->InEpSize);

                    USBH_LL_SetToggle  (pHost, interface->InPipe,0);
                }
                else // OUT
                {
                    interface->OutEp = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].bEndpointAddress;
                    interface->OutEpSize  = pHost->device.CfgDesc.Itf_Desc[interfaceIndex].Ep_Desc[i].wMaxPacketSize;
                    chandle->pDEBUG_TxPipe = __CreatePipe("USB out pipe(DEBUG) lock");
                    if(!chandle->pDEBUG_TxPipe)
                        goto FAIL;

                    interface->OutPipe = USBH_AllocPipe(pHost, interface->OutEp);

                    USBH_OpenPipe (pHost,
                                  interface->OutPipe,
                                  interface->OutEp,
                                  pHost->device.address,
                                  pHost->device.speed,
                                  USB_EP_TYPE_BULK,
                                  interface->OutEpSize);

                    USBH_LL_SetToggle (pHost, chandle->AT_Interface.OutPipe,0);
                }
            }
        }
    }

    chandle->eState = CUSTOM_INIT;

    if(USBH_SetHostName(pHost, "custom"))
        goto FAIL;

    USBH_SetCUSTOM(pHost);

    return USBH_OK;

 FAIL:

     USBH_CUSTOM_InterfaceDeInit(pHost);

    return USBH_FAIL;
}



/**
  * @brief  USBH_CUSTOM_InterfaceDeInit
  *         The function DeInit the Pipes used for the CDC class.
  * @param  pHost: Host handle
  * @retval USBH status
  */
USBH_StatusTypeDef USBH_CUSTOM_InterfaceDeInit (USBH_HandleTypeDef *pHost)
{
    CUSTOM_HandleTypeDef *chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    extern void USBH_ResetCUSTOM(USBH_HandleTypeDef *pHost);

    pHost->pActiveClass->ClassState = USBH_NOT_SUPPORTED;

    if(chandle->pAT_IntPipe)
        __DestroyPipe(chandle->pAT_IntPipe);

    if(chandle->pAT_RxPipe)
        __DestroyPipe(chandle->pAT_RxPipe);

    if(chandle->pAT_TxPipe)
        __DestroyPipe(chandle->pAT_TxPipe);

    if(chandle->pMODEM_IntPipe)
        __DestroyPipe(chandle->pMODEM_IntPipe);

    if(chandle->pMODEM_RxPipe)
        __DestroyPipe(chandle->pMODEM_RxPipe);

    if(chandle->pMODEM_TxPipe)
        __DestroyPipe(chandle->pMODEM_TxPipe);

    if(chandle->pDEBUG_RxPipe)
        __DestroyPipe(chandle->pDEBUG_RxPipe);

    if(chandle->pDEBUG_TxPipe)
        __DestroyPipe(chandle->pDEBUG_TxPipe);

    if ( chandle->AT_Interface.IntPipe)
    {
        USBH_ClosePipe(pHost, chandle->AT_Interface.IntPipe);
        USBH_FreePipe  (pHost, chandle->AT_Interface.IntPipe);
        chandle->AT_Interface.IntPipe = 0;     /* Reset the Channel as Free */
    }

    if ( chandle->AT_Interface.InPipe)
    {
        USBH_CloseAsync(pHost, chandle->AT_Interface.InPipe);
        USBH_ClosePipe(pHost, chandle->AT_Interface.InPipe);
        USBH_FreePipe  (pHost, chandle->AT_Interface.InPipe);
        chandle->AT_Interface.InPipe = 0;     /* Reset the Channel as Free */
    }

    if ( chandle->AT_Interface.OutPipe)
    {
        USBH_ClosePipe(pHost, chandle->AT_Interface.OutPipe);
        USBH_FreePipe  (pHost, chandle->AT_Interface.OutPipe);
        chandle->AT_Interface.OutPipe = 0;     /* Reset the Channel as Free */
    }

    if ( chandle->MODEM_Interface.IntPipe)
    {
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.IntPipe);
        USBH_FreePipe  (pHost, chandle->MODEM_Interface.IntPipe);
        chandle->MODEM_Interface.IntPipe = 0;     /* Reset the Channel as Free */
    }

    if ( chandle->MODEM_Interface.InPipe)
    {
        USBH_CloseAsync(pHost, chandle->MODEM_Interface.InPipe);
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.InPipe);
        USBH_FreePipe  (pHost, chandle->MODEM_Interface.InPipe);
        chandle->MODEM_Interface.InPipe = 0;     /* Reset the Channel as Free */
    }

    if ( chandle->MODEM_Interface.OutPipe)
    {
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.OutPipe);
        USBH_FreePipe(pHost, chandle->MODEM_Interface.OutPipe);
        chandle->MODEM_Interface.OutPipe = 0;     /* Reset the Channel as Free */
    }

    if (chandle->DEBUG_Interface.IntPipe)
    {
        USBH_ClosePipe(pHost, chandle->DEBUG_Interface.IntPipe);
        USBH_FreePipe(pHost, chandle->DEBUG_Interface.IntPipe);
        chandle->DEBUG_Interface.IntPipe = 0;     /* Reset the Channel as Free */
    }

    if (chandle->DEBUG_Interface.InPipe)
    {
        USBH_CloseAsync(pHost, chandle->DEBUG_Interface.InPipe);
        USBH_ClosePipe(pHost, chandle->DEBUG_Interface.InPipe);
        USBH_FreePipe(pHost, chandle->DEBUG_Interface.InPipe);
        chandle->DEBUG_Interface.InPipe = 0;     /* Reset the Channel as Free */
    }

    if (chandle->DEBUG_Interface.OutPipe)
    {
        USBH_ClosePipe(pHost, chandle->DEBUG_Interface.OutPipe);
        USBH_FreePipe(pHost, chandle->DEBUG_Interface.OutPipe);
        chandle->DEBUG_Interface.OutPipe = 0;     /* Reset the Channel as Free */
    }

    if(pHost->pActiveClass->pData)
    {
        USBH_free(pHost->pActiveClass->pData);
        pHost->pActiveClass->pData = 0;
    }

    USBH_ResetCUSTOM(pHost);

    return USBH_OK;
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
#define STANDARD_FEATURE_DEVICE_REMOTE_WAKEUP            1
USBH_StatusTypeDef USBH_CUSTOM_ClearRemoteWakeupFeature(USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_STANDARD |
                                               USB_REQ_RECIPIENT_DEVICE;

        pHost->Control.setup.b.bRequest = USB_REQ_CLEAR_FEATURE;
        pHost->Control.setup.b.wValue.w = STANDARD_FEATURE_DEVICE_REMOTE_WAKEUP; //
        pHost->Control.setup.b.wIndex.w = 0;
        pHost->Control.setup.b.wLength.w = 0;
    }

    status = USBH_CtlReq(pHost, NULL, 0);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            USBH_UsrLog("USB #%04x stack service : error : clear remote wake up feature timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

/**
  * @brief  USBH_CUSTOM_ClassRequest
  *         The function is responsible for handling Standard requests
  *         for CDC class.
  * @param  pHost: Host handle
  * @retval USBH status
  */
static USBH_StatusTypeDef USBH_CUSTOM_ClassRequest (USBH_HandleTypeDef *pHost)
{   
    USBH_StatusTypeDef status = USBH_OK;

    // 去除远程唤醒功能

    while(1)
    {
        status = USBH_CUSTOM_ClearRemoteWakeupFeature(pHost);
        if((USBH_TIMEOUT == status) || (USBH_OK == status))
            break;
    }

    return (USBH_OK);
}


/**
  * @brief  USBH_CUSTOM_Process
  *         The function is for managing state machine for CDC data transfers 
  * @param  pHost: Host handle
  * @retval USBH status
  */
static USBH_StatusTypeDef USBH_CUSTOM_Process (USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef status = USBH_BUSY;
    CUSTOM_HandleTypeDef *chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;

    switch (chandle->eState)
    {
    case CUSTOM_INIT:
        pHost->pActiveClass->ClassState = USBH_OK;
        chandle->eState = CUSTOM_IDLE;
        break;

    case CUSTOM_IDLE:
        Djy_EventDelay(1000000); // 1s跑一次
        break;
    default:
        break;
    // TODO: to safeguard the custom class to work more stable
    }
    return status;
}

/**
  * @brief  USBH_CUSTOM_SOFProcess
  *         The function is for managing SOF callback 
  * @param  pHost: Host handle
  * @retval USBH status
  */
static USBH_StatusTypeDef USBH_CUSTOM_SOFProcess (USBH_HandleTypeDef *pHost)
{
    return USBH_OK;
}
                                   
  
/**
  * @brief  USBH_CDC_Stop 
  *         Stop current CDC Transmission 
  * @param  pHost: Host handle
  * @retval USBH status
  */
USBH_StatusTypeDef  USBH_CUSTOM_Stop (USBH_HandleTypeDef *pHost)
{
  CUSTOM_HandleTypeDef *chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
  
  if(pHost->gState == HOST_CLASS)
  {
    chandle->eState = CUSTOM_IDLE;

    if(chandle->AT_Interface.IntPipe)
        USBH_ClosePipe(pHost, chandle->AT_Interface.IntPipe);

    if(chandle->AT_Interface.InPipe)
        USBH_ClosePipe(pHost, chandle->AT_Interface.InPipe);

    if(chandle->AT_Interface.OutPipe)
        USBH_ClosePipe(pHost, chandle->AT_Interface.OutPipe);

    if(chandle->MODEM_Interface.IntPipe)
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.IntPipe);

    if(chandle->MODEM_Interface.InPipe)
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.InPipe);

    if(chandle->MODEM_Interface.OutPipe)
        USBH_ClosePipe(pHost, chandle->MODEM_Interface.OutPipe);
  }

  return USBH_OK;  
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_AT_Transmit (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pAT_TxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eAT_TxState = CUSTOM_SEND_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __AT_ProcessTransmission(pHost, &pBuff, pLength);
                if(USBH_OK == status)
                    break; // done
                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pAT_TxPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_AT_Receive (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pAT_RxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eAT_RxState = CUSTOM_RECEIVE_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __AT_ProcessReception(pHost, &pBuff, pLength, dwTimeout);
                if(USBH_OK == status)
                    break; // done

                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout

            }
        }
        __ReleasePipe(chandle->pAT_RxPipe);
    }

    return status;
} 

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_AT_Int (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pAT_IntPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eAT_IntState = CUSTOM_RECEIVE_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __AT_ProcessInt(pHost, pBuff, pLength);
                if(USBH_OK == status)
                    break; // done
                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pAT_IntPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_MODEM_Transmit (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pMODEM_TxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eMODEM_TxState = CUSTOM_SEND_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __MODEM_ProcessTransmission(pHost, &pBuff, pLength);
                if(USBH_OK == status)
                    break; // done
                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pMODEM_TxPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_MODEM_Receive (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pMODEM_RxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eMODEM_RxState = CUSTOM_RECEIVE_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __MODEM_ProcessReception(pHost, &pBuff, pLength, dwTimeout);
                if(USBH_OK == status)
                    break; // done

                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pMODEM_RxPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_MODEM_Int (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pMODEM_IntPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eMODEM_IntState = CUSTOM_RECEIVE_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __MODEM_ProcessInt(pHost, pBuff, pLength);
                if(USBH_OK == status)
                    break; // done
                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pMODEM_IntPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_DEBUG_Transmit (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pDEBUG_TxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eDEBUG_TxState = CUSTOM_SEND_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __DEBUG_ProcessTransmission(pHost, &pBuff, pLength);
                if(USBH_OK == status)
                    break; // done
                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout
            }
        }
        __ReleasePipe(chandle->pDEBUG_TxPipe);
    }

    return status;
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
USBH_StatusTypeDef  USBH_CUSTOM_DEBUG_Receive (USBH_HandleTypeDef *pHost, uint8_t *pBuff, uint32_t *pLength, uint32_t dwTimeout)
{
    USBH_StatusTypeDef status;
    CUSTOM_HandleTypeDef *chandle;
    s64 log;

    if(!pHost || !pHost->pActiveClass || !pHost->pActiveClass->pData)
        return (USBH_NOT_SUPPORTED);

    if(dwTimeout < DEFAULT_WAIT_TIME)
        dwTimeout = DEFAULT_WAIT_TIME;

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    status = __AcquirePipe(chandle->pDEBUG_RxPipe, &dwTimeout);
    if(USBH_BUSY != status)
    {
        status = USBH_BUSY;
        if(0 < dwTimeout)
        {
            chandle->eDEBUG_RxState = CUSTOM_RECEIVE_DATA;
            log = DjyGetSysTime();
            while(1)
            {
                status = __DEBUG_ProcessReception(pHost, &pBuff, pLength, dwTimeout);
                if(USBH_OK == status)
                    break; // done

                if((CN_TIMEOUT_FOREVER != dwTimeout) && ((uint32_t)(DjyGetSysTime() - log) > dwTimeout))
                    break; // timeout

            }
        }
        __ReleasePipe(chandle->pDEBUG_RxPipe);
    }

    return status;
}

// ============================================================================
//功能： The function is responsible for sending data to the device
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __AT_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    u8 zeroPacket = 0;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eAT_TxState;

    switch(*state)
    {

    case CUSTOM_SEND_DATA:
        if(*pTxDataLength > chandle->AT_Interface.OutEpSize)
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               chandle->AT_Interface.OutEpSize,
                               chandle->AT_Interface.OutPipe,
                               1);
        }
        else
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               *pTxDataLength,
                               chandle->AT_Interface.OutPipe,
                               1);
        }

        if(chandle->AT_Interface.OutEpSize == *pTxDataLength)
            zeroPacket = 1;

        *state = CUSTOM_SEND_DATA_WAIT;

        break;

    case CUSTOM_SEND_DATA_WAIT:

        status = USBH_LL_GetURBState(pHost, chandle->AT_Interface.OutPipe);
        /*Check the status done for transmission*/
        if(status == USBH_URB_DONE )
        {
            if(*pTxDataLength > chandle->AT_Interface.OutEpSize)
            {
                *pTxDataLength -= chandle->AT_Interface.OutEpSize ;
                *pTxData += chandle->AT_Interface.OutEpSize;
            }
            else
            {
                *pTxDataLength = 0;
            }

            if(( *pTxDataLength > 0) || (zeroPacket))
            {
                *state = CUSTOM_SEND_DATA; // 数据未发送完，继续发送
                zeroPacket = 0;
            }
            else
            {
                USBH_CUSTOM_AT_TransmitCallback(pHost);
                res = USBH_OK;
            }
        }
        else if( status == USBH_URB_NOTREADY )
        {
            *state = CUSTOM_SEND_DATA; // URB未准备好，重新发送
        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能： This function responsible for reception of data from the device
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __AT_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    uint32_t length, rlen;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eAT_RxState;

    switch(*state)
    {
    case CUSTOM_RECEIVE_DATA:

        // 先读取异步缓冲中的数据
        rlen = USBH_Fetch(chandle->AT_Interface.InPipe, *pRxData, *pRxDataLength);
        if(0 == (*pRxDataLength -= rlen))
            return (USBH_OK); //

        *pRxData += rlen;
        USBH_BulkReceiveData (pHost,
                              *pRxData,
                              chandle->AT_Interface.InEpSize,
                              chandle->AT_Interface.InPipe);

        *state = CUSTOM_RECEIVE_DATA_WAIT;
        break;

    case CUSTOM_RECEIVE_DATA_WAIT:

        if(!chandle->dwMode)
        {
            USBH_Wait(chandle->AT_Interface.InPipe, dwTimeout);
        }
        else
        {
            USBH_Wait(chandle->AT_Interface.InPipe, 0); // 睡眠模式下会一直查询，直至超时
        }
        status = USBH_LL_GetURBState(pHost, chandle->AT_Interface.InPipe);
        /*Check the status done for reception*/
        if(status == USBH_URB_DONE )
        {
            length = USBH_LL_GetLastXferSize(pHost, chandle->AT_Interface.InPipe);
            rlen = USBH_Fetch(chandle->AT_Interface.InPipe, *pRxData, *pRxDataLength);
            if(rlen > length)
            {
                USBH_UsrLog("\r\ncustom at channel bug\r\n"); // 正常逻辑不进入,因为此时都是同步
            }
            // if(((*pRxDataLength - length) > 0) && (length > chandle->AT_Interface.InEpSize)) // TODO，第二个条件不知道什么意思
            if(*pRxDataLength > length)
            {
                *pRxDataLength -= length ;
                *pRxData += length;
                *state = CUSTOM_RECEIVE_DATA; // 数据未接收完，继续接收
            }
            else
            {
                if(rlen < length)
                    *pRxDataLength -= rlen; // 接收到的比要求的多
                else
                    *pRxDataLength -= length;
                USBH_CUSTOM_AT_ReceiveCallback(pHost);
                res = USBH_OK;
            }

        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __AT_ProcessInt(USBH_HandleTypeDef *pHost, uint8_t *pRxData, uint32_t *pRxDataLength)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    uint16_t length;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eAT_IntState;

    switch(*state)
    {

    case CUSTOM_RECEIVE_DATA:

        USBH_InterruptReceiveData(pHost,
                                  pRxData,
                                  chandle->AT_Interface.IntEpSize,
                                  chandle->AT_Interface.IntPipe);

        *state = CUSTOM_RECEIVE_DATA_WAIT;

        break;

    case CUSTOM_RECEIVE_DATA_WAIT:

        status = USBH_LL_GetURBState(pHost, chandle->AT_Interface.IntPipe);

        /*Check the status done for reception*/
        if(status == USBH_URB_DONE )
        {
            length = USBH_LL_GetLastXferSize(pHost, chandle->AT_Interface.IntPipe);

            if(((*pRxDataLength - length) > 0) && (length > chandle->AT_Interface.IntEpSize))
            {
                *pRxDataLength -= length ;
                pRxData += length;
                *state = CUSTOM_RECEIVE_DATA; // 数据未接收完，继续接收
            }
            else
            {
                *pRxDataLength -= length ;
                USBH_CUSTOM_AT_IntReceiveCallback(pHost);
                res = USBH_OK;
            }
        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __MODEM_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    uint8_t zeroPacket = 0;

    if(!pHost->pActiveClass)
            return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eMODEM_TxState;

    switch(*state)
    {

    case CUSTOM_SEND_DATA:
        if(*pTxDataLength > chandle->MODEM_Interface.OutEpSize)
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               chandle->MODEM_Interface.OutEpSize,
                               chandle->MODEM_Interface.OutPipe,
                               1);
        }
        else
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               *pTxDataLength,
                               chandle->MODEM_Interface.OutPipe,
                               1);
        }

        if(chandle->MODEM_Interface.OutEpSize == zeroPacket)
            zeroPacket = 1;

        *state = CUSTOM_SEND_DATA_WAIT;

        break;

    case CUSTOM_SEND_DATA_WAIT:

        status = USBH_LL_GetURBState(pHost, chandle->MODEM_Interface.OutPipe);
        /*Check the status done for transmission*/
        if(status == USBH_URB_DONE )
        {
            if(*pTxDataLength > chandle->MODEM_Interface.OutEpSize)
            {
                *pTxDataLength -= chandle->MODEM_Interface.OutEpSize ;
                *pTxData += chandle->MODEM_Interface.OutEpSize;
            }
            else
            {
                *pTxDataLength = 0;
            }

            if((*pTxDataLength > 0) || (zeroPacket))
            {
                *state = CUSTOM_SEND_DATA; // 数据未发送完，继续发送
                zeroPacket = 0;
            }
            else
            {
                USBH_CUSTOM_MODEM_TransmitCallback(pHost);
                res = USBH_OK;
            }
        }
        else if(status == USBH_URB_NOTREADY )
        {
            *state = CUSTOM_SEND_DATA; // URB未准备好，重新发送
        }
        break;

    default:
        break;
    }

    return (res);

}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __MODEM_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout)
{
    uint32_t length, rlen;
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eMODEM_RxState;

    switch(*state)
    {
    case CUSTOM_RECEIVE_DATA:

        // 先读取异步缓冲中的数据
        rlen = USBH_Fetch(chandle->MODEM_Interface.InPipe, *pRxData, *pRxDataLength);
        if(0 == (*pRxDataLength -= rlen))
            return (USBH_OK); //

        *pRxData += rlen;
        USBH_BulkReceiveData (pHost,
                              *pRxData,
                              chandle->MODEM_Interface.InEpSize,
                              chandle->MODEM_Interface.InPipe);

        *state = CUSTOM_RECEIVE_DATA_WAIT;
        break;

    case CUSTOM_RECEIVE_DATA_WAIT:

        if(!chandle->dwMode)
        {
            USBH_Wait(chandle->MODEM_Interface.InPipe, dwTimeout);
        }
        else
        {
            USBH_Wait(chandle->MODEM_Interface.InPipe, 0);
        }
        status = USBH_LL_GetURBState(pHost, chandle->MODEM_Interface.InPipe);
        /*Check the status done for reception*/
        if(status == USBH_URB_DONE )
        {
            length = USBH_LL_GetLastXferSize(pHost, chandle->MODEM_Interface.InPipe);
            rlen = USBH_Fetch(chandle->MODEM_Interface.InPipe, *pRxData, *pRxDataLength);
            if(rlen > length)
            {
                USBH_UsrLog("\r\ncustom modem channel bug\r\n"); // 正常逻辑不进入,因为此时都是同步
            }
            // if(((*pRxDataLength - length) > 0) && (length > chandle->MODEM_Interface.InEpSize)) // TODO，第二个条件不知道什么意思
            if(*pRxDataLength > length)
            {
                *pRxDataLength -= length ;
                *pRxData += length;
                *state = CUSTOM_RECEIVE_DATA; // 数据未接收完，继续接收
            }
            else
            {
                if(rlen < length)
                    *pRxDataLength -= rlen; // 接收到的比要求的多
                else
                    *pRxDataLength -= length;
                USBH_CUSTOM_MODEM_ReceiveCallback(pHost);
                res = USBH_OK;
            }

        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能：
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __MODEM_ProcessInt(USBH_HandleTypeDef *pHost, uint8_t *pRxData, uint32_t *pRxDataLength)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    uint16_t length;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eMODEM_IntState;

    switch(*state)
    {

    case CUSTOM_RECEIVE_DATA:

        USBH_InterruptReceiveData(pHost,
                                  pRxData,
                                  chandle->MODEM_Interface.IntEpSize,
                                  chandle->MODEM_Interface.IntPipe);

        *state = CUSTOM_RECEIVE_DATA_WAIT;

        break;

    case CUSTOM_RECEIVE_DATA_WAIT:

        status = USBH_LL_GetURBState(pHost, chandle->MODEM_Interface.IntPipe);

        /*Check the status done for reception*/
        if(status == USBH_URB_DONE )
        {
            length = USBH_LL_GetLastXferSize(pHost, chandle->MODEM_Interface.IntPipe);

            if(((*pRxDataLength - length) > 0) && (length > chandle->MODEM_Interface.IntEpSize))
            {
                *pRxDataLength -= length ;
                pRxData += length;
                *state = CUSTOM_RECEIVE_DATA; // continue to receive
            }
            else
            {
                *pRxDataLength -= length ;
                USBH_CUSTOM_MODEM_IntReceiveCallback(pHost);
                res = USBH_OK;
            }
        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能： The function is responsible for sending data to the device
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __DEBUG_ProcessTransmission(USBH_HandleTypeDef *pHost, uint8_t **pTxData, uint32_t *pTxDataLength)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    u8 zeroPacket = 0;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eDEBUG_TxState;

    switch(*state)
    {

    case CUSTOM_SEND_DATA:
        if(*pTxDataLength > chandle->DEBUG_Interface.OutEpSize)
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               chandle->DEBUG_Interface.OutEpSize,
                               chandle->DEBUG_Interface.OutPipe,
                               1);
        }
        else
        {
            USBH_BulkSendData (pHost,
                               *pTxData,
                               *pTxDataLength,
                               chandle->DEBUG_Interface.OutPipe,
                               1);
        }

        if(chandle->DEBUG_Interface.OutEpSize == *pTxDataLength)
            zeroPacket = 1;

        *state = CUSTOM_SEND_DATA_WAIT;

        break;

    case CUSTOM_SEND_DATA_WAIT:

        status = USBH_LL_GetURBState(pHost, chandle->DEBUG_Interface.OutPipe);
        /*Check the status done for transmission*/
        if(status == USBH_URB_DONE )
        {
            if(*pTxDataLength > chandle->DEBUG_Interface.OutEpSize)
            {
                *pTxDataLength -= chandle->DEBUG_Interface.OutEpSize ;
                *pTxData += chandle->DEBUG_Interface.OutEpSize;
            }
            else
            {
                *pTxDataLength = 0;
            }

            if((*pTxDataLength > 0) || (zeroPacket))
            {
                *state = CUSTOM_SEND_DATA; // 数据未发送完，继续发送
                zeroPacket = 0;
            }
            else
            {
                res = USBH_OK;
            }
        }
        else if( status == USBH_URB_NOTREADY )
        {
            *state = CUSTOM_SEND_DATA; // URB未准备好，重新发送
        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能： This function responsible for reception of data from the device
//参数：
//返回：
//备注：
// ============================================================================
static USBH_StatusTypeDef __DEBUG_ProcessReception(USBH_HandleTypeDef *pHost, uint8_t **pRxData, uint32_t *pRxDataLength, uint32_t dwTimeout)
{
    CUSTOM_HandleTypeDef *chandle;
    USBH_URBStateTypeDef status = USBH_URB_IDLE;
    CUSTOM_DataStateTypeDef *state;
    USBH_StatusTypeDef res = USBH_BUSY;
    uint32_t length, rlen;

    if(!pHost->pActiveClass)
        return (res);

    chandle =  (CUSTOM_HandleTypeDef*) pHost->pActiveClass->pData;
    state = &chandle->eDEBUG_RxState;

    switch(*state)
    {
    case CUSTOM_RECEIVE_DATA:

        // 先读取异步缓冲中的数据
        rlen = USBH_Fetch(chandle->DEBUG_Interface.InPipe, *pRxData, *pRxDataLength);
        if(0 == (*pRxDataLength -= rlen))
            return (USBH_OK); //

        *pRxData += rlen;
        USBH_BulkReceiveData (pHost,
                              *pRxData,
                              chandle->DEBUG_Interface.InEpSize,
                              chandle->DEBUG_Interface.InPipe);

        *state = CUSTOM_RECEIVE_DATA_WAIT;
        break;

    case CUSTOM_RECEIVE_DATA_WAIT:

        if(!chandle->dwMode)
        {
            USBH_Wait(chandle->DEBUG_Interface.InPipe, dwTimeout);
        }
        else
        {
            USBH_Wait(chandle->DEBUG_Interface.InPipe, 0);
        }
        status = USBH_LL_GetURBState(pHost, chandle->DEBUG_Interface.InPipe);
        /*Check the status done for reception*/
        if(status == USBH_URB_DONE )
        {
            length = USBH_LL_GetLastXferSize(pHost, chandle->DEBUG_Interface.InPipe);
            rlen = USBH_Fetch(chandle->DEBUG_Interface.InPipe, *pRxData, *pRxDataLength);
            if(rlen > length)
            {
                USBH_UsrLog("\r\ncustom at channel bug\r\n"); // 正常逻辑不进入,因为此时都是同步
            }
            
            // if(((*pRxDataLength - length) > 0) && (length > chandle->AT_Interface.InEpSize)) // TODO，第二个条件不知道什么意思
            if(*pRxDataLength > length)
            {
                *pRxDataLength -= length ;
                *pRxData += length;
                *state = CUSTOM_RECEIVE_DATA; // 数据未接收完，继续接收
            }
            else
            {
                if(rlen < length)
                    *pRxDataLength -= rlen; // 接收到的比要求的多
                else
                    *pRxDataLength -= length;

                res = USBH_OK;
            }

        }
        break;

    default:
        break;
    }

    return (res);
}

// ============================================================================
//功能： The function informs user that data have been sent
//参数：
//返回：
//备注：
// ============================================================================
void __attribute((weak)) USBH_CUSTOM_AT_TransmitCallback(USBH_HandleTypeDef *pHost)
{
;
}
  
// ============================================================================
//功能： The function informs user that data have been received
//参数：
//返回：
//备注：
// ============================================================================
void __attribute((weak)) USBH_CUSTOM_AT_ReceiveCallback(USBH_HandleTypeDef *pHost)
{
;
}

// ============================================================================
//功能： The function informs user that data have been received
//参数：
//返回：
//备注：
// ============================================================================
void __attribute__((weak)) USBH_CUSTOM_AT_IntReceiveCallback(USBH_HandleTypeDef *pHost)
{
;
}

// ============================================================================
//功能： The function informs user that data have been sent
//参数：
//返回：
//备注：
// ============================================================================
void __attribute__((weak)) USBH_CUSTOM_MODEM_TransmitCallback(USBH_HandleTypeDef *pHost)
{
;
}

// ============================================================================
//功能： The function informs user that data have been received
//参数：
//返回：
//备注：
// ============================================================================
void __attribute__((weak)) USBH_CUSTOM_MODEM_ReceiveCallback(USBH_HandleTypeDef *pHost)
{
;
}

// ============================================================================
//功能： The function informs user that data have been received
//参数：
//返回：
//备注：
// ============================================================================
void __attribute__((weak)) USBH_CUSTOM_MODEM_IntReceiveCallback(USBH_HandleTypeDef *pHost)
{
;
}

