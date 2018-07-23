//-----------------------------------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

#include <netbsp.h>
#include <osarch.h>
#include "dbug.h"
#include "../component_config_tcpip.h"

#include "../common/link.h"


//first we should implement the device layer
typedef struct
{
    void                         *nxt;                    //dev chain
    char                           name[CN_TCPIP_NAMELEN]; //dev name
    u8                             iftype;   //dev type
    fnIfSend                       ifsend;   //dev snd function
    fnIfRecv                       ifrecv;   //dev receive function
    fnIfCtrl                       ifctrl;   //dev ctrl or stat get fucntion
    void                          *linkops;  //dev link operations
    fnNetDevEventHook              eventhook;//dev event hook dealer
    u32                            devfunc;  //dev hard function,such as tcp chksum
    u16                            mtu;      //dev mtu
    void                          *private;  //the dev driver use this to has its owner property
    u8                             mac[CN_MACADDR_LEN];   //mac address
    //the following used to debug the net device,show some main status
    u32                            pkgsnd;     //frame send
    u32                            pkgsnderr;  //frame snd failed
    u32                            pkgrcv;     //frame receive
    u32                            pkgrcverr;  //frame receive err
}tagNetDev;
typedef struct
{
    mutex_t   lock;
    tagNetDev *lst;
}tagNetDevCB; //interface controller
static tagNetDevCB gIfaceCB;
//this function used to get the specified dev
static tagNetDev* __IfGet(const char *name)
{
    tagNetDev* ret = NULL;
    tagNetDev* tmp = gIfaceCB.lst;
    if(NULL == name)
    {
        ret = gIfaceCB.lst;
    }
    else
    {
        while(NULL != tmp)
        {
            if(0 == strcmp(tmp->name,name))
            {
                ret = tmp;
                tmp = NULL;//end the search
            }
            else
            {
                tmp = tmp->nxt;
            }
        }
    }
    return ret;
}



//use this function to send a package from the net device layer
bool_t NetDevSend(void *iface,tagNetPkg *pkg,u32 framelen,u32 devtask)
{
    bool_t ret = false;
    tagNetDev* dev;
    dev = iface;
    if((NULL != dev)&&(NULL != dev->ifsend))
    {
        TCPIP_DEBUG_INC(((tagNetDev *)iface)->pkgsnd);
        ret =dev->ifsend(iface,pkg,framelen,devtask);
        if(ret == false)
        {
            TCPIP_DEBUG_INC(((tagNetDev *)iface)->pkgsnderr);
        }
    }
    return ret;
}
//use this function to receive a package from the net device layer
bool_t NetDevPush(void *iface,tagNetPkg *pkg)
{
    bool_t ret = false;
    if(NULL != iface)
    {
        TCPIP_DEBUG_INC(((tagNetDev *)iface)->pkgrcv);
        ret = LinkDeal(iface,pkg);
        if(ret == false)
        {
            TCPIP_DEBUG_INC(((tagNetDev *)iface)->pkgrcverr);
        }
    }
    return ret;
}
//get the interface name by the handle for external module
const char *NetDevName(void *iface)
{
    if(NULL!= iface)
    {
        return (const char *)(((tagNetDev* )iface)->name);
    }
    else
    {
        return NULL;
    }
}
//get the interface handle  by name for external module
void *NetDevGet(const char *ifname)
{
    void* ret = NULL;
    if(mutex_lock(gIfaceCB.lock))
    {
        ret = __IfGet(ifname);
        mutex_unlock(gIfaceCB.lock);
    }
    return ret;
}
//get the interface function for external module
u32 NetDevFunc(void *iface)
{
    if(NULL!= iface)
    {
        return ((tagNetDev* )iface)->devfunc;
    }
    else
    {
        return 0;
    }
}
//get the interface mtu for external module
u16 NetDevMtu(void *iface)
{
    if(NULL!= iface)
    {
        return ((tagNetDev* )iface)->mtu;
    }
    else
    {
        return 0;
    }
}
//get the interface type for external module
enLinkType NetDevType(void *iface)
{
    enLinkType ret = EN_LINK_LAST;
    if(NULL != iface)
    {
        ret = ((tagNetDev* )iface)->iftype;
    }
    return ret;
}
void *NetDevLinkOps(void *iface)
{
    void *ret = NULL;
    if(NULL != iface)
    {
        ret = ((tagNetDev* )iface)->linkops;
    }
    return ret;
}
const u8 *NetDevGetMac(void *iface)
{
    if(NULL!= iface)
    {
        return (const u8 *)(((tagNetDev* )iface)->mac);
    }
    else
    {
        return NULL;
    }
}

