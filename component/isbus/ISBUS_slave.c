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
#include <shell.h>
#include "component_config_isbus.h"

//通信端口的描述
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //组成单向链表，末端指向NULL
    struct ISBUS_FunctionSocket *SocketHead; //组成单向循环链表
    struct ISBUS_FunctionSocket *SocketCurrent; //指向当前接收的功能号值
    struct MultiplexSetsCB *MultiplexPort;  //端口多路复用
    struct SemaphoreLCB *MTC_Semp;          //广播或组播，确保APP处理完成并更新回包后再发送回包给主机
    s32 SerialDevice;                       //对应的设备
    ISBUS_FntProtocolError fnError;         //出错对应的回调函数
    u32 ErrorPkgs;                          //累计错误数
    u32 ErrorLast;                          //最后一次错误号
    u32 Timeout;                            //超时时间
    u32 Resttime;                           //离触发超时剩余时间
//  s32 SendTimes;                          //发送buffer中的数据发送次数，-1表示无限次数
    u8  MTCcast[256];                       //最近收到的广播或组播数据包备份
    u8  EchoModel;                          //当前应答模式， ONE_BY_ONE 等
    u8  HostSerial;                         //接收到的主机包序号
    u8  src;                                //当前接收到的包的源地址
    u8  dst;                                //当前接收到的包的目的地址
    s16 analyzeoff;                         //解包指针
    s16 recvoff;                            //当前接收指针
    u8  BoardcastPre;                       //批量广播轮询的前序地址，收到从机表之前，不应答广播
    u8  MTCPre;                             //批量组播轮询的前序地址，收到组播表之前，不应答广播
    u8 *SendPkgBuf;                         //发送数据包指针（包含协议头）
    u8 *RecvPkgBuf;                         //接收数据包指针（包含协议头）
    u16 PortMaxRecvLen;                     //本端口最大接收包长度，包含协议头
    u16 PortMaxSendLen;                     //本端口最大发送包长度，不含协议头
    u8 MTC_Address;                         //本端口接受的组播地址，若不接受组播
                                            //可设为CN_MTC_RESV（默认）
};

u8 sg_u8SlaveAddress = 1;                          //从机本地地址
struct Slave_ISBUSPort *sg_ptSlavePortHead = NULL; //从机端链表初始化
static u16 sg_ptSlaveEvtt;

void __ISBUS_Ack(struct Slave_ISBUSPort *Port);
void __ISBUS_PushMtcPkg(struct Slave_ISBUSPort *Port);

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

//struct dbgrecord
//{
//    u32 ev;
//    u32 time;
//    u8 len;
//    u8  data[64];
//};
//struct dbgrecord dbgrecord[1010] = {0};
//u32 offset485 = 0;
//
//void recdbg(u32 ev, u8 *buf, u32 len)
//{
//    u32 cpylen;
//
//    if(offset485 < 1000)
//    {
//        if(len < 64)
//            cpylen = len;
//        else
//            cpylen = 64;
//        if(cpylen != 0)
//            memcpy(dbgrecord[offset485].data, buf, cpylen);
//        dbgrecord[offset485].len = cpylen;
//        dbgrecord[offset485].time = DjyGetSysTime();
//        dbgrecord[offset485].ev = ev;
//        offset485++;
//    }
//    else
//        offset485 = 0;
//}

