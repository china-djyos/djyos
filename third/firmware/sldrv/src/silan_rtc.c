/*
 * silan_rtc.c
 *
 * Modify Date: 2016-4-28 18:10
 * MaintainedBy: yerenfeng <yerenfeng@silan.com.cn>
 */

#include "ap1508.h"
#include "silan_rtc_regs.h"
#include "silan_rtc.h"
#include "silan_printf.h"
#include "silan_syscfg.h"

void silan_rtcmem_ldo_onoff(u8 onoff)
{
	u8 tmp;
	tmp = __sREG32(RTC_BASE,RTC_CS0);
	if(onoff)
		tmp |= 0x80;   //ldo on
	else
		tmp &= 0x7f;   //ldo off

	__sREG32(RTC_BASE,RTC_CS0) = tmp;
}

void silan_soc_ldo_config(u8 level)
{
	u8 tmp;
	silan_vlsp_cclk_config(CLK_ON);
	tmp = __sREG32(RTC_BASE,RTC_WEEK_ALARM) ;
	tmp &= ~(0xf0);
	tmp |= (level<<4);
	__sREG32(RTC_BASE,RTC_WEEK_ALARM) = tmp;
}

inline void silan_rtc_reg_set(u32 offset, u8 value)
{
	__sREG32(RTC_BASE, offset) = value;     
}

inline u32 silan_rtc_reg_get(u32 offset)
{
	return (__sREG32(RTC_BASE, offset));   
}

inline void silan_rtcmem_write(u32 offset, u8 value)
{
	__sREG32(SILAN_RTCMEM_BASE, (offset<<2)) = value; 
}

inline u32 silan_rtcmem_read(u32 offset)
{
	return __sREG32(SILAN_RTCMEM_BASE, (offset<<2)); 
}

void silan_rtc_init(void)
{
	u8 tmp = 0;  
	silan_vlsp_cclk_config(CLK_ON);
	silan_rtcmem_ldo_onoff(1);
	//pvt
	tmp = silan_rtc_reg_get(RTC_CLKOUT_CON);
}
