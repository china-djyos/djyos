#ifndef __BYPASS_TIMER_H
#define __BYPASS_TIMER_H

extern void Bypass_TimerInit(void);
extern u64 Bypass_GetCnt(void);
extern u64 Bypass_Cnt_to_Systick(void);
extern void Bypass_Timer_PerInit(void);

#endif
