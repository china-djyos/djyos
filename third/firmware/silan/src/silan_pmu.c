/***************************************/
/*	Function:  							 */
/*	Author:    zhugang							 */
/*	Data:			2016-03-26							*/		
/***************************************/
#include "silan_types.h"
#include "silan_pmu.h"
#include "silan_pmu_regs.h"
#include "silan_printf.h"

static void pmu_int_enable(uint32_t val)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr1;
	tmp |= val ;
	sysclk_st->sys_ctr1 = tmp;	
	sysclk_regs_protect_close();
}

static void pmu_int_enable_1(uint32_t val)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp |= val ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

static void pmu_int_disable(uint32_t val)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr1;
	tmp &= ~val ;
	sysclk_st->sys_ctr1 = tmp;	
	sysclk_regs_protect_close();
}

static void pmu_int_disable_1(uint32_t val)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~val ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_tck_config(silan_tck_clksel_t clk_sel)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr0;
	tmp&= ~TCK_CS(0x7); 
	tmp|= TCK_CS(clk_sel);
	sysclk_st->sys_ctr0 = tmp;
	sysclk_regs_protect_close();
}
/*
system ctrl
*/
void silan_pwr_iso_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->iso_en = 0;
	sysclk_regs_protect_close();
}

void silan_pwr_iso_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->iso_en |= ((1<<1)|(5<<1)|(9<<1)|(13<<1));
	sysclk_regs_protect_close();
}

void silan_soc_stop()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr0;
	tmp &= ~SYSCLK_EN;
	sysclk_st->sys_ctr0 = tmp;	
	sysclk_regs_protect_close();
}

void silan_clear_pmu_status(uint32_t val)
{
	sysclk_regs_protect_open();		
	sysclk_st->sys_sts &= ~val;	
	sysclk_regs_protect_close();
}

uint32_t silan_get_pmu_status()
{
	uint32_t tmp;
	sysclk_regs_protect_open();		
	tmp = sysclk_st->sys_sts;
	sysclk_regs_protect_close();
	return tmp;
}

void silan_spiflash_power_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr2 |= SF_PWR;
	sysclk_regs_protect_close();
}

/*
dsp ldo
*/
static void silan_dsp_ldo_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr2 |= LDO_DSP_EN;
	sysclk_regs_protect_close();
}

static void silan_dsp_ldo_vsel(int level)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr2;
	tmp &= ~VSEL(0x3);
	tmp |= VSEL(level); 
	sysclk_st->sys_ctr2 = tmp;
	sysclk_regs_protect_close();
}

void silan_dsp_ldo_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr2 &= ~LDO_DSP_EN;
	sysclk_regs_protect_close();
}

void silan_dsp_ldo_config(int level)
{
	silan_dsp_ldo_vsel(level);
	silan_dsp_ldo_enable();	
}

void silan_upll32k_sel(uint8_t clk_sel)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr0;
	tmp&= ~UPLL32K_SEL(0x7); 
	tmp|= UPLL32K_SEL(clk_sel);
	sysclk_st->sys_ctr0 = tmp;	
	if(clk_sel != _32KPLL_32K)
	while(((sysclk_st->sys_sts)&_32KPLL_LOCK) == 0){ /*code here*/}
	sysclk_regs_protect_close();
}

static void silan_rch_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= RCH_EN;
	sysclk_regs_protect_close();
}

static void silan_rch_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~RCH_EN;
	sysclk_regs_protect_close();
}

static void silan_rcl_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= RCL_EN;
	sysclk_regs_protect_close();
}

static void silan_rcl_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~RCL_EN;
	sysclk_regs_protect_close();
}

static void silan_32kpll_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= _32KPLL_EN|UPLL32K_EN;
	sysclk_regs_protect_close();
}

static void silan_32kpll_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~(_32KPLL_EN|UPLL32K_EN);
	sysclk_regs_protect_close();
}

