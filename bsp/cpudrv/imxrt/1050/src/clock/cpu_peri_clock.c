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
// =============================================================================

// 文件名     ：cpu_peri_clock.c
// 模块描述: 时钟模块根据board_config.h里面的配置进行初始化
// 模块版本: V1.00
// 创建人员: chj
// 创建时间: 05/14.2018
// =============================================================================

#include "fsl_clock.h"
#include "board-config.h"
#include "cpu_peri_clock.h"

#include "project_config.h"    //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip clock"//时钟初始化
//parent                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                     //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                  //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                    //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                   //初始化时机，可选值：early，medium，later, pre-main。
                                    //表示初始化时间，分别是早期、中期、后期
//dependence                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                    //如果依赖多个组件，则依次列出
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                    //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CLOCK == false )
//#warning  " cpu_onchip_clock  组件参数未配置，使用默认配置"
//%$#@target = header              //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CLOCK    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static const clock_arm_pll_config_t armPllConfig = {
    .loopDivider = 100, /* PLL loop divider, Fout = Fin * 50 */
};

static const clock_sys_pll_config_t sysPllConfig = {
    .loopDivider = 1,                         /* PLL loop divider, Fout = Fin * ( 20 + loopDivider*2 + numerator / denominator ) */
    .numerator = 0,                           /* 30 bit numerator of fractional loop divider */
    .denominator = 1,                         /* 30 bit denominator of fractional loop divider */
};

static const clock_usb_pll_config_t usb1PllConfig = {
    .loopDivider = 0, /* PLL loop divider, Fout = Fin * 20 */
};

