#ifndef __SILAN_TIMER_H__
#define __SILAN_TIMER_H__

#include "silan_types.h"

#define TIMER_1MS_CNT	get_sysclk_val_settled()/2/1000

typedef enum {
	TIMER_0           = 0,
	TIMER_1,
	TIMER_2,
	TIMER_3,
	TIMER_4,
	TIMER_MS,
	TIMER_S,
}silan_timer_t;

/*************************************************************************/

void silan_timer_init(void);
void silan_timer_start(u32 addr);
void silan_timer_stop(u32 addr);
void silan_timer_irq_start(u32 addr,u32 time);
void silan_timer_irq_reset(u32 addr);
void silan_timer_irq_clear(u32 addr);
void silan_timer_disable(u32 addr);

u32 silan_get_timer_count(u32 addr);
void silan_timer_irq_enable(u32 addr);
void silan_timer_irq_disable(u32 addr);
void silan_timer_count(u32 addr,u32 count);
void silan_timer_compare(u32 addr,u32 compare);
void silan_timer_sel(u32 addr,u32 sel);
void silan_timer_enable(u32 addr);
u32 silan_timer_to_state(u32 addr);
void silan_timer_to_clear(u32 addr);
void silan_timer_1ms_set(void);

void silan_udelay(u32 us);
void silan_delay_calib(void);
	
#endif

