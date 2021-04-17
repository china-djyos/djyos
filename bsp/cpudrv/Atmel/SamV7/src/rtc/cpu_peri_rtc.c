//-----------------------------------------------------------------------------

// 文件名     ：cpu_peri_busram.c
// 模块描述: CPU片内backup 1K SRAM读写操作
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 09/11.2015
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#include "project_config.h"    //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    ModuleInstall_CpuRtc(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip rtc"//CPU的rtc外设驱动
//parent:"rtc"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                      //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                   //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                     //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                   //初始化时机，可选值：early，medium，later, pre-main。
                                     //表示初始化时间，分别是早期、中期、后期
//dependence:"int","lock","time"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                     //如果依赖多个组件，则依次列出
//weakdependence:"none"              //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件不会被强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                     //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RTC == false )
//#warning  " cpu_onchip_rtc  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RTC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))

static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //需更新的时间

// =============================================================================
// 功能：从RTC设备中读取RTC时间，单位微秒,取从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值，需把日历时间转换成1970年1月1日0:0:0到现在的时间差
// 返回：true,正常操作，否则出错
// =============================================================================
bool_t RTC_GetTime(s64 *time)
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

    *time = 1000000 * Time_MkTime(&dtm);
    return true;
}

// =============================================================================
// 功能：设置RTC设备RTC时间，单位微秒，该时间从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值
// 返回：true,正常操作，否则出错
// =============================================================================
bool_t RTC_SetTime(s64 time)
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
    Time_LocalTime_r(&time_s,&dtm);

    if((dtm.tm_year > 2000) && (dtm.tm_year < 2099))
    {
        RTC->RTC_CR |= RTC_CR_UPDTIM | RTC_CR_UPDCAL;

        while((RTC->RTC_SR & RTC_SR_ACKUPD) != RTC_SR_ACKUPD)
        {
            DJY_EventDelay(100*mS);
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
// 功能：RTC时间更新任务，由于Atmel芯片的RTC更新时间最长可能达到1秒因此，因此专门用一个低
//      优先级的任务作为更新RTC任务，以防占用其他线程的CPU时间
// 参数：无
// 返回：无
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
// 功能：RTC时间注册及初始化
// 参数：time, 时间值
// 返回：true,正常操作，否则出错
// =============================================================================
ptu32_t ModuleInstall_CpuRtc(ptu32_t para)
{
    s64 rtc_time;
    struct timeval tv;
    u16 evtt;

    //初始化硬件
    PMC_EnablePeripheral(ID_RTC);
    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP");

    if(NULL == pRtcSemp)
        return false;

    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Rtc_UpdateTime,NULL,800,
                                "RTC Update Event");

    if(evtt == CN_EVTT_ID_INVALID)
    {
        free(pRtcSemp);
        return false;
    }
    DJY_EventPop(evtt,NULL,0,NULL,0,0);
    RTC_GetTime(&rtc_time);

    tv.tv_sec = rtc_time/1000000;//us ---> s
    tv.tv_usec = 0;

    settimeofday(&tv,NULL);

    //注册RTC时间
    if(!Rtc_RegisterDev(NULL,RTC_SetTime))
        return false;
    return true;
}
