//================================================================================
//组件名称：serialcomm
//组件描述：装置内部串口通信的网络链路层
//       本模块包含主机端与从机端
//开发版本：V1.00
//开发人员: 罗侍田
//================================================================================
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <djyos.h>
#include <object.h>
#include <lock.h>
#include <multiplex.h>
#include <driver.h>
#include <systime.h>
#include "serialcomm.h"
#include "timer_hard.h"

#define CN_OFF_START        0
#define CN_OFF_DST          1
#define CN_OFF_PROTO        2
#define CN_OFF_SRC          3
#define CN_OFF_LENL         4
#define CN_OFF_LENH         5
#define CN_OFF_CHKSUM       6
#define CN_OFF_USER         7

//协议包头，7字节
struct InSerProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //目标地址
    u8 Protocol;        //协议号
    u8 SrcAddress;      //源地址
    u8 LenL;            //数据包长度低字节
    u8 LenH;            //数据包长度高字节
    u8 ChkSum;          //校验和，即前6个成员的和。
};

//通信端口的描述
struct InSerSocketPort
{
    struct InSerSocketPort *Next;           //组成单向链表，末端指向NULL
    struct InSerSocket *SocketHead;         //组成单向循环链表，Head指向当前接收
                                            //的协议插槽
    struct DjyDevice *SerialDevice;         //对应的设备指针
    fntProtocolError fnError;               //出错对应的回调函数
//    u8 Recving[sizeof(struct InSerProtocol)];//用于解码协议头。
    u32 ErrorPkgs;                          //累计错误数
    u32 ErrorLast;                          //最后一次错误号
    u32 Timeout;                            //超时时间
    u32 Resttime;                           //离触发超时剩余时间
    u8  fsm;                                //当前协议接收状态
    u32 relen;                              //检查偏移量
    u8 *SendPkgBuf;                         //发送数据包指针（包含协议头）
    u8 *RecvPkgBuf;                         //接收数据包指针（包含协议头）
    u16 PortMaxRecvLen;                     //本端口最大接收包长度，包含协议头
    u16 PortMaxSendLen;                     //本端口最大发送包长度，不含协议头
    u16 PortSendLen;                        //本端口正在发送的包长度，不含协议头
    u16 RecvP;                              //接收偏移量，不含协议头
    u32 SendP;                              //发送偏移量，！！！含协议头
    u8 MTC_Address;                         //本端口接受的组播地址，若不接受组播
                                            //可设为CN_MTC_RESV（默认）
};

//通信协议的描述
struct InSerSocket
{
    struct InSerSocket *Next;               //组成一个链表，末端指向NULL
    u8 Protocol;                            //协议号
    u16 ProtocolRecvLen;                    //本协议最大接收包长度
    u16 ProtocolSendLen;                    //本协议最大发送包长度
    struct InSerSocketPort *CommPort;       //对应的通信口指针
    fntProtocolProcess MyProcess;           //协议处理函数指针
};

ptu32_t uartTimerHandle;

struct MultiplexSetsCB * Master_sg_ptMultiplexPort;  //主机多路复用
u8 Master_sg_u8Address = 0;                          //主机本板地址
struct InSerSocketPort *Master_sg_ptPortHead = NULL; //主机端链表初始化

struct MultiplexSetsCB * Slave_sg_ptMultiplexPort;  //从机多路复用
u8 Slave_sg_u8Address = 0;                          //从机本板地址
struct InSerSocketPort *Slave_sg_ptPortHead = NULL; //从机端链表初始化

struct Slave* SlaveList=NULL;  //从机描述
struct SemaphoreLCB *UartTimerSemp  = NULL; //信号量

u8 COUNT_Slave;
u8 Effective_Slave=0;

void __INS_GetProtocol(struct InSerSocketPort *Port,u8 Protocol)
{
    struct InSerSocket *Next,*SocketHead;
    SocketHead = Port->SocketHead;
    Next = SocketHead;
    do
    {
        if(Next->Protocol == Protocol)
            break;
        Next = Next->Next;
    } while(Next != SocketHead);
    Port->SocketHead = Next;
    return ;
}

void __Uart_HardTimer(void)
{
    Lock_SempPost(UartTimerSemp);
}

