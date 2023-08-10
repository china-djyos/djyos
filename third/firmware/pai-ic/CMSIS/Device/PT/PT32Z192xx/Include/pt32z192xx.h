/**
  ******************************************************************************
  * @file    pt32z192xx.h
  * @author  应用开发团队
  * @version V1.0.0
  * @date    2020/04/28
  * @brief
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  ******************************************************************************
  * @attention
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  *
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f0xx
  * @{
  */

#ifndef __PT32Z192XX_H
#define __PT32Z192XX_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined (__CC_ARM)
#pragma anon_unions
#endif

/** @addtogroup Library_configuration_section
  * @{
  */

/* Uncomment the line below according to the target PT32F0 device used in your
   application
  */

#if !defined  USE_STDPERIPH_DRIVER
/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will
   be based on direct access to peripherals registers
   */
  /*#define USE_STDPERIPH_DRIVER*/
#endif /* USE_STDPERIPH_DRIVER */

/**
  * @}
  */

/** @addtogroup Configuration_section_for_CMSIS
  * @{
  */

/**
 * @brief PT32F0xx Interrupt Number Definition, according to the selected device
 *        in @ref Library_configuration_section
 */
#define __MPU_PRESENT             0 /*!< PT32F0xx do not provide MPU                   */
#define __NVIC_PRIO_BITS          2 /*!< PT32F0xx uses 2 Bits for the Priority Levels  */
#define __Vendor_SysTickConfig    0 /*!< Set to 1 if different SysTick Config is used  */

/*!< Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,    /*!<  2 Cortex-M3 Non Maskable Interrupt                 */
  HardFault_IRQn                = -13,    /*!<  3 Cortex-M3 Hard Fault Interrupt                   */
  MemoryManagement_IRQn         = -12,    /*!<  4 Cortex-M3 Memory Management Interrupt            */
  BusFault_IRQn                 = -11,    /*!<  5 Cortex-M3 Bus Fault Interrupt                    */
  UsageFault_IRQn               = -10,    /*!<  6 Cortex-M3 Usage Fault Interrupt                  */
  SVCall_IRQn                   = -5,     /*!< 11 Cortex-M3 SV Call Interrupt                      */
  DebugMonitor_IRQn             = -4,     /*!< 12 Cortex-M3 Debug Monitor Interrupt                */
  PendSV_IRQn                   = -2,     /*!< 14 Cortex-M3 Pend SV Interrupt                      */
  SysTick_IRQn                  = -1,     /*!< 15 Cortex-M3 System Tick Interrupt                  */

#if defined (PT32Z192Ex) || defined(PT32Z192Cx)
/******   PT32Z192 Specific Interrupt Numbers *******************************************************/
  WDG_IRQn                          = 0,
  RTC_IRQn                      = 3,       /* RTC Interrupt     */
  FLASH_IRQn                    = 4,       /* FLASH Interrupt   */
  RCC_IRQn                          = 5,
    PA_IRQn                       = 6,
  PB_IRQn                       = 7,
    PC_IRQn                       = 8,
    PD_IRQn                       = 9,
  DMA_IRQn                          = 11,
  ADC_IRQn                          = 18,
  TMR0_IRQn                     = 24,
    TMR1_IRQn                           = 25,
  TMR2_IRQn                         = 28,
  TMR3_IRQn                         = 29,
    TMR4_IRQn                           = 30,
  I2C_IRQn                      = 31,
  SPI0_IRQn                     = 35,
  SPI1_IRQn                     = 36,
  UART0_IRQn                    = 37,      /* UART0 Interrupt    */
  UART1_IRQn                    = 38,      /* UART1 Interrupt    */
  UART2_IRQn                    = 39,      /* UART2 Interrupt    */
  USB11_IRQn                    = 49,
    QSPI_IRQn                     = 51,
    TMR5_IRQn                     = 57,
    AES_IRQn                      = 59       /* AES Interrupt    */
#endif /* PT32Z192 */
} IRQn_Type;

/**
  * @}
  */


#include "core_cm3.h"
#include "system_pt32z192xx.h"
#include <stdint.h>
#include <types.h>         //add by lst for porting djyos

/** @addtogroup Exported_types
  * @{
  */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus, RemapStatus, ProtectStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))


#define wb(addr, value)     (*((uint8_t  volatile *) (addr)) = value)
#define rb(addr)            (*((uint8_t  volatile *) (addr)))
#define whw(addr, value)    (*((uint16_t volatile *) (addr)) = value)
#define rhw(addr)           (*((uint16_t volatile *) (addr)))
#define ww(addr, value)     (*((uint32_t volatile *) (addr)) = value)
#define rw(addr)            (*((uint32_t volatile *) (addr)))

#define ResetBit_BB(Addr, BitNumber) (rw(Addr) &= ~(1UL << BitNumber))
#define SetBit_BB(Addr, BitNumber)   (rw(Addr) |= (1UL << BitNumber))
#define GetBit_BB(Addr, BitNumber)   ((rw(Addr) >> BitNumber) & 1UL)

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;     //C库中有定义 lst dbg

/** @addtogroup Peripheral_registers_structures
  * @{
  */

/**
  * @brief System Control
  */


/**
  * @}
  */

/** @addtogroup Peripheral_memory_map
  * @{
  */
typedef struct
{
  __IO uint32_t  RESERVED0;            /*!< Offset: 0x000 */
  __IO uint32_t  PMU_CTRL;             /*!< Offset: 0x004 PMU Control Register (R/W), this register assigned 1*/
  __IO uint32_t  RESERVED1;            /*!< Offset: 0x008 */
  __IO uint32_t  SYSCLK;                 /*!< Offset: 0x00C System Clock Selection Register  (R/W) */
  __IO uint32_t  RSTINFO;              /*!< Offset: 0x010 Reset Information Register (R/W) */
  __IO uint32_t  RETRIMING;            /*!< Offset: 0x014 Reset Information Register (R/W) */
  __IO uint32_t  RETRIMING_KEY;        /*!< Offset: 0x018 RSTEN Ccontrol Register (R/W) */
  __IO uint32_t  GPIO_WAKEUP_STAT;     /*!< Offset: 0x01C GPIO WAKEUP STATUS Register (R/W) */
  __IO uint32_t  CHIPID;               /*!< Offset: 0x020 CHIP ID information Register (R/W) */
  __IO uint32_t  UID_INFO0;            /*!< Offset: 0x024 UID information Register (R/W) */
  __IO uint32_t  UID_INFO1;            /*!< Offset: 0x028 UID information Register (R/W) */
  __IO uint32_t  UID_INFO2;            /*!< Offset: 0x02C UID information Register (R/W) */
  __IO uint32_t  UID_INFO3;            /*!< Offset: 0x030 UID information Register (R/W) */
} CMSDK_SYSCON_TypeDef;


/**
  * @brief Analog MIX Function
  */

typedef struct
{
  __IO uint32_t IOSC_32K_CON;            /*!< Offset: 0x000 32KHZ IOSC Control Register (R/W) */
    __IO uint32_t IOSC_32K_STAT;         /*!< Offset: 0x004 32KHZ IOSC Status Register (R/W) */
  __IO uint32_t IOSC_24M_CON;          /*!< Offset: 0x008 24MHZ IOSC Control Register (R/W) */
  __IO uint32_t RESERVED0;             /*!< Offset: 0x00C VDD Low Voltage Detect Control Register (R/W) */
  __IO uint32_t PLL_CON;               /*!< Offset: 0x010 LDO Low Voltage Detect Control Register (R/W) */
  __IO uint32_t PLL_CONFIG;            /*!< Offset: 0x014 Crystal Control Register1 (R/W) */
  __IO uint32_t USB_COM;                 /*!< Offset: 0x018 Crystal Control Register2 (R/W) */
  __IO uint32_t USB_TRIM;              /*!< Offset: 0x01C Crystal Control Register2 (R/W) */
  __IO uint32_t USB_DBUG;              /*!< Offset: 0x020 Crystal Control Register2 (R/W) */
  __IO uint32_t FLASH_PMU_CON;           /*!< Offset: 0x024 Frequency Doubling Control Register (R/W) */
  __IO uint32_t FLASH_PMU_STAT;        /*!< Offset: 0x028 Frequency Doubling Control Register (R/W) */
  __IO uint32_t RESERVED[2];           /*!< Offset: 0x02C (R/W) */
  __IO uint32_t EOSC_32K_CON;            /*!< Offset: 0x034 Deglitch Control Register (R/W) */
  __IO uint32_t EOSC_32M_CON;            /*!< Offset: 0x038 GPIO HighSink Control Register (R/W) */
  __IO uint32_t RESERVED1;               /*!< Offset: 0x03C (R/W) */
}  CMSDK_ANAMIX_TypeDef;


/**
  * @brief General Purpose IO
  */

typedef struct
{
 __IO uint32_t  DATA;                /*!< Offset: 0x000 DATA Register (R/W) */
 __IO uint32_t  DATAOUT;             /*!< Offset: 0x004 Data Output Latch Register (R/W) */
 __IO uint32_t  INTTYPESET1;       /*!< Offset: 0x008 Interrupt Type Set Register1  (R/W) */
 __IO uint32_t  INTTYPECLR1;       /*!< Offset: 0x00C Interrupt Type Clear Register1  (R/W) */
 __IO uint32_t  OUTENABLESET;        /*!< Offset: 0x010 Output Enable Set Register  (R/W) */
 __IO uint32_t  OUTENABLECLR;        /*!< Offset: 0x014 Output Enable Clear Register  (R/W) */
 __IO uint32_t  ALTFUNCSET;          /*!< Offset: 0x018 Alternate Function Set Register0  (R/W) */
 __IO uint32_t  ALTFUNCCLR;          /*!< Offset: 0x01C Alternate Function Clear Register0  (R/W) */
 __IO uint32_t  INTENSET;            /*!< Offset: 0x020 Interrupt Enable Set Register  (R/W) */
 __IO uint32_t  INTENCLR;            /*!< Offset: 0x024 Interrupt Enable Clear Register  (R/W) */
 __IO uint32_t  INTTYPESET;          /*!< Offset: 0x028 Interrupt Type Set Register  (R/W) */
 __IO uint32_t  INTTYPECLR;          /*!< Offset: 0x02C Interrupt Type Clear Register  (R/W) */
 __IO uint32_t  INTPOLSET;           /*!< Offset: 0x030 Interrupt Polarity Set Register  (R/W) */
 __IO uint32_t  INTPOLCLR;           /*!< Offset: 0x034 Interrupt Polarity Clear Register  (R/W) */
  union
  {
    __I uint32_t  INTSTATUS;     /*!< Offset: 0x038 Interrupt Status Register (R/ ) */
    __O uint32_t  INTCLEAR;      /*!< Offset: 0x038 Interrupt Clear Register ( /W) */
  };
 __IO uint32_t  PULLUPSET;       /*!< Offset: 0x03C Pull Up Set Register  (R/W) */
 __IO uint32_t  PULLUPCLR;       /*!< Offset: 0x040 Pull Up Clear Register  (R/W) */
 __IO uint32_t  ODRSET;      /*!< Offset: 0x044 Open Drain Set Register  (R/W) */
 __IO uint32_t  ODRCLR;      /*!< Offset: 0x048 Open Drain Clear Register  (R/W) */
 __IO uint32_t  PULLDOWNSET;     /*!< Offset: 0x04C Pull Down Set Register  (R/W) */
 __IO uint32_t  PULLDOWNCLR;     /*!< Offset: 0x050 Pull Down Clear Register  (R/W) */
 __IO uint32_t  DRVS;    /*!< Offset: 0x054 Pin Output Drive Selection Register (R/W) */
 __IO uint32_t  DRVC;    /*!< Offset: 0x058 Pin Output Drive Clear Register  (R/W) */
 __IO uint32_t  SCHMIDTSET;      /*!< Offset: 0x05C Schmidt Set Register (R/W) */
 __IO uint32_t  SCHMIDTCLR;      /*!< Offset: 0x060 Schmidt Clear Register (R/W) */
 __IO uint32_t  RESERVED1[231];
 __IO uint32_t  LB_MASKED[256];      /*!< Offset: 0x400 - 0x7FC Lower byte Masked Access Register (R/W) */
 __IO uint32_t  UB_MASKED[256];      /*!< Offset: 0x800 - 0xBFC Upper byte Masked Access Register (R/W) */

} CMSDK_GPIO_TypeDef;

/**
  * @brief UART
  */
typedef struct
{
  __IO uint32_t BR;                    /*!< Offset: 0x000 Buffer Register       (R/W) */
  __IO uint32_t CR;                    /*!< Offset: 0x004 Control Register      (R/W) */
  __IO uint32_t BRR;                   /*!< Offset: 0x008 Baud Rate Register        (R/W) */
  __IO uint32_t IER;                   /*!< Offset: 0x00C Interrupt Enable Register (R/W) */
  __IO uint32_t SR;                    /*!< Offset: 0x010 Status Register       (R/W) */
  __IO uint32_t GTR;                   /*!< Offset: 0x014 Guard Time Register       (R/W) */
  __IO uint32_t TOR;                   /*!< Offset: 0x018 Time Out Register         (R/W) */
  __IO uint32_t TRR;                   /*!< Offset: 0x01C TX Buffer Reset Register      (WO)  */
  __IO uint32_t RRR;                   /*!< Offset: 0x020 RX Buffer Reset Register      (WO)  */

} CMSDK_UART_TypeDef;

/**
  * @brief FLASH
  */
typedef struct
{
  __IO uint32_t ACR;                   /*!< Offset: 0x000 Flash Command Register (R/W) */
  __IO uint32_t ISR;                     /*!< Offset: 0x004 Flash Interrupt status Register (R/W) */
  __IO uint32_t IER;                     /*!< Offset: 0x008 Flash Interrupt Enable Register  (R/W) */
  __IO uint32_t AR;                    /*!< Offset: 0x00C Flash Address Register (R/W) */
  __IO uint32_t DR0;                   /*!< Offset: 0x010 Flash Programming Data Register0  (R/W) */
  __IO uint32_t DR1;                   /*!< Offset: 0x014 Flash Programming Data Register0  (R/W) */
  __IO uint32_t RESERVED1[4];
  __IO uint32_t DIV;                     /*!< Offset: 0x028 Flash Erase Clock Division Register  (R/W) */

} CMSDK_FLASH_TypeDef;


/**
  * @brief  Analog to Digital Converter
  */
typedef struct
{
  __IO uint32_t CR0;                     /*!< Offset: 0x000 ADC Control0 register       */
  __IO uint32_t CR1;                       /*!< Offset: 0x004 ADC Control1 register       */
  __IO uint32_t SR;                      /*!< Offset: 0x008 ADC Status Register         */
  __IO uint32_t DR;                      /*!< Offset: 0x00C ADC Data register           */
  __IO uint32_t IER;                       /*!< Offset: 0x010 ADC Interrupt register      */
  __IO uint32_t CHER;                      /*!< Offset: 0x014 ADC Channel Enabel register */
} CMSDK_ADC_TypeDef;

/**
  * @brief Inter-integrated Circuit Interface
  */

typedef struct
{
  __IO uint32_t CSR;       /*!< I2C Control Set register ,         Address offset: 0x00 */
  __IO  uint32_t SR;        /*!< I2C status register,               Address offset: 0x04 */
  __IO uint32_t DR;        /*!< I2C data register,                 Address offset: 0x08 */
  __IO uint32_t OAR;       /*!< I2C Own address register,          Address offset: 0x0C */
    __IO uint32_t RESERVED[2];
  __IO uint32_t CCR;       /*!< I2C Control Reset register 2,      Address offset: 0x18 */

} CMSDK_I2C_TypeDef;

/**
  * @brief Serial Peripheral Interface
  */

typedef struct
{
  __IO uint32_t CR0;       /*!< SPI Control register 1                               Address offset: 0x00 */
  __IO uint32_t CR1;       /*!< SPI Control register 2,                              Address offset: 0x04 */
  __IO uint32_t DR;        /*!< SPI data register,                                   Address offset: 0x08 */
  __IO uint32_t SR;        /*!< SPI Status register,                                 Address offset: 0x0C */
  __IO uint32_t CPSR;      /*!< SPI Clock prescaler register                         Address offset: 0x10 */
  __IO uint32_t IE;        /*!< SPI Interrupt ENABLE register                        Address offset: 0x14 */
  __IO uint32_t RIS;       /*!< SPI Interrupt Raw Int Status register                Address offset: 0x18 */
  __IO uint32_t EIS;       /*!< SPI Interrupt Int Status register                    Address offset: 0x1C */
  __IO uint32_t ICR;       /*!< SPI Interrupt Clear Register register                Address offset: 0x20 */
    __IO uint32_t DMACR;     /*!< SPI DMA control register                             Address offset: 0x24 */
  __IO uint32_t CSCR;      /*!< SPI Chip-Select Control register                     Address offset: 0x28 */

} CMSDK_SPI_TypeDef;


/**
  * @brief 7SEG LED Display Interface
  */

typedef struct
{
  __IO uint32_t CR;        /*!< LED Control register 1                               Address offset: 0x00 */
  __IO uint32_t CPSR;      /*!< LED Clock prescaler register                         Address offset: 0x04 */
  __IO uint32_t TR;        /*!< LED Display Timer Configuration  register            Address offset: 0x08 */
  __IO uint32_t DR0;       /*!< LED data0 register                                   Address offset: 0x0C */
  __IO uint32_t DR1;       /*!< LED data1 register                                   Address offset: 0x10 */
  __IO uint32_t DR2;       /*!< LED data2 register                                   Address offset: 0x14 */
  __IO uint32_t DR3;       /*!< LED data3 register                                   Address offset: 0x18 */
  __IO uint32_t EIS;       /*!< LED Interrupt Int Status register                    Address offset: 0x1C */

} CMSDK_LED_TypeDef;



/**
  * @brief PWM
  */
typedef struct
{
  __IO uint32_t ISR;                         /*!< offset: 0x000 Interrupt status Register   (R/W)         */
  __IO uint32_t TCR;                         /*!< offset: 0x004 Control Register            (R/W)         */
  __IO uint32_t INT_RPT;                 /*!< offset: 0x008 Interrupt Repeat Timers     (R/W)         */
  __IO uint32_t INT_RPTC;                /*!< offset: 0x00C Interrupt Repeat Timers CNT (R/W)         */
  __IO uint32_t PR;                      /*!< offset: 0x010 Prescale Register           (R/W)         */
  __IO uint32_t PC;                          /*!< offset: 0x014 Prescale Counter Register   (R/W)         */
  __IO uint32_t TC;                          /*!< offset: 0x018 Timer Counter Register      (R/W)         */
  __IO uint32_t MCR;                         /*!< offset: 0x01C Match Control Register      (R/W)         */
  __IO uint32_t MR0;                         /*!< offset: 0x020 Match Value Register0       (R/W)         */
  __IO uint32_t MR1;                         /*!< offset: 0x024 Match Value Register1       (R/W)         */
  __IO uint32_t MR2;                         /*!< offset: 0x028 Match Value Register2       (R/W)         */
  __IO uint32_t MR3;                       /*!< offset: 0x02C Match Value Register3       (R/W)         */
  __IO uint32_t MR4;                         /*!< offset: 0x030 Match Value Register4       (R/W)         */
  __IO uint32_t CCR;                         /*!< offset: 0x034 Capture Control Register    (R/W)         */
  __IO uint32_t CR1;                         /*!< offset: 0x038 Capture Value Register1     (R/W)         */
  __IO uint32_t CR2;                         /*!< offset: 0x03C Capture Value Register2     (R/W)         */
  __IO uint32_t CR3;                         /*!< offset: 0x040 Capture Value Register3     (R/W)         */
  __IO uint32_t CR4;                         /*!< offset: 0x044 Capture Value Register4     (R/W)         */
  __IO uint32_t CMR;                         /*!< offset: 0x048 Compare Output Register     (R/W)         */
  __IO uint32_t DT;                        /*!< offset: 0x04C Death Time Register         (R/W)         */
} CMSDK_PWM_TypeDef;
/**
  * @brief TIM0
  */
typedef struct
{
  __IO uint32_t ISR;                         /*!< offset: 0x000 Interrupt status Register   (R/W)         */
  __IO uint32_t TCR;                         /*!< offset: 0x004 Control Register            (R/W)         */
  __IO uint32_t TC;                      /*!< offset: 0x008 Prescale Register           (R/W)         */
  __IO uint32_t PR;                          /*!< offset: 0x00C Prescale Counter Register   (R/W)         */
  __IO uint32_t PC;                          /*!< offset: 0x010 Timer Counter Register      (R/W)         */
  __IO uint32_t MCR;                         /*!< offset: 0x014 Match Control Register      (R/W)         */
  __IO uint32_t MR0;                         /*!< offset: 0x018 Match Value Register0       (R/W)         */
    __IO uint32_t MR1;                       /*!< offset: 0x01C Match Value Register0       (R/W)         */
  __IO uint32_t MR2;                         /*!< offset: 0x020 Match Value Register0       (R/W)         */
  __IO uint32_t MR3;                         /*!< offset: 0x024 Match Value Register0       (R/W)         */
  __IO uint32_t CCR;                   /*!< offset: 0x028 Capture Control Register    (R/W)         */
    __IO uint32_t CR0;                   /*!< offset: 0x02C Capture Value Register0     (R/W)         */
    __IO uint32_t CR1;                   /*!< offset: 0x030 Capture Value Register1     (R/W)         */
    __IO uint32_t CR2;                   /*!< offset: 0x034 Capture Value Register2     (R/W)         */
    __IO uint32_t CSTA;                  /*!< offset: 0x038 Capture status register     (R/W)         */
} CMSDK_TIM1_TypeDef;
/**
  * @brief TIM
  */
typedef struct
{
  __IO uint32_t ISR;                         /*!< offset: 0x000 Interrupt status Register   (R/W)         */
  __IO uint32_t TCR;                         /*!< offset: 0x004 Control Register            (R/W)         */
  __IO uint32_t TC;                          /*!< offset: 0x008 Timer Counter Register      (R/W)         */
    __IO uint32_t PR;                        /*!< offset: 0x00C Prescale Register           (R/W)         */
  __IO uint32_t PC;                          /*!< offset: 0x010 Prescale Counter Register   (R/W)         */
  __IO uint32_t MCR;                         /*!< offset: 0x014 Match Control Register      (R/W)         */
  __IO uint32_t MR0;                         /*!< offset: 0x018 Match Value Register0       (R/W)         */
} CMSDK_TIM_TypeDef;




/**
  * @brief Independent WATCHDOG
  */
typedef struct
{
  __IO uint32_t RLR;   /*!< IWDG Reload register,    Address offset: 0x00 */
  __I  uint32_t COUNT; /*!< IWDG COUNT register,     Address offset: 0x04 */
  __IO uint32_t CR;    /*!< IWDG Control register,   Address offset: 0x08 */
  __IO uint32_t CLR;   /*!< IWDG Clear interrupt register,    Address offset: 0x0C */
  __IO uint32_t RIS;   /*!< IWDG Original interrupt flag register,    Address offset: 0x10 */
  __IO uint32_t EIS;   /*!< IWDG Masking interruption flag register,    Address offset: 0x14 */
    __IO uint32_t RESERVED0[249];
  __IO uint32_t KR;    /*!< IWDG Lock register,    Address offset: 0x400 */

} CMSDK_WDG_TypeDef;


/**
  * @brief Independent QSPI
  */
typedef struct
{
  __IO uint32_t CTRLR0;      /*!< QSPI Control register0,                                  Address offset: 0x00 */
  __IO uint32_t CTRLR1;      /*!< QSPI Control register1,                          Address offset: 0x04 */
  __IO uint32_t ENR;         /*!< QSPI Enable  register,                           Address offset: 0x08 */
  __IO uint32_t MWCR;        /*!< QSPI Microwire Control register,                 Address offset: 0x0C */
  __IO uint32_t SER;         /*!< QSPI Slave Select register,                      Address offset: 0x10 */
  __IO uint32_t BAUDR;       /*!< QSPI BaudRate register,                          Address offset: 0x14 */
    __IO uint32_t TXFTLR;      /*!< QSPI TX FIFO Threshold register,                 Address offset: 0x18 */
  __IO uint32_t RXFTLR;      /*!< QSPI RX FIFO Threshold register,                 Address offset: 0x1C */
  __IO uint32_t TXFLR;       /*!< QSPI TX FIFO Status register,                    Address offset: 0x20 */
  __IO uint32_t RXFLR;       /*!< QSPI RX FIFO Status register,                    Address offset: 0x24 */
  __IO uint32_t SR;          /*!< QSPI Status register,                            Address offset: 0x28 */
  __IO uint32_t IMR;         /*!< QSPI Interrupt Enable register,                  Address offset: 0x2C */
  __IO uint32_t ISR;         /*!< QSPI Interrupt Status register,                  Address offset: 0x30 */
  __IO uint32_t RISR;        /*!< QSPI Primitive Interrupt Status register,        Address offset: 0x34 */
    __I  uint32_t TXOICR;      /*!< QSPI TX FIFO Overflow Interrupt Clear register,  Address offset: 0x38 */
  __I  uint32_t RXOICR;      /*!< QSPI RX FIFO Overflow Interrupt Clear register,  Address offset: 0x3C */
  __I  uint32_t RXUICR;      /*!< QSPI RX FIFO Underflow Interrupt Clear register, Address offset: 0x40 */
  __I  uint32_t MSTICR;      /*!< QSPI Multi Master Interrupt Clear register,      Address offset: 0x44 */
  __I  uint32_t ICR;         /*!< QSPI Interrupt Clear register,                   Address offset: 0x48 */
  __IO uint32_t DMACR;       /*!< QSPI DMA Control register,                       Address offset: 0x4C */
  __IO uint32_t DMATDLR;     /*!< QSPI DMA Transmit Trigger Level register,        Address offset: 0x50 */
  __IO uint32_t DMARDLR;     /*!< QSPI DMA Receive Trigger Level register,         Address offset: 0x54 */
    __IO uint32_t IDR;         /*!< QSPI ID register,                                Address offset: 0x58 */
  __IO uint32_t VERSION_ID;  /*!< QSPI VERSION register,                           Address offset: 0x5C */
    __IO uint32_t DR;          /*!< QSPI Data register,                              Address offset: 0x60 */
    __IO uint32_t RESERVED[36];
  __IO uint32_t SPI_CTRLR;  /*!< QSPI SPI Mode Control register,                   Address offset: 0xF4 */
} CMSDK_QSPI_TypeDef;


