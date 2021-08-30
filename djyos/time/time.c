//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: Ϊϵͳ�ṩʱ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-5-1
//   ����:  ������
//   �°汾��: V1.1.0
//   �޸�˵��: ������޸�
// 1. ����: 2011-10-28
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/time.h>
#include "component_config_time.h"

extern s64 __Rtc_Time(s64 *rtctime);
extern s64 __Rtc_TimeUs(s64 *rtctime);
extern bool_t __Rtc_SetTime(s64 rtctime);

u32 g_u32DaysToMonth[13] = {0,31,59,90,120,151,181,212,243,273,304,334,365};
s32 g_u32MonthDays[]={31,28,31,30,31,30,31,31,30,31,30,31,};
s32 sg_s32MyTimezone = CFG_LOCAL_TIMEZONE;

//char g_cTmWdays[][8] = {"������", "����һ", "���ڶ�", "������", "������", "������", "������"};
char g_cTmWdays[][8] = {"������", "����һ", "���ڶ�", "������", "������", "������", "������"};


//----�ѷֽ�ʱ��ת������ʱ��----------------------------------------------
//����: �ѷֽ�ʱ�䣨������ʱ���룩ת��������ʱ��(1970������������),
//����: dt��������ʱ������ʽ��ʱ��
//����: �ֽ�ʱ��
//ע����������Դ��Linux�������ѡ�719499�����Եõ��������㷨����ϸ˵��
//----------------------------------------------------------------------------
s64 Time_MkTime(struct tm *dt)
{
    u32 year = dt->tm_year + 1900;  //ע��tm_year�ĺ���
    u32 mon  = dt->tm_mon + 1;

    //��ʱ����������£������ж�������·ֽ�
    //ֱ����year/4 - year/100 + year/400
    if(0>= (s32)(mon -= 2))
    {
        mon += 12;
        year--;
    }
    //��ǰ���ں�19691101�ֱ���빫Ԫ1��1��1�յ�������
    //Ȼ�����
    return ((((s64)(year/4 - year/100 + year/400 + 367*mon/12 + dt->tm_mday)
        + year*365 -719499) * 24 + dt->tm_hour) *60 + dt->tm_min)*60 + dt->tm_sec;
}

//----�ѷֽ�ʱ��ת������ʱ��(us)-------------------------------------------
//����: �ѷֽ�ʱ�䣨������ʱ���룩ת��������ʱ��(1970��������΢����),
//����: dt��������ʱ������ʽ��ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
s64 Time_MkTimeUs(struct tm *dt)
{
    s64 us;
    us = Time_MkTime(dt);
    us *= 1000000;
    us += dt->tm_us;
    return us;
}

//----�ѱ�������ʱ��ת�ɸ������ηֽ�ʱ��-------------------------------------------
//����: �ѱ�������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970�����ı�������,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_GmTime_r(const s64 *time,struct tm *result)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = *time - ((s64)sg_s32MyTimezone*3600);
    }
    return Time_LocalTime_r(&temp_time,result);
}

//----�ѱ�������ʱ��ת�ɸ������ηֽ�ʱ��-------------------------------------------
//����: �ѱ�������ʱ��(1970������������)ת����������ʱ����,����һ����̬����������
//      �������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970�����ı�������,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_GmTime(const s64 *time)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = *time - ((s64)sg_s32MyTimezone*3600);
    }
    return Time_LocalTime_r(&temp_time,&datetime);
}

//----�ѱ�������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: �ѱ�������ʱ��(1970��������΢����)ת����������ʱ����+΢��,
//����: time,1970�����ı���΢����,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_GmTimeUs_r(const s64 *time,struct tm *result)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (((s64)sg_s32MyTimezone*3600)*1000000);
    }
    else
    {
        temp_time = *time - ((s64)sg_s32MyTimezone*3600)*1000000;
    }
    return Time_LocalTimeUs_r(&temp_time,result);
}

//----�ѱ�������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: �ѱ�������ʱ��(1970��������΢����)ת����������ʱ����+΢��,����һ����̬����
//      �����ؽ������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970�����ı���΢����,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_GmTimeUs(const s64 *time)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (((s64)sg_s32MyTimezone*3600)*1000000);
    }
    else
    {
        temp_time = *time - ((s64)sg_s32MyTimezone*3600)*1000000;
    }
    return Time_LocalTimeUs_r(&temp_time,&datetime);
}
//----�Ѹ�����������ʱ��ת��ָ��ʱ���ֽ�ʱ��-------------------------------------------
//����: �Ѹ�����������ʱ��(1970������������)ת����ָ��ʱ��������ʱ����,
//����: time,1970�����ĸ�����������,����ָ����ֱ��ʹ��ϵͳ����ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//      zone��Ŀ��ʱ���������ڣ�0~23��֮�ڣ���ȡ����ʱ��
//����: Ŀ��ʱ���ķֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_ZoneTime_r(const s64 *time,struct tm *result,s32 zone)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = *time - ((s64)sg_s32MyTimezone*3600);
    }
    if( (zone < 0) ||(zone > 23) )    //Ҫ��24��ʱ����
    {
        temp_time = temp_time + ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }

    return Time_LocalTime_r(&temp_time,result);
}

