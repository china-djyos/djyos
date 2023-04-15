#ifndef __CPU_PERI_TIMER_H__
#define __CPU_PERI_TIMER_H__
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

struct TIMER_REG
{
    vu32 TCFG0;     //0x51000000
    vu32 TCFG1;     //0x51000004
    vu32 TCON;      //0x51000008
    vu32 TCNTB0;    //0x5100000C
    vu32 TCMPB0;    //0x51000010
    vu32 TCNTO0;    //0x51000014
    vu32 TCNTB1;    //0x51000018
    vu32 TCMPB1;    //0x5100001C
    vu32 TCNTO1;    //0x51000020
    vu32 TCNTB2;    //0x51000024
    vu32 TCMPB2;    //0x51000028
    vu32 TCNTO2;    //0x5100002C
    vu32 TCNTB3;    //0x51000030
    vu32 TCMPB3;    //0x51000034
    vu32 TCNTO3;    //0x51000038
    vu32 TCNTB4;    //0x5100003C
    vu32 TCNTO4;    //0x51000040
};

#define TIMER_REG_BASE  ((struct TIMER_REG*)0x51000000)

void Timer_SetClkSource(ufast_t timer,ufast_t value);
void Timer_SetPrecale(ufast_t group,uint16_t value);
void Timer_SetCounter(ufast_t timer,uint16_t value);
void Timer_SetCompare(ufast_t timer,uint16_t value);
bool_t Timer_SetType(ufast_t timer,ufast_t type);
void Timer_Reload(ufast_t timer);
bool_t Timer_Start(ufast_t timer);
bool_t Timer_Stop(ufast_t timer);
uint16_t Timer_Read(ufast_t timer);

#ifdef __cplusplus
}
#endif

#endif //__CPU_PERI_TIMER_H__