static void silan_syspll_enable()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= SYS_PLL_EN;  //sys no pwd
		
	tmp = sysclk_st->sscg;
	tmp &= ~0x1000; 
	sysclk_st->sscg = tmp;
	while(((sysclk_st->sys_sts)&SPLL_LOCK) == 0){ /*code here*/}
	sysclk_st->sys_pll_ctrl2 |= SPLL_CLK_EN;  //pll out
	sysclk_regs_protect_close();
}

static void silan_syspll_disable(void)
{
	sysclk_regs_protect_open();
	sysclk_st->sys_pll_ctrl2 &= ~SPLL_CLK_EN;  
	sysclk_st->sys_ctr0 &= ~SYS_PLL_EN;
	sysclk_regs_protect_close();
}

static int silan_apll_enable(void)
{
	int delay = 0;

	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= AUDIO_PLL_EN;
	while(((sysclk_st->sys_sts)&APLL_LOCK) == 0){
		delay++;
    	//    timer_wait_us(10);
        if (delay > 10000){ // 10ms
            return 0;
        }
	}
	sysclk_st->audio_pll_ctrl1 |= APLL_CLK_EN;
	sysclk_regs_protect_close();
	return 1;
}

static void silan_apll_disable(void)
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~AUDIO_PLL_EN;
	sysclk_st->audio_pll_ctrl1 &= ~APLL_CLK_EN;
	sysclk_regs_protect_close();
}

void silan_ubck_enable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~U11_BCK_PD;
	sysclk_st->sys_ctr0 |= U11_BCK_EN|U11_BCK_RSTN|U11_USB11_RSTN|U11_BCK_REFSEL;
	sysclk_regs_protect_close();
}

void silan_ubck_disable()
{
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 &= ~U11_BCK_EN;
	sysclk_regs_protect_close();
}

void silan_sdram_pwd_deal()
{
	__REG32(SILAN_SYSCFG_SDRAMCTR) |=0x2;  //SDRAM PWD
	while(!(__REG32(SILAN_SYSCFG_SDRAMCTR)&0x20));
	__REG32(SILAN_SYSCFG_REG16) &=~(1<<13);   //sdram
}

void silan_sdram_pwd_quit()
{
	__REG32(SILAN_SYSCFG_SDRAMCTR) &=~0x2;  
	while((__REG32(SILAN_SYSCFG_SDRAMCTR)&0x20));
}

void silan_usb_pwd_deal()
{
	//usb20
    __REG32(SILAN_SYSCFG_REG3) |= ((1<<25));
	__REG32(SILAN_SYSCFG_REG15) |= ((1<<26)|(1<<3)|(1<<9)|(1<<13));
	__REG32(SILAN_SYSCFG_REG15) &= ~(1<<7); 
	__REG32(SILAN_SYSCFG_REG15) |= (1<<8); //suspend sel	
	__REG32(SILAN_SYSCFG_REG15) &= ~((1<<13)|(1<<3)|(1<<9)|(1<<25));
	//usb11
	__REG32(SILAN_SYSCFG_REG15) |= ((1<<24)|(1<<25));  //suspend				
	sysclk_regs_protect_open();
	sysclk_st->sys_ctr0 |= U11_BCK_PD;
	sysclk_regs_protect_close();
}

void silan_usb_pwd_quit()
{
		sysclk_regs_protect_open();
		sysclk_st->sys_ctr0 &= ~U11_BCK_PD;	
		sysclk_regs_protect_close();	
	  __REG32(SILAN_SYSCFG_REG15) = 0x00800200;		  //default
//	  __REG32(SILAN_SYSCFG_REG11) &= ~((0x1f<<5)|(1<<12)); //USB 2.0,USB11复位
	  __REG32(SILAN_SYSCFG_REG11) |= ((0x1f<<5)|(1<<12));  //USB 2.0,USB11释放复位
}

