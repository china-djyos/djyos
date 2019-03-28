/*
 * silan_syscfg.c
 */

#include "silan_types.h"
#include "silan_syscfg.h"
#include "silan_syscfg_regs.h"
#include "silan_printf.h"
#include "silan_pmu.h"
#include "string.h"

// silan_rstgen.c
#define  RST_BIAS_ADDR   (8)
#define  SYSCFG_BASE     SCFG_APB_SYSCFG_ADDR_BASE

// silan_clkgen.c
#define  SYSCFG_BASE     SCFG_APB_SYSCFG_ADDR_BASE
#define  CLKON_BIAS_ADDR (0)

void silan_mcuclk_div_config(silan_mcu_clk_t bus_div)
{
	u32 tmp;
	tmp = __REG32(SILAN_SYSCFG_REG12);
	tmp &= ~(0x3<<10);
	tmp |= ((bus_div&0x3)<<10);
	__REG32(SILAN_SYSCFG_REG12) = tmp;
	
}

void silan_system_misc_clk_config(silan_misc_clk_t misc_mode,int if_div)
{	
	u32 tmp;
	switch(misc_mode)
	{
		case SILAN_SPDIF:
			__REG32(SILAN_SYSCFG_REG2) &= ~(1<<3) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG2); 
			tmp &= ~(0x7);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= 0x1;
			}else if(if_div == 2){
				tmp |= 0x2;
			}else if(if_div == 3){
				tmp |= 0x3;;
			}
			//...
			__REG32(SILAN_SYSCFG_REG2) = tmp; 
			__REG32(SILAN_SYSCFG_REG2) |= (1<<3)  ;//clk able
			break;
		case SILAN_SDMMC:
			__REG32(SILAN_SYSCFG_REG2) &= ~(1<<11) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG2); 
			tmp &= ~(0x7<<8);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<8);
			}else if(if_div == 2){
				tmp |= (0x2<<8);
			}else if(if_div == 3){
				tmp |= (0x3<<8);
			}
			__REG32(SILAN_SYSCFG_REG2) = tmp; 
			__REG32(SILAN_SYSCFG_REG2) |= (1<<11)  ;//clk able
			break;
		case SILAN_SDIO:
			__REG32(SILAN_SYSCFG_REG2) &= ~(1<<15) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG2); 
			tmp &= ~(0x7<<12);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<12);
			}else if(if_div == 2){
				tmp |= (0x2<<12);
			}else if(if_div == 3){
				tmp |= (0x3<<12);
			}
			__REG32(SILAN_SYSCFG_REG2) = tmp; 
			__REG32(SILAN_SYSCFG_REG2) |= (1<<15)  ;//clk able
			break;
		case SILAN_OTP:
			__REG32(SILAN_SYSCFG_REG2) &= ~(1<<19) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG2); 
			tmp &= ~(0x7<<16);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<16);
			}else if(if_div == 2){
				tmp |= (0x2<<16);
			}else if(if_div == 3){
				tmp |= (0x3<<16);
			}
			__REG32(SILAN_SYSCFG_REG2) = tmp; 
			__REG32(SILAN_SYSCFG_REG2) |= (1<<19)  ;//clk able
			break;
		case SILAN_CODEC1:
			__REG32(SILAN_SYSCFG_REG2) |= (1<<23)  ;//clk able
			break;
		case SILAN_CODEC2:
			__REG32(SILAN_SYSCFG_REG2) |= (1<<24)  ;//clk able
			break;
		case SILAN_PWM:
			__REG32(SILAN_SYSCFG_REG3) &= ~(1<<3) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG3); 
			tmp &= ~(0x7);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1);
			}else if(if_div == 2){
				tmp |= (0x2);
			}else if(if_div == 3){
				tmp |= (0x3);
			}
			__REG32(SILAN_SYSCFG_REG3) = tmp; 
			__REG32(SILAN_SYSCFG_REG3) |= (1<<3)  ;//clk able
			break;
		case SILAN_ADC:
			__REG32(SILAN_SYSCFG_REG3) &= ~(1<<7) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG3); 
			tmp &= ~(0x7<<4);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<4);
			}else if(if_div == 2){
				tmp |= (0x2<<4);
			}else if(if_div == 3){
				tmp |= (0x3<<4);
			}
			__REG32(SILAN_SYSCFG_REG3) = tmp; 
			__REG32(SILAN_SYSCFG_REG3) |= (1<<7)  ;//clk able
			break;
		case SILAN_TIMER:
			__REG32(SILAN_SYSCFG_REG3) &= ~(1<<23) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG3); 
			tmp &= ~(0x7<<20);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<20);
			}else if(if_div == 2){
				tmp |= (0x2<<20);
			}else if(if_div == 3){
				tmp |= (0x3<<20);
			}
			__REG32(SILAN_SYSCFG_REG3) = tmp; 
			__REG32(SILAN_SYSCFG_REG3) |= (1<<23)  ;//clk able
			break;
		case SILAN_RTC:
			__REG32(SILAN_SYSCFG_REG3) |= (1<<24)  ;//clk able
			break;
		case SILAN_USBFS:
			__REG32(SILAN_SYSCFG_REG3) |= (1<<25)  ;//clk able
			break;
		case SILAN_PDB:
			__REG32(SILAN_SYSCFG_REG3) &= ~(1<<30) ; //clk disable
			tmp = __REG32(SILAN_SYSCFG_REG3); 
			tmp &= ~(0xf<<26);
			if(!if_div){
			;
			}else if(if_div == 1){
				tmp |= (0x1<<26);
			}else if(if_div == 2){
				tmp |= (0x2<<26);
			}else if(if_div == 3){
				tmp |= (0x3<<26);
			}
			
			tmp |= (0x3<<28);  //¶þ¼¶8·ÖÆµ
 			__REG32(SILAN_SYSCFG_REG3) = tmp; 
			__REG32(SILAN_SYSCFG_REG3) |= (1<<30)  ;//clk able
			break;
		default:
			break;
	}
}

