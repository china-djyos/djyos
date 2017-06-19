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
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ:
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ��������:������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <sys/socket.h>

#include "rout.h"
#include "ipV4.h"
#include "ip.h"
#include "tcpipconfig.h"

typedef struct
{
   u32 rcvnum;
   u32 rcverr;
   u32 rcvcached;
   u32 sndnum;
   u32 snderr;
   u32 unknownsnd;
   u32 unknownrcv;
}tagIpDebug;
//usage: we use this module to create the proto install and uninstall
typedef struct __tagIpProto
{
	void *nxt;
	const char *name;
	u8          proto;
	fnIpProto   dealer;
	//the following for the debug here
	u32         rcv;
	u32         rcverr;
	u32         snd;
	u32         snderr;
}tagIpProto;
#define CN_PROTO_HASHLEN  7
typedef struct
{
	tagNetPkg            *cachedH;
	tagNetPkg            *cachedT;
	struct MutexLCB      *mutex;
	struct SemaphoreLCB  *semp;
	struct MutexLCB       mutexsrc;
	struct SemaphoreLCB   sempsrc;
	u16                   evttID;
	u16                   eventID;
	tagIpProto           *prototab[CN_PROTO_HASHLEN]; //TODO,check if need the mutex to protect
	tagIpDebug            debug;
}tagIp;
static tagIp  gIpCtrl;
//add the proto to the hash tab
bool_t IpInstallProto(const char *name,u8 proto,fnIpProto handler)
{
	tagIpProto  *hash;
	int hashkey;
	hash = malloc(sizeof(tagIpProto));
	if(NULL != hash)
	{
		memset(hash,0,sizeof(tagIpProto));
		hash->proto = proto;
		hash->name = name;
		hash->dealer = handler;
		hash->nxt = NULL;
		hashkey = proto%CN_PROTO_HASHLEN;
		if(NULL == gIpCtrl.prototab[hashkey])
		{
			gIpCtrl.prototab[hashkey] = hash;
		}
		else
		{
			hash->nxt = gIpCtrl.prototab[hashkey];
			gIpCtrl.prototab[hashkey] = hash;
		}
	}
	return true;
}
//uninstall the hash to the hash tab--TODO,not support yet
bool_t IpUnInstallProto(const char *name,u8 proto,fnIpProto handler)
{
	return false;
}

//find the the hash
tagIpProto *IpProtoFind(u8 proto)
{
	tagIpProto  *hash;
	int hashkey;
	hashkey = proto%CN_PROTO_HASHLEN;
	hash = gIpCtrl.prototab[hashkey];
	while(NULL !=  hash)
	{
		if(hash->proto == proto)
		{
			break;
		}
		else
		{
			hash = hash->nxt;
		}
	}
	return hash;
}

//usage:use this function to upload the package
bool_t IpProtoDeal(u8 proto,tagIpAddr *addr,tagNetPkg *pkglst,tagNetDev *dev)
{
	bool_t result = false;
	tagIpProto  *hash;
	hash = IpProtoFind(proto);
	if((NULL != hash)&&(NULL != hash->dealer))
	{
		TCPIP_DEBUG_INC(hash->rcv);
		result =  hash->dealer(addr,pkglst,dev);
		if(false == result)
		{
			TCPIP_DEBUG_INC(hash->rcverr);
		}
	}
	return result;
}

