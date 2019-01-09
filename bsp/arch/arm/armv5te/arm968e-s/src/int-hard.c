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
// =============================================================================
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "int_hard.h"
#include "critical.h"
#include "djyos.h"

#include "intc.h"
#include "icu_pub.h"
#include "drv_model_pub.h"

extern struct IntMasterCtrl  tg_int_global;
extern bool_t g_bScheduleEnable;

void djy_irq_dispatch(void)
{
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal=1;
    intc_irq();
    if(g_ptEventReady != g_ptEventRunning)
          __Djy_ScheduleAsynSignal();       //闂傚倷绀佸﹢閬嶆偡閹惰棄骞㈤柍鍝勫?归弶绋库攽閻愭潙鐏﹂柟灏栨櫊瀹曘垺绂掔?ｎ亣鎽曢梺鎼炲労閸撴瑧绮堥崘顔界厱婵炴垶鐟ラˉ瀣箾閸繄鍩ｆ鐐寸墬濞煎繘鎮欓锟介锟?
    tg_int_global.nest_asyn_signal = 0;
    g_bScheduleEnable = true;
}

void djy_fiq_dispatch(void)
{
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal=1;
    intc_fiq();
    if(g_ptEventReady != g_ptEventRunning)
          __Djy_ScheduleAsynSignal();       //闂傚倷绀佸﹢閬嶆偡閹惰棄骞㈤柍鍝勫?归弶绋库攽閻愭潙鐏﹂柟灏栨櫊瀹曘垺绂掔?ｎ亣鎽曢梺鎼炲労閸撴瑧绮堥崘顔界厱婵炴垶鐟ラˉ瀣箾閸繄鍩ｆ鐐寸墬濞煎繘鎮欓锟介锟?
    tg_int_global.nest_asyn_signal = 0;
    g_bScheduleEnable = true;
}

//----接通异步信号开关---------------------------------------------------------
//功能：接通异步信号开关,如果总开关接通且中断线开关接通,该中断将被允许
//      1.当有独立的硬件开关时,把该开关接通即可
//      2.如果没有独立硬件开关,则接通所有被允许的异步信号的线开关.
//      3.cm3属于第一种情况,把BASEPRI寄存器设为0来允许全部异步信号
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
    portDISABLE_FIQ();
    portDISABLE_IRQ();
}

//----断开异步信号开关---------------------------------------------------------
//功能：断开异步信号开关,所有的异步信号将被禁止
//      1.当有独立的硬件开关时,把该开关断开即可
//      2.如果没有独立硬件开关,则断开所有异步信号的线开关.
//      3.cm3属于第一种情况,把BASEPRI寄存器设为最低来禁止异步信号
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
    portDISABLE_FIQ();
    portDISABLE_IRQ();
}

//----接通总中断开关-----------------------------------------------------------
//功能：接通总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{
    portENABLE_FIQ();
    portENABLE_IRQ();
}

//----断开总中断开关---------------------------------------------------------
//功能：断开总中断开关,所有cpu都会有一个总开关,直接操作该开关即可.
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
    portDISABLE_FIQ();
    portDISABLE_IRQ();
}

//----接通单个中断线开关-------------------------------------------------------
//功能：接通单个中断线开关,该中断是否允许还要看后面的开关状态
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    intc_enable(ufl_line);
    return true;
}

//----断开单个中断线开关-------------------------------------------------------
//功能：断开单个中断线开关，无论总中断和异步信号开关状态如何，该中断线被禁止
//参数：无
//返回：无
//备注：移植敏感
//-----------------------------------------------------------------------------
bool_t Int_CutLine(ufast_t ufl_line)
{
    intc_disable(ufl_line);
    return true;
}

//----清除相应中断线的中断挂起状态---------------------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作，清除前，不能响应同一中断线
//      的后续中断，清除后，才可以响应后续中断。本函数与该中断线被设置为实时中断
//      还是异步信号无关.
//      cm3中，响应中断时自动清挂起状态，本函数用于手动清除挂起状态
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{
    return true;
}

//----激发中断-----------------------------------------------------------------
//功能: 触发一个中断.如果中断本已悬挂,本函数无影响.本函数与该中断线被设置为实时
//      中断还是异步信号无关
//参数：ufast ufl_line，欲触发的中断线号
//返回：如果相应的中断线硬件不提供用软件触发中断功能,返回 false,否则返回 true
//备注: 本函数实现依赖于硬件,有些硬件系统不支持此功能.
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t Int_TapLine(ufast_t ufl_line)
{
    return true;
}
//----清除全部中断线的中断挂起状态---------------------------------------------
//功能：硬件应该有相应的功能，提供清除中断挂起的操作
//参数：ufast ufl_line，指定应答的中断线号
//返回：无
//备注：有些体系中断响应时硬件应答，本函数为空函数。
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
void __Int_ClearAllLine(void)
{
}

//----查询中断线请求状态-------------------------------------------------------
//功能：查询并清除相应中断线状态，可用于查询式中断程序
//参数：ufl_line，欲查询的中断线
//返回：若中断挂起，返回true，否则返回false
//备注: 与硬件结构相关,有些结构可能不提供这个功能,慎用!
//      本函数是移植敏感函数
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    return true;
}

