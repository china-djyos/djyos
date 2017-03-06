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
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.1.0
//�ļ�����:�¼����͡��¼������Լ����¼�������صĴ���ȫ�������ˡ�
//����˵��:
//�޶���ʷ:
//8. ����:2013-2-7
//   ����:  ������.
//   �°汾�ţ�1.3.3
//   �޸�˵��: ������djy_event_exit������һ�����󣬸�bug�������ύ
//7. ����:2013-1-18
//   ����:  ������.
//   �°汾�ţ�1.3.2
//   �޸�˵��: ������djy_event_pop������һ�����󣬸�bug��QQȺ��
//              "ring wang(23424710)"�ύ
//6. ����:2012-12-18
//   ����:  ������.
//   �°汾�ţ�1.3.1
//   �޸�˵��: ������djy_event_complete��djy_wait_event_completed������һ������
//      ��bug������"��ë��"�ύ
//5. ����: 2011-07-19
//   ����:  ������.
//   �°汾��: V1.3.0
//   �޸�˵��:���һ�����ܵ����ڴ�й©��bug����bug���������������ߴ����32�ֽڣ�
//   ���ڵ���djy_event_popʱҪ��̬������������ڴ�ռ䣬djy_task_completed��
//   djy_event_exit�����ͷŲ���ʱ�����ͷŸö�̬������ڴ档
//   �޸ķ�������struct  para_record�ṹ�����ӳ�Աdynamic_mem�����ڱ�ʾ�����Ƿ�
//   ��̬����õ��ģ����ǣ�����djy_task_completed��djy_event_exit�����ͷŲ���ʱ
//   ����m_free�ͷ�֮��
//4. ����: 2009-12-15
//   ����:  ������.
//   �°汾��: V1.3.0
//   �޸�˵��:
//   1��djy_evtt_regist���ж�ͬ���¼�����ʱ�����۸��¼����Ϳ��ƿ���Ч�񣬶�����
//      strcmp�����Ƚϣ�Ч�ʽϵͣ���Ϊ���ж��Ƿ�գ�Ȼ��űȽϡ�
//   2��ԭ����djy_evtt_regist�����ܴ����̵߳��µǼ�ʧ��ʱ��û�д����¼�����
//      ���ƿ飬������Ȼ����registered=1��״̬��
//3. ����: 2009-10-05
//   ����:  ������.
//   �°汾��: V1.2.0
//   �޸�˵��:
//   1��������tg_int_global.nest_asyn_signal==0��Ϊ�Ƿ�������ȵ����ݣ���ᵼ��
//      �ж�ģ��͵���ģ�����������Ϲ�ϵ��
//      ��ȫ�ֱ���bg_schedule_enable��Ϊ�Ƿ�������ȵı�־
//   2���ĵ���һЩgcc���Լ�c99����֧�ֵ�����������Ӧ���㷺�ı�����
//   3��ǰ��İ汾����ʵ��djy_evtt_unregist��ȫ�����ܣ����汾��ȫ֮��
//   4��__djy_resume_delay��������һ�У�
//      g_ptEventDelay = g_ptEventDelay->previous;��Ϊ
//      g_ptEventDelay = g_ptEventDelay->next;
//      ����ᵼ������ͬ�����д���
//   5��ԭ�汾��,__turnto_context�������2ְ���ں˳�ʼ����ɺ��״ε��ñ�������
//      �������̵߳��ȵĹ��ܣ�����arm���ǿ��Եģ�����ȷ������cpuҲ���ԣ�����cm3
//      �ϾͲ��У��Ȱ�������ܷ������������һ������__asm_start_thread������arm
//      �汾���ú궨�壺#define __asm_start_thread __asm_turnto_context
//   6��int_restore_asyn_signal�����У���ִ��__int_contact_asyn_signal���жϣ�
//      Ȼ����ִ�У�
//            if(g_ptEventRunning != g_ptEventReady)
//                __Djy_Schedule();
//      �ڴ��ж���__djy_schedule�������¹ر��ж�֮�䣬������ж���ռ�Ļ�������
//      �������ش���int_restore_trunk����Ҳ��ͬ�������⡣
//      �°汾�У�__djy_schedule��������__asm_switch_contextʱ�Ŵ��жϣ��ž���
//      �������⣬__djy_schedule�������������ڹر��жϵ�������ִ�еġ�
//2. ����: 2009-03-18
//   ����:  ������.
//   �°汾��: V1.1.0
//   �޸�˵��:
//   1��������ΪСɮ�����������ֵ��������¼����͵����������ָ�������Ļ������
//      �û�����y_evtt_registʱʹ�õ��Ǿֲ����飬�����ڱ������ֵ��ڴ汻�ͷŵ�Σ
//      �ա���Ϊ����洢���31�ַ�����struct event_type�ṹ�а�evtt_name��ָ��
//      ��Ϊ���飬�޸��漰���к�����
//          __djy_init_sys��y_evtt_regist��y_evtt_unregist��y_get_evtt_id
//   2����y_evtt_unregist�������ͷ��̴߳�����һ��bug�����ڷ��ʷǷ��ڴ��Σ�գ�
//      ��֮��
//   3��y_evtt_done_sync�����д���ͬ�����д���������bug�����ܵ��·��ʷǷ��ڴ棬
//      ��ʹͬ�����л��ң���֮��
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "board-config.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "cpu.h"
#include "object.h"
#include "lock.h"
#include "int.h"
#include "pool.h"
#include "systime.h"
#include "djyos.h"
#include "exp.h"
#include "core_config.h"

ptu32_t __Djy_Service(void);

struct ProcessVm *  g_ptMyProcess;
//Ϊcn_events_limit���¼����ƿ�����ڴ�
struct EventECB *g_ptECB_Table;
//static struct ParaPCB  s_tEventParaTable[CN_CFG_PARAS_LIMIT];
//Ϊcn_evtts_limit���¼����Ϳ��ƿ�����ڴ�
struct EventType *g_ptEvttTable;
struct EventECB  *s_ptEventFree; //��������ͷ,������
//static struct ParaPCB  *s_ptParaFree; //��������ͷ,������
//��ת����ʱ��Ƭ��0��ʾ��ֹ��ת���ȣ�Ĭ��1��RRS = "round robin scheduling"��д��
static u32  s_u32RRS_Slice = 1;

struct EventECB  *g_ptEventReady;      //��������ͷ
struct EventECB  *g_ptEventRunning;    //��ǰ����ִ�е��¼�
struct EventECB  *g_ptEventDelay;      //����ͬ�����б�ͷ
s64 g_s64RunningStartTime;        //��ǰ�������¼��Ŀ�ʼִ��ʱ��.
s64  g_s64OsTicks;            //����ϵͳ����ticks
bool_t g_bScheduleEnable;     //ϵͳ��ǰ����״̬�Ƿ������
bool_t g_bMultiEventStarted = false;    //���¼�(�߳�)�����Ƿ��Ѿ���ʼ
extern u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024

