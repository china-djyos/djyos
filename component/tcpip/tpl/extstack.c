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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ���������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: ����Э��ջģ��
//����:  lst.
//�汾��V1.0.0
//�ļ�����:���뺬Э��ջ������оƬ������simcom��˾��SIM800C��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-11-10
//   ����:  lst.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if 0
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dbug.h"
#include "../common/tpl.h"
#include "../common/ip.h"
#include "../common/ipv4.h"
#include "../component_config_tcpip.h"
static struct TPL_ProtocalOps  gExtProto;
typedef struct UdpHdr
{
    u16 portsrc;
    u16 portdst;
    u16 msglen;
    u16 chksum;
};

#define CN_UDP_CHANNEL_STATASND    (1<<0)  //APP COULD SND DATA
#define CN_UDP_CHANNEL_STATARCV    (1<<1)  //APP COULD NOT SND DATA
#define CN_UDP_CHANNEL_STATKSND    (1<<2)  //STACK COULD SND DATA
#define CN_UDP_CHANNEL_STATKRCV    (1<<3)  //STAK COULD RCV DATA
#define CN_UDP_CHANNEL_STATCONGEST (1<<4)  //the rcv window is full
typedef struct
{
    int                       buflen;      //the data length in the buffer
    int                       buflenlimit; //the buffer length
    int                       triglevel;   //the buffer trigger level
    struct NetPkg                *ph;          //the data list head
    struct NetPkg                *pt;          //the data list tail
    struct SemaphoreLCB      *bufsync;     //if any data in the buffer, it will be activited
    u32                       timeout;     //if block, use this time to wait
}tagRBufUdp;
//each udp socket has the same controller
typedef struct UdpCB
{
    struct UdpCB           *next;              //used for the ucb control
    u32                     channelstat;       //used for the select
    int                     sndlimit;          //send the most byte a frame
    tagRBufUdp              rbuf;              //receive buffer
    s64                     framrcv;           //record the frame receive
    s64                     framsnd;           //record the frame send
    u32                     lasterr;           //record the last error
}tagUdpCB;
static tagUdpCB          *pUcbFreeLst = NULL;     //this is the udp control list
static struct MutexLCB   *pUcbFreeLstSync = NULL; //this is used for the ucb protect

//define for the udp msg pkg len
#define CN_UDP_PKGMSGLEN        1472
#define CN_UDP_BUFLENDEFAULT    0x2000      //8KB

//we use the hash tab to search add or remove an socket to the socket tab
typedef struct
{
    u32                          tablen;
    struct MutexLCB             *tabsync;
    struct Socket                   *array[0];
}tagUdpHashTab;
static tagUdpHashTab   *pUdpHashTab = NULL;
// =============================================================================
// FUNCTION��this function is used to initialize the udpv4 hash tab
// PARA  IN��len ,this parameter limites the hashtab lenth
// PARA OUT��
// RETURN  ��
// INSTRUCT:
// =============================================================================
static bool_t __hashTabInit(u32 len)
{
    bool_t result = false;

    pUdpHashTab = net_malloc(sizeof(tagUdpHashTab) + len *sizeof(struct Socket *));
    if(NULL == pUdpHashTab)
    {
        goto ERR_ARRAYMEM;
    }
    memset((void *)pUdpHashTab,0,sizeof(tagUdpHashTab) + len *sizeof(struct Socket *));

    pUdpHashTab->tabsync = mutex_init(NULL);
    if(NULL == pUdpHashTab->tabsync)
    {
        goto ERR_SYNC;
    }

    pUdpHashTab->tablen = len;

    result = true;
    return result;

ERR_SYNC:
    net_free(pUdpHashTab);
    pUdpHashTab = NULL;
ERR_ARRAYMEM:
    return result;
}

// =============================================================================
// FUNCTION��this function is used to find an socket in the hashtab
// PARA  IN��the specified ip and port determines the item together
// PARA OUT��
// RETURN  ��the socket find else NULL(no one mathes)
// INSTRUCT:send an mail to the mail box, which to be dealt by the ip engine
// =============================================================================
static struct Socket *__hashSocketSearch(u32 ip, u16 port)
{
    struct Socket *result = NULL;

    struct Socket *tmp;
    u32 hashKey;

