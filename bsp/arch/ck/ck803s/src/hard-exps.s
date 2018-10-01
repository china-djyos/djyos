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
    stw     r13, (sp)   /*����ԭ��r13��ֵ��ԭ����ջ����*/
    addi    sp, 4

    lrw     r13, g_trap_sp
    stw     sp, (r13)   /*����ԭ��SP��ֵ��g_trap_sp����*/

    lrw     sp, g_top_trapstack     /*�����µ�ջ*/

    mov     r0, r0
#ifdef __CSKY_DSP__
    subi    sp, 64
#endif
    subi    sp, 72
    stm     r0-r12, (sp)    /*����R0-R12���µ�ջ*/

    lrw     r0, g_trap_sp   /*��g_trap_sp�ĵ�ַ���浽R0*/
    ldw     r0, (r0)        /*��g_trap_sp��ֵ���浽R0*/

    subi    r0, 4
    ldw     r13, (r0)       /*��g_trap_sp-4��ֵ������R13*/

    stw     r13, (sp, 52)   /*��g_trap_sp-4��ֵ��������*/

    stw     r0, (sp, 56)    /*��g_trap_sp��ֵ���浽�µ�ջ����*/
    stw     r15, (sp, 60)   /*��R15���浽�µ�ջ����*/
#ifdef __CSKY_DSP__
    addi    sp, 64
    stm     r16-r31, (sp)   /*��R16-R31���浽�µ�ջ����*/
    subi    sp, 64
#endif
    mfcr    r0, epsr        /*��epsr��ֵ���浽R0*/
#ifdef __CSKY_DSP__
    addi    sp, 64
#endif
    stw     r0, (sp, 64)    /*��epsr��ֵ���浽�µ�ջ*/
    mfcr    r0, epc
    stw     r0, (sp, 68)    /*��epc��ֵ���浽�µ�ջ*/
#ifdef __CSKY_DSP__
    subi    sp, 64
#endif
    mov     r0, sp      /*��SP��ֵ��R0����ʱSP��ֵ�Ǳ���õĶ�ջ����͵�ַ*/
                        /*��û��__CSKY_DSP__����SP = g_top_trapstack - 72*/
                        /*���У���SP = g_top_trapstack - 136*/
    /* for backtrace */
    subi    sp, 8
    stw     r8, (sp, 0)
    stw     r15, (sp, 4)
    mov     r8, sp      /*�����ǰ�R0��R15��ֵ�ٱ���һ�Σ����Ұ�SP��ֵ�����R8����֪�Ǻ�����*/

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
