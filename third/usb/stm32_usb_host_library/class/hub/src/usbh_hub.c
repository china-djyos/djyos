//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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

//
#include "../../../core/inc/usbh_ctlreq.h"
#include "../inc/usbh_hub.h"
#include "../../../../usbh_lowlevel.h"
#include "../../../../usb.h"
#include <lock.h>
#include <systime.h>
#include <djyos.h>
//
//
//
static USBH_StatusTypeDef __HUB_InterfaceInit(USBH_HandleTypeDef *pHost);
static USBH_StatusTypeDef __HUB_InterfaceDeInit(USBH_HandleTypeDef *pHost);
static USBH_StatusTypeDef __HUB_Process(USBH_HandleTypeDef *pHost);
static USBH_StatusTypeDef __HUB_SOFProcess(USBH_HandleTypeDef *pHost);
static USBH_StatusTypeDef __HUB_ClassRequest(USBH_HandleTypeDef *pHost);
static USBH_StatusTypeDef __HUB_Init(USBH_HandleTypeDef *pHost);

u32  USBH_HUB_Service(void);
USBH_StatusTypeDef USBH_HUB_DeviceRemoved(USBH_HandleTypeDef *pHost, u8 bPort);
USBH_StatusTypeDef USBH_HUB_DeviceConnected(USBH_HandleTypeDef *pHost, u8 bPort);


USBH_ClassTypeDef  HUB_Class =
{
  "HUB",
  USB_HUB_CLASS,
  __HUB_InterfaceInit,
  __HUB_InterfaceDeInit,
  __HUB_ClassRequest,
  __HUB_Process,
  __HUB_SOFProcess,
  NULL,
};

