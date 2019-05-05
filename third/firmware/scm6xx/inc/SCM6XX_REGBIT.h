/*--------------------------------------------
描述：定义各模块寄存器bit
平台： SCM6XX
时间：2016-3-2
作者：张彦欣
-----------------------------------------------*/

#ifndef __REGBIT_H__
#define __REGBIT_H__ 


/* -------------------------------------
                 CLK
---------------------------------------*/
/*SYSCLK_CTRL*/
#define SYSCLK_CTRL_ANA_CLK_ENB          24
#define SYSCLK_CTRL_ANA_CLK_EN          (1 << 24)

#define SYSCLK_CTRL_ANA_CLK_MUXBF          21
#define SYSCLK_CTRL_ANA_CLK_MUX           (0x7 << 21)
#define SYSCLK_CTRL_ANA_CLK_MUX_LOSC      (0x0 << 21)
#define SYSCLK_CTRL_ANA_CLK_MUX_XLOSC     (0x1 << 21)
#define SYSCLK_CTRL_ANA_CLK_MUX_XHOSC_DIV4     (0x2 << 21)
#define SYSCLK_CTRL_ANA_CLK_MUX_HOSC_DIV16     (0x3 << 21)
#define SYSCLK_CTRL_ANA_CLK_MUX_PLL_DIV64     (0x4 << 21)

#define SYSCLK_CTRL_PHY_30M_ENB          20
#define SYSCLK_CTRL_PHY_30M_EN           (1 << 20)

#define SYSCLK_CTRL_HCLK_RDB           19
#define SYSCLK_CTRL_HCLK_RD           (1 << 19)


#define SYSCLK_CTRL_GMAC_PADCLK_ENB           18
#define SYSCLK_CTRL_GMAC_PADCLK_EN           (1 << 18)


#define SYSCLK_CTRL_XLOSC_ENB           17
#define SYSCLK_CTRL_XLOSC_EN           (1 << 17)

#define SYSCLK_CTRL_XHOSC_RDB           16
#define SYSCLK_CTRL_XHOSC_RD           (1 << 16)

#define SYSCLK_CTRL_GPIO_DBCLK_SELB        15
#define SYSCLK_CTRL_GPIO_DBCLK_SEL         (1 << 15)
#define SYSCLK_CTRL_GPIO_DBCLK_SEL_LOSC    (0 << 15)
#define SYSCLK_CTRL_GPIO_DBCLK_SEL_PCLK    (1 << 15)


#define SYSCLK_CTRL_RTC_CLK_SELB        14
#define SYSCLK_CTRL_RTC_CLK_SEL         (1 << 14)
#define SYSCLK_CTRL_RTC_CLK_SEL_XLOSC   (0 << 14)
#define SYSCLK_CTRL_RTC_CLK_SEL_EXTAL32   (1 << 14)


#define SYSCLK_CTRL_ADC_PLL_DIVBF        12
#define SYSCLK_CTRL_ADC_PLL_DIV          (3 << 12)
#define SYSCLK_CTRL_ADC_PLL_DIV_32       (0 << 12)
#define SYSCLK_CTRL_ADC_PLL_DIV_64       (1 << 12)
#define SYSCLK_CTRL_ADC_PLL_DIV_128      (2 << 12)
#define SYSCLK_CTRL_ADC_PLL_DIV_256      (3 << 12)

#define SYSCLK_CTRL_ADC_HOSC_DIVBF        10
#define SYSCLK_CTRL_ADC_HOSC_DIV          (3 << 10)
#define SYSCLK_CTRL_ADC_HOSC_DIV_8        (0 << 10)
#define SYSCLK_CTRL_ADC_HOSC_DIV_16       (1 << 10)
#define SYSCLK_CTRL_ADC_HOSC_DIV_32       (2 << 10)
#define SYSCLK_CTRL_ADC_HOSC_DIV_64       (3 << 10)

#define SYSCLK_CTRL_PLL_LOCKED_OB        9
#define SYSCLK_CTRL_PLL_LOCKED_O         (1 << 9)
#define SYSCLK_CTRL_PLL_LOCKED_O_HIGH    (1 << 9)
#define SYSCLK_CTRL_PLL_LOCKED_O_LOW     (0 << 9)

#define SYSCLK_CTRL_PCLK_SELB           8
#define SYSCLK_CTRL_PCLK_SEL            (1 << 8)
#define SYSCLK_CTRL_PCLK_SEL_HCLK_DIV2  (0 << 8)
#define SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4  (1 << 8)

#define SYSCLK_CTRL_HCLK_SELBF           6
#define SYSCLK_CTRL_HCLK_SEL             (3 << 6)
#define SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV8 (0 << 6)
#define SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV4 (1 << 6)
#define SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2 (2 << 6)
#define SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV1 (3 << 6)

#define SYSCLK_CTRL_OSC_SELB           5
#define SYSCLK_CTRL_OSC_SEL            (1 << 5)
#define SYSCLK_CTRL_OSC_SEL_HOSC       (0 << 5)
#define SYSCLK_CTRL_OSC_SEL_LOSC       (1 << 5)

#define SYSCLK_CTRL_PLL_SELB           4
#define SYSCLK_CTRL_PLL_SEL            (1 << 4)
#define SYSCLK_CTRL_PLL_SEL_OSC        (0 << 4)
#define SYSCLK_CTRL_PLL_SEL_PLL        (1 << 4)

#define SYSCLK_CTRL_XHOSC_ENB           3
#define SYSCLK_CTRL_XHOSC_EN            (1 << 3)

#define SYSCLK_CTRL_PLL_ENB           2
#define SYSCLK_CTRL_PLL_EN            (1 << 2)

#define SYSCLK_CTRL_LOSC_ENB           1
#define SYSCLK_CTRL_LOSC_EN            (1 << 1)

#define SYSCLK_CTRL_HOSC_ENB           0
#define SYSCLK_CTRL_HOSC_EN            (1 << 0)

/*HCLK_EN*/
#define HCLK_EN_MTXB             8
#define HCLK_EN_MTX             (1 << 8)

#define HCLK_EN_DMA3B             7
#define HCLK_EN_DMA3             (1 << 7)

#define HCLK_EN_DMA2B             6
#define HCLK_EN_DMA2             (1 << 6)

#define HCLK_EN_DMA1B             5
#define HCLK_EN_DMA1             (1 << 5)

#define HCLK_EN_DMA0B             4
#define HCLK_EN_DMA0             (1 << 4)

#define HCLK_EN_GMAC1B            3
#define HCLK_EN_GMAC1             (1 << 3)

#define HCLK_EN_GMAC0B            2
#define HCLK_EN_GMAC0             (1 << 2)

#define HCLK_EN_OTGB              1
#define HCLK_EN_OTG              (1 << 1)

#define HCLK_EN_FLASHB             0
#define HCLK_EN_FLASH              (1 << 0)

/*PCLK0_EN*/
#define PCLK0_EN_7816B             18
#define PCLK0_EN_7816             (1 << 18)

#define PCLK0_EN_PWM0B             17
#define PCLK0_EN_PWM0             (1 << 17)

#define PCLK0_EN_UART3B             16
#define PCLK0_EN_UART3             (1 << 16)

#define PCLK0_EN_UART2B             15
#define PCLK0_EN_UART2             (1 << 15)

#define PCLK0_EN_UART1B             14
#define PCLK0_EN_UART1             (1 << 14)

#define PCLK0_EN_UART0B             13
#define PCLK0_EN_UART0             (1 << 13)

#define PCLK0_EN_I2C1B             12
#define PCLK0_EN_I2C1              (1 << 12)

#define PCLK0_EN_I2C0B             11
#define PCLK0_EN_I2C0              (1 << 11)

#define PCLK0_EN_SPI_MST1B         10
#define PCLK0_EN_SPI_MST1              (1 << 10)

#define PCLK0_EN_SPI_MST0B         9
#define PCLK0_EN_SPI_MST0              (1 << 9)

#define PCLK0_EN_SPI1B         8
#define PCLK0_EN_SPI1              (1 << 8)

#define PCLK0_EN_SPI0B         7
#define PCLK0_EN_SPI0              (1 << 7)

#define PCLK0_EN_TESTB         6
#define PCLK0_EN_TEST              (1 << 6)

#define PCLK0_EN_GPIOB         5
#define PCLK0_EN_GPIO              (1 << 5)

#define PCLK0_EN_GPIO_DBB        4
#define PCLK0_EN_GPIO_DB              (1 << 4)

#define PCLK0_EN_RTCB         3
#define PCLK0_EN_RTC              (1 << 3)

#define PCLK0_EN_TIMERBB         2
#define PCLK0_EN_TIMERB              (1 << 2)

#define PCLK0_EN_TIMERAB         1
#define PCLK0_EN_TIMERA              (1 << 1)

#define PCLK0_EN_WDTB            0
#define PCLK0_EN_WDT              (1 << 0)

/*PCLK1_EN*/
#define PCLK1_EN_ADC_CLKB           15
#define PCLK1_EN_ADC_CLK           (1 << 15)

#define PCLK1_EN_ANA_W1B           14
#define PCLK1_EN_ANA_W1           (1 << 14)

#define PCLK1_EN_ANAB           13
#define PCLK1_EN_ANA           (1 << 13)

#define PCLK1_EN_PWM1B           12
#define PCLK1_EN_PWM1           (1 << 12)

#define PCLK1_EN_UART7B           11
#define PCLK1_EN_UART7           (1 << 11)

#define PCLK1_EN_UART6B           10
#define PCLK1_EN_UART6           (1 << 10)

#define PCLK1_EN_UART5B           9
#define PCLK1_EN_UART5           (1 << 9)

#define PCLK1_EN_UART4B           8
#define PCLK1_EN_UART4           (1 << 8)

#define PCLK1_EN_I2C2B           7
#define PCLK1_EN_I2C2           (1 << 7)

#define PCLK1_EN_SPI_MST3B           6
#define PCLK1_EN_SPI_MST3           (1 << 6)

#define PCLK1_EN_SPI_MST2B           5
#define PCLK1_EN_SPI_MST2           (1 << 5)

#define PCLK1_EN_SPI3B           4
#define PCLK1_EN_SPI3           (1 << 4)

#define PCLK1_EN_SPI2B           3
#define PCLK1_EN_SPI2           (1 << 3)

#define PCLK1_EN_CRCB           2
#define PCLK1_EN_CRC           (1 << 2)

#define PCLK1_EN_TIMERDB           1
#define PCLK1_EN_TIMERD           (1 << 1)

#define PCLK1_EN_TIMERCB           0
#define PCLK1_EN_TIMERC           (1 << 0)

//PLL_CTRL
#define PLL_CTRL_PLL_RSELBF        15
#define PLL_CTRL_PLL_RSEL        (0x7 << 15)
#define PLL_CTRL_PLL_RSEL_(n)    (n << 15)

#define PLL_CTRL_PLL_ICPSELBB        14
#define PLL_CTRL_PLL_ICPSELB        (1 << 14)
#define PLL_CTRL_PLL_ICPSELB_0      (0 << 14)
#define PLL_CTRL_PLL_ICPSELB_1      (1 << 14)

#define PLL_CTRL_PLL_ICPSELAB        13
#define PLL_CTRL_PLL_ICPSELA        (1 << 13)
#define PLL_CTRL_PLL_ICPSELA_0        (0 << 13)
#define PLL_CTRL_PLL_ICPSELA_1        (1 << 13)

#define PLL_CTRL_PLL_REFBF        11
#define PLL_CTRL_PLL_REF          (3 << 11)
#define PLL_CTRL_PLL_REF_XHOSC    (0 << 11)
#define PLL_CTRL_PLL_REF_HOSC     (1 << 11)
#define PLL_CTRL_PLL_REF_PAD50M   (2 << 11)

#define PLL_CTRL_PLL_OUTDIVBF        9
#define PLL_CTRL_PLL_OUTDIV          (3 << 9)
#define PLL_CTRL_PLL_OUTDIV_1        (0 << 9)
#define PLL_CTRL_PLL_OUTDIV_2        (1 << 9)
#define PLL_CTRL_PLL_OUTDIV_3        (2 << 9)
#define PLL_CTRL_PLL_OUTDIV_4        (3 << 9)

#define PLL_CTRL_PLL_FBDIVBF        2
#define PLL_CTRL_PLL_FBDIV         (0x7F << 2)
#define PLL_CTRL_PLL_FBDIV_(n)      ((n) << 2)

#define PLL_CTRL_PLL_REFDIVBF        0
#define PLL_CTRL_PLL_REFDIV          (3 << 0)
#define PLL_CTRL_PLL_REFDIV_1        (0 << 0)
#define PLL_CTRL_PLL_REFDIV_2        (1 << 0)
#define PLL_CTRL_PLL_REFDIV_3        (2 << 0)
#define PLL_CTRL_PLL_REFDIV_4        (3 << 0)


/* -------------------------------------
                 RMU
---------------------------------------*/

/*SOFT_RST0*/
#define SOFT_RST0_SYSB        8
#define SOFT_RST0_SYS     (1 << 8)

#define SOFT_RST0_DMA3B        7
#define SOFT_RST0_DMA3     (1 << 7)

#define SOFT_RST0_DMA2B        6
#define SOFT_RST0_DMA2     (1 << 6)

#define SOFT_RST0_DMA1B       5
#define SOFT_RST0_DMA1     (1 << 5)

#define SOFT_RST0_DMA0B        4
#define SOFT_RST0_DMA0     (1 << 4)

#define SOFT_RST0_GMAC1B       3
#define SOFT_RST0_GMAC1     (1 << 3)

#define SOFT_RST0_GMAC0B       2
#define SOFT_RST0_GMAC0     (1 << 2)

#define SOFT_RST0_OTGB       1
#define SOFT_RST0_OTG     (1 << 1)

#define SOFT_RST0_OTG_PHYB    0
#define SOFT_RST0_OTG_PHY     (1 << 0)

/*SOFT_RST1*/
#define SOFT_RST1_7816B        18
#define SOFT_RST1_7816     (1 << 18)

#define SOFT_RST1_PWM0B        17
#define SOFT_RST1_PWM0     (1 << 17)

#define SOFT_RST1_UART3B        16   
#define SOFT_RST1_UART3     (1 << 16)

#define SOFT_RST1_UART2B        15   
#define SOFT_RST1_UART2     (1 << 15)

#define SOFT_RST1_UART1B        14   
#define SOFT_RST1_UART1     (1 << 14)

#define SOFT_RST1_UART0B        13   
#define SOFT_RST1_UART0     (1 << 13)

#define SOFT_RST1_I2C1B        12   
#define SOFT_RST1_I2C1     (1 << 12)


#define SOFT_RST1_I2C0B        11   
#define SOFT_RST1_I2C0     (1 << 11)


#define SOFT_RST1_SPI1B        8  
#define SOFT_RST1_SPI1     (1 << 8)

#define SOFT_RST1_SPI0B        7 
#define SOFT_RST1_SPI0     (1 << 7)

#define SOFT_RST1_GPIOB       5  
#define SOFT_RST1_GPIO     (1 << 5)

#define SOFT_RST1_RTCB        3  
#define SOFT_RST1_RTC     (1 << 3)

#define SOFT_RST1_TIMERBB       2 
#define SOFT_RST1_TIMERB     (1 << 2)

#define SOFT_RST1_TIMERAB       1  
#define SOFT_RST1_TIMERA     (1 << 1)

#define SOFT_RST1_WDTB        0 
#define SOFT_RST1_WDT     (1 << 0)

/*SOFT_RST2*/
#define SOFT_RST2_PWM1B        12
#define SOFT_RST2_PWM1     (1 << 12)

#define SOFT_RST2_UART7B        11
#define SOFT_RST2_UART7     (1 << 11)

#define SOFT_RST2_UART6B        10
#define SOFT_RST2_UART6     (1 << 10)

#define SOFT_RST2_UART5B       9
#define SOFT_RST2_UART5     (1 << 9)

#define SOFT_RST2_UART4B        8   
#define SOFT_RST2_UART4     (1 << 8)

#define SOFT_RST2_I2C2B        7   
#define SOFT_RST2_I2C2     (1 << 7)

#define SOFT_RST2_SPI3B        4 
#define SOFT_RST2_SPI3     (1 << 4)

#define SOFT_RST2_SPI2B        3  
#define SOFT_RST2_SPI2     (1 << 3)

#define SOFT_RST2_CRCB        2   
#define SOFT_RST2_CRC     (1 << 2)

#define SOFT_RST2_TIMERDB        1   
#define SOFT_RST2_TIMERD     (1 << 1)

#define SOFT_RST2_TIMERCB        0   
#define SOFT_RST2_TIMERC     (1 << 0)


/*PER_RST*/
#define PER_RST_RTCB        4
#define PER_RST_RTC     (1 << 4)

#define PER_RST_SPI3B        3 
#define PER_RST_SPI3     (1 << 3)

#define PER_RST_SPI2B        2 
#define PER_RST_SPI2     (1 << 2)

#define PER_RST_SPI1B       1 
#define PER_RST_SPI1     (1 << 1)

#define PER_RST_SPI0B       0  
#define PER_RST_SPI0     (1 << 0)

/*RST_STS*/
#define RST_STS_PORB       1
#define RST_STS_POR        (1 << 1)

#define RST_STS_WDTB       0
#define RST_STS_WDT       (1 << 0)


/* -------------------------------------
                 SCU
---------------------------------------*/
/*SYS_CTRL*/
#define SYS_CTRL_FLASH_STDBY_ENB    6
#define SYS_CTRL_FLASH_STDBY_EN     (1 << 6)

#define SYS_CTRL_GMAC_MODEBF        3
#define SYS_CTRL_GMAC_MODE         (0x7 << 3)
#define SYS_CTRL_GMAC_MODE_MII     (0x0 << 3)
#define SYS_CTRL_GMAC_MODE_RMII    (0x4 << 3)

#define SYS_CTRL_VD_ENB            2
#define SYS_CTRL_VD_EN             (1 << 2)

#define SYS_CTRL_XLFDT_ENB            1
#define SYS_CTRL_XLFDT_EN             (1 << 1)

#define SYS_CTRL_XHFDT_ENB            0
#define SYS_CTRL_XHFDT_EN             (1 << 0)

/*SYS_STS*/
#define SYS_STS_CPU_LOCKUP_STSB      3
#define SYS_STS_CPU_LOCKUP_STS      (1 << 3)

#define SYS_STS_VD_STSB              2
#define SYS_STS_VD_STS              (1 << 2)

#define SYS_STS_XLFDT_STSB              1
#define SYS_STS_XLFDT_STS              (1 << 1)

#define SYS_STS_XHFDT_STSB              0
#define SYS_STS_XHFDT_STS              (1 << 0)

/*INT_EN*/
#define SCU_INT_EN_WAKEUP            (1 << 3)
#define SCU_INT_EN_VD                (1 << 2)
#define SCU_INT_EN_XLFDT             (1 << 1)
#define SCU_INT_EN_XHFDT             (1 << 0)

/*INT_STS*/
#define SCU_INT_STS_WAKEUP            (1 << 3)
#define SCU_INT_STS_VD                (1 << 2)
#define SCU_INT_STS_XLFDT             (1 << 1)
#define SCU_INT_STS_XHFDT             (1 << 0)

/*CACHE_HIT*/
#define CACHE_HIT_ENB                 18
#define CACHE_HIT_EN                  (1 << 18)

