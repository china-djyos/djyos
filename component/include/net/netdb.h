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

//-----------------------------------------------------------------------------
//author:zhangqf
//date  :����12:51:14/2017��2��19��
//usage :
//-----------------------------------------------------------------------------

#include <sys/socket.h>

#ifndef __NETDB_H__
#define __NETDB_H__

extern s32 h_errno;
//rpc_netdb.h
struct rpcent
{
  char *r_name;     /* Name of server for this rpc program.  */
  char **r_aliases; /* Alias list.  */
  s32 r_number;     /* RPC program number.  */
};

extern void setrpcent (s32 __stayopen) ;
extern void endrpcent (void) ;
extern struct rpcent *getrpcbyname (__const char *__name) ;
extern struct rpcent *getrpcbynumber (s32 __number) ;
extern struct rpcent *getrpcent (void) ;
/* Address Information Errors.  The <netdb.h> header shall define the
 * following macros for use as error values for getaddrinfo() and
 * getnameinfo():
 *
 *   EAI_AGAIN       - The name could not be resolved at this time. Future
 *                     attempts may succeed.
 *   EAI_BADFLAGS    - The flags had an invalid value.EAI_FAILA non-
 *                     recoverable error occurred.
 *   EAI_FAMILY      - The address family was not recognized or the address
 *                     length was invalid for the specified family.
 *   EAI_MEMORY      - There was a memory allocation failure.
 *   EAI_NONAME      - The name does not resolve for the supplied
 *                     parameters.  NI_NAMEREQD is set and the host's name
 *                     cannot be located, or both nodename and servname were
 *                     null.
 *   EAI_SERVICE     - The service passed was not recognized for the
 *                     specified socket type.
 *   EAI_SOCKTYPE    - The intended socket type was not recognized.
 *   EAI_SYSTEM      - A system error occurred. The error code can be found
 *                     in errno.
 *   EAI_OVERFLOW    - An argument buffer overflowed.
 */

/* Lookup mode.  */
#define GAI_NOWAIT  1
/* Possible values for `ai_flags' field in `addrinfo' structure.  */
#define AI_PASSIVE  0x0001  /* Socket address is intended for `bind'.  */
#define AI_CANONNAME    0x0002  /* Request for canonical name.  */
#define AI_NUMERICHOST  0x0004  /* Don't use name resolution.  */
#define AI_V4MAPPED 0x0008  /* IPv4 mapped addresses are acceptable.  */
#define AI_ALL      0x0010  /* Return IPv4 mapped and IPv6 addresses.  */
#define AI_ADDRCONFIG   0x0020  /* Use configuration of this host to choose
                   returned address type..  */
#define AI_IDN  0x0040  /* IDN encode input (assuming it is encoded
                   in the current locale's character set)
                   before looking it up. */
#define AI_CANONIDN 0x0080  /* Translate canonical name from IDN format. */
#define AI_IDN_ALLOW_UNASSIGNED 0x0100 /* Don't reject unassigned Unicode
                        code points.  */
#define AI_IDN_USE_STD3_ASCII_RULES 0x0200 /* Validate strings according to
                        STD3 rules.  */
#define AI_NUMERICSERV  0x0400  /* Don't use name resolution.  */

#define EAI_OK      0
/* Error values for `getaddrinfo' function.  */
#define EAI_BADFLAGS      -1    /* Invalid value for `ai_flags' field.  */
#define EAI_NONAME    -2    /* NAME or SERVICE is unknown.  */
#define EAI_AGAIN     -3    /* Temporary failure in name resolution.  */
#define EAI_FAIL      -4    /* Non-recoverable failure in name res.  */
#define EAI_FAMILY    -6    /* `ai_family' not supported.  */
#define EAI_SOCKTYPE      -7    /* `ai_socktype' not supported.  */
#define EAI_SERVICE   -8    /* SERVICE not supported for `ai_socktype'.  */
#define EAI_MEMORY    -10   /* Memory allocation failure.  */
#define EAI_SYSTEM    -11   /* System error returned in `errno'.  */
#define EAI_OVERFLOW      -12   /* Argument buffer overflow.  */
#define EAI_NODATA    -5    /* No address associated with NAME.  */
#define EAI_ADDRFAMILY  -9  /* Address family for NAME not supported.  */
#define EAI_INPROGRESS  -100    /* Processing request in progress.  */
#define EAI_CANCELED      -101  /* Request canceled.  */
#define EAI_NOTCANCELED -102    /* Request not canceled.  */
#define EAI_ALLDONE   -103  /* All requests done.  */
#define EAI_INTR      -104  /* Interrupted by a signal.  */
#define EAI_IDN_ENCODE  -105    /* IDN encoding failed.  */