u32 (*g_fnEntryLowPower)(struct ThreadVm *vm) = NULL;  //����͹���״̬�ĺ���ָ�롣
void __Djy_SelectEventToRun(void);
void __Djy_EventReady(struct EventECB *event_ready);
void __Djy_ResumeDelay(struct EventECB *delay_event);
extern void __DjyInitTick(void);
extern void __M_WaitMemory(struct EventECB *event);
extern void __M_CleanUp(uint16_t event_id);
extern struct ThreadVm *__CreateThread(struct EventType *evtt,u32 *stack_size);
extern void __asm_reset_switch(ptu32_t (*thread_routine)(void),
                               struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_turnto_context(struct ThreadVm  *new_vm);
extern void __asm_start_thread(struct ThreadVm  *new_vm);
extern void __asm_switch_context_int(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __asm_switch_context(struct ThreadVm *new_vm,struct ThreadVm *old_vm);
extern void __Int_ResetAsynSignal(void);


//----΢�뼶��ʱ-------------------------------------------------------------
//���ܣ�����ѭ��ʵ�ֵ�΢��ֱ�����ʱ������__DjySetDelay���������ʹ�ñ�������
//      �����ڲ�ͬ�Ż�����Ͳ�ͬ��������,��ʱ����ͬ.
//������time����ʱʱ�䣬��λΪ΢��
//���أ���
//ע�⣺������ʹ�ô˺�����̫����ʱ������ʱ��ʹ�ú��� Djy_EventDelay,
//-----------------------------------------------------------------------------
void Djy_DelayUs(u32 time)
{
    volatile u32 i;

    i = (time << 10) / g_u32CycleSpeed;
    for(; i >0 ; i--);
}

//----�߳�ջ���---------------------------------------------------------------
//����: ���һ���¼����߳��Ƿ���ջ������գ�����:���ջ���1/16�ռ��ڣ������Ƿ�
//      �ı䣬���ı䣬����ܷ�����ջ�����
//����: event_id,�������¼�id
//����: true=no overflow,false = overflow
//-----------------------------------------------------------------------------
bool_t __Djy_CheckStack(s16 event_id)
{
    struct ExpThrowPara  parahead;
    char StackExp[128];
    u32 loop;
    volatile ptu32_t level;
    u32 *stack;
    u32 pads;
    bool_t result=true;

    pads = 0x64646464;
    if(g_ptECB_Table[event_id].vm == NULL)
        return true;
    if(g_ptECB_Table[event_id].previous ==
                    (struct EventECB*)&s_ptEventFree)
        return true;

    level = g_ptECB_Table[event_id].vm->stack_size>>6;

    if((ptu32_t)(g_ptECB_Table[event_id].vm->stack - (u32*)(g_ptECB_Table[event_id].vm))
                                                            < level)
    {
        result = false;   //ջָ���Ѿ����ڰ�ȫֵ�����������
    }
    else
    {
        stack = (u32*)(&(g_ptECB_Table[event_id].vm[1]));
        for(loop = 0; loop < level; loop++)
        {
            if(stack[loop] != pads)
            {
                result = false;   //��ȫ���ڷ����˸ı䣬���������
                break;
            }
        }
    }
    if(result == false)
    {
        parahead.DecoderName = NULL;
        parahead.ExpAction = EN_EXP_DEAL_RESTART;
        sprintf(StackExp,"ջ�������,ջ��:0x%x,ջ�ߴ�:0x%x,ʣ��:0x%x,�¼���:%d,���ͺ�%d,������:%s",
                            (ptu32_t)stack,g_ptECB_Table[event_id].vm->stack_size,loop,
                            event_id, g_ptECB_Table[event_id].evtt_id&(~CN_EVTT_ID_MASK),
                            g_ptEvttTable[g_ptECB_Table[event_id].evtt_id&(~CN_EVTT_ID_MASK)].evtt_name);
        parahead.ExpInfo = (u8*)StackExp;
        parahead.ExpInfoLen = sizeof(StackExp);
        parahead.ExpType = CN_EXP_TYPE_STACK_OVER;
        printf("%s\n\r",StackExp);
        Exp_Throw(&parahead);
    }
    return result;
}

//����Ϊ�����������

//----tick�ж�-----------------------------------------------------------------
//���ܣ�Ϊ����ϵͳ����ʱ��Դ�����Ƚ�����ͬ�������е��¼�����ʱ�䣬���������ʱ�䵽��
//      �¼����Ͱ����Ǽ���¼����������С����ʱ��Ƭ��ת���ȱ�������Ҫ���Ƿ�Ҫ
//      ִ����ת��ticks������Ϊ�첽�źš�
//������inc_ticks��ticks�жϵ��ж��ߺţ�ʵ���ϲ��á�
//���أ���
//-----------------------------------------------------------------------------
void __DjyMaintainSysTime(void);
void  Djy_IsrTick(u32 inc_ticks)
{
    struct EventECB *pl_ecb,*pl_ecbp,*pl_ecbn;
    g_s64OsTicks += (s64)inc_ticks;    //ϵͳʱ��,Ĭ���������
    //����ά��ϵͳʱ����ת��ʹ��ϵͳʱ��ļ����С��Ӳ����ʱ��ѭ�����ڡ�
    __DjyMaintainSysTime( );
    if(g_ptEventDelay != NULL)
    {
        pl_ecb = g_ptEventDelay;
        while(1)
        {
            if(pl_ecb->delay_end_tick <= g_s64OsTicks) //Ĭ��64λticks�������
            {
                //�¼���ĳͬ�������У�Ӧ�ôӸö���ȡ��
                if(pl_ecb->sync_head != NULL)
                {
                    if(*(pl_ecb->sync_head) == pl_ecb)//���¼�λ�ڶ���ͷ��
                    {
                        if(pl_ecb->multi_next == pl_ecb)//�ö�����ֻ��һ���¼�
                        {
                            *(pl_ecb->sync_head) = NULL;

                            pl_ecb->multi_next = NULL;
                            pl_ecb->multi_previous = NULL;
                        }else                   //��ͬ���������ж���¼�
                        {
                            //ͷָ��ָ����һ���¼�
                            *pl_ecb->sync_head = pl_ecb->multi_next;
                            pl_ecb->multi_previous->multi_next
                                                = pl_ecb->multi_next;
                            pl_ecb->multi_next->multi_previous
                                                = pl_ecb->multi_previous;
                        }
                    }else           //���¼����Ƕ���ͷ
                    {
                        pl_ecb->multi_previous->multi_next
                                            = pl_ecb->multi_next;
                        pl_ecb->multi_next->multi_previous
                                            = pl_ecb->multi_previous;
                    }
                    pl_ecb->sync_head = NULL;   //�¼�ͷָ���ÿ�
                }

                pl_ecb->wakeup_from = pl_ecb->event_status;
                pl_ecb->event_status = CN_STS_EVENT_READY;
                if(pl_ecb->next == pl_ecb)      //��������ͬ���������һ�����.
                {
                    g_ptEventDelay = NULL;
                    __Djy_EventReady(pl_ecb);
                    break;
                }else
                {
                    g_ptEventDelay = pl_ecb->next;
                    pl_ecb->next->previous = pl_ecb->previous;
                    pl_ecb->previous->next = pl_ecb->next;
                    __Djy_EventReady(pl_ecb);
                    pl_ecb = g_ptEventDelay;
                }
            }else
                break;
        }
    }

    //���洦��ʱ��Ƭ��ת����.
    //���ڿ��첽�ź�(�������)�ſ��ܽ���__djy_isr_tick����ʹ�������쵼�����¼���
    //�룬pg_event_runningҲ�ض������ȼ����������У������ܲ�����pg_event_ready
    if(s_u32RRS_Slice != 0)      //������ת����
    {
        if( (g_ptEventRunning->prio == g_ptEventRunning->next->prio)
                    &&(g_ptEventRunning != g_ptEventRunning->next) )
        {//�����ȼ��ж���¼�������תʱ���Ƿ�
            if((u32)g_s64OsTicks % s_u32RRS_Slice == 0) //ʱ��Ƭ����
            {
                //�ȴ������ȼ��������У���pg_event_running�Ӷ�����ȡ������֮��
                //g_ptEventRunning->next��
                pl_ecb = g_ptEventRunning->next;
                pl_ecbn = g_ptEventRunning->multi_next;
                pl_ecbp = g_ptEventRunning->multi_previous;
                pl_ecbp->multi_next = pl_ecb;
                pl_ecb->multi_previous = pl_ecbp;
                pl_ecbn->multi_previous = pl_ecb;
                pl_ecb->multi_next = pl_ecbn;
                g_ptEventRunning->multi_next = NULL;
                g_ptEventRunning->multi_previous = NULL;

                //�ٴ����������,��pg_event_running�Ӿ����������ó�����Ȼ�����
                //�ŵ�ͬ���ȼ������

                //���û�и����ȼ��¼���ͬ��ʱ�䵽������ready���У�����������
                if(g_ptEventReady == g_ptEventRunning)
                    g_ptEventReady = pl_ecb;
//                pl_ecbn = g_ptEventReady->multi_next;
//                pl_ecbp = g_ptEventRunning->previous;
                //�����а�pg_event_running�Ӷ�����ȡ��
                g_ptEventRunning->previous->next = pl_ecb;
                pl_ecb->previous = g_ptEventRunning->previous;
                //�����а�pg_event_running����Ŀ��λ��
                g_ptEventRunning->previous = pl_ecbn->previous;
                g_ptEventRunning->next = pl_ecbn;
                pl_ecbn->previous->next = g_ptEventRunning;
                pl_ecbn->previous = g_ptEventRunning;
            }
        }
    }
    return;
}

void Djy_SaveLastError(u32 ErrorCode)
{
    g_ptEventRunning->error_no = ErrorCode;
}

//----��ȡ���һ��������Ϣ-----------------------------------------------------
//���ܣ�����
//��������
//���أ������
//��������ı�����ʱ��Ĺ��ܣ�ʱ��ʹ��ticks��---db
//-----------------------------------------------------------------------------
u32 Djy_GetLastError(void)
{
    return g_ptEventRunning->error_no;
}

u32 *DjyCurrentErrnoAddr(void)
{
	static u32 err;
	if(NULL == g_ptEventRunning)
	{
		return &err;
	}
	else
	{
		return &g_ptEventRunning->error_no;
	}
}


//----��ready������ȡ��һ���¼�------------------------------------------------
//����: ��һ���¼���ready������ȡ������ʹready������������
//����: event����ȡ�����¼�ָ��
//����: ��
//ע: �������뱣֤���첽�ź�(����)����ֹ������µ��ñ�����
//-----------------------------------------------------------------------------
void __Djy_CutReadyEvent(struct EventECB *event)
{
    struct EventECB *pl_ecb;
    if(event != g_ptEventReady)         //event����ready����ͷ
    {
        if(event->multi_next == NULL)   //�������ȼ�����������
        {
            event->next->previous = event->previous;
            event->previous->next = event->next;
        }else                           //�����ȼ�����������
        {
            pl_ecb = event->next;
            event->next->previous = event->previous;
            event->previous->next = event->next;
            if(pl_ecb->prio == event->prio)     //��Ӧ���ȼ���ֹһ���¼�
            {
                event->multi_previous->multi_next = pl_ecb;
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_next->multi_previous = pl_ecb;
                pl_ecb->multi_next = event->multi_next;
            }else                               //��Ӧ���ȼ�ֻ��һ���¼�
            {
                //pl_ecb��event->multi_next.
                pl_ecb->multi_previous = event->multi_previous;
                event->multi_previous->multi_next = pl_ecb;
            }
        }
    }else                               //event��ready����ͷ
    {
        g_ptEventReady = event->next;
        pl_ecb = event->next;
        event->next->previous = event->previous;
        event->previous->next = event->next;
        if(pl_ecb->prio == event->prio)     //��Ӧ���ȼ���ֹһ���¼�
        {
            event->multi_previous->multi_next = pl_ecb;
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_next->multi_previous = pl_ecb;
            pl_ecb->multi_next = event->multi_next;
        }else                               //��Ӧ���ȼ�ֻ��һ���¼�
        {
            //pl_ecb��event->multi_next.
            pl_ecb->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = pl_ecb;
        }
    }
}

//----������ת����ʱ��Ƭ-------------------------------------------------------
//����: djyos�����в�����ת���ȵ��¼�ʱ��Ƭ������ͬ�ģ����������á������Ϊ0��
//      ��ʾ������ʱ��Ƭ��ת��
//����: slices���µ���ת����ʱ��Ƭ��΢��������������ȡ��Ϊ������ticksʱ��
//����: ��
//-----------------------------------------------------------------------------
void Djy_SetRRS_Slice(u32 slices)
{
#if (32 > CN_CPU_BITS)
    atom_low_t atom_low;
    //���������ֳ���32λ,��Ҫ������ڲ��ܸ���u32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low = Int_LowAtomStart( );   //�������Բ���Ƕ�׵���
#endif

    s_u32RRS_Slice =(slices + CN_CFG_TICK_US -1)/CN_CFG_TICK_US;

#if (32 > CN_CPU_BITS)
    //���������ֳ���32λ,��Ҫ������ڲ��ܸ���u32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    Int_LowAtomEnd( atom_low );
#endif
}

//----��ѯ��ת����ʱ��Ƭ-------------------------------------------------------
//����: djyos�����в�����ת���ȵ��¼�ʱ��Ƭ������ͬ�ģ����ñ�������ѯ��
//����: ��
//����: ��ǰʱ��Ƭ���ȣ�΢������
//-----------------------------------------------------------------------------
u32 Djy_GetRRS_Slice(void)
{
    u32 temp;
#if (32 > CN_CPU_BITS)
    atom_low_t atom_low;
    //�������ֳ���32λ,��Ҫ������ڲ��ܶ�ȡu32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    atom_low = Int_LowAtomStart( );   //�������Բ���Ƕ�׵���
#endif

    temp = s_u32RRS_Slice;

#if (32 > CN_CPU_BITS)
    //�������ֳ���32λ,��Ҫ������ڲ��ܶ�ȡu32g_RRS_slice,�ù��̲��ܱ�ʱ���жϴ��.
    Int_LowAtomEnd( atom_low );
#endif
    return temp * CN_CFG_TICK_US;
}

//----���ҿ������߳�----------------------------------------------------------
//����: �� pg_event_ready�����л��һ���������е��߳�.
//      1.��pg_event_ready����ͷ��ʼ����,������¼���û�������߳�,����ͼ����֮.
//      2.������ܽ���,�϶������ڴ治�㣬��Ѹ��¼��ŵ��ڴ�ȴ�����,�������²���.
//      3.��˷���,ֱ���ҵ�һ���������е��߳�.Ȼ���pg_event_readyָ��ָ����¼�
//����: ��
//����: ��
//��ע: �������ɲ���ϵͳ����,����ǰ��֤���첽�źš�
//      ����ϵͳ�����¼�����ready,�����������ҵ��������е��߳�.
//----------------------------------------------------------------------------
void __Djy_SelectEventToRun(void)
{
    struct EventECB *pl_ecb,*temp_var;
   // struct EventType *pl_evtt;  //���������¼�������ָ��
    struct EventType *pl_evtt;
    while(g_ptEventReady->vm == NULL)
    {
        pl_evtt =& g_ptEvttTable[g_ptEventReady->evtt_id &(~CN_EVTT_ID_MASK)];
        if(pl_evtt->my_free_vm != NULL)     //���¼������п��е��߳�,ֱ��ʹ��
        {
            g_ptEventReady->vm = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = pl_evtt->my_free_vm->next;
        }else       //���¼�����û�п��е��߳�,��ͼ����֮
        {
            //�����߳�
            g_ptEventReady->vm =
                    __CreateThread(pl_evtt,&(g_ptEventReady->wait_mem_size));
            if(g_ptEventReady->vm == NULL)                  //�����߳�ʧ��
            {
                pl_ecb = g_ptEventReady;
                __Djy_CutReadyEvent(pl_ecb);
#if (CN_CFG_DYNAMIC_MEM == 1)
                __M_WaitMemory(pl_ecb);//�õĽ����Ƕ๦��ָ��
#endif
                //��ECB�Ž�EVTT��markedevent�����У�ע��ֻ�Ƕ����¼��Ż�
                //��if���������������
                if(pl_evtt->property.correlative == EN_INDEPENDENCE)
                {
                    if(NULL == pl_evtt->mark_event)
                    {
                        pl_ecb->previous = pl_ecb;
                        pl_ecb->next = pl_ecb;
                        pl_evtt->mark_event = pl_ecb;
                    }
                    else//�������ȼ��Ŷӣ���һ�������ȼ���ߵ�
                    {
                        temp_var = pl_evtt->mark_event;
                        do
                        {
                            if(temp_var->prio > pl_ecb->prio)
                            {
                                break;
                            }
                            else
                            {
                                temp_var = temp_var->next;
                            }
                           }while(temp_var != pl_evtt->mark_event);

                        temp_var->previous->next = pl_ecb;
                        pl_ecb->previous = temp_var->previous;
                        temp_var->previous = pl_ecb;
                        pl_ecb->next = temp_var;
                        if(pl_ecb->prio > pl_ecb->previous->prio)//���������ߵ����ȼ�
                        {
                            pl_evtt->mark_event = pl_ecb->previous;
                        }
                    }
                }
            }else                                           //�ɹ������߳�
            {
                pl_evtt->vpus++;        //�����¼�vpus��ʾ�߳���
                                        //�����¼���vpus������
            }
        }
    }
}

//----��������-----------------------------------------------------------
//����: �������̡�
//����: ��
//����: ��
//��ע: ��ֻ��ռһ��λ���ѣ���mpģʽ����ʵ�ü�ֵ
//-----------------------------------------------------------------------------
void Djy_CreateProcessVm(void)
{
    static struct ProcessVm my_process;
    g_ptMyProcess = &my_process;
}


//----�¼�����-----------------------------------------------------------------
//���ܣ�����running�¼���cpu,��cpu����ready���еĵ�һ���¼�.���ready���еĵ�һ
//      ���¼���δӵ���߳�,����֮.�����߳�ʱ����ڴ治��,�������¼��ŵ�
//      �ڴ�ȴ�������,readyָ��ָ��ready���е���һ���¼�,������֮,ֱ���ҵ�һ��
//      �߳�ָ��ǿջ��߳ɹ������̵߳��¼�.
//��������
//���أ�true = �ɹ��л���false = δ�л�ֱ�ӷ���
//��ע��1.�������ɲ���ϵͳ����
//      2.djyosϵͳ��Ϊ,�û���ֹ�жϾ���Ϊ���ܹ�����ִ��,���߱����ٽ���Դ.
//      djyos���첽�ź��൱�ڸ����ȼ��߳�,����ȫ�ֺ��첽�źŽ�ֹ״̬
//      ��,�ǲ������¼����ȵ�.
//      3.ʵʱ�ж��Ƿ��ֹ,������޹�.
//      4.����������ȼ���ϵͳ�����¼�����ready,��˱����������ܹ��ҵ�Ŀ���¼�
//-----------------------------------------------------------------------------
bool_t __Djy_Schedule(void)
{
    struct EventECB *event;
    u32 time;

//    if(!Djy_QuerySch())
//        return false;
//    Int_CutAsynSignal();
    __Djy_SelectEventToRun();
    if(g_ptEventReady != g_ptEventRunning)
    {//��running�¼�����ready������,���ڴ治���Խ������߳�ʱ,���ܻ��������
     //������running���¼�ȫ�������ڴ�ȴ����еĿ���.��ʱִ��else�Ӿ�.
        event = g_ptEventRunning;
//#if(CN_CFG_DEBUG_INFO == 1)
        time = (u32)DjyGetSysTime();
        event->consumed_time += time - g_s64RunningStartTime;
        g_s64RunningStartTime = time;
//#endif
        g_ptEvttTable[event->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_OUT);

        g_ptEventRunning=g_ptEventReady;
        g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        Int_HalfEnableAsynSignal( );
        __asm_switch_context(g_ptEventReady->vm ,event->vm);
    }else
    {//���ȼ�����running���¼�ȫ�������ڴ�ȴ�����,��һ��Ҫ������¼�����
     //running�¼�,����ִ���κβ���
        return false;
    }
    return true;
}

//----�ж���ִ�е��¼�����------------------------------------------------------
//���ܣ�����running�¼���cpu,��cpu����ready���еĵ�һ���¼�.���ready���еĵ�һ
//      ���¼��в�ӵ���߳�,�򴴽�֮.�����߳�ʱ����ڴ治��,�������¼��ŵ�
//      �ڴ�ȴ�������,readyָ��ָ��ready���е���һ���¼�,������֮,ֱ���ҵ�һ��
//      �߳�ָ��ǿջ��߳ɹ������̵߳��¼�.
//��������
//���أ���
//��ע���������ɲ���ϵͳ���첽�ź����淵��ǰ����
//      ����������ȼ���y_idle_service�¼�����ready,��������ܹ��ҵ����ȶ����.
//-----------------------------------------------------------------------------
void __Djy_ScheduleAsynSignal(void)
{
    struct EventECB *event;
    u32 time;

    __Djy_SelectEventToRun();
    if(g_ptEventReady != g_ptEventRunning)
    {//��running�¼�����ready������,���ڴ治���Խ������߳�ʱ,���ܻ��������
     //������running���¼�ȫ�������ڴ�ȴ����еĿ���.��ʱִ��else�Ӿ�.
         event=g_ptEventRunning;
//#if(CN_CFG_DEBUG_INFO == 1)
         time = (u32)DjyGetSysTime();
         event->consumed_time += time - g_s64RunningStartTime;
         g_s64RunningStartTime = time;
//#endif
         g_ptEvttTable[event->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_OUT);

         g_ptEventRunning=g_ptEventReady;
         g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
         __asm_switch_context_int(g_ptEventReady->vm,event->vm);
    }else
    {//���ȼ�����running���¼�ȫ�������ڴ�ȴ�����,��һ��Ҫ������¼�����
     //running�¼�,����ִ���κβ���
    }
    return;
}

//----�Ǽ��¼�����------------------------------------------------------------
//���ܣ��Ǽ�һ���¼����͵�ϵͳ��,�¼����;����ǼǺ�,�Ϳ���pop��,����,ϵͳ��
//      �ܾ�pop�������¼�
//������relation��ȡֵΪEN_INDEPENDENCE��EN_CORRELATIVE
//      default_prio�����¼����͵�Ĭ�����ȼ���
//      vpus_res��ϵͳ��æʱΪ�������¼��������߳�������
//      vpus_limit���������¼�����ӵ�е��߳����������ֵ
//      para_limit, �������г������ƣ��Ա��������ز�������
//      thread_routine���߳���ں���(�¼�������)
//      Stack���û�Ϊ�����¼����̷߳����ջ��NULL��ʾ��heap�з���ջ������¼���
//              EN_INDEPENDENCE���ͣ����ж����¼����д�������ҪΪ�����ʹ�����
//              ���߳�ʱ���ӵڶ����߳̿�ʼ����һ���Ǵ�heap������Stack��
//      stack_size��ִ��thread_routine��Ҫ��ջ�ߴ磬������thread_routine��������
//          ���õ�ϵͳ����
//      evtt_name���¼�����������ͬģ��֮��Ҫ���浯���¼��Ļ������¼���������
//          ������������NULL����ֻҪ��NULL���Ͳ��������¼����ͱ������е���������
//          ���ֲ�����31�����ֽ��ַ�
//���أ����¼����͵����ͺ�
//------------------------------------------------------------------------------
u16 Djy_EvttRegist(enum enEventRelation relation,
                       ufast_t default_prio,
                       u16 vpus_res,
                       u16 vpus_limit,
//                       u16 para_limit,
                       ptu32_t (*thread_routine)(void),
                       void *Stack,
                       u32 StackSize,
                       char *evtt_name)
{
    struct ExpThrowPara  parahead;
    char ExpStr[64] = "�¼����Ϳ��ƿ�ľ�: ";
    u16 i,evtt_offset;
    u32 temp;
    if((default_prio >= CN_PRIO_SYS_SERVICE) || (default_prio == 0))
    {
        Djy_SaveLastError(EN_KNL_INVALID_PRIO);
        printf("�¼��������ȼ��Ƿ�\n\r");
        return CN_EVTT_ID_INVALID;
    }
    Int_SaveAsynSignal();      //��ֹ����Ҳ���ǽ�ֹ�첽�¼�
    //���ҿ��е��¼����ƿ�
    for(evtt_offset=0; evtt_offset<gc_u32CfgEvttLimit; evtt_offset++)
        if( g_ptEvttTable[evtt_offset].property.registered ==0)
            break;
    if(evtt_offset == gc_u32CfgEvttLimit)     //û�п����¼����ƿ�
    {
        parahead.DecoderName = NULL;
        parahead.ExpAction = EN_EXP_DEAL_RECORD;
        strcat(ExpStr,evtt_name);
        parahead.ExpInfo = (u8*)ExpStr;
        parahead.ExpInfoLen = sizeof(ExpStr);
        parahead.ExpType = CN_EXP_TYPE_ETCB_EXHAUSTED;
        Exp_Throw(&parahead);
        Djy_SaveLastError(EN_KNL_ETCB_EXHAUSTED);
        printf("û�п����¼����ƿ�: %s\n\r",evtt_name);
        Int_RestoreAsynSignal();
        return CN_EVTT_ID_INVALID;
    }else if(evtt_name != NULL) //�����������֣�������û������
    {
        for(i=0; i<gc_u32CfgEvttLimit; i++)
        {
            if(g_ptEvttTable[i].property.registered == 1)
            {
                if(strcmp(g_ptEvttTable[i].evtt_name,evtt_name) == 0)
                {
                    Djy_SaveLastError(EN_KNL_EVTT_HOMONYMY);
                    printf("�¼���������: %s\n\r",evtt_name);
                    Int_RestoreAsynSignal();
                    return CN_EVTT_ID_INVALID;
                }
            }
        }
        if(strnlen(evtt_name,32) <= 31)
            strcpy(g_ptEvttTable[evtt_offset].evtt_name,evtt_name);
        else
        {
            memcpy(g_ptEvttTable[evtt_offset].evtt_name,evtt_name,31);
            g_ptEvttTable[evtt_offset].evtt_name[31] = '\0';
        }
    }else
    {
        g_ptEvttTable[evtt_offset].evtt_name[0] = '\0';   //����¼�������
    }

    g_ptEvttTable[evtt_offset].default_prio = default_prio;
    g_ptEvttTable[evtt_offset].events = 0;
    g_ptEvttTable[evtt_offset].vpus_limit = vpus_limit;
    if(relation == EN_INDEPENDENCE)
    {
        //�����ȼ��¼��������ٱ���һ���߳�
        if( (vpus_res == 0) && ((default_prio < 0x80)
                              || (CN_CFG_DYNAMIC_MEM == 0) ) )
            g_ptEvttTable[evtt_offset].vpus_res = 1;
        else
            g_ptEvttTable[evtt_offset].vpus_res = vpus_res;
    }else
    {
        g_ptEvttTable[evtt_offset].vpus_res = 0;    //�������¼���vpus_res��Ч
    }
    g_ptEvttTable[evtt_offset].SchHook = (SchHookFunc)NULL_func;
    g_ptEvttTable[evtt_offset].thread_routine = thread_routine;
    g_ptEvttTable[evtt_offset].stack_size = StackSize;
    g_ptEvttTable[evtt_offset].mark_event = NULL;
    g_ptEvttTable[evtt_offset].done_sync = NULL;
    g_ptEvttTable[evtt_offset].pop_sync = NULL;
    g_ptEvttTable[evtt_offset].vpus = 0;
//    g_ptEvttTable[evtt_offset].para_limit = para_limit;
    if(Stack == NULL)
    {
        if((default_prio<0x80) ||(CN_CFG_DYNAMIC_MEM == 0))
        {//����ģʽΪsi��dlsp������¼�����ӵ�н������ȼ�����Ԥ�ȴ���һ���߳�
            g_ptEvttTable[evtt_offset].my_free_vm =
                                __CreateThread(&g_ptEvttTable[evtt_offset],&temp);
            if(g_ptEvttTable[evtt_offset].my_free_vm == NULL)
            {//�ڴ治�㣬���ܴ����߳�
                parahead.DecoderName = NULL;
                parahead.ExpAction = EN_EXP_DEAL_RECORD;
                ExpStr[0] = '\0';
                strcat(ExpStr,"�Ǽ��¼�����ʱ�ڴ治��: ");
                strcat(ExpStr,evtt_name);
                parahead.ExpInfo = (u8*)ExpStr;
                parahead.ExpInfoLen = sizeof(ExpStr);
                parahead.ExpType = CN_EXP_TYPE_MEM_EVTT;
                Exp_Throw(&parahead);
                Djy_SaveLastError(EN_MEM_TRIED);
                printf("%s\n\r",evtt_name);
                Int_RestoreAsynSignal();
                return CN_EVTT_ID_INVALID;
            }else
            {
                g_ptEvttTable[evtt_offset].vpus = 1;
            }
        }else
        {
            g_ptEvttTable[evtt_offset].my_free_vm = NULL;
        }
    }
    else
    {
        g_ptEvttTable[evtt_offset].my_free_vm =
               __CreateStaticThread(&g_ptEvttTable[evtt_offset],Stack,StackSize);
    }
    g_ptEvttTable[evtt_offset].property.correlative = relation;
    g_ptEvttTable[evtt_offset].property.registered = 1;
    g_ptEvttTable[evtt_offset].property.inuse = 0;
    g_ptEvttTable[evtt_offset].property.deleting = 0;
    g_ptEvttTable[evtt_offset].pop_times = 0;
    Int_RestoreAsynSignal();
    return evtt_offset | CN_EVTT_ID_MASK;
}


//----ע�����hook����---------------------------------------------------------
//���ܣ�ע�����hook����, �ú������¼����������������ʱ���á�
//������EvttID, �¼�����ID
//      HookFunc, hook����ָ��
//���أ�true = �ɹ�ע��, false = ʧ�ܡ�
//----------------------------------------------------------------------------
bool_t Djy_RegisterHook(u16 EvttID,SchHookFunc HookFunc)
{
    if(EvttID-CN_EVTT_ID_BASE  >= gc_u32CfgEvttLimit)
    {
        return false;
    }
    else
    {
        //�����ж���ӦEvttID�Ƿ�ʹ��
        g_ptEvttTable[EvttID-CN_EVTT_ID_BASE].SchHook = HookFunc;
    }
    return true;
}

//----ȡ�¼�����id-------------------------------------------------------------
//���ܣ������¼����͵����֣����ظ��¼����͵�id�ţ�������û���ֵ��¼����͡�
//������evtt_name���¼�������
//���أ��¼�����id�ţ����û���ҵ��򷵻�cn_invalid_id��
//��ע��ֻ���ҵ������ֵ��¼����ͣ�û���ֵ��¼������ֽ������¼����͡�
//----------------------------------------------------------------------------
u16 Djy_GetEvttId(char *evtt_name)
{
    u16 loop;
    if(evtt_name == NULL)
        return CN_EVTT_ID_INVALID;
    for(loop = 0; loop < gc_u32CfgEvttLimit; loop++)
    {
        if((strcmp(g_ptEvttTable[loop].evtt_name,evtt_name) ==0)
                &&(g_ptEvttTable[loop].property.registered))
            return loop | CN_EVTT_ID_MASK;
    }
    return CN_EVTT_ID_INVALID;
}

//----ɾ���¼�����-------------------------------------------------------------
//����: ɾ��һ���¼�����,��������л��и������¼�(inuse == 1),ֻ��Ǹ�����Ϊ��
//      ɾ��,������ɾ���������� done��������ɵ�.����������Ѿ�û�и������¼���,
//      ����ִ��������ɾ������.�����������,ϵͳ����ܾ����������͵����¼�.
//����: evtt_id,��ɾ�����¼����ͺ�
//����: true = �ɹ��������ɹ���ǣ�false = ʧ��
//-----------------------------------------------------------------------------
bool_t Djy_EvttUnregist(u16 evtt_id)
{
    struct ThreadVm *next_vm,*temp;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb,*pl_ecb_temp;
    bool_t result = true;
    if((evtt_id & (~CN_EVTT_ID_MASK)) >= gc_u32CfgEvttLimit)
        return false;
    Int_SaveAsynSignal();
    pl_evtt = &g_ptEvttTable[evtt_id & (~CN_EVTT_ID_MASK)];
    if(pl_evtt->property.registered == 0)
    {//���¼���������Ч�¼�����
        result = false;
    }
//  if(pl_evtt->done_sync != NULL)     //�����ͬ�����������¼���ȡ������������
//  {
//      pl_ecb = pl_evtt->done_sync;     //ȡͬ������ͷ
//      while(pl_ecb != NULL)
//      {
//          if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //�Ƿ��ڳ�ʱ������
//          {
//              __Djy_ResumeDelay(pl_ecb);    //������ʱ�ȴ�
//          }
//          pl_ecb->wakeup_from = CN_WF_EVTT_DELETED;   //���û���ԭ��
//          pl_ecb->event_status = CN_STS_EVENT_READY;
//          pl_ecb_temp = pl_ecb;
//          if(pl_ecb->multi_next == pl_evtt->done_sync)   //�����һ���¼�
//          {
//              pl_evtt->done_sync = NULL;
//              pl_ecb = NULL;
//          }else
//          {
//              pl_ecb = pl_ecb->multi_next;
//          }
//          __Djy_EventReady(pl_ecb_temp);           //���¼����뵽����������
//      }
//  }
    if(pl_evtt->pop_sync != NULL)     //������ͬ�����������¼���ȡ������������
    {
        pl_ecb = pl_evtt->pop_sync;     //ȡͬ������ͷ
        while(pl_ecb != NULL)
        {
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //�Ƿ��ڳ�ʱ������
            {
                __Djy_ResumeDelay(pl_ecb);    //������ʱ�ȴ�
            }
            pl_ecb->wakeup_from = CN_WF_EVTT_DELETED;    //���û���ԭ��
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb_temp = pl_ecb;
            if(pl_ecb->multi_next == pl_evtt->done_sync)  //�����һ���¼�
            {
                pl_evtt->pop_sync = NULL;  //�ÿ��¼�ͬ������
                pl_ecb = NULL;
            }else
            {
                pl_ecb = pl_ecb->multi_next;      //ȡ��������һ���¼�
            }
            __Djy_EventReady(pl_ecb_temp);       //���¼����뵽����������
        }
    }
    if(pl_evtt->property.inuse)
    {
        //�¼���������ʹ�ã������ͬ���͵���ͬ�����зǿգ�ֻ���ɾ��
        pl_evtt->property.deleting = 1;
    }else
    {
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';      //����¼�������
        next_vm = pl_evtt->my_free_vm;
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ����߳�
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
    }
    Int_RestoreAsynSignal();
    return result;
}
const struct EventECB cn_sys_event = {
                        NULL,NULL,//next,previous
                        NULL,NULL,//multi_next,multi_previous
                        NULL,                       //vm
                        0,0,                        //param1,param2
                       NULL,                       //sync
                        NULL,                       //sync_head
                        0,                          //EventStartTime
                        0,                          //consumed_time
//#if(CN_CFG_DEBUG_INFO == 1)
                        0,                          //consumed_time_second
                        0,                          //consumed_time_record
//#endif
                        0,                          //delay_start_tick
                        0,                          //delay_end_tick
                        EN_KNL_NO_ERROR,          //error_no
                        0,                          //event_result
                        0,                          //wait_mem_size
                        CN_WF_EVENT_NORUN,          //wakeup_from
                        CN_STS_EVENT_READY,         //event_status
                        CN_PRIO_SYS_SERVICE,        //prio_base
                        CN_PRIO_SYS_SERVICE,        //prio
                        CN_EVTT_ID_BASE,            //evtt_id
                        0,                          //sync_counter
                        0,                          //event_id
                        0                           //local_memory
                        };

//----�����¼�����-------------------------------------------------------------
//���ܣ�1.����ϵͳ�趨��ʼ������ϵͳ�̺߳��¼���ָ��
//      2.�� gc_u32CfgEventLimit ���¼����ƿ�ṹ���ӳ�һ�����ж���,���еĽṹ�μ�
//        �ĵ�����ָ��pg_free_event����,
//��������
//���أ���
//-----------------------------------------------------------------------------
void __Djy_InitSys(void)
{
    u16 i;
    u8 *IdleStack;
    struct ThreadVm *vm;
    g_ptEventDelay=NULL;    //��ʱ�¼������

    IdleStack = M_Malloc(gc_IdleSeventStackSize,0);
    g_ptECB_Table = M_Malloc(gc_u32CfgEventLimit * sizeof(struct EventECB),0);
    g_ptEvttTable = M_Malloc(gc_u32CfgEvttLimit * sizeof(struct EventType),0);
    if(g_ptEvttTable == NULL)
    {
        while(1);       //��ʱIOϵͳδ׼���ã����ܵ������������ʱ��Ȼ�ܷ��֡�
    }
    //���¼����ͱ�ȫ����Ϊû��ע��,0Ϊϵͳ��������
    for(i=1; i<gc_u32CfgEvttLimit; i++)
    {
        g_ptEvttTable[i].property.registered = 0;
    }
    for(i = 1; i < gc_u32CfgEventLimit; i++)    //i=0Ϊϵͳ�����¼�
    {
        if(i==(gc_u32CfgEventLimit-1))
            g_ptECB_Table[i].next = NULL;
        else
            g_ptECB_Table[i].next = &g_ptECB_Table[i+1];
        //��ǰָ��ָ��pg_event_free�ĵ�ַ,˵�����¸������¼���.
        //û�б�ĺ���,ֻ����һ��Ψһ�Ҳ������ֵ,ȫ�ֱ�����ַ������������
        //�ǲ���仯��.
        g_ptECB_Table[i].previous = (struct EventECB*)&s_ptEventFree;
        g_ptECB_Table[i].event_id = i;    //��id���ڳ���������ά�ֲ���
        g_ptECB_Table[i].evtt_id = CN_EVENT_ID_INVALID;
    }
    s_ptEventFree = &g_ptECB_Table[1];

    g_ptEventReady = g_ptECB_Table;
    g_ptEventRunning = g_ptEventReady;
    g_ptEvttTable[0].property.correlative = 1;
//    g_ptEvttTable[0].property.overlay = 0;
    g_ptEvttTable[0].property.registered = 1;
    g_ptEvttTable[0].property.inuse = 1;
    g_ptEvttTable[0].property.deleting = 0;
    g_ptEvttTable[0].my_free_vm = NULL;
    g_ptEvttTable[0].SchHook = (SchHookFunc)NULL_func;
   // g_ptEvttTable[0].evtt_name[0] = '\0';

    strcpy(g_ptEvttTable[0].evtt_name,"sys_idle");

    g_ptEvttTable[0].default_prio = CN_PRIO_SYS_SERVICE;
    g_ptEvttTable[0].events = 1;
    g_ptEvttTable[0].vpus_res =0;
    g_ptEvttTable[0].vpus_limit =1;
    g_ptEvttTable[0].vpus = 1;
    g_ptEvttTable[0].thread_routine = __Djy_Service;
    g_ptEvttTable[0].stack_size = gc_IdleSeventStackSize;
    g_ptEvttTable[0].mark_event = g_ptECB_Table;
    g_ptEvttTable[0].done_sync = NULL;
    g_ptEvttTable[0].pop_sync = NULL;

    vm = __CreateStaticThread(&g_ptEvttTable[0],IdleStack,gc_IdleSeventStackSize);
    if(vm == NULL)      //�ڴ治�㣬���ܴ�����פ�߳�
    {
        //��ʱg_ptEventRunning��δ��ֵ���޷�����Djy_SaveLastError
//        Djy_SaveLastError(EN_MEM_TRIED);
//        printf"�����߳�ʱ�ڴ治��\n\r");
        g_ptEvttTable[0].vpus = 0;
        return ;
    }
    //����ģ��popup����,����ϵͳ�����¼�.���¼�����Ϊ��ʱ,����popup�Ľ��
    //�ǲ���Ԥ֪��.����ϵͳ����ʱ,ϵͳ�����¼����ڶ�����,�����¼������ǲ����
    //��,����ģ��popup�¼�,������ֱ�ӵ���,����ʹpopup������ʡ��һ���ж϶����Ƿ�
    //�յĲ���.
    g_ptECB_Table[0] = cn_sys_event;
    g_ptECB_Table[0].vm = vm;
    g_ptECB_Table[0].next = g_ptECB_Table;
    g_ptECB_Table[0].previous = g_ptECB_Table;
    g_ptECB_Table[0].multi_next = g_ptECB_Table;
    g_ptECB_Table[0].multi_previous = g_ptECB_Table;
}

//--------����Ƿ��������------------------------------------------------------
//����: ����Ƿ��������,�����첽�ź����������߳�̬ʱ���������
//����: ��
//����: ������ȷ���true,���򷵻�false
//-----------------------------------------------------------------------------
bool_t Djy_QuerySch(void)
{
    return g_bScheduleEnable;
}

//--------�������Ƿ��Ѿ���ʼ--------------------------------------------------
//����: ����Ƿ��Ѿ��������¼�(�߳�)����,����__Djy_StartOs������,������������true,
//      ���򷵻�false
//����: ��
//����: true = �����Ѿ���ʼ,�����Ƿ������л�.false = ������δ��ʼ
//-----------------------------------------------------------------------------
bool_t Djy_IsMultiEventStarted(void)
{
    return g_bMultiEventStarted;
}
//----���¼��������������----------------------------------------------------
//���ܣ����¼����뵽���������к��ʵ�λ��,���¼�ԭ�����ھ���������.������ֻ����
//      �¼����У�������event_status��ֵ
//������event_ready,��������¼�,���¼�ԭ�����ھ���������
//���أ���
//------------------------------------------------------------------------------
void __Djy_EventReady(struct EventECB *event_ready)
{
    struct EventECB *event;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart(  );             //�������Բ���Ƕ�׵���
    event = g_ptEventReady;
    do
    { //�ҵ�һ�����ȼ��������¼����¼�.����ϵͳ�����¼�prio=250����ready,����������ҵ�.
        if(event->prio <= event_ready->prio)
            event = event->multi_next;
        else
            break;
    }while(event != g_ptEventReady);
    event_ready->next = event;
    event_ready->previous = event->previous;
    event->previous->next = event_ready;
    event->previous = event_ready;

    //�²�����¼���ͬ���ȼ�����󣬹������ܹ��ж����¼��Ƿ�����ȼ���Ψһ�¼���
    //���Ǹ����ȼ���Ψһ�¼�����Ҫ�������ȼ���������
    if(event_ready->prio != event_ready->previous->prio)
    {//��ʱevent == event_ready->next;
        event->multi_previous->multi_next = event_ready;
        event_ready->multi_previous = event->multi_previous;
        event->multi_previous = event_ready;
        event_ready->multi_next = event;
    }else       //����������ȼ���������
    {
        event_ready->multi_next = NULL;
        event_ready->multi_previous = NULL;
    }
    if(event_ready->prio < g_ptEventReady->prio)
        g_ptEventReady = event_ready;
    Int_LowAtomEnd( atom_low );
}

//----�˳�����ͬ������---------------------------------------------------------
//����: ��һ���¼�������ͬ��������ȡ��,��������ʱ���Ƿ��Ѿ�����.���ڴ���ʱ��ͬ��
//      �����У���ͬ���������ڳ�ʱʱ�޵���ʱ��������ͬ������ȡ���¼���
//����: delay_event,��������¼�.
//����: ��
//��ע: 1.�����������Ÿ��û�,�����ǲ���ϵͳ�ڲ�ʹ��.����ϵͳ�����û��ṩ��ǰ
//      �˳���ʱ�Ĺ���,�������Է�ֹ�¼��以�����
//      2.������Ӧ���ڹرյ��������µ���,�����߱�֤,�����ڲ�������ж�״̬.
//      3.������ֻ���¼�������ͬ��������ȡ���������ŵ����������С�
//-----------------------------------------------------------------------------
void __Djy_ResumeDelay(struct EventECB *delay_event)
{
    if(g_ptEventDelay->next == g_ptEventDelay)  //������ֻ��һ���¼�
        g_ptEventDelay = NULL;
    else
    {
        if(delay_event == g_ptEventDelay)
            g_ptEventDelay = g_ptEventDelay->next;
        delay_event->next->previous = delay_event->previous;
        delay_event->previous->next = delay_event->next;
    }
    delay_event->next = NULL;
    delay_event->previous = NULL;
    delay_event->delay_end_tick = DjyGetSysTick();
}

//----������ʱ����------------------------------------------------------------
//���ܣ�������ִ�е��¼�����,ֱ�Ӱ��Լ�������ʱ���У����������Ҳ������ready
//      ���У�һ����ͬ���������ã���timeout!=0ʱ���¼���������ͬ������ʵ��
//      timeout���ܣ����ض������¶�y_timer_sync�����ļ򻯡�
//������u32l_uS,�ӳ�ʱ��,��λ��΢�룬�������ϵ���ΪCN_CFG_TICK_US��������
//���أ���
//��ע��1������ϵͳ�ڲ�ʹ�õĺ����������ڹر��жϣ���ֹ���ȣ���������ʹ�á�
//      2�����ñ�����ǰrunning�¼��Ѿ��Ӿ�������ȡ�������������ı������
//      3���������ڲ�����һ�����ɵ��÷���֤���������ԣ���u32l_uS>0.
//-----------------------------------------------------------------------------
//change by lst in 20130922,ticks��Ϊ64bit��ɾ������32λ��������ƵĴ���
void __Djy_AddToDelay(u32 u32l_uS)
{
    struct EventECB * event;

    g_ptEventRunning->delay_start_tick = DjyGetSysTick(); //�¼���ʱ��ʼʱ��
    g_ptEventRunning->delay_end_tick = g_ptEventRunning->delay_start_tick
                  + ((s64)u32l_uS + CN_CFG_TICK_US -(u32)1)/CN_CFG_TICK_US; //����ʱ��

    if(g_ptEventDelay==NULL)    //��ʱ���п�
    {
        g_ptEventRunning->next = g_ptEventRunning;
        g_ptEventRunning->previous = g_ptEventRunning;
        g_ptEventDelay=g_ptEventRunning;
    }else
    {
        event = g_ptEventDelay;
        do
        {//��ѭ���ҵ���һ��ʣ����ʱʱ���������ʱ�¼����¼�.
            if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
            {
                event = event->next;
            }
            else
                break;

        }while(event != g_ptEventDelay);
        //���û���ҵ�ʣ����ʱʱ�������ʱ�¼������¼�,���¼��������β,
        //������β������pg_event_delay��ǰ��,eventǡ�õ���pg_event_delay
        //����ҵ�ʣ����ʱʱ�䳤�����¼����¼�,���¼�������¼�ǰ��.
        //��������ǰ��ѭ���ҵ����,����ʹ�����д���
        g_ptEventRunning->next = event;
        g_ptEventRunning->previous = event->previous;
        event->previous->next = g_ptEventRunning;
        event->previous = g_ptEventRunning;
        if(g_ptEventDelay->delay_end_tick > g_ptEventRunning->delay_end_tick)
            //���¼���ʱС��ԭ�����е���С��ʱ.
            g_ptEventDelay = g_ptEventRunning;
    }
}

//----��Ӵ������¼�����������-------------------------------------------------
//���ܣ����������е��¼�����ӽ��������У�ֻ��Ӷ��У������ȡ�
//      ֻ֧��FIFO��ʽ�������ȼ�������������ʽ����ʹ�ñ�����
//      ������ֻ�����ں�ģ����ã������߱�֤������ȫ��
//������Head,��������ͷ
//      Qsort�����������Ŷӷ�ʽ��ֻ֧��CN_BLOCK_FIFO��CN_BLOCK_PRIO
//      timeout����ʱ����,������0������������е�ͬʱ�������볬ʱ���С�
//         ��λ��΢�룬timeout==CN_TIMEOUT_FOREVER�򲻻���볬ʱ���У�
//         timeout ==0��Ӧ���ñ���������0ֵ�������ϵ���ΪCN_CFG_TICK_US�ı���
//      Status������״̬���ο�CN_STS_EVENT_READYϵ�ж���
//���أ���
//-----------------------------------------------------------------------------
void __Djy_AddRunningToBlock(struct EventECB **Head,bool_t Qsort,u32 timeout,u32 Status)
{
    struct EventECB *event;
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->next = NULL;

    g_ptEventRunning->sync_head = Head;
    if(*Head == NULL)
    {//ͬ�����п�,running�¼��Գ�˫��ѭ������
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
        *Head = g_ptEventRunning;
    }else
    {
        event = *Head;
        if(Qsort == CN_BLOCK_PRIO)   //ͬ�����а����ȼ�����
        {
            do
            {   //�ҵ�һ�����ȼ��������¼����¼�.
                //����Ҳ�������ͣ�ڶ���ͷ��λ�ã���λ��Ҳ����ȷ�Ĳ���㡣
                if(event->prio <= g_ptEventRunning->prio)
                    event = event->multi_next;
                else
                    break;
            }while(event != *Head);
            g_ptEventRunning->event_status |= CN_BLOCK_PRIO_SORT;
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
            if((*Head)->prio > g_ptEventRunning->prio)
                *Head = g_ptEventRunning;
        }else                               //���Ⱥ�˳�����¼�ֱ�����ڶ�β
        {
            g_ptEventRunning->event_status &= ~CN_BLOCK_PRIO_SORT;
            g_ptEventRunning->multi_next = event;
            g_ptEventRunning->multi_previous = event->multi_previous;
            event->multi_previous->multi_next = g_ptEventRunning;
            event->multi_previous = g_ptEventRunning;
        }
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        //�¼�״̬��Ϊ�ȴ��ź��� +  ��ʱ
        g_ptEventRunning->event_status = Status + CN_STS_SYNC_TIMEOUT;
        __Djy_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = Status;  //�¼�״̬��Ϊ�ȴ��ź���
    }
}

//----�޸���������-------------------------------------------------------------
//���ܣ�����ָ���¼�����PRIO��������������е�λ�ã����޸��������е��¼����ȼ���
//      ����Ҫ���ñ������ı����ڶ����е�λ�á�
//      ������ֻ�����ں�ģ����ã������߱�֤������ȫ��
//������Event�����������¼�
//���أ���
//-----------------------------------------------------------------------------
void __Djy_ChangeBlockQueue(struct EventECB *Event)
{
    struct EventECB *pl_ecb;
    struct EventECB *Head;
    ufast_t Prio;
    Head = *Event->sync_head;
    pl_ecb = Head;
    Prio = Event->prio;
    //�����ƶ�λ�õ�������
    //��һ�У����ڶ���ͷ������С��
    //�ڶ��У����Ȳ����ں�һ����������ǰһ����
    //�����У����ڶ���β�������
    if( ((Event == pl_ecb)  && (Prio <= Event->multi_next->prio))
       ||((Prio >= Event->multi_previous->prio)&& (Prio <= Event->multi_next->prio))
       ||((Event == pl_ecb->multi_previous)&& (Prio >= Event->multi_previous->prio)) )
        return ;

    //�Ӷ�����ȡ���¼����ƿ�
    Event->multi_previous->multi_next = Event->multi_next;
    Event->multi_next->multi_previous = Event->multi_previous;
    do
    {   //�ҵ�һ�����ȼ��������¼����¼�.
        //����Ҳ�������ͣ�ڶ���ͷ��λ�ã���λ��Ҳ����ȷ�Ĳ���㡣
        if(pl_ecb->prio < Event->prio)
            pl_ecb = pl_ecb->multi_next;
        else
            break;
    }while(pl_ecb != Head);

    Event->multi_next = pl_ecb;
    Event->multi_previous = pl_ecb->multi_previous;
    pl_ecb->multi_previous->multi_next = Event;
    pl_ecb->multi_previous = pl_ecb;
    if((Head)->prio > Event->prio)
        Head = Event;
}

//----�����¼����ȼ�-----------------------------------------------------------
//����: �ı��¼����ȼ�������ı���������ȼ����������ȼ����ĵ��ˣ������������ȣ�
//      �������¼����С�
//����: event_id�����������¼�id
//      new_prio,���õ������ȼ�
//����: true = �ɹ����ã�false=ʧ�ܣ�һ�������ȼ����Ϸ�
//-----------------------------------------------------------------------------
bool_t Djy_SetEventPrio(u16 event_id,ufast_t new_prio)
{
    struct EventECB * pl_ecb;

    if((new_prio >= CN_PRIO_SYS_SERVICE) || (new_prio == 0)
       || (event_id >= gc_u32CfgEventLimit))
        return false;

    Int_SaveAsynSignal();
    pl_ecb = &g_ptECB_Table[event_id];
    pl_ecb->prio_base = new_prio;
    if(new_prio != pl_ecb->prio)
    {
        //�¼�ԭ����״̬�����ܣ�1�����ھ���̬��2������ĳ������̬��
        //�������ĳ������̬����������ǰ����ȼ�����ģ���Ҫ�޸ĸö��С�
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //ע���������˳���ܱ䣬��Ϊprio��ֵ��__Djy_CutReadyEvent����ִ��
            //    �����Ӱ��
            __Djy_CutReadyEvent(pl_ecb);
            pl_ecb->prio = new_prio;
            __Djy_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = new_prio;
            __Djy_ChangeBlockQueue(pl_ecb);
        }
    }
    Int_RestoreAsynSignal();
    return true;
}

//----�̳��¼����ȼ�-----------------------------------------------------------
//����: ���g_ptEventRunning�����ȼ��ϸߣ�event_id��ʱ��g_ptEventRunning�����ȼ�
//      ���У�ֱ������Djy_RestorePrio�����򲻸ı����ȼ���
//����: event_id�����������¼�id
//����: true = �ɹ����ã�false=ʧ�ܣ�һ�������ȼ����Ϸ�
//-----------------------------------------------------------------------------
bool_t Djy_RaiseTempPrio(u16 event_id)
{
    struct EventECB * pl_ecb;

    if(event_id >= gc_u32CfgEventLimit)
        return false;

    Int_SaveAsynSignal();
    pl_ecb = &g_ptECB_Table[event_id];
    if(g_ptEventRunning->prio < pl_ecb->prio)
    {
        //�¼�ԭ����״̬�����ܣ�1�����ھ���̬��2������ĳ������̬��
        //�������ĳ������̬����������ǰ����ȼ�����ģ���Ҫ�޸ĸö��С�
        if(pl_ecb->event_status == CN_STS_EVENT_READY)
        {
            //ע���������˳���ܱ䣬��Ϊprio��ֵ��__Djy_CutReadyEvent����ִ��
            //    �����Ӱ��
            __Djy_CutReadyEvent(pl_ecb);
            pl_ecb->prio = g_ptEventRunning->prio;
            __Djy_EventReady(pl_ecb);
        }
        else if(pl_ecb->event_status & CN_BLOCK_PRIO_SORT)
        {
            pl_ecb->prio = g_ptEventRunning->prio;
            __Djy_ChangeBlockQueue(pl_ecb);
        }
    }
    Int_RestoreAsynSignal();
    return true;
}

//----�ָ��¼����ȼ�-----------------------------------------------------------
//����: ����Djy_SetTempPrio�󣬿��Ե��ñ������ָ�g_ptEventRunning�����ȼ���
//����: �ޣ�
//����: true = �ɹ����ã�false=ʧ�ܣ�һ�������ȼ����Ϸ�
//-----------------------------------------------------------------------------
bool_t Djy_RestorePrio(void)
{

    Int_SaveAsynSignal();
    if(g_ptEventRunning->prio != g_ptEventRunning->prio_base)
    {
        //ע���������˳���ܱ䣬��Ϊprio��ֵ��__Djy_CutReadyEvent����ִ��
        //    �����Ӱ��
        __Djy_CutReadyEvent(g_ptEventRunning);
        g_ptEventRunning->prio = g_ptEventRunning->prio_base;
        __Djy_EventReady(g_ptEventRunning);
    }
    Int_RestoreAsynSignal();
    return true;
}

//----����ͬ��-----------------------------------------------------------------
//���ܣ�������ִ�е��¼�����,ʹ�Լ���ͣu32l_uS΢����������.
//������u32l_uS,�ӳ�ʱ��,��λ��΢�룬0��������ת��������¼��ŵ�ͬ���ȼ���
//      ��󡣷�0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�ʵ����ʱʱ��us��
//��ע����ʱ����Ϊ˫��ѭ������
//change by lst in 20130922,ticks��Ϊ64bit��ɾ������32λ��������ƵĴ���
//-----------------------------------------------------------------------------
u32 Djy_EventDelay(u32 u32l_uS)
{
    struct EventECB * event;

    if( !Djy_QuerySch())
    {
//        Djy_SaveLastError(EN_KNL_CANT_SCHED);
        Djy_DelayUs(u32l_uS);
        return u32l_uS;
    }
    Int_SaveAsynSignal();
    //��ʱ��Ϊ0���㷨:������������ͬ���ȼ��ģ��ѱ��¼��ŵ���ת���һ����
    //ע��:���ﲻ����ת�����Ƿ�����
    if(u32l_uS == 0)
    {
        if((g_ptEventRunning->prio == g_ptEventRunning->next->prio)
                    && (g_ptEventRunning != g_ptEventRunning->next)   )
        {
            g_ptEventRunning->delay_start_tick = DjyGetSysTick();//����ʱ��
            __Djy_CutReadyEvent(g_ptEventRunning);      //��ͬ������ȡ��
            __Djy_EventReady(g_ptEventRunning);            //�Ż�ͬ������β��
        }else
        {
            Int_RestoreAsynSignal();
            return 0;   //��ʱ��Ϊ0���Ҳ�������ת����
        }
    }else
    {
        g_ptEventRunning->delay_start_tick =DjyGetSysTick();//�趨�����ʱ��
        g_ptEventRunning->delay_end_tick = g_ptEventRunning->delay_start_tick
                  + ((s64)u32l_uS + CN_CFG_TICK_US -(u32)1)/CN_CFG_TICK_US; //����ʱ��

        __Djy_CutReadyEvent(g_ptEventRunning);

        g_ptEventRunning->event_status = CN_STS_EVENT_DELAY;
        if(g_ptEventDelay==NULL)    //����ͬ�����п�
        {
            g_ptEventRunning->next = g_ptEventRunning;
            g_ptEventRunning->previous = g_ptEventRunning;
            g_ptEventDelay=g_ptEventRunning;
        }else
        {
            event = g_ptEventDelay;
            do
            {//��ѭ���ҵ���һ������ʱ���������¼����¼�.
                if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
                {
                    event = event->next;
                }
                else
                    break;
            }while(event != g_ptEventDelay);
            //��������¼�����ǰ���ҵ����¼�ǰ�棬��û���ҵ�����event������
            //pg_event_delay������˫��ѭ�����У�g_event_delayǰ��Ҳ�͸պ��Ƕ���β��
            g_ptEventRunning->next = event;
            g_ptEventRunning->previous = event->previous;
            event->previous->next = g_ptEventRunning;
            event->previous = g_ptEventRunning;
            if(g_ptEventDelay->delay_end_tick >g_ptEventRunning->delay_end_tick)
                //���¼���ʱС��ԭ�����е���С��ʱ.
                g_ptEventDelay = g_ptEventRunning;
        }
    }
    Int_RestoreAsynSignal();
    return (DjyGetSysTick() -g_ptEventRunning->delay_start_tick)*CN_CFG_TICK_US;
}

//----����ͬ��2----------------------------------------------------------------
//���ܣ�������ִ�е��¼�����,ʹ�Լ���ͣ��s64l_uS΢����������.
//������s64l_uS,�ӳٽ���ʱ��,��λ��΢�룬�������ϵ���ΪCN_CFG_TICK_US��������
//���أ�ʵ����ʱʱ��us��
//��ע����ʱ����Ϊ˫��ѭ������
//add by lst in 20130922
//-----------------------------------------------------------------------------
u32 Djy_EventDelayTo(s64 s64l_uS)
{
    struct EventECB * event;

    if( !Djy_QuerySch())
    {
//      Djy_SaveLastError(EN_KNL_CANT_SCHED);
        Djy_DelayUs((u32)(s64l_uS - DjyGetSysTime()));
        return 0;
    }
    Int_SaveAsynSignal();
    g_ptEventRunning->delay_start_tick =DjyGetSysTick();//�趨�����ʱ��
    g_ptEventRunning->delay_end_tick =(s64l_uS +CN_CFG_TICK_US -1)/CN_CFG_TICK_US;
    if(g_ptEventRunning->delay_end_tick <= g_ptEventRunning->delay_start_tick)
    {
        Int_RestoreAsynSignal();
        return 0;
    }

    __Djy_CutReadyEvent(g_ptEventRunning);

    g_ptEventRunning->event_status = CN_STS_EVENT_DELAY;
    if(g_ptEventDelay==NULL)    //����ͬ�����п�
    {
        g_ptEventRunning->next = g_ptEventRunning;
        g_ptEventRunning->previous = g_ptEventRunning;
        g_ptEventDelay=g_ptEventRunning;
    }else
    {
        event = g_ptEventDelay;
        do
        {//��ѭ���ҵ���һ������ʱ���������¼����¼�.
            if(event->delay_end_tick <= g_ptEventRunning->delay_end_tick)
            {
                event = event->next;
            }
            else
                break;
        }while(event != g_ptEventDelay);
        //��������¼�����ǰ���ҵ����¼�ǰ�棬��û���ҵ�����event������
        //pg_event_delay������˫��ѭ�����У�g_event_delayǰ��Ҳ�͸պ��Ƕ���β��
        g_ptEventRunning->next = event;
        g_ptEventRunning->previous = event->previous;
        event->previous->next = g_ptEventRunning;
        event->previous = g_ptEventRunning;
        if(g_ptEventDelay->delay_end_tick >g_ptEventRunning->delay_end_tick)
            //���¼���ʱС��ԭ�����е���С��ʱ.
            g_ptEventDelay = g_ptEventRunning;
    }
    Int_RestoreAsynSignal();
    return (DjyGetSysTick() -g_ptEventRunning->delay_start_tick)*CN_CFG_TICK_US;
}

//----ͬ���¼�----------------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼���ͬ��������ȥ,Ȼ�����µ��ȡ���ָ���¼�
//      ������ɣ����߳�ʱʱ�䵽�������ѵ�ǰ�¼���
//����: event_id,Ŀ���¼�id��
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: CN_SYNC_SUCCESS=ͬ�������������أ�
//      CN_SYNC_TIMEOUT=��ʱ���أ�
//      EN_KNL_CANT_SCHED=��ֹ����ʱ����ִ��ͬ������
//      EN_KNL_EVENT_FREE,event_id�ǿ���id
//��ע: ����������semp_pendһ������׷����ʷ��semp_post��������ñ�����ʱ��
//      completed�Ѿ���������ֻ�ܵȵ���һ��completed�Żỽ�ѡ�
//----------------------------------------------------------------------------
u32 Djy_WaitEventCompleted(u16 event_id,u32 timeout)
{
    struct EventECB * pl_ecb;
    pl_ecb = &g_ptECB_Table[event_id];

    if (event_id == g_ptEventRunning->event_id)
        return CN_EVENT_ID_INVALID;
    if(Djy_QuerySch() == false)  //�����ڽ�ֹ���ȵ������ִ��ͬ������
        return (u32)EN_KNL_CANT_SCHED;
    if(timeout == 0)
        return (u32)CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if(pl_ecb->previous == (struct EventECB *)&s_ptEventFree)
    {//Ŀ���¼����ƿ��ǿ����¼����ƿ�
        Int_RestoreAsynSignal();
        return (u32)EN_KNL_EVENT_FREE;
    }
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;

    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������

    g_ptEventRunning->sync_head = &pl_ecb->sync;
    if(pl_ecb->sync != NULL)
    {//��ͬ���¼���ͬ�����в��ǿյ�
        g_ptEventRunning->multi_next = pl_ecb->sync;
        g_ptEventRunning->multi_previous = pl_ecb->sync->multi_previous;
        pl_ecb->sync->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->sync->multi_previous = g_ptEventRunning;
    }else
    {//��ͬ���¼���ͬ�������ǿյ�
        pl_ecb->sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }

    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVENT_DONE
                                        +CN_STS_SYNC_TIMEOUT;
        __Djy_AddToDelay(timeout);
    }
    else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVENT_DONE;
    }
    Int_RestoreAsynSignal();  //�ָ��жϻ��������µ���

    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���ء�
        return (u32)CN_SYNC_TIMEOUT;
    }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
    {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
        return (u32)EN_KNL_EVENT_SYNC_EXIT;
    }else
    {
        return (u32)CN_SYNC_SUCCESS;
    }
}

