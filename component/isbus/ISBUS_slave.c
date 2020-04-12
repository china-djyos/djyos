//================================================================================
//组件名称：ISBUS_slave
//组件描述：工业串行总线（Industrial Serial Bus）通信模块从机端部分
//开发版本：V1.00
//开发人员: YT
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <device.h>
//#include <driver.h>
#include <systime.h>
#include "ISBUS.h"
#include "ISBUS_Protocol.h"
#include "timer_hard.h"
#include "dbug.h"
#include "component_config_isbus.h"

//通信端口的描述
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //组成单向链表，末端指向NULL
    struct ISBUS_FunctionSocket *SocketHead; //组成单向循环链表
    struct ISBUS_FunctionSocket *SocketCurrent; //指向当前接收的功能号值
    struct MultiplexSetsCB *MultiplexPort;  //端口多路复用
    s32 SerialDevice;                       //对应的设备
    ISBUS_FntProtocolError fnError;         //出错对应的回调函数
    u32 ErrorPkgs;                          //累计错误数
    u32 ErrorLast;                          //最后一次错误号
    u32 Timeout;                            //超时时间
    u32 Resttime;                           //离触发超时剩余时间
//  s32 SendTimes;                          //发送buffer中的数据发送次数，-1表示无限次数
    u8  MTCcast[256];                       //最近收到的广播或组播数据包备份
    u8  EchoModel;                          //当前应答模式， ONE_BY_ONE 等
    u8  src;                                //当前接收到的包的源地址
    u8  dst;                                //当前接收到的包的目的地址
    s16 analyzeoff;                         //解包指针
    s16 recvoff;                            //当前接收指针
    u8  BoardcastPre;                       //批量广播轮询的前序地址
    u8  MTCPre;                             //批量组播轮询的前序地址
    u8 *SendPkgBuf;                         //发送数据包指针（包含协议头）
    u8 *RecvPkgBuf;                         //接收数据包指针（包含协议头）
    u16 PortMaxRecvLen;                     //本端口最大接收包长度，包含协议头
    u16 PortMaxSendLen;                     //本端口最大发送包长度，不含协议头
//  u16 PortSendLen;                        //本端口正在发送的包长度，不含协议头
//    u16 RecvP;                              //接收偏移量，不含协议头
//    u32 SendP;                              //发送偏移量，！！！含协议头
    u8 MTC_Address;                         //本端口接受的组播地址，若不接受组播
                                            //可设为CN_MTC_RESV（默认）
};

u8 sg_u8SlaveAddress = 1;                          //从机本地地址
struct Slave_ISBUSPort *sg_ptSlavePortHead = NULL; //从机端链表初始化
static u16 sg_ptSlaveEvtt;


struct ISBUS_FunctionSocket * __Slave_GetProtocol(struct Slave_ISBUSPort *Port,u8 Protocol)
{
    struct ISBUS_FunctionSocket *Next,*SocketStart;
    bool_t found = false;
    SocketStart = Port->SocketCurrent;
    if(SocketStart != NULL)
    {
        Next = SocketStart;
        do
        {
            if(Next->Protocol == Protocol)
            {
                found = true;
                break;
            }
            Next = Next->Next;
        } while(Next != SocketStart);
    }
    if(found)
    {
        Port->SocketCurrent = Next; //记住当前值，下次查找往往是同一个功能。
        return Next;
    }
    else
        return NULL;
}

