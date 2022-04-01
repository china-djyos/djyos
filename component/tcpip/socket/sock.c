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
#include <sys/socket.h>
#include  <dirent.h>
#include  <errno.h>
#include "../component_config_tcpip.h"
#include  <fcntl.h>
#include  "../common/tpl.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"sock"        //sock协议
//parent:"tcpip"     //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_SOCK == false )
//#warning  " sock  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_SOCK    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@num,,,
#define     CFG_SOCKET_NUM              10      //"socket数限值"，占一个 tagItem 结构
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure


typedef struct tagSocket  tagItemCB; //each socket is a item
static struct Object *s_ptSocketObject;
static struct MemCellPool *s_ptSocketPool;  //socket控制块内存池头指针

struct tagSocket *__Fd2Sock(s32 fd)
{
    struct objhandle *hdl;
    struct tagSocket *sock;

    hdl = fd2Handle(fd);
    sock = (struct tagSocket *)Handle_GetContext(hdl);
    return sock;
}

struct tagSocket *SocketBuild(void)
{
    struct objhandle *SocketFile;
    struct tagSocket *sock;
    sock = Mb_Malloc(s_ptSocketPool, 0);
    if(sock)
    {
        memset(sock, 0, sizeof(struct tagSocket));
        sock->SockSync = Lock_MutexCreate(NULL);
        SocketFile = Handle_New( );
        if(SocketFile != NULL)
        {
            Handle_Init(SocketFile, s_ptSocketObject, O_RDWR,(ptu32_t)sock);
            sock->sockfd = Handle2fd(SocketFile);
//          sock = Handle2fd(SocketFile);
//          sock->sockfd = sock;
//          sock->ProtocolOps = ProtocolOps;
//          Handle_SetMultiplexEvent(SocketFile, sock->IoInitstat);
        }
        else
        {
//          errno = ENOENT;
            Lock_MutexDelete(sock->SockSync);
            Mb_Free(s_ptSocketPool, sock);
            sock = NULL;
        }
    }
    return sock;
}

bool_t SocketFree(struct tagSocket *sock)
{
    Handle_Delete(fd2Handle(sock->sockfd));
    Lock_MutexDelete(sock->SockSync);
    Mb_Free(s_ptSocketPool, sock);
    return true;
}

//-----------------------------------------------------------------------------
//功能: 创建一个套接字，先检查传输层协议函数集是否已经注册，然后调用传输层的 socket 函数
//     创建socket，在  s_ptSocketObject对象下创建 handle ，socket作为handle的私有成员。
//参数: family，地址族，见socket.h的 AF_INET 等定义
//      type，socket 协议类型，见socket.h的 SOCK_DGRAM 等定义
//      protocol，具体协议，见socket.h的 IPPROTO_UDP 等定义
//返回: 套接字 fd
//-----------------------------------------------------------------------------
s32 socket(s32 family, s32 type, s32 protocol)
{
    s32 result = -1;
    struct TPL_ProtocalOps  *ProtocolOps = NULL;
    struct tagSocket       *sock = NULL;
    ProtocolOps = TPL_GetProto(family, type, protocol);
    if(NULL != ProtocolOps)
    {
        if(NULL != ProtocolOps->__socket)
        {
            sock = ProtocolOps->__socket(family, type, protocol);
            if(sock == NULL)
            {
                errno =EPROTOTYPE;
            }
            else
            {
                errno = EN_NEWLIB_NO_ERROR;
                result = sock->sockfd;
            }
        }
        else
        {
            errno =EPROTOTYPE;
        }
    }
    else
    {
        errno = ENOPROTOOPT;
    }
    return  result;
}

