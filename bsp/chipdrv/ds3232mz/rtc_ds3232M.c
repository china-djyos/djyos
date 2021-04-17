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
// =============================================================================

// 文件名     ：RTC_DS3232M.c
// 模块描述: RTC芯片DS3232M驱动
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 18/08.2017
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "os.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "rtc_ds3232M.h"
#include "board-config.h"
#include <time.h>
#include "shell.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern ptu32_t ModuleInstall_RTCDS3232M(void);
//   ModuleInstall_RTCDS3232M( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"RTC chip ds3232M"//spi接口的rtc芯片
//parent:"none"                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","cpu onchip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_RTC_CHIP_DS3232M == false )
//#warning  " RTC_chip_ds3232M  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_RTC_CHIP_DS3232M    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_DS3232M_BUS_NAME              "IIC2"      //"SPI总线名称",DS3232M使用的SPI总线名称
//%$#@num,0,255,
#define CFG_DS3232MZ_ADDR   (0xD0>>1)       //"器件地址"，
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

bool_t rtcst(char *param);
bool_t rtcgt(void);

#define HexToBcd(x) ((((x) / 10) <<4) + ((x) % 10))            //将16进制转换成BCD码
#define BcdToHex(x) ((((x) & 0xF0) >>4) * 10 + ((x) & 0x0F))   //将BCD码转换成16进制



static struct IIC_Device *s_ptDS3232MZ_Dev=NULL;
u32 timeout = 1000*mS;                 //1000*mS;
#define DS3232MZ_CLK_FRE             (200*1000)      //总线速度，单位Hz
static struct MutexLCB *  ptSemID_RTC;       // IIC操作互斥标志
static struct SemaphoreLCB *pRtcSemp  = NULL;
static s64  UpdateTime = 0;                     //需更新的时间


#define DS3232M_TIME_HOUR_MODE24_MASK   0x3F
#define DS3232M_TIME_HOUR_MODE12_MASK   0x7F

#define DS3232M_ALARM_HOUR_MODE24_MASK  0xBF
#define DS3232M_ALARM_HOUR_MODE12_MASK  0xFF
#define DS3232M_ALARM_DATE_MODE_MASK    0xBF
#define DS3232M_ALARM_DAY_MODE_MASK     0xC7


//连续读写的寄存器个数
#define DS3232_TIME_REG_LEN         7       //DS1390=8多ms位
#define DS3232_ALARM_REG_LEN        5       //DS1390=5多ms位,DS3232M用Alarm2.min位做ms位


#define DS3232_TIME_ADDR    0x00

#define DS3232_REG_SECONDS  0x00
#define DS3232_REG_MINUTES  0x01
#define DS3232_REG_HOURS    0x02
#define DS3232_REG_AMPM     0x02
#define DS3232_REG_DAY      0x03
#define DS3232_REG_DATE     0x04
#define DS3232_REG_MONTH    0x05
#define DS3232_REG_CENTURY  0x05
#define DS3232_REG_YEAR     0x06
#define DS3232_REG_ALARM1   0x07    /* Alarm 1 BASE */
#define DS3232_REG_ALARM2   0x0B    /* Alarm 2 BASE */
#define DS3232_REG_CR       0x0E    /* Control register */

#define DS3232_REG_CR_nEOSC 0x80
#define DS3232_REG_CR_INTCN 0x04
#define DS3232_REG_CR_A2IE  0x02
#define DS3232_REG_CR_A1IE  0x01
#define DS3232_REG_SR       0x0F    /* control/status register */
#define DS3232_REG_SR_OSF   0x80
#define DS3232_REG_SR_BSY   0x04
#define DS3232_REG_SR_A2F   0x02
#define DS3232_REG_SR_A1F   0x01


ptu32_t Rtc_UpdateTime(void);
static bool_t RTC_GetTime(s64 *time);



