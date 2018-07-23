//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include <stm32f7xx_hal_rcc.h>
#include <stm32f7xx_hal_rtc.h>
#include <stm32f7xx_hal_rtc.h>
#include <stm32f7xx_hal_rtc_ex.h>
#include <stm32f7xx_hal_pwr.h>


// =============================================================================


extern void HAL_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);

// =============================================================================



// =============================================================================
// 功能：进入低功耗前，操作系统会调用该函数，做必须的准备工作
// 参数：
// 返回：
// =============================================================================
__attribute__((weak)) void EntrySleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:		//SLEEP
		break;
	case CN_SLEEP_L1:		//LP SLEEP
		break;
	case CN_SLEEP_L2:		//STOP
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return ;
}

// =============================================================================
// 功能：退出低功耗后，操作系统会调用该函数
// 参数：
// 返回：
// =============================================================================
__attribute__((weak)) void ExitSleepReCall(u32 SleepLevel)
{
	switch(SleepLevel)
	{
	case CN_SLEEP_L0:
		break;
	case CN_SLEEP_L1:
		break;
	case CN_SLEEP_L2:		//back to run mode
		break;
	case CN_SLEEP_L3:
		break;
	case CN_SLEEP_L4:
		break;
	default:
		break;
	}
	return ;
}

ptu32_t EXTI_WakeUpIrq(u8 irqline)
{
	if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13) != RESET)
	{
		__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
	}
	Int_ClearLine(CN_INT_LINE_EXTI15_10);
	return 1;
}

// =============================================================================
// 功能：板件低功耗配置：
//      1、GPIO没用到的，都配置为浮空模拟输入
//      2、低功耗模式下，关闭除LPUART1和LPTIM1的时钟；
//      3、配置进入的低功耗级别为STOP2
// 参数：
// 返回：
// =============================================================================
void SR2711_LowPowerCfg(void)
{
	LP_SetSleepLevel(CN_SLEEP_L1);

}
