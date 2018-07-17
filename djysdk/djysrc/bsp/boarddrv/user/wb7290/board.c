//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"


//�����DBG���Դ���ʹ��
static const Pin uart1_pin[] = {
    {PIO_PA5C_URXD1, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT},
    {PIO_PA6C_UTXD1, PIOA, ID_PIOA, PIO_PERIPH_C, PIO_DEFAULT}};

//�����print���ܣ����Ӵ�ӡ��
static const Pin uart3_pin[] = {
    {PIO_PD28A_URXD3, 	PIOD, ID_PIOD, PIO_PERIPH_A, PIO_PULLUP},
    {PIO_PD30A_UTXD3, 	PIOD, ID_PIOD, PIO_PERIPH_A, PIO_PULLUP}
};

//���ʹ��485����
static const Pin usart1_pin[] = {
    {PIO_PA21A_RXD1, 	PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP},
    {PIO_PB4D_TXD1,		PIOB, ID_PIOB, PIO_PERIPH_D, PIO_PULLUP},
	{PIO_PA24, 			PIOA, ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT}	//485 RX TX
};
static const Pin usart2_pin[] = {
    {PIO_PD15B_RXD2, 	PIOD, ID_PIOD, PIO_PERIPH_B, PIO_PULLUP},
    {PIO_PD16B_TXD2,	PIOD, ID_PIOD, PIO_PERIPH_B, PIO_PULLUP},
	{PIO_PD18, 			PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT}	//485 RX TX
};


//AT45�õ�QSPI��SPI MODE
static const Pin Qspi_pin[] = {
		{PIO_PA11A_QCS,PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
		{PIO_PA13A_QIO0,PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
		{PIO_PA12A_QIO1,PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
//		{PIO_PA17A_QIO2,PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
//		{PIO_PD31A_QIO3,PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
		{PIO_PA14A_QSCK,PIOA, ID_PIOA, PIO_PERIPH_A, PIO_DEFAULT},
};

static const Pin Spi0_pin[] = {
		{PIO_PD20B_SPI0_MISO,PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT},
		{PIO_PD21B_SPI0_MOSI,PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT},
		{PIO_PD22B_SPI0_SPCK,PIOD, ID_PIOD, PIO_PERIPH_B, PIO_DEFAULT},
};

static const Pin AT45_WP_pin [] = {
		{PIO_PE5, PIOE, ID_PIOE, PIO_OUTPUT_0, PIO_DEFAULT},
};

//����IIC��GPIO��������
static const Pin Twi0_Pins[]={
        {PIO_PA3A_TWD0,PIOA,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT},
        {PIO_PA4A_TWCK0,PIOA,ID_PIOA,PIO_PERIPH_A,PIO_DEFAULT}
};

static const Pin AT24_WP_pin [] = {
		{PIO_PA2,PIOA,ID_PIOA,PIO_OUTPUT_0,PIO_DEFAULT},
};

static const Pin Gmac_pin[] = {
		{(PIO_PD0A_GTXCK | PIO_PD1A_GTXEN | PIO_PD2A_GTX0 | PIO_PD3A_GTX1
		| PIO_PD4A_GRXDV | PIO_PD5A_GRX0 | PIO_PD6A_GRX1 | PIO_PD7A_GRXER
		| PIO_PD8A_GMDC | PIO_PD9A_GMDIO ),PIOD, ID_PIOD, PIO_PERIPH_A, PIO_DEFAULT},
};

static const Pin Phy_Interrupt_Pin[] = {
		{PIO_PC13, PIOC, ID_PIOC, PIO_INPUT,    PIO_IT_FALL_EDGE}
};

static const Pin AD5697R_LDAC_pin[] = {
		{PIO_PD25, PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT},
};

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����͹���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{
	switch(SerialNo)
	{
	case CN_USART1:
		PIO_Set(&usart1_pin[2]);
		break;
	case CN_USART2:
		PIO_Set(&usart2_pin[2]);
		break;
	default:
		break;
	}
}

// ============================================================================
// ���ܣ����ô��ڱ��ΪSerialNo�Ĵ���Ϊ��˫�����չ���
// ������SerialNo,���ں�
// ���أ���
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{
	//����Ϊ485����
	switch(SerialNo)
	{
	case CN_USART1:
		PIO_Clear(&usart1_pin[2]);
		break;
	case CN_USART2:
		PIO_Clear(&usart2_pin[2]);
		break;
	default:
		break;
	}
}


extern u32 *pProgRomEnd;
void ProgRegionGet(u32 *pStart, u32 *pEnd)
{
	*pStart = 0x00400000;		//������ʼ��ַ
	*pEnd = (u32)(&pProgRomEnd);	//���������ַ
}

void Board_GpioInit(void)
{
	PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));		//UART
	PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
	MATRIX->CCFG_SYSIO |= CCFG_SYSIO_SYSIO4;
	PIO_Configure(usart1_pin,PIO_LISTSIZE(usart1_pin));
	PIO_Configure(usart2_pin, PIO_LISTSIZE(usart2_pin));

	PIO_Configure(Qspi_pin, PIO_LISTSIZE(Qspi_pin));		//AT45
	PIO_Configure(AT45_WP_pin, PIO_LISTSIZE(AT45_WP_pin));
	PIO_Set(AT45_WP_pin);

	PIO_Configure(Spi0_pin, PIO_LISTSIZE(Spi0_pin));		//SPI0

	PIO_Configure(Twi0_Pins,PIO_LISTSIZE(Twi0_Pins));		//AT24
	PIO_Configure(AT24_WP_pin,PIO_LISTSIZE(AT24_WP_pin));
	PIO_Clear(AT24_WP_pin);

	PIO_Configure(Gmac_pin, PIO_LISTSIZE(Gmac_pin));		//GMAC
//	extern void LS_IntHandler(void);
//	PIO_InitInterrupts(Phy_Interrupt_Pin);
//	PIO_ConfigureIt(Phy_Interrupt_Pin,LS_IntHandler);
//	PIO_EnableIt(Phy_Interrupt_Pin);

	PIO_Configure(AD5697R_LDAC_pin,1);
}

void AT24_WPEn(u8 En)
{
	if(En)
	{
		PIO_Set(AT24_WP_pin);
	}
	else
	{
		PIO_Clear(AT24_WP_pin);
	}
}