//the device event listen hook module
static bool_t __NetdevEventHook(void *iface,enNetDevEvent event)
{
    switch(event)
    {
        case EN_NETDEVEVENT_LINKDOWN:
            debug_printf("netdev","link is down\n\r");  //warning the app to do some notice
            break;
        case EN_NETDEVEVENT_LINKUP:
            debug_printf("netdev","link is up\n\r"); //warning the app to do some notice
            break;
        case EN_NETDEVEVENT_IPGET:
            debug_printf("netdev","ip get\n\r"); //warning the app to do some notice
            break;
        case EN_NETDEVEVENT_IPRELEASE:
            debug_printf("netdev","ip release\n\r"); //warning the app to do some notice
            break;
        case EN_NETDEVEVENT_BROAD_OVER:
            //should shutdown the receive broad frame of the net device
            debug_printf("netdev","broad strom has come,stop rcv the broadcast\n\r");
            break;
        case EN_NETDEVEVENT_BROAD_LACK:
            //should open the receive broad frame of the net device
            debug_printf("netdev","broad strom has gone,rcv the broadcast\n\r");
            break;

        case EN_NETDEVEVENT_MULTI_OVER:
            //should shutdown the receive broad frame of the net device
            debug_printf("netdev","MULTI strom has come,stop rcv the multi\n\r");
            break;
        case EN_NETDEVEVENT_MULTI_LACK:
            //should open the receive broad frame of the net device
            debug_printf("netdev","MULTI strom has gone,rcv the multi\n\r");
            break;
        case EN_NETDEVEVENT_POINT_OVER:
            //should shutdown the receive broad frame of the net device
            debug_printf("netdev","point strom has come,stop rcv the point\n\r");
            break;
        case EN_NETDEVEVENT_POINT_LACK:
            //should open the receive broad frame of the net device
            debug_printf("netdev","point strom has gone,rcv the point\n\r");
            break;
        case EN_NETDEVEVENT_FLOW_OVER:
            //maybe some err happened to the device,should reset
            debug_printf("netdev","flow over has come\n\r");
            break;
        case EN_NETDEVEVENT_FLOW_LACKNetDevSend:
            //the net work comes to normal
            debug_printf("netdev","flow lack has COME,reset the net device\n\r");
            break;
        case EN_NETDEVEVENT_RESET:
            debug_printf("netdev","dev reset\n\r"); //should do some log here
            break;
        default:
            debug_printf("netdev","unknown event\n\r"); //should ignore here
            break;
    }
    return true;
}

