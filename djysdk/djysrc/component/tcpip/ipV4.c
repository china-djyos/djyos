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
#include <sys/socket.h>

#include "ip.h"
#include "ipV4.h"
#include "rout.h"
#include "linkhal.h"
#include "tcpipconfig.h"

#define CN_IP_DF_MSK        0x4000      // dont fragment flag
#define CN_IP_MF_MSK        0x2000      // more fragments flag
#define CN_IP_OFFMASK       0x1FFF      // mask for fragmenting bits
#define CN_IP_TTL_DEFALT    0x40

#pragma pack(1)
typedef struct IpHdrV4
{
    u8  ver_len;            // the ver is high 4bits, and the headlen is the low 4 bits
    u8  tos;                // type of service
    u16 len;                // total length
    u16 id;                 // identification
    u16 fragment;           // Flags and Fragment Offset
    u8  ttl;                // time to live;
    u8  protocol;           // protocol
    u16 chksum;             // IP header Checksum
    u32 ipsrc;              // source IP address
    u32 ipdst;              // destination IP address
    u8  option[0];          // IP optional
}tagIpHdrV4;
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
     u32 rcvdrop;
     u32 rcvfragment;   //which means the frame is fragment by more than one
}tagIpv4Statistics;
static tagIpv4Statistics gIpv4Statistics;


