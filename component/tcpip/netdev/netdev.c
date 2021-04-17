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
    struct NetDev      *NextDev;  //所有网卡组成单向循环链表
    struct RoutItem4   *v4lst;    //IPv4路由列表，单向NULL结束链表
    struct RoutItem6   *v6lst;    //IPv6路由列表，单向NULL结束链表
    char                name[CN_TCPIP_NAMELEN]; //dev name
    enum enLinkType     iftype;   //dev type
    fnIfSend            ifsend;   //提供给本地协议栈的发送函数
    struct ExtStackOps *ExtStack; //外部协议栈网卡的操作函数
//  fnIfRecv            ifrecv;   //dev receive function
    fnIfCtrl            ifctrl;   //dev ctrl or stat get fucntion
    struct LinkOps     *linkops;  //链路层收发函数，网卡中记录它，是为了快速查找
    fnNetDevEventHook   eventhook;//dev event hook dealer
    u32                 devfunc;  //网卡附加功能，参见netbsp.h中的 CN_IPDEV_TCPOCHKSUM 等定义
    u16                 mtu;      //dev mtu
    void                *Private;  //the dev driver use this to has its owner property
    u8                  mac[CN_MACADDR_LEN];   //mac address
    s32 (*cb_ip_got)(u32 *ip);  //dhcp client 获得IP后，通知用户的回调
//    s32 (*cb_ip_set)(u32 ip); //for dhcp (no use the discorver)
    //the following used to debug the net device,show some main status
    struct in_addr      gateway;    //本网卡对应的网关，在创建路由时设置，后设置的会取代先前设置的。
    struct in_addr      dns;        //首选dns，在创建路由时设置，后设置的会取代先前设置的。
    struct in_addr      dnsbak;     //备用dns，在创建路由时设置，后设置的会取代先前设置的。
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
//函数原名： NetDevPkgsndInc
void NetDev_PkgsndInc(struct NetDev *iface)
{
    iface->pkgsnd++;
}
//函数原名： NetDevPkgsndErrInc
void NetDev_PkgsndErrInc(struct NetDev *iface)
{
    iface->pkgsnderr++;
}
//函数原名：NetDevPkgrcvInc
void NetDev_PkgrcvInc(struct NetDev *iface)
{
    iface->pkgrcv++;
}
//函数原名：NetDevPkgrcvErrInc
void NetDev_PkgrcvErrInc(struct NetDev *iface)
{
    iface->pkgrcverr++;
}

struct ExtStackOps *NetDev_GetExtStackOps(struct NetDev *iface)
{
    return iface->ExtStack;
}

//-----------------------------------------------------------------------------
//功能: 通过名字获取网卡设备控制块指针。
//参数: name，网卡名，NULL则返回默认网卡
//返回: 网卡指针
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
//功能: 通过网卡设备控制块指针获取网卡名
//参数: DevFace，网络控制块指针
//返回: 网卡名
//-----------------------------------------------------------------------------
//函数原名： NetDevName
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
//功能: 通过名字获取网卡设备控制块指针。
//参数: name，网卡名，NULL则返回第一块网卡
//返回: 网卡指针
//-----------------------------------------------------------------------------
//函数原名： NetDevGet
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
//功能: 获取网卡附加功能，参见netbsp.h中的 CN_IPDEV_TCPOCHKSUM 等定义
//参数: DevFace，网络控制块指针
//返回: 网卡名
//-----------------------------------------------------------------------------
//函数原名： NetDevFunc
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
//函数原名： NetDevMtu
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
//功能: 获取网卡类型，参见netbsp.h中的 EN_LINK_LAST 等定义
//参数: DevFace，网络控制块指针
//返回: 网卡类型
//-----------------------------------------------------------------------------
//函数原名： NetDevType
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
//功能: 获取网卡链路层收发函数集指针
//参数: DevFace，网络控制块指针
//返回: 函数集指针
//-----------------------------------------------------------------------------
//函数原名： NetDevLinkOps
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
//功能: 设置当前默认网卡
//参数: NetDev，将要被设置成默认网卡的指针
//返回: 无
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
//功能: 获取当前默认网卡
//参数: 无
//返回: 默认网卡结构指针
//-----------------------------------------------------------------------------
struct NetDev * NetDev_GetDefault(void)
{
    return g_ptDefaultNetDev;
}