void __INS_Process(u32 timeout)
{
    struct InSerSocket *Me;
    struct InSerSocketPort *Port;
    struct DjyDevice *Device;
    ptu32_t DevRe;
    u32 sta,Completed,Completed1;
    u8 FSM,temp,loop;

    while(1)
    {
        u32 Len;
        //等待多路复用触发，多个通信口只要有一个触发即触发。
        //注，由于多路复用被设置为ET（边沿触发）策略，每次触发，必须全部读取数据。
        DevRe = Multiplex_Wait(Master_sg_ptMultiplexPort, &sta, timeout); //如果被触发,返回一个被触发的ObjectID,否则-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
            continue; //等于-1则继续等待
        }
        //串口设备的UserTag即为端口指针
        Port = (struct InSerSocketPort *)Driver_GetUserTag(Device); //提取用户标签
        if(sta & CN_MULTIPLEX_SENSINGBIT_READ)  //如果对象状态可读
        {
            u8 offset;
            if(Port->relen!=0)  //指针偏移量不为0,为1包多的情况
            {
                Port->fsm=0;
            }
            while(1)
            {
                //一次性读出来,这里包含协议头
                Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //读到0就跳出
                    break;
                FSM = Port->fsm;  //当前接收到哪里的状态机FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct InSerProtocol); //包头长度

                if(FSM < CN_OFF_USER)  //解码协议头(7个字节),状态机小于用户数据
                {
                    head: offset=0,FSM=0;
                    while(offset < Completed)
                    {
                        switch(FSM) //一个个解码
                        {
                            case CN_OFF_START:  //FSM=CN_OFF_START启动字节状态
                                //从已读到的缓冲区中，查找0xEB
                                while(offset < Completed + FSM)
                                {
                                    if(Port->RecvPkgBuf[offset++] == 0xEB)
                                    {
                                        FSM = CN_OFF_DST; //找到0xEB，就指向下一个
                                        if(FSM==Port->relen)
                                        {
                                            goto tail;
                                        }
                                        break;
                                    }
                                }
                                break ;
                            case CN_OFF_DST: //是否本板地址或本板组播地址
                                if((Port->RecvPkgBuf[offset] == Master_sg_u8Address)
                                   || (Port->RecvPkgBuf[offset] == Port->MTC_Address)
                                   || (Port->RecvPkgBuf[offset] == CN_INS_BROADCAST ))
                                {
                                    FSM = CN_OFF_PROTO; //无论正确与否,都往下解包
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                else
                                {
                                    FSM = CN_OFF_PROTO;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                offset++;
                                break ;
                            case CN_OFF_PROTO: //接收协议号时才知道哪个插槽
                                __INS_GetProtocol(Port,Port->RecvPkgBuf[offset]);
                                offset++;
                                FSM = CN_OFF_SRC;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_SRC: //源地址
                                offset++;
                                FSM = CN_OFF_LENL;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LENL:  //长度低字节
                                if((u16)Port->RecvPkgBuf[offset++] > Port->SocketHead->ProtocolRecvLen)
                                {
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_LEN_ERR);
                                    Port->ErrorPkgs++;  //错误包数
                                    Port->ErrorLast = CN_INS_LEN_ERR;
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                FSM = CN_OFF_LENH;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LENH: //长度高字节
                                if( (Port->RecvPkgBuf[offset-1] + ((u16)Port->RecvPkgBuf[offset] << 8))
                                    > Port->SocketHead->ProtocolRecvLen)
                                {
                                    offset++;
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_LEN_ERR);
                                    Port->ErrorLast = CN_INS_LEN_ERR;
                                    Port->ErrorPkgs++; //错误包数
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                offset++;
                                FSM = CN_OFF_CHKSUM;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_CHKSUM: //校验和
                                temp = 0;
                                for(loop = 0; loop < offset; loop++)
                                    temp += Port->RecvPkgBuf[loop];
                                if(temp != Port->RecvPkgBuf[offset++])  //校验和不等于前6个成员的和,则返回至CN_OFF_START
                                {
                                    if(Port->fnError != NULL)
                                        Port->fnError(Port, CN_INS_CHKSUM_ERR); //校验和检验不正确,调用错误回调函数处理
                                    Port->ErrorLast = CN_INS_CHKSUM_ERR;
                                    Port->ErrorPkgs++;
                                    FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
                                    {
                                        goto tail;
                                    }
                                }
                                else  //校验和等于前6个成员的和
                                {
                                    COUNT_Slave++;
                                    Effective_Slave=1;

                                    if((Port->RecvPkgBuf[offset-6] != Master_sg_u8Address) //地址不对
                                     &&(Port->RecvPkgBuf[offset-6] != Port->MTC_Address)
                                     &&(Port->RecvPkgBuf[offset-6] != CN_INS_BROADCAST ))
                                    {
                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LENL]+ (Port->RecvPkgBuf[CN_OFF_LENH] << 8);
                                        //大于1包,第一包非本板包
                                        if(((reln+sizeof(struct InSerProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct InSerProtocol)));loop++) //移位
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct InSerProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct InSerProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct InSerProtocol))==Completed1)  //判断第二包是否为完整包
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct InSerProtocol); //丢掉应用数据
                                        FSM=0;  //状态机归零
                                        goto tail;  //丢掉应用数据直接跳出
                                    }
                                    else
                                    {
                                        FSM = CN_OFF_USER;  //用户数据,FSM=7
                                        if(FSM==Port->relen)
                                        {
                                            goto tail;
                                        }
                                    }
                                }
                                break ;
                            default:break;
                        }
                    }
                }
                Len = (Port->RecvPkgBuf[CN_OFF_LENL] + (Port->RecvPkgBuf[CN_OFF_LENH] << 8));
                if((Len>(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //跳出整个大循环
                }
                if((Len<=(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //状态机返回到启动查找协议头的状态
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //InSerSocketPort中通讯协议赋给Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //InSerSocket对应协议处理函数
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //大于1包,第1包为本板包
                if(((Len+sizeof(struct InSerProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct InSerProtocol))); loop++) //移位
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct InSerProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct InSerProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break; //跳出整个大循环
            }
        }
//        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //如果对象状态可写
//        {    //判断我当前的发完没有,没发完看看还要发多少,继续发
//             if(Port->SendP < Port->PortSendLen + sizeof(struct InSerProtocol))
//            {
//                u32 SendLen;
//                SendLen = Port->PortSendLen + sizeof(struct InSerProtocol) - Port->SendP;
//                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
//                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
//                if(Completed != -1)
//                    Port->SendP += Completed;
//            }
//        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_ERROR) //如果对象状态出错,就调用出错的回调函数,记录错误号
        {
            if(Port->fnError != NULL)
                Port->fnError(Port, CN_INS_PHY_ERR);
            Port->ErrorLast = CN_INS_PHY_ERR;
            Port->ErrorPkgs++;
        }
        break;
    }

}

// ============================================================================
// 函数功能：主机端协议处理函数
// 输入参数：  ReplyModel，应答模式。取值如下
//                    ONE_BY_ONE，主从机一问一答模式。
//                    ONE_BY_MORE，主从机一问多答模式，提高通讯效率。
//ConfSlave，配置从机，取值如下
//                   SLAVE_INFORM_NUKNOWN，从机参数未知，即事先不知道从机地址、下发顺序等参数；
//                   SLAVE_INFORM_KNOWN，从机参数已知，由用户事先告知并配置。
//        Timeout，SLAVE_INFORM_NUKNOWN模式下等待从机响应报文的超时时间，该参数由用户配置。SLAVE_NFORM_KNOWN模式下无效,取NULL。
//        PoolModel，定时轮询模式，取值如下
//                   NO_TIMER_POLL，定时周期不做要求，各个从机有多快就多快回复；
//                   EVENLY_SPACED_POLL，等间隔周期轮询各个从机；
//                   ACCUMU_TIMER_POLL，总间隔周期轮询所有从机。
//        SlaveNum，从机数量。SLAVE_INFORM_NUKNOWN模式下取NULL。
//        Send_Buff，待发送的数据包，不含协议头
//        len，发送数据包长度
// 返回值：  无
// ============================================================================
ptu32_t Master_INS_Process(u32 ReplyModel,u32 ConfSlave,u32 Timeout,u32 PoolModel,u32 SlaveNum,u8 *Send_Buff,u16 len)
{
    u8 pos=1;
    u32 times=0;
    static u32 timeout;
    static u8 Slave_Address=1;
    static u8 Slave_Rank=1;
    struct InSerSocket *Me;
    struct InSerSocketPort *Port;

    while(1) //等待，直到注册通信端口
    {
        if(Master_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    timeout=Timeout;
    Port=Master_sg_ptPortHead;
    Me=Port->SocketHead;

    if(ConfSlave==SLAVE_INFORM_NUKNOWN)
    {
        while(1)
        {
            INS_SendPkg(Me,Slave_Address,Master_sg_u8Address,Send_Buff,len);
            __INS_Process(timeout);

            if(Effective_Slave)
            {
               SlaveList=Slave_Creat(Me->CommPort,Me->CommPort->SerialDevice,SlaveList,
                                                     Slave_Address,Slave_Rank,timeout,HALF_DUPLEX_COMM);
               Slave_Rank++;
               Effective_Slave=0;
            }
            Slave_Address++;
               if(Slave_Address==255)
                   break;
        }
        SlaveNum=COUNT_Slave;
    }

    while(SlaveList->Rank!=pos) //首先找到下发顺序为1的从机
    {
        SlaveList=SlaveList->Next;
    }

    //1、定时周期不做要求，各个从机有多快就多快回复
    if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
            __INS_Process(SlaveList->Timeout);
            SlaveList=SlaveList->Next;
        }
    }
    //2、等间隔周期轮询各个从机
    if((PoolModel==EVENLY_SPACED_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
            {
                INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
                __INS_Process(SlaveList->Timeout);
                SlaveList=SlaveList->Next;
            }
        }
    }
    //3、总间隔周期轮询所有从机
    if((PoolModel==ACCUMU_TIMER_POLL)&&(ReplyModel==ONE_BY_ONE))
    {
        while(1)
        {
            if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
            {
                while(1)
                {
                    INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
                    __INS_Process(SlaveList->Timeout);
                    SlaveList=SlaveList->Next;
                    times++;
                    if(times==SlaveNum)
                    {
                        times=0;
                        break;
                    }
                }
            }
        }
    }
    //4、主从机一问多答模式
    if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==ONE_BY_MORE))
    {
       INS_SendPkg(Me,SlaveList->Address,Master_sg_u8Address,Send_Buff,len);
       while(1)
       {
           __INS_Process(SlaveList->Timeout);
           SlaveList=SlaveList->Next;
       }
    }
    return 0;
}


