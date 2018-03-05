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
#include "../../tcpipconfig.h"
#include "dhcpLib.h"

#include <stdio.h>
#include <shell.h>
static u32   gDhcpIpMapBytes;                       //which defines the bitmap lenth
static u8   *gDhcpServerIpBitmap;                   //this is bitmap
typedef struct
{
	const char *dhcpserverip;
	const char *routerip;
	const char *subnetmask;
	const char *domainname;
	const char *dnsip;
    u32   ipnum;
    u32   leasetime;
}tagDhcpServerConfig;

static tagDhcpMsg          gDhcpClientMsg;

typedef struct  DhcpClient
{
	struct  DhcpClient  *nxt;
	u8      clientmac[CN_MACADDR_LEN];
	int     offset;       //which offer an ip for the client in the bitmap
	u32     ipaddress;    //client ip
	u32     timeout;      //which will count the lease time
}tagDhcpClient;

static tagDhcpClient   *pDhcpClientQ;
bool_t showDhcpClient(char *param)
{
	tagDhcpClient *client;
	u32 num = 1;
	client = pDhcpClientQ;
	while(NULL != client)
	{
		printf("Client:%d\n\r",num++);
		printf("      :offset :%d\n\r",client->offset);
		printf("      :ipaddr :%s\n\r",inet_ntoa(*(struct in_addr*)&client->ipaddress));
		printf("      :mac    :%s\n\r",Mac2string(client->clientmac));
		printf("      :timeout:%d\n\r",client->timeout);
		client = client->nxt;
	}
	return true;
}

//return -1 invalid while others ok
static int __mallocIp()
{
	int result;
	int bytes;
	int bit;

	result = -1;
	for(bytes =0;bytes<gDhcpIpMapBytes;bytes++)
	{
		for(bit =0;bit<8;bit++)
		{
			if(0==(gDhcpServerIpBitmap[bytes]&(1<<bit)))
			{
				//not used, ok, now malloc it
				result = bytes*8 + bit;
				gDhcpServerIpBitmap[bytes] |= (1<<bit);//mark it
				break;
			}
		}
		if(result != -1)
		{
			break;
		}
	}
    return result;
}
static int __freeIp(int offset)
{
	int result = -1;
	int bytes;
	int bit;
	if((offset >=0)&&(offset < gDhcpServerIpNum))
	{
		bytes = offset/8;
		bit = offset%8;
		gDhcpServerIpBitmap[bytes] &= (~(1<<bit));//remove the mark
	}
	return result;
}

//if any client match the mac, then return the match one, else malloc one
static tagDhcpClient *__newClient(u8 *mac)
{
	tagDhcpClient *result;

	result = pDhcpClientQ;
	while(NULL != result)
	{
		if(0 ==  memcmp(mac,result->clientmac,CN_MACADDR_LEN))
		{
			break;
		}
		result = result->nxt;
	}

	if(NULL == result)
	{
		//OK, NOW MALLOC ONE
		result = (tagDhcpClient *)malloc(sizeof(tagDhcpClient));
		if(NULL != result)
		{
			result->offset = __mallocIp();
			if(-1 != result->offset)
			{
				memcpy(result->clientmac,mac,CN_MACADDR_LEN);
				result->timeout = gDhcpLeaseTime;
				u32 ip = 0;
				ip = inet_addr(pDhcpServerStartIp);
				ip = ntohl(ip);
				ip += result->offset;
				ip = htonl(ip);
				result->ipaddress = ip;
				//add it to the client queue
				result->nxt = pDhcpClientQ;
				pDhcpClientQ = result;
			}
		}
	}

	return result;
}

static void __delClient(tagDhcpClient *client)
{
	tagDhcpClient *tmp;
	if(NULL != client)
	{
		//first remove it from the client queue
		if(client == pDhcpClientQ)
		{
			pDhcpClientQ = client->nxt;
		}
		else
		{
			tmp = pDhcpClientQ;
			while(NULL != tmp)
			{
				if(tmp->nxt == client)
				{
					tmp->nxt = client->nxt;
					break;
				}
				else
				{
					tmp = tmp->nxt;
				}
			}
		}

		//free the client ip and the mem
		__freeIp(client->offset);
		free((void *)client);
	}
	return;
}