// =============================================================================
// FUNCTION   :install an net device to the stack
// PARAMS IN  :para, the net device function paras as tagNetDev *Para defined
// PARAMS OUT :NULL
// RETURN     :the dev handle
// DESCRIPTION:NO DUPLICATION NAME PERMITTED
// =============================================================================
void *NetDevInstall(tagNetDevPara *para)
{
    tagNetDev* iface = NULL;
    void *linkops;
    if(NULL == para)
    {
        return (void *)iface;
    }
    linkops = LinkFindOps(para->iftype);
    if(NULL == linkops)
    {
        return (void *)iface;
    }

    if(mutex_lock(gIfaceCB.lock))
    {
        iface = __IfGet(para->name);
        if(NULL == iface)
        {
            iface = net_malloc(sizeof(tagNetDev));
            if(NULL != iface)
            {
                //fill the dev
                memset(iface, 0, sizeof(tagNetDev));
                strncpy(iface->name,para->name,CN_TCPIP_NAMELEN);
                memcpy(iface->mac, para->mac, CN_MACADDR_LEN);
                iface->ifsend  = para->ifsend;
                iface->ifrecv = para->ifrecv;
                iface->ifctrl  = para->ifctrl;
                iface->linkops = linkops;
                iface->eventhook = __NetdevEventHook;
                iface->private = para->private;
                iface->iftype = para->iftype;
                iface->mtu= para->mtu;
                iface->devfunc= para->devfunc;
                //add it to the dev chain
                iface->nxt = gIfaceCB.lst;
                gIfaceCB.lst = iface;
            }
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return iface;
}
// =============================================================================
// FUNCTION   :remove the name specified net device
// PARAMS IN  :name, the net device name,which specified by installation
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
bool_t  NetDevUninstall(const char *name)
{
    tagNetDev* tmp;
    tagNetDev* bak;
    bool_t     result = false;

    if(mutex_lock(gIfaceCB.lock))
    {
        tmp = gIfaceCB.lst;
        bak = tmp;
        while(NULL != tmp)
        {
            if(0 == strcmp(tmp->name,name))
            {
                //bingo,we got it now
                //remove it from the dev chain
                if(tmp == gIfaceCB.lst)
                {
                    //the head one
                    gIfaceCB.lst = tmp->nxt;
                }
                else
                {
                    bak->nxt = tmp->nxt;
                }
                //net_free the device memory now
                net_free((void *)tmp);
                result = true;
                break;
            }
            else
            {
                bak = tmp;
                tmp = tmp->nxt;
            }
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return result;
}
// =============================================================================
// FUNCTION   :register a hook to do the device event here
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//             devname:the name of the net device that has been installed
//             hook:which will used to deal the net dev event
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevRegisterEventHook(void *handle,const char *devname,fnNetDevEventHook hook)
{
    bool_t result = false;
    tagNetDev* iface = NULL;
    if(mutex_lock(gIfaceCB.lock))
    {
        if(NULL != (void *)handle)
        {
            iface =(tagNetDev* )handle;
        }
        else
        {
            iface = __IfGet(devname);
        }
        if(NULL != iface)
        {
            iface->eventhook = hook;
            result = true;
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return result;
}
// =============================================================================
// FUNCTION   :Unregister the event hook  of the device
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//             devname:the name of the net device that has been installed
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevUnRegisterEventHook(ptu32_t handle,const char *devname)
{
    bool_t result = false;
    tagNetDev* dev = NULL;

    if(mutex_lock(gIfaceCB.lock))
    {
        if(NULL !=(void *) handle)
        {
            dev =(tagNetDev* )handle;
        }
        else
        {
            dev = __IfGet(devname);
        }
        if(NULL != dev)
        {
            dev->eventhook = NULL;
            result = true;
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return result;
}
// =============================================================================
// FUNCTION   :used to post a event to the net device
// PARAMS IN  :handle:the device handle returned by NetDevInstall
//             devname:the name of the net device that has been installed
//             event:the event to post
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:if handle NULL,then use the devname to search the net device
// =============================================================================
bool_t NetDevPostEvent(void* handle,const char *devname,enNetDevEvent event)
{
    bool_t result = false;
    tagNetDev* iface = NULL;
    fnNetDevEventHook hook= NULL;
    if(mutex_lock(gIfaceCB.lock))
    {
        if(NULL != (void *)handle)
        {
            iface = handle;
        }
        else
        {
            iface = __IfGet(devname);
        }
        if(NULL != iface)
        {
            hook =iface->eventhook;
        }
        mutex_unlock(gIfaceCB.lock);
        if(NULL != hook)
        {
            result = hook(iface,event);
        }
    }
    return result;
}
// =============================================================================
// FUNCTION   :ctrl the specified netdev
// PARAMS IN  :name, the net device name,which specified by installation
//             cmd, the ctrl cmd. which defined by tagNetDev *Cmd
//             para,used by the cmd, and its type defined by cmd
// PARAMS OUT :NULL
// RETURN     :true success while false failed
// DESCRIPTION:
// =============================================================================
bool_t  NetDevCtrl(const char *name,enNetDevCmd cmd, ptu32_t para)
{
    bool_t      ret = false;
    tagNetDev  *dev;

    dev = NetDevGet((const char *)name);
    if((NULL !=dev)&&(NULL != dev->ifctrl))
    {
        ret = dev->ifctrl(dev,cmd,para);
        if(ret &&(cmd == EN_NETDEV_SETMAC))
        {
            memcpy(dev->mac,(u8 *)para,CN_MACADDR_LEN);
        }
    }
    else
    {
        debug_printf("netdev","%s:no dev matched or no ctrl function\n\r",__FUNCTION__);
    }
    return ret;
}
//if you got the handle when install the net device, please use the handle
//it maybe more fast
bool_t  NetDevCtrlByHandle(void* handle,enNetDevCmd cmd, ptu32_t para)
{
    bool_t     ret = false;
    tagNetDev* dev;

    dev = (tagNetDev* )handle;
    if((NULL !=dev)&&(NULL != dev->ifctrl))
    {
        ret = dev->ifctrl(dev,cmd,para);
        if(ret &&(cmd == EN_NETDEV_SETMAC))
        {
            memcpy(dev->mac,(u8 *)para,CN_MACADDR_LEN);
        }
    }
    return ret;
}



// =============================================================================
// FUNCTION   :get the netdev private member
// PARAMS IN  :handle, the install returns handle
// PARAMS OUT :NULL
// RETURN     :the private data you installed
// DESCRIPTION:
// =============================================================================
void * NetDevPrivate(void *iface)
{
    void *ret = NULL;
    if(NULL != iface)
    {
        ret = ((tagNetDev *)iface)->private;
    }
    return ret;
}

//netdev shell here
static bool_t __IfconfigShell(char *param)
{
    int i = 0;
    tagNetDev *iface;
    OsPrintSplit('*',100);
    debug_printf("netdev","%-2s %-10s %-10s %-8s %-8s %-8s %-8s %-8s %-8s %-s\n\r",\
            "NO","NAME","TYPE","FUNCTION","MTU","SNDTOTAL","SNDERR","RCVTOTAL","RCVERR","MAC");
    if(mutex_lock(gIfaceCB.lock))
    {
        iface = gIfaceCB.lst;
        while(NULL != iface)
        {
            i++;
            debug_printf("netdev","%-2d %-10s %-10s %-8x %-8x %-8x %-8x %-8x %-8x %-s\n\r",\
                    i,iface->name,LinkTypeName(iface->iftype),iface->devfunc,iface->mtu,\
                    iface->pkgsnd,iface->pkgsnderr,iface->pkgrcv,iface->pkgrcverr,mac2string(iface->mac));
            iface = iface->nxt;
        }
        mutex_unlock(gIfaceCB.lock);
    }
    OsPrintSplit('*',100);
    return true;
}
struct ShellCmdTab  gIfaceDebug[] =
{
    {
        "ifconfig",
        __IfconfigShell,
        "usage:ifconfig",
        "usage:ifconfig",
    },
};
#define CN_IfaceDebug_NUM  ((sizeof(gIfaceDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gIfaceDebugCmdRsc[CN_IfaceDebug_NUM];
// =============================================================================
// FUNCTION   :this is the rout module initialize function
// PARAMS IN  :
// PARAMS OUT :
// RETURN     :
// DESCRIPTION:after this function,you could add some devices
// =============================================================================
bool_t NetDevInit(void)
{
    bool_t ret = false;
    gIfaceCB.lst = NULL;
    gIfaceCB.lock = mutex_init(NULL);
    if(NULL == gIfaceCB.lock)
    {
        goto EXIT_MUTEX;
    }
    ret = Sh_InstallCmd(gIfaceDebug,gIfaceDebugCmdRsc,CN_IfaceDebug_NUM);
    if(false == ret)
    {
        goto EXIT_ROUTCMD;
    }
    return ret;

EXIT_ROUTCMD:
    mutex_del(gIfaceCB.lock);
    gIfaceCB.lock = NULL;
EXIT_MUTEX:
    ret = false;
    return ret;
}






