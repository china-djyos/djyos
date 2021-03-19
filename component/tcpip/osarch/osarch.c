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

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <os.h>
#include <shell.h>
#include <osarch.h>
#include "dbug.h"
#include "../component_config_tcpip.h"


typedef struct
{
    u32 mem;     //how many heap memory has been consumed
    u32 stack;   //how much task stack has been consumed
}tagOsCB;
static tagOsCB gOsCB;

//use this function for the statistics
void *net_malloc(s32 size)
{
    void *ret;
    ret = malloc(size);
    if(NULL != ret)
    {
        gOsCB.mem += M_CheckSize(ret);
    }
    return ret;
}
void net_free(void *mem)
{
    if(NULL != mem)
    {
        free(mem);
        gOsCB.mem -=M_CheckSize(mem);
    }
    return;
}

//mutex
//改为： Lock_MutexCreate(name);
//mutex_t mutex_init(const char *name)
//{
//    gOsCB.lock++;
//    return Lock_MutexCreate(name);
//}
//改为： Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
//bool_t mutex_lock(mutex_t mutex)
//{
//    return Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
//}
//改为： Lock_MutexPend(mutex,timeout);
//bool_t mutex_locktimeout(mutex_t mutex,u32 timeout)
//{
//    return Lock_MutexPend(mutex,timeout);
//}
//改为： Lock_MutexPost(mutex);
//bool_t mutex_unlock(mutex_t mutex)
//{
//    Lock_MutexPost(mutex);
//    return true;
//}
//改为： Lock_MutexDelete(mutex);
//void mutex_del(mutex_t mutex)
//{
//    Lock_MutexDelete(mutex);
//    gOsCB.lock--;
//    return;
//}
////-----------------------------------------------------------------------------
////功能:use this function to create a semaphore
////参数:
////返回:the semaphore if success and NULL when failed
////备注:
////-----------------------------------------------------------------------------
//改为： Lock_SempCreate(limit,value,CN_BLOCK_FIFO,name);
//semp_t semp_init(u32 limit,u32 value,const char *name)
//{
//    gOsCB.semp++;
//    return Lock_SempCreate(limit,value,CN_BLOCK_FIFO,name);
//}
////-----------------------------------------------------------------------------
////功能:use this function to pend a semaphore
////参数:semp, the semaphore we will pend later
////返回:true when got the semaphore else false
////备注:never return if not got the semaphore and the condition permit waiting
////-----------------------------------------------------------------------------
//改为：  Lock_SempPend(semp,CN_TIMEOUT_FOREVER);
//bool_t semp_pend(semp_t semp)
//{
//    return Lock_SempPend(semp,CN_TIMEOUT_FOREVER);
//}
//改为： Lock_SempPend(semp,timeout);
//bool_t semp_pendtimeout(semp_t semp, u32 timeout)
//{
//    return Lock_SempPend(semp,timeout);
//}
//改为： Lock_SempPost(semp);
//bool_t semp_post(semp_t semp)
//{
//    Lock_SempPost(semp);
//    return true;
//}
//改为： Lock_SempDelete(semp)
//void semp_del(semp_t semp)
//{
//    gOsCB.semp--;
//    Lock_SempDelete(semp);
//    return;
//}

//-----------------------------------------------------------------------------
//功能:use this function to create a task
//参数:
//返回:
//备注:
//作者:zhangqf@下午4:38:04/2017年1月5日
//-----------------------------------------------------------------------------
bool_t taskcreate(const char *name,u16 stacksize,u8 prior,ptu32_t (*fnTask)(void),void *para)
{
    u16 evttID,eventID;
    evttID= DJY_EvttRegist(EN_INDEPENDENCE, prior, 0, 1,fnTask,NULL, stacksize,(char *)name);
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    eventID = DJY_EventPop(evttID, NULL, 0,(ptu32_t)para, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        goto EXIT_EVENTFAILED;
    }
    gOsCB.stack += stacksize;
    return true;

EXIT_EVENTFAILED:
    DJY_EvttUnregist(evttID);
EXIT_EVTTFAILED:
    return false;
}


