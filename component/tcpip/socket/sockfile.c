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
// Copyright (c) 2018������Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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



