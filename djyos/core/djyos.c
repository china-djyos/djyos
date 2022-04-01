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
//所属模块:核心模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述:事件类型、事件管理以及多事件调度相关的代码全在这里了。
//其他说明:
//修订历史:
//8. 日期:2013-2-7
//   作者:  罗侍田.
//   新版本号：1.3.3
//   修改说明: 修正了djy_event_exit函数的一处错误，该bug由裴乐提交
//7. 日期:2013-1-18
//   作者:  罗侍田.
//   新版本号：1.3.2
//   修改说明: 修正了djy_event_pop函数的一处错误，该bug由QQ群友
//              "ring wang(23424710)"提交
//6. 日期:2012-12-18
//   作者:  罗侍田.
//   新版本号：1.3.1
//   修改说明: 修正了djy_event_complete和djy_wait_event_completed函数的一处错误，
//      该bug由网友"王毛磊"提交
//5. 日期: 2011-07-19
//   作者:  罗侍田.
//   新版本号: V1.3.0
//   修改说明:解决一个可能导致内存泄漏的bug。该bug发作条件：参数尺寸大于32字节，
//   且在调用djy_event_pop时要求动态分配参数所需内存空间，djy_task_completed和
//   djy_event_exit函数释放参数时将不释放该动态分配的内存。
//   修改方法是在struct  para_record结构中增加成员dynamic_mem，用于表示参数是否
//   动态分配得到的，如是，则在djy_task_completed和djy_event_exit函数释放参数时
//   调用m_free释放之。
//4. 日期: 2009-12-15
//   作者:  罗侍田.
//   新版本号: V1.3.0
//   修改说明:
//   1、djy_evtt_regist中判断同名事件类型时，无论该事件类型控制块有效否，都调用
//      strcmp函数比较，效率较低，改为先判断是否空，然后才比较。
//   2、原来在djy_evtt_regist中因不能创建线程导致登记失败时，没有处理事件类型
//      控制块，导致仍然处于registered=1的状态。
//3. 日期: 2009-10-05
//   作者:  罗侍田.
//   新版本号: V1.2.0
//   修改说明:
//   1、不再以tg_int_global.nest_asyn_signal==0作为是否允许调度的依据，这会导致
//      中断模块和调度模块间隐含的耦合关系。
//      以全局变量bg_schedule_enable作为是否允许调度的标志
//   2、改掉了一些gcc（以及c99）才支持的特征，以适应更广泛的编译器
//   3、前面的版本并无实现djy_evtt_unregist的全部功能，本版本补全之。
//   4、__djy_resume_delay函数中这一行：
//      g_ptEventDelay = g_ptEventDelay->previous;改为
//      g_ptEventDelay = g_ptEventDelay->next;
//      否则会导致闹钟同步队列错乱
//   5、原版本中,__turnto_context函数身兼2职，内核初始化完成后，首次调用本函数还
//      有启动线程调度的功能，这在arm中是可以的，但不确保其他cpu也可以，例如cm3
//      上就不行，先把这个功能分离出来，增加一个函数__asm_start_thread。但在arm
//      版本中用宏定义：#define __asm_start_thread __asm_turnto_context
//   6、int_restore_asyn_signal函数中，先执行__int_contact_asyn_signal打开中断，
//      然后再执行：
//            if(g_ptEventRunning != g_ptEventReady)
//                __DJY_Schedule();
//      在打开中断与__djy_schedule函数重新关闭中断之间，如果有中断抢占的话，可能
//      导致严重错误，int_restore_trunk函数也有同样的问题。
//      新版本中，__djy_schedule函数调用__asm_switch_context时才打开中断，杜绝了
//      上述问题，__djy_schedule函数整个都是在关闭中断的条件下执行的。
//2. 日期: 2009-03-18
//   作者:  罗侍田.
//   新版本号: V1.1.0
//   修改说明:
//   1、根据无为小僧（曾波）发现的隐患，事件类型的名字如果用指针索引的话，如果
//      用户调用y_evtt_regist时使用的是局部数组，将存在保存名字的内存被释放的危
//      险。改为数组存储，最长31字符，在struct event_type结构中把evtt_name由指针
//      改为数组，修改涉及下列函数：
//          __djy_init_sys、y_evtt_regist、y_evtt_unregist、y_get_evtt_id
//   2、在y_evtt_unregist函数中释放线程处发现一个bug，存在访问非法内存的危险，
//      改之。
//   3、y_evtt_done_sync函数中处理同步队列处存在严重bug，可能导致访问非法内存，
//      且使同步队列混乱，改之。
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include "align.h"
#include "object.h"
#include "lock.h"
#include "int.h"
#include "pool.h"
#include "systime.h"
#include "djyos.h"
#include "blackbox.h"
#include "dbug.h"
#include "heap.h"

#include "lowpower.h"
#include "component_config_core.h"
#include "../heap/component_config_heap.h"
#include <stdlib.h>
ptu32_t __DJY_Service(void);
s64 __DJY_GetSysTick(void);

struct ProcessVm *  g_ptMyProcess;
//为cn_events_limit条事件控制块分配内存
struct EventECB g_tECB_Table[CFG_EVENT_LIMIT];
//static struct ParaPCB  s_tEventParaTable[CN_CFG_PARAS_LIMIT];
//为cn_evtts_limit个事件类型控制块分配内存
struct EventType g_tEvttTable[CFG_EVENT_TYPE_LIMIT];
struct EventECB  *s_ptEventFree; //空闲链表头,不排序
//static struct ParaPCB  *s_ptParaFree; //空闲链表头,不排序
//轮转调度时间片，0表示禁止轮转调度，默认1，RRS = "round robin scheduling"缩写。
static u32 s_u32RRS_Slice = 1;
static u32 s_u32StackCheckLevel = 10;      //栈报警水平，百分数
struct EventECB  *g_ptEventReady;      //就绪队列头
struct EventECB  *g_ptEventRunning;    //当前正在执行的事件
struct EventECB  *g_ptEventDelay;      //闹钟同步队列表头
//typedef struct
//{
//    u64 DelayTick;
//    u64 RRSTicks;
//}tagSchduleTick;
//static tagSchduleTick gSchduleTick = {
//    .DelayTick = CN_LIMIT_UINT64,
//    .RRSTicks = CN_LIMIT_UINT64,
//};
s64 g_s64RunningStartTime;              //当前运行中事件的开始执行时间.
s64  g_s64OsTicks = 0;                  //操作系统运行ticks，由tick中断增量，表示从
                                        //系统启动到最后一次响应tick中断的ticks数
bool_t g_bScheduleEnable;               //系统当前运行状态是否允许调
bool_t g_bMultiEventStarted = false;    //多事件(线程)调度是否已经开始
u32 g_u32OsRunMode;     //运行模式，参看 CN_RUNMODE_SI 系列定义

void __DJY_SelectEventToRun(void);
void __DJY_EventReady(struct EventECB *event_ready);
void __DJY_ResumeDelay(struct EventECB *delay_event);
extern void __DjyInitTick(void);
extern void __Heap_CleanUp(uint16_t event_id);
extern void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                               struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_turnto_context(struct ThreadVm  *new_vm);
extern void __asm_start_thread(struct ThreadVm  *new_vm);
extern void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __Int_ResetAsynSignal(void);
extern void __asm_delay_cycle(u64 timeCoreClock);
void __DJY_ChangeBlockQueue(struct EventECB *Event);

//----微秒级延时-------------------------------------------------------------
//功能：利用循环实现的微秒分辨率延时，__asm_delay_cycle函数由汇编实现，参数是时钟周期数
//参数：time，延时时间，单位为微秒
//返回：无
//注意：不建议使用此函数做太长延时，长延时请使用函数 DJY_EventDelay,
//-----------------------------------------------------------------------------
void DJY_DelayUs(u32 time)
{
//  if(time > 0)
    __asm_delay_cycle((u64)(time*((CN_CFG_MCLK)/1000000)));
}

//----纳秒级延时-------------------------------------------------------------
//功能：利用循环实现的纳秒分辨率延时，__asm_delay_cycle函数由汇编实现，参数是时钟周期数
//参数：time，延时时间，单位为纳秒
//返回：无
//注意：不建议使用此函数做太长延时，长延时请使用函数 DJY_EventDelay,
//-----------------------------------------------------------------------------
void DJY_DelayNano(u32 time)
{
//  if(time > 0)
    __asm_delay_cycle((u64)(time*((CN_CFG_MCLK)/1000000)/1000));
}

//----设置栈检查水平-----------------------------------------------------------
//功能：设定栈检查功能检查的栈比例，设置范围是0%~50%，创建新线程时，栈将被填充为
//      固定数据，CPU空闲时，将扫描所有线程的栈空间（启用了低功耗特性除外），一旦
//      发现栈底附近 Level%范围内的内存被修改，将报警并保存异常。同一个线程，只
//      报一次。
//参数：Level，扫描范围，0~50
//返回： 无
//-----------------------------------------------------------------------------
void DJY_SetStackCheckLevel(u32 Level)
{
    if(Level >50)
        s_u32StackCheckLevel = 50;
    else
        s_u32StackCheckLevel = Level;
}


