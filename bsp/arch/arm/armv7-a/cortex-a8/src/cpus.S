/*#include "cpu.h"*/

.global __asm_reset_thread
.globl djy_switch_interrupt_flag
.globl djy_interrupt_from_thread
.globl djy_interrupt_to_thread


.equ NOINT,                             0xc0
.equ BOOT_MODE_MASK,                    0x1F
.equ BOOT_MODE_USR,                     0x10
.equ BOOT_MODE_FIQ,                     0x11
.equ BOOT_MODE_IRQ,                     0x12
.equ BOOT_MODE_SVC,                     0x13
.equ BOOT_MODE_ABT,                     0x17
.equ BOOT_MODE_UND,                     0x1B
.equ BOOT_MODE_SYS,                     0x1F
.equ BOOT_FIQ_IRQ_MASK,                 0xC0
.equ BOOT_IRQ_MASK,                     0x80
@----重置线程-----------------------------------------------------------------
@功能:  初始化线程的栈，并挂上线程执行函数，新创建线程时将会调用本函数
@参数:  thread_routine，被重置的线程函数
@               vm，线程虚拟机指针
@返回:  初始化结束后的当前栈指针
@函数原型:void __asm_reset_thread(void (*thread_routine)(void),
@                                   struct  tagThreadVm  *vm);
@-----------------------------------------------------------------------------
.section .text.__asm_reset_thread, "ax", %progbits
.global __asm_reset_thread
__asm_reset_thread:
    LDR     R2,[R1,#4]          @取虚拟机栈顶指针
    LDR     R3,=__DJY_VmEngine  @取虚拟机引擎指针
    STMFD   R2!,{R3}            @存pc，
    SUB     R2,R2,#13*4         @后退13个寄存器,初始状态r1-r12随意，__vm_engine
                                @函数不返回，lr也无意义
                                @__vm_engine的参数，切换上下文时，service_routine将恢复到r0中，根据调用约
                                @定，r0的值就是__vm_engine函数的参数。这只在创建并初始化线程上下文时需要这
                                @样做，线程启动以后线程切换要保存上下文时，保存r0的实际值即可
    STMFD   R2!,{R0}            @保存 thread_routine指针到r0的位置.
    LDR     R3,=BOOT_MODE_SVC   @取cpsr
    TST     R0, #0X01
    BEQ     __ASM_ARM
    ORR     R3, R3, #0X20           @; it's thumb code set T=1
__ASM_ARM:
    STMFD   R2!,{R3}                    @保存cpsr
    STR     R2,[R1]             @保存vm的当前栈指针到vm->stack中
    BX      LR
@ end of __asm_reset_thread
/*
@----复位老线程，切换到新线程-------------------------------------------------
@功能:  把old_vm的上下文重新初始化到新创建的状态，然后切换到新线程的
@       上下文中继续执行。
@       当一个在常驻内存的线程中处理的事件完成后，不能删除该线程，必须复位
@       该线程，然后切换到其他就绪线程，这时候就需要调用本函数。因此本函数在
@       被复位的线程上下文里面执行。
@参数:  thread_routine，被重置的线程函数
@       new_vm，目标线程的控制块指针
@       old_vm，被复位线程的控制块指针
@返回:  无
@函数原型:void __asm_reset_switch(void (*thread_routine)(void),
@                           struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_reset_switch
.type   __asm_reset_switch, %function
__asm_reset_switch:
    MOV     R5,  R1
    MOV     R1,  R2
    BL      __asm_reset_thread
    LDR     SP, [R5]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----启动多事件调度-----------------------------------------------------------
@功能: 系统初始化完成后，启动多事件调度并切入第一个事件.
@参数: new_vm，首次启动的线程控制块指针
@返回: 无
@函数原型: void __asm_start_thread(struct  tagThreadVm  *new_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_start_thread
.type   __asm_start_thread, %function
__asm_start_thread:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----切入上下文---------------------------------------------------------------
@功能:  不保存原上下文，直接切入新的上下文执行
@参数:  new_vm，待切入的线程控制块指针
@返回:  无
@函数原型: void __asm_turnto_context(struct  tagThreadVm  *new_vm)@
@说明:  当事件完成,就没有必要保存旧事件的上下文,直接切换到新事件即可.
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_turnto_context
.type   __asm_turnto_context, %function
__asm_turnto_context:
    LDR     SP, [R0]                @; get new task stack pointer
    LDMFD   SP!, {R4}               @; pop new task cpsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----上下文切换---------------------------------------------------------------
@功能:  保存当前线程的上下文，切换到新线程的上下文中继续执行。
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，切离线程的控制块指针，即&vm->stack。无需提供旧上下文栈指针，sp寄存器的当前值就是
@返回:  无
@函数原型: void __asm_switch_context(struct  tagThreadVm *new_vm,struct  tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context
.type   __asm_switch_context, %function
__asm_switch_context:
 /*保存旧线程，切出新线程*/
    STMFD   SP!, {LR}               @; push pc (lr should be pushed in place of pc)
    STMFD   SP!, {R0-R12, LR}       @; push lr & register file
    MRS     R4, CPSR
    tst     lr, #0x01
    beq     _ARM_MODE
    orr     r4, r4, #0x20           @; it's thumb code set T=1

_ARM_MODE:
    STMFD   SP!, {R4}               @; push cpsr
    STR     SP, [R1]                @; store sp in preempted tasks tcb

    LDR     SP, [R0]                @; get new task stack pointer

    LDMFD   SP!, {R4}               @; pop new task spsr
    AND     R4,  R4, #0Xffffff3f
    MSR     SPSR_cxsf, R4
    LDMFD   SP!, {R0-R12, LR, PC}^  @; pop new task r0-r12, lr & pc

/*
@----中断中的上下文切换-------------------------------------------------------
@功能:  保存被中断线程的上下文，切换到新线程的上下文中继续执行。本函数虽然在中
@       断服务函数(非用户用int_isr_connect函数连接的中断服务函数)中，但在ARM
@       中，却运行在svc态
@参数:  new_vm，切入线程的控制块指针
@参数:  old_vm，被中断线程的控制块指针
@返回:  无
@函数原型: void __asm_switch_context_int(struct tagThreadVm *new_vm,struct tagThreadVm *old_vm)@
@-----------------------------------------------------------------------------
*/
.align 2
.global __asm_switch_context_int
.type   __asm_switch_context_int, %function
__asm_switch_context_int:
    LDR     R2, =djy_switch_interrupt_flag
    MOV     R3, #1                          @; set flag to 1
    STR     R3, [R2]
    LDR     R2, =djy_interrupt_from_thread   @; set djy_interrupt_from_thread
    /*LDR     R1, [R1]*/
    STR     R1, [R2]
    LDR     R2, =djy_interrupt_to_thread     @; set djy_interrupt_to_thread
    /*LDR     R0, [R0]*/
    STR     R0, [R2]

    BX      LR

.align 2
.global __asm_delay_cycle
.type   __asm_delay_cycle, %function
__asm_delay_cycle:
    subs    r0 ,#6      /*减去运算的周期数*/
    lsr     r0 ,r0, #2  /*每次循环4个机械周期 每个机械周期 1个内核时钟*/
 loop:
    subs    r0 ,#1
    bne     loop
    subs    r1 ,#1
    bne     end
    mov     r0 ,#0xffffffff
    b       __asm_delay_cycle
end:
    bx      lr

.global __asm_bl_fun
.type   __asm_bl_fun, %function
__asm_bl_fun:
    BX   R0


/*
@----保存上下文---------------------------------------------------------------
@功能: 保存当前线程的上下文，但不切换,专用于进入CN_SLEEP_L3之前调用.
@参数: running_vm，当前线程的控制块指针，即&vm->stack。
@参数: SaveRamL3, 把执行过step1的内存保存到flash中.
@参数: EntrySleep, 进入低功耗.
@返回:  无
@函数原型: void __LP_BSP_AsmSaveReg(struct tagThreadVm *running_vm，
                    bool_t (*SaveRamL3)(void),
                    void (*EntrySleep)(void))
@-----------------------------------------------------------------------------
*/
.global __LP_BSP_AsmSaveReg
.type   __LP_BSP_AsmSaveReg, %function
__LP_BSP_AsmSaveReg:
    push    {lr}                /* 保存PC,退出L3低功耗后,将从本函数返回地址处继续运行*/
    push    {r0-r3,r12,lr}      /* 保存r0-r3,r12,lr */
    push    {r4-r11}
    str     sp,[r0]             /* 保存旧上下文栈指针到vm->stack */
    push    {r2}
    blx      r1                  /* 调用保存RAM到非易失存储器的函数*/
    pop     {r2}
    mov     r0, #0x4            /* 选择休眠方式:L3 */
    blx      r2                  /* 调用进入SLEEP3的函数 */
    bx      lr

 /*   BX   LR */


/*
; * void WFI(void);
; */
    .text
    .code 32
    .globl WFI
    .type WFI, %function
WFI:
    MOV R0, #0
    MCR p15, 0, R0, c7, c0, 4
    BX LR

;/*
; * void cp15_enable_alignfault(void)
; */
    .text
    .globl cp15_enable_alignfault
    .type cp15_enable_alignfault, %function
cp15_enable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    ORR R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

;/*
; * void cp15_disable_alignfault(void)
; */
    .text
    .globl cp15_disable_alignfault
    .type cp15_disable_alignfault, %function
cp15_disable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    BIC R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR

.text
.globl Int_GetPreStatus
.type Int_GetPreStatus, %function
Int_GetPreStatus:
    MRS     R0, SPSR
    and     R0,R0,#0x1f
    BX      LR
.text
.globl Int_GetStatus
.type Int_GetStatus, %function
Int_GetStatus:
    MRS     R0, CPSR
    BX      LR

.text
.globl GetLR
.type GetLR, %function
GetLR:
    mov     R0, LR
    BX      LR

/*如果硬件设计时，jtag口被占用，调用本函数将恢复jtag功能，shell命令 initjtag 的底层实现 */
.globl EnJtag
.type EnJtag, %function
EnJtag:
    push    {r0-r3}

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFFCFF
    and     r1,r2
    ldr     r2,=0x100
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFF3FF
    and     r1,r2
    ldr     r2,=0x400
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFFCFFF
    and     r1,r2
    ldr     r2,=0x1000
    orr     r1,r2
    str     r1,[r0]

    ldr     r0,=(0x0802800 + 46*4)
    ldr     r1,[r0]
    ldr     r2,=0xFFFF3FFF
    and     r1,r2
    ldr     r2,=0x4000
    orr     r1,r2
    str     r1,[r0]

    pop     {r0-r3}
    BX      LR

.end
