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
//date  :下午4:54:56/2016年12月29日
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
//add your own specified header here
#include <sys/socket.h>
#include <netbsp.h>
#include <netdb.h>
#include "../common/link.h"
#include "../common/netdev.h"
#include "../common/netpkg.h"
#include "arp.h"


//data flow in the loop
//ipout->linkout->linkoutloop->loopout->loopin->LinkIn->linkInLoop->link2ip
//the internet proto we supported now
#pragma pack(1)
typedef struct EthernetHdr
{
    u8 macdst[CN_MACADDR_LEN];
    u8 macsrc[CN_MACADDR_LEN];
    u16 type_or_tpid;           //如果是vlan帧，则=0x8100，否则，代表协议类型
    u16 vlantci;                //如果是vlan帧，则是vlan的tci域
    u16 type_if_vlan;           //如果是vlan帧，协议类型移到这里

}tagEthernetHdr;
#pragma pack()
#define CN_ETHERNET_HEADLEN_UNVLAN  14      //非vlan帧的帧头长度
#define CN_ETHERNET_HEADLEN_VLAN    18      //vlan帧的帧头长度
//-----------------------------------------------------------------------------
//功能: 链路层发送函数，准备好Ethernet的包头，如果不知道对方Mac地址，则调用ARP功能获取，
//      然后调用网卡设备端额ifsend函数发送包链
//参数: iface，网卡设备指针
//      pkg，须发送的包链
//      framlen，须发送的报文总长度，包括包链中的所有节点
//      devtask，网卡硬件加速功能需要完成的工作，例如填充tcp或IP头的校验和
//      proto，协议类型，0800/0806/0835等
//      ver，IP版本号。TODO：此处值得商榷，网卡难道只能发送IP包吗？
//      ipdst，目的IP地址
//      ipsrc，源IP地址
//返回:
//备注:
//作者:zhangqf@下午8:55:19/2016年12月28日
//-----------------------------------------------------------------------------
static bool_t __Link_Out(struct NetDev *iface,struct NetPkg *pkg,u32 devtask,\
                           u16 proto,enum_ipv_t ver,ipaddr_t ipdst,ipaddr_t ipsrc)
{
    bool_t            ret;
    tagEthernetHdr   *hdr;
    struct NetPkg    *ethpkg;

    ret = false;
    ethpkg = PkgMalloc(CN_ETHERNET_HEADLEN_UNVLAN,0);
    if(NULL != ethpkg)
    {
        hdr = (tagEthernetHdr *)PkgGetCurrentBuffer(ethpkg);
//      hdr = (tagEthernetHdr  *)(ethpkg->buf + ethpkg->offset);
        memcpy(hdr->macsrc, NetDev_GetMac(iface), CN_MACADDR_LEN);
        hdr->type_or_tpid = htons(proto);
        if(ver == EN_IPV_4)
        {
            //we should just do it here
            if(devtask&CN_IPDEV_IPBROAD) //do broad here
            {
                memcpy(hdr->macdst,CN_MAC_BROAD,CN_MACADDR_LEN);
                ret = true;
            }
            else if(devtask&CN_IPDEV_IPMULTI) //do multi
            {
                ret = true; //not implement yet
            }
            else
            {
                //we must find the mac address use the arp processor
                ret = ResolveMacByArp(ipdst,ipsrc,iface,hdr->macdst);  //the ipv4 support
            }
            if(false == ret)//if false,we use the broadcast mac address
            {
                memcpy(hdr->macdst,CN_MAC_BROAD,CN_MACADDR_LEN);
            }
            PkgSetNextUnit(ethpkg,pkg);
            PkgSetDataLen(ethpkg, CN_ETHERNET_HEADLEN_UNVLAN);
//          ethpkg->datalen = CN_ETHERNET_HEADLEN;
//          ethpkg->partnext = pkg;
//          framlen += CN_ETHERNET_HEADLEN_UNVLAN;
//          ret = NetDev_Send(iface,ethpkg,framlen,devtask);
            NetDev_PkgsndInc(iface);
            ret = NetDev_Send(iface,ethpkg,devtask);
            if(ret == false)
            {
                NetDev_PkgsndErrInc(iface);
            }
            PkgTryFreePart(ethpkg);
        }
        else
        {
            //not support the ipv6 yet
        }

    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 由网卡推送函数调用，处理链路层部分，取出上层协议，并调用LinkPush把数据推送到更高
//      层协议处理。
//参数: iface，数据源网卡设备指针
//      pkg，推送上来的数据包
//返回: true or false
//-----------------------------------------------------------------------------
static bool_t  __LinkIn(struct NetDev *iface,struct NetPkg *pkg)
{
    bool_t          ret=false;
    u8              *buf;
    tagEthernetHdr *hdr;
    u16             proto;
    if(PkgGetDataLen(pkg) > CN_ETHERNET_HEADLEN_UNVLAN)
//  if(pkg->datalen > CN_ETHERNET_HEADLEN)
    {
        //we analyze the ethernet header first, which type it has
        hdr = (tagEthernetHdr *)PkgGetCurrentBuffer(pkg);
//      hdr = (tagEthernetHdr *)(pkg->buf + pkg->offset);
//      memcpy(&proto,&hdr->type,sizeof(proto));
        buf = (u8*)&hdr->type_or_tpid;
        proto = (((u16)*buf)<<8) + *(buf+1);
        if(proto==0x8100)           //带vlan标签的帧，有些交换机会把vlan帧发过来
        {
            buf += 4;               //vlan 域 4 字节
            proto = (((u16)*buf)<<8) + *(buf+1);        //取出真正的协议字段
            PkgMoveOffsetUp(pkg,CN_ETHERNET_HEADLEN_VLAN);
        }
        else
            PkgMoveOffsetUp(pkg,CN_ETHERNET_HEADLEN_UNVLAN);

//      pkg->offset += CN_ETHERNET_HEADLEN;
//      pkg->datalen -= CN_ETHERNET_HEADLEN;
        ret = Link_Push(iface,pkg,(enum enLinkProto)proto);
    }
    return ret;
}
//-----------------------------------------------------------------------------
//功能:we use this function to register the ethernet type to the link
//参数:
//返回:
//备注:
//作者:zhangqf@上午9:26:04/2016年12月29日
//-----------------------------------------------------------------------------
bool_t LinkEthernetInit(void)
{
    bool_t ret = true;
    //first we will register a loop link type to the link hal
    struct LinkOps   ops;
    memset(&(ops),0,sizeof(ops));
    ops.linkin = __LinkIn;
    ops.linkout =__Link_Out;
    ret = Link_Register(EN_LINK_ETHERNET,"ETHERNET",&ops);

    return ret;
}

