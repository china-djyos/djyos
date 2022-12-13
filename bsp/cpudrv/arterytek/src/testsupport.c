#include <cpu_peri.h>
#include <djyos.h>
#include <int.h>
#include <systime.h>

tmr_type *timer;
bool_t TimerStarted = false;    //不要修改变量名，测试入口函数SysPerformTest中要用
extern u32 IntLine;     //所使用定时器的中断号，在测试入口函数SysPerformTest中赋值

void Test_PushTimeISR(u32 time_counter);
void Test_PushTimeHook(u32 time_counter);

//这里重新实现TimerIsr，在定时器驱动中，须定义成 weak 函数
//函数功能是，读出从中断发生到进入本函数的时间间隔，并调用Test_PushTimeISR保存
u32 __STM32Timer_isr(ptu32_t TimerHandle)
{
    return 0;
}

//实现性能检测模块中的 weak 函数
//重置并设置定时器启动标志。
u32 __Test_TimerStart(void)
{
    return 0;
}

//实现性能检测模块中的 weak 函数
//如果定时器处于启动状态，读取从上次调用 __Test_TimerStart 以来的时间间隔，并停止定时器。
bool_t __Test_TimerEnd(u32 *val)
{
    return false;
}

//实现性能检测模块中的 weak 函数
//用于中断同步测试，读出从中断发生到进入本函数的时间间隔，并调用Test_PushTimeHook保存
void Test_IntSyncHook(ucpu_t SchType)
{
    return;
}
