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

#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dbug.h"
#include "ftp.h"
#include "../../component_config_tcpip.h"



extern bool_t ServiceFtpdInit(ptu32_t para);
extern bool_t ServiceFtpcInit(ptu32_t para);

bool_t ServiceFtpInit(ptu32_t para)
{
    bool_t ret = true;
    if((CFG_FTPD_ENABLE)&&(false == ServiceFtpdInit(0)))
    {
        error_printf("ftp","###err:FTPD ERR");
        ret = false;
    }

    if((CFG_FTPC_ENABLE)&&(false == ServiceFtpcInit(0)))
    {
        error_printf("ftp","###err:FTPC ERR");
        ret = false;
    }

    return ret;

}

//some common function tools here used by the server or client
//this function used to display the ftp connection stat
void FtpShowClient(tagFtpClient *client)
{
    info_printf("ftp","Client:user       :%s\r\n",client->user);
    info_printf("ftp","Client:passwd     :%s\r\n",client->passwd);
    info_printf("ftp","Client:curdir     :%s\r\n",client->curdir);
    info_printf("ftp","Client:cmdsock    :%d\r\n",client->cchannel.s);
    info_printf("ftp","Client:ip:%s port:%d\r\n",inet_ntoa(client->cchannel.ipaddr),ntohs(client->cchannel.port));
    if(client->dchannel.ispasv)
    {
        info_printf("ftp","Client:PASV:LISTENIP:%s LISTENPORT:%d  slisten:%d saccept:%d\r\n",\
                    inet_ntoa(client->dchannel.ipaddr),ntohs(client->dchannel.port),\
                        client->dchannel.slisten,client->dchannel.saccept);
    }
    else
    {
        info_printf("ftp","Client:PORT:LISTENIP:%s LISTENPORT:%d  sconnect:%d n\r",\
                    inet_ntoa(client->dchannel.ipaddr),ntohs(client->dchannel.port),\
                        client->dchannel.sconnect);
    }
    info_printf("ftp","Client:buf:0x%08x buflen:%d datalen:%d\r\n",(u32)client->buf,client->buflen,client->datalen);
    return;
}

//use this function to receive the specified line message here,if more message here
//we will only storage the first few strings,len must be more than one
int FtpRcvLine(int sock,u8 *buf,int len)
{
    int timeout = 0;
    int rcv = 0;
    char c;
    int ret = 1; //initialize the first time
    memset(buf,0,len);
    while(ret&&(timeout <CN_FTPCLIENT_TRYTIMES))
    {
        ret = recv(sock,&c,1,0);
        if(ret == 1)
        {
            if(c == '\n')//get the response line
            {
                *buf++=c;
                rcv++;
                break;
            }
            else if(rcv < (len-1)) //the last char is '\0'
            {
                *buf++=c;
                rcv++;
            }
            else
            {
                //ignore the data,just abandon it
            }
        }
        else if(ret == -1) //timeout
        {
            timeout ++;
        }
        else
        {
            break;
        }
    }
    return rcv;
}

//create a socket and connect to the server,return the connected socket handle
int FtpConnect(struct in_addr *addr,u16 port)
{
    struct sockaddr_in address;
    int s = -1;
    memset(&address, 0, sizeof(address));
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) > 0 )
    {
        //set the timeout
        //set the receive and send timeout limit
        u32 timeo = CN_FTPCLIENT_TIMEOUT;  //DO THE TIME OUT SET
        setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo));
        setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeo, sizeof(timeo));
        //fill the address and connect the server
        address.sin_family = AF_INET;
        address.sin_port = port;
        address.sin_addr =*addr;
        if(connect(s, (struct sockaddr*) &address, sizeof(address)) == -1)
        {
            closesocket(s);
            s = -1;
            error_printf("ftp","CONNECT ERR");
        }
    }
    else
    {
        error_printf("ftp","SOCKET ERR");
    }
    return s;
}
//accept a connect and do the set
int FtpAccept(int s,struct in_addr *ipaddr,u16 *port)
{
    struct sockaddr_in address;
    int addrlen;
    int ret = -1;
    int trytimes = 0;
    int sockopt = 1;

    addrlen = sizeof(struct sockaddr_in);
    memset(&address, 0, addrlen);
    //set the timeout
    //set the receive and send timeout limit
    sockopt = CN_FTPCLIENT_TIMEOUT;  //DO THE TIME OUT SET
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &sockopt, sizeof(sockopt));
    while((trytimes < CN_FTPCLIENT_TRYTIMES)&&(ret <=0))
    {
        ret = accept(s, (struct sockaddr*) &address,&addrlen);
        if(ret>0)
        {
            sockopt = CN_FTPCLIENT_TIMEOUT;
            setsockopt(ret, SOL_SOCKET, SO_SNDTIMEO, &sockopt, sizeof(sockopt));
            setsockopt(ret, SOL_SOCKET, SO_RCVTIMEO, &sockopt, sizeof(sockopt));
            sockopt = 1;
            setsockopt(ret,SOL_SOCKET,SO_KEEPALIVE,&sockopt,sizeof(sockopt));
            if(NULL != ipaddr)
            {
                *ipaddr = address.sin_addr;
            }
            if(NULL != port)
            {
                *port = address.sin_port;
            }
        }
        trytimes++;
    }
    return ret;
}


