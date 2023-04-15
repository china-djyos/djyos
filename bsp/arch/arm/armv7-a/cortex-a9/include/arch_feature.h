#ifndef __ARCH_FEATURE_H__
#define __ARCH_FEATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm32_feature.h"
#include "board-config.h"

#define arm926ej_s

//��Ȼcache.h���ж���cache�ߴ�ĵط�,��926ej_s��cache�ߴ��ɳ��̾�����ֱ����CPU
//ʵ�����,����Ҫ�����ﶨ��.
#define CN_CACHE_SIZE           0x4000

//���������������ջ�ռ����ֵ
//16=ͨ�üĴ�����+cpsr-SP
#define CN_CONTEXT_LIMIT        (16)*4

#ifdef __cplusplus
}
#endif
#endif // __ARCH_FEATURE_H__

