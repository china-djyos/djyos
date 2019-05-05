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
#include <ring.h>
#include <stdlib.h>
#include <string.h>
#include <lock.h>
#include "./stm32_usb_host_library/core/inc/usbh_def.h"
#include "usbh_lowlevel.h"
#include <djyos.h>

#define BUFFER_SIZE                128

typedef struct channel{
    struct RingBuf *pChannel;
    struct channel *pNext;
    u8 bPipe; // channel 对应的pipe号
    struct SemaphoreLCB *pSemp; // 是否有数据
}tagChannel;

tagChannel *pChannels;
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static tagChannel *__FindChannel(u8 bPipe)
{
    tagChannel *channel = pChannels;

    for(;;)
    {
        if(!channel)
            return (NULL); // 结束，未找到

        if(bPipe == channel->bPipe)
            return (channel); // 找到

        channel = channel->pNext;
    }

    return (NULL);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void __AddChannel(tagChannel * channel)
{

    if(!pChannels)
    {
        pChannels = channel; // 队列本身为空，则将本成员作为第一个
    }
    else
    {
        // 将本成员插入第一个的后面
        channel->pNext = pChannels->pNext;
        pChannels->pNext = channel;
    }
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//----------------------------------------------------------------------------
static tagChannel *__DelChannel(u8 bPipe)
{
    tagChannel *channel = pChannels;
    tagChannel *pre = NULL;

    for(;;)
    {
        if(!channel)
            return (NULL); // 结束，未找到

        if(bPipe == channel->bPipe)
        {
            if(pre)
                pre->pNext = channel->pNext; // 将本成员从队列中剔除
            else
                pChannels = channel->pNext; // 本成员是队列中第一个

            return (channel);
        }

        pre = channel;
        channel = channel->pNext;
    }

    return (NULL);
}

//-----------------------------------------------------------------------------
//功能: 创建异步缓冲
//参数:
//返回:
//备注: 构建一个ring buffer用于存放缓冲数据
//-----------------------------------------------------------------------------
static tagChannel *__CreateChannel(u8 bPipe)
{
    u32 size;
    tagChannel *res;
    struct RingBuf *ring = NULL;
    u8 *space = NULL;
    struct SemaphoreLCB *semp = NULL;

    size = sizeof(tagChannel);
    res = malloc(size);
    if(!res)
        goto FAIL;

    memset(res, 0x0, size);

    ring = malloc(sizeof(struct RingBuf));
    if(!ring)
        goto FAIL;

    space = malloc(BUFFER_SIZE);
    if(!space)
        goto FAIL;

    semp = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, NULL);
    if(!semp)
        goto FAIL;

    Ring_Init(ring, space, BUFFER_SIZE);
    res->pChannel = ring;
    res->bPipe = bPipe;
    res->pSemp = semp;

    return (res);

FAIL:
    if(res)
        free(res);

    if(ring)
        free(ring);

    if(space)
        free(space);

    if(semp)
        Lock_SempDelete(semp);

    return (NULL);
}

//-----------------------------------------------------------------------------
//功能: 释放异步缓冲
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void __DestroyChannel(tagChannel *pChannel)
{
    if(pChannel->pChannel)
    {
        if(pChannel->pChannel->buf)
            free(pChannel->pChannel->buf);

        free(pChannel->pChannel);
    }

    Lock_SempDelete(pChannel->pSemp);
    free(pChannel);
}

//-----------------------------------------------------------------------------
//功能: 创建异步缓冲
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
s32 USBH_OpenAsync(USBH_HandleTypeDef *pHost, u8 bPipe)
{
    tagChannel *channel;

    channel = __FindChannel(bPipe);
    if(channel)
        return (0); // pipe已经存在

    channel = __CreateChannel(bPipe);
    if(!channel)
        return (-1);

    __AddChannel(channel);

    USBH_LL_EnAsync(pHost, bPipe);
    return (0);
}
//-----------------------------------------------------------------------------
//功能: 释放异步缓冲
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
s32 USBH_CloseAsync(USBH_HandleTypeDef *pHost, u8 bPipe)
{
    tagChannel *channel;

    channel = __DelChannel(bPipe);
    if(!channel)
        return (0); // pipe不存在

    USBH_LL_DisAsync(pHost, bPipe);
    __DestroyChannel(channel);
    return (0);
}

//-----------------------------------------------------------------------------
//功能: 将数据存入异步缓冲
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void USBH_Store(u8 bPipe, u8 *pData, u32 dwLen)
{
    tagChannel *channel;
    u32 res;

    channel = __FindChannel(bPipe);
    if(!channel)
        return; // pipe不存在

    res = Ring_Write(channel->pChannel, pData, dwLen);
    if(res != dwLen)
    {
        USBH_UsrLog("\r\nUSB Module : error : channel %d buffer overflow.\r\n", bPipe);
    }

    Lock_SempPost(channel->pSemp); // 存在数据，设置信号
}

//-----------------------------------------------------------------------------
//功能: 从异步缓冲中获取数据
//参数: bPipe -- 通道号；pBuffer -- 数据缓冲；dwLen -- 读取字节数
//返回: 获取数据字节数
//备注:
//-----------------------------------------------------------------------------
u32 USBH_Fetch(u8 bPipe, u8 *pBuffer, u32 dwLen)
{
    u32 len;
    tagChannel *channel;

    channel = __FindChannel(bPipe);
    if(!channel)
        return (0); // pipe不存在

    len = Ring_Read(channel->pChannel, pBuffer, dwLen);

    if(!Ring_IsEmpty(channel->pChannel))
        Lock_SempPost(channel->pSemp); // 仍旧有数据可读，设置信号

    return (len);
}

//-----------------------------------------------------------------------------
//功能: 等待数据
//参数: bPipe -- 通道号；dwTimeout -- 等待时间；
//返回:
//备注:
//-----------------------------------------------------------------------------
void USBH_Wait(u8 bPipe, u32 dwTimeout)
{
    tagChannel *channel;

    channel = __FindChannel(bPipe);
    if(!channel)
        return ; // pipe不存在

    Lock_SempPend(channel->pSemp, dwTimeout);
}
