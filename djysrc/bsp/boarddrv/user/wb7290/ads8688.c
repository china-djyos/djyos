// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��ADS8668.c
// ģ������: ADS8688�ɼ�ģ��ĵײ�������������SPI�ӿ�ͨ�ź�����ȡAD�ɼ�ֵ
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
//#include "spibus.h"
#include "cpu_peri.h"

static const Pin ADRST[] = {
    {PIO_PB5, PIOB, ID_PIOB, PIO_OUTPUT_1, PIO_DEFAULT},//ADRST
};

static const Pin ADCS[] = {
        {PIO_PA31, PIOA, ID_PIOA, PIO_OUTPUT_1, PIO_DEFAULT},//ADCS
};

static const Pin ADSH[] = {
        {PIO_PD26, PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT}

};

//Command Register
#define NO_OP       0X0000
#define STDBY       0X8200
#define PWR_DN      0X8300
#define RST         0X8500
#define AUTO_RST    0XA000


#define WRITE 1
#define READ 0
// =============================================================================
#define ADS8688_SPI_BAUD     (16000000)      //ADS8688��SSP�����ٶ�,���17MHZ
#define ADS8688_CS           (0)

// =============================================================================
// ���ܣ���SPI���ռĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ
// ���أ�����������
// =============================================================================
static s32 __SPI_Read( Spi * Reg )
{
    if ( (Reg->SPI_SR & SPI_SR_RDRF) == 0 ) 
    {
        Djy_DelayUs((1000000*16+1000000*16 -1)/ADS8688_SPI_BAUD);
        if ( (Reg->SPI_SR & SPI_SR_RDRF) == 0 ) 
            return -1;
    }

    return (s32)(Reg->SPI_RDR & 0xFFFF) ;
}

// =============================================================================
// ���ܣ�дSPI���ͼĴ�������ѯ��ʽ,��__SPI_TxRxPoll����
// ������Reg,�Ĵ�����ַ��dwNpcs,Ƭѡ��wData��д����
// ���أ���
// =============================================================================
static s32 __SPI_Write( Spi * Reg, uint32_t dwNpcs, uint16_t wData )
{
    u8 pcs = ((~(1 << dwNpcs) & 0xF) << 16);
    /* Send data */
    if ( (Reg->SPI_SR & SPI_SR_TXEMPTY) == 0 ) 
    {
        Djy_DelayUs((1000000*16+1000000*16 -1)/ADS8688_SPI_BAUD);
        if ( (Reg->SPI_SR & SPI_SR_TXEMPTY) == 0 ) 
            return -1;
    }
    Reg->SPI_TDR = wData | pcs ;
    return 0;
//  while ( (Reg->SPI_SR & SPI_SR_TDRE) == 0 ) ;
}
static void __ADS8688_SpiInit(u8 port)
{
	//�Ѿ���board.c�����ʼ����SPI0������
    PIO_Configure(ADCS,1);
    PIO_Configure(ADSH,1);
    PMC_EnablePeripheral(ID_SPI0);

    SPI0->SPI_CR = SPI_CR_SPIDIS|SPI_CR_SWRST;           //��λ������
    SPI0->SPI_MR = SPI_MR_MSTR|SPI_MR_MODFDIS;

    SPI0->SPI_CSR[ADS8688_CS] = SPI_CSR_BITS_16_BIT
                    | SPI_CSR_SCBR(CN_CFG_MCLK/2/ADS8688_SPI_BAUD);

    SPI0->SPI_CR = SPI_CR_SPIEN;
}

static bool_t __ADS8688_SpiTrans(u16 *wBuf, u32 wlen, u16 *rBuf,u32 rlen)
{
    u32 i;
    s32 ret;
    bool_t result = false;

	SPI0->SPI_CSR[ADS8688_CS] &= ~SPI_CSR_NCPHA;

    PIO_Clear(ADCS);

	for(i = 0; i < wlen; i++)
	{
    	ret = __SPI_Write(SPI0,ADS8688_CS,wBuf[i]);
    	if(ret == -1)
    		goto SPI_EXIT;
    	ret = __SPI_Read(SPI0);
    	if(ret == -1)
    		goto SPI_EXIT;
	}

   SPI0->SPI_CSR[ADS8688_CS] |= SPI_CSR_NCPHA;

	for(i = 0; i < rlen; i ++)
	{
    	ret = __SPI_Write(SPI0,ADS8688_CS,0x0000);
    	if(ret == -1)
    		goto SPI_EXIT;
    	ret = __SPI_Read(SPI0);
    	if(ret == -1)
    	{
    		goto SPI_EXIT;
    	}
    	rBuf[i] = (u16)(ret & 0xFFFF);
	}

	result = true;

SPI_EXIT:
	SPI0->SPI_CR = SPI_CR_LASTXFER;
	PIO_Set(ADCS);
	return result;
}

// =============================================================================

static void __ADS8688_WriteCommandReg(u16 command)//дADS8688����Ĵ���
{
    __ADS8688_SpiTrans(&command,1,NULL,0);
}

static void __ADS8688_EnterAutoRstMode(void)    //�����Զ�ɨ��ģʽ
{
    __ADS8688_WriteCommandReg(AUTO_RST);
}

static void __ADS8688_EnterRstMode(void)
{
    __ADS8688_WriteCommandReg(RST);
}
// =============================================================================
// ���ܣ���ʼ��ADS8688�������������úͳ�ʼ�����ã��ӿڵĳ�ʼ����
// ��������
// ���أ���
// =============================================================================
bool_t ADS8688_Init(u8 spi_port)
{
    __ADS8688_SpiInit(spi_port);

    //��PB4 ��PB5����Ϊͨ��IO
    MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO5;

    PIO_Configure(ADRST,PIO_LISTSIZE(ADRST));
    PIO_Set(ADRST);

    // ���³�ʼ����SSP���
    //CS��ʼ��
    PIO_Configure(ADCS,PIO_LISTSIZE(ADCS));
    PIO_Set(ADCS);

    __ADS8688_EnterRstMode();
    __ADS8688_EnterAutoRstMode();

    return true;
}


// =============================================================================
// ���ܣ���ȡADS8688�Ĳɼ�ֵ������������ת����ת���������ٶ�ȡADת����ֵ
// ��������
// ���أ���
// =============================================================================
bool_t ADS8688_GetValue(uint16_t *AinBuf)
{
    u16 i,chnum = 8;;
    u16 bData = {0x0000};
    u16 tembuf[16];
    u16 *pAinBuf = (u16*)tembuf;
    bool_t ret = true;

    if(AinBuf == NULL)
        return false;

    PIO_Set(ADSH);

	for (i = 0; i < chnum; i ++)
	{
		ret = __ADS8688_SpiTrans(&bData,1,pAinBuf,2);
		if(ret == false)
			goto ADS8688_EXIT;
		pAinBuf += 2;
	}

	for(i = 0; i < chnum*2; i++)
	{
		if(i<8)
		{
			AinBuf[i] = tembuf[1+2*i];
		}
		else
		{
			AinBuf[i] = tembuf[(i-8)*2];
		}
	}

ADS8688_EXIT:
    PIO_Clear(ADSH);
    return ret;
}

