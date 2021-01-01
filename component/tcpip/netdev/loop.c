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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
//author:zhangqf
//date  :下午6:28:01/2017年1月3日
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
    u32    evttID;     //用32位数，以后事件类型ID要改为32位的
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

//----自环网卡的发送函数---------------------------------------------------------
//功能：把上层传下来的pkg链合并为一个大的pkg，然后链接到网卡控制块的pkg成员下，并
//      弹出事件，自环网卡的接收事件将启动。
//参数：iface，网卡指针
//      pkg，待发送的数据包链表
//      netdevtask，须网卡做的辅助任务，对于自环网卡来说，无意义。
//返回：true = 成功发送，false = 内存不足导致失败
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
//                PkgSetNextUnit(sndpkg, NULL);      //PkgMalloc已经设置好
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
//功能:
//参数:
//返回:
//备注:
//作者:zhangqf@上午9:26:04/2016年12月29日
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