//-----------------------------------------------------------------------------
//功能: 获取网卡的Mac地址，按网络字节序的buffer。
//参数: DevFace，网络控制块指针
//返回: Mac地址指针
//-----------------------------------------------------------------------------
//函数原名： NetDevGetMac
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
//功能: 从默认网卡开始，遍历所有网卡
//参数: Current，当前网卡，若==NULL，则返回默认网卡，否则返回其 nxp 指针，若
//      若 DevFace->NextDev == 当前网卡，则返回NULL
//返回: 网卡控制块，遍历完成则返回NULL
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
//功能：取默认网卡的dns和dnsbak
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收首选dns地址，类型是 struct in_addr 或 struct in6_addr
//      addrbak：接收备用dns地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：设置默认网卡的dns地址
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收首选dns地址，类型是 struct in_addr 或 struct in6_addr
//      addrbak：接收备用dns地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：取指定网卡的dns和dnsbak
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收首选dns地址，类型是 struct in_addr 或 struct in6_addr
//      addrbak：接收备用dns地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：设置指定网卡的dns地址
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收首选dns地址，类型是 struct in_addr 或 struct in6_addr
//      addrbak：接收备用dns地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}


//------------------------------------------------------------------------------
//功能：取默认网卡的网关
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收gateway地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：取默认网卡的IP，若有多条路由，任取其中一个
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收IP地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ret = false;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：设置默认网卡的网关
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：gateway地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：取指定网卡的网关
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：接收gateway地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//------------------------------------------------------------------------------
//功能：设置指定网卡的网关
//参数：ver：IP版本，取 EN_IPV_4 或 EN_IPV_6
//      addr：gateway地址，类型是 struct in_addr 或 struct in6_addr
//返回：true= 成功；false= 失败。
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
            ;   //IPV6待完善
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 取网卡的IPv4 路由条目链表入口
//参数: NetDev，网卡指针
//返回: 网卡控制块，遍历完成则返回NULL
//-----------------------------------------------------------------------------
struct RoutItem4 *NetDev_GetIPv4RoutEntry(struct NetDev *NetDev)
{
    if(NetDev != NULL)
        return NetDev->v4lst;
    else
        return NULL;
}

//-----------------------------------------------------------------------------
//功能: 设置网卡的IPv4 路由条目链表入口
//参数: NetDev，网卡指针
//返回: 网卡控制块，遍历完成则返回NULL
//-----------------------------------------------------------------------------
void NetDev_SetIPv4RoutEntry(struct NetDev *NetDev, struct RoutItem4 *new_root)
{
    if(NetDev != NULL)
        NetDev->v4lst = new_root;
}

//-----------------------------------------------------------------------------
//功能: 取网卡的IPv6 路由条目链表入口
//参数: NetDev，网卡指针
//返回: 网卡控制块，遍历完成则返回NULL
//-----------------------------------------------------------------------------
struct RoutItem6 *NetDev_GetIPv6RoutEntry(struct NetDev *NetDev)
{
    if(NetDev != NULL)
        return NetDev->v6lst;
    else
        return NULL;
}

//-----------------------------------------------------------------------------
//功能: 设置网卡的IPv6 路由条目链表入口
//参数: NetDev，网卡指针
//返回: 网卡控制块，遍历完成则返回NULL
//-----------------------------------------------------------------------------
void NetDev_SetIPv6RoutEntry(struct NetDev *NetDev, struct RoutItem6 *new_root)
{
    if(NetDev != NULL)
        NetDev->v6lst = new_root;
}

//-----------------------------------------------------------------------------
//功能: 添加网卡的 IPv4 路由条目
//参数: NetDev，网卡指针
//返回: 无
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
//功能: 删除网卡的 IPv4 路由条目
//参数: NetDev，网卡指针
//返回: 无
//-----------------------------------------------------------------------------
struct RoutItem4 * __RemoveFromQueueV4(struct RoutItem4 *queue, struct RoutItem4 *item);
void NetDev_DelIPv4RoutItem(struct NetDev *NetDev, struct RoutItem4 *v4Item)
{
    NetDev->v4lst = __RemoveFromQueueV4(NetDev->v4lst, v4Item);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//-----------------------------------------------------------------------------
//功能: 添加网卡的 IPv6 路由条目
//参数: NetDev，网卡指针
//返回: 无
//-----------------------------------------------------------------------------
void NetDev_AddIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v6Item)
{
    //未实现
}

//-----------------------------------------------------------------------------
//功能: 删除网卡的 IPv6 路由条目
//参数: NetDev，网卡指针
//返回: 无
//-----------------------------------------------------------------------------
void NetDev_DelIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v4Item)
{
    //未实现
}

