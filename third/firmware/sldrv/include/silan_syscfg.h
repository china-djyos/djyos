/*
 * silan_syscfg.h
 */

#ifndef __SILAN_SYSCFG_H__
#define __SILAN_SYSCFG_H__

#include "silan_types.h"
#include "silan_syscfg_regs.h"

typedef enum {
	CLK_OFF = 0,
	CLK_ON
} silan_clk_onff_t;


typedef enum {
	CLKSYS_DIV1 = 0,
	CLKSYS_DIV2,
	CLKSYS_DIV3,
	CLKSYS_DIV4
} silan_presys_clksel_t;

/*
silan mcu cclk config
*/
typedef enum{
	MCU_CCLK_DIV1=0,
	MCU_CCLK_DIV2,
	MCU_CCLK_DIV4
}silan_mcu_cclksel_t;

void silan_otp_cclk_config(int clksel, silan_clk_onff_t onoff);
void silan_dsp_cclk_onoff(silan_clk_onff_t onoff);
void silan_risc_cclk_onoff(silan_clk_onff_t onoff);
void silan_dsp_boot_from(int media);
void silan_risc_boot(void);
void silan_dsp_reboot(void);
void silan_risc_reboot(void);

inline int *ptr_dma_to_cpu(int *addr);
inline int *ptr_cpu_to_dma(int *addr);
inline u32 cpu_to_dma(u32 addr);
inline u32 dma_to_cpu(u32 addr);
inline dcache_writeback(void *addr, u32 size);
inline dcache_invalidate(void *addr, u32 size);

/*
 * return 0: request attachs to dmac0(ADMAC).
 *        1: request attachs to dmac1(SDMAC).
 */
int silan_syscfg_get_dmac_reqid(int id, int *dmac_req);

/*
silan misc clk config
*/
typedef enum{
		SILAN_SPDIF = 0,
		SILAN_SDMMC,
		SILAN_SDIO,
		SILAN_OTP,
		SILAN_CODEC1,
		SILAN_CODEC2,
		SILAN_PWM,
		SILAN_ADC,
		SILAN_TIMER,
		SILAN_RTC,
		SILAN_USBFS,
		SILAN_PDB	
}silan_misc_clk_t;

/*
silan mcu clk config
*/
typedef enum{
	MCUCLK_BUS_1=0,
	MCUCLK_BUS_2,
	MCUCLK_BUS_4,
}silan_mcu_clk_t;


/*
if_div:
0: sysclk
1: sysclk/2
2: pll_ref/2
3: rcl/2

but:
SILAN_SPDIF 
*/
void silan_system_misc_clk_config(silan_misc_clk_t misc_mode,int if_div);
void silan_mcuclk_div_config(silan_mcu_clk_t bus_div);
void silan_dsp_dcache_cfg(int mode);
void silan_adc_cclk_config(int clksel, silan_clk_onff_t onoff);
void silan_pwm_cclk_config(int clksel, silan_clk_onff_t onoff);
void silan_vlsp_cclk_config(silan_clk_onff_t onoff);
void silan_usbfs_cclk_config(silan_clk_onff_t onoff);
void silan_codec1_cclk_config(silan_clk_onff_t onoff);
void silan_codec2_cclk_config(silan_clk_onff_t onoff);
void silan_sdio_cclk_config(int clksel, silan_clk_onff_t onoff);

void silan_mcu_debug_close(void);
void silan_risc_debug_close(void);
void silan_dsp_debug_close(void);
void silan_dsp_debug_open(void);
void silan_risc_debug_open(void);

void silan_mcu_cclksel(silan_mcu_cclksel_t bus_div);
void silan_timer_cclk_config(int clksel, silan_clk_onff_t onoff);
void silan_bootup_dsp(u32 load_addr, u32 len, int media);
void silan_bootup_risc(u32 load_addr, u32 len);

u32 silan_get_timer_cclk(void);
u32 silan_get_mcu_cclk(void);
u32 silan_get_dsp_cclk(void);