void ClockDisableAll(void)
{
    CLOCK_DisableClock(kCLOCK_Lpuart1);
    CLOCK_DisableClock(kCLOCK_Lpuart2);
    CLOCK_DisableClock(kCLOCK_Lpuart3);
    CLOCK_DisableClock(kCLOCK_Lpuart4);
    CLOCK_DisableClock(kCLOCK_Lpuart5);
    CLOCK_DisableClock(kCLOCK_Lpuart6);
    CLOCK_DisableClock(kCLOCK_Lpuart7);
    CLOCK_DisableClock(kCLOCK_Lpuart8);

    CLOCK_DisableClock(kCLOCK_Can1);
    CLOCK_DisableClock(kCLOCK_Can1S);
    CLOCK_DisableClock(kCLOCK_Can2);
    CLOCK_DisableClock(kCLOCK_Can2S);

    CLOCK_DisableClock(kCLOCK_Trace);

    CLOCK_DisableClock(kCLOCK_Gpt1);
    CLOCK_DisableClock(kCLOCK_Gpt1S);
    CLOCK_DisableClock(kCLOCK_Gpt2);
    CLOCK_DisableClock(kCLOCK_Gpt2S);

    CLOCK_DisableClock(kCLOCK_Gpio1);
    CLOCK_DisableClock(kCLOCK_Gpio2);
    CLOCK_DisableClock(kCLOCK_Gpio3);
    CLOCK_DisableClock(kCLOCK_Gpio4);
    CLOCK_DisableClock(kCLOCK_Gpio5);

    CLOCK_DisableClock(kCLOCK_Lpspi1);
    CLOCK_DisableClock(kCLOCK_Lpspi2);
    CLOCK_DisableClock(kCLOCK_Lpspi3);
    CLOCK_DisableClock(kCLOCK_Lpspi4);

    CLOCK_DisableClock(kCLOCK_Adc1);
    CLOCK_DisableClock(kCLOCK_Adc2);

    CLOCK_DisableClock(kCLOCK_Enet);

    CLOCK_DisableClock(kCLOCK_Aoi1);
    CLOCK_DisableClock(kCLOCK_Aoi2);

    CLOCK_DisableClock(kCLOCK_Pit);

    CLOCK_DisableClock(kCLOCK_Semc);
    CLOCK_DisableClock(kCLOCK_SemcExsc);

    CLOCK_DisableClock(kCLOCK_Csu);

    CLOCK_DisableClock(kCLOCK_OcramExsc);
    CLOCK_DisableClock(kCLOCK_FlexRam);

    CLOCK_DisableClock(kCLOCK_Csi);

    CLOCK_DisableClock(kCLOCK_Lpi2c1);
    CLOCK_DisableClock(kCLOCK_Lpi2c2);
    CLOCK_DisableClock(kCLOCK_Lpi2c3);
    CLOCK_DisableClock(kCLOCK_Lpi2c4);

    CLOCK_DisableClock(kCLOCK_Ocotp);

    CLOCK_DisableClock(kCLOCK_Xbar1);
    CLOCK_DisableClock(kCLOCK_Xbar2);
    CLOCK_DisableClock(kCLOCK_Xbar3);

    CLOCK_DisableClock(kCLOCK_Ipmux1);
    CLOCK_DisableClock(kCLOCK_Ipmux2);
    CLOCK_DisableClock(kCLOCK_Ipmux3);
    CLOCK_DisableClock(kCLOCK_Ipmux4);

    CLOCK_DisableClock(kCLOCK_Iomuxc);
    CLOCK_DisableClock(kCLOCK_IomuxcGpr);
    CLOCK_DisableClock(kCLOCK_IomuxcSnvs);
    CLOCK_DisableClock(kCLOCK_IomuxcSnvsGpr);

    CLOCK_DisableClock(kCLOCK_Lcd);
    CLOCK_DisableClock(kCLOCK_LcdPixel);

    CLOCK_DisableClock(kCLOCK_Pxp);

    CLOCK_DisableClock(kCLOCK_Ewm0);

    CLOCK_DisableClock(kCLOCK_Wdog1);
    CLOCK_DisableClock(kCLOCK_Wdog2);
    CLOCK_DisableClock(kCLOCK_Wdog3);

    CLOCK_DisableClock(kCLOCK_Acmp1);
    CLOCK_DisableClock(kCLOCK_Acmp2);
    CLOCK_DisableClock(kCLOCK_Acmp3);
    CLOCK_DisableClock(kCLOCK_Acmp4);

    CLOCK_DisableClock(kCLOCK_Bee);
/*
    CLOCK_DisableClock(kCLOCK_SimM7);
    CLOCK_DisableClock(kCLOCK_SimM);
    CLOCK_DisableClock(kCLOCK_SimEms);
    CLOCK_DisableClock(kCLOCK_SimMain);
    CLOCK_DisableClock(kCLOCK_Sim_M_Main);
    CLOCK_DisableClock(kCLOCK_SimPer);
*/
    CLOCK_DisableClock(kCLOCK_Tsc);

    CLOCK_DisableClock(kCLOCK_Pwm1);
    CLOCK_DisableClock(kCLOCK_Pwm2);
    CLOCK_DisableClock(kCLOCK_Pwm3);
    CLOCK_DisableClock(kCLOCK_Pwm4);

    CLOCK_DisableClock(kCLOCK_Enc1);
    CLOCK_DisableClock(kCLOCK_Enc2);
    CLOCK_DisableClock(kCLOCK_Enc3);
    CLOCK_DisableClock(kCLOCK_Enc4);

/*  CLOCK_DisableClock(kCLOCK_Rom);*/

    CLOCK_DisableClock(kCLOCK_Flexio1);
    CLOCK_DisableClock(kCLOCK_Flexio2);
/*
    CLOCK_DisableClock(kCLOCK_FlexSpi);
    CLOCK_DisableClock(kCLOCK_FlexSpiExsc);
*/
    CLOCK_DisableClock(kCLOCK_Dma);

    CLOCK_DisableClock(kCLOCK_Kpp);

    CLOCK_DisableClock(kCLOCK_Aips_tz1);
    CLOCK_DisableClock(kCLOCK_Aips_tz2);
    CLOCK_DisableClock(kCLOCK_Aips_tz3);
    CLOCK_DisableClock(kCLOCK_Aips_tz4);

    CLOCK_DisableClock(kCLOCK_Spdif);

    CLOCK_DisableClock(kCLOCK_Sai1);
    CLOCK_DisableClock(kCLOCK_Sai2);
    CLOCK_DisableClock(kCLOCK_Sai3);

    CLOCK_DisableClock(kCLOCK_SnvsHp);
    CLOCK_DisableClock(kCLOCK_SnvsLp);

    CLOCK_DisableClock(kCLOCK_Usdhc1);
    CLOCK_DisableClock(kCLOCK_Usdhc2);
    CLOCK_DisableClock(kCLOCK_UsbOh3);

    CLOCK_DisableClock(kCLOCK_Dcdc);

    CLOCK_DisableClock(kCLOCK_Trng);

    CLOCK_DisableClock(kCLOCK_Mqs);

    CLOCK_DisableClock(kCLOCK_Dcp);

    CLOCK_DisableClock(kCLOCK_Anadig);

    CLOCK_DisableClock(kCLOCK_Timer1);
    CLOCK_DisableClock(kCLOCK_Timer2);
    CLOCK_DisableClock(kCLOCK_Timer3);
    CLOCK_DisableClock(kCLOCK_Timer4);
}

