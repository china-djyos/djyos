
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
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ��������ɣ���������ֲ����
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
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ������: telnet.c
// ģ��汾: V1.00
// ������Ա: ZQF
// ����ʱ��: ����2:12:24/2015-1-26
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
#include <sys/socket.h>

#include "../../tcpipconfig.h"

//�ð汾Ϊ������汾
#define CN_TELNET_SERVERPORT  23
//which send a char string to the client,which specified by the setclientfocus
#include <driver.h>
#define CN_TELNET_DEVNAME  "telnet"
#define CN_TELNET_PATH     "/dev/telnet"

//Ϊ��ͻ��˲�������׼��
#define CN_TELNET_CLIENT_MAX   2
#define CN_TELNET_USER_LEN     8
typedef struct __TelnetClient
{
	int   sock;
	char  user[CN_TELNET_USER_LEN];
	char  passwd[CN_TELNET_USER_LEN];
}tagTelnetClient;
static tagTelnetClient *pTelnetClientQ;
struct  MutexLCB  *pTelnetClientSync = NULL;

//install the device as an stdout device
static u32 __telnetWrite(ptu32_t tag,u8 *buf, u32 len,u32 offset, bool_t block,u32 timeout)
{
    tagTelnetClient *client;
    if(Lock_MutexPend(pTelnetClientSync,CN_TIMEOUT_FOREVER))
    {
        client = pTelnetClientQ;
        if(NULL != client)
        {
            sendexact(client->sock,buf,len);
        }
        Lock_MutexPost(pTelnetClientSync);
    }
    return len;
}

static u32 __telnetRead(ptu32_t tag,u8 *buf,u32 len,u32 offset,u32 timeout)
{
    return 0;
}



typedef struct
{
	FILE *stdinbak;
	FILE *stderrbak;
	FILE *stdoutbak;
	FILE *intelnet;
	FILE *outtelnet;
	FILE *errtelnet;
}tagTelnetStdio;
static tagTelnetStdio gTelnetStdio;

static void __changeio2telnet(void)
{
	if(NULL != gTelnetStdio.intelnet)
	{
		stdin = gTelnetStdio.intelnet;
	}
	if(NULL != gTelnetStdio.outtelnet)
	{
		stdout = gTelnetStdio.outtelnet;
	}
	if(NULL != gTelnetStdio.errtelnet)
	{
		stderr = gTelnetStdio.errtelnet;
	}
}
static void __changetelnet2io(void)
{
	if(NULL != gTelnetStdio.stdinbak)
	{
		stdin = gTelnetStdio.stdinbak;
	}
	if(NULL != gTelnetStdio.stdoutbak)
	{
		stdout = gTelnetStdio.stdoutbak;
	}
	if(NULL != gTelnetStdio.stderrbak)
	{
		stderr = gTelnetStdio.stderrbak;
	}
}

#define CN_CLIENT_PREFIX   "DJYOS@:"
#define CN_CLIENT_WELCOM   "Welcome TELNET FOR DJYOS\n\rENTER QUIT TO EXIT\n\r"
#define CN_CLIENT_BUFLEN   64
extern bool_t Sh_ExecCommand(char *buf);

