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
// AND ANY BLACKBOXRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
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
// 模块描述: 异常的抛出以及异常信息解析
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 5:59:37 PM/Apr 17, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
// 备注:所有异常的统一入口
// 该文件为异常的抛出处理，在抛出异常之前，务必保证针对该异常的特殊处理已经做了
// 该文件只是更好的管理异常信息及其架构，而非统辖整个异常处理


#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "int.h"
#include "arch_feature.h"
#include "blackbox.h"
#include "endian.h"
#include "time.h"
#include "blackbox_decoder.h"
#include "blackbox_hook.h"
#include "blackbox_osstate.h"
#include "BlackBox_Record.h"
#include "dbug.h"
#include "component_config_blackbox.h"
#include <shell.h>

const char *gBlackBoxActionName[EN_BLACKBOX_DEAL_LENTH] = {
    "IGNORE",
    "DEFAULT",
    "RECORD",
    "RESET",
    "REBOOT",
    "RESTART",
    "WAIT",
    "DELERR",
};

const char *BlackBox_ActionName(enum EN_BlackBoxAction action)
{
    if(action < EN_BLACKBOX_DEAL_LENTH)
    {
        return gBlackBoxActionName[action];
    }
    else
    {
        return NULL;
    }
}
extern bool_t HardExp_InfoDecoderInit(void);
#define CN_DECODER_NAMELEN_LIMIT 16
//全部相同，那么不涉及到大小端的问题
#define CN_BLACKBOX_HEADINFO_MAGICNUMBER    ((u32)(0x77777777))
#define CN_BLACKBOX_PARTIALINFO_VALID       ((u32)(0x12345678))
struct BlackBoxHeadInfo
{
    u32   recordendian;     //异常记录是大端还是小端,利于离线分析异常记录
    u32   magicnumber;      //用于检查信息是否有效
    u32   BlackBoxType;     //异常类型
    u32   osstatevalid;     //CN_BLACKBOX_PARTIALINFO_VALID=操作系统运行状态信息有效
    u32   osstateinfolen;   //操作系统运行状态信息长度
    u32   hookvalid;        //CN_BLACKBOX_PARTIALINFO_VALID=hook收集的信息有效
    u32   hookinfolen;      //hook收集的信息长度
    u32   HookAction;       //hook要求的异常处理动作,由enum EN_BlackBoxAction定义
    u32   throwinfovalid;   //抛出的信息存储区是否有效
    u32   throwinfolen;     //抛出的信息长度
    u32   ThrowAction;      //抛出时要求的处理动作,由enum EN_BlackBoxAction定义
    u32   BlackBoxAction;        //最终采取的动作.,由enum EN_BlackBoxAction定义
    char  decodername[CN_DECODER_NAMELEN_LIMIT];  //异常解码器名字,长度不得超过15
                            //如果是个NULL指针,则表示无解码器
};//可以考虑使用bit位来省存储空间，但是考虑下来，根本省不了几个字节，改动意义不大

static bool_t s_bBlackBoxMoDuleInitState = false;
// =============================================================================
// 函数功能:__BlackBox_ResultMerge
//           将HOOK结果和THROW结果做综合处理
// 输入参数:Result_Throw,设定的异常处理结果
//          Result_Hook,HOOK的异常处理结果
// 输出参数:无
// 返回值  :_SYSBLACKBOX_RESULT_TYPE,异常的处理结果
// 说明    :内部调用函数
// =============================================================================
static enum EN_BlackBoxAction  __BlackBox_ResultMerge(enum EN_BlackBoxAction Result_Throw,\
                                             enum EN_BlackBoxAction Result_Hook)
{
    enum EN_BlackBoxAction result;

    if(EN_BLACKBOX_DEAL_DEFAULT == Result_Hook)
    {
        result = Result_Throw;
        result = Result_Throw;
    }
    else
    {
        result = Result_Hook;
    }
    return result;
}

