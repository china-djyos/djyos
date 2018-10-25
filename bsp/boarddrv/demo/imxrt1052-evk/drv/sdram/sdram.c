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

// �ļ���     ��cpu_peri_semc.c
// ģ������: MIMXRT1052��semc����
// ģ��汾:
// ������Ա:    chj
// ����ʱ��: 5/4.2018
// =============================================================================
#include "sdram.h"
#include "fsl_iomuxc.h"
#include "fsl_semc.h"
#include "fsl_clock.h"
#include "board-config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_SEMC SEMC
#define EXAMPLE_SEMC_START_ADDRESS (0x80000000U)
#define EXAMPLE_SEMC_CLK_FREQ CLOCK_GetFreq(kCLOCK_SemcClk)

void SemcPinInit(void)
{
    IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_00_SEMC_DATA00,         /* GPIO_EMC_00 is configured as SEMC_DATA00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_01_SEMC_DATA01,         /* GPIO_EMC_01 is configured as SEMC_DATA01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_02_SEMC_DATA02,         /* GPIO_EMC_02 is configured as SEMC_DATA02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_03_SEMC_DATA03,         /* GPIO_EMC_03 is configured as SEMC_DATA03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_04_SEMC_DATA04,         /* GPIO_EMC_04 is configured as SEMC_DATA04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_05_SEMC_DATA05,         /* GPIO_EMC_05 is configured as SEMC_DATA05 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_06_SEMC_DATA06,         /* GPIO_EMC_06 is configured as SEMC_DATA06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_07_SEMC_DATA07,         /* GPIO_EMC_07 is configured as SEMC_DATA07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_08_SEMC_DM00,           /* GPIO_EMC_08 is configured as SEMC_DM00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_09_SEMC_ADDR00,         /* GPIO_EMC_09 is configured as SEMC_ADDR00 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_10_SEMC_ADDR01,         /* GPIO_EMC_10 is configured as SEMC_ADDR01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_11_SEMC_ADDR02,         /* GPIO_EMC_11 is configured as SEMC_ADDR02 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_12_SEMC_ADDR03,         /* GPIO_EMC_12 is configured as SEMC_ADDR03 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_13_SEMC_ADDR04,         /* GPIO_EMC_13 is configured as SEMC_ADDR04 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_14_SEMC_ADDR05,         /* GPIO_EMC_14 is configured as SEMC_ADDR05 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_15_SEMC_ADDR06,         /* GPIO_EMC_15 is configured as SEMC_ADDR06 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_16_SEMC_ADDR07,         /* GPIO_EMC_16 is configured as SEMC_ADDR07 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_17_SEMC_ADDR08,         /* GPIO_EMC_17 is configured as SEMC_ADDR08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_18_SEMC_ADDR09,         /* GPIO_EMC_18 is configured as SEMC_ADDR09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_19_SEMC_ADDR11,         /* GPIO_EMC_19 is configured as SEMC_ADDR11 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_20_SEMC_ADDR12,         /* GPIO_EMC_20 is configured as SEMC_ADDR12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_21_SEMC_BA0,            /* GPIO_EMC_21 is configured as SEMC_BA0 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_22_SEMC_BA1,            /* GPIO_EMC_22 is configured as SEMC_BA1 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_23_SEMC_ADDR10,         /* GPIO_EMC_23 is configured as SEMC_ADDR10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_24_SEMC_CAS,            /* GPIO_EMC_24 is configured as SEMC_CAS */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_25_SEMC_RAS,            /* GPIO_EMC_25 is configured as SEMC_RAS */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_26_SEMC_CLK,            /* GPIO_EMC_26 is configured as SEMC_CLK */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_27_SEMC_CKE,            /* GPIO_EMC_27 is configured as SEMC_CKE */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_28_SEMC_WE,             /* GPIO_EMC_28 is configured as SEMC_WE */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_29_SEMC_CS0,            /* GPIO_EMC_29 is configured as SEMC_CS0 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_30_SEMC_DATA08,         /* GPIO_EMC_30 is configured as SEMC_DATA08 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_31_SEMC_DATA09,         /* GPIO_EMC_31 is configured as SEMC_DATA09 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_32_SEMC_DATA10,         /* GPIO_EMC_32 is configured as SEMC_DATA10 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_33_SEMC_DATA11,         /* GPIO_EMC_33 is configured as SEMC_DATA11 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_34_SEMC_DATA12,         /* GPIO_EMC_34 is configured as SEMC_DATA12 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_35_SEMC_DATA13,         /* GPIO_EMC_35 is configured as SEMC_DATA13 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_36_SEMC_DATA14,         /* GPIO_EMC_36 is configured as SEMC_DATA14 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_37_SEMC_DATA15,         /* GPIO_EMC_37 is configured as SEMC_DATA15 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_38_SEMC_DM01,           /* GPIO_EMC_38 is configured as SEMC_DM01 */
      0U);                                    /* Software Input On Field: Input Path is determined by functionality */
  IOMUXC_SetPinMux(
      IOMUXC_GPIO_EMC_39_SEMC_DQS,            /* GPIO_EMC_39 is configured as SEMC_DQS */
      1U);                                    /* Software Input On Field: Force input path of pad GPIO_EMC_39 */

