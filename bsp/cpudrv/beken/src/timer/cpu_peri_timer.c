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
// 模块描述: TIMER的驱动
// 模块版本: V1.00
// 创建人员: czz
// 创建时间:
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
#include "board-config.h"
#include "cpu_peri_reg.h"
#include "driver/include/arm_arch.h"
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
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
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

enum ENUM__TIMER
{
    EN_TIMER_0=0,
    EN_TIMER_1,
    EN_TIMER_2,
    EN_TIMER_3,
    EN_TIMER_4,
    EN_TIMER_5,
    EN_TIMER_MAX
};

//各个定时器芯片的定时器应该有自己的句柄
struct TimerCB
{
    u32     timerno;          //定时器号
    u32     cycle;            //定时周期
    u32     timerstate;       //定时器标识
    fnTimerIsr UserIsr;            //用户中断响应函数
};
static struct TimerCB stgTimerHandle[EN_TIMER_MAX];

//最高位代表timer0 依次类推
static u32  gs_dwTimerBitmap;  //对于定时器这种东西，一般的不会很多，32个应该足够
#define CN_TIMER_BITMAP_MSK  (0x80000000)  //最高位为1，依次右移即可


//获取32位数第一个0bit位置(从高位到低位算起)
static u8 Timer_GetFirstZeroBit(u32 para)
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
// 函数功能:Timer_StartCount
//          开始计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
static bool_t Timer_StartCount(struct TimerCB  *timer)
{
    u32 timerno;
    timerno = timer->timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;
    if(timerno > EN_TIMER_MAX)
        return false;

    if(timerno < 3)
    {
        REG32_WRITE_REG(REG_TIMERCTLA_PERIOD_ADDR(timerno),timer->cycle);
        REG32_SET_BIT(TIMER0_2_CTL, (1 << timerno ));
    }
    else
    {
        REG32_WRITE_REG(REG_TIMERCTLB_PERIOD_ADDR(timerno),timer->cycle);
        REG32_SET_BIT(TIMER3_5_CTL, (1 << (timerno - 3)));
    }
    //似乎是timer的bug，+计数模式时，得经历一次CNT计满回绕到0以后，reload功能
    //才有效，故初值设为 0xffffffff
    timer->timerstate = (timer->timerstate)| (CN_TIMER_ENCOUNT);

    return true;
}

// =============================================================================
// 函数功能:Timer_SetCycle,的timer,最大为0xFFFFFFFF,根据Timer_ModuleInit
//          设定计数器每计一次数为(1/108)1us，可知最大时间不超-1
//          设定周期
// 输入参数:timer，PIC定时器
//          cycle,周期（定时器时钟个数），
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :如果设置周期太大（超过最大定时器能力），则设置为定时器的最大周期
// =============================================================================
static bool_t  Timer_SetCycle(struct TimerCB  *timer, u32 cycle)
{
    u8 timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;
    timerno = timer->timerno;
    if(timerno >= EN_TIMER_MAX)
        return false;
    timer->cycle = cycle;
    return true;
}
// =============================================================================
// 函数功能:Timer_SetAutoReload
//          设定周期
// 输入参数:timer，PIC定时器
//         autoreload, true则自己reload周期，否则手动每次设置周期
// 输出参数:
// 返回值  :true成功 fasle失败
// =============================================================================
static bool_t  Timer_SetAutoReload(struct TimerCB  *timer, bool_t autoreload)
{
    //消除告警
    (void) timer;
    (void) autoreload;

    return false;
}

