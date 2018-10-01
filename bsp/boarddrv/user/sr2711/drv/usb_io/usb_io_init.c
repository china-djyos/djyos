//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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
#include <int.h>
#include <string.h>

// 管脚定义在board.c之中
extern const Pin USB_FS[2];
extern const Pin USB_HS[2];
extern const Pin M4G_RST[1];
extern const Pin M4G_WAKEUP[1];
extern const Pin M4G_POWER[1];
extern const Pin M4G_WAKEUP_INPUT[1];

extern char *CUSTOM_GetInfos(void);
void __M4G_Wakeup(void);
void __M4G_Sleep(void);


#define SUPPORTS                    4 // 目前支持的品牌

struct __sleepCommands{
    char *pBrand;
    u32 dwCount;
    char *pCommand[2];
};

struct __sleepCommands brands[] = {
    {"HUAWEI", 2, {"AT^WAKEUPCFG=0,0,0 \r", "AT^CURC=0 \r"}},
    {"SIMCOM", 2, {"AT+CSCLK=1 \r", "AT&D0 \r"}},
    {"FORGE", 1, {"AT+SLEEPEN=1 \r"}},
    {"QUECTEL", 1, {"AT+QSCLK=1 \r"}}
};

static u8 configLowPower = 1; // 每次复位后，系统重新进入低功耗时，需要提前设置


// ============================================================================
// 功能：USB IO控制逻辑
// 参数：
// 返回：
// 备注：
// ===========================================================================
void USB_IOs(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(M4G_RST, PIO_LISTSIZE(M4G_RST));
    PIO_Set(&M4G_RST[0]);

    __HAL_RCC_GPIOK_CLK_ENABLE();
    PIO_Configure(M4G_POWER, PIO_LISTSIZE(M4G_POWER));
    PIO_Set(&M4G_POWER[0]);

    __HAL_RCC_GPIOH_CLK_ENABLE();
    PIO_Configure(M4G_WAKEUP, PIO_LISTSIZE(M4G_WAKEUP));
    PIO_Clear(&M4G_WAKEUP[0]);
}

// ============================================================================
// 功能：USB设备复位逻辑
// 参数：
// 返回：
// 备注：通过复位管脚
// ============================================================================
void USB_DeviceReset(u8 bHostID, u8 bDevID)
{
    if((1 == bHostID) && (0 == bDevID))
    {
        // 4G模块复位
        //Djy_EventDelay(10000*mS);
#if 0
        PIO_Clear(&M4G_RST[0]); // reset device
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_RST[0]); // release the reset
        Djy_EventDelay(20000*mS); // wait for device ready
#else
        PIO_Clear(&M4G_POWER[0]);
        Djy_EventDelay(1000*mS);
        PIO_Set(&M4G_POWER[0]);
        Djy_EventDelay(20000*mS); // wait for device ready
#endif
        configLowPower = 1;
    }
    else if((2 == bHostID) && (0 == bDevID))
    {
        // USB_FS没有复位逻辑
    }
}

// ============================================================================
// 功能：USB设备唤醒MCU的中断服务
// 参数：
// 返回：
// 备注：未使用，无法实现功能
// ============================================================================
static ptu32_t __M4G_IrqService(u8 bLine)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
    }

    Int_ClearLine(CN_INT_LINE_EXTI15_10);

    return (1);
}

