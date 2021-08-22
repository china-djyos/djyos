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
// =============================================================================
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "int_hard.h"
#include "hard-exp.h"
#include "exp_vector.h"
#include "critical.h"
#include "djyos.h"

#include "intc.h"
#include "icu_pub.h"
#include "drv_model_pub.h"

extern void __DJY_ScheduleAsynSignal(void);
void intc_enable(int index);
extern void intc_disable(int index);;

extern struct IntLine *tg_pIntLineTable[];       //�ж��߲��ұ�
//void* intertab[2] __attribute__ ((section(".data.isrtab")));
extern struct IntMasterCtrl  tg_int_global;
typedef void (*inter_dispatch)(ufast_t intStatus);
u32 Int_GetPreStatus(void);

void djy_irq_dispatch(void)
{
    inter_dispatch isr_fun;
    ufast_t intStatus = icu_ctrl(CMD_GET_INTR_STATUS, 0);

//  isr_fun = intertab[0];
    isr_fun = g_u32ExpTable[CN_VECTOR_IRQ];
    isr_fun(intStatus);
}

void djy_fiq_dispatch(void)
{

    inter_dispatch isr_fun;
    ufast_t intStatus = icu_ctrl(CMD_GET_INTR_STATUS, 0);

//  isr_fun = intertab[1];
    isr_fun = g_u32ExpTable[CN_VECTOR_FIQ];
    isr_fun(intStatus);
}

//----ԭ��״̬���--------------------------------------------------------------
//���ܣ����� Int_LowAtomStart ��������ֵ�������ڵ��øú���ǰ�ĵͼ�ԭ�Ӳ���״̬
//������AtomStatus��Int_LowAtomStart ��������ֵ
//���أ�true = ���� Int_LowAtomStart ǰ�Ѿ�����ԭ�Ӳ���״̬��false ��֮
//-----------------------------------------------------------------------------
bool_t Int_IsLowAtom(atom_low_t AtomStatus)
{
    if((AtomStatus & 0xc0) != 0xc0)
        return false;
    else
        return true;
}

//----ԭ��״̬���--------------------------------------------------------------
//���ܣ����� Int_HighAtomStart ��������ֵ�������ڵ��øú���ǰ�ĵͼ�ԭ�Ӳ���״̬
//������AtomStatus��Int_HighAtomStart ��������ֵ
//���أ�true = ���� Int_HighAtomStart ǰ�Ѿ�����ԭ�Ӳ���״̬��false ��֮
//-----------------------------------------------------------------------------
bool_t Int_IsHighAtom(atom_high_t AtomStatus)
{
    if((AtomStatus & 0xc0) != 0xc0)
        return false;
    else
        return true;
}

//----��ͨ�첽�źſ���---------------------------------------------------------
//���ܣ���ͨ�첽�źſ���,����ܿ��ؽ�ͨ���ж��߿��ؽ�ͨ,���жϽ�������
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ؽ�ͨ����
//      2.���û�ж���Ӳ������,���ͨ���б�������첽�źŵ��߿���.
//      3.cm3���ڵ�һ�����,��BASEPRI�Ĵ�����Ϊ0������ȫ���첽�ź�
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactAsynSignal(void)
{
//  portENABLE_FIQ();
//  portENABLE_IRQ();
    portENABLE_INT();
//    *(u32*)0x802044 = 3;
}

//----�Ͽ��첽�źſ���---------------------------------------------------------
//���ܣ��Ͽ��첽�źſ���,���е��첽�źŽ�����ֹ
//      1.���ж�����Ӳ������ʱ,�Ѹÿ��ضϿ�����
//      2.���û�ж���Ӳ������,��Ͽ������첽�źŵ��߿���.
//      3.cm3���ڵ�һ�����,��BASEPRI�Ĵ�����Ϊ�������ֹ�첽�ź�
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutAsynSignal(void)
{
//  portDISABLE_FIQ();
//  portDISABLE_IRQ();
    portDISABLE_INT();
//    *(u32*)0x802044 = 0;
}

//----��ͨ���жϿ���-----------------------------------------------------------
//���ܣ���ͨ���жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_ContactTrunk(void)
{
//  portENABLE_FIQ();
//  portENABLE_IRQ();
    portENABLE_INT();
}

//----�Ͽ����жϿ���---------------------------------------------------------
//���ܣ��Ͽ����жϿ���,����cpu������һ���ܿ���,ֱ�Ӳ����ÿ��ؼ���.
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
void Int_CutTrunk(void)
{
//  portDISABLE_FIQ();
//  portDISABLE_IRQ();
    portDISABLE_INT();
}