// =============================================================================
// 函数功能: 处理异常的最终结果
// 输入参数:blackboxdealresult,异常结果
//          blackboxinfo,搜集的异常信息
// 输出参数:无
// 返回值  :异常的最终处理结果(见enum _EN_BLACKBOX_DEAL_REASULT声明)
// 说明    :内部调用函数,如果异常结果中有需要重启服务，那么就不会返回的。
// =============================================================================
static enum EN_BlackBoxDealResult  __BlackBox_ExecAction(u32 FinalAction,\
                                    struct BlackBoxRecordPara *blackboxinfo)
{
    enum EN_BlackBoxDealResult result;
    result = __BlackBox_Record(blackboxinfo);
    if(result != EN_BLACKBOX_RESULT_SUCCESS)
    {
        printk("blackboxresultdealer:RECORD FAILED!\n\r");
    }
    switch (FinalAction)
    {
        //重启是不会返回的，不然必然是出错了
        case EN_BLACKBOX_DEAL_RESET:
            CPU_Reset();
            break;
        case EN_BLACKBOX_DEAL_REBOOT:
            CPU_Reboot();
            break;
        case EN_BLACKBOX_DEAL_RESTART:
            CPU_RestartSystem();
            break;
        case EN_BLACKBOX_DEAL_WAIT:
            while(1);
            break;
        default:
            break;
    }
    return result;
}
// =============================================================================
// 函数功能: 抛出异常信息
// 输入参数:throwpara,抛出的异常信息参数
// 返回值  :异常处理需采取的行动，如果有hook，可能会被hook的返回值替代
// 说明    :本函数很可能不返回，直接复位
// =============================================================================
enum EN_BlackBoxDealResult  BlackBox_ThrowExp(struct BlackBoxThrowPara *throwpara)
{
    bool_t result;
    enum EN_BlackBoxAction   HookAction;
    enum EN_BlackBoxAction   ThrowAction;
    enum EN_BlackBoxAction   FinalAction;
    enum EN_BlackBoxDealResult Result;
    ptu32_t infoaddr;
    u32 infolen;
    struct BlackBoxHeadInfo  headinfo;
    struct BlackBoxRecordPara recordpara;
    if(s_bBlackBoxMoDuleInitState == false)  //组件未初始化，很多系统调用都不能使用
    {
        return EN_BLACKBOX_RESULT_PARAERR;
    }
    printk("\r\nblockbox called form function: %s line: %d", __FUNCTION__,__LINE__);
    if(NULL != throwpara)   //抛出有效
    {
        //抛出信息处理
        if(NULL != throwpara->DecoderName)
        {
            strncpy(headinfo.decodername, throwpara->DecoderName, CN_DECODER_NAMELEN_LIMIT);
        }
        else
        {
            headinfo.decodername[0] = '\0';
        }
        headinfo.throwinfolen = throwpara->BlackBoxInfoLen;
        headinfo.ThrowAction = (u32)throwpara->BlackBoxAction;
        headinfo.BlackBoxType = throwpara->BlackBoxType;
        headinfo.throwinfovalid = CN_BLACKBOX_PARTIALINFO_VALID;
        ThrowAction = throwpara->BlackBoxAction;
        recordpara.throwinfoaddr = (ptu32_t)(throwpara->BlackBoxInfo);
        recordpara.throwinfolen = throwpara->BlackBoxInfoLen;
        recordpara.headinfoaddr = (ptu32_t)(&(headinfo));
        recordpara.headinfolen = (u32)(sizeof(headinfo));

        //HOOK信息的搜集（如果搜集不成功，并且抛出者无意记录，则直接返回）
        result = __BlackBox_HookDealer(throwpara, &infoaddr, &infolen, &HookAction);
        if(false == result)
        {
            HookAction = EN_BLACKBOX_DEAL_DEFAULT;
            headinfo.hookvalid = ~CN_BLACKBOX_PARTIALINFO_VALID;
            infoaddr = 0;
            infolen = 0;
        }
        else
        {
            headinfo.hookvalid = CN_BLACKBOX_PARTIALINFO_VALID;
        }
        FinalAction = __BlackBox_ResultMerge(ThrowAction, HookAction);

        if(FinalAction < EN_BLACKBOX_DEAL_RECORD)
        {
            //连记录都无需的话，那么就直接返回吧,简直开玩笑
            Result = EN_BLACKBOX_RESULT_IGNORE;
        }
        else
        {
            headinfo.BlackBoxAction = (u32)FinalAction;
            headinfo.hookinfolen = infolen;
            headinfo.HookAction = HookAction;
            recordpara.hookinfoaddr = infoaddr;
            recordpara.hookinfolen = infolen;

            //操作系统运行状态信息的搜集(如果搜集不成功，无关大局)
            __BlackBox_OsStateInfoGather(&infoaddr, &infolen);
            headinfo.osstatevalid = CN_BLACKBOX_PARTIALINFO_VALID;
            headinfo.osstateinfolen = infolen;
            headinfo.recordendian = CN_CFG_BYTE_ORDER;
            headinfo.magicnumber = CN_BLACKBOX_HEADINFO_MAGICNUMBER;

            recordpara.osstateinfoaddr = infoaddr;
            recordpara.osstateinfolen = infolen;

            //该搜集的信息都搜集完毕，那么就处理吧
            Result = __BlackBox_ExecAction(FinalAction, &recordpara);
        }
    }
    else
    {
        Result = EN_BLACKBOX_RESULT_IGNORE;
    }
    return Result;
}