//----�¼��������ͬ��----------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼����͵�ǰͬ��������ȥ,Ȼ�����µ��ȡ����
//      ͬ����Ŀ���¼����͵���ɴ���Ϊͬ��������
//����: evtt_id,Ŀ���¼����ͺ�
//      done_times,��ɴ�����0��ʾ�����һ���������¼����.
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: CN_SYNC_SUCCESS=ͬ�������������أ�
//      CN_SYNC_TIMEOUT=��ʱ���أ�
//      EN_KNL_CANT_SCHED=��ֹ����ʱ����ִ��ͬ������
//      EN_KNL_EVTT_FREE=evtt_id�ǿ���id
//      ������������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ�����ô����ġ�
//��ע: ����������semp_pendһ������׷����ʷ��semp_post��������ñ�����ʱ��
//      completed�Ѿ���������ֻ�ܵȵ���һ��completed�Żỽ�ѡ�
//----------------------------------------------------------------------------
u32 Djy_WaitEvttCompleted(u16 evtt_id,u16 done_times,u32 timeout)
{
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb;
    u16 evttoffset;
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= gc_u32CfgEvttLimit)
        return EN_KNL_EVTTID_LIMIT;
    pl_evtt = &g_ptEvttTable[evttoffset];
    if(Djy_QuerySch() == false)  //�����ڽ�ֹ���ȵ������ִ��ͬ������
        return (u32)EN_KNL_CANT_SCHED;
    if(timeout == 0)
        return (u32)CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if((pl_evtt->property.registered == 0)     //�¼�����δע��
        ||(pl_evtt->property.deleting == 1))       //�¼��������ڵȴ�ע��
    {
        Int_RestoreAsynSignal();
        return (u32)EN_KNL_EVTT_FREE;
    }
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->sync_counter = done_times;

    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������
    g_ptEventRunning->sync_head = &pl_evtt->done_sync;
    if(pl_evtt->done_sync != NULL)
    {//��ͬ���¼����͵�ͬ�����в��ǿյ�
        pl_ecb = pl_evtt->done_sync;
        do
        {//��ѭ���ҵ���һ��ʣ����ɴ�����С�����¼����¼�.
            if(pl_ecb->sync_counter < done_times)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != pl_evtt->done_sync);
        //��������¼�(g_ptEventRunning)���뵽������
        g_ptEventRunning->multi_next = pl_ecb;
        g_ptEventRunning->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->multi_previous = g_ptEventRunning;
        if(pl_evtt->done_sync->sync_counter > done_times)
            pl_evtt->done_sync = g_ptEventRunning;
    }else
    {//��ͬ���¼����͵�ͬ�������ǿյ�
        pl_evtt->done_sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }

    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_DONE
                                         +CN_STS_SYNC_TIMEOUT;
        __Djy_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_DONE;
    }
    Int_RestoreAsynSignal();  //�ָ����Ȼ������¼��л�

    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ���ʱ����ͬ�����¼��϶���û����ɡ�
        return (u32)CN_SYNC_TIMEOUT;
    }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
    {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
        return (u32)EN_KNL_EVENT_SYNC_EXIT;
    }else
    {
        return (u32)CN_SYNC_SUCCESS;
    }
}

