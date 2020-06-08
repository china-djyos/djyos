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

#include <arpa/inet.h>
#include "dhcpLib.h"
#include "dbug.h"


void DHCP_MakeDhcpRequestMsg(tagDhcpMsg *msg,tagDhcpRequestPara *para)
{
    char *p;
    u32 data32;
    u16 data16;

    p = (char *)(msg->options + 4);
    memset((void *)msg, 0, sizeof(tagDhcpMsg));
    //fill the dhcp header
    msg->op        =   DHCP_BOOTREQUEST;
    msg->htype     =   ARPHRD_ETHER;
    msg->hlen      =   ETH_ALEN;
    msg->xid       =   htonl(para->transaction);
    msg->secs      =   0;
    msg->flags     =   htons(CN_DHCP_FLAG_BROAD);

    memcpy(msg->chaddr,para->clientmac,ETH_ALEN);
    //the following set the dhcp options
    //set the magic number
    memcpy(msg->options,CN_DHCP_MAGICCOOKIE,4);
    //dhcp msg operate type
    ((tagDhcpOpt *)p)->type = dhcpMessageType;
    ((tagDhcpOpt *)p)->len = 1;
    ((tagDhcpOpt *)p)->data[0] = para->msgtype;
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //clientID
    ((tagDhcpOpt *)p)->type = dhcpClientIdentifier;
    ((tagDhcpOpt *)p)->len = 7;
    ((tagDhcpOpt *)p)->data[0] = ARPHRD_ETHER;
    memcpy(&((tagDhcpOpt *)p)->data[1],para->clientmac,ETH_ALEN);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //dhcp host name
    ((tagDhcpOpt *)p)->type = hostName;
    ((tagDhcpOpt *)p)->len = strlen(CN_DHCP_CLIENT_HOST);
    memcpy(&((tagDhcpOpt *)p)->data[0],CN_DHCP_CLIENT_HOST,strlen(CN_DHCP_CLIENT_HOST));
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //verdorID
    ((tagDhcpOpt *)p)->type = vendorSpecificInfo;
    ((tagDhcpOpt *)p)->len = strlen(CN_DHCP_CLIENT_VENDOR);
    memcpy(&((tagDhcpOpt *)p)->data[0],CN_DHCP_CLIENT_VENDOR,strlen(CN_DHCP_CLIENT_VENDOR));
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //dhcp max msg szie
    ((tagDhcpOpt *)p)->type = dhcpMaxMsgSize;
    ((tagDhcpOpt *)p)->len = 2;
    data16 = htons(CN_MSGMAX_LEN);
    memcpy(&((tagDhcpOpt *)p)->data[0],&data16,2);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //requestIp
    if(para->msgtype == DHCP_REQUEST)
    {
        ((tagDhcpOpt *)p)->type = dhcpRequestedIPaddr;
        ((tagDhcpOpt *)p)->len = 4;
        data32 = para->reqip;
        memcpy(&((tagDhcpOpt *)p)->data[0],&data32,4);
        p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

        if(para->dhcpserver) {
            ((tagDhcpOpt *)p)->type = dhcpServerIdentifier;
            ((tagDhcpOpt *)p)->len = 4;
            data32 = para->dhcpserver;
            memcpy(&((tagDhcpOpt *)p)->data[0],&data32,4);
            p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;
        }
    }
    //dhcp parameter request
    ((tagDhcpOpt *)p)->type = dhcpParamRequest;
    ((tagDhcpOpt *)p)->len = 14;
    ((tagDhcpOpt *)p)->data[0] = subnetMask;
    ((tagDhcpOpt *)p)->data[1] = routersOnSubnet;
    ((tagDhcpOpt *)p)->data[2] = dns;
    ((tagDhcpOpt *)p)->data[3] = hostName;
    ((tagDhcpOpt *)p)->data[4] = domainName;
    ((tagDhcpOpt *)p)->data[5] = rootPath;
    ((tagDhcpOpt *)p)->data[6] = defaultIPTTL;
    ((tagDhcpOpt *)p)->data[7] = broadcastAddr;
    ((tagDhcpOpt *)p)->data[8] = performMaskDiscovery;
    ((tagDhcpOpt *)p)->data[9] = performRouterDiscovery;
    ((tagDhcpOpt *)p)->data[10] = nisDomainName;
    ((tagDhcpOpt *)p)->data[11] = nisServers;
    ((tagDhcpOpt *)p)->data[12] = ntpServers;
    ((tagDhcpOpt *)p)->data[13] = staticRoute;
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;
//end of the options
    ((tagDhcpOpt *)p)->type = endOption;
}
void DHCP_ShowDhcpRequestMsg(tagDhcpRequestPara *para)
{
    debug_printf("dhcp","DHCPDISREQ:\n\r");
    debug_printf("dhcp","          :operation type:%d\n\r",para->optype);
    debug_printf("dhcp","          :message type  :%d\n\r",para->msgtype);
    debug_printf("dhcp","          :request ipaddr:%s\n\r",inet_ntoa(*(struct in_addr*)&para->reqip));
    debug_printf("dhcp","          :client mac    :%02X:%02X:%02X:%02X:%02X:%02X\n\r",\
            para->clientmac[0],para->clientmac[1],para->clientmac[2],\
            para->clientmac[3],para->clientmac[4],para->clientmac[5]);
    debug_printf("dhcp","          :client transID:%d\n\r",para->transaction);
    return;
}

