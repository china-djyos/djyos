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
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �����жϺ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009--9-28
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_WWDG            (0x00)  //WatchDog Interrupt
#define CN_INT_LINE_RESV0           (0x01)
#define CN_INT_LINE_RESV1           (0x02)
#define CN_INT_LINE_RTC             (0x03)  //RTC global Interrupt
#define CN_INT_LINE_FLASH           (0x04)  //FLASH global Interrupt
#define CN_INT_LINE_RCC             (0x05)  //RCC global Interrupt
#define CN_INT_LINE_PORTA           (0x06)  //PORTA Line Interrupt
#define CN_INT_LINE_PORTB           (0x07)  //PORTB Line Interrupt
#define CN_INT_LINE_PORTC           (0x08)  //PORTC Line Interrupt
#define CN_INT_LINE_PORTD           (0x09)  //PORTD Line Interrupt
#define CN_INT_LINE_RESV2           (0x0A)
#define CN_INT_LINE_DMA             (0x0B)  //DMA
#define CN_INT_LINE_RESV3           (0x0C)
#define CN_INT_LINE_RESV4           (0x0D)
#define CN_INT_LINE_RESV5           (0x0E)
#define CN_INT_LINE_RESV6           (0x0F)
#define CN_INT_LINE_RESV7           (0x10)
#define CN_INT_LINE_RESV8           (0x11)
#define CN_INT_LINE_ADC             (0x12)  //ADC
#define CN_INT_LINE_RESV9           (0x13)
#define CN_INT_LINE_RESV10          (0x14)
#define CN_INT_LINE_RESV11          (0x15)
#define CN_INT_LINE_RESV12          (0x16)
#define CN_INT_LINE_RESV13          (0x17)
#define CN_INT_LINE_TMR0            (0x18)  //TIM0
#define CN_INT_LINE_TMR1            (0x19)  //TIM1
#define CN_INT_LINE_RESV14          (0x1A)
#define CN_INT_LINE_RESV15          (0x1B)
#define CN_INT_LINE_TMR2            (0x1C)  //TIM2
#define CN_INT_LINE_TMR3            (0x1D)  //TIM3
#define CN_INT_LINE_TMR4            (0x1E)  //TIM4
#define CN_INT_LINE_I2C             (0x1F)  //I2C1 Interrupt
#define CN_INT_LINE_RESV16          (0x20)
#define CN_INT_LINE_RESV17          (0x21)
#define CN_INT_LINE_RESV18          (0x22)
#define CN_INT_LINE_SPI0            (0x23)  //SPI1 global Interrupt
#define CN_INT_LINE_SPI1            (0x24)  //SPI2 global Interrupt
#define CN_INT_LINE_USART0          (0x25)  //USART0 global Interrupt
#define CN_INT_LINE_USART1          (0x26)  //USART1 global Interrupt
#define CN_INT_LINE_USART2          (0x27)  //USART2 global Interrupt
#define CN_INT_LINE_RESV19          (0x28)
#define CN_INT_LINE_RESV20          (0x29)
#define CN_INT_LINE_RESV21          (0x2A)
#define CN_INT_LINE_RESV22          (0x2B)
#define CN_INT_LINE_RESV23          (0x2C)
#define CN_INT_LINE_RESV24          (0x2D)
#define CN_INT_LINE_RESV25          (0x2E)
#define CN_INT_LINE_RESV26          (0x2F)
#define CN_INT_LINE_RESV27          (0x30)
#define CN_INT_LINE_USB11           (0x31)  //USB11_Handler
#define CN_INT_LINE_RESV28          (0x32)
#define CN_INT_LINE_QSPI            (0x33)  //QSPI_Handler
#define CN_INT_LINE_RESV29          (0x34)
#define CN_INT_LINE_RESV30          (0x35)
#define CN_INT_LINE_RESV31          (0x36)
#define CN_INT_LINE_RESV32          (0x37)
#define CN_INT_LINE_RESV33          (0x38)
#define CN_INT_LINE_TMR5            (0x39)  //TIM5
#define CN_INT_LINE_RESV34          (0x3A)
#define CN_INT_LINE_AES             (0x3B)  //AES_Handler
#define CN_INT_LINE_LAST            (0x3B)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

