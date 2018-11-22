// =============================================================================

// �ļ���     ��AD7606.h
// ģ������: AD7606�ɼ�ģ��ĵײ�����ͷ�ļ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 09/04.2015
// =============================================================================
#ifndef __AD7606_H__
#define __AD7606_H__

#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

void AD7606_Init(void);
bool_t AD7606_GetValue(uint16_t *AinBuf);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AD7606_H__ */
