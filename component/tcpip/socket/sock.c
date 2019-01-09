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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include  <dirent.h>
#include  <errno.h>
#include "../component_config_tcpip.h"
#include  <fcntl.h>
#include  "../common/tpl.h"
#include "sockfile.h"
#include "dbug.h"
typedef tagSocket  tagItemCB; //each socket is a item
static struct obj *s_ptSocketObject;
static struct MemCellPool *s_ptSocketPool;  //socket���ƿ��ڴ��ͷָ��

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

//������ǰ���õ�app��������
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
// FUNCTION��CHECK WHETHER THE SOCKET IS ACTIVE
// PARA  IN��mode:
// PARA OUT��
// RETURN  ��
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

//----socket�ļ���������-------------------------------------------------------
//���ܣ�ʵ�ֶ�д�ȹ��ܡ�
//������Target���ο�����ָ������ fnObjOps �Ķ���
//      cmd�������룬�μ� CN_OBJ_CMD_OPEN �嶨��
//      para1��para2�������������
//���أ������������
//todo:����ɣ�lst
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
            //�˴�len���з�������send �� recv �����������
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
// �������ܣ�  Socket_Init
//
// ���������para��Ŀǰ������
// ���������
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��1,�����׽�������Ҫ���ڴ��
// =============================================================================
bool_t SocketInit(ptu32_t para)
{

    s_ptSocketPool = Mb_CreatePool(NULL, CFG_SOCKET_NUM, sizeof(tagSocket), 10, 1000, "sockmem");
    if(NULL == s_ptSocketPool)
    {
        printf("%s:����socket���ƿ��ڴ治��\n\r",__FUNCTION__);
        goto EXIT_SOCKMEM;
    }
    s_ptSocketObject = obj_newchild(obj_root(),Socket_ObjOps, 0, "socket");
    if(s_ptSocketObject == NULL)
    {
        printf("%s:����socket�ļ�ʧ��\n\r",__FUNCTION__);
        goto EXIT_CREATE_OBJ;
    }

//    tcpipmemlog("sockmem",sizeof(tagSocket),CFG_SOCKET_NUM);

    return true;
EXIT_CREATE_OBJ:
    Mb_DeletePool(s_ptSocketPool);
EXIT_SOCKMEM:
    return false;
}

