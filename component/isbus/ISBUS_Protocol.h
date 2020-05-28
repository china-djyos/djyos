#ifndef __ISBUS_PROTOCOL_h__
#define __ISBUS_PROTOCOL_h__

#ifdef __cplusplus
extern "C" {
#endif

#define ONE_BY_ONE        (0)  //一问一答
#define BROADCAST_MODEL   (1)  //一问多答，广播模式
#define MULTICAST_MODEL   (2)  //一问多答，组播模式

//通信协议（功能号）的描述
struct ISBUS_FunctionSocket
{
    struct ISBUS_FunctionSocket *Next;       //组成一个链表，末端指向NULL
    u8 Protocol;                            //协议号
    u16 ProtocolRecvLen;                    //本协议最大接收包长度
    u16 ProtocolSendLen;                    //本协议最大发送包长度
    void *CommPort;                         //对应的通信口(主机或从机)指针
    ISBUS_FntProtocolProcess MyProcess;      //协议处理函数指针
};
//协议包头，7字节
struct ISBUS_Protocol
{
    u8 start;           //0xeb
    u8 DstAddress;      //目标地址
    u8 Protocol;        //功能码
    u8 SrcAddress;      //源地址
    u8 PkgSerial;       //包序号，仅主机下发的有效，从机上送的填 0
    u8 Len;             //数据包长度
    u8 ChkSum;          //校验和，即前5个成员的和。
};

#ifdef __cplusplus
}
#endif

#endif //__ISBUS_PROTOCOL_h__



