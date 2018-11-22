
#ifndef CPU_PERI_WAT_H
#define CPU_PERI_WAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stddef.h"

extern void WDT_Start(void);
extern void WDT_Reboot(void);
extern bool_t WatDog_Install(void);

#ifdef __cplusplus
}
#endif

#endif












































































