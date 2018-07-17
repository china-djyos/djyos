//-----------------------------------------------------------------------------
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
#include  <errno.h>
#include "../component_config_tcpip.h"

#include  "../common/tpl.h"
#include "sockfile.h"
#include "dbug.h"
typedef tagSocket  tagItemCB; //each socket is a item

//use this function to get a item
tagItemCB *socket_malloc(void)
{
    void *sockobj = NULL;
    tagItemCB *ret= NULL;
    ret = net_malloc(sizeof(tagItemCB));
    if(NULL == ret)
    {
        debug_printf("socket","%s:MEM ERR\n\r",__FUNCTION__);
        goto EXIT_MALLOC;
    }
    memset(ret,0,sizeof(tagItemCB));
    ret->sync = mutex_init(NULL);
    if(NULL == ret->sync)
    {
        debug_printf("socket","%s:MUTEX ERR\n\r",__FUNCTION__);
        goto EXIT_MUTEX;
    }
    sockobj = sockobj_malloc(NULL);
    if(NULL == sockobj)
    {
        debug_printf("socket","%s:OBJ ERR\n\r",__FUNCTION__);
        goto EXIT_SOCKOBJ;
    }
    //set the context to the object
    sockobj_setcontext(sockobj,ret);
    ret->obj = sockobj;

    return ret;

EXIT_SOCKOBJ:
    mutex_del(ret->sync);
    ret->sync = NULL;
EXIT_MUTEX:
    net_free(ret);
    ret = NULL;
EXIT_MALLOC:
    return ret;
}

bool_t socket_free(tagItemCB *ret)
{
    void *sockobj;
    //remove the obj
    sockobj = ret->obj;
    sockobj_setcontext(ret->obj,NULL);
    sockobj_free(NULL,sockobj);
    mutex_del(ret->sync);
    ret->sync = NULL;
    net_free(ret);
    return true;
}

bool_t socket_setstatus(tagItemCB *sock,u32 status)
{
    sock->iostat |= status;
    sockobj_setstatus(sock->obj,status);
    return true;
}
bool_t socket_clrsatus(tagItemCB *sock,u32 status)
{
    sock->iostat &= (~status);
    sockobj_clrstatus(sock->obj,status);

    return true;
}

//this used to map the socket controller and its file handle
static tagItemCB *__socket_fd2sock(int fd)
{
    tagItemCB *ret = NULL;
    void *sockobj;
    sockobj = sockobj_obj(fd);
    if(NULL != sockobj)
    {
        ret = sockobj_getcontext(sockobj);
    }
    return ret;
}

static int __socket_sock2fd(tagItemCB *sock)
{
    if(NULL != sock)
    {
        return sockobj_fd(sock->obj);
    }
    else
    {
        return -1;
    }
}