// ============================================================================
// 函数功能：从机端协议处理函数，该处理函数一般作为一个高优先级的线程弹出
// 输入参数：无
// 返回值：  无
// ============================================================================
ptu32_t ISBUS_SlaveProcess(void)
{
    u32 starttime;
    struct Slave_ISBUSPort *Port;
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    s32 DevRe;
    u8 *protobuf;
    u8 chk,len, mydst;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    bool_t needread = true;
    bool_t newpkg;

    Djy_GetEventPara((ptu32_t*)&Port, NULL);
    DevRe = Port->SerialDevice;
    mydst = sg_u8SlaveAddress;
    while(1)
    {

        bool_t Gethead = false;

        protobuf = Port->RecvPkgBuf;
        startoffset = Port->analyzeoff;
        readed = Port->recvoff;
        starttime = (u32)DjyGetSysTime();
        while(1)        //此循环用于接收包头
        {
            if(startoffset == readed)
            {
                startoffset = 0;
                readed = 0;
                Port->analyzeoff = 0;
                Port->recvoff = 0;
                needread = true;
            }
            else if((readed >= 256) && (startoffset > 128))
            {
                memcpy(protobuf, &protobuf[startoffset], readed-startoffset);
                readed -= startoffset;
                startoffset = 0;
                Port->analyzeoff = 0;
                Port->recvoff = readed;
            }
            if(needread)
            {
//              recdbg(2, NULL,0);
                tmp = DevRead(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed,
                                                0, Port->Timeout);
                if(tmp != 0)
                {
//                  recdbg(1, &protobuf[readed],tmp);
                    if(debug_ctrl ==true)
                    {
                        printf("\r\nslave recv:");
                        for(tmp1 = 0; tmp1<tmp;tmp1++)
                            printf("%02x ",protobuf[tmp1+readed]);
                    }
                    readed += tmp;
                }
                else
                {
                    continue;
                }
            }
//            if((tmp == 12) && (protobuf[readed+7] == 4))
//                tmp = 12;
            if( ! Gethead)
            {
                for(; startoffset < readed;startoffset++)
                {
                    if(protobuf[startoffset] == 0xEB)       //找到包头，往下走
                    {
                        Gethead = true;
                        starttime = (u32)DjyGetSysTime();
                        break;
                    }
                    else
                        Gethead = false;
                }
            }
            if(Gethead )
            {
                if(readed - (s16)startoffset >= (s16)sizeof(struct ISBUS_Protocol))
                {
                    u8 *proto = protobuf + startoffset;
                    needread = false;
//                  if((proto[CN_OFF_DST] == mydst)    //本机地址
//                      ||(proto[CN_OFF_DST] >= CN_INS_MULTICAST)  //广播或组播地址
//                      ||(proto[CN_OFF_SRC] == Port->BoardcastPre)//广播前置地址
//                      ||(proto[CN_OFF_SRC] == Port->MTCPre))     //组前置地址
//                  {
                        chk = 0xEB + proto[CN_OFF_DST]
                                   + proto[CN_OFF_PROTO]
                                   + proto[CN_OFF_SRC]
                                   + proto[CN_OFF_SERIAL]
                                   + proto[CN_OFF_LEN];    //计算chk
                        if(chk == proto[CN_OFF_CHKSUM])
                        {
                            if(proto[CN_OFF_SRC] == 0)      //查看是否主机发来的包
                            {
                                if(proto[CN_OFF_SERIAL] != Port->HostSerial)
                                {
                                    newpkg = true;
                                    Port->HostSerial = proto[CN_OFF_SERIAL];
                                }
                                else
                                    newpkg = false;
                            }
                            break;      //协议头校验正确，退出循环，继续收和处理数据包
                        }
                        else            //包头校验出错，记录错误信息
                        {
                            if(Port->fnError != NULL)
                                Port->fnError((void*)Port, CN_INS_CHKSUM_ERR, sg_u8SlaveAddress);
                            Port->ErrorLast = CN_INS_CHKSUM_ERR;
                            Port->ErrorPkgs++;
                            startoffset++;
                            Gethead = false;
                            continue;       //while循环中,从当前位置重新寻找 0xEB
                        }
//                  }
//                  else
//                  {
//                      Gethead = false;
//                      startoffset++;
//                      continue;       //while循环中,从当前位置重新寻找 0xEB
//                  }
//                  break;
                }
                else        //一个超时周期过去了，没收到完整的协议头，肯定超时了。
                {
                    if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
                    {
                        if(Port->fnError != NULL)
                            Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR,sg_u8SlaveAddress);
                        Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                        Port->ErrorPkgs++;
                        Gethead = false;
                        startoffset = 0;
                        readed = 0;
                        printf("\r\n timeover return....");
                    }
                    needread = true;
                    continue;
                }
            }
            else
                needread = true;
        };
//      recdbg(7, NULL, 0);
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
//              recdbg(6, NULL, 0);
                tmp = DevRead(DevRe, &protobuf[readed],restlen, 0, Port->Timeout);
//              if(tmp != 0) recdbg(3, &protobuf[readed],tmp);
                Completed += tmp;
                readed += tmp;
                if(Completed >= restlen)
                    break;
                if(((u32)DjyGetSysTime() - starttime) > Port->Timeout)
                {
                    if(Port->fnError != NULL)
                        Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR,sg_u8SlaveAddress);
                    Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                    Port->ErrorPkgs++;
                    Gethead = false;
                    startoffset = 0;
                    readed = 0;
                    printf("\r\n protocol timeover return....");
                    break;
                }
            }
        }

        if(Completed >= restlen)    //检查是否收齐了数据
        {
//          recdbg(9, &protohead, 7);
            Me = __Slave_GetProtocol(Port, protohead.Protocol);
            if(Me != NULL)
            {
//              recdbg(10, NULL, 0);
                if(protohead.SrcAddress == 0)   //主机发来的数据包
                {
                    if(protohead.DstAddress < CN_INS_MULTICAST)
                        Port->EchoModel = ONE_BY_ONE;
                    else
                        Port->EchoModel = BROADCAST_MODEL;
                }
                if(protohead.DstAddress == mydst)  //收到点播包
                {
//                  recdbg(11, NULL,0);
                    if((newpkg) && (Me->MyProcess != NULL))
                    {
//                      recdbg(8, NULL, 0);
                        Me->MyProcess(Me, protohead.SrcAddress,
                                  protobuf + startoffset + sizeof(struct ISBUS_Protocol), len);
                    }
                    else
                    {
                        __ISBUS_Ack(Port);        //Ack仅代表收到了完整包
                        printf("\r\nignore repead pkg");
                    }
                }
                else if(protohead.DstAddress >= CN_INS_MULTICAST)   //收到广播或组播包
                {
//                  recdbg(12, NULL,0);
                    memcpy(Port->MTCcast, protobuf + startoffset + sizeof(struct ISBUS_Protocol),len);
                    if((newpkg) && (Me->MyProcess != NULL))
                    {
                        Me->MyProcess(Me, protohead.SrcAddress,Port->MTCcast, len);
                    }
                    else
                    {
                        __ISBUS_Ack(Port);        //Ack仅代表收到了完整包
                        printf("\r\nignore repead pkg");
                    }
                    if(Port->BoardcastPre == 0)     //本机地址是第一个从机。
                    {
                        __ISBUS_PushMtcPkg(Port);   //此函数会等待信号量，待用户准备好数据包
                    }
                }
                else if((protohead.SrcAddress == Port->BoardcastPre)
                            && (Port->EchoModel == BROADCAST_MODEL))
                {
//                  recdbg(13, NULL,0);
                    //广播状态下，收到前序地址发的包
                    __ISBUS_PushMtcPkg(Port);   //此函数会等待信号量，待用户准备好数据包
                }
            }
            else
            {
//              recdbg(14, NULL,0);
                __ISBUS_Ack(Port);        //Ack仅代表收到了完整包
            }
        }