//show all the ip status
static bool_t ipstatusshell(char *param)
{
	tagIpProto  *hash;
	printf("IPSTATUS:\n\r");
	printf("EVTTID  :0x%02x\n\r",gIpCtrl.evttID);
	printf("EVENTID :0x%02x\n\r",gIpCtrl.eventID);
	printf("RCVSEMP :%s\n\r",gIpCtrl.semp->lamp_counter>0?"active":"inactive");
	printf("RCVMUTEX:%s\n\r",gIpCtrl.mutex->enable?"inactive":"active");
	printf("SND     :0x%08x\n\r",gIpCtrl.debug.sndnum);
    printf("SNDERR  :0x%08x\n\r",gIpCtrl.debug.snderr);
    printf("RCV     :0x%08x\n\r",gIpCtrl.debug.rcvnum);
    printf("RCVERR  :0x%08x\n\r",gIpCtrl.debug.rcverr);
    printf("RCVCAC  :0x%08x\n\r",gIpCtrl.debug.rcvcached);
    int i = 0;
	printf("%5s %-5s %-10s %-10s %-10s %-10s\n\r",\
			"proto","No.","SndNum","SndErr","RcvNum","RcvErr");
    for( i = 0;i < CN_PROTO_HASHLEN;i++)
	{
    	hash = gIpCtrl.prototab[i];
    	while(NULL != hash)
    	{
    		printf("%-5s 0x%02x 0x%08x 0x%08x 0x%08x 0x%08x\n\r",
    				hash->name,hash->proto,hash->snd,hash->snderr,hash->rcv,hash->rcverr);
    		hash = hash->nxt;
    	}
	}
    return true;
}

