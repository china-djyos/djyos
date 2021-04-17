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

#include <stdio.h>
#include <lock.h>
#include <dbug.h>
#include <device.h>
#include <djyos.h>
#include "./stm32_usb_host_library/core/inc/usbh_core.h"
#include "./stm32_usb_host_library/class/msc/inc/usbh_msc.h"
#include "./stm32_usb_host_library/class/cdc/inc/usbh_cdc.h"
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"
#include "./stm32_usb_host_library/class/hub/inc/usbh_hub.h"
#include "./stm32_usb_host_library/class/hid/inc/usbh_hid.h"
#include "usb.h"
#include <stdlib.h>

USBH_HandleTypeDef gUSBHost[2];
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

// ============================================================================
// 功能： 设备复位
// 参数：
// 返回：
// 备注： 由BSP实现
// ============================================================================
__attribute__((weak)) void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    bHostID = bHostID;
    bDevID = bDevID;
}

// ============================================================================
// 功能： 用户逻辑
// 参数：
// 返回：
// 备注： 由BSP实现
// ============================================================================
__attribute__((weak)) void USB_UserInstall(const char *TargetFs,u16 wHostID)
{
    wHostID = wHostID;
}

// ============================================================================
// 功能： 用户逻辑，执行USB主机挂起前的逻辑
// 参数：
// 返回：
// 备注： 由BSP实现
// ============================================================================
__attribute__((weak)) void USB_UserSuspend(u16 wHostID)
{
    wHostID = wHostID;
}

// ============================================================================
// 功能： 用户逻辑，执行USB主机恢复后的逻辑
// 参数：
// 返回：
// 备注： 由BSP实现
// ============================================================================
__attribute__((weak)) void USB_UserResume(u8 bHostID)
{
    bHostID = bHostID;
}

// ============================================================================
// 功能： 通过名称获取USB host句柄
// 参数：
// 返回： NULL -- 控制器不存在
// 备注：
// ============================================================================
tagHost *pHostList;
USBH_HandleTypeDef *USBH_GetHandle(char *pName)
{
    tagHost *host = pHostList;

    while(host)
    {
        if(!strcmp(pName, host->pName))
            break;

        host = host->pNext;
    }

    return (&host->handle);
}

// ============================================================================
// 功能： 通过USB host句柄查找其管理结构体
// 参数：
// 返回： NULL -- 控制器不存在
// 备注：
// ============================================================================
tagHost *USBH_GetHost(USBH_HandleTypeDef *pHandle)
{
    tagHost *host = pHostList;

    if(!pHandle)
        return (NULL);

    while(host)
    {
        if(pHandle == &host->handle)
            break;

        host = host->pNext;
    }

    return (host);
}

// ============================================================================
// 功能： 通过USB host句柄查找其名称
// 参数：
// 返回： NULL -- 控制器不存在
// 备注：
// ============================================================================
char *USBH_GetName(USBH_HandleTypeDef *pHandle)
{
    tagHost *host = pHostList;
    char *res = NULL;

    if(!pHandle)
        return (NULL);

    while(host)
    {
        if(pHandle == &host->handle)
            break;

        host = host->pNext;
    }

    if(host)
        res = host->pName;

    return (res);
}