#define CACHE_HIT_RSTB                 17
#define CACHE_HIT_RST                  (1 << 17)

#define CACHE_HIT_OVFB                 16
#define CACHE_HIT_OVF                  (1 << 16)

#define CACHE_HIT_CNTBF                 0
#define CACHE_HIT_CNT                 0xFFFF

/*CACHE_MISS*/
#define CACHE_MISS_ENB                 18
#define CACHE_MISS_EN                  (1 << 18)

#define CACHE_MISS_RSTB                 17
#define CACHE_MISS_RST                  (1 << 17)

#define CACHE_MISS_OVFB                 16
#define CACHE_MISS_OVF                  (1 << 16)

#define CACHE_MISS_CNTBF                 0
#define CACHE_MISS_CNT                 0xFFFF

/*WAKEUP_EN*/
#define WAKEUP_EN_OTG_SUPB                 16
#define WAKEUP_EN_OTG_SUP                  (1 << 16)

#define WAKEUP_EN_UART0_RXB                 15
#define WAKEUP_EN_UART0_RX                  (1 << 15)

#define WAKEUP_EN_M4_SWB                 14
#define WAKEUP_EN_M4_SW                  (1 << 14)

#define WAKEUP_EN_WDT_INTB               13
#define WAKEUP_EN_WDT_INT                (1 << 13)

#define WAKEUP_EN_TIMERA1_INTB             12
#define WAKEUP_EN_TIMERA1_INT              (1 << 12)

#define WAKEUP_EN_RTC_PRDB             11
#define WAKEUP_EN_RTC_PRD              (1 << 11)

#define WAKEUP_EN_RTC_ALMB             10
#define WAKEUP_EN_RTC_ALM              (1 << 10)

#define WAKEUP_EN_GPIO9B             9
#define WAKEUP_EN_GPIO9              (1 << 9)

#define WAKEUP_EN_GPIO8B             8
#define WAKEUP_EN_GPIO8             (1 << 8)

#define WAKEUP_EN_GPIO7B             7
#define WAKEUP_EN_GPIO7              (1 << 7)

#define WAKEUP_EN_GPIO6B             6
#define WAKEUP_EN_GPIO6             (1 << 6)

#define WAKEUP_EN_GPIO5B             5
#define WAKEUP_EN_GPIO5              (1 << 5)

#define WAKEUP_EN_GPIO4B             4
#define WAKEUP_EN_GPIO4             (1 << 4)

#define WAKEUP_EN_GPIO3B             3
#define WAKEUP_EN_GPIO3              (1 << 3)

#define WAKEUP_EN_GPIO2B             2
#define WAKEUP_EN_GPIO2            (1 << 2)

#define WAKEUP_EN_GPIO1B             1
#define WAKEUP_EN_GPIO1              (1 << 1)

#define WAKEUP_EN_GPIO0B             0
#define WAKEUP_EN_GPIO0            (1 << 0)

/* -------------------------------------
                 FMC
---------------------------------------*/
/*STANDBY_EN*/
#define STANDBY_EN_DPD_ENB          1
#define STANDBY_EN_DPD_EN          (1 << 1)
#define STANDBY_EN_DPD_DIS         (0 << 1)

#define STANDBY_EN_STANDBY_ENB      0
#define STANDBY_EN_STANDBY_EN      (1 << 0)
#define STANDBY_EN_STANDBY_DIS     (0 << 0)

/*PROG_ERASE_FAIL*/
#define ERASE_START_FAILB      2
#define ERASE_START_FAIL       (1 << 2)
#define ERASE_START_SUCCESS    (0 << 2)

#define PROG_FAIL_1B           1
#define PROG_FAIL_1_SET           (1 << 1)
#define PROG_FAIL_1_CLR           (0 << 1)


#define PROG_FAIL_0B           0
#define PROG_FAIL_0_SET           (1 << 0)
#define PROG_FAIL_0_CLR           (0 << 0)

/*REDUNDANCY_INFO*/
#define REDUNDANCY_INFO_CHIP1_RR3_STS  ((UINT32)3 << 18)
#define REDUNDANCY_INFO_CHIP1_RR2_STS  ((UINT32)3 << 16)
#define REDUNDANCY_INFO_CHIP1_RR1_STS  ((UINT32)3 << 14)
#define REDUNDANCY_INFO_CHIP1_RR0_STS  ((UINT32)3 << 12)
#define REDUNDANCY_INFO_CHIP0_RR3_STS  ((UINT32)3 << 10)
#define REDUNDANCY_INFO_CHIP0_RR2_STS  ((UINT32)3 << 8)
#define REDUNDANCY_INFO_CHIP0_RR1_STS  ((UINT32)3 << 6)
#define REDUNDANCY_INFO_CHIP0_RR0_STS  ((UINT32)3 << 4)

#define REDUNDANCY_INFO_TRIM_FLAG      ((UINT32)1 << 2)
#define REDUNDANCY_INFO_CHIP1_DATA_RIGHT_FLAG      ((UINT32)1 << 1)
#define REDUNDANCY_INFO_CHIP0_DATA_RIGHT_FLAG      ((UINT32)1 << 0)

//FMC_STATUS
#define FMC_STATUS_BOOT_512KBB         4
#define FMC_STATUS_BOOT_512KB         (1 << 4)
#define FMC_STATUS_BOOT_0             (0 << 4)

#define FMC_STATUS_READ_BUSYB         0
#define FMC_STATUS_READ_BUSY          (1 << 0)
#define FMC_STATUS_READ_IDLE          (1 << 1)




/* -------------------------------------
                 TC
---------------------------------------*/

/*TC_TCFSR*/
#define TC_TCFSR_TCSELBF         0 
#define TC_TCFSR_TCSEL_TIMER     0
#define TC_TCFSR_TCSEL_CNT0      1
#define TC_TCFSR_TCSEL_CNT1      2
#define TC_TCFSR_TCSEL_CNT2      3
 
#define TC_TCFSR_CNTSRCBF        4 
#define TC_TCFSR_CNTSRC          (0xF << 4) 

#define TC_TCFSR_CNTSRC_EXIN1       (1 << 4) 
#define TC_TCFSR_CNTSRC_EXIN2       (2 << 4)  
#define TC_TCFSR_CNTSRC_EXIN3       (3 << 4)  
#define TC_TCFSR_CNTSRC_EXIN4       (4 << 4)  
#define TC_TCFSR_CNTSRC_32768       (5 << 4) 
#define TC_TCFSR_CNTSRC_RTCSEC      (6 << 4) 
#define TC_TCFSR_CNTSRC_RTCMIN      (7 << 4) 
#define TC_TCFSR_CNTSRC_LOWOUT      (0x8 << 4)
#define TC_TCFSR_CNTSRC_RXD0        (0xA << 4) 
#define TC_TCFSR_CNTSRC_RXD1        (0xB << 4)
#define TC_TCFSR_CNTSRC_RXD2        (0xC << 4)
#define TC_TCFSR_CNTSRC_RCLF        (0xD << 4)
#define TC_TCFSR_CNTSRC_LOWOV       (0xE << 4)


#define TC_TCFSR_CAPSRCBF        8 
#define TC_TCFSR_CAPSRC_EXIN1       (1 << 8) 
#define TC_TCFSR_CAPSRC_EXIN2       (2 << 8)  
#define TC_TCFSR_CAPSRC_EXIN3       (3 << 8)  
#define TC_TCFSR_CAPSRC_EXIN4       (4 << 8)  
#define TC_TCFSR_CAPSRC_32768       (5 << 8) 
#define TC_TCFSR_CAPSRC_RTCSEC      (6 << 8) 
#define TC_TCFSR_CAPSRC_RTCMIN      (7 << 8) 
#define TC_TCFSR_CAPSRC_RXD0        (0xA << 8) 
#define TC_TCFSR_CAPSRC_RXD1        (0xB << 8)
#define TC_TCFSR_CAPSRC_RXD2        (0xC << 8)
#define TC_TCFSR_CAPSRC_RCLF        (0xD << 8)
#define TC_TCFSR_CAPSRC_LOWOV       (0xE << 8)

/*TC_CR*/                         
#define TC_TCCR_TMENB           0 
#define TC_TCCR_TMEN           (1 << 0) 

#define TC_TCCR_TMRSTB          1 
#define TC_TCCR_TMRST           (1 << 1) 

/*TC_MCR*/                         
#define TC_MCR_MRENB          0
#define TC_MCR_MREN           (1 << 0)
#define TC_MCR_MRDIS          ~(1 << 0)

#define TC_MCR_MRRB           1
#define TC_MCR_CLEARTC        (1 << 1)
#define TC_MCR_NOTCLEARTC        ~(1 << 1)

#define TC_MCR_MRSB           2
#define TC_MCR_STOPTC         (1 << 2)
#define TC_MCR_NOTSTOPTC      ~(1 << 2)



/*TC_CCR*/                         
#define TC_CCR_CAPENB           0  

#define TC_CCR_CAPREB           1
#define TC_CCR_CAPRE            (1 << 1) 


#define TC_CCR_CAPFEB           2 
#define TC_CCR_CAPFE            (1 << 2) 



/*TC_EMR0*/
#define TC_EMR0_EMB           0  
#define TC_EMR0_EM_NONE       (0 << 0) 
#define TC_EMR0_EM_NPULSE     (1 << 0)  
#define TC_EMR0_EM_PPULSE     (2 << 0)  
#define TC_EMR0_EM_LEVEL      (3 << 0)  
 
/*interrupt type*/
#define TC_INTR_MR           (1 << 0)   /*发生match中断*/
#define TC_INTR_TCOV         (1 << 1)   /*溢出发生中断*/
#define TC_INTR_CAPRE        (1 << 2)   /*capture端口发生上升沿事件中断*/
#define TC_INTR_CAPFE        (1 << 3)   /*capture端口发生下升沿事件中断*/


/* -------------------------------------
                 TIMER2
---------------------------------------*/
//CTRL
#define TC2_CTRL_TC_ENB      0
#define TC2_CTRL_TC_EN       (1 << 0)
#define TC2_CTRL_TC_DIS      (0 << 0)

#define TC2_CTRL_TC_INT_TCLK_ENB      1
#define TC2_CTRL_TC_INT_TCLK_EN       (1 << 1)
#define TC2_CTRL_TC_INT_TCLK_DIS      (0 << 1)

#define TC2_CTRL_TC_INT_PCLK_ENB      2
#define TC2_CTRL_TC_INT_PCLK_EN       (1 << 2)
#define TC2_CTRL_TC_INT_PCLK_DIS      (0 << 2)

#define TC2_CTRL_TC_ADC_TRIG_ENB      3
#define TC2_CTRL_TC_ADC_TRIG_EN       (1 << 3)
#define TC2_CTRL_TC_ADC_TRIG_DIS      (0 << 3)

/* -------------------------------------
                 7816
---------------------------------------*/
/*U7816_CHNL*/
#define U7816_CHNL0_SRST_ENB           0   /*软件复位从器件0使能信号位*/
#define U7816_CHNL0_SRST_EN           (1 << U7816_CHNL0_SRST_ENB)   /*软件复位从器件使能*/
#define U7816_CHNL0_SRST_DIS          (0 << U7816_CHNL0_SRST_ENB)   /*软件复位从器件不使能*/

#define U7816_CHNL0_SCLK_ENB           1   /*从器件0时钟使能信号位*/
#define U7816_CHNL0_SCLK_EN           (1 << U7816_CHNL0_SCLK_ENB)   /*从器件时钟信号使能*/
#define U7816_CHNL0_SCLK_DIS          (0 << U7816_CHNL0_SCLK_ENB)   /*从器件时钟信号不使能*/

#define U7816_CHNL1_SRST_ENB             2   /*软件复位从器件1使能信号位*/
#define U7816_CHNL1_SRST_EN              (1 << U7816_CHNL1_SRST_ENB)    
#define U7816_CHNL1_SRST_DIS             (0 << U7816_CHNL1_SRST_ENB)    

#define U7816_CHNL1_SCLK_ENB             3   /*从器件1时钟使能信号位*/
#define U7816_CHNL1_SCLK_EN              (1 << U7816_CHNL1_SCLK_ENB)    
#define U7816_CHNL1_SCLK_DIS             (0 << U7816_CHNL1_SCLK_ENB)    

#define U7816_CHNL_CSELB             4   /*7816通道选择控制位*/
#define U7816_CHNL_CSEL_1            (1 << U7816_CHNL_CSELB)     /*通道1*/
#define U7816_CHNL_CSEL_0            (0 << U7816_CHNL_CSELB)     /*通道0*/

#define U7816_CHNL_ENB             5   /*7816使能控制位*/
#define U7816_CHNL_EN             (1 << U7816_CHNL_ENB)     /*使能*/
#define U7816_CHNL_DIS            (0 << U7816_CHNL_ENB)     /*不使能*/

/*U7816_DFRM*/
#define U7816_DFRM_RTXBF             0   /*自动重发次数控制位*/

#define U7816_DFRM_TMODB             3   /*通信方式选择位*/
#define U7816_DFRM_TMOD_T1            (1 << U7816_DFRM_TMODB)     /*T=1*/
#define U7816_DFRM_TMOD_T0            (0 << U7816_DFRM_TMODB)     /*T=0*/

#define U7816_DFRM_DICONVB             4   /*传输次序，编码方式选择位*/
#define U7816_DFRM_DICONV_INV          (1 << U7816_DFRM_DICONVB)     /*反向编码*/
#define U7816_DFRM_DICONV_NOR           (0 << U7816_DFRM_DICONVB)     /*正向编码*/

#define U7816_DFRM_PARB             5  /*奇偶校验类型选择*/
#define U7816_DFRM_PAR_ODD          (1 << U7816_DFRM_PARB)  /*奇校验*/
#define U7816_DFRM_PAR_EVEN         (0 << U7816_DFRM_PARB)  /*偶校验*/

#define U7816_DFRM_ERRWIDBF             6  /*error signal宽度选择*/
#define U7816_DFRM_ERRWID_1etu          (0 << U7816_DFRM_ERRWIDBF)  /*1 etu*/
#define U7816_DFRM_ERRWID_1nhalfetu        (1 << U7816_DFRM_PARB)  /*1.5etu*/
#define U7816_DFRM_ERRWID_2etu          (2 << U7816_DFRM_PARB)  /*2etu*/

#define U7816_DFRM_TXEGTBF             8  /*发送时插入的额外延迟时间*/
#define U7816_DFRM_TXEGT(x)            ((x) << U7816_DFRM_TXEGTBF)  /*发送时插入的额外延迟时间*/

/*U7816_FIFOCTRL*/
#define U7816_FIFOCTRL_TFLUSHB             0  /*发送FIFO的清空使能位*/
#define U7816_FIFOCTRL_TFLUSH              (1 << U7816_FIFOCTRL_TFLUSHB)  /*发送FIFO的清空使能*/

#define U7816_FIFOCTRL_RFLUSHB             1  /*接收FIFO的清空使能位*/
#define U7816_FIFOCTRL_RFLUSH              (1 << U7816_FIFOCTRL_RFLUSHB)  /*接收FIFO的清空使能*/

#define U7816_FIFOCTRL_RTRIGBF             2  /*接收FIFO的trigger值控制位域*/
#define U7816_FIFOCTRL_RTRIG               (7 << 2)  /*接收FIFO的trigger值控制位域*/
#define U7816_FIFOCTRL_RTRIG_1            (0 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为1*/
#define U7816_FIFOCTRL_RTRIG_2            (1 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为2*/
#define U7816_FIFOCTRL_RTRIG_3            (2 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为3*/
#define U7816_FIFOCTRL_RTRIG_4            (3 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为4*/
#define U7816_FIFOCTRL_RTRIG_5            (4 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为5*/
#define U7816_FIFOCTRL_RTRIG_6            (5 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为6*/
#define U7816_FIFOCTRL_RTRIG_7            (6 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为7*/
#define U7816_FIFOCTRL_RTRIG_8            (7 << U7816_FIFOCTRL_RTRIGBF)  /*接收FIFO的trigger值设置为8*/


/*U7816_SR*/
#define U7816_SR_RXERRB             0  /*接收数据奇偶校验错误标志位*/
#define U7816_SR_RXERR             (1 << U7816_SR_RXERRB)  /*接收数据奇偶校验错误*/

#define U7816_SR_RXEMPTB            1  /*接收FIFO空标志位*/
#define U7816_SR_RXEMPT            (1 << U7816_SR_RXEMPTB)  /*接收FIFO空*/

#define U7816_SR_RXFULLB            2  /*接收FIFO满标志位*/
#define U7816_SR_RXFULL            (1 << U7816_SR_RXFULLB)  /*接收FIFO满*/

#define U7816_SR_RXMTRGB            4  /*接收FIFO达到trigger值指示位*/
#define U7816_SR_RXMTRG             (1 << U7816_SR_RXMTRGB)  /*接收FIFO达到trigger值*/

#define U7816_SR_RXBUSYB            5  /*接收忙标志位*/
#define U7816_SR_RXBUSY            (1 << U7816_SR_RXBUSYB)  /*接收忙*/

#define U7816_SR_TXERRB             6  /*发送数据奇偶校验错误标志位*/
#define U7816_SR_TXERR             (1 << U7816_SR_TXERRB)  /*发送数据奇偶校验错误*/

#define U7816_SR_TXEMPTB            7  /*发送FIFO空标志位*/
#define U7816_SR_TXEMPT            (1 << U7816_SR_TXEMPTB)  /*发送FIFO空*/

#define U7816_SR_TXFULLB            8  /*发送FIFO满标志位*/
#define U7816_SR_TXFULL            (1 << U7816_SR_TXFULLB)  /*发送FIFO满*/

#define U7816_SR_TXBUSYB            9  /*发送忙标志位*/
#define U7816_SR_TXBUSY            (1 << U7816_SR_TXBUSYB)  /*发送忙*/

/*U7816 interrupt type*/
#define U7816_RXFULL_INTR            (1 << 0)  /*RXFIFO满中断使能位*/
#define U7816_RXMTRG_INTR            (1 << 1)  /*RXFIFO达到trigger值中断使能位*/
#define U7816_TXERR_INTR             (1 << 2)  /*发送数据奇偶校验错误中断使能位*/
#define U7816_TXEMPT_INTR            (1 << 3)  /*TXFIFO空中断使能位*/

/* -------------------------------------
                 SYSWDT
---------------------------------------*/
/*SYSWDT_CTRL*/
#define SYSWDT_CTRL_WDT_ENB      0
#define SYSWDT_CTRL_WDT_EN      (1 << 0)

#define SYSWDT_CTRL_RSTABLEB     1
#define SYSWDT_CTRL_RSTABLE     (1 << 1)

#define SYSWDT_CTRL_WAKABLEB      2
#define SYSWDT_CTRL_WAKABLE      (1 << 2)

#define SYSWDT_CTRL_CLRWAKEB      3
#define SYSWDT_CTRL_CLRWAKE      (1 << 3)

