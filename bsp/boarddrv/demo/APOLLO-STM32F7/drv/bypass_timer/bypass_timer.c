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
#include "cpu_peri.h"
#include "int.h"

static u64 g_bypass_timer_cnt = 0;

static u32  Bypadd_Timer_Int(ptu32_t handle)
{
    static u8 fi = 0;
    TIM5->SR = 0;//清中断标志
    if(fi!=0)
        g_bypass_timer_cnt += 1080000000;
    fi = 1;
    return 0;
}

u64 Bypass_GetCnt(void)
{
    static u32 temp_per = 0;
    static u32 temp = 0;

    if(TIM5->ARR==0)
        return 0;

    if(temp_per==0 && temp==0)
    {
        temp = TIM5->CNT;
        temp_per = temp;
        return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
    }
    else
    {
        temp = TIM5->CNT;
        if( temp_per < temp)
        {
            temp_per = temp;
            g_bypass_timer_cnt += 1080000000;
            return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
        }
        else
        {
            temp_per = temp;
            return (g_bypass_timer_cnt + TIM5->ARR - TIM5->CNT);
        }
    }
}

u64 Bypass_Cnt_to_Systick(void)
{
    return (Bypass_GetCnt()<<1);
    //return (Bypass_GetCnt()/108)*(CN_CFG_TIME_BASE_HZ/Mhz);
}

void Bypass_Timer_PerInit(void)
{
    g_bypass_timer_cnt = 0;
    TIM5->ARR = 0;
    TIM5->CNT = 0;
}

void Bypass_TimerInit(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    TIM5->CR1 &= ~(TIM_CR1_CEN); //禁止TIMER
    //TIM5->CR1 &= ~(TIM_CR1_DIR);
    TIM5->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//自动重装

    //TIM5->DIER |= TIM_DIER_UIE;//使能更新中断
    TIM5->PSC = 0;//分频系数 为零 不分频(1/108)1uS
    TIM5->ARR = 1080000000;//定时器预装初值:10S

//  Int_Register(CN_INT_LINE_TIM5);
//  Int_SetClearType(CN_INT_LINE_TIM5,CN_INT_CLEAR_AUTO);
//  Int_IsrConnect(CN_INT_LINE_TIM5,Bypadd_Timer_Int);
//  Int_SettoAsynSignal(CN_INT_LINE_TIM5);
//  Int_ClearLine(CN_INT_LINE_TIM5);
//  Int_RestoreAsynLine(CN_INT_LINE_TIM5);

    //TIM5->CNT = 0;
    TIM5->SR = 0;//清中断标志
    TIM5->CR1 |= TIM_CR1_CEN;
}

