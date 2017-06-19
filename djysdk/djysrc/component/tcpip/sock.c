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
#include  "tpl.h"
#include  "tcpipconfig.h"

//�׽����ڴ��
#define CN_SOCK_FREE            (1<<31)      //IF THIS SOCKET IS FREE, SET THIS BIT
#define CN_SOCK_FDMASK          (0x7FFFFFFF) //USE TO MASK THE SOCK FD
#define CN_SOCKET_BASEFD        0X200
static struct MutexLCB         *pSocketMemSync = NULL;   //used to protect the mem
static struct MutexLCB          gSocketMemSyncMem;
static tagSocket               *pgSocketMemHead = NULL;   //the socket mem

tagSocket *SocketMalloc(void)
{
    u32 i = 0;
    tagSocket *result = NULL;
    if(Lock_MutexPend(pSocketMemSync,CN_TIMEOUT_FOREVER))
    {
        while(i < gSockNum)
        {
            if(CN_SOCK_FREE&pgSocketMemHead[i].sockfd)
            {
                result = &pgSocketMemHead[i];
                result->sync = Lock_MutexCreate_s(&result->syncmem,NULL);
                result->sockfd = i +CN_SOCKET_BASEFD;
                break;
            }
            i++;
        }
        Lock_MutexPost(pSocketMemSync);
    }
    return result;
}

bool_t SocketFree(tagSocket *sock)
{
    bool_t result = false;
    if(Lock_MutexPend(pSocketMemSync,CN_TIMEOUT_FOREVER))
    {
    	Lock_MutexDelete_s(sock->sync);
        memset((void *)sock,0, sizeof(tagSocket));
        sock->sockfd = CN_SOCK_FREE ;
        result = true;

        Lock_MutexPost(pSocketMemSync);
    }
    return result;
}

