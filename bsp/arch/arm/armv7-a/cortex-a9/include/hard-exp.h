#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board-config.h"

#include "blackbox.h"

#define CN_EXP_TYPE_UNDEF_CODE      (CN_BLACKBOX_TYPE_HARD_START+0)
#define CN_EXP_TYPE_PREFETCH        (CN_BLACKBOX_TYPE_HARD_START+1)
#define CN_EXP_TYPE_DATA_ABORT      (CN_BLACKBOX_TYPE_HARD_START+2)

extern u8 usr_top[];
extern u8 usr_bottom[];
extern u8 svc_top[];
extern u8 svc_bottom[];
extern u8 irq_top[];
extern u8 irq_bottom[];
extern u8 fiq_top[];
extern u8 fiq_bottom[];
extern u8 undef_top[];
extern u8 undef_bottom[];
extern u8 abort_top[];
extern u8 abort_bottom[];

extern void (* engine_irq)(ufast_t ufl_line);


//异常解码名称和标志
#define CN_HARDEXP_DECODERNAME          "HARDEXP_DECODER"
#define CN_SYS_EXP_CPUINFO_VALIDFLAG    ((u32)(0x87654321))

//该异常信息是属于CPU 的CORE 信息，属于CORE所有，和具体CPU型号无关
struct SysExpCoreInfo
{
    u32 spr_sp;
    u32 ger_r4;
    u32 ger_r5;
    u32 ger_r6;
    u32 ger_r7;
    u32 ger_r8;
    u32 ger_r9;
    u32 ger_r10;
    u32 ger_r11;    //r4-r11属于现在栈也保存

    u32 ger_r0;
    u32 ger_r1;
    u32 ger_r2;
    u32 ger_r3;
    u32 ger_r12;
    u32 spr_lr;
    u32 spr_pc;
    u32 spr_xpsr;   //r0-r3,r12-r15是自动大压栈
};

struct SysExpRegInfo
{
    u32 fpu_s16;
    u32 fpu_s17;
    u32 fpu_s18;
    u32 fpu_s19;
    u32 fpu_s20;
    u32 fpu_s21;
    u32 fpu_s22;
    u32 fpu_s23;
    u32 fpu_s24;
    u32 fpu_s25;
    u32 fpu_s26;
    u32 fpu_s27;
    u32 fpu_s28;
    u32 fpu_s29;
    u32 fpu_s30;
    u32 fpu_s31;
    struct SysExpCoreInfo CoreInfo;
    u32 fpu_s0;
    u32 fpu_s1;
    u32 fpu_s2;
    u32 fpu_s3;
    u32 fpu_s4;
    u32 fpu_s5;
    u32 fpu_s6;
    u32 fpu_s7;
    u32 fpu_s8;
    u32 fpu_s9;
    u32 fpu_s10;
    u32 fpu_s11;
    u32 fpu_s12;
    u32 fpu_s13;
    u32 fpu_s14;
    u32 fpu_s15;
    u32 fpu_fpscr;
    u32 fpu_res;
};

struct SysExceptionInfo
{
    u32 SysExpType;                     //异常信息类型
    u32 SysExpCpuFlag;                 //该异常信息的有效编码
    struct SysExpRegInfo ExpRegInfo;
    //struct SysExpNvicInfo NvicInfo;
};

extern void *g_u32ExpTable[32];
void HardExp_Init(void);
void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks));

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

