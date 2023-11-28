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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块: BSP
//作者：lst
//版本：V1.0.0
//文件描述: 利用cortex-m系列CPU的systick作为系统ticks时钟源，默认勾选，但dynamic-tick
//         模式会有误差。低功耗要求高的场合，应使用低功耗定时器以替代之。
//其他说明:
//1. 日期: 2019-08-11
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <stdint.h>
#include <cpu.h>
#include <int.h>
#include <cpu_peri_int_line.h>
#include <int_hard.h>
#include "cpu_peri.h"

extern void __DJY_ScheduleAsynSignal(void);
void SysTickTimer(void);

#define CFG_TICKMODE_DYNAMIC        false
#define CFG_REAL_CRITICAL           100     //若距离当前tick中断时间小于此 uS 数，则不修改tick中断时间

static s32 s_gCurrentTicks = 1;     //当前tick运行周期。
static s32 s_gCriticalCycle;        //CFG_REAL_CRITICAL对应的定时器计数值
static s32 s_gTicksLimit;           //24bit定时器单次定时最大ticks数
extern s64  g_s64OsTicks;

//----初始化tick---------------------------------------------------------------
//功能: 初始化定时器,并连接tick中断函数,启动定时器.
//参数: 无
//返回: 无
//备注: 本函数是移植敏感函数.
//      weak函数，用户也可以在boarddrv中重新实现。
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    Int_Register(CN_INT_LINE_TIMER2);
    Int_CutLine(CN_INT_LINE_TIMER2);
    //连接timer2作为tick中断
    Int_IsrConnect(CN_INT_LINE_TIMER2,SysTickTimer);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER2);          //tick中断被设为异步信号
    //以下设置定时器参数，需与board-config.h中CN_CFG_TICK_US、CN_CFG_TICK_HZ和
    //CN_CFG_FINE_US、CN_CFG_FINE_HZ的定义一致
    Timer_SetClkSource(2,1);            //时钟源设为24M
    Timer_SetPrecale(2,0);              //预分频设为1，即不分频
    Timer_SetInterval(2, CN_CFG_TICK_US*24);    //24M
    Timer_SetAutoReload(2);                 //设置tick定时器连续工作
    Timer_Start(2);                         //启动定时值
    Timer_EnableInt(2);
    Int_RestoreLine(CN_INT_LINE_TIMER2);
}

//------------------------------------------------------------------------------
//功能：清理CPU的休眠唤醒事件状态，使CPU在关中断后，确实能被发生在关中断期间的唤醒事件
//     （中断或多处理机信号）唤醒，无论唤醒事件发生在 WFE（ARM的指令，其他CPU可能不同）
//     之前或者之后。
//参数：无
//返回：无
//注：本函数典型地被用于低功耗控制。
//------------------------------------------------------------------------------
void CleanWakeupEvent(void)
{
}

// =============================================================================
// 功能:在Cortex M系统内核中，内核系统定时器SYSTICK定时中断属于异常，本函数类似于异步信号
//     入口函数，更新变量 g_s64OsTicks 并把下一次tick间隔设为1个tick。
// 参数:无
// 返回:无
// =============================================================================
void SysTickTimer(void)
{
    s32 inc;
    Timer_ClearInt(2);
//    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal_counter++;
//    tg_int_global.nest_asyn_signal++;
    inc = s_gCurrentTicks;
    s_gCurrentTicks = 1;
    DJY_ScheduleIsr(inc);
//    tg_int_global.nest_asyn_signal--;
//    tg_int_global.en_asyn_signal_counter--;
//    if((g_ptEventReady != g_ptEventRunning) && (tg_int_global.nest_asyn_signal==0))
//        __DJY_ScheduleAsynSignal();       //执行中断内调度
//    g_bScheduleEnable = true;
}

