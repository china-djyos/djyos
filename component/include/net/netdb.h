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

//-----------------------------------------------------------------------------
//author:zhangqf
//date  :下午12:51:14/2017年2月19日
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
  s32        h_addrtype;   /* Address type.如 AF_INET  */
  s32        h_length;     /* The length, in bytes, of the address. */
  char **h_addr_list;  /* A pointer to an array of pointers to network
                            * addresses (in network byte order) for the host,
                            * terminated by a null pointer. */
};

//添加hostent支持可重入问题。
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

//外置协议栈接口定义，外置协议栈设备（例如cat.1）须实现这些接口，并在安装网卡时提供
struct ExtStackOps
{
    //创建一个套接字，此函数4G模块无须实现。
    s32 (*ExtSocket)(s32 family, s32 type, s32 protocol);
    //绑定地址，此函数4G模块无须实现
    s32 (*ExtBind)(s32 Sockfd,struct sockaddr *myaddr, s32 addrlen);
    //启动一个tcp服务器，参数来自socket，当接收到客户端连接时，检查处于 CN_SOCKET_CLIENT
    //状态的所有socket，看新连接属于谁，设置多路复用位
    s32 (*ExtListen)(s32 Sockfd, s32 backlog);
    //接受一个连接，此函数4G模块无须实现。
    s32 (*ExtAccept)(s32 sockfd, struct SemaphoreLCB *rs);
    //让一个客户端去连接服务器
    s32 (*ExtConnect)(s32 sockfd,struct SemaphoreLCB *rs);
    //发送数据
    s32 (*ExtSend)(s32 sockfd, const void *msg, s32 len);
    //接收数据
    s32 (*ExtRecv)(s32 sockfd, void *buf,s32 len, u32 flags);
    //直接发送数据到目的端
    s32 (*ExtSendto)(s32 sockfd, const void *msg,s32 len, u32 flags,\
              const struct sockaddr *addr, s32 addrlen);
    //直接从目的端读取数据
    s32 (*ExtRecvfrom)(s32 sockfd,void *buf, s32 len, u32 flags,\
                struct sockaddr *addr, s32 *addrlen);
    //关闭一个套接口
    s32 (*ExtClose)(s32 sockfd);
    //设置套接字选项
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