// =============================================================================
// 功能：将调用者提供的新增器件结点挂接到IIC总线结点上,调用者提供IIC_Device控制块
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,新增器件名称
//       NewDev,新增器件指针
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
static s32 __rtc_ReadData(uint8_t byAddress,uint8_t *pbyBuf,uint8_t byLen)
{
    s32 ret;
    ret=IIC_Read(s_ptDS3232MZ_Dev,byAddress,pbyBuf,byLen,timeout);

    return ret;
}


// =============================================================================
// 功能：将调用者提供的新增器件结点挂接到IIC总线结点上,调用者提供IIC_Device控制块
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,新增器件名称
//       NewDev,新增器件指针
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
static s32 __rtc_WriteData(uint8_t byAddress,uint8_t *pbyBuf,uint8_t byLen)
{
    s32 ret;
    ret=IIC_Write(s_ptDS3232MZ_Dev,byAddress,pbyBuf,byLen,true,timeout);
    return ret;
}

// =============================================================================
// 功能：将调用者提供的新增器件结点挂接到IIC总线结点上,调用者提供IIC_Device控制块
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,新增器件名称
//       NewDev,新增器件指针
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
bool_t RTC_DS3232MZ_UpdateTime(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_TIME_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec);
    dat[1]=HexToBcd(ptm->tm_min);
    dat[2]=HexToBcd(ptm->tm_hour);
    dat[3]=HexToBcd(ptm->tm_wday);
    dat[4]=HexToBcd(ptm->tm_mday);
    dat[5]=HexToBcd(ptm->tm_mon);
    dat[6]=HexToBcd(ptm->tm_year-2000);
    ret=__rtc_WriteData(DS3232_TIME_ADDR,dat,DS3232_TIME_REG_LEN);
    if(ret==-1)
        return false;
    return true;

}

// =============================================================================
// 功能：将调用者提供的新增器件结点挂接到IIC总线结点上,调用者提供IIC_Device控制块
// 参数：BusName,新增子器件挂接的总线名称
//       DevName,新增器件名称
//       NewDev,新增器件指针
// 返回：控制块结点指针，添加失败时返回NULL
// =============================================================================
bool_t RTC_DS3232MZ_GetTime(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_TIME_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_TIME_ADDR,dat,DS3232_TIME_REG_LEN);
    if(ret==-1)
        return false;
     //将BCD格式转化为正常模式
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_wday   = BcdToHex(dat[3] & 0x07);
    ptm->tm_mday   = BcdToHex(dat[4] & 0x3F);
    ptm->tm_mon    = BcdToHex(dat[5] & 0x1F);
    ptm->tm_year   = BcdToHex(dat[6]) + 2000;
    return true;
}


bool_t RTC_DS3232MZ_SetAlarm1Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec)&0x7F;
    dat[1]=HexToBcd(ptm->tm_min)&0x7F;
    dat[2]=HexToBcd(ptm->tm_hour&DS3232M_ALARM_HOUR_MODE24_MASK);
    dat[3]=HexToBcd(ptm->tm_mday&DS3232M_ALARM_DATE_MODE_MASK);
    ret=__rtc_WriteData(DS3232_REG_ALARM1,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
    return true;
}

bool_t RTC_DS3232MZ_SetAlarm2Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    dat[0]=HexToBcd(ptm->tm_sec)&0x7F;;
    dat[1]=HexToBcd(ptm->tm_min)&0x7F;;
    dat[2]=HexToBcd(ptm->tm_hour&DS3232M_ALARM_HOUR_MODE24_MASK);
    dat[3]=HexToBcd(ptm->tm_mday&DS3232M_ALARM_DATE_MODE_MASK);
    ret=__rtc_WriteData(DS3232_REG_ALARM2,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
    return true;
}


bool_t RTC_DS3232MZ_GetAlarm1Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_REG_ALARM1,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
     //将BCD格式转化为正常模式
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_mday   = BcdToHex(dat[3] & 0x3F);
    return true;
}

