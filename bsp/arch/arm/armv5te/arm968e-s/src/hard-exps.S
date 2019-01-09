.globl boot_reset
.globl irq_handler
.globl fiq_handler

/*
; * void WFI(void);
; */
    .globl WFI
    .type WFI, %function
WFI:
    MOV R0, #0
    MCR p15, 0, R0, c7, c0, 4
    BX LR

;/*
; * void cp15_enable_alignfault(void)
; */
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
    .globl cp15_disable_alignfault
    .type cp15_disable_alignfault, %function
cp15_disable_alignfault:
    MRC p15, 0, R0, c1, c0, 0
    BIC R0, R0, #0x02
    MCR p15, 0, R0, c1, c0, 0
    BX LR


    .section ".vectors", "ax"
.align  3
.global isr_vector
isr_vector:
    b   boot_reset
    ldr pc, HardExp_UndefJump
    ldr pc, HardExp_SwiJump
    ldr pc, HardExp_PrefetchJump
    ldr pc, HardExp_DataAbortJump
    b   .
    ldr pc, HardExp_IrqJump
    ldr pc, HardExp_FiqJump



ResetAddr:
        .word boot_reset
HardExp_UndefJump:
        .word HardExp_UndefHandler
HardExp_SwiJump:
        .word HardExp_SwiHandler
HardExp_PrefetchJump:
        .word HardExp_PrefetchHandler
HardExp_DataAbortJump:
        .word HardExp_DataAbortHandler
Nouse:
        .word 0
HardExp_IrqJump:
        .word irq_handler
HardExp_FiqJump:
        .word fiq_handler

.global isr_vector_end
isr_vector_end:
                .word 0x00000000


HardExp_UndefHandler:
    b   .
HardExp_SwiHandler:
    b   .
HardExp_PrefetchHandler:
    b   .
    nop
    nop
    subs        pc,lr,#0

HardExp_DataAbortHandler:
    nop
    nop
    b   .
    nop
    nop
    subs pc,lr,#0


.end