//use this function to get a package from the arp package queue
static tagNetPkg* __IpPkgPop(void)
{
    tagNetPkg *result = NULL;
    if(Lock_MutexPend(gIpCtrl.mutex,CN_TIMEOUT_FOREVER))
    {
        if(NULL != gIpCtrl.cachedH)
        {
            if(gIpCtrl.cachedH ==  gIpCtrl.cachedT)
            {
                result = gIpCtrl.cachedH;
                gIpCtrl.cachedH = NULL;
                 gIpCtrl.cachedT = NULL;
            }
            else
            {
                result = gIpCtrl.cachedH;
                gIpCtrl.cachedH = result->partnext;
            }
            result->partnext = NULL;
            TCPIP_DEBUG_DEC(gIpCtrl.debug.rcvcached);
        }

        Lock_MutexPost(gIpCtrl.mutex);
    }
    return result;
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
bool_t  IpPushPkg(tagNetDev *dev,tagNetPkg *pkg)
{
    bool_t result = false;
    if(Lock_MutexPend(gIpCtrl.mutex,CN_TIMEOUT_FOREVER))
    {
        pkg->private = (ptu32_t)dev;
        if(NULL ==  gIpCtrl.cachedT)
        {
             gIpCtrl.cachedT = pkg;
             gIpCtrl.cachedH =  gIpCtrl.cachedT;
        }
        else
        {
             gIpCtrl.cachedT->partnext = pkg;
             gIpCtrl.cachedT           = pkg;
        }
        PkgCachedPart(pkg);
        TCPIP_DEBUG_INC(gIpCtrl.debug.rcvcached);
        Lock_MutexPost(gIpCtrl.mutex);

        Lock_SempPost(gIpCtrl.semp);
        result = true;
    }

    return result;
}

//this function used to scan the arp item tab with a fixed cycle
//if the life of the item is less than the trigger level, then it will send a request
//if the life is zero, then it will be delete from the queue
//we scan the item by the hash value, so we will not block the sync too much
#define CN_IP_VERMASK    (0X0f)
static ptu32_t  __IpProcessor(void)
{
    tagNetPkg    *pkg;
    tagNetDev    *dev;
    u8            ipv;
    bool_t        result;
    while(1)
    {
        if(Lock_SempPend(gIpCtrl.semp,CN_TIMEOUT_FOREVER))
        {
            while((pkg = __IpPkgPop()) != NULL)
            {
                TCPIP_DEBUG_INC(gIpCtrl.debug.rcvnum);

                dev = (tagNetDev *)pkg->private;
                if((NULL != dev)&& (NULL!= pkg))
                {
                    ipv = *(u8 *)(pkg->buf + pkg->offset);
                    ipv = (ipv >>4)&CN_IP_VERMASK;
                    if(ipv == EN_IPV_4)
                    {
                    	result = IpV4Process(pkg,dev);
                    	if(result == false)
                    	{
                    		TCPIP_DEBUG_INC(gIpCtrl.debug.rcverr);
                    	}
                    }
                    else
                    {
                        //do nothing here, unknown internet protocol version
                    }
                }
                PkgTryFreePart(pkg);
            }
        }
    }

    return 0;
}


// =============================================================================
// FUNCTION:This function is used to compute the checksum
// PARA  IN:buf, which storage the data
//          len, the length of buf
//          chksum, the original checksum
//          done,if true which means do the carry-over
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
u16 IpChksumSoft16(void *dataptr, int len,u16 chksum,bool_t done)
{
	u8 *pb = (u8 *)dataptr;
	u16 *ps, t = 0;
	u32 sum = 0;
	int odd = ((ptu32_t)pb & 1);
	u16 data16;

	/* Get aligned to u16 */
	if (odd && len > 0) {
		((u8 *)&t)[1] = *pb++;
		len--;
	}

	/* Add the bulk of the data */
	ps = (u16 *)(void *)pb;
	while (len > 1) {
		sum += *ps++;
		len -= 2;
	}

	/* Consume left-over byte, if any */
	if (len > 0) {
		((u8 *)&t)[0] = *(u8 *)ps;
	}

	/* Add end bytes */
	sum += t;

	/* Fold 32-bit sum to 16 bits
	 calling this twice is propably faster than if statements... */
	sum = (sum>>16)+ (sum&0xffff);
	sum = (sum>>16)+ (sum&0xffff);
	data16 = (u16)sum;
	/* Swap if alignment was odd */
	if (odd) {
		data16 = htons(data16);
	}

	//add the sum before
	sum = chksum + data16;
	sum = (sum>>16)+ (sum&0xffff);
	sum = (sum>>16)+ (sum&0xffff);
	data16 = (u16)sum;

	if(done)
	{
		data16 = ~data16;
	}

	return data16;
}

// =============================================================================
// FUNCTION:This function is used to compute one package checksum
// PARA  IN:pkg, which storage the data
//          chksum, used to strorage the checksum
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
void IpPkgChkSum(tagNetPkg *pkg,u16 *chksum,u16 sum)
{
    int                             chklen;
    u16                             result;
    void                            *chkbuf;

    chklen = pkg->datalen;
    chkbuf = (u8 *)(pkg->offset + pkg->buf);
    result = IpChksumSoft16((void *)chkbuf, chklen,sum, true);
    *chksum = result;
    return;
}
// =============================================================================
// FUNCTION:This function is used to compute one package list checksum
// PARA  IN:pkg, which storage the data
//          chksum, used to strorage the checksum
// PARA OUT:
// RETURN  :
// INSTRUCT:
// =============================================================================
void IpPkgLstChkSum(tagNetPkg *pkg,u16 *chksum,u16 sum)
{
    int                             chklen;
    u16                             chksumtmp;
    void                            *chkbuf;
    tagNetPkg                       *pkgtmp;

    chksumtmp = sum;
    pkgtmp = pkg;
    do{
        chklen = pkgtmp->datalen;
        chkbuf = (u8 *)(pkgtmp->offset + pkgtmp->buf);
        if((NULL == pkgtmp->partnext)||PKG_ISLISTEND(pkgtmp))
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, chklen,chksumtmp, true);
            *chksum = chksumtmp;
            pkgtmp = NULL;
        }
        else
        {
            chksumtmp = IpChksumSoft16((void *)chkbuf, chklen,chksumtmp, false);
            pkgtmp = pkgtmp->partnext;
        }
    }while(NULL != pkgtmp);
}
// =============================================================================
// FUNCTION:This function is used to make an interface for the transmit layer
// PARA  IN:ver:which means that EN_IPV_6 (which means ptu32_t is pointed to
//               16 byte array) or EN_IPV_4(which means ptu32_t is pointed to
//               an u32)
//          ipsrc, the local ip
//          ipdst, the detination ip
//          pkg,the transmit layer data, maybe an list
//          translen, the pkg (including the list) length
//          proto, the transmit layer proto
//          devtask, you may need the udp or tcp checksum
//          chksum, which storage the check sum
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================
bool_t IpSend(enum_ipv_t ver,ptu32_t ipsrc, ptu32_t ipdst, tagNetPkg *pkg,\
              u16 translen,u8 proto,u32  devtask, u16 *chksum)
{
    bool_t result;

    TCPIP_DEBUG_INC(gIpCtrl.debug.sndnum);

    tagIpProto  *hash;
    hash = IpProtoFind(proto);
    if(NULL != hash)
    {
    	TCPIP_DEBUG_INC(hash->snd);
    }
    if(ver == EN_IPV_4)
    {
        result = IpV4Send((u32)ipsrc, (u32)ipdst, pkg,translen,proto,devtask, chksum);
        if(false == result)
        {
        	TCPIP_DEBUG_INC(hash->snderr);
        }
    }
    else
    {
        result = false;   //not support yet
    }

    return result;
}
extern bool_t Ipv4Show(char *param);
extern bool_t TcpIpVersion(char *param);
struct ShellCmdTab  gIpDebug[] =
{
    {
        "ipv4",
        Ipv4Show,
        "usage:ipv4",
        "usage:ipv4",
    },
    {
        "ipstatus",
		ipstatusshell,
        "usage:ipstatus",
        "usage:ipstatus",
    },
    {
        "tcpipversion",
        TcpIpVersion,
        "usage:tcpipversion",
        "usage:tcpipversion",
    },
};

