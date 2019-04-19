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
#include <stdlib.h>
#include <sys/socket.h>
#include <osarch.h>
#include <netbsp.h>

#include "dhcpLib.h"
#include "dbug.h"
#include "../common/router.h"

typedef struct
{
    void                      *nxt;            //the task lst, for the extend fuction
    u32                        stat;           //the task stat
    u32                        timeout;        //when timeout,do the
    u32                        transID;        //trans action ID
    u8                         mac[CN_MACADDR_LEN]; //the rout mac
//    tagDhcpReplyPara           replypara;      //used to record the reply
    const char                *ifname;         //dev name
    struct RoutItem4          *routwan;       //if get the gateway,create one here
    struct RoutItem4          *routlan;       //for the local router
    u32                        offerip;
    u32                        offerserver;
}tagTaskItem;
typedef enum
{
    EN_CLIENT_DICOVER,  //which means the task has snd the discover request
    EN_CLIENT_REQUEST,  //which means the task has recieve the offer and snd the request
    EN_CLIENT_STABLE,   //which means the task has recieve the ack of the request from the server
    EN_CLIENT_INFORM,   //which means the task has recieve the ack of the request from the server
}enDhcpClientStat;

#define CN_DHCP_TICKER_CYCLE 100    //100ms run once
#define CN_DHCP_TIMEOUT      0x10   //if not reply in the time, then will snd again
#define CN_DHCP_CLIENTTXID   0x10000000
//rebuild the dhcp client task
typedef struct
{
    mutex_t         lock; //clien list lock
    tagTaskItem    *lst;  //client list here
    u32             txid;
    tagDhcpMsg      msg;
    int             sockfd; //the client sock here
    void           *ticker; //dhcp ticks here
}tagDhcpClientCB;
static tagDhcpClientCB gClientCB;
//do the reply message deal
static bool_t __cpyReplyMsg(tagDhcpMsg *msg)
{
    u32              txid;
    tagTaskItem     *tmp;
    bool_t           result = false;
    tagRouterPara    routpara;
    tagHostAddrV4    addr;
    tagDhcpReplyPara reply;
    if(msg->op != DHCP_BOOTREPLY)
    {
        return result;
    }
    memset(&reply,0,sizeof(reply));
    pasteDhcpReplyMsg(&reply,msg);

    txid = ntohl(msg->xid);
    tmp = gClientCB.lst;
    while(NULL != tmp)
    {
        if((tmp->transID == txid)&&(0 == memcmp(reply.clientmac,tmp->mac,CN_MACADDR_LEN)))
        {
            tmp->offerip = reply.offerip;
            tmp->offerserver = reply.dhcpserver;
            if(reply.msgtype == DHCP_OFFER)
            {
                tmp->stat = EN_CLIENT_REQUEST;
                tmp->timeout = 0;
            }
            else if(reply.msgtype == DHCP_ACK)
            {
                tmp->stat = EN_CLIENT_INFORM;
                tmp->timeout = reply.leasetime;
                //here we also to update the rout info
                //we get the new router,so must del the old one
                if(tmp->routlan)
                {
                    RouterRemoveByHandle(tmp->routlan);
                }
                if(tmp->routwan)
                {
                    RouterRemoveByHandle(tmp->routwan);
                }
                //NOW SET THE DNS HERE
                //OK,NOW CREATE THE WAN ROUT
                memset(&addr,0,sizeof(addr));
                memset(&routpara,0,sizeof(routpara));
                addr.host = reply.offerip;
                addr.hop = reply.router;
                addr.mask = INADDR_ANY;
                addr.net = INADDR_ANY;//could match any one
                addr.broad = INADDR_BROADCAST;
                routpara.ifname = tmp->ifname;
                routpara.ver = EN_IPV_4;
                routpara.host = &addr.host;
                routpara.net = &addr.net;
                routpara.mask = &addr.mask;
                routpara.broad =&addr.broad;
                routpara.hop = &addr.hop;
                routpara.prior = CN_ROUT_PRIOR_ANY;
                tmp->routwan = RouterCreate(&routpara);
                //CREATE THE LOCAL ROUT
                memset(&addr,0,sizeof(addr));
                addr.host = reply.offerip;
                addr.mask = reply.ipmask;
                addr.net = addr.host &addr.mask;
                addr.broad = (~(addr.mask))|addr.net;
                routpara.ifname = tmp->ifname;
                routpara.ver = EN_IPV_4;
                routpara.host = &addr.host;
                routpara.net = &addr.net;
                routpara.mask = &addr.mask;
                routpara.broad =&addr.broad;
                routpara.hop = NULL;
                routpara.prior = CN_ROUT_PRIOR_UNI;
                tmp->routlan = RouterCreate(&routpara);
                DnsSet(EN_IPV_4,&reply.dns1,&reply.dns2);
                NetDevPostEvent(NetDevGet(tmp->ifname),EN_NETDEVEVENT_IPGET);
            }
            else if(reply.msgtype == DHCP_NAK)
            {
                tmp->stat = EN_CLIENT_DICOVER;
                tmp->timeout = 0;
            }
            break;
        }
        else
        {
            tmp = tmp->nxt;
        }
    }

    return result;
}

