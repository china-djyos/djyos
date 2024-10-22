//----------------------------------------------------
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
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 模块描述: TCP服务器发送测试
// 模块版本: V1.00
// 创建人员: Administrator
// 创建时间: 9:23:45 AM/Jul 7, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "endian.h"

#include "os.h"

#include <tcpip/comport/sys/socket.h>

#define CN_TCP_SERVERPORT  2048
static u32 sgSndTimes = 0;

#define CN_SNDBUF_LEN 1400
#define CN_SNDBUF_STACKLEN  CN_SNDBUF_LEN*64
bool_t TcpServer_Snd(void)
{
    int sockfd;
    int clientfd;
    int addrlen;
    int i =0;
    struct sockaddr_in serveraddr;
    struct sockaddr_in clientaddr;
    char *sndbuf;
    int sndlen;
    s64 sndtotal;
    unsigned int sndprint =0;
    int sndopt;
    s64 sndtimestart;
    s64 sndtimetest;
    int sndspeed;
    int sndkbytes;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == sockfd)
    {
        printk("socket failed!\n\r");
        return true;
    }
    else
    {
        printk("socket success!\n\r");
    }

    serveraddr.sin_port = htons(CN_TCP_SERVERPORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(-1==bind(sockfd, &serveraddr, sizeof(serveraddr)))
    {
        printk("bind failed!\n\r");
        return true;
    }
    else
    {
        printk("bind success!\n\r");
    }

    if(-1 == listen(sockfd, 100))
    {
        printk("listen failed!\n\r");
        return true;
    }
    else
    {
        printk("listen success!\n\r");
    }

    while(1)
    {
        clientfd = accept(sockfd, &clientaddr, &addrlen);
        if(clientfd != -1)
        {
            printk("Got an client:ip = %08x  port = %04x\n\r",\
                         ntohl(clientaddr.sin_addr.s_addr),ntohs(clientaddr.sin_port));
            //关闭NAGLE
            sndopt = 1;
            if(0 == setsockopt(clientfd, IPPROTO_TCP,TCP_NODELAY,&sndopt,4))
            {
                printk("Client:close nagle success!\n\r");
            }
            else
            {
                printk("Client:close nagle  failed!\n\r");
            }
            //设置发送缓冲区
            sndopt = CN_SNDBUF_STACKLEN;
            if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDBUF,&sndopt,4))
            {
                printk("Client:set client sndbuf success!\n\r");
            }
            else
            {
                printk("Client:set client sndbuf failed!\n\r");
            }
            //设置发送缓冲区触发水平
//          sndopt = CN_SNDBUF_STACKLEN/2;
//          if(0 == setsockopt(clientfd,SOL_SOCKET ,SO_SNDLOWAT,&sndopt,4))
//          {
//              printk("Client:set client sndbuf triggerlevel success!\n\r");
//          }
//          else
//          {
//              printk("Client:set client sndbuf trigerlevel failed!\n\r");
//          }

            sndbuf = M_Malloc(CN_SNDBUF_LEN,CN_TIMEOUT_FOREVER);
            if(sndbuf)
            {
                for(i = 0; i <CN_SNDBUF_LEN; i++)
                {
                    sndbuf[i] = 'a'+i%27;
                }
                while(1)
                {
                    sndlen = send(clientfd,sndbuf,CN_SNDBUF_LEN,0);
                    if(sndlen > 0)
                    {
                        sndtotal += sndlen;
                    }
                    else
                    {
                        printk("TcpServerSnd:snd failed!\n\r");
                        closesocket(clientfd);
                        break;
                    }
                    sgSndTimes++;
                }
            }
            else
                error_printf("tcpserver", "发送时内存不足\r\n");
        }
        else
        {
            printk("Bad Accept!\n\r");
        }
    }

    closesocket(sockfd);
    return true;
}


bool_t TcpServer_SndTask(char *param)
{
   u16   evtt_id = CN_EVTT_ID_INVALID;
   evtt_id = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS-1, 0, 1,
        (ptu32_t (*)(void))TcpServer_Snd,NULL, 0x1000, "TcpServer_Snd");
    if (evtt_id != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(evtt_id, NULL, 0, 0, 0, 0);
    }
    return true;
}