//----线程栈检查---------------------------------------------------------------
//功能: 检查一个事件的线程是否有栈溢出风险，方法:检测栈最低1/16空间内，内容是否
//      改变，若改变，则可能发生了栈溢出。
//参数: event_id,被检查的事件id
//返回: true=no overflow,false = overflow
//-----------------------------------------------------------------------------
bool_t __DJY_CheckStack(s16 event_id)
{
    struct BlackBoxThrowPara  parahead;
    struct ThreadVm *Vm;
    char StackExp[128];
    u32 loop;
    volatile ptu32_t level;
    u32 *stack;
    u32 pads;
    bool_t result=true;
    char *name;

    pads = 0x64646464;
    if(g_tECB_Table[event_id].vm == NULL)
        return true;
    if(g_tECB_Table[event_id].previous ==
                    (struct EventECB*)&s_ptEventFree)
        return true;

    Vm = g_tECB_Table[event_id].vm;
    level = Vm->stack_size * s_u32StackCheckLevel / 100;

    stack = (u32*)(&(Vm[1]));
    for(loop = 0; loop < (Vm->stack_size>>2)-1; loop++)
    {
        if(stack[loop] != pads)
        {
            result = false;   //安全区内发生了改变，有溢出风险
            break;
        }
    }
    if((loop << 2) <= level)        //超越警戒线，
    {
        if(&stack[loop] < Vm->stack_used)   //突破警戒线到新高度
        {
            Vm->stack_used = &stack[loop];  //记录新的已用指针
            parahead.DecoderName = NULL;
            parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RESTART;
#if CFG_OS_TINY == false
            name = g_tEvttTable[g_tECB_Table[event_id].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            name = "unknown";
#endif  //CFG_OS_TINY == false
            sprintf(StackExp,"栈溢出风险,栈底:0x%x,栈尺寸:0x%x,剩余:0x%x,事件号:%d,类型号%d,类型名:%s",
                                (ptu32_t)stack,Vm->stack_size,loop<<2,
                                event_id, g_tECB_Table[event_id].evtt_id&(~CN_EVTT_ID_MASK),
                                name);
            parahead.BlackBoxInfo = (u8*)StackExp;
            parahead.BlackBoxInfoLen = sizeof(StackExp);
            parahead.BlackBoxType = CN_BLACKBOX_TYPE_STACK_OVER;
            printk("%s\n\r",StackExp);
            BlackBox_ThrowExp(&parahead);
        }
    }
    else
    {
        Vm->stack_used = &stack[loop];  //记录新的已用指针，不需要判断是否有更新
    }
    return result;
}

//以下为多任务管理函数

//----tick中断-----------------------------------------------------------------
//功能：为操作系统产生时钟源，并比较闹钟同步队列中的事件到期时间，如果有闹铃时间到的
//      事件，就把他们激活到事件就绪队列中。如果时间片轮转调度被允许，还要看是否要
//      执行轮转。ticks被设置为异步信号。
//参数：inc_ticks，ticks中断的中断线号，实际上不用。
//返回：无
//-----------------------------------------------------------------------------
void __DJY_MaintainSysTime(void);
void  DJY_ScheduleIsr(u32 inc_ticks)
{
    struct EventECB *pl_ecb,*pl_ecbp,*pl_ecbn;
    g_s64OsTicks += inc_ticks;
//  now_tick = __DJY_GetSysTick();
    //用于维护系统时钟运转，使读系统时间的间隔，小于硬件定时器循环周期。
    __DJY_MaintainSysTime( );
    if(g_ptEventDelay != NULL)
    {
        pl_ecb = g_ptEventDelay;
        while(1)
        {
            if(pl_ecb->delay_end_tick <= g_s64OsTicks) //默认64位ticks不会溢出
            {
                //事件在某同步队列中，应该从该队列取出
                if(pl_ecb->sync_head != NULL)
                {
                    if(*(pl_ecb->sync_head) == pl_ecb)//本事件位于队列头部
                    {
                        if(pl_ecb->multi_next == pl_ecb)//该队列中只有一个事件
                        {
                            *(pl_ecb->sync_head) = NULL;

                            pl_ecb->multi_next = NULL;
                            pl_ecb->multi_previous = NULL;
                        }else                   //该同步队列中有多个事件
                        {
                            //头指针指向下一个事件
                            *pl_ecb->sync_head = pl_ecb->multi_next;
                            pl_ecb->multi_previous->multi_next
                                                = pl_ecb->multi_next;
                            pl_ecb->multi_next->multi_previous
                                                = pl_ecb->multi_previous;
                        }
                    }else           //本事件不是队列头
                    {
                        pl_ecb->multi_previous->multi_next
                                            = pl_ecb->multi_next;
                        pl_ecb->multi_next->multi_previous
                                            = pl_ecb->multi_previous;
                    }
                    pl_ecb->sync_head = NULL;   //事件头指针置空
                }

                pl_ecb->wakeup_from = pl_ecb->event_status;
                pl_ecb->event_status = CN_STS_EVENT_READY;
                if(pl_ecb->next == pl_ecb)      //这是闹钟同步队列最后一个结点.
                {
                    g_ptEventDelay = NULL;
                    __DJY_EventReady(pl_ecb);
//                  gSchduleTick.DelayTick = CN_LIMIT_UINT64;
                    break;
                }else
                {
                    g_ptEventDelay = pl_ecb->next;
                    pl_ecb->next->previous = pl_ecb->previous;
                    pl_ecb->previous->next = pl_ecb->next;
                    __DJY_EventReady(pl_ecb);
                    pl_ecb = g_ptEventDelay;
//                  gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
                }
            }else
                break;
        }
    }

    //下面处理时间片轮转调度.
    //因在开异步信号(允许调度)才可能进入__djy_isr_tick，即使因闹钟响导致新事件加
    //入，pg_event_running也必定在优先级单调队列中，但可能不等于pg_event_ready
    if(s_u32RRS_Slice != 0)      //允许轮转调度
    {
        if( (g_ptEventRunning->prio == g_ptEventRunning->next->prio)
                    &&(g_ptEventRunning != g_ptEventRunning->next) )
        {//该优先级有多个事件，看轮转时间是否到
            if((u32)g_s64OsTicks % s_u32RRS_Slice == 0) //时间片用完
            {
                //先处理优先级单调队列，把pg_event_running从队列中取出，代之以
                //g_ptEventRunning->next。
                pl_ecb = g_ptEventRunning->next;
                pl_ecbn = g_ptEventRunning->multi_next;
                pl_ecbp = g_ptEventRunning->multi_previous;
                pl_ecbp->multi_next = pl_ecb;
                pl_ecb->multi_previous = pl_ecbp;
                pl_ecbn->multi_previous = pl_ecb;
                pl_ecb->multi_next = pl_ecbn;
                g_ptEventRunning->multi_next = NULL;
                g_ptEventRunning->multi_previous = NULL;

                //再处理就绪链表,把pg_event_running从就绪队列中拿出来，然后把它
                //放到同优先级的最后。

                //如果没有高优先级事件因同步时间到而加入ready队列，条件将成立
                if(g_ptEventReady == g_ptEventRunning)
                    g_ptEventReady = pl_ecb;
//                pl_ecbn = g_ptEventReady->multi_next;
//                pl_ecbp = g_ptEventRunning->previous;
                //这两行把pg_event_running从队列中取出
                g_ptEventRunning->previous->next = pl_ecb;
                pl_ecb->previous = g_ptEventRunning->previous;
                //这两行把pg_event_running插入目标位置
                g_ptEventRunning->previous = pl_ecbn->previous;
                g_ptEventRunning->next = pl_ecbn;
                pl_ecbn->previous->next = g_ptEventRunning;
                pl_ecbn->previous = g_ptEventRunning;
//              gSchduleTick.RRSTicks = CN_LIMIT_UINT64;
            }
        }
    }
    return;
}

void DJY_SaveLastError(u32 ErrorCode)
{
    g_ptEventRunning->error_no = ErrorCode;
}

//----提取最后一条错误信息-----------------------------------------------------
//功能：如名
//参数：无
//返回：错误号
//还需添加文本串和时间的功能，时间使用ticks数---db
//-----------------------------------------------------------------------------
u32 DJY_GetLastError(void)
{
    return g_ptEventRunning->error_no;
}

u32 *DJY_CurrentErrnoAddr(void)
{
    static u32 err;
    if(NULL == g_ptEventRunning)
    {
        return &err;
    }
    else
    {
        return &g_ptEventRunning->error_no;
    }
}

/**
 * 修改成DJY_CurrentErrnoAddr函数后和C库有冲突，copy了一份，改了C库之后删掉
 */
u32 *DjyCurrentErrnoAddr(void)
{
    static u32 err;
    if(NULL == g_ptEventRunning)
    {
        return &err;
    }
    else
    {
        return &g_ptEventRunning->error_no;
    }
}


//----从ready队列中取出一个事件------------------------------------------------
//功能: 把一个事件从ready队列中取出，并使ready队列重新连接
//参数: event，待取出的事件指针
//返回: 无
//注: 调用者请保证在异步信号(调度)被禁止的情况下调用本函数
//-----------------------------------------------------------------------------
void __DJY_CutReadyEvent(struct EventECB *event)
{
    struct EventECB *pl_ecb;
    if(event != g_ptEventReady)         //event不是ready队列头
    {
        if(event->multi_next == NULL)   //不在优先级单调队列中
        {
            event->next->previous = event->previous;
            event->previous->next = event->next;
        }else                           //在优先级单调队列中
        {
            pl_ecb = event->next;
            event->next->previous = event->previous;
            event->previous->next = event->next;
            if(pl_ecb->prio == event->prio)     //相应优先级不止一个事件
            {
                event->multi_previous->multi_next = pl_ecb;
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_next->multi_previous = pl_ecb;
                pl_ecb->multi_next = event->multi_next;
            }else                               //相应优先级只有一个事件
            {
                //pl_ecb即event->multi_next.
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pl_ecb;
            }
        }
    }else                               //event是ready队列头
    {
        g_ptEventReady = event->next;
        pl_ecb = event->next;
        event->next->previous = event->previous;
        event->previous->next = event->next;
        if(pl_ecb->prio == event->prio)     //相应优先级不止一个事件
        {
            event->multi_previous->multi_next = pl_ecb;
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_next->multi_previous = pl_ecb;
            pl_ecb->multi_next = event->multi_next;
        }else                               //相应优先级只有一个事件
        {
            //pl_ecb即event->multi_next.
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pl_ecb;
        }
    }
}

//----设置轮转调度时间片-------------------------------------------------------
//功能: djyos总所有参与轮转调度的事件时间片都是相同的，在这里设置。如果设为0，
//      表示不允许时间片轮转。
//参数: slices，新的轮转调度时间片，微秒数，将被向上取整为整数个ticks时间
//返回: 无
//-----------------------------------------------------------------------------
void DJY_SetRRS_Slice(u32 slices)
{
#if (64 > CN_CPU_BITS)
    atom_low_t atom_low;
    //若处理器字长＜32位,需要多个周期才能更新u32g_RRS_slice,该过程不能被时钟中断打断.
    atom_low = Int_LowAtomStart( );   //本函数对不能嵌套调用
#endif
    s_u32RRS_Slice =(slices + CN_CFG_TICK_US -1)/CN_CFG_TICK_US;

#if (64 > CN_CPU_BITS)
    //若处理器字长＜32位,需要多个周期才能更新u32g_RRS_slice,该过程不能被时钟中断打断.
    Int_LowAtomEnd( atom_low );
#endif
}

//----查询轮转调度时间片-------------------------------------------------------
//功能: djyos总所有参与轮转调度的事件时间片都是相同的，可用本函数查询。
//参数: 无
//返回: 当前时间片长度，微秒数。
//-----------------------------------------------------------------------------
u32 DJY_GetRRS_Slice(void)
{
    u32 temp;
#if (64 > CN_CPU_BITS)
    atom_low_t atom_low;
    //处理器字长＜32位,需要多个周期才能读取u32g_RRS_slice,该过程不能被时钟中断打断.
    atom_low = Int_LowAtomStart( );   //本函数对不能嵌套调用
#endif
    temp = s_u32RRS_Slice;
#if (64 > CN_CPU_BITS)
    //处理器字长＜32位,需要多个周期才能读取u32g_RRS_slice,该过程不能被时钟中断打断.
    Int_LowAtomEnd( atom_low );
#endif

    return temp * CN_CFG_TICK_US;
}
//----创建线程-----------------------------------------------------------------
//功能：为事件类型创建线程，初始化上下文环境，安装执行函数，构成完整线程
//参数：evtt_id，待创建的线程所服务的事件类型id
//返回：新创建的线程指针
//注: 移植敏感函数
//-----------------------------------------------------------------------------
extern void *(*__MallocStack)(struct EventECB *pl_ecb,u32 size);
struct ThreadVm *__DJY_CreateThread(struct EventType *evtt,u32 *stack_size)
{
    struct ThreadVm  *result;
    ptu32_t  len;

    //计算线程栈:线程+最大单个api需求的栈
    len = evtt->stack_size;
    //栈顶需要对齐，malloc函数能保证栈底是对齐的，对齐长度可以使栈顶对齐
    len = align_up_sys(len);
    result=(struct ThreadVm  *)__MallocStack(g_ptEventReady,len);
    *stack_size = len;
    if(result==NULL)
    {
        DJY_SaveLastError(EN_MEM_TRIED);   //内存不足，返回错误
        return result;
    }
    len = M_CheckSize(result);
    memset(result,'d',len);

    //看实际分配了多少内存，djyos内存分配使用块相联策略，如果分配的内存量大于
    //申请量，可以保证其实际尺寸是对齐的。之所以注释掉，是因为当len大于申请量时，
    //对齐只是实际结果，而不是内存管理的规定动作，如果不注释掉，就要求内存管理
    //模块必须提供对齐的结果，对模块独立性是不利的。
//    len = M_CheckSize(result);
    result->stack_top = (u32*)((ptu32_t)result+len); //栈顶地址，移植敏感
    result->stack_used = result->stack_top;
    result->next = NULL;
    result->stack_size = len - sizeof(struct ThreadVm); //保存栈深度
    result->host_vm = NULL;
    //复位线程并重置线程
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----静态创建线程-------------------------------------------------------------
//功能：为事件类型创建线程，初始化上下文环境，安装执行函数，构成完整线程
//参数：evtt_id，待创建的线程所服务的事件类型id
//返回：新创建的线程指针
//注: 移植敏感函数
//-----------------------------------------------------------------------------
struct ThreadVm *__DJY_CreateStaticThread(struct EventType *evtt,void *Stack,
                                    u32 StackSize)
{
    struct ThreadVm  *result;
    result = (struct ThreadVm  *)align_up_sys(Stack);

    memset(Stack, 'd', StackSize-((ptu32_t)result - (ptu32_t)Stack));

    //看实际分配了多少内存，djyos内存分配使用块相联策略，如果分配的内存量大于
    //申请量，可以保证其实际尺寸是对齐的。之所以注释掉，是因为当len大于申请量时，
    //对齐只是实际结果，而不是内存管理的规定动作，如果不注释掉，就要求内存管理
    //模块必须提供对齐的结果，对模块独立性是不利的。
//    len = M_CheckSize(result);
    result->stack_top = (u32*)align_down_sys((ptu32_t)Stack+StackSize); //栈顶地址，移植敏感
    result->stack_used = result->stack_top;
    result->next = NULL;
    result->stack_size = (ptu32_t)(result->stack_top) - (ptu32_t)result
                            - sizeof(struct ThreadVm);       //保存栈深度
    result->host_vm = NULL;
    //复位线程并重置线程
    __asm_reset_thread(evtt->thread_routine,result);
    return result;
}

//----查找可运行线程----------------------------------------------------------
//功能: 在 pg_event_ready队列中获得一个可以运行的线程.
//      1.从pg_event_ready队列头开始查找,如果该事件还没有连接线程,则试图创建之.
//      2.如果不能建立,肯定是因内存不足，则把该事件放到内存等待表中,继续往下查找.
//      3.如此反复,直到找到一个可以运行的线程.然后把pg_event_ready指针指向该事件
//参数: 无
//返回: 无
//备注: 本函数由操作系统调用,调用前保证关异步信号。
//      由于系统服务事件总是ready,所以总是能找到可以运行的线程.
//----------------------------------------------------------------------------
void __DJY_SelectEventToRun(void)
{
    struct ThreadVm *vm;
    // struct EventType *pl_evtt;  //被操作的事件的类型指针
    struct EventType *pl_evtt;
    while(g_ptEventReady->vm == NULL)
    {
        pl_evtt =& g_tEvttTable[g_ptEventReady->evtt_id &(~CN_EVTT_ID_MASK)];
        if(pl_evtt->my_free_vm != NULL)     //该事件类型有空闲的线程,直接使用
        {
            g_ptEventReady->vm = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = pl_evtt->my_free_vm->next;
        }else       //该事件类型没有空闲的线程,试图创建之
        {
            //创建线程，若不成功，在 __DJY_CreateThread 函数内，g_ptEventReady 将指
            //向就绪队列的下一个。
            vm = __DJY_CreateThread(pl_evtt,&(g_ptEventReady->wait_mem_size));
            if(vm == NULL)                  //创建线程失败
            {
//                __DJY_CutReadyEvent(g_ptEventReady);          //从ready队列取出
            }else                                           //成功创建线程
            {
                g_ptEventReady->vm = vm;
                pl_evtt->vpus++;        //独立事件vpus表示线程数
                                        //关联事件的vpus无意义
            }
        }
    }
}

//----创建进程-----------------------------------------------------------
//功能: 创建进程。
//参数: 无
//返回: 无
//备注: 这只是占一个位而已，在mp模式才有实用价值
//-----------------------------------------------------------------------------
void DJY_CreateProcessVm(void)
{
    static struct ProcessVm my_process;
    g_ptMyProcess = &my_process;
}


//----事件调度-----------------------------------------------------------------
//功能：剥夺running事件的cpu,把cpu交给ready队列的第一个事件.如果ready队列的第一
//      个事件尚未拥有线程,则建立之.建立线程时如果内存不足,则把这个事件放到
//      内存等待链表中,ready指针指向ready队列的下一个事件,依次推之,直到找到一个
//      线程指针非空或者成功创建线程的事件.
//参数：无
//返回：true = 成功切换，false = 未切换直接返回
//备注：1.本函数由操作系统调用
//      2.djyos系统认为,用户禁止中断就是为了能够连续执行,或者保护临界资源.
//      djyos中异步信号相当于高优先级线程,所以全局和异步信号禁止状态
//      下,是不允许事件调度的.
//      3.实时中断是否禁止,与调度无关.
//      4.由于最低优先级的系统服务事件总是ready,因此本函数总是能够找到目标事件
//-----------------------------------------------------------------------------
bool_t __DJY_Schedule(void)
{
    struct EventECB *event;
    u32 time;

//    if(!DJY_QuerySch())
//        return false;
//    Int_CutAsynSignal();
    __DJY_SelectEventToRun();
    if(g_ptEventReady != g_ptEventRunning)
    {//当running事件仍在ready队列中,且内存不足以建立新线程时,可能会出现优先
     //级高于running的事件全部进入内存等待队列的可能.此时执行else子句.
        event = g_ptEventRunning;
        time = (u32)DJY_GetSysTime();
#if CFG_OS_TINY == false
        event->consumed_time += time - g_s64RunningStartTime;
        g_s64RunningStartTime = time;
#endif  //CFG_OS_TINY == false
        g_tEvttTable[event->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_OUT);

        g_ptEventRunning=g_ptEventReady;
//        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        Int_HalfEnableAsynSignal( );
        __asm_switch_context(g_ptEventReady->vm ,event->vm);
        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
    }else
    {//优先级高于running的事件全部进入内存等待队列,下一个要处理的事件就是
     //running事件,无须执行任何操作
        return false;
    }
    return true;
}

//----中断内执行的事件调度------------------------------------------------------
//功能：剥夺running事件的cpu,把cpu交给ready队列的第一个事件.如果ready队列的第一
//      个事件尚不拥有线程,则创建之.建立线程时如果内存不足,则把这个事件放到
//      内存等待链表中,ready指针指向ready队列的下一个事件,依次推之,直到找到一个
//      线程指针非空或者成功创建线程的事件.
//参数：无
//返回：无
//备注：本函数由操作系统在异步信号引擎返回前调用
//      由于最低优先级的 IDLE 事件总是ready,因此总是能够找到调度对象的.
//-----------------------------------------------------------------------------
void __DJY_ScheduleAsynSignal(void)
{
    struct EventECB *event;
    u32 time;

    __DJY_SelectEventToRun();
    if(g_ptEventReady != g_ptEventRunning)
    {//当running事件仍在ready队列中,且内存不足以建立新线程时,可能会出现优先
     //级高于running的事件全部进入内存等待队列的可能.此时执行else子句.
         event=g_ptEventRunning;
         time = (u32)DJY_GetSysTime();
#if CFG_OS_TINY == false
         event->consumed_time += time - g_s64RunningStartTime;
         g_s64RunningStartTime = time;
#endif  //CFG_OS_TINY == false
         g_tEvttTable[event->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_OUT);

         g_ptEventRunning=g_ptEventReady;
         __asm_switch_context_int(g_ptEventReady->vm,event->vm);
         g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
    }else
    {//优先级高于running的事件全部进入内存等待队列,下一个要处理的事件就是
     //running事件,无须执行任何操作
    }
    return;
}

//----登记事件类型------------------------------------------------------------
//功能：登记一个事件类型到系统中,事件类型经过登记后,就可以pop了,否则,系统会
//      拒绝pop该类型事件
//参数：relation，取值为EN_INDEPENDENCE或EN_CORRELATIVE
//      default_prio，本事件类型的默认优先级。
//      vpus_res，系统繁忙时为本类型事件保留的线程数量，
//      vpus_limit，本类型事件所能拥有的线程数量的最大值
//      thread_routine，线程入口函数(事件处理函数)
//      Stack，用户为处理事件的线程分配的栈，NULL表示从heap中分配栈。如果事件是
//              EN_INDEPENDENCE类型，且有多条事件并行处理导致需要为该类型创建多
//              个线程时，从第二个线程开始，就一定是从heap中申请Stack。
//      stack_size，执行thread_routine需要的栈尺寸，不包括thread_routine函数可能
//          调用的系统服务。
//      evtt_name，事件类型名，不同模块之间要交叉弹出事件的话，用事件类型名。
//          本参数允许是NULL，但只要非NULL，就不允许与事件类型表中已有的名字重名
//          名字不超过31个单字节字符
//返回：新事件类型的类型号
//------------------------------------------------------------------------------
u16 DJY_EvttRegist(enum enEventRelation relation,
                       ufast_t default_prio,
                       u16 vpus_res,
                       u16 vpus_limit,
//                       u16 para_limit,
                       ptu32_t (*thread_routine)(void),
                       void *Stack,
                       u32 StackSize,
                       char *evtt_name)
{
    struct BlackBoxThrowPara  parahead;
    char ExpStr[64] = "事件类型控制块耗尽: ";
    u16 i,evtt_offset;

    if((default_prio >= CN_PRIO_SYS_SERVICE) || (default_prio == 0))
    {
        DJY_SaveLastError(EN_KNL_INVALID_PRIO);
        info_printf("djyos","事件类型优先级非法\n\r");
        return CN_EVTT_ID_INVALID;
    }
    Int_SaveAsynSignal();      //禁止调度也就是禁止异步事件
    //查找空闲的事件类型控制块
    for(evtt_offset=0; evtt_offset<CFG_EVENT_TYPE_LIMIT; evtt_offset++)
        if( g_tEvttTable[evtt_offset].property.registered ==0)
            break;
    if(evtt_offset == CFG_EVENT_TYPE_LIMIT)     //没有空闲事件类型控制块
    {
        parahead.DecoderName = NULL;
        parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
        strcat(ExpStr,evtt_name);
        parahead.BlackBoxInfo = (u8*)ExpStr;
        parahead.BlackBoxInfoLen = sizeof(ExpStr);
        parahead.BlackBoxType = CN_BLACKBOX_TYPE_ETCB_EXHAUSTED;
        BlackBox_ThrowExp(&parahead);
        DJY_SaveLastError(EN_KNL_ETCB_EXHAUSTED);
        info_printf("djyos","没有空闲事件控制块: %s\n\r",evtt_name);
        Int_RestoreAsynSignal();
        return CN_EVTT_ID_INVALID;
    }else if(evtt_name != NULL) //新类型有名字，需检查有没有重名
    {
#if CFG_OS_TINY == false
        for(i=0; i<CFG_EVENT_TYPE_LIMIT; i++)
        {
            if(g_tEvttTable[i].property.registered == 1)
            {
                if(strncmp(g_tEvttTable[i].evtt_name,evtt_name,31) == 0)
                {
                    DJY_SaveLastError(EN_KNL_EVTT_HOMONYMY);
                    info_printf("djyos","事件类型重名: %s\n\r",evtt_name);
                    Int_RestoreAsynSignal();
                    return CN_EVTT_ID_INVALID;
                }
            }
        }
        if(strnlen(evtt_name,32) <= 31)
            strcpy(g_tEvttTable[evtt_offset].evtt_name,evtt_name);
        else
        {
            memcpy(g_tEvttTable[evtt_offset].evtt_name,evtt_name,31);
            g_tEvttTable[evtt_offset].evtt_name[31] = '\0';
        }
#endif  //CFG_OS_TINY == false
    }else
    {
#if CFG_OS_TINY == false
        g_tEvttTable[evtt_offset].evtt_name[0] = '\0';   //清空事件类型名
#endif  //CFG_OS_TINY == false
    }

    g_tEvttTable[evtt_offset].default_prio = default_prio;
    g_tEvttTable[evtt_offset].correlativeID = CN_EVENT_ID_INVALID;
    g_tEvttTable[evtt_offset].events = 0;
    g_tEvttTable[evtt_offset].vpus_limit = vpus_limit;
    if(relation == EN_INDEPENDENCE)
    {
        //高优先级事件类型至少保留一个线程
//        if( (vpus_res == 0) && ((default_prio < 0x80)
//                              || (CFG_DYNAMIC_MEM == true) ) )
//            g_tEvttTable[evtt_offset].vpus_res = 1;
//        else
            g_tEvttTable[evtt_offset].vpus_res = vpus_res;
    }else
    {
        g_tEvttTable[evtt_offset].vpus_res = 0;    //关联型事件，vpus_res无效
    }
    g_tEvttTable[evtt_offset].SchHook = (SchHookFunc)DJY_NullFunc;
    g_tEvttTable[evtt_offset].thread_routine = thread_routine;
    g_tEvttTable[evtt_offset].stack_size = StackSize;
//  g_tEvttTable[evtt_offset].mark_event = NULL;
    g_tEvttTable[evtt_offset].done_sync = NULL;
    g_tEvttTable[evtt_offset].pop_sync = NULL;
//    g_tEvttTable[evtt_offset].para_limit = para_limit;
    if(Stack == NULL)
    {
//        if((default_prio<0x80) ||(CFG_DYNAMIC_MEM == false))
//        {//运行模式为si或dlsp，或该事件类型拥有紧急优先级，需预先创建一个线程
//            g_tEvttTable[evtt_offset].my_free_vm =
//                                __DJY_CreateThread(&g_tEvttTable[evtt_offset],&temp);
//            if(g_tEvttTable[evtt_offset].my_free_vm == NULL)
//            {//内存不足，不能创建线程
//                parahead.DecoderName = NULL;
//                parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
//                ExpStr[0] = '\0';
//                strcat(ExpStr,"登记事件类型时内存不足: ");
//                strcat(ExpStr,evtt_name);
//                parahead.BlackBoxInfo = (u8*)ExpStr;
//                parahead.BlackBoxInfoLen = sizeof(ExpStr);
//                parahead.BlackBoxType = CN_BLACKBOX_TYPE_MEM_EVTT;
//                BlackBox_ThrowExp(&parahead);
//                DJY_SaveLastError(EN_MEM_TRIED);
//                info_printf("djyos","%s\n\r",evtt_name);
//                Int_RestoreAsynSignal();
//                return CN_EVTT_ID_INVALID;
//            }else
//            {
//                g_tEvttTable[evtt_offset].vpus = 1;
//            }
//        }else
//        {
            g_tEvttTable[evtt_offset].vpus = 0;
            g_tEvttTable[evtt_offset].my_free_vm = NULL;
//        }
    }
    else
    {
        g_tEvttTable[evtt_offset].vpus = 1;
        g_tEvttTable[evtt_offset].my_free_vm =
               __DJY_CreateStaticThread(&g_tEvttTable[evtt_offset],Stack,StackSize);
    }
    g_tEvttTable[evtt_offset].property.correlative = relation;
    g_tEvttTable[evtt_offset].property.registered = 1;
    g_tEvttTable[evtt_offset].property.inuse = 0;
    g_tEvttTable[evtt_offset].property.deleting = 0;
    g_tEvttTable[evtt_offset].pop_times = 0;
    Int_RestoreAsynSignal();
    return evtt_offset | CN_EVTT_ID_MASK;
}


//----注册调度hook函数---------------------------------------------------------
//功能：注册调度hook函数, 该函数在事件上下文切入和切离时调用。
//参数：EvttID, 事件类型ID
//      HookFunc, hook函数指针
//返回：true = 成功注册, false = 失败。
//----------------------------------------------------------------------------
bool_t DJY_RegisterHook(u16 EvttID,SchHookFunc HookFunc)
{
    if(EvttID-CN_EVTT_ID_BASE  >= CFG_EVENT_TYPE_LIMIT)
    {
        return false;
    }
    else
    {
        //无须判断相应EvttID是否被使用
        g_tEvttTable[EvttID-CN_EVTT_ID_BASE].SchHook = HookFunc;
    }
    return true;
}

//----取事件类型id-------------------------------------------------------------
//功能：根据事件类型的名字，返回该事件类型的id号，不查找没名字的事件类型。
//参数：evtt_name，事件类型名
//返回：事件类型id号，如果没有找到则返回cn_invalid_id。
//备注：只能找到有名字的事件类型，没名字的事件类型又叫隐身事件类型。
//----------------------------------------------------------------------------
u16 DJY_GetEvttId(char *evtt_name)
{
#if CFG_OS_TINY == false
    u16 loop;
    if(evtt_name == NULL)
        return CN_EVTT_ID_INVALID;
    for(loop = 0; loop < CFG_EVENT_TYPE_LIMIT; loop++)
    {
        if((strcmp(g_tEvttTable[loop].evtt_name,evtt_name) ==0)
                &&(g_tEvttTable[loop].property.registered))
            return loop | CN_EVTT_ID_MASK;
    }
    return CN_EVTT_ID_INVALID;
#else
    return CN_EVTT_ID_INVALID;
#endif  //CFG_OS_TINY == false
}

//----删除事件类型-------------------------------------------------------------
//功能: 删除一个事件类型,如果队列中还有该类型事件(inuse == 1),只标记该类型为被
//      删除,真正的删除工作是在 done函数里完成的.如果队列中已经没有该类型事件了,
//      将会执行真正的删除操作.无论哪种情况,系统均会拒绝弹出该类型的新事件.
//参数: evtt_id,待删除的事件类型号
//返回: true = 成功，包括成功标记；false = 失败
//-----------------------------------------------------------------------------
bool_t DJY_EvttUnregist(u16 evtt_id)
{
    struct ThreadVm *next_vm,*temp;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb,*pl_ecb_temp;
    bool_t result = true;
    if((evtt_id & (~CN_EVTT_ID_MASK)) >= CFG_EVENT_TYPE_LIMIT)
        return false;
    Int_SaveAsynSignal();
    pl_evtt = &g_tEvttTable[evtt_id & (~CN_EVTT_ID_MASK)];
    if(pl_evtt->property.registered == 0)
    {//该事件类型是无效事件类型
        result = false;
    }
//  if(pl_evtt->done_sync != NULL)     //若完成同步队列中有事件，取出到就绪队列
//  {
//      pl_ecb = pl_evtt->done_sync;     //取同步队列头
//      while(pl_ecb != NULL)
//      {
//          if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //是否在超时队列中
//          {
//              __DJY_ResumeDelay(pl_ecb);    //结束超时等待
//          }
//          pl_ecb->wakeup_from = CN_STS_EVTTSYNC_DELETED;   //设置唤醒原因
//          pl_ecb->event_status = CN_STS_EVENT_READY;
//          pl_ecb_temp = pl_ecb;
//          if(pl_ecb->multi_next == pl_evtt->done_sync)   //是最后一个事件
//          {
//              pl_evtt->done_sync = NULL;
//              pl_ecb = NULL;
//          }else
//          {
//              pl_ecb = pl_ecb->multi_next;
//          }
//          __DJY_EventReady(pl_ecb_temp);           //把事件加入到就绪队列中
//      }
//  }
    if(pl_evtt->pop_sync != NULL)     //若弹出同步队列中有事件，取出到就绪队列
    {
        pl_ecb = pl_evtt->pop_sync;     //取同步队列头
        while(pl_ecb != NULL)
        {
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //是否在超时队列中
            {
                __DJY_ResumeDelay(pl_ecb);    //结束超时等待
            }
            pl_ecb->wakeup_from = CN_STS_EVTTSYNC_DELETED;    //设置唤醒原因
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == pl_evtt->pop_sync)  //是最后一个事件
            {
                pl_evtt->pop_sync = NULL;  //置空事件同步队列
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;      //取队列中下一个事件
            }
            __DJY_EventReady(pl_ecb_temp);       //把事件加入到就绪队列中
        }
    }
    if(pl_evtt->property.inuse)
    {
        //事件类型正在使用，或完成同步和弹出同步队列非空，只标记删除
        pl_evtt->property.deleting = 1;
    }else
    {
        //回收事件类型控制块，只需把registered属性清零。
        pl_evtt->property.registered = 0;
#if CFG_OS_TINY == false
        pl_evtt->evtt_name[0] = '\0';      //清空事件类型名
#endif  //CFG_OS_TINY == false
        next_vm = pl_evtt->my_free_vm;
        while(next_vm != NULL)          //释放该事件类型拥有的空闲线程
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
    }
    Int_RestoreAsynSignal();
    return result;
}
const struct EventECB cn_sys_event = {
                        NULL,NULL,                  //next,previous
                        NULL,NULL,                  //multi_next,multi_previous
                        NULL,                       //vm
                        0,0,                        //param1,param2
                        0,                          //userdata
                        NULL,                       //sync
                        NULL,                       //sync_head
#if CFG_OS_TINY == false
                        0,                          //EventStartTime
                        0,                          //consumed_time
                        0,                          //consumed_time_second
                        0,                          //consumed_time_record
#endif
                        0,                          //delay_start_tick
                        0,                          //delay_end_tick
                        EN_KNL_NO_ERROR,            //error_no
                        0,                          //event_result
                        0,                          //wait_mem_size
                        0,                          //HeapSize
                        0,                          //HeapSizeMax
                        CN_STS_EVENT_NORUN,          //wakeup_from
                        CN_STS_EVENT_READY,         //event_status
                        0,                          //prio_raise_cnt
                        CN_PRIO_SYS_SERVICE,        //prio_base
                        CN_PRIO_SYS_SERVICE,        //prio
                        CN_EVTT_ID_BASE,            //evtt_id
                        0,                          //sync_counter
                        0,                          //event_id
                        0                           //local_memory
                        };

//----建立事件链表-------------------------------------------------------------
//功能：1.根据系统设定初始化操作系统线程和事件表指针
//      2.把 CFG_EVENT_LIMIT 个事件控制块结构连接成一个空闲队列,队列的结构参见
//        文档，由指针pg_free_event索引,
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void __DJY_InitSys(void)
{
    u16 i;
    u8 *IdleStack;
    struct ThreadVm *vm;
    g_ptEventDelay=NULL;    //延时事件链表空

    IdleStack = M_Malloc(CFG_IDLESTACK_LIMIT,0);
    if(g_tEvttTable == NULL)
    {
        while(1);       //此时IO系统未准备好，能跑到这里，开发调试时必然能发现。
    }
    //把事件类型表全部置为没有注册,0为系统服务类型
    for(i=1; i<CFG_EVENT_TYPE_LIMIT; i++)
    {
        g_tEvttTable[i].property.registered = 0;
    }
    for(i = 1; i < CFG_EVENT_LIMIT; i++)    //i=0为系统服务事件
    {
        if(i==(CFG_EVENT_LIMIT-1))
            g_tECB_Table[i].next = NULL;
        else
            g_tECB_Table[i].next = &g_tECB_Table[i+1];
        //向前指针指向pg_event_free的地址,说明这事个空闲事件块.
        //没有别的含义,只是找一个唯一且不变的数值,全局变量地址在整个运行期
        //是不会变化的.
        g_tECB_Table[i].previous = (struct EventECB*)&s_ptEventFree;
        g_tECB_Table[i].event_id = i;    //本id号在程序运行期维持不变
        g_tECB_Table[i].evtt_id = CN_EVENT_ID_INVALID;
    }
    s_ptEventFree = &g_tECB_Table[1];

    g_ptEventReady = g_tECB_Table;
    g_ptEventRunning = g_ptEventReady;
    g_tEvttTable[0].property.correlative = 1;
//    g_tEvttTable[0].property.overlay = 0;
    g_tEvttTable[0].property.registered = 1;
    g_tEvttTable[0].property.inuse = 1;
    g_tEvttTable[0].property.deleting = 0;
    g_tEvttTable[0].my_free_vm = NULL;
    g_tEvttTable[0].SchHook = (SchHookFunc)DJY_NullFunc;
   // g_tEvttTable[0].evtt_name[0] = '\0';

#if CFG_OS_TINY == false
    strcpy(g_tEvttTable[0].evtt_name,"sys_idle");
#endif  //CFG_OS_TINY == false

    g_tEvttTable[0].default_prio = CN_PRIO_SYS_SERVICE;
    g_tEvttTable[0].correlativeID = CN_EVENT_ID_INVALID;
    g_tEvttTable[0].events = 1;
    g_tEvttTable[0].vpus_res =0;
    g_tEvttTable[0].vpus_limit =1;
    g_tEvttTable[0].vpus = 1;
    g_tEvttTable[0].thread_routine = __DJY_Service;
    g_tEvttTable[0].stack_size = CFG_IDLESTACK_LIMIT;
//  g_tEvttTable[0].mark_event = g_tECB_Table;
    g_tEvttTable[0].done_sync = NULL;
    g_tEvttTable[0].pop_sync = NULL;

    vm = __DJY_CreateStaticThread(&g_tEvttTable[0],IdleStack,CFG_IDLESTACK_LIMIT);
    if(vm == NULL)      //内存不足，不能创建常驻线程
    {
        //此时g_ptEventRunning尚未赋值，无法调用Djy_SaveLastError
//        DJY_SaveLastError(EN_MEM_TRIED);
//        info_printf("djyos","创建线程时内存不足\n\r");
        g_tEvttTable[0].vpus = 0;
        return ;
    }
    //以下模拟popup函数,弹出系统服务事件.当事件队列为空时,调用popup的结果
    //是不可预知的.由于系统运行时,系统服务事件总在队列中,所以事件队列是不会空
    //的,这里模拟popup事件,而不是直接调用,可以使popup函数中省掉一个判断队列是否
    //空的操作.
    g_tECB_Table[0] = cn_sys_event;
    g_tECB_Table[0].vm = vm;
    g_tECB_Table[0].next = g_tECB_Table;
    g_tECB_Table[0].previous = g_tECB_Table;
    g_tECB_Table[0].multi_next = g_tECB_Table;
    g_tECB_Table[0].multi_previous = g_tECB_Table;
}

//--------检查是否允许调度------------------------------------------------------
//功能: 检查是否允许调度,允许异步信号且运行在线程态时，允许调度
//参数: 无
//返回: 允许调度返回true,否则返回false
//-----------------------------------------------------------------------------
bool_t DJY_QuerySch(void)
{
    return g_bScheduleEnable;
}

//--------检查调度是否已经开始--------------------------------------------------
//功能: 检查是否已经启动多事件(线程)调度,调用__Djy_StartOs函数后,本函数即返回true,
//      否则返回false
//参数: 无
//返回: true = 调度已经开始,不管是否允许切换.false = 调度尚未开始
//-----------------------------------------------------------------------------
bool_t DJY_IsMultiEventStarted(void)
{
    return g_bMultiEventStarted;
}

//--------检查系统运行模式--------------------------------------------------
//功能: 检查系统的运行模式，参看 CN_RUNMODE_SI 族宏定义
//参数: 无
//返回: g_u32OsRunMode的值，参看 CN_RUNMODE_SI 族宏定义
//-----------------------------------------------------------------------------
u32 DJY_GetRunMode(void)
{
    return g_u32OsRunMode;
}

//----把事件插入就绪队列中----------------------------------------------------
//功能：把事件插入到就绪队列中合适的位置,该事件原来不在就绪队列中.本函数只处理
//      事件队列，不设置event_status的值
//参数：event_ready,待插入的事件,该事件原来不在就绪队列中
//返回：无
//------------------------------------------------------------------------------
void __DJY_EventReady(struct EventECB *event_ready)
{
    struct EventECB *event;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart(  );
    event = g_ptEventReady;
    do
    { //找到一个优先级低于新事件的事件.由于系统服务事件prio=250总是ready,因此总是能找到.
        if(event->prio <= event_ready->prio)
            event = event->multi_next;
        else
            break;
    }while(event != g_ptEventReady);
    event_ready->next = event;
    event_ready->previous = event->previous;
    event->previous->next = event_ready;
    event->previous = event_ready;

    //新插入的事件在同优先级的最后，故这样能够判断新事件是否该优先级的唯一事件。
    //若是该优先级的唯一事件，就要插入优先级单调队列
    if(event_ready->prio != event_ready->previous->prio)
    {//此时event == event_ready->next;
        event->multi_previous->multi_next = event_ready;
        event_ready->multi_previous = event->multi_previous;
        event->multi_previous = event_ready;
        event_ready->multi_next = event;
    }else       //无须插入优先级单调队列
    {
        event_ready->multi_next = NULL;
        event_ready->multi_previous = NULL;
    }
    if(event_ready->prio < g_ptEventReady->prio)
        g_ptEventReady = event_ready;
    Int_LowAtomEnd( atom_low );
}

//----退出闹钟同步队列---------------------------------------------------------
//功能: 把一个事件从闹钟同步队列中取出,不管闹铃时间是否已经到达.用在带超时的同步
//      功能中，当同步条件先于超时时限到达时，从闹钟同步队列取出事件。
//参数: delay_event,待处理的事件.
//返回: 无
//备注: 1.本函数不开放给用户,仅仅是操作系统内部使用.操作系统不给用户提供提前
//      退出延时的功能,这样可以防止事件间互相干涉
//      2.本函数应该在关闭调度条件下调用,调用者保证,函数内部不检查中断状态.
//      3.本函数只把事件从闹钟同步链表中取出，并不放到就绪队列中。
//-----------------------------------------------------------------------------
void __DJY_ResumeDelay(struct EventECB *delay_event)
{
    if(g_ptEventDelay->next == g_ptEventDelay)  //队列中只有一个事件
    {
        g_ptEventDelay = NULL;
//      gSchduleTick.DelayTick = CN_LIMIT_UINT64;
    }
    else
    {
        if(delay_event == g_ptEventDelay)
        {
            g_ptEventDelay = g_ptEventDelay->next;
//          gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
        }
        delay_event->next->previous = delay_event->previous;
        delay_event->previous->next = delay_event->next;
    }
    delay_event->next = NULL;
    delay_event->previous = NULL;
    delay_event->delay_end_tick = __DJY_GetSysTick();
}

//----加入延时队列------------------------------------------------------------
//功能：由正在执行的事件调用,直接把自己加入延时队列，不引起调度也不操作ready
//      队列，一般由同步函数调用，在timeout!=0时把事件加入闹钟同步队列实现
//      timeout功能，是特定条件下对y_timer_sync函数的简化。
//参数：u32l_uS,延迟时间,单位是微秒，将被向上调整为CN_CFG_TICK_US的整数倍
//返回：无
//备注：1、操作系统内部使用的函数，且需在关闭中断（禁止调度）的条件下使用。
//      2、调用本函数前running事件已经从就绪表中取出，本函数不改变就绪表。
//      3、与其他内部函数一样，由调用方保证参数合理性，即u32l_uS>0.
//-----------------------------------------------------------------------------
//change by lst in 20130922,ticks改为64bit后，删掉处理32位数溢出回绕的代码
void __DJY_AddToDelay(u32 u32l_uS)
{
    struct EventECB * event;

    g_ptEventRunning->delay_start_tick = __DJY_GetSysTick(); //事件延时开始时间
    g_ptEventRunning->delay_end_tick = g_ptEventRunning->delay_start_tick
                  + ((s64)u32l_uS + CN_CFG_TICK_US -(u32)1)/CN_CFG_TICK_US; //闹铃时间
    if(g_ptEventDelay==NULL)    //延时队列空
    {
        g_ptEventRunning->next = g_ptEventRunning;
        g_ptEventRunning->previous = g_ptEventRunning;
        g_ptEventDelay=g_ptEventRunning;
//      gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
    }else
    {
        event = g_ptEventDelay;
        do
        {//本循环找到第一个剩余延时时间大于新延时事件的事件.
            if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
            {
                event = event->next;
            }
            else
                break;

        }while(event != g_ptEventDelay);
        //如果没有找到剩余延时时间比新延时事件长的事件,新事件插入队列尾,
        //而队列尾部就是pg_event_delay的前面,event恰好等于pg_event_delay
        //如果找到剩余延时时间长于新事件的事件,新事件插入该事件前面.
        //所以无论前面循环找到与否,均可使用下列代码
        g_ptEventRunning->next = event;
        g_ptEventRunning->previous = event->previous;
        event->previous->next = g_ptEventRunning;
        event->previous = g_ptEventRunning;
        if(g_ptEventDelay->delay_end_tick > g_ptEventRunning->delay_end_tick)
        {
            //新事件延时小于原队列中的最小延时.
            g_ptEventDelay = g_ptEventRunning;
//          gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
        }
    }
}

//----添加事件进阻塞队列-------------------------------------------------
//功能：功能类似 __DJY_AddRunningToBlock，把 Ready 队列头部事件加入阻塞队列，且
//      不能设置超时函数。设计本函数是为了解决调度过程中创建线程时，需要从堆中分
//      配栈，但此时堆的互斥量被占用，导致不能分配，进而导致的一系列问题。
//      只支持FIFO方式或者优先级排序，其他排序方式不能使用本函数
//      本函数只允许内核模块调用，调用者保证并发安全。
//参数：Head,阻塞队列头
//      Qsort，阻塞队列排队方式，只支持CN_BLOCK_FIFO和CN_BLOCK_PRIO
//      Status，阻塞状态，参看CN_STS_EVENT_READY系列定义
//返回：无
//-----------------------------------------------------------------------------
void __DJY_AddToBlockForStack(struct EventECB **Head,bool_t Qsort,u32 Status)
{
    struct EventECB *current;
    struct EventECB *event = g_ptEventReady;

    __DJY_CutReadyEvent(event);
    event->previous = NULL;
    event->next = NULL;

    event->sync_head = Head;
    if(*Head == NULL)
    {//同步队列空,running事件自成双向循环链表
        event->multi_next = event;
        event->multi_previous = event;
        *Head = event;
    }else
    {
        current = *Head;
        if(Qsort == CN_BLOCK_PRIO)   //同步队列按优先级排序
        {
            do
            {   //找到一个优先级低于新事件的事件.
                //如果找不到，会停在队列头的位置，该位置也是正确的插入点。
                if(current->prio <= event->prio)
                    current = current->multi_next;
                else
                    break;
            }while(current != *Head);
            event->event_status = CN_BLOCK_PRIO_SORT;
            event->multi_next = current;
            event->multi_previous = current->multi_previous;
            current->multi_previous->multi_next = event;
            current->multi_previous = event;
            if((*Head)->prio > event->prio)
                *Head = event;
        }else                               //按先后顺序，新事件直接排在队尾
        {
            event->event_status &= ~CN_BLOCK_PRIO_SORT;
            event->multi_next = current;
            event->multi_previous = current->multi_previous;
            current->multi_previous->multi_next = event;
            current->multi_previous = event;
        }
    }
    event->event_status |= Status;
}

//----从ready事件继承优先级-----------------------------------------------------
//功能: 如果 src_id 的优先级较高，event_id临时以 src_id 的优先级，运行，直到调用
//      DJY_RestorePrio，否则不改变优先级。本函数专用在创建线程时，从堆中分配栈的
//      过程中，因为阻塞而需要优先级继承的情况。此时不是从 running 中继承优先级，
//      而是从 ready 中继承。
//参数: event_id，被操作的事件id
//返回: true = 成功设置，false=失败，一般是优先级不合法
//注：本函数是分配 stack 的函数专用，在禁止调度条件下使用。
//-----------------------------------------------------------------------------
bool_t __DJY_RaiseTempPrioForStack(u16 event_id)
{
    struct EventECB * pl_ecb;

    if(event_id >= CFG_EVENT_LIMIT)
        return false;

    pl_ecb = &g_tECB_Table[event_id];
    if(g_ptEventReady->prio < pl_ecb->prio)
    {
        //事件原来的状态，可能：1、处于就绪态，2、处于某种阻塞态。
        //如果处于某种阻塞态，假如队列是按优先级排序的，则要修改该队列。
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //注：此三句的顺序不能变，因为prio的值对__Djy_CutReadyEvent函数执行
            //    结果有影响
            __DJY_CutReadyEvent(pl_ecb);
            pl_ecb->prio = g_ptEventReady->prio;
            __DJY_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = g_ptEventReady->prio;
            __DJY_ChangeBlockQueue(pl_ecb);
        }
    }
    return true;
}

