//----------------------------------------------------
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
#include <netdb.h>

#include "rout.h"
#include "tcpipconfig.h"
#include "arp.h"
#include "linkhal.h"

//this define for the arp protocol
#pragma pack(1)
typedef struct
{
#define CN_ARP_HW_TYPE_ETHERNET        0x01
    u16 hwtype;
    u16 protype;
    u8  hwaddrlen;
    u8  proaddrlen;
#define CN_ARP_OP_REQUEST              0x01
#define CN_ARP_OP_RESPONSE             0x02
    u16 opcode;
    u8  senhwaddr[CN_MACADDR_LEN];
    u8  senproaddr[CN_IPADDR_LEN_V4];
    u8  tarhwaddr[CN_MACADDR_LEN];
    u8  tarproaddr[CN_IPADDR_LEN_V4];
    u8  rev_blank[18];  // the ethernet need at least more than 60 bytes, so we add some reserved bytes
}tagArpHdr;
typedef struct
{
	u8   macto[CN_MACADDR_LEN];
	u8   macfrom[CN_MACADDR_LEN];
	u16  frametype;
}tagEthAddr;
typedef struct
{
	tagEthAddr  eth;
	tagArpHdr   arp;
}tagArpFrame;
#pragma pack()

#define CN_ARP_MSGLEN          (sizeof(tagArpHdr))
#define CN_ARPITEM_SCANCYCLE   (60*1000*mS)   //(30 seconds)
#define CN_ARPITEM_LIFE        (20)           //20 UNIT(20 minute)
#define CN_ARPITEM_LIFE_TRIG   (10)           //if life less than this, we will give the arp request
#define CN_ARP_FRAMELEN        (sizeof(tagArpFrame))

typedef struct  __ArpItem
{
   struct  __ArpItem  *nxt;                   //as the hash chain
   u32 ip;
   u8  mac[CN_MACADDR_LEN];
   u8  timeout;
   u8  pro;

#define CN_ARP_SYNC_TIME         (5*1000*mS) //anyway,we should at most 5 seconds
   struct SemaphoreLCB *semp;                //use this semp to sync all the task waiting for the same arp
   struct SemaphoreLCB  sempmem;
   u32 reffers;        //means how many times reffered, if none and the timeout is up
                       //this arp item will be deleted,each cycle will reset
}tagArpItem;
static struct MutexLCB      *pArpItemMutex = NULL;//use this to protect the arp item
static struct MutexLCB      gArpItemMutexMem     ;


static struct SemaphoreLCB  *pArpEngineSync = NULL; //use this to triger the arp scan thread
static struct SemaphoreLCB  gArpEngineSyncMem;

static tagArpItem          **pArpItemTab  = NULL;     //which used to arp hash chain
static tagNetPkg            *pArpPkgQH  = NULL;     //this used to storage the arp package head
static struct MutexLCB      *pArpPkgMutex = NULL; //this used to protect the arp package queue
static struct MutexLCB      gArpPkgMutexMem;
#define CN_ARPITEM_PRO_DYNAMIC (1<<0)         //set by the hand
#define CN_ARPITEM_PRO_STABLE  (1<<1)         //which means the mac could be use
#define CN_ARPITEM_PRO_NONE    (0)            //no property

bool_t ArpItemCreate(u32 ip,u8 *mac,u32 pro);
bool_t ArpItemDelete(u32 ip);
bool_t ArpItemUpdate(u32 ip,u8 *mac);
bool_t ArpTabClean();