// =============================================================================
// 函数功能: 信息头字节序转换
// 输入参数:blackboxinfo,异常信息
// 输出参数:无
// 返回值  :无
// 说明    :将blackboxinfo中已知信息转换字节序,name以前的字节必须是4字节对齐的
// =============================================================================
void  __BlackBox_HeadinfoSwapByEndian(struct BlackBoxHeadInfo *headinfo)
{
    u32 temp;
    u32 limit;
    u32 *addr;

    temp = (u32)headinfo;
    limit = (u32)(&(headinfo->decodername[0]));
    while(temp < limit)
    {
        addr = (u32 *)temp;
        *addr = swapl(*addr);
        temp = temp +4;
    }
}
// =============================================================================
// 函数功能: 异常信息头解析
// 输入参数:headinfo,异常信息头
// 输出参数:无
// 返回值  :true成功 false失败
// 说明    :内部调用
// =============================================================================
bool_t __BlackBox_HeadinfoDecoder(struct BlackBoxHeadInfo *headinfo)
{
    debug_printf("blackbox","blackbox_headinfo:magicnumber   :0x%08x\n\r",headinfo->magicnumber);
    debug_printf("blackbox","blackbox_headinfo:record endian :%s\n\r",\
            headinfo->recordendian==CN_CFG_LITTLE_ENDIAN?"LittleEndian":"BigEndian");
    debug_printf("blackbox","blackbox_headinfo:BlackBoxType       :0x%08x\n\r",headinfo->BlackBoxType);
    debug_printf("blackbox","blackbox_headinfo:osstatevalid  :%s\n\r",\
            headinfo->osstatevalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:osstateinfolen:%d Bytes\n\r",headinfo->osstateinfolen);
    debug_printf("blackbox","blackbox_headinfo:hookinfo stat :%s\n\r",\
            headinfo->hookvalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:hookinfolen   :%d Bytes\n\r",headinfo->hookinfolen);
    debug_printf("blackbox","blackbox_headinfo:HookAction    :%s\n\r",BlackBox_ActionName(headinfo->HookAction));
    debug_printf("blackbox","blackbox_headinfo:throwinfo stat:%s\n\r",\
            headinfo->throwinfovalid==CN_BLACKBOX_PARTIALINFO_VALID?"Valid":"Invalid");
    debug_printf("blackbox","blackbox_headinfo:throwinfolen  :%d Bytes\n\r",headinfo->throwinfolen);
    debug_printf("blackbox","blackbox_headinfo:ThrowAction   :%s\n\r",BlackBox_ActionName(headinfo->ThrowAction));
    debug_printf("blackbox","blackbox_headinfo:BlackBoxAction     :%s\n\r",BlackBox_ActionName(headinfo->BlackBoxAction));
    debug_printf("blackbox","blackbox_headinfo:DecoderName   :%s\n\r",headinfo->decodername);

    return true;
}

// =============================================================================
// 函数功能: 解析异常信息
// 输入参数:recordpara,异常信息
// 输出参数:无
// 返回值  :true成功 false失败
// 说明    :异常信息一定已经拷贝到了内存当中，或则可能会修改原信息
// =============================================================================
bool_t  BlackBox_InfoDecoder(struct BlackBoxRecordPara *recordpara)
{
    bool_t  result = false;
    u32  endian;
    struct BlackBoxHeadInfo *headinfo;
    struct BlackBoxThrowPara throwpara;
    if(NULL == recordpara)
    {
        result = false;
        debug_printf("blackbox","BlackBox_InfoDecoder:invalid parameter!\n\r");
    }
    else if(( 0 == recordpara->headinfoaddr ) || (sizeof(struct BlackBoxHeadInfo) != recordpara->headinfolen))
    {
        result = false;
        debug_printf("blackbox","BlackBox_InfoDecoder:incomplete exception headinfo!\n\r");
    }
    else
    {
        headinfo = (struct BlackBoxHeadInfo *)(recordpara->headinfoaddr);

        //用小端作为标准主要是因为小端是0，大小端都一样
        if(((headinfo->recordendian == CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER != CN_CFG_LITTLE_ENDIAN))||\
           ((headinfo->recordendian != CN_CFG_LITTLE_ENDIAN)&&(CN_CFG_BYTE_ORDER == CN_CFG_LITTLE_ENDIAN)))
        {
            //需要大小端转换
            __BlackBox_HeadinfoSwapByEndian(headinfo);
        }
        if(CN_BLACKBOX_HEADINFO_MAGICNUMBER != headinfo->magicnumber)
        {
            debug_printf("blackbox","BlackBox_InfoDecoder:headinfo has been destroyed!\n\r");
            result = false;
        }
        else
        {
            //重现抛出信息参数
            endian = headinfo->recordendian;
            throwpara.BlackBoxAction = headinfo->ThrowAction;
            throwpara.DecoderName = headinfo->decodername;
            throwpara.BlackBoxInfo = (u8 *)(recordpara->throwinfoaddr);
            throwpara.BlackBoxInfoLen = recordpara->throwinfolen;
            throwpara.BlackBoxType = headinfo->BlackBoxType;
            //信息头解析
            if(recordpara->headinfolen != sizeof(struct BlackBoxHeadInfo))
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:headinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_HeadinfoDecoder(headinfo);
            }
            //OS状态解析,解析器自己判断包的数据是否被修改
            if(recordpara->osstateinfolen != headinfo->osstateinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:osstateinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_OsStateInfoDecoder(recordpara->osstateinfoaddr,\
                                             recordpara->osstateinfolen, endian);
            }
            //抛出异常解析,解析器自己判断包的长度是否完整，包的内容是否被破坏
            if(recordpara->throwinfolen != headinfo->throwinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:throwinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_ThrowInfodecode(&throwpara,endian);
            }
            //HOOK解析,解析器自己判断包的长度是否完整，包的内容是否被破坏
            if(recordpara->hookinfolen != headinfo->hookinfolen)
            {
                debug_printf("blackbox","BlackBox_InfoDecoder:hookinfo incomplete!\n\r");
            }
            else
            {
                result = __BlackBox_HookInfoDecoder(&throwpara,recordpara->hookinfoaddr,\
                                             recordpara->hookinfolen, endian);
            }
        }
    }
    return result;
}

// =============================================================================
// 函数功能: 异常组件初始化,同时安装了硬件异常的信息解析函数。
// 输入参数:para
// 输出参数:无
// 返回值  :ptu32_t 暂时无定义
// =============================================================================
void ModuleInstall_BlackBox(void)
{

    extern bool_t BlackBox_ShellInit();
    BlackBox_ShellInit();

    dListInit(&BlackBoxDecoderListHead);

    // 初始化硬件异常信息解析函数,这里要特别说明一下,本着谁抛出异常谁解析的原则,
    // 例如看门狗异常,在ModuleInstall_Wdt函数中调用BlackBox_RegisterThrowinfoDecoder
    // 初始化看门狗的异常解析函数.硬件异常信息解析函数,也应该由初始化硬件异常的
    // 模块注册。但硬件异常不一样,它是在预加载时初始化的,而那时异常组件还没有准
    // 备好,故必须在这里初始化.
    HardExp_InfoDecoderInit( );
    s_bBlackBoxMoDuleInitState = true;

    return ;
}



static char *pStartModeName[]={
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
    u32      modesoft;   //defines by enStartMode
    u32      modehard;   //defines by enStartMode
}tagStartMode;
static tagStartMode gStartMode;

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
#define CN_ILLEGAL_StartNAME   "ILLEGALSTART"

//默认的启动方式的获取
static enStartMode GetSysStartModeDefault(void)
{
    return EN_Start_UNKNOWN;
}
//该变量用于存储软件提供的vm可能使用的内存空间
static tagVmMemItem *pMemLegalTab;
static fnGetStartMode fnGetModeHard = GetSysStartModeDefault;  //获取启动标志：硬件解决方案
static fnGetStartMode fnGetModeSoft = GetSysStartModeDefault;  //获取启动标志:软件解决方案

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
        start = (u8*)item->start;
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
bool_t ThrowSysStartInfo(enStartMode mode)
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
    parahead.DecoderName = CN_ILLEGAL_StartNAME;
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

    debug_printf("SysStart","DUMPHEAD:Decoder:%s Action:%d BlackBoxLen:%d BlackBoxType:%d Endian:%d\n\r",\
            exppara->DecoderName,exppara->BlackBoxAction,exppara->BlackBoxInfoLen,exppara->BlackBoxType,endian);
    dumpinfo = (tagSysDumpState *)exppara->BlackBoxInfo;
    debug_printf("SysStart","DUMPINFO:Reason:%s EcbRun:0x%08x EcbReady:0x%08x EcbDelay:0x%08x\n\r",\
            pStartModeName[dumpinfo->dumpreason],(u32)dumpinfo->eventrunning,\
            (u32)dumpinfo->eventready,(u32)dumpinfo->eventdelay);
    debug_printf("SysStart","EcbRunning:%s\n\r",dumpinfo->ecblegal?"LEGAL":"ILLEGAL");
    if(dumpinfo->ecblegal)
    {
        ecb = &dumpinfo->ecbRunning;
        debug_printf("SysStart","EcbPre:0X%08x EcbNxt:0x%08x EcbMpre:0x%08x EcbMnxt:0x%08x\n\r",\
                (u32)ecb->previous,(u32)ecb->next,(u32)ecb->multi_previous,(u32)ecb->multi_next);
        debug_printf("SysStart","EcbVmAddr:0x%08x\n\r",(u32)ecb->vm);
        debug_printf("SysStart","Param1:0x%08x Param2:0x%08x\n\r",ecb->param1,ecb->param2);
        debug_printf("SysStart","Sync:0x%08x SyncHead:0x%08x SyncCounter:0x%08x\n\r",(u32)ecb->sync,(u32)ecb->sync_head,ecb->sync_counter);
#if CFG_OS_TINY == false
    debug_printf("SysStart","EventStartTime:0x%llx  EventConsumeTime:0x%llx\n\r",ecb->EventStartTime,ecb->consumed_time);
    debug_printf("SysStart","ConsumedTimeSecond:0x%08x ComsumeTimeRecord:0x%08x\n\r",ecb->consumed_time_record,ecb->consumed_time_record);
#endif  //CFG_OS_TINY == false
    debug_printf("SysStart","DelayStartTick:0x%llx DelayEndTick:0x%llx\n\r",ecb->delay_start_tick,ecb->delay_end_tick);
    debug_printf("SysStart","ErrNo:%d EventResult:0x%08x\n\r",ecb->error_no,ecb->event_result);
    debug_printf("SysStart","WaitMemSize:%d\n\r",ecb->wait_mem_size);
    debug_printf("SysStart","WakeupFrom:0x%08x Status:0x%08x\n\r",ecb->wakeup_from,ecb->event_status);
    debug_printf("SysStart","Prior:%d\n\r",ecb->prio);
#if CFG_OS_TINY == false
        debug_printf("SysStart","EvttID:%d Name:%s\n\r",(ecb->evtt_id&(~CN_EVTT_ID_MASK)),\
                (ecb->evtt_id&(~CN_EVTT_ID_MASK)) < CFG_EVENT_TYPE_LIMIT?\
                        g_tEvttTable[(ecb->evtt_id&(~CN_EVTT_ID_MASK))].evtt_name:"unknown");
#else
        debug_printf("SysStart","EvttID:%d Name:%s\n\r",(ecb->evtt_id&(~CN_EVTT_ID_MASK)),"unknown");
#endif  //CFG_OS_TINY == false

        debug_printf("SysStart","EventID:%d LocalMem:%d\n\r",ecb->event_id,ecb->local_memory);
        debug_printf("SysStart","EcbStack:%s\n\r",dumpinfo->vmlegal?"LEGAL":"ILLEGAL");

        if(dumpinfo->vmlegal)
        {
            vm = ecb->vm;
            debug_printf("SysStart","Stack:0x%08x StackTop:0x%08x StackSize:%d Nxt:0x%08x HostVm:0x%08x\n\r",\
                    (u32)vm->stack,(u32)vm->stack_top,vm->stack_size,(u32)vm->next,(u32)vm->host_vm);
        }
    }
    debug_printf("SysStart","DUMPEND\n\r");
    return true;
}


