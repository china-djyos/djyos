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
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ��������ɣ���������ֲ����
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
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "dbug.h"

#include "../common/router.h"
#include "../common/tpl.h"
#include "../common/ip.h"
#include "../common/ipV4.h"
#include "../common/netpkg.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"tcp"          //tcpЭ��
//parent:"tcpip"                //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"lock","heap","device file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_TCP == false )
//#warning  " tcp  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_TCP    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,,,
//%$#@enum,true,false,
#define     CFG_TCP_REORDER             true    //"TCP��������ʹ��",��Դ����Ŵ�
//%$#@num,,,
#define     CFG_TCP_CCBNUM              10      //"tcp �ͻ�������ֵ"��ռһ�� ָ�� �� struct ClientCB
#define     CFG_TCP_SCBNUM              5       //"tcp ����������ֵ"��ռһ�� ָ�� �� struct ServerCB
#define     CFG_TCP_SOCKET_HASH_LEN     10      //"tcp socket hashtable����"������ͨ����IP+port����Ԫ�����socket
//%$#@string,1,256,
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure



//some defines for the tcp
//LOCAL MACHINE STAT ENUM
enum _EN_TCPSTATE
{
    EN_TCP_MC_CREAT = 0 ,  //�ձ�������״̬��һ�����ֻ�����������ĲŻ��ڴ�״̬
    EN_TCP_MC_SYNRCV,      //socket�յ���SYN
    EN_TCP_MC_SYNSNT,      //socket�Ѿ�������SYN�ź�
    EN_TCP_MC_ESTABLISH,   //�ȶ�����״̬
    EN_TCP_MC_FINWAIT1,    //�����رշ�����FIN
    EN_TCP_MC_FINWAIT2,    //�����رշ�����FIN�����յ���ACK
    EN_TCP_MC_CLOSING,     //�����رշ�����FIN(δ�յ�ACK)���յ������FIN
    EN_TCP_MC_TIMEWAIT,    //�����رշ���FIN���ҽ��յ���FIN��2ML��ʱ
    EN_TCP_MC_CLOSEWAIT,   //�����ر��յ��˶����FIN
    EN_TCP_MC_LASTACK,     //�����رշ�����FIN
    EN_TCP_MC_2FREE,       //���ͷ�״̬
};
//some default defines for the tcp
#define CN_TCP_BUFTRIGDEFAULT   0           //TRIGGER LEVEL DEFAULT
#define CN_TCP_SNDBUF_SIZEDEFAULT  0x800    //Ĭ�� tcp ���� buffer size
#define CN_TCP_RCVBUF_SIZEDEFAULT  0x800    //Ĭ�� tcp ���� buffer size
#define CN_TCP_MSSDEFAULT          1460     //Ĭ�� tcp mss size
//#define CN_TCP_LISTENDEFAULT    5           //LISTEN BAKLOG DEFAULT
#define CN_TCP_RCMBLENDEFAULT   0x10        //most allowing num frame in the recombine list
#define CN_TCP_ACCEPTMAX        0x10000     //������listen��acceptǰ���ŶӵĿͻ�������

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

