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
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netbsp.h>
#include "dbug.h"
#include <shell.h>
#include "../common/router.h"
#include "../common/netdev.h"
//introduction:
//when send a package, we use the destination to match the rout list,if not a proper rout
//could be found,then check if destination is the host, if it matches the host, then use the
//local route, else use the default rout(if has been set)
//when receive a package, we use the destination to match the host address;if none address is been
//matched,then do the forward(if permitted)

typedef struct
{
    u16 G :1;
    u16 U :1;
    u16 R :1;
    u16 V4 :1;
    u16 S  :1; //static or not
    u16 C:1  ; //clone or not
    u16 B:1;   //broadcast or not
    u16 M:1;   //multicast or not
}tagRoutFlag;

typedef union
{
    tagRoutFlag bits;
    u16         value;
}tagURoutFlag;

typedef struct
{
    tagURoutFlag flag;
    u8 pri;
    u8 len; //most 256 bits
    u16 mtu; //how much bytes could be sent by the rout:default decided by the interface
             //but could be modified by the protocols:such as the icmpv6
}tagRoutPro;

typedef struct
{
    struct in6_addr  host;  //if the source addr is INADDR_ANY,we will use this addr to  replace
    struct in6_addr  net;  //which used to match the destination
    struct in6_addr  hop;  //for the local,if no router exist, then the nxthop is INADDR_ANY,which means no nxthop
}tagRoutV6;
typedef struct
{
    struct in_addr   host;  //if the source addr is INADDR_ANY,we will use this addr to  replace
    struct in_addr   net;   //sequence the queue use the same
    struct in_addr   hop;   //for the local,if no router exist, then the nxthop is INADDR_ANY,which means no nxthop
    struct in_addr   broad; //for the broadcast
    struct in_addr   mask;  //for the mask
}tagRoutV4;
//defines for the rout structure
typedef struct
{
    void      *nxt;           //point to the next rout item
    void      *iface;         //point to the  interface binded
    void *rout;               //which include the rout
    tagRoutPro pro;           //like the unix,we use G U R AND SO ON FLAGS
}tagRoutItem;

