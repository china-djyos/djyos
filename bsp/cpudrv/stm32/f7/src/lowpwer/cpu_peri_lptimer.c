//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018 著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
// =============================================================================

// 文件名     ：cpu_peri_lptimer.c
// 模块描述: 低功耗定时器底层硬件驱动模块
// 模块版本:V1.0.0
// 创建人员:chj
// 创建时间:2018/6/14
// =============================================================================
#include "cpu_peri_lptimer.h"
#include "cpu_peri_int_line.h"
#include "cpu_peri.h"
#include "int.h"
#include "board-config.h"

#if (CN_USE_TICKLESS_MODE)
static u16 g_lptimer_reload = 0;
static u16 g_lptimer_pre_cnt = 0;
void Lptimer1_ClearAllInt(void)
{
    LPTIM1->ICR = 0x7f;
}

bool_t Lptimer1_GetIntFlag(void)
{
    return ((LPTIM1->ISR&LPTIM_ISR_CMPM) ? true : false);
}

u8 Lptimer1_ClearISR(void)
{
    u8 temp = CN_LPTIMER_NONE;
    if(LPTIM1->ISR & LPTIM_ISR_CMPM)
    {
        if(LPTIM1->ISR & LPTIM_ISR_ARRM)
        {
            LPTIM1->ICR |= LPTIM_ICR_ARRMCF | LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_RELOAD_AND_CMP;
        }
        else
        {
            LPTIM1->ICR |= LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_CMP;
        }
    }
    else
    {
        if(LPTIM1->ISR & LPTIM_ISR_ARRM)
        {
            LPTIM1->ICR |= LPTIM_ICR_ARRMCF;
            temp =  CN_LPTIMER_RELOAD;
        }
        else
        {
            LPTIM1->ICR = 0x7f;
            temp =  CN_LPTIMER_NONE;
        }
    }
    //Lptimer1_ClearAllInt();
    return temp;
}

void Lptimer1_set_period(u16 period)
{
    u16 temp = 0;
    u16 temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = LPTIM1->CNT;
        temp2 = LPTIM1->CNT;
    }

    g_lptimer_pre_cnt = temp1;
    temp = (period + g_lptimer_pre_cnt > (CN_LIMIT_UINT16)) ? \
            (period + g_lptimer_pre_cnt - CN_LIMIT_UINT16) : (period + g_lptimer_pre_cnt);
    LPTIM1->ICR |= (LPTIM_ICR_CMPOKCF | LPTIM_ICR_CMPMCF);
    LPTIM1->CMP = temp;
    g_lptimer_reload = period;
    while(!(LPTIM1->ISR & LPTIM_ISR_CMPOK));
}

u16 Lptimer1_read_reload(void)
{
    return g_lptimer_reload;
}

u16 Lptimer1_read_cnt(void)
{
    u16 temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = LPTIM1->CNT;
        temp2 = LPTIM1->CNT;
    }
    if(LPTIM1->ISR & LPTIM_ISR_ARRM)
        return (CN_LIMIT_UINT16 + temp1);
    return temp1;
}

void Lptimer1_PreInit(void)
{
    g_lptimer_reload = 0;
    g_lptimer_pre_cnt = 0;
}

void Lptimer1_Init(u16 period,void (*isr)(ptu32_t param))
{
    u32 presc=0;
    RCC_OscInitTypeDef          RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef    RCC_PeriphCLKInitStruct;

    presc = 32000/CN_CFG_TIME_BASE_HZ;
    switch(presc)
    {
        case 1:
            presc = (0x00 << (LPTIM_CFGR_PRESC_Pos));
            break;
        case 2:
            presc = LPTIM_CFGR_PRESC_0;
            break;
        case 4:
            presc = LPTIM_CFGR_PRESC_1;
            break;
        case 8:
            presc = (0x03 << (LPTIM_CFGR_PRESC_Pos));
            //presc = LPTIM_CFGR_PRESC_2;
            break;
        default:
            return;
    }
      /* Enable LSI clock */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LPTIM1;
    RCC_PeriphCLKInitStruct.Lptim1ClockSelection = RCC_LPTIM1CLKSOURCE_LSI;
    HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

    __HAL_RCC_LPTIM1_CLK_ENABLE();

    LPTIM1->CFGR = presc; // 分频
    LPTIM1->IER = LPTIM_IER_CMPMIE | LPTIM_IER_ARRMIE; // 开定时器中断

    Int_Register(CN_INT_LINE_LPTIM1);
    Int_SetClearType(CN_INT_LINE_LPTIM1,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_LPTIM1,isr);
    Int_SettoAsynSignal(CN_INT_LINE_LPTIM1);
    Int_ClearLine(CN_INT_LINE_LPTIM1);
    Int_RestoreAsynLine(CN_INT_LINE_LPTIM1);

    Lptimer1_ClearAllInt();
    LPTIM1->CR = LPTIM_CR_ENABLE; // 打开LPTIM, 但暂不开始计时
    LPTIM1->ARR = period;
    LPTIM1->CMP = period;
    Lptimer1_ClearAllInt();
    g_lptimer_reload = period;
    g_lptimer_pre_cnt = 0;
    LPTIM1->CR |= LPTIM_CR_CNTSTRT;
    while(LPTIM1->CNT==0);
}
#endif