/*
void EnterLowFreq(uint8_t freq)
{
	uint8_t i;
	volatile int delay=0;
	silan_codec_pwd_deal();
	silan_sdram_pwd_deal();
	silan_usb_pwd_deal();
	for(i=0;i<32;i++)
		silan_gpio_output(SILAN_GPIO1_BASE,GPIO_PIN(i));
	for(i=0;i<32;i++)
		silan_gpio_output(SILAN_GPIO2_BASE,GPIO_PIN(i));
	for(i=0;i<32;i++)
		silan_gpio_set_low(SILAN_GPIO1_BASE,GPIO_PIN(i));
	for(i=0;i<32;i++)
		silan_gpio_set_low(SILAN_GPIO2_BASE,GPIO_PIN(i));
	
	//__REG32(SILAN_SYSCFG_REG12) &=0xfffffff0;  //ck803 dsp
	silan_risc_cclk_onoff(CLK_OFF);
	silan_dsp_cclk_onoff(CLK_OFF);
	
	silan_dsp_ldo_disable();
	silan_soc_ldo_config(LDO_OUT_10V);  // 1.0V
	//silan_system_misc_clk_config(SILAN_OTP,0);
	silan_otp_cclk_config(OTP_CCLK_PLLREF_DIV2, CLK_OFF);
	
	delay = 0x10000;
	while(delay--);

	pre_sysclk_sel(SYSCLK_FROM_32KPLL,freq);
}
*/
void silan_sar_adc_vref_select(int inner)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr2;
	if(!inner)
		tmp &= ~SAR_VREF_SEL; //3.3
	else
		tmp |= SAR_VREF_SEL; //2.4
	sysclk_st->sys_ctr2 = tmp;
	sysclk_regs_protect_close();
}

void silan_pmu_wdt_reset_config(uint32_t wdt_cnt)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	
	sysclk_st->sys_wdt_tg = wdt_cnt;  //溢出阈值寄存器
	
	tmp =  sysclk_st->sys_ctr2;
	tmp |= SYS_WDT_RST_EN ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_pmu_wdt_reset_disable(void)
{
	uint32_t tmp;
	
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~SYS_WDT_RST_EN ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_pmu_wdt_clr(void)
{
	sysclk_regs_protect_open();
	sysclk_st->sys_wdt_clr = 0;	
	sysclk_regs_protect_close();
}

void silan_pmu_wakeup_enable(uint32_t id)
{
	if(id == PMU_WAKEUP_RTC)
		pmu_int_enable_1(id);
	else
		pmu_int_enable(id);
}

void silan_pmu_wakeup_disable(uint32_t id)
{
	if(id == PMU_WAKEUP_RTC)
		pmu_int_disable_1(id);
	else
		pmu_int_disable(id);
}

void silan_pmu_irq_enable(uint32_t id)
{
	if(id == PMU_IRQ_WDT)
		pmu_int_enable_1(id);
	else
		pmu_int_enable(id);
}

void silan_lvd_disable()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~LVDEN ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_lvd_config(silan_lvd_level_t type)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~LVDLS(0x7);
	tmp |= LVDLS(type);
	tmp |= LVDEN;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_lvr_disable()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~LVREN ;
	sysclk_st->sys_ctr2 = tmp;	
	sysclk_regs_protect_close();
}

void silan_lvr_config(silan_lvr_level_t type)
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp =  sysclk_st->sys_ctr2;
	tmp &= ~LVRLS(0x3);
	tmp |= LVRLS(type);
	tmp |= LVREN;
	sysclk_st->sys_ctr2 = tmp;
	sysclk_regs_protect_close();
}

void silan_pmu_wdt_init(uint32_t wdt_cnt)
{
	silan_pmu_wdt_clr();
	silan_pmu_wdt_reset_config(wdt_cnt);
}

