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
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netbsp.h>
#include "dbug.h"

#include "arp.h"
#include <shell.h>
#include "../component_config_tcpip.h"

#include "../common/netdev.h"
#include "../common/router.h"
#include "../common/link.h"



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
}tagArpPH;
typedef struct
{
    u8   macto[CN_MACADDR_LEN];
    u8   macfrom[CN_MACADDR_LEN];
    u16  frametype;
}tagEthAddr;
typedef struct
{
    tagEthAddr  eth;
    tagArpPH   arp;
}tagArpFrame;
#pragma pack()

#define CN_ARP_MSGLEN          (sizeof(tagArpPH))
#define CN_ARPITEM_SCANCYCLE   (60*1000*mS)   //(30 seconds)
#define CN_ARPITEM_LIFE        (20)           //20 UNIT(20 minute)
#define CN_ARPITEM_LIFE_TRIG   (10)           //if life less than this, we will give the arp request
#define CN_ARP_FRAMELEN        (sizeof(tagArpFrame))

typedef struct
{
   void  *nxt;                   //as the hash chain
   u32 iphost;
   u32 ippeer;
   void *iface;
   u8  mac[CN_MACADDR_LEN];
   u8  timeout;
   u8  pro;
#define CN_ARP_SYNC_TIME         (1*1000*mS) //anyway,we should at most 1 seconds
   semp_t semp;                //use this semp to sync all the task waiting for the same arp
   u32 reffers;        //means how many times reffered, if none and the timeout is up
                       //this arp item will be deleted,each cycle will reset
}tagArpItem;
typedef struct
{
    mutex_t lock;
    tagArpItem **hashtab;
}tagArpCB;
static tagArpCB gArpCB;
#define CN_ARPITEM_PRO_DYNAMIC (1<<0)         //set by the hand
#define CN_ARPITEM_PRO_STABLE  (1<<1)         //which means the mac could be use
#define CN_ARPITEM_PRO_NONE    (0)            //no property
//use this function to match an item in the arp tab with specified ip address
static tagArpItem *__ItemMatch(u32 ippeer)
{
    tagArpItem  *ret = NULL;
    u32          offset;
    tagArpItem *item;
    offset = ippeer%CFG_ARP_HASHLEN;
    item = gArpCB.hashtab[offset];
    while(NULL != item)
    {
        if(item->ippeer == ippeer)
        {
            ret = item;
            break;
        }
        item = item->nxt;
    }
    return ret;
}
//use this function to net_malloc mem and insert an arpitem to the arp item tab
static tagArpItem *__ItemCreate(u32 ippeer,u32 iphost,void *iface)
{
    tagArpItem *ret;
    u32         offset;

    ret = (tagArpItem *)net_malloc(sizeof(tagArpItem));
    if(NULL != ret)
    {
        memset((void *)ret,0,sizeof(tagArpItem));
        ret->semp= semp_init(0X7fffffff,0,NULL); //almost unlimited
        if(NULL != ret->semp)
        {
            //add this arp item to the hash tab
            ret->ippeer = ippeer;
            ret->iphost = iphost;
            ret->iface = iface;
            offset = ippeer%CFG_ARP_HASHLEN;
            ret->nxt = gArpCB.hashtab[offset];
            gArpCB.hashtab[offset] = ret;
        }
        else
        {
            net_free(ret);
            ret = NULL;
        }
    }
    return ret;
}
//use this function to delete the arp item
static bool_t __ItemDel(u32 ippeer)
{
    bool_t      ret = false;
    tagArpItem *tmp,*bak;
    u32         offset;
    offset = ippeer%CFG_ARP_HASHLEN;
    tmp = gArpCB.hashtab[offset];
    bak = tmp;
    while(NULL != tmp)
    {
        if(tmp->ippeer == ippeer)
        {
            //match this, so delete it
            if(gArpCB.hashtab[offset] == tmp)
            {
                gArpCB.hashtab[offset] = tmp->nxt;
            }
            else
            {
                bak->nxt = tmp->nxt;
            }
            semp_del(tmp->semp);
            net_free((void *)tmp);
            ret = true;
            break;
        }
        else
        {
            bak = tmp;
            tmp = tmp->nxt;
        }
    }
    return ret;
}
//update a item
static bool_t __ItemUpdate(u32 ip,u8 *mac)
{
    bool_t      result = false;
    tagArpItem *tmp;

    if(mutex_lock(gArpCB.lock))
    {
        tmp = __ItemMatch(ip);
        if((NULL != tmp)&&(tmp->pro &CN_ARPITEM_PRO_DYNAMIC))
        {
            memcpy(tmp->mac,mac,CN_MACADDR_LEN);
            tmp->pro |= CN_ARPITEM_PRO_STABLE;
            tmp->timeout = CN_ARPITEM_LIFE;
            //post all the task pend on the arp
            while(Lock_SempCheckBlock(tmp->semp))
            {
                semp_post(tmp->semp);
            }
        }
        mutex_unlock(gArpCB.lock);
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
static bool_t __TabClean(void)
{
    bool_t      ret = false;
    tagArpItem *tmp;
    u32 offset;
    if(mutex_lock(gArpCB.lock))
    {
        for(offset =0; offset < CFG_ARP_HASHLEN;offset ++)
        {
            tmp = gArpCB.hashtab[offset];
            while(NULL != tmp)
            {
                gArpCB.hashtab[offset] = tmp->nxt;
                semp_del(tmp->semp);
                net_free((void *)tmp);
                tmp = gArpCB.hashtab[offset];
            }
        }
        mutex_unlock(gArpCB.lock);
        ret = true;
    }
    return ret;
}
//debug the arp module
static bool_t __TabShow(void)
{
    bool_t      ret = false;
    u32         offset = 0;
    u32         num = 0;
    tagArpItem *tmp;
    struct in_addr  addr;
    if(mutex_lock(gArpCB.lock))
    {
        OsPrintSplit('*',100);
        debug_printf("arp","ArpItem:\r\n");
        debug_printf("arp","%-6s%-18s%-18s%-8s%-6s%-10s%-10s\n\r",\
                "Item","IpAddress","MacAddress","Type","Life","Refers","Property");
        for(offset =0;offset < CFG_ARP_HASHLEN;offset++)
        {
            tmp = gArpCB.hashtab[offset];
            while(tmp != NULL)
            {
                memcpy((void *)&addr,(void *)&tmp->ippeer,sizeof(addr));
                debug_printf("arp","%-4d  %-18s%-18s%-8s%-4d  %-8d  %-8d\n\r",\
                        num,inet_ntoa(addr),mac2string(tmp->mac),\
                        tmp->pro&CN_ARPITEM_PRO_DYNAMIC?"dynamic":"static",tmp->timeout,tmp->reffers,tmp->pro);
                tmp = tmp->nxt;
                num++;
            }
        }
        mutex_unlock(gArpCB.lock);

        debug_printf("arp","ArpTotal:%d :hashlen:%d\n\r",num,CFG_ARP_HASHLEN);
        OsPrintSplit('*',100);
        ret = true;
    }
    return ret;
}


//-----------------------------------------------------------------------------
//功能:use this function to build a arp frame
//参数:the ip is must be the net byte order,macdst and macsrc is for the arp proto
//    and the macto and macfrom is for the eth rout
//返回:
//备注:care that the macto and macfrom must not be NULL!
//作者:zhangqf@上午10:05:54/2017年3月14日
//-----------------------------------------------------------------------------
static tagNetPkg *__BuildArppH(u8 *macto,u8 *macfrom,u32 ipdst,u32 ipsrc,\
                                  u8 *macdst,u8 *macsrc,u16 opcode)
{
    tagArpPH         *arp;
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

static bool_t __SndReq(u32 ippeer,u32 iphost,void *iface)
{
    bool_t             ret = false;
    tagNetPkg         *pkg;
    u8                *macto;
    u8                *macfrom;

    macfrom = (u8 *)NetDevGetMac(iface);
    macto = (u8 *)CN_MAC_BROAD;
    pkg = __BuildArppH(macto,macfrom,ippeer,iphost,macto,macfrom,CN_ARP_OP_REQUEST);
    if(NULL != pkg)
    {
        ret =NetDevSend(iface,pkg,pkg->datalen,CN_IPDEV_NONE);
        PkgTryFreePart(pkg);
        ret = true;
    }
    return ret;
}

static bool_t __SndRes(u32 ippeer,u32 iphost,u8 *macpeer,void *iface)
{
    bool_t             ret = false;
    tagNetPkg         *pkg;
    u8                *macto;
    u8                *macfrom;

    macfrom = (u8 *)NetDevGetMac(iface);
    macto = macpeer;
    pkg = __BuildArppH(macto,macfrom,ippeer,iphost,macto,macfrom,CN_ARP_OP_RESPONSE);
    if(NULL != pkg)
    {
        ret =NetDevSend(iface,pkg,pkg->datalen,CN_IPDEV_NONE);
        PkgTryFreePart(pkg);
        ret = true;
    }
    return ret;
}
//-----------------------------------------------------------------------------
//功能:here we process the arp request
//参数:
//返回:
//备注:
//作者:zhangqf@下午3:02:23/2016年12月29日
//-----------------------------------------------------------------------------
//use this function to deal with the arp request
static bool_t __DealReq(void *iface,tagArpPH *arp)
{
    bool_t             ret = true;
    u32                iphost;
    u32                ippeer;
    memcpy(&iphost,arp->tarproaddr,CN_IPADDR_LEN_V4);
    memcpy(&ippeer,arp->senproaddr,CN_IPADDR_LEN_V4);
    //if we are the destination ip host, then we will send a response to it
    tagRoutLink  rout;
    memset(&rout,0,sizeof(rout));
    rout.ver = EN_IPV_4;
    rout.dst = &iphost;
    if(RouterMatch(&rout)&&(rout.type==EN_IPTYPE_V4_LOCAL))
    {
        ret= __SndRes(ippeer,iphost,arp->senhwaddr,iface);//do the response here
    }
    return ret;
}

//deal the response frame
static bool_t __DealRes(void *iface,tagArpPH *arp)
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
    if((ipsrc != INADDR_ANY)&&(ipsrc != INADDR_BROAD)&&(0!= memcmp(macsrc,CN_MAC_BROAD,CN_MACADDR_LEN)))
    {
        result = __ItemUpdate(ipsrc,macsrc);
    }
    return result;
}
//this function used to process the arp package
static bool_t __ArpPush(void *iface,tagNetPkg *pkg)
{
    bool_t ret = true;
    tagArpPH    *hdr;
    u16          opcode;
    if((NULL != iface)&& (NULL!= pkg))
    {
        hdr = (tagArpPH *)(pkg->buf + pkg->offset);
        pkg->offset += sizeof(tagArpPH);
        pkg->datalen -= sizeof(tagArpPH);
        opcode = ntohs(hdr->opcode);
        switch (opcode)
        {
            case CN_ARP_OP_REQUEST:
                __DealReq(iface,hdr);
                break;
            case CN_ARP_OP_RESPONSE:
                __DealRes(iface,hdr);
                break;
            default: //drops here
                break;
        }
    }
    return ret;
}

//this function used to scan the arp item tab with a fixed cycle
//if the life of the item is less than the trigger level, then it will send a request
//if the life is zero, then it will be delete from the queue
//we scan the item by the hash value, so we will not block the sync too much
static void  __ArpTicker(void)
{
    u32         offset;
    tagArpItem *tmp;
    tagArpItem *bak;
    //timeout we should process all the timeout arp item
    for(offset =0; offset < CFG_ARP_HASHLEN;offset ++)
    {
        if(mutex_lock(gArpCB.lock))
        {
            tmp = gArpCB.hashtab[offset];
            while(NULL != tmp)
            {
                bak = tmp->nxt;
                if(tmp->timeout == 0)
                {
                    __ItemDel(tmp->ippeer);
                }
                else if(tmp->timeout <= CN_ARPITEM_LIFE_TRIG)
                {
                    //we should send an arp request
                    __SndReq(tmp->ippeer,tmp->iphost,tmp->iface);
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
            mutex_unlock(gArpCB.lock);
        }
    }
    return ;
}

// =============================================================================
// FUNCTION:use this function to update the arp item
// PARA  IN:ip,the is an ipv4 address
//         :mac, the corresponding mac address with ip
// PARA OUT:
// RETURN  :true success while false failed
// INSTRUCT:
// =============================================================================

// =============================================================================
// FUNCTION:use this function to find the mac with the specified address
// PARA  IN:ip,the is an ipv4 address
//          mac, use to storage the mac address
// PARA OUT:
// RETURN  :true find while false not or finding now
// INSTRUCT:
// =============================================================================
bool_t ResolveMacByArp(u32 ippeer,u32 iphost,void *iface,u8 *macbuf)
{
    bool_t      ret = false;
    tagArpItem *tmp;

    if(mutex_lock(gArpCB.lock))
    {
        tmp = __ItemMatch(ippeer);
        if(NULL == tmp)
        {
            //which means no item matches, so we should build a item and send the
            //arp request
            tmp =__ItemCreate(ippeer,iphost,iface);
            if(NULL != tmp)
            {
                tmp->pro = CN_ARPITEM_PRO_DYNAMIC;
                tmp->timeout = CN_ARPITEM_LIFE_TRIG;
                __SndReq(ippeer,iphost,iface);
            }
        }
        else
        {
            if(tmp->pro &CN_ARPITEM_PRO_STABLE)
            {
                memcpy(macbuf,tmp->mac,CN_MACADDR_LEN);
                tmp->reffers++;
                ret = true;
            }
            else
            {
                __SndReq(ippeer,iphost,iface);  //do an arp request
            }
        }
        mutex_unlock(gArpCB.lock);
    }
    if((false == ret)&&(NULL != tmp))
    {
        //just for the wait
        semp_pendtimeout(tmp->semp,CN_ARP_SYNC_TIME);
        //check once more
        if(mutex_lock(gArpCB.lock))
        {
            if(tmp->pro &CN_ARPITEM_PRO_STABLE)
            {
                memcpy(macbuf,tmp->mac,CN_MACADDR_LEN);
                tmp->reffers++;
                ret = true;
            }
            mutex_unlock(gArpCB.lock);
        }
    }
    return ret;
}



//options usage:
//-i interface
//-a (add a arp item)
//-d (del a arp item/clean all)
//-p ippeer
//-h iphost
typedef enum
{
    EN_ITEM_ACTION_PRINT = 0,
    EN_ITEM_ACTION_ADD,
    EN_ITEM_ACTION_DEL,
    EN_ITEM_ACTION_LAST,
}tagItemAction;

//static bool_t __ArpShell(char *param)
bool_t arp(char *param)
{
    bool_t ret = true;
    tagItemAction action =EN_ITEM_ACTION_PRINT ;
    void *iface = NULL;
    u32 ippeer = 0;
    u32 iphost = 0;
    int i = 0;
    int argc =14;
    const char *argv[14];
    string2arg(&argc,argv,param);
    while(i <argc) //do the parameters decode
    {
        if(0 == strcmp(argv[i],"-a"))
        {
             action = EN_ITEM_ACTION_ADD;
             i = i+1;
        }
        else if(0 == strcmp(argv[i],"-d"))
        {
             action = EN_ITEM_ACTION_DEL;
             i = i+1;
        }
        else if(0 == strcmp(argv[i],"-i"))
        {
             iface = NetDevGet(argv[i+1]);
             i = i+2;
        }
        else if(0 == strcmp(argv[i],"-p"))
        {
             ippeer = inet_addr(argv[i+1]);
             i = i+2;
        }
        else if(0 == strcmp(argv[i],"-h"))
        {
             iphost = inet_addr(argv[i+1]);
             i = i+2;
        }
        else
        {
            i =i+1; //unknown
        }
    }
    switch(action)
    {
        case EN_ITEM_ACTION_ADD:
            if(mutex_lock(gArpCB.lock))
            {
                __ItemCreate(ippeer,iphost,iface);
                mutex_unlock(gArpCB.lock);
            }
            break;
        case EN_ITEM_ACTION_DEL:
            if(ippeer == INADDR_ANY)
            {
                ret = __TabClean();
            }
            else
            {
                if(mutex_lock(gArpCB.lock))
                {
                    ret = __ItemDel(ippeer);
                    mutex_unlock(gArpCB.lock);
                }
            }
            break;
        default:
            ret = __TabShow();
            break;
    }
    return ret;
}


// =============================================================================
// FUNCTION:this is the arp module init function
// PARA  IN:
// PARA OUT:
// RETURN  :true find while false not
// INSTRUCT:
// =============================================================================
bool_t ArpInit(void)
{
    bool_t ret = false;
    gArpCB.lock = mutex_init(NULL);
    if(NULL == gArpCB.lock)
    {
        debug_printf("arp","LOCK CREATE ERR\n\r");
        goto EXIT_ITEMMUTEX;
    }
    gArpCB.hashtab = (tagArpItem **)net_malloc(CFG_ARP_HASHLEN*sizeof(void *));
    if(NULL == gArpCB.hashtab)
    {
        debug_printf("arp","HASHTAB CREATE ERR\n\r");
        goto EXIT_ITEMTAB;
    }
    memset((void *)gArpCB.hashtab,0,CFG_ARP_HASHLEN*sizeof(void *));
    //we now register the arp receive function to the linker to deal arp frames
    LinkPushRegister(EN_LINKPROTO_ARP,__ArpPush);
    //also need to register the ticker to the netticker queue
    NetTickerIsrInstall("ARPTICKER",__ArpTicker,30*1000); //30 SECOND

    return true;
 EXIT_SHELLCMD:
    net_free((void *)gArpCB.hashtab);
    gArpCB.hashtab = NULL;
EXIT_ITEMTAB:
    mutex_del(gArpCB.lock);
    gArpCB.lock = NULL;
EXIT_ITEMMUTEX:
    return ret;
}

ADD_TO_ROUTINE_SHELL(arp,arp,"usage:arp [-a]/[-d] [-i interface] [-p peeraddr] [-h hostaddr]");










