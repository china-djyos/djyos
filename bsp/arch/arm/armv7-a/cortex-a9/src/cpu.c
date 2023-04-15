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

#define CN_CFG_TIME_BASE_HZ 250
#define CN_INT_LINE_TIMER3	CN_INT_LINE_29

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
//    return (pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3);
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

