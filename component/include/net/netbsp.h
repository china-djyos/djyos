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
#ifndef TCPIP_NETINCLUDE_NETBSP_H_
#define TCPIP_NETINCLUDE_NETBSP_H_

#include <stdint.h>
#include <stddef.h>

#define CN_ETH_MTU      1400
//һ��PKGLST�ڴ���Ĺ����У���ĳ��PKGӵ��CN_PKLGLST_END��ǻ���NULL == partnext��������Ϊ
//��PKGLST�������������ڷ��͵�ʱ����������
#define CN_PKLGLST_END   (1<<0)
struct NetPkg;
struct NetDev;
struct RoutItem4;
struct RoutItem6;

//used to defines the net device task
#define CN_IPDEV_TCPOCHKSUM  (1<<0)
#define CN_IPDEV_TCPICHKSUM  (1<<1)
#define CN_IPDEV_UDPOCHKSUM  (1<<2)
#define CN_IPDEV_UDPICHKSUM  (1<<3)
#define CN_IPDEV_ICMPOCHKSUM (1<<4)
#define CN_IPDEV_ICMPICHKSUM (1<<5)
#define CN_IPDEV_IPOCHKSUM   (1<<6)
#define CN_IPDEV_IPICHKSUM   (1<<7)
#define CN_IPDEV_IPBROAD     (1<<10)   //which means do the broadcast
#define CN_IPDEV_IPMULTI     (1<<11)   //which means do the multicast
#define CN_IPDEV_IPUNI       (1<<12)   //which means do the unicast
#define CN_IPDEV_ALL         (0xFFFFFFFF)
#define CN_IPDEV_NONE        (0X0)

#define CN_MACADDR_LEN       6
#define CN_MAC_BROAD         "\xff\xff\xff\xff\xff\xff"
//enum the ip version
typedef enum
{
    EN_IPV_4 =4,
    EN_IPV_6 =6,
}enum_ipv_t;


/////////////////////////////////USED FOR THE INTERFACE///////////////////////////////////////

enum NetDevCmd
{
    EN_NETDEV_SETNOPKG = 0,     //PARA IS NOT CARE
    EN_NETDEV_SETBORAD,         //para is int,0 disable else enable
    EN_NETDEV_SETPOINT,         //para is int,0 disable else enable
    EN_NETDEV_SETMULTI,         //para is int,0 disable else enable
    EN_NETDEV_SETRECV,          //para is int,0 disable else enable
    EN_NETDEV_SETSEND,          //para is int,0 disable else enable
    EN_NETDEV_SETMAC,           //para point to an buf which contains the mac
                                //driver must modify the dev struct mac at the same time
    EN_NETDEV_SETMULTIMAC,      //para point to an buf which contains the mac
    EN_NETDEV_SETHOOK,          //para is a receive hook fucntion name
    EN_NETDEV_GTETMAC,          //para point to an buf which used to contain the mac
    EN_NETDEV_RESET,            //para must be true
    EN_NETDEV_LOWPOWER,         //para is int,0 disable else enable
    EN_NETDEV_ADDRFILTER,       //��������Mac��ַ���˹���
    EN_NETDEV_CMDLAST,          //which means the max command
};

enum EthFramType
{
    EN_NETDEV_FRAME_BROAD = 0,  //�㲥������
    EN_NETDEV_FRAME_POINT,      //�ಥ������
    EN_NETDEV_FRAME_MULTI,      //����������
    EN_NETDEV_FRAME_ALL,        //�����������ݰ�����
    EN_NETDEV_FRAME_LAST,
};

enum enLinkType
{
    EN_LINK_ETHERNET = 0,  //ethenet net device,ethernetII���� 802.3
    EN_LINK_RAW,           //raw,just do the ip frames,no other link
    EN_LINK_80211,
    EN_LINK_LAST,
};

