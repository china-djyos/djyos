//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// ģ��汾: V1.00
// ������Ա:czz
// ����ʱ��:2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#define LSE_Flag_Reg 0xA5A5
#define LSI_Flag_Reg 0xA5A0

#define BAK_Reg   2

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))

static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //����µ�ʱ��

// =============================================================================
// ���ܣ�RTC��������SRAM
// ������BKPx:�����Ĵ������,��Χ:0~19
// ���أ�����������
// =============================================================================

u32 BKP_ReadBackupRegister(u32 BKRx)
{
	u32 temp=0;
	temp=RTC_BASE+0x50+BKRx*4;
	return (*(u32*)temp);		//���ض�ȡ����ֵ
}

// =============================================================================
// ���ܣ�RTCд�������SRAM
// ������BKPx:�����Ĵ������,��Χ:0~19
//	   data:Ҫд�������,32λ����
// ���أ�true,�����������������
// =============================================================================

void BKP_WriteBackupRegister(u32 BKRx,u32 data)
{
	u32 temp=0;
	temp=RTC_BASE+0x50+BKRx*4;
	(*(u32*)temp)=data;
}
// =============================================================================
// ���ܣ��ȴ�RSFͬ�� ��ֹ��ͬ��ʱ��ȡ������
// ������void
// ���أ�
// =============================================================================
bool_t RTC_Wait_Rsf(void)
{
	u32 retry=0XFFFFF;

	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	RTC->ISR&=~(1<<5);		//���RSFλ

	while(retry&&((RTC->ISR&(1<<5))==0x00))
	{
		retry--;
	}

    if(retry==0)
    	return false; //ͬ��ʧ��
	RTC->WPR=0xFF;	 //ʹ��RTC�Ĵ���д����
	return true;
}
// =============================================================================
// ���ܣ���RTC�����ʼ��ģʽ
// ��������
// ���أ�true,�����������������
// =============================================================================
static bool_t RTC_Init_Mode(void)
{
	u32 retry=0XFFFFF;
	if(RTC->ISR&(1<<6))
		return true;
	RTC->ISR|=1<<7;
	while(retry&&((RTC->ISR&(1<<6))==0x00))
		retry--;
    if(retry==0)
    	return false;
	return true;
}
// =============================================================================
// ���ܣ���RTC�豸�ж�ȡRTCʱ�䣬��λ΢��,ȡ��1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ���������ʱ��ת����1970��1��1��0:0:0�����ڵ�ʱ���
// ���أ�true,�����������������
// =============================================================================
bool_t Rtc_GetTime(s64 *time)
{
    struct tm dtm;
    u32 year,month,date,hour,min,sec;
    u32 DR_bak=0,TR_bak=0;
    u32 tim_us;
    u8 timeout=10;
	while(timeout&&(false==RTC_Wait_Rsf()))
	{
		timeout--;
	}
 	if(0==timeout)	//�ȴ�ͬ��
 		return false;

 	DR_bak=RTC->DR;//�������ա���һ�ζ�������ֹ��λ���
 	TR_bak=RTC->TR;
 	tim_us = (1000000*(0xff - RTC->SSR) )/ (0xff + 1);
	year=BcdToHex((DR_bak>>16)&0XFF)+1970;
	month=BcdToHex((DR_bak>>8)&0X1F);
	date=BcdToHex(DR_bak&0X3F);

	hour=BcdToHex((TR_bak>>16)&0X3F);
	min=BcdToHex((TR_bak>>8)&0X7F);
	sec=BcdToHex(TR_bak&0X7F);

    dtm.tm_year = year;
    dtm.tm_mon  = month;
    dtm.tm_mday = date;
    dtm.tm_hour = hour;
    dtm.tm_min  = min;
    dtm.tm_sec  = sec;

    *time = (s64)(1000000 * Tm_MkTime(&dtm)+tim_us);
    return true;
}

// =============================================================================
// ���ܣ�����RTC�豸RTCʱ�䣬��λ΢�룬��ʱ���1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
bool_t Rtc_SetTime(s64 time)
{
    atom_low_t  atom_bak;

    atom_bak = Int_LowAtomStart();
    UpdateTime = time;
    Int_LowAtomEnd(atom_bak);
    Lock_SempPost(pRtcSemp);
    return true;
}

static bool_t __Rtc_SetTime(s64 time)
{
    struct tm dtm;
    s64 time_s;
    u8 tm_wday;
    time_s = time/1000000;
    Tm_LocalTime_r(&time_s,&dtm);

    //�ر�RTC�Ĵ���д����
	RTC->WPR=0xCA;
	RTC->WPR=0x53;
	if(false==RTC_Init_Mode())
		return false;//����RTC��ʼ��ģʽʧ��

	if(dtm.tm_wday==0)
		tm_wday=7;
	else
		tm_wday=dtm.tm_wday;
	RTC->DR=(((u32)(tm_wday&0X07))<<13)|
			((u32)HexToBcd(dtm.tm_year-1970)<<16)|
			((u32)HexToBcd(dtm.tm_mon)<<8)|
			((u32)HexToBcd(dtm.tm_mday));

	RTC->TR=((u32)HexToBcd(dtm.tm_hour)<<16)|\
			((u32)HexToBcd(dtm.tm_min)<<8)|\
			(u32)(HexToBcd(dtm.tm_sec));

	RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ

	return true;
}