//-----------------------------------------------------------------------------
//功能: 为一个套接字绑定一个网络地址
//参数: sockfd,待绑定的套接字
//      myaddr,指定的网络地址
//      addrlen,网络地址长度
//返回：0 成功  -1失败
//说明：失败一般的是因为该网络地址冲突（端口号）
//-----------------------------------------------------------------------------
s32 bind(s32 sockfd,struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__bind))
    {
        result = sock->ProtocolOps->__bind(sock, addr, addrlen);
        if(result < 0)
        {
            errno =EADDRINUSE;
        }
        else
        {
            errno =EN_NEWLIB_NO_ERROR;
        }
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}
// =============================================================================
// 函数功能：  listen
//           指定套接字为监听状态
// 输入参数：  sockfd,待指定的套接字
//           pendlimit,允许的链接个数，默认的为5
// 输出参数：
// 返回值  ：0 成功  -1失败
// 说明    ：失败一般的是因为重复指定
// =============================================================================
s32 listen(s32 sockfd, s32 pendlimit)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__listen))
    {
        result = sock->ProtocolOps->__listen(sock, pendlimit);
    }
    if(result < 0)
    {
        errno = ENOTSOCK;
    }
    return  result;
}
// =============================================================================
// 函数功能：  accept
//           等待客户端的链接
// 输入参数：  sockfd,服务器端套接字
// 输出参数：  addr,用于接收连接的客户端地址
//           addrlen，用于接收地址长度
// 返回值  ：链接的客户端套接字，-1出错
// 说明    ：出错一般是因为非阻塞而且没有连接的客户端，或者超出链接限制数
// =============================================================================
s32 accept(s32 sockfd, struct sockaddr *addr, s32 *addrlen)
{
    struct tagSocket *sock;
    struct tagSocket *result;
    s32        client = -1;

    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__accept))
    {
        result = sock->ProtocolOps->__accept(sock, addr,addrlen);
        if(result == NULL)
        {
            client = -1;
            errno = EAGAIN;
        }
        else
        {
            client = result->sockfd;
            errno =EN_NEWLIB_NO_ERROR;
        }
    }
    else
    {
        errno = ENOTSOCK;
    }
    return client;
}

// =============================================================================
// 函数功能：  connect
//           链接服务器
// 输入参数：  sockfd,客户端套接字
//           addr,连接的服务器地址
//           addrlen，地址长度
// 输出参数：
// 返回值  ：-1出错，0成功
// 说明    ：
// =============================================================================
s32 connect(s32 sockfd, struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__connect))
    {
        result = sock->ProtocolOps->__connect(sock, addr, addrlen);
        if(errno < 0)
        {
            if ((sock->sockstat & CN_SOCKET_PROBLOCK) == 0 &&
                (sock->sockstat & CN_SOCKET_PROCONNECT)){//非阻塞, TCP
                //printf("connect noblock, EAGAIN\r\n");
                errno = EAGAIN;
            }
            else
            {
                errno = ECONNREFUSED;
            }
        }
        else
        {
            errno =EN_NEWLIB_NO_ERROR;
        }
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}
// =============================================================================
// 函数功能：  send
//           向套接字发送数据
// 输入参数：  sockfd,目的套接字
//           msg,待发送的信息
//           len,信息长度
//           flags,一般为0
// 输出参数：
// 返回值  ：-1出错，否则返回发送字节数
// 说明    ：
// =============================================================================
s32 send(s32 sockfd, const void *msg, s32 len, s32 flags)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__send))
        {
            result = sock->ProtocolOps->__send(sock, msg, len, flags);
            if(result < 0)
            {
                errno = EAGAIN;
            }
            else if(result == 0)
            {
                errno = ESHUTDOWN;
            }
            else
            {
                errno =EN_NEWLIB_NO_ERROR;
            }
        }
        else
        {
            errno = ENOTSOCK;
        }
    }
    return  result;
}
//the send or recv exact function used to deal the application ProtocolOps
//which has negotiate the data structure
//we use the function to send the exact messages
//if return true, then the message has been sent in to the buffer at least
//if return false, then the socket must has some err
bool_t sendexact(s32 sock,u8 *buf,s32 len)
{
//  bool_t ret=true;
    s32 sentlen;
//  sentlen = 0;
    sentlen = send(sock,buf,len,0);
//  while(len > 0)
//  {
//      if(sentlen > 0)
//      {
//          len -= sentlen;
//          buf += sentlen;
//      }
//      else if(sentlen == 0)
//      {
//          ret = false;
//          break; //maybe not permit to send any data
//      }
//      else
//      {
//          //maybe timeout continue
//      }
//  }
    if(sentlen == len)
        return true;
    else
        return false;
}