/**
  * @brief Independent DSP
  */
typedef struct
{
  __IO uint32_t  DSP_STAUS;         /*!<                                Address offset: 0x00 */
  __IO uint32_t  ABSCALC;           /*!<                          Address offset: 0x04 */
  __IO uint32_t  SQRTCALC;          /*!<                          Address offset: 0x08 */
  __IO uint32_t  ATAN_X;            /*!<                          Address offset: 0x0C */
  __IO uint32_t  ATAN_Y;            /*!<                          Address offset: 0x10 */
  __IO uint32_t  ATAN_RESULT;       /*!<                          Address offset: 0x14 */
    __IO uint32_t  GET_BIT1;          /*!<                          Address offset: 0x18 */
  __IO uint32_t  RESERVED0[1];      /*!<                          Address offset: 0x1C */
  __IO uint32_t  BATCH_OFFSET;      /*!<                          Address offset: 0x20 */
  __IO uint32_t  SLWR;              /*!<                          Address offset: 0x24 */
  __IO uint32_t  SAWR;              /*!<                          Address offset: 0x28 */
  __IO uint32_t  DEF_LENERROR;      /*!<                          Address offset: 0x2C */
  __IO uint32_t  DEF_DIRERROR;      /*!<                          Address offset: 0x30 */
  __IO uint32_t  BATCH_LEN;         /*!<                          Address offset: 0x34 */
    __IO uint32_t  BATCH_A1;          /*!<                          Address offset: 0x38 */
  __IO uint32_t  BATCH_A2;          /*!<                          Address offset: 0x3C */
  __IO uint32_t  PAIRSIMILARITY;    /*!<                                    Address offset: 0x40 */
  __IO uint32_t  RESERVED1[3];      /*!<                                Address offset: 0x44 */
  __IO uint32_t  ROTATE_ANGLE;      /*!<                          Address offset: 0x48 */
  __IO uint32_t  ROTATE_XY0;        /*!<                          Address offset: 0x4C */
  __IO uint32_t  ROTATE_XY1;        /*!<                          Address offset: 0x50 */
  __IO uint32_t  ROTATE_XY;         /*!<                          Address offset: 0x54 */
    __IO uint32_t  PIXEL_CFG;         /*!<                          Address offset: 0x58 */
  __IO uint32_t  PIXEL1_DATA0;      /*!<                          Address offset: 0x5C */
    __IO uint32_t  PIXEL1_DATA1;      /*!<                          Address offset: 0x60 */
    __IO uint32_t  PIXEL2_DATA0;      /*!<                          Address offset: 0x60 */
  __IO uint32_t  PIXEL2_DATA1;      /*!< QSPI SPI Control register,                       Address offset: 0xF4 */
  __IO uint32_t  PIXEL_CMP_RESULT;    /*!< Offset: 0x074    */
  __IO uint32_t  Reserve1[2];        /*!< Offset: 0x078~0x7F    */
  __IO uint32_t  CAPBIT_ADDR_CFG;     /*!< Offset: 0x080    */
  __IO uint32_t  CAPBIT_CFG;          /*!< Offset: 0x084    */
  __IO uint32_t  CAPBIT_READ_RESULT;  /*!< Offset: 0x088    */
  __IO uint32_t  Reserve2[1];        /*!< Offset: 0x08C    */
  __IO uint32_t  COMP_SADDR;          /*!< Offset: 0x090    */
  __IO uint32_t  COMP_EADDR;          /*!< Offset: 0x094    */
  __IO uint32_t  COMP_MIN;            /*!< Offset: 0x098    */
  __IO uint32_t  COMP_MIN_INDEX;      /*!< Offset: 0x09C    */
  __IO uint32_t  COMP_MAX;            /*!< Offset: 0x0A0    */
  __IO uint32_t  COMP_MAX_INDEX;      /*!< Offset: 0x0A4    */
  __IO uint32_t  Reserve3[2];     /*!< Offset: 0x0A8~0x0AC */
  __IO uint32_t  CONV_A1_A4;      /*!< Offset: 0x0B0    */
  __IO uint32_t  CONV_A5_A8;      /*!< Offset: 0x0B4    */
  __IO uint32_t  CONV_A9_A12;     /*!< Offset: 0x0B8    */
  __IO uint32_t  CONV_A13;        /*!< Offset: 0x0BC    */
  __IO uint32_t  CONV_B1_B4;      /*!< Offset: 0x0C0    */
  __IO uint32_t  CONV_B5_B8;      /*!< Offset: 0x0C4    */
  __IO uint32_t  CONV_B9_B12;     /*!< Offset: 0x0C8    */
  __IO uint32_t  CONV_B13;        /*!< Offset: 0x0CC    */
  __IO uint32_t  CONV_C1_C4;      /*!< Offset: 0x0D0    */
  __IO uint32_t  CONV_C5_C8;      /*!< Offset: 0x0D4    */
  __IO uint32_t  CONV_C9_C12;     /*!< Offset: 0x0D8    */
  __IO uint32_t  CONV_C13;        /*!< Offset: 0x0DC    */
  __IO uint32_t  CONV_D1_D4;      /*!< Offset: 0x0E0    */
  __IO uint32_t  CONV_D5_D8;      /*!< Offset: 0x0E4    */
  __IO uint32_t  CONV_D9_D12;     /*!< Offset: 0x0E8    */
  __IO uint32_t  CONV_D13;        /*!< Offset: 0x0EC    */
  __IO uint32_t  CONV_E1_E4;      /*!< Offset: 0x0F0    */
  __IO uint32_t  CONV_E5_E8;      /*!< Offset: 0x0F4    */
  __IO uint32_t  CONV_E9_E12;     /*!< Offset: 0x0F8    */
  __IO uint32_t  CONV_E13;        /*!< Offset: 0x0FC    */
  __IO uint32_t  CONV_F1_F4;      /*!< Offset: 0x100    */
  __IO uint32_t  CONV_F5_F8;      /*!< Offset: 0x104    */
  __IO uint32_t  CONV_F9_F12;     /*!< Offset: 0x108    */
  __IO uint32_t  CONV_F13;        /*!< Offset: 0x10C    */
  __IO uint32_t  CONV_G1_G4;      /*!< Offset: 0x110    */
  __IO uint32_t  CONV_G5_G8;      /*!< Offset: 0x114    */
  __IO uint32_t  CONV_G9_G12;     /*!< Offset: 0x118    */
  __IO uint32_t  CONV_G13;        /*!< Offset: 0x11C    */
  __IO uint32_t  CONV_H1_H4;      /*!< Offset: 0x120    */
  __IO uint32_t  CONV_H5_H8;      /*!< Offset: 0x124    */
  __IO uint32_t  CONV_H9_H12;     /*!< Offset: 0x128    */
  __IO uint32_t  CONV_H13;        /*!< Offset: 0x12C    */
  __IO uint32_t  CONV_I1_I4;      /*!< Offset: 0x130    */
  __IO uint32_t  CONV_I5_I8;      /*!< Offset: 0x134    */
  __IO uint32_t  CONV_I9_I12;     /*!< Offset: 0x138    */
  __IO uint32_t  CONV_I13;        /*!< Offset: 0x13C    */
  __IO uint32_t  CONV_J1_J4;      /*!< Offset: 0x140    */
  __IO uint32_t  CONV_J5_J8;      /*!< Offset: 0x144    */
  __IO uint32_t  CONV_J9_J12;     /*!< Offset: 0x148    */
  __IO uint32_t  CONV_J13;        /*!< Offset: 0x14C    */
  __IO uint32_t  CONV_K1_K4;      /*!< Offset: 0x150    */
  __IO uint32_t  CONV_K5_K8;      /*!< Offset: 0x154    */
  __IO uint32_t  CONV_K9_K12;     /*!< Offset: 0x158    */
  __IO uint32_t  CONV_K13;        /*!< Offset: 0x15C    */
  __IO uint32_t  CONV_L1_L4;      /*!< Offset: 0x160    */
  __IO uint32_t  CONV_L5_L8;      /*!< Offset: 0x164    */
  __IO uint32_t  CONV_L9_L12;     /*!< Offset: 0x168    */
  __IO uint32_t  CONV_L13;        /*!< Offset: 0x16C    */
  __IO uint32_t  CONV_M1_M4;      /*!< Offset: 0x170    */
  __IO uint32_t  CONV_M5_M8;      /*!< Offset: 0x174    */
  __IO uint32_t  CONV_M9_M12;     /*!< Offset: 0x178    */
  __IO uint32_t  CONV_M13;        /*!< Offset: 0x17C    */
  __IO uint32_t  CONV_CTL;        /*!< Offset: 0x180    */
  __IO uint32_t  CONV_CORE_SEL;   /*!< Offset: 0x184    */
  __IO uint32_t  CONV_OUT;        /*!< Offset: 0x188    */
  __IO uint32_t  Reserve4[1];     /*!< Offset: 0x18C */
  __IO uint32_t  PIXEL_CMP_ADDR1;      /*!< Offset: 0x190    */
  __IO uint32_t  PIXEL_CMP_ADDR2;      /*!< Offset: 0x194    */
  __IO uint32_t  PIXEL_CMP_HW;         /*!< Offset: 0x198    */
  __IO uint32_t  PIXEL_CMP_XY;         /*!< Offset: 0x19C    */
  __IO uint32_t  PIXEL_CMP_ANGLE;      /*!< Offset: 0x1A0    */
  __IO uint32_t  PIXEL_CMP_CTL;        /*!< Offset: 0x1A4    */
  __IO uint32_t  PIXEL_RESULT;      /*!< Offset: 0x1A8    */
  __IO uint32_t  PIXEL_ZERO_COUNTER0;        /*!< Offset: 0x1AC    */
  __IO uint32_t  PIXEL_RESULT_EVEN;        /*!< Offset: 0x1A4    */
  __IO uint32_t  PIXEL_ZERO_COUNTER0_EVEN;      /*!< Offset: 0x1A8    */
  __IO uint32_t  PIXEL_ZERO_COUNTER1;        /*!< Offset: 0x1AC    */


} CMSDK_DSP_TypeDef;




/**
 * @brief Peripheral Direct Memory Access Channel
 */
typedef struct
{
  __IO uint32_t SADR;                        /*!< offset: 0x000 DMA CHx Data Source Address Register              */
  __IO uint32_t DADR;                        /*!< offset: 0x004 DMA CHx Data Destination Address Register         */
  __IO uint32_t BLKCNT;                  /*!< offset: 0x008 DMA CHx Data Block Counter Register               */
  __IO uint32_t CTL;                         /*!< offset: 0x00C DMA CHx Control Register                          */
  __IO uint32_t CSADR;                   /*!< offset: 0x010 DMA CHx Current Data Source Address Register      */
  __IO uint32_t CDADR;                 /*!< offset: 0x014 DMA CHx Current Data Destination Address Register */
  __IO uint32_t TCNT;                        /*!< offset: 0x018 DMA CHx Residue Cnt Register                      */
  __IO uint32_t Reserve0[1];           /*!< offset: 0x01C Reserve                                           */
} CMSDK_DMACH_TypeDef;


/**
  * @brief Peripheral Direct Memory Access Global
  */
typedef struct
{
  CMSDK_DMACH_TypeDef   DMACH0;                  /*!< offset: 0x000 DMA channel 0 registers                */
  CMSDK_DMACH_TypeDef   DMACH1;                  /*!< offset: 0x020 DMA channel 1 registers                */
  CMSDK_DMACH_TypeDef   DMACH2;                  /*!< offset: 0x040 DMA channel 2 registers                */
  CMSDK_DMACH_TypeDef   DMACH3;                  /*!< offset: 0x060 DMA channel 3 registers                */
  __IO uint32_t IER;                                 /*!< offset: 0x080 DMA Interrupt Enable Register                     */
  __IO uint32_t CLR;                                 /*!< offset: 0x084 DMA Flag Clear Register                           */
  __IO uint32_t STATUS;                            /*!< offset: 0x088 DMA Status Register                               */
  __IO uint32_t CHCFG;                             /*!< offset: 0x08C DMA Relevance Peripheral Register                 */
} CMSDK_DMA_TypeDef;



/**
  * @brief SYSTEM WATCHDOG
  */
typedef struct
{
  __I uint32_t DR;                            /*!< Current rtc count value register,   Address offset: 0x00 */
    __IO uint32_t MR;                             /*!< Compare matching value register,    Address offset: 0x04 */
    __IO uint32_t LR;                             /*!< Load value register,                Address offset: 0x08 */
    __IO uint32_t CR;                             /*!< RTC Control register,               Address offset: 0x0C */
    __IO uint32_t IR;                             /*!< RTC Interrupt register,             Address offset: 0x10 */
    __IO uint32_t RISR;                           /*!< RTC Compare match status register,           Address offset: 0x14 */
    __IO uint32_t MISR;                           /*!< RTC Interrupt status register,               Address offset: 0x18 */
    __IO uint32_t ICR;                            /*!< RTC Interrupt clear register,                Address offset: 0x1C */
} CMSDK_RTC_TypeDef;


/*Peripheral and SRAM base address */
#define CMSDK_SRAM_BASE             (0x20000000)     /*!< (SRAM      ) Base Address */
#define CMSDK_FLASH_BASE            (0x00000000)

#define CMSDK_APB_BASE              (0x40000000)
#define CMSDK_AHB_BASE              (0x40010000)


/* AHB peripherals */
#define CMSDK_PA_BASE               (CMSDK_AHB_BASE + 0x00000000)
#define CMSDK_PB_BASE               (CMSDK_AHB_BASE + 0x00001000)
#define CMSDK_PC_BASE               (CMSDK_AHB_BASE + 0x00002000)
#define CMSDK_PD_BASE               (CMSDK_AHB_BASE + 0x00005000)
#define CMSDK_QSPI_BASE             (CMSDK_AHB_BASE + 0x00003000)
#define CMSDK_SYSCTRL_BASE          (CMSDK_AHB_BASE + 0x00011000)
/* APB peripherals */
#define CMSDK_FLASH0_CTRL_BASE      (CMSDK_APB_BASE + 0x0000F800)
#define CMSDK_TIMER2_BASE           (CMSDK_APB_BASE + 0x00000000)
#define CMSDK_TIMER3_BASE           (CMSDK_APB_BASE + 0x00000400)
#define CMSDK_TIMER5_BASE           (CMSDK_APB_BASE + 0x00000C00)
#define CMSDK_ANA_MIX_BASE          (CMSDK_APB_BASE + 0x00005800)
#define CMSDK_TIMER4_BASE           (CMSDK_APB_BASE + 0x00000800)
#define CMSDK_IWATCHDOG_BASE        (CMSDK_APB_BASE + 0x00002C00)
#define CMSDK_SPI1_BASE             (CMSDK_APB_BASE + 0x00003C00)
#define CMSDK_UART0_BASE            (CMSDK_APB_BASE + 0x00004400)
#define CMSDK_UART2_BASE            (CMSDK_APB_BASE + 0x00004C00)
#define CMSDK_I2C0_BASE             (CMSDK_APB_BASE + 0x00005400)
#define CMSDK_ADC_BASE              (CMSDK_APB_BASE + 0x00006000)
#define CMSDK_PWM_BASE              (CMSDK_APB_BASE + 0x00001000)
#define CMSDK_TIMER1_BASE           (CMSDK_APB_BASE + 0x00001400)
#define CMSDK_SPI0_BASE             (CMSDK_APB_BASE + 0x00003800)
#define CMSDK_UART1_BASE            (CMSDK_APB_BASE + 0x00004800)
#define CMSDK_DSP_BASE              (CMSDK_APB_BASE + 0x00001800)
#define CMSDK_DMA_BASE              (CMSDK_APB_BASE + 0x0000F000)
#define CMSDK_AES_BASE              (CMSDK_APB_BASE + 0x0000E800)
#define CMSDK_RTC_BASE              (CMSDK_APB_BASE + 0x00002800)
/**
  * @}
  */

/** @addtogroup Peripheral_declaration
  * @{
  */

#define CMSDK_SYSCON            ((CMSDK_SYSCON_TypeDef   *) CMSDK_SYSCTRL_BASE )
#define CMSDK_ANAMIX            ((CMSDK_ANAMIX_TypeDef   *) CMSDK_ANA_MIX_BASE )
#define CMSDK_PA                ((CMSDK_GPIO_TypeDef     *) CMSDK_PA_BASE )
#define CMSDK_PB                ((CMSDK_GPIO_TypeDef     *) CMSDK_PB_BASE )
#define CMSDK_PC                ((CMSDK_GPIO_TypeDef     *) CMSDK_PC_BASE )
#define CMSDK_PD                ((CMSDK_GPIO_TypeDef     *) CMSDK_PD_BASE )
#define UART0                   ((CMSDK_UART_TypeDef     *) CMSDK_UART0_BASE )
#define UART1                   ((CMSDK_UART_TypeDef     *) CMSDK_UART1_BASE )
#define UART2                   ((CMSDK_UART_TypeDef     *) CMSDK_UART2_BASE )
#define CMSDK_FLASH             ((CMSDK_FLASH_TypeDef    *) CMSDK_FLASH0_CTRL_BASE )
#define ADC                     ((CMSDK_ADC_TypeDef      *) CMSDK_ADC_BASE )
#define I2C0                    ((CMSDK_I2C_TypeDef      *) CMSDK_I2C0_BASE )
#define SPI0                    ((CMSDK_SPI_TypeDef      *) CMSDK_SPI0_BASE )
#define SPI1                    ((CMSDK_SPI_TypeDef      *) CMSDK_SPI1_BASE )
#define PWM0                    ((CMSDK_PWM_TypeDef      *) CMSDK_PWM_BASE )
#define TIM1                    ((CMSDK_TIM1_TypeDef      *) CMSDK_TIMER1_BASE )
#define TIM2                    ((CMSDK_TIM_TypeDef      *) CMSDK_TIMER2_BASE )
#define TIM3                    ((CMSDK_TIM_TypeDef      *) CMSDK_TIMER3_BASE )
#define TIM4                    ((CMSDK_TIM_TypeDef      *) CMSDK_TIMER4_BASE )
#define TIM5                    ((CMSDK_TIM_TypeDef      *) CMSDK_TIMER5_BASE )
#define IWDG                    ((CMSDK_WDG_TypeDef      *) CMSDK_IWATCHDOG_BASE )
#define QSPI                    ((CMSDK_QSPI_TypeDef     *) CMSDK_QSPI_BASE )
#define DSP                     ((CMSDK_DSP_TypeDef      *) CMSDK_DSP_BASE  )
#define DMA                     ((CMSDK_DMA_TypeDef      *) CMSDK_DMA_BASE  )
#define AES                     ((CMSDK_AES_TypeDef      *) CMSDK_AES_BASE  )
#define RTC                     ((CMSDK_RTC_TypeDef      *) CMSDK_RTC_BASE  )
/**
  * @}
  */

/** @addtogroup Exported_constants
  * @{
  */

  /** @addtogroup Peripheral_Registers_Bits_Definition
  * @{
  */

/******************************************************************************/
/*                         Peripheral Registers Bits Definition               */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                      System Control (SYSCON)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for SYSCON_REMAP register  ******************/
#define SYSCON_REMAP_BOOT                        ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_RESETOP register  ******************/
#define SYSCON_RESETOP_LOCKUPRST                 ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_SYSCLK register  ******************/
#define SYSCON_SYSCLK_SYSCLKSEL                  ((uint32_t)0x00000003)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_24MIOSC          ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_EXTCLK           ((uint32_t)0x00000001)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_DCLK             ((uint32_t)0x00000002)        /*!<  */
#define SYSCON_SYSCLK_SYSCLKSEL_32KIOSC          ((uint32_t)0x00000003)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL                   ((uint32_t)0x0000000C)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL_24MIOSC           ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL_EXTCLK            ((uint32_t)0x00000004)        /*!<  */
#define SYSCON_SYSCLK_FDCLKSEL_32KIOSC           ((uint32_t)0x00000008)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV                    ((uint32_t)0x00000070)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_1DIV               ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_2DIV               ((uint32_t)0x00000010)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_4DIV               ((uint32_t)0x00000020)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_8DIV               ((uint32_t)0x00000030)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_16DIV              ((uint32_t)0x00000040)        /*!<  */
#define SYSCON_SYSCLK_HCLKDIV_32DIV              ((uint32_t)0x00000050)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV                    ((uint32_t)0x00000700)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_1DIV               ((uint32_t)0x00000000)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_2DIV               ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_4DIV               ((uint32_t)0x00000200)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_8DIV               ((uint32_t)0x00000300)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_16DIV              ((uint32_t)0x00000400)        /*!<  */
#define SYSCON_SYSCLK_PCLKDIV_32DIV              ((uint32_t)0x00000500)        /*!<  */
#define SYSCON_SYSCLK_EOSFAIL_DETECTEN           ((uint32_t)0x00001000)        /*!<  */
#define SYSCON_SYSCLK_EOSFAIL_STATUS             ((uint32_t)0x00002000)        /*!<  */
#define SYSCON_SYSCLK_FDFAIL_DETECTEN            ((uint32_t)0x00004000)        /*!<  */
#define SYSCON_SYSCLK_FDFAIL_STATUS              ((uint32_t)0x00008000)        /*!<  */
#define SYSCON_SYSCLK_WAKE_DELAY                 ((uint32_t)0x00030000)        /*!<  */
#define SYSCON_SYSCLK_RECOVER_SEL                ((uint32_t)0x00100000)        /*!<  */

/********************  Bits definition for SYSCON_RSTINFO register  ******************/
#define SYSCON_RSTINFO_SYSSOFTRST                ((uint32_t)0x00000001)        /*!<  */
#define SYSCON_RSTINFO_IWDGRST                   ((uint32_t)0x00000002)        /*!<  */
#define SYSCON_RSTINFO_LOCKUPRST                 ((uint32_t)0x00000004)        /*!<  */
#define SYSCON_RSTINFO_WWDGRST                   ((uint32_t)0x00000010)        /*!<  */
#define SYSCON_RSTINFO_EOSFAILRST                ((uint32_t)0x00000020)        /*!<  */
#define SYSCON_RSTINFO_FDFAILRST                 ((uint32_t)0x00000040)        /*!<  */
#define SYSCON_RSTINFO_PORST                     ((uint32_t)0x00000080)        /*!<  */
#define SYSCON_RSTINFO_EXRST                     ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_RSTINFO_LVD5VRST                  ((uint32_t)0x00000200)        /*!<  */
#define SYSCON_RSTINFO_LVD1V5RST                 ((uint32_t)0x00000400)        /*!<  */

/********************  Bits definition for SYSCON_RETRIMING register  ******************/
#define SYSCON_RETRIMING_CTRL                    ((uint32_t)0x0000FFFF)        /*!<  */
#define SYSCON_RETRIMING_KEY                     ((uint32_t)0x0000AB56)        /*!<  */

/********************  Bits definition for SYSCON_RSTENCTRL register  ******************/
#define SYSCON_RSTENCTRL_EOSFAILRSTEN            ((uint32_t)0x00000010)        /*!<  */
#define SYSCON_RSTENCTRL_FDFAILRSTEN             ((uint32_t)0x00000020)        /*!<  */
#define SYSCON_RSTENCTRL_BGRSTEN                 ((uint32_t)0x00000040)        /*!<  */
#define SYSCON_RSTENCTRL_VDDLVDRSTEN             ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_RSTENCTRL_LDOLVDRSTEN             ((uint32_t)0x00000200)        /*!<  */
#define SYSCON_RSTENCTRL_RETRIMINGEN             ((uint32_t)0x00000400)        /*!<  */

/********************  Bits definition for SYSCON_CUSTOMERID register  ******************/
#define SYSCON_CUSTOMERID_INFO                   ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_FLASHPROTECT register  ******************/
#define SYSCON_FLASHPROTECT_STATUS               ((uint32_t)0x00000001)        /*!<  */

/********************  Bits definition for SYSCON_AGENTID register  ******************/
#define SYSCON_AGENTID_INFO                      ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO1                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO2                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_UID register  ******************/
#define SYSCON_UID_INFO3                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_CID register  ******************/
#define SYSCON_CID_FLASHSIZE                     ((uint32_t)0x0000000F)        /*!<  */
#define SYSCON_CID_SRAMSIZE                      ((uint32_t)0x000000F0)        /*!<  */
#define SYSCON_CID_PACK                          ((uint32_t)0x00000100)        /*!<  */
#define SYSCON_CID_INFO                          ((uint32_t)0x0000FE00)        /*!<  */

/********************  Bits definition for SYSCON_PID0 register  ******************/
#define SYSCON_PID0_JEP106CODE                   ((uint32_t)0x0000000F)        /*!<  */
#define SYSCON_PID0_BLOCKCOUNT                   ((uint32_t)0x000000F0)        /*!<  */

