//-----------------------------------------------------------------------------
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 模块版本: V1.00
// 创建人员:yt
// 创建时间:2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#include <silan_syscfg.h>
#include <silan_rtc.h>


#if 0
typedef struct _RTC_
{
	vu32 TIMER;
	vu32 TMCON;
	vu32 CLKOUT_CON;
	vu32 WEEK_ALARM;
	vu32 DAY_ALARM;
	vu32 HOUR_ALARM;
	vu32 MIN_ALARM;
	vu32 YEARL;
	vu32 MON;
	vu32 WEEK;
	vu32 DAY;
	vu32 HOUR;
	vu32 MIN;
	vu32 SEC;
	vu32 YEARH;
	vu32 RTC_CS0;
	vu32 RTC_CS1;
	vu32 SCOUNTERLOADH;
	vu32 SCOUNTERLOADL;
}tagRTCReg;

#endif
#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))  //16进制转换成BCD
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F)) //BCD转换成16进制

// =============================================================================
// 功能：从RTC设备中读取RTC时间，单位微秒,取从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值，需把日历时间转换成1970年1月1日0:0:0到现在的时间差
// 返回：true,正常操作，否则出错
// =============================================================================
bool_t Rtc_GetTime(s64 *time)
{
    struct tm dtm;
    u32 yearh,yearl,month,date,hour,min,sec;

    sec = silan_rtc_reg_get(0x134);
    min = silan_rtc_reg_get(0x130);
    hour = silan_rtc_reg_get(0x12C);
    date = silan_rtc_reg_get(0x128);
    month = silan_rtc_reg_get(0x120);
    yearl = silan_rtc_reg_get(0x11C);
    yearh = silan_rtc_reg_get(0x138);

    dtm.tm_year  = BcdToHex(yearl & 0xFF);
    dtm.tm_year += BcdToHex(yearh & 0xFF) * 100;
    dtm.tm_mon  = BcdToHex(month & 0x1F);
    dtm.tm_mday = BcdToHex(date & 0x3F);
    dtm.tm_hour = BcdToHex(hour & 0x3F);
    dtm.tm_min  = BcdToHex(min & 0x7F);
    dtm.tm_sec  = BcdToHex(sec & 0x7F);

    *time = (s64)(1000000 * Tm_MkTime(&dtm));
    return true;
}

// =============================================================================
// 功能：设置RTC设备RTC时间，单位微秒，该时间从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t Rtc_SetTime(s64 time)
{
    struct tm dtm;
    u8 rtccs0,year;
    time = time/1000000;
    Tm_LocalTime_r(&time,&dtm);

	rtccs0 = silan_rtc_reg_get(0x13c);
	silan_rtc_reg_set(0x13C,rtccs0 | (1<<5));//暂时关闭时钟

	silan_rtc_reg_set(0x134,(u8)(HexToBcd(dtm.tm_sec)));
	silan_rtc_reg_set(0x130,(u8)(HexToBcd(dtm.tm_min)));
	silan_rtc_reg_set(0x12C,(u8)(HexToBcd(dtm.tm_hour)));

	silan_rtc_reg_set(0x128,(u8)(HexToBcd(dtm.tm_mday)));
	silan_rtc_reg_set(0x120,(u8)(HexToBcd(dtm.tm_mon)));

	year = (u8)(dtm.tm_year % 100);
	silan_rtc_reg_set(0x11C,(u8)(HexToBcd(year)));
	year = (dtm.tm_year/100);
	silan_rtc_reg_set(0x138,(u8)(HexToBcd(year)));

	silan_rtc_reg_set(0x124,(u8)(HexToBcd(dtm.tm_wday)));

	silan_rtc_reg_set(0x13C,rtccs0);//暂时关闭时钟

	return true;
}

// =============================================================================
// 功能：RTC硬件初始化
//     首先检查LSE是否能可用,如果不行则使用LSI时钟
// 参数：void
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t RTC_Configuration(void)
{
	silan_rtc_init();
	return true;
}
// =============================================================================
// 功能：RTC时间注册及初始化
// 参数：time, 时间值
// 返回：true,正常操作，否则出错
// =============================================================================
ptu32_t ModuleInstall_CpuRtc(ptu32_t para)
{
    s64 rtc_time;
    struct timeval tv;

	RTC_Configuration();    //配置RTC

    Rtc_GetTime(&rtc_time);  //从RTC设备中读取RTC时间，单位是us,
                             //读取的值放到&rtc_time中

    tv.tv_sec  = rtc_time/1000000;   //把读取的时间中s的部分留下
    tv.tv_usec = rtc_time%1000000;   //把读取的时间中us的部分留下(余数)

    settimeofday(&tv,NULL);

    //注册RTC时间
    if(!Rtc_RegisterDev(NULL,Rtc_SetTime))
        return false;
    return true;
}