//----�¼����͵���ͬ��---------------------------------------------------------
//����: ���������е��¼����뵽ָ���¼����͵ĵ���ͬ��������ȥ,Ȼ�����µ��ȡ�����
//      ͬ����ָ�Ը��¼����͵��¼��������ɴ�Ϊͬ��������
//      pop_times������Ϊ����ͬ������ʼ������ͬʱ�������ͬ�������ﵽʱ��Ŀ����
//      �����͵ĵ���������
//����: evtt_id,Ŀ���¼����ͺ�
//      base_times,����������ʼֵ��Ŀ���¼��ۼƵ���*base_times+1��Ϊͬ��������
//          ͬ�������ﵽʱ������ʵ�ʵ��������������NULL,��ӵ���ʱ�ĵ�������+1
//          ��ͬ�����������ܵõ�ʵ�ʵ���������
//      timeout����ʱ����,��λ��΢�룬CN_TIMEOUT_FOREVER=���޵ȴ���0��������
//      ��ʱ���ء���0ֵ�������ϵ���ΪCN_CFG_TICK_US��������
//����: CN_SYNC_SUCCESS=ͬ�������������أ�
//      CN_SYNC_TIMEOUT=��ʱ���أ�
//      CN_SYNC_ERROR=����
//      ������������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ�����ô����ġ�
//----------------------------------------------------------------------------
u32 Djy_WaitEvttPop(u16 evtt_id,u32 *base_times, u32 timeout)
{
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb;
    u32 popt;
    u16 evttoffset;
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= gc_u32CfgEvttLimit)
        return EN_KNL_EVTTID_LIMIT;
    pl_evtt = &g_ptEvttTable[evttoffset];
    //�����ڽ�ֹ���ȵ������ִ��ͬ������
    if(Djy_QuerySch() == false)
        return CN_SYNC_ERROR;
    if(timeout == 0)
        return CN_SYNC_TIMEOUT;
    Int_SaveAsynSignal();
    if((pl_evtt->property.registered == 0)  //�¼�����δע��
        ||(pl_evtt->property.deleting == 1))   //�¼��������ڵȴ�ע��
    {
        Int_RestoreAsynSignal();
        return CN_SYNC_ERROR;
    }

    if(base_times != NULL)
        popt = *base_times - pl_evtt->pop_times +1;
    else
        popt = 1;
    //����жϺ���Ҫ����������˵�����ñ�����ʱ���ȴ������Ѿ�������Ӧ������������
    //���أ���Ȼ��û��û�˵صȡ�
    if((popt >= 0x80000000) || (popt == 0))
    {
        if(base_times != NULL)
            *base_times = pl_evtt->pop_times;
        Int_RestoreAsynSignal();
        return CN_SYNC_SUCCESS;
    }
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->next = NULL;
    g_ptEventRunning->previous = NULL;
    g_ptEventRunning->sync_counter = popt;
    //���°�pg_event_running���뵽Ŀ���¼���ͬ��������
    g_ptEventRunning->sync_head = &pl_evtt->pop_sync;
    if(pl_evtt->pop_sync != NULL)
    {//��ͬ���¼����͵�ͬ�����в��ǿյ�
        pl_ecb = pl_evtt->pop_sync;
        do
        {//��ѭ���ҵ���һ��ʣ�൯��������С�����¼����¼�.
            if(pl_ecb->sync_counter < g_ptEventRunning->sync_counter)
                pl_ecb = pl_ecb->multi_next;
            else
                break;
        }while(pl_ecb != pl_evtt->pop_sync);
        //��������¼�(g_ptEventRunning)���뵽������
        g_ptEventRunning->multi_next = pl_ecb;
        g_ptEventRunning->multi_previous = pl_ecb->multi_previous;
        pl_ecb->multi_previous->multi_next = g_ptEventRunning;
        pl_ecb->multi_previous = g_ptEventRunning;
        if(pl_evtt->pop_sync->sync_counter > g_ptEventRunning->sync_counter)
            pl_evtt->pop_sync = g_ptEventRunning;
    }else
    {//��ͬ���¼���ͬ�������ǿյ�
        pl_evtt->pop_sync = g_ptEventRunning;
        g_ptEventRunning->multi_next = g_ptEventRunning;
        g_ptEventRunning->multi_previous = g_ptEventRunning;
    }
    if(timeout != CN_TIMEOUT_FOREVER)
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_POP
                                         + CN_STS_SYNC_TIMEOUT;
        __Djy_AddToDelay(timeout);
    }else
    {
        g_ptEventRunning->event_status = CN_STS_WAIT_EVTT_POP;
    }

    Int_RestoreAsynSignal();  //�ָ��жϻ��������µ���
    if(base_times != NULL)
        *base_times = pl_evtt->pop_times;

    //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
    if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
    {//˵��ͬ������δ�����ӳ�ʱ���أ���ʱ��ͬ�������϶���û�д�ɡ�
        return CN_SYNC_TIMEOUT;
    }else
    {
        return CN_SYNC_SUCCESS;
    }
}

