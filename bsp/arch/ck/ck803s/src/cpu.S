#include <csi_config.h>

.import g_user_sp

.text

.global __asm_reset_thread
.type   __asm_reset_thread, %function
__asm_reset_thread:
    psrclr   ie, fe
    ldw      r2, (r1, 4)
    subi     r2, 4
    lrw      r3, __DJY_VmEngine
    stw      r3, (r2, 0)
    lrw      r3, 0xe0000140
    subi     r2, 4
    stw      r3, (r2, 0)
#ifdef __CSKY_DSP__
    subi     r2, 64
#endif
    subi     r2, 60
    stw      r0, (r2,56)
    stw      r0, (r2)
    stw      r2, (r1)
    rts
    
.global __asm_reset_switch
.type   __asm_reset_switch, %function
__asm_reset_switch:
    psrclr   ie, fe
    ldw      sp, (r2, 4)
    subi     sp, 4
    lrw      r3, __DJY_VmEngine
    stw      r3, (sp, 0)
    lrw      r3, 0xe0000140
    subi     sp, 4
    stw      r3, (sp, 0)
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    subi     sp, 60
    stw      r0, (sp,56)
    stw      r0, (sp)
    stw      sp, (r2)

    ld.w     sp, (r1)
    ldm      r0-r13, (sp)
    #ifdef __CSKY_DSP__
    addi     sp, 60
    ldm      r16-r31, (sp)
    addi     sp, 4
#endif
    ldw      r15, (sp, 64)
    mtcr     r15, epc
    ldw      r15, (sp, 60)
    bseti    r15, r15, 6
    mtcr     r15, epsr
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    ldw      r15, (sp, 56)
    addi     sp, 68
#ifdef __CSKY_DSP__
    addi     sp, 64
#endif
    rte 
    
.global __asm_start_thread
.type   __asm_start_thread, %function
__asm_start_thread:
    psrclr   ie, fe
    ld.w     sp, (r0)
    
    ldm      r0-r13, (sp)
#ifdef __CSKY_DSP__
    addi     sp, 60
    ldm      r16-r31, (sp)
    addi     sp, 4
#endif
    ldw      r15, (sp, 64)
    mtcr     r15, epc
    ldw      r15, (sp, 60)
    bseti     r15, r15, 6
    mtcr     r15, epsr
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    ldw      r15, (sp, 56)
    addi     sp, 68
#ifdef __CSKY_DSP__
    addi     sp, 64
#endif
    /*mtcr     r0, epc*/
    rte
    
.global __asm_turnto_context
.type   __asm_turnto_context, %function
__asm_turnto_context:
    psrclr   ie, fe
    ld.w     sp, (r0)
    
    ldm      r0-r13, (sp)
#ifdef __CSKY_DSP__
    addi     sp, 60
    ldm      r16-r31, (sp)
    addi     sp, 4
#endif
    ldw      r15, (sp, 64)
    mtcr     r15, epc
    ldw      r15, (sp, 60)
    bseti     r15, r15, 6
    mtcr     r15, epsr
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    ldw      r15, (sp, 56)
    addi     sp, 68
#ifdef __CSKY_DSP__
    addi     sp, 64
#endif
    rte
    
.global __asm_switch_context
.type   __asm_switch_context, %function
__asm_switch_context:
    psrset  ee
    trap    0
    rts

.global __asm_switch_context_int
.type   __asm_switch_context_int, %function
__asm_switch_context_int:
    lrw     r3,  g_user_sp
    ldw     r2,  (r3)
    ldw     r0,  (r0)
    stw     r0,  (r3)
    stw     r2,  (r1)
    rts

.global __asm_delay_cycle
.type   __asm_delay_cycle, %function
__asm_delay_cycle:
    lsri    r0 ,r0, 5
    bez     r0, carry
loop:
    subi    r0, 1
    bnez    r0, loop
carry:
    bez     r1, end
    subi    r1 , 1
    lrw     r0 ,0xffffffff
    br      __asm_delay_cycle
 end:
    rts
/*
    subi    r0 , 6
    lsri    r0 ,r0, 0
 loop:
    subi    r0 , 1
    blsz    r0, loop
    bez     r1, end
    subi    r1 , 1
    blsz    r1, end
    lrw     r0 ,0xffffffff
    br      __asm_delay_cycle
end:
    rts
*/
.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    jsr   r0
    rts
.end
