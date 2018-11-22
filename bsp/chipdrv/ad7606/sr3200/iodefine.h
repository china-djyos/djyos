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

#define ADCONVS_PORT    (2)
#define ADCONVS_PIN     (7)                             // ADSH����P2.7

#define ADRST_PORT      (2)
#define ADRST_PIN       (6)                             // ADRST����P2.6

#define ADBUSY_PORT     (2)
#define ADBUSY_PIN      (8)                             // ADBUSY����P2.8

#define ADCS_PORT       (0)
#define ADCS_PIN        (16)                             // ADFS����P0.16

#define ADOS0_PORT      (2)                             // ADOS����P2.3
#define ADOS0_PIN       (3)
#define ADOS1_PORT      (2)                             // ADOS����P2.4
#define ADOS1_PIN       (4)
#define ADOS2_PORT      (2)                             // ADOS����P2.5
#define ADOS2_PIN       (5)


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __AD7606_H__ */
