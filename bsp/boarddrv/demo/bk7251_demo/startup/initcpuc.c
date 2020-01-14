#include <stdint.h>
#include <stddef.h>
#include "arch_feature.h"

extern void Load_Preload(void);

void Init_Cpu(void)
{
    /* Here we may setting exception vector, MGU, cache, and so on. */
//  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
//  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
//  csi_dcache_enable();
    void IAP_SelectLoadProgam(void);
    IAP_SelectLoadProgam();
}

void IAP_GpioPinInit(void)
{

}

bool_t IAP_IsForceIboot(void)
{
    return false;
}
void AppStart() __attribute__((optimize("O0")));
u32 abccc = 0x87654321;
void AppStart()
{
    void __asm_reset_msp();
    __asm_reset_msp();
    if((Int_GetStatus() & 0xc0)  != 0xc0)
    {
        abccc = 0x12345678;
        while(1);
    }
    Load_Preload();
}

