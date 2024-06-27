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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// 模块描述: PIC控制器上的TIMER的驱动
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 1:55:12 PM/Mar 4, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri_timer.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"
#include "board-config.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_HardTimer(void);
//    ModuleInstall_HardTimer();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip timer"//CPU的定时器外设驱动
//parent:"none"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER == false )
//#warning  " cpu_onchip_timer  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TIMER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define CN_TIMER_FREQ           24000000    //输入频率固定为24M
#define CN_MAX_COUNTER     0xFFFFFFFF   //32位定时器的最大计数值
//定时器的最大计时周期，uS数
#define CN_MAX_TIME_US     (0x100000000/CN_TIMER_FREQ*1000000)

//列举timer模块管理的、可用于 Timer_Alloc 函数分配的定时器硬件编号
enum ENUM_TIMER
{
    EN_TIMER_0=0,
    EN_TIMER_1=1
};

struct F1cx00Timer *pg_tTimerReg = (struct F1cx00Timer *)F1C100S_TIMER_BASE;

//各个定时器芯片的定时器应该有自己的句柄
struct TimerHandle
{
    u16     timerno;          //定时器号
    u16     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    fnTimerIsr UserIsr;       //用户中断响应函数
};

#define CN_TIMER_NUM   2

static struct TimerHandle  stgTimerHandle[CN_TIMER_NUM];

//timer0..timern的irq
static u32 sgHaltimerIrq[CN_TIMER_NUM]={    CN_INT_LINE_TIMER0,
                                            CN_INT_LINE_TIMER1
                                            };

//----设置定时器当前值-------------------------------------------------------
//功能: 设置各定时器当前值，周期数.
//参数: timer,定时器硬件编号
//      cycle,定时器预分频数,0~7对应1~128分频
//返回: 无
//-----------------------------------------------------------------------------
void Timer_EnableInt(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->IrqEn |= 1<<timer;
}
//----取定时器当前值-------------------------------------------------------
//功能:取定时器当前值，周期数.
//参数: timer,定时器硬件编号
//返回: 定时器当前值
//-----------------------------------------------------------------------------
void Timer_DisableInt(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->IrqEn &= ~(1<<timer);
}//----设置定时器时钟源----------------------------------------------------------
//功能: 设置各定时器时钟源.
//参数: timer,定时器硬件编号
//      value,定时器输入时钟源,1=24M,0=32768
//返回: 无
//-----------------------------------------------------------------------------
void Timer_SetClkSource(ufast_t timer,ufast_t value)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(3<<2);
    pg_tTimerReg->Cell[timer].TimerCtrl |= (value<<2);
}
//----设置定时器预分频数-------------------------------------------------------
//功能: 设置各定时器预分频数.
//参数: timer,定时器硬件编号
//      value,定时器预分频数,0~7对应1~128分频
//返回: 无
//-----------------------------------------------------------------------------
void Timer_SetPrecale(ufast_t timer,uint16_t value)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(7<<4);
    pg_tTimerReg->Cell[timer].TimerCtrl |= (value<<4);
}
//----设置定时器中断间隔-------------------------------------------------------
//功能: 设置各定时器中断间隔，周期数.
//参数: timer,定时器硬件编号
//      value,定时器预分频数,0~7对应1~128分频
//返回: 无
//-----------------------------------------------------------------------------
void Timer_SetInterval(ufast_t timer,u32 cycle)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerInterval = cycle;
}
//----取定时器中断间隔-------------------------------------------------------
//功能: 取定时器中断间隔，周期数.
//参数: timer,定时器硬件编号
//返回: 中断间隔，周期数
//-----------------------------------------------------------------------------
u32 Timer_GetInterval(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return 0;
    return pg_tTimerReg->Cell[timer].TimerInterval;
}
//----设置定时器当前值-------------------------------------------------------
//功能: 设置各定时器当前值，周期数.
//参数: timer,定时器硬件编号
//      cycle,定时器预分频数,0~7对应1~128分频
//返回: 无
//-----------------------------------------------------------------------------
void Timer_SetCounter(ufast_t timer,u32 cycle)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerValue = cycle;
}
//----取定时器当前值-------------------------------------------------------
//功能:取定时器当前值，周期数.
//参数: timer,定时器硬件编号
//返回: 定时器当前值
//-----------------------------------------------------------------------------
u32 Timer_GetCounter(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return 0;
    return pg_tTimerReg->Cell[timer].TimerValue;
}