// =============================================================================
// 函数功能：  socket
//        create an new socket
// 输入参数： family,所属域，如AF_INET AF_LOCAL等
//        type,类型，如SOCK_STREAM SOCK_DGRAM等
//        protocol,协议类型，一般的默认为0
// 输出参数：
// 返回值  ：套接字号，-1表示创建失败
// 说明    ：套接字创建不成功，可能是指定协议没有找到或者内存不足
// =============================================================================
//创建一个套接字
int socket(int family, int type, int protocol)
{
    int ret = -1;
    tagTlayerProto *proto = NULL;
    tagItemCB *sock = NULL;
    proto = TPL_GetProto(family, type, protocol);
    if(NULL != proto)
    {
        if(NULL != proto->socket)
        {
            sock = proto->socket(family, type, protocol);
            if(sock == NULL)
            {
                errno =EPROTOTYPE;
            }
            else
            {
                socket_setstatus(sock,sock->iostat);
                ret = __socket_sock2fd(sock);
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
    return  ret;
}

// =============================================================================
// 函数功能：  bind
//           为一个套接字绑定一个网络地址
// 输入参数：  sockfd,待绑定的套接字
//           myaddr,指定的网络地址
//           addrlen,网络地址长度
// 输出参数：
// 返回值  ：0 成功  -1失败
// 说明    ：失败一般的是因为该网络地址冲突（端口号）
// =============================================================================
//创建一个套接字
int bind(int sockfd,struct sockaddr *addr, int addrlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->bind))
    {
        result = sock->proto->bind(sock, addr, addrlen);
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
//           backlog,允许的链接个数，默认的为5
// 输出参数：
// 返回值  ：0 成功  -1失败
// 说明    ：失败一般的是因为重复指定
// =============================================================================
int listen(int sockfd, int backlog)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->listen))
    {
        result = sock->proto->listen(sock, backlog);
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
// 输出参数：  addr,连接的客户端地址
//           addrlen，地址长度
// 返回值  ：链接的客户端套接字，-1出错
// 说明    ：出错一般是因为非阻塞而且没有连接的客户端，或者超出链接限制数
// =============================================================================
int accept(int sockfd, struct sockaddr *addr, int *addrlen)
{
    tagItemCB *sock;
    tagItemCB *result;
    int        client = -1;

    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->accept))
    {
        result = sock->proto->accept(sock, addr,addrlen);
        client = __socket_sock2fd(result);
        if(client < 0)
        {
            errno = EAGAIN;
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
int connect(int sockfd, struct sockaddr *addr, int addrlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->connect))
    {
        result = sock->proto->connect(sock, addr, addrlen);
        if(errno < 0)
        {
            errno = ECONNREFUSED;
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
int send(int sockfd, const void *msg, int len, int flags)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __socket_fd2sock(sockfd);
        if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->send))
        {
            result = sock->proto->send(sock, msg, len, flags);
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
//the send or recv exact function used to deal the application proto
//which has negotiate the data structure
//we use the function to send the exact messages
//if return true, then the message has been sent in to the buffer at least
//if return false, then the socket must has some err
bool_t sendexact(int sock,u8 *buf,int len)
{
//  bool_t ret=true;
    int sentlen;
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
bool_t recvexact(int sock,u8 *buf,int len)
{
    bool_t result;
    int recvlen;

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
int recv(int sockfd, void *buf,int len, unsigned int flags)
{
    int  result;
    tagItemCB *sock;

    result = -1;

    if((NULL != buf)&&(len > 0))
    {
        sock = __socket_fd2sock(sockfd);
        if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->recv))
        {
            result = sock->proto->recv(sock, buf, len, flags);
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
int sendto(int sockfd, const void *msg,int len, unsigned int flags,\
          const struct sockaddr *addr, int addrlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __socket_fd2sock(sockfd);
        if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->sendto))
        {
            result = sock->proto->sendto(sock, msg, len, flags,addr, addrlen);
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
// 函数功能：  recvfrom
//           从目的网络地址读取数据
// 输入参数：  sockfd,目的套接字
//           flags,一般为0
//           addr,目的网络地址
//           addrlen,目的网络地址长度
// 输出参数：
//           buf,读取的数据
//           len,信息长度
// 返回值  ：-1出错，否则返回收到字节数
// 说明    ：
// =============================================================================
int recvfrom(int sockfd,void *buf, int len, unsigned int flags,\
            struct sockaddr *addr, int *addrlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    if((NULL != buf)&&(len > 0))
    {
        sock = __socket_fd2sock(sockfd);
        if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->recvfrom))
        {
            result = sock->proto->recvfrom(sock, buf, len, flags,addr, addrlen);
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
bool_t  shutdown(int sockfd, u32 how)
{
    bool_t  result;
    tagItemCB *sock;

    result = false;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->shutdown))
    {
        result = sock->proto->shutdown(sock, how);
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
bool_t closesocket(int sockfd)
{
    bool_t  result;
    tagItemCB *sock;

    result = false;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->close))
    {
        result = sock->proto->close(sock);
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
//        optname,需设置选项
//        optval,缓冲区
//        optlen,缓冲区长度
// 输出参数：
// 返回值  ：0 成功 -1失败
// 说明    ：
// =============================================================================
int setsockopt(int sockfd, int level, int optname,\
               const void *optval, int optlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->setsockopt))
    {
        result = sock->proto->setsockopt(sock, level, optname,\
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
int getsockopt(int sockfd, int level, int optname, void *optval,\
               int *optlen)
{
    int  result;
    tagItemCB *sock;

    result = -1;
    sock = __socket_fd2sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->getsockopt))
    {
        result = sock->proto->getsockopt(sock, level, optname,\
                                            optval,optlen);
    }
    else
    {
        errno = ENOTSOCK;
    }
    return  result;
}

//返回以前设置的app关联数据
ptu32_t socket_setprivate(int sockfd, ptu32_t private)
{
    ptu32_t result;
    tagItemCB *sock;

    result =(ptu32_t)NULL;
    sock = __socket_fd2sock(sockfd);
    if(NULL != sock)
    {
        result = sock->private;
        sock->private = private;
    }

    return result;
}
ptu32_t socket_getprivate(int sockfd)
{
    ptu32_t result;
    tagItemCB *sock;

    result =(ptu32_t)NULL;
    sock = __socket_fd2sock(sockfd);
    if(NULL != sock)
    {
        result = sock->private;
    }

    return result;
}
int getsockname(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int result = -1;
    tagItemCB *sock;
    struct sockaddr_in *addrin;

    sock = __socket_fd2sock(sockfd);
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

int getpeername(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int result = -1;
    tagItemCB *sock;
    struct sockaddr_in *addrin;

    sock = __socket_fd2sock(sockfd);
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


int issocketactive(int sockfd, int mode)
{
    int  result = 0;
    tagItemCB *sock;

    result = false;
    sock = __socket_fd2sock(sockfd);
    if(NULL != sock)
    {
        if(mode &sock->iostat)
        {
            result = 1;
        }
    }
    return  result;
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
    sockobj_init(CFG_SOCKET_NUM);
    return true;
}

