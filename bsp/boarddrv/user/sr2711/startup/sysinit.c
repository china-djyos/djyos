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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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

#if !defined  (OSCILLATORTYPE)
#define OSCILLATORTYPE   OSCILLATORTYPE_HSE
#endif
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

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    if (OSCILLATORTYPE == OSCILLATORTYPE_HSE )
    {
        RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //时钟源为HSE
        RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //打开HSE
        RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;                  //打开PLL
        RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;          //PLL时钟源选择HSE
        RCC_OscInitStructure.PLL.PLLM=(CN_CFG_HSE/Mhz); //主PLL和音频PLL分频系数(PLL之前的分频)
        if(CN_CFG_MCLK == 216*Mhz)
        {
            RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //主PLL倍频系数(PLL倍频)
            RCC_OscInitStructure.PLL.PLLP=2; //系统时钟的主PLL分频系数(PLL之后的分频)
        }
        else if(CN_CFG_MCLK == 108*Mhz)
        {
            RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*4); //主PLL倍频系数(PLL倍频)
            RCC_OscInitStructure.PLL.PLLP=4; //系统时钟的主PLL分频系数(PLL之后的分频)
        }
        else
        {
            while(1); // 系统时钟配置不支持
        }

        RCC_OscInitStructure.PLL.PLLQ=9; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
        ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
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
    ret=HAL_PWREx_EnableOverDrive(); //开启Over-Driver功能
    if(ret!=HAL_OK)
        flag=false;
    //选中PLL作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK\
            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB分频系数为1
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1分频系数为4
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2分频系数为2

    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);
    if(ret!=HAL_OK)
        flag=false;
    LL_FLASH_EnableART();
    return flag;
}

/**
  该函数照抄STM32F7库中stm32f7xx_hal_rcc.c中HAL_RCC_ClockConfig函数，不同之处在于,应用工程低功耗需求
  ，应用代码库文件基于LPTIM作为系统TICK中断源，在HAL_RCC_ClockConfig函数最后两行使能系统SysClock作为TICK
  时钟源代码需要屏蔽掉。
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
  //以下两行必须屏蔽
  /* Update the SystemCoreClock global variable */
//  SystemCoreClock = HAL_RCC_GetSysClockFreq() >> AHBPrescTable[(RCC->CFGR & RCC_CFGR_HPRE)>> POSITION_VAL(RCC_CFGR_HPRE)];
//
//  /* Configure the source of time base considering new system clocks settings*/
//  HAL_InitTick (TICK_INT_PRIORITY);


  return HAL_OK;
}

// =============================================================================
// 功能：退出低功耗模式调用该函数 1,使能PLL压控振荡器 2,切换PLL为系统时钟
// 参数：无
// 返回：true false
// =============================================================================
bool_t STOP_EXIT_SysClockInit(void)
{
    RCC_OscInitTypeDef RCC_OscInitStructure = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStructure = {0};
    HAL_StatusTypeDef ret = HAL_OK;

    RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSE; // 时钟源为HSE
    RCC_OscInitStructure.HSEState = RCC_HSE_ON; // 打开HSE
    RCC_OscInitStructure.PLL.PLLState = RCC_PLL_ON; // 打开PLL
    RCC_OscInitStructure.PLL.PLLSource = RCC_PLLSOURCE_HSE; // PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM = (CN_CFG_HSE/Mhz); // 主PLL和音频PLL分频系数(PLL之前的分频)
    if(CN_CFG_MCLK == 216*Mhz)
    {
        RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*2); //主PLL倍频系数(PLL倍频)
        RCC_OscInitStructure.PLL.PLLP=2; //系统时钟的主PLL分频系数(PLL之后的分频)
    }
    else if(CN_CFG_MCLK == 108*Mhz)
    {
        RCC_OscInitStructure.PLL.PLLN=((CN_CFG_MCLK/Mhz)*4); //主PLL倍频系数(PLL倍频)
        RCC_OscInitStructure.PLL.PLLP=4; //系统时钟的主PLL分频系数(PLL之后的分频)
    }
    else
    {
        while(1); // 系统时钟配置不支持
    }

    RCC_OscInitStructure.PLL.PLLQ = 9; //USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频)
    ret = HAL_RCC_OscConfig(&RCC_OscInitStructure);//初始化
    if(ret!=HAL_OK)
        return (FALSE);

    // 将系统时钟源切换到PLL
    RCC_ClkInitStructure.ClockType = RCC_CLOCKTYPE_SYSCLK;
    RCC_ClkInitStructure.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK; // 设置系统时钟时钟源为PLL
    ret = SR2711_RCC_ClockConfig(&RCC_ClkInitStructure, FLASH_LATENCY_7);
    if(ret!=HAL_OK)
        return (FALSE);

    return (TRUE);
}