//----�����¼�-----------------------------------------------------------------
//���ܣ������ϵͳ���淢���¼�,���Դ�һ���������������ṩ�����¼�����Ҫ�ĸ�����
//      Ϣ������ϵͳ�ӱ���,�������¼����ƿ飬���߰Ѳ������뵽�Ѵ����¼��Ĳ�����
//      ����.
//
//������hybrid_id��id���������¼�����id��Ҳ�������¼�id��
//          ������¼�id��ϵͳ����Ҹ��¼���Ӧ���¼�����id��
//          ����ǹ���/�������¼����ͣ����¼�id���¼�����id�ǵ�Ч�ġ�
//          ����Ƕ���/�������¼����ͣ������hybrid_id�����id�������ֱ���:
//              1���¼�����id�����·���һ���¼����Ϳ��ƿ飬�����¼�ʱ���������/
//                 ����һ���µ��̡߳�
//              2���¼�id����ֻ�ǰ��²��������¼��Ĳ��������С�
//      timeout,���峬ʱʱ�䣬���=0���򵯳��¼����������أ���ʱ�����¼�����
//          ��������������!=0����ȴ��¼�������ɺ�ŷ��أ�������pop_result�з�
//          �ش���������λ��us
//      pop_result,�¼���������״̬��������������˺Ϸ����¼�id��
//          ��timeout !=0����
//              pop_result = cn_sync_success����ʾ�¼���������ɺ󷵻�
//              pop_result = cn_sync_timeout����ʾ�¼�δ�������꣬��ʱ����
//              pop_result = enum_knl_event_sync_exit���¼������쳣��ֹ
//              ���timeout ==0����pop_result�����塣
//
//          �������������cn_invalid_event_id���򷵻ؾ���ĳ�����Ϣ
//      PopPrarm1��PopPrarm2�����ݸ��¼��Ĳ�����������������������¼����¼���
//          �ƿ������Ǳ������һ�ε����Ĳ�����
//      prio,�¼����ȼ�,������Ҫ�������¼��������0��ʾ���¼�ʹ��Ĭ��ֵ(�����¼�
//          ���Ϳ��ƿ���)��
//          �������贴�����¼�����������������:
//              1�����û��Я�������������κδ�������ı��Ѿ������¼����е�
//                 �¼������ȼ���
//              2�����Я��������prio�������������ȼ����²�������task_low_prio��
//                 �У�prio�����������ȼ����²�������task_high_prio���У���Խ��
//                 ���ȼ�Խ�͡�
//���أ������ܻ�ȡ�¼����ƿ飬����cn_invalid_event_id�����򷵻��¼�id��
//ע: �������ܻ���¼����ƿ��������
//-----------------------------------------------------------------------------
u16 Djy_EventPop(   u16  hybrid_id,
                    u32 *pop_result,
                    u32 timeout,    //������������峬ʱʱ�䣬
                    ptu32_t PopPrarm1,
                    ptu32_t PopPrarm2,
                    ufast_t prio)
{
    struct ExpThrowPara  parahead;
    char ExpStr[32];
    struct EventECB *pl_ecb;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb_temp;
    u16 evtt_offset;
    u16 return_result;
    bool_t schbak;          //�Ƿ��������
    if(hybrid_id >= CN_EVTT_ID_BASE)
    {
        evtt_offset = hybrid_id & (~CN_EVTT_ID_MASK);
        if(evtt_offset >= gc_u32CfgEvttLimit)
        {
            Djy_SaveLastError(EN_KNL_EVTTID_LIMIT);
            if(pop_result != NULL)
                *pop_result = (ptu32_t)EN_KNL_EVTTID_LIMIT;
            return CN_EVENT_ID_INVALID;
        }
    }
    else
    {
        if(hybrid_id >= gc_u32CfgEventLimit)
        {
            Djy_SaveLastError(EN_KNL_EVENT_FREE);
            if(pop_result != NULL)
                *pop_result = (ptu32_t)EN_KNL_EVENT_FREE;
            return CN_EVENT_ID_INVALID;
        }
        else
        {
            evtt_offset = g_ptECB_Table[hybrid_id].evtt_id & (~CN_EVTT_ID_MASK);
        }
    }
    schbak = Djy_QuerySch( );
    if((timeout != 0) && (!schbak))
    {   //Ҫ���¼�������ŷ��أ�����������ȶ��޷�ִ��
        Djy_SaveLastError(EN_KNL_CANT_SCHED);
        if(pop_result != NULL)
            *pop_result = (ptu32_t)EN_KNL_CANT_SCHED;
        return CN_EVENT_ID_INVALID;
    }
    pl_evtt =&g_ptEvttTable[evtt_offset];
    if((pl_evtt->property.registered == 0)      //����δ�Ǽ�
        ||(pl_evtt->property.deleting == 1))    //�¼��������ڵȴ�ע��
    {
        Djy_SaveLastError(EN_KNL_EVTT_UNREGISTER);
        if(pop_result != NULL)
            *pop_result = (u32)EN_KNL_EVTT_UNREGISTER;
        return CN_EVENT_ID_INVALID;
    }else if(prio >= CN_PRIO_SYS_SERVICE)       //�Ƿ����ȼ�
    {
        Djy_SaveLastError(EN_KNL_INVALID_PRIO);
        if(pop_result != NULL)
            *pop_result = (u32)EN_KNL_INVALID_PRIO;
        return CN_EVENT_ID_INVALID;
    }

//    malloc_para = para_options && CN_MALLOC_OVER_32;
    Int_SaveAsynSignal();                     //���첽�ź�(�ص���)
    //���洦���¼����͵���ͬ�����ο�Djy_WaitEvttPop��˵��
    //����ͬ���е��¼�������ʽ����:
    //1������ǹ������¼����򼤻�����sync_counter����0���¼���
    //2������Ƕ������¼�����Ҫ���������:
    //  a�����hybrid_id���¼�id���򲻼���ͬ���͵������¼���
    //  b�����hybrid_id���¼�����id���򼤻�����sync_counter����0���¼���
    pl_ecb = pl_evtt->pop_sync;
    if(pl_ecb != NULL)
    {
        do
        {
            //sync_counter����������:
            //1���������¼�������:
            //2���������¼���hybrid_id���¼�id���ҵ���pl_ecb->event_id������:
            //3���������¼��������ͺ�pl_ecb->evtt_id��ͬ
            if( (pl_evtt->property.correlative == EN_CORRELATIVE)
                    || (pl_ecb->event_id == hybrid_id)
                    || ((evtt_offset|CN_EVTT_ID_MASK) != pl_ecb->evtt_id) )
            {
                //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
                if(pl_ecb->sync_counter <= 1)
                {
                    pl_ecb->sync_head = NULL;
                    //ָ���ĳ�ʱδ��
                    if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)
                    {
                        __Djy_ResumeDelay(pl_ecb);     //�����Ӷ������Ƴ��¼�
                    }
                    pl_ecb->wakeup_from = CN_STS_WAIT_EVTT_POP;
                    pl_ecb->event_status = CN_STS_EVENT_READY;
                    pl_ecb_temp = pl_ecb;
                    if(pl_ecb_temp == pl_ecb_temp->multi_next)  //�����һ���¼�
                    {
                        pl_ecb = NULL;
                        pl_evtt->pop_sync = NULL;
                    }else
                    {
                        pl_ecb = pl_ecb->multi_next;
                        pl_ecb_temp->multi_previous->multi_next
                                            = pl_ecb_temp->multi_next;
                        pl_ecb_temp->multi_next->multi_previous
                                            = pl_ecb_temp->multi_previous;
                    }
                    __Djy_EventReady(pl_ecb_temp);
//                  if(pl_evtt->pop_sync != NULL)
//                      pl_evtt = NULL;
                }else
                {
                    pl_ecb->sync_counter--;
                    pl_ecb = pl_ecb->multi_next;
                }
            }
            else
            {
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->pop_sync);
    }

    //��Ҫ�������¼������Ϊ����֮һ:
    //1���¼����������޸������¼���
    //2���������¼���hybrid_id���¼�����id
    if((pl_evtt->property.inuse == 0)
                || ((pl_evtt->property.correlative == EN_INDEPENDENCE)
                    && (hybrid_id & CN_EVTT_ID_MASK)) )
    {
        if(s_ptEventFree==NULL)            //û�п��е��¼����ƿ�
        {
            parahead.DecoderName = NULL;
            parahead.ExpAction = EN_EXP_DEAL_RECORD;
            itoa(hybrid_id,ExpStr,16);
            strcat(ExpStr,": ���䲻���¼����ƿ�");
            parahead.ExpInfo = (u8*)ExpStr;
            parahead.ExpInfoLen = sizeof(ExpStr);
            parahead.ExpType = CN_EXP_TYPE_ECB_EXHAUSTED;
            Exp_Throw(&parahead);
            Djy_SaveLastError(EN_KNL_ECB_EXHAUSTED);
            if(pop_result != NULL)
                *pop_result = (u32)EN_KNL_ECB_EXHAUSTED;
            return_result = CN_EVENT_ID_INVALID;
            goto end_pop;
        }else if((pl_evtt->property.correlative == EN_INDEPENDENCE)
                && ((pl_evtt->vpus)> pl_evtt->vpus_limit))
        {
            Djy_SaveLastError(EN_KNL_VPU_OVER);
            return_result = CN_EVENT_ID_INVALID;
            if(pop_result != NULL)
                *pop_result = (u32)EN_KNL_VPU_OVER;
            return_result = CN_EVENT_ID_INVALID;
            goto end_pop;
        }else                       //�п����¼����ƿ�
        {
            pl_ecb = s_ptEventFree;         //�ӿ�����������ȡһ���¼����ƿ�
            s_ptEventFree=s_ptEventFree->next;  //�����¼����ƿ�������1
            pl_evtt->events++;
            //�������¼����¼����ƿ�
            pl_ecb->next = NULL;
            pl_ecb->previous = NULL;
            pl_ecb->multi_next = NULL;
            pl_ecb->multi_previous = NULL;
            pl_ecb->vm = NULL;
            pl_ecb->param1 = PopPrarm1;
            pl_ecb->param2 = PopPrarm2;
            pl_ecb->sync = NULL;
            pl_ecb->sync_head = NULL;

            pl_ecb->EventStartTime = DjyGetSysTime();   //�¼�����ʱ��
            pl_ecb->consumed_time = 0;
//#if(CN_CFG_DEBUG_INFO == 1)
            pl_ecb->consumed_time_second = 0;
            pl_ecb->consumed_time_record = 0;
//#endif
            pl_ecb->delay_start_tick = 0;
            pl_ecb->delay_end_tick = 0;
            pl_ecb->error_no = 0;
            pl_ecb->wait_mem_size = 0;
            pl_ecb->wakeup_from = CN_WF_EVENT_NORUN;
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->evtt_id = evtt_offset | CN_EVTT_ID_MASK;    //�����¼�����
            pl_ecb->sync_counter = 0;
            pl_ecb->local_memory = 0;
            pl_evtt->property.inuse = 1;
            if(pl_evtt->property.correlative == EN_CORRELATIVE)
            {
                pl_evtt->mark_event = pl_ecb;
            }
            else
            {

            }
            if(prio != 0)
            {
                pl_ecb->prio = prio;                    //�����¼����ȼ�,
                pl_ecb->prio_base = pl_ecb->prio;       //�����¼����ȼ�,
            }else
            {
                pl_ecb->prio =pl_evtt->default_prio;    //���¼������м̳����ȼ�
                pl_ecb->prio_base = pl_ecb->prio;       //�����¼����ȼ�,
            }
            __Djy_EventReady(pl_ecb);
            return_result = pl_ecb->event_id;
        }
    }
    else  //�������¼����Ҷ������Ѿ��и������¼� �� ��������hybrid_id���¼�id
    {
        if(pl_evtt->property.correlative == EN_INDEPENDENCE)
        {
            //��ʱhybrid_idһ�����¼�ID,��mark_event����ָ����Ӧ�¼�
            pl_ecb = &g_ptECB_Table[hybrid_id];
        }
        else
        {
            pl_ecb = pl_evtt->mark_event;
        }
        pl_ecb->param1 = PopPrarm1;     //�����滻
        pl_ecb->param2 = PopPrarm2;
        return_result = pl_ecb->event_id;

    }
    //�趨�˳�ʱʱ�䣬�����µ���
    //��������¼�ͬ�����У��¼���������Ȼ���ػ��쳣�˳��Ž������
    pl_evtt->pop_times++;
    if(timeout != 0)
    {
        __Djy_AddRunningToBlock(&pl_ecb->sync, CN_BLOCK_FIFO, timeout, CN_STS_WAIT_EVENT_DONE);
        Int_RestoreAsynSignal();  //�ָ��жϻ��������µ���

        //ע:�¼���������djy_task_completed�����и�event_result��ֵ
        //�������ﷵ�أ��ǳ�ʱ����ͬ���¼���ɡ�
        if(g_ptEventRunning->wakeup_from & CN_STS_SYNC_TIMEOUT)
        {//˵��ͬ������δ�����ӳ�ʱ���ء�
            if(pop_result != NULL)
                *pop_result = (u32)CN_SYNC_TIMEOUT;
        }else if(g_ptEventRunning->wakeup_from & CN_STS_EVENT_EXP_EXIT)
        {//��ͬ���¼����쳣��ֹ������û�б���ȷִ��
            if(pop_result != NULL)
                *pop_result = (u32)CN_STS_EVENT_EXP_EXIT;
        }else
        {
            if(pop_result != NULL)
                *pop_result = (u32)CN_SYNC_SUCCESS;
        }
    }
end_pop:
    Int_RestoreAsynSignal();  //�ָ��ж�״̬
    return return_result;
}

