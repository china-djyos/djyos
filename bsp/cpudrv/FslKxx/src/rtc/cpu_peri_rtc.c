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
// =============================================================================

// �ļ���     ��cpu_peri_rtc.c
// ģ������: RTCģ�飬��ϵͳ��ʱʱ������ΪRTCʱ��ͨ��RTC���жϣ�����ϵͳʱ�䣬ʹϵͳ
//           ��ʱʱ���ڶϵ��������Ȼ��������
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 30/07.2014
// =============================================================================

#include "stdio.h"
#include "cpu_peri.h"
#include "time.h"
#include "project_config.h"    //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_CpuRtc(0);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip rtc"//CPU��rtc��������
//parent:"rtc"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                   //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"int","lock","time"//������������������������none����ʾ�������������
                                     //����������������������г�
//weakdependence:"none"              //��������������������������none����ʾ�������������
                                     //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                     //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RTC == false )
//#warning  " cpu_onchip_rtc  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RTC    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================

//static s64 last_rtc_cnt = 0;

// =============================================================================
// ���ܣ�RTC���ú������������RTCģ�鸴λ��Ȼ��ʹ����������������ؼĴ���
// ��������
// ���أ���
// =============================================================================
static void RTC_Configuration(void)
{
    u32 i;

#ifdef DEBUG
    printf("SRTC time invalid set...\r\n");
#endif
    RTC->CR   = RTC_CR_SWR_MASK;
    RTC->CR  &= ~RTC_CR_SWR_MASK;
    RTC->SR  &= 0x07;               // ͷ���������ʹ������Ч
    RTC->TSR = 0x00000000;              // ��TIF��־λ

    /*Enable the oscillator*/
    RTC->CR |= RTC_CR_OSCE_MASK;

    /*Wait to all the 32 kHz to stabilize, refer to the
     * crystal startup time in the crystal datasheet*/
    for(i=0;i<0x600000;i++);

    /*Set time compensation parameters*/
    RTC->TCR = RTC_TCR_CIR(0) | RTC_TCR_TCR(0);

    /*Configure the timer seconds and alarm registers*/
    RTC->TSR = 1;
    RTC->TAR = 0xFFFFFFFF;

    /*Enable the counter*/
    RTC->SR |= RTC_SR_TCE_MASK;
}

// =============================================================================
// ���ܣ�RTc�ж����ã��˴�ʹ�����ж�
// ��������
// ���أ���
// =============================================================================
//static void RTC_IntConfig(void)
//{
//  // ʹ�����ж�
//  RTC->IER |= RTC_IER_TSIE_MASK;
//}

// =============================================================================
// ���ܣ�RTCʵʱʱ��ģ���жϷ��������ж��и���ϵͳʱ�䣬���ж�����ʱʵ�������жϣ�
//       ÿ�ж�һ�Σ�ϵͳʱ�������һ��
// �������ж��ߺ�
// ���أ�0
// =============================================================================
//static u32 RTC_ISR(ufast_t rtc_int_line)
//{
//  s64 rtc_cnt;
//  // FSL���ж�û�б�־λ����ȡ���ж��Ƿ�ʹ��
//  if(RTC_IER_TSIE_MASK)
//  {
//      rtc_cnt = RTC->TSR;
//      Tm_IncSecond(rtc_cnt - last_rtc_cnt);
//      last_rtc_cnt = rtc_cnt;
//  }
//
//  if(RTC->SR & RTC_SR_TIF_MASK)       // TIF�жϣ���������ʱ��ر�ȡ��
//  {
//      RTC->SR &= ~RTC_SR_TCE_MASK;
//      RTC->TSR = last_rtc_cnt;            // ��TIF��־�������¸���TSR
//      RTC->SR |= RTC_SR_TCE_MASK;
//  }
//  return 0;
//}

// =============================================================================
// ���ܣ�RTCʵʱʱ�Ӹ��£���RTCģ���е�ʱ�����Ϊ����ʱ�䣬������ģ�����
// ������DateTime,ϵͳʱ�䣬�軻�����
// ���أ�1
// =============================================================================
bool_t RTC_SetTime(s64 time)
{
    u32 rtc_cnt;

    rtc_cnt = (u32)(time / 1000000) ;
    RTC->SR &= ~RTC_SR_TCE_MASK;        // ���������
    RTC->TSR = rtc_cnt;                 // �����������
    RTC->SR |= RTC_SR_TCE_MASK;         // ʹ���������

    return true;
}

// =============================================================================
// ���ܣ���ȡRTCʱ�䣬�ȶ�ȡ������Ȼ����ɸ�������ʱ��󷵻�
// ������DateTime,���ص�ʱ��
// ���أ�true
// =============================================================================
bool_t RTC_GetTime(s64 *time)
{
    s64 rtc_cnt;
    rtc_cnt = RTC->TSR;

    *time = rtc_cnt * 1000000;  //ת����Ϊ΢����
    return true;
}

// =============================================================================
// ���ܣ���ʼ��RTCʵʱʱ��ģ�顣
// ������ģ���ʼ������û�в���
// ���أ�true = �ɹ���ʼ����false = ��ʼ��ʧ��
// =============================================================================
ptu32_t ModuleInstall_RTC(ptu32_t para)
{
    // ʹ��RTCʱ�ӣ����ṩϵͳʱ��
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL_MASK;

    // �ж��Ƿ�ʱ����Ч�����ж��Ƿ���Ҫ��ʼ��ʱ��
    // һ���ǵ�ص����������λ����Ҫ���³�ʼ��
    if(RTC->SR & RTC_SR_TIF_MASK)
    {
        RTC_Configuration();
    }

    Rtc_RegisterDev(RTC_GetTime,RTC_SetTime);

    return true;
}
