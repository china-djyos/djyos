#include <stdint.h>
#include <cpu.h>
#include <int.h>
#include <int_hard.h>
#include "cpu_peri.h"
//#include <core_cm3.h>


/*----------------------------------------------*/

struct SystickReg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //当倒数至零时?将被重装载的值
    vu32 current;   //读取时返回当前倒计数的值?写则清零?并清CTRL中的COUNTFLAG 标志
    vuc32 verify;   //SysTick校准数值寄存器
};


struct SystickReg *pg_systick_reg = (void *)0;
#define CN_CFG_FCLK 999
#define bo_systick_ctrl_clksource 0
#define bo_systick_ctrl_enable    0


#define __SEV()  do { } while(0)
#define __WFE()  do { } while(0)
/*----------------------------------------------*/


extern struct IntMasterCtrl  tg_int_global;
extern void __DJY_ScheduleAsynSignal(void);

#define CFG_TICKMODE_DYNAMIC        true
#define CFG_REAL_CRITICAL           100     //若距离当前tick中断时间小于此 uS 数，则不修改tick中断时间

static s32 s_gCurrentTicks = 1;     // 当前tick运行周期。
static s32 s_gCriticalCycle;        // CFG_REAL_CRITICAL 对应的定时器计数值
static s32 s_gTicksLimit;           // 24bit定时器单次定时最大ticks数
extern s64 g_s64OsTicks;
u32 Exp_SystickTickHandler(ptu32_t timeline);

__attribute__((weak)) void __DjyInitTick(void)
{
    u32 a=7;
    //todo :在此添加定时器初始化代码
    Int_Register(29);
    Int_SettoAsynSignal(29);
    Int_IsrConnect(29,Exp_SystickTickHandler);
    Int_RestoreLine(29);

    /* interface to OS(djy) */
    s_gTicksLimit = (s32)((s64)0xffffffff*CN_CFG_TICK_HZ/CN_CFG_FCLK);
    s_gCriticalCycle =(u32)((u64)CN_CFG_FCLK * CFG_REAL_CRITICAL / 1000000);

    /* interface to HW */
    // Enable Private Timer for periodic IRQ
//  setPriorityMask(0x1F);
    Int_ContactLine(29);
    Int_SetPrio(29, a);
//  init_private_timer(0xF0000, 0);
    init_private_timer(0x10000000, 0);  // 0 - Auto Reload
    start_private_timer();
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
    __SEV();
    __WFE();
}

// =============================================================================
// 功能:在Cortex M系统内核中，内核系统定时器SYSTICK定时中断属于异常，本函数类似于异步信号
//     入口函数，更新变量 g_s64OsTicks 并把下一次tick间隔设为1个tick。
// 参数:无
// 返回:无
// =============================================================================
u32 Exp_SystickTickHandler(ptu32_t timeline)
{
    s32 inc;
    g_bScheduleEnable = false;
//  tg_int_global.en_asyn_signal_counter++;
    tg_int_global.nest_asyn_signal++;
//  if(!DjyGetUpdateTickFlag())
//      DjyUpdateTicks(1);
//  else
//      DjySetUpdateTickFlag(false);
//  g_s64OsTicks += s_gCurrentTicks;
    inc = s_gCurrentTicks;
    s_gCurrentTicks = 1;
    pg_systick_reg->ctrl;   //清零，与 __DjyGetSysTime 函数配合使用
    DJY_ScheduleIsr(inc);
    tg_int_global.nest_asyn_signal--;
//  tg_int_global.en_asyn_signal_counter--;
    if(g_ptEventReady != g_ptEventRunning)
        __DJY_ScheduleAsynSignal();       //执行中断内调度
    g_bScheduleEnable = true;
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
    s32 cyclep,cyclen,interval,reload,ctrl;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    interval = s_gCurrentTicks;
    cyclep = pg_systick_reg->current;
    reload = pg_systick_reg->reload;
    ctrl = pg_systick_reg->ctrl;
    cyclen = pg_systick_reg->current;
    current = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    if(cyclen > cyclep)     //减计数，说明在读寄存器过程中发生了定时器重载
    {
        cyclen = reload - cyclen;
        current += interval;
    }
    else                    //在读寄存器过程中未发生定时器重载，但不能排除之前发生过
    {
        if(ctrl & 0x10000)      //说明读寄存器前定时器已经重载了，但中断还没有响应。
        {
            cyclen = reload - cyclen;
            current += interval;
        }
        else
        {
            cyclen =(s32)((s64)interval*CN_CFG_FCLK/CN_CFG_TICK_HZ - cyclen);
        }
    }
    current = current*CN_CFG_TICK_US + ((s64)cyclen*CN_CFG_FINE_US >>16);

    return (u64)current;
}

//----设置下次ticks中断时间-----------------------------------------------------
//功能：恒定tick模式下，由于tick间隔恒等于1个tick，故直接返回1，定时器设置维持不变。
//     动态tick模式下，通过修改定时器参数，把当次tick间隔设为参数Ticks。由于只有idle
//      事件运行时，才会重设tick间隔，而CPU被唤醒后，idle并不一定能及时获得运行时间，故
//      每次定时结束后，须把中断间隔设为1。参数Ticks的含义，是从上次tick中断算起的，而
//      定时器是持续不断在走的，因此需要考虑从上次tick中断以来定时器已经走的时间，下述
//      两类定时器有不同的处理。
//     定时器有两大类：
//     1、从0计数到设定值，然后请求中断，定时器归0开始重新计时。这种定时器，只需要修改设
//        定值即可，无须考虑当前已经走过的时间。把下次中断间隔设为1的工作在tick中断的ISR
//        中完成。
//     2、类似cortex-m的24位systick定时器，从reload开始减计数到0，然后从reload重新开始
//        计数的，则设置完当次tick后，还要把reload设置到1个tick。特别注意，设置定时器
//        时，要减去从上次ISR中断以来定时器已经走过的时间。
//
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
#if CFG_TICKMODE_DYNAMIC == true
    s32 temp;
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
        temp = (s32)(s_gCurrentTicks - Ticks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

        atom_high = Int_HighAtomStart();
        temp = pg_systick_reg->current - temp;
        pg_systick_reg->reload = temp;
        pg_systick_reg->current = temp;
        Int_HighAtomEnd(atom_high);
        pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
//        pg_systick_reg->reload = 0xffffff;
        s_gCurrentTicks = Ticks;
    }
    return s_gCurrentTicks;

#else       // for CFG_TICKMODE_DYNAMIC == true
    return 1;
#endif      // for CFG_TICKMODE_DYNAMIC == true
}

