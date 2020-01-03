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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================
// =============================================================================

// �ļ���     ��cpu_peri_clock.c
// ģ������: ʱ��ģ�����board_config.h��������ý��г�ʼ��
// ģ��汾: V1.00
// ������Ա: chj
// ����ʱ��: 05/14.2018
// =============================================================================

#include "fsl_clock.h"
#include "board-config.h"
#include "cpu_peri_clock.h"

#include "project_config.h"    //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip clock"//ʱ�ӳ�ʼ��
//parent                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                     //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                  //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                    //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                    //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence                   //������������������������none����ʾ�������������
                                    //����������������������г�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                    //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_CLOCK == false )
//#warning  " cpu_onchip_clock  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_CLOCK    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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



