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
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "djyos.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "board-config.h"

#if (!CN_CFG_USE_USERTIMER)
#define CN_TIME_ROUNDING    (32768U)//四舍五入的值
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_BASE_HZ/Mhz):(Mhz/CN_CFG_TIME_BASE_HZ))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):\
                            (TIME_GLUE>=100)?(4*TIME_GLUE):((4+(100/TIME_GLUE))*TIME_GLUE))
static u64 g_time_base_tick=0;
static u32 __DjyIsrTime(ptu32_t line);
//----初始化tick---------------------------------------------------------------
//功能: 初始化定时器,并连接tick中断函数,启动定时器.
//参数: 无
//返回: 无
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
void __InitTimeBase(void)
{
    Int_Register(CN_INT_LINE_TIMER3);
    Int_CutLine(CN_INT_LINE_TIMER3);
    //连接timer3作为tick中断
    Int_IsrConnect(CN_INT_LINE_TIMER3,__DjyIsrTime);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER3);          //tick中断被设为异步信号
    //以下设置定时器参数，需与board-config.h中CN_CFG_TICK_US、CN_CFG_TICK_HZ和
    //CN_CFG_FINE_US、CN_CFG_FINE_HZ的定义一致
    Timer_SetClkSource(3,0);          //pclk预分频数的1/2分频
    Timer_SetPrecale(1,CN_CFG_TIMER_CLK/CN_CFG_TIME_BASE_HZ/2 -1);
    //以上把定时器输入时钟的频率设为CN_CFG_FINE_HZ
    //Timer_SetCounter(3,(uint16_t)((u32)CN_CFG_TICK_US*1000/CN_CFG_FINE_US));
    Timer_SetCounter(3, CN_LIMIT_UINT16);
    Timer_SetType(3,1);                //设置tick定时器连续工作
    Timer_Reload(3);                    //重载定时值
}

void __DjyStartTimeBase(void)
{
    Timer_Start(3);                     //启动定时器
    Int_RestoreAsynLine(CN_INT_LINE_TIMER3);//启动tick中断S
}

//获取可以定时的最大值
u32 __Djy_GetDelayMaxCnt(void)
{
    return CN_LIMIT_UINT16;
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

//----重新设置tick中断时间---------------------------------------------------------------
//功能: 重新设置定时器reload值.
//参数: time:单位us
//返回: 无
//备注: 本函数是移植敏感函数.
//这个是内部调用函数，这里不对time的值做判断，由调用者确保正确性
//-----------------------------------------------------------------------------
void __Djy_SetTimeBaseCnt(u32 cnt)
{
    if(cnt>CN_LIMIT_UINT16)
    {
        //理论上不可能出现此事件
        return;
    }
    g_time_base_tick += pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3;
    pg_timer_reg->TCNTB3 = cnt;
    pg_timer_reg->TCNTO3 = pg_timer_reg->TCNTB3;
}

//----获取tick中断时间---------------------------------------------------------------
//功能: 获取当定时中断发生时，reload与current的差值.
//参数: 无
//返回: reload对应的US值
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
static u32 __Djy_GetTimeBaseRealCnt(void)
{
    return (pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3);
}

//----获取定时器的reload值对应的US值---------------------------------------------------------------
//功能: 获取定时器reload值对应的US值.
//参数: 无
//返回: reload对应的US值
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
u32 __Djy_GetTimeBaseReload(void)
{
    return (pg_timer_reg->TCNTB3);
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            (us*FAST_TIME_GLUE + CN_TIME_ROUNDING)>>16);
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    g_time_base_tick += cnt;
    return g_time_base_tick;
}

u64 __DjyGetSysCnt(void)
{
    u32 cnt=0;
    cnt = __Djy_GetTimeBaseRealCnt();
//  if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
//      return (g_time_base_tick+cnt+__Djy_GetTimeBaseReload());
//  else
        return (g_time_base_tick+cnt);
}


//----读取当前时间(uS)---------------------------------------------------------
//功能：读取当前时间(uS),从计算机启动以来经历的us数，64位，默认不会溢出
//      g_s64OsTime 为64位变量，非64位系统中，读取 g_s64OsTime 需要超过1个
//      周期,需要使用原子操作。
//参数：无
//返回：当前时钟
//说明: 这是一个桩函数,被systime.c文件的 DJY_GetSysTime 函数调用。
//      如果systime不使用ticks作为时基，本函数可保持空函数。
//-----------------------------------------------------------------------------
u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
           (temp*FAST_TIME_GLUE+(u64)CN_TIME_ROUNDING)>>16:
            (temp*TIME_GLUE));
    Int_LowAtomEnd(atom_low);
    return time;
}

static u32 __DjyIsrTime(ptu32_t line)
{
    u32 tick=0;
    tick = __Djy_GetTimeBaseReload();
    Djy_IsrTimeBase(tick);
    return 0;
}

#endif

// =============================================================================
// 功能：运行到选择系统运行方式前，对于M3/M4的CPU，即PC跳转到Init_CPU()
// 参数：无
// 返回：无
// =============================================================================
void CPU_Reboot(void)
{

}
// =============================================================================
// 功能：Reset硬件CPU，相当于上电重新启动，硬件软件都得到复位
// 参数：无
// 返回：无
// =============================================================================
void CPU_Reset(void)
{
    volatile u32 *SWRST = (u32*)0x4C000044;

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    *SWRST = 0x533C2416;
}
// =============================================================================
// 功能：运行到CPU加载代码前，即pre_load()前
// 参数：无
// 返回：无
// =============================================================================
void CPU_RestartSystem(void)
{

}

