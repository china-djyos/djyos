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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
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





