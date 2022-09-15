//================================================================================
//组件名称：ISBUS_host
//组件描述：工业串行总线（Industrial Serial Bus）通信模块主机端部分
//开发版本：V1.00
//开发人员:YT
//================================================================================
#ifndef __ISBUSHOST_H__
#define __ISBUSHOST_H__

#ifdef __cplusplus
extern "C" {
#endif

//通信错误编号
#define CN_INS_OK              0x0
#define CN_INS_LEN_ERR         0x1
#define CN_INS_CHKSUM_ERR      0x2
#define CN_INS_PHY_ERR         0x3
#define CN_INS_TIMEROUT_ERR    0x4
#define CN_INS_SLAVE_FAULT     0x5      //从机故障
#define CN_INS_SLAVE_RECOVER   0x6      //从机恢复

//协议字段偏移量
#define CN_OFF_START        0       //同步头长度偏移
#define CN_OFF_DST          1       //目标地址长度偏移
#define CN_OFF_PROTO        2       //功能号长度偏移
#define CN_OFF_SRC          3       //源地址长度偏移
#define CN_OFF_SERIAL       4
#define CN_OFF_LEN          5       //应用层数据包长度偏移
#define CN_OFF_CHKSUM       6       //校验码长度偏移
#define CN_OFF_USER         7       //应用层数据包长度偏移

//系统协议号定义，由平台确定和管理。
#define CN_SET_SLAVE_TABLE          0       //传送从机地址列表，数据包格式：数量+列表
#define CN_SET_MTC_TABLE            1       //建立组播号并传送组播地址表
#define CN_SET_LINKLAYER_PARAM      2       //发布链路层通信参数
#define CN_SET_POLL_CYCLE           3       //告知轮询周期
#define CN_SET_ECHO_TIME_LIMIT      4       //告知从机，收到轮询命令后必须应答的时限
#define CN_CHK_SLAVE                5       //检查从机是否存在，从机收到后须在20mS内应答。
#define CN_SLAVE_ACK                6       //表示收到空插槽或重发的数据
#define CN_TRANSMIT_DATA            7       //数据转发协议号

//用户协议号定义
#define CN_PROTOCOL_USER_START     16
//广播地址
#define CN_INS_BROADCAST        0xff        //广播地址

//组播起始地址
#define CN_INS_MULTICAST        0x80        //组播起始地址
#define CN_INS_MTC_INVALID      0xfe        //非法组播地址

//轮询方式定义
#define CN_POLL_FASTEST         (0)  //定时周期不做要求，有多快就轮询多快
#define CN_POLL_SAME_INTERVAL   (1)  //等间隔周期轮询各个从机
#define CN_POLL_SAME_CYCLE      (2)  //等周期轮询所有从机

#define UNLIMITED_NUMBER  (0xFFFFFFFF)  //无限次数轮询
extern bool_t debug_ctrl;

struct ISBUS_FunctionSocket;
struct ISBUS_Protocol;
struct Host_ISBUSPort;      //通信端口
struct Slave_ISBUSPort;
struct SlaveList;                             //从机列表
typedef void (*ISBUS_FntProtocolProcess) (struct ISBUS_FunctionSocket  *InSerSocket, u8 src, u8 *buf, u32 len);
typedef void (*ISBUS_FntProtocolError) (void  *Port , u32 ErrorNo, u8 SlaveAddress);


//主机API
struct Host_ISBUSPort *ISBUS_HostRegistPort(char *dev,ISBUS_FntProtocolError fnError,u32 Timeout);
struct ISBUS_FunctionSocket *ISBUS_HostRegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                          u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn);
u32 ISBUS_SetPollPkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                        u8 *buf, u8 len, s32 times, u32 Timeout);
u32 ISBUS_HostSetIM_Pkg(struct ISBUS_FunctionSocket  *ISBUS_FunctionSocket,u8 dst,
                            u8 *buf, u8 len, u32 Timeout);
void ISBUS_PollConfig(struct Host_ISBUSPort *Port,u32 timercycle,u8 PoolModel);
void ISBUS_AddSlave(struct Host_ISBUSPort *Port, u8 address);
void ISBUS_SendSlaveTable(struct Host_ISBUSPort *Port,u8 *address, u8 num);
struct SlaveList* ISBUS_ScanSlave(struct Host_ISBUSPort *Port,u8 MinAddress,u8 MaxAddress);
u8 ISBUS_GetSlaveTable(struct Host_ISBUSPort *Port,u8 *address);
void ISBUS_SendSlaveTable(struct Host_ISBUSPort *Port,u8 *address, u8 num);
void ISBUS_DeleteSlave (struct Host_ISBUSPort * Port, u8 address);
void ISBUS_HostInit(u32 HostStackSize);

//从机API
bool_t ISBUS_SlaveInit(u32 StackSize);
struct Slave_ISBUSPort *ISBUS_SlaveRegistPort(char * dev,ISBUS_FntProtocolError fnError,u32 Timeout);
struct ISBUS_FunctionSocket *ISBUS_SlaveRegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                                           u16 MaxRecvLen,u16 MaxSendLen, ISBUS_FntProtocolProcess fn);
u32 ISBUS_SlaveSendPkg(struct ISBUS_FunctionSocket  *Slave_FunctionSocket, u8 dst, u8 *buf, u8 len);
void ISBUS_SlaveSetAddress(u8 Addr);
void ISBUS_SlaveSetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr);

#ifdef __cplusplus
}
#endif

#endif //ifndef __ISBUSHost_H__