//-----------------------------------------------------------------------------
// 功能：定时器中断响应函数，对于像atmel m7这样，需要自行操作定时器的寄存器才能
//       清中断的定时器，必须在这里实现ISR，然后间接调用user ISR，否则在
//       user isr中将无所适从。
// 参数：定时器句柄。
// 返回：user ISR的返回值
//-----------------------------------------------------------------------------
static  u32 Header_TimerIsr(ptu32_t TimerHandle)
{

    int i;
    UINT32 status;
    (void) TimerHandle;

    status = REG_READ(TIMER0_2_CTL);
    for(i = 0; i < 3; i++)
    {
        if(status & (1 << (i + TIMERCTLA_INT_POSI)))
        {
            if(stgTimerHandle[i].UserIsr!=NULL)
            {
                stgTimerHandle[i].UserIsr((ptu32_t)&stgTimerHandle[i]);
            }
        }
    }
    do
    {
        REG_WRITE(TIMER0_2_CTL, status);
    }
    while(REG_READ(TIMER0_2_CTL) & status & (0x7 << TIMERCTLA_INT_POSI));

    status = REG_READ(TIMER3_5_CTL);
    for(i = 0; i < 3; i++)
    {
        if(status & (1 << (i + TIMERCTLB_INT_POSI)))
        {

            if(stgTimerHandle[i+3].UserIsr!=NULL)
            {
                stgTimerHandle[i+3].UserIsr((ptu32_t)&stgTimerHandle[i+3]);
            }
        }
    }
    do
    {
        REG_WRITE(TIMER3_5_CTL, status);
    }
    while(REG_READ(TIMER3_5_CTL) & status & (0x7 << TIMERCTLB_INT_POSI));

    Int_ClearLine(CN_INT_LINE_TIMER);
    return 0;
}

// =============================================================================
// 函数功能:__Timer_Free
//          释放定时器
// 输入参数:timer，分配的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
static bool_t  __Timer_Free(ptu32_t timerhandle)
{
    u32 timerno;
    atom_low_t  timeratom;  //保护公用资源
    struct TimerCB  *timer;
    timer = (struct TimerCB  *)timerhandle;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timerno = timer->timerno;
        if(timerno < EN_TIMER_MAX)//还有空闲的，则设置标志位
        {       //修改全局标志一定是原子性的
            timeratom = Int_LowAtomStart();
            gs_dwTimerBitmap = gs_dwTimerBitmap &(~(CN_TIMER_BITMAP_MSK>> timerno));
            //解除掉中断所关联的内容
            timer->timerstate = 0;

            if(timerno < 3)
            {
                REG32_CLEAR_BIT(TIMER0_2_CTL,(1 << timerno ));
            }
            else
            {
                REG32_CLEAR_BIT(TIMER3_5_CTL,(timerno - 3));
            }
            Int_LowAtomEnd(timeratom);  //原子操作完毕
            return true;
        }
    }
    return false;
}


// =============================================================================
// 函数功能:Timer_SetIntPro
//          设置定时器的中断类型
// 输入参数:timer，待操作的定时器
//          real_prior为true则为实时信号否则为异步信号
// 输出参数:
// 返回值  :分配的定时器，NULL则分配不成功
// 说明    :
// =============================================================================
static bool_t  Timer_SetIntPro(struct TimerCB  *timer, bool_t real_prior)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(real_prior)
        {
            return false;
//            timer->timerstate = (timer->timerstate)| (CN_TIMER_REALINT);
//            Int_EvttDisConnect(CN_INT_LINE_TIMER);
//            return Int_SettoReal(CN_INT_LINE_TIMER);
        }
        else
        {
            timer->timerstate = (timer->timerstate)&(~CN_TIMER_REALINT);
            return Int_SettoAsynSignal(CN_INT_LINE_TIMER);
        }
    }
    else
    {
        return false;
    }
}

// =============================================================================
// 函数功能:Timer_EnInt
//          使能定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
static bool_t  Timer_EnInt(struct TimerCB  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)| (CN_TIMER_ENINT);
        return Int_EnableLine(CN_INT_LINE_TIMER);
    }
    return false;
}
// =============================================================================
// 函数功能:Timer_DisInt
//          禁止定时器中断
// 输入参数:timer，待操作的定时器
// 输出参数:
// 返回值  :true成功false失败
// 说明    :
// =============================================================================
static bool_t  Timer_DisInt(struct TimerCB  *timer)
{
    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENINT);
        return true;