//----�Ѹ�����������ʱ��ת�ɸ������ηֽ�ʱ��-------------------------------------------
//����: �Ѹ�����������ʱ��(1970������������)ת����������ʱ����,����һ����̬����������
//      �������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970�����ĸ�����������,����ָ����ֱ��ʹ������ʱ��
//      zone��Ŀ��ʱ���������ڣ�0~23��֮�ڣ���ȡ����ʱ��
//���أ�ָ��ʱ���ķֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_ZoneTime(const s64 *time,s32 zone)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_Time(NULL);
        temp_time -= ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = *time - (s64)sg_s32MyTimezone*3600;
    }
    if( (zone < 0) ||(zone > 23) )    //Ҫ��24��ʱ����
    {
        temp_time = temp_time + (s64)sg_s32MyTimezone*3600;
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }
    return Time_LocalTime_r(&temp_time,&datetime);
}

//----�Ѹ�����������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: �Ѹ�����������ʱ��(1970��������΢����)ת����������ʱ����+΢��,
//����: time,1970�����ĸ�������΢����,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//      zone��Ŀ��ʱ���������ڣ�0~23��֮�ڣ���ȡ����ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_ZoneTimeUs_r(const s64 *time,struct tm *result,s32 zone)
{
    s64 temp_time;
    if(result == NULL)
        return NULL;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (s64)sg_s32MyTimezone*3600;
    }
    else
    {
        temp_time = *time - (s64)sg_s32MyTimezone*3600;
    }
    if( (zone < 0) ||(zone > 23) )    //Ҫ��24��ʱ����
    {
        temp_time = temp_time + ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }
    temp_time *= 1000000;
    return Time_LocalTimeUs_r(&temp_time,result);
}

