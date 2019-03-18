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
//���ߣ�huzb
//�汾��V1.0.0
//�ļ�����: scm610�����жϺ�
//����˵��:
//�޶���ʷ:
//
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_SCU      (0)
#define CN_INT_LINE_WDT      (1)
#define CN_INT_LINE_IIC0     (2)
#define CN_INT_LINE_IIC1     (3)
#define CN_INT_LINE_IIC2     (4)
#define CN_INT_LINE_UART0    (5)
#define CN_INT_LINE_UART1    (6)
#define CN_INT_LINE_UART2    (7)
#define CN_INT_LINE_UART3    (8)
#define CN_INT_LINE_UART4    (9)
#define CN_INT_LINE_UART5    (10)
#define CN_INT_LINE_UART6    (11)
#define CN_INT_LINE_UART7    (12)
#define CN_INT_LINE_SPI0     (13)
#define CN_INT_LINE_SPI1     (14)
#define CN_INT_LINE_SPI2     (15)
#define CN_INT_LINE_SPI3     (16)
#define CN_INT_LINE_TIMERA0  (17)
#define CN_INT_LINE_TIMERA1  (18)
#define CN_INT_LINE_TIMERB0  (19)
#define CN_INT_LINE_TIMERB1  (20)
#define CN_INT_LINE_TIMERC0  (21)
#define CN_INT_LINE_TIMERC1  (22)
#define CN_INT_LINE_TIMERD0  (23)
#define CN_INT_LINE_TIMERD1  (24)
#define CN_INT_LINE_DMA00    (25)
#define CN_INT_LINE_DMA01    (26)
#define CN_INT_LINE_DMA02    (27)
#define CN_INT_LINE_DMA03    (28)
#define CN_INT_LINE_DMA10    (29)
#define CN_INT_LINE_DMA11    (30)
#define CN_INT_LINE_DMA12    (31)
#define CN_INT_LINE_DMA13    (32)
#define CN_INT_LINE_DMA20    (33)
#define CN_INT_LINE_DMA21    (34)
#define CN_INT_LINE_DMA22    (35)
#define CN_INT_LINE_DMA23    (36)
#define CN_INT_LINE_DMA30    (37)
#define CN_INT_LINE_DMA31    (38)
#define CN_INT_LINE_DMA32    (39)
#define CN_INT_LINE_DMA33    (40)
#define CN_INT_LINE_ALM      (41)
#define CN_INT_LINE_PRD      (42)
#define CN_INT_LINE_ANACTL   (43)
#define CN_INT_LINE_GPIO     (44)
#define CN_OTG_INT_LINE      (45)
#define CN_INT_LINE_GMAC0    (46)
#define CN_INT_LINE_GMAC1    (47)
#define CN_INT_LINE_U7816    (47)
#define CN_INT_LINE_LAST     (47)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

