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
#include <djyos.h>
#include <int.h>
#include <systime.h>

TIM_TypeDef *TimerReg;
bool_t TimerStarted = false;    //不要修改变量名，测试入口函数SysPerformTest中要用
extern u32 IntLine;     //所使用定时器的中断号，在测试入口函数SysPerformTest中赋值

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//这里重新实现TimerIsr，在定时器驱动中，须定义成 weak 函数
//函数功能是，读出从中断发生到进入本函数的时间间隔，并调用Test_PushTimeISR保存
u32 __STM32Timer_isr(ptu32_t TimerHandle)
{
    u32 cnt1,cnt2;
    //cnt1需要获取的是进入ISR的时间，但如果直接读取，则读取CNT寄存器的时间也计算在内，读两次，
    //减去差值，更精确。
    cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
    cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
    ((TIM_TypeDef *)TimerReg)->SR = 0;//清中断标志
    Int_ClearLine(IntLine);

    cnt1 = cnt1 - (cnt2-cnt1);
    Test_PushTimeISR(cnt1);     //记录数据
    return 0;
}
#pragma GCC diagnostic pop

//实现性能检测模块中的 weak 函数
//重置并设置定时器启动标志。
u32 __Test_TimerStart(void)
{
    TimerStarted = true;
    ((TIM_TypeDef *)TimerReg)->CNT = 0;
    return 0;
}

//实现性能检测模块中的 weak 函数
//如果定时器处于启动状态，读取从上次调用 __Test_TimerStart 以来的时间间隔，并停止定时器。
bool_t __Test_TimerEnd(u32 *val)
{
    if(TimerStarted)
    {
        TimerStarted = false;
        *val = ((TIM_TypeDef *)TimerReg)->CNT;
        return true;
    }
    else
        return false;
}

//实现性能检测模块中的 weak 函数
//用于中断同步测试，读出从中断发生到进入本函数的时间间隔，并调用Test_PushTimeHook保存
void Test_IntSyncHook(ucpu_t SchType)
{
    u32 cnt1,cnt2;

    //切入的时候才获取时间,切出不做处理
    if(SchType == EN_SWITCH_IN)
    {
        cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
        cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
        cnt1 = cnt1 - (cnt2-cnt1);
        Test_PushTimeHook(cnt1);        //记录数据
    }
}

