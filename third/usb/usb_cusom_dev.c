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
#include <stddef.h>
#include <stdlib.h>
#include <device.h>
#include <ring.h>
#include <djyos.h>
#include <systime.h>
#include "./stm32_usb_host_library/class/custom/inc/usbh_custom.h"
#include "usb.h"

#define DEVICES                 3
#define DEVICES_IN_TASK         2
#define DEVCIES_BUF_SIZE        128

static char *dev_name[DEVICES] = {"usb-at", "usb-modem", "usb-dbg"};
typedef struct __devCustom
{
     struct RingBuf *pRingRd;
     // struct DjyDevice *pDev;
     struct Object *pDev;
     struct SemaphoreLCB *pSemp;
} tagDevCustom;

typedef enum __workingMode
{
    NORMAL, // 直接访问模式，不存在USB睡眠逻辑
    SLEEP, // 睡眠模式，即读逻辑通过专用的睡眠线程实现
    STOP
}tagMode;
struct __Custom
{
    USBH_HandleTypeDef *pHandle;
    u32 dwJobs; // 允许多个任务同时进行
    u32 dwSleepCycle;
    u32 dwWakeCycle;
    u32 dwBackup;
    void *pSleep;
    void *pLock;
    enum {
        TASK_OFF,
        TASK_ON,
        BLOCK
    }status;
    tagMode toMode;
    tagMode curMode;
    tagDevCustom dev[DEVICES];
}custom;

struct __cycle{
    u32 dwWake;
    u32 dwSleep;
};

struct __cycle defaultCycle = {500000, 10000000};
s32 CUSTOM_Suspend(void);
s32 CUSTOM_Resume(void);

// ============================================================================
// 功能：等待BLOCK操作完成完成
// 参数:
// 返回:
// 备注:
// ============================================================================
void __CUSTOM_WAIT(void)
{
    while(1)
    {
        if(BLOCK == custom.status)
            DJY_EventDelay(1000); // 等待BLOCK操作完成
        else
            break;
    }
}

// ============================================================================
// 功能：增加一个标记操作
// 参数：
// 返回：
// 备注：保证设备不会被挂起
// ============================================================================
void __CUSTOM_Jobup(void)
{
    custom.dwJobs++;
    __CUSTOM_WAIT(); // 等待BLOCK操作完成
}

// ============================================================================
// 功能：消除一个操作标记
// 参数:
// 返回:
// 备注:
// ============================================================================
void __CUSTOM_Jobdown(void)
{
    custom.dwJobs--;
}

// ============================================================================
// 功能：
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 __CUSTOM_Job(void)
{
    return (custom.dwJobs);
}

// ============================================================================
// 功能：
// 参数:
// 返回:
// 备注:
// ============================================================================
void __CUSTOM_Lockup(void)
{
    if(custom.pLock)
        Lock_MutexPend(custom.pLock, CN_TIMEOUT_FOREVER);
}

// ============================================================================
// 功能：
// 参数:
// 返回:
// 备注:
// ============================================================================
void __CUSTOM_Lockdown(void)
{
    if(custom.pLock)
        Lock_MutexPost(custom.pLock);
}