//tcp timer���壬���ж��嶼��tcp ticks ����ÿtick=100mS��
#define CN_TCP_TICK_TIME                   (100*mS) //Units:Micro Seconds
#define CN_TCP_TICK_2ML                    (10)     //unit:tcp tick
#define CN_TCP_TICK_KEEPALIVE              (500*10) //unit:tcp tick
#define CN_TCP_TICK_PERSIST                (10)     //unit:tcp tick
#define CN_TCP_TICK_ACK                    (5)      //Unit:package number
#define CN_TCP_TIMER_SYN                   (1<<0)   //SYNC TIMER
#define CN_TCP_TIMER_2MSL                  (1<<1)   //2ML TIMER control bit
#define CN_TCP_TIMER_KEEPALIVE             (1<<2)   //KEEP TIMER control
#define CN_TCP_TIMER_PERSIST               (1<<3)   //PERSIST TIMER control bit
#define CN_TCP_TIMER_CORK                  (1<<4)   //�������� TIMER control bit
#define CN_TCP_TIMER_LINGER                (1<<5)   //LINGER TIMER START
#define CN_TCP_TIMER_RESEND                (1<<6)   //RESEND TIMER
#define CN_TCP_TIMER_FIN                   (1<<7)   //FIN TIMER
#define CN_TCP_TIMER_NONE                  (0)      //NO TIMER IS START
//define for the rtt
#define CN_TCP_RTOMAX                      150    //�ش���ʱ����ʼʱ�����ֵ:tcp tick
#define CN_TCP_RTOMIN                      2      //�ش���ʱ����ʼʱ����Сֵ:tcp tick
#define CN_TCP_SA_INIT                     0      //rttƽ��ֵ�ĳ�ʼֵ:tcp tick
#define CN_TCP_SD_INIT                     4      //rttƽ��ֵƫ��ĳ�ʼֵ:tcp tick
#define CN_TCP_FASTRETRANS                 3      //�յ����ٴ���ͬ ackno ִ�п����ش�
#define CN_TCP_CWND_LIMIT                  60000
#define CN_TCP_RSNDMAX                     8     //����ش�����
//tcp flags
#define CN_TCP_FLAG_FIN  (1<<0)     //����λ��Ҳ�ִ�С��ô��
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
    struct SemaphoreLCB      *bufsync;     //��ʼ��Ϊ���źţ�
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
    struct SemaphoreLCB *bufsync;     //buflenleft����trigerlevel��post
    u32                  timeout;     //if block, the block time
    u32                  rtotick;     //���һ�η������� ������ticks��
    u8                  *tcpsendbuf;  //tcp����buf��malloc�õ���
    s32                  buflenlimit; //����ccbʱ��ֵ������buffer lenʱ��ֵΪbuffer len.
    u32                  unackno;     //�ȴ��Է�ȷ�ϵ���ţ�����ccbʱ��ֵΪ sndnxtno��
                                      //�յ��Է�ackʱ����ֵΪ�Է���ackno���ڴ��յ�����
                                      //���ʱ������ʱ-1
    s32                  unackoff;    //����ccbʱ���㣻__ackdataʱ��������������ĩ�����㣻
                                      //����buffer len ʱ����;
    s32                  unacklen;    //uackoff �� sndnxtoff֮��ĳ��ȣ�����ccbʱ����;
                                      //����buffer len ʱ����;__ackdataʱ���������ʱ������ʱ-1
    u32                  sndnxtno;    //�����͵���ţ�����ccbʱ��__computeIsn�õ���
                                      //ÿ��������ʱ���ӣ���SYN���������1
    s32                  sndnxtoff;   //�����͵�����ƫ�ƣ�����ccbʱ���㣻__senddataʱ������
                                      //��������ĩ�����㣻����buffer len ʱ����;
    s32                  dataoff;     //bufferд��ƫ�ƣ�����ccbʱ���㣻__cpy2sndbufʱ������
                                      //��������ĩ�����㣻����buffer len ʱ����;
    s32                  datalen;     //δ�������ݳ���(����δȷ��)������ccbʱ���㣬__cpy2sndbufʱ������
                                      //__senddataʱ����,����buffer len ʱ����
    s32                  buflenleft;  //����buffer��������ccbʱ��ֵ��__cpy2sndbufʱ������
                                      //__ackdataʱ����������buffer lenʱ��ֵΪbuffer len.
    s32                  trigerlevel; //����������ˮƽ
    u8                   dupacktims;  //�յ���ͬ ackno �������ﵽ��ִֵ�п����ش�
                                      //unackno �ı�ʱ���㣬�յ� ackno==unackno ʱ����
}tagSendBuf;  //this buf used for the tcp send
//each client has an ClientCB for the tcp state control
struct ClientCB
{
    struct ClientCB           *nxt;          //���ڶ�̬�����ڴ��
    struct tagSocket         *server;       //�ǿձ�ʾboundin���ӣ���ָ����Ӧ������
    u16                       machinestat;  //the machine stat of the tcb
    u16                       channelstat;  //the stat of the channel,which mean we could recv or send
    tagRecvBuf                rbuf;         //rcv buffer
    tagSendBuf                sbuf;         //sendbuffer
    struct NetPkg            *pkgrecomblst; //the package recomb queue
    //the send window member
    u16                       mss;         //���η��͵�������ݰ�
    u8                        sndwndscale; //Զ�̴��ڷŴ�ָ��ѡ�����ʱ syn ������
    s32                       cwnd;        //ӵ�����ⴰ�ڣ���ʼ��Ϊ mss*10
    s32                       ssthresh;    //��������ֵ������ȷ��cwnd���м������Ĭ�� 65535
    s32                       sndwnd;      //Զ�̽��մ��ڣ�synʱ��ʼ��������ʱ������
                                           //�յ���ʱ���£���Ҫ����δȷ�ϵ�����
    //round trip time measure
    /* RTT(round trip time)��ر��� */
    /*
    Err = M-A           //A��RTTƽ��ֵ��M��ʵ�ʲ���ֵ��Err�����
    A��A + gErr         //��������ƽ��ֵ
    D��D + h( | Err |-D) //D�Ǿ�ֵƫ��������¾�ֵƫ��
    RTO = A + 4D        //����õ�RTO (�ش���ʱʱ��)  (��ʼ��ʱRTO = A + 2D  )
    g=0.125 ; h=0.25
    */
    s16 sa, sd;                /* ���ڼ���RTO��sa = 8*A��sv = 4*D */
    s16 rto;                   //�ش���ʱ����ʼ���ڣ������ش�ʱָ������
    //tcp timer
    u16                        timerctrl;
    u16                        mltimer;     //(unit:tcp tick,used for 2msl wait)
    u16                        lingertimer; //(unit:tcp tick,used for linger close)
    u16                        keeptimer;   //(unit:tcp tick,used for keep alive)
    u16                        persistimer; //(unit:tcp tick,used for probe window)
    u16                        corktimer;   //(unit:tcp tick,used for �������� timeout(supper Nagle))
    u16                        resndtimer;  //�����ش���syn��fin����һ��ʱ������rto��ʼ���� CN_TCP_RSNDMAX
                                            //      ���˱��ش����ղ�����Ӧ����ֹ��Ĭ�ϲ���Լ204�롣
                                            //syn������syn������
                                            //�����ش����������ݺ󣬻��յ���������ack������
                                            //fin������ fin ������
    u8                         resndtimes;  //when resend,add it;when ack the data then clear it
    u8                         acktimes;    //if acktimes overrun the limit, then will do fast ack
};
//each server has an ServerCB
struct ServerCB
{
    struct ServerCB           *nxt;                 //���ڶ�̬�����ڴ��
    s32                        pendlimit;           //which limit the pending num
    s32                        pendnum;             //which means how much still in pending
    u32                        accepttime;          //block time for the accept
    struct tagSocket          *clst;                //�÷�����accetp�����пͻ��ˣ�including the pending stat
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

//������������IP+port����Ԫ��ƥ���socket����ϣ�㷨�� __hashTabInit ����˵��
typedef struct
{
//  s32                          tablen;     //how long the hash tab is
    struct MutexLCB             *tabsync;    //used to peotect the hash tab
    struct tagSocket            *array[CFG_TCP_SOCKET_HASH_LEN];   //this is the hash tab
}tagTcpHashTab;
static tagTcpHashTab   TcpHashTab;
static void *pTcpTicker = NULL;

//-----------------------------------------------------------------------------
//����: ��ʼ��tcp hash���˱����ڽ��յ����ݰ�ʱƥ����Ӧ��socket�ã�hashtable�����ڴ�
//      ��ϵͳheap�з��䡣hash�㷨���ѵ�ַ��Ԫ��ȫ�����޷�����������������tablenȡģ��
//      ��hashֵ��hashֵ��ͬ�� socket �� Nextsock ָ�봮�������ǵ���ѭ������
//����: len��hash����
//����: true = �ɹ���false=ʧ�ܣ����ڴ治��
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
//����: ��hash���в���socket��
//����: iplocal������IP
//      portlocal�����ض˿�
//      ipremote��Զ��IP
//      portremote��Զ�̶˿�
//����: socketָ�룬��NULL
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
//���ܣ���hashtable�в���socket����ֻƥ�䱾�ض˿ں�IP������ȷ������bind�ظ��ĵ�ַ��
//����: iplocal������IP
//      portlocal�����ض˿�
//����: socketָ�룬��NULL
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
//���ܣ����hash��������hashֵ�ظ��ģ����������ͬhash������ĩβ��
//������sock������ӵı���
//���أ�true��
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
// PARA OUT��
// RETURN  :the hash item we create
// INSTRUCT:add the create item to the last
// =============================================================================
//------------------------------------------------------------------------------
//���ܣ��Ƴ�hash���ֻ�Ǵ�hash����ɾ���������ͷŸ�socket��
//������sock�����Ƴ���socket
//���أ�true
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

//----�����ʼ���--------------------------------------------------------------
//���ܣ������ʼ��ţ�ÿ��tcp���ӣ����͵����ݶ�ʼ��ĳһ�������ţ������������ʱ��
//     syn������֪ͨ�Է���
//��������
//���أ�32λ���
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
// PARA OUT��
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
// PARA OUT��
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
// ���ܣ���socket��һ����ַ�������˿ںź�IP��ַ
// ������sock�����󶨵�ַ��socket
//      addr�����󶨵ĵ�ַ
//      addrlen����ַ����
// ���أ�0 success while -1 failed
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
//���ܣ�����һ��������socket��ʹһ���˿����ڼ���״̬���ȼ���socket�Ƿ��Ѿ����ӡ�
//������sock����������sock��������δ���ӵ�socket
//      pendlimit���÷�����socket�ܽ��ܵ���������
//���أ�0=�ɹ���-1=ʧ��
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
//���ܣ��ӷ�����sock��ȡһ�������ȶ�����״̬�Ŀͻ���
//������sock��������socket
//���أ��ҵ��򷵻�����һ��socket���Ҳ����򷵻�NULL
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
        //һ����˵��������������Ҫ read ��־��ֻ��Ҫaccept��־���ɣ�����Щ��Դ���
        //��read��־��accept��־��������accept��־�������á�
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
        result = __acceptclient(sock);  //�鿴�Ƿ��Ѿ��ɽ���
        waittime = scb->accepttime;
        if((NULL == result)&&(0 != waittime))
        {
            Lock_SempPend(scb->acceptsemp,0);   //�������ź���
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
//Ӧ����ack��ʱ�����hash����
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
//        printf("----------server��%4u:%4d:%7u; %s\r\n",
//                        ntohl(hdr->seqno),translen,intervaltime(),n);
//    else if(hdr->portsrc == 0x5c54)
//        printf("------------client��%4u:%4d:%7u; %s\r\n",
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
                //�ȴ����������� syn ��־
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
                        (sock->sockstat & CN_SOCKET_CLOSE)==0) {//�������÷�������
                    //result = -1; //Ĭ����-1�����ﲻ������Ҳ����
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

//----copy����------------------------------------------------------------------
//���ܣ������ݴ��û�������copy����Ӧsocket��tcp�����������ƻ��λ������ṹ���������
//      ���ƣ���copy buffer��ĩ�ˣ���copy���Ʋ��֡�
//������sock��Ŀ��socket��ָ��
//      msg�������͵�����
//      len�������������ݳ��ȣ��ɵ��÷�ȷ�����ᳬ�� buf ʣ�೤�ȡ�
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

    if(len > 0)     //��ʾ�����л���
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
//���ܣ�ȷ��socket���ж������ݿ��Է��ͣ�ʹ��nagle�㷨��ӵ�������㷨��ȷ����ǰ�ж���
//      ���ݿ��Է��͡�
//������sock��Ŀ��sock
//���أ������͵��ֽ���
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
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)     //û�д�ȷ�ϵ�����
        {
            result = datalen;           //�ж������ݷ��Ͷ�������
        }
        else if(datalen > ccb->mss)     //���д�ȷ�ϵ����ݣ����ܹ�mss�ŷ���
        {
            result = datalen;           //�ж������ݷ��Ͷ�������
        }
        else
        {
            result = 0;                 //����mss��������
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
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //ֻ��һ����
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
//                            ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //�ݴ�����������紫���˺�����fin��4ML���ջء�
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
                PkgInit(pkgdata,CN_PKLGLST_END,0,datalen,data);  //ֻ��һ����
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
                    (sock->sockstat & CN_SOCKET_PROCONNECT)) {//�������÷�������
                result = -1;                //Ĭ����-1�����ﲻ������Ҳ����
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
                    (sock->sockstat & CN_SOCKET_PROCONNECT)) //�������÷�������
            {
                //result = -1; //Ĭ����-1�����ﲻ������Ҳ����
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
//                ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //�ݴ�����������紫���˺�����fin��4ML���ջء�
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
        case SO_RCVTIMEO:       // *optval ==0��Ч�ڷ�����ģʽ����
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
        case SO_SNDTIMEO:       // *optval ==0��Ч�ڷ�����ģʽ����
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
            //*optval == 0��ʾ��Ϊ����ģʽ��!=0��ʾ��Ϊ������ģʽ
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
//���ܣ��������ݣ������������͵���һ��
//������client���������ݵ�socket
//     seqno���������
//     pkg�����ݰ�
//���أ�true or false
//------------------------------------------------------------------------------
static u32 __rcvdata(struct tagSocket *client, u32 seqno,struct NetPkg *pkg)
{
    struct ClientCB   *ccb;
    tagRecvBuf *recbuf;
    u32        pkgstart;
    u32        pkgstop;     //����
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
        //�յ��İ��պþ����ڴ��İ�
        PkgCachedPart(pkg);         //��ס�½��յİ�
        if(NULL == ccb->rbuf.phead)    //�������뵽��������
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //���������ĩ�˽ڵ�
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if((pkgstop > pkgstart)&& ((ccb->rbuf.rcvnxt > pkgstart)
                                   &&(ccb->rbuf.rcvnxt < pkgstop)))
    {
        //tcp���δ���ƣ��ڴ����յ���������ݰ���Χ�ڣ����յ��İ������Ѿ������ա�
        //����δ�����ա�
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);    //�ƶ���Ч����ƫ�ƣ��Թ��ѽ��ղ���
        PkgCachedPart(pkg);         //��ס�½��յİ�
        if(NULL == ccb->rbuf.phead)    //�������뵽��������
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //���������ĩ�˽ڵ�
        rcvlen -= pkgdataoff;       //ʵ�ʽ����������ȥ�ظ���������
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if((pkgstop < pkgstart)&&((ccb->rbuf.rcvnxt > pkgstart)    //��Ӧrcvnxt�ܴ�
                                    ||(ccb->rbuf.rcvnxt < pkgstop)))//��Ӧrcvnxt��С
    {
        //ע�������������һ��else if һ��
        //tcp��ŷ������ƣ��ڴ����յ���������ݰ���Χ�ڣ����յ��İ������Ѿ������ա�
        //�޷�������������ʹrcvnxt < pkgstartҲ�ܵó���ȷ���
        pkgdataoff = ccb->rbuf.rcvnxt - pkgstart;
        PkgMoveOffsetUp(pkg,pkgdataoff);
        PkgCachedPart(pkg);         //��ס�½��յİ�
        if(NULL == ccb->rbuf.phead)    //�������뵽��������
        {
            ccb->rbuf.phead = pkg;
        }
        else
        {
            PkgSetNextUnit(ccb->rbuf.ptail,pkg);
        }
        rcvlen = PkgGetDataLen(pkg);
        ccb->rbuf.ptail = pkg;         //���������ĩ�˽ڵ�
        ccb->rbuf.buflen += rcvlen;
        ccb->rbuf.rcvnxt = pkgstop;
    }
    else if(pkgstop == ccb->rbuf.rcvnxt) //���Ѿ����չ��ˣ�Ӧ���ǳٵ����ط���������
    {

    }
    else if((ccb->rbuf.rcvnxt - pkgstart) > 0x80000000)
    {
        //�յ��İ����峬ǰ�ڴ����յ����
#if(CFG_TCP_REORDER == true)        //֧��������Ŵ���
        PkgCachedPart(pkg);
        PkgSetPrivate(pkg, seqno);
        distance  = seqno - ccb->rbuf.rcvnxt;           //�°��ĳ�ǰ��
        current = ccb->pkgrecomblst;
        if(current != NULL)
        {
            //�°�������������һ���ڵ�ǰ
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
                            break;      //��ǰ�ڵ�Ϊ�����
                        }
                    }
                    else
                        break;
                }
                if((seqno == PkgGetPrivate(pre)) && (PkgGetDataLen(pkg)==PkgGetDataLen(pre)))
                {
                    PkgTryFreePart(pkg);  //�����ظ����������ڶ���ط�ͬһ����
                                          //ע�⣬���������ص��İ����ܶ���
                }
                else  //����ǲ����ص�����������ʱ����������ʱ�Ŵ���
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
//      �����������˴������ͷţ�����������������ĵط��ͷš�
#endif      //for (CFG_TCP_REORDER == true)
    }
    else
    {
        //      �����������˴������ͷţ�����������������ĵط��ͷš�
    }