    if(mutex_lock(pUdpHashTab->tabsync))
    {
        hashKey = port%pUdpHashTab->tablen;
        tmp = pUdpHashTab->array[hashKey];
        while((NULL != tmp))
        {
            if((ip == tmp->element.v4.iplocal)&&(port == tmp->element.v4.portlocal))
            {
                result = tmp;
                break;
            }
            else
            {
                tmp = tmp->Nextsock;
            }
        }

        mutex_unlock(pUdpHashTab->tabsync);
    }
    return result;
}
// =============================================================================
// FUNCTION:and an socket to the hash tab
// PARA  IN:ip and port are the only hash property we need
// PARA OUT��
// RETURN  :true add success while false failed
// INSTRUCT:if the same ip and port has existed, then will fail
// =============================================================================
static bool_t __hashSocketAdd(struct Socket *sock)
{
    bool_t      result;
    struct Socket  *tmp;
    u32         hashKey;
    u32         ip;
    u16         port;

    result = false;
    ip = sock->element.v4.iplocal;
    port = sock->element.v4.portlocal;
    hashKey = port%pUdpHashTab->tablen;
    if(mutex_lock(pUdpHashTab->tabsync))
    {
        tmp = pUdpHashTab->array[hashKey];
        while(NULL != tmp)
        {
            if((ip == tmp->element.v4.iplocal)&&(port == tmp->element.v4.portlocal))
            {
                break;
            }
            else
            {
                tmp = tmp->Nextsock;
            }
        }
        if(NULL == tmp)
        {
            sock->Nextsock = pUdpHashTab->array[hashKey];
            pUdpHashTab->array[hashKey] = sock;
            result = true;
        }
        mutex_unlock(pUdpHashTab->tabsync);
    }
    return result;
}

// =============================================================================
// FUNCTION:this function is used to net_free the item created
// PARA  IN:ip and port are the only hash property we need
// PARA OUT��
// RETURN  :the hash item we create
// INSTRUCT:add the create item to the last
// =============================================================================
static bool_t __hashSocketRemove(struct Socket *sock)
{

    struct Socket  *tmp;
    u32         hashKey;

    if(mutex_lock(pUdpHashTab->tabsync))
    {
        hashKey = sock->element.v4.portlocal%pUdpHashTab->tablen;
        tmp = pUdpHashTab->array[hashKey];
        if(sock == tmp)
        {
            pUdpHashTab->array[hashKey] = sock->Nextsock;
            sock->Nextsock = NULL;
        }
        else
        {
            while(NULL != tmp)
            {
                if(tmp->Nextsock == sock)
                {
                    tmp->Nextsock = sock->Nextsock;
                    sock->Nextsock = NULL;
                    break;
                }
                else
                {
                    tmp = tmp->Nextsock;
                }
            }
        }

        mutex_unlock(pUdpHashTab->tabsync);
    }
    return true;
}
//used for manage the ucb mem
// =============================================================================
// FUNCTION��this function is used to do the udp net_free list initializing
// PARA  IN��len ,this parameter limit how many ucb you could create
// PARA OUT��
// RETURN  ��
// INSTRUCT:
// =============================================================================
static  bool_t  __UdpCbInit(int len)
{
    bool_t result = false;
    int i = 0;

    pUcbFreeLstSync = mutex_init(NULL);
    if(NULL == pUcbFreeLstSync)
    {
        return result;
    }

    pUcbFreeLst = net_malloc(len *sizeof(tagUdpCB));
    if(NULL != pUcbFreeLst)
    {
        memset((void *)pUcbFreeLst,0,sizeof(tagUdpCB)*len);
        if(len > 2)
        {
            for(i = 0;i < len -2;i++)
            {
                pUcbFreeLst[i].next = &pUcbFreeLst[i +1];
            }
        }
        else if(len == 2)
        {
            pUcbFreeLst[0].next = &pUcbFreeLst[1];
        }
        else
        {
            //do nothing if only one
        }

        result = true;
    }
    return result;
}
// =============================================================================
// FUNCTION��this function is used to net_free an ucb
// PARA  IN��
// PARA OUT��
// RETURN  ��the ucb you net_malloc else NULL
// INSTRUCT:
// =============================================================================
static  bool_t  __UdpCbFree(tagUdpCB  *cb)
{
    if(NULL != cb)
    {
        PkgTryFreeQ(cb->rbuf.ph);
        semp_del(cb->rbuf.bufsync);
        if(mutex_lock(pUcbFreeLstSync))
        {
            memset((void *)cb,0,sizeof(tagUdpCB));
            cb->next =pUcbFreeLst;
            pUcbFreeLst = cb;
            mutex_unlock(pUcbFreeLstSync);
        }
    }
    return true;
}

// =============================================================================
// FUNCTION��this function is used to net_malloc an ucb
// PARA  IN��
// PARA OUT��
// RETURN  ��the ucb you net_malloc else NULL
// INSTRUCT:
// =============================================================================
static  tagUdpCB * __UdpCbMalloc(void )
{
    tagUdpCB           *result = NULL;
    if(mutex_lock(pUcbFreeLstSync))
    {
        result = pUcbFreeLst;
        if(NULL != result)
        {
            pUcbFreeLst = result->next;
        }
        mutex_unlock(pUcbFreeLstSync);
    }

    if(NULL != result)
    {
        //init the ucb member
        memset((void *)result, 0, sizeof(tagUdpCB));
        result->rbuf.bufsync = semp_init(1,0,NULL);
        if(NULL == result->rbuf.bufsync)
        {
            __UdpCbFree(result);
        }
        else
        {
            //create two semphore for the rcv and sndbuf
            result->sndlimit          = CN_UDP_PKGMSGLEN;
            result->rbuf.buflenlimit  = CN_UDP_BUFLENDEFAULT;
            result->rbuf.timeout      = CN_TIMEOUT_FOREVER;
            result->channelstat       = CN_UDP_CHANNEL_STATASND|CN_UDP_CHANNEL_STATARCV|\
                                     CN_UDP_CHANNEL_STATKSND|CN_UDP_CHANNEL_STATKRCV;
        }
    }
    return result;
}
// =============================================================================
// FUNCTION��this function is used to generate an socket
// PARA  IN��family:such as AF_INET AF_INET6  AF_LOCAL
// PARA OUT��the socket generated
// RETURN  ��
// INSTRUCT:
// =============================================================================
static u16        gPortEngineUdp = 0x400;
static struct Socket * __udpsocket(int family, int type, int protocal)
{
    struct Socket  *sock;
    struct Socket  *tmp;
    u32         hashKey;
    u16         port;
    u32         ip;
    bool_t      getport;
    tagUdpCB   *ucb;