// ============================================================================
// 函数功能：从机端协议处理函数，该处理函数一般作为一个高优先级的线程弹出
// 输入参数：无
// 返回值：  无
// ============================================================================
ptu32_t ISBUS_SlaveProcess(void)
{
    u32 starttime,nowtime;
    struct Slave_ISBUSPort *Port;
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    s32 DevRe = Port->SerialDevice;
    u8 *protobuf;
    u8 chk,len, mydst;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    s16 exdata;

    Djy_GetEventPara((ptu32_t*)&Port, NULL);
    DevRe = Port->SerialDevice;
    mydst = sg_u8SlaveAddress;
    while(1)
    {

        bool_t Gethead = false;

        protobuf = Port->RecvPkgBuf;
        startoffset = Port->analyzeoff;
        readed = Port->recvoff;
        while(1)
        {
            starttime = (u32)DjyGetSysTime();
            if(startoffset == readed)
            {
                startoffset = 0;
                readed = 0;
                Port->analyzeoff = 0;
                Port->recvoff = 0;
            }
            if((readed >= 256) && (startoffset > 128))
            {
                memcpy(protobuf, &protobuf[startoffset], readed-startoffset);
                readed -= startoffset;
                startoffset = 0;
                Port->analyzeoff = 0;
                Port->recvoff = readed;
            }
            tmp = DevRead(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed,
                                        0, Port->Timeout);
            if(tmp != 0)
            {
                if(debug_ctrl ==true)
                {
                    printf("\r\nslave recv:");
                    for(tmp1 = 0; tmp1<tmp;tmp1++)
                        printf("%02x ",protobuf[tmp1+readed]);
                }
                readed += tmp;
            }
            if( ! Gethead)
            {
                for(; startoffset < readed;startoffset++)
                {
                    if(protobuf[startoffset] == 0xEB)       //找到包头，往下走
                    {
                        Gethead = true;
                        break;
                    }
                }
            }
            if(Gethead && (readed - (s16)startoffset >= sizeof(struct ISBUS_Protocol)))
            {
                if((protobuf[startoffset + CN_OFF_DST] == mydst)    //本机地址
                    ||(protobuf[startoffset + CN_OFF_DST] >= CN_INS_MULTICAST)  //广播或组播地址
                    ||(protobuf[startoffset + CN_OFF_SRC] == Port->BoardcastPre)//广播前置地址
                    ||(protobuf[startoffset + CN_OFF_SRC] == Port->MTCPre))     //组前置地址
                {
                    chk = 0xEB + protobuf[startoffset + CN_OFF_DST]
                               + protobuf[startoffset + CN_OFF_PROTO]
                               + protobuf[startoffset + CN_OFF_SRC]
                               + protobuf[startoffset + CN_OFF_LEN];    //计算chk
                    if(chk == protobuf[startoffset + CN_OFF_CHKSUM])
                    {
                        break;       //找到合法的协议头，退出循环，继续收数据包
                    }
                    else
                    {
                        if(Port->fnError != NULL)
                            Port->fnError((void*)Port, CN_INS_CHKSUM_ERR);
                        Port->ErrorLast = CN_INS_CHKSUM_ERR;
                        Port->ErrorPkgs++;
                        startoffset++;
                        Gethead = false;
                        continue;       //startoffset不变，while循环中,从当前位置重新寻找 0xEB
                    }
                }
                else
                {
                    Gethead = false;
                    startoffset++;
                    continue;       //startoffset不变，while循环中,从当前位置重新寻找 0xEB
                }
                break;
            }
            else        //一个超时周期过去了，没收到完整的协议头，肯定超时了。
            {
                if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
                {
                    if(Port->fnError != NULL)
                        Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR);
                    Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                    Port->ErrorPkgs++;
                    Port->analyzeoff = 0;
                    Port->recvoff = 0;
                    return false;
                }
            }
        };
        memcpy((u8*)&protohead, protobuf+startoffset, sizeof(struct ISBUS_Protocol));
        len = protobuf[startoffset + CN_OFF_LEN];
        //计算仍需要接收的字节数，如果已经超量接收，则会是负数
        restlen = len - (readed-startoffset - sizeof(struct ISBUS_Protocol));
        Completed = 0;
        if(restlen > 0)                 //仍有数据需要接收
        {
            //以下接收用户协议包
            if((Port->PortMaxRecvLen-readed) < restlen)    //buf的剩余空间不够容纳数据包
            {
                //buf剩余空间不够，先把已接收的数据移动到buf的开头。
                memcpy(protobuf,protobuf+startoffset,readed-startoffset);
                readed -= startoffset;
                startoffset = 0;
            }
            while(1)
            {
                tmp = DevRead(DevRe, &protobuf[readed],restlen, 0, Port->Timeout);
                Completed += tmp;
                readed += tmp;
                if(Completed >= restlen)
                    break;
            }
            exdata = Completed - restlen;
        }
        else
            exdata = 0 - restlen;

        Me = __Slave_GetProtocol(Port, protohead.Protocol);
        if(Me != NULL)
        {
            if((protohead.DstAddress == mydst) && (Me->MyProcess != NULL))
            {
                Me->MyProcess(Me, protohead.SrcAddress,
                              protobuf + startoffset + sizeof(struct ISBUS_Protocol), len);
            }
            else if(protohead.DstAddress >= CN_INS_MULTICAST)
            {
                Port->EchoModel = BROADCAST_MODEL;
                memcpy(Port->MTCcast, protobuf + startoffset + sizeof(struct ISBUS_Protocol),len);
                if((Port->BoardcastPre == 0) && (Me->MyProcess != NULL))     //本机地址是第一个从机。
                {
                    Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
                }
            }
            else if((protohead.SrcAddress == Port->BoardcastPre)
                        && (Port->EchoModel == BROADCAST_MODEL)
                        && (Me->MyProcess != NULL))
            {
                Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
            }
        }
        startoffset += sizeof(struct ISBUS_Protocol) + len;
        if(startoffset == readed)
        {
            Port->analyzeoff = 0;
            Port->recvoff = 0;
        }
        else
        {
            Port->analyzeoff = startoffset;
            Port->recvoff = readed;
        }
        Port->src = protohead.SrcAddress;
        Port->dst = protohead.DstAddress;
//        return true;
    }
}