int pre_sysclk_sel(silan_sysclk_ref_t sysclk_ref,silan_syspllsel_t if_clk_sel)
{
	uint32_t tmp;
	//uint16_t delay = 1000;
	uint8_t retry = 100;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_ctr0;
	tmp |= SYSCLK_EN;
	sysclk_st->sys_ctr0 = tmp;	

////  close hardware auto clk change	
	tmp = sysclk_st->sys_ctr1;
	tmp &= ~CLKMUX_RESETORE_EN;   //no hardware auto change clk  //zhugang 2016-12-10
	sysclk_st->sys_ctr1 = tmp;
	
	sysclk_st->sys_sts &= 0x0;
	
	sysclk_regs_protect_close();   //sysclk enable

	switch(sysclk_ref)
	{	
		case SYSCLK_FROM_RCH:
			sysclk_regs_protect_open();
			sysclk_st->sys_ctr0 |= RCH_EN;
			while(((sysclk_st->sys_sts)&RCH_STB_ASYN) == 0){ /*code here*/}
			sysclk_st->sys_ctr0 &= ~SYSCLK_CS(0x03);
			sysclk_regs_protect_close();
			break;
		case SYSCLK_FROM_OSC:
			sysclk_regs_protect_open();
			sysclk_st->sys_ctr0 |= OSC_EN;
			while(((sysclk_st->sys_sts)&OSC_CLKDET) == 1){ /*code here*/}  //文档 BIT 反了
			sysclk_st->sys_sts &= ~OSC_STS_REG;
			sysclk_st->sys_ctr2 &= ~OSC_PLL_SEL;
			tmp = sysclk_st->sys_ctr0;
			tmp &= ~SYSCLK_CS(0x03);
			tmp |= SYSCLK_CS(0x01);
			sysclk_st->sys_ctr0 = tmp;
			sysclk_regs_protect_close();
			break;
		case SYSCLK_FROM_32KPLL:
			silan_32kpll_enable();
			silan_upll32k_sel(if_clk_sel&0x7);
	//	  while(((sysclk_st->sys_sts)&_32KPLL_STS_REG) == 1){ /*code here*/}
			
			sysclk_regs_protect_open();				
			sysclk_st->sys_sts &= _32KPLL_STS_REG;
			sysclk_st->sys_ctr2 |= OSC_PLL_SEL;       //sel 32kpll //zhugang
			tmp = sysclk_st->sys_ctr0;
			tmp &= ~SYSCLK_CS(0x03);
			tmp |= SYSCLK_CS(0x01);
			tmp &= ~OSC_EN;
			sysclk_st->sys_ctr0 = tmp;
			sysclk_regs_protect_close();
			break;
			
		case SYSCLK_FROM_PLL:		
#ifdef _SINGLE_OSC__
			silan_32kpll_enable();
			silan_upll32k_sel(5);  //12M
	//	  while(((sysclk_st->sys_sts)&_32KPLL_STS_REG) == 0){ /*code here*/}
			sysclk_regs_protect_open();
			sysclk_st->sys_ctr2 |= OSC_PLL_SEL;
			sysclk_regs_protect_close();
#else
			sysclk_regs_protect_open();
			sysclk_st->sys_ctr0 |= OSC_EN;
			while(((sysclk_st->sys_sts)&OSC_CLKDET) == 1){ /*code here*/}  //文档 BIT 反了
		  sysclk_st->sys_ctr2 &= ~OSC_PLL_SEL;
			sysclk_regs_protect_close();
#endif
			syspll_config(if_clk_sel&0x7);
			
			while(retry--)
			{
			sysclk_regs_protect_open();		
			sysclk_st->sys_sts &= SPLL_STS_REG;	
			tmp = sysclk_st->sys_ctr0;
			tmp &= ~SYSCLK_CS(0x03);
			tmp |= SYSCLK_CS(0x02);
			sysclk_st->sys_ctr0 = tmp;
			sysclk_regs_protect_close();
				
			tmp = sysclk_st->sys_ctr0;
     		if((tmp&SYSCLK_CS(0x03))==SYSCLK_CS(0x02))	
				break;
			}
			
			break;
		case SYSCLK_FROM_RCL:
			sysclk_regs_protect_open();
			sysclk_st->sys_ctr0 |= RCL_EN;

			tmp = sysclk_st->sys_ctr0;
			tmp &= ~SYSCLK_CS(0x03);
			tmp |= SYSCLK_CS(0x03);
			tmp &= ~OSC_EN;
			sysclk_st->sys_ctr0 = tmp;
			sysclk_regs_protect_close();
			break;
		case SYSCLK_FROM_BCK:
			silan_ubck_enable();
		
			sysclk_regs_protect_open();	
		
			sysclk_st->sys_sts &= _32KPLL_STS_REG;
			sysclk_st->sys_ctr2 |= OSC_PLL_SEL;       //sel osc //zhugang
	
			tmp = sysclk_st->sys_ctr0;
			tmp &= ~SYSCLK_CS(0x03);
			tmp |= SYSCLK_CS(0x01);
			tmp |= REF_CLK_CS;
			tmp&= ~OSC_EN;
			sysclk_st->sys_ctr0 = tmp;
			sysclk_regs_protect_close();
			break;
		default:
			break;
	}
	return 0;
}

