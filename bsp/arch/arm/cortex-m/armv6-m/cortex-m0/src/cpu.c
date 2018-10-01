//----------------------------------------------------
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
#include "board-config.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"

void __start_systick(void);
//struct SystickReg volatile * const pg_systick_reg
 //                       = (struct SystickReg *)0xE000E010;

//struct ScbReg volatile * const pg_scb_reg
//                        = (struct ScbReg *)0xe000ed00;

#if (CN_USE_TICKLESS_MODE)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_SAVE_ASYN    (0U)
#define CN_TIME_ROUNDING    (32768U)//四舍五入的值
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
                            //((Mhz%CN_CFG_TIME_BASE_HZ==0) ? (Mhz/CN_CFG_TIME_BASE_HZ) :((float)Mhz/CN_CFG_TIME_BASE_HZ)))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(500*TIME_GLUE):((500*CN_CFG_TIME_BASE_HZ)/Mhz))
#define NOTINT_FLAG             (0U)
#define GETSYSCNT_FLAG          (1U)
#define GETTIMEBASECNT_FLAG     (2U)
static u64 g_time_base_tick=0;
static bool_t bg_cnt_int_flag = false;
static u32 g_time_base_reload=0;
static u32 g_per_sys_cnt = 0;
static u8 bg_reload_flag = NOTINT_FLAG;
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
#if (CN_CFG_USE_BYPASSTIMER)
extern void Bypass_TimerInit(void);
extern void Bypass_Timer_PerInit(void);
#endif

void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Djy_IsrTimeBase);
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
#if (CN_CFG_USE_BYPASSTIMER)
    Bypass_Timer_PerInit();
#endif
}

void __DjyStartTimeBase(void)
{
    pg_systick_reg->reload = CN_LIMIT_UINT24;
    pg_systick_reg->current = CN_LIMIT_UINT24;
    g_time_base_reload = CN_LIMIT_UINT24;
#if (CN_CFG_USE_BYPASSTIMER)
    Bypass_TimerInit();
#endif
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //使能
                                |(1<<bo_systick_ctrl_tickint)   //允许产生中断
                                |(1<<bo_systick_ctrl_clksource);//用内核时钟
}

u32 __Djy_GetDelayMaxCnt(void)
{
    return CN_LIMIT_UINT24;
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
    u32 temp_reload = 0;
    u32 temp_cur = 0;
    if((cnt>CN_LIMIT_UINT24) || (cnt==0) || (bg_reload_flag == GETSYSCNT_FLAG))
    {
        //理论上不可能出现此事件
        return;
    }
    if( bg_reload_flag != NOTINT_FLAG )
        temp_reload = pg_systick_reg->reload;
    else
        temp_reload = g_time_base_reload;
//    if(pg_systick_reg->current < TIME_BASE_MIN_GAP)
//        return;
//    pg_systick_reg->reload = cnt;
    Int_CutTrunk();
    pg_systick_reg->reload = cnt;
    temp_cur = pg_systick_reg->current;
    pg_systick_reg->current = cnt;
    if(temp_reload > temp_cur )
            g_time_base_tick += temp_reload - temp_cur;
    if((bg_reload_flag == NOTINT_FLAG)&&(temp_cur > g_per_sys_cnt))
    {
        pg_systick_reg->reload = temp_reload;
        pg_systick_reg->current = temp_reload;
        bg_cnt_int_flag = true;
        Int_ContactTrunk();
        //pg_systick_reg->reload = CN_LIMIT_UINT24;
        return;
    }
    Int_ContactTrunk();
//    if(g_time_base_tick < g_per_cur)
//        g_time_base_tick = g_per_cur;
    bg_reload_flag = NOTINT_FLAG;
    g_time_base_reload = cnt;
#if CN_CFG_SAVE_ASYN
    pg_systick_reg->reload = CN_LIMIT_UINT24;
#endif
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
    if( bg_reload_flag )
        return (pg_systick_reg->reload - pg_systick_reg->current);
    else
        return (g_time_base_reload - pg_systick_reg->current );
}

