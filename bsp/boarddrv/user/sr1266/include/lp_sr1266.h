// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��lp_SR1266.c
// ģ������: SR1266���͹��ķ���Դ���뼰ʵ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 6/20.2017
// =============================================================================

#ifndef _BOARDDRV_SR1266_INCLUDE_LP_SR1266_H_
#define _BOARDDRV_SR1266_INCLUDE_LP_SR1266_H_
#include <stddef.h>

void SR1266_LowPowerCfg(void);
void GPIO_LowPower(void);
u32 EntrySleepReCall(u32 SleepLevel);
u32 ExitSleepReCall(u32 SleepLevel);
bool_t __LP_BSP_RestoreRamL3(void);
bool_t __LP_BSP_SaveRamL3(void);
void SR1266_LowPowerCfg(void);

#endif /* SRC_BSP_BOARDDRV_SR1266_INCLUDE_LP_SR1266_H_ */
