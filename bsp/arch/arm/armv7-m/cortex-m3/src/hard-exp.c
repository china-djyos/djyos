//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:�ж�ģ��
//���ߣ�lst
//�汾��V1.1.0
//�ļ�����: cm3�쳣����ģ��
//����˵��:
//�޶���ʷ:
//2. ����: 2010-01-20
//   ����: lst
//   �°汾��: V1.1.0
//   �޸�˵��: �޸� exp_esr_tick������������tick�ж��е����첽�źŵ�ʹ�ܺͽ�ֹ
//      ����ʱ������en_asyn_signal_counter�Ȳ���δ��ȷ���ã�������⡣
//1. ����: 2009-08-30
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��:
//------------------------------------------------------
#include "stdint.h"
#include "string.h"
#include "hard-exp.h"
#include "exp.h"
#include "stdio.h"
#include "endian.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include "board-config.h"

extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __Djy_ScheduleAsynSignal(void);

//�쳣��������ⲿ����
extern void HardExp_SystickHandler(void);
extern void HardExp_SvcHandler(void);
extern void HardExp_MemfaultHandler(void);
extern void HardExp_UsagefaultHandler(void);
extern void HardExp_BusfaultHandler(void);

extern   uint32_t   msp_top[ ];
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
#if (CN_USE_TICKLESS_MODE) 
extern   u32 __Djy_GetTimeBaseReload(void);
#endif
bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian);

struct SystickReg volatile * const pg_systick_reg
                        = (struct SystickReg *)0xE000E010;
struct ScbReg volatile * const pg_scb_reg
                        = (struct ScbReg *)0xe000ed00;


//ϵͳtick����������
void (*user_systick)(u32 inc_ticks);

u32 g_u32ExpTable[16] __attribute__((section(".table.exceptions")));

// =============================================================================
// ����:�쳣�������ʼ����������������ָ���ĵ�ַ����,��ʹ�ܸ����쳣
// ����:��
// ����:��
// =============================================================================
static void __Exp_TableSet(void)
{
    g_u32ExpTable[0]                            = (uint32_t)msp_top;
    g_u32ExpTable[CN_SYSVECT_RESET]             = (uint32_t)(*(u32*)0x00000004);
    g_u32ExpTable[CN_SYSVECT_NMI]               = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_HARD_FAULT]        = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_MEMMANAGE_FAULT]  = (uint32_t) HardExp_MemfaultHandler;
    g_u32ExpTable[CN_SYSVECT_BUS_FAULT]         = (uint32_t) HardExp_BusfaultHandler;
    g_u32ExpTable[CN_SYSVECT_USAGE_FAULT]   = (uint32_t) HardExp_UsagefaultHandler;
    g_u32ExpTable[7]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[8]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[9]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[10]                       = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SVC]               = (u32)HardExp_SvcHandler;
    g_u32ExpTable[CN_SYSVECT_DEBUG]             = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[13]                       = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_PENDSV]            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SYSTICK]           = (u32)HardExp_SystickHandler;

    pg_scb_reg->VTOR = (u32)g_u32ExpTable;
    pg_scb_reg->systick_pri15 = 0xff;   //������ȼ�,systickӦ�������첽�źŹض�
    pg_scb_reg->svc_pri11 = 0x0;       //svc�����ȼ����첽�ź���ͬ��
    pg_scb_reg->SHCSR |=0x70000;     //���� ���ߡ��÷����洢�� fault

    pg_scb_reg->CCR |= 0x610;       //ǿ��SP�Զ����룬��0
}

// =============================================================================
// �������ܣ�__SwapExpCpuInfoByEndian
//          ת��Ӳ��CPU�쳣��Ϣ�ֽ���
// ���������cpuinfo, CPU�쳣ͨ����Ϣ
// ���������cpuinfo, CPU�쳣ͨ����Ϣ��ת����
// ����ֵ  ����
// ˵��    ���ڲ�����
// =============================================================================
void __SwapExpCpuInfoByEndian(struct SysExceptionInfo *cpuinfo)
{
    u32 *temp;
    u32 i,len;
    temp =(u32 *)cpuinfo;
    len = (sizeof(struct SysExceptionInfo))/4;
    for(i = 0; i<len; i++)
    {
        *temp = swapl(*temp);
        temp++;
    }
}

