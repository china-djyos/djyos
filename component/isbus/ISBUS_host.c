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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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

//================================================================================
//组件名称：Host_ISBUS
//组件描述：工业串行总线（Industrial Serial Bus）通信模块主机端部分
//开发版本：V1.00
//开发人员:YT
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <device.h>
#include <dbug.h>
//#include <driver.h>
#include <systime.h>
#include "ISBUS.h"
#include "ISBUS_Protocol.h"
#include "component_config_isbus.h"

struct IM_Pkg
{
    struct IM_Pkg *next;
    u8 *IM_buf;
    struct SemaphoreLCB *IM_Semp;           //用于通知发送完成。
};

//从机描述
struct SlaveList                            //从机列表
{
    struct Host_ISBUSPort *CommPort;        //对应的通信口指针
    u8  Address;                            //从机地址
    u8  Errors;                             //通信出错次数，出错累加，正常一次即清零
    struct SlaveList *Next;                 //将从机组成一个循环链表，末端指向表头
};


//通信端口的描述
struct Host_ISBUSPort
{
    struct Host_ISBUSPort *Next;            //组成单向链表，末端指向NULL
    struct ISBUS_FunctionSocket *SocketHead; //组成单向循环链表
    struct ISBUS_FunctionSocket *SocketCurrent; //指向当前接收的功能号值
    struct SlaveList* SlaveHead;            //从机队列头，单向循环链表
    struct SlaveList* SlaveTail;            //从机队列尾
    struct SlaveList* SlaveCurrent;         //从机队列当前轮询
    struct SemaphoreLCB *PortSemp;          //保护待发送的消息队列，确保数据包完整性
    struct SemaphoreLCB *PollSemp;          //保护轮询队列，用于通知发送完成。
    s32 SerialDevice;                       //对应的设备
    ISBUS_FntProtocolError fnError;         //出错对应的回调函数
    u32 ErrorPkgs;                          //累计错误数
    u32 ErrorLast;                          //最后一次错误号
    u32 Timeout;                            //超时时间
    u32 Resttime;                           //离触发超时剩余时间
    s32 SendTimes;                          //发送buffer中的数据发送次数，-1表示无限次数
    u32 PollCycle;                          //轮询周期，uS，含义与 PollModel 有关
//  u8  EchoModel;                          //当前应答模式， ONE_BY_ONE 等
    u8  PollModel;                          //轮询模式，取值 CN_POLL_FASTEST 等
    u8  PkgSerial;                          //当前发送包序号
    u8  SlaveNum;                           //从机数量
    u8  FaultNum;                           //通信失效的从机数量
    s16 analyzeoff;                         //解包指针
    s16 recvoff;                            //当前接收指针
    u8 *ResendPkgBuf;                       //发包指针备份，用于重发
    u8 *PollSendPkgBuf;                     //发送数据包指针（包含协议头）
    struct IM_Pkg *IM_Head;                 //立即发送包队列头
    struct IM_Pkg *IM_Tail;                 //立即发送包队列尾
    u8 *RecvPkgBuf;                         //接收数据包指针（包含协议头）
//  u8 MTC_Address;                         //本端口接受的组播地址，若不接受组播
//                                          //可设为CN_MTC_RESV（默认）
    u16 PortMaxRecvLen;                     //本端口最大接收包长度，包含协议头
    u16 PortMaxSendLen;                     //本端口最大发送包长度，不含协议头
//  u16 PortSendLen;                        //本端口正在发送的包长度，不含协议头
//    u16 RecvP;                              //接收偏移量，不含协议头
//    u32 SendP;                              //发送偏移量，！！！含协议头
};

struct Host_ISBUSPort *sg_ptHostPortHead = NULL; //主机端链表初始化

struct SemaphoreLCB *UartTimerSemp  = NULL; //信号量
static u16 sg_ptHostEvtt;
u8 COUNT_Slave;
u8 Effective_Slave=0;
u8 g_TimeOut=0;  //超时标志位
bool_t __HostSendPkg(struct Host_ISBUSPort *Port, u8 resend, u8 *dst);


struct ISBUS_FunctionSocket * __Host_GetProtocol(struct Host_ISBUSPort *Port,u8 Protocol)
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

