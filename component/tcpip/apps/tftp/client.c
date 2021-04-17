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
#include <arpa/inet.h>
#include <netdb.h>
#include "component_config_tftp.h"

#include "tftplib.h"
#include "dbug.h"
#include <shell.h>
#include <misc/misc.h>

static u16  gRemoteServerPort = CN_TFTP_SERVERPORT_DEFAULT;

static bool_t __TFTP_ClientMain(char *mode, char *serverip,char *filename)
{
    bool_t          result = false;
    tagTftpClient  *client = NULL;
    u16             reqmod;
    u16             errcode;
    struct sockaddr_in  netaddr;

    if(0 ==strcmp(mode,"get"))
    {
        reqmod = TFTP_RRQ;
    }
    else if(0 ==strcmp(mode,"put"))
    {
        reqmod = TFTP_WRQ;
    }
    else
    {
        debug_printf("tftp","%s:unknow mode:%s\n\r",__FUNCTION__,mode);
        return result;
    }

    netaddr.sin_family = AF_INET;
    netaddr.sin_addr.s_addr = inet_addr(serverip);
    netaddr.sin_port = htons(gRemoteServerPort);
    errcode = TFTP_CreateClient(filename,"octet",reqmod,&netaddr,\
               CN_TFTP_BLKSIZE_DEFAULT,CN_TFTP_TIMEOUT_DEFAULT,0,false,&client);
    if(NULL != client)
    {
        //OK NOW, DO THE TFTP TRANSACTION
        TFTP_TransEngine(client);
    }
    else
    {
        debug_printf("tftp","%s:create client err:%s\n\r",__FUNCTION__,TFTP_ErrMsg(errcode));
    }

    return true;
}


//use it as:tftp serverip filename
//bool_t TftpClientShell(char *param)
bool_t tftp(char *param)
{
    int argc =3;
    char *argv[3];

    string2arg(&argc,argv,param);
    if(argc == 3)
    {
        __TFTP_ClientMain(argv[0],argv[1],argv[2]);
    }
    else
    {
        debug_printf("tftp","%s:para err:argc:%d\n\r",__FUNCTION__,argc);
    }

    return true;
}
extern bool_t TFTP_ServerShell(void);

//THIS IS PING MODULE FUNCTION
bool_t TFTP_ServiceInit(void)
{
    bool_t result;

    TFTP_PathSet((char *)CFG_TFTP_PATHDEFAULT);


    result = TFTP_ServerShell();

    return result;
}

ADD_TO_ROUTINE_SHELL(tftp,tftp,"usage:tftp get/put hostname filename");


