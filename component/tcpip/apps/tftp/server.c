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

#include <sys/socket.h>
#include "component_config_tftp.h"

#include "tftplib.h"
#include "dbug.h"

static u16  gLocalSeverPort = CN_TFTP_SERVERPORT_DEFAULT; //could be specified the port

ptu32_t TFTP_Server(void)
{
    int serverfd;
    struct sockaddr_in  addr;
    int                 addrlen;
    u8                 *buf;
    u8                 *tmpbuf;
    int                 len;
    int                 msglen;
    u16                 reqmod;
    char               *filename;
    char               *mode;
    int                 oblksize;
    int                 otimeout;
    int                 otsize;
    tagTftpClient      *client = NULL;
    u16                 errcode;

    buf = net_malloc(CN_TFTPSERVER_BUFLEN);
    if(NULL == buf)
    {
        error_printf("tftp","server buf net_malloc failed\n\r");
        goto EXIT_BUF;
    }
    serverfd = socket(AF_INET,SOCK_DGRAM,0);
    if(serverfd < 0)
    {
        error_printf("tftp","socket failed\n\r");
        goto EXIT_SOCKET;
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(gLocalSeverPort);
    addrlen = sizeof(addr);
    if(0 != bind(serverfd,(struct sockaddr *)&addr,addrlen))
    {
        error_printf("tftp","bind failed\n\r");
        goto EXIT_BIND;
    }
    //OK, NOW ALWAYS CHECK THE SERVER
    while(1)
    {
        msglen = recvfrom(serverfd, buf,CN_TFTPSERVER_BUFLEN,0,(struct sockaddr *)&addr,&addrlen);
        if(msglen > 0)
        {
           client = NULL;
           tmpbuf = buf;
           memcpy(&reqmod,tmpbuf,sizeof(reqmod));
           len = sizeof(reqmod);
           tmpbuf += len;
           msglen -= len;
           reqmod = ntohs(reqmod);

           oblksize = CN_TFTP_BLKSIZE_DEFAULT;
           otimeout = CN_TFTP_TIMEOUT_DEFAULT;
           otsize= 0;
           TFTP_DecodeRequestMsg(tmpbuf,msglen,&filename,&mode,&oblksize,&otimeout,&otsize);
           if((reqmod == TFTP_RRQ)||(reqmod == TFTP_WRQ))
           {
               errcode = TFTP_CreateClient(filename,mode,reqmod,&addr,oblksize,otimeout,otsize,true,&client);
               if(NULL == client)
               {
                   msglen = TFTP_MakeErrMsg(buf,CN_TFTPSERVER_BUFLEN,\
                           errcode,TFTP_ErrMsg(errcode));
                   sendto(serverfd,buf,msglen,0,(struct sockaddr*)&addr,addrlen);
               }
               else
               {
                   //create the client engine here
                   //let the engine do the left work
                   TFTP_TransEngine(client);
               }
           }
           else
           {
               errcode = EN_TFTPERR_INVALIDOPERATION;
               msglen = TFTP_MakeErrMsg(buf,CN_TFTPSERVER_BUFLEN,\
                       errcode,TFTP_ErrMsg(errcode));
               sendto(serverfd,buf,msglen,0,(struct sockaddr*)&addr,addrlen);
           }
        }
        //so another loop
    }
EXIT_BIND:
    closesocket(serverfd);
    serverfd = -1;
EXIT_SOCKET:
    net_free(buf);
    buf = NULL;
EXIT_BUF:
    error_printf("tftp","tftp server exit...\n\r");

    return 0;
}

bool_t TFTP_ServerShell(void)
{

    bool_t ret;
    ret = taskcreate("TFTPD",0x1000,CN_PRIO_RRS,TFTP_Server,NULL);
    if (ret == false) {
        error_printf("tftp","TFTPD:TASK CREATE ERR\n\r");
    }
    return ret;
}





