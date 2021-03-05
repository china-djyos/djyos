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
//date  :����5:02:15/2016��12��28��
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
//����:you could use this function to unregister the hook you got last time
//����:hook,the hook you registered;
//����:
//��ע:
//����:zhangqf@����7:45:38/2016��12��28��
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

//��·��ӿ�����
struct LinkInterface
{
    enum enLinkType  iftype;
    const char *name;
    struct LinkOps  ops;
};
static struct LinkInterface  *pLinkHal[EN_LINK_LAST];
//-----------------------------------------------------------------------------
//����:use this function to find a link operation
//����:
//����:
//��ע:
//����:zhangqf@����8:22:20/2016��12��28��
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
//����:the ip layer use this function to send the ip frame to the link
//����:
//����:
//��ע:
//����:zhangqf@����7:48:16/2016��12��28��
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
//����:the device thread or irq handler use this function to pass the package to the stack
//����:
//����:
//��ע:
//����:zhangqf@����7:59:30/2016��12��28��
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
//����:the link type like the ethernet 80211 link dealer call this func to pass the
//    the package to the tcpip stack
//����:
//����:
//��ע:
//����:zhangqf@����3:55:30/2016��12��29��
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
//���ܣ���·����IP�������Э��push���ݣ���802.3��raw����·���豸���á�
//������iface�������豸ָ��
//      pkg�������͵İ�
//      protocol��Э����ţ��� IPV4��IPV6��ARP��RARP���Զ����
//���أ�true = �ϲ�Э��ɹ����ã�false = �ϲ�Э�����ʧ��
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
        case EN_LINKPROTO_IPV6: //��fnIPv4Deal(__IpPushNew)�У�Ҳ���ж�IP�汾����֧����
                                //�Ĵ��룬������δ���IPV6������
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

        default:    //TODO���˴�Ҫ�ģ���� pDrive-fnrcvhook ����
            __Link_RcvMonitor(iface,protocol,pkg);
            break;
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ������豸�������ݵ���·�㣬�������������ã�������ֻ�Ǹ�ת�ӣ���ת���������Ӧ����·
//      �㣨�� enum enLinkType ���壬���� ��̫���������ݽӿڵȣ�
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
//����:you could use this function to find a registered link type name
//����:type,which defines by enLinkType in socket.h
//����:
//��ע:
//����:zhangqf@����6:44:54/2017��1��3��
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
//����:use this function to register a link here
//����:
//����:
//��ע:
//����:zhangqf@����8:25:38/2016��12��28��
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
//����:use this function to unregister ther link we registered
//����:
//����:
//��ע:
//����:zhangqf@����1:55:39/2016��12��29��
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

