// =============================================================================

// 文件名     ：AD5697R.h
// 模块描述: ADI的AD芯片驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 2/9.2017
// =============================================================================

#ifndef _CHIP_AD5697R_H_
#define _CHIP_AD5697R_H_

#ifdef __cplusplus
extern "C" {
#endif

bool_t AD5697R_Output1(u16 DacValue1);
bool_t AD5697R_Output2(u16 DacValue2);
bool_t AD5697R_OutputAll(u16 DacValue1,u16 DacValue2);

#ifdef __cplusplus
}
#endif

#endif





