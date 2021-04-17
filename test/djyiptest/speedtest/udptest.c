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

#define UDP_PKG_BUF_LEN (1024)                  //数据包长度
static char byMsgBuf[UDP_PKG_BUF_LEN];          //缓冲区
static int iMsgCnt = 0;                         //接收数据计数器

ptu32_t __Udp_ServerTast( void )
{
    struct sockaddr_in  tClientAddr;
    int                 sockAddrSize;
    int                 sFd;
    int                 iMsgLen;

    DJY_GetEventPara((ptu32_t *)&sFd,NULL);
    while(1)
    {
        iMsgLen = recvfrom( sFd, byMsgBuf, UDP_PKG_BUF_LEN, 0, (struct sockaddr *)&tClientAddr, &sockAddrSize );
        if( -1 == iMsgLen )
        {
            printf("udp server recv Error!\r\n");
            closesocket (sFd);
            return -1;
        }

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
int Udp_SvrStart( char* SvrIP,short SvrPort )
{
    short evtt;
    struct sockaddr_in  tServerAddr;
    int                 sockAddrSize;
    int                 sFd;
    int                 rcvLast;

    /* set up the local address */
    sockAddrSize = sizeof (struct sockaddr_in);
    memset ((char *) &tServerAddr,0, sockAddrSize);
    tServerAddr.sin_family       = AF_INET;
    tServerAddr.sin_port         = htons(SvrPort);
    tServerAddr.sin_addr.s_addr  = inet_addr(SvrIP);   // 采用 UDP/IP

    /* create a UDP-based socket */
    if ((sFd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("udp socket Create Error\n");
        return -1;
    }

    /* bind socket to local address */
    if (bind (sFd, (struct sockaddr *) &tServerAddr, sockAddrSize) == -1)
    {
        printf("Udp Bind Error %s:%d\n",SvrIP,SvrPort);
        closesocket (sFd);
        return -1;
    }

    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS - 20,0,0,
                                __Udp_ServerTast,NULL,4096, "UDP_SVR_Event");

    if(evtt == CN_EVTT_ID_INVALID)
    {
        closesocket (sFd);
        return -1;
    }

    DJY_EventPop(evtt,NULL,0,(ptu32_t)sFd,0,0);
    printf("UDP Server Start Receive Packet!\r\n");
    while(1)
    {
        rcvLast = iMsgCnt;
        DJY_EventDelay(1000*mS);
        printf("UDP Server Receive Speed = %d bytes/s.\r\n",iMsgCnt-rcvLast);
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
int Udp_ClientSend( char *SvrIP,short SvrPort, int pDlyUs )
{
    int                 sockfd;
    struct sockaddr_in  tServerAddr;
    int                 sockAddrSize;
    int                 iMsgLen;

    if(NULL == SvrIP)
        return -1;

    /* set up the remote address */
    sockAddrSize = sizeof (struct sockaddr_in);
    memset ((char *) &tServerAddr,0, sockAddrSize);
    tServerAddr.sin_family       = AF_INET;
    tServerAddr.sin_port         = htons(SvrPort);
    tServerAddr.sin_addr.s_addr  = inet_addr(SvrIP);

    /* create a UDP-based socket */
    if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("udp socket Create Error.\r\n");
        return -1;
    }

    memset(byMsgBuf,0x00,UDP_PKG_BUF_LEN);

    printf("UDP Client Start Send Packet!\r\n");
    while(1)
    {
        byMsgBuf[0] = byMsgBuf[0] + 1;
        DJY_DelayUs(pDlyUs);
        iMsgLen = sendto(sockfd, byMsgBuf, UDP_PKG_BUF_LEN, 0,
                (struct sockaddr *)&tServerAddr, sockAddrSize);
        if(-1 == iMsgLen)
        {
            printf("UDP Client sendto error!\r\n");
            closesocket (sockfd);
            return -1;
        }
    }

    closesocket (sockfd);
    return 0;
}