static tagSocket *__SocketMapFd2Sock(int fd)
{
    tagSocket *result = NULL;
    if((fd >=CN_SOCKET_BASEFD)&&(fd <(CN_SOCKET_BASEFD+CN_SOCKET_BASEFD)))
    {
        result = pgSocketMemHead + (fd-CN_SOCKET_BASEFD);
        if(result->sockfd&CN_SOCK_FREE)
        {
            result = NULL;
        }
    }
    return result;
}
static int __SocketMapSock2Fd(tagSocket *sock)
{
    if(NULL != sock)
    {
        return sock->sockfd;
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
	int result = -1;
    tagTlayerProto  *proto = NULL;
    tagSocket       *sock = NULL;
    proto = TPL_GetProto(family, type, protocol);
    if(NULL != proto)
    {
        if(NULL != proto->socket)
        {
            sock = proto->socket(family, type, protocol);
            result = __SocketMapSock2Fd(sock);
            if(result < 0)
            {
            	errno =EPROTOTYPE;
            }
            else
            {
            	errno = EN_NEWLIB_NO_ERROR;
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
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    tagSocket *result;
    int        client = -1;
    
    sock = __SocketMapFd2Sock(sockfd);
    if((NULL != sock)&&(NULL != sock->proto)&&(NULL != sock->proto->accept))
    {
        result = sock->proto->accept(sock, addr,addrlen);
        client = __SocketMapSock2Fd(result);
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
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __SocketMapFd2Sock(sockfd);
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
	bool_t result;
	int sentlen;

	sentlen = 0;
	result = true;
	while(len > 0)
	{
		sentlen = send(sock,buf,len,0);
		if(sentlen > 0)
		{
			len -= sentlen;
			buf += sentlen;
		}
		else if(sentlen == 0)
		{
			result = false;
			break; //maybe not permit to send any data
		}
		else
		{
			//maybe timeout continue
		}
	}
	return result;
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
        sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    if((NULL != msg)&&(len > 0))
    {
        sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    if((NULL != buf)&&(len > 0))
    {
        sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = false;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;

    result = false;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;
    
    result = -1;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __SocketMapFd2Sock(sockfd);
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
    tagSocket *sock;

    result =(ptu32_t)NULL;
    sock = __SocketMapFd2Sock(sockfd);
    if(NULL != sock)
    {
        result = sock->private;
    }

    return result;
}
int getsockname(int sockfd,struct sockaddr *addr,socklen_t *addrlen)
{
    int result = -1;
    tagSocket *sock;
    struct sockaddr_in *addrin;

    sock = __SocketMapFd2Sock(sockfd);
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

    sock = __SocketMapFd2Sock(sockfd);
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
// �������ܣ�����׽��ֵ�ָ���Ķ��ø��ü���     
// ���������  MultiplexSets,ָ�����׽��ּ���
//        sock,��������׽���
//        SensingBit,��Ҫ������λ
// ��������� 
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    ��
// =============================================================================
bool_t Socket_MultiplexAdd(struct MultiplexSetsCB *sets,
                           int sockfd,u32 SensingBit)
{
    bool_t result;
    tagSocket *sock;
    
    result = false;
    sock = __SocketMapFd2Sock(sockfd);
    if((NULL != sock)&&(Lock_MutexPend(sock->sync, CN_TIMEOUT_FOREVER)))
    {
        result = Multiplex_AddObject(sets,&sock->ioselect,sock->iostat,\
                                     sockfd,SensingBit);
        Lock_MutexPost(sock->sync);
    }

    return result;
}
// =============================================================================
// �������ܣ����׽��ִ�ָ���Ķ��ø��ü���ɾ��     
// ���������  MultiplexSets,ָ�����׽��ּ���
//        sock,��ɾ�����׽���
// ��������� 
// ����ֵ  ��true �ɹ� falseʧ��
// ˵��    ��
// =============================================================================
bool_t Socket_MultiplexDel(struct MultiplexSetsCB *sets,int sockfd)
{
    bool_t result;
    tagSocket *sock;
    
    result = false;

    sock = __SocketMapFd2Sock(sockfd);
    if((NULL != sock)&&(Lock_MutexPend(sock->sync, CN_TIMEOUT_FOREVER)))
    {
        result = Multiplex_DelObject(sets,&sock->ioselect);
        Lock_MutexPost(sock->sync);
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
    sock = __SocketMapFd2Sock(sockfd);
    if(NULL != sock)
    {
        if(mode &sock->iostat)
        {
            result = 1;
        }
    }
    return  result;
}

void sockinfo(int sockfd,char *filter)
{
    tagSocket *sock;

    sock = __SocketMapFd2Sock(sockfd);
    if(NULL != sock)
    {
        if(NULL != sock->proto->debuginfo)
        {
            sock->proto->debuginfo(sock,filter);
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
    int i =0;

    for (i =0; i <gSockNum;i++)
    {
        sockinfo(pgSocketMemHead[i].sockfd,param);
    }

    return true;
}

struct ShellCmdTab  gSocketDebug[] =
{
    {
        "sockall",
        sockallinfo,
        "usage:sockall",
        NULL
    }
};

#define CN_SOCKETDEBUG_NUM  ((sizeof(gSocketDebug))/(sizeof(struct ShellCmdTab)))
static struct ShellCmdRsc gSocketDebugCmdRsc[CN_SOCKETDEBUG_NUM];

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
    bool_t result;
    int i =0;

    //get the mem lock
    pSocketMemSync = Lock_MutexCreate_s(&gSocketMemSyncMem,NULL);
    if(NULL == pSocketMemSync)
    {
        printk("%s:ERR:SOCKET MEM SYNC CREATE FAILED\n\r",__FUNCTION__);
    	goto EXIT_SOCKSYNC;
    }
    //get the socket mem source
    pgSocketMemHead = (tagSocket *)malloc(gSockNum*sizeof(tagSocket));
    if(NULL == pgSocketMemHead)
    {
        printk("%s:ERR:SOCKET MEM MALLOC  FAILED\n\r",__FUNCTION__);
        goto EXIT_SOCKMEM;
    }
    //initialize the sock mem
    memset((void *)pgSocketMemHead,0, gSockNum*sizeof(tagSocket));
    for(i = 0;i <gSockNum;i++ )
    {
        pgSocketMemHead[i].sockfd|= CN_SOCK_FREE;
    }
    result = Sh_InstallCmd(gSocketDebug,gSocketDebugCmdRsc,CN_SOCKETDEBUG_NUM);
    if(result == false)
    {
        printk("%s:ERR:SOCKET DEBUG INSTALL FAILED\n\r",__FUNCTION__);
        goto EXIT_SOCKDEBUG;
    }

    tcpipmemlog("sockmem",sizeof(tagSocket),gSockNum);

    return result;

EXIT_SOCKDEBUG:
	free(pgSocketMemHead);
	pgSocketMemHead = NULL;
EXIT_SOCKMEM:
    Lock_MutexDelete_s(pSocketMemSync);
    pSocketMemSync = NULL;
EXIT_SOCKSYNC:
	result = false;
	return result;
}

