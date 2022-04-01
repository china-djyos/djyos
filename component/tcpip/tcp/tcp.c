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
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
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
#define     CFG_TCP_CCBNUM              10      //"tcp 客户端数限值"，占一个 指针 和 struct ClientCB
#define     CFG_TCP_SCBNUM              5       //"tcp 服务器数限值"，占一个 指针 和 struct ServerCB
#define     CFG_TCP_SOCKET_HASH_LEN     10      //"tcp socket hashtable长度"，用于通过“IP+port”四元组检索socket
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
    EN_TCP_MC_SYNRCV,      //socket收到了SYN
    EN_TCP_MC_SYNSNT,      //socket已经发送了SYN信号
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
//#define CN_TCP_LISTENDEFAULT    5           //LISTEN BAKLOG DEFAULT
#define CN_TCP_RCMBLENDEFAULT   0x10        //most allowing num frame in the recombine list
#define CN_TCP_ACCEPTMAX        0x10000     //服务器listen后，accept前能排队的客户端数量

//define for the channel stat
//rcv fin makes krcv 0
//snt fin makes ksnd 0
//shutdown_rd makes krcv and arcv 0
//shutdown_wr makes asnd 0
//close makes the ksnd krcv 0 arcv 0 asnd 0
#define CN_TCP_CHANNEL_STATASND    (1<<0)  //APP COULD SND DATA
#define CN_TCP_CHANNEL_STATARCV    (1<<1)  //APP COULD RCV DATA
#define CN_TCP_CHANNEL_STATKSND    (1<<2)  //STACK COULD SND DATA
#define CN_TCP_CHANNEL_STATKRCV    (1<<3)  //STACK COULD RCV DATA
#define CN_TCP_CHANNEL_STATCONGEST (1<<4)  //the rcv window is full or channel is bad

//tcp timer定义，所有定义都是tcp ticks 数，每tick=100mS。
#define CN_TCP_TICK_TIME                   (100*mS) //Units:Micro Seconds
#define CN_TCP_TICK_2ML                    (10)     //unit:tcp tick
#define CN_TCP_TICK_KEEPALIVE              (500*10) //unit:tcp tick
#define CN_TCP_TICK_PERSIST                (10)     //unit:tcp tick
#define CN_TCP_TICK_ACK                    (5)      //Unit:package number
#define CN_TCP_TIMER_SYN                   (1<<0)   //SYNC TIMER
#define CN_TCP_TIMER_2MSL                  (1<<1)   //2ML TIMER control bit
#define CN_TCP_TIMER_KEEPALIVE             (1<<2)   //KEEP TIMER control
#define CN_TCP_TIMER_PERSIST               (1<<3)   //PERSIST TIMER control bit
#define CN_TCP_TIMER_CORK                  (1<<4)   //积攒数据 TIMER control bit
#define CN_TCP_TIMER_LINGER                (1<<5)   //LINGER TIMER START
#define CN_TCP_TIMER_RESEND                (1<<6)   //RESEND TIMER
#define CN_TCP_TIMER_FIN                   (1<<7)   //FIN TIMER
#define CN_TCP_TIMER_NONE                  (0)      //NO TIMER IS START
//define for the rtt
#define CN_TCP_RTOMAX                      150    //重传定时器起始时间最大值:tcp tick
#define CN_TCP_RTOMIN                      2      //重传定时器起始时间最小值:tcp tick
#define CN_TCP_SA_INIT                     0      //rtt平均值的初始值:tcp tick
#define CN_TCP_SD_INIT                     4      //rtt平均值偏差的初始值:tcp tick
#define CN_TCP_FASTRETRANS                 3      //收到多少次相同 ackno 执行快速重传
#define CN_TCP_CWND_LIMIT                  60000
#define CN_TCP_RSNDMAX                     8     //最大重传次数
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
    struct SemaphoreLCB      *bufsync;     //初始化为无信号，
    u32                       timeout;     //if block, the block time
    s32                       buflen;      //the valid data length in the buffer
    s32                       buflenlimit; //the buffer length
    s32                       trigerlevel; //the buffer trigerlevel
    struct NetPkg            *phead;          //the buffer package head
    struct NetPkg            *ptail;          //the buffer package tail
    u32                       rcvnxt;      //this is we receive next number
}tagRecvBuf;
//send buffer
typedef struct
{
    struct SemaphoreLCB *bufsync;     //buflenleft超过trigerlevel则post
    u32                  timeout;     //if block, the block time
    u32                  rtotick;     //最后一次发送数据 以来的ticks数
    u8                  *tcpsendbuf;  //tcp发送buf，malloc得到。
    s32                  buflenlimit; //创建ccb时赋值，重设buffer len时赋值为buffer len.
    u32                  unackno;     //等待对方确认的序号，创建ccb时赋值为 sndnxtno；
                                      //收到对方ack时，赋值为对方的ackno（期待收到）；
                                      //保活定时器启动时-1
    s32                  unackoff;    //创建ccb时清零；__ackdata时增量，到缓冲区末则清零；
                                      //重设buffer len 时清零;
    s32                  unacklen;    //uackoff 和 sndnxtoff之间的长度，创建ccb时清零;
                                      //重设buffer len 时清零;__ackdata时减量；保活定时器启动时-1
    u32                  sndnxtno;    //待发送的序号，创建ccb时用__computeIsn得到，
                                      //每发送数据时增加，发SYN则额外增加1
    s32                  sndnxtoff;   //待发送的数据偏移，创建ccb时清零；__senddata时增量，
                                      //到缓冲区末则清零；重设buffer len 时清零;
    s32                  dataoff;     //buffer写入偏移，创建ccb时清零；__cpy2sndbuf时增量，
                                      //到缓冲区末则清零；重设buffer len 时清零;
    s32                  datalen;     //未发送数据长度(不含未确认)，创建ccb时清零，__cpy2sndbuf时增量，
                                      //__senddata时减量,重设buffer len 时清零
    s32                  buflenleft;  //空闲buffer量，创建ccb时赋值，__cpy2sndbuf时减量，
                                      //__ackdata时增量；重设buffer len时赋值为buffer len.
    s32                  trigerlevel; //缓冲区触发水平
    u8                   dupacktims;  //收到相同 ackno 计数，达到限值执行快速重传
                                      //unackno 改变时清零，收到 ackno==unackno 时增量
}tagSendBuf;  //this buf used for the tcp send
//each client has an ClientCB for the tcp state control
struct ClientCB
{
    struct ClientCB           *nxt;          //用于动态分配内存块
    struct tagSocket         *server;       //非空表示boundin连接，并指向相应服务器
    u16                       machinestat;  //the machine stat of the tcb
    u16                       channelstat;  //the stat of the channel,which mean we could recv or send
    tagRecvBuf                rbuf;         //rcv buffer
    tagSendBuf                sbuf;         //sendbuffer
    struct NetPkg            *pkgrecomblst; //the package recomb queue
    //the send window member
    u16                       mss;         //单次发送的最大数据包
    u8                        sndwndscale; //远程窗口放大指数选项，握手时 syn 包传递
    s32                       cwnd;        //拥塞避免窗口，初始化为 mss*10
    s32                       ssthresh;    //慢启动阈值，用来确定cwnd的中间变量，默认 65535
    s32                       sndwnd;      //远程接收窗口，syn时初始化，发送时减量，
                                           //收到包时更新，但要减掉未确认的数据
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
    s16 rto;                   //重传定时器起始周期，发生重传时指数增加
    //tcp timer
    u16                        timerctrl;
    u16                        mltimer;     //(unit:tcp tick,used for 2msl wait)
    u16                        lingertimer; //(unit:tcp tick,used for linger close)
    u16                        keeptimer;   //(unit:tcp tick,used for keep alive)
    u16                        persistimer; //(unit:tcp tick,used for probe window)
    u16                        corktimer;   //(unit:tcp tick,used for 积攒数据 timeout(supper Nagle))
    u16                        resndtimer;  //数据重传、syn、fin三合一定时器，从rto开始，经 CN_TCP_RSNDMAX
                                            //      次退避重传后收不到响应，终止，默认参数约204秒。
                                            //syn：发送syn后启动
                                            //数据重传：发送数据后，或收到部分数据ack后启动
                                            //fin：发送 fin 后启动
    u8                         resndtimes;  //when resend,add it;when ack the data then clear it
    u8                         acktimes;    //if acktimes overrun the limit, then will do fast ack
};
//each server has an ServerCB
struct ServerCB
{
    struct ServerCB           *nxt;                 //用于动态分配内存块
    s32                        pendlimit;           //which limit the pending num
    s32                        pendnum;             //which means how much still in pending
    u32                        accepttime;          //block time for the accept
    struct tagSocket          *clst;                //该服务器accetp的所有客户端，including the pending stat
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

//用于搜索跟“IP+port”四元组匹配的socket，哈希算法见 __hashTabInit 函数说明
typedef struct
{
//  s32                          tablen;     //how long the hash tab is
    struct MutexLCB             *tabsync;    //used to peotect the hash tab
    struct tagSocket            *array[CFG_TCP_SOCKET_HASH_LEN];   //this is the hash tab
}tagTcpHashTab;
static tagTcpHashTab   TcpHashTab;
static void *pTcpTicker = NULL;

//-----------------------------------------------------------------------------
//功能: 初始化tcp hash表，此表用于接收到数据包时匹配相应的socket用，hashtable所需内存
//      从系统heap中分配。hash算法：把地址四元组全部按无符号数加起来，再与tablen取模，
//      得hash值，hash值相同的 socket 用 Nextsock 指针串起来，是单向不循环链表。
//参数: len，hash表长度
//返回: true = 成功，false=失败，因内存不足
//-----------------------------------------------------------------------------
static bool_t __hashTabInit( void )
{
//  TcpHashTab = net_malloc(sizeof(tagTcpHashTab) + len *sizeof(struct tagSocket *));
//  if(NULL == TcpHashTab)
//  {
//      goto ERR_ARRAYMEM;
//  }
//  memset((void *)TcpHashTab,0,sizeof(tagTcpHashTab) + len *sizeof(struct tagSocket *));

    TcpHashTab.tabsync = Lock_MutexCreate(NULL);
    if(NULL == TcpHashTab.tabsync)
    {
        goto ERR_SYNC;
    }

//  TcpHashTab.tablen = len;

    return true;

ERR_SYNC:
//    net_free(TcpHashTab);
//    TcpHashTab = NULL;
//ERR_ARRAYMEM:
    return false;
}

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
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;
    tmp = TcpHashTab.array[hashKey];
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

//------------------------------------------------------------------------------
//功能：在hashtable中查找socket，但只匹配本地端口和IP，用于确保不会bind重复的地址。
//参数: iplocal，本地IP
//      portlocal，本地端口
//返回: socket指针，或NULL
//-----------------------------------------------------------------------------
static struct tagSocket *__hashSocketLocalSearch(u32 iplocal, u16 portlocal)
{
    struct tagSocket *result = NULL;

