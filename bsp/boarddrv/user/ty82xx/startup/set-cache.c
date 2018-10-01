//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
// -----------------------------------------------------------------------------
// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��sysinit.c
// ģ������: CPUʱ�ӵĳ�ʼ����Ƭ��Ƭ��RAM�ȵĳ�ʼ��
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 08/13.2015
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "arch_feature.h"
#include "mpu.h"

#define memory_sync()        __DSB();__ISB();

// =============================================================================
#define MPU_NOCACHE_SRAM_REGION_NUM    11

//���ڲ�SRAM��Ϊ�����Σ�����ӦMPU������
//ÿһ��region ����ʼ��ַ���밴RamSize���룬��RamSize ��С����2^(N+1) �� 3 < N < 32
extern uint32_t gc_ptCacheRam1Size;
extern uint32_t gc_ptCacheRam2Size;
extern uint32_t gc_ptNoCacheRamSize;
// =============================================================================
// ���ܣ�����MPU����Ҫ��������cache�����򣬾���������Ϣ���£�
/* Default memory map
   Address range        Memory region      Memory type   Shareability  Cache policy
   0x00000000- 0x1FFFFFFF Code             Normal        Non-shareable  WT
   0x20000000- 0x3FFFFFFF SRAM             Normal        Non-shareable  WBWA
   0x40000000- 0x5FFFFFFF Peripheral       Device        Non-shareable  -
   0x60000000- 0x7FFFFFFF RAM              Normal        Non-shareable  WBWA
   0x80000000- 0x9FFFFFFF RAM              Normal        Non-shareable  WT
   0xA0000000- 0xBFFFFFFF Device           Device        Shareable
   0xC0000000- 0xDFFFFFFF Device           Device        Non Shareable
   0xE0000000- 0xFFFFFFFF System           -                  -
   */