typedef struct
{
   u32 sndnum;
   u32 snderr;
   u32 rcvnum;
   u32 rcvreq;
   u32 rcvres;
   u32 rcverr;
   u32 rcvcached;

   u64 resolvenum;
   u64 resolveerr;
}taggArpDebugs;
static taggArpDebugs gArpDebugs;
//use this function to match an item in the arp tab with specified ip address
static tagArpItem *__ArpMatchItem(u32 ip)
{
    tagArpItem  *result = NULL;
    u32          offset;
    tagArpItem *tmp;

    offset = ip%gArpItemHashLen;
    tmp = pArpItemTab[offset];
    while(NULL != tmp)
    {
        if(tmp->ip == ip)
        {
            result = tmp;
            break;
        }
        tmp = tmp->nxt;
    }

    return result;
}
//use this function to malloc mem and insert an arpitem to the arp item tab
static tagArpItem *__ArpCreateItem(u32 ip)
{
    tagArpItem *result;
    u32         offset;

    result = (tagArpItem *)malloc(sizeof(tagArpItem));
    if(NULL != result)
    {
        memset((void *)result,0,sizeof(tagArpItem));
        result->semp = Lock_SempCreate_s(&result->sempmem,1,0,CN_BLOCK_FIFO,NULL);
        if(NULL != result->semp)
        {
            //add this arp item to the hash tab
            result->ip = ip;
            offset = ip%gArpItemHashLen;
            result->nxt = pArpItemTab[offset];
            pArpItemTab[offset] = result;
        }
        else
        {
        	free(result);
        	result = NULL;
        }
    }
    return result;
}
//use this function to delete the arp item
static bool_t __ArpDeleteItem(u32 ip)
{
    bool_t      result = false;
    tagArpItem *tmp,*bak;
    u32         offset;

    offset = ip%gArpItemHashLen;
    tmp = pArpItemTab[offset];

    bak = tmp;
    while(NULL != tmp)
    {
        if(tmp->ip == ip)
        {
            //match this, so delete it
            if(pArpItemTab[offset] == tmp)
            {
                pArpItemTab[offset] = tmp->nxt;
            }
            else
            {
                bak->nxt = tmp->nxt;
            }
            Lock_SempDelete_s(tmp->semp);
            free((void *)tmp);
            result = true;
            break;
        }
        else
        {
            bak = tmp;
            tmp = tmp->nxt;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
//����:use this function to build a arp frame
//����:the ip is must be the net byte order,macdst and macsrc is for the arp proto
//    and the macto and macfrom is for the eth rout
//����:
//��ע:care that the macto and macfrom must not be NULL!
//����:zhangqf@����10:05:54/2017��3��14��
//-----------------------------------------------------------------------------
static tagNetPkg *__buildArpFrame(u8 *macto,u8 *macfrom,u32 ipdst,u32 ipsrc,\
		                          u8 *macdst,u8 *macsrc,u16 opcode)
{
	tagArpHdr         *arp;
	tagNetPkg         *pkg;
	tagArpFrame       *frame;
	tagEthAddr        *eth;

	//we will create an item for this ip
	pkg = PkgMalloc(CN_ARP_FRAMELEN, CN_PKLGLST_END);
	if(NULL != pkg)
	{
		frame = (tagArpFrame*)(pkg->buf + pkg->offset);
		memset((void *)frame,0,CN_ARP_FRAMELEN);
		//fill the arp proto
		arp = &frame->arp;
		memcpy(arp->tarproaddr ,&ipdst,CN_IPADDR_LEN_V4);
		memcpy(arp->senproaddr ,&ipsrc,CN_IPADDR_LEN_V4);
		if(NULL != macdst)
		{
			memcpy(arp->tarhwaddr, macdst, CN_MACADDR_LEN);
		}
		if(NULL != macsrc)
		{
			memcpy(arp->senhwaddr,macsrc,CN_MACADDR_LEN);
		}
		arp->hwtype = htons(CN_ARP_HW_TYPE_ETHERNET);
		arp->protype = htons(EN_LINKPROTO_IPV4);
		arp->hwaddrlen = CN_MACADDR_LEN;
		arp->proaddrlen = CN_IPADDR_LEN_V4;
		arp->opcode = htons(opcode);
		//fill the eth part
		eth = &frame->eth;
		memcpy(eth->macto, macto, CN_MACADDR_LEN);
		memcpy(eth->macfrom,macfrom,CN_MACADDR_LEN);
		eth->frametype = htons(EN_LINKPROTO_ARP);
		pkg->datalen = CN_ARP_FRAMELEN;
	}
	return pkg;
}

//use this function to do the arp request
static bool_t __ArpSndRequest(u32 ip)
{
    bool_t             result;
    tagNetPkg         *pkg;
    tagRout           *rout;
    u32                ipsrc;
    u32                ipdst;
    u8                *macsrc;
    u8                *macdst;
    u8                *macto;
    u8                *macfrom;

    result = false;
    ipdst = ip;
    macdst = NULL;
    TCPIP_DEBUG_INC(gArpDebugs.sndnum);
    //if the src is in the sub net, we will response it, else do nothing
    rout = RoutMatch(EN_IPV_4,ipdst);
    if(NULL == rout)
    {
    	return result; //NO ROUT FOR THIS IP TO FIND
    }
	macsrc = NetDevGetMac(rout->dev);
	ipsrc = rout->ipaddr.ipv4.ip;
	macto = (u8 *)CN_MAC_BROAD;
	macfrom = macsrc;
	pkg = __buildArpFrame(macto,macfrom,ipdst,ipsrc,macdst,macsrc,CN_ARP_OP_REQUEST);
	if(NULL != pkg)
	{
		result =LinkSendRaw(rout->dev,pkg,pkg->datalen,CN_IPDEV_NONE);
        PkgTryFreePart(pkg);
        result = true;
	}
    if(result == false)
    {
        TCPIP_DEBUG_DEC(gArpDebugs.snderr);
    }
    return result;
}
//-----------------------------------------------------------------------------
//����:use this function to send the arp response
//����:
//����:
//��ע:
//����:zhangqf@����11:10:56/2017��3��14��
//-----------------------------------------------------------------------------
//use this function to do the arp request
static bool_t __ArpSndResponse(u32 ip,u8 *mac,tagRout *rout)
{
    bool_t             result;
    tagNetPkg         *pkg;
    u32                ipsrc;
    u32                ipdst;
    u8                *macsrc;
    u8                *macdst;
    u8                *macto;
    u8                *macfrom;

    result = false;
    ipdst = ip;
    macdst = mac;
    TCPIP_DEBUG_INC(gArpDebugs.sndnum);
	macsrc = NetDevGetMac(rout->dev);
	ipsrc = rout->ipaddr.ipv4.ip;
	macto = macdst;
	macfrom = macsrc;
	pkg = __buildArpFrame(macto,macfrom,ipdst,ipsrc,macdst,macsrc,CN_ARP_OP_RESPONSE);
	if(NULL != pkg)
	{
		result =LinkSendRaw(rout->dev,pkg,pkg->datalen,CN_IPDEV_NONE);
        PkgTryFreePart(pkg);
        result = true;
	}
    if(result == false)
    {
        TCPIP_DEBUG_DEC(gArpDebugs.snderr);
    }
    return result;
}

//-----------------------------------------------------------------------------
//����:here we process the arp request
//����:
//����:
//��ע:
//����:zhangqf@����3:02:23/2016��12��29��
//-----------------------------------------------------------------------------
//use this function to deal with the arp request
static bool_t __ProcessRequst(tagNetDev *dev,tagArpHdr *arp)
{
    bool_t             result;
    u32                iphost;
    u32                ippeer;
    u32                ipdst;
    u32                ipsrc;
    u8                *macsrc;
    u8                *macdst;
    u8                *macto;
    u8                *macfrom;
    tagNetPkg         *pkg;

    result = false;
    memcpy(&iphost,arp->tarproaddr,CN_IPADDR_LEN_V4);
    memcpy(&ippeer,arp->senproaddr,CN_IPADDR_LEN_V4);
    //if we are the destination ip host, then we will send a response to it
    if(RoutHostIp(EN_IPV_4,iphost)&&(RoutSubNet(dev,EN_IPV_4,ippeer)||(ippeer == INADDR_ANY)))
    {
    	//do the response here
    	ipsrc = iphost;
    	ipdst = ippeer;
    	macsrc =NetDevGetMac(dev);
    	macfrom = macsrc;
    	macdst = arp->senhwaddr;
    	macto = macdst;
    	pkg = __buildArpFrame(macto,macfrom,ipdst,ipsrc,macdst,macsrc,CN_ARP_OP_RESPONSE);
		if(NULL != pkg)
		{
			result =LinkSendRaw(dev,pkg,pkg->datalen,CN_IPDEV_NONE);
			PkgTryFreePart(pkg);
			result = true;
		}
		if(result == false)
		{
			TCPIP_DEBUG_DEC(gArpDebugs.snderr);
		}
    }
    return result;
}
//-----------------------------------------------------------------------------
//����:here we process the arp response
//����:
//����:
//��ע:
//����:zhangqf@����3:02:55/2016��12��29��
//-----------------------------------------------------------------------------
static bool_t __ProcessResponse(tagNetDev *netdev,tagArpHdr *arp)
{
    bool_t             result;
    u32                ipsrc;
    u32                ipdst;
    u8                *macsrc;
    result = false;
    memcpy(&ipsrc,arp->senproaddr,CN_IPADDR_LEN_V4);
    macsrc = arp->senhwaddr;
    memcpy(&ipdst,arp->tarproaddr,CN_IPADDR_LEN_V4);
    //if the src is in the sub net, update the item
    if(RoutSubNet(netdev,EN_IPV_4,ipsrc)&&\
      (ipsrc != INADDR_ANY)&&(ipsrc != INADDR_BROAD)&&(0!= memcmp(macsrc,CN_MAC_BROAD,CN_MACADDR_LEN)))
    {
    	result = ArpItemUpdate(ipsrc,macsrc);
    }
    return result;
}

//use this function to get a package from the arp package queue
static tagNetPkg* __ArpPkgPop(void)
{
    tagNetPkg *result = NULL;
    if(Lock_MutexPend(pArpPkgMutex,CN_TIMEOUT_FOREVER))
    {
        result = pArpPkgQH;
        if(NULL != result)
        {
            pArpPkgQH = result->partnext;
            result->partnext = NULL;
            TCPIP_DEBUG_DEC(gArpDebugs.rcvcached);
        }
        Lock_MutexPost(pArpPkgMutex);
    }
    return result;
}
//this function used to process the arp package
static void __ArpPkgDeal(void)
{
    tagArpHdr    *hdr;
    u16           opcode;
    tagNetDev    *dev;
    tagNetPkg    *pkg;

    while((pkg = __ArpPkgPop()) != NULL)
    {
        dev = (tagNetDev *)pkg->private;

        if((NULL != dev)&& (NULL!= pkg))
        {
            hdr = (tagArpHdr *)(pkg->buf + pkg->offset);
            pkg->offset += sizeof(tagArpHdr);
            pkg->datalen -= sizeof(tagArpHdr);
            opcode = ntohs(hdr->opcode);
            switch (opcode)
            {
                case CN_ARP_OP_REQUEST:
                    __ProcessRequst(dev,hdr);
                    TCPIP_DEBUG_INC(gArpDebugs.rcvreq);
                    break;
                case CN_ARP_OP_RESPONSE:
                    __ProcessResponse(dev,hdr);
                    TCPIP_DEBUG_INC(gArpDebugs.rcvres);
                    break;
                default:
                    TCPIP_DEBUG_INC(gArpDebugs.rcverr);
                    break;
            }
        }
        PkgTryFreePart(pkg);
    }
}
// =============================================================================
// FUNCTION:this function is the arp post function. the link will call this function
//          to deal with arp package
// PARA  IN:dev, which dev receive the package
//          pkg, the arp frame
// PARA OUT:
// RETURN  :true post success while false failed
// INSTRUCT:
// =============================================================================
bool_t  ArpPushPkg(tagNetDev *dev,tagNetPkg *pkg)
{
    bool_t result = false;

    TCPIP_DEBUG_INC(gArpDebugs.rcvnum);

    if(Lock_MutexPend(pArpPkgMutex,CN_TIMEOUT_FOREVER))
    {
        pkg->private = (ptu32_t)dev;
        pkg->partnext = pArpPkgQH;
        pArpPkgQH = pkg;
        PkgCachedPart(pkg);
        TCPIP_DEBUG_INC(gArpDebugs.rcvcached);
        Lock_MutexPost(pArpPkgMutex);

        Lock_SempPost(pArpEngineSync);
        result = true;
    }

    return result;
}

//this function used to scan the arp item tab with a fixed cycle
//if the life of the item is less than the trigger level, then it will send a request
//if the life is zero, then it will be delete from the queue
//we scan the item by the hash value, so we will not block the sync too much
static ptu32_t  __ArpProcessor(void)
{
    u32         offset;
    tagArpItem *tmp;
    tagArpItem *bak;

    while(1)
    {
        if(Lock_SempPend(pArpEngineSync,CN_ARPITEM_SCANCYCLE))
        {
            //there is some arp package that we should process
            __ArpPkgDeal(); //deal all the arp package
        }
        else
        {
            //timeout we should process all the timeout arp item
            for(offset =0; offset < gArpItemHashLen;offset ++)
            {
                if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
                {
                    tmp = pArpItemTab[offset];
                    while(NULL != tmp)
                    {
                        bak = tmp->nxt;
                        if(tmp->timeout == 0)
                        {
                            __ArpDeleteItem(tmp->ip);
                        }
                        else if(tmp->timeout <= CN_ARPITEM_LIFE_TRIG)
                        {
                            //we should send an arp request
                            __ArpSndRequest(tmp->ip);
                            tmp->timeout--;
                        }
                        else
                        {
                            //we need do nothing here
                            if(tmp->pro & CN_ARPITEM_PRO_DYNAMIC)
                            {
                                tmp->timeout--;
                            }
                        }
                        tmp = bak;
                    }
                    Lock_MutexPost(pArpItemMutex);
                }
            }
        }
    }

    return 0;
}

// =============================================================================
// FUNCTION:use this function to update the arp item
// PARA  IN:ip,the is an ipv4 address
//         :mac, the corresponding mac address with ip
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t ArpItemUpdate(u32 ip,u8 *mac)
{
    bool_t      result = false;
    tagArpItem *tmp;

    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        tmp = __ArpMatchItem(ip);
        if((NULL != tmp)&&(tmp->pro &CN_ARPITEM_PRO_DYNAMIC))
        {
            memcpy(tmp->mac,mac,CN_MACADDR_LEN);
            tmp->pro |= CN_ARPITEM_PRO_STABLE;
            tmp->timeout = CN_ARPITEM_LIFE;
            //post all the task pend on the arp
            while(Lock_SempCheckBlock(tmp->semp))
            {
            	Lock_SempPost(tmp->semp);
            }
        }
        Lock_MutexPost(pArpItemMutex);
    }
    return result;
}

// =============================================================================
// FUNCTION:use this function to create a arp item
// PARA  IN:ip,the is an ipv4 address
//         :mac, the corresponding mac address with ip
//         :pro, the item property, such as dynamic or static
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t ArpItemCreate(u32 ip,u8 *mac,u32 pro)
{
    bool_t      result = false;
    tagArpItem *tmp;

    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        tmp = __ArpMatchItem(ip);
        if(NULL != tmp)
        {
            memcpy(tmp->mac,mac,CN_MACADDR_LEN);
            tmp->pro = pro|CN_ARPITEM_PRO_STABLE;
            tmp->timeout = CN_ARPITEM_LIFE;
            result = true;
        }
        else
        {
            tmp = __ArpCreateItem(ip);
            if(NULL != tmp)
            {
                memcpy(tmp->mac,mac,CN_MACADDR_LEN);
                tmp->pro = pro|CN_ARPITEM_PRO_STABLE;
                tmp->timeout = CN_ARPITEM_LIFE;
                result = true;
            }
        }
        Lock_MutexPost(pArpItemMutex);
    }
    return result;
}
// =============================================================================
// FUNCTION:use this function to delete the arp item
// PARA  IN:ip,the is an ipv4 address
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t ArpItemDelete(u32 ip)
{
    bool_t      result = false;

    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        result = __ArpDeleteItem(ip);

        Lock_MutexPost(pArpItemMutex);
    }
    return result;
}