//  IOMUXC_SetPinMux(
//    IOMUXC_GPIO_EMC_40_SEMC_RDY,            /* GPIO_EMC_40 is configured as SEMC_RDY */
//    0U);                                    /* Software Input On Field: Input Path is determined by functionality */
//  IOMUXC_SetPinMux(
//    IOMUXC_GPIO_EMC_41_SEMC_CSX00,          /* GPIO_EMC_41 is configured as SEMC_CSX00 */
//    0U);                                    /* Software Input On Field: Input Path is determined by functionality */

  IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_00_SEMC_DATA00,         /* GPIO_EMC_00 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_01_SEMC_DATA01,         /* GPIO_EMC_01 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_02_SEMC_DATA02,         /* GPIO_EMC_02 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_03_SEMC_DATA03,         /* GPIO_EMC_03 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_04_SEMC_DATA04,         /* GPIO_EMC_04 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_05_SEMC_DATA05,         /* GPIO_EMC_05 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_06_SEMC_DATA06,         /* GPIO_EMC_06 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_07_SEMC_DATA07,         /* GPIO_EMC_07 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_08_SEMC_DM00,           /* GPIO_EMC_08 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_09_SEMC_ADDR00,         /* GPIO_EMC_09 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_10_SEMC_ADDR01,         /* GPIO_EMC_10 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_11_SEMC_ADDR02,         /* GPIO_EMC_11 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_12_SEMC_ADDR03,         /* GPIO_EMC_12 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_13_SEMC_ADDR04,         /* GPIO_EMC_13 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_14_SEMC_ADDR05,         /* GPIO_EMC_14 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_15_SEMC_ADDR06,         /* GPIO_EMC_15 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_16_SEMC_ADDR07,         /* GPIO_EMC_16 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_17_SEMC_ADDR08,         /* GPIO_EMC_17 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_18_SEMC_ADDR09,         /* GPIO_EMC_18 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_19_SEMC_ADDR11,         /* GPIO_EMC_19 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_20_SEMC_ADDR12,         /* GPIO_EMC_20 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_21_SEMC_BA0,            /* GPIO_EMC_21 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_22_SEMC_BA1,            /* GPIO_EMC_22 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_23_SEMC_ADDR10,         /* GPIO_EMC_23 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_24_SEMC_CAS,            /* GPIO_EMC_24 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_25_SEMC_RAS,            /* GPIO_EMC_25 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_26_SEMC_CLK,            /* GPIO_EMC_26 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_27_SEMC_CKE,            /* GPIO_EMC_27 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_28_SEMC_WE,             /* GPIO_EMC_28 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_29_SEMC_CS0,            /* GPIO_EMC_29 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_30_SEMC_DATA08,         /* GPIO_EMC_30 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_31_SEMC_DATA09,         /* GPIO_EMC_31 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_32_SEMC_DATA10,         /* GPIO_EMC_32 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_33_SEMC_DATA11,         /* GPIO_EMC_33 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_34_SEMC_DATA12,         /* GPIO_EMC_34 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_35_SEMC_DATA13,         /* GPIO_EMC_35 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_36_SEMC_DATA14,         /* GPIO_EMC_36 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_37_SEMC_DATA15,         /* GPIO_EMC_37 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_38_SEMC_DM01,           /* GPIO_EMC_38 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
    IOMUXC_SetPinConfig(
        IOMUXC_GPIO_EMC_39_SEMC_DQS,            /* GPIO_EMC_39 PAD functional properties : */
        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
                                                   Drive Strength Field: R0/7
                                                   Speed Field: max(200MHz)
                                                   Open Drain Enable Field: Open Drain Disabled
                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
                                                   Pull / Keep Select Field: Keeper
                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
                                                   Hyst. Enable Field: Hysteresis Enabled */
