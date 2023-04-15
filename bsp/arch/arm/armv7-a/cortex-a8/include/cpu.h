#ifndef __CPU_H__
#define __CPU_H__
#include "stdint.h"
#include "djyos.h"
#ifdef __cplusplus
extern "C" {
#endif

#define ADD_TO_TEST_DEBUG                __attribute__ ((section(".test_debug")))

#define _FIQ_STACK_SIZE_                  0xFF0
#define _IRQ_STACK_SIZE_                  0xFF0
#define _SVC_STACK_SIZE_                  0x3F0
#define _SYS_STACK_SIZE_                  0x3F0
#define _UND_STACK_SIZE_                  0x010
#define _ABT_STACK_SIZE_                  0x010

#define NOINT                             0xc0

#define BOOT_MODE_MASK                    0x1F
#define BOOT_MODE_USR                     0x10
#define BOOT_MODE_FIQ                     0x11
#define BOOT_MODE_IRQ                     0x12
#define BOOT_MODE_SVC                     0x13
#define BOOT_MODE_ABT                     0x17
#define BOOT_MODE_UND                     0x1B
#define BOOT_MODE_SYS                     0x1F
#define BOOT_FIQ_IRQ_MASK                 0xC0
#define BOOT_IRQ_MASK                     0x80

#define BOOT_COLOR_UNUSED                 0xAAAAAAAA      //Pattern to fill UNUSED stack
#define BOOT_COLOR_SVC                    0xBBBBBBBB      //Pattern to fill SVC stack
#define BOOT_COLOR_IRQ                    0xCCCCCCCC      //Pattern to fill IRQ stack
#define BOOT_COLOR_FIQ                    0xDDDDDDDD      //Pattern to fill FIQ stack
#define BOOT_COLOR_SYS                    0xEEEEEEEE      //Pattern to fill SYS stack

/*****************************/
/* CPU Mode                  */
/*****************************/
#define USERMODE        0x10
#define FIQMODE         0x11
#define IRQMODE         0x12
#define SVCMODE         0x13
#define ABORTMODE       0x17
#define UNDEFMODE       0x1b
#define MODEMASK        0x1f
#define NOINT           0xc0

void *__asm_reset_thread(ptu32_t (*thread_routine)(void),
                           struct ThreadVm  *vm);
void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                           struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_start_thread(struct ThreadVm  *new_vm);
void __asm_turnto_context(struct ThreadVm  *new_vm);
void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);

uint64_t __DjyGetTicks(void);
void DjyUpdateTicks(uint32_t ticks);
void DjySetUpdateTickFlag(bool_t flag);
bool_t DjyGetUpdateTickFlag(void);

#ifdef __cplusplus
}
#endif

#endif /*__CPU_H__*/

