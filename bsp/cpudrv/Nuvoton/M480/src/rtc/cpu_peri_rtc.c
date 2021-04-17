//-----------------------------------------------------------------------------

// 模块版本: V1.00
// 创建人员:yt
// 创建时间:2017
// =============================================================================
#if 0
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "time.h"
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
    u32 temp=0;
    temp=RTC_BASE+0x50+BKRx*4;
    return (*(u32*)temp);       //返回读取到的值
}

// =============================================================================
// 功能：RTC写入后备区域SRAM
// 参数：BKPx:后备区寄存器编号,范围:0~19
//     data:要写入的数据,32位长度
// 返回：true,正常操作，否则出错
// =============================================================================

void BKP_WriteBackupRegister(u32 BKRx,u32 data)
{
    u32 temp=0;
    temp=RTC_BASE+0x50+BKRx*4;
    (*(u32*)temp)=data;
}
// =============================================================================
// 功能：等待RSF同步，防止在同步时读取造成误差
// 参数：void
// 返回：
// =============================================================================
bool_t RTC_Wait_Rsf(void)
{
    u32 retry=0XFFFFF;

    RTC->WPR=0xCA;
    RTC->WPR=0x53;
    RTC->ISR&=~(1<<5);      //清除RSF位

    while(retry&&((RTC->ISR&(1<<5))==0x00))
    {
        retry--;
    }

    if(retry==0)
        return false; //同步失败
    RTC->WPR=0xFF;    //使能RTC寄存器写保护
    return true;
}
// =============================================================================
// 功能：从RTC进入初始化模式
// 参数：无
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t RTC_Init_Mode(void)
{
    u32 retry=0XFFFFF;
    if(RTC->ISR&(1<<6))  //初始化RTC
        return true;
    RTC->ISR|=1<<7;      //初始化模式，用于编程时间和日期寄存器（ RTC_TR和 RTC_DR）以
                         //及预分频器寄存器(RTC_PRER)。计数器停止计数，当 INIT被复位后，计数器从新值开始计数。
    while(retry&&((RTC->ISR&(1<<6))==0x00))  //   ==判断是否相等，若等则为真1，等待RTC进入初始化状态
        retry--;
    if(retry==0)
        return false;
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
    u32 year,month,date,hour,min,sec;
    u32 DR_bak=0,TR_bak=0;
    u32 tim_us;
    u8 timeout=10;
    while(timeout&&(false==RTC_Wait_Rsf()))
    {
        timeout--;
    }
    if(0==timeout)  //等待同步
        return false;

    DR_bak=RTC->DR;//将年月日。。一次读出来防止进位误差
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
    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);

    //关闭RTC寄存器写保护
    RTC->WPR=0xCA;
    RTC->WPR=0x53;
    if(false==RTC_Init_Mode())
        return false;//进入RTC初始化模式失败

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

    RTC->ISR&=~(1<<7);          //退出RTC初始化模式

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

    u16 retry=400;
    u32 SSR;

    RCC->APB1ENR|=1<<28;        //使能电源接口时钟，即使能PWR时钟
    PWR->CR|=1<<8;              //后备区域访问使能，即使能对 RTC、RTC备份寄存器和备份 SRAM的访问
    if(LSE_Flag_Reg!=BKP_ReadBackupRegister(BAK_Reg))
    {

            RCC->BDCR=1<<16;            //复位BDCR
            DJY_DelayUs(10);
            RCC->BDCR=0;                //结束复位

            RCC->CSR|=1<<0;             //LSI总是使能
            while(!(RCC->CSR&0x02));    //等待LSI就绪

            RCC->BDCR|=1<<0;            //尝试开启LSE
            while(retry&&((RCC->BDCR&0X02)==0))//等待LSE准备好
            {
                retry--;
                DJY_DelayUs(5*1000);
            }

            RCC->BDCR&=~(3<<8);         //清零BDCR寄存器第8、9位，即无时钟

            if(retry==0)
            {
                BKP_WriteBackupRegister(BAK_Reg,LSI_Flag_Reg);
                RCC->BDCR|=1<<9;    //LSE开启失败,选择LSI作为RTC时钟
            }
            else
            {
                BKP_WriteBackupRegister(BAK_Reg,LSE_Flag_Reg);
                RCC->BDCR|=1<<8;            //选择LSE,作为RTC时钟
            }
            RCC->BDCR|=1<<15;               //使能RTC时钟

            RTC->WPR=0xCA;  //关闭RTC寄存器写保护
            RTC->WPR=0x53;

            /*手册中，系统复位后，可通过 PWR 电源控制寄存器 (PWR_CR)的 DBP位保护 RTC寄存器以防止
            非正常的写访问。必须将 DBP位置 1 才能使能 RTC寄存器的写访问。
            上电复位后，所有 RTC寄存器均受到写保护。通过向写保护寄存器 (RTC_WPR)写入一个
            密钥来使能对 RTC寄存器的写操作。
            要解锁所有 RTC寄存器（ RTC_ISR[13:8]、 RTC_TAFCR 和 RTC_BKPxR 除外）的写保护，
            需要执行以下步骤：
            1. 将“ 0xCA”写入 RTC_WPR 寄存器。
            2. 将“ 0x53”写入 RTC_WPR 寄存器。*/

            RTC->CR=0;   //RTC控制寄存器清零
            if(false==RTC_Init_Mode())
            {
                RCC->BDCR=1<<16;        //复位BDCR
                DJY_DelayUs(10);
                RCC->BDCR=0;            //结束复位
                return false;           //进入RTC初始化模式
            }
            RTC->PRER=0XFF;             //RTC同步分频系数(0~7FFF),必须先设置同步分频,再设置异步分频,Frtc=Fclks/((Sprec+1)*(Asprec+1))
            RTC->PRER|=0X7F<<16;        //RTC异步分频系数(1~0X7F)
            RTC->CR&=~(1<<6);           //RTC设置为,24小时格式
            RTC->ISR&=~(1<<7);          //退出RTC初始化模式
            RTC->WPR=0xFF;              //使能RTC寄存器写保护
            //第一次配置装载时间初值退出初始化模式
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
        retry=10;       //连续10次SSR的值都没变化,则LSE死了.
        SSR=RTC->SSR;   //读取初始值
        while(retry)    //检测ssr寄存器的动态,来判断LSE是否正常
        {
            DJY_DelayUs(10*1000);
            if(SSR==RTC->SSR)retry--;   //对比
            else break;
        }
        if(retry==0)    //LSE挂了,清除配置等待下次进入重新设置
        {
            BKP_WriteBackupRegister(BAK_Reg,0XFFFF);    //标记错误的值
            RCC->BDCR=1<<16;            //复位BDCR
            DJY_DelayUs(10);
            RCC->BDCR=0;                //结束复位
        }
    }
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
    if(!Rtc_RegisterDev(NULL,RTC_SetTime))
        return false;
    return true;
}

#endif



