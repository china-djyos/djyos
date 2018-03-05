//================================================================================
//组件名称：serialcomm
//组件描述：装置内部串口通信的网络链路层
//       本模块包含主机端与从机端
//开发版本：V1.00
//开发人员: 罗侍田
//================================================================================

#ifndef __SERIALCOMM_H__
#define __SERIALCOMM_H__

#ifdef __cplusplus
extern "C" {
#endif

//系统协议号定义，由平台确定和管理。
#define CN_INS_PROTOCOL_MANAGE      0       //通信管理
#define CN_INS_PROTOCOL_SHELL       1       //shell

//用户协议号定义，方案由研发管理会议确定，编码研发中心副总工统一管理。
#define CN_INS_PROTOCOL_DEFAULT     16
#define CN_INS_PROTOCOL_HAIWAI_ISA  17      //海外综自

//组播地址定义
#define CN_MTC_RESV             0x80        //保留
#define CN_MTC_IN_BOARD         0x81        //开入板
#define CN_MTC_OUT_BOARD        0x82        //开出板
#define CN_MTC_AC_BOARD         0x83        //交流采样板
#define CN_MTC_DC_BOARD         0x84        //直流采样板
#define CN_MTC_MFS_BOARD        0x85        //多功能采样板，例如直流采样+开入
#define CN_MTC_DSP_BOARD        0x86        //LCD显示板
#define CN_MTC_LED_BOARD        0x87        //灯板

#define CN_MTC_USER_BOARD       0xC0        //用户定义的组播类型起始编号，共63个

//广播地址
#define CN_INS_BROADCAST        0xff        //广播

//通信错误编号
#define CN_INS_OK              0x0
#define CN_INS_LEN_ERR         0x1
#define CN_INS_CHKSUM_ERR      0x2
#define CN_INS_PHY_ERR         0x3
#define CN_INS_TIMEROUT_ERR    0x4

#define MASTER_COMM_PORT (0) //主机端通信端口
#define SLAVE_COMM_PORT  (1) //从机端通信端口

#define SIMPLES_COMM      (0)//单工
#define HALF_DUPLEX_COMM  (1)//半双工

#define EVENLY_SPACED_POLL (0)  //等间隔周期轮询各个从机
#define ACCUMU_TIMER_POLL  (1)  //总间隔周期轮询各个从机（轮询一轮间隔），轮询一轮时间为timercycle加上各从机超时及处理时间
#define NO_TIMER_POLL      (2)  //定时周期不做要求，有多快就轮询多快
#define S_TO_US            (1000000)

#define SLAVE_INFORM_NUKNOWN  (0)
#define SLAVE_INFORM_KNOWN    (1)

#define ONE_BY_ONE     (0)  //一问一答
#define ONE_BY_MORE    (1)  //一问多答

struct InSerSocketPort;
struct InSerSocket;

struct Slave //从机的描述
{
    struct DjyDevice *SerialDevice;          //对应的设备指针
    struct InSerSocketPort *CommPort;        //对应的通信口指针
    u8  Address;                             //从机地址
    u8  Rank;                                //主机向从机下发数据包顺序
    u32 Timeout;                             //超时时间
    u32 Commcode;                            //通讯方式，半双工或单工
    struct  Slave *Next;                     //将从机组成一个循环链表，末端指向表头
};

typedef void (*fntProtocolProcess) (struct InSerSocket  *InSerSocket , u8 *buf, u32 len);
typedef void (*fntProtocolError) (struct InSerSocketPort  *Port , u32 ErrorNo);

bool_t Slave_ModuleInstall_InSerial(u32 StackSize);
ptu32_t Master_INS_Process(u32 ReplyModel,u32 ConfSlave,u32 Timeout,u32 PoolModel,
                                                       u32 SlaveNum,u8 *Send_Buff,u16 len);
struct InSerSocketPort *INS_RegistPort(struct DjyDevice *dev,u8 comType,
                                                 fntProtocolError fnError,u32 Timeout);
struct InSerSocket *INS_RegistProtocol(struct InSerSocketPort *Port, u8 Protocol,
                                         u16 MaxRecvLen,u16 MaxSendLen, fntProtocolProcess fn);
struct Slave *Slave_Creat(struct InSerSocketPort *Port,struct DjyDevice *dev,
                                             struct Slave *pHead,u8 address,u8 rank,u32 timeout,u32 commode);

struct Slave * Slave_Delete (struct Slave * pHead, int deleteaddr);
u32 Slave_GetNum(struct Slave * pHead);
u32 INS_SendPkg(struct InSerSocket  *InSerSocket, u8 dst, u8 src,u8 *buf, u16 len);
void Master_INS_SetAddress(u8 Addr);
void Slave_INS_SetAddress(u8 Addr);
void INS_SetMtcAddress(struct InSerSocketPort *Port, u8 Addr);

#ifdef __cplusplus
}
#endif

#endif //ifndef __SERIALCOMM_H__

