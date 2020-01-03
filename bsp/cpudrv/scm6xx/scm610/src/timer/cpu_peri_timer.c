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
// 模块描述: PIC控制器上的TIMER的驱动
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 1:55:12 PM/Mar 4, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注：由于提供给timer_core.c使用，所以不再做参数检查之类的
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "timer_hard.h"
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
//parent:"Software Timers"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"Software Timers","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
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

//define timer address

#define CN_TIMERA0_BASE (0x40500000)
#define CN_TIMERA1_BASE (0x40500000 + 0x40)
#define CN_TIMERB0_BASE (0x40510000)
#define CN_TIMERB1_BASE (0x40510000 + 0x40)
#define CN_TIMERC0_BASE (0x41520000)
#define CN_TIMERC1_BASE (0x41520000 + 0x40)

//#define CN_TIMERD0_BASE (0x41530000)
//#define CN_TIMERD1_BASE (0x41530000 + 0x40)

//寄存器定义,16Bit全部串成32Bit定时器

typedef struct
{
    volatile u32 FSR;
    volatile u32 CR;
    volatile u32 TC;
    volatile u32 PR;
    volatile u32 PC;
    volatile u32 MR;
    volatile u32 MCR;
    volatile u32 EMR;
    volatile u32 EWCR;
    volatile u32 CAPTR;
    volatile u32 CCR;
    volatile u32 INTIF;
    volatile u32 INTIE;
}tagTimer16Reg;

typedef struct
{
    volatile tagTimer16Reg *mp_TimerReg_L;//低16bit定时器寄存器
    volatile tagTimer16Reg *mp_TimerReg_H;//高16bit定时器寄存器
}tagTimer32Reg;

//32bit 定时器高位寄存器定义

volatile tagTimer32Reg sgpt_TimerReg[] = {
    {
        (volatile tagTimer32Reg*)CN_TIMERA0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERA1_BASE,
    },
    {
        (volatile tagTimer32Reg*)CN_TIMERB0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERB1_BASE,
    },
    {
        (volatile tagTimer32Reg*)CN_TIMERC0_BASE,
        (volatile tagTimer32Reg*)CN_TIMERC1_BASE,
    },

};

#define CN_PIT_MAX_COUNTER     0xFFFFFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFFFFFF/375/10)  //0xFFFFFFFF/375Mhz=1us


enum ENUM_SCM6XX_TIMER
{
    EN_SCM6XXTIMER_A=0,//按顺序赋值
    EN_SCM6XXTIMER_B,   //EN_STM32TIMER_5为1，EN_STM32TIMER_5下再增加一个则为2
    EN_SCM6XXTIMER_C,
//  EN_STM32TIMER_D,
};

//各个定时器芯片的定时器应该有自己的句柄
struct SCM6xxTimerHandle
{
    u32     timerno;          //定时器号
    u32     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    bool_t  autoReload;       //自动重装载标记 true 使能  false 使能
    fnTimerIsr UserIsr;      //用户中断响应函数
};
#define CN_SCM6XXTIMER_NUM   (EN_SCM6XXTIMER_C +1)
#define CN_SCM6XXTIMER_MAX    EN_SCM6XXTIMER_C

static struct SCM6xxTimerHandle  stgTimerHandle[CN_SCM6XXTIMER_NUM];

//最高位代表timer0，依次类推
static u32  gs_dwSCM6xxTimerBitmap;  //对于定时器这种东西，一般的不会很多，32个应该足够
#define CN_SCM6XXTIMER_BITMAP_MSK  (0x80000000)  //最高位为1，依次右移即可

//timer0到timern的irq中断
static u32 sgHaltimerIrq[CN_SCM6XXTIMER_NUM]={
                                            CN_INT_LINE_TIMERA1,
                                            CN_INT_LINE_TIMERB1,
                                            CN_INT_LINE_TIMERC1,
                                            };

//获取32位数第一个0bit位置(从高位到低位算起)
u8 __SCM6xxTimer_GetFirstZeroBit(u32 para)
{
    u8 i;
    for(i = 0; i < 32; i++)
    {
        if(!(para & (1<<(31-i))))
            break;
    }
    return i;
}