//----ȡ�¼����͵�������-------------------------------------------------------
//����: ȡ��ǰĳ�����¼������ܴ���
//����: evtt_id���¼�����id
//����: �¼���������
//-----------------------------------------------------------------------------
u32 Djy_GetEvttPopTimes(u16 evtt_id)
{
    return g_ptEvttTable[evtt_id &(~CN_EVTT_ID_MASK)].pop_times;
}

//----ȡ�¼�������-----------------------------------------------------------
//����: һ���¼��ڴ�������У�������������¼������ҵȴ��¼�������(����
//      djy_event_pop������sync=true)���������djy_event_popʱ�����˺Ϸ����¼�
//      id���Ҳ��ǳ�ʱ���أ�������ñ�������ȡ���¼��Ĵ�������ֻ��ȡ���һ��
//      �ɹ�������¼������
//����: ��
//����: �¼���������
//-----------------------------------------------------------------------------
ptu32_t Djy_GetEventResult(void)
{
    return g_ptEventRunning->event_result;
}

//----��ȡ�¼�����-------------------------------------------------------------
//����: ��ȡ�������¼��Ĳ���������˳��:1�������ȼ��Ķ��зǿգ�ȡ��һ��
//      �������Ӷ�����ɾ����2��pparaָ��ǿ�,ȡ��ֵ����ա�3�������ȼ����зǿգ�
//      ȡ��һ�����Ӷ���ɾ��
//����: time�������NULL���򷵻�����ȡ�Ĳ���������е�ʱ�䣬uS��
//����: �¼�����ָ��
//ע: ������ֻ��ȡ���ڴ����е��¼��Լ��Ĳ���
//-----------------------------------------------------------------------------
void Djy_GetEventPara(ptu32_t *Param1,ptu32_t *Param2)
{
    if(Param1 != NULL)
        *Param1 = g_ptEventRunning->param1;
    if(Param2 != NULL)
        *Param2 = g_ptEventRunning->param2;
}

