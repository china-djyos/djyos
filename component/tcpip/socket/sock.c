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
typedef struct Socket  tagItemCB; //each socket is a item
static struct obj *s_ptSocketObject;
static struct MemCellPool *s_ptSocketPool;  //socket���ƿ��ڴ��ͷָ��

static struct Socket *__Fd2Sock(s32 fd)
{
    struct objhandle *hdl;
    struct Socket *sock;

    hdl = fd2Handle(fd);
    sock = (struct Socket *)handle_context(hdl);
    return sock;
}

struct Socket *SocketBuild(void)
{
    struct objhandle *SocketFile;
    struct Socket *sock;
    sock = Mb_Malloc(s_ptSocketPool, 0);
    if(sock)
    {
        memset(sock, 0, sizeof(struct Socket));
        sock->SockSync = mutex_init(NULL);
        SocketFile = handle_new( );
        if(SocketFile != NULL)
        {
            handle_init(SocketFile, s_ptSocketObject, O_RDWR,(ptu32_t)sock);
            sock->sockfd = Handle2fd(SocketFile);
//          sock = Handle2fd(SocketFile);
//          sock->sockfd = sock;
//          sock->ProtocolOps = ProtocolOps;
//          handle_SetMultiplexEvent(SocketFile, sock->IoInitstat);
        }
        else
        {
//          errno = ENOENT;
            Mb_Free(s_ptSocketPool, sock);
        }
    }
    return sock;
}

bool_t SocketFree(struct Socket *sock)
{
    handle_Delete(fd2Handle(sock->sockfd));
    Mb_Free(s_ptSocketPool, sock);
    return true;
}

