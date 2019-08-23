 
#ifndef _CPU_PERI_REG_
#define _CPU_PERI_REG_

//==============¼Ä´æÆ÷ TIMER ======================================
#define PWM_NEW_BASE                    (0x00802A00)
#define REG_TIMERCTLA_PERIOD_ADDR(n)    (PWM_NEW_BASE +  0x04 * (n))
#define TIMER0_CNT                     (PWM_NEW_BASE + 0 * 4)
#define TIMER1_CNT                     (PWM_NEW_BASE + 1 * 4)
#define TIMER2_CNT                     (PWM_NEW_BASE + 2 * 4)
#define TIMER0_2_CTL                   (PWM_NEW_BASE + 3 * 4)

#define TIMERCTL0_EN_BIT               (0x01UL << 0)
#define TIMERCTL1_EN_BIT               (0x01UL << 1)
#define TIMERCTL2_EN_BIT               (0x01UL << 2)
#define TIMERCTLA_CLKDIV_POSI          (3)
#define TIMERCTLA_CLKDIV_MASK          (0x07)
#define TIMERCTL0_INT_BIT              (0x01UL << 7)
#define TIMERCTL1_INT_BIT              (0x01UL << 8)
#define TIMERCTL2_INT_BIT              (0x01UL << 9)
#define TIMERCTLA_INT_POSI             (7)


#define REG_TIMERCTLB_PERIOD_ADDR(n)   (PWM_NEW_BASE + 0x10 * 4 + 0x04 * (n - 3))
#define TIMER3_CNT                     (PWM_NEW_BASE + 0x10 * 4)
#define TIMER4_CNT                     (PWM_NEW_BASE + 0x11 * 4)
#define TIMER5_CNT                     (PWM_NEW_BASE + 0x12 * 4)

#define TIMER3_5_CTL                   (PWM_NEW_BASE + 0x13 * 4)
#define TIMERCTL3_EN_BIT               (0x01UL << 0)
#define TIMERCTL4_EN_BIT               (0x01UL << 1)
#define TIMERCTL5_EN_BIT               (0x01UL << 2)
#define TIMERCTLB_CLKDIV_POSI          (3)
#define TIMERCTLB_CLKDIV_MASK          (0x07)
#define TIMERCTL3_INT_BIT              (0x01UL << 7)
#define TIMERCTL4_INT_BIT              (0x01UL << 8)
#define TIMERCTL5_INT_BIT              (0x01UL << 9)
#define TIMERCTLB_INT_POSI             (7)



//=============================ICU REG DEF=====================================
#define ICU_BASE                                     (0x00802000)
#define ICU_INTERRUPT_ENABLE                         (ICU_BASE + 16 * 4)
#define ICU_PERI_CLK_PWD                             (ICU_BASE + 2 * 4)
#define ICU_ARM_WAKEUP_EN                            (ICU_BASE + 20 * 4)





//==========================¼Ä´æÆ÷²Ù×÷·½·¨======================================
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