//----添加处理中事件进阻塞队列-------------------------------------------------
//功能：把正在运行的事件，添加进阻塞队列，只添加队列，不调度。
//      只支持FIFO方式或者优先级排序，其他排序方式不能使用本函数
//      本函数只允许内核模块调用，调用者保证并发安全。
//参数：Head,阻塞队列头
//      Qsort，阻塞队列排队方式，只支持CN_BLOCK_FIFO和CN_BLOCK_PRIO
//      timeout，超时设置,不等于0则进入阻塞队列的同时，还进入超时队列。
//         单位是微秒，timeout==CN_TIMEOUT_FOREVER则不会进入超时队列，
//         timeout ==0则不应调用本函数。非0值将被向上调整为CN_CFG_TICK_US的倍数
//      Status，阻塞状态，参看CN_STS_EVENT_READY系列定义
//返回：无
//-----------------------------------------------------------------------------
void __DJY_AddRunningToBlock(struct EventECB **Head,bool_t Qsort,u32 timeout,u32 Status)
{
    struct EventECB *event;
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->next = NULL;

    g_ptEventRunning->sync_head = Head;
    if(*Head == NULL)
    {//同步队列空,running事件自成双向循环链表
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
        *Head = g_ptEventRunning;
    }else
    {
        event = *Head;
        if(Qsort == CN_BLOCK_PRIO)   //同步队列按优先级排序
        {
            do
            {   //找到一个优先级低于新事件的事件.
                //如果找不到，会停在队列头的位置，该位置也是正确的插入点。
                if(event->prio <= g_ptEventRunning->prio)
                    event = event->multi_next;
                else
                    break;
            }while(event != *Head);
            g_ptEventRunning->event_status |= CN_BLOCK_PRIO_SORT;
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
            if((*Head)->prio > g_ptEventRunning->prio)
                *Head = g_ptEventRunning;
        }else                               //按先后顺序，新事件直接排在队尾
        {
            g_ptEventRunning->event_status &= ~CN_BLOCK_PRIO_SORT;
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
        }
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        //事件状态设为等待信号量 +  超时
        g_ptEventRunning->event_status |= Status + CN_STS_SYNC_TIMEOUT;
        __DJY_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status |= Status;  //事件状态设为等待信号量
    }
}