// =============================================================================
// FUNCTION:use this function to find the mac with the specified address
// PARA  IN:ip,the is an ipv4 address
//          mac, use to storage the mac address
// PARA OUT:
// RETURN  :true find while false not or finding now
// INSTRUCT:
// =============================================================================
bool_t ArpFindMac(u32 ip,u8 *mac)
{
    bool_t      result = false;
    tagArpItem *tmp;

    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        TCPIP_DEBUG_INC(gArpDebugs.resolvenum);
        tmp = __ArpMatchItem(ip);
        if(NULL == tmp)
        {
            //which means no item matches, so we should build a item and send the
            //arp request
            tmp =__ArpCreateItem(ip);
            if(NULL != tmp)
            {
                tmp->pro = CN_ARPITEM_PRO_DYNAMIC;
                tmp->timeout = CN_ARPITEM_LIFE_TRIG;
                __ArpSndRequest(ip);
            }
        }
        else
        {
            if(tmp->pro &CN_ARPITEM_PRO_STABLE)
            {
                memcpy(mac,tmp->mac,CN_MACADDR_LEN);
                tmp->reffers++;
                result = true;
            }
            else
            {
                __ArpSndRequest(ip);  //do an arp request
            }
        }
        Lock_MutexPost(pArpItemMutex);
    }
    if((false == result)&&(NULL != tmp))
    {
    	//just for the wait
    	Lock_SempPend(tmp->semp,CN_ARP_SYNC_TIME);
    	//check once more
        if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
        {
            if(tmp->pro &CN_ARPITEM_PRO_STABLE)
            {
                memcpy(mac,tmp->mac,CN_MACADDR_LEN);
                tmp->reffers++;
                result = true;
            }
            else
            {
                TCPIP_DEBUG_INC(gArpDebugs.resolveerr);
            }
            Lock_MutexPost(pArpItemMutex);
        }
    }
    return result;
}
// =============================================================================
// FUNCTION:use this function to clean all the arp item
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
bool_t ArpTabClean()
{
    bool_t      result = false;
    tagArpItem *tmp;
    u32 offset;

    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        for(offset =0; offset < gArpItemHashLen;offset ++)
        {
            tmp = pArpItemTab[offset];
            while(NULL != tmp)
            {
                pArpItemTab[offset] = tmp->nxt;
                free((void *)tmp);
                tmp = pArpItemTab[offset];
            }
        }

        result = true;
        Lock_MutexPost(pArpItemMutex);
    }

    return result;
}


