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
// =============================================================================

// 文件名     ：cpu_peri_timer.c
// 模块描述: 定时器驱动模块
// 模块版本: V1.0.0
// 创建人员: chj
// 创建时间: 2018-05-07
// =============================================================================
#include "stdio.h"
#include "cpu_peri_timer.h"
#include "cpu_peri_int_line.h"
#include "int.h"
#include "djyos.h"
#include "timer_hard.h"

#include "fsl_gpt.h"
#include "fsl_clock.h"
//#include "clock_config.h"
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
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                   //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence:"Software Timers","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件将强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:none               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@num,32,512,

//%$#@enum,true,false,
//%$#@string,1,30,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Select IPG Clock as PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for PERCLK_CLK clock source */
#define EXAMPLE_GPT_CLOCK_DIVIDER_SELECT (0U)
#define EXAMPLE_GPT_CLOCK_DIV            (3U)
/* Get source clock for GPT driver (GPT prescaler = 0) */
#define EXAMPLE_GPT_CLK_FREQ (CLOCK_GetFreq(kCLOCK_IpgClk) / (EXAMPLE_GPT_CLOCK_DIVIDER_SELECT + 1U))
#define EXAMPLE_GPT_CLOCK_FREQ           ((EXAMPLE_GPT_CLK_FREQ/EXAMPLE_GPT_CLOCK_DIV)/100000)  //单位：MHZ

#define CN_PIT_MAX_COUNTER     0xFFFFFFFF
#define CN_PIT_MAX_TIME_US     (0xFFFFFFFF/EXAMPLE_GPT_CLOCK_FREQ)

#define CN_CHANNEL              kGPT_OutputCompare_Channel1
#define CN_INTEERUPT            kGPT_OutputCompare1InterruptEnable
#define CN_INTEERUPT_FLAG       kGPT_OutputCompare1Flag
typedef GPT_Type    tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                                        (tagTimerReg *)GPT1_BASE,
                                                        (tagTimerReg *)GPT2_BASE
                                                     };

enum ENUM_TIMER
{
    EN_TIMER_1=0,//按顺序赋值
    EN_TIMER_2
};


//各个定时器芯片的定时器应该有自己的句柄
struct TimerHandle
{
    u32     timerno;          //定时器号
    u32     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    fnTimerIsr UserIsr;      //用户中断响应函数
};

#define CN_TIMER_NUM   (EN_TIMER_2 +1)
#define CN_TIMER_MAX    EN_TIMER_2

static struct TimerHandle  stgTimerHandle[CN_TIMER_NUM];

//最高位代表timer0，依次类推
static u32  gs_dwTimerBitmap;  //对于定时器这种东西，一般的不会很多，32个应该足够
#define CN_TIMER_BITMAP_MSK  (0x80000000)  //最高位为1，依次右移即可

//timer0到timern的irq中断
static u32 sgHaltimerIrq[CN_TIMER_NUM]={
                                                CN_INT_LINE_GPT1,
                                                CN_INT_LINE_GPT2
                                            };

//获取32位数第一个0bit位置(从高位到低位算起)
u8 __Timer_GetFirstZeroBit(u32 para)
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
// 函数功能:__Time2Counter
//          将提供的时间换算为定时器的计数单位,
// 输入参数:time，要换算的时间(单位为微秒)
// 输出参数:counter,对应时间的counter
// 返回值  :true成功 fasle失败
// 说明    :time太大会失败，计算的counter则是定时器的最大值
// =============================================================================
bool_t __Time2Counter(u64 time, u64 *counter)
{

    u64 counter_1=time*EXAMPLE_GPT_CLOCK_FREQ;
    *counter = counter_1 > CN_PIT_MAX_COUNTER ? CN_PIT_MAX_COUNTER : counter_1;

    if(*counter == counter_1)
        return true;

    return false;

}

