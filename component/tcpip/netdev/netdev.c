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
#include <net/arpa/inet.h>
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
    struct NetDev      *NextDev;  //����������ɵ���ѭ������
    struct RoutItem4   *v4lst;    //IPv4·���б�����NULL��������
    struct RoutItem6   *v6lst;    //IPv6·���б�����NULL��������
    char                name[CN_TCPIP_NAMELEN]; //dev name
    enum enLinkType     iftype;   //dev type
    fnIfSend            ifsend;   //�ṩ������Э��ջ�ķ��ͺ���
    struct ExtStackOps *ExtStack; //�ⲿЭ��ջ�����Ĳ�������
//  fnIfRecv            ifrecv;   //dev receive function
    fnIfCtrl            ifctrl;   //dev ctrl or stat get fucntion
    struct LinkOps     *linkops;  //��·���շ������������м�¼������Ϊ�˿��ٲ���
    fnNetDevEventHook   eventhook;//dev event hook dealer
    u32                 devfunc;  //�������ӹ��ܣ��μ�netbsp.h�е� CN_IPDEV_TCPOCHKSUM �ȶ���
    u16                 mtu;      //dev mtu
    void                *Private;  //the dev driver use this to has its owner property
    u8                  mac[CN_MACADDR_LEN];   //mac address
    s32 (*cb_ip_got)(u32 *ip);  //dhcp client ���IP��֪ͨ�û��Ļص�
//    s32 (*cb_ip_set)(u32 ip); //for dhcp (no use the discorver)
    //the following used to debug the net device,show some main status
    struct in_addr      gateway;    //��������Ӧ�����أ��ڴ���·��ʱ���ã������õĻ�ȡ����ǰ���õġ�
    struct in_addr      dns;        //��ѡdns���ڴ���·��ʱ���ã������õĻ�ȡ����ǰ���õġ�
    struct in_addr      dnsbak;     //����dns���ڴ���·��ʱ���ã������õĻ�ȡ����ǰ���õġ�
    u32                 pkgsnd;     //frame send
    u32                 pkgsnderr;  //frame snd failed
    u32                 pkgrcv;     //frame receive
    u32                 pkgrcverr;  //frame receive err
    tagNetDevRcvFilter  rfilter[EN_NETDEV_FRAME_LAST];  //the recv filter
};

static struct NetDev *g_ptDefaultNetDev;
static struct MutexLCB* g_ptNetDevLock;

struct in_addr __Router_PickIPv4(struct RoutItem4 *Item);
struct in6_addr __Router_PickIPv6(struct RoutItem6 *Item);
struct RoutItem4 *__Router_GetNextV4(struct RoutItem4 *Item);
struct RoutItem6 *__Router_GetNextV6(struct RoutItem6 *Item);
//����ԭ���� NetDevPkgsndInc
void NetDev_PkgsndInc(struct NetDev *iface)
{
    iface->pkgsnd++;
}
//����ԭ���� NetDevPkgsndErrInc
void NetDev_PkgsndErrInc(struct NetDev *iface)
{
    iface->pkgsnderr++;
}
//����ԭ����NetDevPkgrcvInc
void NetDev_PkgrcvInc(struct NetDev *iface)
{
    iface->pkgrcv++;
}
//����ԭ����NetDevPkgrcvErrInc
void NetDev_PkgrcvErrInc(struct NetDev *iface)
{
    iface->pkgrcverr++;
}

struct ExtStackOps *NetDev_GetExtStackOps(struct NetDev *iface)
{
    return iface->ExtStack;
}

