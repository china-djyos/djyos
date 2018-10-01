#ifndef _CACHE_REGS_H_
#define _CACHE_REGS_H__

#include "silan_types.h"

typedef volatile struct spicache_struct
{
	u32  main_ctrl;           // offset 0x000
	u32  cache_ctrl;          // offset 0x004
	u32  opaddr;              // offset 0x008
	u32  opsize;              // offset 0x00C
	u32  reserve1[8];        // offset 0x010
	u32  intr;                // offset 0x030
	u32  reserve2;          // offset 0x034
	u32  softreset;           // offset 0x038
	u32  reserve0[85];        // offset 0x040 ~ 0x0fc reserved
	u32  cache_hit_num_l;             // offset 0x190
	u32  cache_hit_num_h;             // offset 0x194
	u32  cache_miss_num_l;         // offset 0x198
	u32  cache_miss_num_h;         // offset 0x19c
}spicache_struct_t;

#define spicache_st  ((spicache_struct_t *)0xa0000000)

//main_ctrl
#define CACHE_AREA_1    ((u32)1<<24)
#define CACHE_AREA_2    ((u32)1<<25)
#define CACHE_AREA_3    ((u32)1<<26)
#define CACHE_AREA_4    ((u32)1<<27)

#define CACHE_RAM_EN    ((u32)1<<8)
#define CACHE_ENABLE    1

// cache_ctrl
#define CACHE_UNVALID             			(1 << 20)

#define CACHE_UNLOCK_END             		(1 << 18)
#define CACHE_UNLOCKING              		(1 << 17)
#define CACHE_UNLOCK                 		(1 << 16)
#define CACHE_AUTOCLEAR_END             (1 << 10)
#define CACHE_AUTOCLEARING              (1 << 9)
#define CACHE_AUTOCLEAR                 (1 << 8)

//#define CACHE_RUNLOCK             			(1 << 5)

#define CACHE_AUTOFILL_LOCK             (1 << 4)
#define CACHE_AUTOFILL_END              (1 << 2)
#define CACHE_AUTOFILLING               (1 << 1)
#define CACHE_AUTOFILL                  (1 << 0)

#define CACHE_STATISTICS_ENABLE         ((u32)1 << 30)

// cache intr
#define CACHE_STATISTICS_ERR_INTR_EN    (1 << 21)
#define CACHE_STATE_ERR_INTR_EN         (1 << 17)
#define CACHE_STATISTICS_ERR_INTR       (1 << 5)
#define CACHE_STATE_ERR_INTR            (1 << 1)

#define CACHE_SOFTRESET_KEY      0xa05f9696



#endif

