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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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
//ϵͳ�����Լ���������쳣��Ϣ�Ѽ�����
typedef struct
{
    time_t   time;       //rtc time when boot
    u32      modesoft;   //defines by enBootMode
    u32      modehard;   //defines by enBootMode
}tagBootMode;
static tagBootMode gBootMode;

extern struct EventECB  *g_ptEventReady;      //��������ͷ
extern struct EventECB  *g_ptEventRunning;    //��ǰ����ִ�е��¼�
extern struct EventECB  *g_ptEventDelay;      //����ͬ�����б�ͷ
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

//Ĭ�ϵ�������ʽ�Ļ�ȡ
static enBootMode GetOsBootModeDefault(void)
{
    return EN_BOOT_UNKNOWN;
}
//�ñ������ڴ洢����ṩ��vm����ʹ�õ��ڴ�ռ�
static tagVmMemItem *pMemLegalTab;
static fnGetBootMode fnGetModeHard = GetOsBootModeDefault;  //��ȡ������־��Ӳ���������
static fnGetBootMode fnGetModeSoft = GetOsBootModeDefault;  //��ȡ������־:����������

//check if addr in mem:true in mem�� else not in mem
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
    //ͨ��Ӳ������
    if(NULL != fnGetModeHard)
    {
        gBootMode.modehard = fnGetModeHard();
    }
    else
    {
        gBootMode.modehard = EN_BOOT_NOTIMPLEMENT;
    }
    //ͨ���������
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
//----��װϵͳ����ģ��-----------------------------------------------------------
//���ܣ���ģ�����ڷ���ϵͳ����ԭ���Լ���¼ϵͳ�Ƿ�����״̬
//������tab,ϵͳջ�Ŀ���ʹ���ڴ淶Χ��getmodehard���ڻ�ȡӲ������ģʽ��getmodesoft��
//      �ڻ�ȡ�������ģʽlog,���false,��ִ�м�¼�����򱾴����������¼
//���أ��ɹ�����ʧ��
//ע:  ��ģ�鱾���BSP�޹أ�����tab �Լ���ȡ������־��ҪBSP��֧�֣�Ӳ��������־����˼
//      �Ǻܶ�CPU�����мĴ�����¼����ԭ��
//      ���������־������RAM��������BSP porting��ʱ����Ƶġ�
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

    //��¼ϵͳ����״̬
    __OsBootModeLog();
    return result;

}

ADD_TO_ROUTINE_SHELL(restart,restart,"usage:restart [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootaddress,bootaddress,"usage:bootaddress [address]");
ADD_TO_ROUTINE_SHELL(rebootshell,rebootshell,"usage:rebootshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(resetshell,resetshell,"usage:resetshell [key](if key is 0XAA55AA55 then will not record)");
ADD_TO_ROUTINE_SHELL(bootmsg,bootmsg,"usage:bootmsg");
