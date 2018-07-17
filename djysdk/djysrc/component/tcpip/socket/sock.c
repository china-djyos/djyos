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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
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
// �������ܣ�  socket
//        create an new socket
// ��������� family,��������AF_INET AF_LOCAL��
//        type,���ͣ���SOCK_STREAM SOCK_DGRAM��
//        protocol,Э�����ͣ�һ���Ĭ��Ϊ0
// ���������
// ����ֵ  ���׽��ֺţ�-1��ʾ����ʧ��
// ˵��    ���׽��ִ������ɹ���������ָ��Э��û���ҵ������ڴ治��
// =============================================================================
//����һ���׽���
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
// �������ܣ�  bind
//           Ϊһ���׽��ְ�һ�������ַ
// ���������  sockfd,���󶨵��׽���
//           myaddr,ָ���������ַ
//           addrlen,�����ַ����
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�������ַ��ͻ���˿ںţ�
// =============================================================================
//����һ���׽���
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
// �������ܣ�  listen
//           ָ���׽���Ϊ����״̬
// ���������  sockfd,��ָ�����׽���
//           backlog,��������Ӹ�����Ĭ�ϵ�Ϊ5
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�ظ�ָ��
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
// �������ܣ�  accept
//           �ȴ��ͻ��˵�����
// ���������  sockfd,���������׽���
// ���������  addr,���ӵĿͻ��˵�ַ
//           addrlen����ַ����
// ����ֵ  �����ӵĿͻ����׽��֣�-1����
// ˵��    ������һ������Ϊ����������û�����ӵĿͻ��ˣ����߳�������������
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
// �������ܣ�  connect
//           ���ӷ�����
// ���������  sockfd,�ͻ����׽���
//           addr,���ӵķ�������ַ
//           addrlen����ַ����
// ���������
// ����ֵ  ��-1����0�ɹ�
// ˵��    ��
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
// �������ܣ�  send
//           ���׽��ַ�������
// ���������  sockfd,Ŀ���׽���
//           msg,�����͵���Ϣ
//           len,��Ϣ����
//           flags,һ��Ϊ0
// ���������
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
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
// �������ܣ�  recv
//           ��ȡ��������
// ���������  sockfd,Ŀ���׽���
//           flags,һ��Ϊ0
// ���������
//           buf,�յ�����Ϣ
//           len,buf����
// ����ֵ  ��-1�������򷵻��յ��ֽ���
// ˵��    ��
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
// �������ܣ�  sendto
//           ��Ŀ�������ַ��������
// ���������  sockfd,Ŀ���׽���
//           msg,�����͵���Ϣ
//           len,��Ϣ����
//           flags,һ��Ϊ0
//           addr,Ŀ�������ַ
//           addrlen,Ŀ�������ַ����
// ���������
// ����ֵ  ��-1�������򷵻ط����ֽ���
// ˵��    ��
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
// �������ܣ�  recvfrom
//           ��Ŀ�������ַ��ȡ����
// ���������  sockfd,Ŀ���׽���
//           flags,һ��Ϊ0
//           addr,Ŀ�������ַ
//           addrlen,Ŀ�������ַ����
// ���������
//           buf,��ȡ������
//           len,��Ϣ����
// ����ֵ  ��-1�������򷵻��յ��ֽ���
// ˵��    ��
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
// �������ܣ�  shutdown
//           �ر��׽���ͨ��
// ���������  sockfd,Ŀ���׽���
//           how,�رշ����� ��SHUT_RD�ȶ���
// ���������
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
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
// �������ܣ�  close
//           �ر��׽���
// ���������  sockfd,Ŀ���׽���
// ���������
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
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
// �������ܣ�  setsockopt
//        �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//        level,���õĲ�Σ�֧��SOL_SOCKET��IPPROTO_TCP��IPPROTO_IP��IPPROTO_IPV6
//        optname,������ѡ��
//        optval,������
//        optlen,����������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
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
// �������ܣ�  getsockopt
//        �����׽���ѡ��
// ���������  sockfd,Ŀ���׽���
//        level,���õĲ�����磬֧�ֵĲ����SOL_SOCKET��IPPROTO_TCP
//        optname,������ѡ��
//        optval,������
//        optlen,����������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
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

//������ǰ���õ�app��������
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
// �������ܣ�  Socket_Init
//
// ���������para��Ŀǰ������
// ���������
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��1,�����׽�������Ҫ���ڴ��
// =============================================================================
bool_t SocketInit(void)
{
    sockobj_init(CFG_SOCKET_NUM);
    return true;
}