static void __buildReplyPara(tagDhcpClient *client,tagDhcpReplyPara *para,u32 txid,u8 msgtype)
{
	u32    ip;

	memset((void *)para,0,sizeof(tagDhcpReplyPara));
	para->optype  = DHCP_BOOTREPLY;
	para->msgtype = msgtype;
	para->transaction = txid;

	ip = inet_addr(pDhcpSubnetMask);
	para->ipmask = ip;
	para->offerip  = client->ipaddress;

	ip = inet_addr(pDhcpRouterIp);
	para->router = ip;
	para->relayagent = ip;

	ip = inet_addr(pDhcpServerIp);
	para->dhcpserver = ip;
	para->dhcpservernxt = ip;

	memcpy(para->clientmac,client->clientmac,CN_MACADDR_LEN);
	para->renewtime = gDhcpRenewTime;
	para->bindtime  = gDhcpRebindTime;
	para->leasetime = gDhcpLeaseTime;

	memcpy(para->domainname,pDhcpDomainName,CN_DHCP_STRLEN);
	ip = inet_addr(pDhcpDns);
	para->dns1 = ip;
	para->dns2 = ip;

	return;
}


//the first time to get the ip from the server is init->discover->request->stable
//then if you want to extend the leas time, then:request,if reive ack, then goto
//the stable, if nack then goto discover and so the whole flow again
//do the client socket int ,after this ,we could read and write the client socket,
//which means we could do the client socket communicate; this func returns the
//socket of the client
static int __initSocket(void)
{
    int result = -1;
    struct sockaddr_in ipportaddr;

    result = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //USE THE DEFAULE UDP PROGRAM
    if(-1 == result)
    {
        closesocket(result);
        goto SOCKET_EXIT_FAILED;
    }

    ipportaddr.sin_family = AF_INET;
    ipportaddr.sin_port = htons(DHCP_SERVER_PORT);
    ipportaddr.sin_addr.s_addr = INADDR_ANY;

    if(-1 == bind(result, (struct sockaddr *)&ipportaddr, sizeof(ipportaddr)))
    {
        closesocket(result);
        goto SOCKET_EXIT_FAILED;
    }

    ipportaddr.sin_family = AF_INET;
    ipportaddr.sin_port = htons(DHCP_CLIENT_PORT);
    ipportaddr.sin_addr.s_addr = INADDR_BROAD;
    if(-1 == connect(result,(struct sockaddr *)&ipportaddr,sizeof(ipportaddr)))
    {
        closesocket(result);
        goto SOCKET_EXIT_FAILED;
    }
    int opt = 1;
    if(0 != setsockopt(result,SOL_SOCKET,SO_NOBLOCK,&opt, sizeof(opt)))
    {
        closesocket(result);
        goto SOCKET_EXIT_FAILED;
    }

    opt = 1;
    if(0 != setsockopt(result,SOL_SOCKET,SO_BROADCAST,&opt, sizeof(opt)))
    {
        closesocket(result);
        goto SOCKET_EXIT_FAILED;
    }
    return result;

SOCKET_EXIT_FAILED:
    result = -1;
    return result;
}

