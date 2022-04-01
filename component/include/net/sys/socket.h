//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

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

// 模块描述: 本文件套接字的定义，以及使用的传输层的接口标准的定义
// 模块版本: V1.00
// 创建人员: Administrator
// 创建时间: 3:15:08 PM/Jul 15, 2014
// =============================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =============================================================================

#ifndef __SOCKET_H
#define __SOCKET_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>   //use of malloc
#include <endian.h>   //use of hton and ntoh
#include <stdio.h>
#include <string.h>

#include <os.h>
//#include <shell.h>
#include <multiplex.h>
//all the bsp driver needed is in netbdp.h
#include <netbsp.h>
//all the system function are supplied in osarch.h
#include <osarch.h>


/* Supported address families. */
#define AF_UNSPEC       0
#define AF_UNIX         1       /* Unix domain sockets          */
#define AF_LOCAL        1       /* POSIX name for AF_UNIX       */
#define AF_INET         2       /* Internet IP Protocol         */
#define AF_AX25         3       /* Amateur Radio AX.25          */
#define AF_IPX          4       /* Novell IPX                   */
#define AF_APPLETALK    5       /* AppleTalk DDP                */
#define AF_NETROM       6       /* Amateur Radio NET/ROM        */
#define AF_BRIDGE       7       /* Multiprotocol bridge         */
#define AF_ATMPVC       8       /* ATM PVCs                     */
#define AF_X25          9       /* Reserved for X.25 project    */
#define AF_INET6        10      /* IP version 6                 */
#define AF_ROSE         11      /* Amateur Radio X.25 PLP       */
#define AF_DECnet       12      /* Reserved for DECnet project  */
#define AF_NETBEUI      13      /* Reserved for 802.2LLC project*/
#define AF_SECURITY     14      /* Security callback pseudo AF */
#define AF_KEY   15      /* PF_KEY key management API */
#define AF_NETLINK      16
#define AF_ROUTE        AF_NETLINK /* Alias to emulate 4.4BSD */
#define AF_PACKET       17      /* Packet family                */
#define AF_ASH          18      /* Ash                          */
#define AF_ECONET       19      /* Acorn Econet                 */
#define AF_ATMSVC       20      /* ATM SVCs                     */
#define AF_SNA          22      /* Linux SNA Project (nutters!) */
#define AF_IRDA         23      /* IRDA sockets                 */
#define AF_MAX          32      /* For now.. */
/* Protocol families, same as address families. */
#define PF_UNSPEC       AF_UNSPEC
#define PF_UNIX         AF_UNIX
#define PF_LOCAL        AF_LOCAL
#define PF_INET         AF_INET
#define PF_AX25         AF_AX25
#define PF_IPX          AF_IPX
#define PF_APPLETALK    AF_APPLETALK
#define PF_NETROM       AF_NETROM
#define PF_BRIDGE       AF_BRIDGE
#define PF_ATMPVC       AF_ATMPVC
#define PF_X25          AF_X25
#define PF_INET6        AF_INET6
#define PF_ROSE         AF_ROSE
#define PF_DECnet       AF_DECnet
#define PF_NETBEUI      AF_NETBEUI
#define PF_SECURITY     AF_SECURITY
#define PF_KEY          AF_KEY
#define PF_NETLINK      AF_NETLINK
#define PF_ROUTE        AF_ROUTE
#define PF_PACKET       AF_PACKET
#define PF_ASH          AF_ASH
#define PF_ECONET       AF_ECONET
#define PF_ATMSVC       AF_ATMSVC
#define PF_SNA          AF_SNA
#define PF_IRDA         AF_IRDA
#define PF_MAX          AF_MAX
/* Maximum queue length specifiable by listen.  */
#define SOMAXCONN       128
/* Flags we can use with send/ and recv.
   Added those for 1003.1g not all are supported yet
 */

