// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��brdwdt.c
// ģ������: SR3200�Ŀ��Ź���������ʼ����ι������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "djyos.h"
#include "cpu_peri_gpio.h"


#define CLRDOG_PORT		(0)		//CLRDOG_L p0.30
#define CLRDOG_PIN		(30)

#define WDT_FEED_CYCLE  (1600*1000)	//1.6S

// =============================================================================
// ���ܣ����Ͽ��Ź�ι������,max706��ι��ʱ����1.6s,�����ֲᣬι���Ǳ��ش���
// ��������
// ���أ���
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
//	printk("wdtR\r\n");
    if( GPIO_DataGet( CLRDOG_PORT) & ( 1<<CLRDOG_PIN) )
    {
        GPIO_SetToLow( CLRDOG_PORT, ( 1<<CLRDOG_PIN));
    }
    else
    {
        GPIO_SetToHigh( CLRDOG_PORT, ( 1<<CLRDOG_PIN));
    }

    return true;
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ��
// ��������
// ���أ���
// =============================================================================
void BrdWdt_ModuleInit(void)
{
	GPIO_CfgPinFunc(CLRDOG_PORT,CLRDOG_PIN,0);
	GPIO_CfgPinMode(CLRDOG_PORT,CLRDOG_PIN,CN_GPIO_MODE_OD_PULLUP);
	GPIO_SetToOut(CLRDOG_PORT,(1 << CLRDOG_PIN));			//����Ϊ���

	// LPC17xx ��p0.29��p0.30����Ϊ��ͬ���
	GPIO_CfgPinFunc(0,29,0);
	GPIO_CfgPinMode(CLRDOG_PORT,CLRDOG_PIN,CN_GPIO_MODE_OD_PULLUP);
	GPIO_SetToOut(0,(1 << 29));			//����Ϊ���

	BrdWdt_FeedDog();

	WdtHal_RegisterWdtChip("board_Wdt",WDT_FEED_CYCLE,BrdWdt_FeedDog,NULL,NULL);
}







