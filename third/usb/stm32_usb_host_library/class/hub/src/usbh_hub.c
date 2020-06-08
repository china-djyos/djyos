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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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

    // ��λ��PORT���Ƴ���PORT���߳�
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

    DJY_EvttUnregist(gHubThreatID); // ע��HUBά���߳�

    return USBH_OK;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static USBH_StatusTypeDef __HUB_ClassRequest (USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef status = USBH_OK;

    return status;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
                res = USBH_HUB_GetPortStatus(pHost, i, &status); // ��ȡ�˿�״̬
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

            if(status.bit.connect) // ���豸����
            {
                while(1)
                {
                    res = USBH_HUB_SetPortFeature(pHost, i, PORT_RESET); // ʹ�ܶ˿�
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
                        break; // �ȴ�RESET���

                    DJY_EventDelay(100000); // �ȴ�100ms
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
                        printf("\r\nHUB : debug : uncovered logic."); // �������߼�
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
            else // �豸�γ�
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
                            break; // ֱ�����߳��˳�
                        }
                    }
                }
                // �߳��˳�
                USBH_HUB_DeviceRemoved(pHost, i);

                // ��port������״̬�ı�
                if(status.bit.connectionChange)
                {
                    // ��port�ȵ��磬ԭ���ֹHUB��ʼʱ�����ϵ磬������豸��״̬���ԣ��Ӷ����·����豸����
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

                    DJY_EventDelay(10000000); // 10��ȴ������ݷŵ��
                    // ��port�ϵ�
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
        DJY_EventDelay(1000000); // 1s����ɨ��һ��HUB���˿ڵ�״̬
        hub->state = HUB_PORT_SCAN;
        break;

    case HUB_PORT_SCAN:
        while(1)
        {
            DJY_EventDelay(200);
            res = HUB_StatusChange(pHost, hub->change.pStatus, hub->change.bBytes); // ��ȡHUB�˿�״̬,�ж϶˿��Ƿ����˱仯
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
        res = USBH_HUB_HandleChange(pHost); // ����port��״̬�仯
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
// ���ܣ����¶˿��豸��Timer����
// ������
// ���أ�
// ��ע��
// ============================================================================
static USBH_StatusTypeDef __HUB_SOFProcess (USBH_HandleTypeDef *pHost)
{
    u8 i;
    USBH_HandleTypeDef *port;
    tagHUB_Handle *hub = (tagHUB_Handle*) pHost->pActiveClass->pData;

    if(HUB_INIT >= hub->state)
        return (USBH_OK); // hub��δ��ʼ��

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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
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
// ���ܣ���ȡHUB��PORT��״̬�Ƿ�仯
// ������
// ���أ�
// ��ע��
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
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static USBH_StatusTypeDef __HUB_Init(USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef status;
    u32 size;
    tagDescHUB description = {0};
    tagHUB_Handle *hub =  (tagHUB_Handle*)pHost->pActiveClass->pData;
    u16 i;
    u8 *space;

    // ��ȡdescriptor����
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

    // ��ȡdescriptor
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
    if((hub->bPorts + 1) & 0x7) // �൱��%8
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

    // ��port�ȵ��磬ԭ���ֹHUB��ʼʱ�����ϵ磬������豸��״̬���ԣ��Ӷ����·����豸����
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

    DJY_EventDelay(10000000); // 10��ȴ������ݷŵ��
    // ��port�ϵ�
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
// ���ܣ� hub port���豸���Ƴ�
// ������
// ���أ�
// ��ע��
// ============================================================================
USBH_StatusTypeDef USBH_HUB_DeviceRemoved(USBH_HandleTypeDef *pHost, u8 bPort)
{
    USBH_HandleTypeDef *port;
    tagHUB_Handle *hub = (tagHUB_Handle*)pHost->pActiveClass->pData;

    port = hub->pPort[bPort - 1];
    port->gState = HOST_DEV_DISCONNECTED;
    port->device.is_connected = 0;
    hub->pPort[bPort - 1] = NULL;

    USB_DeviceReset((port->id & 0xFF), ((port->id >> 8) & 0xFF)); // Ӳ����λ����BSPʵ��

    return (USBH_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
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

    // Ϊ���豸�������
    new = USBH_NewHost(NULL);
    if(!new)
        return (USBH_FAIL);

    DeInitStateMachine(new);
    new->pData = pHost->pData; // ����
    new->id = pHost->id | ((bPort << 8) & 0xFF00);
    new->pUser = pHost->pUser;
    size = sizeof(pHost->pClass) / sizeof(pHost->pClass[0]);
    for(i = 0; i < size; i++)
        new->pClass[i] = pHost->pClass[i];

    new->ClassNumber = pHost->ClassNumber;
    new->pPipes = pHost->Pipes; // TODO
    new->device.is_connected = 1;
    hub->pPort[bPort - 1] = new;
    id = DJY_EventPop(gHubThreatID, NULL, 0, (ptu32_t)new, 0, 0); // Ϊ���豸���������߳�
    if(CN_EVENT_ID_INVALID == id)
    {
        free(new);
        printf("\r\nUSB HUB : error : cannot create new thread for new device in port %d", bPort);
        return (USBH_FAIL);
    }

    return (USBH_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��ע������¼����ͣ���port�����豸����ʱ���ᵯ��һ���¼�(�߳�)����ά������豸
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
            break; // service�˳�

        // ��ֹö�ٹ��̿���
        comsumed = DJY_GetSysTime() - time;
        if(!devReady)
        {
            if(HOST_CLASS == handle->gState)
            {
                devReady = 1; // �豸���Ѿ���������������
            }
            else if(comsumed > 100000000) // 100��
            {
                printf("\r\nUSB HUB : error : device on port <%d> is not ready in limited time. re-power the port", port);
                break;
            }
        }
    }

    // ��ö�ٹ��̳����ˣ���port�ĵ�Դ���ã������Դ�޷����ã�����������HUB
    if(!devReady)
    {
        USB_DeviceReset((handle->id & 0xFF), ((handle->id &0xFF00)>>8)); // port�ϵ��豸��λ
        time = DJY_GetSysTime();
        while(1)
        {
            status = USBH_HUB_ClearPortFeature(handle, port, PORT_POWER);
            if(USBH_OK == status)
                break;

            // TODO: ��ʱHUB����Ҳ�ҵ��ˣ����������޷�������ȥ�����Կ��Ǹ�λ����HUB
            comsumed = DJY_GetSysTime() - time;
            if(comsumed > 30000000) // 30��
            {
                printf("\r\nUSB HUB : error : the hub is not working. reset hub and its ports\r\n");
                USB_DeviceReset((handle->id & 0xFF), 0); // ���ö˿��ϵ��豸�󣬷���HUB���ʳ�����λHUB
                break;
            }
        }

        DJY_EventDelay(10000000); // �ȴ�һ��ʱ�䣬���ݷŵ��

        if(USBH_OK == status)
        {
            time = DJY_GetSysTime();
            while(1)
            {
                status = USBH_HUB_SetPortFeature(handle, port, PORT_POWER);
                if(USBH_OK == status)
                    break;

                comsumed = DJY_GetSysTime() - time;
                if(comsumed > 30000000) // 30��
                {
                    printf("\r\nUSB HUB : error : the hub is not working. reset hub and its ports.");
                    USB_DeviceReset((handle->id & 0xFF), 0); // // ��λHUB�����˿�����HUB��DeInit��ִ��
                    break;
                }
            }
        }
    }

    // �߳��˳�
    printf("\r\nUSB HUB : info : USB #%04x stack service exit.", handle->id);
    USBH_FreePipe(handle, handle->Control.pipe_in);
    USBH_FreePipe(handle, handle->Control.pipe_out);
    USBH_FreeHost(handle);

    return (0);
}
