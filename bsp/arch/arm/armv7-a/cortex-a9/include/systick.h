#ifndef __SYSTICK_H__
#define __SYSTICK_H__
#ifdef __cplusplus
extern "C" {
#endif

__attribute__((weak))   u64 __DjyGetSysTime(void);
void Exp_SystickTickHandler(void);
void CleanWakeupEvent(void);
__attribute__((weak))   u64 __DjyGetSysTime(void);
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks);

#ifdef __cplusplus
}
#endif
#endif