    //�ոտ��ܽ�����һЩ���ݵ�buffer���������������������޿ɽ��յ�����
#if(CFG_TCP_REORDER == true)
    pkgcomb = ccb->pkgrecomblst;
    while(NULL != pkgcomb)
    {
        pkgstart = (u32)PkgGetPrivate(pkgcomb);
        pkglen = PkgGetDataLen(pkgcomb);
        pkgstop = pkgstart + pkglen;
        recbuf = &ccb->rbuf;
        if(pkgstart == recbuf->rcvnxt)      //��ͷ���ص�����
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
        else if((pkgstart - recbuf->rcvnxt) > 0x80000000)       //�������ص�
        {
            //�����ص�����β������أ�������Ŵ�0��ʼ
            //�Է��������   �����հ����            �Է��յ�Ӧ�����
            //0~999         0~999�����ղ�ȷ�ϣ�      1000
            //1000~1999     δ�յ�                             1000
            //2000~2999     2000~2999����������У�  1000
            //3000~3999     3000~3999����������У�  1000
            //1000~2460     1000~2460�����ղ�ȷ�ϣ�  4000
            //��ʱ����������У�2000~2999�����������460�ֽ����ѽ��������ص�
            if((recbuf->rcvnxt - pkgstart) >= pkglen)
            {
                ccb->pkgrecomblst = PkgGetNextUnit(pkgcomb);
                PkgSetNextUnit(pkgcomb,NULL);
                pkgcomb = ccb->pkgrecomblst;
                PkgTryFreePart(pkgcomb);  //����ȫ�������Ѿ�����
            }
            else        //���ݰ�����Ӧ�ý���
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
        else    //���������û�пɽ��հ��ˡ�
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
//���ܣ�Ӧ���յ�������
//������client���ͻ���
//     hdr��tcpͷ
//���أ�true or false
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
    //���ȴ�����buffer���ͷſռ䡣
    //�Է�û���յ������� �� �Է�����Ҫ���յ�������
    if((ackno  == ccb->sbuf.unackno)&&(ccb->sbuf.unacklen > 0))
    {
        ccb->sbuf.dupacktims++;
        if(ccb->sbuf.dupacktims > CN_TCP_FASTRETRANS)
        {
            //do the fast retransmition
            ccb->rto = (u16)ccb->rto<<1;
//          if(ccb->rto < CN_TCP_RTOMIN)    //rto���������ж�����
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
    //ack�˲������ݵ�����δȷ�ϵ�
    else if(((ccb->sbuf.sndnxtno > ccb->sbuf.unackno)&&    //��Ų��޻�������δȷ�ϵ�����
       ((ackno >ccb->sbuf.unackno)&&(ackno <= ccb->sbuf.sndnxtno)))||
            ((ccb->sbuf.sndnxtno < ccb->sbuf.unackno)&&    //��Ų��޻�������δȷ�ϵ�����
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
//���ܣ����������Ѿ��յ�SYN
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
        if(ccb->sbuf.unackno == ccb->sbuf.sndnxtno)     //ack ����������
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
            //һ����˵��������������Ҫ read ��־��ֻ��Ҫaccept��־���ɣ�����Щ��Դ���
            //��read��־��accept��־��������accept��־�������á�
            Handle_SetMultiplexEvent(fd2Handle(server->sockfd),CN_SOCKET_IOACCEPT|CN_SOCKET_IOREAD);
            Lock_SempPost(scb->acceptsemp);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//���ܣ����հ������Ѿ�������SYN�ź�
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
               (client->sockstat & CN_SOCKET_PROCONNECT)) {//�������÷�������
                ccb->channelstat|=CN_TCP_CHANNEL_STATASND|CN_TCP_CHANNEL_STATARCV;
            }
            //notice the applications  the connect success
            Lock_SempPost(ccb->rbuf.bufsync);
        }
    }
    return true;
}

//------------------------------------------------------------------------------
//���ܣ����հ������Ѿ������ȶ�����״̬
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ������رշ�����FIN
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
        ccb->timerctrl |= CN_TCP_TIMER_2MSL;     //�ݴ�����������紫���˺�����fin��4ML���ջء�
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
//���ܣ������رշ�����FIN�����յ���ACK�����뿼�ǵ����ܻ��и������ݵ��������ȴû��ȥ����
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ������رշ�����FIN(δ�յ�ACK)���յ������FIN���κ����ݶ��ᱻ�����ش����ݡ�
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ������رշ���FIN���ҽ��յ���FIN��2ML��ʱ���κ����ݶ��ᱻ�����ش����ݡ�
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ������ر��յ��˶����FIN���κ����ݶ��ᱻ�����ش����ݡ�
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ������رշ�����FIN���κ����ݶ��ᱻ�����ش����ݡ�
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ�����socket�յ���Զ�����ݰ���
//������client���ͻ���
//     hdr��tcpͷ
//     pkg��ƫ���Ѿ�Խ��tcpͷ�������
//���أ�true or false
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
//���ܣ���server�յ�synʱ������һ��socket������tcp��server���ƿ��clst�£�Ȼ����
//      syn+ack�����ٴ��յ��Է���ackʱ������������״̬��
//������server��֤��listen��socket
//      hdr����ǰ�յ���tcpͷ
//      ipdst,portdst,ipsrc,portsrc����ַ��Ԫ��
//���أ��´�����socket��
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
    //resndtimer ���� syn �� fin �� ���ݰ� ���߷��ͺ���ش�
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
                //����д���ɶ���ǿ����ִ��Ч�ʸ�
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
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATARCV?"ʹ��APP����":"����APP����");
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATASND?"ʹ��APP����":"����APP����");
    printf("    %s  ",ccb->channelstat&CN_TCP_CHANNEL_STATKRCV?"ʹ��Э��ջ����":"����Э��ջ����");
    printf("    %s\r\n",ccb->channelstat&CN_TCP_CHANNEL_STATKSND?"ʹ��Э��ջ����":"����Э��ջ����");