enum enLinkProto
{
    EN_LINKPROTO_IPV4    = 0x0800,
    EN_LINKPROTO_IPV6    = 0x86dd,
    EN_LINKPROTO_ARP     = 0x0806,
    EN_LINKPROTO_RARP    = 0x8035,
    EN_LINKPROTO_RESBASE = 0x1000,
};
//usage:for the net device event
//      all these event are edge-triggled
enum NetDevEvent
{
    //THE OLD ONES ARE DEPRECATED
    EN_NETDEVEVENT_LINKDOWN = 0, //����ֹͣ����������ϵ���߰γ�����
    EN_NETDEVEVENT_LINKUP,       //���������������ͨ��Դ���������
    EN_NETDEVEVENT_IPGET,        //from dhcp or ppp�����IP
    EN_NETDEVEVENT_IPRELEASE,    //release ip to dhcp or ppp down
    EN_NETDEVEVENT_RESET,        //������ʱ���λ
    EN_NETDEVEVENT_BROAD_OVER,   //means the broad over
    EN_NETDEVEVENT_BROAD_LACK,   //means the broad lack
    EN_NETDEVEVENT_MULTI_OVER,   //means the multi over
    EN_NETDEVEVENT_MULTI_LACK,   //means the multi lack,
    EN_NETDEVEVENT_POINT_OVER,   //means the point over
    EN_NETDEVEVENT_POINT_LACK,   //means the point lack,
    EN_NETDEVEVENT_FLOW_OVER,    //means the FLOW over
    EN_NETDEVEVENT_FLOW_LACK,    //means the FLOW lack,
    EN_NETDEVEVENT_RESERVED,     //which means nothing
};
typedef bool_t (*fnNetDevEventHook)(struct NetDev* iface,enum NetDevEvent event);
//net device type
//netdev snd module function
//return means the data has put out or put into the net card buffer
//pkg maybe an lst or not,you could use the PkgIsEnd to check
//pkglen is fram len
typedef bool_t (*fnIfSend)(struct NetDev* iface,struct NetPkg *pkglst,u32 netdevtask);
//typedef struct NetPkg* (*fnIfRecv)(struct NetDev* iface);

//used to ctrl the dev or get the dev stat
//��������ο� EN_NETDEV_SETNOPKG ��ö�ٳ���
typedef bool_t (*fnIfCtrl)(struct NetDev* iface,enum NetDevCmd cmd,ptu32_t para);
struct NetDevPara
{
    const char    *name;     //dev name
    enum enLinkType iftype;  //dev type
    fnIfSend       ifsend;   //dev snd function
//  fnIfRecv       ifrecv;   //dev receive function���ݲ��ã����Ƕ�·����
    fnIfCtrl       ifctrl;   //dev ctrl or stat get fucntion
    u32            devfunc;  //�������ӹ��ܣ��μ�netbsp.h�е� CN_IPDEV_TCPOCHKSUM �ȶ���
    u16            mtu;      //dev mtu
    void          *Private;  //the dev driver use this to has its owner property
    u8             mac[CN_MACADDR_LEN];   //mac address
};

//////////////////////USED FOR THE ROUTER//////////////////////////////////
//if you need to add a rout to the host,then the following will be used
//this defines for the external applications
#define CN_ROUT_PRO_G         (1<<0)
#define CN_ROUT_PRO_U         (1<<1)
#define CN_ROUT_PRO_R         (1<<2)
#define CN_ROUT_PRO_V         (1<<3)
#define CN_ROUT_PRO_S         (1<<4)
#define CN_ROUT_PRO_C         (1<<5)
#define CN_ROUT_PRO_B         (1<<6)
#define CN_ROUT_PRO_M         (1<<7)
#define CN_ROUT_PRIOR_LOOP    (10) //ANY INTERNAL ROUT WILL USE THIS
#define CN_ROUT_PRIOR_UNI     (5)  //ALMOST THE HIGHEST
#define CN_ROUT_PRIOR_ANY     (0)  //COULD MATCH ANY DESTINATION


//add a route to the hosts
typedef struct
{
    enum_ipv_t ver;
    const char *ifname;
    void *mask;
    void *net;
    void *host;
    void *hop;
    void *dns;
    void *dnsbak;
    u8  prior;
    u16 flags;
    u16 mtu;
}tagRouterPara;