#define SYSWDT_CTRL_OTSETBF       4
#define SYSWDT_CTRL_OTSET         (0xf << 4)
#define SYSWDT_CTRL_OTSET_3906us  (0x0 << 4)
#define SYSWDT_CTRL_OTSET_7812us  (0x1 << 4)
#define SYSWDT_CTRL_OTSET_15625us (0x2 << 4)
#define SYSWDT_CTRL_OTSET_31250us (0x3 << 4)
#define SYSWDT_CTRL_OTSET_62500us (0x4 << 4)
#define SYSWDT_CTRL_OTSET_125ms   (0x5 << 4)
#define SYSWDT_CTRL_OTSET_250ms   (0x6 << 4)
#define SYSWDT_CTRL_OTSET_500ms   (0x7 << 4)
#define SYSWDT_CTRL_OTSET_1s      (0x8 << 4)
#define SYSWDT_CTRL_OTSET_2s      (0x9 << 4)
#define SYSWDT_CTRL_OTSET_4s      (0xA << 4)
#define SYSWDT_CTRL_OTSET_8s      (0xB << 4)
#define SYSWDT_CTRL_OTSET_12s     (0xC << 4)
#define SYSWDT_CTRL_OTSET_16s     (0xD << 4)
#define SYSWDT_CTRL_OTSET_24s     (0xE << 4)
#define SYSWDT_CTRL_OTSET_32s     (0xF << 4)

#define SYSWDT_CTRL_PASSWORDBF     8
#define SYSWDT_CTRL_PASSWORD_WDTCLR    (0x5a << 8)

#define SYSWDT_CTRL_INT_ENB     16
#define SYSWDT_CTRL_INT_EN     ((UINT32)1 << 16)


/* -------------------------------------
                 SPI
---------------------------------------*/

/*SPI_CR0*/
#define SPI_CR0_RFFB          6   /*主模式下，RXFIFO满中断使能位*/
#define SPI_CR0_RFF_EN        (1 << 6)   /*使能*/
#define SPI_CR0_RFF_DIS       (0 << 6)   /*不使能*/

#define SPI_CR0_TFEB          5   /*主模式下，TXFIFO空中断使能位*/
#define SPI_CR0_TFE_EN        (1 << 5)   /*使能*/
#define SPI_CR0_TFE_DIS       (0 << 5)   /*不使能*/

#define SPI_CR0_SSNB          4   /*从模式下SSN_S电平异常中断的中断使能位*/
#define SPI_CR0_SSN_EN        (1 << SPI_CR0_SSNB)   /*使能*/
#define SPI_CR0_SSN_DIS       (0 << SPI_CR0_SSNB)   /*不使能*/

#define SPI_CR0_TNFB          3   /*TNF中断的中断使能位*/
#define SPI_CR0_TNF_EN        (1 << SPI_CR0_TNFB)   /*使能*/
#define SPI_CR0_TNF_DIS       (0 << SPI_CR0_TNFB)   /*不使能*/


#define SPI_CR0_RNEB          2   /*RNE中断的中断使能位*/
#define SPI_CR0_RNE_EN        (1 << SPI_CR0_RNEB)   /*使能*/
#define SPI_CR0_RNE_DIS       (0 << SPI_CR0_RNEB)   /*不使能*/


#define SPI_CR0_BYTEDONEB          1   /*RNE中断的中断使能位*/
#define SPI_CR0_BYTEDONE_EN        (1 << SPI_CR0_BYTEDONEB)   /*使能*/
#define SPI_CR0_BYTEDONE_DIS       (0 << SPI_CR0_BYTEDONEB)   /*不使能*/

#define SPI_CR0_DNUMB          0   /*TNF中断的中断使能位*/
#define SPI_CR0_DUNM_EN        (1 << SPI_CR0_DNUMB)   /*使能*/
#define SPI_CR0_DNUM_DIS       (0 << SPI_CR0_DNUMB)   /*不使能*/



/*SPI_CR1*/

#define SPI_CR1_R_ENB           7   /*接收功能使能位*/
#define SPI_CR1_R_EN        (1 << SPI_CR1_R_ENB)   /*使能*/
#define SPI_CR1_R_DIS       (0 << SPI_CR1_R_ENB)   /*不使能*/

#define SPI_CR1_T_ENB           6   /*发送功能使能位*/
#define SPI_CR1_T_EN        (1 << SPI_CR1_T_ENB)   /*使能*/
#define SPI_CR1_T_DIS       (0 << SPI_CR1_T_ENB)   /*不使能*/

#define SPI_CR1_MSTRB            0   /*主从模式选择位*/
#define SPI_CR1_MSTR_MSTR       (1 << SPI_CR1_MSTRB)   /*主模式*/
#define SPI_CR1_MSTR_SLAVE      (0 << SPI_CR1_MSTRB)   /*从模式*/


#define SPI_CR1_SCK_MODBF        1   /*SPI模式选择选择*/
#define SPI_CR1_SCK_MOD          0x6   /*SPI模式选择选择*/
#define SPI_CR1_SCK_MOD0         (0 << SPI_CR1_SCK_MODBF)   /*SPI模式0*/
#define SPI_CR1_SCK_MOD1         (1 << SPI_CR1_SCK_MODBF)   /*SPI模式1*/
#define SPI_CR1_SCK_MOD2         (2 << SPI_CR1_SCK_MODBF)   /*SPI模式2*/
#define SPI_CR1_SCK_MOD3         (3 << SPI_CR1_SCK_MODBF)   /*SPI模式3*/


/*SPI_CR2*/
#define SPI_CR2_TTHDBF           4
#define SPI_CR2_TTHD_1           (1 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_2           (2 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_3           (3 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_4           (4 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_5           (5 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_6           (6 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_7           (7 << SPI_CR2_TTHDBF)
#define SPI_CR2_TTHD_8           (8 << SPI_CR2_TTHDBF)

#define SPI_CR2_RTHDBF           0
#define SPI_CR2_RTHD_0           (0 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_1           (1 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_2           (2 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_3           (3 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_4           (4 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_5           (5 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_6           (6 << SPI_CR2_RTHDBF)
#define SPI_CR2_RTHD_7           (7 << SPI_CR2_RTHDBF)

/*SPI_CR3*/
#define SPI_CR3_DEBNB           6   /*从模式下，SSN DEBOUNCE开关位*/
#define SPI_CR3_DEBN_EN        (1 << SPI_CR3_DEBNB)   /*使能*/
#define SPI_CR3_DEBN_DIS       (0 << SPI_CR3_DEBNB)  /*不使能*/

#define SPI_CR3_DNUMB           5   /*主模式下数据计数功能使能信号位*/
#define SPI_CR3_DNUM_EN        (1 << SPI_CR3_DNUMB)   /*使能*/
#define SPI_CR3_DNUM_DIS       (0 << SPI_CR3_DNUMB)  /*不使能*/

#define SPI_CR3_SPEB           4   /*SPI使能位*/
#define SPI_CR3_SPE_EN     (1 << SPI_CR3_SPEB)   /*使能*/
#define SPI_CR3_SPE_DIS    (0 << SPI_CR3_SPEB)   /*不使能*/

#define SPI_CR3_RFCB           3   /*SPI RFIFO清零使能位*/
#define SPI_CR3_RFC_EN     (1 << SPI_CR3_RFCB)   /*使能*/
#define SPI_CR3_RFC_DIS    (0 << SPI_CR3_RFCB)   /*不使能*/

#define SPI_CR3_TFCB           2   /*SPI TFIFO清零使能位*/
#define SPI_CR3_TFC_EN     (1 << SPI_CR3_TFCB)   /*使能*/
#define SPI_CR3_TFC_DIS    (0 << SPI_CR3_TFCB)   /*不使能*/

#define SPI_CR3_STRB           1   /*SPI START使能位*/
#define SPI_CR3_STR_EN     (1 << SPI_CR3_STRB)   /*使能*/
#define SPI_CR3_STR_DIS    (0 << SPI_CR3_STRB)   /*不使能*/

#define SPI_CR3_SSNB           0   /*SSN位*/
#define SPI_CR3_SSN_EN     (1 << SPI_CR3_SSNB)   /*使能*/
#define SPI_CR3_SSN_DIS    (0 << SPI_CR3_SSNB)   /*不使能*/

/*SPI_INTSR*/
#define SPI_INTR_RFF           (1 << 6)   /*RXFIFO满中断标志*/
#define SPI_INTR_TFE           (1 << 5)   /*TXFIFO空中断标志*/
#define SPI_INTR_SSNERR        (1 << 4)   /*ssn异常*/
#define SPI_INTR_TNF           (1 << 3)   /*发送FIFO非满标志*/
#define SPI_INTR_RNE           (1 << 2)   /*接收FIFO非空标志*/
#define SPI_INTR_BYTEDONE      (1 << 1)   /*单次传输完成中断*/
#define SPI_INTR_TRANDONE      (1 << 0)  /*数据传输达到预定量*/

/*SPI_SCKDIV*/
#define SPI_SCK_DIVBF        0   /*SPI主模式SCK频率选择*/

/* -------------------------------------
                 I2C
---------------------------------------*/


/*I2C_CFGR*/
#define I2C_CFGR_IICEB         0   
#define I2C_CFGR_IICE_EN          (1 << I2C_CFGR_IICEB ) 
#define I2C_CFGR_IICE_DIS          (0 << I2C_CFGR_IICEB )  

#define I2C_CFGR_TOB           1   
#define I2C_CFGR_TO           (1 << I2C_CFGR_TOB )   
#define I2C_CFGR_TO_EN          (1 << I2C_CFGR_TOB )  
#define I2C_CFGR_TO_DIS          (0 << I2C_CFGR_TOB ) 

#define I2C_CFGR_STRB          2   
#define I2C_CFGR_STR           (1 << 2)  
#define I2C_CFGR_STR_EN          (1 << I2C_CFGR_STRB )  
#define I2C_CFGR_STR_DIS          (0 << I2C_CFGR_STRB ) 

#define I2C_CFGR_DBCNBF        3   
#define I2C_CFGR_DBCN          (3 <<  I2C_CFGR_DBCNBF)  
#define I2C_CFGR_DB0           (0 <<  I2C_CFGR_DBCNBF)  
#define I2C_CFGR_DB2           (1 <<  I2C_CFGR_DBCNBF)   
#define I2C_CFGR_DB4           (2 <<  I2C_CFGR_DBCNBF)   
#define I2C_CFGR_DB6           (3 <<  I2C_CFGR_DBCNBF)   

#define I2C_CFGR_INTB          5   
#define I2C_CFGR_INT_EN          (1 << I2C_CFGR_INTB ) 
#define I2C_CFGR_INT_DIS          (0 << I2C_CFGR_INTB ) 

#define I2C_CFGR_DEIB          6   
#define I2C_CFGR_DEI_EN          (1 << I2C_CFGR_DEIB )  
#define I2C_CFGR_DEI_DIS          (0 << I2C_CFGR_DEIB ) 

#define I2C_CFGR_AKFB          7   
#define I2C_CFGR_AKF_EN          (1 << I2C_CFGR_AKFB )  
#define I2C_CFGR_AKF_DIS          (0 << I2C_CFGR_AKFB )  

#define I2C_CFGR_STADB          8  
#define I2C_CFGR_STAD_EN          (1 << I2C_CFGR_STADB )  
#define I2C_CFGR_STAD_DIS          (0 << I2C_CFGR_STADB )  

#define I2C_CFGR_STODB          9  
#define I2C_CFGR_STOD_EN          (1 << I2C_CFGR_STODB ) 
#define I2C_CFGR_STOD_DIS          (0 << I2C_CFGR_STODB )  

#define I2C_CFGR_BYDB          10  
#define I2C_CFGR_BYD_EN          (1 << I2C_CFGR_BYDB ) 
#define I2C_CFGR_BYD_DIS          (0 << I2C_CFGR_BYDB )  

#define I2C_CFGR_RXOVB         11  
#define I2C_CFGR_RXOV_EN          (1 << I2C_CFGR_RXOVB )  
#define I2C_CFGR_RXOV_DIS          (0 << I2C_CFGR_RXOVB )   

#define I2C_CFGR_TXOVB         12  
#define I2C_CFGR_TXOV_EN          (1 << I2C_CFGR_TXOVB )  
#define I2C_CFGR_TXOV_DIS          (0 << I2C_CFGR_TXOVB ) 

#define I2C_CFGR_ARBB         13  
#define I2C_CFGR_ARB_EN          (1 << I2C_CFGR_ArbB )  
#define I2C_CFGR_ARB_DIS          (0 << I2C_CFGR_ArbB ) 

#define I2C_CFGR_BSEB         14  
#define I2C_CFGR_BSE_EN          (1 << I2C_CFGR_BseB )  
#define I2C_CFGR_BSE_DIS          (0 << I2C_CFGR_BseB ) 

#define I2C_CFGR_TIOB         15 
#define I2C_CFGR_TIO_EN          (1 << I2C_CFGR_TIOB ) 
#define I2C_CFGR_TIO_DIS          (0 << I2C_CFGR_TIOB )  

/*I2C_ADR*/

#define I2C_ADR_GCAB         10    
#define I2C_ADR_GCA_EN       (1 << I2C_ADR_GCAB)   
#define I2C_ADR_GCA_DIS      (0 << I2C_ADR_GCAB)   

#define I2C_ADR_ADMABF       12    
#define I2C_ADR_ADMA_0       (0 << I2C_ADR_ADMABF)    
#define I2C_ADR_ADMA_1       (1 << I2C_ADR_ADMABF)   
#define I2C_ADR_ADMA_2       (2 << I2C_ADR_ADMABF)    
#define I2C_ADR_ADMA_3       (3 << I2C_ADR_ADMABF)   
#define I2C_ADR_ADMA_4       (4 << I2C_ADR_ADMABF)    
#define I2C_ADR_ADMA_5       (5 << I2C_ADR_ADMABF)   
#define I2C_ADR_ADMA_6       (6 << I2C_ADR_ADMABF)    
#define I2C_ADR_ADMA_7       (7 << I2C_ADR_ADMABF)   

#define I2C_ADR_ADSEB        15    
#define I2C_ADR_ADSE_10BIT   (1 << I2C_ADR_ADSEB )  
#define I2C_ADR_ADSE_7BIT    (0 << I2C_ADR_ADSEB ) 

/*I2C_CMDR*/

#define I2C_CMDR_MOSEB        0    
#define I2C_CMDR_MODE_MASTER          (1 << I2C_CMDR_MOSEB )
#define I2C_CMDR_MODE_SLAVE          (0 << I2C_CMDR_MOSEB ) 

#define I2C_CMDR_RSTAB        1    
#define I2C_CMDR_RSTA_EN          (1 << I2C_CMDR_RSTAB )  
#define I2C_CMDR_RSTA_DIS          (0 << I2C_CMDR_RSTAB ) 

#define I2C_CMDR_ACKB         2    
#define I2C_CMDR_NACK          (1 << I2C_CMDR_ACKB ) 
#define I2C_CMDR_ACK          (0 << I2C_CMDR_ACKB )  

#define I2C_CMDR_DMAB         3    
#define I2C_CMDR_DMA_EN          (1 << I2C_CMDR_DMAB ) 
#define I2C_CMDR_DMA_DIS          (0 << I2C_CMDR_DMAB ) 

/*I2C_SR*/

#define I2C_SR_MSMDB        0   
#define I2C_SR_MSMD         (1 << 0)   

#define I2C_SR_ADMB        1   
#define I2C_SR_ADM        (1 << 1)    

#define I2C_SR_TIPB        2    
#define I2C_SR_TIP        (1 << 2)    

#define I2C_SR_TRDB        3    
#define I2C_SR_TRD        (1 << 3)   

#define I2C_SR_BUFB        4    
#define I2C_SR_BUF        (1 << 4)   

#define I2C_SR_GCAMB        5   
#define I2C_SR_GCAM        (1 << 5)     


#define I2C_SR_DAEIB       6   
#define I2C_SR_DATAERR     (1 << 6)    

#define I2C_SR_ACFIB        7   
#define I2C_SR_ACKFAIL       (1 << 7)    

#define I2C_SR_STAIB        8    
#define I2C_SR_START        (1 << I2C_SR_STAIB )

#define I2C_SR_STOIB        9   
#define I2C_SR_STOP         (1 << I2C_SR_STOIB )   

#define I2C_SR_BYDIB        10   
#define I2C_SR_BYTEDONE     (1 << I2C_SR_BYDIB  )

#define I2C_SR_RXOIB        11   
#define I2C_SR_RXOV        (1 << 11)   

#define I2C_SR_TXOIB        12    
#define I2C_SR_TXOV        (1 << 12)    

#define I2C_SR_ARLIB        13    
#define I2C_SR_ARBLOST      (1 << 13)    

#define I2C_SR_BUEIB        14    
#define I2C_SR_BUFERR       (1 << 14)    

#define I2C_SR_TIOIB        15   
#define I2C_SR_TIMEOUT      (1 << 15)   

/* -------------------------------------
                 CRC
---------------------------------------*/

/*CRC_CTRL*/
#define CRC_CTRL_POTYBF          0   /*CRC */
#define CRC_CTRL_CCITT           0   /*CRC-CCITT??*/
#define CRC_CTRL_CRC_16          1   /*CRC-16??*/
#define CRC_CTRL_CRC_32          2   /*CRC-32??*/
#define CRC_CTRL_CRC_8           3   /*CRC-8??*/

#define CRC_CTRL_REFINB          4  /*?????????*/
#define CRC_CTRL_REFIN_EN        (1 << CRC_CTRL_REFINB)   /*????????*/
#define CRC_CTRL_REFIN_DIS       (0 << CRC_CTRL_REFINB)   /*?????????*/

#define CRC_CTRL_REFOUTB          5  /*????????*/
#define CRC_CTRL_REFOUT_EN        (1 << CRC_CTRL_REFOUTB)   /*???????*/
#define CRC_CTRL_REFOUT_DIS       (0 << CRC_CTRL_REFOUTB)   /*????????*/

#define CRC_CTRL_INIT_PATTB          6  /*CRC_REG??????*/
#define CRC_CTRL_INIT_PATT_EN        (1 << CRC_CTRL_INIT_PATTB)   /*CRC_REG?????1*/
#define CRC_CTRL_INIT_PATT_DIS       (0 << CRC_CTRL_INIT_PATTB)   /*CRC_REG?????0*/

#define CRC_CTRL_XOROUTB          7  /*CRC_REG???????*/
#define CRC_CTRL_XOROUT_EN        (1 << CRC_CTRL_XOROUTB)   /*CRC_REG??????1??*/
#define CRC_CTRL_XOROUT_DIS       (0 << CRC_CTRL_XOROUTB)   /*CRC_REG??????0??*/

#define CRC_CTRL_MODB          8  /*CRC_?????*/
#define CRC_CTRL_MOD_GENERATE     (0 << CRC_CTRL_MODB)   /*????*/ 
#define CRC_CTRL_MOD_VERIFY       (1 << CRC_CTRL_MODB)   /*??????crc_reg???0,?crc_fail*/

#define CRC_CTRL_STRBF          10   /*CRC???*/
#define CRC_CTRL_BYTE           0   /*????*/
#define CRC_CTRL_HALF           1   /*????*/
#define CRC_CTRL_WORD           3   /*???*/

#define CRC_CTRL_CRCENB          9  /*CRC_???*/
#define CRC_CTRL_CRCEN_EN        (1 << CRC_CTRL_CRCENB)   /*??CRC????*/
#define CRC_CTRL_CRCEN_DIS       (0 << CRC_CTRL_CRCENB)   /*??CRC????*/

/*CRC_STAT*/
#define CRC_STAT_CRC_FAILB          0 /*CRC_????*/
#define CRC_STAT_CRC_FAIL          1 /*fail*/
#define CRC_STAT_CRC_SUCCESS        0 /*fail*/

