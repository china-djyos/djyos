//-----------------------------------------------------------------------------

// 模块版本: V1.00
// 创建人员:huzb
// 创建时间:2018
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
//    ModuleInstall_CpuRtc(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip rtc"//CPU的rtc外设驱动
//parent:"rtc"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                  //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence:"time","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件将强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"             //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                    //选中该组件时，被依赖组件不会被强制选中，
                                    //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RTC == false )
//#warning  " cpu_onchip_rtc  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RTC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//当前所属世纪-20 世纪

#define CN_CUURENT_CENTURY_CONFIG (2000)

typedef struct
{
    volatile u32 Time0;
    volatile u32 Time1;
    volatile u32 CurTime0;
    volatile u32 CurTime1;
    volatile u32 AlarmTime0;
    volatile u32 AlarmTime1;
    volatile u32 RESERVED0[2];
    volatile u32 Control;
    volatile u32 Status;
}tagRtcReg;

static volatile tagRtcReg *sgpt_RtcReg = (volatile tagRtcReg *)0x40900000UL;


#define LSE_Flag_Reg 0xA5A5   //标志，板件掉电之后
#define LSI_Flag_Reg 0xA5A0   //标志

#define BAK_Reg   2  //??

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))  //16进制转换成BCD
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F)) //BCD转换成16进制

static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //需更新的时间

// =============================================================================
// 功能：RTC读后备区域SRAM
// 参数：BKPx:后备区寄存器编号,范围:0~19
// 返回：后备区域数据
// =============================================================================

u32 BKP_ReadBackupRegister(u32 BKRx)
{

}

// =============================================================================
// 功能：RTC写入后备区域SRAM
// 参数：BKPx:后备区寄存器编号,范围:0~19
//     data:要写入的数据,32位长度
// 返回：true,正常操作，否则出错
// =============================================================================

void BKP_WriteBackupRegister(u32 BKRx,u32 data)
{

}
// =============================================================================
// 功能：等待RSF同步，防止在同步时读取造成误差
// 参数：void
// 返回：
// =============================================================================
bool_t RTC_Wait_Rsf(void)
{

}

// =============================================================================
// 功能：从RTC进入初始化模式
// 参数：无
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t RTC_Init_Mode(void)
{
    u32 tmp = 0;

    //关闭Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //等待Rtc失能生效
    while(sgpt_RtcReg->Status & (1<<0));

    //清闹钟以及周期性中断
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

    return true;
}
// =============================================================================
// 功能：从RTC设备中读取RTC时间，单位微秒,取从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值，需把日历时间转换成1970年1月1日0:0:0到现在的时间差
// 返回：true,正常操作，否则出错
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;
    u32 temp1,temp2;
    u32 year,month,date,hour,min,sec;
    u8 secondU,secondD;
    u8 minuteU,minuteD;
    u8 hourU,hourD;
    u8 weekDay;
    u8 dayU,dayD;
    u8 monthU,monthD;
    u8 yearU,yearD;

    //读取当前寄存器
    temp1 = sgpt_RtcReg->CurTime0;
    temp2 = sgpt_RtcReg->CurTime1;

    //低32timer为时分秒以及星期信息
    secondU =  temp1 & 0xf;                   //获取sec信息
    secondD = (temp1 & (0x7 << 0x4)) >> 0x4;

    minuteU = (temp1 & (0xf << 0x8)) >> 0x8;  //获取min信息  0~6有效
    minuteD = (temp1 & (0x7 << 12)) >> 12;

    hourU   = (temp1 & (0xF << 16)) >> 16;
    hourD   = (temp1 & (0x3 << 20)) >> 20;    //获取hour信息

    weekDay = (temp1 & (0x7 << 24)) >> 24;    //获取星期信息

    //高32Bit记录年份以及月份 日信息
    dayU    =  temp2 & 0xf;
    dayD    = (temp2 & 0x3 << 4)   >>4;

    monthU  = (temp2 & (0xF << 8)) >>8;
    monthD  = (temp2 & (0x1 << 12))>>12;

    yearU   = (temp2 & (0xF << 16))>>16;
    yearD   = (temp2 & (0xF << 20))>>20;

    //因为获取的年份失具体所属世纪(20)的差年-故还原具体年份时间应该加所属世纪

    //将时间转换为年、月、日、时、分、秒 - 具体
    year   = yearD * 10  + yearU + CN_CUURENT_CENTURY_CONFIG;
    month  = monthD * 10 + monthU;
    date   = dayD * 10 + dayU;

    hour   = hourD * 10 + hourU;
    min    = minuteD * 10 + minuteU;
    sec    = secondD * 10 + secondU;

    u32 tim_us = 0;

    dtm.tm_year = year;
    dtm.tm_mon  = month;
    dtm.tm_mday = date;
    dtm.tm_hour = hour;
    dtm.tm_min  = min;
    dtm.tm_sec  = sec;

    *time = (s64)(1000000 * Time_MkTime(&dtm)+tim_us);
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
    Lock_SempPost(pRtcSemp);  //点亮一个信号灯
    return true;
}

