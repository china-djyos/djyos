//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
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
#include <shell.h>
#include <multiplex.h>

//一个PKGLST在传输的过程中，当某个PKG拥有CN_PKLGLST_END标记或者NULL == partnext，即可认为
//该PKGLST结束，该特性在发送的时候尤其明显
#define CN_PKLGLST_END   (1<<0)
//tagNetPkg的原理
//bufsize在申请时指定，使用过程中一直不变;data一直指向buf的起始位置，保持不变
//当向PKG写入数据时，offset不变，从buf的offset+datalen的地方开始写新数据，写完之后
//                  datalen +=len(len为写入数据长度)
//当从PKG读取数据时，从buf的offset开始cpy，cpy完成之后，
//                  offset += len,datalen -= len(len为取出数据长度)
typedef struct NetPkg
{
    struct NetPkg   *partnext;        //该组指针负责数据包在协议栈的传递
    ptu32_t  private;   //used for the module who malloc the package
    u8   level;         // PKG的大小等级
    u8   pkgflag;       // PKG的后续扩展属性
    u8   refers;        // 缓存的次数
    u16  datalen;       // buf中的有效数据长度
    u16  bufsize;       // buf的长度
    u16  offset;        // 有效数据偏离buf的位置，offset之前的数据无效,当分拆数据或者数据对齐的时候很有用
    u8   *buf;          // pkg的buf（数据缓存区）
}tagNetPkg;

bool_t     Pkg_SetAlignOffset(u16 alignsize);
tagNetPkg *PkgMalloc(u16 bufsize,u8 flags);
bool_t     PkgTryFreePart(tagNetPkg *pkg);
bool_t     PkgTryFreeLst(tagNetPkg  *pkglst);
bool_t     PkgTryFreeQ(tagNetPkg  *pkglst);
bool_t     PkgCachedPart(tagNetPkg  *pkg);
bool_t     PkgCachedLst(tagNetPkg   *pkglst);
u16        PkgAlignSize(u16 size);
#define    PKG_ISLISTEND(pkg)      (pkg->pkgflag&CN_PKLGLST_END)

//used to defines the net device task
#define CN_IPDEV_TCPOCHKSUM  (1<<0)
#define CN_IPDEV_TCPICHKSUM  (1<<1)
#define CN_IPDEV_UDPOCHKSUM  (1<<2)
#define CN_IPDEV_UDPICHKSUM  (1<<3)
#define CN_IPDEV_ICMPOCHKSUM (1<<4)
#define CN_IPDEV_ICMPICHKSUM (1<<5)
#define CN_IPDEV_IPOCHKSUM   (1<<6)
#define CN_IPDEV_IPICHKSUM   (1<<7)
#define CN_IPDEV_ALL         (0xFFFFFFFF)
#define CN_IPDEV_NONE        (0X0)

//enum the ip version
typedef enum
{
	EN_IPV_4 =4,
	EN_IPV_6 =6,
}enum_ipv_t;

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
#define SOCK_STREAM		1        // 可靠的双向字节流服务 - TCP
#define SOCK_DGRAM		2        // 最好的运输层数据报服务 - UDP
#define SOCK_RAW        3        // 最好的网络层数据报服务 - ICMP IGMP 原始IP
#define SOCK_RDW        4        // 可靠的数据报服务（未实现） - n/a
#define SOCK_SEQPACKET	5        // 可靠的双向记录流服务 - n/a

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
#define CN_INVALID_FD  -1


//bsd socket type and addr type defines
typedef uint16_t sa_family_t;
typedef uint16_t in_port_t;
typedef uint32_t in_addr_t;