#define MSG_OOB         1
#define MSG_PEEK        2
#define MSG_DONTROUTE   4
#define MSG_TRYHARD     4       /* Synonym for MSG_DONTROUTE for DECnet */
#define MSG_CTRUNC      8
#define MSG_PROXY       0x10    /* Supply or ask second address. */
#define MSG_TRUNC       0x20
#define MSG_DONTWAIT    0x40    /* Nonblocking io                */
#define MSG_EOR         0x80    /* End of record */
#define MSG_WAITALL     0x100   /* Wait for a full request */
#define MSG_FIN         0x200
#define MSG_SYN         0x400
#define MSG_URG         0x800
#define MSG_RST         0x1000
#define MSG_ERRQUEUE    0x2000
#define MSG_NOSIGNAL    0x4000
#define MSG_CTLIGNORE   0x80000000
#define MSG_EOF         MSG_FIN
#define MSG_CTLFLAGS    (MSG_OOB|MSG_URG|MSG_FIN|MSG_SYN|MSG_RST)


// type
#define SOCK_STREAM     1        // 可靠的双向字节流服务 - TCP
#define SOCK_DGRAM      2        // 最好的运输层数据报服务 - UDP
#define SOCK_RAW        3        // 最好的网络层数据报服务 - ICMP IGMP 原始IP
#define SOCK_RDW        4        // 可靠的数据报服务（未实现） - n/a
#define SOCK_SEQPACKET  5        // 可靠的双向记录流服务 - n/a

//proto
#define SOL_SOCKET       4
#define IPPROTO_IP       0      // dummy for IP
#define IPPROTO_ICMP     1      // control message protocol
#define IPPROTO_IGMP     2      // internet group management protocol
#define IPPROTO_GGP      3      // gateway^2 (deprecated)
#define IPPROTO_TCP      6      // tcp
#define IPPROTO_PUP      12     // pup
#define IPPROTO_UDP      17     // user datagram protocol
#define IPPROTO_IDP      22     // xns idp
#define IPPROTO_ND       77     // UNOFFICIAL net disk proto */
#define IPPROTO_RAW      255    // raw IP packet
#define IPPROTO_MAX      256

//typedef int socklen_t;
#define socklen_t int
//#define CN_INVALID_FD  -1


//bsd socket type and addr type defines
typedef uint16_t sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct sockaddr {
      sa_family_t sa_family;       // Address family
      char sa_data[14];            // protocol-specific address
};
struct in_addr  {
    in_addr_t s_addr;                    /* IPv4 address */
};
struct sockaddr_in {
     sa_family_t sin_family;             /* AF_INET */
     in_port_t sin_port;                 // remote Port number.  sa_data[0~1]
     struct in_addr sin_addr;            // remote IP. sa_data[2~5]

     /* Pad to size of `struct sockaddr'.  */
     unsigned char sin_zero[8];
};
struct in6_addr {
    union {
        uint8_t u6_addr8[16];
        uint16_t u6_addr16[8];
        uint32_t u6_addr32[4];
    } in6_u;

    #define s6_addr                 in6_u.u6_addr8
    #define s6_addr16               in6_u.u6_addr16
    #define s6_addr32               in6_u.u6_addr32
};

struct sockaddr_in6 {
    sa_family_t sin6_family;    /* AF_INET6 */
    in_port_t sin6_port;        /* Transport layer port # */
    uint32_t sin6_flowinfo;     /* IPv6 flow information */
    struct in6_addr sin6_addr;  /* IPv6 address */
    uint32_t sin6_scope_id;     /* IPv6 scope-id */
};
/* Structure large enough to hold any socket address
(with the historical exception of AF_UNIX). 128 bytes reserved.  */

#if ULONG_MAX > 0xffffffff
# define __ss_aligntype __uint64_t
#else
# define __ss_aligntype __uint32_t
#endif
#define _SS_SIZE        128
#define _SS_PADSIZE     (_SS_SIZE - (2 * sizeof (__ss_aligntype)))

struct sockaddr_storage
{
    sa_family_t ss_family;      /* Address family */
    __ss_aligntype __ss_align;  /* Force desired alignment.  */
    char __ss_padding[_SS_PADSIZE];
};