//tell all the others this rout has existed
bool_t ArpBroadRout(tagRout *rout)
{
    bool_t      result = false;
	u32         offset;
    tagArpItem *tmp;
    u32         ip;
    u8         *mac;

    if(NULL != rout)
    {
        //timeout we should process all the timeout arp item
        for(offset =0; offset < gArpItemHashLen;offset ++)
        {
            if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
            {
                tmp = pArpItemTab[offset];
                while(NULL != tmp)
                {
                    //we need do nothing here
                    if(tmp->pro & CN_ARPITEM_PRO_STABLE)
                    {
                    	ip = tmp->ip;
                    	mac = tmp->mac;
                    	__ArpSndResponse(ip,mac,rout);
                    }
                    tmp = tmp->nxt;
                }
                Lock_MutexPost(pArpItemMutex);
            }
        }
        result = true;
    }
	return result;
}
bool_t ArpInform(char *param)
{
    tagRout *rout;
    tagNetDev *dev;

    if(NULL == param)
    {
        return false;
    }
    dev = NetDevGet((const char *)param);
    if(NULL == dev)
    {
        return false;
    }
    rout = dev->routq;
    while(NULL != rout)
    {
    	ArpBroadRout(rout);

    	rout = rout->nxt;
    }
    return true;
}

//debug the arp module
static bool_t ArpShow(char *param)
{
    bool_t      result = false;
    u32         offset = 0;
    u32         num = 0;
    tagArpItem *tmp;
    struct in_addr  addr;
    if(Lock_MutexPend(pArpItemMutex,CN_TIMEOUT_FOREVER))
    {
        printf("ArpItem:\r\n");
        printf("%-6s%-18s%-18s%-8s%-6s%-10s%-10s\n\r",\
        		"Item","IpAddress","MacAddress","Type","Life","Refers","Property");
        for(offset =0;offset < gArpItemHashLen;offset++)
        {
            tmp = pArpItemTab[offset];
            while(tmp != NULL)
            {
                memcpy((void *)&addr,(void *)&tmp->ip,sizeof(addr));
                printf("%-4d  %-18s%-18s%-8s%-4d  %-8d  %-8d\n\r",\
                        num,inet_ntoa(addr),Mac2string(tmp->mac),\
                        tmp->pro&CN_ARPITEM_PRO_DYNAMIC?"dynamic":"static",tmp->timeout,tmp->reffers,tmp->pro);
                tmp = tmp->nxt;
                num++;
            }
        }
        Lock_MutexPost(pArpItemMutex);

        printf("ArpTotal:%d :hashlen:%d\n\r",num,gArpItemHashLen);
        printf("ARP STATISTICS:\n\r");
        printf("ARP STATISTICS:SND         :%d\n\r",gArpDebugs.sndnum);
        printf("ARP STATISTICS:SND  ERR    :%d\n\r",gArpDebugs.snderr);
        printf("ARP STATISTICS:RCV         :%d\n\r",gArpDebugs.rcvnum);
        printf("ARP STATISTICS:RCV  REQ    :%d\n\r",gArpDebugs.rcvreq);
        printf("ARP STATISTICS:RCV  RES    :%d\n\r",gArpDebugs.rcvres);
        printf("ARP STATISTICS:RCV  ERR    :%d\n\r",gArpDebugs.rcverr);
        printf("ARP STATISTICS:RCV  CACHED :%d\n\r",gArpDebugs.rcvcached);
        printf("ARP STATISTICS:RESOLVE     :0x%llx\n\r",gArpDebugs.resolvenum);
        printf("ARP STATISTICS:RESOLVE ERR :0x%llx\n\r",gArpDebugs.resolveerr);

        result = true;
    }
    return result;
}
//add arp
static bool_t ArpAdd(char *param)
{
    u8   mac[CN_MACADDR_LEN];
    int  argc = 2;
    char *argv[2];
    bool_t result;
    struct in_addr addr;

    result = false;
    string2arg(&argc,argv,param);
    if(argc == 2)
    {
        if((inet_aton(argv[0],&addr))&&\
           string2Mac(argv[1],mac))
        {
            result = ArpItemCreate(addr.s_addr,mac,CN_ARPITEM_PRO_NONE);
        }
    }
    return result;
}
//delete a specified arp or all
static bool_t ArpDel(char *param)
{
    int  argc = 2;
    char *argv[2];
    bool_t result;
    struct in_addr addr;

    result = false;
    string2arg(&argc,argv,param);
    if(argc == 1)
    {
        if(inet_aton(argv[0],&addr))
        {
            result = ArpItemDelete(addr.s_addr);
        }
    }
    else
    {
        //clean all the arp
        result = ArpTabClean();
    }
    return result;
}

