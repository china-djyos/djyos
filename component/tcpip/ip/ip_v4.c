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
#include "dbug.h"
#include <shell.h>
#include "../component_config_tcpip.h"

#include "../common/ip.h"
#include "../common/ipV4.h"
#include "../common/router.h"
#include "../common/netdev.h"
#include "../common/link.h"
#include "../common/netpkg.h"


#define CN_IP_DF_MSK        0x4000      // dont fragment flag
#define CN_IP_MF_MSK        0x2000      // more fragments flag
#define CN_IP_OFFMASK       0x1FFF      // mask for fragmenting bits
#define CN_IP_TTL_DEFALT    0x40

#pragma pack(1)
typedef struct
{
    u8  ver_len;            // the ver is high 4bits, and the headlen is the low 4 bits
    u8  tos;                // type of service
    u16 len;                // total length
    u16 id;                 // identification，每发送一个IP包加1
    u16 fragment;           // Flags and Fragment Offset
    u8  ttl;                // time to live;默认为 64
    u8  protocol;           // protocol
    u16 chksum;             // IP header Checksum
    u32 ipsrc;              // source IP address
    u32 ipdst;              // destination IP address
    u8  option[0];          // IP optional
}tagV4PH;
#pragma pack()

typedef struct
{
     u32 sndnum;
     u32 snderrnum;
     u32 sndnoroutnum;
     u32 sndwan;
     u32 sndbroad;
     u32 sndsubnet;
     u32 rcvnum;
     u32 rcvhost;
     u32 rcvforward;
     u32 rcvnorout;
     u32 rcvdrop;
     u32 rcvfragment;   //which means the frame is fragment by more than one
}tagV4CB;
static tagV4CB gV4CB;

// =============================================================================
// FUNCTION：This function is used to compute the pseudo header chksum
// PARA  IN：
// PARA OUT：
// RETURN  ：
// INSTRUCT: all the var is net endian
// =============================================================================
void IpPseudoPkgLstChkSumV4(u32 iplocal, u32 ipdst, u8  proto,struct NetPkg *pkg,u16 translen,u16 *chksum)
{
    int                             chklen;
    u16                             chksumtmp;
    void                            *chkbuf;
    tagPseudoHdr                    pseudohdr;   //用于校验的伪部首
    struct NetPkg                       *pkgtmp;

    pseudohdr.ipsrc = iplocal;
    pseudohdr.ipdst = ipdst;
    pseudohdr.zero = 0;
    pseudohdr.protocol = proto;
    pseudohdr.len = htons(translen);
    chksumtmp = IpChksumSoft16((void *)&pseudohdr, sizeof(pseudohdr),0, false);
    //校验所有的PKG
    pkgtmp = pkg;
    do{
        chklen = PkgGetDataLen(pkgtmp);
//      chklen = pkgtmp->datalen;
        chkbuf = (u8 *)PkgGetCurrentBuffer(pkgtmp);
//      chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
        if((NULL == PkgGetNextUnit(pkgtmp))||PkgIsBufferEnd(pkgtmp))
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, \
                                chklen,chksumtmp, true);
            *chksum =(u16)chksumtmp;
            pkgtmp = NULL;
        }
        else
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf,  chklen,chksumtmp, false);
            pkgtmp = PkgGetNextUnit(pkgtmp);
        }
    }while(NULL != pkgtmp);
}

// =============================================================================
// FUNCTION:This function is used to compute the pseudo header chksum
// PARA  IN:ipsrc,the host ip
//          ipdst,the dst ip
//          pkg, the translation layer frame
//          translen, the pkg lst len(may be only one)
//          proto, trans layer proto
//          devtask, the dev should do some work, like chksum
//          chksum, where to storage the chksum
// PARA OUT:
// RETURN  :
// INSTRUCT:all the var is net endian
// =============================================================================
static u16 Ipv4Flg = 0;
#define CN_IPV4_HDR_DEFAULTLEN      5

//this function used to make an ipv4 head pkg
static struct NetPkg *__PHMake(u32 ipsrc,u32 ipdst,u8 proto,u32 translen,bool_t chksum)
{
    struct NetPkg    *result;
    tagV4PH   *hdr;         //ip header

    result = PkgMalloc(sizeof(tagV4PH),0);
    if(NULL != result)
    {
        hdr = (tagV4PH *)PkgGetCurrentBuffer(result);
//      hdr = (tagV4PH *)(result->buf + result->offset);
        hdr->ver_len = (EN_IPV_4<<4)|CN_IPV4_HDR_DEFAULTLEN;
        hdr->tos = 0;
        hdr->len = htons(translen + CN_IPV4_HDR_DEFAULTLEN*4);
        hdr->id = htons(Ipv4Flg);
        Ipv4Flg++;
        hdr->fragment = htons(CN_IP_DF_MSK);
        hdr->ttl = 64;
        hdr->protocol = proto;
        hdr->chksum = 0;
        hdr->ipsrc = ipsrc;
        hdr->ipdst = ipdst;
        PkgSetNextUnit(result,NULL);
        PkgSetDataLen(result, sizeof(tagV4PH));
//      result->datalen = sizeof(tagV4PH);
//      result->partnext = NULL;

        //check the ip head
        if(chksum)
        {
            IpPkgChkSum(result,&hdr->chksum,0);
        }
    }
    return result;
}