#define Bank5_SDRAM_ADDR                         ((u32)(0XC0000000))    //SDRAM起始地址
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
/* 功能 :------------PIN_NUM---------PIN
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
     RCC->AHB3ENR|=1<<0;  //使能FMC时钟
     PIO_Configure(FmcSdRamPins, PIO_LISTSIZE(FmcSdRamPins));
}

u8 SDRAM_Send_Cmd(u8 bankx,u8 cmd,u8 refresh,u16 regval,SDRAM_HandleTypeDef *hsdram)
{
    u32 target_bank=0;
    FMC_SDRAM_CommandTypeDef Command;

    if(bankx==0) target_bank=FMC_SDRAM_CMD_TARGET_BANK1;
    else if(bankx==1) target_bank=FMC_SDRAM_CMD_TARGET_BANK2;
    Command.CommandMode=cmd;                //命令
    Command.CommandTarget=target_bank;      //目标SDRAM存储区域
    Command.AutoRefreshNumber=refresh;      //自刷新次数
    Command.ModeRegisterDefinition=regval;  //要写入模式寄存器的值
    if(HAL_SDRAM_SendCommand(hsdram,&Command,0x1000)==HAL_OK) //向SDRAM发送命令
    {
        return 0;
    }
    else return 1;
}


void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
    u32 temp=0;

    /* Step 1: Configure a clock configuration enable command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_CLK_ENABLE,1,0,hsdram); //时钟配置使能
    /* Step 2: Insert 100 us minimum delay */
    Djy_DelayUs(1000);                                      //至少延时100us
    /* Step 3: Configure a PALL (precharge all) command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_PALL,1,0,hsdram);       //对所有存储区预充电

    /* Step 4: Configure an Auto Refresh command */
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_AUTOREFRESH_MODE,8,0,hsdram);//设置自刷新次数
    //配置模式寄存器,SDRAM的bit0~bit2为指定突发访问的长度，
    //bit3为指定突发访问的类型，bit4~bit6为CAS值，bit7和bit8为运行模式
    //bit9为指定的写突发模式，bit10和bit11位保留位
    temp=(u32)SDRAM_MODEREG_BURST_LENGTH_1          |   //设置突发长度:1(可以是1/2/4/8)
              SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |   //设置突发类型:连续(可以是连续/交错)
              SDRAM_MODEREG_CAS_LATENCY_3           |   //设置CAS值:3(可以是2/3)
              SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //设置操作模式:0,标准模式
              SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //设置突发写模式:1,单点访问
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp,hsdram);   //设置SDRAM的模式寄存器

    //刷新频率计数器(以SDCLK频率计数),计算方法:
    //COUNT=SDRAM刷新周期/行数-20=SDRAM刷新周期(us)*SDCLK频率(Mhz)/行数
    //我们使用的SDRAM刷新周期为64ms,SDCLK=216/2=108Mhz,行数为8192(2^13).
    //所以,COUNT=64*1000*108/8192-20=823
    HAL_SDRAM_ProgramRefreshRate(hsdram,823);

}

//16Mbytes
//SDRAM初始化
void SDRAM_Init(void)
{
    SDRAM_HandleTypeDef SDRAM_Handler;
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
    SDRAM_Timing.LoadToActiveDelay=2;                      //加载模式寄存器到激活时间的延迟为2个时钟周期  OK
    SDRAM_Timing.ExitSelfRefreshDelay=6;                   //退出自刷新延迟为8个时钟周期          OK
    SDRAM_Timing.SelfRefreshTime=4;                        //自刷新时间为6个时钟周期
    SDRAM_Timing.RowCycleDelay=6;                          //行循环延迟为6个时钟周期      OK
    SDRAM_Timing.WriteRecoveryTime=2;                     //恢复延迟为2个时钟周期       OK
    SDRAM_Timing.RPDelay=2;                               //行预充电延迟为2个时钟周期   OK
    SDRAM_Timing.RCDDelay=2;                              //行到列延迟为2个时钟周期   OK
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //SDRAM接在BANK5上
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //列数量
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //行数量
//    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_12;
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //数据宽度为16位
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //一共4个BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CAS为3
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//失能写保护
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAM时钟为HCLK/2=216M/2=108M=9.3ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;                //使能突发
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;            //读通道延时
    SDRAM_Handler.State=HAL_SDRAM_STATE_RESET;
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;  //SDRAM在BANK5,6

    HAL_SDRAM_GPIOInit();
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);
    SDRAM_Initialization_Sequence(&SDRAM_Handler);//发送SDRAM初始化序列
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
        RCC->AHB3ENR &=~(1<<0);  //禁能FMC时钟
    }
    else
    {
        SDRAM_Init();
    }
}

//SDRAM初始化
void SRAM_Init(void)
{
    SDRAM_Init();
}