#define CRC_STAT_CRC_RDYB           1 /*CRC_????*/
#define CRC_STAT_CRC_RDY            (1 << 1) /*CRC_????*/
#define CRC_STAT_CRC_RUN            (0 << 1) /*CRC_???*/

/* -------------------------------------
                 UART
---------------------------------------*/
/*STATUS*/
#define UART_STATUS_TXFULLB            0
#define UART_STATUS_TXFULL             ((UINT32)1 << 0)

#define UART_STATUS_RXEMPB             1
#define UART_STATUS_RXEMP             ((UINT32)1 << 1)

#define UART_STATUS_TXINGB             2
#define UART_STATUS_TXING             ((UINT32)1 << 2)

#define UART_STATUS_RXINGB             3
#define UART_STATUS_RXING             ((UINT32)1 << 3)

#define UART_STATUS_RXFULLB            4
#define UART_STATUS_RXFULL            ((UINT32)1 << 4)

#define UART_STATUS_TXEMPB             5
#define UART_STATUS_TXEMP             ((UINT32)1 << 5)

#define UART_STATUS_RXFIFO_OVERRUNB    6
#define UART_STATUS_RXFIFO_OVERRUN    ((UINT32)1 << 6)

#define UART_STATUS_TXFIFO_OVERRUNB    7
#define UART_STATUS_TXFIFO_OVERRUN    ((UINT32)1 << 7)

/*CTRL*/
#define UART_CTRL_TXENB            0
#define UART_CTRL_TXEN            ((UINT32)1 << 0)

#define UART_CTRL_RXENB             1
#define UART_CTRL_RXEN             ((UINT32)1 << 1)

#define UART_CTRL_TXINTENB            2
#define UART_CTRL_TXINTEN            ((UINT32)1 << 2)
#define UART_CTRL_TXINTDIS           ((UINT32)0 << 2)

#define UART_CTRL_RXINTENB            3
#define UART_CTRL_RXINTEN            ((UINT32)1 << 3)
#define UART_CTRL_RXINTDIS           ((UINT32)0 << 3)

#define UART_CTRL_PARITY_ENB          4
#define UART_CTRL_PARITY_EN          ((UINT32)1 << 4)

#define UART_CTRL_STOP_NUMB           5
#define UART_CTRL_STOP_NUM           ((UINT32)1 << 5)
#define UART_CTRL_STOP_1BIT          ((UINT32)0 << 5)
#define UART_CTRL_STOP_2BIT          ((UINT32)1 << 5)

#define UART_CTRL_STOPBIT_CHECKB      6
#define UART_CTRL_STOPBIT_CHECK      ((UINT32)1 << 6)

#define UART_CTRL_PARITY_TYPEB        7
#define UART_CTRL_PARITY_TYPE        ((UINT32)1 << 7)
#define UART_CTRL_PARITY_EVEN        ((UINT32)0 << 7)
#define UART_CTRL_PARITY_ODD         ((UINT32)1 << 7)




/*INTSTATUS*/
#define UART_INTSTATUS_TXINTB          0
#define UART_INTSTATUS_TXINT          ((UINT32)1 << 0)

#define UART_INTSTATUS_RXINTB          1
#define UART_INTSTATUS_RXINT          ((UINT32)1 << 1)

#define UART_INTSTATUS_RXERRB          2
#define UART_INTSTATUS_RXERR          ((UINT32)1 << 2)


/* -------------------------------------
                DMA
---------------------------------------*/
/*TRANS_CTRL_REG*/
#define DMA_TRANS_CTRL_REG_DMAENB      0
#define DMA_TRANS_CTRL_REG_DMAEN      ((UINT32)1 << 0)

/*TRANS_FMT1_REG*/
#define DMA_TRANS_FMT1_REG_SRC_BLK_OFFSETB  15
#define DMA_TRANS_FMT1_REG_SRC_BLK_OFFSET   (1 << 15)

#define DMA_TRANS_FMT1_REG_SRC_INCBF        12
#define DMA_TRANS_FMT1_REG_SRC_INC         (0x7 << 12)
#define DMA_TRANS_FMT1_REG_SRC_NOCHANGE    (0x0 << 12)
#define DMA_TRANS_FMT1_REG_SRC_INC1UNIT    (0x1 << 12)
#define DMA_TRANS_FMT1_REG_SRC_INC2UNIT    (0x2 << 12)
#define DMA_TRANS_FMT1_REG_SRC_DEC1UNIT    (0x5 << 12)
#define DMA_TRANS_FMT1_REG_SRC_DEC2UNIT    (0x6 << 12)


#define DMA_TRANS_FMT1_REG_DST_BLK_OFFSETB  11
#define DMA_TRANS_FMT1_REG_DST_BLK_OFFSET   (1 << 11)

#define DMA_TRANS_FMT1_REG_DST_INCBF        8
#define DMA_TRANS_FMT1_REG_DST_INC         (0x7 << 8)
#define DMA_TRANS_FMT1_REG_DST_NOCHANGE    (0x0 << 8)
#define DMA_TRANS_FMT1_REG_DST_INC1UNIT    (0x1 << 8)
#define DMA_TRANS_FMT1_REG_DST_INC2UNIT    (0x2 << 8)
#define DMA_TRANS_FMT1_REG_DST_DEC1UNIT    (0x5 << 8)
#define DMA_TRANS_FMT1_REG_DST_DEC2UNIT    (0x6 << 8)

#define DMA_TRANS_FMT1_REG_INTENB      4
#define DMA_TRANS_FMT1_REG_INTEN      ((UINT32)1 << 4)

#define DMA_TRANS_FMT1_REG_TR_UNITBF    2
#define DMA_TRANS_FMT1_REG_TR_UNIT     ((UINT32)3 << 2)
#define DMA_TRANS_FMT1_REG_TR_UNIT_BYTE     ((UINT32)0 << 2)
#define DMA_TRANS_FMT1_REG_TR_UNIT_HALFWORD ((UINT32)1 << 2)
#define DMA_TRANS_FMT1_REG_TR_UNIT_WORD     ((UINT32)2 << 2)

#define DMA_TRANS_FMT1_REG_TRANS_MODEBF     0
#define DMA_TRANS_FMT1_REG_TRANS_MODE       ((UINT32)0x3 << 0)
#define DMA_TRANS_FMT1_REG_TRANS_MODE_COMMON       ((UINT32)0x0 << 0)
#define DMA_TRANS_FMT1_REG_TRANS_MODE_REPEAT1      ((UINT32)0x1 << 0)
#define DMA_TRANS_FMT1_REG_TRANS_MODE_REPEAT2      ((UINT32)0x2 << 0)


//DMA_TRANS_FMT2_REG
#define DMA_TRANS_FMT2_REG_BLOCKSBF                  16
#define DMA_TRANS_FMT2_REG_BLOCKS                   ((UINT32)0xFFFF << 16)

#define DMA_TRANS_FMT2_REG_TR_CNTBF                  0
#define DMA_TRANS_FMT2_REG_TR_CNT                  0xFFFF

/*TRANS_STAT2_REG*/
#define DMA_TRANS_STAT2_REG_REPTIMES_LEFTBF  16
#define DMA_TRANS_STAT2_REG_REPTIMES_LEFT  ((UINT32)0xFFFF << 16)

#define DMA_TRANS_STAT2_REG_TR_CNT_LEFTBF  0
#define DMA_TRANS_STAT2_REG_TR_CNT_LEFT    0xFFFF

/*TRANS_STAT1_REG*/
#define DMA_TRANS_STAT1_REG_DMA_IN_PROCESSB  4
#define DMA_TRANS_STAT1_REG_DMA_IN_PROCESS   (1 << 4)

#define DMA_TRANS_STAT1_REG_DMA_ERRORBF    2
#define DMA_TRANS_STAT1_REG_DMA_ERR        (3 << 2)
#define DMA_TRANS_STAT1_REG_DMA_CFGERR     (1 << 2)

#define DMA_TRANS_STAT1_REG_DMA_INTB    1
#define DMA_TRANS_STAT1_REG_DMA_INT     (1 << 1)

#define DMA_TRANS_STAT1_REG_DMA_ENB    0
#define DMA_TRANS_STAT1_REG_DMA_EN     (1 << 0)


/*TRIGEN_REG*/
#define DMA_TRIG_I2C0_RX      0
#define DMA_TRIG_I2C0_TX      1
#define DMA_TRIG_I2C1_RX      2
#define DMA_TRIG_I2C1_TX      3
#define DMA_TRIG_I2C2_RX      4
#define DMA_TRIG_I2C2_TX      5
#define DMA_TRIG_UART0_RX     6
#define DMA_TRIG_UART0_TX     7
#define DMA_TRIG_UART1_RX     8
#define DMA_TRIG_UART1_TX     9
#define DMA_TRIG_UART2_RX      10
#define DMA_TRIG_UART2_TX      11
#define DMA_TRIG_UART3_RX      12
#define DMA_TRIG_UART3_TX      13
#define DMA_TRIG_UART4_RX      14
#define DMA_TRIG_UART4_TX      15
#define DMA_TRIG_UART5_RX      16
#define DMA_TRIG_UART5_TX      17
#define DMA_TRIG_UART6_RX      18
#define DMA_TRIG_UART6_TX      19
#define DMA_TRIG_UART7_RX      20
#define DMA_TRIG_UART7_TX      21
#define DMA_TRIG_TIMERA0        22
#define DMA_TRIG_TIMERA1        23
#define DMA_TRIG_TIMERB0        24
#define DMA_TRIG_TIMERB1        25
#define DMA_TRIG_TIMERC0        26
#define DMA_TRIG_TIMERC1        27
#define DMA_TRIG_TIMERD0        28
#define DMA_TRIG_TIMERD1        29
#define DMA_TRIG_SPI0_TX       30
#define DMA_TRIG_SPI0_RX       31
#define DMA_TRIG_SPI1_TX       32
#define DMA_TRIG_SPI1_RX       33
#define DMA_TRIG_SPI2_TX       34
#define DMA_TRIG_SPI2_RX       35
#define DMA_TRIG_SPI3_TX       36
#define DMA_TRIG_SPI3_RX       37
#define DMA_TRIG_CRC_RDY       38
#define DMA_TRIG_ADC0_RDY      39
#define DMA_TRIG_ADC1_RDY      40
#define DMA_TRIG_ADC2_RDY      41
#define DMA_TRIG_ADC3_RDY      42
#define DMA_TRIG_ADC4_RDY      43
#define DMA_TRIG_ADC5_RDY      44
#define DMA_TRIG_ADC6_RDY      45
#define DMA_TRIG_ADC7_RDY      46
#define DMA_TRIG_ADC8_RDY      47
#define DMA_TRIG_ADC9_RDY      48
#define DMA_TRIG_ADC10_RDY      49
#define DMA_TRIG_ADC11_RDY      50
#define DMA_TRIG_NONE      51


/* -------------------------------------
                RTC
---------------------------------------*/
/*RTC_TIME0/RTC current TIME0/ RTC alarm time0*/
#define RTC_TIME0_BCD_SECBF           0x0
#define RTC_TIME0_BCD_SEC             0xF

#define RTC_TIME0_BCD_SEC_DECBF           0x4
#define RTC_TIME0_BCD_SEC_DEC           ((UINT32)0x7 << 4)


#define RTC_TIME0_BCD_MINBF           0x8
#define RTC_TIME0_BCD_MIN             ((UINT32)0xF << 8)

#define RTC_TIME0_BCD_MIN_DECBF           12
#define RTC_TIME0_BCD_MIN_DEC           ((UINT32)0x7 << 12)

#define RTC_TIME0_BCD_HOURBF           16
#define RTC_TIME0_BCD_HOUR             ((UINT32)0xF << 16)

#define RTC_TIME0_BCD_HOUR_DECBF           20
#define RTC_TIME0_BCD_HOUR_DEC           ((UINT32)0x3 << 20)

#define RTC_TIME0_BCD_WEEKDAYBF           24
#define RTC_TIME0_BCD_WEEKDAY           ((UINT32)0x7 << 24)

/*RTC_TIME1 /RTC current TIME1/ RTC alarm time1*/
#define RTC_TIME1_BCD_DATEBF           0x0
#define RTC_TIME1_BCD_DATE             0xF

#define RTC_TIME1_BCD_DATE_DECBF           0x4
#define RTC_TIME1_BCD_DATE_DEC           ((UINT32)0x3 << 4)


#define RTC_TIME1_BCD_MONTHBF           0x8
#define RTC_TIME1_BCD_MONTH             ((UINT32)0xF << 8)

#define RTC_TIME1_BCD_MONTH_DECB           12
#define RTC_TIME1_BCD_MONTH_DEC           ((UINT32)0x1 << 12)

#define RTC_TIME1_BCD_YEARBF           16
#define RTC_TIME1_BCD_YEAR             ((UINT32)0xF << 16)

#define RTC_TIME1_BCD_YEAR_DECBF           20
#define RTC_TIME1_BCD_YEAR_DEC           ((UINT32)0xF << 20)


/*RTC_CTL*/
#define RTC_CTL_RTC_ENB                        0
#define RTC_CTL_RTC_EN                         0x1

#define RTC_CTL_ALARM_ENB            0x1
#define RTC_CTL_ALARM_EN             ((UINT32)0x1 << 1)

#define RTC_CTL_PRD_INT_ENB            0x2
#define RTC_CTL_PRD_INT_EN             ((UINT32)0x1 << 2)

#define RTC_CTL_ALARM_INT_ENB            0x3
#define RTC_CTL_ALARM_INT_EN             ((UINT32)0x1 << 3)

#define RTC_CTL_PERINT_SELBF            0x4
#define RTC_CTL_PERINT_SEL             ((UINT32)0x7 << 4)
#define RTC_CTL_PERINT_SEL_SECOND      ((UINT32)0x7 << 4)
#define RTC_CTL_PERINT_SEL_MINUTE      ((UINT32)0x6 << 4)
#define RTC_CTL_PERINT_SEL_HOUR        ((UINT32)0x5 << 4)
#define RTC_CTL_PERINT_SEL_DAY         ((UINT32)0x4 << 4)
#define RTC_CTL_PERINT_SEL_MONTH       ((UINT32)0x3 << 4)


/*RTC_STATUS*/
#define RTC_EN_STATUS                ((UINT32)1 << 0)
#define RTC_ALARM_INT_STATUS         ((UINT32)1 << 4)
#define RTC_PRD_INT_STATUS           ((UINT32)1 << 5)


/* -------------------------------------
                GPIO
---------------------------------------*/
/*DBSYR*/
#define GPIO_DBSYR_DEBOUNCE1BF                0
#define GPIO_DBSYR_DEBOUNCE1                 (3 << 0)

#define GPIO_DBSYR_DEBOUNCE_CYCLE1BF           2
#define GPIO_DBSYR_DEBOUNCE_CYCLE1            (3 << 2)

#define GPIO_DBSYR_INTER_MODE1BF               4
#define GPIO_DBSYR_INTER_MODE1                 (7 << 4)

#define GPIO_DBSYR_SYNC1B                       7
#define GPIO_DBSYR_SYNC1                       (1 << 7)

#define GPIO_DBSYR_DEBOUNCE2BF                8
#define GPIO_DBSYR_DEBOUNCE2                 (3 << 8)

#define GPIO_DBSYR_DEBOUNCE_CYCLE2BF           10
#define GPIO_DBSYR_DEBOUNCE_CYCLE2            (3 << 10)

#define GPIO_DBSYR_INTER_MODE2BF               12
#define GPIO_DBSYR_INTER_MODE2                 (7 << 12)

#define GPIO_DBSYR_SYNC2B                       15
#define GPIO_DBSYR_SYNC2                       (1 << 15)

#define GPIO_DBSYR_DEBOUNCE3BF                16
#define GPIO_DBSYR_DEBOUNCE3                 (3 << 16)

#define GPIO_DBSYR_DEBOUNCE_CYCLE3BF           18
#define GPIO_DBSYR_DEBOUNCE_CYCLE3            (3 << 18)

#define GPIO_DBSYR_INTER_MODE3BF               20
#define GPIO_DBSYR_INTER_MODE3                 (7 << 20)

#define GPIO_DBSYR_SYNC3B                       23
#define GPIO_DBSYR_SYNC3                       (1 << 23)

#define GPIO_DBSYR_DEBOUNCE4BF                24
#define GPIO_DBSYR_DEBOUNCE4                 (3 << 24)

#define GPIO_DBSYR_DEBOUNCE_CYCLE4BF           26
#define GPIO_DBSYR_DEBOUNCE_CYCLE4            (3 << 26)

#define GPIO_DBSYR_INTER_MODE4BF               28
#define GPIO_DBSYR_INTER_MODE4                (7 << 28)

#define GPIO_DBSYR_SYNC4B                       31
#define GPIO_DBSYR_SYNC4                       (1 << 31)

/* -------------------------------------
                PWM
---------------------------------------*/
/*PWM_CONTROL*/
#define PWM_CONTROL_TMENB             0  //计数器使能
#define PWM_CONTROL_TRSTB             1  //计数器复位
#define PWM_CONTROL_OUTENB            2  //输出使能
#define PWM_CONTROL_REVB              3  //输出反向

/* -------------------------------------
                 IOCTRL
---------------------------------------*/
//IO_CTRL1
#define IO_CTRL1_JTAG_GPIO346B          0 
#define IO_CTRL1_JTAG_GPIO346          (1 << 0) 
#define IO_CTRL1_JTAG_GPIO346_JTAG     (0 << 0) 
#define IO_CTRL1_JTAG_GPIO346_GPIO     (1 << 0) 

#define IO_CTRL1_MCO_GPIO111B           1
#define IO_CTRL1_MCO_GPIO111           (1 << 1) 
#define IO_CTRL1_MCO_GPIO111_MCO       (0 << 1) 
#define IO_CTRL1_MCO_GPIO111_GPIO0     (1 << 1) 


#define IO_CTRL1_SRAM_EN               (1 << 2) 
#define IO_CTRL1_OTG_EN                (1 << 3) 
#define IO_CTRL1_IRDA_EN               (1 << 4) 
#define IO_CTRL1_ADC_TEST_EN           (1 << 6) 
#define IO_CTRL1_ANA_TEST_EN           (1 << 7) 

#define IO_CTRL1_XTAL32_GPIO109B         8 
#define IO_CTRL1_XTAL32_GPIO109         (1 << 8) 
#define IO_CTRL1_XTAL32_GPIO109_XTAL32  (0 << 8) 
#define IO_CTRL1_XTAL32_GPIO109_GPIO    (1 << 8) 

#define IO_CTRL1_EXTAL32_32KCLKIN_GPIO110BF         9 
#define IO_CTRL1_EXTAL32_32KCLKIN_GPIO110           (3 << 9) 
#define IO_CTRL1_EXTAL32_32KCLKIN_GPIO110_EXTAL32   (0 << 9) 
#define IO_CTRL1_EXTAL32_32KCLKIN_GPIO110_32KCLKIN  (1 << 9) 
#define IO_CTRL1_EXTAL32_32KCLKIN_GPIO110_GPIO      (2 << 9) 

#define IO_CTRL1_XTAL0_GPIO113B         11
#define IO_CTRL1_XTAL0_GPIO113          (1 << 11)
#define IO_CTRL1_XTAL0_GPIO113_XTAL0    (0 << 11)
#define IO_CTRL1_XTAL0_GPIO113_GPIO     (1 << 11)

