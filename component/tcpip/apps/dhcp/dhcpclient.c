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
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/dhcp.h>
#include <osarch.h>
#include <netbsp.h>

#include "dhcpLib.h"
#include "dbug.h"
#include "../../common/router.h"

typedef struct __tagTaskItem
{
    struct __tagTaskItem      *nxt;            //the task lst, for the extend fuction
    u32                        stat;           //the task stat
    u32                        timeout;        //when timeout,do the
    u32                        transID;        //trans action ID
    u8                         mac[CN_MACADDR_LEN]; //the rout mac
//    tagDhcpReplyPara           replypara;      //used to record the reply
    const char                *ifname;         //dev name
//  struct RoutItem4          *routwan;       //if get the gateway,create one here
    struct RoutItem4          *routlan;       //for the local router
    u32                        offerip;
    u32                        offerserver;
    s32                        is_exist_ip;     //IP Got previous.
    s32                        try_cnts;
    struct SemaphoreLCB*       sem; //notify dhcp get ip successfully
}tagTaskItem;
typedef enum
{
    EN_CLIENT_DICOVER,  //which means the task has snd the discover request�ͻ����Ѿ��Ѿ������˷���IP����
    EN_CLIENT_REQUEST,  //which means the task has recieve the offer and snd the request�ͻ����Ѿ��Ѿ����������յ���������offer��
    EN_CLIENT_STABLE,   //which means the task has recieve the ack of the request from the server�ͻ����Ѿ��յ���������Ӧ��
    EN_CLIENT_INFORM,   //�ͻ����Ѿ���ȷ�յ������������ipv4��ַ��
}enDhcpClientStat;

#define CN_DHCP_TICKER_CYCLE 100    //100ms run once
#define CN_DHCP_TIMEOUT      10     //CN_DHCP_TICKER_CYCLE * CN_DHCP_TIMEOUT if not reply in the time, then will snd again
#define CN_DHCP_CLIENTTXID   0x10000000
//rebuild the dhcp client task
typedef struct
{
    struct MutexLCB *lock; //clien list lock
    tagTaskItem    *lst;  //client list here
    u32             txid;
    tagDhcpMsg      msg;
    s32             sockfd; //the client sock here
    void           *ticker; //dhcp ticks here
}tagDhcpClientCB;
static tagDhcpClientCB gClientCB;

enDHCPStatus DHCP_ConnetStatus(enDHCPCmd cmd,enDHCPStatus status)
{
    static enDHCPStatus flag = EN_DHCP_DISCONNET_STATUS;
    enDHCPStatus temp_flag;
    switch(cmd)
    {
        case EN_DHCP_SET_STATUS_CMD:
            if(status>=EN_DHCP_LAST_STATUS)
            {
                temp_flag = EN_DHCP_CMD_ERR_STATUS;
                break;
            }
            flag = status;
            break;
        case EN_DHCP_GET_STATUS_CMD:
            temp_flag = flag;
            break;
        default:
            break;
    }
    return temp_flag;
}
//DHCP_WaitDhcpDone
//ifname: interface name of net card
//timeout: microsecond
s32 DHCP_WaitDhcpDone(char *ifname, u32 timeout)
{
    s32 ret = 0;
    tagTaskItem     *tmp;

    s64 timemark = DJY_GetSysTime(); //microsecond
    u32 spend=0;
    while (Lock_MutexPend(gClientCB.lock,CN_TIMEOUT_FOREVER))
    {
        tmp = gClientCB.lst;
        while(NULL != tmp) {
            if(strcmp(tmp->ifname,  ifname) == 0) {
                Lock_MutexPost(gClientCB.lock);
                if (tmp->sem) {
                    ret = Lock_SempPend(tmp->sem, timeout-spend);
                }
                return ret;
            }
            else {
                tmp = tmp->nxt;
            }
        }
        Lock_MutexPost(gClientCB.lock);
        spend = DJY_GetSysTime()-timemark;
        if (spend > timeout) {
            break;
        }
        else {
            DJY_EventDelay(300*1000); //re-schedule
        }
    }

    return ret;
}