// ============================================================================
// 功能：
// 参数:
// 返回:
// 备注:
// ============================================================================
struct __debugCustom{
    u32 device0 : 1;
    u32 device1 : 1;
    u32 device2 : 1;
    u32 format    : 1; // 0 -- hex; 1 -- ascii
};
u32 dwDebugCustomDev;
static struct __debugCustom *pLocalDebug = (struct __debugCustom*)&dwDebugCustomDev;
static u32 __CUSTOM_TaskRead(void)
{
    u8 buf[DEVCIES_BUF_SIZE];
    u32 len, max, i;
    struct __Custom *custom;
    struct __cycle *cycle;
    s64 timeout, current;

    DJY_GetEventPara((ptu32_t*)&custom, (ptu32_t*)&cycle);

    if(!custom)
    {
        printf("\r\n: erro : usb    : custom read task's parameter is bad.");
        return (0);
    }

    if(!custom->dwSleepCycle)
        custom->dwSleepCycle = cycle->dwSleep;

    if(!custom->dwWakeCycle)
        custom->dwWakeCycle = cycle->dwWake;

    for(i = 0; i < DEVICES_IN_TASK; i++)
    {
        custom->dev[i].pRingRd = Ring_Create(DEVCIES_BUF_SIZE);
        if(!custom->dev[i].pRingRd)
        {
            printf("\r\n: erro : usb    : custom create ring failed.");
            goto __EXIT_READ_TASK;
        }

        custom->dev[i].pSemp = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, NULL);
        if(!custom->dev[i].pSemp)
        {
            printf("\r\n: erro : usb    : custom create semaphore failed.");
            goto __EXIT_READ_TASK;
        }
    }

    while(custom->pHandle)
    {
        switch(custom->toMode)
        {
            case NORMAL:
            {
                s32 temp;
                temp = CUSTOM_Resume();
                if(-1 == temp)
                    temp = 0;
                custom->curMode = NORMAL;
                Lock_SempPend(custom->pSleep, CN_TIMEOUT_FOREVER);
                break;
            }

            case SLEEP:
                timeout = DJY_GetSysTime() + custom->dwWakeCycle;
                do
                {
                    for(i = 0; i < DEVICES_IN_TASK; i++)
                    {
                        max = Ring_CheckFree(custom->dev[i].pRingRd);
                        if(max > 64)
                        {
                            len = max;
                            __CUSTOM_Jobup();
                            CUSTOM_Resume();
                            switch(i)
                            {
                                case 0 :
                                    USBH_CUSTOM_AT_Receive(custom->pHandle, buf, &len, 2000);
                                    break;

                                case 1 :
                                    USBH_CUSTOM_MODEM_Receive(custom->pHandle, buf, &len, 2000);
                                    break;

                                default: break;
                            }

                            __CUSTOM_Jobdown();
                            max = max-len; // 实际读到的字节数
                            if(max)
                            {
                                if((pLocalDebug->device1) || (pLocalDebug->device0))
                                {
                                    u32 j;
                                    if((pLocalDebug->device0) && (0 == i))
                                    {
                                        printf("\r\n: dbug : usb    : custom has %d bytes from at channel >> ", max);
                                    }

                                    if((pLocalDebug->device1) && (1 == i))
                                    {
                                        printf("\r\n: dbug : usb    : custom has %d bytes from modem channel >> ", max);
                                    }

                                    if(pLocalDebug->format)
                                    {
                                        for(j = 0; j < max; j++)
                                        {
                                            printf("%02x ", buf[j]);
                                        }
                                    }
                                    else
                                    {
                                        for(j = 0; j < max; j++)
                                        {
                                            printf("%c ", buf[j]);
                                        }
                                    }
                                }

                                len = Ring_Write(custom->dev[i].pRingRd, buf, max);
                                if(len != max)
                                {
                                    printf("\r\n: warn : usb    : custom ring buffer overflow. %d bytes lost\r\n", (max-len));
                                }
                                Lock_SempPost(custom->dev[i].pSemp);
                            }
                        }
                    }
                    current = DJY_GetSysTime();
                }while(current <= timeout);

                if(!CUSTOM_Suspend())
                {
                    custom->curMode = SLEEP;
                    Lock_SempPend(custom->pSleep, custom->dwSleepCycle);
                }
                else
                {
                    printf("\r\n: info : usb    : custom no suspend, no sleep\r\n");
                }
                break;

            default: break;
        }
    }

__EXIT_READ_TASK:
    for(i = 0; i < DEVICES_IN_TASK; i++)
    {
        Ring_Destroy(custom->dev[i].pRingRd);
        Lock_SempDelete(custom->dev[i].pSemp);
    }

    custom->status = TASK_OFF;

    return (0);
}

// ============================================================================
// 功能：从read task的读缓冲中获取modem的数据
// 参数：pBuffer -- 读缓冲；dwLen -- 读长度（单位：字节）；dwTimeout -- 读超时；
// 返回：实际读到的字节数；
// 备注：
// ============================================================================
u32 __CUSTOM_ReadFromTaskMODEM(u8 *pBuffer, u32 dwLen, u32 dwTimeout)
{
    u32 len;

    if(FALSE == Lock_SempPend(custom.dev[1].pSemp, dwTimeout))
        return (0);

    len = Ring_Read(custom.dev[1].pRingRd, pBuffer, dwLen);

    if(!Ring_IsEmpty(custom.dev[1].pRingRd))
        Lock_SempPost(custom.dev[1].pSemp); // 仍旧有数据可读，设置信号

    return len;
}

