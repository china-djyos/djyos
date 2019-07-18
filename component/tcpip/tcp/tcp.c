//----------------------------------------------------
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
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得书面许可，不允许移植到非
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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dbug.h"

#include "../common/router.h"
#include "../common/tpl.h"
#include "../common/ip.h"
#include "../common/ipV4.h"
#include "../common/netpkg.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"tcp"          //tcp协议
//parent:"tcpip"                //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","heap","device file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TCP == false )
//#warning  " tcp  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TCP    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,,,
//%$#@enum,true,false,
#define     CFG_TCP_REORDER             true    //"TCP乱序重组使能",资源充足才打开
//%$#@num,,,
#define     CFG_TCP_CCBNUM              10      //"tcp 客户端数限值"，占一个 指针 和 struct ClienCB
#define     CFG_TCP_SCBNUM              5       //"tcp 服务器数限值"，占一个 指针 和 struct ServerCB
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure



//some defines for the tcp
//LOCAL MACHINE STAT ENUM
enum _EN_TCPSTATE
{
    EN_TCP_MC_CREAT = 0 ,  //刚被创建的状态，一般而言只有主动创建的才会在此状态
    EN_TCP_MC_SYNRCV,      //服务器端收到了FIN
    EN_TCP_MC_SYNSNT,      //客户端已经发送了FIN信号
    EN_TCP_MC_ESTABLISH,   //稳定链接状态
    EN_TCP_MC_FINWAIT1,    //主动关闭发送了FIN
    EN_TCP_MC_FINWAIT2,    //主动关闭发送了FIN并且收到了ACK
    EN_TCP_MC_CLOSING,     //主动关闭发送了FIN(未收到ACK)，收到对面的FIN
    EN_TCP_MC_TIMEWAIT,    //主动关闭发送FIN并且接收到了FIN，2ML超时
    EN_TCP_MC_CLOSEWAIT,   //被动关闭收到了对面的FIN
    EN_TCP_MC_LASTACK,     //被动关闭发送了FIN
    EN_TCP_MC_2FREE,       //待释放状态
};
//some default defines for the tcp
#define CN_TCP_BUFTRIGDEFAULT   0           //TRIGGER LEVEL DEFAULT
#define CN_TCP_SNDBUF_SIZEDEFAULT  0x800    //默认 tcp 发送 buffer size
#define CN_TCP_RCVBUF_SIZEDEFAULT  0x800    //默认 tcp 接收 buffer size
#define CN_TCP_MSSDEFAULT          1460     //默认 tcp mss size
#define CN_TCP_LISTENDEFAULT    5           //LISTEN BAKLOG DEFAULT
#define CN_TCP_RCMBLENDEFAULT   0x10        //most allowing num frame in the recombine list
#define CN_TCP_ACCEPTMAX        0x10000

//define for the channel stat
//rcv fin makes krcv 0
//snt fin makes ksnd 0
//shutdown_rd makes krcv and arcv 0
//shutdown_wr makes asnd 0
//close makes the ksnd krcv 0 arcv 0 asnd 0
#define CN_TCP_CHANNEL_STATASND    (1<<0)  //APP COULD SND DATA
#define CN_TCP_CHANNEL_STATARCV    (1<<1)  //APP COULD NOT SND DATA
#define CN_TCP_CHANNEL_STATKSND    (1<<2)  //STACK COULD SND DATA
#define CN_TCP_CHANNEL_STATKRCV    (1<<3)  //STAK COULD RCV DATA
#define CN_TCP_CHANNEL_STATCONGEST (1<<4)  //the rcv window is full or channel is bad

//define for the tcp timer
#define CN_TCP_TICK_TIME                   (100*mS) //Units:Micro Seconds
#define CN_TCP_TICK_2ML                    (10)     //unit:tcp tick
#define CN_TCP_TICK_KEEPALIVE              (500*10) //unit:tcp tick
#define CN_TCP_TICK_PERSIST                (10)     //unit:tcp tick
#define CN_TCP_TICK_ACK                    (5)      //Unit:package number
#define CN_TCP_TIMER_SYN                   (1<<0)   //SYNC TIMER
#define CN_TCP_TIMER_2MSL                  (1<<1)   //2ML TIMER control bit
#define CN_TCP_TIMER_KEEPALIVE             (1<<2)   //KEEP TIMER control
#define CN_TCP_TIMER_PERSIST               (1<<3)   //PERSIST TIMER control bit
#define CN_TCP_TIMER_CORK                  (1<<4)   //CORK TIMER control bit
#define CN_TCP_TIMER_LINGER                (1<<5)   //LINGER TIMER START
#define CN_TCP_TIMER_RESEND                (1<<6)   //RESEND TIMER
#define CN_TCP_TIMER_FIN                   (1<<7)   //FIN TIMER
#define CN_TCP_TIMER_NONE                  (0)      //NO TIMER IS START
//define for the rtt
#define CN_TCP_RTOMAX                      150    //unit:tcp tick
#define CN_TCP_RTOMIN                      2      //unit:tcp tick
#define CN_TCP_SA_INIT                     0
#define CN_TCP_SD_INIT                     4      //unit:tcp tick
#define CN_TCP_FASTRETRANS                 3
#define CN_TCP_CWND_LIMIT                  60000
#define CN_TCP_RSNDMAX                     8     //RESND TIMES MOST
//tcp flags
#define CN_TCP_FLAG_FIN  (1<<0)     //网络位序也分大小端么？
#define CN_TCP_FLAG_SYN  (1<<1)
#define CN_TCP_FLAG_RST  (1<<2)
#define CN_TCP_FLAG_PSH  (1<<3)
#define CN_TCP_FLAG_ACK  (1<<4)
#define CN_TCP_FLAG_URG  (1<<5)
#define CN_TCP_FLAG_MSK  0x3f
#pragma pack(1)
struct TcpHdr
{
    u16 portsrc;     //source port
    u16 portdst;     //dstination port
    u32 seqno;       //sequence number
    u32 ackno;       //acknowledge number
    u8  hdroff;
    u8  flags;       //some flags like sync and ack
    u16 window;      //the remote window size
    u16 chksum;      //package checksum
    u16 urgencyp;    //urgency pointer
    u8  opt[0];      //optional
};
//this structure is used in the option to set the windowscale
typedef struct WindowScale
{
    u8 resvered;
    u8 kind;
    u8 len;
    u8 shiftbits;
}tagWindowScale;
//this structure is used in the option to set the mss
typedef struct Mss
{
    u8  kind;
    u8  len;
    u16 mss;
}tagMss;
//we use the fixed mss and window scale in the syn handshake
typedef struct SynOption
{
    tagMss mss;
    tagWindowScale winscale;
}tagSynOption;
#pragma pack()
//receive buffer
typedef struct
{
    struct SemaphoreLCB      *bufsync;     //the buffer sync member
    u32                       timeout;     //if block, the block time
    s32                       buflen;      //the valid data length in the buffer
    s32                       buflenlimit; //the buffer length
    s32                       trigerlevel; //the buffer trigerlevel
    struct NetPkg            *ph;          //the buffer package head
    struct NetPkg            *pt;          //the buffer package tail
    u32                       rcvnxt;      //this is we receive next number
}tagRecvBuf;
//send buffer
typedef struct
{
    struct SemaphoreLCB      *bufsync;     //buflenleft超过trigerlevel则post
    u32                       timeout;     //if block, the block time
    u32                       rtotick;     //最后一次发送数据 以来的ticks数
    u8                       *tcpsendbuf;  //tcp发送buf，malloc得到。
    s32                       buflenlimit; //the buffer length
    u32                       unackno;     //which means the unack sequence
    s32                       unackoff;    //which means off the buf to ack
    s32                       unacklen;     //which means the length between the uack and sndnxt;
    u32                       sndnxtno;    //which means the sequence no to send
    s32                       sndnxtoff;   //which means off the buf to send
    s32                       dataoff;     //which means off the buf,and buf from dataoff to unackoff could be write
    s32                       datalen;     //which means the unsend data length
    s32                       buflenleft;  //the net_free buffer len,which means how many data you could use
    s32                       trigerlevel; //the buffer trigerlevel,if the bufleftlen is less than this ,then will active the sync
                                           //the default is the size of the buffer
    u8                        dupacktims;  //when receive the same ack,add it,when over,do the resend
}tagSendBuf;  //this buf used for the tcp send
//each client has an ClienCB for the tcp state control
struct ClienCB
{
    struct ClienCB           *nxt;          //用于动态分配内存块
    struct tagSocket         *server;       //if this is an accept one
    u16                       machinestat;  //the machine stat of the tcb
    u16                       channelstat;  //the stat of the channel,which mean we could recv or send
    tagRecvBuf                rbuf;         //rcv buffer
    tagSendBuf                sbuf;         //sendbuffer
    struct NetPkg            *pkgrecomblst; //the package recomb queue
    u16                       pkgrecomblen; //the package recomb queue length
    //the send window member
    u16                       mss;         //you could send the most data one time
    u8                        sndwndscale; //the remote window scale,update by the handshake
    s32                       cwnd;        //the congest avoiding window
    s32                       ssthresh;    //slow thresh,default 65535
    s32                       sndwnd;      //the remote receive window,update by the receive frame
    //round trip time measure
    /* RTT(round trip time)相关变量 */
    /*
    Err = M-A           //A是RTT平均值，M是实际测量值，Err是误差
    A←A + gErr         //用误差更新平均值
    D←D + h( | Err |-D) //D是均值偏差，用误差更新均值偏差
    RTO = A + 4D        //计算得到RTO (重传超时时间)  (初始化时RTO = A + 2D  )
    g=0.125 ; h=0.25
    */
    s16 sa, sd;                /* 用于计算RTO：sa = 8*A；sv = 4*D */
    s16 rto;                   /* RTO */
    //tcp timer
    u8                         timerctrl;
    u16                        mltimer;     //(unit:tcp tick,used for 2msl wait)
    u16                        lingertimer; //(unit:tcp tick,used for linger close)
    u16                        keeptimer;   //(unit:tcp tick,used for keep alive)
    u16                        persistimer; //(unit:tcp tick,used for probe window)
    u16                        corktimer;   //(unit:tcp tick,used for cork timeout)
    u8                         resndtimes;  //when resend,add it;when ack the data then clear it
    u8                         resndtimer;  //when resend time is zero,do the resend
    u8                         acktimes;    //if acktimes overrun the limit, then will do fast ack
};
//each server has an ServerCB
struct ServerCB
{
    struct ServerCB           *nxt;                 //用于动态分配内存块
    s32                        backlog;             //which limit the pending num
    s32                        pendnum;             //which means how much still in pending
    u32                        accepttime;          //block time for the accept
    struct tagSocket             *clst;                //all the client including the pending stat
    struct SemaphoreLCB       *acceptsemp;          //if block, then wait this num
};  //tcp server control block
//we use this structure to statistics the tcp state
typedef struct
{
    u32  ccbnum;
    u32  ccbfreenum;
    u32  scbnum;
    u32  scbfreenum;
}tagTcpStatistics;
//define for the register in the tpl layer
static struct TPL_ProtocalOps         gTcpProto;
//used for the connection to snd syn (server or client)  tcp option
static tagSynOption           sgSynOptDefault;   //when snd syn,use this const option
//we use the hash table to search the socket
typedef struct
{
    s32                          tablen;     //how long the hash tab is
    struct MutexLCB             *tabsync;    //used to peotect the hash tab
    struct tagSocket               *array[0];   //this is the hash tab
}tagTcpHashTab;
static tagTcpHashTab   *pTcpHashTab = NULL;
static void *pTcpTicker = NULL;
// =============================================================================
// FUNCTION：this function is used to initialize the tcp hash tab
// PARA  IN：len ,this parameter limits the hashtab length
// PARA OUT：
// RETURN  ：
// INSTRUCT:
// =============================================================================
static bool_t __hashTabInit(u32 len)
{
    bool_t result = false;
    pTcpHashTab = net_malloc(sizeof(tagTcpHashTab) + len *sizeof(struct tagSocket *));
    if(NULL == pTcpHashTab)
    {
        goto ERR_ARRAYMEM;
    }
    memset((void *)pTcpHashTab,0,sizeof(tagTcpHashTab) + len *sizeof(struct tagSocket *));

    pTcpHashTab->tabsync = mutex_init(NULL);
    if(NULL == pTcpHashTab->tabsync)
    {
        goto ERR_SYNC;
    }

    pTcpHashTab->tablen = len;

    result = true;
    return result;

ERR_SYNC:
    net_free(pTcpHashTab);
    pTcpHashTab = NULL;
ERR_ARRAYMEM:
    return result;
}
// =============================================================================
// FUNCTION：this function is used to find an socket in the hashtab
// PARA  IN：the specified ip and port determines the item together
// PARA OUT：
// RETURN  ：the socket find else NULL(no one mathes)
// INSTRUCT:send an mail to the mail box, which to be dealt by the ip engine
// =============================================================================
//-----------------------------------------------------------------------------
//功能: 从hash表中查找socket。
//参数: iplocal，本地IP
//      portlocal，本地端口
//      ipremote，远程IP
//      portremote，远程端口
//返回: socket指针，或NULL
//-----------------------------------------------------------------------------
static struct tagSocket *__hashSocketSearch(u32 iplocal, u16 portlocal,u32 ipremote,u16 portremote)
{
    struct tagSocket *result = NULL;

    struct tagSocket *tmp;
    u32 hashKey;

    hashKey = iplocal+portlocal + ipremote +portremote;
    hashKey = hashKey%pTcpHashTab->tablen;
    tmp = pTcpHashTab->array[hashKey];
    while((NULL != tmp))
    {
        if((iplocal == tmp->element.v4.iplocal)&&(portlocal == tmp->element.v4.portlocal)&&\
           (ipremote == tmp->element.v4.ipremote)&&(portremote == tmp->element.v4.portremote))
        {
            result = tmp;
            break;
        }
        else
        {
            tmp = tmp->Nextsock;
        }
    }

    return result;
}

//this function is only used for the bind function
//when we bind a address,we must make sure than if any socket with the same address
//if any socket find with the same address, then bind failed
static struct tagSocket *__hashSocketLocalSearch(u32 iplocal, u16 portlocal)
{
    struct tagSocket *result = NULL;

    s32 i = 0;
    struct tagSocket *tmp;

    for(i =0; i < pTcpHashTab->tablen;i++ )
    {
        tmp = pTcpHashTab->array[i];
        while((NULL != tmp))
        {
            if((iplocal == tmp->element.v4.iplocal)&&(portlocal == tmp->element.v4.portlocal))
            {
                result = tmp;
                break;
            }
            else
            {
                tmp = tmp->Nextsock;
            }
        }
        if(NULL != tmp)
        {
            break;
        }
    }

    return result;
}
// =============================================================================
// FUNCTION:this function is used to create an hash item
// PARA  IN:ip and port are the only hash property we need
// PARA OUT：
// RETURN  :the hash item we create
// INSTRUCT:add the create item to the last
// =============================================================================
static bool_t __hashSocketAdd(struct tagSocket *sock)
{
    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal + v4->portlocal + v4->ipremote + v4->portremote;
    hashKey = hashKey%pTcpHashTab->tablen;
    tmp = pTcpHashTab->array[hashKey];
    if(NULL == tmp)
    {
        pTcpHashTab->array[hashKey] = sock;
        sock->Nextsock = NULL;
    }
    else
    {
        while(NULL != tmp->Nextsock)
        {
            tmp = tmp->Nextsock;
        }
        tmp->Nextsock = sock;
        sock->Nextsock = NULL;
    }
    return true;
}