//                Int_DisableLine(timer->irqline);
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能:Timer_GetTime
//          获取定时器走时
// 输入参数:timer，待操作的定时器
// 输出参数:time，走时（微秒）
// 返回值  :true成功false失败
// 说明    :从设定的周期算起，即cycle-剩余时间,表示已经走掉的时间(单位：定时器主频时钟个数)
// =============================================================================
static bool_t Timer_GetTime(struct TimerCB  *timer, u32 *time)
{
    u32 timerno = timer->timerno;
    if(timerno >= EN_TIMER_MAX)
        return false;

    if(timer->timerstate & CN_TIMER_ENUSE)
    {
        if(timerno<3)
        {
            *time = REG32_READ_REG(REG_TIMERCTLA_PERIOD_ADDR(timerno));
        }
        else
        {
            *time = REG32_READ_REG(REG_TIMERCTLB_PERIOD_ADDR(timerno));
        }
        return true;
    }
    return false;
}

// =============================================================================
// 函数功能:取定时器硬件控制寄存器地址
// 输入参数:timer，待操作的定时器
// 返回值  :true成功 false失败
// 说明    :
// =============================================================================
static bool_t Timer_GetReg(struct TimerCB  *timer, void **reg)
{
    (void) timer;
    (void) reg;
    return false;
}
// =============================================================================
// 函数功能：Timer_GetID
//          获取定时器ID
// 输入参数：timer，待操作定时器，
// 输出参数：timerId,高16位为TIMERNO,低16为对应的IRQNO
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
static bool_t Timer_GetID(struct TimerCB   *timer,u32 *timerId)
{
    (void) timer;
    (void)timerId;
    return false;
}

