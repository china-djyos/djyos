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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:时钟模块
//作者:  王建忠
//版本：V1.0.0
//文件描述: 为系统提供时钟日历
//其他说明:
//修订历史:
// 1. 日期: 2012-5-1
//   作者:  罗侍田
//   新版本号: V1.1.0
//   修改说明: 大幅度修改
// 1. 日期: 2011-10-28
//   作者:  王建忠
//   新版本号: V1.0.0
//   修改说明: 原始版本
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

//char g_cTmWdays[][8] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};
char g_cTmWdays[][8] = {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};


//----把分解时间转成日历时间----------------------------------------------
//功能: 把分解时间（年月日时分秒）转换成日历时间(1970年以来的秒数),
//参数: dt，年月日时分秒形式的时间
//返回: 分解时间
//注：本函数来源于Linux，网上搜“719499”可以得到本函数算法的详细说明
//----------------------------------------------------------------------------
s64 Time_MkTime(struct tm *dt)
{
    u32 year = dt->tm_year + 1900;  //注意tm_year的含义
    u32 mon  = dt->tm_mon + 1;

    //将时间后推两个月，则不用判断闰年二月分界
    //直接用year/4 - year/100 + year/400
    if(0>= (s32)(mon -= 2))
    {
        mon += 12;
        year--;
    }
    //当前日期和19691101分别距离公元1年1月1日的秒数，
    //然后相减
    return ((((s64)(year/4 - year/100 + year/400 + 367*mon/12 + dt->tm_mday)
        + year*365 -719499) * 24 + dt->tm_hour) *60 + dt->tm_min)*60 + dt->tm_sec;
}

//----把分解时间转成日历时间(us)-------------------------------------------
//功能: 把分解时间（年月日时分秒）转换成日历时间(1970年以来的微秒数),
//参数: dt，年月日时分秒形式的时间
//返回: 分解时间
//----------------------------------------------------------------------------
s64 Time_MkTimeUs(struct tm *dt)
{
    s64 us;
    us = Time_MkTime(dt);
    us *= 1000000;
    us += dt->tm_us;
    return us;
}

//----把本地日历时间转成格林威治分解时间-------------------------------------------
//功能: 把本地日历时间(1970年以来的秒数)转换成年月日时分秒,
//参数: time,1970年来的本地秒数,给空指针则直接使用日历时间
//      result，用来返回结果的指针，必须非空
//返回: 分解时间
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

//----把本地日历时间转成格林威治分解时间-------------------------------------------
//功能: 把本地日历时间(1970年以来的秒数)转换成年月日时分秒,因共用一个静态缓冲区返回
//      结果，因此多次调用的话，结果只反映最后一次调用的值
//参数: time,1970年来的本地秒数,给空指针则直接使用日历时间
//返回: 分解时间
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

//----把本地日历时间转成格林威治分解时间(us)---------------------------------------
//功能: 把本地日历时间(1970年以来的微秒数)转换成年月日时分秒+微秒,
//参数: time,1970年来的本地微秒数,给空指针则直接使用日历时间
//      result，用来返回结果的指针，必须非空
//返回: 分解时间
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

//----把本地日历时间转成格林威治分解时间(us)---------------------------------------
//功能: 把本地日历时间(1970年以来的微秒数)转换成年月日时分秒+微秒,因共用一个静态缓冲
//      区返回结果，因此多次调用的话，结果只反映最后一次调用的值
//参数: time,1970年来的本地微秒数,给空指针则直接使用日历时间
//返回: 分解时间
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
//----把格林威治日历时间转成指定时区分解时间-------------------------------------------
//功能: 把格林威治日历时间(1970年以来的秒数)转换成指定时区年月日时分秒,
//参数: time,1970年来的格林威治秒数,给空指针则直接使用系统日历时间
//      result，用来返回结果的指针，必须非空
//      zone，目标时区，若不在（0~23）之内，则取本地时区
//返回: 目标时区的分解时间
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
    if( (zone < 0) ||(zone > 23) )    //要在24个时区内
    {
        temp_time = temp_time + ((s64)sg_s32MyTimezone*3600);
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }

    return Time_LocalTime_r(&temp_time,result);
}

//----把格林威治日历时间转成格林威治分解时间-------------------------------------------
//功能: 把格林威治日历时间(1970年以来的秒数)转换成年月日时分秒,因共用一个静态缓冲区返回
//      结果，因此多次调用的话，结果只反映最后一次调用的值
//参数: time,1970年来的格林威治秒数,给空指针则直接使用日历时间
//      zone，目标时区，若不在（0~23）之内，则取本地时区
//返回：指定时区的分解时间
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
    if( (zone < 0) ||(zone > 23) )    //要在24个时区内
    {
        temp_time = temp_time + (s64)sg_s32MyTimezone*3600;
    }
    else
    {
        temp_time = temp_time + ((s64)zone*3600);
    }
    return Time_LocalTime_r(&temp_time,&datetime);
}