int get_sysclk_val_settled(void)
{
	uint32_t tmp,sysclk_val;
	uint32_t tmp_vco,tmp_RefDiv,tmp_div1,tmp_div2;
	tmp = sysclk_st->sys_ctr0;
	switch((tmp>>29)&0x3)
	{
		case 0:	//RCH
			sysclk_val = 3000000;
			break;
		case 1: //PLL32K
			if(sysclk_st->sys_ctr2&OSC_PLL_SEL){
				switch((tmp>>5)&0x7)
				{
					case 0:
						sysclk_val = 4000000;
						break;
					case 1:
						sysclk_val = 2000000;
						break;
					case 2:
						sysclk_val = 1000000;
						break;
					case 3:
						sysclk_val = 500000;
						break;
					case 4:
						sysclk_val = 8000000;
						break;
					case 5:
						sysclk_val = 12000000;
						break;
					case 6:
						sysclk_val = 16000000;
						break;
					case 7:
						sysclk_val = 32000;
						break;
				}	
			}
			else{ //OSC
				sysclk_val = 12000000;
			}
			break;
		case 2: //SYSPLL
			tmp_vco = sysclk_st->sys_pll_ctrl1&0xfff;
			tmp_div1 = (sysclk_st->sys_pll_ctrl1>>12)&0x7;
		  	tmp_div2 = (sysclk_st->sys_pll_ctrl1>>15)&0x7;
		  	tmp_RefDiv  = (sysclk_st->sys_pll_ctrl2>>24)&0x3f;
			sysclk_val = tmp_vco*12/tmp_div1/tmp_div2/tmp_RefDiv*1000000;
			break;
		case 3: //RCL
			sysclk_val = 80000;
			break;
	}
	return sysclk_val;
}

static void audiopll_power_down()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->audio_pll_ctrl1;
	tmp |= (APLL_FOUTPOSTDIVPD|APLL_DSMPD|APLL_DACPD|APLL_FOUT4PHASEPD|APLL_FOUTVCOPD);
	sysclk_st->audio_pll_ctrl1 = tmp;
	sysclk_regs_protect_close();
}

static void audiopll_power_up()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->audio_pll_ctrl1;
	tmp &= ~(APLL_FOUTPOSTDIVPD|APLL_DSMPD|APLL_DACPD|APLL_FOUT4PHASEPD|APLL_FOUTVCOPD);
	sysclk_st->audio_pll_ctrl1 = tmp;
	sysclk_regs_protect_close();
}

/*
 *  audiopll clk change 
 */