#if defined(__XCC__)
inline u32 cpu_to_dma(u32 addr)
{
	/*
	 * dsp inner sram addr is 0x30XX_XXXX, 
	 * but dmac accesses them at 0x23XX_XXXX.
	 *
	 * dmac can't access SDRAM at 0x02XX_XXXX,
	 * but accesses them at 0x22XX_XXXX.
	 * TODO, SDRAM maybe on 0x00XX_XXXX.
	 */
	switch (addr&(0xff000000)) {
		case 0x30000000 : 
			return 0x23000000 | (addr & 0x00ffffff);
		case 0x02000000 :
			return 0x22000000 | (addr & 0x00ffffff);
		default:
			return addr;
	}
}

inline u32 dma_to_cpu(u32 addr)
{
	/*
	 * dsp inner sram addr is 0x30XX_XXXX, 
	 * but dmac accesses them at 0x23XX_XXXX.
	 *
	 * dmac can't access SDRAM at 0x02XX_XXXX,
	 * but accesses them at 0x22XX_XXXX.
	 * TODO, SDRAM maybe on 0x00XX_XXXX.
	 */
	switch (addr&(0xff000000)) {
		case 0x23000000 : 
			return 0x30000000 | (addr & 0x00ffffff);
		case 0x22000000 : 
			return 0x02000000 | (addr & 0x00ffffff);
		default:
			return addr;
	}
}
inline dcache_writeback(void *addr, u32 size)
{
	xthal_dcache_region_writeback(addr, size);
}

inline dcache_invalidate(void *addr, u32 size)
{
	xthal_dcache_region_invalidate(addr, size);
}
#endif

#if defined(__CC_ARM)||  defined ( __GNUC__ )
inline u32 cpu_to_dma(u32 addr)
{
	return addr;
}

inline u32 dma_to_cpu(u32 addr)
{
	return addr;
}

inline dcache_writeback(void *addr, u32 size)
{
	// do nothing!
}

inline dcache_invalidate(void *addr, u32 size)
{
	// do nothing!
}
#endif

inline int *ptr_cpu_to_dma(int *addr)
{
	return (int *)cpu_to_dma((u32)addr);
}

