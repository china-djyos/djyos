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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//3. ����: 2018-05-31
//   ����: chj
//   �°汾��: V1.0.2
//   �޸�˵��: ��reboot���������
//  InitCpu_Addr=*(u32*)0x00000004;�ĳ���InitCpu_Addr = (u32)Init_Cpu;
//  0x00000004��ĳЩCPU�Ͽ��ܻ����
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "dbug.h"
#include <board-config.h>

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_MSP(uint32_t topOfProcStack);
extern void Init_Cpu(void);
// void __start_systick(void);

#if (CN_USE_TICKLESS_MODE)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_SAVE_ASYN    (0U)
#define CN_TIME_ROUNDING    (32768U)//���������ֵ
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
                            //((Mhz%CN_CFG_TIME_BASE_HZ==0) ? (Mhz/CN_CFG_TIME_BASE_HZ) :((float)Mhz/CN_CFG_TIME_BASE_HZ)))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(500*TIME_GLUE):((500*CN_CFG_TIME_BASE_HZ)/Mhz))
#define NOTINT_FLAG             (0U)
#define GETSYSCNT_FLAG          (1U)
#define GETTIMEBASECNT_FLAG     (2U)
static u64 g_time_base_tick=0;
static bool_t bg_cnt_int_flag = false;
static u32 g_time_base_reload=0;
static u32 g_per_sys_cnt = 0;
static u8 bg_reload_flag = NOTINT_FLAG;
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
#if (CN_CFG_USE_BYPASSTIMER)
extern void Bypass_TimerInit(void);
extern void Bypass_Timer_PerInit(void);
#endif

void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Djy_IsrTimeBase);
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
#if (CN_CFG_USE_BYPASSTIMER)
    Bypass_Timer_PerInit();
#endif
}

void __DjyStartTimeBase(void)
{
    pg_systick_reg->reload = CN_LIMIT_UINT24;
    pg_systick_reg->current = CN_LIMIT_UINT24;
    g_time_base_reload = CN_LIMIT_UINT24;
#if (CN_CFG_USE_BYPASSTIMER)
    Bypass_TimerInit();
#endif
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //���������ж�
                                |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
}

u32 __Djy_GetDelayMaxCnt(void)
{
    return CN_LIMIT_UINT24;
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
    u32 temp_reload = 0;
    u32 temp_cur = 0;
    if((cnt>CN_LIMIT_UINT24) || (cnt==0) || (bg_reload_flag == GETSYSCNT_FLAG))
    {
        //�����ϲ����ܳ��ִ��¼�
        return;
    }
    if( bg_reload_flag != NOTINT_FLAG )
        temp_reload = pg_systick_reg->reload;
    else
        temp_reload = g_time_base_reload;
//    if(pg_systick_reg->current < TIME_BASE_MIN_GAP)
//        return;
//    pg_systick_reg->reload = cnt;
    Int_CutTrunk();
    pg_systick_reg->reload = cnt;
    temp_cur = pg_systick_reg->current;
    pg_systick_reg->current = cnt;
    if(temp_reload > temp_cur )
            g_time_base_tick += temp_reload - temp_cur;
    if((bg_reload_flag == NOTINT_FLAG)&&(temp_cur > g_per_sys_cnt))
    {
        pg_systick_reg->reload = temp_reload;
        pg_systick_reg->current = temp_reload;
        bg_cnt_int_flag = true;
        Int_ContactTrunk();
        //pg_systick_reg->reload = CN_LIMIT_UINT24;
        return;
    }
    Int_ContactTrunk();
//    if(g_time_base_tick < g_per_cur)
//        g_time_base_tick = g_per_cur;
    bg_reload_flag = NOTINT_FLAG;
    g_time_base_reload = cnt;
#if CN_CFG_SAVE_ASYN
    pg_systick_reg->reload = CN_LIMIT_UINT24;
#endif
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
    if( bg_reload_flag )
        return (pg_systick_reg->reload - pg_systick_reg->current);
    else
        return (g_time_base_reload - pg_systick_reg->current );
}

u32 __Djy_GetTimeBaseReload(void)
{
    return g_time_base_reload;
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    u64 temp = 0;
    temp = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            ((us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
    if( temp < TIME_BASE_MIN_GAP )
        temp = TIME_BASE_MIN_GAP;
    return temp;
}

u32 __Djy_TimeBaseCntToUs(u64 cnt)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (cnt/(u32)TIME_GLUE):
            (u32)(((u64)(cnt*TIME_GLUE))>>16));
}

//ע�⣺�˺���ֻ����SYSTICK�жϴ���������ʹ��
u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    bg_reload_flag = GETTIMEBASECNT_FLAG;
    bg_cnt_int_flag = false;
    g_time_base_tick += cnt;
    return (g_time_base_tick + __Djy_GetTimeBaseRealCnt());
}

