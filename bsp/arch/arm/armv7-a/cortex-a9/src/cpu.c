#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "djyos.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "board-config.h"

#define CN_CFG_TIME_BASE_HZ 250
#define CN_INT_LINE_TIMER3	CN_INT_LINE_29

#if (!CN_CFG_USE_USERTIMER)
#define CN_TIME_ROUNDING    (32768U)//���������ֵ
#define TIME_GLUE           (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_BASE_HZ/Mhz):(Mhz/CN_CFG_TIME_BASE_HZ))
#define FAST_TIME_GLUE      ((1<<16)/TIME_GLUE)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):\
                            (TIME_GLUE>=100)?(4*TIME_GLUE):((4+(100/TIME_GLUE))*TIME_GLUE))
static u64 g_time_base_tick=0;
static u32 __DjyIsrTime(ptu32_t line);
//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
void __InitTimeBase(void)
{
    Int_Register(CN_INT_LINE_TIMER3);
    Int_CutLine(CN_INT_LINE_TIMER3);
    //����timer3��Ϊtick�ж�
    Int_IsrConnect(CN_INT_LINE_TIMER3,__DjyIsrTime);
    Int_SettoAsynSignal(CN_INT_LINE_TIMER3);          //tick�жϱ���Ϊ�첽�ź�
    //�������ö�ʱ������������board-config.h��CN_CFG_TICK_US��CN_CFG_TICK_HZ��
    //CN_CFG_FINE_US��CN_CFG_FINE_HZ�Ķ���һ��
    Timer_SetClkSource(3,0);          //pclkԤ��Ƶ����1/2��Ƶ
    Timer_SetPrecale(1,CN_CFG_TIMER_CLK/CN_CFG_TIME_BASE_HZ/2 -1);
    //���ϰѶ�ʱ������ʱ�ӵ�Ƶ����ΪCN_CFG_FINE_HZ
    //Timer_SetCounter(3,(uint16_t)((u32)CN_CFG_TICK_US*1000/CN_CFG_FINE_US));
    Timer_SetCounter(3, CN_LIMIT_UINT16);
    Timer_SetType(3,1);                //����tick��ʱ����������
    Timer_Reload(3);                    //���ض�ʱֵ
}

void __DjyStartTimeBase(void)
{
    Timer_Start(3);                     //������ʱ��
    Int_RestoreAsynLine(CN_INT_LINE_TIMER3);//����tick�ж�S
}

//��ȡ���Զ�ʱ�����ֵ
u32 __Djy_GetDelayMaxCnt(void)
{
    return CN_LIMIT_UINT16;
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

//----��������tick�ж�ʱ��---------------------------------------------------------------
//����: �������ö�ʱ��reloadֵ.
//����: time:��λus
//����: ��
//��ע: ����������ֲ���к���.
//������ڲ����ú��������ﲻ��time��ֵ���жϣ��ɵ�����ȷ����ȷ��
//-----------------------------------------------------------------------------
void __Djy_SetTimeBaseCnt(u32 cnt)
{
    if(cnt>CN_LIMIT_UINT16)
    {
        //�����ϲ����ܳ��ִ��¼�
        return;
    }
    g_time_base_tick += pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3;
    pg_timer_reg->TCNTB3 = cnt;
    pg_timer_reg->TCNTO3 = pg_timer_reg->TCNTB3;
}

//----��ȡtick�ж�ʱ��---------------------------------------------------------------
//����: ��ȡ����ʱ�жϷ���ʱ��reload��current�Ĳ�ֵ.
//����: ��
//����: reload��Ӧ��USֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
static u32 __Djy_GetTimeBaseRealCnt(void)
{
//    return (pg_timer_reg->TCNTB3 - pg_timer_reg->TCNTO3);
}

//----��ȡ��ʱ����reloadֵ��Ӧ��USֵ---------------------------------------------------------------
//����: ��ȡ��ʱ��reloadֵ��Ӧ��USֵ.
//����: ��
//����: reload��Ӧ��USֵ
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
u32 __Djy_GetTimeBaseReload(void)
{
    return (pg_timer_reg->TCNTB3);
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            (us*FAST_TIME_GLUE + CN_TIME_ROUNDING)>>16);
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    g_time_base_tick += cnt;
    return g_time_base_tick;
}

u64 __DjyGetSysCnt(void)
{
    u32 cnt=0;
    cnt = __Djy_GetTimeBaseRealCnt();
//  if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
//      return (g_time_base_tick+cnt+__Djy_GetTimeBaseReload());
//  else
        return (g_time_base_tick+cnt);
}


//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTime Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTime ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DJY_GetSysTime �������á�
//      ���systime��ʹ��ticks��Ϊʱ�����������ɱ��ֿպ�����
//-----------------------------------------------------------------------------
u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    atom_low_t atom_low;

    atom_low = Int_LowAtomStart();
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
           (temp*FAST_TIME_GLUE+(u64)CN_TIME_ROUNDING)>>16:
            (temp*TIME_GLUE));
    Int_LowAtomEnd(atom_low);
    return time;
}

static u32 __DjyIsrTime(ptu32_t line)
{
    u32 tick=0;
    tick = __Djy_GetTimeBaseReload();
    Djy_IsrTimeBase(tick);
    return 0;
}

#endif

// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void CPU_Reboot(void)
{

}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void CPU_Reset(void)
{
    volatile u32 *SWRST = (u32*)0x4C000044;

#if CN_CPU_OPTIONAL_CACHE == 1
    Cache_CleanData();
    Cache_InvalidInst();
#endif
    *SWRST = 0x533C2416;
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void CPU_RestartSystem(void)
{

}