bool_t RTC_DS3232MZ_GetAlarm2Time(struct tm *ptm)
{
    s32 ret;
    uint8_t dat[DS3232_ALARM_REG_LEN];
    if(ptm==NULL)
    {
        printf("%s para is invalid.\r\n",__FUNCTION__);
        return false;
    }
    ret=__rtc_ReadData(DS3232_REG_ALARM2,dat,DS3232_ALARM_REG_LEN);
    if(ret==-1)
        return false;
     //将BCD格式转化为正常模式
    ptm->tm_sec    = BcdToHex(dat[0] & 0x7F);
    ptm->tm_min    = BcdToHex(dat[1] & 0x7F);
    ptm->tm_hour   = BcdToHex(dat[2]);
    ptm->tm_mday   = BcdToHex(dat[3] & 0x3F);
    return true;
}


static bool_t RTC_DS3232MZ_ClearAlarmSta(void)
{
    s32 ret;
    u8 dat;

    ret=__rtc_ReadData(DS3232_REG_SR,&dat,1);
    if(ret==-1)
        return false;

    dat &= (u8)(~(DS3232_REG_SR_A1F | DS3232_REG_SR_A1F));//clear;
    ret=__rtc_WriteData(DS3232_REG_SR,&dat,1);
    if(ret==-1)
        return false;
    return true;
}

static bool_t RTC_DS3232MZ_AlarmConfig(u8 alarm1,u8 alarm2)
{
    s32 ret;
    u8 dat;

    ret=__rtc_ReadData(DS3232_REG_CR,&dat,1);
    if(ret==-1)
        return false;

    dat &= ~(DS3232_REG_CR_INTCN | DS3232_REG_CR_A2IE |DS3232_REG_CR_A1IE );
    if(alarm1 || alarm2 )
    {
        dat |= DS3232_REG_CR_INTCN;
        if(alarm1)
            dat |= DS3232_REG_CR_A1IE;
        if(alarm2)
            dat |= DS3232_REG_CR_A2IE;
    }
    ret=__rtc_WriteData(DS3232_REG_CR,&dat,1);
    if(ret==-1)
        return false;

    return true;
}

void (*gfunRTC_TrigIoInit)(void) =  NULL;

bool_t RTC_SetAlarm(u32 timeout_s)
{
    bool_t result = false;
    static bool_t sAlarmSet = false;
    s64 time;
    struct tm dtm;

    if(true == RTC_DS3232MZ_ClearAlarmSta())
    {
        if(false == sAlarmSet)
        {
            RTC_DS3232MZ_AlarmConfig(1,0);
            sAlarmSet = true;
            if(NULL != gfunRTC_TrigIoInit)
                gfunRTC_TrigIoInit();
        }

        if(true == sAlarmSet)
        {
            if(true == RTC_DS3232MZ_GetTime(&dtm))
            {
                time = (s64)(Time_MkTime(&dtm) + timeout_s);

                Time_LocalTime_r(&time,&dtm);
                result = RTC_DS3232MZ_SetAlarm1Time(&dtm);

                RTC_DS3232MZ_GetAlarm1Time(&dtm);
            }
        }
    }

    return result;
}

bool_t RTC_AlarmConfig(void (*RTC_TrigIoInit)(void))
{
    bool_t result = false;
    if(NULL != RTC_TrigIoInit)
    {
        gfunRTC_TrigIoInit = RTC_TrigIoInit;
        result = true;
    }
    return result;
}