// ============================================================================
// 功能：从read task的读缓冲中获取at的数据
// 参数：pBuffer -- 读缓冲；dwLen -- 读长度（单位：字节）；dwTimeout -- 读超时；
// 返回：实际读到的字节数；
// 备注：
// ============================================================================
u32 __CUSTOM_ReadFromTaskAT(u8 *pBuffer, u32 dwLen, u32 dwTimeout)
{
    u32 len;

    if(FALSE == Lock_SempPend(custom.dev[0].pSemp, dwTimeout))
        return (0);

    len = Ring_Read(custom.dev[0].pRingRd, pBuffer, dwLen);

    if(!Ring_IsEmpty(custom.dev[0].pRingRd))
        Lock_SempPost(custom.dev[0].pSemp); // 仍旧有数据可读，设置信号

    return len;
}

// ============================================================================
// 功能：将custom设备的主控设置为suspend
// 参数:
// 返回：0 -- 成功；-1 -- 失败；
// 备注:
// ============================================================================
s32 CUSTOM_Suspend(void)
{
    u32 status;
    s32 res = -1;

    if(!__CUSTOM_Job()) // CUSTOM设备在被操作期间，不进入suspend，而是直接退出
    {
        __CUSTOM_Lockup();
        status = custom.status;
        custom.status = BLOCK; // suspend期间需要BLOCK
        res = USBH_Suspend(custom.pHandle);
        custom.status = status;
        __CUSTOM_Lockdown();
    }
    return (res);

}

// ============================================================================
// 功能：将custom设备的主控从suspend变为resume
// 参数:
// 返回：0 -- 成功；-1 -- 失败；
// 备注:
// ============================================================================
s32 CUSTOM_Resume(void)
{
    u32 status;
    s32 res = -1;

    __CUSTOM_Lockup();

    status = custom.status;
    custom.status = BLOCK;
    res = USBH_Resume(custom.pHandle);
    DJY_DelayUs(10000); // 等待10ms
    custom.status = status;
    __CUSTOM_Lockdown();

    return (res);
}

//============================================================================
//功能：打开AT接口
// 参数：hdl，待操作的串口文件指针
//      Mode，打开模式，O_RDONLY、O_WRONLY、O_RDWR
//      timeout，超时时间。
// 返回：0=success，-1=error
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
//============================================================================
s32 CUSTOM_OpenAT(struct objhandle *hdl, u32 Mode, u32 timeout)
{
//    USBH_HandleTypeDef *host;

    if(hdl == NULL)
        return (-1);

//    host = (USBH_HandleTypeDef *)Device_GetDrvTag(Handle2fd(hdl));
//    Handle_SetContext(hdl, (ptu32_t)host);  //Fd上下文指向串口控制块struct UartGeneralCB
    return 0;
}


// ============================================================================
// 功能：通过AT接口发送数据
// 参数：pBuf -- 命令；dwLen -- 命令字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际发送字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
// u32 CUSTOM_WriteAT(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
//s32 CUSTOM_WriteAT(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
s32 CUSTOM_WriteAT(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u8 *buf = pBuf;
    u32 len = dwLen;
    u32 timeout = dwTimeout;

    if(!host)
    {
        printf("\r\n: warn : usb    : \"at\" device(channel) is not register.");
        return (0);
    }

    __CUSTOM_Jobup();
    if(HOST_SUSPENDED == host->gState)
    {
        if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
        {
            CUSTOM_Resume();
        }
        else
        {
            __CUSTOM_Jobdown();
            return (0);
        }
    }

    USBH_CUSTOM_AT_Transmit(host, buf, &len, timeout);

    __CUSTOM_Jobdown();
#if 0
    if(SLEEP == custom.mode)
        CUSTOM_Suspend();
#else
    if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
    {
        //CUSTOM_Suspend();
        // 立即读，华为模块必须这样，因为进入睡眠模式，其不缓存数据
        Lock_SempPost(custom.pSleep);
    }
#endif

    return (dwLen - len);
}

// ============================================================================
// 功能：通过AT接口接收数据
// 参数：pBuf -- 数据缓冲；dwLen -- 数据字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际接收字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
// u32 CUSTOM_ReadAT(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 wdOffset, u32 dwTimeout)
//s32 CUSTOM_ReadAT(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 wdOffset, u32 dwTimeout)
s32 CUSTOM_ReadAT(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u32 len, bLen;

    if(!host)
    {
        printf("\r\n: warn : usb    : \"at\" device(channel) is not register.");
        return (0);
    }

    if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
    {
        len = __CUSTOM_ReadFromTaskAT(pBuf, dwLen, dwTimeout);
        return (len);
    }
    else
    {
        __CUSTOM_Jobup();
        if(HOST_SUSPENDED == host->gState)
        {
            __CUSTOM_Jobdown();
            return (0);
        }

        // 先把buffer中的数据取光
        bLen = __CUSTOM_ReadFromTaskAT(pBuf, dwLen, 0);
        if(dwLen == bLen)
        {
            __CUSTOM_Jobdown();
            return (len);
        }

        len = dwLen - bLen; // 剩余字节数
        dwLen = len;

        USBH_CUSTOM_AT_Receive(host, pBuf, &len, dwTimeout);
        __CUSTOM_Jobdown();
        len = dwLen - len; // 读取的字节数
        return (bLen + len);
    }
}

