#include <csi_config.h>


.section .bss
   .align 2
    .globl g_user_sp
    .type  g_user_sp, object
g_user_sp:
    .long 0
    .size g_user_sp, .-g_user_sp

.text
.align  1
.global __start_asyn_signal
.type   __start_asyn_signal, %function
__start_asyn_signal:
    psrset   ee
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    subi     sp, 68
    stm      r0-r13, (sp)
    stw      r15, (sp, 56)
#ifdef __CSKY_DSP__
    addi     sp, 60
    stm      r16-r31, (sp)
    subi     sp, 60
#endif
    mov      r1, sp
    mfcr     r0, epsr
#ifdef __CSKY_DSP__
    addi     r1, 64
#endif
    stw      r0, (r1, 60)
    mfcr     r0, epc
    stw      r0, (r1, 64)

    lrw       r7, g_user_sp
    stw       sp, (r7)
    
    /*lrw       sp, g_top_irqstack*/
    lrw       sp, msp_top

    lrw       r1, __Int_EngineAsynSignal
    mfcr      r0, psr
    lsri      r0, 16
    sextb     r0
    subi      r0, 32
    jsr       r1
    
    lrw       r0,   g_user_sp
    ldw       sp,   (r0)
/*
#ifdef CONFIG_STACK_GUARD
    mfcr      r3, cr<0, 4>
    bseti     r3, 0
    bseti     r3, 1
    mtcr      r3, cr<0, 4>
#endif
*/
    /*test start*/
    /*ldw      r0, (sp, 128)*/
    /*jbsr      epc_test*/
    /*test end*/

    ldm       r0-r13, (sp)

#ifdef __CSKY_DSP__
    addi      sp, 60
    ldm       r16-r31, (sp)
    addi      sp, 4
#endif

    ldw       r15, (sp, 64)
    mtcr      r15, epc
    ldw       r15, (sp, 60)
    bseti     r15, r15, 6
    mtcr      r15, epsr

#ifdef __CSKY_DSP__
    subi      sp, 64
#endif
    ldw       r15, (sp, 56)
    addi      sp, 68
#ifdef __CSKY_DSP__
    addi      sp, 64
#endif
    rte

.global __start_real
.type   __start_real, %function
__start_real:
    psrset   ee
#ifdef __CSKY_DSP__
    subi     sp, 64
#endif
    subi     sp, 68
    stm      r0-r13, (sp)
    stw      r15, (sp, 56)
#ifdef __CSKY_DSP__
    addi     sp, 60
    stm      r16-r31, (sp)
    subi     sp, 60
#endif
    mov      r1, sp
    mfcr     r0, epsr
#ifdef __CSKY_DSP__
    addi     r1, 64
#endif
    stw      r0, (r1, 60)
    mfcr     r0, epc
    stw      r0, (r1, 64)

    lrw       r7, g_user_sp
    stw       sp, (r7)

    /*lrw       sp, g_top_irqstack*/
    lrw       sp, msp_top

    lrw       r1, __Int_EngineAsynSignal
    mfcr      r0, psr
    lsri      r0, 16
    sextb     r0
    subi      r0, 32
    jsr       r1

    lrw       r0,   g_user_sp
    ldw       sp,   (r0)
/*
#ifdef CONFIG_STACK_GUARD
    mfcr      r3, cr<0, 4>
    bseti     r3, 0
    bseti     r3, 1
    mtcr      r3, cr<0, 4>
#endif
*/
    /*test start*/
    /*ldw      r0, (sp, 128)*/
    /*jbsr      epc_test*/
    /*test end*/

    ldm       r0-r13, (sp)

#ifdef __CSKY_DSP__
    addi      sp, 60
    ldm       r16-r31, (sp)
    addi      sp, 4
#endif

    ldw       r15, (sp, 64)
    mtcr      r15, epc
    ldw       r15, (sp, 60)
    bseti     r15, r15, 6
    mtcr      r15, epsr

#ifdef __CSKY_DSP__
    subi      sp, 64
#endif
    ldw       r15, (sp, 56)
    addi      sp, 68
#ifdef __CSKY_DSP__
    addi      sp, 64
#endif
    rte

.global Int_LowAtomStart
.type   Int_LowAtomStart, %function
Int_LowAtomStart:
    mfcr   r0,  psr
    psrclr ie
    rts

.global Int_LowAtomEnd
.type   Int_LowAtomEnd, %function
Int_LowAtomEnd:
    mtcr   r0,  psr
    rts
    
.global Int_HighAtomStart
.type   Int_HighAtomStart, %function
Int_HighAtomStart:
    mfcr   r0,  psr
    psrclr ie
    rts

.global Int_HighAtomEnd
.type   Int_HighAtomEnd, %function
Int_HighAtomEnd:
    mtcr   r0,  psr
    rts

.end

