/* translate from startup_at32f435_437.s */
#ifndef	__STARTUP_AT32F435_437_IRQ_H__
#define	__STARTUP_AT32F435_437_IRQ_H__
/* External Interrupts */
#define	CN_INT_LINE_WWDT                  0	/* Window Watchdog Timer                   */
#define	CN_INT_LINE_PVM                   1	/* PVM through EXINT Line detect           */
#define	CN_INT_LINE_TAMP_STAMP            2	/* Tamper and TimeStamps through the EXINT line */
#define	CN_INT_LINE_ERTC_WKUP             3	/* ERTC Wakeup through the EXINT line      */
#define	CN_INT_LINE_FLASH                 4	/* Flash                                   */
#define	CN_INT_LINE_CRM                   5	/* CRM                                     */
#define	CN_INT_LINE_EXINT0                6	/* EXINT Line 0                            */
#define	CN_INT_LINE_EXINT1                7	/* EXINT Line 1                            */
#define	CN_INT_LINE_EXINT2                8	/* EXINT Line 2                            */
#define	CN_INT_LINE_EXINT3                9	/* EXINT Line 3                            */
#define	CN_INT_LINE_EXINT4                10	/* EXINT Line 4                            */
#define	CN_INT_LINE_EDMA_STREAM1          11	/* EDMA Stream 1                           */
#define	CN_INT_LINE_EDMA_STREAM2          12	/* EDMA Stream 2                           */
#define	CN_INT_LINE_EDMA_STREAM3          13	/* EDMA Stream 3                           */
#define	CN_INT_LINE_EDMA_STREAM4          14	/* EDMA Stream 4                           */
#define	CN_INT_LINE_EDMA_STREAM5          15	/* EDMA Stream 5                           */
#define	CN_INT_LINE_EDMA_STREAM6          16	/* EDMA Stream 6                           */
#define	CN_INT_LINE_EDMA_STREAM7          17	/* EDMA Stream 7                           */
#define	CN_INT_LINE_ADC1_2_3              18	/* ADC1 & ADC2 & ADC3                      */
#define	CN_INT_LINE_CAN1_TX               19	/* CAN1 TX                                 */
#define	CN_INT_LINE_CAN1_RX0              20	/* CAN1 RX0                                */
#define	CN_INT_LINE_CAN1_RX1              21	/* CAN1 RX1                                */
#define	CN_INT_LINE_CAN1_SE               22	/* CAN1 SE                                 */
#define	CN_INT_LINE_EXINT9_5              23	/* EXINT Line [9:5]                        */
#define	CN_INT_LINE_TMR1_BRK_TMR9         24	/* TMR1 Brake and TMR9                     */
#define	CN_INT_LINE_TMR1_OVF_TMR10        25	/* TMR1 Overflow and TMR10                 */
#define	CN_INT_LINE_TMR1_TRG_HALL_TMR11   26	/* TMR1 Trigger and hall and TMR11         */
#define	CN_INT_LINE_TMR1_CH               27	/* TMR1 Channel                            */
#define	CN_INT_LINE_TMR2_GLOBAL           28	/* TMR2                                    */
#define	CN_INT_LINE_TMR3_GLOBAL           29	/* TMR3                                    */
#define	CN_INT_LINE_TMR4_GLOBAL           30	/* TMR4                                    */
#define	CN_INT_LINE_I2C1_EVT              31	/* I2C1 Event                              */
#define	CN_INT_LINE_I2C1_ERR              32	/* I2C1 Error                              */
#define	CN_INT_LINE_I2C2_EVT              33	/* I2C2 Event                              */
#define	CN_INT_LINE_I2C2_ERR              34	/* I2C2 Error                              */
#define	CN_INT_LINE_SPI1                  35	/* SPI1                                    */
#define	CN_INT_LINE_SPI2_I2S2EXT          36	/* SPI2                                    */
#define	CN_INT_LINE_USART1                37	/* USART1                                  */
#define	CN_INT_LINE_USART2                38	/* USART2                                  */
#define	CN_INT_LINE_USART3                39	/* USART3                                  */
#define	CN_INT_LINE_EXINT15_10            40	/* EXINT Line [15:10]                      */
#define	CN_INT_LINE_ERTCALARM             41	/* RTC Alarm through EXINT Line            */
#define	CN_INT_LINE_OTGFS1_WKUP           42	/* OTGFS1 Wakeup from suspend              */
#define	CN_INT_LINE_TMR8_BRK_TMR12        43	/* TMR8 Brake and TMR12                    */
#define	CN_INT_LINE_TMR8_OVF_TMR13        44	/* TMR8 Overflow and TMR13                 */
#define	CN_INT_LINE_TMR8_TRG_HALL_TMR14   45	/* TMR8 Trigger and hall and TMR14         */
#define	CN_INT_LINE_TMR8_CH               46	/* TMR8 Channel                            */
#define	CN_INT_LINE_EDMA_STREAM8          47	/* EDMA Stream 8                           */
#define	CN_INT_LINE_XMC                   48	/* XMC                                     */
#define	CN_INT_LINE_SDIO1                 49	/* SDIO1                                   */
#define	CN_INT_LINE_TMR5_GLOBAL           50	/* TMR5                                    */
#define	CN_INT_LINE_SPI3_I2S3EXT          51	/* SPI3                                    */
#define	CN_INT_LINE_UART4                 52	/* UART4                                   */
#define	CN_INT_LINE_UART5                 53	/* UART5                                   */
#define	CN_INT_LINE_TMR6_DAC_GLOBAL       54	/* TMR6 & DAC                              */
#define	CN_INT_LINE_TMR7_GLOBAL           55	/* TMR7                                    */
#define	CN_INT_LINE_DMA1_CHANNEL1         56	/* DMA1 Channel 1                          */
#define	CN_INT_LINE_DMA1_CHANNEL2         57	/* DMA1 Channel 2                          */
#define	CN_INT_LINE_DMA1_CHANNEL3         58	/* DMA1 Channel 3                          */
#define	CN_INT_LINE_DMA1_CHANNEL4         59	/* DMA1 Channel 4                          */
#define	CN_INT_LINE_DMA1_CHANNEL5         60	/* DMA1 Channel 5                          */
#define	CN_INT_LINE_EMAC                  61	/* EMAC                                    */
#define	CN_INT_LINE_EMAC_WKUP             62	/* EMAC Wakeup                             */
#define	CN_INT_LINE_CAN2_TX               63	/* CAN2 TX                                 */
#define	CN_INT_LINE_CAN2_RX0              64	/* CAN2 RX0                                */
#define	CN_INT_LINE_CAN2_RX1              65	/* CAN2 RX1                                */
#define	CN_INT_LINE_CAN2_SE               66	/* CAN2 SE                                 */
#define	CN_INT_LINE_OTGFS1                67	/* OTGFS1                                  */
#define	CN_INT_LINE_DMA1_CHANNEL6         68	/* DMA1 Channel 6                          */
#define	CN_INT_LINE_DMA1_CHANNEL7         69	/* DMA1 Channel 7                          */
#define	CN_INT_LINE_70                    70	/* Reserved                                */
#define	CN_INT_LINE_USART6                71	/* USART6                                  */
#define	CN_INT_LINE_I2C3_EVT              72	/* I2C3 Event                              */
#define	CN_INT_LINE_I2C3_ERR              73	/* I2C3 Error                              */
#define	CN_INT_LINE_74                    74	/* Reserved                                */
#define	CN_INT_LINE_75                    75	/* Reserved                                */
#define	CN_INT_LINE_OTGFS2_WKUP           76	/* OTGFS2 Wakeup from suspend              */
#define	CN_INT_LINE_OTGFS2                77	/* OTGFS2                                  */
#define	CN_INT_LINE_DVP                   78	/* DVP                                     */
#define	CN_INT_LINE_79                    79	/* Reserved                                */
#define	CN_INT_LINE_80                    80	/* Reserved                                */
#define	CN_INT_LINE_FPU                   81	/* FPU                                     */
#define	CN_INT_LINE_UART7                 82	/* UART7                                   */
#define	CN_INT_LINE_UART8                 83	/* UART8                                   */
#define	CN_INT_LINE_SPI4                  84	/* SPI4                                    */
#define	CN_INT_LINE_85                    85	/* Reserved                                */
#define	CN_INT_LINE_86                    86	/* Reserved                                */
#define	CN_INT_LINE_87                    87	/* Reserved                                */
#define	CN_INT_LINE_88                    88	/* Reserved                                */
#define	CN_INT_LINE_89                    89	/* Reserved                                */
#define	CN_INT_LINE_90                    90	/* Reserved                                */
#define	CN_INT_LINE_QSPI2                 91	/* QSPI2                                   */
#define	CN_INT_LINE_QSPI1                 92	/* QSPI1                                   */
#define	CN_INT_LINE_93                    93	/* Reserved                                */
#define	CN_INT_LINE_DMAMUX                94	/* Reserved                                */
#define	CN_INT_LINE_95                    95	/* Reserved                                */
#define	CN_INT_LINE_96                    96	/* Reserved                                */
#define	CN_INT_LINE_97                    97	/* Reserved                                */
#define	CN_INT_LINE_98                    98	/* Reserved                                */
#define	CN_INT_LINE_99                    99	/* Reserved                                */
#define	CN_INT_LINE_100                   100	/* Reserved                                */
#define	CN_INT_LINE_101                   101	/* Reserved                                */
#define	CN_INT_LINE_SDIO2                 102	/* SDIO2                                   */
#define	CN_INT_LINE_ACC                   103	/* ACC                                     */
#define	CN_INT_LINE_TMR20_BRK             104	/* TMR20 Brake                             */
#define	CN_INT_LINE_TMR20_OVF             105	/* TMR20 Overflow                          */
#define	CN_INT_LINE_TMR20_TRG_HALL        106	/* TMR20 Trigger and hall                  */
#define	CN_INT_LINE_TMR20_CH              107	/* TMR20 Channel                           */
#define	CN_INT_LINE_DMA2_CHANNEL1         108	/* DMA2 Channel 1                          */
#define	CN_INT_LINE_DMA2_CHANNEL2         109	/* DMA2 Channel 2                          */
#define	CN_INT_LINE_DMA2_CHANNEL3         110	/* DMA2 Channel 3                          */
#define	CN_INT_LINE_DMA2_CHANNEL4         111	/* DMA2 Channel 4                          */
#define	CN_INT_LINE_DMA2_CHANNEL5         112	/* DMA2 Channel 5                          */
#define	CN_INT_LINE_DMA2_CHANNEL6         113	/* DMA2 Channel 6                          */
#define	CN_INT_LINE_DMA2_CHANNEL7         114	/* DMA2 Channel 7                          */

#define CN_INT_LINE_LAST                  115

#endif
