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
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��cpu_peri_systime.c
// ģ������: ϵͳ�ϵ�ʱ��ģ�飬��һ��RTT��32λ��ʱ����ʱϵͳ�ϵ�ʱ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/15.2015
// =============================================================================

#include "cpu_peri.h"
#include "systime.h"
// =============================================================================
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���

//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t MoudleInit_Systime(ptu32_t para);
//    MoudleInit_Systime(0)��
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_systime"      //��д�����������
//parent:"time"                       //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                      //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                        //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                            //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                        //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"time"                  //������������������������none����ʾ�������������
                                        //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                        //����������������������г�
//weakdependence:"none"                 //��������������������������none����ʾ�������������
                                        //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                        //����������������������г����á�,���ָ�
//mutex:"none"                          //������������������������none����ʾ�������������
                                        //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

// =============================================================================

#define TIM_CLK          (CN_CFG_PCLK1*2)  //36MHz
#define CLK_DIV          TIM_CLK/1000000   //
#define SYSTIME_CYCLE    (65536) //����Ϊ��󣬼�0xFFFF,���T= SYSTIME_FRE * 0xFFFF
#define SYS_TIM  TIM7            //sys�õ��Ķ�ʱ��

// =============================================================================
// ���ܣ���ȡ16λ��ʱ����ǰֵ
// ��������
// ���أ�16λ�Ķ�ʱ��ʱ��ֵ ms
// =============================================================================
u32 Systime_GetTime(void)
{
    return (SYS_TIM->CNT&0xffff);
}

// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ����ʹ��16λ��ʱ�Ӽ�������Ϊϵͳʱ��
// ��������
// ���أ���
// =============================================================================
static void SysTime_TcConfig(void)
{
    //SYS_TIMʱ��Ϊ2*36M

    RCC->APB1ENR |=RCC_APB1ENR_TIM7EN;

    SYS_TIM->CR1 = 0;            //
    SYS_TIM->CR2 = 0;
    SYS_TIM->CR1 |= TIM_CR1_ARPE;//�Զ���װ

    SYS_TIM->DIER &=~ TIM_DIER_UIE;//��ֹ�����ж�
    SYS_TIM->PSC = CLK_DIV-1;      //�����Ƶ
    SYS_TIM->ARR = 0xFFFF;         //��ʱ��Ԥװ��ֵ

    SYS_TIM->CR1 |= (TIM_CR1_CEN); //ʹ��TIMER
}
// =============================================================================
// ���ܣ�ϵͳʱ�ӳ�ʼ����ע��ϵͳʱ�ӣ�����ʼ��RTT�����ж�
// ������para
// ���أ�true,�ɹ�;false,ʧ��
// =============================================================================
bool_t MoudleInit_Systime(ptu32_t para)
{
    //��ʼ��TC������
    SysTime_TcConfig();

    //ע��ϵͳʱ�Ӻ���
    SysTimeConnect(Systime_GetTime,NULL,1000000,SYSTIME_CYCLE);
    return true;
}


