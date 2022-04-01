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
// =============================================================================

// 模块描述: 软看门狗实现
// 模块版本: V1.00
// 创建人员: zhangqf_cyg
// 创建时间: 11:45:07 AM/Sep 20, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// 1,在看门狗监控主任务启动之前，所有的看门狗API都是直接操作看门狗队列
// 2,在看门狗监控主任务启动之后，所有的看门狗API都是直接发送给看门狗监控主任务进行
// =============================================================================
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

#include <os.h>
#include <wdt_soft.h>
#include <blackbox.h>
#include "dbug.h"
#include <shell.h>
#include "board-config.h"
#include "component_config_wdt.h"


#define CN_WDT_YIP_NEVER      CN_LIMIT_SINT64     //当timeout为该时间时表示该看门狗暂停状态
#define CN_WDT_YIP_PRECISION  CN_CFG_TICK_US      //软件看门狗模块的狗叫时间精度
#define CN_WDTMSG_LIMIT       5                   //看门狗操作消息队列长度
//struct Wdt;
//typedef u32 (* fnYipHook)(struct Wdt *wdt);
//the struct of the wdt
struct Wdt
{
    struct Wdt *ppre;        //双向不循环链表指针-前
    struct Wdt *pnxt;        //双向不循环链表指针-后
    char            *pname;       //看门狗名字，指向静态或者常量字符串
    fnYipHook       fnhook;       //狗叫善后钩子函数
    enum EN_BlackBoxAction action;       //狗叫动作
    u32             cycle;        //看门狗周期，单位：微秒
    u16             WdtOnwer;     //看门狗所属事件ID
    s64             deadtime;     //看门狗喂狗截止时间，到此时间还不喂，则狗叫，单位：微秒
    s64             runtime;      //上次操作看门狗时看门狗所属任务的运行时间，单位：微秒
    u32             timeoutreason;     //看门狗狗叫原因
    u16             shyiptimes;        //调度原因引起的看门狗狗叫次数   ，单位：次
    //用于分辨狗叫原因，如果发生看门狗溢出，在该狗叫周期内，若当事件执行时间大于
    //ExhaustLevelSet（uS）,则判定为逻辑错误，即被监控代码有时间运行却不喂狗；若
    //事件执行时间小于ExhaustLevelSet（uS），则判定为调度问题，例如高优先级事件
    //或者中断长时间占用CPU，导致被监控代码没有机会执行。
    u32             ExhaustLevelSet;
    //因ExhaustLevelSet而导致狗叫的容忍次数。如果因ExhaustLevelSet而导致狗叫，
    //说明狗叫并非被监控代码的逻辑错误引起，可以用本函数设定容许次数，，超过
    //该限制则执行fnhook，否则不执行。
    //本参数可用于容许IO延迟，具体如下：
    //1、假定被监视代码正常时1秒必定会执行一次，则cycle可设为1.2S(留点余量）
    //2、但被监控代码在某特定条件下（if语句），有执行IO操作，该IO操作可能耗时
    //   5秒。则可在执行IO操作前，把ExhaustLimit设为5（用EN_WDTCMD_SETSCHLEVEL命令）
    u32             ExhaustLimit;
};

//软件看门狗的消息类型
typedef struct
{
    tagWdt       *pwdt;         //待操作的看门狗
    u32           opcode;       //操作类型
    ptu32_t       para;         //操作类型指定的参数
}tagWdtMsg;

#define CN_WDTEXP_NAMELEN_LIMIT 16
//看门狗异常信息组织结构
struct WdtExpInfo
{
    char       wdtname[CN_WDTEXP_NAMELEN_LIMIT];    //异常的看门狗的名字
    tagWdt     wdt;                              //异常的看门狗
};

//该模块使用的静态全局变量
static struct MemCellPool         *ptWdtPool = NULL;    //看门狗内存池
static tagWdt                     *ptWdtHead = NULL;    //看门狗队列头
static tagWdt                     *ptWdtTail = NULL;    //看门狗队列尾
static tagWdt                     *ptWdtHard = NULL;    //硬件看门狗对应的软件看门狗
static bool_t (*fnWdtHardFeed)(void) = NULL;            //硬件看门狗的喂狗方法
static struct MsgQueue            *ptWdtMsgBox = NULL;  //看门狗消息队列

