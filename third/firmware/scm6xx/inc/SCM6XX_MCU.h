#ifndef __SCM6XX_H__
#define __SCM6XX_H__

#ifdef SCM6XX
extern "C" {
#endif




/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,      /*!<  3 HardFault Interrupt             */



  SVCall_IRQn                   =  -5,      /*!< 11 SV Call Interrupt               */

  PendSV_IRQn                   =  -2,      /*!< 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,      /*!< 15 System Tick Interrupt           */

/* ----------------------  SM1226A01 Specific Interrupt Numbers  ------------------ */

  SCU_IRQn                      =   0,      /*!<                                    */
  WDT_IRQn                      =   1,      /*!<                                    */
  IIC0_IRQn                     =   2,      /*!<                                    */ 
  IIC1_IRQn                     =   3,      /*!<                                    */
  IIC2_IRQn                     =   4,      /*!<                                    */
  UART0_IRQn                    =   5,      /*!<                                    */
  UART1_IRQn                    =   6,      /*!<                                    */
  UART2_IRQn                    =   7,      /*!<                                    */
  UART3_IRQn                    =   8,      /*!<                                    */
  UART4_IRQn                    =   9,      /*!<                                    */
  UART5_IRQn                    =   10,      /*!<                                    */
  UART6_IRQn                    =   11,      /*!<                                    */
  UART7_IRQn                    =   12,      /*!<                                    */
  SPI0_IRQn                     =   13,      /*!<                                    */
  SPI1_IRQn                     =   14,      /*!<                                    */
  SPI2_IRQn                     =  15,      /*!<                                    */
  SPI3_IRQn                     =  16,      /*!<                                    */ 
  TIMERA0_IRQn                      =  17,      /*!<                                    */
  TIMERA1_IRQn                      =  18,      /*!<                                    */
  TIMERB0_IRQn                      =  19,      /*!<                                    */
  TIMERB1_IRQn                      =  20,      /*!<                                    */
  TIMERC0_IRQn                      =  21,      /*!<                                    */
  TIMERC1_IRQn                      =  22,      /*!<                                    */
  TIMERD0_IRQn                      =  23,      /*!<                                    */
  TIMERD1_IRQn                      =  24,      /*!<                                    */
  DMA00_IRQn                     =  25,      /*!<                                    */
  DMA01_IRQn                     =  26,      /*!<                                    */
  DMA02_IRQn                     =  27,      /*!<                                    */
  DMA03_IRQn                     =  28,      /*!<                                    */
  DMA10_IRQn                     =  29,      /*!<                                    */
  DMA11_IRQn                     =  30,      /*!<                                    */
  DMA12_IRQn                     =  31,      /*!<                                    */
  DMA13_IRQn                     =  32,      /*!<                                    */
  DMA20_IRQn                     =  33,      /*!<                                    */
  DMA21_IRQn                     =  34,      /*!<                                    */
  DMA22_IRQn                     =  35,      /*!<                                    */
  DMA23_IRQn                     =  36,      /*!<                                    */
  DMA30_IRQn                     =  37,      /*!<                                    */
  DMA31_IRQn                     =  38,      /*!<                                    */
  DMA32_IRQn                     =  39,      /*!<                                    */
  DMA33_IRQn                     =  40,      /*!<                                    */
  ALM_IRQn                      =  41,      /*!<                                    */
  PRD_IRQn                      =  42,      /*!<                                    */
  ANACTL_IRQn                   =  43,      /*!<                                    */
  GPIO_IRQn                     =  44,      /*!<                                    */
  OTG_IRQn                      =  45,      /*!<                                    */
  GMAC0_IRQn                    =  46,      /*!<                                    */
  GMAC1_IRQn                    =  47,      /*!<                                    */
  U7816_IRQn                    =  47,      /*!<                                    */
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M0 Processor and Core Peripherals  ------- */
#define __CM4_REV                 0x0000    /*!< Core revision r0p0                              */
#define __MPU_PRESENT             0         /*!< MPU present or not                              */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels         */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used    */

#ifndef __FPU_PRESENT
#define __FPU_PRESENT 1
#endif

#include <core_cm4.h>                       /* Processor and core peripherals                    */

//#ifndef SCM6XX
//typedef enum {EN_FALSE = 0, EN_TRUE = !EN_FALSE} bool;
//#endif

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif



/* ================================================================================ */
/* ================            IOCTRL UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t IO_CTRL1;
  __IO uint32_t IO_CTRL2;
  __IO uint32_t IO_CTRL3;
  __I uint32_t RESERVED0;
  __IO uint32_t IO_DS;
  __I  uint32_t RESERVED1[3];
  __IO uint32_t IO_PU1;
  __IO uint32_t IO_PU2;
  __IO uint32_t IO_PU3;
  __IO uint32_t IO_PU4;
  __IO uint32_t IO_PD1;
  __IO uint32_t IO_PD2;
  __IO uint32_t IO_PD3;
  __IO uint32_t IO_PD4;
} SGCC_IOCTRL_TypeDef;


/* ================================================================================ */
/* ================            FLASH UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __O uint32_t PROG_START; //0x0
  __O uint32_t SECTOR_ERASE_START; //0x4
  __O uint32_t CHIP_ERASE_START; //0x8
  __I  uint32_t RESERVED0[5];
  __IO uint32_t STANDBY_EN; //0x20
  __I  uint32_t RESERVED1[7];
  __IO uint32_t PROG_ERASE_FAIL;//0x40
  __IO uint32_t REDUNDANCY_INFO; //0x44
  __I uint32_t FMC_STATUS;  //0x48
  __I  uint32_t RESERVED2[13];
  __IO uint32_t T_1US_REF; //0x80
  __IO uint32_t T_RW_CNT; //0x84
  __IO uint32_t T_AEPH_CNT; //0x88
  __IO uint32_t T_AA_CNT; //0x8c
  __IO uint32_t T_APH_CNT; //0x90
  __IO uint32_t T_DS_CNT; //0x94
  __IO uint32_t T_HV_US_CNT; //0x98
  __IO uint32_t T_NVSP_US_CNT; //0x9c
  __IO  uint32_t T_PGS_US_CNT; //0xA0
  __IO uint32_t T_PROG_US_CNT; //0xA4
  __IO uint32_t T_RCVP_US_CNT; //0xA8
  __IO uint32_t T_SERASE_US_CNT; //0xAC
  __IO uint32_t T_CERASE_US_CNT; //0xB0
  __IO uint32_t T_NVSC_US_CNT; //0xB4
  __IO uint32_t T_RCVE_US_CNT; //0xB8
  __I  uint32_t RESERVED3[113];
  __O uint32_t FMC_PROTECT; //0x280
  
} SGCC_FMC_TypeDef;

/* ================================================================================ */
/* ================            ANALOGCTROL UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t ANA_CFG_REG; //0x0
  __IO uint32_t ANA_STAT_REG; //0x4
  __IO uint32_t ADC_TRIM_REG; //0x8
  __IO uint32_t ADC_RDY_REG;//0xc
  __IO uint32_t ADC_CON1_REG;//0x10
  __IO uint32_t ADC_CON2_REG;//0x14
  __IO uint32_t ADC_CON3_REG;//0x18
  __IO uint32_t ADC_CON4_REG;//0x1c
  __I  uint32_t RESERVED1[4]; 
  __IO uint32_t BGA01_REG;  //0x30  
  __IO uint32_t BGA02_REG ;//0x34
  __IO uint32_t BGB01_REG;  //0x38  
  __IO uint32_t BGB02_REG ;//0x3c
  __IO uint32_t VR12_1_REG ;//0x40
  __IO uint32_t VR12_2_REG ;//0x44
  __IO uint32_t VR16_REG ;//0x48
  __I  uint32_t RESERVED2;//0x4c
  __IO uint32_t VD33_REG ;//0x50
  __IO uint32_t LOSC_REG ;//0x54
  __IO uint32_t HOSC_REG ;//0x58
  __IO uint32_t XLOSC_REG ;//0x5c
  __IO uint32_t XHOSC_REG ;//0x60
  __IO uint32_t ANATEST_REG ;//0x64
  __I  uint32_t RESERVED3[6];//0x68~0x7c
  __I  uint32_t ADC_DATA_REG0; //0x80
  __I  uint32_t ADC_DATA_REG1; //0x84
  __I  uint32_t ADC_DATA_REG2; //0x88
  __I  uint32_t ADC_DATA_REG3; //0x8c
  __I  uint32_t ADC_DATA_REG4; //0x90
  __I  uint32_t ADC_DATA_REG5; //0x94
  __I  uint32_t ADC_DATA_REG6; //0x98
  __I  uint32_t ADC_DATA_REG7; //0x9c
  __I  uint32_t ADC_DATA_REG8; //0xA0
  __I  uint32_t ADC_DATA_REG9; //0xA4
  __I  uint32_t ADC_DATA_REG10; //0xA8
  __I  uint32_t ADC_DATA_REG11; //0xAC
  __I  uint32_t RESERVED4[19];//0xB0~0xF8
  __IO uint32_t trimProtect;
} SGCC_ANACTL_TypeDef;

/* ================================================================================ */
/* ================            RTC UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __O uint32_t Time0;
  __O uint32_t Time1;
  __I uint32_t CurTime0;
  __I uint32_t CurTime1;
  __IO uint32_t AlarmTime0;
  __IO uint32_t AlarmTime1;
  __I  uint32_t RESERVED0[2];
  __IO uint32_t Control;
  __IO uint32_t Status;
} SGCC_RTC_TypeDef;

/* ================================================================================ */
/* ================    SYS CONTROL UNIT (OSC PMU and RESET)        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SYS_CTRL;
  __IO uint32_t SYS_STS;
  __IO uint32_t INT_EN;
  __IO uint32_t INT_STS;
  __IO uint32_t CACHE_HIT;
  __IO uint32_t CACHE_MISS;
  __IO uint32_t WAKEUP_EN;  
} SGCC_SCU_TypeDef;

/* ================================================================================ */
/* ================    CLK CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SYSCLK_CTRL;
  __IO uint32_t HCLK_EN;
  __IO uint32_t PCLK0_EN;
  __IO uint32_t PCLK1_EN;
  __IO uint32_t PLL_CTRL;
} SGCC_CLK_TypeDef;

/* ================================================================================ */
/* ================    RESET CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SOFT_RST0;
  __IO uint32_t SOFT_RST1;
  __IO uint32_t SOFT_RST2;
  __IO uint32_t PER_RST;
  __IO uint32_t RST_STS;
} SGCC_RESET_TypeDef;


/* ================================================================================ */
/* ================            PIN_CTRL Multiplex UNIT                 ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t IO_CTRL1;
  __IO uint32_t IO_CTRL2;
  __IO uint32_t IO_CTRL3;
  __IO uint32_t RESERVED;
  __IO uint32_t IO_DS;
} SGCC_PINCTRL_TypeDef;




/*--------------------- UART ---------------------*/
typedef struct {                            
   __O  uint32_t TXDATA;
   __I  uint32_t RXDATA;
   __IO uint32_t STATUS;
   __IO uint32_t CTRL;
   __IO uint32_t INTSTA;
   __IO uint32_t BAUDDIV;
} SGCC_UART_TypeDef;

/* ================================================================================ */
/* ================            U7816 UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t BDR;
  __IO uint32_t CHNL;
  __IO uint32_t DFRM;
  __IO uint32_t DATA;
  __IO uint32_t FIFO;
  __IO  uint32_t SR;
  __IO uint32_t INTEN;
  __IO  uint32_t INTFLAG;
} SGCC_U7816_TypeDef;

/*--------------------- DMA ---------------------*/
typedef struct {                         
   __IO  uint32_t TRANS_CTRL;
   __IO  uint32_t TRANS_FMT1;
   __IO uint32_t TRANS_FMT2_STAT2;
   __IO  uint32_t TRANS_STAT1;
   __IO uint32_t TRANS_SRC_ADDR;
   __IO uint32_t TRANS_DST_ADDR;
   __IO uint32_t TRIGEN0_REG;
   __IO uint32_t TRIGEN1_REG;
} SGCC_DMA_TypeDef;



/*--------------------- WDT ---------------------*/
typedef struct {                            /*!< (@ 0x50500000) WDT Structure          */
  __IO uint32_t CTRL;                       /*!< (@ 0x50500000) WDT Control Register.  */
  __O  uint32_t CLR;                        /*!< (@ 0x50500004) WDT Clear Register.  */
} SGCC_WDT_TypeDef;

/*--------------------- T/C ---------------------*/
typedef struct {                            /*!< (@ 0x50600000/ 0x50600040/ 0x50600080/0x506000c0) T/C Structure          */
  __IO uint32_t FSR;                        /*!< (@ 0x50600000) T/C Function Select Register.  */
  __IO uint32_t CR;                         /*!< (@ 0x50600004) T/C Control Register.  */
  __IO uint32_t TC;                         /*!< (@ 0x50600008) T/C Timer Counter Register.  */
  __IO uint32_t PR;                         /*!< (@ 0x5060000C) T/C Prescale Register.  */
  __IO uint32_t PC;                         /*!< (@ 0x50600010) T/C Prescale CountervRegister.  */
  __IO uint32_t MR;                         /*!< (@ 0x50600014) T/C Match Register.  */
  __IO uint32_t MCR;                        /*!< (@ 0x50600018) T/C Match Control Register.  */
  __IO uint32_t EMR;                        /*!< (@ 0x5060001C) T/C Match Out Control Register.  */
  __IO uint32_t EWCR;                       /*!< (@ 0x50600020) T/C Match Out Pulse Width Register.  */
  __IO uint32_t CAPTR  ;                    /*!< (@ 0x50600024) T/C Capture Register.  */
  __IO uint32_t CCR;                        /*!< (@ 0x50600028) T/C Capture Control Register.  */
  __IO uint32_t INTIF;                      /*!< (@ 0x5060002C) T/C Interrupt Flag Register.  */
  __IO uint32_t INTIE;                      /*!< (@ 0x50600030) T/C Interrupt Enable Register.  */
} SGCC_TC_TypeDef;



/*--------------------- PWM ---------------------*/
typedef struct {                            /*!< (@ 0x50F00000/ 0x50F00010/ 0x50F00020) PWM Structure          */
  __IO uint32_t DIV;                        /*!< (@ 0x50500000) PWM Division Register.  */
  __IO uint32_t PERIOD;                     /*!< (@ 0x50500004) PWM Period Register.  */
  __IO uint32_t WIDTH;                      /*!< (@ 0x50500008) PWM Width Register.  */
  __IO uint32_t CONTROL;                    /*!< (@ 0x5050000C) PWM Control Register.  */
} SGCC_PWM_TypeDef;

/* ================================================================================ */
/* ================            SPI UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SPCR0;
  __IO uint32_t SPCR1;
  __IO uint32_t SPCR2;
  __IO uint32_t SPCR3;
  __I  uint32_t SPSR;
  __IO uint32_t SPWAR;
  __IO uint32_t SPDR;
  __IO uint32_t SPNUMR;
  __IO uint32_t SPINTSR;
  __IO uint32_t SPI_SCKDIV;
} SGCC_SPI_TypeDef;

/* ================================================================================ */
/* ================            I2C UNIT                            ================ */
/* ================================================================================ */
/*--------------------- I2C ---------------------*/
typedef struct {                            /*!<          */
  __IO uint32_t CFGR;                         /*!< (@ 0x50100000) I2C ????? */
  __IO  uint32_t CLKR;                         /*!< (@ 0x50100004) I2C ????? */
  __IO uint32_t ADR;                        /*!< (@ 0x50100008) I2C ?????? */
  __IO uint32_t CMDR;                         /*!< (@ 0x5010000C) I2C ?????  */
  __IO uint32_t SR;                       /*!< (@ 0x50100014) I2C ?????????? */
  __IO  uint32_t BUFR;                        /*!< (@ 0x50100018) ??????? */
} SGCC_I2C_TypeDef;

/*--------------------- GPIO ---------------------*/
typedef struct {                            /*  GPIO Structure          */
  __IO uint32_t DR;                    /*offset[0x0] Port A Data Register  */
  __IO uint32_t DIR;                    /*offset[0x4] Port A Data Direction Register  */
  __IO uint32_t INTER;                  /*offset[0x8] Interrupt Enable Register  */
  __IO  uint32_t INTMR;                 /*offset[0xC] Interrupt Mask Register */
  __IO  uint32_t INTSR;                 /*offset[0x10] Interrupt status register  */
  __IO uint32_t DBSYR;                 /*offset[0x14] Debounce and Syn Register  */
  __IO  uint32_t INTCR;                 /*offset[0x18] Interrupt Clear Register  */
  __IO  uint32_t ETPR;                  /*offset[0x1c] External Port A Register  */
  __IO uint32_t PULLEN;                  /*offset[0x20] Pullenable Regsiter  */
  __IO uint32_t PULLTYPE;                  /*offset[0x24] Pulletype Regsiter  */

} SGCC_GPIO_TypeDef;


/*--------------------- CRC ---------------------*/
typedef struct {                            /*!< (@ 0x50300000) CRC Structure          */
  __IO uint32_t CTRL;                       /*!< (@ 0x50300000) CRC ÔËËã¿ØÖÆ¼Ä´æÆ÷.  */
  __I  uint32_t STAT;                        /*!< (@ 0x50300004) CRC ÔËËã×´Ì¬¼Ä´æÆ÷.  */
  __I uint32_t RESU;                       /*!< (@ 0x50300008)  CRC Éú³É½á¹û Register.  */
  union
  {
      __IO  uint32_t DATA_WORD;                        /*!< (@ 0x5030000C) CRC ????? Register.  */
      __IO  uint16_t DATA_HALFW[2];
      __IO  uint8_t DATA_BYTE[4];
      
  }DATA;
} SGCC_CRC_TypeDef;

/** 
  * @brief Ethernet MAC
  */

typedef struct
{
  __IO uint32_t MACCR;  //0x0
  __IO uint32_t MACFFR; //0x4
  __IO uint32_t MACHTHR; //0x8
  __IO uint32_t MACHTLR; //0xC
  __IO uint32_t MACMIIAR; //0x10
  __IO uint32_t MACMIIDR; //0x14
  __IO uint32_t MACFCR;   //0x18
  __IO uint32_t MACVLANTR;      //0x1C       /*    8 */
       uint32_t RESERVED0[2];
  __IO uint32_t MACRWUFFR;      //0x28       /*   11 */
  __IO uint32_t MACPMTCSR;      //0x2C
  __IO uint32_t MACLPICSR;      //0x30
       uint32_t MACLPITCR;      //0x34
  __IO uint32_t MACSR;          //0x38       /*   15 */
  __IO uint32_t MACIMR;         //0x3C
  __IO uint32_t MACA0HR;        //0x40
  __IO uint32_t MACA0LR;   //0x44
  __IO uint32_t MACA1HR;   //0x48
  __IO uint32_t MACA1LR;   //0x4C
  __IO uint32_t MACA2HR;   //0x50
  __IO uint32_t MACA2LR;   //0x54
  __IO uint32_t MACA3HR;   //0x58
  __IO uint32_t MACA3LR;   //0x5C            /*   24 */
  __IO uint32_t MACA4HR;   //0x60
  __IO uint32_t MACA4LR;   //0x64
  __IO uint32_t MACA5HR;   //0x68
  __IO uint32_t MACA5LR;   //0x6C
  __IO uint32_t MACA6HR;   //0x70
  __IO uint32_t MACA6LR;   //0x74
  __IO uint32_t MACA7HR;   //0x78
  __IO uint32_t MACA7LR;   //0x7C          
  __IO uint32_t MACA8HR;   //0x80
  __IO uint32_t MACA8LR;   //0x84
  __IO uint32_t MACA9HR;   //0x88
  __IO uint32_t MACA9LR;   //0x8C
  __IO uint32_t MACA10HR;  //0x90
  __IO uint32_t MACA10LR;  //0x94
  __IO uint32_t MACA11HR;  //0x98
  __IO uint32_t MACA11LR;  //0x9C           
  __IO uint32_t MACA12HR;  //0xA0
  __IO uint32_t MACA12LR;  //0xA4
  __IO uint32_t MACA13HR;  //0xA8
  __IO uint32_t MACA13R;   //0xAC
  __IO uint32_t MACA14HR;  //0xB0
  __IO uint32_t MACA14LR;  //0xB4
  __IO uint32_t MACA15HR;  //0xB8
  __IO uint32_t MACA15LR;  //0xBC           
  __IO uint32_t MACANCR;   //0xC0
  __IO uint32_t MACANSR;   //0xC4
  __IO uint32_t MACANAR;   //0xC8
  __IO uint32_t MACANLPAR; //0xCC
  __IO uint32_t MACANER;  //0xD0
  uint32_t RESERVED2[11];
  __IO uint32_t MMCCR;     //0x100            /*   65 */
  __IO uint32_t MMCRIR;    //0x104
  __IO uint32_t MMCTIR;    //0x108
  __IO uint32_t MMCRIMR;   //0x10C
  __IO uint32_t MMCTIMR;   //0x110            /*   69 */
       uint32_t RESERVED3[14];
  __IO uint32_t MMCTGFSCCR;            /*   84 */
  __IO uint32_t MMCTGFMSCCR;
       uint32_t RESERVED4[5];
  __IO uint32_t MMCTGFCR;
       uint32_t RESERVED5[10];
  __IO uint32_t MMCRFCECR;
  __IO uint32_t MMCRFAECR;
       uint32_t RESERVED6[10];
  __IO uint32_t MMCRGUFCR;
       uint32_t RESERVED7[334];
  __IO uint32_t PTPTSCR;
  __IO uint32_t PTPSSIR;
  __IO uint32_t PTPTSHR;
  __IO uint32_t PTPTSLR;
  __IO uint32_t PTPTSHUR;
  __IO uint32_t PTPTSLUR;
  __IO uint32_t PTPTSAR;
  __IO uint32_t PTPTTHR;
  __IO uint32_t PTPTTLR;
       uint32_t RESERVED8[567];
  __IO uint32_t DMABMR;
  __IO uint32_t DMATPDR;
  __IO uint32_t DMARPDR;
  __IO uint32_t DMARDLAR;
  __IO uint32_t DMATDLAR;
  __IO uint32_t DMASR;
  __IO uint32_t DMAOMR;
  __IO uint32_t DMAIER;
  __IO uint32_t DMAMFBOCR;
       uint32_t RESERVED9[9];
  __IO uint32_t DMACHTDR;
  __IO uint32_t DMACHRDR;
  __IO uint32_t DMACHTBAR;
  __IO uint32_t DMACHRBAR;
} ETH_TypeDef;

/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */



#define SGCC_CLK_BASE             (0x40000000UL)
#define SGCC_I2C0_BASE            (0x40200000UL)
#define SGCC_I2C1_BASE            (0x40210000UL)
#define SGCC_UART0_BASE           (0x40300000UL)
#define SGCC_UART1_BASE           (0x40310000UL)
#define SGCC_UART2_BASE           (0x40320000UL)
#define SGCC_UART3_BASE           (0x40330000UL)
#define SGCC_PWMA_BASE            (0x40400000UL)
#define SGCC_TIMERA0_BASE          (0x40500000UL)
#define SGCC_TIMERA1_BASE          (0x40500040UL)
#define SGCC_TIMERB0_BASE          (0x40510000UL)
#define SGCC_TIMERB1_BASE          (0x40510040UL)
#define SGCC_SPI0_BASE            (0x40600000UL)
#define SGCC_SPI1_BASE            (0x40610000UL)
#define SGCC_WDT_BASE             (0x40700000UL)
#define SGCC_RTC_BASE             (0x40900000UL)
#define SGCC_GPIO_BASE            (0x40A00000UL)
#define SGCC_TEST_BASE            (0x40B00000UL)
#define SGCC_IOCTRL_BASE          (0x40C00000UL)
#define SGCC_U7816_BASE           (0x40D00000UL)

#define SGCC_RESET_BASE           (0x41100000UL)
#define SGCC_I2C2_BASE            (0x41220000UL)
#define SGCC_UART4_BASE           (0x41340000UL)
#define SGCC_UART5_BASE           (0x41350000UL)
#define SGCC_UART6_BASE           (0x41360000UL)
#define SGCC_UART7_BASE           (0x41370000UL)
#define SGCC_PWMB_BASE            (0x41410000UL)
#define SGCC_TIMERC0_BASE          (0x41520000UL)
#define SGCC_TIMERC1_BASE          (0x41520040UL)
#define SGCC_TIMERD0_BASE          (0x41530000UL)
#define SGCC_TIMERD1_BASE          (0x41530040UL)

#define SGCC_SPI2_BASE            (0x41620000UL)
#define SGCC_SPI3_BASE            (0x41630000UL)
#define SGCC_CRC_BASE             (0x41800000UL)
#define SGCC_ANA_BASE             (0x41C00000UL)
#define SGCC_SCU_BASE             (0x41d00000UL)
#define SGCC_CACHE_BASE           (0x41f00000UL)

#define SGCC_OTG_BASE             (0x50000000UL)
#define SGCC_DMA0_BASE            (0x50100000UL)
#define SGCC_DMA1_BASE            (0x50110000UL)
#define SGCC_DMA2_BASE            (0x50120000UL)
#define SGCC_DMA3_BASE            (0x50130000UL)

#define SGCC_GMAC0_BASE           (0x50200000UL)
#define SGCC_GMAC1_BASE           (0x50210000UL)

#define ETH_MAC_BASE          (SGCC_GMAC1_BASE)
#define ETH_MMC_BASE          (SGCC_GMAC1_BASE + 0x0100)
#define ETH_PTP_BASE          (SGCC_GMAC1_BASE + 0x0700)
#define ETH_DMA_BASE          (SGCC_GMAC1_BASE + 0x1000)

#define SGCC_FMC_BASE        (0x208000UL)




/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define SGCC_FMC_P            ((     SGCC_FMC_TypeDef *)  SGCC_FMC_BASE)
#define SGCC_CRC_P            ((     SGCC_CRC_TypeDef *)  SGCC_CRC_BASE)

#define SGCC_CLK_P            ((     SGCC_CLK_TypeDef *)  SGCC_CLK_BASE)
#define SGCC_I2C0_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C0_BASE)
#define SGCC_I2C1_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C1_BASE)
#define SGCC_UART0_P          ((     SGCC_UART_TypeDef *)  SGCC_UART0_BASE)
#define SGCC_UART1_P          ((     SGCC_UART_TypeDef *)  SGCC_UART1_BASE)
#define SGCC_UART2_P          ((     SGCC_UART_TypeDef *)  SGCC_UART2_BASE)
#define SGCC_UART3_P          ((     SGCC_UART_TypeDef *)  SGCC_UART3_BASE)
#define SGCC_PWMA0_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWMA_BASE))
#define SGCC_PWMA1_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWMA_BASE + 0x10))
#define SGCC_TIMERA0_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERA0_BASE)
#define SGCC_TIMERA1_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERA1_BASE)
#define SGCC_TIMERB0_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERB0_BASE)
#define SGCC_TIMERB1_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERB1_BASE)
#define SGCC_SPI0_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI0_BASE)
#define SGCC_SPI1_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI1_BASE)
#define SGCC_WDT_P            ((     SGCC_WDT_TypeDef *)  SGCC_WDT_BASE)
#define SGCC_RTC_P            ((     SGCC_RTC_TypeDef *)  SGCC_RTC_BASE)
#define SGCC_GPIO_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO_BASE)
#define SGCC_IOCTRL_P         ((     SGCC_IOCTRL_TypeDef *)  SGCC_IOCTRL_BASE)
#define SGCC_TEST_P           ((     SGCC_TEST_TypeDef *)  SGCC_TEST_BASE)

#define SGCC_RESET_P          ((     SGCC_RESET_TypeDef *)  SGCC_RESET_BASE)
#define SGCC_I2C2_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C2_BASE)
#define SGCC_UART4_P          ((     SGCC_UART_TypeDef *)  SGCC_UART4_BASE)
#define SGCC_UART5_P          ((     SGCC_UART_TypeDef *)  SGCC_UART5_BASE)
#define SGCC_UART6_P          ((     SGCC_UART_TypeDef *)  SGCC_UART6_BASE)
#define SGCC_UART7_P          ((     SGCC_UART_TypeDef *)  SGCC_UART7_BASE)
#define SGCC_PWMB0_P           ((     SGCC_PWM_TypeDef *)  SGCC_PWMB_BASE)
#define SGCC_PWMB1_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWMB_BASE + 0x10))

#define SGCC_TIMERC0_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERC0_BASE)
#define SGCC_TIMERC1_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERC1_BASE)
#define SGCC_TIMERD0_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERD0_BASE)
#define SGCC_TIMERD1_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERD1_BASE)

#define SGCC_SPI2_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI2_BASE)
#define SGCC_SPI3_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI3_BASE)
#define SGCC_CRC_P            ((     SGCC_CRC_TypeDef *)  SGCC_CRC_BASE)
#define SGCC_ANACTL_P         ((     SGCC_ANACTL_TypeDef *)  SGCC_ANA_BASE)
#define SGCC_SCU_P            ((     SGCC_SCU_TypeDef *)  SGCC_SCU_BASE)
#define SGCC_CACHE_P           ((UINT32*)(SGCC_CACHE_BASE))

#define SGCC_OTG_P            ((     SGCC_OTG_TypeDef *)  SGCC_OTG_BASE)
#define SGCC_DMA0_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA0_BASE)
#define SGCC_DMA1_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA1_BASE)
#define SGCC_DMA2_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA2_BASE)
#define SGCC_DMA3_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA3_BASE)

#define SGCC_GMAC0_P          ((     ETH_TypeDef *)  SGCC_GMAC0_BASE)
#define SGCC_GMAC1_P          ((     ETH_TypeDef *)  SGCC_GMAC1_BASE)

#define SGCC_U7816_P              ((   SGCC_U7816_TypeDef *)  SGCC_U7816_BASE)

#define SGCC_FMC_P           ((     SGCC_FMC_TypeDef *)  SGCC_FMC_BASE)



#ifdef SCM6XX
}
#endif

#endif  /* __SMSCM6XXA01_H__ */
