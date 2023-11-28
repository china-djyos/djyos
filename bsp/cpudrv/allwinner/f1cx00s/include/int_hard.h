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
//����ģ��:�ж�ģ��
//���ߣ�liuwei
//�汾��V1.0.0
//�ļ�����: ���ж���صĴ��룬�����첽�ź���ʵʱ�ж�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-04-22
//   ����: liuwei
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __INT_HARD_H__
#define __INT_HARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef ucpu_t atom_high_t;
typedef u64 atom_low_t;

//���������ж�ģ��ĳ�����������Ӳ���йأ�Ҳ����������йء���ʵ���첽�źź�
//ʵʱ�жϾ�ʹ��irq��irq��Ϊ���жϿ��أ����첽�ź���û�ж�����ʹ�ܿ��أ���ֹ�첽
//�ź���ͨ����ֹ���б�����Ϊ�첽�źŵ��жϺ������ʵ�ֵģ���û�ж������첽�ź�
//ʹ�ܿ��ء������ֲ�߾�����fiqʵ��ʵʱ�жϣ���irqʵ���첽�źţ�������ж�����
//�첽�ź�ʹ�ܿ��ء�
#define CN_CFG_ASYN_TURNKEY    0   //�Ƿ�֧�ֶ������첽�ź�ʹ�ܿ���
#define CN_CFG_TRUNK_TURNKEY   1   //�Ƿ�֧�ֶ��������ж�ʹ�ܿ���

#define CN_CFG_REG_ATOM   0     //�����첽�źſ��ؼĴ�����ԭ���ԣ���������д
                                //�첽�źſ���ʱ���Ƿ����һ��ָ������ꡣ
                                //����:arm9��s3c2410,INTMSK��һ��32λ�Ĵ�����
                                //      ��32λcpu�ϣ�һ��ָ���ִ�ж���д��
                                //      ����Ϊ����ԭ���ԣ���������Ϊ1
                                //     arm9��s3c2416��INTMSK��64λ��������
                                //      32λ�Ĵ�������������ָ����ܶ���д��
                                //      �ʲ���ԭ���ԣ���������Ϊ0
                                //     ��cortex-m3�ϣ�ֱ�Ӷ�дBASEPRI�Ĵ�����
                                //      Ҳ��һ��ָ������꣬�߱�ԭ���ԣ���Ϊ1

struct INTC_REG                 //0x01c20400
{

    vu32 VECTOR;                // 00 ��ǰ��Ӧ���ж�������DJYOS���������жϺ�
    vu32 BASE_ADDR;             // 04 �ж��������ַ�Ĵ���
    vu32 PAD0;
    vu32 NMICTRL;               // 0c NMI ���ƼĴ���

    vu32 INTPND1;               // 10 interrupt pending reg
    vu32 INTPND2;               // 14 interrupt pending reg
    vu32 PAD1;
    vu32 PAD2;

    //�ر�ע�⣬INTEN1.0�κ�ʱ�򶼲�����1
    //��INTEN1.0=1������x���жϣ�����ӦIRQ+x�ж�һ�Σ�Ȼ�������ӦFIQ+x�жϣ���ʹ����
    //x�жϣ���Ȼ�������ӦFIQ+x�жϡ���x�ж��źŲ��������޸�INTEN1.0��ֵΪ0������IRQ
    //��Ӧx�жϣ�������FIQ
    vu32 INTEN1;                // 20 interrupt enable reg
    vu32 INTEN2;                // 24 interrupt enable reg
    vu32 PAD3;
    vu32 PAD4;

    vu32 INTMSK1;               // 30 interrupt mask reg
    vu32 INTMSK2;               // 34 interrupt mask reg
    vu32 PAD5;
    vu32 PAD6;

    vu32 INTRESPONSE1;          // 40 interrupt response reg
    vu32 INTRESPONSE2;          // 44 interrupt response reg
    vu32 PAD7;
    vu32 PAD8;

    vu32 INTFORCE1;             // 50 interrupt trig reg
    vu32 INTFORCE2;             // 54 interrupt trig reg
    vu32 PAD9;
    vu32 PAD10;

    vu32 PRIORITY1;             // 60 priority reg
    vu32 PRIORITY2;             // 64 priority reg
    vu32 PRIORITY3;             // 68 priority reg
    vu32 PRIORITY4;             // 6c priority reg

};

#define cn_noirq    0x80
#define cn_nofiq    0x40
#define cn_noint    0xc0

void __Int_ClearAllLine(void);
void __Int_InitHard(void);
void __Int_EngineReal(ufast_t ufl_line);
void __Int_EngineAsynSignal(ufast_t ufl_line);
void __Int_EngineAll(ufast_t ufl_line);

void int_enable_arm_int(void);
void int_disable_arm_int(void);
void __int_enable_irq(void);
void __int_disable_irq(void);
void __int_enable_fiq(void);
void __int_disable_fiq(void);

#ifdef __cplusplus
}
#endif

#endif //__INT_HARD_H__
