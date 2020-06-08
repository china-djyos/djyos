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

// 文件名     ：cpu_peri_rtc.c
// 模块描述: RTC模块，当系统走时时间配置为RTC时，通过RTC秒中断，更新系统时间，使系统
//           走时时基在断电情况下依然正常运行
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 18/08.2014
// =============================================================================
#include "cpu_peri.h"
#include "time.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   ModuleInstall_RTC(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip rtc"//CPU的rtc外设驱动
//parent:"rtc"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"time","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
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
// =============================================================================
// 功能：RTC配置函数，首先完成RTC模块复位，然后使能振荡器，并配置相关寄存器
// 参数：无
// 返回：无
// =============================================================================
static void RTC_Configuration(void)
{
    // 使能秒中断
    LPC_RTC->ILR |= RTC_ILR_RTCCIF_MASK;
    LPC_RTC->CIIR |= RTC_CIIR_IMSEC_MASK;
    // 时钟使能
    LPC_RTC->CCR |= RTC_CCR_CLKEN_MASK;
}

// =============================================================================
// 功能：RTC实时时钟更新，将RTC模块中的时间更新为最新时间，由其他模块调用
// 参数：DateTime,系统时间，需换算成秒
// 返回：1
// =============================================================================
bool_t RTC_SetTime(s64 time)
{
    bool_t result = false;
    struct tm *DateTime;

    DateTime = Time_GmTime(&time);
    if(DateTime != NULL)
    {
        LPC_RTC->SEC = DateTime->tm_sec;
        LPC_RTC->MIN = DateTime->tm_min;
        LPC_RTC->HOUR = DateTime->tm_hour;
        LPC_RTC->DOM = DateTime->tm_mday;
        LPC_RTC->MONTH  = DateTime->tm_mon;
        LPC_RTC->YEAR = DateTime->tm_year;
        result = true;
    }

    return result;
}

// =============================================================================
// 功能：RTC实时时钟读取，读取RTC时间，并更新系统时间
// 参数：DateTime,系统时间，需换算成秒
// 返回：1
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    struct tm DateTime;

    DateTime.tm_us   = 0;
    DateTime.tm_sec = LPC_RTC->SEC;
    DateTime.tm_min = LPC_RTC->MIN;
    DateTime.tm_hour = LPC_RTC->HOUR;
    DateTime.tm_mday = LPC_RTC->DOM;
    DateTime.tm_mon  = LPC_RTC->MONTH;
    DateTime.tm_year = LPC_RTC->YEAR;

    *time = 1000000 * Time_MkTime(&DateTime);
    return true;
}

// =============================================================================
// 功能：初始化RTC实时时钟模块。
// 参数：模块初始化函数没有参数
// 返回：true = 成功初始化，false = 初始化失败
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    // 使能RTC时钟，即提供系统时钟
    LPC_SC->PCONP |= (1<<9);

    //  RTC振荡器失效检测标志
    if(LPC_RTC->RTC_AUX & RTC_AUX_OSCF_MASK)
    {
        LPC_RTC->RTC_AUX |= RTC_AUX_OSCF_MASK;//清除标志位
    }

    // 判断是否时钟有效，即判断是否需要初始化时钟
    // 一般是电池掉电或主动复位后需要重新初始化
    if(!(LPC_RTC->CCR & RTC_CCR_CLKEN_MASK))
    {
        RTC_Configuration();
    }

    Rtc_RegisterDev(RTC_GetTime,RTC_SetTime);

    return true;
}

