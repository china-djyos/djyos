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
//所属模块: 调度器
//作者：lst
//版本：V1.0.1
//文件描述: 调度器中与CPU直接相关的代码。
//其他说明:
//修订历史:
//2. 日期: 2009-04-24
//   作者: lst
//   新版本号: V1.0.1
//   修改说明: 删除了一些为dlsp版本准备的东西
//1. 日期: 2009-01-04
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#include <os.h>
#include <shell.h>
#include <blackbox.h>
#include <osboot.h>
#include "dbug.h"
#include "board-config.h"
#include "component_config_core.h"
static char *pBootModeName[]={
    "POWERDOWN",
    "HARDRESET",
    "SOFTRESET",
    "REBOOT",
    "RELOAD",
    "NOIMPLEMENT",
    "UNKNOWN",
};
//系统重启以及启动后的异常信息搜集分析
typedef struct
{
    time_t   time;       //rtc time when boot
    u32      modesoft;   //defines by enBootMode
    u32      modehard;   //defines by enBootMode
}tagBootMode;
static tagBootMode gBootMode;

extern struct EventECB  *g_ptEventReady;      //就绪队列头
extern struct EventECB  *g_ptEventRunning;    //当前正在执行的事件
extern struct EventECB  *g_ptEventDelay;      //闹钟同步队列表头
extern struct EventType g_tEvttTable[];
typedef struct
{
    u32                  dumpreason;
    struct EventECB     *eventrunning;
    struct EventECB     *eventready;
    struct EventECB     *eventdelay;
    struct ThreadVm     *eventvm;
    bool_t   ecblegal;
    bool_t   vmlegal;
    struct EventECB  ecbRunning;  //if the pEventRunning is in the proper memory,then record it
    struct ThreadVm  vm;          //if vm in proper mem, then record it
}tagSysDumpState;
#define CN_ILLEGAL_BOOTNAME   "ILLEGALBOOT"

//默认的启动方式的获取
static enBootMode GetOsBootModeDefault(void)
{
    return EN_BOOT_UNKNOWN;
}
//该变量用于存储软件提供的vm可能使用的内存空间
static tagVmMemItem *pMemLegalTab;
static fnGetBootMode fnGetModeHard = GetOsBootModeDefault;  //获取启动标志：硬件解决方案
static fnGetBootMode fnGetModeSoft = GetOsBootModeDefault;  //获取启动标志:软件解决方案

//check if addr in mem:true in mem， else not in mem
static bool_t __AddrInMem(u8 *addr)
{
    bool_t result = false;
    tagVmMemItem *item;
    u8      *start;
    u8      *end;

    if(NULL == pMemLegalTab)
    {
        return result;
    }
    item = pMemLegalTab;
    while(NULL != item)
    {
        start = item->start;
        end = start + item->len;
        if((addr >= start)&&((addr <= end))) //in mem section
        {
            result = true;
            break;
        }
        else
        {
            item++;
        }
    }
    return result;
}
//this function used to record the ecb and the stack
bool_t ThrowOsBootInfo(enBootMode mode)
{
    tagSysDumpState dumpinfo;
    struct BlackBoxThrowPara  parahead;

    u8  *srcstart;
    u8  *srcend;

    memset(&dumpinfo,0,sizeof(dumpinfo));
    dumpinfo.dumpreason = mode;
    dumpinfo.eventready = g_ptEventReady;
    dumpinfo.eventrunning = g_ptEventRunning;
    dumpinfo.eventdelay = g_ptEventDelay;
    //check if event running is correct,if correct then copy it
    srcstart = (u8 *)dumpinfo.eventrunning;
    srcend = srcstart + sizeof(struct EventECB);
    if(__AddrInMem(srcstart)&&__AddrInMem(srcend))
    {
        memcpy(&dumpinfo.ecbRunning,dumpinfo.eventrunning,sizeof(struct EventECB));
        dumpinfo.ecblegal = true;
        //check if the running vm is correct,if correct then copy it
        srcstart = (u8 *)dumpinfo.eventrunning->vm;
        srcend = srcstart + sizeof(struct ThreadVm);
        if(__AddrInMem(srcstart)&&__AddrInMem(srcend))
        {
            memcpy(&dumpinfo.vm,dumpinfo.eventrunning->vm,sizeof(struct ThreadVm));
            dumpinfo.vmlegal = true;
        }
    }
    parahead.DecoderName = CN_ILLEGAL_BOOTNAME;
    parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
    parahead.BlackBoxInfo = (u8 *)&dumpinfo;
    parahead.BlackBoxInfoLen = sizeof(dumpinfo);
    parahead.BlackBoxType = CN_BLACKBOX_TYPE_SYS_START+11;
    BlackBox_Recorder(&parahead);
    return true;
}