// =============================================================================
// 函数功能:__Counter2Time
//          将定时器的计数单位换算为时间
// 输入参数:counter,定时器的counter单元
// 输出参数:time，换算的时间(单位为微秒)
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t __Counter2Time(u64 counter,u64 *time)
{
    u32 time_1=counter/EXAMPLE_GPT_CLOCK_FREQ;
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
// 函数功能:__Timer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __Timer_PauseCount(struct TimerHandle  *timer)
{
    u32  flag=0;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            GPT_StopTimer((tagTimerReg*)tg_TIMER_Reg[timerno]);
            flag=GPT_GetEnabledInterrupts((tagTimerReg*)tg_TIMER_Reg[timerno]);
            GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], flag);
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
// 函数功能:__Timer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __Timer_StartCount(struct TimerHandle  *timer)
{
    u32 flag=0;
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            flag=GPT_GetEnabledInterrupts((tagTimerReg*)tg_TIMER_Reg[timerno]);
            GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], flag);
            GPT_StartTimer((tagTimerReg*)tg_TIMER_Reg[timerno]);
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
// 函数功能:__Timer_SetCycle,timer,最大为0xFFFFFFFF,根据Timer_ModuleInit
//          设定计数器每计一次数为(1/84)1us，可知最大时间不超-1
//          设定周期
// 输入参数:timer，PIC定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __Timer_SetCycle(struct TimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        else
        {
            GPT_SetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[timerno],\
                                                            CN_CHANNEL, cycle);
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
// 函数功能:__Timer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t  __Timer_SetAutoReload(struct TimerHandle  *timer, bool_t autoreload)
{
    bool_t result;
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno <= CN_TIMER_MAX)
        {
            if(autoreload == true)
            {
                return true;
            }
            else
            {
                return false;
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
u32 __Timer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    timerno = ((struct TimerHandle  *)TimerHandle)->timerno;
    GPT_ClearStatusFlags((tagTimerReg*)tg_TIMER_Reg[timerno], CN_INTEERUPT_FLAG);
    Int_ClearLine(((struct TimerHandle  *)TimerHandle)->irqline);
    return ((struct TimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// 函数功能:__Timer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __Timer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct TimerHandle  *timer;
    ptu32_t timerhandle;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = __Timer_GetFirstZeroBit(gs_dwTimerBitmap);
    if(timerno < CN_TIMER_NUM)//还有空闲的，则设置标志位
    {
        gs_dwTimerBitmap = gs_dwTimerBitmap | (CN_TIMER_BITMAP_MSK<< timerno);
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
    __Timer_PauseCount(timer);
    //__Timer_SetCycle(timer,cycle);
    //设置定时器中断,先结束掉该中断所有的关联相关内容
    Int_Register(irqline);
    Int_CutLine(irqline);

    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);

    Int_SetClearType(irqline,CN_INT_CLEAR_USER);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    Int_IsrConnect(irqline, __Timer_isr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}

// =============================================================================
// 函数功能:__Timer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __Timer_Free(ptu32_t timerhandle)
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
        if(timerno < CN_TIMER_NUM)//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            gs_dwTimerBitmap = gs_dwTimerBitmap &(~(CN_TIMER_BITMAP_MSK<< timerno));
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
// 函数功能:__Timer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __Timer_SetIntPro(struct TimerHandle  *timer, bool_t real_prior)
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
// 函数功能:__Timer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __Timer_EnInt(struct TimerHandle  *timer)
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
// 函数功能:__Timer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __Timer_DisInt(struct TimerHandle  *timer)
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
// 函数功能:__Timer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
bool_t __Timer_GetTime(struct TimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->CNT;//CNT即是微秒数
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}

// ========================= ====================================================
// 函数功能:__Timer_CheckTimeout
//          看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __Timer_CheckTimeout(struct TimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_TIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp=GPT_GetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[timerno],\
                                                                        CN_CHANNEL);
            counter = tg_TIMER_Reg[timerno]->CNT;
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
// 函数功能：__Timer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __Timer_GetID(struct TimerHandle   *timer,u32 *timerId)
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
// 函数功能：__Timer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __Timer_GetCycle(struct TimerHandle   *timer, u32 *cycle)
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
// 函数功能：__Timer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __Timer_GetState(struct TimerHandle   *timer, u32 *timerflag)
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
// 函数功能:__Timer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  : true成功  false失败
// 说明    :
// =============================================================================
bool_t __Timer_Ctrl(ptu32_t timerhandle, \
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
                result = __Timer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __Timer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __Timer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __Timer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __Timer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __Timer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __Timer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __Timer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __Timer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __Timer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __Timer_GetState(timer, (u32 *)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}

// =============================================================================
// 函数功能:__Timer_GetFreq
//       获取定时器主频
// 输入参数:timerhandle 待操作的定时器句柄
// 输出参数:
// 返回值  :定时器主频
// 说明    :单位（HZ）
// =============================================================================
u32  __Timer_GetFreq(ptu32_t timerhandle)
{
    return (EXAMPLE_GPT_CLK_FREQ/EXAMPLE_GPT_CLOCK_DIV);
}

// =============================================================================
// 函数功能:module_init_timer
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  timer;
    gpt_config_t gptConfig;
    u8 i;

    /*Clock setting for GPT*/
    CLOCK_SetMux(kCLOCK_PerclkMux, EXAMPLE_GPT_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_PerclkDiv, EXAMPLE_GPT_CLOCK_DIVIDER_SELECT);
    GPT_GetDefaultConfig(&gptConfig);

    for(i=0;i<CN_TIMER_NUM;i++)
    {
        GPT_Init((tagTimerReg*)tg_TIMER_Reg[i], &gptConfig);
        GPT_SetClockDivider((tagTimerReg*)tg_TIMER_Reg[i], EXAMPLE_GPT_CLOCK_DIV);
        GPT_SetOutputCompareValue((tagTimerReg*)tg_TIMER_Reg[i], CN_CHANNEL, 0XFFFFFFFF);
        GPT_EnableInterrupts((tagTimerReg*)tg_TIMER_Reg[i], CN_INTEERUPT);
        GPT_StopTimer((tagTimerReg*)tg_TIMER_Reg[i]);
    }

    timer.chipname = "TIMER";
    timer.HardTimerAlloc = __Timer_Alloc;     //分配定时器
    timer.HardTimerFree = __Timer_Free;       //释放定时器
    timer.HardTimerCtrl = __Timer_Ctrl;       //控制定时器
    timer.HardTimerGetFreq = __Timer_GetFreq; //获取定时器计数频率

    HardTimer_RegisterChip(&timer);                //注册定时器芯片到系统定时器模块

    return true;
}