// =============================================================================
// FUNCTION:this function is used to net_free the item created
// PARA  IN:ip and port are the only hash property we need
// PARA OUT：
// RETURN  :the hash item we create
// INSTRUCT:add the create item to the last
// =============================================================================
static bool_t __hashSocketRemove(struct tagSocket *sock)
{

    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal+ v4->portlocal+ v4->ipremote + v4->portremote;
    hashKey = hashKey%pTcpHashTab->tablen;
    tmp = pTcpHashTab->array[hashKey];
    if(sock == tmp)
    {
        pTcpHashTab->array[hashKey] = sock->Nextsock;
        sock->Nextsock = NULL;
    }
    else
    {
        while((NULL != tmp)&&(sock != tmp->Nextsock))
        {
            tmp = tmp->Nextsock;
        }
        if(NULL != tmp)
        {
            tmp->Nextsock = sock->Nextsock;
            sock->Nextsock = NULL;
        }
    }
    return true;
}
//this function used to compute a send sequence number;make sure you could not guess
//the sequence number when handshake
static u32 __computeIsn(void)
{
    #define CN_TCP_ISNPRE_MS    (250)
    #define CN_ISN_PER_CONNECT  (64000)
    static u16 sIsnCnt = 0;
    u32 result;

    result = (u32)((sIsnCnt++)*CN_ISN_PER_CONNECT +
            DjyGetSysTime()*mS*CN_TCP_ISNPRE_MS);
    return result;
}

static struct MutexLCB *pCBSync = NULL;      //use this to protect the
static struct ClienCB          *pCCBFreeList = NULL; //this is used for the net_free ccb queue
static struct ServerCB          *pSCBFreeList = NULL; //this is used for the net_free scb queue
//this function is used to initialize the ccb and scb module
//net_malloc the mem from the configuration and create the sync
//after the initialize, we net_malloc CB from the net_free queue
static bool_t  __initCB(s32 ccbnum, s32 scbnum)
{
    s32 i = 0;

    if((ccbnum <= 0) || (scbnum <= 0))
        return false;
    pCBSync = mutex_init(NULL);

    //do the ccb initialize
    pCCBFreeList = net_malloc(ccbnum *sizeof(struct ClienCB));
    if(NULL == pCCBFreeList)
    {
        goto CCB_MEM;
    }

    memset(pCCBFreeList, 0, (ccbnum *sizeof(struct ClienCB)));

    //do ClienCB initialize
    for(i=0;i <(ccbnum -1);i++)
    {
        pCCBFreeList[i].nxt = &pCCBFreeList[i +1];
    }
    pCCBFreeList[i].nxt = NULL;
   //do the scb initialize
    pSCBFreeList = net_malloc(scbnum *sizeof(struct ServerCB));
    if(NULL == pSCBFreeList)
    {
        goto SCB_MEM;
    }

    memset(pSCBFreeList, 0, (scbnum *sizeof(struct ServerCB)));

    //do ServerCB initialize
    for(i=0;i <(scbnum -1);i++)
    {
        pSCBFreeList[i].nxt = &pSCBFreeList[i +1];
    }
    pSCBFreeList[i].nxt = NULL;

    return true;

SCB_MEM:
    net_free((void *)pCCBFreeList);
    pCCBFreeList = NULL;
CCB_MEM:
    mutex_del(pCBSync);
    pCBSync = NULL;
    return false;
}

//net_malloc a ccb
static struct ClienCB  *mallocccb(void)
{
    struct ClienCB           *result = NULL;
    if(mutex_lock(pCBSync))
    {
        if(NULL != pCCBFreeList)
        {
            result = pCCBFreeList;
            pCCBFreeList = result->nxt;
        }

        mutex_unlock(pCBSync);
    }
    return result;
}
//net_free a ccb
static bool_t  freeccb(struct ClienCB  *ccb)
{
    if(mutex_lock(pCBSync))
    {
        ccb->nxt = pCCBFreeList;
        pCCBFreeList = ccb;

        mutex_unlock(pCBSync);
    }
    return true;
}
//net_malloc a scb
static struct ServerCB  *mallocscb(void)
{
    struct ServerCB           *result = NULL;

    if(mutex_lock(pCBSync))
    {
        if(NULL != pSCBFreeList)
        {
            result = pSCBFreeList;
            pSCBFreeList = result->nxt;
        }
        mutex_unlock(pCBSync);
    }
    return result;
}
//net_free a scb
static bool_t  freescb(struct ServerCB  *scb)
{
    if(mutex_lock(pCBSync))
    {
        scb->nxt = pSCBFreeList;
        pSCBFreeList = scb;

        mutex_unlock(pCBSync);
    }
    return true;
}

// =============================================================================
// FUNCTION:this function to net_malloc an ccb and init it
// PARA  IN:
// PARA OUT：
// RETURN  :
// INSTRUCT:
// =============================================================================
//net_malloc a ccb and init the member in it
static struct ClienCB  *__CreateCCB(void)
{
    struct ClienCB *result;

    result =  mallocccb();
    if(NULL == result)
    {
        goto EXIT_CCBMEM;
    }
    memset((void *)result,0, sizeof(struct ClienCB));
    result->sbuf.bufsync = semp_init(1,1,NULL);
    result->rbuf.bufsync = semp_init(1,0,NULL);
    if((NULL == result->sbuf.bufsync)||(NULL == result->rbuf.bufsync))
    {
        goto EXIT_CCBSEMP;
    }

    //net_malloc the send buffer  and init it
    result->sbuf.tcpsendbuf = net_malloc(CN_TCP_SNDBUF_SIZEDEFAULT);
    if(NULL == result->sbuf.tcpsendbuf)
    {
        goto EXIT_CCBSBUF;
    }
    result->sbuf.buflenleft =  CN_TCP_SNDBUF_SIZEDEFAULT;
    result->sbuf.buflenlimit = CN_TCP_SNDBUF_SIZEDEFAULT;
    result->sbuf.trigerlevel = 0;
    result->sbuf.timeout     = CN_TIMEOUT_FOREVER;
    result->sbuf.dataoff     = 0;
    result->sbuf.datalen     = 0;
    result->sbuf.unackoff    = 0;
    result->sbuf.unackno     = __computeIsn();
    result->sbuf.unacklen     = 0;
    result->sbuf.sndnxtoff   = 0;
    result->sbuf.sndnxtno    = result->sbuf.unackno;
    result->sbuf.dupacktims  = 0;
    result->sbuf.rtotick     = NetTickerTicks(pTcpTicker);

    //init the receive buffer
    result->rbuf.buflen       = 0;
    result->rbuf.buflenlimit  = CN_TCP_RCVBUF_SIZEDEFAULT;
    result->rbuf.timeout      = CN_TIMEOUT_FOREVER;
    result->rbuf.trigerlevel    = 0;
    result->rbuf.rcvnxt       = 0;
    result->rbuf.ph = NULL;
    result->rbuf.pt = NULL;

    result->pkgrecomblst = NULL;
    result->pkgrecomblen = 0;
    //set the local mss to default
    //the window initialize
    result->ssthresh    = 65535;
    result->sndwnd      = 0;
    result->sndwndscale = 0;
    result->mss         = CN_TCP_MSSDEFAULT;
    //this the send round trip initialize
    result->sa = CN_TCP_SA_INIT;
    result->sd = CN_TCP_SD_INIT;
    result->rto = result->sa + result->sd*2; //rto init:rto = A+2D
    result->cwnd = result->mss*10;

    //do the timer init
    result->timerctrl   = CN_TCP_TIMER_NONE;
    result->acktimes    = 0;
    result->keeptimer   = 0;
    result->persistimer = 0;
    result->mltimer     = 0;
    //init the stat
    result->channelstat =0;
    result->machinestat = EN_TCP_MC_CREAT;

    return result;

EXIT_CCBSBUF:
EXIT_CCBSEMP:
    semp_del(result->rbuf.bufsync);
    semp_del(result->sbuf.bufsync);
    freeccb(result);
    result = NULL;
EXIT_CCBMEM:
    return result;
}
//reset the ccb to the specified stat, such as the CREAT or 2FREE
static void  __ResetCCB(struct ClienCB *ccb,u16 machinestat)
{
    //init the ccb member
    //set the snd and receive buf limit to default
    semp_post(ccb->sbuf.bufsync);
    //reset the receive buf
    ccb->rbuf.buflen       = 0;
    ccb->rbuf.buflenlimit  = CN_TCP_RCVBUF_SIZEDEFAULT;
    ccb->rbuf.timeout      = CN_TIMEOUT_FOREVER;
    ccb->rbuf.trigerlevel    = 0;
    PkgTryFreeQ(ccb->rbuf.ph);
    ccb->rbuf.ph = NULL;
    ccb->rbuf.pt = NULL;
    semp_post(ccb->rbuf.bufsync);

    //reset the recomb queue
    PkgTryFreeQ(ccb->pkgrecomblst);
    ccb->pkgrecomblst = NULL;
    ccb->pkgrecomblen = 0;
    //init the stat
    ccb->channelstat =0;
    ccb->machinestat = machinestat;
    //do the timer init
    ccb->timerctrl   = CN_TCP_TIMER_NONE;
    ccb->acktimes    = 0;
    ccb->keeptimer   = 0;
    ccb->persistimer = 0;
    ccb->mltimer     = 0;
}
//use this function to net_free the ccb
static bool_t __DeleCCB(struct ClienCB *ccb)
{
    //net_free all the pkg to snd
    net_free(ccb->sbuf.tcpsendbuf);
    //net_free all the pkg to rcv
    PkgTryFreeQ(ccb->rbuf.ph);
    //net_free all the pkg to recomb
    PkgTryFreeQ(ccb->pkgrecomblst);
    //del the semp for the buf
    semp_del(ccb->rbuf.bufsync);
    semp_del(ccb->sbuf.bufsync);

    freeccb(ccb);
    return true;
}

//use this function to create a scb and init it
static struct ServerCB* __CreateScb(void)
{
    struct ServerCB *result;

    result = mallocscb();
    if(NULL == result)
    {
        goto SCB_MEM;
    }
    result->acceptsemp = semp_init(CN_TCP_ACCEPTMAX,0,NULL);
    if(NULL == result->acceptsemp)
    {
        goto SCB_SYNC;
    }
    result->backlog     =  CN_TCP_LISTENDEFAULT;
    result->clst        =  NULL;
    result->pendnum     =  0;
    result->accepttime  =  CN_TIMEOUT_FOREVER;
    return result;
SCB_SYNC:
    freescb(result);
    result = NULL;
SCB_MEM:
    return  result;
}
//use this function to delete an scb and net_free it
static bool_t __DeleteScb(struct ServerCB* scb)
{
    semp_del(scb->acceptsemp);
    freescb(scb);
    return true;
}
static bool_t __ReseSCB(struct ServerCB* scb)
{
    semp_post(scb->acceptsemp);
    scb->backlog =  CN_TCP_LISTENDEFAULT;
    scb->clst    =  NULL;
    scb->pendnum =  0;
    scb->accepttime  =  CN_TIMEOUT_FOREVER;
    return true;
}

// =============================================================================
// FUNCTION:this function used to make a tcp terminal
// PARA  IN:the parameters has the same meaning as socket
// PARA OUT：
// RETURN  :
// INSTRUCT:
// =============================================================================
static u16 gPortEngineTcp = 1024;//usually, the dynamic port is more than 1024
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static struct tagSocket * __tcpsocket(s32 family, s32 type, s32 protocal)
{
    struct tagSocket    *sock,*tmp;
    u16           findtime = 0;
    if(AF_INET == family)
    {
        sock = SocketBuild();
        if(NULL != sock)
        {
            sock->ProtocolOps = &gTcpProto;
            handle_SetMultiplexEvent(fd2Handle(sock->sockfd), 0);
//          memset(sock, 0, sizeof(struct tagSocket));
//          sock->SockSync = mutex_init(NULL);
            if(mutex_lock(pTcpHashTab->tabsync))
            {
                //ok, find any port could use
                do
                {
                    tmp = __hashSocketLocalSearch(INADDR_ANY,gPortEngineTcp);
                    if(NULL != tmp)
                    {
                        gPortEngineTcp ++;
                    }
                    findtime++;
                }while((tmp != NULL)&&(findtime < 65535));//we find until get the port,if over the failed
                //find over now, check if we go the port
                if(NULL == tmp)
                {
                    sock->af_inet = AF_INET;
                    sock->element.v4.iplocal = INADDR_ANY;
                    sock->element.v4.portlocal = gPortEngineTcp ++;
                    //net_malloc an cb for the socket
                    sock->TplCB = __CreateCCB();
                    if(NULL == sock->TplCB)
                    {
                        SocketFree(sock);
                        sock = NULL;
                    }
                    else
                    {
                        //initialize the sock add the sock to the hash tab
//                      sock->IoInitstat = 0;
                        sock->sockstat |=CN_SOCKET_OPEN|CN_SOCKET_CLIENT|CN_SOCKET_PROBLOCK|CN_SOCKET_PRONAGLE;
                        sock->ProtocolOps = &gTcpProto;
                        __hashSocketAdd(sock);
                    }
                }
                else
                {
                    //no port for you,you must do some kidding
                }
                mutex_unlock(pTcpHashTab->tabsync);
            }//end if for the lock pend
        }//end if NULL != sock
    }//end if AF_INET == family
    return sock;
}

#pragma GCC diagnostic pop

