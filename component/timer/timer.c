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
// 模块描述: 软件定时器模块
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 2:36:18 PM/Apr 1, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注:该文件需要提供一个硬件定时器，需要一个不间断走时的64位定时器
#include "board-config.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "os.h"

#include "timer.h"
#include "timer_hard.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Timer(void);
//    ModuleInstall_Timer();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"Software Timers"//软件定时器组件
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"message queue"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_SOFTWARE_TIMERS == false )
//#warning  " Software_Timers  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_SOFTWARE_TIMERS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,2000,
#define CFG_TIMERS_LIMIT        5       //"定时器数量",可创建的定时器数量（不包含图形界面的定时器）
//%$#@enum,true,false,
#define CFG_TIMER_SOUCE_HARD    true    //"硬件定时器提供时钟源",选择专用硬件还是tick做时钟源
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//timer的数据结构
struct Timer
{
    struct Timer  *pre;
    struct Timer  *nxt;
    char          *name;
    u32           cycle;        //定时器周期 (单位是微秒)
    fnTimerRecall isr;          //定时器定时时间节点钩子函数
    u32           stat;         //定时器状态标志，参见 CN_TIMER_ENCOUNT 等常数
    ptu32_t       TimerTag;     //私有标签
    s64           deadline;     //定时器定时时间(单位是微秒)
};

#define CN_TIMER_ALARMNEVER      CN_LIMIT_SINT64             //挂起时限

// 时精度的算法：
// 果时间基准是tick，则取1个tick对应的uS数。
// 果是硬件定时器，取1000个主频周期对应的uS数取整，若主频大于1G，取1uS。
// 硬件定时器时，限制精度是为了限制定时器中断频繁度
// 意，即使使用了硬件定时器，如果systime没有专用的定时器，而是依赖tick的话，
// 依然无法获得所需精度。
static u32 s_u32TimerPrecision;     //主频1000clk对应的uS数，取整
static u32 s_u32Precision2Tclk;     //取整后的s_u32TimerPrecision对应的定时器的周期数
static u32 s_u32TimerFreq;          //所用的硬件定时器频率
//static bool_t  sbUsingHardTimer = false;
static tagTimer* ptTimerHead = NULL;                 //软定时器队列头
static tagTimer* ptTimerTail = NULL;                 //软件定时器队尾
static struct MemCellPool  *ptTimerMemPool = NULL;    //内存池头指针。
static struct MutexLCB     *ptTimerQSync =NULL;       //API资源竞争锁

//使用的硬件定时器（指定使用硬件定时器）
static ptu32_t sgHardTimerDefault = (ptu32_t)NULL;           //用作定时的32位定时器
//使用线程模拟硬件定时器
enum __EN_TIMERSOFT_CMD
{
    EN_TIMERSOFT_ADD = EN_TIMER_LASTOPCODE+1,
    EN_TIMERSOFT_REMOVE,
};

static struct MsgQueue    *ptTimerMsgQ = NULL;
typedef struct
{
    tagTimer*     timer;
    u32               type;
    u32               para;
}tagTimerMsg;
#define CN_TIMERSOFT_MSGLEN  0X10
// =============================================================================
// 函数功能：__Timer_Get64Time
//           获取S64timer的时间
// 输入参数: 无
// 输出参数：time,单位为微秒
// 返回值  ：true 成功  false失败
// 说明    ：内部调用函数,注意该函数是移植敏感函数，S64timer,必须为不停表不被打断的
//           64位定时器，每个系统或者CPU可能用的都不一样
// =============================================================================
bool_t __Timer_Get64Time(s64 *time)
{
    *time = DJY_GetSysTime();
    return true;
}
// =============================================================================
// 函数功能：__Timer_ExeAlarmHandler
//          执行timersoft的钩子函数
// 输入参数：timersoft,待执行的定时器.
// 输出参数：无
// 返回值  ：true 成功  false失败
// 说明    ：内部调用函数，没有HOOK则不执行
// =============================================================================
bool_t __Timer_ExeAlarmHandler(tagTimer *timer)
{
    if(NULL != timer->isr)
    {
        timer->isr(timer);
    }
    return true;
}