//SOL_SOCKET
enum _EN_SOL_SOCKET_OPTNAME
{
    //OPTNAME              MEANS                            optvaltype
    SO_BROADCAST = 0 , //允许发送广播数据　　　　　　　　　　　　int
    SO_DEBUG,          //允许调试　　　　　　　　　　　　　　　　int
    SO_DONTROUTE,      //不查找路由　　　　　　　　　　　　　　　int
    SO_ERROR,          //获得套接字错误　　　　　　　　　　　　　int
    SO_KEEPALIVE,      //保持连接　　　　　　　　　　　　　　　　int
    SO_LINGER,         //延迟关闭连接　　　　　　　　　　　　　　struct linger
    SO_OOBINLINE,      //带外数据放入正常数据流　　　　　　　　　int
    SO_RCVBUF,         //接收缓冲区大小　　　　　　　　　　　　　int
    SO_SNDBUF,         //发送缓冲区大小　　　　　　　　　　　　　int
    SO_RCVLOWAT,       //接收缓冲区下限　　　　　　　　　　　　　int
    SO_SNDLOWAT,       //发送缓冲区下限　　　　　　　　　　　　　int
    SO_RCVTIMEO,       //接收超时　　　　　　　　　　　　　　　　struct timeval
    SO_SNDTIMEO,       //发送超时　　　　　　　　　　　　　　　　struct timeval
    SO_REUSEADDR,      //允许重用本地地址和端口　　　　　　　　　int
    SO_TYPE,           // 获得套接字类型　　　　　　　　　　　　 int
    SO_BSDCOMPAT,      //与BSD系统兼容　　　　　　　　　　　　　 int
    SO_NONBLOCK,       //DJY 非阻塞                                                    int
};
//IPPROTO_IP
enum _EN_IPPROTO_IP_OPTNAME
{
    //OPTNAME              MEANS                           optvaltype
    IP_HDRINCL = 0,   //在数据包中包含IP首部　　　　　　　　　　int
    IP_OPTIONS    ,   //IP首部选项　　　　　　　　　　　　　　　int
    IP_TOS        ,   //服务类型                                                        int
    IP_TTL        ,   //生存时间　　　　　　　　　　　　　　　　int
};
//IPPROTO_TCP
enum _EN_IPPROTO_TCP_OPTNAME
{
    //OPTNAME              MEANS                           optvaltype
    TCP_MAXSEG = 0,      //TCP最大数据段大小　　　　　　　　　 int
    TCP_NODELAY   ,      //不使用Nagle算法　　　　　　　　　　 int
};

enum _EN_SOCKEY_ERRORNO
{
    EN_SOCKET_ERROR_PARAERR =0 ,      //para error
    EN_SOCKET_TRY_AGAIN,              //when there is no data what we need, and
                                      //the socket is noblock, then the no will be set
    EN_SOCKET_ERROR_SNDPKG2LARGE ,    //the snd buf exceed the sndbuf len
    EN_SOCKET_CHANNEL_SHUTDOWN_W,     //the communication has been shutdown WRITE
    EN_SOCKET_CHANNEL_SHUTDOWN_R,     //the communication has been shutdown READ
};
//SOCKET IOSTAT
#define  CN_SOCKET_IOREAD      CN_MULTIPLEX_SENSINGBIT_READ       //COULE  READ
#define  CN_SOCKET_IOWRITE     CN_MULTIPLEX_SENSINGBIT_WRITE      //COULD  WRITE, which means the
#define  CN_SOCKET_IOERR       CN_MULTIPLEX_SENSINGBIT_ERROR      //SOCKET ERROR
#define  CN_SOCKET_IOACCEPT    CN_MULTIPLEX_SENSINGBIT_USERBASE   //COULD  ACCEPT
#define  CN_SOCKET_IOOR        CN_MULTIPLEX_SENSINGBIT_OR         //BITS OR TRIGGLE

//SOCKET SOCKSTAT
#define CN_SOCKET_CLIENT        (1<<0)
#define CN_SOCKET_BIND          (1<<1)
#define CN_SOCKET_CONNECT       (1<<2)
#define CN_SOCKET_LISTEN        (1<<3)
#define CN_SOCKET_SHUTWR        (1<<5)
#define CN_SOCKET_SHUTRD        (1<<6)
#define CN_SOCKET_CLOSE         (1<<7)
#define CN_SOCKET_OPEN          (1<<8)    //if use socket or accept to get the sock
#define CN_SOCKET_PROBLOCK      (1<<10)
#define CN_SOCKET_PROASYNC      (1<<11)
#define CN_SOCKET_PROOOB        (1<<12)
#define CN_SOCKET_PRONAGLE      (1<<13)
#define CN_SOCKET_PROLINGER     (1<<14)
#define CN_SOCKET_PROCORK       (1<<15)     //收集到最大量数据才发送。
#define CN_SOCKET_PROREUSE      (1<<16)
#define CN_SOCKET_PROKEEPALIVE  (1<<17)
#define CN_SOCKET_PROBROAD      (1<<18)

