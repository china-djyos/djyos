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
// �ļ���     ��sysinit.c
// ģ������: ϵͳ��ʼ�����֣���Ҫ���ں�������صĳ�ʼ������ʱ�ӣ����ߵ�
// ģ��汾:
// ������Ա:
// ����ʱ��:
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "board-config.h"

// =============================================================================
// ���ܣ��ú���ʵ��ϵͳʱ�ӵĳ�ʼ�����޸������board-config.h
// �еĺ궨��
// ��������
// ���أ�true false
// =============================================================================
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 400 000 000 (CPU Clock)
  *            HCLK(Hz)                       = 200 000 000 (AXI and AHBs Clock)
  *            AHB Prescaler                  = 2
  *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
  *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
  *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
  *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 5
  *            PLL_N                          = 160
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            PLL_R                          = 2
  *            VDD(V)                         = 3.3
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */

uint32_t SystemD2Clock = 64000000;
const  uint8_t D1CorePrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
bool_t SysClockInit(void)
{
      RCC_ClkInitTypeDef RCC_ClkInitStruct;
      RCC_OscInitTypeDef RCC_OscInitStruct;
      HAL_StatusTypeDef ret = HAL_OK;

      /*!< Supply configuration update enable */
      MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0);

      /* The voltage scaling allows optimizing the power consumption when the device is
         clocked below the maximum system frequency, to update the voltage scaling value
         regarding system frequency refer to product datasheet.  */
      __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

      while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) {}

      /* Enable HSE Oscillator and activate PLL with HSE as source */
      RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
      RCC_OscInitStruct.HSEState = RCC_HSE_ON;
      RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
      RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
      RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
      RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

      RCC_OscInitStruct.PLL.PLLM = 5;
      RCC_OscInitStruct.PLL.PLLN = 2*(CN_CFG_MCLK/5000000);
      RCC_OscInitStruct.PLL.PLLP = 2;
      RCC_OscInitStruct.PLL.PLLR = 2;
      RCC_OscInitStruct.PLL.PLLQ = 4;

      RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
      RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
      ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
      if(ret != HAL_OK)
          while(1);

      /* Select PLL as system clock source and configure  bus clocks dividers */
      RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_D1PCLK1|RCC_CLOCKTYPE_PCLK1|\
        RCC_CLOCKTYPE_PCLK2 |RCC_CLOCKTYPE_D3PCLK1);
      RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
      RCC_ClkInitStruct.SYSCLKDivider  = RCC_SYSCLK_DIV1;
      RCC_ClkInitStruct.AHBCLKDivider  = RCC_HCLK_DIV2;
      RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
      RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
      RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
      RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
      ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4);
      if(ret != HAL_OK)
          while(1);
      return true;
}

// =============================================================================
// ���ܣ�Ƭ��SRAM��ʼ��������GPIO��FMC�������ĳ�ʼ��
// ��������
// ���أ���
// =============================================================================
#define SDRAM_TIMEOUT                    ((uint32_t)0xFFFF)
#define REFRESH_COUNT                    ((uint32_t)0x0603)   /* SDRAM refresh counter */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


