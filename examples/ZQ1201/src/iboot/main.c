
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
#include "systime.h"
#include <Timer_hard.h>

#include <silan_syscfg.h>
#include <silan_iomux.h>

#define CTRL_LED1               IO_CONFIG_PB8
#define CTRL_LED2               IO_CONFIG_PA6

//----LED_init-----------------------------------------------------------------
//���ܣ�ʹ��gpio�˿���������ģʽ
//������void
//���أ�void
//-----------------------------------------------------------------------------
void Led_Init(void)
{
    silan_io_func_config(CTRL_LED2, IO_FUNC_GPIO);
    silan_io_output(CTRL_LED2);
}

void Led_Goggle(void)
{
    silan_io_toggle(CTRL_LED2);
}

#include <uart.h>

void uartTest(void)
{

//    char *str =  "hello world\r\n";
    //ModuleInstall_UART(CN_UART1);                               //3
    //uart_dev = Driver_OpenDevice("UART1",D_RDWR,0);
//    DevOpen("UART1",D_RDWR,);
//    Driver_CtrlDevice(uart_dev, CN_UART_SET_BAUD,115200,0);    //6
//    Driver_CtrlDevice(uart_dev, CN_UART_START,0,0);
//
//    Driver_WriteDevice(uart_dev,(ptu32_t)str,strlen(str),0,CN_BLOCK_BUFFER,10*mS);
}
static u8 iCount = 0;
u32 timerTest(ptu32_t i)
{

    Led_Goggle();
    printf("app timer1 interrupt %d\r\n",iCount++);
    return 0;
}

u32 timerTest1(ptu32_t i)
{

    Led_Goggle();
    printf("app timer2 interrrupt %d\r\n",iCount++);
    return 0;
}

void testFortimer(void)
{
    static u8 byCount = 0;
    ptu32_t TimerHandle = HardTimer_Alloc(timerTest);
    u32 CrystalFreq = HardTimer_GetFreq(TimerHandle);          //��ȡ��ʱ��Ƶ��
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,CrystalFreq*10); //���ö�ʱ����,�����õ����ڣ�����ֵ��,1s
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETRELOAD,true);   //reloadģʽ
    HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);       //�ж�ʹ��
//  HardTimer_Ctrl(TimerHandle,EN_TIMER_SETINTPRO,true);   //�ж���������,true����ʵʱ�ź�
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);     //ʹ�ܼ���
    printf("run timer1 usrisr \r\n");

    ptu32_t TimerHandle1 = HardTimer_Alloc(timerTest1);
    CrystalFreq = HardTimer_GetFreq(TimerHandle1);          //��ȡ��ʱ��Ƶ��
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETCYCLE,CrystalFreq*3); //���ö�ʱ����,�����õ����ڣ�����ֵ��,1s
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETRELOAD,true);   //reloadģʽ
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_ENINT,true);       //�ж�ʹ��
    //  HardTimer_Ctrl(TimerHandle1,EN_TIMER_SETINTPRO,true);   //�ж���������,true����ʵʱ�ź�
    HardTimer_Ctrl(TimerHandle1,EN_TIMER_STARTCOUNT,0);     //ʹ�ܼ���
    struct CPUTimerHandle*q = (struct CPUTimerHandle*)TimerHandle1;
    printf("run timer2 usrisr \r\n");
}
#include "cpu_peri_wat.h"
//test for watchDog
void testWatDog(void)
{
    WatDog_Install();//��װ���Ź�
    printf("wat init finish\r\n");
    //�������Ź�
    WDT_Start();
    printf("start wat dog finish\r\n");
}

//test for ad
#include "cpu_peri_adc.h"
void testADC(void)
{

     ModuleInstall_ADC(EN_ADC_VREF_EXT33);
    //��������
    appADC_Ctrl(0,CN_ADC_CHANNEL_PINCFG, CN_AD_CFG_PA3,0);
    //���ö�ͨ��
    appADC_Ctrl(0,CN_ADC_CHANNEL_SET, CN_AD_CHANNEL3,0);
}

void get_AD_Dat()
{
    u32 dat;
    appADC_Ctrl(0,CN_ADC_CHANNEL_RD_DAT, CN_AD_CHANNEL3,(u32)&dat);
    printf("AD:%d\r\n",dat);
}
extern u32 isr_vector[];
volatile u32 *p = (volatile u32 *)isr_vector;
ptu32_t djy_main(void)
{
      Led_Init();
    extern bool_t ModuleInstall_HardTimer(void);
    testFortimer();
    testWatDog();
    testADC();
    printf("%x\r\n",*p);
	while(1)
	{
	    Led_Goggle();
	    get_AD_Dat();
		printf("hello world!\r\n");
		Djy_EventDelay(100*mS);
	}
	return 0;
}