//----��ͨ�����ж��߿���-------------------------------------------------------
//���ܣ���ͨ�����ж��߿���,���ж��Ƿ�����Ҫ������Ŀ���״̬
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_ContactLine(ufast_t ufl_line)
{
    intc_enable(ufl_line);
    return true;
}

//----�Ͽ������ж��߿���-------------------------------------------------------
//���ܣ��Ͽ������ж��߿��أ��������жϺ��첽�źſ���״̬��Σ����ж��߱���ֹ
//��������
//���أ���
//��ע����ֲ����
//-----------------------------------------------------------------------------
bool_t Int_CutLine(ufast_t ufl_line)
{
    intc_disable(ufl_line);
    return true;
}

//----�����Ӧ�ж��ߵ��жϹ���״̬---------------------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ��������ǰ��������Ӧͬһ�ж���
//      �ĺ����жϣ�����󣬲ſ�����Ӧ�����жϡ�����������ж��߱�����Ϊʵʱ�ж�
//      �����첽�ź��޹�.
//      cm3�У���Ӧ�ж�ʱ�Զ������״̬�������������ֶ��������״̬
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_ClearLine(ufast_t ufl_line)
{

    if( (ufl_line > CN_INT_LINE_LAST))
        return false;
    UINT32 param = (1<<ufl_line);
    icu_ctrl(CMD_CLR_INTR_STATUS,&param);
    return true;
}

//----�����ж�-----------------------------------------------------------------
//����: ����һ���ж�.����жϱ�������,��������Ӱ��.����������ж��߱�����Ϊʵʱ
//      �жϻ����첽�ź��޹�
//������ufast ufl_line�����������ж��ߺ�
//���أ������Ӧ���ж���Ӳ�����ṩ����������жϹ���,���� false,���򷵻� true
//��ע: ������ʵ��������Ӳ��,��ЩӲ��ϵͳ��֧�ִ˹���.
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_TapLine(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    return false;
}
//----���ȫ���ж��ߵ��жϹ���״̬---------------------------------------------
//���ܣ�Ӳ��Ӧ������Ӧ�Ĺ��ܣ��ṩ����жϹ���Ĳ���
//������ufast ufl_line��ָ��Ӧ����ж��ߺ�
//���أ���
//��ע����Щ��ϵ�ж���ӦʱӲ��Ӧ�𣬱�����Ϊ�պ�����
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
void __Int_ClearAllLine(void)
{

    UINT32 param = (0xffffffff);
    icu_ctrl(CMD_CLR_INTR_STATUS,&param);

}

//----��ѯ�ж�������״̬-------------------------------------------------------
//���ܣ���ѯ�������Ӧ�ж���״̬�������ڲ�ѯʽ�жϳ���
//������ufl_line������ѯ���ж���
//���أ����жϹ��𣬷���true�����򷵻�false
//��ע: ��Ӳ���ṹ���,��Щ�ṹ���ܲ��ṩ�������,����!
//      ����������ֲ���к���
//-----------------------------------------------------------------------------
bool_t Int_QueryLine(ufast_t ufl_line)
{
    UINT32 param ;
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    param =  icu_ctrl(CMD_CLR_INTR_STATUS,&param);
    if(param & (1<<ufl_line))
        return true;
    return false;
}

//----��ָ���ж�������Ϊ�첽�ź�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊ�첽�ź�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoAsynSignal(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    tg_pIntLineTable[ufl_line]->int_type = CN_ASYN_SIGNAL;   //�ж�������

    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
                    &= ~(1<<(ufl_line % CN_CPU_BITS));   //����λͼ
    return true;
}

//----��ָ���ж�������Ϊʵʱ�ж�--------������---------------------------------
//���ܣ���ָ���ж�������Ϊʵʱ�ж�,���ж�������Ӧ,��ǰ�жϷ��غ���Ч
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ�true=�ɹ���false=ʧ��
//      ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SettoReal(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    if(tg_pIntLineTable[ufl_line]->sync_event != NULL)
        return false;     //���߳��ڵȴ�����жϣ�������Ϊʵʱ�ж�
    tg_pIntLineTable[ufl_line]->int_type = CN_REAL;    //�ж�������
    tg_pIntLineTable[ufl_line]->enable_nest = false;   //��ʵ�ֲ�֧��ʵʱ�ж�Ƕ��
    tg_int_global.property_bitmap[ufl_line/CN_CPU_BITS]
            |= 1<<(ufl_line % CN_CPU_BITS);   //����λͼ
    return true;
}

