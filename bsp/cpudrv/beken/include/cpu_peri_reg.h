 
#ifndef _CPU_PERI_REG_
#define _CPU_PERI_REG_









#define REG8_SET_BIT(addr, bit)      (*((volatile u8 *)addr) |= (bit))
#define REG8_CLEAR_BIT(addr, bit)    (*((volatile u8 *)addr) &= ~(bit))
#define REG8_READ_BIT(addr, bit)     (*((volatile u8 *)addr) & (bit))
#define REG8_CLEAR_REG(addr)         (*((volatile u8 *)addr) = (0x0))
#define REG8_WRITE_REG(addr, val)    (*((volatile u8 *)addr) = (val))
#define REG8_READ_REG(addr)          (*((volatile u8 *)addr))

#define REG16_SET_BIT(addr, bit)      (*((volatile u16 *)addr) |= (bit))
#define REG16_CLEAR_BIT(addr, bit)    (*((volatile u16 *)addr) &= ~(bit))
#define REG16_READ_BIT(addr, bit)     (*((volatile u16 *)addr) & (bit))
#define REG16_CLEAR_REG(addr)         (*((volatile u16 *)addr) = (0x0))
#define REG16_WRITE_REG(addr, val)    (*((volatile u16 *)addr) = (val))
#define REG16_READ_REG(addr)          (*((volatile u16 *)addr))

#define REG32_SET_BIT(addr, bit)      (*((volatile u32 *)addr) |= (bit))
#define REG32_CLEAR_BIT(addr, bit)    (*((volatile u32 *)addr) &= ~(bit))
#define REG32_READ_BIT(addr, bit)     (*((volatile u32 *)addr) & (bit))
#define REG32_CLEAR_REG(addr)         (*((volatile u32 *)addr) = (0x0))
#define REG32_WRITE_REG(addr, val)    (*((volatile u32 *)addr) = (val))
#define REG32_READ_REG(addr)          (*((volatile u32 *)addr))

#endif 