inline int *ptr_dma_to_cpu(int *addr)
{
	return (int *)dma_to_cpu((u32)addr);
}

void silan_dsp_dcache_cfg(int mode)
{
	if(mode == DCACHE_MODE_DC)
		__REG32(SILAN_SYSCFG_SOC1) |=  (0x1);
	else
		__REG32(SILAN_SYSCFG_SOC1) &= ~(0x1);
}

inline static void silan_dsp_unreset(void)
{
	__REG32(SILAN_SYSCFG_REG11) |= (0x1<<2);
}
//-------------------------------------------
inline static void silan_dsp_reset(void)
{
	__REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<2);
}
//-------------------------------------------
inline static void silan_risc_unreset(void)
{
	__REG32(SILAN_SYSCFG_REG11) |= (0x1);
}
//-------------------------------------------
inline static void silan_risc_reset(void)
{
	__REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
}

void silan_risc_reboot(void)
{
	silan_risc_reset();
	silan_risc_unreset();
}

void silan_dsp_reboot(void)
{
	silan_dsp_reset();
	silan_dsp_unreset();
}

void silan_dsp_boot_from(int media)
{
	if(media == DSP_BOOT_SDRAM)
		__REG32(SILAN_SYSCFG_SOC0) |=  (0x1<<1);
	else
		__REG32(SILAN_SYSCFG_SOC0) &= ~(0x1<<1);

#if 1//__DEBUG__
	int inst;
	SL_LOG("Check DSP First Inst: ");
	inst = (media == DSP_BOOT_SDRAM) ? __REG32(0x02000000) :
		                               __REG32(0x00000800) ;
	if(inst == 0x0000AF02)
		SL_LOG("%08x, Success!", inst);
	else
		SL_LOG("%08x, Fail!", inst);
#endif

	silan_dsp_unreset();
}

void silan_risc_boot(void)
{
#if 1//__DEBUG__
	int inst, tmp;
	SL_LOG("Check RISC First Inst: ");
	inst = __REG32(0x00000400);
	tmp  = (inst & 0xFF000000) >> 24;
	if((tmp == 0x02) || (tmp == 0x22))
		SL_LOG("%08x, Success!", inst);
	else
		SL_LOG("%08x, Fail!", inst);
#endif

	silan_risc_unreset();
}

void silan_bootup_dsp(u32 load_addr, u32 len, int media)
{	
	int inst;

	inst = __REG32(load_addr);
	if(media == DSP_BOOT_SDRAM) {
		if(inst == 0x0000AF02)
			memcpy((u8 *)0x02000000, (char *)load_addr, len);
		else
			SL_LOG("No DSP Firmware on Flash!");
	}
	silan_dsp_boot_from(media);
}

void silan_bootup_risc(u32 load_addr, u32 len)
{
	u32 risc_boot_addr;

	//silan_risc_reset();
	risc_boot_addr = (*((u32 *)(0x400)));
	SL_LOG("RISC_RUN_AT = 0x%08x", risc_boot_addr);
	if((risc_boot_addr & 0xFF000000) != 0x00000000)
		memcpy((u8 *)(risc_boot_addr & 0xFFFFF000), (char *)load_addr, len);
	silan_risc_boot();
}

void silan_risc_debug_open(void)
{
	__REG32(SILAN_SYSCFG_REG11) |=  (0x1<<21);
}

void silan_risc_debug_close(void)
{
	__REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<21);
}

void silan_dsp_debug_open(void)
{
	__REG32(SILAN_SYSCFG_REG11) |=  (0x1<<22);
}

void silan_dsp_debug_close(void)
{
	__REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<22);
}

void silan_mcu_debug_open(void)
{
	__REG32(SILAN_SYSCFG_REG11) |=  (0x1<<20);
}

void silan_mcu_debug_close(void)
{
	__REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<20);
}

void silan_halt_dsp(void)
{
	__REG32(SILAN_SYSCFG_SOC0) |=  (0x1<<0);
}

void silan_run_dsp(void)
{
	__REG32(SILAN_SYSCFG_SOC0) &= ~(0x1<<0);
}

