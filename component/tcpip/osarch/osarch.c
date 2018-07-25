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
    u32 lock;   //how many mutex  has been consumed
    u32 semp;    //how many semp  has been consumed
    u32 stack;   //how much task stack has been consumed
}tagOsCB;
static tagOsCB gOsCB;

//use this function for the statistics
void *net_malloc(int size)
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
mutex_t mutex_init(const char *name)
{
    gOsCB.lock++;
    return Lock_MutexCreate(name);
}
bool_t mutex_lock(mutex_t mutex)
{
    return Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
}
bool_t mutex_locktimeout(mutex_t mutex,u32 timeout)
{
    return Lock_MutexPend(mutex,CN_TIMEOUT_FOREVER);
}
bool_t mutex_unlock(mutex_t mutex)
{
    Lock_MutexPost(mutex);
    return true;
}

void mutex_del(mutex_t mutex)
{
    Lock_MutexDelete(mutex);
    gOsCB.lock--;
    return;
}
//-----------------------------------------------------------------------------
//功能:use this function to create a semaphore
//参数:
//返回:the semaphore if success and NULL when failed
//备注:
//-----------------------------------------------------------------------------
semp_t semp_init(u32 limit,u32 value,const char *name)
{
    gOsCB.semp++;
    return Lock_SempCreate(0x1000,0,CN_BLOCK_FIFO,NULL);
}
//-----------------------------------------------------------------------------
//功能:use this function to pend a semaphore
//参数:semp, the semaphore we will pend later
//返回:true when got the semaphore else false
//备注:never return if not got the semaphore and the condition permit waiting
//-----------------------------------------------------------------------------
bool_t semp_pend(semp_t semp)
{
    return Lock_SempPend(semp,CN_TIMEOUT_FOREVER);
}

bool_t semp_pendtimeout(semp_t semp, unsigned int timeout)
{
    return Lock_SempPend(semp,timeout);
}

bool_t semp_post(semp_t semp)
{
    Lock_SempPost(semp);
    return true;
}

void semp_del(semp_t semp)
{
    gOsCB.semp--;
    Lock_SempDelete(semp);
    return;
}

void semp_reset(semp_t semp)
{
    semp->lamp_counter = 0;
    return ;
}

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
    evttID= Djy_EvttRegist(EN_INDEPENDENCE, prior, 0, 1,fnTask,NULL, stacksize,(char *)name);
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    eventID = Djy_EventPop(evttID, NULL, 0,(ptu32_t)para, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        goto EXIT_EVENTFAILED;
    }
    gOsCB.stack += stacksize;
    return true;

EXIT_EVENTFAILED:
    Djy_EvttUnregist(evttID);
EXIT_EVTTFAILED:
    return false;
}
//-----------------------------------------------------------------------------
//功能:use this function to get a random data, which got from the system time
//参数:NULL
//返回:the random data
//备注:
//-----------------------------------------------------------------------------
u32 get_random(void)
{
    return (u32)DjyGetSysTime();
}
//this function is used to format the char string to the argc mode
//this function will changed the original string, used it carefully
int string2arg(int *argc, const char *argv[],char *string)
{
    int argvlen = 0;
    int paramnum = 0;
    char *tmp = NULL;
    char bak;
    int len;

    argvlen = *argc;
    *argc = paramnum;
    if(NULL == string)
    {
        return 1;
    }

    //use the '\0' to replace the ' '
    len = strlen(string);
    tmp = string;
    while(tmp < (string + len))
    {
        if(*tmp == ' ')
        {
            *tmp = '\0';
        }
        tmp++;
    }
    bak = '\0';
    tmp = string;
    while(tmp < (string + len))
    {
        if((*tmp != '\0')&&(bak =='\0'))
        {
            if(paramnum < argvlen)
            {
                argv[paramnum] = tmp;
                paramnum++;
            }
        }
        bak = *tmp;
        tmp++;
    }
    *argc = paramnum;

    return 0;
}

//usage:use this function to change the string to the args
int getargs(int argc, char *argv[],char *string)
{
    int argvlen = 0;
    int paramnum = 0;
    char *tmp = NULL;
    char bak;
    int len;

    argvlen = argc;
    if(NULL == string)
    {
        return paramnum;
    }

    //use the '\0' to replace the ' '
    len = strlen(string);
    tmp = string;
    while(tmp < (string + len))
    {
        if(*tmp == ' ')
        {
            *tmp = '\0';
        }
        tmp++;
    }
    bak = '\0';
    tmp = string;
    while(tmp < (string + len))
    {
        if((*tmp != '\0')&&(bak =='\0'))
        {
            if(paramnum < argvlen)
            {
                argv[paramnum] = tmp;
                paramnum++;
            }
        }
        bak = *tmp;
        tmp++;
    }
    return paramnum;
}



