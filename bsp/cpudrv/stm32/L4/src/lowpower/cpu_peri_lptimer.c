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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
#include "stm32l433xx.h"
#include "int.h"
#include "board-config.h"

#if (CN_USE_TICKLESS_MODE)
#if CN_CFG_USE_USERTIMER
#define DJYBSP_LPTIMER_NUM      (1U)
static LPTIM_TypeDef *djybsp_lptimer_reg[DJYBSP_LPTIMER_NUM] = {LPTIM1};

#define DJYLPTIMER_DEBUG   (0U)
#if     DJYLPTIMER_DEBUG
struct djylptimer_debug_t{
    uint16_t pre_cnt;
    uint16_t pre_cmp;
}djylptimer_debug={
   0,
   0,
};
#endif

// =============================================================================
// 功能：清理所有中断标志位
// 参数：要清理的定时器号
// 返回：无
// =============================================================================
void DjyBsp_LptimerClearAllIsrFlag(uint8_t id)
{
    djybsp_lptimer_reg[id]->ICR = 0x7f;
}

// =============================================================================
// 功能：清理对应的中断标志位
// 参数：要清理的定时器号
// 返回：无
// =============================================================================
uint8_t DjyBsp_LptimerClearIsrFlag(uint8_t id)
{
    uint8_t temp = CN_LPTIMER_NONE;
    if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_CMPM)
    {
        if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_ARRMCF | LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_RELOAD_AND_CMP;
        }
        else
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_CMPMCF;
            temp =  CN_LPTIMER_CMP;
        }
    }
    else
    {
        if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        {
            djybsp_lptimer_reg[id]->ICR |= LPTIM_ICR_ARRMCF;
            temp =  CN_LPTIMER_RELOAD;
        }
        else
        {
            djybsp_lptimer_reg[id]->ICR = 0x7f;
            temp =  CN_LPTIMER_NONE;
        }
    }
    return temp;
}

// =============================================================================
// 功能：获取中断标志位
// 参数：定时器号
// 返回：无
// =============================================================================
bool_t DjyBsp_LptimerGetIsrFlag(uint8_t id)
{
    return ((djybsp_lptimer_reg[id]->ISR&LPTIM_ISR_CMPM) ? true : false);
}

// =============================================================================
// 功能：设置定时器重装载值
// 参数：id:定时器号,period:需要设置的值
// 返回：无
// =============================================================================
void DjyBsp_LptimerSetReload(uint8_t id,uint16_t period)
{
    uint16_t temp = 0;
    uint16_t temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = djybsp_lptimer_reg[id]->CNT;
        temp2 = djybsp_lptimer_reg[id]->CNT;
    }
#if     DJYLPTIMER_DEBUG
    djylptimer_debug.pre_cnt = temp1;
#endif
    temp = (period + temp1 > (CN_LIMIT_UINT16)) ? \
            (period + temp1 - CN_LIMIT_UINT16) : (period + temp1);
    djybsp_lptimer_reg[id]->ICR |= (LPTIM_ICR_CMPOKCF | LPTIM_ICR_CMPMCF);
    djybsp_lptimer_reg[id]->CMP = temp;
#if     DJYLPTIMER_DEBUG
    djylptimer_debug.pre_cmp = temp;
#endif
    while(!(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_CMPOK));
}

// =============================================================================
// 功能：读取定时器的cnt值
// 参数：定时器号
// 返回：无
// =============================================================================
uint16_t DjyBsp_LptimerReadCnt(uint8_t id)
{
    uint16_t temp1=0,temp2=1;
    while(temp1!=temp2)
    {
        temp1 = djybsp_lptimer_reg[id]->CNT;
        temp2 = djybsp_lptimer_reg[id]->CNT;
    }
    if(djybsp_lptimer_reg[id]->ISR & LPTIM_ISR_ARRM)
        return (CN_LIMIT_UINT16 + temp1);
    return temp1;
}

// =============================================================================
// 功能：定时器预初始化函数，有各种标志位的化可以在这里面清，系统起来以后会调用
// 参数：定时器号
// 返回：无
// =============================================================================
void DjyBsp_LptimerPreInit(uint8_t id)
{
}

// =============================================================================
// 功能：定时器初始化函数
// 参数：id:定时器号,reload:重装载值，isr:中断函数
// 返回：无
// =============================================================================
void DjyBsp_LptimerInit(uint8_t id,uint16_t reload,uint32_t (*isr)(ptu32_t param))
{
    uint32_t presc=0;
    uint32_t int_line = 0;

    presc = 32768/CN_CFG_TIME_BASE_HZ;
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
    switch(id)
    {
        case 0:
            RCC->APB1ENR1 |= RCC_APB1ENR1_LPTIM1EN | RCC_APB1ENR1_PWREN;
            RCC->CCIPR = RCC_CCIPR_LPTIM1SEL; // LPTIM1时钟选LSI
            int_line = CN_INT_LINE_LPTIM1;
            break;
        default:
            return;
    }

    djybsp_lptimer_reg[id]->CFGR = presc; // 4分频
    djybsp_lptimer_reg[id]->IER = LPTIM_IER_CMPMIE | LPTIM_IER_ARRMIE; // 开定时器中断

    Int_Register(int_line);
    Int_SetClearType(int_line,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(int_line,isr);
    Int_SettoAsynSignal(int_line);
    Int_ClearLine(int_line);
    Int_RestoreAsynLine(int_line);

    djybsp_lptimer_reg[id]->CR = LPTIM_CR_ENABLE; // 打开LPTIM, 但暂不开始计时
    djybsp_lptimer_reg[id]->ARR = reload;
    djybsp_lptimer_reg[id]->CMP = reload;
    DjyBsp_LptimerClearAllIsrFlag(id);
    djybsp_lptimer_reg[id]->CR |= LPTIM_CR_CNTSTRT;
    while(djybsp_lptimer_reg[id]->CNT==0);

}
#endif
#endif