/********************  Bits definition for SYSCON_PID1 register  ******************/
#define SYSCON_PID1_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID2 register  ******************/
#define SYSCON_PID2_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID3 register  ******************/
#define SYSCON_PID3_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID4 register  ******************/
#define SYSCON_PID4_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID5 register  ******************/
#define SYSCON_PID5_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID6 register  ******************/
#define SYSCON_PID6_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/********************  Bits definition for SYSCON_PID7 register  ******************/
#define SYSCON_PID7_INFO                         ((uint32_t)0xFFFFFFFF)        /*!<  */

/******************************************************************************/
/*                                                                            */
/*                      Analog MIX Function (ANAMIX)                     */
/*                                                                            */
/******************************************************************************/
/********************  Bits definition for ANAMIX_VDDLVDCTRL register  ******************/
#define ANAMIX_VDDLVDCTRL_EN                     ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_VDDLVDCTRL_TRIP                   ((uint32_t)0x0000000E)        /*!<  */
#define ANAMIX_VDDLVDCTRL_2V0                    ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_VDDLVDCTRL_1V7                    ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_VDDLVDCTRL_2V2                    ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_VDDLVDCTRL_2V5                    ((uint32_t)0x00000006)        /*!<  */
#define ANAMIX_VDDLVDCTRL_2V75                   ((uint32_t)0x00000008)        /*!<  */
#define ANAMIX_VDDLVDCTRL_3V0                    ((uint32_t)0x0000000A)        /*!<  */
#define ANAMIX_VDDLVDCTRL_3V5                    ((uint32_t)0x0000000C)        /*!<  */
#define ANAMIX_VDDLVDCTRL_4V0                    ((uint32_t)0x0000000E)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_VDDLVDCTRL register  ******************/
#define ANAMIX_LDOLVDCTRL_TRIP                   ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_LDOLVDCTRL_1V35                   ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_LDOLVDCTRL_1V4                    ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_LDOLVDCTRL_1V45                   ((uint32_t)0x00000004)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_EOSCCON1 register  ******************/
#define ANAMIX_EOSCCON1_EOSCEN                    ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_EOSCCON1_AUTOAMP                   ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_EOSCCON1_EXCLKEN                   ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCRANG                  ((uint32_t)0x00000010)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH              ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH_4NS          ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH_6NS          ((uint32_t)0x00000100)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH_8NS          ((uint32_t)0x00000200)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH_10NS         ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCDEGLITCH_BYPASSEN     ((uint32_t)0x00000400)        /*!<  */
#define ANAMIX_EOSCCON1_EOSCWORK_READY            ((uint32_t)0x80000000)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_EOSCCON2 register  ******************/
#define ANAMIX_EOSCCON2_EOSCDRIVE                 ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX1               ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX1P5             ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX2               ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX2P5             ((uint32_t)0x00000003)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX3               ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX3P5             ((uint32_t)0x00000005)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX4               ((uint32_t)0x00000006)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCDRIVEX4P5             ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP               ((uint32_t)0x00000070)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_9PF           ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_12PF          ((uint32_t)0x00000010)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_15PF          ((uint32_t)0x00000020)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_18PF          ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_21PF          ((uint32_t)0x00000040)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_24PF          ((uint32_t)0x00000050)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_27PF          ((uint32_t)0x00000060)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCLOADCAP_30PF          ((uint32_t)0x00000070)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCFBRES                 ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCFBRES_1100K           ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCFBRES_800K            ((uint32_t)0x00000100)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCFBRES_500K            ((uint32_t)0x00000200)        /*!<  */
#define ANAMIX_EOSCCON2_EOSCFBRES_220K            ((uint32_t)0x00000300)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_IOSC24MCON register  ******************/
#define ANAMIX_IOSC24MCON_IOSCEN                 ((uint32_t)0x00000001)        /*!<  */

/******************************************************************************/

/********************  Bits definition for ANAMIX_IOSC32KCON register  ******************/
#define ANAMIX_IOSC32KCON_IOSCEN                 ((uint32_t)0x00000001)        /*!<  */

/******************************************************************************/

/********************  Bits definition for ANAMIX_FDCON register  ******************/
#define ANAMIX_FDCON_FDEN                        ((uint32_t)0x00000001)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_DEGLITCHCON register  ******************/
#define ANAMIX_DEGLITCHCON_COMP0                 ((uint32_t)0x00000003)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP0_1US             ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP0_2US             ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP0_3US             ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP0_4US             ((uint32_t)0x00000003)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP1                 ((uint32_t)0x0000000C)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP1_1US             ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP1_2US             ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP1_3US             ((uint32_t)0x00000008)        /*!<  */
#define ANAMIX_DEGLITCHCON_COMP1_4US             ((uint32_t)0x0000000C)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA                    ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_2US                ((uint32_t)0x00000010)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_3US                ((uint32_t)0x00000020)        /*!<  */
#define ANAMIX_DEGLITCHCON_PA_4US                ((uint32_t)0x00000030)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB                    ((uint32_t)0x000000C0)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_2US                ((uint32_t)0x00000040)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_3US                ((uint32_t)0x00000080)        /*!<  */
#define ANAMIX_DEGLITCHCON_PB_4US                ((uint32_t)0x000000C0)        /*!<  */
#define ANAMIX_DEGLITCHCON_PF                    ((uint32_t)0x00000300)        /*!<  */
#define ANAMIX_DEGLITCHCON_PF_1US                ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_DEGLITCHCON_PF_2US                ((uint32_t)0x00000100)        /*!<  */
#define ANAMIX_DEGLITCHCON_PF_3US                ((uint32_t)0x00000200)        /*!<  */
#define ANAMIX_DEGLITCHCON_PF_4US                ((uint32_t)0x00000300)        /*!<  */

/******************************************************************************/
/********************  Bits definition for ANAMIX_HSINKCON register  ******************/
#define ANAMIX_HSINKCON_COM0                     ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_HSINKCON_COM1                     ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_HSINKCON_COM2                     ((uint32_t)0x00000004)        /*!<  */
#define ANAMIX_HSINKCON_COM3                     ((uint32_t)0x00000008)        /*!<  */

/********************  Bits definition for ANAMIX_MCODIVCON register  ******************/
#define ANAMIX_MCODIVCON                         ((uint32_t)0x00000007)        /*!<  */
#define ANAMIX_MCODIVCON_1DIV                    ((uint32_t)0x00000000)        /*!<  */
#define ANAMIX_MCODIVCON_2DIV                    ((uint32_t)0x00000001)        /*!<  */
#define ANAMIX_MCODIVCON_4DIV                    ((uint32_t)0x00000002)        /*!<  */
#define ANAMIX_MCODIVCON_8DIV                    ((uint32_t)0x00000003)        /*!<  */
#define ANAMIX_MCODIVCON_16DIV                   ((uint32_t)0x00000004)        /*!<  */

/******************************************************************************/
/*                                                                            */
/*                       General Purpose IOs (GPIO)                           */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
/*!<******************  Bit definition for GPIO_DATA register  *******************/
#define GPIO_DATA_DATA0                        ((uint16_t)0x0001)               /*!< Port input data, bit 0 */
#define GPIO_DATA_DATA1                        ((uint16_t)0x0002)               /*!< Port input data, bit 1 */
#define GPIO_DATA_DATA2                        ((uint16_t)0x0004)               /*!< Port input data, bit 2 */
#define GPIO_DATA_DATA3                        ((uint16_t)0x0008)               /*!< Port input data, bit 3 */
#define GPIO_DATA_DATA4                        ((uint16_t)0x0010)               /*!< Port input data, bit 4 */
#define GPIO_DATA_DATA5                        ((uint16_t)0x0020)               /*!< Port input data, bit 5 */
#define GPIO_DATA_DATA6                        ((uint16_t)0x0040)               /*!< Port input data, bit 6 */
#define GPIO_DATA_DATA7                        ((uint16_t)0x0080)               /*!< Port input data, bit 7 */
#define GPIO_DATA_DATA8                        ((uint16_t)0x0100)               /*!< Port input data, bit 8 */
#define GPIO_DATA_DATA9                        ((uint16_t)0x0200)               /*!< Port input data, bit 9 */
#define GPIO_DATA_DATA10                       ((uint16_t)0x0400)               /*!< Port input data, bit 10 */
#define GPIO_DATA_DATA11                       ((uint16_t)0x0800)               /*!< Port input data, bit 11 */
#define GPIO_DATA_DATA12                       ((uint16_t)0x1000)               /*!< Port input data, bit 12 */
#define GPIO_DATA_DATA13                       ((uint16_t)0x2000)               /*!< Port input data, bit 13 */
#define GPIO_DATA_DATA14                       ((uint16_t)0x4000)               /*!< Port input data, bit 14 */
#define GPIO_DATA_DATA15                       ((uint16_t)0x8000)               /*!< Port input data, bit 15 */

/*******************  Bit definition for GPIO_DATAOUT register  *******************/
#define GPIO_DATAOUT_DATAOUT0                        ((uint16_t)0x0001)         /*!< Port output data, bit 0 */
#define GPIO_DATAOUT_DATAOUT1                        ((uint16_t)0x0002)         /*!< Port output data, bit 1 */
#define GPIO_DATAOUT_DATAOUT2                        ((uint16_t)0x0004)         /*!< Port output data, bit 2 */
#define GPIO_DATAOUT_DATAOUT3                        ((uint16_t)0x0008)         /*!< Port output data, bit 3 */
#define GPIO_DATAOUT_DATAOUT4                        ((uint16_t)0x0010)         /*!< Port output data, bit 4 */
#define GPIO_DATAOUT_DATAOUT5                        ((uint16_t)0x0020)         /*!< Port output data, bit 5 */
#define GPIO_DATAOUT_DATAOUT6                        ((uint16_t)0x0040)         /*!< Port output data, bit 6 */
#define GPIO_DATAOUT_DATAOUT7                        ((uint16_t)0x0080)         /*!< Port output data, bit 7 */
#define GPIO_DATAOUT_DATAOUT8                        ((uint16_t)0x0100)         /*!< Port output data, bit 8 */
#define GPIO_DATAOUT_DATAOUT9                        ((uint16_t)0x0200)         /*!< Port output data, bit 9 */
#define GPIO_DATAOUT_DATAOUT10                       ((uint16_t)0x0400)         /*!< Port output data, bit 10 */
#define GPIO_DATAOUT_DATAOUT11                       ((uint16_t)0x0800)         /*!< Port output data, bit 11 */
#define GPIO_DATAOUT_DATAOUT12                       ((uint16_t)0x1000)         /*!< Port output data, bit 12 */
#define GPIO_DATAOUT_DATAOUT13                       ((uint16_t)0x2000)         /*!< Port output data, bit 13 */
#define GPIO_DATAOUT_DATAOUT14                       ((uint16_t)0x4000)         /*!< Port output data, bit 14 */
#define GPIO_DATAOUT_DATAOUT15                       ((uint16_t)0x8000)         /*!< Port output data, bit 15 */

/*******************  Bit definition for GPIO_OES register  *******************/
#define GPIO_OES_OES0                        ((uint16_t)0x0001)                 /*!< Port output enable bit set, bit 0 */
#define GPIO_OES_OES1                        ((uint16_t)0x0002)                 /*!< Port output enable bit set, bit 1 */
#define GPIO_OES_OES2                        ((uint16_t)0x0004)                 /*!< Port output enable bit set, bit 2 */
#define GPIO_OES_OES3                        ((uint16_t)0x0008)                 /*!< Port output enable bit set, bit 3 */
#define GPIO_OES_OES4                        ((uint16_t)0x0010)                 /*!< Port output enable bit set, bit 4 */
#define GPIO_OES_OES5                        ((uint16_t)0x0020)                 /*!< Port output enable bit set, bit 5 */
#define GPIO_OES_OES6                        ((uint16_t)0x0040)                 /*!< Port output enable bit set, bit 6 */
#define GPIO_OES_OES7                        ((uint16_t)0x0080)                 /*!< Port output enable bit set, bit 7 */
#define GPIO_OES_OES8                        ((uint16_t)0x0100)                 /*!< Port output enable bit set, bit 8 */
#define GPIO_OES_OES9                        ((uint16_t)0x0200)                 /*!< Port output enable bit set, bit 9 */
#define GPIO_OES_OES10                       ((uint16_t)0x0400)                 /*!< Port output enable bit set, bit 10 */
#define GPIO_OES_OES11                       ((uint16_t)0x0800)                 /*!< Port output enable bit set, bit 11 */
#define GPIO_OES_OES12                       ((uint16_t)0x1000)                 /*!< Port output enable bit set, bit 12 */
#define GPIO_OES_OES13                       ((uint16_t)0x2000)                 /*!< Port output enable bit set, bit 13 */
#define GPIO_OES_OES14                       ((uint16_t)0x4000)                 /*!< Port output enable bit set, bit 14 */
#define GPIO_OES_OES15                       ((uint16_t)0x8000)                 /*!< Port output enable bit set, bit 15 */

/*******************  Bit definition for GPIO_OES register  *******************/
#define GPIO_OEC_OEC0                        ((uint16_t)0x0001)                 /*!< Port output enable bit reset, bit 0 */
#define GPIO_OEC_OEC1                        ((uint16_t)0x0002)                 /*!< Port output enable bit reset, bit 1 */
#define GPIO_OEC_OEC2                        ((uint16_t)0x0004)                 /*!< Port output enable bit reset, bit 2 */
#define GPIO_OEC_OEC3                        ((uint16_t)0x0008)                 /*!< Port output enable bit reset, bit 3 */
#define GPIO_OEC_OEC4                        ((uint16_t)0x0010)                 /*!< Port output enable bit reset, bit 4 */
#define GPIO_OEC_OEC5                        ((uint16_t)0x0020)                 /*!< Port output enable bit reset, bit 5 */
#define GPIO_OEC_OEC6                        ((uint16_t)0x0040)                 /*!< Port output enable bit reset, bit 6 */
#define GPIO_OEC_OEC7                        ((uint16_t)0x0080)                 /*!< Port output enable bit reset, bit 7 */
#define GPIO_OEC_OEC8                        ((uint16_t)0x0100)                 /*!< Port output enable bit reset, bit 8 */
#define GPIO_OEC_OEC9                        ((uint16_t)0x0200)                 /*!< Port output enable bit reset, bit 9 */
#define GPIO_OEC_OEC10                       ((uint16_t)0x0400)                 /*!< Port output enable bit reset, bit 10 */
#define GPIO_OEC_OEC11                       ((uint16_t)0x0800)                 /*!< Port output enable bit reset, bit 11 */
#define GPIO_OEC_OEC12                       ((uint16_t)0x1000)                 /*!< Port output enable bit reset, bit 12 */
#define GPIO_OEC_OEC13                       ((uint16_t)0x2000)                 /*!< Port output enable bit reset, bit 13 */
#define GPIO_OEC_OEC14                       ((uint16_t)0x4000)                 /*!< Port output enable bit reset, bit 14 */
#define GPIO_OEC_OEC15                       ((uint16_t)0x8000)                 /*!< Port output enable bit reset, bit 15 */

/****************** Bit definition for GPIO_AFS1 register  ********************/
#define GPIO_AFS1_AFS0            ((uint32_t)0x0000000F)
#define GPIO_AFS1_AFS1            ((uint32_t)0x000000F0)
#define GPIO_AFS1_AFS2            ((uint32_t)0x00000F00)
#define GPIO_AFS1_AFS3            ((uint32_t)0x0000F000)
#define GPIO_AFS1_AFS4            ((uint32_t)0x000F0000)
#define GPIO_AFS1_AFS5            ((uint32_t)0x00F00000)
#define GPIO_AFS1_AFS6            ((uint32_t)0x0F000000)
#define GPIO_AFS1_AFS7            ((uint32_t)0xF0000000)

/****************** Bit definition for GPIO_AFS2 register  ********************/
#define GPIO_AFS2_AFS0            ((uint32_t)0x0000000F)
#define GPIO_AFS2_AFS1            ((uint32_t)0x000000F0)
#define GPIO_AFS2_AFS2            ((uint32_t)0x00000F00)
#define GPIO_AFS2_AFS3            ((uint32_t)0x0000F000)
#define GPIO_AFS2_AFS4            ((uint32_t)0x000F0000)
#define GPIO_AFS2_AFS5            ((uint32_t)0x00F00000)
#define GPIO_AFS2_AFS6            ((uint32_t)0x0F000000)
#define GPIO_AFS2_AFS7            ((uint32_t)0xF0000000)



/*******************  Bit definition for GPIO_AFC register  *******************/
#define GPIO_AFC_AFC0                        ((uint16_t)0x0001)                 /*!< Port alternate function bit reset, bit 0 */
#define GPIO_AFC_AFC1                        ((uint16_t)0x0002)                 /*!< Port alternate function bit reset, bit 1 */
#define GPIO_AFC_AFC2                        ((uint16_t)0x0004)                 /*!< Port alternate function bit reset, bit 2 */
#define GPIO_AFC_AFC3                        ((uint16_t)0x0008)                 /*!< Port alternate function bit reset, bit 3 */
#define GPIO_AFC_AFC4                        ((uint16_t)0x0010)                 /*!< Port alternate function bit reset, bit 4 */
#define GPIO_AFC_AFC5                        ((uint16_t)0x0020)                 /*!< Port alternate function bit reset, bit 5 */
#define GPIO_AFC_AFC6                        ((uint16_t)0x0040)                 /*!< Port alternate function bit reset, bit 6 */
#define GPIO_AFC_AFC7                        ((uint16_t)0x0080)                 /*!< Port alternate function bit reset, bit 7 */
#define GPIO_AFC_AFC8                        ((uint16_t)0x0100)                 /*!< Port alternate function bit reset, bit 8 */
#define GPIO_AFC_AFC9                        ((uint16_t)0x0200)                 /*!< Port alternate function bit reset, bit 9 */
#define GPIO_AFC_AFC10                       ((uint16_t)0x0400)                 /*!< Port alternate function bit reset, bit 10 */
#define GPIO_AFC_AFC11                       ((uint16_t)0x0800)                 /*!< Port alternate function bit reset, bit 11 */
#define GPIO_AFC_AFC12                       ((uint16_t)0x1000)                 /*!< Port alternate function bit reset, bit 12 */
#define GPIO_AFC_AFC13                       ((uint16_t)0x2000)                 /*!< Port alternate function bit reset, bit 13 */
#define GPIO_AFC_AFC14                       ((uint16_t)0x4000)                 /*!< Port alternate function bit reset, bit 14 */
#define GPIO_AFC_AFC15                       ((uint16_t)0x8000)                 /*!< Port alternate function bit reset, bit 15 */


/*******************  Bit definition for GPIO_IES register  *******************/
#define GPIO_IES_IES0                        ((uint16_t)0x0001)                 /*!< Port interrupt enable bit set, bit 0 */
#define GPIO_IES_IES1                        ((uint16_t)0x0002)                 /*!< Port interrupt enable bit set, bit 1 */
#define GPIO_IES_IES2                        ((uint16_t)0x0004)                 /*!< Port interrupt enable bit set, bit 2 */
#define GPIO_IES_IES3                        ((uint16_t)0x0008)                 /*!< Port interrupt enable bit set, bit 3 */
#define GPIO_IES_IES4                        ((uint16_t)0x0010)                 /*!< Port interrupt enable bit set, bit 4 */
#define GPIO_IES_IES5                        ((uint16_t)0x0020)                 /*!< Port interrupt enable bit set, bit 5 */
#define GPIO_IES_IES6                        ((uint16_t)0x0040)                 /*!< Port interrupt enable bit set, bit 6 */
#define GPIO_IES_IES7                        ((uint16_t)0x0080)                 /*!< Port interrupt enable bit set, bit 7 */
#define GPIO_IES_IES8                        ((uint16_t)0x0100)                 /*!< Port interrupt enable bit set, bit 8 */
#define GPIO_IES_IES9                        ((uint16_t)0x0200)                 /*!< Port interrupt enable bit set, bit 9 */
#define GPIO_IES_IES10                       ((uint16_t)0x0400)                 /*!< Port interrupt enable bit set, bit 10 */
#define GPIO_IES_IES11                       ((uint16_t)0x0800)                 /*!< Port interrupt enable bit set, bit 11 */
#define GPIO_IES_IES12                       ((uint16_t)0x1000)                 /*!< Port interrupt enable bit set, bit 12 */
#define GPIO_IES_IES13                       ((uint16_t)0x2000)                 /*!< Port interrupt enable bit set, bit 13 */
#define GPIO_IES_IES14                       ((uint16_t)0x4000)                 /*!< Port interrupt enable bit set, bit 14 */
#define GPIO_IES_IES15                       ((uint16_t)0x8000)                 /*!< Port interrupt enable bit set, bit 15 */

/*******************  Bit definition for GPIO_IES register  *******************/
#define GPIO_IEC_IEC0                        ((uint16_t)0x0001)                 /*!< Port interrupt enable bit reset, bit 0 */
#define GPIO_IEC_IEC1                        ((uint16_t)0x0002)                 /*!< Port interrupt enable bit reset, bit 1 */
#define GPIO_IEC_IEC2                        ((uint16_t)0x0004)                 /*!< Port interrupt enable bit reset, bit 2 */
#define GPIO_IEC_IEC3                        ((uint16_t)0x0008)                 /*!< Port interrupt enable bit reset, bit 3 */
#define GPIO_IEC_IEC4                        ((uint16_t)0x0010)                 /*!< Port interrupt enable bit reset, bit 4 */
#define GPIO_IEC_IEC5                        ((uint16_t)0x0020)                 /*!< Port interrupt enable bit reset, bit 5 */
#define GPIO_IEC_IEC6                        ((uint16_t)0x0040)                 /*!< Port interrupt enable bit reset, bit 6 */
#define GPIO_IEC_IEC7                        ((uint16_t)0x0080)                 /*!< Port interrupt enable bit reset, bit 7 */
#define GPIO_IEC_IEC8                        ((uint16_t)0x0100)                 /*!< Port interrupt enable bit reset, bit 8 */
#define GPIO_IEC_IEC9                        ((uint16_t)0x0200)                 /*!< Port interrupt enable bit reset, bit 9 */
#define GPIO_IEC_IEC10                       ((uint16_t)0x0400)                 /*!< Port interrupt enable bit reset, bit 10 */
#define GPIO_IEC_IEC11                       ((uint16_t)0x0800)                 /*!< Port interrupt enable bit reset, bit 11 */
#define GPIO_IEC_IEC12                       ((uint16_t)0x1000)                 /*!< Port interrupt enable bit reset, bit 12 */
#define GPIO_IEC_IEC13                       ((uint16_t)0x2000)                 /*!< Port interrupt enable bit reset, bit 13 */
#define GPIO_IEC_IEC14                       ((uint16_t)0x4000)                 /*!< Port interrupt enable bit reset, bit 14 */
#define GPIO_IEC_IEC15                       ((uint16_t)0x8000)                 /*!< Port interrupt enable bit reset, bit 15 */

/*******************  Bit definition for GPIO_ITS0 register  *******************/
#define GPIO_ITS0_ITS0                        ((uint16_t)0x0001)                /*!< Port interrupt type0 bit set, bit 0 */
#define GPIO_ITS0_ITS1                        ((uint16_t)0x0002)                /*!< Port interrupt type0 bit set, bit 1 */
#define GPIO_ITS0_ITS2                        ((uint16_t)0x0004)                /*!< Port interrupt type0 bit set, bit 2 */
#define GPIO_ITS0_ITS3                        ((uint16_t)0x0008)                /*!< Port interrupt type0 bit set, bit 3 */
#define GPIO_ITS0_ITS4                        ((uint16_t)0x0010)                /*!< Port interrupt type0 bit set, bit 4 */
#define GPIO_ITS0_ITS5                        ((uint16_t)0x0020)                /*!< Port interrupt type0 bit set, bit 5 */
#define GPIO_ITS0_ITS6                        ((uint16_t)0x0040)                /*!< Port interrupt type0 bit set, bit 6 */
#define GPIO_ITS0_ITS7                        ((uint16_t)0x0080)                /*!< Port interrupt type0 bit set, bit 7 */
#define GPIO_ITS0_ITS8                        ((uint16_t)0x0100)                /*!< Port interrupt type0 bit set, bit 8 */
#define GPIO_ITS0_ITS9                        ((uint16_t)0x0200)                /*!< Port interrupt type0 bit set, bit 9 */
#define GPIO_ITS0_ITS10                       ((uint16_t)0x0400)                /*!< Port interrupt type0 bit set, bit 10 */
#define GPIO_ITS0_ITS11                       ((uint16_t)0x0800)                /*!< Port interrupt type0 bit set, bit 11 */
#define GPIO_ITS0_ITS12                       ((uint16_t)0x1000)                /*!< Port interrupt type0 bit set, bit 12 */
#define GPIO_ITS0_ITS13                       ((uint16_t)0x2000)                /*!< Port interrupt type0 bit set, bit 13 */
#define GPIO_ITS0_ITS14                       ((uint16_t)0x4000)                /*!< Port interrupt type0 bit set, bit 14 */
#define GPIO_ITS0_ITS15                       ((uint16_t)0x8000)                /*!< Port interrupt type0 bit set, bit 15 */

