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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
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
#include <shell.h>
#include <multiplex.h>

//һ��PKGLST�ڴ���Ĺ����У���ĳ��PKGӵ��CN_PKLGLST_END��ǻ���NULL == partnext��������Ϊ
//��PKGLST�������������ڷ��͵�ʱ����������
#define CN_PKLGLST_END   (1<<0)
//tagNetPkg��ԭ��
//bufsize������ʱָ����ʹ�ù�����һֱ����;dataһֱָ��buf����ʼλ�ã����ֲ���
//����PKGд������ʱ��offset���䣬��buf��offset+datalen�ĵط���ʼд�����ݣ�д��֮��
//                  datalen +=len(lenΪд�����ݳ���)
//����PKG��ȡ����ʱ����buf��offset��ʼcpy��cpy���֮��
//                  offset += len,datalen -= len(lenΪȡ�����ݳ���)
typedef struct NetPkg
{
    struct NetPkg   *partnext;        //����ָ�븺�����ݰ���Э��ջ�Ĵ���
    ptu32_t  private;   //used for the module who malloc the package
    u8   level;         // PKG�Ĵ�С�ȼ�
    u8   pkgflag;       // PKG�ĺ�����չ����
    u8   refers;        // ����Ĵ���
    u16  datalen;       // buf�е���Ч���ݳ���
    u16  bufsize;       // buf�ĳ���
    u16  offset;        // ��Ч����ƫ��buf��λ�ã�offset֮ǰ��������Ч,���ֲ����ݻ������ݶ����ʱ�������
    u8   *buf;          // pkg��buf�����ݻ�������
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

//this is for the
#define SOMAXCONN       100

//this is the domain family
#define AF_UNSPEC		(0) /* unspecified */
#define AF_LOCAL		(1) /* local to host (pipes, portals) */
#define AF_UNIX			AF_LOCAL /* backward compatibility */
#define AF_INET			(2) /* internetwork: UDP, TCP, etc. */
#define PF_INET         AF_INET
#define AF_IMPLINK		(3) /* arpanet imp addresses */
#define AF_PUP			(4) /* pup protocols: e.g. BSP */
#define AF_CHAOS        (5) /* mit CHAOS protocols */
#define AF_NS			(6) /* XEROX NS protocols */
#define AF_ISO			(7) /* ISO protocols */
#define AF_OSI			AF_ISO
#define AF_ECMA			(8) /* european computer manufacturers */
#define AF_DATAKIT		(9) /* datakit protocols */
#define AF_CCITT        (10) /* CCITT protocols, X.25 etc */
#define AF_SNA			(11) /* IBM SNA */
#define AF_DECnet		(12) /* DECnet */
#define AF_DLI			(13) /* DEC Direct data link interface */
#define AF_LAT			(14) /* LAT */
#define AF_HYLINK		(15) /* NSC Hyperchannel */
#define AF_APPLETALK    (16) /* Apple Talk */
#define AF_ROUTE        (17) /* Internal Routing Protocol */
#define AF_LINK			(18) /* Link layer interface */
#define pseudo_AF_XTP	(19) /* eXpress Transfer Protocol (no AF) */
#define AF_COIP			(20) /* connection-oriented IP, aka ST II */
#define AF_CNT			(21) /* Computer Network Technology */
#define pseudo_AF_RTIP	(22) /* Help Identify RTIP packets */
#define AF_IPX			(23) /* Novell Internet Protocol */
#define AF_SIP			(24) /* Simple Internet Protocol */
#define pseudo_AF_PIP	(25) /* Help Identify PIP packets */

// type
#define SOCK_STREAM		1        // �ɿ���˫���ֽ������� - TCP
#define SOCK_DGRAM		2        // ��õ���������ݱ����� - UDP
#define SOCK_RAW        3        // ��õ���������ݱ����� - ICMP IGMP ԭʼIP
#define SOCK_RDW        4        // �ɿ������ݱ�����δʵ�֣� - n/a
#define SOCK_SEQPACKET	5        // �ɿ���˫���¼������ - n/a

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


//BSD��׼�������ַ�ṹ
//�ں�ʹ�õ�sockaddr�ṹ
struct sockaddr
{
    u16 sa_family;    // ��ַ�壬AF_INET/AF_ISO/AP_UNIX������AF_xxx
    char sa_data[14]; // 14 �ֽڵ�Э���ַ �������socket�Ķ˿ںź�IP��ַ
};
struct in_addr
{
    unsigned long s_addr;
};
// Ϊ�˸�����ʹ��sockaddr�������Զ���һ��socketaddr�ṹ���û�����ʹ�ñ��ṹ�����ò���
struct sockaddr_in
{
    u16             sin_family;    // ��ַ�壬AF_INET/AF_ISO/AP_UNIX������AF_xxx
    u16             sin_port;      // �˿ڵ�ַ�������ֽ���
    struct in_addr  sin_addr;      // IP��ַ
    u8              sin_zero[8];   // ����ֽ�
};

//SOL_SOCKET
enum _EN_SOL_SOCKET_OPTNAME
{
    //OPTNAME              MEANS                            optvaltype
    SO_BROADCAST = 0 , //�����͹㲥���ݡ�����������������������int
    SO_DEBUG,          //������ԡ�������������������������������int
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

#define CN_IPV6_LEN_WORD    4
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

//netdb.h
/* Description of data base entry for a single host.  */
struct hostent
{
	char * h_name;			/* Official name of host.  */
	char **h_aliases;		/* Alias list.  */
	int    h_addrtype;		/* Host address type.  */
	int    h_length;	    /* Length of address.  */
	char **h_addr_list;		/* List of addresses from name server.  */
	# define	h_addr	h_addr_list[0] /* Address, for backward compatibility.*/
};
/* Description of data base entry for a single service.  */
struct servent
{
	char      *s_name;			/* Official service name.  */
	char     **s_aliases;		/* Alias list.  */
	int        s_port;			/* Port number.  */
	char      *s_proto;		    /* Protocol to use.  */
};

/* Description of data base entry for a single service.  */
struct protoent
{
	char    *p_name;			/* Official protocol name.  */
	char   **p_aliases;		    /* Alias list.  */
	int      p_proto;			/* Protocol number.  */
};

struct servent *getservbyname(const char *name, const char *proto);
u32 inet_addr(const char *addr);
char *inet_ntoa(struct in_addr addr);
int inet_aton(const char *string,struct in_addr *addr);
struct hostent *gethostbyname(const char *name);
int  gethostname(char *name, int len);
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);

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
	EN_NETDEV_FRAME_NOPKG = 0,
	EN_NETDEV_FRAME_BROAD,
	EN_NETDEV_FRAME_POINT,
	EN_NETDEV_FRAME_MULTI,
	EN_NETDEV_FRAME_LAST,
}enNetDevFramType;
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

