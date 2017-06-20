// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ADS8668.c
// ģ������: ADS8688�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 05/10.2017
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "shell.h"
#include "ads8688.h"
#include "harddrv_ad.h"


extern const Pin ADCS[];
// =============================================================================
#define ADS8688_CS           (0)
#define WRITE                 1
#define READ                  0

extern s32 SPI_Read(void);
extern s32 SPI_Write(uint32_t dwNpcs, uint16_t wData);

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������port, SPI��������ţ���sr5333-v2.0��ADS8688���õ���SPI2;
// ���أ���.
// =============================================================================
void ADS8688_SpiInit(uint8_t port)
{
	//�Ѿ���board.c�����ʼ����SPI2������
	 Spi *Reg;
	 Reg = (Spi *)SPI_BASE;
	 //����SPIʹ��GPIO����
	 RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // ʹ��SPI2
	 RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST; // ��λSPI2
	 RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; // ֹͣ��λSPI2

	 Reg->CR1 |=SPI_CR1_BR_0;//����������  108M/4=27MHz
	 Reg->CR1 &= ~SPI_CR1_CPOL;                      // ����ģʽ��SCKΪ1 CPOL=0
	 Reg->CR1 |= SPI_CR1_CPHA;                      // ���ݲ����ӵ�2��ʱ����ؿ�ʼ,CPHA=1
	 Reg->CR1 &= ~SPI_CR1_RXONLY;//ȫ˫��ͨѶģʽ
	 Reg->CR1 &= ~SPI_CR1_LSBFIRST;//����֡��ʽ
	 Reg->CR1 &= ~ SPI_CR1_SSM;         // ��ֹ���nss����
	 Reg->CR1 |= SPI_CR1_SSI;  //SSM
	 Reg->CR1 |= SPI_CR1_MSTR;       // SPI����

	 Reg->CR2 |=SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3 ;//����֡��ʽΪ16bit
	 Reg->CR2 |=SPI_CR2_SSOE;//SS ���ʹ��
	 Reg->CR2 |=SPI_CR2_NSSP;//���� NSS ����
	 Reg->CR2 &=~SPI_CR2_FRXTH;//16�ֽ�fifo
	 Reg->I2SCFGR &= (uint16_t)(~SPI_I2SCFGR_I2SMOD);// ѡ��SPIģʽ
	 Reg->CR1 |= SPI_CR1_SPE;
}

// =============================================================================
// ���ܣ�ADS8688��Ӧ��SPI��������ʼ��
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t __ADS8688_SpiTrans(uint16_t *wBuf, uint32_t wlen, uint16_t *rBuf,uint32_t rlen)
{
    uint32_t i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
   	Reg = (Spi *)SPI_BASE;
    PIO_Clear(ADCS);
	for(i = 0; i < wlen; i++)
	{
    	ret=SPI_Write(ADS8688_CS,wBuf[i]);
		if(ret==-1)
	      	goto SPI_EXIT;
    	ret=SPI_Read();
	    if(ret==-1)
	      	goto SPI_EXIT;
	}
	for(i = 0; i < rlen; i ++)
	{
    	ret=SPI_Write(ADS8688_CS,0x0000);
		if(ret==-1)
	      	goto SPI_EXIT;
    	 ret = SPI_Read();
		 if(ret==-1)
		    goto SPI_EXIT;

	     rBuf[i] = (uint16_t)(ret & 0xFFFF);
	}
  	result = true;
SPI_EXIT:
    PIO_Set(ADCS);
	return result;
}

// =============================================================================
void ADS8688_WriteCmdReg(uint16_t command)//дADS8688����Ĵ���
{
	uint16_t tmp=0;
    __ADS8688_SpiTrans(&command,1,&tmp,1);
}

void ADS8688_EnterAutoRstMode(void)    //�����Զ�ɨ��ģʽ
{
    ADS8688_WriteCmdReg(AUTO_RST);
}

void ADS8688_EnterRstMode(void)
{
    ADS8688_WriteCmdReg(RST);
}

// =============================================================================
// ���ܣ�Program Registerд����
// ������addr, Program Register��ַ;
//       data������дָ����Ч��Ϊд���мĴ�����ֵ.
// ���أ�
// =============================================================================
static bool_t ADS8668_WRProgReg(uint8_t addr,uint8_t data)
{
	uint16_t w_dat=0x0000;
	uint16_t r_dat=0x0000;
	bool_t ret=true;
	uint8_t temp=0x00;
	temp=addr<<1;
	temp=temp|WRITE;
	w_dat=temp;
	w_dat=w_dat<<8;
	w_dat=w_dat|data;
	ret=__ADS8688_SpiTrans(&w_dat,1,&r_dat,1);
	if(ret==false)
		return ret;
	r_dat=r_dat>>8;
	if(r_dat!=data)
		return false;
	return true;
}
// =============================================================================
// ���ܣ�����ͨ�����뷶Χ
// ������seq
// ���أ�
// =============================================================================
bool_t ADS8688_SetChInRange(uint8_t ch,uint8_t range)
{
	bool_t result;
	result=ADS8668_WRProgReg(ch,range);
	return result;
}


// =============================================================================
// ���ܣ���ȡɨ��ͨ�����е�ADת������
// ������seq
// ���أ�
// =============================================================================
bool_t ADS8688_GetRstModeData(uint8_t seq,uint16_t *pdata,uint8_t chnum)
{
	uint8_t i;
	uint16_t bData = 0x0000;
	bool_t ret=true;
	uint16_t temp=0;
	ret=ADS8664_WRProgReg(AUTO_SEQ_EN,seq);
	if(!ret)
		return false;
	ADS8688_EnterRstMode();
	for (i = 0; i < chnum; i ++)
	{
		ret=__ADS8688_SpiTrans(&bData,1,pdata,1);
		if(ret==false)
			return ret;
		temp=*pdata;
		*pdata=temp;
		pdata++;
	}
	return true;
}

// =============================================================================
// ���ܣ���ȡ�ֶ�ͨ����ADת������
// �������ޡ�
// ���أ�
// =============================================================================
uint16_t ADS8688_GetManChModeData(uint16_t ch)
{
	uint16_t dat=0;
	bool_t ret=true;
	uint16_t bData = 0x0000;
	//����ָ��ͨ��Ϊ�ֶ�ģʽ
	ADS8688_WriteCmdReg(ch);
	ret=ADS8688_SpiTrans(&bData,1,&dat,1);
	if(ret==false)
		return 0;
	return dat;
}