#define CN_SOCKET_PROCONNECT    (1<<19)  //使用非阻塞连接时添加这个标志来记录目前发起了连接请求
#define CN_SOCKET_WAITACCEPT    (1<<20)  // 1 代表正在clst链表中，可能已完成3次握手，也可能未完成

typedef struct
{
    u32 iplocal;
    u32 ipremote;;
    u16 portlocal;
    u16 portremote;
}tagSockElementV4;
#define INET_ADDRSTRLEN 16      //IPv4点分十进制地址长度
#define CN_IPV6_LEN_WORD    4   // u32 表示的ipv6地址长度
#define INET6_ADDRSTRLEN    46  //IPv6点分十进制地址最大长度
typedef struct
{
    u32 iplocal[CN_IPV6_LEN_WORD];
    u32 ipremote[CN_IPV6_LEN_WORD];
    u16 portlocal;
    u16 portremote;
}tagSockElementv6;

typedef union
{
    tagSockElementV4  v4;
    tagSockElementv6  v6;
}uSockElement;

//传输层协议接口定义
//传输层定义的接口,各个传输层协议，比方TCP,UDP等必须按照这个接口来实现
//对于不需要的函数接口，可以置空
//返回值为标准的BSD返回值
struct TPL_ProtocalOps
{
    //创建一个套接字
    struct tagSocket* (*__socket)(int family, int type, int protocol);
    //绑定一个端口号
    int (*__bind)(struct tagSocket *sockfd,struct sockaddr *myaddr, int addrlen);
    //让一个端口处于监听状态
    int (*__listen)(struct tagSocket *sockfd, int backlog);
    //让一个主机端处于接收状态(服务器才会)
    struct tagSocket* (*__accept)(struct tagSocket *sockfd, struct sockaddr *addr, int *addrlen);
    //让一个客户端去链接服务器
    int (*__connect)(struct tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen);
    //发送数据
    int (*__send)(struct tagSocket *sockfd, const void *msg, int len, int flags);
    //接收数据
    int (*__recv)(struct tagSocket *sockfd, void *buf,int len, unsigned int flags);
    //直接发送数据到目的端
    int (*__sendto)(struct tagSocket * sockfd, const void *msg,int len, unsigned int flags,\
              const struct sockaddr *addr, int addrlen);
    //直接从目的端读取数据
    int (*__recvfrom)(struct tagSocket * sockfd,void *buf, int len, unsigned int flags,\
                struct sockaddr *addr, int *addrlen);
    //关闭一个套接口
#define SHUT_RD   0  //关闭读
#define SHUT_WR   1  //关闭写
#define SHUT_RDWR 2  //关闭读写
    int (*__shutdown)(struct tagSocket *sockfd, u32 how);
    int (*__close)(struct tagSocket *sockfd);
//    int (*isactive)(struct tagSocket *sockfd,int mode);
    //设置套接字选项
    int (*__setsockopt)(struct tagSocket *sockfd, int level, int optname,\
                   const void *optval, int optlen);
    //获取套接字选项
    int (*__getsockopt)(struct tagSocket *sockfd, int level, int optname, void *optval,\
                   int *optlen);
    void (*__debuginfo)(struct tagSocket *sockfd,char *filter);
};

struct tagSocket
{
    //the following used by the proto
//  void                           *SockObj;      //used for the socket layqueue
    s32                             sockfd;       //socket对应的文件指针
    struct tagSocket               *Nextsock;     //用于hash表，把hash值相同的socket
                                                  //串起来，单向链表，NULL结束
    struct tagSocket               *NextClient;   //client在被 accept 前，用此链表
                                                  //挂在struct ServerCB的clst指针上。
                                                  //单向链表，NULL结束
    struct MutexLCB                *SockSync;     //used to protect the socket
    struct TPL_ProtocalOps         *ProtocolOps;  //传输层操作函数集
    void                           *TplCB;        //传输层协议控制块指针，类型由具体传输层解析
//  struct MultiplexObjectCB       *ioselect;     //used for the select;
//  u32                             IoInitstat;   //多路复用状态，如 CN_SOCKET_IOREAD
    u32                             errorno;      //record the socket errorno
    u32                             sockstat;     //socket状态，如 CN_SOCKET_OPEN
    ptu32_t                         SockUserTag;  //由用户决定怎么用
    //下面是协议地址，考虑把它挪到网络层去
    int                             af_inet;      //地址族，如 AF_INET
    uSockElement                    element;      //socket addr
};