#define NI_MAXHOST      1025
#define NI_MAXSERV      32

#define NI_NUMERICHOST  1   /* Don't try to look up hostname.  */
#define NI_NUMERICSERV 2    /* Don't convert port number to name.  */
#define NI_NOFQDN   4   /* Only return nodename portion.  */
#define NI_NAMEREQD 8   /* Don't return numeric addresses.  */
#define NI_DGRAM    16  /* Look up UDP service rather than TCP.  */
#define NI_IDN  32  /* Convert name from IDN format.  */
#define NI_IDN_ALLOW_UNASSIGNED 64 /* Don't reject unassigned Unicode
                    code points.  */
#define NI_IDN_USE_STD3_ASCII_RULES 128 /* Validate strings according to
                         STD3 rules.  */

/* h_errno values that may be returned by gethosbyname(), gethostbyname_r(),
 * gethostbyaddr(), or gethostbyaddr_r()
 *
 *   HOST_NOT_FOUND - No such host is known.
 *
 *   NO_DATA - The server recognized the request and the name, but no
 *     address is available. Another type of request to the name server
 *     for the domain might return an answer.
 *
 *   NO_RECOVERY - An unexpected server failure occurred which cannot be
 *     recovered.
 *
 *   TRY_AGAIN - A temporary and possibly transient error occurred, such as
 *     a failure of a server to respond.
 */

/* Possible values left in `h_errno'.  */
#define HOST_NOT_FOUND  1   /* Authoritative Answer Host not found.  */
#define TRY_AGAIN   2   /* Non-Authoritative Host not found,
                   or SERVERFAIL.  */
#define NO_RECOVERY 3   /* Non recoverable errors, FORMERR, REFUSED,
                   NOTIMP.  */
#define NO_DATA     4   /* Valid name, no data record of requested
                   type.  */
#define NETDB_INTERNAL  -1  /* See errno.  */
#define NETDB_SUCCESS   0   /* No problem.  */
#define NO_ADDRESS  NO_DATA /* No address, look for MX record.  */

/* Highest reserved Internet port number.  */
#define IPPORT_RESERVED 1024

/* Scope delimiter for getaddrinfo(), getnameinfo().  */
#define SCOPE_DELIMITER '%'

/****************************************************************************
 * Public Types
 ****************************************************************************/

struct hostent
{
  char  *h_name;       /* Official name of the host. */
  char **h_aliases;    /* A pointer to an array of pointers to alternative
                            * host names, terminated by a null pointer. */
  s32        h_addrtype;   /* Address type.�� AF_INET  */
  s32        h_length;     /* The length, in bytes, of the address. */
  char **h_addr_list;  /* A pointer to an array of pointers to network
                            * addresses (in network byte order) for the host,
                            * terminated by a null pointer. */
};

//���hostent֧�ֿ��������⡣
#define CN_RESULT_NUM     10
#define DNS_NAME_LEN_MAX     64

typedef struct StDnsResult{
    char      arrDnsCNameAddr[CN_RESULT_NUM+1][DNS_NAME_LEN_MAX];
    char      arrDnsINameAddrV4[CN_RESULT_NUM+1][4];
}StDnsResult;

typedef struct hostent_ext
{
    char *h_name;
    char **h_aliases;
    s32 h_addrtype;
    s32 h_length;
    char **h_addr_list;
    char arr_name[100];
    char *arr_aliases[CN_RESULT_NUM+1];
    char *arr_addr_list[CN_RESULT_NUM+1];
    struct StDnsResult dns_res;
}hostent_ext;

#define h_addr h_addr_list[0] /* For backward compatibility */

