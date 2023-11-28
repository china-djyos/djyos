#ifndef __IO_H__
#define __IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
//typedef signed long             off_t;
typedef signed long long        loff_t;

//typedef signed int              bool_t;

typedef signed long             register_t;
typedef unsigned long           irq_flags_t;

typedef unsigned long long      virtual_addr_t;
typedef unsigned long long      virtual_size_t;
typedef unsigned long long      physical_addr_t;
typedef unsigned long long      physical_size_t;

static   u8 read8(virtual_addr_t addr)
{
    return ( *((volatile u8 *)(addr)) );
}

static   u16 read16(virtual_addr_t addr)
{
    return ( *((volatile u16 *)(addr)) );
}

static   u32 read32(virtual_addr_t addr)
{
    return( *((volatile u32 *)(addr)) );
}

static   u64 read64(virtual_addr_t addr)
{
    return( *((volatile u64 *)(addr)) );
}



static   void write8(virtual_addr_t addr, u8 value)
{
    *((volatile u8 *)(addr)) = value;
}

static   void write16(virtual_addr_t addr, u16 value)
{
    *((volatile u16 *)(addr)) = value;
}

static   void write32(virtual_addr_t addr, u32 value)
{
    *((volatile u32 *)(addr)) = value;
}

static   void write64(virtual_addr_t addr, u64 value)
{
    *((volatile u64 *)(addr)) = value;
}


//ÉèÖÃ¼Ä´æÆ÷Öµ
#define C_BIT(addr,bit)                 write32(addr,read32(addr) & (~((u64)(1)<<bit)) )
#define S_BIT(addr,bit)                 write32(addr,read32(addr) | ((u64)(1)<<bit)    )
#define C_Vlue(addr,bit,vlue)   write32(addr,read32(addr) & (~((u64)(vlue)<<bit)) )
#define S_Vlue(addr,bit,vlue)   write32(addr,read32(addr) | ((u64)(vlue)<<bit)    )

virtual_addr_t phys_to_virt(physical_addr_t phys);
physical_addr_t virt_to_phys(virtual_addr_t virt);

#ifdef __cplusplus
}
#endif

#endif /* __IO_H__ */
