#include <stdint.h>
#include <cpu.h>
#include <int.h>
#include <int_hard.h>
#include "cpu_peri.h"
//#include <core_cm3.h>


/*----------------------------------------------*/

struct SystickReg      //0xe000e010
{
    vu32 ctrl;
    vu32 reload;    //����������ʱ?������װ�ص�ֵ
    vu32 current;   //��ȡʱ���ص�ǰ��������ֵ?д������?����CTRL�е�COUNTFLAG ��־
    vuc32 verify;   //SysTickУ׼��ֵ�Ĵ���
};


struct SystickReg *pg_systick_reg = (void *)0;
#define CN_CFG_FCLK 999
#define bo_systick_ctrl_clksource 0
#define bo_systick_ctrl_enable    0


#define __SEV()  do { } while(0)
#define __WFE()  do { } while(0)
/*----------------------------------------------*/


extern struct IntMasterCtrl  tg_int_global;
extern void __DJY_ScheduleAsynSignal(void);

#define CFG_TICKMODE_DYNAMIC        true
#define CFG_REAL_CRITICAL           100     //�����뵱ǰtick�ж�ʱ��С�ڴ� uS �������޸�tick�ж�ʱ��

static s32 s_gCurrentTicks = 1;     // ��ǰtick�������ڡ�
static s32 s_gCriticalCycle;        // CFG_REAL_CRITICAL ��Ӧ�Ķ�ʱ������ֵ
static s32 s_gTicksLimit;           // 24bit��ʱ�����ζ�ʱ���ticks��
extern s64 g_s64OsTicks;
u32 Exp_SystickTickHandler(ptu32_t timeline);

__attribute__((weak)) void __DjyInitTick(void)
{
    u32 a=7;
    //todo :�ڴ���Ӷ�ʱ����ʼ������
    Int_Register(29);
    Int_SettoAsynSignal(29);
    Int_IsrConnect(29,Exp_SystickTickHandler);
    Int_RestoreLine(29);

    /* interface to OS(djy) */
    s_gTicksLimit = (s32)((s64)0xffffffff*CN_CFG_TICK_HZ/CN_CFG_FCLK);
    s_gCriticalCycle =(u32)((u64)CN_CFG_FCLK * CFG_REAL_CRITICAL / 1000000);

    /* interface to HW */
    // Enable Private Timer for periodic IRQ
//  setPriorityMask(0x1F);
    Int_ContactLine(29);
    Int_SetPrio(29, a);
//  init_private_timer(0xF0000, 0);
    init_private_timer(0x10000000, 0);  // 0 - Auto Reload
    start_private_timer();
}

//------------------------------------------------------------------------------
//���ܣ�����CPU�����߻����¼�״̬��ʹCPU�ڹ��жϺ�ȷʵ�ܱ������ڹ��ж��ڼ�Ļ����¼�
//     ���жϻ�ദ����źţ����ѣ����ۻ����¼������� WFE��ARM��ָ�����CPU���ܲ�ͬ��
//     ֮ǰ����֮��
//��������
//���أ���
//ע�����������͵ر����ڵ͹��Ŀ��ơ�
//------------------------------------------------------------------------------
void CleanWakeupEvent(void)
{
    __SEV();
    __WFE();
}

// =============================================================================
// ����:��Cortex Mϵͳ�ں��У��ں�ϵͳ��ʱ��SYSTICK��ʱ�ж������쳣���������������첽�ź�
//     ��ں��������±��� g_s64OsTicks ������һ��tick�����Ϊ1��tick��
// ����:��
// ����:��
// =============================================================================
u32 Exp_SystickTickHandler(ptu32_t timeline)
{
    s32 inc;
    g_bScheduleEnable = false;
//  tg_int_global.en_asyn_signal_counter++;
    tg_int_global.nest_asyn_signal++;
//  if(!DjyGetUpdateTickFlag())
//      DjyUpdateTicks(1);
//  else
//      DjySetUpdateTickFlag(false);
//  g_s64OsTicks += s_gCurrentTicks;
    inc = s_gCurrentTicks;
    s_gCurrentTicks = 1;
    pg_systick_reg->ctrl;   //���㣬�� __DjyGetSysTime �������ʹ��
    DJY_ScheduleIsr(inc);
    tg_int_global.nest_asyn_signal--;
//  tg_int_global.en_asyn_signal_counter--;
    if(g_ptEventReady != g_ptEventRunning)
        __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
}

