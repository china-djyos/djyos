
//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"

//��ʼ��PF9��PF10Ϊ�����.��ʹ���������ڵ�ʱ��
//LED IO��ʼ��
#define LD2     0	//PA5
u32 led_mask[] = {1UL << 5};
void LED_Init(void)
{
	RCC->AHB2ENR|=1<<0;//ʹ��PORTAʱ��

	GPIOA->MODER    &= ~((3UL << 2*5));
	GPIOA->MODER    |=  ((1UL << 2*5));
	GPIOA->OTYPER   &= ~((1UL << 2*5));
	GPIOA->OSPEEDR  &= ~((3UL << 2*5));
	GPIOA->OSPEEDR  |=  ((2UL << 2*5));
	GPIOA->PUPDR    &= ~((3UL << 2*5));
	GPIOA->PUPDR    |=  ((1UL << 2*5));
}
void LED_On (unsigned int num)
{
	GPIOA->BSRR = led_mask[num];
}
void LED_Off (unsigned int num)
{
	GPIOA->BSRR = led_mask[num]<<16;
}
ptu32_t FlashEraseTest(u32 num)
{
	s32 Ret;
	u32 SectorNo;
	u32 SECTORError=0;//�������������Ϣ
	static FLASH_EraseInitTypeDef EraseInitStruct;

	SectorNo = num + 512;
	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Banks       = FLASH_BANK_2;
	EraseInitStruct.Page        = SectorNo;
	EraseInitStruct.NbPages     = 1;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	 	Ret=-1;
	else
		Ret=0;
	HAL_FLASH_Lock();

	return Ret;
}

s32 Flash_PageProgram( u8 *Data)
{
	u32 Ret,i,ipage;
	u64 *pData = (u64*)Data;

	u32 Addr = 0x08080000;

	if(!Data)
		return (-1);

	HAL_FLASH_Unlock();

	for(i = 0; i < 512;)
	{
    	if(*(u64*)Addr != *pData)
    	{
    		Ret = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,Addr,*pData);
    		if(Ret != HAL_OK)
    			break;
    	}
		pData++;
		i += 8;
		Addr += 8;
	}

	HAL_FLASH_Lock();

	return i;
}
u8 buf[512];
ptu32_t djy_main(void)
{
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);
	LED_Init();

	u32 i,flag = 0;
	for(i = 0; i < 512; i++)
	{
		buf[i] = i;
	}
	while(1)
	{
		Djy_EventDelay(500*mS);
		LED_On(LD2);
		Djy_EventDelay(500*mS);
		LED_Off(LD2);

		if(flag)
		{
			flag = 0;
			Flash_PageProgram(buf);
			FlashEraseTest(0);
		}
	}
	return 0;
}