/*******************  Bit definition for GPIO_ITC0 register  *******************/
#define GPIO_ITC0_ITC0                        ((uint16_t)0x0001)                /*!< Port interrupt type0 bit reset, bit 0 */
#define GPIO_ITC0_ITC1                        ((uint16_t)0x0002)                /*!< Port interrupt type0 bit reset, bit 1 */
#define GPIO_ITC0_ITC2                        ((uint16_t)0x0004)                /*!< Port interrupt type0 bit reset, bit 2 */
#define GPIO_ITC0_ITC3                        ((uint16_t)0x0008)                /*!< Port interrupt type0 bit reset, bit 3 */
#define GPIO_ITC0_ITC4                        ((uint16_t)0x0010)                /*!< Port interrupt type0 bit reset, bit 4 */
#define GPIO_ITC0_ITC5                        ((uint16_t)0x0020)                /*!< Port interrupt type0 bit reset, bit 5 */
#define GPIO_ITC0_ITC6                        ((uint16_t)0x0040)                /*!< Port interrupt type0 bit reset, bit 6 */
#define GPIO_ITC0_ITC7                        ((uint16_t)0x0080)                /*!< Port interrupt type0 bit reset, bit 7 */
#define GPIO_ITC0_ITC8                        ((uint16_t)0x0100)                /*!< Port interrupt type0 bit reset, bit 8 */
#define GPIO_ITC0_ITC9                        ((uint16_t)0x0200)                /*!< Port interrupt type0 bit reset, bit 9 */
#define GPIO_ITC0_ITC10                       ((uint16_t)0x0400)                /*!< Port interrupt type0 bit reset, bit 10 */
#define GPIO_ITC0_ITC11                       ((uint16_t)0x0800)                /*!< Port interrupt type0 bit reset, bit 11 */
#define GPIO_ITC0_ITC12                       ((uint16_t)0x1000)                /*!< Port interrupt type0 bit reset, bit 12 */
#define GPIO_ITC0_ITC13                       ((uint16_t)0x2000)                /*!< Port interrupt type0 bit reset, bit 13 */
#define GPIO_ITC0_ITC14                       ((uint16_t)0x4000)                /*!< Port interrupt type0 bit reset, bit 14 */
#define GPIO_ITC0_ITC15                       ((uint16_t)0x8000)                /*!< Port interrupt type0 bit reset, bit 15 */

/*******************  Bit definition for GPIO_ITS1 register  *******************/
#define GPIO_ITS1_ITS0                        ((uint16_t)0x0001)                /*!< Port interrupt type1 bit set, bit 0 */
#define GPIO_ITS1_ITS1                        ((uint16_t)0x0002)                /*!< Port interrupt type1 bit set, bit 1 */
#define GPIO_ITS1_ITS2                        ((uint16_t)0x0004)                /*!< Port interrupt type1 bit set, bit 2 */
#define GPIO_ITS1_ITS3                        ((uint16_t)0x0008)                /*!< Port interrupt type1 bit set, bit 3 */
#define GPIO_ITS1_ITS4                        ((uint16_t)0x0010)                /*!< Port interrupt type1 bit set, bit 4 */
#define GPIO_ITS1_ITS5                        ((uint16_t)0x0020)                /*!< Port interrupt type1 bit set, bit 5 */
#define GPIO_ITS1_ITS6                        ((uint16_t)0x0040)                /*!< Port interrupt type1 bit set, bit 6 */
#define GPIO_ITS1_ITS7                        ((uint16_t)0x0080)                /*!< Port interrupt type1 bit set, bit 7 */
#define GPIO_ITS1_ITS8                        ((uint16_t)0x0100)                /*!< Port interrupt type1 bit set, bit 8 */
#define GPIO_ITS1_ITS9                        ((uint16_t)0x0200)                /*!< Port interrupt type1 bit set, bit 9 */
#define GPIO_ITS1_ITS10                       ((uint16_t)0x0400)                /*!< Port interrupt type1 bit set, bit 10 */
#define GPIO_ITS1_ITS11                       ((uint16_t)0x0800)                /*!< Port interrupt type1 bit set, bit 11 */
#define GPIO_ITS1_ITS12                       ((uint16_t)0x1000)                /*!< Port interrupt type1 bit set, bit 12 */
#define GPIO_ITS1_ITS13                       ((uint16_t)0x2000)                /*!< Port interrupt type1 bit set, bit 13 */
#define GPIO_ITS1_ITS14                       ((uint16_t)0x4000)                /*!< Port interrupt type1 bit set, bit 14 */
#define GPIO_ITS1_ITS15                       ((uint16_t)0x8000)                /*!< Port interrupt type1 bit set, bit 15 */

/*******************  Bit definition for GPIO_ITC1 register  *******************/
#define GPIO_ITC1_ITC0                        ((uint16_t)0x0001)                /*!< Port interrupt type1 bit reset, bit 0 */
#define GPIO_ITC1_ITC1                        ((uint16_t)0x0002)                /*!< Port interrupt type1 bit reset, bit 1 */
#define GPIO_ITC1_ITC2                        ((uint16_t)0x0004)                /*!< Port interrupt type1 bit reset, bit 2 */
#define GPIO_ITC1_ITC3                        ((uint16_t)0x0008)                /*!< Port interrupt type1 bit reset, bit 3 */
#define GPIO_ITC1_ITC4                        ((uint16_t)0x0010)                /*!< Port interrupt type1 bit reset, bit 4 */
#define GPIO_ITC1_ITC5                        ((uint16_t)0x0020)                /*!< Port interrupt type1 bit reset, bit 5 */
#define GPIO_ITC1_ITC6                        ((uint16_t)0x0040)                /*!< Port interrupt type1 bit reset, bit 6 */
#define GPIO_ITC1_ITC7                        ((uint16_t)0x0080)                /*!< Port interrupt type1 bit reset, bit 7 */
#define GPIO_ITC1_ITC8                        ((uint16_t)0x0100)                /*!< Port interrupt type1 bit reset, bit 8 */
#define GPIO_ITC1_ITC9                        ((uint16_t)0x0200)                /*!< Port interrupt type1 bit reset, bit 9 */
#define GPIO_ITC1_ITC10                       ((uint16_t)0x0400)                /*!< Port interrupt type1 bit reset, bit 10 */
#define GPIO_ITC1_ITC11                       ((uint16_t)0x0800)                /*!< Port interrupt type1 bit reset, bit 11 */
#define GPIO_ITC1_ITC12                       ((uint16_t)0x1000)                /*!< Port interrupt type1 bit reset, bit 12 */
#define GPIO_ITC1_ITC13                       ((uint16_t)0x2000)                /*!< Port interrupt type1 bit reset, bit 13 */
#define GPIO_ITC1_ITC14                       ((uint16_t)0x4000)                /*!< Port interrupt type1 bit reset, bit 14 */
#define GPIO_ITC1_ITC15                       ((uint16_t)0x8000)                /*!< Port interrupt type1 bit reset, bit 15 */

/*******************  Bit definition for GPIO_PLS register  *******************/
#define GPIO_PLS_PLS0                        ((uint16_t)0x0001)                 /*!< Port interrupt polarity bit set, bit 0 */
#define GPIO_PLS_PLS1                        ((uint16_t)0x0002)                 /*!< Port interrupt polarity bit set, bit 1 */
#define GPIO_PLS_PLS2                        ((uint16_t)0x0004)                 /*!< Port interrupt polarity bit set, bit 2 */
#define GPIO_PLS_PLS3                        ((uint16_t)0x0008)                 /*!< Port interrupt polarity bit set, bit 3 */
#define GPIO_PLS_PLS4                        ((uint16_t)0x0010)                 /*!< Port interrupt polarity bit set, bit 4 */
#define GPIO_PLS_PLS5                        ((uint16_t)0x0020)                 /*!< Port interrupt polarity bit set, bit 5 */
#define GPIO_PLS_PLS6                        ((uint16_t)0x0040)                 /*!< Port interrupt polarity bit set, bit 6 */
#define GPIO_PLS_PLS7                        ((uint16_t)0x0080)                 /*!< Port interrupt polarity bit set, bit 7 */
#define GPIO_PLS_PLS8                        ((uint16_t)0x0100)                 /*!< Port interrupt polarity bit set, bit 8 */
#define GPIO_PLS_PLS9                        ((uint16_t)0x0200)                 /*!< Port interrupt polarity bit set, bit 9 */
#define GPIO_PLS_PLS10                       ((uint16_t)0x0400)                 /*!< Port interrupt polarity bit set, bit 10 */
#define GPIO_PLS_PLS11                       ((uint16_t)0x0800)                 /*!< Port interrupt polarity bit set, bit 11 */
#define GPIO_PLS_PLS12                       ((uint16_t)0x1000)                 /*!< Port interrupt polarity bit set, bit 12 */
#define GPIO_PLS_PLS13                       ((uint16_t)0x2000)                 /*!< Port interrupt polarity bit set, bit 13 */
#define GPIO_PLS_PLS14                       ((uint16_t)0x4000)                 /*!< Port interrupt polarity bit set, bit 14 */
#define GPIO_PLS_PLS15                       ((uint16_t)0x8000)                 /*!< Port interrupt polarity bit set, bit 15 */

/*******************  Bit definition for GPIO_PLC register  *******************/
#define GPIO_PLC_PLC0                        ((uint16_t)0x0001)                 /*!< Port interrupt polarity bit reset, bit 0 */
#define GPIO_PLC_PLC1                        ((uint16_t)0x0002)                 /*!< Port interrupt polarity bit reset, bit 1 */
#define GPIO_PLC_PLC2                        ((uint16_t)0x0004)                 /*!< Port interrupt polarity bit reset, bit 2 */
#define GPIO_PLC_PLC3                        ((uint16_t)0x0008)                 /*!< Port interrupt polarity bit reset, bit 3 */
#define GPIO_PLC_PLC4                        ((uint16_t)0x0010)                 /*!< Port interrupt polarity bit reset, bit 4 */
#define GPIO_PLC_PLC5                        ((uint16_t)0x0020)                 /*!< Port interrupt polarity bit reset, bit 5 */
#define GPIO_PLC_PLC6                        ((uint16_t)0x0040)                 /*!< Port interrupt polarity bit reset, bit 6 */
#define GPIO_PLC_PLC7                        ((uint16_t)0x0080)                 /*!< Port interrupt polarity bit reset, bit 7 */
#define GPIO_PLC_PLC8                        ((uint16_t)0x0100)                 /*!< Port interrupt polarity bit reset, bit 8 */
#define GPIO_PLC_PLC9                        ((uint16_t)0x0200)                 /*!< Port interrupt polarity bit reset, bit 9 */
#define GPIO_PLC_PLC10                       ((uint16_t)0x0400)                 /*!< Port interrupt polarity bit reset, bit 10 */
#define GPIO_PLC_PLC11                       ((uint16_t)0x0800)                 /*!< Port interrupt polarity bit reset, bit 11 */
#define GPIO_PLC_PLC12                       ((uint16_t)0x1000)                 /*!< Port interrupt polarity bit reset, bit 12 */
#define GPIO_PLC_PLC13                       ((uint16_t)0x2000)                 /*!< Port interrupt polarity bit reset, bit 13 */
#define GPIO_PLC_PLC14                       ((uint16_t)0x4000)                 /*!< Port interrupt polarity bit reset, bit 14 */
#define GPIO_PLC_PLC15                       ((uint16_t)0x8000)                 /*!< Port interrupt polarity bit reset, bit 15 */

/*******************  Bit definition for GPIO_PR register  *******************/
#define GPIO_PR_PR0                        ((uint16_t)0x0001)                   /*!< Port interrupt pending bit set, bit 0 */
#define GPIO_PR_PR1                        ((uint16_t)0x0002)                   /*!< Port interrupt pending bit set, bit 1 */
#define GPIO_PR_PR2                        ((uint16_t)0x0004)                   /*!< Port interrupt pending bit set, bit 2 */
#define GPIO_PR_PR3                        ((uint16_t)0x0008)                   /*!< Port interrupt pending bit set, bit 3 */
#define GPIO_PR_PR4                        ((uint16_t)0x0010)                   /*!< Port interrupt pending bit set, bit 4 */
#define GPIO_PR_PR5                        ((uint16_t)0x0020)                   /*!< Port interrupt pending bit set, bit 5 */
#define GPIO_PR_PR6                        ((uint16_t)0x0040)                   /*!< Port interrupt pending bit set, bit 6 */
#define GPIO_PR_PR7                        ((uint16_t)0x0080)                   /*!< Port interrupt pending bit set, bit 7 */
#define GPIO_PR_PR8                        ((uint16_t)0x0100)                   /*!< Port interrupt pending bit set, bit 8 */
#define GPIO_PR_PR9                        ((uint16_t)0x0200)                   /*!< Port interrupt pending bit set, bit 9 */
#define GPIO_PR_PR10                       ((uint16_t)0x0400)                   /*!< Port interrupt pending bit set, bit 10 */
#define GPIO_PR_PR11                       ((uint16_t)0x0800)                   /*!< Port interrupt pending bit set, bit 11 */
#define GPIO_PR_PR12                       ((uint16_t)0x1000)                   /*!< Port interrupt pending bit set, bit 12 */
#define GPIO_PR_PR13                       ((uint16_t)0x2000)                   /*!< Port interrupt pending bit set, bit 13 */
#define GPIO_PR_PR14                       ((uint16_t)0x4000)                   /*!< Port interrupt pending bit set, bit 14 */
#define GPIO_PR_PR15                       ((uint16_t)0x8000)                   /*!< Port interrupt pending bit set, bit 15 */

/*******************  Bit definition for GPIO_PUS register  *******************/
#define GPIO_PUS_PUS0                        ((uint16_t)0x0001)                 /*!< Port internal pull-up bit set, bit 0 */
#define GPIO_PUS_PUS1                        ((uint16_t)0x0002)                 /*!< Port internal pull-up bit set, bit 1 */
#define GPIO_PUS_PUS2                        ((uint16_t)0x0004)                 /*!< Port internal pull-up bit set, bit 2 */
#define GPIO_PUS_PUS3                        ((uint16_t)0x0008)                 /*!< Port internal pull-up bit set, bit 3 */
#define GPIO_PUS_PUS4                        ((uint16_t)0x0010)                 /*!< Port internal pull-up bit set, bit 4 */
#define GPIO_PUS_PUS5                        ((uint16_t)0x0020)                 /*!< Port internal pull-up bit set, bit 5 */
#define GPIO_PUS_PUS6                        ((uint16_t)0x0040)                 /*!< Port internal pull-up bit set, bit 6 */
#define GPIO_PUS_PUS7                        ((uint16_t)0x0080)                 /*!< Port internal pull-up bit set, bit 7 */
#define GPIO_PUS_PUS8                        ((uint16_t)0x0100)                 /*!< Port internal pull-up bit set, bit 8 */
#define GPIO_PUS_PUS9                        ((uint16_t)0x0200)                 /*!< Port internal pull-up bit set, bit 9 */
#define GPIO_PUS_PUS10                       ((uint16_t)0x0400)                 /*!< Port internal pull-up bit set, bit 10 */
#define GPIO_PUS_PUS11                       ((uint16_t)0x0800)                 /*!< Port internal pull-up bit set, bit 11 */
#define GPIO_PUS_PUS12                       ((uint16_t)0x1000)                 /*!< Port internal pull-up bit set, bit 12 */
#define GPIO_PUS_PUS13                       ((uint16_t)0x2000)                 /*!< Port internal pull-up bit set, bit 13 */
#define GPIO_PUS_PUS14                       ((uint16_t)0x4000)                 /*!< Port internal pull-up bit set, bit 14 */
#define GPIO_PUS_PUS15                       ((uint16_t)0x8000)                 /*!< Port internal pull-up bit set, bit 15 */

/*******************  Bit definition for GPIO_PUC register  *******************/
#define GPIO_PUC_PUC0                        ((uint16_t)0x0001)                 /*!< Port internal pull-up bit reset, bit 0 */
#define GPIO_PUC_PUC1                        ((uint16_t)0x0002)                 /*!< Port internal pull-up bit reset, bit 1 */
#define GPIO_PUC_PUC2                        ((uint16_t)0x0004)                 /*!< Port internal pull-up bit reset, bit 2 */
#define GPIO_PUC_PUC3                        ((uint16_t)0x0008)                 /*!< Port internal pull-up bit reset, bit 3 */
#define GPIO_PUC_PUC4                        ((uint16_t)0x0010)                 /*!< Port internal pull-up bit reset, bit 4 */
#define GPIO_PUC_PUC5                        ((uint16_t)0x0020)                 /*!< Port internal pull-up bit reset, bit 5 */
#define GPIO_PUC_PUC6                        ((uint16_t)0x0040)                 /*!< Port internal pull-up bit reset, bit 6 */
#define GPIO_PUC_PUC7                        ((uint16_t)0x0080)                 /*!< Port internal pull-up bit reset, bit 7 */
#define GPIO_PUC_PUC8                        ((uint16_t)0x0100)                 /*!< Port internal pull-up bit reset, bit 8 */
#define GPIO_PUC_PUC9                        ((uint16_t)0x0200)                 /*!< Port internal pull-up bit reset, bit 9 */
#define GPIO_PUC_PUC10                       ((uint16_t)0x0400)                 /*!< Port internal pull-up bit reset, bit 10 */
#define GPIO_PUC_PUC11                       ((uint16_t)0x0800)                 /*!< Port internal pull-up bit reset, bit 11 */
#define GPIO_PUC_PUC12                       ((uint16_t)0x1000)                 /*!< Port internal pull-up bit reset, bit 12 */
#define GPIO_PUC_PUC13                       ((uint16_t)0x2000)                 /*!< Port internal pull-up bit reset, bit 13 */
#define GPIO_PUC_PUC14                       ((uint16_t)0x4000)                 /*!< Port internal pull-up bit reset, bit 14 */
#define GPIO_PUC_PUC15                       ((uint16_t)0x8000)                 /*!< Port internal pull-up bit reset, bit 15 */

/*******************  Bit definition for GPIO_PDS register  *******************/
#define GPIO_PDS_PDS0                        ((uint16_t)0x0001)                 /*!< Port internal pull-down bit set, bit 0 */
#define GPIO_PDS_PDS1                        ((uint16_t)0x0002)                 /*!< Port internal pull-down bit set, bit 1 */
#define GPIO_PDS_PDS2                        ((uint16_t)0x0004)                 /*!< Port internal pull-down bit set, bit 2 */
#define GPIO_PDS_PDS3                        ((uint16_t)0x0008)                 /*!< Port internal pull-down bit set, bit 3 */
#define GPIO_PDS_PDS4                        ((uint16_t)0x0010)                 /*!< Port internal pull-down bit set, bit 4 */
#define GPIO_PDS_PDS5                        ((uint16_t)0x0020)                 /*!< Port internal pull-down bit set, bit 5 */
#define GPIO_PDS_PDS6                        ((uint16_t)0x0040)                 /*!< Port internal pull-down bit set, bit 6 */
#define GPIO_PDS_PDS7                        ((uint16_t)0x0080)                 /*!< Port internal pull-down bit set, bit 7 */
#define GPIO_PDS_PDS8                        ((uint16_t)0x0100)                 /*!< Port internal pull-down bit set, bit 8 */
#define GPIO_PDS_PDS9                        ((uint16_t)0x0200)                 /*!< Port internal pull-down bit set, bit 9 */
#define GPIO_PDS_PDS10                       ((uint16_t)0x0400)                 /*!< Port internal pull-down bit set, bit 10 */
#define GPIO_PDS_PDS11                       ((uint16_t)0x0800)                 /*!< Port internal pull-down bit set, bit 11 */
#define GPIO_PDS_PDS12                       ((uint16_t)0x1000)                 /*!< Port internal pull-down bit set, bit 12 */
#define GPIO_PDS_PDS13                       ((uint16_t)0x2000)                 /*!< Port internal pull-down bit set, bit 13 */
#define GPIO_PDS_PDS14                       ((uint16_t)0x4000)                 /*!< Port internal pull-down bit set, bit 14 */
#define GPIO_PDS_PDS15                       ((uint16_t)0x8000)                 /*!< Port internal pull-down bit set, bit 15 */

/*******************  Bit definition for GPIO_PDC register  *******************/
#define GPIO_PDC_PDC0                        ((uint16_t)0x0001)                 /*!< Port internal pull-down bit reset, bit 0 */
#define GPIO_PDC_PDC1                        ((uint16_t)0x0002)                 /*!< Port internal pull-down bit reset, bit 1 */
#define GPIO_PDC_PDC2                        ((uint16_t)0x0004)                 /*!< Port internal pull-down bit reset, bit 2 */
#define GPIO_PDC_PDC3                        ((uint16_t)0x0008)                 /*!< Port internal pull-down bit reset, bit 3 */
#define GPIO_PDC_PDC4                        ((uint16_t)0x0010)                 /*!< Port internal pull-down bit reset, bit 4 */
#define GPIO_PDC_PDC5                        ((uint16_t)0x0020)                 /*!< Port internal pull-down bit reset, bit 5 */
#define GPIO_PDC_PDC6                        ((uint16_t)0x0040)                 /*!< Port internal pull-down bit reset, bit 6 */
#define GPIO_PDC_PDC7                        ((uint16_t)0x0080)                 /*!< Port internal pull-down bit reset, bit 7 */
#define GPIO_PDC_PDC8                        ((uint16_t)0x0100)                 /*!< Port internal pull-down bit reset, bit 8 */
#define GPIO_PDC_PDC9                        ((uint16_t)0x0200)                 /*!< Port internal pull-down bit reset, bit 9 */
#define GPIO_PDC_PDC10                       ((uint16_t)0x0400)                 /*!< Port internal pull-down bit reset, bit 10 */
#define GPIO_PDC_PDC11                       ((uint16_t)0x0800)                 /*!< Port internal pull-down bit reset, bit 11 */
#define GPIO_PDC_PDC12                       ((uint16_t)0x1000)                 /*!< Port internal pull-down bit reset, bit 12 */
#define GPIO_PDC_PDC13                       ((uint16_t)0x2000)                 /*!< Port internal pull-down bit reset, bit 13 */
#define GPIO_PDC_PDC14                       ((uint16_t)0x4000)                 /*!< Port internal pull-down bit reset, bit 14 */
#define GPIO_PDC_PDC15                       ((uint16_t)0x8000)                 /*!< Port internal pull-down bit reset, bit 15 */

/*******************  Bit definition for GPIO_ODS register  *******************/
#define GPIO_ODS_ODS0                        ((uint16_t)0x0001)                 /*!< Port open-drain bit set, bit 0 */
#define GPIO_ODS_ODS1                        ((uint16_t)0x0002)                 /*!< Port open-drain bit set, bit 1 */
#define GPIO_ODS_ODS2                        ((uint16_t)0x0004)                 /*!< Port open-drain bit set, bit 2 */
#define GPIO_ODS_ODS3                        ((uint16_t)0x0008)                 /*!< Port open-drain bit set, bit 3 */
#define GPIO_ODS_ODS4                        ((uint16_t)0x0010)                 /*!< Port open-drain bit set, bit 4 */
#define GPIO_ODS_ODS5                        ((uint16_t)0x0020)                 /*!< Port open-drain bit set, bit 5 */
#define GPIO_ODS_ODS6                        ((uint16_t)0x0040)                 /*!< Port open-drain bit set, bit 6 */
#define GPIO_ODS_ODS7                        ((uint16_t)0x0080)                 /*!< Port open-drain bit set, bit 7 */
#define GPIO_ODS_ODS8                        ((uint16_t)0x0100)                 /*!< Port open-drain bit set, bit 8 */
#define GPIO_ODS_ODS9                        ((uint16_t)0x0200)                 /*!< Port open-drain bit set, bit 9 */
#define GPIO_ODS_ODS10                       ((uint16_t)0x0400)                 /*!< Port open-drain bit set, bit 10 */
#define GPIO_ODS_ODS11                       ((uint16_t)0x0800)                 /*!< Port open-drain bit set, bit 11 */
#define GPIO_ODS_ODS12                       ((uint16_t)0x1000)                 /*!< Port open-drain bit set, bit 12 */
#define GPIO_ODS_ODS13                       ((uint16_t)0x2000)                 /*!< Port open-drain bit set, bit 13 */
#define GPIO_ODS_ODS14                       ((uint16_t)0x4000)                 /*!< Port open-drain bit set, bit 14 */
#define GPIO_ODS_ODS15                       ((uint16_t)0x8000)                 /*!< Port open-drain bit set, bit 15 */

/*******************  Bit definition for GPIO_ODC register  *******************/
#define GPIO_ODC_ODC0                        ((uint16_t)0x0001)                 /*!< Port open-drain bit reset, bit 0 */
#define GPIO_ODC_ODC1                        ((uint16_t)0x0002)                 /*!< Port open-drain bit reset, bit 1 */
#define GPIO_ODC_ODC2                        ((uint16_t)0x0004)                 /*!< Port open-drain bit reset, bit 2 */
#define GPIO_ODC_ODC3                        ((uint16_t)0x0008)                 /*!< Port open-drain bit reset, bit 3 */
#define GPIO_ODC_ODC4                        ((uint16_t)0x0010)                 /*!< Port open-drain bit reset, bit 4 */
#define GPIO_ODC_ODC5                        ((uint16_t)0x0020)                 /*!< Port open-drain bit reset, bit 5 */
#define GPIO_ODC_ODC6                        ((uint16_t)0x0040)                 /*!< Port open-drain bit reset, bit 6 */
#define GPIO_ODC_ODC7                        ((uint16_t)0x0080)                 /*!< Port open-drain bit reset, bit 7 */
#define GPIO_ODC_ODC8                        ((uint16_t)0x0100)                 /*!< Port open-drain bit reset, bit 8 */
#define GPIO_ODC_ODC9                        ((uint16_t)0x0200)                 /*!< Port open-drain bit reset, bit 9 */
#define GPIO_ODC_ODC10                       ((uint16_t)0x0400)                 /*!< Port open-drain bit reset, bit 10 */
#define GPIO_ODC_ODC11                       ((uint16_t)0x0800)                 /*!< Port open-drain bit reset, bit 11 */
#define GPIO_ODC_ODC12                       ((uint16_t)0x1000)                 /*!< Port open-drain bit reset, bit 12 */
#define GPIO_ODC_ODC13                       ((uint16_t)0x2000)                 /*!< Port open-drain bit reset, bit 13 */
#define GPIO_ODC_ODC14                       ((uint16_t)0x4000)                 /*!< Port open-drain bit reset, bit 14 */
#define GPIO_ODC_ODC15                       ((uint16_t)0x8000)                 /*!< Port open-drain bit reset, bit 15 */

