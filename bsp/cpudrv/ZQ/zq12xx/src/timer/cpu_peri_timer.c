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
//todo: 修改注释\文件名等.
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
//#include "cfg/misc_config.h"
#include "int.h"
#include "cpu.h"
#ifdef CFG_CPU_ZQ12XX_M0
#include "cpu_peri_int_line.h"
#endif
#include "timer_hard.h"

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
//parent:"Software Timers"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"Software Timers","int"//该组件的依赖组件名（可以是none，表示无依赖组件），
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

#ifdef CFG_CPU_ZQ12XX_M0
typedef struct _TIMER
{
    vu32 COUNTER;
    vu32 COMPARE;
    vu32 CONTROL;
    vu32 CLKSEL;
}tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                    (tagTimerReg *)0x41860000,      //TIMER1
                                    (tagTimerReg *)0x41860010,      //TIMER2
                                    };

enum ENUM_CPU_TIMER
{
    EN_CPU_TIMER_1=0,
    EN_CPU_TIMER_2
};

//#define CN_TIMER_DIV     (CN_CFG_PCLK1/1000000)

//各个定时器芯片的定时器应该有自己的句柄
struct CPUTimerHandle
{
    bool_t  autoReloadSet;    //自动重装载设置
    u32     timerno;          //定时器号
    u32     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    fnTimerIsr UserIsr;      //用户中断响应函数,对于多个定时器共用 一条中断线,,操作句柄参数传进的永远只有一个
};

enum TIM_RELOAD_SET{

    EN_RELOAD_NOT_SET = 0,    //设置 不自动重装载
    EN_RELOAD_SET  ,          //设置 自动重装载

};

#define CN_CPUTIMER_NUM   (EN_CPU_TIMER_2 +1)
#define CN_CPUTIMER_MAX    EN_CPU_TIMER_2


static struct CPUTimerHandle  stgTimerHandle[CN_CPUTIMER_NUM];

//最高位代表timer0 依次类推
static u32  gs_dwCPUTimerBitmap;  //对于定时器这种东西，一般的不会很多，32个应该足够
#define CN_CPUTIMER_BITMAP_MSK  (0x80000000)  //最高位为1，依次右移即可

//timer0..timern的irq
static u8 spTimerIrqOffset[CN_CPUTIMER_NUM]={   0,1  };

#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x620)
#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x520)
//获取32位数第一个0bit位置(从高位到低位算起)
u8 __CPUTimer_GetFirstZeroBit(u32 para)
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
// 函数功能:__CPUTimer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_PauseCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->CONTROL = 0;         //停止计数器，关中断
            tg_TIMER_Reg[timerno]->COUNTER = 0;         //清计数器
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
// 函数功能:__CPUTimer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_StartCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COUNTER    = 0;
            tg_TIMER_Reg[timerno]->CONTROL &= ~(1 << 1);            //清中断标识
            tg_TIMER_Reg[timerno]->CONTROL    = (1<<0)|(1<<2);      //timer and int
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
// 函数功能:__CPUTimer_SetCycle,CPU的timer,最大为0xFFFFFFFF,根据Timer_ModuleInit
//          设定计数器每计一次数为(1/108)1us，可知最大时间不超-1
//          设定周期
// 输入参数:timer，PIC定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __CPUTimer_SetCycle(struct CPUTimerHandle  *timer, u32 cycle)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COMPARE = cycle;
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
// 函数功能:__CPUTimer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t  __CPUTimer_SetAutoReload(struct CPUTimerHandle  *timer, bool_t autoreload)
{
    if(autoreload == true)
    {
        timer->autoReloadSet = EN_RELOAD_SET;
    }
    else{
        timer->autoReloadSet = EN_RELOAD_NOT_SET;
    }


    return true;        //reload every time
}

//-----------------------------------------------------------------------------
// 功能：定时器中断响应函数，对于像atmel m7这样，需要自行操作定时器的寄存器才能
//       清中断的定时器，必须在这里实现ISR，然后间接调用user ISR，否则在
//       user isr中将无所适从。
// 参数：定时器句柄。
// 返回：user ISR的返回值
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __CPUTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno;
    struct CPUTimerHandle  *timer = (struct CPUTimerHandle  *)TimerHandle;

    timerno = timer->timerno;
//  if((((u32*)TIMER_IRQ_STA_ADDR)[0])  & (1<< spTimerIrqOffset[0]))
//  {
//      timerno = 0;
//  }
//  else if((((u32*)TIMER_IRQ_STA_ADDR )[0])&(1<<spTimerIrqOffset[1]))
//  {
//      timerno = 1;
//  }