//看门狗狗叫的原因：
//正常的看门狗不会叫，只有没有按时喂的狗才会叫；因此看门狗叫是因为没有按时喂
//没有按时喂分为以下几种：
//1,有时间却不喂，即看门狗所属任务有足够的运行时间却不去喂狗，这种我们称之为逻辑原因
//2,没有时间喂，即看门狗所属任务得不到足够的运行时间导致任务完不成而没有按时喂狗，这种我们
//            称之为调度原因；
//3,其他原因，即没有指定看门狗所属任务，这种情况看门狗叫就叫了，但是莫名原因
enum __EN_WDTYIP
{
    EN_WDT_NOYIP          = 0,     //看门狗正常没有叫
    EN_WDT_YIPFORLOGIC,            //逻辑错误引起的看门狗狗叫
    EN_WDT_YIPFORSHEDULE,          //调度原因引起的看门狗狗叫
    EN_WDT_YIPOTHERS,              //未知原因引起的看门狗狗叫
    EN_WDT_YIPLENTH,               //未知原因引起的看门狗狗叫
};
#define CN_WDT_EXPDECODERNAME   "WdtExp Decoder"    //看门狗异常信息解析器名称

//获取YIP原因
const char *gWdtYipReason[EN_WDT_YIPLENTH]={

        "NOYIP",
        "YIPFORLOGIC",
        "YIPFORSHEDULE",
        "YIPOTHERS"
};
const char *WdtYipReasonName(enum __EN_WDTYIP  yipreason)
{
    if(yipreason < EN_WDT_YIPLENTH)
    {
        return gWdtYipReason[yipreason];
    }
    else
    {
        return NULL;
    }
}
// =============================================================================
// 函数功能：抛出具体的看门狗异常
// 输入参数：result, 看门狗的异常时需要做的处理
//        wdt, 异常的看门狗
// 输出参数：
// 返回值  ：最终的处理结果
// 说明    ：主要是搜集WDT异常时的信息，该函数仅限内部调用，当发现看门狗异常并且需要处理该
//        异常的时候调用该函数
// =============================================================================
enum EN_BlackBoxAction __Wdt_TrowWdtExp(enum EN_BlackBoxAction WdtAction,
                                            tagWdt *wdt)
{
    struct BlackBoxThrowPara  parahead;
    struct WdtExpInfo wdtexp;
    wdtexp.wdt = *wdt;
    memcpy(wdtexp.wdtname,wdt->pname,CN_WDTEXP_NAMELEN_LIMIT);
    parahead.DecoderName = CN_WDT_EXPDECODERNAME;
    parahead.BlackBoxAction = WdtAction;
    parahead.BlackBoxInfo = (u8 *)&wdtexp;
    parahead.BlackBoxInfoLen = sizeof(wdtexp);
    parahead.BlackBoxType = CN_BLACKBOX_TYPE_WDT;
    return BlackBox_ThrowExp(&parahead);
}

