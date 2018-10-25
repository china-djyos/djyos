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
#include "stdint.h"
#include "cpu_peri.h"
#include "SRAM.h"
#include "stm32f1xx_hal_sram.h"

void SRAM_GPIO_Init(void)
{
    RCC->AHBENR|=1<<8;              //ʹ��FSMCʱ��
    RCC->APB2ENR|=1<<0;
    RCC->APB2ENR|=1<<5;         //ʹ��PORTDʱ��
    RCC->APB2ENR|=1<<6;         //ʹ��PORTEʱ��
    RCC->APB2ENR|=1<<7;         //ʹ��PORTFʱ��
     RCC->APB2ENR|=1<<8;          //ʹ��PORTGʱ��

    //PORTD�����������
    GPIOD->CRH &=0X00000000;        //8,9,10,11,12,13,14,15
    GPIOD->CRH |=0XBBBBBBBB;
    GPIOD->CRL &=0X0F00FF00;        //0,1,4,5,7
    GPIOD->CRL |=0XB0BB00BB;

    //PORTE�����������
    GPIOE->CRH &=0X00000000;        //8~15
    GPIOE->CRH |=0XBBBBBBBB;
    GPIOE->CRL &=0X0F000F00;        //0,1,3,4,5,7
    GPIOE->CRL |=0XB0BBB0BB;

    //PORTF�����������
    GPIOF->CRH &=0X0000FFFF;        //12,13,14,15
    GPIOF->CRH |=0XBBBB0000;
    GPIOF->CRL &=0XFF000000;        //0,1,2,3,4,5
    GPIOF->CRL |=0X00BBBBBB;

    GPIOG->CRH&=0XFFF0F00F;            //9,10,12
    GPIOG->CRH|=0X000B0BB0;
    GPIOG->CRL&=0XFF000000;            //0,1,2,3,4,5
    GPIOG->CRL|=0X00BBBBBB;
}

void SRAM_FSMCConfig(void)
{
    SRAM_HandleTypeDef hsram;

    FSMC_NORSRAM_TimingTypeDef Timing;



    hsram.Instance  = FSMC_NORSRAM_DEVICE;
    hsram.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

    Timing. AddressSetupTime = 1;
    Timing. AddressHoldTime = 0;
    Timing. DataSetupTime = 2;
    Timing. BusTurnAroundDuration = 0;
    Timing. CLKDivision = 0;
    Timing. DataLatency = 0;
    Timing. AccessMode = FSMC_ACCESS_MODE_A;

    hsram.Init. NSBank= FSMC_NORSRAM_BANK1;
    hsram.Init. DataAddressMux= FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init. MemoryType =FSMC_MEMORY_TYPE_SRAM;
    hsram.Init. MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init. BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init. WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init. WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram.Init. WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init. WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init. WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init. ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init. AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init. WriteBurst = FSMC_WRITE_BURST_DISABLE;
    //      /* Initialize the NAND controller */

    if(HAL_SRAM_Init(&hsram, &Timing, &Timing) != HAL_OK)
    {
        return;//����
    }

}

void LCD_FSMCConfig(void)
{
    SRAM_HandleTypeDef hsram;

    FSMC_NORSRAM_TimingTypeDef Timing;



    hsram.Instance  = FSMC_NORSRAM_DEVICE;
    hsram.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

    Timing. AddressSetupTime = 1;
    Timing. AddressHoldTime = 0;
    Timing. DataSetupTime = 2;
    Timing. BusTurnAroundDuration = 0;
    Timing. CLKDivision = 0;
    Timing. DataLatency = 1;
    Timing. AccessMode = FSMC_ACCESS_MODE_A;


    hsram.Init. NSBank= FSMC_NORSRAM_BANK3;
    hsram.Init. DataAddressMux= FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init. MemoryType =FSMC_MEMORY_TYPE_SRAM;
    hsram.Init. MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init. BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init. WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init. WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram.Init. WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init. WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init. WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init. ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init. AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init. WriteBurst = FSMC_WRITE_BURST_DISABLE;

    if(HAL_SRAM_Init(&hsram, &Timing, &Timing) != HAL_OK)
    {
        return;//����
    }
}

void NandFlash_FSMCConfig(void)
{

    FSMC_NAND_PCC_TimingTypeDef  Timing;
    NAND_HandleTypeDef nandHandle;

    nandHandle.Instance  = FSMC_NAND_DEVICE;

    nandHandle.Init. NandBank=FSMC_NAND_BANK3;
    nandHandle.Init. Waitfeature =FSMC_NAND_PCC_WAIT_FEATURE_ENABLE;
    nandHandle.Init. MemoryDataWidth=FSMC_NAND_PCC_MEM_BUS_WIDTH_8;
    nandHandle.Init. EccComputation =FSMC_NAND_ECC_ENABLE;
    nandHandle.Init. ECCPageSize=FSMC_NAND_ECC_PAGE_SIZE_512BYTE;
    nandHandle.Init. TCLRSetupTime=0;
    nandHandle.Init. TARSetupTime=0;

    Timing. SetupTime=0;
    Timing. WaitSetupTime=3;
    Timing. HoldSetupTime=9;
    Timing. HiZSetupTime=0;

    if(HAL_NAND_Init(&nandHandle, &Timing, &Timing) != HAL_OK)
    {
        return;/* Initialization Error */
    }

}

void Dm9000a_FSMCConfig()
{

    SRAM_HandleTypeDef hsram;
    FSMC_NORSRAM_TimingTypeDef Timing;


    hsram.Instance  = FSMC_NORSRAM_DEVICE;
    hsram.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;

    Timing. AddressSetupTime = 1;
    Timing. AddressHoldTime = 1;
    Timing. DataSetupTime = 2;
    Timing. BusTurnAroundDuration = 1;
    Timing. CLKDivision = 1;
    Timing. DataLatency = 1;
    Timing. AccessMode = FSMC_ACCESS_MODE_A;


    hsram.Init. NSBank= FSMC_NORSRAM_BANK4;
    hsram.Init. DataAddressMux= FSMC_DATA_ADDRESS_MUX_DISABLE;
    hsram.Init. MemoryType =FSMC_MEMORY_TYPE_SRAM;
    hsram.Init. MemoryDataWidth=FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram.Init. BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
    hsram.Init. WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram.Init. WrapMode = FSMC_WRAP_MODE_DISABLE;
    hsram.Init. WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
    hsram.Init. WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
    hsram.Init. WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
    hsram.Init. ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
    hsram.Init. AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram.Init. WriteBurst = FSMC_WRITE_BURST_DISABLE;

    if(HAL_SRAM_Init(&hsram, &Timing, &Timing) != HAL_OK)
    {
        return;//����
    }


}

void SRAM_Init(void)
{
    SRAM_GPIO_Init();
    SRAM_FSMCConfig();
    LCD_FSMCConfig();
    NandFlash_FSMCConfig();
    Dm9000a_FSMCConfig();
}

