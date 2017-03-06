#ifndef	__IOT_H__
#define	__IOT_H__

#ifdef	__cplusplus
extern "C"{
#endif

#include "stdio.h"
#include "string.h"

#include "os.h"
#include "int.h"
#include "lowpower.h"
#include "uart.h"

/*=====================================================================*/
#define	TRANSFER_MODULE_LOG	"Transfer-20160413-1645\r\n"
#define	MONITOR_MODULE_LOG	"Monitor-20160413-1645\r\n"
#define TRANSFER_MODULE_UPDATE_LOG  "Transfer UPDATE\r\n"
#define MONITOR_MODULE_UPDATE_LOG   "Monitor Update\r\n"

/*=====================================================================*/

#define SINGLE_UPDATE_PKG_SIZE_MAX     180
#define UPDATE_PKG_NUM_MAX             2048    //����ζ��������APP������Ϊ360k.
#define MONITOR_NUM_MAX                256     //һ���м�����Ӧ�ļ��˽ڵ������.
#define CN_REPEAT_MAX_NUM              4
//#define	SX1278_TX_TIME		800		//(2.6K)SX1278 RF���ݰ���������Ҫ��ʱ��(ms)
#define	SX1278_TX_TIME		200		//(18.5K)SX1278 RF���ݰ���������Ҫ��ʱ��(ms)
#define	SX1278_RX_TIMEOUT	2000	//SX1278 RF���ݰ����ճ�ʱʱ��(ms)

/*=====================================================================*/
#define	BATT_VOL_STEP	50	//��ص�ѹ����ֵ(50mV)
#define	VOICE_VOL_STEP	10	//������ѹ����ֵ(10mV)

//����ʱ�м������������߼��˽ڵ����м����ظ�ACK���dat������
#define ACK_CODE_UPDATE_CMD       0xA1   //UPDATE_CMD �ظ���ACK
#define ACK_CODE_UPDATE_MINITOR   0xA2   //UPDATE_MONITOR_NUM�ظ���ACK

//����ʱ�м������������߼����������ظ�UPDATE_ACK����ʱ��dat������
#define ACK_CODE_UPDATE_SUCCESS  0xA1
#define ACK_CODE_UPDATE_FAILED   0xA2
#define ACK_CODE_UPDATE_END      0xA3

//����ʧ��ԭ��
#define CN_COMMUNICATION_ERROR   0xA1
#define CN_FLASH_WRITE_ERROE     0xA2
#define CN_MONITOR_NO_WORK       0xA3


//�������
#define CN_UPDATE_FAILED       0xA1
#define CN_UPDATE_SUCCESS      0xA2

//�ڵ��ɫ�����м����ͼ��˽ڵ�
#define CN_TRANSFER_ROLE      0xA1
#define CN_MONITOR_ROLE       0xA2

/*=====================================================================*/

//����/�м̶˷������˵�����
#define	GET_STATUS				0x01	//���״̬
#define	SET_VOICE_ONOFF			0x02	//������Ƶ������/��
#define	GET_VOICE_VOLUME		0x03	//�����Ƶ����(��ѹֵ)
#define	RESET_VOICE_VOLUME		0x04	//���¿�ʼ��¼����ֵ(֮ǰ��¼��ֵ������)
#define	GET_VOICE_RECODE		0x05	//���¼������
#define	SET_ENTER_SLEEP_TIME	0x06	//���ÿ���״̬��,����˯��״̬ʱ��.
#define	VOICE_RECODE_RESTART	0x07	//���¿�ʼ¼��
#define	VOICE_PLAY_HOST			0x08	//��������(��Host����ȥ����������)
#define	VOICE_PLAY_SOUND_1		0x09	//���ű���Ԥ����Ƶ1(ѯ���Ƿ�����)
#define	WAKEUP_DEVICE			0xE0	//����Host�˻����豸
#define	ACK						0xF0	//Ӧ��
#define UPDATE_CMD              0xA0    //��������
#define UPDATE_DATA             0xB0    //������Ч����
#define UPDATE_ACK              0xC0    //�����ظ�(�����м����ظ������ͼ��˻ظ��м���)
#define UPDATE_DATA_REPEAT      0xD0    //ǰ������ٴδ���
#define UPDATE_MONITOR_NUM       0xD1    //���������м�������





#pragma	pack(1)

#define	HOST_PKT_TAG	0xF0A0

//���������ݰ�ͷ�ṹ
struct	tagHOST_PKT_HDR
{
	u16 tag;		//HOST_PKT_TAG
	u8  hdr_len;	//hostͷ����(6�ֽ�)
	u8  net_id;		//����ID
	u8  usr_id_h;	//�û�ID��λ
	u8  usr_id_l;	//�û�ID��λ
	u8  dat[0];		//��������ʼ
};


#define	MON_PKT_TAG		0xF0A1	//�������ݰ���ʶ

//�������ݰ�ͷ�ṹ
struct	tagMON_PKT_HDR
{
	u16 tag;		//MON_PKT_TAG
	u8  cmd;		//����
	u16 pkgnum;     //����������cmd��ΪUPDATE_CMD��UPDATE_DATA��UPDATE_ACKʱ��
                    //������Ϊ0x00.��cmdΪUPDATE_DATA��UPDATE_ACK�ֱ��ʾ�ļ�
                    //��Ҫ���͵����ݰ�������Ҫ�ظ�ACK�İ���.
	u16 index;      //��������ţ���cmd��ΪUPDATE_DATA�� UPDATE_ACKʱ��������Ϊ0x00.
	u8  dat_len;	//���ݳ���

	u32 ecc_code;   //ECCУ����,��cmd��ΪUPDATE_DATAʱ��������Ϊ0x00000000.
	u8	dat[0];		//��������ʼ
};


struct tagUpdatePara
{
	u16 pkgnum;
	u16 index;
	u32 ecc_code;
};

#define	SIZEOF_MONPKT_HDR		(sizeof(struct tagMON_PKT_HDR))
#define	SIZEOF_MONPKT_END		(2) //CS8+0x00


//GET_STATUS��������ݽṹ
struct	tagMON_STATUS_DATA
{
	u32 smog_status;	//����״̬
	u8  batt_voltage;	//��ص�ѹ(x50mV)
	s8	temperature;	//�¶�ֵ
};

#define	SIZEOF_STATUS_PKT	(sizeof(struct tagMON_PKT_HDR) + sizeof(struct	tagMON_STATUS_DATA) + 2)

//SET_VOICE_ONOFF���ݽṹ
struct	tagMON_SET_VOICE_ONOFF_DATA
{
	u8 onoff;		//0:�ر�; 1:��
};
#define	SIZEOF_SET_VOICE_ONOFF_PKT	(sizeof(struct tagMON_PKT_HDR) + sizeof(struct	tagMON_SET_VOICE_ONOFF_DATA) + 2)


//GET_VOICE_VOLUME���ݽṹ
#define	VOICE_CHECK_COUNT	180		//����������

struct	tagMON_VOICE_VOLUME_DATA
{
	u8 vol[VOICE_CHECK_COUNT];
};

#define	SIZEOF_VOICE_VOLUME_PKT	(sizeof(struct tagMON_PKT_HDR) + sizeof(struct	tagMON_VOICE_VOLUME_DATA) + 2)

extern u8	VoiceVolBuf[VOICE_CHECK_COUNT];

//SET_ENTER_SLEEP_TIME���ݽṹ
struct	tagMON_SET_ENTER_SLEEP_TIME_DATA
{
	u32 time;	//����״̬��,����˯��״̬ʱ��,��1��Ϊ����
};

#define	SIZEOF_SET_ENTER_SLEEP_TIME_PKT	(sizeof(struct tagMON_PKT_HDR) + sizeof(struct	tagMON_SET_ENTER_SLEEP_TIME_DATA) + 2)

/*=====================================================================*/
//GET_VOICE���ݽṹ

#define	VOICE_DATA_PKT_NUM		(20)
#define	SIZEOF_VOICE_DATA_SLOT	(160)

struct	tagVOICE_DATA
{
	u8 pkt_max;		//������ݰ����
	u8 pkt_cur;		//��ǰ���ݰ����
	u8 dat[SIZEOF_VOICE_DATA_SLOT];	//������
};
#define	SIZEOF_VOICE_DATA_PKT	(sizeof(struct tagMON_PKT_HDR) + sizeof(struct	tagVOICE_DATA) + 2)

/*=====================================================================*/

#pragma	pack()

/*=====================================================================*/
#define	SLEEP_PKT_TAG		0xF0A2	//������ģ���������.
//A2,F0,FE,00,92,00

/*=====================================================================*/

#define	VOICE_STATUS_NONE			0
#define	VOICE_STATUS_POWER_ON		1
#define	VOICE_STATUS_POWER_OFF		2
#define	VOICE_STATUS_RECODE_RESTART	3	//���¿�ʼ¼��
#define	VOICE_STATUS_STOP			4	//��ͣ
#define	VOICE_STATUS_PLAY			5
#define	VOICE_STATUS_PLAY_SOUND_1	6	//������ʾ��

extern u8 voice_rec_buf[VOICE_DATA_PKT_NUM*SIZEOF_VOICE_DATA_SLOT];
extern u8 voice_play_buf[VOICE_DATA_PKT_NUM*SIZEOF_VOICE_DATA_SLOT];
extern int voice_rec_count;
extern int voice_play_count;
extern bool_t IsTransfer;

const unsigned char test_voice_data[];
const unsigned char voice_data_1[];

int VoiceGetStatus(void);
void VoiceSetStatus(int status);
int Voice_GetRecodeCount(void);

/*=====================================================================*/

void IOT_Startup(void);


void IOT_CommonInit(void);
void DebugPutHex(u32 Offset,u32 Count,const void *Buf);
bool_t	IsTransferBoard(void);
void IOT_LED_ON(void);
void IOT_LED_OFF(void);
u8  IOT_GetExternVoltage(void);

int	IOT_MakePacket(u8 *buf,u8 cmd,const void *dat,u8 len,struct tagUpdatePara *pUpdatePara);
bool_t	IOT_SendPacket(u8 cmd,const void *dat,u8 len,struct tagUpdatePara *pUpdatePara);
int		IOT_GetPacket(u8 *buf,u32 wait_ms);

void IOT_TemperatureModuleInit(void);
s8  IOT_GetTemperature(void);

void IOT_SmogModuleInit(void);
bool_t	IOT_GetSmogStatus(void);
void IOT_SmogCheck_200ms(void);

void IOT_VoiceModuleInit(void);
void IOT_VoicePowerOn(void);
void IOT_VoicePowerOff(void);
void IOT_VoiceEncodeEnable(void);
void IOT_VoiceEncodeDisable(void);
u8   IOT_GetVoiceVoltage(void);
void IOT_VoiceResetVolume(void);
void IOT_VoiceVoltageCheck_200ms(void);
//void VoicePlay(u8 *play_buf,u32 size);
//int Voice_Recode(u8* rec_buf,int size);

int MA2400_Send(const u8 *buf,u32 size);
int MA2400_Receive(u8 *buf,u32 size,u32 wait_ms);
void MA2400_SetWorkingMode(u8 mode);
void MA2400_SetBaudrate(void);
void MA2400_SetEncoderEnable(bool_t enable);
void MA2400_SetDecodeEnable(bool_t enable);
void MA2400_SetChecksumEnable(bool_t enable);
void MA2400_SetVolume(void);


void IOT_SX1278_ModuleInit(void);
void SX1278_EnterNormalMode(void);
void SX1278_EnterCenterMode(void);
void SX1278_SendWakeupCode(void);
int	SX1278_Send(const u8 *buf,u32 size);
int	SX1278_Receive(u8 *buf,u32 size,u32 wait_ms);

void SX1278_WriteCfg(const u8 *cfg_dat);
bool_t SX1278_ReadCfg(u8 *cfg_buf);

bool_t SX1278_SetNetId(u8 net_id);
bool_t SX1278_GetId(u8 *net_id,u8 *user_id_h,u8 *user_id_l);

void    SX1278_EnterSleep(void);

//bool_t SX1278_SetId(u8 net_id,u8 user_id_h,u8 user_id_l);
//bool_t SX1278_GetId(u8 *net_id,u8 *user_id_h,u8 *user_id_l);

void IOT_MonitorMain(bool_t enter_sleep);
void IOT_TransferMain(void);

void IOT_MonitorUpdate(void);
void IOT_TransferUpdate(void);
void IOT_Update(void);
void TransferToMonitorUpdate(void);
void HandleAfterUpdate(void);
int Transfer_SendToHost(const u8 *buf,int size);
void UpdateRecvData(u8 role,struct tagMON_PKT_HDR* iot_hdr);
s32 DealECCandWriteFlash(u8 *pdata,u16 pkgindex,u8 len,u32 ecc,u8 role);

/*=====================================================================*/
/*=====================================================================*/

#ifdef	__cplusplus
}
#endif

#endif	/*__IOT_H__*/
