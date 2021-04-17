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

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netdb.h>

#define TCP_PKG_BUF_LEN (1024)                  //���ݰ�����
static char byMsgBuf[TCP_PKG_BUF_LEN];          //������
static int iMsgCnt = 0;                         //�������ݼ�����

#define TCP_PORT    (5000)

ptu32_t __Tcp_ServerTast( void )
{
    int                 sFd;
    int                 iMsgLen;

    DJY_GetEventPara((ptu32_t *)&sFd,NULL);
    while(1)
    {
        iMsgLen = recv( sFd, byMsgBuf, TCP_PKG_BUF_LEN, 0);
        iMsgCnt += iMsgLen;
     }

    closesocket (sFd);
    return 0;
}


// =============================================================================
// ���ܣ��������˵��ã�����UDP�������ԣ����������������UDP���ݰ����ٶȣ�����ӡ�������
// ������SvrIP��������IP��ַ�ַ���,����"192.168.0.179"
//      svrPort��UDP���������յĶ˿ںţ�����5000
// ���أ����������أ�������-1�������
// =============================================================================
int Tcp_SvrStart(void)
{
    short evtt;
    struct sockaddr_in  tServerAddr,tClientAddr;
    int                 sockAddrSize;
    int                 sockFd,sFd;
    int                 rcvLast;

    sockAddrSize = sizeof (struct sockaddr_in);
    memset ((char *) &tServerAddr,0, sockAddrSize);
    tServerAddr.sin_family       = AF_INET;
    tServerAddr.sin_port         = htons(TCP_PORT);
    tServerAddr.sin_addr.s_addr  = htonl(INADDR_ANY);

    if ((sockFd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("tcp socket Create Error.\r\n");
        return -1;
    }

    if (bind (sockFd, (struct sockaddr *) &tServerAddr, sockAddrSize) == -1)
    {
        printf("TCP Bind Error.\r\n");
        closesocket (sockFd);
        return -1;
    }

    if( listen( sockFd, 10 ) == -1 )
    {
        printf("TCP listen error!\r\n");
        closesocket( sockFd );
        return -1;
    }

    sFd = accept( sockFd, (struct sockaddr*)&tClientAddr, &sockAddrSize );
    if( sFd == -1)
    {
        printf("TCP accept error!\r\n");
        closesocket( sockFd );
        return -1;
    }

    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS - 20,0,0,
                                __Tcp_ServerTast,NULL,4096, "TCP_SVR_Event");

    if(evtt == CN_EVTT_ID_INVALID)
    {
        closesocket (sockFd);
        printf("TCP Create Evtt Error!\r\n");
        return -1;
    }

    DJY_EventPop(evtt,NULL,0,(ptu32_t)sFd,0,0);
    printf("TCP Server Start Receive Packet!\r\n");
    while(1)
    {
        rcvLast = iMsgCnt;
        DJY_EventDelay(1000*mS);
        printf("TCP Server Receive Speed = %d bytes/s.\r\n",iMsgCnt-rcvLast);
    }

    return 1;
}


// =============================================================================
// ���ܣ��ͻ��˵��ã�һֱ����UDP�������ݰ���һֱ���ͣ�(ע��IP��ַ��MAC�����������һ��)
// ������SvrIP����������IP��ַ�ַ���,����"192.168.0.179"
//      svrPort���������˽��յĶ˿ںţ�����5000
//      pDlyUs������һ�����ݰ�����ʱ������0������100(������ʱ������ֹĳЩ���Ͻ���̫�����)
// ���أ����������أ�������-1�������
// =============================================================================
int Tcp_ClientSend( char *SvrIP,short SvrPort)
{
    int                 sockfd;
    struct sockaddr_in  tServerAddr;
    int                 sockAddrSize;
    int                 sockopt;
    int                 iMsgLen;

    if(NULL == SvrIP)
        return -1;

    sockAddrSize = sizeof (struct sockaddr_in);
    memset ((char *) &tServerAddr,0, sockAddrSize);
    tServerAddr.sin_family       = AF_INET;
    tServerAddr.sin_port         = htons(SvrPort);
    tServerAddr.sin_addr.s_addr  = inet_addr(SvrIP);

    if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("tcp socket Create Error.\r\n");
        return -1;
    }

    sockopt = 1;
    if( 0 != setsockopt( sockfd, IPPROTO_TCP, TCP_NODELAY, (void*)&sockopt, sizeof( int ) ) )
    {
        printf("tcp set socket opt 'TCP_NODELAY' failed!\r\n");
        return -1;
    }
    sockopt = 1;
    if( 0 != setsockopt( sockfd, SOL_SOCKET, SO_KEEPALIVE, (void*)&sockopt, sizeof( int ) ) )
    {
        printf("tcp set socket opt 'SO_KEEPALIVE' failed!\r\n");
        return -1;
    }

    if( (connect(sockfd,(struct sockaddr *)&tServerAddr,sockAddrSize)) == -1)
    {
        printf("tcp connect failed.\r\n");
        return -1;
    }

    memset(byMsgBuf,0x00,TCP_PKG_BUF_LEN);

    printf("TCP Client Start Send Packet!\r\n");
    while(1)
    {
        byMsgBuf[0] = byMsgBuf[0] + 1;
//      iMsgLen =  (int)(drand48() * TCP_PKG_BUF_LEN);
        iMsgLen = send(sockfd, byMsgBuf, TCP_PKG_BUF_LEN, 0);
        if(iMsgLen)
            iMsgLen = send(sockfd, byMsgBuf, iMsgLen, 0);
    }

    closesocket (sockfd);
    return 0;
}
