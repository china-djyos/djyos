#include <cpu_peri.h>

#define UID_BASE_1       0x1FFFF7E8
#define UID_BASE_2       0x1FFFF7EC
#define UID_BASE_3       0x1FFFF7F0

void GetCpuID(u32 *ID0,u32 *ID1,u32 *ID2)
{
    /* read the hardware. */
    *ID0 = *(u32*)UID_BASE_1;
    *ID1 = *(u32*)UID_BASE_2;
    *ID2 = *(u32*)UID_BASE_3;
}
