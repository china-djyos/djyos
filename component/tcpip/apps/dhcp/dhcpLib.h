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


#include <sys/socket.h>

#define CN_DHCP_CLIENT_HOST       "DJYOS"
#define CN_DHCP_CLIENT_VENDOR     "DHCP_CV1.0"
#define CN_DHCP_SERVER_HOST       "DJYOS_SERVER"
#define CN_DHCP_SERVER_VENDOR     "DHCP_SV1.0"

#define CN_DHCP_FLAG_UNICAST      0X0000
#define CN_DHCP_FLAG_BROAD        0X8000
#define CN_DHCP_MAGICCOOKIE       "\x63\x82\x53\x63"
#define ETH_ALEN                  CN_MACADDR_LEN
#define ARPHRD_ETHER              1


/* UDP port numbers for DHCP */
#define DHCP_SERVER_PORT    67  /* from client to server */
#define DHCP_CLIENT_PORT    68  /* from server to client */

/* DHCP message OP code */
#define DHCP_BOOTREQUEST    1
#define DHCP_BOOTREPLY      2

/* DHCP 报文类型 */
//DHCP客户端在请求IP地址时并不知道DHCP服务器的位置，因此DHCP客户端会在本地网络内以
//广播方式发送Discover请求报文，以发现网络中的DHCP服务器。所有收到Discover报文的
//DHCP服务器都会发送应答报文，DHCP客户端据此可以知道网络中存在的DHCP服务器的位置。
#define DHCP_DISCOVER       1   //客户端发送广播包在网络上寻找 DHCP 服务器
//DHCP服务器收到Discover报文后，就会在所配置的地址池中查找一个合适的IP地址，加上相应
//的租约期限和其他配置信息（如网关、DNS服务器等），构造一个Offer报文，发送给DHCP客户端，
//告知用户本服务器可以为其提供IP地址。但这个报文只是告诉DHCP客户端可以提供IP地址，
//最终还需要客户端通过ARP来检测该IP地址是否重复。
#define DHCP_OFFER          2   //服务器发送，单播，含 IP、MAC地址以及地址租期
//DHCP客户端可能会收到很多Offer请求报文，所以必须在这些应答中选择一个。通常是选择
//第一个Offer应答报文的服务器作为自己的目标服务器，并向该服务器发送一个广播的Request
//请求报文，通告选择的服务器，希望获得所分配的IP地址。另外，DHCP客户端在成功获取IP
//地址后，在地址使用租期达到50%时，会向DHCP服务器发送单播Request请求报文请求续延租约，
//如果没有收到ACK报文，在租期达到87.5%时，会再次发送广播的Request请求报文以请求续延租约。
#define DHCP_REQUEST        3   //客户端发送，广播包，功能：1、请求配置信息；2、确认信息是否仍然有效；3、延长租期
//DHCP客户端收到DHCP服务器ACK应答报文后，通过地址冲突检测发现服务器分配的地址冲突或者
//由于其他原因导致不能使用，则会向DHCP服务器发送Decline请求报文，通知服务器所分配的
//IP地址不可用，以期获得新的IP地址。
#define DHCP_DECLINE        4   //客户端发送，告知服务器此地址已被使用
//DHCP服务器收到Request请求报文后，根据Request报文中携带的用户MAC来查找有没有相应的
//租约记录，如果有则发送ACK应答报文，通知用户可以使用分配的IP地址。
#define DHCP_ACK            5   //服务器发送 DHCP ACK 单播包，确认主机的请求
//如果DHCP服务器收到Request请求报文后，没有发现有相应的租约记录或者由于某些原因无法
//正常分配IP地址，则向DHCP客户端发送NAK应答报文，通知用户无法分配合适的IP地址。
#define DHCP_NAK            6   //服务器发送，告知地址无效或租期已满
//当DHCP客户端不再需要使用分配IP地址时（一般出现在客户端关机、下线等状况）就会主动向
//DHCP服务器发送RELEASE请求报文，告知服务器用户不再需要分配IP地址，请求DHCP服务器释放
//对应的IP地址。
#define DHCP_RELEASE        7   //客户端发送，告知服务器地址不再使用
//DHCP客户端如果需要从DHCP服务器端获取更为详细的配置信息，则向DHCP服务器发送Inform
//请求报文；DHCP服务器在收到该报文后，将根据租约进行查找到相应的配置信息后，向DHCP
//客户端发送ACK应答报文。目前基本上不用了。
#define DHCP_INFORM         8   //客户端发送，请求配置参数，IP地址已经有了。