//------------------------------------------------------------------------------
//���ܣ������յ���DHCPӦ���
//������msg�����յ��� DHCP Ӧ�����Ϣ
//���أ�true = �ɹ�����
//-----------------------------------------------------------------------------
s32 __NetDev_DHCP_GotIP(struct NetDev *pNetDev, u32 ip_temp);
static bool_t __DHCP_CpyReplyMsg(tagDhcpMsg *msg)
{
    u32              renew_counter;
    u32              txid;
    tagTaskItem     *tmp;
    bool_t           result = false;
    tagRouterPara    para;
    u32 hop,subnet,ip,submask,dns,dnsbak;
    tagDhcpReplyPara reply;
    struct NetDev *NetDev;

    if(msg->op != DHCP_BOOTREPLY)
    {
        return result;
    }
    memset(&reply,0,sizeof(reply));
    DHCP_PasteDhcpReplyMsg(&reply,msg);

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
                renew_counter = reply.leasetime*10;//reply.leasetime: seconds, timer 100ms everytime
                tmp->timeout = renew_counter>>1; //renew_counter must before timeout
                //here we also to update the rout info
                //we get the new router,so must del the old one
                if(tmp->routlan)
                {
                    RouterRemoveByHandle(tmp->routlan);
                }
                NetDev = NetDev_GetHandle(tmp->ifname);

                memset(&para,0,sizeof(para));

                ip      = reply.offerip;
                submask = reply.ipmask;
                hop     = reply.router;
                dns     = reply.dns1;
                dnsbak  = reply.dns2;
                subnet = ip & submask;

                para.ver = EN_IPV_4;
                para.ifname = tmp->ifname;
                para.mask = &submask;
                para.net = &subnet;
                para.host = &ip;
//              para.hop = &hop;
//              para.dns = &dns;
//              para.dnsbak = &dnsbak;
                para.prior = CN_ROUT_PRIOR_UNI;
                para.flags = 0;

                NetDev_SetDns(EN_IPV_4,NetDev, &dns, &dnsbak);
                NetDev_SetGateway(EN_IPV_4,NetDev, &hop);
                tmp->routlan = RouterCreate(&para);
                NetDev_PostEvent(NetDev_GetHandle(tmp->ifname),EN_NETDEVEVENT_IPGET);
                DHCP_ConnetStatus(EN_DHCP_SET_STATUS_CMD,EN_DHCP_CONNET_STATUS);
                if(tmp->sem) {
                    Lock_SempPost(tmp->sem);
                }
                __NetDev_DHCP_GotIP(NetDev_GetHandle(tmp->ifname), reply.offerip);
                tmp->is_exist_ip = 0;
                result = true;
            }
            else if(reply.msgtype == DHCP_NAK)
            {
                tmp->stat = EN_CLIENT_DICOVER;
                tmp->is_exist_ip = 0;
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

static void __DHCP_DealTask(void)
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

                DHCP_MakeDhcpRequestMsg(&gClientCB.msg,&reqpara);
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
                if (task->is_exist_ip) {
                    reqpara.dhcpserver = 0;
                }
                else {
                    reqpara.dhcpserver = task->offerserver;
                }

                DHCP_MakeDhcpRequestMsg(&gClientCB.msg,&reqpara);
                send(gClientCB.sockfd,(void *)&gClientCB.msg,sizeof(gClientCB.msg),0);
                task->timeout = CN_DHCP_TIMEOUT;
                if(task->try_cnts++ > 8) {//��ʱ��λ��ͷ����,8��û��Ӧ,������������̻������ܿ�ź���
                    printf("==warning: EN_CLIENT_REQUEST, timeout, begin from EN_CLIENT_DICOVER==!!!\r\n");
                    task->stat = EN_CLIENT_DICOVER;
                    task->transID = gClientCB.txid++;
                    task->timeout = 0;
                    task->offerip = 0;
                    task->is_exist_ip = 0;
                    task->try_cnts = 0;
                }
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
static void __DHCP_ClientTicker(void)
{
    s32 recvlen =0;
    if(Lock_MutexPend(gClientCB.lock,CN_TIMEOUT_FOREVER))
    {
        do{
            //sockfd������Ϊ������
            recvlen = recv(gClientCB.sockfd, (void *)&gClientCB.msg, sizeof(gClientCB.msg), 0);
            if(recvlen >0) //if any message here
            {
                __DHCP_CpyReplyMsg(&gClientCB.msg);
            }
        }while(recvlen > 0);
        __DHCP_DealTask();//check all the task,if any work need to do
        Lock_MutexPost(gClientCB.lock);
    }

    return ;
}
void   RouterRemoveByHandle(struct RoutItem4 *rout);

void DhcpclientDeleteAllTask(void)
{
//  tagTaskItem      *task;
//  tagTaskItem      *task_tmp;
//  //OK,NOW ADD IT TO THE QUEUE
//  if(Lock_MutexPend(gClientCB.lock))
//  {
//      task = gClientCB.lst;
//      while (task) {
//          task_tmp = task;
//          task = task->nxt;
//          gClientCB.lst = task;
//
//          //delete task_tmp
//          if(task_tmp->routwan){
//              RouterRemoveByHandle(task_tmp->routwan);
//              task->routwan = 0;
//          }
//          if(task_tmp->routlan){
//              RouterRemoveByHandle(task_tmp->routlan);
//              task_tmp->routlan = 0;
//          }
//          if(task_tmp->sem) {
//              Lock_SempDelete(task_tmp->sem);
//              task_tmp->sem = 0;
//          }
//          net_free(task_tmp);
//          printf("-------------DhcpclientDeleteAllTask----------------\r\n");
//      }
//      Lock_MutexPost(gClientCB.lock);
//  }
}
void RouterRemoveByNetDev(const char *ifname);
void DhcpclientDeleteTask(const char *ifname)
{
    tagTaskItem      *task;
    tagTaskItem      *task_tmp;
    in_addr_t        addr= INADDR_NONE;
    if(Lock_MutexPend(gClientCB.lock,CN_TIMEOUT_FOREVER))
    {
        task = gClientCB.lst;
        while (task) {//����������һ��
            task_tmp = task;
            task = task->nxt;
            gClientCB.lst = task;
            //printf("task_tmp->ifname:%s, ifname:%s\r\n", task_tmp->ifname, ifname);
            if (strcmp(task_tmp->ifname, ifname)==0) {
                //delete task_tmp
                RouterRemoveByNetDev(ifname);//ɾ��ĳ����������·��
                NetDev_SetGateway(EN_IPV_4,NetDev_GetHandle(ifname), &addr);
                if(task_tmp->sem) {
                    Lock_SempDelete(task_tmp->sem);
                    task_tmp->sem = 0;
                }
                net_free(task_tmp);
            }
            //printf("-------------DhcpclientDeleteTask----------------\r\n");
        }
        Lock_MutexPost(gClientCB.lock);
    }

}

//s32 __NetDev_DHCP_SET_GotIP_CB(const char *ifname, s32 (*cb_ip_got)(u32 *ip));
//s32 dhcp_setip_cb(const char *ifname, s32 (*cb_ip_set)(u32 ip));
//s32 net_get_dhcp_ip(struct NetDev *pNetDev, u32 *ip_temp);
//if you want to use the interface to get a ipv4 dynamic, then call this function

//-----------------------------------------------------------------------------
//���ܣ���ӻ�ȡIPv4�����񣬿ͻ�����Ҫ��̬��ȡIP��ַʱ�����ñ�����
//������ifname��������
//      OldIP���ڴ������IP��һ����ϵͳ����������������ֱ������ԭ��ʹ�õ�IP���������
//          ����£�DHCP�������е�IP��Ŀ���ڣ������ܿ졣
//���أ��ɹ� = true��ʧ�� = false//
//-----------------------------------------------------------------------------
struct NetDev;
void NetDev_SetDefault(struct NetDev *NetDev);
bool_t DHCP_AddClientTask(const char *ifname, u32 OldIP)
{
    if(gClientCB.sockfd == 0)
        DHCP_ClientInit();
    bool_t ret = false;
    tagTaskItem *task;
//    tagHostAddrV4 addr;
//    tagRouterPara  routpara; //use this to make the router

    //ÿ���������ǰɾ���������������dhcp����
    DhcpclientDeleteTask(ifname);
    NetDev_SetDefault(NetDev_GetHandle(ifname));
    task = net_malloc(sizeof(tagTaskItem));

    if(NULL == task) //no mem here
    {
        error_printf("dhcp","%s:MEMERR\n\r",__FUNCTION__);
        goto EXIT_MEM;
    }
    memset((void *)task, 0, sizeof(tagTaskItem));
//  u32 ip_temp = 0;
//  if(net_get_dhcp_ip(NetDev_GetHandle(ifname), &ip_temp))
    if(OldIP != 0)
    {
        task->stat = EN_CLIENT_REQUEST;
        task->offerip = OldIP;
        task->is_exist_ip = 1;
    }
    else {
        task->stat = EN_CLIENT_DICOVER;
        task->is_exist_ip = 0;
    }
    task->timeout = 0;
    task->ifname = ifname;
    task->sem = Lock_SempCreate(1,0,CN_BLOCK_FIFO,ifname);
    if(!task->sem) {
        error_printf("dhcp","%s: sem_init FAILED\n\r",__FUNCTION__);
        goto EXIT_ROUTWAN;
    }

    memcpy(task->mac,NetDev_GetMac(NetDev_GetHandle(ifname)),CN_MACADDR_LEN);
    task->transID = gClientCB.txid++;

//  task->routwan = RouterCreate(&routpara); //has create one
//  if(NULL == task->routwan)
//  {
//      error_printf("dhcp","%s:ROUTWAN\n\r",__FUNCTION__);
//      goto EXIT_ROUTWAN;
//  }
    //OK,NOW ADD IT TO THE QUEUE
    if(Lock_MutexPend(gClientCB.lock,CN_TIMEOUT_FOREVER))
    {
        task->nxt = gClientCB.lst;
        gClientCB.lst = task;
        Lock_MutexPost(gClientCB.lock);
    }
    else
    {
        error_printf("dhcp","%s:QUEUE ERR\n\r",__FUNCTION__);
        goto EXIT_QUEERR;
    }
    ret = true;
    return ret;

EXIT_QUEERR:
//  RouterRemoveByHandle(task->routwan);
//  task->routwan = NULL;
EXIT_ROUTWAN:
    if(task->sem) {
       Lock_SempDelete(task->sem);
       task->sem = NULL;
    }
    net_free(task);
EXIT_MEM:
    return ret;
}


//the first time to get the ip from the server is init->discover->request->stable
//then if you want to extend the lease time, then:request,if receive ack, then goto
//the stable, if nack then goto discover and so the whole flow again
//do the client socket s32 ,after this ,we could read and write the client socket,
//which means we could do the client socket communicate; this func returns the
//socket of the client
static s32 __DHCP_SockFdInit(void)
{
    s32 ret = -1;
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
    s32 opt = 1;
    if(0 != setsockopt(ret,SOL_SOCKET,SO_NONBLOCK,&opt, sizeof(opt)))
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
bool_t  DHCP_ClientInit(void)
{
    bool_t  ret = false;

    if(gClientCB.sockfd != 0)
        return ret;
    memset(&gClientCB,0,sizeof(gClientCB));
    gClientCB.txid = CN_DHCP_CLIENTTXID;
    gClientCB.lock = Lock_MutexCreate(NULL);
    if(NULL == gClientCB.lock)
    {
        error_printf("dhcp","%s:LOCL CREATE FAILED\n\r",__FUNCTION__);
        goto EXIT_LOCK;
    }
    gClientCB.ticker = NetTickerIsrInstall("DHCPCLIENT",__DHCP_ClientTicker,\
                                            CN_DHCP_TICKER_CYCLE);
    if(NULL == gClientCB.ticker)
    {
        error_printf("dhcp","%s:TICKER CREATE FAILED\n\r",__FUNCTION__);
        goto EXIT_TICKER;
    }
    gClientCB.sockfd = __DHCP_SockFdInit();
    if(-1 == gClientCB.sockfd)
    {
        error_printf("dhcp","SOCKFD INITIALIZE FAILED\r\n");
        goto EXIT_SOCKFD;
    }
    ret = true;
    return ret;
EXIT_SOCKFD:
    NetTickerIsrRemove(gClientCB.ticker);
    gClientCB.ticker = NULL;
EXIT_TICKER:
    Lock_MutexDelete(gClientCB.lock);
    gClientCB.lock = NULL;
EXIT_LOCK:
    return ret;
}