//linger选项
struct linger
{
    int l_onff;//0 off while 1 on
    int l_linger;//linger time,Units:Seconds
};

//some defines for the STACK SOCKET TOOL FUNCTION
#define CN_SOCKET_PORT_TEMPBASE  0x8000
#define CN_SOCKET_PORT_INVALID   0x0
#define CN_SOCKET_PORT_LIMIT     0xFFFF

struct tagSocket *SocketBuild(void);
bool_t SocketFree(struct tagSocket *sock);

//FOR ALL THE APPLICATIONS, ONLY THE FOLLOWING INTERFACE COULD BE USED
int socket(int family, int type, int protocol);
int bind(int sockfd,struct sockaddr *addr, int addrlen);
int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, int *addrlen);
int connect(int sockfd, struct sockaddr *serv_addr, int addrlen);
int send(int sockfd, const void *msg, int len, int flags);
int recv(int sockfd, void *buf,int len, unsigned int flags);
bool_t sendexact(int sock,u8 *buf,int len);
bool_t recvexact(int sock,u8 *buf,int len);
int sendto(int sockfd, const void *msg,int len, unsigned int flags,\
          const struct sockaddr *addr, int addrlen);
int recvfrom(int sockfd,void *buf, int len, unsigned int flags,\
            struct sockaddr *addr, int *addrlen);
bool_t shutdown(int sockfd, u32 how);
bool_t closesocket(int sockfd);
int setsockopt(int sockfd, int level, int optname,\
                const void *optval, int optlen);
int getsockopt(int sockfd, int level, int optname, void *optval,int *optlen);
bool_t Socket_MultiplexAdd(struct MultiplexSetsCB *MultiplexSets,
                           int sock,u32 SensingBit);