#define CN_ERR_ROUTPREDIX     "ROUTMODULE"
#define CN_IP_STRMAX           20
typedef struct
{
    tagRoutItem *v6lst;     //which point to the ipv6 route list
    tagRoutItem *v4lst;     //which point to the ipv4 route list
    tagRoutItem *v4host;    //use the host address here
    tagRoutItem *v6host;    //use the host address here
    mutex_t      lock;      //which used to protect the ipv6 and ipv4 rout list
}tagRoutCB;
static tagRoutCB  gRoutCB;
static bool_t __ShowTab(enum_ipv_t ver)
{
    int i = 0;
    tagRoutItem *item;
    char net[CN_IP_STRMAX];
    char host[CN_IP_STRMAX];
    char hop[CN_IP_STRMAX];
    char mask[CN_IP_STRMAX];
    tagRoutV4 *rout;
    if (mutex_lock(gRoutCB.lock))
    {
        //show the ipv4
        OsPrintSplit('*',100);
        debug_printf("router","%s:IPV4 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        //print the format:NO NET HOST HOP U G R PRI LEN REFIN REFSOUT INTERFACE
        if(ver == EN_IPV_4) //do the ipv4
        {
            debug_printf("router","%2s %-15s %-15s %-15s %-15s %s%s%s %-3s %-3s %-4s %s\n\r",
                    "NO", "NET","MASK", "HOST", "HOP","U", "G", "R", "PRI", "LEN","MTU","IFACE");
            item = gRoutCB.v4lst;
            while (NULL != item)
            {
                rout = item->rout;
                inet_ntop(AF_INET, &rout->net, net, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->host, host, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->hop, hop, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->mask, mask, CN_IP_STRMAX);
                debug_printf("router","%-2d %-15s %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r", \
                        i++, net,mask, host, hop,item->pro.flag.bits.G, \
                        item->pro.flag.bits.R, item->pro.flag.bits.R, item->pro.pri, \
                        item->pro.len,item->pro.mtu,NetDevName(item->iface));
                item = item->nxt;
            }
            if(NULL != gRoutCB.v4host)
            {
                item =  gRoutCB.v4host;
                rout = item->rout;
                inet_ntop(AF_INET, &rout->net, net, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->host, host, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->hop, hop, CN_IP_STRMAX);
                inet_ntop(AF_INET, &rout->mask, mask, CN_IP_STRMAX);
                debug_printf("router","%-2d %-15s %-15s %-15s %-15s %d%d%d %-3d %-3d %-4x %s\n\r", \
                        i++, net,mask, host, hop,item->pro.flag.bits.G, \
                        item->pro.flag.bits.R, item->pro.flag.bits.R, item->pro.pri, \
                        item->pro.len,item->pro.mtu,NetDevName(item->iface));
            }
        }
        else if(ver == EN_IPV_6) //do the ipv6
        {
            debug_printf("router","%s:IPV6 KERNEL ROUT:\n\r", CN_ERR_ROUTPREDIX);
        }
        else //do nothing here
        {

        }
        mutex_unlock(gRoutCB.lock);
        debug_printf("router","%s:ROUT PRINT DONE\n\r", CN_ERR_ROUTPREDIX);
        OsPrintSplit('*',100);
    }
    return true;
}

typedef enum
{
    enRoutOperation_print= 0,
    enRoutOperation_add,
    enRoutOperation_del,
    enRoutOperation_match,
}enRoutOperation;

//usage:rout -a/d/p(action) -v(4/6) -i ifname -n net -h host -g gateway
//static bool_t __RoutShell(char *param)
ADD_TO_IN_SHELL_HELP(rout,"usage:rout");
ADD_TO_IN_SHELL  bool_t rout(char *param)
{
    enum_ipv_t ver = EN_IPV_4;
    enRoutOperation  opt = enRoutOperation_print;
    tagHostAddrV4 v4addr;
    struct in_addr  ipmatchv4;
    const char *ifname;
    tagRouterPara para;
    int argc = 12;
    const char *argv[12];
    int i = 0;
    //del or add operation
    memset(&para,0,sizeof(para));
    memset(&v4addr,0,sizeof(v4addr));
    string2arg(&argc,argv,param);
    //now deal all the parameters here
    i = 0;
    while(i < argc)
    {
        if(0 == strcmp(argv[i],"-a")) //which means add a rout
        {
            opt = enRoutOperation_add;
            i++;
        }
        else if(0 == strcmp(argv[i],"-d"))
        {
            opt = enRoutOperation_del;
            i++;
        }
        else if(0 == strcmp(argv[i],"-p"))
        {
            opt = enRoutOperation_print;
            i++;
        }
        else if(0 == strcmp(argv[i],"-v4"))
        {
            ver = EN_IPV_4;
            i++;
        }
        else if(0 == strcmp(argv[i],"-v6"))
        {
            ver = EN_IPV_6;
            i++;
        }
        else if(0 == strcmp(argv[i],"-match")) //do match
        {
            opt = enRoutOperation_match;
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&ipmatchv4);
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-p")) //do prior
        {
            para.prior = (u8)strtol(argv[i+1],NULL,0);
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-i"))
        {
            ifname = argv[i+1];
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-n"))  //net
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.net);
                para.net =&v4addr.net;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-m"))  //mask
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.mask);
                para.mask =&v4addr.mask;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-h")) //host
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.host);
                para.host =&v4addr.host;
            }
            i += 2;
        }
        else if(0 == strcmp(argv[i],"-g")) //gateway
        {
            if(ver == EN_IPV_4)
            {
                inet_pton(AF_INET,argv[i+1],&v4addr.hop);
                para.hop =&v4addr.hop;
            }
            i += 2;
        }

        else
        {
            i++; //default not support yet
        }
    }
    //now do the operation now
    switch(opt)
    {
        case enRoutOperation_print:
            __ShowTab(ver);
            break;
        case enRoutOperation_match:
            if(ver == EN_IPV_4)
            {
                tagRoutLink  rout;
                memset(&rout,0,sizeof(rout));
                rout.ver = EN_IPV_4;
                rout.dst = &ipmatchv4;
                rout.host = &v4addr.host;
                rout.hop = &v4addr.hop;
                if(RouterMatch(&rout))
                {
                    debug_printf("router","host :%s \n\r",inet_ntoa(*(struct in_addr*)rout.host));
                    debug_printf("router","hop  :%s \n\r",inet_ntoa(*(struct in_addr*)rout.hop));
                    debug_printf("router","iface:%s \n\r",NetDevName(rout.iface));
                }
                else
                {
                    debug_printf("router","COULD NOT MATCH\n\r");
                }
            }
            break;
        case enRoutOperation_del:
            para.ver = ver;
            para.ifname = ifname;
            RouterRemove(&para);
            break;
        case enRoutOperation_add:
            para.ver = ver;
            para.ifname = ifname;
            RouterCreate(&para);
            break;
        default: //not support yet
            break;
    }

    return true;
}


