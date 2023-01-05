#include "cpu_peri.h"
#include <djyos.h>
#include <int.h>
#include <systime.h>

void *TimerReg;
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
//##WIP##    u32 cnt1,cnt2;
//##WIP##    //cnt1需要获取的是进入ISR的时间，但如果直接读取，则读取CNT寄存器的时间也计算在内，读两次，
//##WIP##    //减去差值，更精确。
//##WIP##    cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
//##WIP##    cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
//##WIP##    ((TIM_TypeDef *)TimerReg)->SR = 0;//清中断标志
//##WIP##    Int_ClearLine(IntLine);
//##WIP##
//##WIP##    cnt1 = cnt1 - (cnt2-cnt1);
//##WIP##    Test_PushTimeISR(cnt1);     //记录数据
    return 0;
}
#pragma GCC diagnostic pop

//实现性能检测模块中的 weak 函数
//重置并设置定时器启动标志。
u32 __Test_TimerStart(void)
{
//##WIP##    TimerStarted = true;
//##WIP##    ((TIM_TypeDef *)TimerReg)->CNT = 0;
    return 0;
}

//实现性能检测模块中的 weak 函数
//如果定时器处于启动状态，读取从上次调用 __Test_TimerStart 以来的时间间隔，并停止定时器。
bool_t __Test_TimerEnd(u32 *val)
{
//##WIP##    if(TimerStarted)
//##WIP##    {
//##WIP##        TimerStarted = false;
//##WIP##        *val = ((TIM_TypeDef *)TimerReg)->CNT;
//##WIP##        return true;
//##WIP##    }
//##WIP##    else
//##WIP##        return false;
}

//实现性能检测模块中的 weak 函数
//用于中断同步测试，读出从中断发生到进入本函数的时间间隔，并调用Test_PushTimeHook保存
void Test_IntSyncHook(ucpu_t SchType)
{
//##WIP##    u32 cnt1,cnt2;
//##WIP##
//##WIP##    //切入的时候才获取时间,切出不做处理
//##WIP##    if(SchType == EN_SWITCH_IN)
//##WIP##    {
//##WIP##        cnt1 = ((TIM_TypeDef *)TimerReg)->CNT;
//##WIP##        cnt2 = ((TIM_TypeDef *)TimerReg)->CNT;
//##WIP##        cnt1 = cnt1 - (cnt2-cnt1);
//##WIP##        Test_PushTimeHook(cnt1);        //记录数据
//##WIP##    }
}