/*******************  Bit definition for GPIO_ANAS register  *******************/
#define GPIO_ANAS_ANAS0                        ((uint16_t)0x0001)               /*!< Port analog function bit set, bit 0 */
#define GPIO_ANAS_ANAS1                        ((uint16_t)0x0002)               /*!< Port analog function bit set, bit 1 */
#define GPIO_ANAS_ANAS2                        ((uint16_t)0x0004)               /*!< Port analog function bit set, bit 2 */
#define GPIO_ANAS_ANAS3                        ((uint16_t)0x0008)               /*!< Port analog function bit set, bit 3 */
#define GPIO_ANAS_ANAS4                        ((uint16_t)0x0010)               /*!< Port analog function bit set, bit 4 */
#define GPIO_ANAS_ANAS5                        ((uint16_t)0x0020)               /*!< Port analog function bit set, bit 5 */
#define GPIO_ANAS_ANAS6                        ((uint16_t)0x0040)               /*!< Port analog function bit set, bit 6 */
#define GPIO_ANAS_ANAS7                        ((uint16_t)0x0080)               /*!< Port analog function bit set, bit 7 */
#define GPIO_ANAS_ANAS8                        ((uint16_t)0x0100)               /*!< Port analog function bit set, bit 8 */
#define GPIO_ANAS_ANAS9                        ((uint16_t)0x0200)               /*!< Port analog function bit set, bit 9 */
#define GPIO_ANAS_ANAS10                       ((uint16_t)0x0400)               /*!< Port analog function bit set, bit 10 */
#define GPIO_ANAS_ANAS11                       ((uint16_t)0x0800)               /*!< Port analog function bit set, bit 11 */
#define GPIO_ANAS_ANAS12                       ((uint16_t)0x1000)               /*!< Port analog function bit set, bit 12 */
#define GPIO_ANAS_ANAS13                       ((uint16_t)0x2000)               /*!< Port analog function bit set, bit 13 */
#define GPIO_ANAS_ANAS14                       ((uint16_t)0x4000)               /*!< Port analog function bit set, bit 14 */
#define GPIO_ANAS_ANAS15                       ((uint16_t)0x8000)               /*!< Port analog function bit set, bit 15 */

/*******************  Bit definition for GPIO_ANAC register  *******************/
#define GPIO_ANAC_ANAC0                        ((uint16_t)0x0001)               /*!< Port analog function bit reset, bit 0 */
#define GPIO_ANAC_ANAC1                        ((uint16_t)0x0002)               /*!< Port analog function bit reset, bit 1 */
#define GPIO_ANAC_ANAC2                        ((uint16_t)0x0004)               /*!< Port analog function bit reset, bit 2 */
#define GPIO_ANAC_ANAC3                        ((uint16_t)0x0008)               /*!< Port analog function bit reset, bit 3 */
#define GPIO_ANAC_ANAC4                        ((uint16_t)0x0010)               /*!< Port analog function bit reset, bit 4 */
#define GPIO_ANAC_ANAC5                        ((uint16_t)0x0020)               /*!< Port analog function bit reset, bit 5 */
#define GPIO_ANAC_ANAC6                        ((uint16_t)0x0040)               /*!< Port analog function bit reset, bit 6 */
#define GPIO_ANAC_ANAC7                        ((uint16_t)0x0080)               /*!< Port analog function bit reset, bit 7 */
#define GPIO_ANAC_ANAC8                        ((uint16_t)0x0100)               /*!< Port analog function bit reset, bit 8 */
#define GPIO_ANAC_ANAC9                        ((uint16_t)0x0200)               /*!< Port analog function bit reset, bit 9 */
#define GPIO_ANAC_ANAC10                       ((uint16_t)0x0400)               /*!< Port analog function bit reset, bit 10 */
#define GPIO_ANAC_ANAC11                       ((uint16_t)0x0800)               /*!< Port analog function bit reset, bit 11 */
#define GPIO_ANAC_ANAC12                       ((uint16_t)0x1000)               /*!< Port analog function bit reset, bit 12 */
#define GPIO_ANAC_ANAC13                       ((uint16_t)0x2000)               /*!< Port analog function bit reset, bit 13 */
#define GPIO_ANAC_ANAC14                       ((uint16_t)0x4000)               /*!< Port analog function bit reset, bit 14 */
#define GPIO_ANAC_ANAC15                       ((uint16_t)0x8000)               /*!< Port analog function bit reset, bit 15 */

/*******************  Bit definition for GPIO_DRS register  *******************/
#define GPIO_DRS_DRS0                        ((uint16_t)0x0001)                 /*!< Port driver halved bit set, bit 0 */
#define GPIO_DRS_DRS1                        ((uint16_t)0x0002)                 /*!< Port driver halved bit set, bit 1 */
#define GPIO_DRS_DRS2                        ((uint16_t)0x0004)                 /*!< Port driver halved bit set, bit 2 */
#define GPIO_DRS_DRS3                        ((uint16_t)0x0008)                 /*!< Port driver halved bit set, bit 3 */
#define GPIO_DRS_DRS4                        ((uint16_t)0x0010)                 /*!< Port driver halved bit set, bit 4 */
#define GPIO_DRS_DRS5                        ((uint16_t)0x0020)                 /*!< Port driver halved bit set, bit 5 */
#define GPIO_DRS_DRS6                        ((uint16_t)0x0040)                 /*!< Port driver halved bit set, bit 6 */
#define GPIO_DRS_DRS7                        ((uint16_t)0x0080)                 /*!< Port driver halved bit set, bit 7 */
#define GPIO_DRS_DRS8                        ((uint16_t)0x0100)                 /*!< Port driver halved bit set, bit 8 */
#define GPIO_DRS_DRS9                        ((uint16_t)0x0200)                 /*!< Port driver halved bit set, bit 9 */
#define GPIO_DRS_DRS10                       ((uint16_t)0x0400)                 /*!< Port driver halved bit set, bit 10 */
#define GPIO_DRS_DRS11                       ((uint16_t)0x0800)                 /*!< Port driver halved bit set, bit 11 */
#define GPIO_DRS_DRS12                       ((uint16_t)0x1000)                 /*!< Port driver halved bit set, bit 12 */
#define GPIO_DRS_DRS13                       ((uint16_t)0x2000)                 /*!< Port driver halved bit set, bit 13 */
#define GPIO_DRS_DRS14                       ((uint16_t)0x4000)                 /*!< Port driver halved bit set, bit 14 */
#define GPIO_DRS_DRS15                       ((uint16_t)0x8000)                 /*!< Port driver halved bit set, bit 15 */

/*******************  Bit definition for GPIO_DRC register  *******************/
#define GPIO_DRC_DRC0                        ((uint16_t)0x0001)                 /*!< Port driver halved bit reset, bit 0 */
#define GPIO_DRC_DRC1                        ((uint16_t)0x0002)                 /*!< Port driver halved bit reset, bit 1 */
#define GPIO_DRC_DRC2                        ((uint16_t)0x0004)                 /*!< Port driver halved bit reset, bit 2 */
#define GPIO_DRC_DRC3                        ((uint16_t)0x0008)                 /*!< Port driver halved bit reset, bit 3 */
#define GPIO_DRC_DRC4                        ((uint16_t)0x0010)                 /*!< Port driver halved bit reset, bit 4 */
#define GPIO_DRC_DRC5                        ((uint16_t)0x0020)                 /*!< Port driver halved bit reset, bit 5 */
#define GPIO_DRC_DRC6                        ((uint16_t)0x0040)                 /*!< Port driver halved bit reset, bit 6 */
#define GPIO_DRC_DRC7                        ((uint16_t)0x0080)                 /*!< Port driver halved bit reset, bit 7 */
#define GPIO_DRC_DRC8                        ((uint16_t)0x0100)                 /*!< Port driver halved bit reset, bit 8 */
#define GPIO_DRC_DRC9                        ((uint16_t)0x0200)                 /*!< Port driver halved bit reset, bit 9 */
#define GPIO_DRC_DRC10                       ((uint16_t)0x0400)                 /*!< Port driver halved bit reset, bit 10 */
#define GPIO_DRC_DRC11                       ((uint16_t)0x0800)                 /*!< Port driver halved bit reset, bit 11 */
#define GPIO_DRC_DRC12                       ((uint16_t)0x1000)                 /*!< Port driver halved bit reset, bit 12 */
#define GPIO_DRC_DRC13                       ((uint16_t)0x2000)                 /*!< Port driver halved bit reset, bit 13 */
#define GPIO_DRC_DRC14                       ((uint16_t)0x4000)                 /*!< Port driver halved bit reset, bit 14 */
#define GPIO_DRC_DRC15                       ((uint16_t)0x8000)                 /*!< Port driver halved bit reset, bit 15 */


/******************************************************************************/
/*                                                                            */
/*                      FLASH Registers                      */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for FLASH_ACR register  ******************/
#define  FLASH_ACR_START                     ((uint32_t)0x00000001)        /*!< 启动FLASH操作 */
#define  FLASH_ACR_CMD                       ((uint32_t)0x0000000E)        /*!< FLASH操作命令 */
#define  FLASH_ACR_PERA                      ((uint32_t)0x00000002)        /*!< FLASH操作命令页擦除 */
#define  FLASH_ACR_MERA                      ((uint32_t)0x00000004)        /*!< FLASH操作命令主代码区擦除 */
#define  FLASH_ACR_LATENCY                   ((uint32_t)0x000000E0)        /*!< FLASH访问潜伏期 */
#define  FLASH_ACR_LATENCY_0                 ((uint32_t)0x00000000)        /*!< FLASH访问潜伏期_0 */
#define  FLASH_ACR_LATENCY_1                 ((uint32_t)0x00000020)        /*!< FLASH访问潜伏期_1 */
#define  FLASH_ACR_LATENCY_2                 ((uint32_t)0x00000040)        /*!< FLASH访问潜伏期_2 */
#define  FLASH_ACR_LATENCY_3                 ((uint32_t)0x00000060)        /*!< FLASH访问潜伏期_3 */
#define  FLASH_ACR_LATENCY_4                 ((uint32_t)0x00000080)        /*!< FLASH访问潜伏期_4 */
#define  FLASH_ACR_LATENCY_5                 ((uint32_t)0x000000A0)        /*!< FLASH访问潜伏期_5 */
#define  FLASH_ACR_LATENCY_6                 ((uint32_t)0x000000C0)        /*!< FLASH访问潜伏期_6 */
#define  FLASH_ACR_LATENCY_7                 ((uint32_t)0x000000E0)        /*!< FLASH访问潜伏期_7 */
#define  FLASH_ACR_CACHE_EN                  ((uint32_t)0x00000010)        /*!< FLASH Cache使能 */
#define  FLASH_ACR_MODE                      ((uint32_t)0x00000100)        /*!< FLASH读写访问方式 */
#define  FLASH_ACR_DPSTB                     ((uint32_t)0x00000800)        /*!< FLASH深度休眠使能 */
#define  FLASH_ACR_AINC                      ((uint32_t)0x00002000)        /*!< FLASH地址自动递增 */
#define  FLASH_FKEY1                         ((uint32_t)0xA5A50000)        /*!< FLASH主代码区擦写密码*/


/*******************  Bit definition for FLASH_ISR register  ******************/

#define  FLASH_ISR_WREND                     ((uint32_t)0x00000001)        /*!< 写命令完成 */
#define  FLASH_ISR_CMDER                     ((uint32_t)0x00000002)        /*!< FLASH操作命令错误标志 */
#define  FLASH_ISR_ACCER                     ((uint32_t)0x00000004)        /*!< FLASH存取错误标志*/
#define  FLASH_ISR_ADER                      ((uint32_t)0x00000008)        /*!< FLASH地址错误标志 */
#define  FLASH_ISR_SEC                       ((uint32_t)0x00000010)        /*!< FLASH 1个数据位错误被纠正标志位 */
#define  FLASH_ISR_DEC                       ((uint32_t)0x00000020)        /*!< FLASH 2个数据位错误被纠正标志位 */
#define  FLASH_ISR_CACHE_HIT                 ((uint32_t)0x00000040)        /*!< Flash 访问总次数计数饱和标志 */


/*******************  Bit definition for FLASH_IER register  ******************/

#define  FLASH_IER_WREND                     ((uint32_t)0x00000001)        /*!< 写命令完成中断使能 */
#define  FLASH_IER_CMDER                     ((uint32_t)0x00000002)        /*!< FLASH操作命令错误中断使能 */
#define  FLASH_IER_ADER                      ((uint32_t)0x00000008)        /*!< FLASH地址错误中断使能 */
#define  FLASH_IER_CACHE_HIT                 ((uint32_t)0x00000040)        /*!< Flash 访问总次数计数饱和中断使能 */


/*******************  Bit definition for FLASH_AR register  ******************/

/*******************  Bit definition for FLASH_AR register  *******************/
#define  FLASH_AR_FAR                        ((uint32_t)0x0027FFFC)        /*!< Flash Address */

/*******************  Bit definition for FLASH_DR0 register  *******************/
#define  FLASH_DR0_FDR                        ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/*******************  Bit definition for FLASH_DR0 register  *******************/
#define  FLASH_DR1_FDR                        ((uint32_t)0xFFFFFFFF)        /*!< Flash Address */

/*******************  Bit definition for FLASH_DIV register  *******************/
#define  FLASH_DIV_FDIV                      ((uint32_t)0x0000000F)        /*!< Flash Address */

/******************************************************************************/
/*                                                                            */
/*      Universal Asynchronous Receiver Transmitter (UART)       */
/*                                                                            */
/******************************************************************************/
/******************  Bit definition for UART_BR register  *******************/
#define  UART_BR_BR                        ((uint16_t)0x01FF)               /*!< BR[8:0] bits (Receiver Transmitter Buffer value) */

/******************  Bit definition for UART_CR register  *******************/
#define  UART_CR_MODE                      ((uint32_t)0x00000007)           /*!< MODE[2:0] bits (工作模式选择) */
#define  UART_CR_STOP                      ((uint32_t)0x00000018)           /*!< STOP[1:0] bits (停止位长度选择) */
#define  UART_CR_PBR                       ((uint32_t)0x00000020)           /*!< 奇偶校验方式 */
#define  UART_CR_LPB                       ((uint32_t)0x00000040)           /*!< 回绕模式控制 */
#define  UART_CR_BRUN                      ((uint32_t)0x00000080)           /*!< 波特率发生器使能 */
#define  UART_CR_RXEN                      ((uint32_t)0x00000100)           /*!< 数据接收使能 */
#define  UART_CR_RXPOL                     ((uint32_t)0x00010000)           /*!< 数据接收极性控制 */
#define  UART_CR_TXPOL                     ((uint32_t)0x00020000)           /*!< 数据发送极性控制 */
#define  UART_CR_OWE                       ((uint32_t)0x00200000)           /*!< OneWire模式使能 */
#define  UART_CR_OWD                       ((uint32_t)0x00400000)           /*!< OneWire模式收发方向控制 */

/******************  Bit definition for UART_BRR register  *******************/
#define  UART_BRR_BRR                      ((uint32_t)0x0000FFFF)               /*!< BRR[15:0] bits (波特率配置) */
#define  UART_BRR_SR                       ((uint32_t)0x00030000)               /*!< 采样率配置 */
#define  UART_BRR_SRX16                    ((uint32_t)0x00000000)               /*!< 采样率配置 */
#define  UART_BRR_SRX8                     ((uint32_t)0x00020000)               /*!< 采样率配置 */
#define  UART_BRR_SRX4                     ((uint32_t)0x00010000)               /*!< 采样率配置 */

/******************  Bit definition for UART_IER register  *******************/
//以下是原厂定义，似乎是错的
//#define  UART_IER_RXNEIE                   ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空中断允许控制位 */
//#define  UART_IER_RXHFIE                   ((uint32_t)0x00000002)        /*!< 接收缓冲队列半满中断允许控制位 */
//#define  UART_IER_RXFIE                    ((uint32_t)0x00000004)        /*!< 接收缓冲队列全满中断允许控制位 */
//#define  UART_IER_TONEIE                   ((uint32_t)0x00000008)        /*!< 接收缓冲队列清空超时中断允许控制位 */
//#define  UART_IER_TOIDLEIE                 ((uint32_t)0x00000010)        /*!< 空闲超时中断允许控制位 */
//#define  UART_IER_PERRIE                   ((uint32_t)0x00000020)        /*!< 奇偶校验错误中断允许控制位 */
//#define  UART_IER_FERRIE                   ((uint32_t)0x00000040)        /*!< 帧错误中断允许控制位 */
//#define  UART_IER_OVERRIE                  ((uint32_t)0x00000080)        /*!< 接收缓冲队列溢出中断允许控制位 */
//#define  UART_IER_TXEIE                    ((uint32_t)0x00000100)        /*!< 发送缓冲队列全空中断允许控制位 */
//#define  UART_IER_TXHEIE                   ((uint32_t)0x00000200)        /*!< 发送缓冲队列半空中断允许控制位 */
//#define  UART_IER_TXFIE                    ((uint32_t)0x00000400)        /*!< 发送缓冲队列全满中断允许控制位 */
//#define  UART_IER_TXENDIE                  ((uint32_t)0x00000800)        /*!< 发送全部完成中断允许控制位 */
//改正之
#define  UART_IER_RXNEIE                   ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空中断允许控制位 */
#define  UART_IER_TXEIE                    ((uint32_t)0x00000002)        /*!< 发送缓冲队列全空中断允许控制位 */
#define  UART_IER_TXHEIE                   ((uint32_t)0x00000004)        /*!< 发送缓冲队列半空中断允许控制位 */
#define  UART_IER_PERRIE                   ((uint32_t)0x00000008)        /*!< 奇偶校验错误中断允许控制位 */
#define  UART_IER_FERRIE                   ((uint32_t)0x00000010)        /*!< 帧错误中断允许控制位 */
#define  UART_IER_OVERRIE                  ((uint32_t)0x00000020)        /*!< 接收缓冲队列溢出中断允许控制位 */
#define  UART_IER_TONEIE                   ((uint32_t)0x00000040)        /*!< 接收缓冲队列清空超时中断允许控制位 */
#define  UART_IER_TOIDLEIE                 ((uint32_t)0x00000080)        /*!< 空闲超时中断允许控制位 */
#define  UART_IER_RXHFIE                   ((uint32_t)0x00000100)        /*!< 接收缓冲队列半满中断允许控制位 */
#define  UART_IER_RXFIE                    ((uint32_t)0x00000200)        /*!< 接收缓冲队列全满中断允许控制位 */
#define  UART_IER_TXENDIE                  ((uint32_t)0x00000400)        /*!< 发送全部完成中断允许控制位 */
#define  UART_IER_TXFIE                    ((uint32_t)0x00000800)        /*!< 发送队列全满中断允许控制位 */

/******************  Bit definition for UART_IER register  *******************/
//#define  UART_IER_RXNEIE                   ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空中断标识位 */
//#define  UART_IER_RXHFIE                   ((uint32_t)0x00000002)        /*!< 接收缓冲队列半满中断标识位 */
//#define  UART_IER_RXFIE                    ((uint32_t)0x00000004)        /*!< 接收缓冲队列全满中断标识位 */
//#define  UART_IER_TONEIE                   ((uint32_t)0x00000008)        /*!< 接收缓冲队列清空超时中断标识位 */
//#define  UART_IER_TOIDLEIE                 ((uint32_t)0x00000010)        /*!< 空闲超时中断标识位 */
//#define  UART_IER_PERRIE                   ((uint32_t)0x00000020)        /*!< 奇偶校验错误中断标识位 */
//#define  UART_IER_FERRIE                   ((uint32_t)0x00000040)        /*!< 帧错误中断标识位 */
//#define  UART_IER_OVERRIE                  ((uint32_t)0x00000080)        /*!< 接收缓冲队列溢出中断标识位 */
//#define  UART_IER_TXEIE                    ((uint32_t)0x00000100)        /*!< 发送缓冲队列全空中断标识位 */
//#define  UART_IER_TXHEIE                   ((uint32_t)0x00000200)        /*!< 发送缓冲队列半空中断标识位 */
//#define  UART_IER_TXFIE                    ((uint32_t)0x00000400)        /*!< 发送缓冲队列全满中断标识位 */
//#define  UART_IER_TXENDIE                  ((uint32_t)0x00000800)        /*!< 发送全部完成中断标识位 */

/******************  Bit definition for UART_ISR register  *******************/
#define  UART_ISR_RXNE                     ((uint32_t)0x00000001)        /*!< 接收缓冲队列非空标志位 */
#define  UART_ISR_TXE                      ((uint32_t)0x00000002)        /*!< 发送缓冲队列全空标志位 */
#define  UART_ISR_TXHE                     ((uint32_t)0x00000004)        /*!< 发送缓冲队列半空标志位 */
#define  UART_ISR_PERR                     ((uint32_t)0x00000008)        /*!< 奇偶校验错误标志位 */
#define  UART_ISR_FERR                     ((uint32_t)0x00000010)        /*!< 帧错误标志位 */
#define  UART_ISR_OVERR                    ((uint32_t)0x00000020)        /*!< 接收缓冲队列溢出标志位 */
#define  UART_ISR_TONE                     ((uint32_t)0x00000040)        /*!< 接收缓冲队列清空超时标志位 */
#define  UART_ISR_TOIDLE                   ((uint32_t)0x00000080)        /*!< 空闲超时标志位 */
#define  UART_ISR_RXHF                     ((uint32_t)0x00000100)        /*!< 接收缓冲队列半满标志位 */
#define  UART_ISR_RXF                      ((uint32_t)0x00000200)        /*!< 接收缓冲队列全满标志位 */
#define  UART_ISR_TXEND                    ((uint32_t)0x00000400)        /*!< 发送全部完成标志位 */
#define  UART_ISR_TXF                      ((uint32_t)0x00000800)        /*!< 发送队列全满标志位 */

/******************  Bit definition for UART_GTR register  *******************/
#define  UART_GTR_GTR                      ((uint8_t)0xFF)               /*!< GTR[8:0] bits (发送两个连续数据帧的间隔时间，单位为波特率) */

/******************  Bit definition for UART_TO register  *******************/
#define  UART_TO_TO                        ((uint8_t)0xFF)               /*!< TO[8:0] bits (接收超时计时寄存器，单位为波特率) */

/******************  Bit definition for UART_TXFR register  *******************/
#define  UART_TXFR_TXFR                    ((uint32_t)0xFFFFFFFF)        /*!< TXFR[31:0] bits (发送队列复位) */

/******************  Bit definition for UART_RXFR register  *******************/
#define  UART_RXFR_RXFR                    ((uint32_t)0xFFFFFFFF)        /*!< RXFR[31:0] bits (接收队列复位) */

/******************  Bit definition for UART_ISO7816CR register  *******************/
#define  UART_ISO7816CR_EN                 ((uint32_t)0x00000001)        /*!< ISO7816功能使能 */
#define  UART_ISO7816CR_DIR                ((uint32_t)0x00000002)        /*!< ISO7816数据传输方向控制 */
#define  UART_ISO7816CR_CLKDIV             ((uint32_t)0x00000070)        /*!< ISO7816时钟频率选择 */

/******************  Bit definition for UART_IRR register  *******************/
#define  UART_IRR_EN                       ((uint32_t)0x00000001)        /*!<红外功能使能控制 */
#define  UART_IRR_TXPOL                    ((uint32_t)0x00000002)        /*!<红外发送极性控制 */
#define  UART_IRR_SPD                      ((uint32_t)0x00000030)        /*!<SPD[1:0]红外速率选择控制 */
#define  UART_IRR_RST                      ((uint32_t)0x00000800)        /*!<红外调制软复位 */
#define  UART_IRR_SSBWITH                  ((uint32_t)0x00000020)        /*!<SSBWITH[3:0] 红外解调起始位宽度缩减 */


/******************  Bit definition for UART_IRPWMC register  *******************/
#define  UART_IRPWMC_IRPWMC                ((uint32_t)0x00000FFF)        /*!< PWMC[11:0] bits (红外调制占空比控制) */

/******************************************************************************/
/*                         Peripheral Registers Bits Definition               */
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/*                      Analog to Digital Converter (ADC)                     */
/*                                                                            */
/******************************************************************************/

/********************  Bits definition for ADC_CR0 register  ******************/
#define ADC_CR0_ADEN                         ((uint32_t)0x00000001)       /*!< ADC enable control */
#define ADC_CR0_SOC                          ((uint32_t)0x00000002)       /*!< ADC enable control */
#define ADC_CR0_RESO                         ((uint32_t)0x0000000C)       /*!< Continuous Conversion */
#define ADC_CR0_SAIN                         ((uint32_t)0x000001F0)       /*!< SOCSEL[3:2] bits(ADC trigger source selection) */
#define ADC_CR0_DISH                         ((uint32_t)0x00001000)       /*!< Bit 0 */
#define ADC_CR0_GCMP                         ((uint32_t)0x00002000)       /*!< Bit 1 */
#define ADC_CR0_OFFSET                       ((uint32_t)0x003F0000)

/********************  Bits definition for ADC_CR1 register  ******************/
#define ADC_CR1_DMAEN                        ((uint32_t)0x00000001)       /*!< ADC enable control */
#define ADC_CR1_MODE                         ((uint32_t)0x00000006)       /*!< ADC enable control */
#define ADC_CR1_RSTN                         ((uint32_t)0x000000F0)       /*!< Continuous Conversion */
#define ADC_CR1_CLKDIV                       ((uint32_t)0x0000FF00)       /*!< SOCSEL[3:2] bits(ADC trigger source selection) */

/********************  Bits definition for ADC_SR register  ******************/
#define ADC_SR_EOC                           ((uint32_t)0x00000001)       /*!< ADC End of Conversion Flag */

