//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

#if !defined  (OSCILLATORTYPE)
#define OSCILLATORTYPE   OSCILLATORTYPE_HSE
#endif
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

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    if (OSCILLATORTYPE == OSCILLATORTYPE_HSE )
    {
        RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
        RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
        RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;                  //��PLL
        RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLLʱ��Դѡ��HSE
        RCC_OscInitStructure.PLL.PLLM=(CN_CFG_HSE/Mhz); //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ)
        if(CN_CFG_MCLK == 216*Mhz)
        {
            RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //��PLL��Ƶϵ��(PLL��Ƶ)
            RCC_OscInitStructure.PLL.PLLP=2; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
        }
        else if(CN_CFG_MCLK == 108*Mhz)
        {
            RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*4); //��PLL��Ƶϵ��(PLL��Ƶ)
            RCC_OscInitStructure.PLL.PLLP=4; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
        }
        else
        {
            while(1); // ϵͳʱ�����ò�֧��
        }

        RCC_OscInitStructure.PLL.PLLQ=9; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
        ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
        if(ret!=HAL_OK)
            flag=false;
    }
    else
    {
          /**Initializes the CPU, AHB and APB busses clocks
          */
        RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        RCC_OscInitStructure.LSEState = RCC_LSE_BYPASS;
        RCC_OscInitStructure.HSIState = RCC_HSI_ON;
        RCC_OscInitStructure.HSICalibrationValue = 16;
        RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSI;
        RCC_OscInitStructure.PLL.PLLM = 16;
        RCC_OscInitStructure.PLL.PLLN = ((CN_CFG_MCLK/Mhz)*2);;
        RCC_OscInitStructure.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStructure.PLL.PLLQ = 9;
        if (HAL_RCC_OscConfig(&RCC_OscInitStructure) != HAL_OK)
            flag=false;
        if (HAL_PWREx_EnableOverDrive() != HAL_OK)
            flag=false;

        RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CLK48;
        PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            flag=false;
    }
    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK)
        flag=false;
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK\
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2��Ƶϵ��Ϊ2

    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);
    if(ret!=HAL_OK)
        flag=false;
    LL_FLASH_EnableART();
    return flag;
}

/**
  �ú����ճ�STM32F7����stm32f7xx_hal_rcc.c��HAL_RCC_ClockConfig��������֮ͬ������,Ӧ�ù��̵͹�������
  ��Ӧ�ô�����ļ�����LPTIM��ΪϵͳTICK�ж�Դ����HAL_RCC_ClockConfig�����������ʹ��ϵͳSysClock��ΪTICK
  ʱ��Դ������Ҫ���ε���
  */