//----设置定时器为周期中断模式-------------------------------------------------------
//功能: 设置定时器为周期中断模式
//参数: timer,定时器硬件编号
//返回: 无
//-----------------------------------------------------------------------------
void Timer_SetAutoReload(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(1<<7);
}
//----启动定时器----------------------------------------------------------------
//功能: 启动定时器
//参数: timer,定时器硬件编号
//返回: 无
//-----------------------------------------------------------------------------
void Timer_Start(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl |= 1<<0;
}
//----停止定时器----------------------------------------------------------------
//功能: 停止定时器
//参数: timer,定时器硬件编号
//返回: 无
//-----------------------------------------------------------------------------
void Timer_Stop(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->Cell[timer].TimerCtrl &= ~(1<<0);
}
//----获取定时器中断状态--------------------------------------------------------
//功能: 获取定时器中断状态
//参数: timer,定时器硬件编号
//返回: true = 中断已发生，false = 中断未发生
//-----------------------------------------------------------------------------
bool_t Timer_GetIntStatus(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    if(pg_tTimerReg->IrqSta & (1<<timer))
        return true;
    else
        return false;
}
//----清除定时器预中断-------------------------------------------------------
//功能: 清除定时器中断
//参数: timer,定时器硬件编号
//返回: 无
//-----------------------------------------------------------------------------
void Timer_ClearInt(ufast_t timer)
{
    if(timer >2)    //c100共3个定时器
        return;
    pg_tTimerReg->IrqSta |= 1<<timer; // clear int
}

u8 __F1CTimer_GetFreeTimer(void)
{
    u8 i;
    for(i = 0; i < CN_TIMER_NUM; i++)
    {
        if(stgTimerHandle[i].timerstate & CN_TIMER_FREE)
            break;
    }
    return i;
}

// =============================================================================
// 函数功能: 将提供的时间换算为定时器的计数脉冲数
// 输入参数:time，要换算的时间(单位为微秒)
// 输出参数:counter,对应时间的counter
// 返回值  :true成功 fasle失败
// 说明    :time太大会失败，计算的counter则是定时器的最大值
// =============================================================================
bool_t __F1CTimer_Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*CN_TIMER_FREQ/1000000;
    *counter = counter_1 > CN_MAX_COUNTER ? CN_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}
// =============================================================================
// 函数功能: 将定时器的计数脉冲数换算为时间
// 输入参数:counter,定时器的counter单元
// 输出参数:time，换算的时间(单位为微秒)
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t __F1CTimer_Counter2Time(u64 counter,u64 *time)
{
    u32 time_1=counter/(CN_TIMER_FREQ/1000000);
    if(time_1 > CN_MAX_TIME_US)
    {
        *time = CN_MAX_TIME_US;
        return false;
    }
    else
    {
        *time = time_1;
        return true;
    }
}
// =============================================================================
// 函数功能: 暂停止计时
// 输入参数: timer，定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __F1CTimer_PauseCount(struct TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
            pg_tTimerReg->Cell[timerno].TimerCtrl &= ~((1)<<0);
            pg_tTimerReg->IrqSta = 1<<timerno;
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENCOUNT);
            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能: 开始计时
// 输入参数:timer，定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __F1CTimer_StartCount(struct TimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
//            pg_tTimerReg->IrqSta = 1<<timerno;
            pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<1;
            pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<0;
            timer->timerstate = (timer->timerstate)| (CN_TIMER_ENCOUNT);

            return true;
        }
    }
    else
    {
        return false;
    }

}

// =============================================================================
// 函数功能:设定中断间隔的计时周期数
// 输入参数:timer，定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __F1CTimer_SetCycle(struct TimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u64 counter,time_set;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        else
        {
            pg_tTimerReg->Cell[timerno].TimerInterval = cycle;
            timer->cycle = cycle;
            return true;
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能: 设置定时器为自动重载
// 输入参数:timer，定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t  __F1CTimer_SetAutoReload(struct TimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno < CN_TIMER_NUM)
        {
            if(autoreload == true)
            {
                pg_tTimerReg->Cell[timerno].TimerCtrl &= ~(1<<7);
            }
            else
            {
                pg_tTimerReg->Cell[timerno].TimerCtrl |= 1<<7;
            }
        }
        else
        {
            result = false;
        }
    }
    else
    {
        result = false;
    }

    return result;
}

//-----------------------------------------------------------------------------
// 功能：定时器中断响应函数，先做必要的硬件操作，然后间接调用user ISR。
// 参数：定时器句柄。
// 返回：user ISR的返回值
// 说明：须定义为 weak 函数，已允许用户自己定义 ISR，f1c100需要自行操作定时器的寄存器才能
//       清中断，自定义ISR需注意。
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __F1CTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    timerno = ((struct TimerHandle  *)TimerHandle)->timerno;
    Timer_ClearInt(timerno);
    return ((struct TimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// 函数功能: 分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __F1CTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct TimerHandle  *timer;
    ptu32_t timerhandle;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = __F1CTimer_GetFreeTimer();
    if(timerno < CN_TIMER_NUM)     //还有空闲的，则设置标志位
    {
        Int_LowAtomEnd(timeratom);  //原子操作完毕
    }
    else//没有的话直接返回就可以了，用不着再啰嗦了
    {
        Int_LowAtomEnd(timeratom);   //原子操作完毕
        return 0;
    }
    irqline = sgHaltimerIrq[timerno];
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    //好了，中断号和定时器号码都有了，该干嘛就干嘛了。
    //先设置好定时器周期
    __F1CTimer_PauseCount(timer);
    //设置定时器中断,先结束掉该中断所有的关联相关内容
    Int_Register(irqline);
    Int_DisableLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __F1CTimer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// 函数功能: 释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __F1CTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //保护公用资源
    struct TimerHandle  *timer;
    timer = (struct TimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_TIMER_NUM)
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            //解除掉中断所关联的内容
            timer->timerstate = CN_TIMER_FREE;
            Int_DisableLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
            Int_UnRegister(irqline);
            Int_LowAtomEnd(timeratom);  //原子操作完毕

            return true;
        }
        else//没有的话直接返回就可以了，用不着再啰嗦了
        {
            return false;
        }

    }
    else
    {
        return false;
    }
}