// =============================================================================
// 功能：获取由tick驱动的系统时间，如果bsp设计使用了专用定时器提供系统时钟，则可以不提供
//      本函数。读取时，是 g_s64OsTicks 加上定时器的当前计数值。
//      读取当前计数值特别要注意，如果在关异步信号期间调用本函数且期间发生了定时器重载，
//      或者恰好在本函数内关中断后且在读 cycle 之前发生了定时器重载，就需要处理定时器翻转
//      的问题。
// 参数：无
// 返回：当前us数
// =============================================================================
__attribute__((weak))   u64 __DjyGetSysTime(void)
{
    s64 current;
    s32 cyclep,cyclen,interval,reload;
    bool_t ctrl;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    interval = s_gCurrentTicks;
    cyclep = Timer_GetCounter(2);
    reload = Timer_GetInterval(2);
    ctrl = Timer_GetIntStatus(2);
    cyclen = Timer_GetCounter(2);
    current = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    if(cyclen > cyclep)     //减计数，说明在读寄存器过程中发生了定时器重载
    {
        cyclen = reload - cyclen;   //重载值 - 当前值
        current += interval;
    }
    else                    //在读寄存器过程中未发生定时器重载，但不能排除之前发生过
    {
        if(ctrl)      //说明读寄存器前定时器已经重载了，但中断还没有响应。
        {
            cyclen = reload - cyclen;   //重载值 - 当前值
            current += interval;
        }
        else
        {
            cyclen =(s32)((s64)interval*24000000 - cyclen);
        }
    }
    current = current*CN_CFG_TICK_US + ((s64)cyclen*CN_CFG_FINE_US >>16);

    return (u64)current;
}

//----设置下次ticks中断时间-----------------------------------------------------
//功能：恒定tick模式下，由于tick间隔恒等于1个tick，故直接返回1，定时器设置维持不变。
//     动态tick模式下，通过修改定时器参数，把当次tick间隔设为参数Ticks。由于只有idle
//      事件运行时，才会重设tick间隔，而CPU被唤醒后，idle并不一定能及时获得运行时间，
//      故每次定时结束后，须把中断间隔设为1，以确保idle没有机会运行的期间，ticks能正常
//      中断。参数Ticks的含义，是从上次tick中断算起的，而定时器是持续不断在走的，因此
//      需要考虑从上次tick中断以来定时器已经走的时间，下述两类定时器有不同的处理。
//     定时器有两大类：
//     1、从0计数到设定值，然后请求中断，定时器归0开始重新计时。这种定时器，只需要修改设
//        定值即可，无须考虑当前已经走过的时间。把下次中断间隔设为1的工作在tick中断的ISR
//        中完成。
//     2、类似cortex-m的24位systick定时器，从reload开始减计数到0，然后从reload重新开始
//        计数的，则设置完当次tick后，还要把reload设置到1个tick。特别注意，设置定时器
//        时，要减去从上次ISR中断以来定时器已经走过的时间。
//
//参数：ticks,希望的下次中断距上次的中断间隔数
//返回：实际的下次中断距上次的中断间隔数
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
#if CFG_TICKMODE_DYNAMIC == true
    u32 temp;
    atom_high_t atom_high;
    if(Ticks > s_gTicksLimit)
        Ticks = s_gTicksLimit;
    if(s_gCurrentTicks == Ticks)
        return Ticks;
    temp = pg_systick_reg->current;
    //tick中断快到了，为避工作在临界区导致的问题，不允许改变tick时间
    //由于调用本函数时没有关闭实时中断，故实时中断的ISR执行不能大于CFG_REAL_CRITICAL微秒
    if(temp >= s_gCriticalCycle)
    {
        if(s_gCurrentTicks > Ticks)
        {
            temp = (s_gCurrentTicks - Ticks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

            atom_high = Int_HighAtomStart();
            //减计数定时器有个固定的缺陷，读——调整——写回计数值的过程时间差，将会产生计时误差
            //虽然误差很小，但总是误差。
            //加计数的就不存在问题。
            temp = Timer_GetCounter(2) - temp;
            //只需要改变current寄存器，Timer_GetCounter(2)上或减去新ticks和原ticks的差值即可
            Timer_SetCounter(2,temp);
            Int_HighAtomEnd(atom_high);
        }
        else
        {
            temp = (Ticks - s_gCurrentTicks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

            atom_high = Int_HighAtomStart();
            //减计数定时器有个固定的缺陷，读——调整——写回计数值的过程时间差，将会产生计时误差
            //虽然误差很小，但总是误差。
            //加计数的就不存在问题。
            temp = Timer_GetCounter(2) + temp;
            //只需要改变current寄存器，Timer_GetCounter(2)上或减去新ticks和原ticks的差值即可
            Timer_SetCounter(2,temp);
            Int_HighAtomEnd(atom_high);
        }
//        pg_systick_reg->reload = 0xffffff;
        s_gCurrentTicks = Ticks;
    }
    return s_gCurrentTicks;

#else       // for CFG_TICKMODE_DYNAMIC == true
    return 1;
#endif      // for CFG_TICKMODE_DYNAMIC == true
}