#define IO_CTRL1_EXTAL0_CLKIN_GPIO112BF         12 
#define IO_CTRL1_EXTAL0_CLKIN_GPIO112           (3 << 12) 
#define IO_CTRL1_EXTAL0_CLKIN_GPIO112_EXTAL0    (0 << 12) 
#define IO_CTRL1_EXTAL0_CLKIN_GPIO112_CLKIN     (1 << 12) 
#define IO_CTRL1_EXTAL0_CLKIN_GPIO112_GPIO      (2 << 12) 

#define IO_CTRL1_GPIO1516_CANB                      16 
#define IO_CTRL1_GPIO1516_CAN                       ((UINT32)1 << 16) 
#define IO_CTRL1_GPIO1516_CAN_GPIO                  ((UINT32)0 << 16) 
#define IO_CTRL1_GPIO1516_CAN_CAN                   ((UINT32)1 << 16) 

#define IO_CTRL1_SPI3_GPIO1720B                  20 
#define IO_CTRL1_SPI3_GPIO1720                   ((UINT32)1 << 20) 
#define IO_CTRL1_SPI3_GPIO1720_SPI3              ((UINT32)0 << 20) 
#define IO_CTRL1_SPI3_GPIO1720_GPIO              ((UINT32)1 << 20) 

#define IO_CTRL1_UART1_GPIO2122B                      21 
#define IO_CTRL1_UART1_GPIO2122                       ((UINT32)1 << 21) 
#define IO_CTRL1_UART1_GPIO2122_UART1                 ((UINT32)0 << 21)
#define IO_CTRL1_UART1_GPIO2122_GPIO                  ((UINT32)1 << 21)

#define IO_CTRL1_UART3_GPIO2324B                      24 
#define IO_CTRL1_UART3_GPIO2324                     ((UINT32)1 << 24)
#define IO_CTRL1_UART3_GPIO2324_UART3               ((UINT32)0 << 24)
#define IO_CTRL1_UART3_GPIO2324_GPIO                ((UINT32)1 << 24)
 
#define IO_CTRL1_DRVVBUS_GPIO25B                      25
#define IO_CTRL1_DRVVBUS_GPIO25                     ((UINT32)1 << 25)
#define IO_CTRL1_DRVVBUS_GPIO25_DRVVBUS             ((UINT32)0 << 25)
#define IO_CTRL1_DRVVBUS_GPIO25_GPIO                ((UINT32)1 << 25)

#define IO_CTRL1_UART4_GPIO2627B                      26 
#define IO_CTRL1_UART4_GPIO2627                      ((UINT32)1 << 26) 
#define IO_CTRL1_UART4_GPIO2627_UART4                ((UINT32)0 << 26) 
#define IO_CTRL1_UART4_GPIO2627_GPIO                 ((UINT32)1 << 26) 

#define IO_CTRL1_RMII0_GPIO28B                      28
#define IO_CTRL1_RMII0_GPIO28                       ((UINT32)1 << 28)
#define IO_CTRL1_RMII0_GPIO28_RMII0                 ((UINT32)0 << 28)
#define IO_CTRL1_RMII0_GPIO28_GPIO                  ((UINT32)1 << 28)

#define IO_CTRL1_PWM0_GPIO29B                      29
#define IO_CTRL1_PWM0_GPIO29                       ((UINT32)1 << 29)
#define IO_CTRL1_PWM0_GPIO29_PWM0                  ((UINT32)0 << 29)
#define IO_CTRL1_PWM0_GPIO29_GPIO                  ((UINT32)1 << 29)

#define IO_CTRL1_PWM1_GPIO30B                      30
#define IO_CTRL1_PWM1_GPIO30                       ((UINT32)1 << 30)
#define IO_CTRL1_PWM1_GPIO30_PWM1                  ((UINT32)0 << 30)
#define IO_CTRL1_PWM1_GPIO30_GPIO                  ((UINT32)1 << 30)

#define IO_CTRL1_MII0_GPIO3133B                    31
#define IO_CTRL1_MII0_GPIO3133                     ((UINT32)1 << 31)
#define IO_CTRL1_MII0_GPIO3133_MII0                ((UINT32)0 << 31)
#define IO_CTRL1_MII0_GPIO3133_GPIO                ((UINT32)1 << 31)

//IO_CTRL2
#define IO_CTRL2_RMII0_GPIO3439B                      0 
#define IO_CTRL2_RMII0_GPIO3439                      (1 << 0) 
#define IO_CTRL2_RMII0_GPIO3439_RMII0                (0 << 0) 
#define IO_CTRL2_RMII0_GPIO3439_GPIO                 (1 << 0) 

#define IO_CTRL2_I2C2_GPIO4041B                      8
#define IO_CTRL2_I2C2_GPIO4041                       (1 << 8)
#define IO_CTRL2_I2C2_GPIO4041_I2C2                  (0 << 8)
#define IO_CTRL2_I2C2_GPIO4041_GPIO                  (1 << 8)

#define IO_CTRL2_MII0_GPIO4249B                     12 
#define IO_CTRL2_MII0_GPIO4249                      (1 << 12) 
#define IO_CTRL2_MII0_GPIO4249_MII0                 (0 << 12) 
#define IO_CTRL2_MII0_GPIO4249_GPIO                 (1 << 12) 

#define IO_CTRL2_SPI1_SCI1_GPIO5053BF                   16 
#define IO_CTRL2_SPI1_SCI1_GPIO5053                     ((UINT32)3 << 16) 
#define IO_CTRL2_SPI1_SCI1_GPIO5053_SPI1                ((UINT32)0 << 16) 
#define IO_CTRL2_SPI1_SCI1_GPIO5053_SCI1                ((UINT32)1 << 16) 
#define IO_CTRL2_SPI1_SCI1_GPIO5053_GPIO                ((UINT32)2 << 16) 

#define IO_CTRL2_MII1_GPIO5457B                   24 
#define IO_CTRL2_MII1_GPIO5457                    ((UINT32)1 << 24) 
#define IO_CTRL2_MII1_GPIO5457_MII1               ((UINT32)0 << 24) 
#define IO_CTRL2_MII1_GPIO5457_GPIO               ((UINT32)1 << 24) 

#define IO_CTRL2_RMII1_GPIO5863B                   28 
#define IO_CTRL2_RMII1_GPIO5863                   ((UINT32)1 << 28) 
#define IO_CTRL2_RMII1_GPIO5863_RMII1             ((UINT32)0 << 28) 
#define IO_CTRL2_RMII1_GPIO5863_GPIO              ((UINT32)1 << 28) 

//IO_CTRL3
#define IO_CTRL3_MII1_GPIO6465B                  0
#define IO_CTRL3_MII1_GPIO6465                   (1 << 0)
#define IO_CTRL3_MII1_GPIO6465_MII1              (0 << 0)
#define IO_CTRL3_MII1_GPIO6465_GPIO              (1 << 0)

#define IO_CTRL3_MII1_SPI3_GPIO6669BF             2
#define IO_CTRL3_MII1_SPI3_GPIO6669              (3 << 2)
#define IO_CTRL3_MII1_SPI3_GPIO6669_MII1         (0 << 2)
#define IO_CTRL3_MII1_SPI3_GPIO6669_SPI3         (1 << 2)
#define IO_CTRL3_MII1_SPI3_GPIO6669_GPIO         (2 << 2)

#define IO_CTRL3_RMII1_GPIO7071B                 4
#define IO_CTRL3_RMII1_GPIO7071                  (1 << 4)
#define IO_CTRL3_RMII1_GPIO7071_RMII1            (0 << 4)
#define IO_CTRL3_RMII1_GPIO7071_GPIO             (1 << 4)

#define IO_CTRL3_I2C1_SCL_GPIO72B                 7
#define IO_CTRL3_I2C1_SCL_GPIO72                  (1 << 7)
#define IO_CTRL3_I2C1_SCL_GPIO72_I2C1             (0 << 7)
#define IO_CTRL3_I2C1_SCL_GPIO72_GPIO             (1 << 7)

#define IO_CTRL3_I2C1_SDA_SCI0_GPIO73B            8
#define IO_CTRL3_I2C1_SDA_SCI0_GPIO73             (1 << 8)
#define IO_CTRL3_I2C1_SDA_SCI0_GPIO73_I2C1        (0 << 8)
#define IO_CTRL3_I2C1_SDA_SCI0_GPIO73_SCI0        (1 << 8)
#define IO_CTRL3_I2C1_SDA_SCI0_GPIO73_GPIO        (2 << 8)

#define IO_CTRL3_UART0_GPIO7475B                  10
#define IO_CTRL3_UART0_GPIO7475                   (1 << 10)
#define IO_CTRL3_UART0_GPIO7475_UART0             (0 << 10)
#define IO_CTRL3_UART0_GPIO7475_GPIO              (1 << 10)

#define IO_CTRL3_PWM2_GPIO76B                     12
#define IO_CTRL3_PWM2_GPIO76                     (1 << 12)
#define IO_CTRL3_PWM2_GPIO76_PWM2                (0 << 12)
#define IO_CTRL3_PWM2_GPIO76_GPIO                (1 << 12)

#define IO_CTRL3_PWM3_GPIO77B                     13
#define IO_CTRL3_PWM3_GPIO77                     (1 << 13)
#define IO_CTRL3_PWM3_GPIO77_PWM3                (0 << 13)
#define IO_CTRL3_PWM3_GPIO77_GPIO                (1 << 13)

#define IO_CTRL3_SPI0_GPIO8081B                     15
#define IO_CTRL3_SPI0_GPIO8081                      (1 << 15)
#define IO_CTRL3_SPI0_GPIO8081_SPI0                 (0 << 15)
#define IO_CTRL3_SPI0_GPIO8081_GPIO                 (1 << 15)

#define IO_CTRL3_SPI0_SCI0_GPIO7879BF                16
#define IO_CTRL3_SPI0_SCI0_GPIO7879                 ((UINT32)3 << 16)
#define IO_CTRL3_SPI0_SCI0_GPIO7879_SPI0            ((UINT32)0 << 16)
#define IO_CTRL3_SPI0_SCI0_GPIO7879_SCI0            ((UINT32)1 << 16)
#define IO_CTRL3_SPI0_SCI0_GPIO7879_GPIO            ((UINT32)2 << 16)

#define IO_CTRL3_UART2_GPIOB                       18
#define IO_CTRL3_UART2_GPIO                       ((UINT32)1 << 18)
#define IO_CTRL3_UART2_GPIO_UART2                 ((UINT32)0 << 18)
#define IO_CTRL3_UART2_GPIO_GPIO8283                  ((UINT32)1 << 18)

#define IO_CTRL3_UART6_GPIO8485B                  20
#define IO_CTRL3_UART6_GPIO8485                   ((UINT32)1 << 20)
#define IO_CTRL3_UART6_GPIO8485_UART6             ((UINT32)0 << 20)
#define IO_CTRL3_UART6_GPIO8485_GPIO              ((UINT32)1 << 20)

#define IO_CTRL3_I2C0_GPIO8687B                 22
#define IO_CTRL3_I2C0_GPIO8687                  ((UINT32)1 << 22)
#define IO_CTRL3_I2C0_GPIO8687_I2C0             ((UINT32)0 << 22)
#define IO_CTRL3_I2C0_GPIO8687_GPIO             ((UINT32)1 << 22)

#define IO_CTRL3_UART7_CAN1_GPIO8889BF            24
#define IO_CTRL3_UART7_CAN1_GPIO8889              ((UINT32)3 << 24)
#define IO_CTRL3_UART7_CAN1_GPIO8889_UART7        ((UINT32)0 << 24)
#define IO_CTRL3_UART7_CAN1_GPIO8889_CAN1         ((UINT32)1 << 24)
#define IO_CTRL3_UART7_CAN1_GPIO8889_GPIO         ((UINT32)2 << 24)

#define IO_CTRL3_SPI2_GPIO9093BF            26
#define IO_CTRL3_SPI2_GPIO9093              ((UINT32)1 << 26)
#define IO_CTRL3_SPI2_GPIO9093_SPI2         ((UINT32)0 << 26)
#define IO_CTRL3_SPI2_GPIO9093_GPIO         ((UINT32)1 << 26)

#define IO_CTRL3_UART5_GPIO9495B            27
#define IO_CTRL3_UART5_GPIO9495             ((UINT32)1 << 27)
#define IO_CTRL3_UART5_GPIO9495_UART5       ((UINT32)0 << 27)
#define IO_CTRL3_UART5_GPIO9495_GPIO        ((UINT32)1 << 27)


//IO_DS
#define IO_DS_SPI0BF              0  
#define IO_DS_SPI0               (3 << 0)   
#define IO_DS_SPI0_4P5           (0 << 0)  
#define IO_DS_SPI0_9             (1 << 0)
#define IO_DS_SPI0_13P5          (2 << 0)
#define IO_DS_SPI0_18            (3 << 0)

#define IO_DS_SPI1BF              2  
#define IO_DS_SPI1               (3 << 2)   
#define IO_DS_SPI1_4P5           (0 << 2)  
#define IO_DS_SPI1_9             (1 << 2)
#define IO_DS_SPI1_13P5          (2 << 2)
#define IO_DS_SPI1_18            (3 << 2)

#define IO_DS_SPI2BF              4  
#define IO_DS_SPI2               (3 << 4)   
#define IO_DS_SPI2_4P5           (0 << 4)  
#define IO_DS_SPI2_9             (1 << 4)
#define IO_DS_SPI2_13P5          (2 << 4)
#define IO_DS_SPI2_18            (3 << 4)

#define IO_DS1_SPI3BF              6  
#define IO_DS1_SPI3               (3 << 6)   
#define IO_DS1_SPI3_4P5           (0 << 6)  
#define IO_DS1_SPI3_9             (1 << 6)
#define IO_DS1_SPI3_13P5          (2 << 6)
#define IO_DS1_SPI3_18            (3 << 6)

#define IO_DS2_SPI3BF              8  
#define IO_DS2_SPI3               (3 << 8)   
#define IO_DS2_SPI3_4P5           (0 << 8)  
#define IO_DS2_SPI3_9             (1 << 8)
#define IO_DS2_SPI3_13P5          (2 << 8)
#define IO_DS2_SPI3_18            (3 << 8)

#define IO_DS_MII0BF              10  
#define IO_DS_MII0               (3 << 10)   
#define IO_DS_MII0_4P5           (0 << 10)  
#define IO_DS_MII0_9             (1 << 10)
#define IO_DS_MII0_13P5          (2 << 10)
#define IO_DS_MII0_18            (3 << 10)

#define IO_DS_MII1BF              12  
#define IO_DS_MII1               (3 << 12)   
#define IO_DS_MII1_4P5           (0 << 12)  
#define IO_DS_MII1_9             (1 << 12)
#define IO_DS_MII1_13P5          (2 << 12)
#define IO_DS_MII1_18            (3 << 12)

//IO_PU1/IO_PD1
#define IO_PUD1_GPIO0              (1 << 0)  
#define IO_PUD1_GPIO1              (1 << 1)
#define IO_PUD1_GPIO2              (1 << 2)  
#define IO_PUD1_GPIO3              (1 << 3)  
#define IO_PUD1_GPIO4              (1 << 4)  
#define IO_PUD1_GPIO5              (1 << 5)
#define IO_PUD1_GPIO6              (1 << 6)  
#define IO_PUD1_GPIO7              (1 << 7)  
#define IO_PUD1_GPIO8              (1 << 8)  
#define IO_PUD1_GPIO9              (1 << 9)
#define IO_PUD1_GPIO10             (1 << 10)  
#define IO_PUD1_GPIO11             (1 << 11)  
#define IO_PUD1_GPIO12             (1 << 12)  
#define IO_PUD1_GPIO13             (1 << 13)
#define IO_PUD1_GPIO14             (1 << 14)  
#define IO_PUD1_GPIO15             (1 << 15)  
#define IO_PUD1_GPIO16             ((UINT32)1 << 16)  
#define IO_PUD1_SPI3_PCS           ((UINT32)1 << 17)  
#define IO_PUD1_SPI3_SOUT          ((UINT32)1 << 18)  
#define IO_PUD1_SPI3_SCK           ((UINT32)1 << 19)  
#define IO_PUD1_SPI3_SIN           ((UINT32)1 << 20)  
#define IO_PUD1_UART1_TX           ((UINT32)1 << 21)  
#define IO_PUD1_UART1_RX           ((UINT32)1 << 22)  
#define IO_PUD1_UART3_TX           ((UINT32)1 << 23)  
#define IO_PUD1_UART3_RX           ((UINT32)1 << 24)
#define IO_PUD1_DRVVBUS            ((UINT32)1 << 25)  
#define IO_PUD1_UART4_TX           ((UINT32)1 << 26)  
#define IO_PUD1_UART4_RX           ((UINT32)1 << 27)
#define IO_PUD1_MII0_RXER          ((UINT32)1 << 28)
#define IO_PUD1_PWM0               ((UINT32)1 << 29)
#define IO_PUD1_PWM1               ((UINT32)1 << 30)
#define IO_PUD1_MII0_RXD3          ((UINT32)1 << 31)

//IO_PU2/IO_PD2
#define IO_PUD2_MII0_RXD2                      (1 << 0)  
#define IO_PUD2_MII0_RXCLK                     (1 << 1)  
#define IO_PUD2_MII0_RXD1                      (1 << 2)  
#define IO_PUD2_MII0_RXD0                      (1 << 3)  
#define IO_PUD2_RMII0_CRS_DV_MII0_RXDV         (1 << 4)  
#define IO_PUD2_MII0_TXEN                      (1 << 5)  
#define IO_PUD2_MII0_TXD0                      (1 << 6)  
#define IO_PUD2_MII0_TXD1                      (1 << 7)  
#define IO_PUD2_I2C2_SDA                       (1 << 8)  
#define IO_PUD2_I2C2_SCL                       (1 << 9)  
#define IO_PUD2_MII0_TXD2                      (1 << 10)
#define IO_PUD2_MII0_TXCLK                     (1 << 11)  
#define IO_PUD2_MII0_TXD3                      (1 << 12)
#define IO_PUD2_MII0_CRS                       (1 << 13)
#define IO_PUD2_MII0_TXER                      (1 << 14)
#define IO_PUD2_MII0_COL                       (1 << 15)
#define IO_PUD2_MII0_MDIO                      ((UINT32)1 << 16)
#define IO_PUD2_MII0_MDC                       ((UINT32)1 << 17)
#define IO_PUD2_SPI1_PCS0                      ((UINT32)1 << 18)
#define IO_PUD2_SPI1_SCK                       ((UINT32)1 << 19)
#define IO_PUD2_SPI1_SOUT                      ((UINT32)1 << 20)
#define IO_PUD2_SPI1_SIN                       ((UINT32)1 << 21)
#define IO_PUD2_RMII1_RXER                     ((UINT32)1 << 22)
#define IO_PUD2_MII1_RXD3                      ((UINT32)1 << 23)
#define IO_PUD2_MII1_RXD2                      ((UINT32)1 << 24)
#define IO_PUD2_MII1_RXCLK                     ((UINT32)1 << 25)
#define IO_PUD2_MII1_RXD1                      ((UINT32)1 << 26)
#define IO_PUD2_MII1_RXD0                      ((UINT32)1 << 27)
#define IO_PUD2_RMII1_CRS_DV_MII1_RXDV         ((UINT32)1 << 28)  
#define IO_PUD2_MII1_TXEN                      ((UINT32)1 << 29)  
#define IO_PUD2_MII1_TXD0                      ((UINT32)1 << 30)  
#define IO_PUD2_MII1_TXD1                      ((UINT32)1 << 31)