// =============================================================================
// ����:SYSTICK����������������ָ����tick������ʼ������Exp_SystickTickHandler������
// ����:tick,SYSTICK���û�������
// ����:��
// =============================================================================
void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks))
{
    user_systick = tick;
}

// =============================================================================
// ����:��Cortex Mϵͳ�ں��У��ں�ϵͳ��ʱ��SYSTICI��ʱ�ж������쳣���ú����Ǵ��쳣
//      ������Ļ�����������C�ķ���������ʵ��ϵͳTICK����
// ����:��
// ����:��
// =============================================================================
void Exp_SystickTickHandler(void)
{
#if (CN_USE_TICKLESS_MODE)
    u32 tick=0;
#endif
    g_bScheduleEnable = false;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
#if (CN_USE_TICKLESS_MODE)
    tick=__Djy_GetTimeBaseReload();
    user_systick(tick);
#else
    user_systick(1);
#endif
    tg_int_global.nest_asyn_signal = 0;
//    tg_int_global.en_asyn_signal = true;
    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
}

// =============================================================================
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_MemManageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct ExpThrowPara  head;
    u32 ExpAddr;
    enum EN_ExpAction Action;
    bool_t   shedule_bak,MMA_Flag=false;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));
    ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    if(pg_scb_reg->MFSR & CN_EXP_MFSR_MMARVALID)
    {
        ExpAddr = pg_scb_reg->MMFAR;
        MMA_Flag = true;
    }
    //handler the exp
    if(pg_scb_reg->MFSR & CN_EXP_MFSR_IACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: 0x%08x ��ָ����ʴ���\n\r",ExpAddr);
        }
        else
        {
            printk("MEM Fault: ָ����ʴ�, �޷����������ַ\n\r");
        }
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_DACCVIOL)
    {
        if(MMA_Flag == true)
        {
            printk("MEM Fault: 0x%08x �����ݷ��ʴ���\n\r",ExpAddr);
        }
        else
        {
            printk("MEM Fault: ���ݷ��ʴ�, �޷����������ַ\n\r");
        }
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MUNSTKERR)
    {
        printk("MEM Fault: ��ջ����\n\r");
    }
    else if(pg_scb_reg->MFSR & CN_EXP_MFSR_MSTKERR)
    {
        printk("MEM Fault: ��ջ����\n\r");
    }
    else
    {
        printk("MEM Fault: %s\n\r","δ֪�洢������");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.ExpAction = EN_EXP_DEAL_WAIT;
#else
   head.ExpAction = EN_EXP_DEAL_RESET;
#endif
   head.ExpInfo  = (u8 *)(&ExpRecord);
   head.ExpInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.ExpType = CN_EXP_TYPE_MEMMANAGE_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);

   Action = Exp_Throw(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ����:ϵͳӲ�����󣬿��ܻ�������������fault�ϷµĽ��������ǣ���Ӧ�ý�һ����ѯԭ��
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_HardFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct ExpThrowPara  head;
    enum EN_ExpAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));
    ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->HFSR & CN_EXP_HFSR_VECTBL)
    {
        printk("Hard Fault: ȡ������ʱ��������\n\r");
    }
    else if(pg_scb_reg->UFSR & CN_EXP_HFSR_FORCED)
    {
        printk("Hard Fault: ����fault����\n\r");
    }
    else if(pg_scb_reg->HFSR & CN_EXP_HFSR_DEBUGEVT)
    {
        printk("Hard Fault: ����Բ�����fault\n\r");
    }
    else
    {
        printk("Hard Fault: %s\n\r","δ֪hardfault");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.ExpAction = EN_EXP_DEAL_WAIT;
#else
   head.ExpAction = EN_EXP_DEAL_RESET;
#endif
   head.ExpInfo  = (u8 *)(&ExpRecord);
   head.ExpInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.ExpType = CN_EXP_TYPE_HARD_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = Exp_Throw(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ����:ϵͳ���ڵ���������쳣������������ֻ�������
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_DebugFaultHandler(u32 *core_info)
{
//    struct SysExceptionInfo ExpRecord;
//
//    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
//    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
//    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
//    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
//    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;
//
//    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
//    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;
//    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    printk("Debug Fault: %s\n\r","This is a debug fault");

    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;

    return EN_EXP_DEAL_IGNORE;
}

// =============================================================================
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_BusFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct ExpThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_ExpAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));
    ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);
    CoreRegister = &ExpRecord.ExpRegInfo.CoreInfo;

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->BFSR & CN_EXP_BFSR_IBUSERR)
    {
        printk("Bus Fault: 0x%08x��ȡָ����\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_PRECISERR)
    {
        //�����쳣ʱ�ĵ�ַ
        if(pg_scb_reg->BFSR & CN_EXP_BFSR_BFARVALID)
        {
            printk("Bus Fault: 0x%08x�����ݷ��ʴ���, PC= 0x%08x\n\r",
                                pg_scb_reg->BFAR,CoreRegister->spr_pc);
        }
        else
        {
            printk("Bus Fault: δ֪��ַ���ݷ��ʴ���, PC= 0x%08x\n\r",
                                CoreRegister->spr_pc);
        }
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_IMPRECISERR)
    {
        printk("Bus Fault: δ֪��ַ���ݷ��ʴ���, PC= 0x%08x\n\r",
                            CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_UNSTKERR)
    {
        printk("Bus Fault: ��ջ�쳣,SP = 0x%08x,PC = 0x%08x\n\r",
                            CoreRegister->spr_sp,CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->BFSR & CN_EXP_BFSR_STKERR)
    {
        printk("Bus Fault: ��ջ�쳣,SP = 0x%08x,PC = 0x%08x\n\r",
                            CoreRegister->spr_sp,CoreRegister->spr_pc);
    }
    else
    {
        printk("Bus Fault: %s\n\r","Unknown Fault");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;

#if (DEBUG == 1)
   head.ExpAction = EN_EXP_DEAL_WAIT;
#else
   head.ExpAction = EN_EXP_DEAL_RESET;
#endif
   head.ExpInfo  = (u8 *)(&ExpRecord);
   head.ExpInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.ExpType = CN_EXP_TYPE_BUS_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = Exp_Throw(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ���ܣ��˲�����Ҫ��������������Usage�쳣��Ϣ
// ������core_info,�쳣�����ֳ�
// ���� �� EN_ExpAction
// =============================================================================
enum EN_ExpAction Exp_UsageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct ExpThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_ExpAction Action;
    bool_t   shedule_bak;

    shedule_bak = g_bScheduleEnable ;
    g_bScheduleEnable = false;

    memset(&ExpRecord,0,sizeof(struct SysExceptionInfo));
    ExpRecord.ExpRegInfo.CoreInfo = *((struct SysExpCoreInfo *)core_info);
    CoreRegister = &ExpRecord.ExpRegInfo.CoreInfo;

    ExpRecord.NvicInfo.bfsr = pg_scb_reg->BFSR;
    ExpRecord.NvicInfo.dfsr = pg_scb_reg->DFSR;
    ExpRecord.NvicInfo.hfsr = pg_scb_reg->HFSR;
    ExpRecord.NvicInfo.mfsr = pg_scb_reg->MFSR;
    ExpRecord.NvicInfo.ufsr = pg_scb_reg->UFSR;

    ExpRecord.NvicInfo.mmar = pg_scb_reg->MMFAR;
    ExpRecord.NvicInfo.bfar = pg_scb_reg->BFAR;

    ExpRecord.SysExpCpuFlag = CN_SYS_EXP_CPUINFO_VALIDFLAG;

    //handler the exp
    if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNDEFINSTR)
    {
        printk("Usage Fault: δ����ָ��,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVSTATE)
    {
        printk("Usage Fault: ��ͼ����ARM״̬,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_INVPC)
    {
        printk("Usage Fault: ����PCʱʹ���˷Ƿ�ֵ,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_NOCP)
    {
        printk("Usage Fault: Э������ָ�����, PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_UNALIGNED)
    {
        printk("Usage Fault: �Ƕ����ַ����, PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else if(pg_scb_reg->UFSR & CN_EXP_UFSR_DIVBYZERO)
    {
        printk("Usage Fault: ��0����,PC = 0x%08x\n\r",CoreRegister->spr_pc);
    }
    else
    {
        printk("Usage Fault: %s\n\r","Unknown Fault");
    }
    //clear the flag of exp
    pg_scb_reg->HFSR = 0xFFFFFFFF;
    pg_scb_reg->BFSR = 0xFF;
    pg_scb_reg->MFSR = 0xFF;
    pg_scb_reg->UFSR = 0xFFFF;


#if (DEBUG == 1)
   head.ExpAction = EN_EXP_DEAL_WAIT;
#else
   head.ExpAction = EN_EXP_DEAL_RESET;
#endif
   head.ExpInfo  = (u8 *)(&ExpRecord);
   head.ExpInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.ExpType = CN_EXP_TYPE_USAGE_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = Exp_Throw(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// �������ܣ��˲�����Ҫ��������������ͨ���쳣��Ϣ��NVIC�Ĳ��ִ����쳣��Ϣ�ļĴ���
// ���������layer,�׳����
//           parahead���׳�ʱ�Ĳ���
//           endian, ��Ϣ�Ĵ洢��ʽ
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// =============================================================================
bool_t  HardExp_Analysis(struct ExpThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //�Ǳ�����Ϣ���������
        printk("CPU����Ӳ��û���쳣\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->ExpInfoLen)//�������Ƿ�����
        {
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->ExpInfo);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                printk("�쳣����: ");
                switch(parahead->ExpType)
                {
                    case CN_EXP_TYPE_HARD_FAULT: printk("hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: printk("�洢�������쳣\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: printk("�����쳣\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: printk("�÷��쳣\r\n");break;
                    default: break;
                }
                printk("�쳣���ն���: ");
                switch(parahead->ExpAction)
                {
                    case EN_EXP_DEAL_RECORD: printk("����¼\r\n");break;
                    case EN_EXP_DEAL_RESET: printk("Ӳ����λ\r\n");break;
                    case EN_EXP_DEAL_REBOOT: printk("��ת��������ַ,���³�ʼ���ڴ��ʱ��\r\n");break;
                    case EN_EXP_DEAL_RESTART: printk("��ת��Ԥ����֮������е�ַ\r\n");break;
                    case EN_EXP_DEAL_WAIT: printk("��ѭ���ȴ�\r\n");break;
                    default: break;
                }
                //EXP FAULT ��Ϣ����
                printk("�쳣ʱCPU�Ĵ���ֵ:\n\r");
                printk("R0 :0X%08x R1 :0x%08x R2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r0,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r1,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r2);
                printk("R3 :0X%08x R4 :0x%08x R5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r3,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r4,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r5);
                printk("R6 :0X%08x R7 :0x%08x R8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r6,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r7,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r8);
                printk("R9 :0X%08x R10:0x%08x R11:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r9,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r10,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r11);
                printk("R12:0X%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r12);
                printk("SP :0X%08x LR :0x%08x PC :0x%08x  XPSR:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.spr_sp,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_lr,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_pc,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_xpsr);
                printk("NVIC Exception Registers:\n\r");
                printk("BFAR:0x%08x MMAR:0x%08x MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfar,\
                                                                 mycpuinfo->NvicInfo.mmar,\
                                                                 mycpuinfo->NvicInfo.mfsr);
                printk("HFSR:0x%08x DFSR:0x%08x BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.hfsr,\
                                                                 mycpuinfo->NvicInfo.dfsr,\
                                                                 mycpuinfo->NvicInfo.bfsr);
                printk("UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);

                result = true;
            }
            else
            {
                printk("CPU Info Decode:ivalid info flag!\n\r");
                result = false; //���ܲ���һ����CPU���߰汾���޷�����
            }
        }
        else
        {
            printk("CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }

    return result;
}
// =============================================================================
// �������ܣ��쳣��Ϣ�����������쳣��Ϣ
// ���������layer,�׳����
//           parahead���׳�ʱ�Ĳ���
//           endian, ��Ϣ�Ĵ洢��ʽ
// �����������
// ����ֵ  : true�ɹ�, falseʧ��
// =============================================================================
bool_t  HardExp_Decoder(struct ExpThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //�Ǳ�����Ϣ���������
        printf("CPU����Ӳ��û���쳣\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->ExpInfoLen)//�������Ƿ�����
        {
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->ExpInfo);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                printf("�쳣����: ");
                switch(parahead->ExpType)
                {
                    case CN_EXP_TYPE_HARD_FAULT: printf("hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: printf("�洢�������쳣\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: printf("�����쳣\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: printf("�÷��쳣\r\n");break;
                    default: break;
                }
                printf("�쳣���ն���:");
                switch(parahead->ExpAction)
                {
                    case EN_EXP_DEAL_RECORD: printf("����¼\r\n");break;
                    case EN_EXP_DEAL_RESET: printf("Ӳ����λ\r\n");break;
                    case EN_EXP_DEAL_REBOOT: printf("��ת��������ַ,���³�ʼ���ڴ��ʱ��\r\n");break;
                    case EN_EXP_DEAL_RESTART: printf("��ת��Ԥ����֮������е�ַ\r\n");break;
                    case EN_EXP_DEAL_WAIT: printf("��ѭ���ȴ�\r\n");break;
                    default: break;
                }
                //EXP FAULT ��Ϣ����
                printf("�쳣ʱCPU�Ĵ���ֵ:\n\r");
                printf("R0 :0X%08x R1 :0x%08x R2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r0,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r1,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r2);
                printf("R3 :0X%08x R4 :0x%08x R5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r3,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r4,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r5);
                printf("R6 :0X%08x R7 :0x%08x R8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r6,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r7,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r8);
                printf("R9 :0X%08x R10:0x%08x R11:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r9,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r10,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r11);
                printf("R12:0X%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r12);
                printf("SP :0X%08x LR :0x%08x PC :0x%08x  XPSR:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.spr_sp,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_lr,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_pc,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_xpsr);
                printf("NVIC Exception Registers:\n\r");
                printf("BFAR:0x%08x MMAR:0x%08x MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfar,\
                                                                 mycpuinfo->NvicInfo.mmar,\
                                                                 mycpuinfo->NvicInfo.mfsr);
                printf("HFSR:0x%08x DFSR:0x%08x BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.hfsr,\
                                                                 mycpuinfo->NvicInfo.dfsr,\
                                                                 mycpuinfo->NvicInfo.bfsr);
                printf("UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);
            }
            else
            {
                printf("CPU Info Decode:ivalid info flag!\n\r");
                result = false; //���ܲ���һ����CPU���߰汾���޷�����
            }
        }
        else
        {
            printf("CPU Info Decode:incomplete info frame!\n\r");
            result = false;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ��쳣���ݽ����������쳣Ӳ�����뺯����ע��,��ʼ���쳣ģ������
// �����������
// �����������
// ����ֵ     :true�ɹ�, falseʧ��
// =============================================================================
bool_t HardExp_InfoDecoderInit(void)
{
    static struct ExpInfoDecoder HardExpDecoder;
    bool_t result;
    HardExpDecoder.MyDecoder = HardExp_Decoder;
    HardExpDecoder.DecoderName = CN_HARDEXP_DECODERNAME;
    result = Exp_RegisterThrowInfoDecoder(&HardExpDecoder);

    return result;
}

// =============================================================================
// �������ܣ��쳣��ʼ��,�����쳣������ĳ�ʼ��
// �����������ʱ�ޣ�����������
// �����������
// ����ֵ     :true�ɹ�, falseʧ��
// =============================================================================
void HardExp_Init(void)
{
    __Exp_TableSet();
}