// ============================================================================
// 函数功能：安装内部串口通信模块。该模块仅适用于从机部分。
// 输入参数：StackSize，模块需要的内存尺寸，由于串行通信协议解析后，要调用用户
//              提供的回调函数处理数据，本模块自身需要的内存很少，最多不过数百
//              字节，请用户根据自己的回调函数决定。
//       CN_PRIO_RLYMAIN，事件优先级，由用户自己选取，一般选择较高优先级
// 返回值： 总是成功，不成功就进入死循环。
// ============================================================================
bool_t ISBUS_SlaveInit(u32 StackSize)
{
    sg_ptSlaveEvtt = Djy_EvttRegist(EN_INDEPENDENCE, CN_PRIO_REAL, 1, 1, ISBUS_SlaveProcess,
                            NULL, StackSize, "ISBUS slave");
    if(sg_ptSlaveEvtt == CN_EVTT_ID_INVALID)
    {
        debug_printf("ISBUS","ISBUS通信模块从机端初始化异常\n\r");
        while(1);                               //初始化阶段就异常，直接死循环
    }
    else
    {
        return true;
    }
}

//-----------------------------------------------------------------------------
//功能：协议处理函数，用于接收组播列表，设置本端口组播号、组播前序地址，并把本端口
//      的应答模式改为组播批量模式，立即生效。
//参数：ProtocolSocket，协议槽指针
//      buf，接收到的数据包
//      len，数据包长度
//返回：无异议
//-----------------------------------------------------------------------------
void __SetMTC_Address(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src, u8 *buf,u32 len)
{

}

//-----------------------------------------------------------------------------
//功能：协议处理函数，用于接收从机列表，设置本端口广播前序地址，并把本端口的应答
//      模式改为广播批量模式，立即生效。
//参数：ProtocolSocket，协议槽指针
//      buf，接收到的数据包
//      len，数据包长度
//返回：无异议
//-----------------------------------------------------------------------------
void __SetSlaveList(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    struct Slave_ISBUSPort *Port;
    u8 nearest = 255,least = 255;
    u8 loop,now;
    Port = ProtocolSocket->CommPort;
    for(loop = 0;loop < len; loop++)
    {
        now = buf[loop];
        if(now < sg_u8SlaveAddress)
        {
            if(now- sg_u8SlaveAddress < nearest)
                nearest = now;
        }
        if(now < least)
            least = now;            //找出从机表中编号最小的地址
    }
    if(sg_u8SlaveAddress == least)  //若本板地址编号最小，则其前置地址是主机地址
        sg_ptSlavePortHead->BoardcastPre = 0;
    else
        sg_ptSlavePortHead->BoardcastPre = nearest;
}