    sock = NULL;
    if(AF_INET == family)
    {
        if(mutex_lock(pUdpHashTab->tabsync))
        {
            getport = false;
            while(getport == false)
            {
                port = gPortEngineUdp;
                ip = INADDR_ANY;
                hashKey = port%pUdpHashTab->tablen;
                tmp = pUdpHashTab->array[hashKey];
                while(NULL != tmp)
                {
                    if((ip == tmp->element.v4.iplocal)&&(port == tmp->element.v4.portlocal))
                    {
                        break;
                    }
                    else
                    {
                        tmp = tmp->Nextsock;
                    }
                }
                if(NULL == tmp)
                {
                    getport = true;
                }
                gPortEngineUdp++;
            }
            //now we got an port, so we should net_malloc an socket and do the initialize
            sock = SocketBuild();
            ucb = __UdpCbMalloc();
            if((NULL != sock)&&(NULL != ucb))
            {
                sock->ProtocolOps = gTcpProto;
                handle_SetMultiplexEvent(fd2Handle(sock->sockfd), 0);
//                memset(sock, 0, sizeof(struct Socket));
                //ok, now we got an port here
                sock->af_inet = AF_INET;
                sock->element.v4.iplocal = INADDR_ANY;
                sock->element.v4.portlocal = port;
                //initialize the sock add the sock to the hash tab
                sock->sockstat |=CN_SOCKET_PROBLOCK;
                sock->ProtocolOps = &gExtProto;
                sock->IoInitstat = CN_SOCKET_IOWRITE;
                sock->TplCB = ucb;
                //add it to the socket queue
                sock->Nextsock = pUdpHashTab->array[hashKey];
                pUdpHashTab->array[hashKey] = sock;
            }
            else
            {
                if(NULL != ucb)
                {
                    __UdpCbFree(ucb);
                }
                if(NULL != sock)
                {
                    SocketFree(sock);
                    sock = NULL;
                }
            }
            mutex_unlock(pUdpHashTab->tabsync);
        }
    }//end if AF_INET == family
    return sock;
}
// =============================================================================
// FUNCTION��this function is used to bind an sock to the specified ip and port
// PARA  IN��
// PARA OUT��
// RETURN  ��0 success while -1 failed
// INSTRUCT:
// =============================================================================
static int __udpbind(struct Socket *sock,struct sockaddr *addr, int addrlen)
{
    int  result = -1;
    struct sockaddr_in *sockaddrin;
    struct Socket    *tmp;

    if((NULL != addr)&&(AF_INET == addr->sa_family)&&((NULL != sock)))
    {
        if((0 == (sock->sockstat&CN_SOCKET_BIND))&&\
           (0 == (CN_SOCKET_CLOSE&sock->sockstat)))  //NOT BIND YET
        {
            sockaddrin = (struct sockaddr_in *)addr;
            tmp = __hashSocketSearch(sockaddrin->sin_addr.s_addr,sockaddrin->sin_port);
            if(tmp == sock) //same socket, do noting
            {
                sock->element.v4.iplocal = sockaddrin->sin_addr.s_addr;
                sock->element.v4.portlocal = sockaddrin->sin_port;
                sock->sockstat |= CN_SOCKET_BIND;
                result = 0;
            }
            else
            {
                //remove it and reinsert
                __hashSocketRemove(sock);
                sock->element.v4.iplocal = sockaddrin->sin_addr.s_addr;
                sock->element.v4.portlocal = sockaddrin->sin_port;
                sock->sockstat |= CN_SOCKET_BIND;
                if( __hashSocketAdd(sock))
                {
                    result = 0;
                }
            }
        }
    }//end if AF_INET == family

    return result;
}

