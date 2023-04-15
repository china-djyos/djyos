#ifndef __ARCH_FEATURE_H__
#define __ARCH_FEATURE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "arm32_feature.h"
#include "board-config.h"   //在startup目录下对应板件目录中,须在工程配置中加入 -I
#include "arch_stdint.h"

//中断禁能情况下，执行CPU休眠指令（wfi）时，如果中断已经悬起的，有两种可能情况：
//一是不会进入休眠，待中断使能后立即响应中断，二是进入休眠待下一次中断才唤醒。
//第一种情况比较常见，不知道有没有第二种情况的，第二种情况低功耗设计会非常麻烦。
#define CN_SLEEP_ON_INT     true        //true表示有中断悬起时能进入休眠

//保存上下文所需的栈空间最大值
//16=通用寄存器，+cpsr-SP
#define CN_CONTEXT_LIMIT        (16)*4

#ifdef __cplusplus
}
#endif
#endif // __ARCH_FEATURE_H__