/* =============================================================================
 功能：DS3232MZ芯片初始化，初始化和加载设备到对应的IIC总线.
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/

static bool_t DS3232MZ_Init(char *BusName)
{
    ptSemID_RTC = Lock_MutexCreate("DS3232MZ Lock");
    if(ptSemID_RTC==NULL)
    {
        printf("%s malloc failed.\r\n",__FUNCTION__);
        return false;
    }

    s_ptDS3232MZ_Dev = IIC_DevAdd(BusName,"DS3232MZ",CFG_DS3232MZ_ADDR,0,8);
    if(s_ptDS3232MZ_Dev)
    {
        IIC_BusCtrl(s_ptDS3232MZ_Dev,CN_IIC_SET_CLK,DS3232MZ_CLK_FRE,0);//设置时钟大小
        IIC_BusCtrl(s_ptDS3232MZ_Dev,CN_IIC_SET_POLL,0,0);       //使用中断方式发送
        return true;
    }
    else
    {
        Lock_MutexDelete(ptSemID_RTC);
        return false;
    }
}


// =============================================================================
// 功能：从RTC设备中读取RTC时间，单位微秒,取从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值，需把日历时间转换成1970年1月1日0:0:0到现在的时间差
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t RTC_GetTime(s64 *time)
{
    struct tm dtm;
    RTC_DS3232MZ_GetTime(&dtm);
    *time = (s64)(1000000 * Time_MkTime(&dtm));
    return true;
}

// =============================================================================
// 功能：设置RTC设备RTC时间，单位微秒，该时间从1970年1月1日0:0:0到现在的时间差
// 参数：time, 时间值
// 返回：true,正常操作，否则出错
// =============================================================================
static bool_t RTC_SetTime(s64 time)
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
    bool_t ret;
    time_s = time/1000000;
    Time_LocalTime_r(&time_s,&dtm);
    ret=RTC_DS3232MZ_UpdateTime(&dtm);
    return ret;
}

// =============================================================================
// 功能：RTC时间更新任务，专门用一个低
//     优先级的任务作为更新RTC任务，以防占用其他线程的CPU时间
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


ptu32_t ModuleInstall_RTCDS3232M(void)
{
    u16 evtt;
    s64 rtc_time;
    struct timeval tv;

    DS3232MZ_Init(CFG_DS3232M_BUS_NAME);
    pRtcSemp = Lock_SempCreate(1,0,CN_BLOCK_FIFO,"RTC_SEMP");
    if(NULL == pRtcSemp)
         return -1;
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

    tv.tv_sec  = rtc_time/1000000;
    tv.tv_usec = rtc_time%1000000;

    settimeofday(&tv,NULL);

//  RTC_Shell_Module_Install();
    //注册RTC时间
    if(!Rtc_RegisterDev(NULL,RTC_SetTime))
        return false;
    else
        return true;
}



//void Sh_RTC_SetTime(char *param)
bool_t rtcst(char *param)
{
    char command[20];
    bool_t ret;
    struct tm ptDateTime;
    if(param!=NULL)
    {
         memcpy(command,param,20);
         sscanf(command,"%d/%d/%d,%d:%d:%d:%d",\
                     &ptDateTime.tm_year,&ptDateTime.tm_mon,&ptDateTime.tm_mday,\
                     &ptDateTime.tm_hour,&ptDateTime.tm_min,&ptDateTime.tm_sec);
         ret=RTC_DS3232MZ_UpdateTime(&ptDateTime);
         if(ret)
         {
             printf("Update Time OK.\r\n");
         }
         else
         {
             printf("Update Time failed.\r\n");
         }
    }

    return true;

}

//void Sh_RTC_GetTime(void)
bool_t rtcgt(void)
{
    struct tm ptDateTime;
    bool_t ret;
    ret=RTC_DS3232MZ_GetTime(&ptDateTime);
    if(!ret)
    {
         printf("Get RTC Time failed.\r\n");
    }
    else
    {
         printf("Get RTC Time:%04d/%02d/%02d,%02d:%02d:%02d.\r\n",
                ptDateTime.tm_year, ptDateTime.tm_mon, ptDateTime.tm_mday,
                ptDateTime.tm_hour, ptDateTime.tm_min,ptDateTime.tm_sec);
    }
   return true;
}


ADD_TO_ROUTINE_SHELL(rtcst,rtcst,"设置RTC时间   格式:2017/08/20,21:00:00");
ADD_TO_ROUTINE_SHELL(rtcgt,rtcgt,"获取RTC时间");