//  timer = &stgTimerHandle[timerno];
    tg_TIMER_Reg[timerno]->CONTROL = (1<<0) | (1<<2);

    if(timer->autoReloadSet == EN_RELOAD_NOT_SET)
    {
        tg_TIMER_Reg[timerno]->CONTROL = 0;         //停止计数器，关中断
//        tg_TIMER_Reg[timerno]->COUNTER = 0;         //清计数器
    }
    return (timer->UserIsr(TimerHandle));
}

// =============================================================================
// 函数功能:__CPUTimer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __CPUTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct CPUTimerHandle  *timer;
    ptu32_t timerhandle;
    u8 subIntLine;

    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = __CPUTimer_GetFirstZeroBit(gs_dwCPUTimerBitmap);
    if(timerno < CN_CPUTIMER_NUM)//还有空闲的，则设置标志位
    {
        gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap | (CN_CPUTIMER_BITMAP_MSK>> timerno);

        Int_LowAtomEnd(timeratom);  //原子操作完毕
    }
    else//没有的话直接返回就可以了，用不着再啰嗦了
    {
        Int_LowAtomEnd(timeratom);   //原子操作完毕
        return 0;
    }

    irqline = CN_INT_LINE_TIMER;
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;

    //好了，中断号和定时器号码都有了，该干嘛就干嘛了。
    //先设置好定时器周期
    __CPUTimer_PauseCount(timer);
    Int_SetIsrPara(irqline,(ptu32_t)timer);

    switch(timerno)
    {
        case EN_CPU_TIMER_1:
             subIntLine  = CN_SUBID_TIMER_0;
             break;
        case EN_CPU_TIMER_2:
             subIntLine  = CN_SUBID_TIMER_1;
             break;
        default:printk("no timer to use\r\n");break;
    }

    //默认情况下不设置自动重装载
    timer->autoReloadSet = EN_RELOAD_NOT_SET;

    djybsp_isr_hdl_register(irqline, subIntLine, __CPUTimer_isr,0);

    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// 函数功能:__CPUTimer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //保护公用资源
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_CPUTIMER_NUM)//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap &(~(CN_CPUTIMER_BITMAP_MSK<< timerno));
            //解除掉中断所关联的内容
            timer->timerstate = 0;
            timer->autoReloadSet = EN_RELOAD_NOT_SET;
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
// 函数功能:__CPUTimer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __CPUTimer_SetIntPro(struct CPUTimerHandle  *timer, bool_t real_prior)
{      //多个外设定时器共用共用中断线，这里不设置
//    if(timer->timerstate & CN_TIMER_ENUSE)
//    {
//        if(real_prior)
//        {
//            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
//            Int_EvttDisConnect(timer->irqline);
//            return Int_SettoReal(timer->irqline);
//        }
//        else
//        {
//            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
//            return Int_SettoAsynSignal(timer->irqline);
//        }
//    }
//    else
//    {
//        return false;
//    }

    return false;
}