u32 __Djy_GetTimeBaseReload(void)
{
    return g_time_base_reload;
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    u64 temp = 0;
    temp = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            ((us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
    if( temp < TIME_BASE_MIN_GAP )
        temp = TIME_BASE_MIN_GAP;
    return temp;
}

u32 __Djy_TimeBaseCntToUs(u64 cnt)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (cnt/(u32)TIME_GLUE):
            (u32)(((u64)(cnt*TIME_GLUE))>>16));
}

//注意：此函数只能在SYSTICK中断处理函数中使用
u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    bg_reload_flag = GETTIMEBASECNT_FLAG;
    bg_cnt_int_flag = false;
    g_time_base_tick += cnt;
    return (g_time_base_tick + __Djy_GetTimeBaseRealCnt());
}

u64 __DjyGetSysCnt(void)
{
    u32 cnt2=0;
    u64 temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    cnt2 = pg_systick_reg->current;
    if((pg_systick_reg->ctrl & bm_systick_ctrl_countflag) || bg_cnt_int_flag)
    {
        bg_cnt_int_flag = true;
        bg_reload_flag = GETSYSCNT_FLAG;
    }
    else
        bg_reload_flag = NOTINT_FLAG;
    //cnt = __Djy_GetTimeBaseRealCnt();
    g_per_sys_cnt = cnt2;
    if( bg_reload_flag == GETSYSCNT_FLAG )
    {
        temp = g_time_base_tick + g_time_base_reload + pg_systick_reg->reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
    else
    {
        temp = g_time_base_tick + g_time_base_reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
}

//----读取当前时间(uS)---------------------------------------------------------
//功能：读取当前时间(uS),从计算机启动以来经历的us数，64位，默认不会溢出
//      g_s64OsTime 为64位变量，非64位系统中，读取 g_s64OsTime 需要超过1个
//      周期,需要使用原子操作。
//参数：无
//返回：当前时钟
//说明: 这是一个桩函数,被systime.c文件的 DjyGetSysTime 函数调用。
//      如果systime不使用ticks作为时基，本函数可保持空函数。
//-----------------------------------------------------------------------------
u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
//    atom_low_t atom_low;

//    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (temp/(u32)TIME_GLUE):
            ((u64)(temp*TIME_GLUE))>>16);
    //Int_LowAtomEnd(atom_low);
    return time;
}
#endif
#else
extern s64  g_s64OsTicks;             //操作系统运行ticks数
__attribute__((weak)) void __InitTimeBase(void)
{}

//----初始化tick---------------------------------------------------------------
//功能: 初始化定时器,并连接tick中断函数,启动定时器.
//参数: 无
//返回: 无
//备注: 本函数是移植敏感函数.
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_IsrTick);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //使能
                               // |(1<<bo_systick_ctrl_tickint)   //M0,此时不允许产生中断
                                |(1<<bo_systick_ctrl_clksource);//用使用外部时钟

}

//----读取当前时间(uS)---------------------------------------------------------
//功能：读取当前时间(uS),从计算机启动以来经历的us数，64位，默认不会溢出
//      g_s64OsTicks 为64位变量，非64位系统中，读取 g_s64OsTicks 需要超过1个
//      周期,需要使用原子操作。
//参数：无
//返回：当前时钟
//说明: 这是一个桩函数,被systime.c文件的 DjyGetSysTime 函数调用。
//      如果systime不使用ticks作为时基，本函数可保持空函数。
//-----------------------------------------------------------------------------
__attribute__((weak)) s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return time;
}
#endif
extern void Init_Cpu(void);
extern void Load_Preload(void);
// =============================================================================
// 功能：运行到选择系统运行方式前，对于M3/M4的CPU，即PC跳转到Init_CPU()
// 参数：无
// 返回：无
// =============================================================================
void reboot(void)
{
    u32 InitCpu_Addr;
    InitCpu_Addr = *(u32*)0x00000004;
    ((void (*)(void))(InitCpu_Addr))();
}
// =============================================================================
// 功能：Reset硬件CPU，相当于上电重新启动，硬件软件都得到复位
// 参数：无
// 返回：无
// =============================================================================
void reset(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}
// =============================================================================
// 功能：运行到CPU加载代码前，即pre_load()前
// 参数：无
// 返回：无
// =============================================================================
void restart_system(void)
{
    Load_Preload();
}

