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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
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
#include "../common/link.h"

#include "../component_config_tcpip.h"

#include "../common/router.h"
#include "../common/netdev.h"

/////////////////////////////LINK MONITOR///////////////////////////////////////
//usage:this is the link monitor item data structure
typedef struct __rcvhookitem
{
    struct __rcvhookitem *nxt;  //used to do the queue
    const char *      name;         //may be you want to specify an name here
    u16               proto;        //the listen  protocol you have set
    void             *iface;    //the listen device you have set
    u32               framenum;     //how many frames has listen
    fnLinkProtoDealer hook;         //the listen hook you have set
}tagRcvHookItem;
typedef struct
{
    u32 itemtotal;
    u32 framenum;
    u32 frameunknown;
    tagRcvHookItem *list;
    mutex_t lock;
}tagRcvHookCB;
static tagRcvHookCB  gRcvHookCB;
//if you specified the special device, only the same device and the same proto frame
//will passed into the hook;if the devhandler is NULL,then any proto with the same proto
//will passed into the hook.
//you could use this function to register some link protocol to do the functions
bool_t LinkRegisterRcvHook(fnLinkProtoDealer hook,const char *ifname,u16 proto,const char *hookname)
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
    item->iface = NetDevGet(ifname);
    item->hook = hook;
    item->proto = proto;
    item->name = hookname;

    if(mutex_lock(gRcvHookCB.lock))
    {
        item->nxt = gRcvHookCB.list;
        gRcvHookCB.list = item;
        TCPIP_DEBUG_INC(gRcvHookCB.itemtotal);
        result = true;
        mutex_unlock(gRcvHookCB.lock);
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
bool_t LinkUnRegisterRcvHook(const char *hookname)
{
    bool_t result = false;
    tagRcvHookItem *item;
    tagRcvHookItem *nxt;

    if(mutex_lock(gRcvHookCB.lock))
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
        mutex_unlock(gRcvHookCB.lock);
    }
    return result;
}
//use this function to do hook protocol deal
static bool_t __RcvMonitor(void *iface,u16 proto,tagNetPkg *pkg)
{
    bool_t  result = false;
    fnLinkProtoDealer hook = NULL;
    tagRcvHookItem *item;
    //find the hook first
    if(mutex_lock(gRcvHookCB.lock))
    {
        item = gRcvHookCB.list;
        if(NetDevType(iface) == EN_LINK_ETHERNET)
        {
            //here we recover the link head for the ethernet(14 bytes here 6 macdst 6 macsrc 2 type)
            pkg->datalen += 14;
            pkg->offset -= 14;
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
        mutex_unlock(gRcvHookCB.lock);
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

//here define the link register and unregister here
typedef struct
{
    enLinkType  iftype;
    const char *name;
    tagLinkOps  ops;
}tagLink;
static tagLink  *pLinkHal[EN_LINK_LAST];
//-----------------------------------------------------------------------------
//����:use this function to find a link operation
//����:
//����:
//��ע:
//����:zhangqf@����8:22:20/2016��12��28��
//-----------------------------------------------------------------------------
static tagLinkOps *__FindOps(enLinkType type)
{
    tagLinkOps *ret = NULL;
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
bool_t LinkSend(void *iface,tagNetPkg *pkg,u32 framlen,u32 devtask,u16 proto,\
                enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc)
{
    bool_t ret = false;
    tagLinkOps *ops;
    ops = NetDevLinkOps(iface);
    if(NULL != ops)
    {
        ret = ops->linkout(iface,pkg,framlen,devtask,proto,ver,ipdst,ipsrc);
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
//bool_t LinkPost(void *iface,tagNetPkg *pkg)
//{
//    bool_t ret = false;
//    enLinkType linktype;
//    tagLinkOps *ops;
//
//    linktype = NetDevType(iface);
//    ops = __FindOps(linktype);
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
bool_t LinkPushRegister(u16 protocol,fnLinkProtoDealer dealer)
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
bool_t LinkPush(void  *iface,tagNetPkg *pkg,u16 protocol)
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
        case EN_LINKPROTO_IPV6:
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

        default:
            __RcvMonitor(iface,protocol,pkg);
            break;
    }
    return ret;
}
//the net device layer will call this function to do more deal
bool_t LinkDeal(void *iface,tagNetPkg *pkg)
{
    bool_t ret = false;
    tagLinkOps *ops;
    ops = NetDevLinkOps(iface);
    if(NULL != ops)
    {
        ret = ops->linkin(iface,pkg);
    }
    return ret;
}
//find a link ops here
void  *LinkFindOps(enLinkType type)
{
    return __FindOps(type);
}
//-----------------------------------------------------------------------------
//����:you could use this function to find a registered link type name
//����:type,which defines by enLinkType in socket.h
//����:
//��ע:
//����:zhangqf@����6:44:54/2017��1��3��
//-----------------------------------------------------------------------------
const char *LinkTypeName(enLinkType type)
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
bool_t LinkRegister(enLinkType type,const char *name,tagLinkOps *ops)
{
    bool_t result = false;
    tagLink *link = NULL;

    link = net_malloc(sizeof(tagLink));
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
bool_t LinkUnRegister(enLinkType type,const char *name,tagLinkOps *ops)
{
    bool_t result = false;
    tagLink *link = NULL;

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
static bool_t __LinkHookShell(void)
{
    int i = 0;
    tagRcvHookItem *item;

    debug_printf("link","LinkHookModule\n\r");
    if(mutex_lock(gRcvHookCB.lock))
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
                    item->framenum,NetDevName(item->iface));
            item = item->nxt;
        }
        mutex_unlock(gRcvHookCB.lock);
    }
    return true;
}


//usage:use this shell to show all the link type supported
static bool_t __LinkTypeShell(void)
{
    u8 i;
    tagLink *link;
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

ADD_TO_SHELL_HELP(net_link,"usage:net_link [-t (show types)]/[-h (show hooks)]");
ADD_TO_IN_SHELL bool_t net_link(char *param)
{
    bool_t ret = true;
    if((NULL == param)||(0 == strcmp(param,"-t")))
    {
        ret = __LinkTypeShell();
    }
    else if(0 == strcmp(param,"-h"))
    {
        ret = __LinkHookShell();
    }
    else
    {
        ret = false;
    }
    return ret;
}

//usage:this data used for the debug here
struct shell_debug  gLinkDebug[] =
{
    {
        "link",
        net_link,
        "usage:link [-t (show types)]/[-h (show hooks)]",
        "usage:link [-t (show types)]/[-h (show hooks)]",
    },
};
#define CN_LinkDebug_NUM  ((sizeof(gLinkDebug))/(sizeof(struct shell_debug)))
//static struct ShellCmdRsc gLinkDebugCmdRsc[CN_LinkDebug_NUM];

// =============================================================================
// FUNCTION   :this is the link module initialize function
// PARAMS IN  :
// PARAMS OUT :
// RETURN     :
// DESCRIPTION:uptils now, we do nothing here
// =============================================================================
bool_t LinkInit(void)
{
    //the gLinkHook module must be initialized
    memset((void *)&gRcvHookCB,0,sizeof(gRcvHookCB));
    gRcvHookCB.lock = mutex_init(NULL);
    shell_debug_add(gLinkDebug, CN_LinkDebug_NUM);
    return true;
}
