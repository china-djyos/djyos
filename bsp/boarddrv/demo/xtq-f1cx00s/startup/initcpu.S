//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2023，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:  CPU初始化
 //作者：     lst
 //版本：      V1.0.0
 //文件描述:  CPU初始化必须用汇编语言实现的部分
//其他说明:  无
//修订历史:

 //1. 日期: 2015-02-22
 //  作者: lst
 //  新版本号：V1.0.0
 //  修改说明:
 //------------------------------------------------------

#include <board-config.h>
//.fpu    softvfp
.file   "initcpu.S"


//// CP15 R1
#define R1_M    (1<<0)
#define R1_A    (1<<1)
#define R1_C    (1<<2)
#define R1_W    (1<<3)
#define R1_Z    (1<<11)
#define R1_I    (1<<12)
#define R1_V    (1<<13)     //high vector
#define R1_RR   (1<<14)
#define R1_VE   (1<<24)
#define R1_nF   (1<<30)
#define R1_iA   (1<<31)

/*
 * Macro for switching ARM mode
 */
 .align 5
    .macro  BOOT_CHANGE_MODE, mode, mode_mask
    MRS   R0, CPSR
    BIC   R0, R0, #\mode_mask
    ORR   R0, R0, #\mode
    MSR   CPSR_c, R0
    .endm

/* ========================================================================
 *                      Stack and Heap Definitions
 * ========================================================================
 */
/*各种异常不会嵌套，用同一个栈*/
    .global cp15_enable_alignfault
    .section .data
    .space 0x1000
    .align 3
    .global und_stack_top
    .global abt_stack_top
und_stack_top:
abt_stack_top:
    .word 0

.text
.align 5
.globl ExpStackInit
.type ExpStackInit, %function
ExpStackInit:
    BOOT_CHANGE_MODE 0x1F 0x1F  /*System mode 系统模式*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x17 0x1F /*Abort mode */
    ldr     sp,=abt_stack_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x1B 0x1F  /*Undefined mode*/
    ldr     sp,=und_stack_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x12 0x1F/*IRQ mode*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07

    BOOT_CHANGE_MODE 0x11 0x1F /*FIQ mode*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07
    sub     sp,sp,#0x44        /*IRQ响应到禁止FIQ之前，要用56字节，必须避开，否则会被覆盖*/

    //Clear FIQ banked registers while in FIQ mode
    MOV     R8,  #0
    MOV     R9,  #0
    MOV     R10, #0
    MOV     R11, #0
    MOV     R12, #0

    BOOT_CHANGE_MODE 0x13 0x1F  /*svc mode管理模式*/
    ldr     sp,=msp_top
    bic     sp,sp,#0x07
    bx      lr
    //==================

.text
.align 5
.globl Init_Cpu
.global isr_vector
.global Cache_CleanData
.type Init_Cpu, %function
Init_Cpu:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0
    ldr     sp,=msp_top
    ldr   r0,=0
    mcr   p15, 0, r0, c1, c0, 0

    mov     r0,#0
    mcr     p15,0,r0,c7,c7,0    //Invalidate I+D Cache
    mcr     p15,0,r0,c8,c7,0    //Invalidate I+D TLB

    BL    ExpStackInit

    ldr   r0,=CN_CFG_MCLK
    bl    Sys_Clock_Init

    bl    MMU_Init

//copy向量表，全志的f1cx00在0地址处在reset时存放了boot信息，需要在启动后手动copy
//异常向量表。
    ldr r0, =isr_vector
    mrc p15, 0, r2, c1, c0, 0  //读取 CP15
    ands r2, r2, #(1 << 13)    //取bit13
    ldreq r1, =0x00000000      //bit13=0
    ldrne r1, =0xFFFF0000      //bit13=1


    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}
    ldmia r0!, {r2-r8, r10}
    stmia r1!, {r2-r8, r10}

    //Clear Registers
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0

    bl  cp15_enable_alignfault
    ldr pc, =Iboot_IAP_SelectLoadProgam



//注意，Init_Cpu将在Iboot中被连接，AppStart将在APP中被连接。
//ExpStackInit中各状态的栈指针，将由Iboot.lds或app.lds分配，其值可能不一样。
.section ".text.AppStart", "ax"
.align 5
.globl AppStart
.type AppStart, %function
AppStart:
    //Disable IRQ and FIQ before starting anything
    MRS   R0, CPSR
    ORR   R0, R0, #0xC0
    MSR   CPSR_c, R0

    BL    ExpStackInit

    bl    MMU_Init
//copy向量表，全志的f1cx00在0地址处在reset时存放了boot信息，需要在启动后手动copy
//异常向量表。
    ldr r0, =isr_vector
    mrc p15, 0, r2, c1, c0, 0  //读取 CP15
    ands r2, r2, #(1 << 13)    //取bit13
    ldreq r1, =0x00000000      //bit13=0
    ldrne r1, =0xFFFF0000      //bit13=1

    add   r0,r0,#4              //避开复位向量，复位的话，要跑 Init_Cpu
    add   r1,r1,#4
    ldmia r0!, {r2-r8}
    stmia r1!, {r2-r8}

    add   r0,r0,#4              //避开复位向量，复位的话，要跑 Init_Cpu
    add   r1,r1,#4
    ldmia r0!, {r2-r8}
    stmia r1!, {r2-r8}

    //Clear Registers
    MOV R0,  #0
    MOV R1,  #0
    MOV R2,  #0
    MOV R3,  #0
    MOV R4,  #0
    MOV R5,  #0
    MOV R6,  #0
    MOV R7,  #0
    MOV R8,  #0
    MOV R9,  #0
    MOV R10, #0
    MOV R11, #0
    MOV R12, #0

    bl  cp15_enable_alignfault
    bl  Iboot_LoadPreload

#if 0   //这段代码是小淘气初始化页表前后的代码，查问题时用过。
.text
.globl entry_mmu
.type entry_mmu, %function
entry_mmu:
//  sysFlushCache(D_CACHE);
tci_loop:
    MRC p15, 0, r15, c7, c14, 3
    BNE tci_loop
    MOV r0, #0x0
    MCR p15, 0, r0, c7, c10, 4 /* drain write buffer */

//  arm32_dcache_disable();
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #4
    mcr p15, 0, r0, c1, c0, 0

//  arm32_icache_disable();
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x1000
    mcr p15, 0, r0, c1, c0, 0

//  arm32_mmu_disable();
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x1
    mcr p15, 0, r0, c1, c0, 0

    BX      LR

.globl exit_mmu
.globl mmu_page_table
.type exit_mmu, %function
exit_mmu:
//  arm32_ttb_set((uint32_t)(ttb));//写入页表地址
    ldr r0,=mmu_page_table
    mcr p15, 0, r0, c2, c0, 0

//  arm32_tlb_invalidate();
    ldr r0,=0
    mcr p15, 0, r0, c7, c10, 4
    mcr p15, 0, r0, c8, c6, 0
    mcr p15, 0, r0, c8, c5, 0

//arm32_domain_set(0x1);
    ldr r0, =1
    mcr p15, 0, r0, c3, c0, 0

//arm32_mmu_enable();
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1
    mcr p15, 0, r0, c1, c0, 0

//arm32_icache_enable();
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #0x1000
    mcr p15, 0, r0, c1, c0, 0

//arm32_dcache_enable();
    mrc p15, 0, r0, c1, c0, 0
    orr r0, r0, #4
    mcr p15, 0, r0, c1, c0, 0

    BX      LR
#endif
    .end

