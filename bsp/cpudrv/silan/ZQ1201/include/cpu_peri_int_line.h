//----------------------------------------------------
// Copyright (c) 2014 SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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
//作者:lst
//版本：V1.0.0
//文件描述: 定义中断号
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-01-20
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef CPU_PERI_INT_LINE_H_
#define CPU_PERI_INT_LINE_H_

#ifdef __cplusplus
extern "C" {
#endif
#define PIC_SUBID_MISC_UART1          (0)
#define PIC_SUBID_MISC_UART2          (1)
#define PIC_SUBID_MISC_UART3          (2)
#define PIC_SUBID_MISC_UART4          (3)
#define PIC_SUBID_MISC_SPI1           (4)
#define PIC_SUBID_MISC_SPI2           (5)
#define PIC_SUBID_MISC_I2C1           (6)
#define PIC_SUBID_MISC_I2C2           (7)
#define PIC_SUBID_MISC_PDB            (9)
#define PIC_SUBID_I2C_SPI1            (0)
#define PIC_SUBID_I2C_SPI2            (1)
#define PIC_SUBID_I2C_I2C1            (2)
#define PIC_SUBID_I2C_I2C2            (3)
#define PIC_SUBID_DMAC_CH0            (0)
#define PIC_SUBID_DMAC_CH1            (1)
#define PIC_SUBID_DMAC_CH2            (2)
#define PIC_SUBID_DMAC_CH3            (3)
#define PIC_SUBID_DMAC_CH4            (4)
#define PIC_SUBID_DMAC_CH5            (5)
#define PIC_SUBID_DMAC_CH6            (6)
#define PIC_SUBID_DMAC_CH7            (7)
#define PIC_SUBID_SPDIF_IN            (0)
#define PIC_SUBID_SPDIF_OUT           (1)
#define PIC_SUBID_I2S_O1              (0)
#define PIC_SUBID_I2S_O2              (1)
#define PIC_SUBID_I2S_O3              (2)
#define PIC_SUBID_I2S_I1              (8)
#define PIC_SUBID_I2S_I2              (9)
#define PIC_SUBID_I2S_I3              (10)
#define PIC_SUBID_TIMER_0             (0)
#define PIC_SUBID_TIMER_1             (1)
#define PIC_SUBID_TIMER_2             (2)
#define PIC_SUBID_TIMER_3             (3)
#define PIC_SUBID_TIMER_4             (4)
#define PIC_SUBID_TIMER_MSO           (5)     /* millisecond */
#define PIC_SUBID_TIMER_SO            (6)     /* second      */
//#define  NonMaskableInt                   -14    /*!< 2 Non Maskable Interrupt                                          */
//#define  MemoryManagement                 -12    /*!< 4 Cortex-M7 Memory Management Interrupt                           */
//#define  BusFault                         -11    /*!< 5 Cortex-M7 Bus Fault Interrupt                                   */
//#define  UsageFault                       -10    /*!< 6 Cortex-M7 Usage Fault Interrupt                                 */
//#define  SVCall                           -5     /*!< 11 Cortex-M7 SV Call Interrupt                                    */
//#define  DebugMonitor                     -4     /*!< 12 Cortex-M7 Debug Monitor Interrupt                              */
//#define  PendSV                           -2     /*!< 14 Cortex-M7 Pend SV Interrupt                                    */
//#define  SysTick                          -1     /*!< 15 Cortex-M7 System Tick Interrupt                                */
/******  STM32 specific Interrupt Numbers **********************************************************************/
#define  CN_INT_LINE_DEFAULT0                         (0<<5)      /*!< Window WatchDog Interrupt                                         */
#define  CN_INT_LINE_DEFAULT1                         (1<<5)      /*!< PVD through EXTI Line detection Interrupt                         */
#define  CN_INT_LINE_SARADC                           (2<<5)      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
#define  CN_INT_LINE_SPIFLASH                         ((3<<5) + 0)      /*!< RTC Wakeup interrupt through the EXTI line                        */
#define  CN_INT_LINE_MISC_UART1                       ((4<<5) + 0)
#define  CN_INT_LINE_MISC_UART2                       ((4<<5) + 1)
#define  CN_INT_LINE_MISC_UART3                       ((4<<5) + 2)
#define  CN_INT_LINE_MISC_UART4                       ((4<<5) + 3)
#define  CN_INT_LINE_MISC_SPI1                        ((4<<5) + 4)
#define  CN_INT_LINE_MISC_SPI2                        ((4<<5) + 5)
#define  CN_INT_LINE_MISC_I2C1                        ((4<<5) + 6)
#define  CN_INT_LINE_MISC_I2C2                        ((4<<5) + 7)
#define  CN_INT_LINE_MISC_PDB                         ((4<<5) + 9)
#define  CN_INT_LINE_PDM                              (5<<5)      /*!< RCC global Interrupt                                              */
#define  CN_INT_LINE_GPIO1_1                          ((6<<5) + 0)
#define  CN_INT_LINE_GPIO1_2                          ((6<<5) + 1)
#define  CN_INT_LINE_GPIO1_3                          ((6<<5) + 2)
#define  CN_INT_LINE_GPIO1_4                          ((6<<5) + 3)
#define  CN_INT_LINE_GPIO1_5                          ((6<<5) + 4)
#define  CN_INT_LINE_GPIO1_6                          ((6<<5) + 5)
#define  CN_INT_LINE_GPIO1_7                          ((6<<5) + 6)
#define  CN_INT_LINE_GPIO1_8                          ((6<<5) + 7)
#define  CN_INT_LINE_GPIO1_9                          ((6<<5) + 8)
#define  CN_INT_LINE_GPIO1_10                         ((6<<5) + 9)
#define  CN_INT_LINE_GPIO1_11                         ((6<<5) + 10)
#define  CN_INT_LINE_GPIO1_12                         ((6<<5) + 11)
#define  CN_INT_LINE_GPIO1_13                         ((6<<5) + 12)
#define  CN_INT_LINE_GPIO1_14                         ((6<<5) + 13)
#define  CN_INT_LINE_GPIO1_15                         ((6<<5) + 14)
#define  CN_INT_LINE_GPIO1_16                         ((6<<5) + 15)
#define  CN_INT_LINE_GPIO1_17                          ((6<<5) + 16)
#define  CN_INT_LINE_GPIO1_18                          ((6<<5) + 17)
#define  CN_INT_LINE_GPIO1_19                          ((6<<5) + 18)
#define  CN_INT_LINE_GPIO1_20                          ((6<<5) + 19)
#define  CN_INT_LINE_GPIO1_21                          ((6<<5) + 20)
#define  CN_INT_LINE_GPIO1_22                          ((6<<5) + 21)
#define  CN_INT_LINE_GPIO1_23                          ((6<<5) + 22)
#define  CN_INT_LINE_GPIO1_24                          ((6<<5) + 23)
#define  CN_INT_LINE_GPIO1_25                          ((6<<5) + 24)
#define  CN_INT_LINE_GPIO1_26                         ((6<<5) + 25)
#define  CN_INT_LINE_GPIO1_27                         ((6<<5) + 26)
#define  CN_INT_LINE_GPIO1_28                         ((6<<5) + 27)
#define  CN_INT_LINE_GPIO1_29                         ((6<<5) + 28)
#define  CN_INT_LINE_GPIO1_30                         ((6<<5) + 29)
#define  CN_INT_LINE_GPIO1_31                         ((6<<5) + 30)
#define  CN_INT_LINE_GPIO1_32                         ((6<<5) + 31)
#define  CN_INT_LINE_GPIO2_1                          ((7<<5) + 0)
#define  CN_INT_LINE_GPIO2_2                          ((7<<5) + 1)
#define  CN_INT_LINE_GPIO2_3                          ((7<<5) + 2)
#define  CN_INT_LINE_GPIO2_4                          ((7<<5) + 3)
#define  CN_INT_LINE_GPIO2_5                          ((7<<5) + 4)
#define  CN_INT_LINE_GPIO2_6                          ((7<<5) + 5)
#define  CN_INT_LINE_GPIO2_7                          ((7<<5) + 6)
#define  CN_INT_LINE_GPIO2_8                          ((7<<5) + 7)
#define  CN_INT_LINE_GPIO2_9                          ((7<<5) + 8)
#define  CN_INT_LINE_GPIO2_10                         ((7<<5) + 9)
#define  CN_INT_LINE_GPIO2_11                         ((7<<5) + 10)
#define  CN_INT_LINE_GPIO2_12                         ((7<<5) + 11)
#define  CN_INT_LINE_GPIO2_13                         ((7<<5) + 12)
#define  CN_INT_LINE_GPIO2_14                         ((7<<5) + 13)
#define  CN_INT_LINE_GPIO2_15                         ((7<<5) + 14)
#define  CN_INT_LINE_GPIO2_16                         ((7<<5) + 15)
#define  CN_INT_LINE_GPIO2_17                          ((7<<5) + 16)
#define  CN_INT_LINE_GPIO2_18                          ((7<<5) + 17)
#define  CN_INT_LINE_GPIO2_19                          ((7<<5) + 18)
#define  CN_INT_LINE_GPIO2_20                          ((7<<5) + 19)
#define  CN_INT_LINE_GPIO2_21                          ((7<<5) + 20)
#define  CN_INT_LINE_GPIO2_22                          ((7<<5) + 21)
#define  CN_INT_LINE_GPIO2_23                          ((7<<5) + 22)
#define  CN_INT_LINE_GPIO2_24                          ((7<<5) + 23)
#define  CN_INT_LINE_GPIO2_25                          ((7<<5) + 24)
#define  CN_INT_LINE_GPIO2_26                         ((7<<5) + 25)
#define  CN_INT_LINE_GPIO2_27                         ((7<<5) + 26)
#define  CN_INT_LINE_GPIO2_28                         ((7<<5) + 27)
#define  CN_INT_LINE_GPIO2_29                         ((7<<5) + 28)
#define  CN_INT_LINE_GPIO2_30                         ((7<<5) + 29)
#define  CN_INT_LINE_GPIO2_31                         ((7<<5) + 30)
#define  CN_INT_LINE_GPIO2_32                         ((7<<5) + 31)
#define  CN_INT_LINE_TIMER1                            ((8<<5) + 0)      /*!< EXTI Line2 Interrupt                                              */
#define  CN_INT_LINE_TIMER2                            ((8<<5) + 1)
#define  CN_INT_LINE_TIMER3                            ((8<<5) + 3)
#define  CN_INT_LINE_TIMER4                            ((8<<5) + 4)
#define  CN_INT_LINE_TIMER_MSO                         ((8<<5) + 5)
#define  CN_INT_LINE_TIMER_SO                          ((8<<5) + 6)
#define  CN_INT_LINE_SDMAC                             (9<<5)      /*!< EXTI Line3 Interrupt                                              */
#define  CN_INT_LINE_SDMMC                             (10<<5)     /*!< EXTI Line4 Interrupt                                              */
#define  CN_INT_LINE_IIR                                (11<<5)     /*!< DMA1 Stream 0 global Interrupt                                    */
#define  CN_INT_LINE_SDIO                               (12<<5)     /*!< DMA1 Stream 1 global Interrupt                                    */
#define  CN_INT_LINE_USBFS                              (13<<5)     /*!< DMA1 Stream 2 global Interrupt                                    */
#define  CN_INT_LINE_USBHS                              (14<<5)     /*!< DMA1 Stream 3 global Interrupt                                    */
#define  CN_INT_LINE_CXC                                (15<<5)     /*!< DMA1 Stream 4 global Interrupt                                    */
#define  CN_INT_LINE_ADMAC                              (16<<5)     /*!< DMA1 Stream 5 global Interrupt                                    */
#define  CN_INT_LINE_SPDIF_IN                           ((17<<5) + 0)    /*!< DMA1 Stream 6 global Interrupt                                    */
#define  CN_INT_LINE_SPDIF_OUT                          ((17<<5) + 1)
#define  CN_INT_LINE_PCM                                ((18<<5) + 0)     /*!< ADC1 ADC2 and ADC3 global Interrupts                             */
#define  CN_INT_LINE_IIS                                ((19<<5) + 0)     /*!< CAN1 TX Interrupt                                                 */
#define  CN_INT_LINE_RTC                                ((20<<5) + 0)     /*!< CAN1 RX0 Interrupt                                                */
#define  CN_INT_LINE_I2S_O1                             ((21<<5) + 0)     /*!< CAN1 RX1 Interrupt                                                */
#define  CN_INT_LINE_I2S_O2                             ((21<<5) + 1)
#define  CN_INT_LINE_I2S_O3                             ((21<<5) + 2)
#define  CN_INT_LINE_I2S_I1                             ((21<<5) + 8)
#define  CN_INT_LINE_I2S_I2                             ((21<<5) + 9)
#define  CN_INT_LINE_I2S_I3                             ((21<<5) + 10)
#define  CN_INT_LINE_DMAC_CH0                           ((22<<5) + 0)     /*!< CAN1 SCE Interrupt                                                */
#define  CN_INT_LINE_DMAC_CH1                           ((22<<5) + 1)
#define  CN_INT_LINE_DMAC_CH2                           ((22<<5) + 2)
#define  CN_INT_LINE_DMAC_CH3                           ((22<<5) + 3)
#define  CN_INT_LINE_DMAC_CH4                           ((22<<5) + 4)
#define  CN_INT_LINE_DMAC_CH5                           ((22<<5) + 5)
#define  CN_INT_LINE_DMAC_CH6                           ((22<<5) + 6)
#define  CN_INT_LINE_DMAC_CH7                           ((22<<5) + 7)
#define  CN_INT_LINE_DEFAULT23                             (23<<5)     /*!< External Line[9:5] Interrupts                                     */
#define  CN_INT_LINE_DEFAULT24                             (24<<5)     /*!< TIM1 Break interrupt and TIM9 global interrupt                    */
#define  CN_INT_LINE_DEFAULT25                             (25<<5)     /*!< TIM1 Update Interrupt and TIM10 global interrupt                  */
#define  CN_INT_LINE_DEFAULT26                             (26<<5)     /*!< TIM1 Trigger and Commutation Interrupt and TIM11 global interrupt */
#define  CN_INT_LINE_DEFAULT27                             (27<<5)     /*!< TIM1 Capture Compare Interrupt                                    */
#define  CN_INT_LINE_DEFAULT28                             (28<<5)     /*!< TIM2 global Interrupt                                             */
#define  CN_INT_LINE_DEFAULT29                             (29<<5)     /*!< TIM3 global Interrupt                                             */
#define  CN_INT_LINE_DEFAULT30                             (30<<5)
#define  CN_INT_LINE_CORET                                 (31<<5)

#define CN_INT_LINE_LAST                                   (31)
#ifdef __cplusplus
}
#endif

#endif //__int_line_h__