//-----------------------------------------------------------------------------
//功能：协议处理函数，用于发送主机查询
//      模式改为广播批量模式，立即生效。
//参数：ProtocolSocket，协议槽指针
//      buf，接收到的数据包
//      len，数据包长度
//返回：无异议
//-----------------------------------------------------------------------------
void __CHK_SlaveSend(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}

// ============================================================================
// 函数功能：注册使用的通信端口，事实上，不是串口也可以注册的。从机端。
//        通信端口必须按djyos的设备驱动框架来驱动。
// 输入参数：dev，通信端口的设备指针
//        fnError，通信错误对应的回调函数，不需要则给NULL
//        Timeout，长时间没有收到数据的超时告警门限，uS为单位，单时间精度受
//                操作系统tick间隔限制。0xffffffff（-1）代表不设超时。
// 返回值：  通信插口指针
// ============================================================================
struct Slave_ISBUSPort *ISBUS_SlaveRegistPort(char *dev,\
                                ISBUS_FntProtocolError fnError,u32 Timeout)
{
    struct Slave_ISBUSPort *Port;
    u8 *recvbuf;
    s32 devfd;
    devfd = open(dev, O_RDWR);
    if(devfd == -1)
        return NULL;
    Port = (struct Slave_ISBUSPort *)malloc(sizeof(struct Slave_ISBUSPort ));
    recvbuf = malloc(512+2*sizeof(struct ISBUS_Protocol));
    if((Port != NULL) && (recvbuf != NULL)) //分配成功
    {
        memset(Port, 0, sizeof(struct Slave_ISBUSPort ));
        if(sg_ptSlavePortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = sg_ptSlavePortHead;
        }
        sg_ptSlavePortHead = Port;//申请到的Port赋给sg_ptPortHead，Port是中间量
        Port->SocketHead = NULL;   //对Slave_FunctionSocket链表进行初始化
        Port->SocketCurrent = NULL;
        Port->SerialDevice = devfd;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->analyzeoff = 0;
        Port->recvoff = 0;
        Port->SendPkgBuf = recvbuf + 256 + sizeof(struct ISBUS_Protocol);
        Port->RecvPkgBuf = recvbuf;
        Port->PortMaxRecvLen = sizeof(struct ISBUS_Protocol);
        Port->PortMaxSendLen = sizeof(struct ISBUS_Protocol);
//      Port->PortSendLen = 0;
//        Port->RecvP = 0;
        //若置0，初始状态会发送一包数据。
//        Port->SendP = sizeof(struct ISBUS_Protocol);  //发送偏移量为协议头
        Port->MTC_Address = CN_INS_MTC_INVALID;
//        Driver_SetUserTag(dev, (ptu32_t)Port);
        dev_SetUserTag(devfd, (ptu32_t)Port);
//        Multiplex_AddObject(Port->MultiplexPort, devfd,
//                                           CN_MULTIPLEX_SENSINGBIT_READ
////                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //对于主机端，可写不可用
//                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                        |  CN_MULTIPLEX_SENSINGBIT_ET
//                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
        Djy_EventPop(sg_ptSlaveEvtt, NULL, 0, (ptu32_t)Port, 0, 0);
        ISBUS_SlaveRegistProtocol(Port, CN_SET_SLAVE_TABLE, 255, 0, __SetSlaveList);
        ISBUS_SlaveRegistProtocol(Port, CN_SET_MTC_TABLE, 255, 0, __SetMTC_Address);
        ISBUS_SlaveRegistProtocol(Port, CN_CHK_SLAVE, 0, 0, __CHK_SlaveSend);
    }
    else
    {
        free(Port);
        free(recvbuf);
    }
    return Port;
}