// =============================================================================
// 函数功能:__CPUTimer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_EnInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        *(u32*)TIMER_IRQ_ENABLE_ADDR |= (1<<spTimerIrqOffset[timer->timerno]);

        return Int_ContactLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_DisInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        *(u32*)TIMER_IRQ_ENABLE_ADDR &= ~(1<<spTimerIrqOffset[timer->timerno]);
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return Int_CutLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
bool_t __CPUTimer_GetTime(struct CPUTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->COUNTER;   //CNT即是微秒数
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_CheckTimeout
//          看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_CheckTimeout(struct CPUTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->COMPARE;
            counter = tg_TIMER_Reg[timerno]->COUNTER;
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
// 函数功能：__CPUTimer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __CPUTimer_GetID(struct CPUTimerHandle   *timer,u32 *timerId)
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
// 函数功能：__CPUTimer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __CPUTimer_GetCycle(struct CPUTimerHandle   *timer, u32 *cycle)
{
    u8 timerno;

    if(NULL == timer)
    {
        return false;
    }
    else
    {
        timerno =  timer->timerno;
        tg_TIMER_Reg[timerno]->COMPARE = cycle;
        *cycle = timer->cycle;
        return true;
    }
}
// =============================================================================
// 函数功能：__CPUTimer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __CPUTimer_GetState(struct CPUTimerHandle   *timer, u32 *timerflag)
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
// 函数功能:__CPUTimer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  :true成功  false失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __CPUTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __CPUTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __CPUTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __CPUTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __CPUTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __CPUTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __CPUTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __CPUTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __CPUTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __CPUTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __CPUTimer_GetState(timer, (u32 *)inoutpara);
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
u32  __CPUTimer_GetFreq(ptu32_t timerhandle)
{
    return (CN_CFG_TIMER_CLK/128);
}
// =============================================================================
// 函数功能:module_init_timer
//          P1020的PICtimer初始化,timer时钟源为APB2 = CN_CFG_PCLK1 = 108M(timer234567)
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  CPUtimer;
    u8 i;
    u8 irqline;

    for(i=0;i<CN_CPUTIMER_NUM;i++)
    {
        tg_TIMER_Reg[i]->CONTROL = 0;           //禁止TIMER
        tg_TIMER_Reg[i]->COUNTER = 0;           //计数器清0
        tg_TIMER_Reg[i]->COMPARE = 0xFFFFFFFF;
        tg_TIMER_Reg[i]->CLKSEL  = 7;           //时钟源为80M，128分频
    }

//    //由原先分配定时器的移动到这里
//     irqline = CN_INT_LINE_TIMER;     //对已经分配得到的stgTimerHandle 进行初始化
//    //设置定时器中断,先结束掉该中断所有的关联相关内容
//      Int_Register(irqline);
//      Int_CutLine(irqline);
//
//      Int_IsrDisConnect(irqline);
//      Int_EvttDisConnect(irqline);
//      Int_SettoAsynSignal(irqline);
//
//      Int_SetClearType(irqline,CN_INT_CLEAR_USER);
//     // Int_SetIsrPara(irqline,(ptu32_t)timer);//这里设置timer 中断发生是传入timer 作为句柄进行操作,这里不能用了
//    //对于多个定时器共用一条中断线这种情况，中断传递的参数永远都只是第一个分配的定时器，这里，原先通过中断
//    //传递进来的定时器句柄对定时器做相关的操作不适用了，故该参数不在设置.
//    //因为所有定时器公用一个中断线,timer 定时器操作永远为最后一个timerhandle。该参数在调用各个函数会传入
//      Int_IsrConnect(irqline, __CPUTimer_isr);

    CPUtimer.chipname = "CPUTIMER";
    CPUtimer.HardTimerAlloc = __CPUTimer_Alloc;
    CPUtimer.HardTimerFree = __CPUTimer_Free;
    CPUtimer.HardTimerCtrl = __CPUTimer_Ctrl;
    CPUtimer.HardTimerGetFreq = __CPUTimer_GetFreq;
    HardTimer_RegisterChip(&CPUtimer);

    return true;
}
#endif

#ifdef CFG_CPU_ZQ12XX_CK
typedef struct _TIMER
{
    vu32 COUNTER;
    vu32 COMPARE;
    vu32 CONTROL;
    vu32 CLKSEL;
}tagTimerReg;

static tagTimerReg volatile * const tg_TIMER_Reg[] = {
                                    (tagTimerReg *)0x41860000,      //TIMER1
                                    (tagTimerReg *)0x41860010,      //TIMER2
                                    };

enum ENUM_CPU_TIMER
{
    EN_CPU_TIMER_1=0,
    EN_CPU_TIMER_2
};

//#define CN_TIMER_DIV     (CN_CFG_PCLK1/1000000)

//各个定时器芯片的定时器应该有自己的句柄
struct CPUTimerHandle
{
    u32     timerno;          //定时器号
    u32     irqline;          //中断号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    bool_t  auto_reload;      //自动重装载
    fnTimerIsr UserIsr;            //用户中断响应函数,对于多个定时器共用 一条中断线,,操作句柄参数传进的永远只有一个
};

#define CN_CPUTIMER_NUM   (EN_CPU_TIMER_2 +1)
#define CN_CPUTIMER_MAX    EN_CPU_TIMER_2

static struct CPUTimerHandle  stgTimerHandle[CN_CPUTIMER_NUM];

//最高位代表timer0 依次类推
static u32  gs_dwCPUTimerBitmap;  //对于定时器这种东西，一般的不会很多，32个应该足够
#define CN_CPUTIMER_BITMAP_MSK  (0x80000000)  //最高位为1，依次右移即可

//timer0..timern的irq
static u8 spTimerIrqOffset[CN_CPUTIMER_NUM]={   0,1  };

//#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x620)
//#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x520)
#define TIMER_IRQ_ENABLE_ADDR                (0x42030000+0x220)
#define TIMER_IRQ_STA_ADDR                      (0x42030000+0x120)