// =============================================================================
// FUNCTION��this function is used to snd the meassage to the ip layer
// PARA  IN��
// PARA OUT��
// RETURN  ��0 success while -1 failed
// INSTRUCT:the ipv6 udp not implement yet
// =============================================================================
static int __msgsnd(struct Socket *sock, const void *msg, int len, int flags,\
                  const struct sockaddr *addr, int addrlen)
{
    int             result;
    u16             translen;
    u32             iplocal, ipremote;
    u16             portlocal, portremote;
    struct UdpHdr       *hdr;
    struct NetPkg       *pkg,*pkgdata;
    tagUdpCB        *ucb;
    struct sockaddr_in *addrin;

    result = -1;
    ucb =(tagUdpCB *)sock->TplCB;
    if(ucb->sndlimit >=len)
    {
        pkg = PkgMalloc(sizeof(struct UdpHdr),0);
        if(NULL != pkg)
        {
            pkgdata = PkgMalloc(0,CN_PKLGLST_END);
//          pkgdata = PkgMalloc(sizeof(struct NetPkg),CN_PKLGLST_END);
            if(NULL != pkgdata)
            {
                //compute the ip and port
                iplocal = sock->element.v4.iplocal;
                portlocal =  sock->element.v4.portlocal;

                addrin = (struct sockaddr_in*)addr;
                if(NULL != addrin)
                {
                    ipremote = addrin->sin_addr.s_addr;
                    portremote = addrin->sin_port;
                }
                else
                {
                    ipremote = sock->element.v4.ipremote;
                    portremote = sock->element.v4.portremote;
                }
                translen = len + sizeof(struct UdpHdr);
                //do the udp header
                hdr = (struct UdpHdr *)PkgGetCurrentBuffer(pkg);
//              hdr = (struct UdpHdr *)(pkg->buf + pkg->offset);
                hdr->portdst = portremote;
                hdr->portsrc = portlocal;
                hdr->msglen = htons(translen);
                hdr->chksum = 0;
                PkgSetDataLen(pkg,sizeof(struct UdpHdr));
//              pkg->datalen = sizeof(struct UdpHdr);

                PkgSetNextUnit(pkg,pkgdata);
//              pkg->partnext = pkgdata;
                PkgInit(pkgdata,CN_PKLGLST_END,0,len,msg);
//              pkgdata->partnext = NULL;
//              pkgdata->offset = 0;
//              pkgdata->buf = (u8 *)msg;
//              pkgdata->datalen = len;
                //ok, the msg has cpy to the pkg, then snd the pkg
                if(IpSend(EN_IPV_4,(ptu32_t)iplocal, (ptu32_t)ipremote, pkg,translen,IPPROTO_UDP,\
                                    CN_IPDEV_UDPOCHKSUM,&hdr->chksum))
                {
                    result = len;
                }
                PkgTryFreePart(pkg);
                PkgTryFreePart(pkgdata);
                TCPIP_DEBUG_INC(ucb->framsnd);
            }
            else
            {
                PkgTryFreePart(pkg);
            }
        }
    }
    return result;
}

