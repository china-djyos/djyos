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
//date  :下午5:02:15/2016年12月28日
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
#include "dbug.h"
#include <shell.h>
//add your own specified header here
#include <sys/socket.h>
#include <netbsp.h>
#include <netdb.h>
#include "../common/link.h"

#include "../component_config_tcpip.h"

#include "../common/router.h"
#include "../common/netdev.h"
#include "../common/netpkg.h"

/////////////////////////////LINK MONITOR///////////////////////////////////////
//usage:this is the link monitor item data structure
typedef struct __rcvhookitem
{
    struct __rcvhookitem *nxt;  //used to do the queue
    const char *      name;         //may be you want to specify an name here
    u16               proto;        //the listen  protocol you have set
    struct NetDev     *iface;    //the listen device you have set
    u32               framenum;     //how many frames has listen
    fnLinkProtoDealer hook;         //the listen hook you have set
}tagRcvHookItem;
typedef struct
{
    u32 itemtotal;
    u32 framenum;
    u32 frameunknown;
    tagRcvHookItem *list;
    struct MutexLCB *lock;
}tagRcvHookCB;
static tagRcvHookCB  gRcvHookCB;
//if you specified the special device, only the same device and the same proto frame
//will passed into the hook;if the devhandler is NULL,then any proto with the same proto
//will passed into the hook.
//you could use this function to register some link protocol to do the functions
bool_t Link_RegisterRcvHook(fnLinkProtoDealer hook,const char *ifname,u16 proto,
                            const char *hookname)
{
    bool_t result = false;
    tagRcvHookItem *item;

    if(NULL == hook)
    {
        return result;
    }
    item = net_malloc(sizeof(tagRcvHookItem));
    if(NULL == item)
    {
        return result;
    }
    memset((void *)item,0,sizeof(tagRcvHookItem));
    item->iface = NetDev_GetHandle(ifname);
    item->hook = hook;
    item->proto = proto;
    item->name = hookname;

    if(Lock_MutexPend(gRcvHookCB.lock,CN_TIMEOUT_FOREVER))
    {
        item->nxt = gRcvHookCB.list;
        gRcvHookCB.list = item;
        TCPIP_DEBUG_INC(gRcvHookCB.itemtotal);
        result = true;
        Lock_MutexPost(gRcvHookCB.lock);
    }
    else
    {
        net_free(item);
    }
    return result;
}

//-----------------------------------------------------------------------------
//功能:you could use this function to unregister the hook you got last time
//参数:hook,the hook you registered;
//返回:
//备注:
//作者:zhangqf@下午7:45:38/2016年12月28日
//-----------------------------------------------------------------------------
bool_t Link_UnRegisterRcvHook(const char *hookname)
{
    bool_t result = false;
    tagRcvHookItem *item;
    tagRcvHookItem *nxt;

    if(Lock_MutexPend(gRcvHookCB.lock,CN_TIMEOUT_FOREVER))
    {
        item = gRcvHookCB.list;
        if(0 == strcmp(item->name,hookname))
        {
            //the first match
            gRcvHookCB.list = item->nxt;
            net_free(item);
            TCPIP_DEBUG_DEC(gRcvHookCB.itemtotal);
            result= true;
        }
        else
        {
            while(NULL != item->nxt)
            {
                nxt = item->nxt;
                if(0 == strcmp(item->name,hookname))
                {

                    item->nxt = nxt->nxt;
                    net_free(nxt);
                    TCPIP_DEBUG_DEC(gRcvHookCB.itemtotal);
                    result= true;
                    break;
                }
                else
                {
                    item = nxt;
                }
            }
        }
        Lock_MutexPost(gRcvHookCB.lock);
    }
    return result;
}
//use this function to do hook protocol deal
static bool_t __Link_RcvMonitor(struct NetDev *iface,u16 proto,struct NetPkg *pkg)
{
    bool_t  result = false;
    fnLinkProtoDealer hook = NULL;
    tagRcvHookItem *item;
    //find the hook first
    if(Lock_MutexPend(gRcvHookCB.lock,CN_TIMEOUT_FOREVER))
    {
        item = gRcvHookCB.list;
        if(NetDev_GetType(iface) == EN_LINK_ETHERNET)
        {
            //here we recover the link head for the ethernet(14 bytes here 6 macdst 6 macsrc 2 type)
            PkgMoveOffsetUp(pkg,14);
//          pkg->datalen += 14;
//          pkg->offset -= 14;
        }
        while(NULL != item)
        {
            if((proto == item->proto)&&\
              ((iface == item->iface)||\
               (NULL == item->iface)))
            {
                hook = item->hook;
                TCPIP_DEBUG_INC(item->framenum);
                break;
            }
            item = item->nxt;
        }
        Lock_MutexPost(gRcvHookCB.lock);
    }

    if(NULL != hook)
    {
        result = hook(iface,pkg);
        TCPIP_DEBUG_INC(gRcvHookCB.framenum);
    }
    else
    {
        TCPIP_DEBUG_INC(gRcvHookCB.frameunknown);
    }
    return result;
}