static void audiopll_change(silan_audiopllsel_t sel)
{
	uint32_t tmp;
	if(sel == AUDIOPLL_45M){	
		sysclk_regs_protect_open();
		tmp = sysclk_st->audio_pll_ctrl1;
		tmp &= ~APLL_FBDIV(0xfff);
		tmp |= APLL_FBDIV(45);
		sysclk_st->audio_pll_ctrl1 = tmp;	

		tmp = sysclk_st->audio_pll_ctrl2;
		tmp &= ~APLL_FRAC(0xffffff);
		tmp |= APLL_FRAC(2657511);

		sysclk_st->audio_pll_ctrl2 = tmp;	
		sysclk_regs_protect_close();
	}
	else if(sel == AUDIOPLL_49M){
		//???i
		sysclk_regs_protect_open();
		tmp = sysclk_st->audio_pll_ctrl1;
		tmp &= ~APLL_FBDIV(0xfff);
		tmp |= APLL_FBDIV(49);
		
		sysclk_st->audio_pll_ctrl1 = tmp;	
		
		tmp = sysclk_st->audio_pll_ctrl2;
		tmp &= ~APLL_FRAC(0xffffff);
		tmp |= APLL_FRAC(2550137);

		sysclk_st->audio_pll_ctrl2 = tmp;	
		sysclk_regs_protect_close();
	}

}
/*
 * para type = AUDIOPLL_45M , AUDIOPLL_49M
 */
static int pll_lock_cnt = 0;
void audiopll_config(silan_audiopllsel_t type)
{
#if 0
	audiopll_power_down();
	silan_apll_disable();
	audiopll_change(type);
	audiopll_power_up();
	silan_apll_enable();
#endif
    int fbdiv = 0;

    if (type == AUDIOPLL_45M){
        fbdiv = 45;
    }else if (type == AUDIOPLL_49M){
        fbdiv = 49;
    }
    if ((sysclk_st->audio_pll_ctrl1 & 0xfff) == fbdiv)
    {
        SL_DBGLOG("sel 45m 49m return\n");
        return;
    }

    pll_lock_cnt = 0;
reset:    
	audiopll_power_down();
	silan_apll_disable();
	audiopll_change(type);
	audiopll_power_up();
	if (silan_apll_enable() == 0){
        pll_lock_cnt++;
        
        if (pll_lock_cnt > 10){
            SL_LOG("[err]apll_enable\n");
            return;
        }
        SL_LOG("[err]apll_enable reset%d\n", pll_lock_cnt);
        goto reset;
    }
}

uint32_t silan_get_audiopll_config(void)
{
	uint32_t val;
	float fbdiv, frac;
	
	sysclk_regs_protect_open();
	fbdiv = sysclk_st->audio_pll_ctrl1 & 0xFFF;
	frac = sysclk_st->audio_pll_ctrl2 & 0xFFFFFF;
	sysclk_regs_protect_close();
	
	val = (fbdiv + frac/(2 << 23)) * 6000000;
	//SL_LOG("apll=%d", val);
	return val;
}

static void audiopll_init()
{
	uint32_t tmp;
	audiopll_power_down();
	
	sysclk_regs_protect_open();
	
	tmp = sysclk_st->audio_pll_ctrl1;
	tmp &= ~APLL_FBDIV(0xfff);
	tmp |= APLL_FBDIV(49);
	tmp &= ~APLL_POST_DIV1(0x7);
	tmp |= APLL_POST_DIV1(2);
	tmp &= ~APLL_POST_DIV2(0x7);
	tmp |= APLL_POST_DIV2(1);
	sysclk_st->audio_pll_ctrl1 = tmp;	

	tmp = sysclk_st->audio_pll_ctrl2;
	tmp &= ~APLL_REFDIV(0x3f);
	tmp |= APLL_REFDIV(1);
	tmp &= ~APLL_FRAC(0xffffff);
	tmp |= APLL_FRAC(2657511);
	sysclk_st->audio_pll_ctrl2 = tmp;	
	sysclk_regs_protect_close();
	
	audiopll_power_up();
	silan_apll_enable();
	
}