//----把指定中断线设置为异步信号--------－－－---------------------------------
//功能：把指定中断线设置为异步信号,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    return true;
}

//----把指定中断线设置为实时中断--------－－－---------------------------------
//功能：把指定中断线设置为实时中断,若中断正在响应,则当前中断返回后生效
//参数：ufast ufl_line，指定被设置的中断线号
//返回：true=成功，false=失败
//      本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    return true;
}

//----使能中断嵌套-------------------------------------------------------------
//功能: 使能一个中断源被抢占，使能后，相应的中断服务期间，可能会被别的中断抢占。
//      本函数不分实时中断还是异步信号，与具体的实现有关，移植者应该根据硬件特性
//      和应用需求，确定是否允许嵌套。
//      嵌套发生在实时中断之间或者异步信号之间。实时中断永远可以打断异步信号，这
//      不算嵌套。
//      对于三星系列的ARM芯片中断管理器的特点，对于实时中断的处理，有三种方式:
//      1、用fiq实现实时中断，INTOFFSET1和INTOFFSET2寄存器提供了当前正在服务的
//         中断号，但该两个寄存器在fiq中无效，如果实时中断用fiq实现，则需要逐位
//         检查被设为实时中断的中断号在INTPND1和INTPND1寄存器中的值，才能确定被
//         服务的中断号，这个过程可能非常漫长。这种情况能够实现实时中断嵌套，但
//         却完全失去了实时中断的设计初衷，djyos for s3c2416不采取这种方式。
//      2、有一个特殊情况是，系统只需要一个实时中断，其他全部设为异步信号，这种
//         情况，用fiq实现实时中断是最为理想的，当然只有一个中断，嵌套也就无从
//         谈起了。
//      3、实时中断和异步信号都是用irq方式实现，cpsr的I位用做trunk开关，异步信号
//         没有独立开关，是通过把所有的属于异步信号的中断号的开关全部关掉来模拟
//         异步信号开关的。这种情况是不能实现实时中断嵌套的。为什么呢?arm被设计
//         为，只要I位为0，新的中断就可以抢占正在服务的中断，也就意味着，异步信
//         号抢占实时中断，在硬件上是允许的。实时中断要实现嵌套，须先关掉异步信
//         号，再设置I=0，如果实时中断响应后，在异步信号被关掉之前，紧接着有异步
//         信号发生的话，该irq请求就会发出，随后关掉异步信号也没用，总中断一旦被
//         允许，该异步信号就会抢占实时中断。这种抢占，不仅把"辈分"搞乱，还会引
//         发严重问题，甚至使系统崩溃。如果抢占实时中断的异步信号，服务期间发生
//         了线程切换，把fiq服务的栈，当作pg_event_running的栈。因此，djyos for
//         2416版本不支持实时中断嵌套。
//         cortex-m3中，异步信号全部设置为最低优先级，根据cm3的中断嵌套机制，这样是
//         不能实现嵌套的。
//参数: ufl_line，被操作的中断线
//返回: 无
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{
    return true;
}

//----禁止中断嵌套-------------------------------------------------------------
//功能: 清除某中断号的中断嵌套使能。
//参数: ufl_line，被操作的中断线
//返回: 无
//-----------------------------------------------------------------------------
void Int_DisableNest(ufast_t ufl_line)
{
}

//----设定嵌套优先级-----------------------------------------------------------
//功能: 设定指定中断线的嵌套优先级，本函数严重依赖硬件功能。如果硬件不支持，可
//      保持空函数。
//参数：ufast ufl_line，指定被设置的中断线号
//返回：无
//注: 本函数移植敏感
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    return true;
}


//----初始化中断---------------------------------------------------------------
//功能：初始化中断硬件,初始化中断线数据结构
//      2.异步信号保持禁止,它会在线程启动引擎中打开.
//      3.总中断允许，
//      用户初始化过程应该遵守如下规则:
//      1.系统开始时就已经禁止所有异步信号,用户初始化时无须担心异步信号发生.
//      2.初始化过程中如果需要操作总中断/实时中断/异步信号,应该成对使用.禁止使
//        异步信号实际处于允许状态(即异步和总中断开关同时允许).
//      3.可以操作中断线,比如连接、允许、禁止等,但应该成对使用.
//      4.建议使用save/restore函数对,不要使用enable/disable函数对.
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void Int_Init(void)
{
    ufast_t ufl_line;

    //Int_CutTrunk();
    //Int_HighAtomStart();
    for(ufl_line=0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
    {
        //属性位图清零,全部置为异步信号方式
        tg_int_global.property_bitmap[ufl_line] = 0;
        //中断使能位图清0,全部处于禁止状态
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal =0;
    tg_int_global.nest_real=0;

//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //异步信号计数
   // Int_CutAsynSignal();
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;       //总中断计数
//    intc_init();
    //Int_ContactTrunk();                    //接通总中断开关
}
