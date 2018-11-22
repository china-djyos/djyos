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
// 文件名     ：sysinit.c
// 模块描述: 系统初始化部分，主要是内核启动相关的初始化，如时钟，总线等
// 模块版本:
// 创建人员:
// 创建时间:
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "board-config.h"
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_ll_system.h"

// =============================================================================
// 功能：该函数实现系统时钟的初始化，修改请参照board-config.h
// 中的宏定义
// 参数：无
// 返回：true false
// =============================================================================
  /* This variable is updated in three ways:
      1) by calling CMSIS function SystemCoreClockUpdate()
      2) by calling HAL API function HAL_RCC_GetHCLKFreq()
      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
         Note: If you use this function to configure the system clock; then there
               is no need to call the 2 first functions listed above, since SystemCoreClock
               variable is updated automatically.
  */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

bool_t SysClockInit(void)
{
    bool_t flag =true;
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure;
    RCC_ClkInitTypeDef RCC_ClkInitStructure;

    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别，以便在器件未以最大频率工作

    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;                  //打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=(CN_CFG_HSE/Mhz); //主PLL和音频PLL分频系数(PLL之前的分频)
    RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //主PLL倍频系数(PLL倍频)
    RCC_OscInitStructure.PLL.PLLP=2; //系统时钟的主PLL分频系数(PLL之后的分频)
    RCC_OscInitStructure.PLL.PLLQ=9; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
    if(ret!=HAL_OK)
        flag=false;

    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK)
        flag=false;

    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2分频系数为2

    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);//同时设置FLASH延时周期为7WS，也就是8个CPU周期。
    if(ret!=HAL_OK)
        flag=false;
    LL_FLASH_EnableART();
    return flag;
}

// =============================================================================
// 功能：片外SRAM初始化，包括GPIO和FMC控制器的初始化
// 参数：无
// 返回：无
// =============================================================================
static void SRAM_GpioInit(void)
{
    GPIO_PowerOn(GPIO_D);//使能P-DEFGHI时钟
    GPIO_PowerOn(GPIO_E);
    GPIO_PowerOn(GPIO_F);
    GPIO_PowerOn(GPIO_G);
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);
//初始化总线IO
    GPIO_CfgPinFunc(GPIO_D,
            PIN0|PIN1|PIN4|PIN5|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);         //PC0/2/3

    GPIO_CfgPinFunc(GPIO_E,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);     //PD0/1/8/9/10/14/15

    GPIO_CfgPinFunc(GPIO_F,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);         //PE0/1/7~15

    GPIO_CfgPinFunc(GPIO_G,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN10,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);     //PG0~5/11~15

    GPIO_CfgPinFunc(GPIO_H,
            PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);         //PC0/2/3

    GPIO_CfgPinFunc(GPIO_I,
            PIN0|PIN1|PIN3|PIN6|PIN7|PIN9|PIN10,
            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE); //PF0~2/4/5/8/15
//////////////////////////////////////
    GPIO_AFSet(GPIO_D,
            PIN0|PIN1|PIN4|PIN5|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,AF12);            //PC0/2/3

    GPIO_AFSet(GPIO_E,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,AF12);        //PD0/1/8/9/10/14/15

    GPIO_AFSet(GPIO_F,
                    PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN12|PIN13|PIN14|PIN15,AF12);            //PE0/1/7~15

    GPIO_AFSet(GPIO_G,
            PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN10,AF12);      //PG0~5/11~15

    GPIO_AFSet(GPIO_H,
            PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15,AF12);            //PC0/2/3

    GPIO_AFSet(GPIO_I,
            PIN0|PIN1|PIN3|PIN6|PIN7|PIN9|PIN10,AF12);  //PF0~2/4/5/8/15
//功能I/o


}
static void SRAM_FmcInit(void)
{

    RCC->AHB3ENR|=1<<0;         //使能FMC时钟
      /* Configure and enable Bank1_SRAM3，NE3 */
    //bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。

    FMC_Bank1->BTCR[4]=0X00000000;
    FMC_Bank1->BTCR[5]=0X00000000;
    FMC_Bank1E->BWTR[4]=0X00000000;
    //操作BCR寄存器 使用异步模式,模式A(读写共用一个时序寄存器)
    //BTCR[偶数]:BCR寄存器;BTCR[奇数]:BTR寄存器
    FMC_Bank1->BTCR[4]|=1<<12;//存储器写使能
    FMC_Bank1->BTCR[4]|=1<<4; //存储器数据宽度为16bit
    //操作BTR寄存器         （HCLK=216M/2, 1个HCLK=2*4.629ns
    FMC_Bank1->BTCR[5]|=8<<5; //数据保持时间（DATAST）
    FMC_Bank1->BTCR[5]|=0<<3; //地址保持时间（ADDHLD）未用到
    FMC_Bank1->BTCR[5]|=6<<0; //地址建立时间（ADDSET）为0个HCLK 0ns
    //闪存写时序寄存器
    FMC_Bank1E->BWTR[4]=0x0FFFFFFF;//默认值
    //使能BANK1区域3
    FMC_Bank1->BTCR[4]|=1<<0;
}

//SDRAM初始化
void SRAM_Init(void)
{
//    SRAM_GpioInit();
//    SRAM_FmcInit();

}