//----把格林威治日历时间转成格林威治分解时间(us)---------------------------------------
//功能: 把格林威治日历时间(1970年以来的微秒数)转换成年月日时分秒+微秒,
//参数: time,1970年来的格林威治微秒数,给空指针则直接使用日历时间
//      result，用来返回结果的指针，必须非空
//      zone，目标时区，若不在（0~23）之内，则取本地时区
//返回: 分解时间
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
    if( (zone < 0) ||(zone > 23) )    //要在24个时区内
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

//----把格林威治日历时间转成格林威治分解时间(us)---------------------------------------
//功能: 把格林威治日历时间(1970年以来的微秒数)转换成年月日时分秒+微秒,因共用一个静态缓冲
//      区返回结果，因此多次调用的话，结果只反映最后一次调用的值
//参数: time,1970年来的格林威治微秒数,给空指针则直接使用日历时间
//      zone，目标时区，若不在（0~23）之内，则取本地时区
//返回: 分解时间
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
    if( (zone < 0) ||(zone > 23) )    //要在24个时区内
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

//----把日历时间(秒)转成本地分解时间-------------------------------------------
//功能: 把日历时间(1970年以来的秒数)转换成年月日时分秒,
//参数: time,1970年来的秒数,给空指针则直接使用日历时间
//      result，用来返回结果的指针，必须非空
//返回: 分解时间
//-----------------------------------------------------------------------------
struct tm *Time_LocalTime_r(const s64 *time,struct tm *result)
{
    u32 second, minute, hour;
    u32 day, month, year;

    u32 days;
    u32 years;
    u32 leap_years;
    u32 dayth;    // 本年的第x天（1月1日0时0分0秒即为第1天了）
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
    if(temp_time > 47650000000) //算法在公元3480年后会出错
        return NULL;

    second = temp_time % 60;
    minute = temp_time/60 %60;
    hour = temp_time/3600 %24;
    days = (u32)(temp_time/86400);

    years = days / 365;
    leap_years = (years + 1) >> 2; // 不含本年度的闰
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
    dayth = temp - leap_years;    // 本年度第x天（如1月1日0时为第1天）

    year = 1970 + years;    //未作闰年调整，1400多年后会出错。