// ��������
// ���أ���
// =============================================================================
void Cache_config( void )
{
    uint32_t dwRegionBaseAddr;
    uint32_t dwRegionAttr;

    memory_barrier();
/***************************************************
    ITCM memory region --- Normal
    START_Addr:-  0x00000000UL
    END_Addr:-    0x00400000UL
****************************************************/
    dwRegionBaseAddr =
        ITCM_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_DEFAULT_ITCM_REGION;        // 1

    dwRegionAttr =
        MPU_AP_PRIVILEGED_READ_WRITE |
        MPU_REGION_EXECUTE_NEVER |
        MPU_CalMPURegionSize(ITCM_END_ADDRESS - ITCM_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
    Internal flash memory region --- Normal read-only
    (update to Strongly ordered in write accesses)
    START_Addr:-  0x00400000UL
    END_Addr:-    0x00600000UL
******************************************************/

    dwRegionBaseAddr =
        IFLASH_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_DEFAULT_IFLASH_REGION;      //2

    dwRegionAttr =
        MPU_AP_READONLY |
        INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
        MPU_CalMPURegionSize(IFLASH_END_ADDRESS - IFLASH_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
    DTCM memory region --- Normal
    START_Addr:-  0x20000000L
    END_Addr:-    0x20400000UL
******************************************************/

    /* DTCM memory region */
    dwRegionBaseAddr =
        DTCM_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_DEFAULT_DTCM_REGION;         //3

    dwRegionAttr =
        MPU_AP_PRIVILEGED_READ_WRITE |
        MPU_REGION_EXECUTE_NEVER |
        MPU_CalMPURegionSize(DTCM_END_ADDRESS - DTCM_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);

/****************************************************
    SRAM Cacheable memory region --- Normal
    START_Addr:-  0x20400000UL
    END_Addr:-    0x2043FFFFUL
******************************************************/
    /* SRAM memory  region */

    if(gc_ptCacheRam1Size)
    {
        dwRegionBaseAddr =
            SRAM_START_ADDRESS |
            MPU_REGION_VALID |
            MPU_DEFAULT_SRAM_REGION_1;         //4

        dwRegionAttr =
            MPU_AP_FULL_ACCESS    |
            MPU_REGION_EXECUTE_NEVER |
            INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
            MPU_CalMPURegionSize(gc_ptCacheRam1Size - 1) |
            MPU_REGION_ENABLE;

        MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
    }

/****************************************************
    Internal SRAM second partition memory region --- Normal
    START_Addr:-  0x20440000UL
    END_Addr:-    0x2045FFFFUL
******************************************************/
    /* SRAM memory region */
    if(gc_ptCacheRam2Size)
    {
        dwRegionBaseAddr =
            (SRAM_START_ADDRESS + gc_ptCacheRam1Size) |
            MPU_REGION_VALID |
            MPU_DEFAULT_SRAM_REGION_2;         //5

        dwRegionAttr =
            MPU_AP_FULL_ACCESS    |
            MPU_REGION_EXECUTE_NEVER |
            INNER_NORMAL_WB_NWA_TYPE( NON_SHAREABLE ) |
            MPU_CalMPURegionSize(gc_ptCacheRam2Size - 1) |
            MPU_REGION_ENABLE;

        MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
    }

    if(gc_ptNoCacheRamSize)
    {
        dwRegionBaseAddr =
            (SRAM_START_ADDRESS + gc_ptCacheRam1Size + gc_ptCacheRam2Size) |
            MPU_REGION_VALID |
            MPU_NOCACHE_SRAM_REGION_NUM;          //11

        dwRegionAttr =
            MPU_AP_FULL_ACCESS    |
            MPU_REGION_EXECUTE_NEVER |
            INNER_OUTER_NORMAL_NOCACHE_TYPE( SHAREABLE ) |
            MPU_CalMPURegionSize(gc_ptNoCacheRamSize - 1) |
            MPU_REGION_ENABLE;

        MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
    }

/****************************************************
    Peripheral memory region --- DEVICE Shareable
    START_Addr:-  0x40000000UL
    END_Addr:-    0x5FFFFFFFUL
******************************************************/
    dwRegionBaseAddr =
        PERIPHERALS_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_PERIPHERALS_REGION;          //6

    dwRegionAttr = MPU_AP_FULL_ACCESS |
        MPU_REGION_EXECUTE_NEVER |
        SHAREABLE_DEVICE_TYPE |
        MPU_CalMPURegionSize(PERIPHERALS_END_ADDRESS - PERIPHERALS_START_ADDRESS)
        |MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);


/****************************************************
    External EBI memory  memory region --- Strongly Ordered
    START_Addr:-  0x60000000UL
    END_Addr:-    0x6FFFFFFFUL
******************************************************/
    dwRegionBaseAddr =
        EXT_EBI_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_EXT_EBI_REGION;

    dwRegionAttr =
        MPU_AP_FULL_ACCESS |
        MPU_REGION_EXECUTE_NEVER |
        /* External memory Must be defined with 'Device' or 'Strongly Ordered'
        attribute for write accesses (AXI) */
        STRONGLY_ORDERED_SHAREABLE_TYPE |
        MPU_CalMPURegionSize(EXT_EBI_END_ADDRESS - EXT_EBI_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
/****************************************************
    SDRAM Cacheable memory region --- Normal
    START_Addr:-  0x70000000UL
    END_Addr:-    0x7FFFFFFFUL
******************************************************/
    dwRegionBaseAddr =
        SDRAM_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_DEFAULT_SDRAM_REGION;        //7

    dwRegionAttr =
        MPU_AP_FULL_ACCESS    |
        MPU_REGION_EXECUTE_NEVER |
        //INNER_NORMAL_WB_NWA_TYPE( SHAREABLE ) |
        INNER_NORMAL_NOCACHE_TYPE(NON_SHAREABLE)|
        MPU_CalMPURegionSize(SDRAM_END_ADDRESS - SDRAM_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
/****************************************************
    QSPI memory region --- Strongly ordered
    START_Addr:-  0x80000000UL
    END_Addr:-    0x9FFFFFFFUL
******************************************************/
    dwRegionBaseAddr =
        QSPI_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_QSPIMEM_REGION;              //8

    dwRegionAttr =
        MPU_AP_FULL_ACCESS |
        MPU_REGION_EXECUTE_NEVER |
        STRONGLY_ORDERED_SHAREABLE_TYPE |
        MPU_CalMPURegionSize(QSPI_END_ADDRESS - QSPI_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);


/****************************************************
    USB RAM Memory region --- Device
    START_Addr:-  0xA0100000UL
    END_Addr:-    0xA01FFFFFUL
******************************************************/
    dwRegionBaseAddr =
        USBHSRAM_START_ADDRESS |
        MPU_REGION_VALID |
        MPU_USBHSRAM_REGION;              //9

    dwRegionAttr =
        MPU_AP_FULL_ACCESS |
        MPU_REGION_EXECUTE_NEVER |
        SHAREABLE_DEVICE_TYPE |
        MPU_CalMPURegionSize(USBHSRAM_END_ADDRESS - USBHSRAM_START_ADDRESS) |
        MPU_REGION_ENABLE;

    MPU_SetRegion( dwRegionBaseAddr, dwRegionAttr);
    /* Enable the memory management fault , Bus Fault, Usage Fault exception */
    SCB->SHCSR |= (SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk
                    | SCB_SHCSR_USGFAULTENA_Msk);

    /* Enable the MPU region */
    MPU_Enable( MPU_ENABLE | MPU_PRIVDEFENA);

    memory_sync();
}