u16 gHubThreatID;
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __HUB_InterfaceInit (USBH_HandleTypeDef *pHost)
{
    uint8_t interface;
    tagHUB_Handle *hub;

    pHost->pActiveClass->ClassState = USBH_BUSY;
    interface = USBH_FindInterface(pHost, 0x9, 0, 0);
    if(interface == 0xFF) // No Valid Interface
    {
        USBH_DbgLog ("Cannot Find the interface for Communication Interface Class %s.", pHost->pActiveClass->Name);
        return (USBH_FAIL);
    }

    USBH_SelectInterface (pHost, interface);

    if(!pHost->pActiveClass->pData)
    {
        pHost->pActiveClass->pData = (tagHUB_Handle *)USBH_malloc (sizeof(*hub));
        if(!pHost->pActiveClass->pData)
            return (USBH_FAIL);

        USBH_memset(pHost->pActiveClass->pData, 0x0, sizeof(*hub));
    }

    hub = (tagHUB_Handle*) pHost->pActiveClass->pData;

    if(pHost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress & 0x80)
    {
         hub->change.bEp = pHost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].bEndpointAddress;
         hub->change.wEpSize  = pHost->device.CfgDesc.Itf_Desc[interface].Ep_Desc[0].wMaxPacketSize;
    }

     // Allocate the length for host channel number in
     hub->change.bPipe = USBH_AllocPipe(pHost, hub->change.bEp);

     // Open pipe for Notification endpoint
     USBH_OpenPipe (pHost,
                    hub->change.bPipe,
                    hub->change.bEp,
                    pHost->device.address,
                    pHost->device.speed,
                    USB_EP_TYPE_INTR,
                    hub->change.wEpSize);

     USBH_LL_SetToggle (pHost, hub->change.bPipe, 0);

     hub->requestState = CMD_SEND;

     gHubThreatID = DJY_EvttRegist(EN_INDEPENDENCE, 200, 0, 4,
                          USBH_HUB_Service, NULL, 0x800, "USB HUB service"); //

     hub->state = HUB_INIT;

     if(!USBH_SetHostName(pHost, "hub"))
         return USBH_OK;

     __HUB_InterfaceDeInit(pHost);

     return USBH_FAIL;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef __HUB_InterfaceDeInit (USBH_HandleTypeDef *pHost)
{
    tagHUB_Handle *hub =  (tagHUB_Handle*) pHost->pActiveClass->pData;
    u8 i;

    pHost->pActiveClass->ClassState = USBH_NOT_SUPPORTED;

    if(hub->change.bPipe)
    {
        USBH_ClosePipe(pHost, hub->change.bPipe);
        USBH_FreePipe(pHost, hub->change.bPipe);
        hub->change.bPipe = 0;
    }

    // 复位各PORT，移除各PORT的线程
    for(i = 1; i < hub->bPorts; i++)
    {
        if(hub->pPort[i - 1])
        {
            USBH_HUB_DeviceRemoved(pHost, i);
        }
    }

    if(pHost->pActiveClass->pData)
    {
        USBH_free(hub->pPort);
        USBH_free(pHost->pActiveClass->pData);
        pHost->pActiveClass->pData = NULL;
    }

    DJY_EvttUnregist(gHubThreatID); // 注销HUB维护线程

    return USBH_OK;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __HUB_ClassRequest (USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef status = USBH_OK;

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_HandleChange(USBH_HandleTypeDef *pHost)
{
    u8 i;
    tagPortStatus status;
    USBH_StatusTypeDef res;
    tagHUB_Handle *hub =  (tagHUB_Handle*) pHost->pActiveClass->pData;
    u32 retry = 0;

    for(i = 1; i <= hub->bPorts; i++)
    {
        if((*hub->change.pStatus >> i) & (0x1))
        {
            while(1)
            {
                res = USBH_HUB_GetPortStatus(pHost, i, &status); // 读取端口状态
                if((USBH_OK == res))
                {
                    printf("\r\nUSB HUB : debug : port <%d> status %04xH", i, status.dw);
                    break;
                }
                else if(USBH_TIMEOUT == res)
                {
                    printf("\r\nUSB HUB : debug : get port <%d> status timeout", i);
                    return (USBH_FAIL);
                }
            }

            if(status.bit.connect) // 新设备接入
            {
                while(1)
                {
                    res = USBH_HUB_SetPortFeature(pHost, i, PORT_RESET); // 使能端口
                    if((USBH_OK == res))
                    {
                        break;
                    }
                    else if(USBH_TIMEOUT == res)
                    {
                        printf("\r\nUSB HUB : debug : set port <%d> \"RESET\" timeout", i);
                        return (USBH_FAIL);
                    }
                }

                retry = 1000;
                while(1)
                {
                    while(1)
                    {
                        res = USBH_HUB_GetPortStatus(pHost, i, &status);
                        if((USBH_OK == res))
                        {
                            break;
                        }
                        else if(USBH_TIMEOUT == res)
                        {
                            if(retry)
                                break;

                            printf("\r\nUSB HUB : debug : get port <%d> status timeout after reset", i);
                            return (USBH_FAIL);
                        }
                    }

                    if(!status.bit.reset)
                        break; // 等待RESET完成

                    DJY_EventDelay(100000); // 等待100ms
                    retry--;
                }

                if(status.bit.enabled)
                {
                    if(status.bit.resetChange)
                    {
                        while(1)
                        {
                            res = USBH_HUB_ClearPortFeature(pHost, i, C_PORT_RESET);
                            if((USBH_OK == res))
                            {
                                break;
                            }
                            else if(USBH_TIMEOUT == res)
                            {
                                printf("\r\nUSB HUB : debug : clear port <%d> \"C_PORT_RESET\" timeout", i);
                                return (USBH_FAIL);
                            }
                        }
                    }
                    else
                    {
                        printf("\r\nHUB : debug : uncovered logic."); // 不正常逻辑
                    }

                    res = USBH_HUB_DeviceConnected(pHost, i);
                    if(USBH_OK == res)
                    {
                        if(status.bit.connectionChange)
                        {
                            while(1)
                            {
                                res = USBH_HUB_ClearPortFeature(pHost, i, C_PORT_CONNECTION);
                                if((USBH_OK == res))
                                {
                                    break;
                                }
                                else if(USBH_TIMEOUT == res)
                                {
                                    printf("\r\nUSB HUB : debug : clear port <%d> \"C_PORT_CONNECTION\" timeout", i);
                                    return (USBH_FAIL);
                                }
                            }
                        }
                    }

                    return (USBH_OK);
                }
            }
            else // 设备拔出
            {
                if(status.bit.enableChange)
                {
                    while(1)
                    {
                        res = USBH_HUB_ClearPortFeature(pHost, i, C_PORT_ENABLE);
                        if((USBH_OK == res))
                        {
                            break;
                        }
                        else if(USBH_TIMEOUT == res)
                        {
                            printf("\r\nUSB HUB : debug : clear port <%d> \"C_PORT_ENABLE\" timeout", i);
                            break; // 直接让线程退出
                        }
                    }
                }
                // 线程退出
                USBH_HUB_DeviceRemoved(pHost, i);

                // 清port的链接状态改变
                if(status.bit.connectionChange)
                {
                    // 各port先掉电，原因防止HUB初始时就是上电，会造成设备的状态不对，从而导致访问设备出错
                    while(1)
                    {
                        res = USBH_HUB_ClearPortFeature(pHost, i, PORT_POWER);
                        if(USBH_OK == res)
                        {
                            break;
                        }
                        else if(USBH_TIMEOUT == res)
                        {
                            printf("\r\nUSB HUB : debug : clear port <%d> \"PORT_POWER\" timeout", i);
                            return (USBH_FAIL);
                        }
                    }

                    DJY_EventDelay(10000000); // 10秒等待，电容放电等
                    // 各port上电
                    while(1)
                    {
                        res = USBH_HUB_SetPortFeature(pHost, i, PORT_POWER);
                        if(USBH_OK == res)
                        {
                            break;
                        }
                        else if(USBH_TIMEOUT == res)
                        {
                            printf("\r\nUSB HUB : debug : set port <%d> \"PORT_POWER\" timeout", i);
                            return (USBH_FAIL);
                        }
                    }

                    while(1)
                    {
                        res = USBH_HUB_ClearPortFeature(pHost, i, C_PORT_CONNECTION);
                        if((USBH_OK == res))
                        {
                            break;
                        }
                        else if(USBH_TIMEOUT == res)
                        {
                            printf("\r\nUSB HUB : debug : clear port <%d> \"C_PORT_CONNECTION\" timeout", i);
                            return (USBH_FAIL);
                        }
                    }
                }

                return (USBH_OK);
            }
        }
    }

    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __HUB_Process (USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef ret = USBH_BUSY;
    tagHUB_Handle *hub =  (tagHUB_Handle*) pHost->pActiveClass->pData;
    u8 res;

    switch (hub->state)
    {
    case HUB_DEBUG:
        hub->state = HUB_DEBUG;
        break;

    case HUB_INIT:
        if(USBH_FAIL == __HUB_Init(pHost))
        {
            printf("\r\n: warn : usbs%02x : hub init failed.", pHost->id);
            break;
        }

        printf("\r\n: info : usbs%02x : hub init success.", pHost->id);
        pHost->pActiveClass->ClassState = USBH_OK;
        hub->state = HUB_PORT_SCAN;
        break;

    case HUB_IDLE:
        DJY_EventDelay(1000000); // 1s周期扫描一次HUB各端口的状态
        hub->state = HUB_PORT_SCAN;
        break;

    case HUB_PORT_SCAN:
        while(1)
        {
            DJY_EventDelay(200);
            res = HUB_StatusChange(pHost, hub->change.pStatus, hub->change.bBytes); // 获取HUB端口状态,判断端口是否发生了变化
            if((USBH_OK == res) && (hub->change.pStatus))
            {
                printf("\r\n: info : usbs%02x : hub's status changed(%xH).", pHost->id, *(hub->change.pStatus));
                hub->state = HUB_PORT_CHANGE;
                break;
            }

            if(USBH_TIMEOUT == res)
            {
                hub->state = HUB_IDLE;
                break;
            }
        }
        break;

    case HUB_PORT_CHANGE:
        res = USBH_HUB_HandleChange(pHost); // 处理port的状态变化
        if(USBH_OK == res)
        {
            hub->state = HUB_IDLE;
        }

        if(USBH_FAIL == res)
        {
            hub->state = HUB_IDLE;
//            hub->state = HUB_PORT_ABORT;
        }
        break;

    case HUB_PORT_ABORT:
    {
        break; // TODO
    }

    default:
        break;
    }

    return ret;
}

// ============================================================================
// 功能：更新端口设备的Timer计数
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __HUB_SOFProcess (USBH_HandleTypeDef *pHost)
{
    u8 i;
    USBH_HandleTypeDef *port;
    tagHUB_Handle *hub = (tagHUB_Handle*) pHost->pActiveClass->pData;

    if(HUB_INIT >= hub->state)
        return (USBH_OK); // hub尚未初始化

    for(i = 0; i < hub->bPorts; i++)
    {
        port = hub->pPort[i];
        if(!port)
            continue;

        if(port->processSOF)
            port->processSOF(port);
        else
            port->Timer ++;
    }

    return (USBH_OK); //
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_GetDescriptor(USBH_HandleTypeDef *pHost, u8 *pBuf, u8 bLen)
{
    USBH_StatusTypeDef  status;

    if(pHost->RequestState == CMD_SEND)
    {
        pHost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_DEVICE;

        pHost->Control.setup.b.bRequest = HUB_GET_DESCRIPTOR;
        pHost->Control.setup.b.wValue.w = 0x2900;
        pHost->Control.setup.b.wIndex.w = 0;
        pHost->Control.setup.b.wLength.w = bLen;
    }

    status = USBH_CtlReq(pHost, pBuf, bLen);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            printf("\r\n: warn : usbs%02x : get hub descriptor timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_GetHubStatus(USBH_HandleTypeDef *pHost, tagHubStatus *pState)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_DEVICE;

        pHost->Control.setup.b.bRequest = HUB_GET_STATUS;
        pHost->Control.setup.b.wValue.w = 0; //
        pHost->Control.setup.b.wIndex.w = 0;
        pHost->Control.setup.b.wLength.w = 4;
    }

    status = USBH_CtlReq(pHost, (u8*)pState, 4);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            // USBH_UsrLog("USB #%xh stack: get hub status timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_GetPortStatus(USBH_HandleTypeDef *pHost, u16 wPort, tagPortStatus *pStatus)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_D2H | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_OTHER;

        pHost->Control.setup.b.bRequest = HUB_GET_STATUS;
        pHost->Control.setup.b.wValue.w = 0; //
        pHost->Control.setup.b.wIndex.w = wPort;
        pHost->Control.setup.b.wLength.w = 4;
    }

    status = USBH_CtlReq(pHost, (u8*)pStatus, 4);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            // USBH_UsrLog("USB #%xh stack: get port status timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_SetPortFeature(USBH_HandleTypeDef *pHost, u16 wPort, u16 wFeature)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_OTHER;

        pHost->Control.setup.b.bRequest = HUB_SET_FEATURE;
        pHost->Control.setup.b.wValue.w = wFeature; //
        pHost->Control.setup.b.wIndex.w = wPort;
        pHost->Control.setup.b.wLength.w = 0;
    }

    status = USBH_CtlReq(pHost, NULL, 0);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            // USBH_UsrLog("USB #%xh stack: get port status timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_ClearPortFeature(USBH_HandleTypeDef *pHost, u16 wPort, u16 wFeature)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_OTHER;

        pHost->Control.setup.b.bRequest = HUB_CLEAR_FEATURE;
        pHost->Control.setup.b.wValue.w = wFeature; //
        pHost->Control.setup.b.wIndex.w = wPort;
        pHost->Control.setup.b.wLength.w = 0;
    }

    status = USBH_CtlReq(pHost, NULL, 0);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 10000)
        {
            pHost->dwTimeout = 0;
            pHost->RequestState = CMD_SEND;
            printf("\r\n: warn : usbs%02x : clear port feature timeout.", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_SetHubFeature(USBH_HandleTypeDef *pHost, u16 wFeature)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_DEVICE;

        pHost->Control.setup.b.bRequest = HUB_SET_FEATURE;
        pHost->Control.setup.b.wValue.w = wFeature; //
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
            // USBH_UsrLog("USB #%xh stack: get port status timeout", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_ClearHubFeature(USBH_HandleTypeDef *pHost, u16 wFeature)
{
    USBH_StatusTypeDef  status;

    if(CMD_SEND == pHost->RequestState)
    {
        pHost->Control.setup.b.bmRequestType = USB_H2D | USB_REQ_TYPE_CLASS |
                                               USB_REQ_RECIPIENT_DEVICE;

        pHost->Control.setup.b.bRequest = HUB_CLEAR_FEATURE;
        pHost->Control.setup.b.wValue.w = wFeature; //
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
            printf("\r\n: warn : usbs%02x : clear hub feature timeout.", pHost->id);
            status = USBH_TIMEOUT;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
    }

    return status;
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
//----------------------------------------------------------------------------
USBH_StatusTypeDef HUB_StatusChangeControl(USBH_HandleTypeDef *pHost)
{
    USBH_URBStateTypeDef statusURB;
    USBH_StatusTypeDef status = USBH_BUSY;
    tagHUB_Handle *hub = (tagHUB_Handle*)pHost->pActiveClass->pData;

    switch(pHost->RequestState)
    {
    case CMD_SEND:
        USBH_InterruptReceiveData(pHost,
                                  hub->change.pBuf,
                                  hub->change.bLen,
                                  hub->change.bPipe);

        pHost->RequestState = CMD_WAIT;
        break;

    case CMD_WAIT:
        statusURB = USBH_LL_GetURBState(pHost, hub->change.bPipe);
        if  (statusURB == USBH_URB_DONE)
        {
            pHost->RequestState = CMD_SEND;
            status = USBH_OK;
        }

        /* manage error cases*/
        if  (statusURB == USBH_URB_STALL)
        {
            status = USBH_NOT_SUPPORTED;
        }
        else if (statusURB == USBH_URB_ERROR)
        {
            /* Device error */
            status = USBH_NOT_SUPPORTED;
        }
        break;

    default:
        break;
    }

    return (status);
}

// ============================================================================
// 功能：获取HUB和PORT的状态是否变化
// 参数：
// 返回：
// 备注：
//----------------------------------------------------------------------------
USBH_StatusTypeDef HUB_StatusChange(USBH_HandleTypeDef *pHost, u8 *pBuf, u8 bLen)
{
    USBH_StatusTypeDef status = USBH_BUSY;
    tagHUB_Handle *hub = (tagHUB_Handle*)pHost->pActiveClass->pData;

    if(CMD_SEND == pHost->RequestState)
    {
        hub->change.pBuf = pBuf;
        hub->change.bLen = bLen;
    }

    status = HUB_StatusChangeControl(pHost);
    if(USBH_OK != status)
    {
        if(pHost->dwTimeout++ > 100)
        {
            pHost->dwTimeout = 0;
            status = USBH_TIMEOUT;
            pHost->RequestState = CMD_SEND;
        }
    }
    else
    {
        pHost->dwTimeout = 0;
        pHost->RequestState = CMD_SEND;
    }

    return (status);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static USBH_StatusTypeDef __HUB_Init(USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef status;
    u32 size;
    tagDescHUB description = {0};
    tagHUB_Handle *hub =  (tagHUB_Handle*)pHost->pActiveClass->pData;
    u16 i;
    u8 *space;

    // 获取descriptor长度
    while(1)
    {
        status = USBH_HUB_GetDescriptor(pHost, (u8*)&description, 1);
        if(USBH_OK == status)
        {
            break;
        }
        else if(USBH_TIMEOUT == status)
        {
            printf("\r\n: erro : usbs%02x : get hub's descriptor time out", pHost->id);
            return (USBH_FAIL);
        }
    }

    // 获取descriptor
    while(1)
    {
        status = USBH_HUB_GetDescriptor(pHost, (u8*)&description, description.bLen);
        if(USBH_OK == status)
        {
            break;
        }
        else if(USBH_TIMEOUT == status)
        {
            printf("\r\n: erro : usbs%02x : get hub's descriptor time out", pHost->id);
            return (USBH_FAIL);
        }
    }

    size = sizeof(USBH_HandleTypeDef**) * (description.bPorts); //
    space = malloc(size);
    if(!space)
    {
        printf("\r\n: erro : usbs%02x : memory out", pHost->id);
        return (USBH_FAIL);
    }

    memset(space, 0x0, size);
    hub->pPort = (USBH_HandleTypeDef**)space;
    hub->bPorts = description.bPorts;

    size = (hub->bPorts + 1) >> 3;
    if((hub->bPorts + 1) & 0x7) // 相当于%8
        size += 1;

    hub->change.pStatus = malloc(size);
    if(!hub->change.pStatus)
    {
        free(space);
        printf("\r\n: erro : usbs%02x : memory out", pHost->id);
        return (USBH_FAIL);
    }

    memset(hub->change.pStatus, 0x0, size);
    hub->change.bBytes = size;

    // 各port先掉电，原因防止HUB初始时就是上电，会造成设备的状态不对，从而导致访问设备出错
    for(i = 1; i <= hub->bPorts; i++)
    {
        while(1)
        {
            status = USBH_HUB_ClearPortFeature(pHost, i, PORT_POWER);
            if(USBH_OK == status)
            {
                break;
            }
            else if(USBH_TIMEOUT == status)
            {
                printf("\r\n: info : usbs%02x : clear port <%d> power time out", pHost->id, i);
                return (USBH_FAIL);
            }
        }
    }

    DJY_EventDelay(10000000); // 10秒等待，电容放电等
    // 各port上电
    for(i = 1; i <= hub->bPorts; i++)
    {
        while(1)
        {
            status = USBH_HUB_SetPortFeature(pHost, i, PORT_POWER);
            if(USBH_OK == status)
            {
                break;
            }
            else if(USBH_TIMEOUT == status)
            {
                printf("\r\n: info : usbs%02x : enable port <%d> power time out", pHost->id, i);
                return (USBH_FAIL);
            }
        }
    }

    return (USBH_OK);
}

// ============================================================================
// 功能： hub port的设备被移除
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_DeviceRemoved(USBH_HandleTypeDef *pHost, u8 bPort)
{
    USBH_HandleTypeDef *port;
    tagHUB_Handle *hub = (tagHUB_Handle*)pHost->pActiveClass->pData;

    port = hub->pPort[bPort - 1];
    port->gState = HOST_DEV_DISCONNECTED;
    port->device.is_connected = 0;
    hub->pPort[bPort - 1] = NULL;

    USB_DeviceReset((port->id & 0xFF), ((port->id >> 8) & 0xFF)); // 硬件复位，由BSP实现

    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
USBH_StatusTypeDef USBH_HUB_DeviceConnected(USBH_HandleTypeDef *pHost, u8 bPort)
{
    USBH_HandleTypeDef *new;
    u16 size, i;
    u16 id;
    tagHUB_Handle *hub =  (tagHUB_Handle*)pHost->pActiveClass->pData;

    if(hub->pPort[bPort - 1])
    {
        printf("\r\nUSB HUB : error : port #%d is not removed, but new attached. remove the old.", bPort);
        USBH_HUB_DeviceRemoved(pHost, bPort);
        return (USBH_FAIL);
    }

    // 为新设备创建句柄
    new = USBH_NewHost(NULL);
    if(!new)
        return (USBH_FAIL);

    DeInitStateMachine(new);
    new->pData = pHost->pData; // 驱动
    new->id = pHost->id | ((bPort << 8) & 0xFF00);
    new->pUser = pHost->pUser;
    size = sizeof(pHost->pClass) / sizeof(pHost->pClass[0]);
    for(i = 0; i < size; i++)
        new->pClass[i] = pHost->pClass[i];

    new->ClassNumber = pHost->ClassNumber;
    new->pPipes = pHost->Pipes; // TODO
    new->device.is_connected = 1;
    hub->pPort[bPort - 1] = new;
    id = DJY_EventPop(gHubThreatID, NULL, 0, (ptu32_t)new, 0, 0); // 为新设备建立服务线程
    if(CN_EVENT_ID_INVALID == id)
    {
        free(new);
        printf("\r\nUSB HUB : error : cannot create new thread for new device in port %d", bPort);
        return (USBH_FAIL);
    }

    return (USBH_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：注册这个事件类型，当port有新设备插入时，会弹出一个事件(线程)用于维护这个设备
// ============================================================================
u32  USBH_HUB_Service(void)
{
    USBH_StatusTypeDef status;
    USBH_HandleTypeDef *handle;
    s64 comsumed, time;
    u8 devReady = 0;
    u8 port;

    DJY_GetEventPara((ptu32_t*)&handle, NULL);
    port = (u8)((handle->id & 0xFF00) >> 8);

    time = DJY_GetSysTime();
    while(1)
    {
        USBH_Process(handle);
        if(HOST_IDLE == handle->gState ||
                HOST_ABORT_STATE == handle->gState)
            break; // service退出

        // 防止枚举过程卡死
        comsumed = DJY_GetSysTime() - time;
        if(!devReady)
        {
            if(HOST_CLASS == handle->gState)
            {
                devReady = 1; // 设备类已经就绪，正常启动
            }
            else if(comsumed > 100000000) // 100秒
            {
                printf("\r\nUSB HUB : error : device on port <%d> is not ready in limited time. re-power the port", port);
                break;
            }
        }
    }

    // 在枚举过程出错了，将port的电源重置，如果电源无法重置，则重置整个HUB
    if(!devReady)
    {
        USB_DeviceReset((handle->id & 0xFF), ((handle->id &0xFF00)>>8)); // port上的设备复位
        time = DJY_GetSysTime();
        while(1)
        {
            status = USBH_HUB_ClearPortFeature(handle, port, PORT_POWER);
            if(USBH_OK == status)
                break;

            // TODO: 此时HUB可能也挂掉了，这个命令就无法发送下去，所以考虑复位整个HUB
            comsumed = DJY_GetSysTime() - time;
            if(comsumed > 30000000) // 30秒
            {
                printf("\r\nUSB HUB : error : the hub is not working. reset hub and its ports\r\n");
                USB_DeviceReset((handle->id & 0xFF), 0); // 重置端口上的设备后，发现HUB访问出错，则复位HUB
                break;
            }
        }

        DJY_EventDelay(10000000); // 等待一段时间，电容放电等

        if(USBH_OK == status)
        {
            time = DJY_GetSysTime();
            while(1)
            {
                status = USBH_HUB_SetPortFeature(handle, port, PORT_POWER);
                if(USBH_OK == status)
                    break;

                comsumed = DJY_GetSysTime() - time;
                if(comsumed > 30000000) // 30秒
                {
                    printf("\r\nUSB HUB : error : the hub is not working. reset hub and its ports.");
                    USB_DeviceReset((handle->id & 0xFF), 0); // // 复位HUB，各端口上在HUB的DeInit中执行
                    break;
                }
            }
        }
    }

    // 线程退出
    printf("\r\nUSB HUB : info : USB #%04x stack service exit.", handle->id);
    USBH_FreePipe(handle, handle->Control.pipe_in);
    USBH_FreePipe(handle, handle->Control.pipe_out);
    USBH_FreeHost(handle);

    return (0);
}