// ============================================================================
// 函数功能：注册通信协议。从机端。
// 输入参数：Port，通信插口指针，为INS_RegistPort函数的返回值
//        Protocol，功能码，看 CN_INS_PROTOCOL_DEFAULT 等常数定义
//        MaxRecvLen，本协议接收数据包的最大长度
//        MaxSendLen，本协议发送数据包的最大长度
//        fn，接收数据包的协议处理函数
// 返回值：  协议插口指针
// ============================================================================
struct ISBUS_FunctionSocket *ISBUS_SlaveRegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                            u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn)
{
    struct ISBUS_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct ISBUS_FunctionSocket *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result用于出错判断，不能用指针是否NULL，因为有单工工作的通信口，Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen - sizeof(struct ISBUS_Protocol))
        {
//            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //接收数据包指针（不含协议头）
//                                        (MaxRecvLen+ sizeof(struct ISBUS_Protocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct ISBUS_Protocol));
//            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen - sizeof(struct ISBUS_Protocol)) //发送部分
        {
//            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
//                                       MaxSendLen + sizeof(struct ISBUS_Protocol));
            Port->PortMaxSendLen = MaxSendLen + sizeof(struct ISBUS_Protocol);
//            result |= (Port->RecvPkgBuf != NULL);
        }
        if(Port->SocketHead == NULL)
        {
            ProtocolSocket->Next = ProtocolSocket;
            Port->SocketHead = ProtocolSocket;
            Port->SocketCurrent = ProtocolSocket;
        }
        else
        {
            ProtocolSocket->Next = Port->SocketHead->Next;
            Port->SocketHead->Next = ProtocolSocket;
        }
        ProtocolSocket->Protocol = Protocol;           //功能号
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //本协议最大接收包长度
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //本协议最大发送包长度
        ProtocolSocket->CommPort = Port;               //对应的通信口指针
        ProtocolSocket->MyProcess = fn;                //协议处理函数指针
    }
    else
    {
        debug_printf("ISBUS","ISBUS_SlaveRegistProtocol failure");
//      free(Port->RecvPkgBuf);
//      free(ProtocolSocket);
//      free(Port->PollSendPkgBuf);
//      return NULL;
    }
    return ProtocolSocket;
}

// ============================================================================
// 函数：设置发送数据包发送函数，由用户调用，本函数加上协议头然后发送出去。从机端。
// 输入参数：Slave_FunctionSocket，通信协议插口指针，为INS_RegistProtocol函数的返回值
//        dst，对于从机：目的地址为主机地址，源地址为本板地址Slave_sg_u8Address
//        buf，待发送的数据包，不含协议头
//        len，发送长度
//        times，发送次数，-1表示无限次数。
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 ISBUS_SlaveSendPkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket, u8 dst, u8 *buf, u8 len)
{
    struct Slave_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen,tmp;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
//  Port->SendTimes = times;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + ISBUS_FunctionSocket->Protocol
                              + sg_u8SlaveAddress + len;
    SendLen = len + sizeof(struct ISBUS_Protocol);
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);

    DevWrite(Port->SerialDevice, SendBuf, SendLen, 0, CN_TIMEOUT_FOREVER);
    if((debug_ctrl ==true))
    {
        printf("\r\nslave send:");
        for(tmp = 0;tmp < SendLen;tmp++)
        {
            printf("%02x ",SendBuf[tmp]);
        }
    }
//  if(Completed != -1)
//      Port->SendP = Completed;

    return len;
}

// ============================================================================
// 函数功能：设置从机的本机地址。
// 输入参数：Addr，本机地址，8位
// 返回值： 无
// 注意：须在注册端口之前调用
// ============================================================================
void ISBUS_SlaveSetAddress(u8 Addr)
{
    if(Addr < CN_INS_MULTICAST)  //不能超过组播起始地址
    {
        sg_u8SlaveAddress = Addr;
    }
}

// ============================================================================
// 函数功能：设置本机接受的组播地址。
// 输入参数：Port，被设置的目标通信口
//           Addr，组播地址，8位
// 返回值： 无
// ============================================================================
void ISBUS_SlaveSetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr)
{
    if((Addr >= CN_INS_MULTICAST) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}

