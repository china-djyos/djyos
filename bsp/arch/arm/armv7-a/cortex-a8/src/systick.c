#include <stdint.h>
#include <djyos.h>
#include <hard-exp.h>
#include <int.h>
#include <int_hard.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

u32 Get_SysTickCnt();
void Set_SysTickEnd(u32 value);
void  DJY_ScheduleIsr(u32 inc_ticks);


extern struct IntMasterCtrl  tg_int_global;
extern void __DJY_ScheduleAsynSignal(void);

#define CFG_TICKMODE_DYNAMIC        true
#define CFG_REAL_CRITICAL           100     //若距离当前tick中断时间小于此 uS 数，则不修改tick中断时间

static s32 s_gCurrentTicks = 1;     //当前tick运行周期。
static s32 s_gCriticalCycle;        //CFG_REAL_CRITICAL对应的定时器计数值
static s32 s_gTicksLimit;           //32bit定时器单次定时最大ticks数
extern s64  g_s64OsTicks;


__attribute__((weak)) void __DjyInitTick(void)
{
    s_gTicksLimit = (s32)((s64)0xffffffff/26000000);
    s_gCriticalCycle =(u32)((u64)1000);

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
//    asm("WFE");
//    asm("SVE");

// __asm__ __volatile__ ("sev" : : : "memory");
// __asm__ __volatile__ ("wfe" : : : "memory");
// #define wfi()  __asm__ __volatile__ ("wfi" : : : "memory")

//    __asm__ volatile ("SEV");
//    __asm__ volatile ("SEV");

//    __SEV();
//    __WFE();
}

// =============================================================================
// 功能:在Cortex M系统内核中，内核系统定时器SYSTICK定时中断属于异常，本函数类似于异步信号
//     入口函数，更新变量 g_s64OsTicks 并把下一次tick间隔设为1个tick。
// 参数:无
// 返回:无
// =============================================================================
void Exp_SystickTickHandler(void)
{
    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal++;

//  g_s64OsTicks += s_gCurrentTicks;

    DJY_ScheduleIsr(s_gCurrentTicks);
    s_gCurrentTicks = 1;
    tg_int_global.nest_asyn_signal--;
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
    atom_low_t atom_low;
//  u32 tick_cnt;
    atom_low = Int_LowAtomStart();
    current = g_s64OsTicks;
//    tick_cnt = Get_SysTickCnt();  //BK7251 无法读出定时器当前值
    Int_LowAtomEnd(atom_low);

//  return (current*CN_CFG_TICK_US + (tick_cnt*1000*1000)/26*1000*1000);
    return (current*CN_CFG_TICK_US);
}


//----设置下次ticks中断时间-----------------------------------------------------
//功能：恒定tick模式下，由于tick间隔恒等于1个tick，故直接返回1，定时器设置维持不变。
//     动态tick模式下，通过修改定时器参数，把当次tick间隔设为参数Ticks。根据设计原理，
//     当次定时结束后，须把中断间隔设为1。参数Ticks的含义，是从上次tick中断算起，定时器
//     是持续不断在走的，因此需要考虑从上次tick中断依赖定时器已经走的时间，下述两类定时器
//     有不同的处理。
//     定时器有两大类：
//     1、从0计数到设定值，然后请求中断，定时器从0开始重新计时。这种定时器，只需要修改设
//        定值即可，无须考虑当前已经走过的时间。把下次中断间隔设为1的工作在tick中断的ISR
//        中完成。
//     2、类似cortex-m的24位systick定时器，从reload开始减计数到0，然后从reload重新开始
//        计数的，则设置完当次tick后，还要把reload设置到1个tick。特别注意，设置定时器
//        时，要减去从上次ISR中断依赖定时器已经走过的时间。
//
//参数：无
//返回：当前时钟
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
//  s32 temp;
    if(Ticks > s_gTicksLimit)
        Ticks = s_gTicksLimit;
    if(s_gCurrentTicks == Ticks)
        return Ticks;


    //tick中断快到了，为避工作在临界区导致的问题，不允许改变tick时间
    //由于调用本函数时没有关闭实时中断，故实时中断的ISR执行不能大于 CFG_REAL_CRITICAL 微秒
    //特别注释：BK7251 无论定时器还是pwm，都无法读出当前计数值，故无法实现此功能。
    //只能直接重新设置tick。
    //理论分析，应该没有问题

//  temp = Get_SysTickCnt();
//  if((temp +s_gCriticalCycle) <(s_gCurrentTicks*26*1000/1000*1000))
//  {
    Set_SysTickEnd(Ticks*(CN_CFG_TICK_US*1000/1000)*26);
    s_gCurrentTicks = Ticks;
//  }

    return s_gCurrentTicks;
}
