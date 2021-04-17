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
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <netdb.h>

#define TCP_PKG_BUF_LEN (1024)                  //数据包长度
static char byMsgBuf[TCP_PKG_BUF_LEN];          //缓冲区
static int iMsgCnt = 0;                         //接收数据计数器

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
// 功能：服务器端调用，启动UDP流量测试，该任务会阻塞测试UDP数据包的速度，并打印到输出端
// 参数：SvrIP：服务器IP地址字符串,例如"192.168.0.179"
//      svrPort：UDP服务器接收的端口号，例如5000
// 返回：正常不返回，若返回-1，则出错。
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
// 功能：客户端调用，一直启动UDP发送数据包，一直发送，(注意IP地址与MAC与服务器不能一样)
// 参数：SvrIP：服务器端IP地址字符串,例如"192.168.0.179"
//      svrPort：服务器端接收的端口号，例如5000
//      pDlyUs：发送一个数据包的延时，例如0，或者100(增加延时参数防止某些网上接收太快会死)
// 返回：正常不返回，若返回-1，则出错。
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