bool_t IpV4Send(u32 ipsrc, u32 ipdst, struct NetPkg *pkg,u16 translen,u8 proto,\
                u32 devtask, u16 *chksum)
{
    bool_t                          ret = false;
    u32                             devfunc;     //rout function
    struct NetPkg                      *ippkg;       //ip header pkg
    bool_t                          ipchksum;
    u32                             framlen;
    u32 iphop = INADDR_ANY;
    u32 iphost = INADDR_ANY;
    tagRoutLink  rout;

    if(ipsrc != 0)
        ret = false;
    TCPIP_DEBUG_INC(gV4CB.sndnum);
    memset(&rout,0,sizeof(rout));
    rout.ver = EN_IPV_4;
    rout.DstIP = &ipdst;
    rout.HostIP = &iphost;
    rout.HopIP = &iphop;
    ret = RouterMatch(&rout);
    if(ret)
    {
        switch(rout.type)
        {
            case EN_IPTYPE_V4_HOST:
                devtask |= CN_IPDEV_IPUNI;
                break;
            case EN_IPTYPE_V4_LOCAL:
                devtask |= CN_IPDEV_IPUNI;
                break;
            case EN_IPTYPE_V4_SUBNET:
                devtask |= CN_IPDEV_IPUNI;
                break;
            case EN_IPTYPE_V4_BROAD:
                devtask |= CN_IPDEV_IPBROAD;
                break;
            case EN_IPTYPE_V4_MULTI:
                devtask |= CN_IPDEV_IPMULTI;
                break;
            default:
                ret = false;
                break;
        }
        if(ret)
        {
            if(ipsrc == INADDR_ANY)
            {
                ipsrc = iphost;
            }
            if(iphop == INADDR_ANY) //if not,which means need send to the hop
            {
                iphop = ipdst;
            }
            devfunc = NetDevFunc(rout.DevFace);
            if((0 == (devfunc&devtask))&&(CN_IPDEV_NONE!=devtask))
            {
                switch (devtask & (CN_IPDEV_TCPOCHKSUM | CN_IPDEV_UDPOCHKSUM \
                        | CN_IPDEV_ICMPOCHKSUM))
                {
                    case  CN_IPDEV_TCPOCHKSUM:
                        IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                        devtask = CN_IPDEV_NONE;  //do it by the software
                        break;
                    case  CN_IPDEV_UDPOCHKSUM:
                        IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                        devtask = CN_IPDEV_NONE; //do it by the software
                        break;
                    case  CN_IPDEV_ICMPOCHKSUM:
                        IpPkgLstChkSum(pkg,chksum,0);
                        devtask = CN_IPDEV_NONE; //do it by the software
                        break;
                    default: //not supported yet,do nothing here
                        break;
                }
            }
            if(devfunc & CN_IPDEV_IPOCHKSUM)
            {
                devtask |= CN_IPDEV_IPOCHKSUM;
                ipchksum = false;
            }
            else
            {
                ipchksum = true;
            }
            //check if the rout has so much mtu,if not do the fragment here //--TODO
            ippkg = __PHMake(ipsrc,ipdst,proto,translen,ipchksum);
            PkgSetNextUnit(ippkg,pkg);
//          ippkg->partnext = pkg;
//          framlen = PkgGetDataLen(ippkg) + translen;
//          framlen = ippkg->datalen + translen;
            ret = LinkSend(rout.DevFace,ippkg,devtask,EN_LINKPROTO_IPV4,EN_IPV_4,iphop,ipsrc);
            PkgTryFreePart(ippkg);
        }
    }
    if(ret == false) //may be send failed here for the mac address or the interface reason
    {
        TCPIP_DEBUG_INC(gV4CB.snderrnum);
    }
    return ret;
}

