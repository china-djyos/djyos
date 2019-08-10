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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
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
#include "board-config.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"

void __start_systick(void);
//struct SystickReg volatile * const pg_systick_reg
 //                       = (struct SystickReg *)0xE000E010;

//struct ScbReg volatile * const pg_scb_reg
//                        = (struct ScbReg *)0xe000ed00;

#if (CN_USE_TICKLESS_MODE)
#include "tickless.h"
#if (!CN_CFG_USE_USERTIMER)
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz ? (CN_CFG_TIME_BASE_HZ/Mhz) : (Mhz/CN_CFG_TIME_BASE_HZ))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_PRECISION*TIME_GLUE):\
                                ((CN_CFG_TIME_PRECISION*CN_CFG_TIME_BASE_HZ)/Mhz))
#define NOTINT_FLAG             (0U)
#define GETSYSCNT_FLAG          (1U)
#define GETTIMEBASECNT_FLAG     (2U)
extern void Int_CutTrunk(void);
extern void Int_ContactTrunk(void);
/*�������systick�����ݽṹ*/
struct djybsp_systick_tickless_t
{
    bool int_flag;              /*�����жϱ�־*/
    bool reload_flag;           /*��װ�ر�־*/
    uint32_t reload_value;      /*��װ�ص�ֵ*/
    uint32_t cnt_before;        /*��һ�ζ�����cntֵ*/
    uint64_t total_cnt;         /*ϵͳ�ۼƵ�cntֵ*/
}static djybsp_systick;

//////////////////////////////////////tickless-ops/////////////////////////////////////////////
// =============================================================================
// ���ܣ���λsystick
// ��������
// ���أ���
// =============================================================================
static void DjyBsp_SystickReset(void)
{
    HardExp_ConnectSystick(Djy_ScheduleIsr);
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
    pg_systick_reg->ctrl =   ~((1<<bo_systick_ctrl_enable)    //��1?��
                                |(1<<bo_systick_ctrl_tickint)   //?��D��2������?D??
                                |(1<<bo_systick_ctrl_clksource));//��??��o?����?��
    djybsp_systick.int_flag = false;
    djybsp_systick.reload_flag = false;
    djybsp_systick.reload_value = 0;
    djybsp_systick.cnt_before = 0;
    djybsp_systick.total_cnt = 0;
}

// =============================================================================
// ���ܣ�����systick
// ��������
// ���أ���
// =============================================================================
static void DjyBsp_SystickStart(void)
{
    pg_systick_reg->reload = CN_LIMIT_UINT24;
    pg_systick_reg->current = CN_LIMIT_UINT24;
    djybsp_systick.reload_value = CN_LIMIT_UINT24;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //��1?��
                                |(1<<bo_systick_ctrl_tickint)   //?��D��2������?D??
                                |(1<<bo_systick_ctrl_clksource);//��??��o?����?��
}


// =============================================================================
// ���ܣ�����systick�Ķ�ʱ����
// ������Ҫ���õ�ֵ
// ���أ���
// =============================================================================
static void DjyBsp_SystickSetReload(uint32_t cnt)
{
    uint32_t temp_reload = 0;
    uint32_t temp_cur = 0;
    if((cnt>CN_LIMIT_UINT24) || (cnt==0) || \
            ((djybsp_systick.reload_flag) == GETSYSCNT_FLAG))
    {
        //�����ϲ����ܳ��ִ��¼�
        return;
    }
    if( (djybsp_systick.reload_flag) != NOTINT_FLAG )
        temp_reload = pg_systick_reg->reload;
    else
        temp_reload = (djybsp_systick.reload_value);
    Int_CutTrunk();
    pg_systick_reg->reload = cnt;
    temp_cur = pg_systick_reg->current;
    pg_systick_reg->current = cnt;
    if(temp_reload > temp_cur )
        djybsp_systick.total_cnt += temp_reload - temp_cur;
    if(((djybsp_systick.reload_flag) == NOTINT_FLAG) && (temp_cur > djybsp_systick.cnt_before))
    {
        pg_systick_reg->reload = temp_reload;
        pg_systick_reg->current = temp_reload;
        djybsp_systick.int_flag = true;
        Int_ContactTrunk();
        return;
    }
    Int_ContactTrunk();
    djybsp_systick.reload_flag = NOTINT_FLAG;
    djybsp_systick.reload_value = cnt;
}

// =============================================================================
// ���ܣ���ȡsystick��ǰ��cntֵ
// ��������
// ���أ���ǰcntֵ
// =============================================================================
static uint32_t DjyBsp_SystickReadCnt(void)
{
    if( djybsp_systick.reload_flag )
        return (pg_systick_reg->reload - pg_systick_reg->current);
    else
        return ((djybsp_systick.reload_value) - (pg_systick_reg->current) );
}

