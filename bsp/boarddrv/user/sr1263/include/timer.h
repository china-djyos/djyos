// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：timer.h
// 模块描述: sr1263定时器模块的头文件API
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 5/31.2017
// =============================================================================

#ifndef __SR1263_TIMER_H__
#define __SR1263_TIMER_H__
#include <stdint.h>

bool_t Timer_Init(u32 IntCycle,u32 (*IsrHook)(void) );
bool_t Timer_Start(void);
bool_t Timer_IntCycleSet(u32 IntCycle);
u32 Timer_GetTime(void);
bool_t Timer_CounterReset(void);

#endif
