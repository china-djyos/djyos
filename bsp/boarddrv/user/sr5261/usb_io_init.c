//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <cpu_peri_gpio.h>
#include <stm32f7xx_hal.h>
#include <systime.h>

extern const Pin USB_FS[2];
extern const Pin USB_HS[2];
extern const Pin HUB_RST[1];
extern const Pin USB_LED1[1]; // USB运行状态指示灯
extern const Pin M4G_RST[1];
extern const Pin M4G_POWER[1];
extern const Pin HUB_POWER[1];
extern const Pin TOUCH_POWER[1];
extern void RunIboot(void);

// ============================================================================
// 功能：USB IAP状态灯亮
// 参数：
// 返回：
// ===========================================================================
void USB_IOs(void)
{
    PIO_Configure(USB_LED1, PIO_LISTSIZE(USB_LED1));  //USB LED1

    PIO_Configure(M4G_RST, PIO_LISTSIZE(M4G_RST));
    PIO_Set(&M4G_RST[0]);

    PIO_Configure(M4G_POWER, PIO_LISTSIZE(M4G_POWER));
    PIO_Set(&M4G_POWER[0]);

    PIO_Configure(HUB_RST, PIO_LISTSIZE(HUB_RST));
    PIO_Set(&HUB_RST[0]);

    PIO_Configure(HUB_POWER, PIO_LISTSIZE(HUB_POWER));
    PIO_Clear(&HUB_POWER[0]);
}

// ============================================================================
// 功能：USB IAP状态灯亮
// 参数：
// 返回：
// ============================================================================
void USB_IAP_TurnOnLED(void)
{
    PIO_Set(USB_LED1);
}

// ============================================================================
// 功能：USB IAP状态灯灭
// 参数：
// 返回：
// ============================================================================
void USB_IAP_TurnOffLED(void)
{
    PIO_Clear(USB_LED1);
}

// ============================================================================
// 功能：USB设备复位逻辑
// 参数：
// 返回：
// ============================================================================
void USB_UDiskReset(void)
{
    PIO_Clear(&HUB_RST[0]); // reset
    PIO_Set(&HUB_POWER[0]);
    Djy_EventDelay(1000*mS);

    PIO_Clear(&HUB_POWER[0]);
    PIO_Set(&HUB_RST[0]); // release the reset
    Djy_EventDelay(30*mS);

    RunIboot();
}
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    if((USB_4G_PORT == bHostID) && (0 == bDevID))
    {
        // 4G模块复位
        //Djy_EventDelay(10000*mS);
        PIO_Clear(&M4G_POWER[0]); // reset device
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_POWER[0]); // release the reset
        Djy_EventDelay(20000*mS); // wait for device ready
    }
    else if((USB_HUB_PORT == bHostID))
    {

        if(2 == bDevID)
        {
            PIO_Set(&TOUCH_POWER[0]); //
            Djy_EventDelay(10000*mS);
            PIO_Clear(&TOUCH_POWER[0]); //
            Djy_EventDelay(100*mS); //
            return;
        }
        else if (0 == bDevID)
        {
            // HUB复位
            #if 0
            PIO_Clear(&HUB_RST[0]); // reset
            Djy_EventDelay(10*mS);
            PIO_Set(&HUB_RST[0]); // release the reset
            Djy_EventDelay(100*mS); // wait for device ready
            #elif 0
            PIO_Set(&HUB_POWER[0]); //
            Djy_EventDelay(1000*mS);
            PIO_Clear(&HUB_POWER[0]); //
            Djy_EventDelay(100*mS); //
            #else
            PIO_Clear(&HUB_RST[0]); // reset
            PIO_Set(&HUB_POWER[0]);
            Djy_EventDelay(1000*mS);

            PIO_Clear(&HUB_POWER[0]);
            Djy_EventDelay(30*mS);
            PIO_Set(&HUB_RST[0]); // release the reset
            #endif
        }
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// ============================================================================
void USB_UserInstall(u8 bHostID)
{
    if(USB_HUB_PORT == bHostID) // HUB
    {
        extern s32 USBH_ResigerDevMSC(void);
        USBH_ResigerDevMSC();
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_FS_GPIO_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    PIO_Configure(USB_FS, PIO_LISTSIZE(USB_FS));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_HS_GPIO_Init(void)
{
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(USB_HS, PIO_LISTSIZE(USB_HS));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void M4G_PowerSet(void)
{
     PIO_Set(&M4G_POWER[0]);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void M4G_PowerClr(void)
{
    PIO_Clear(&M4G_POWER[0]);
}
