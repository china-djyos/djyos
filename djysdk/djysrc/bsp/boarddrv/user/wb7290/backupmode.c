// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��brdwdt.c
// ģ������: SR3200�Ŀ��Ź���������ʼ����ι������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================

#if 0
#include <stdint.h>
#include "os.h"
#include <samv7/samv71.h>
#include "cpu_peri_int_line.h"
#include "cpu_peri_pio.h"
#include "supc.h"
#include "pmc.h"

//�������Ŷ�����Ϊ����������͵�ƽ
static const Pin AllPin[] = {
		{0xFFFFFFFF,PIOA,ID_PIOA,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOB,ID_PIOB,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOC,ID_PIOC,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOD,ID_PIOD,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOE,ID_PIOE,PIO_OUTPUT_0,PIO_DEFAULT},
};

#define SCR_SLEEPDEEP   (0x1 <<  2)
const uint32_t gWakeUpPinId = (1 << 7);			//���ѹ���Pin��
// ������������Ϊ����
static const Pin VccWkup_Pin[] = {
		{PIO_PA11, PIOA, ID_PIOA, PIO_INPUT,    PIO_DEFAULT}
};


// =============================================================================
// ���ܣ�����͹���ģʽ���ȹرյ�Դ��⹦�ܣ�����Ҫ�������޷����ѣ�����������������Ϊ���
//      �͵�ƽ����������������Ϊ���룬�����û�������
// ��������
// ���أ���
// ע�ͣ����Ѳ��û����������룬���������ϳ���������ʱ��CPU��back up mode����
// =============================================================================
void BackupMode_Entry(void)
{
	u32 smmr;
	u32 SupcRegSr;

	SupcRegSr = SUPC->SUPC_SR ;

	//�ر� supply monitor����
	smmr = SUPC_SMMR_SMTH(0x0A) | SUPC_SMMR_SMSMPL_SMD |
			SUPC_SMMR_SMRSTEN_NOT_ENABLE;

	//enable ext32k
	SUPC_ConfigSupplyMonitor(smmr);

	MATRIX->CCFG_SYSIO |= (1 << 4) | (1 << 5) |(1 << 6) |(1 << 7);
	PIO_Configure(AllPin,PIO_LISTSIZE(AllPin));

	/* Disable UTMI PLL clock */
	PMC->CKGR_UCKR &= ~CKGR_UCKR_UPLLEN;

	/* Disable PCK */
	PMC->PMC_SCDR = 0xFFFFFFFF;

	/* Disable all the peripheral clocks */
	PMC_DisableAllPeripherals();

	PIO_Configure(VccWkup_Pin,1);

	/* Enable the PIO for wake-up */
	SUPC->SUPC_WUIR = (gWakeUpPinId << 16) | gWakeUpPinId ;
	PMC->PMC_FSMR   = PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN | gWakeUpPinId;
	/* Set the SLEEPDEEP bit of Cortex-M processor. */
	SCB->SCR |= SCR_SLEEPDEEP ;
	/* Set the VROFF bit of SUPC_CR. */
	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
	SUPC->SUPC_WUMR = SUPC_WUMR_WKUPDBC_3_SLCK;
}

u32 Supc_IRQHandler(ptu32_t para)
{
	BackupMode_Entry();
	Int_ClearLine(CN_INT_LINE_SUPC);
	return 0;
}

// =============================================================================
// ���ܣ��͹���ģ��ģ�����ã����õ�ѹˮƽ�������жϣ�����ѹ���͵���ѹˮƽʱ�������жϣ����ж�
//       �������У�����back up mode
// ������
// ���أ�
// =============================================================================
void BackupMode_Config(void)
{
	u32 smmr;

	u32 SupcRegSr;

	SupcRegSr = SUPC->SUPC_SR ;

	SUPC->SUPC_MR |= SUPC_MR_BKUPRETON;				//backup sram on

	//���õ�ѹthreshold�����������ڡ���λʹ�ܡ��ж�ʹ��
	//��ѹ����2.68V����ʱ�������ж�
	//0x08 ----2.56V; 0x09 --- 2.68V ; 0x0A ---- 2.8V; 0x0B ----3.04V
	smmr = SUPC_SMMR_SMTH(0x0A) | SUPC_SMMR_SMSMPL_CSM |
			SUPC_SMMR_SMRSTEN_NOT_ENABLE | SUPC_SMMR_SMIEN;

	//enable ext32k
	SUPC_ConfigSupplyMonitor(smmr);

	//����ѹ���ͺ󣬽����жϣ����ý���BACKUPMODE
	Int_Register(CN_INT_LINE_SUPC);
    Int_IsrConnect(CN_INT_LINE_SUPC,Supc_IRQHandler);
    Int_SettoReal(CN_INT_LINE_SUPC);
    Int_ClearLine(CN_INT_LINE_SUPC);
    Int_RestoreRealLine(CN_INT_LINE_SUPC);
}
#endif


