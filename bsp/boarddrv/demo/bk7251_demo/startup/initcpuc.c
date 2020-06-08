#include <stdint.h>
#include <stddef.h>
#include "arch_feature.h"

extern void Iboot_LoadPreload(void);

//void Init_Cpu(void)
//{
//    /* Here we may setting exception vector, MGU, cache, and so on. */
////  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
////  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
////  csi_dcache_enable();
//    void Iboot_IAP_SelectLoadProgam(void);
//    Iboot_IAP_SelectLoadProgam();
//}
//
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

