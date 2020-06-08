//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "blackbox.h"
#include "stdio.h"
#include "endian.h"
#include "cpu_peri.h"
#include "int.h"
#include "djyos.h"
#include "cpu.h"
#include "dbug.h"
#include "board-config.h"
#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#endif
extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
extern void __DJY_ScheduleAsynSignal(void);

//�쳣��������ⲿ����
extern void HardExp_SystickHandler(void);
extern void HardExp_SvcHandler(void);
extern void HardExp_MemfaultHandler(void);
extern void HardExp_UsagefaultHandler(void);
extern void HardExp_BusfaultHandler(void);

extern   uint32_t   msp_top[ ];
extern   void Init_Cpu(void);
extern   void HardExp_HardfaultHandler(void);
bool_t  HardExp_Analysis(struct BlackBoxThrowPara *parahead, u32 endian);
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
    g_u32ExpTable[CN_SYSVECT_RESET]             = (uint32_t)Init_Cpu;
    g_u32ExpTable[CN_SYSVECT_NMI]               = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_HARD_FAULT]        = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_MEMMANAGE_FAULT]   = (uint32_t) HardExp_MemfaultHandler;
    g_u32ExpTable[CN_SYSVECT_BUS_FAULT]         = (uint32_t) HardExp_BusfaultHandler;
    g_u32ExpTable[CN_SYSVECT_USAGE_FAULT]       = (uint32_t) HardExp_UsagefaultHandler;
    g_u32ExpTable[7]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[8]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[9]                            = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[10]                           = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[CN_SYSVECT_SVC]               = (u32)HardExp_SvcHandler;
    g_u32ExpTable[CN_SYSVECT_DEBUG]             = (uint32_t) HardExp_HardfaultHandler;
    g_u32ExpTable[13]                           = (uint32_t) HardExp_HardfaultHandler;
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
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_MemManageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    u32 ExpAddr;
    enum EN_BlackBoxAction Action;
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
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_MEMMANAGE_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ����:ϵͳӲ�����󣬿��ܻ�������������fault�ϷµĽ��������ǣ���Ӧ�ý�һ����ѯԭ��
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_HardFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    enum EN_BlackBoxAction Action;
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
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_HARD_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);

    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ����:ϵͳ���ڵ���������쳣������������ֻ�������
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_DebugFaultHandler(u32 *core_info)
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

    return EN_BLACKBOX_DEAL_IGNORE;
}

// =============================================================================
// ����:ϵͳ�ڴ�����쳣����������Ҫ������memory manage �쳣��ص��쳣
// ����:core_info���쳣�����ֳ�
// ����: EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_BusFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_BlackBoxAction Action;
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
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_BUS_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);
    //recovethe shedule
    g_bScheduleEnable = shedule_bak;

    return Action;
}