// =============================================================================
// ���ܣ�ˢ��systick��ǰ�ۼƵ�cntֵ
// ������������cnt��
// ���أ���ǰ�ۼƵ�cnt��
// =============================================================================
static uint64_t Djybsp_SystickGetTotalCntIsr(uint32_t cnt)
{
    djybsp_systick.reload_flag = GETTIMEBASECNT_FLAG;
    djybsp_systick.int_flag = false;
    djybsp_systick.total_cnt += djybsp_systick.reload_value;
    return ((djybsp_systick.total_cnt) + DjyBsp_SystickReadCnt());
}

// =============================================================================
// ���ܣ���ȡsystick��ǰ�ۼƵ�cntֵ
// ��������
// ���أ���ǰ�ۼƵ�cntֵ
// =============================================================================
static uint64_t DjyBsp_SystickGetTotalCnt(void)
{
    uint32_t cnt2=0;
    uint64_t temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    cnt2 = pg_systick_reg->current;
    if((pg_systick_reg->ctrl & bm_systick_ctrl_countflag) || (djybsp_systick.int_flag))
    {
        djybsp_systick.int_flag = true;
        djybsp_systick.reload_flag = GETSYSCNT_FLAG;
    }
    else
        djybsp_systick.reload_flag = NOTINT_FLAG;
    djybsp_systick.cnt_before = cnt2;
    if( (djybsp_systick.reload_flag) == GETSYSCNT_FLAG )
    {
        temp = (djybsp_systick.total_cnt) + (djybsp_systick.reload_value) + pg_systick_reg->reload - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
    else
    {
        temp = (djybsp_systick.total_cnt) + (djybsp_systick.reload_value) - cnt2;
        Int_LowAtomEnd(atom_low);
        return temp;
    }
}



/*����OPSͨ������ṹ��ע�ᵽticklessģ������*/
static struct djytickless_op_t djyticklss_systick_op =
{
    .get_total_cnt = DjyBsp_SystickGetTotalCnt,
    .get_total_cnt_isr = Djybsp_SystickGetTotalCntIsr,
    .set_reload = DjyBsp_SystickSetReload,
};

static struct djytickless_register_param_t djyticklss_systick =
{
     .op = &djyticklss_systick_op,
     .freq = CN_CFG_TIME_BASE_HZ,
     .max_reload_value = CN_LIMIT_UINT24,
     .min_reload_value = TIME_BASE_MIN_GAP,
};

#endif
#endif
///////////////////////////////////////////////djy-api start//////////////////////////////////
static uint64_t gRunTicks = 0;
static bool_t gResumeTickFlag = false;

__attribute__((weak)) uint64_t __DjyGetTicks(void)
{
    return gRunTicks;
}

//�ɵ����߱�֤���ð�ȫ
__attribute__((weak)) void DjySetUpdateTickFlag(bool_t flag)
{
    gResumeTickFlag = flag;
}

__attribute__((weak)) bool_t DjyGetUpdateTickFlag(void)
{
    return gResumeTickFlag;
}

//�ɵ����߱�֤ԭ�Ӳ���
__attribute__((weak)) void DjyUpdateTicks(uint32_t ticks)
{
    gRunTicks += ticks;
}

// =============================================================================
// ���ܣ���ȡϵͳʱ��
// ��������
// ���أ���ǰus��
// =============================================================================
__attribute__((weak))   uint64_t __DjyGetSysTime(void)
{
#if     CN_USE_TICKLESS_MODE
    uint64_t temp=0;
    temp = DjyTickless_GetTotalCnt();
    return DjyTickless_CntToUs(temp);
#else
    extern s64  g_s64OsTicks;
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return (uint64_t)time;
#endif
}

// =============================================================================
// ���ܣ���ϵͳ�����Ժ���Ѹ��ֱ�־��λ
// ��������
// ���أ���
// =============================================================================
__attribute__((weak))   void __InitTimeBase(void)
{
#if     CN_USE_TICKLESS_MODE
#if (!CN_CFG_USE_USERTIMER)
    DjyTickless_Register(&djyticklss_systick);
    DjyBsp_SystickReset();
#endif
#endif
}

// =============================================================================
// ���ܣ���ʼ��systick
// ��������
// ���أ���
// =============================================================================
__attribute__((weak))   void __DjyInitTick(void)
{
#if     CN_USE_TICKLESS_MODE
    DjyBsp_SystickStart();
#else
    HardExp_ConnectSystick(Djy_ScheduleIsr);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    //��M0û�� basepri �Ĵ������첽�źſ��أ�systick �ж���ʹ���ˣ���ֱ��ʹ����
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                               // |(1<<bo_systick_ctrl_tickint)   //M0,��ʱ����������ж�
                                |(1<<bo_systick_ctrl_clksource);//��ʹ���ⲿʱ��
#endif
}
///////////////////////////////////////////////djy-api end//////////////////////////////////

extern void Init_Cpu(void);
extern void Load_Preload(void);
// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void reboot(void)
{
    u32 InitCpu_Addr;
    InitCpu_Addr = *(u32*)0x00000004;
    ((void (*)(void))(InitCpu_Addr))();
}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void reset(void)
{
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void restart_app(void)
{
    Load_Preload();
}