//----�Ѹ�����������ʱ��ת�ɸ������ηֽ�ʱ��(us)---------------------------------------
//����: �Ѹ�����������ʱ��(1970��������΢����)ת����������ʱ����+΢��,����һ����̬����
//      �����ؽ������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970�����ĸ�������΢����,����ָ����ֱ��ʹ������ʱ��
//      zone��Ŀ��ʱ���������ڣ�0~23��֮�ڣ���ȡ����ʱ��
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
struct tm *Time_ZoneTimeUs(const s64 *time,s32 zone)
{
    static struct tm datetime;
    s64 temp_time;
    if(time == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_time -= (s64)sg_s32MyTimezone*3600;
    }
    else
    {
        temp_time = *time - (s64)sg_s32MyTimezone*3600;
    }
    if( (zone < 0) ||(zone > 23) )    //Ҫ��24��ʱ����
    {
        temp_time = temp_time + ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }
    temp_time *= 1000000;
    return Time_LocalTimeUs_r(&temp_time,&datetime);
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Time_LocalTime_r(const s64 *time,struct tm *result)
{
    u32 second, minute, hour;
    u32 day, month, year;

    u32 days;
    u32 years;
    u32 leap_years;
    u32 dayth;    // ����ĵ�x�죨1��1��0ʱ0��0�뼴Ϊ��1���ˣ�
    u32 day_of_week;
    u32 tmp_month_days;
    u32 temp,temp1;
    s64 temp_time;

    if(result == NULL)
        return NULL;
    if(time != NULL)
        temp_time = *time;
    else
        temp_time = __Rtc_Time(NULL);
    if(temp_time > 47650000000) //�㷨�ڹ�Ԫ3480�������
        return NULL;

    second = temp_time % 60;
    minute = temp_time/60 %60;
    hour = temp_time/3600 %24;
    days = (u32)(temp_time/86400);

    years = days / 365;
    leap_years = (years + 1) >> 2; // ��������ȵ���
    if(((years+1970 - 1)/100 != ((years+1970)/100)) && ((years+1970 )%400)!=0 )
        leap_years++;
    leap_years -= (years+1970)/100 - 1970/100;
    leap_years += (years+1970)/400 - 1970/400;
    temp = days%365;
    temp1 = temp;
    if(temp < leap_years)
    {
        years--;
        temp += 365;
    }
    dayth = temp - leap_years;    // ����ȵ�x�죨��1��1��0ʱΪ��1�죩

    year = 1970 + years;    //δ�����������1400���������

    g_u32MonthDays[1] = 28;
    if (year%4 == 0)        // ������������꣬��ô����2�·����һ��
    {
        if ((year%100 == 0) && (year%400 != 0))
        {
            // ���겻���İ�����
        }
        else
        {
            g_u32MonthDays[1] = 29;
            if(temp1 < leap_years)// ������������������С�������������һ��
                dayth +=1;
        }
    }

    month = 0;                      // 0~11
    day = dayth;
    tmp_month_days = g_u32MonthDays[month];
    while (dayth >= tmp_month_days)
    {
        dayth -= tmp_month_days;
        month++;
        tmp_month_days = g_u32MonthDays[month];
    }

    day_of_week = (days + 4) % 7;

    result->tm_yday = day;           // days since January 1 -[0,365]
    result->tm_us = 0;               // us����
    result->tm_sec = second;        // seconds after the minute - [0,59]
    result->tm_min = minute;        // minutes after the hour - [0,59]
    result->tm_hour = hour;          // hours since midnight - [0,23]
    result->tm_wday = day_of_week;   // days since Sunday - [0,6]
    result->tm_mday = dayth+1;           // day of the month - [1,31]
    result->tm_mon = month;         // months since January - [0,11]
    result->tm_year = year - 1900;   // years 1970-����ʾΪ��1900��Ĳ�ֵ

    return result;
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,����һ����̬����������
//      �������˶�ε��õĻ������ֻ��ӳ���һ�ε��õ�ֵ
//����: time,1970����������,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Time_LocalTime(const s64 *time)
{
    static struct tm datetime;

    return Time_LocalTime_r(time,&datetime);
}

//----������ʱ��(��)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//      result���������ؽ����ָ�룬����ǿ�
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Time_LocalTimeUs_r(const s64 *time_us,struct tm *result)
{
    s64 temp_time;
    s64 temp_us;
    if(result == NULL)
        return NULL;
    if(time_us == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_us = temp_time%1000000;
        temp_time /=1000000;
    }
    else
    {
        temp_us = *time_us%1000000;
        temp_time = *time_us/1000000;
    }
    Time_LocalTime_r(&temp_time,result);
    result->tm_us = temp_us;
    return result;
}

//----������ʱ��(us)ת�ɱ��طֽ�ʱ��-------------------------------------------
//����: ������ʱ��(1970������������)ת����������ʱ����,
//����: time,1970������΢����,����ָ����ֱ��ʹ������ʱ��
//����: �ֽ�ʱ��
//-----------------------------------------------------------------------------
struct tm *Time_LocalTimeUs(s64 *time_us)
{
    static struct tm datetime;
    s64 temp_us;
    s64 temp_time;
    if(time_us == NULL)
    {
        temp_time = __Rtc_TimeUs(NULL);
        temp_us = temp_time%1000000;
        temp_time /=1000000;
    }
    else
    {
        temp_us = *time_us%1000000;
        temp_time = *time_us/1000000;
    }

    Time_LocalTime_r(&temp_time,&datetime);
    datetime.tm_us = temp_us;
    return &datetime;
}

//----��������ʱ��-------------------------------------------------------------
//����: �ø�ʽΪ"2011/10/28,22:37:50:000"���ַ�����ʾ��ʱ����������ʱ�䣬�ɵ���
//      ����������ϵͳ����ʱ�䣬���һ����uS�������Բ���
//����: buf������ʱ��ֵ
//����: 1=�ɹ�������ֵ:��Ӧ�Ĵ������
//-----------------------------------------------------------------------------
//2011/10/28,22:37:50:
s32 Time_SetDateTimeStr(char *buf)
{
    s64 nowtime;
    struct tm ptDateTime;
    s32 params;

    memset(&ptDateTime,0,sizeof(ptDateTime));
    params = sscanf(buf,"%d/%d/%d,%d:%d:%d:%d",\
            &ptDateTime.tm_year,&ptDateTime.tm_mon,&ptDateTime.tm_mday,\
            &ptDateTime.tm_hour,&ptDateTime.tm_min,&ptDateTime.tm_sec,\
            &ptDateTime.tm_us);

    //parameters enough
    if(params < 6)
    {
        return EN_CLOCK_FMT_ERROR;
    }
    //year check
    if ((1970>ptDateTime.tm_year) || (ptDateTime.tm_year>2100))
    {
        return EN_CLOCK_YEAR_ERROR;
    }

    //moth check
    if ((1>ptDateTime.tm_mon) || (ptDateTime.tm_mon>12))
    {
        return EN_CLOCK_MON_ERROR;
    }

    //day check
    if ((1>ptDateTime.tm_mday) || (ptDateTime.tm_mday>31))
    {
        return EN_CLOCK_DAY_ERROR;
    }
    if (ptDateTime.tm_mday > g_u32MonthDays[ptDateTime.tm_mon-1])
    {
        return EN_CLOCK_DAY_ERROR;
    }
    if (ptDateTime.tm_mon == 2)
    {
        if (ptDateTime.tm_mday > 29)
        {
            return EN_CLOCK_DAY_ERROR;
        }

        if (ptDateTime.tm_year%4 == 0)
        {
            if (ptDateTime.tm_year%100 == 0)
            {
                if (ptDateTime.tm_year%400)
                {
                    if (ptDateTime.tm_mday > 28)
                        return EN_CLOCK_DAY_ERROR;
                }
            }
        }
        else
        {
            if (ptDateTime.tm_mday > 28)
                return EN_CLOCK_DAY_ERROR;
        }
    }

    //hour check
    if (ptDateTime.tm_hour>23)
    {
        return EN_CLOCK_HOUR_ERROR;
    }

    //min check
    if (ptDateTime.tm_min>59)
    {
        return EN_CLOCK_MIN_ERROR;
    }

    //sec check
    if (ptDateTime.tm_sec >59)
    {
        return EN_CLOCK_SEC_ERROR;
    }

    ptDateTime.tm_year -= 1900;
    nowtime = Time_MkTimeUs(&ptDateTime);
    //use the rtc module to set
    __Rtc_SetTime(nowtime);
    return 1;
}
//----��������ʱ��-------------------------------------------------------------
//����: �÷ֽ�ʱ���������ʱ�䣬��rtcӲ����ϵͳ�У����Ӳ��ֱ������ֽ�ʱ��ģ�
//      Ӳ���������ñ�������
//      ��Ϊ����ʱ��ֻ��ȷ���룬���tm�е�us��Ա����Ӳ����������ά��������������
//      ȡ�����Ӳ����ά��us��Ա(����tm_connect_rtcʱ����get_rtc_hard_us=NULL),
//      rtc driver��ʹ���жϷ�ʽ����ȷ��������ʱ��������get_rtc_hard_us��ϵͳ��
//      �Դ�ʱ��Ϊ������us����
//����: tm,��ǰʱ�䣬��������ʱ����ֽ�
//����: ��
//-----------------------------------------------------------------------------
void Time_SetDateTime(struct tm *tm)
{
    s64 nowtime;
    nowtime = Time_MkTimeUs(tm);
    //use the rtc module to set
    __Rtc_SetTime(nowtime);
}

//----�ѷֽ�ʱ��ת���ַ���-----------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50"
//����: tm,�ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������21�ֽڡ�
//����: �ֽ�ʱ��
//----------------------------------------------------------------------------
void Time_AscTime(struct tm *tm, char buf[])
{
    itoa(tm->tm_year + 1900, &buf[0], 10);
    buf[4] = '/';
    itoa(tm->tm_mon, &buf[5], 10);
    if(buf[6] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[6] = buf[5];
        buf[5] = '0';
    }
    buf[7] = '/';
    itoa(tm->tm_mday, &buf[8], 10);
    if(buf[9] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[9] = buf[8];
        buf[8] = '0';
    }
    buf[10] = ',';
    itoa(tm->tm_hour, &buf[11], 10);
    if(buf[12] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[12] = buf[11];
        buf[11] = '0';
    }
    buf[13] = ':';
    itoa(tm->tm_min, &buf[14], 10);
    if(buf[15] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[15] = buf[14];
        buf[14] = '0';
    }
    buf[16] = ':';
    itoa(tm->tm_sec, &buf[17], 10);
    if(buf[18] == '\0')  //ɾ���ַ�����β����0���
    {
        buf[18] = buf[17];
        buf[17] = '0';
    }
    buf[19] = '\0';

    return ;
}

//----�ѷֽ�ʱ��ת���ַ���(ms)-------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50:000"
//����: tm���ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������24�ֽڡ�
//����: �ַ���
//----------------------------------------------------------------------------
void Time_AscTimeMs(struct tm *tm, char buf[])
{
    Time_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us/1000, &buf[20], 10);
    buf[23] = '\0';
    return ;
}

//----�ѷֽ�ʱ��ת���ַ���(us)-------------------------------------------------
//����: ��һ���ֽ�ʱ��ת���ַ�������ʽ"2011/10/28,22:37:50:000000"
//����: tm,�ֽ�ʱ��
//      buf�����ڷ��ؽ���������ַ������27�ֽڡ�
//����: �ַ���
//----------------------------------------------------------------------------
void Time_AscTimeUs(struct tm *tm, char buf[])
{
    Time_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us, &buf[20], 10);
    buf[26] = '\0';
    return ;
}