    g_u32MonthDays[1] = 28;
    if (year%4 == 0)        // 如果本年是闰年，那么先在2月份添加一天
    {
        if ((year%100 == 0) && (year%400 != 0))
        {
            // 百年不闰，四百年闰
        }
        else
        {
            g_u32MonthDays[1] = 29;
            if(temp1 < leap_years)// 闰年且整除所余天数小于闰年数，多加一天
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
    result->tm_us = 0;               // us清零
    result->tm_sec = second;        // seconds after the minute - [0,59]
    result->tm_min = minute;        // minutes after the hour - [0,59]
    result->tm_hour = hour;          // hours since midnight - [0,23]
    result->tm_wday = day_of_week;   // days since Sunday - [0,6]
    result->tm_mday = dayth+1;           // day of the month - [1,31]
    result->tm_mon = month;         // months since January - [0,11]
    result->tm_year = year - 1900;   // years 1970-，表示为与1900年的差值

    return result;
}

//----把日历时间(秒)转成本地分解时间-------------------------------------------
//功能: 把日历时间(1970年以来的秒数)转换成年月日时分秒,因共用一个静态缓冲区返回
//      结果，因此多次调用的话，结果只反映最后一次调用的值
//参数: time,1970年来的秒数,给空指针则直接使用日历时间
//返回: 分解时间
//-----------------------------------------------------------------------------
struct tm *Time_LocalTime(const s64 *time)
{
    static struct tm datetime;

    return Time_LocalTime_r(time,&datetime);
}

//----把日历时间(秒)转成本地分解时间-------------------------------------------
//功能: 把日历时间(1970年以来的秒数)转换成年月日时分秒,
//参数: time,1970年来的微秒数,给空指针则直接使用日历时间
//      result，用来返回结果的指针，必须非空
//返回: 分解时间
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

//----把日历时间(us)转成本地分解时间-------------------------------------------
//功能: 把日历时间(1970年以来的秒数)转换成年月日时分秒,
//参数: time,1970年来的微秒数,给空指针则直接使用日历时间
//返回: 分解时间
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

//----更新日历时间-------------------------------------------------------------
//功能: 用格式为"2011/10/28,22:37:50:000"的字符串表示的时间设置日历时间，可调用
//      本函数设置系统日历时间，最后一段是uS数，可以不给
//参数: buf，输入时间值
//返回: 1=成功，其他值:对应的错误代码
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
//----更新日历时间-------------------------------------------------------------
//功能: 用分解时间更新日历时间，有rtc硬件的系统中，如果硬件直接输出分解时间的，
//      硬件驱动调用本函数。
//      因为日历时间只精确到秒，因此tm中的us成员，由硬件驱动自行维护，本函数不读
//      取。如果硬件不维护us成员(调用tm_connect_rtc时参数get_rtc_hard_us=NULL),
//      rtc driver须使用中断方式，以确保秒跳变时立即调用get_rtc_hard_us，系统将
//      以此时刻为起点计算us数。
//参数: tm,当前时间，按年月日时分秒分解
//返回: 无
//-----------------------------------------------------------------------------
void Time_SetDateTime(struct tm *tm)
{
    s64 nowtime;
    nowtime = Time_MkTimeUs(tm);
    //use the rtc module to set
    __Rtc_SetTime(nowtime);
}

//----把分解时间转成字符串-----------------------------------------------------
//功能: 把一个分解时间转成字符串，格式"2011/10/28,22:37:50"
//参数: tm,分解时间
//      buf，用于返回结果的数组地址，至少21字节。
//返回: 分解时间
//----------------------------------------------------------------------------
void Time_AscTime(struct tm *tm, char buf[])
{
    itoa(tm->tm_year + 1900, &buf[0], 10);
    buf[4] = '/';
    itoa(tm->tm_mon, &buf[5], 10);
    if(buf[6] == '\0')  //删除字符串结尾，用0填充
    {
        buf[6] = buf[5];
        buf[5] = '0';
    }
    buf[7] = '/';
    itoa(tm->tm_mday, &buf[8], 10);
    if(buf[9] == '\0')  //删除字符串结尾，用0填充
    {
        buf[9] = buf[8];
        buf[8] = '0';
    }
    buf[10] = ',';
    itoa(tm->tm_hour, &buf[11], 10);
    if(buf[12] == '\0')  //删除字符串结尾，用0填充
    {
        buf[12] = buf[11];
        buf[11] = '0';
    }
    buf[13] = ':';
    itoa(tm->tm_min, &buf[14], 10);
    if(buf[15] == '\0')  //删除字符串结尾，用0填充
    {
        buf[15] = buf[14];
        buf[14] = '0';
    }
    buf[16] = ':';
    itoa(tm->tm_sec, &buf[17], 10);
    if(buf[18] == '\0')  //删除字符串结尾，用0填充
    {
        buf[18] = buf[17];
        buf[17] = '0';
    }
    buf[19] = '\0';

    return ;
}

//----把分解时间转成字符串(ms)-------------------------------------------------
//功能: 把一个分解时间转成字符串，格式"2011/10/28,22:37:50:000"
//参数: tm，分解时间
//      buf，用于返回结果的数组地址，至少24字节。
//返回: 字符串
//----------------------------------------------------------------------------
void Time_AscTimeMs(struct tm *tm, char buf[])
{
    Time_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us/1000, &buf[20], 10);
    buf[23] = '\0';
    return ;
}

//----把分解时间转成字符串(us)-------------------------------------------------
//功能: 把一个分解时间转成字符串，格式"2011/10/28,22:37:50:000000"
//参数: tm,分解时间
//      buf，用于返回结果的数组地址，至少27字节。
//返回: 字符串
//----------------------------------------------------------------------------
void Time_AscTimeUs(struct tm *tm, char buf[])
{
    Time_AscTime(tm,buf);
    buf[19] = ':';
    itoa(tm->tm_us, &buf[20], 10);
    buf[26] = '\0';
    return ;
}

//----取日历时间(秒)-----------------------------------------------------------
//功能: 取从1970年1月1日0:0:0到现在的时间差，秒数。
//参数: ret,非空则返回日历时间。
//返回: 日历时间
//-----------------------------------------------------------------------------
s64 Time_Time(s64 *ret)
{
    return __Rtc_Time(ret);
}
//----取日历时间(微秒)---------------------------------------------------------
//功能: 取从1970年1月1日0:0:0到现在的时间差，秒数。精确度只能到tick间隔
//参数: ret,非空则返回日历时间。
//返回: 日历时间
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

//TODO：补充 timezone 相关代码
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

//TODO：补充 timezone 相关代码
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