// =============================================================================
// FUNCTION:this function used to bind a address for the socket
// PARA  IN:the parameters has the same meaning as bind
// PARA OUT：
// RETURN  :0 success while -1 failed
// INSTRUCT:
// =============================================================================
static s32 __tcpbind(struct tagSocket *sock,struct sockaddr *addr, s32 addrlen)
{
    s32  result;
    u32 ip;
    u16 port;
    struct sockaddr_in *sockaddrin;
    struct tagSocket  *tmp;

    result = -1;
    if((NULL == addr)||(addrlen != sizeof(struct sockaddr))||\
       (addr->sa_family != AF_INET))
    {
        return result;
    }
    if(mutex_lock(pTcpHashTab->tabsync))
    {
        if(mutex_lock(sock->SockSync))
        {
            if((0 == (sock->sockstat&CN_SOCKET_BIND))&&\
               (0 == (CN_SOCKET_CLOSE&sock->sockstat)))  //NOT BIND YET
            {
                sockaddrin = (struct sockaddr_in *)addr;
                ip = sockaddrin->sin_addr.s_addr;
                port = sockaddrin->sin_port;

                enIPTYPE  type = EN_IPTYPE_V4_UNKOWN;
                tagRoutLink  rout;
                memset(&rout,0,sizeof(rout));
                rout.ver = EN_IPV_4;
                rout.DstIP = &ip;
                if(RouterMatch(&rout))
                {
                    type = rout.type;
                }
                if((type == EN_IPTYPE_V4_ANY)||(type == EN_IPTYPE_V4_HOST)||\
                   (type == EN_IPTYPE_V4_LOCAL)) //only these ip could be binded
                {
                    tmp = __hashSocketLocalSearch(ip,port);
                    if(NULL != tmp)
                    {
                        if(tmp == sock) //same socket, do noting
                        {
                            sock->sockstat |= CN_SOCKET_BIND;
                            result = 0;
                        }
                        else
                        {
                            //some one has the same ip and port ,so bind failed
                            result = -1;
                        }
                    }
                    else //no one matches, so reinsert the socket
                    {
                        __hashSocketRemove(sock);
                        sock->element.v4.iplocal = sockaddrin->sin_addr.s_addr;
                        sock->element.v4.portlocal = sockaddrin->sin_port;
                        sock->sockstat |= CN_SOCKET_BIND;
                        __hashSocketAdd(sock);
                        result = 0;
                    }
                }
            }
            mutex_unlock(sock->SockSync);
        }
        mutex_unlock(pTcpHashTab->tabsync);
    }
    return  result;
}
// =============================================================================
// FUNCTION:this function used to make the socket to listen
// PARA  IN:the parameters has the same meaning as listen
// PARA OUT：
// RETURN  :0 success while -1 failed
// INSTRUCT:this function to make the socket to become an server terminal
// =============================================================================
static s32 __tcplisten(struct tagSocket *sock, s32 backlog)
{
    s32  result;
    struct ServerCB  *scb;

    result = -1;
    if(mutex_lock(sock->SockSync))
    {
        if((CN_SOCKET_BIND &sock->sockstat)&&\
           (0 == (CN_SOCKET_CONNECT&sock->sockstat))&&\
           (0 == (CN_SOCKET_CLOSE&sock->sockstat))&&\
           (0==(CN_SOCKET_LISTEN&sock->sockstat)))
        {
            scb = __CreateScb();
            if(NULL != scb)
            {
                //may be we should cpy some options from the ClienCB,which set before listen
                __DeleCCB(sock->TplCB);
                scb->accepttime = ((struct ClienCB *)(sock->TplCB))->rbuf.timeout;
                sock->TplCB = scb;
                scb->backlog = backlog;
                sock->sockstat&=(~CN_SOCKET_CLIENT);
                sock->sockstat |=CN_SOCKET_LISTEN;

                result = 0;
            }
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}

//find an new client which is stable in the scb client queue
static struct tagSocket *__acceptclient(struct tagSocket *sock)
{
    struct tagSocket    *result;
    struct tagSocket    *client;
    struct tagSocket    *pre;
    struct ClienCB       *ccb;
    struct ServerCB  *scb;

    scb = (struct ServerCB *)sock->TplCB;
    result = NULL;
    client = scb->clst;
    pre = client;
    while(NULL != client)  //find the established and unopend socket
    {
        ccb = (struct ClienCB*)client->TplCB;
        if(EN_TCP_MC_ESTABLISH== ccb->machinestat)
        {
            ccb->channelstat|=CN_TCP_CHANNEL_STATARCV|CN_TCP_CHANNEL_STATASND;
            scb->pendnum--;
            //remove it from the scb client
            if(scb->clst == client)
            {
                scb->clst = client->Nextsock;
            }
            else
            {
                pre->Nextsock = client->Nextsock;
            }
            client->Nextsock = NULL;
            result = client;
            break;
        }
        else
        {
            pre = client;
            client = client->Nextsock;
        }
    }
    if(scb->clst == NULL)
       handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOACCEPT);
    return result;
}

// =============================================================================
// FUNCTION:this function used to accept client connection from the server
// PARA  IN:the parameters has the same meaning as accept
// PARA OUT:
// RETURN  :
// INSTRUCT:if any ternimal hung on the server is established, then will return
// =============================================================================
static struct tagSocket *__tcpaccept(struct tagSocket *sock, struct sockaddr *addr, s32 *addrlen)
{
    struct tagSocket           *result;
    struct ServerCB             *scb;
    struct sockaddr_in  *addrin;
    u32                  waittime;

    result = NULL;

    //first we will find if any client is ready
    if((0 ==(CN_SOCKET_CLIENT&sock->sockstat))&&\
       (sock->sockstat &CN_SOCKET_LISTEN)&&\
       mutex_lock(sock->SockSync))
    {
        scb = (struct ServerCB *)sock->TplCB;
        result = __acceptclient(sock);
        waittime = scb->accepttime;
        if((NULL == result)&&(0 != waittime))
        {
            //if none find and permit the wait
            mutex_unlock(sock->SockSync);
            if(semp_pendtimeout(scb->acceptsemp,waittime))
            {
                if(mutex_lock(sock->SockSync))
                {
                    result = __acceptclient(sock);
                }
            }
        }
        if(NULL== result)  //no one to accept
        {
//          handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOACCEPT);
        }
        else
        {
            result->sockstat |= CN_SOCKET_OPEN;
        }
        mutex_unlock(sock->SockSync);
    }
    if((NULL != result)&&(mutex_lock(pTcpHashTab->tabsync)))
    {
        __hashSocketAdd(result);
        mutex_unlock(pTcpHashTab->tabsync);
    }
    if((NULL != result)&&(NULL != addr) &&(NULL != addrlen))
    {
        //fill the address if possible
        addrin = (struct sockaddr_in *)addr;
        addrin->sin_family = AF_INET;
        addrin->sin_port = result->element.v4.portremote;
        addrin->sin_addr.s_addr = result->element.v4.ipremote;
        *addrlen = sizeof(struct sockaddr);
    }
    return result;
}

//use this function to send a packge or pakage list to the ip layer
static bool_t __sendmsg(struct tagSocket *sock, struct NetPkg *pkg,u16 translen)
{
    bool_t       result;
    struct TcpHdr    *hdr;
    struct ClienCB      *ccb;
    u32          iplocal, ipremote;

    iplocal = sock->element.v4.iplocal;
    ipremote = sock->element.v4.ipremote;
    hdr = (struct TcpHdr *)PkgGetCurrentBuffer(pkg);
//  hdr = (struct TcpHdr *)(pkg->buf + pkg->offset);
    hdr->chksum = 0;
    //if any ack timer set, we'd better to clear the ack timer
    ccb = (struct ClienCB *)sock->TplCB;
    result = IpSend(EN_IPV_4,(ptu32_t)iplocal, (ptu32_t)ipremote,pkg,translen,IPPROTO_TCP,\
                     CN_IPDEV_TCPOCHKSUM,&hdr->chksum);
    if(result)
    {
        ccb->acktimes = 0;
    }
    return result;
}

//use this function to make a tcp header
static struct NetPkg  *__buildhdr(struct tagSocket *sock, u8 flags,\
        void *option, u8 optionlen,u32 pkgflag,u32 sndno)
{
    struct NetPkg  *result;
    struct TcpHdr  *hdr;
    struct ClienCB     *ccb;
    u32         datalen;

    datalen = sizeof(struct TcpHdr)+((optionlen+3)/4)*4;
    result = PkgMalloc(datalen,pkgflag);
    if(NULL != result)
    {
        PkgSetDataLen(result, datalen);
//      result->datalen = datalen;
        ccb = (struct ClienCB*)sock->TplCB;
        hdr = (struct TcpHdr *)PkgGetCurrentBuffer(result);
//      hdr =(struct TcpHdr *)(result->offset + result->buf);
        hdr->portdst = sock->element.v4.portremote;
        hdr->portsrc = sock->element.v4.portlocal;
        hdr->ackno = htonl(ccb->rbuf.rcvnxt);
        hdr->seqno = htonl(sndno);
        hdr->hdroff = ((datalen/4)&0x0f)<<4;
        hdr->flags = flags;
        hdr->chksum = 0;
        if(ccb->rbuf.buflenlimit> ccb->rbuf.buflen)
        {
            hdr->window = htons(ccb->rbuf.buflenlimit- ccb->rbuf.buflen);
        }
        else
        {
            hdr->window = 0;
        }
        hdr->urgencyp = 0;
        memcpy((void *)hdr->opt,option,optionlen);
    }
    return result;
}
//build a tcp header and as your wish to send it
static bool_t __sendflag(struct tagSocket *sock, u8 flags, void *option, u8 optionlen,u32 sndno)
{
    bool_t              result;
    struct NetPkg          *pkg;

    result=false;
    //OK, construct the package for the connect
    pkg = __buildhdr(sock, flags, option, optionlen,CN_PKLGLST_END,sndno);
    if(NULL != pkg)
    {
        result = __sendmsg(sock,pkg,PkgGetDataLen(pkg));
//      result = __sendmsg(sock,pkg,pkg->datalen);
        PkgTryFreePart(pkg);
    }
    return  result;
}
// =============================================================================
// FUNCTION:the client use this function to connect the server, will do the handshake
// PARA  IN:sock, the client socket
//          addr, the server addr,
//          addrlen, the server addr len
// PARA OUT:
// RETURN  :0 success while -1 failed
// INSTRUCT:
// =============================================================================
//make sure this is the client, so we should do the handshaker with the server
//much more, we must adjust its handle in the hash tab
static s32 __tcpconnect(struct tagSocket *sock, struct sockaddr *serveraddr, s32 addrlen)
{
    s32  result;
    struct sockaddr_in *addrin;
    struct ClienCB             *ccb;
    result = -1;

    //fist we make sure to adjust the its place in the hash tab
    if(mutex_lock(pTcpHashTab->tabsync))
    {
        if((NULL != serveraddr)&&(addrlen == sizeof(struct sockaddr_in))&&\
            (CN_SOCKET_CLIENT&sock->sockstat))
        {
            addrin = (struct sockaddr_in *)serveraddr;
            __hashSocketRemove(sock);
            sock->element.v4.ipremote =addrin->sin_addr.s_addr;
            sock->element.v4.portremote = addrin->sin_port;
            sock->sockstat |= CN_SOCKET_BIND;
            __hashSocketAdd(sock);
            result = 0;
        }
        mutex_unlock(pTcpHashTab->tabsync);
    }
    if(0 == result)
    {
        result = -1;
        //now do the handshake with the server
        if(mutex_lock(sock->SockSync))
        {
            ccb = (struct ClienCB *)sock->TplCB;
            if((0 ==(CN_SOCKET_CONNECT &sock->sockstat))&&\
               (EN_TCP_MC_CREAT ==ccb->machinestat))  //it does not connect yet
            {
                __sendflag(sock,CN_TCP_FLAG_SYN,(void *)&sgSynOptDefault,\
                                        sizeof(sgSynOptDefault),ccb->sbuf.sndnxtno);
                //change the machine stat and open the timer flag
                ccb->resndtimer = ccb->rto;
                ccb->sbuf.sndnxtno++;
                ccb->machinestat = EN_TCP_MC_SYNSNT;
                ccb->timerctrl  = CN_TCP_TIMER_SYN;
                mutex_unlock(sock->SockSync); //release the mutex
                //wait the SockSync signal happens
                semp_pendtimeout(ccb->rbuf.bufsync,ccb->rbuf.timeout);
                //check the connection
                mutex_lock(sock->SockSync);
                if(ccb->machinestat == EN_TCP_MC_ESTABLISH)
                {
                    //the app could send and rceive data from now on
                    ccb->channelstat|=CN_TCP_CHANNEL_STATASND|CN_TCP_CHANNEL_STATARCV;
                    result = 0; //connection success
                }
                else
                {
                    __ResetCCB(ccb, EN_TCP_MC_2FREE); //THE NEXT STEP NEED BE CLOSE
                }
            }
            mutex_unlock(sock->SockSync);
        }
    }
    return  result;
}
//we use this function to copy the data to the buffer,the len has been specified by the
//buflenleft,we write from the data off, may be round over
static void __cpy2sndbuf(struct tagSocket *sock, const void *msg, s32 len)
{
    struct ClienCB           *ccb;
    u8               *src;
    u8               *dst;
    s32               cpylen;

    ccb = (struct ClienCB *)sock->TplCB;

    cpylen = ccb->sbuf.buflenlimit-ccb->sbuf.dataoff;
    if(cpylen >= len)
    {
        cpylen = len;
        len = 0;
    }
    else
    {
        len -= cpylen;
    }
    dst = (u8 *)(ccb->sbuf.tcpsendbuf + ccb->sbuf.dataoff);
    src = (u8 *)msg;
    memcpy(dst,src,cpylen);
    src += cpylen;
    ccb->sbuf.dataoff += cpylen;
    if(ccb->sbuf.dataoff == ccb->sbuf.buflenlimit) //actually,we could be same at most
    {
        ccb->sbuf.dataoff = 0;
    }
    //update the member
    ccb->sbuf.buflenleft -= cpylen;
    ccb->sbuf.datalen    += cpylen;

    if(len > 0)
    {
        cpylen = len;
        dst = (u8 *)(ccb->sbuf.tcpsendbuf + ccb->sbuf.dataoff);
        memcpy(dst,src,cpylen);
        ccb->sbuf.dataoff += cpylen;
        if(ccb->sbuf.dataoff >= ccb->sbuf.buflenlimit)
        {
            ccb->sbuf.dataoff = 0;
        }
        //update the member
        ccb->sbuf.buflenleft -= cpylen;
        ccb->sbuf.datalen    += cpylen;
    }
}
//check how many data could send in the current state
//consider how many data in the buffer and the channel could send most
//the result is the little one during them
static s32 __chkchannelsendlen(struct tagSocket *sock)
{
    s32      result;
    s32      datalen;
    struct ClienCB  *ccb;

    ccb =(struct ClienCB *)sock->TplCB;
    if(0 ==(ccb->channelstat & CN_TCP_CHANNEL_STATKSND))
    {
        return 0;
    }

    datalen = ccb->sbuf.datalen;
    if(CN_SOCKET_PRONAGLE&sock->sockstat) //nagle is open
    {
        //this means nothing to ack now or the new data is more than the mss,
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)
        {
            //this means nothing to acknowledge, so you could send as much data as possible
            result = datalen;
        }
        else if(datalen > ccb->mss)
        {
            result = datalen;
        }
        else
        {
            result = 0;
        }
    }
    else //nagle is close
    {
        //we will send the mixnum of (sndwnd cwnd datalen)
        result = ccb->cwnd;
        if(result > ccb->sndwnd)
        {
            result = ccb->sndwnd;
        }
        if(result > datalen)
        {
            result = datalen;
        }
    }
    return result;
}