//----ȡ����ʱ��(��)-----------------------------------------------------------
//����: ȡ��1970��1��1��0:0:0�����ڵ�ʱ��������
//����: ret,�ǿ��򷵻�����ʱ�䡣
//����: ����ʱ��
//-----------------------------------------------------------------------------
s64 Time_Time(s64 *ret)
{
    return __Rtc_Time(ret);
}
//----ȡ����ʱ��(΢��)---------------------------------------------------------
//����: ȡ��1970��1��1��0:0:0�����ڵ�ʱ����������ȷ��ֻ�ܵ�tick���
//����: ret,�ǿ��򷵻�����ʱ�䡣
//����: ����ʱ��
//-----------------------------------------------------------------------------
s64 Time_TimeUs(s64 *ret)
{
    return __Rtc_TimeUs(ret);
}
//modified by zqf--make the libc time interface
#define CN_WEEKDAY_MAX  7
static const char *pWeekDay[CN_WEEKDAY_MAX]={
        "Sun",
        "Mon",
        "Tue",
        "Wed",
        "Thu",
        "Fri",
        "Sat"
};

#define CN_MONTH_MAX 13
static const char *pMonth[CN_MONTH_MAX] ={
        NULL,
        "Jan",
        "Feb",
        "Mar",
        "Apr",
        "May",
        "Jun",
        "Jul",
        "Aug",
        "Sep",
        "Oct",
        "Nov",
        "Dec"
};


struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    return Time_LocalTime_r(timep,result);
}
struct tm *localtime(const time_t *timep)
{
    static struct  tm result;

    return localtime_r(timep,&result);
}

//     moth is is 0 to 11
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
    struct tm *myresult;
    myresult = Time_GmTime_r(timep,result);
    if(NULL != myresult)
    {
//      myresult->tm_year -= 1900;
        myresult->tm_mon -= 1;
    }
    return myresult;
}
struct tm *gmtime(const time_t *timep)
{
    static struct  tm result;

    return gmtime_r(timep,&result);
}

time_t mktime(struct tm *tm)
{
    return Time_MkTime(tm);
}
//put the break-down time to the char string
char *asctime_r(const struct tm *tm,char *buf)
{
    char *result = NULL;

    if((NULL != tm)&&(NULL != buf)&&(tm->tm_wday <CN_WEEKDAY_MAX)&&\
        (tm->tm_mon<CN_MONTH_MAX))
    {
        //do the transfer
        sprintf(buf,"%s %s %d %d:%02d:%02d %d",pWeekDay[tm->tm_wday],pMonth[tm->tm_mon],\
                tm->tm_mday,tm->tm_hour,tm->tm_min,tm->tm_sec,tm->tm_year);
        result = buf;
    }
    return result;
}

char *asctime(const struct tm *tm)
{
    static char  buf[32];

    return asctime_r(tm,buf);
}


char *ctime_r(const time_t *timep, char *buf)
{
    struct tm var,*tmp;

    tmp = localtime_r(timep,&var);

    return asctime_r(tmp,buf);

}

char *ctime(const time_t *timep)
{

    static char  buf[32];

    return ctime_r(timep,buf);
}


time_t time(time_t *t)
{
    return Time_Time(t);
}

//TODO������ timezone ��ش���
s32 gettimeofday(struct timeval *tv, struct timezone *tz)
{
    s32 result = -1;

    if(NULL != tv)
    {
        tv->tv_usec = Time_TimeUs(NULL);
        tv->tv_sec = (time_t)(tv->tv_usec / 1000000);
        result = 0;
    }

    return result;
}

//TODO������ timezone ��ش���
s32 settimeofday(const struct timeval *tv, const struct timezone *tz)
{
    s32 result = -1;
    time_t timetmp;
    struct tm  time_tm,*time_p;

    if(NULL != tv)
    {
        timetmp = tv->tv_sec;
        time_p = localtime_r(&timetmp,&time_tm);
        if(NULL != time_p)
        {
            Time_SetDateTime(time_p);
            result = 0;
        }

    }

    return result;
}

s32 gettimezone(s32 *result)
{
    if(NULL != result)
    {
        *result = sg_s32MyTimezone;
    }
    return 0;
}
s32 settimezone(s32 timezone)
{
    sg_s32MyTimezone = timezone;
    return 0;
}


