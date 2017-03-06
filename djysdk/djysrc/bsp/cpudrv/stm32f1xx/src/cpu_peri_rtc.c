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
//����ģ�飺ʱ��ģ��
//���ߣ���ǫ
//�汾��V0.1.0
//�ļ�������DJYOS��Ӳ��RTC�����ļ�
//����˵����
//�޶���ʷ��
//2. ���ڣ�2012-06-26
//   ���ߣ�Merlin
//   �°汾�ţ�
//   �޸�˵�����¼���ʼ����djyos/clock.c�н��У����ļ����ڲ�������ƽ̨�е�RTCӲ��
//1. ����: 2012-05-02
//   ���ߣ���ǫ
//   �°汾�ţ�V0.1.0
//   �޸�˵����ԭʼ�汾
//------------------------------------------------------
#include "stddef.h"
#include "time.h"
#include "cpu_peri.h"

#define LSE_Flag_Reg 0xA5A5
#define LSI_Flag_Reg 0xA5A0
#define BAK_Reg   RTC_BKP_DR1

RTC_HandleTypeDef RTC_Handler;  //RTC���


static u32 RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((u32)RTC->CNTH << 16 ) | tmp) ;
}

static void RTC_WaitForLastTask(void)
{
  /* Loop until RTOFF flag is set */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
  }
}
static void RTC_SetCounter(u32 CounterValue)
{
    RTC->CRL |= RTC_CRL_CNF;
  /* Set RTC COUNTER MSB word */
  RTC->CNTH = CounterValue >> 16;
  /* Set RTC COUNTER LSB word */
  RTC->CNTL = (CounterValue & RTC_CNTL_RTC_CNT_Msk);
  RTC->CRL &= (uint16_t)~((uint16_t)RTC_CRL_CNF);
}

static void RTC_WaitForSynchro(void)
{
  /* Clear RSF flag */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* Loop until RSF flag is set */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
  }
}

// =============================================================================
// ���ܣ�RTCӲ���Ĵ�������
// ��������
// ���أ���
// =============================================================================
void RTC_Configuration(void)
{
    u32 timeout;

    RTC_Handler.Instance=RTC;
    RTC_Handler.Init.AsynchPrediv=32767;
    RTC_Handler.Init.OutPut=RTC_OUTPUTSOURCE_NONE;

    RCC->APB1ENR |=RCC_APB1ENR_BKPEN|RCC_APB1ENR_PWREN;
    HAL_PWR_EnableBkUpAccess();//����������ʹ��

    if(HAL_RTCEx_BKUPRead(&RTC_Handler,BAK_Reg)!=LSE_Flag_Reg)
    {
        RCC->BDCR=RCC_BDCR_BDRST;            //��λBDCR
        Djy_DelayUs(10);
        RCC->BDCR=0;                //������λ

        RCC->CSR|=RCC_CSR_LSION;                //LSI����ʹ��
        while(!(RCC->CSR&RCC_CSR_LSIRDY));    //�ȴ�LSI����

        RCC->BDCR |=RCC_BDCR_LSEON;//�����ⲿ����ʱ��
        timeout=0;
        while(timeout<1000)
        {
           if(RCC->BDCR|RCC_BDCR_LSERDY)
               break;
           timeout++;
           Djy_DelayUs(1000);
        }
        if(timeout==1000)//�ⲿʱ��������ʱѡ���ڲ�ʱ��LSI
        {
           RCC->BDCR &=~RCC_BDCR_RTCSEL_Msk;
           RCC->BDCR |=RCC_BDCR_RTCSEL_1;
           HAL_RTCEx_BKUPWrite(&RTC_Handler,BAK_Reg,LSI_Flag_Reg);
        }
        else//ѡ���ⲿʱ��LSE
        {
           RCC->BDCR &=~RCC_BDCR_RTCSEL_Msk;
           RCC->BDCR |=RCC_BDCR_RTCSEL_0;
           HAL_RTCEx_BKUPWrite(&RTC_Handler,BAK_Reg,LSE_Flag_Reg);
        }

        RCC->BDCR |=RCC_BDCR_RTCEN;
        HAL_RTC_Init(&RTC_Handler);
    }

}

// =============================================================================
// ���ܣ���ȡRTCʱ�䣬��ȡ��ʱ����S���õ�΢��Ҫ����1000000
// ������time,ʱ�䣬��λΪ΢��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_TimeGet(s64 *time)
{
    s64 time_s;
    u32 time_us;

    time_s=RTC_GetCounter();
    time_us = ((32767-RTC->DIVL)*1000000)/32768;
    *time = (s64)((1000000 * time_s)+(time_us));
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// ���ܣ�����RTCʱ�䣬RTC�����ʱ����S�����Ҫ����1000000
// ������time,ʱ�䣬��λΪ΢��
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t RTC_TimeUpdate(s64 time)
{
    u32 time_s;

    time_s = (u32)(time/1000000);
    RCC->APB1ENR |=RCC_APB1ENR_BKPEN|RCC_APB1ENR_PWREN;
    HAL_PWR_EnableBkUpAccess();//����������ʹ��
    RTC_WaitForSynchro();

    RTC_SetCounter(time_s);
    RTC_WaitForLastTask();

    return true;
}

// =============================================================================
// ���ܣ�RTCģ���ʼ��������ע��ӿں���������RTC
// ������para,��ʱ����
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    struct timeval tv;
    s64 rtc_time;

    RTC_Configuration();    // ����RTC

    RTC_TimeGet(&rtc_time);

    tv.tv_sec  = rtc_time/1000000;//us ---> s
    tv.tv_usec = rtc_time%1000000;

    settimeofday(&tv,NULL);
    if(!Rtc_RegisterDev(RTC_TimeGet,RTC_TimeUpdate))
        return false;
    return true;
}