#define CN_IPDEBUG_ITEMNUM  ((sizeof(gIpDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gIpDebugCmdRsc[CN_IPDEBUG_ITEMNUM];
extern bool_t  Ipv4Init(void);
// =============================================================================
// FUNCTION:this function is used to init the ip module
// PARA  IN:
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:if success, then the transmit layer could send message to the ip layer
//          and the link layer could post package to ipv4 layer,absolutely this engine
//          creates the ip deal engine
// =============================================================================
bool_t IpInit(void)
{
    bool_t result;

    Ipv4Init();
    memset((void *)&gIpCtrl,0,sizeof(gIpCtrl));
    gIpCtrl.semp = Lock_SempCreate_s(&gIpCtrl.sempsrc,1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == gIpCtrl.semp)
    {
        goto EXIT_ENGINESYNC;
    }
    gIpCtrl.cachedH = NULL;
    gIpCtrl.cachedT = NULL;
    gIpCtrl.mutex = Lock_MutexCreate_s(&gIpCtrl.mutexsrc,NULL);
    if(NULL == gIpCtrl.mutex)
    {
        goto EXIT_PKGMUTEX;
    }
    gIpCtrl.evttID= Djy_EvttRegist(EN_CORRELATIVE, gIpRcvTaskPrior, 0, 1,
            (ptu32_t (*)(void))__IpProcessor,NULL, gIpRcvTaskStack, "IpProcessor");
    if(gIpCtrl.evttID == CN_EVTT_ID_INVALID)
    {
        goto EXIT_EVTTFAILED;
    }
    gIpCtrl.eventID = Djy_EventPop(gIpCtrl.evttID, NULL, 0, 0, 0, 0);
    if(gIpCtrl.eventID == CN_EVENT_ID_INVALID)
    {
        goto EXIT_EVENTFAILED;
    }
    result = Sh_InstallCmd(gIpDebug,gIpDebugCmdRsc,CN_IPDEBUG_ITEMNUM);
    return result;

EXIT_EVENTFAILED:
    Djy_EvttUnregist(gIpCtrl.evttID);
EXIT_EVTTFAILED:
    Lock_MutexDelete(gIpCtrl.mutex);
    gIpCtrl.mutex = NULL;
EXIT_PKGMUTEX:
    Lock_SempDelete(gIpCtrl.semp);
    gIpCtrl.semp = NULL;
EXIT_ENGINESYNC:
	memset((void *)&gIpCtrl,0,sizeof(gIpCtrl));
    result = false;
    return result;
}

