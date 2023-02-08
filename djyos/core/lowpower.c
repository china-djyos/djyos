#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include <systick.h>
#include "string.h"
#include "lowpower.h"
#include "int.h"
#include "int_hard.h"
#include "cpu_peri.h"
#include <shell.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//#define LP_DEFAULT_TRIGGER_TICK         (2U)
struct LowPowerCtrl
{
    u32 SleepLevel;                             //��ǰ�͹��ļ���
//  u32 TriggerTick;
    u32 DisableCounter;     //��ֹ�͹��Ĵ�������,0�ſ��Խ���L1�����ϼ���͹���

    u32 (*EntrySleepReCall)(u32 SleepLevel);    //����͹���״̬ǰ�Ļص�����
                                                //����ֵ��ʾ�Ƿ����������Ӧ����͹���
    u32 (*ExitSleepReCall)(u32 SleepLevel);     //�ӵ͹���״̬���Ѻ�Ļص�����

    void (*EntrySleep)(u8 sleep_level, u32 pend_ticks);    //����͹��ĵĺ���
    bool_t (*SaveSleepLevel)(u32 SleepLevel);                        //�������ߵȼ��ĺ���
    bool_t (*SaveRamL3)(void);              //��ִ�й�AsmSaveReg���ڴ汣�浽flash��

    void (*AsmSaveReg)(struct ThreadVm *running_vm,
                                bool_t (*SaveRamL3)(void),
                                void (*EntrySleep)(u8 sleep_level, u32 pend_ticks));  //��ϵͳ��L3ģʽ����ʱ��ϵͳӦ��__LP_BSP_AsmSaveReg�ķ��ص�ַ������һ�д��봦�������С�Ҫʵ������˳��ָ����

};

struct LowPowerCtrl g_tLowPower;

//----����͹���״̬----------------------------------------------------------
//����: ���ñ�������,�����趨�ĵ͹���״̬,����ǰ������ػص�����.
//����: ��
//����: CN_SLEEP_NORMAL,CN_SLEEP_L0-L3֮һ, �����ܷ���CN_SLEEP_L4
//˵����ʹ��L3Ҫ�ر�ע�⣬����豸���������������ߺ������ûᶪʧ������ʱ����
//      ����������������������״̬����ȷ����ˣ�����������EntrySleepReCall��
//      ExitSleepReCall����������
//----------------------------------------------------------------------------
u32 LP_EntryLowPower(struct ThreadVm *vm,u32 pend_ticks)
{
    atom_low_t  atom_bak;

    if(g_tLowPower.DisableCounter == 0)
    {
        Tick_SetNextTimeTick(pend_ticks);       //��������Ϊ����ʱ�䣬��BSP����
        switch(g_tLowPower.SleepLevel)
        {
           case CN_SLEEP_L0:
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L0))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L0, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
               }
               return CN_SLEEP_L0;
               break;
           case CN_SLEEP_L1:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L1))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L1, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L1);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L1;
               break;
           case CN_SLEEP_L2:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L2))
               {
                   g_tLowPower.EntrySleep(CN_SLEEP_L2, pend_ticks);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L2);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L2;
               break;
           case CN_SLEEP_L3:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L3))
               {
                   g_tLowPower.SaveSleepLevel(CN_SLEEP_L3);
                   //��ϵͳ��L3ģʽ����ʱ��ϵͳӦ��__LP_BSP_AsmSaveReg�ķ��ص�ַ����
                   //��һ�д��봦�������С�Ҫʵ������˳��ָ����
                   //step1��__LP_BSP_AsmSaveReg��ȡ���Լ��ķ��ص�ַ���ڵ������Ĳ����浽ջ�С�
                   //step2��__LP_BSP_SaveRamL3��ִ�й�step1���ڴ汣�浽flash�С�
                   //step3��g_tLowPower.EntrySleep����͹���
                   //step4���ӵ͹��Ļ��Ѻ󣬻ָ��ڴ�������ģ������������
                   //�ƺ�ֻ�а�__LP_BSP_SaveRamL3��g_tLowPower.EntrySleep����������
                   //�������ʵ�֡�
                   g_tLowPower.AsmSaveReg(vm, g_tLowPower.SaveRamL3, g_tLowPower.EntrySleep);
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_L3;
               break;
           case CN_SLEEP_L4:
               atom_bak = Int_LowAtomStart();
               if(g_tLowPower.EntrySleepReCall(CN_SLEEP_L4))
               {
                   g_tLowPower.SaveSleepLevel(CN_SLEEP_L4);
                   g_tLowPower.EntrySleep(CN_SLEEP_L4, 0);    //����͹���,���Ҳ��᷵������
                   //����ȷ���ã�����ĺ����������أ����ǵȴ���λ������Ĵ��룬��Ϊ��������
                   //����ִ�в�����������ʱ����ʹϵͳ���ҡ�
                   g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
               }
               Int_LowAtomEnd(atom_bak);
               return CN_SLEEP_ERROR;
               break;

           default:
               return CN_SLEEP_NORMAL;
               break;
        }
    }
    else
        return CN_SLEEP_NORMAL;
}