struct ShellCmdTab  gArpDebug[] =
{
    {
        "arp",
        ArpShow,
        "usage:arp",
        "usage:arp",
    },
    {
        "arpadd",
        ArpAdd,
        "usage:arpadd ip  mac",
        "usage:arpadd ip  mac",
    },
    {
        "arpdel",
        ArpDel,
        "usage:arpdel [ip]",
        "usage:arpadd [ip]",
    },
    {
        "arpinform",
        ArpInform,
        "usage:arpinform name",
        "usage:arpinform name",
    },
};
#define CN_ARPDEBUG_NUM  ((sizeof(gArpDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gArpDebugCmdRsc[CN_ARPDEBUG_NUM];
// =============================================================================
// FUNCTION:this is the arp module init function
// PARA  IN:
// PARA OUT:
// RETURN  :true find while false not
// INSTRUCT:
// =============================================================================
bool_t ArpInit(void)
{
    bool_t result;
    u16   evttID;
    u16   eventID;

    pArpItemMutex = Lock_MutexCreate_s(&gArpItemMutexMem,NULL);
    if(NULL == pArpItemMutex)
    {
        goto EXIT_ITEMMUTEX;
    }
    pArpEngineSync = Lock_SempCreate_s(&gArpEngineSyncMem,1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == pArpEngineSync)
    {
        goto EXIT_ENGINESYNC;
    }
    pArpItemTab = (tagArpItem **)malloc(gArpItemHashLen*sizeof(void *));
    if(NULL == pArpItemTab)
    {
        goto EXIT_ITEMTAB;
    }
    memset((void *)pArpItemTab,0,gArpItemHashLen*sizeof(void *));
    pArpPkgQH = NULL;

    pArpPkgMutex = Lock_MutexCreate_s(&gArpPkgMutexMem,NULL);
    if(NULL == pArpItemMutex)
    {
        goto EXIT_PKGMUTEX;
    }
    evttID= Djy_EvttRegist(EN_CORRELATIVE, gArpTaskPrior, 0, 1,
            (ptu32_t (*)(void))__ArpProcessor,NULL, gArpTaskStack, "ArpProcessor");
    if(evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    eventID = Djy_EventPop(evttID, NULL, 0, 0, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
        goto EXIT_EVENTFAILED;
    }
    result = Sh_InstallCmd(gArpDebug,gArpDebugCmdRsc,CN_ARPDEBUG_NUM);

    tcpipmemlog("arptab",sizeof(void *),gArpItemHashLen);

    return result;

EXIT_EVENTFAILED:
    Djy_EvttUnregist(evttID);
EXIT_EVTTFAILED:
    Lock_MutexDelete(pArpPkgMutex);
    pArpPkgMutex = NULL;
EXIT_PKGMUTEX:
    free((void *)pArpItemTab);
    pArpItemTab = NULL;
EXIT_ITEMTAB:
    Lock_SempDelete(pArpEngineSync);
    pArpEngineSync = NULL;
EXIT_ENGINESYNC:
    Lock_MutexDelete(pArpItemMutex);
    pArpItemMutex = NULL;
EXIT_ITEMMUTEX:
    result = false;
    return result;
}