//============================================================================
//功能：打开MODEM接口
// 参数：hdl，待操作的串口文件指针
//      Mode，打开模式，O_RDONLY、O_WRONLY、O_RDWR
//      timeout，超时时间。
// 返回：0=success，-1=error
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
//============================================================================
s32 CUSTOM_OpenMODEM(struct objhandle *hdl, u32 Mode, u32 timeout)
{
//    USBH_HandleTypeDef *host;

    if(hdl == NULL)
        return (-1);

//    host = (USBH_HandleTypeDef *)Device_GetDrvTag(Handle2fd(hdl));
//    Handle_SetContext(hdl, (ptu32_t)host);  //Fd上下文指向串口控制块struct UartGeneralCB
    return 0;
}

// ============================================================================
// 功能：通过MODEM接口发送数据
// 参数：pBuf -- 命令；dwLen -- 命令字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际发送字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
//u32 CUSTOM_WriteMODEM(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
//s32 CUSTOM_WriteMODEM(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
s32 CUSTOM_WriteMODEM(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u8 *buf = pBuf;
    u32 len = dwLen;
    u32 timeout = dwTimeout;

    if(!host)
    {
        printf("\r\n: warn : usb    : modem device(channel) is not register.");
        return (0);
    }

    __CUSTOM_Jobup();
    if(HOST_SUSPENDED == host->gState)
    {
        if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
        {
            CUSTOM_Resume();
        }
        else
        {
            __CUSTOM_Jobdown();
            return (0);
        }
    }

    USBH_CUSTOM_MODEM_Transmit(host, buf, &len, timeout);

    __CUSTOM_Jobdown();
    if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
    {
        //CUSTOM_Suspend();
        // 立即读，华为模块必须这样，因为进入睡眠模式，其不缓存数据
        Lock_SempPost(custom.pSleep);
    }

    return (dwLen - len);
}

// ============================================================================
// 功能：通过MODEM接口接收数据
// 参数：pBuf -- 数据缓冲；dwLen -- 数据字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际接收字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
// u32 CUSTOM_ReadMODEM(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, u32 dwTimeout)
//s32 CUSTOM_ReadMODEM(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, u32 dwTimeout)
s32 CUSTOM_ReadMODEM(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u32 len, bLen;

    if(!host)
    {
//        USBH_UsrLog("\r\nCUSTOM(USB) : error : UMODEM driver is not register.\r\n");
        return (0);
    }

    if((SLEEP == custom.curMode) || (SLEEP == custom.toMode))
    {
        len = __CUSTOM_ReadFromTaskMODEM(pBuf, dwLen, dwTimeout);
        return (len);
    }
    else
    {
        __CUSTOM_Jobup();
        if(HOST_SUSPENDED == host->gState)
        {
            __CUSTOM_Jobdown();
            return (0);
        }

        // 先从buffer把数据取光
        bLen = __CUSTOM_ReadFromTaskMODEM(pBuf, dwLen, 0);
        if(dwLen == bLen)
        {
            __CUSTOM_Jobdown();
            return (dwLen);     //应伟胜要求改
        }

        len = dwLen - bLen; // 剩余字节数
        dwLen = len;
        USBH_CUSTOM_MODEM_Receive(host, pBuf, &len, dwTimeout);
        __CUSTOM_Jobdown();
        len = dwLen - len; // 读取的自己数

        return (bLen + len);
    }

    return (0);
}

