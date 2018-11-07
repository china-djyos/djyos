
#include "csi_core.h"
#include "arch_feature.h"

extern void Load_Preload(void);

void Init_Cpuc(void)
{
    /* Here we may setting exception vector, MGU, cache, and so on. */
//  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
//  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
//  csi_dcache_enable();
    
    Load_Preload();
}

void IAP_GpioPinInit(void)
{

}

bool_t IAP_IsForceIboot(void)
{
    return false;
}



void epc_test(uint32_t r_epc)
{
    if(r_epc>=0x021e0100)
    {
        return;
    }
    else if(r_epc<=0x02160000)
    {
        return;
    }
}



