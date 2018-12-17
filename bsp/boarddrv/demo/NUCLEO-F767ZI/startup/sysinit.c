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

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���

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
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=SYSCLK_DIV;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=AHB1_DIV;//APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=AHB2_DIV;//APB2��Ƶϵ��Ϊ2

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
static void SRAM_GpioInit(void)
{
    GPIO_PowerOn(GPIO_D);//ʹ��P-DEFGHIʱ��
    GPIO_PowerOn(GPIO_E);
    GPIO_PowerOn(GPIO_F);
    GPIO_PowerOn(GPIO_G);
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);
//��ʼ������IO
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
//����I/o


}
static void SRAM_FmcInit(void)
{

    RCC->AHB3ENR|=1<<0;         //ʹ��FMCʱ��
      /* Configure and enable Bank1_SRAM3��NE3 */
    //bank1��NE1~4,ÿһ����һ��BCR+TCR�������ܹ��˸��Ĵ�����

    FMC_Bank1->BTCR[4]=0X00000000;
    FMC_Bank1->BTCR[5]=0X00000000;
    FMC_Bank1E->BWTR[4]=0X00000000;
    //����BCR�Ĵ��� ʹ���첽ģʽ,ģʽA(��д����һ��ʱ��Ĵ���)
    //BTCR[ż��]:BCR�Ĵ���;BTCR[����]:BTR�Ĵ���
    FMC_Bank1->BTCR[4]|=1<<12;//�洢��дʹ��
    FMC_Bank1->BTCR[4]|=1<<4; //�洢�����ݿ��Ϊ16bit
    //����BTR�Ĵ���         ��HCLK=216M/2, 1��HCLK=2*4.629ns
    FMC_Bank1->BTCR[5]|=8<<5; //���ݱ���ʱ�䣨DATAST��
    FMC_Bank1->BTCR[5]|=0<<3; //��ַ����ʱ�䣨ADDHLD��δ�õ�
    FMC_Bank1->BTCR[5]|=6<<0; //��ַ����ʱ�䣨ADDSET��Ϊ0��HCLK 0ns
    //����дʱ��Ĵ���
    FMC_Bank1E->BWTR[4]=0x0FFFFFFF;//Ĭ��ֵ
    //ʹ��BANK1����3
    FMC_Bank1->BTCR[4]|=1<<0;
}

//SDRAM��ʼ��
void SRAM_Init(void)
{
//    SRAM_GpioInit();
//    SRAM_FmcInit();

}