static void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd =0;
  /* Step 1:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 2: Insert 100 us minimum delay */
  /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 4 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 5: Program the external memory mode register */



  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK2;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

  /* Step 6: Set the refresh rate counter */
  /* Set the device refresh rate */
  HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);

}
void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
  GPIO_InitTypeDef gpio_init_structure;

  __HAL_RCC_FMC_CLK_ENABLE();
  __HAL_RCC_MDMA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  gpio_init_structure.Mode      = GPIO_MODE_AF_PP;
  gpio_init_structure.Pull      = GPIO_PULLUP;
  gpio_init_structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init_structure.Alternate = GPIO_AF12_FMC;

  gpio_init_structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3| GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6 |\
                              GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 |\
                              GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpio_init_structure);

  gpio_init_structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3| GPIO_PIN_4|GPIO_PIN_7     |\
                              GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12 |\
                              GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpio_init_structure);
  gpio_init_structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3|GPIO_PIN_4     |\
                              GPIO_PIN_5|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpio_init_structure);
  gpio_init_structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|\
                             GPIO_PIN_6 |GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14;
  HAL_GPIO_Init(GPIOG, &gpio_init_structure);
  gpio_init_structure.Pin   =  GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10| GPIO_PIN_11|\
                              GPIO_PIN_12| GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOH, &gpio_init_structure);
  gpio_init_structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3|
                                GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7 |
                              GPIO_PIN_9|GPIO_PIN_10;
  HAL_GPIO_Init(GPIOI, &gpio_init_structure);
  gpio_init_structure.Pin   = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOB, &gpio_init_structure);

}
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
  GPIO_InitTypeDef  GPIO_Init_Structure;
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_FMC_CLK_ENABLE();

  GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init_Structure.Pull      = GPIO_PULLUP;
  GPIO_Init_Structure.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_Init_Structure.Alternate = GPIO_AF12_FMC;

  GPIO_Init_Structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_8| GPIO_PIN_9|GPIO_PIN_10 |\
                              GPIO_PIN_14|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_7| GPIO_PIN_8|GPIO_PIN_9 |\
                              GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2| GPIO_PIN_3|GPIO_PIN_4 |\
                              GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 |\
                              GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15;
  HAL_GPIO_Init(GPIOG, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pin   = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 |\
                              GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14 |\
                              GPIO_PIN_15;
  HAL_GPIO_Init(GPIOH, &GPIO_Init_Structure);
  GPIO_Init_Structure.Pin   = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4 |\
                              GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10;
  HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);

}

//SDRAM��ʼ��0xd0000000 8M*32bit
bool_t SDRAM_Init(void)
{
    SDRAM_HandleTypeDef      hsdram;
    FMC_SDRAM_TimingTypeDef  SDRAM_Timing;
    FMC_SDRAM_CommandTypeDef command;
    /*##-1- Configure the SDRAM device #########################################*/
    /* SDRAM device configuration */
    hsdram.Instance = FMC_SDRAM_DEVICE;

      /* Timing configuration for 100Mhz as SDRAM clock frequency (System clock is up to 200Mhz) */
    SDRAM_Timing.LoadToActiveDelay    = 2;
    SDRAM_Timing.ExitSelfRefreshDelay = 7;
    SDRAM_Timing.SelfRefreshTime      = 4;
    SDRAM_Timing.RowCycleDelay        = 7;
    SDRAM_Timing.WriteRecoveryTime    = 2;
    SDRAM_Timing.RPDelay              = 2;
    SDRAM_Timing.RCDDelay             = 2;

    hsdram.Init.SDBank             = FMC_SDRAM_BANK2;
    hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
    hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
    hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;
    hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
    hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
    hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
    hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;
    HAL_SDRAM_MspInit(&hsdram);
    /* Initialize the SDRAM controller */
    if(HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK)
        return false;
    SDRAM_Initialization_Sequence(&hsdram, &command);
    return true;
}
//0x68000000 1M*16bit
bool_t SRAM_Init(void)
{
    SRAM_HandleTypeDef hsram;
    FMC_NORSRAM_TimingTypeDef SRAM_Timing;
    /*##-1- Configure the SRAM device ##########################################*/
    /* SRAM device configuration */

    hsram.Instance  = FMC_NORSRAM_DEVICE;
    hsram.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;

    /* SRAM device configuration */
    SRAM_Timing.AddressSetupTime       = 4;
    SRAM_Timing.AddressHoldTime        = 1;
    SRAM_Timing.DataSetupTime          = 2;
    SRAM_Timing.BusTurnAroundDuration  = 1;
    SRAM_Timing.CLKDivision            = 2;
    SRAM_Timing.DataLatency            = 2;
    SRAM_Timing.AccessMode             = FMC_ACCESS_MODE_A;

    hsram.Init.NSBank             = FMC_NORSRAM_BANK3;
    hsram.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init.MemoryType         = FMC_MEMORY_TYPE_SRAM;
    hsram.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
    hsram.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
    hsram.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
    hsram.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
    hsram.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;

    /* Initialize the SRAM controller */
    if(HAL_SRAM_Init(&hsram, &SRAM_Timing, &SRAM_Timing) != HAL_OK)
         return false;
    return true;
}