//----ȡ������¼�����id-------------------------------------------------------
//����: ��Ӧ�ó�����ã�ȡ���ڴ�����¼����¼�����id
//����: ��
//����: �¼�����id�������ǰ�������첽�ź��ж��У��򷵻� CN_EVTT_ID_ASYN
//-----------------------------------------------------------------------------
u16 Djy_MyEvttId(void)
{
    if(Int_GetRunLevel() >0)
    {
        return CN_EVTT_ID_ASYN;
    }
    else
    {
        return g_ptEventRunning->evtt_id;
    }
}

//----ȡ������¼�id-------------------------------------------------------
//����: ��Ӧ�ó�����ã�ȡ���ڴ�����¼����¼�id
//����: ��
//����: �¼�id�������ǰ�������첽�ź��ж��У��򷵻� CN_EVENT_ID_ASYN
//-----------------------------------------------------------------------------
u16 Djy_MyEventId(void)
{
    if(Int_GetRunLevel() >0)
    {
        return CN_EVENT_ID_ASYN;
    }
    else
    {
        return g_ptEventRunning->event_id;
    }
}

//----��������ϵͳ--------------------------------------------------------------
//����: ��ʼ��ʱ����ણ���λ�첽�ź�ʹ��״̬��ѡ��������ȼ��¼�������
//      __asm_turnto_context���������л�������¼�
//����: ��
//����: ��
//_____________________________________________________________________________
void __Djy_StartOs(void)
{
    //����Ϊ�ݴ����ʣ��Է��û�ģ���ʼ��������û�гɶԵ����첽�ź�ʹ�����ֹ����
    __Int_ResetAsynSignal();
    __DjyInitTick();
    __Djy_SelectEventToRun();
    g_ptEventRunning = g_ptEventReady;
    g_bScheduleEnable = true;
    g_bMultiEventStarted = true;
    Int_HalfEnableAsynSignal( );
    __asm_start_thread(g_ptEventRunning->vm);
}

#define CN_KEEP         0   //�̱߳�����ɾ��
#define CN_DELETE       1   //�߳�Ӧ�ñ�ɾ��
#define CN_DELETED      2   //�߳��Ѿ���ɾ��

//----�����¼���ͬ������---------------------------------------------------
//����: �¼��������ʱ��������¼�ͬ�������е������¼�������
//      ��ͬ���е��¼���"ִ�н��"����Ϊ0����ͬ���¼��쳣����λ����λ
//����: para���������еĶ���ͷ����������һ��˫������
//����: ��
//-----------------------------------------------------------------------------
void __Djy_ActiveEventSyncExp(struct EventECB *sync)
{
    struct EventECB *event_temp;
    if(sync == NULL)
        return ;

    sync->multi_previous->multi_next = NULL;
    do{
        sync->event_result = 0;
        sync->sync_head = NULL;
        //�Ƿ��ڳ�ʱ������
        if(sync->event_status & CN_STS_SYNC_TIMEOUT)
        {
            __Djy_ResumeDelay(sync);                   //������ʱ�ȴ�
        }
        sync->event_status = CN_STS_EVENT_READY;        //ȡ��"ͬ����"״̬
        sync->wakeup_from = CN_STS_EVENT_EXP_EXIT;      //��λ�쳣������־
        event_temp = sync->multi_next;
        __Djy_EventReady(sync);           //���¼����뵽����������
        sync = event_temp;
    }while(sync != NULL);
    return;
}

//----�쳣�����¼�������ͬ������-----------------------------------------------
//����: �¼��쳣����ʱ������һ���¼����в�����ͬ�������е������¼������ͷŲ�����
//      �ƿ�.���ظ�ͬ���е��¼���"ִ�н��"����Ϊ0����ͬ���¼��쳣����λ����λ
//����: para���������еĶ���ͷ����������һ��˫������
//����: ��
//-----------------------------------------------------------------------------
//void __Djy_ActiveEventParaSyncExp(struct ParaPCB *event_para)
//{
//    struct ParaPCB *para_temp;
//    struct EventECB *pl_ecb,*event_temp;
//    if(event_para == NULL)
//        return ;
//
//    event_para->previous->next = NULL;
//    do{
//        pl_ecb = event_para->sync;
//        if(pl_ecb != NULL)
//        {
//            pl_ecb->multi_previous->multi_next = NULL;
//            do{
//                pl_ecb->event_result = 0;
//                pl_ecb->sync_head = NULL;
//                //�Ƿ��ڳ�ʱ������
//                if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)
//                {
//                    __Djy_ResumeDelay(pl_ecb);               //������ʱ�ȴ�
//                }
//                pl_ecb->event_status = CN_STS_EVENT_READY;    //ȡ��"ͬ����"״̬
//                pl_ecb->wakeup_from = CN_STS_EVENT_EXP_EXIT;  //��λ�쳣������־
//                event_temp = pl_ecb->multi_next;
//                __Djy_EventReady(pl_ecb);           //���¼����뵽����������
//                pl_ecb = event_temp;
//            }while(pl_ecb != NULL);
//        }
//
//        if(event_para->dynamic_mem == true)
//        {
//            free(event_para->event_para);
//        }
//        para_temp = event_para;
//        event_para = event_para->next;                  //ȡ��������һ���¼�
//        para_temp->sync = NULL;
//        para_temp->next = s_ptParaFree;     //�ͷŲ������ƿ�
//        s_ptParaFree = para_temp;
//    }while(event_para != NULL);
//    return;
//}

//----��ͬ���������Ƴ���ECB-----------------------------------------------
//����: //�˺��������κεļ�飬�ڲ����ã�ȷ�ϴ�ecb��sync������
//����: ��
//      ��ͬ�������Ƕ๦��ָ���˫��ѭ������
//-----------------------------------------------------------------------------
void __Djy_CutEcbFromSync(struct EventECB  *event)
{
    struct EventECB  **syn_head;

    syn_head = event->sync_head;
    if(NULL == syn_head)
    {
        return;
    }
    if(event->multi_next == event)
    {//�����һ���¼�
        *syn_head  = NULL;
    }else
    {
        if(*syn_head == event)  //���Ƴ����Ƕ���ͷ
            *syn_head = event->multi_next;
        event->multi_next->multi_previous = event->multi_previous;
        event->multi_previous->multi_next = event->multi_next;
        event->multi_next = NULL;
        event->multi_previous = NULL;
    }
    event->sync_head = NULL;
}

//----��ͬ���������Ƴ���ECB-----------------------------------------------
//����: ��event ��ECB��Ӧ��EVTT��marked������ɾ����Ӧ��ȷ��event��marked������
//����: ��
//      �ö�����ǰ��ָ���˫��ѭ������,˳���ǰ������ȼ��ߵ�˳��
//      �����ǹ����Դ��룬������ȫ���
//-----------------------------------------------------------------------------
void __Djy_CutEventFromEvttMarked(struct EventECB *event)
{
    struct EventType  *pl_evtt;

    pl_evtt =& g_ptEvttTable[event->evtt_id &(~CN_EVTT_ID_MASK)];
    if(event == event->previous)//maked������ֻ��һ��
    {
        pl_evtt->mark_event = NULL;
    }
    else//��marked�������Ƴ�
    {
        if(event == pl_evtt->mark_event)//����ͷ����ʵһ��Ķ��Ǵ�ͷ����
        {
            pl_evtt->mark_event = event->next;
        }
        event->previous->next = event->next;
        event->next->previous = event->previous;
    }
}

//----�¼��ݳ�-----------------------------------------------------------------
//����: �¼��������쳣����,һ���ڿ��Ź��ȼ��ӻ��Ƽ�쵽ϵͳ����ʱ�����ƺ���
//      ��ɾ���¼����������߳����У���λ�߳����¿�ʼ���в����߳����³�ʼ����
//����: event_ECB��Ŀ���¼�
//      exit_code��������
//      action�������Ķ���
//����: ������������
//todo: δ���
//-----------------------------------------------------------------------------
void Djy_EventExit(struct EventECB *event, u32 exit_code,u32 action)
{
    struct ThreadVm *next_vm,*temp;
    struct EventECB *pl_ecb;
    struct EventType *pl_evtt;
    struct EventECB *pl_ecb_temp;
    struct ExpThrowPara  parahead;
    char ExpStr[32];
    ucpu_t  vm_final = CN_DELETE;

    parahead.DecoderName = NULL;
    parahead.ExpAction = EN_EXP_DEAL_RECORD;
    itoa(event->event_id,ExpStr,16);
    strcat(ExpStr,"�¼������������");
    parahead.ExpInfo = (u8*)ExpStr;
    parahead.ExpInfoLen = sizeof(ExpStr);
    parahead.ExpType = CN_EXP_TYPE_EVENT_EXIT;
    Exp_Throw(&parahead);
    //�˴�����int_save_asyn_signal������������Ӧ�ó�����bug��û�гɶԵ���
    //int_save_asyn_signal��int_restore_asyn_signal������£�ȷ�����󵽴�Ϊֹ��
    __Int_ResetAsynSignal();  //ֱ��__vm_engine�������ٴδ�.

    //�����¼�ͬ�������е������¼�
    __Djy_ActiveEventSyncExp(g_ptEventRunning->sync);

//    if(g_ptEventRunning->para_current != NULL)
//        __Djy_ActiveEventSyncExp(g_ptEventRunning->para_current->sync);
    //���¼��������ͬ�������е��¼�,���ͷŲ��������е�ȫ������
//  __Djy_ActiveEventParaSyncExp(g_ptEventRunning->para_high_prio);
//  __Djy_ActiveEventParaSyncExp(g_ptEventRunning->para_low_prio);

#if (CN_CFG_DYNAMIC_MEM == 1)
    if(g_ptEventRunning->local_memory != 0)
    {
        __M_CleanUp(g_ptEventRunning->event_id);    //
    }
#endif
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous
                    = (struct EventECB*)&s_ptEventFree;//��ʾ�����ƿ����
    g_ptEventRunning->next = s_ptEventFree;     //pg_event_free�ǵ����ѭ������
    g_ptEventRunning->evtt_id = CN_EVTT_ID_INVALID;     //todo
    s_ptEventFree = g_ptEventRunning;
    s_ptEventFree->event_id = s_ptEventFree - g_ptECB_Table;   //�ݴ���

    //���洦�����ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_evtt =&g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)];
    pl_evtt->events--;
    pl_ecb = pl_evtt->done_sync;
    if(pl_ecb != NULL)
    {
        do
        {//�����е��¼�����Ҫôû��ָ����ʱ��Ҫôʱ��δ����������������ڴ�����
         //�����ºۼ���evtt_done_syncλҲ�Ѿ����

            //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
            //2��ͬ��������Ϊ0�ұ��������һ���¼��Ѿ�������
            if((pl_ecb->sync_counter == 1)
                    ||((pl_ecb->sync_counter == 0) && (pl_evtt->vpus == 1)) )
            {
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)//ָ���ĳ�ʱδ��
                {
                    __Djy_ResumeDelay(pl_ecb);       //�����Ӷ������Ƴ��¼�
                }
                pl_ecb->event_status = CN_STS_EVENT_READY;
                pl_ecb->wakeup_from = CN_STS_EVENT_EXP_EXIT;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //�����һ���¼�
                {
                    pl_ecb = NULL;
                    pl_evtt->done_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous
                                        = pl_ecb_temp->multi_previous;
                }
                __Djy_EventReady(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //�̺߳��¼����ƿ�Ĵ���ʽ
    //1�����߳�����С�ڵ���vpus_res������.
    //2����������������߳�
    //3���¼����ƿ���ֱ���ջ�

    if(pl_evtt->property.correlative == EN_CORRELATIVE)      //�������¼�
    {
        pl_evtt->property.inuse = 0;
        if(pl_evtt->default_prio<0x80)
        {//����ģʽΪsi��dlsp�����߸��¼�����ӵ�н������ȼ�ʱ��
         //�뱣���߳�
            vm_final = CN_KEEP;
        }else
        {
            vm_final = CN_DELETE;
        }
    }else   //�������¼�
    {
        //��δ�õ��̵߳��¼�������֮
        if(pl_evtt->events >= pl_evtt->vpus)
        {
            vm_final = CN_KEEP;
        }else   //û��δ�õ��̵߳��¼����ٿ��Ƿ��豣��
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)//�������¼�ӵ�е��߳����Ѿ�����
                                                    //�������ͱ�����������֮
            {
                vm_final = CN_KEEP;
            }else                                   //����ɾ���߳�
            {
                if(pl_evtt->vpus == 1)   //�������һ���¼�
                {
                    pl_evtt->property.inuse = 0;
                    if(pl_evtt->default_prio<0x80)
                    {
                 //����ģʽΪsi��dlsp������¼�����ӵ�н������ȼ�ʱ���뱣���߳�
                        vm_final = CN_KEEP;
                    }else
                    {
                        vm_final = CN_DELETE;
                    }
                }else
                {
                    vm_final = CN_DELETE;
                }
            }
        }
    }

//���¼����Ϳ��ƿ��Ƿ�Ҫɾ�����û�ע��һ���¼����ͣ�������¼���������ʹ�ã���
//������ע����ֻ�ǲ��ܵ������¼���Ҫ�����ڸ��¼����͵������¼����������ע����
    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //����¼�������
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ����߳�
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
        vm_final = CN_DELETED;          //�߳��Ѿ���ɾ��
    }

    __Djy_SelectEventToRun();
    if(vm_final == CN_DELETE)                   //ɾ���߳�
    {
        free((void*)g_ptEventRunning->vm);    //ɾ���߳�
        pl_evtt->vpus--;
        g_ptEventRunning = g_ptEventReady;
        g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DjyGetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }else if(vm_final == CN_KEEP)    //�����߳�
    {
        //���¼������еȴ���VM��ECB,��vmת������Ӧ�����е�ECB�Ϳ��ԡ�
        if((pl_evtt->property.correlative == EN_INDEPENDENCE)&&\
                (pl_evtt->mark_event != NULL))
        {
            pl_ecb = pl_evtt->mark_event;
            if(pl_evtt->mark_event->previous == pl_evtt->mark_event)//ֻ��һ��
            {
                pl_evtt->mark_event = NULL;
            }
            else
            {
                pl_ecb->previous->next = pl_ecb->next;
                pl_ecb->next->previous = pl_ecb->previous;
                pl_evtt->mark_event = pl_ecb->next;
            }
            //��ͬ���������Ƴ�,���Կ϶������ڴ�ͬ������
            if(NULL != pl_ecb->sync_head)
            {
                __Djy_CutEcbFromSync(pl_ecb);
            }
            //���ܾ��̵߳�VM
            pl_ecb->vm = g_ptEventRunning->vm;
            __Djy_EventReady(pl_ecb);
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);

            g_ptEventRunning=g_ptEventReady;
            g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DjyGetSysTime();
            //����һ��ܹؼ���running��pl_ecb������ͬһ��������һ��Ҫ��reset��Ȼ
            //��switch
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
        else//��������»��Ƿ���free���а�
        {
            g_ptEventRunning->vm->next = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = g_ptEventRunning->vm;
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);

            g_ptEventRunning=g_ptEventReady;
            g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DjyGetSysTime();
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
    }else               //�߳��Ѿ���ɾ��
    {
//        pl_ecb = g_ptEventRunning;
        g_ptEventRunning = g_ptEventReady;
        g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DjyGetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }
}