/********************  Bit definition for ADC_DR register  ********************/
#define  ADC_DR_DATA                         ((uint32_t)0x0000FFFF)        /*!< Regular data */

/********************  Bits definition for ADC_IER register  ******************/
#define ADC_IER_EOC                          ((uint32_t)0x00000001)       /*!< ADC End of Conversion Interrupt*/

/********************  Bits definition for ADC_CHER register  ******************/
#define ADC_CHER_CHSEL                       ((uint32_t)0x0000003F)        /*!< ADC输入通道使能控制 */
#define ADC_CHER_CHSEL_0                     ((uint32_t)0x00000001)        /*!< ADC输入通道0使能    */
#define ADC_CHER_CHSEL_1                     ((uint32_t)0x00000002)        /*!< ADC输入通道1使能    */
#define ADC_CHER_CHSEL_2                     ((uint32_t)0x00000004)        /*!< ADC输入通道2使能    */
#define ADC_CHER_CHSEL_3                     ((uint32_t)0x00000008)        /*!< ADC输入通道3使能    */
#define ADC_CHER_CHSEL_4                     ((uint32_t)0x00000010)        /*!< ADC输入通道4使能    */
#define ADC_CHER_CHSEL_5                     ((uint32_t)0x00000020)        /*!< ADC输入通道5使能    */


/******************************************************************************/
/*                                                                            */
/*                   Inter-integrated Circuit Interface (I2C)                 */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for I2C_CSR register  *******************/
#define  I2C_CSR_CR0                         ((uint32_t)0x00000001)        /*!< System clock division factor bit0  */
#define  I2C_CSR_CR1                         ((uint32_t)0x00000002)        /*!< System clock division factor bit1  */
#define  I2C_CSR_CR2                         ((uint32_t)0x00000080)        /*!< System clock division factor bit2  */
#define  I2C_CSR_ACK                         ((uint32_t)0x00000004)        /*!< ACK generation (slave mode) */
#define  I2C_CSR_IS                          ((uint32_t)0x00000008)        /*!< Interrupt status */
#define  I2C_CSR_STOP                        ((uint32_t)0x00000010)        /*!< STOP generation (master mode) */
#define  I2C_CSR_START                       ((uint32_t)0x00000020)        /*!< START generation */
#define  I2C_CSR_EN                          ((uint32_t)0x00000040)        /*!< Peripheral enable */

/*******************  Bit definition for I2C_DR register  *******************/

#define  I2C_DR_DATA                         ((uint32_t)0x000000F8)        /*!< Enable Broadcast addressing  */

/*******************  Bit definition for I2C_OAR register  *******************/
#define  I2C_OAR_BCSTEN                      ((uint32_t)0x00000001)        /*!< Enable Broadcast addressing  */
#define  I2C_OAR_ADDR                        ((uint32_t)0x000000FE)        /*!< Enable Broadcast addressing  */

/*******************  Bit definition for I2C_CCR register  *******************/
#define  I2C_CCR_MASK                        ((uint32_t)0x000000FF)        /*!<  */
#define  I2C_CCR_CR0                         ((uint32_t)0x00000001)        /*!< System clock division factor bit0  */
#define  I2C_CCR_CR1                         ((uint32_t)0x00000002)        /*!< System clock division factor bit1  */
#define  I2C_CCR_CR2                         ((uint32_t)0x00000080)        /*!< System clock division factor bit2  */
#define  I2C_CCR_CR                          ((uint32_t)0x00000083)        /*!< System clock division factor bit2  */
#define  I2C_CCR_ACK                         ((uint32_t)0x00000004)        /*!< ACK generation (slave mode) */
#define  I2C_CCR_IS                          ((uint32_t)0x00000008)        /*!< Interrupt status */
#define  I2C_CCR_STOP                        ((uint32_t)0x00000010)        /*!< STOP generation (master mode) */
#define  I2C_CCR_START                       ((uint32_t)0x00000020)        /*!< START generation */
#define  I2C_CCR_EN                          ((uint32_t)0x00000040)        /*!< Peripheral enable */


/******************************************************************************/
/*                                                                            */
/*                        Serial Peripheral Interface (SPI)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for SPI_CR0 register  ********************/
#define  SPI_CR0_DSS                         ((uint32_t)0x0000000F)         /*!< SPI_data_size */
#define  SPI_CR0_DSS_4B                      ((uint32_t)0x00000003)         /*!< Data frame length is 4 bits  */
#define  SPI_CR0_DSS_5B                      ((uint32_t)0x00000004)         /*!< Data frame length is 5 bits  */
#define  SPI_CR0_DSS_6B                      ((uint32_t)0x00000005)         /*!< Data frame length is 6 bits  */
#define  SPI_CR0_DSS_7B                      ((uint32_t)0x00000006)         /*!< Data frame length is 7 bits  */
#define  SPI_CR0_DSS_8B                      ((uint32_t)0x00000007)         /*!< Data frame length is 8 bits  */
#define  SPI_CR0_DSS_9B                      ((uint32_t)0x00000008)         /*!< Data frame length is 9 bits  */
#define  SPI_CR0_DSS_10B                     ((uint32_t)0x00000009)         /*!< Data frame length is 10 bits  */
#define  SPI_CR0_DSS_11B                     ((uint32_t)0x0000000A)         /*!< Data frame length is 11 bits  */
#define  SPI_CR0_DSS_12B                     ((uint32_t)0x0000000B)         /*!< Data frame length is 12 bits  */
#define  SPI_CR0_DSS_13B                     ((uint32_t)0x0000000C)         /*!< Data frame length is 13 bits  */
#define  SPI_CR0_DSS_14B                     ((uint32_t)0x0000000D)         /*!< Data frame length is 14 bits  */
#define  SPI_CR0_DSS_15B                     ((uint32_t)0x0000000E)         /*!< Data frame length is 15 bits  */
#define  SPI_CR0_DSS_16B                     ((uint32_t)0x0000000F)         /*!< Data frame length is 16 bits  */
#define  SPI_CR0_FRF                         ((uint32_t)0x00000030)         /*!< SPI_data frame format */
#define  SPI_CR0_FRF_SPI                     ((uint32_t)0x00000000)         /*!< SPI_data SPI Motorola frame format */
#define  SPI_CR0_FRF_TI                      ((uint32_t)0x00000010)         /*!< SPI_data_TI frame format */
#define  SPI_CR0_FRF_MICROWIRE               ((uint32_t)0x00000020)         /*!< SPI_data Microwire frame format */
#define  SPI_CR0_CPOL                        ((uint32_t)0x00000040)         /*!< Clock Polarity */
#define  SPI_CR0_CPHA                        ((uint32_t)0x00000080)         /*!< Clock Phase */
#define  SPI_CR0_SCR                         ((uint32_t)0x0000FF00)         /*!< Post-division factor */
/*******************  Bit definition for SPI_CR1 register  ********************/
#define  SPI_CR1_LOOPBACK                    ((uint32_t)0x00000001)         /*!< Loopback Mode */
#define  SPI_CR1_SPIEN                       ((uint32_t)0x00000002)         /*!< Loopback Mode */
#define  SPI_CR1_MSTR                        ((uint32_t)0x00000004)         /*!< Master-slave selection */
#define  SPI_CR1_SOD                         ((uint32_t)0x00000004)         /*!< Slave output disable */

/********************  Bit definition for SPI_DR register  ********************/
#define  SPI_DR_DR                           ((uint16_t)0xFFFF)            /*!< Data Register */

/********************  Bit definition for SPI_SR register  ********************/
#define  SPI_SR_TFE                          ((uint32_t)0x00000001)         /*!< Transmission FIFO is empty */
#define  SPI_SR_TFNF                         ((uint32_t)0x00000002)         /*!< Transmission FIFO is not empty */
#define  SPI_SR_RFNE                         ((uint32_t)0x00000004)         /*!< Receiver FIFO is not empty */
#define  SPI_SR_RFF                          ((uint32_t)0x00000008)         /*!< Receiver FIFO is Full */
#define  SPI_SR_BUSY                         ((uint32_t)0x00000010)         /*!< Transmission module is busy */

/********************  Bit definition for SPI_CPSR register  ********************/
#define  SPI_CPSR_CPSR                       ((uint32_t)0x000000FF)         /*!< Prescaler factor */

/********************  Bit definition for SPI_IE register  ********************/
#define  SPI_IE_RFOF                        ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_IE_RFNEOT                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_IE_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_IE_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_RIS register  ********************/
#define  SPI_RIS_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_RIS_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_RIS_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_RIS_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_EIS register  ********************/
#define  SPI_EIS_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_EIS_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */
#define  SPI_EIS_RFHF                        ((uint32_t)0x00000004)         /*!< Receiver FIFO Half-full */
#define  SPI_EIS_TFHF                        ((uint32_t)0x00000008)         /*!< Transmission FIFO Half-full */

/********************  Bit definition for SPI_ICR register  ********************/
#define  SPI_ICR_RFOVF                       ((uint32_t)0x00000001)         /*!< Receiver FIFO overflow */
#define  SPI_ICR_RFNETO                      ((uint32_t)0x00000002)         /*!< Receiver FIFO Non-empty timeout */

/********************  Bit definition for SPI_CSCR register  ********************/
#define  SPI_ICR_SWSEL                       ((uint32_t)0x00000004)         /*!< CS is controlled by software  */
#define  SPI_ICR_SWCS                        ((uint32_t)0x00000008)         /*!< CS Set Bit by software  */

/******************************************************************************/
/*                                                                            */
/*                        7SEG LED Display Interface (LED)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for LED_CR register  ********************/
#define LED_CR_EN                            ((uint32_t)0x00000001)          /*!< Peripheral enable */
#define LED_CR_COMSEL                        ((uint32_t)0x00000006)          /*!< Select Number of LED displayed in turn  */
#define LED_CR_COMSEL_1                      ((uint32_t)0x00000000)          /*!< Select Number of LED displayed in turn  */
#define LED_CR_COMSEL_2                      ((uint32_t)0x00000002)          /*!< Select Number of LED displayed in turn  */
#define LED_CR_COMSEL_3                      ((uint32_t)0x00000004)          /*!< Select Number of LED displayed in turn  */
#define LED_CR_COMSEL_4                      ((uint32_t)0x00000006)          /*!< Select Number of LED displayed in turn  */
#define LED_CR_IE                            ((uint32_t)0x00000001)          /*!< Display completion interrupt */

/*******************  Bit definition for LED_CSPR register  ********************/
#define LED_CSPR_PR                          ((uint32_t)0x000000FF)          /*!< CLOCK Prescaler factor */

/*******************  Bit definition for LED_DR0 register  ********************/
#define LED_CSPR_DR0                         ((uint32_t)0x000000FF)          /*!< Data0 Register */

/*******************  Bit definition for LED_DR1 register  ********************/
#define LED_CSPR_DR1                         ((uint32_t)0x000000FF)          /*!< Data1 Register */

/*******************  Bit definition for LED_DR2 register  ********************/
#define LED_CSPR_DR2                         ((uint32_t)0x000000FF)          /*!< Data2 Register */

/*******************  Bit definition for LED_DR3 register  ********************/
#define LED_CSPR_DR3                         ((uint32_t)0x000000FF)          /*!< Data3 Register */

/*******************  Bit definition for LED_TR register  ********************/
#define LED_TR_DISPLAYT                      ((uint32_t)0x000000FF)          /*!< LED Display Time */
#define LED_TR_TURNT                         ((uint32_t)0x0000FF00)          /*!< LED Turn Time */

/*******************  Bit definition for LED_EIS register  ********************/
#define LED_EIS_DISPLAYEND                   ((uint32_t)0x00000001)          /*!< LED Display Flnish */


/******************************************************************************/
/*                                                                            */
/*                        7SEG LED Display Interface (LED)                   */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for CMPOPA_CR register  ********************/
#define CMPOPA_CR1_EN                        ((uint32_t)0x00000001)          /*!< Comparator /Op Amp Enable */
#define CMPOPA_CR1_OUTPUTPOS                 ((uint32_t)0x00000002)          /*!< Comparator Output when Positive input is greater than negative input */
#define CMPOPA_CR1_REFSEL                    ((uint32_t)0x0000000C)          /*!< Comparator Positive input source Select */
#define CMPOPA_CR1_REFSEL_PIN                ((uint32_t)0x00000000)          /*!< Comparator Positive input From PIN */
#define CMPOPA_CR1_REFSEL_DAC                ((uint32_t)0x00000004)          /*!< Comparator Positive input From DAC */
#define CMPOPA_CR1_REFSEL_BG                 ((uint32_t)0x00000008)          /*!< Comparator Positive input From BandGap Voltage */
#define CMPOPA_CR1_FS                        ((uint32_t)0x00000070)          /*!< Comparator Digital filter sampling rate Set */
#define CMPOPA_CR1_FS_NONE                   ((uint32_t)0x00000000)          /*!< Comparator Turn off the digital filter */
#define CMPOPA_CR1_FS_X2                     ((uint32_t)0x00000010)          /*!< Comparator Digital filter 2 times sampling */
#define CMPOPA_CR1_FS_X4                     ((uint32_t)0x00000020)          /*!< Comparator Digital filter 4 times sampling*/
#define CMPOPA_CR1_FS_X8                     ((uint32_t)0x00000030)          /*!< Comparator Digital filter 8 times sampling */
#define CMPOPA_CR1_FS_X16                    ((uint32_t)0x00000040)          /*!< Comparator Digital filter 16 times sampling */
#define CMPOPA_CR1_FS_X32                    ((uint32_t)0x00000050)          /*!< Comparator Digital filter 32 times sampling */
#define CMPOPA_CR1_FS_X64                    ((uint32_t)0x00000060)          /*!< Comparator Digital filter 64 times sampling */
#define CMPOPA_CR1_HSMODE                    ((uint32_t)0x00000080)          /*!< Comparator High speed mode */
#define CMPOPA_CR1_OUTPUTPOL                 ((uint32_t)0x00000100)          /*!< Comparator Output polarity selection */

/*******************  Bit definition for CMPOPA_CR register  ********************/
#define CMPOPA_CR2_OPAEN                     ((uint32_t)0x00000001)          /*!< Enable Operational amplifier mode */

/*******************  Bit definition for CMPOPA_DAC register  ********************/
#define CMPOPA_DAC_DATA                      ((uint32_t)0x0000001F)          /*!< Comparator DAC input data register*/

/********************  Bit definition for CMPOPA_IE register  ********************/
#define  CMPOPA_IE_FALL                       ((uint32_t)0x00000001)         /*!< Comparator output falling edge trigger interrupt enable */
#define  CMPOPA_IE_RISE                       ((uint32_t)0x00000002)         /*!< Comparator output rising edge trigger interrupt enable */
#define  CMPOPA_IE_FALL_WEAK                  ((uint32_t)0x00000004)         /*!< Comparator output falling edge trigger interrupt enable */
#define  CMPOPA_IE_RISE_WEAK                  ((uint32_t)0x00000008)         /*!< Comparator output rising edge trigger interrupt enable */

/********************  Bit definition for CMPOPA_EIS register  ********************/
#define  CMPOPA_EIS_FALL                       ((uint32_t)0x00000001)         /*!< Comparator output falling edge trigger interrupt enable */
#define  CMPOPA_EIS_RISE                       ((uint32_t)0x00000002)         /*!< Comparator output rising edge trigger interrupt enable */
#define  CMPOPA_EIS_FALL_WEAK                  ((uint32_t)0x00000004)         /*!< Comparator output falling edge trigger interrupt enable */
#define  CMPOPA_EIS_RISE_WEAK                  ((uint32_t)0x00000008)         /*!< Comparator output rising edge trigger interrupt enable */

/********************  Bit definition for CMPOPA_INITDELAY register  ********************/
#define  CMPOPA_INITDELAY                     ((uint32_t)0x000003FF)

/******************************************************************************/
/*                                                                            */
/*                      TIM Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for TIM_ISR register  *******************/
#define  TIM_ISR_OV                        ((uint32_t)0x00000001)       /*!<定时器与MR0值匹配中断标志位       */

/*******************  Bit definition for TIM_TCR register  *******************/
#define  TIM_TCR_EN                        ((uint32_t)0x00000001)       /*!<定时器使能控制位        */
#define  TIM_TCR_UPDATE                    ((uint32_t)0x00000002)       /*!<定时器匹配值更新        */

/*******************  Bit definition for TIM_PR register  *******************/
#define  TIM_PR                            ((uint8_t)0xFF)               /*!<PR[7:0]定时器预分频系数   */

/*******************  Bit definition for TIM_PC register  *******************/
#define  TIM_PC                            ((uint8_t)0xFF)               /*!<PC[15:0]定时器预分频计数当前值   */

/*******************  Bit definition for TIM_TC register  *******************/
#define  TIM_TC                            ((uint16_t)0xFFFF)            /*!<PC[15:0]定时器当前计数值   */

/*******************  Bit definition for TIM_MCR register  *******************/
#define  TIM_MCR_MR0I                      ((uint32_t)0x00000001)        /*!<TIM_TC与MR0值匹配时产生中断控制位          */
#define  TIM_MCR_MR0R                      ((uint32_t)0x00000002)        /*!<TIM_TC与MR0值匹配时计数复位控制位          */
#define  TIM_MCR_MR0S                      ((uint32_t)0x00000004)        /*!<TIM_TC与MR0值匹配时计数器停止控制位        */
#define  TIM_MCR_DIR                       ((uint32_t)0x00000008)       /*!<TIM_TC计数方向控制位                       */


/*******************  Bit definition for PWM_MR0 register  *******************/
#define  TIM_MR0                           ((uint16_t)0xFFFF)            /*!<MR0[15:0]定时器MR0匹配值   */



/******************************************************************************/
/*                                                                            */
/*                      PWM Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for PWM_ISR register  *******************/
#define  PWM_ISR_MR0                       ((uint32_t)0x00000001)       /*!<定时器匹配0标志位       */
#define  PWM_ISR_MR1                       ((uint32_t)0x00000002)       /*!<定时器匹配1标志位       */
#define  PWM_ISR_MR2                       ((uint32_t)0x00000004)       /*!<定时器匹配2标志位       */
#define  PWM_ISR_MR3                       ((uint32_t)0x00000006)       /*!<定时器匹配3标志位       */
#define  PWM_ISR_MR4                       ((uint32_t)0x00000010)       /*!<定时器匹配4标志位       */
#define  PWM_ISR_CR1_R                     ((uint32_t)0x00000020)       /*!<定时器捕获1上升沿标志位 */
#define  PWM_ISR_CR1_F                     ((uint32_t)0x00000040)       /*!<定时器匹配1下降沿标志位 */
#define  PWM_ISR_CR2_R                     ((uint32_t)0x00000080)       /*!<定时器匹配2上升沿标志位 */
#define  PWM_ISR_CR2_F                     ((uint32_t)0x00000100)       /*!<定时器匹配2下降沿标志位 */
#define  PWM_ISR_CR3_R                     ((uint32_t)0x00000200)       /*!<定时器匹配3上升沿标志位 */
#define  PWM_ISR_CR3_F                     ((uint32_t)0x00000400)       /*!<定时器匹配3下降沿标志位 */
#define  PWM_ISR_CR4_R                     ((uint32_t)0x00000800)       /*!<定时器匹配4上升沿标志位 */
#define  PWM_ISR_CR4_F                     ((uint32_t)0x00001000)       /*!<定时器匹配4下降沿标志位 */
#define  PWM_ISR_BKI                       ((uint32_t)0x00002000)       /*!<定时器刹车输入标志位    */

/*******************  Bit definition for PWM_TCR register  *******************/
#define  PWM_TCR_EN                        ((uint32_t)0x00000001)       /*!<定时器使能控制位        */
#define  PWM_TCR_UPDATE                    ((uint32_t)0x00000002)       /*!<定时器匹配值更新        */
#define  PWM_TCR_CLKSEL                    ((uint32_t)0x00000004)       /*!<定时器时钟选择控制      */

/*******************  Bit definition for PWM_INT_RPT register  *******************/
#define  PWM_INT_RPT                       ((uint8_t)0xF)               /*!<INT_RPT[3:0]定时器中断累计次数控制位   */

/*******************  Bit definition for PWM_INT_RPTC register  *******************/
#define  PWM_INT_RPTC                      ((uint8_t)0xF)               /*!<INT_RPTC[3:0]定时器中断累计次数当前计数值   */

/*******************  Bit definition for PWM_PR register  *******************/
#define  PWM_PR                            ((uint8_t)0xFF)               /*!<PR[7:0]定时器预分频系数   */

/*******************  Bit definition for PWM_PC register  *******************/
#define  PWM_PC                            ((uint8_t)0xFF)               /*!<PC[15:0]定时器预分频计数当前值   */

/*******************  Bit definition for PWM_TC register  *******************/
#define  PWM_TC                            ((uint16_t)0xFFFF)            /*!<PC[15:0]定时器当前计数值   */

/*******************  Bit definition for PWM_MCR register  *******************/
#define  PWM_MCR_MR0I                      ((uint32_t)0x00000001)       /*!<PMW_TC和PWM_MR0匹配时产生中断控制位        */
#define  PWM_MCR_MR1I                      ((uint32_t)0x00000002)       /*!<PMW_TC和PWM_MR1匹配时产生中断控制位        */
#define  PWM_MCR_MR2I                      ((uint32_t)0x00000004)       /*!<PMW_TC和PWM_MR2匹配时产生中断控制位        */
#define  PWM_MCR_MR3I                      ((uint32_t)0x00000008)       /*!<PMW_TC和PWM_MR3匹配时产生中断控制位        */
#define  PWM_MCR_MR4I                      ((uint32_t)0x00000010)       /*!<PMW_TC和PWM_MR4匹配时产生中断控制位        */
#define  PWM_MCR_MR0S                      ((uint32_t)0x00000020)       /*!<PMW_TC和PWM_MR0匹配时计时器停止控制位      */
#define  PWM_MCR_DIR                       ((uint32_t)0x00000040)       /*!<PWM_TC计数方向控制位                       */
#define  PWM_MCR_DIR_RV                    ((uint32_t)0x00000080)       /*!<PWM_TC计数方向交替控制位                   */

/*******************  Bit definition for PWM_MR0 register  *******************/
#define  PWM_MR0                           ((uint16_t)0xFFFF)            /*!<MR0[15:0]定时器MR0匹配值   */

/*******************  Bit definition for PWM_MR1 register  *******************/
#define  PWM_MR1                           ((uint16_t)0xFFFF)            /*!<MR1[15:0]定时器MR1匹配值   */

/*******************  Bit definition for PWM_MR2 register  *******************/
#define  PWM_MR2                           ((uint16_t)0xFFFF)            /*!<MR2[15:0]定时器MR2匹配值   */

/*******************  Bit definition for PWM_MR3 register  *******************/
#define  PWM_MR3                           ((uint16_t)0xFFFF)            /*!<MR3[15:0]定时器MR3匹配值   */

/*******************  Bit definition for PWM_MR4 register  *******************/
#define  PWM_MR4                           ((uint16_t)0xFFFF)            /*!<MR4[15:0]定时器MR4匹配值   */

/*******************  Bit definition for PWM_CCR register  *******************/
#define  PWM_CCR_CAP1RE                    ((uint32_t)0x00000001)       /*!<通道1脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP1FE                    ((uint32_t)0x00000002)       /*!<通道1脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP1RST                   ((uint32_t)0x00000004)       /*!<通道1脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP1IE                    ((uint32_t)0x00000008)       /*!<通道1脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP2RE                    ((uint32_t)0x00000010)       /*!<通道2脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP2FE                    ((uint32_t)0x00000020)       /*!<通道2脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP2RST                   ((uint32_t)0x00000040)       /*!<通道2脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP2IE                    ((uint32_t)0x00000080)       /*!<通道2脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP3RE                    ((uint32_t)0x00000100)       /*!<通道3脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP3FE                    ((uint32_t)0x00000200)       /*!<通道3脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP3RST                   ((uint32_t)0x00000400)       /*!<通道3脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP3IE                    ((uint32_t)0x00000800)       /*!<通道3脉冲沿捕捉中断使能控制          */
#define  PWM_CCR_CAP4RE                    ((uint32_t)0x00001000)       /*!<通道4脉冲上升沿捕捉使能控制          */
#define  PWM_CCR_CAP4FE                    ((uint32_t)0x00002000)       /*!<通道4脉冲下降沿捕捉使能控制          */
#define  PWM_CCR_CAP4RST                   ((uint32_t)0x00004000)       /*!<通道4脉冲沿捕捉计数器复位使能控制    */
#define  PWM_CCR_CAP4IE                    ((uint32_t)0x00008000)       /*!<通道4脉冲沿捕捉中断使能控制          */

/*******************  Bit definition for PWM_CR1 register  *******************/
#define  PWM_CR1_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道1沿变化计数值   */
#define  PWM_CR1_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR2 register  *******************/
#define  PWM_CR2_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道2沿变化计数值   */
#define  PWM_CR2_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR3 register  *******************/
#define  PWM_CR3_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道3沿变化计数值   */
#define  PWM_CR3_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CR4 register  *******************/
#define  PWM_CR4_CAP                       ((uint32_t)0x0000FFFF)       /*!<CAP[15:0]捕捉通道4沿变化计数值   */
#define  PWM_CR4_CAPEDGE                   ((uint32_t)0x00010000)       /*!<捕获沿标志位            */