//use this function to send the new data or the data with the fin flag
static void __senddata(struct tagSocket *sock,s32 length)
{
    u16                 datalen;
    struct NetPkg          *pkghdr;
    struct NetPkg          *pkgdata;
    struct ClienCB             *ccb;
    u8                  flags;
    u8                 *data;
    bool_t              sendresult;
    u32                 translen;

    ccb = (struct ClienCB *)sock->TplCB;
    while(length > 0) //we send all the data specified by length or until send failed
    {
        //compute the data datalen flags first
        flags = CN_TCP_FLAG_ACK;
        if((length <= ccb->mss)) //we could send it at one time
        {
            flags |= CN_TCP_FLAG_PSH;
            if(0 == (ccb->channelstat & CN_TCP_CHANNEL_STATASND))
            {
                flags |= CN_TCP_FLAG_FIN;   //we would not send any more
            }
            datalen = length;
        }
        else
        {
            datalen = ccb->mss;
        }
        //make sure the data not round trip
        if((ccb->sbuf.sndnxtoff + datalen )>ccb->sbuf.buflenlimit)
        {
            //this means the data round up ,we just send the part
            datalen = ccb->sbuf.buflenlimit - ccb->sbuf.sndnxtoff;
            flags &=(~CN_TCP_FLAG_FIN);
        }
        else
        {
            //no round trip,just do it
        }
        data = ccb->sbuf.tcpsendbuf + ccb->sbuf.sndnxtoff;
        pkghdr = __buildhdr(sock,flags,NULL,0,0,ccb->sbuf.sndnxtno);
        if(NULL != pkghdr)
        {
            pkgdata = PkgMalloc(0,CN_PKLGLST_END);
//          pkgdata = PkgMalloc(sizeof(struct NetPkg),CN_PKLGLST_END);
            if(NULL != pkgdata)
            {
                //we got the data pkgnow, make sure how many data we will send
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //只有一个包
//              pkgdata->buf = data;
//              pkgdata->offset = 0;
//              pkgdata->datalen = datalen;
//              pkgdata->partnext = NULL;
                PkgSetNextUnit(pkghdr,pkgdata);
//              pkghdr->partnext = pkgdata;
                translen = PkgGetDataLen(pkghdr)+datalen;
//              translen = datalen + pkghdr->datalen;
                sendresult = __sendmsg(sock,pkghdr,translen);
                //net_free the package
                PkgTryFreePart(pkghdr);
                PkgTryFreePart(pkgdata);
                //update the members
                if(sendresult)
                {
                    ccb->sbuf.sndnxtno += datalen;
                    ccb->sbuf.sndnxtoff += datalen;
                    if(ccb->sbuf.sndnxtoff == ccb->sbuf.buflenlimit)
                    {
                        ccb->sbuf.sndnxtoff = 0; //round
                    }
                    ccb->sbuf.unacklen += datalen;
                    ccb->sbuf.datalen -= datalen;
                    if(ccb->sndwnd > datalen)
                    {
                        ccb->sndwnd -= datalen;
                    }
                    else
                    {
                        ccb->sndwnd = 0;
                    }
                    length -= datalen;
                    if(flags & CN_TCP_FLAG_FIN) //IF FIN SEND ,THEN STAT CHANGE,CLOSE THE KERNEL SEND
                    {
                        ccb->sbuf.sndnxtno++;
                        if(EN_TCP_MC_CLOSEWAIT == ccb->machinestat)
                        {
                            ccb->machinestat = EN_TCP_MC_LASTACK;
                        }
                        else
                        {
                            ccb->machinestat = EN_TCP_MC_FINWAIT1;
                        }
                        ccb->channelstat &= (~CN_TCP_CHANNEL_STATKSND);
                        ccb->timerctrl |= CN_TCP_TIMER_FIN;  //open the fin timer
                        //no send anymore and break
                        break;
                    }
                }
                else
                {
                    ccb->timerctrl |= CN_TCP_TIMER_PERSIST;
                    ccb->persistimer = CN_TCP_TICK_PERSIST;
                    break;  //send failed,open the persist timer
                }
            }
            else
            {
                PkgTryFreePart(pkghdr);  //no pkg memory for the tcpdata
                break;
            }
        }
        else
        {
            break;  //no pkg memory for the tcp header
        }
        ccb->sbuf.rtotick = NetTickerTicks(pTcpTicker);
    }
    if(ccb->sbuf.unacklen > 0)
    {
        ccb->resndtimer = ccb->rto;
        ccb->timerctrl |= CN_TCP_TIMER_RESEND;   //OPEN THE RESEND TIMER
    }
    if(((ccb->sbuf.datalen > 0)&&(ccb->sndwnd < ccb->mss)))
    {
        ccb->timerctrl |= CN_TCP_TIMER_PERSIST;
        ccb->persistimer = CN_TCP_TICK_PERSIST;
    }
    else
    {
        ccb->persistimer &= (~CN_TCP_TICK_PERSIST);
        ccb->persistimer = 0;
    }
    return;
}
//use this function to resend the data,we just resend the unacknowledge data
static void __resenddata(struct tagSocket *sock)
{
    u16                 datalen;
    struct NetPkg          *pkghdr;
    struct NetPkg          *pkgdata;
    struct ClienCB             *ccb;
    u8                  flags;
    u8                 *data;
    u32                 translen;

    ccb = (struct ClienCB *)sock->TplCB;
    //first we want to make sure if any data or how many data to resend
    //we could only send a mss at most
    datalen = ccb->sbuf.unacklen;
    if((datalen + ccb->sbuf.unackoff)>ccb->sbuf.buflenlimit)
    {
        //the unack is round up, so just send part
        datalen = ccb->sbuf.buflenlimit - ccb->sbuf.unackoff;
    }
    if(datalen > ccb->mss)
    {
        //sorry to tell you that we could send a mss at most
        datalen = ccb->mss;
    }
    if(datalen > 0)
    {
        //compute the data datalen flags first
        flags = CN_TCP_FLAG_ACK|CN_TCP_FLAG_PSH;
        data = ccb->sbuf.tcpsendbuf + ccb->sbuf.unackoff;
        pkghdr = __buildhdr(sock,flags,NULL,0,0,ccb->sbuf.unackno);
        if(NULL != pkghdr)
        {
            pkgdata = PkgMalloc(0,CN_PKLGLST_END);
//          pkgdata = PkgMalloc(sizeof(struct NetPkg),CN_PKLGLST_END);
            if(NULL != pkgdata)
            {
                //we got the data pkgnow, make sure how many data we will send
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //只有一个包
//              pkgdata->buf = data;
//              pkgdata->offset = 0;
//              pkgdata->datalen = datalen;
//              pkgdata->partnext = NULL;
                PkgSetNextUnit(pkghdr,pkgdata);
//              pkghdr->partnext = pkgdata;
                translen = PkgGetDataLen(pkghdr)+datalen;
//              translen = datalen + pkghdr->datalen;
                __sendmsg(sock,pkghdr,translen);
                //net_free the package
                PkgTryFreePart(pkghdr);
                PkgTryFreePart(pkgdata);
            }
            else
            {
                PkgTryFreePart(pkghdr);  //no pkg memory for the tcpdata
            }
        }
    }
    return;
}
// =============================================================================
// FUNCTION:this function used to send data to the socket
// PARA  IN:sock, the socket to send
//          msg, the message to send
//          len, the message length
//          flags,not used yet
// PARA OUT:
// RETURN  :
// INSTRUCT:-1 failed while others return the send length really
// =============================================================================
//1,first we must got the SockSync,
//2,then got the mutex to copy data
static s32 __tcpsend(struct tagSocket *sock, const void *msg, s32 len, s32 flags)
{
    s32        result;
    s32        sndlen;
    struct ClienCB    *ccb;

    result = -1;
    if(0 == (CN_SOCKET_CLIENT &sock->sockstat))
    {
        return result;
    }
    ccb = (struct ClienCB *)sock->TplCB;
    if(semp_pendtimeout(ccb->sbuf.bufsync,ccb->sbuf.timeout))
    {
        //got the mutex
        if(mutex_lock(sock->SockSync))
        {
            //check if we could copy it to the buffer
            if(CN_TCP_CHANNEL_STATASND&ccb->channelstat)
            {
                sndlen = ccb->sbuf.buflenleft>len?len:ccb->sbuf.buflenleft;
                if(sndlen > 0)
                {
                    //enough,we could cpy the buf to the sbuf;
                   __cpy2sndbuf(sock,msg,sndlen);
                   result = sndlen;
                   //check if we could send some data out
                   sndlen = __chkchannelsendlen(sock);
                   if(sndlen > 0)
                   {
                       __senddata(sock,sndlen);
                   }
                   //if the fin or send is not normal, then will not clean the trigle
                   //which means that the channel is shutdown and you could got -1 result
                   if((ccb->sbuf.buflenleft > ccb->sbuf.trigerlevel)||
                      (0 ==(ccb->channelstat&CN_TCP_CHANNEL_STATASND)))
                   {
                       handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
                       semp_post(ccb->sbuf.bufsync);
                   }
                   else
                   {
                       handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
                   }
                }
                else
                {
                    if(len == 0)
                    {
                        result = len;   //0 bytes copied
                    }
                    else
                    {
                        result = -1;    //unkonw reason
                    }
                }
            }
            else
            {
                result = 0;  // the channel is shutdown now
                handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
                semp_post(ccb->sbuf.bufsync);
            }
            mutex_unlock(sock->SockSync);
        }
    }
    else
    {
        //has not got the SockSync yet. maybe time out
    }
    return result;
}