#define CN_ASSEMBLE_FLAG_START     (1<<0)  //the first comes
#define CN_ASSEMBLE_FLAG_LAST      (1<<1)  //the last comes
#define CN_ASSEMBLE_LIMIT          (31)
//this data struct is used to deal the recombination package,not implement yet
typedef struct AssembleNode
{
	struct AssembleNode   *nxt;
	tagNetPkg             *pkgq;    //this is the assemble queue
	u32                    ipsrc;   //this is the ip source
	u32                    ipdst;   //this is the ip destination
	tagNetDev             *dev;    //which is the dev func
	u16                    datalen; //the fram len
	u16                    id;      //this is the ip frame id
	u8                     lastbit; //which means it is the last one
	u8                     proto;   //this is the ip internal proto
	u8                     qlen;    //how many pkg in the queue
	u32                    bitmap;  //which means the offset bit map,only support 32 bit
	s64                    timeout; //which is the timeout member,when timeout, it will be delete
}tagAssembleNode;
static tagAssembleNode *pAssembleQueue = NULL;  //use for the cached queue
static struct MutexLCB *pAssembleSync = NULL;   //use for the cached queue sync
static struct MutexLCB  gAssembleSyncMem;
//void show ipassemble
static bool_t __showAssemble(char *param)
{
	u32 i = 1;
	tagAssembleNode *node;
	printf("Ipv4 Assemble:%s\n\r",gEnableIpAssemble?"enable":"disable");
	if(Lock_MutexPend(pAssembleSync,CN_TIMEOUT_FOREVER))
	{
		node = pAssembleQueue;
		while(NULL != node)
		{
			printf("Assemble:%d\n\r",i);
			printf("        :ipsrc:0x%08x",node->ipsrc);
			printf("        :ipdst:0x%08x",node->ipdst);
			printf("        :dlen :0x%04x",node->datalen);
			printf("        :id   :0x%04x",node->id);
			printf("        :lb   :0x%02x",node->lastbit);
			printf("        :proto:0x%08x",node->proto);
			printf("        :qlen :0x%02x",node->qlen);
			printf("        :bimap:0x%08x",node->bitmap);
			printf("        :deadl:%lld",node->timeout);
			node = node->nxt;
		}
		Lock_MutexPost(pAssembleSync);
	}
	return true;
}
#define CN_ASSEMBLE_LIFE          (200*mS)    //assemble time == 200ms
//we use this function to check out the timeout
static void __assembletimeoutcheck(void)
{
	tagAssembleNode *tmp,*pre;
	s64   timenow;

	tmp = pAssembleQueue;
	pre = tmp;
	timenow = DjyGetSysTime();
	while(NULL != tmp)
	{
		if(tmp->timeout <= timenow)//we should delete the node
		{
			if(tmp == pAssembleQueue)
			{
				pAssembleQueue = tmp->nxt;
				pre = pAssembleQueue;

			}
			else
			{
				pre->nxt = tmp->nxt;
			}
			PkgTryFreeQ(tmp->pkgq);
			free((void *)tmp);
			if(NULL != pre)
			{
				tmp = pre->nxt;
			}
			else
			{
				tmp = NULL;
			}
		}
		else
		{
			//do the next
			pre = tmp;
			tmp = tmp->nxt;
		}
	}
}
//we use this function to do an assemble here
static bool_t assemblepkg(u32 ipsrc,u32 ipdst,u16 id,u16 fragment,u8 proto,tagNetDev *dev,tagNetPkg *pkg)
{
	tagAssembleNode  *assemble,*pre;
	u16        offset;
	tagNetPkg *pkgassemble,*pkgtmp;
	u8        *src,*dst;
	offset = fragment&CN_IP_OFFMASK;
	tagIpAddr  addr;
	if(offset > CN_ASSEMBLE_LIMIT )
	{
		return false;
	}
	//find an assemble node here
	if(Lock_MutexPend(pAssembleSync,CN_TIMEOUT_FOREVER))
	{
		assemble = pAssembleQueue;
		pre = assemble;
		while(NULL != assemble)
		{
			if((ipsrc == assemble->ipsrc)&&\
			   (ipdst == assemble->ipdst)&&
			   (id == assemble->id)&&\
			   (proto == assemble->proto))
			{
				break;
			}
			else
			{
				pre = assemble;
				assemble = assemble->nxt;
			}
		}
		if(NULL == assemble)
		{
			//find no one, so build a new one
			assemble = malloc(sizeof(tagAssembleNode));
			if(NULL != assemble)
			{
				memset((void *)assemble,0,sizeof(tagAssembleNode));
				assemble->ipsrc = ipsrc;
				assemble->ipdst = ipdst;
				assemble->proto = proto;
				assemble->id = id;
				assemble->dev = dev;
				assemble->timeout = DjyGetSysTime() + CN_ASSEMBLE_LIFE;
				if(0 == (fragment & CN_IP_MF_MSK))
				{
					assemble->lastbit = offset;
				}
				assemble->bitmap = (1<<offset);
				assemble->datalen = pkg->datalen;
				//add it to the queue
				pkg->private= (ptu32_t)offset;
				pkg->partnext = NULL;
				assemble->pkgq =pkg;
				assemble->qlen++;
				PkgCachedPart(pkg);
				//add the node to the queue
				assemble->nxt = pAssembleQueue;
				pAssembleQueue = assemble;
			}
		}
		else
		{
			//we find an old one
			//first check if the same offset comes
			if(0 == ((1<<offset)& assemble->bitmap))
			{
				//a new one, so add it to the queue
				assemble->bitmap = (1<<offset);
				assemble->datalen = pkg->datalen;
				if(0 == (fragment & CN_IP_MF_MSK))
				{
					assemble->lastbit = offset;
				}
				pkg->partnext = assemble->pkgq;
				assemble->pkgq =pkg;
				assemble->qlen++;
				PkgCachedPart(pkg);
			}
			if((assemble->lastbit > 0)&&(assemble->qlen == (assemble->lastbit +1)))
			{
				//the queue is ready, so do the check
				pkgassemble = PkgMalloc(assemble->datalen,CN_PKLGLST_END);
				if(NULL != pkgassemble)
				{
					src = NULL;
					dst = pkgassemble->buf + pkgassemble->offset;
					//fetch the package one by one in order
					for(offset = 0;offset < assemble->qlen;offset ++)
					{
						//find the one which has the same offset
						pkgtmp = assemble->pkgq;
						while(NULL != pkgtmp)
						{
							if(offset == (u16)pkgtmp->private)
							{
								break;
							}
							else
							{
								pkgtmp = pkgtmp->partnext;
							}
						}
						if(NULL != pkgtmp)
						{
							//do the data copy
							src = pkgtmp->buf + pkgtmp->offset;
							memcpy(dst,src,pkgtmp->datalen);
							pkgassemble->datalen += pkgtmp->datalen;
							dst += pkgtmp->datalen;
						}
						else
						{
							break; //not find yet, some thing is wrong
						}
					}
					if(offset != assemble->qlen)
					{
						//the frame may be not complet yet, so just drop it
						PkgTryFreePart(pkgassemble);
					}
					else
					{
						//ok, the frame is ok, so put it to the upper
//						IpTplHandler(proto,EN_IPV_4,(ptu32_t)ipsrc, (ptu32_t)ipdst,pkgassemble,dev);
				        addr.ver = EN_IPV_4;
				        addr.src.ipv4 = ipsrc;
				        addr.dst.ipv4 = ipdst;
				        IpProtoDeal(proto,&addr,pkg,dev);
						PkgTryFreePart(pkgassemble);
					}
				}
				//now remove the assemble and all the cached pkg
				if(assemble == pAssembleQueue)
				{
					pAssembleQueue = assemble->nxt;
				}
				else
				{
					pre->nxt = assemble->nxt;
				}
				PkgTryFreeQ(assemble->pkgq);
				free((void *)assemble);
			}
		}
		//here we'd better to check the assemble queue if any is timeout
		__assembletimeoutcheck();
		Lock_MutexPost(pAssembleSync);
	}
	return true;
}