//this is an function tools, used to make the string to mac address
//string must be xx-xx-xx-xx-xx-xx style
bool_t string2mac(char *str,u8 *mac)
{
    bool_t result = false;
    int num = 0;
    int i =0;

    unsigned int data[6];

    num = sscanf(str,"%x-%x-%x-%x-%x-%x",&data[0],&data[1],&data[2],&data[3],&data[4],&data[5]);
    if(num == 6)
    {
        result = true;
        //cpy the mac
        for(i=0;i<6;i++)
        {
            mac[i] = (unsigned char)data[i];
        }
    }

    return result;
}
char* mac2string(u8 *mac)
{
    int i =0;
    static char  str[20];

    unsigned int data[6];

    for(i = 0;i <6;i++)
    {
        data[i] = (unsigned int)mac[i];
    }
    memset(str,0,30);
    sprintf(str,"%02x-%02x-%02x-%02x-%02x-%02x",data[0],data[1],data[2],data[3],data[4],data[5]);

    return str;
}

#define CN_NETTICK_CYCLE  (10)

typedef struct
{
    void               *nxt;
    int                 life;
    int                 cycle;
    u32                 runtimes;
    fnNetTickIsr        isr;
    const char         *name;
}tagTickerItem;

typedef struct
{
    tagTickerItem *lst;
    int            num;
    mutex_t        lock;
}tagTikerCB;
static tagTikerCB gTickerCB;
//use this function to install a ticker isr
//unit:ms
void*  NetTickerIsrInstall(const char *name,fnNetTickIsr isr,int cycle)
{
    tagTickerItem *item;
    item = malloc(sizeof(tagTickerItem));
    if(NULL != item)
    {
        memset(item,0,sizeof(tagTickerItem));
        if(mutex_lock(gTickerCB.lock))
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
            mutex_unlock(gTickerCB.lock);
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
        if(mutex_lock(gTickerCB.lock))
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
            mutex_unlock(gTickerCB.lock);
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
        if(mutex_lock(gTickerCB.lock))
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
            mutex_unlock(gTickerCB.lock);
        }
        Djy_EventDelay(CN_NETTICK_CYCLE*mS);
    }
    return 0;
}

//this is the nettick shell function:use it for the debug
static bool_t  __NetTickShell(char *param)
{
    bool_t ret = true;
    tagTickerItem *item;
    int  no = 0;

    if(mutex_lock(gTickerCB.lock))
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
        mutex_unlock(gTickerCB.lock);
    }
    Djy_EventDelay(CN_NETTICK_CYCLE*mS);
    return ret;
}

//show the tcpip memory consumed
static bool_t __tcpipmem(char *para)
{
    debug_printf("osarch","MEMNEED  :%-8d Bytes\n\r",gOsCB.mem);
    debug_printf("osarch","TASKSTACK:%-8d Bytes\n\r",gOsCB.stack);
    debug_printf("osarch","LOCK     :%-8d \n\r",gOsCB.lock);
    debug_printf("osarch","SEMP     :%-8d \n\r",gOsCB.semp);
    return true;
}


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
    gTickerCB.lock = mutex_init(NULL);
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
    mutex_del(gTickerCB.lock);
    gTickerCB.lock = NULL;
EXIT_LOCK:
    return ret;
}

void OsPrintSplit(char c,int num)
{
    int i = 0;
    for(i = 0;i<num;i++)
    {
        debug_printf("osarch","%c",c);
    }
    printf("\n\r");
    return;
}


//////////////////////////////OSARCH SERVICE///////////////////////////////////
//shell tab here
static const struct ShellCmdTab  gOSARCHDebug[] =
{
    {
        "netticker",
        __NetTickShell,
        "usage:netticker",
        "usage:netticker",
    },
    {
        "tcpipmem",
        __tcpipmem,
        "usage:tcpipmem",
        "usage:tcpipmem",
    },
};
#define CN_OSARCHDebug_NUM  ((sizeof(gOSARCHDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gOSARCHDebugCmdRsc[CN_OSARCHDebug_NUM];
bool_t OsArchInit()
{
    bool_t ret = false;
    memset(&gOsCB,0,sizeof(gOsCB));
    ret = NetTickerInit();
    if(ret == false)
    {
        error_printf("osarch","NETTICKER INIT ERR\n\r");
    }
    ret = Sh_InstallCmd(gOSARCHDebug,gOSARCHDebugCmdRsc,CN_OSARCHDebug_NUM);
    if(ret == false)
    {
        error_printf("osarch","CMD INSTALL ERR\n\r");
    }
    return ret;
}