static bool_t __CpuDumpDecoder(struct BlackBoxThrowPara  *exppara,u32 endian)
{
    tagSysDumpState *dumpinfo;
    struct EventECB *ecb;
    struct ThreadVm *vm;

    debug_printf("osboot","DUMPHEAD:Decoder:%s Action:%d BlackBoxLen:%d BlackBoxType:%d Endian:%d\n\r",\
            exppara->DecoderName,exppara->BlackBoxAction,exppara->BlackBoxInfoLen,exppara->BlackBoxType,endian);
    dumpinfo = (tagSysDumpState *)exppara->BlackBoxInfo;
    debug_printf("osboot","DUMPINFO:Reason:%s EcbRun:0x%08x EcbReady:0x%08x EcbDelay:0x%08x\n\r",\
            pBootModeName[dumpinfo->dumpreason],(u32)dumpinfo->eventrunning,\
            (u32)dumpinfo->eventready,(u32)dumpinfo->eventdelay);
    debug_printf("osboot","EcbRunning:%s\n\r",dumpinfo->ecblegal?"LEGAL":"ILLEGAL");
    if(dumpinfo->ecblegal)
    {
        ecb = &dumpinfo->ecbRunning;
        debug_printf("osboot","EcbPre:0X%08x EcbNxt:0x%08x EcbMpre:0x%08x EcbMnxt:0x%08x\n\r",\
                (u32)ecb->previous,(u32)ecb->next,(u32)ecb->multi_previous,(u32)ecb->multi_next);
        debug_printf("osboot","EcbVmAddr:0x%08x\n\r",(u32)ecb->vm);
        debug_printf("osboot","Param1:0x%08x Param2:0x%08x\n\r",ecb->param1,ecb->param2);
        debug_printf("osboot","Sync:0x%08x SyncHead:0x%08x SyncCounter:0x%08x\n\r",(u32)ecb->sync,(u32)ecb->sync_head,ecb->sync_counter);
#if CFG_OS_TINY == false
#if (CN_USE_TICKLESS_MODE)
        debug_printf("osboot","EventStartTime:0x%llx  EventConsumeTime:0x%llx\n\r",ecb->EventStartCnt,ecb->consumed_cnt);
        debug_printf("osboot","ConsumedTimeSecond:0x%08x ComsumeTimeRecord:0x%08x\n\r",ecb->consumed_cnt_second,ecb->consumed_cnt_record);
#else
        debug_printf("osboot","EventStartTime:0x%llx  EventConsumeTime:0x%llx\n\r",ecb->EventStartTime,ecb->consumed_time);
        debug_printf("osboot","ConsumedTimeSecond:0x%08x ComsumeTimeRecord:0x%08x\n\r",ecb->consumed_time_record,ecb->consumed_time_record);
#endif
#endif  //CFG_OS_TINY == false
#if (CN_USE_TICKLESS_MODE)
        debug_printf("osboot","DelayStartTick:0x%llx DelayEndTick:0x%llx\n\r",ecb->delay_start_cnt,ecb->delay_end_cnt);
#else
        debug_printf("osboot","DelayStartTick:0x%llx DelayEndTick:0x%llx\n\r",ecb->delay_start_tick,ecb->delay_end_tick);
#endif
        debug_printf("osboot","ErrNo:%d EventResult:0x%08x\n\r",ecb->error_no,ecb->event_result);
        debug_printf("osboot","WaitMemSize:%d\n\r",ecb->wait_mem_size);
        debug_printf("osboot","WakeupFrom:0x%08x Status:0x%08x\n\r",ecb->wakeup_from,ecb->event_status);
        debug_printf("osboot","Prior:%d\n\r",ecb->prio);
#if CFG_OS_TINY == false
        debug_printf("osboot","EvttID:%d Name:%s\n\r",(ecb->evtt_id&(~CN_EVTT_ID_MASK)),\
                (ecb->evtt_id&(~CN_EVTT_ID_MASK)) < CFG_EVENT_TYPE_LIMIT?\
                        g_tEvttTable[(ecb->evtt_id&(~CN_EVTT_ID_MASK))].evtt_name:"unknown");
#else
        debug_printf("osboot","EvttID:%d Name:%s\n\r",(ecb->evtt_id&(~CN_EVTT_ID_MASK)),"unknown");
#endif  //CFG_OS_TINY == false

        debug_printf("osboot","EventID:%d LocalMem:%d\n\r",ecb->event_id,ecb->local_memory);
        debug_printf("osboot","EcbStack:%s\n\r",dumpinfo->vmlegal?"LEGAL":"ILLEGAL");

        if(dumpinfo->vmlegal)
        {
            vm = ecb->vm;
            debug_printf("osboot","Stack:0x%08x StackTop:0x%08x StackSize:%d Nxt:0x%08x HostVm:0x%08x\n\r",\
                    (u32)vm->stack,(u32)vm->stack_top,vm->stack_size,(u32)vm->next,(u32)vm->host_vm);
        }
    }
    debug_printf("osboot","DUMPEND\n\r");
    return true;
}


