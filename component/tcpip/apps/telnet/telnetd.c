
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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 模块描述: telnet.c
// 模块版本: V1.00
// 创建人员: ZQF
// 创建时间: 下午2:12:24/2015-1-26
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================

#include <device.h>
#include <stdio.h>
#include <ring.h>
#include <sys/socket.h>
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"telnet"      //telnet协议
//parent:"tcpip"     //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip","tcp"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TELNET == false )
//#warning  " telnet  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TELNET    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,,,
//%$#@enum,true,false,
#define     CFG_TELNETD_ENABLE          true    //"telnet 服务器使能"
#define     CFG_TELNETC_ENABLE          false   //"telnet 客户端使能"
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure



//该版本为单任务版本
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
    struct SemaphoreLCB  *rcvsync;     //the read task will pend here
    int                   clientfd;    //which connect to the server
    struct objhandle     *hdl;         //which will be created by open
}tagDevTelnetd;
static tagDevTelnetd gDevTelnetd;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//do the open
static s32 __Telnet_open(struct objhandle *hdl, u32 dwMode, u32 timeout)
{
    s32 ret =-1;
    if(NULL == gDevTelnetd.hdl)
    {
        gDevTelnetd.hdl = hdl;
        ret = 0;
    }
    return ret;
}
static s32 __Telnet_close(struct objhandle *hdl)
{
    s32 ret =-1;
    if(NULL == gDevTelnetd.hdl)
    {
        gDevTelnetd.hdl = NULL;
        ret = 0;
    }
    return ret;
}


//install the device as an io device
static s32 __Telnet_write(struct objhandle *hdl,u8 *buf, u32 len,u32 offset, u32 timeout)
{
    if(gDevTelnetd.clientfd >0)
    {
        sendexact(gDevTelnetd.clientfd,buf,len);
    }
    return len;
}
static s32 __Telnet_read(struct objhandle *hdl,u8 *buf,u32 len,u32 offset,u32 timeout)
{
    u32 ret =0;
    if(Lock_SempPend(gDevTelnetd.rcvsync,timeout))
    {
        ret = Ring_Read(gDevTelnetd.ring,buf,len);
        if(Ring_Check(gDevTelnetd.ring)) //still some data in the ring
        {
            Lock_SempPost(gDevTelnetd.rcvsync);
            fcntl(Handle2fd(gDevTelnetd.hdl),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
        }
        else
        {
            fcntl(Handle2fd(gDevTelnetd.hdl),F_CLREVENT,CN_MULTIPLEX_SENSINGBIT_READ);
        }
    }
    return ret;
}
#pragma GCC diagnostic pop

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


static void __Telnet_ClientEngine(int sock)
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
    Lock_SempPost(gDevTelnetd.rcvsync);
    fcntl(Handle2fd(gDevTelnetd.hdl),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
    //OK,now send info here
    sendexact(sock,(u8 *)CN_CLIENT_WELCOM,strlen(CN_CLIENT_WELCOM));
    gDevTelnetd.clientfd = sock;
    while(1)
    {
        len = recv(sock,&ch,sizeof(ch),0);
        if(len == sizeof(ch))
        {
            Ring_Write(gDevTelnetd.ring,(u8 *)&ch,1);
            Lock_SempPost(gDevTelnetd.rcvsync);
            fcntl(Handle2fd(gDevTelnetd.hdl),F_SETEVENT,CN_MULTIPLEX_SENSINGBIT_READ);
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
// 函数功能：服务器监听任务，该任务是telnet服务器的单任务模式
// 输入参数：
// 输出参数：
// 返回值  ：
// 说明    :
// =============================================================================
static ptu32_t __Telnet_DomainMain(void)
{
    struct sockaddr_in sa_server;
    int sockserver = -1;
    int sockclient = -1;
    int sockopt = 1;

    //安装我们自己的 设备, TODO,这套接口作的有问题
    if(NULL==Device_Create(CN_TELNET_DEVNAME,__Telnet_open,__Telnet_close,__Telnet_write,__Telnet_read,NULL,0))
    {
        printf("\r\n: info : net    : create dev %s failed.",CN_TELNET_DEVNAME);
        return 0;
    }

    add2listenset(CN_TELNET_PATH);//添加到设备输出

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
        __Telnet_ClientEngine(sockclient);
        closesocket(sockclient);
    }
    //anyway, could never reach here
    closesocket(sockserver);
    return 0;//never reach here
}
//THIS IS TELNET SERVER MODULE FUNCTION
bool_t Telnet_ServiceInit(void)
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
    gDevTelnetd.rcvsync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == gDevTelnetd.rcvsync)
    {
        debug_printf("telnet","TELNETD RING CREATE FAILED\n\r");
        goto EXIT_SEMP;
    }
    ret = taskcreate("telnet",0x800,CN_PRIO_RRS,__Telnet_DomainMain,NULL);
    if (ret == false) {
        debug_printf("telnet","TFTPD:TASK CREATE ERR\n\r");
        goto EXIT_TASK;
    }

    return ret;

EXIT_TASK:
    Lock_SempDelete(gDevTelnetd.rcvsync);
EXIT_SEMP:
    Ring_Destroy(gDevTelnetd.ring);
    gDevTelnetd.ring = NULL;
EXIT_RING:
    return ret;
}