HAL_StatusTypeDef SR2711_RCC_ClockConfig(RCC_ClkInitTypeDef  *RCC_ClkInitStruct, uint32_t FLatency)
{
  uint32_t tickstart = 0;

  /* Check the parameters */
  assert_param(IS_RCC_CLOCKTYPE(RCC_ClkInitStruct->ClockType));
  assert_param(IS_FLASH_LATENCY(FLatency));

  /* To correctly read data from FLASH memory, the number of wait states (LATENCY)
  must be correctly programmed according to the frequency of the CPU clock
  (HCLK) and the supply voltage of the device. */

  /* Increasing the CPU frequency */
  if(FLatency > (FLASH->ACR & FLASH_ACR_LATENCY))
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- HCLK Configuration --------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_HCLK) == RCC_CLOCKTYPE_HCLK)
  {
    assert_param(IS_RCC_HCLK(RCC_ClkInitStruct->AHBCLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_ClkInitStruct->AHBCLKDivider);
  }

  /*------------------------- SYSCLK Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_SYSCLK) == RCC_CLOCKTYPE_SYSCLK)
  {
    assert_param(IS_RCC_SYSCLKSOURCE(RCC_ClkInitStruct->SYSCLKSource));

    /* HSE is selected as System Clock Source */
    if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
    {
      /* Check the HSE ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* PLL is selected as System Clock Source */
    else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      /* Check the PLL ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }
    /* HSI is selected as System Clock Source */
    else
    {
      /* Check the HSI ready flag */
      if(__HAL_RCC_GET_FLAG(RCC_FLAG_HSIRDY) == RESET)
      {
        return HAL_ERROR;
      }
    }

    __HAL_RCC_SYSCLK_CONFIG(RCC_ClkInitStruct->SYSCLKSource);
    /* Get Start Tick*/
    tickstart = HAL_GetTick();

    if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_HSE)
    {
      while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSE)
      {
        if((HAL_GetTick() - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else if(RCC_ClkInitStruct->SYSCLKSource == RCC_SYSCLKSOURCE_PLLCLK)
    {
      while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK)
      {
        if((HAL_GetTick() - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
    else
    {
      while(__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI)
      {
        if((HAL_GetTick() - tickstart ) > CLOCKSWITCH_TIMEOUT_VALUE)
        {
          return HAL_TIMEOUT;
        }
      }
    }
  }

  /* Decreasing the number of wait states because of lower CPU frequency */
  if(FLatency < (FLASH->ACR & FLASH_ACR_LATENCY))
  {
    /* Program the new number of wait states to the LATENCY bits in the FLASH_ACR register */
    __HAL_FLASH_SET_LATENCY(FLatency);

    /* Check that the new number of wait states is taken into account to access the Flash
    memory by reading the FLASH_ACR register */
    if((FLASH->ACR & FLASH_ACR_LATENCY) != FLatency)
    {
      return HAL_ERROR;
    }
  }

  /*-------------------------- PCLK1 Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK1) == RCC_CLOCKTYPE_PCLK1)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB1CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_ClkInitStruct->APB1CLKDivider);
  }

  /*-------------------------- PCLK2 Configuration ---------------------------*/
  if(((RCC_ClkInitStruct->ClockType) & RCC_CLOCKTYPE_PCLK2) == RCC_CLOCKTYPE_PCLK2)
  {
    assert_param(IS_RCC_PCLK(RCC_ClkInitStruct->APB2CLKDivider));
    MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, ((RCC_ClkInitStruct->APB2CLKDivider) << 3));
  }
  //�������б�������
  /* Update the SystemCoreClock global variable */
//  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> POSITION_VAL(RCC_CFGR_HPRE)];
//
//  /* Configure the source of time base considering new system clocks settings*/
//  HAL_InitTick (TICK_INT_PRIORITY);


  return HAL_OK;
}

// =============================================================================
// ���ܣ��˳��͹���ģʽ���øú��� 1,ʹ��PLLѹ������ 2,�л�PLLΪϵͳʱ��
// ��������
// ���أ�true false
// =============================================================================
bool_t STOP_EXIT_SysClockInit(void)
{
    RCC_OscInitTypeDef RCC_OscInitStructure = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStructure = {0};
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE; // ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState = RCC_HSE_ON; // ��HSE
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON; // ��PLL
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE; // PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM = (CN_CFG_HSE/Mhz); // ��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ)
    if(CN_CFG_MCLK == 216*Mhz)
    {
        RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //��PLL��Ƶϵ��(PLL��Ƶ)
        RCC_OscInitStructure.PLL.PLLP=2; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    }
    else if(CN_CFG_MCLK == 108*Mhz)
    {
        RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*4); //��PLL��Ƶϵ��(PLL��Ƶ)
        RCC_OscInitStructure.PLL.PLLP=4; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    }
    else
    {
        while(1); // ϵͳʱ�����ò�֧��
    }

    RCC_OscInitStructure.PLL.PLLQ = 9; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ)
    ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
    if(ret!=HAL_OK)
        return (FALSE);

    // ��ϵͳʱ��Դ�л���PLL
    RCC_ClkInitStructure.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // ����ϵͳʱ��ʱ��ԴΪPLL
    ret = SR2711_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_7);
    if(ret!=HAL_OK)
        return (FALSE);

    return (TRUE);
}



#define Bank5_SDRAM_ADDR                         ((u32)(0XC0000000))    //SDRAM��ʼ��ַ
#define SDRAM_MODEREG_BURST_LENGTH_1             ((u16)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((u16)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((u16)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((u16)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((u16)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((u16)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((u16)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((u16)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((u16)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((u16)0x0200)


//SDRAM
/* ���� :------------PIN_NUM---------PIN
   R/B-------------150------------ PD6
   WP--------------145-------------PD3
   CE--------------152-------------PG9
   ALE-------------100-------------PD12
   WE--------------147-------------PD5
   OE--------------146-------------PD4
   CLE-------------99--------------PD11
   LocalBus_D0-----104-------------PD14
   LocalBus_D1---- 105-------------PD15
   LocalBus_D2-----142-------------PD0
   LocalBus_D3-----143-------------PD1
   LocalBus_D4-----68--------------PE7
   LocalBus_D5-----69--------------PE8
   LocalBus_D6-----70--------------PE9
   LocalBus_D7-----73--------------PE10
 */

extern const Pin FmcSdRamPins[39];
void HAL_SDRAM_GPIOInit(void)
{
     RCC->AHB3ENR|=1<<0;  //ʹ��FMCʱ��
     PIO_Configure(FmcSdRamPins, PIO_LISTSIZE(FmcSdRamPins));
}

u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval,SDRAM_HandleTypeDef *hsdram)
{
    u32 target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;

    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;
    Command.CommandMode=cmd;                //����
    Command.CommandTarget=target_bank;      //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber=refresh;      //��ˢ�´���
    Command.ModeRegisterDefinition=regval;  //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(hsdram,&Command,0x1000)==HAL_OK) //��SDRAM��������
    {
        return 0;
    }
    else return 1;
}