bool_t DHCP_PasteDhcpReplyMsg(tagDhcpReplyPara *para,tagDhcpMsg *msg)
{
    unsigned char *p;
    tagDhcpOpt *item;
    int lenleft;
    u32 data32;
    u8  data8;
    bool_t result = true;
    //for the static message paste
    para->optype  =  msg->op;
    para->transaction = ntohl(msg->xid);
    para->offerip = msg->yiaddr;
    para->dhcpservernxt = msg->siaddr;
    para->relayagent = msg->giaddr;
    memcpy(para->clientmac,msg->chaddr,CN_MACADDR_LEN);
    //for the option message paste
    p = msg->options;
    lenleft = 312;
    if(0 != memcmp(p,CN_DHCP_MAGICCOOKIE,4))
    {
        result = false;
        debug_printf("dhcp","%s:dhcp magic cookie error\n\r",__FUNCTION__);
        return result;
    }

    p += 4;
    lenleft -= 4;
    while(lenleft > 0)
    {
        item = (tagDhcpOpt *)p;
        switch(item->type)
        {
            case  dhcpMessageType:
                  data8 = *(u8 *)(item->data);
                  para->msgtype = data8;
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case routersOnSubnet:
                  memcpy(&data32, item->data,4);
                  para->router = data32;
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case subnetMask:
                  memcpy(&data32, item->data,4);
                  para->ipmask = data32;
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case dhcpServerIdentifier:
                  memcpy(&data32, item->data,4);
                  para->dhcpserver= data32;
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case dhcpRenewTime:
                  memcpy(&data32, item->data,4);
                  para->renewtime= ntohl(data32);
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case dhcpRebindTime:
                  memcpy(&data32, item->data,4);
                  para->bindtime= ntohl(data32);
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case dhcpIPaddrLeaseTime:
                  memcpy(&data32, item->data,4);
                  para->leasetime= ntohl(data32);
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case dns:
                  memcpy(&data32, item->data,4);
                  para->dns1= data32;
                  if(item->len > 4)
                  {
                      memcpy(&data32, item->data+4,4);
                      para->dns2= data32;
                  }
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case domainName:
                  if(item->len >= CN_DHCP_STRLEN)
                  {
                      memcpy(para->domainname, item->data,CN_DHCP_STRLEN-1);
                  }
                  else
                  {
                      memcpy(para->domainname, item->data,item->len);
                  }
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;

            case endOption:
                  lenleft = 0;
                  break;
            default:
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
        }
    }

    return result;
}
void DHCP_ShowDhcpReplyMsg(tagDhcpReplyPara *para)
{
    debug_printf("dhcp","DHCPOFFSERACK:\n\r");
    debug_printf("dhcp","          :operation type:%d\n\r",para->optype);
    debug_printf("dhcp","          :message   type:%d\n\r",para->msgtype);
    debug_printf("dhcp","          :transaction id:%d\n\r",para->transaction);
    debug_printf("dhcp","          :offer ipaddr  :%s\n\r",inet_ntoa(*(struct in_addr*)&para->offerip));
    debug_printf("dhcp","          :dhcpservernxt :%s\n\r",inet_ntoa(*(struct in_addr*)&para->dhcpservernxt));
    debug_printf("dhcp","          :relay agent   :%s\n\r",inet_ntoa(*(struct in_addr*)&para->relayagent));
    debug_printf("dhcp","          :client mac    :%02X:%02X:%02X:%02X:%02X:%02X\n\r",\
            para->clientmac[0],para->clientmac[1],para->clientmac[2],\
            para->clientmac[3],para->clientmac[4],para->clientmac[5]);
    debug_printf("dhcp","          :renew time    :%d(seconds)\n\r",para->renewtime);
    debug_printf("dhcp","          :bind time     :%d(seconds)\n\r",para->bindtime);
    debug_printf("dhcp","          :lease time    :%d(seconds)\n\r",para->leasetime);
    debug_printf("dhcp","          :dhcp server   :%s\n\r",inet_ntoa(*(struct in_addr*)&para->dhcpserver));
    debug_printf("dhcp","          :ip mask       :%s\n\r",inet_ntoa(*(struct in_addr*)&para->ipmask));
    debug_printf("dhcp","          :router        :%s\n\r",inet_ntoa(*(struct in_addr*)&para->router));
    debug_printf("dhcp","          :domain name   :%s\n\r",para->domainname);
    debug_printf("dhcp","          :domain name s1:%s\n\r",inet_ntoa(*(struct in_addr*)&para->dns1));
    debug_printf("dhcp","          :domain name s2:%s\n\r",inet_ntoa(*(struct in_addr*)&para->dns2));
    return;
}