u64 __DjyGetSysCnt(void)
{
    u32 cnt2=0;
    u64 temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    cnt2 = pg_systick_reg->current;
    if((pg_systick_reg->ctrl & bm_systick_ctrl_countflag) || bg_cnt_int_flag)
    {
        bg_cnt_int_flag = true;
        bg_reload_flag = GETSYSCNT_FLAG;
    }
    else
        bg_reload_flag = NOTINT_FLAG;
    //cnt = __Djy_GetTimeBaseRealCnt();
    g_per_sys_cnt = cnt2;
    if( bg_reload_flag == GETSYSCNT_FLAG )
    {
        temp = g_time_base_tick + g_time_base_reload + pg_systick_reg->reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
    else
    {
        temp = g_time_base_tick + g_time_base_reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTime Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTime ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DjyGetSysTime �������á�
//      ���systime��ʹ��ticks��Ϊʱ�����������ɱ��ֿպ�����
//-----------------------------------------------------------------------------
u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
//    atom_low_t atom_low;

//    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (temp/(u32)TIME_GLUE):
            ((u64)(temp*TIME_GLUE))>>16);
    //Int_LowAtomEnd(atom_low);
    return time;
}
#endif
#else
extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
__attribute__((weak)) void __InitTimeBase(void)
{}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_IsrTick);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //���������ж�
                                |(1<<bo_systick_ctrl_clksource);//��ʹ���ⲿʱ��

}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DjyGetSysTime �������á�
//      ���systime��ʹ��ticks��Ϊʱ�����������ɱ��ֿպ�����
//-----------------------------------------------------------------------------
__attribute__((weak)) s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return time;
}
#endif


extern void Load_Preload(void);
#include <blackbox.h>
#include <osboot.h>
#include <IAP_Inner.h>
extern tagIapVar pg_IapVar;

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG         (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)
typedef struct
{
    union
    {
        u32 value;
        struct
        {
            u32 procrst:1;
            u32 res3:2;
            u32 extrst:1;
            u32 res2:4;
            u32 res1:16;
            u32 key:8;
        }bits;
    }cr;
    union
    {
        u32 value;
        struct
        {
            u32 ursts:1;
            u32 res3:7;
            u32 rsttype:3;
            u32 res2:5;
            u32 nrstl:1;
            u32 srcmp:1;
            u32 res1:14;
        }bits;
    }sr;
    union
    {
        u32 value;
        struct
        {
            u32 ursten:1;
            u32 res3:3;
            u32 urstien:1;
            u32 res2:3;
            u32 erstl:4;
            u32 res1:12;
            u32 key:8;
        }bits;
    }mr;
}tagRstc;
// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void reboot(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        debug_printf("reboot","IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_REBOOT);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTREBOOT_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
#if CN_CPU_OPTIONAL_CACHE == 1
        SCB_DisableDCache();
#endif
        u32 InitCpu_Addr;
        InitCpu_Addr = (u32)Init_Cpu;
        ((void (*)(void))(InitCpu_Addr))();
    }
    return;
}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ���������õ���λ
// ��������
// ���أ���
// =============================================================================
void reset(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        debug_printf("reset","IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_SRST);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRESET_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
    }
    return;
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void restart_system(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        debug_printf("restart_system","IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_RELOAD);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRELOAD_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        __set_PSP((uint32_t)msp_top);
        __set_MSP((uint32_t)msp_top);
        Load_Preload();
    }
    return;
}

enBootMode GetBootMethodSoft(void)
{
    vu32 *addr;
    vu32 value;
    enBootMode result;
    addr = (u32*)&pg_IapVar.IbootFlag[12];
    value = *addr;
    if(value == CN_BOOT_SOFTRESET_FLAG)
    {
        result = EN_BOOT_SRST;
    }
    else if(value == CN_BOOT_SOFTREBOOT_FLAG)
    {
        result = EN_BOOT_REBOOT;
    }
    else if(value == CN_BOOT_SOFTRELOAD_FLAG)
    {
        result = EN_BOOT_RELOAD;
    }
    else if(value == CN_BOOT_HARDRST_FLAG)
    {
        result = EN_BOOT_HRST;
    }
    else
    {
        result = EN_BOOT_POWERDOWN;
    }
    value = CN_BOOT_HARDRST_FLAG;
    *addr = value;
    return result;
}
enum
{
    EN_RESET_GENERALRST = 0,
    EN_RESET_BACKUPRST,
    EN_RESET_WDTRST,
    EN_RESET_SOFTRST,
    EN_RESET_USERRST,
    EN_RESET_RES,
}enResetType;
enBootMode GetBootMethodHard(void)
{
    enBootMode result;
    tagRstc rstc;
    rstc.cr.value = *(vu32 *)(0x400e1800);
    rstc.sr.value = *(vu32 *)(0x400e1804);
    rstc.mr.value = *(vu32 *)(0x400e1808);
    switch(rstc.sr.bits.rsttype)
    {
        case EN_RESET_GENERALRST:
            printk("General RESET\n\r");
            result =EN_BOOT_POWERDOWN;
            break;
        case EN_RESET_BACKUPRST:
            printk("BackUp RESET\n\r");
            result =EN_BOOT_UNKNOWN;
            break;
        case EN_RESET_WDTRST:
            printk("WDT RESET\n\r");
            result =EN_BOOT_UNKNOWN;
            break;
        case EN_RESET_SOFTRST:
            printk("SOFT RESET\n\r");
            result =EN_BOOT_SRST;
            break;
        case EN_RESET_USERRST:
            printk("USER RESET\n\r");
            result =EN_BOOT_HRST;
            break;
        default:
            printk("UNKOWN MODE\n\r");
            result =EN_BOOT_UNKNOWN;
            break;
    }
    return result;
}