//----��ֹ����͹���״̬------------------------------------------------------
//����: ���ñ�������,ϵͳ�������L1�����ϼ���ĵ͹���״̬.����ʹ��ֹ����������
//      +1,�����ͬ�ȴ�����LP_EnableSleepʹ����������0��,���������L1�����ϼ���
//      �͹���״̬.
//����: ��
//����: ��ֹ��������ǰֵ
//----------------------------------------------------------------------------
u32 LP_DisableSleep(void)
{
    if(g_tLowPower.DisableCounter != CN_LIMIT_UINT32)
        g_tLowPower.DisableCounter++;
    return g_tLowPower.DisableCounter;
}

//----ʹ�ܽ���͹���״̬------------------------------------------------------
//����: ���ñ�������,��ֹ������������1,����0��,�������L1�����ϼ���͹���״̬.
//����: ��
//����: ��ֹ��������ǰֵ
//----------------------------------------------------------------------------
u32 LP_EnableSleep(void)
{
    if(g_tLowPower.DisableCounter != 0)
        g_tLowPower.DisableCounter--;
    return g_tLowPower.DisableCounter;
}

//----���õ͹��ļ���----------------------------------------------------------
//����: ���õ�ǰ�͹��ļ���,CN_SLEEP_L0---CN_SLEEP_L4
//����: Level, CN_SLEEP_NORMAL---CN_SLEEP_L4�е�һ��
//����: ��ǰ�͹��ļ���
//----------------------------------------------------------------------------
u32 LP_SetSleepLevel(u32 Level)
{
    if(Level <= CN_SLEEP_L4)
        g_tLowPower.SleepLevel = Level;
    return g_tLowPower.SleepLevel;
}


u32 LP_GetSleepLevel(void)
{
    return g_tLowPower.SleepLevel;
}

//void LP_SetTriggerTick(u32 tick)
//{
//    atom_low_t atom_bak;
//    atom_bak = Int_LowAtomStart();
//    if(tick<LP_DEFAULT_TRIGGER_TICK)
//        return;
//    else
//        g_tLowPower.TriggerTick = tick;
//    Int_LowAtomEnd(atom_bak);
//}
//
//u32 LP_GetTriggerTick(void)
//{
//    return g_tLowPower.TriggerTick;
//}
//
void LP_SetHook(u32 (*EntrySleepReCall)(u32 SleepLevel),
                u32 (*ExitSleepReCall)(u32 SleepLevel))
{
    if(EntrySleepReCall != NULL)
        g_tLowPower.EntrySleepReCall = EntrySleepReCall;
    if(ExitSleepReCall != NULL)
        g_tLowPower.ExitSleepReCall = ExitSleepReCall;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

u32 LP_EmptyReCall(u32 SleepLevel)
{
    return 1;
}

//-----------------------------------------------------------------------------
//����: ע��͹�����صĹ��ܺ���
//����: EntrySleep,����͹���״̬ǰ���õĻص�����
//      SaveSleepLevel, �������ߵȼ�
//      SaveRamL3, �����ڽ������ߵȼ�3֮ǰ���ڴ�
//      AsmSaveReg, ��ȡ���Լ��ķ��ص�ַ���ڵ������Ĳ����浽ջ��
//����: ������
//-----------------------------------------------------------------------------
void Register_LowPower_Function (void (*EntrySleep)(u8 sleep_level, u32 pend_ticks),
                                 bool_t (*SaveSleepLevel)(u32 SleepLevel),
                                 bool_t (*SaveRamL3)(void),
                                 void (*AsmSaveReg)(struct ThreadVm *running_vm,
                                 bool_t (*SaveRamL3)(void),
                                 void (*EntrySleep)(u8 sleep_level, u32 pend_ticks)))
{
    g_tLowPower.EntrySleep = EntrySleep;
    g_tLowPower.SaveSleepLevel = SaveSleepLevel;
    g_tLowPower.SaveRamL3 = SaveRamL3;
    g_tLowPower.AsmSaveReg = AsmSaveReg;

    g_tLowPower.EntrySleepReCall = LP_EmptyReCall;
    g_tLowPower.ExitSleepReCall = LP_EmptyReCall;
    g_tLowPower.SleepLevel = CN_SLEEP_L0;
    g_tLowPower.DisableCounter = 0;
    return ;
}

__attribute__((weak)) bool_t __LP_BSP_SetSleepMode(ptu32_t param)
{
    return false;
}

#pragma GCC diagnostic pop