// ============================================================================
// 函数功能：从机端协议处理函数，该处理函数可作为一个高优先级的线程弹出
// 输入参数：无
// 返回值：  无
// ============================================================================
ptu32_t Slave_INS_Process(void)
{
    u32 timeout,sta,Completed,Completed1;
    ptu32_t DevRe;
    struct InSerSocketPort *Port;
    struct InSerSocket *Me;
    struct DjyDevice *Device; //对应的设备指针
    u8 FSM,temp,loop;
    while(1)
    {
        //由于多事件并发，以及优先级的关系，运行至此时，可能还没有注册通信口，等
        //待，没100mS查看一下，直到注册通信口。
        if(Slave_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port = Slave_sg_ptPortHead;
    timeout = Port->Timeout;
    while(Port != NULL)
    {
        if(timeout > Port->Timeout)
            timeout = Port->Timeout;
        Port = Port->Next;
    }

    while(1)
    {
        u32 Len;
        //等待多路复用触发，多个通信口只要有一个触发即触发。
        //注，由于多路复用被设置为ET（边沿触发）策略，每次触发，必须全部读取数据。
        DevRe = Multiplex_Wait(Slave_sg_ptMultiplexPort, &sta, timeout); //如果被触发,返回一个被触发的ObjectID,否则-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
            continue; //等于-1则继续等待
        }
        //串口设备的UserTag即为端口指针
        Port = (struct InSerSocketPort *)Driver_GetUserTag(Device); //提取用户标签
        if(sta & CN_MULTIPLEX_SENSINGBIT_READ)  //如果对象状态可读
        {
            u8 offset;
            if(Port->relen!=0)  //指针偏移量不为0,为1包多的情况
            {
                Port->fsm=0;
            }
            while(1)
            {
            	//一次性读出来,这里包含协议头
				Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
				if(Completed1==0) //读到0就跳出
					break;
				FSM = Port->fsm;  //当前接收到哪里的状态机FSM
				offset = FSM;
				Completed1=Completed1+Port->relen;
				Port->relen=0;
            	Completed=sizeof(struct InSerProtocol); //包头长度

                if(FSM < CN_OFF_USER)  //解码协议头(7个字节),状态机小于用户数据
                {
                	head: offset=0,FSM=0;
                	while(offset < Completed)
                    {
                		switch(FSM) //一个个解码
						{
							case CN_OFF_START:  //FSM=CN_OFF_START启动字节状态
								//从已读到的缓冲区中，查找0xEB
								while(offset < Completed + FSM)
								{
								    if(Port->RecvPkgBuf[offset++] == 0xEB)
									{
									    FSM = CN_OFF_DST; //找到0xEB，就指向下一个
										if(FSM==Port->relen)
										{
											goto tail;
										}
										break;
									}
								}
								break ;
							case CN_OFF_DST: //是否本板地址或本板组播地址
								if((Port->RecvPkgBuf[offset] == Slave_sg_u8Address)
								   || (Port->RecvPkgBuf[offset] == Port->MTC_Address)
								   || (Port->RecvPkgBuf[offset] == CN_INS_BROADCAST ))
								{
								    FSM = CN_OFF_PROTO; //无论正确与否,都往下解包
									if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								else
								{
								    FSM = CN_OFF_PROTO;
									if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								offset++;
								break ;
							case CN_OFF_PROTO: //接收协议号时才知道哪个插槽
								__INS_GetProtocol(Port,Port->RecvPkgBuf[offset]);
								offset++;
								FSM = CN_OFF_SRC;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_SRC: //源地址
								offset++;
								FSM = CN_OFF_LENL;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_LENL:  //长度低字节
								if((u16)Port->RecvPkgBuf[offset++] > Port->SocketHead->ProtocolRecvLen)
								{
								    if(Port->fnError != NULL)
										Port->fnError(Port, CN_INS_LEN_ERR);
									Port->ErrorPkgs++;  //错误包数
									Port->ErrorLast = CN_INS_LEN_ERR;
									FSM = CN_OFF_START;
									if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								FSM = CN_OFF_LENH;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_LENH: //长度高字节
								if( (Port->RecvPkgBuf[offset-1] + ((u16)Port->RecvPkgBuf[offset] << 8))
									> Port->SocketHead->ProtocolRecvLen)
								{
								    offset++;
									if(Port->fnError != NULL)
										Port->fnError(Port, CN_INS_LEN_ERR);
									Port->ErrorLast = CN_INS_LEN_ERR;
									Port->ErrorPkgs++; //错误包数
									FSM = CN_OFF_START;
									if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								offset++;
								FSM = CN_OFF_CHKSUM;
								if(FSM==Port->relen)
								{
									goto tail;
								}
								break ;
							case CN_OFF_CHKSUM: //校验和
								temp = 0;
								for(loop = 0; loop < offset; loop++)
									temp += Port->RecvPkgBuf[loop];
								if(temp != Port->RecvPkgBuf[offset++])  //校验和不等于前6个成员的和,则返回至CN_OFF_START
								{
								    if(Port->fnError != NULL)
										Port->fnError(Port, CN_INS_CHKSUM_ERR); //校验和检验不正确,调用错误回调函数处理
									Port->ErrorLast = CN_INS_CHKSUM_ERR;
									Port->ErrorPkgs++;
									FSM = CN_OFF_START;
                                    if(FSM==Port->relen)
									{
										goto tail;
									}
								}
								else  //校验和等于前6个成员的和
								{
								    if((Port->RecvPkgBuf[offset-6] != Slave_sg_u8Address) //地址不对
								     &&(Port->RecvPkgBuf[offset-6] != Port->MTC_Address)
								     &&(Port->RecvPkgBuf[offset-6] != CN_INS_BROADCAST ))
									{
										u32 reln=Port->RecvPkgBuf[CN_OFF_LENL]+ (Port->RecvPkgBuf[CN_OFF_LENH] << 8);
										//大于1包,第一包非本板包
										if(((reln+sizeof(struct InSerProtocol))<Completed1))
										{
											for(loop=0;loop<(Completed1-(reln+sizeof(struct InSerProtocol)));loop++) //移位
											Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct InSerProtocol)+loop];
											Port->relen=(Completed1-(reln+sizeof(struct InSerProtocol)));
											Completed1=Port->relen;
											goto head;
										}
										if((reln+sizeof(struct InSerProtocol))==Completed1)  //判断第二包是否为完整包
										{
											Port->relen=0;
										}
										Completed1=sizeof(struct InSerProtocol); //丢掉应用数据
										FSM=0;  //状态机归零
										goto tail;  //丢掉应用数据直接跳出
									}
									else
									{
 									    FSM = CN_OFF_USER;  //用户数据,FSM=7
 										if(FSM==Port->relen)
 										{
 											goto tail;
 										}
									}
								}
								break ;
							default:break;
						}
                    }
                }
                Len = (Port->RecvPkgBuf[CN_OFF_LENL] + (Port->RecvPkgBuf[CN_OFF_LENH] << 8));
                if((Len>(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
                {
                	goto tail; //跳出整个大循环
                }
                if((Len<=(Completed1-sizeof(struct InSerProtocol)))&&(FSM==CN_OFF_USER))
				{
                	FSM = CN_OFF_START; //状态机返回到启动查找协议头的状态
					Port->fsm = FSM;
					Me = Port->SocketHead;  //InSerSocketPort中通讯协议赋给Me
					Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //InSerSocket对应协议处理函数
					Port->RecvP = 0;
					Port->relen=0;
				}
                //大于1包,第1包为本板包
                if(((Len+sizeof(struct InSerProtocol))<Completed1))
                {
				   for(loop = 0; loop < (Completed1-(Len+sizeof(struct InSerProtocol))); loop++) //移位
					   Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct InSerProtocol))];
				   Port->relen=Completed1-(Len+sizeof(struct InSerProtocol));
				   Completed1=Port->relen;
				   goto head;
                }
                tail:break; //跳出整个大循环
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //如果对象状态可写
        {    //判断我当前的发完没有,没发完看看还要发多少,继续发
             if(Port->SendP < Port->PortSendLen + sizeof(struct InSerProtocol))
            {
                u32 SendLen;
                SendLen = Port->PortSendLen + sizeof(struct InSerProtocol) - Port->SendP;
                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
                if(Completed != -1)
                    Port->SendP += Completed;
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_ERROR) //如果对象状态出错,就调用出错的回调函数,记录错误号
        {
            if(Port->fnError != NULL)
                Port->fnError(Port, CN_INS_PHY_ERR);
            Port->ErrorLast = CN_INS_PHY_ERR;
            Port->ErrorPkgs++;
        }
    }
}

// ============================================================================
// 函数功能：从机部分安装内部串口通信模块。该模块仅适用于从机部分。
// 输入参数：StackSize，模块需要的内存尺寸，由于串行通信协议解析后，要调用用户
//              提供的回调函数处理数据，本模块自身需要的内存很少，最多不过数百
//              字节，请用户根据自己的回调函数决定。
// 返回值： 总是成功，不成功就进入死循环。
// ============================================================================
bool_t Slave_ModuleInstall_InSerial(u32 StackSize)
{
    u16 evtt,event;
    Slave_sg_ptMultiplexPort = Multiplex_Create(1);

    evtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RLYMAIN, 1, 1, Slave_INS_Process,
                            NULL, StackSize, "In serial com process with slave");
    event = Djy_EventPop(evtt, NULL, 0, 0, 0, 0);
    if((evtt == CN_EVTT_ID_INVALID) || (event == CN_EVENT_ID_INVALID)
         || (Slave_sg_ptMultiplexPort == NULL) )
    {
        printf("从机部分内部串行通信模块初始化异常\n\r");
        while(1);                               //初始化阶段就异常，直接死循环
    }
    else
    {
        return true;
    }
}

// ============================================================================
// 函数功能：注册模块使用的通信端口，事实上，不是串口也可以注册的。
//        通信端口必须按djyos的设备驱动框架来驱动。主、从机公用。
// 输入参数：dev，通信端口的设备指针
//        comType，通信类型，取值MASTER_COMM_PORT或SLAVE_COMM_PORT
//                 注册主机端通信端口--MASTER_COMM_PORT;注册从机端通信端口--SLAVE_COMM_PORT
//        fnError，通信错误对应的回调函数，不需要则给NULL
//        Timeout，长时间没有收到数据的超时告警门限，uS为单位，单时间精度受
//                操作系统tick间隔限制。0xffffffff（-1）代表不设超时。
// 返回值：  通信插口指针
// ============================================================================
struct InSerSocketPort *INS_RegistPort(struct DjyDevice *dev,u8 comType,
                                       fntProtocolError fnError,u32 Timeout)
{
    struct InSerSocketPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct InSerSocketPort *)malloc(sizeof(struct InSerSocketPort ));
    if(Port != NULL) //分配成功
    {
        if(comType==0)
        {
            if(Master_sg_ptPortHead == NULL)
            {
                Port->Next = NULL;
            }
            else
            {
                Port->Next = Master_sg_ptPortHead;
            }
            Master_sg_ptPortHead = Port;
        }
        else
        {
            if(Slave_sg_ptPortHead == NULL)
            {
                Port->Next = NULL;
            }
            else
            {
                Port->Next = Slave_sg_ptPortHead;
            }
            Slave_sg_ptPortHead = Port;//申请到的Port赋给sg_ptPortHead，Port是中间量
        }
        Port->SocketHead = NULL; //对InSerSocket链表进行初始化
        Port->SerialDevice = dev;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->fsm = CN_OFF_START;
        Port->relen = 0;        //新增偏移量
        Port->SendPkgBuf = NULL;
        Port->RecvPkgBuf = NULL;
        Port->PortMaxRecvLen = 0;
        Port->PortMaxSendLen = 0;
        Port->PortSendLen = 0;
        Port->RecvP = 0;
        //若置0，初始状态会发送一包数据。
        Port->SendP = sizeof(struct InSerProtocol);  //发送偏移量为协议头
        Port->MTC_Address = CN_MTC_RESV;
        Driver_SetUserTag(dev, (ptu32_t)Port);
        if(comType==0)//添加多路复用
        {
            Driver_MultiplexAdd(Master_sg_ptMultiplexPort, &dev, 1,
                                               CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //对于主机端，可写不可用
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        }
        else
        {
            Driver_MultiplexAdd(Slave_sg_ptMultiplexPort, &dev, 1,
                                               CN_MULTIPLEX_SENSINGBIT_READ
                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        }
    }
    return Port;
}

// ============================================================================
// 函数功能：注册通信协议。
// 输入参数：Port，通信插口指针，为INS_RegistPort函数的返回值
//        Protocol，协议号，看 CN_INS_PROTOCOL_DEFAULT 等常数定义
//        MaxRecvLen，本协议接收数据包的最大长度
//        MaxSendLen，本协议发送数据包的最大长度
//        fn，接收数据包的协议处理函数
// 返回值：  协议插口指针
// ============================================================================
struct InSerSocket *INS_RegistProtocol(struct InSerSocketPort *Port, u8 Protocol,
                                      u16 MaxRecvLen,u16 MaxSendLen, fntProtocolProcess fn)
{
    struct InSerSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct InSerSocket *)malloc(sizeof(struct InSerSocketPort ));
    if(ProtocolSocket != NULL)
    {
        //result用于出错判断，不能用指针是否NULL，因为有单工工作的通信口，Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //接收数据包指针（不含协议头）
            		                    (MaxRecvLen+ sizeof(struct InSerProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct InSerProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //发送部分
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct InSerProtocol));
            Port->PortMaxSendLen = MaxSendLen;
            result |= (Port->RecvPkgBuf != NULL);
        }
        if(Port->SocketHead == NULL)
        {
            ProtocolSocket->Next = ProtocolSocket;
            Port->SocketHead = ProtocolSocket;
        }
        else
        {
            ProtocolSocket->Next = Port->SocketHead->Next;
            Port->SocketHead->Next = ProtocolSocket;
        }
        ProtocolSocket->Protocol = Protocol;           //协议号
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //本协议最大接收包长度
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //本协议最大发送包长度
        ProtocolSocket->CommPort = Port;               //对应的通信口指针
        ProtocolSocket->MyProcess = fn;                //协议处理函数指针
    }
    if((ProtocolSocket == NULL) || (result == false) )
    {
        printf("INS_RegistProtocol failure");
        free(ProtocolSocket);
        free(Port->RecvPkgBuf);
        free(Port->SendPkgBuf);
        return NULL;
    }
    else
        return ProtocolSocket;
}
// ============================================================================
// 函数功能：数据包发送函数，由用户调用，本函数加上协议头然后发送出去。主、从机公用。
// 输入参数：InSerSocket，通信协议插口指针，为INS_RegistProtocol函数的返回值
//        dst，目的地址，对于主机端：目的地址为从机从机地址，原地址为Master_Address；对于从机：目的地址为主机地址，
//             源地址为本板地址Slave_sg_u8Address
//        src，源地址
//        buf，待发送的数据包，不含协议头
//        len，发送长度
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 INS_SendPkg(struct InSerSocket  *InSerSocket, u8 dst,u8 src, u8 *buf, u16 len)
{
    struct InSerSocketPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    u32 Completed;
    if(InSerSocket == NULL)
        return 0;
    if(len > InSerSocket->ProtocolSendLen)
        return 0;
    Port = InSerSocket->CommPort;
    Port->PortSendLen = len;    //不含协议头
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = InSerSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LENL]    = len & 0xff;
    SendBuf[CN_OFF_LENH]    = len >> 8;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + InSerSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct InSerProtocol);
    memcpy(SendBuf + sizeof(struct InSerProtocol), buf, len);

    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
                                   0, CN_BLOCK_BUFFER, 0);
    if(Completed != -1)
        Port->SendP = Completed;

    return len;
}

// ============================================================================
// 函数功能：定时轮询周期配置。
//       说明：配置轮询周期前需了解从机特性，各个从机超时时间及处理回复时间，对于ACCUMU_TIMER_POLL总间隔周
//           期轮询模式，总的一圈的轮询周期为各个从机超时时间加上所配置额外的定时时间。
// 输入参数：timercycle，定时周期，单位us。NO_TIMER_POLL模式下取NULL.
//        PoolModel，定时轮询模式，EVENLY_SPACED_POLL--等间隔周期轮询各个从机；
//                            ACCUMU_TIMER_POLL--总间隔周期轮询各个从机（轮询一轮间隔），轮询一轮时间为timercycle加上各从机超时及处理时间；
//                            NO_TIMER_POLL--定时周期不做要求，有多快就轮询多快。
// 返回值：无
// ============================================================================
void Timer_Poll_Config(u32 timercycle,u8 PoolModel)
{
    static u32 CrystalFreq;
    u32 timer;
    timer=timercycle;

    if(PoolModel==NO_TIMER_POLL)
       return;
    if(PoolModel==EVENLY_SPACED_POLL||PoolModel==ACCUMU_TIMER_POLL)
    {
        UartTimerSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UART_TIMER_SEMP");
        uartTimerHandle=HardTimer_Alloc(__Uart_HardTimer);   //定时器分配
        if(uartTimerHandle==(-1))
           printf("主机端定时轮询模式配置异常\n\r");
        CrystalFreq = HardTimer_GetFreq(uartTimerHandle);  //获取定时器频率，对于F3来说，72MHZ
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETCYCLE,(CrystalFreq/1000000)*timer); //设置定时周期,单位us
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETRELOAD,true);    // reload模式
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_ENINT,true);        // 中断使能
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_SETINTPRO,false);   // 中断属性设置,true代表实时信号
        HardTimer_Ctrl(uartTimerHandle,EN_TIMER_STARTCOUNT,0);      // 使能计数
    }
    return;
}
// ============================================================================
// 函数功能：添加从板并设置参数。
// 输入参数：Port，对应通信端口指针
//        SerialDevice通信端口设备指针
//        pHead，从机结构体描述指针
//        address，从机地址
//        rank，主机端向各从机下发数据包顺序
//        timeout，超时时间，不知道超时时间情况设为NULL
//        commcode，通讯模式，半双工或单工，取值HALF_DUPLEX_COMM或SIMPLES_COMM
// 返回值：从机结构体描述指针
// ============================================================================
struct Slave *Slave_Creat(struct InSerSocketPort *Port,struct DjyDevice *dev,
                                   struct Slave *pHead,u8 address,u8 rank,u32 timeout,u32 commcode)
{
    struct Slave *p1;
    p1=(struct Slave*)malloc(sizeof(struct Slave));//申请新节点
    if(p1==NULL)
    {
        printf("内存申请失败\r\n");
    }
    p1->CommPort=Port;
    p1->SerialDevice=dev;
    p1->Address=address;
    p1->Rank = rank;
    p1->Timeout=timeout;
    p1->Commcode=commcode;

