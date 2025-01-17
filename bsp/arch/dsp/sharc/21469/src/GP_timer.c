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
// 模块描述: SHARC21469 GP TIMER的驱动
// 模块版本: V1.00
// 创建人员: zhuhb_cyg
// 创建时间: 3:27:12 PM/June 11, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注：由于提供给timer_core.c使用，所以不再做参数检查之类的
#include "timer_hard.h"
#include <cdef21469.h>
#include <def21489.h>
#include "misc_config.h"
#include "board_config.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri_int_line.h"
#include "stdint.h"

#include <stdio.h>

#define CN_GP_TIMER_COUNT_MAX  0x7FFFFFFF
#define CN_GP_TIMER_NUM        2


//各个定时器芯片的定时器应该有自己的句柄
struct GPTimerHandle
{
    u32     timerno;          //定时器号
    u32     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
};


static struct GPTimerHandle  s_tGPTimerHandle[2];

static u32 sgHaltimerIrq[2]={cn_int_line_GPTMR0I,cn_int_line_GPTMR1I};

static int s_s32GPTimerHandle = 0;
static int s_s32Timer1Counter = 0;


// =============================================================================
// 函数功能:__GPTimer_Time2Counter
//          将提供的时间换算为定时器的计数单位
// 输入参数:time，要换算的时间(单位为微秒)
// 输出参数:counter,对应时间的counter
// 返回值  :true成功 fasle失败
// 说明    :time太大会失败，计算的counter则是定时器的最大值
// =============================================================================
bool_t __GPTimer_Time2Counter(u32 time, u32 *counter)
{
    u32 time_max;
    u32 time_set;
    time_max = (u32)(2*(2^31-1)*(10^6/CN_CFG_PCLK1));    //CN_CFG_PCLK1=200MHz时，time_max=21.474836s=21474836us.
    time_set = time > time_max ? time_max : time;
    if(time_max > time)
    {
        time_set = time;
        *counter =  (time_set *CN_CFG_PCLK1)/(2*10^6);
        return true;
    }
    else
    {
        time_set = time_max;
        *counter =  (time_set *CN_CFG_PCLK1)/(2*10^6);
        return false;
    }
}
// =============================================================================
// 函数功能:__GPTimer_Counter2Time
//          将定时器的计数单位换算为时间
// 输入参数:counter,定时器的counter单元
// 输出参数:time，换算的时间(单位为微秒)
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __GPTimer_Counter2Time(u32 counter,u32 *time)
{
    u32 time_max;

    time_max = (u32)(2*CN_GP_TIMER_COUNT_MAX *10^6)/CN_CFG_PCLK1;


    if(counter > CN_GP_TIMER_COUNT_MAX)
    {
        *time = time_max;
        return false;
    }
    else
    {
        *time = (u32)(2*counter*10^6)/(CN_CFG_PCLK1);
        return true;
    }
}
// =============================================================================
// 函数功能:__GPTimer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================