// =============================================================================
// ���ܣ���ȡ��tick������ϵͳʱ�䣬���bsp���ʹ����ר�ö�ʱ���ṩϵͳʱ�ӣ�����Բ��ṩ
//      ����������ȡʱ���� g_s64OsTicks ���϶�ʱ���ĵ�ǰ����ֵ��
//      ��ȡ��ǰ����ֵ�ر�Ҫע�⣬����ڹ��첽�ź��ڼ���ñ��������ڼ䷢���˶�ʱ�����أ�
//      ����ǡ���ڱ������ڹ��жϺ����ڶ� cycle ֮ǰ�����˶�ʱ�����أ�����Ҫ����ʱ����ת
//      �����⡣
// ��������
// ���أ���ǰus��
// =============================================================================
__attribute__((weak))   u64 __DjyGetSysTime(void)
{
    s64 current;
    s32 cyclep,cyclen,interval,reload,ctrl;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    interval = s_gCurrentTicks;
    cyclep = pg_systick_reg->current;
    reload = pg_systick_reg->reload;
    ctrl = pg_systick_reg->ctrl;
    cyclen = pg_systick_reg->current;
    current = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    if(cyclen > cyclep)     //��������˵���ڶ��Ĵ��������з����˶�ʱ������
    {
        cyclen = reload - cyclen;
        current += interval;
    }
    else                    //�ڶ��Ĵ���������δ������ʱ�����أ��������ų�֮ǰ������
    {
        if(ctrl & 0x10000)      //˵�����Ĵ���ǰ��ʱ���Ѿ������ˣ����жϻ�û����Ӧ��
        {
            cyclen = reload - cyclen;
            current += interval;
        }
        else
        {
            cyclen =(s32)((s64)interval*CN_CFG_FCLK/CN_CFG_TICK_HZ - cyclen);
        }
    }
    current = current*CN_CFG_TICK_US + ((s64)cyclen*CN_CFG_FINE_US >>16);

    return (u64)current;
}

//----�����´�ticks�ж�ʱ��-----------------------------------------------------
//���ܣ��㶨tickģʽ�£�����tick��������1��tick����ֱ�ӷ���1����ʱ������ά�ֲ��䡣
//     ��̬tickģʽ�£�ͨ���޸Ķ�ʱ���������ѵ���tick�����Ϊ����Ticks������ֻ��idle
//      �¼�����ʱ���Ż�����tick�������CPU�����Ѻ�idle����һ���ܼ�ʱ�������ʱ�䣬��
//      ÿ�ζ�ʱ����������жϼ����Ϊ1������Ticks�ĺ��壬�Ǵ��ϴ�tick�ж�����ģ���
//      ��ʱ���ǳ����������ߵģ������Ҫ���Ǵ��ϴ�tick�ж�������ʱ���Ѿ��ߵ�ʱ�䣬����
//      ���ඨʱ���в�ͬ�Ĵ���
//     ��ʱ���������ࣺ
//     1����0�������趨ֵ��Ȼ�������жϣ���ʱ����0��ʼ���¼�ʱ�����ֶ�ʱ����ֻ��Ҫ�޸���
//        ��ֵ���ɣ����뿼�ǵ�ǰ�Ѿ��߹���ʱ�䡣���´��жϼ����Ϊ1�Ĺ�����tick�жϵ�ISR
//        ����ɡ�
//     2������cortex-m��24λsystick��ʱ������reload��ʼ��������0��Ȼ���reload���¿�ʼ
//        �����ģ��������굱��tick�󣬻�Ҫ��reload���õ�1��tick���ر�ע�⣬���ö�ʱ��
//        ʱ��Ҫ��ȥ���ϴ�ISR�ж�������ʱ���Ѿ��߹���ʱ�䡣
//
//��������
//���أ���ǰʱ��
//-----------------------------------------------------------------------------
__attribute__((weak)) u32 Tick_SetNextTimeTick(s32 Ticks)
{
#if CFG_TICKMODE_DYNAMIC == true
    s32 temp;
    atom_high_t atom_high;
    if(Ticks > s_gTicksLimit)
        Ticks = s_gTicksLimit;
    if(s_gCurrentTicks == Ticks)
        return Ticks;
    temp = pg_systick_reg->current;
    //tick�жϿ쵽�ˣ�Ϊ�ܹ������ٽ������µ����⣬������ı�tickʱ��
    //���ڵ��ñ�����ʱû�йر�ʵʱ�жϣ���ʵʱ�жϵ�ISRִ�в��ܴ���CFG_REAL_CRITICAL΢��
    if(temp >= s_gCriticalCycle)
    {
        temp = (s32)(s_gCurrentTicks - Ticks )* (s64)CN_CFG_FCLK / CN_CFG_TICK_HZ;

        atom_high = Int_HighAtomStart();
        temp = pg_systick_reg->current - temp;
        pg_systick_reg->reload = temp;
        pg_systick_reg->current = temp;
        Int_HighAtomEnd(atom_high);
        pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
//        pg_systick_reg->reload = 0xffffff;
        s_gCurrentTicks = Ticks;
    }
    return s_gCurrentTicks;

#else       // for CFG_TICKMODE_DYNAMIC == true
    return 1;
#endif      // for CFG_TICKMODE_DYNAMIC == true
}