//-----------------------------------------------------------------------------
//功能：点对点方式接收并处理数据包
//参数：Port，接收数据包的端口
//      src，接收数据包的源地址，点播地址
//返回：true= 成功，false=  失败
//-----------------------------------------------------------------------------
bool_t __ISBUS_UniProcess(struct Host_ISBUSPort *Port,u8 src)
{
    struct ISBUS_FunctionSocket *Me;
    struct ISBUS_Protocol protohead;
    u32 starttime;
    s32 DevRe = Port->SerialDevice;
    u8 *protobuf;
    u8 chk,len;
    s16 restlen,Completed,readed,startoffset,tmp,tmp1;
    s16 exdata;

    bool_t Gethead = false;

    if(src >= CN_INS_MULTICAST)
        return false;
    protobuf = Port->RecvPkgBuf;
    startoffset = Port->analyzeoff;
    readed = Port->recvoff;
    starttime = (u32)DJY_GetSysTime();
    while(1)
    {
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
        tmp = Device_Read(DevRe, &protobuf[readed], 256+sizeof(struct ISBUS_Protocol) - readed, 0, Port->Timeout);
        if(tmp != 0)
        {
            if(debug_ctrl ==true)
            {
                printf("\r\nhost recv:");
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
        if(Gethead && (readed - (s16)startoffset >= (s16)sizeof(struct ISBUS_Protocol)))
        {
            if((protobuf[startoffset + CN_OFF_DST] != 0)     //只收发给主机的包，主机地址总是0
                ||(protobuf[startoffset + CN_OFF_SRC] != src))  //校验源地址
            {
                Gethead = false;
                startoffset++;
                continue;       //startoffset不变，while循环中,从当前位置重新寻找 0xEB
            }
            else
            {
                chk = 0xEB + 0 + protobuf[startoffset + CN_OFF_PROTO]
                               + src
                               + protobuf[startoffset + CN_OFF_SERIAL]
                               + protobuf[startoffset + CN_OFF_LEN];    //计算chk
                if(chk == protobuf[startoffset + CN_OFF_CHKSUM])
                {
                    break;       //找到合法的协议头，退出循环，继续收数据包
                }
                else
                {
                    if(Port->fnError != NULL)
                        Port->fnError((void*)Port, CN_INS_CHKSUM_ERR, src);
                    Port->ErrorLast = CN_INS_CHKSUM_ERR;
                    Port->ErrorPkgs++;
                    startoffset++;
                    Gethead = false;
                    printf("\r\nhost chksum error:");
                    continue;       //startoffset不变，while循环中,从当前位置重新寻找 0xEB
                }
            }
            break;
        }
        else        //一个超时周期过去了，没收到完整的协议头，肯定超时了。
                    //此处与从机不一样，从机只在收到部分包时判超时，因为从机时被动接收。
                    //主机是发包后等待从机响应，从机必须立即响应。
        {
            if(((u32)DJY_GetSysTime() - starttime) > Port->Timeout)
            {
                if(Port->fnError != NULL)
                    Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR, src);
                Port->ErrorLast = CN_INS_TIMEROUT_ERR;
                Port->ErrorPkgs++;
                Port->analyzeoff = 0;
                Port->recvoff = 0;
                printf("\r\nhost protocol timeout,%d  %d",Gethead,tmp);
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
        while(((u32)DJY_GetSysTime() - starttime) < Port->Timeout)
        {
            tmp = Device_Read(DevRe, &protobuf[readed],restlen, 0, Port->Timeout);
            Completed += tmp;
            readed += tmp;
            if(Completed >= restlen)
                break;
        }
        exdata = Completed - restlen;
    }
    else
        exdata = 0 - restlen;


    if(exdata >= 0)
    {
        Me = __Host_GetProtocol(Port, protohead.Protocol);
        if(Me != NULL)
        {
            if(Me->MyProcess != NULL)
                Me->MyProcess(Me,src,protobuf+startoffset+sizeof(struct ISBUS_Protocol),len);
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
        return true;
    }
    else
    {
        if(Port->fnError != NULL)
            Port->fnError((void*)Port, CN_INS_TIMEROUT_ERR, src);
        Port->ErrorLast = CN_INS_TIMEROUT_ERR;
        Port->ErrorPkgs++;
        Port->analyzeoff = 0;
        Port->recvoff = 0;
        printf("\r\nhost data timeout");
        return false;
    }
}
//返回值：true = 需要重发；false = 不需要重发
bool_t __ISBUS_RecordSlaveError(struct SlaveList *slave, bool_t error)
{
    struct Host_ISBUSPort *Port = slave->CommPort;
    bool_t resent = false;

    if(!error)
    {
        if((slave->Errors >= 3) && (Port->FaultNum != 0))
        {
            Port->FaultNum--;
            if(Port->fnError != NULL)
                Port->fnError((void *)Port, CN_INS_SLAVE_RECOVER, slave->Address);
        }
        slave->Errors = 0;
    }
    else
    {
        if(slave->Errors != 255)
        {
            slave->Errors++;
            if(slave->Errors < 3)
                resent = true;
            else if(slave->Errors == 3)
            {
                //出错次数达到3的时候，用回调函数通知APP。
                resent = false;
                Port->FaultNum++;
                if(Port->fnError != NULL)
                    Port->fnError((void*)Port, CN_INS_SLAVE_FAULT, slave->Address);
            }
        }
    }
    return resent;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
//返回值：true = 需要重发；false = 不需要重发
bool_t __ISBUS_BroadcastProcess(struct Host_ISBUSPort *Port,u8 src)
{
    u8 slaveaddr;
    bool_t err,resent = false;
    struct SlaveList *Current = Port->SlaveHead;
    if(Current == NULL)
        return resent;
    do
    {
        slaveaddr = Current->Address;
        err = __ISBUS_UniProcess(Port, slaveaddr);
        if(__ISBUS_RecordSlaveError(Current, ! err))
            resent = true;
        Current = Current->Next;
    }while(Current != Port->SlaveHead);
    return resent;
}

bool_t __ISBUS_MulticastProcess(struct Host_ISBUSPort *Port,u8 src)
{
    return true;
}
#pragma GCC diagnostic pop

// ============================================================================
// 函数功能：主机端协议处理函数
// 输入参数：  ReplyModel，应答模式。取值如下
//                   ONE_BY_ONE ，主从机一问一答模式。
//                   BOOADCAST_MODEL，主从机一问多答模式，提高通讯效率。广播模式
//                   MULTICAST_MODEL，主从机一问多答模式，提高通讯效率。组播模式
//        PoolModel，定时轮询模式，取值如下
//                   NO_TIMER_POLL，定时周期不做要求，各个从机有多快就多快回复；
//                   EVENLY_SPACED_POLL，等间隔周期轮询各个从机；
//                   ACCUMU_TIMER_POLL，总间隔周期轮询所有从机。
//        PoolTimes，轮询次数，取 UNLIMITED_NUMBER 时为周而复始地轮询
//        Protocol，功能号
//        GroupModel，被轮询的从机集，全部或部分组，可由用户配置
//                   GroupModel取ALL_GROUP_POLL时全部轮询，其余数值为具体的组号
//        SlaveNum，从机数量。
//        Send_Buff，待发送的数据包，不含协议头
//        len，发送数据包长度
// 返回值：  无
// ============================================================================
ptu32_t ISBUS_HostProcess(void)
{
    struct Host_ISBUSPort *Port;
    s64 Polltime;
    u8 dst;
    bool_t send,resend = false;
    DJY_GetEventPara((ptu32_t*)&Port, NULL);
    Polltime = DJY_GetSysTime();
    while(1)
    {
        send = __HostSendPkg(Port, resend, &dst);   //dst用于提取发送包的dst地址
        if(send == true)
        {
            if(dst < CN_INS_MULTICAST)          //点对点通信,一应一答模式
            {
                resend = __ISBUS_UniProcess(Port,dst);          //接收并处理数据包
                resend = __ISBUS_RecordSlaveError(Port->SlaveCurrent, ! resend);
                //注：快速轮询直接继续执行下一个while循环，无须单独处理
                if(Port->PollModel == CN_POLL_SAME_INTERVAL)   //等间隔轮询
                {
                    Polltime += Port->PollCycle;        //此时PollCycle代表查询每个子机的间隔时间
                    DJY_EventDelayTo(Polltime);
                }
                else if(Port->PollModel == CN_POLL_SAME_CYCLE)         //等周期轮询
                {
                    if(dst == Port->SlaveTail->Address)
                    {
                        Polltime += Port->PollCycle;    //此时PollCycle代表轮询一个周期的时间
                        DJY_EventDelayTo(Polltime);
                    }
                }
            }
            else       //组播/广播地址，组播号内的从机依序上传
            {
                resend = __ISBUS_BroadcastProcess(Port,dst);
                if(Port->PollModel == CN_POLL_SAME_CYCLE)         //等周期轮询
                {
                    Polltime += Port->PollCycle;    //此时PollCycle代表轮询一个周期的时间
                    DJY_EventDelayTo(Polltime);
                }
            }
        }
        else
        {
            //创建port时，事件立即启动，此时pollcycle = 0，又没有数据包要发送，导致事件100%占用CPU
            //运行中隐患：删掉了发送数据包，且pollcycle被设置为0，会导致事件100%占用CPU。
            if(Port->PollCycle < 20000)
                DJY_EventDelay(20000);
            else
                DJY_EventDelay(Port->PollCycle);
            resend = false;
        }
    }
    return 0;
}

// ============================================================================
// 函数功能：注册模块使用的通信端口，事实上，不是串口也可以注册的。
//        通信端口必须按djyos的设备驱动框架来驱动。主机端。
// 输入参数：dev，通信端口的设备指针
//        fnError，通信错误对应的回调函数，不需要则给NULL
//        Timeout，长时间没有收到数据的超时告警门限，uS为单位，单时间精度受
//                操作系统tick间隔限制。0xffffffff（-1）代表不设超时。
// 返回值：  通信插口指针
// ============================================================================
struct Host_ISBUSPort *ISBUS_HostRegistPort(char *dev,
                                ISBUS_FntProtocolError fnError,u32 Timeout)
{
    struct SemaphoreLCB *Semp1,*Semp2;
    struct Host_ISBUSPort *Port;
    u8 *recvbuf;
    s32 devfd;
    devfd = open(dev, O_RDWR);
    if(devfd == -1)
        return NULL;
    Port = (struct Host_ISBUSPort *)malloc(sizeof(struct Host_ISBUSPort ));
    recvbuf = malloc(3*(256+sizeof(struct ISBUS_Protocol)));
    Semp1 = Lock_SempCreate(1, 1, CN_BLOCK_FIFO, "ISBUS_Host");
    Semp2 = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, "ISBUS_Poll");
    if((Port != NULL) && (recvbuf != NULL)&&(Semp1 != NULL)&&(Semp2 != NULL)) //分配成功
    {
        memset(Port, 0, sizeof(struct Host_ISBUSPort ));
        Port->PortSemp = Semp1;
        Port->PollSemp = Semp2;
        if(sg_ptHostPortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = sg_ptHostPortHead;
        }
        sg_ptHostPortHead = Port;
        Port->SocketHead = NULL;
        Port->SocketCurrent = NULL;
        Port->SerialDevice = devfd;
        Port->fnError = fnError;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->RecvPkgBuf = recvbuf;
        Port->PollSendPkgBuf = Port->RecvPkgBuf + 256 + sizeof(struct ISBUS_Protocol);
        Port->ResendPkgBuf = Port->PollSendPkgBuf + 256 + sizeof(struct ISBUS_Protocol);
        Port->PortMaxRecvLen = sizeof(struct ISBUS_Protocol);
        Port->PortMaxSendLen = sizeof(struct ISBUS_Protocol);
//      Port->PortSendLen = 0;
//        Port->RecvP = 0;
        //若置0，初始状态会发送一包数据。
//        Port->SendP = sizeof(struct ISBUS_Protocol);  //发送偏移量为协议头
//      Port->MTC_Address = CN_INS_MTC_INVALID;
        Device_SetUserTag(devfd, (ptu32_t)Port);
//        Multiplex_AddObject(Port->MultiplexPort, devfd,
//                                           CN_MULTIPLEX_SENSINGBIT_READ
////                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //对于主机端，可写不可用
//                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                        |  CN_MULTIPLEX_SENSINGBIT_ET
//                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
        DJY_EventPop(sg_ptHostEvtt, NULL, 0, (ptu32_t)Port, 0, 0);
    }
    else
    {
        free(Port);
        free(recvbuf);
        Lock_SempDelete(Semp1);
        Lock_SempDelete(Semp2);
    }
    return Port;
}

// ============================================================================
// 函数功能：注册通信协议。主机端。
// 输入参数：Port，通信插口指针，为INS_RegistPort函数的返回值
//        Protocol，功能号，看 CN_INS_PROTOCOL_DEFAULT 等常数定义
//        MaxRecvLen，本协议接收数据包的最大长度
//        MaxSendLen，本协议发送数据包的最大长度
//        fn，接收数据包的协议处理函数
// 返回值：  协议插口指针
// ============================================================================
struct ISBUS_FunctionSocket *ISBUS_HostRegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                          u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn)
{
    struct ISBUS_FunctionSocket *ProtocolSocket;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct ISBUS_FunctionSocket *)malloc(sizeof(struct Host_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result用于出错判断，不能用指针是否NULL，因为有单工工作的通信口，Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen - sizeof(struct ISBUS_Protocol))
        {
//          Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //接收数据包指针（不含协议头）
//                                      (MaxRecvLen+ sizeof(struct ISBUS_Protocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct ISBUS_Protocol));
//          result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen - sizeof(struct ISBUS_Protocol)) //发送部分
        {
//          Port->PollSendPkgBuf = (u8*)realloc(Port->PollSendPkgBuf,
//                                     MaxSendLen + sizeof(struct ISBUS_Protocol));
            Port->PortMaxSendLen = MaxSendLen + sizeof(struct ISBUS_Protocol);
//          result |= (Port->RecvPkgBuf != NULL);
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
        debug_printf("ISBUS","ISBUS_HostRegistProtocol failure");
//      free(Port->RecvPkgBuf);
//      free(ProtocolSocket);
//      free(Port->PollSendPkgBuf);
//      return NULL;
    }
    return ProtocolSocket;
}
// ============================================================================
//功能：数据包发送函数，把发送缓冲区中的数据发送出去，如果是即时命令，直接发送出去；
//      如果是轮询命令，且dst是点播地址，则更换目的地址再发送。
//参数：Port，发送数据的端口
//      dst，返回发送的目的地址
//返回值： 发送的目的地址，用于调用方决定下一步接收过程。
// ============================================================================
bool_t __HostSendPkg(struct Host_ISBUSPort *Port, u8 resend, u8 *dst)
{
    u8 *SendBuf;
    bool_t result;
    bool_t AllError = false;
    struct IM_Pkg *me;
    u8 SendLen,tmp;
    if(Port->FaultNum >= Port->SlaveNum)
        return false;
    if(resend)
    {
        SendBuf = Port->ResendPkgBuf;
//        tmp = SendBuf[CN_OFF_SERIAL];
//        SendBuf[CN_OFF_SERIAL] = Port->PkgSerial;
//        SendBuf[CN_OFF_CHKSUM] += Port->PkgSerial - tmp;
//        Port->PkgSerial++;
        SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
        Device_Write(Port->SerialDevice, SendBuf, SendLen,0,0);
        *dst = SendBuf[CN_OFF_DST];
//        printf("\r\nhost resend:");
//        for(tmp = 0;tmp < SendLen;tmp++)
//        {
//            printf("%02x ",SendBuf[tmp]);
//        }
        result = true;
    }
    else
    {
        Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
        if(Port->IM_Head != NULL)       //发送即时消息
        {
                me = Port->IM_Head;
                if(Port->IM_Head == Port->IM_Tail)
                {
                    Port->IM_Tail = NULL;
                    Port->IM_Head = NULL;
                }
                else
                {
                    Port->IM_Head = Port->IM_Head->next;
                }
                SendBuf = me->IM_buf;
                tmp = SendBuf[CN_OFF_SERIAL];
                SendBuf[CN_OFF_SERIAL] = Port->PkgSerial;
                SendBuf[CN_OFF_CHKSUM] += Port->PkgSerial - tmp;
                Port->PkgSerial++;
                SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
                Device_Write(Port->SerialDevice, SendBuf, SendLen,0,0);
                if((debug_ctrl ==true))
                {
                    printf("\r\nhost send:");
                    for(tmp = 0;tmp < SendLen;tmp++)
                    {
                        printf("%02x ",SendBuf[tmp]);
                    }
                }
                *dst = SendBuf[CN_OFF_DST];
                result = true;
                memcpy(Port->ResendPkgBuf,SendBuf,SendLen);
                Lock_SempPost(me->IM_Semp);
                free(me);   //对应的 malloc 函数在 ISBUS_HostSetIM_Pkg 函数中
        }
        else                            //发送周期性轮询消息
        {
            if((Port->SendTimes != 0) && (Port->SlaveHead != NULL))
            {
                SendBuf = Port->PollSendPkgBuf;
                if(SendBuf[CN_OFF_DST] < CN_INS_MULTICAST)
                {
                    struct SlaveList *end;
                    Port->SlaveCurrent = Port->SlaveCurrent->Next;
                    end = Port->SlaveCurrent;
                    while(Port->SlaveCurrent->Errors >= 3)
                    {
                        Port->SlaveCurrent = Port->SlaveCurrent->Next;
                        if(Port->SlaveCurrent == end)
                        {
                            AllError = true;
                            break;
                        }
                    }
                    SendBuf[CN_OFF_DST] = Port->SlaveCurrent->Address;
                    if((Port->SlaveCurrent == Port->SlaveTail) && (Port->SendTimes != -1))
                    {
                        Port->SendTimes--;
                        if(Port->SendTimes == 0)
                            Lock_SempPost(Port->PollSemp);
                    }
                }
                else
                {
                    if(Port->SendTimes != -1)
                    {
                        Port->SendTimes--;
                        if(Port->SendTimes == 0)
                            Lock_SempPost(Port->PollSemp);
                    }
                }
                if(AllError == false)
                {
                    SendBuf[CN_OFF_SERIAL] = Port->PkgSerial;
                    SendBuf[CN_OFF_CHKSUM]  = 0xEB + SendBuf[CN_OFF_DST]  + SendBuf[CN_OFF_PROTO]
                                              + SendBuf[CN_OFF_SRC] + SendBuf[CN_OFF_SERIAL] + SendBuf[CN_OFF_LEN];
                    Port->PkgSerial++;
                    *dst = SendBuf[CN_OFF_DST];
                    SendLen = SendBuf[CN_OFF_LEN] + sizeof(struct ISBUS_Protocol);
                    Device_Write(Port->SerialDevice, SendBuf, SendLen,0,0);
                    if((debug_ctrl ==true))
                    {
                        printf("\r\nhost send:");
                        for(tmp = 0;tmp < SendLen;tmp++)
                        {
                            printf("%02x ",SendBuf[tmp]);
                        }
                    }
                    result = true;
                    memcpy(Port->ResendPkgBuf,SendBuf,SendLen);
                }
                else
                {
                    *dst = 0;
                    result = false;
                }
            }
            else
            {
                *dst = 0;
                result = false;
            }
        }
        Lock_SempPost(Port->PortSemp);
    }

    return result;
}

// ============================================================================
//功能：设置轮询数据包，但不发送，待轮到自己发送时将发送。发送时，将根据扫描到的从机
//      地址表依次改变目的地址。仅主机端需要这个API。
//参数：Host_FunctionSocket，通信协议插口指针，为ISBUS_HostRegistProtocol函数的返回值
//      dst，目的地址，可以是广播、组播地址，如果是点播地址，将被忽略，自动改为从机
//           列表中的第一个从机地址。
//      buf，待发送的数据包，不含协议头
//      len，发送长度
//      times，发送次数（若为点播，则所有从机都发送一遍为一次）
//      Timeout等待发送完成的时限，，若times = -1，本参数将被忽略。
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 ISBUS_SetPollPkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                        u8 *buf, u8 len, s32 times, u32 Timeout)
{
    struct Host_ISBUSPort *Port;
    u8 *SendBuf;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
    if(Port->SlaveHead == NULL)
        return 0;
    Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
    Port->SendTimes = times;
    SendBuf = Port->PollSendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    if(dst < CN_INS_MULTICAST)
        SendBuf[CN_OFF_DST]     = Port->SlaveHead->Address;
    else
        SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = 0;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_SERIAL] = Port->PkgSerial;   //包序号不修改
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + SendBuf[CN_OFF_DST] + SendBuf[CN_OFF_PROTO]
                              + len + SendBuf[CN_OFF_SERIAL];
    memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);
    Lock_SempPend(Port->PollSemp, 0);   //解释一下：如果上一次调用本函数时，参数
                                        //Timeout=0，发送完成后 PollSemp 将处于
                                        //有信号状态，将使后续等待失效。
                                        //本行须在 PortSemp 之前调用才严谨。
    Lock_SempPost(Port->PortSemp);

    if((Timeout != 0) && (times != -1))
    {
        Lock_SempPend(Port->PollSemp, Timeout);
    }
    return len;
}

// ============================================================================
//功能：设置即时发送数据包，但不发送，待轮到自己发送时将发送。
//参数：ISBUS_FunctionSocket，通信协议插口指针，为ISBUS_HostRegistProtocol函数的返回值
//      dst，目的地址，对于主机端：目的地址为从机地址，原地址为 0；
//           对于从机：目的地址为主机地址，源地址为本板地址
//      buf，待发送的数据包，不含协议头
//      len，发送长度
//      Timeout，等待发送完成的时限，若不等待，填 0 即可。
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 ISBUS_HostSetIM_Pkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                            u8 *buf, u8 len, u32 Timeout)
{
    struct Host_ISBUSPort *Port;
    struct SlaveList *current;
    struct IM_Pkg *mypkg;
    u8 *SendBuf;
    bool_t found = false;
    if(ISBUS_FunctionSocket == NULL)
        return 0;
    if(len > ISBUS_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = ISBUS_FunctionSocket->CommPort;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        if(dst < CN_INS_MULTICAST)      //组播、广播时可以发送的
        {
            do
            {
                if(current->Address == dst)
                {
                    found = true;
                    break;
                }
                else
                    current = current->Next;
            }while(current != Port->SlaveHead);
            if( ! found )                  //从机不存在，拒绝发送
                return 0;
        }
    }
    //对应的 free 函数在 __HostSendPkg 函数中
    mypkg = malloc(sizeof(struct IM_Pkg)+len+sizeof(struct ISBUS_Protocol));
    if(mypkg != NULL)
    {
        if(Timeout != 0)
        {
            mypkg->IM_Semp = Lock_SempCreate(1, 0, CN_BLOCK_FIFO, NULL);
            if(mypkg->IM_Semp == NULL)
                return 0;
        }
        else
            mypkg->IM_Semp = NULL;
        Lock_SempPend(Port->PortSemp,CN_TIMEOUT_FOREVER);
        mypkg->IM_buf = (u8*)(mypkg+1);
        if(Port->IM_Tail == NULL)
        {
            Port->IM_Head = mypkg;
            Port->IM_Tail = mypkg;
        }
        else
        {
            Port->IM_Tail->next = mypkg;
            Port->IM_Tail = mypkg;
        }
        mypkg->next = NULL;

//      Port->PortSendLen = len;    //不含协议头
        SendBuf = mypkg->IM_buf;
        SendBuf[CN_OFF_START]   = 0xEB;
        SendBuf[CN_OFF_DST]     = dst;
        SendBuf[CN_OFF_PROTO]   = ISBUS_FunctionSocket->Protocol;
        SendBuf[CN_OFF_SRC]     = 0;
        SendBuf[CN_OFF_LEN]     = len;
        SendBuf[CN_OFF_SERIAL] = Port->PkgSerial;   //包序号不修改
        SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + SendBuf[CN_OFF_PROTO] + 0
                                  + len + SendBuf[CN_OFF_SERIAL];
        memcpy(SendBuf + sizeof(struct ISBUS_Protocol), buf, len);
        Lock_SempPost(Port->PortSemp);
        if(Timeout != 0)
        {
            Lock_SempPend(mypkg->IM_Semp, Timeout);
        }
    }
    else
        len = 0;
    return len;
}

