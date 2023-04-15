#include "stdint.h"

//---- MMU段映射 ----------------------------------------------------------------
//描述: 将物理段地址映射到虚拟地址.
//参数：tt_base: 转换表基址.
//      va_start: 虚拟地址(必须是1MB地址对齐).
//      pa_start: 物理地址(必须是1MB地址对齐).
//      sec_count: 需要映射的段数量.
//      attr: 段属性.
// C B   写通型cache         写回型cache         可选型cache
// 0 0   禁cache，禁写缓冲   禁cache，禁写缓冲   禁cache，禁写缓冲
// 0 1   禁cache，开写缓冲   禁cache，开写缓冲   禁cache，开写缓冲
// 1 0   开cache，禁写缓冲   不可预测            写通开cache，开写缓冲
// 1 1   开cache，开写缓冲   开cache，开写缓冲   写回开cache，开写缓冲
//返回：无.
//------------------------------------------------------------------------------

void MMU_MapSection(u32 *tt_base,u32 va_start,u32 pa_start,u32 sec_count,u32 attr)
{
    u32 *page_tbl;
    int i;

    page_tbl  = (u32 *)tt_base;
    page_tbl += (va_start>>20);
    pa_start  = pa_start>>20;

    for(i=0;i<sec_count;i++)
    {
        page_tbl[i] =((pa_start+i)<<20) | (attr);
    }

}

// ---- mmu段映射--------------------------------------------------------------
//功能: 将虚拟地址vaddr开始的一个段(1M)映射到物理地址paddr中，同时设置该段的
//      cache及写缓冲属性。映射后，访问vaddr将被mmu重定向到paddr中。
//页表条目C(bit3),B(bit2)位含义定义:
// C B   写通型cache         写回型cache         可选型cache
// 0 0   禁cache，禁写缓冲   禁cache，禁写缓冲   禁cache，禁写缓冲
// 0 1   禁cache，开写缓冲   禁cache，开写缓冲   禁cache，开写缓冲
// 1 0   开cache，禁写缓冲   不可预测            写通开cache，开写缓冲
// 1 1   开cache，开写缓冲   开cache，开写缓冲   写回开cache，开写缓冲
//返回：无
//-----------------------------------------------------------------------------
/*
void xxMMU_MapSection(u32 vaddr, u32 paddr, int properties)
{
    mmu_page_table[vaddr>>20] = (paddr & 0xfff00000)|(properties&0xFFF);

}*/

// ---- 将某个entry锁定在TLB中 ----
// 将某些地址变换锁定在TLB中，这样对于这些虚拟地址到物理地址的访问就会变得更加迅速
// 需要加参数
// ---- C10
void MMU_LockEntry()
{

}

// ---- 使指令TLB中的一个条目无效 ----
// ---- C8
void MMU_InvalidateTlbInstructionEntry(void)
{
}

// ---- 使数据TLB中的一个条目无效 ----
// ---- C8
void MMU_InvalidateTlbDatasEntry(void)
{
}

// ---- 使指令TLB中的所有条目无效 ----
// ---- C8
void MMU_InvalidateTlbInstructionsAll(void)
{
}

// ---- 使数据TLB中的所有条目无效 ----
// ---- C8
void MMU_InvalidateTlbDatasAll(void)
{
}

// ---- 使统一TLB中的所有条目无效 ----
// 使统一TLB失效，即指令与数据的TLB都失效
// ---- C8
void MMU_InvalidateTlbsAll(void)
{
}

// ---- 设置MMU一级页表的基址 ----
void MMU_SetSectionsBase(u32 *base)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c2, c0, 0 @页表基地址 \n\t"
            :
            :"r"(base)
            :"memory"
    );
}

// ---- 设置MMU域访问权限 -----------------------------------------------------
//功能: 设置mmu的域控制，arm可以设置16个域，每个域可以定义访问权限。在页表中，
//      可以指定该段的访问权限受哪个域控制。
//参数: authorization,访问权限控制字，每个域占两位
//返回: 无
//-----------------------------------------------------------------------------
void MMU_SetDomainAccess(u32 authorization)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c3, c0, 0 @写域寄存器 \n\t"
            :
            :"r"(authorization)
            :"memory"
    );
}

// ---- 禁止MMU ----
void MMU_Disable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "bic    r0, r0, #0x1 @禁止mmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

// ---- 使能MMU ----
void MMU_Enable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "orr    r0, r0, #0x1 @允许mmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

