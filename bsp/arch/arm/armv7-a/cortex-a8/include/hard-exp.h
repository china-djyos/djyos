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


//�쳣�������ƺͱ�־
#define CN_HARDEXP_DECODERNAME          "HARDEXP_DECODER"
#define CN_SYS_EXP_CPUINFO_VALIDFLAG    ((u32)(0x87654321))

//���쳣��Ϣ������CPU ��CORE ��Ϣ������CORE���У��;���CPU�ͺ��޹�
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
    u32 ger_r11;    //r4-r11��������ջҲ����

    u32 ger_r0;
    u32 ger_r1;
    u32 ger_r2;
    u32 ger_r3;
    u32 ger_r12;
    u32 spr_lr;
    u32 spr_pc;
    u32 spr_xpsr;   //r0-r3,r12-r15���Զ���ѹջ
};

struct SysExceptionInfo
{
    u32 SysExpType;                     //�쳣��Ϣ����
    u32 SysExpCpuFlag;                 //���쳣��Ϣ����Ч����
    struct SysExpCoreInfo ExpRegInfo;
    //struct SysExpNvicInfo NvicInfo;
};

extern void * g_u32ExpTable[8];
void HardExp_Init(void);
void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks));

#ifdef __cplusplus
}
#endif

#endif // _EXCEPTION_H_

