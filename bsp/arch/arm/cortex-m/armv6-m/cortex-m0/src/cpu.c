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
#include "tickless.h"
#if (!CN_CFG_USE_USERTIMER)
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_PRECISION*TIME_GLUE):\
                                ((CN_CFG_TIME_PRECISION*CN_CFG_TIME_BASE_HZ)/Mhz))
#define NOTINT_FLAG             (0U)
#define GETSYSCNT_FLAG          (1U)
#define GETTIMEBASECNT_FLAG     (2U)
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
/*负责管理systick的数据结构*/
struct djybsp_systick_tickless_t
{
    bool int_flag;              /*发生中断标志*/
    bool reload_flag;           /*重装载标志*/
    uint32_t reload_value;      /*重装载的值*/
    uint32_t cnt_before;        /*上一次读到的cnt值*/
    uint64_t total_cnt;         /*系统累计的cnt值*/
}static djybsp_systick;

//////////////////////////////////////tickless-ops/////////////////////////////////////////////
// =============================================================================
// 功能：复位systick
// 参数：无
// 返回：无
// =============================================================================
static void DjyBsp_SystickReset(void)
{
    HardExp_ConnectSystick(Djy_ScheduleIsr);
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
    pg_systick_reg->ctrl =   ~((1<<bo_systick_ctrl_enable)    //ê1?ü
                                |(1<<bo_systick_ctrl_tickint)   //?êDí2úéú?D??
                                |(1<<bo_systick_ctrl_clksource));//ó??úo?ê±?ó
    djybsp_systick.int_flag = false;
    djybsp_systick.reload_flag = false;
    djybsp_systick.reload_value = 0;
    djybsp_systick.cnt_before = 0;
    djybsp_systick.total_cnt = 0;
}

// =============================================================================
// 功能：启动systick
// 参数：无
// 返回：无
// =============================================================================
static void DjyBsp_SystickStart(void)
{
    pg_systick_reg->reload = CN_LIMIT_UINT24;
    pg_systick_reg->current = CN_LIMIT_UINT24;
    djybsp_systick.reload_value = CN_LIMIT_UINT24;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ê1?ü
                                |(1<<bo_systick_ctrl_tickint)   //?êDí2úéú?D??
                                |(1<<bo_systick_ctrl_clksource);//ó??úo?ê±?ó
}


// =============================================================================
// 功能：设置systick的定时周期
// 参数：要设置的值
// 返回：无
// =============================================================================
static void DjyBsp_SystickSetReload(uint32_t cnt)
{
    uint32_t temp_reload = 0;
    uint32_t temp_cur = 0;
    if((cnt>CN_LIMIT_UINT24) || (cnt==0) || \
            ((djybsp_systick.reload_flag) == GETSYSCNT_FLAG))
    {
        //理论上不可能出现此事件
        return;
    }
    if( (djybsp_systick.reload_flag) != NOTINT_FLAG )
        temp_reload = pg_systick_reg->reload;
    else
        temp_reload = (djybsp_systick.reload_value);
    Int_CutTrunk();
    pg_systick_reg->reload = cnt;
    temp_cur = pg_systick_reg->current;
    pg_systick_reg->current = cnt;
    if(temp_reload > temp_cur )
        djybsp_systick.total_cnt += temp_reload - temp_cur;
    if(((djybsp_systick.reload_flag) == NOTINT_FLAG) && (temp_cur > djybsp_systick.cnt_before))
    {
        pg_systick_reg->reload = temp_reload;
        pg_systick_reg->current = temp_reload;
        djybsp_systick.int_flag = true;
        Int_ContactTrunk();
        return;
    }
    Int_ContactTrunk();
    djybsp_systick.reload_flag = NOTINT_FLAG;
    djybsp_systick.reload_value = cnt;
}

// =============================================================================
// 功能：获取systick当前的cnt值
// 参数：无
// 返回：当前cnt值
// =============================================================================
static uint32_t DjyBsp_SystickReadCnt(void)
{
    if( djybsp_systick.reload_flag )
        return (pg_systick_reg->reload - pg_systick_reg->current);
    else
        return ((djybsp_systick.reload_value) - (pg_systick_reg->current) );
}