//use this function to copy data from the receive buf
static s32 __cpyfromrcvbuf(struct ClienCB *ccb, void *buf, s32 len)
{
    s32  result;
    s32 cpylen;
    u8   *cpybuf;
    u8   *srcbuf;
    struct NetPkg  *pkg;

    srcbuf = (u8 *)buf;
    result = 0;

    pkg = ccb->rbuf.ph;
    while((len >0) && (NULL != pkg))
    {
        cpylen = len > PkgGetDataLen(pkg)?PkgGetDataLen(pkg):len;
//      cpylen = len > pkg->datalen?pkg->datalen:len;
        cpybuf = (u8 *)PkgGetCurrentBuffer(pkg);
//      cpybuf = (u8 *)(pkg->buf + pkg->offset);
        memcpy(srcbuf, cpybuf,cpylen);
        PkgMoveOffsetUp(pkg,cpylen);
//      pkg->offset += cpylen;
//      pkg->datalen -= cpylen;
        ccb->rbuf.buflen -= cpylen;
        result +=cpylen;
        len-= cpylen;
        srcbuf+=cpylen;
        if(0==PkgGetDataLen(pkg)) //release the no data pkg
//      if(0==pkg->datalen) //release the no data pkg
        {
            ccb->rbuf.ph = PkgGetNextUnit(pkg);
            if(NULL == ccb->rbuf.ph)
            {
                ccb->rbuf.pt = NULL;
            }
            PkgTryFreePart(pkg);
            pkg = ccb->rbuf.ph;
        }
    }

    return result;
}
// =============================================================================
// FUNCTION:this function used to receive data from the socket
// PARA  IN:the parameters has the same meaning as recv
// PARA OUT:
// RETURN  :
// INSTRUCT:0 means the socket is closed or fin receive -1 means no data,others means
//          the data length copy from the socket
// =============================================================================
static s32 __tcprecv(struct tagSocket *sock, void *buf,s32 len, u32 flags)
{
    s32        result;
    struct ClienCB    *ccb;

    result = -1;
    if(0 == (CN_SOCKET_CLIENT &sock->sockstat))
    {
        return result;
    }
    ccb = (struct ClienCB *)sock->TplCB;
    if(semp_pendtimeout(ccb->rbuf.bufsync,ccb->rbuf.timeout))
    {
        //got the mutex
        if(mutex_lock(sock->SockSync))
        {
            if(CN_TCP_CHANNEL_STATARCV & ccb->channelstat)
            {
                //maybe there are some data
                result = __cpyfromrcvbuf(ccb,buf,len);   //the buf already has data
                if(result == 0)
                {
                    //no data yet
                    if(0 == (ccb->channelstat&CN_TCP_CHANNEL_STATKRCV))
                    {
                        //the remote is shutdown,so close the areceive
                        ccb->channelstat &= (~CN_TCP_CHANNEL_STATARCV);
                    }
                    else
                    {
                        result = -1;  //not shutdown yet
                    }
                }

                if((ccb->rbuf.buflen > ccb->rbuf.trigerlevel)||
                   (0 == (ccb->channelstat&CN_TCP_CHANNEL_STATKRCV)))
                {
                    handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    semp_post(ccb->rbuf.bufsync);
                }
                else
                {
                    handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    ccb->channelstat &=(~CN_TCP_CHANNEL_STATCONGEST);
                    //changed the rcv window
                    //we may prevent the data in because of the limited buf, then we
                    //if we has some space, we'd better to notice the remote terminal
                    //if the data is zero, we could snd the window
                    __sendflag(sock,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
                }
            }
            else
            {
                result = 0;  // the channel receive is shutdown now
                handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                semp_post(ccb->rbuf.bufsync);
            }

            mutex_unlock(sock->SockSync);
        }
    }
    else
    {
        //not got the SockSync yet
    }
    return  result;
}

// =============================================================================
// FUNCTION:this function used to shutdown the socket receive channel
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed
// =============================================================================
static s32 __shutdownRD(struct tagSocket *sock)
{
    s32 result;
    struct ClienCB *ccb;

    result = -1;
    ccb = (struct ClienCB *)sock->TplCB;

    //clear the rcv buf and recombination queue
    //and set some specified flags at the same time
    ccb->channelstat&=(~(CN_TCP_CHANNEL_STATARCV|CN_TCP_CHANNEL_STATKRCV));
    //release the rcv and recombination queue
    PkgTryFreeQ(ccb->rbuf.ph);
    ccb->rbuf.buflen = 0;
    ccb->rbuf.buflenlimit  = CN_TCP_RCVBUF_SIZEDEFAULT;
    ccb->rbuf.ph = NULL;
    ccb->rbuf.pt = NULL;
    ccb->rbuf.trigerlevel = 0;
    semp_post(ccb->rbuf.bufsync);
    //net_free all the recomblst
    PkgTryFreeQ(ccb->pkgrecomblst);
    ccb->pkgrecomblst = NULL;
    ccb->pkgrecomblen = 0;
    result = 0;

    return result;
}
// =============================================================================
// FUNCTION:this function used to shutdown the socket send channel
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed
// =============================================================================
static s32 __shutdownWR(struct tagSocket *sock)
{
    s32 result;
    struct ClienCB     *ccb;

    result = -1;
    if(sock->sockstat & CN_SOCKET_CLIENT)
    {
        ccb = (struct ClienCB *)sock->TplCB;
        ccb->channelstat&=(~(CN_TCP_CHANNEL_STATASND));  //APP NO PERMIT TO TRANSMISSION
        //if send buf still has some data to snd, then set the flag is OK
        if(ccb->sbuf.datalen == 0)
        {
            ccb->channelstat&=(~(CN_TCP_CHANNEL_STATKSND));
            __sendflag(sock,CN_TCP_FLAG_FIN|CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
            ccb->sbuf.sndnxtno++;
            ccb->resndtimer = ccb->rto;
            ccb->timerctrl |= CN_TCP_TIMER_FIN;
            if(EN_TCP_MC_CLOSEWAIT == ccb->machinestat)
            {
                ccb->machinestat = EN_TCP_MC_LASTACK;
            }
            else
            {
                ccb->machinestat  = EN_TCP_MC_FINWAIT1;
            }
            result = 0;
        }
        else
        {
            //send the fin with the data, this will do in __senddata
        }
    }
    return result;
}
// =============================================================================
// FUNCTION:this function used to shutdown the socket  channel
// PARA  IN:sock, the socket to shutdown
//          how,SHUT_RD means receive channel,SHUT_WR means send channel,SHUT_RDWR means all channle
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed
// =============================================================================
static s32 __tcpshutdown(struct tagSocket *sock, u32 how)
{
    s32    result;

    if((CN_SOCKET_CLIENT&sock->sockstat)&&\
        mutex_lock(sock->SockSync))
    {
        switch(how)
        {
            case SHUT_RD:
                result = __shutdownRD(sock);
                break;
            case SHUT_WR:
                result = __shutdownWR(sock);
                break;
            case SHUT_RDWR:
                result = __shutdownRD(sock);
                if(0 ==result)
                {
                    result = __shutdownWR(sock);
                }
                break;
            default:
                result = -1;
                break;
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}
// =============================================================================
// FUNCTION:this function used to close the socket  channel
// PARA  IN:sock, the socket to close
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed;
//          1,we will not delete it here but mark some flags,we will delete it in
//            the tick processor(scan the sock with CLOSE and ccb stat is 2free)
//          2,if the linger is closed, then send rst;if linger is open and there
//             is still some data in the send buffer,then send the rst, otherwise
//            do the close handshake
// =============================================================================
static s32 __closesocket(struct tagSocket *sock)
{
    s32       result;
    struct ClienCB   *ccb;
    struct ServerCB   *scb;
    struct tagSocket *client;
    struct tagSocket *server;
    result = -1;

    if(mutex_lock(pTcpHashTab->tabsync))
    {
        if(mutex_lock(sock->SockSync))
        {
            if(CN_SOCKET_CLIENT&sock->sockstat)  //sock client
            {
                client = sock;
                client->sockstat |= CN_SOCKET_CLOSE;
                ccb = (struct ClienCB *)client->TplCB;
                if(ccb->machinestat == EN_TCP_MC_CREAT)
                {
                    __ResetCCB(ccb, EN_TCP_MC_2FREE);
                    result = true;
                }
                else
                {
                    __shutdownRD(client);
                    result = __shutdownWR(client);
                }
                mutex_unlock(client->SockSync);
            }
            else   //this is the server,so just close it and close all the client hung on it
            {
                server = sock;
                server->sockstat |= CN_SOCKET_CLOSE;
                //shutdown each socket hunge on the server(not accept yet)
                scb = (struct ServerCB *)server->TplCB;
                while(NULL != scb->clst)
                {
                    client = scb->clst;
                    scb->clst = client->Nextsock;

                    ccb = (struct ClienCB *)client->TplCB;
                    __ResetCCB(ccb,EN_TCP_MC_2FREE);
                    __DeleCCB(ccb);
                    SocketFree(client);
                }
                __ReseSCB(scb);
                __DeleteScb(scb);
                __hashSocketRemove(server);
                SocketFree(server);
            }
            result = 0;
        }

        mutex_unlock(pTcpHashTab->tabsync);
    }

    return result;
}

//this function deal with SOL_SOCK
static s32 __setsockopt_sol(struct tagSocket *sock,s32 optname,const void *optval, s32 optlen)
{
    bool_t result;
    struct ClienCB *ccb;
    struct ServerCB *scb;
    struct linger      *mylinger;
    u8 *buf;
    s32 buflen;

    result = -1;
    switch(optname)
    {
        case SO_BROADCAST:
            break;
        case SO_DEBUG:
            break;
        case SO_DONTROUTE:
            break;
        case SO_ERROR:
            break;
        case SO_KEEPALIVE:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                if(*(s32 *)optval)
                {
                    ccb->timerctrl |= CN_TCP_TIMER_KEEPALIVE;
                    ccb->keeptimer = CN_TCP_TICK_KEEPALIVE;
                }
                else
                {
                    ccb->timerctrl &= (~CN_TCP_TIMER_KEEPALIVE);
                }
                result = 0;
            }
            break;
        case SO_LINGER:
            if((CN_SOCKET_CLIENT&sock->sockstat)&&\
                    (optlen == sizeof(struct linger)))
            {
                ccb = (struct ClienCB *)sock->TplCB;
                mylinger  = (struct linger *)optval;
                if(mylinger->l_onff)
                {
                    ccb->timerctrl |= CN_TCP_TIMER_LINGER;
                    ccb->lingertimer = mylinger->l_linger*1000/CN_TCP_TICK_TIME;
                }
                else
                {
                    ccb->timerctrl &= (~CN_TCP_TIMER_LINGER);
                }
                result = 0;
            }
            break;
        case SO_OOBINLINE:
            break;
        case SO_RCVBUF:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                s32 buflen;
                buflen = *(s32 *)optval;
                if(buflen >0)
                {
                    ccb->rbuf.buflenlimit = buflen;
                    ccb->rbuf.trigerlevel = 0;
                    result = 0;
                }
            }
            break;
        case SO_SNDBUF:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                buflen = *(s32 *)optval;
                if((buflen >0)&&((buf = net_malloc(buflen))!= NULL))
                {
                    ccb->sbuf.buflenleft =  buflen;
                    ccb->sbuf.buflenlimit = buflen;
                    ccb->sbuf.trigerlevel = 0;
                    ccb->sbuf.timeout     = CN_TIMEOUT_FOREVER;
                    ccb->sbuf.dataoff     = 0;
                    ccb->sbuf.datalen     = 0;
                    ccb->sbuf.unackoff    = 0;
                    ccb->sbuf.unacklen    = 0;
                    ccb->sbuf.sndnxtoff   = 0;
                    ccb->sbuf.dupacktims  = 0;
                    if(NULL != ccb->sbuf.tcpsendbuf)
                    {
                        net_free((void *)ccb->sbuf.tcpsendbuf);
                    }
                    ccb->sbuf.tcpsendbuf = buf;
                    result = 0;
                }
            }
            break;
        case SO_RCVLOWAT:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                if(*(s32 *)optval>=0)
                {
                    ccb->rbuf.trigerlevel = *(s32 *)optval;
                    result = 0;
                }
            }
            break;
        case SO_SNDLOWAT:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                if(*(s32 *)optval>=0)
                {
                    ccb->sbuf.trigerlevel = *(s32 *)optval;
                    result = 0;
                }
            }
            break;
        case SO_RCVTIMEO:       // *optval ==0等效于非阻塞模式接收
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                if(*(s32 *)optval>=0)
                {
                    ccb->rbuf.timeout = *(s32 *)optval;
                    result = 0;
                }
            }
            else
            {
                scb = (struct ServerCB *)sock->TplCB;
                if(*(s32 *)optval>=0)
                {
                    scb->accepttime = *(s32 *)optval;
                    result = 0;
                }
            }
            break;
        case SO_SNDTIMEO:       // *optval ==0等效于非阻塞模式发送
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                if(*(s32 *)optval>=0)
                {
                    ccb->sbuf.timeout = *(s32 *)optval;
                    result = 0;
                }
            }
            break;
        case SO_REUSEADDR:
            result = 0;
            break;
        case SO_TYPE:
            result = 0;
            break;
        case SO_BSDCOMPAT:
            result = 0;
            break;
        case SO_NONBLOCK:
            //*optval == 0表示设为阻塞模式，!=0表示设为非阻塞模式
            if(*(s32 *)optval)
            {
                sock->sockstat &= (~CN_SOCKET_PROBLOCK);
                if(CN_SOCKET_CLIENT&sock->sockstat)
                {
                    ccb = (struct ClienCB *)sock->TplCB;
                    ccb->rbuf.timeout = 0;
                    ccb->sbuf.timeout = 0;
                }
                else
                {
                    scb = (struct ServerCB *)sock->TplCB;
                    scb->accepttime = 0;
                }
            }
            else
            {
                sock->sockstat |=  CN_SOCKET_PROBLOCK;
                if(CN_SOCKET_CLIENT&sock->sockstat)
                {
                    ccb = (struct ClienCB *)sock->TplCB;
                    ccb->rbuf.timeout = CN_TIMEOUT_FOREVER;
                    ccb->sbuf.timeout = CN_TIMEOUT_FOREVER;
                }
                else
                {
                    scb = (struct ServerCB *)sock->TplCB;
                    scb->accepttime = CN_TIMEOUT_FOREVER;
                }
            }
            result = 0;
            break;
        default:
            result = -1;
            break;
    }
    return result;
}
//this function deal with the IPPROTO_IP
static s32 __setsockopt_ip(struct tagSocket *sock,s32 optname,const void *optval, s32 optlen)
{
    bool_t result;

    result = -1;
    switch(optname)
    {
        case IP_HDRINCL:
            break;
        case IP_OPTIONS:
            break;
        case IP_TOS:
            break;
        case IP_TTL:
            break;
        default:
            break;
    }

    return result;
}
//this function deal with ipproto_tcp
static s32 __setsockopt_tcp(struct tagSocket *sock,s32 optname,const void *optval, s32 optlen)
{
    bool_t result;
    struct ClienCB *ccb;

    result = -1;
    switch(optname)
    {
        case TCP_MAXSEG:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClienCB *)sock->TplCB;
                ccb->mss = *(u8 *)optval;
                result = 0;
            }
            break;
        case TCP_NODELAY:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                if(*(s32 *)optval)
                {
                    sock->sockstat &=(~CN_SOCKET_PRONAGLE);
                }
                else
                {
                    sock->sockstat |= CN_SOCKET_PRONAGLE;
                }
                result = 0;
            }
            break;
        default:
            break;
    }

    return result;
}
// =============================================================================
// FUNCTION:this function used to the socket option
// PARA  IN:sock, the socket to set option
//          level,such as IPPROTO_TCP
//          optname, level details, such as SO_KEEPALIVE and so on
//          optvalue, the value of the optname;
//          optlen,the length of optvalue
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed
// =============================================================================
static s32 __tcpsetsockopt(struct tagSocket *sock, s32 level, s32 optname,\
               const void *optval, s32 optlen)
{
    s32  result;

    if(mutex_lock(sock->SockSync))
    {
        switch(level)
        {
            case SOL_SOCKET:
                result = __setsockopt_sol(sock,optname,optval,optlen);
                break;
            case IPPROTO_IP:
                result = __setsockopt_ip(sock,optname,optval,optlen);
                break;
            case IPPROTO_TCP:
                result = __setsockopt_tcp(sock,optname,optval,optlen);
                break;
            default:
                result = -1;
                break;
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}
// =============================================================================
// FUNCTION:this function used to get the socket option, not implement yet
// PARA  IN:sock, the socket to set option
//          level,such as IPPROTO_TCP
//          optname, level details, such as SO_KEEPALIVE and so on
//          optvalue, the value of the optname;
//          optlen,the length of optvalue
// PARA OUT:
// RETURN  :
// INSTRUCT:0 success while -1 failed
// =============================================================================
static s32 __tcpgetsockopt(struct tagSocket *sock, s32 level, s32 optname, void *optval,\
               s32 *optlen)
{
    s32  result = -1;


    return  result;
}
////////////////////////MACHINE STAT DEAL///////////////////////////////////////
//do the tcp checksum if the net dev not do it
static bool_t  __rcvchecksum(u32 iplocal,u32 ipremote, struct NetPkg *pkglst)
{
    bool_t  result;
    u16     chksum;
    u16     translen;
    struct NetPkg *pkg;
//  translen = 0;
    pkg = pkglst;
    translen = PkgListDatastatistics(pkg);
//  while(pkg != NULL)
//  {
//      translen += pkg->datalen;
//      pkg = PkgGetNextUnit(pkg);
//  }
    IpPseudoPkgLstChkSumV4(iplocal, ipremote, IPPROTO_TCP,\
                         pkglst,translen,&chksum);
    if(0 == chksum)
        result =true;
    else
        result = false;
    return result;
}
//if could not find or create a socket for the remote, then just reset it
static bool_t __resetremoteraw(u32 iplocal, u16 portlocal,u32 ipremote, u16 portremote,u32 ackno,u32 seqno)
{
    bool_t result ;
    struct NetPkg *pkg;
    struct TcpHdr   *hdr;
    u16 translen;

    result = false;
    pkg = PkgMalloc(sizeof(struct TcpHdr),CN_PKLGLST_END);
    if(NULL != pkg)
    {
        hdr = (struct TcpHdr *)PkgGetCurrentBuffer(pkg);
//      hdr = (struct TcpHdr *)(pkg->offset + pkg->buf);
        hdr->chksum = 0;
        hdr->flags = CN_TCP_FLAG_RST|CN_TCP_FLAG_ACK;
        hdr->hdroff = 0x05<<4;
        hdr->portdst = portremote;
        hdr->portsrc = portlocal;
        hdr->ackno  = htonl(ackno);
        hdr->seqno = htonl(seqno);
        hdr->urgencyp = 0;
        PkgSetDataLen(pkg,sizeof(struct TcpHdr));
//      pkg->datalen = sizeof(struct TcpHdr);
        translen = PkgGetDataLen(pkg);
//      translen = pkg->datalen;

        result = IpSend(EN_IPV_4,(ptu32_t)iplocal, (ptu32_t)ipremote, pkg,translen,IPPROTO_TCP,\
                 CN_IPDEV_TCPOCHKSUM,&hdr->chksum);
        PkgTryFreePart(pkg);
    }
    return result;
}

enum __EN_TCP_OPT_CODE
{
    EN_TCP_OPT_END = 0,
    EN_TCP_OPT_NONE,
    EN_TCP_OPT_MSS ,
    EN_TCP_OPT_SCALE,
    EN_TCP_OPT_SACKP,
    EN_TCP_OPT_SACKR,
    EN_TCP_OPT_TAMP = 8,
};

//do the remote optional
static void dealtcpoption(struct ClienCB *ccb, struct TcpHdr *hdr)
{
    u8  totallen;
    u8  optlen;
    u8  kind;
    u8  data8;
    u16 data16;
    u8  *src;
    u8  *optsrc;

    totallen = ((hdr->hdroff>>4) -0x05)*4;
    src = (u8 *)hdr->opt;
    while(totallen > 0)
    {
        optsrc = src;
        kind = *optsrc++;
        switch(kind)
        {
            case EN_TCP_OPT_END:      //OPTION END
                totallen = 0;
                break;
            case EN_TCP_OPT_NONE:     //OPTION NONE
                totallen -= 1;
                src += 1;
                break;
            case EN_TCP_OPT_MSS:      //SET THE REMOTE MSS
                optlen = *optsrc++;
                data16 = *(u16 *)(optsrc);
                data16 = htons(data16);
                ccb->mss = ccb->mss > data16?data16:ccb->mss;
                totallen -= optlen;
                src += optlen;
                break;
            case EN_TCP_OPT_SCALE:      //SET THE REMOTE WND SCALE
                optlen = *optsrc++;
                data8 = *(u8 *)(optsrc);
                ccb->sndwndscale = data8;
                ccb->sndwnd = ccb->sndwnd<<ccb->sndwndscale;
                totallen -= optlen;
                src += optlen;
                break;
            case EN_TCP_OPT_TAMP:
                optlen = *optsrc++;
                totallen -= optlen;
                src += optlen;
                break;
            default:
                totallen = 0;
                break;
        }
    }
    return ;
}

//we use this function to deal with the data accepted, which means that if the krcv
//bit of the ccb is set then could use this function to deal with the data received
//check the krcv bit and the pkg.data len > 0
//which means how many data received

//------------------------------------------------------------------------------
//功能：接收数据，并把数据推送到上一层
//参数：client，接收数据的socket
//     seqno，数据序号
//     pkg，数据包
//     proto，传输协议接口函数集
//返回：true or false
//------------------------------------------------------------------------------
static u32 __rcvdata(struct tagSocket *client, u32 seqno,struct NetPkg *pkg)
{
    struct ClienCB   *ccb;
    u32        pkgstart;
    u32        pkgstop;
    u32        rcvlen;
    u32        pkgdataoff;
    struct NetPkg *pkgcomb;

    rcvlen = 0;
    ccb = (struct ClienCB *)client->TplCB;
    pkgstart = seqno;
    pkgstop = PkgGetDataLen(pkg)+seqno;
//  pkgstop = seqno + pkg->datalen;
    if(ccb->rbuf.rcvnxt == pkgstart)
    {
        //this is just the pkg what we want to receive next
        PkgCachedPart(pkg); //cached the package
        //and add it to the receive queue
        if(NULL == ccb->rbuf.pt)
        {
            ccb->rbuf.ph = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.pt,pkg);
//          ccb->rbuf.pt->partnext = pkg;
        }
        ccb->rbuf.pt = pkg;
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt+= rcvlen;
//      ccb->rbuf.buflen += pkg->datalen;
//      ccb->rbuf.rcvnxt+= pkg->datalen;
//      rcvlen += pkg->datalen;
    }
    else if((pkgstop > pkgstart)&&((ccb->rbuf.rcvnxt > pkgstart)&&(ccb->rbuf.rcvnxt < pkgstop)))
    {
        //this is only part of the package data valid
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);
//      pkg->offset += pkgdataoff;
//      pkg->datalen -= pkgdataoff;
        //then cached it and add it to the queue
        PkgCachedPart(pkg); //cached the package
        //and add it to the receive queue
        if(NULL == ccb->rbuf.pt)
        {
            ccb->rbuf.ph = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.pt,pkg);
//          ccb->rbuf.pt->partnext = pkg;
        }
        ccb->rbuf.pt = pkg;
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt+= rcvlen;
//      ccb->rbuf.buflen += pkg->datalen;
//      ccb->rbuf.rcvnxt+= pkg->datalen;
//      rcvlen += pkg->datalen;
    }
    else if((pkgstop < pkgstart)&&((ccb->rbuf.rcvnxt > pkgstart)||(ccb->rbuf.rcvnxt < pkgstop)))
    {
        //this is only part of the package data valid and the seqo turn over
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);
//      pkg->offset += pkgdataoff;
//      pkg->datalen -= pkgdataoff;
        //then cached it and add it to the queue
        PkgCachedPart(pkg); //cached the package
        //and add it to the receive queue
        if(NULL == ccb->rbuf.pt)
        {
            ccb->rbuf.ph = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.pt,pkg);
