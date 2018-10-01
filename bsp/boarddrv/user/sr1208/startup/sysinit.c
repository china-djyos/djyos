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
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2��Ƶϵ��Ϊ2

    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_7);
    if(ret!=HAL_OK)
        flag=false;
    LL_FLASH_EnableART();
    return flag;
}

// =============================================================================
// ���ܣ�Ƭ��SRAM��ʼ��������GPIO��FMC�������ĳ�ʼ��
// ��������
// ���أ���
//˵����  ���Ŷ�Ӧ˵����
//LocalBus_CS0 --PD7  XA0 --PF0   NANDWE --PD5   NANDOE --PD4
//D0 --PD14   D1 --PD15  D2--PD0   D3--PD1  D4--PE7  D5--PE8  D6--PE9  D7--PE10
// =============================================================================
//static void SRAM_GpioInit(void)
//{
//  RCC->AHB3ENR|=1<<0;         //ʹ��FMCʱ��
//    GPIO_PowerOn(GPIO_D);//ʹ��P-DEFGHIʱ��
//    GPIO_PowerOn(GPIO_E);
//    GPIO_PowerOn(GPIO_F);
//    GPIO_PowerOn(GPIO_G);
//    GPIO_PowerOn(GPIO_H);
//    GPIO_PowerOn(GPIO_I);
////��ʼ������IO
//    GPIO_CfgPinFunc(GPIO_D,PIN0|PIN1|PIN4|PIN5|PIN7|PIN14|PIN15,
//            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE); //PC0/2/3
//
//    GPIO_CfgPinFunc(GPIO_E,PIN7|PIN8|PIN9|PIN10,
//            GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);//PD0/1/8/9/10/14/15
//
//  GPIO_CfgPinFunc(GPIO_F,PIN0,
//            GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_NONE);         //PE0/1/7~15
//
////////////////////////////////////////
//    GPIO_AFSet(GPIO_D,PIN0|PIN1|PIN4|PIN5|PIN7|PIN14|PIN15,AF12); //PC0/2/3
//
//    GPIO_AFSet(GPIO_E,PIN7|PIN8|PIN9|PIN10,AF12);        //PD0/1/8/9/10/14/15
//
////    GPIO_AFSet(GPIO_F,PIN0,AF12);            //PE0/1/7~15
//}


//void LCD_FSMCConfig(void)
//{
//   FMC_NORSRAM_InitTypeDef FSMC_NORSRAMInitStructure;
//   FMC_NORSRAM_TimingTypeDef p;
//   FMC_NORSRAM_TypeDef *dev=(FMC_Bank1_TypeDef*)FMC_Bank1_R_BASE;
////    /*-- FSMC Configuration ------------------------------------------------------*/
////    /*----------------------- SRAM Bank 1 ----------------------------------------*/
//      /* FSMC_Bank1_NORSRAM1 configuration */
//      p.AddressSetupTime = 7;//15;
//      p.AddressHoldTime = 2;//30;    //��Ч
//      p.DataSetupTime = 12;//30;
//      p.BusTurnAroundDuration = 0;//3;
//      p.CLKDivision = 2;//1;
//      p.DataLatency = 2;
//
//      p.AccessMode = FMC_ACCESS_MODE_A;
//
////      /* Color LCD configuration ------------------------------------
////         LCD configured as follow:
////            - Data/Address MUX = Disable
////            - Memory Type = SRAM
////            - Data Width = 8bit
////            - Write Operation = Enable
////            - Extended Mode = Enable
////            - Asynchronous Wait = Disable */
//      FSMC_NORSRAMInitStructure.NSBank = FMC_NORSRAM_BANK1;
//      FSMC_NORSRAMInitStructure.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
//      FSMC_NORSRAMInitStructure.MemoryType = FMC_MEMORY_TYPE_SRAM;
//      FSMC_NORSRAMInitStructure.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_8;
//      FSMC_NORSRAMInitStructure.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
//      FSMC_NORSRAMInitStructure.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
//      FSMC_NORSRAMInitStructure.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
//      FSMC_NORSRAMInitStructure.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
//      FSMC_NORSRAMInitStructure.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
//      FSMC_NORSRAMInitStructure.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
//      FSMC_NORSRAMInitStructure.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
//      FSMC_NORSRAMInitStructure.WriteBurst = FMC_WRITE_BURST_DISABLE;
//      FSMC_NORSRAMInitStructure.ContinuousClock=FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
//    FSMC_NORSRAMInitStructure.PageSize=FMC_PAGE_SIZE_NONE;
//    FSMC_NORSRAMInitStructure.WriteFifo=FMC_WRITE_FIFO_DISABLE;
//
//    FMC_NORSRAM_Init(dev,&FSMC_NORSRAMInitStructure);
//    FMC_NORSRAM_Timing_Init(dev,&p,0);
////      /* BANK 0 (of NOR/SRAM Bank) is enabled */
//    dev->BTCR[0]|=FMC_BCR1_MBKEN;
//}


//void NandFlash_FSMCConfig(void)
//{
//  FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
//  FSMC_NAND_PCCARDTimingInitTypeDef  p;
//
//  /*-- FSMC Configuration ------------------------------------------------------*/
//  p.FSMC_SetupTime = 0x0;
//  p.FSMC_WaitSetupTime = 0x3;
//  p.FSMC_HoldSetupTime = 0x9;
//  p.FSMC_HiZSetupTime = 0x0;
//
//  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank3_NAND;
//  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Enable;
//  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
//  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Enable;
//  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_512Bytes;
//  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;
//  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;
//  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
//  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;
//
//  FSMC_NANDInit(&FSMC_NANDInitStructure);
//
//  /*!< FSMC NAND Bank Cmd Test */
//  FSMC_NANDCmd(FSMC_Bank3_NAND, ENABLE);
//}


