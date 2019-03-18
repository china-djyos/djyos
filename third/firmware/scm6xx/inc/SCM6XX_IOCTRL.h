#ifndef _SCM6XX_IOCTRL_H_
#define _SCM6XX_IOCTRL_H_

#include "SCM6XX_ALL.h"

#define P_GPIO0          0
#define P_GPIO1          1
#define P_GPIO2          2
#define P_GPIO3          3
#define P_GPIO4          4
#define P_GPIO5          5
#define P_GPIO6          6
#define P_GPIO7          7
#define P_GPIO8          8
#define P_GPIO9          9
#define P_GPIO10         10
#define P_GPIO11         11
#define P_GPIO12         12
#define P_GPIO13         13
#define P_GPIO14         14
#define P_GPIO15         15
#define P_GPIO16         16
#define P_SPI3_PCS       17
#define P_SPI3_SOUT      18
#define P_SPI3_SCK       19
#define P_SPI3_SIN       20
#define P_UART1_TX       21
#define P_UART1_RX       22
#define P_UART3_TX       23
#define P_UART3_RX       24
#define P_DRVVBUS        25
#define P_UART4_TX       26
#define P_UART4_RX       27
#define P_MII0_RXER      28
#define P_PWM0           29
#define P_PWM1           30
#define P_MII0_RXD3      31
#define P_MII0_RXD2      32
#define P_MII0_RXCLK     33
#define P_MII0_RXD1      34
#define P_MII0_RXD0      35
#define P_RMII0_CRS_DV   36
#define P_MII0_RXDV      36
#define P_MII0_TXEN      37
#define P_MII0_TXD0      38
#define P_MII0_TXD1      39
#define P_I2C2_SDA       40
#define P_I2C2_SCL       41
#define P_MII0_TXD2      42
#define P_MII0_TXCLK     43
#define P_MII0_TXD3      44
#define P_MII0_CRS       45
#define P_MII0_TXER      46
#define P_MII0_COL       47
#define P_MII0_MDIO      48
#define P_MII0_MDC       49
#define P_SPI1_PCS0      50
#define P_SPI1_SCK       51
#define P_SPI1_SOUT      52
#define P_SPI1_SIN       53
#define P_MII1_RXER      54
#define P_MII1_RXD3      55
#define P_MII1_RXD2      56
#define P_MII1_RXCLK     57
#define P_MII1_RXD1      58
#define P_MII1_RXD0      59
#define P_RMII1_CRS_DV   60
#define P_MII1_RXDV      60
#define P_MII1_TXEN      61
#define P_MII1_TXD0      62
#define P_MII1_TXD1      63
#define P_MII1_TXD2      64
#define P_MII1_TXCLK     65
#define P_MII1_TXD3      66
#define P_MII1_CRS       67
#define P_MII1_TXER      68
#define P_MII1_COL       69
#define P_MII1_MDIO      70
#define P_MII1_MDC       71
#define P_I2C1_SCL       72
#define P_I2C1_SDA       73
#define P_UART0_TX       74
#define P_UART0_RX       75
#define P_PWM2           76
#define P_PWM3           77
#define P_SPI0_PCS      78
#define P_SPI0_SCK       79
#define P_SPI0_SOUT      80
#define P_SPI0_SIN       81
#define P_UART2_RX       82
#define P_UART2_TX       83
#define P_UART6_RX       84
#define P_UART6_TX       85
#define P_I2C0_SCL       86
#define P_I2C0_SDA       87
#define P_UART7_RX       88
#define P_UART7_TX       89
#define P_SPI2_SCK       90
#define P_SPI2_SOUT      91
#define P_SPI2_SIN       92
#define P_SPI2_PCS       93
#define P_UART5_TX       94
#define P_UART5_RX       95
#define P_GPIO96         96
#define P_GPIO97         97
#define P_GPIO98         98
#define P_GPIO99         99
#define P_GPIO100        100
#define P_GPIO101        101
#define P_GPIO102        102
#define P_GPIO103        103
#define P_GPIO104        104
#define P_GPIO105        105
#define P_GPIO106        106
#define P_GPIO107        107
#define P_GPIO108        108
#define P_GPIO109        109
#define P_GPIO110        110
#define P_GPIO111        111
#define P_GPIO112        112
#define P_GPIO113        113
/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void MII0_InitPad(void);
void RMII0_InitPad(void);
void MII1_InitPad(void);
void RMII1_InitPad(void);
void OTG_InitPad(void);
void SRAM_EnPad(FunctionalState opt);
void MCO_InitPad(void);
void PWM0_InitPad(void);
void PWM1_InitPad(void);
void PWM2_InitPad(void);
void PWM3_InitPad(void);
void UART0_EnIrda(FunctionalState opt);
void UART0_InitPad(void);
void UART1_InitPad(void);
void UART2_InitPad(void);
void UART3_InitPad(void);
void UART4_InitPad(void);
void UART5_InitPad(void);
void UART6_InitPad(void);
void UART7_InitPad(void);
void SPI0_InitPad(void);
void SPI1_InitPad(void);
void SPI2_InitPad(void);
void SPI3_InitPad(FunctionalState opt);
void I2C0_InitPad(void);
void I2C1_InitPad(void);
void I2C2_InitPad(void);
void SCI0_InitPad(void);
void SCI1_InitPad(void);
void CAN0_InitPad(void);
void CAN1_InitPad(void);
void EXTAL0_InitPad(void);
void XTAL0_InitPad(void);
void EXTAL32_InitPad(void);
void XTAL32_InitPad(void);
void CLKIN_InitPad(void);
void CLKIN32K_InitPad(void);
void JTAG_InitPad(void);
void GPIO346_InitPad(void);
void GPIO1516_InitPad(void);
void GPIO1720_InitPad(void);
void GPIO2122_InitPad(void);
void GPIO2324_InitPad(void);
void GPIO25_InitPad(void);
void GPIO2627_InitPad(void);
void GPIO28_InitPad(void);
void GPIO29_InitPad(void);
void GPIO30_InitPad(void);
void GPIO3133_InitPad(void);
void GPIO3439_InitPad(void);
void GPIO4041_InitPad(void);
void GPIO4249_InitPad(void);
void GPIO5053_InitPad(void);
void GPIO5457_InitPad(void);
void GPIO5863_InitPad(void);
void GPIO6465_InitPad(void);
void GPIO6669_InitPad(void);
void GPIO7071_InitPad(void);
void GPIO72_InitPad(void);
void GPIO73_InitPad(void);
void GPIO7475_InitPad(void);
void GPIO76_InitPad(void);
void GPIO77_InitPad(void);
void GPIO7879_InitPad(void);
void GPIO8081_InitPad(void);
void GPIO8485_InitPad(void);
void GPIO8687_InitPad(void);
void GPIO8889_InitPad(void);
void GPIO9093_InitPad(void);
void GPIO9495_InitPad(void);
void GPIO109_InitPad(void);
void GPIO110_InitPad(void);
void GPIO111_InitPad(void);
void GPIO112_InitPad(void);
void GPIO113_InitPad(void);
void ANA_EnTest(FunctionalState opt);
void ADC_EnTest(FunctionalState opt);
void SPI0_ConfigDS(UINT32 SPI0_Ds);
void SPI1_ConfigDS(UINT32 SPI1_Ds);
void SPI2_ConfigDS(UINT32 SPI2_Ds);
void SPI3_ConfigDS(UINT32 SPI3_Ds);
void MII0_ConfigDS(UINT32 MII0_Ds);
void MII1_ConfigDS(UINT32 MII1_Ds);
void IO_Configure_PullUp(UINT8 IO_No, FunctionalState opt);
void IO_Configure_PullDown(UINT8 IO_No, FunctionalState opt);

#endif 

