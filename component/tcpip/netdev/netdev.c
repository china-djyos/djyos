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

typedef struct
{
//  vu32       enable:1;             //enable it or not
//  vu32       uaction:1;            //if more than the upperlimit then set it
//  vu32       laction:1;            //if less than lower limit then set it
//  vu32       lactionb:5;           //low begin message
//  vu32       lactione:5;           //low end message
//  vu32       uactionb:5;           //upper begin message
//  vu32       uactione:5;           //upper end message

    bool_t     en;                   //means the filter en or not
    bool_t     overaction;           //means over the upper limit and action now
    bool_t     lackaction;           //means less than the floor limit and action
    vu32       overevent;            //means over action event to post
    vu32       lackevent;            //means lack action event to post
    vu32       actiontimes;          //which means the action total times
    vu32       fcounter;             //which means how many frame received
    vu32       fulimit;              //which means the upper limit
    vu32       fllimit;              //which means the lower limit
    vu32       period;               //which means the measure period
    vu64       ftotal;               //which means the total frame of the type
    vs64       deadtime;             //which means the measure end time
}tagNetDevRcvFilter;
//first we should implement the device layer
struct NetDev
{
    struct NetDev      *nxt;                    //dev chain
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
    tagNetDevRcvFilter  rfilter[EN_NETDEV_FRAME_LAST];  //the recv filter
};
struct NetDevCB
{
    mutex_t   lock;
    struct NetDev *lst;
}; //interface controller
static struct NetDevCB gIfaceCB;

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
static struct NetDev* __NetDevGet(const char *name)
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
        ret = __NetDevGet(ifname);
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
        case EN_NETDEVEVENT_FLOW_LACK:
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
        iface = __NetDevGet(para->name);
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
                //initialize the dev filter part
                iface->rfilter[EN_NETDEV_FRAME_POINT].en = false;
                iface->rfilter[EN_NETDEV_FRAME_POINT].overevent =EN_NETDEVEVENT_POINT_OVER;
                iface->rfilter[EN_NETDEV_FRAME_POINT].lackevent =EN_NETDEVEVENT_POINT_LACK;
                iface->rfilter[EN_NETDEV_FRAME_MULTI].en = false;
                iface->rfilter[EN_NETDEV_FRAME_MULTI].overevent =EN_NETDEVEVENT_MULTI_OVER;
                iface->rfilter[EN_NETDEV_FRAME_MULTI].lackevent =EN_NETDEVEVENT_MULTI_LACK;
                iface->rfilter[EN_NETDEV_FRAME_BROAD].en = false;
                iface->rfilter[EN_NETDEV_FRAME_BROAD].overevent =EN_NETDEVEVENT_BROAD_OVER;
                iface->rfilter[EN_NETDEV_FRAME_BROAD].lackevent =EN_NETDEVEVENT_BROAD_LACK;
                iface->rfilter[EN_NETDEV_FRAME_ALL   ].en = false;
                iface->rfilter[EN_NETDEV_FRAME_ALL   ].overevent =EN_NETDEVEVENT_FLOW_OVER;
                iface->rfilter[EN_NETDEV_FRAME_ALL   ].lackevent =EN_NETDEVEVENT_FLOW_LACK;
                //add it to the dev chain
                iface->nxt = gIfaceCB.lst;
                gIfaceCB.lst = iface;
            }
            else
            {
                //no more mem for the dev, so failed
                printf("%s:failed--no mem\r\n",__FUNCTION__);
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
    struct NetDev* iface = handle;
    fnNetDevEventHook hook= NULL;
    if(NULL == handle)
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
bool_t  NetDevSend(struct NetDev* handle,struct NetPkg *pkglst,u32 netdevtask)
{
    bool_t     ret = false;

    if(NULL !=handle)
    {
        ret = handle->ifsend(handle,pkglst,netdevtask);
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
bool_t  NetDevCtrl(struct NetDev* handle,enum NetDevCmd cmd, ptu32_t para)
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

// =============================================================================
//���ܣ������������������Ʋ���
//������handle�������豸���
//     type�������Ƶİ����ͣ�netbsp.h�ж���
//     llimit���������ޣ���periodʱ���ڣ��յ�type���͵İ������ڴ��ޣ������������¼�
//     ulimit���������ޣ���periodʱ���ڣ��յ�type���͵İ����������ޣ������������¼�
//     period������ͳ������
//     enable :1 true while 0 false
// ���أ�true success while false failed
// =============================================================================
bool_t NetDevFlowSet(struct NetDev* handle,enNetDevFramType type,\
                     u32 llimit,u32 ulimit,u32 period,int enable)
{
    bool_t result = false;
    tagNetDevRcvFilter *filter;

    if(Lock_MutexPend(gIfaceCB.lock, CN_TIMEOUT_FOREVER))
    {
        if(NULL != handle)
        {
            filter = &handle->rfilter[type];
            filter->period = period;
            filter->fulimit =ulimit;
            filter->fllimit =llimit;
            filter->fcounter = 0;
            filter->deadtime = DjyGetSysTime() + period;
            filter->en = enable?1:0;
            result = true;
        }
        Lock_MutexPost(gIfaceCB.lock);
    }
    return result;
}

// =============================================================================
// FUNCTION   :use this function to check the frame type
// PARAMS IN  :buf, the frame buffer
// PARAMS OUT :
// RETURN     :enNetDevFramType,which type of the frame
// DESCRIPTION:
// =============================================================================
enNetDevFramType NetDevFrameType(u8 *buf,u16 len)
{
    enNetDevFramType result = EN_NETDEV_FRAME_LAST;
    if((NULL != buf)&&(len > CN_MACADDR_LEN))
    {
        if(0 == memcmp(buf,CN_MAC_BROAD,CN_MACADDR_LEN))
        {
            result = EN_NETDEV_FRAME_BROAD;
        }
        else if(buf[0]&0x01)
        {
            result = EN_NETDEV_FRAME_MULTI;
        }
        else
        {
            result = EN_NETDEV_FRAME_POINT;
        }
    }
    return result;
}

//THE OLD ONE IS NOT CORRECT, ONLY THE FRAME BETWEEN THE UPPER AND FLOOR LIMIT IS PROPER
static void __NetDevFlowCheck(struct NetDev* handle,tagNetDevRcvFilter *filter,s64 timenow)
{
    if(filter->en)
    {
        if(timenow > filter->deadtime) //should check if the floor limit is reached
        {
            //check the upper limit
            if(filter->fllimit > filter->fcounter)
            {
                NetDevPostEvent(handle,filter->lackevent);//post a lack message
                filter->lackaction = true;
                filter->actiontimes++;
            }
            else
            {
                filter->lackaction = false;
            }
            filter->overaction = false;
            filter->fcounter = 0;
            filter->deadtime = timenow + filter->period;
        }
        else
        {
            //check the upper limit
            if(filter->fulimit < filter->fcounter)
            {
                filter->actiontimes++;
                filter->fcounter = 0;
                filter->overaction = true;
                NetDevPostEvent(handle,filter->overevent);//post a begin message
            }
        }
    }
}
bool_t NetDevFlowCtrl(struct NetDev* handle,enNetDevFramType type)
{
    bool_t result = false;
    tagNetDevRcvFilter *filter;
    s64 timenow;
    u32 looptype;
    if(NULL != handle)
    {
        if(type < EN_NETDEV_FRAME_LAST)
        {
            filter = &handle->rfilter[type];
            if(filter->en)
            {
                filter->fcounter ++;
                filter->ftotal++;
            }
            //any data will inc the frame filter
            if(type != EN_NETDEV_FRAME_ALL   )
            {
                filter = &handle->rfilter[EN_NETDEV_FRAME_ALL   ];
                if(filter->en)
                {
                    filter->fcounter ++;
                    filter->ftotal++;
                }
            }
        }
        //check all the filter
        timenow = DjyGetSysTime();
        for(looptype =0;looptype < EN_NETDEV_FRAME_LAST;looptype++)
        {
            filter = &handle->rfilter[looptype];
            __NetDevFlowCheck(handle,filter,timenow);
        }
    }
    return result;
}


const char *pFilterItemName[EN_NETDEV_FRAME_LAST]=
{
    "BROAD",
    "POINT",
    "MULTI",
    "FRAME",
};
//we use this function to show the net device filter state
static bool_t NetDevFlowStat(char *param)
{
    bool_t      result = false;
    struct NetDev* handle;
    tagNetDevRcvFilter *filter;
    u32         type;

    handle = NetDevGet(param);
    if(NULL != handle)
    {
        printf("NETDEVFLOW:UNIT FOR CYCLE AND DEADLINE IS US\n\r");
        printf("%-6s%-3s%-3s%-3s%-9s%-9s%-9s%-9s%-9s%-9s%-9s\n\r",\
                "Name","EN","LS","US","FC","FU","FL","ACTIONS","CYCLE","DEADLINE","FT");
        for(type =0; type < EN_NETDEV_FRAME_LAST;type++)
        {
            filter = &handle->rfilter[type];
            printf("%-6s%-3s%-3s%-3s%-8x %-8x %-8x %-8x %-8x %-8llx %-8llx\n\r",\
                    pFilterItemName[type],\
                    filter->en==true?"Y":"N",\
                    filter->lackaction?"Y":"N",\
                    filter->overaction?"Y":"N",\
                    filter->fcounter,\
                    filter->fulimit,\
                    filter->fllimit,\
                    filter->actiontimes,\
                    filter->period,\
                    filter->deadtime,\
                    filter->ftotal
                    );
        }
        result =true;
    }
    return result;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
#pragma GCC diagnostic pop

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
ADD_TO_ROUTINE_SHELL(netflow,NetDevFlowStat,"usage:netflow [net card name]");