//we use the function to recv the exact messages
//if return true, then the message has been cpied to the buffer
//if return false, then the socket must has some err
bool_t recvexact(s32 sock,u8 *buf,s32 len)
{
    bool_t result;
    s32 recvlen;

    recvlen = 0;
    result = true;
    while(len > 0)
    {
        recvlen = recv(sock,buf,len,0);
        if(recvlen > 0)
        {
            len -= recvlen;
            buf += recvlen;
        }
        else if(recvlen == 0)
        {
            result = false;
            break; //maybe the socket has been shutdown
        }
        else
        {
            //maybe timeout continue
        }
    }
    return result;
}

// =============================================================================
// 函数功能：  recv
//           读取接字数据
// 输入参数：  sockfd,目的套接字
//           flags,一般为0
// 输出参数：
//           buf,收到的信息
//           len,buf长度
// 返回值  ：-1出错，否则返回收到字节数
// 说明    ：
// =============================================================================
s32 recv(s32 sockfd, void *buf,s32 len, u32 flags)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;

    if((NULL != buf)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__recv))
        {
            result = sock->ProtocolOps->__recv(sock, buf, len, flags);
            if(result < 0)
            {
                errno = EAGAIN;
            }
            else if(result == 0)
            {
                errno = ESHUTDOWN;
            }
            else
            {
                errno =EN_NEWLIB_NO_ERROR;
            }
        }
        else
        {
            errno = ENOTSOCK;
        }
    }

    return  result;
}
// =============================================================================
// 函数功能：  sendto
//           向目的网络地址发送数据
// 输入参数：  sockfd,目的套接字
//           msg,待发送的信息
//           len,信息长度
//           flags,一般为0
//           addr,目的网络地址
//           addrlen,目的网络地址长度
// 输出参数：
// 返回值  ：-1出错，否则返回发送字节数
// 说明    ：
// =============================================================================
s32 sendto(s32 sockfd, const void *msg,s32 len, u32 flags,\
          const struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__sendto))
        {
            result = sock->ProtocolOps->__sendto(sock, msg, len, flags,addr, addrlen);
            if(result < 0)
            {
                errno = EAGAIN;
            }
            else if(result == 0)
            {
                errno = ESHUTDOWN;
            }
            else
            {
                errno =EN_NEWLIB_NO_ERROR;
            }
        }
        else
        {
            errno = ENOTSOCK;
        }
    }
    return  result;
}
// =============================================================================
// 函数功能：  读取数据并记录数据源地址
// 输入参数：  sockfd,目的套接字
//           flags,一般为0
//           addr,接收到的数据的来源地址
//           addrlen,网络地址长度
// 输出参数：
//           buf,读取的数据
//           len,信息长度
// 返回值  ：-1出错，否则返回收到字节数
// 说明    ：
// =============================================================================
s32 recvfrom(s32 sockfd,void *buf, s32 len, u32 flags,\
            struct sockaddr *addr, s32 *addrlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    if((NULL != buf)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__recvfrom))
        {
            result = sock->ProtocolOps->__recvfrom(sock, buf, len, flags,addr, addrlen);
            if(result < 0)
            {
                errno = EAGAIN;
            }
            else if(result == 0)
            {
                errno = ESHUTDOWN;
            }
            else
            {
                errno =EN_NEWLIB_NO_ERROR;
            }
        }
        else
        {
            errno = ENOTSOCK;
        }
    }

    return  result;
}
// =============================================================================
// 函数功能：  shutdown
//           关闭套接字通道
// 输入参数：  sockfd,目的套接字
//           how,关闭方法， 见SHUT_RD等定义
// 输出参数：
// 返回值  ：false失败  true 成功
// 说明    ：
// =============================================================================
bool_t  shutdown(s32 sockfd, u32 how)
{
    bool_t  result;
    struct tagSocket *sock;

    result = false;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__shutdown))
    {
        result = sock->ProtocolOps->__shutdown(sock, how);
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}

