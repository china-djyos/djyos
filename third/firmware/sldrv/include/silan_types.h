/*
 * types.h
 */

#ifndef __SILAN_TYPES_H__
#define __SILAN_TYPES_H__

#include "ap1508_datatype.h"

#define __REG32(addr)             (*(volatile u32*)(addr))
#define __REG16(addr)             (*(volatile u16*)(addr))
#define __REG8(addr)              (*(volatile u8*)(addr))
#define __sREG32(addr, offset)    (*(volatile u32*)((addr) + offset))
#define __sREG16(addr, offset)    (*(volatile u16*)((addr) + offset))
#define __sREG8(addr,  offset)    (*(volatile u8*)((addr) + offset))
	
#define BIT0 0X01
#define BIT1 0X02
#define BIT2 0X04
#define BIT3 0X08
#define BIT4 0X10
#define BIT5 0X20
#define BIT6 0X40
#define BIT7 0X80

#define nBIT0 0xfe
#define nBIT1 0xfd
#define nBIT2 0xfb
#define nBIT3 0xf7
#define nBIT4 0xef
#define nBIT5 0xdf
#define nBIT6 0xbf
#define nBIT7 0x7f

#define bit0 0x01
#define bit1 0x02
#define bit2 0x04
#define bit3 0x08
#define bit4 0x10
#define bit5 0x20
#define bit6 0x40
#define bit7 0x80

#define nbit0	0xfe
#define nbit1	0xfd
#define nbit2	0xfb
#define nbit3	0xf7
#define nbit4	0xef
#define nbit5	0xdf
#define nbit6	0xbf
#define nbit7	0x7f

#include "silan_errno.h"

#endif