//this is an function tools, used to make the string to mac address
//string must be xx-xx-xx-xx-xx-xx style
bool_t string2mac(char *str,u8 *mac)
{
    bool_t result = false;
    s32 num = 0;
    s32 i =0;

    u32 data[6];

    num = sscanf(str,"%x-%x-%x-%x-%x-%x",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5]);
    if(num == 6)
    {
        result = true;
        //cpy the mac
        for(i=0;i<6;i++)
        {
            mac[i] = (u8)data[i];
        }
    }

    return result;
}
char* mac2string(u8 *mac)
{
    s32 i =0;
    static char  str[30];

    u32 data[6];

    for(i = 0;i <6;i++)
    {
        data[i] = (u32)mac[i];
    }
    memset(str,0,30);
    sprintf(str,"%02x-%02x-%02x-%02x-%02x-%02x",data[0],data[1],data[2],data[3],data[4],data[5]);

    return str;
}

#define CN_NETTICK_CYCLE  (10)      //网络tick运行周期，mS数

typedef struct
{
    void               *nxt;
    s32                 life;       //倒计时计数器
    s32                 cycle;      //定时周期，mS数
    u32                 runtimes;   //定时次数
    fnNetTickIsr        isr;
    const char         *name;
}tagTickerItem;

typedef struct
{
    tagTickerItem *lst;
    s32            num;
    struct MutexLCB *lock;
}tagTikerCB;
static tagTikerCB gTickerCB;
//use this function to install a ticker isr
//unit:ms
void*  NetTickerIsrInstall(const char *name,fnNetTickIsr isr,s32 cycle)
{
    tagTickerItem *item;
    item = malloc(sizeof(tagTickerItem));
    if(NULL != item)
    {
        memset(item,0,sizeof(tagTickerItem));
        if(Lock_MutexPend(gTickerCB.lock,CN_TIMEOUT_FOREVER))
        {
            //do the initialize
            item->life = cycle;
            item->isr = isr;
            item->cycle = cycle;
            item->name = name;
            //add it to the queu
            item->nxt = gTickerCB.lst;
            gTickerCB.lst = item;
            gTickerCB.num++;
            Lock_MutexPost(gTickerCB.lock);
        }
    }
    return item;
}
//remove a ticker
bool_t  NetTickerIsrRemove(void *ticker)
{
    bool_t ret = true;
    tagTickerItem *item;

    if(NULL != ticker)
    {
        if(Lock_MutexPend(gTickerCB.lock,CN_TIMEOUT_FOREVER))
        {
            item = gTickerCB.lst;
            if((ticker == item)&&(NULL != item))
            {
                gTickerCB.lst = item->nxt;
            }
            else //check the list
            {
                while(NULL != item)
                {
                    if(ticker == item->nxt)
                    {
                        item->nxt = ((tagTickerItem *)ticker)->nxt;
                        free(ticker);
                        break;
                    }
                    item = item->nxt;
                }
            }
            Lock_MutexPost(gTickerCB.lock);
        }
    }
    return ret;
}