//-----------------------------------------------------------------------------
//����: ͨ�����ֻ�ȡ�����豸���ƿ�ָ�롣
//����: name����������NULL�򷵻�Ĭ������
//����: ����ָ��
//-----------------------------------------------------------------------------
static struct NetDev* __NetDevGet(const char *name)
{
    struct NetDev* ret = NULL;
    struct NetDev* tmp = g_ptDefaultNetDev;
    if(NULL == name)
    {
        ret = g_ptDefaultNetDev;
    }
    else if(g_ptDefaultNetDev != NULL)
    {
        do
        {
            if(0 == strcmp(tmp->name,name))
            {
                ret = tmp;
                break;
            }
            else
            {
                tmp = tmp->NextDev;
            }
        }while(g_ptDefaultNetDev != tmp);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ͨ�������豸���ƿ�ָ���ȡ������
//����: DevFace��������ƿ�ָ��
//����: ������
//-----------------------------------------------------------------------------
//����ԭ���� NetDevName
const char *NetDev_GetName(struct NetDev *DevFace)
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
//����ԭ���� NetDevGet
struct NetDev *NetDev_GetHandle(const char *ifname)
{
    struct NetDev * ret = NULL;
    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        ret = __NetDevGet(ifname);
        Lock_MutexPost(g_ptNetDevLock);
    }
    return ret;
}
//-----------------------------------------------------------------------------
//����: ��ȡ�������ӹ��ܣ��μ�netbsp.h�е� CN_IPDEV_TCPOCHKSUM �ȶ���
//����: DevFace��������ƿ�ָ��
//����: ������
//-----------------------------------------------------------------------------
//����ԭ���� NetDevFunc
u32 NetDev_GetFunc(struct NetDev *DevFace)
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
//����ԭ���� NetDevMtu
u16 NetDev_GetMtu(struct NetDev *DevFace)
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
//����ԭ���� NetDevType
enum enLinkType NetDev_GetType(struct NetDev *DevFace)
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
//����ԭ���� NetDevLinkOps
struct LinkOps *NetDev_GetLinkOps(struct NetDev *DevFace)
{
    struct LinkOps *ret = NULL;
    if(NULL != DevFace)
    {
        ret = (DevFace)->linkops;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ���õ�ǰĬ������
//����: NetDev����Ҫ�����ó�Ĭ��������ָ��
//����: ��
//-----------------------------------------------------------------------------
void NetDev_SetDefault(struct NetDev *NetDev)
{
    struct NetDev *current;
    current = g_ptDefaultNetDev;
    if ((NetDev != NULL) && (current != NULL))
    {
        do
        {
            if(current == NetDev)
            {
                g_ptDefaultNetDev = NetDev;
                break;
            }
            current = current->NextDev;
        }while(current != g_ptDefaultNetDev);
    }
    return;
}

//-----------------------------------------------------------------------------
//����: ��ȡ��ǰĬ������
//����: ��
//����: Ĭ�������ṹָ��
//-----------------------------------------------------------------------------
struct NetDev * NetDev_GetDefault(void)
{
    return g_ptDefaultNetDev;
}

//-----------------------------------------------------------------------------
//����: ��ȡ������Mac��ַ���������ֽ����buffer��
//����: DevFace��������ƿ�ָ��
//����: Mac��ַָ��
//-----------------------------------------------------------------------------
//����ԭ���� NetDevGetMac
const u8 *NetDev_GetMac(struct NetDev *DevFace)
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
//����: ��Ĭ��������ʼ��������������
//����: Current����ǰ��������==NULL���򷵻�Ĭ�����������򷵻��� nxp ָ�룬��
//      �� DevFace->NextDev == ��ǰ�������򷵻�NULL
//����: �������ƿ飬��������򷵻�NULL
//-----------------------------------------------------------------------------
struct NetDev *NetDev_ForEachFromDefault(struct NetDev *Current)
{
    if(Current == NULL)
        return g_ptDefaultNetDev;
    else
    {
        if((g_ptDefaultNetDev != NULL) && (Current->NextDev != g_ptDefaultNetDev))
        {
            return Current->NextDev;
        }
        else
        {
            return NULL;
        }
    }
}

//------------------------------------------------------------------------------
//���ܣ�ȡĬ��������dns��dnsbak
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��������ѡdns��ַ�������� struct in_addr �� struct in6_addr
//      addrbak�����ձ���dns��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_GetDefaultDns(enum_ipv_t ver,void *addr,void *addrbak)
{
    bool_t ret = false;
    if(g_ptDefaultNetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(addr, &g_ptDefaultNetDev->dns, sizeof(struct in_addr));
            }
            if(NULL != addrbak)
            {
                memcpy(addrbak,&g_ptDefaultNetDev->dnsbak,sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�����Ĭ��������dns��ַ
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��������ѡdns��ַ�������� struct in_addr �� struct in6_addr
//      addrbak�����ձ���dns��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_SetDefaultDns(enum_ipv_t ver,void *addr,void *addrbak)
{
    bool_t ret = false;
    if(g_ptDefaultNetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(&g_ptDefaultNetDev->dns, addr, sizeof(struct in_addr));
                debug_printf("dns","DNSSET:1:%s\n\r",inet_ntoa(*(struct in_addr*)addr));
            }
            if(NULL != addrbak)
            {
                memcpy(&g_ptDefaultNetDev->dnsbak,addrbak,sizeof(struct in_addr));
                debug_printf("dns","DNSSET:2:%s\n\r",inet_ntoa(*(struct in_addr*)addrbak));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�ȡָ��������dns��dnsbak
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��������ѡdns��ַ�������� struct in_addr �� struct in6_addr
//      addrbak�����ձ���dns��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_GetDns(enum_ipv_t ver,struct NetDev *NetDev,void *addr,void *addrbak)
{
    bool_t ret = false;
    if(NetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(addr, &NetDev->dns, sizeof(struct in_addr));
            }
            if(NULL != addrbak)
            {
                memcpy(addrbak,&NetDev->dnsbak,sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�����ָ��������dns��ַ
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��������ѡdns��ַ�������� struct in_addr �� struct in6_addr
//      addrbak�����ձ���dns��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_SetDns(enum_ipv_t ver,struct NetDev *NetDev,void *addr,void *addrbak)
{
    bool_t ret = false;
    if(NetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(&NetDev->dns, addr, sizeof(struct in_addr));
                debug_printf("dns","DNS1 SET:%s\n\r",inet_ntoa(*(struct in_addr*)addr));
            }
            if(NULL != addrbak)
            {
                memcpy(&NetDev->dnsbak,addrbak,sizeof(struct in_addr));
                debug_printf("dns","DNS2 SET:%s\n\r",inet_ntoa(*(struct in_addr*)addrbak));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}


//------------------------------------------------------------------------------
//���ܣ�ȡĬ������������
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr������gateway��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_GetDefaultGateway(enum_ipv_t ver,void *addr)
{
    bool_t ret = false;
    if(g_ptDefaultNetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(addr, &g_ptDefaultNetDev->gateway, sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�ȡĬ��������IP�����ж���·�ɣ���ȡ����һ��
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr������IP��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_GetDefaultIP(enum_ipv_t ver,void *addr)
{
    bool_t ret = false;
    struct in_addr myaddr;
    if(g_ptDefaultNetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if((NULL != addr) && (g_ptDefaultNetDev->v4lst != NULL))
            {
                myaddr = __Router_PickIPv4(g_ptDefaultNetDev->v4lst);
                memcpy(addr, &myaddr, sizeof(struct in_addr));
                ret = true;
            }
            else
                ret = false;
        }
        else
        {
            ret = false;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�����Ĭ������������
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��gateway��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_SetDefaultGateway(enum_ipv_t ver,void *addr)
{
    bool_t ret = false;
    if(g_ptDefaultNetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(&g_ptDefaultNetDev->gateway, addr, sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�ȡָ������������
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr������gateway��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_GetGateway(enum_ipv_t ver,struct NetDev *NetDev,void *addr)
{
    bool_t ret = false;
    if(NetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(addr, &NetDev->gateway, sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//���ܣ�����ָ������������
//������ver��IP�汾��ȡ EN_IPV_4 �� EN_IPV_6
//      addr��gateway��ַ�������� struct in_addr �� struct in6_addr
//���أ�true= �ɹ���false= ʧ�ܡ�
//------------------------------------------------------------------------------
bool_t NetDev_SetGateway(enum_ipv_t ver,struct NetDev *NetDev,void *addr)
{
    bool_t ret = false;
    if(NetDev != NULL)
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != addr)
            {
                memcpy(&NetDev->gateway, addr, sizeof(struct in_addr));
            }
            ret = true;
        }
        else
        {
            ;   //IPV6������
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
//����: ȡ������IPv4 ·����Ŀ�������
//����: NetDev������ָ��
//����: �������ƿ飬��������򷵻�NULL
//-----------------------------------------------------------------------------
struct RoutItem4 *NetDev_GetIPv4RoutEntry(struct NetDev *NetDev)
{
    if(NetDev != NULL)
        return NetDev->v4lst;
    else
        return NULL;
}

//-----------------------------------------------------------------------------
//����: ����������IPv4 ·����Ŀ�������
//����: NetDev������ָ��
//����: �������ƿ飬��������򷵻�NULL
//-----------------------------------------------------------------------------
void NetDev_SetIPv4RoutEntry(struct NetDev *NetDev, struct RoutItem4 *new_root)
{
    if(NetDev != NULL)
        NetDev->v4lst = new_root;
}

//-----------------------------------------------------------------------------
//����: ȡ������IPv6 ·����Ŀ�������
//����: NetDev������ָ��
//����: �������ƿ飬��������򷵻�NULL
//-----------------------------------------------------------------------------
struct RoutItem6 *NetDev_GetIPv6RoutEntry(struct NetDev *NetDev)
{
    if(NetDev != NULL)
        return NetDev->v6lst;
    else
        return NULL;
}

//-----------------------------------------------------------------------------
//����: ����������IPv6 ·����Ŀ�������
//����: NetDev������ָ��
//����: �������ƿ飬��������򷵻�NULL
//-----------------------------------------------------------------------------
void NetDev_SetIPv6RoutEntry(struct NetDev *NetDev, struct RoutItem6 *new_root)
{
    if(NetDev != NULL)
        NetDev->v6lst = new_root;
}

//-----------------------------------------------------------------------------
//����: ��������� IPv4 ·����Ŀ
//����: NetDev������ָ��
//����: ��
//-----------------------------------------------------------------------------
struct RoutItem4 *__Add2Queue(struct RoutItem4 *queue, struct RoutItem4 *item);
void NetDev_AddIPv4RoutItem(struct NetDev *NetDev, struct RoutItem4 *v4Item)
{
    if(NetDev != NULL)
    {
        NetDev->v4lst = __Add2Queue(NetDev->v4lst, v4Item);
    }
}

//-----------------------------------------------------------------------------
//����: ɾ�������� IPv4 ·����Ŀ
//����: NetDev������ָ��
//����: ��
//-----------------------------------------------------------------------------
struct RoutItem4 * __RemoveFromQueueV4(struct RoutItem4 *queue, struct RoutItem4 *item);
void NetDev_DelIPv4RoutItem(struct NetDev *NetDev, struct RoutItem4 *v4Item)
{
    NetDev->v4lst = __RemoveFromQueueV4(NetDev->v4lst, v4Item);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//-----------------------------------------------------------------------------
//����: ��������� IPv6 ·����Ŀ
//����: NetDev������ָ��
//����: ��
//-----------------------------------------------------------------------------
void NetDev_AddIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v6Item)
{
    //δʵ��
}

//-----------------------------------------------------------------------------
//����: ɾ�������� IPv6 ·����Ŀ
//����: NetDev������ָ��
//����: ��
//-----------------------------------------------------------------------------
void NetDev_DelIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v4Item)
{
    //δʵ��
}

//-----------------------------------------------------------------------------
//����: ȡ����IPv4��·�ɱ�
//����: NetDev������ָ��
//      v4Item������IP����=NULL��ֻ����·����Ŀ�������󶨵�IP����
//����: ��
//-----------------------------------------------------------------------------
u32 NetDev_GetIPv4Table(struct NetDev *NetDev, struct in_addr *v4Item)
{
    struct RoutItem4 *rt4;
    u32 num = 0;
    if(NetDev != NULL)
    {
        rt4 = NetDev->v4lst;
        while(rt4 != NULL)
        {
            if(v4Item != NULL)
                v4Item[num] = __Router_PickIPv4(rt4);
            num++;
            rt4 = __Router_GetNextV4(rt4);
        }
    }
    return num;
}

//-----------------------------------------------------------------------------
//����: ȡ����IPv4��·�ɱ�
//����: NetDev������ָ��
//      v6Item������·IP����=NULL��ֻ����·����Ŀ�������󶨵�IP����
//����: ��
//-----------------------------------------------------------------------------
u32 NetDev_GetIPv6Table(struct NetDev *NetDev, struct in6_addr *v6Item)
{
    struct RoutItem6 *rt6;
    u32 num = 0;
    if(NetDev != NULL)
    {
        rt6 = NetDev->v6lst;
        while(rt6 != NULL)
        {
            if(v6Item != NULL)
                v6Item[num] = __Router_PickIPv6(rt6);
            num++;
            rt6 = __Router_GetNextV6(rt6);
        }
    }
    return num;
}

//-----------------------------------------------------------------------------
//����: ϵͳ�ṩ�������¼��������ӣ�APP���Ե��� NetDev_RegisterEventHook �����������ã�
//      ֻ��ӡ��Ϣ������ʲô��������
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
#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//����: ��װ������
//����: para���������� struct NetDevPara ����
//����: �������ƿ�ָ��
//-----------------------------------------------------------------------------
//����ԭ���� NetDevInstall
struct NetDev* NetDev_Install(struct NetDevPara *para)
{
    struct NetDev* iface = NULL;
    struct LinkOps *linkops;
    if(NULL == para)
    {
        return iface;
    }
    linkops = Link_FindOps(para->iftype);
    if(NULL == linkops)
    {
        return iface;
    }

    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
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
                iface->rfilter[EN_NETDEV_FRAME_ALL  ].en = false;
                iface->rfilter[EN_NETDEV_FRAME_ALL  ].overevent =EN_NETDEVEVENT_FLOW_OVER;
                iface->rfilter[EN_NETDEV_FRAME_ALL  ].lackevent =EN_NETDEVEVENT_FLOW_LACK;
                //add it to the dev chain
//              iface->NextDev = gIfaceCB.NetDevLst;
//              gIfaceCB.NetDevLst = iface;
                //�����ӵ���������Ĭ�������ĺ��棬�����������Ĭ������
                if(g_ptDefaultNetDev == NULL)
                {
                    iface->NextDev = iface;
                    g_ptDefaultNetDev = iface;
                }
                else
                {
                    iface->NextDev = g_ptDefaultNetDev->NextDev;
                    g_ptDefaultNetDev->NextDev = iface;
                }
            }
            else
            {
                //no more mem for the dev, so failed
                printf("%s:failed--no mem\r\n",__FUNCTION__);
            }
        }
        Lock_MutexPost(g_ptNetDevLock);
    }
    return iface;
}

//-----------------------------------------------------------------------------
//����: ж��������
//����: name��������
//����: true  or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevUninstall
bool_t  NetDev_Uninstall(const char *name)
{
    struct NetDev* tmp;
    struct NetDev* bak;
    bool_t     result = false;

    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        tmp = g_ptDefaultNetDev;
        if(tmp != NULL)
        {
            bak = tmp;
            while(bak->NextDev != tmp)      //bakָ��Ĭ��������ǰһ������
                bak = bak->NextDev;
            if(bak == tmp)                  //ֻ��һ������
                g_ptDefaultNetDev = NULL;
            else
            {
                do
                {
                    if(0 == strcmp(tmp->name,name))
                    {
                        bak->NextDev = tmp->NextDev;
                        if(tmp == g_ptDefaultNetDev)    //ж��Ĭ������
                        {
                            g_ptDefaultNetDev = tmp->NextDev;
                        }
                        break;
                    }
                    else
                    {
                        bak = tmp;
                        tmp = tmp->NextDev;
                    }
                }while(g_ptDefaultNetDev != tmp);
            }
            net_free((void *)tmp);
            result = true;
        }
        Lock_MutexPost(g_ptNetDevLock);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: ���������¼����Ӻ������������ã���ʹ��Ĭ�ϵ� __NetdevEventHook ����
//����: handle���� NetDev_Install ���ص��������ƿ�ָ��
//      hook�����Ӻ���
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevRegisterEventHook
bool_t NetDev_RegisterEventHook(struct NetDev *handle,fnNetDevEventHook hook)
{
    bool_t result = false;
    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        if(NULL != handle)
        {
            handle->eventhook = hook;
            result = true;
        }
        Lock_MutexPost(g_ptNetDevLock);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: ж�������¼����Ӻ���,ж�غ�ʹ��Ĭ�ϵ� __NetdevEventHook ����
//����: handle���� NetDev_Install ���ص��������ƿ�ָ��
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevUnRegisterEventHook
bool_t NetDev_UnRegisterEventHook(struct NetDev * handle)
{
    bool_t result = false;

    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        if(NULL != handle)
        {
            handle->eventhook = __NetdevEventHook;
            result = true;
        }
        Lock_MutexPost(g_ptNetDevLock);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: �׳������¼������� dev->eventhook ������Ӧ
//����: handle���� NetDev_Install ���ص��������ƿ�ָ��
//      event�����׳��������¼�
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevPostEvent
bool_t NetDev_PostEvent(struct NetDev* handle,enum NetDevEvent event)
{
    bool_t result = false;
    struct NetDev* iface = handle;
    fnNetDevEventHook hook= NULL;
    if(NULL == handle)
    {
        return false;
    }
    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        if(NULL != iface)
        {
            hook =iface->eventhook;
        }
        Lock_MutexPost(g_ptNetDevLock);
        if(NULL != hook)
        {
            result = hook(iface,event);
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
//����: �������ͺ�����
//����: handle���� NetDev_Install ���ص��������ƿ�ָ��
//      pkglst�������͵İ�
//      framlen���������ݳ���
//      netdevtask����Ҫ������ɵ������� CN_IPDEV_NONE �ȵĶ���
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevSend
bool_t  NetDev_Send(struct NetDev* handle,struct NetPkg *pkglst,u32 netdevtask)
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
//����: handle���� NetDev_Install ���ص��������ƿ�ָ��
//      cmd��������, enum NetDevCmd ����
//      para���������
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevCtrl
bool_t  NetDev_Ctrl(struct NetDev* handle,enum NetDevCmd cmd, ptu32_t para)
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
//     llimit���������ޣ���periodʱ���ڣ��յ�type���͵İ������ڴ��ޣ��� NetDev_PostEvent �����¼�
//     ulimit���������ޣ���periodʱ���ڣ��յ�type���͵İ����������ޣ��� NetDev_PostEvent �����¼�
//     period������ͳ������
//     enable :1 true while 0 false
// ���أ�true success while false failed
// =============================================================================
//����ԭ���� NetDevFlowSet
bool_t NetDev_FlowSet(struct NetDev* handle,enum EthFramType type,\
                     u32 llimit,u32 ulimit,u32 period,s32 enable)
{
    bool_t result = false;
    tagNetDevRcvFilter *filter;

    if(Lock_MutexPend(g_ptNetDevLock, CN_TIMEOUT_FOREVER))
    {
        if(NULL != handle)
        {
            filter = &handle->rfilter[type];
            filter->period = period;
            filter->fulimit =ulimit;
            filter->fllimit =llimit;
            filter->fcounter = 0;
            filter->deadtime = DJY_GetSysTime() + period;
            filter->en = enable?1:0;
            result = true;
        }
        Lock_MutexPost(g_ptNetDevLock);
    }
    return result;
}

// =============================================================================
// ���ܣ�����Mac��ַ�����֡���ͣ��㲥���ಥ������
// ������buf, the frame buffer
//      len��buf����Ч���ݳ���
// ���أ�֡����
// =============================================================================
//����ԭ���� NetDevFrameType
enum EthFramType NetDev_FrameType(u8 *buf,u16 len)
{
    enum EthFramType result = EN_NETDEV_FRAME_LAST;
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
                NetDev_PostEvent(handle,filter->lackevent);//post a lack message
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
                NetDev_PostEvent(handle,filter->overevent);//post a begin message
            }
        }
    }
}

//TODO�������и����⣬ֻ���յ����ݰ��ŵ��ñ��������������޼�齫ʧЧ���ɿ���ȥ�����޼�鹦��
//����ԭ���� NetDevFlowCtrl
bool_t NetDev_FlowCtrl(struct NetDev* handle,enum EthFramType type)
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
        timenow = DJY_GetSysTime();
        for(looptype =0;looptype < EN_NETDEV_FRAME_LAST;looptype++)
        {
            filter = &handle->rfilter[looptype];
            __NetDevFlowCheck(handle,filter,timenow);
        }
    }
    return result;
}

//------------------------------------------------------------------------------
//���ܣ����û�ȡIP��Ļص��������ûص��������ڸ�֪�û�ĳ�������ɹ���ȡ��IP
//������ifname��������
//      cb_ip_got���û��ṩ�Ļص�����
//���أ�-1 = ������-1 = �ɹ�
//------------------------------------------------------------------------------
s32 NetDev_DHCP_SET_GotIP_CB(const char *ifname, s32 (*cb_ip_got)(u32 *ip))
{
    struct NetDev *pNetDev = NetDev_GetHandle(ifname);
    if(pNetDev==NULL) return  -1;
    pNetDev->cb_ip_got =  cb_ip_got;
    return 1;
}

//s32 dhcp_setip_cb(const char *ifname, s32 (*cb_ip_set)(u32 ip))
//{
//    struct NetDev *pNetDev = NetDev_GetHandle(ifname);
//    if(pNetDev==NULL) return  -1;
//    pNetDev->cb_ip_set = cb_ip_set;
//    return 1;
//}

//s32 net_get_dhcp_ip(struct NetDev *pNetDev, u32 *ip_temp)
//{
//    if (pNetDev && pNetDev->cb_ip_get && pNetDev->cb_ip_get(ip_temp)){
//        return 1;
//    }
//    return 0;
//}

//------------------------------------------------------------------------------
//���ܣ���ͨ�� DHCP ��õ�IP��ַ�ûص�����֪ͨ�û�
//������pNetDev���õ�IP��ַ������
//      ip_temp�����䵽��IP��ַ
//���أ�1 = �ɹ���0 = ʧ��
//------------------------------------------------------------------------------
s32 __NetDev_DHCP_GotIP(struct NetDev *pNetDev, u32 ip_temp)
{
    if (pNetDev && pNetDev->cb_ip_got && pNetDev->cb_ip_got(&ip_temp)){
        return 1;
    }
    else
        return 0;
}



const char *pFilterItemName[EN_NETDEV_FRAME_LAST]=
{
    "BROAD",
    "POINT",
    "MULTI",
    "FRAME",
};
//we use this function to show the net device filter state
//����ԭ���� NetDevFlowStat
static bool_t NetDev_FlowStat(char *param)
{
    bool_t      result = false;
    struct NetDev* handle;
    tagNetDevRcvFilter *filter;
    u32         type;

    handle = NetDev_GetHandle(param);
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
//����: DevFace���� NetDev_Install ���ص��������ƿ�ָ��
//����: true or false
//-----------------------------------------------------------------------------
//����ԭ���� NetDevPrivate
void * NetDev_GetPrivate(struct NetDev *iface)
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
    s32 i = 0;
    struct NetDev *iface;
    OsPrintSplit('*',100);
    debug_printf("netdev","%-2s %-10s %-10s %-8s %-8s %-8s %-8s %-8s %-8s %-s\n\r",\
            "NO","NAME","TYPE","FUNCTION","MTU","SNDTOTAL","SNDERR","RCVTOTAL","RCVERR","MAC");
    if(Lock_MutexPend(g_ptNetDevLock,CN_TIMEOUT_FOREVER))
    {
        iface = g_ptDefaultNetDev;
        if(iface != NULL)
        {
            do
            {
                i++;
                debug_printf("netdev","%-2d %-10s %-10s %-8x %-8x %-8x %-8x %-8x %-8x %-s\n\r",\
                        i,iface->name,Link_TypeName(iface->iftype),iface->devfunc,iface->mtu,\
                        iface->pkgsnd,iface->pkgsnderr,iface->pkgrcv,iface->pkgrcverr,mac2string(iface->mac));
                iface = iface->NextDev;
            }while(g_ptDefaultNetDev != iface);
            Lock_MutexPost(g_ptNetDevLock);
        }
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
//����ԭ���� NetDevInit
bool_t NetDev_Init(void)
{
    bool_t ret = false;
    g_ptDefaultNetDev = NULL;
    g_ptNetDevLock = Lock_MutexCreate(NULL);
    if(NULL == g_ptNetDevLock)
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
ADD_TO_ROUTINE_SHELL(netflow,NetDev_FlowStat,"usage:netflow [net card name]");