#define CN_STARTLOG_NAME   "SysStartLOG"
static void __ShowStartMode(tagStartMode *startmsg)
{
    time_t  logtime;
    u32      startreason;
    logtime = startmsg->time;
    debug_printf("SysStart","BootTime:%s\n\r",ctime(&logtime));
    //SOFT CHECK
    startreason = startmsg->modesoft;
    debug_printf("SysStart","BootMode(SoftCheck):%s\n\r",startreason<EN_Start_LAST?pStartModeName[startreason]:"UNKNOWN");
    //HARD CHECK
    startreason = startmsg->modehard;
    debug_printf("SysStart","BootMode(HardCheck):%s\n\r",startreason<EN_Start_LAST?pStartModeName[startreason]:"UNKNOWN");
}

//static bool_t __BootMsgShowShell(char *param)
bool_t startmsg(char *param)
{
    tagStartMode *msg;

    msg = &gStartMode;
    __ShowStartMode(msg);

    return true;
}

static bool_t __StartModeDecoder(struct BlackBoxThrowPara *exppara,u32 endian)
{
    tagStartMode msg;
    debug_printf("SysStart","DUMPHEAD:Decoder:%s Action:%d BlackBoxLen:%d BlackBoxType:%d Endian:%d\n\r",\
            exppara->DecoderName,exppara->BlackBoxAction,exppara->BlackBoxInfoLen,exppara->BlackBoxType,endian);
    memcpy(&msg,exppara->BlackBoxInfo,sizeof(msg));
    __ShowBootMode(&msg);
    return true;
}


