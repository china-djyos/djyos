// =============================================================================
// Copyright (C) 2012-2020 �����߲���ϵͳ�з��Ŷ�  All Rights Reserved
// �ļ���     ��SPIbus.h
// ģ������: SPIģ��Ľṹ�弰API����
// ģ��汾: V1.00
// ������Ա: hm
// ����ʱ��: 02/07.2014
// =============================================================================

#ifndef __SPIBUS_H__
#define __SPIBUS_H__

#include "object.h"

typedef ptu32_t (*TransferFunc)(ptu32_t SpecificFlag,u32 sendlen,u32 recvlen,u32 recvoff);
typedef bool_t (*TransferPoll)(ptu32_t SpecificFlag,u8* srcaddr,u32 sendlen,
                               u8* destaddr,u32 recvlen,u32 recvoff,u8 cs);
typedef bool_t (*CsActiveFunc)(ptu32_t SpecificFlag, u8 cs);
typedef bool_t (*CsInActiveFunc)(ptu32_t SpecificFlag, u8 cs);
typedef ptu32_t (*SPIBusCtrlFunc)(ptu32_t SpecificFlag,u32 cmd,ptu32_t data1,ptu32_t data2);

struct SPI_Device;
struct SPI_Param
{
    char            *BusName;               //�������ƣ���SPI1
    u8              *SPIBuf;                //���߻�����ָ��
    u32             SPIBufLen;              //���߻�������С���ֽ�
    ptu32_t         SpecificFlag;           //SPI˽�б�ǩ������ƼĴ�����ַ
    bool_t          MultiCSRegFlag;         //SPI���ƼĴ����Ƿ��ж���CS���üĴ���
    TransferFunc    pTransferTxRx;          //���ͽ��ջص��������жϷ�ʽ
    TransferPoll    pTransferPoll;          //���ͽ��ջص���������ѯ��ʽ
    CsActiveFunc    pCsActive;              //Ƭѡʹ��
    CsInActiveFunc  pCsInActive;            //Ƭѡʧ��
    SPIBusCtrlFunc  pBusCtrl;               //���ƺ���
};

struct SPI_DataFrame
{
    u8* SendBuf;        //��������ָ��
    u32 SendLen;        //�������ݳ��ȣ��ֽ�
    u8* RecvBuf;        //��������ָ��
    u32 RecvLen;        //�������ݳ��ȣ��ֽ�
    u32 RecvOff;        //��������ƫ��
};

// SPI mode flags
#define SPI_CPHA    0x01            /* clock phase */
#define SPI_CPOL    0x02            /* clock polarity */
#define SPI_MODE_0  (0|0)        //clk����Ϊ�͵�ƽ ��һ�������زɼ�/�������
#define SPI_MODE_1  (0|SPI_CPHA) //clk����Ϊ�͵�ƽ �ڶ��������زɼ�/�������
#define SPI_MODE_2  (SPI_CPOL|0) //clk����Ϊ�ߵ�ƽ ��һ�������زɼ�/�������
#define SPI_MODE_3  (SPI_CPOL|SPI_CPHA)//clk����Ϊ�͵�ƽ �ڶ��������زɼ�/�������


#define SPI_SHIFT_MSB  0
#define SPI_SHIFT_LSB  1

#define SPI_MODE_SLAVE  0
#define SPI_MODE_MASTER 1

typedef struct SpiConfig
{
   u8 Mode;     //����CPHA,CPOL
   u8 ShiftDir; //MSB or LSB
   u8 CharLen;  //�ַ�����,bits
   u32 Freq;    //ʱ��Ƶ��
} tagSpiConfig;

#define CN_INVALID_CS               0xFF    //��Ч��Ƭѡ

//����BusCtrl()�Ĳ���cmd,0x80������ϵͳ���壬0x80�����û�����
#define CN_SPI_SET_CLK               0
#define CN_SPI_SET_ERROR_POP_EVENT   1
#define CN_SPI_CS_CONFIG             2
#define CN_SPI_SET_AUTO_CS_EN        3
#define CN_SPI_SET_AUTO_CS_DIS       4
#define CN_SPI_SET_POLL              5
#define CN_SPI_SET_INT               6
#define CN_SPI_OS_USED               0x80

//�����������
#define CN_SPI_EXIT_NOERR                   0
#define CN_SPI_EXIT_UNKNOW_ERR              -1
#define CN_SPI_EXIT_TIMEOUT                 -2
#define CN_SPI_EXIT_PARAM_ERR               -3


//���巢�����󵯳��¼�����ֵ
#define CN_SPI_POP_UNKNOW_ERR               (0x01 << 16)        //δ֪����

//SPIģ��API����
bool_t ModuleInstall_SPIBus(void);

struct SPI_CB *SPI_BusAdd(struct SPI_Param *NewSPIParam);
bool_t SPI_BusDelete(struct SPI_CB *DelSPI);
struct SPI_CB *SPI_BusFind(const char *BusName);

struct SPI_Device *SPI_DevAdd(const char *BusName ,const char *DevName,u8 cs,u8 charlen,
                                u8 mode,u8 shiftdir,u32 freq,u8 autocs);
bool_t SPI_DevDelete(struct SPI_Device *DelDev);
struct SPI_Device *SPI_DevFind(const char *BusName ,const char *DevName);

s32 SPI_Transfer(struct SPI_Device *Dev,struct SPI_DataFrame *spidata,
                u8 block_option,u32 timeout);
bool_t SPI_CsActive(struct SPI_Device *Dev,u32 timeout);
bool_t SPI_CsInactive(struct SPI_Device *Dev);
s32  SPI_PortRead( struct SPI_CB *pSPI,u8 *buf,u32 len);
s32  SPI_PortWrite(struct SPI_CB *pSPI,u8 *buf,u32 len);
s32  SPI_BusCtrl(struct SPI_Device *Dev,u32 cmd,ptu32_t data1,ptu32_t data2);
s32  SPI_ErrPop(struct SPI_CB *pSPI, u32 ErrNo);

#endif /* __SPIBUS_H__ */