// =============================================================================
// 函数功能：WDT异常信息字节序转换
// 输入参数：wdt, 待转换的看门狗,
//          endian, 大端还是小端
//          CN_CFG_LITTLE_ENDIAN/CN_CFG_BIG_ENDIAN
// 输出参数：wdt, 转换后的看门狗
// 返回值  ：无
// 说明    ：由于异常信息是按照异常时的机器的本地端格式存储的，如果发生异常的机器和异常信息
//        的机器的大小端不一致，那么就需要转换；该操作是为异常存储机器和异常解析机器不同而
//        考虑
// =============================================================================
void __Wdt_SwapWdtInfoByEndian(tagWdt *wdt, u32 endian)
{
    //大小端转换
    if(CN_CFG_BYTE_ORDER != endian)
    {
        wdt->action = swapl(wdt->action);
        wdt->cycle =  swapl(wdt->cycle);
        wdt->ExhaustLevelSet = swapl(wdt->ExhaustLevelSet);
        wdt->ExhaustLimit = swaps(wdt->ExhaustLimit);
        wdt->shyiptimes = swaps(wdt->shyiptimes);
        wdt->WdtOnwer = swapl(wdt->WdtOnwer);
        wdt->timeoutreason = swapl(wdt->timeoutreason);
    }
    return;
}
// =============================================================================
// 函数功能：看门狗异常信息解析
// 输入参数：wdtinfo,看门狗异常信息
//          endian, 大小端
// 输出参数：
// 返回值  ：
// 说明    ：注册异常解析器的时候会将该异常解析器注册进去
// =============================================================================
bool_t __Wdt_WdtExpInfoDecoder(struct BlackBoxThrowPara  *WdtinfoHead,u32 endian)
{
    tagWdt  *wdt;
    struct WdtExpInfo *wdtinfo = (struct WdtExpInfo *)WdtinfoHead->BlackBoxInfo;
    wdt = &wdtinfo->wdt;
    __Wdt_SwapWdtInfoByEndian(wdt, endian);
    debug_printf("wdtinfo","name               :%s\n\r",    wdtinfo->wdtname);
    debug_printf("wdtinfo","Owner              :0x%04x\n\r",wdt->WdtOnwer);
    debug_printf("wdtinfo","Action             :%s\n\r",BlackBox_ActionName(wdt->action));
    debug_printf("wdtinfo","Cycle              :%d(us)\n\r",wdt->cycle);
    debug_printf("wdtinfo","Reason             :%s\n\r",WdtYipReasonName(wdt->timeoutreason));
    debug_printf("wdtinfo","OwnerTimeLevel     :0x%08x(us)\n\r",wdt->ExhaustLevelSet);
    debug_printf("wdtinfo","TimeoutForShedule  :0x%08x\n\r",wdt->shyiptimes);
    debug_printf("wdtinfo","SheduleTimeoutLimit:0x%08x\n\r",wdt->ExhaustLimit);
    return true;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

// =============================================================================
// 函数功能：硬件看门狗狗叫善后函数
// 输入参数：wdt，待操作的看门狗
// 输出参数：
// 返回值  ：
// 说明    ：主要是对注册的硬件看门狗的HOOK进行包装;更重要的是它的返回值是忽略，避免系统因
//        硬件看门狗对应的软件看门狗狗叫而动作
// =============================================================================
static u32 __Wdt_HardWdtYipHook(tagWdt *wdt)
{
    if(NULL != fnWdtHardFeed)//有硬件狗则执行喂狗
    {
        fnWdtHardFeed();
    }
    return EN_BLACKBOX_DEAL_IGNORE;
}
// =============================================================================
// 函数功能：将看门狗添加进队列中，该看门狗已经设置好了deadline时间
// 输入参数：wdt，待操作的看门狗(该看门狗已经设置好了狗叫时间)
// 输出参数：
// 返回值  ：
// 说明    ：该看门狗事先一定不在队列中
// =============================================================================
static void __Wdt_Add2Queque(tagWdt *wdt)
{
    tagWdt *wdtcomp;
    s64    deadline;
    if(NULL == ptWdtHead)
    {
        ptWdtHead = wdt;
        ptWdtTail = wdt;
        wdt->pnxt = NULL;
        wdt->ppre = NULL;
    }
    else if(wdt->deadtime == CN_WDT_YIP_NEVER) //the wdt to add is pause
    {
        ptWdtTail->pnxt = wdt;
        wdt->ppre = ptWdtTail;
        wdt->pnxt = NULL;
        ptWdtTail = wdt;
    }
    else //compare each the deadline of wdt of the queue
    {
        deadline = wdt->deadtime;
        wdtcomp = ptWdtHead;
        while((wdtcomp != NULL)&&(wdtcomp->deadtime < deadline))
        {
            wdtcomp = wdtcomp->pnxt;
        }
        if(NULL == wdtcomp)
        {
            //no deadline is bigger than the one to add, so add it to the tail
            ptWdtTail->pnxt = wdt;
            wdt->ppre = ptWdtTail;
            wdt->pnxt = NULL;
            ptWdtTail = wdt;
        }
        else
        {
            //there exist one whose deadline is bigger than the one to add
            if(wdtcomp == ptWdtHead)
            {
                wdt->pnxt = wdtcomp;
                wdt->ppre = NULL;
                wdtcomp->ppre = wdt;
                ptWdtHead = wdt;
            }
            else
            {
                wdtcomp->ppre->pnxt = wdt;
                wdt->ppre = wdtcomp->ppre;
                wdtcomp->ppre = wdt;
                wdt->pnxt = wdtcomp;
            }
        }
    }
}
// =============================================================================
// 函数功能：从看门狗队列中移除看门狗
// 输入参数：wdt，待操作的看门狗
// 输出参数：
// 返回值  ：
// 说明    ：该看门狗务必在队列中
// =============================================================================
static void __Wdt_RemovefQueue(tagWdt *wdt)
{
    if(wdt == ptWdtHead)  //the head to remove
    {
        if(NULL == wdt->pnxt)
        {
            //only one wdt in the queue
            ptWdtHead = NULL;
            ptWdtTail = NULL;
        }
        else
        {
            //only the head
            ptWdtHead = wdt->pnxt;
            wdt->pnxt->ppre = NULL;
        }
    }
    else if(wdt == ptWdtTail) // the tail to remove
    {
        ptWdtTail = wdt->ppre;
        wdt->ppre->pnxt = NULL;
    }
    else
    {
        wdt->ppre->pnxt = wdt->pnxt;
        wdt->pnxt->ppre = wdt->ppre;
    }
    wdt->ppre = NULL;
    wdt->pnxt = NULL;
}
// =============================================================================
// 函数功能：处理传递给监视任务的消息指示
// 输入参数：msg, 待处理参数，ostime，当前系统时间
// 输出参数：
// 返回值  ：
// 说明    ：
// =============================================================================
static void __Wdt_DealMsg(tagWdtMsg *msg)
{
    u32                  opcode;
    ptu32_t              oppara;
    tagWdt               *wdt;
    struct EventInfo  eventinfo;
    s64                  ostime;

    wdt = msg->pwdt;
    opcode = msg->opcode;
    oppara = msg->para;
    if(NULL != wdt)
    {
        //更新WDT线程拥有者最后一次操作WDT时间
        if(CN_EVENT_ID_INVALID != wdt->WdtOnwer)
        {
            if(DJY_GetEventInfo(wdt->WdtOnwer, &eventinfo))
            {
#if (CN_USE_TICKLESS_MODE)
                wdt->runtime = eventinfo.consumed_cnt;
#else
                wdt->runtime = eventinfo.consumed_time;
#endif
            }
        }
        //清空连续狗叫成员
        wdt->shyiptimes = 0;
        wdt->timeoutreason = EN_WDT_NOYIP;

        ostime = DJY_GetSysTime();
        switch (opcode)
        {
            case EN_WDTCMD_ADD:
            {
                //add an new wdt to the queue
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_DEL:
            {
                __Wdt_RemovefQueue(wdt);
                break;
            }
            case EN_WDTCMD_PAUSE:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = CN_WDT_YIP_NEVER;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_RESUME:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_CLEAN:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->deadtime = wdt->cycle + ostime;
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_OWNER:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->WdtOnwer = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_CYCLE:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->cycle = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_YIP_ACTION:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->action = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_HOOK:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->fnhook = (fnYipHook)oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_CONSUME_LEVEL:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->ExhaustLevelSet = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            case EN_WDTCMD_SET_SCHED_LEVEL:
            {
                __Wdt_RemovefQueue(wdt);
                wdt->ExhaustLimit = oppara;
                if(wdt->deadtime != CN_WDT_YIP_NEVER)
                {
                    wdt->deadtime = wdt->cycle + ostime;
                }
                __Wdt_Add2Queque(wdt);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return;
}
// =============================================================================
// 函数功能：看门狗狗叫结果处理
// 输入参数：result，异常处理结果，wdt，异常的看门狗
// 输出参数：
// 返回值  ：
// 说明    ：只有需要记录或者重启的动作才会真正的抛出异常
// =============================================================================
static void __Wdt_DealWdtYipResult(enum EN_BlackBoxAction result, tagWdt *wdt)
{
    //befor we do the wdt exception, we'd better feed the hard wdt to avoid
    //reseting during the dealing
    if(result > EN_BLACKBOX_DEAL_DEFAULT)
    {
        __Wdt_HardWdtYipHook(NULL);
        __Wdt_TrowWdtExp(result,wdt);
    }
}
// =============================================================================
// 函数功能：分析看门狗狗叫原因
// 输入参数：wdt，待操作的看门狗
// 输出参数：
// 返回值  ：无
// 说明    ：目前仅仅考虑了任务调度延时或者自己出现逻辑错误；如果没有指定任务，则认为是others
// 判断标准：
//         任务从上次操作看门狗开始到被调度剥离运行最后一条指令的时间(time)和用户
//         设定的标准（level）相比较
//         1, time> level,属于任务有足够时间运行却没有喂狗，属于逻辑错误（自己看门
//             狗时间设置错误也属于此类），一般的属于局域while(1)现象，
//             返回EN_WDT_YIPFORLOGIC
//         2, time <level,属于任务没有获取足够的运行时间导致的，属于被调度切离时间
//             太久远，返回EN_WDT_YIPFORSHEDULE
//         3,  获取任务时间失败则认为是OTHERS
// =============================================================================
static void __Wdt_AnalyzeYipReason(tagWdt *wdt)
{
    u32    Tm_TimeRun;
    struct EventInfo wdt_event_info;

    if( DJY_GetEventInfo((u16)wdt->WdtOnwer, &wdt_event_info))
    {
#if (CN_USE_TICKLESS_MODE)
        Tm_TimeRun = (u32)(wdt_event_info.consumed_cnt - wdt->runtime);
#else
        Tm_TimeRun = (u32)(wdt_event_info.consumed_time - wdt->runtime);
#endif
        if(Tm_TimeRun < wdt->ExhaustLevelSet) //运行时间不够
        {
            wdt->timeoutreason = EN_WDT_YIPFORSHEDULE;
            wdt->shyiptimes++;

        }
        else //时间够了却不喂狗，这个是什么心态
        {
            wdt->timeoutreason = EN_WDT_YIPFORLOGIC;
        }
#if (CN_USE_TICKLESS_MODE)
        wdt->runtime = wdt_event_info.consumed_cnt;
#else
        wdt->runtime = wdt_event_info.consumed_time;
#endif
    }
    else
    {
        wdt->timeoutreason = EN_WDT_YIPOTHERS;   //莫名的异常
    }
    return;
}
// =============================================================================
// 函数功能：扫描整个看门狗队列并处理超时的看门狗
// 输入参数：
// 输出参数：
// 返回值  ：
// 说明    ：
// =============================================================================
static void __Wdt_ScanWdtQueque(void)
{
    s64             timenow;
    enum EN_BlackBoxAction result;
    tagWdt          *wdt;

    wdt = ptWdtHead;
    while(NULL != wdt)//处理所有在叫的狗
    {
        timenow = DJY_GetSysTime();//实时更新时间

        if((timenow + CN_WDT_YIP_PRECISION)>= wdt->deadtime) // the wdt has been timeout
        {
//            debug_printf("WDT","看门狗狗叫：%s\n\r",wdt->pname);
            __Wdt_AnalyzeYipReason(wdt);
            if(NULL !=  wdt->fnhook)
            {
                result = wdt->fnhook(wdt);//尽情的叫吧，少年
                if(result == EN_BLACKBOX_DEAL_DEFAULT)
                {
                    result = wdt->action;
                }
            }
            else
            {
                result = wdt->action;
            }
            if((wdt->timeoutreason == EN_WDT_YIPFORSHEDULE)
                            && (wdt->shyiptimes < wdt->ExhaustLimit)) //在忍耐限度以内
            {
                result = EN_BLACKBOX_DEAL_RECORD;
            }
            //this wdt should be relocate in the queue
//            printk("wdt:%s timeout\r\n", wdt->pname);
            __Wdt_DealWdtYipResult(result,wdt);
            __Wdt_RemovefQueue(wdt);
            wdt->deadtime = wdt->cycle + timenow;
            __Wdt_Add2Queque(wdt);
            //ok, do another wdt
            wdt = ptWdtHead;
        }
        else
        {
            //the head is not timeout, so we believe that all others are all not
            //time out
            wdt = NULL;
        }
    }
}

// =============================================================================
// 函数功能：看门狗监视任务
// 输入参数：
// 输出参数：
// 返回值  ：
// 说明    ：
// =============================================================================
static ptu32_t Wdt_Service(void)
{
    s64           timenow;
    tagWdt        *wdt;
    u32           waittime;
    tagWdtMsg     wdtmsg;
    // deal all the msg cached in the msgbox
    timenow = DJY_GetSysTime();
    while(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),0))
    {
        __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
    }

    //now we  come into the never get out loop, deal all the msg and timeout wdt
    while(1)
    {
        //preapara the wait time
        wdt = ptWdtHead;
        if((NULL == wdt)||(wdt->deadtime == CN_WDT_YIP_NEVER))
        {
            waittime = CN_TIMEOUT_FOREVER;
        }
        else
        {
            timenow = DJY_GetSysTime();
            waittime = (u32)(wdt->deadtime - timenow);
        }
        //ok, now we wait the msg box during the expect time
        if(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),waittime))
        {
            //must look out that:there maybe more than one msgs in the msgbox
            //some app has send some msg to the
            __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
            // deal all the msg cached in the msgbox
            while(MsgQ_Receive(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),0))
            {
                __Wdt_DealMsg(&wdtmsg); //all the wdt will be queued in the wdt queue
            }
        }
        //scan the queue to deal the timeout wdt
        __Wdt_ScanWdtQueque();
    }

    return 1; //if no exp, this could not be reached
}