//============================================================================
//功能：打开DEBUG接口
// 参数：hdl，待操作的串口文件指针
//      Mode，打开模式，O_RDONLY、O_WRONLY、O_RDWR
//      timeout，超时时间。
// 返回：0=success，-1=error
// 备注：O_RDONLY、O_WRONLY、O_RDWR 须符合POSIX（2016）的最新定义
//============================================================================
s32 CUSTOM_OpenDEBUG(struct objhandle *hdl, u32 Mode, u32 timeout)
{
//    USBH_HandleTypeDef *host;

    if(hdl == NULL)
        return (-1);

//    host = (USBH_HandleTypeDef *)Device_GetDrvTag(Handle2fd(hdl));
//    Handle_SetContext(hdl, (ptu32_t)host);  //Fd上下文指向串口控制块struct UartGeneralCB
    return 0;
}


// ============================================================================
// 功能：通过DEBUG接口发送数据
// 参数：pBuf -- 命令；dwLen -- 命令字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际发送字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
// u32 CUSTOM_WriteDEBUG(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
//s32 CUSTOM_WriteDEBUG(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, bool_t dwBlock, u32 dwTimeout)
s32 CUSTOM_WriteDEBUG(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u8 *command = pBuf;
    u32 len = dwLen;
    u32 timeout = dwTimeout;

    if(!host)
    {
        printf("\r\n: warn : usb    : custom debug device(channel) is not register.\r\n");
        return (0);
    }

    if(HOST_SUSPENDED == host->gState)
    {
        return (0);
    }

    USBH_CUSTOM_DEBUG_Transmit(host, command, &len, timeout);

    return (dwLen - len);
}

// ============================================================================
// 功能：通过DEBUG接口接收数据
// 参数：pBuf -- 数据缓冲；dwLen -- 数据字节长；dwOffset（未用）；dwTimeout -- 等待完成时间。
// 返回：实际接收字节数；
// 备注：dwTimeout为零，则采用默认的等待时间。
// ============================================================================
// u32 CUSTOM_ReadDEBUG(ptu32_t pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, u32 dwTimeout)
//s32 CUSTOM_ReadDEBUG(struct objhandle *pUSB, u8 *pBuf, u32 dwLen, u32 dwOffset, u32 dwTimeout)
s32 CUSTOM_ReadDEBUG(struct objhandle *hdl, u8 *pBuf, u32 dwLen, u32 offset, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u8 *command = pBuf;
    u32 len = dwLen;
    u32 timeout = dwTimeout;

    if(!host)
    {
        printf("\r\n: warn : usb    : \"modem\" device channel is not register.");
        return (0);
    }

    if(HOST_SUSPENDED == host->gState)
    {
        return (0);
    }

    USBH_CUSTOM_DEBUG_Receive(host, command, &len, timeout);

    return (dwLen - len);
}

// ============================================================================
// 功能：查询模块状态
// 参数:
// 返回：0 -- 正常运行；1 -- 模块处于挂起状态；-1 -- 未就绪；-2 -- 模块处于阻塞
// 备注:
// ============================================================================
s32 CUSTOM_DeviceReady(void)
{
    if(BLOCK == custom.status)
        return (-2);

    return (USBH_Query(custom.pHandle));
}

// ============================================================================
// 功能：注册CUSTOM USB设备
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 CUSTOM_RegisterDev(USBH_HandleTypeDef *pHost)
{
    // struct DjyDevice *dev;
    // struct Object *dev;
    struct Object *res = NULL;
    s32 ret = 0;
    static u16 thread = -1;

    res = Device_Create((const char*)dev_name[0], CUSTOM_OpenAT, NULL,
                  CUSTOM_WriteAT, CUSTOM_ReadAT, NULL, 0);
    if(res)
    {
        printf("\r\n: warn : usb    : cannot register \"at\" device(channel).");
        ret = -1;
    }

    res = Device_Create((const char*)dev_name[1], CUSTOM_OpenMODEM, NULL,
            CUSTOM_WriteMODEM, CUSTOM_ReadMODEM, NULL, 0);
    if(res)
    {
        printf("\r\n: warn : usb    : cannot register \"modem\" device(channel).");
        ret = -1;
    }


    res = Device_Create((const char*)dev_name[2], CUSTOM_OpenDEBUG, NULL,
            CUSTOM_WriteDEBUG, CUSTOM_ReadDEBUG, NULL, 0);
    if(res)
    {
        printf("\r\n: warn : usb    : cannot register debug device(channel).");
        ret = -1;
    }

    if(!custom.pLock)
    {
        custom.pLock = (void*)Lock_MutexCreate("custom class(USB)");
        if(!custom.pLock)
        {
            printf("\r\n: erro : usb    : cannot create lock for custom class.");
            ret = -1;
        }
    }

    if(!custom.pSleep)
    {
        custom.pSleep = (void*)Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "custom class(USB) sleep signal");
        if(!custom.pSleep)
        {
            printf("\r\n: erro : usb    : cannot create sleep signal for custom class.");
            ret = -1;
        }
    }

    // 建立用于sleep模式的线程
    if(CN_EVTT_ID_INVALID == thread)
    {
        thread = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_RRS, 0, 0,
                                __CUSTOM_TaskRead, NULL, 0x600, "read task for custom class(USB)  sleep mode");
        if(CN_EVTT_ID_INVALID == thread)
        {
            printf("\r\n: erro : usb    : cannot register read custom class task for sleep mode.\r\n");
            ret = -1;
        }
    }

    if(CN_EVTT_ID_INVALID != thread)
    {
        if(TASK_OFF == custom.status)
        {
            DJY_EventPop(thread, NULL, 0, (ptu32_t)&custom, (ptu32_t)&defaultCycle, 0);
            custom.status = TASK_ON;
        }
    }

    return (ret); //
}