//use this function to analyze the boot mode
static bool_t __SysStartModeLog(void)
{
    gStartMode.time = time(NULL);
    //通过硬件分析
    if(NULL != fnGetModeHard)
    {
        gStartMode.modehard = fnGetModeHard();
    }
    else
    {
        gStartMode.modehard = EN_Start_NOTIMPLEMENT;
    }
    //通过软件分析
    if(NULL != fnGetModeSoft)
    {
        gStartMode.modesoft = fnGetModeSoft();
    }
    else
    {
        gStartMode.modesoft = EN_Start_NOTIMPLEMENT;
    }
    //record the boot message
    struct BlackBoxThrowPara  parahead;
    parahead.BlackBoxAction = EN_BLACKBOX_DEAL_RECORD;
    parahead.DecoderName = CN_STARTLOG_NAME;
    parahead.BlackBoxInfo = (u8 *)&gStartMode;
    parahead.BlackBoxInfoLen =sizeof(gStartMode);
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
bool_t ModuleInstall_SysStartReason(const tagVmMemItem *tab[],fnGetStartMode getmodehard,
                            fnGetStartMode getmodesoft)
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
    dumpdecoder.DecoderName = CN_ILLEGAL_StartNAME;
    dumpdecoder.MyDecoder = __CpuDumpDecoder;
    BlackBox_RegisterThrowInfoDecoder(&dumpdecoder);

    memset(&bootdecoder,0,sizeof(bootdecoder));
    bootdecoder.DecoderName = CN_STARTLOG_NAME;
    bootdecoder.MyDecoder = __StartModeDecoder;
    BlackBox_RegisterThrowInfoDecoder(&bootdecoder);

    //记录系统启动状态
    __SysStartModeLog();
    return result;

}
#if CFG_MODULE_ENABLE_BLACK_BOX==true
ADD_TO_ROUTINE_SHELL(startmsg,startmsg,"usage:bootmsg");
#endif
