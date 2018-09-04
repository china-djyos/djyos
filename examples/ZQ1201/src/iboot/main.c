
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
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "stddef.h"
#include "systime.h"
#include <Timer_hard.h>

#include <silan_syscfg.h>
#include <silan_iomux.h>

#define CTRL_LED1               IO_CONFIG_PB8
#define CTRL_LED2               IO_CONFIG_PA6

//----LED_init-----------------------------------------------------------------
//功能：使能gpio端口设置引脚模式
//参数：void
//返回：void
//-----------------------------------------------------------------------------
void Led_Init(void)
{
    silan_io_func_config(CTRL_LED2, IO_FUNC_GPIO);
    silan_io_output(CTRL_LED2);
}

void Led_Goggle(void)
{
    silan_io_toggle(CTRL_LED2);
}

#include <uart.h>

void uartTest(void)
{

//    char *str =  "hello world\r\n";
    //ModuleInstall_UART(CN_UART1);                               //3
    //uart_dev = Driver_OpenDevice("UART1",D_RDWR,0);
//    DevOpen("UART1",D_RDWR,);
//    Driver_CtrlDevice(uart_dev, CN_UART_SET_BAUD,115200,0);    //6
//    Driver_CtrlDevice(uart_dev, CN_UART_START,0,0);
//
//    Driver_WriteDevice(uart_dev,(ptu32_t)str,strlen(str),0,CN_BLOCK_BUFFER,10*mS);
}
static u8 iCount = 0;
u32 timerTest(ptu32_t i)
{

    Led_Goggle();
    printf("app timer1 interrupt %d\r\n",iCount++);
    return 0;
}

u32 timerTest1(ptu32_t i)
{

    Led_Goggle();
    printf("app timer2 interrrupt %d\r\n",iCount++);
    return 0;
}

void testFortimer(void)
{
    static u8 byCount = 0;
    ptu32_t TimerHandle = HardTimer_Alloc(timerTest);
    u32 CrystalFreq = HardTimer_GetFreq(TimerHandle);          //获取定时器频率
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,CrystalFreq*10); //设置定时周期,待设置的周期（计数值）,1s
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETRELOAD,true);   //reload模式
    HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);       //中断使能
//  HardTimer_Ctrl(TimerHandle,EN_TIMER_SETINTPRO,true);   //中断属性设置,true代表实时信号
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);     //使能计数
    printf("run timer1 usrisr \r\n");

    ptu32_t TimerHandle1 = HardTimer_Alloc(timerTest1);
    CrystalFreq = HardTimer_GetFreq(TimerHandle1);          //获取定时器频率
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETCYCLE,CrystalFreq*3); //设置定时周期,待设置的周期（计数值）,1s
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETRELOAD,true);   //reload模式
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_ENINT,true);       //中断使能
    //  HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETINTPRO,true);   //中断属性设置,true代表实时信号
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_STARTCOUNT,0);     //使能计数
    struct CPUTimerHandle*q = (struct CPUTimerHandle*)TimerHandle1;
    printf("run timer2 usrisr \r\n");
}
#include "cpu_peri_wat.h"
//test for watchDog
void testWatDog(void)
{
    WatDog_Install();//安装看门狗
    printf("wat init finish\r\n");
    //启动看门狗
    WDT_Start();
    printf("start wat dog finish\r\n");
}

//test for ad
#include "cpu_peri_adc.h"
void testADC(void)
{

     ModuleInstall_ADC(EN_ADC_VREF_EXT33);
    //配置引脚
    appADC_Ctrl(0,CN_ADC_CHANNEL_PINCFG, CN_AD_CFG_PA3,0);
    //配置读通道
    appADC_Ctrl(0,CN_ADC_CHANNEL_SET, CN_AD_CHANNEL3,0);
}

void get_AD_Dat()
{
    u32 dat;
    appADC_Ctrl(0,CN_ADC_CHANNEL_RD_DAT, CN_AD_CHANNEL3,(u32)&dat);
    printf("AD:%d\r\n",dat);
}
extern u32 isr_vector[];
volatile u32 *p = (volatile u32 *)isr_vector;
ptu32_t djy_main(void)
{
      Led_Init();
    extern bool_t ModuleInstall_HardTimer(void);
    testFortimer();
    testWatDog();
    testADC();
    printf("%x\r\n",*p);
	while(1)
	{
	    Led_Goggle();
	    get_AD_Dat();
		printf("hello world!\r\n");
		Djy_EventDelay(100*mS);
	}
	return 0;
}
