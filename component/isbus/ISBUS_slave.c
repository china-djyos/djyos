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
#include "ISBUS_slave.h"
#include "timer_hard.h"
#include "dbug.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t Slave_ModuleInstall_InSerial(u32 StackSize);
//    Slave_ModuleInstall_InSerial(CFG_INSERIAL_STACK_SIZE);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"isbus slave"//嵌入式串行通信从机端
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","lock","multiplex"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束
//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ISBUS_SLAVE == false )
//#warning  " isbus_slave  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ISBUS_SLAVE    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,0x10000,
#define CFG_INSERIAL_STACK_SIZE     4096     //"协议处理栈尺寸",配置serial模块协议处理函数所需要的局部内存
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
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
struct Slave_ISBUSProtocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //目标地址
    u8 Protocol;        //功能码
    u8 SrcAddress;      //源地址
    u8 Len;             //数据包长度，2byte
    u8 ChkSum;          //校验和，即前5个成员的和。
};

//通信端口的描述
struct Slave_ISBUSPort
{
    struct Slave_ISBUSPort *Next;           //组成单向链表，末端指向NULL
    struct Slave_FunctionSocket *SocketHead;//组成单向循环链表，Head指向当前接收的功能号值
    s32 SerialDevice;         //对应的设备指针
    Slave_FntProtocolError fnError;         //出错对应的回调函数
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
struct Slave_FunctionSocket
{
    struct Slave_FunctionSocket *Next;      //组成一个链表，末端指向NULL
    u8 Protocol;                            //功能码
    u16 ProtocolRecvLen;                    //本协议最大接收包长度
    u16 ProtocolSendLen;                    //本协议最大发送包长度
    struct Slave_ISBUSPort *CommPort;       //对应的通信口指针
    Slave_FntProtocolProcess MyProcess;     //协议处理函数指针
};

struct MultiplexSetsCB * Slave_sg_ptMultiplexPort;  //从机多路复用
u8 Slave_sg_u8Address = 0;                          //从机本地地址
u8 Slave_Broadcast_u8Address = 0;                   //从机组播号
struct Slave_ISBUSPort *Slave_sg_ptPortHead = NULL; //从机端链表初始化

u8 Host_Address=0;  //解包时保存的主机地址

void __Slave_GetProtocol(struct Slave_ISBUSPort *Port,u8 Protocol)
{
    struct Slave_FunctionSocket *Next,*SocketHead;
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

// ============================================================================
// 函数功能：从机端协议处理函数，该处理函数一般作为一个高优先级的线程弹出
// 输入参数：无
// 返回值：  无
// ============================================================================
ptu32_t Slave_ISBUS_Process(void)
{
    u32 timeout,sta,Completed,Completed1;
    s32 DevRe;
    struct Slave_ISBUSPort *Port;
    struct Slave_FunctionSocket *Me;
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
//        Port = (struct Slave_ISBUSPort *)Driver_GetUserTag(Device); //提取用户标签
        Port = (struct Slave_ISBUSPort *)Dev_GetDevTag(Device); //提取用户标签
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
//              Completed1 = Driver_ReadDevice(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                Completed1 = DevRead(Device, &Port->RecvPkgBuf[Port->relen],Port->PortMaxRecvLen, 0, 0);
                if(Completed1==0) //读到0就跳出
                    break;
                FSM = Port->fsm;  //当前接收到哪里的状态机FSM
                offset = FSM;
                Completed1=Completed1+Port->relen;
                Port->relen=0;
                Completed=sizeof(struct Slave_ISBUSProtocol); //包头长度
//              Completed=6;
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
                                offset++;
                                break ;
                            case CN_OFF_PROTO: //接收功能码时才知道哪个插槽
                                __Slave_GetProtocol(Port,Port->RecvPkgBuf[offset]);
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
                                //*******************************************************************************
//                              if((Port->RecvPkgBuf[1]==CN_INS_BROADCAST)||
//                                          (Port->RecvPkgBuf[1] == Port->MTC_Address))  //如果收到广播地址或组播地址，
//                                                                                         //此时将主机地址记录下来
//                              {
                                    Host_Address=Port->RecvPkgBuf[3];
//                              }
                                //*******************************************************************************
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
                                else  //校验和等于前5个成员的和
                                {
                                    if(Port->RecvPkgBuf[offset-5] != Slave_sg_u8Address)  //地址不对
//                                   &&(Port->RecvPkgBuf[offset-5] != Port->MTC_Address)
//                                   &&(Port->RecvPkgBuf[offset-5] != CN_INS_BROADCAST ))
                                    {
                                        //******************************************************************************
                                        if((Port->RecvPkgBuf[offset-5]==CN_INS_BROADCAST)||
                                                (Port->RecvPkgBuf[offset-5] == Port->MTC_Address)) //本板地址不对，且为广播或组播时
                                        {
                                            if(Slave_sg_u8Address==1) //本板地址为从机列表中地址为1的那个
                                            {
                                                FSM = CN_OFF_USER;
                                            }
                                        }

                                        if((Port->RecvPkgBuf[1]==Host_Address)&&
                                                  (Port->RecvPkgBuf[3])==Slave_sg_u8Address-1) //一问多答，本板收到上一板回给主机的报文
                                        {
                                            FSM = CN_OFF_USER;  //用户数据,FSM=6,直接调用协议处理函数进行发包操作
                                        }
                                        //*******************************************************************************

                                        u32 reln=Port->RecvPkgBuf[CN_OFF_LEN];
                                        //大于1包,第一包非本板包
                                        if(((reln+sizeof(struct Slave_ISBUSProtocol))<Completed1))
                                        {
                                            for(loop=0;loop<(Completed1-(reln+sizeof(struct Slave_ISBUSProtocol)));loop++) //移位
                                            Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[reln+sizeof(struct Slave_ISBUSProtocol)+loop];
                                            Port->relen=(Completed1-(reln+sizeof(struct Slave_ISBUSProtocol)));
                                            Completed1=Port->relen;
                                            goto head;
                                        }
                                        if((reln+sizeof(struct Slave_ISBUSProtocol))==Completed1)  //判断第二包是否为完整包
                                        {
                                            Port->relen=0;
                                        }
                                        Completed1=sizeof(struct Slave_ISBUSProtocol); //丢掉应用数据
                                        FSM=0;  //状态机归零
                                        goto tail;  //丢掉应用数据直接跳出
                                    }
                                    else//本板地址对上了
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
                if((Len>(Completed1-sizeof(struct Slave_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    goto tail; //跳出整个大循环
                }
                if((Len<=(Completed1-sizeof(struct Slave_ISBUSProtocol)))&&(FSM==CN_OFF_USER))
                {
                    FSM = CN_OFF_START; //状态机返回到启动查找协议头的状态
                    Port->fsm = FSM;
                    Me = Port->SocketHead;  //Slave_ISBUSPort中通讯协议赋给Me
                    Me->MyProcess(Me, &Port->RecvPkgBuf[CN_OFF_USER],Len);
                    Port->RecvP = 0;
                    Port->relen=0;
                }
                //大于1包,第1包为本板包
                if(((Len+sizeof(struct Slave_ISBUSProtocol))<Completed1))
                {
                   for(loop = 0; loop < (Completed1-(Len+sizeof(struct Slave_ISBUSProtocol))); loop++) //移位
                       Port->RecvPkgBuf[loop] = Port->RecvPkgBuf[loop+(Len+sizeof(struct Slave_ISBUSProtocol))];
                   Port->relen=Completed1-(Len+sizeof(struct Slave_ISBUSProtocol));
                   Completed1=Port->relen;
                   goto head;
                }
                tail:break;
            }
        }
        if(sta & CN_MULTIPLEX_SENSINGBIT_WRITE) //如果对象状态可写，判断我当前的发完没有,没发完看看还要发多少,继续发
        {
             if(Port->SendP < Port->PortSendLen + sizeof(struct Slave_ISBUSProtocol))
            {
                u32 SendLen;
                SendLen = Port->PortSendLen + sizeof(struct Slave_ISBUSProtocol) - Port->SendP;
//                Completed = Driver_WriteDevice(Device, Port->SendPkgBuf + Port->SendP,
//                                                SendLen, 0, CN_BLOCK_BUFFER, 0);
                Completed = DevWrite(Device, Port->SendPkgBuf + Port->SendP,
                                                SendLen, 0,0);
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
// 函数功能：安装内部串口通信模块。该模块仅适用于从机部分。
// 输入参数：StackSize，模块需要的内存尺寸，由于串行通信协议解析后，要调用用户
//              提供的回调函数处理数据，本模块自身需要的内存很少，最多不过数百
//              字节，请用户根据自己的回调函数决定。
//       CN_PRIO_RLYMAIN，事件优先级，由用户自己选取，一般选择较高优先级
// 返回值： 总是成功，不成功就进入死循环。
// ============================================================================
bool_t Slave_ModuleInstall_InSerial(u32 StackSize)
{
    u16 evtt,event;
    Slave_sg_ptMultiplexPort = Multiplex_Create(1);

    evtt = Djy_EvttRegist(EN_CORRELATIVE, CN_PRIO_RLYMAIN-5, 1, 1, Slave_ISBUS_Process,
                            NULL, StackSize, "ISBUS com process with slave");
    event = Djy_EventPop(evtt, NULL, 0, 0, 0, 0);
    if((evtt == CN_EVTT_ID_INVALID) || (event == CN_EVENT_ID_INVALID)
         || (Slave_sg_ptMultiplexPort == NULL) )
    {
        debug_printf("ISBUS","ISBUS通信模块从机端初始化异常\n\r");
        while(1);                               //初始化阶段就异常，直接死循环
    }
    else
    {
        return true;
    }
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
struct Slave_ISBUSPort *Slave_ISBUS_RegistPort(s32 dev,\
                               Slave_FntProtocolError fnError,u32 Timeout)
{
    struct Slave_ISBUSPort *Port;
    if(dev == NULL)
        return NULL;
    Port = (struct Slave_ISBUSPort *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(Port != NULL) //分配成功
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
        Port->SocketHead = NULL;   //对Slave_FunctionSocket链表进行初始化
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
        Port->SendP = sizeof(struct Slave_ISBUSProtocol);  //发送偏移量为协议头
        Port->MTC_Address = CN_INS_MULTICAST;
//        Driver_SetUserTag(dev, (ptu32_t)Port);
        dev_SetUserTag(dev, (ptu32_t)Port);
//        Driver_MultiplexAdd(Slave_sg_ptMultiplexPort, &dev, 1,
//                                               CN_MULTIPLEX_SENSINGBIT_READ
//                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
//                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
//                                            |  CN_MULTIPLEX_SENSINGBIT_ET
//                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
        Multiplex_AddObject(Slave_sg_ptMultiplexPort,  dev,
                                               CN_MULTIPLEX_SENSINGBIT_READ
                                            |  CN_MULTIPLEX_SENSINGBIT_WRITE
                                            |  CN_MULTIPLEX_SENSINGBIT_ERROR
                                            |  CN_MULTIPLEX_SENSINGBIT_ET
                                            |  CN_MULTIPLEX_SENSINGBIT_OR    );
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
struct Slave_FunctionSocket *Slave_ISBUS_RegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                            u16 MaxRecvLen,u16 MaxSendLen, Slave_FntProtocolProcess fn)
{
    struct Slave_FunctionSocket *ProtocolSocket;
    bool_t result = true;
    if(Port == NULL)
        return NULL;
    ProtocolSocket = (struct Slave_FunctionSocket *)malloc(sizeof(struct Slave_ISBUSPort ));
    if(ProtocolSocket != NULL)
    {
        //result用于出错判断，不能用指针是否NULL，因为有单工工作的通信口，Len=0
        if(MaxRecvLen > Port->PortMaxRecvLen)
        {
            Port->RecvPkgBuf = (u8*)realloc(Port->RecvPkgBuf,  //接收数据包指针（不含协议头）
                                        (MaxRecvLen+ sizeof(struct Slave_ISBUSProtocol)));

            Port->PortMaxRecvLen = (MaxRecvLen+ sizeof(struct Slave_ISBUSProtocol));
            result = (Port->RecvPkgBuf != NULL);
        }
        if(MaxSendLen > Port->PortMaxSendLen) //发送部分
        {
            Port->SendPkgBuf = (u8*)realloc(Port->SendPkgBuf,
                                       MaxSendLen + sizeof(struct Slave_ISBUSProtocol));
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
        ProtocolSocket->Protocol = Protocol;           //功能码
        ProtocolSocket->ProtocolRecvLen = MaxRecvLen;  //本协议最大接收包长度
        ProtocolSocket->ProtocolSendLen = MaxSendLen;  //本协议最大发送包长度
        ProtocolSocket->CommPort = Port;               //对应的通信口指针
        ProtocolSocket->MyProcess = fn;                //协议处理函数指针
    }
    if((ProtocolSocket == NULL) || (result == false) )
    {
        debug_printf("ISBUS","Slave_INS_RegistProtocol failure");
        free(ProtocolSocket);
        free(Port->RecvPkgBuf);
        free(Port->SendPkgBuf);
        return NULL;
    }
    else
        return ProtocolSocket;
}
// ============================================================================
// 函数功能：数据包发送函数，由用户调用，本函数加上协议头然后发送出去。从机端。
// 输入参数：Slave_FunctionSocket，通信协议插口指针，为INS_RegistProtocol函数的返回值
//        dst，对于从机：目的地址为主机地址，源地址为本板地址Slave_sg_u8Address
//        src，源地址
//        buf，待发送的数据包，不含协议头
//        len，发送长度
// 返回值：  发送的数据量，只是copy到了发送buf。
// ============================================================================
u32 Slave_ISBUS_SendPkg(struct Slave_FunctionSocket  *Slave_FunctionSocket, u8 dst,u8 src, u8 *buf, u16 len)
{
    struct Slave_ISBUSPort *Port;
    u8 *SendBuf;
    u16 SendLen;
    s32 Completed;
    if(Slave_FunctionSocket == NULL)
        return 0;
    if(len > Slave_FunctionSocket->ProtocolSendLen)
        return 0;
    Port = Slave_FunctionSocket->CommPort;
    Port->PortSendLen = len;    //不含协议头
    SendBuf = Port->SendPkgBuf;
    SendBuf[CN_OFF_START]   = 0xEB;
    SendBuf[CN_OFF_DST]     = dst;
    SendBuf[CN_OFF_PROTO]   = Slave_FunctionSocket->Protocol;
    SendBuf[CN_OFF_SRC]     = src;
    SendBuf[CN_OFF_LEN]     = len;
    SendBuf[CN_OFF_CHKSUM]  = 0xEB + dst + Slave_FunctionSocket->Protocol
                              + src + (len & 0xff) + (len >> 8);
    SendLen = len + sizeof(struct Slave_ISBUSProtocol);
    memcpy(SendBuf + sizeof(struct Slave_ISBUSProtocol), buf, len);

//    Completed = Driver_WriteDevice(Port->SerialDevice, SendBuf, SendLen,
//                                   0, CN_BLOCK_BUFFER, 0);
    Completed = DevWrite(Port->SerialDevice, SendBuf, SendLen,
                                                           0,0);
    if(Completed != -1)
        Port->SendP = Completed;

    return len;
}

// ============================================================================
// 函数功能：设置从机地址。
// 输入参数：Addr，本机地址，8位
// 返回值： 无
// ============================================================================
void Slave_ISBUS_SetAddress(u8 Addr)
{
    if(Addr < CN_INS_MULTICAST)  //不能超过组播起始地址
        Slave_sg_u8Address = Addr;
}

// ============================================================================
// 函数功能：设置本机接受的组播地址。
// 输入参数：Port，被设置的目标通信口
//           Addr，组播地址，8位
// 返回值： 无
// ============================================================================
void Slave_ISBUS_SetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr)
{
    if((Addr >= CN_INS_MULTICAST) && (Addr != CN_INS_BROADCAST) && (Port != NULL))
        Port->MTC_Address = Addr;
}