static void __dealTask(void)
{
    tagTaskItem      *task;

    tagDhcpRequestPara  reqpara;

    task = gClientCB.lst;
    while(NULL != task)
    {
        if(task->timeout == 0)
        {
            if(task->stat == EN_CLIENT_DICOVER)
            {
                memcpy(reqpara.clientmac,task->mac,CN_MACADDR_LEN);
                reqpara.optype = DHCP_BOOTREQUEST;
                reqpara.msgtype = DHCP_DISCOVER;
                reqpara.transaction = task->transID;
                reqpara.reqip = INADDR_ANY;

                makeDhcpRequestMsg(&gClientCB.msg,&reqpara);
                send(gClientCB.sockfd,(void *)&gClientCB.msg,sizeof(gClientCB.msg),0);
                task->timeout = CN_DHCP_TIMEOUT;
            }
            else if(task->stat == EN_CLIENT_REQUEST)
            {
                memcpy(reqpara.clientmac,task->mac,CN_MACADDR_LEN);
                reqpara.optype = DHCP_BOOTREQUEST;
                reqpara.msgtype = DHCP_REQUEST;
                reqpara.transaction = task->transID;
                reqpara.reqip = task->offerip;
                reqpara.dhcpserver = task->offerserver;

                makeDhcpRequestMsg(&gClientCB.msg,&reqpara);
                send(gClientCB.sockfd,(void *)&gClientCB.msg,sizeof(gClientCB.msg),0);
                task->timeout = CN_DHCP_TIMEOUT;
            }
            else
            {
                //maybe do the inform here,but we do it as discover
                task->stat = EN_CLIENT_DICOVER;
                task->transID = gClientCB.txid++;
                task->timeout = 0;
            }
            memset((void *)&gClientCB.msg,0,sizeof(gClientCB.msg));
        }
        else
        {
            task->timeout--;
        }
        task = task->nxt;
    }
    return;
}

//dhcpclient ticker
static void __DhcpclientTicker(void)
{
    int               recvlen =0;
    if(mutex_lock(gClientCB.lock))
    {
        do{
            recvlen = recv(gClientCB.sockfd, (void *)&gClientCB.msg, sizeof(gClientCB.msg), 0);
            if(recvlen >0) //if any message here
            {
                __cpyReplyMsg(&gClientCB.msg);
            }
        }while(recvlen > 0);
        __dealTask();//check all the task,if any work need to do
        mutex_unlock(gClientCB.lock);
    }

    return ;
}

//if you want to use the interface to get a ipv4 dynamic, then call this function
bool_t DhcpAddClientTask(const char *ifname)
{
    bool_t ret = false;
    tagTaskItem *task;
    tagHostAddrV4 addr;
    tagRouterPara  routpara; //use this to make the router
    //first we will do some thing here
    memset(&addr,0,sizeof(addr));
    addr.broad = INADDR_BROADCAST;
    //prepare the parameters
    memset(&routpara,0,sizeof(routpara));
    routpara.ifname = ifname;
    routpara.ver = EN_IPV_4;
    routpara.host = &addr.host;
    routpara.net = &addr.net;
    routpara.mask = &addr.mask;
    routpara.broad = &addr.broad;
    routpara.hop = &addr.hop;
    routpara.prior = CN_ROUT_PRIOR_ANY;
    task = net_malloc(sizeof(tagTaskItem));
    if(NULL == task) //no mem here
    {
        error_printf("dhcp","%s:MEMERR\n\r",__FUNCTION__);
        goto EXIT_MEM;
    }
    memset((void *)task, 0, sizeof(tagTaskItem));
    task->stat = EN_CLIENT_DICOVER;
    task->timeout = 0;
    task->ifname = ifname;
    memcpy(task->mac,NetDevGetMac(NetDevGet(ifname)),CN_MACADDR_LEN);
    task->transID = gClientCB.txid++;
    task->routwan = RouterCreate(&routpara); //has create one
    if(NULL == task->routwan)
    {
        error_printf("dhcp","%s:ROUTWAN\n\r",__FUNCTION__);
        goto EXIT_ROUTWAN;
    }
    //OK,NOW ADD IT TO THE QUEUE
    if(mutex_lock(gClientCB.lock))
    {
        task->nxt = gClientCB.lst;
        gClientCB.lst = task;
        mutex_unlock(gClientCB.lock);
    }
    else
    {
        error_printf("dhcp","%s:QUEUE ERR\n\r",__FUNCTION__);
        goto EXIT_QUEERR;
    }
    ret = true;
    return ret;

EXIT_QUEERR:
    RouterRemoveByHandle(task->routwan);
    task->routwan = NULL;
EXIT_ROUTWAN:
    net_free(task);
EXIT_MEM:
    return ret;
}