//paset the client request message
bool_t DHCP_PasteDhcpRequestMsg(tagDhcpRequestPara *para,tagDhcpMsg *msg)
{
    unsigned char *p;
    tagDhcpOpt *item;
    int lenleft;
    u8  data8;
    bool_t result = true;
    //for the static message paste
    para->optype  =  msg->op;
    para->transaction = ntohl(msg->xid);
    para->reqip = msg->ciaddr;
    para->dhcpserver = msg->siaddr;
    memcpy(para->clientmac,msg->chaddr,CN_MACADDR_LEN);
    //for the option message paste
    p = msg->options;
    lenleft = 312;
    if((0 != memcmp(p,CN_DHCP_MAGICCOOKIE,4))||(para->optype != DHCP_BOOTREQUEST))
    {
        result = false;
        return result;
    }

    p += 4;
    lenleft -= 4;
    while(lenleft > 0)
    {
        item = (tagDhcpOpt *)p;
        switch(item->type)
        {
            case  dhcpMessageType:
                  data8 = *(u8 *)(item->data);
                  para->msgtype = data8;
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
            case endOption:
                  lenleft = 0;
                  break;
            default:
                  p += sizeof(tagDhcpOpt) + item->len;
                  lenleft = lenleft -sizeof(tagDhcpOpt)-item->len;
                  break;
        }
    }

    return result;
}