// =============================================================================
// 函数功能：  close
//           关闭套接字
// 输入参数：  sockfd,目的套接字
// 输出参数：
// 返回值  ：false失败  true 成功
// 说明    ：
// =============================================================================
bool_t closesocket(s32 sockfd)
{
    bool_t  result;
    struct tagSocket *sock;

    result = false;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__close))
    {
        result = sock->ProtocolOps->__close(sock);
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}

// =============================================================================
// 函数功能：  setsockopt
//        设置套接字选项
// 输入参数：  sockfd,目的套接字
//        level,设置的层次，支持SOL_SOCKET、IPPROTO_TCP、IPPROTO_IP和IPPROTO_IPV6
//        optname,需设置选项，例如 SO_NONBLOCK
//        optval,选项参数缓冲区
//        optlen,缓冲区长度
// 输出参数：
// 返回值  ：0 成功 -1失败
// 说明    ：
// =============================================================================
s32 setsockopt(s32 sockfd, s32 level, s32 optname,\
               const void *optval, s32 optlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__setsockopt))
    {
        result = sock->ProtocolOps->__setsockopt(sock, level, optname,\
                                            optval,optlen);
    }
    else
    {
        errno = ENOTSOCK;
    }

    return  result;
}
// =============================================================================
// 函数功能：  getsockopt
//        设置套接字选项
// 输入参数：  sockfd,目的套接字
//        level,设置的层次例如，支持的层次有SOL_SOCKET、IPPROTO_TCP
//        optname,需设置选项
//        optval,缓冲区
//        optlen,缓冲区长度
// 输出参数：
// 返回值  ：0 成功 -1失败
// 说明    ：
// =============================================================================
s32 getsockopt(s32 sockfd, s32 level, s32 optname, void *optval,\
               s32 *optlen)
{
    s32  result;
    struct tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__getsockopt))
    {
        result = sock->ProtocolOps->__getsockopt(sock, level, optname,\
                                            optval,optlen);
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}

//返回以前设置的app关联数据
ptu32_t socket_SetUserTag(s32 sockfd, ptu32_t UserTag)
{
    ptu32_t result;
    struct tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
        sock->SockUserTag = UserTag;
    }

    return result;
}
ptu32_t socket_GetUserTag(s32 sockfd)
{
    ptu32_t result;
    struct tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
    }

    return result;
}

//------------------------------------------------------------------------------
//功能：取 socket 的本地端口和IP地址
//参数：sockfd，socket的文件标识符
//      addr，接收地址的缓冲区
//      addrlen，缓冲区长度
//返回：0=成功
//------------------------------------------------------------------------------
s32 getsockname(s32 sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    s32 result = -1;
    struct tagSocket *sock;
    struct sockaddr_in *addrin;

    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = 0;
        addrin =(struct sockaddr_in*)addr;

        addrin->sin_family = sock->af_inet;
        addrin->sin_addr.s_addr = sock->element.v4.iplocal;
        addrin->sin_port = sock->element.v4.portlocal;

        *addrlen = sizeof(struct sockaddr_in);
    }

    return result;
}