//----ʹ���ж�Ƕ��-------------------------------------------------------------
//����: ʹ��һ���ж�Դ����ռ��ʹ�ܺ���Ӧ���жϷ����ڼ䣬���ܻᱻ����ж���ռ��
//      ����������ʵʱ�жϻ����첽�źţ�������ʵ���йأ���ֲ��Ӧ�ø���Ӳ������
//      ��Ӧ������ȷ���Ƿ�����Ƕ�ס�
//      Ƕ�׷�����ʵʱ�ж�֮������첽�ź�֮�䡣ʵʱ�ж���Զ���Դ���첽�źţ���
//      ����Ƕ�ס�
//      ��������ϵ�е�ARMоƬ�жϹ��������ص㣬����ʵʱ�жϵĴ��������ַ�ʽ:
//      1����fiqʵ��ʵʱ�жϣ�INTOFFSET1��INTOFFSET2�Ĵ����ṩ�˵�ǰ���ڷ����
//         �жϺţ����������Ĵ�����fiq����Ч�����ʵʱ�ж���fiqʵ�֣�����Ҫ��λ
//         ��鱻��Ϊʵʱ�жϵ��жϺ���INTPND1��INTPND1�Ĵ����е�ֵ������ȷ����
//         ������жϺţ�������̿��ܷǳ���������������ܹ�ʵ��ʵʱ�ж�Ƕ�ף���
//         ȴ��ȫʧȥ��ʵʱ�жϵ���Ƴ��ԣ�djyos for s3c2416����ȡ���ַ�ʽ��
//      2����һ����������ǣ�ϵͳֻ��Ҫһ��ʵʱ�жϣ�����ȫ����Ϊ�첽�źţ�����
//         �������fiqʵ��ʵʱ�ж�����Ϊ����ģ���Ȼֻ��һ���жϣ�Ƕ��Ҳ���޴�
//         ̸���ˡ�
//      3��ʵʱ�жϺ��첽�źŶ�����irq��ʽʵ�֣�cpsr��Iλ����trunk���أ��첽�ź�
//         û�ж������أ���ͨ�������е������첽�źŵ��жϺŵĿ���ȫ���ص���ģ��
//         �첽�źſ��صġ���������ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�Ϊʲô��?arm�����
//         Ϊ��ֻҪIλΪ0���µ��жϾͿ�����ռ���ڷ�����жϣ�Ҳ����ζ�ţ��첽��
//         ����ռʵʱ�жϣ���Ӳ����������ġ�ʵʱ�ж�Ҫʵ��Ƕ�ף����ȹص��첽��
//         �ţ�������I=0�����ʵʱ�ж���Ӧ�����첽�źű��ص�֮ǰ�����������첽
//         �źŷ����Ļ�����irq����ͻᷢ�������ص��첽�ź�Ҳû�ã����ж�һ����
//         �������첽�źžͻ���ռʵʱ�жϡ�������ռ��������"����"���ң�������
//         ���������⣬����ʹϵͳ�����������ռʵʱ�жϵ��첽�źţ������ڼ䷢��
//         ���߳��л�����fiq�����ջ������pg_event_running��ջ����ˣ�djyos for
//         2416�汾��֧��ʵʱ�ж�Ƕ�ס�
//         cortex-m3�У��첽�ź�ȫ������Ϊ������ȼ�������cm3���ж�Ƕ�׻��ƣ�������
//         ����ʵ��Ƕ�׵ġ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
bool_t Int_EnableNest(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return false;
    return true;
}

