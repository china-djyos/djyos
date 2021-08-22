
#ifndef __AP1508_DATATYPE_H__
#define __AP1508_DATATYPE_H__

#include "arch_stdint.h"
#ifndef NULL
#define NULL  0x00
#endif

#ifndef true
#define true  0x01
#endif
#ifndef false
#define false 0x00
#endif

#ifndef SUCCESS
#define SUCCESS  0
#endif
#ifndef FAILURE
#define FAILURE  -1
#endif
#define TIMEOUT  0x1000

#define STATUS_ERR  1
#define STATUS_OK   0

//#define u8  unsigned char
//#define u16  unsigned short
//#define u32  unsigned int

#define  IN
#define  OUT
#define INOUT

//typedef unsigned char       uint8_t;
//typedef unsigned short      uint16_t;
//typedef unsigned int        uint32_t;
#if 0
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
typedef signed long long    int64_t;
//typedef unsigned long long  uint64_t;

//typedef uint64_t                u64;
//typedef sint64_t                s64;
//typedef uint32_t                u32;
//typedef sint32_t                s32;
//typedef uint16_t                u16;
//typedef sint16_t                s16;
//typedef uint8_t                 u8;
//typedef sint8_t                 s8;
//typedef uint64_t const          uc64;
//typedef sint64_t const          sc64;
//typedef uint32_t const          uc32;
//typedef sint32_t const          sc32;
//typedef uint16_t const          uc16;
//typedef sint16_t const          sc16;
//typedef uint8_t  const          uc8;
//typedef sint8_t  const          sc8;
//typedef unsigned char           ucpu_char;
//typedef char                    scpu_char;
//
//typedef volatile ucpu_t         vucpu_t;
//typedef volatile scpu_t         vscpu_t;
//typedef volatile uint64_t       vu64;
//typedef volatile sint64_t       vs64;
//typedef volatile uint32_t       vu32;
//typedef volatile sint32_t       vs32;
//typedef volatile uint16_t       vu16;
//typedef volatile sint16_t       vs16;
//typedef volatile uint8_t        vu8;
//typedef volatile sint8_t        vs8;
//typedef volatile ucpu_t   const vuccpu_t;
//typedef volatile scpu_t   const vsccpu_t;
//typedef volatile uint64_t const vuc64;
//typedef volatile sint64_t const vsc64;
//typedef volatile uint32_t const vuc32;
//typedef volatile sint32_t const vsc32;
//typedef volatile uint16_t const vuc16;
//typedef volatile sint16_t const vsc16;
//typedef volatile uint8_t  const vuc8;
//typedef volatile sint8_t  const vsc8;
//
//#define CN_LIMIT_UINT64         0xffffffffffffffff
//#define CN_LIMIT_UINT32         0xffffffff
//#define CN_LIMIT_UINT24           0xffffff
//#define CN_LIMIT_UINT16         0xffff
//#define CN_LIMIT_UINT8          0xff
//#define CN_LIMIT_SINT64         0x7fffffffffffffff
//#define CN_LIMIT_SINT32         0x7fffffff
//#define CN_LIMIT_SINT16         0x7fff
//#define CN_LIMIT_SINT8          0x7f
//
//#define CN_MIN_UINT64           0
//#define CN_MIN_UINT32           0
//#define CN_MIN_UINT16           0
//#define CN_MIN_UINT8            0
//#define CN_MIN_SINT64           0x8000000000000000
//#define CN_MIN_SINT32           0x80000000
//#define CN_MIN_SINT16           0x8000
//#define CN_MIN_SINT8            0x80
#endif

#define __IO                volatile

#define __REG32(addr)             (*(volatile uint32_t*)(addr))
#define __REG16(addr)             (*(volatile uint16_t*)(addr))
#define __REG8(addr)              (*(volatile uint8_t*)(addr))
#define __sREG32(addr, offset)    (*(volatile uint32_t*)((addr) + offset))
#define __sREG16(addr, offset)    (*(volatile uint16_t*)((addr) + offset))
#define __sREG8(addr,  offset)    (*(volatile uint8_t*)((addr) + offset))

#define bit0 0x01
#define bit1 0x02
#define bit2 0x04
#define bit3 0x08
#define bit4 0x10
#define bit5 0x20
#define bit6 0x40
#define bit7 0x80

//typedef void (*hdl_t)(uint32_t );
typedef void (*tsk_t)(void);

#endif  // __DATATYPE_H__

