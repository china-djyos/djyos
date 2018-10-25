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
//���ߣ�Liuwei
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-05-19
//   ����: Liuwei
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_EINT0     (0)
#define CN_INT_LINE_EINT1     (1)
#define CN_INT_LINE_EINT2     (2)
#define CN_INT_LINE_EINT3     (3)
#define CN_INT_LINE_EINT4_7   (4)
#define CN_INT_LINE_EINT8_15  (5)
#define CN_INT_LINE_RSV6      (6)
#define CN_INT_LINE_BATT_FLT  (7)
#define CN_INT_LINE_TICK      (8)
#define CN_INT_LINE_WDT       (9)
#define CN_INT_LINE_TIMER0    (10)
#define CN_INT_LINE_TIMER1    (11)
#define CN_INT_LINE_TIMER2    (12)
#define CN_INT_LINE_TIMER3    (13)
#define CN_INT_LINE_TIMER4    (14)
#define CN_INT_LINE_UART2     (15)
#define CN_INT_LINE_LCD       (16)
#define CN_INT_LINE_DMA       (17)
#define CN_INT_LINE_UART3     (18)
#define CN_INT_LINE_RSV19     (19)
#define CN_INT_LINE_SDI1      (20)
#define CN_INT_LINE_SDI0      (21)
#define CN_INT_LINE_SPI0      (22)
#define CN_INT_LINE_UART1     (23)
#define CN_INT_LINE_NAND      (24)
#define CN_INT_LINE_USBD      (25)
#define CN_INT_LINE_USBH      (26)
#define CN_INT_LINE_IIC0      (27)
#define CN_INT_LINE_UART0     (28)
#define CN_INT_LINE_RSV29     (29)
#define CN_INT_LINE_RTC       (30)
#define CN_INT_LINE_ADC       (31)
#define CN_INT_LINE_LAST      (31)

#define CN_INT_MASK_ALL_LINE  (0xFFFFFFFF)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

