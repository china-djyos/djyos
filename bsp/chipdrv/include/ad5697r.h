// =============================================================================

// �ļ���     ��AD5697R.h
// ģ������: ADI��ADоƬ����
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��: 2/9.2017
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





