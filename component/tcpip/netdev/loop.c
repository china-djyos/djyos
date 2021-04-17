//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "../common/netpkg.h"


//THIS IS THE INTERNAL LOOP DEVICE
//each router should add it self to the loop rout,if not,then any
//package to the route will be refused,for it will think the destination
//is not to the host
#define CN_LOOP_DEVICE   "LOOP"
#define CN_LOOP_MTU      4*1024        //4KB
typedef struct
{
    struct MutexLCB *lock;
    u32    evttID;     //��32λ�����Ժ��¼�����IDҪ��Ϊ32λ��
    struct NetDev *iface;
    struct NetPkg *pkg;
}tagLoopCB;
static tagLoopCB gLoopCB;

//for the tempory, we will create a loop task here
static ptu32_t __LoopTask(void)
{
    struct NetPkg *pkg = NULL;
    while(1)
    {
        DJY_WaitEvttPop(DJY_GetMyEvttId(), NULL, 100 * mS);
        while(1)
        {
            if(Lock_MutexPend(gLoopCB.lock,CN_TIMEOUT_FOREVER))
            {
                pkg = gLoopCB.pkg;
                if(pkg != NULL)
                {
                    gLoopCB.pkg = PkgGetNextUnit(pkg);
                    PkgSetNextUnit(pkg, NULL);
                }
                Lock_MutexPost(gLoopCB.lock);
                if(pkg != NULL)
                {
                    Link_NetDevPush(gLoopCB.iface,pkg);
                    PkgTryFreePart(pkg);
                }
                else
                {
                    break;
                }
            }
            else
                break;
        }
    }
    return 0;
}
//
////implement the device io functions
////device receive function
//static struct NetPkg * __LoopIn(struct NetDev *iface)
//{
//    struct NetPkg *pkg = NULL;
//    if(Lock_MutexPend(gLoopCB.lock))
//    {
//        pkg = gLoopCB.pkg;
//        gLoopCB.pkg = NULL;
//        Lock_MutexPost(gLoopCB.lock);
//    }
//    return pkg;
//}
//device send function

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�Ի������ķ��ͺ���---------------------------------------------------------
//���ܣ����ϲ㴫������pkg���ϲ�Ϊһ�����pkg��Ȼ�����ӵ��������ƿ��pkg��Ա�£���
//      �����¼����Ի������Ľ����¼���������
//������iface������ָ��
//      pkg�������͵����ݰ�����
//      netdevtask�����������ĸ������񣬶����Ի�������˵�������塣
//���أ�true = �ɹ����ͣ�false = �ڴ治�㵼��ʧ��
//------------------------------------------------------------------------------
static bool_t __LoopOut(struct NetDev *iface,struct NetPkg *pkg,u32 netdevtask)
{
    bool_t  ret = false;
    struct NetPkg *tmp;
    struct NetPkg *sndpkg;
    u32 framlen;
    framlen = PkgFrameDatastatistics(pkg);
    sndpkg = PkgMalloc(framlen,0);
    if(NULL != sndpkg)
    {
        PkgCopyFrameToPkg(pkg, sndpkg);
        if(Lock_MutexPend(gLoopCB.lock,CN_TIMEOUT_FOREVER))
        {
            tmp = gLoopCB.pkg;
            if(NULL == tmp)
            {
                gLoopCB.pkg = sndpkg;
                ret = true;
            }
            else
            {
                framlen = 0;
                while(PkgGetNextUnit(tmp) != NULL)
                {
                    framlen++;
                    tmp = PkgGetNextUnit(tmp);
                }
                if(framlen >3)
                    framlen = 4;
                PkgSetNextUnit(tmp, sndpkg);
//                PkgSetNextUnit(sndpkg, NULL);      //PkgMalloc�Ѿ����ú�
            }
            Lock_MutexPost(gLoopCB.lock);
            DJY_EventPop(gLoopCB.evttID,NULL,0,0,0,0);
        }
        ret = true;
    }
    return ret;
}

#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//����:zhangqf@����9:26:04/2016��12��29��
//-----------------------------------------------------------------------------
bool_t LoopInit(void)
{
    struct NetDevPara   devpara;
    u16 evttID;
    u32 hop,subnet,ip,submask,dns,dnsbak;
    tagRouterPara para;

    //do the loop device initialize
    memset(&gLoopCB,0,sizeof(gLoopCB));
    gLoopCB.lock = Lock_MutexCreate(NULL);
    evttID = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1, __LoopTask, NULL, 0x2800, "LOOPDEV");
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    gLoopCB.evttID = evttID;
    //then we will register a loop device to the stack
    memset((void *)&devpara,0,sizeof(devpara));
    devpara.ifsend = __LoopOut;
    devpara.iftype = EN_LINK_RAW;
    devpara.name = CN_LOOP_DEVICE;
    devpara.Private = 0;
    devpara.mtu = CN_LOOP_MTU;
    devpara.devfunc = CN_IPDEV_ALL;
    memcpy(devpara.mac,CN_MAC_BROAD,CN_MACADDR_LEN);
    gLoopCB.iface = NetDev_Install(&devpara);
    if(NULL == (void *)gLoopCB.iface)
    {
        goto EXIT_LOOPDEV;
    }

    memset(&para,0,sizeof(para));
    ip      = INADDR_LOOPBACK;
    submask = INADDR_BROAD;
    hop     = INADDR_LOOPBACK;
    dns     = INADDR_LOOPBACK;
    dnsbak  = INADDR_LOOPBACK;
    subnet  = INADDR_LOOPBACK;

    para.ver = EN_IPV_4;
    para.ifname = CN_LOOP_DEVICE;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
//    para.dns = &dns;
//    para.dnsbak = &dnsbak;
    para.prior = CN_ROUT_PRIOR_LOOP;
    para.flags = 0;

    NetDev_SetDns(EN_IPV_4,gLoopCB.iface, &dns, &dnsbak);
    NetDev_SetGateway(EN_IPV_4,gLoopCB.iface, &hop);
    if(RouterCreate(&para))
    {
        printf("Create loop router success\r\n");
    }
    else
    {
        printf("Create loop router fail\r\n");
    }

    //then we will create a loop rout to the stack
    RouterSetHost(EN_IPV_4,CN_LOOP_DEVICE);
    return true;

EXIT_LOOPDEV:
    DJY_EvttUnregist(evttID);
EXIT_EVTTFAILED:
    return false;
}