void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    u32 temp=0;

    /* Step 1: Configure a clock configuration enable command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0,hsdram); //ʱ������ʹ��
    /* Step 2: Insert 100 us minimum delay */
    Djy_DelayUs(1000);                                      //������ʱ100us
    /* Step 3: Configure a PALL (precharge all) command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0,hsdram);       //�����д洢��Ԥ���

    /* Step 4: Configure an Auto Refresh command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0,hsdram);//������ˢ�´���
    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
    //bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
    //bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
    temp=(u32)SDRAM_MODEREG_BURST_LENGTH_1          |   //����ͻ������:1(������1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   //����ͻ������:����(����������/����)
              SDRAM_MODEREG_CAS_LATENCY_3           |   //����CASֵ:3(������2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp,hsdram);   //����SDRAM��ģʽ�Ĵ���

    //ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
    //COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
    //����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=216/2=108Mhz,����Ϊ8192(2^13).
    //����,COUNT=64*1000*108/8192-20=823
    HAL_SDRAM_ProgramRefreshRate(hsdram,823);

}

//16Mbytes
//SDRAM��ʼ��
void SDRAM_Init(void)
{
    SDRAM_HandleTypeDef SDRAM_Handler;
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
    SDRAM_Timing.LoadToActiveDelay=2;                      //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������  OK
    SDRAM_Timing.ExitSelfRefreshDelay=6;                   //�˳���ˢ���ӳ�Ϊ8��ʱ������          OK
    SDRAM_Timing.SelfRefreshTime=4;                        //��ˢ��ʱ��Ϊ6��ʱ������
    SDRAM_Timing.RowCycleDelay=6;                          //��ѭ���ӳ�Ϊ6��ʱ������      OK
    SDRAM_Timing.WriteRecoveryTime=2;                     //�ָ��ӳ�Ϊ2��ʱ������       OK
    SDRAM_Timing.RPDelay=2;                               //��Ԥ����ӳ�Ϊ2��ʱ������   OK
    SDRAM_Timing.RCDDelay=2;                              //�е����ӳ�Ϊ2��ʱ������   OK
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //SDRAM����BANK5��
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //������
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //������
//    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_12;
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //���ݿ��Ϊ16λ
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //һ��4��BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CASΪ3
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAMʱ��ΪHCLK/2=216M/2=108M=9.3ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;                //ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;            //��ͨ����ʱ
    SDRAM_Handler.State=HAL_SDRAM_STATE_RESET;
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;  //SDRAM��BANK5,6

    HAL_SDRAM_GPIOInit();
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);
    SDRAM_Initialization_Sequence(&SDRAM_Handler);//����SDRAM��ʼ������
}



void SDRAM_LowPowerCfg(u8 flag)
{
    u8 i;
    if(flag==InLowPower)
    {
        for(i=0;i<PIO_LISTSIZE(FmcSdRamPins);i++)
        {
             GPIO_CfgPinFunc(FmcSdRamPins[i].PORT,FmcSdRamPins[i].Pinx,GPIO_MODE_AIN,\
                                GPIO_OTYPE_PP,GPIO_SPEED_L,GPIO_PUPD_NONE);
        }
        RCC->AHB3ENR &=~(1<<0);  //����FMCʱ��
    }
    else
    {
        SDRAM_Init();
    }
}

//SDRAM��ʼ��
void SRAM_Init(void)
{
    SDRAM_Init();
}


