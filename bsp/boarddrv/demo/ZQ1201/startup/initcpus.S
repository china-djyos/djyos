#ifdef CFG_CK803S

.import CK_Trap0ISR
.import Iboot_IAP_SelectLoadProgam
.import exp_status_handler
.import __start_asyn_signal
.import msp_top
/*
    .section .data.InitStack

    .align  2
    .globl  msp_stack
    .global msp_bottom
    .global msp_top
msp_stack:
msp_bottom:
    .space  4096
msp_top:
*/
    .section .StartupExpTbl
    .align  10
    .globl  __Vectors_Tbl
    .type   __Vectors_Tbl, @object
__Vectors_Tbl:
    /*.long   Reset_Handler*/
    .long   Init_Cpu
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   CK_Trap0ISR

    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    
    .long   default_handler
    
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    .long   default_handler
    
    /*全部设成异步信号引擎*/
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal    /*  default interrupt entry  */
    .long   __start_asyn_signal      /*  default interrupt entry  */

    .size    __Vectors_Tbl, . - __Vectors_Tbl

    .text
    .align   1
    .globl   Init_Cpu
    .type    Init_Cpu, %function
Init_Cpu:
    lrw      r0, 0x80000000

    mtcr     r0, psr
    
    lrw      a1, msp_top
    mov      sp, a1
    
    jbsr    Init_Cpuc

    .section .text.AppStart
    .align   1
    .globl   AppStart
    .type    AppStart, %function
AppStart:
    lrw      r0, 0x80000000

    mtcr     r0, psr

    lrw      a1, msp_top
    mov      sp, a1

    jbsr     Iboot_LoadPreload

.text
   .align  1
    .weak   default_handler
    .type   default_handler, %function
default_handler:
    br      exp_status_handler
    .size   default_handler, . - default_handler

    .end
#endif