//----修改阻塞队列-------------------------------------------------------------
//功能：调整指定事件在以PRIO排序的阻塞队列中的位置，在修改了阻塞中的事件优先级后，
//      就需要调用本函数改变其在队列中的位置。
//      本函数只允许内核模块调用，调用者保证并发安全。
//参数：Event，被操作的事件
//返回：无
//-----------------------------------------------------------------------------
void __DJY_ChangeBlockQueue(struct EventECB *Event)
{
    struct EventECB *pl_ecb;
    struct EventECB *Head;
    ufast_t Prio;
    Head = *Event->sync_head;
    pl_ecb = Head;
    Prio = Event->prio;
    //无须移动位置的条件：
    //第一行：处于队列头，且最小。
    //第二行：优先不低于后一个，不高于前一个。
    //第三行：处于队列尾，且最大。
    if( ((Event == pl_ecb)  && (Prio <= Event->multi_next->prio))
       ||((Prio >= Event->multi_previous->prio)&& (Prio <= Event->multi_next->prio))
       ||((Event == pl_ecb->multi_previous)&& (Prio >= Event->multi_previous->prio)) )
        return ;

    //从队列中取出事件控制块
    Event->multi_previous->multi_next = Event->multi_next;
    Event->multi_next->multi_previous = Event->multi_previous;
    do
    {   //找到一个优先级低于新事件的事件.
        //如果找不到，会停在队列头的位置，该位置也是正确的插入点。
        if(pl_ecb->prio < Event->prio)
            pl_ecb = pl_ecb->multi_next;
        else
            break;
    }while(pl_ecb != Head);

    Event->multi_next = pl_ecb;
    Event->multi_previous = pl_ecb->multi_previous;
    pl_ecb->multi_previous->multi_next = Event;
    pl_ecb->multi_previous = pl_ecb;
    if((Head)->prio > Event->prio)
        *Event->sync_head = Event;
}

//----设置事件优先级-----------------------------------------------------------
//功能: 改变事件优先级，如果改变自身的优先级，并把优先级被改低了，可能立即调度，
//      阻塞本事件运行。
//参数: event_id，被操作的事件id
//      new_prio,设置的新优先级
//返回: true = 成功设置，false=失败，一般是优先级不合法
//-----------------------------------------------------------------------------
bool_t DJY_SetEventPrio(u16 event_id,ufast_t new_prio)
{
    struct EventECB * pl_ecb;

    if((new_prio >= CN_PRIO_SYS_SERVICE) || (new_prio == 0)
       || (event_id >= CFG_EVENT_LIMIT))
        return false;

    Int_SaveAsynSignal();
    pl_ecb = &g_tECB_Table[event_id];
    pl_ecb->prio_base = new_prio;
    if(new_prio != pl_ecb->prio)
    {
        //事件原来的状态，可能：1、处于就绪态，2、处于某种阻塞态。
        //如果处于某种阻塞态，假如队列是按优先级排序的，则要修改该队列。
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //注：此三句的顺序不能变，因为prio的值对__Djy_CutReadyEvent函数执行
            //    结果有影响
            __DJY_CutReadyEvent(pl_ecb);
            pl_ecb->prio = new_prio;
            __DJY_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = new_prio;
            __DJY_ChangeBlockQueue(pl_ecb);
        }
        else
            pl_ecb->prio = new_prio;
    }
    Int_RestoreAsynSignal();
    return true;
}

