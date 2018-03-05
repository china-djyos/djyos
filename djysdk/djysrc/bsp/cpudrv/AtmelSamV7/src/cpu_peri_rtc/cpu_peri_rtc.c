//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_busram.c
// ģ������: CPUƬ��backup 1K SRAM��д����
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/11.2015
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os.h"
#include "cpu_peri.h"
#include "time.h"

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))

static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //����µ�ʱ��

// =============================================================================
// ���ܣ���RTC�豸�ж�ȡRTCʱ�䣬��λ΢��,ȡ��1970��1��1��0:0:0�����ڵ�ʱ���
// ������time, ʱ��ֵ���������ʱ��ת����1970��1��1��0:0:0�����ڵ�ʱ���
// ���أ�true,�����������������
// =============================================================================
bool_t Rtc_GetTime(s64 *time)
{
    struct tm dtm;
    u32 rtc_timr,rtc_calr;
    u32 year,month,date,hour,min,sec;

    rtc_timr = RTC->RTC_TIMR;
    rtc_calr = RTC->RTC_CALR;

    year = (rtc_calr & RTC_CALR_YEAR_Msk) >> RTC_CALR_YEAR_Pos;
    month = (rtc_calr & RTC_CALR_MONTH_Msk) >> RTC_CALR_MONTH_Pos;
    date = (rtc_calr & RTC_CALR_DATE_Msk) >> RTC_CALR_DATE_Pos;

    hour = (rtc_timr & RTC_TIMR_HOUR_Msk) >> RTC_TIMR_HOUR_Pos;
    min = (rtc_timr & RTC_TIMR_MIN_Msk) >> RTC_TIMR_MIN_Pos;
    sec = (rtc_timr & RTC_TIMR_SEC_Msk) >> RTC_TIMR_SEC_Pos;

    dtm.tm_year = BcdToHex(year) + 2000;
    dtm.tm_mon  = BcdToHex(month);
    dtm.tm_mday = BcdToHex(date);
    dtm.tm_hour = BcdToHex(hour);
    dtm.tm_min  = BcdToHex(min);
    dtm.tm_sec  = BcdToHex(sec);

    *time = 1000000 * Tm_MkTime(&dtm);
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
    bool_t result = false;
    struct tm dtm;
    s64 time_s;
    u32 dtime,timout = 1500*mS;

    time_s = time/1000000;
    Tm_LocalTime_r(&time_s,&dtm);

    if((dtm.tm_year > 2000) && (dtm.tm_year < 2099))
    {
        RTC->RTC_CR |= RTC_CR_UPDTIM | RTC_CR_UPDCAL;

        while((RTC->RTC_SR & RTC_SR_ACKUPD) != RTC_SR_ACKUPD)
        {
            Djy_EventDelay(100*mS);
            timout -= 100*mS;
            if(timout <= 0)
            {
                RTC->RTC_CR &= ~(RTC_CR_UPDCAL | RTC_CR_UPDTIM) ;
                RTC->RTC_SCCR |= RTC_SCCR_SECCLR; /* clear SECENV in SCCR */
                return false;
            }
        }
//      while ((RTC->RTC_SR & RTC_SR_ACKUPD) != RTC_SR_ACKUPD) ;
        RTC->RTC_SCCR = RTC_SCCR_ACKCLR ;

        dtime = (HexToBcd(dtm.tm_hour) << RTC_TIMR_HOUR_Pos) |
                (HexToBcd(dtm.tm_min) << RTC_TIMR_MIN_Pos) |
                (HexToBcd(dtm.tm_sec) << RTC_TIMR_SEC_Pos);

        RTC->RTC_TIMR = dtime;
        if(dtm.tm_wday == 0)
            dtm.tm_wday = 7;
        dtime = (HexToBcd(dtm.tm_year - 2000) << RTC_CALR_YEAR_Pos) |
                (HexToBcd(dtm.tm_mon) << RTC_CALR_MONTH_Pos) |
                (HexToBcd(dtm.tm_mday) << RTC_CALR_DATE_Pos) |
                (HexToBcd(dtm.tm_wday) << RTC_CALR_DAY_Pos)|
                (HexToBcd(20) << RTC_CALR_CENT_Pos);

        RTC->RTC_CALR = dtime;

        RTC->RTC_CR &= ~(RTC_CR_UPDCAL | RTC_CR_UPDTIM) ;
        RTC->RTC_SCCR |= RTC_SCCR_SECCLR; /* clear SECENV in SCCR */
        result = !((RTC->RTC_VER & RTC_VER_NVTIM)
                | (RTC->RTC_VER & RTC_VER_NVCAL)) ;
    }

    return result;
}

// =============================================================================
// ���ܣ�RTCʱ�������������AtmelоƬ��RTC����ʱ������ܴﵽ1����ˣ����ר����һ����
//      ���ȼ���������Ϊ����RTC�����Է�ռ�������̵߳�CPUʱ��
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
// ���ܣ�RTCʱ��ע�ἰ��ʼ��
// ������time, ʱ��ֵ
// ���أ�true,�����������������
// =============================================================================
ptu32_t ModuleInstall_CpuRtc(ptu32_t para)
{
    s64 rtc_time;
    struct timeval tv;
    u16 evtt;

    //��ʼ��Ӳ��
    PMC_EnablePeripheral(ID_RTC);
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

    tv.tv_sec = rtc_time/1000000;//us ---> s
    tv.tv_usec = 0;

    settimeofday(&tv,NULL);

    //ע��RTCʱ��
    if(!Rtc_RegisterDev(NULL,Rtc_SetTime))
        return false;
    return true;
}