//static bool_t wdtshow(char *param)

bool_t wdtshow(char *param)
{
    u8 i = 0;
    tagWdt *wdt;
    wdt = ptWdtHead;
    debug_printf("WDT","\n\r");
    debug_printf("WDT","%-4s%-16s%-9s%-9s%-9s%-9s%-9s\n\r",
            "No","NAME","hook    ","ACTION","CYCLE","OWNER","DEADLINE");
    while(NULL != wdt)
    {
        debug_printf("WDT","%-4d%-16s%-8x %-8x %-8x %-8x %llx\n\r",\
                i++ ,wdt->pname,(ptu32_t)wdt->fnhook,wdt->action,wdt->cycle,wdt->WdtOnwer,wdt->deadtime);
        wdt = wdt->pnxt;
    }
    return true;
}

#pragma GCC diagnostic pop

// =============================================================================
// 函数功能：看门狗模块的初始化
// 输入参数：
// 输出参数：
// 返回值  ：true = 成功  false = 失败
// 说明    ：创建看门狗硬件对应的软看门狗。注册看门狗异常信息解析器
// =============================================================================
bool_t ModuleInstall_Wdt(void)
{
    static struct BlackBoxInfoDecoder WdtDecoder;
    static tagWdt wdtpoolbuf[CFG_WDT_LIMIT];
    u16     evttid;

    ptWdtPool = Mb_CreatePool(wdtpoolbuf,CFG_WDT_LIMIT,sizeof(tagWdt),0,0,"wdt pool");
    //init the queue
    ptWdtHead = NULL;
    ptWdtTail = NULL;
    ptWdtHard = NULL;

    //create the msg box for the api to snd msg to the wdt service task
    ptWdtMsgBox = MsgQ_Create(CN_WDTMSG_LIMIT,sizeof(tagWdtMsg),CN_MSGQ_TYPE_FIFO);

    //create the main service
    evttid = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_WDT,0,0,Wdt_Service,
                                NULL,0x400,"wdt service");
    if(evttid == CN_EVTT_ID_INVALID)
        return false;
    if( DJY_EventPop(evttid,NULL,0,0,0,0) == CN_EVENT_ID_INVALID)
    {
        debug_printf("WDT","POP SERVICE FAILED!\n\r");
        DJY_EvttUnregist(evttid);
        return false;
    }

    WdtDecoder.MyDecoder = __Wdt_WdtExpInfoDecoder;
    WdtDecoder.DecoderName = CN_WDT_EXPDECODERNAME;
    if(false ==BlackBox_RegisterThrowInfoDecoder(&WdtDecoder))
    {
        debug_printf("WDT","Register Wdt Exp Decoder Failed!\n\r");
    }

    debug_printf("WDT","Init end ...\n\r");
    return true;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////下述函数提供给使用看门狗的用户//////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//备注：所有的用户API函数，当操作的WDT不涉及到看门狗的队列头时，修改队列的操作就会直接
