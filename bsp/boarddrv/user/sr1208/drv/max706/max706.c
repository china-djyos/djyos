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
#include "cpu_peri.h"
#include "wdt_hal.h"


#define WDT_TIM    TIM12    //���������õ��Ķ�ʱ��TIM12
// BOARD WDT
// =============================================================================
const Pin wdt_pin[] = {
{GPIO_H,PIN12,GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE,AF_NUll},
};

#define WDT_FEED_CYCLE  (1600*1000) //1.6S
static u32 sBootDogFeedTime = 0;
// =============================================================================
// ���ܣ����Ͽ��Ź�ι������,max706��ι��ʱ����1.6s,�����ֲᣬι���Ǳ��ش���
// ��������
// ���أ���
// =============================================================================
bool_t BrdWdt_FeedDog(void)
{
    if( PIO_Get(wdt_pin) )
    {
        PIO_Clear(wdt_pin);
    }
    else
    {
        PIO_Set(wdt_pin);
    }
    return true;
}


bool_t BrdBoot_FeedEnd(void);
u32 FeedDog_Isr(ptu32_t intline)
{
	WDT_TIM->CNT = 0;
	WDT_TIM->SR = 0;//���жϱ�־
	BrdWdt_FeedDog();
	Int_ClearLine(intline);
	return 1;
}
bool_t BrdBoot_FeedStart(u32 bootfeedtime)
{
	u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
	sBootDogFeedTime = bootfeedtime;
	RCC->APB1ENR |=RCC_APB1ENR_TIM12EN;
	WDT_TIM->CR1 &= ~(TIM_CR1_CEN); //��ֹTIMER
	WDT_TIM->CR1 |= TIM_CR1_ARPE | TIM_CR1_DIR;//�Զ���װ
	WDT_TIM->DIER |= TIM_DIER_UIE;//ʹ�ܸ����ж�
	WDT_TIM->PSC = 4000-1;//��Ƶϵ�� Ϊ�� ����Ƶ(1/108)1uS
	WDT_TIM->ARR = 27000;//��ʱ��Ԥװ��ֵ
    Int_Register(irqline);
    Int_IsrConnect(irqline,FeedDog_Isr);
    Int_SettoReal(irqline);
    Int_ClearLine(irqline);
    Int_RestoreRealLine(irqline);
    WDT_TIM->CR1 |= (TIM_CR1_CEN); //ʹ��TIMER
	return true;
}

bool_t BrdBoot_FeedEnd(void)
{
	u8 irqline = CN_INT_LINE_TIM8_BRK_TIM12;
	BrdWdt_FeedDog();
	WDT_TIM->CR1 &=~(TIM_CR1_CEN); //��ֹTIMER
	Int_SaveRealLine(irqline);
    Int_IsrDisConnect(irqline);
    Int_UnRegister(irqline);
    RCC->APB1ENR &=(~RCC_APB1ENR_TIM12EN);
	return true;
}

// =============================================================================
// ���ܣ����Ͽ��Ź�оƬ��ʼ��
// ��������
// ���أ���
// =============================================================================
void ModuleInstall_BrdWdt(void)
{
//��initcpuc.c�г�ʼ��������usb����ʱ������Ҫ��usb�������в�ͣι��.by zhb20170314
	PIO_Configure(wdt_pin, PIO_LISTSIZE(wdt_pin));

    BrdWdt_FeedDog();

    WdtHal_RegisterWdtChip("Wdt_MAX706",WDT_FEED_CYCLE/4,BrdWdt_FeedDog,\
    		BrdBoot_FeedStart,BrdBoot_FeedEnd);
}







