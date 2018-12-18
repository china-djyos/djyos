#include <csi_config.h>

/*.import trap_c*/
.import default_handler
.import HardExp_Trap_c
/*.import epc_test*/

.section .bss
    .align  2
    .globl  g_trapstackalloc
    .global g_trapstackbase
    .global g_top_trapstack
g_trapstackalloc:
g_trapstackbase:
    .space 768
g_top_trapstack:

    .align 2
    .globl g_trap_sp
    .type  g_trap_sp, object
g_trap_sp:
    .long 0
    .size g_trap_sp, .-g_trap_sp


.text

.align  1
.global exp_status_handler
.type   exp_status_handler, %function
exp_status_handler:
    psrset  ee

    subi    sp, 4
    stw     r13, (sp)   /*保存原来r13的值到原来的栈里面*/
    addi    sp, 4

    lrw     r13, g_trap_sp
    stw     sp, (r13)   /*保存原来SP的值到g_trap_sp里面*/

    lrw     sp, g_top_trapstack     /*换到新的栈*/

    mov     r0, r0
#ifdef __CSKY_DSP__
    subi    sp, 64
#endif
    subi    sp, 72
    stm     r0-r12, (sp)    /*保存R0-R12到新的栈*/

    lrw     r0, g_trap_sp   /*把g_trap_sp的地址保存到R0*/
    ldw     r0, (r0)        /*把g_trap_sp的值保存到R0*/

    subi    r0, 4
    ldw     r13, (r0)       /*把g_trap_sp-4的值保存在R13*/

    stw     r13, (sp, 52)   /*把g_trap_sp-4的值保存起来*/

    stw     r0, (sp, 56)    /*把g_trap_sp的值保存到新的栈里面*/
    stw     r15, (sp, 60)   /*把R15保存到新的栈里面*/
#ifdef __CSKY_DSP__
    addi    sp, 64
    stm     r16-r31, (sp)   /*把R16-R31保存到新的栈里面*/
    subi    sp, 64
#endif
    mfcr    r0, epsr        /*把epsr的值保存到R0*/
#ifdef __CSKY_DSP__
    addi    sp, 64
#endif
    stw     r0, (sp, 64)    /*把epsr的值保存到新的栈*/
    mfcr    r0, epc
    stw     r0, (sp, 68)    /*把epc的值保存到新的栈*/
#ifdef __CSKY_DSP__
    subi    sp, 64
#endif
    mov     r0, sp      /*把SP的值给R0，此时SP的值是保存好的堆栈的最低地址*/
                        /*若没有__CSKY_DSP__，则SP = g_top_trapstack - 72*/
                        /*若有，则SP = g_top_trapstack - 136*/
    /* for backtrace */
    subi    sp, 8
    stw     r8, (sp, 0)
    stw     r15, (sp, 4)
    mov     r8, sp      /*这里是把R0和R15的值再保存一次，并且把SP的值保存给R8，不知是何用意*/

    lrw     r1, HardExp_Trap_c
    jmp     r1
    
    rte /*never reach here*/

.align  1
.global CK_Trap0ISR
.type   CK_Trap0ISR, %function
CK_Trap0ISR:
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
    mov      r4, sp
    mfcr     r5, epsr
#ifdef __CSKY_DSP__
    addi     r4, 64
#endif
    stw      r5, (r4, 60)
    mfcr     r5, epc
    addi     r5, 4
    stw      r5, (r4, 64)
    
    st.w     sp, (r1)
    
    ld.w     sp, (r0)

    /*test start*/
    /*ldw      r0, (sp, 128)*/
    /*jbsr      epc_test*/
    /*test end*/

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
    
.end