// silan_rstgen.c
static void write_rstreg(char value)
{
    int reg_bias,data_bias,tmp;
    reg_bias = value/32;
    data_bias= value - reg_bias*32;
    tmp = __REG32(SYSCFG_BASE+((RST_BIAS_ADDR +reg_bias)<<2));
    __REG32(SYSCFG_BASE+((RST_BIAS_ADDR +reg_bias)<<2)) = tmp & (~(1<<data_bias));
    __REG32(SYSCFG_BASE+((RST_BIAS_ADDR +reg_bias)<<2)) = tmp | (1<<data_bias);
}

void silan_soft_rst(unsigned int mod)
{
    char byte0,byte1,byte2,byte3;
    byte0 = mod & 0xff;
    byte1 = (mod>>8) & 0xff;
    byte2 = (mod>>16) & 0xff;
    byte3 = (mod>>24) & 0xff;
    write_rstreg(byte0);
    if (byte1 !=0) {
        write_rstreg(byte1);
        if(byte2 !=0) {
            write_rstreg(byte2);
            if(byte3 !=0) {
                write_rstreg(byte3);
            }
        }
    }
}

// silan_clkgen.c
static void write_clkon(char value)
{
    int reg_bias,data_bias,tmp;
    reg_bias = value/32;
    data_bias= value - reg_bias*32;
    tmp = __REG32(SYSCFG_BASE+((CLKON_BIAS_ADDR +reg_bias)<<2));
    __REG32(SYSCFG_BASE+((CLKON_BIAS_ADDR +reg_bias)<<2)) = tmp | (1<<data_bias);
}

static void write_clkoff(char value)
{
    int reg_bias,data_bias,tmp;
    reg_bias = value/32;
    data_bias= value - reg_bias*32;
    tmp = __REG32(SYSCFG_BASE+((CLKON_BIAS_ADDR +reg_bias)<<2));
    __REG32(SYSCFG_BASE+((CLKON_BIAS_ADDR +reg_bias)<<2)) = tmp &(~ (1<<data_bias));
}

void silan_module_alloff(void)
{
	__REG32(SILAN_SYSCFG_REG0) = (1 << 25);
	__REG32(SILAN_SYSCFG_REG1) = 0;
}

void silan_module_clkon(int mod, silan_clk_onff_t onoff)
{
    char byte0;
    byte0 = mod & 0xff;
	if(onoff == CLK_ON)
    	write_clkon(byte0);
	else
		write_clkoff(byte0);
}

void silan_syspre_clksel(silan_presys_clksel_t div)
{
	int regvalue;
	regvalue = __REG32(SILAN_SYSCFG_REG12);
	regvalue &= ~(0x3<<8);
	regvalue |= div & 0x3;
	__REG32(SILAN_SYSCFG_REG12);
}

void silan_mcu_cclksel(silan_mcu_cclksel_t bus_div)
{
	u32 tmp;
	tmp = __REG32(SILAN_SYSCFG_REG12);
	tmp &= ~(0x3<<10);
	tmp |= ((bus_div&0x3)<<10);
	__REG32(SILAN_SYSCFG_REG12) = tmp;	
}

void silan_risc_cclk_onoff(silan_clk_onff_t onoff)
{
	if(onoff == CLK_ON)
		__REG32(SILAN_SYSCFG_REG12) |=  (1 << 2);
	else
		__REG32(SILAN_SYSCFG_REG12) &= ~(1 << 2);
}

void silan_dsp_cclk_onoff(silan_clk_onff_t onoff)
{
	if(onoff == CLK_ON)
		__REG32(SILAN_SYSCFG_REG12) |=  (1 << 3);
	else
		__REG32(SILAN_SYSCFG_REG12) &= ~(1 << 3);
}

void silan_timer_cclk_config(int clksel, silan_clk_onff_t onoff)
{
	misc_clk_reg2_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
    misc_clk_reg.b.timer_clksel = clksel;
	misc_clk_reg.b.timer_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG3) = misc_clk_reg.d32;
}