//this is the router controller
bool_t RouterInit(void)
{
    bool_t ret = false;
    memset(&gRoutCB,0,sizeof(gRoutCB));
    gRoutCB.lock = mutex_init(NULL);
    if(NULL == gRoutCB.lock)
    {
        debug_printf("router","%s:LOCK CREATED FOR ROUTER FAILED\n\r", CN_ERR_ROUTPREDIX);
    }
    else
    {

            return (TRUE);
    }
    return ret;
}
//check how many ones in the net mask
static u8 __CheckOnes(u32 value)
{
    u8 ret = 0;
    int i = 0;
    for(i = 0;i<32;i++)
    {
        if((1<<i)&value)
        {
            ret ++;
        }
    }
    return ret;
}
//we will match the destination
//you also could use this function to do the address type check
bool_t RouterMatch(tagRoutLink *para)
{
    bool_t ret = false;
    tagRoutItem *tmp = NULL;
    tagRoutV4   *v4;
    struct in_addr addr;
    if((NULL == para)||(NULL == para->dst))
    {
        return ret;
    }
    if(mutex_lock(gRoutCB.lock))
    {
        ret = true;
        para->type = EN_IPTYPE_V4_UNKOWN;
        if(para->ver ==EN_IPV_4)
        {
            memcpy(&addr.s_addr,para->dst,sizeof(addr));
            if(addr.s_addr == INADDR_ANY) //use the host
            {
                para->type = EN_IPTYPE_V4_ANY;
            }
            if(addr.s_addr == INADDR_LOOPBACK) //use the host
            {
                para->type = EN_IPTYPE_V4_HOST;
                tmp = gRoutCB.v4host;
                if(NULL != tmp)
                {
                    v4 = tmp->rout;
                    para->iface = tmp->iface;
                    if(NULL != para->host)
                    {
                        memcpy(para->host,&v4->host.s_addr,sizeof(struct in_addr));
                    }
                    if(NULL != para->hop)
                    {
                        memcpy(para->hop,&v4->hop.s_addr,sizeof(struct in_addr));
                    }
                }
            }
            else  //do the find
            {
                tmp = gRoutCB.v4lst;
                while(NULL != tmp)
                {
                    v4 = tmp->rout;
                    if(v4->host.s_addr == addr.s_addr)
                    {
                        para->type = EN_IPTYPE_V4_LOCAL;
                        tmp = gRoutCB.v4host;
                        break;
                    }
                    else if (v4->broad.s_addr == addr.s_addr)
                    {
                        para->type = EN_IPTYPE_V4_BROAD;
                        break;
                    }
                    else if(v4->net.s_addr == (addr.s_addr&v4->mask.s_addr))
                    {
                        if(addr.s_addr != INADDR_ANY)
                        {
                            para->type = EN_IPTYPE_V4_SUBNET;
                        }
                        break;
                    }
                    else
                    {
                        tmp = tmp->nxt;
                    }
                }
                if(NULL != tmp)
                {
                    v4 = tmp->rout;
                    para->iface = tmp->iface;
                    if(NULL != para->host)
                    {
                        memcpy(para->host,&v4->host.s_addr,sizeof(struct in_addr));
                    }
                    if(NULL != para->hop)
                    {
                        memcpy(para->hop,&v4->hop.s_addr,sizeof(struct in_addr));
                    }
                }
            }
        }
        mutex_unlock(gRoutCB.lock);
    }
    return ret;
}


