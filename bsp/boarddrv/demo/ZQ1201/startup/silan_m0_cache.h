#ifndef _SILAN_CACHE_H_
#define _SILAN_CACHE_H_

#include "silan_types.h"

#define M0_CACHE_LINE_SIZE    16

#define CACHE_ENABLE 		  1
#define CACHE_DISABLE 		  0

#define CACHE_LOCK			  1
#define CACHE_ULOCK		      0

#define CACHE_LOCK_ADDR       0x1FDC00
#define CACHE_LOCK_LEN        0xFF
/*
m0_cache_init()
operation :1 cahce en  ;0 cache dis
if_lock :1 cahce lock  ;0 cache unlock
...
*/
extern void m0_cache_init(u8 operation,u8 if_lock,u32 lock_addr,u32 len);
/*
ex_spi_crc_enable()
addr :crc save addr in flash
...
*/
extern void ex_spi_crc_enable(u32 addr);

//void cache_spi_cmd_sel()__attribute__((section("cachelock")));

#endif