static tagDhcpReplyPara  gDhcpServerPara;
ptu32_t __DhcpServerMain(void)
{
    int                 recvlen;
    int                 serversock;
    tagDhcpRequestPara  request;
    tagDhcpClient      *client;

    serversock = __initSocket();
    if(serversock != -1)
    {
        while(1)
        {
            do{
                recvlen = recv(serversock, (void *)&gDhcpClientMsg, sizeof(gDhcpClientMsg), 0);
                if(recvlen >0)
                {
                	//now paste client request message
                	if(pasteDhcpRequestMsg(&request,&gDhcpClientMsg))
                	{
                		
                		//ok now deal it
                		client = __newClient(request.clientmac);
                		if(NULL != client)
                		{
                			if(request.msgtype == DHCP_DISCOVER)
                			{
                				__buildReplyPara(client,&gDhcpServerPara,\
                						request.transaction,DHCP_OFFER);
                				makeDhcpReplyMsg(&gDhcpClientMsg,&gDhcpServerPara);
                				send(serversock,&gDhcpClientMsg,sizeof(gDhcpClientMsg),0);
                			}
                			else if(request.msgtype == DHCP_REQUEST)
                			{
                				__buildReplyPara(client,&gDhcpServerPara,\
                						request.transaction,DHCP_ACK);
                				makeDhcpReplyMsg(&gDhcpClientMsg,&gDhcpServerPara);
                				send(serversock,&gDhcpClientMsg,sizeof(gDhcpClientMsg),0);                			}
                			else if(request.msgtype == DHCP_INFORM)
                			{
                				__buildReplyPara(client,&gDhcpServerPara,\
                						request.transaction,DHCP_ACK);
                				makeDhcpReplyMsg(&gDhcpClientMsg,&gDhcpServerPara);
                				send(serversock,&gDhcpClientMsg,sizeof(gDhcpClientMsg),0);                			}
                			else  //maybe lease
                			{
                				//del the client
                				__delClient(client);
                			}
                		}
                	}
                }
            }while(recvlen >0);
            //each seconds we will be runing
            Djy_EventDelay(1000*mS);
        }
    }
    return 0;
}
struct ShellCmdTab  gServiceDhcpd[] =
{
    {
        "dhcpdclient",
        showDhcpClient,
        "usage:dhcpdclient:show all the client",
        "usage:dhcpdclient:show all the client",
    }
};
#define CN_PINGDEBUG_NUM  ((sizeof(gServiceDhcpd))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gServiceDhcpdCmdRsc[CN_PINGDEBUG_NUM];
//this is main dhcp client module
bool_t  ModuleInstall_DhcpServer(ptu32_t para)
{
    bool_t  result = false;
    u16 evttID;
    u16 eventID;

    u32 serverip;
    u32 ipmask;
    u32 offset;
    u32 bits;
    u32 bytes;
    //do the bitmap initialize
    gDhcpIpMapBytes = (gDhcpServerIpNum +7)/8;
    gDhcpServerIpBitmap = malloc(gDhcpIpMapBytes);
    if(NULL == gDhcpServerIpBitmap)
    {
    	goto BITMAP_FAILED;
    }
    memset(gDhcpServerIpBitmap,0,gDhcpIpMapBytes);
    serverip = inet_addr(pDhcpServerIp);
    ipmask = inet_addr(pDhcpSubnetMask);
    serverip = ntohl(serverip);
    ipmask = ntohl(ipmask);
    offset = (~ipmask)&serverip;
    if(offset < gDhcpServerIpNum)  //mark it in the bitmap
    {
    	bytes = offset/8;
    	bits = offset%8;
    	gDhcpServerIpBitmap[bytes] = 1<<bits;
    }
    evttID= Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 1,\
            (ptu32_t (*)(void))__DhcpServerMain,NULL, 0x800, "DHCPD");
    if(evttID == CN_EVTT_ID_INVALID)
    {
    	goto EVTT_FAILED;
    }
    eventID = Djy_EventPop(evttID, NULL, 0, 0, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
    	goto EVENT_FAILED;
    }

    result = Sh_InstallCmd(gServiceDhcpd,gServiceDhcpdCmdRsc,CN_PINGDEBUG_NUM);
    return result;

EVENT_FAILED:
	Djy_EvttUnregist(evttID);
	evttID = CN_EVTT_ID_INVALID;
EVTT_FAILED:
	free((void *)gDhcpServerIpBitmap);
	gDhcpServerIpBitmap = NULL;
BITMAP_FAILED:
	return result;
}
bool_t ServiceDhcpdInit(ptu32_t para)
{

    bool_t result;
    result = ModuleInstall_DhcpServer(0);
    return result;
}