//return how many ticks has been gone
u32  NetTickerTicks(void *ticker)
{
    u32 ret = 0;
    if(NULL != ticker)
    {
        ret = ((tagTickerItem *)ticker)->runtimes;
    }
    return ret;
}
//THIS IS THE OSARCH MAIN FUNCTION:CHECK ALL THE ITEM AND DO THE HOOK
static ptu32_t __NetTickerTask(void)
{
    tagTickerItem *item;
    while(1)
    {
        if(Lock_MutexPend(gTickerCB.lock,CN_TIMEOUT_FOREVER))
        {
            item = gTickerCB.lst;
            while(NULL != item)
            {
                if(item->life > 0)
                {
                    item->life -= CN_NETTICK_CYCLE;
                }
                else
                {
                    item->isr();
                    item->runtimes++;
                    item->life = item->cycle;
                }
                item = item->nxt;
            }
            Lock_MutexPost(gTickerCB.lock);
        }
        DJY_EventDelay(CN_NETTICK_CYCLE*mS);
    }
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//this is the nettick shell function:use it for the debug
//static bool_t  __NetTickShell(char *param)

 bool_t  netticker(char *param)
{
    bool_t ret = true;
    tagTickerItem *item;
    s32  no = 0;

    if(Lock_MutexPend(gTickerCB.lock,CN_TIMEOUT_FOREVER))
    {
        debug_printf("osarch","NETTICK:NUM:%d PRECISION:%d MS\n\r",gTickerCB.num,CN_NETTICK_CYCLE);
        if(gTickerCB.num > 0)
        {
            debug_printf("osarch","%-2s %-8s %-8s %-8s %-10s %s\n\r",\
                    "NO","LIFE","CYCLE-MS","ISR","NAME","RUNTIMES");
        }
        item = gTickerCB.lst;
        while(NULL != item)
        {
            debug_printf("osarch","%-2d %-8d %-8d %-8x %-10s %d\n\r",\
                    no,item->life,item->cycle,(u32)item->isr,item->name,item->runtimes);
            no++;
            item = item->nxt;
        }
        Lock_MutexPost(gTickerCB.lock);
    }
    DJY_EventDelay(CN_NETTICK_CYCLE*mS);
    return ret;
}

//show the tcpip memory consumed
//static bool_t __tcpipmem(char *para)
bool_t tcpipmem(char *para)
{
    debug_printf("osarch","MEMNEED  :%-8d Bytes\n\r",gOsCB.mem);
    debug_printf("osarch","TASKSTACK:%-8d Bytes\n\r",gOsCB.stack);
    return true;
}

#pragma GCC diagnostic pop

__attribute__((weak))  const u8 CN_NETTICKER_PRIOR = CN_PRIO_RRS;
__attribute__((weak))  const u32 CN_NETTICKER_STACKSIZE = 0x800;
// =============================================================================
// FUNCTION:
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:use this function to initialize the net ticker module, then you could
//          add timer task here,this module is independent,and must be initialize
//          in the initialize stage
// =============================================================================
static bool_t NetTickerInit(void)
{
    bool_t ret = false;
    memset(&gTickerCB,0,sizeof(gTickerCB));
    gTickerCB.lst = NULL;
    gTickerCB.lock = Lock_MutexCreate(NULL);
    if(NULL == gTickerCB.lock)
    {
        error_printf("osarch","LOCK CREATE ERR\n\r");
        goto EXIT_LOCK;
    }
    ret = taskcreate("NETTIKER",CN_NETTICKER_STACKSIZE,CN_NETTICKER_PRIOR,__NetTickerTask,NULL);
    if(ret == false)
    {
        error_printf("osarch","TASK CREATE ERR\n\r");
        goto EXIT_TASK;
    }
    return ret;

EXIT_TASK:
    Lock_MutexDelete(gTickerCB.lock);
    gTickerCB.lock = NULL;
EXIT_LOCK:
    return ret;
}

void OsPrintSplit(char c,s32 num)
{
    s32 i = 0;
    for(i = 0;i<num;i++)
    {
        printf("%c",c);
    }
    printf("\n\r");
    return;
}


//////////////////////////////OSARCH SERVICE///////////////////////////////////


bool_t OsArchInit()
{
    bool_t ret;
    memset(&gOsCB,0,sizeof(gOsCB));
    ret = NetTickerInit();
    if(ret == FALSE)
    {
        error_printf("osarch","NETTICKER INIT ERR\n\r");
    }



    return ret;
}
ADD_TO_ROUTINE_SHELL(tcpipmem,tcpipmem,"usage:tcpipmem");
ADD_TO_ROUTINE_SHELL(netticker,netticker,"usage:netticker");


