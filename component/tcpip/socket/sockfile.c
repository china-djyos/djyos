//-----------------------------------------------------------------------------
// Copyright (c) 2018, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// Copyright (c) 2018，著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <shell.h>
#include <os.h>
#include <osarch.h>
#include "dbug.h"

//------------------this file system should implement this--------------------//
//1,malloc a sockobject for us, anything sockobject has been done
//2,free a sockobject,anything sockobject contacted should been closed
//3,set a context
//4,map a sockobject to handle
//5,map a handle to sockobject
//6,set sockobject status:like the readable writable
//7,clear sockobject status:like the readable writable
//////////////////////////WE WILL DO A EASY ONE HERE////////////////////////////
typedef struct
{
    u32   flag;
    u32   status;
    void *context;
}tagItem;
typedef struct
{
    tagItem *tab;
    u32      tablen;
    mutex_t  lock;   //used to protect the tab
    u32      used;   //how many item has been used
}tagSockFileCB;
static tagSockFileCB  gSockFileCB;   //all the control member here

#define CN_SOCKFILE_BUSY (1<<0)
#define CN_SOCKFILE_BASEFD    (0X10000)
void *sockobj_malloc(void *cb)
{
    void *ret = NULL;
    tagItem *tmp;
    int i = 0;
    if(mutex_lock(gSockFileCB.lock))
    {
        for(i =0;i <gSockFileCB.tablen;i++)
        {
            tmp = &gSockFileCB.tab[i];
            if(0==(tmp->flag &CN_SOCKFILE_BUSY))
            {
                tmp->flag =CN_SOCKFILE_BUSY;
                ret = tmp;
                gSockFileCB.used++;
                break;
            }
        }
        mutex_unlock(gSockFileCB.lock);
    }
    return ret;
}
void  sockobj_free(void *cb,void *obj)
{
    tagItem *tmp;
    tmp = obj;
    if(mutex_lock(gSockFileCB.lock))
    {
        memset(tmp,0,sizeof(tagItem));
        gSockFileCB.used--;
        mutex_unlock(gSockFileCB.lock);
    }
    return;
}
bool_t sockobj_setcontext(void *obj,void *context)
{
    tagItem *tmp;
    tmp = obj;
    tmp->context = context;
    return true;
}
void *sockobj_getcontext(void *obj)
{
    void *ret = NULL;
    if(NULL != obj)
    {
        ret = ((tagItem *)obj)->context;
    }
    return ret;
}
void * sockobj_obj(int fd)
{
    void *ret = NULL;
    int off;
    off = fd- CN_SOCKFILE_BASEFD;
    if((off >= 0)&&(off < gSockFileCB.tablen))
    {
        ret = &gSockFileCB.tab[off];
    }
    return ret;
}
int sockobj_fd(void *obj)
{
    int ret = -1;
    tagItem *tmp;
    int      off;
    tmp = obj;
    off = tmp-gSockFileCB.tab;
    if((off >=0)&&(off < gSockFileCB.tablen))
    {
        ret = CN_SOCKFILE_BASEFD+off;
    }
    return ret;
}
bool_t sockobj_setstatus(void *obj,u32 status)
{
    bool_t ret = true;

    tagItem *tmp;
    tmp = obj;
    tmp->status |= status;
    return ret;
}
bool_t sockobj_clrstatus(void *obj,u32 status)
{
    bool_t ret = true;

    tagItem *tmp;
    tmp = obj;
    tmp->status &= (~status);
    return ret;
}


//show all the sockfile fd

//static bool_t __sockobj_shell(char *param)
ADD_TO_IN_SHELL_HELP(sockfile,"usage:sockfile [-a]");
ADD_TO_IN_SHELL  bool_t sockfile(char *param)
{
    //first show the statistics
    info_printf("sockfile","TABLEN:%d USED:%d ITEMSIZE:%d TABADDR:0X%08X",\
            gSockFileCB.tablen,gSockFileCB.used,sizeof(tagItem),(u32)gSockFileCB.tab);
    if((NULL != param)&&(0 == strcmp(param,"-a"))) //show all the information
    {
        tagItem *tmp;
        int i = 0;
        if(mutex_lock(gSockFileCB.lock))
        {
            info_printf("sockfile","%-4s %-8s %-8s %-8s %-8s",\
                    "NO","FLAG","STATUS","CONTEXT","FD");
            for(i =0;i <gSockFileCB.tablen;i++)
            {
                tmp = &gSockFileCB.tab[i];
                info_printf("sockfile","%-4d %-8x %-8x %-8x %-8x",\
                        i,tmp->flag,tmp->status,(u32)tmp->context,i+CN_SOCKFILE_BASEFD);
            }
            mutex_unlock(gSockFileCB.lock);
        }
    }
    return true;
}


//do the sockfile initialize here
bool_t sockobj_init(u32 num)
{
    bool_t ret = false;
    memset(&gSockFileCB,0,sizeof(gSockFileCB));
    gSockFileCB.tab = net_malloc(num*sizeof(tagItem));
    if(NULL == gSockFileCB.tab)
    {
        error_printf("sockfile","%s:TABERR",__FUNCTION__);
        goto EXIT_TABMEM;
    }
    memset(gSockFileCB.tab,0,num*sizeof(tagItem));
    gSockFileCB.tablen = num;
    gSockFileCB.lock = mutex_init(NULL);
    if(NULL == gSockFileCB.lock)
    {
        error_printf("sockfile","%s:TABERR",__FUNCTION__);
        goto EXIT_TABLOCK;
    }

    return (TRUE);

EXIT_TABLOCK:
    net_free(gSockFileCB.tab);
    gSockFileCB.tab = NULL;
EXIT_TABMEM:
    return ret;

}



