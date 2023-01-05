#include <cpu_peri.h>

#ifndef UID_BASE
#warning ST公司的固件库中，唯STM32H7 没有定义 UID_BASE，请在stm32h7xx.h中加上如下语句
//#define UID_BASE              0x1FF1E800U                   /*!< Unique device ID register base address */
#endif
void GetCpuID(u32 *ID0,u32 *ID1,u32 *ID2)
{
    *ID0 = (void *)0;   // ##WIP##    *ID0 = *(u32*)UID_BASE;
    *ID1 = (void *)0;   // ##WIP##    *ID1 = *(u32*)(UID_BASE + sizeof(u32));
    *ID2 = (void *)0;   // ##WIP##    *ID2 = *(u32*)(UID_BASE + sizeof(u32));
}

