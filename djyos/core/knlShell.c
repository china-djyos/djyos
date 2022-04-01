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
//版本：V1.0.0
//文件描述:内核的shell命令。
//其他说明:
//修订历史:
//1. 日期: 2016-04-26
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "stdint.h"
#include "board-config.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "object.h"
#include "lock.h"
#include "int.h"
#include "systime.h"
#include "djyos.h"
#include "shell.h"
#include "wdt_soft.h"
#include "blackbox.h"
#include "dbug.h"
#include "component_config_core.h"

extern struct EventECB g_tECB_Table[];
extern struct EventType g_tEvttTable[];
extern struct EventECB  *s_ptEventFree; //空闲链表头,不排序

//void __M_ShowHeap(void);
//void __M_ShowHeapSpy(void);
bool_t event(char *param);
bool_t evtt(char *param);
bool_t stack(char *param);
bool_t spyk(u16 pl_ecb);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// ============================================================================
// 功能：显示事件列表
// 参数：无
// 返回：
// 备注：
// ============================================================================
bool_t event(char *param)
{
    u16 pl_ecb;
    u32 time1,MemSize,StackSize,heapsize=0,maxsize=0;
    char *name;
    bool_t native = false;

    MemSize = 0;
    printf("事件号  类型号  优先级 CPU  栈尺寸   动态内存 最大内存   类型名：总时间(us)");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_time_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                StackSize = 0;
            }
            else
            {
                StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            }

            printf("\r\n%05d   %05d   %03d    %02d%%  %08x %08x %08x %s : %lld",\
                   pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK),\
                                    g_tECB_Table[pl_ecb].prio,time1,\
                                    StackSize, g_tECB_Table[pl_ecb].HeapSize,
                                    g_tECB_Table[pl_ecb].HeapSizeMax, name,
                                    g_tECB_Table[pl_ecb].consumed_time);
            heapsize += g_tECB_Table[pl_ecb].HeapSize;
            maxsize += g_tECB_Table[pl_ecb].HeapSizeMax;
            MemSize += StackSize;
            if(g_tECB_Table[pl_ecb].HeapSize > 0x80000000)
                native = true;
        }
        else
        {
        }
    }

    printf("\r\n所有事件栈尺寸总计:         %08x %08x %08x", MemSize,heapsize,maxsize);
    printf("\r\n允许的事件总数:             %d", CFG_EVENT_LIMIT);
    if(native == true)
        printf("\r\n温馨提示：个别事件使用的动态内存统计出现非常大的值，请不要惊慌，说明您的代码中存在"
               "\r\n非对称内存分配与释放的情况，即A事件分配的内存，在B事件释放");
    return true;
}
bool_t eventk(char *param)
{
    u16 pl_ecb,busy = CN_EVENT_ID_INVALID;
    u32 time1,MemSize,StackSize,heapsize=0,maxsize=0;
    char *name;
    bool_t native = false;

    MemSize = 0;
    printk("事件号  类型号  优先级 CPU  栈尺寸   动态内存 最大内存   类型名：总时间(us)");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_time_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(time1 >50)
                busy = pl_ecb;
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                StackSize = 0;
            }
            else
            {
                StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            }

            printk("\r\n%05d   %05d   %03d    %02d%%  %08x %08x %08x %s : %lld",\
                   pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK),\
                    g_tECB_Table[pl_ecb].prio,time1,\
                    StackSize, g_tECB_Table[pl_ecb].HeapSize,
                    g_tECB_Table[pl_ecb].HeapSizeMax, name,
                    g_tECB_Table[pl_ecb].consumed_time);
            heapsize += g_tECB_Table[pl_ecb].HeapSize;
            maxsize += g_tECB_Table[pl_ecb].HeapSizeMax;
            MemSize += StackSize;
            if(g_tECB_Table[pl_ecb].HeapSize > 0x80000000)
                native = true;
        }
        else
        {
        }
    }

    printk("\r\n所有事件栈尺寸总计:         %08x %08x %08x", MemSize,heapsize,maxsize);
    printk("\r\n允许的事件总数:             %d", CFG_EVENT_LIMIT);
    if(native == true)
        printk("\r\n温馨提示：个别事件使用的动态内存统计出现非常大的值，请不要惊慌，说明您的代码中存在"
               "\r\n非对称内存分配与释放的情况，即A事件分配的内存，在B事件释放");
    if(busy == CN_EVENT_ID_INVALID)
        printk("当前没有事件占用CPU超过50%%");
    else
        spyk(busy);
    return true;
}

