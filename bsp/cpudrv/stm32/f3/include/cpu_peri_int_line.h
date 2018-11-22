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
/******  STM32 specific Interrupt Numbers **********************************************************************/
#define  CN_INT_LINE_WWDG                             0      /*!< Window WatchDog Interrupt                                         */
#define  CN_INT_LINE_PVD                              1      /*!< PVD through EXTI Line detection Interrupt                         */
#define  CN_INT_LINE_TAMP_STAMP                       2      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
#define  CN_INT_LINE_RTC_WKUP                         3      /*!< RTC Wakeup interrupt through the EXTI line                        */
#define  CN_INT_LINE_FLASH                            4      /*!< FLASH global Interrupt                                            */
#define  CN_INT_LINE_RCC                              5      /*!< RCC global Interrupt                                              */
#define  CN_INT_LINE_EXTI0                            6      /*!< EXTI Line0 Interrupt                                              */
#define  CN_INT_LINE_EXTI1                            7      /*!< EXTI Line1 Interrupt                                              */
#define  CN_INT_LINE_EXTI2                            8      /*!< EXTI Line2 Interrupt                                              */
#define  CN_INT_LINE_EXTI3                            9      /*!< EXTI Line3 Interrupt                                              */
#define  CN_INT_LINE_EXTI4                            10     /*!< EXTI Line4 Interrupt                                              */
#define  CN_INT_LINE_DMA1_Channel1                    11     /*!< DMA1 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel2                    12     /*!< DMA1 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel3                    13     /*!< DMA1 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel4                    14     /*!< DMA1 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel5                    15     /*!< DMA1 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel6                    16     /*!< DMA1 Stream 5 global Interrupt                                    */
#define  CN_INT_LINE_DMA1_Channel7                    17     /*!< DMA1 Stream 6 global Interrupt                                    */
#define  CN_INT_LINE_ADC1_2                           18     /*!< ADC1 ADC2 and ADC3 global Interrupts                             */
#define  CN_INT_LINE_USB_HP_CAN_TX                    19     /*!< CAN1 TX Interrupt                                                 */
#define  CN_INT_LINE_USB_LP_CAN_RX0                   20     /*!< CAN1 RX0 Interrupt                                                */
#define  CN_INT_LINE_CAN_RX1                          21     /*!< CAN1 RX1 Interrupt                                                */
#define  CN_INT_LINE_CAN_SCE                          22     /*!< CAN1 SCE Interrupt                                                */
#define  CN_INT_LINE_EXTI9_5                          23     /*!< External Line[9:5] Interrupts                                     */
#define  CN_INT_LINE_TIM1_BRK_TIM15                   24     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
#define  CN_INT_LINE_TIM1_UP_TIM16                    25     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
#define  CN_INT_LINE_TIM1_TRG_COM_TIM17               26     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
#define  CN_INT_LINE_TIM1_CC                          27     /*!< TIM1 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_TIM2                             28     /*!< TIM2 global Interrupt                                             */
#define  CN_INT_LINE_TIM3                             29     /*!< TIM3 global Interrupt                                             */
#define  CN_INT_LINE_TIM4                             30     /*!< TIM4 global Interrupt                                             */
#define  CN_INT_LINE_I2C1_EV                          31     /*!< I2C1 Event Interrupt                                              */
#define  CN_INT_LINE_I2C1_ER                          32     /*!< I2C1 Error Interrupt                                              */
#define  CN_INT_LINE_I2C2_EV                          33     /*!< I2C2 Event Interrupt                                              */
#define  CN_INT_LINE_I2C2_ER                          34     /*!< I2C2 Error Interrupt                                              */
#define  CN_INT_LINE_SPI1                             35     /*!< SPI1 global Interrupt                                             */
#define  CN_INT_LINE_SPI2                             36     /*!< SPI2 global Interrupt                                             */
#define  CN_INT_LINE_USART1                           37     /*!< USART1 global Interrupt                                           */
#define  CN_INT_LINE_USART2                           38     /*!< USART2 global Interrupt                                           */
#define  CN_INT_LINE_USART3                           39     /*!< USART3 global Interrupt                                           */
#define  CN_INT_LINE_EXTI15_10                        40     /*!< External Line[15:10] Interrupts                                   */
#define  CN_INT_LINE_RTC_Alarm                        41     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
#define  CN_INT_LINE_USB_WakeUp                       42     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
#define  CN_INT_LINE_TIM8_BRK                         43     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
#define  CN_INT_LINE_TIM8_UP                          44     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
#define  CN_INT_LINE_TIM8_TRG_COM                     45     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#define  CN_INT_LINE_TIM8_CC                          46     /*!< TIM8 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_ADC3                             47     /*!< DMA1 Stream7 Interrupt                                            */

#define  CN_INT_LINE_SPI3                             51     /*!< SPI3 global Interrupt                                             */
#define  CN_INT_LINE_UART4                            52     /*!< UART4 global Interrupt                                            */
#define  CN_INT_LINE_UART5                            53     /*!< UART5 global Interrupt                                            */
#define  CN_INT_LINE_TIM6_DAC                         54     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
#define  CN_INT_LINE_TIM7                             55     /*!< TIM7 global interrupt                                             */
#define  CN_INT_LINE_DMA2_Channel1                    56     /*!< DMA2 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_DMA2_Channel2                    57     /*!< DMA2 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_DMA2_Channel3                    58     /*!< DMA2 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_DMA2_Channel4                    59     /*!< DMA2 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_DMA2_Channel5                    60     /*!< DMA2 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_ADC4                             61     /*!< Ethernet global Interrupt                                         */

#define  CN_INT_LINE_COMP1_2_3_IRQn                   64     /*!< CAN2 RX0 Interrupt                                                */
#define  CN_INT_LINE_COMP4_5_6_IRQn                   65     /*!< CAN2 RX1 Interrupt                                                */
#define  CN_INT_LINE_COMP7                            66     /*!< CAN2 SCE Interrupt                                                */

#define  CN_INT_LINE_USB_HP                           74     /*!< USB OTG HS End Point 1 Out global interrupt                       */
#define  CN_INT_LINE_USB_LP                           75     /*!< USB OTG HS End Point 1 In global interrupt                        */
#define  CN_INT_LINE_USBWakeUp_RMP                    76     /*!< USB OTG HS Wakeup through EXTI interrupt                          */

#define  CN_INT_LINE_FPU                              81     /*!< FPU global interrupt                                              */
#define  CN_INT_LINE_LAST                             66     //�ж��ܸ���
#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