// ============================================================================
// 功能：注销USB设备
// 参数：
// 返回：
// 备注：注销的成员是SLEEP模式与非SLEEP模式都会用到的信息。
// ============================================================================
s32 CUSTOM_UNRegisterDev(USBH_HandleTypeDef *pHost)
{
    s32 res = 0;
    u8 i;

    for(i = 0; i < DEVICES; i++)
    {
#if 0
        if(custom.dev[i].pDev)
        {
            if(FALSE == DevDel(custom.dev[i].pDev))
            {
                USBH_UsrLog("\r\nCUSTOM(USB) : error : cannot unregister device <%s> .\r\n", dev_name[i]);
                res = -1;
            }
            else
            {
                custom.dev[i].pDev = NULL;
            }
        }
#else
        if(Device_DeleteByName(dev_name[i]))
            res = -1;
#endif
    }

#if 0
    // 这几个锁在好几处需要使用，为防止异常，就不将其注销
    if(custom.pLock)
    {
        if(Lock_MutexDelete(custom.pLock))
        {
            custom.pLock = NULL;
        }
        else
        {
            USBH_UsrLog("\r\nCUSTOM(USB) : error : cannot delete lock for USB custom device.\r\n");
            res = -1;
        }
    }


    if(custom.pSleep)
    {
        if(Lock_MutexDelete(custom.pSleep))
        {
            custom.pSleep = NULL;
        }
        else
        {
            USBH_UsrLog("\r\nCUSTOM(USB) : error : cannot delete sleep signal.\r\n");
            res = -1;
        }
    }
#endif
    return (res);
}

// ============================================================================
// 功能: 注册USB设备
// 参数:
// 返回:
// 备注：无线模块相关接口设备
// ============================================================================
void USBH_SetCUSTOM(USBH_HandleTypeDef *pHost)
{
    CUSTOM_RegisterDev(pHost);
    custom.pHandle = pHost;
    custom.curMode = STOP;
//    custom.toMode = NORMAL;
}

// ============================================================================
// 功能：注销USB设备
// 参数：
// 返回：
// 备注：
// ============================================================================
void USBH_ResetCUSTOM(USBH_HandleTypeDef *pHost)
{
    if(pHost == custom.pHandle)
    {
        custom.pHandle = NULL;
        custom.curMode = STOP;
//        custom.toMode = STOP;
        CUSTOM_UNRegisterDev(pHost);
    }
}
#if 0
// ============================================================================
// 功能：开启模块低功耗逻辑
// 参数：dwLogic -- (1)立即进入睡眠；（3）USB自行决定何时进入睡眠；
// 返回：
// 备注：
// ============================================================================
void CUSTOM_SleepEnable(u32 dwLogic)
{
    if((0x1 & dwLogic) && (!custom.bSleepModeEnable))
    {
        CUSTOM_HandleTypeDef *inner;

        custom.bSleepModeEnable = 1;
        if(custom.pHandle && custom.pHandle->pActiveClass)
        {
            inner = (CUSTOM_HandleTypeDef*)custom.pHandle->pActiveClass->pData;
            inner->dwMode = 1;
        }
    }

    if(0x2 & dwLogic)
        return;

    if(!custom.bSleepModeEnable)
        return;

    if(SLEEP == custom.mode)
        return;

    custom.mode = SLEEP;

    CUSTOM_Suspend(); // 此时sleep线程运行中，suspend就不会成功
    Lock_SempPost(custom.pSleep);
}

