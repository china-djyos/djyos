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
//所属模块: 网络协议栈模块
//作者:  lst.
//版本：V1.0.0
//文件描述:接入含协议栈的网络芯片，例如simcom公司的SIM800C，
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-11-10
//   作者:  lst.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ExtStack"     //外置协议，例如使用uart+AT方式使用4G模块
//parent:"tcpip"                //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"tcp","udp"             //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_EXTSTACK == false )
//#warning  " udp  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_EXTSTACK    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@num,,,
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

#if CFG_MODULE_ENABLE_EXTSTACK

static struct TPL_ProtocalOps  gExtUdpProto;
static struct TPL_ProtocalOps  gExtTcpProto;
static struct ExtStackOps *sg_pExtStackOps;

//用于搜索跟“IP+port”四元组匹配的socket，哈希算法见 __TcpHashTabInit 函数说明
struct tagExtTcpHashTab
{
//  s32                          tablen;     //how long the hash tab is
    struct MutexLCB             *tabsync;    //used to peotect the hash tab
    struct tagSocket            *array[CFG_TCP_SOCKET_HASH_LEN];   //this is the hash tab
};
struct tagExtTcpHashTab   ExtTcpHashTab;

//each server has an ServerCB
struct ExtTcpCB
{
    s32                        backlog;             //which limit the pending num
    s32                        pendnum;             //which means how much still in pending
    u32                        sndtimeout;          //发送超时时间
    u32                        rcvtimeout;          //接收超时时间
    struct RingBuf             rbuf;                //rcv buffer
    struct tagSocket          *clst;                //该服务器accetp的所有客户端，including the pending stat
    struct SemaphoreLCB       *syncdev;             //与外部协议栈硬件同步
    u16                       machinestat;  //the machine stat of the tcb
    u16                       channelstat;  //the stat of the channel,which mean we could recv or send
};  //tcp server control block
#define CN_TCP_RCVBUF_SIZEDEFAULT  0x800    //默认 tcp 接收 buffer size

//use this function to create a scb and init it
static struct ExtTcpCB* __ExtTcpCreateCB(void)
{
    struct ExtTcpCB *result;

    result = malloc(sizeof(struct ExtTcpCB) + CN_TCP_RCVBUF_SIZEDEFAULT);
    Ring_Init(&(result->rbuf), (u8*)(result + 1), CN_TCP_RCVBUF_SIZEDEFAULT)
    if(NULL == result)
    {
        goto SCB_MEM;
    }
    result->syncdev = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(NULL == result->syncdev)
    {
        goto SCB_SYNC;
    }
    result->backlog     =  CN_TCP_LISTENDEFAULT;
    result->clst        =  NULL;
    result->pendnum     =  0;
    result->sndtimeout  =  CN_TIMEOUT_FOREVER;
    result->rcvtimeout  =  CN_TIMEOUT_FOREVER;
    return result;
SCB_SYNC:
    freescb(result);
    result = NULL;
SCB_MEM:
    return  result;
}
//use this function to delete an scb and net_free it
static bool_t __DeleteCB(struct ExtTcpCB* scb)
{
    Lock_SempDelete(scb->syncdev);
    free(scb);
    return true;
}
static bool_t __ResetCB(struct ExtTcpCB* scb)
{
    Lock_SempPost(scb->syncdev);
    scb->backlog =  CN_TCP_LISTENDEFAULT;
    scb->clst    =  NULL;
    scb->pendnum =  0;
    scb->sndtimeout  =  CN_TIMEOUT_FOREVER;
    scb->rcvtimeout  =  CN_TIMEOUT_FOREVER;
    return true;
}