#define CN_BOOTLOG_NAME   "OSBOOTLOG"
static void __ShowBootMode(tagBootMode *bootmsg)
{
    time_t  logtime;
    u32      bootreason;
    logtime = bootmsg->time;
    debug_printf("osboot","BootTime:%s\n\r",ctime(&logtime));
    //SOFT CHECK
    bootreason = bootmsg->modesoft;
    debug_printf("osboot","BootMode(SoftCheck):%s\n\r",bootreason<EN_BOOT_LAST?pBootModeName[bootreason]:"UNKNOWN");
    //HARD CHECK
    bootreason = bootmsg->modehard;
    debug_printf("osboot","BootMode(HardCheck):%s\n\r",bootreason<EN_BOOT_LAST?pBootModeName[bootreason]:"UNKNOWN");
}

//static bool_t __BootMsgShowShell(char *param)
bool_t bootmsg(char *param)
{
    tagBootMode *msg;

    msg = &gBootMode;
    __ShowBootMode(msg);

    return true;
}

static bool_t __BootModeDecoder(struct BlackBoxThrowPara *exppara,u32 endian)
{
    tagBootMode msg;
    debug_printf("osboot","DUMPHEAD:Decoder:%s Action:%d BlackBoxLen:%d BlackBoxType:%d Endian:%d\n\r",\
            exppara->DecoderName,exppara->BlackBoxAction,exppara->BlackBoxInfoLen,exppara->BlackBoxType,endian);
    memcpy(&msg,exppara->BlackBoxInfo,sizeof(msg));
    __ShowBootMode(&msg);
    return true;
}