// =============================================================================
// FUNCTION��This function is used to compute the pseudo header chksum
// PARA  IN��
// PARA OUT��
// RETURN  ��
// INSTRUCT: all the var is net endian
// =============================================================================
u16 IpPseudoChkSumV4(u32 iplocal, u32 ipdst, u8 proto, u16 translen)
{
    u16             chksum;
    tagPseudoHdr    pseudohdr;

    pseudohdr.ipsrc = iplocal;
    pseudohdr.ipdst = ipdst;
    pseudohdr.zero = 0;
    pseudohdr.protocol = proto;
    pseudohdr.len = htons(translen);
    chksum = IpChksumSoft16((void *)&pseudohdr, sizeof(pseudohdr),0, false);

    return chksum;
}
void IpPseudoPkgLstChkSumV4(u32 iplocal, u32 ipdst, u8  proto,tagNetPkg *pkg,u16 translen,u16 *chksum)
{
    int                             chklen;
    u16                             chksumtmp;
    void                            *chkbuf;
    tagPseudoHdr                    pseudohdr;   //����У���α����
    tagNetPkg                       *pkgtmp;

    pseudohdr.ipsrc = iplocal;
    pseudohdr.ipdst = ipdst;
    pseudohdr.zero = 0;
    pseudohdr.protocol = proto;
    pseudohdr.len = htons(translen);
    chksumtmp = IpChksumSoft16((void *)&pseudohdr, sizeof(pseudohdr),0, false);
    //У�����е�PKG
    pkgtmp = pkg;
    do{
        chklen = pkgtmp->datalen;
        chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
        if((NULL == pkgtmp->partnext)||PKG_ISLISTEND(pkgtmp))
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, \
                                chklen,chksumtmp, true);
            *chksum =(u16)chksumtmp;
            pkgtmp = NULL;
        }
        else
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf,  chklen,chksumtmp, false);
            pkgtmp = pkgtmp->partnext;
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
tagNetPkg *__Ipv4MakeHdr(u32 ipsrc,u32 ipdst,u8 proto,u32 translen,bool_t chksum)
{
    tagNetPkg    *result;
    tagIpHdrV4   *hdr;         //ip header

    result = PkgMalloc(sizeof(tagIpHdrV4),0);
    if(NULL != result)
    {
        hdr = (tagIpHdrV4 *)(result->buf + result->offset);
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
        result->datalen = sizeof(tagIpHdrV4);
        result->partnext = NULL;

        //check the ip head
        if(chksum)
        {
            IpPkgChkSum(result,&hdr->chksum,0);
        }
    }
    return result;
}