typedef struct
{
    u32 ip;
    u32 dns;
    u32 gatway;
    u32 submask;
    u32 dnsbak;
    u32 subnet;
    //above member has been abandoned
    u32 multi;
    u32 hop;
    u32 host;
    u32 mask;
    u32 net;
}tagHostAddrV4;
typedef struct
{
    u32 ip[4];
    u32 dns[4];
    u32 gatway[4];
    u32 subnetmask[4];
    u32 broad[4];
    u32 subnet[4];
}tagHostAddrV6;
#define CN_ROUT_DHCP    (1<<0)  //use this bit to get ip address from the dhcp server
#define CN_ROUT_NONE    (0)

struct NetPkg *PkgMalloc(u16 bufsize,u8 flags);
bool_t     PkgTryFreePart(struct NetPkg *pkg);
//bool_t     PkgTryFreeLst(struct NetPkg  *pkglst);
bool_t     PkgTryFreeQ(struct NetPkg  *pkglst);
bool_t     PkgCachedPart(struct NetPkg  *pkg);
bool_t     PkgCachedLst(struct NetPkg   *pkglst);
void PkgInit(struct NetPkg *pkg, u8 flag, u16 offset, u16 datalen, u8* buf);
//struct NetPkg *PkgMalloc(u16 bufsize, u8 flags);
//bool_t PkgTryFreePart(struct NetPkg *pkg);
struct NetPkg *PkgGetNextUnit(struct NetPkg *NextUnit);
void PkgSetNextUnit(struct NetPkg *pkg,struct NetPkg *NextUnit);
u8* PkgGetCurrentBuffer(struct NetPkg *pkg);
u8* PkgGetBuffer(struct NetPkg *pkg);
void PkgSetBuffer(struct NetPkg *pkg, u8 *buf);
u16 PkgGetDataLen(struct NetPkg *pkg);
void PkgSetDataLen(struct NetPkg *pkg,u16 len);
u16 PkgGetOffset(struct NetPkg *pkg);
void PkgSetOffset(struct NetPkg *pkg,u16 offset);
ptu32_t PkgGetPrivate(struct NetPkg *pkg);
void PkgSetPrivate(struct NetPkg *pkg,ptu32_t PkgPrivate);
bool_t PkgIsBufferEnd(struct NetPkg *pkg);
void PkgMoveOffsetUp(struct NetPkg *pkg, u16 len);
void PkgMoveOffsetDown(struct NetPkg *pkg, u16 len);
u16 PkgListDatastatistics(struct NetPkg *pkg);
u16 PkgFrameDatastatistics(struct NetPkg *pkg);
u16 PkgListDataCopy(struct NetPkg *pkg,u8 *dst);
u16 PkgFrameDataCopy(struct NetPkg *pkg,u8 *dst);
void PkgCopyListToPkg(struct NetPkg *pkg,struct NetPkg *dst);
void PkgCopyFrameToPkg(struct NetPkg *pkg,struct NetPkg *dst);
bool_t PkgModuleInit(void);

struct in_addr;
struct in6_addr;
struct NetDev   *NetDev_Install(struct NetDevPara *para);
bool_t  NetDev_Uninstall(const char *name);
const u8 *NetDev_GetMac(struct NetDev *iface);
struct NetDev *NetDev_ForEachFromDefault(struct NetDev *Current);
const char *NetDev_GetName(struct NetDev *iface);
struct NetDev *NetDev_GetHandle(const char *ifname);
bool_t NetDev_Send(struct NetDev *iface,struct NetPkg *pkg,u32 devtask);
bool_t Link_NetDevPush(struct NetDev *iface,struct NetPkg *pkg);//if you get a package,you could call this function
//handle :the netdevice you install (returned by NetDev_Install)
//devname:if the netdevice is NULL,then we use the devname to search the device
//hook   :which used to deal the device message has been triggled
bool_t  NetDev_RegisterEventHook(struct NetDev *handle, fnNetDevEventHook hook);
//handle :the netdevice you install (returned by NetDev_Install)
//devname:if the netdevice is NULL,then we use the devname to search the device
//event  :the message want to send to the device
bool_t  NetDev_PostEvent(struct NetDev* handle,enum NetDevEvent event);
bool_t  NetDev_Ctrl(struct NetDev* handle,enum NetDevCmd cmd, ptu32_t para);
bool_t NetDev_FlowSet(struct NetDev* handle,enum EthFramType type,\
                     u32 llimit,u32 ulimit,u32 period,s32 enable);