static void syspll_power_down()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_pll_ctrl1;
//	tmp |= SPLL_FOUTPOSTDIVPD;
	tmp |= (SPLL_DSMPD|SPLL_DACPD|APLL_FOUTVCOPD|APLL_FOUT4PHASEPD|SPLL_FOUTPOSTDIVPD);
	sysclk_st->sys_pll_ctrl1 = tmp;
	sysclk_regs_protect_close();
}

static void syspll_power_up()
{
	uint32_t tmp;
	sysclk_regs_protect_open();
	tmp = sysclk_st->sys_pll_ctrl1;
	tmp &= ~(SPLL_DSMPD|SPLL_DACPD|APLL_FOUTVCOPD|APLL_FOUT4PHASEPD|SPLL_FOUTPOSTDIVPD);
	sysclk_st->sys_pll_ctrl1 = tmp;
	sysclk_regs_protect_close();
}

static void syspll_change(uint8_t sel)
{
	uint32_t tmp;	
	sysclk_regs_protect_open();
	switch(sel)
	{
		case SYSPLL_60M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(15);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;

		case SYSPLL_80M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(20);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;

		case SYSPLL_100M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(25);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;
		
		case SYSPLL_120M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(30);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;
		case SYSPLL_140M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(35);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;
		
		case SYSPLL_160M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(40);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;
		case SYSPLL_180M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(45);
			sysclk_st->sys_pll_ctrl1 = tmp;
			break;
		case SYSPLL_200M:
			tmp = sysclk_st->sys_pll_ctrl1;
			tmp &= ~SPLL_FBDIV(0xfff);
			tmp |= SPLL_FBDIV(50);
			sysclk_st->sys_pll_ctrl1 = tmp;

			break;

	}
	sysclk_regs_protect_close();
}
/*
 * syspll init    default: 150M
 */
static void syspll_init()
{
	uint32_t tmp;
	//volatile int i = 0;
	syspll_power_down();
	
	sysclk_regs_protect_open();
	
	tmp = sysclk_st->sscg;
	tmp |= 0x1000; 
	sysclk_st->sscg = tmp;
	
	tmp = sysclk_st->sys_pll_ctrl2;
	tmp &= ~SPLL_REFDIV(0x3f);
	tmp |= SPLL_REFDIV(1);	
	sysclk_st->sys_pll_ctrl2 = tmp;	
	
	tmp = sysclk_st->sscg;
	tmp &= ~0x1000; 
	sysclk_st->sscg = tmp;
	
	tmp = sysclk_st->sys_pll_ctrl1;
//	tmp &= ~(SPLL_DSMPD|SPLL_DACPD|APLL_FOUTVCOPD|APLL_FOUT4PHASEPD);                      //VCO N pwd
	tmp &= ~SPLL_POST_DIV1(0x7);
	tmp |= SPLL_POST_DIV1(3); 
	tmp &= ~SPLL_POST_DIV2(0x7);
	tmp |= SPLL_POST_DIV2(1);
	tmp &= ~SPLL_FBDIV(0xfff);
	tmp |= SPLL_FBDIV(30);
	tmp |=  SPLL_FOUTVCOPD;
	sysclk_st->sys_pll_ctrl1 = tmp;

	tmp = sysclk_st->sscg;
	tmp &= ~(DOWNSPREAD|DIVVAL(0xff)|SPREAD(0xff)); 
#if 1 //no sscg
	tmp |= DISABLE_SSCG;
#else
	tmp &= ~DISABLE_SSCG;
	tmp |= SPREAD(0x1f);
#endif  
	tmp |= DIVVAL(8);
	tmp |= 0x1000; 
	sysclk_st->sscg = tmp;
	sysclk_regs_protect_close();
	
	syspll_power_up();
	silan_syspll_enable();
	
}
/*
 *  para  type  SYSPLL_60M   SYSPLL_90M ....
 */
void syspll_config(silan_syspllsel_t type)
{
	silan_syspll_disable();
	syspll_power_down();
	syspll_change(type);	
	syspll_power_up();
	silan_syspll_enable();
}