static bool_t Ipv4Snd2Rout(u32 ipsrc, u32 ipdst, tagNetPkg *pkg,u16 translen,u8 proto,\
                           u32 devtask, u16 *chksum,tagRout *rout)
{

    bool_t                          result;
    u32                             devfunc;     //rout function
    tagNetPkg                      *ippkg;       //ip header pkg
    bool_t                          ipchksum;
    u32                             framlen;
    result = false;

    if(ipsrc == INADDR_ANY)
    {
        ipsrc = rout->ipaddr.ipv4.ip;
    }
    devfunc = rout->func;
    if((0 == (devfunc&devtask))&&(CN_IPDEV_NONE!=devtask))
    {
        switch (devtask)
        {
            case  CN_IPDEV_TCPOCHKSUM:
                IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                break;
            case  CN_IPDEV_UDPOCHKSUM:
                IpPseudoPkgLstChkSumV4(ipsrc,ipdst,proto,pkg,translen,chksum);
                break;
            case  CN_IPDEV_ICMPOCHKSUM:
                IpPkgLstChkSum(pkg,chksum,0);
                break;
            default:
                result = false;
                break;
        }
        devtask = CN_IPDEV_NONE;
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
    ippkg = __Ipv4MakeHdr(ipsrc,ipdst,proto,translen,ipchksum);
    ippkg->partnext = pkg;
    framlen = ippkg->datalen + translen;
    result = LinkSend(rout,ippkg,framlen,devtask,EN_LINKPROTO_IPV4,EN_IPV_4,ipdst);
    PkgTryFreePart(ippkg);

    return result;
}

bool_t IpV4Send(u32 ipsrc, u32 ipdst, tagNetPkg *pkg,u16 translen,u8 proto,\
                u32 devtask, u16 *chksum)
{
    bool_t                          result;
    tagRout                        *rout;        //rout to send
    result = false;

    TCPIP_DEBUG_INC(gIpv4Statistics.sndnum);
    if(ipdst == INADDR_BROADCAST)
    {
        TCPIP_DEBUG_INC(gIpv4Statistics.sndbroad);
    	//this is a broadcast send,so send it to each rout
    	tagNetDev *dev;
        dev = NetDevGet(NULL);
        while(NULL != dev)
        {
            rout = dev->routq;
            if(NULL != rout)
            {
                result = Ipv4Snd2Rout(ipsrc,ipdst,pkg,translen,proto,devtask,chksum,rout);
                if(false == result)
                {
                    TCPIP_DEBUG_INC(gIpv4Statistics.snderrnum);
                }
            }
            dev = dev->nxt;
        }
    }
    else
    {
        rout = RoutMatch(EN_IPV_4,ipdst);
        if(NULL == rout)
        {
            TCPIP_DEBUG_INC(gIpv4Statistics.sndwan);
        	rout = RoutGetDefault(EN_IPV_4);
        }
        else
        {
            TCPIP_DEBUG_INC(gIpv4Statistics.sndsubnet);
        }
        if(NULL != rout)  //find a rout to send the frame
        {
            result = Ipv4Snd2Rout(ipsrc,ipdst,pkg,translen,proto,devtask,chksum,rout);
            if(result == false)
            {
                TCPIP_DEBUG_INC(gIpv4Statistics.snderrnum);
            }
        }
        else
        {
            TCPIP_DEBUG_INC(gIpv4Statistics.sndnoroutnum);
        }
    }
    return result;
}

// =============================================================================
// FUNCTION��This function is used to deal the ip mail,especially the ipv4
// PARA  IN��pkg,which load the message, maybe an list
//          devfunc, the netdev has done some task ,like the ip checksum or tcp checksum
// PARA OUT��
// RETURN  ��
// INSTRUCT:
// =============================================================================
static bool_t rcvlocal(tagNetPkg *pkg,tagNetDev *dev)
{
    bool_t                      result;
    u8                          proto;
    u32                         hdrlen;
    u32                         ipsrc;
    u32                         ipdst;
    tagIpHdrV4                 *hdr;
    u32                         devfunc;
    u16                         fragment;
    u16                         id;
    u16                         framlen;
    tagIpAddr addr;

    result = true;
    hdr = (tagIpHdrV4 *)(pkg->buf + pkg->offset);
    hdrlen = (hdr->ver_len&0x0f)*4;
    devfunc = NetDevGetFunc(dev);
    if((0 ==(devfunc &CN_IPDEV_IPICHKSUM))&&\
       (0 != IpChksumSoft16((void *)(pkg->buf + pkg->offset),hdrlen,0,true)))
    {
        TCPIP_DEBUG_INC(gIpv4Statistics.rcvdrop);
        return result;
    }
    memcpy(&fragment,&hdr->fragment,sizeof(fragment));
    fragment = ntohs(fragment);
    memcpy(&id,&hdr->id,sizeof(id));
    proto = hdr->protocol;
    memcpy(&ipdst,&hdr->ipdst,sizeof(ipdst));
    memcpy(&ipsrc,&hdr->ipsrc,sizeof(ipsrc));
    memcpy(&framlen,&hdr->len,sizeof(framlen));
    framlen = ntohs(framlen);

    pkg->offset += hdrlen;
    if(pkg->datalen < framlen)
    {
    	return result;
    }

    if(framlen > hdrlen)
    {
    	pkg->datalen = framlen - hdrlen;
    }
    else
    {
    	return result;
    }
    if((fragment & CN_IP_DF_MSK)||\
        ((0== (fragment&CN_IP_MF_MSK))&&(0 == (fragment&CN_IP_OFFMASK))))
    {
        //OK, this mail could go upper now, so remove the ip message
//        result = IpTplHandler(proto,EN_IPV_4,(ptu32_t)ipsrc, (ptu32_t)ipdst,pkg,dev);
        addr.ver = EN_IPV_4;
        addr.src.ipv4 = ipsrc;
        addr.dst.ipv4 = ipdst;
        result = IpProtoDeal(proto,&addr,pkg,dev);
    }
    else
    {
        //may be we need an reassemble here
        TCPIP_DEBUG_INC(gIpv4Statistics.rcvfragment);
        if(gEnableIpAssemble)
        {
            result = assemblepkg(ipsrc,ipdst,id,fragment,proto,dev,pkg);
        }
    }
    return result;
}
static bool_t rcvforward(tagNetPkg *pkg,tagNetDev *dev)
{
    return true;
}

//use this to deal with the ipv4 package
bool_t IpV4Process(tagNetPkg *pkg,tagNetDev *dev)
{
    bool_t                      result;
    u32                         ipdst;

    result = true;
    TCPIP_DEBUG_INC(gIpv4Statistics.rcvnum);
    memcpy(&ipdst,&((tagIpHdrV4 *)(pkg->buf + pkg->offset))->ipdst,sizeof(ipdst));

    if(RoutHostTarget(EN_IPV_4,ipdst))
    {
        result = rcvlocal(pkg,dev);
    }
    else
    {
        TCPIP_DEBUG_INC(gIpv4Statistics.rcvforward);
        result = rcvforward(pkg,dev);
    }
    return result;
}
bool_t Ipv4Show(char *param)
{
    printf("IPV4 STATISTICS:\n\r");
    printf("IPV4 STATISTICS:SND         :%d\n\r",gIpv4Statistics.sndnum);
    printf("IPV4 STATISTICS:SND  FAILED :%d\n\r",gIpv4Statistics.snderrnum);
    printf("IPV4 STATISTICS:SND  NOROUT :%d\n\r",gIpv4Statistics.sndnoroutnum);
    printf("IPV4 STATISTICS:SND  BROAD  :%d\n\r",gIpv4Statistics.sndbroad);
    printf("IPV4 STATISTICS:SND  WAN    :%d\n\r",gIpv4Statistics.sndwan);
    printf("IPV4 STATISTICS:SND  SUBNET :%d\n\r",gIpv4Statistics.sndsubnet);
    printf("IPV4 STATISTICS:RCV         :%d\n\r",gIpv4Statistics.rcvnum);
    printf("IPV4 STATISTICS:RCV  HOST   :%d\n\r",gIpv4Statistics.rcvhost);
    printf("IPV4 STATISTICS:RCV  DROPS  :%d\n\r",gIpv4Statistics.rcvdrop);
    printf("IPV4 STATISTICS:RCV  FORWARD:%d\n\r",gIpv4Statistics.rcvforward);
    printf("IPV4 STATISTICS:RCV  FRAG   :%d\n\r",gIpv4Statistics.rcvfragment);
    __showAssemble(NULL);
    return true;
}




bool_t  Ipv4Init(void)
{
	if(gEnableIpAssemble)
	{
		pAssembleSync = Lock_MutexCreate_s(&gAssembleSyncMem,NULL);
	}
	else
	{
		pAssembleSync = NULL;
	}
	return true;
}