//IO_PU3/IO_PD3
#define IO_PUD3_MII1_TXD2                      (1 << 0) 
#define IO_PUD3_MII1_TXCLK                     (1 << 1) 
#define IO_PUD3_MII1_TXD3                      (1 << 2) 
#define IO_PUD3_MII1_CRS                       (1 << 3) 
#define IO_PUD3_MII1_TXER                      (1 << 4) 
#define IO_PUD3_MII1_COL                       (1 << 5) 
#define IO_PUD3_MII1_MDIO                      (1 << 6) 
#define IO_PUD3_MII1_MDC                       (1 << 7) 
#define IO_PUD3_I2C1_SCL                       (1 << 8)  
#define IO_PUD3_I2C1_SDA                       (1 << 9)
#define IO_PUD3_UART0_TX                       (1 << 10)  
#define IO_PUD3_UART0_RX                       (1 << 11) 
#define IO_PUD3_PWM2                           (1 << 12)
#define IO_PUD3_PWM3                           (1 << 13)
#define IO_PUD3_SPI0_PCS                       (1 << 14)
#define IO_PUD3_SPI0_SCK                       (1 << 15)
#define IO_PUD3_SPI0_SOUT                      ((UINT32)1 << 16)
#define IO_PUD3_SPI0_SIN                       ((UINT32)1 << 17)
#define IO_PUD3_UART2_RX                       ((UINT32)1 << 18)  
#define IO_PUD3_UART2_TX                       ((UINT32)1 << 19)
#define IO_PUD3_UART6_RX                       ((UINT32)1 << 20)  
#define IO_PUD3_UART6_TX                       ((UINT32)1 << 21)
#define IO_PUD3_I2C0_SCL                       ((UINT32)1 << 22)  
#define IO_PUD3_I2C0_SDA                       ((UINT32)1 << 23)
#define IO_PUD3_UART7_RX                       ((UINT32)1 << 24)  
#define IO_PUD3_UART7_TX                       ((UINT32)1 << 25)
#define IO_PUD3_SPI2_SCK                       ((UINT32)1 << 26)
#define IO_PUD3_SPI2_SOUT                      ((UINT32)1 << 27)
#define IO_PUD3_SPI2_SIN                       ((UINT32)1 << 28)
#define IO_PUD3_SPI2_PCS                       ((UINT32)1 << 29)
#define IO_PUD3_UART5_TX                       ((UINT32)1 << 30)  
#define IO_PUD3_UART5_RX                       ((UINT32)1 << 31)

//IO_PU4/IO_PD4
#define IO_PUD4_GPIO96                      (1 << 0)
#define IO_PUD4_GPIO97                      (1 << 1)
#define IO_PUD4_GPIO98                      (1 << 2)
#define IO_PUD4_GPIO99                      (1 << 3)
#define IO_PUD4_GPIO100                     (1 << 4)
#define IO_PUD4_GPIO101                     (1 << 5)
#define IO_PUD4_GPIO102                     (1 << 6)
#define IO_PUD4_GPIO103                     (1 << 7)
#define IO_PUD4_GPIO104                     (1 << 8)
#define IO_PUD4_GPIO105                     (1 << 9)
#define IO_PUD4_GPIO106                     (1 << 10)
#define IO_PUD4_GPIO107                     (1 << 11)
#define IO_PUD4_GPIO108                     (1 << 12)
#define IO_PUD4_GPIO109                     (1 << 13)
#define IO_PUD4_GPIO110                     (1 << 14)
#define IO_PUD4_GPIO111                     (1 << 15)
#define IO_PUD4_GPIO112                     ((UINT32)1 << 16)
#define IO_PUD4_GPIO113                     ((UINT32)1 << 17)

 
/* -------------------------------------
                 ANA_CTRL
---------------------------------------*/
//ANA_CFG_REG
#define ANA_CFG_REG_ADC_CHN_INTENB  2
#define ANA_CFG_REG_ADC_CHN_INTEN   (1 << 2)

#define ANA_CFG_REG_CALIB_INTENB    1
#define ANA_CFG_REG_CALIB_INTEN     (1 << 1)

#define ANA_CFG_REG_ADC_INTENB    0
#define ANA_CFG_REG_ADC_INTEN     (1 << 0)

//ANA_STAT_REG
#define ANA_STAT_REG_CHSBF         24
#define ANA_STAT_REG_CHS           ((UINT32)0xFF << 24)

#define ANA_STAT_REG_CALIBREB         23
#define ANA_STAT_REG_CALIBRE         ((UINT32)1 << 23)

#define ANA_STAT_REG_SMP_PULSEB         22
#define ANA_STAT_REG_SMP_PULSE        ((UINT32)1 << 22)

#define ANA_STAT_REG_CONVSTARTB         21
#define ANA_STAT_REG_CONVSTART         ((UINT32)1 << 21)

#define ANA_STAT_REG_RSTNB         20
#define ANA_STAT_REG_RSTN         ((UINT32)1 << 20)

#define ANA_STAT_REG_CSB          19
#define ANA_STAT_REG_CS           ((UINT32)1 << 19)

#define ANA_STAT_REG_CHN_UPDATEDB          18
#define ANA_STAT_REG_CHN_UPDATED          ((UINT32)1 << 18)

#define ANA_STAT_REG_CALIB_FAILB          17
#define ANA_STAT_REG_CALIB_FAIL           ((UINT32)1 << 17)

#define ANA_STAT_REG_ADC_ALLINT          0x3FFF

#define ANA_STAT_REG_ADC_CHN_INTB          13
#define ANA_STAT_REG_ADC_CHN_INT          (1 << 13)

#define ANA_STAT_REG_CALIB_INTB          12
#define ANA_STAT_REG_CALIB_INT          (1 << 12)

#define ANA_STAT_REG_ADC11_INTB          11
#define ANA_STAT_REG_ADC11_INT          (1 << 11)

#define ANA_STAT_REG_ADC10_INTB          10
#define ANA_STAT_REG_ADC10_INT          (1 << 10)

#define ANA_STAT_REG_ADC9_INTB          9
#define ANA_STAT_REG_ADC9_INT          (1 << 9)

#define ANA_STAT_REG_ADC8_INTB          8
#define ANA_STAT_REG_ADC8_INT          (1 << 8)

#define ANA_STAT_REG_ADC7_INTB          7
#define ANA_STAT_REG_ADC7_INT          (1 << 7)

#define ANA_STAT_REG_ADC6_INTB          6
#define ANA_STAT_REG_ADC6_INT          (1 << 6)

#define ANA_STAT_REG_ADC5_INTB          5
#define ANA_STAT_REG_ADC5_INT          (1 << 5)

#define ANA_STAT_REG_ADC4_INTB          4
#define ANA_STAT_REG_ADC4_INT          (1 << 4)

#define ANA_STAT_REG_ADC3_INTB          3
#define ANA_STAT_REG_ADC3_INT          (1 << 3)

#define ANA_STAT_REG_ADC2_INTB          2
#define ANA_STAT_REG_ADC2_INT          (1 << 2)

#define ANA_STAT_REG_ADC1_INTB          1
#define ANA_STAT_REG_ADC1_INT          (1 << 1)

#define ANA_STAT_REG_ADC0_INTB          0
#define ANA_STAT_REG_ADC0_INT          (1 << 0)

//ADC_TRIM
#define ADC_TRIM_CALIB_A0BF            16
#define ADC_TRIM_CALIB_A0            ((UINT32)0xf << 16)
#define ADC_TRIM_CALIB_A0_M3         ((UINT32)0xB << 16)
#define ADC_TRIM_CALIB_A0_M2         ((UINT32)0xA << 16)
#define ADC_TRIM_CALIB_A0_M1         ((UINT32)0x9 << 16)
#define ADC_TRIM_CALIB_A0_0         ((UINT32)0x0 << 16)
#define ADC_TRIM_CALIB_A0_P1         ((UINT32)0x1 << 16)
#define ADC_TRIM_CALIB_A0_P2         ((UINT32)0x2 << 16)
#define ADC_TRIM_CALIB_A0_P3         ((UINT32)0x3 << 16)
#define ADC_TRIM_CALIB_A0_P4         ((UINT32)0x4 << 16)

#define ADC_TRIM_CALIB_A1BF            12
#define ADC_TRIM_CALIB_A1            ((UINT32)0xf << 12)
#define ADC_TRIM_CALIB_A1_M3         ((UINT32)0xB << 12)
#define ADC_TRIM_CALIB_A1_M2         ((UINT32)0xA << 12)
#define ADC_TRIM_CALIB_A1_M1         ((UINT32)0x9 << 12)
#define ADC_TRIM_CALIB_A1_0         ((UINT32)0x0 << 12)
#define ADC_TRIM_CALIB_A1_P1         ((UINT32)0x1 << 12)
#define ADC_TRIM_CALIB_A1_P2         ((UINT32)0x2 << 12)
#define ADC_TRIM_CALIB_A1_P3         ((UINT32)0x3 << 12)
#define ADC_TRIM_CALIB_A1_P4         ((UINT32)0x4 << 12)

#define ADC_TRIM_TRIMBF            2
#define ADC_TRIM_TRIM             (0x7f << 2)
#define ADC_TRIM_TRIM_(n)           (n << 2)

#define ADC_TRIM_IBB                1
#define ADC_TRIM_IB_SMALLI          (0 << 1)
#define ADC_TRIM_IB_BIGI            (1 << 1)

//ADC_RDY_REG
#define ADC_RDY_REG_CH11            (1 << 11)
#define ADC_RDY_REG_CH10            (1 << 10)
#define ADC_RDY_REG_CH9            (1 << 9)
#define ADC_RDY_REG_CH8            (1 << 8)
#define ADC_RDY_REG_CH7            (1 << 7)
#define ADC_RDY_REG_CH6            (1 << 6)
#define ADC_RDY_REG_CH5            (1 << 5)
#define ADC_RDY_REG_CH4            (1 << 4)
#define ADC_RDY_REG_CH3            (1 << 3)
#define ADC_RDY_REG_CH2            (1 << 2)
#define ADC_RDY_REG_CH1            (1 << 1)
#define ADC_RDY_REG_CH0            (1 << 0)


//ADC_CON1
#define ADC_CON1_SMP_WIDTHBF         8
#define ADC_CON1_SMP_WIDTH          (0xFF << 8)
#define ADC_CON1_SMP_WIDTH_(n)       (n << 8)

#define ADC_CON1_AVGTIMESBF         6
#define ADC_CON1_AVGTIMES           (3 << 6)
#define ADC_CON1_AVGTIMES_1         (0 << 6)
#define ADC_CON1_AVGTIMES_2         (1 << 6)
#define ADC_CON1_AVGTIMES_4         (2 << 6)
#define ADC_CON1_AVGTIMES_8         (3 << 6)

#define ADC_CON1_ADC_MODEBF         4
#define ADC_CON1_ADC_MODE           (3 << 4)
#define ADC_CON1_ADC_MODE_0         (0 << 4)
#define ADC_CON1_ADC_MODE_1         (1 << 4)
#define ADC_CON1_ADC_MODE_2         (2 << 4)
#define ADC_CON1_ADC_MODE_3         (3 << 4)

#define ADC_CON1_TRIG_ENB          0
#define ADC_CON1_TRIG_EN          (1 << 0)
#define ADC_CON1_TRIG_DIS         (0 << 0)


//ADC_CON2
#define ADC_CON2_CALIBREB          2
#define ADC_CON2_CALIBRE_1          (1 << 2)
#define ADC_CON2_CALIBRE_0          (0 << 2)

#define ADC_CON2_RSTNB             1
#define ADC_CON2_RSTN              (1 << 1)
#define ADC_CON2_RSTN_LOW         (0 << 1)
#define ADC_CON2_RSTN_HIGH        (1 << 1)

#define ADC_CON2_CSB             0
#define ADC_CON2_CS              (1 << 0)
#define ADC_CON2_CS_LOW          (0 << 0)
#define ADC_CON2_CS_HIGH         (1 << 0)

//ADC_CON3
#define ADC_CON3_CHNCOMBINEDB             16
#define ADC_CON3_CHNCOMBINED             (1 << 16)
#define ADC_CON3_CHNCOMBINED_1           (1 << 16)
#define ADC_CON3_CHNCOMBINED_0           (0 << 16)

#define ADC_CON3_MCHSBF             0
#define ADC_CON3_MCHS              0xFFF
#define ADC_CON3_MCHS_(n)          (1 << n)
#define ADC_CON3_MCHS_ALLSINGCH        0xFF
#define ADC_CON3_MCHS_ALLDIFFCH        0xF00

//ADC_CON4
#define ADC_CON4_CALIBENB             1
#define ADC_CON4_CALIB_EN              (1 << 1)

#define ADC_CON4_ADCENB             0
#define ADC_CON4_ADCEN              (1 << 0)


//BGB01_REG
#define BGB01_REG_EN_VREF1VB       4
#define BGB01_REG_EN_VREF1V_EN     (1 << 4)
#define BGB01_REG_EN_VREF1V_DIS    (0 << 4)
     
#define BGB01_REG_TRIMBF           0
#define BGB01_REG_TRIM             0xF
#define BGB01_REG_TRIM_520mv       0x0
#define BGB01_REG_TRIM_530mv       0x1
#define BGB01_REG_TRIM_540mv       0x2
#define BGB01_REG_TRIM_550mv       0x3
#define BGB01_REG_TRIM_560mv       0x4
#define BGB01_REG_TRIM_570mv       0x5
#define BGB01_REG_TRIM_580mv       0x6
#define BGB01_REG_TRIM_590mv       0x7
#define BGB01_REG_TRIM_600mv       0x8
#define BGB01_REG_TRIM_610mv       0x9
#define BGB01_REG_TRIM_620mv       0xA
#define BGB01_REG_TRIM_630mv       0xB
#define BGB01_REG_TRIM_640mv       0xC
#define BGB01_REG_TRIM_650mv       0xD
#define BGB01_REG_TRIM_660mv       0xE
#define BGB01_REG_TRIM_670mv       0xF

//VR12_2_REG
#define VR12_2_REG_EN_VR2B       4
#define VR12_2_REG_EN_VR_EN      (1 << 4)
#define VR12_2_REG_EN_VR_DIS     (0 << 4)

#define VR12_2_REG_TRIMBF        0x0
#define VR12_2_REG_TRIM          0xF
#define VR12_2_REG_TRIM_1040mv         0x0
#define VR12_2_REG_TRIM_1060mv         0x1
#define VR12_2_REG_TRIM_1080mv         0x2
#define VR12_2_REG_TRIM_1100mv         0x3
#define VR12_2_REG_TRIM_1120mv         0x4
#define VR12_2_REG_TRIM_1140mv         0x5
#define VR12_2_REG_TRIM_1160mv         0x6
#define VR12_2_REG_TRIM_1180mv         0x7
#define VR12_2_REG_TRIM_1200mv         0x8
#define VR12_2_REG_TRIM_1220mv         0x9
#define VR12_2_REG_TRIM_1240mv         0xA
#define VR12_2_REG_TRIM_1260mv         0xB
#define VR12_2_REG_TRIM_1280mv         0xC
#define VR12_2_REG_TRIM_1300mv         0xD
#define VR12_2_REG_TRIM_1320mv         0xE
#define VR12_2_REG_TRIM_1340mv         0xF

//VR16_REG
#define VR16_REG_ENB                    3
#define VR16_REG_EN                     (1 << 3)
#define VR16_REG_DIS                    (0 << 3)

#define VR16_REG_TRIMBF                  0
#define VR16_REG_TRIM                    0x7
#define VR16_REG_TRIM_1343mv             0x0
#define VR16_REG_TRIM_1429mv             0x1
#define VR16_REG_TRIM_1515mv             0x2
#define VR16_REG_TRIM_1601mv             0x3
#define VR16_REG_TRIM_1687mv             0x4
#define VR16_REG_TRIM_1773mv             0x5
#define VR16_REG_TRIM_1859mv             0x6
#define VR16_REG_TRIM_1945mv             0x7

//HOSC_REG
#define HOSC_REG_TRIMBF                  0x4
#define HOSC_REG_TRIM                    (0xF << 4)
#define HOSC_REG_TRIM_0                  (0x0 << 4)
#define HOSC_REG_TRIM_1                  (0x1 << 4)
#define HOSC_REG_TRIM_2                  (0x2 << 4)
#define HOSC_REG_TRIM_3                  (0x3 << 4)
#define HOSC_REG_TRIM_4                  (0x4 << 4)
#define HOSC_REG_TRIM_5                  (0x5 << 4)
#define HOSC_REG_TRIM_6                  (0x6 << 4)
#define HOSC_REG_TRIM_7                  (0x7 << 4)
#define HOSC_REG_TRIM_8                  (0x8 << 4)
#define HOSC_REG_TRIM_9                  (0x9 << 4)
#define HOSC_REG_TRIM_10                  (0xA << 4)
#define HOSC_REG_TRIM_11                  (0xB << 4)
#define HOSC_REG_TRIM_12                  (0xC << 4)
#define HOSC_REG_TRIM_13                  (0xD << 4)
#define HOSC_REG_TRIM_14                  (0xE << 4)
#define HOSC_REG_TRIM_15                  (0xF << 4)

#define HOSC_REG_OPTBF                    0x2
#define HOSC_REG_OPT                      (3 << 2)
#define HOSC_REG_OPT_60MHz                (0 << 2)
#define HOSC_REG_OPT_80MHz                (2 << 2)
#define HOSC_REG_OPT_100MHz               (3 << 2)

#define HOSC_REG_ITRIMB                    0x1
#define HOSC_REG_ITRIM                     (1 << 1)
#define HOSC_REG_ITRIM_BIGI                (1 << 1)
#define HOSC_REG_ITRIM_SMALLI              (0 << 1)

//XLOSC_REG

//XHOSC_REG

//ANATEST_REG
 
/******************************************************************************/
/*                Ethernet MAC Registers bits definitions                     */
/******************************************************************************/
/* Bit definition for Ethernet MAC Control Register register */
#define ETH_MACCR_WD      ((uint32_t)0x00800000)  /* Watchdog disable */
#define ETH_MACCR_JD      ((uint32_t)0x00400000)  /* Jabber disable */
#define ETH_MACCR_IFG     ((uint32_t)0x000E0000)  /* Inter-frame gap */
  #define ETH_MACCR_IFG_96Bit     ((uint32_t)0x00000000)  /* Minimum IFG between frames during transmission is 96Bit */
  #define ETH_MACCR_IFG_88Bit     ((uint32_t)0x00020000)  /* Minimum IFG between frames during transmission is 88Bit */
  #define ETH_MACCR_IFG_80Bit     ((uint32_t)0x00040000)  /* Minimum IFG between frames during transmission is 80Bit */
  #define ETH_MACCR_IFG_72Bit     ((uint32_t)0x00060000)  /* Minimum IFG between frames during transmission is 72Bit */
  #define ETH_MACCR_IFG_64Bit     ((uint32_t)0x00080000)  /* Minimum IFG between frames during transmission is 64Bit */        
  #define ETH_MACCR_IFG_56Bit     ((uint32_t)0x000A0000)  /* Minimum IFG between frames during transmission is 56Bit */
  #define ETH_MACCR_IFG_48Bit     ((uint32_t)0x000C0000)  /* Minimum IFG between frames during transmission is 48Bit */
  #define ETH_MACCR_IFG_40Bit     ((uint32_t)0x000E0000)  /* Minimum IFG between frames during transmission is 40Bit */              
