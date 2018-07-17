// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：PWM_SR1266.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 6/20.2017
// =============================================================================

#ifndef _BOARDDRV_SR1266_INCLUDE_PWM_SR1266_H_
#define _BOARDDRV_SR1266_INCLUDE_PWM_SR1266_H_

#include <stddef.h>

bool_t PWM_Init(void);
void PWM_Start(void);
void PWM_Stop(void);

#endif /* SRC_BSP_BOARDDRV_SR1266_INCLUDE_PWM_SR1266_H_ */