// =============================================================================
// FUNCTION:use this function for the connect,
// PARA  IN:sock,the socket
//          addr,the net addr
//          addrlen,the netaddr len
// PARA OUT:
// RETURN  :0 success while -1 failed
// INSTRUCT:
// =============================================================================
static int __udpconnect(struct Socket *sock, struct sockaddr *serveraddr, int addrlen)
{
    int  result=-1;
    struct sockaddr_in *addrin;

    addrin = (struct sockaddr_in *)serveraddr;
    if((NULL == addrin)||(addrlen != sizeof(struct sockaddr))||\
        (addrin->sin_family!= AF_INET))
    {
        return result;  //if para error
    }
    if(mutex_lock(sock->SockSync))
    {
        if(0 == (sock->sockstat & CN_SOCKET_CONNECT)) //make sure not connect yet
        {
            sock->element.v4.ipremote = addrin->sin_addr.s_addr;
            sock->element.v4.portremote = addrin->sin_port;
            sock->sockstat |= CN_SOCKET_CONNECT;
            result = 0;
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}
// =============================================================================
// ��������:__udpsend
//         ���׽��ַ�������
// �������:sock,Ŀ���׽���
//        msg,�����͵���Ϣ
//        len,��Ϣ����
//        flags,һ��Ϊ0
// ���������
// ����ֵ  ��-1���������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
static int __udpsend(struct Socket *sock, const void *msg, int len, int flags)
{
    int  result= -1;
    tagUdpCB  *ucb;

    if(mutex_lock(sock->SockSync))
    {
        ucb = (tagUdpCB *)sock->TplCB;
        if(CN_UDP_CHANNEL_STATASND&ucb->channelstat)
        {
            result = __msgsnd(sock,msg,len,flags,NULL,0);
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}
// =============================================================================
// �������ܣ�  __cpyfromrbuf
// ���������
// ���������
// ����ֵ  ��-1���������򷵻ؿ������ֽ���
// ˵��    ��must remember that each time only one frame
// =============================================================================
static int __cpyfromrbuf(struct Socket *sock, void *buf, int len,\
                     struct sockaddr *addr, int *addrlen)
{
    int cpylen;
    u8   *srcbuf;
    struct NetPkg  *pkg;
    tagUdpCB *ucb;

    ucb = (tagUdpCB *)sock->TplCB;
    srcbuf = (u8 *)buf;
    //���ܻᳬ���أ������ⲻ�ǹؼ�
    //�ȿ�������û��û�����������ݰ����������ǵ�����
    pkg = ucb->rbuf.ph;
    cpylen = len > PkgGetDataLen(pkg) ? PkgGetDataLen(pkg) : len;
//  cpylen = len > pkg->datalen?pkg->datalen:len;
    srcbuf = PkgGetBuffer(pkg);
//  srcbuf = (pkg->buf);
    if((NULL != addr)&&(NULL != addrlen))   //cpy the address
    {
        memcpy((void *)addr,srcbuf, sizeof(struct sockaddr));
        *addrlen = sizeof(struct sockaddr);
    }
    srcbuf += PkgGetOffset(pkg);
//  srcbuf += pkg->offset;
    memcpy(buf, srcbuf,cpylen);
    //udp�ǰ�����֡�ĸ�ʽ�����Լ�ʹû�н�����ϣ�����Ҳ������ʣ�������
    ucb->rbuf.buflen -= PkgGetDataLen(pkg);
//  ucb->rbuf.buflen-=pkg->datalen;
    ucb->rbuf.ph = PkgGetNextUnit(pkg);
    if(NULL == ucb->rbuf.ph)
    {
        ucb->rbuf.pt = NULL;
    }
    PkgTryFreePart(pkg);
    return cpylen;
}
// =============================================================================
// �������ܣ�  __udprecv
//        ��ȡ��������
// ���������  sock,Ŀ���׽���
//        flags,һ��Ϊ0
// ���������
//       buf,�յ�����Ϣ
//       len,buf����
// ����ֵ  ��-1���������򷵻��յ��ֽ���
// ˵��    ��
// =============================================================================
static int __udprecv(struct Socket *sock, void *buf,int len, unsigned int flags)
{
    int  result = -1;
    tagUdpCB *ucb;
    u32  timeout;
    ucb = (tagUdpCB *)sock->TplCB;

    timeout = ucb->rbuf.timeout;
    if(semp_pendtimeout(ucb->rbuf.bufsync,timeout))
    {
        if(mutex_lock(sock->SockSync))
        {
            if(ucb->channelstat & CN_UDP_CHANNEL_STATARCV)
            {
                if(NULL != ucb->rbuf.ph)
                {
                    result = __cpyfromrbuf(sock,buf,len,NULL, NULL);
                }
                else
                {
                    result = -1;
                }
            }
            else
            {
                result = 0;
            }

            //if any data or could receive again
            if((ucb->rbuf.buflen > ucb->rbuf.triglevel)||(0 == (ucb->channelstat & CN_UDP_CHANNEL_STATARCV)))
            {
                handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                semp_post(ucb->rbuf.bufsync);
            }
            else
            {
                handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
            }
            mutex_unlock(sock->SockSync);
        }
    }
    else
    {
        result = -1;//maybe timeout ;need more try
    }
    return  result;
}


// =============================================================================
// �������ܣ�  __udpsendto
//        ��Ŀ�������ַ��������
// ���������  sock,     Ŀ���׽���
//        msg,        �����͵���Ϣ
//        len,        ��Ϣ����
//        flags,      һ��Ϊ0
//        addr,       Ŀ�������ַ
//        addrlen,    Ŀ�������ַ����
// ���������
// ����ֵ  ��-1���������򷵻ط����ֽ���
// ˵��    ��
// =============================================================================
static int __udpsendto(struct Socket *sock, const void *msg,int len, unsigned int flags,\
              const struct sockaddr *addr, int addrlen)
{
    int  result= -1;
    tagUdpCB   *ucb;

    if(mutex_lock(sock->SockSync))
    {
        ucb = (tagUdpCB *)sock->TplCB;
        if(CN_UDP_CHANNEL_STATASND&ucb->channelstat)
        {
            result = __msgsnd(sock,msg,len,flags,addr,addrlen);
        }
        mutex_unlock(sock->SockSync);
    }
    return  result;
}
// =============================================================================
// �������ܣ� __udprecvform
//           ��Ŀ�������ַ��ȡ����
// ���������  sock,Ŀ���׽���
//           flags,һ��Ϊ0
//           addr,Ŀ�������ַ
//           addrlen,Ŀ�������ַ����
// ���������
//           buf,��ȡ������
//           len,��Ϣ����
// ����ֵ  ��-1���������򷵻��յ��ֽ���
// ˵��    ��
// =============================================================================
static int __udprecvform(struct Socket *sock,void *buf, int len, unsigned int flags,\
            struct sockaddr *addr, int *addrlen)
{
    int  result = -1;
    tagUdpCB *ucb;
    u32  timeout;

    ucb = (tagUdpCB *)sock->TplCB;
    timeout = ucb->rbuf.timeout;
    if(semp_pendtimeout(ucb->rbuf.bufsync,timeout))
    {
        if(mutex_lock(sock->SockSync))
        {
            if(ucb->channelstat & CN_UDP_CHANNEL_STATARCV)
            {
                if(NULL != ucb->rbuf.ph)
                {
                    result = __cpyfromrbuf(sock,buf,len,addr, addrlen);
                }
                else
                {
                    result = -1;
                }
            }
            else
            {
                result = 0;
            }

            //if any data or could receive again
            if((ucb->rbuf.buflen > ucb->rbuf.triglevel)||(0 == (ucb->channelstat & CN_UDP_CHANNEL_STATARCV)))
            {
                handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                semp_post(ucb->rbuf.bufsync);
            }
            else
            {
                handle_ClrMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
            }
            mutex_unlock(sock->SockSync);
        }
    }
    else
    {
        result = -1;//maybe timeout ;need more try
    }
    return  result;
}
// =============================================================================
// �������ܣ�  __shutdown_rd
//           shutdown the socked  read channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
static int __shutdown_rd(struct Socket *sock)
{
    int result;
    tagUdpCB *ucb;

    result = -1;
    ucb = (tagUdpCB *)sock->TplCB;
    if(CN_UDP_CHANNEL_STATARCV& ucb->channelstat)
    {
        //clear the rcv buf and recombination queue
        //and set some specified flags at the same time
        ucb->channelstat&=(~(CN_UDP_CHANNEL_STATARCV|CN_UDP_CHANNEL_STATKRCV));
        //release the rcv and recombination queue
        PkgTryFreeQ(ucb->rbuf.ph);
        ucb->rbuf.buflen = 0;
        ucb->rbuf.buflenlimit  = CN_UDP_BUFLENDEFAULT;
        ucb->rbuf.ph = NULL;
        ucb->rbuf.pt = NULL;
        ucb->rbuf.triglevel = 0;

        result = 0;
    }
    return result;
}
// =============================================================================
// �������ܣ�  __shutdown_wr
//        shutdown the socked  snd channel
// ���������  socket, the socket to be shutdown
// ���������
// ����ֵ  ��0 success while -1 failed
// ˵��    ��
// =============================================================================
static int __shutdown_wr(struct Socket *sock)
{
    int result;
    tagUdpCB     *ucb;

    result = -1;
    ucb = (tagUdpCB *)sock->TplCB;
    if(CN_UDP_CHANNEL_STATASND&ucb->channelstat)
    {
        ucb->channelstat&=(~(CN_UDP_CHANNEL_STATKSND|CN_UDP_CHANNEL_STATASND));  //APP NO PERMIT TO TRANSMISSION
        result = 0;
    }
    return result;
}
// =============================================================================
// �������ܣ�  __udpshutdown
//        �ر��׽���
// ���������  sock,Ŀ���׽���
//        how,�رշ����� ��SHUT_RD�ȶ���
// ���������
// ����ֵ  ��0 �ɹ�  -1ʧ��
// ˵��    ��
// =============================================================================
static int __udpshutdown(struct Socket *sock, u32 how)
{
    int    result=-1;

    if(mutex_lock(sock->SockSync))
    {
        switch(how)
        {
            case SHUT_RD:
                result = __shutdown_rd(sock);
                break;
            case SHUT_WR:
                result = __shutdown_wr(sock);
                break;
            case SHUT_RDWR:
                result = __shutdown_rd(sock);
                if(0 ==result)
                {
                    result = __shutdown_wr(sock);
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
// �������ܣ�  __udpclose
// ���������  sock,���رյ��׽���
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
static int __udpclose(struct Socket *sock)
{
    int result = -1;

    __hashSocketRemove(sock);
    if(mutex_lock(sock->SockSync))
    {
        __UdpCbFree(sock->TplCB);
        sock->TplCB = NULL;
        SocketFree(sock);
        result = 0;
    }
    return result;
}
// =============================================================================
// �������ܣ�  __sol_socket
//           SOL_SOCKETѡ���
// ���������  sock,���������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
static int __sol_socket(struct Socket *sock,int optname,const void *optval, int optlen)
{
    bool_t result = -1;
    tagUdpCB *ucb;

    ucb = (tagUdpCB *)sock->TplCB;
    switch(optname)
    {
        case SO_BROADCAST:
            if(*(int *)optval)
            {
                sock->sockstat |=CN_SOCKET_PROBROAD;
            }
            else
            {
                sock->sockstat &=(~CN_SOCKET_PROBROAD);
            }
            result = 0;
            break;
        case SO_DEBUG:
            break;
        case SO_DONTROUTE:
            break;
        case SO_ERROR:
            break;
        case SO_KEEPALIVE:
            break;
        case SO_LINGER:
            break;
        case SO_OOBINLINE:
            break;
        case SO_RCVBUF:
            if(*(int *)optval > 0)
            {
                ucb->rbuf.buflenlimit =*(int *)optval;
                result = 0;
            }
            break;
        case SO_SNDBUF:
            if(*(int *)optval > 0)
            {
                ucb->sndlimit = *(int *)optval;
                result = 0;
            }
            break;
        case SO_RCVLOWAT:
            if(*(int *)optval >= 0)
            {
                ucb->rbuf.triglevel =*(int *)optval;
                result = 0;
            }
            break;
        case SO_SNDLOWAT:
            break;
        case SO_RCVTIMEO:
            sock->sockstat |=CN_SOCKET_PROBLOCK;
            ucb->rbuf.timeout = *(u32 *)optval;
            result = 0;
            break;
        case SO_SNDTIMEO:
            break;
        case SO_REUSEADDR:
            break;
        case SO_TYPE:
            break;
        case SO_BSDCOMPAT:
            break;
        case SO_NOBLOCK:
            if(*(int *)optval)
            {
                sock->sockstat &=(~CN_SOCKET_PROBLOCK);
                ucb = (tagUdpCB *)sock->TplCB;
                ucb->rbuf.timeout = 0;
            }
            else
            {
                sock->sockstat |=CN_SOCKET_PROBLOCK;
                ucb = (tagUdpCB *)sock->TplCB;
                ucb->rbuf.timeout = CN_TIMEOUT_FOREVER;
            }
            result = 0;
            break;
        default:
            break;
    }
    return result;
}
// =============================================================================
// �������ܣ�  __ipproto_ip
//           IPPROTO_IPѡ���
// ���������         sock,���������׽���
//           optname,ѡ������
//           optval,�����ò���
//           optlen,��������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
static int __ipproto_ip(struct Socket *sock,int optname,const void *optval, int optlen)
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
// =============================================================================
// �������ܣ�  __udpsetsockopt
//           �����׽���ѡ��
// ���������  sock,Ŀ���׽���
//           level,���õĲ�Σ�֧��SOL_SOCKET��IPPROTO_TCP��IPPROTO_IP��IPPROTO_IPV6
//           optname,������ѡ��
//           optval,������
//           optlen,����������
// ���������
// ����ֵ  ��0 �ɹ� -1ʧ��
// ˵��    ��
// =============================================================================
static int __udpsetsockopt(struct Socket *sock, int level, int optname,\
               const void *optval, int optlen)
{
    int  result;

    result = -1;

    if(mutex_lock(sock->SockSync))
    {
        switch(level)
        {
            case SOL_SOCKET:
                result = __sol_socket(sock,optname,optval,optlen);
                break;
            case IPPROTO_IP:
                result = __ipproto_ip(sock,optname,optval,optlen);
                break;
            case IPPROTO_TCP:
                break;
            default:
                break;
        }
        mutex_unlock(sock->SockSync);
    }

    return  result;
}

static void __addpkg2rbuf(tagUdpCB *ucb, struct NetPkg *pkg)
{
    if(NULL == ucb->rbuf.pt)
    {
        ucb->rbuf.pt = pkg;
        ucb->rbuf.ph = pkg;
    }
    else
    {
        PkgSetNextUnit(ucb->rbuf.pt,pkg);
//      ucb->rbuf.pt->partnext = pkg;
        ucb->rbuf.pt = pkg;
    }
    ucb->rbuf.buflen += PkgGetDataLen(pkg);
//  ucb->rbuf.buflen+= pkg->datalen;

    PkgCachedPart(pkg);
    TCPIP_DEBUG_INC(ucb->framrcv);
    return;
}

// =============================================================================
// FUNCTION:this function is used to do the udp rcv
// PARA  IN:
// PARA OUT��
// RETURN  :
// INSTRUCT:
// =============================================================================
static bool_t __rcvdealv4(u32 ipsrc, u32 ipdst, struct NetPkg *pkg, u32 devfunc)
{
    bool_t              result ;
    u16                 portdst;
    u16                 portsrc;
    struct UdpHdr          *hdr;
    tagUdpCB           *ucb;
    struct Socket          *sock;
    struct  sockaddr_in addrin;

    result  = true;
    if((NULL == pkg)||(PkgGetDataLen(pkg) <= sizeof(struct UdpHdr)))
//  if((NULL == pkg)||(pkg->datalen <= sizeof(struct UdpHdr)))
    {
        goto EXIT_RCVEND;  //para error
    }
    hdr = (struct UdpHdr *)PkgGetCurrentBuffer(pkg);
//  hdr = (struct UdpHdr *)(pkg->buf + pkg->offset);
    if((0 != hdr->chksum)&&(0 == (devfunc&CN_IPDEV_UDPICHKSUM))) //if need chksum
    {
        IpPseudoPkgLstChkSumV4(ipdst, ipsrc, IPPROTO_UDP,\
                       pkg,htons(hdr->msglen),&hdr->chksum);
        if(0 != hdr->chksum)
        {
            goto EXIT_RCVEND;
        }
    }
    PkgMoveOffsetUp(pkg, sizeof(struct UdpHdr));
//  pkg->datalen -= sizeof(struct UdpHdr);
//  pkg->offset += sizeof(struct UdpHdr);
    if(0 == PkgGetDataLen(pkg))
//  if(0 == pkg->datalen)
    {
        goto EXIT_RCVEND;
    }
    //ok,now we deal the pkg now, the pkg check success
    portdst = hdr->portdst;
    portsrc = hdr->portsrc;

    //find the socket
    sock = __hashSocketSearch(ipdst,portdst);
    if(NULL == sock)
    {
        sock =__hashSocketSearch(INADDR_ANY,portdst); //if INADDR_ANY match
    }

    if((NULL != sock)&&mutex_lock(sock->SockSync))
    {
        ucb = (tagUdpCB *)sock->TplCB;
        result = true;

        if((ipdst == INADDR_BROAD)&&(0 == (CN_SOCKET_PROBROAD &sock->sockstat)))
        {
            result = false;    //could not receive broad info
        }

        if(CN_UDP_CHANNEL_STATCONGEST&ucb->channelstat)
        {
            result = false;  //no mem for the info
        }
        if(ucb->rbuf.buflen >= ucb->rbuf.buflenlimit)
        {
            result = false;
        }

        if(result)
        {
//          if(pkg->offset >= sizeof(struct sockaddr_in))
            if(PkgGetOffset(pkg) >= sizeof(struct sockaddr_in))
            {
                addrin.sin_family = AF_INET;
                addrin.sin_port =portsrc;
                addrin.sin_addr.s_addr = ipsrc;
                memcpy(PkgGetBuffer(pkg),(void *)&addrin, sizeof(addrin));
                __addpkg2rbuf(ucb,pkg);
                if(ucb->rbuf.buflen > ucb->rbuf.triglevel)
                {
                    handle_SetMultiplexEvent(fd2Handle(sock->sockfd),CN_SOCKET_IOREAD);
                    semp_post(ucb->rbuf.bufsync);
                }
            }
        }
        mutex_unlock(sock->SockSync);
    }
    else
    {
        //do the icmp host not reached
    }

EXIT_RCVEND:
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
            result = __rcvdealv4(ipsrc,ipdst,pkglst,devfunc);
        }
    }
    return result;
}


#define CN_UDP_DEBUG_PREFIX  "         "
static void __udpdebug(struct Socket *sock,char *filter)
{
    tagUdpCB    *ucb = NULL;
    char *prefix = CN_UDP_DEBUG_PREFIX;
    debug_printf("udp","UDPSOCKET:address:0x%08x\n\r",(u32)sock);
    debug_printf("udp","%s:iplocal :%s    portlocal :%d\r\n",\
            prefix,inet_ntoa(*(struct in_addr*)&sock->element.v4.iplocal),ntohs(sock->element.v4.portlocal));
    debug_printf("udp","%s:ipremote:%s    portremote:%d\r\n",\
            prefix,inet_ntoa(*(struct in_addr*)&sock->element.v4.ipremote),ntohs(sock->element.v4.portremote));
    debug_printf("udp","%s:0x%08x sockstat:0x%08x    UserTag:0x%08x\n\r",\
            prefix,sock->sockstat,sock->SockUserTag);
    debug_printf("udp","%s:syncstat:%d\n\r",prefix,sock->SockSync->enable);
    debug_printf("udp","%s:errno   :%d\n\r",prefix,sock->errorno);

    ucb = (tagUdpCB    *)sock->TplCB;
    if(NULL != ucb)
    {
        debug_printf("udp","%s:channelstat:0x%04x\n\r",prefix,ucb->channelstat);
        debug_printf("udp","%s:rbuf:len:0x%08x buflen:0x%08x triglevel:0x%08x timeout:0x%08x SockSync:%d\n\r",\
                prefix,ucb->rbuf.buflen,ucb->rbuf.buflenlimit,ucb->rbuf.triglevel,\
                ucb->rbuf.timeout,ucb->rbuf.bufsync->lamp_counter);
        debug_printf("udp","%s:framrcv:0x%llx  framsnd:0x%llx\n\r",prefix,ucb->framrcv,ucb->framsnd);
    }
    else
    {
        debug_printf("udp","%s:no control block yet\n\r",prefix);
    }
}

// =============================================================================
// FUNCTION��this function is used to initialize the udp protocol
// PARA  IN��
// PARA OUT��
// RETURN  ��true success while false failed
// INSTRUCT:
// =============================================================================
bool_t UdpInit(void)
{
    bool_t   result = false;

    if(false == __hashTabInit(CFG_UDP_HASHLEN))
    {
        return result;
    }
    if(false ==__UdpCbInit(CFG_UDP_CBNUM))
    {
        return result;
    }

    memset((void *)&gExtProto,0, sizeof(gExtProto));
    gExtProto.__socket     = __udpsocket;
    gExtProto.__bind       = __udpbind;
    gExtProto.__connect    = __udpconnect;
    gExtProto.__recv       = __udprecv;
    gExtProto.__recvfrom   = __udprecvform;
    gExtProto.__send       = __udpsend;
    gExtProto.__sendto     = __udpsendto;
    gExtProto.__setsockopt = __udpsetsockopt;
    gExtProto.__shutdown   = __udpshutdown;
    gExtProto.__close      = __udpclose;
    gExtProto.__debuginfo  = __udpdebug;
    if((false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,IPPROTO_UDP, &gExtProto))||\
       (false ==TPL_RegisterProto(AF_INET,SOCK_DGRAM,0, &gExtProto))||\
       (false == IpInstallProto("udp",IPPROTO_UDP,__rcvdeal)))
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