//链路层接口描述
struct LinkInterface
{
    enum enLinkType  iftype;
    const char *name;
    struct LinkOps  ops;
};
static struct LinkInterface  *pLinkHal[EN_LINK_LAST];
//-----------------------------------------------------------------------------
//功能:use this function to find a link operation
//参数:
//返回:
//备注:
//作者:zhangqf@下午8:22:20/2016年12月28日
//-----------------------------------------------------------------------------
static struct LinkOps *__Link_FindOps(enum enLinkType type)
{
    struct LinkOps *ret = NULL;
    if((type < EN_LINK_LAST)&&(NULL != pLinkHal[type]))
    {
        ret = &pLinkHal[type]->ops;
    }
    return ret;
}
////////////////////////LINK LAYER///////////////////////////////////
//-----------------------------------------------------------------------------
//功能:the ip layer use this function to send the ip frame to the link
//参数:
//返回:
//备注:
//作者:zhangqf@下午7:48:16/2016年12月28日
//-----------------------------------------------------------------------------
bool_t Link_Send(struct NetDev *DevFace,struct NetPkg *pkg,u32 devtask,u16 proto,\
                enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc)
{
    bool_t ret = false;
    struct LinkOps *ops;
    ops = NetDev_GetLinkOps(DevFace);
    if(NULL != ops)
    {
        ret = ops->linkout(DevFace,pkg,devtask,proto,ver,ipdst,ipsrc);
    }
    return ret;
}
//-----------------------------------------------------------------------------
//功能:the device thread or irq handler use this function to pass the package to the stack
//参数:
//返回:
//备注:
//作者:zhangqf@下午7:59:30/2016年12月28日
//-----------------------------------------------------------------------------
//bool_t LinkPost(void *iface,struct NetPkg *pkg)
//{
//    bool_t ret = false;
//    enLinkType linktype;
//    struct LinkOps *ops;
//
//    linktype = NetDev_GetType(iface);
//    ops = __Link_FindOps(linktype);
//    if(NULL != ops)
//    {
//      ret = ops->linkin(iface,pkg);
//    }
//    return ret;
//}