//      recdbg(15, NULL,0);
        startoffset += sizeof(struct ISBUS_Protocol) + len;
        if(startoffset == readed)
        {
            Port->analyzeoff = 0;
            Port->recvoff = 0;
            needread = true;
        }
        else
        {
            Port->analyzeoff = startoffset;
            Port->recvoff = readed;
        }
//      Port->src = protohead.SrcAddress;
//      Port->dst = protohead.DstAddress;
    }
}

//bool_t showbuf(void)
//{
//    u32 loop,n;
//    for(loop = 0; loop < offset485; loop++)
//    {
//        if(dbgrecord[loop].ev == 1)
//            printf("\r\n首次：");
//        else if(dbgrecord[loop].ev == 2)
//            printf("\r\n始读：");
//        else if(dbgrecord[loop].ev == 3)
//            printf("\r\n补充：");
//        else if(dbgrecord[loop].ev == 4)
//            printf("\r\n广播：");
//        else if(dbgrecord[loop].ev == 5)
//            printf("\r\n点播：");
//        else if(dbgrecord[loop].ev == 6)
//            printf("\r\n再读：");
//        else if(dbgrecord[loop].ev == 7)
//            printf("\r\n收包：");
//        else if(dbgrecord[loop].ev == 8)
//            printf("\r\n处理：");
//        else if(dbgrecord[loop].ev == 9)
//            printf("\r\n收9 ：");
//        else if(dbgrecord[loop].ev == 10)
//            printf("\r\n收10：");
//        else if(dbgrecord[loop].ev == 11)
//            printf("\r\n收11：");
//        else if(dbgrecord[loop].ev == 12)
//            printf("\r\n收12：");
//        else if(dbgrecord[loop].ev == 13)
//            printf("\r\n收13：");
//        else if(dbgrecord[loop].ev == 14)
//            printf("\r\n收14：");
//        else if(dbgrecord[loop].ev == 15)
//            printf("\r\n收15：");
//        else
//            printf("\r\n");
//        printf("time = %d num = %d  ", dbgrecord[loop].time,dbgrecord[loop].len);
//        for(n = 0; n < dbgrecord[loop].len;n++)
//            printf(" %x",dbgrecord[loop].data[n]);
//    }
//}
//ADD_TO_ROUTINE_SHELL(showbuf, showbuf, "");

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
//返回：无意义
//-----------------------------------------------------------------------------
void __SetSlaveList(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    struct Slave_ISBUSPort *Port;
    u8 nearest = 255;   //小于本机地址且最接近本机地址的
    u8 least = 255;     //列表中最小地址
    u8 loop,now;
    bool_t found = false;
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
        if(now == sg_u8SlaveAddress)
            found = true;
    }
    if(found == true)
    {
        if(sg_u8SlaveAddress == least)  //若本板地址编号最小，则其前置地址是主机地址
            sg_ptSlavePortHead->BoardcastPre = 0;
        else
            sg_ptSlavePortHead->BoardcastPre = nearest;
    }
    else
        sg_ptSlavePortHead->BoardcastPre = 0xff;    //本机不在列表中，不响应广播。
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}

