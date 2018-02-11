#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"
#include "lowpower.h"
#include "int.h"
#include "int_hard.h"

struct LowPowerCtrl g_tLowPower;

//----����͹���״̬----------------------------------------------------------
//����: ���ñ�������,�����趨�ĵ͹���״̬,����ǰ������ػص�����.
//����: ��
//����: CN_SLEEP_NORMAL,CN_SLEEP_L0-L3֮һ, �����ܷ���CN_SLEEP_L4
//˵����ʹ��L3Ҫ�ر�ע�⣬����豸���������������ߺ������ûᶪʧ������ʱ����
//      ����������������������״̬����ȷ����ˣ�����������EntrySleepReCall��
//      ExitSleepReCall����������
//----------------------------------------------------------------------------
u32 LP_EntryLowPower(struct ThreadVm *vm)
{
    atom_low_t  atom_bak;

    if(g_tLowPower.DisableCounter != 0)
    {
        g_tLowPower.EntrySleepReCall(CN_SLEEP_L0);
        __LP_BSP_EntrySleepL0( );
        g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
        return CN_SLEEP_L0;
    }
    switch(g_tLowPower.SleepLevel)
    {
       case CN_SLEEP_L0:
    	   Int_CutTrunk();
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L0);
           __LP_BSP_EntrySleepL0( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L0);
           Int_ContactTrunk();
           return CN_SLEEP_L0;
           break;
       case CN_SLEEP_L1:
    	   Int_CutTrunk();
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L1);
           __LP_BSP_EntrySleepL1( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L1);
           Int_ContactTrunk();
           return CN_SLEEP_L1;
           break;
       case CN_SLEEP_L2:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L2);
           __LP_BSP_EntrySleepL2( );
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L2);
           return CN_SLEEP_L2;
           break;
       case CN_SLEEP_L3:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L3);
           __LP_BSP_SaveSleepLevel(CN_SLEEP_L3);
           atom_bak = Int_LowAtomStart();
           __LP_BSP_AsmSaveReg(vm,__LP_BSP_SaveRamL3,__LP_BSP_EntrySleepL3);
           Int_LowAtomEnd(atom_bak);
           g_tLowPower.ExitSleepReCall(CN_SLEEP_L3);
           return CN_SLEEP_L3;
           break;
       case CN_SLEEP_L4:
           g_tLowPower.EntrySleepReCall(CN_SLEEP_L4);
           __LP_BSP_SaveSleepLevel(CN_SLEEP_L4);
           __LP_BSP_EntrySleepL4( );    //����͹���,���Ҳ��᷵������
           break;

       default:
           break;
    }
    return 0;
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
//����: Level, CN_SLEEP_L0---CN_SLEEP_L4�е�һ��
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



u32 EmptyReCall(u32 SleepLevel)
{
    return 0;
}
//----��ʼ��-------------------------------------------------------------------
//����: ��ʼ���͹������
//����: EntrySleepReCall,����͹���״̬ǰ���õĻص�����
//      ExitSleepReCall, �뿪�͹���״̬ʱ���õĻص�����
//����: ������
//-----------------------------------------------------------------------------
extern u32 (*g_fnEntryLowPower)(struct ThreadVm *vm);  //����͹���״̬�ĺ���ָ�롣
ptu32_t ModuleInstall_LowPower (u32 (*EntrySleepReCall)(u32 SleepLevel),
                                u32 (*ExitSleepReCall)(u32 SleepLevel))
{
    if(EntrySleepReCall != NULL)
        g_tLowPower.EntrySleepReCall = EntrySleepReCall;
    else
        g_tLowPower.EntrySleepReCall = EmptyReCall;
    if(ExitSleepReCall != NULL)
        g_tLowPower.ExitSleepReCall = ExitSleepReCall;
    else
        g_tLowPower.ExitSleepReCall = EmptyReCall;
    g_tLowPower.SleepLevel = CN_SLEEP_NORMAL;
    g_tLowPower.DisableCounter = 0;
    g_fnEntryLowPower = LP_EntryLowPower;
    return 0;
}

