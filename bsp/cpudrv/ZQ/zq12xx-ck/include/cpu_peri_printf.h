#ifndef CPU_PERI_PRINTF_H_
#define CPU_PERI_PRINTF_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "silan_printf.h"

//#define   djy_printf               sl_printf
s32 djy_printf (const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif //CPU_PERI_PRINTF_H_