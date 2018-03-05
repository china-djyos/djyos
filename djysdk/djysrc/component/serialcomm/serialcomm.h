//================================================================================
//������ƣ�serialcomm
//���������װ���ڲ�����ͨ�ŵ�������·��
//       ��ģ�������������ӻ���
//�����汾��V1.00
//������Ա: ������
//================================================================================

#ifndef __SERIALCOMM_H__
#define __SERIALCOMM_H__

#ifdef __cplusplus
extern "C" {
#endif

//ϵͳЭ��Ŷ��壬��ƽ̨ȷ���͹���
#define CN_INS_PROTOCOL_MANAGE      0       //ͨ�Ź���
#define CN_INS_PROTOCOL_SHELL       1       //shell

//�û�Э��Ŷ��壬�������з��������ȷ���������з����ĸ��ܹ�ͳһ����
#define CN_INS_PROTOCOL_DEFAULT     16
#define CN_INS_PROTOCOL_HAIWAI_ISA  17      //��������

//�鲥��ַ����
#define CN_MTC_RESV             0x80        //����
#define CN_MTC_IN_BOARD         0x81        //�����
#define CN_MTC_OUT_BOARD        0x82        //������
#define CN_MTC_AC_BOARD         0x83        //����������
#define CN_MTC_DC_BOARD         0x84        //ֱ��������
#define CN_MTC_MFS_BOARD        0x85        //�๦�ܲ����壬����ֱ������+����
#define CN_MTC_DSP_BOARD        0x86        //LCD��ʾ��
#define CN_MTC_LED_BOARD        0x87        //�ư�

#define CN_MTC_USER_BOARD       0xC0        //�û�������鲥������ʼ��ţ���63��

//�㲥��ַ
#define CN_INS_BROADCAST        0xff        //�㲥

//ͨ�Ŵ�����
#define CN_INS_OK              0x0
#define CN_INS_LEN_ERR         0x1
#define CN_INS_CHKSUM_ERR      0x2
#define CN_INS_PHY_ERR         0x3
#define CN_INS_TIMEROUT_ERR    0x4

#define MASTER_COMM_PORT (0) //������ͨ�Ŷ˿�
#define SLAVE_COMM_PORT  (1) //�ӻ���ͨ�Ŷ˿�

#define SIMPLES_COMM      (0)//����
#define HALF_DUPLEX_COMM  (1)//��˫��

#define EVENLY_SPACED_POLL (0)  //�ȼ��������ѯ�����ӻ�
#define ACCUMU_TIMER_POLL  (1)  //�ܼ��������ѯ�����ӻ�����ѯһ�ּ��������ѯһ��ʱ��Ϊtimercycle���ϸ��ӻ���ʱ������ʱ��
#define NO_TIMER_POLL      (2)  //��ʱ���ڲ���Ҫ���ж�����ѯ���
#define S_TO_US            (1000000)

#define SLAVE_INFORM_NUKNOWN  (0)
#define SLAVE_INFORM_KNOWN    (1)

#define ONE_BY_ONE     (0)  //һ��һ��
#define ONE_BY_MORE    (1)  //һ�ʶ��

struct InSerSocketPort;
struct InSerSocket;

struct Slave //�ӻ�������
{
    struct DjyDevice *SerialDevice;          //��Ӧ���豸ָ��
    struct InSerSocketPort *CommPort;        //��Ӧ��ͨ�ſ�ָ��
    u8  Address;                             //�ӻ���ַ
    u8  Rank;                                //������ӻ��·����ݰ�˳��
    u32 Timeout;                             //��ʱʱ��
    u32 Commcode;                            //ͨѶ��ʽ����˫���򵥹�
    struct  Slave *Next;                     //���ӻ����һ��ѭ������ĩ��ָ���ͷ
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

