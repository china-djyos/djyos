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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "core_ck803.h"
#include "arch_feature.h"
#include "align.h"
#include "int.h"
#include "stdint.h"
#include "djyos.h"
#include "hard-exp.h"
#include "board-config.h"
#include "cpu_peri.h"

#if (CN_USE_TICKLESS_MODE)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_SAVE_ASYN    (0U)
#define CN_TIME_ROUNDING    (32768U)//闁搞儲绋栭崹妤佺閺傚灝寮抽柣銊ュ閿燂拷
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
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
#if CN_CFG_USE_BYPASSTIMER
#include "bypass_timer.h"
#else
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
#endif

void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Djy_IsrTimeBase);
    CORET->LOAD = 0;
    CORET->VAL = 0;
#if CN_CFG_USE_BYPASSTIMER
    Bypass_Timer_PerInit();
#endif
}

void __DjyStartTimeBase(void)
{
    CORET->LOAD = CN_LIMIT_UINT24;
    CORET->VAL = CN_LIMIT_UINT24;
    g_time_base_reload = CN_LIMIT_UINT24;
    //CORET->LOAD = 160000;
    //CORET->VAL = 160000;
    //g_time_base_reload = 160000;
#if CN_CFG_USE_BYPASSTIMER
    Bypass_TimerInit();
#endif
    CORET->CTRL = CORET_CTRL_CLKSOURCE_Msk |
                   CORET_CTRL_TICKINT_Msk |
                   CORET_CTRL_ENABLE_Msk;                               /* Enable CORET IRQ and CORET Timer */

    csi_vic_enable_irq(PIC_IRQID_CTIMER);

    Int_Register(CN_INT_LINE_CORET);
    Int_SetClearType(CN_INT_LINE_CORET,CN_INT_CLEAR_AUTO);
    Int_IsrConnect(CN_INT_LINE_CORET,Exp_SystickTickHandler);
    Int_SettoAsynSignal(CN_INT_LINE_CORET);
    Int_ClearLine(CN_INT_LINE_CORET);
    Int_RestoreAsynLine(CN_INT_LINE_CORET);
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
        //闁荤偛妫滈鎴炵▔婵犱胶鐟濋柛娆樺灥閸忔﹢宕欓搹鐟扮疀婵縿鍊撶花銊︾閿燂拷        return;
    }
    if( bg_reload_flag != NOTINT_FLAG )
        temp_reload = CORET->LOAD;
    else
        temp_reload = g_time_base_reload;
    if( CORET->VAL < TIME_BASE_MIN_GAP)
        return;
//    pg_systick_reg->reload = cnt;
#if (!CN_CFG_USE_BYPASSTIMER)
    Int_CutTrunk();
#endif
    CORET->LOAD = cnt;
    temp_cur = CORET->VAL;
    CORET->VAL = cnt;//闁告劖鐛憉rrent閻庨潧瀚悺銊╁闯閵娿儳鏉介梻鍕噸缁楀倹瀵煎顐⑩枏闁稿繐鐖奸崳鍝ユ啑閸涚増绁皉eload闁稿鎷�
#if (!CN_CFG_USE_BYPASSTIMER)
    if(temp_reload > temp_cur )
            g_time_base_tick += temp_reload - temp_cur;
    if((bg_reload_flag == NOTINT_FLAG)&&(temp_cur > g_per_sys_cnt))
    {
        CORET->LOAD = temp_reload;
        CORET->VAL = temp_reload;
        bg_cnt_int_flag = true;
        Int_ContactTrunk();
        //pg_systick_reg->reload = CN_LIMIT_UINT24;
        return;
    }
//#if   (!CN_CFG_USE_BYPASSTIMER)
    Int_ContactTrunk();
//    if(g_time_base_tick < g_per_cur)
//        g_time_base_tick = g_per_cur;
#endif
    bg_reload_flag = NOTINT_FLAG;
    g_time_base_reload = cnt;

#if CN_CFG_SAVE_ASYN
    CORET->LOAD = CN_LIMIT_UINT24;
#endif
}

#if (!CN_CFG_USE_BYPASSIMER)
static u32 __Djy_GetTimeBaseRealCnt(void)
{
    if( bg_reload_flag )
        return (CORET->LOAD - CORET->VAL);
    else
        return (g_time_base_reload - CORET->VAL );
}
#endif

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
//    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
//            (us*TIME_GLUE):
//            ((us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
}

u32 __Djy_TimeBaseCntToUs(u64 cnt)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (cnt/(u32)TIME_GLUE):
            (u32)(((u64)(cnt*TIME_GLUE))>>16));
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    bg_reload_flag = GETTIMEBASECNT_FLAG;
    bg_cnt_int_flag = false;
    g_time_base_tick += cnt;
#if (!CN_CFG_USE_BYPASSTIMER)
    return (g_time_base_tick + __Djy_GetTimeBaseRealCnt());
#else
    return Bypass_Cnt_to_Systick();
#endif
}

u64 __DjyGetSysCnt(void)
{
#if (!CN_CFG_USE_BYPASSTIMER)
    u32 cnt2=0;
    u64 temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    cnt2 = CORET->VAL;
    if((CORET->CTRL & (1<<16)) || bg_cnt_int_flag)
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
#else
    return Bypass_Cnt_to_Systick();
#endif
}

u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    //atom_low_t atom_low;

   // atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (temp/(u32)TIME_GLUE):
            ((u64)(temp*TIME_GLUE))>>16);
   // Int_LowAtomEnd(atom_low);
    return time;
}
#endif
#else
extern s64  g_s64OsTicks;             //闁瑰灝绉崇紞鏃傚寲閼姐倗鍩犻弶鈺傚姌椤㈡唸icks闁轰緤鎷�
extern void Init_Cpu(void);
extern void Load_Preload(void);
__attribute__((weak)) void __InitTimeBase(void)
{}

__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_IsrTick);
    CORET->LOAD = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    CORET->VAL =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    CORET->CTRL = CORET_CTRL_CLKSOURCE_Msk |
               CORET_CTRL_TICKINT_Msk |
               CORET_CTRL_ENABLE_Msk;                               /* Enable CORET IRQ and CORET Timer */

    csi_vic_enable_irq(PIC_IRQID_CTIMER);

//    Int_Register(CN_INT_LINE_CORET);
//    Int_SetClearType(CN_INT_LINE_CORET,CN_INT_CLEAR_AUTO);
//    Int_IsrConnect(CN_INT_LINE_CORET,Exp_SystickTickHandler);
//    Int_SettoAsynSignal(CN_INT_LINE_CORET);
//    Int_ClearLine(CN_INT_LINE_CORET);
//    Int_RestoreAsynLine(CN_INT_LINE_CORET);

    djybsp_isr_hdl_register(CN_INT_LINE_CORET,0,Exp_SystickTickHandler,CN_INT_LINE_CORET);
}

__attribute__((weak)) s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - CORET->VAL;

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

void reboot(u32 key)
{
    Init_Cpu();
}

void reset(u32 key)
{

}

void restart_system(u32 key)
{
    Load_Preload();
}

