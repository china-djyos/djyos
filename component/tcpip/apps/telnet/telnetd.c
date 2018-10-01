
//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

#include <device.h>
#include <ring.h>
#include <sys/socket.h>
#include "dbug.h"

#include "../../component_config_tcpip.h"


//�ð汾Ϊ������汾
#define CN_TELNET_SERVERPORT  23
//which send a char string to the client,which specified by the setclientfocus
#define CN_TELNET_DEVNAME  "telnet"
#define CN_TELNET_PATH     "/dev/telnet"
//here we will create a virtual device as the terminal for the shell,the shell could
//read or write the terminal
#define CN_DEV_BUFLEN  64  //maybe enough for the client input
typedef struct
{
    struct RingBuf       *ring;
    semp_t                rcvsync;     //the read task will pend here
    int                   clientfd;    //which connect to the server
    void                 *obj;         //which will be created by open
}tagDevTelnetd;
static tagDevTelnetd gDevTelnetd;

//do the open
static s32 __open(void *obj, u32 dwMode, u32 timeout)
{
    s32 ret =-1;
    if(NULL == gDevTelnetd.obj)
    {
        gDevTelnetd.obj = obj;
        ret = 0;
    }
    return ret;
}
static s32 __close(void *obj)
{
    s32 ret =-1;
    if(NULL == gDevTelnetd.obj)
    {
        gDevTelnetd.obj = NULL;
        ret = 0;
    }
    return ret;
}
//install the device as an io device
static u32 __write(void *obj,u8 *buf, u32 len,u32 offset, bool_t block,u32 timeout)
{
    if(gDevTelnetd.clientfd >0)
    {
        sendexact(gDevTelnetd.clientfd,buf,len);
    }
    return len;
}
static u32 __read(void *obj,u8 *buf,u32 len,u32 offset,u32 timeout)
{
    u32 ret =0;
    if(semp_pendtimeout(gDevTelnetd.rcvsync,timeout))
    {
        ret = Ring_Read(gDevTelnetd.ring,buf,len);
        if(Ring_Check(gDevTelnetd.ring)) //still some data in the ring
        {
            semp_post(gDevTelnetd.rcvsync);
            fcntl(hande2fd(gDevTelnetd.obj),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
        }
        else
        {
            fcntl(hande2fd(gDevTelnetd.obj),F_CLREVENT,CN_MULTIPLEX_SENSINGBIT_READ);
        }
    }
    return ret;
}

#define CN_CLIENT_WELCOM   "Welcome TELNET FOR DJYOS\n\rENTER QUIT TO EXIT\n\r"

#pragma pack(1)
typedef struct
{
    u8 iac;
    u8 code;
    u8 opt;
}tagNvtCmd;
#pragma pack()

#define CN_NVT_IAC   0XFF
#define CN_NVT_DONT  0XFE
#define CN_NVT_DO    0XFD
#define CN_NVT_WONT  0XFC
#define CN_NVT_WILL  0XFB

#define CN_NVT_OPT_ECHO    1
#define CN_NVT_OPT_SGA     3   //suppress go ahead
#define CN_NVT_OPT_STATUS  5


static void __telnetclientengine(int sock)
{
    char ch;
    int  len;
    u8  optbuf[15];
    tagNvtCmd *opt;
    //first we should send the nvt info to the client,tell them not do the echo
    len = 0;
    opt = (tagNvtCmd*)optbuf;
    //will echo.
    opt->iac = CN_NVT_IAC;
    opt->code = CN_NVT_WILL;
    opt->opt = CN_NVT_OPT_ECHO;
    opt++;
    len += sizeof(tagNvtCmd);

    //dont echo
    opt->iac = CN_NVT_IAC;
    opt->code = CN_NVT_DONT;
    opt->opt = CN_NVT_OPT_ECHO;
    opt++;
    len += sizeof(tagNvtCmd);
    sendexact(sock,optbuf,len);
    //rcv the code
    do
    {
        if(recvexact(sock,optbuf,sizeof(tagNvtCmd)))
        {
            opt = (tagNvtCmd*)optbuf;
            if(opt->code != CN_NVT_IAC)
            {
                break;
            }
        }
        else
        {
            break;
        }
    }while(1);

    Ring_Write(gDevTelnetd.ring,(u8 *)opt,sizeof(tagNvtCmd));
    semp_post(gDevTelnetd.rcvsync);
    fcntl(hande2fd(gDevTelnetd.obj),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
    //OK,now send info here
    sendexact(sock,(u8 *)CN_CLIENT_WELCOM,strlen(CN_CLIENT_WELCOM));
    gDevTelnetd.clientfd = sock;
    while(1)
    {
        len = recv(sock,&ch,sizeof(ch),0);
        if(len == sizeof(ch))
        {
            Ring_Write(gDevTelnetd.ring,(u8 *)&ch,1);
            semp_post(gDevTelnetd.rcvsync);
            fcntl(hande2fd(gDevTelnetd.obj),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
        }
        else if(len == 0)
        {
            break;//bye bye
        }
        else
        {
            //do nothing ,no data yet
        }
    }
    gDevTelnetd.clientfd = -1;
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

    //��װ�����Լ��� �豸, TODO,���׽ӿ�����������
    if(-1==dev_add(NULL, CN_TELNET_DEVNAME,__open,__close,__write,__read,NULL,0))
    {
        printf("\r\n: info : net    : create dev %s failed.",CN_TELNET_DEVNAME);
        return 0;
    }

    add2listenset(CN_TELNET_PATH,true);//��ӵ��豸���

    sockserver = socket(AF_INET, SOCK_STREAM, 0);
    sa_server.sin_family = AF_INET;
    sa_server.sin_port = htons(CN_TELNET_SERVERPORT);
    sa_server.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockserver, (struct sockaddr *) &sa_server, sizeof(sa_server)) < 0)
    {
        closesocket(sockserver);
        return 0;
    }

    if (listen(sockserver, 1) != 0)
    {
        closesocket(sockserver);
        return 0;
    }
    //here we accept all the client
    while(1)
    {
ACCEPT_AGAIN:
        sockclient = accept(sockserver, NULL, 0);

        if(sockclient < 0)
        {
            debug_printf("telnet","%s:accept err\n\r",__FUNCTION__);
            goto ACCEPT_AGAIN;
        }
        sockopt= 1;
        if(0 != setsockopt(sockclient,IPPROTO_TCP,TCP_NODELAY,&sockopt,sizeof(sockopt)))
        {
            debug_printf("telnet","%s:set nodelay err\n\r",__FUNCTION__);
            closesocket(sockclient);
            goto   ACCEPT_AGAIN;
        }
        sockopt =1;
        if(0 != setsockopt(sockclient,SOL_SOCKET,SO_KEEPALIVE,&sockopt,sizeof(sockopt)))
        {
            debug_printf("telnet","%s:set keepalive err\n\r",__FUNCTION__);
            closesocket(sockclient);
            goto   ACCEPT_AGAIN;
        }
        __telnetclientengine(sockclient);
        closesocket(sockclient);
    }
    //anyway, could never reach here
    closesocket(sockserver);
    return 0;//never reach here
}
//THIS IS TELNET SERVER MODULE FUNCTION
bool_t ServiceInit_Telnetd(void)
{
    bool_t ret = false;
    memset(&gDevTelnetd,0,sizeof(gDevTelnetd));
    gDevTelnetd.clientfd = -1;
    gDevTelnetd.ring = Ring_Create(CN_DEV_BUFLEN);
    if(NULL == gDevTelnetd.ring)
    {
        debug_printf("telnet","TELNETD RING CREATE FAILED\n\r");
        goto EXIT_RING;
    }
    gDevTelnetd.rcvsync = semp_init(1,0,NULL);
    if(NULL == gDevTelnetd.rcvsync)
    {
        debug_printf("telnet","TELNETD RING CREATE FAILED\n\r");
        goto EXIT_SEMP;
    }
    ret = taskcreate("telnet",0x800,CN_PRIO_RRS,__telnetdmain,NULL);
    if (ret == false) {
        debug_printf("telnet","TFTPD:TASK CREATE ERR\n\r");
        goto EXIT_TASK;
    }

    return ret;

EXIT_TASK:
    semp_del(gDevTelnetd.rcvsync);
EXIT_SEMP:
    Ring_Destroy(gDevTelnetd.ring);
    gDevTelnetd.ring = NULL;
EXIT_RING:
    return ret;
}