struct netent
{
  char  *n_name;       /* Official, fully-qualified (including the domain)
                            * name of the host. */
  char **n_aliases;    /* A pointer to an array of pointers to alternative
                            * network names, terminated by a null pointer. */
  s32        n_addrtype;   /* The address type of the network. */
  uint32_t   n_net;        /* The network number, in host byte order. */
};

struct protoent
{
  char  *p_name;       /* Official name of the protocol. */
  char **p_aliases;    /* A pointer to an array of pointers to
                            * alternative protocol names, terminated by a
                            * null pointer. */
  s32    p_proto;      /* The protocol number. */
};

struct servent
{
  char  *s_name;       /* Official name of the service. */
  char **s_aliases;    /* A pointer to an array of pointers to
                            * alternative service names, terminated by a
                            * null pointer.  */
  s32    s_port;       /* The port number at which the service resides,
                            * in network byte order. */
  char  *s_proto;      /* The name of the protocol to use when
                            * contacting the service. */
};

struct addrinfo
{
  s32        ai_flags;     /* Input flags.  */
  s32        ai_family;    /* Address family of socket.  */
  s32        ai_socktype;  /* Socket type.  */
  s32        ai_protocol;  /* Protocol of socket.  */
  socklen_t  ai_addrlen;   /* Length of socket address.  */

  struct sockaddr *ai_addr;      /* Socket address of socket.  */
  char            *ai_canonname; /* Canonical name of service location.  */
  struct addrinfo *ai_next;      /* Pointer to next in list.  */
};

//����Э��ջ�ӿڶ��壬����Э��ջ�豸������cat.1����ʵ����Щ�ӿڣ����ڰ�װ����ʱ�ṩ
struct ExtStackOps
{
    //����һ���׽��֣��˺���4Gģ������ʵ�֡�
    s32 (*ExtSocket)(s32 family, s32 type, s32 protocol);
    //�󶨵�ַ���˺���4Gģ������ʵ��
    s32 (*ExtBind)(s32 Sockfd,struct sockaddr *myaddr, s32 addrlen);
    //����һ��tcp����������������socket�������յ��ͻ�������ʱ����鴦�� CN_SOCKET_CLIENT
    //״̬������socket��������������˭�����ö�·����λ
    s32 (*ExtListen)(s32 Sockfd, s32 backlog);
    //����һ�����ӣ��˺���4Gģ������ʵ�֡�
    s32 (*ExtAccept)(s32 sockfd, struct SemaphoreLCB *rs);
    //��һ���ͻ���ȥ���ӷ�����
    s32 (*ExtConnect)(s32 sockfd,struct SemaphoreLCB *rs);
    //��������
    s32 (*ExtSend)(s32 sockfd, const void *msg, s32 len);
    //��������
    s32 (*ExtRecv)(s32 sockfd, void *buf,s32 len, u32 flags);
    //ֱ�ӷ������ݵ�Ŀ�Ķ�
    s32 (*ExtSendto)(s32 sockfd, const void *msg,s32 len, u32 flags,\
              const struct sockaddr *addr, s32 addrlen);
    //ֱ�Ӵ�Ŀ�Ķ˶�ȡ����
    s32 (*ExtRecvfrom)(s32 sockfd,void *buf, s32 len, u32 flags,\
                struct sockaddr *addr, s32 *addrlen);
    //�ر�һ���׽ӿ�
    s32 (*ExtClose)(s32 sockfd);
    //�����׽���ѡ��
    s32 (*ExtSetSockOpt)(s32 sockfd, s32 optname,const void *optval, s32 optlen);
};


void NetDev_PkgsndInc(struct NetDev *iface);
void NetDev_PkgsndErrInc(struct NetDev *iface);
void NetDev_PkgrcvInc(struct NetDev *iface);
void NetDev_PkgrcvErrInc(struct NetDev *iface);
struct servent *getservbyname(const char *name, const char *proto);
struct hostent *gethostbyname(const char *name);
struct hostent * gethostbyname_r(const char *name,struct hostent_ext *pnew);
s32  gethostname(char *name, s32 len);
s32 sethostname(const char *name, size_t len);

s32 getaddrinfo( const char *hostname, const char *service, const struct addrinfo *hints, struct addrinfo **result );
void freeaddrinfo (struct addrinfo*ai);
#endif /* __NETDB_H__ */
