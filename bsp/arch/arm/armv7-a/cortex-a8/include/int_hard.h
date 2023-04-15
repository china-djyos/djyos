#ifndef __INT_HARD_H__
#define __INT_HARD_H__

#include "arch_feature.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef ucpu_t atom_high_t;
typedef ucpu_t atom_low_t;

#define cn_real_prio_default    0x80    //默认的实时中断优先级，用户把某中断设
                                        //为实时中断时，这是它的默认优先级，当
                                        //然，用户可调用int_set_prio函数修改。

struct IntReg     //at 0xe000e100,
{
    vu32 setena[32];
    vu32 clrena[32];
    vu32 setpend[32];
    vu32 clrpend[32];
    vu32 active[32];    //CM3的的中断活动状态寄存器，在处理器执行了其 ISR 的第一
                        //条指令后，相应位被置 1，直到 ISR 返回时才硬件清零。
                        //共240位，stm32只使用了开始的43位
    u32 RESERVED5[32];
    vu8 pri[240];
};

extern struct IntReg volatile * const pg_int_reg;
extern u32 u32g_vect_table[];

//bool_t Int_IsLowAtom(atom_low_t AtomStatus);
//bool_t Int_IsHighAtom(atom_low_t AtomStatus);
void __Int_ClearAllLine(void);
void __Int_InitHard(void);
void __Int_EngineReal(ufast_t ufl_line);
void __Int_EngineAsynSignal(ufast_t ufl_line);
void __Int_EngineAll(ufast_t ufl_line);
void portENABLE_INT(void);
s32 portDISABLE_INT(void);
void portENABLE_IRQ(void);
s32 portDISABLE_IRQ(void);
void portENABLE_FIQ(void);
s32 portDISABLE_FIQ(void);
#ifdef __cplusplus
}
#endif

#endif //__INT_HARD_H__