//----设置事件优先级-----------------------------------------------------------
//功能: 改变事件优先级，如果改变自身的优先级，并把优先级被改低了，可能立即调度，
//      阻塞本事件运行。
//参数: event_id，被操作的事件id
//      new_prio,设置的新优先级
//返回: true = 成功设置，false=失败，一般是优先级不合法
//注意：名字改成DJY_SetEventPrio函数后和C库有冲突(__assert中)，copy了一份，改了C库之后删掉
//-----------------------------------------------------------------------------
bool_t Djy_SetEventPrio(u16 event_id,ufast_t new_prio)
{
    struct EventECB * pl_ecb;

    if((new_prio >= CN_PRIO_SYS_SERVICE) || (new_prio == 0)
       || (event_id >= CFG_EVENT_LIMIT))
        return false;

    Int_SaveAsynSignal();
    pl_ecb = &g_tECB_Table[event_id];
    pl_ecb->prio_base = new_prio;
    if(new_prio != pl_ecb->prio)
    {
        //事件原来的状态，可能：1、处于就绪态，2、处于某种阻塞态。
        //如果处于某种阻塞态，假如队列是按优先级排序的，则要修改该队列。
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //注：此三句的顺序不能变，因为prio的值对__Djy_CutReadyEvent函数执行
            //    结果有影响
            __DJY_CutReadyEvent(pl_ecb);
            pl_ecb->prio = new_prio;
            __DJY_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = new_prio;
            __DJY_ChangeBlockQueue(pl_ecb);
        }
        else
            pl_ecb->prio = new_prio;
    }
    Int_RestoreAsynSignal();
    return true;
}

//----继承事件优先级-----------------------------------------------------------
//功能: 如果g_ptEventRunning的优先级较高，event_id临时以g_ptEventRunning的优先级
//      运行，直到调用Djy_RestorePrio，否则不改变优先级。
//参数: event_id，被操作的事件id
//返回: true = 成功设置，false=失败，一般是优先级不合法
//-----------------------------------------------------------------------------
bool_t DJY_RaiseTempPrio(u16 event_id)
{
    struct EventECB * pl_ecb;

    if(event_id >= CFG_EVENT_LIMIT)
        return false;

    Int_SaveAsynSignal();
    pl_ecb = &g_tECB_Table[event_id];
    pl_ecb->prio_raise_cnt++;
    if(g_ptEventRunning->prio < pl_ecb->prio)
    {
        //事件原来的状态，可能：1、处于就绪态，2、处于某种阻塞态。
        //如果处于某种阻塞态，假如队列是按优先级排序的，则要修改该队列。
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //注：此三句的顺序不能变，因为prio的值对__Djy_CutReadyEvent函数执行
            //    结果有影响
            __DJY_CutReadyEvent(pl_ecb);
            pl_ecb->prio = g_ptEventRunning->prio;
            __DJY_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = g_ptEventRunning->prio;
            __DJY_ChangeBlockQueue(pl_ecb);
        }
        else
            pl_ecb->prio = g_ptEventRunning->prio;
    }
    Int_RestoreAsynSignal();
    return true;
}

//----恢复事件优先级-----------------------------------------------------------
//功能: 调用Djy_SetTempPrio后，可以调用本函数恢复g_ptEventRunning的优先级。
//参数: 无，
//返回: true = 成功设置，false=失败，一般是优先级不合法
//-----------------------------------------------------------------------------
bool_t DJY_RestorePrio(void)
{

    Int_SaveAsynSignal();
    g_ptEventRunning->prio_raise_cnt--;
    if(g_ptEventRunning->prio_raise_cnt == 0)
    {
        if(g_ptEventRunning->prio != g_ptEventRunning->prio_base)
        {
            //注：此三句的顺序不能变，因为prio的值对__Djy_CutReadyEvent函数执行
            //    结果有影响
            __DJY_CutReadyEvent(g_ptEventRunning);
            g_ptEventRunning->prio = g_ptEventRunning->prio_base;
            __DJY_EventReady(g_ptEventRunning);
        }
    }
    Int_RestoreAsynSignal();
    return true;
}

//----闹钟同步-----------------------------------------------------------------
//功能：由正在执行的事件调用,使自己暂停u32l_uS微秒后继续运行.
//参数：u32l_uS,延迟时间,单位是微秒，0且允许轮转调度则把事件放到同优先级的
//      最后。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//返回：实际延时时间us数
//备注：延时队列为双向循环链表
//change by lst in 20130922,ticks改为64bit后，删掉处理32位数溢出回绕的代码
//-----------------------------------------------------------------------------
u32 DJY_EventDelay(u32 u32l_uS)
{
    struct EventECB * event;
    u32 us_return=0;

    if( !DJY_QuerySch())
    {
//        DJY_SaveLastError(EN_KNL_CANT_SCHED);
        DJY_DelayUs(u32l_uS);
        return u32l_uS;
    }
    Int_SaveAsynSignal();
    //延时量为0的算法:就绪队列中有同优先级的，把本事件放到轮转最后一个，
    //注意:这里不管轮转调度是否允许
    if(u32l_uS == 0)
    {
        if((g_ptEventRunning->prio == g_ptEventRunning->next->prio)
                    && (g_ptEventRunning != g_ptEventRunning->next)   )
        {
            g_ptEventRunning->delay_start_tick = __DJY_GetSysTick();//闹铃时间
            __DJY_CutReadyEvent(g_ptEventRunning);          //从ready队列取出
            __DJY_EventReady(g_ptEventRunning);             //放回同步队列尾部
        }else
        {
            Int_RestoreAsynSignal();
            return 0;   //延时量为0，且不符合轮转条件
        }
    }else
    {
        g_ptEventRunning->delay_start_tick =__DJY_GetSysTick();//设定闹铃的时间
        g_ptEventRunning->delay_end_tick = g_ptEventRunning->delay_start_tick
                  + ((s64)u32l_uS + CN_CFG_TICK_US -(u32)1)/CN_CFG_TICK_US; //闹铃时间

        __DJY_CutReadyEvent(g_ptEventRunning);

        g_ptEventRunning->event_status = CN_STS_EVENT_DELAY;
        if(g_ptEventDelay==NULL)    //闹钟同步队列空
        {
            g_ptEventRunning->next = g_ptEventRunning;
            g_ptEventRunning->previous = g_ptEventRunning;
            g_ptEventDelay=g_ptEventRunning;
//          gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
        }else
        {
            event = g_ptEventDelay;
            do
            {//本循环找到第一个闹铃时间晚于新事件的事件.
                if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
                {
                    event = event->next;
                }
                else
                    break;
            }while(event != g_ptEventDelay);
            //下面把新事件插入前述找到的事件前面，如没有找到，则event将等于
            //pg_event_delay，因是双向循环队列，g_event_delay前面也就刚好是队列尾。
            g_ptEventRunning->next = event;
            g_ptEventRunning->previous = event->previous;
            event->previous->next = g_ptEventRunning;
            event->previous = g_ptEventRunning;
            if(g_ptEventDelay->delay_end_tick >g_ptEventRunning->delay_end_tick)
            //新事件延时小于原队列中的最小延时.
            {
                g_ptEventDelay = g_ptEventRunning;
//              gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
            }
        }
    }
    Int_RestoreAsynSignal();
    us_return = (__DJY_GetSysTick() -g_ptEventRunning->delay_start_tick)*CN_CFG_TICK_US;
    return us_return;
}

//----闹钟同步2----------------------------------------------------------------
//功能：由正在执行的事件调用,使自己暂停至s64l_uS微秒后继续运行.
//参数：s64l_uS,延迟结束时刻,单位是微秒，将被向上调整为CN_CFG_TICK_US的整数倍
//返回：实际延时时间us数
//备注：延时队列为双向循环链表
//add by lst in 20130922
//-----------------------------------------------------------------------------
s64 DJY_EventDelayTo(s64 s64l_uS)
{
    struct EventECB * event;

    if( !DJY_QuerySch())
    {
//      DJY_SaveLastError(EN_KNL_CANT_SCHED);
        DJY_DelayUs((u32)(s64l_uS - DJY_GetSysTime()));
        return 0;
    }
    Int_SaveAsynSignal();
    g_ptEventRunning->delay_start_tick =__DJY_GetSysTick();//设定闹铃的时间
    g_ptEventRunning->delay_end_tick =(s64l_uS +CN_CFG_TICK_US -1)/CN_CFG_TICK_US;
    if(g_ptEventRunning->delay_end_tick <= g_ptEventRunning->delay_start_tick)
    {
        Int_RestoreAsynSignal();
        return 0;
    }

    __DJY_CutReadyEvent(g_ptEventRunning);

    g_ptEventRunning->event_status = CN_STS_EVENT_DELAY;
    if(g_ptEventDelay==NULL)    //闹钟同步队列空
    {
        g_ptEventRunning->next = g_ptEventRunning;
        g_ptEventRunning->previous = g_ptEventRunning;
        g_ptEventDelay=g_ptEventRunning;
//      gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
    }else
    {
        event = g_ptEventDelay;
        do
        {//本循环找到第一个闹铃时间晚于新事件的事件.
            if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
            {
                event = event->next;
            }
            else
                break;
        }while(event != g_ptEventDelay);
        //下面把新事件插入前述找到的事件前面，如没有找到，则event将等于
        //pg_event_delay，因是双向循环队列，g_event_delay前面也就刚好是队列尾。
        g_ptEventRunning->next = event;
        g_ptEventRunning->previous = event->previous;
        event->previous->next = g_ptEventRunning;
        event->previous = g_ptEventRunning;
        if(g_ptEventDelay->delay_end_tick >g_ptEventRunning->delay_end_tick)
        {
            //新事件延时小于原队列中的最小延时.
            g_ptEventDelay = g_ptEventRunning;
//          gSchduleTick.DelayTick = g_ptEventDelay->delay_end_tick;
        }
    }
    Int_RestoreAsynSignal();
    return (__DJY_GetSysTick() -g_ptEventRunning->delay_start_tick)*CN_CFG_TICK_US;
}

//----同步事件----------------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件的同步队列中去,然后重新调度。当指定事件
//      处理完成，或者超时时间到，将唤醒当前事件。
//参数: event_id,目标事件id号
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//      超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//返回: CN_SYNC_SUCCESS=同步条件成立返回，
//      CN_SYNC_TIMEOUT=超时返回，
//      EN_KNL_CANT_SCHED=禁止调度时不可执行同步操作
//      EN_KNL_EVENT_FREE,event_id是空闲id
//特注: 本函数不像semp_pend一样，会追溯历史的semp_post。如果调用本函数时，
//      completed已经发生，则只能等到下一次completed才会唤醒。
//----------------------------------------------------------------------------
u32 DJY_WaitEventCompleted(u16 event_id,u32 timeout)
{
    struct EventECB * pl_ecb;
    pl_ecb = &g_tECB_Table[event_id];

    if (event_id == g_ptEventRunning->event_id)
        return CN_EVENT_ID_INVALID;
    if(DJY_QuerySch() == false)  //不能在禁止调度的情况下执行同步操作
        return (u32)EN_KNL_CANT_SCHED;
    if(timeout == 0)
        return (u32)CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if(pl_ecb->previous == (struct EventECB *)&s_ptEventFree)
    {//目标事件控制块是空闲事件控制块
        Int_RestoreAsynSignal();
        return (u32)EN_KNL_EVENT_FREE;
    }
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;

    //以下把pg_event_running加入到目标事件的同步队列中

    g_ptEventRunning->sync_head = &pl_ecb->sync;
    if(pl_ecb->sync != NULL)
    {//被同步事件的同步队列不是空的
        g_ptEventRunning->multi_next = pl_ecb->sync;
        g_ptEventRunning->multi_previous = pl_ecb->sync->multi_previous;
        pl_ecb->sync->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->sync->multi_previous = g_ptEventRunning;
    }else
    {//被同步事件的同步队列是空的
        pl_ecb->sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }

    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVENT_DONE
                                        +CN_STS_SYNC_TIMEOUT;
        __DJY_AddToDelay(timeout);
    }
    else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVENT_DONE;
    }
    Int_RestoreAsynSignal();  //恢复中断会引发重新调度

    //检查从哪里返回，是超时还是同步事件完成。
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//说明同步条件未到，从超时返回。
        return (u32)CN_SYNC_TIMEOUT;
    }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
    {//被同步事件被异常终止，可能没有被正确执行
        return (u32)EN_KNL_EVENT_SYNC_EXIT;
    }else
    {
        return (u32)CN_SYNC_SUCCESS;
    }
}

//----事件类型完成同步----------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件类型的前同步队列中去,然后重新调度。完成
//      同步以目标事件类型的完成次数为同步条件。
//参数: evtt_id,目标事件类型号
//      done_times,完成次数，0表示待最后一条该类型事件完成.
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//      超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//返回: CN_SYNC_SUCCESS=同步条件成立返回，
//      CN_SYNC_TIMEOUT=超时返回，
//      EN_KNL_CANT_SCHED=禁止调度时不可执行同步操作
//      EN_KNL_EVTT_FREE=evtt_id是空闲id
//      本函数必须联系共享文档的相关章节才容易读懂，注释难于解释那么清楚的。
//特注: 本函数不像semp_pend一样，会追溯历史的semp_post。如果调用本函数时，
//      completed已经发生，则只能等到下一次completed才会唤醒。
//----------------------------------------------------------------------------
u32 DJY_WaitEvttCompleted(u16 evtt_id,u16 done_times,u32 timeout)
{
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb;
    u16 evttoffset;
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= CFG_EVENT_TYPE_LIMIT)
        return EN_KNL_EVTTID_LIMIT;
    pl_evtt = &g_tEvttTable[evttoffset];
    if(DJY_QuerySch() == false)  //不能在禁止调度的情况下执行同步操作
        return (u32)EN_KNL_CANT_SCHED;
    if(timeout == 0)
        return (u32)CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if((pl_evtt->property.registered == 0)     //事件类型未注册
        ||(pl_evtt->property.deleting == 1))       //事件类型正在等待注销
    {
        Int_RestoreAsynSignal();
        return (u32)EN_KNL_EVTT_FREE;
    }
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->sync_counter = done_times;

    //以下把pg_event_running加入到目标事件的同步队列中
    g_ptEventRunning->sync_head = &pl_evtt->done_sync;
    if(pl_evtt->done_sync != NULL)
    {//被同步事件类型的同步队列不是空的
        pl_ecb = pl_evtt->done_sync;
        do
        {//本循环找到第一个剩余完成次数不小于新事件的事件.
            if(pl_ecb->sync_counter < done_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != pl_evtt->done_sync);
        //下面把新事件(g_ptEventRunning)插入到队列中
        g_ptEventRunning->multi_next = pl_ecb;
        g_ptEventRunning->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->multi_previous = g_ptEventRunning;
        if(pl_evtt->done_sync->sync_counter > done_times)
            pl_evtt->done_sync = g_ptEventRunning;
    }else
    {//被同步事件类型的同步队列是空的
        pl_evtt->done_sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }

    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_DONE
                                         +CN_STS_SYNC_TIMEOUT;
        __DJY_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_DONE;
    }
    Int_RestoreAsynSignal();  //恢复调度会引发事件切换

    //检查从哪里返回，是超时还是同步事件完成。
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//说明同步条件未到，从超时返回，此时，被同步的事件肯定还没有完成。
        return (u32)CN_SYNC_TIMEOUT;
    }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
    {//被同步事件被异常终止，可能没有被正确执行
        return (u32)EN_KNL_EVENT_SYNC_EXIT;
    }else
    {
        return (u32)CN_SYNC_SUCCESS;
    }
}