    if(pHead==NULL) //空表，接入表头
    {
        pHead->Next=pHead=p1;
    }
    else
    {
        //直接将p结点插入在尾结点后成为新的尾结点
        p1->Next=pHead->Next;
        pHead=pHead->Next=p1;
    }
    if(pHead==NULL)
    {
        printf("添加从机失败\r\n");
        while(1);   //初始化阶段就异常，直接死循环
    }
    return pHead;
}

// ============================================================================
// 函数功能：从已创建的从机中删除指定从机。
// 输入参数：pHead，从机结构体描述指针
//       deleteaddr，待删除从机地址
// 返回值： 从机结构体描述指针
// ============================================================================
struct Slave * Slave_Delete (struct Slave * pHead, int deleteaddr)
{
    struct Slave* p,*delete;
    p = pHead;

    if(p==NULL)
    {
        printf("No Slave!\n"); //没有从机
    }
    do
    {
        if(p->Next->Address==deleteaddr)
        break;
        else
        p = p->Next;
    } while (p!= pHead);

    if (p==pHead&&pHead->Next->Address!=deleteaddr)
    {
        printf("Not found!\n");
    }
    else
    {
        if (pHead->Next==pHead)
        {
            //表明此时链表中仅有一个结点且是待删结点
            free(pHead);
            pHead=NULL;
        }
        else
        {
            //此时链表中有两个结点及其以上
            delete=p->Next;
            p->Next=delete->Next;

            if(delete==pHead)
            {
                //表明待删结点是尾结点，所以需要挪尾指针
                pHead = p;
            }
            free(delete);
        }
    }
    return pHead;
}

