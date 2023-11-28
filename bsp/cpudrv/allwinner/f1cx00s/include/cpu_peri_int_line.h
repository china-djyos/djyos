//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms with or without
// modification are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT INDIRECT INCIDENTAL SPECIAL EXEMPLARY OR
// CONSEQUENTIAL DAMAGES (INCLUDING BUT NOT LIMITED TO PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE DATA OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY WHETHER IN
// CONTRACT STRICT LIABILITY OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE EVEN IF ADVISED OF THE
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
//����:lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-01-20
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef CPU_PERI_INT_LINE_H_
#define CPU_PERI_INT_LINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_NMI            0
#define CN_INT_LINE_UART0          1
#define CN_INT_LINE_UART1          2
#define CN_INT_LINE_UART2          3
#define CN_INT_LINE_OWA            5
#define CN_INT_LINE_CIR            6
#define CN_INT_LINE_TWI0           7
#define CN_INT_LINE_TWI1           8
#define CN_INT_LINE_TWI2           9
#define CN_INT_LINE_SPI0           10
#define CN_INT_LINE_SPI1           11
#define CN_INT_LINE_TIMER0         13
#define CN_INT_LINE_TIMER1         14
#define CN_INT_LINE_TIMER2         15
#define CN_INT_LINE_WDT            16
#define CN_INT_LINE_RSB            17
#define CN_INT_LINE_DMA            18
#define CN_INT_LINE_TOUCH          20
#define CN_INT_LINE_AUDIO_CODEC    21
#define CN_INT_LINE_KEYADC         22
#define CN_INT_LINE_SDC0           23
#define CN_INT_LINE_SDC1           24
#define CN_INT_LINE_USB_OTG        26
#define CN_INT_LINE_TVD            27
#define CN_INT_LINE_TVE            28
#define CN_INT_LINE_TCON           29
#define CN_INT_LINE_DE_FE          30
#define CN_INT_LINE_DE_BE          31
#define CN_INT_LINE_CSI            32
#define CN_INT_LINE_DE_INTERLACE   33
#define CN_INT_LINE_VE             34
#define CN_INT_LINE_DAUDIO         35
#define CN_INT_LINE_PIOD           38
#define CN_INT_LINE_PIOE           39
#define CN_INT_LINE_PIOF           40
#define CN_INT_LINE_LAST           40

#ifdef __cplusplus
}
#endif

#endif // CPU_PERI_INT_LINE_H_


