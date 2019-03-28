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
//所属模块：系统性能测试模块
//作者:
//版本：V1.0.0
//文件描述:
//其他说明:
//修订历史:
//
#include "cpu_peri.h"
#include <int.h>
#include <systime.h>
#include "djyos.h"

typedef struct
{
    volatile u32 FSR;
    volatile u32 CR;
    volatile u32 TC;
    volatile u32 PR;
    volatile u32 PC;
    volatile u32 MR;
    volatile u32 MCR;
    volatile u32 EMR;
    volatile u32 EWCR;
    volatile u32 CAPTR;
    volatile u32 CCR;
    volatile u32 INTIF;
    volatile u32 INTIE;
}TIM_TypeDef;

TIM_TypeDef *TimerReg;
bool_t TimerStarted = false;
extern u32 IntLine;

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//这里重新实现TimerIsr，在定时器驱动中，须定义成 weak 函数
u32 __SCM6xxTimer_isr(ptu32_t TimerHandle)
{
    u32 cnt1,cnt2;
    u32 cnt16L1 = 0,cnt16H1 = 0;
    u32 cnt16L2 = 0,cnt16H2 = 0;

    TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
    TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

    cnt16L1 = Reg_Tim16L->TC;
    cnt16H1 = Reg_Tim16H->TC;
    cnt1   = (cnt16H1 << 16) | cnt16L1;

    cnt16L2 = Reg_Tim16L->TC;
    cnt16H2 = Reg_Tim16H->TC;
    cnt2   = (cnt16H2 << 16) | cnt16L2;

    Reg_Tim16L->INTIF |= (1<<0);//清中断标志
    Reg_Tim16H->INTIF |= (1<<0);
    Int_ClearLine(IntLine);

    cnt1 = cnt1 - (cnt2-cnt1);
    Test_PushTimeISR(cnt1);     //记录数据
    return 0;
}
#pragma GCC diagnostic pop

u32 __Test_TimerStart(void)
{
    TimerStarted = true;
    TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
    TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

    //清中断标记
    Reg_Tim16L->INTIF |= 0xf;
    Reg_Tim16H->INTIF |= 0xf;

    Reg_Tim16L->TC  = 0;
    Reg_Tim16H->TC  = 0;

    //复位
    Reg_Tim16L->CR |= (1<<1);
    Reg_Tim16H->CR |= (1<<1);

    //停止复位
    Reg_Tim16L->CR &= ~(1<<1);
    Reg_Tim16H->CR &= ~(1<<1);

    //启动计数
    Reg_Tim16L->CR = 0x1;
    Reg_Tim16H->CR = 0x1;


    return 0;
}

bool_t __Test_TimerEnd(u32 *val)
{
    u32 valueL = 0,valueH = 0;
    if(TimerStarted)
    {
        TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
        TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);

        TimerStarted = false;
        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        *val   = (valueH<<16) | valueL;
        return true;
    }
    else
        return false;
}

void Test_IntSyncHook(ucpu_t SchType)
{
    u32 cnt1,cnt2;
    u32 valueL = 0,valueH = 0;

    //切入的时候才获取时间,切出不做处理
    if(SchType == EN_SWITCH_IN)
    {
        TIM_TypeDef* Reg_Tim16L = (TIM_TypeDef*) TimerReg;
        TIM_TypeDef* Reg_Tim16H = (TIM_TypeDef*)((u32)TimerReg + 0x40);
        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        cnt1 = (valueH<<16) | valueL;

        valueL = Reg_Tim16L->TC;
        valueH = Reg_Tim16H->TC;
        cnt2   = (valueH<<16) | valueL;

        cnt1 = cnt1 - (cnt2-cnt1);
        Test_PushTimeHook(cnt1);//记录数据
    }
}

