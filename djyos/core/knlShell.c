//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "exp.h"
#include "dbug.h"
#include "component_config_core.h"
#include "newshell.h"
extern struct EventECB g_tECB_Table[];
extern struct EventType g_tEvttTable[];
extern struct EventECB  *s_ptEventFree; //空闲链表头,不排序

//void __M_ShowHeap(void);
//void __M_ShowHeapSpy(void);

//bool_t Sh_ShowEvent(char *param);
//bool_t Sh_ShowEvtt(char *param);
//bool_t Sh_ShowStack(char *param);
//----显示事件表---------------------------------------------------------------
//功能: 显示事件列表
//参数: 无
//返回: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowEvent(char *param)
ADD_TO_SHELL_HELP(event,"显示事件表");
ADD_TO_IN_SHELL bool_t event(char *param)
{
    u16 pl_ecb;
    u32 time1,MemSize;
    char *name;

    MemSize = 0;
    debug_printf("knlshell","事件号  类型号  优先级 CPU  栈尺寸   类型名\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {
        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            debug_printf("knlshell","%05d %05d     ",pl_ecb,g_tECB_Table[pl_ecb].evtt_id &(~CN_EVTT_ID_MASK));
            debug_printf("knlshell","%03d    ",g_tECB_Table[pl_ecb].prio);
           // debug_printf("knlshell","%02d%%  %8x",time1,g_tECB_Table[pl_ecb].vm->stack_size);
#if CFG_OS_TINY == false
            time1 = g_tECB_Table[pl_ecb].consumed_cnt_second/10000;
            name = g_tEvttTable[g_tECB_Table[pl_ecb].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name;
#else
            time1 = 0;
            name = "unkown";
#endif  //CFG_OS_TINY == false
            if(NULL == g_tECB_Table[pl_ecb].vm)
            {
                debug_printf("knlshell","%02d%%  %08x %s",0,0,name);
            }
            else
            {
                debug_printf("knlshell","%02d%%  %08x %s",time1,g_tECB_Table[pl_ecb].vm->stack_size,name);
                MemSize += g_tECB_Table[pl_ecb].vm->stack_size;
            }
            debug_printf("knlshell","\n\r");
        }
        else
        {
//          debug_printf("knlshell","%5d   空闲",pl_ecb);
        }
    }
    debug_printf("knlshell","所有事件栈尺寸总计:           %08x\n\r",MemSize);
    debug_printf("knlshell","允许的事件总数:              %d\n\r",CFG_EVENT_LIMIT);

    return true;
}