bool_t __GPTimer_PauseCount(struct GPTimerHandle  *timer)
{
    u8 timerno;
    u32 temp;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_GP_TIMER_NUM-1)
        {
            return false;
        }
        else
        {
           if(timerno==0)
            {
                *pTMSTAT |= (~TIM0EN);
            }
            else
            {
                *pTMSTAT |= (~TIM1EN);
            }

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
// 函数功能:__GPTimer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __GPTimer_StartCount(struct GPTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_GP_TIMER_NUM-1)
        {
            return false;
        }
        else

        {
            if(timerno==0)
            {
                *pTMSTAT |= TIM0EN;
            }
            else
            {
                *pTMSTAT |= TIM1EN;
            }

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
// 函数功能:__GPTimer_SetCycle
//          设定周期
// 输入参数:timer，GP定时器
//          cycle,周期（微秒），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __GPTimer_SetCycle(struct GPTimerHandle  *timer, u32 cycle)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        __GPTimer_Time2Counter(cycle,&counter);

        timerno = timer->timerno;

        if(timerno==0)
        {
            *pTM0PRD = counter&CN_GP_TIMER_COUNT_MAX;
                return true;
        }

        else if(timerno==1)
        {
            *pTM1PRD = counter&CN_GP_TIMER_COUNT_MAX;
                return true;
        }
        else
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
// 函数功能:__GPTimer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明：SHARC21469 GP_Timer0/GP_Timer1默认为reload，要想改变周期，需要先disable，然后再重新设置周期。
// =============================================================================

bool_t  __GPTimer_SetAutoReload(struct GPTimerHandle  *timer, bool_t autoreload)
{

    if(timer->timerstate&CN_TIMER_ENUSE)
    {
        if(autoreload)
        {
            timer->timerstate= timer->timerstate | CN_TIMER_RELOAD;
        }
        else
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
// 函数功能:__GPTimer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __GPTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 i=0;
    u8 irqline;
    struct GPTimerHandle  *timer;
    ptu32_t timerhandle;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    timer=&s_tGPTimerHandle[0];
    if(!timer->timerstate)
    {
        timerno=timer->timerno;

    }
    else
    {
        timer=&s_tGPTimerHandle[1];
        if(!timer->timerstate)
        {
            timerno=timer->timerno;

        }
        else   //没有的话直接返回就可以了，用不着再啰嗦了
        {
            return NULL;
        }
    }
    Int_LowAtomEnd(timeratom);  //原子操作完毕

    irqline = sgHaltimerIrq[timerno];
    timer = &s_tGPTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    //好了，中断号和定时器号码都有了，该干嘛就干嘛了。
    //先设置好定时器周期
    //__P1020PicTimer_PauseCount(timer);
//    __GPTimer_SetCycle(timer,cycle);
    //设置定时器中断,先结束掉该中断所有的关联相关内容
    Int_Register(irqline);
    Int_CutLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_EvttDisConnect(irqline);
    Int_SettoAsynSignal(irqline);
    Int_IsrConnect(irqline, timerisr);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// 函数功能:__GPTimer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __GPTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //保护公用资源
    struct GPTimerHandle  *timer;
    timer = (struct GPTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_GP_TIMER_NUM )//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            //解除掉中断所关联的内容
            timer->timerstate = 0;
            Int_CutLine(irqline);
            Int_IsrDisConnect(irqline);
            Int_EvttDisConnect(irqline);
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
// 函数功能:__P1020PicTimer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __GPTimer_SetIntPro(struct GPTimerHandle  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            return false;
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            //return Int_SettoAsynSignal(timer->irqline);
            return true;
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// 函数功能:__GPTimer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __GPTimer_EnInt(struct GPTimerHandle  *timer)
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
// 函数功能:__GPTimer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __GPTimer_DisInt(struct GPTimerHandle  *timer)
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
// 函数功能:__GPTimer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间
// =============================================================================
bool_t __GPTimer_GetTime(struct GPTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >=CN_GP_TIMER_NUM)
        {
            return false;
        }
        else
        {
            if(timerno==0)
            {
                *pTMSTAT=(~TIM0EN);
                counter=*pTM0CNT;
                temp=*pTM0PRD;
                counter=counter-(0xFFFFFFFF-temp);
            }
            else
            {
                *pTMSTAT=(~TIM1EN);
                counter=*pTM1CNT;
                temp=*pTM1PRD;
                counter=counter-(0xFFFFFFFF-temp);
            }
            return __GPTimer_Counter2Time(counter, time);
        }
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__GPTimer_CheckTimeout
//          看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __GPTimer_CheckTimeout(struct GPTimerHandle  *timer, bool_t *timeout)
{
    bool_t result;
    u8 timerno;
    u32 temp;
    u32 counter;
    u32 *time;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno >=CN_GP_TIMER_NUM)
        {
            result = false;
        }
        else
        {
            result = true;

            __GPTimer_GetTime(timer, time);
            if(*time>timer->cycle)
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
// 函数功能：_GPTimer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __GPTimer_GetID(struct GPTimerHandle   *timer,u32 *timerId)
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
// 函数功能：_GPTimer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(微秒)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __GPTimer_GetCycle(struct GPTimerHandle   *timer, u32 *cycle)
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
// 函数功能：_GPTimer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __GPTimer_GetState(struct GPTimerHandle   *timer, u32 *timerflag)
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
// 函数功能:__P1020PicTimer_Ctrl
//            定时器控制命令
// 输入参数:timerhandle 待操作的定时器句柄
//          ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  :true成功  false失败
// 说明    :
// =============================================================================
bool_t __GPTimer_Ctrl(ptu32_t timerhandle, \
                         enum TimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result;
    struct GPTimerHandle  *timer;
    timer = (struct GPTimerHandle  *)timerhandle;
    if(NULL == timer)
    {
        result = false;
    }
    else
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __GPTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __GPTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __GPTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __GPTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __GPTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __GPTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __GPTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __GPTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __GPTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __GPTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __GPTimer_GetState(timer, (u32 *)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}

// =============================================================================
// 函数功能:__GPTimer0_ISR
//           GP Timer0中断服务函数，清除中断状态位
// 输入参数:GPTMR1I_int_line GP Timer0中断号
// 输出参数:无
// 返回值  :无
// 说明    :
// =============================================================================
void __GPTimer0_ISR(ptu32_t GPTMR0I_int_line)
{
    //MUST clear timer0 interrupt status first
    *pTMSTAT = TIM0IRQ;

    asm("nop;");

    s_s32GPTimerHandle++;
    printf("GP Timer0 interrupt  \n"); //Cost too much cycle in printf, so just for demo. Need to remove it.

    //return 1;
}

// =============================================================================
// 函数功能:__GPTimer1_ISR
//           GP Timer1中断服务函数，清除中断状态位
// 输入参数:GPTMR1I_int_line GP Timer1中断号
// 输出参数:无
// 返回值  :无
// 说明    :
// =============================================================================
void __GPTimer1_ISR(ptu32_t GPTMR1I_int_line)
{
    //MUST clear timer1 interrupt status first
    *pTMSTAT = TIM1IRQ;
    asm("nop;");

    s_s32Timer1Counter++;
    printf("GP Timer1 interrupt  \n");  //Cost too much cycle in printf, so just for demo. Need to remove it.
}



// =============================================================================
// 函数功能:module_init_timer
//          SHARC21469的GP timer初始化
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
void GPTimer_ModuleInit(void)
{
    struct TimerChip  Sharc21469GPtimer;

    Int_Register(cn_int_line_GPTMR0I);
    Int_IsrConnect(cn_int_line_GPTMR0I,__GPTimer0_ISR);
    Int_SettoAsynSignal(cn_int_line_GPTMR0I);
    Int_ClearLine(cn_int_line_GPTMR0I);     //清掉初始化产生的发送fifo空的中断
    Int_RestoreAsynLine(cn_int_line_GPTMR0I);

    Int_Register(cn_int_line_GPTMR1I);
    Int_IsrConnect(cn_int_line_GPTMR1I,__GPTimer1_ISR);
    Int_SettoAsynSignal(cn_int_line_GPTMR1I);
    Int_ClearLine(cn_int_line_GPTMR1I);     //清掉初始化产生的发送fifo空的中断
    Int_RestoreAsynLine(cn_int_line_GPTMR1I);


    *pTM0CTL = TIMODEPWM | PRDCNT | IRQEN;      // configure the timer0
    *pTM0W = 1;                             // timer0 width

    *pTM1CTL = TIMODEPWM | PRDCNT | IRQEN;      // configure the timer1
    *pTM1W = 1;                             // timer1 width register


    Sharc21469GPtimer.chipname = "SHARC21469GPTimer";
    Sharc21469GPtimer.HardTimerAlloc = __GPTimer_Alloc;
    Sharc21469GPtimer.HardTimerFree = __GPTimer_Free;
    Sharc21469GPtimer.HardTimerCtrl = __GPTimer_Ctrl;
    HardTimer_RegisterChip(&Sharc21469GPtimer);


    // =============下面代码仅做测试使用===================================
/*
struct GPTimerHandle *testTimer;
//step1:分配一个定时器
  testTimer=__GPTimer_Alloc(30000000, __GPTimer0_ISR);
    //step2:设置定时周期
   // __GPTimer_SetCycle(testTimer, 30000000);
    //step3:使能中断
    __GPTimer_EnInt(testTimer);
    //开始计数
    __GPTimer_StartCount(testTimer);
   */

}
