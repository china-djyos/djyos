 
#ifndef _CPU_PERI_REG_
#define _CPU_PERI_REG_

#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"

void CPU_Reboot(u32 key);
void CPU_Reset(u32 key);
void CPU_RestartSystem(u32 key);

#ifdef __cplusplus
}
#endif

#endif 