// =============================================================================
// ���ܣ�RTCʱ���������ר����һ����
//     ���ȼ���������Ϊ����RTC�����Է�ռ�������̵߳�CPUʱ��
// ��������
// ���أ���
// =============================================================================
ptu32_t Rtc_UpdateTime(void)
{

    while(1)
    {
        if(Lock_SempPend(pRtcSemp,CN_TIMEOUT_FOREVER))
        {
            __Rtc_SetTime(UpdateTime);
        }
    }
    return 0;
}

// =============================================================================
// ���ܣ�RTCӲ����ʼ�����ȼ��LSE�Ƿ��ܿ������������ʹ��LSIʱ��
// ������void
// ���أ�true,�����������������
// =============================================================================

static bool_t RTC_Configuration(void)
{

	u16 retry=400;
	u32 SSR;

	RCC->APB1ENR|=1<<28;		//ʹ�ܵ�Դ�ӿ�ʱ��
	PWR->CR1|=1<<8;				//���������ʹ��(RTC+SRAM)
	if(LSE_Flag_Reg!=BKP_ReadBackupRegister(BAK_Reg))
	{

			RCC->BDCR=1<<16;			//��λBDCR
			Djy_DelayUs(10);
			RCC->BDCR=0;				//������λ

			RCC->CSR|=1<<0;				//LSI����ʹ��
			while(!(RCC->CSR&0x02));	//�ȴ�LSI����

			RCC->BDCR|=1<<0;			//���Կ���LSE
			while(retry&&((RCC->BDCR&0X02)==0))//�ȴ�LSE׼����
			{
				retry--;
				Djy_DelayUs(5*1000);
			}

			RCC->BDCR&=~(3<<8);			//����8/9λ

			if(retry==0)
			{
				BKP_WriteBackupRegister(BAK_Reg,LSI_Flag_Reg);
				RCC->BDCR|=1<<9;	//LSE����ʧ��,����LSI.
			}
			else
			{
				BKP_WriteBackupRegister(BAK_Reg,LSE_Flag_Reg);
				RCC->BDCR|=1<<8;			//ѡ��LSE,��ΪRTCʱ��
			}
			RCC->BDCR|=1<<15;				//ʹ��RTCʱ��

			RTC->WPR=0xCA;	//�ر�RTC�Ĵ���д����
			RTC->WPR=0x53;
			RTC->CR=0;
			if(false==RTC_Init_Mode())
			{
				RCC->BDCR=1<<16;		//��λBDCR
				Djy_DelayUs(10);
				RCC->BDCR=0;			//������λ
				return false;				//����RTC��ʼ��ģʽ
			}
			RTC->PRER=0XFF;				//RTCͬ����Ƶϵ��(0~7FFF),����������ͬ����Ƶ,�������첽��Ƶ,Frtc=Fclks/((Sprec+1)*(Asprec+1))
			RTC->PRER|=0X7F<<16;		//RTC�첽��Ƶϵ��(1~0X7F)
			RTC->CR&=~(1<<6);			//RTC����Ϊ,24Сʱ��ʽ
			RTC->ISR&=~(1<<7);			//�˳�RTC��ʼ��ģʽ
			RTC->WPR=0xFF;				//ʹ��RTC�Ĵ���д����
			//��һ������װ��ʱ���ֵ�˳���ʼ��ģʽ
			if(BKP_ReadBackupRegister(BAK_Reg)!=LSI_Flag_Reg &&\
			 BKP_ReadBackupRegister(BAK_Reg)!=LSI_Flag_Reg)
			{

					RTC->DR=(((u32)(1))<<13)|
					((u32)HexToBcd(2017-1970)<<16)|
					((u32)HexToBcd(2)<<8)|
					((u32)HexToBcd(20));

					RTC->TR=((u32)HexToBcd(12)<<16)|\
					((u32)HexToBcd(0)<<8)|\
					(u32)(HexToBcd(0));
			}

	}
	else
	{
		retry=10;		//����10��SSR��ֵ��û�仯,��LSE����.
		SSR=RTC->SSR;	//��ȡ��ʼֵ
		while(retry)	//���ssr�Ĵ����Ķ�̬,���ж�LSE�Ƿ�����
		{
			Djy_DelayUs(10*1000);
			if(SSR==RTC->SSR)retry--;	//�Ա�
			else break;
		}
		if(retry==0)	//LSE����,������õȴ��´ν�����������
		{
			BKP_WriteBackupRegister(BAK_Reg,0XFFFF);	//��Ǵ����ֵ
			RCC->BDCR=1<<16;			//��λBDCR
			Djy_DelayUs(10);
			RCC->BDCR=0;				//������λ
		}
	}
	return true;
}
// =============================================================================
// ���ܣ�RTCʱ��ע�ἰ��ʼ��
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
ptu32_t ModuleInstall_CpuRtc(ptu32_t para)
{
    s64 rtc_time;
    struct timeval tv;
    u16 evtt;

	RTC_Configuration();    // ����RTC
    //��ʼ��Ӳ��

    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP");

    if(NULL == pRtcSemp)
        return false;

    evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Rtc_UpdateTime,NULL,800,
                                "RTC Update Event");

    if(evtt == CN_EVTT_ID_INVALID)
    {
        free(pRtcSemp);
        return false;
    }
    Djy_EventPop(evtt,NULL,0,NULL,0,0);
    Rtc_GetTime(&rtc_time);

    tv.tv_sec  = rtc_time/1000000;//us ---> s
    tv.tv_usec = rtc_time%1000000;

    settimeofday(&tv,NULL);

    //ע��RTCʱ��
    if(!Rtc_RegisterDev(NULL,Rtc_SetTime))
        return false;
    return true;
}