// ============================================================================
// 功能：关闭模块低功耗逻辑
// 参数：dwLogic -- 关闭低功耗逻辑（1）
// 返回：0 -- 未关闭；1 -- 关闭成功；
// 备注：
// ============================================================================
s32 CUSTOM_SleepDisable(u32 dwLogic)
{
    if(!custom.bSleepModeEnable)
        return (0); // 还未开启此逻辑

    if(dwLogic)
    {
        CUSTOM_HandleTypeDef *inner;

        custom.bSleepModeEnable = 0;
        if(custom.pHandle && custom.pHandle->pActiveClass)
        {
            inner = (CUSTOM_HandleTypeDef*)custom.pHandle->pActiveClass->pData;
            inner->dwMode = 0;
        }
    }

    if(NORMAL == custom.mode)
        return (0);

    custom.mode = NORMAL;
    CUSTOM_Resume(); // 先恢复USB
    Lock_SempPost(custom.pSleep);

    return (1);
}
#endif
#if 0
// ============================================================================
// 功能：控制CUSTOM设备的运行模式
// 参数：dwSettings -- （0x1）正常模式；(0x2)低功耗模式；
// 返回：
// 备注：
// ============================================================================
#define LOWPOWER_MODE        0x2
#define NORMAL_MODE            0x1
s32 CUSTOM_ModeSet(u32 dwSettings)
{
    CUSTOM_HandleTypeDef *classHandle;

    if((!custom.pHandle) || (custom.pHandle->pActiveClass))
    {
        return (-1);
    }

    classHandle = custom.pHandle->pActiveClass;

    switch(dwSettings)
    {
        case LOWPOWER_MODE:
                if(0 == classHandle->dwMode)
                    classHandle->dwMode = 0;

                if(SLEEP != custom.mode)
                    custom.mode = SLEEP;

                break;

        case NOMAL_MODE:
                if(!classHandle->dwMode)
                    classHandle->dwMode = 1;

                if(NORMAL != custom.mode)
                    custom.mode = NORMAL;

                break;

        default:
                break;
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 CUSTOM_IntoSleep(void)
{

}
#else

// ============================================================================
// 功能：允许CUSTOM设备的工作模式
// 参数：bArgC -- 参数个数；
//      参数1 -- (0x0)正常模式，缺省；（0x1）睡眠模式。
// 返回：成功（0）；失败（-1）；
// 备注：控制读写端口时的逻辑，睡眠模式下，读写端口是不阻塞的。以防止系统进入低功耗模式。
// ============================================================================
s32 CUSTOM_PermitWorkMode(u8 bArgC, ...)
{
    va_list ap;
    tagMode mode;
    CUSTOM_HandleTypeDef *classHandle;

    if(!custom.pHandle || !custom.pHandle->pActiveClass ||
       !custom.pHandle->pActiveClass->pData)
    {
        return (-1);
    }

    va_start(ap, bArgC);
    mode = (tagMode)va_arg(ap, u32);
    va_end(ap);

    classHandle = (CUSTOM_HandleTypeDef*)custom.pHandle->pActiveClass->pData;

    switch(mode)
    {
        case SLEEP: classHandle->dwMode = 1; break;
        case NORMAL: classHandle->dwMode = 0; break;
        default: break;
    }

    return (0);
}

// ============================================================================
// 功能：切换CUSTOM设备的运行模式
// 参数：bArgC -- 参数个数；
//      参数1 -- (0)正常模式；（1）睡眠模式。
//      参数2 -- 阻塞执行（1）；
//      参数3 -- 阻塞时限，单位秒；
// 返回：成功（0）；失败（-1）；超时（-2）；
// 备注：
// ============================================================================
s32 CUSTOM_ChangeToMode(u8 bArgC, ...)
{
    u8 i;
    va_list ap;
    u32 timeout = 60, block = 0;
    tagMode changeTo;

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0: changeTo = (tagMode)va_arg(ap, u32);break;
            case 1: block = va_arg(ap, u32);break;
            case 2: timeout = va_arg(ap, u32);break;
            default: break;
        }
    }
    va_end(ap);

    if(((SLEEP == custom.curMode) && (SLEEP == changeTo)) ||
        ((NORMAL == custom.curMode) && (NORMAL == changeTo)))
    {
        return (0);
    }

    switch(changeTo)
    {
        case SLEEP : custom.toMode = changeTo; break;
        case NORMAL: custom.toMode = changeTo; break;
        default: return (-1); // 未支持模式
    }

    Lock_SempPost(custom.pSleep);

    // 这个点，如果发生了断线???
    if(block && (timeout))
    {
        if(-1 != (s32)timeout)
            timeout = DJY_GetSysTime() + timeout*1000*mS;

        if(changeTo == custom.curMode)
            return (0);

        while(1)
        {
            DJY_EventDelay(10*mS);

            if(changeTo == custom.curMode)
                return (0);

            if((-1 != (s32)timeout) && DJY_GetSysTime() > timeout)
                return (-2);
        }
    }

    return (0);
}
#endif
// ============================================================================
// 功能：设置休眠时长
// 参数： dwTime -- 时间长度，单位为毫秒
// 返回：
// 备注：
// ============================================================================
void CUSTOM_SetSleepCycle(u32 dwTime)
{
    custom.dwSleepCycle = dwTime;
}