    //send buffer
    sbuf = &ccb->sbuf;
    printf("    ����buffer��ַ 0x%x������ %d�����г��� %d\r\n",
                    sbuf->tcpsendbuf,sbuf->buflenlimit, sbuf->buflenleft);
    printf("    ��ȷ����� %u����������� %u�������ͳ��� %d\r\n",
                    sbuf->unackno, sbuf->sndnxtno, sbuf->datalen);

    //receive buffer
    rbuf = &ccb->rbuf;
    printf("    ����buffer���� 0x%x��buffered��%dbytes����������ţ�%u\r\n",
                rbuf->buflenlimit, rbuf->buflen, rbuf->rcvnxt);

    //windows
    printf("    mss:%04d cwnd:%d sndwndscale:%d\n\r",ccb->mss,ccb->cwnd);
    //timer
    printf("    ������tcp��ʱ���У�\r\n");
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
    printf("    accept��ʱ���� :%d,pending �� clients ��:%d ��\n\r",scb->accepttime,scb->pendnum);
    client = scb->clst;
    while(NULL != client)  //find the established and unopend socket
    {
        ccb = (struct ClientCB*)client->TplCB;
        if(EN_TCP_MC_ESTABLISH == ccb->machinestat)
        {
            printf("        client=%d,��accept��buffered��%dbytes\r\n",
                            client->sockfd, ccb->rbuf.buflen);
        }
        else
        {
            printf("        client=%d,��δ���3������\r\n", client->sockfd);
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
//���ܣ�tcpЭ���ʼ��
//��������
//���أ�true or false
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