//-----------------------------------------------------------------------------
//功能: 取网卡IPv4的路由表
//参数: NetDev，网卡指针
//      v4Item，接收IP表，若=NULL则只计算路由条目数（即绑定的IP数）
//返回: 无
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
//功能: 取网卡IPv4的路由表
//参数: NetDev，网卡指针
//      v6Item，接收路IP表，若=NULL则只计算路由条目数（即绑定的IP数）
//返回: 无
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
//功能: 系统提供的网卡事件监听钩子，APP可以调用 NetDev_RegisterEventHook 函数重新设置，
//      只打印信息，其他什么都不做。
//参数: iface，网络控制块指针
//      event，网卡事件，参见 EN_NETDEVEVENT_LINKDOWN 等定义
//返回: true
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
//功能: 安装网卡。
//参数: para，参数，见 struct NetDevPara 定义
//返回: 网卡控制块指针
//-----------------------------------------------------------------------------
//函数原名： NetDevInstall
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
                //新增加的网卡排在默认网卡的后面，并不立即变成默认网卡
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
//功能: 卸载网卡。
//参数: name，网卡名
//返回: true  or false
//-----------------------------------------------------------------------------
//函数原名： NetDevUninstall
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
            while(bak->NextDev != tmp)      //bak指向默认网卡的前一个网卡
                bak = bak->NextDev;
            if(bak == tmp)                  //只有一个网卡
                g_ptDefaultNetDev = NULL;
            else
            {
                do
                {
                    if(0 == strcmp(tmp->name,name))
                    {
                        bak->NextDev = tmp->NextDev;
                        if(tmp == g_ptDefaultNetDev)    //卸载默认网卡
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
//功能: 设置网卡事件钩子函数，若不设置，则使用默认的 __NetdevEventHook 函数
//参数: handle，由 NetDev_Install 返回的网卡控制块指针
//      hook，钩子函数
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevRegisterEventHook
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
//功能: 卸载网卡事件钩子函数,卸载后，使用默认的 __NetdevEventHook 函数
//参数: handle，由 NetDev_Install 返回的网卡控制块指针
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevUnRegisterEventHook
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
//功能: 抛出网卡事件，将由 dev->eventhook 函数响应
//参数: handle，由 NetDev_Install 返回的网卡控制块指针
//      event，被抛出的网卡事件
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevPostEvent
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
//功能: 网卡发送函数，
//参数: handle，由 NetDev_Install 返回的网卡控制块指针
//      pkglst，待发送的包
//      framlen，发送数据长度
//      netdevtask，需要网卡完成的任务，如 CN_IPDEV_NONE 等的定义
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevSend
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
//功能: 网卡控制函数，功能由 cmd 决定。
//参数: handle，由 NetDev_Install 返回的网卡控制块指针
//      cmd，命令码, enum NetDevCmd 定义
//      para，命令参数
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevCtrl
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
//功能：配置网卡的流量控制参数
//参数：handle，网卡设备句柄
//     type，被控制的包类型，netbsp.h中定义
//     llimit，流量下限，在period时间内，收到type类型的包数低于此限，即 NetDev_PostEvent 网卡事件
//     ulimit，流量上限，在period时间内，收到type类型的包数超过此限，即 NetDev_PostEvent 网卡事件
//     period，流量统计周期
//     enable :1 true while 0 false
// 返回：true success while false failed
// =============================================================================
//函数原名： NetDevFlowSet
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
// 功能：根据Mac地址，检查帧类型，广播、多播、单播
// 参数：buf, the frame buffer
//      len，buf中有效数据长度
// 返回：帧类型
// =============================================================================
//函数原名： NetDevFrameType
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

//TODO：这里有个问题，只有收到数据包才调用本函数，流量下限检查将失效，可考虑去掉下限检查功能
//函数原名： NetDevFlowCtrl
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
//功能：设置获取IP后的回调函数，该回调函数用于告知用户某块网卡成功获取了IP
//参数：ifname，网卡名
//      cb_ip_got，用户提供的回调函数
//返回：-1 = 出错；非-1 = 成功
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
//功能：把通过 DHCP 获得的IP地址用回调函数通知用户
//参数：pNetDev，得到IP地址的网卡
//      ip_temp，分配到的IP地址
//返回：1 = 成功，0 = 失败
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
//函数原名： NetDevFlowStat
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
//功能: 取网卡的私有指针
//参数: DevFace，由 NetDev_Install 返回的网卡控制块指针
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevPrivate
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
//功能: 网络设备接口初始化
//参数: 无
//返回: true or false
//-----------------------------------------------------------------------------
//函数原名： NetDevInit
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