//----显示事件类型表-----------------------------------------------------------
//功能: 显示事件类型列表
//参数: 无
//返回: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowEvtt(char *param)
ADD_TO_SHELL_HELP(evtt,"显示事件类型表");
ADD_TO_IN_SHELL bool_t evtt(char *param)
{
    u16 pl_ecb;
    u32 MemSize;
    char *name;
    MemSize = 0;
    debug_printf("knlshell","类型号  优先级 处理函数  栈需求   名字\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_TYPE_LIMIT; pl_ecb++)
    {
        if(g_tEvttTable[pl_ecb].property.registered ==1)
        {
            MemSize += g_tEvttTable[pl_ecb].stack_size;
            debug_printf("knlshell","%05d   ",pl_ecb);
            debug_printf("knlshell","%03d    ",g_tEvttTable[pl_ecb].default_prio);
            debug_printf("knlshell","%08x  %08x ",
                        (ptu32_t)g_tEvttTable[pl_ecb].thread_routine,
                        g_tEvttTable[pl_ecb].stack_size);
#if CFG_OS_TINY == false
            name = g_tEvttTable[pl_ecb].evtt_name;
#else
            name = "unkown";
#endif  //CFG_OS_TINY == false
            debug_printf("knlshell","%s",name);
            debug_printf("knlshell","\n\r");
        }
        else
        {
//          printf("knlshell","%05d   空闲",pl_ecb);
        }
    }
    debug_printf("knlshell","所有类型栈需求总计:      %08x\n\r",MemSize);
    debug_printf("knlshell","允许的事件类型总数:      %d\n\r",CFG_EVENT_TYPE_LIMIT);
    return true;
}

//----显示栈使用情况-----------------------------------------------------------
//功能: 显示系统中所有已经分配线程的事件的栈信息
//参数: 无
//返回: true
//-----------------------------------------------------------------------------
//bool_t Sh_ShowStack(char *param)

ADD_TO_SHELL_HELP(stack,"显示系统中所有已经分配线程的事件的栈信息");
ADD_TO_IN_SHELL bool_t stack(char *param)
{
    u16 pl_ecb;
    u32 loop,StackSize,pads;
    u32 *stack;
    char *name;

    debug_printf("knlshell","事件号 线程   栈底     栈指针   栈尺寸   剩余量   类型名\n\r");
    for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
    {

        if(g_tECB_Table[pl_ecb].previous !=
                        (struct EventECB*)&s_ptEventFree)
        {
            debug_printf("knlshell","%05d  ",pl_ecb);
            if(g_tECB_Table[pl_ecb].vm)
                debug_printf("knlshell","已分配 ");
            else
            {
                debug_printf("knlshell","未分配 ");
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
            debug_printf("knlshell","%08x %08x %08x %08x %s",
                        (ptu32_t)(&g_tECB_Table[pl_ecb].vm[1]),
                        (ptu32_t)(g_tECB_Table[pl_ecb].vm->stack),
                        StackSize,(loop<<2),
                        name);
            debug_printf("knlshell","\n\r");
        }
        else
        {
//            debug_printf("knlshell","%05d  空闲事件控制块",pl_ecb);
        }
    }
    debug_printf("knlshell","栈指针是最后一次上下文切换时保存的值\n\r");

    return true;
}

//struct ShellCmdTab const tg_ShellKernelCmdTbl[] =
//{
//   {
//        "event",
//        Sh_ShowEvent,
//        "显示事件表",
//        NULL
//    },
//    {
//        "evtt",
//        Sh_ShowEvtt,
//        "显示事件类型表",
//        NULL
//    },
//    {
//        "heap",
//        (bool_t (*)(char*))__M_ShowHeap,
//        "显示堆使用情况",
//        NULL
//    },
//    {
//        "heap-spy",
//        (bool_t (*)(char*))__M_ShowHeapSpy,
//        "显示动态内存详细分配情况",
//        NULL
//    },
//    {
//        "stack",
//        (bool_t (*)(char*))Sh_ShowStack,
//        "显示系统中所有已经分配线程的事件的栈信息",
//        NULL
//    },
//};
//
//static struct ShellCmdRsc tg_ShellKernelCmd
//                        [sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab)];

#if CFG_OS_TINY == false
ptu32_t Debug_Scan(void)
{
    u32 pl_ecb;

    while(1)
    {

        for(pl_ecb = 0; pl_ecb < CFG_EVENT_LIMIT; pl_ecb++)
        {
            g_tECB_Table[pl_ecb].consumed_cnt_second =
                              (u32)g_tECB_Table[pl_ecb].consumed_cnt
                            - g_tECB_Table[pl_ecb].consumed_cnt_record;
            g_tECB_Table[pl_ecb].consumed_cnt_record =
                            (u32)g_tECB_Table[pl_ecb].consumed_cnt;
        }
        Djy_EventDelay(1000*mS);
    }
}
#endif  //CFG_OS_TINY == false

//----初始化调试信息模块-------------------------------------------------------
//功能: 创建调试信息事件类型并启动之
//参数: 无
//返回: 无
//-----------------------------------------------------------------------------
void ModuleInstall_DebugInfo(ptu32_t para)
{
    u16 evtt_debug;
    para = para;        //消除编译器告警
#if CFG_OS_TINY == false
    evtt_debug = Djy_EvttRegist(EN_CORRELATIVE,1,0,0,
                                 Debug_Scan,NULL,1024,"debug_info");
    if(evtt_debug == CN_EVTT_ID_INVALID)
        return;
    Djy_EventPop(evtt_debug,NULL,0,0,0,0);
#endif  //CFG_OS_TINY == false
//    Sh_InstallCmd(tg_ShellKernelCmdTbl,tg_ShellKernelCmd,
//            sizeof(tg_ShellKernelCmdTbl)/sizeof(struct ShellCmdTab));
}


