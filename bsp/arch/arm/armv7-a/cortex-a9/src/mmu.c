#include "stdint.h"

//---- MMU��ӳ�� ----------------------------------------------------------------
//����: ������ε�ַӳ�䵽�����ַ.
//������tt_base: ת�����ַ.
//      va_start: �����ַ(������1MB��ַ����).
//      pa_start: �����ַ(������1MB��ַ����).
//      sec_count: ��Ҫӳ��Ķ�����.
//      attr: ������.
// C B   дͨ��cache         д����cache         ��ѡ��cache
// 0 0   ��cache����д����   ��cache����д����   ��cache����д����
// 0 1   ��cache����д����   ��cache����д����   ��cache����д����
// 1 0   ��cache����д����   ����Ԥ��            дͨ��cache����д����
// 1 1   ��cache����д����   ��cache����д����   д�ؿ�cache����д����
//���أ���.
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

// ---- mmu��ӳ��--------------------------------------------------------------
//����: �������ַvaddr��ʼ��һ����(1M)ӳ�䵽�����ַpaddr�У�ͬʱ���øöε�
//      cache��д�������ԡ�ӳ��󣬷���vaddr����mmu�ض���paddr�С�
//ҳ����ĿC(bit3),B(bit2)λ���嶨��:
// C B   дͨ��cache         д����cache         ��ѡ��cache
// 0 0   ��cache����д����   ��cache����д����   ��cache����д����
// 0 1   ��cache����д����   ��cache����д����   ��cache����д����
// 1 0   ��cache����д����   ����Ԥ��            дͨ��cache����д����
// 1 1   ��cache����д����   ��cache����д����   д�ؿ�cache����д����
//���أ���
//-----------------------------------------------------------------------------
/*
void xxMMU_MapSection(u32 vaddr, u32 paddr, int properties)
{
    mmu_page_table[vaddr>>20] = (paddr & 0xfff00000)|(properties&0xFFF);

}*/

// ---- ��ĳ��entry������TLB�� ----
// ��ĳЩ��ַ�任������TLB�У�����������Щ�����ַ�������ַ�ķ��ʾͻ��ø���Ѹ��
// ��Ҫ�Ӳ���
// ---- C10
void MMU_LockEntry()
{

}

// ---- ʹָ��TLB�е�һ����Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbInstructionEntry(void)
{
}

// ---- ʹ����TLB�е�һ����Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbDatasEntry(void)
{
}

// ---- ʹָ��TLB�е�������Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbInstructionsAll(void)
{
}

// ---- ʹ����TLB�е�������Ŀ��Ч ----
// ---- C8
void MMU_InvalidateTlbDatasAll(void)
{
}

// ---- ʹͳһTLB�е�������Ŀ��Ч ----
// ʹͳһTLBʧЧ����ָ�������ݵ�TLB��ʧЧ
// ---- C8
void MMU_InvalidateTlbsAll(void)
{
}

// ---- ����MMUһ��ҳ��Ļ�ַ ----
void MMU_SetSectionsBase(u32 *base)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c2, c0, 0 @ҳ�����ַ \n\t"
            :
            :"r"(base)
            :"memory"
    );
}

// ---- ����MMU�����Ȩ�� -----------------------------------------------------
//����: ����mmu������ƣ�arm��������16����ÿ������Զ������Ȩ�ޡ���ҳ���У�
//      ����ָ���öεķ���Ȩ�����ĸ�����ơ�
//����: authorization,����Ȩ�޿����֣�ÿ����ռ��λ
//����: ��
//-----------------------------------------------------------------------------
void MMU_SetDomainAccess(u32 authorization)
{
    __asm__ volatile (
            "mcr    p15, 0, r0, c3, c0, 0 @д��Ĵ��� \n\t"
            :
            :"r"(authorization)
            :"memory"
    );
}

// ---- ��ֹMMU ----
void MMU_Disable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "bic    r0, r0, #0x1 @��ֹmmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

// ---- ʹ��MMU ----
void MMU_Enable(void)
{
    __asm__ volatile (
            "mrc    p15, 0, r0, c1, c0, 0 \n\t"
            "orr    r0, r0, #0x1 @����mmu \n\t"
            "mcr    p15, 0, r0, c1, c0, 0 \n\t"
            :
            :
            :"memory"
    );
}