//          ccb->rbuf.pt->partnext = pkg;
        }
        ccb->rbuf.pt = pkg;
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt+= rcvlen;
//      ccb->rbuf.buflen += pkg->datalen;
//      ccb->rbuf.rcvnxt+= pkg->datalen;
//      rcvlen += pkg->datalen;
    }
    else if(pkgstop == ccb->rbuf.rcvnxt) //maybe a keepalive package,just ignore it
    {

    }
    else
    {
        //this data is out of sequence, we'd better to cached it in the recombination queue
        //could cached in the recombination queue
        if(CFG_TCP_REORDER)
        {
            PkgCachedPart(pkg);
            PkgSetPrivate(pkg, seqno);
//          pkg->Private = seqno;
            //find an proplace to insert the pkg
            if(NULL == ccb->pkgrecomblst)//this queue is empty
            {
                PkgSetNextUnit(pkg,NULL);
//              pkg->partnext = NULL;
                ccb->pkgrecomblst = pkg;
            }
            else
            {
                PkgSetNextUnit(pkg,ccb->pkgrecomblst);
//              pkg->partnext = ccb->pkgrecomblst;
                ccb->pkgrecomblst = pkg;
            }
            ccb->pkgrecomblen++;
        }
    }
    //if we have receive some data to the receive buffer, then we check all the data
    //in the combination queue, if mathes then move it to the receive buffer, else drops now
    if(rcvlen > 0)
    {
#if(CFG_TCP_REORDER == true)
        {
            while(NULL != ccb->pkgrecomblst)
            {
                pkgcomb = ccb->pkgrecomblst;
                ccb->pkgrecomblst = PkgGetNextUnit(pkgcomb);
                PkgSetNextUnit(pkgcomb,NULL);
//              pkgcomb->partnext = NULL;
                pkgstart = (u32)PkgGetPrivate(pkgcomb);
//              pkgstart = pkgcomb->Private;
                pkgstop = pkgstart + PkgGetDataLen(pkgcomb);
//              pkgstop = pkgstart + pkgcomb->datalen;
                if(pkgstart == ccb->rbuf.rcvnxt)
                {
                    //and add it to the receive queue
                    if(NULL == ccb->rbuf.pt)
                    {
                        ccb->rbuf.ph = pkgcomb;
                    }
                    else
                    {
                        PkgSetNextUnit(ccb->rbuf.pt,pkgcomb);
//                      ccb->rbuf.pt->partnext = pkgcomb;
                    }
                    ccb->rbuf.pt = pkgcomb;
                    ccb->rbuf.buflen += PkgGetDataLen(pkgcomb);
                    ccb->rbuf.rcvnxt+= PkgGetDataLen(pkgcomb);
                    rcvlen += PkgGetDataLen(pkgcomb);
//                  ccb->rbuf.buflen += pkgcomb->datalen;
//                  ccb->rbuf.rcvnxt+= pkgcomb->datalen;
//                  rcvlen += pkgcomb->datalen;
                }
                else
                {
                    PkgTryFreePart(pkgcomb);        //drops it now
                }
                ccb->pkgrecomblen = 0;
            }
        }
#endif      //for (CFG_TCP_REORDER == true)
        if(ccb->rbuf.buflen > ccb->rbuf.trigerlevel)
        {
            handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
            semp_post(ccb->rbuf.bufsync);
        }
        if(ccb->rbuf.buflen >= (2*ccb->rbuf.buflenlimit))
        {
            ccb->channelstat |= CN_TCP_CHANNEL_STATCONGEST;
        }
    }
    return rcvlen;
}
#define __TCPCHKINTARGETNOE(target,start,stop) (stop>=start?((target>start)&&(target<stop)):\
                                            ((target>start)||(target<stop)))

#define __TCPCHKINTARGET(target,start,stop) (stop>=start?((target>=start)&&(target<=stop)):\
                                            ((target>=start)||(target<=stop)))

//------------------------------------------------------------------------------
//功能：应答收到的数据
//参数：client，客户端
//     hdr，tcp头
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __ackdata(struct tagSocket *client, struct TcpHdr *hdr)
{
    u32                ackno;
    s32                acklen;
    struct ClienCB    *ccb;
    s32                err;
    s32                uerr;
    u32                usedtime;

    acklen = 0;
    ccb    = (struct ClienCB *)client->TplCB;
    ackno  = ntohl(hdr->ackno);
    //first we will deal the send buffer to release some space
    if((ackno  == ccb->sbuf.unackno)&&(ccb->sbuf.unacklen > 0))
    {
        ccb->sbuf.dupacktims++;
        if(ccb->sbuf.dupacktims > CN_TCP_FASTRETRANS)
        {
            //do the fast retransmition
            ccb->rto = ccb->rto<<1;
            if(ccb->rto < CN_TCP_RTOMIN)
            {
                ccb->rto = CN_TCP_RTOMIN;
            }
            else if(ccb->rto > CN_TCP_RTOMAX)
            {
                ccb->rto = CN_TCP_RTOMAX;
            }

            ccb->ssthresh = ccb->cwnd>>1;
            if(ccb->ssthresh < (2*ccb->mss))
            {
                ccb->ssthresh = 2*ccb->mss;
            }
            ccb->cwnd = ccb->ssthresh + 3*ccb->mss;
            __resenddata(client);
        }
    }
    //1,sndnxtno > unackno, then ackno must be between them
    //2,sndnxtno <= unackno,then ackno must be bigger than unack no or less than
    //  sndnxtno
    else if(((ccb->sbuf.sndnxtno > ccb->sbuf.unackno)&&\
       ((ackno >ccb->sbuf.unackno)&&(ackno <= ccb->sbuf.sndnxtno)))||\
            ((ccb->sbuf.sndnxtno < ccb->sbuf.unackno)&&\
             ((ackno >ccb->sbuf.unackno)||(ackno <= ccb->sbuf.sndnxtno))))
    {
        acklen = ackno-ccb->sbuf.unackno;
        if(acklen > ccb->sbuf.unacklen)
        {
            acklen = ccb->sbuf.unacklen;
        }
        ccb->sbuf.unackoff   +=  acklen;
        if(ccb->sbuf.unackoff >= ccb->sbuf.buflenlimit)
        {
            ccb->sbuf.unackoff -= ccb->sbuf.buflenlimit;
        }
        ccb->sbuf.unackno = ackno;
        ccb->sbuf.unacklen   -=  acklen;
        ccb->sbuf.buflenleft += acklen;
        ccb->sbuf.dupacktims = 0;
        //do the cwnd
        if(ccb->cwnd > ccb->ssthresh)
        {
            ccb->cwnd += ccb->cwnd/32;  //it is congestion avoid
        }
        else
        {
            ccb->cwnd += ccb->mss;
        }
        if(ccb->cwnd < 0)
            ccb->cwnd = CN_LIMIT_SINT32;
        if(ccb->resndtimes == 0)
        {
            //no resend happens,so measure the rto time
            usedtime = NetTickerTicks(pTcpTicker) - ccb->sbuf.rtotick;
            err = usedtime - ccb->sa;
            uerr = err>0?err:(-err);
            //scale the sa sd and rto
            ccb->sa = ccb->sa+(err>>3);
            ccb->sd = ccb->sd + ((uerr-ccb->sd)>>2);
            ccb->rto = ccb->sa + (ccb->sd<<2);
            if(ccb->rto < CN_TCP_RTOMIN)
            {
                ccb->rto = CN_TCP_RTOMIN;
            }
            else if(ccb->rto > CN_TCP_RTOMAX)
            {
                ccb->rto = CN_TCP_RTOMAX;
            }
        }
        else
        {
            ccb->resndtimes = 0;//the remote has acknowledge some data,so just clear it
        }

        if((ccb->sbuf.buflenleft > ccb->sbuf.trigerlevel)||
           (0 ==(ccb->channelstat&CN_TCP_CHANNEL_STATASND)))
        {
            handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE);
            semp_post(ccb->sbuf.bufsync);
        }
        else
        {
            handle_ClrMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE);
        }
        if(ccb->sbuf.sndnxtno == ccb->sbuf.unackno)  //noting to resend, so close the resend timer
        {
            ccb->timerctrl &= (~CN_TCP_TIMER_RESEND);
        }
        ccb->resndtimer = ccb->rto;
    }
    else //ack the unseen data, we could do something or nothing
    {
        //maybe nothing to ack or ack the unseen data
    }
    //update the window
    ccb->sndwnd = (ntohs(hdr->window))<<ccb->sndwndscale;
    if(ccb->sndwnd > ccb->sbuf.unacklen)
    {
        ccb->sndwnd -= ccb->sbuf.unacklen;
        ccb->timerctrl &= (~CN_TCP_TIMER_PERSIST);
    }
    else
    {
        ccb->sndwnd = 0;
        ccb->timerctrl |= CN_TCP_TIMER_PERSIST;
        ccb->persistimer = CN_TCP_TICK_PERSIST;
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：服务器端已经收到FIN
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __rcvsyn_ms(struct tagSocket *client, struct TcpHdr *hdr, struct NetPkg *pkg)
{
    struct ClienCB   *ccb;
    struct ServerCB   *scb;
    struct tagSocket *server;

    //ack the data
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client,hdr);
        ccb = (struct ClienCB *)client->TplCB;
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno) //ok, has ack the syn
        {
            ccb->machinestat = EN_TCP_MC_ESTABLISH;
            ccb->channelstat|= CN_TCP_CHANNEL_STATKRCV| CN_TCP_CHANNEL_STATKSND;
            ccb->sbuf.unackno = ccb->sbuf.sndnxtno;
            //close the syn resend timer
            ccb->timerctrl &= (~CN_TCP_TIMER_SYN);
            ccb->resndtimer = ccb->rto;
            //notice the server to accept
            server = ccb->server;
            scb = (struct ServerCB *)server->TplCB;
            handle_SetMultiplexEvent(fd2Handle(server->sockfd),CN_SOCKET_IOACCEPT);
            semp_post(scb->acceptsemp);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：客户端已经发送了FIN信号
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __sndsyn_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB   *ccb;

    ccb = (struct ClienCB *)client->TplCB;
    //only care about the ack to the syn
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client,hdr);
        if((ccb->sbuf.unackno == ccb->sbuf.sndnxtno)&&\
           (hdr->flags == (CN_TCP_FLAG_ACK|CN_TCP_FLAG_SYN))) //ok, has ack the syn
        {
            ccb->machinestat = EN_TCP_MC_ESTABLISH;
            ccb->channelstat|= CN_TCP_CHANNEL_STATKRCV| CN_TCP_CHANNEL_STATKSND;
            ccb->sbuf.unackno = ccb->sbuf.sndnxtno;
            //close the syn resend timer
            ccb->timerctrl &= (~CN_TCP_TIMER_SYN);
            ccb->resndtimer = ccb->rto;
            dealtcpoption(ccb,hdr);
            ccb->rbuf.rcvnxt = ntohl(hdr->seqno) + 1;
            __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
            //notice the applications  the connect success
            semp_post(ccb->rbuf.bufsync);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：已经处于稳定连接状态
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __stable_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    u32       seqno;
    struct ClienCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClienCB *)client->TplCB;
    //deal the acknowledge to release the send buffer
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    //receive the data to the receive buffer
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        if((0==(CN_TCP_CHANNEL_STATCONGEST&ccb->channelstat))&&\
                   (CN_TCP_CHANNEL_STATKRCV&ccb->channelstat))
        {
            //we will deal the pkg for two reson:
            //1,the channel permit to receive more
            //2,the pkg is sequance and is the last pkg
            rcvlen = __rcvdata(client,seqno,pkg);
        }
        ccb->acktimes ++;
    }
    //if any fin received
    if((CN_TCP_FLAG_FIN & hdr->flags)&&((ccb->rbuf.rcvnxt == seqno)||(rcvlen > 0)))
    {
        ccb->acktimes += CN_TCP_TICK_ACK;
        ccb->rbuf.rcvnxt++;
        ccb->machinestat= EN_TCP_MC_CLOSEWAIT;
        //could never to receive more data
        ccb->channelstat &= (~CN_TCP_CHANNEL_STATKRCV);
        handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        semp_post(ccb->rbuf.bufsync);
        //--TODO,maybe signal an sigpipe signal
    }
    else if(CN_TCP_FLAG_SYN & hdr->flags) //maybe the acknowledge to the syn we sent has lost
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    else
    {

    }
    //if any more data should be sent
    if(ccb->channelstat&CN_TCP_CHANNEL_STATKSND)
    {
        __senddata(client,__chkchannelsendlen(client));
    }
    if(ccb->acktimes >= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：主动关闭发送了FIN
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __finwait1_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    u32      seqno;
    struct ClienCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClienCB *)client->TplCB;
    //deal the acn to release the package in the send buffer
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    //receive the data to the receive buffer
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        if((0==(CN_TCP_CHANNEL_STATCONGEST&ccb->channelstat))&&\
                   (CN_TCP_CHANNEL_STATKRCV&ccb->channelstat))
        {
            //we will deal the pkg for two reson:
            //1,the channel permit to receive more
            //2,the pkg is sequance and is the last pkg
            rcvlen = __rcvdata(client,seqno,pkg);
        }
        ccb->acktimes ++;
    }

    if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)             //FIN ACK --TIME_WAIT
    {
        ccb->timerctrl &= (~CN_TCP_TIMER_FIN);  //CLOSE THE FIN TIMER
        ccb->machinestat= EN_TCP_MC_FINWAIT2;
    }
    //if any fin received
    if((CN_TCP_FLAG_FIN & hdr->flags)&&((ccb->rbuf.rcvnxt== seqno)||(rcvlen > 0)))
    {
        ccb->acktimes += CN_TCP_TICK_ACK;
        ccb->rbuf.rcvnxt++;
        if(ccb->machinestat == EN_TCP_MC_FINWAIT1)
        {
            ccb->machinestat = EN_TCP_MC_CLOSING;
        }
        else
        {
            ccb->machinestat = EN_TCP_MC_TIMEWAIT;
            ccb->timerctrl = CN_TCP_TIMER_2MSL;
            ccb->mltimer = CN_TCP_TICK_2ML;
        }
        //could never to receive more data
        ccb->channelstat &= (~CN_TCP_CHANNEL_STATKRCV);
        handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        semp_post(ccb->rbuf.bufsync);
        //--TODO,maybe signal an sigpipe signal
    }
    if(ccb->acktimes>= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：主动关闭发送了FIN并且收到了ACK，必须考虑到可能还有更多数据到达，但我们却没有去接收
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __finwait2_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    u32      seqno;
    struct ClienCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClienCB *)client->TplCB;
    //deal the acn to release the package in the send buffer
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    //receive the data to the receive buffer
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        if((0==(CN_TCP_CHANNEL_STATCONGEST&ccb->channelstat))&&\
                   (CN_TCP_CHANNEL_STATKRCV&ccb->channelstat))
        {
            //we will deal the pkg for two reson:
            //1,the channel permit to receive more
            //2,the pkg is sequance and is the last pkg
            rcvlen = __rcvdata(client,seqno,pkg);
        }
        ccb->acktimes ++;
    }
    //if any fin received
    if((CN_TCP_FLAG_FIN & hdr->flags)&&((ccb->rbuf.rcvnxt== seqno)||(rcvlen > 0)))
    {
        ccb->acktimes += CN_TCP_TICK_ACK;
        ccb->rbuf.rcvnxt++;
        ccb->machinestat = EN_TCP_MC_TIMEWAIT;
        ccb->timerctrl = CN_TCP_TIMER_2MSL;
        ccb->mltimer = CN_TCP_TICK_2ML;
        //could never to receive more data
        ccb->channelstat &= (~CN_TCP_CHANNEL_STATKRCV);
        handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        semp_post(ccb->rbuf.bufsync);
        //--TODO,maybe signal an sigpipe signal
    }
    if(ccb->acktimes>= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：主动关闭发送了FIN(未收到ACK)，收到对面的FIN，任何数据都会被当做重传数据。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __closing_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB  *ccb;

    ccb = (struct ClienCB *)client->TplCB;
    //deal the ack first
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)
    {
        ccb->machinestat = EN_TCP_MC_TIMEWAIT;
        ccb->timerctrl &= (~CN_TCP_TIMER_FIN);  //CLOSE THE FIN TIMER
        ccb->timerctrl |= CN_TCP_TIMER_2MSL;    //open the 2ml timer
        ccb->mltimer = CN_TCP_TICK_2ML;
     }
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//   if((NULL != pkg)&&(pkg->datalen > 0))
     {
         ccb->acktimes ++;
     }
     if(ccb->acktimes>= CN_TCP_TICK_ACK)
     {
         __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
     }
     return true;
}

