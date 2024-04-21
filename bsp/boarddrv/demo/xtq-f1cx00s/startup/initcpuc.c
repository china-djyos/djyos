#include <stdint.h>
#include <stddef.h>
#include "mmu.h"
#include "arch_feature.h"

extern void Iboot_LoadPreload(void);

// ---- 初始化MMU ----
u32 mmu_page_table[4096] __attribute__((aligned(0x4000),section(".data.mmutab")));
void MMU_Init(void)
{
    Cache_CleanInvalidData();
    Cache_InvalidInst();
    Cache_DisableAll();
    MMU_Disable();

    MMU_MapSection(mmu_page_table,0x00000000,0x00000000,1,  //on chip memory
          SECTION_ATTR(AP_USER_RW,DOMAIN_NO_CHECK,CB));

    MMU_MapSection(mmu_page_table,0x00100000,0x00100000,27,  //forbit
          SECTION_ATTR(AP_USER_RW, DOMAIN_NO_ACCESS, NCNB));

    MMU_MapSection(mmu_page_table,0x01c00000,0x01c00000,3,  //外设
          SECTION_ATTR(AP_USER_RW,DOMAIN_NO_CHECK,NCNB));

    MMU_MapSection(mmu_page_table,0x01f00000,0x01f00000,2017,  //forbit
          SECTION_ATTR(AP_USER_RW, DOMAIN_NO_ACCESS, NCNB));

    MMU_MapSection(mmu_page_table,0x80000000,0x80000000,64,   //ddr mem
          SECTION_ATTR(AP_USER_RW,DOMAIN_NO_CHECK,CB));

    MMU_MapSection(mmu_page_table,0x84000000,0x84000000,1984, //forbit
          SECTION_ATTR(AP_USER_RW,DOMAIN_NO_ACCESS,NCNB));

    MMU_InvalidateTlbsAll();
    MMU_SetSectionsBase((u32*)mmu_page_table);
    MMU_SetDomainAccess(0xffffffff);

    MMU_Enable();
    Cache_EnableAll();
//    Cache_EnableWriteBuf();

}
ptu32_t Iboot_GetResetAddr(void)
{
    return 0x80000000;
}

void IAP_GpioPinInit(void)
{

}

bool_t Iboot_IAP_IsForceIboot(void)
{
    return false;
}
//void AppStart() __attribute__((optimize("O0")));
//void AppStart()
//{
//    void ExpStackInit();
//    ExpStackInit();
//    Iboot_LoadPreload();
//}

