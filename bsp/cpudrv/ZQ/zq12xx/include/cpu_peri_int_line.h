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
#ifndef __int_line_h__
#define __int_line_h__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_SUBID_SOLO               (-1)
#define CN_SUBID_LMT                (32)
#define CN_SUBID_MISC_UART1          (0)
#define CN_SUBID_MISC_UART2          (1)
#define CN_SUBID_MISC_UART3          (2)
#define CN_SUBID_MISC_UART4          (3)
#define CN_SUBID_MISC_SPI1           (4)
#define CN_SUBID_MISC_SPI2           (5)
#define CN_SUBID_MISC_I2C1           (6)
#define CN_SUBID_MISC_I2C2           (7)
#define CN_SUBID_MISC_PDB            (9)
#define CN_SUBID_I2C_SPI1            (0)
#define CN_SUBID_I2C_SPI2            (1)
#define CN_SUBID_I2C_I2C1            (2)
#define CN_SUBID_I2C_I2C2            (3)
#define CN_SUBID_DMAC_CH0            (0)
#define CN_SUBID_DMAC_CH1            (1)
#define CN_SUBID_DMAC_CH2            (2)
#define CN_SUBID_DMAC_CH3            (3)
#define CN_SUBID_DMAC_CH4            (4)
#define CN_SUBID_DMAC_CH5            (5)
#define CN_SUBID_DMAC_CH6            (6)
#define CN_SUBID_DMAC_CH7            (7)
#define CN_SUBID_SPDIF_IN            (0)
#define CN_SUBID_SPDIF_OUT           (1)
#define CN_SUBID_I2S_O1              (0)
#define CN_SUBID_I2S_O2              (1)
#define CN_SUBID_I2S_O3              (2)
#define CN_SUBID_I2S_I1              (8)
#define CN_SUBID_I2S_I2              (9)
#define CN_SUBID_I2S_I3              (10)
#define CN_SUBID_TIMER_0             (0)
#define CN_SUBID_TIMER_1             (1)
#define CN_SUBID_TIMER_2             (2)
#define CN_SUBID_TIMER_3             (3)
#define CN_SUBID_TIMER_4             (4)
#define CN_SUBID_TIMER_MSO           (5)     /* millisecond */
#define CN_SUBID_TIMER_SO            (6)     /* second      */


#define CN_INT_LINE_SARADC                (0x00)
#define CN_INT_LINE_ECAP_EQEP             (0x01)
#define CN_INT_LINE_EPWM                  (0x02)
#define CN_INT_LINE_SPIFLASH              (0x03)
#define CN_INT_LINE_MISC                  (0x04)
#define CN_INT_LINE_UART                  (0x04)
#define CN_INT_LINE_SPI                   (0x04)    //���蹲���ж���
#define CN_INT_LINE_I2C                   (0x04)
#define CN_INT_LINE_PDM                   (0x05)
#define CN_INT_LINE_GPIO1                 (0x06)
#define CN_INT_LINE_GPIO2                 (0x07)
#define CN_INT_LINE_TIMER                 (0x08)
#define CN_INT_LINE_SDMAC                 (0x09)
#define CN_INT_LINE_SDMMC                 (0x0A)
#define CN_INT_LINE_IIR                   (0x0B)
#define CN_INT_LINE_SDIO                  (0x0C)
#define CN_INT_LINE_USBFS                 (0x0D)
#define CN_INT_LINE_USBHS                 (0x0E)
#define CN_INT_LINE_CXC                   (0x0F)
#define CN_INT_LINE_ADMAC                 (0X10)
#define CN_INT_LINE_SPDIF                 (0X11)
#define CN_INT_LINE_PCM                   (0x12)
#define CN_INT_LINE_IIS                   (0x13)
#define CN_INT_LINE_RTC                   (0x14)
#define CN_INT_LINE_PMU                   (0x15)
#define CN_INT_LINE_DMAC                  (0x16)
#define  CN_INT_LINE_DEFAULT23                        (23U)
#define  CN_INT_LINE_DEFAULT24                        (24U)
#define  CN_INT_LINE_DEFAULT25                        (25U)
#define  CN_INT_LINE_DEFAULT26                        (26U)
#define  CN_INT_LINE_DEFAULT27                        (27U)
#define  CN_INT_LINE_DEFAULT28                        (28U)
#define  CN_INT_LINE_DEFAULT29                        (29U)
#define  CN_INT_LINE_DEFAULT30                        (30U)
#define  CN_INT_LINE_CORET                            (31U)
#define CN_INT_LINE_LAST                 (0x1F)

#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

