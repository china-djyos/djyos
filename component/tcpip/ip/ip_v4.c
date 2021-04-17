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
    u16 id;                 // identification��ÿ����һ��IP����1
    u16 fragment;           // Flags and Fragment Offset
    u8  ttl;                // time to live;Ĭ��Ϊ 64
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
// FUNCTION��This function is used to compute the pseudo header chksum
// PARA  IN��
// PARA OUT��
// RETURN  ��
// INSTRUCT: all the var is net endian
// =============================================================================
void IpPseudoPkgLstChkSumV4(u32 iplocal, u32 ipdst, u8  proto,struct NetPkg *pkg,u16 translen,u16 *chksum)
{
    int                             chklen;
    u16                             chksumtmp;
    void                            *chkbuf;
    tagPseudoHdr                    pseudohdr;   //����У���α����
    struct NetPkg                       *pkgtmp;

    pseudohdr.ipsrc = iplocal;
    pseudohdr.ipdst = ipdst;
    pseudohdr.zero = 0;
    pseudohdr.protocol = proto;
    pseudohdr.len = htons(translen);
    chksumtmp = IpChksumSoft16((void *)&pseudohdr, sizeof(pseudohdr),0, false);
    //У�����е�PKG
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
//        PkgSetNextUnit(result,NULL);      //PkgMalloc�Ѿ����ú�
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
    bool_t       ret = false;
    u32            devfunc;     //rout function
    struct NetPkg   *ippkg;       //ip header pkg
    bool_t         ipchksum;
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
            case EN_IPTYPE_V4_UNKOWN:
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
            devfunc = NetDev_GetFunc(rout.DevFace);
            if( (devtask & CN_IPDEV_TCPOCHKSUM) && !(devfunc & CN_IPDEV_TCPOCHKSUM) )
            {
                IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                devtask &= ~CN_IPDEV_TCPOCHKSUM;  //do it by the software
            }
            if( (devtask & CN_IPDEV_UDPOCHKSUM) && !(devfunc & CN_IPDEV_UDPOCHKSUM) )
            {
                IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                devtask &= ~CN_IPDEV_UDPOCHKSUM;  //do it by the software
            }
            if( (devtask & CN_IPDEV_ICMPOCHKSUM) && !(devfunc & CN_IPDEV_ICMPOCHKSUM) )
            {
                //chenws: ע��tcp,udp��Ҫαͷ��һ����㣬icmp����Ҫαͷ������
                //IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                IpPkgChkSum(pkg, chksum, 0);
                devtask &= ~CN_IPDEV_ICMPOCHKSUM;  //do it by the software
            }

            if(devfunc & CN_IPDEV_IPOCHKSUM)
            {
                devtask |= CN_IPDEV_IPOCHKSUM;  //������IPCHK���ܣ��������������
                ipchksum = false;               //make��ͷʱ�Ͳ��ù�IPCHK��
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
            ret = Link_Send(rout.DevFace,ippkg,devtask,EN_LINKPROTO_IPV4,EN_IPV_4,iphop,ipsrc);
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
// FUNCTION��This function is used to deal the ip mail,especially the ipv4
// PARA  IN��pkg,which load the message, maybe an list
//          devfunc, the netdev has done some task ,like the ip checksum or tcp checksum
// PARA OUT��
// RETURN  ��
// INSTRUCT:
// =============================================================================
static bool_t __rcvhost(struct NetPkg *pkg, u32 devfunc)
//bool_t IpV4Process(struct NetPkg *pkg,u32 devfunc)
{
    bool_t                      ret;
    u8                          proto;
    u32                         hdrlen;
    u32                         ipsrc;
    u32                         ipdst;
    tagV4PH                     *ph;
    u16                         fragment;
    u16                         framlen;
    tagIpAddr                   addr;

    ret = true;
    ph = (tagV4PH *)PkgGetCurrentBuffer(pkg);
    hdrlen = (ph->ver_len&0x0f)*4;
    if((0 ==(devfunc &CN_IPDEV_IPICHKSUM))&&\
       (0 != IpChksumSoft16(ph,hdrlen,0,true)))
    {
        TCPIP_DEBUG_INC(gV4CB.rcvdrop);
        return ret;
    }
    fragment = ph->fragment;
    fragment = ntohs(fragment);
    proto = ph->protocol;
    ipdst = ph->ipdst;
    ipsrc = ph->ipsrc;
    framlen = ph->len;
    framlen = ntohs(framlen);

    if(PkgGetDataLen(pkg) < framlen)
    {
        return ret;
    }
    if(framlen > hdrlen)
    {
        PkgMoveOffsetUp(pkg,sizeof(tagV4PH));
        PkgSetDataLen(pkg, framlen - hdrlen);   //ԭdatalen��������̫��֡��������
    }
    else
    {
        return ret;
    }
    if((fragment & CN_IP_DF_MSK)||\
        ((0== (fragment&CN_IP_MF_MSK))&&(0 == (fragment&CN_IP_OFFMASK))))
    {
        //OK, ���ϴ���IP���ݱ���
        addr.ver = EN_IPV_4;
        addr.src.ipv4.s_addr = ipsrc;
        addr.dst.ipv4.s_addr = ipdst;
        ret = IpProtoDeal(proto,&addr,pkg,devfunc);
    }
    else
    {
        //ִ��IP��Ƭ���鹦��
    }
    return ret;
}
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static bool_t __rcvsubnet(struct NetPkg *pkg, u32 devfunc)
{
    return false;
}
#pragma GCC diagnostic pop

//use this to deal with the ipv4 package
bool_t IpV4Process(struct NetPkg *pkg,u32 devfunc)
{
    bool_t                      ret = true;
    u32                         ipdst;
    tagV4PH                     *ph;

    TCPIP_DEBUG_INC(gV4CB.rcvnum);
    ph = (tagV4PH *)PkgGetCurrentBuffer(pkg);
    ipdst = ph->ipdst;

    tagRoutLink  rout;
    memset(&rout,0,sizeof(rout));
    rout.ver = EN_IPV_4;
    rout.DstIP = &ipdst;
    ret = RouterMatch(&rout);
    if(ret)
    {
        switch(rout.type)
        {
            case EN_IPTYPE_V4_HOST:     //Ŀ��ip���Ի�ip��ʵ�����Ѿ�������ipͳһ�ˡ�
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_LOCAL:    //Ŀ��ip�Ǳ���ip
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_BROAD:    //Ŀ��ip�ǹ㲥ip
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_MULTI:    //Ŀ��ip�Ƕಥip
                ret =__rcvhost(pkg,devfunc);
                break;
            case EN_IPTYPE_V4_SUBNET:  //Ŀ��ip�뱾��ip����ͬһ������
                ret =__rcvsubnet(pkg,devfunc);
                break;
            default:
                break;
        }
    }
    return ret;
}

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