void __ISBUS_SentChkSlave(struct Host_ISBUSPort *Port,u8 dst)
{
    u8 SendBuf[sizeof(struct ISBUS_Protocol)];
//  SendBuf = Port->PollSendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = CN_CHK_SLAVE;
    SendBuf[CN_OFF_SRC]     = 0;
    SendBuf[CN_OFF_LEN]     = 0;
    SendBuf[CN_OFF_SERIAL]  = Port->PkgSerial;
    Port->PkgSerial++;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + SendBuf[CN_OFF_PROTO] + SendBuf[CN_OFF_SERIAL];
    Device_Write(Port->SerialDevice, SendBuf, sizeof(struct ISBUS_Protocol),0,0);
    return ;
}

// ============================================================================
// 函数功能：定时轮询周期配置。
//       说明：配置轮询周期前需了解从机特性，各个从机超时时间及处理回复时间，对于ACCUMU_TIMER_POLL总间隔周
//           期轮询模式，总的一圈的轮询周期为各个从机超时时间加上所配置额外的定时时间。
// 输入参数：timercycle，定时周期，单位us。NO_TIMER_POLL模式下取NULL.
//        PoolModel，轮询模式，CN_POLL_FASTEST      ，定时周期不做要求，有多快就轮询多快
//                            CN_POLL_SAME_INTERVAL，等间隔周期轮询各个从机 ；
//                            CN_POLL_SAME_CYCLE   ，等周期轮询所有从机
// 返回值：无
// ============================================================================
void ISBUS_PollConfig(struct Host_ISBUSPort *Port,u32 PollCycle,u8 PollModel)
{
    if(Port != NULL)
    {
        Port->PollModel = PollModel;
        Port->PollCycle = PollCycle;
    }
    return;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// ============================================================================
// 功能：扫描子机的功能函数，添加从机并设置参数。
// 输入参数：InSerSocket，功能槽号
//        src，从机地址
//        buf，接收到的协议数据包，本功能槽无用
//        len，数据包长度，无用
// 返回值：从机结构体描述指针
// ============================================================================
void __ISBUS_AddSlave(struct ISBUS_FunctionSocket  *InSerSocket , u8 src, u8 *buf, u32 len)
{
    struct Host_ISBUSPort *Port;
    Port = InSerSocket->CommPort;
    ISBUS_AddSlave(Port, src);
}
#pragma GCC diagnostic pop

//-----------------------------------------------------------------------------
//功能：添加从机，从机构成单向循环链表，该链表按从机地址大小排序，Port->SlaveHead指向
//      地址最小的从机。
//参数：Port，新从机添加到本端口中
//      address，新从机的地址
//返回：无
//-----------------------------------------------------------------------------
void ISBUS_AddSlave(struct Host_ISBUSPort *Port, u8 address)
{
    struct SlaveList *p1;
    struct SlaveList* current, *pre = NULL;
    if(Port == NULL)
        return ;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        do
        {
            if(current->Address == address)
                return;     //从机已经存在
            else if(current->Address > address)
                break;
            else
            {
                pre = current;
                current = current->Next;
            }
        }while(current != Port->SlaveHead);
    }
    Port->SlaveNum++;
    p1=(struct SlaveList*)malloc(sizeof(struct SlaveList));//申请新节点
    if(p1==NULL)
    {
        debug_printf("ISBUS","内存申请失败\r\n");
    }
    p1->CommPort = Port;
    p1->Address = address;
    p1->Errors = 0;
//    p1->Timeout = CN_LIMIT_UINT32;
    if(current == NULL)             //尚未添加从机
    {
        p1->Next = p1;
        Port->SlaveHead = p1;
        Port->SlaveCurrent = p1;
        Port->SlaveTail = p1;
    }
    else if(pre == NULL)    //current指向队列头，表明新从机地址小于队列头
    {
        p1->Next = current;
        Port->SlaveTail->Next = p1;
        Port->SlaveHead = p1;
    }
    else if(pre == current)     //只有一个从机，且地址小于新从机
    {
        p1->Next = current;
        current->Next = p1;
        Port->SlaveTail = p1;
    }else                       //有多个从机
    {
        p1->Next = current;
        pre->Next = p1;
        if(address > Port->SlaveTail->Address)  //新从机地址大于队列尾部
            Port->SlaveTail = p1;
    }
}