//----事件类型弹出同步---------------------------------------------------------
//功能: 把正在运行的事件加入到指定事件类型的弹出同步队列中去,然后重新调度。弹出
//      同步是指以该事件类型的事件弹出若干次为同步条件。
//      pop_times输入作为计算同步的起始次数，同时用于输出同步条件达到时，目标事
//      件类型的弹出次数。
//参数: evtt_id,目标事件类型号
//      base_times,弹出次数条件，目标事件累计弹出*base_times+1作为同步条件，
//          同步条件达到时，返回实际弹出次数。如果给NULL,则从调用时的弹出次数+1
//          做同步条件，不能得到实际弹出次数。
//      timeout，超时设置,单位是微秒，CN_TIMEOUT_FOREVER=无限等待，0则立即按
//      超时返回。非0值将被向上调整为CN_CFG_TICK_US的整数倍
//返回: CN_SYNC_SUCCESS=同步条件成立返回，
//      CN_SYNC_TIMEOUT=超时返回，
//      CN_SYNC_ERROR=出错，
//      本函数必须联系共享文档的相关章节才容易读懂，注释难于解释那么清楚的。
//----------------------------------------------------------------------------
u32 DJY_WaitEvttPop(u16 evtt_id,u32 *base_times, u32 timeout)
{
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb;
    u32 popt;
    u16 evttoffset;


    if(0 == (evtt_id & CN_EVTT_ID_MASK))
    {
        return CN_SYNC_ERROR;
    }
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= CFG_EVENT_TYPE_LIMIT)
        return CN_SYNC_ERROR;
    pl_evtt = &g_tEvttTable[evttoffset];
    //不能在禁止调度的情况下执行同步操作
    if(DJY_QuerySch() == false)
        return CN_SYNC_ERROR;
    if(timeout == 0)
        return CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if((pl_evtt->property.registered == 0)  //事件类型未注册
        ||(pl_evtt->property.deleting == 1))   //事件类型正在等待注销
    {
        Int_RestoreAsynSignal();
        return CN_SYNC_ERROR;
    }

    if(base_times != NULL)
        popt = *base_times - pl_evtt->pop_times +1;
    else
        popt = 1;
    //这个判断很重要，条件成立说明调用本函数时，等待条件已经成立，应按照条件成立
    //返回，不然会没完没了地等。
    if((popt >= 0x80000000) || (popt == 0))
    {
        if(base_times != NULL)
            *base_times = pl_evtt->pop_times;
        Int_RestoreAsynSignal();
        return CN_SYNC_SUCCESS;
    }
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->sync_counter = popt;
    //以下把pg_event_running加入到目标事件的同步队列中
    g_ptEventRunning->sync_head = &pl_evtt->pop_sync;
    if(pl_evtt->pop_sync != NULL)
    {//被同步事件类型的同步队列不是空的
        pl_ecb = pl_evtt->pop_sync;
        do
        {//本循环找到第一个剩余弹出次数不小于新事件的事件.
            if(pl_ecb->sync_counter < g_ptEventRunning->sync_counter)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != pl_evtt->pop_sync);
        //下面把新事件(g_ptEventRunning)插入到队列中
        g_ptEventRunning->multi_next = pl_ecb;
        g_ptEventRunning->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->multi_previous = g_ptEventRunning;
        if(pl_evtt->pop_sync->sync_counter > g_ptEventRunning->sync_counter)
            pl_evtt->pop_sync = g_ptEventRunning;
    }else
    {//被同步事件的同步队列是空的
        pl_evtt->pop_sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_POP
                                         + CN_STS_SYNC_TIMEOUT;
        __DJY_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_POP;
    }

    Int_RestoreAsynSignal();  //恢复中断会引发重新调度
    if(base_times != NULL)
        *base_times = pl_evtt->pop_times;

    //检查从哪里返回，是超时还是同步事件完成。
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//说明同步条件未到，从超时返回，此时，同步条件肯定还没有达成。
        return CN_SYNC_TIMEOUT;
    }else
    {
        return CN_SYNC_SUCCESS;
    }
}

//----弹出事件-----------------------------------------------------------------
//功能：向操作系统报告发生事件,可以带一个参数缓冲区，提供处理事件所需要的附加信
//      息。操作系统接报后,将分配事件控制块，或者把参数加入到已存在事件的参数队
//      列中.
//
//参数：hybrid_id，id，可能是事件类型id，也可能是事件id。
//          如果是事件id，系统会查找该事件对应的事件类型id。
//          如果是关联型事件类型，则事件id和事件类型id是等效的。
//          如果是独立型事件类型，则根据hybrid_id传入的id类型做分别处理:
//              1、事件类型id，将新分配一个事件控制块，处理事件时，将会分配/
//                 创建一个新的线程。
//              2、事件id，则只是把新参数加入事件的参数中。
//      timeout,定义超时时间，如果=0，则弹出事件后，立即返回，何时处理事件将由
//          调度器决定；若!=0，则等待事件处理完成后才返回，并且在pop_result中返
//          回处理结果。单位是us
//      pop_result,事件弹出或处理状态，如果函数返回了合法的事件id，
//          且timeout !=0，则
//              pop_result = CN_SYNC_SUCCESS ，表示事件被处理完成后返回
//              pop_result = CN_SYNC_TIMEOUT ，表示事件未被处理完，超时返回
//              pop_result = CN_STS_EVENT_EXP_EXIT ，事件处理被异常终止
//              如果timeout ==0，则pop_result无意义。
//
//          如果函数返回了 CN_EVENT_ID_INVALID ，则返回具体的出错信息
//      PopPrarm1，PopPrarm2：传递给事件的参数，如果连续弹出关联型事件，事件控
//          制块中总是保存最后一次弹出的参数。
//      prio,事件优先级,对于需要创建新事件的情况，0表示新事件使用默认值(存在事件
//          类型控制块中)。
//          对于无需创建新事件的情况，分两种情况:
//              1、如果没有携带参数，则不作任何处理，不会改变已经进入事件队列的
//                 事件的优先级。
//              2、如果携带参数，prio不高于已有优先级则新参数置入task_low_prio队
//                 列，prio高于已有优先级则新参数置入task_high_prio队列，数越大，
//                 优先级越低。
//返回：若不能获取事件控制块，返回 CN_EVENT_ID_INVALID，否则返回事件id。
//注: 不会因不能获得事件控制块而阻塞。
//-----------------------------------------------------------------------------
u16 DJY_EventPop(   u16  hybrid_id,
                    u32 *pop_result,
                    u32 timeout,    //如果阻塞，定义超时时间，
                    ptu32_t PopPrarm1,
                    ptu32_t PopPrarm2,
                    ufast_t prio)
{
    struct BlackBoxThrowPara  parahead;
    char ExpStr[32];
    struct EventECB *pl_ecb;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb_temp;
    u16 evtt_offset;
    u16 return_result;
    bool_t schbak;          //是否允许调度
    if(hybrid_id >= CN_EVTT_ID_BASE)
    {
        evtt_offset = hybrid_id & (~CN_EVTT_ID_MASK);
        if(evtt_offset >= CFG_EVENT_TYPE_LIMIT)
        {
            DJY_SaveLastError(EN_KNL_EVTTID_LIMIT);
            if(pop_result != NULL)
                *pop_result = (ptu32_t)EN_KNL_EVTTID_LIMIT;
            return CN_EVENT_ID_INVALID;
        }
    }
    else
    {
        if(hybrid_id >= CFG_EVENT_LIMIT)
        {
            DJY_SaveLastError(EN_KNL_EVENTID_LIMIT);
            if(pop_result != NULL)
                *pop_result = (ptu32_t)EN_KNL_EVENTID_LIMIT;
            return CN_EVENT_ID_INVALID;
        }
        else
        {
            evtt_offset = g_tECB_Table[hybrid_id].evtt_id & (~CN_EVTT_ID_MASK);
        }
    }
    schbak = DJY_QuerySch( );
    if((timeout != 0) && (!schbak))
    {   //要求事件处理完才返回，但因不允许调度而无法执行
        DJY_SaveLastError(EN_KNL_CANT_SCHED);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)EN_KNL_CANT_SCHED;
        return CN_EVENT_ID_INVALID;
    }
    pl_evtt =&g_tEvttTable[evtt_offset];
    if((pl_evtt->property.registered == 0)      //类型未登记
        ||(pl_evtt->property.deleting == 1))    //事件类型正在等待注销
    {
        DJY_SaveLastError(EN_KNL_EVTT_UNREGISTER);
        if(pop_result != NULL)
            *pop_result = (u32)EN_KNL_EVTT_UNREGISTER;
        return CN_EVENT_ID_INVALID;
    }else if(prio >= CN_PRIO_SYS_SERVICE)       //非法优先级
    {
        DJY_SaveLastError(EN_KNL_INVALID_PRIO);
        if(pop_result != NULL)
            *pop_result = (u32)EN_KNL_INVALID_PRIO;
        return CN_EVENT_ID_INVALID;
    }

//    malloc_para = para_options && CN_MALLOC_OVER_32;
    Int_SaveAsynSignal();                     //关异步信号(关调度)
    //下面处理事件类型弹出同步，参看Djy_WaitEvttPop的说明
    //弹出同步中的事件，处理方式如下:
    //1、如果是关联型事件，则激活所有sync_counter减至0的事件。
    //2、如果是独立型事件，则要分两种情况:
    //  a、如果hybrid_id是事件id，则不激活同类型的其他事件。
    //  b、如果hybrid_id是事件类型id，则激活所有sync_counter减至0的事件。
    pl_ecb = pl_evtt->pop_sync;
    while(NULL != pl_ecb)
    {
        //sync_counter减量的条件:
        //1、关联型事件，或者:
        //2、独立型事件，hybrid_id是事件id，且等于pl_ecb->event_id，或者:
        //3、独立型事件，且类型和pl_ecb->evtt_id相同
        if( (pl_evtt->property.correlative == EN_CORRELATIVE)
                || (pl_ecb->event_id == hybrid_id)
                || ((evtt_offset|CN_EVTT_ID_MASK) == pl_ecb->evtt_id) )
        {
            //同步条件达成的条件: 1、同步计数器已减至1。
            if(pl_ecb->sync_counter <= 1)
            {
                pl_ecb->sync_head = NULL;
                //指定的超时未到
                if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)
                {
                    __DJY_ResumeDelay(pl_ecb);     //从闹钟队列中移除事件
                }
                pl_ecb->wakeup_from = CN_STS_WAIT_EVTT_POP;
                pl_ecb->event_status = CN_STS_EVENT_READY;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)
                {
                    pl_ecb = NULL;
                    pl_evtt->pop_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous
                                        = pl_ecb_temp->multi_previous;

                    if(pl_ecb_temp == pl_evtt->pop_sync) //if match the head,then change it
                    {
                        pl_evtt->pop_sync = pl_ecb;
                    }
                    else if(pl_ecb == pl_evtt->pop_sync)
                    {
                        pl_ecb = NULL;//which means we loop back,so terminate
                    }
                    else
                    {
                        //continue to loop,do nothing here
                    }
                }
                __DJY_EventReady(pl_ecb_temp);
            }else
            {
                pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
                if(pl_ecb == pl_evtt->pop_sync) //which means it is back now, so terminate the loop
                {
                    pl_ecb = NULL;
                }
            }
        }
        else
        {
            pl_ecb = pl_ecb->multi_next;
            if(pl_ecb == pl_evtt->pop_sync) //which means it is back now, so terminate the loop
            {
                pl_ecb = NULL;
            }
        }
    }
    //需要创建新事件的情况为两者之一:
    //1、事件队列中尚无该类型事件。
    //2、独立型事件且hybrid_id是事件类型id
    if((pl_evtt->property.inuse == 0)
                || ((pl_evtt->property.correlative == EN_INDEPENDENCE)
                    && (hybrid_id & CN_EVTT_ID_MASK)) )
    {
        if(s_ptEventFree==NULL)            //没有空闲的事件控制块
        {
            parahead.DecoderName = NULL;
            parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
            itoa(hybrid_id,ExpStr,16);
            strcat(ExpStr,": 分配不到事件控制块");
            parahead.BlackBoxInfo = (u8*)ExpStr;
            parahead.BlackBoxInfoLen = sizeof(ExpStr);
            parahead.BlackBoxType = CN_BLACKBOX_TYPE_ECB_EXHAUSTED;
            BlackBox_ThrowExp(&parahead);
            DJY_SaveLastError(EN_KNL_ECB_EXHAUSTED);
            if(pop_result != NULL)
                *pop_result = (u32)EN_KNL_ECB_EXHAUSTED;
            return_result = CN_EVENT_ID_INVALID;
            goto end_pop;
//      }else if((pl_evtt->property.correlative == EN_INDEPENDENCE)
//              && ((pl_evtt->vpus)> pl_evtt->vpus_limit))
//      {
//          DJY_SaveLastError(EN_KNL_VPU_OVER);
//          return_result = CN_EVENT_ID_INVALID;
//          if(pop_result != NULL)
//              *pop_result = (u32)EN_KNL_VPU_OVER;
//          return_result = CN_EVENT_ID_INVALID;
//          goto end_pop;
        }else                       //有空闲事件控制块
        {
            pl_ecb = s_ptEventFree;         //从空闲链表中提取一个事件控制块
            s_ptEventFree=s_ptEventFree->next;  //空闲事件控制块数量减1
            pl_evtt->events++;
            //设置新事件的事件控制块
            pl_ecb->next = NULL;
            pl_ecb->previous = NULL;
            pl_ecb->multi_next = NULL;
            pl_ecb->multi_previous = NULL;
            pl_ecb->vm = NULL;
            pl_ecb->param1 = PopPrarm1;
            pl_ecb->param2 = PopPrarm2;
            pl_ecb->sync = NULL;
            pl_ecb->sync_head = NULL;

#if CFG_OS_TINY == false
            pl_ecb->EventStartTime = DJY_GetSysTime();   //事件发生时间
            pl_ecb->consumed_time = 0;
            pl_ecb->consumed_time_second = 0;
            pl_ecb->consumed_time_record = 0;
#endif  //CFG_OS_TINY == false
            pl_ecb->delay_start_tick = 0;
            pl_ecb->delay_end_tick = 0;
            pl_ecb->error_no = 0;
            pl_ecb->wait_mem_size = 0;
            pl_ecb->HeapSize = 0;
            pl_ecb->HeapSizeMax = 0;
            pl_ecb->wakeup_from = CN_STS_EVENT_NORUN;
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->evtt_id = evtt_offset | CN_EVTT_ID_MASK;    //设置事件类型
            pl_ecb->sync_counter = 0;
            pl_ecb->local_memory = 0;
            pl_evtt->property.inuse = 1;
            pl_evtt->correlativeID = pl_ecb->event_id;
//          if(pl_evtt->property.correlative == EN_CORRELATIVE)
//          {
//              pl_evtt->mark_event = pl_ecb;
//          }
//          else
//          {
//
//          }
            if(prio != 0)
            {
                pl_ecb->prio = prio;                    //设置事件优先级,
                pl_ecb->prio_base = pl_ecb->prio;       //设置事件优先级,
            }else
            {
                pl_ecb->prio =pl_evtt->default_prio;    //从事件类型中继承优先级
                pl_ecb->prio_base = pl_ecb->prio;       //设置事件优先级,
            }
            __DJY_EventReady(pl_ecb);
            return_result = pl_ecb->event_id;
        }
    }
    else  //关联型事件，且队列中已经有该类型事件 或 独立型且hybrid_id是事件id
    {
//      if(pl_evtt->property.correlative == EN_INDEPENDENCE)
//      {
        pl_ecb = &g_tECB_Table[pl_evtt->correlativeID];
//      }
//      else
//      {
//          pl_ecb = pl_evtt->mark_event;
//      }
        pl_ecb->param1 = PopPrarm1;     //参数替换
        pl_ecb->param2 = PopPrarm2;
        return_result = pl_ecb->event_id;

    }
    //设定了超时时间，将重新调度
    //running事件将进入新事件的同步队列，事件处理函数自然返回或异常退出才解除阻塞
    pl_evtt->pop_times++;
    if(timeout != 0)
    {
        __DJY_AddRunningToBlock(&pl_ecb->sync, CN_BLOCK_FIFO, timeout, CN_STS_WAIT_EVENT_DONE);
        Int_RestoreAsynSignal();  //恢复中断会引发重新调度

        //注:事件处理结果在djy_task_completed函数中给event_result赋值
        //检查从哪里返回，是超时还是同步事件完成。
        if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
        {//说明同步条件未到，从超时返回。
            if(pop_result != NULL)
                *pop_result = (u32)CN_SYNC_TIMEOUT;
        }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
        {//被同步事件被异常终止，可能没有被正确执行
            if(pop_result != NULL)
                *pop_result = (u32)CN_STS_EVENT_EXP_EXIT;
        }else
        {
            if(pop_result != NULL)
                *pop_result = (u32)CN_SYNC_SUCCESS;
        }
    }