//      运行用户的上下文当中，否则就发参数给service处理，由service进行队列的操作
//      看门狗模块未启动之前，是直接操作看门狗队列的。

// =============================================================================
// 函数功能：创建看门狗
// 输入参数：
//          dogname,看门狗名字
//          yip_cylce,狗叫周期，单位为微秒
//          yiphook,狗叫善后函数指针
//          yip_action,狗叫动作，当yiphook为空或者返回default时采用此值
//          ExhaustLevelSet，参看 tagWdt 中ExhaustLevelSet的说明
//          ExhaustLimit，参看 tagWdt 中ExhaustLimit的说明
// 输出参数：
// 返回值  ：创建的虚拟看门狗，NULL表示失败
// 说明    ：务必保证看门狗善后函数返回值的定义符合标准，见enum _EN_BLACKBOX_DEAL_TYPE
//           的声明（exp_api.h文件）
//           创建的看门狗若成功，则下次狗叫时间为当前时间+yip_cycle=time
// =============================================================================
tagWdt *Wdt_Create(char *dogname,u32 yip_cycle,\
                   fnYipHook yiphook,
                   enum EN_BlackBoxAction yip_action,
                   u32 ExhaustLevelSet,
                   u32 ExhaustLimit)
{
    tagWdt  *result;
    tagWdt  *wdt;

    result = NULL;
    wdt = Mb_Malloc(ptWdtPool,0);
    if(NULL != wdt)
    {
        result = Wdt_Create_s(wdt,dogname,yip_cycle,yiphook,yip_action,
                              ExhaustLevelSet,ExhaustLimit);
        if(NULL == result)
        {
            Mb_Free(ptWdtPool,wdt);
        }
    }
    return result;
}
// =============================================================================
// 函数功能：创建看门狗，wdt控制块使用用户提供的。
// 输入参数：
//          wdt,用户自己提供的存储空间
//          dogname,看门狗名字
//          yip_cylce,狗叫周期，单位为微秒
//          yiphook,狗叫善后函数指针
//          yip_action,狗叫动作，当yiphook为空或者返回default时采用此值
//          ExhaustLevelSet，参看 tagWdt 中ExhaustLevelSet的说明
//          ExhaustLimit，参看 tagWdt 中ExhaustLimit的说明
// 输出参数：
// 返回值  ：创建的虚拟看门狗，NULL表示失败
// 说明    ：务必保证看门狗善后函数返回值的定义符合标准，见enum _EN_BLACKBOX_DEAL_TYPE
//           的声明（exp_api.h文件）
//           创建的看门狗若成功，则下次狗叫时间为当前时间+yip_cycle
// =============================================================================
tagWdt *Wdt_Create_s(tagWdt *wdt, char *dogname,u32 yip_cycle,
                     fnYipHook yiphook,
                     enum EN_BlackBoxAction yip_action,
                     u32 ExhaustLevelSet,
                     u32 ExhaustLimit)
{

    tagWdt     *result;
    tagWdtMsg  wdtmsg;

    result = NULL;
    if((NULL != wdt)&&( 0 != yip_cycle))
    {
        memset((u8 *)wdt, 0, sizeof(tagWdt));
        wdt->pname = dogname;
        wdt->cycle = yip_cycle;
        wdt->fnhook = yiphook;
        wdt->action = yip_action;
        wdt->ExhaustLevelSet = ExhaustLevelSet;
        wdt->ExhaustLimit = ExhaustLimit;
        wdt->WdtOnwer = DJY_GetMyEventId( );

        //snd the msg to the service task
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = EN_WDTCMD_ADD;
        wdtmsg.para = 0;
        if(MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg, sizeof(tagWdtMsg),CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL))
        {
            result = wdt;
        }
        else
        {
            memset((u8 *)wdt, 0, sizeof(tagWdt));
        }
    }

    return result;
}