// ============================================================================
// 功能：获取从机地址列表，ISBUS模块扫描所有可能的地址。
// 参数：Port，待扫描的端口
//      MaxAddress，最大地址编号
// 返回值：从机列表指针
// 说明：如果主机上电立即扫描，从机可能还没上电，提供一个接口，应用程序调时才扫描
// ============================================================================
struct SlaveList* ISBUS_ScanSlave(struct Host_ISBUSPort *Port,u8 MinAddress,u8 MaxAddress)
{
    u8 SlaveAddress = MinAddress;
    ISBUS_HostRegistProtocol(Port, CN_CHK_SLAVE, 0, 0, __ISBUS_AddSlave);
    while(1)
    {
        __ISBUS_SentChkSlave(Port,SlaveAddress);
        __ISBUS_UniProcess(Port,SlaveAddress);
        SlaveAddress++;
        if(SlaveAddress > MaxAddress)
            break;
    }
    Port->SlaveCurrent = Port->SlaveTail;
    Port->ErrorLast = 0;
    Port->ErrorPkgs = 0;
    return Port->SlaveHead;
}

//-----------------------------------------------------------------------------
//功能：获取从机列表，数组形式。
//参数：Port，目标端口
//      address，保存从机地址表的buffer，如果是NULL，则只返回从机数量
//返回：从机数量
//用法：可先以 address = NULL调用，获得数量后，再分配内存，再次调用本函数。
//-----------------------------------------------------------------------------
u8 ISBUS_GetSlaveTable(struct Host_ISBUSPort *Port,u8 *address)
{
    struct SlaveList* current;
    u8 result = 0;
    if(Port == NULL)
        return 0;
    current = Port->SlaveHead;
    if(current != NULL)
    {
        do
        {
            if(address != NULL)
                address[result] = current->Address;
            current = current->Next;
            result++;
       }while(current != Port->SlaveHead);
    }
    return result;
}