//----��ֹ�ж�Ƕ��-------------------------------------------------------------
//����: ���ĳ�жϺŵ��ж�Ƕ��ʹ�ܡ�
//����: ufl_line�����������ж���
//����: ��
//-----------------------------------------------------------------------------
void Int_DisableNest(ufast_t ufl_line)
{
    if( (ufl_line > CN_INT_LINE_LAST)
            || (tg_pIntLineTable[ufl_line] == NULL) )
        return ;
    return ;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

//----�趨��ռ���ȼ�-----------------------------------------------------------
//����: �趨ָ���ж��ߵ�Ƕ�����ȼ�����������������Ӳ�����ܡ����Ӳ����֧�֣��ɷ���false
//������ufast ufl_line��ָ�������õ��ж��ߺ�
//���أ���
//ע: ��������ֲ����
//-----------------------------------------------------------------------------
bool_t Int_SetPrio(ufast_t ufl_line,u32 prio)
{
    return false;
}

#pragma GCC diagnostic pop

//----���ж�����---------------------------------------------------------------
//���ܣ���Щϵͳ�����ж�����������������ʵʱ�жϻ����첽�źŵģ����߲�ϣ���ڻ�
//      ��׶�ʹ�ù��ڸ��ӵĴ���ģ�����2416��2440�ȣ����ڻ��׶λ�ȡ�жϺź�
//      ֱ�ӵ��ñ��������ɱ���������������첽�ź������ʵʱ�ж����档
//      ��cortex-m3��omapl138���������ж��������־Ϳ�������ʵʱ�жϻ����첽��
//      �ţ��������ṩ�������������ڻ�ಿ��ֱ�ӵ�����Ӧ�����档
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
#define CN_CPU_MODE_USR    0x10
#define CN_CPU_MODE_SVC    0x13
#define CN_CPU_MODE_SYS    0x1F
void __irq_Int_EngineAll(ufast_t intStatus)
{
    ufast_t ufl_line = 0;
    u32 CpuStatus;
    intStatus &=0x0000ffff;
    for(;(intStatus!=0)&&(ufl_line<=16);ufl_line++)
    {
      if(intStatus & (1<<ufl_line))
      {
          if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
              __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
          else
          {
              __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
//            if(g_ptEventReady != g_ptEventRunning)
//            {
//                __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
//            }
          }
          intStatus &=~(1<<ufl_line);
      }
    }
    if(g_ptEventReady != g_ptEventRunning)
    {
        CpuStatus = Int_GetPreStatus();
        if((CpuStatus == CN_CPU_MODE_USR) || (CpuStatus == CN_CPU_MODE_SYS)
                                          || (CpuStatus == CN_CPU_MODE_SVC))
            __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
    }
}

void __fiq_Int_EngineAll(ufast_t intStatus)
{
    ufast_t ufl_line = 16;
    u32 CpuStatus;
    intStatus &=0xffff0000;

    for(;(intStatus!=0)&&(ufl_line<=31);ufl_line++)
    {
      if(intStatus & (1<<ufl_line))
      {
          if(tg_pIntLineTable[ufl_line]->int_type == CN_REAL)
              __Int_EngineReal(ufl_line);                //��ʵʱ�ж�
          else
              __Int_EngineAsynSignal(ufl_line);         //���첽�ź�
          intStatus &=~(1<<ufl_line);
      }
    }
    if(g_ptEventReady != g_ptEventRunning)
    {
        CpuStatus = Int_GetPreStatus();
        if((CpuStatus == CN_CPU_MODE_USR) || (CpuStatus == CN_CPU_MODE_SYS)
                                          || (CpuStatus == CN_CPU_MODE_SVC))
            __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
    }
}
void __Int_InitHard()
{
    /*
     * ʧ�������ж�
     * */
    void Disable_AllPeri();
    Disable_AllPeri();

//  intertab[0] = _irq_Int_EngineAll;
//  intertab[1] = _fiq_Int_EngineAll;
//    g_u32ExpTable[CN_VECTOR_IRQ] = __irq_Int_EngineAll;
//    g_u32ExpTable[CN_VECTOR_FIQ] = __fiq_Int_EngineAll;
}

//----��ʼ���ж�---------------------------------------------------------------
//���ܣ���ʼ���ж�Ӳ��,��ʼ���ж������ݽṹ
//      2.�첽�źű��ֽ�ֹ,��������������������д�.
//      3.���ж�����
//      �û���ʼ������Ӧ���������¹���:
//      1.ϵͳ��ʼʱ���Ѿ���ֹ�����첽�ź�,�û���ʼ��ʱ���뵣���첽�źŷ���.
//      2.��ʼ�������������Ҫ�������ж�/ʵʱ�ж�/�첽�ź�,Ӧ�óɶ�ʹ��.��ֹʹ
//        �첽�ź�ʵ�ʴ�������״̬(���첽�����жϿ���ͬʱ����).
//      3.���Բ����ж���,�������ӡ�������ֹ��,��Ӧ�óɶ�ʹ��.
//      4.����ʹ��save/restore������,��Ҫʹ��enable/disable������.
//��������
//���أ���
//-----------------------------------------------------------------------------
extern uint32_t djy_switch_interrupt_flag;
extern uint32_t *djy_interrupt_from_thread;
extern uint32_t *djy_interrupt_to_thread;
void Int_Init(void)
{
    ufast_t ufl_line;

    djy_switch_interrupt_flag = 0;
    djy_interrupt_from_thread = NULL;
    djy_interrupt_to_thread = NULL;
    __Int_InitHard();
    __Int_ClearAllLine();
    for(ufl_line=0;ufl_line <= CN_INT_LINE_LAST;ufl_line++)
    {
        tg_pIntLineTable[ufl_line] = NULL;
    }

    for(ufl_line=0; ufl_line < CN_INT_BITS_WORDS; ufl_line++)
    {
        //����λͼ����,ȫ����Ϊ�첽�źŷ�ʽ
        tg_int_global.property_bitmap[ufl_line] = 0;
        //�ж�ʹ��λͼ��0,ȫ�����ڽ�ֹ״̬
        tg_int_global.enable_bitmap[ufl_line] = 0;
    }
    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.nest_real = 0;
//    tg_int_global.en_asyn_signal = false;
    tg_int_global.en_asyn_signal_counter = 1;   //�첽�źż���
    Int_CutAsynSignal();
//    tg_int_global.en_trunk = true;
    tg_int_global.en_trunk_counter = 0;   //���жϼ���
    //Int_ContactTrunk();                //��ͨ���жϿ���
}




//----ʵʱ�ж�����-------------------------------------------------------------
//���ܣ���Ӧʵʱ�жϣ������жϺŵ����û�ISR
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
void __Int_EngineReal(ufast_t ufl_line)
{
    struct IntLine *ptIntLine;
    tg_int_global.nest_real++;
    ptIntLine = tg_pIntLineTable[ufl_line];

    //��if�������ֲ���У���cpu���жϹ������ļ��������й�:
    //1���첽�ź��Ƿ��ж����Ŀ��أ�����cortex-m3��omapl138�����еģ�2440��2416��
    //   2410����û�еġ����û�ж������أ����ڴ����ж�ǰ��ϵ��첽�ź��߿���
    //2���첽�źź�ʵʱ�ж϶����ڿ���״̬������£��첽�ź��Ƿ������ռʵʱ�жϡ�
    //   �������ʵ�����ǲ���ʵ��ʵʱ�ж�Ƕ�׵ġ�
    //3��ʵʱ�ж���Ӧ���Ƿ��Զ��ر�ʵʱ�ж�
    //4���þ���ʵ���Ƿ�֧��ʵʱ�ж�Ƕ��
    //5����ʵ�ֲ�֧��ʵʱ�ж�Ƕ�ף�������ע�͵�
    if(ptIntLine->ISR != NULL)
        ptIntLine->ISR(ptIntLine->para);  //�����û��жϺ���


    //�����ʵ��֧��ʵʱ�ж�Ƕ�ף���������4��
    tg_int_global.nest_real--;

}
#define CN_CPU_MODE_USR    0x10
#define CN_CPU_MODE_SVC    0x13
#define CN_CPU_MODE_SYS    0x1F
//----�첽�¼��ж�����---------------------------------------------------------
//���ܣ���Ӧ�첽�źţ������жϺŵ����û�ISR����󵯳��ж��߿��ƿ��my_evtt_id
//      ��Աָ�����¼����ͣ�����ڷ���ǰ�鿴�Ƿ���Ҫ���������л�������Ҫ���л�
//      ֮��
//������ufast ufl_line����Ӧ���ж��ߺ�
//���أ���
//-----------------------------------------------------------------------------
extern void __DJY_EventReady(struct EventECB *event_ready);
void __Int_EngineAsynSignal(ufast_t ufl_line)
{
    struct EventECB *event;
    struct IntLine *ptIntLine;
    u32 isr_result;

    g_bScheduleEnable = false;
    tg_int_global.nest_asyn_signal++;
    ptIntLine = tg_pIntLineTable[ufl_line];
    if(ptIntLine->clear_type == CN_INT_CLEAR_AUTO)
         Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    if(ptIntLine->ISR != NULL)
    {
        isr_result = ptIntLine->ISR(ptIntLine->para);

    }
    else
    {
        if(ptIntLine->clear_type == CN_INT_CLEAR_USER)
            Int_ClearLine(ufl_line);        //�ж�Ӧ��,
    }
    event = ptIntLine->sync_event;
    if(event != NULL)   //��ͬ��ָ������û���¼�(ע�������¼������Ƕ���)
    {
        event->event_result = isr_result;
        __DJY_EventReady(event);   //�Ѹ��¼��ŵ�ready����
        ptIntLine->sync_event = NULL;   //���ͬ��
    }
    if(ptIntLine->my_evtt_id != CN_EVTT_ID_INVALID)
    {
        DJY_EventPop(ptIntLine->my_evtt_id,
                        NULL,0,(ptu32_t)isr_result, (ptu32_t)ufl_line,0);
    }
    tg_int_global.nest_asyn_signal--;

    g_bScheduleEnable = true;
}