#define ETH_MACCR_CSD     ((uint32_t)0x00010000)  /* Carrier sense disable (during transmission) */
#define ETH_MACCR_FES     ((uint32_t)0x00004000)  /* Fast ethernet speed */
#define ETH_MACCR_ROD     ((uint32_t)0x00002000)  /* Receive own disable */
#define ETH_MACCR_LM      ((uint32_t)0x00001000)  /* loopback mode */
#define ETH_MACCR_DM      ((uint32_t)0x00000800)  /* Duplex mode */
#define ETH_MACCR_IPCO    ((uint32_t)0x00000400)  /* IP Checksum offload */
#define ETH_MACCR_RD      ((uint32_t)0x00000200)  /* Retry disable */
#define ETH_MACCR_APCS    ((uint32_t)0x00000080)  /* Automatic Pad/CRC stripping */
#define ETH_MACCR_BL      ((uint32_t)0x00000060)  /* Back-off limit: random integer number (r) of slot time delays before rescheduling
                                                       a transmission attempt during retries after a collision: 0 =< r <2^k */
  #define ETH_MACCR_BL_10    ((uint32_t)0x00000000)  /* k = min (n, 10) */
  #define ETH_MACCR_BL_8     ((uint32_t)0x00000020)  /* k = min (n, 8) */
  #define ETH_MACCR_BL_4     ((uint32_t)0x00000040)  /* k = min (n, 4) */
  #define ETH_MACCR_BL_1     ((uint32_t)0x00000060)  /* k = min (n, 1) */ 
#define ETH_MACCR_DC      ((uint32_t)0x00000010)  /* Defferal check */
#define ETH_MACCR_TE      ((uint32_t)0x00000008)  /* Transmitter enable */
#define ETH_MACCR_RE      ((uint32_t)0x00000004)  /* Receiver enable */

/* Bit definition for Ethernet MAC Frame Filter Register */
#define ETH_MACFFR_RA     ((uint32_t)0x80000000)  /* Receive all */ 
#define ETH_MACFFR_HPF    ((uint32_t)0x00000400)  /* Hash or perfect filter */ 
#define ETH_MACFFR_SAF    ((uint32_t)0x00000200)  /* Source address filter enable */ 
#define ETH_MACFFR_SAIF   ((uint32_t)0x00000100)  /* SA inverse filtering */ 
#define ETH_MACFFR_PCF    ((uint32_t)0x000000C0)  /* Pass control frames: 3 cases */
  #define ETH_MACFFR_PCF_BlockAll                ((uint32_t)0x00000040)  /* MAC filters all control frames from reaching the application */
  #define ETH_MACFFR_PCF_ForwardAll              ((uint32_t)0x00000080)  /* MAC forwards all control frames to application even if they fail the Address Filter */
  #define ETH_MACFFR_PCF_ForwardPassedAddrFilter ((uint32_t)0x000000C0)  /* MAC forwards control frames that pass the Address Filter. */ 
#define ETH_MACFFR_BFD    ((uint32_t)0x00000020)  /* Broadcast frame disable */ 
#define ETH_MACFFR_PAM 	  ((uint32_t)0x00000010)  /* Pass all mutlicast */ 
#define ETH_MACFFR_DAIF   ((uint32_t)0x00000008)  /* DA Inverse filtering */ 
#define ETH_MACFFR_HM     ((uint32_t)0x00000004)  /* Hash multicast */ 
#define ETH_MACFFR_HU     ((uint32_t)0x00000002)  /* Hash unicast */
#define ETH_MACFFR_PM     ((uint32_t)0x00000001)  /* Promiscuous mode */

/* Bit definition for Ethernet MAC Hash Table High Register */
#define ETH_MACHTHR_HTH   ((uint32_t)0xFFFFFFFF)  /* Hash table high */

/* Bit definition for Ethernet MAC Hash Table Low Register */
#define ETH_MACHTLR_HTL   ((uint32_t)0xFFFFFFFF)  /* Hash table low */

/* Bit definition for Ethernet MAC MII Address Register */
#define ETH_MACMIIAR_PA   ((uint32_t)0x0000F800)  /* Physical layer address */ 
#define ETH_MACMIIAR_MR   ((uint32_t)0x000007C0)  /* MII register in the selected PHY */ 
#define ETH_MACMIIAR_CR   ((uint32_t)0x0000001C)  /* CR clock range: 6 cases */ 
  #define ETH_MACMIIAR_CR_Div42   ((uint32_t)0x00000000)  /* HCLK:60-72 MHz; MDC clock= HCLK/42 */
  #define ETH_MACMIIAR_CR_Div16   ((uint32_t)0x00000008)  /* HCLK:20-35 MHz; MDC clock= HCLK/16 */
  #define ETH_MACMIIAR_CR_Div26   ((uint32_t)0x0000000C)  /* HCLK:35-60 MHz; MDC clock= HCLK/26 */
#define ETH_MACMIIAR_MW   ((uint32_t)0x00000002)  /* MII write */ 
#define ETH_MACMIIAR_MB   ((uint32_t)0x00000001)  /* MII busy */ 
  
/* Bit definition for Ethernet MAC MII Data Register */
#define ETH_MACMIIDR_MD   ((uint32_t)0x0000FFFF)  /* MII data: read/write data from/to PHY */

/* Bit definition for Ethernet MAC Flow Control Register */
#define ETH_MACFCR_PT     ((uint32_t)0xFFFF0000)  /* Pause time */
#define ETH_MACFCR_ZQPD   ((uint32_t)0x00000080)  /* Zero-quanta pause disable */
#define ETH_MACFCR_PLT    ((uint32_t)0x00000030)  /* Pause low threshold: 4 cases */
  #define ETH_MACFCR_PLT_Minus4   ((uint32_t)0x00000000)  /* Pause time minus 4 slot times */
  #define ETH_MACFCR_PLT_Minus28  ((uint32_t)0x00000010)  /* Pause time minus 28 slot times */
  #define ETH_MACFCR_PLT_Minus144 ((uint32_t)0x00000020)  /* Pause time minus 144 slot times */
  #define ETH_MACFCR_PLT_Minus256 ((uint32_t)0x00000030)  /* Pause time minus 256 slot times */      
#define ETH_MACFCR_UPFD   ((uint32_t)0x00000008)  /* Unicast pause frame detect */
#define ETH_MACFCR_RFCE   ((uint32_t)0x00000004)  /* Receive flow control enable */
#define ETH_MACFCR_TFCE   ((uint32_t)0x00000002)  /* Transmit flow control enable */
#define ETH_MACFCR_FCBBPA ((uint32_t)0x00000001)  /* Flow control busy/backpressure activate */

/* Bit definition for Ethernet MAC VLAN Tag Register */
#define ETH_MACVLANTR_VLANTC ((uint32_t)0x00010000)  /* 12-bit VLAN tag comparison */
#define ETH_MACVLANTR_VLANTI ((uint32_t)0x0000FFFF)  /* VLAN tag identifier (for receive frames) */

/* Bit definition for Ethernet MAC Remote Wake-UpFrame Filter Register */ 
#define ETH_MACRWUFFR_D   ((uint32_t)0xFFFFFFFF)  /* Wake-up frame filter register data */
/* Eight sequential Writes to this address (offset 0x28) will write all Wake-UpFrame Filter Registers.
   Eight sequential Reads from this address (offset 0x28) will read all Wake-UpFrame Filter Registers. */
/* Wake-UpFrame Filter Reg0 : Filter 0 Byte Mask
   Wake-UpFrame Filter Reg1 : Filter 1 Byte Mask
   Wake-UpFrame Filter Reg2 : Filter 2 Byte Mask
   Wake-UpFrame Filter Reg3 : Filter 3 Byte Mask
   Wake-UpFrame Filter Reg4 : RSVD - Filter3 Command - RSVD - Filter2 Command - 
                              RSVD - Filter1 Command - RSVD - Filter0 Command
   Wake-UpFrame Filter Re5 : Filter3 Offset - Filter2 Offset - Filter1 Offset - Filter0 Offset
   Wake-UpFrame Filter Re6 : Filter1 CRC16 - Filter0 CRC16
   Wake-UpFrame Filter Re7 : Filter3 CRC16 - Filter2 CRC16 */

/* Bit definition for Ethernet MAC PMT Control and Status Register */ 
#define ETH_MACPMTCSR_WFFRPR ((uint32_t)0x80000000)  /* Wake-Up Frame Filter Register Pointer Reset */
#define ETH_MACPMTCSR_GU     ((uint32_t)0x00000200)  /* Global Unicast */
#define ETH_MACPMTCSR_WFR    ((uint32_t)0x00000040)  /* Wake-Up Frame Received */
#define ETH_MACPMTCSR_MPR    ((uint32_t)0x00000020)  /* Magic Packet Received */
#define ETH_MACPMTCSR_WFE    ((uint32_t)0x00000004)  /* Wake-Up Frame Enable */
#define ETH_MACPMTCSR_MPE    ((uint32_t)0x00000002)  /* Magic Packet Enable */
#define ETH_MACPMTCSR_PD     ((uint32_t)0x00000001)  /* Power Down */

/* Bit definition for Ethernet MAC Status Register */
#define ETH_MACSR_TSTS      ((uint32_t)0x00000200)  /* Time stamp trigger status */
#define ETH_MACSR_MMCTS     ((uint32_t)0x00000040)  /* MMC transmit status */
#define ETH_MACSR_MMMCRS    ((uint32_t)0x00000020)  /* MMC receive status */
#define ETH_MACSR_MMCS      ((uint32_t)0x00000010)  /* MMC status */
#define ETH_MACSR_PMTS      ((uint32_t)0x00000008)  /* PMT status */

/* Bit definition for Ethernet MAC Interrupt Mask Register */
#define ETH_MACIMR_TSTIM     ((uint32_t)0x00000200)  /* Time stamp trigger interrupt mask */
#define ETH_MACIMR_PMTIM     ((uint32_t)0x00000008)  /* PMT interrupt mask */

/* Bit definition for Ethernet MAC Address0 High Register */
#define ETH_MACA0HR_MACA0H   ((uint32_t)0x0000FFFF)  /* MAC address0 high */

/* Bit definition for Ethernet MAC Address0 Low Register */
#define ETH_MACA0LR_MACA0L   ((uint32_t)0xFFFFFFFF)  /* MAC address0 low */

/* Bit definition for Ethernet MAC Address1 High Register */
#define ETH_MACA1HR_AE       ((uint32_t)0x80000000)  /* Address enable */
#define ETH_MACA1HR_SA       ((uint32_t)0x40000000)  /* Source address */
#define ETH_MACA1HR_MBC      ((uint32_t)0x3F000000)  /* Mask byte control: bits to mask for comparison of the MAC Address bytes */
  #define ETH_MACA1HR_MBC_HBits15_8    ((uint32_t)0x20000000)  /* Mask MAC Address high reg bits [15:8] */
  #define ETH_MACA1HR_MBC_HBits7_0     ((uint32_t)0x10000000)  /* Mask MAC Address high reg bits [7:0] */
  #define ETH_MACA1HR_MBC_LBits31_24   ((uint32_t)0x08000000)  /* Mask MAC Address low reg bits [31:24] */
  #define ETH_MACA1HR_MBC_LBits23_16   ((uint32_t)0x04000000)  /* Mask MAC Address low reg bits [23:16] */
  #define ETH_MACA1HR_MBC_LBits15_8    ((uint32_t)0x02000000)  /* Mask MAC Address low reg bits [15:8] */
  #define ETH_MACA1HR_MBC_LBits7_0     ((uint32_t)0x01000000)  /* Mask MAC Address low reg bits [7:0] */ 
#define ETH_MACA1HR_MACA1H   ((uint32_t)0x0000FFFF)  /* MAC address1 high */

/* Bit definition for Ethernet MAC Address1 Low Register */
#define ETH_MACA1LR_MACA1L   ((uint32_t)0xFFFFFFFF)  /* MAC address1 low */

/* Bit definition for Ethernet MAC Address2 High Register */
#define ETH_MACA2HR_AE       ((uint32_t)0x80000000)  /* Address enable */
#define ETH_MACA2HR_SA       ((uint32_t)0x40000000)  /* Source address */
#define ETH_MACA2HR_MBC      ((uint32_t)0x3F000000)  /* Mask byte control */
  #define ETH_MACA2HR_MBC_HBits15_8    ((uint32_t)0x20000000)  /* Mask MAC Address high reg bits [15:8] */
  #define ETH_MACA2HR_MBC_HBits7_0     ((uint32_t)0x10000000)  /* Mask MAC Address high reg bits [7:0] */
  #define ETH_MACA2HR_MBC_LBits31_24   ((uint32_t)0x08000000)  /* Mask MAC Address low reg bits [31:24] */
  #define ETH_MACA2HR_MBC_LBits23_16   ((uint32_t)0x04000000)  /* Mask MAC Address low reg bits [23:16] */
  #define ETH_MACA2HR_MBC_LBits15_8    ((uint32_t)0x02000000)  /* Mask MAC Address low reg bits [15:8] */
  #define ETH_MACA2HR_MBC_LBits7_0     ((uint32_t)0x01000000)  /* Mask MAC Address low reg bits [70] */
#define ETH_MACA2HR_MACA2H   ((uint32_t)0x0000FFFF)  /* MAC address1 high */

/* Bit definition for Ethernet MAC Address2 Low Register */
#define ETH_MACA2LR_MACA2L   ((uint32_t)0xFFFFFFFF)  /* MAC address2 low */

/* Bit definition for Ethernet MAC Address3 High Register */
#define ETH_MACA3HR_AE       ((uint32_t)0x80000000)  /* Address enable */
#define ETH_MACA3HR_SA       ((uint32_t)0x40000000)  /* Source address */
#define ETH_MACA3HR_MBC      ((uint32_t)0x3F000000)  /* Mask byte control */
  #define ETH_MACA3HR_MBC_HBits15_8    ((uint32_t)0x20000000)  /* Mask MAC Address high reg bits [15:8] */
  #define ETH_MACA3HR_MBC_HBits7_0     ((uint32_t)0x10000000)  /* Mask MAC Address high reg bits [7:0] */
  #define ETH_MACA3HR_MBC_LBits31_24   ((uint32_t)0x08000000)  /* Mask MAC Address low reg bits [31:24] */
  #define ETH_MACA3HR_MBC_LBits23_16   ((uint32_t)0x04000000)  /* Mask MAC Address low reg bits [23:16] */
  #define ETH_MACA3HR_MBC_LBits15_8    ((uint32_t)0x02000000)  /* Mask MAC Address low reg bits [15:8] */
  #define ETH_MACA3HR_MBC_LBits7_0     ((uint32_t)0x01000000)  /* Mask MAC Address low reg bits [70] */
#define ETH_MACA3HR_MACA3H   ((uint32_t)0x0000FFFF)  /* MAC address3 high */

/* Bit definition for Ethernet MAC Address3 Low Register */
#define ETH_MACA3LR_MACA3L   ((uint32_t)0xFFFFFFFF)  /* MAC address3 low */

/******************************************************************************/
/*                Ethernet MMC Registers bits definition                      */
/******************************************************************************/

/* Bit definition for Ethernet MMC Contol Register */
#define ETH_MMCCR_MCF        ((uint32_t)0x00000008)  /* MMC Counter Freeze */
#define ETH_MMCCR_ROR        ((uint32_t)0x00000004)  /* Reset on Read */
#define ETH_MMCCR_CSR        ((uint32_t)0x00000002)  /* Counter Stop Rollover */
#define ETH_MMCCR_CR         ((uint32_t)0x00000001)  /* Counters Reset */

/* Bit definition for Ethernet MMC Receive Interrupt Register */
#define ETH_MMCRIR_RGUFS     ((uint32_t)0x00020000)  /* Set when Rx good unicast frames counter reaches half the maximum value */
#define ETH_MMCRIR_RFAES     ((uint32_t)0x00000040)  /* Set when Rx alignment error counter reaches half the maximum value */
#define ETH_MMCRIR_RFCES     ((uint32_t)0x00000020)  /* Set when Rx crc error counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmit Interrupt Register */
#define ETH_MMCTIR_TGFS      ((uint32_t)0x00200000)  /* Set when Tx good frame count counter reaches half the maximum value */
#define ETH_MMCTIR_TGFMSCS   ((uint32_t)0x00008000)  /* Set when Tx good multi col counter reaches half the maximum value */
#define ETH_MMCTIR_TGFSCS    ((uint32_t)0x00004000)  /* Set when Tx good single col counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Receive Interrupt Mask Register */
#define ETH_MMCRIMR_RGUFM    ((uint32_t)0x00020000)  /* Mask the interrupt when Rx good unicast frames counter reaches half the maximum value */
#define ETH_MMCRIMR_RFAEM    ((uint32_t)0x00000040)  /* Mask the interrupt when when Rx alignment error counter reaches half the maximum value */
#define ETH_MMCRIMR_RFCEM    ((uint32_t)0x00000020)  /* Mask the interrupt when Rx crc error counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmit Interrupt Mask Register */
#define ETH_MMCTIMR_TGFM     ((uint32_t)0x00200000)  /* Mask the interrupt when Tx good frame count counter reaches half the maximum value */
#define ETH_MMCTIMR_TGFMSCM  ((uint32_t)0x00008000)  /* Mask the interrupt when Tx good multi col counter reaches half the maximum value */
#define ETH_MMCTIMR_TGFSCM   ((uint32_t)0x00004000)  /* Mask the interrupt when Tx good single col counter reaches half the maximum value */

/* Bit definition for Ethernet MMC Transmitted Good Frames after Single Collision Counter Register */
#define ETH_MMCTGFSCCR_TGFSCC     ((uint32_t)0xFFFFFFFF)  /* Number of successfully transmitted frames after a single collision in Half-duplex mode. */

/* Bit definition for Ethernet MMC Transmitted Good Frames after More than a Single Collision Counter Register */
#define ETH_MMCTGFMSCCR_TGFMSCC   ((uint32_t)0xFFFFFFFF)  /* Number of successfully transmitted frames after more than a single collision in Half-duplex mode. */

/* Bit definition for Ethernet MMC Transmitted Good Frames Counter Register */
#define ETH_MMCTGFCR_TGFC    ((uint32_t)0xFFFFFFFF)  /* Number of good frames transmitted. */

/* Bit definition for Ethernet MMC Received Frames with CRC Error Counter Register */
#define ETH_MMCRFCECR_RFCEC  ((uint32_t)0xFFFFFFFF)  /* Number of frames received with CRC error. */

/* Bit definition for Ethernet MMC Received Frames with Alignement Error Counter Register */
#define ETH_MMCRFAECR_RFAEC  ((uint32_t)0xFFFFFFFF)  /* Number of frames received with alignment (dribble) error */

/* Bit definition for Ethernet MMC Received Good Unicast Frames Counter Register */
#define ETH_MMCRGUFCR_RGUFC  ((uint32_t)0xFFFFFFFF)  /* Number of good unicast frames received. */

/******************************************************************************/
/*               Ethernet PTP Registers bits definition                       */
/******************************************************************************/