// =============================================================================
// FUNCTION：This function is used to deal the ip mail,especially the ipv4
// PARA  IN：pkg,which load the message, maybe an list
//          devfunc, the netdev has done some task ,like the ip checksum or tcp checksum
// PARA OUT：
// RETURN  ：
// INSTRUCT:
// =============================================================================
static bool_t __rcvhost(struct NetPkg *pkg, u32 devfunc)
{
    bool_t                      ret;
    u8                          proto;
    u32                         hdrlen;
    u32                         ipsrc;
    u32                         ipdst;
    tagV4PH                     *ph;
//  u32                         devfunc;
    u16                         fragment;
    u16                         framlen;
    tagIpAddr                   addr;

    ret = true;
    ph = (tagV4PH *)PkgGetCurrentBuffer(pkg);
//  ph = (tagV4PH *)(pkg->buf + pkg->offset);
    hdrlen = (ph->ver_len&0x0f)*4;
//  devfunc = NetDevFunc(iface);
    if((0 ==(devfunc &CN_IPDEV_IPICHKSUM))&&\
       (0 != IpChksumSoft16(ph,hdrlen,0,true)))
    {
        TCPIP_DEBUG_INC(gV4CB.rcvdrop);
        return ret;
    }
//  memcpy(&fragment,&ph->fragment,sizeof(fragment));
    fragment = ph->fragment;
    fragment = ntohs(fragment);
//  memcpy(&id,&ph->id,sizeof(id));
    proto = ph->protocol;
//  memcpy(&ipdst,&ph->ipdst,sizeof(ipdst));
//  memcpy(&ipsrc,&ph->ipsrc,sizeof(ipsrc));
    ipdst = ph->ipdst;
//  ipdst = ntohl(ipdst);
    ipsrc = ph->ipsrc;
//  ipsrc = ntohl(ipsrc);
//  memcpy(&framlen,&ph->len,sizeof(framlen));
    framlen = ph->len;
    framlen = ntohs(framlen);

//  pkg->offset += hdrlen;
    if(PkgGetDataLen(pkg) < framlen)
//  if(pkg->datalen < framlen)
    {
        return ret;
    }
    if(framlen > hdrlen)
    {
        PkgMoveOffsetUp(pkg,sizeof(tagV4PH));
        PkgSetDataLen(pkg, framlen - hdrlen);   //原datalen包含了以太网帧填充的数据
//      PkgSetDataLen(pkg, framlen - hdrlen);
//      pkg->datalen = framlen - hdrlen;
    }
    else
    {
        return ret;
    }
    if((fragment & CN_IP_DF_MSK)||\
        ((0== (fragment&CN_IP_MF_MSK))&&(0 == (fragment&CN_IP_OFFMASK))))
    {
        //OK, 向上传送IP数据报文
        addr.ver = EN_IPV_4;
        addr.src.ipv4.s_addr = ipsrc;
        addr.dst.ipv4.s_addr = ipdst;
        ret = IpProtoDeal(proto,&addr,pkg,devfunc);
    }
    else
    {
        //执行IP分片重组功能
    }
    return ret;
}


//use this to deal with the ipv4 package
bool_t IpV4Process(struct NetPkg *pkg,u32 devfunc)
{
    bool_t                      ret = true;
    u32                         ipdst;
//    u32                         ipsrc;
    tagV4PH                     *ph;

    TCPIP_DEBUG_INC(gV4CB.rcvnum);
    ph = (tagV4PH *)PkgGetCurrentBuffer(pkg);
//  ph = (tagV4PH *)(pkg->buf + pkg->offset);
    ipdst = ph->ipdst;
//    ipdst = ntohl(ipdst);
//    ipsrc = ph->ipsrc;
//    ipsrc = ntohl(ipsrc);
//  memcpy(&ipdst,&ph->ipdst,sizeof(ipdst));
//  memcpy(&ipsrc,&ph->ipsrc,sizeof(ipsrc));

    tagRoutLink  rout;
    memset(&rout,0,sizeof(rout));
    rout.ver = EN_IPV_4;
    rout.DstIP = &ipdst;
    ret = RouterMatch(&rout);
    if(ret)
    {
        switch(rout.type)
        {
            case EN_IPTYPE_V4_HOST:
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_LOCAL:
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_BROAD:
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_MULTI:  //if permit the multi broad here
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_SUBNET: //if could do the forward,we will do this here--TODO,zhangqf
                break;
            default:
                break;
        }
    }
    return ret;
}
//static bool_t _V4Show(char *param)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t ipv4(char *param)
{
    debug_printf("IPV4","IPV4 STATISTICS:\n\r");
    debug_printf("IPV4","IPV4 STATISTICS:SND         :%d\n\r",gV4CB.sndnum);
    debug_printf("IPV4","IPV4 STATISTICS:SND  FAILED :%d\n\r",gV4CB.snderrnum);
    debug_printf("IPV4","IPV4 STATISTICS:SND  NOROUT :%d\n\r",gV4CB.sndnoroutnum);
    debug_printf("IPV4","IPV4 STATISTICS:SND  BROAD  :%d\n\r",gV4CB.sndbroad);
    debug_printf("IPV4","IPV4 STATISTICS:SND  WAN    :%d\n\r",gV4CB.sndwan);
    debug_printf("IPV4","IPV4 STATISTICS:SND  SUBNET :%d\n\r",gV4CB.sndsubnet);
    debug_printf("IPV4","IPV4 STATISTICS:RCV         :%d\n\r",gV4CB.rcvnum);
    debug_printf("IPV4","IPV4 STATISTICS:RCV  HOST   :%d\n\r",gV4CB.rcvhost);
    debug_printf("IPV4","IPV4 STATISTICS:RCV  DROPS  :%d\n\r",gV4CB.rcvdrop);
    debug_printf("IPV4","IPV4 STATISTICS:RCV  FORWARD:%d\n\r",gV4CB.rcvforward);
    debug_printf("IPV4","IPV4 STATISTICS:RCV  FRAG   :%d\n\r",gV4CB.rcvfragment);
    return true;
}

ADD_TO_ROUTINE_SHELL(ipv4,ipv4,"usage:ipv4");

#pragma GCC diagnostic pop
