#ifndef __CPU_H__
#define __CPU_H__
#include "stdint.h"
#include "djyos.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void __asm_reset_switch(ptu32_t (*thread_routine)(void), struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_turnto_context(struct ThreadVm  *new_vm);
extern void __asm_start_thread(struct ThreadVm  *new_vm);
extern void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);

#define Djy_GetFineTime DJY_GetSysTime

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

