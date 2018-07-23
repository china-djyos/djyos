// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��timer.h
// ģ������: sr1263��ʱ��ģ���ͷ�ļ�API
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/31.2017
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