// =============================================================================
// 函数功能：__Timer_Remove
//          将指定的定时器从定时器队列中移除
// 输入参数：timer,待移除的定时器
// 输出参数：无
// 返回值  ：调整的定时器
// 说明    ：
// =============================================================================
void __Timer_Remove(tagTimer *timer)
{
    if((timer == ptTimerHead)&&(timer == ptTimerTail)) //only one
    {
        ptTimerHead = NULL;
        ptTimerTail = NULL;
    }
    else if(timer == ptTimerHead)  //head to remove
    {
        ptTimerHead = ptTimerHead->nxt;
        ptTimerHead->pre = NULL;
    }
    else if(timer == ptTimerTail) //tail to remove
    {
        ptTimerTail = ptTimerTail->pre;
        ptTimerTail->nxt = NULL;
    }
    else //normal to remove
    {
        timer->pre->nxt = timer->nxt;
        timer->nxt->pre = timer->pre;
    }
    return;
}
// =============================================================================
// 函数功能：__Timer_Add
//          添加定时器到定时器队列
// 输入参数：timer,待添加的定时器
// 输出参数：无
// 返回值  ：
// 说明    ：
// =============================================================================
void __Timer_Add(tagTimer *timer)
{
    tagTimer *tmp;
    if(NULL == ptTimerHead) //the queue is empty
    {
        ptTimerHead = timer;
        ptTimerTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else        //find proper place to add
    {
        tmp = ptTimerHead;
        while((NULL != tmp)&&(tmp->deadline < timer->deadline))
        {
            tmp = tmp->nxt;
        }
        if(NULL == tmp) //add the timer to the tail
        {
            timer->pre = ptTimerTail;
            timer->nxt = NULL;
            ptTimerTail->nxt = timer;
            ptTimerTail = timer;
        }
        else
        {
            timer->pre = tmp->pre;
            timer->nxt = tmp;
            if(NULL != tmp->pre)
            {
                tmp->pre->nxt = timer;
            }
            else
            {
                ptTimerHead = timer;
            }
            tmp->pre = timer;
        }
    }
    return;
}

// =============================================================================
// 函数功能：__Timer_AddLast
//          添加定时器到定时器队列尾
// 输入参数：timer,待添加的定时器
// 输出参数：无
// 返回值  ：
// 说明    ：
// =============================================================================
void __Timer_AddLast(tagTimer *timer)
{
    if(NULL == ptTimerHead) //the queue is empty
    {
        ptTimerHead = timer;
        ptTimerTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else
    {
        timer->pre = ptTimerTail;
        timer->nxt = NULL;
        ptTimerTail->nxt = timer;
        ptTimerTail = timer;
    }
    return;
}

// =============================================================================
// 函数功能：__Timer_ChkTimeout
// 输入参数：无
// 输出参数：无
// 返回值  ：true，该定时器超时，否则该定时器不超时
// 说明     :
// =============================================================================
bool_t  __Timer_ChkTimeout(tagTimer *timer, s64 timenow)
{
    bool_t result = false;
    if((timer->deadline -s_u32TimerPrecision)<timenow)
    {
        result = true;
    }
    return result;
}
// =============================================================================
// 函数功能：__Timer_DealTimeout
//          处理超时的定时器队列
// 输入参数：
// 输出参数：
// 返回值  ：下次等待的时间
// 说明    ：内部调用函数
// =============================================================================
u32 __Timer_DealTimeout(void)
{
    u32           result;
    s64           timenow;
    tagTimer *timer;

    result = CN_TIMEOUT_FOREVER;
    timer = ptTimerHead;
    while(timer) //执行完所有的TIMER ALARM
    {
        if(timer->stat &CN_TIMER_ENCOUNT)
        {
            timenow = DJY_GetSysTime();     //使用系统64位不停表的定时器，可消除
                                        //定时器中断启停之间产生的积累误差
            if(__Timer_ChkTimeout(timer, timenow))
            {
                __Timer_Remove(timer);
                if(timer->stat & CN_TIMER_RELOAD)
                {
                    timer->deadline = timenow + timer->cycle;
                    __Timer_Add(timer);
                }
                else
                {
                    timer->stat &= (~CN_TIMER_ENCOUNT);
                    timer->deadline = CN_TIMER_ALARMNEVER;
                    __Timer_AddLast(timer);
                }
                __Timer_ExeAlarmHandler(timer); //execute the hook

                timer = ptTimerHead;
            }
            else
            {
                //compute the wait time
                result = (u32)(timer->deadline - timenow);
                timer = NULL;
            }
        }
        else
        {
            timer = NULL; //跳出循环，证明所有的TIMER都处于PAUSE或者没有定时器状态
        }
    }

    return result;
}

u32 __GetTclkCycle(u32 waittime)
{
    u32 result;
    result = waittime *(s_u32TimerFreq / 1E6);
    if(result < s_u32Precision2Tclk)      //中断间隔小于最小间隔
        result = s_u32Precision2Tclk;
    return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// =============================================================================
// 函数功能：Timer_ISR
//          定时器中断服务HOOK
// 输入参数：irq_no,中断号
// 输出参数：无
// 返回值     ：暂时未定义
// =============================================================================
u32 Timer_ISR(ptu32_t irq_no)
{
    u32 waittime;
    //定时器停止计数
    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

    waittime = __Timer_DealTimeout();
    if(waittime != CN_TIMEOUT_FOREVER)
    {
        waittime = __GetTclkCycle(waittime);
        HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
        HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
    }
    else
    {
        //all the timer is paused or no timer, so no need to start the counter
    }

    return 0;
}
#pragma GCC diagnostic pop

// =============================================================================
// 函数功能：Timer_Create_s
//          创建一个定时器(自己提供资源)
// 输入参数：name,定时器名字
//          cycle,定时器周期,单位微秒
//          isr,定时器定时时间到执行HOOk，中断中可能被调用
//          timer,提供的虚拟定时器的资源空间
// 输出参数：无
// 返回值  ： NULL分配失败  否则返回创建成功的定时器句柄
//           创建的定时器默认的reload模式，如果需要手动的话，那么创建之后自己设置；
//           创建的定时器还是处于pause状态，需要手动开启该定时器
// =============================================================================
tagTimer*  Timer_Create_s(tagTimer *timer,const char *name,
                                  u32 cycle, fnTimerRecall isr)
{
    tagTimer*      result = NULL;
    u32                waittime;
    if(NULL == timer)  //参数检查
    {
        result =NULL;
    }
    else
    {
#if CFG_TIMER_SOUCE_HARD == true        //由硬件计时器提供时钟源
//      if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER))
            {
                //暂停闹钟，进行中断互斥
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
                //开始执行指定任务的操作
                //虚拟定时器的初始化
                timer->name = (char*)name;
                timer->cycle = cycle;
                timer->isr = isr;
                timer->deadline = CN_TIMER_ALARMNEVER;
                timer->stat = CN_TIMER_RELOAD;
                //将创建的虚拟定时器插进timer定时器队列中
                __Timer_AddLast(timer);
                waittime = __Timer_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    __GetTclkCycle(waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerQSync);
                result = timer;
            }
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg    msg;
            timer->name = (char*)name;
            timer->cycle = cycle;
            timer->isr = isr;
            timer->deadline = CN_TIMER_ALARMNEVER;
            timer->stat = CN_TIMER_RELOAD;
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_ADD;
            if(MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }
    return result;
}
// =============================================================================
// 函数功能：Timer_Delete_s
//           删除一个定时器(自己提供资源的定时器)
// 输入参数：timersoft,待删除的定时器
// 输出参数：无
// 返回值  ：被删除的定时器，NULL删除不成功
// =============================================================================
tagTimer* Timer_Delete_s(tagTimer* timer)
{
    tagTimer*      result = NULL;
    u32                waittime;

    if(NULL == timer)  //参数检查
    {
        result =NULL;
    }
    else
    {
#if CFG_TIMER_SOUCE_HARD == true        //由硬件计时器提供时钟源
//      if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER))
            {
                //暂停闹钟，进行中断互斥
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

                //将创建的虚拟定时器插进timer定时器队列中
                __Timer_Remove(timer);
                waittime = __Timer_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    __GetTclkCycle(waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerQSync);
                result = (tagTimer*)timer;
            }
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg    msg;
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_REMOVE;
            if(MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }
    return result;
}
// =============================================================================
// 函数功能：Timer_Create
//          创建一个定时器
// 输入参数：name,定时器名字
//          cycle,定时器周期，单位微秒
//          isr,定时器定时时间到执行HOOk，中断中可能被调用
// 输出参数：无
// 返回值  ：NULL分配失败  否则返回分配到的定时器句柄
//           创建的定时器默认的reload模式，如果需要手动的话，那么创建之后自己设置；
//           创建的定时器还是处于pause状态，需要手动开启该定时器
// =============================================================================
tagTimer* Timer_Create(const char *name, u32 cycle,fnTimerRecall isr)
{
    tagTimer *timer;
    tagTimer *result;

    result = NULL;
    timer = Mb_Malloc(ptTimerMemPool,CN_TIMEOUT_FOREVER);
    if(NULL != timer)
    {
        result = Timer_Create_s(timer,name, cycle, isr);
        if(NULL == result)
        {
            Mb_Free(ptTimerMemPool,timer);
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
//功能：查看定时器是否在运行状态
//参数：Timer，待查看的定时器
//返回：true = running，false= stop
//-----------------------------------------------------------------------------
bool_t Timer_IsRunning(tagTimer *Timer)
{
    if(Timer == NULL)
    {
        return false;
    }
    else
    {
        if(Timer->stat & CN_TIMER_ENCOUNT)
            return true;
        else
            return false;
    }
}

// =============================================================================
// 函数功能：Timer_Delete
//           删除一个定时器
// 输入参数：timersoft,待删除的定时器
// 输出参数：无
// 返回值  ：true 成功 false失败
// =============================================================================
bool_t Timer_Delete(tagTimer* timer)
{
    tagTimer* result;

    result = Timer_Delete_s(timer);
    if(result != (tagTimer*)NULL)
    {
        Mb_Free(ptTimerMemPool, (void *)timer);
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// 函数功能：Timer_Ctrl
//          设置定时器标识和状态
// 输入参数：timersoft,待设定的定时器.
//          opcode,定时器的操作码
//          inoutpara,根据操作码解析的参数，比方设定cycle的时候，inoutpara代表cyle
// 输出参数：无
// 返回值  ：true 成功 false失败
// 说明：opcode对应的para的属性定义见enum TimerCmdCode说明
// =============================================================================
bool_t Timer_Ctrl(tagTimer* timer,u32 opcode, u32 para)
{
    bool_t  result = false;
    u32     waittime;
    s64     timenow;               //当前时间

    if(timer)                      //参数检查
    {
#if CFG_TIMER_SOUCE_HARD == true        //由硬件计时器提供时钟源
//      if(sbUsingHardTimer)
        {
            Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER);
            //暂停闹钟，进行中断互斥
            HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
            result = true;
            switch(opcode)
            {
                case EN_TIMER_SOFT_START:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))    //本来未使能
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        timenow = DJY_GetSysTime(); //使用系统64位不停表的定时器，可消除
                                                //定时器中断启停之间产生的积累误差
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_STOP:
                    if(CN_TIMER_ENCOUNT & timer->stat)          //本来在运行态
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __Timer_Remove(timer);
                        __Timer_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        timenow = DJY_GetSysTime(); //使用系统64位不停表的定时器，可消除
                                                //定时器中断启停之间产生的积累误差
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                default:
                    result = false;
                    break;
            }
            //上述操作可能会有定时器超时,要做定时器队列的超时处理
            waittime = __Timer_DealTimeout();
            if(waittime != CN_TIMEOUT_FOREVER)
            {
                __GetTclkCycle(waittime);
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
            }
            Lock_MutexPost(ptTimerQSync);
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg msg;
            msg.timer = timer;
            msg.type = opcode;
            msg.para = para;
            result = MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL);
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }

    return result;
}
// =============================================================================
// 函数功能：Timer_VMTask 线程模仿硬件定时器
// 输入参数：无
// 输出参数：无
// 返回值  ：
// 说明    :当使用线程作为模拟定时器的时候，除了添加和删除以外，所有的对定时器
//          队列的操作都是在该线程中完成
// =============================================================================
ptu32_t  Timer_VMTask(void)
{
    u32              waittime;
    u32              opcode;
    u32              para;
    s64              timenow;
    tagTimerMsg  msg;
    tagTimer     *timer;

    while(1)
    {
        //any way, we scan the queue to chk if any timer is timeout
        waittime = __Timer_DealTimeout();
        if(MsgQ_Receive(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),waittime))
        {
            //has got some msg from the api
            opcode = msg.type;
            timer = msg.timer;
            para = msg.para;
            switch(opcode)
            {
                case EN_TIMER_SOFT_START:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))    //本来未使能
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        timenow = DJY_GetSysTime(); //使用系统64位不停表的定时器，可消除
                                                //错过tick中断产生的积累误差
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_STOP:
                    if(CN_TIMER_ENCOUNT & timer->stat)      //本来在运行态
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __Timer_Remove(timer);
                        __Timer_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        timenow = DJY_GetSysTime(); //使用系统64位不停表的定时器，可消除
                                                //错过tick中断产生的积累误差
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                case EN_TIMERSOFT_ADD:
                    __Timer_Add(timer);
                    break;
                case EN_TIMERSOFT_REMOVE:
                    __Timer_Remove(timer);
                    break;
                default:
                    break;
            }

        }

    }
    return 0;
}

//-----------------------------------------------------------------------------
//功能：设置定时器私有标签
//参数：timer,定时器指针.
//      Tag，新的标签
//返回：定时器的私有标签
//-----------------------------------------------------------------------------
bool_t Timer_SetTag(tagTimer* timer,ptu32_t Tag)
{
    if(timer != NULL)
    {
        timer->TimerTag = Tag;
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------------------------------
//功能：取定时器私有标签
//参数：timer,定时器指针.
//返回：定时器的私有标签
//-----------------------------------------------------------------------------
ptu32_t Timer_GetTag(tagTimer* timer)
{
    if(timer != NULL)
        return timer->TimerTag;
    else
        return 0;
}

//-----------------------------------------------------------------------------
//功能：取定时器名字
//参数：timersoft,定时器指针.
//返回：定时器名字
//-----------------------------------------------------------------------------
char *Timer_GetName(tagTimer* timer)
{
    if(timer != NULL)
        return timer->name;
    else
        return NULL;
}


// =============================================================================
// 函数功能：ModuleInstall_Timer
//          虚拟定时器初始化模块
// 输入参数：para,表示定时器模块使用硬件定时器还是tick做定时基准，取值为
//              CN_TIMER_SOURCE_TICK或CN_TIMER_SOURCE_HARD
// 输出参数：无
// 返回值  ：true = 成功  false =失败
// =============================================================================
bool_t ModuleInstall_Timer(void)
{
    static tagTimer ptTimerMem[CFG_TIMERS_LIMIT];
    printk("Timer:Init Start....\n\r");
    ptTimerMemPool = Mb_CreatePool(ptTimerMem,CFG_TIMERS_LIMIT,
                                sizeof(tagTimer),0,0,"Timer");
    if(NULL ==ptTimerMemPool)
    {
        goto EXIT_POOLFAILED;
    }
#if CFG_TIMER_SOUCE_HARD == true        //由硬件计时器提供时钟源
        //使用硬件定时器的时候才会使用该同步标记
        ptTimerQSync = Lock_MutexCreate("Timer");
        if(NULL == ptTimerQSync)
        {
            goto EXIT_TIMERFAILED;
        }
        sgHardTimerDefault = HardTimer_Alloc(Timer_ISR);
        if((ptu32_t)NULL == sgHardTimerDefault)
        {
            Lock_MutexDelete(ptTimerQSync);
            goto EXIT_TIMERFAILED;
        }
        s_u32TimerPrecision = 1E9/CN_CFG_MCLK;     //计算1000个CPU周期对应的uS数，取整
        if(s_u32TimerPrecision == 0)               //如果主频超过1G
            s_u32TimerPrecision = 1;
        s_u32TimerFreq = s_u32TimerPrecision*CN_CFG_MCLK/1000000;    //取整后s_u32TimerPrecision uS对应的主频周期数
        s_u32Precision2Tclk = HardTimer_GetFreq(sgHardTimerDefault);
        //算对应的定时器周期数，这是timer模块中定时器设计的最小定时间隔
        s_u32Precision2Tclk =s_u32Precision2Tclk*(s_u32TimerFreq/ 1E6);
        //使能定时器中断，但是没有使能定时器,坐等API的调用
        HardTimer_Ctrl(sgHardTimerDefault,EN_TIMER_ENINT,(ptu32_t)NULL);
        HardTimer_Ctrl(sgHardTimerDefault,EN_TIMER_SETRELOAD,(ptu32_t)false);
#else   //CFG_TIMER_SOUCE_HARD == true      由tick提供时钟源
        u16 u16EvttId;
        u16 u16EventId;

        //建立通信用的消息队列
        ptTimerMsgQ = MsgQ_Create(CN_TIMERSOFT_MSGLEN, \
                                      sizeof(tagTimerMsg),CN_MSGQ_TYPE_FIFO);
        if(NULL == ptTimerMsgQ)
        {
            goto EXIT_TIMERFAILED;
        }
        else
        {
            u16EvttId = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS-2,0,0,
                                   Timer_VMTask,NULL,0x1000,NULL);
            if(CN_EVTT_ID_INVALID == u16EvttId)
            {
                MsgQ_Delete(ptTimerMsgQ);
                ptTimerMsgQ = NULL;
                goto EXIT_TIMERFAILED;
            }
            else
            {
                s_u32TimerPrecision = CN_CFG_TICK_US;   //精度=1个tick
                u16EventId = DJY_EventPop(u16EvttId,NULL,0,0,0,0);
                if(CN_EVENT_ID_INVALID == u16EventId)
                {
                    MsgQ_Delete(ptTimerMsgQ);
                    ptTimerMsgQ = NULL;
                    DJY_EvttUnregist(u16EventId);
                    goto EXIT_TIMERFAILED;
                }
            }
        }
#endif  //CFG_TIMER_SOUCE_HARD == true

    printk("Timer:Init Success\n\r");
    return true;

EXIT_TIMERFAILED:
    Mb_DeletePool(ptTimerMemPool);
EXIT_POOLFAILED:
    free(ptTimerMem);
    printk("Timer:Init Failed\n\r");
    return false;
}