end_pop:
    Int_RestoreAsynSignal();  //恢复中断状态
    return return_result;
}

//----取事件类型弹出次数-------------------------------------------------------
//功能: 取当前某类型事件弹出总次数
//参数: evtt_id，事件类型id
//返回: 事件处理结果。
//-----------------------------------------------------------------------------
u32 DJY_GetEvttPopTimes(u16 evtt_id)
{
    return g_tEvttTable[evtt_id &(~CN_EVTT_ID_MASK)].pop_times;
}

//----取事件处理结果-----------------------------------------------------------
//功能: 一个事件在处理过程中，如果弹出了新事件，并且等待事件处理结果(调用
//      DJY_EventPop 函数时 timeout != 0)。如果调用 DJY_EventPop 时返回了合法的事件
//      id，且不是超时返回，则可以用本函数获取新事件的处理结果。只能取最后一次
//      成功处理的事件结果。
//参数: 无
//返回: 事件处理结果。
//-----------------------------------------------------------------------------
ptu32_t DJY_GetEventResult(void)
{
    return g_ptEventRunning->event_result;
}

//----提取事件参数-------------------------------------------------------------
//功能: 提取处理中事件的参数
//参数: Param1，返回事件参数1，若空则不取
//      Param2，返回事件参数2，若空则不取
//返回: 无
//注: 本函数只能取正在处理中的事件自己的参数
//-----------------------------------------------------------------------------
void DJY_GetEventPara(ptu32_t *Param1,ptu32_t *Param2)
{
    if(Param1 != NULL)
        *Param1 = g_ptEventRunning->param1;
    if(Param2 != NULL)
        *Param2 = g_ptEventRunning->param2;
}

//----设置事件参数-------------------------------------------------------------
//功能: 设置事件参数，本函数非 API ，用户的事件参数应该在弹出事件时作为 DJY_EventPop
//      函数的参数。__SetEventPara 函数只用于特定场合
//参数: Param1，事件参数1，若空则忽略
//      Param2，事件参数2，若空则忽略
//返回: 无
//注: 本函数只能设置正在处理中的事件自己的参数
//-----------------------------------------------------------------------------
void __SetEventPara(ptu32_t *Param1,ptu32_t *Param2)
{
    if(Param1 != NULL)
        g_ptEventRunning->param1 = *Param1;
    if(Param2 != NULL)
        g_ptEventRunning->param2 = *Param2;
}

//----提取事件的用户数据---------------------------------------------------------
//功能: 提取处理中事件控制块中的私有数据
//参数: 无
//返回: 私有数据
//注: 本函数只能取正在处理中的事件
//-----------------------------------------------------------------------------
ptu32_t DJY_GetEventUserdata(void)
{
    return g_ptEventRunning->userdata;
}

//----设置事件的用户数据---------------------------------------------------------
//功能: 设置处理中事件控制块中的私有数据
//参数: userdata，待写入的数据
//返回: 无
//注: 本函数只能设置正在处理中的事件
//-----------------------------------------------------------------------------
void DJY_SetEventUserdata(ptu32_t userdata)
{
    g_ptEventRunning->userdata = userdata;
}

//----取自身的事件类型id-------------------------------------------------------
//功能: 由应用程序调用，取正在处理的事件的事件类型id
//参数: 无
//返回: 事件类型id，如果当前运行在异步信号中断中，则返回 CN_EVTT_ID_ASYN
//-----------------------------------------------------------------------------
u16 DJY_GetMyEvttId(void)
{
    if(Int_GetRunLevel() >0)
    {
        return CN_EVTT_ID_ASYN;
    }
    else
    {
        return g_ptEventRunning->evtt_id;
    }
}

//----取自身的事件id-------------------------------------------------------
//功能: 由应用程序调用，取正在处理的事件的事件id
//参数: 无
//返回: 事件id，如果当前运行在异步信号中断中，则返回 CN_EVENT_ID_ASYN
//-----------------------------------------------------------------------------
u16 DJY_GetMyEventId(void)
{
    if(Int_GetRunLevel() >0)
    {
        return CN_EVENT_ID_ASYN;
    }
    else
    {
        return g_ptEventRunning->event_id;
    }
}

//----取自身的事件id-------------------------------------------------------
//功能: 由应用程序调用，取正在处理的事件的事件id
//参数: 无
//返回: 事件id，如果当前运行在异步信号中断中，则返回 CN_EVENT_ID_ASYN
//注意：修改成DJY_GetMyEventId函数后和C库有冲突，copy了一份，改了C库之后删掉
//-----------------------------------------------------------------------------
u16 Djy_MyEventId(void)
{
    if(Int_GetRunLevel() >0)
    {
        return CN_EVENT_ID_ASYN;
    }
    else
    {
        return g_ptEventRunning->event_id;
    }
}

//----启动操作系统--------------------------------------------------------------
//功能: 初始化时钟嘀嗒，复位异步信号使能状态，选出最高优先级事件，调用
//      __asm_turnto_context把上下文切换到这个事件
//参数: 无
//返回: 无
//_____________________________________________________________________________
void __DJY_StartOs(void)
{
    //本句为容错性质，以防用户模块初始化过程中没有成对调用异步信号使能与禁止函数
    __Int_ResetAsynSignal();
    __DJY_SelectEventToRun();
    g_ptEventRunning = g_ptEventReady;
    g_bScheduleEnable = true;
    g_bMultiEventStarted = true;
    g_u32OsRunMode = CN_RUNMODE_SI;
    Int_HalfEnableAsynSignal( );
    __asm_start_thread(g_ptEventRunning->vm);
}

#define CN_KEEP         0   //线程保留不删除
#define CN_DELETE       1   //线程应该被删除
#define CN_DELETED      2   //线程已经被删除

//----激活事件的同步队列---------------------------------------------------
//功能: 事件处理结束时，激活该事件同步队列中的所有事件，返回
//      给同步中的事件的"执行结果"被置为0，被同步事件异常返回位被置位
//参数: para，参数队列的队列头，参数构成一个双向链表。
//返回: 无
//-----------------------------------------------------------------------------
void __DJY_ActiveEventSyncExp(struct EventECB *sync)
{
    struct EventECB *event_temp;
    if(sync == NULL)
        return ;

    sync->multi_previous->multi_next = NULL;
    do{
        sync->event_result = 0;
        sync->sync_head = NULL;
        //是否在超时队列中
        if(sync->event_status & CN_STS_SYNC_TIMEOUT)
        {
            __DJY_ResumeDelay(sync);                   //结束超时等待
        }
        sync->event_status = CN_STS_EVENT_READY;        //取消"同步中"状态
        sync->wakeup_from = CN_STS_EVENT_EXP_EXIT;      //置位异常结束标志
        event_temp = sync->multi_next;
        __DJY_EventReady(sync);           //把事件加入到就绪队列中
        sync = event_temp;
    }while(sync != NULL);
    return;
}

//----异常激活事件参数的同步队列-----------------------------------------------
//功能: 事件异常结束时，激活一个事件所有参数的同步队列中的所有事件，并释放参数控
//      制块.返回给同步中的事件的"执行结果"被置为0，被同步事件异常返回位被置位
//参数: para，参数队列的队列头，参数构成一个双向链表。
//返回: 无
//-----------------------------------------------------------------------------
//void __Djy_ActiveEventParaSyncExp(struct ParaPCB *event_para)
//{
//    struct ParaPCB *para_temp;
//    struct EventECB *pl_ecb,*event_temp;
//    if(event_para == NULL)
//        return ;
//
//    event_para->previous->next = NULL;
//    do{
//        pl_ecb = event_para->sync;
//        if(pl_ecb != NULL)
//        {
//            pl_ecb->multi_previous->multi_next = NULL;
//            do{
//                pl_ecb->event_result = 0;
//                pl_ecb->sync_head = NULL;
//                //是否在超时队列中
//                if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)
//                {
//                    __DJY_ResumeDelay(pl_ecb);               //结束超时等待
//                }
//                pl_ecb->event_status = CN_STS_EVENT_READY;    //取消"同步中"状态
//                pl_ecb->wakeup_from = CN_STS_EVENT_EXP_EXIT;  //置位异常结束标志
//                event_temp = pl_ecb->multi_next;
//                __DJY_EventReady(pl_ecb);           //把事件加入到就绪队列中
//                pl_ecb = event_temp;
//            }while(pl_ecb != NULL);
//        }
//
//        if(event_para->dynamic_mem == true)
//        {
//            free(event_para->event_para);
//        }
//        para_temp = event_para;
//        event_para = event_para->next;                  //取队列中下一个事件
//        para_temp->sync = NULL;
//        para_temp->next = s_ptParaFree;     //释放参数控制块
//        s_ptParaFree = para_temp;
//    }while(event_para != NULL);
//    return;
//}

//----从同步队列中移除该ECB-----------------------------------------------
//功能: 此函数不做任何的检查，内部调用，确认此ecb在sync队列中
//返回: 无
//      该同步队列是多功能指针的双向循环队列
//-----------------------------------------------------------------------------
void __DJY_CutEcbFromSync(struct EventECB  *event)
{
    struct EventECB  **syn_head;

    syn_head = event->sync_head;
    if(NULL == syn_head)
    {
        return;
    }
    if(event->multi_next == event)
    {//是最后一个事件
        *syn_head  = NULL;
    }else
    {
        if(*syn_head == event)  //被移除的是队列头
            *syn_head = event->multi_next;
        event->multi_next->multi_previous = event->multi_previous;
        event->multi_previous->multi_next = event->multi_next;
        event->multi_next = NULL;
        event->multi_previous = NULL;
    }
    event->sync_head = NULL;
}

//----从同步队列中移除该ECB-----------------------------------------------
//功能: 将event 从ECB对应的EVTT的marked队列中删除，应该确认event在marked队列中
//返回: 无
//      该队列是前后指针的双向循环队列,顺序是按照优先级高低顺序
//      仅仅是功能性代码，不做安全检查
//-----------------------------------------------------------------------------
//void __Djy_CutEventFromEvttMarked(struct EventECB *event)
//{
//    struct EventType  *pl_evtt;
//
//    pl_evtt =& g_tEvttTable[event->evtt_id &(~CN_EVTT_ID_MASK)];
//    if(event == event->previous)//maked队列中只有一个
//    {
//        pl_evtt->mark_event = NULL;
//    }
//    else//从marked队里中移除
//    {
//        if(event == pl_evtt->mark_event)//队列头，其实一般的都是从头移走
//        {
//            pl_evtt->mark_event = event->next;
//        }
//        event->previous->next = event->next;
//        event->next->previous = event->previous;
//    }
//}

//----事件逸出-----------------------------------------------------------------
//功能: 事件处理函数异常返回,一般在看门狗等监视机制监察到系统错误时，做善后处理，
//      或删除事件，并结束线程运行，或复位线程重新开始运行并把线程重新初始化。
//参数: event_ECB，目标事件
//      exit_code，出错码
//      action，出错后的动作
//返回: 本函数不返回
//todo: 未完成
//-----------------------------------------------------------------------------
void __DJY_EventExit(struct EventECB *event, u32 exit_code,enum EN_BlackBoxAction action)
{
    struct ThreadVm *next_vm,*temp;
    struct EventECB *pl_ecb;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb_temp;
    struct BlackBoxThrowPara  parahead;
    char ExpStr[32];
    ucpu_t  vm_final = CN_DELETE;

    DJY_SaveLastError(exit_code);
    parahead.DecoderName = NULL;
    parahead.BlackBoxAction = action;
    itoa(event->event_id,ExpStr,16);
    strcat(ExpStr,"事件处理意外结束");
    parahead.BlackBoxInfo = (u8*)ExpStr;
    parahead.BlackBoxInfoLen = sizeof(ExpStr);
    parahead.BlackBoxType = CN_BLACKBOX_TYPE_EVENT_EXIT;
    BlackBox_ThrowExp(&parahead);
    //此处不用int_save_asyn_signal函数，可以在应用程序有bug，没有成对调用
    //int_save_asyn_signal和int_restore_asyn_signal的情况下，确保错误到此为止。
    __Int_ResetAsynSignal();  //直到__vm_engine函数才再次打开.

    //激活事件同步队列中的所有事件
    __DJY_ActiveEventSyncExp(g_ptEventRunning->sync);

//    if(g_ptEventRunning->para_current != NULL)
//        __DJY_ActiveEventSyncExp(g_ptEventRunning->para_current->sync);
    //以下激活参数的同步队列中的事件,并释放参数队列中的全部参数
//  __Djy_ActiveEventParaSyncExp(g_ptEventRunning->para_high_prio);
//  __Djy_ActiveEventParaSyncExp(g_ptEventRunning->para_low_prio);

#if (CFG_DYNAMIC_MEM == true)
    if(g_ptEventRunning->local_memory != 0)
    {
        __Heap_CleanUp(g_ptEventRunning->event_id);    //
    }
#endif
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous
                    = (struct EventECB*)&s_ptEventFree;//表示本控制块空闲
    g_ptEventRunning->next = s_ptEventFree;     //pg_event_free是单向非循环队列
    s_ptEventFree = g_ptEventRunning;
    s_ptEventFree->event_id = s_ptEventFree - g_tECB_Table;   //容错用

    //下面处理完成同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_evtt =&g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)];
    g_ptEventRunning->evtt_id = CN_EVTT_ID_INVALID;
    pl_evtt->events--;
    pl_ecb = pl_evtt->done_sync;
    while(pl_ecb != NULL)
    {
        //链表中的事件都是要么没有指定超时，要么时限未到，其他情况不会在此链表
        //中留下痕迹，evtt_done_sync位也已经清除
        //同步条件达成的条件: 1、同步计数器已减至1。
        //2、同步计数器为0且本类型最后一条事件已经处理完
        if((pl_ecb->sync_counter == 1)
         ||((pl_ecb->sync_counter == 0) && (pl_evtt->vpus == 1)) )
        {
            pl_ecb->sync_head = NULL;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT) //指定的超时未到
            {
                __DJY_ResumeDelay(pl_ecb);       //从闹钟队列中移除事件
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVTT_DONE;
            pl_ecb_temp = pl_ecb;
            if(pl_ecb_temp == pl_ecb_temp->multi_next)    //是最后一个事件
            {
                pl_ecb = NULL;
                pl_evtt->done_sync = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;
                pl_ecb_temp->multi_previous->multi_next
                                    = pl_ecb_temp->multi_next;
                pl_ecb_temp->multi_next->multi_previous
                                    = pl_ecb_temp->multi_previous;
                if(pl_ecb_temp == pl_evtt->done_sync) //if match the head,then change it
                {
                    pl_evtt->done_sync = pl_ecb;
                }
                else if(pl_ecb == pl_evtt->done_sync)
                {
                    pl_ecb = NULL;//which means we loop back,so terminate
                }
                else
                {
                    //continue to loop,do nothing here
                }

            }
            __DJY_EventReady(pl_ecb_temp);
        }else
        {
            if(pl_ecb->sync_counter != 0)
                pl_ecb->sync_counter--;
            pl_ecb = pl_ecb->multi_next;
            if(pl_ecb == pl_evtt->done_sync)
            {
                pl_ecb = NULL;//which means we loop back,so terminate
            }
        }
    }

    //线程和事件控制块的处理方式
    //1、若线程数量小于等于vpus_res，则保留.
    //2、其他情况，销毁线程
    //3、事件控制块则直接收回

    if(pl_evtt->property.correlative == EN_CORRELATIVE)      //关联型事件
    {
        pl_evtt->property.inuse = 0;
        if(pl_evtt->default_prio < 0x80)
        {//运行模式为si或dlsp，或者该事件类型拥有紧急优先级时，
         //须保留线程
            vm_final = CN_KEEP;
        }else
        {
            vm_final = CN_DELETE;
        }
    }else   //独立型事件
    {
        //有未得到线程的事件，保留之
        if(pl_evtt->events >= pl_evtt->vpus)
        {
            vm_final = CN_KEEP;
        }else   //没有未得到线程的事件，再看是否需保留
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)//该类型事件拥有的线程数已经低于
                                                    //或等于最低保留量，保留之
            {
                vm_final = CN_KEEP;
            }else                                   //可能删除线程
            {
                if(pl_evtt->vpus == 1)   //这是最后一个事件
                {
                    pl_evtt->property.inuse = 0;
                    if(pl_evtt->default_prio<0x80)
                    {
                 //运行模式为si或dlsp，或该事件类型拥有紧急优先级时，须保留线程
                        vm_final = CN_KEEP;
                    }else
                    {
                        vm_final = CN_DELETE;
                    }
                }else
                {
                    vm_final = CN_DELETE;
                }
            }
        }
    }

