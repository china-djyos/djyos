//================================================================================
//组件名称：Host_ISBUS
//组件描述：工业串行总线（Industrial Serial Bus）通信模块主机端部分
//开发版本：V1.00
//开发人员:YT
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
#include "ISBUS_host.h"
#include "timer_hard.h"
#include "dbug.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"isbus_host"   //嵌入式串行通信主机端
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"devfile","lock","multiplex",         //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define CN_OFF_START        0
#define CN_OFF_DST          1
#define CN_OFF_PROTO        2
#define CN_OFF_SRC          3
#define CN_OFF_LEN          4
#define CN_OFF_CHKSUM       5
#define CN_OFF_USER         6

//协议包头，6字节
struct Host_ISBUSProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //目标地址
    u8 Protocol;        //功能号
    u8 SrcAddress;      //源地址
    u8 Len;             //数据包长度，2byte
    u8 ChkSum;          //校验和，即前5个成员的和。
};

//通信端口的描述
struct Host_ISBUSPort
{
    struct Host_ISBUSPort *Next;            //组成单向链表，末端指向NULL
    struct Host_FunctionSocket *SocketHead; //组成单向循环链表，Head指向当前接收的功能号值
    struct DjyDevice *SerialDevice;         //对应的设备指针
    Host_FntProtocolError fnError;          //出错对应的回调函数
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

//通信协议（功能号）的描述
struct Host_FunctionSocket
{
    struct Host_FunctionSocket *Next;       //组成一个链表，末端指向NULL
    u8 Protocol;                            //协议号
    u16 ProtocolRecvLen;                    //本协议最大接收包长度
    u16 ProtocolSendLen;                    //本协议最大发送包长度
    struct Host_ISBUSPort *CommPort;        //对应的通信口指针
    Host_FntProtocolProcess MyProcess;      //协议处理函数指针
};

ptu32_t uartTimerHandle; //定时器句柄

struct MultiplexSetsCB * Host_sg_ptMultiplexPort; //主机多路复用
u8 Host_sg_u8Address = 0;                         //主机本板地址
struct Host_ISBUSPort *Host_sg_ptPortHead = NULL; //主机端链表初始化

struct Slave* SlaveList=NULL;               //从机描述
struct SemaphoreLCB *UartTimerSemp  = NULL; //信号量

u8 COUNT_Slave;
u8 Effective_Slave=0;
u8 g_TimeOut=0;  //超时标志位

void __Host_GetProtocol(struct Host_ISBUSPort *Port,u8 Protocol)
{
    struct Host_FunctionSocket *Next,*SocketHead;
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

void __ISBUS_Process(u32 timeout)
{
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;
    struct DjyDevice *Device;
    ptu32_t DevRe;
    u32 sta,Completed,Completed1;
    u8 FSM,temp,loop;

    while(1)
    {
        u32 Len;
        //等待多路复用触发，多个通信口只要有一个触发即触发。
        //注，由于多路复用被设置为ET（边沿触发）策略，每次触发，必须全部读取数据。
        DevRe = Multiplex_Wait(Host_sg_ptMultiplexPort, &sta, timeout); //如果被触发,返回一个被触发的ObjectID,否则-1
        if(DevRe != -1)
            Device = (struct DjyDevice *)DevRe;
        else
        {
//            continue; //等于-1则继续等待
            //超时了，置超时标志位
            g_TimeOut=1;      //此标志位用于一问多答模式，某一从机超时未答的情况
            break;
        }
        //串口设备的UserTag即为端口指针
//        Port = (struct Host_ISBUSPort *)Driver_GetUserTag(Device); //提取用户标签
        Port = (struct Host_ISBUSPort *)Dev_GetDevTag(Device); //提取用户标签
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
//                Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                Completed1 = DevRead(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //读到0就跳出
                    break;
                FSM = Port->fsm;  //当前接收到哪里的状态机FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct Host_ISBUSProtocol); //包头长度

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
                                if((Port->RecvPkgBuf[offset] == Host_sg_u8Address)
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
                            case CN_OFF_PROTO: //接收功能号时才知道哪个插槽
                                __Host_GetProtocol(Port,Port->RecvPkgBuf[offset]);
                                offset++;
                                FSM = CN_OFF_SRC;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_SRC: //源地址
                                offset++;
                                FSM = CN_OFF_LEN;
                                if(FSM==Port->relen)
                                {
                                    goto tail;
                                }
                                break ;
                            case CN_OFF_LEN: //数据包长度
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
                                if(temp != Port->RecvPkgBuf[offset++])  //校验和不等于前5个成员的和,则返回至CN_OFF_START
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
                                else  //校验和等于前5个成员的和
                                {
                                    COUNT_Slave++;
                                    Effective_Slave=1;

                                    if((Port->RecvPkgBuf[offset-5] != Host_sg_u8Address) //地址不对
                                     &&(Port->RecvPkgBuf[offset-5] != Port->MTC_Address)
                                     &&(Port->RecvPkgBuf[offset-5] != CN_INS_BROADCAST ))
                                    {
                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LEN];
                                        //大于1包,第一包非本板包
                                        if(((reln+sizeof(struct Host_ISBUSProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct Host_ISBUSProtocol)));loop++) //移位
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct Host_ISBUSProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct Host_ISBUSProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct Host_ISBUSProtocol))==Completed1)  //判断第二包是否为完整包
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct Host_ISBUSProtocol); //丢掉应用数据
                                        FSM=0;  //状态机归零
                                        goto tail;  //丢掉应用数据直接跳出
                                    }
                                    else
                                    {
                                        FSM = CN_OFF_USER;  //用户数据,FSM=6
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
                Len = Port->RecvPkgBuf[CN_OFF_LEN]; //应用数据包长度
                if((Len>(Completed1-sizeof(struct Host_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //跳出整个大循环
                }
                if((Len<=(Completed1-sizeof(struct Host_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //状态机返回到启动查找协议头的状态
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //Host_ISBUSPort中通讯协议赋给Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);  //Host_FunctionSocket对应协议处理函数
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //大于1包,第1包为本板包
                if(((Len+sizeof(struct Host_ISBUSProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct Host_ISBUSProtocol))); loop++) //移位
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct Host_ISBUSProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct Host_ISBUSProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break; //跳出整个大循环
            }
        }
//        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //如果对象状态可写
//        {    //判断我当前的发完没有,没发完看看还要发多少,继续发
//             if(Port->SendP < Port->PortSendLen + sizeof(struct Host_ISBUSProtocol))
//            {
//                u32 SendLen;
//                SendLen = Port->PortSendLen + sizeof(struct Host_ISBUSProtocol) - Port->SendP;
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
//                   ONE_BY_ONE，主从机一问一答模式。
//                   BOOADCAST_MODEL，主从机一问多答模式，提高通讯效率。广播模式
//                   MULTICAST_MODEL，主从机一问多答模式，提高通讯效率。组播模式
//        PoolModel，定时轮询模式，取值如下
//                   NO_TIMER_POLL，定时周期不做要求，各个从机有多快就多快回复；
//                   EVENLY_SPACED_POLL，等间隔周期轮询各个从机；
//                   ACCUMU_TIMER_POLL，总间隔周期轮询所有从机。
//        PoolTimes，轮询次数，取UNLIMITED_NUMBER时为周而复始地轮询
//        Protocol，功能号
//        GroupModel，被轮询的从机集，全部或部分组，可由用户配置
//                   GroupModel取ALL_GROUP_POLL时全部轮询，其余数值为具体的组号
//        SlaveNum，从机数量。
//        Send_Buff，待发送的数据包，不含协议头
//        len，发送数据包长度
// 返回值：  无
// ============================================================================
ptu32_t Host_ISBUS_Process(u32 ReplyModel,u32 PoolModel,u32 PoolTimes,u8 Protocol,u8 GroupModel,u32 SlaveNum,u8 *Send_Buff,u16 len)
{
    u8 pos=1;
    u8 cnt=0;
    u8 cn[15];
    u32 times=0;
    u32 pooltimes=0;
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;

    while(1) //等待，直到注册通信端口
    {
        if(Host_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port=Host_sg_ptPortHead;
    Me=Port->SocketHead;

    if(ReplyModel==ONE_BY_ONE) //一问一答，点对点模式
    {
        while(SlaveList->Rank!=pos) //首先找到下发顺序为1的从机
        {
            SlaveList=SlaveList->Next;
        }

        switch(PoolModel)
        {
            //1、定时周期不做要求，主动快速轮询，各个从机有多快就多快回复
            case NO_TIMER_POLL:
                while(1)
                {
                    Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                    pooltimes++;
                    __ISBUS_Process(SlaveList->Timeout);
                    SlaveList=SlaveList->Next;
                    if(pooltimes==PoolTimes)
                    {
                        break; //轮询次数到，跳出
                    }
                }
                break;
            //2、等间隔周期轮询各个从机
            case EVENLY_SPACED_POLL:
                while(1)
                {
                    if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
                    {
                        Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                        pooltimes++;
                        __ISBUS_Process(SlaveList->Timeout);
                        SlaveList=SlaveList->Next;
                        if(pooltimes==PoolTimes)
                        {
                            break; //轮询次数到，跳出
                        }
                    }
                }
                break;
            //3、总间隔周期轮询所有从机
            case ACCUMU_TIMER_POLL:
                while(1)
                {
                    if(Lock_SempPend(UartTimerSemp,CN_TIMEOUT_FOREVER))
                    {
                        while(1)
                        {
                            Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Address,Host_sg_u8Address,Send_Buff,len);
                            pooltimes++;
                            __ISBUS_Process(SlaveList->Timeout);
                            SlaveList=SlaveList->Next;
                            times++;
                            if(times==SlaveNum)
                            {
                                times=0;
                                break;
                            }
                            if(pooltimes==PoolTimes)
                            {
                                break; //轮询次数到，跳出
                            }
                        }
                        if(pooltimes==PoolTimes)
                        {
                            break; //轮询次数到，跳出
                        }
                    }
                }
                break;
            default:break;
        }
    }
    else //广播、组播模式
    {
        //4、主从机一问多答广播模式
        if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==BOOADCAST_MODEL))
        {
           Host_ISBUS_SendPkg(Me,Protocol,CN_INS_BROADCAST,Host_sg_u8Address,Send_Buff,len); //广播地址，第二个字节为0XFF
           while(1)
           {
               __ISBUS_Process(SlaveList->Timeout);
               if(g_TimeOut)  //出现超时，直接下发下一从机
               {
                   Host_ISBUS_SendPkg(Me,Protocol,SlaveList->Next->Address,
                                                       Host_sg_u8Address,Send_Buff,len); //地址：下一从机地址
               }
               SlaveList=SlaveList->Next;
           }
        }

        //删除从机集中非特定组号的从机集
        for(int i=0;i<Slave_GetNum(SlaveList);i++)
        {
            if(SlaveList->Group!=GroupModel)
            {
                cn[cnt]=SlaveList->Group;
                cnt++;
            }
            SlaveList=SlaveList->Next;
        }
        for(int i=0;i<cnt;i++)
        {
            SlaveList=Slave_Delete(SlaveList,cn[i]);
        }

        //5、主从机一问多答组播模式
        if((PoolModel==NO_TIMER_POLL)&&(ReplyModel==MULTICAST_MODEL))
        {
           Host_ISBUS_SendPkg(Me,Protocol,(CN_INS_MULTICAST+GroupModel),Host_sg_u8Address,Send_Buff,len);
           while(1)
           {
               __ISBUS_Process(SlaveList->Timeout);
               SlaveList=SlaveList->Next;
           }
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
struct Host_ISBUSPort *Host_ISBUS_RegistPort(struct DjyDevice *dev,
                                             Host_FntProtocolError fnError,u32 Timeout)
{
    struct Host_ISBUSPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct Host_ISBUSPort *)malloc(sizeof(struct Host_ISBUSPort ));
    if(Port != NULL) //分配成功
    {
        if(Host_sg_ptPortHead == NULL)
        {
            Port->Next = NULL;
        }
        else
        {
            Port->Next = Host_sg_ptPortHead;
        }
        Host_sg_ptPortHead = Port;
        Port->SocketHead = NULL; //对Host_FunctionSocket链表进行初始化
        Port->SerialDevice = dev;
        Port->fnError = fnError;
        Port->ErrorPkgs = 0;
        Port->ErrorLast = CN_INS_OK;
        Port->Timeout = Timeout;
        Port->Resttime = 0;
        Port->fsm = CN_OFF_START;
        Port->relen = 0;            //新增偏移量
        Port->SendPkgBuf = NULL;
        Port->RecvPkgBuf = NULL;
        Port->PortMaxRecvLen = 0;
        Port->PortMaxSendLen = 0;
        Port->PortSendLen = 0;
        Port->RecvP = 0;
        //若置0，初始状态会发送一包数据。
        Port->SendP = sizeof(struct Host_ISBUSProtocol);  //发送偏移量为协议头
        Port->MTC_Address = CN_MTC_RESV;
        Driver_SetUserTag(dev, (ptu32_t)Port);
        Driver_MultiplexAdd(Host_sg_ptMultiplexPort, &dev, 1,
                                           CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE  //对于主机端，可写不可用
                                        |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                        |  CN_MULTIPLEX_SENSINGBIT_ET
                                        |  CN_MULTIPLEX_SENSINGBIT_OR    );
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
struct Host_FunctionSocket *Host_ISBUS_RegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                          u16 MaxRecvLen,u16 MaxSendLen, Host_FntProtocolProcess fn)
{
    struct Host_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct Host_FunctionSocket *)malloc(sizeof(struct Host_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result用于出错判断，不能用指针是否NULL，因为有单工工作的通信口，Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //接收数据包指针（不含协议头）
                                        (MaxRecvLen+ sizeof(struct Host_ISBUSProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct Host_ISBUSProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //发送部分
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct Host_ISBUSProtocol));
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
        ProtocolSocket->Protocol = Protocol;           //功能号
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //本协议最大接收包长度
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //本协议最大发送包长度
        ProtocolSocket->CommPort = Port;               //对应的通信口指针
        ProtocolSocket->MyProcess = fn;                //协议处理函数指针
    }
    if((ProtocolSocket == NULL) || (result == false) )
    {
        debug_printf("ISBUS","Host_ISBUS_RegistProtocol failure");
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
// 输入参数：Host_FunctionSocket，通信协议插口指针，为Host_ISBUS_RegistProtocol函数的返回值
//        Protocol，功能号
//        dst，目的地址，对于主机端：目的地址为从机从机地址，原地址为Host_Address；对于从机：目的地址为主机地址，
//             源地址为本板地址Slave_sg_u8Address
//        src，源地址
//        buf，待发送的数据包，不含协议头
//        len，发送长度
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 Host_ISBUS_SendPkg(struct Host_FunctionSocket  *Host_FunctionSocket,u8 Protocol,u8 dst,u8 src, u8 *buf, u16 len)
{
    struct Host_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    u32 Completed;
    if(Host_FunctionSocket == NULL)
        return 0;
    if(len > Host_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = Host_FunctionSocket->CommPort;
    Port->PortSendLen = len;    //不含协议头
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = Host_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + Host_FunctionSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct Host_ISBUSProtocol);
    memcpy(SendBuf + sizeof(struct Host_ISBUSProtocol), buf, len);

//    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
//                                   0, CN_BLOCK_BUFFER, 0);
    Completed = DevWrite(Port->SerialDevice, SendBuf, SendLen,
                                                             0,0);

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

    if(PoolModel==NO_TIMER_POLL) //定时时间不做要求
       return;
    if(PoolModel==EVENLY_SPACED_POLL||PoolModel==ACCUMU_TIMER_POLL)
    {
        UartTimerSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"UART_TIMER_SEMP");
        uartTimerHandle=HardTimer_Alloc(__Uart_HardTimer);   //定时器分配
        if(uartTimerHandle==(-1))
            debug_printf("ISBUS","主机端定时轮询模式配置异常\n\r");
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
// 函数功能：获取从机地址列表。ISBUS模块扫描所有可能的地址。在不知道从机参数情况下使用。
//    说明：主机上电，从机可能还没上电，提供一个接口，应用程序调时才扫描
// 输入参数：
// 返回值：从机列表
// ============================================================================
struct Slave* Slave_Addr_Get(struct Host_FunctionSocket  *InSerSocket)
{
    struct Host_FunctionSocket *Me;
    struct Host_ISBUSPort *Port;
    static u8 SlaveAddress=1;
    static u8 Slave_Rank=1;

    while(1) //等待，直到注册通信端口
    {
        if(Host_sg_ptPortHead != NULL)
            break ;
        Djy_EventDelay(100*mS);
    }
    Port=Host_sg_ptPortHead;
    Me=Port->SocketHead;
    u8 Send_Buff[1]={0x00};
    while(1)
    {
        Host_ISBUS_SendPkg(Me,0,SlaveAddress,Host_sg_u8Address,Send_Buff,sizeof(Send_Buff));
        __ISBUS_Process(10*mS);  //超时时间默认为10*mS

        if(Effective_Slave)
        {
           SlaveList=Slave_Creat(InSerSocket->CommPort,InSerSocket->CommPort->SerialDevice,SlaveList,
                                                       SlaveAddress,ALL_GROUP_POLL,Slave_Rank,10*mS);
           Slave_Rank++;
           Effective_Slave=0;
        }
        SlaveAddress++;
        if(SlaveAddress==255)
           break;
    }
    return SlaveList;
}

// ============================================================================
// 函数功能：添加从机并设置参数。
// 输入参数：Port，对应通信端口指针
//        SerialDevice通信端口设备指针
//        pHead，从机结构体描述指针
//        address，从机地址
//        group，从机所在组号（组播时用到），group取ALL_GROUP_POLL不分组
//        rank，主机端向各从机下发数据包顺序
//        timeout，超时时间，不知道超时时间情况设为NULL
//        commcode，通讯模式，半双工或单工，取值HALF_DUPLEX_COMM或SIMPLES_COMM
// 返回值：从机结构体描述指针
// ============================================================================
struct Slave *Slave_Creat(struct Host_ISBUSPort *Port,struct DjyDevice *dev,
                                   struct Slave *pHead,u8 address,u8 group,u8 rank,u32 timeout)
{
    struct Slave *p1;
    p1=(struct Slave*)malloc(sizeof(struct Slave));//申请新节点
    if(p1==NULL)
    {
        debug_printf("ISBUS","内存申请失败\r\n");
    }
    p1->CommPort=Port;
    p1->SerialDevice=dev;
    p1->Address=address;
    p1->Group = group;
    p1->Rank = rank;
    p1->Timeout=timeout;

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
        debug_printf("ISBUS","添加从机失败\r\n");
        while(1);   //初始化阶段就异常，直接死循环
    }
    return pHead;
}

// ============================================================================
// 函数功能：从已创建的从机中删除指定组号从机。
// 输入参数：pHead，从机结构体描述指针
//        group，删除某一组从机
// 返回值： 从机结构体描述指针
// ============================================================================
struct Slave * Slave_Delete (struct Slave * pHead, int group)
{
    struct Slave* p,*delete;
    p = pHead;

    while(1)
    {
        if(p==NULL)
        {
            debug_printf("ISBUS","No Slave!\n"); //没有从机
        }
        do
        {
            if(p->Next->Group==group)
            break;
            else
            p = p->Next;
        } while (p!= pHead);

        if (p==pHead&&pHead->Next->Group!=group)
        {
            debug_printf("ISBUS","Not found!\n");
            break;
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
        debug_printf("ISBUS","No Slave!\n"); //没有从机
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
// 函数功能：设置主机地址。主机端。
// 输入参数：Addr，本机地址，8位
// 返回值： 无
// ============================================================================
void Host_ISBUS_SetAddress(u8 Addr)
{
    Host_sg_u8Address = Addr;
}

