#ifndef _SILAN_PMU_H
#define _SILAN_PMU_H

#include "silan_types.h"
#include "silan_syscfg.h"

typedef enum {
	SYSCLK_FROM_RCH  = 0,
	SYSCLK_FROM_OSC,
	SYSCLK_FROM_32KPLL,
	SYSCLK_FROM_PLL,
	SYSCLK_FROM_RCL,
	SYSCLK_FROM_BCK,
}silan_sysclk_ref_t;


typedef enum {
	_32KPLL_4M  = 0,
	_32KPLL_2M,
	_32KPLL_1M,
	_32KPLL_500K,
	_32KPLL_8M,
	_32KPLL_12M,
	_32KPLL_16M,
	_32KPLL_32K,
	
	//SYSPLL?
}silan_sysclk_ifsubclk_t;

// usb20 
typedef enum {
  USB20_REFCLKSEL_12MOSC  = 0,
  USB20_REFCLKSEL_M32KPLL , 	
  USB20_REFCLKSEL_SYSPLL_DIV , 
}silan_usb20_ref_t;

typedef enum {
  USB20_SYSPLL_DIV2  = 0,
  USB20_SYSPLL_DIV4  	,
  USB20_SYSPLL_DIV6  	,
  USB20_SYSPLL_DIV8  	,
  USB20_SYSPLL_DIV10  ,
  USB20_SYSPLL_DIV12  ,
  USB20_SYSPLL_DIV14  ,
  USB20_SYSPLL_DIV16  ,
}silan_usb20_under_syspll_div_t;

typedef enum {
	AUDIOPLL_45M  = 0,
	AUDIOPLL_49M,
}silan_audiopllsel_t;

typedef enum {
//	SYSPLL_40M  
	SYSPLL_60M = 0,
	SYSPLL_80M ,
	SYSPLL_100M ,
	SYSPLL_120M ,
	SYSPLL_140M ,
	SYSPLL_160M ,
	SYSPLL_180M ,
	SYSPLL_200M ,
}silan_syspllsel_t;

#define SYSPLL_SSCG_ON    1
#define SYSPLL_SSCG_OFF   0

#define LDO_ON  1
#define LDO_OFF 0 
typedef enum {
	LDO_OUT_12V, //1.2V
	LDO_OUT_10V,
	LDO_OUT_08V,
	LDO_OUT_14V,
}silan_ldo_level_sel_t;

#define DEFAULT_VREF 0
typedef enum {
	LVDLS_20  = 0,
	LVDLS_21,
	LVDLS_22,
	LVDLS_23,
	LVDLS_24,
	LVDLS_25,
	LVDLS_27,
	LVDLS_29,
}silan_lvd_level_t;

typedef enum {
	LVRLS_19  = 0,
	LVRLS_21,
	LVRLS_23,
	LVRLS_25,
}silan_lvr_level_t;

typedef struct 
{
	u32  sysclk;       //
	u32  sysclk_ref;     // res
}sysclk_cfg_t;

extern sysclk_cfg_t sysclk_init;

typedef enum{
	TCK_CLKSEL_RCL = 0,
	TCK_CLKSEL_RCH,
	TCK_CLKSEL_OSC32K,
	TCK_CLKSEL_OSC12M,
	TCK_CLKSEL_32KPLL,
	TCK_CLKSEL_UBCK,
	TCK_CLKSEL_APLLDIV4,
	TCK_CLKSEL_SPLLDIV16,
}silan_tck_clksel_t;

int get_sysclk_val_settled(void);
void syspll_config(silan_syspllsel_t type);
void sysclk_init_op(void);
int pre_sysclk_sel(silan_sysclk_ref_t sysclk_ref,silan_syspllsel_t if_clk_sel);

void silan_spiflash_power_enable(void);
void silan_pmu_wakeup_enable(u32 id);
void silan_pmu_wakeup_disable(u32 id);
void silan_sar_adc_vref_select(int inner);

void silan_dsp_ldo_disable(void);
void silan_dsp_ldo_config(int level);
void silan_pmu_wdt_init(u32 wdt_cnt);
void silan_pmu_wdt_reset_config(u32 wdt_cnt);
void silan_pmu_wdt_reset_disable(void);
inline void silan_pmu_wdt_clr(void);
void silan_pwr_iso_disable(void);
void silan_pwr_iso_enable(void);

void silan_pmu_wakeup_disable(u32 id);
void silan_sar_adc_vref_select(int inner);

void silan_soc_stop(void);
void silan_sdram_pwd_deal(void);
void silan_sdram_pwd_quit(void);
void silan_usb_pwd_deal(void);
void silan_usb_pwd_quit(void);

void silan_audiopll_init(void);
void silan_syspll_init(u32 sscg);
void silan_sysclk_change(silan_sysclk_ref_t sysclk_ref,silan_syspllsel_t if_clk_sel);
void audiopll_config(silan_audiopllsel_t type);
void silan_module_clkon(int mod, silan_clk_onff_t onoff);

void silan_pmu_wakeup_disable(u32 id);
void silan_sar_adc_vref_select(int inner);

void silan_ubck_enable(void);

#endif