//-----------------------------------------------------------------------------
//功能: 初始化tcp hash表，此表用于接收到数据包时匹配相应的socket用，hashtable所需内存
//      从系统heap中分配。hash算法：把地址四元组全部按无符号数加起来，再与tablen取模，
//      得hash值，hash值相同的 socket 用 Nextsock 指针串起来，是单向不循环链表。
//参数: len，hash表长度
//返回: true = 成功，false=失败，因内存不足
//-----------------------------------------------------------------------------
static bool_t __TcpHashTabInit( void )
{
    ExtTcpHashTab.tabsync = Lock_MutexCreate(NULL);
    if(NULL == ExtTcpHashTab.tabsync)
    {
        goto ERR_SYNC;
    }

//  ExtTcpHashTab.tablen = len;

    return true;

ERR_SYNC:
//    net_free(ExtTcpHashTab);
//    ExtTcpHashTab = NULL;
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
static struct tagSocket *__TcpHashSocketSearch(u32 iplocal, u16 portlocal,u32 ipremote,u16 portremote)
{
    struct tagSocket *result = NULL;

    struct tagSocket *tmp;
    u32 hashKey;

    hashKey = iplocal+portlocal + ipremote +portremote;
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;
    tmp = ExtTcpHashTab.array[hashKey];
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
static struct tagSocket *__TcpHashSocketLocalSearch(u32 iplocal, u16 portlocal)
{
    struct tagSocket *result = NULL;

    s32 i = 0;
    struct tagSocket *tmp;

    for(i =0; i < CFG_TCP_SOCKET_HASH_LEN;i++ )
    {
        tmp = ExtTcpHashTab.array[i];
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
static bool_t __TcpHashSocketAdd(struct tagSocket *sock)
{
    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal + v4->portlocal + v4->ipremote + v4->portremote;
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;

    tmp = ExtTcpHashTab.array[hashKey];
    if(NULL == tmp)
    {
        ExtTcpHashTab.array[hashKey] = sock;
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
static bool_t __TcpHashSocketRemove(struct tagSocket *sock)
{

    struct tagSocket  *tmp;
    u32         hashKey;
    tagSockElementV4  *v4 = &sock->element.v4;

    hashKey = v4->iplocal+ v4->portlocal+ v4->ipremote + v4->portremote;
    hashKey = hashKey%CFG_TCP_SOCKET_HASH_LEN;
    tmp = ExtTcpHashTab.array[hashKey];
    if(sock == tmp)
    {
        ExtTcpHashTab.array[hashKey] = sock->Nextsock;
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

//-----------------------------------------------------------------------------
//功能: 创建外部协议栈的 tcp 传输层套接字。
//参数: family，地址族，见socket.h的 AF_INET 等定义
//      type，socket 协议类型，见socket.h的 SOCK_DGRAM 等定义
//      protocol，具体协议，见socket.h的 IPPROTO_UDP 等定义
//返回: 网卡控制块指针
//-----------------------------------------------------------------------------
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
static struct tagSocket * __ExtTcpsocket(s32 family, s32 type, s32 protocal)
{
    struct tagSocket    *sock = NULL,*tmp;
    struct ExtTcpCB *scb;
    u16           findtime = 0;
    if(AF_INET == family)
    {
        sock = SocketBuild();
        if(NULL != sock)
        {
            sock->ProtocolOps = &gExtTcpProto;
            Handle_SetMultiplexEvent(fd2Handle(sock->sockfd), 0);
//          memset(sock, 0, sizeof(struct tagSocket));
//          sock->SockSync = Lock_MutexCreate(NULL);
            if(Lock_MutexPend(TcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
            {
                sock->af_inet = AF_INET;
                sock->element.v4.iplocal = INADDR_ANY;
                sock->element.v4.portlocal = gPortEngineTcp ++;
                sock->sockstat |=CN_SOCKET_OPEN|CN_SOCKET_CLIENT|CN_SOCKET_PROBLOCK;
                __TcpHashSocketAdd(sock);
                //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
                sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
                if(sg_pExtStackOps->ExtSocket)
                {
                    sg_pExtStackOps->ExtSocket(family,type,protocal);
                }
                scb = __ExtTcpCreateCB();
                if(NULL != scb)
                {
                    //may be we should cpy some options from the ClientCB,which set before listen
                    scb->sndtimeout  =  CN_TIMEOUT_FOREVER;
                    scb->rcvtimeout  =  CN_TIMEOUT_FOREVER;
                    sock->TplCB = scb;
                    scb->backlog = 0;
                    sock->sockstat&=(~CN_SOCKET_CLIENT);
                    sock->sockstat |=CN_SOCKET_LISTEN;
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
static s32 __ExtTcpbind(struct tagSocket *sock,struct sockaddr *addr, s32 addrlen)
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
    if(Lock_MutexPend(ExtTcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
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
                    tmp = __TcpHashSocketLocalSearch(ip,port);
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
                        __TcpHashSocketRemove(sock);
                        sock->element.v4.iplocal = sockaddrin->sin_addr.s_addr;
                        sock->element.v4.portlocal = sockaddrin->sin_port;
                        sock->sockstat |= CN_SOCKET_BIND;
                        __TcpHashSocketAdd(sock);
                        //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
                        sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
                        if(sg_pExtStackOps->ExtBind)
                        {
                            sg_pExtStackOps->ExtBind(sock->sockfd,addr,addrlen);
                        }
                        result = 0;
                    }
                }

            }
            Lock_MutexPost(sock->SockSync);
        }
        Lock_MutexPost(ExtTcpHashTab.tabsync);
    }
    return  result;
}

//------------------------------------------------------------------------------
//功能：开启一个服务器socket，使一个端口置于监听状态，先检查该socket是否已经连接。
//参数：sock，待监听的sock，必须是未连接的socket
//      backlog，该服务器socket能接受的连接数量
//返回：0=成功，-1=失败
static s32 __ExtTcplisten(struct tagSocket *sock, s32 backlog)
{
    s32  result;
    struct ExtTcpCB  *scb;

    result = -1;
    if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
    {
        if((CN_SOCKET_BIND &sock->sockstat)&&\
           (0 == (CN_SOCKET_CONNECT&sock->sockstat))&&\
           (0 == (CN_SOCKET_CLOSE&sock->sockstat))&&\
           (0==(CN_SOCKET_LISTEN&sock->sockstat)))
        {
            scb = (struct ExtTcpCB*)sock->TplCB;
            scb->backlog = backlog;
            //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
            sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
            if(sg_pExtStackOps->ExtListen)
            {
                sg_pExtStackOps->ExtListen(sock->sockfd,backlog);
            }
            result = 0;
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
static struct tagSocket *__ExtAcceptclient(struct tagSocket *sock)
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
    if(scb->clst == NULL) {
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
static struct tagSocket *__ExtTcpaccept(struct tagSocket *sock, struct sockaddr *addr, s32 *addrlen)
{
    struct tagSocket           *result;
    struct ExtTcpCB             *scb;
    struct sockaddr_in  *addrin;
    u32                  waittime;

    result = NULL;

    //first we will find if any client is ready
    if((0 ==(CN_SOCKET_CLIENT&sock->sockstat))&&\
       (sock->sockstat &CN_SOCKET_LISTEN)&&\
       Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
    {
        scb = (struct ServerCB *)sock->TplCB;
        result = __ExtAcceptclient(sock);  //查看是否已经可接受
        waittime = scb->rcvtimeout;
        if((NULL == result)&&(0 != waittime))
        {
            //if none find and permit the wait
            Lock_MutexPost(sock->SockSync);
            //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
            sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
            if(sg_pExtStackOps->ExtAccept)
            {
                sg_pExtStackOps->ExtAccept(sock->sockfd, scb->syncdev);
            }
            if(Lock_SempPend(scb->syncdev,waittime))
            {
                if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
                {
                    result = __ExtAcceptclient(sock);
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
        }
        Lock_MutexPost(sock->SockSync);
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
static s32 __ExtTcpconnect(struct tagSocket *sock, struct sockaddr *serveraddr, s32 addrlen)
{
    s32  result;
    struct sockaddr_in *addrin;
    struct SemaphoreLCB *rs;
    struct ExtTcpCB  *scb;
    result = -1;

    //fist we make sure to adjust the its place in the hash tab
    if(Lock_MutexPend(ExtTcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
    {
        if((NULL != serveraddr)&&(addrlen == sizeof(struct sockaddr_in))&&\
            (CN_SOCKET_CLIENT&sock->sockstat))
        {
            addrin = (struct sockaddr_in *)serveraddr;
            __TcpHashSocketRemove(sock);
            sock->element.v4.ipremote =addrin->sin_addr.s_addr;
            sock->element.v4.portremote = addrin->sin_port;
            sock->sockstat |= CN_SOCKET_BIND;
            __TcpHashSocketAdd(sock);
            result = 0;
        }
        Lock_MutexPost(ExtTcpHashTab.tabsync);
    }
    if(0 == result)
    {
        result = -1;
        //now do the handshake with the server
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
        {

//          ccb = (struct ClientCB *)sock->TplCB;
            if(0 ==(CN_SOCKET_CONNECT &sock->sockstat))
            {
                scb = (struct ExtTcpCB*)sock->TplCB;
                Lock_MutexPost(sock->SockSync); //release the mutex
                //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
                sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
                if(sg_pExtStackOps->ExtConnect)
                {
                    sg_pExtStackOps->ExtConnect(sock->sockfd,scb->syncdev);
                }
                //wait the SockSync signal happens
                Lock_SempPend(scb->syncdev, scb->rcvtimeout);
                //check the connection
                Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER);
                if(scb->machinestat == EN_TCP_MC_ESTABLISH)
                {
                    //the app could send and rceive data from now on
                    scb->channelstat|=CN_TCP_CHANNEL_STATASND|CN_TCP_CHANNEL_STATARCV;
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
                    __ResetCB(scb); //THE NEXT STEP NEED BE CLOSE
                }
            }
            Lock_MutexPost(sock->SockSync);
        }
    }
    return  result;
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
static s32 __ExtTcpsend(struct tagSocket *sock, const void *msg, s32 len, s32 flags)
{
    //使用外置协议栈时，网卡只能有一个，故直接去默认网卡
    sg_pExtStackOps = NetDev_GetExtStackOps(NetDev_GetDefault());
    sg_pExtStackOps->ExtSend(sock, msg, len);
}
syncdev
// =============================================================================
// FUNCTION:this function used to receive data from the socket
// PARA  IN:the parameters has the same meaning as recv
// PARA OUT:
// RETURN  :
// INSTRUCT:0 means the socket is closed or fin receive -1 means no data,others means
//          the data length copy from the socket
// =============================================================================
static s32 __ExtTcprecv(struct tagSocket *sock, void *buf,s32 len, u32 flags)
{
    s32        result;
    struct ExtTcpCB    *scb;

    result = -1;
    if(0 == (CN_SOCKET_CLIENT &sock->sockstat))
    {
        return result;
    }

    if(Lock_SempPend(scb->syncdev, ccb->rbuf.timeout))
    {
        //got the mutex
        if(Lock_MutexPend(sock->SockSync,CN_TIMEOUT_FOREVER))
        {
            scb = (struct ClientCB *)sock->TplCB;
            if(CN_TCP_CHANNEL_STATARCV & scb->channelstat)
            {
                result = Ring_Read(scb,buf,len);   //the buf already has data
                if(result == 0)
                {
                    //no data yet
                    if(0 == (scb->channelstat&CN_TCP_CHANNEL_STATKRCV))
                    {
                        //the remote is shutdown,so close the areceive
                        scb->channelstat &= (~CN_TCP_CHANNEL_STATARCV);
                    }
                    else
                    {
                        result = -1;  //not shutdown yet
                    }
                }
                if((Ring_Check(&(scb->rbuf)) != 0) &&
                   (0 != (scb->channelstat&CN_TCP_CHANNEL_STATKRCV)))
                {
                    Handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                }
                else
                {
                    Handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    scb->channelstat &=(~CN_TCP_CHANNEL_STATCONGEST);
                    //changed the rcv window
                    //we may prevent the data in because of the limited buf, then we
                    //if we has some space, we'd better to notice the remote terminal
                    //if the data is zero, we could snd the window
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

    if(Lock_MutexPend(ExtTcpHashTab.tabsync,CN_TIMEOUT_FOREVER))
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
                    __TcpHashSocketRemove(client);
                    SocketFree(client);
                }
                __ReseSCB(scb);
                __DeleteScb(scb);
                __TcpHashSocketRemove(server);
                SocketFree(server);
            }
            result = 0;
        }

        Lock_MutexPost(ExtTcpHashTab.tabsync);
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
                scb = (struct ExtTcpCB *)sock->TplCB;
//                if(*(s32 *)optval>=0)
//                {
                    scb->rcvtimeout = *(s32 *)optval;
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

//------------------------------------------------------------------------------
//功能：外置协议栈网卡收到数据时，调用本函数把数据推送到外部协议栈接口层。
//参数：
//返回：
//------------------------------------------------------------------------------
bool_t ExtStask_DevPush(s32 sockfd, u8 *data, s32 size)
{
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


bool_t ExtStackInit(void)
{
    bool_t   result = false;

    memset((void *)&gExtUdpProto,0, sizeof(gExtUdpProto));
    gExtUdpProto.__socket     = __ExtUdpsocket;
    gExtUdpProto.__bind       = __ExtUdpbind;
    gExtUdpProto.__connect    = __ExtUdpconnect;
    gExtUdpProto.__recv       = __ExtUdprecv;
    gExtUdpProto.__recvfrom   = __ExtUdprecvform;
    gExtUdpProto.__send       = __ExtUdpsend;
    gExtUdpProto.__sendto     = __ExtUdpsendto;
    gExtUdpProto.__setsockopt = __ExtUdpsetsockopt;
    gExtUdpProto.__shutdown   = __ExtUdpshutdown;
    gExtUdpProto.__close      = __ExtUdpclose;
    gExtUdpProto.__debuginfo  = __ExtUdpdebug;
    if((false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,IPPROTO_UDP, &gExtUdpProto))||\
       (false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,0, &gExtUdpProto)))
    {
        debug_printf("udp","%s:ERR:UDP PROTO REGISTER FAILED\n\r",__FUNCTION__);
        result = false;
    }
    else
    {
        result = true;
    }

    memset((void *)&gExtTcpProto,0, sizeof(gExtTcpProto));
    gExtTcpProto.__socket     = __ExtTcpsocket;
    gExtTcpProto.__bind       = __ExtTcpbind;
    gExtTcpProto.__listen     = __ExtTcplisten;
    gExtTcpProto.__accept     = __ExtTcpaccept;
    gExtTcpProto.__connect    = __ExtTcpconnect;
    gExtTcpProto.__recv       = __ExtTcprecv;
    gExtTcpProto.__recvfrom   = NULL;
    gExtTcpProto.__send       = __ExtTcpsend;
    gExtTcpProto.__sendto     = NULL;
    gExtTcpProto.__setsockopt = __ExtTcpsetsockopt;
    gExtTcpProto.__shutdown   = __ExtTcpshutdown;
    gExtTcpProto.__close      = __ExtTcpclose;
    gExtTcpProto.__debuginfo  = __ExtTcpdebug;
    if((false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,IPPROTO_UDP, &gExtTcpProto))||\
       (false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,0, &gExtTcpProto)))
    {
        debug_printf("udp","%s:ERR:UDP PROTO REGISTER FAILED\n\r",__FUNCTION__);
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}

#endif