// =============================================================================
// 函数功能：Timer_GetCycle
//          获取定时器周期
// 输入参数：timer，待操作定时器，
// 输出参数：cycle，定时器周期(单位：定时器主频时钟个数)
// 返回值  ：true 成功 false失败
// =============================================================================
static bool_t Timer_GetCycle(struct TimerCB   *timer, u32 *cycle)
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
// 函数功能：Timer_GetState
//          获取定时器状态
// 输入参数：timer,待操作的定时器
// 输出参数：timerflag，定时器状态
// 返回值  ：true 成功 false失败
// 说明    : 本层实现
// =============================================================================
static bool_t Timer_GetState(struct TimerCB   *timer, u32 *timerflag)
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
// 函数功能:Timer_PauseCount
//          停止计数
// 输入参数:timer，PIC定时器
// 输出参数:
// 返回值  :true成功 fasle失败
// 说明    :
// =============================================================================
static bool_t Timer_PauseCount(struct TimerCB  *timer)
{
    u32 timerno;
    if(!(timer->timerstate & CN_TIMER_ENUSE))
        return false;

    timerno = timer->timerno;
    if(timerno > EN_TIMER_MAX)
    {
        return false;
    }

    if(timerno < 3)
    {
        REG32_CLEAR_BIT(TIMER0_2_CTL,(1<<timerno));
    }
    else
    {
        REG32_CLEAR_BIT(TIMER0_2_CTL,(1<<(timerno-3)));
    }
    timer->timerstate = (timer->timerstate)&(~CN_TIMER_ENCOUNT);
    return true;

}
// =============================================================================
// 函数功能:__Timer_Ctrl
//          看定时器的定时时间是否已经到了
// 输入参数:timerhandle 待操作的定时器句柄
//         ctrlcmd, 操作命令码
// 输出参数:inoutpara,根据ctrlcmd的不同而不同
// 返回值  :true成功  false失败
// 说明    :
// =============================================================================
static bool_t __Timer_Ctrl(ptu32_t timerhandle, \
                         enum HardTimerCmdCode ctrlcmd, \
                         ptu32_t inoutpara)
{
    bool_t result = false;
    struct TimerCB  *timer;
    timer = (struct TimerCB  *)timerhandle;
    if(NULL != timer)
    {
        switch(ctrlcmd)
        {
            case EN_TIMER_STARTCOUNT:
                result = Timer_StartCount(timer);
                break;
            case EN_TIMER_PAUSECOUNT:
                result = Timer_PauseCount(timer);
                break;
            case EN_TIMER_SETCYCLE:
                result = Timer_SetCycle(timer,(u32)inoutpara);
                break;
            case EN_TIMER_SETRELOAD:
                result = Timer_SetAutoReload(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_ENINT:
                result = Timer_EnInt(timer);
                break;
            case EN_TIMER_DISINT:
                result = Timer_DisInt(timer);
                break;
            case EN_TIMER_SETINTPRO:
                result = Timer_SetIntPro(timer, (bool_t)inoutpara);
                break;
            case EN_TIMER_GETTIME:
                result = Timer_GetTime(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETCYCLE:
                result = Timer_GetCycle(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETID:
                result = Timer_GetID(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETSTATE:
                result = Timer_GetState(timer, (u32 *)inoutpara);
                break;
            case EN_TIMER_GETREG:
                result = Timer_GetReg(timer, (void **)inoutpara);
                break;
            default:
                break;
        };
    }

    return result;
}
// =============================================================================
// 函数功能:P1020Pic__Timer_GetFreq
//       获取定时器主频
// 输入参数:timerhandle 待操作的定时器句柄
// 输出参数:
// 返回值  :定时器主频
// 说明    :单位（HZ）
// =============================================================================
static u32  __Timer_GetFreq(ptu32_t timerhandle)
{

    struct TimerCB  *timer = (struct TimerCB*)timerhandle;
    if(timer->timerno<3)
    {
        return 26*1000*1000;

    }
    if(timer->timerno < 6)
    {
        return 32*1000;
    }
    return 0;
}



// =============================================================================
// 函数功能:__Timer_Alloc
//          分配定时器
// 输入参数:timerisr,定时器的中断处理函数
// 输出参数:
// 返回值  :分配的定时器句柄，NULL则分配不成功
// 说明    :
// =============================================================================
static ptu32_t __Timer_Alloc(fnTimerIsr timerisr)
{
    u32 timerno;
    struct TimerCB  *timer;
    //原子操作，防止资源竞争
    atom_low_t  timeratom;
    timeratom = Int_LowAtomStart();

    //寻找空闲的timer
    timerno = Timer_GetFirstZeroBit(gs_dwTimerBitmap);
    if(timerno < EN_TIMER_MAX)//还有空闲的，则设置标志位
    {
        gs_dwTimerBitmap = gs_dwTimerBitmap | (CN_TIMER_BITMAP_MSK<< timerno);
        Int_LowAtomEnd(timeratom);  //原子操作完毕
    }
    else//没有的话直接返回就可以了，用不着再啰嗦了
    {
        Int_LowAtomEnd(timeratom);   //原子操作完毕
        return 0;
    }

    timer = &stgTimerHandle[timerno];
    timer->cycle = 0;
    timer->timerno = timerno;
    timer->timerstate = CN_TIMER_ENUSE;
    timer->UserIsr=timerisr;
    Timer_PauseCount(timer);

    return (ptu32_t)timer;
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
    struct TimerChip  timer;

    for(u8 i=0;i<sizeof(stgTimerHandle)/sizeof(struct TimerCB);i++)
    {
        stgTimerHandle[i].UserIsr    = NULL;
        stgTimerHandle[i].timerstate = 0;
        stgTimerHandle[i].cycle      = 0;
        stgTimerHandle[i].timerno    = i;
    }

    //设置不分频 时钟：timer0~2 26M   time3~5 32K
    REG32_CLEAR_BIT(TIMER0_2_CTL,TIMERCTLA_CLKDIV_MASK << TIMERCTLA_CLKDIV_POSI);
    REG32_CLEAR_BIT(TIMER3_5_CTL,TIMERCTLA_CLKDIV_MASK << TIMERCTLA_CLKDIV_POSI);

    //value = (PWD_TIMER_26M_CLK);
    REG32_CLEAR_BIT(ICU_PERI_CLK_PWD,(3<<20));//时钟使能

    timer.chipname = "TIMER";
    timer.HardTimerAlloc = __Timer_Alloc;
    timer.HardTimerFree = __Timer_Free;
    timer.HardTimerCtrl = __Timer_Ctrl;
    timer.HardTimerGetFreq = __Timer_GetFreq;
    HardTimer_RegisterChip(&timer);

    Int_EvttDisConnect(CN_INT_LINE_TIMER);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER);
    Int_SetClearType(CN_INT_LINE_TIMER,CN_INT_CLEAR_USER);
    Int_IsrConnect(CN_INT_LINE_TIMER, Header_TimerIsr);
    return true;
}