    s32 i = 0;
    struct tagSocket *tmp;

    for(i =0; i < CFG_TCP_SOCKET_HASH_LEN;i++ )
    {
        tmp = TcpHashTab.array[i];
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

//------------------------------------------------------------------------------
//功能：添加hash表项，如果有hash值重复的，则添加在相同hash的链表末尾。
//参数：sock，待添加的表项
//返回：true。
//------------------------------------------------------------------------------
static bool_t __hashSocketAdd(struct tagSocket *sock)
{
    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal + v4->portlocal + v4->ipremote + v4->portremote;
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;

    tmp = TcpHashTab.array[hashKey];
    if(NULL == tmp)
    {
        TcpHashTab.array[hashKey] = sock;
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
//------------------------------------------------------------------------------
//功能：移除hash表项，只是从hash表中删除，并不释放该socket。
//参数：sock，待移除的socket
//返回：true
//------------------------------------------------------------------------------
static bool_t __hashSocketRemove(struct tagSocket *sock)
{

    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal+ v4->portlocal+ v4->ipremote + v4->portremote;
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;
    tmp = TcpHashTab.array[hashKey];
    if(sock == tmp)
    {
        TcpHashTab.array[hashKey] = sock->Nextsock;
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

//----计算初始序号--------------------------------------------------------------
//功能：计算初始序号，每个tcp连接，发送的数据都始于某一个随机序号，该序号在连接时的
//     syn报文中通知对方。
//参数：无
//返回：32位序号
//------------------------------------------------------------------------------
static u32 __computeIsn(void)
{
    #define CN_TCP_ISNPRE_MS    (250)
    #define CN_ISN_PER_CONNECT  (64000)
    static u16 sIsnCnt = 0;
    u32 result;

    result = (u32)((sIsnCnt++)*CN_ISN_PER_CONNECT +
            DJY_GetSysTime()*mS*CN_TCP_ISNPRE_MS);
    return result;
}

static struct MutexLCB *pCBSync = NULL;      //use this to protect the
static struct ClientCB *pCCBFreeList = NULL; //this is used for the net_free ccb queue
static struct ServerCB *pSCBFreeList = NULL; //this is used for the net_free scb queue
//this function is used to initialize the ccb and scb module
//net_malloc the mem from the configuration and create the sync
//after the initialize, we net_malloc CB from the net_free queue
static bool_t  __initCB(s32 ccbnum, s32 scbnum)
{
    s32 i = 0;

    if((ccbnum <= 0) || (scbnum <= 0))
        return false;
    pCBSync = Lock_MutexCreate(NULL);

    //do the ccb initialize
    pCCBFreeList = net_malloc(ccbnum *sizeof(struct ClientCB));
    if(NULL == pCCBFreeList)
    {
        goto CCB_MEM;
    }

    memset(pCCBFreeList, 0, (ccbnum *sizeof(struct ClientCB)));

    //do ClientCB initialize
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
    Lock_MutexDelete(pCBSync);
    pCBSync = NULL;
    return false;
}

//net_malloc a ccb
static struct ClientCB  *mallocccb(void)
{
    struct ClientCB           *result = NULL;
    if(Lock_MutexPend(pCBSync,CN_TIMEOUT_FOREVER))
    {
        if(NULL != pCCBFreeList)
        {
            result = pCCBFreeList;
            pCCBFreeList = result->nxt;
        }

        Lock_MutexPost(pCBSync);
    }
    return result;
}
//net_free a ccb
static bool_t  freeccb(struct ClientCB  *ccb)
{
    if(Lock_MutexPend(pCBSync,CN_TIMEOUT_FOREVER))
    {
        ccb->nxt = pCCBFreeList;
        pCCBFreeList = ccb;

        Lock_MutexPost(pCBSync);
    }
    return true;
}
//net_malloc a scb
static struct ServerCB  *mallocscb(void)
{
    struct ServerCB           *result = NULL;

    if(Lock_MutexPend(pCBSync,CN_TIMEOUT_FOREVER))
    {
        if(NULL != pSCBFreeList)
        {
            result = pSCBFreeList;
            pSCBFreeList = result->nxt;
        }
        Lock_MutexPost(pCBSync);
    }
    return result;
}
//net_free a scb
static bool_t  freescb(struct ServerCB  *scb)
{
    if(Lock_MutexPend(pCBSync,CN_TIMEOUT_FOREVER))
    {
        scb->nxt = pSCBFreeList;
        pSCBFreeList = scb;

        Lock_MutexPost(pCBSync);
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
static struct ClientCB  *__CreateCCB(void)
{
    struct ClientCB *result;

    result =  mallocccb();
    if(NULL == result)
    {
        goto EXIT_CCBMEM;
    }
    memset((void *)result,0, sizeof(struct ClientCB));
    result->sbuf.bufsync = Lock_SempCreate(1,1,CN_BLOCK_FIFO,NULL);
    result->rbuf.bufsync = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
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
    result->rbuf.phead = NULL;
    result->rbuf.ptail = NULL;

    result->pkgrecomblst = NULL;
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
    Lock_SempDelete(result->rbuf.bufsync);
    Lock_SempDelete(result->sbuf.bufsync);
    freeccb(result);
    result = NULL;
EXIT_CCBMEM:
    return result;
}
//reset the ccb to the specified stat, such as the CREAT or 2FREE
static void  __ResetCCB(struct ClientCB *ccb,u16 machinestat)
{
    //init the ccb member
    //set the snd and receive buf limit to default
    Lock_SempPost(ccb->sbuf.bufsync);
    //reset the receive buf
    ccb->rbuf.buflen       = 0;
    ccb->rbuf.buflenlimit  = CN_TCP_RCVBUF_SIZEDEFAULT;
    ccb->rbuf.timeout      = CN_TIMEOUT_FOREVER;
    ccb->rbuf.trigerlevel    = 0;
    PkgTryFreeQ(ccb->rbuf.phead);
    ccb->rbuf.phead = NULL;
    ccb->rbuf.ptail = NULL;
    Lock_SempPost(ccb->rbuf.bufsync);

    //reset the recomb queue
    PkgTryFreeQ(ccb->pkgrecomblst);
    ccb->pkgrecomblst = NULL;
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
static bool_t __DeleCCB(struct ClientCB *ccb)
{
    //net_free all the pkg to snd
    net_free(ccb->sbuf.tcpsendbuf);
    //net_free all the pkg to rcv
    PkgTryFreeQ(ccb->rbuf.phead);
    //net_free all the pkg to recomb
    PkgTryFreeQ(ccb->pkgrecomblst);
    //del the semp for the buf
    Lock_SempDelete(ccb->rbuf.bufsync);
    Lock_SempDelete(ccb->sbuf.bufsync);

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
    result->acceptsemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == result->acceptsemp)
    {
        goto SCB_SYNC;
    }
    result->pendlimit     =  CN_TCP_ACCEPTMAX;
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
    Lock_SempDelete(scb->acceptsemp);
    freescb(scb);
    return true;
}
static bool_t __ReseSCB(struct ServerCB* scb)
{
    Lock_SempPost(scb->acceptsemp);
    scb->pendlimit =  CN_TCP_ACCEPTMAX;
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
    struct tagSocket    *sock = NULL,*tmp;
    u16           findtime = 0;
    if(AF_INET == family)
    {
        sock = SocketBuild();
        if(NULL != sock)
        {
            sock->ProtocolOps = &gTcpProto;
            Handle_SetMultiplexEvent(fd2Handle(sock->sockfd), 0);
//          memset(sock, 0, sizeof(struct tagSocket));
//          sock->SockSync = Lock_MutexCreate(NULL);
            if(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
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
//                        sock->ProtocolOps = &gTcpProto;
                        __hashSocketAdd(sock);
                    }
                }
                else
                {
                    //no port for you,you must do some kidding
                }
                Lock_MutexPost(TcpHashTab.tabsync);
            }//end if for the lock pend
        }//end if NULL != sock
    }//end if AF_INET == family
    return sock;
}

#pragma GCC diagnostic pop

// =============================================================================
// 功能：给socket绑定一个地址，包含端口号和IP地址
// 参数：sock，待绑定地址的socket
//      addr：被绑定的地址
//      addrlen：地址长度
// 返回：0 success while -1 failed
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
    if(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
    {
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
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
            Lock_MutexPost(sock->SockSync);
        }
        Lock_MutexPost(TcpHashTab.tabsync);
    }
    return  result;
}

//------------------------------------------------------------------------------
//功能：开启一个服务器socket，使一个端口置于监听状态，先检查该socket是否已经连接。
//参数：sock，待监听的sock，必须是未连接的socket
//      pendlimit，该服务器socket能接受的连接数量
//返回：0=成功，-1=失败
static s32 __tcplisten(struct tagSocket *sock, s32 pendlimit)
{
    s32  result;
    struct ServerCB  *scb;

    result = -1;
    if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
    {
        if((CN_SOCKET_BIND &sock->sockstat)&&\
           (0 == (CN_SOCKET_CONNECT&sock->sockstat))&&\
           (0 == (CN_SOCKET_CLOSE&sock->sockstat))&&\
           (0==(CN_SOCKET_LISTEN&sock->sockstat)))
        {
            scb = __CreateScb();
            if(NULL != scb)
            {
                scb->accepttime = ((struct ClientCB *)(sock->TplCB))->rbuf.timeout;
                __DeleCCB(sock->TplCB);
                sock->TplCB = scb;
                scb->pendlimit = pendlimit;
                sock->sockstat&=(~CN_SOCKET_CLIENT);
                sock->sockstat |=CN_SOCKET_LISTEN;

                result = 0;
            }
        }
        Lock_MutexPost(sock->SockSync);
    }
    return  result;
}

//------------------------------------------------------------------------------
//功能：从服务器sock中取一个处于稳定连接状态的客户端
//参数：sock，服务器socket
//返回：找到则返回其中一个socket，找不到则返回NULL
//------------------------------------------------------------------------------
static struct tagSocket *__acceptclient(struct tagSocket *sock)
{
    struct tagSocket    *result;
    struct tagSocket    *client;
    struct tagSocket    *pre;
    struct ClientCB       *ccb;
    struct ServerCB  *scb;

    scb = (struct ServerCB *)sock->TplCB;
    result = NULL;
    client = scb->clst;
    pre = client;
    while(NULL != client)  //find the established and unopend socket
    {
        ccb = (struct ClientCB*)client->TplCB;
        if(EN_TCP_MC_ESTABLISH== ccb->machinestat)
        {
            ccb->channelstat|=CN_TCP_CHANNEL_STATARCV|CN_TCP_CHANNEL_STATASND;
            scb->pendnum--;
            //remove it from the scb client
            if(scb->clst == client)
            {
                scb->clst = client->NextClient;
            }
            else
            {
                pre->NextClient = client->NextClient;
            }
            client->NextClient = NULL;
            result = client;
            break;
        }
        else
        {
            pre = client;
            client = client->NextClient;
        }
    }
//  if(scb->clst == NULL) {
    if(result == NULL) {
        //一般来说，服务器并不需要 read 标志，只需要accept标志即可，但有些开源软件
        //把read标志当accept标志，真正的accept标志反而不用。
        Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOACCEPT|CN_SOCKET_IOREAD);
       //printf("--info: low level remove the accept event...\r\n");
    }
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
       Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
    {
        scb = (struct ServerCB *)sock->TplCB;
        result = __acceptclient(sock);  //查看是否已经可接受
        waittime = scb->accepttime;
        if((NULL == result)&&(0 != waittime))
        {
            Lock_SempPend(scb->acceptsemp,0);   //先清理信号量
            //if none find and permit the wait
            Lock_MutexPost(sock->SockSync);
            if(Lock_SempPend(scb->acceptsemp,waittime))
            {
                if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
                {
                    result = __acceptclient(sock);
                }
            }
        }
        if(NULL== result)  //no one to accept
        {
//          Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOACCEPT);
        }
        else
        {
            result->sockstat |= CN_SOCKET_OPEN;
            result->sockstat &= ~CN_SOCKET_WAITACCEPT;
        }
        Lock_MutexPost(sock->SockSync);
    }
//应该在ack的时候添加hash表项
//    if((NULL != result)&&(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER)))
//    {
//        __hashSocketAdd(result);
//        Lock_MutexPost(TcpHashTab.tabsync);
//    }
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
    struct ClientCB      *ccb;
    u32          iplocal, ipremote;

    iplocal = sock->element.v4.iplocal;
    ipremote = sock->element.v4.ipremote;
    hdr = (struct TcpHdr *)PkgGetCurrentBuffer(pkg);
//  hdr = (struct TcpHdr *)(pkg->buf + pkg->offset);
    hdr->chksum = 0;
    //if any ack timer set, we'd better to clear the ack timer
    ccb = (struct ClientCB *)sock->TplCB;
//    char n[32];
//    DJY_GetEvttName(DJY_GetMyEvttId(),n,32);
//    if(hdr->portsrc == 0xb315)
//        printf("----------server发%4u:%4d:%7u; %s\r\n",
//                        ntohl(hdr->seqno),translen,intervaltime(),n);
//    else if(hdr->portsrc == 0x5c54)
//        printf("------------client发%4u:%4d:%7u; %s\r\n",
//                        ntohl(hdr->seqno),translen,intervaltime(),n);
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
    struct ClientCB     *ccb;
    u32         datalen;

    datalen = sizeof(struct TcpHdr)+((optionlen+3)/4)*4;
    result = PkgMalloc(datalen,pkgflag);
    if(NULL != result)
    {
        PkgSetDataLen(result, datalen);
//      result->datalen = datalen;
        ccb = (struct ClientCB*)sock->TplCB;
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
    struct ClientCB             *ccb;
    result = -1;

    //fist we make sure to adjust the its place in the hash tab
    if(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
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
        Lock_MutexPost(TcpHashTab.tabsync);
    }
    if(0 == result)
    {
        result = -1;
        //now do the handshake with the server
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
        {
            ccb = (struct ClientCB *)sock->TplCB;
            if((0 ==(CN_SOCKET_CONNECT &sock->sockstat))&&\
               (EN_TCP_MC_CREAT ==ccb->machinestat))  //it does not connect yet
            {
                __sendflag(sock,CN_TCP_FLAG_SYN,(void *)&sgSynOptDefault,\
                                        sizeof(sgSynOptDefault),ccb->sbuf.sndnxtno);
                //change the machine stat and open the timer flag
                ccb->resndtimer = (u16)ccb->rto;
                ccb->sbuf.sndnxtno++;
                ccb->machinestat = EN_TCP_MC_SYNSNT;
                ccb->timerctrl  = CN_TCP_TIMER_SYN;
                Lock_MutexPost(sock->SockSync); //release the mutex
                //等待服务器返回 syn 标志
                Lock_SempPend(ccb->rbuf.bufsync,ccb->rbuf.timeout);
                //check the connection
                Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER);
                if(ccb->machinestat == EN_TCP_MC_ESTABLISH)
                {
                    //the app could send and rceive data from now on
                    ccb->channelstat|=CN_TCP_CHANNEL_STATASND|CN_TCP_CHANNEL_STATARCV;
                    result = 0; //connection success
                }
                else if((sock->sockstat & CN_SOCKET_PROBLOCK) == 0 &&
                        (sock->sockstat & CN_SOCKET_CLIENT) &&
                        (sock->sockstat & CN_SOCKET_CLOSE)==0) {//握手设置非阻塞，
                    //result = -1; //默认是-1，这里不用设置也可以
                    sock->sockstat |= CN_SOCKET_PROCONNECT;
                }
                else
                {
                    __ResetCCB(ccb, EN_TCP_MC_2FREE); //THE NEXT STEP NEED BE CLOSE
                }
            }
            Lock_MutexPost(sock->SockSync);
        }
    }
    return  result;
}

//----copy数据------------------------------------------------------------------
//功能：把数据从用户缓冲区copy到对应socket的tcp缓冲区，类似环形缓冲区结构。如果发生
//      环绕，先copy buffer的末端，再copy卷绕部分。
//参数：sock，目标socket的指针
//      msg，待发送的数据
//      len，待拷贝的数据长度，由调用方确保不会超过 buf 剩余长度。
//------------------------------------------------------------------------------
static void __cpy2sndbuf(struct tagSocket *sock, const void *msg, s32 len)
{
    struct ClientCB           *ccb;
    u8               *src;
    u8               *dst;
    s32               cpylen;

    ccb = (struct ClientCB *)sock->TplCB;

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

    if(len > 0)     //表示数据有环绕
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

//------------------------------------------------------------------------------
//功能：确定socket中有多少数据可以发送，使用nagle算法和拥塞避免算法，确定当前有多少
//      数据可以发送。
//参数：sock，目标sock
//返回：允许发送的字节数
//------------------------------------------------------------------------------
static s32 __chkchannelsendlen(struct tagSocket *sock)
{
    s32      result = 0,congest;
    s32      datalen;
    struct ClientCB  *ccb;

    ccb =(struct ClientCB *)sock->TplCB;
    if(0 ==(ccb->channelstat & CN_TCP_CHANNEL_STATKSND))
    {
        return 0;
    }

    datalen = ccb->sbuf.datalen;
    if(CN_SOCKET_PRONAGLE&sock->sockstat) //nagle is open
    {
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)     //没有待确认的数据
        {
            result = datalen;           //有多少数据发送多少数据
        }
        else if(datalen > ccb->mss)     //若有待确认的数据，则攒够mss才发送
        {
            result = datalen;           //有多少数据发送多少数据
        }
        else
        {
            result = 0;                 //不足mss，不发送
        }
    }
    else
        result = datalen;
//  else //nagle is close
//  {
//      //we will send the mixnum of (sndwnd cwnd datalen)
//      result = ccb->cwnd;
//      if(result > ccb->sndwnd)
//      {
//          result = ccb->sndwnd;
//      }
//      if(result > datalen)
//      {
//          result = datalen;
//      }
//  }
        //we will send the mixnum of (sndwnd cwnd datalen)
    congest = ccb->cwnd;
    if(congest > ccb->sndwnd)
    {
        congest = ccb->sndwnd;
    }
    if(result > congest)
    {
        result = congest;
    }
    return result;
}

//use this function to send the new data or the data with the fin flag
static void __senddata(struct tagSocket *sock,s32 length)
{
    u16                 datalen;
    struct NetPkg          *pkghdr;
    struct NetPkg          *pkgdata;
    struct ClientCB             *ccb;
    u8                  flags;
    u8                 *data;
    bool_t              sendresult;
    u32                 translen;

    ccb = (struct ClientCB *)sock->TplCB;
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
            if(NULL != pkgdata)
            {
                //we got the data pkgnow, make sure how many data we will send
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //只有一个包
                PkgSetNextUnit(pkghdr,pkgdata);
                translen = PkgGetDataLen(pkghdr)+datalen;
                sendresult = __sendmsg(sock,pkghdr,translen);
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
//                            ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //容错计数，若网络传丢了后续的fin，4ML后将收回。
//                            ccb->mltimer = CN_TCP_TICK_2ML*2;
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
        ccb->resndtimer = (u16)ccb->rto;
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
    struct ClientCB             *ccb;
    u8                  flags;
    u8                 *data;
    u32                 translen;

    ccb = (struct ClientCB *)sock->TplCB;
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
            if(NULL != pkgdata)
            {
                //we got the data pkgnow, make sure how many data we will send
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //只有一个包
                PkgSetNextUnit(pkghdr,pkgdata);
                translen = PkgGetDataLen(pkghdr)+datalen;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
    struct ClientCB    *ccb;

    result = -1;
    if(0 == (CN_SOCKET_CLIENT &sock->sockstat))
    {
        return result;
    }
    ccb = (struct ClientCB *)sock->TplCB;
    if(Lock_SempPend(ccb->sbuf.bufsync,ccb->sbuf.timeout))
    {
        //got the mutex
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
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
                       Handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
                       Lock_SempPost(ccb->sbuf.bufsync);
                   }
                   else
                   {
                       Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
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
            else if((sock->sockstat & CN_SOCKET_PROBLOCK) == 0 &&
                    (sock->sockstat & CN_SOCKET_PROCONNECT)) {//握手设置非阻塞，
                result = -1;                //默认是-1，这里不用设置也可以
                Lock_SempPost(ccb->sbuf.bufsync);
            }
            else
            {
                result = 0;  // the channel is shutdown now
                Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOWRITE);
                Lock_SempPost(ccb->sbuf.bufsync);
            }
            Lock_MutexPost(sock->SockSync);
        }
    }
    else
    {
        //has not got the SockSync yet. maybe time out
    }
    return result;
}

//use this function to copy data from the receive buf
static s32 __cpyfromrcvbuf(struct ClientCB *ccb, void *buf, s32 len)
{
    s32  result;
    s32 cpylen;
    u8   *cpybuf;
    u8   *srcbuf;
    struct NetPkg  *pkg;

    srcbuf = (u8 *)buf;
    result = 0;

    pkg = ccb->rbuf.phead;
    while((len >0) && (NULL != pkg))
    {
        cpylen = PkgGetDataLen(pkg);
        cpylen = len > cpylen?cpylen:len;
        cpybuf = (u8 *)PkgGetCurrentBuffer(pkg);
        memcpy(srcbuf, cpybuf,cpylen);
        PkgMoveOffsetUp(pkg,cpylen);
        ccb->rbuf.buflen -= cpylen;
        result +=cpylen;
        len-= cpylen;
        srcbuf+=cpylen;
        if(0==PkgGetDataLen(pkg)) //release the no data pkg
        {
            ccb->rbuf.phead = PkgGetNextUnit(pkg);
            if(NULL == ccb->rbuf.phead)
            {
                ccb->rbuf.ptail = NULL;
            }
            PkgTryFreePart(pkg);
            pkg = ccb->rbuf.phead;
        }
        else
            break;
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
    struct ClientCB    *ccb;

    result = -1;
    if(0 == (CN_SOCKET_CLIENT &sock->sockstat))
    {
        return result;
    }
    ccb = (struct ClientCB *)sock->TplCB;
    if(Lock_SempPend(ccb->rbuf.bufsync,ccb->rbuf.timeout))
    {
        //got the mutex
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
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

//              if((ccb->rbuf.buflen > ccb->rbuf.trigerlevel)||
//                 (0 == (ccb->channelstat&CN_TCP_CHANNEL_STATKRCV)))
                if((ccb->rbuf.buflen > ccb->rbuf.trigerlevel) &&
                   (0 != (ccb->channelstat&CN_TCP_CHANNEL_STATKRCV)))
                {
                    Handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    Lock_SempPost(ccb->rbuf.bufsync);
                }
                else
                {
                    Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    ccb->channelstat &=(~CN_TCP_CHANNEL_STATCONGEST);
                    //changed the rcv window
                    //we may prevent the data in because of the limited buf, then we
                    //if we has some space, we'd better to notice the remote terminal
                    //if the data is zero, we could snd the window
                    __sendflag(sock,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
                }
            }
            else if((sock->sockstat & CN_SOCKET_PROBLOCK) == 0 &&
                    (sock->sockstat & CN_SOCKET_PROCONNECT)) //握手设置非阻塞，
            {
                //result = -1; //默认是-1，这里不用设置也可以
                Lock_SempPost(ccb->rbuf.bufsync);
            }
            else
            {
                result = 0;  // the channel receive is shutdown now
//                Handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                Lock_SempPost(ccb->rbuf.bufsync);
            }

            Lock_MutexPost(sock->SockSync);
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
    struct ClientCB *ccb;

    result = -1;
    ccb = (struct ClientCB *)sock->TplCB;

    //clear the rcv buf and recombination queue
    //and set some specified flags at the same time
    ccb->channelstat&=(~(CN_TCP_CHANNEL_STATARCV|CN_TCP_CHANNEL_STATKRCV));
    //release the rcv and recombination queue
    PkgTryFreeQ(ccb->rbuf.phead);
    ccb->rbuf.buflen = 0;
    ccb->rbuf.buflenlimit  = CN_TCP_RCVBUF_SIZEDEFAULT;
    ccb->rbuf.phead = NULL;
    ccb->rbuf.ptail = NULL;
    ccb->rbuf.trigerlevel = 0;
    Lock_SempPost(ccb->rbuf.bufsync);
    //net_free all the recomblst
    PkgTryFreeQ(ccb->pkgrecomblst);
    ccb->pkgrecomblst = NULL;
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
    struct ClientCB     *ccb;

    result = -1;
    if(sock->sockstat & CN_SOCKET_CLIENT)
    {
        ccb = (struct ClientCB *)sock->TplCB;
        ccb->channelstat&=(~(CN_TCP_CHANNEL_STATASND));  //APP NO PERMIT TO TRANSMISSION
        //if send buf still has some data to snd, then set the flag is OK
        if(ccb->sbuf.datalen == 0)
        {
            ccb->channelstat&=(~(CN_TCP_CHANNEL_STATKSND));
            __sendflag(sock,CN_TCP_FLAG_FIN|CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);
            ccb->sbuf.sndnxtno++;
            ccb->resndtimer = (u16)ccb->rto;
            ccb->timerctrl |= CN_TCP_TIMER_FIN;
            if(EN_TCP_MC_CLOSEWAIT == ccb->machinestat)
            {
                ccb->machinestat = EN_TCP_MC_LASTACK;
            }
            else
            {
                ccb->machinestat  = EN_TCP_MC_FINWAIT1;
//                ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //容错计数，若网络传丢了后续的fin，4ML后将收回。
//                ccb->mltimer = CN_TCP_TICK_2ML*2;
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
    s32    result = -1;

    if((CN_SOCKET_CLIENT&sock->sockstat)&&\
        Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
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
        Lock_MutexPost(sock->SockSync);
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
    struct ClientCB   *ccb;
    struct ServerCB   *scb;
    struct tagSocket *client;
    struct tagSocket *server;
    result = -1;

    if(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
    {
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
        {
            if(CN_SOCKET_CLIENT&sock->sockstat)  //sock client
            {
                client = sock;
                client->sockstat |= CN_SOCKET_CLOSE;
                ccb = (struct ClientCB *)client->TplCB;
                if(ccb->machinestat == EN_TCP_MC_CREAT)
                {
                    __ResetCCB(ccb, EN_TCP_MC_2FREE);
                    result = 1;
                }
                else
                {
                    __shutdownRD(client);
                    result = __shutdownWR(client);
                }
                Lock_MutexPost(client->SockSync);
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
                    scb->clst = client->NextClient;

                    ccb = (struct ClientCB *)client->TplCB;
                    __ResetCCB(ccb,EN_TCP_MC_2FREE);
                    __DeleCCB(ccb);
                    __hashSocketRemove(client);
                    SocketFree(client);
                }
                __ReseSCB(scb);
                __DeleteScb(scb);
                __hashSocketRemove(server);
                SocketFree(server);
            }
            result = 0;
        }

        Lock_MutexPost(TcpHashTab.tabsync);
    }

    return result;
}

//this function deal with SOL_SOCK
static s32 __setsockopt_sol(struct tagSocket *sock,s32 optname,const void *optval, s32 optlen)
{
    bool_t result;
    struct ClientCB *ccb;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
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
                ccb = (struct ClientCB *)sock->TplCB;
//                if(*(s32 *)optval>=0)
//                {
                    ccb->rbuf.timeout = *(s32 *)optval;
                    result = 0;
//                }
            }
            else
            {
                scb = (struct ServerCB *)sock->TplCB;
//                if(*(s32 *)optval>=0)
//                {
                    scb->accepttime = *(s32 *)optval;
                    result = 0;
//                }
            }
            break;
        case SO_SNDTIMEO:       // *optval ==0等效于非阻塞模式发送
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClientCB *)sock->TplCB;
//                if(*(s32 *)optval>=0)
//                {
                    ccb->sbuf.timeout = *(s32 *)optval;
                    result = 0;
//                }
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
                    ccb = (struct ClientCB *)sock->TplCB;
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
                    ccb = (struct ClientCB *)sock->TplCB;
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
    struct ClientCB *ccb;

    result = -1;
    switch(optname)
    {
        case TCP_MAXSEG:
            if(CN_SOCKET_CLIENT&sock->sockstat)
            {
                ccb = (struct ClientCB *)sock->TplCB;
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
    s32  result = -1;

    if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
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
        Lock_MutexPost(sock->SockSync);
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
    if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER)) {
        switch(level) {
        case SOL_SOCKET:
            if (optname == SO_ERROR) {
                *(s32*)optval = sock->errorno;
            }
            result = 0;
            break;
        default:
            break;
        }
        Lock_MutexPost(sock->SockSync);
    }

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
static void dealtcpoption(struct ClientCB *ccb, struct TcpHdr *hdr)
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

//------------------------------------------------------------------------------
//功能：接收数据，并把数据推送到上一层
//参数：client，接收数据的socket
//     seqno，数据序号
//     pkg，数据包
//返回：true or false
//------------------------------------------------------------------------------
static u32 __rcvdata(struct tagSocket *client, u32 seqno,struct NetPkg *pkg)
{
    struct ClientCB   *ccb;
    tagRecvBuf *recbuf;
    u32        pkgstart;
    u32        pkgstop;     //不含
    u32        pkglen;
    u32        rcvlen;
    u32        pkgdataoff;
    struct NetPkg *pkgcomb;
    u32 distance;
    struct NetPkg *current,*pre;

    rcvlen = 0;
    ccb = (struct ClientCB *)client->TplCB;

    pkgstart = seqno;
//  rcvlen = PkgGetDataLen(pkg);
    pkgstop = PkgGetDataLen(pkg) + seqno;
//  pkgstop = seqno + pkg->datalen;
    if(ccb->rbuf.rcvnxt == pkgstart)
    {
        //收到的包刚好就是期待的包
        PkgCachedPart(pkg);         //锁住新接收的包
        if(NULL == ccb->rbuf.phead)    //把它加入到接收链中
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //设置链表的末端节点
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if((pkgstop > pkgstart)&& ((ccb->rbuf.rcvnxt > pkgstart)
                                   &&(ccb->rbuf.rcvnxt < pkgstop)))
    {
        //tcp序号未回绕，期待接收的序号在数据包范围内，即收到的包部分已经被接收。
        //部分未被接收。
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);    //移动有效数据偏移，略过已接收部分
        PkgCachedPart(pkg);         //锁住新接收的包
        if(NULL == ccb->rbuf.phead)    //把它加入到接收链中
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //设置链表的末端节点
        rcvlen -= pkgdataoff;       //实际接收数量须减去重复部分数据
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if((pkgstop < pkgstart)&&((ccb->rbuf.rcvnxt > pkgstart)    //对应rcvnxt很大
                                    ||(ccb->rbuf.rcvnxt < pkgstop)))//对应rcvnxt很小
    {
        //注：处理过程与上一个else if 一致
        //tcp序号发生回绕，期待接收的序号在数据包范围内，即收到的包部分已经被接收。
        //无符号数减法，即使rcvnxt < pkgstart也能得出正确结果
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);
        PkgCachedPart(pkg);         //锁住新接收的包
        if(NULL == ccb->rbuf.phead)    //把它加入到接收链中
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //设置链表的末端节点
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if(pkgstop == ccb->rbuf.rcvnxt) //包已经接收过了，应该是迟到的重发包，忽略
    {

    }
    else if((ccb->rbuf.rcvnxt - pkgstart) > 0x80000000)
    {
        //收到的包整体超前于待接收的序号
#if(CFG_TCP_REORDER == true)        //支持乱序包才处理
        PkgCachedPart(pkg);
        PkgSetPrivate(pkg, seqno);
        distance  = seqno - ccb->rbuf.rcvnxt;           //新包的超前量
        current = ccb->pkgrecomblst;
        if(current != NULL)
        {
            //新包插入点在链表第一个节点前
            if(PkgGetPrivate(current) - ccb->rbuf.rcvnxt > distance)
            {
                PkgSetNextUnit(pkg,current);
                ccb->pkgrecomblst = pkg;
            }
            else
            {
                while(1)
                {
                    pre = current;
                    current = PkgGetNextUnit(current);
                    if(current != NULL)
                    {
                        if(PkgGetPrivate(current) - ccb->rbuf.rcvnxt > distance)
                        {
                            break;      //当前节点为插入点
                        }
                    }
                    else
                        break;
                }
                if((seqno == PkgGetPrivate(pre)) && (PkgGetDataLen(pkg)==PkgGetDataLen(pre)))
                {
                    PkgTryFreePart(pkg);  //丢弃重复包，产生于多次重发同一个包
                                          //注意，部分数据重叠的包不能丢弃
                }
                else  //如果是部分重叠包，在入列时不处理，出列时才处理。
                {
                    PkgSetNextUnit(pre,pkg);
                    PkgSetNextUnit(pkg,current);
                }

            }
        }
        else
        {
            PkgSetNextUnit(pkg,NULL);
            ccb->pkgrecomblst = pkg;
        }
#else
//      包被丢弃，此处无须释放，由网络驱动分配包的地方释放。
#endif      //for (CFG_TCP_REORDER == true)
    }
    else
    {
        //      包被丢弃，此处无须释放，由网络驱动分配包的地方释放。
    }

    //刚刚可能接收了一些数据到buffer，须检查乱序重组序列有无可接收的数据
#if(CFG_TCP_REORDER == true)
    pkgcomb = ccb->pkgrecomblst;
    while(NULL != pkgcomb)
    {
        pkgstart = (u32)PkgGetPrivate(pkgcomb);
        pkglen = PkgGetDataLen(pkgcomb);
        pkgstop = pkgstart + pkglen;
        recbuf = &ccb->rbuf;
        if(pkgstart == recbuf->rcvnxt)      //包头无重叠数据
        {
            //and add it to the receive queue
            if(NULL == recbuf->phead)
            {
                recbuf->phead = pkgcomb;
            }
            else
            {
                PkgSetNextUnit(recbuf->ptail,pkgcomb);
            }
            recbuf->ptail = pkgcomb;
            ccb->pkgrecomblst = PkgGetNextUnit(pkgcomb);
            PkgSetNextUnit(pkgcomb,NULL);
            pkgcomb = ccb->pkgrecomblst;
            recbuf->buflen += pkglen;
            recbuf->rcvnxt += pkglen;
            rcvlen += pkglen;
        }
        else if((pkgstart - recbuf->rcvnxt) > 0x80000000)       //有数据重叠
        {
            //数据重叠是如何产生的呢？假设序号从0开始
            //对方发包序号   本方收包序号            对方收到应答序号
            //0~999         0~999（接收并确认）      1000
            //1000~1999     未收到                             1000
            //2000~2999     2000~2999（进重组队列）  1000
            //3000~3999     3000~3999（进重组队列）  1000
            //1000~2460     1000~2460（接收并确认）  4000
            //此时，重组队列中（2000~2999）这个包将有460字节与已接收数据重叠
            if((recbuf->rcvnxt - pkgstart) >= pkglen)
            {
                ccb->pkgrecomblst = PkgGetNextUnit(pkgcomb);
                PkgSetNextUnit(pkgcomb,NULL);
                pkgcomb = ccb->pkgrecomblst;
                PkgTryFreePart(pkgcomb);  //包中全部数据已经接收
            }
            else        //数据包部分应该接收
            {
                if(NULL == recbuf->phead)
                {
                    recbuf->phead = pkgcomb;
                }
                else
                {
                    PkgSetNextUnit(ccb->rbuf.ptail,pkgcomb);
                }
                recbuf->ptail = pkgcomb;
                ccb->pkgrecomblst = PkgGetNextUnit(pkgcomb);
                PkgSetNextUnit(pkgcomb,NULL);
                pkgcomb = ccb->pkgrecomblst;
                pkglen = pkglen - (recbuf->rcvnxt - pkgstart);
                PkgMoveOffsetUp(pkgcomb,recbuf->rcvnxt - pkgstart);
                recbuf->buflen += pkglen;
                recbuf->rcvnxt += pkglen;
                rcvlen += pkglen;
            }
        }
        else    //重组队列中没有可接收包了。
        {
            break;
        }
    }
#endif      //for (CFG_TCP_REORDER == true)
    if(rcvlen > 0)
    {
        if(ccb->rbuf.buflen > ccb->rbuf.trigerlevel)
        {
            Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
            Lock_SempPost(ccb->rbuf.bufsync);
        }
        if(ccb->rbuf.buflen >= (2*ccb->rbuf.buflenlimit))
        {
            ccb->channelstat |= CN_TCP_CHANNEL_STATCONGEST;
        }
    }
//  PkgSetNextUnit(ccb->rbuf.ptail,NULL);
    return rcvlen;
}

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
    struct ClientCB    *ccb;
    s32                err;
    s32                uerr;
    u32                usedtime;

    acklen = 0;
    ccb    = (struct ClientCB *)client->TplCB;
    ackno  = ntohl(hdr->ackno);
    //首先处理发送buffer，释放空间。
    //对方没有收到新数据 且 对方有需要接收的新数据
    if((ackno  == ccb->sbuf.unackno)&&(ccb->sbuf.unacklen > 0))
    {
        ccb->sbuf.dupacktims++;
        if(ccb->sbuf.dupacktims > CN_TCP_FASTRETRANS)
        {
            //do the fast retransmition
            ccb->rto = (u16)ccb->rto<<1;
//          if(ccb->rto < CN_TCP_RTOMIN)    //rto增大，无须判断下限
//          {
//              ccb->rto = CN_TCP_RTOMIN;
//          }
//          else
            if(ccb->rto > CN_TCP_RTOMAX)
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
    //ack了部分数据但仍有未确认的
    else if(((ccb->sbuf.sndnxtno > ccb->sbuf.unackno)&&    //序号不无回绕且有未确认的数据
       ((ackno >ccb->sbuf.unackno)&&(ackno <= ccb->sbuf.sndnxtno)))||
            ((ccb->sbuf.sndnxtno < ccb->sbuf.unackno)&&    //序号不无回绕且有未确认的数据
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
            Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE);
            Lock_SempPost(ccb->sbuf.bufsync);
        }
        else
        {
            Handle_ClrMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE);
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
//功能：服务器端已经收到SYN
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __rcvsyn_ms(struct tagSocket *client, struct TcpHdr *hdr, struct NetPkg *pkg)
{
    struct ClientCB   *ccb;
    struct ServerCB   *scb;
    struct tagSocket *server;

    //ack the data
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client,hdr);
        ccb = (struct ClientCB *)client->TplCB;
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)     //ack 了所有数据
        {
            ccb->machinestat = EN_TCP_MC_ESTABLISH;
            ccb->channelstat|= CN_TCP_CHANNEL_STATKRCV| CN_TCP_CHANNEL_STATKSND;
//          ccb->sbuf.unackno = ccb->sbuf.sndnxtno;
            //close the syn resend timer
            ccb->timerctrl &= (~CN_TCP_TIMER_SYN);
            ccb->resndtimer = (u16)ccb->rto;
            //notice the server to accept
            server = ccb->server;
            scb = (struct ServerCB *)server->TplCB;
            if((Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER)))
            {
                __hashSocketAdd(client);
                Lock_MutexPost(TcpHashTab.tabsync);
            }
            //一般来说，服务器并不需要 read 标志，只需要accept标志即可，但有些开源软件
            //把read标志当accept标志，真正的accept标志反而不用。
            Handle_SetMultiplexEvent(fd2Handle(server->sockfd),CN_SOCKET_IOACCEPT|CN_SOCKET_IOREAD);
            Lock_SempPost(scb->acceptsemp);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：接收包处理，已经发送了SYN信号
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __sndsyn_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClientCB   *ccb;

    ccb = (struct ClientCB *)client->TplCB;
    //only care about the ack to the syn
    if(hdr->flags|CN_TCP_FLAG_ACK)
    {
        __ackdata(client,hdr);
        if((ccb->sbuf.unackno == ccb->sbuf.sndnxtno)&&\
           (hdr->flags == (CN_TCP_FLAG_ACK|CN_TCP_FLAG_SYN))) //ok, has ack the syn
        {
            ccb->machinestat = EN_TCP_MC_ESTABLISH;
            ccb->channelstat|= CN_TCP_CHANNEL_STATKRCV| CN_TCP_CHANNEL_STATKSND;
//            ccb->sbuf.unackno = ccb->sbuf.sndnxtno;
            //close the syn resend timer
            ccb->timerctrl &= (~CN_TCP_TIMER_SYN);
            ccb->resndtimer = (u16)ccb->rto;
            dealtcpoption(ccb,hdr);
            ccb->rbuf.rcvnxt = ntohl(hdr->seqno) + 1;
            __sendflag(client,CN_TCP_FLAG_ACK,NULL,0,ccb->sbuf.sndnxtno);

            if((client->sockstat & CN_SOCKET_PROBLOCK) == 0 &&
               (client->sockstat & CN_SOCKET_PROCONNECT)) {//握手设置非阻塞，
                ccb->channelstat|=CN_TCP_CHANNEL_STATASND|CN_TCP_CHANNEL_STATARCV;
            }
            //notice the applications  the connect success
            Lock_SempPost(ccb->rbuf.bufsync);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//功能：接收包处理，已经处于稳定连接状态
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __stable_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    u32       seqno;
    struct ClientCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClientCB *)client->TplCB;
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
        Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        Lock_SempPost(ccb->rbuf.bufsync);
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
    struct ClientCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClientCB *)client->TplCB;
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
        ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //容错计数，若网络传丢了后续的fin，4ML后将收回。
        ccb->mltimer = CN_TCP_TICK_2ML*2;
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
        Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        Lock_SempPost(ccb->rbuf.bufsync);
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
    struct ClientCB  *ccb;
    u32       rcvlen = 0;

    seqno =ntohl(hdr->seqno);
    ccb = (struct ClientCB *)client->TplCB;
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
        Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOREAD);
        Lock_SempPost(ccb->rbuf.bufsync);
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
    struct ClientCB  *ccb;

    ccb = (struct ClientCB *)client->TplCB;
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
    struct ClientCB  *ccb;
    ccb = (struct ClientCB *)client->TplCB;

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

#pragma GCC diagnostic pop

//------------------------------------------------------------------------------
//功能：被动关闭收到了对面的FIN，任何数据都会被当做重传数据。
//参数：client，客户端
//     hdr，tcp头
//     pkg，偏移已经越过tcp头的网络包
//返回：true or false
//------------------------------------------------------------------------------
static bool_t __closewait_ms(struct tagSocket *client, struct TcpHdr *hdr,struct NetPkg *pkg)
{
    struct ClientCB  *ccb;
    ccb = (struct ClientCB *)client->TplCB;
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
    struct ClientCB         *ccb;

    ccb = (struct ClientCB *)client->TplCB;
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
    struct ClientCB  *ccb;
    ccb = (struct ClientCB *)client->TplCB;
    if(ccb->timerctrl&CN_TCP_TIMER_KEEPALIVE)
    {
        ccb->keeptimer = CN_TCP_TICK_KEEPALIVE; //any receive data will reset the keep alive timer
    }
    if((hdr->flags & CN_TCP_FLAG_RST)&&(ccb->rbuf.rcvnxt == htonl(hdr->seqno)))
    {
        __ResetCCB(ccb, EN_TCP_MC_2FREE);           //general deal the reset socket
        client->errorno =  ECONNRESET;
        //Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOWRITE|CN_SOCKET_IOREAD);
        Handle_SetMultiplexEvent(fd2Handle(client->sockfd),CN_SOCKET_IOERR);
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

//------------------------------------------------------------------------------
//功能：当server收到syn时，创建一个socket，挂在tcp的server控制块的clst下，然后发送
//      syn+ack，待再次收到对方的ack时，进入已连接状态。
//参数：server，证在listen的socket
//      hdr，当前收到的tcp头
//      ipdst,portdst,ipsrc,portsrc，地址四元组
//返回：新创建的socket。
//------------------------------------------------------------------------------
static struct tagSocket* __newclient(struct tagSocket *server, struct TcpHdr *hdr,\
                          u32 ipdst,u16 portdst,u32 ipsrc, u16 portsrc)
{
    struct ServerCB *scb;
    struct ClientCB *ccb;
    struct tagSocket *result= NULL;

    scb = (struct ServerCB *)server->TplCB;
    if((0 ==(CN_SOCKET_CLOSE&server->sockstat))&&\
       (CN_SOCKET_LISTEN&server->sockstat)&&\
       (scb->pendnum <scb->pendlimit)&&\
       (CN_TCP_FLAG_SYN&hdr->flags)&&\
       (0 == (CN_TCP_FLAG_ACK&hdr->flags)))  //could accept more in the pending
    {
        result = SocketBuild();
        if(NULL != result)
        {
            result->ProtocolOps = &gTcpProto;
            Handle_SetMultiplexEvent(fd2Handle(result->sockfd), 0);
//          memset(result, 0, sizeof(struct tagSocket));
//          result->SockSync = Lock_MutexCreate(NULL);
            ccb = __CreateCCB();
            if(NULL == ccb)
            {
                SocketFree(result);
                result = NULL;
            }
            else
            {
                result->TplCB = (void *)ccb;
                result->sockstat |=CN_SOCKET_CLIENT|CN_SOCKET_PROBLOCK|CN_SOCKET_PRONAGLE|CN_SOCKET_WAITACCEPT;
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
                result->NextClient = scb->clst;
                scb->clst = result;
                scb->pendnum++;
                ccb->server = server;
                ccb->timerctrl = CN_TCP_TIMER_SYN;
                ccb->resndtimer = (u16)ccb->rto;
                ccb->machinestat = EN_TCP_MC_SYNRCV;
                ccb->sbuf.sndnxtno++;
            }
        }
    }
    else
    {
        if(scb->pendnum >= scb->pendlimit)
            printf("pendnum over\r\n");
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
            client = client->NextClient;
        }
    }
    return result;
}

static bool_t __tcprcvdealv4(u32 ipsrc, u32 ipdst,  struct NetPkg *pkg, u32 devfunc)
{

    u16    portdst;
    u16    portsrc;
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
    PkgMoveOffsetUp(pkg, (hdr->hdroff>>4)*4);
    //cpy the hdr to the thread stack avoid the align error
    memcpy((void *)cpyhdr,(void *)hdr,(hdr->hdroff>>4)*4);
    hdr = (struct TcpHdr *)cpyhdr;

    //ok,now we deal the pkg now, the pkg check success
    portdst = hdr->portdst;
    portsrc = hdr->portsrc;
    //if any client match this pkg
    Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER);
    if((sock = __hashSocketSearch(ipdst,portdst,ipsrc,portsrc)) != NULL)
    {
        //get the communicate client
        client = sock;
        Lock_MutexPend(client->SockSync,CN_TIMEOUT_FOREVER);
        __dealrecvpkg(client,hdr,pkg);
        Lock_MutexPost(client->SockSync);
    }
    else if((sock = __hashSocketSearch(INADDR_ANY,portdst,ipsrc,portsrc)) != NULL)
    {
        //the inaddr_any client matches
        client = sock;
        Lock_MutexPend(client->SockSync,CN_TIMEOUT_FOREVER);
        __dealrecvpkg(client,hdr,pkg);
        Lock_MutexPost(client->SockSync);
    }
    else if((sock = __hashSocketSearch(ipdst,portdst,0,0))!= NULL)
    {
        //the specified server matches
        server = sock;
        Lock_MutexPend(server->SockSync,CN_TIMEOUT_FOREVER);
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
        Lock_MutexPost(server->SockSync);
    }
    else if((sock = __hashSocketSearch(INADDR_ANY,portdst,0,0))!= NULL)
    {
        //the inaddr_any server matches
        server = sock;
        Lock_MutexPend(server->SockSync,CN_TIMEOUT_FOREVER);
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
        Lock_MutexPost(server->SockSync);
    }
    else   //no port matches, so just reset it
    {
        if(0==(hdr->flags & CN_TCP_FLAG_RST))
        {
            __resetremoteraw(ipdst, portdst,ipsrc, portsrc,ntohl(hdr->seqno),ntohl(hdr->ackno));
        }
    }
    Lock_MutexPost(TcpHashTab.tabsync);

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
    struct ClientCB   *ccb;

    ccb = (struct ClientCB *)client->TplCB;
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
    //resndtimer 用于 syn 、 fin 、 数据包 三者发送后的重传
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
            if ((ccb->sbuf.unacklen == 0) && (ccb->sbuf.datalen == 0))
            {
//              ccb->sbuf.unackoff = (ccb->sbuf.unackoff + ccb->sbuf.buflenlimit
//                                   - 1) % ccb->sbuf.buflenlimit;
                //下面写法可读性强，且执行效率高
                if(ccb->sbuf.unackoff == 0)
                    ccb->sbuf.unackoff = ccb->sbuf.buflenlimit - 1;
                else
                    ccb->sbuf.unackoff--;
                ccb->sbuf.unacklen = 1;
                ccb->sbuf.unackno -= 1;
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
    struct ClientCB       *ccb;
    s32 i;

    for(i = 0; i < CFG_TCP_SOCKET_HASH_LEN; i ++)
    {
        //each hash number we will lock and unlock ,so left some time for others
        Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER);
        sock = TcpHashTab.array[i];
        while(NULL != sock)
        {
            Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER);
            if(CN_SOCKET_CLIENT&sock->sockstat)//this is the client
            {
                client = sock;
                ccb = (struct ClientCB *)client->TplCB;
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
                    Lock_MutexPost(client->SockSync);
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
                    ccb = (struct ClientCB *)client->TplCB;
                    if(EN_TCP_MC_2FREE == ccb->machinestat)
                    {
                        //remove it from the queue
                        if(client == scb->clst)
                        {
                            scb->clst = client->NextClient;
                            clientpre = client->NextClient;
                        }
                        else
                        {
                            clientpre->NextClient = client->NextClient;
                        }
                        clientnxt = client->NextClient;
                        client->NextClient = NULL;
                        __ResetCCB(ccb,EN_TCP_MC_2FREE);
                        __DeleCCB(ccb);
                        __hashSocketRemove(client);
                        SocketFree(client);  //net_free the client
                        if (scb->pendnum > 0) {
                            scb->pendnum--;
                            Lock_SempPend(scb->acceptsemp, 0);
                        }
                    }
                    else if (EN_TCP_MC_CLOSEWAIT == ccb->machinestat) {
                        //send the fin to close
                        ccb->channelstat &= (~(CN_TCP_CHANNEL_STATKSND));
                        __sendflag(sock, CN_TCP_FLAG_FIN | CN_TCP_FLAG_ACK,
                                NULL, 0, ccb->sbuf.sndnxtno);
                        ccb->sbuf.sndnxtno++;
                        ccb->resndtimer = (u16)ccb->rto;
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
                Lock_MutexPost(server->SockSync);
            }
        }
        Lock_MutexPost(TcpHashTab.tabsync);
    }

    return;
}

static char *gCCBLinkStat[]=
{
    "CREAT",
    "SYNRCV",
    "SYNSNT",
    "ESTABLISH",
    "FINWAIT1",
    "FINWAIT2",
    "CLOSING",
    "TIMEWAIT",
    "CLOSEWAIT",
    "LASTACK",
    "2FREE"
};

static void __tcpdebugsockinfo(struct tagSocket *sock)
{
    printf("    iplocal :%s    portlocal :%d",\
            inet_ntoa(*(struct in_addr*)&sock->element.v4.iplocal),
            ntohs(sock->element.v4.portlocal));
    printf("    ipremote:%s    portremote:%d\r\n",\
            inet_ntoa(*(struct in_addr*)&sock->element.v4.ipremote),
            ntohs(sock->element.v4.portremote));
    printf("    sockstat:0x%08x    UserTag:0x%08x    errno   :0x%08x\n\r",\
            sock->sockstat,sock->SockUserTag,sock->errorno);
}

static void __tcpdebugccb(struct ClientCB *ccb)
{
    tagSendBuf *sbuf;
    tagRecvBuf *rbuf;
    //machine state
    printf("    machinestat:%s\n\r",gCCBLinkStat[ccb->machinestat]);
    //channel stat
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATARCV?"使能APP接收":"禁能APP接收");
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATASND?"使能APP发送":"禁能APP发送");
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATKRCV?"使能协议栈接收":"禁能协议栈接收");
    printf("    %s\r\n",ccb->channelstat&CN_TCP_CHANNEL_STATKSND?"使能协议栈发送":"禁能协议栈发送");

    //send buffer
    sbuf = &ccb->sbuf;
    printf("    发送buffer地址 0x%x，长度 %d，空闲长度 %d\r\n",
                    sbuf->tcpsendbuf,sbuf->buflenlimit, sbuf->buflenleft);
    printf("    待确认序号 %u，待发送序号 %u，待发送长度 %d\r\n",
                    sbuf->unackno, sbuf->sndnxtno, sbuf->datalen);

    //receive buffer
    rbuf = &ccb->rbuf;
    printf("    接收buffer长度 0x%x，buffered：%dbytes，欲接收序号：%u\r\n",
                rbuf->buflenlimit, rbuf->buflen, rbuf->rcvnxt);

    //windows
    printf("    mss:%04d cwnd:%d sndwndscale:%d\n\r",ccb->mss,ccb->cwnd);
    //timer
    printf("    启动的tcp定时器有：\r\n");
    if(ccb->timerctrl&CN_TCP_TIMER_SYN)
        printf("syn = %dticks; ",ccb->mltimer);
    if(ccb->timerctrl&CN_TCP_TIMER_2MSL)
        printf("2msl = %dticks; ",ccb->mltimer);
    if(ccb->timerctrl&CN_TCP_TIMER_KEEPALIVE)
        printf("keepalive = %dticks; ",ccb->keeptimer);
    if(ccb->timerctrl&CN_TCP_TIMER_PERSIST)
        printf("probe = %dticks; ",ccb->persistimer);
    if(ccb->timerctrl&CN_TCP_TIMER_CORK)
        printf("cork = %dticks; ",ccb->corktimer);
    if(ccb->timerctrl&CN_TCP_TIMER_LINGER)
        printf("linger = %dticks; ",ccb->lingertimer);
    if(ccb->timerctrl&CN_TCP_TIMER_RESEND)
        printf("resend = %dticks; ",ccb->mltimer);
    if(ccb->timerctrl&CN_TCP_TIMER_FIN)
        printf("fin = %dticks; ",ccb->mltimer);
    printf("\r\n");
    return;
}

static void __tcpdebugscb(struct ServerCB *scb)
{
    struct ClientCB     *ccb;
    struct tagSocket    *client;
    printf("    accept超时设置 :%d,pending 的 clients 数:%d ：\n\r",scb->accepttime,scb->pendnum);
    client = scb->clst;
    while(NULL != client)  //find the established and unopend socket
    {
        ccb = (struct ClientCB*)client->TplCB;
        if(EN_TCP_MC_ESTABLISH == ccb->machinestat)
        {
            printf("        client=%d,可accept，buffered：%dbytes\r\n",
                            client->sockfd, ccb->rbuf.buflen);
        }
        else
        {
            printf("        client=%d,尚未完成3次握手\r\n", client->sockfd);
        }
        client = client->NextClient;
    }


}

static void __tcpdebug(struct tagSocket *sock)
{
    struct ClientCB   *ccb;
    struct ServerCB   *scb;

    if(sock->sockstat&CN_SOCKET_CLIENT)
    {
        ccb = (struct ClientCB *)sock->TplCB;
        printf("TCPCLIENT:address = 0x%08x,fd = %d\n\r", (u32)sock, sock->sockfd);
        __tcpdebugsockinfo(sock);
        __tcpdebugccb(ccb);
    }
    else
    {
        scb = (struct ServerCB *)sock->TplCB;
        printf("TCPSERVER:address = 0x%08x,fd = %d\n\r", (u32)sock, sock->sockfd);
        __tcpdebugsockinfo(sock);
        __tcpdebugscb(scb);
    }
}

//------------------------------------------------------------------------------
//功能：tcp协议初始化
//参数：无
//返回：true or false
//------------------------------------------------------------------------------
u16 rand_port() __attribute__ ((weak));
u16 rand_port()
{
    return (u16)(rand() >> 16)%(0xffff-1024)+1024;
}
bool_t TcpInit(void)
{
    bool_t ret = false;

    //do the port random initialize
//    gPortEngineTcp = (u16)RNG_Get_RandomRange(1024,65535);
    //gPortEngineTcp = (u16)(rand() >> 16)%(0xffff-1024)+1024;
    gPortEngineTcp = rand_port();
    if(false == __hashTabInit())
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

