#ifndef ____CPU_PERI_IWDG_H___
#define ____CPU_PERI_IWDG_H___
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif


struct WDT_REG
{
    vu32 WTCON;     //0x53000000
    vu32 WTDAT;     //0x53000004
    vu32 WTCNT;     //0x53000008
};

#define WDT_REG_BASE    ((struct WDT_REG*)0x53000000)

#ifdef __cplusplus
}
#endif

#endif //____CPU_PERI_IWDG_H___
