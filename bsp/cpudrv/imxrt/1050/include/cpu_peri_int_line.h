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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:中断模块
//作者:chj
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2018-05-07
//   作者: chj
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef CPU_PERI_INT_LINE_H_
#define CPU_PERI_INT_LINE_H_

#ifdef __cplusplus
extern "C" {
#endif



//#define  NonMaskableInt                   -14    /*!< 2 Non Maskable Interrupt                                          */
//#define  MemoryManagement                 -12    /*!< 4 Cortex-M7 Memory Management Interrupt                           */
//#define  BusFault                         -11    /*!< 5 Cortex-M7 Bus Fault Interrupt                                   */
//#define  UsageFault                       -10    /*!< 6 Cortex-M7 Usage Fault Interrupt                                 */
//#define  SVCall                           -5     /*!< 11 Cortex-M7 SV Call Interrupt                                    */
//#define  DebugMonitor                     -4     /*!< 12 Cortex-M7 Debug Monitor Interrupt                              */
//#define  PendSV                           -2     /*!< 14 Cortex-M7 Pend SV Interrupt                                    */
//#define  SysTick                          -1     /*!< 15 Cortex-M7 System Tick Interrupt                                */
/******  RT1052 specific Interrupt Numbers **********************************************************************/
#define  CN_INT_LINE_DMA0_DMA16                             0      /*!< Window WatchDog Interrupt                                         */
#define  CN_INT_LINE_DMA1_DMA17                             1      /*!< PVD through EXTI Line detection Interrupt                         */
#define  CN_INT_LINE_DMA2_DMA18                       2      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
#define  CN_INT_LINE_DMA3_DMA19                         3      /*!< RTC Wakeup interrupt through the EXTI line                        */
#define  CN_INT_LINE_DMA4_DMA20                            4      /*!< FLASH global Interrupt                                            */
#define  CN_INT_LINE_DMA5_DMA21                              5      /*!< RCC global Interrupt                                              */
#define  CN_INT_LINE_DMA6_DMA22                            6      /*!< EXTI Line0 Interrupt                                              */
#define  CN_INT_LINE_DMA7_DMA23                            7      /*!< EXTI Line1 Interrupt                                              */
#define  CN_INT_LINE_DMA8_DMA24                            8      /*!< EXTI Line2 Interrupt                                              */
#define  CN_INT_LINE_DMA9_DMA25                            9      /*!< EXTI Line3 Interrupt                                              */
#define  CN_INT_LINE_DMA10_DAM26                            10     /*!< EXTI Line4 Interrupt                                              */
#define  CN_INT_LINE_DMA11_DMA27                     11     /*!< DMA1 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_DMA12_DMA28                     12     /*!< DMA1 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_DMA13_DMA29                     13     /*!< DMA1 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_DMA14_DMA30                     14     /*!< DMA1 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_DMA15_DMA31                     15     /*!< DMA1 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_DMA_ERR                     16     /*!< DMA1 Stream 5 global Interrupt                                    */
#define  CN_INT_LINE_CTI0_ERR                     17     /*!< DMA1 Stream 6 global Interrupt                                    */
#define  CN_INT_LINE_CTR_ERR                              18     /*!< ADC1 ADC2 and ADC3 global Interrupts                             */
#define  CN_INT_LINE_CORE                          19     /*!< CAN1 TX Interrupt                                                 */
#define  CN_INT_LINE_LPUART1                         20     /*!< CAN1 RX0 Interrupt                                                */
#define  CN_INT_LINE_LRUART2                         21     /*!< CAN1 RX1 Interrupt                                                */
#define  CN_INT_LINE_LRUART3                         22     /*!< CAN1 SCE Interrupt                                                */
#define  CN_INT_LINE_LRUART4                          23     /*!< External Line[9:5] Interrupts                                     */
#define  CN_INT_LINE_LPUART5                    24     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
#define  CN_INT_LINE_LPUART6                    25     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
#define  CN_INT_LINE_LPUART7               26     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
#define  CN_INT_LINE_LPUART8                          27     /*!< TIM1 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_LPI2C1                             28     /*!< TIM2 global Interrupt                                             */
#define  CN_INT_LINE_LPI2C2                             29     /*!< TIM3 global Interrupt                                             */
#define  CN_INT_LINE_LPI2C3                            30     /*!< TIM4 global Interrupt                                             */
#define  CN_INT_LINE_LPI2C4                          31     /*!< I2C1 Event Interrupt                                              */
#define  CN_INT_LINE_LPSPI1                          32     /*!< I2C1 Error Interrupt                                              */
#define  CN_INT_LINE_LPSPI2                          33     /*!< I2C2 Event Interrupt                                              */
#define  CN_INT_LINE_LPSPI3                          34     /*!< I2C2 Error Interrupt                                              */
#define  CN_INT_LINE_LPSPI4                             35     /*!< SPI1 global Interrupt                                             */
#define  CN_INT_LINE_CAN1                             36     /*!< SPI2 global Interrupt                                             */
#define  CN_INT_LINE_CAN2                          37     /*!< USART1 global Interrupt                                           */
#define  CN_INT_LINE_FLEXRAM                           38     /*!< USART2 global Interrupt                                           */
#define  CN_INT_LINE_KPP                           39     /*!< USART3 global Interrupt                                           */
#define  CN_INT_LINE_TSC_DIG                        40     /*!< External Line[15:10] Interrupts                                   */
#define  CN_INT_LINE_GPR                        41     /*!< RTC Alarm (A and B) through EXTI Line Interrupt                   */
#define  CN_INT_LINE_LCDIF                      42     /*!< USB OTG FS Wakeup through EXTI line interrupt                     */
#define  CN_INT_LINE_CSI                   43     /*!< TIM8 Break Interrupt and TIM12 global interrupt                   */
#define  CN_INT_LINE_PXP                    44     /*!< TIM8 Update Interrupt and TIM13 global interrupt                  */
#define  CN_INT_LINE_WDOG2               45     /*!< TIM8 Trigger and Commutation Interrupt and TIM14 global interrupt */
#define  CN_INT_LINE_SNVS_HP_WRAPPER                          46     /*!< TIM8 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_SNVS_HP_WRAPPER_TZ                     47     /*!< DMA1 Stream7 Interrupt                                            */
#define  CN_INT_LINE_SNVS_LP_WRAPPER                              48     /*!< FMC global Interrupt                                              */
#define  CN_INT_LINE_CSU                           49     /*!< SDMMC1 global Interrupt                                           */
#define  CN_INT_LINE_DCP                             50     /*!< TIM5 global Interrupt                                             */
#define  CN_INT_LINE_DCP_VMI                             51     /*!< SPI3 global Interrupt                                             */
#define  CN_INT_LINE_RESERVED68                            52     /*!< UART4 global Interrupt                                            */
#define  CN_INT_LINE_TRNG                            53     /*!< UART5 global Interrupt                                            */
#define  CN_INT_LINE_SJC                         54     /*!< TIM6 global and DAC1&2 underrun error  interrupts                 */
#define  CN_INT_LINE_BEE                             55     /*!< TIM7 global interrupt                                             */
#define  CN_INT_LINE_SAI1                     56     /*!< DMA2 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_SAI2                     57     /*!< DMA2 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_SAI3_RX                     58     /*!< DMA2 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_SAI3_TX                     59     /*!< DMA2 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_SPDIF                     60     /*!< DMA2 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_ANATOP_EVENT0                              61     /*!< Ethernet global Interrupt                                         */
#define  CN_INT_LINE_ANATOP_EVENT1                         62     /*!< Ethernet Wakeup through EXTI line Interrupt                       */
#define  CN_INT_LINE_ANATOP_TAMP_LOW_HIGH                          63     /*!< CAN2 TX Interrupt                                                 */
#define  CN_INT_LINE_ANATOP_TEMP_PANIC                         64     /*!< CAN2 RX0 Interrupt                                                */
#define  CN_INT_LINE_USB_PHY1                         65     /*!< CAN2 RX1 Interrupt                                                */
#define  CN_INT_LINE_USB_PHY2                         66     /*!< CAN2 SCE Interrupt                                                */
#define  CN_INT_LINE_ADC1                           67     /*!< USB OTG FS global Interrupt                                       */
#define  CN_INT_LINE_ADC2                     68     /*!< DMA2 Stream 5 global interrupt                                    */
#define  CN_INT_LINE_DCDC                     69     /*!< DMA2 Stream 6 global interrupt                                    */
#define  CN_INT_LINE_RESERVED86                     70     /*!< DMA2 Stream 7 global interrupt                                    */
#define  CN_INT_LINE_RESERVED87                           71     /*!< USART6 global interrupt                                           */
#define  CN_INT_LINE_GPIO1_INT0                          72     /*!< I2C3 event interrupt                                              */
#define  CN_INT_LINE_GPIO1_INT1                          73     /*!< I2C3 error interrupt                                              */
#define  CN_INT_LINE_GPIO1_INT2                   74     /*!< USB OTG HS End Point 1 Out global interrupt                       */
#define  CN_INT_LINE_GPIO1_INT3                    75     /*!< USB OTG HS End Point 1 In global interrupt                        */
#define  CN_INT_LINE_GPIO1_INT4                      76     /*!< USB OTG HS Wakeup through EXTI interrupt                          */
#define  CN_INT_LINE_GPIO1_INT5                           77     /*!< USB OTG HS global interrupt                                       */
#define  CN_INT_LINE_GPIO1_INT6                             78     /*!< DCMI global interrupt                                             */
#define  CN_INT_LINE_GPIO1_INT7                             79     /*!< CRYP crypto global interrupt                                      */
#define  CN_INT_LINE_GPIO1_COMBINED_0_15                         80     /*!< Hash and Rng global interrupt                                     */
#define  CN_INT_LINE_GPIO1_COMBINED_16_31                              81     /*!< FPU global interrupt                                              */
#define  CN_INT_LINE_GPIO2_COMBINED_0_15                            82     /*!< UART7 global interrupt                                            */
#define  CN_INT_LINE_GPIO2_COMBINED_16_31                            83     /*!< UART8 global interrupt                                            */
#define  CN_INT_LINE_GPIO3_COMBINED_0_15                             84     /*!< SPI4 global Interrupt                                             */
#define  CN_INT_LINE_GPIO3_COMBINED_16_31                             85     /*!< SPI5 global Interrupt                                             */
#define  CN_INT_LINE_GPIO4_COMBINED_0_15                             86     /*!< SPI6 global Interrupt                                             */
#define  CN_INT_LINE_GPIO4_COMBINED_16_31                             87     /*!< SAI1 global Interrupt                                             */
#define  CN_INT_LINE_GPIO5_COMBINED_0_15                             88     /*!< LTDC global Interrupt                                             */
#define  CN_INT_LINE_GPIO5_COMBINED_16_31                          89     /*!< LTDC Error global Interrupt                                       */
#define  CN_INT_LINE_FLEXIO1                            90     /*!< DMA2D global Interrupt                                            */
#define  CN_INT_LINE_FLEXIO2                             91     /*!< SAI2 global Interrupt                                             */
#define  CN_INT_LINE_WDOG1                          92     /*!< Quad SPI global interrupt                                         */
#define  CN_INT_LINE_RTWDOG                           93     /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_EWM                              94     /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_CCM_1                          95     /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_CCM_2                          96     /*!< I2C4 Error Interrupt                                              */
#define  CN_INT_LINE_GPC                         97     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_SRC                         98     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_RESERVED115                      99    /*!< DFSDM1 Filter 0 global Interrupt                                   */
#define  CN_INT_LINE_GPT1                      100   /*!< DFSDM1 Filter 1 global Interrupt                                   */
#define  CN_INT_LINE_GPT2                      101  /*!< DFSDM1 Filter 2 global Interrupt                                    */
#define  CN_INT_LINE_PWM1_0                      102  /*!< DFSDM1 Filter 3 global Interrupt                                    */
#define  CN_INT_LINE_PWM1_1                           103   /*!< SDMMC2 global Interrupt                                            */
#define  CN_INT_LINE_PWM1_2                          104     /*!< CAN3 TX Interrupt                                                 */
#define  CN_INT_LINE_PWM1_3                         105     /*!< CAN3 RX0 Interrupt                                                */
#define  CN_INT_LINE_PWM1_FAULT                         106     /*!< CAN3 RX1 Interrupt                                                */
#define  CN_INT_LINE_RESERVED123                         107     /*!< CAN3 SCE Interrupt            */
#define  CN_INT_LINE_FLEXSPI                           108     /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_SEMC                              109     /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_USDHC1                          110     /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_USDHC2                          111     /*!< I2C4 Error Interrupt                                              */
#define  CN_INT_LINE_USB_OTG2                         112     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_USB_OTG1                         113     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_ENET                      114    /*!< DFSDM1 Filter 0 global Interrupt                                   */
#define  CN_INT_LINE_ENET_1588_TIMER                      115   /*!< DFSDM1 Filter 1 global Interrupt                                   */
#define  CN_INT_LINE_XBAR1_IRQ_0_1                      116  /*!< DFSDM1 Filter 2 global Interrupt                                    */
#define  CN_INT_LINE_XBAR1_IRQ_2_3                      117  /*!< DFSDM1 Filter 3 global Interrupt                                    */
#define  CN_INT_LINE_ADC_ETC_IRQ0                           118   /*!< SDMMC2 global Interrupt                                            */
#define  CN_INT_LINE_ADC_ETC_IRQ1                          119     /*!< CAN3 TX Interrupt                                                 */
#define  CN_INT_LINE_ADC_ETC_IRQ2                         120     /*!< CAN3 RX0 Interrupt                                                */
#define  CN_INT_LINE_ADC_ETC_ERR                         121     /*!< CAN3 RX1 Interrupt                                                */
#define  CN_INT_LINE_PIT                         122     /*!< CAN3 SCE Interrupt   */
#define  CN_INT_LINE_ACMP1                           123     /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_ACMP2                              124     /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_ACMP3                          125     /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_ACMP4                          126     /*!< I2C4 Error Interrupt                                              */
#define  CN_INT_LINE_RESERVED143                         127     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_RESERVED144                         128     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_ENC1                      129    /*!< DFSDM1 Filter 0 global Interrupt                                   */
#define  CN_INT_LINE_ENC2                     130   /*!< DFSDM1 Filter 1 global Interrupt                                   */
#define  CN_INT_LINE_ENC3                      131  /*!< DFSDM1 Filter 2 global Interrupt                                    */
#define  CN_INT_LINE_ENC4                      132  /*!< DFSDM1 Filter 3 global Interrupt                                    */
#define  CN_INT_LINE_TMR1                           133   /*!< SDMMC2 global Interrupt                                            */
#define  CN_INT_LINE_TMR2                          134     /*!< CAN3 TX Interrupt                                                 */
#define  CN_INT_LINE_TMR3                         135     /*!< CAN3 RX0 Interrupt                                                */
#define  CN_INT_LINE_TMR4                         136     /*!< CAN3 RX1 Interrupt                                                */
#define  CN_INT_LINE_PWM2_0                         137     /*!< CAN3 SCE Interrupt                                             */
#define  CN_INT_LINE_PWM2_1                         138     /*!< CAN3 SCE Interrupt   */
#define  CN_INT_LINE_PWM2_2                           139     /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_PWM2_3                              140     /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_PWM2_FAULT                          141     /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_PWM3_0                          142     /*!< I2C4 Error Interrupt                                              */
#define  CN_INT_LINE_PWM3_1                         143     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_PWM3_2                         144     /*!< SPDIF-RX global Interrupt  */
#define  CN_INT_LINE_PWM3_3                      145    /*!< DFSDM1 Filter 0 global Interrupt                                   */
#define  CN_INT_LINE_PWM3_FAULT                     146   /*!< DFSDM1 Filter 1 global Interrupt                                   */
#define  CN_INT_LINE_PWM4_0                      147  /*!< DFSDM1 Filter 2 global Interrupt                                    */
#define  CN_INT_LINE_PWM4_1                      148  /*!< DFSDM1 Filter 3 global Interrupt                                    */
#define  CN_INT_LINE_PWM4_2                           149   /*!< SDMMC2 global Interrupt                                            */
#define  CN_INT_LINE_PWM4_3                          150     /*!< CAN3 TX Interrupt                                                 */
#define  CN_INT_LINE_PWM4_FAULT                         151     /*!< CAN3 RX0 Interrupt                                                */
#define  CN_INT_LINE_RESERVED168                         152     /*!< CAN3 RX1 Interrupt                                                */
#define  CN_INT_LINE_RESERVED169                         153     /*!< CAN3 SCE Interrupt   */
#define  CN_INT_LINE_RESERVED170                         154     /*!< CAN3 SCE Interrupt                                             */
#define  CN_INT_LINE_RESERVED171                         155     /*!< CAN3 SCE Interrupt   */
#define  CN_INT_LINE_RESERVED172                           156     /*!< LP TIM1 interrupt                                                 */
#define  CN_INT_LINE_RESERVED173                              157     /*!< HDMI-CEC global Interrupt                                         */
#define  CN_INT_LINE_SJC_ARM_DEBUG                          158     /*!< I2C4 Event Interrupt                                              */
#define  CN_INT_LINE_NMI_WAKEUP                          159     /*!< I2C4 Error Interrupt                                              */

#define CN_INT_LINE_LAST                (159)
#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