static bool_t __Rtc_SetTime(s64 time)
{
    struct tm dtm;
    s64 time_s;
    u8 tm_wday;
    u32 year,month,date,hour,min,sec;
    u8 secondU,secondD;
    u8 minuteU,minuteD;
    u8 hourU,hourD;
    u8 weekDay;
    u8 dayU,dayD;
    u8 monthU,monthD;
    u8 yearU,yearD;
    u32 timeValSet = 0;

    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);



    weekDay = tm_wday=dtm.tm_wday;

    //关闭Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //等待Rtc失能生效
    while(sgpt_RtcReg->Status & (1<<0));

    //清闹钟以及周期性中断
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

    year   = dtm.tm_year-CN_CUURENT_CENTURY_CONFIG;
    yearD  = year / 10;
    yearU  = year % 10;

    month  = dtm.tm_mon;
    monthD = dtm.tm_mon /10;
    monthU  = dtm.tm_mon %10;

    date   = dtm.tm_mday;
    dayD   = date /10;
    dayU   = date %10;

    hour   = dtm.tm_hour;
    hourD  = hour /10;
    hourU  = hour %10;

    min    = dtm.tm_min;
    minuteD= min /10;
    minuteU= min %10;

    sec    = dtm.tm_sec;
    secondD= sec /10;
    secondU= sec %10;

    //设置写低位进寄存器Time0
    timeValSet |= secondU;          //set sec_u
    timeValSet |= (secondD << 0x4);

    timeValSet |= (minuteU << 0x8); //set min_u
    timeValSet |= (minuteD << 12);

    timeValSet |= (hourU << 16);
    timeValSet |= (hourD << 20);      //set hour_u

    timeValSet |= (weekDay<<24);      //set weekday

    sgpt_RtcReg->Time0 = timeValSet;
    timeValSet = 0;

    //设置写低位进寄存器Time1
    timeValSet |= dayU;             //set day_u
    timeValSet |= (dayD<<4);

    timeValSet |= (monthU << 8);        //set month_u
    timeValSet |= (monthD << 12);

    timeValSet |= (yearU << 16);        //set year_u
    timeValSet |= (yearD << 20);

    sgpt_RtcReg->Time1 = timeValSet;

    //重新使能Rtc
    sgpt_RtcReg->Control |= (1<<0);

    //等待Rtc就绪
    while(!(sgpt_RtcReg->Status & (1<<0)));


    return true;
}

// =============================================================================
// 功能：RTC时间更新任务
//     专门用一个低优先级的任务作为更新RTC任务，以防占用其他线程的CPU时间
// 参数：无
// 返回：无
// =============================================================================
ptu32_t Rtc_UpdateTime(void)
{

    while(1)
    {
        if(Lock_SempPend(pRtcSemp,CN_TIMEOUT_FOREVER))  //请求一盏信号灯
        {
            __Rtc_SetTime(UpdateTime);
        }
    }
    return 0;
}

// =============================================================================
// 功能：RTC硬件初始化
//     首先检查LSE是否能可用,如果不行则使用LSI时钟
// 参数：void
// 返回：true,正常操作，否则出错
// =============================================================================

static bool_t RTC_Configuration(void)
{

    IO_Configure_PullDown(P_GPIO109, DISABLE);
    IO_Configure_PullDown(P_GPIO110, DISABLE);
    XTAL32_InitPad();
    EXTAL32_InitPad();
    CLK_Enable_XLOSC(ENABLE);

    //关闭Rtc
    sgpt_RtcReg->Control &= ~(1<<0);

    //等待Rtc失能生效
    while(sgpt_RtcReg->Status & (1<<0));

    //清闹钟以及周期性中断
    sgpt_RtcReg->Status |= (1<<4)|(1<<5);

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
    u16 evtt;

    RTC_Configuration();    //配置RTC
    //初始化硬件

    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP"); //创建一个信号量

    if(NULL == pRtcSemp)
        return false;

    evtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Rtc_UpdateTime,NULL,800,
                                "RTC Update Event");
    //登记一个事件类型：关联型时间类型，优先级130，入口函数Rtc_UpdateTime，栈尺寸800

    if(evtt == CN_EVTT_ID_INVALID)
    {
        free(pRtcSemp);
        return false;
    }

    DJY_EventPop(evtt,NULL,0,NULL,0,0);
    //弹出事件

    RTC_GetTime(&rtc_time);  //从RTC设备中读取RTC时间，单位是us,
                             //读取的值放到&rtc_time中

    tv.tv_sec  = rtc_time/1000000;   //把读取的时间中s的部分留下
    tv.tv_usec = rtc_time%1000000;   //把读取的时间中us的部分留下(余数)

    settimeofday(&tv,NULL);      //????

    //注册RTC时间
    if(!Rtc_RegisterDev(RTC_GetTime,RTC_SetTime))
        return false;
    return true;
}







