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
#include "sockfile.h"
#include "dbug.h"
typedef tagSocket  tagItemCB; //each socket is a item
static struct obj *s_ptSocketObject;
static struct MemCellPool *s_ptSocketPool;  //socket控制块内存池头指针

tagSocket *SocketMalloc(void)
{
    return Mb_Malloc(s_ptSocketPool, 0);
}

bool_t SocketFree(tagSocket *sock)
{
    Mb_Free(s_ptSocketPool, sock);
    return true;
}

static tagSocket *__Fd2Sock(int fd)
{
    struct objhandle *hdl;
    tagSocket *sock;

    hdl = fd2Handle(fd);
    sock = handle_context(hdl);
    return sock;
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
    struct obj *NewObj;
    struct objhandle *SocketFile;
    int result = -1;
    struct TPL_ProtocalOps  *ProtocolOps = NULL;
    tagSocket       *sock = NULL;
    ProtocolOps = TPL_GetProto(family, type, protocol);
    if(NULL != ProtocolOps)
    {
        if(NULL != ProtocolOps->socket)
        {
            sock = ProtocolOps->socket(family, type, protocol);
            if(sock == NULL)
            {
                errno =EPROTOTYPE;
            }
            else
            {
                errno = EN_NEWLIB_NO_ERROR;
                SocketFile = handle_new( );
//              SocketFile = OBJ_AddFile(s_ptSocketObject);
                if(SocketFile != NULL)
                {
                    handle_init(SocketFile, s_ptSocketObject, O_RDWR,sock);
//                  handle_SetContext(SocketFile, (ptu32_t)sock);
                    result = Handle2fd(SocketFile);
                    sock->sockfd = result;
                    handle_SetMultiplexEvent(SocketFile, sock->IoInitstat);
                }
                else
                {
                    errno = ENOENT;
                    closesocket(result);
                }
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
    tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->bind))
    {
        result = sock->ProtocolOps->bind(sock, addr, addrlen);
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
    tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->listen))
    {
        result = sock->ProtocolOps->listen(sock, backlog);
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
    tagSocket *sock;
    tagSocket *result;
    int        client = -1;

    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->accept))
    {
        result = sock->ProtocolOps->accept(sock, addr,addrlen);
        client = result->sockfd;
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
    tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->connect))
    {
        result = sock->ProtocolOps->connect(sock, addr, addrlen);
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
    tagSocket *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->send))
        {
            result = sock->ProtocolOps->send(sock, msg, len, flags);
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
    tagSocket *sock;

    result = -1;

    if((NULL != buf)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->recv))
        {
            result = sock->ProtocolOps->recv(sock, buf, len, flags);
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
    tagSocket *sock;

    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->sendto))
        {
            result = sock->ProtocolOps->sendto(sock, msg, len, flags,addr, addrlen);
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
    tagSocket *sock;

    result = -1;
    if((NULL != buf)&&(len > 0))
    {
        sock = __Fd2Sock(sockfd);
        if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->recvfrom))
        {
            result = sock->ProtocolOps->recvfrom(sock, buf, len, flags,addr, addrlen);
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
    tagSocket *sock;

    result = false;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->shutdown))
    {
        result = sock->ProtocolOps->shutdown(sock, how);
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
    tagSocket *sock;

    result = false;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->close))
    {
        result = sock->ProtocolOps->close(sock);
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
    tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->setsockopt))
    {
        result = sock->ProtocolOps->setsockopt(sock, level, optname,\
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
    tagSocket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->getsockopt))
    {
        result = sock->ProtocolOps->getsockopt(sock, level, optname,\
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
    tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
        sock->SockUserTag = private;
    }

    return result;
}
ptu32_t socket_getprivate(int sockfd)
{
    ptu32_t result;
    tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
    }

    return result;
}
int getsockname(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int result = -1;
    tagSocket *sock;
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

int getpeername(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int result = -1;
    tagSocket *sock;
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

// =============================================================================
// FUNCTION：CHECK WHETHER THE SOCKET IS ACTIVE
// PARA  IN：mode:
// PARA OUT：
// RETURN  ：
// INSTRUCT:
// =============================================================================
//#define  CN_SOCKET_IOREAD      CN_MULTIPLEX_SENSINGBIT_READ   //COULE  READ
//#define  CN_SOCKET_IOWRITE     CN_MULTIPLEX_SENSINGBIT_WRITE  //COULD  WRITE, which means the
//#define  CN_SOCKET_IOERR       CN_MULTIPLEX_SENSINGBIT_ERROR  //SOCKET ERROR
//#define  CN_SOCKET_IOACCEPT    (CN_SOCKET_IOERR<<1)           //COULD  ACCEPT
//#define  CN_SOCKET_IOOR        CN_MULTIPLEX_SENSINGBIT_OR     //BITS OR TRIGGLE
int issocketactive(int sockfd, int mode)
{
    int  result = 0;
    tagSocket *sock;

    result = false;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        if(mode &sock->sockstat)
        {
            result = 1;
        }
    }
    return  result;
}

void sockinfo(int sockfd,char *filter)
{
    tagSocket *sock;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        if(NULL != sock->ProtocolOps->debuginfo)
        {
            sock->ProtocolOps->debuginfo(sock,filter);
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
        Current = obj_ForeachHandle(Current, s_ptSocketObject);
        if(Current != NULL)
            sockinfo((tagSocket *)handle_context(Current), param);
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
ptu32_t Socket_ObjOps(void *Target, u32 cmd, ptu32_t para1, ptu32_t para2)
{
    s32 result = 0;
    switch(cmd)
    {
        case CN_OBJ_CMD_READ:
        {
            s32 sockfd;
            sockfd = Handle2fd((struct objhandle *)Target);
            //此处len是有符号数，send 和 recv 函数本身如此
            return recv(sockfd, (u8*)para1, (s32)para2, 0);
        }break;
        case CN_OBJ_CMD_WRITE:
        {
            s32 sockfd;
            sockfd = Handle2fd((struct objhandle *)Target);
            return sendexact(sockfd, (void*)para1, (s32)para2);
        }break;
        case CN_OBJ_CMD_SHOW:
        {
            sockallinfo((char*)para1);
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
bool_t SocketInit(ptu32_t para)
{

    s_ptSocketPool = Mb_CreatePool(NULL, CFG_SOCKET_NUM, sizeof(tagSocket), 10, 1000, "sockmem");
    if(NULL == s_ptSocketPool)
    {
        printf("%s:分配socket控制块内存不足\n\r",__FUNCTION__);
        goto EXIT_SOCKMEM;
    }
    s_ptSocketObject = obj_newchild(obj_root(),Socket_ObjOps, 0, "socket");
    if(s_ptSocketObject == NULL)
    {
        printf("%s:创建socket文件失败\n\r",__FUNCTION__);
        goto EXIT_CREATE_OBJ;
    }

//    tcpipmemlog("sockmem",sizeof(tagSocket),CFG_SOCKET_NUM);

    return true;
EXIT_CREATE_OBJ:
    Mb_DeletePool(s_ptSocketPool);
EXIT_SOCKMEM:
    return false;
}