//-----------------------------------------------------------------------------
//功能:the link type like the ethernet 80211 link dealer call this func to pass the
//    the package to the tcpip stack
//参数:
//返回:
//备注:
//作者:zhangqf@下午3:55:30/2016年12月29日
//-----------------------------------------------------------------------------
fnLinkProtoDealer  fnIPv6Deal;
fnLinkProtoDealer  fnIPv4Deal;
fnLinkProtoDealer  fnArpDeal;
bool_t Link_PushRegister(u16 protocol,fnLinkProtoDealer dealer)
{
    bool_t ret = true;
    if(protocol == EN_LINKPROTO_IPV4)
    {
        fnIPv4Deal =dealer;
    }
    else if(protocol == EN_LINKPROTO_IPV6)
    {

        fnIPv6Deal = dealer;
    }
    else if (protocol== EN_LINKPROTO_ARP)
    {
        fnArpDeal = dealer;
    }
    else
    {
        ret = false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能：链路层往IP层等生成协议push数据，由802.3、raw等链路层设备调用。
//参数：iface，网卡设备指针
//      pkg，待上送的包
//      protocol，协议代号，如 IPV4、IPV6、ARP、RARP、自定义等
//返回：true = 上层协议成功调用，false = 上层协议调用失败
//-----------------------------------------------------------------------------
bool_t Link_Push(struct NetDev  *iface,struct NetPkg *pkg,enum enLinkProto protocol)
{
    bool_t          ret=true;
    //we analyze the ethernet header first, which type it has
    switch(protocol)
    {
     //uptil now, we support the ipv4
        case EN_LINKPROTO_IPV4:
            if(NULL != fnIPv4Deal)
            {
                ret = fnIPv4Deal(iface,pkg);
            }
            break;
        case EN_LINKPROTO_IPV6: //在fnIPv4Deal(__IpPushNew)中，也有判断IP版本并分支处理
                                //的代码，最终如何处理IPV6，待定
            if(NULL != fnIPv6Deal)
            {
                ret = fnIPv6Deal(iface,pkg);
            }
            break;
        case EN_LINKPROTO_ARP:
            if(NULL != fnArpDeal)
            {
                ret = fnArpDeal(iface,pkg);
            }
            break;

        default:    //TODO：此处要改，结合 pDrive-fnrcvhook 来改
            __Link_RcvMonitor(iface,protocol,pkg);
            break;
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：网卡设备推送数据到链路层，由网卡驱动调用，本函数只是个转接，将转向该网卡对应的链路
//      层（由 enum enLinkType 定义，例如 以太网、裸数据接口等）
bool_t Link_NetDevPush(struct NetDev *iface,struct NetPkg *pkg)
{
    bool_t ret = false;
    struct LinkOps *ops;
    if(NULL != iface)
    {
        NetDev_PkgrcvInc(iface);
//      ret = LinkDeal(iface,pkg);
        ops = NetDev_GetLinkOps(iface);
        if(NULL != ops)
        {
            ret = ops->linkin(iface,pkg);
        }
        if(ret == false)
        {
            NetDev_PkgrcvErrInc(iface);
        }
    }
    return ret;
}

//the net device layer will call this function to do more deal
//bool_t LinkDeal(struct NetDev *iface,struct NetPkg *pkg)
//{
//    bool_t ret = false;
//    struct LinkOps *ops;
//    ops = NetDev_GetLinkOps(iface);
//    if(NULL != ops)
//    {
//        ret = ops->linkin(iface,pkg);
//    }
//    return ret;
//}
//find a link ops here
struct LinkOps  *Link_FindOps(enum enLinkType type)
{
    return __Link_FindOps(type);
}
//-----------------------------------------------------------------------------
//功能:you could use this function to find a registered link type name
//参数:type,which defines by enLinkType in socket.h
//返回:
//备注:
//作者:zhangqf@下午6:44:54/2017年1月3日
//-----------------------------------------------------------------------------
const char *Link_TypeName(enum enLinkType type)
{
    const char *name = "Unknown";
    if((type < EN_LINK_LAST)&&(NULL != pLinkHal[type]))
    {
        name = pLinkHal[type]->name;
    }
    return name;
}
//-----------------------------------------------------------------------------
//功能:use this function to register a link here
//参数:
//返回:
//备注:
//作者:zhangqf@下午8:25:38/2016年12月28日
//-----------------------------------------------------------------------------
bool_t Link_Register(enum enLinkType type,const char *name,struct LinkOps *ops)
{
    bool_t result = false;
    struct LinkInterface *link = NULL;

    link = net_malloc(sizeof(struct LinkInterface));
    if(NULL != link)
    {
        link->iftype = type;
        link->name = name;
        link->ops = *ops;
        pLinkHal[type] = link;
        result = true;
    }
    return result;
}

//-----------------------------------------------------------------------------
//功能:use this function to unregister ther link we registered
//参数:
//返回:
//备注:
//作者:zhangqf@下午1:55:39/2016年12月29日
//-----------------------------------------------------------------------------
bool_t Link_UnRegister(enum enLinkType type,const char *name)
{
    bool_t result = false;
    struct LinkInterface *link = NULL;

    link = pLinkHal[type];
    if((NULL != link)&&(0 == strcmp(link->name,name)))
    {
        net_free(link);
        pLinkHal[type] = NULL;
        result = true;
    }
    return result;
}



//do the hook debug module
static bool_t __Link_HookShell(void)
{
    int i = 0;
    tagRcvHookItem *item;

    debug_printf("link","LinkHookModule\n\r");
    if(Lock_MutexPend(gRcvHookCB.lock,CN_TIMEOUT_FOREVER))
    {
        debug_printf("link","Item:Total:%d  FrameRcvTotal:%d FrameRcvUnknown:%d\n\r",\
                gRcvHookCB.itemtotal,gRcvHookCB.framenum,gRcvHookCB.frameunknown);

        debug_printf("link","%-10s%-10s%-10s%-10s%-10s%-10s\n\r",\
                "Number","HookName","Protocol","Function","FrameNum","Interface");

        item = gRcvHookCB.list;
        while(NULL != item)
        {
            debug_printf("link","No.%-4d   %-8s  %-8x  %-8x  %-8x  %s\n\r",\
                    i++,item->name?item->name:"NULL",\
                    item->proto,(u32)item->hook,\
                    item->framenum,NetDev_GetName(item->iface));
            item = item->nxt;
        }
        Lock_MutexPost(gRcvHookCB.lock);
    }
    return true;
}


//usage:use this shell to show all the link type supported
static bool_t __Link_TypeShell(void)
{
    u8 i;
    struct LinkInterface *link;
    debug_printf("link","%-4s%-10s\n\r","Type","Name");
    for(i = 0;i < EN_LINK_LAST;i++)
    {
        link = pLinkHal[i];
        if(NULL != link)
        {
            debug_printf("link","%-4d%-10s\n\r",i,link->name);
        }
    }
    return true;
}

//static bool_t __LinkShell(char *param)

bool_t Link_net(char *param)
{
    bool_t ret = true;
    if((NULL == param)||(0 == strcmp(param,"-t")))
    {
        ret = __Link_TypeShell();
    }
    else if(0 == strcmp(param,"-h"))
    {
        ret = __Link_HookShell();
    }
    else
    {
        ret = false;
    }
    return ret;
}


// =============================================================================
// FUNCTION   :this is the link module initialize function
// PARAMS IN  :
// PARAMS OUT :
// RETURN     :
// DESCRIPTION:uptils now, we do nothing here
// =============================================================================
bool_t Link_Init(void)
{
    //the gLinkHook module must be initialized
    memset((void *)&gRcvHookCB,0,sizeof(gRcvHookCB));
    gRcvHookCB.lock = Lock_MutexCreate(NULL);
    return true;
}
ADD_TO_ROUTINE_SHELL(net_link,Link_net,"usage:net_link [-t (show types)]/[-h (show hooks)]");