// ============================================================================
// 功能：设置唤醒时长
// 参数：dwTime -- 时间长度，单位为毫秒
// 返回：
// 备注：
// ============================================================================
void CUSTOM_SetWakeCycle(u32 dwTime)
{
    custom.dwWakeCycle = dwTime;
}

// ============================================================================
// 功能：设置设备为直接阻塞模式
// 参数：
// 返回：
// 备注：函数是阻塞型的，直至设置成功返回。
// ============================================================================
void CUSTOM_DirectBlock(void)
{
    __CUSTOM_Lockup();

    // 等待所有操作结束，并且处于非BLOCK状态下
    while(__CUSTOM_Job() || (BLOCK == custom.status))
        DJY_EventDelay(5000);

    custom.dwBackup = custom.status;
    custom.status = BLOCK;

    __CUSTOM_Lockdown();
}

// ============================================================================
// 功能：取消设备的直接阻塞模式
// 参数：
// 返回：
// 备注：
// ============================================================================
void CUSTOM_DirectUnblock(void)
{
    __CUSTOM_Lockup();

    if(BLOCK == custom.status)
    {
        custom.status = custom.dwBackup;
        custom.dwBackup = BLOCK;
    }

    __CUSTOM_Lockdown();
}

// ============================================================================
// 功能：直接访问AT通道（写）
// 参数：pBuf -- 写缓冲；dwLen -- 写长度；dwTimeout -- 写超时。
// 返回：实际写长度。
// 备注：
// ============================================================================
u32 __CUSTOM_DirectWriteAT(u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u32 len = dwLen;

    USBH_CUSTOM_AT_Transmit(host, pBuf, &len, dwTimeout);

    return (dwLen - len);
}

// ============================================================================
// 功能：直接访问AT通道（读）
// 参数：pBuf -- 读缓冲；dwLen -- 读长度；dwTimeout -- 读超时。
// 返回：实际读长度。
// 备注：
// ============================================================================
u32 __CUSTOM_DirectReadAT(u8 *pBuf, u32 dwLen, u32 dwTimeout)
{
    USBH_HandleTypeDef *host = custom.pHandle;
    u32 len = dwLen;

    USBH_CUSTOM_AT_Receive(host, pBuf, &len, dwTimeout);

    return (dwLen - len);
}

// ============================================================================
// 功能：直接访问AT通道时（读）保存不相关的数据
// 参数：pBuf -- 数据；dwLen -- 数据长度；
// 返回：实际保存的数据长度；
// 备注：
// ============================================================================
u32 __CUSTOM_SaveUnrelatedAT(u8 *pBuf, u32 dwLen)
{
    u32 rLen;

    rLen = Ring_Write(custom.dev[0].pRingRd, pBuf, dwLen);
    if(dwLen != rLen)
    {
        if(!((1 == dwLen) && ((*pBuf == '\r') || (*pBuf = '\n')))) // 在AT通道'\r'、'\n'
        {
            printf("\r\n: erro : usb    : \"at\" device(channel)'s ring buffer overflow. %d bytes lost\r\n", (dwLen-rLen));
        }
    }
    Lock_SempPost(custom.dev[0].pSemp);

    return (rLen);
}

// ============================================================================
// 功能：获取模块的信息
// 参数：
// 返回：模块信息
// 备注：
// ============================================================================
char *CUSTOM_GetInfos(void)
{
    if(custom.pHandle)
    {
        CUSTOM_HandleTypeDef *inner;

        inner = (CUSTOM_HandleTypeDef*)custom.pHandle->pActiveClass->pData;
        return inner->pInfos;
    }

    return (NULL);
}
