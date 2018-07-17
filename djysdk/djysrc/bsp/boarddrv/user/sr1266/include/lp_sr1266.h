// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：lp_SR1266.c
// 模块描述: SR1266板板低功耗方案源代码及实现
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 6/20.2017
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