void ClockSetXtal(void)
{
    if(CN_RTC_XTAL!=0)
        CLOCK_SetRtcXtalFreq(CN_RTC_XTAL);
    if(CN_XTAL!=0)
        CLOCK_SetXtalFreq(CN_XTAL);
}

static void delay_clock(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 6400; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void ClockInit(void)
{
    u32 ahb_podf=0;

    if(CN_XTAL==0)
        return;

    switch(CN_CFG_MCLK)
    {
        case (600*Mhz):
                ahb_podf = 0;
                break;
        case (300*Mhz):
                ahb_podf = 1;
                break;
        case (150*Mhz):
                ahb_podf = 3;
                break;
        case (100*Mhz):
                ahb_podf = 5;
                break;
        default:
            return;
    }

    /* Setting PeriphClk2Mux and PeriphMux to provide stable clock before PLLs are initialed */
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 1); /* Set PERIPH_CLK2 MUX to OSC */
    CLOCK_SetMux(kCLOCK_PeriphMux, 1);     /* Set PERIPH_CLK MUX to PERIPH_CLK2 */

    /* Setting the VDD_SOC to 1.5V. It is necessary to config AHB to 600Mhz. */
    DCDC->REG3 = (DCDC->REG3 & (~DCDC_REG3_TRG_MASK)) | DCDC_REG3_TRG(0x12);
    /* Waiting for DCDC_STS_DC_OK bit is asserted */
    while (DCDC_REG0_STS_DC_OK_MASK != (DCDC_REG0_STS_DC_OK_MASK & DCDC->REG0))
    {
    }

     /* Init ARM PLL. */
    CLOCK_InitArmPll(&armPllConfig);
    /* Init System PLL. */
    CLOCK_InitSysPll(&sysPllConfig);
    /* Init Usb1 PLL. */
    CLOCK_InitUsb1Pll(&usb1PllConfig);

    /* Enbale Audio PLL output. */
    CCM_ANALOG->PLL_AUDIO |= CCM_ANALOG_PLL_AUDIO_ENABLE_MASK;
    /* Enbale Video PLL output. */
    CCM_ANALOG->PLL_VIDEO |= CCM_ANALOG_PLL_VIDEO_ENABLE_MASK;
    /* Enable ENET PLL output. */
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENABLE_MASK;
    CCM_ANALOG->PLL_ENET |= CCM_ANALOG_PLL_ENET_ENET_25M_REF_EN_MASK;
    /* Set periph clock2 clock source. */
    CLOCK_SetMux(kCLOCK_PeriphClk2Mux, 0);
    /* Set PERIPH_CLK2_PODF. */
    CLOCK_SetDiv(kCLOCK_PeriphClk2Div, 0);
    /* Set periph clock source. */
    CLOCK_SetMux(kCLOCK_PeriphMux, 0);
    /* Set AHB_PODF. */
    CLOCK_SetDiv(kCLOCK_AhbDiv, ahb_podf);
    /* Set IPG_PODF. */
    CLOCK_SetDiv(kCLOCK_IpgDiv, 3);
    /* Set ARM_PODF. */
    CLOCK_SetDiv(kCLOCK_ArmDiv, 1);
    /* Set preperiph clock source. */
    CLOCK_SetMux(kCLOCK_PrePeriphMux, 3);
    /* Set PERCLK_PODF. */
    CLOCK_SetDiv(kCLOCK_PerclkDiv, 1);
    /* Set per clock source. */
    CLOCK_SetMux(kCLOCK_PerclkMux, 0);
    /* Set Usdhc1 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc1Mux, 0);
    /* Set USDHC1_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc1Div, 1);
    /* Set Usdhc2 clock source. */
    CLOCK_SetMux(kCLOCK_Usdhc2Mux, 0);
    /* Set USDHC2_PODF. */
    CLOCK_SetDiv(kCLOCK_Usdhc2Div, 1);
    /* Set Semc alt clock source. */
    CLOCK_SetMux(kCLOCK_SemcAltMux, 0);
    /* Set Semc clock source. */
    CLOCK_SetMux(kCLOCK_SemcMux, 0);
    /* Set SEMC_PODF. */
    CLOCK_SetDiv(kCLOCK_SemcDiv, 7);
    /* Set Flexspi clock source. */
    CLOCK_SetMux(kCLOCK_FlexspiMux, 0);
    /* Set FLEXSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_FlexspiDiv, 1);

    /* Set Csi clock source. */
    CLOCK_SetMux(kCLOCK_CsiMux, 0);
    /* Set CSI_PODF. */
    CLOCK_SetDiv(kCLOCK_CsiDiv, 1);
    /* Set Lpspi clock source. */
    CLOCK_SetMux(kCLOCK_LpspiMux, 2);
    /* Set LPSPI_PODF. */
    CLOCK_SetDiv(kCLOCK_LpspiDiv, 4);
    /* Set Trace clock source. */
    CLOCK_SetMux(kCLOCK_TraceMux, 2);
    /* Set TRACE_PODF. */
    CLOCK_SetDiv(kCLOCK_TraceDiv, 2);
    /* Set Sai1 clock source. */
    CLOCK_SetMux(kCLOCK_Sai1Mux, 0);
    /* Set SAI1_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Sai1PreDiv, 3);
    /* Set SAI1_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Sai1Div, 1);
    /* Set Sai2 clock source. */
    CLOCK_SetMux(kCLOCK_Sai2Mux, 0);
    /* Set SAI2_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Sai2PreDiv, 3);
    /* Set SAI2_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Sai2Div, 1);
    /* Set Sai3 clock source. */
    CLOCK_SetMux(kCLOCK_Sai3Mux, 0);
    /* Set SAI3_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Sai3PreDiv, 3);
    /* Set SAI3_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Sai3Div, 1);
    /* Set Lpi2c clock source. */
    CLOCK_SetMux(kCLOCK_Lpi2cMux, 0);
    /* Set LPI2C_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Lpi2cDiv, 0);
    /* Set Can clock source. */
    CLOCK_SetMux(kCLOCK_CanMux, 2);
    /* Set CAN_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_CanDiv, 1);
    /* Set Uart clock source. */
    CLOCK_SetMux(kCLOCK_UartMux, 0);
    /* Set UART_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_UartDiv, 0);
    /* Set Lcdif pre clock source. */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 5);
    /* Set LCDIF_PRED. */
    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 1);
    /* Set LCDIF_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_LcdifDiv, 3);
    /* Set Spdif clock source. */
    CLOCK_SetMux(kCLOCK_SpdifMux, 3);
    /* Set SPDIF0_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Spdif0PreDiv, 1);
    /* Set SPDIF0_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Spdif0Div, 7);
    /* Set Flexio1 clock source. */
    CLOCK_SetMux(kCLOCK_Flexio1Mux, 3);
    /* Set FLEXIO1_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Flexio1PreDiv, 1);
    /* Set FLEXIO1_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Flexio1Div, 7);
    /* Set Flexio2 clock source. */
    CLOCK_SetMux(kCLOCK_Flexio2Mux, 3);
    /* Set FLEXIO2_CLK_PRED. */
    CLOCK_SetDiv(kCLOCK_Flexio2PreDiv, 1);
    /* Set FLEXIO2_CLK_PODF. */
    CLOCK_SetDiv(kCLOCK_Flexio2Div, 7);
    /* Set Pll3 sw clock source. */
    CLOCK_SetMux(kCLOCK_Pll3SwMux, 0);
    /* Set lvds1 clock source. */
    CCM_ANALOG->MISC1 =
        (CCM_ANALOG->MISC1 & (~CCM_ANALOG_MISC1_LVDS1_CLK_SEL_MASK)) | CCM_ANALOG_MISC1_LVDS1_CLK_SEL(0);

    CLOCK_EnableClock(kCLOCK_Iomuxc);           /* iomuxc clock (iomuxc_clk_enable): 0x03u */
    /*ClockDisableAll();*/
   // delay_clock();
}