typedef enum _EN_LINK_INTERFACE_TYPE
{
    EN_LINK_ETHERNET = 0,  //ethenet net device,ethernetII
	EN_LINK_RAW,           //raw,just do the ip frames,no other link
	EN_LINK_80211,
    EN_LINK_LAST,
}enLinkType;

typedef enum
{
	EN_LINKPROTO_IPV4 = 0x0800,
	EN_LINKPROTO_IPV6 = 0x86dd,
	EN_LINKPROTO_ARP = 0x0806,
	EN_LINKPROTO_RARP = 0x8035,
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
//the following we be cut in the socket.h
ptu32_t NetDevInstall(tagNetDevPara *para);          //return net dev handle, 0 failed
ptu32_t NetDevHandle(const char *name);
bool_t NetDevCtrl(const char *name,enNetDevCmd cmd, ptu32_t para);
bool_t NetDevCtrlByHandle(ptu32_t handle,enNetDevCmd cmd, ptu32_t para);
bool_t NetDevUninstall(const char *name);
bool_t NetDevPrivate(ptu32_t handle);
//link function that driver should use
bool_t LinkPost(ptu32_t devhandle,tagNetPkg *pkg);
//this is used for the net device filter
bool_t NetDevFilterSet(const char *name,enNetDevFramType type,u32 framelimit,\
		u32 actiontime,u32 measuretime);
bool_t NetDevFilterEnable(const char *name,enNetDevFramType type,bool_t enable);
bool_t NetDevFilterCounter(ptu32_t handle,enNetDevFramType type);
bool_t NetDevFilterCheck(ptu32_t handle);

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
bool_t *RoutSetDefaultAddr(enum_ipv_t ver,ipaddr_t ip,ipaddr_t mask,ipaddr_t gateway,ipaddr_t dns);
//this function use to alloc an ip from the dhcp dynamicly
bool_t DhcpAddClientTask(const char *name);
//this is the tcpip stack main entry
ptu32_t ModuleInstall_TcpIp(ptu32_t para);

#endif /* __SOCKET_H */

