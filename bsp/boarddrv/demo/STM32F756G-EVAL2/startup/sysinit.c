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
#include "stm32f7xx_hal_conf.h"
#include "stm32f7xx_ll_system.h"

// =============================================================================
// ���ܣ��ú���ʵ��ϵͳʱ�ӵĳ�ʼ�����޸������board-config.h
// �еĺ궨��
// ��������
// ���أ�true false
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

    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
    //���õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;                  //��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM=(CN_CFG_HSE/Mhz); //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ)
    RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //��PLL��Ƶϵ��(PLL��Ƶ)
    RCC_OscInitStructure.PLL.PLLP=2; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    RCC_OscInitStructure.PLL.PLLQ=9; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
    if(ret!=HAL_OK)
        flag=false;

    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK)
        flag=false;

    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK\
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB��Ƶϵ��
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1��Ƶϵ��
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2��Ƶϵ��

    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);//ͬʱ����FLASH��ʱ����Ϊ7WS��Ҳ����8��CPU���ڡ�
    if(ret!=HAL_OK)
        flag=false;
    LL_FLASH_EnableART();
    return flag;
}

// =============================================================================
// ���ܣ�Ƭ��SRAM��ʼ��������GPIO��FMC�������ĳ�ʼ��
// ��������
// ���أ���
// =============================================================================
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
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
  __IO uint32_t tmpmrd =0;
  /* Step 1:  Configure a clock configuration enable command */
  Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 2: Insert 100 us minimum delay */
  extern void HAL_Delay(__IO uint32_t Delay);
  HAL_Delay(1);

  /* Step 3: Configure a PALL (precharge all) command */
  Command->CommandMode = FMC_SDRAM_CMD_PALL;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 4 : Configure a Auto-Refresh command */
  Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 8;
  Command->ModeRegisterDefinition = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 5: Program the external memory mode register */
  tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          |
                     SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
                     SDRAM_MODEREG_CAS_LATENCY_3           |
                     SDRAM_MODEREG_OPERATING_MODE_STANDARD |
                     SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
  Command->CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
  Command->AutoRefreshNumber = 1;
  Command->ModeRegisterDefinition = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(hsdram, Command, 0x1000);

  /* Step 6: Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  hsdram->Instance->SDRTR |= ((uint32_t)((1292)<< 1));

}
void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{

  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_FMC_CLK_ENABLE();

  u32 Pin   = PIN0|PIN1|PIN8| PIN9|PIN10|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_D,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_D,Pin,AF12);

  Pin   = PIN0|PIN1|PIN7| PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14 |PIN15;
  GPIO_CfgPinFunc(GPIO_E,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_E,Pin,AF12);

  Pin   = PIN0|PIN1|PIN2| PIN3|PIN4|PIN5|PIN11|PIN12|PIN13|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_F,Pin,AF12);

  Pin   = PIN0|PIN1|PIN4| PIN5|PIN8|PIN15;
  GPIO_CfgPinFunc(GPIO_G,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_G,Pin,AF12);

  Pin   = PIN2|PIN3|PIN5|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_H,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_H,Pin,AF12);

  Pin   = PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN9|PIN10;
  GPIO_CfgPinFunc(GPIO_I,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_I,Pin,AF12);

}



void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
   __HAL_RCC_FMC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();

  /* GPIOD configuration */
  u32 Pin   = PIN0|PIN1|PIN3| PIN4|PIN5|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_D,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_D,Pin,AF12);

  /* GPIOE configuration */
  Pin   = PIN0|PIN1|PIN3| PIN4|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_E,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_E,Pin,AF12);

  /* GPIOF configuration */
  Pin   = PIN0|PIN1|PIN2| PIN3|PIN4|PIN5|PIN12|PIN13|PIN14|PIN15;
  GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_F,Pin,AF12);

  /* GPIOG configuration */
  Pin   = PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN10;
  GPIO_CfgPinFunc(GPIO_G,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE);
  GPIO_AFSet(GPIO_G,Pin,AF12);

}

//SDRAM��ʼ��
bool_t SRAM_Init(void)
{
    bool_t flag;
    SDRAM_HandleTypeDef      hsdram;
    FMC_SDRAM_TimingTypeDef  SDRAM_Timing;
    FMC_SDRAM_CommandTypeDef command;

    hsdram.Instance = FMC_SDRAM_DEVICE;
    SDRAM_Timing.LoadToActiveDelay    = 2;
    SDRAM_Timing.ExitSelfRefreshDelay = 6;
    SDRAM_Timing.SelfRefreshTime      = 4;
    SDRAM_Timing.RowCycleDelay        = 6;
    SDRAM_Timing.WriteRecoveryTime    = 2;
    SDRAM_Timing.RPDelay              = 2;
    SDRAM_Timing.RCDDelay             = 2;

    hsdram.State = HAL_SRAM_STATE_RESET;
    hsdram.Init.SDBank             = FMC_SDRAM_BANK1;
    hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_9;
    hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
    hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_32;
    hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
    hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
    hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_ENABLE;
    hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_0;
    if(HAL_SDRAM_Init(&hsdram, &SDRAM_Timing) != HAL_OK)
        flag=false;
    BSP_SDRAM_Initialization_Sequence(&hsdram, &command);
    return flag;
}