//    IOMUXC_SetPinConfig(
//        IOMUXC_GPIO_EMC_40_SEMC_RDY,            /* GPIO_EMC_40 PAD functional properties : */
//        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
//                                                   Drive Strength Field: R0/7
//                                                   Speed Field: max(200MHz)
//                                                   Open Drain Enable Field: Open Drain Disabled
//                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
//                                                   Pull / Keep Select Field: Keeper
//                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
//                                                   Hyst. Enable Field: Hysteresis Enabled */
//    IOMUXC_SetPinConfig(
//        IOMUXC_GPIO_EMC_41_SEMC_CSX00,          /* GPIO_EMC_41 PAD functional properties : */
//        0x0110F9u);                             /* Slew Rate Field: Fast Slew Rate
//                                                   Drive Strength Field: R0/7
//                                                   Speed Field: max(200MHz)
//                                                   Open Drain Enable Field: Open Drain Disabled
//                                                   Pull / Keep Enable Field: Pull/Keeper Enabled
//                                                   Pull / Keep Select Field: Keeper
//                                                   Pull Up / Down Config. Field: 100K Ohm Pull Down
//                                                   Hyst. Enable Field: Hysteresis Enabled */
}

bool_t Sdram_SemcInit(void)
{
    status_t status;
    semc_config_t config;
    semc_sdram_config_t sdramconfig;
    //uint32_t clockFrq = EXAMPLE_SEMC_CLK_FREQ;
    uint32_t clockFrq = CN_XTAL;

    /* Initializes the MAC configure structure to zero. */
    memset(&config, 0, sizeof(semc_config_t));
    memset(&sdramconfig, 0, sizeof(semc_sdram_config_t));

    SemcPinInit();
    /* Set semc clock to 132Mhz */
    CLOCK_SetMux(kCLOCK_SemcMux, 1);
    CLOCK_SetDiv(kCLOCK_SemcDiv, 2);

    /* Initialize SEMC. */
    SEMC_GetDefaultConfig(&config);
    SEMC_Init(SEMC, &config);

    /* Configure SDRAM. */
    sdramconfig.csxPinMux = kSEMC_MUXCSX0;
    sdramconfig.address = 0x80000000;
    sdramconfig.memsize_kbytes = CN_SDRAM_SIZE * 1024; /* 32MB = 32*1024*1KBytes*/
    sdramconfig.portSize = kSEMC_PortSize16Bit;
    sdramconfig.burstLen = kSEMC_Sdram_BurstLen8;
    sdramconfig.columnAddrBitNum = kSEMC_SdramColunm_9bit;
    sdramconfig.casLatency = kSEMC_LatencyThree;
    sdramconfig.tPrecharge2Act_Ns = 18;   /* Trp 18ns */
    sdramconfig.tAct2ReadWrite_Ns = 18;   /* Trcd 18ns */
    sdramconfig.tRefreshRecovery_Ns = 67; /* Use the maximum of the (Trfc , Txsr). */
    sdramconfig.tWriteRecovery_Ns = 12;   /* 12ns */
    sdramconfig.tCkeOff_Ns = (1000000000 / clockFrq);
    sdramconfig.tAct2Prechage_Ns = 42; /* Tras 42ns */
    sdramconfig.tSelfRefRecovery_Ns = 67;
    sdramconfig.tRefresh2Refresh_Ns = 60;
    sdramconfig.tAct2Act_Ns = 60;
    sdramconfig.tPrescalePeriod_Ns = 160 * (1000000000 / clockFrq);
    sdramconfig.refreshPeriod_nsPerRow = 64 * 1000000 / 8192; /* 64ms/8192 */
    sdramconfig.refreshUrgThreshold = sdramconfig.refreshPeriod_nsPerRow;
    sdramconfig.refreshBurstLen = 1;
    status=SEMC_ConfigureSDRAM(SEMC, kSEMC_SDRAM_CS0, &sdramconfig, clockFrq);
    if(status==kStatus_Success)
        return true;
    return false;
}
