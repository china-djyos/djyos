#ifndef __SYS_CLOCK_H__
#define __SYS_CLOCK_H__

//#include "sys_types.h"

//----------------------------------------
#define USB_OTG_CLOCK               (0x000 | 24)
#define SPI1_CLOCK                  (0x000 | 21)
#define SPI0_CLOCK                  (0x000 | 20)
#define SDRAM   _CLOCK              (0x000 | 14)
#define SD1_CLOCK                   (0x000 | 9)
#define SD0_CLOCK                   (0x000 | 8)
#define DMA_CLOCK                   (0x000 | 6)
//----------------------------------------
#define DEFE_CLOCK                  (0x100 | 14)
#define DEBE_CLOCK                  (0x100 | 12)
#define TVE_CLOCK                   (0x100 | 10)
#define TVD_CLOCK                   (0x100 | 9)
#define CSI_CLOCK                   (0x100 | 8)
#define DEINTERLACE_CLOCK           (0x100 | 5)
#define LCD_CLOCK                   (0x100 | 4)
#define VE_CLOCK                    (0x100 | 0)
//----------------------------------------
#define UART2_CLOCK                 (0x200 | 22)
#define UART1_CLOCK                 (0x200 | 21)
#define UART0_CLOCK                 (0x200 | 20)
#define TWI2_CLOCK                  (0x200 | 18)
#define TWI1_CLOCK                  (0x200 | 17)
#define TWI0_CLOCK                  (0x200 | 16)
#define DAUDIO_CLOCK                (0x200 | 12)
#define RSB_CLOCK                   (0x200 | 3)
#define CIR_CLOCK                   (0x200 | 2)
#define OWA_CLOCK                   (0x200 | 1)
#define AUDIO_CODEC_CLOCK           (0x200 | 0)
//----------------------------------------

void Sys_Clock_Init(int cpu_frequency);/*设置系统频率*/
#define sys_clock_init Sys_Clock_Init

unsigned int get_pll_periph_frequency(void);
unsigned int get_cpu_frequency(void);
unsigned int get_ahb_frequency(void);
unsigned int get_apb_frequency(void);
unsigned int get_hclk_frequency(void);


void Open_Dev_Clock(int Dev);
void Close_Dev_Clock(int Dev);

#endif