/* Bit definition for Ethernet PTP Time Stamp Contol Register */
#define ETH_PTPTSCR_TSARU    ((uint32_t)0x00000020)  /* Addend register update */
#define ETH_PTPTSCR_TSITE    ((uint32_t)0x00000010)  /* Time stamp interrupt trigger enable */
#define ETH_PTPTSCR_TSSTU    ((uint32_t)0x00000008)  /* Time stamp update */
#define ETH_PTPTSCR_TSSTI    ((uint32_t)0x00000004)  /* Time stamp initialize */
#define ETH_PTPTSCR_TSFCU    ((uint32_t)0x00000002)  /* Time stamp fine or coarse update */
#define ETH_PTPTSCR_TSE      ((uint32_t)0x00000001)  /* Time stamp enable */

/* Bit definition for Ethernet PTP Sub-Second Increment Register */
#define ETH_PTPSSIR_STSSI    ((uint32_t)0x000000FF)  /* System time Sub-second increment value */

/* Bit definition for Ethernet PTP Time Stamp High Register */
#define ETH_PTPTSHR_STS      ((uint32_t)0xFFFFFFFF)  /* System Time second */

/* Bit definition for Ethernet PTP Time Stamp Low Register */
#define ETH_PTPTSLR_STPNS    ((uint32_t)0x80000000)  /* System Time Positive or negative time */
#define ETH_PTPTSLR_STSS     ((uint32_t)0x7FFFFFFF)  /* System Time sub-seconds */

/* Bit definition for Ethernet PTP Time Stamp High Update Register */
#define ETH_PTPTSHUR_TSUS    ((uint32_t)0xFFFFFFFF)  /* Time stamp update seconds */

/* Bit definition for Ethernet PTP Time Stamp Low Update Register */
#define ETH_PTPTSLUR_TSUPNS  ((uint32_t)0x80000000)  /* Time stamp update Positive or negative time */
#define ETH_PTPTSLUR_TSUSS   ((uint32_t)0x7FFFFFFF)  /* Time stamp update sub-seconds */

/* Bit definition for Ethernet PTP Time Stamp Addend Register */
#define ETH_PTPTSAR_TSA      ((uint32_t)0xFFFFFFFF)  /* Time stamp addend */

/* Bit definition for Ethernet PTP Target Time High Register */
#define ETH_PTPTTHR_TTSH     ((uint32_t)0xFFFFFFFF)  /* Target time stamp high */

/* Bit definition for Ethernet PTP Target Time Low Register */
#define ETH_PTPTTLR_TTSL     ((uint32_t)0xFFFFFFFF)  /* Target time stamp low */

/******************************************************************************/
/*                 Ethernet DMA Registers bits definition                     */
/******************************************************************************/

/* Bit definition for Ethernet DMA Bus Mode Register */
#define ETH_DMABMR_AAB       ((uint32_t)0x02000000)  /* Address-Aligned beats */
#define ETH_DMABMR_FPM        ((uint32_t)0x01000000)  /* 8xPBL mode */
#define ETH_DMABMR_USP       ((uint32_t)0x00800000)  /* Use separate PBL */
#define ETH_DMABMR_RDP       ((uint32_t)0x007E0000)  /* RxDMA PBL */
  #define ETH_DMABMR_RDP_1Beat    ((uint32_t)0x00020000)  /* maximum number of beats to be transferred in one RxDMA transaction is 1 */
  #define ETH_DMABMR_RDP_2Beat    ((uint32_t)0x00040000)  /* maximum number of beats to be transferred in one RxDMA transaction is 2 */
  #define ETH_DMABMR_RDP_4Beat    ((uint32_t)0x00080000)  /* maximum number of beats to be transferred in one RxDMA transaction is 4 */
  #define ETH_DMABMR_RDP_8Beat    ((uint32_t)0x00100000)  /* maximum number of beats to be transferred in one RxDMA transaction is 8 */
  #define ETH_DMABMR_RDP_16Beat   ((uint32_t)0x00200000)  /* maximum number of beats to be transferred in one RxDMA transaction is 16 */
  #define ETH_DMABMR_RDP_32Beat   ((uint32_t)0x00400000)  /* maximum number of beats to be transferred in one RxDMA transaction is 32 */                
  #define ETH_DMABMR_RDP_8xPBL_8Beat   ((uint32_t)0x01020000)  /* maximum number of beats to be transferred in one RxDMA transaction is 4 */
  #define ETH_DMABMR_RDP_8xPBL_16Beat   ((uint32_t)0x01040000)  /* maximum number of beats to be transferred in one RxDMA transaction is 8 */
  #define ETH_DMABMR_RDP_8xPBL_32Beat  ((uint32_t)0x01080000)  /* maximum number of beats to be transferred in one RxDMA transaction is 16 */
  #define ETH_DMABMR_RDP_8xPBL_64Beat  ((uint32_t)0x01100000)  /* maximum number of beats to be transferred in one RxDMA transaction is 32 */
  #define ETH_DMABMR_RDP_8xPBL_128Beat  ((uint32_t)0x01200000)  /* maximum number of beats to be transferred in one RxDMA transaction is 64 */
  #define ETH_DMABMR_RDP_8xPBL_256Beat ((uint32_t)0x01400000)  /* maximum number of beats to be transferred in one RxDMA transaction is 128 */  
#define ETH_DMABMR_FB        ((uint32_t)0x00010000)  /* Fixed Burst */
#define ETH_DMABMR_RTPR      ((uint32_t)0x0000C000)  /* Rx Tx priority ratio */
  #define ETH_DMABMR_RTPR_1_1     ((uint32_t)0x00000000)  /* Rx Tx priority ratio */
  #define ETH_DMABMR_RTPR_2_1     ((uint32_t)0x00004000)  /* Rx Tx priority ratio */
  #define ETH_DMABMR_RTPR_3_1     ((uint32_t)0x00008000)  /* Rx Tx priority ratio */
  #define ETH_DMABMR_RTPR_4_1     ((uint32_t)0x0000C000)  /* Rx Tx priority ratio */  
#define ETH_DMABMR_PBL    ((uint32_t)0x00003F00)  /* Programmable burst length */
  #define ETH_DMABMR_PBL_1Beat    ((uint32_t)0x00000100)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 1 */
  #define ETH_DMABMR_PBL_2Beat    ((uint32_t)0x00000200)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 2 */
  #define ETH_DMABMR_PBL_4Beat    ((uint32_t)0x00000400)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
  #define ETH_DMABMR_PBL_8Beat    ((uint32_t)0x00000800)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
  #define ETH_DMABMR_PBL_16Beat   ((uint32_t)0x00001000)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
  #define ETH_DMABMR_PBL_32Beat   ((uint32_t)0x00002000)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */                
  #define ETH_DMABMR_PBL_8xPBL_8Beat   ((uint32_t)0x01000100)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 4 */
  #define ETH_DMABMR_PBL_8xPBL_16Beat   ((uint32_t)0x01000200)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 8 */
  #define ETH_DMABMR_PBL_8xPBL_32Beat  ((uint32_t)0x01000400)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 16 */
  #define ETH_DMABMR_PBL_8xPBL_64Beat  ((uint32_t)0x01000800)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 32 */
  #define ETH_DMABMR_PBL_8xPBL_128Beat  ((uint32_t)0x01001000)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 64 */
  #define ETH_DMABMR_PBL_8xPBL_256Beat ((uint32_t)0x01002000)  /* maximum number of beats to be transferred in one TxDMA (or both) transaction is 128 */
#define ETH_DMABMR_DSL       ((uint32_t)0x0000007C)  /* Descriptor Skip Length */
#define ETH_DMABMR_DA        ((uint32_t)0x00000002)  /* DMA arbitration scheme */
#define ETH_DMABMR_SR        ((uint32_t)0x00000001)  /* Software reset */

/* Bit definition for Ethernet DMA Transmit Poll Demand Register */
#define ETH_DMATPDR_TPD      ((uint32_t)0xFFFFFFFF)  /* Transmit poll demand */

/* Bit definition for Ethernet DMA Receive Poll Demand Register */
#define ETH_DMARPDR_RPD      ((uint32_t)0xFFFFFFFF)  /* Receive poll demand  */

/* Bit definition for Ethernet DMA Receive Descriptor List Address Register */
#define ETH_DMARDLAR_SRL     ((uint32_t)0xFFFFFFFF)  /* Start of receive list */

/* Bit definition for Ethernet DMA Transmit Descriptor List Address Register */
#define ETH_DMATDLAR_STL     ((uint32_t)0xFFFFFFFF)  /* Start of transmit list */

/* Bit definition for Ethernet DMA Status Register */
#define ETH_DMASR_TSTS       ((uint32_t)0x20000000)  /* Time-stamp trigger status */
#define ETH_DMASR_PMTS       ((uint32_t)0x10000000)  /* PMT status */
#define ETH_DMASR_MMCS       ((uint32_t)0x08000000)  /* MMC status */
#define ETH_DMASR_EBS        ((uint32_t)0x03800000)  /* Error bits status */
  /* combination with EBS[2:0] for GetFlagStatus function */
  #define ETH_DMASR_EBS_DescAccess      ((uint32_t)0x02000000)  /* Error bits 0-data buffer, 1-desc. access */
  #define ETH_DMASR_EBS_ReadTransf      ((uint32_t)0x01000000)  /* Error bits 0-write trnsf, 1-read transfr */
  #define ETH_DMASR_EBS_DataTransfTx    ((uint32_t)0x00800000)  /* Error bits 0-Rx DMA, 1-Tx DMA */
#define ETH_DMASR_TPS         ((uint32_t)0x00700000)  /* Transmit process state */
  #define ETH_DMASR_TPS_Stopped         ((uint32_t)0x00000000)  /* Stopped - Reset or Stop Tx Command issued  */
  #define ETH_DMASR_TPS_Fetching        ((uint32_t)0x00100000)  /* Running - fetching the Tx descriptor */
  #define ETH_DMASR_TPS_Waiting         ((uint32_t)0x00200000)  /* Running - waiting for status */
  #define ETH_DMASR_TPS_Reading         ((uint32_t)0x00300000)  /* Running - reading the data from host memory */
  #define ETH_DMASR_TPS_Suspended       ((uint32_t)0x00600000)  /* Suspended - Tx Descriptor unavailabe */
  #define ETH_DMASR_TPS_Closing         ((uint32_t)0x00700000)  /* Running - closing Rx descriptor */
#define ETH_DMASR_RPS         ((uint32_t)0x000E0000)  /* Receive process state */
  #define ETH_DMASR_RPS_Stopped         ((uint32_t)0x00000000)  /* Stopped - Reset or Stop Rx Command issued */
  #define ETH_DMASR_RPS_Fetching        ((uint32_t)0x00020000)  /* Running - fetching the Rx descriptor */
  #define ETH_DMASR_RPS_Waiting         ((uint32_t)0x00060000)  /* Running - waiting for packet */
  #define ETH_DMASR_RPS_Suspended       ((uint32_t)0x00080000)  /* Suspended - Rx Descriptor unavailable */
  #define ETH_DMASR_RPS_Closing         ((uint32_t)0x000A0000)  /* Running - closing descriptor */
  #define ETH_DMASR_RPS_Queuing         ((uint32_t)0x000E0000)  /* Running - queuing the recieve frame into host memory */
#define ETH_DMASR_NIS        ((uint32_t)0x00010000)  /* Normal interrupt summary */
#define ETH_DMASR_AIS        ((uint32_t)0x00008000)  /* Abnormal interrupt summary */
#define ETH_DMASR_ERS        ((uint32_t)0x00004000)  /* Early receive status */
#define ETH_DMASR_FBES       ((uint32_t)0x00002000)  /* Fatal bus error status */
#define ETH_DMASR_ETS        ((uint32_t)0x00000400)  /* Early transmit status */
#define ETH_DMASR_RWTS       ((uint32_t)0x00000200)  /* Receive watchdog timeout status */
#define ETH_DMASR_RPSS       ((uint32_t)0x00000100)  /* Receive process stopped status */
#define ETH_DMASR_RBUS       ((uint32_t)0x00000080)  /* Receive buffer unavailable status */
#define ETH_DMASR_RS         ((uint32_t)0x00000040)  /* Receive status */
#define ETH_DMASR_TUS        ((uint32_t)0x00000020)  /* Transmit underflow status */
#define ETH_DMASR_ROS        ((uint32_t)0x00000010)  /* Receive overflow status */
#define ETH_DMASR_TJTS       ((uint32_t)0x00000008)  /* Transmit jabber timeout status */
#define ETH_DMASR_TBUS       ((uint32_t)0x00000004)  /* Transmit buffer unavailable status */
#define ETH_DMASR_TPSS       ((uint32_t)0x00000002)  /* Transmit process stopped status */
#define ETH_DMASR_TS         ((uint32_t)0x00000001)  /* Transmit status */

/* Bit definition for Ethernet DMA Operation Mode Register */
#define ETH_DMAOMR_DTCEFD    ((uint32_t)0x04000000)  /* Disable Dropping of TCP/IP checksum error frames */
#define ETH_DMAOMR_RSF       ((uint32_t)0x02000000)  /* Receive store and forward */
#define ETH_DMAOMR_DFRF      ((uint32_t)0x01000000)  /* Disable flushing of received frames */
#define ETH_DMAOMR_TSF       ((uint32_t)0x00200000)  /* Transmit store and forward */
#define ETH_DMAOMR_FTF       ((uint32_t)0x00100000)  /* Flush transmit FIFO */
#define ETH_DMAOMR_TTC       ((uint32_t)0x0001C000)  /* Transmit threshold control */
  #define ETH_DMAOMR_TTC_64Bytes       ((uint32_t)0x00000000)  /* threshold level of the MTL Transmit FIFO is 64 Bytes */
  #define ETH_DMAOMR_TTC_128Bytes      ((uint32_t)0x00004000)  /* threshold level of the MTL Transmit FIFO is 128 Bytes */
  #define ETH_DMAOMR_TTC_192Bytes      ((uint32_t)0x00008000)  /* threshold level of the MTL Transmit FIFO is 192 Bytes */
  #define ETH_DMAOMR_TTC_256Bytes      ((uint32_t)0x0000C000)  /* threshold level of the MTL Transmit FIFO is 256 Bytes */
  #define ETH_DMAOMR_TTC_40Bytes       ((uint32_t)0x00010000)  /* threshold level of the MTL Transmit FIFO is 40 Bytes */
  #define ETH_DMAOMR_TTC_32Bytes       ((uint32_t)0x00014000)  /* threshold level of the MTL Transmit FIFO is 32 Bytes */
  #define ETH_DMAOMR_TTC_24Bytes       ((uint32_t)0x00018000)  /* threshold level of the MTL Transmit FIFO is 24 Bytes */
  #define ETH_DMAOMR_TTC_16Bytes       ((uint32_t)0x0001C000)  /* threshold level of the MTL Transmit FIFO is 16 Bytes */
#define ETH_DMAOMR_ST        ((uint32_t)0x00002000)  /* Start/stop transmission command */
#define ETH_DMAOMR_FEF       ((uint32_t)0x00000080)  /* Forward error frames */
#define ETH_DMAOMR_FUGF      ((uint32_t)0x00000040)  /* Forward undersized good frames */
#define ETH_DMAOMR_RTC       ((uint32_t)0x00000018)  /* receive threshold control */
  #define ETH_DMAOMR_RTC_64Bytes       ((uint32_t)0x00000000)  /* threshold level of the MTL Receive FIFO is 64 Bytes */
  #define ETH_DMAOMR_RTC_32Bytes       ((uint32_t)0x00000008)  /* threshold level of the MTL Receive FIFO is 32 Bytes */
  #define ETH_DMAOMR_RTC_96Bytes       ((uint32_t)0x00000010)  /* threshold level of the MTL Receive FIFO is 96 Bytes */
  #define ETH_DMAOMR_RTC_128Bytes      ((uint32_t)0x00000018)  /* threshold level of the MTL Receive FIFO is 128 Bytes */
#define ETH_DMAOMR_OSF       ((uint32_t)0x00000004)  /* operate on second frame */
#define ETH_DMAOMR_SR        ((uint32_t)0x00000002)  /* Start/stop receive */

/* Bit definition for Ethernet DMA Interrupt Enable Register */
#define ETH_DMAIER_NISE      ((uint32_t)0x00010000)  /* Normal interrupt summary enable */
#define ETH_DMAIER_AISE      ((uint32_t)0x00008000)  /* Abnormal interrupt summary enable */
#define ETH_DMAIER_ERIE      ((uint32_t)0x00004000)  /* Early receive interrupt enable */
#define ETH_DMAIER_FBEIE     ((uint32_t)0x00002000)  /* Fatal bus error interrupt enable */
#define ETH_DMAIER_ETIE      ((uint32_t)0x00000400)  /* Early transmit interrupt enable */
#define ETH_DMAIER_RWTIE     ((uint32_t)0x00000200)  /* Receive watchdog timeout interrupt enable */
#define ETH_DMAIER_RPSIE     ((uint32_t)0x00000100)  /* Receive process stopped interrupt enable */
#define ETH_DMAIER_RBUIE     ((uint32_t)0x00000080)  /* Receive buffer unavailable interrupt enable */
#define ETH_DMAIER_RIE       ((uint32_t)0x00000040)  /* Receive interrupt enable */
#define ETH_DMAIER_TUIE      ((uint32_t)0x00000020)  /* Transmit Underflow interrupt enable */
#define ETH_DMAIER_ROIE      ((uint32_t)0x00000010)  /* Receive Overflow interrupt enable */
#define ETH_DMAIER_TJTIE     ((uint32_t)0x00000008)  /* Transmit jabber timeout interrupt enable */
#define ETH_DMAIER_TBUIE     ((uint32_t)0x00000004)  /* Transmit buffer unavailable interrupt enable */
#define ETH_DMAIER_TPSIE     ((uint32_t)0x00000002)  /* Transmit process stopped interrupt enable */
#define ETH_DMAIER_TIE       ((uint32_t)0x00000001)  /* Transmit interrupt enable */

/* Bit definition for Ethernet DMA Missed Frame and Buffer Overflow Counter Register */
#define ETH_DMAMFBOCR_OFOC   ((uint32_t)0x10000000)  /* Overflow bit for FIFO overflow counter */
#define ETH_DMAMFBOCR_MFA    ((uint32_t)0x0FFE0000)  /* Number of frames missed by the application */
#define ETH_DMAMFBOCR_OMFC   ((uint32_t)0x00010000)  /* Overflow bit for missed frame counter */
#define ETH_DMAMFBOCR_MFC    ((uint32_t)0x0000FFFF)  /* Number of frames missed by the controller */

/* Bit definition for Ethernet DMA Current Host Transmit Descriptor Register */
#define ETH_DMACHTDR_HTDAP   ((uint32_t)0xFFFFFFFF)  /* Host transmit descriptor address pointer */

/* Bit definition for Ethernet DMA Current Host Receive Descriptor Register */
#define ETH_DMACHRDR_HRDAP   ((uint32_t)0xFFFFFFFF)  /* Host receive descriptor address pointer */

/* Bit definition for Ethernet DMA Current Host Transmit Buffer Address Register */
#define ETH_DMACHTBAR_HTBAP  ((uint32_t)0xFFFFFFFF)  /* Host transmit buffer address pointer */

/* Bit definition for Ethernet DMA Current Host Receive Buffer Address Register */
#define ETH_DMACHRBAR_HRBAP  ((uint32_t)0xFFFFFFFF)  /* Host receive buffer address pointer */
#endif // __SMSCM6XXA01_REGBIT_H__ 