//------------------------------------------------------------------------------
//功能：取 socket 的远程端口和IP地址
//参数：sockfd，socket的文件标识符
//      addr，接收地址的缓冲区
//      addrlen，缓冲区长度
//------------------------------------------------------------------------------
s32 getpeername(s32 sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    s32 result = -1;
    struct tagSocket *sock;
    struct sockaddr_in *addrin;

    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = 0;
        addrin =(struct sockaddr_in*)addr;
        addrin->sin_family = sock->af_inet;
        addrin->sin_addr.s_addr = sock->element.v4.ipremote;
        addrin->sin_port = sock->element.v4.portremote;
        *addrlen = sizeof(struct sockaddr_in);
    }

    return result;
}

void sockinfo(s32 sockfd,char *filter)
{
    struct tagSocket *sock;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        if(NULL != sock->ProtocolOps->__debuginfo)
        {
            sock->ProtocolOps->__debuginfo(sock,filter);
        }
        else
        {
            printf("%s:sockfd:%d :no debug function\r\n",__FUNCTION__,sockfd);
        }
    }

    return ;
}

bool_t sockallinfo(char *param)
{
    struct objhandle *Current = NULL;
    while(1)
    {
        Current = OBJ_ForeachHandle(Current, s_ptSocketObject);
        if(Current != NULL)
            sockinfo(Handle2fd(Current), param);
        else
            break;
    }

    return true;
}

//----socket文件操作函数-------------------------------------------------------
//功能：实现读写等功能。
//参数：Target，参考函数指针类型 fnObjOps 的定义
//      cmd，命令码，参见 CN_OBJ_CMD_OPEN 族定义
//      para1，para2：与命令码相关
//返回：与命令码相关
//todo:待完成，lst
//-----------------------------------------------------------------------------
s32 Socket_ObjOps(void *opsTarget, u32 opscmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;
    switch(opscmd)
    {
        case CN_OBJ_CMD_READ:
        {
            s32 sockfd;
            sockfd = Handle2fd((struct objhandle *)opsTarget);
            //此处len是有符号数，send 和 recv 函数本身如此
            *(ssize_t *)OpsArgs1 = recv(sockfd, (u8*)OpsArgs2, (s32)OpsArgs3, 0);
        }break;
        case CN_OBJ_CMD_WRITE:
        {
            s32 sockfd;
            sockfd = Handle2fd((struct objhandle *)opsTarget);
            *(ssize_t *)OpsArgs1 = send(sockfd, (void*)OpsArgs2, (s32)OpsArgs3,0);
        }break;
        case CN_OBJ_CMD_SHOW:
        {
            if( sockallinfo((char*)OpsArgs2) )
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
        }break;
        case CN_OBJ_CMD_CLOSE:
        {
            s32 sockfd;
            sockfd = Handle2fd((struct objhandle *)opsTarget);
            if(closesocket(sockfd) == true)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
        }break;
        default:
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
    }
    return result;
}

// =============================================================================
// 函数功能：  Socket_Init
//
// 输入参数：para，目前无意义
// 输出参数：
// 返回值  ：true成功 false失败
// 说明    ：1,建立套接字所需要的内存池
// =============================================================================
bool_t SocketInit(void)
{

    s_ptSocketPool = Mb_CreatePool(NULL, 0, sizeof(struct tagSocket), 10, 1000, "sockmem");
    if(NULL == s_ptSocketPool)
    {
        printf("%s:分配socket控制块内存不足\n\r",__FUNCTION__);
        goto EXIT_SOCKMEM;
    }
    s_ptSocketObject = OBJ_NewChild(OBJ_GetRoot(),Socket_ObjOps, 0, "socketsets");
    if(s_ptSocketObject == NULL)
    {
        printf("%s:创建socket文件失败\n\r",__FUNCTION__);
        goto EXIT_CREATE_OBJ;
    }

//    tcpipmemlog("sockmem",sizeof(struct tagSocket),CFG_SOCKET_NUM);

    return true;
EXIT_CREATE_OBJ:
    Mb_DeletePool(s_ptSocketPool);
EXIT_SOCKMEM:
    return false;
}

