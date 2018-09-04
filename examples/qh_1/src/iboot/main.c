
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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * app_main.c
 *
 *  Created on: 2014-5-28
 *      Author: Administrator
 */

#include "stdint.h"
#include "stddef.h"

#include "cpu_peri.h"

//LDE define
#define LED1_ON   GPIO_SettoLow(CN_GPIO_C, (1<<9))
#define LED1_OFF  GPIO_SettoHigh(CN_GPIO_C,  (1<<9))
#define LED2_ON   GPIO_SettoLow(CN_GPIO_C, (1<<8))
#define LED2_OFF  GPIO_SettoHigh(CN_GPIO_C,  (1<<8))
#define LED3_ON   GPIO_SettoLow(CN_GPIO_C, (1<<4))
#define LED3_OFF  GPIO_SettoHigh(CN_GPIO_C,  (1<<4))
#define LED4_ON   GPIO_SettoLow(CN_GPIO_C, (1<<6))
#define LED4_OFF  GPIO_SettoHigh(CN_GPIO_C,  (1<<6))

//----LED_init-----------------------------------------------------------------
//���ܣ�ʹ��gpio�˿���������ģʽ
//������void
//���أ�void
//-----------------------------------------------------------------------------
void led_init(void)
{
    GPIO_PowerOn(CN_GPIO_C);
    GPIO_CfgPinFunc(CN_GPIO_C, 6, CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
    GPIO_CfgPinFunc(CN_GPIO_C, 4, CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
    GPIO_CfgPinFunc(CN_GPIO_C, 8, CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
    GPIO_CfgPinFunc(CN_GPIO_C, 9, CN_GPIO_MODE_GPIO_OUT_PP_2Mhz);
    LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
}

ptu32_t djy_main(void)
{
    led_init();
//    Hello_Word();
    while(1)
    {
        Djy_EventDelay(500*1000);
        LED1_ON;
        LED2_OFF;
        LED3_OFF;
        LED4_OFF;
        Djy_EventDelay(500*1000);
        LED1_OFF;
        LED2_ON;
        LED3_OFF;
        LED4_OFF;
        Djy_EventDelay(500*1000);
        LED1_OFF;
        LED2_OFF;
        LED3_ON;
        LED4_OFF;
        Djy_EventDelay(500*1000);
        LED1_OFF;
        LED2_OFF;
        LED3_OFF;
        LED4_ON;
    }
    return 0;
}
