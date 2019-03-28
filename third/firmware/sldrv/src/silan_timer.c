//
// timer.c
// 
// TIMER      |   Usage
// Timer0     |   Do not use. 
// Timer1     |   System tick, fixed to 100uS. 
// Timer2     |   Delay count. 
// 

#define MODULE_NAME            "SILAN TIMER"
//#define __DEBUG__

#include "silan_syscfg.h"
#include "silan_syscfg_regs.h"
#include "silan_types.h"
#include "silan_timer_regs.h"
#include "silan_timer.h"
#include "silan_printf.h"

#if defined(__CC_ARM)||  defined ( __GNUC__ )
#define INST_PER_CYCLE		5
static u32 delay_calib = 8;
#endif

#if defined(__XCC__)
#define INST_PER_CYCLE		10
static u32 delay_calib = 16;
#endif

void silan_timer_start(u32 addr)
{
    silan_timer_count(addr,0x0);
    silan_timer_compare(addr,0xffffffff);
    silan_timer_enable(addr);
}
void silan_timer_stop(u32 addr)
{ 
    __sREG32(addr, TIMER_CTRL) = ~(TIMER_TO|TIMER_EN);   
}

void silan_timer_irq_start(u32 addr,u32 time)
{
    silan_timer_count(addr,0x0);
    silan_timer_compare(addr,time);
    silan_timer_irq_enable(addr);
}

void silan_timer_1ms_set(void)
{
	TIMER_1MS_COMPARE = silan_get_timer_cclk() / 1000;
}

void silan_timer_irq_reset(u32 addr)
{
   __sREG32(addr, TIMER_CTRL) = TIMER_EN|TIMER_INT;  
}

void silan_timer_irq_clear(u32 addr)
{
   __sREG32(addr, TIMER_CTRL) = ~TIMER_INT;  
}

void silan_timer_count(u32 addr,u32 count)
{
    __sREG32(addr, TIMER_COUNT) = count;  
}

void silan_timer_compare(u32 addr,u32 compare)
{
    __sREG32(addr, TIMER_COMPARE) = compare;  
}

void silan_timer_sel(u32 addr,u32 sel)
{
    __sREG32(addr, TIMER_SEL) = sel;     
}

void silan_timer_enable(u32 addr)
{
    __sREG32(addr, TIMER_CTRL) = TIMER_EN;     
}

void silan_timer_disable(u32 addr)
{
	__sREG32(addr, TIMER_CTRL) &= ~TIMER_EN;     
}

void silan_timer_to_clear(u32 addr)
{
    __sREG32(addr, TIMER_CTRL) &= ~TIMER_TO;        
}

u32 silan_timer_to_state(u32 addr)
{
	if(__sREG32(addr, TIMER_CTRL)& TIMER_TO)
        return 1;
    else
        return 0;    
}

void silan_timer_irq_enable(u32 addr)
{
   __sREG32(addr, TIMER_CTRL) = TIMER_EN|TIMER_INT; 
}

void silan_timer_irq_disable(u32 addr)
{
   __sREG32(addr, TIMER_CTRL) = ~(TIMER_EN|TIMER_INT); 
}

u32 silan_get_timer_count(u32 addr)
{
	return __sREG32(addr, TIMER_COUNT);
}
//todo
//u32 delay_calib = 100*1000;
void silan_udelay(u32 us)
{
	volatile u32 i,j;

	for(i=0;i<us;i++)
	{
		for(j=0;j<delay_calib;j++);
	}
}

void silan_delay_calib(void)
{
#if defined(__CC_ARM)||  defined ( __GNUC__ )
	delay_calib = silan_get_mcu_cclk()/1000000/INST_PER_CYCLE;
#endif
#if defined(__XCC__)
	delay_calib = silan_get_dsp_cclk()/1000000/INST_PER_CYCLE;
#endif
}