//SDRAM��ʼ��                                 //TODO �캣��д��SRAM_Init,��ע�ͣ�����
//void SRAM_Init(void)
//{
//    SRAM_GpioInit();
//  LCD_FSMCConfig();
////    NandFlash_FSMCConfig();
//}


//16Mbytes           //TODO �ο�sr5261�ģ�����û�н��йܽų�ʼ����������Ҫ��board.c
//SDRAM��ʼ��

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

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram)
{
    u32 Pin;
    RCC->AHB3ENR|=1<<0;  //ʹ��FMCʱ��
    GPIO_PowerOn(GPIO_C);
    GPIO_PowerOn(GPIO_D);//ʹ��P-DEFGHIʱ��
    GPIO_PowerOn(GPIO_E);
    GPIO_PowerOn(GPIO_F);
    GPIO_PowerOn(GPIO_G);

    Pin=PIN0|PIN2|PIN3;
    GPIO_AFSet(GPIO_C,Pin,AF12);
    GPIO_CfgPinFunc(GPIO_C,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN0|PIN1|PIN8|PIN9|PIN10|PIN14|PIN15;
    GPIO_AFSet(GPIO_D,Pin,AF12);
    GPIO_CfgPinFunc(GPIO_D,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    Pin=PIN0|PIN1|PIN7|PIN8|PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
    GPIO_AFSet(GPIO_E,Pin,AF12);
    GPIO_CfgPinFunc(GPIO_E,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    Pin=PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN11|PIN12|PIN13|PIN14|PIN15;
    GPIO_AFSet(GPIO_F,Pin,AF12);
    GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN0|PIN1|PIN2|PIN4|PIN5|PIN8|PIN15;
    GPIO_AFSet(GPIO_G,Pin,AF12);
    GPIO_CfgPinFunc(GPIO_G,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);
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


void SDRAM_Init(void)
{
    SDRAM_HandleTypeDef SDRAM_Handler;
    FMC_SDRAM_TimingTypeDef SDRAM_Timing;
    SDRAM_Timing.LoadToActiveDelay=2;                      //����ģʽ�Ĵ���������ʱ����ӳ�Ϊ2��ʱ������  OK
    SDRAM_Timing.ExitSelfRefreshDelay=6;                   //�˳���ˢ���ӳ�Ϊ8��ʱ������          OK
    SDRAM_Timing.SelfRefreshTime=4;                        //��ˢ��ʱ��Ϊ6��ʱ������
    SDRAM_Timing.RowCycleDelay=6;                          //��ѭ���ӳ�Ϊ6��ʱ������      OK
    SDRAM_Timing.WriteRecoveryTime=2;                      //�ָ��ӳ�Ϊ2��ʱ������       OK
    SDRAM_Timing.RPDelay=2;                                //��Ԥ����ӳ�Ϊ2��ʱ������   OK
    SDRAM_Timing.RCDDelay=2;                               //�е����ӳ�Ϊ2��ʱ������   OK
    SDRAM_Handler.Init.SDBank=FMC_SDRAM_BANK1;                           //SDRAM����BANK5��
    SDRAM_Handler.State=HAL_SDRAM_STATE_RESET;
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;                              //SDRAM��BANK5,6
    SDRAM_Handler.Init.ColumnBitsNumber=FMC_SDRAM_COLUMN_BITS_NUM_9;     //������
//    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_13;          //������
    SDRAM_Handler.Init.RowBitsNumber=FMC_SDRAM_ROW_BITS_NUM_12;
    SDRAM_Handler.Init.MemoryDataWidth=FMC_SDRAM_MEM_BUS_WIDTH_16;       //���ݿ��Ϊ16λ
    SDRAM_Handler.Init.InternalBankNumber=FMC_SDRAM_INTERN_BANKS_NUM_4;  //һ��4��BANK
    SDRAM_Handler.Init.CASLatency=FMC_SDRAM_CAS_LATENCY_3;               //CASΪ3
    SDRAM_Handler.Init.WriteProtection=FMC_SDRAM_WRITE_PROTECTION_DISABLE;//ʧ��д����
    SDRAM_Handler.Init.SDClockPeriod=FMC_SDRAM_CLOCK_PERIOD_2;           //SDRAMʱ��ΪHCLK/2=216M/2=108M=9.3ns
    SDRAM_Handler.Init.ReadBurst=FMC_SDRAM_RBURST_ENABLE;                //ʹ��ͻ��
    SDRAM_Handler.Init.ReadPipeDelay=FMC_SDRAM_RPIPE_DELAY_0;            //��ͨ����ʱ
    SDRAM_Handler.State=HAL_SDRAM_STATE_RESET;
    SDRAM_Handler.Instance=FMC_SDRAM_DEVICE;  //SDRAM��BANK5,6
    HAL_SDRAM_Init(&SDRAM_Handler,&SDRAM_Timing);
    SDRAM_Initialization_Sequence(&SDRAM_Handler);//����SDRAM��ʼ������
}

//SDRAM��ʼ��
void SRAM_Init(void)
{
    SDRAM_Init();
}