// =============================================================================
// 功能：刷新systick当前累计的cnt值
// 参数：新增的cnt数
// 返回：当前累计的cnt数
// =============================================================================
static uint64_t Djybsp_SystickGetTotalCntIsr(uint32_t cnt)
{
    djybsp_systick.reload_flag = GETTIMEBASECNT_FLAG;
    djybsp_systick.int_flag = false;
    djybsp_systick.total_cnt += djybsp_systick.reload_value;
    return ((djybsp_systick.total_cnt) + DjyBsp_SystickReadCnt());
}

// =============================================================================
// 功能：获取systick当前累计的cnt值
// 参数：无
// 返回：当前累计的cnt值
// =============================================================================
static uint64_t DjyBsp_SystickGetTotalCnt(void)
{
    uint32_t cnt2=0;
    uint64_t temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    cnt2 = pg_systick_reg->current;
    if((pg_systick_reg->ctrl & bm_systick_ctrl_countflag) || (djybsp_systick.int_flag))
    {
        djybsp_systick.int_flag = true;
        djybsp_systick.reload_flag = GETSYSCNT_FLAG;
    }
    else
        djybsp_systick.reload_flag = NOTINT_FLAG;
    djybsp_systick.cnt_before = cnt2;
    if( (djybsp_systick.reload_flag) == GETSYSCNT_FLAG )
    {
        temp = (djybsp_systick.total_cnt) + (djybsp_systick.reload_value) + pg_systick_reg->reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
    else
    {
        temp = (djybsp_systick.total_cnt) + (djybsp_systick.reload_value) - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
}



/*上述OPS通过这个结构体注册到tickless模块里面*/
static struct djytickless_op_t djyticklss_systick_op =
{
    .get_total_cnt = DjyBsp_SystickGetTotalCnt,
    .get_total_cnt_isr = Djybsp_SystickGetTotalCntIsr,
    .set_reload = DjyBsp_SystickSetReload,
};

static struct djytickless_register_param_t djyticklss_systick =
{
     .op = &djyticklss_systick_op,
     .freq = CN_CFG_TIME_BASE_HZ,
     .max_reload_value = CN_LIMIT_UINT24,
     .min_reload_value = TIME_BASE_MIN_GAP,
};

#endif
#endif
///////////////////////////////////////////////djy-api start//////////////////////////////////
static uint64_t gRunTicks = 0;
static bool_t gResumeTickFlag = false;

__attribute__((weak)) uint64_t __DjyGetTicks(void)
{
    return gRunTicks;
}

//由调用者保证调用安全
__attribute__((weak)) void DjySetUpdateTickFlag(bool_t flag)
{
    gResumeTickFlag = flag;
}

__attribute__((weak)) bool_t DjyGetUpdateTickFlag(void)
{
    return gResumeTickFlag;
}

//由调用者保证原子操作
__attribute__((weak)) void DjyUpdateTicks(uint32_t ticks)
{
    gRunTicks += ticks;
}

// =============================================================================
// 功能：获取系统时间
// 参数：无
// 返回：当前us数
// =============================================================================
__attribute__((weak))   uint64_t __DjyGetSysTime(void)
{
#if     CN_USE_TICKLESS_MODE
    uint64_t temp=0;
    temp = DjyTickless_GetTotalCnt();
    return DjyTickless_CntToUs(temp);
#else
    extern s64  g_s64OsTicks;
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return (uint64_t)time;
#endif
}

// =============================================================================
// 功能：在系统起来以后需把各种标志复位
// 参数：无
// 返回：无
// =============================================================================
__attribute__((weak))   void __InitTimeBase(void)
{
#if     CN_USE_TICKLESS_MODE
#if (!CN_CFG_USE_USERTIMER)
    DjyTickless_Register(&djyticklss_systick);
    DjyBsp_SystickReset();
#endif
#endif
}

// =============================================================================
// 功能：初始化systick
// 参数：无
// 返回：无
// =============================================================================
__attribute__((weak))   void __DjyInitTick(void)
{
#if     CN_USE_TICKLESS_MODE
    DjyBsp_SystickStart();
#else
    HardExp_ConnectSystick(Djy_ScheduleIsr);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    //因M0没有 basepri 寄存器做异步信号开关，systick 中断线使能了，就直接使能了
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //使能
                               // |(1<<bo_systick_ctrl_tickint)   //M0,此时不允许产生中断
                                |(1<<bo_systick_ctrl_clksource);//用使用外部时钟
#endif
}
///////////////////////////////////////////////djy-api end//////////////////////////////////

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
void restart_app(void)
{
    Load_Preload();
}