//----�¼��׶������----------------------------------------------------------
//���ܣ��������һ��EventPop,���¼�δ��ֹʱ,���ñ�������ʾ�ô�EventPop�Ѿ����,
//      �������¼�ͬ�������е��¼�.һ�����ڵ���Djy_WaitEvttPop�ҷ���
//      CN_SYNC_SUCCESS��,����֪ͨ����Djy_EventPop���¼�"�ôε����Ѿ�������".
//      ����ô�Djy_EventPop�趨��ͬ��(��timeout !=0),��ͬ��������ɡ�
//
//      �����������,����������������:
//      A�¼�����Djy_EventPop����C�¼�,timeout = 0,��C������,δ�л���C.
//      B�¼�����Djy_EventPop����C�¼�,timeout = ����,�л���C.
//      C ����Djy_EventSessionComplete,BҲ������.
//      ���������,������ΪC�õ�������Djy_EventSessionComplete�ֱ��Ӧ��
//      �ε���,�ڶ��ε���ʱ���ܼ���B.��Ϊ,�������¼�����ɵ�,�������һ��
//      ����,��Ϊǰ�󵯳����й�����.
//
//������result���¼��������������������ظ��������¼����¼�(����趨��ͬ��)
//���أ���
//-----------------------------------------------------------------------------
void Djy_EventSessionComplete(ptu32_t result)
{
    struct EventECB *pl_ecb,*event_temp;
    Int_SaveAsynSignal();
    pl_ecb = g_ptEventRunning->sync;     //ȡͬ������ͷ
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //�Ƿ��ڳ�ʱ������
            {
                __Djy_ResumeDelay(pl_ecb);                 //������ʱ�ȴ�
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVENT_DONE;
            event_temp = pl_ecb->multi_next;
            __Djy_EventReady(pl_ecb);           //���¼����뵽����������
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }
    g_ptEventRunning->sync = NULL;
    Int_RestoreAsynSignal();

}

//----�¼����-----------------------------------------------------------------
//���ܣ��¼���������Ȼ����,���������.
//      1.����¼�ͬ�����зǿգ���ͬ���¼��ŵ�ready��
//      2.�����¼��������ͬ������
//      3.���δ�ͷŵĶ�̬�����ڴ棬�ͷ�֮��
//      4.���¼����ƿ���������õ���ʼ̬,������free��
//  ������߳��豣������:
//      5.��λ�̡߳�
//      6.�����������ģ�ֱ���л����µ�ready��ͷ
//  ����߳����豣��
//      5.�ͷ��߳��ڴ档
//      6.�����������ģ�ֱ���л����µ�ready��ͷ
//������result���¼��������������������ظ��������¼����¼�(����趨��ͬ��)
//���أ���
//��ע: ���ñ������ıض���running�¼�,��running�¼���������ִ�У������Ե���
//      __asm_reset_thread������
//-----------------------------------------------------------------------------
void Djy_EventComplete(ptu32_t result)
{
    struct ThreadVm *next_vm,*temp;
    struct EventECB *pl_ecb,*event_temp;
    struct EventType   *pl_evtt;
    struct EventECB *pl_ecb_temp;
    ucpu_t  vm_final = CN_DELETE;

    pl_evtt =&g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)];
    __Int_ResetAsynSignal();  //ֱ��__vm_engine�������ٴδ�.
    //���洦��ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_ecb = g_ptEventRunning->sync;     //ȡͬ������ͷ
    if(pl_ecb != NULL)
    {
        pl_ecb->multi_previous->multi_next = NULL;
        do{
            pl_ecb->sync_head = NULL;
            pl_ecb->event_result = result;
            if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT)  //�Ƿ��ڳ�ʱ������
            {
                __Djy_ResumeDelay(pl_ecb);                 //������ʱ�ȴ�
            }
            pl_ecb->event_status = CN_STS_EVENT_READY;
            pl_ecb->wakeup_from = CN_STS_WAIT_EVENT_DONE;
            event_temp = pl_ecb->multi_next;
            __Djy_EventReady(pl_ecb);           //���¼����뵽����������
            pl_ecb = event_temp;
        }while(pl_ecb != NULL);
    }

    //���洦�����ͬ�����У�������ϵ�����ĵ�������½ڲ����׶�����ע�����ڽ���
    //��ô����ġ�
    pl_ecb = pl_evtt->done_sync;
    while(pl_ecb != NULL)
    {
        do
        {//�����е��¼�����Ҫôû��ָ����ʱ��Ҫôʱ��δ����������������ڴ�����
         //�����ºۼ���evtt_done_syncλҲ�Ѿ����

            //ͬ��������ɵ�����: 1��ͬ���������Ѽ���1��
            //2��ͬ��������Ϊ0�ұ��������һ���¼��Ѿ�������
         if((pl_ecb->sync_counter == 1)
             ||((pl_ecb->sync_counter == 0) && (pl_evtt->vpus == 1)) )
            {
                pl_ecb->sync_head = NULL;
                if(pl_ecb->event_status & CN_STS_SYNC_TIMEOUT) //ָ���ĳ�ʱδ��
                {
                    __Djy_ResumeDelay(pl_ecb);       //�����Ӷ������Ƴ��¼�
                }
                pl_ecb->event_status = CN_STS_EVENT_READY;
                pl_ecb->wakeup_from = CN_STS_WAIT_EVTT_DONE;
                pl_ecb_temp = pl_ecb;
                if(pl_ecb_temp == pl_ecb_temp->multi_next)    //�����һ���¼�
                {
                    pl_ecb = NULL;
                    pl_evtt->done_sync = NULL;
                }else
                {
                    pl_ecb = pl_ecb->multi_next;
                    pl_ecb_temp->multi_previous->multi_next
                                        = pl_ecb_temp->multi_next;
                    pl_ecb_temp->multi_next->multi_previous
                                        = pl_ecb_temp->multi_previous;
                }
                __Djy_EventReady(pl_ecb_temp);
            }else
            {
                if(pl_ecb->sync_counter != 0)
                    pl_ecb->sync_counter--;
                pl_ecb = pl_ecb->multi_next;
            }
        }while(pl_ecb != pl_evtt->done_sync);
    }

    //���¿��¼����߳���δ���
#if (CN_CFG_DYNAMIC_MEM == 1)
    if(g_ptEventRunning->local_memory != 0)
    {
        __M_CleanUp(g_ptEventRunning->event_id);//ǿ������¼�������������ڴ�
    }
#endif
    __Djy_CutReadyEvent(g_ptEventRunning);
    g_ptEventRunning->previous
                    = (struct EventECB*)&s_ptEventFree;//��ʾ�����ƿ����
    g_ptEventRunning->next = s_ptEventFree;     //pg_event_free�ǵ����ѭ������
    g_ptEventRunning->evtt_id = CN_EVTT_ID_INVALID;     //todo
    s_ptEventFree = g_ptEventRunning;
    s_ptEventFree->event_id = s_ptEventFree - g_ptECB_Table;   //�ݴ���

    pl_evtt->events--;

    if(pl_evtt->property.correlative == EN_CORRELATIVE)      //�������¼�
    {
        pl_evtt->property.inuse = 0;
        if(pl_evtt->default_prio<0x80)
        {//����ģʽΪsi��dlsp�����߸��¼�����ӵ�н������ȼ�ʱ��
         //�뱣���߳�
            vm_final = CN_KEEP;
        }else
        {
            vm_final = CN_DELETE;
        }
    }else   //�������¼�
    {
        //��δ�õ��̵߳��¼�������֮
        if(pl_evtt->events >= pl_evtt->vpus)
        {
            vm_final = CN_KEEP;
        }else   //û��δ�õ��̵߳��¼����ٿ��Ƿ��豣��
        {
            if(pl_evtt->vpus<=pl_evtt->vpus_res)//�������¼�ӵ�е��߳����Ѿ�����
                                                    //�������ͱ�����������֮
            {
                vm_final = CN_KEEP;
            }else                                   //����ɾ���߳�
            {
                if(pl_evtt->vpus == 1)   //�������һ���¼�
                {
                    pl_evtt->property.inuse = 0;
                    if(pl_evtt->default_prio<0x80)
                    {
                 //����ģʽΪsi��dlsp������¼�����ӵ�н������ȼ�ʱ���뱣���߳�
                        vm_final = CN_KEEP;
                    }else
                    {
                        vm_final = CN_DELETE;
                    }
                }else
                {
                    vm_final = CN_DELETE;
                }
            }
        }
    }

    if((pl_evtt->property.inuse == 0) && (pl_evtt->property.deleting == 1))
    {
        next_vm = pl_evtt->my_free_vm;
        //�����¼����Ϳ��ƿ飬ֻ���registered�������㡣
        pl_evtt->property.registered = 0;
        pl_evtt->evtt_name[0] = '\0';   //����¼�������
        while(next_vm != NULL)          //�ͷŸ��¼�����ӵ�еĿ����߳�
        {
            temp = next_vm;
            next_vm = next_vm->next;
            free((void *)temp);
        }
        vm_final = CN_DELETED;
    }

    __Djy_SelectEventToRun();
    if(vm_final == CN_DELETE)      //ɾ���߳�
    {
        free((void*)g_ptEventRunning->vm);    //ɾ���߳�
        pl_evtt->vpus--;
        g_ptEventRunning = g_ptEventReady;
        g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DjyGetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }else if(vm_final == CN_KEEP)    //�����߳�,��exitһ��
    {
        //���¼������еȴ���VM��ECB,��vmת������Ӧ�����е�ECB�Ϳ��ԡ�
        if((pl_evtt->property.correlative == EN_INDEPENDENCE)&&\
                (pl_evtt->mark_event != NULL))
        {
            pl_ecb = pl_evtt->mark_event;
            if(pl_evtt->mark_event->previous == pl_evtt->mark_event)//ֻ��һ��
            {
                pl_evtt->mark_event = NULL;
            }
            else
            {
                pl_ecb->previous->next = pl_ecb->next;
                pl_ecb->next->previous = pl_ecb->previous;
                pl_evtt->mark_event = pl_ecb->next;
            }
            //��ͬ���������Ƴ�,���Կ϶������ڴ�ͬ������
            if(NULL != pl_ecb->sync_head)
            {
                __Djy_CutEcbFromSync(pl_ecb);
            }
            //���ܾ��̵߳�VM
            pl_ecb->vm = g_ptEventRunning->vm;
            __Djy_EventReady(pl_ecb);
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);
            g_ptEventRunning = g_ptEventReady;
            g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DjyGetSysTime();
            //����һ��ܹؼ���running��pl_ecb������ͬһ��������һ��Ҫ��reset��Ȼ
            //��switch
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
        else//��������»��Ƿ���free���а�
        {
            g_ptEventRunning->vm->next = pl_evtt->my_free_vm;
            pl_evtt->my_free_vm = g_ptEventRunning->vm;
            pl_ecb = g_ptEventRunning;
            pl_evtt->SchHook(EN_SWITCH_OUT);
            g_ptEventRunning = g_ptEventReady;
            g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
            g_s64RunningStartTime = DjyGetSysTime();
            Int_HalfEnableAsynSignal( );
            __asm_reset_switch(pl_evtt->thread_routine,
                            g_ptEventRunning->vm,pl_ecb->vm);
        }
    }else               //�߳��Ѿ���ɾ��
    {
//        pl_ecb = g_ptEventRunning;
        g_ptEventRunning = g_ptEventReady;
        g_ptEvttTable[g_ptEventRunning->evtt_id & (~CN_EVTT_ID_MASK)].SchHook(EN_SWITCH_IN);
        g_s64RunningStartTime = DjyGetSysTime();
        Int_HalfEnableAsynSignal( );
        __asm_turnto_context(g_ptEventRunning->vm);
    }
}

//----��ѯ����ԭ��-------------------------------------------------------------
//����: ��ѯ����ִ�е��¼���ִ�е�ԭ��.
//����: ��
//����: ����ԭ��,��ͷ�ļ�union event_status���崦ע��.
//-----------------------------------------------------------------------------
u32 Djy_WakeUpFrom(void)
{
    return g_ptEventRunning->wakeup_from;
}

//----��ȡ�¼���Ϣ-------------------------------------------------------------
//����: �����¼��Ż�ȡ�¼���Ϣ
//����: id,�¼�id; struct EventInfo *info,�洢�¼���Ϣ���������
//����: true�ɹ���falseʧ��
//-----------------------------------------------------------------------------
bool_t Djy_GetEventInfo(u16 id, struct EventInfo *info)
{
    struct EventECB *event;
    if(id >=gc_u32CfgEventLimit)
    {
        return false;
    }
    else
    {
        event = &g_ptECB_Table[id];
        info->consumed_time = event->consumed_time;
        info->error_no = event->error_no;
        info->EventStartTime = event->EventStartTime;
        return true;
    }
}
//----��ȡ�¼���������-----------------------------------------------------------
//����: �����¼����ͺŻ�ȡ�¼���������
//����: evtt_id,�¼�����ID,dest,�洢�ռ䣬len,���洢����
//����: true�ɹ���falseʧ��
//-----------------------------------------------------------------------------
bool_t Djy_GetEvttName(u16 evtt_id, char *dest, u32 len)
{
    struct EventType *pl_evtt;
    u16 evttoffset;
    if((NULL == dest)||(0 ==len))
    {
        return false;
    }
    evttoffset = evtt_id & (~CN_EVTT_ID_MASK);
    if(evttoffset >= gc_u32CfgEvttLimit)
        return false;
    pl_evtt = &g_ptEvttTable[evttoffset];

    if(NULL == pl_evtt->evtt_name)
    {
         dest[0] = '\0';
    }
    else
    {
        strncpy(dest, pl_evtt->evtt_name, len);
    }

    return true;
}
//----�߳�����---------------------------------------------------------------
//����: �����߳�,ִ���߳���ں���,�¼���ɺ�ִ��������
//����: thread_routine ������¼����͵ĺ���ָ��.
//����: ��
//-----------------------------------------------------------------------------
void __Djy_VmEngine(ptu32_t (*thread_routine)(void))
{
    Djy_EventComplete( thread_routine() );
}

//----ϵͳ����-----------------------------------------------------------------
//����: ����ϵͳ��ϵͳ������,�ṩ����,ͳ�Ƶȷ���.
//����: ��
//����: ��������.
//��ʹ��ѵ:����һ�������������ĺ�������ҹ3��������Ľ�ѵ��
//-----------------------------------------------------------------------------
ptu32_t __Djy_Service(void)
{
    u32 loop;
    while(1)
    {
        for(loop = 0; loop<gc_u32CfgEventLimit; loop++)
        {
            __Djy_CheckStack(loop);
//            {
//                Djy_SaveLastError(EN_KNL_STACK_OVERFLOW);
//              printk("%d ���¼���ջ�������,����=%d Name = %s\n\r",loop,
//                     g_ptEvttTable[loop],g_ptEvttTable[loop].evtt_name);
//            }
        }
        if(g_fnEntryLowPower != NULL)
            g_fnEntryLowPower(g_ptEventRunning->vm);      //����͹���״̬
    }
    return 0;//�������뾯��
}


//----api��������--------------------------------------------------------------
//����: ����api�ŵ�����Ӧ��api����.
//����: api_no,api��
//����: mpģʽ���ã����ҷ�������
//-----------------------------------------------------------------------------
void Djy_ApiStart(u32 api_no)
{
    switch(api_no)
    {
        default :break;
    }
    return;
}
//�պ���,δ��ʼ���ĺ���ָ��ָ��.
void NULL_func(void)
{}

