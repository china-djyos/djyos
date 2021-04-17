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
//所属模块：时钟模块
//作者：林谦
//版本：V0.1.0
//文件描述：DJYOS与硬件RTC连接文件
//其他说明：
//修订历史：
//2. 日期：2012-06-26
//   作者：Merlin
//   新版本号：
//   修改说明：事件初始化在djyos/clock.c中进行，本文件用于操作运行平台中的RTC硬件
//1. 日期: 2012-05-02
//   作者：林谦
//   新版本号：V0.1.0
//   修改说明：原始版本
//------------------------------------------------------
#include "stddef.h"
#include "time.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    ModuleInstall_RTC(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip rtc"//CPU的rtc外设驱动
//parent:"rtc"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"int","time","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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

#define LSE_Flag_Reg 0xA5A5
#define LSI_Flag_Reg 0xA5A0
#define BAK_Reg   RTC_BKP_DR1

RTC_HandleTypeDef RTC_Handler;  //RTC句柄


static u32 RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((u32)RTC->CNTH << 16 ) | tmp) ;
}

static void RTC_WaitForLastTask(void)
{
  /* Loop until RTOFF flag is set */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
  }
}
static void RTC_SetCounter(u32 CounterValue)
{
    RTC->CRL |= RTC_CRL_CNF;
  /* Set RTC COUNTER MSB word */
  RTC->CNTH = CounterValue >> 16;
  /* Set RTC COUNTER LSB word */
  RTC->CNTL = (CounterValue & RTC_CNTL_RTC_CNT_Msk);
  RTC->CRL &= (uint16_t)~((uint16_t)RTC_CRL_CNF);
}

static void RTC_WaitForSynchro(void)
{
  /* Clear RSF flag */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* Loop until RSF flag is set */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
  }
}

// =============================================================================
// 功能：RTC硬件寄存器配置
// 参数：无
// 返回：无
// =============================================================================
void RTC_Configuration(void)
{
    u32 timeout;

    RTC_Handler.Instance=RTC;
    RTC_Handler.Init.AsynchPrediv=32767;
    RTC_Handler.Init.OutPut=RTC_OUTPUTSOURCE_NONE;

    RCC->APB1ENR |=RCC_APB1ENR_BKPEN|RCC_APB1ENR_PWREN;
    HAL_PWR_EnableBkUpAccess();//备份区访问使能

    if(HAL_RTCEx_BKUPRead(&RTC_Handler,BAK_Reg)!=LSE_Flag_Reg)
    {
        RCC->BDCR=RCC_BDCR_BDRST;            //复位BDCR
        DJY_DelayUs(10);
        RCC->BDCR=0;                //结束复位

        RCC->CSR|=RCC_CSR_LSION;                //LSI总是使能
        while(!(RCC->CSR&RCC_CSR_LSIRDY));    //等待LSI就绪

        RCC->BDCR |=RCC_BDCR_LSEON;//开启外部低速时钟
        timeout=0;
        while(timeout<1000)
        {
           if(RCC->BDCR|RCC_BDCR_LSERDY)
               break;
           timeout++;
           DJY_DelayUs(1000);
        }
        if(timeout==1000)//外部时钟启动超时选择内部时钟LSI
        {
           RCC->BDCR &=~RCC_BDCR_RTCSEL_Msk;
           RCC->BDCR |=RCC_BDCR_RTCSEL_1;
           HAL_RTCEx_BKUPWrite(&RTC_Handler,BAK_Reg,LSI_Flag_Reg);
        }
        else//选择外部时钟LSE
        {
           RCC->BDCR &=~RCC_BDCR_RTCSEL_Msk;
           RCC->BDCR |=RCC_BDCR_RTCSEL_0;
           HAL_RTCEx_BKUPWrite(&RTC_Handler,BAK_Reg,LSE_Flag_Reg);
        }

        RCC->BDCR |=RCC_BDCR_RTCEN;
        HAL_RTC_Init(&RTC_Handler);
    }

}

// =============================================================================
// 功能：获取RTC时间，获取的时间是S，得到微秒要乘以1000000
// 参数：time,时间，单位为微秒
// 返回：true,成功;false,失败
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    s64 time_s;
    u32 time_us;

    time_s=RTC_GetCounter();
    time_us = ((32767-RTC->DIVL)*1000000)/32768;
    *time = (s64)((1000000 * time_s)+(time_us));
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// 功能：更新RTC时间，RTC里面的时间是S，因此要除以1000000
// 参数：time,时间，单位为微秒
// 返回：true,成功;false,失败
// =============================================================================
bool_t RTC_SetTime(s64 time)
{
    u32 time_s;

    time_s = (u32)(time/1000000);
    RCC->APB1ENR |=RCC_APB1ENR_BKPEN|RCC_APB1ENR_PWREN;
    HAL_PWR_EnableBkUpAccess();//备份区访问使能
    RTC_WaitForSynchro();

    RTC_SetCounter(time_s);
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// 功能：RTC模块初始化，包括注册接口函数和配置RTC
// 参数：para,暂时无用
// 返回：true,成功;false,失败
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    struct timeval tv;
    s64 rtc_time;

    RTC_Configuration();    // 配置RTC

    RTC_GetTime(&rtc_time);

    tv.tv_sec  = rtc_time/1000000;//us ---> s
    tv.tv_usec = rtc_time%1000000;

    settimeofday(&tv,NULL);
    if(!Rtc_RegisterDev(RTC_GetTime,RTC_SetTime))
        return false;
    return true;
}
