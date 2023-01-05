/* auto generated file, don't edit */
/* translate from GD32E10x/Include/gd32e10x.h */

#ifndef __IRQNO_GD32E103__
#define __IRQNO_GD32E103__

/* External Interrupts */
#define	CN_INT_LINE_WWDGT_IRQN                     	 0	/*!< window watchDog timer interrupt                          */
#define	CN_INT_LINE_LVD_IRQN                       	 1	/*!< LVD through EXTI line detect interrupt                   */
#define	CN_INT_LINE_TAMPER_IRQN                    	 2	/*!< tamper through EXTI line detect                          */
#define	CN_INT_LINE_RTC_IRQN                       	 3	/*!< RTC through EXTI line interrupt                          */
#define	CN_INT_LINE_FMC_IRQN                       	 4	/*!< FMC interrupt                                            */
#define	CN_INT_LINE_RCU_CTC_IRQN                   	 5	/*!< RCU and CTC interrupt                                    */
#define	CN_INT_LINE_EXTI0_IRQN                     	 6	/*!< EXTI line 0 interrupts                                   */
#define	CN_INT_LINE_EXTI1_IRQN                     	 7	/*!< EXTI line 1 interrupts                                   */
#define	CN_INT_LINE_EXTI2_IRQN                     	 8	/*!< EXTI line 2 interrupts                                   */
#define	CN_INT_LINE_EXTI3_IRQN                     	 9	/*!< EXTI line 3 interrupts                                   */
#define	CN_INT_LINE_EXTI4_IRQN                     	10	/*!< EXTI line 4 interrupts                                   */
#define	CN_INT_LINE_DMA0_CHANNEL0_IRQN             	11	/*!< DMA0 channel0 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL1_IRQN             	12	/*!< DMA0 channel1 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL2_IRQN             	13	/*!< DMA0 channel2 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL3_IRQN             	14	/*!< DMA0 channel3 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL4_IRQN             	15	/*!< DMA0 channel4 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL5_IRQN             	16	/*!< DMA0 channel5 interrupt                                  */
#define	CN_INT_LINE_DMA0_CHANNEL6_IRQN             	17	/*!< DMA0 channel6 interrupt                                  */
#define	CN_INT_LINE_ADC0_1_IRQN                    	18	/*!< ADC0 and ADC1 interrupt                                  */
#define	CN_INT_LINE_EXTI5_9_IRQN                   	23	/*!< EXTI[9:5] interrupts                                     */
#define	CN_INT_LINE_TIMER0_BRK_TIMER8_IRQN         	24	/*!< TIMER0 break and TIMER8 interrupts                       */
#define	CN_INT_LINE_TIMER0_UP_TIMER9_IRQN          	25	/*!< TIMER0 update and TIMER9 interrupts                      */
#define	CN_INT_LINE_TIMER0_TRG_CMT_TIMER10_IRQN    	26	/*!< TIMER0 trigger and commutation  and TIMER10 interrupts   */
#define	CN_INT_LINE_TIMER0_CHANNEL_IRQN            	27	/*!< TIMER0 channel capture compare interrupts                */
#define	CN_INT_LINE_TIMER1_IRQN                    	28	/*!< TIMER1 interrupt                                         */
#define	CN_INT_LINE_TIMER2_IRQN                    	29	/*!< TIMER2 interrupt                                         */
#define	CN_INT_LINE_TIMER3_IRQN                    	30	/*!< TIMER3 interrupts                                        */
#define	CN_INT_LINE_I2C0_EV_IRQN                   	31	/*!< I2C0 event interrupt                                     */
#define	CN_INT_LINE_I2C0_ER_IRQN                   	32	/*!< I2C0 error interrupt                                     */
#define	CN_INT_LINE_I2C1_EV_IRQN                   	33	/*!< I2C1 event interrupt                                     */
#define	CN_INT_LINE_I2C1_ER_IRQN                   	34	/*!< I2C1 error interrupt                                     */
#define	CN_INT_LINE_SPI0_IRQN                      	35	/*!< SPI0 interrupt                                           */
#define	CN_INT_LINE_SPI1_IRQN                      	36	/*!< SPI1 interrupt                                           */
#define	CN_INT_LINE_USART0_IRQN                    	37	/*!< USART0 interrupt                                         */
#define	CN_INT_LINE_USART1_IRQN                    	38	/*!< USART1 interrupt                                         */
#define	CN_INT_LINE_USART2_IRQN                    	39	/*!< USART2 interrupt                                         */
#define	CN_INT_LINE_EXTI10_15_IRQN                 	40	/*!< EXTI[15:10] interrupts                                   */
#define	CN_INT_LINE_RTC_ALARM_IRQN                 	41	/*!< RTC alarm interrupt                                      */
#define	CN_INT_LINE_USBFS_WKUP_IRQN                	42	/*!< USBFS wakeup interrupt                                   */
#define	CN_INT_LINE_TIMER7_BRK_TIMER11_IRQN        	43	/*!< TIMER7 break and TIMER11 interrupts                      */
#define	CN_INT_LINE_TIMER7_UP_TIMER12_IRQN         	44	/*!< TIMER7 update and TIMER12 interrupts                     */
#define	CN_INT_LINE_TIMER7_TRG_CMT_TIMER13_IRQN    	45	/*!< TIMER7 trigger and commutation and TIMER13 interrupts    */
#define	CN_INT_LINE_TIMER7_CHANNEL_IRQN            	46	/*!< TIMER7 channel capture compare interrupts                */
#define	CN_INT_LINE_EXMC_IRQN                      	48	/*!< EXMC global interrupt                                    */
#define	CN_INT_LINE_TIMER4_IRQN                    	50	/*!< TIMER4 global interrupt                                  */
#define	CN_INT_LINE_SPI2_IRQN                      	51	/*!< SPI2 global interrupt                                    */
#define	CN_INT_LINE_UART3_IRQN                     	52	/*!< UART3 global interrupt                                   */
#define	CN_INT_LINE_UART4_IRQN                     	53	/*!< UART4 global interrupt                                   */
#define	CN_INT_LINE_TIMER5_IRQN                    	54	/*!< TIMER5 global interrupt                                  */
#define	CN_INT_LINE_TIMER6_IRQN                    	55	/*!< TIMER6 global interrupt                                  */
#define	CN_INT_LINE_DMA1_CHANNEL0_IRQN             	56	/*!< DMA1 channel0 global interrupt                           */
#define	CN_INT_LINE_DMA1_CHANNEL1_IRQN             	57	/*!< DMA1 channel1 global interrupt                           */
#define	CN_INT_LINE_DMA1_CHANNEL2_IRQN             	58	/*!< DMA1 channel2 global interrupt                           */
#define	CN_INT_LINE_DMA1_CHANNEL3_IRQN             	59	/*!< DMA1 channel3 global interrupt                           */
#define	CN_INT_LINE_DMA1_CHANNEL4_IRQN             	60	/*!< DMA1 channel3 global interrupt                           */
#define	CN_INT_LINE_USBFS_IRQN                     	67	/*!< USBFS global interrupt                                   */

#define CN_INT_LINE_LAST                                68

#endif