typedef enum {
	DCACHE_MODE_DC = 0,
	DCACHE_MODE_MEM
} silan_dcache_mode_t;

typedef enum {
	DSP_BOOT_FLASH = 0,
	DSP_BOOT_SDRAM
} silan_dsp_boot_t;

#define    SOFT_RST_SF1                  0
#define    SOFT_RST_SR1                  1
#define    SOFT_RST_SHDW                 2
#define    SOFT_RST_OTP                  (  3+(  4<<8))
#define    SOFT_RST_SDRAM                (  5+(  6<<8))
#define    SOFT_RST_SPER                 7
#define    SOFT_RST_APER                 8
#define    SOFT_RST_SCFG                 9
#define    SOFT_RST_PCFG                 10
#define    SOFT_RST_SDMMC                11
#define    SOFT_RST_SDIO                 12
#define    SOFT_RST_SDMAC                13
#define    SOFT_RST_OSPDIF               ( 14+( 15<<8))
#define    SOFT_RST_ISPDIF               ( 16+( 17<<8))
#define    SOFT_RST_I1_I2S               ( 18+( 19<<8))
#define    SOFT_RST_I2_I2S               ( 20+( 21<<8))
#define    SOFT_RST_I3_I2S               ( 22+( 23<<8))
#define    SOFT_RST_O1_I2S               ( 24+( 25<<8))
#define    SOFT_RST_O2_I2S               ( 26+( 27<<8))
#define    SOFT_RST_O3_I2S               ( 28+( 29<<8))
#define    SOFT_RST_PCM_I2S              ( 30+( 31<<8))
#define    SOFT_RST_PDM                  ( 32+( 33<<8))
#define    SOFT_RST_ADMAC                34
#define    SOFT_RST_GPIO1                35
#define    SOFT_RST_GPIO2                36
#define    SOFT_RST_SPI                  37
#define    SOFT_RST_UART1                38
#define    SOFT_RST_UART2                39
#define    SOFT_RST_UART3                40
#define    SOFT_RST_UART4                41
#define    SOFT_RST_I2C1                 42
#define    SOFT_RST_I2C2                 43
#define    SOFT_RST_CXC                  44
#define    SOFT_RST_IIR                  45
#define    SOFT_RST_TIMER                ( 46+( 47<<8))
#define    SOFT_RST_PMU                  48
#define    SOFT_RST_RTC                  49
#define    SOFT_RST_CODEC1               50
#define    SOFT_RST_CODEC2               51
#define    SOFT_RST_PDP                  ( 52+( 53<<8))
#define    SOFT_RST_PWM                  ( 54+( 55<<8))
#define    SOFT_RST_ADC                  ( 56+( 57<<8))

void silan_soft_rst(unsigned int mod);

#define    CLK_SDMMC                0
#define    CLK_SDIO                 1
#define    CLK_USBHS                2
#define    CLK_USBFS                3
#define    CLK_SDMAC                4
#define    CLK_OSPDIF               5
#define    CLK_ISPDIF               6
#define    CLK_I1_I2S               7
#define    CLK_I2_I2S               8
#define    CLK_I3_I2S               9
#define    CLK_O1_I2S               10
#define    CLK_O2_I2S               11
#define    CLK_O3_I2S               12
#define    CLK_PCM_I2S              13
#define    CLK_PDM                  14
#define    CLK_ADMAC                15
#define    CLK_GPIO1                16
#define    CLK_GPIO2                17
#define    CLK_SPI                  18
#define    CLK_UART1                19
#define    CLK_UART2                20
#define    CLK_UART3                21
#define    CLK_UART4                22
#define    CLK_I2C1                 23
#define    CLK_I2C2                 24
#define    CLK_PMU                  25
#define    CLK_CXC                  26
#define    CLK_IIR                  27
#define    CLK_TIMER                28
#define    CLK_PDP                  29
#define    CLK_PWM                  30
#define    CLK_ADC                  31
#define    CLK_SSP                  32

#endif  // __SILAN_SYSCFG_H__

