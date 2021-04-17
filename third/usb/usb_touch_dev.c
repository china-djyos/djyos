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
#include <stdio.h>
#include "usb.h"


extern s32 USBH_HID_TouchGetData(USBH_HandleTypeDef *pHost, u8 *pButton, u32 *pX, u32 *pY);
USBH_HandleTypeDef *pTouch;
static char *touchname = "usb touch";
// ============================================================================
// 功能：读触摸屏设备文件
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __TOUCH_DriRead(struct objhandle *pUSB, u8 *data, u32 dwLen, u32 wdOffset, u32 dwTimeout)
{
    s32 res;
    u8 *pBuf = data;

    if((9 != dwLen) || (0 != wdOffset) || (!pUSB)) // 由于touch并非流媒体，每次读的大小读是固定的
        return (0);

    res = USBH_HID_TouchGetData((void*)Device_GetDrvTag(Handle2fd(pUSB)), pBuf, (u32*)(pBuf+1), (u32*)(pBuf+5));
    if(-1 == res)
        return (0);

    return (9);
}

// ============================================================================
// 功能：读触摸屏
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 TOUCH_DirectRead(u8 *pButton, u32 *pX, u32 *pY)
{
    s32 res;

    if(!pTouch)
    {
        printf("\r\n: erro : usb    : touch device do not exist.");
        return (0);
    }

    res = USBH_HID_TouchGetData(pTouch, pButton, pX, pY);
    if(-1 == res)
        return (0);

    return (9);
}

// ============================================================================
// 功能：触摸屏设备是否就绪
// 参数：
// 返回：0 -- 设备就绪； -1 -- 设备未就绪。
// 备注：
// ============================================================================
s32 TOUCH_DeviceReady(void)
{
    return (USBH_Query(pTouch));
}

// ============================================================================
// 功能：注册触摸屏
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 USBH_ResigerDevTouch(void *pParam)
{

    if(NULL == Device_Create((const char*)touchname, NULL, NULL,NULL,
            __TOUCH_DriRead, NULL, (ptu32_t)pParam))
    {
        printf("\r\n: erro : usb    : register touch device failed.");
        return (-1); // register failure
    }

    return (0); // register failure
}

// ============================================================================
// 功能：注销触摸屏
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 USBH_UnResigerDevTouch(void)
{
    if(Device_DeleteByName(touchname))
    {
        printf("\r\n: erro : usb    : unregister touch device failed.\r\n");
        return (-1); // register failure
    }

    return (0);
}

// ============================================================================
// 功能：设置触摸屏
// 参数：
// 返回：
// 备注：
// ============================================================================
void USBH_SetTouch(void *pHost)
{
    if((pTouch) || (!pHost))
    {
        printf("\r\n: erro : usb    : cannot set touch device.\r\n");
        return ;
    }

    if(!USBH_ResigerDevTouch(pHost))
        pTouch = pHost;
}

// ============================================================================
// 功能：重置触摸屏
// 参数：
// 返回：
// 备注：
// ============================================================================
void USBH_ResetTouch(void *pHost)
{
    if(pHost == pTouch)
    {
        if(!USBH_UnResigerDevTouch())// 注销设备
            pTouch = NULL;
    }
}
