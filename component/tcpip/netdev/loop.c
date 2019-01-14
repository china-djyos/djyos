//----------------------------------------------------
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
//author:zhangqf
//date  :����6:28:01/2017��1��3��
//usage :
//------------------------------------------------------

//standard includes
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <os.h>
#include <osarch.h>


#include <sys/socket.h>
#include <netdb.h>
#include <netbsp.h>
#include "../component_config_tcpip.h"

#include "../common/router.h"


//THIS IS THE INTERNAL LOOP DEVICE
//each router should add it self to the loop rout,if not,then any
//package to the route will be refused,for it will think the destination
//is not to the host
#define CN_LOOP_DEVICE   "LOOP"
#define CN_LOOP_MTU      4*1024        //4KB
typedef struct
{
    mutex_t lock;
    semp_t  sync;
    void  *iface;
    tagNetPkg *pkg;
}tagLoopCB;
static tagLoopCB gLoopCB;
//for the tempory, we will create a loop task here
static ptu32_t __LoopTask(void)
{
    tagNetPkg *pkg = NULL;
    while(1)
    {
        if(semp_pendtimeout(gLoopCB.sync,10*mS))
        {
            if(mutex_lock(gLoopCB.lock))
            {
                pkg = gLoopCB.pkg;
                gLoopCB.pkg = NULL;
                mutex_unlock(gLoopCB.lock);
            }
            if(NULL!= pkg) //PUSH IT
            {
                NetDevPush(gLoopCB.iface,pkg);
                PkgTryFreePart(pkg);
            }
        }
    }
    return 0;
}

//implement the device io functions
//device receive function
static tagNetPkg * __LoopIn(void *iface)
{
    tagNetPkg *pkg = NULL;
    if(mutex_lock(gLoopCB.lock))
    {
        pkg = gLoopCB.pkg;
        gLoopCB.pkg = NULL;
        mutex_unlock(gLoopCB.lock);
    }
    return pkg;
}
//device send function
static bool_t __LoopOut(void *iface,tagNetPkg *pkg,u32 framlen,u32 netdevtask)
{
    bool_t  ret = false;
    tagNetPkg *tmp;
    tagNetPkg *sndpkg;
    u8 *src;
    u8 *dst;
    sndpkg = PkgMalloc(framlen,0);
    if(NULL != sndpkg)
    {
        tmp = pkg;
        while(NULL != tmp)
        {
            src = (u8 *)(tmp->buf + tmp->offset);
            dst = (u8 *)(sndpkg->buf + sndpkg->offset + sndpkg->datalen);
            memcpy(dst, src, tmp->datalen);
            sndpkg->datalen += tmp->datalen;
            if(tmp->pkgflag & CN_PKLGLST_END)
            {
                tmp = NULL;
            }
            else
            {
                tmp = tmp->partnext;
            }
        }
        //push it to the loop controller
        if(mutex_lock(gLoopCB.lock))
        {
            if(NULL == gLoopCB.pkg)
            {
                gLoopCB.pkg = sndpkg;
                ret = true;
            }
            mutex_unlock(gLoopCB.lock);
            semp_post(gLoopCB.sync);
        }
        if(false == ret)
        {
            PkgTryFreePart(sndpkg);
        }
        ret = true;
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
bool_t LoopInit(void)
{
    tagNetDevPara   devpara;

    //do the loop device initialize
    memset(&gLoopCB,0,sizeof(gLoopCB));
    gLoopCB.lock = mutex_init(NULL);
    gLoopCB.sync = semp_init(1,0,NULL);
    taskcreate("LOOPDEV",0x800,CN_PRIO_RRS,__LoopTask,NULL);
    //then we will register a loop device to the stack
    memset((void *)&devpara,0,sizeof(devpara));
    devpara.ifsend = __LoopOut;
    devpara.ifrecv = __LoopIn;
    devpara.iftype = EN_LINK_RAW;
    devpara.name = CN_LOOP_DEVICE;
    devpara.private = 0;
    devpara.mtu = CN_LOOP_MTU;
    devpara.devfunc = CN_IPDEV_ALL;
    memcpy(devpara.mac,CN_MAC_BROAD,CN_MACADDR_LEN);
    gLoopCB.iface = NetDevInstall(&devpara);
    if(NULL == (void *)gLoopCB.iface)
    {
        goto EXIT_LOOPDEV;
    }
    //then we will create a loop rout to the stack
    RouterSetHost(EN_IPV_4,CN_LOOP_DEVICE);
    return true;

EXIT_LOOPDEV:
    return false;
}
