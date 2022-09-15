//================================================================================
//������ƣ�ISBUS_host
//�����������ҵ�������ߣ�Industrial Serial Bus��ͨ��ģ�������˲���
//�����汾��V1.00
//������Ա:YT
//================================================================================
#ifndef __ISBUSHOST_H__
#define __ISBUSHOST_H__

#ifdef __cplusplus
extern "C" {
#endif

//ͨ�Ŵ�����
#define CN_INS_OK              0x0
#define CN_INS_LEN_ERR         0x1
#define CN_INS_CHKSUM_ERR      0x2
#define CN_INS_PHY_ERR         0x3
#define CN_INS_TIMEROUT_ERR    0x4
#define CN_INS_SLAVE_FAULT     0x5      //�ӻ�����
#define CN_INS_SLAVE_RECOVER   0x6      //�ӻ��ָ�

//Э���ֶ�ƫ����
#define CN_OFF_START        0       //ͬ��ͷ����ƫ��
#define CN_OFF_DST          1       //Ŀ���ַ����ƫ��
#define CN_OFF_PROTO        2       //���ܺų���ƫ��
#define CN_OFF_SRC          3       //Դ��ַ����ƫ��
#define CN_OFF_SERIAL       4
#define CN_OFF_LEN          5       //Ӧ�ò����ݰ�����ƫ��
#define CN_OFF_CHKSUM       6       //У���볤��ƫ��
#define CN_OFF_USER         7       //Ӧ�ò����ݰ�����ƫ��

//ϵͳЭ��Ŷ��壬��ƽ̨ȷ���͹���
#define CN_SET_SLAVE_TABLE          0       //���ʹӻ���ַ�б����ݰ���ʽ������+�б�
#define CN_SET_MTC_TABLE            1       //�����鲥�Ų������鲥��ַ��
#define CN_SET_LINKLAYER_PARAM      2       //������·��ͨ�Ų���
#define CN_SET_POLL_CYCLE           3       //��֪��ѯ����
#define CN_SET_ECHO_TIME_LIMIT      4       //��֪�ӻ����յ���ѯ��������Ӧ���ʱ��
#define CN_CHK_SLAVE                5       //���ӻ��Ƿ���ڣ��ӻ��յ�������20mS��Ӧ��
#define CN_SLAVE_ACK                6       //��ʾ�յ��ղ�ۻ��ط�������
#define CN_TRANSMIT_DATA            7       //����ת��Э���

//�û�Э��Ŷ���
#define CN_PROTOCOL_USER_START     16
//�㲥��ַ
#define CN_INS_BROADCAST        0xff        //�㲥��ַ

//�鲥��ʼ��ַ
#define CN_INS_MULTICAST        0x80        //�鲥��ʼ��ַ
#define CN_INS_MTC_INVALID      0xfe        //�Ƿ��鲥��ַ

//��ѯ��ʽ����
#define CN_POLL_FASTEST         (0)  //��ʱ���ڲ���Ҫ���ж�����ѯ���
#define CN_POLL_SAME_INTERVAL   (1)  //�ȼ��������ѯ�����ӻ�
#define CN_POLL_SAME_CYCLE      (2)  //��������ѯ���дӻ�

#define UNLIMITED_NUMBER  (0xFFFFFFFF)  //���޴�����ѯ
extern bool_t debug_ctrl;

struct ISBUS_FunctionSocket;
struct ISBUS_Protocol;
struct Host_ISBUSPort;      //ͨ�Ŷ˿�
struct Slave_ISBUSPort;
struct SlaveList;                             //�ӻ��б�
typedef void (*ISBUS_FntProtocolProcess) (struct ISBUS_FunctionSocket  *InSerSocket, u8 src, u8 *buf, u32 len);
typedef void (*ISBUS_FntProtocolError) (void  *Port , u32 ErrorNo, u8 SlaveAddress);


//����API
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

//�ӻ�API
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