/* DHCP option and value (cf. RFC1533) */
enum
{
    padOption                 =   0,
    subnetMask                =   1,
    timerOffset               =   2,
    routersOnSubnet           =   3,
    timeServer                =   4,
    nameServer                =   5,
    dns                       =   6,
    logServer                 =   7,
    cookieServer              =   8,
    lprServer                 =   9,
    impressServer             =   10,
    resourceLocationServer    =   11,
    hostName                  =   12,
    bootFileSize              =   13,
    meritDumpFile             =   14,
    domainName                =   15,
    swapServer                =   16,
    rootPath                  =   17,
    extentionsPath            =   18,
    IPforwarding              =   19,
    nonLocalSourceRouting     =   20,
    policyFilter              =   21,
    maxDgramReasmSize         =   22,
    defaultIPTTL              =   23,
    pathMTUagingTimeout       =   24,
    pathMTUplateauTable       =   25,
    ifMTU                     =   26,
    allSubnetsLocal           =   27,
    broadcastAddr             =   28,
    performMaskDiscovery      =   29,
    maskSupplier              =   30,
    performRouterDiscovery    =   31,
    routerSolicitationAddr    =   32,
    staticRoute               =   33,
    trailerEncapsulation      =   34,
    arpCacheTimeout           =   35,
    ethernetEncapsulation     =   36,
    tcpDefaultTTL             =   37,
    tcpKeepaliveInterval      =   38,
    tcpKeepaliveGarbage       =   39,
    nisDomainName             =   40,
    nisServers                =   41,
    ntpServers                =   42,
    vendorSpecificInfo        =   43,
    netBIOSnameServer         =   44,
    netBIOSdgramDistServer    =   45,
    netBIOSnodeType           =   46,
    netBIOSscope              =   47,
    xFontServer               =   48,
    xDisplayManager           =   49,
    dhcpRequestedIPaddr       =   50,
    dhcpIPaddrLeaseTime       =   51,
    dhcpOptionOverload        =   52,
    dhcpMessageType           =   53,
    dhcpServerIdentifier      =   54,
    dhcpParamRequest          =   55,
    dhcpMsg                   =   56,
    dhcpMaxMsgSize            =   57,
    dhcpRenewTime             =   58,
    dhcpRebindTime            =   59,
    dhcpClassIdentifier       =   60,
    dhcpClientIdentifier      =   61,
    endOption                 =   255
};

#pragma pack(1)
typedef struct DhcpMessage
{
  u8  op;       /* operation type */
  u8  htype;    /* hardware address type */
  u8  hlen;     /* hardware address length */
  u8  hops;     /* should be zero in client's message */
  u32 xid;      /* transaction id */
  u16 secs;     /* elapsed time in sec. from trying to boot */
  u16 flags;
  u32 ciaddr;   /* (previously allocated) client IP address */
  u32 yiaddr;   /* 'your' client IP address */
  u32 siaddr;   /* should be zero in client's messages */
  u32 giaddr;   /* should be zero in client's messages */
  u8  chaddr[16];   /* client's hardware address */
  u8  sname[64];    /* server host name, null terminated string */
  u8  file[128];    /* boot file name, null terminated string */
  u8  options[312]; /* message options */
}tagDhcpMsg;

typedef struct
{
    u8 type;
    u8 len;
    u8 data[0];
}tagDhcpOpt;

#pragma pack()

#define CN_MSGMAX_LEN    (sizeof(tagDhcpMsg))
#define CN_DHCP_STRLEN   16

//used for the discover and request message
typedef struct
{
    u8   optype;
    u8   msgtype;
    u32  reqip;
    u32  dhcpserver;
    u8   clientmac[CN_MACADDR_LEN];
    u32  transaction;
}tagDhcpRequestPara;
//used for the offer and ack message
typedef struct
{
    u8   optype;
    u8   msgtype;
    u32  transaction;
    u32  offerip;
    u32  dhcpservernxt;
    u32  relayagent;
    u8   clientmac[CN_MACADDR_LEN];
    u32  renewtime;
    u32  bindtime;
    u32  leasetime;
    u32  dhcpserver;
    u32  ipmask;
    u32  router;
    char domainname[CN_DHCP_STRLEN];
    u32  dns1;
    u32  dns2;
}tagDhcpReplyPara;


void DHCP_MakeDhcpRequestMsg(tagDhcpMsg *msg,tagDhcpRequestPara *para);
void DHCP_ShowDhcpRequestMsg(tagDhcpRequestPara *para);
void DHCP_ShowDhcpReplyMsg(tagDhcpReplyPara *para);
bool_t DHCP_PasteDhcpReplyMsg(tagDhcpReplyPara *para,tagDhcpMsg *msg);
void DHCP_MakeDhcpReplyMsg(tagDhcpMsg *msg,tagDhcpReplyPara *para);
bool_t DHCP_PasteDhcpRequestMsg(tagDhcpRequestPara *para,tagDhcpMsg *msg);