//------------------------------------------------------------------------------
//功能：主动关闭发送FIN并且接收到了FIN，2ML超时，任何数据都会被当做重传数据。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __timewait_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB  *ccb;
    ccb = (struct ClienCB *)client->TplCB;

    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        ccb->acktimes ++;
    }
    if(ccb->acktimes>= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：被动关闭收到了对面的FIN，任何数据都会被当做重传数据。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __closewait_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB  *ccb;
    ccb = (struct ClienCB *)client->TplCB;
    //deal the ack first
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    //if any fin received
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        ccb->acktimes ++;
    }
    //see if anymore data to send
    if(ccb->channelstat&CN_TCP_CHANNEL_STATKSND)
    {
        //SEND ANY DATA
        __senddata(client,__chkchannelsendlen(client));
    }
    if(ccb->acktimes>= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：被动关闭发送了FIN，任何数据都会被当做重传数据。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __lastack_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB         *ccb;

    ccb = (struct ClienCB *)client->TplCB;
    //deal the ack first
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client, hdr);
    }
    if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)
    {
        ccb->machinestat = EN_TCP_MC_2FREE;
        ccb->timerctrl &= (~CN_TCP_TIMER_FIN);  //CLOSE THE FIN TIMER
    }
    if((NULL != pkg)&&(PkgGetDataLen(pkg) > 0))
//  if((NULL != pkg)&&(pkg->datalen > 0))
    {
        ccb->acktimes ++;
    }
    //so it will be net_free next
    if(ccb->acktimes>= CN_TCP_TICK_ACK)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：处理socket收到的远程数据包。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __dealrecvpkg(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClienCB  *ccb;
    ccb = (struct ClienCB *)client->TplCB;
    if(ccb->timerctrl&CN_TCP_TIMER_KEEPALIVE)
    {
        ccb->keeptimer = CN_TCP_TICK_KEEPALIVE; //any receive data will reset the keep alive timer
    }
    if((hdr->flags & CN_TCP_FLAG_RST)&&(ccb->rbuf.rcvnxt == htonl(hdr->seqno)))
    {
        __ResetCCB(ccb, EN_TCP_MC_2FREE);           //general deal the reset socket
        handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE|CN_SOCKET_IOREAD);
    }
    else
    {
        //rcv the pkg means we still in the communication, so reset the keepalive
        switch(ccb->machinestat)
        {
            case EN_TCP_MC_SYNSNT:
                __sndsyn_ms(client, hdr, pkg);
                break;
            case EN_TCP_MC_SYNRCV:
                __rcvsyn_ms(client, hdr,pkg);
                break;
            case EN_TCP_MC_ESTABLISH:
                __stable_ms(client, hdr,pkg);
                break;
            case EN_TCP_MC_FINWAIT1:
                __finwait1_ms(client, hdr, pkg);
                break;
            case EN_TCP_MC_FINWAIT2:
                __finwait2_ms(client, hdr,pkg);
                break;
            case EN_TCP_MC_CLOSING:
                __closing_ms(client,hdr,pkg);
                break;
            case EN_TCP_MC_TIMEWAIT:
                __timewait_ms(client, hdr, pkg);
                break;
            case EN_TCP_MC_CLOSEWAIT:
                __closewait_ms(client,hdr,pkg);
                break;
            case EN_TCP_MC_LASTACK:
                __lastack_ms(client,hdr,pkg);
                break;
            default:
                break;
        }
    }
    return true;
}

static struct tagSocket* __newclient(struct tagSocket *server, struct TcpHdr *hdr,\
                          u32 ipdst,u16 portdst,u32 ipsrc, u16 portsrc)
{
    struct ServerCB *scb;
    struct ClienCB *ccb;
    struct tagSocket *result= NULL;

    scb = (struct ServerCB *)server->TplCB;
    if((0 ==(CN_SOCKET_CLOSE&server->sockstat))&&\
       (CN_SOCKET_LISTEN&server->sockstat)&&\
       (scb->pendnum <scb->backlog)&&\
       (CN_TCP_FLAG_SYN&hdr->flags)&&\
       (0 == (CN_TCP_FLAG_ACK&hdr->flags)))  //could accept more in the pending
    {
        result = SocketBuild();
        if(NULL != result)
        {
            result->ProtocolOps = &gTcpProto;
            handle_SetMultiplexEvent(fd2Handle(result->sockfd), 0);
//          memset(result, 0, sizeof(struct tagSocket));
//          result->SockSync = mutex_init(NULL);
            ccb = __CreateCCB();
            if(NULL == ccb)
            {
                SocketFree(result);
                result = NULL;
            }
            else
            {
                result->TplCB = (void *)ccb;
                result->sockstat |=CN_SOCKET_CLIENT|CN_SOCKET_PROBLOCK|CN_SOCKET_PRONAGLE;
                result->element.v4.iplocal = ipdst;
                result->element.v4.portlocal = portdst;
                result->element.v4.ipremote = ipsrc;
                result->element.v4.portremote = portsrc;
                result->af_inet = AF_INET;
                result->ProtocolOps = &gTcpProto;
                ccb->rbuf.rcvnxt = htonl(hdr->seqno)+1;
                ccb->sndwnd = htons(hdr->window);
                dealtcpoption(ccb,hdr);

                //ok, now send the syn ack and open the syn timer
                __sendflag(result,CN_TCP_FLAG_ACK|CN_TCP_FLAG_SYN,\
                          (void *)&sgSynOptDefault,sizeof(sgSynOptDefault),ccb->sbuf.sndnxtno);
                result->Nextsock = scb->clst;
                scb->clst = result;
                scb->pendnum++;
                ccb->server = server;
                ccb->timerctrl = CN_TCP_TIMER_SYN;
                ccb->resndtimer = ccb->rto;
                ccb->machinestat = EN_TCP_MC_SYNRCV;
                ccb->sbuf.sndnxtno++;
            }
        }
    }

    return result;

}
static struct tagSocket* __tcpmatchclient(struct tagSocket *server, u32 ip, u16 port)
{
    struct tagSocket *result = NULL;
    struct tagSocket *client;
    struct ServerCB   *scb;

    scb = (struct ServerCB *)server->TplCB;
    client = scb->clst;
    while(NULL != client)
    {
        if((ip == client->element.v4.ipremote)&&(port == client->element.v4.portremote))
        {
            result = client;
            break;
        }
        else
        {
            client = client->Nextsock;
        }
    }
    return result;
}

static bool_t __tcprcvdealv4(u32 ipsrc, u32 ipdst,  struct NetPkg *pkg, u32 devfunc)
{

    u16         portdst;
    u16         portsrc;
    struct TcpHdr  *hdr;
    struct tagSocket  *server;
    struct tagSocket  *client;
    struct tagSocket  *sock;
    u32         cpyhdr[15];

    if(0 == (devfunc &CN_IPDEV_TCPICHKSUM))
    {
        if(false == __rcvchecksum(ipdst, ipsrc, pkg))
        {
            return true;  //chksum failed
        }
    }
    hdr = (struct TcpHdr *)PkgGetCurrentBuffer(pkg);
//  hdr = (struct TcpHdr *)(pkg->buf + pkg->offset);
    PkgMoveOffsetUp(pkg, (hdr->hdroff>>4)*4);
//    pkg->offset += (hdr->hdroff>>4)*4;
//    if((PkgGetDataLen(pkg) > (hdr->hdroff>>4)*4)
////  if(pkg->datalen> (hdr->hdroff>>4)*4)
//    {
//        (PkgSetDataLen(pkg) -= (hdr->hdroff>>4)*4;
////      pkg->datalen -= (hdr->hdroff>>4)*4;
//    }
//    else
//    {
//        pkg = NULL;
//    }
    //cpy the hdr to the thread stack avoid the align error
    memcpy((void *)cpyhdr,(void *)hdr,(hdr->hdroff>>4)*4);
    hdr = (struct TcpHdr *)cpyhdr;

    //ok,now we deal the pkg now, the pkg check success
    portdst = hdr->portdst;
    portsrc = hdr->portsrc;

    //if any client match this pkg
    mutex_lock(pTcpHashTab->tabsync);
    if((sock = __hashSocketSearch(ipdst,portdst,ipsrc,portsrc)) != NULL)
    {
        //get the communicate client
        client = sock;
        mutex_lock(client->SockSync);
        __dealrecvpkg(client,hdr,pkg);
        mutex_unlock(client->SockSync);
    }
    else if((sock = __hashSocketSearch(INADDR_ANY,portdst,ipsrc,portsrc)) != NULL)
    {
        //the inaddr_any client matches
        client = sock;
        mutex_lock(client->SockSync);
        __dealrecvpkg(client,hdr,pkg);
        mutex_unlock(client->SockSync);
    }
    else if((sock = __hashSocketSearch(ipdst,portdst,0,0))!= NULL)
    {
        //the specified server matches
        server = sock;
        mutex_lock(server->SockSync);
        client = __tcpmatchclient(server,ipsrc,portsrc);
        if(NULL == client)
        {
            client = __newclient(server,hdr,ipdst, portdst, ipsrc, portsrc);
            if(NULL == client) //could not add any more client
            {
                __resetremoteraw(ipdst, portdst,ipsrc, portsrc,ntohl(hdr->seqno),ntohl(hdr->ackno));
            }
        }
        else
        {
            __dealrecvpkg(client,hdr,pkg);
        }
        mutex_unlock(server->SockSync);
    }
    else if((sock = __hashSocketSearch(INADDR_ANY,portdst,0,0))!= NULL)
    {
        //the inaddr_any server matches
        server = sock;
        mutex_lock(server->SockSync);
        client = __tcpmatchclient(server,ipsrc,portsrc);
        if(NULL == client)
        {
            client = __newclient(server,hdr,ipdst, portdst, ipsrc, portsrc);
            if(NULL == client) //could not add any more client
            {
                __resetremoteraw(ipdst, portdst,ipsrc, portsrc,ntohl(hdr->seqno),ntohl(hdr->ackno));
            }
        }
        else
        {
            __dealrecvpkg(client,hdr,pkg);
        }
        mutex_unlock(server->SockSync);
    }
    else   //no port matches, so just reset it
    {
        if(0==(hdr->flags & CN_TCP_FLAG_RST))
        {
            __resetremoteraw(ipdst, portdst,ipsrc, portsrc,ntohl(hdr->seqno),ntohl(hdr->ackno));
        }
    }
    mutex_unlock(pTcpHashTab->tabsync);

    return true;

}

static bool_t __rcvdeal(tagIpAddr *addr,struct NetPkg *pkglst, u32 devfunc)
{
    bool_t result = false;
    enum_ipv_t  ver;
    u32 ipsrc;
    u32 ipdst;
    if((NULL != addr)&&(NULL != pkglst))
    {
        ver = addr->ver;
        if(ver == EN_IPV_4)
        {
            ipsrc = addr->src.ipv4.s_addr;
            ipdst = addr->dst.ipv4.s_addr;
            result = __tcprcvdealv4(ipsrc,ipdst,pkglst,devfunc);
        }
    }
    return result;
}