// ============================================================================
// 功能： 新建一个USB host句柄
// 参数： pName -- USB host的名称，可以为NULL
// 返回： NULL -- 控制器不存在
// 备注： 未进行同名检查
// ============================================================================
USBH_HandleTypeDef *USBH_NewHost(char *pName)
{
    tagHost *host;
    char *name = NULL;
    u32 size;

    if(pName)
    {
        size = strlen(pName) + 1;
        name = malloc(size);
        if(!name)
            return (NULL);
        strcpy(name, pName);
    }

    size = sizeof(tagHost);
    host = malloc(size);
    if(!host)
    {
        free(name);
        return (NULL);
    }

    memset(host, 0x0, size);
    host->pName = name;

    if(!pHostList)
    {
        pHostList = host;
    }
    else
    {
        // 新成员放入队列首
        host->pNext = pHostList;
        pHostList = host;
    }

    return (&host->handle);
}
// ============================================================================
// 功能： 重置USB host的名称
// 参数：
// 返回： -1 -- 失败；0 -- 成功；
// 备注：
// ============================================================================
s32 USBH_SetHostName(USBH_HandleTypeDef *pHandle, char *pName)
{
    tagHost *host;
    char *new;

    if(!pName)
        return (-1);

    host = USBH_GetHost(pHandle);
    if(!host)
        return (-1);

    new = malloc(strlen(pName) + 1);
    if(!new)
        return (-1);

    if(host->pName)
        free(host->pName); // 释放掉原有名称

    host->pName = new;
    strcpy(host->pName, pName); // 设置新名称

    return (0);
}
// ============================================================================
// 功能：
// 参数：
// 返回： NULL -- 控制器不存在
// 备注：
// ============================================================================
s32 USBH_FreeHost(USBH_HandleTypeDef *pHandle)
{
    tagHost *host = pHostList;
    tagHost *prev = NULL;

    while(host)
    {
        if(pHandle == &host->handle)
            break;

        prev = host;
        host = host->pNext;
    }

    if(host) // 需要释放的host
    {
        if(prev)
            prev->pNext = host->pNext; // 从链表中剔除
        else
            pHostList = host->pNext; // 第一个

        free(host->pName);
        free(host);
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能： USB0协议栈服务线程
// 参数：
// 返回：
// 备注：
// ============================================================================
u32 USB_ServiceThread(void)
{
    USBH_HandleTypeDef *host;
    s64 time, comsumed;
    u32 id;
    char name[] = "root #x";
    u8 devAttached = 0;
    u8 devReady = 0;

    DJY_GetEventPara((ptu32_t*)&id, NULL); // id -- host ID;

    itoa(id, &name[6], 10);
    host = USBH_NewHost(name);
    if(!host)
    {
        printf("\r\n: erro : usbs%02x : quit! cannot get new host.", host->id);
        return (0);
    }

    USBH_LL_Register(id); // 驱动注册
    USBH_Init(host, USBH_UserProcess, id); // 初始化

    USBH_RegisterClass(host, USBH_MSC_CLASS); // 注册MSC类
    USBH_RegisterClass(host, USBH_CDC_CLASS); // 注册CDC类
    USBH_RegisterClass(host, USBH_CUSTOM_CLASS); // 注册CUSTOM类
    USBH_RegisterClass(host, USBH_HUB_CLASS); // 注册HUB类
    USBH_RegisterClass(host, USBH_HID_CLASS); // 注册HID类

    USBH_Start(host); // 启动USB接口；开中断，VBUS上电

    printf("\r\n: info : usbs%02x : start to run...", host->id);

    USB_DeviceReset(id, 0);

    time = DJY_GetSysTime();
    // Run Application (Blocking mode)
    while (1)
    {
        // USB Host Background task
        USBH_Process(host);

        // 枚举过程中，如果一直未检测到设备，则每隔45秒复位一次设备
        comsumed = DJY_GetSysTime() - time;
        if(!devAttached)
        {
            if(HOST_DEV_ATTACHED == host->gState)
            {
                devAttached = 1; // 已检测到设备
                time = DJY_GetSysTime();
            }
            else if(comsumed > 60000000) // 60秒
            {
                printf("\r\n: info : usbs%02x : no device attach, will reset device.", host->id);
                USB_DeviceReset(id, 0);
                time = DJY_GetSysTime();
            }
        }

        // 设备已经attached一段时间，但枚举过程太久，可能在某个环节卡死
        if(devAttached && !devReady)
        {
            if(HOST_CLASS == host->gState)
            {
                devReady = 1; // 设备类已经就绪，正常启动
            }
            else if(comsumed > 100000000) // 100秒
            {
                // 重置
                devAttached = 0;
                DeInitStateMachine(host);
                if(host->pActiveClass != NULL)
                {
                    host->pActiveClass->DeInit(host);
                    host->pActiveClass = NULL;
                }

                printf("\r\n: erro : usbs%02x : no device ready, will reset device.", host->id);
                USB_DeviceReset(id, 0);
                time = DJY_GetSysTime();
            }
        }
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static void USBH_UserProcess(USBH_HandleTypeDef *pHost, u8 bID)
{
    switch(bID)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            printf("\r\n: info : usbs%02x : host user select configuration hook.", pHost->id);
            break;

        case HOST_USER_DISCONNECTION:
            printf("\r\n: info : usbs%02x : host user disconnection hook.", pHost->id);
            break;

        case HOST_USER_CLASS_SELECTED:
            printf("\r\n: info : usbs%02x : host user class selected hook.", pHost->id);
            break;

        case HOST_USER_CLASS_ACTIVE:
            printf("\r\n: info : usbs%02x : host user class active hook.", pHost->id);
            break;

        case HOST_USER_CONNECTION:
            printf("\r\n: info : usbs%02x : host user connection hook.", pHost->id);
            break;

        default:
            break;
    }
}

// ============================================================================
// 功能： 查询USB设备状态
// 参数： bController -- USB设备号
// 返回：  0 -- 设备正常运行；1 -- 设备已挂起；-1 -- 设备未准备好；
// 备注：
// ============================================================================
s32 USBH_Query(void *pHost)
{
    USBH_HandleTypeDef *host;

    if(!pHost)
        return (-1);

    host = (USBH_HandleTypeDef *)pHost;

    if((HOST_CLASS == host->gState) && (USBH_OK == host->pActiveClass->ClassState))
        return (0);

    if((HOST_SUSPENDED == host->gState) &&  (USBH_OK == host->pActiveClass->ClassState))
        return (1);

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：未完成
// ============================================================================
#define FORCE_DISCONNECT            1
#define FORCE_CHANGE_STATE            2
void USBH_Force(USBH_HandleTypeDef *pHost, u32 dwOps, void *pParams)
{
    USBH_StatusTypeDef  res;

    if(!pHost)
        return;

    if(FORCE_DISCONNECT == dwOps)
    {
        res = USBH_LL_Disconnect(pHost);
        if(USBH_OK != res)
            printf("\r\n: erro : usbs%02x : cannot disconnect the device from host.");

        return ;
    }

    if(FORCE_CHANGE_STATE == dwOps)
    {
        u32 parameters;
        if(!pParams)
        {
            printf("\r\n: warn : usbs%02x : force changing state failed(parameters error).");
            return;
        }

        parameters = *(u32*)pParams;
        if(1 == parameters)
        {
            pHost->device.is_connected = 1;
            return;
        }
        else if(2 == parameters)
        {
            pHost->gState = HOST_DEV_ATTACHED;
            return;
        }
    }

}


// ============================================================================
// 功能： 查询USB设备状态
// 参数： pName-- USB设备名
// 返回：  0 -- 设备已准备；-1 -- 设备未准备好；
// 备注：
// ============================================================================
s32 USBH_QueryByName(char *pName)
{
    USBH_HandleTypeDef *host;

    if(!pName)
        return (-1);

    host = USBH_GetHandle(pName);
    if(!host)
        return (-1);

    return (USBH_Query(host));
}

// ============================================================================
// 功能： 判断是否是分支设备
// 参数：
// 返回： -1 -- HUB的扩展设备； 0 -- 非HUB扩展设备
// 备注：
// ============================================================================
s32 USBH_DeviceFromHub(USBH_HandleTypeDef *pHost)
{
    if(pHost->id & 0xFF00)
        return (-1); // 来自于HUB的设备

    return (0);
}

// ============================================================================
// 功能： 挂起USB
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 USBH_Suspend(USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef res;
    extern USBH_StatusTypeDef USBH_LL_Suspend (USBH_HandleTypeDef *pHost);

    if(!pHost)
        return (-1);

    if(HOST_SUSPENDED == pHost->gState)
        return (0);

    USB_UserSuspend(pHost->id);

    res = USBH_LL_Suspend(pHost);
    if(USBH_OK != res)
        return (-1);

    pHost->resumeState = pHost->gState;
    pHost->gState = HOST_SUSPENDED;
    return (0);
}

// ============================================================================
// 功能： 恢复USB
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 USBH_Resume(USBH_HandleTypeDef *pHost)
{
    USBH_StatusTypeDef res;
    extern USBH_StatusTypeDef USBH_LL_Resume (USBH_HandleTypeDef *pHost);

    if(!pHost)
        return (-1);

    if(HOST_SUSPENDED != pHost->gState)
        return (-1);

    USB_UserResume(pHost->id);

    res = USBH_LL_Resume(pHost);
    if(USBH_OK != res)
        return (-1);


    pHost->gState = pHost->resumeState;
    return (0);
}

// ============================================================================
// 功能：
// 参数： controller -- 根据用户配置
// 返回： "0" -- 成功; "-1" -- 失败;
// 备注：
// ============================================================================
s32 ModuleInstall_USB(const char *TargetFs,u8 controller)
{
    u16 thread;
//    static char dev_usb = 0;
    char id[2];
    char name[] = "USB #x stack service";
    void USB_ShellInstall(void);

    if(controller >= 10)
    {
        error_printf("usb", "too big ID(%d) for usb controller.", controller);
        return (-1);
    }
    if(strcmp(TargetFs,"NULL") == 0)
        TargetFs = NULL;
//  if(!dev_group_addo("usb"))
//  {
//      error_printf("usb", "cannot create \"usb\" group.");
//      return (-1);
//  }
    itoa(controller, id, 10);
//  memcpy(&name[5], id, 1); // 初始化线程名
    name[5] = id[0];

    USB_ShellInstall(); // USB 相关shell命令

    USB_UserInstall(TargetFs,controller);

    thread = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 0,
                            USB_ServiceThread, NULL, 0x2000, name);
    if(CN_EVTT_ID_INVALID != thread)
    {
        DJY_EventPop(thread, NULL, 0, controller, 0, 0);
        return (0);
    }

    error_printf("usb", "%s cannot establish.", name);
    return (-1);

}
#if 0
s32 ModuleInstall_USB(u8 bController)
{
    u16 thread;
    s32 res;
    USBH_HandleTypeDef *host;
#if 0
    char *name[] = {"#1", "#2"};
#else
    char *name = "USB #x stack service";
    char id[2];

    itoa(bController, id, 10);

    memcpy(&name[6], id, 1);

#endif
#if 0
    extern void USBH_LL_Register(u8 bController);
#endif
    extern void USB_ShellInstall(void);

#if 0
    host = USBH_HostHandle(bController);
    if(!host)
        return (-1);

    USBH_LL_Register(bController); // 驱动注册
    USBH_Init(host, USBH_UserProcess, bController); // 初始化
#else
#if 0
    host = USBH_NewHost(name[bController-1]);
    if(!host)
    {
        USBH_UsrLog("\r\nUSB #1 stack service QUIT <the host is not initialized>\r\n");
        return (0);
    }

    USBH_LL_Register(bController); // 驱动注册
    USBH_Init(host, USBH_UserProcess, bController); // 初始化
#endif
#endif
    USB_ShellInstall(); // install USB shell command

//    USBH_RegisterDevCUSTOM();
    USBH_ResigerDevMSC();

#if 0
    if(1 == bController)
    {
#if 0
        extern s32 USB_RegisterMSC_Device(u8 bController);
        res = USB_RegisterMSC_Device(bController);
        if(res)
        {
            USBH_UsrLog("\r\nUSB has been registered.");
            return (-1);
        }
#endif
        thread = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 0,
                    USB_1_Thread, NULL, 0x2000, "USB #1 stack service");
        if(CN_EVTT_ID_INVALID != thread)
        {
            DJY_EventPop(thread, NULL, 0, 0, 0, 0);
            return (0);
        }
    }
    else
    {
#if 0
        extern s32 USBH_RegisterCUSTOM_Device(u8 bController);
        res = USBH_RegisterCUSTOM_Device(bController);
        if(res)
        {
            USBH_UsrLog("\r\nUSB has been registered.");
            return (-1);
        }
#endif
        thread = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 0,
                    USB_2_Thread, NULL, 0x2000, "USB #2 stack service");
        if(CN_EVTT_ID_INVALID != thread)
        {
            DJY_EventPop(thread, NULL, 0, 0, 0, 0);
            return (0);
        }
    }
#else
    thread = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 0,
                        USB_1_Thread, NULL, 0x2000, "USB #1 stack service");
    if(CN_EVTT_ID_INVALID != thread)
    {
        DJY_EventPop(thread, NULL, 0, 0, 0, 0);
        return (0);
    }
#endif

    return (-1);
}
#endif