//获取32位数第一个0bit位置(从高位到低位算起)
u8 __CPUTimer_GetFirstZeroBit(u32 para)
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
// 函数功能:__CPUTimer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_PauseCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->CONTROL = 0;         //停止计数器，关中断
            tg_TIMER_Reg[timerno]->COUNTER = 0;         //清计数器
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
// 函数功能:__CPUTimer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_StartCount(struct CPUTimerHandle  *timer)
{
    u8 timerno;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COUNTER    = 0;
            tg_TIMER_Reg[timerno]->CONTROL &= ~(1 << 1);               //清中断标识
            tg_TIMER_Reg[timerno]->CONTROL    = (1<<0)|(1<<2);      //timer and int
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
// 函数功能:__CPUTimer_SetCycle,CPU的timer,最大为0xFFFFFFFF,根据Timer_ModuleInit
//          设定计数器每计一次数为(1/108)1us，可知最大时间不超-1
//          设定周期
// 输入参数:timer，PIC定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
bool_t  __CPUTimer_SetCycle(struct CPUTimerHandle  *timer, u32 cycle)
{
    u8 timerno;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        else
        {
            tg_TIMER_Reg[timerno]->COMPARE = cycle;
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
// 函数功能:__CPUTimer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================
bool_t  __CPUTimer_SetAutoReload(struct CPUTimerHandle  *timer, bool_t autoreload)
{
    timer->auto_reload = autoreload;
    return true;        //reload every time
}

//-----------------------------------------------------------------------------
// 功能：定时器中断响应函数，对于像atmel m7这样，需要自行操作定时器的寄存器才能
//       清中断的定时器，必须在这里实现ISR，然后间接调用user ISR，否则在
//       user isr中将无所适从。
// 参数：定时器句柄。
// 返回：user ISR的返回值
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 __CPUTimer_isr(ptu32_t TimerHandle)
{
    u32 timerno = (struct CPUTimerHandle  *)TimerHandle->timerno;

    //以下两句顺序不能改
    silan_timer_irq_reset(TIMER_ADDR(timerno));
    if(!(stgTimerHandle[timerno].auto_reload))
        silan_timer_disable(TIMER_ADDR(timerno));

    return ((struct CPUTimerHandle  *)TimerHandle)->UserIsr(TimerHandle);
}

// =============================================================================
// 函数功能:__CPUTimer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
ptu32_t __CPUTimer_Alloc(fnTimerIsr timerisr)
{
    u8 timerno;
    u8 irqline;
    struct CPUTimerHandle  *timer;
    ptu32_t timerhandle;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = __CPUTimer_GetFirstZeroBit(gs_dwCPUTimerBitmap);
    if(timerno < CN_CPUTIMER_NUM)//还有空闲的，则设置标志位
    {
        gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap | (CN_CPUTIMER_BITMAP_MSK>> timerno);
        Int_LowAtomEnd(timeratom);  //原子操作完毕
    }
    else//没有的话直接返回就可以了，用不着再啰嗦了
    {
        Int_LowAtomEnd(timeratom);   //原子操作完毕
        return 0;
    }

    irqline = CN_INT_LINE_TIMER;
    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->irqline = irqline;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    //好了，中断号和定时器号码都有了，该干嘛就干嘛了。
    //先设置好定时器周期
    __CPUTimer_PauseCount(timer);
    Int_SetIsrPara(irqline,(ptu32_t)timer);
    djybsp_isr_hdl_register(CN_INT_LINE_TIMER, CN_SUBID_TIMER_0 + timer->timerno, __CPUTimer_isr,0);
    timerhandle = (ptu32_t)timer;

    return timerhandle;
}


// =============================================================================
// 函数功能:__CPUTimer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_Free(ptu32_t timerhandle)
{
    u8 timerno;
    u8 irqline;
    atom_low_t  timeratom;  //保护公用资源
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        irqline = timer->irqline;
        if(timerno < CN_CPUTIMER_NUM)//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            gs_dwCPUTimerBitmap = gs_dwCPUTimerBitmap &(~(CN_CPUTIMER_BITMAP_MSK<< timerno));
            //解除掉中断所关联的内容
            timer->timerstate = 0;
            silan_timer_disable(TIMER_ADDR(timerno));
            silan_timer_count(TIMER_ADDR(timerno), 0);
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
// 函数功能:__CPUTimer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
bool_t  __CPUTimer_SetIntPro(struct CPUTimerHandle  *timer, bool_t real_prior)
{      //多个外设定时器共用共用中断线，这里不设置
    return false;
}

// =============================================================================
// 函数功能:__CPUTimer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_EnInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        *(u32*)TIMER_IRQ_ENABLE_ADDR |= (1<<spTimerIrqOffset[timer->timerno]);
        silan_timer_irq_enable(TIMER_ADDR(timer->timerno));
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
bool_t  __CPUTimer_DisInt(struct CPUTimerHandle  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        *(u32*)TIMER_IRQ_ENABLE_ADDR &= ~(1<<spTimerIrqOffset[timer->timerno]);
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        silan_timer_irq_disable(TIMER_ADDR(timer->timerno));
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
bool_t __CPUTimer_GetTime(struct CPUTimerHandle  *timer, u32 *time)
{
    u8 timerno;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            return false;
        }
        counter = tg_TIMER_Reg[timerno]->COUNTER;   //CNT即是微秒数
        *time = counter;
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:__CPUTimer_CheckTimeout
//          看定时器的定时时间是否已经到了
// 输入参数:timer，待操作的定时器
// 输出参数:timeout, true超时，false不超时
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_CheckTimeout(struct CPUTimerHandle  *timer, bool_t *timeout)
{
    bool_t result=true;
    u8 timerno;
    u32 temp;
    u32 counter;
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno > CN_CPUTIMER_MAX)
        {
            result = false;
        }
        else
        {
            temp = tg_TIMER_Reg[timerno]->COMPARE;
            counter = tg_TIMER_Reg[timerno]->COUNTER;
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
// 函数功能：__CPUTimer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __CPUTimer_GetID(struct CPUTimerHandle   *timer,u32 *timerId)
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
// 函数功能：__CPUTimer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t __CPUTimer_GetCycle(struct CPUTimerHandle   *timer, u32 *cycle)
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
// 函数功能：__CPUTimer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
bool_t __CPUTimer_GetState(struct CPUTimerHandle   *timer, u32 *timerflag)
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
// 函数功能:__CPUTimer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  :true成功  false失败
// 说明    :
// =============================================================================
bool_t __CPUTimer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct CPUTimerHandle  *timer;
    timer = (struct CPUTimerHandle  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = __CPUTimer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = __CPUTimer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = __CPUTimer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = __CPUTimer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = __CPUTimer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = __CPUTimer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = __CPUTimer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = __CPUTimer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = __CPUTimer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = __CPUTimer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = __CPUTimer_GetState(timer, (u32 *)inoutpara);
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
u32  __CPUTimer_GetFreq(ptu32_t timerhandle)
{
    return (CN_CFG_TIMER_CLK/128);
}
// =============================================================================
// 函数功能:module_init_timer
//          P1020的PICtimer初始化,timer时钟源为APB2 = CN_CFG_PCLK1 = 108M(timer234567)
// 输入参数:
// 输出参数:
// 返回值  :
// 说明    :
// =============================================================================
bool_t ModuleInstall_HardTimer(void)
{
    struct TimerChip  CPUtimer;
    u8 i;
    u8 irqline;

    for(i=0;i<CN_CPUTIMER_NUM;i++)
    {
        tg_TIMER_Reg[i]->CONTROL = 0;           //禁止TIMER
        tg_TIMER_Reg[i]->COUNTER = 0;           //计数器清0
        tg_TIMER_Reg[i]->COMPARE = 0xFFFFFFFF;
        tg_TIMER_Reg[i]->CLKSEL  = 7;           //时钟源为80M，128分频
    }

//    //由原先分配定时器的移动到这里
//     irqline = CN_INT_LINE_TIMER;       //对已经分配得到的stgTimerHandle 进行初始化
//    //设置定时器中断,先结束掉该中断所有的关联相关内容
//      Int_Register(irqline);
//      Int_CutLine(irqline);
//
//      Int_IsrDisConnect(irqline);
//      Int_EvttDisConnect(irqline);
//      Int_SettoAsynSignal(irqline);
//
//      Int_SetClearType(irqline,CN_INT_CLEAR_USER);
     // Int_SetIsrPara(irqline,(ptu32_t)timer);//这里设置timer 中断发生是传入timer 作为句柄进行操作,这里不能用了
    //对于多个定时器共用一条中断线这种情况，中断传递的参数永远都只是第一个分配的定时器，这里，原先通过中断
    //传递进来的定时器句柄对定时器做相关的操作不适用了，故该参数不在设置.
    //因为所有定时器公用一个中断线,timer 定时器操作永远为最后一个timerhandle。该参数在调用各个函数会传入
    //  Int_IsrConnect(irqline, __CPUTimer_isr);

    CPUtimer.chipname = "CPUTIMER";
    CPUtimer.HardTimerAlloc = __CPUTimer_Alloc;
    CPUtimer.HardTimerFree = __CPUTimer_Free;
    CPUtimer.HardTimerCtrl = __CPUTimer_Ctrl;
    CPUtimer.HardTimerGetFreq = __CPUTimer_GetFreq;
    HardTimer_RegisterChip(&CPUtimer);

    return true;
}

#endif



