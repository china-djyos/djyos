
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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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

/*led test*/
#include "SCM6XX_ALL.h"
#include "djyos.h"

ptu32_t ledTestTask(void)
{
    #ifndef mS
    #define mS 1000
    #endif

    extern void BoardLedInit(void);
    BoardLedInit();

    while(1)
    {
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(13,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(14,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(29,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(30,GPIO_HIGH);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(13,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(14,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(29,GPIO_LOW);
        Djy_EventDelay(100*mS);
        GPIO_Write_Out(30,GPIO_LOW);
        Djy_EventDelay(100*mS);
    }

    return 0;
}

void LED_Task(void)
{
  u16 evtt_led;
  evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
          ledTestTask,NULL,1000,"Led_Test");
  //�¼���������������Ϊ0,�����shell����,�������ɼ�shell�����в���
  if(evtt_led!=CN_EVTT_ID_INVALID)
      Djy_EventPop(evtt_led,NULL,0,NULL,0,0);
}

/*test timer*/
#include "timer_hard.h"

u32 UsrTimerHandle0(ptu32_t TimerHandle)
{
    printk("timer 0 Usr\r\n");
    return 0;
}

u32 UsrTimerHandle1(ptu32_t TimerHandle)
{
    printk("timer 1 Usr\r\n");
    return 0;
}

void timerTestTask(void)
{
    ptu32_t timerHandle;
    u32 timerFre;

    extern bool_t ModuleInstall_HardTimer(void);
    ModuleInstall_HardTimer();

    timerHandle =  HardTimer_Alloc(UsrTimerHandle0);
    if(timerHandle == 0)
    {
        return;
    }

    timerFre = HardTimer_GetFreq(timerHandle);
    HardTimer_Ctrl(timerHandle, EN_TIMER_SETCYCLE,1*timerFre);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETINTPRO,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);

    timerHandle =  HardTimer_Alloc(UsrTimerHandle1);
    timerFre = HardTimer_GetFreq(timerHandle);
    HardTimer_Ctrl(timerHandle, EN_TIMER_SETCYCLE,2*timerFre);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETINTPRO,false);
    HardTimer_Ctrl(timerHandle,EN_TIMER_SETRELOAD,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_ENINT,true);
    HardTimer_Ctrl(timerHandle,EN_TIMER_STARTCOUNT,true);
}

/*test Rtc*/
void Rtc_Test(void)
{
    extern bool_t djyos_ver(char *param);
    djyos_ver(0);

    extern ptu32_t ModuleInstall_CpuRtc(ptu32_t para);
    ModuleInstall_CpuRtc(0);
}

/*Test Wdt*/

void Wdt_Test(void)
{
    extern bool_t Wdt_Initial(u32 setcycle);
    Wdt_Initial(0);

    printk("wdt test begin\r\n");
}

ptu32_t djy_main(void)
{
    LED_Task();
    timerTestTask();
    Rtc_Test();
//    Wdt_Test();
	while(1)
	{
//	    ledTestTask();
//		printf("hello world!\r\n");
		Djy_EventDelay(1000*1000);
	}
	return 0;
}