void silan_adc_cclk_config(int clksel, silan_clk_onff_t onoff)
{
	misc_clk_reg2_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
    misc_clk_reg.b.adc_clksel = clksel;
	misc_clk_reg.b.adc_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG3) = misc_clk_reg.d32;
}

void silan_otp_cclk_config(int clksel, silan_clk_onff_t onoff)
{
	misc_clk_reg1_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG2);
    misc_clk_reg.b.otp_clksel = clksel;
	misc_clk_reg.b.otp_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG2) = misc_clk_reg.d32;
}

// RTC/CODEC1/CODEC2 Config Bus Clock enable
void silan_vlsp_cclk_config(silan_clk_onff_t onoff)
{
	misc_clk_reg2_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
	misc_clk_reg.b.rtc_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG3) = misc_clk_reg.d32;
}

void silan_pwm_cclk_config(int clksel, silan_clk_onff_t onoff)
{
	misc_clk_reg2_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
	misc_clk_reg.b.pwm_clksel = clksel;
	misc_clk_reg.b.pwm_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG3) = misc_clk_reg.d32;
}

void silan_usbfs_cclk_config(silan_clk_onff_t onoff)
{
	misc_clk_reg2_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
	misc_clk_reg.b.usbfs_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG3) = misc_clk_reg.d32;
}

void silan_codec1_cclk_config(silan_clk_onff_t onoff)
{
	misc_clk_reg1_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG2);
	misc_clk_reg.b.codec1_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG2) = misc_clk_reg.d32;
}

void silan_codec2_cclk_config(silan_clk_onff_t onoff)
{
	misc_clk_reg1_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG2);
	misc_clk_reg.b.codec2_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG2) = misc_clk_reg.d32;
}

void silan_sdio_cclk_config(int clksel, silan_clk_onff_t onoff)
{
	misc_clk_reg1_t misc_clk_reg;

	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG2);
    misc_clk_reg.b.sdio_clksel = clksel;
	misc_clk_reg.b.sdio_clkon = onoff;
	__REG32(SILAN_SYSCFG_REG2) = misc_clk_reg.d32;
}

u32 silan_get_timer_cclk(void)
{
	misc_clk_reg2_t misc_clk_reg;
	u32 timer_sel, timer_cclk;
	
	misc_clk_reg.d32 = __REG32(SILAN_SYSCFG_REG3);
	timer_sel = misc_clk_reg.b.timer_clksel;
	switch(timer_sel) {
		case TIMER_CCLK_CLKSYS_DIV2 :
		case TIMER_CCLK_CLKSYS_DIV3 :
			timer_cclk = get_sysclk_val_settled()/(timer_sel+2);
			break;
		case TIMER_CCLK_PLLREF_DIV2 :
			timer_cclk = 6000000;
			break;
		case TIMER_CCLK_RCL_DIV2 :
			timer_cclk = 32000;
			break;
		default :
			timer_cclk = 6000000;
			break;
	}

	return timer_cclk;
}

u32 silan_get_mcu_cclk(void)
{
	SOC_CLK_REG soc_clk;
	u32 bus_clksel, mcu_cclksel, mcu_cclk, syspre_clksel;
	
	soc_clk.d32 = __REG32(SILAN_SYSCFG_REG12);
	syspre_clksel = soc_clk.b.syspre_clksel;
	bus_clksel = soc_clk.b.bus_clksel;
	mcu_cclksel = soc_clk.b.mcu_cclksel;
	if(mcu_cclksel == 3)
		mcu_cclksel = 2;
	
	mcu_cclk = get_sysclk_val_settled()/(syspre_clksel+1)/(bus_clksel+1)/(2^mcu_cclksel);

	return mcu_cclk;
}

u32 silan_get_dsp_cclk(void)
{
	SOC_CLK_REG soc_clk;
	u32 dsp_cclksel, syspre_clksel, cclk;
	
	soc_clk.d32 = __REG32(SILAN_SYSCFG_REG12);
	syspre_clksel = soc_clk.b.syspre_clksel;
	dsp_cclksel = soc_clk.b.dsp_cclksel;
	
	cclk = get_sysclk_val_settled()/(syspre_clksel+1)/(dsp_cclksel+1);

	return cclk;
}