//we find all the matched item in the table,storage the result in the array
//return how many item found, and storage the result in the array
//if handle is NULL,then NOT check the handle
//if net is NULL,then NOT check the net:if not must check the bits specified
//if local is NULL,then NOT check the local
static int __ItemFind(enum_ipv_t ver, void *iface, void *net, void *host, void *hop, tagRoutItem *array[], int len)
{
    int ret = 0;
    tagRoutItem *tmp = NULL;
    tagRoutV4 *v4;
    tagRoutV6 *v6;

    if (ver == EN_IPV_4)
    {
        tmp = gRoutCB.v4lst;
        while ((NULL != tmp) && (ret < len))
        {
            v4 = tmp->rout;
            if ((NULL != iface) && (iface != tmp->iface))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != net) && (0 != memcmp((void *) &v4->net, net, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != host) && (0 != memcmp((void *) &v4->host, host, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            if ((NULL != hop) && (0 != memcmp((void *) &v4->hop, hop, sizeof(struct in_addr))))
            {
                goto V4_CHECKMORE;
            }
            //OK this is the one we get
            array[ret] = tmp;
            ret++;
            //and check more
            V4_CHECKMORE:
            tmp = tmp->nxt;
        }
    }
    else if (ver == EN_IPV_6)
    {
        tmp = gRoutCB.v6lst;
        while ((NULL != tmp) && (ret < len))
        {
            v6 = tmp->rout;
            if ((NULL != iface) && (iface != tmp->iface))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != net) && (0 != memcmp((void *) &v6->net, net, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != host) && (0 != memcmp((void *) &v6->host, host, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            if ((NULL != hop) && (0 != memcmp((void *) &v6->hop, hop, sizeof(struct in6_addr))))
            {
                goto V6_CHECKMORE;
            }
            //OK this is the one we get
            array[ret] = tmp;
            ret++;
            V6_CHECKMORE:
            tmp = tmp->nxt;
        }
    }
    else
    {
        //NOT SUPPORT YET
    }

    return ret;
}


//compare the two rout sequence in the queue when add it to the queue
//rules:first prior, then mask length,else the refers times
//return:>0 which means a should be before b
//       =0 which means a should be after b
static int  __SeqCompare(tagRoutItem *a,tagRoutItem *b)
{
    int ret = 1;
    if(a->pro.pri > b->pro.pri)
    {
        ret =1;
    }
    else if(a->pro.pri == b->pro.pri) //same prior
    {
        if(a->pro.len >= b->pro.len)
        {
            ret = 1;
        }
        else
        {
            ret = 0;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}
//add a route to the queue,return the queue head:use the compare function to compare
static tagRoutItem *__Add2Queue(tagRoutItem *queue, tagRoutItem *item) //--TODO
{
    tagRoutItem *tmp;
    tagRoutItem *nxt;
    if (NULL == queue) //if the queue is empty now
    {
        queue = item;
        return queue;
    }
    //if the item should be inserted into the header
    if (__SeqCompare(item,queue))
    {
        item->nxt = queue;
        queue = item;
        return queue;
    }
    //then select a proper place to insert the item
    tmp = queue;
    while (NULL != tmp)
    {
        if (NULL == tmp->nxt) //the tmp is the last one
        {
            tmp->nxt = item;
            break;
        }
        else
        {
            nxt = tmp->nxt;
            if (__SeqCompare(item,nxt)) //insert it between the tmp and nxt
            {
                tmp->nxt = item;
                item->nxt = nxt;
                break;
            }
            else //do the continue loop
            {
                tmp = nxt;
            }
        }
    }
    return queue;
}
//return the new queue header
static tagRoutItem * __RemoveFromQueue(tagRoutItem *queue, tagRoutItem *item) //--TODO
{
    tagRoutItem *tmp;
    //queue is empty now
    if (NULL == queue)
    {
        return queue;
    }
    //if the item is the head
    if (item == queue)
    {
        queue = queue->nxt;
        item->nxt = NULL;
    }
    //now do the loop,this operation will not change the queue head
    tmp = queue;
    while (tmp != NULL)
    {
        if (item == tmp->nxt) //find it here
        {
            tmp->nxt = item->nxt;
            item->nxt = NULL;
            break;
        }
        tmp = tmp->nxt; //for the loop
    }
    return queue;
}

//when enable the loop.then call it
void RouterSetHost(enum_ipv_t ver,const char *ifname)
{
    tagRoutItem *newitem = NULL;
    void *iface;
    int memsize;
    tagRoutV4 *v4;
    iface = NetDevGet(ifname);
    if(iface == NULL) //no interface for this rout,so return NULL.
    {
        return ;
    }
    //first we should make a new one:supposed that you could create one
    //we do it here for we will not do the block following
    memsize = (ver == EN_IPV_4) ? sizeof(tagRoutV4) : sizeof(tagRoutV6);
    memsize += sizeof(tagRoutItem);
    newitem = net_malloc(memsize);
    if (NULL == newitem)
    {
        return;
    }
    memset(newitem, 0, memsize);
    newitem->rout = sizeof(tagRoutItem) + (u8 *) newitem;
    //USED TO SEE IF ANY ROUT USE THE SAME ROUTE
    if (mutex_lock(gRoutCB.lock))
    {
        if(ver == EN_IPV_4)
        {
            if(NULL != gRoutCB.v4host)
            {
                net_free(newitem);
            }
            else
            {
                newitem->pro.flag.bits.V4 = 1;
                newitem->pro.len = 32;
                newitem->pro.pri = CN_ROUT_PRIOR_LOOP;
                newitem->pro.flag.value = 0;
                newitem->pro.mtu = NetDevMtu(iface);
                newitem->nxt = NULL;
                newitem->iface = iface;
                v4 = newitem->rout;
                v4->net.s_addr = INADDR_LOOPBACK;
                v4->host.s_addr = INADDR_LOOPBACK;
                v4->mask.s_addr = INADDR_BROAD;
                gRoutCB.v4host = newitem;
            }
        }
        else //not implement yet --IPV6
        {
            net_free(newitem);
        }
        mutex_unlock(gRoutCB.lock);
    }
    return;
}
//add a route to the hosts
void *RouterCreate(tagRouterPara *para)
{
    void *ret = NULL;
    tagRoutItem *tmp = NULL;
    tagRoutItem *newitem = NULL;
    void *iface;
    int memsize;
    tagRoutV4 *v4;
    if((para == NULL)||(NULL == para->ifname)||(NULL==para->mask)||\
       (NULL==para->host)||(NULL==para->net)) //parameters is not enough
    {
        return ret;
    }
    iface = NetDevGet( para->ifname);
    if(iface == NULL) //no interface for this rout,so return NULL.
    {
        return ret;
    }
    //first we should make a new one:supposed that you could create one
    //we do it here for we will not do the block following
    memsize = (para->ver == EN_IPV_4) ? sizeof(tagRoutV4) : sizeof(tagRoutV6);
    memsize += sizeof(tagRoutItem);
    newitem = net_malloc(memsize);
    if (NULL == newitem)
    {
        return ret;
    }
    memset(newitem, 0, memsize);
    newitem->rout = sizeof(tagRoutItem) + (u8 *) newitem;
    //USED TO SEE IF ANY ROUT USE THE SAME ROUTE
    if (mutex_lock(gRoutCB.lock))
    {
        tmp = NULL;
        __ItemFind(para->ver, iface, para->net, para->host, para->hop,&tmp,1);
        if (tmp) //not NULL,which means has get the same rout item,so could not add it anymore
        {
            debug_printf("router","%s:ROUT EXSITED\n\r", CN_ERR_ROUTPREDIX);
            net_free(newitem);
        }
        else
        {
            //so initialize the new one and  add it to the queue
            newitem->pro.pri = para->prior;
            newitem->pro.flag.value = para->flags;
            if(para->mtu == 0) //use the interfa
            {
                newitem->pro.mtu=NetDevMtu(iface);
            }
            else
            {
                newitem->pro.mtu = para->mtu;
            }
            newitem->nxt = NULL;
            newitem->iface = iface;
            if (para->ver == EN_IPV_4)
            {
                v4 = newitem->rout;
                memcpy((void *) &v4->net, para->net, sizeof(struct in_addr));
                memcpy((void *) &v4->host, para->host, sizeof(struct in_addr));
                memcpy((void *) &v4->mask, para->mask, sizeof(struct in_addr));
                if(NULL != para->hop)
                {
                    memcpy((void *) &v4->hop, para->hop, sizeof(struct in_addr));
                }
                //compute the broad address
                if(NULL != para->broad)
                {
                    memcpy((void *) &v4->broad, para->broad, sizeof(struct in_addr));
                }
                newitem->pro.len = __CheckOnes(v4->mask.s_addr);
                newitem->pro.flag.bits.V4 = 1;
                gRoutCB.v4lst = __Add2Queue(gRoutCB.v4lst, newitem);
            }
            else
            {
                //not implement yet --TODO
            }
            ret = newitem;
        }
        mutex_unlock(gRoutCB.lock);
    }
    return ret;
}

//such as you added the rout and storage the result,then you could use it to remove
void RouterRemoveByHandle(void *rout)
{
    tagRoutItem *item;
    if (NULL == rout)
    {
        return;
    }
    if (mutex_lock(gRoutCB.lock))
    {
        item = rout;
        if (item->pro.flag.bits.V4)
        {
            gRoutCB.v4lst = __RemoveFromQueue(gRoutCB.v4lst, item);
        }
        else
        {
            gRoutCB.v6lst = __RemoveFromQueue(gRoutCB.v6lst, item);
        }
        mutex_unlock(gRoutCB.lock);
    }
    return;
}

//delete the rout item filtered by if name ,net ,host ,hop
//delete the rout,such as you get a new ip,then you will delete all the old binded to the old host address
void   RouterRemove(tagRouterPara *para)
{
    int ret;
    tagRoutItem *item;

    void *iface = NULL;
    iface = NetDevGet(para->ifname);
    if (mutex_lock(gRoutCB.lock))
    {
        if (para->ver == EN_IPV_4) //deal the ipv4
        {
            do
            {
                ret = __ItemFind(EN_IPV_4, iface, para->net, para->host, para->hop, &item, 1);
                gRoutCB.v4lst = __RemoveFromQueue(gRoutCB.v4lst, item);
            }
            while (ret > 0); //delete all the rout(V4) binded to the interface
        }
        else
        {
            //not implement yet
        }
        mutex_unlock(gRoutCB.lock);
    }
    return;
}
//this function has been abandoned, but must do the compatible
bool_t RoutCreate(const char *ifname,enum_ipv_t ver,void *netaddr,u32 pro)
{
    bool_t     ret = false;
    if(ver != EN_IPV_4)
    {
        return ret;
    }
    ret = true;
    //add this one here
    tagHostAddrV4 v4;
    tagRouterPara para;
    memset(&para,0,sizeof(para));
    memcpy(&v4,netaddr,sizeof(v4));
    v4.gatway = INADDR_ANY;
    v4.subnet = v4.ip&v4.submask;
    para.ver = ver;
    para.host = &v4.ip;
    para.mask = &v4.submask;
    para.broad = &v4.broad;
    para.hop = &v4.gatway;
    para.net = &v4.subnet;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.ifname = ifname;
    RouterRemove(&para);
    RouterCreate(&para);
    return ret;
}














