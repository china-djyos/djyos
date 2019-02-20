//-----------------------------------------------------------------------------
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

#include <netbsp.h>
#include <osarch.h>
#include "dbug.h"
#include <shell.h>
#include "../component_config_tcpip.h"

#include "../common/link.h"

//first we should implement the device layer
struct NetDev
{
    void                *nxt;                    //dev chain
    char                name[CN_TCPIP_NAMELEN]; //dev name
    u8                  iftype;   //dev type
    fnIfSend            ifsend;   //dev snd function
//  fnIfRecv            ifrecv;   //dev receive function
    fnIfCtrl            ifctrl;   //dev ctrl or stat get fucntion
    struct LinkOps     *linkops;  //dev link operations
    fnNetDevEventHook   eventhook;//dev event hook dealer
    u32                 devfunc;  //dev hard function,such as tcp chksum
    u16                 mtu;      //dev mtu
    void                *Private;  //the dev driver use this to has its owner property
    u8                  mac[CN_MACADDR_LEN];   //mac address
    //the following used to debug the net device,show some main status
    u32                 pkgsnd;     //frame send
    u32                 pkgsnderr;  //frame snd failed
    u32                 pkgrcv;     //frame receive
    u32                 pkgrcverr;  //frame receive err
};
typedef struct
{
    mutex_t   lock;
    struct NetDev *lst;
}tagNetDevCB; //interface controller
static tagNetDevCB gIfaceCB;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void NetDevPkgsndInc(struct NetDev *iface)
{
    iface->pkgsnd++;
}

void NetDevPkgsndErrInc(struct NetDev *iface)
{
    iface->pkgsnderr++;
}

void NetDevPkgrcvInc(struct NetDev *iface)
{
    iface->pkgrcv++;
}

void NetDevPkgrcvErrInc(struct NetDev *iface)
{
    iface->pkgrcverr++;
}