#if 1

#include <stdint.h>
#include <stdio.h>

#include <cpu_peri_int_line.h>
#include <samv7/samv71.h>
#include <cpu_peri_pio.h>

//�������Ŷ�����Ϊ����������͵�ƽ
static const Pin AllPin[] = {
		{0xFFFFFFFF,PIOA,ID_PIOA,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOB,ID_PIOB,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOC,ID_PIOC,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOD,ID_PIOD,PIO_OUTPUT_0,PIO_DEFAULT},
		{0xFFFFFFFF,PIOE,ID_PIOE,PIO_OUTPUT_0,PIO_DEFAULT},
};

#define SCR_SLEEPDEEP   (0x1 <<  2)
const uint32_t gWakeUpPinId = (1 << 12);			//���ѹ���Pin��
// ������������Ϊ����
static const Pin VccWkup_Pin[] = {
		{PIO_PB3, PIOB, ID_PIOB, PIO_INPUT,    PIO_DEFAULT}
};

static const Pin LpEntry_Pin[] = {
		{PIO_PB3, PIOB, ID_PIOB, PIO_INPUT,    PIO_IT_FALL_EDGE}
};
// =============================================================================
// ���ܣ�����͹���ģʽ���ȹرյ�Դ��⹦�ܣ�����Ҫ�������޷����ѣ�����������������Ϊ���
//      �͵�ƽ����������������Ϊ���룬�����û�������
// ��������
// ���أ���
// ע�ͣ����Ѳ��û����������룬���������ϳ���������ʱ��CPU��back up mode����
// =============================================================================
void BackupMode_Entry(void)
{
	MATRIX->CCFG_SYSIO |= (1 << 4) | (1 << 5) |(1 << 6) |(1 << 7);
	PIO_Configure(AllPin,PIO_LISTSIZE(AllPin));

	/* Disable UTMI PLL clock */
	PMC->CKGR_UCKR &= ~CKGR_UCKR_UPLLEN;

	/* Disable PCK */
	PMC->PMC_SCDR = 0xFFFFFFFF;

	/* Disable all the peripheral clocks */
	PMC_DisableAllPeripherals();

	PIO_Configure(VccWkup_Pin,1);

	/* Enable the PIO for wake-up */
	SUPC->SUPC_WUIR = (gWakeUpPinId << 16) | gWakeUpPinId ;
	PMC->PMC_FSMR   = PMC_FSMR_FLPM_FLASH_DEEP_POWERDOWN | gWakeUpPinId;
	/* Set the SLEEPDEEP bit of Cortex-M processor. */
	SCB->SCR |= SCR_SLEEPDEEP ;
	/* Set the VROFF bit of SUPC_CR. */
	SUPC->SUPC_CR = SUPC_CR_KEY_PASSWD | SUPC_CR_VROFF_STOP_VREG;
	SUPC->SUPC_WUMR = SUPC_WUMR_WKUPDBC_3_SLCK;
}

u32 Lp_IRQHandler(ptu32_t para)
{
	BackupMode_Entry();
//	Int_ClearLine(CN_INT_LINE_SUPC);
	return 0;
}

// =============================================================================
// ���ܣ��͹���ģ��ģ�����ã����õ�ѹˮƽ�������жϣ�����ѹ���͵���ѹˮƽʱ�������жϣ����ж�
//       �������У�����back up mode
// ������
// ���أ�
// =============================================================================
void BackupMode_Config(void)
{
	SUPC->SUPC_MR |= SUPC_MR_BKUPRETON;
	PIO_InitInterrupts(LpEntry_Pin);
	PIO_ConfigureIt(LpEntry_Pin,Lp_IRQHandler);

	Int_SettoReal(CN_INT_LINE_PIOB);
	PIO_EnableIt(LpEntry_Pin);
}

#endif