//add some boot commands
extern void reboot(u32 key);
extern void reset(u32 key);
extern void restart_system(u32 key);
//static bool_t rebootshell(char *param)
bool_t rebootshell(char *param)
{
    u32 key = 0XAA55AA55;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }

    reboot(key);
    return true;
}
//static bool_t resetshell(char *param)
bool_t resetshell(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    reset(key);
    return true;
}
//static bool_t reloadshell(char *param)
bool_t restart(char *param)
{
    u32 key = 0;
    if(NULL != param)
    {
        key = strtoul(param,NULL,0);
    }
    restart_system(key);
    return true;
}

//static bool_t bootaddressshell(char *param)
bool_t bootaddress(char *param)
{
    u32 addr;
    u32 InitCpu_Addr;
    addr = strtoul(param,NULL,0);

    debug_printf("osboot","%s:addr:0x%08x\n\r",__FUNCTION__,addr);
    InitCpu_Addr = *(u32*)addr;
    ((void (*)(void))(InitCpu_Addr))();

    return true;
}


//use this function to analyze the boot mode
static bool_t __OsBootModeLog(void)
{
    gBootMode.time = time(NULL);
    //通过硬件分析
    if(NULL != fnGetModeHard)
    {
        gBootMode.modehard = fnGetModeHard();
    }
    else
    {
        gBootMode.modehard = EN_BOOT_NOTIMPLEMENT;
    }
    //通过软件分析
    if(NULL != fnGetModeSoft)
    {
        gBootMode.modesoft = fnGetModeSoft();
    }
    else
    {
        gBootMode.modesoft = EN_BOOT_NOTIMPLEMENT;
    }
    //record the boot message
    struct BlackBoxThrowPara  parahead;
    parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
    parahead.DecoderName = CN_BOOTLOG_NAME;
    parahead.BlackBoxInfo = (u8 *)&gBootMode;
    parahead.BlackBoxInfoLen =sizeof(gBootMode);
    parahead.BlackBoxType = CN_BLACKBOX_TYPE_SYS_START+10;
    BlackBox_Recorder(&parahead);
    return true;
}
//----安装系统启动模块-----------------------------------------------------------
//功能：该模块用于分析系统启动原因，以及记录系统非法重启状态
//参数：tab,系统栈的可能使用内存范围；getmodehard用于获取硬件启动模式；getmodesoft用
//      于获取软件启动模式log,如果false,则不执行记录，否则本次启动将会记录
//返回：成功或者失败
//注:  该模块本身和BSP无关，但是tab 以及获取启动标志需要BSP的支持；硬件启动标志的意思
//      是很多CPU本身有寄存器记录启动原因
//      软件启动标志是利用RAM的特性由BSP porting的时候设计的。
//-----------------------------------------------------------------------------
bool_t ModuleInstall_OsBoot(const tagVmMemItem *tab[],fnGetBootMode getmodehard,
                            fnGetBootMode getmodesoft)
{
    bool_t result=true;
    static struct BlackBoxInfoDecoder dumpdecoder;
    static struct BlackBoxInfoDecoder bootdecoder;

    pMemLegalTab = tab;
    if(NULL != getmodehard)
    {
        fnGetModeHard = getmodehard;
    }
    if(NULL != getmodesoft)
    {
        fnGetModeSoft = getmodesoft;
    }

    memset(&dumpdecoder,0,sizeof(dumpdecoder));
    dumpdecoder.DecoderName = CN_ILLEGAL_BOOTNAME;
    dumpdecoder.MyDecoder = __CpuDumpDecoder;
    BlackBox_RegisterThrowInfoDecoder(&dumpdecoder);

    memset(&bootdecoder,0,sizeof(bootdecoder));
    bootdecoder.DecoderName = CN_BOOTLOG_NAME;
    bootdecoder.MyDecoder = __BootModeDecoder;
    BlackBox_RegisterThrowInfoDecoder(&bootdecoder);

    //记录系统启动状态
    __OsBootModeLog();
    return result;

}

ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootaddress,bootaddress,"usage:bootaddress [address]");
ADD_TO_ROUTINE_SHELL(rebootshell,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(resetshell,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootmsg,bootmsg,"usage:bootmsg");