// ============================================================================
// 功能：显示事件类型列表
// 参数：无
// 返回：
// 备注：
// ============================================================================
//bool_t Sh_ShowEvtt(char *param)
bool_t evtt(char *param)
{
    u16 pl_ecb;
    u32 MemSize;
    char *name;

    MemSize = 0;
    printf("类型号  优先级 处理函数  栈需求   名字\r\n");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_TYPE_LIMIT; pl_ecb++)
    {
        if(g_tEvttTable[pl_ecb].property.registered ==1)
        {
#if CFG_OS_TINY == false
            name = g_tEvttTable[pl_ecb].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == 0
            MemSize += g_tEvttTable[pl_ecb].stack_size;
            printf("%05d   %03d    %08x  %08x %s",pl_ecb,\
                    g_tEvttTable[pl_ecb].default_prio,\
                    (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,\
                    g_tEvttTable[pl_ecb].stack_size,\
                    name);
//            printf("knlshell","%03d    ",g_tEvttTable[pl_ecb].default_prio);
//            printf("knlshell","%08x  %08x ",
//                        (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,
//                        g_tEvttTable[pl_ecb].stack_size);
//            printf("knlshell","%s",name);
            printf("\r\n");
        }
        else
        {
//          printf("%05d   空闲",pl_ecb);
        }
    }

    printf("所有类型栈需求总计:      %08x",MemSize);
    printf("\r\n允许的事件类型总数:      %d",CFG_EVENT_TYPE_LIMIT);
    return (TRUE);
}

