
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

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include <os.h>
#include "cpu_peri.h"

#define GPIO_PIN_MASK            0x1Fu
#define GPIO_PIN_NO(x)           (((1)<<(x & GPIO_PIN_MASK)))

ptu32_t Led1Task(void)
{
    PORT_MuxConfig(PORT_PORT_A,PORT_PIN(28),PORT_PINMUX_GPIO);
    PORT_MuxConfig(PORT_PORT_A,PORT_PIN(29),PORT_PINMUX_GPIO);

    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(28),GPIO_FUNC_OUTPUT);
    GPIO_PinConfig(GPIO_PORT_A,GPIO_PIN(29),GPIO_FUNC_OUTPUT);

    while(1)
    {
        GPIO_PinToggle(GPIO_PORT_A,GPIO_PIN(28));
        Djy_EventDelay(1000000);
    }
    return 0;
}

void LedTest(void)
{
    uint16_t evtt_led;
    evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                Led1Task,NULL,0x300,"led1 blink");

    Djy_EventPop(evtt_led,NULL,0,NULL,0,0);
}

ptu32_t djy_main(void)
{
    printf("%s:welcom to the djyos--k70\n\r",__FUNCTION__);
    LedTest();
	extern void Sh_GetStatus(char *param);
	extern void Sh_GetRunMode(char *param);
	Sh_GetRunMode(NULL);
	Sh_GetStatus(NULL);
	void  gdd_test(void);
	gdd_test();
    while(1)
    {
    	Djy_EventDelay(500*mS);
    }
    return 0;
}

