
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 模块描述: telnet.c
// 模块版本: V1.00
// 创建人员: ZQF
// 创建时间: 下午2:12:24/2015-1-26
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================
#include <sys/socket.h>

#include "../../tcpipconfig.h"

//该版本为单任务版本
#define CN_TELNET_SERVERPORT  23
//which send a char string to the client,which specified by the setclientfocus
#include <driver.h>
#define CN_TELNET_DEVNAME  "telnet"
#define CN_TELNET_PATH     "/dev/telnet"

//为多客户端并发做的准备
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
    //切换IO口为telnet模式
    __changeio2telnet();
    while(1)
    {
        rcvlen = recv(sock,&ch,sizeof(ch),0);
        if(rcvlen == sizeof(ch))
        {
            //now check each char to do something
            if (ch == '\n') //换行符标识结束
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
                //输出标志符并复位buffer
                sendexact(sock,CN_CLIENT_PREFIX,strlen(CN_CLIENT_PREFIX));
                chars =0;
            }
            else if(ch == '\r')
            {
            	//忽略回车符号
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

    //从telnet中恢复以前的IO
    __changetelnet2io();
    return ;
}
// =============================================================================
// 函数功能：服务器监听任务，该任务是telnet服务器的单任务模式
// 输入参数：
// 输出参数：
// 返回值  ：
// 说明    :
// =============================================================================
static ptu32_t __telnetdmain(void)
{
    struct sockaddr_in sa_server;
    int sockserver = -1;
    int sockclient = -1;
    int sockopt = 1;
    tagTelnetClient  *client;

    //备份以前的IO
    gTelnetStdio.stderrbak = stderr;
    gTelnetStdio.stdinbak = stdin;
    gTelnetStdio.stdoutbak = stdout;
    //安装我们自己的 设备
    if(NULL ==Driver_DeviceCreate(NULL,CN_TELNET_DEVNAME,NULL,NULL,__telnetWrite,__telnetRead,NULL,NULL,0))
    {
    	printf("create dev:%s failed\n\r",CN_TELNET_DEVNAME);
        return 0;
    }
    //打开自己的设备
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






