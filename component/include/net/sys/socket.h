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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// ģ������: ���ļ��׽��ֵĶ��壬�Լ�ʹ�õĴ����Ľӿڱ�׼�Ķ���
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:15:08 PM/Jul 15, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
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
#define SOCK_STREAM     1        // �ɿ���˫���ֽ������� - TCP
#define SOCK_DGRAM      2        // ��õ���������ݱ����� - UDP
#define SOCK_RAW        3        // ��õ���������ݱ����� - ICMP IGMP ԭʼIP
#define SOCK_RDW        4        // �ɿ������ݱ�����δʵ�֣� - n/a
#define SOCK_SEQPACKET  5        // �ɿ���˫���¼������ - n/a

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
    SO_BROADCAST = 0 , //�������͹㲥���ݡ�����������������������int
    SO_DEBUG,          //�������ԡ�������������������������������int
    SO_DONTROUTE,      //������·�ɡ�����������������������������int
    SO_ERROR,          //����׽��ִ��󡡡�����������������������int
    SO_KEEPALIVE,      //�������ӡ�������������������������������int
    SO_LINGER,         //�ӳٹر����ӡ���������������������������struct linger
    SO_OOBINLINE,      //�������ݷ�������������������������������int
    SO_RCVBUF,         //���ջ�������С��������������������������int
    SO_SNDBUF,         //���ͻ�������С��������������������������int
    SO_RCVLOWAT,       //���ջ��������ޡ�������������������������int
    SO_SNDLOWAT,       //���ͻ��������ޡ�������������������������int
    SO_RCVTIMEO,       //���ճ�ʱ��������������������������������struct timeval
    SO_SNDTIMEO,       //���ͳ�ʱ��������������������������������struct timeval
    SO_REUSEADDR,     //�������ñ��ص�ַ�Ͷ˿ڡ�����������������  int
    SO_TYPE,           // ����׽������͡�����������������������  int
    SO_BSDCOMPAT,      //��BSDϵͳ���ݡ������������������������� int
    SO_NOBLOCK,        //DJY ������                                                    int
};
//IPPROTO_IP
enum _EN_IPPROTO_IP_OPTNAME
{
    //OPTNAME              MEANS                           optvaltype
    IP_HDRINCL = 0,   //�����ݰ��а���IP�ײ���������������������int
    IP_OPTIONS    ,   //IP�ײ�ѡ�����������������������������int
    IP_TOS        ,   //��������                                                        int
    IP_TTL        ,   //����ʱ�䡡������������������������������int
};
//IPPROTO_TCP
enum _EN_IPPROTO_TCP_OPTNAME
{
    //OPTNAME              MEANS                           optvaltype
    TCP_MAXSEG = 0,      //TCP������ݶδ�С������������������ int
    TCP_NODELAY   ,      //��ʹ��Nagle�㷨�������������������� int
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
//�����Э��ӿڶ���
//����㶨��Ľӿ�,���������Э�飬�ȷ�TCP,UDP�ȱ��밴������ӿ���ʵ��
//���ڲ���Ҫ�ĺ����ӿڣ������ÿ�
//����ֵΪ��׼��BSD����ֵ
typedef struct TlayerProto
{
    //����һ���׽���
    tagSocket* (*socket)(int family, int type, int protocol);
    //��һ���˿ں�
    int (*bind)(tagSocket *sockfd,struct sockaddr *myaddr, int addrlen);
    //��һ���˿ڴ��ڼ���״̬
    int (*listen)(tagSocket *sockfd, int backlog);
    //��һ�������˴��ڽ���״̬(�������Ż�)
    tagSocket* (*accept)(tagSocket *sockfd, struct sockaddr *addr, int *addrlen);
    //��һ���ͻ���ȥ���ӷ�����
    int (*connect)(tagSocket *sockfd, struct sockaddr *serv_addr, int addrlen);
    //��������
    int (*send)(tagSocket *sockfd, const void *msg, int len, int flags);
    //��������
    int (*recv)(tagSocket *sockfd, void *buf,int len, unsigned int flags);
    //ֱ�ӷ������ݵ�Ŀ�Ķ�
    int (*sendto)(tagSocket * sockfd, const void *msg,int len, unsigned int flags,\
              const struct sockaddr *addr, int addrlen);
    //ֱ�Ӵ�Ŀ�Ķ˶�ȡ����
    int (*recvfrom)(tagSocket * sockfd,void *buf, int len, unsigned int flags,\
                struct sockaddr *addr, int *addrlen);
    //�ر�һ���׽ӿ�
#define SHUT_RD   0  //�رն�
#define SHUT_WR   1  //�ر�д
#define SHUT_RDWR 2  //�رն�д
    int (*shutdown)(tagSocket *sockfd, u32 how);
    int (*close)(tagSocket *sockfd);
    int (*isactive)(tagSocket *sockfd,int mode);
    //�����׽���ѡ��
    int (*setsockopt)(tagSocket *sockfd, int level, int optname,\
                   const void *optval, int optlen);
    //��ȡ�׽���ѡ��
    int (*getsockopt)(tagSocket *sockfd, int level, int optname, void *optval,\
                   int *optlen);
    void (*debuginfo)(tagSocket *sockfd,char *filter);
}tagTlayerProto;

struct Socket
{
    //the following used by the proto
    void                           *obj;     //used for the socket layqueue
    struct Socket                  *nxt;         //nxt node
    struct MutexLCB                *sync;         //used to protect the socket
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

//lingerѡ��
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
tagSocket *socket_malloc(void);
bool_t     socket_free(tagSocket *sock);
bool_t     socket_setsatus(tagSocket *sock,u32 status);
bool_t     socket_clrsatus(tagSocket *sock,u32 status);
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

#include <unistd.h>
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


//bool_t RoutCreate(const char *name,enum_ipv_t ver,void *netaddr,u32 pro);
//bool_t RoutDelete(const char *name,enum_ipv_t ver,ipaddr_t addr);
//bool_t RoutSet(const char *name,enum_ipv_t ver,ipaddr_t ipold,void *newaddr);
//bool_t RoutSetDefault(enum_ipv_t ver,ipaddr_t ip);
//bool_t RoutDns(enum_ipv_t ver, ipaddr_t ip);
//bool_t RoutSetDefaultAddr(enum_ipv_t ver,ipaddr_t ip,ipaddr_t mask,ipaddr_t gateway,ipaddr_t dns);
//this function use to alloc an ip from the dhcp dynamicly
bool_t DhcpAddClientTask(const char *name);

//used for the multicast
#define MULTICAST_MASK     (htonl(0xf0000000))
#define MULTICAST_FLAG     (htonl(0xe0000000))
#define IN_MULTICAST(x)    (MULTICAST_MASK&x==MULTICAST_FLAG?true:false)
#define IN6_IS_ADDR_MULTICAST(x) (*(u8 *)x==0xff?true:false)

//ftp client interface
//upload:put the local file(sfile) to the server(dfile)
//download:download the sourcefile(sfile)  to the local file(dfile) from the server
int ftpupload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);
int ftpdownload(const char *host,const char *port,const char *user,const char *passwd,const char *sfile,const char *dfile);



#endif /* __SOCKET_H */