//-----------------------------------------------------------------------------
//����: ����һ���׽��֣��ȼ�鴫���Э�麯�����Ƿ��Ѿ�ע�ᣬȻ����ô����� socket ����
//     ����socket����  s_ptSocketObject�����´��� handle ��socket��Ϊhandle��˽�г�Ա��
//����: family����ַ�壬��socket.h�� AF_INET �ȶ���
//      type��socket Э�����ͣ���socket.h�� SOCK_DGRAM �ȶ���
//      protocol������Э�飬��socket.h�� IPPROTO_UDP �ȶ���
//����: �������ƿ�ָ��
//-----------------------------------------------------------------------------
s32 socket(s32 family, s32 type, s32 protocol)
{
    s32 result = -1;
    struct TPL_ProtocalOps  *ProtocolOps = NULL;
    struct Socket       *sock = NULL;
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
//              SocketFile = handle_new( );
//              SocketFile = OBJ_AddFile(s_ptSocketObject);
//              if(SocketFile != NULL)
//              {
//                  handle_init(SocketFile, s_ptSocketObject, O_RDWR,(ptu32_t)sock);
//                  handle_SetContext(SocketFile, (ptu32_t)sock);
//                  result = Handle2fd(SocketFile);
//                  sock->sockfd = result;
//                  sock->ProtocolOps = ProtocolOps;
//                  handle_SetMultiplexEvent(fd2Handle(sock->sockfd), sock->IoInitstat);
//              }
//              else
//              {
//                  errno = ENOENT;
//                  closesocket(result);
//              }
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
//����: Ϊһ���׽��ְ�һ�������ַ
//����: sockfd,���󶨵��׽���
//      myaddr,ָ���������ַ
//      addrlen,�����ַ����
//���أ�0 �ɹ�  -1ʧ��
//˵����ʧ��һ�������Ϊ�������ַ��ͻ���˿ںţ�
//-----------------------------------------------------------------------------
s32 bind(s32 sockfd,struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct Socket *sock;

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
// �������ܣ�  listen
//           ָ���׽���Ϊ����״̬
// ���������  sockfd,��ָ�����׽���
//           backlog,��������Ӹ�����Ĭ�ϵ�Ϊ5
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��ʧ��һ�������Ϊ�ظ�ָ��
// =============================================================================
s32 listen(s32 sockfd, s32 backlog)
{
    s32  result;
    struct Socket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__listen))
    {
        result = sock->ProtocolOps->__listen(sock, backlog);
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
// ���������  addr,���ڽ������ӵĿͻ��˵�ַ
//           addrlen�����ڽ��յ�ַ����
// ����ֵ  �����ӵĿͻ����׽��֣�-1����
// ˵��    ������һ������Ϊ����������û�����ӵĿͻ��ˣ����߳�������������
// =============================================================================
s32 accept(s32 sockfd, struct sockaddr *addr, s32 *addrlen)
{
    struct Socket *sock;
    struct Socket *result;
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
// �������ܣ�  connect
//           ���ӷ�����
// ���������  sockfd,�ͻ����׽���
//           addr,���ӵķ�������ַ
//           addrlen����ַ����
// ���������
// ����ֵ  ��-1����0�ɹ�
// ˵��    ��
// =============================================================================
s32 connect(s32 sockfd, struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct Socket *sock;

    result = -1;
    sock = __Fd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->ProtocolOps)&&(NULL != sock->ProtocolOps->__connect))
    {
        result = sock->ProtocolOps->__connect(sock, addr, addrlen);
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
s32 send(s32 sockfd, const void *msg, s32 len, s32 flags)
{
    s32  result;
    struct Socket *sock;

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
s32 recv(s32 sockfd, void *buf,s32 len, u32 flags)
{
    s32  result;
    struct Socket *sock;

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
s32 sendto(s32 sockfd, const void *msg,s32 len, u32 flags,\
          const struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    struct Socket *sock;

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
s32 recvfrom(s32 sockfd,void *buf, s32 len, u32 flags,\
            struct sockaddr *addr, s32 *addrlen)
{
    s32  result;
    struct Socket *sock;

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
// �������ܣ�  shutdown
//           �ر��׽���ͨ��
// ���������  sockfd,Ŀ���׽���
//           how,�رշ����� ��SHUT_RD�ȶ���
// ���������
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
// =============================================================================
bool_t  shutdown(s32 sockfd, u32 how)
{
    bool_t  result;
    struct Socket *sock;

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
// �������ܣ�  close
//           �ر��׽���
// ���������  sockfd,Ŀ���׽���
// ���������
// ����ֵ  ��falseʧ��  true �ɹ�
// ˵��    ��
// =============================================================================
bool_t closesocket(s32 sockfd)
{
    bool_t  result;
    struct Socket *sock;

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
s32 setsockopt(s32 sockfd, s32 level, s32 optname,\
               const void *optval, s32 optlen)
{
    s32  result;
    struct Socket *sock;

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
s32 getsockopt(s32 sockfd, s32 level, s32 optname, void *optval,\
               s32 *optlen)
{
    s32  result;
    struct Socket *sock;

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

//������ǰ���õ�app��������
ptu32_t socket_setprivate(s32 sockfd, ptu32_t private)
{
    ptu32_t result;
    struct Socket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
        sock->SockUserTag = private;
    }

    return result;
}
ptu32_t socket_getprivate(s32 sockfd)
{
    ptu32_t result;
    struct Socket *sock;

    result =(ptu32_t)NULL;
    sock = __Fd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->SockUserTag;
    }

    return result;
}
s32 getsockname(s32 sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    s32 result = -1;
    struct Socket *sock;
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

s32 getpeername(s32 sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    s32 result = -1;
    struct Socket *sock;
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
    struct Socket *sock;
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
        Current = obj_ForeachHandle(Current, s_ptSocketObject);
        if(Current != NULL)
            sockinfo(Handle2fd(Current), param);
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
            //�˴�len���з�������send �� recv �����������
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
// �������ܣ�  Socket_Init
//
// ���������para��Ŀǰ������
// ���������
// ����ֵ  ��true�ɹ� falseʧ��
// ˵��    ��1,�����׽�������Ҫ���ڴ��
// =============================================================================
bool_t SocketInit(void)
{

    s_ptSocketPool = Mb_CreatePool(NULL, 0, sizeof(struct Socket), 10, 1000, "sockmem");
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

//    tcpipmemlog("sockmem",sizeof(struct Socket),CFG_SOCKET_NUM);

    return true;
EXIT_CREATE_OBJ:
    Mb_DeletePool(s_ptSocketPool);
EXIT_SOCKMEM:
    return false;
}