/*******************  Bit definition for PWM_CMR register  *******************/
#define  PWM_CMR_MC1                       ((uint32_t)0x00000007)       /*!<MC1[2:0]  输出通道1比较输出模式选择    */
#define  PWM_CMR_MC2                       ((uint32_t)0x00000070)       /*!<MC2[6:4]  输出通道2比较输出模式选择    */
#define  PWM_CMR_MC3                       ((uint32_t)0x00000700)       /*!<MC3[10:8] 输出通道3比较输出模式选择    */
#define  PWM_CMR_MC4                       ((uint32_t)0x00007000)       /*!<MC4[14:12]输出通道4比较输出模式选择    */
#define  PWM_CMR_PWM1_IDLE                 ((uint32_t)0x00010000)       /*!<输出通道1初始值                        */
#define  PWM_CMR_PWM2_IDLE                 ((uint32_t)0x00020000)       /*!<输出通道2初始值                        */
#define  PWM_CMR_PWM3_IDLE                 ((uint32_t)0x00040000)       /*!<输出通道3初始值                        */
#define  PWM_CMR_PWM4_IDLE                 ((uint32_t)0x00080000)       /*!<输出通道4初始值                        */
#define  PWM_CMR_PWM1n_EN                  ((uint32_t)0x00100000)       /*!<输出通道1互补输出使能                  */
#define  PWM_CMR_PWM2n_EN                  ((uint32_t)0x00200000)       /*!<输出通道2互补输出使能                  */
#define  PWM_CMR_PWM3n_EN                  ((uint32_t)0x00400000)       /*!<输出通道3互补输出使能                  */
#define  PWM_CMR_PWM4n_EN                  ((uint32_t)0x00800000)       /*!<输出通道4互补输出使能                  */
#define  PWM_CMR_BKE                       ((uint32_t)0x01000000)       /*!<刹车输入使能控制                       */
#define  PWM_CMR_BKP                       ((uint32_t)0x02000000)       /*!<刹车输入有效电平控制                   */
#define  PWM_CMR_BKI                       ((uint32_t)0x04000000)       /*!<刹车输入中断使能控制                   */

/*******************  Bit definition for PWM_DT register  *******************/
#define  PWM_DT                            ((uint8_t)0xFF)              /*!<DT[7:0]死区时间控制   */

/******************************************************************************/
/*                                                                            */
/*                      CRC Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_CR register  *******************/
#define CRC_CR_EN                          ((uint32_t)0x00000001)          /*!< CRC Enable */
#define CRC_CR_INIT                        ((uint32_t)0x00000002)          /*!< CRC Initial */
#define CRC_CR_DINWIDTH                    ((uint32_t)0x00000004)          /*!< CRC DataIn Width */
#define CRC_CR_INPUT_BITREV                ((uint32_t)0x00000008)          /*!< CRC DataIn Width */
#define CRC_CR_INPUT_BYTEREV               ((uint32_t)0x00000010)          /*!< CRC DataIn Width */
#define CRC_CR_OUTPUT_BITREV               ((uint32_t)0x00000020)          /*!< CRC DataIn Width */

/*******************  Bit definition for CRC_SEED register  *******************/
#define CRC_SEED_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC SEED */

/*******************  Bit definition for CRC_POLY register  *******************/
#define CRC_POLY_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC SEED */

/*******************  Bit definition for CRC_POLY register  *******************/
#define CRC_DIN_MASK                        ((uint32_t)0x0000FFFF)          /*!< CRC DATA INPUT */

/*******************  Bit definition for CRC_POLY register  *******************/
#define CRC_DOUT_MASK                       ((uint32_t)0x0000FFFF)          /*!< CRC DATA OUT */

/******************************************************************************/
/*                                                                            */
/*                      CRC Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for CRC_CR register  *******************/


/******************************************************************************/
/*                                                                            */
/*                        Independent WATCHDOG (WDG)                         */
/*                                                                            */
/******************************************************************************/
/*******************  Bit definition for WDG_RLR register  *******************/
#define  WDG_RLR_RL                          ((uint32_t)0xFFFFFFFF)            /*!< Watchdog counter reload value */

/*******************  Bit definition for WDG_CR register  *******************/
#define  WDG_CR_INTE                         ((uint32_t)0x00000001)       /*!< Enable IWDG        */
#define  WDG_CR_RSTE                         ((uint32_t)0x00000002)       /*!< RESET IWDG         */
#define  WDG_CR_DBGE                         ((uint32_t)0x00000004)       /*!< RESET IWDG         */

/*******************  Bit definition for WDG_RLR register  *******************/
#define  WDG_RLR_RL                          ((uint32_t)0xFFFFFFFF)            /*!< Watchdog counter reload value */

/*******************  Bit definition for WDG_RIS register  *******************/
#define  WDG_RIS_RVU                         ((uint32_t)0x00000001)               /*!< Watchdog counter reload value update */

/*******************  Bit definition for WDG_KR register  ********************/
#define  WDG_KR_KEY                          ((uint32_t)0xFFFFFFFF)            /*!< Key value (write only, read 0000h) */
#define  WDG_KR_LOCK                         ((uint32_t)0x00000001)            /*!< IWDG LOCK        */

/*******************  Bit definition for WDG_RIS register  *******************/
#define  WDG_EIS_RVU                         ((uint32_t)0x00000001)               /*!< Watchdog counter reload value update */

/******************************************************************************/
/*                                                                            */
/*                         BEEP (BEEP)                         */
/*                                                                            */
/******************************************************************************/
#define  BEEP_CR_EN                          ((uint32_t)0x00000001)       /*!< Enable BEEP        */
#define  BEEP_CR_CPSR                        ((uint32_t)0x0000003E)       /*!<         */
#define  BEEP_CR_SUBDIV                      ((uint32_t)0x000000C0)       /*!<         */


/******************************************************************************/
/*                                                                            */
/*                        QSPI  Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for QSPI_CTRLR0 register  ********************/
#define  QSPI_CTRLR0_DFS                     ((uint32_t)0x0000001F)         /*!< 定义数据帧长          */
#define  QSPI_CTRLR0_DFS_4B                  ((uint32_t)0x00000003)         /*!< 数据帧长 4 bits       */
#define  QSPI_CTRLR0_DFS_5B                  ((uint32_t)0x00000004)         /*!< 数据帧长 5 bits       */
#define  QSPI_CTRLR0_DFS_6B                  ((uint32_t)0x00000005)         /*!< 数据帧长 6 bits       */
#define  QSPI_CTRLR0_DFS_7B                  ((uint32_t)0x00000006)         /*!< 数据帧长 7 bits       */
#define  QSPI_CTRLR0_DFS_8B                  ((uint32_t)0x00000007)         /*!< 数据帧长 8 bits       */
#define  QSPI_CTRLR0_DFS_9B                  ((uint32_t)0x00000008)         /*!< 数据帧长 9 bits       */
#define  QSPI_CTRLR0_DFS_10B                 ((uint32_t)0x00000009)         /*!< 数据帧长 10 bits      */
#define  QSPI_CTRLR0_DFS_11B                 ((uint32_t)0x0000000A)         /*!< 数据帧长 11 bits      */
#define  QSPI_CTRLR0_DFS_12B                 ((uint32_t)0x0000000B)         /*!< 数据帧长 12 bits      */
#define  QSPI_CTRLR0_DFS_13B                 ((uint32_t)0x0000000C)         /*!< 数据帧长 13 bits      */
#define  QSPI_CTRLR0_DFS_14B                 ((uint32_t)0x0000000D)         /*!< 数据帧长 14 bits      */
#define  QSPI_CTRLR0_DFS_15B                 ((uint32_t)0x0000000E)         /*!< 数据帧长 15 bits      */
#define  QSPI_CTRLR0_DFS_16B                 ((uint32_t)0x0000000F)         /*!< 数据帧长 16 bits      */
#define  QSPI_CTRLR0_DFS_17B                 ((uint32_t)0x00000010)         /*!< 数据帧长 17 bits      */
#define  QSPI_CTRLR0_DFS_18B                 ((uint32_t)0x00000011)         /*!< 数据帧长 18 bits      */
#define  QSPI_CTRLR0_DFS_19B                 ((uint32_t)0x00000012)         /*!< 数据帧长 19 bits      */
#define  QSPI_CTRLR0_DFS_20B                 ((uint32_t)0x00000013)         /*!< 数据帧长 20 bits      */
#define  QSPI_CTRLR0_DFS_21B                 ((uint32_t)0x00000014)         /*!< 数据帧长 21 bits      */
#define  QSPI_CTRLR0_DFS_22B                 ((uint32_t)0x00000015)         /*!< 数据帧长 22 bits        */
#define  QSPI_CTRLR0_DFS_23B                 ((uint32_t)0x00000016)         /*!< 数据帧长 23 bits      */
#define  QSPI_CTRLR0_DFS_24B                 ((uint32_t)0x00000017)         /*!< 数据帧长 24 bits      */
#define  QSPI_CTRLR0_DFS_25B                 ((uint32_t)0x00000018)         /*!< 数据帧长 25 bits      */
#define  QSPI_CTRLR0_DFS_26B                 ((uint32_t)0x00000019)         /*!< 数据帧长 26 bits      */
#define  QSPI_CTRLR0_DFS_27B                 ((uint32_t)0x0000001A)         /*!< 数据帧长 27 bits      */
#define  QSPI_CTRLR0_DFS_28B                 ((uint32_t)0x0000001B)         /*!< 数据帧长 28 bits      */
#define  QSPI_CTRLR0_DFS_29B                 ((uint32_t)0x0000001C)         /*!< 数据帧长 29 bits      */
#define  QSPI_CTRLR0_DFS_30B                 ((uint32_t)0x0000001D)         /*!< 数据帧长 30 bits      */
#define  QSPI_CTRLR0_DFS_31B                 ((uint32_t)0x0000001E)         /*!< 数据帧长 31 bits      */
#define  QSPI_CTRLR0_DFS_32B                 ((uint32_t)0x0000001F)         /*!< 数据帧长 32 bits      */
#define  QSPI_CTRLR0_FRF                     ((uint32_t)0x000000C0)         /*!< 定义帧格式            */
#define  QSPI_CTRLR0_FRF_SPI                 ((uint32_t)0x00000000)         /*!< SPI格式               */
#define  QSPI_CTRLR0_FRF_MICROWIRE           ((uint32_t)0x00000080)         /*!< Microwire格式         */
#define  QSPI_CTRLR0_CPHA                    ((uint32_t)0x00000100)         /*!< 时钟相位选择          */
#define  QSPI_CTRLR0_CPOL                    ((uint32_t)0x00000200)         /*!< 时钟极性选择          */
#define  QSPI_CTRLR0_TMOD                    ((uint32_t)0x00000C00)         /*!< 传输模式              */
#define  QSPI_CTRLR0_TMOD_TXRX               ((uint32_t)0x00000000)         /*!< 传输模式_收发模式     */
#define  QSPI_CTRLR0_TMOD_TX                 ((uint32_t)0x00000400)         /*!< 传输模式_发送模式     */
#define  QSPI_CTRLR0_TMOD_RX                 ((uint32_t)0x00000800)         /*!< 传输模式_接收模式     */
#define  QSPI_CTRLR0_TMOD_EPPREAD            ((uint32_t)0x00000C00)         /*!< 传输模式_EPPROM读模式 */
#define  QSPI_CTRLR0_SLV_OE                  ((uint32_t)0x00001000)         /*!< 从机输出禁止          */
#define  QSPI_CTRLR0_SRL                     ((uint32_t)0x00002000)         /*!< 回绕模式使能控制      */
#define  QSPI_CTRLR0_SSTE                    ((uint32_t)0x00004000)         /*!< CS信号翻转使能        */
#define  QSPI_CTRLR0_CFS                     ((uint32_t)0x000F0000)         /*!< Microwire 控制帧长度  */
#define  QSPI_CTRLR0_CFS_1B                  ((uint32_t)0x00000000)         /*!< Microwire 控制帧长度 1bits  */
#define  QSPI_CTRLR0_CFS_2B                  ((uint32_t)0x00010000)         /*!< Microwire 控制帧长度 2bits  */
#define  QSPI_CTRLR0_CFS_3B                  ((uint32_t)0x00020000)         /*!< Microwire 控制帧长度 3bits  */
#define  QSPI_CTRLR0_CFS_4B                  ((uint32_t)0x00030000)         /*!< Microwire 控制帧长度 4bits  */
#define  QSPI_CTRLR0_CFS_5B                  ((uint32_t)0x00040000)         /*!< Microwire 控制帧长度 5bits  */
#define  QSPI_CTRLR0_CFS_6B                  ((uint32_t)0x00050000)         /*!< Microwire 控制帧长度 6bits  */
#define  QSPI_CTRLR0_CFS_7B                  ((uint32_t)0x00060000)         /*!< Microwire 控制帧长度 7bits  */
#define  QSPI_CTRLR0_CFS_8B                  ((uint32_t)0x00070000)         /*!< Microwire 控制帧长度 8bits  */
#define  QSPI_CTRLR0_CFS_9B                  ((uint32_t)0x00080000)         /*!< Microwire 控制帧长度 9bits  */
#define  QSPI_CTRLR0_CFS_10B                 ((uint32_t)0x00090000)         /*!< Microwire 控制帧长度 10bits */
#define  QSPI_CTRLR0_CFS_11B                 ((uint32_t)0x000A0000)         /*!< Microwire 控制帧长度 11bits */
#define  QSPI_CTRLR0_CFS_12B                 ((uint32_t)0x000B0000)         /*!< Microwire 控制帧长度 12bits */
#define  QSPI_CTRLR0_CFS_13B                 ((uint32_t)0x000C0000)         /*!< Microwire 控制帧长度 13bits */
#define  QSPI_CTRLR0_CFS_14B                 ((uint32_t)0x000D0000)         /*!< Microwire 控制帧长度 14bits */
#define  QSPI_CTRLR0_CFS_15B                 ((uint32_t)0x000E0000)         /*!< Microwire 控制帧长度 15bits */
#define  QSPI_CTRLR0_CFS_16B                 ((uint32_t)0x000F0000)         /*!< Microwire 控制帧长度 16bits */
#define  QSPI_CTRLR0_SPI_FRF                 ((uint32_t)0x00C00000)         /*!< SPI模式帧格式         */
#define  QSPI_CTRLR0_SPI_FRF_SINGLE          ((uint32_t)0x00000000)         /*!< SPI模式帧格式_单线    */
#define  QSPI_CTRLR0_SPI_FRF_DUAL            ((uint32_t)0x00400000)         /*!< SPI模式帧格式_双线    */
#define  QSPI_CTRLR0_SPI_FRF_QUAD            ((uint32_t)0x00800000)         /*!< SPI模式帧格式_四线    */

/*******************  Bit definition for QSPI_CTRLR1 register  ********************/
#define  QSPI_CTRLR1_NDF                     ((uint32_t)0x0000FFFF)         /*!< 设置接收数据帧的数目   */

/*******************  Bit definition for QSPI_ENR register  ********************/
#define  QSPI_EN                             ((uint32_t)0x00000001)         /*!< QSPI使能               */

/*******************  Bit definition for QSPI_MWCR register  ********************/
#define  QSPI_MWCR_MWMOD                      ((uint32_t)0x00000001)         /*!< Microwire传输模式     */
#define  QSPI_MWCR_MDD                        ((uint32_t)0x00000002)         /*!< Microwire方向设置     */
#define  QSPI_MWCR_MHS                        ((uint32_t)0x00000004)         /*!< Microwire握手使能     */

/*******************  Bit definition for QSPI_SER register  ********************/
#define  QSPI_CS_EN                           ((uint32_t)0x00000001)         /*!< CS使能                 */

/*******************  Bit definition for QSPI_BAUDR register  ********************/
#define  QSPI_BAUDR_SCKDIV                    ((uint32_t)0x0000FFFF)         /*!< QSPI时钟分频 0~65535    */

/*******************  Bit definition for QSPI_TXFTLR register  ********************/
#define  QSPI_TXFTLR_TFT                      ((uint32_t)0x000000FF)         /*!< 发送FIFO中断触发阈值设定    */
#define  QSPI_TXFTLR_TXFTHR                   ((uint32_t)0x00FF0000)         /*!< 开始发送FIFO级别设定    */

/*******************  Bit definition for QSPI_RXFTLR register  ********************/
#define  QSPI_RXFTLR_RFT                      ((uint32_t)0x000000FF)         /*!< 接收FIFO中断触发阈值设定  */

/*******************  Bit definition for QSPI_TXFLR register  ********************/
#define  QSPI_TXTFL                           ((uint32_t)0x0000001F)         /*!< TX FIFO的当前数据总数      */

/*******************  Bit definition for QSPI_RXFLR register  ********************/
#define  QSPI_RXTFL                           ((uint32_t)0x0000001F)         /*!< RX FIFO的当前数据总数      */

/*******************  Bit definition for QSPI_SR register  ********************/
#define  QSPI_SR_BUSY                          ((uint32_t)0x00000001)         /*!< 收发忙状态位            */
#define  QSPI_SR_TFNF                          ((uint32_t)0x00000002)         /*!< 发送FIFO未满状态位      */
#define  QSPI_SR_TFE                           ((uint32_t)0x00000004)         /*!< 发送FIFO全空状态位      */
#define  QSPI_SR_RNE                           ((uint32_t)0x00000008)         /*!< 接收FIFO非空状态位      */
#define  QSPI_SR_RFF                           ((uint32_t)0x00000010)         /*!< 接收FIFO满状态位        */

/*******************  Bit definition for QSPI_IMR register  ********************/
#define  QSPI_IMR_TXEIM                        ((uint32_t)0x00000001)         /*!< 发送FIFO空中断使能控制        */
#define  QSPI_IMR_TXOIM                        ((uint32_t)0x00000002)         /*!< 发送FIFO溢出中断使能控制      */
#define  QSPI_IMR_RXUIM                        ((uint32_t)0x00000004)         /*!< 接收FIFO下溢中断使能控制      */
#define  QSPI_IMR_RXOIM                        ((uint32_t)0x00000008)         /*!< 接收器FIFO溢出中断使能控制    */
#define  QSPI_IMR_RXFIM                        ((uint32_t)0x00000010)         /*!< 接收FIFO满中断使能控制        */

/*******************  Bit definition for QSPI_ISR register  ********************/
#define  QSPI_ISR_TXEIS                        ((uint32_t)0x00000001)         /*!< 发送FIFO空中断状态位        */
#define  QSPI_ISR_TXOIS                        ((uint32_t)0x00000002)         /*!< 发送FIFO溢出中断状态位      */
#define  QSPI_ISR_RXUIS                        ((uint32_t)0x00000004)         /*!< 接收FIFO下溢中断状态位      */
#define  QSPI_ISR_RXOIS                        ((uint32_t)0x00000008)         /*!< 接收FIFO溢出中断状态位      */
#define  QSPI_ISR_RXFIS                        ((uint32_t)0x00000010)         /*!< 接收FIFO满中断状态位        */

/*******************  Bit definition for QSPI_RISR register  ********************/
#define  QSPI_RISR_TXEIR                       ((uint32_t)0x00000001)         /*!< 发送FIFO空原始状态位        */
#define  QSPI_RISR_TXOIR                       ((uint32_t)0x00000002)         /*!< 发送FIFO溢出原始状态位      */
#define  QSPI_RISR_RXUIR                       ((uint32_t)0x00000004)         /*!< 接收FIFO下溢原始状态位      */
#define  QSPI_RISR_RXOIR                       ((uint32_t)0x00000008)         /*!< 接收FIFO溢出原始状态位      */
#define  QSPI_RISR_RXFIR                       ((uint32_t)0x00000010)         /*!< 接收FIFO满状原始态位        */

/*******************  Bit definition for QSPI_TXOICR register  ********************/
#define  QSPI_TXOICR                           ((uint32_t)0x00000001)         /*!< 清除发送FIFO溢出中断      */

/*******************  Bit definition for QSPI_RXOICR register  ********************/
#define  QSPI_RXOICR                           ((uint32_t)0x00000001)         /*!< 清除接收FIFO溢出中断      */

/*******************  Bit definition for QSPI_RXUICR register  ********************/
#define  QSPI_RXUICR                           ((uint32_t)0x00000001)         /*!< 清除接收IFO下溢中断      */

/*******************  Bit definition for QSPI_MSTICR register  ********************/
#define  QSPI_MSTICR                           ((uint32_t)0x00000001)         /*!< 清除多主机中断            */

/*******************  Bit definition for QSPI_ICR register  ********************/
#define  QSPI_ICR                              ((uint32_t)0x00000001)         /*!< 中断清除(TXOIS/RXOIS/RXUIS/MSTIS)  */

/*******************  Bit definition for QSPI_DMACR register  ********************/
#define  QSPI_DMACR_RDMAE                      ((uint32_t)0x00000001)         /*!< 接收DMA使能控制        */
#define  QSPI_DMACR_TDMAE                      ((uint32_t)0x00000002)         /*!< 发送DMA使能控制      */

/*******************  Bit definition for QSPI_DMATDLR register  ********************/
#define  QSPI_DMATDL                           ((uint32_t)0x0000000F)         /*!< DMA发送数据触发级别        */

/*******************  Bit definition for QSPI_DMARDLR register  ********************/
#define  QSPI_DMARDL                           ((uint32_t)0x0000000F)         /*!< DMA接收数据触发级别        */

/*******************  Bit definition for QSPI_IDR register  ********************/
#define  QSPI_ID                               ((uint32_t)0xFFFFFFFF)         /*!< QSPI ID                    */

/*******************  Bit definition for QSPI_VERSION_IDR register  ********************/
#define  QSPI_VERSION_ID                      ((uint32_t)0xFFFFFFFF)         /*!< QSPI版本ID                 */

/*******************  Bit definition for QSPI_DR register  ********************/
#define  QSPI_DATA                            ((uint32_t)0xFFFFFFFF)         /*!< QSPI读写数据寄存器             */

/*******************  Bit definition for QSPI_SPI_CTRLR register  ********************/
#define  QSPI_SPI_CTRLR_TRANS_TYPE            ((uint32_t)0x00000003)         /*!< 指令和地址发送格式             */
#define  QSPI_SPI_CTRLR_TRANS_TYPE_TT0        ((uint32_t)0x00000000)         /*!< 指令和地址将以标准SPI模式发送  */
#define  QSPI_SPI_CTRLR_TRANS_TYPE_TT1        ((uint32_t)0x00000001)         /*!< 指令将以标准SPI模式发送，地址将以指定的模式发送  */
#define  QSPI_SPI_CTRLR_TRANS_TYPE_TT2        ((uint32_t)0x00000002)         /*!< 指令和地址将以SPI_FRF指定的模式发送             */
#define  QSPI_SPI_CTRLR_ADDR_L                ((uint32_t)0x0000003C)         /*!< 地址帧长度                     */
#define  QSPI_SPI_CTRLR_ADDR_L0               ((uint32_t)0x00000000)         /*!< 地址帧长度0                      */
#define  QSPI_SPI_CTRLR_ADDR_L4               ((uint32_t)0x00000004)         /*!< 地址帧长度4                      */
#define  QSPI_SPI_CTRLR_ADDR_L8               ((uint32_t)0x00000008)         /*!< 地址帧长度8                      */
#define  QSPI_SPI_CTRLR_ADDR_L12              ((uint32_t)0x0000000C)         /*!< 地址帧长度12                     */
#define  QSPI_SPI_CTRLR_ADDR_L16              ((uint32_t)0x00000010)         /*!< 地址帧长度16                     */
#define  QSPI_SPI_CTRLR_ADDR_L20              ((uint32_t)0x00000014)         /*!< 地址帧长度20                    */
#define  QSPI_SPI_CTRLR_ADDR_L24              ((uint32_t)0x00000018)         /*!< 地址帧长度24                     */
#define  QSPI_SPI_CTRLR_ADDR_L28              ((uint32_t)0x0000001C)         /*!< 地址帧长度28                     */
#define  QSPI_SPI_CTRLR_ADDR_L32              ((uint32_t)0x00000020)         /*!< 地址帧长度32                     */
#define  QSPI_SPI_CTRLR_ADDR_L36              ((uint32_t)0x00000024)         /*!< 地址帧长度36                     */
#define  QSPI_SPI_CTRLR_ADDR_L40              ((uint32_t)0x00000028)         /*!< 地址帧长度40                     */
#define  QSPI_SPI_CTRLR_ADDR_L44              ((uint32_t)0x0000002C)         /*!< 地址帧长度44                     */
#define  QSPI_SPI_CTRLR_ADDR_L48              ((uint32_t)0x00000030)         /*!< 地址帧长度48                     */
#define  QSPI_SPI_CTRLR_ADDR_L52              ((uint32_t)0x00000034)         /*!< 地址帧长度52                     */
#define  QSPI_SPI_CTRLR_ADDR_L56              ((uint32_t)0x00000038)         /*!< 地址帧长度56                     */
#define  QSPI_SPI_CTRLR_ADDR_L60              ((uint32_t)0x0000003C)         /*!< 地址帧长度60                     */
#define  QSPI_SPI_CTRLR_INST_L                ((uint32_t)0x00000300)         /*!< 指令帧长度                       */
#define  QSPI_SPI_CTRLR_INST_L0               ((uint32_t)0x00000000)         /*!< 指令帧长度0                      */
#define  QSPI_SPI_CTRLR_INST_L4               ((uint32_t)0x00000100)         /*!< 指令帧长度4                      */
#define  QSPI_SPI_CTRLR_INST_L8               ((uint32_t)0x00000200)         /*!< 指令帧长度8                      */
#define  QSPI_SPI_CTRLR_INST_L16              ((uint32_t)0x00000300)         /*!< 指令帧长度16                     */
#define  QSPI_SPI_CTRLR_WAIT_CYCLES           ((uint32_t)0x0000F800)         /*!< 等待周期                       */

/**
  * @}
  */

/******************************************************************************/
/*                                                                            */
/*                        DSP  Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DSP_SQRT_STATUS register  ********************/
#define  DSP_SQRT_STATUS                     ((uint32_t)0x00000001)         /*!< 开方状态寄存器   */