void __telnetclientengine(tagTelnetClient  *client)
{
    int sock;
    int chars;  //how many chars in the buf
    char ch;
    int  rcvlen;
    u8   buf[CN_CLIENT_BUFLEN];
    sock = client->sock;
    sendexact(sock,CN_CLIENT_WELCOM,strlen(CN_CLIENT_WELCOM));
    sendexact(sock,CN_CLIENT_PREFIX,strlen(CN_CLIENT_PREFIX));
    chars = 0;
    //�л�IO��Ϊtelnetģʽ
    __changeio2telnet();
    while(1)
    {
        rcvlen = recv(sock,&ch,sizeof(ch),0);
        if(rcvlen == sizeof(ch))
        {
            //now check each char to do something
            if (ch == '\n') //���з���ʶ����
            {
                buf[chars] = '\0';
                if(chars > 0)
                {
                    if(0 == strcmp((const char *)buf,"quit"))
                    {
                        break;
                    }
                    else
                    {
                        //exe the command
                        Sh_ExecCommand((char *)buf);
                    }
                }
                //�����־������λbuffer
                sendexact(sock,CN_CLIENT_PREFIX,strlen(CN_CLIENT_PREFIX));
                chars =0;
            }
            else if(ch == '\r')
            {
            	//���Իس�����
            }
            else  //put the char in the buffer or move from the buf
            {
                if (ch == 8)    // Backspace
                {
                    if(chars > 0)
                    {
                        chars --;
                    }
                }
                else
                {
                    if(chars < CN_CLIENT_BUFLEN-1) //the last one will be '\0'
                    {
                        buf[chars] = ch;
                        chars++;
                    }
                    else
                    {
                        //exceed the buffer part ,will be ignored
                    }
                }
            }
        }
        else if(rcvlen == 0)
        {
            break;//bye bye
        }
        else
        {
            //do nothing ,no data yet
        }
    }

    //��telnet�лָ���ǰ��IO
    __changetelnet2io();
    return ;
}
// =============================================================================
// �������ܣ��������������񣬸�������telnet�������ĵ�����ģʽ
// ���������
// ���������
// ����ֵ  ��
// ˵��    :
// =============================================================================
static ptu32_t __telnetdmain(void)
{
    struct sockaddr_in sa_server;
    int sockserver = -1;
    int sockclient = -1;
    int sockopt = 1;
    tagTelnetClient  *client;

    //������ǰ��IO
    gTelnetStdio.stderrbak = stderr;
    gTelnetStdio.stdinbak = stdin;
    gTelnetStdio.stdoutbak = stdout;
    //��װ�����Լ��� �豸
    if(NULL ==Driver_DeviceCreate(NULL,CN_TELNET_DEVNAME,NULL,NULL,__telnetWrite,__telnetRead,NULL,NULL,0))
    {
    	printf("create dev:%s failed\n\r",CN_TELNET_DEVNAME);
        return 0;
    }
    //���Լ����豸
    gTelnetStdio.outtelnet = fopen(CN_TELNET_PATH,"a");
    if (gTelnetStdio.outtelnet == NULL)
    {
    	printf("open telnet:%s failed\n\r",CN_TELNET_PATH);
        return 0;
    }
    sockserver = socket(AF_INET, SOCK_STREAM, 0);
    sa_server.sin_family = AF_INET;
    sa_server.sin_port = htons(CN_TELNET_SERVERPORT);
    sa_server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockserver, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
    {
        closesocket(sockserver);
        return 0;
    }

    if (listen(sockserver, CN_TELNET_CLIENT_MAX) != 0)
    {
        closesocket(sockserver);
        return 0;
    }
    //here we accept all the client
    while(1)
    {
        sockclient = accept(sockserver, NULL, 0);

        if(sockclient < 0)
        {
            goto ACCEPT_ERR;
        }
        sockopt= 1;
        if(0 != setsockopt(sockclient,IPPROTO_TCP,TCP_NODELAY,&sockopt,sizeof(sockopt)))
        {
        	goto   KEEPALIVE_ERR;
        }
        sockopt =1;
        if(0 != setsockopt(sockclient,SOL_SOCKET,SO_KEEPALIVE,&sockopt,sizeof(sockopt)))
        {
        	goto   KEEPALIVE_ERR;
        }
        client = (tagTelnetClient *)malloc(sizeof(tagTelnetClient));
        if(NULL == client)
        {
            goto CLIENT_MEM;
        }
        memset((void *)client,0,sizeof(tagTelnetClient));
        client->sock = sockclient;
        pTelnetClientQ = client;
        __telnetclientengine(client);
        pTelnetClientQ = NULL;
        closesocket(sockclient);
        free(client);
        goto ACCEPT_AGAIN;
CLIENT_MEM:
        printf("%s:client memory malloc failed\n\r",__FUNCTION__);
KEEPALIVE_ERR:
        closesocket(sockclient);
ACCEPT_ERR:
        printf("%s:accept err\n\r",__FUNCTION__);
ACCEPT_AGAIN:
        printf("%s:accept complete\n\r",__FUNCTION__);
    }
    //anyway, could never reach here
    closesocket(sockserver);
    return 0;//never reach here
}
//THIS IS TELNET SERVER MODULE FUNCTION
bool_t ServiceTelnetInit(ptu32_t para)
{
    bool_t result;
    u16    evttID;
    u16    eventID;

    result = false;
    pTelnetClientSync = Lock_MutexCreate(NULL);
    if(NULL == pTelnetClientSync)
    {
    	printf("%s:mutex create err\n\r",__FUNCTION__);
        goto EXIT_MUTEX;
    }
    evttID = Djy_EvttRegist(EN_CORRELATIVE, gTelnetAcceptPrior, 0, 1,
            __telnetdmain,NULL, gTelnetAcceptStack, "TELNETD");
    if(evttID == CN_EVTT_ID_INVALID)
    {
    	printf("%s:evtt create err\n\r",__FUNCTION__);
    	goto EXIT_EVTT;
    }
    eventID = Djy_EventPop(evttID, NULL, 0, 0, 0, 0);
    if(eventID == CN_EVENT_ID_INVALID)
    {
    	printf("%s:event create err\n\r",__FUNCTION__);
    	goto EXIT_EVENT;
    }
    return true;


EXIT_EVENT:
	Djy_EvttUnregist(evttID);
EXIT_EVTT:
	Lock_MutexDelete(pTelnetClientSync);
	pTelnetClientSync = NULL;
EXIT_MUTEX:
    return false;
}






