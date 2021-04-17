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
//所属模块: CPU相关低功耗功能
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: cpu相关的低功耗管理,实现lowpower.h文件中规定bsp需要提供的函数,在这
//          里实现CPU相关的函数, 跟板件相关的,可到boarddrv目录下去找
//          使用了stm32的42个备份寄存器中最后一个.
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

//stm32上的低功耗Level定义(参考lowpower.h文件中的定义)
//L0:即执行wfe指令,无其他操作.与L1一样,进入的是处理器的睡眠模式.
//L1:wfe指令,内核执行的操作与L0一致,但用户的回调函数可能停掉更多时钟
//L2:即进入stm32的停止模式,保留SRAM和寄存器值,退出后继续运行
//L3/L4:进入stm32的待机模式,SRAM和寄存器值都丢失只有备份寄存器维持供电.区别在于
//退出后,L3进入时要保存SRAM和寄存器,退出时要恢复并从退出处继续执行.

#include "stdint.h"
#include "lowpower.h"
#include "cpu_peri.h"

#include <dbug.h>
#include <stm32h7xx_hal_rcc.h>
#include <stm32h7xx_hal_rtc.h>
#include <stm32h7xx_hal_rtc_ex.h>
#include <stm32h7xx_hal_pwr.h>
#include <stm32h7xx_hal_pwr_ex.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    void ModuleInstall_LowPower (void)
//    ModuleInstall_LowPower();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip peripheral lowpower control"//低功耗组件外设驱动
//parent:"none"                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                       //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                      //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"none"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                      //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL == false )
//#warning  " cpu_onchip_peripheral_lowpower_control  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_PERIPHERAL_LOWPOWER_CONTROL    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,

#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
 extern void __Int_ClearAllLine(void);
//----初始化低功耗硬件--------------------------------------------------------
//功能: 初始化低功耗管理硬件,如果不需要初始化,可以直接return true.因为stm32低功耗
//      管理需要使用备份区保存参数, 故需要打开备份模块电源和使能备份区访问.
//参数: 无
//返回: 无
//----------------------------------------------------------------------------
bool_t __LP_BSP_HardInit(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();//PWR模块使能
    HAL_PWR_EnableBkUpAccess();//后备区使能
    return true;
}

//----读取休眠级别------------------------------------------------------------
//功能: 当个系统从休眠唤醒并重新,调用本函数可以获得唤醒前的休眠状态,返回值是
//      在lowpower.h文件中定义的CN_SLEEP_L3、CN_SLEEP_L4、CN_SLEEP_NORMAL之一,
//      CN_SLEEP_NORMAL表示上电或者硬复位,或者软件指令复位.注意,返回值不会是
//      CN_SLEEP_L0~2之一.
//参数: 无
//返回: CN_SLEEP_L3、CN_SLEEP_L4、CN_SLEEP_NORMAL之一
//----------------------------------------------------------------------------
u32 __LP_BSP_GetSleepLevel(void)
{
    u32 bkt_DR;
    RTC_HandleTypeDef RTC_Handler;  //RTC句柄
    RTC_Handler.Instance=RTC;
    if(__HAL_PWR_GET_FLAG(PWR_FLAG_WU+PWR_FLAG_SB)& PWR_FLAG_WU)
    {
        bkt_DR = HAL_RTCEx_BKUPRead(&RTC_Handler,RTC_BKP_DR0);//todo
//        bkt_DR = Stm32SleepModel4;
        if( (bkt_DR == CN_SLEEP_L3) || (bkt_DR == CN_SLEEP_L4) )
            return bkt_DR;
        else
            return CN_SLEEP_NORMAL;
    }
    else
        return CN_SLEEP_NORMAL;
}

//----保存休眠级别-------------------------------------------------------------
//功能: 把即将进入的休眠级别保存到相应的硬件中,注意,只有L3和L4需要.
//参数: SleepLevel, 即将进入的低功耗级别
//返回true=成功,false=失败
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveSleepLevel(u32 SleepLevel)
{
    RTC_HandleTypeDef RTC_Handler;  //RTC句柄
    RTC_Handler.Instance=RTC;

    if((SleepLevel!= CN_SLEEP_L3) && (SleepLevel!= CN_SLEEP_L4))
        return false;
    HAL_RTCEx_BKUPWrite(&RTC_Handler,RTC_BKP_DR0,SleepLevel);
    return true;

}

//-----------------------------------------------------------------------------
//功能: 进入休眠
//参数: sleep_level,休眠等级
//      pend_ticks, 休眠tick数
//返回: 无意义
//-----------------------------------------------------------------------------
void __LP_BSP_EntrySleep(u8 sleep_level, u32 pend_ticks)
{
    switch(sleep_level)
    {
        case CN_SLEEP_L0:
            //Sleep
        //    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFE);
             __Int_ClearAllLine();
            HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
            break;
        case CN_SLEEP_L1:
            //Mode : Run --> lprun--->lpsleep
            __Int_ClearAllLine();
            HAL_PWREx_EnableFlashPowerDown();
            HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
            break;
        case CN_SLEEP_L2:
            warning_printf("LP", "Entry sleep level_2 undefined");
            break;
        case CN_SLEEP_L3:
            warning_printf("LP", "Entry sleep level_3 undefined");
            break;
        case CN_SLEEP_L4:
            warning_printf("LP", "Entry sleep level_4 undefined");
            break;
    }
}

bool_t __LP_BSP_RestoreRamL3(void)
{
    return true;
}
bool_t __LP_BSP_SaveRamL3(void)
{
   return true;
}

void __LP_BSP_AsmSaveReg(struct ThreadVm *running_vm,bool_t (*SaveRamL3)(void),\
        void (*EntrySleepL3)(void))
{
    return;
}

//-----------------------------------------------------------------------------
//功能: 安装低功耗组件，要把一些低功耗需要使用到的函数，注册到系统中
//参数: 无
//返回: 无意义
//-----------------------------------------------------------------------------
void ModuleInstall_LowPower (void)
{
    Register_LowPower_Function(__LP_BSP_EntrySleep, __LP_BSP_SaveSleepLevel, __LP_BSP_SaveRamL3, __LP_BSP_AsmSaveReg);
}