/*******************  Bit definition for DSP_ABSCALC register  ********************/
#define  DSP_ABSCALC                         ((int32_t)0xFFFFFFFF)         /*!< 绝对值计算寄存器   */

/*******************  Bit definition for DSP_SQRTCALC register  ********************/
#define  DSP_SQRTCALC                        ((uint32_t)0xFFFFFFFF)         /*!< 开方数据寄存器     */

/*******************  Bit definition for DSP_ATAN_X register  ********************/
#define  DSP_ATAN_X                          ((int32_t)0xFFFFFFFF)          /*!< 反正切x坐标寄存器   */

/*******************  Bit definition for DSP_ATAN_Y register  ********************/
#define  DSP_ATAN_Y                          ((int32_t)0xFFFFFFFF)          /*!< 反正切y坐标寄存器     */

/*******************  Bit definition for DSP_ATAN_RESULT register  ********************/
#define  DSP_ATAN_RESULT                     ((uint32_t)0xFFFF)             /*!< 反正切计算结果寄存器  */

/*******************  Bit definition for DSP_GET_BIT1 register  ********************/
#define  DSP_GET_BIT1                       ((uint32_t)0xFFFFFFFF)         /*!< 稀疏1计算寄存器        */

/*******************  Bit definition for DSP_BATCH_OFFSET register  ********************/
#define  DSP_BATCH_OFFSET                   ((uint32_t)0x000000FF)         /*!< 批量计算offset配置寄存器    */

/*******************  Bit definition for DSP_BATCH_SLWR register  ********************/
#define  DSP_BATCH_SLWR                     ((uint32_t)0x000003FF)         /*!< 批量计算slwr配置寄存器    */

/*******************  Bit definition for DSP_BATCH_SAWR register  ********************/
#define  DSP_BATCH_SAWR                     ((uint32_t)0x000003FF)         /*!< 批量计算SAWR配置寄存器    */

/*******************  Bit definition for DSP_BATCH_DEF_LENERROR register  ********************/
#define  DSP_BATCH_DEF_LENERROR             ((uint32_t)0x000000FF)         /*!< 批量计算DEF_LENERROR配置寄存器    */

/*******************  Bit definition for DSP_BATCH_DEF_DIRERROR register  ********************/
#define  DSP_BATCH_DEF_DIRERROR             ((uint32_t)0x000000FF)         /*!< 批量计算DEF_DIRERROR配置寄存器    */

/*******************  Bit definition for DSP_BATCH_LEN register  ********************/
#define  DSP_BATCH_TEST_LEN                 ((uint32_t)0x000000FF)           /*!< 批量计算TEST_LEN配置寄存器    */
#define  DSP_BATCH_LINE_LEN                 ((uint32_t)0x0000FF00)           /*!< 批量计算LINE_LEN配置寄存器    */

/*******************  Bit definition for DSP_BATCH_A1 register  ********************/
#define  DSP_BATCH_TEST_A1                  ((uint32_t)0x000000FF)           /*!< 批量计算TEST_A1配置寄存器    */
#define  DSP_BATCH_LINE_A1                  ((uint32_t)0x0000FF00)           /*!< 批量计算LINE_A1配置寄存器    */

/*******************  Bit definition for DSP_BATCH_A2 register  ********************/
#define  DSP_BATCH_TEST_A2                  ((uint32_t)0x000000FF)           /*!< 批量计算TEST_A2配置寄存器    */
#define  DSP_BATCH_LINE_A2                  ((uint32_t)0x0000FF00)           /*!< 批量计算LINE_A2配置寄存器    */

/*******************  Bit definition for DSP_BATCH_PAIRSIMILARITY register  ********************/
#define  DSP_BATCH_PAIRSIMILARITY           ((uint32_t)0x000003FF)           /*!< 批量计算结果PAIRSIMILARITY寄存器    */

/*******************  Bit definition for DSP_ROTATE_ANGLE register  ********************/
#define  DSP_ROTATE_ANGLE                   ((uint32_t)0x000000FF)           /*!< 点坐标旋转角度配置寄存器    */

/*******************  Bit definition for DSP_ROTATE_XY0 register  ********************/
#define  DSP_ROTATE_X0                      ((int16_t)0xFFFF)                /*!< 旋转原点x坐标x0    */
#define  DSP_ROTATE_Y0                      ((int16_t)0xFFFF)                /*!< 旋转原点y坐标y0    */

/*******************  Bit definition for DSP_ROTATE_XY1 register  ********************/
#define  DSP_ROTATE_X1                      ((int16_t)0xFFFF)                /*!< 旋转点x坐标x1    */
#define  DSP_ROTATE_Y1                      ((int16_t)0xFFFF)                /*!< 旋转点y坐标y1    */

/*******************  Bit definition for DSP_ROTATE_XY register  ********************/
#define  DSP_ROTATE_RESULT_X                ((int16_t)0xFFFF)                /*!< 旋转结果点x坐标x    */
#define  DSP_ROTATE_RESULT_Y                ((int16_t)0xFFFF)                /*!< 旋转结果点y坐标y    */

/*******************  Bit definition for DSP_PIXEL_CFG register  ********************/
#define  DSP_PIXEL_CFG_B1                   ((uint32_t)0x000000FF)            /*!< 像素位对比第二组比较地址B1    */
#define  DSP_PIXEL_CFG_B0                   ((uint32_t)0x0000FF00)            /*!< 像素位对比第二组比较地址B0    */
#define  DSP_PIXEL_CFG_A1                   ((uint32_t)0x00FF0000)            /*!< 像素位对比第一组比较地址A1    */
#define  DSP_PIXEL_CFG_A0                   ((uint32_t)0xFF000000)            /*!< 像素位对比第一组比较地址A0    */

/*******************  Bit definition for DSP_PIXEL1_DATA0 register  ********************/
#define  DSP_PIXEL1_DATA0_BYTE3             ((uint32_t)0x000000FF)            /*!< 像素位对比第一组数据Byte3    */
#define  DSP_PIXEL1_DATA0_BYTE2             ((uint32_t)0x0000FF00)            /*!< 像素位对比第一组数据Byte2    */
#define  DSP_PIXEL1_DATA0_BYTE1             ((uint32_t)0x00FF0000)            /*!< 像素位对比第一组数据Byte1    */
#define  DSP_PIXEL1_DATA0_BYTE0             ((uint32_t)0xFF000000)            /*!< 像素位对比第一组数据Byte0    */

/*******************  Bit definition for DSP_PIXEL1_DATA1 register  ********************/
#define  DSP_PIXEL1_DATA1_BYTE7             ((uint32_t)0x000000FF)            /*!< 像素位对比第一组数据Byte7    */
#define  DSP_PIXEL1_DATA1_BYTE6             ((uint32_t)0x0000FF00)            /*!< 像素位对比第一组数据Byte6    */
#define  DSP_PIXEL1_DATA1_BYTE5             ((uint32_t)0x00FF0000)            /*!< 像素位对比第一组数据Byte5    */
#define  DSP_PIXEL1_DATA1_BYTE4             ((uint32_t)0xFF000000)            /*!< 像素位对比第一组数据Byte4    */

/*******************  Bit definition for DSP_PIXEL2_DATA0 register  ********************/
#define  DSP_PIXEL2_DATA0_BYTE3             ((uint32_t)0x000000FF)            /*!< 像素位对比第二组数据Byte3    */
#define  DSP_PIXEL2_DATA0_BYTE2             ((uint32_t)0x0000FF00)            /*!< 像素位对比第二组数据Byte2    */
#define  DSP_PIXEL2_DATA0_BYTE1             ((uint32_t)0x00FF0000)            /*!< 像素位对比第二组数据Byte1    */
#define  DSP_PIXEL2_DATA0_BYTE0             ((uint32_t)0xFF000000)            /*!< 像素位对比第二组数据Byte0    */

/*******************  Bit definition for DSP_PIXEL2_DATA1 register  ********************/
#define  DSP_PIXEL2_DATA1_BYTE7             ((uint32_t)0x000000FF)            /*!< 像素位对比第二组数据Byte7    */
#define  DSP_PIXEL2_DATA1_BYTE6             ((uint32_t)0x0000FF00)            /*!< 像素位对比第二组数据Byte6    */
#define  DSP_PIXEL2_DATA1_BYTE5             ((uint32_t)0x00FF0000)            /*!< 像素位对比第二组数据Byte5    */
#define  DSP_PIXEL2_DATA1_BYTE4             ((uint32_t)0xFF000000)            /*!< 像素位对比第二组数据Byte4    */

/*******************  Bit definition for DSP_PIXEL_COMP_RESULT register  ********************/
#define  DSP_PIXEL_COMP_RESULT              ((uint32_t)0x000000FF)            /*!< 像素位对比结果    */

/*******************  Bit definition for DSP_CAPBIT_ADDR_CFG register  ********************/
#define  DSP_CAPBIT_ADDR_CFG               ((uint32_t)0xFFFFFFFF)             /*!< 设定位获取地址   */

/*******************  Bit definition for DSP_CAPBIT_CFG register  ********************/
#define  DSP_CAPBIT_CFG_N                  ((uint32_t)0x0000FFFF)             /*!< 位获取起始位置   */
#define  DSP_CAPBIT_CFG_C                  ((uint32_t)0xFFFF0000)             /*!< 位获取获取bit数  */

/*******************  Bit definition for DSP_CAPBIT_RESULT register  ********************/
#define  DSP_CAPBIT_RESULT                 ((uint32_t)0xFFFFFFFF)             /*!< 位获取结果       */

/*******************  Bit definition for DSP_CMPBIT_SADDR register  ********************/
#define  DSP_CMPBIT_SADDR                  ((uint32_t)0xFFFFFFFF)            /*!< 最大值&最小值 起始地址   */

/*******************  Bit definition for DSP_CMPBIT_EADDR register  ********************/
#define  DSP_CMPBIT_EADDR                  ((uint32_t)0xFFFFFFFF)            /*!< 最大值&最小值 结束地址   */

/*******************  Bit definition for DSP_CMPBIT_MIN register  ********************/
#define  DSP_CMPBIT_MIN                    ((uint32_t)0xFFFFFFFF)            /*!< 设定的地址范围内最小值   */

/*******************  Bit definition for DSP_CMPBIT_MIN_INDEX register  ********************/
#define  DSP_CMPBIT_MIN_INDEX               ((uint32_t)0xFFFFFFFF)            /*!< 设定的地址范围内最小值位置   */

/*******************  Bit definition for DSP_CMPBIT_MAX register  ********************/
#define  DSP_CMPBIT_MAX                    ((uint32_t)0xFFFFFFFF)            /*!< 设定的地址范围内最大值   */

/*******************  Bit definition for DSP_CMPBIT_MAX_INDEX register  ********************/
#define  DSP_CMPBIT_MAX_INDEX               ((uint32_t)0xFFFFFFFF)            /*!< 设定的地址范围内最大值位置   */

/*******************  Bit definition for DSP_CONV_X1_X4 register  ********************/
#define  DSP_CONV_X1                        ((uint32_t)0x000000FF)            /*!< 卷积X列元素A1 (X=A,B,C,D....L,M)   */
#define  DSP_CONV_X2                        ((uint32_t)0x0000FF00)            /*!< 卷积X列元素A2 (X=A,B,C,D....L,M)   */
#define  DSP_CONV_X3                        ((uint32_t)0x00FF0000)            /*!< 卷积X列元素A3 (X=A,B,C,D....L,M)   */
#define  DSP_CONV_X4                        ((uint32_t)0xFF000000)            /*!< 卷积X列元素A4 (X=A,B,C,D....L,M)   */

/*******************  Bit definition for DSP_CONV_X5_X8 register  ********************/
#define  DSP_CONV_X5                        ((uint32_t)0x000000FF)            /*!< 卷积X列元素A5 (X=A,B,C,D....L,M)    */
#define  DSP_CONV_X6                        ((uint32_t)0x0000FF00)            /*!< 卷积X列元素A6 (X=A,B,C,D....L,M)    */
#define  DSP_CONV_X7                        ((uint32_t)0x00FF0000)            /*!< 卷积X列元素A7 (X=A,B,C,D....L,M)    */
#define  DSP_CONV_X8                        ((uint32_t)0xFF000000)            /*!< 卷积X列元素A8 (X=A,B,C,D....L,M)    */

/*******************  Bit definition for DSP_CONV_X9_X12 register  ********************/
#define  DSP_CONV_X9                        ((uint32_t)0x000000FF)            /*!< 卷积X列元素A9 (X=A,B,C,D....L,M)     */
#define  DSP_CONV_X10                       ((uint32_t)0x0000FF00)            /*!< 卷积X列元素A10(X=A,B,C,D....L,M)     */
#define  DSP_CONV_X11                       ((uint32_t)0x00FF0000)            /*!< 卷积X列元素A11(X=A,B,C,D....L,M)     */
#define  DSP_CONV_X12                       ((uint32_t)0xFF000000)            /*!< 卷积X列元素A12 X=A,B,C,D....L,M)     */

/*******************  Bit definition for DSP_CONV_X13 register  ********************/
#define  DSP_CONV_X13                       ((uint32_t)0x000000FF)            /*!< 卷积X列元素A13(X=A,B,C,D....L,M)      */

/*******************  Bit definition for DSP_CONV_CTL register  ********************/
#define  DSP_CONV_CTL_EN                    ((uint32_t)0x00000001)            /*!< 卷积计算使能                          */
#define  DSP_CONV_CTL_DIVEN                 ((uint32_t)0x00000002)            /*!< 卷积计算结果是否除以112211            */

/*******************  Bit definition for DSP_CONV_CORE_SEL register  ********************/
#define  DSP_CONV_CORE_SEL                  ((uint32_t)0x000000FF)            /*!< 选择卷积核矩阵(0~239)                 */

/*******************  Bit definition for DSP_CONV_RESULT register  ********************/
#define  DSP_CONV_RESULT                    ((uint32_t)0xFFFFFFFF)            /*!< 卷积计算结果                     */

/*******************  Bit definition for DSP_PIXEL_CMP_ADDR1 register  ********************/
#define  DSP_PIXEL_CMP_ADDR1                 ((uint32_t)0xFFFFFFFF)           /*!< 像素比对阵列地址1                     */

/*******************  Bit definition for DSP_PIXEL_CMP_ADDR2 register  ********************/
#define  DSP_PIXEL_CMP_ADDR2                 ((uint32_t)0xFFFFFFFF)           /*!< 像素比对阵列地址1                     */

/*******************  Bit definition for DSP_PIXEL_CMP_HW register  ********************/
#define  DSP_PIXEL_CMP_HW_WIDTH              ((uint32_t)0x0000FFFF)           /*!< 像素比对阵列的宽度配置                     */
#define  DSP_PIXEL_CMP_HW_HIGH               ((uint32_t)0xFFFF0000)           /*!< 像素比对阵列的高度配置                     */

/*******************  Bit definition for DSP_PIXEL_CMP_XY register  ********************/
#define  DSP_PIXEL_CMP_CY_OFFSET             ((int16_t)0xFFFF)                /*!< 像素比对阵列的y轴坐标偏移量                     */
#define  DSP_PIXEL_CMP_CX_OFFSET             ((int16_t)0xFFFF)                /*!< 像素比对阵列的x轴坐标偏移量                     */

/*******************  Bit definition for DSP_PIXEL_CMP_ANGLE register  ********************/
#define  DSP_PIXEL_CMP_ANGLE                 ((uint32_t)0x000000FF)           /*!< 像素比对阵列旋转角度                     */

/*******************  Bit definition for DSP_PIXEL_CMP_CTL register  ********************/
#define  DSP_PPIXEL_CMP_CTL_EN               ((uint32_t)0x00000001)           /*!< 像素比对使能                     */

/*******************  Bit definition for DSP_PIXEL_RESULT register  ********************/
#define  DSP_PIXEL_RESULT_COUNT               ((uint32_t)0x0000FFFF)           /*!< 像素比对阵列1,2相同的个数                     */
#define  DSP_PIXEL_RESULT_ALL                 ((uint32_t)0xFFFF0000)           /*!< 像素比对阵列1,2相交的个数                     */

/*******************  Bit definition for DSP_PIXEL_ZERO_COUNT0 register  ********************/
#define  DSP_PIXEL_ARRY2_ZERO_COUNT           ((uint32_t)0x0000FFFF)           /*!< 阵列2为0的个数                     */
#define  DSP_PIXEL_ARRY1_ZERO_COUNT           ((uint32_t)0xFFFF0000)           /*!< 阵列1为0的个数                     */

/*******************  Bit definition for DSP_PIXEL_RESULT_EVEN register  ********************/
#define  DSP_PIXEL_ALL_EVEN                   ((uint32_t)0x0000FFFF)           /*!< 像素比对偶数行列比对，阵列1,2相同的个数                     */
#define  DSP_PIXEL_COUNT_EVEN                 ((uint32_t)0xFFFF0000)           /*!< 像素比对偶数行列比对，阵列1,2相交的个数                     */

/*******************  Bit definition for DSP_PIXEL_ZERO_COUNT0_EVEN register  ********************/
#define  DSP_PIXEL_ARRY2_ZERO_COUNT_EVEN      ((uint32_t)0x0000FFFF)           /*!< 像素比对偶数行列比对，阵列2为0的个数                     */
#define  DSP_PIXEL_ARRY1_ZERO_COUNT_EVEN      ((uint32_t)0xFFFF0000)           /*!< 像素比对偶数行列比对，阵列1为0的个数                     */

/*******************  Bit definition for DSP_PIXEL_ZERO_COUNT1 register  ********************/
#define  DSP_PIXEL_ZERO_COUNT1_EVEN           ((uint32_t)0x0000FFFF)           /*!< 偶数行列比对,阵列1,2都为0个数                  */
#define  DSP_PIXEL_ZERO_COUNT1_ALL            ((uint32_t)0xFFFF0000)           /*!< 全部比对,阵列1，2都为0个数                     */


/******************************************************************************/
/*                                                                            */
/*                        DMA  Registers                                     */
/*                                                                            */
/******************************************************************************/

/*******************  Bit definition for DMACH_SADR register  ********************/
#define  DMACH_SADR                           ((uint32_t)0xFFFFFFFF)           /*!< DMA通道x数据源基地址                     */

/*******************  Bit definition for DMACH_DADR register  ********************/
#define  DMACH_DADR                           ((uint32_t)0xFFFFFFFF)           /*!< DMA通道x数据目的基地址                   */

/*******************  Bit definition for DMACH_BLKCNT register  ********************/
#define  DMACH_BLKCNT                         ((uint32_t)0x0000FFFF)           /*!< DMA通道x数据块数目配置                   */

/*******************  Bit definition for DMACH_CTL register  ********************/
#define  DMACH_CTL_EN                         ((uint32_t)0x00000001)           /*!< DMA通道x使能                             */
#define  DMACH_CTL_SINC                       ((uint32_t)0x00000002)           /*!< DMA通道x源地址自动递增使能               */
#define  DMACH_CTL_DINC                       ((uint32_t)0x00000004)           /*!< DMA通道x目的地址自动递增使能             */
#define  DMACH_CTL_SSIZE                      ((uint32_t)0x00000018)           /*!< DMA通道x数据源至DMA控制器的传输数据宽度  */
#define  DMACH_CTL_SBURST                     ((uint32_t)0x00000060)           /*!< DMA通道x数据源至控制器的传输数据突发长度 */
#define  DMACH_CTL_DSIZE                      ((uint32_t)0x00000180)           /*!< DMA通道x控制器至目的地址的传输数据宽度   */
#define  DMACH_CTL_M2M                        ((uint32_t)0x00000800)           /*!< DMA通道x内存至内存传输                   */
#define  DMACH_CTL_DIR                        ((uint32_t)0x00002000)           /*!< DMA通道x传输方向控制                     */
#define  DMACH_CTL_RT                         ((uint32_t)0x00004000)           /*!< DMA通道x自动重复传输控制                 */

/*******************  Bit definition for DMACH_CSADR register  ********************/
#define  DMACH_CSADR                          ((uint32_t)0xFFFFFFFF)           /*!< DMA通道x当前源地址寄存器                 */

/*******************  Bit definition for DMACH_CSADR register  ********************/
#define  DMACH_CSADR                          ((uint32_t)0xFFFFFFFF)           /*!< DMA通道x当前目的地址寄存器                */

/*******************  Bit definition for DMACH_TCNT register  ********************/
#define  DMACH_TCNT                           ((uint32_t)0x0000FFFF)           /*!< DMA通道x传输过程中有剩余数据单元尚未完成  */

/*******************  Bit definition for DMA_IER register  ********************/
#define  DMA_IER_CH0                          ((uint32_t)0x00000001)           /*!< 通道0数据传输中断使能                     */
#define  DMA_IER_CH1                          ((uint32_t)0x00000002)           /*!< 通道1数据传输中断使能                     */
#define  DMA_IER_CH2                          ((uint32_t)0x00000004)           /*!< 通道2数据传输中断使能                     */
#define  DMA_IER_CH3                          ((uint32_t)0x00000008)           /*!< 通道3数据传输中断使能                     */
#define  DMA_IER_MME                          ((uint32_t)0x80000000)           /*!< 配置错误中断使能                          */

/*******************  Bit definition for DMA_CLR register  ********************/
#define  DMA_CLR_CH0                          ((uint32_t)0x00000001)           /*!< 通道0数据传输中断旗标清除                 */
#define  DMA_CLR_CH1                          ((uint32_t)0x00000002)           /*!< 通道1数据传输中断旗标清除                 */
#define  DMA_CLR_CH2                          ((uint32_t)0x00000004)           /*!< 通道2数据传输中断旗标清除                 */
#define  DMA_CLR_CH3                          ((uint32_t)0x00000008)           /*!< 通道3数据传输中断旗标清除                 */
#define  DMA_CLR_MME                          ((uint32_t)0x80000000)           /*!< 配置错误中断旗标清除                      */

/*******************  Bit definition for DMA_STATUS register  ********************/
#define  DMA_STATUS_CH0_FINISH                ((uint32_t)0x00000001)           /*!< 通道0数据传输完成中断标志                 */
#define  DMA_STATUS_CH1_FINISH                ((uint32_t)0x00000002)           /*!< 通道0数据传输完成中断标志                 */
#define  DMA_STATUS_CH2_FINISH                ((uint32_t)0x00000004)           /*!< 通道0数据传输完成中断标志                 */
#define  DMA_STATUS_CH3_FINISH                ((uint32_t)0x00000008)           /*!< 通道0数据传输完成中断标志                 */
#define  DMA_STATUS_CH0_BUSY                  ((uint32_t)0x00000100)           /*!< 通道0数据传输中状态旗标                   */
#define  DMA_STATUS_CH1_BUSY                  ((uint32_t)0x00000200)           /*!< 通道1数据传输中状态旗标                   */
#define  DMA_STATUS_CH2_BUSY                  ((uint32_t)0x00000400)           /*!< 通道2数据传输中状态旗标                   */
#define  DMA_STATUS_CH3_BUSY                  ((uint32_t)0x00000800)           /*!< 通道3数据传输中状态旗标                   */
#define  DMA_STATUS_MME                       ((uint32_t)0x80000000)           /*!< 配置错误中断标志                          */

/*******************  Bit definition for DMA_CHCFG register  ********************/
#define  DMA_CHCFG_MASK                       ((uint32_t)0x0000001F)           /*!< 通道x关联外设                             */
#define  DMA_CHCFG_UART0_TX                   ((uint32_t)0x00000000)           /*!< 通道x关联UART0_TX                         */
#define  DMA_CHCFG_UART0_RX                   ((uint32_t)0x00000001)           /*!< 通道x关联UART0_RX                         */
#define  DMA_CHCFG_UART1_TX                   ((uint32_t)0x00000002)           /*!< 通道x关联UART1_TX                         */
#define  DMA_CHCFG_UART1_RX                   ((uint32_t)0x00000003)           /*!< 通道x关联UART1_RX                         */
#define  DMA_CHCFG_UART2_TX                   ((uint32_t)0x00000004)           /*!< 通道x关联UART2_TX                         */
#define  DMA_CHCFG_UART2_RX                   ((uint32_t)0x00000005)           /*!< 通道x关联UART2_RX                         */
#define  DMA_CHCFG_SPI0_TX                    ((uint32_t)0x00000006)           /*!< 通道x关联SPI0_TX                          */
#define  DMA_CHCFG_SPI0_RX                    ((uint32_t)0x00000007)           /*!< 通道x关联SPI0_RX                          */
#define  DMA_CHCFG_SPI1_TX                    ((uint32_t)0x00000008)           /*!< 通道x关联SPI1_TX                          */
#define  DMA_CHCFG_SPI1_RX                    ((uint32_t)0x00000009)           /*!< 通道x关联SPI1_RX                          */
#define  DMA_CHCFG_QSPI_TX                    ((uint32_t)0x0000000A)           /*!< 通道x关联QSPI_TX                          */
#define  DMA_CHCFG_QSPI_RX                    ((uint32_t)0x0000000B)           /*!< 通道x关联QSPI_RX                          */
#define  DMA_CHCFG_TIMER3                     ((uint32_t)0x0000000C)           /*!< 通道x关联TIMER3                           */
#define  DMA_CHCFG_TIMER4                     ((uint32_t)0x0000000D)           /*!< 通道x关联TIMER4                           */
#define  DMA_CHCFG_ADC                        ((uint32_t)0x0000000E)           /*!< 通道x关联ADC                              */


#ifdef USE_STDPERIPH_DRIVER
 #include "pt32z192xx_conf.h"
#else
  #define assert_param(expr) ((void)0)
#endif

/** @addtogroup Exported_macro
  * @{
  */
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __PT32Z192XX_H */

/**
  * @}
  */

  /**
  * @}
  */
  /**
  * @}
  */