// =============================================================================
// 函数功能：获取看门狗控制块指针
// 输入参数:wat_name：想获取的看门狗名字
// 输出参数：NULL：未找到指定看门狗，其它：找到看门狗的地址
// 返回值  ：true成功，false失败
// 说明    ：和Wdt_Create成对调用
// =============================================================================
tagWdt *Wdt_Get(char *wdt_name)
{
    tagWdt *ret = NULL;

    if(ptWdtHead != NULL)
    {
        ret = ptWdtHead;
        while(ret != NULL)
        {
            if(strcmp(ret->pname, wdt_name) == 0)
            {
                break;
            }
            ret = ret->pnxt;
        }

    }

    return (ret);
}

// =============================================================================
// 函数功能：删除看门狗
// 输入参数:wdt,待删除的看门狗
// 输出参数：
// 返回值  ：true成功，false失败
// 说明    ：和Wdt_Create成对调用
// =============================================================================
bool_t Wdt_Delete(tagWdt *wdt)
{
    bool_t result = false;
    if(NULL != wdt)
    {
        result = Wdt_Delete_s(wdt);
        if(result)
        {
            Mb_Free(ptWdtPool,wdt);
        }
    }

    return result;
}
// =============================================================================
// 函数功能：删除看门狗
// 输入参数：wdt,待删除的看门狗
// 输出参数：
// 返回值  ：true成功，false失败
// 说明    ：和Wdt_Create_s成对调用
// =============================================================================
bool_t Wdt_Delete_s(tagWdt *wdt)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;

    result = false;
    if(NULL != wdt)
    {
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = EN_WDTCMD_DEL;
        wdtmsg.para = 0;
        result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                           CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }

    return result;
}
// =============================================================================
// 函数功能：复位看门狗
// 输入参数：wdt,待操作的看门狗
// 输出参数：
// 返回值  ：true success while false failed
// 说明    ：
// =============================================================================
bool_t Wdt_Clean(tagWdt *wdt)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;
    atom_low_t   atom;
    s64          ostime;

    result = false;
    if(NULL != wdt)
    {
        atom = Int_LowAtomStart();
        if(ptWdtHead != wdt)
        {
            //可以直接修改wdtqueue
            ostime = DJY_GetSysTime();
            __Wdt_RemovefQueue(wdt);
            wdt->deadtime = wdt->cycle + ostime;
            wdt->shyiptimes = 0;
            __Wdt_Add2Queque(wdt);
            Int_LowAtomEnd(atom);
        }
        else
        {
            //snd msg to the monitor task
            Int_LowAtomEnd(atom);
            wdtmsg.pwdt = wdt;
            wdtmsg.opcode = EN_WDTCMD_CLEAN;
            wdtmsg.para = 0;
            result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                               CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
        }

    }
    return result;
}
// =============================================================================
// 函数功能：看门狗的属性设置
// 输入参数：wdt,待操作的看门狗
//          type,操作类型，参见enum EN_WDTCMD的定义
//          para,随着type的不同而不同
// 输出参数：
// 返回值  ：操作完毕的看门狗
// 说明    ：操作码只能是EN_WDTCMD_PAUSE及其以下
// =============================================================================
bool_t Wdt_Ctrl(tagWdt *wdt, u32 type, ptu32_t para)
{
    bool_t       result;
    tagWdtMsg    wdtmsg;

    result = false;
    if((NULL != wdt)&&(type >= EN_WDTCMD_PAUSE)&&(type < EN_WDTCMD_INVALID))
    {
        wdtmsg.pwdt = wdt;
        wdtmsg.opcode = type;
        wdtmsg.para = para;
        result = MsgQ_Send(ptWdtMsgBox,(u8 *)&wdtmsg,sizeof(tagWdtMsg),\
                           CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }
    return result;
}

// =============================================================================
// 功能：WdtHal_RegisterWdtChip
//          注册硬件看门狗芯片
// 参数：chipname,芯片名字
//       yipcycle,WDT芯片狗叫周期，填芯片手册中的最小值即可，函数内部留了20%裕量。
//       wdtchip_feed,硬件看门狗的喂狗方法
// 返回值 ：true成功 false失败
// 说明   ：失败一定是参数不正确，name存储的地方一定是const型，千万别是局部变量
// =============================================================================
bool_t WdtHal_RegisterWdtChip(char *chipname, u32 yipcycle,\
                              bool_t (*wdtchip_feed)(void))
{
    fnWdtHardFeed = wdtchip_feed;
    //喂狗周期调整为溢出周期的80%
    ptWdtHard = Wdt_Create(chipname,yipcycle*4/5,__Wdt_HardWdtYipHook,
                            EN_BLACKBOX_DEAL_IGNORE, 0,0);
    return true;
}

#if CFG_MODULE_ENABLE_WATCH_DOG==true
ADD_TO_ROUTINE_SHELL(wdtshow,wdtshow,"usage:wdtshow");
#endif