//the first time to get the ip from the server is init->discover->request->stable
//then if you want to extend the lease time, then:request,if receive ack, then goto
//the stable, if nack then goto discover and so the whole flow again
//do the client socket int ,after this ,we could read and write the client socket,
//which means we could do the client socket communicate; this func returns the
//socket of the client
static int __sockfdinit(void)
{
    int ret = -1;
    struct sockaddr_in ipportaddr;

    ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //USE THE DEFAULE UDP PROGRAM
    if(-1 == ret)
    {
        closesocket(ret);
        goto ERR_SOCKET;
    }
    ipportaddr.sin_family = AF_INET;
    ipportaddr.sin_port = htons(DHCP_CLIENT_PORT);
    ipportaddr.sin_addr.s_addr = INADDR_ANY;
    if(-1 == bind(ret, (struct sockaddr *)&ipportaddr, sizeof(ipportaddr)))
    {
        closesocket(ret);
        goto ERR_SOCKET;
    }

    ipportaddr.sin_family = AF_INET;
    ipportaddr.sin_port = htons(DHCP_SERVER_PORT);
    ipportaddr.sin_addr.s_addr = INADDR_BROAD;
    if(-1 == connect(ret,(struct sockaddr *)&ipportaddr,sizeof(ipportaddr)))
    {
        closesocket(ret);
        goto ERR_SOCKET;
    }
    int opt = 1;
    if(0 != setsockopt(ret,SOL_SOCKET,SO_NOBLOCK,&opt, sizeof(opt)))
    {
        closesocket(ret);
        goto ERR_SOCKET;
    }

    opt = 1;
    if(0 != setsockopt(ret,SOL_SOCKET,SO_BROADCAST,&opt, sizeof(opt)))
    {
        closesocket(ret);
        goto ERR_SOCKET;
    }
    return ret;

ERR_SOCKET:
    ret = -1;
    return ret;
}
//this is main dhcp client module
static bool_t  DhcpClientInit(void)
{
    bool_t  ret = false;

    memset(&gClientCB,0,sizeof(gClientCB));
    gClientCB.txid = CN_DHCP_CLIENTTXID;
    gClientCB.lock = mutex_init(NULL);
    if(NULL == gClientCB.lock)
    {
        error_printf("dhcp","%s:LOCL CREATE FAILED\n\r",__FUNCTION__);
        goto EXIT_LOCK;
    }
    gClientCB.ticker = NetTickerIsrInstall("DHCPCLIENT",__DhcpclientTicker,\
                                            CN_DHCP_TICKER_CYCLE);
    if(NULL == gClientCB.ticker)
    {
        error_printf("dhcp","%s:TICKER CREATE FAILED\n\r",__FUNCTION__);
        goto EXIT_TICKER;
    }
    gClientCB.sockfd = __sockfdinit();
    if(-1 == gClientCB.sockfd)
    {
        error_printf("dhcp","SOCKFD INITIALIZE FAILED");
        goto EXIT_SOCKFD;
    }
    ret = true;
    return ret;
EXIT_SOCKFD:
    NetTickerIsrRemove(gClientCB.ticker);
    gClientCB.ticker = NULL;
EXIT_TICKER:
    mutex_del(gClientCB.lock);
    gClientCB.lock = NULL;
EXIT_LOCK:
    return ret;
}

//this is the dhcp entry
bool_t ServiceDhcpcInit(void)
{
    bool_t result;
    result = DhcpClientInit();
    return result;
}