// ============================================================================
// 功能：发送从机地址列表
// 参数：Port，待发送的端口
//      address，地址表指针
//      num，从机数量
// 返回值：无
// 说明：
// ============================================================================
void ISBUS_SendSlaveTable(struct Host_ISBUSPort *Port,u8 *address, u8 num)
{
    struct ISBUS_FunctionSocket *sock;
    sock = ISBUS_HostRegistProtocol(Port, CN_SET_SLAVE_TABLE, 0, 127, NULL);
    ISBUS_SetPollPkg(sock,0, address,num, 1,1000000); //对于轮询包，除广播地址外，其他目的地址并无意义。
}

// ============================================================================
// 功能：从已创建的从机中删除指定地址从机。
// 参数：pHead，从机结构体描述指针
//      address，被删除的从机地址
// 返回： 无
// ============================================================================
void ISBUS_DeleteSlave (struct Host_ISBUSPort * Port, u8 address)
{
    struct SlaveList* current,*pre;

    if(Port == NULL)
        return ;
    current = Port->SlaveHead;
    pre = Port->SlaveTail;
    while(current != NULL)
    {
        if(address == current->Address)
        {
            if(current != current->Next)
            {
                pre->Next = current->Next;
                if(Port->SlaveHead == current)
                    Port->SlaveHead = current->Next;
                if(Port->SlaveCurrent == current)
                    Port->SlaveCurrent = current->Next;
                if(Port->SlaveTail == current)
                    Port->SlaveTail = current->Next;
            }
            else
            {
                Port->SlaveHead = NULL;
                Port->SlaveCurrent = NULL;
                Port->SlaveTail = NULL;
            }
            free(current);
            break;
        }
        pre = current;
        current = current->Next;
    }
    return;
}

//-----------------------------------------------------------------------------
//功能：主机端初始化
//参数：HostStackSize，应用程序注册的回调函数所需要的栈空间
//返回：无
//-----------------------------------------------------------------------------
void ISBUS_HostInit(u32 HostStackSize)
{

    sg_ptHostEvtt = DJY_EvttRegist(EN_INDEPENDENCE, CN_PRIO_REAL, 1, 1, ISBUS_HostProcess,
                            NULL, HostStackSize, "ISBUS host");
    if(sg_ptHostEvtt == CN_EVTT_ID_INVALID)
    {
        debug_printf("ISBUS","ISBUS通信模块主机端初始化异常\n\r");
        while(1);                               //初始化阶段就异常，直接死循环
    }

}

void ISBUS_Host_SetTimeOut(struct Host_ISBUSPort *port, u32 timeout)
{
    port->Timeout = timeout;
}