// ============================================================================
// 功能：USB设备唤醒MCU的中断初始化
// 参数：
// 返回：
// 备注：未使用，无法实现功能
// ============================================================================
static void __M4G_IrqInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_13; // PC13为4G模块唤醒单片机

    GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed= GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    Int_Register(CN_INT_LINE_EXTI15_10);
    Int_SetClearType(CN_INT_LINE_EXTI15_10, CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_EXTI15_10, (uint32_t(*)(ptu32_t))__M4G_IrqService);
    Int_SettoReal(CN_INT_LINE_EXTI15_10);
    Int_ClearLine(CN_INT_LINE_EXTI15_10);
    Int_RestoreRealLine(CN_INT_LINE_EXTI15_10);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_UserInstall(u8 bHostID)
{
    if(0x1 == bHostID)
    {
        // __M4G_IrqInit(); // HS上的4G模块的唤醒中断逻辑.
    }
    else if(0x2 == bHostID)
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
void USB_UserSuspend(u16 wHostID)
{

    if(0x1 == (wHostID & 0xFF))
    {
        __M4G_Sleep();
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void USB_UserResume(u16 wHostID)
{
    if(0x1 == (wHostID & 0xFF))
    {
        __M4G_Wakeup();
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
//  __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    PIO_Configure(USB_HS, PIO_LISTSIZE(USB_HS));
}

// ============================================================================
// 功能：命令回复检查
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __CheckResponse(char *pCommand)
{
    u32 i, j, res;
    u32 responsed;
    char response = 0;
    char buffer[128];
    u32 wait = 200;
    u32 cLen = strlen(pCommand);
    char OK[2] = {'O', 'K'};
    char ERROR[5] = {'E', 'R', 'R', 'O', 'R'};
    extern u32 __CUSTOM_DirectReadAT(u8 *pBuf, u32 dwLen, u32 dwTimeout);
    extern u32 __CUSTOM_SaveUnrelatedAT(u8 *pBuf, u32 dwLen);

    i = 0;
    responsed = 0;
    while(1)
    {
        res = __CUSTOM_DirectReadAT((u8*)&response, 1, 0);
        if(!res)
        {
            if(!wait--)
            {
                printk("\r\nUSB IO : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));
                return (-1);
            }

            Djy_DelayUs(1000);
            continue;
        }

        responsed++;
        if(pCommand[i] == response)
        {
            buffer[i] = response;
            i++;
        }
        else
        {
            if(i)
            {
                __CUSTOM_SaveUnrelatedAT((u8*)buffer, i);
                i = 0; // 重置
            }
            __CUSTOM_SaveUnrelatedAT((u8*)&response, 1);
        }

        if(i == cLen)
            break;
    }

    i = 0;
    j = 0;
    while(1)
    {
        if(OK[i] == response)
        {
            if(++i == 2)
                break;

        }
        else if(ERROR[j] == response)
        {
            if(++j == 5)
            {
                printk("\r\nUSB IO : error : \"%s\" has error response.\r\n", pCommand);
                return (1);
            }
        }
        else
        {
            i = 0; // 重置
            j = 0;
        }

        res = __CUSTOM_DirectReadAT((u8*)&response, 1, 0);
        if(!res)
        {
            if(!wait--)
            {
                printk("\r\nUSB IO : error : \"%s\" has %s response.\r\n", pCommand, (responsed?"error":"no"));
                return (-1);
            }

            Djy_DelayUs(1000);
            continue;
        }

        responsed++;
    }

    return (0);
}

// ============================================================================
// 功能：4G模块退出睡眠
// 参数：
// 返回：
// 备注：唤醒操作存在次序
// ============================================================================
void __M4G_Wakeup(void)
{
    ;// 没有什么要做的
}

// ============================================================================
// 功能：4G模块进入睡眠
// 参数：
// 返回：
// 备注：睡眠操作存在次序
// ============================================================================
void __M4G_Sleep(void)
{
    extern u32 __CUSTOM_DirectWriteAT(u8 *pBuf, u32 dwLen, u32 dwTimeout);
    u8 commandLen, i, j;
    u32 res;

    if(configLowPower)
    {
        char *workingBrand = CUSTOM_GetInfos();
        configLowPower = 0;

        if(!workingBrand)
            return ; // 模块没有信息

        for(i = 0; i < SUPPORTS; i++)
        {
            if(strstr(workingBrand, brands[i].pBrand))
            {
                break;
            }
        }

        if(i == SUPPORTS)
        {
            return; // 目前不支持该器件
        }

        // 配置进入模块进入低功耗逻辑
        for(j = 0; j < brands[i].dwCount; j++)
        {
            commandLen = (strlen(brands[i].pCommand[j]) + 1);
            res = __CUSTOM_DirectWriteAT((u8*)(brands[i].pCommand[j]), commandLen, 1000); //
            brands[i].pCommand[j][commandLen-2] = '\0';
            if(res != commandLen)
            {
                printk("\r\nUSB IO : error : \"%s\" set(send) failed during sleep.\r\n", brands[i].pCommand[j]);
            }

            res = __CheckResponse((brands[i].pCommand[j]));
            if(res)
            {
                printk("\r\n\"%s\" set(response) failed during sleep.\r\n", brands[i].pCommand[j]);
            }
        }
    }
}
