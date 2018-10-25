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
//�ļ�����: STM32F407�����жϺ�
//����˵��:
//�޶���ʷ:
//
//------------------------------------------------------
#ifndef __CPU_PERI_INT_LINE_H__
#define __CPU_PERI_INT_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_INT_LINE_WWDG            (0x00)
#define CN_INT_LINE_PVD             (0x01)
#define CN_INT_LINE_TAMPER          (0x02)
#define CN_INT_LINE_RTC_WKUP        (0x03)
#define CN_INT_LINE_FLASH           (0x04)
#define CN_INT_LINE_RCC             (0x05)
#define CN_INT_LINE_EXTI0           (0x06)
#define CN_INT_LINE_EXTI1           (0x07)
#define CN_INT_LINE_EXTI2           (0x08)
#define CN_INT_LINE_EXTI3           (0x09)
#define CN_INT_LINE_EXTI4           (0x0A)
#define CN_INT_LINE_DMA1_CH1        (0x0B)
#define CN_INT_LINE_DMA1_CH2        (0x0C)
#define CN_INT_LINE_DMA1_CH3        (0x0D)
#define CN_INT_LINE_DMA1_CH4        (0x0E)
#define CN_INT_LINE_DMA1_CH5        (0x0F)
#define CN_INT_LINE_DMA1_CH6        (0x10)
#define CN_INT_LINE_DMA1_CH7        (0x11)
#define CN_INT_LINE_ADC1_2          (0x12)
#define CN_INT_LINE_CAN_TX          (0x13)
#define CN_INT_LINE_CAN_RX0         (0x14)
#define CN_INT_LINE_CAN_RX1         (0x15)
#define CN_INT_LINE_CAN_SCE         (0x16)
#define CN_INT_LINE_EXTI9_5         (0x17)
#define CN_INT_LINE_TIM1_BRK_TIM15        (0x18)
#define CN_INT_LINE_TIM1_UP_TIM16         (0x19)
#define CN_INT_LINE_TIM1_TRG_COM_TIM17   (0x1A)
#define CN_INT_LINE_TIM1_CC         (0x1B)
#define CN_INT_LINE_TIM2            (0x1C)
#define CN_INT_LINE_TIM3            (0x1D)
#define CN_INT_LINE_TIM4            (0x1E)
#define CN_INT_LINE_I2C1_EV         (0x1F)
#define CN_INT_LINE_I2C1_ER         (0x20)
#define CN_INT_LINE_I2C2_EV         (0x21)
#define CN_INT_LINE_I2C2_ER         (0x22)
#define CN_INT_LINE_SPI1            (0x23)
#define CN_INT_LINE_SPI2            (0x24)
#define CN_INT_LINE_USART1          (0x25)
#define CN_INT_LINE_USART2          (0x26)
#define CN_INT_LINE_USART3          (0x27)
#define CN_INT_LINE_EXTI15_10       (0x28)
#define CN_INT_LINE_RTCAlarm        (0x29)
#define CN_INT_LINE_DFSDM1_FLT13    (0x2A)
#define CN_INT_LINE_TIM8_BRK        (0x2B)
#define CN_INT_LINE_TIM8_UP         (0x2C)
#define CN_INT_LINE_TIM8_TRG_COM    (0x2D)
#define CN_INT_LINE_TIM8_CC         (0x2E)
#define CN_INT_LINE_ADC3            (0x2F)
#define CN_INT_LINE_FMC             (0x30)
#define CN_INT_LINE_SDMMC1          (0x31)
#define CN_INT_LINE_TIM5            (0x32)
#define CN_INT_LINE_SPI3            (0x33)
#define CN_INT_LINE_UART4           (0x34)
#define CN_INT_LINE_UART5           (0x35)
#define CN_INT_LINE_TIM6_DAC        (0x36)
#define CN_INT_LINE_TIM7            (0x37)
#define CN_INT_LINE_DMA2_CH1        (0x38)
#define CN_INT_LINE_DMA2_CH2        (0x39)
#define CN_INT_LINE_DMA2_CH3        (0x3A)
#define CN_INT_LINE_DMA2_CH4        (0x3B)
#define CN_INT_LINE_DMA2_CH5        (0x3C)
#define CN_INT_LINE_DFSDM1_FLT0     (0x3D)
#define CN_INT_LINE_DFSDM1_FLT1     (0x3E)
#define CN_INT_LINE_DFSDM1_FLT2     (0x3F)
#define CN_INT_LINE_COMP            (0x40)
#define CN_INT_LINE_LPTIM1          (0x41)
#define CN_INT_LINE_LPTIM2          (0x42)
#define CN_INT_LINE_OTG_FS          (0x43)
#define CN_INT_LINE_DMA2_CH6        (0x44)
#define CN_INT_LINE_DMA2_CH7        (0x45)
#define CN_INT_LINE_LPUART1         (0x46)
#define CN_INT_LINE_QUADSPI         (0x47)
#define CN_INT_LINE_I2C3_EV         (0x48)
#define CN_INT_LINE_I2C3_ER         (0x49)
#define CN_INT_LINE_SAI1            (0x4A)
#define CN_INT_LINE_SAI2            (0x4B)
#define CN_INT_LINE_SWPMI1          (0x4C)
#define CN_INT_LINE_TSC             (0x4D)
#define CN_INT_LINE_LCD             (0x4E)
#define CN_INT_LINE_AES             (0x4F)
#define CN_INT_LINE_RNG             (0x50)
#define CN_INT_LINE_FPU             (0x51)
#define CN_INT_LINE_LAST            (0x51)

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_INT_LINE_H__