void sysclk_init_op(void)
{
	//uint32_t tmp;

	pre_sysclk_sel(SYSCLK_FROM_RCH,0);
	get_sysclk_val_settled();

	audiopll_init();
	syspll_init();

	pre_sysclk_sel(SYSCLK_FROM_PLL,SYSPLL_120M);
}

void silan_syspll_init(uint32_t sscg)
{
	uint32_t tmp;
	// volatile int i = 0;
	syspll_power_down();
	
	sysclk_regs_protect_open();
	
	tmp = sysclk_st->sscg;
	tmp |= 0x1000; 
	sysclk_st->sscg = tmp;
	
	tmp = sysclk_st->sys_pll_ctrl2;
	tmp &= ~SPLL_REFDIV(0x3f);
	tmp |= SPLL_REFDIV(1);	
	sysclk_st->sys_pll_ctrl2 = tmp; 
	
	tmp = sysclk_st->sscg;
	tmp &= ~0x1000; 
	sysclk_st->sscg = tmp;
	
	tmp = sysclk_st->sys_pll_ctrl1;
	//	tmp &= ~(SPLL_DSMPD|SPLL_DACPD|APLL_FOUTVCOPD|APLL_FOUT4PHASEPD);					   //VCO N pwd
	tmp &= ~SPLL_POST_DIV1(0x7);
	tmp |= SPLL_POST_DIV1(3); 
	tmp &= ~SPLL_POST_DIV2(0x7);
	tmp |= SPLL_POST_DIV2(1);
	tmp &= ~SPLL_FBDIV(0xfff);
	tmp |= SPLL_FBDIV(30);
	tmp |=	SPLL_FOUTVCOPD;
	sysclk_st->sys_pll_ctrl1 = tmp;
	
	tmp = sysclk_st->sscg;
	tmp &= ~(DOWNSPREAD|DIVVAL(0xff)|SPREAD(0xff)); 
	if(sscg == 0) { //no sscg
		tmp |= DISABLE_SSCG;
	}
    else {
		tmp &= ~DISABLE_SSCG;
		tmp |= SPREAD(0x1f);
    }
	tmp |= DIVVAL(8);
	tmp |= 0x1000; 
	sysclk_st->sscg = tmp;
	
	syspll_power_up();
	silan_syspll_enable();
	sysclk_regs_protect_close();
}

void silan_audiopll_init()
{
	uint32_t tmp;
	audiopll_power_down();
	
	sysclk_regs_protect_open();
	
	tmp = sysclk_st->audio_pll_ctrl1;
	tmp &= ~APLL_FBDIV(0xfff);
	tmp |= APLL_FBDIV(45);
	tmp &= ~APLL_POST_DIV1(0x7);
	tmp |= APLL_POST_DIV1(1);
	tmp &= ~APLL_POST_DIV2(0x7);
	tmp |= APLL_POST_DIV2(1);
	sysclk_st->audio_pll_ctrl1 = tmp;	

	tmp = sysclk_st->audio_pll_ctrl2;
	tmp &= ~APLL_REFDIV(0x3f);
	tmp |= APLL_REFDIV(2);
	tmp &= ~APLL_FRAC(0xffffff);
	tmp |= APLL_FRAC(2657511);
	sysclk_st->audio_pll_ctrl2 = tmp;	
	
	audiopll_power_up();
	silan_apll_enable();
	sysclk_regs_protect_close();
}

void silan_sysclk_change(silan_sysclk_ref_t sysclk_ref,silan_syspllsel_t if_clk_sel)
{
	//int ret;
	if (sysclk_ref == SYSCLK_FROM_PLL)
		pre_sysclk_sel(SYSCLK_FROM_RCH, 0);
	pre_sysclk_sel(sysclk_ref, if_clk_sel);
	//SL_LOG("\n\n\nclk_sys = %dM", get_sysclk_val_settled()/1000000);
}