// =============================================================================
// 函数功能: 设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __F1CTimer_SetIntPro(struct TimerHandle  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
            Int_EvttDisConnect(timer->irqline);
            return Int_SettoReal(timer->irqline);
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            return Int_SettoAsynSignal(timer->irqline);
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// 函数功能: 使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __F1CTimer_EnInt(struct TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        pg_tTimerReg->IrqEn |= 1 << timer->timerno;
        return Int_EnableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能: 禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __F1CTimer_DisInt(struct TimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        pg_tTimerReg->IrqEn &= ~(1 << timer->timerno);
        return Int_DisableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能: 获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（时钟数）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
bool_t __F1CTimer_GetTime(struct TimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            return false;
        }
        counter = pg_tTimerReg->Cell[timerno].TimerInterval - pg_tTimerReg->Cell[timerno].TimerValue;
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}

// =============================================================================
// 函数功能:取定时器硬件控制寄存器地址
// 输入参数:timer，待操作的定时器
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __F1CTimer_GetReg(struct TimerHandle  *timer, void **reg)
{
    *reg = pg_tTimerReg;
    return true;
}
// =============================================================================
// 函数功能: 看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __F1CTimer_CheckTimeout(struct TimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >= CN_TIMER_NUM)
        {
            result = false;
        }
        else
        {
            temp = pg_tTimerReg->Cell[timerno].TimerValue;
            if(temp == 0)
            {
                *timeout = true;
            }
            else
            {
                *timeout = false;
            }
        }
    }
    else
    {
        result = false;
    }
    return result;
}

// =============================================================================
// 函数功能：获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为 irqno,低16为对应的 timerno
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __F1CTimer_GetID(struct TimerHandle   *timer,u32 *timerId)
{
    u16 irqno;
    u16 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        timerno = timer->timerno;
        irqno = timer->irqline;
        *timerId = ((u32)irqno<<16)|(timerno);
        return true;
    }
}

// =============================================================================
// 函数功能：__F1CTimer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __F1CTimer_GetCycle(struct TimerHandle   *timer, u32 *cycle)
{
    if(NULL == timer)
    {
        return false;
    }
    else
    {
        *cycle = timer->cycle;
        return true;
    }
}
// =============================================================================
// 函数功能：__F1CTimer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __F1CTimer_GetState(struct TimerHandle   *timer, u32 *timerflag)
{

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        *timerflag = timer->timerstate;
        return true;
    }
}
// =============================================================================
// 函数功能:__F1CTimer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  :true成功  false失败
// 说明    :
// =============================================================================
bool_t __F1CTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct TimerHandle  *timer;
    timer = (struct TimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __F1CTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __F1CTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __F1CTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __F1CTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __F1CTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __F1CTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __F1CTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __F1CTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __F1CTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __F1CTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __F1CTimer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = __F1CTimer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// 函数功能: 获取定时器主频
// 输入参数:timerhandle 待操作的定时器句柄
// 输出参数:
// 返回值  :定时器主频
// 说明    :单位（HZ）
// =============================================================================
u32  __F1CTimer_GetFreq(ptu32_t timerhandle)
{
    return CN_TIMER_FREQ;
}
// =============================================================================
// 函数功能:
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  allwinner_timer;


    //定时器TIM0~2 选择时钟源为24M
    u8 i;

    for(i=0;i<CN_TIMER_NUM;i++)
    {
        stgTimerHandle[i].timerstate = CN_TIMER_FREE;
        Timer_SetClkSource(i,1);            //时钟源设为24M
        Timer_SetPrecale(i,0);              //预分频设为1，即不分频
    }

    allwinner_timer.chipname = "f1cx00-timer";
    allwinner_timer.HardTimerAlloc = __F1CTimer_Alloc;
    allwinner_timer.HardTimerFree = __F1CTimer_Free;
    allwinner_timer.HardTimerCtrl = __F1CTimer_Ctrl;
    allwinner_timer.HardTimerGetFreq = __F1CTimer_GetFreq;
    HardTimer_RegisterChip(&allwinner_timer);

    return true;
}