//看事件类型控制块是否要删除，用户注销一个事件类型，如果该事件类型正在使用，则不
//会立即注销，只是不能弹出新事件，要等属于该事件类型的所有事件处理完才能注销。
    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //回收事件类型控制块，只需把registered属性清零。
        pl_evtt->property.registered = 0;
#if CFG_OS_TINY == false
        pl_evtt->evtt_name[0] = '\0';   //清空事件类型名
#endif  //CFG_OS_TINY == false
        while(next_vm != NULL)          //释放该事件类型拥有的空闲线程
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
        vm_final = CN_DELETED;          //线程已经被删除
    }

    __DJY_SelectEventToRun();
    if(vm_final == CN_DELETE)                   //删除线程
    {
        free((void*)g_ptEventRunning->vm);    //删除线程
        pl_evtt->vpus--;
        g_ptEventRunning = g_ptEventReady;
        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DJY_GetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }else if(vm_final == CN_KEEP)    //保留线程
    {
        {
            g_ptEventRunning->vm->next = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = g_ptEventRunning->vm;
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);

            g_ptEventRunning=g_ptEventReady;
            g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DJY_GetSysTime();
            pl_ecb->vm->stack_used = pl_ecb->vm->stack_top;//复原已用指针
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
    }else               //线程已经被删除
    {
//        pl_ecb = g_ptEventRunning;
        g_ptEventRunning = g_ptEventReady;
        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DJY_GetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }
}

//----事件处理完成----------------------------------------------------------
//功能：通知系统，事件已经处理完成，但事件处理函数仍将继续运行，等待处理下一条事件,将激
//      活正在等待本事件完成的事件。常见使用方法：
//      A事件调用Djy_WaitEvttCompleted等待B类型事件完成n次，则B类型事件调用n次本函数后，
//          将激活A事件
//      A事件调用Djy_WaitEventCompleted等待B事件完成，则B事件调用本函数，将激活A事件
//      A事件调用Djy_EventPop弹出B事件,且timeout != 0,则B事件调用本函数，将激活A事件
//参数：result，事件处理结果，这个结果将返回给弹出该事件的事件(如果设定了同步)
//返回：无
//-----------------------------------------------------------------------------
void DJY_EventComplete(ptu32_t result)
{
    struct EventECB *pl_ecb,*event_temp;
    Int_SaveAsynSignal();
    pl_ecb = g_ptEventRunning->sync;     //取同步队列头
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //是否在超时队列中
            {
                __DJY_ResumeDelay(pl_ecb);                 //结束超时等待
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVENT_DONE;
            event_temp = pl_ecb->multi_next;
            __DJY_EventReady(pl_ecb);           //把事件加入到就绪队列中
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }
    g_ptEventRunning->sync = NULL;
    Int_RestoreAsynSignal();

}

//----事件处理终结-------------------------------------------------------------
//功能：事件处理函数返回,由系统调用本函数，将完成清理工作，用户不应该调用本函数
//      1.如果事件同步队列非空，把同步事件放到ready表。
//      2.处理事件类型完成同步队列
//      3.如果未释放的动态分配内存，释放之。
//      4.把事件控制块各分量设置到初始态,并放入free表。
//  如果是线程需保留，则:
//      5.复位线程。
//      6.不保存上下文，直接切换到新的ready表头
//  如果线程无需保留
//      5.释放线程内存。
//      6.不保存上下文，直接切换到新的ready表头
//参数：result，事件处理结果，这个结果将返回给弹出该事件的事件(如果设定了同步)
//返回：无
//备注: 调用本函数的必定是running事件,在running事件上下文中执行，不可以调用
//      __asm_reset_thread函数。
//-----------------------------------------------------------------------------
void __DJY_EventFinal(ptu32_t result)
{
    struct ThreadVm *next_vm,*temp;
    struct EventECB *pl_ecb,*event_temp;
    struct EventType   *pl_evtt;
    struct EventECB *pl_ecb_temp;
    ucpu_t  vm_final = CN_DELETE;

    pl_evtt =&g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)];
    __Int_ResetAsynSignal();  //直到__vm_engine函数才再次打开.
    //下面处理同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_ecb = g_ptEventRunning->sync;     //取同步队列头
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //是否在超时队列中
            {
                __DJY_ResumeDelay(pl_ecb);                 //结束超时等待
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVENT_DONE;
            event_temp = pl_ecb->multi_next;
            __DJY_EventReady(pl_ecb);           //把事件加入到就绪队列中
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }

    //下面处理完成同步队列，必须联系共享文档的相关章节才容易读懂，注释难于解释
    //那么清楚的。
    pl_ecb = pl_evtt->done_sync;
    while(pl_ecb != NULL)
    {
        //链表中的事件都是要么没有指定超时，要么时限未到，其他情况不会在此链表
        //中留下痕迹，evtt_done_sync位也已经清除
        //同步条件达成的条件: 1、同步计数器已减至1。
        //2、同步计数器为0且本类型最后一条事件已经处理完
        if((pl_ecb->sync_counter == 1)
         ||((pl_ecb->sync_counter == 0) && (pl_evtt->vpus == 1)) )
        {
            pl_ecb->sync_head = NULL;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT) //指定的超时未到
            {
                __DJY_ResumeDelay(pl_ecb);       //从闹钟队列中移除事件
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVTT_DONE;
            pl_ecb_temp = pl_ecb;
            if(pl_ecb_temp == pl_ecb_temp->multi_next)    //是最后一个事件
            {
                pl_ecb = NULL;
                pl_evtt->done_sync = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;
                pl_ecb_temp->multi_previous->multi_next
                                    = pl_ecb_temp->multi_next;
                pl_ecb_temp->multi_next->multi_previous
                                    = pl_ecb_temp->multi_previous;
                if(pl_ecb_temp == pl_evtt->done_sync) //if match the head,then change it
                {
                    pl_evtt->done_sync = pl_ecb;
                }
                else if(pl_ecb == pl_evtt->done_sync)
                {
                    pl_ecb = NULL;//which means we loop back,so terminate
                }
                else
                {
                    //continue to loop,do nothing here
                }

            }
            __DJY_EventReady(pl_ecb_temp);
        }else
        {
            if(pl_ecb->sync_counter != 0)
                pl_ecb->sync_counter--;
            pl_ecb = pl_ecb->multi_next;
            if(pl_ecb == pl_evtt->done_sync)
            {
                pl_ecb = NULL;//which means we loop back,so terminate
            }
        }
    }
    //以下看事件的线程如何处理。
#if (CFG_DYNAMIC_MEM == true)
    if(g_ptEventRunning->local_memory != 0)
    {
        __Heap_CleanUp(g_ptEventRunning->event_id);//强行清除事件运行中申请的内存
    }
#endif
    __DJY_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous
                    = (struct EventECB*)&s_ptEventFree;//表示本控制块空闲
    g_ptEventRunning->next = s_ptEventFree;     //pg_event_free是单向非循环队列
    g_ptEventRunning->evtt_id = CN_EVTT_ID_INVALID;     //todo
    s_ptEventFree = g_ptEventRunning;
    s_ptEventFree->event_id = s_ptEventFree - g_tECB_Table;   //容错用

    pl_evtt->events--;

    if(pl_evtt->property.correlative == EN_CORRELATIVE)      //关联型事件
    {
        pl_evtt->property.inuse = 0;
        if(pl_evtt->default_prio<0x80)
        {//运行模式为si或dlsp，或者该事件类型拥有紧急优先级时，
         //须保留线程
            vm_final = CN_KEEP;
        }else
        {
            vm_final = CN_DELETE;
        }
    }else   //独立型事件
    {
        //有未得到线程的事件，保留之
        if(pl_evtt->events >= pl_evtt->vpus)
        {
            vm_final = CN_KEEP;
        }else   //没有未得到线程的事件，再看是否需保留
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)    //该类型事件拥有的线程数已经低于
                                                    //或等于最低保留量，保留之
            {
                vm_final = CN_KEEP;
            }else                                   //可能删除线程
            {
                if(pl_evtt->vpus == 1)              //这是最后一个事件
                {
                    pl_evtt->property.inuse = 0;
                    if(pl_evtt->default_prio<0x80)
                    {
                 //运行模式为si或dlsp，或该事件类型拥有紧急优先级时，须保留线程
                        vm_final = CN_KEEP;
                    }else
                    {
                        vm_final = CN_DELETE;
                    }
                }else
                {
                    vm_final = CN_DELETE;
                }
            }
        }
    }

    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //回收事件类型控制块，只需把registered属性清零。
        pl_evtt->property.registered = 0;
#if CFG_OS_TINY == false
        pl_evtt->evtt_name[0] = '\0';   //清空事件类型名
#endif  //CFG_OS_TINY == false
        while(next_vm != NULL)          //释放该事件类型拥有的空闲线程
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
        vm_final = CN_DELETED;
    }

    __DJY_SelectEventToRun();
    if(vm_final == CN_DELETE)      //删除线程
    {
        free((void*)g_ptEventRunning->vm);    //删除线程
        pl_evtt->vpus--;
        g_ptEventRunning = g_ptEventReady;
        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DJY_GetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }else if(vm_final == CN_KEEP)    //保留线程,和exit一样
    {
        {
            g_ptEventRunning->vm->next = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = g_ptEventRunning->vm;
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);
            g_ptEventRunning = g_ptEventReady;
            g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DJY_GetSysTime();
            pl_ecb->vm->stack_used = pl_ecb->vm->stack_top;//复原已用指针
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
    }else               //线程已经被删除
    {
//        pl_ecb = g_ptEventRunning;
        g_ptEventRunning = g_ptEventReady;
        g_tEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DJY_GetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }
}

//----查询唤醒原因-------------------------------------------------------------
//功能: 查询正在执行的事件被执行的原因.
//参数: 无
//返回: 唤醒原因,见头文件union event_status定义处注释.
//-----------------------------------------------------------------------------
u32 DJY_WakeUpFrom(void)
{
    return g_ptEventRunning->wakeup_from;
}

//----获取事件信息-------------------------------------------------------------
//功能: 根据事件号获取事件信息
//参数: id,事件id; struct EventInfo *info,存储事件信息，输出参数
//返回: true成功，false失败
//-----------------------------------------------------------------------------
bool_t DJY_GetEventInfo(u16 id, struct EventInfo *info)
{
    struct EventECB *event;
    if(id >=CFG_EVENT_LIMIT)
    {
        return false;
    }
    else
    {
        event = &g_tECB_Table[id];
        info->error_no = event->error_no;
#if CFG_OS_TINY == false
        info->consumed_time = event->consumed_time;
        info->EventStartTime = event->EventStartTime;
        info->consumed_time_second = event->consumed_time_second;
        info->EvttName = g_tEvttTable[event->evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
        info->StackTop = event->vm->stack_top;
        info->StackBottom = (u32*)(&(event->vm[1]));

        info->Stack = event->vm->stack;
        info->StackUsed = event->vm->stack_used;
        info->StackSize = event->vm->stack_size;
#endif  //CFG_OS_TINY == false
        return true;
    }
}
//----获取事件类型名字-----------------------------------------------------------
//功能: 根据事件类型号获取事件类型名字
//参数: evtt_id,事件类型ID,
//      dest,存储空间，
//      len,最大存储长度
//返回: true成功，false失败
//-----------------------------------------------------------------------------
bool_t DJY_GetEvttName(u16 evtt_id, char *dest, u32 len)
{
#if CFG_OS_TINY == false
    struct EventType *pl_evtt;
    u16 evttoffset;
    if((NULL == dest)||(0 ==len))
    {
        return false;
    }
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= CFG_EVENT_TYPE_LIMIT)
        return false;
    pl_evtt = &g_tEvttTable[evttoffset];

    if(NULL == pl_evtt->evtt_name)
    {
         dest[0] = '\0';
    }
    else
    {
        strncpy(dest, pl_evtt->evtt_name, len);
    }

    return true;
#else
    dest[0] = '\0';
    return false;
#endif  //CFG_OS_TINY == false
}
//----线程引擎---------------------------------------------------------------
//功能: 启动线程,执行线程入口函数,事件完成后执行清理工作
//参数: thread_routine 处理该事件类型的函数指针.
//返回: 无
//-----------------------------------------------------------------------------
void __DJY_VmEngine(ptu32_t (*thread_routine)(void))
{
    __DJY_EventFinal( thread_routine() );
}

//----取IDLE事件控制块---------------------------------------------------------
//功能：取IDLE事件控制块的指针
//参数：无
//返回：IDLE事件控制块指针。
//-----------------------------------------------------------------------------
struct EventECB *__DJY_GetIdle(void)
{
    return &g_tECB_Table[0];
}
//----系统服务-----------------------------------------------------------------
//功能: 操作系统的系统服务功能,提供调试,统计等服务.
//参数: 无
//返回: 永不返回.
//惨痛教训:这是一个不允许阻塞的函数，深夜3点调出来的教训。
//-----------------------------------------------------------------------------
void CleanWakeupEvent(void);
//u64 buff[256] = {0};
u32 LP_EntryLowPower(struct ThreadVm *vm,u32 pend_ticks);
ptu32_t __DJY_Service(void)
{
    u32 loop;
//    u8 i = 0;
    u8 level;
    u64 now_tick;
    u64 int_tick;
    u32 AllowPendTicks;     //允许休眠的时间，从上一次tick中断起计
    atom_low_t  atom_bak;
    while(1)
    {
        //注：改成tickless模式后，因没有tick中断，不再需要判断，每次运行直接跑栈检查
        level=LP_GetSleepLevel();
        if(level==CN_SLEEP_NORMAL)
        {
          for(loop = 0; loop<CFG_EVENT_LIMIT; loop++)
          {
                __DJY_CheckStack(loop);
          }
        }
        CleanWakeupEvent();
        atom_bak = Int_LowAtomStart();
        now_tick = __DJY_GetSysTick();
        if(g_ptEventDelay != NULL)
            int_tick = g_ptEventDelay->delay_end_tick;
        else
            int_tick = CN_LIMIT_UINT64;

        AllowPendTicks = (u32)(int_tick - now_tick);   //计算允许休眠的ticks数，但实际休眠多少时间，由BSP决定
        if(AllowPendTicks > CN_LIMIT_UINT32)
            AllowPendTicks = CN_LIMIT_UINT32;
//            buff[i++] = AllowPendTicks;
//            buff[i] = __DjyGetSysTime();
        if(AllowPendTicks != 0)
            LP_EntryLowPower(g_ptEventRunning->vm,AllowPendTicks); //进入低功耗状态
//            buff[i] = __DjyGetSysTime() - buff[i];
//            i++;
        Int_LowAtomEnd(atom_bak);
    }
    return 0;//消除编译警告
}


//----api启动函数--------------------------------------------------------------
//功能: 根据api号调用相应的api函数.
//参数: api_no,api号
//返回: mp模式才用，暂且放在这里
//-----------------------------------------------------------------------------
void DJY_ApiStart(u32 api_no)
{
    switch(api_no)
    {
        default :break;
    }
    return;
}
//空函数,未初始化的函数指针指向.
void DJY_NullFunc(void)
{
}