//-----------------------------------------------------------------------------
//功能：协议处理函数，用于回应主机查询状态，通常用于扫描从机是否存在
//参数：ProtocolSocket，协议槽指针
//      buf，接收到的数据包
//      len，数据包长度
//返回：无异议
//-----------------------------------------------------------------------------
void __CHK_SlaveSend(struct ISBUS_FunctionSocket *ProtocolSocket,u8 src,u8 *buf,u32 len)
{
    struct Slave_ISBUSPort *Port;
    Port = ProtocolSocket->CommPort;
    Port->HostSerial = 255;         //收到重新扫描从机命令，重置以下3个参数
    Port->BoardcastPre = 255;
    Port->MTCPre = 255;
    ISBUS_SlaveSendPkg(ProtocolSocket, 0, NULL, 0);
}
#pragma GCC diagnostic pop

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
    memset(Port, 0, sizeof(struct Slave_ISBUSPort ));
    recvbuf = malloc(512+2*sizeof(struct ISBUS_Protocol));
    Port->MTC_Semp = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "Slave Mtc");
    if((Port != NULL) && (recvbuf != NULL) && (Port->MTC_Semp != NULL)) //分配成功
    {
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
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->HostSerial = 255;
        Port->BoardcastPre = 255;
        Port->MTCPre = 255;
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
        Lock_SempDelete(Port->MTC_Semp);
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

u32 baktime;
//-----------------------------------------------------------------------------
//功能：收到地址正确，但协议插槽却不存在的数据包，或者插槽的处理函数是空的的情况，调用本函数
//      发送应答，以免主机苦等。
//参数：Port，被操作的端口。
//返回：无
//------------------------------------------------------------------------------
void __ISBUS_Ack(struct Slave_ISBUSPort *Port)
{
    u8 *SendBuf;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = 0;
    SendBuf[CN_OFF_PROTO]   = CN_SLAVE_ACK;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = 0;
    SendBuf[CN_OFF_SERIAL]  = 0;        //从机上送的包序号总是0
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + CN_SLAVE_ACK + sg_u8SlaveAddress;

    if(Port->EchoModel == ONE_BY_ONE)
        DevWrite(Port->SerialDevice, SendBuf, sizeof(struct ISBUS_Protocol),
                                    0, CN_TIMEOUT_FOREVER);
    else
    {
        baktime = (u32)DjyGetSysTime();
        Lock_SempPost(Port->MTC_Semp);
    }
}

//-----------------------------------------------------------------------------
//功能：把组播或广播模式下待发送的数据包发送出去。在组播或广播模式下，应用程序处理完成接收
//      到的数据后，并不能立即回包，而是必须收到排在你前面的从机发出的包后才能发包。
//参数：Port，被操作的端口。
//返回：无
//------------------------------------------------------------------------------
void __ISBUS_PushMtcPkg(struct Slave_ISBUSPort *Port)
{
    if(Lock_SempPend(Port->MTC_Semp, Port->Timeout))
    {
        if(DjyGetSysTime() - baktime > 2000000)
            baktime = 0;
//      recdbg(4, Port->SendPkgBuf,Port->SendPkgBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol));
        DevWrite(Port->SerialDevice, Port->SendPkgBuf,
                Port->SendPkgBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol),
                0, Port->Timeout);
        return ;
    }
}
// ============================================================================
// 函数：设置发送数据包，由从机端用户调用，本函数加上协议头然后发送出去。如果当前处于组播
//      或广播的模式，则并不立即发送出去，而是释放一个信号量，等到排在前面的从机发送完毕后，
//      由协议接收函数pend这个信号量，得到之后发送。
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
    extern u32 dbg_stopack;
    if(dbg_stopack != 0)        //用于调试模拟通信错误
    {
        dbg_stopack--;
        return false;
    }
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
//  Port->SendTimes = times;
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = 0;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = sg_u8SlaveAddress;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_SERIAL]  = 0;        //从机上送的包序号总是0
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + ISBUS_FunctionSocket->Protocol
                              + sg_u8SlaveAddress + len;
    SendLen = len + sizeof(struct ISBUS_Protocol);
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);

    if(Port->EchoModel == ONE_BY_ONE)
    {
//      recdbg(5, SendBuf,SendLen);
        DevWrite(Port->SerialDevice, SendBuf, SendLen, 0, Port->Timeout);
        if((debug_ctrl ==true))
        {
            printf("\r\nslave send:");
            for(tmp = 0;tmp < SendLen;tmp++)
            {
                printf("%02x ",SendBuf[tmp]);
            }
        }
    }
    else
    {
        Lock_SempPost(Port->MTC_Semp);
        baktime = (u32)DjyGetSysTime();
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