// ============================================================================
// 功能：显示系统中所有已经分配线程的事件的栈信息
// 参数：无
// 返回：
// 备注：
// ============================================================================
//bool_t Sh_ShowStack(char *param)
bool_t stack(char *param)
{
    u32 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printf("\r\n事件号 线程   栈底     栈指针   栈尺寸   剩余量   类型名");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {

        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            printf("\n\r");
            printf("%05d  ",pl_ecb);
            if(g_tECB_Table[pl_ecb].vm)
                printf("已分配 ");
            else
            {
                printf("未分配 ");
                continue ;
            }
            StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
            pads = 0x64646464;
            stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
            for(loop = 0; loop < (StackSize>>2)-1; loop++)
            {
                if(stack[loop] != pads)
                {
                    break;
                }
            }
#if CFG_OS_TINY == false
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == false
            printf("%08x %08x %08x %08x %s",
                        (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                        (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                        StackSize,(loop<<2),
                        name);
        }
        else
        {
//            printf("knlshell","%05d  空闲事件控制块",pl_ecb);
        }
    }

    printf("\n\r栈指针是最后一次上下文切换时保存的值");
    return (TRUE);
}
u32 knlYipHook(struct Wdt *wdt)
{
    printk("--------idle wdt yit--------");
    eventk(NULL);
    return 0;
}
#pragma GCC diagnostic pop

// ============================================================================
// 功能：统计一定内事件运行占用率
// 参数：无；
// 返回：无；
// 备注：
// ============================================================================
ptu32_t kernel_spy(void)
{
#if CFG_OS_TINY == false
    u32 pl_ecb;
    u32 cycle;
    struct Wdt *wdt;
    DJY_GetEventPara((ptu32_t*)(&cycle), NULL);
    cycle *=mS;
#if(CFG_IDLE_MONITOR_CYCLE > 0)
#if(CFG_IDLE_WDT_RESET == false)
    wdt = Wdt_Create("runtime watch", cycle * CFG_IDLE_MONITOR_CYCLE, knlYipHook, EN_BLACKBOX_DEAL_IGNORE, 0, 0);
#else
    wdt = Wdt_Create("runtime watch", cycle * CFG_IDLE_MONITOR_CYCLE, NULL, EN_BLACKBOX_DEAL_RESET, 0, 0);
#endif  //for (CFG_IDLE_WDT_RESET != 1)
#endif  //for (CFG_IDLE_MONITOR_CYCLE > 0)
    while(1)
    {

        for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
        {
           g_tECB_Table[pl_ecb].consumed_time_second =
                              (u32)g_tECB_Table[pl_ecb].consumed_time
                            - g_tECB_Table[pl_ecb].consumed_time_record;
           g_tECB_Table[pl_ecb].consumed_time_record =
                            (u32)g_tECB_Table[pl_ecb].consumed_time;
        }
#if(CFG_IDLE_MONITOR_CYCLE > 0)
        //如果idle事件运行时间超过 1/16，则喂狗
        if(g_tECB_Table[0].consumed_time_second > (cycle >> 4))
        {
            Wdt_Clean(wdt);
        }
#endif  //for (CFG_IDLE_MONITOR_CYCLE > 0)
        DJY_EventDelay(cycle); // 延时1秒；
    }
#endif
}

// ============================================================================
// 功能：kernel shell命令的调试模式
// 参数：
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 kernel_command(void)
{
    u16 res;

    u32 cycle = 1000; // 1s时间，监测周期


    res = DJY_EvttRegist(EN_CORRELATIVE, 1, 0, 0,
                        kernel_spy, NULL, 1024, "kernel spy");
    if(res==CN_EVTT_ID_INVALID)
        return(-1);

    DJY_EventPop(res, NULL, 0, (ptu32_t)cycle, 0, 0);
    return (0);
}

bool_t spy(char *param)
{
    u16 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printf("\r\n事件号 线程   栈底     栈指针   栈尺寸   剩余量   类型名");
    pl_ecb = atoi(param);
    if(g_tECB_Table[pl_ecb].previous !=
                    (struct EventECB*)&s_ptEventFree)
    {
        printf("\n\r");
        printf("%05d  ",pl_ecb);
        if(g_tECB_Table[pl_ecb].vm)
            printf("已分配 ");
        else
        {
            printf("未分配 ");
            return true ;
        }
        StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
        pads = 0x64646464;
        stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
        for(loop = 0; loop < (StackSize>>2)-1; loop++)
        {
            if(stack[loop] != pads)
            {
                break;
            }
        }
#if CFG_OS_TINY == false
        name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
        name = "unkown";
#endif  //CFG_OS_TINY == false
        printf("%08x %08x %08x %08x %s",
                    (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                    (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                    StackSize,(loop<<2),name);
        stack = g_tECB_Table[pl_ecb].vm->stack;
        printf("\n\r%x: ",(u32)stack);
        for(;stack < g_tECB_Table[pl_ecb].vm->stack_top; stack++)
        {
            if((u32)stack % 32 == 0)
                printf("\n\r%x: ",(u32)stack);
            printf(" %08x",*stack);
        }
    }
    else
    {
        printf("%05d  空闲事件控制块",pl_ecb);
    }
    return (TRUE);
}
bool_t spyk(u16 pl_ecb)
{
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    printk("\r\n事件号 线程   栈底     栈指针   栈尺寸   剩余量   类型名");
    if(g_tECB_Table[pl_ecb].previous !=
                    (struct EventECB*)&s_ptEventFree)
    {
        printk("\n\r");
        printk("%05d  ",pl_ecb);
        if(g_tECB_Table[pl_ecb].vm)
            printk("已分配 ");
        else
        {
            printk("未分配 ");
            return true ;
        }
        StackSize = g_tECB_Table[pl_ecb].vm->stack_size;
        pads = 0x64646464;
        stack = (u32*)(&(g_tECB_Table[pl_ecb].vm[1]));
        for(loop = 0; loop < (StackSize>>2)-1; loop++)
        {
            if(stack[loop] != pads)
            {
                break;
            }
        }
#if CFG_OS_TINY == false
        name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
        name = "unkown";
#endif  //CFG_OS_TINY == false
        printk("%08x %08x %08x %08x %s",
                    (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                    (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                    StackSize,(loop<<2),name);
        stack = g_tECB_Table[pl_ecb].vm->stack;
        printk("\n\r%x: ",(u32)stack);
        for(;stack < g_tECB_Table[pl_ecb].vm->stack_top; stack++)
        {
            if((u32)stack % 32 == 0)
                printk("\n\r%x: ",(u32)stack);
            printk(" %08x",*stack);
        }
    }
    else
    {
        printk("%05d  空闲事件控制块",pl_ecb);
    }
    return (TRUE);
}

ADD_TO_ROUTINE_SHELL(spy,spy,"");
ADD_TO_ROUTINE_SHELL(stack,stack,"显示系统中所有已经分配线程的事件的栈信息");
ADD_TO_ROUTINE_SHELL(event,event,"显示事件表");
ADD_TO_ROUTINE_SHELL(evtt,evtt,"显示事件类型表");