//-----------------------------------------------------------------------------
//����: ͨ�����ֻ�ȡ�����豸���ƿ�ָ�롣
//����: name����������NULL�򷵻ص�һ������
//����: ����ָ��
//-----------------------------------------------------------------------------
static struct NetDev* __IfGet(const char *name)
{
    struct NetDev* ret = NULL;
    struct NetDev* tmp = gIfaceCB.lst;
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
//bool_t NetDevSend(struct NetDev *iface,struct NetPkg *pkg,u32 framelen,u32 devtask)
//{
//    bool_t ret = false;
//
//    if((NULL != iface)&&(NULL != iface->ifsend))
//    {
//        TCPIP_DEBUG_INC((iface)->pkgsnd);
//        ret =iface->ifsend(iface,pkg,framelen,devtask);
//        if(ret == false)
//        {
//            TCPIP_DEBUG_INC((iface)->pkgsnderr);
//        }
//    }
//    return ret;
//}
//use this function to receive a package from the net device layer
//bool_t NetDevPush(void *iface,struct NetPkg *pkg)
//{
//    bool_t ret = false;
//    if(NULL != iface)
//    {
//        TCPIP_DEBUG_INC(((struct NetDev *)iface)->pkgrcv);
//        ret = LinkDeal(iface,pkg);
//        if(ret == false)
//        {
//            TCPIP_DEBUG_INC(((struct NetDev *)iface)->pkgrcverr);
//        }
//    }
//    return ret;
//}

//-----------------------------------------------------------------------------
//����: ͨ�������豸���ƿ�ָ���ȡ������
//����: DevFace��������ƿ�ָ��
//����: ������
//-----------------------------------------------------------------------------
const char *NetDevName(struct NetDev *DevFace)
{
    if(NULL!= DevFace)
    {
        return (const char *)(DevFace->name);
    }
    else
    {
        return NULL;
    }
}

//-----------------------------------------------------------------------------
//����: ͨ�����ֻ�ȡ�����豸���ƿ�ָ�롣
//����: name����������NULL�򷵻ص�һ������
//����: ����ָ��
//-----------------------------------------------------------------------------
struct NetDev *NetDevGet(const char *ifname)
{
    struct NetDev * ret = NULL;
    if(mutex_lock(gIfaceCB.lock))
    {
        ret = __IfGet(ifname);
        mutex_unlock(gIfaceCB.lock);
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����: ��ȡ�������ӹ��ܣ��μ�netbsp.h�е� CN_IPDEV_TCPOCHKSUM �ȶ���
//����: DevFace��������ƿ�ָ��
//����: ������
//-----------------------------------------------------------------------------
u32 NetDevFunc(struct NetDev *DevFace)
{
    if(NULL!= DevFace)
    {
        return DevFace->devfunc;
    }
    else
    {
        return 0;
    }
}
//get the interface mtu for external module
u16 NetDevMtu(struct NetDev *DevFace)
{
    if(NULL!= DevFace)
    {
        return (DevFace)->mtu;
    }
    else
    {
        return 0;
    }
}
//-----------------------------------------------------------------------------
//����: ��ȡ�������ͣ��μ�netbsp.h�е� EN_LINK_LAST �ȶ���
//����: DevFace��������ƿ�ָ��
//����: ��������
//-----------------------------------------------------------------------------
enum enLinkType NetDevType(struct NetDev *DevFace)
{
    enum enLinkType ret = EN_LINK_LAST;
    if(NULL != DevFace)
    {
        ret = (DevFace)->iftype;
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����: ��ȡ������·���շ�������ָ��
//����: DevFace��������ƿ�ָ��
//����: ������ָ��
//-----------------------------------------------------------------------------
struct LinkOps *NetDevLinkOps(struct NetDev *DevFace)
{
    struct LinkOps *ret = NULL;
    if(NULL != DevFace)
    {
        ret = (DevFace)->linkops;
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����: ��ȡ������Mac��ַ���������ֽ����buffer��
//����: DevFace��������ƿ�ָ��
//����: Mac��ַָ��
//-----------------------------------------------------------------------------
const u8 *NetDevGetMac(struct NetDev *DevFace)
{
    if(NULL!= DevFace)
    {
        return (const u8 *)((DevFace)->mac);
    }
    else
    {
        return NULL;
    }
}

//-----------------------------------------------------------------------------
//����: Ĭ�������¼��������ӣ�APP���Ե��� NetDevRegisterEventHook �����������ã�ֻ��ӡ
//      ��Ϣ������ʲô��������
//����: iface��������ƿ�ָ��
//      event�������¼����μ� EN_NETDEVEVENT_LINKDOWN �ȶ���
//����: true
//-----------------------------------------------------------------------------
static bool_t __NetdevEventHook(struct NetDev *iface,enum NetDevEvent event)
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

//-----------------------------------------------------------------------------
//����: ��װ������
//����: para���������� struct NetDevPara ����
//����: �������ƿ�ָ��
//-----------------------------------------------------------------------------
struct NetDev* NetDevInstall(struct NetDevPara *para)
{
    struct NetDev* iface = NULL;
    struct LinkOps *linkops;
    if(NULL == para)
    {
        return iface;
    }
    linkops = LinkFindOps(para->iftype);
    if(NULL == linkops)
    {
        return iface;
    }

    if(mutex_lock(gIfaceCB.lock))
    {
        iface = __IfGet(para->name);
        if(NULL == iface)
        {
            iface = net_malloc(sizeof(struct NetDev));
            if(NULL != iface)
            {
                //fill the dev
                memset(iface, 0, sizeof(struct NetDev));
                strncpy(iface->name,para->name,CN_TCPIP_NAMELEN);
                memcpy(iface->mac, para->mac, CN_MACADDR_LEN);
                iface->ifsend  = para->ifsend;
//              iface->ifrecv = para->ifrecv;
                iface->ifctrl  = para->ifctrl;
                iface->linkops = linkops;
                iface->eventhook = __NetdevEventHook;
                iface->Private = para->Private;
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

//-----------------------------------------------------------------------------
//����: ж��������
//����: name��������
//����: true  or false
//-----------------------------------------------------------------------------
bool_t  NetDevUninstall(const char *name)
{
    struct NetDev* tmp;
    struct NetDev* bak;
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

//-----------------------------------------------------------------------------
//����: ���������¼����Ӻ������������ã���ʹ��Ĭ�ϵ� __NetdevEventHook ����
//����: handle���� NetDevInstall ���ص��������ƿ�ָ��
//      hook�����Ӻ���
//����: true or false
//-----------------------------------------------------------------------------
bool_t NetDevRegisterEventHook(struct NetDev *handle,fnNetDevEventHook hook)
{
    bool_t result = false;
    if(mutex_lock(gIfaceCB.lock))
    {
        if(NULL != handle)
        {
            handle->eventhook = hook;
            result = true;
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: ж�������¼����Ӻ���,ж�غ�ʹ��Ĭ�ϵ� __NetdevEventHook ����
//����: handle���� NetDevInstall ���ص��������ƿ�ָ��
//����: true or false
//-----------------------------------------------------------------------------
bool_t NetDevUnRegisterEventHook(struct NetDev * handle)
{
    bool_t result = false;

    if(mutex_lock(gIfaceCB.lock))
    {
        if(NULL != handle)
        {
            handle->eventhook = __NetdevEventHook;
            result = true;
        }
        mutex_unlock(gIfaceCB.lock);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: �׳������¼������� dev->eventhook ������Ӧ
//����: handle���� NetDevInstall ���ص��������ƿ�ָ��
//      event�����׳��������¼�
//����: true or false
//-----------------------------------------------------------------------------
bool_t NetDevPostEvent(struct NetDev* handle,enum NetDevEvent event)
{
    bool_t result = false;
    struct NetDev* iface = NULL;
    fnNetDevEventHook hook= NULL;
    if(NULL != handle)
    {
        return false;
    }
    if(mutex_lock(gIfaceCB.lock))
    {
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

//-----------------------------------------------------------------------------
//����: �������ͺ�����
//����: handle���� NetDevInstall ���ص��������ƿ�ָ��
//      pkglst�������͵İ�
//      framlen���������ݳ���
//      netdevtask����������
//����: true or false
//-----------------------------------------------------------------------------
bool_t  NetDevSend(struct NetDev* handle,struct NetPkg *pkglst,u32 framlen,u32 netdevtask)
{
    bool_t     ret = false;

    if(NULL !=handle)
    {
        ret = handle->ifsend(handle,pkglst,framlen,netdevtask);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: �������ƺ����������� cmd ������
//����: handle���� NetDevInstall ���ص��������ƿ�ָ��
//      cmd��������
//      para���������
//����: true or false
//-----------------------------------------------------------------------------
bool_t  NetDevCtrl(struct NetDev* handle,enNetDevCmd cmd, ptu32_t para)
{
    bool_t     ret = false;
    struct NetDev* dev;

    dev = handle;
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

//-----------------------------------------------------------------------------
//����: ȡ������˽��ָ��
//����: DevFace���� NetDevInstall ���ص��������ƿ�ָ��
//����: true or false
//-----------------------------------------------------------------------------
void * NetDevPrivate(struct NetDev *iface)
{
    void *ret = NULL;
    if(NULL != iface)
    {
        ret = (iface)->Private;
    }
    return ret;
}

//netdev shell here
//static bool_t __IfconfigShell(char *param)
bool_t ifconfig(char *param)
{
    int i = 0;
    struct NetDev *iface;
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
//-----------------------------------------------------------------------------
//����: �����豸�ӿڳ�ʼ��
//����: ��
//����: true or false
//-----------------------------------------------------------------------------
bool_t NetDevInit(void)
{
    bool_t ret = false;
    gIfaceCB.lst = NULL;
    gIfaceCB.lock = mutex_init(NULL);
    if(NULL == gIfaceCB.lock)
    {
        goto EXIT_MUTEX;
    }

    ret = true;
    return ret;


EXIT_MUTEX:
    ret = false;
    return ret;
}

ADD_TO_ROUTINE_SHELL(ifconfig,ifconfig,"usage:ifconfig");
#pragma GCC diagnostic pop