void DHCP_MakeDhcpReplyMsg(tagDhcpMsg *msg,tagDhcpReplyPara *para)
{

    char *p;
    u32  data32;

    memset((void *)msg,0,sizeof(tagDhcpMsg));
    //OK, NOW FILL THE STATIC MESSAGE
    //fill the dhcp header
    msg->op        =   DHCP_BOOTREPLY;
    msg->htype     =   ARPHRD_ETHER;
    msg->hlen      =   ETH_ALEN;
    msg->xid       =   htonl(para->transaction);
    msg->secs      =   0;
    msg->flags     =   htons(CN_DHCP_FLAG_BROAD);
    msg->ciaddr    =   para->offerip;
    msg->yiaddr    =   para->offerip;
    msg->siaddr    =   para->dhcpservernxt;
    msg->giaddr    =   para->relayagent;
    memcpy(msg->chaddr,para->clientmac,ETH_ALEN);
    //NOW FILL THE OPTIONNAL MESSAGES
    p = (char *)(msg->options);
    //set the magic number
    memcpy(p,CN_DHCP_MAGICCOOKIE,4);
    p += 4;
    //dhcp msg operate type
    ((tagDhcpOpt *)p)->type = dhcpMessageType;
    ((tagDhcpOpt *)p)->len = 1;
    ((tagDhcpOpt *)p)->data[0] = para->msgtype;
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //renew time
    ((tagDhcpOpt *)p)->type = dhcpRenewTime;
    ((tagDhcpOpt *)p)->len = 4;
    data32 = htonl(para->renewtime);
    memcpy(&((tagDhcpOpt *)p)->data[0],&data32,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //rebind time
    ((tagDhcpOpt *)p)->type = dhcpRebindTime;
    ((tagDhcpOpt *)p)->len = 4;
    data32 = htonl(para->bindtime);
    memcpy(&((tagDhcpOpt *)p)->data[0],&data32,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //lease time
    ((tagDhcpOpt *)p)->type = dhcpIPaddrLeaseTime;
    ((tagDhcpOpt *)p)->len = 4;
    data32 = htonl(para->leasetime);
    memcpy(&((tagDhcpOpt *)p)->data[0],&data32,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //SERVER ID
    ((tagDhcpOpt *)p)->type = dhcpServerIdentifier;
    ((tagDhcpOpt *)p)->len = 4;
    memcpy(&((tagDhcpOpt *)p)->data[0],&para->dhcpserver,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //SUBNET MASK
    ((tagDhcpOpt *)p)->type = subnetMask;
    ((tagDhcpOpt *)p)->len = 4;
    memcpy(&((tagDhcpOpt *)p)->data[0],&para->ipmask,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //vendor info
    ((tagDhcpOpt *)p)->type = vendorSpecificInfo;
    ((tagDhcpOpt *)p)->len = strlen(CN_DHCP_SERVER_VENDOR);
    memcpy(&((tagDhcpOpt *)p)->data[0],CN_DHCP_SERVER_VENDOR,strlen(CN_DHCP_SERVER_VENDOR));
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //domain name
    ((tagDhcpOpt *)p)->type = domainName;
    ((tagDhcpOpt *)p)->len = strlen(para->domainname);
    memcpy(&((tagDhcpOpt *)p)->data[0],para->domainname,strlen(para->domainname));
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //router
    ((tagDhcpOpt *)p)->type = routersOnSubnet;
    ((tagDhcpOpt *)p)->len = 4;
    memcpy(&((tagDhcpOpt *)p)->data[0],&para->router,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;

    //dns
    ((tagDhcpOpt *)p)->type = dns;
    ((tagDhcpOpt *)p)->len = 8;
    memcpy(&((tagDhcpOpt *)p)->data[0],&para->dns1,4);
    memcpy(&((tagDhcpOpt *)p)->data[4],&para->dns2,4);
    p += sizeof(tagDhcpOpt) + ((tagDhcpOpt *)p)->len;
    //end of the options
    ((tagDhcpOpt *)p)->type = endOption;
}



