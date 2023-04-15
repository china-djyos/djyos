#ifndef __ARCH_FEATURE_H__
#define __ARCH_FEATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm32_feature.h"
#include "board-config.h"

#define arm926ej_s

//虽然cache.h中有定义cache尺寸的地方,但926ej_s的cache尺寸由厂商决定，直接与CPU
//实现相关,故需要在这里定义.
#define CN_CACHE_SIZE           0x4000

//保存上下文所需的栈空间最大值
//16=通用寄存器，+cpsr-SP
#define CN_CONTEXT_LIMIT        (16)*4

#ifdef __cplusplus
}
#endif
#endif // __ARCH_FEATURE_H__