bool_t Socket_MultiplexDel(struct MultiplexSetsCB *MultiplexSets,int sock);
ptu32_t socket_SetUserTag(int sock, ptu32_t Private);
ptu32_t socket_GetUserTag(int sock);
void sockinfo(int sockfd,char *filter);
bool_t sockallinfo(char *param);
int getsockname(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
int getpeername(int sockfd,struct sockaddr *addr,socklen_t *addrlen);

////THIS IS THE COMPORT DEFINES FOR THE FREE SOFTWARE**********************//
////fcntl.h
///* open/fcntl - O_SYNC is only implemented on blocks devices and on files
//   located on an ext2 file system */
//#ifndef O_ACCMODE
//#define O_ACCMODE   00000003
//#endif
//
//#ifndef O_RDONLY
//#define O_RDONLY    00000000
//#endif
//
//#ifndef O_WRONLY
//#define O_WRONLY    00000001
//#endif
//
//#ifndef O_RDWR
//#define O_RDWR      00000002
//#endif
//
//
//#ifndef O_CREAT
//#define O_CREAT     00000100    /* not fcntl */
//#endif
//#ifndef O_EXCL
//#define O_EXCL      00000200    /* not fcntl */
//#endif
//#ifndef O_NOCTTY
//#define O_NOCTTY    00000400    /* not fcntl */
//#endif
//#ifndef O_TRUNC
//#define O_TRUNC     00001000    /* not fcntl */
//#endif
//#ifndef O_APPEND
//#define O_APPEND    00002000
//#endif
//#ifndef O_NONBLOCK
//#define O_NONBLOCK  00004000
//#endif
//#ifndef O_SYNC
//#define O_SYNC      00010000
//#endif
//#ifndef FASYNC
//#define FASYNC      00020000    /* fcntl, for BSD compatibility */
//#endif
//#ifndef O_DIRECT
//#define O_DIRECT    00040000    /* direct disk access hint */
//#endif
//#ifndef O_LARGEFILE
//#define O_LARGEFILE 00100000
//#endif
//#ifndef O_DIRECTORY
//#define O_DIRECTORY 00200000    /* must be a directory */
//#endif
//#ifndef O_NOFOLLOW
//#define O_NOFOLLOW  00400000    /* don't follow links */
//#endif
//#ifndef O_NOATIME
//#define O_NOATIME   01000000
//#endif
//#ifndef O_CLOEXEC
//#define O_CLOEXEC   02000000    /* set close_on_exec */
//#endif
//#ifndef O_NDELAY
//#define O_NDELAY    O_NONBLOCK
//#endif
//
//#ifndef FD_CLOEXEC
//#define FD_CLOEXEC     (1<<0)
//#endif
//
//#ifndef FD_SETSIZE
//#define FD_SETSIZE     (10)    //the mac select num
//#endif

#ifndef O_NDELAY
#define O_NDELAY    O_NONBLOCK
#endif

//#ifndef FD_SETSIZE
//#define FD_SETSIZE     (10)    //the mac select num
//#endif

#include <unistd.h>
//select.h
#include <sys/time.h>

//#define CN_SELECT_TIMEDEFAULT 10
//在“工具链路径\include\sys\select.h”上有定义
//typedef struct
//{
//    int  mode;
//    u32  fd[CN_SELECT_MAXNUM];
//}_types_fd_set;
//
//#define fd_set _types_fd_set
//#define ARG_MAX 256
//int FD_SET(int fd, fd_set *sets);
//int FD_CLR(int fd, fd_set *sets);
//int FD_ISSET(int fd, fd_set *sets);
//int FD_ZERO(fd_set *sets);
//
//int select(int maxfd, fd_set *reads,fd_set *writes, fd_set *exps,
//           struct timeval *timeout);
////////////////////////NETDEV DRIVER USED INTERFACE///////////////////////////////
//ip address defines
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK   htonl(0x7f000001)
#endif
#define INADDR_ANY          0
#define INADDR_NONE         0XFFFFFFFF
#define INADDR_BROAD        0XFFFFFFFF
#define INADDR_BROADCAST    INADDR_BROAD
#define INVALID_SOCKET      -1
#define CN_IPADDR_LEN_V4     4
#define CN_IPADDR_LEN_V6     16
#define CN_IPADDR_ANY_V4     INADDR_ANY


////////////////////////////////////////////////////////////////////////////////
//////////////////////////I WANT TO REBUILD THE NEW LINK ROUT///////////////////
//this defines for the debug
#define TCPIP_DEBUG_INC(x)     do{x++;}while(0)
#define TCPIP_DEBUG_DEC(x)     do{x--;}while(0)

#define CN_TCPIP_NAMELEN      32    //IN THE TCP IP STACK ,THIS IS THE NAME LEN LIMIT


//the user could use these function to create delete modify the rout
//warning: ipaddr_t for ip, if ver is EN_IPV_4, then it is an u32 or an point
//         to u32, if EN_IPV_6, it must be an u8*, and the len is CN_IPADDR_LEN_V6
typedef ptu32_t   ipaddr_t;


//bool_t RoutDelete(const char *name,enum_ipv_t ver,ipaddr_t addr);
//bool_t RoutSet(const char *name,enum_ipv_t ver,ipaddr_t ipold,void *newaddr);
//bool_t RoutSetDefault(enum_ipv_t ver,ipaddr_t ip);
//bool_t RoutDns(enum_ipv_t ver, ipaddr_t ip);
//bool_t RoutSetDefaultAddr(enum_ipv_t ver,ipaddr_t ip,ipaddr_t mask,ipaddr_t gateway,ipaddr_t dns);
//this function use to alloc an ip from the dhcp dynamicly
bool_t DHCP_AddClientTask(const char *ifname, u32 OldIP);
bool_t  DHCP_ServerInit(void);
bool_t  DHCP_ClientInit(void);

//used for the multicast
#define MULTICAST_MASK     (htonl(0xf0000000))
#define MULTICAST_FLAG     (htonl(0xe0000000))
#define IN_MULTICAST(x)    (MULTICAST_MASK&x==MULTICAST_FLAG?true:false)
#define IN6_IS_ADDR_MULTICAST(x) (*(u8 *)x==0xff?true:false)

//ftp client interface
//upload:put the local file(sfile) to the server(dfile)
//download:download the sourcefile(sfile)  to the local file(dfile) from the server
int FTP_ClientUpload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);
int FTP_ClientDownload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);



#endif /* __SOCKET_H */