struct sockaddr {
      sa_family_t sa_family;       /* Address family */
      char sa_data[14];            /* protocol-specific address */
};
struct in_addr  {
    in_addr_t s_addr;                    /* IPv4 address */
};
struct sockaddr_in {
     sa_family_t sin_family;             /* AF_INET */
     in_port_t sin_port;                 /* Port number.  */
     struct in_addr sin_addr;            /* Internet address.  */

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
    SO_REUSEADDR,     //允许重用本地地址和端口　　　　　　　　　  int
    SO_TYPE,           // 获得套接字类型　　　　　　　　　　　　  int
    SO_BSDCOMPAT,      //与BSD系统兼容　　　　　　　　　　　　　 int
    SO_NOBLOCK,        //DJY 非阻塞                                                    int
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
#define  CN_SOCKET_IOREAD      CN_MULTIPLEX_SENSINGBIT_READ   //COULE  READ
#define  CN_SOCKET_IOWRITE     CN_MULTIPLEX_SENSINGBIT_WRITE  //COULD  WRITE, which means the
#define  CN_SOCKET_IOERR       CN_MULTIPLEX_SENSINGBIT_ERROR  //SOCKET ERROR
#define  CN_SOCKET_IOACCEPT    (CN_SOCKET_IOREAD)             //COULD  ACCEPT
#define  CN_SOCKET_IOOR        CN_MULTIPLEX_SENSINGBIT_OR     //BITS OR TRIGGLE

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
#define CN_SOCKET_PROCORK       (1<<15)
#define CN_SOCKET_PROREUSE      (1<<16)
#define CN_SOCKET_PROKEEPALIVE  (1<<17)
#define CN_SOCKET_PROBROAD      (1<<18)

typedef struct
{
    u32 iplocal;
    u32 ipremote;;
    u16 portlocal;
    u16 portremote;
}tagSockElementV4;
#define INET_ADDRSTRLEN 16 /* for IPv4 dotted-decimal */
#define CN_IPV6_LEN_WORD    4
#define INET6_ADDRSTRLEN    46
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

typedef struct Socket tagSocket;
//传输层协议接口定义
//传输层定义的接口,各个传输层协议，比方TCP,UDP等必须按照这个接口来实现
//对于不需要的函数接口，可以置空
//返回值为标准的BSD返回值
typedef struct TlayerProto
{
    //创建一个套接字
    tagSocket* (*socket)(int family, int type, int protocol);
    //绑定一个端口号
    int (*bind)(tagSocket *sockfd,struct sockaddr *myaddr, int addrlen);
    //让一个端口处于监听状态
    int (*listen)(tagSocket *sockfd, int backlog);
    //让一个主机端处于接收状态(服务器才会)
    tagSocket* (*accept)(tagSocket *sockfd, struct sockaddr *addr, int *addrlen);
    //让一个客户端去链接服务器
    int (*connect)(tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen);
    //发送数据
    int (*send)(tagSocket *sockfd, const void *msg, int len, int flags);
    //接收数据
    int (*recv)(tagSocket *sockfd, void *buf,int len, unsigned int flags);
    //直接发送数据到目的端
    int (*sendto)(tagSocket * sockfd, const void *msg,int len, unsigned int flags,\
              const struct sockaddr *addr, int addrlen);
    //直接从目的端读取数据
    int (*recvfrom)(tagSocket * sockfd,void *buf, int len, unsigned int flags,\
                struct sockaddr *addr, int *addrlen);
    //关闭一个套接口
#define SHUT_RD   0  //关闭读
#define SHUT_WR   1  //关闭写
#define SHUT_RDWR 2  //关闭读写
    int (*shutdown)(tagSocket *sockfd, u32 how);
    int (*close)(tagSocket *sockfd);
    int (*isactive)(tagSocket *sockfd,int mode);
    //设置套接字选项
    int (*setsockopt)(tagSocket *sockfd, int level, int optname,\
                   const void *optval, int optlen);
    //获取套接字选项
    int (*getsockopt)(tagSocket *sockfd, int level, int optname, void *optval,\
                   int *optlen);
    void (*debuginfo)(tagSocket *sockfd,char *filter);
}tagTlayerProto;

struct Socket
{
    //the following used by the proto
    struct Socket                  *nxt;         //nxt node
    struct MutexLCB                *sync;         //used to protect the socket
    struct MutexLCB                 syncmem;      //used for the sync mem
    tagTlayerProto                 *proto;        //protocol for the socket
    void                           *cb;           //the control node
    struct MultiplexObjectCB       *ioselect;     //used for the select;
    u32                             iostat;       //used for the select stat;
    u32                             errorno;      //record the socket errorno
    u32                             sockstat;     //which used by the specified protocal
    u32                             sockfd;       //the socket user fd
    ptu32_t                         private;      //used for the user, we could know who use the
                                                  //socket to communicate
    int                             af_inet;      //AF_FAMILY
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

//the sock layer only supply the sock malloc and free
tagSocket *SocketMalloc(void);
bool_t     SocketFree(tagSocket *sock);

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
ptu32_t socket_setprivate(int sock, ptu32_t private);
ptu32_t socket_getprivate(int sock);
int issocketactive(int sockfd, int mode);
void sockinfo(int sockfd,char *filter);
bool_t sockallinfo(char *param);
int getsockname(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
int getpeername(int sockfd,struct sockaddr *addr,socklen_t *addrlen);

//THIS IS THE COMPORT DEFINES FOR THE FREE SOFTWARE**********************//
//fcntl.h
/* open/fcntl - O_SYNC is only implemented on blocks devices and on files
   located on an ext2 file system */
#ifndef O_ACCMODE
#define O_ACCMODE   00000003
#endif

#ifndef O_RDONLY
#define O_RDONLY    00000000
#endif

#ifndef O_WRONLY
#define O_WRONLY    00000001
#endif

#ifndef O_RDWR
#define O_RDWR      00000002
#endif


#ifndef O_CREAT
#define O_CREAT     00000100    /* not fcntl */
#endif
#ifndef O_EXCL
#define O_EXCL      00000200    /* not fcntl */
#endif
#ifndef O_NOCTTY
#define O_NOCTTY    00000400    /* not fcntl */
#endif
#ifndef O_TRUNC
#define O_TRUNC     00001000    /* not fcntl */
#endif
#ifndef O_APPEND
#define O_APPEND    00002000
#endif
#ifndef O_NONBLOCK
#define O_NONBLOCK  00004000
#endif
#ifndef O_SYNC
#define O_SYNC      00010000
#endif
#ifndef FASYNC
#define FASYNC      00020000    /* fcntl, for BSD compatibility */
#endif
#ifndef O_DIRECT
#define O_DIRECT    00040000    /* direct disk access hint */
#endif
#ifndef O_LARGEFILE
#define O_LARGEFILE 00100000
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY 00200000    /* must be a directory */
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW  00400000    /* don't follow links */
#endif
#ifndef O_NOATIME
#define O_NOATIME   01000000
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC   02000000    /* set close_on_exec */
#endif
#ifndef O_NDELAY
#define O_NDELAY    O_NONBLOCK
#endif

#ifndef FD_CLOEXEC
#define FD_CLOEXEC     (1<<0)
#endif

#ifndef FD_SETSIZE
#define FD_SETSIZE     (10)    //the mac select num
#endif
enum __FCNTL_CMD
{
	F_SETFD = 0,
	F_GETFD,
	F_DUPFD,
	F_DUPFD_CLOEXEC,
	F_GETFL,
	F_SETFL,
};

#include <unistd.h>
extern int fcntl(int fd,int cmd,...);
extern off_t lseek(int fd,off_t offset, int whence);

//select.h
#include <sys/time.h>
#define CN_SELECT_MAXNUM      FD_SETSIZE
#define CN_SELECT_TIMEDEFAULT 10
typedef struct
{
	int  mode;
	u32  fd[CN_SELECT_MAXNUM];
}_types_fd_set;

#define fd_set _types_fd_set
#define ARG_MAX 256
int FD_SET(int fd, fd_set *sets);
int FD_CLR(int fd, fd_set *sets);
int FD_ISSET(int fd, fd_set *sets);
int FD_ZERO(fd_set *sets);

int select(int maxfd, fd_set *reads,fd_set *writes, fd_set *exps, \
           struct timeval *timeout);
int string2arg(int *argc, char *argv[],char *string);
bool_t string2Mac(char *str,u8 *mac);  //string must be xx-xx-xx-xx-xx-xx style
char* Mac2string(u8 *mac);

////////////////////////NETDEV DRIVER USED INTERFACE///////////////////////////////
//ip address defines
#ifndef INADDR_LOOPBACK
#define INADDR_LOOPBACK     0x7f000001     //127.0.0.1
#endif
#define INADDR_ANY          0
#define INADDR_NONE         0XFFFFFFFF
#define INADDR_BROAD        0XFFFFFFFF
#define INADDR_BROADCAST    INADDR_BROAD
#define INVALID_SOCKET      -1
#define CN_IPADDR_LEN_V4     4
#define CN_IPADDR_LEN_V6     16
#define CN_MACADDR_LEN       6
#define CN_IPADDR_ANY_V4     INADDR_ANY
#define CN_MAC_BROAD         "\xff\xff\xff\xff\xff\xff"

////////////////////////////////////////////////////////////////////////////////
//////////////////////////I WANT TO REBUILD THE NEW LINK ROUT///////////////////
//this defines for the debug
#define TCPIP_DEBUG_INC(x)     do{x++;}while(0)
#define TCPIP_DEBUG_DEC(x)     do{x--;}while(0)

#define CN_TCPIP_NAMELEN      32    //IN THE TCP IP STACK ,THIS IS THE NAME LEN LIMIT
typedef enum
{
    EN_NETDEV_SETNOPKG = 0,      //PARA IS NOT CARE
	EN_NETDEV_SETBORAD,          //para is int,0 disable else enable
	EN_NETDEV_SETPOINT,          //para is int,0 disable else enable
	EN_NETDEV_SETMULTI,          //para is int,0 disable else enable
	EN_NETDEV_SETRECV,           //para is int,0 disable else enable
	EN_NETDEV_SETSEND,           //para is int,0 disable else enable
    EN_NETDEV_SETMAC,            //para point to an buf which contains the mac
                                 //driver must modify the dev struct mac at the same time
	EN_NETDEV_SETMULTIMAC,       //para point to an buf which contains the mac
	EN_NETDEV_GTETMAC,           //para point to an buf which used to contain the mac
	EN_NETDEV_RESET,             //para must be true
	EN_NETDEV_CMDLAST,           //which means the max command
}enNetDevCmd;

typedef enum
{
	EN_NETDEV_FRAME_BROAD = 0,
	EN_NETDEV_FRAME_POINT,
	EN_NETDEV_FRAME_MULTI,
	EN_NETDEV_FRAME_LAST,
}enNetDevFramType;

typedef enum
{
	EN_NETDEV_FLOW_BROAD = EN_NETDEV_FRAME_BROAD,   //broad flow control type
	EN_NETDEV_FLOW_POINT = EN_NETDEV_FRAME_POINT,   //multi flow control type
	EN_NETDEV_FLOW_MULTI = EN_NETDEV_FRAME_MULTI,   //point flow control type
	EN_NETDEV_FLOW_FRAME,                           //frame flow control type
	EN_NETDEV_FLOW_LAST,
}enNetDevFlowType;
enNetDevFramType NetDevFrameType(u8 *buf,u16 len);
//frame flow control,there are four independent counter control module,you could
//each of them or all of them
//****************************************************************************//
//devname :the net device name
//type    :the frame type, which defines by enNetDevFlowType
//uflimit :the upper limit of the frame
//lflimit :the lower limit of the frame
//measuretime:during the time, if the flow is over, then triggle the corresponding event
//enable :1 true while 0 false
bool_t NetDevFlowSet(const char *devname,enNetDevFlowType type,\
		             u32 llimit,u32 ulimit,u32 period,int enable);
//handle:the device handle,returned by NetDevInstall
//type  :the frame type has received
bool_t NetDevFlowCounter(ptu32_t handle,enNetDevFramType type);

typedef enum _EN_LINK_INTERFACE_TYPE
{
    EN_LINK_ETHERNET = 0,  //ethenet net device,ethernetII
	EN_LINK_RAW,           //raw,just do the ip frames,no other link
	EN_LINK_80211,
    EN_LINK_LAST,
}enLinkType;

typedef enum
{
	EN_LINKPROTO_IPV4    = 0x0800,
	EN_LINKPROTO_IPV6    = 0x86dd,
	EN_LINKPROTO_ARP     = 0x0806,
	EN_LINKPROTO_RARP    = 0x8035,
	EN_LINKPROTO_RESBASE = 0x1000,
}enLinkProto;

//net device type
//netdev snd module function
//return means the data has put out or put into the net card buffer
//pkg maybe an lst or not,you could use the PkgIsEnd to check
//pkglen is fram len
typedef bool_t (*fnIfSend)(ptu32_t dev,tagNetPkg *pkglst,u32 framlen,u32 netdevtask);
//used to ctrl the dev or get the dev stat
typedef bool_t (*fnIfCtrl)(ptu32_t dev,enNetDevCmd cmd,ptu32_t para);
typedef struct NetDevPara
{
    const char                    *name;    //dev name
    u8                             iftype;   //dev type
    fnIfSend                       ifsend;   //dev snd function
    fnIfCtrl                       ifctrl;   //dev ctrl or stat get fucntion
    u32                            devfunc;  //dev hard function,such as tcp chksum
    u16                            mtu;      //dev mtu
    ptu32_t                        private;  //the dev driver use this to has its owner property
    u8                             mac[CN_MACADDR_LEN];   //mac address
}tagNetDevPara;  //we use the para to create an net device

//usage:for the net device event
//      all these event are edge-triggled
typedef enum
{
	//THE OLD ONES ARE DEPRECATED
	EN_NETDEVEVENT_LINKDOWN = 0, //which means the phy down or network cable is pullout
	EN_NETDEVEVENT_LINKUP,       //which means the phy up or network cable has been inserted ok
	EN_NETDEVEVENT_IPGET,        //which means has get ip from dhcp or ppp
	EN_NETDEVEVENT_IPRELEASE,    //which means has release ip to dhcp or ppp down
	EN_NETDEVEVENT_RESET,        //which means the dev has been reset for some reason
	EN_NETDEVEVENT_BROAD_OVER,   //means the broad over
	EN_NETDEVEVENT_BROAD_LACK,   //means the broad lack
	EN_NETDEVEVENT_MULTI_OVER,   //means the multi over
	EN_NETDEVEVENT_MULTI_LACK,   //means the multi lack,
	EN_NETDEVEVENT_POINT_OVER,   //means the point over
	EN_NETDEVEVENT_POINT_LACK,   //means the point lack,
	EN_NETDEVEVENT_FLOW_OVER,    //means the FLOW over
	EN_NETDEVEVENT_FLOW_LACK,    //means the FLOW lack,
	EN_NETDEVEVENT_RESERVED,     //which means nothing
}enNetDevEvent;
//the following we be cut in the socket.h
ptu32_t NetDevInstall(tagNetDevPara *para);          //return net dev handle, 0 failed
ptu32_t NetDevHandle(const char *name);
bool_t NetDevCtrl(const char *name,enNetDevCmd cmd, ptu32_t para);
bool_t NetDevCtrlByHandle(ptu32_t handle,enNetDevCmd cmd, ptu32_t para);
bool_t NetDevUninstall(const char *name);
bool_t NetDevPrivate(ptu32_t handle);
//link function that driver should use
bool_t LinkPost(ptu32_t devhandle,tagNetPkg *pkg);
//the hook function  module for the dev event
//handle:which device has triggled the event
//event :the message we has get
typedef bool_t (*fnNetDevEventHook)(ptu32_t handle,enNetDevEvent event);
//handle :the netdevice you install (returned by NetDevInstall)
//devname:if the netdevice is NULL,then we use the devname to search the device
//hook   :which used to deal the device message has been triggled
bool_t NetDevRegisterEventHook(ptu32_t handle,const char *devname,fnNetDevEventHook hook);
//handle :the netdevice you install (returned by NetDevInstall)
//devname:if the netdevice is NULL,then we use the devname to search the device
bool_t NetDevUnRegisterEventHook(ptu32_t handle,const char *devname);
//handle :the netdevice you install (returned by NetDevInstall)
//devname:if the netdevice is NULL,then we use the devname to search the device
//event  :the message want to send to the device
bool_t NetDevPostEvent(ptu32_t handle,const char *devname,enNetDevEvent event);
//the user could use the following api to listen on more protocol or send specified frames
typedef bool_t (*fnLinkProtoDealer)(ptu32_t devhandle,u16 proto,tagNetPkg *pkg);
bool_t LinkRegisterRcvHook(fnLinkProtoDealer hook, ptu32_t devhandle,u16 proto,const char *hookname);
bool_t LinkUnRegisterRcvHook(fnLinkProtoDealer hook, ptu32_t devhandle,u16 proto,const char *hookname);
bool_t LinkSendBufRaw(ptu32_t devhandle,u8 *buf,u32 len);

//the user could use these function to create delete modify the rout
//warning: ipaddr_t for ip, if ver is EN_IPV_4, then it is an u32 or an point
//         to u32, if EN_IPV_6, it must be an u8*, and the len is CN_IPADDR_LEN_V6
typedef ptu32_t   ipaddr_t;
typedef struct
{
    u32 ip;
    u32 dns;
    u32 gatway;
    u32 submask;
    u32 broad;
    u32 multi;
    u32 dnsbak;
}tagHostAddrV4;
typedef struct
{
    u32 ip[4];
    u32 dns[4];
    u32 gatway[4];
    u32 subnetmask[4];
    u32 broad[4];
}tagHostAddrV6;
#define CN_ROUT_DHCP    (1<<0)  //use this bit to get ip address from the dhcp server
#define CN_ROUT_NONE    (0)
bool_t RoutCreate(const char *name,enum_ipv_t ver,void *netaddr,u32 pro);
bool_t RoutDelete(const char *name,enum_ipv_t ver,ipaddr_t addr);
bool_t RoutSet(const char *name,enum_ipv_t ver,ipaddr_t ipold,void *newaddr);
bool_t RoutSetDefault(enum_ipv_t ver,ipaddr_t ip);
bool_t RoutDns(enum_ipv_t ver, ipaddr_t ip);
bool_t RoutSetDefaultAddr(enum_ipv_t ver,ipaddr_t ip,ipaddr_t mask,ipaddr_t gateway,ipaddr_t dns);
//this function use to alloc an ip from the dhcp dynamicly
bool_t DhcpAddClientTask(const char *name);
//this is the tcpip stack main entry
ptu32_t ModuleInstall_TcpIp(ptu32_t para);

//used for the multicast
#define MULTICAST_MASK     (htonl(0xf0000000))
#define MULTICAST_FLAG     (htonl(0xe0000000))
#define IN_MULTICAST(x)    (MULTICAST_MASK&x==MULTICAST_FLAG?true:false)
#define IN6_IS_ADDR_MULTICAST(x) (*(u8 *)x==0xff?true:false)


//add the mem here for the tcpip statistics for the internal call
bool_t tcpipmemlog(const char *name,int size, int num);
#define CN_LOCK_SIZE      (struct MutexLCB)

//some string deal functions in the tcp ip
char* Mac2string(u8 *mac);
bool_t string2Mac(char *str,u8 *mac);
int getargs(int argc, char *argv[],char *string);


//ftp client interface
//upload:put the local file(sfile) to the server(dfile)
//download:download the sourcefile(sfile)  to the local file(dfile) from the server
int ftpupload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);
int ftpdownload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);



#endif /* __SOCKET_H */