// =============================================================================
// ���ܣ��˲�����Ҫ��������������Usage�쳣��Ϣ
// ������core_info,�쳣�����ֳ�
// ���� �� EN_ExpAction
// =============================================================================
enum EN_BlackBoxAction Exp_UsageFaultHandler(u32 *core_info)
{
    struct SysExceptionInfo ExpRecord;
    struct BlackBoxThrowPara  head;
    struct SysExpCoreInfo *CoreRegister;
    enum EN_BlackBoxAction Action;
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
   head.BlackBoxAction = EN_BLACKBOX_DEAL_WAIT;
#else
   head.BlackBoxAction = EN_BLACKBOX_DEAL_RESET;
#endif
   head.BlackBoxInfo  = (u8 *)(&ExpRecord);
   head.BlackBoxInfoLen = sizeof(struct SysExceptionInfo);
   head.DecoderName = CN_HARDEXP_DECODERNAME;
   head.BlackBoxType = CN_EXP_TYPE_USAGE_FAULT;
   HardExp_Analysis(&head,CN_CFG_BYTE_ORDER);
   Action = BlackBox_ThrowExp(&head);

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
bool_t  HardExp_Analysis(struct BlackBoxThrowPara *parahead, u32 endian)
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
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//�������Ƿ�����
        {
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->BlackBoxInfo);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                printk("�쳣����: ");
                switch(parahead->BlackBoxAction)
                {
                    case CN_EXP_TYPE_HARD_FAULT: printk("hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: printk("�洢�������쳣\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: printk("�����쳣\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: printk("�÷��쳣\r\n");break;
                    default: break;
                }
                printk("�쳣���ն���:");
                switch(parahead->BlackBoxAction)
                {
                    case EN_BLACKBOX_DEAL_RECORD: printk("����¼\r\n");break;
                    case EN_BLACKBOX_DEAL_RESET: printk("Ӳ����λ\r\n");break;
                    case EN_BLACKBOX_DEAL_REBOOT: printk("��ת��������ַ,���³�ʼ���ڴ��ʱ��\r\n");break;
                    case EN_BLACKBOX_DEAL_RESTART: printk("��ת��Ԥ����֮������е�ַ\r\n");break;
                    case EN_BLACKBOX_DEAL_WAIT: printk("��ѭ���ȴ�\r\n");break;
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
bool_t  HardExp_Decoder(struct BlackBoxThrowPara *parahead, u32 endian)
{
    bool_t result = false;
    struct SysExceptionInfo  *mycpuinfo;

    if(parahead == NULL)
    {
        //�Ǳ�����Ϣ���������
        debug_printf("cortex_EXP","CPU����Ӳ��û���쳣\r\n");
        result = false;
    }
    else
    {
        if((sizeof(struct SysExceptionInfo))== parahead->BlackBoxInfoLen)//�������Ƿ�����
        {
            //��������������������������
            mycpuinfo = (struct SysExceptionInfo *)(parahead->BlackBoxInfo);
            //�洢��ת��
            if(endian != CN_CFG_BYTE_ORDER)
            {
                __SwapExpCpuInfoByEndian(mycpuinfo);
            }
            if(CN_SYS_EXP_CPUINFO_VALIDFLAG == mycpuinfo->SysExpCpuFlag)//��ǰ�汾�����Խ���
            {
                debug_printf("cortex_EXP","�쳣����: ");
                switch(parahead->BlackBoxType)
                {
                    case CN_EXP_TYPE_HARD_FAULT: debug_printf("cortex_EXP","hard fault\r\n");break;
                    case CN_EXP_TYPE_MEMMANAGE_FAULT: debug_printf("cortex_EXP","�洢�������쳣\r\n");break;
                    case CN_EXP_TYPE_BUS_FAULT: debug_printf("cortex_EXP","�����쳣\r\n");break;
                    case CN_EXP_TYPE_USAGE_FAULT: debug_printf("cortex_EXP","�÷��쳣\r\n");break;
                    default: break;
                }
                debug_printf("cortex_EXP","�쳣���ն���:");
                switch(parahead->BlackBoxAction)
                {
                    case EN_BLACKBOX_DEAL_RECORD: debug_printf("cortex_EXP","����¼\r\n");break;
                    case EN_BLACKBOX_DEAL_RESET: debug_printf("cortex_EXP","Ӳ����λ\r\n");break;
                    case EN_BLACKBOX_DEAL_REBOOT: debug_printf("cortex_EXP","��ת��������ַ,���³�ʼ���ڴ��ʱ��\r\n");break;
                    case EN_BLACKBOX_DEAL_RESTART: debug_printf("cortex_EXP","��ת��Ԥ����֮������е�ַ\r\n");break;
                    case EN_BLACKBOX_DEAL_WAIT: debug_printf("cortex_EXP","��ѭ���ȴ�\r\n");break;
                    default: break;
                }
                //EXP FAULT ��Ϣ����
                debug_printf("cortex_EXP","�쳣ʱCPU�Ĵ���ֵ:\n\r");
                debug_printf("cortex_EXP","R0 :0X%08x R1 :0x%08x R2 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r0,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r1,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r2);
                debug_printf("cortex_EXP","R3 :0X%08x R4 :0x%08x R5 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r3,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r4,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r5);
                debug_printf("cortex_EXP","R6 :0X%08x R7 :0x%08x R8 :0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r6,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r7,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r8);
                debug_printf("cortex_EXP","R9 :0X%08x R10:0x%08x R11:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r9,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r10,\
                                                              mycpuinfo->ExpRegInfo.CoreInfo.ger_r11);
                debug_printf("cortex_EXP","R12:0X%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.ger_r12);
                debug_printf("cortex_EXP","SP :0X%08x LR :0x%08x PC :0x%08x  XPSR:0x%08x\n\r",mycpuinfo->ExpRegInfo.CoreInfo.spr_sp,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_lr,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_pc,\
                                                                       mycpuinfo->ExpRegInfo.CoreInfo.spr_xpsr);
                debug_printf("cortex_EXP","NVIC Exception Registers:\n\r");
                debug_printf("cortex_EXP","BFAR:0x%08x MMAR:0x%08x MFSR:0x%02x\n\r",mycpuinfo->NvicInfo.bfar,\
                                                                 mycpuinfo->NvicInfo.mmar,\
                                                                 mycpuinfo->NvicInfo.mfsr);
                debug_printf("cortex_EXP","HFSR:0x%08x DFSR:0x%08x BFSR:0x%02x\n\r",mycpuinfo->NvicInfo.hfsr,\
                                                                 mycpuinfo->NvicInfo.dfsr,\
                                                                 mycpuinfo->NvicInfo.bfsr);
                debug_printf("cortex_EXP","UFSR:0x%04x\n\r",mycpuinfo->NvicInfo.ufsr);
            }
            else
            {
                debug_printf("cortex_EXP","CPU Info Decode:ivalid info flag!\n\r");
                result = false; //���ܲ���һ����CPU���߰汾���޷�����
            }
        }
        else
        {
            debug_printf("cortex_EXP","CPU Info Decode:incomplete info frame!\n\r");
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
    static struct BlackBoxInfoDecoder HardExpDecoder;
    bool_t result;
    HardExpDecoder.MyDecoder = HardExp_Decoder;
    HardExpDecoder.DecoderName = CN_HARDEXP_DECODERNAME;
    result = BlackBox_RegisterThrowInfoDecoder(&HardExpDecoder);

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