// =============================================================================
// 函数功能:__SCM6xxTimer_Time2Counter
//          将提供的时间换算为定时器的计数单位,
// 输入参数:time，要换算的时间(单位为微秒)
// 输出参数:counter,对应时间的counter
// 返回值  :true成功 fasle失败
// 说明    :time太大会失败，计算的counter则是定时器的最大值
// =============================================================================
bool_t __SCM6xxTimer_Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*375/10;
    *counter = counter_1 > CN_PIT_MAX_COUNTER ? CN_PIT_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}
// =============================================================================
// 函数功能:__SCM6xxTimer_Counter2Time
//          将定时器的计数单位换算为时间
// 输入参数:counter,定时器的counter单元
// 输出参数:time，换算的时间(单位为微秒)
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t __SCM6xxTimer_Counter2Time(u64 counter,u64 *time)
{
    //这里设置对Pcl时钟进行计数 - 不分频- 37500000
    u32 time_1=counter/375* 10;
    if(time_1 > CN_PIT_MAX_TIME_US)
    {
        *time = CN_PIT_MAX_TIME_US;
        return false;
    }
    else
    {
        *time = time_1;
        return true;
    }
}
// =============================================================================
// 函数功能:__SCM6xxTimer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================

bool_t __SCM6xxTimer_PauseCount(struct SCM6xxTimerHandle  *timer)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0;
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0;

            //清中断标志
            sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF = 0;
            sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF = 0;

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
// 函数功能:__SCM6xxTimer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __SCM6xxTimer_StartCount(struct SCM6xxTimerHandle  *timer)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            //清中断标志
            sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF |= 0xf;
            sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF |= 0xf;

            //复位以及停止复位 TC0
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR |= (1<<1);
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR |= (1<<1);

            sgpt_TimerReg[timerno].mp_TimerReg_L->CR &= ~(1<<1);
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR &= ~(1<<1);

            //启动计数
            sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0x1;
            sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0x1;

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
// 函数功能:__SCM6xxTimer_SetCycle,STM32的timer,最大为0xFFFFFFFF,根据Timer_ModuleInit
//          设定计数器每计一次数为(1/84)1us，可知最大时间不超-1
//          设定周期
// 输入参数:timer，PIC定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __SCM6xxTimer_SetCycle(struct SCM6xxTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u64 counter,time_set;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        else
        {
            //设置匹配值
            sgpt_TimerReg[timerno].mp_TimerReg_L->MR = (u16)cycle;
            sgpt_TimerReg[timerno].mp_TimerReg_H->MR = (u16)(cycle>>16);
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
// 函数功能:__SCM6xxTimer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================

bool_t  __SCM6xxTimer_SetAutoReload(struct SCM6xxTimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;

        if(timerno < CN_SCM6XXTIMER_MAX)
        {
            if(autoreload == true)
            {
                stgTimerHandle[timerno].autoReload = true;
                //默认就是自动重装载
//                sgpt_TimerReg[timerno].mp_TimerReg_L->MCR &= ~(1<<2);
//                sgpt_TimerReg[timerno].mp_TimerReg_H->MCR &= ~(1<<2);
            }
            else
            {
                stgTimerHandle[timerno].autoReload = false;
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
// 功能：定时器中断响应函数，对于像atmel m7这样，需要自行操作定时器的寄存器才能
//       清中断的定时器，必须在这里实现ISR，然后间接调用user ISR，否则在
//       user isr中将无所适从。
// 参数：定时器句柄。
// 返回：user ISR的返回值
//-----------------------------------------------------------------------------
__attribute__((weak))  u32 __SCM6xxTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;

    timerno = ((struct SCM6xxTimerHandle  *)TimerHandle)->timerno;

    if(stgTimerHandle[timerno].autoReload == false)
    {
         sgpt_TimerReg[timerno].mp_TimerReg_L->CR = 0;
         sgpt_TimerReg[timerno].mp_TimerReg_H->CR = 0;
    }
    //清匹配中断
    sgpt_TimerReg[timerno].mp_TimerReg_L->INTIF |= (1<<0);
    sgpt_TimerReg[timerno].mp_TimerReg_H->INTIF |= (1<<0);

    Int_ClearLine(((struct SCM6xxTimerHandle  *)TimerHandle)->irqline);
    return ((struct SCM6xxTimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// 函数功能:__SCM6xxTimer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __SCM6xxTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct SCM6xxTimerHandle  *timer;
    ptu32_t timerhandle;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = __SCM6xxTimer_GetFirstZeroBit(gs_dwSCM6xxTimerBitmap);
    if(timerno < CN_SCM6XXTIMER_NUM)//还有空闲的，则设置标志位
    {
        gs_dwSCM6xxTimerBitmap = gs_dwSCM6xxTimerBitmap | (CN_SCM6XXTIMER_BITMAP_MSK<< timerno);
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
    __SCM6xxTimer_PauseCount(timer);
    //__SCM6xxTimer_SetCycle(timer,cycle);
    //设置定时器中断,先结束掉该中断所有的关联相关内容
    Int_Register(irqline);
    Int_CutLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __SCM6xxTimer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// 函数功能:__SCM6xxTimer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __SCM6xxTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //保护公用资源
    struct SCM6xxTimerHandle  *timer;
    timer = (struct SCM6xxTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_SCM6XXTIMER_NUM)//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            gs_dwSCM6xxTimerBitmap = gs_dwSCM6xxTimerBitmap &(~(CN_SCM6XXTIMER_BITMAP_MSK<< timerno));
            //解除掉中断所关联的内容
            timer->timerstate = 0;
            Int_CutLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
            Int_UnRegister(irqline);
            Int_LowAtomEnd(timeratom);  //原子操作完毕？？？？？

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
// 函数功能:__SCM6xxTimer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __SCM6xxTimer_SetIntPro(struct SCM6xxTimerHandle  *timer, bool_t real_prior)
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
// 函数功能:__SCM6xxTimer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __SCM6xxTimer_EnInt(struct SCM6xxTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        return Int_ContactLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__SCM6xxTimer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __SCM6xxTimer_DisInt(struct SCM6xxTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return Int_CutLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__SCM6xxTimer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
bool_t __SCM6xxTimer_GetTime(struct SCM6xxTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    u16 cntValueH,cntValueL;
    u32 cntValue;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->TC;
        cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->TC;
        cntValue  =  (cntValueH<<16)|cntValueL;
        counter   = cntValue;                  //CNT即是微秒数
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
bool_t __SCM6xxTimer_GetReg(struct SCM6xxTimerHandle  *timer, void **reg)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            return false;
        }
        *reg = sgpt_TimerReg[timerno].mp_TimerReg_L;
        return true;
    }
    else
    {
        return false;
    }
}

// ========================= ====================================================
// 函数功能:__SCM6xxTimer_CheckTimeout
//          看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __SCM6xxTimer_CheckTimeout(struct SCM6xxTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    u16 cntValueH,cntValueL;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_SCM6XXTIMER_MAX)
        {
            result = false;
        }
        else
        {

            cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->TC;
            cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->TC;
            counter   = (cntValueH << 16) | cntValueL;

            cntValueL = sgpt_TimerReg[timerno].mp_TimerReg_L->MR;
            cntValueH = sgpt_TimerReg[timerno].mp_TimerReg_H->MR;

            temp =  (cntValueH << 16) | cntValueL;

            if(temp == counter)
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
// 函数功能：__SCM6xxTimer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __SCM6xxTimer_GetID(struct SCM6xxTimerHandle   *timer,u32 *timerId)
{
    u16 irqno;
    u16 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        irqno = (u16)timer->timerno;
        timerno = (u16)timer->irqline;
        *timerId = (timerno<<16)|(irqno);
        return true;
    }
}
// =============================================================================
// 函数功能：__SCM6xxTimer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __SCM6xxTimer_GetCycle(struct SCM6xxTimerHandle   *timer, u32 *cycle)
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
// 函数功能：__SCM6xxTimer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __SCM6xxTimer_GetState(struct SCM6xxTimerHandle   *timer, u32 *timerflag)
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
// 函数功能:__SCM6xxTimer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  : true成功  false失败
// 说明    :
// =============================================================================
bool_t __SCM6xxTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct SCM6xxTimerHandle  *timer;
    timer = (struct SCM6xxTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __SCM6xxTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __SCM6xxTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __SCM6xxTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __SCM6xxTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __SCM6xxTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __SCM6xxTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __SCM6xxTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __SCM6xxTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __SCM6xxTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __SCM6xxTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __SCM6xxTimer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = __SCM6xxTimer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// 函数功能:__P1020PicTimer_GetFreq
//       获取定时器主频
// 输入参数:timerhandle 待操作的定时器句柄
// 输出参数:
// 返回值  :定时器主频
// 说明    :单位（HZ）
// =============================================================================
u32  __SCM6xxTimer_GetFreq(ptu32_t timerhandle)
{
    //定时器TIM2、5 时钟源为低速外设时钟PCLK1(APB1时钟)，速度为42M，
    //除非APB1分频系数为1，否则通用定时器时钟为APB1的2倍
    //board-config.h中有设置STM32F4主频168M,FCLK时钟168M,AHB总线时钟168M,高速外设时钟CLK2=84M（手册规定不超过84M）
    //低速外设时钟CLK1=42M（手册规定不超过42M），具手册说明体参见
    return 37500000;  //37.5MHz，参见手册RCC_DCKCFGR、TIMx_PSC寄存器
}
// =============================================================================
// 函数功能:module_init_timer
//       P1020的PICtimer初始化,timer时钟源为APB1 = CN_CFG_PCLK1 = 84M(timer234567)
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  SCM6xxtimer;
    u8 i;

    for(i=0;i<CN_SCM6XXTIMER_NUM;i++)
    {
        sgpt_TimerReg[i].mp_TimerReg_L->CR = 0;
        sgpt_TimerReg[i].mp_TimerReg_L->CR = 0;
        //设置为定时模式
        sgpt_TimerReg[i].mp_TimerReg_L->FSR =0;
        sgpt_TimerReg[i].mp_TimerReg_H->FSR =(0xE << 4) | (0x1);

        //设置定时器计数最大值
        sgpt_TimerReg[i].mp_TimerReg_L->MR = 0xffff;
        sgpt_TimerReg[i].mp_TimerReg_H->MR = 0xffff;

        //设置时钟输入全部分频值为不分频
        sgpt_TimerReg[i].mp_TimerReg_L->PR = 0;

                                                //禁止TIMER，这里为什么要禁止TIMER???
                                                //如果定时器中断属性设为实时中断，则用户实现的中断服务函数ISR中必须清
                                                //中断，且不能调用任何系统服务；如果设定为异步信号，则
                                                //无须清中断，且允许调用全部系统调用。
        //使能匹配
        sgpt_TimerReg[i].mp_TimerReg_H->MCR   = 0x3;

        //使能匹配中断
        sgpt_TimerReg[i].mp_TimerReg_H->INTIE = 0x1;

        //out control
        sgpt_TimerReg[i].mp_TimerReg_H->EMR   = 0x3;

        //默认情况下自动重装不使能
        stgTimerHandle[i].autoReload = true;


    }

    SCM6xxtimer.chipname = "SCM610TIMER";
    SCM6xxtimer.HardTimerAlloc = __SCM6xxTimer_Alloc;     //分配定时器
    SCM6xxtimer.HardTimerFree  = __SCM6xxTimer_Free;       //释放定时器
    SCM6xxtimer.HardTimerCtrl  = __SCM6xxTimer_Ctrl;       //控制定时器
    SCM6xxtimer.HardTimerGetFreq = __SCM6xxTimer_GetFreq; //获取定时器计数频率，84Mhz

    HardTimer_RegisterChip(&SCM6xxtimer);                //注册定时器芯片到系统定时器模块

    return true;
}

