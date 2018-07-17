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
//�鲥��ʼ��ַ
#define CN_INS_MULTICAST        0x80        //�鲥

#define ALL_GROUP_POLL          0x0         //ȫ����ѯ

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
#define ACCUMU_TIMER_POLL  (1)  //�ܼ��������ѯ�����ӻ�����ѯһ�ּ��������ѯһ��ʱ��Ϊtimercycle���ϸ��ӻ���ʱ������ʱ�䣡
#define NO_TIMER_POLL      (2)  //��ʱ���ڲ���Ҫ���ж�����ѯ���
#define S_TO_US            (1000000)

#define SLAVE_INFORM_NUKNOWN  (0) //���Ȳ�֪���ӻ���������Ϣ
#define SLAVE_INFORM_KNOWN    (1) //����Ԥ֪���ӻ���������Ϣ

#define ONE_BY_ONE        (0)  //һ��һ��
#define BOOADCAST_MODEL   (1)  //һ�ʶ�𣬹㲥ģʽ
#define MULTICAST_MODEL   (2)  //һ�ʶ���鲥ģʽ

#define UNLIMITED_NUMBER  (0xFFFFFFFF)  //���޴�����ѯ

struct Host_ISBUSPort;      //ͨ�Ŷ˿�
struct Host_FunctionSocket; //ͨ��Э��

struct Slave //�ӻ�������
{
    struct DjyDevice *SerialDevice;          //��Ӧ���豸ָ��
    struct Host_ISBUSPort *CommPort;         //��Ӧ��ͨ�ſ�ָ��
    u8  Address;                             //�ӻ���ַ
    u8  Rank;                                //������ӻ��·����ݰ�˳��
    u8  Group;                               //�ôӻ������ӻ������飩
    u32 Timeout;                             //��ʱʱ��
    struct  Slave *Next;                     //���ӻ����һ��ѭ������ĩ��ָ���ͷ
};

typedef void (*Host_FntProtocolProcess) (struct Host_FunctionSocket  *InSerSocket , u8 *buf, u32 len);

typedef void (*Host_FntProtocolError) (struct Host_ISBUSPort  *Port , u32 ErrorNo);

void Timer_Poll_Config(u32 timercycle,u8 PoolModel);

ptu32_t Host_ISBUS_Process(u32 ReplyModel,u32 PoolModel,
		                    u32 PoolTimes,u8 Protocol,u8 GroupModel,u32 SlaveNum,u8 *Send_Buff,u16 len);

struct Host_ISBUSPort *Host_ISBUS_RegistPort(struct DjyDevice *dev,
		                                                 Host_FntProtocolError fnError,u32 Timeout);


struct Host_FunctionSocket *Host_ISBUS_RegistProtocol(struct Host_ISBUSPort *Port, u8 Protocol,
                                         u16 MaxRecvLen,u16 MaxSendLen, Host_FntProtocolProcess fn);

struct Slave *Slave_Creat(struct Host_ISBUSPort *Port,struct DjyDevice *dev,
                                   struct Slave *pHead,u8 address,u8 group,u8 rank,u32 timeout);

struct Slave * Slave_Delete (struct Slave * pHead, int deleteaddr);

u32 Slave_GetNum(struct Slave * pHead);

u32 Host_ISBUS_SendPkg(struct Host_FunctionSocket  *Host_FunctionSocket,u8 Protocol, u8 dst,u8 src, u8 *buf, u16 len);

void Host_ISBUS_SetAddress(u8 Addr);


#ifdef __cplusplus
}
#endif

#endif //ifndef __ISBUSHost_H__