// =============================================================================
// FUNCTION:deal the tcp timer
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:true success while false failed
// =============================================================================
static bool_t __dealclienttimer(struct tagSocket *client)
{
    u8        flag;
    struct ClienCB   *ccb;

    ccb = (struct ClienCB *)client->TplCB;
    if(ccb->timerctrl&CN_TCP_TIMER_LINGER)   //deal the linger timer
    {
        if( 0 == ccb->lingertimer)
        {
            __ResetCCB(ccb,EN_TCP_MC_2FREE);
        }
        else
        {
            ccb->lingertimer--;
        }
    }

    if(ccb->timerctrl &CN_TCP_TIMER_2MSL)    //deal the 2ml timer
    {
        if(ccb->mltimer == 0)
        {
            __ResetCCB(ccb,EN_TCP_MC_2FREE);
        }
        else
        {
            ccb->mltimer--;
        }
    }
    //chk all the transmissions if any package need retransmission
    if(ccb->timerctrl &(CN_TCP_TIMER_SYN|CN_TCP_TIMER_RESEND|CN_TCP_TIMER_FIN))
    {
        if(ccb->resndtimer == 0)
        {

            if(ccb->resndtimes > CN_TCP_RSNDMAX)
            {
                if ((client->sockstat & CN_SOCKET_CLOSE)
                        || (0 == (client->sockstat & CN_SOCKET_OPEN))) {
                    __ResetCCB(ccb, EN_TCP_MC_2FREE);
                } else //just state the state as it before but close the channel
                {
                    __ResetCCB(ccb, ccb->machinestat);
                }
            }
            else
            {
                if(ccb->timerctrl & CN_TCP_TIMER_SYN)  //syn
                {
                    //we will help you to send the syn
                    flag = CN_TCP_FLAG_SYN;
                    if(ccb->machinestat == EN_TCP_MC_SYNRCV)
                    {
                        flag |= CN_TCP_FLAG_ACK;
                    }
                    __sendflag(client,flag,&sgSynOptDefault,sizeof(sgSynOptDefault),ccb->sbuf.unackno);
                }
                else if(ccb->timerctrl & CN_TCP_TIMER_RESEND) //resend
                {
                    //do the resend
                    __resenddata(client);

                }
                else  //fin
                {
                    __sendflag(client,CN_TCP_FLAG_FIN|CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno-1);
                }
                //do the rto update
                ccb->rto = ccb->rto<<1;
                if(ccb->rto < CN_TCP_RTOMIN)
                {
                    ccb->rto = CN_TCP_RTOMIN;
                }
                else if(ccb->rto > CN_TCP_RTOMAX)
                {
                    ccb->rto = CN_TCP_RTOMAX;
                }
                //because we has dealt an timeout retransmition, so we must deal the
                //congestion window
                ccb->ssthresh = ccb->cwnd>>1;
                if(ccb->ssthresh < (2*ccb->mss))
                {
                    ccb->ssthresh = 2*ccb->mss;
                }
                ccb->cwnd = ccb->mss;
                ccb->resndtimer = ccb->rto;
                ccb->resndtimes++;
            }
        }
        else
        {
            ccb->resndtimer--;
        }
    }
    if(ccb->timerctrl &CN_TCP_TIMER_CORK)     //do the socket cork
    {
        //this is the to-do deal
    }
    if(ccb->timerctrl &CN_TCP_TIMER_KEEPALIVE)   //do the socket keepalive timer
    {
        if(ccb->keeptimer == 0)
        {
            if ((ccb->sbuf.unacklen == 0) && (ccb->sbuf.datalen == 0)) {
                ccb->sbuf.unackoff = (ccb->sbuf.unackoff + ccb->sbuf.buflenlimit
                        - 1) % ccb->sbuf.buflenlimit;
                ccb->sbuf.unacklen = 1;
                ccb->sbuf.unackno = ccb->sbuf.unackno - 1;
                ccb->timerctrl |= CN_TCP_TIMER_RESEND;
            }
            else {
            }
            ccb->keeptimer = CN_TCP_TICK_KEEPALIVE;
        }
        else
        {
            ccb->keeptimer--;
        }
    }
    if(ccb->timerctrl &CN_TCP_TIMER_PERSIST)   //do the socket persist
    {
        if(0 == ccb->persistimer)
        {
            if(ccb->sbuf.datalen > 0)
            {
                __senddata(client,1);
            }
//          else   //better not to send the flag
//          {
//              __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
//          }
            ccb->persistimer = CN_TCP_TICK_PERSIST;
        }
        else
        {
            ccb->persistimer--;
        }
    }
    if(ccb->acktimes > 0)
    {
        __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
    }
    if(NULL != ccb->pkgrecomblst)
    {
        PkgTryFreeQ(ccb->pkgrecomblst);
        ccb->pkgrecomblst = NULL;
        ccb->pkgrecomblen = 0;
    }
    return true;
}
// =============================================================================
// FUNCTION:the tcp tick main function
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:never return
// =============================================================================
static void __tcptick(void)
{
    struct tagSocket    *sock;
    struct tagSocket    *server;
    struct tagSocket    *client;
    struct tagSocket    *clientnxt;
    struct tagSocket    *clientpre;
    struct ServerCB       *scb;
    struct ClienCB       *ccb;
    s32 i;

    for(i = 0; i <pTcpHashTab->tablen; i ++)
    {
        //each hash number we will lock and unlock ,so left some time for others
        mutex_lock(pTcpHashTab->tabsync);
        sock = pTcpHashTab->array[i];
        while(NULL != sock)
        {
            mutex_lock(sock->SockSync);
            if(CN_SOCKET_CLIENT&sock->sockstat)//this is the client
            {
                client = sock;
                ccb = (struct ClienCB *)client->TplCB;
                if((CN_SOCKET_CLOSE&client->sockstat)&&\
                    (EN_TCP_MC_2FREE == ccb->machinestat))
                {
                    __ResetCCB(ccb,EN_TCP_MC_2FREE);
                    __DeleCCB(ccb);
                    sock = client->Nextsock;
                    __hashSocketRemove(client);
                    SocketFree(client);
                }
                else
                {
                    __dealclienttimer(client);
                    sock = client->Nextsock;
                    mutex_unlock(client->SockSync);
                }
            }//end for the client
            else//this is an server, we should deal the client hang on it
            {
                server = sock;
                scb = (struct ServerCB *)server->TplCB;
                client = scb->clst;
                clientpre = client;
                while(NULL != client)  //deal all the client
                {
                    __dealclienttimer(client);
                    ccb = (struct ClienCB *)client->TplCB;
                    if(EN_TCP_MC_2FREE == ccb->machinestat)
                    {
                        //remove it from the queue
                        if(client == scb->clst)
                        {
                            scb->clst = client->Nextsock;
                            clientpre = client->Nextsock;
                        }
                        else
                        {
                            clientpre->Nextsock = client->Nextsock;
                        }
                        clientnxt = client->Nextsock;
                        client->Nextsock = NULL;
                        __ResetCCB(ccb,EN_TCP_MC_2FREE);
                        __DeleCCB(ccb);
                        SocketFree(client);  //net_free the client
                        if (scb->pendnum > 0) {
                            scb->pendnum--;
                            semp_pendtimeout(scb->acceptsemp, 0);
                        }
                    }
                    else if (EN_TCP_MC_CLOSEWAIT == ccb->machinestat) {
                        //send the fin to close
                        ccb->channelstat &= (~(CN_TCP_CHANNEL_STATKSND));
                        __sendflag(sock, CN_TCP_FLAG_FIN | CN_TCP_FLAG_ACK,
                                NULL, 0, ccb->sbuf.sndnxtno);
                        ccb->sbuf.sndnxtno++;
                        ccb->resndtimer = ccb->rto;
                        ccb->timerctrl |= CN_TCP_TIMER_FIN;
                        ccb->machinestat = EN_TCP_MC_LASTACK;
                        clientpre = client;
                        clientnxt = client->Nextsock;
                    }
                    else
                    {
                        clientpre = client;
                        clientnxt = client->Nextsock;
                    }
                    client = clientnxt;
                }
                //deal the server it self
                sock = server->Nextsock;
                mutex_unlock(server->SockSync);
            }
        }
        mutex_unlock(pTcpHashTab->tabsync);
    }

    return;
}

static char *gCCBLinkStat[]=
{
    "EN_TCP_MC_CREAT",
    "EN_TCP_MC_SYNRCV",
    "EN_TCP_MC_SYNSNT",
    "EN_TCP_MC_ESTABLISH",
    "EN_TCP_MC_FINWAIT1",
    "EN_TCP_MC_FINWAIT2",
    "EN_TCP_MC_CLOSING",
    "EN_TCP_MC_TIMEWAIT",
    "EN_TCP_MC_CLOSEWAIT",
    "EN_TCP_MC_LASTACK",
    "EN_TCP_MC_2FREE"
};

static void __tcpdebugsockinfo(struct tagSocket *sock,char *prefix)
{
    debug_printf("tcp","%s:iplocal :%s    portlocal :%d\r\n",\
            prefix,inet_ntoa(*(struct in_addr*)&sock->element.v4.iplocal),ntohs(sock->element.v4.portlocal));
    debug_printf("tcp","%s:ipremote:%s    portremote:%d\r\n",\
            prefix,inet_ntoa(*(struct in_addr*)&sock->element.v4.ipremote),ntohs(sock->element.v4.portremote));
    debug_printf("tcp","%s:sockstat:0x%08x    UserTag:0x%08x\n\r",\
            prefix,sock->sockstat,sock->SockUserTag);
    debug_printf("tcp","%s:syncstat:%d\n\r",prefix,sock->SockSync->enable);
    debug_printf("tcp","%s:errno   :%d\n\r",prefix,sock->errorno);
}

static void __tcpdebugccb(struct ClienCB *ccb,char *prefix)
{
    //machine state
    debug_printf("tcp","%s:machinestat:%s\n\r",prefix,gCCBLinkStat[ccb->machinestat]);
    //channel stat
    debug_printf("tcp","%s:channelstat:apprcv:%s\n\r",prefix,\
            ccb->channelstat&CN_TCP_CHANNEL_STATARCV?"enable":"disable");
    debug_printf("tcp","%s:channelstat:appsnd:%s\n\r",prefix,\
            ccb->channelstat&CN_TCP_CHANNEL_STATASND?"enable":"disable");
    debug_printf("tcp","%s:channelstat:knlrcv:%s\n\r",prefix,\
            ccb->channelstat&CN_TCP_CHANNEL_STATKRCV?"enable":"disable");
    debug_printf("tcp","%s:channelstat:knlsnd:%s\n\r",prefix,\
            ccb->channelstat&CN_TCP_CHANNEL_STATKSND?"enable":"disable");
    //send buffer
    debug_printf("tcp","%s:sndbuffer:buflen:%04d bufleft:%04d  unacklen:%04d datalen:%04d\n\r",prefix,\
            ccb->sbuf.buflenlimit,ccb->sbuf.buflenleft,ccb->sbuf.unacklen,ccb->sbuf.datalen);
    debug_printf("tcp","%s:sndbuffer:unackoff:%04d sndnxtoff:%04d dataoff:%04d  trigger:%04d\n\r",prefix,\
            ccb->sbuf.unackoff,ccb->sbuf.sndnxtoff,ccb->sbuf.dataoff,ccb->sbuf.trigerlevel);
    debug_printf("tcp","%s:sndbuffer:unackno:0x%08x sndnxtno:0x%08x \n\r",prefix,\
            ccb->sbuf.unackno,ccb->sbuf.sndnxtno);
    debug_printf("tcp","%s:sndbuffer:timeout:0x%08x rtotick:0x%08x dupacks:%04d SockSync:%s\n\r",prefix,\
            ccb->sbuf.timeout,ccb->sbuf.rtotick,ccb->sbuf.dupacktims,ccb->sbuf.bufsync->lamp_counter?"enable":"disable");
    //receive buffer
    debug_printf("tcp","%s:rcvbuf:buflen:%04d datalen:%04d trigerlevel:%04d\n\r",\
            prefix,ccb->rbuf.buflenlimit,ccb->rbuf.buflen,ccb->rbuf.trigerlevel);
    debug_printf("tcp","%s:rcvbuf:rcvnxtno:0x%08x timeout:0x%08x SockSync:%s\n\r",prefix,\
            ccb->rbuf.rcvnxt,ccb->rbuf.timeout,ccb->rbuf.bufsync->lamp_counter?"enable":"disable");
    //recomb queue
    debug_printf("tcp","%s:recomb:len:%d\n\r",prefix,ccb->pkgrecomblen);
    //windows
    debug_printf("tcp","%s:mss:%04d sndwnd:%d sndwndscale:%d\n\r",prefix,ccb->mss,ccb->sndwnd,ccb->sndwndscale);
    debug_printf("tcp","%s:cwnd :%d ssh:%d \r\n",prefix,ccb->cwnd,ccb->ssthresh);
    //resend
    debug_printf("tcp","%s:resndtimer:%04d resndtimes:%04d acktimes:%04d\r\n",prefix,\
            ccb->resndtimer,ccb->resndtimes,ccb->acktimes);
    //timer
    debug_printf("tcp","%s:syntimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_SYN?"enable":"disable",ccb->mltimer);
    debug_printf("tcp","%s:2mltimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_2MSL?"enable":"disable",ccb->mltimer);
    debug_printf("tcp","%s:kpatimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_KEEPALIVE?"enable":"disable",ccb->keeptimer);
    debug_printf("tcp","%s:pestimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_PERSIST?"enable":"disable",ccb->persistimer);
    debug_printf("tcp","%s:cortimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_CORK?"enable":"disable",ccb->corktimer);
    debug_printf("tcp","%s:ligtimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_LINGER?"enable":"disable",ccb->lingertimer);
    debug_printf("tcp","%s:sndtimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_RESEND?"enable":"disable",ccb->mltimer);
    debug_printf("tcp","%s:fintimer:%s:%d\r\n",prefix,ccb->timerctrl&CN_TCP_TIMER_FIN?"enable":"disable",ccb->mltimer);
    return;
}

static void __tcpdebugscb(struct ServerCB *scb,char *prefix)
{
    debug_printf("tcp","%s:backlog  :%d pendnum:%d\n\r",prefix,scb->backlog,scb->pendnum);
    debug_printf("tcp","%s:accepttm :0x%08x acceptsync:%d\n\r",prefix,scb->accepttime,scb->acceptsemp->lamp_counter);
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define CN_TCP_DEBUG_PREFIX  "         "
static void __tcpdebug(struct tagSocket *sock,char *filter)
{
    struct ClienCB   *ccb;
    struct ServerCB   *scb;

    if(sock->sockstat&CN_SOCKET_CLIENT)
    {
        ccb = (struct ClienCB *)sock->TplCB;
        debug_printf("tcp","TCPCLIENT:address:0x%08x\n\r",(u32)sock);
        __tcpdebugsockinfo(sock,CN_TCP_DEBUG_PREFIX);
        __tcpdebugccb(ccb,CN_TCP_DEBUG_PREFIX);
    }
    else
    {
        scb = (struct ServerCB *)sock->TplCB;
        debug_printf("tcp","TCPSERVER:address:0x%08x\n\r",(u32)sock);
        __tcpdebugsockinfo(sock,CN_TCP_DEBUG_PREFIX);
        __tcpdebugscb(scb,CN_TCP_DEBUG_PREFIX);
    }
}

#pragma GCC diagnostic pop

//------------------------------------------------------------------------------
//功能：tcp协议初始化
//参数：无
//返回：true or false
//------------------------------------------------------------------------------
bool_t TcpInit(void)
{
    bool_t ret = false;

    //do the port random initialize
//    gPortEngineTcp = (u16)RNG_Get_RandomRange(1024,65535);
    gPortEngineTcp = (u16)(rand() >> 16)%(0xffff-1024)+1024;
    if(false == __hashTabInit(CFG_TCP_CCBNUM+CFG_TCP_SCBNUM))
    {
        goto EXIT_REGISTERTCPFAILED;
    }
    if(false == __initCB(CFG_TCP_CCBNUM,CFG_TCP_SCBNUM))
    {
        goto EXIT_REGISTERTCPFAILED;
    }

    sgSynOptDefault.mss.kind = EN_TCP_OPT_MSS;             //do the syn option
    sgSynOptDefault.mss.len = 4;
    sgSynOptDefault.mss.mss = htons(CN_TCP_MSSDEFAULT);
    sgSynOptDefault.winscale.resvered = EN_TCP_OPT_NONE;
    sgSynOptDefault.winscale.kind = EN_TCP_OPT_SCALE;
    sgSynOptDefault.winscale.len = 3;
    sgSynOptDefault.winscale.shiftbits = 0;


    memset((void *)&gTcpProto,0,sizeof(gTcpProto));
    gTcpProto.__socket = __tcpsocket;                //register the tcp to the  tpl
    gTcpProto.__bind = __tcpbind;
    gTcpProto.__listen = __tcplisten;
    gTcpProto.__accept = __tcpaccept;
    gTcpProto.__connect = __tcpconnect;
    gTcpProto.__getsockopt = __tcpgetsockopt;
    gTcpProto.__recv = __tcprecv;
    gTcpProto.__recvfrom = NULL;
    gTcpProto.__send = __tcpsend;
    gTcpProto.__sendto = NULL;
    gTcpProto.__setsockopt = __tcpsetsockopt;
    gTcpProto.__shutdown = __tcpshutdown;
    gTcpProto.__close = __closesocket;
    gTcpProto.__debuginfo = __tcpdebug;

    if((false ==TPL_RegisterProto(AF_INET,SOCK_STREAM,IPPROTO_TCP, &gTcpProto))||\
       (false ==TPL_RegisterProto(AF_INET,SOCK_STREAM,0, &gTcpProto))||\
       (false == IpInstallProto("tcp",IPPROTO_TCP,__rcvdeal)))
    {
        debug_printf("tcp","%s:ERR:TCP PROTO REGISTER FAILED\n\r",__FUNCTION__);
        goto EXIT_REGISTERTCPFAILED;
    }
    pTcpTicker = NetTickerIsrInstall("TCPTIKER",__tcptick,100); //100 ms
    ret = true;

EXIT_REGISTERTCPFAILED:
    info_printf("tcpip","LOAD:TCP------------%s\n\r",ret?"SUCCESS":"FAILURE");
//    __LoadLog("TCP",ret);
    return ret;
}