// ============================================================================
// 函数功能：获取从机数量。
// 输入参数：pHead，从机结构体描述指针
// 返回值：    从机数量
// ============================================================================
u32 Slave_GetNum(struct Slave * pHead)
{
    struct Slave* p;
    u32 num=1;
    p=pHead;

    if(p==NULL)
    {
        printf("No Slave!\n"); //没有从机
        return 0;
    }
    p=p->Next;
    while(p!=pHead)
    {
        num++;
        p=p->Next;
    }
    return num;
}

// ============================================================================
// 函数功能：设置主机地址。
// 输入参数：Addr，本机地址，8位
// 返回值： 无
// ============================================================================
void Master_INS_SetAddress(u8 Addr)
{
    if(Addr < CN_MTC_RESV)
        Master_sg_u8Address = Addr;
}

// ============================================================================
// 函数功能：设置从机地址。
// 输入参数：Addr，本机地址，8位
// 返回值： 无
// ============================================================================
void Slave_INS_SetAddress(u8 Addr)
{
    if(Addr < CN_MTC_RESV)
        Slave_sg_u8Address = Addr;
}

// ============================================================================
// 函数功能：设置本机接受的组播地址。
// 输入参数：Port，被设置的目标通信口
//           Addr，组播地址，8位
// 返回值： 无
// ============================================================================
void INS_SetMtcAddress(struct InSerSocketPort *Port, u8 Addr)
{
    if((Addr >= CN_MTC_RESV) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}
