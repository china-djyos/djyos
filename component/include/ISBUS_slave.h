//================================================================================
//������ƣ�ISBUS_slave
//�����������ҵ�������ߣ�Industrial Serial Bus��ͨ��ģ��Ӽ��˲���
//�����汾��V1.00
//������Ա:YT
//================================================================================

#ifndef __ISBUS_SLAVE_H__
#define __ISBUS_SLAVE_H__

#ifdef __cplusplus
extern "C" {
#endif

//�㲥��ַ
#define CN_INS_BROADCAST        0xff        //�㲥
//�鲥��ʼ��ַ
#define CN_INS_MULTICAST        0x80        //�鲥

//ͨ�Ŵ�����
#define CN_INS_OK              0x0
#define CN_INS_LEN_ERR         0x1
#define CN_INS_CHKSUM_ERR      0x2
#define CN_INS_PHY_ERR         0x3
#define CN_INS_TIMEROUT_ERR    0x4

struct Slave_ISBUSPort;
struct Slave_FunctionSocket;

typedef void (*Slave_FntProtocolProcess) (struct Slave_FunctionSocket  *Slave_InSerSocket , u8 *buf, u32 len);
typedef void (*Slave_FntProtocolError) (struct Slave_ISBUSPort  *Port , u32 ErrorNo);

bool_t Slave_ModuleInstall_InSerial(u32 StackSize);

struct Slave_ISBUSPort *Slave_ISBUS_RegistPort(s32 dev,\
                                    Slave_FntProtocolError fnError,u32 Timeout);

struct Slave_FunctionSocket *Slave_ISBUS_RegistProtocol(struct Slave_ISBUSPort *Port, u8 Protocol,
                                                           u16 MaxRecvLen,u16 MaxSendLen, Slave_FntProtocolProcess fn);

u32 Slave_ISBUS_SendPkg(struct Slave_FunctionSocket  *Slave_FunctionSocket, u8 dst, u8 src,u8 *buf, u16 len);

ptu32_t Slave_ISBUS_Process(void);

void Slave_ISBUS_SetAddress(u8 Addr);

void Slave_ISBUS_SetMtcAddress(struct Slave_ISBUSPort *Port, u8 Addr);

#ifdef __cplusplus
}
#endif

#endif //ifndef __ISBUS_SLAVE_H__