enum EthFramType NetDev_FrameType(u8 *buf,u16 len);
bool_t NetDev_FlowCtrl(struct NetDev* handle,enum EthFramType type);
s32 NetDev_DHCP_SET_GotIP_CB(const char *ifname, s32 (*cb_ip_got)(u32 *ip));

void   *NetDev_GetPrivate(struct NetDev *iface);
//here declares the function we need from other modules,and the application also
//needs them
u16 NetDev_GetMtu(struct NetDev *DevFace);
u32 NetDev_GetFunc(struct NetDev *DevFace);
enum enLinkType NetDev_GetType(struct NetDev *DevFace);
struct LinkOps *NetDev_GetLinkOps(struct NetDev *DevFace);
void NetDev_SetDefault(struct NetDev *NetDev);
struct NetDev * NetDev_GetDefault(void);

//usage:is ver is EN_IPV_4, then the netaddr is tagHostAddrV4,else  tagHostAddrV6
bool_t NetDev_GetDefaultDns(enum_ipv_t ver,void *addr,void *addrbak);
bool_t NetDev_SetDefaultDns(enum_ipv_t ver,void *addr,void *addrbak);
bool_t NetDev_GetDns(enum_ipv_t ver,struct NetDev *NetDev,void *addr,void *addrbak);
bool_t NetDev_SetDns(enum_ipv_t ver,struct NetDev *NetDev,void *addr,void *addrbak);
bool_t NetDev_GetDefaultGateway(enum_ipv_t ver,void *addr);
bool_t NetDev_SetDefaultGateway(enum_ipv_t ver,void *addr);
bool_t NetDev_GetDefaultIP(enum_ipv_t ver,void *addr);
bool_t NetDev_GetGateway(enum_ipv_t ver,struct NetDev *NetDev,void *addr);
bool_t NetDev_SetGateway(enum_ipv_t ver,struct NetDev *NetDev,void *addr);
struct RoutItem4 *NetDev_GetIPv4RoutEntry(struct NetDev *NetDev);
struct RoutItem6 *NetDev_GetIPv6RoutEntry(struct NetDev *NetDev);
void NetDev_SetIPv4RoutEntry(struct NetDev *NetDev, struct RoutItem4 *new_root);
void NetDev_SetIPv6RoutEntry(struct NetDev *NetDev, struct RoutItem6 *new_root);
void NetDev_AddIPv4RoutItem(struct NetDev *NetDev, struct RoutItem4 *v4Item);
void NetDev_DelIPv4RoutItem(struct NetDev *NetDev, struct RoutItem4 *v4Item);
void NetDev_AddIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v6Item);
void NetDev_DelIPv6RoutItem(struct NetDev *NetDev, struct RoutItem6 *v4Item);
u32 NetDev_GetIPv4Table(struct NetDev *NetDev, struct in_addr *v4Item);
u32 NetDev_GetIPv6Table(struct NetDev *NetDev, struct in6_addr *v6Item);

////////////////////////defines for the link////////////////////////////////////////
//the following we be cut in the socket.h
//link function that driver should use
//bool_t LinkPost(void* iface,struct NetPkg *pkg);
//the hook function  module for the dev event
//handle:which device has triggled the event
//event :the message we has get
//the user could use the following api to listen on more protocol or send specified frames
typedef bool_t (*fnLinkProtoDealer)(struct NetDev *iface,struct NetPkg *pkg);
bool_t Link_RegisterRcvHook(fnLinkProtoDealer hook, const char *ifname,u16 proto,const char *hookname);
bool_t Link_UnRegisterRcvHook(const char *hookname);

void  *RouterCreate(tagRouterPara *para);
void   RouterRemoveByHandle(struct RoutItem4 *rout);
void   RouterRemove(tagRouterPara *para);


#endif /* TCPIP_NETINCLUDE_NETBSP_H_ */
