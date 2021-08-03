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
//����ģ��:����ģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����:�¼����͡��¼������Լ����¼�������صĴ���ȫ�������ˡ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __DJYOS_H__
#define __DJYOS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "errno.h"
#include "arch_feature.h"
#include "blackbox.h"
#include "board-config.h"
//�ṹ��������,���������ļ�����Ľṹ
struct SemaphoreLCB;
//�ṹ��������,���ļ�����Ľṹ
struct ThreadVm;
struct EventECB;
struct EventType;

#define CN_BLOCK_FIFO           0       //�������������е��Ⱥ�˳���Ŷ�
#define CN_BLOCK_PRIO           1       //�������������¼������ȼ��Ŷ�

#define CN_RUNMODE_SI           0       //�����̵�ӳ��ģʽ
#define CN_RUNMODE_DLSP         1       //�����̶�̬����ӳ��ģʽ
#define CN_RUNMODE_MP           2       //�����ģʽ
#define CN_RUNMODE_SMP          3       //�Գƶദ����ģʽ
#define CN_RUNMODE_AMP          4       //�ǶԳƶദ����ģʽ

#if 0
// todo:��������ΪContainerOf�ͱ���
#define Container(Ptr, Type, Member)  ((Type *)((char *)(Ptr)-(unsigned long)(&((Type *)0)->Member)))/* ����Linux */
#endif

//�����¼������¼�����id����С��0x8000��0x8000���ϵ�����������;�������ڴ����
//ģ����������ʶ�ڴ�page�ķ�������������struct mem_global��index_event_id��Ա
//��ע��
#define CN_EVENT_ID_INVALID CN_LIMIT_UINT16
#define CN_EVTT_ID_INVALID  CN_LIMIT_UINT16     //�Ƿ��¼����ͺ�
#define CN_EVTT_ID_ASYN     (CN_LIMIT_UINT16-1) //�첽�ź��¼����ͱ�־
#define CN_EVENT_ID_ASYN    (CN_LIMIT_UINT16-1) //�첽�ź��¼�ID��־
#define CN_EVENT_ID_BASE    0           //��С�¼�id��
#define CN_EVENT_ID_LIMIT   16383       //����¼�id��
#define CN_EVTT_ID_BASE     16384       //��С�¼�����id��
#define CN_EVTT_ID_LIMIT    32767       //����¼�����id��
#define CN_EVTT_ID_MASK     0x4000      //�¼����ͺŵĻ���

#define CN_SYNC_SUCCESS     0       //ͬ���������
#define CN_SYNC_TIMEOUT     1       //ͬ������δ��ɣ���ʱ���أ���timeout��Ϊ0
#define CN_SYNC_ERROR       CN_LIMIT_UINT32    //����

//������Ϣ����
enum _KNL_ERROR_CODE_
{
    EN_KNL_NO_ERROR = CN_KNL_NO_ERROR,  //û�д���
    EN_KNL_ETCB_ERROR,            //�¼����Ϳ��ƿ����
    EN_KNL_ECB_EXHAUSTED,         //�¼����ƿ�ľ�
    EN_KNL_ETCB_EXHAUSTED,        //�¼����Ϳ��ƿ�ľ�
    EN_KNL_PCB_EXHAUSTED,         //�������ƿ�ľ�
    EN_KNL_CANT_SCHED,            //��ͼ�ڽ�ֹ����ʱִ�п��������¼����ȵĲ���
    EN_KNL_INVALID_PRIO,          //�Ƿ����ȼ�
    EN_KNL_VPU_OVER,              //�¼����͵��̳߳���������
    EN_KNL_PARA_OVER,             //�¼�������������������
    EN_KNL_EVTT_HOMONYMY,         //�¼���������
    EN_KNL_EVENT_FREE,            //��ͼʹ�ÿ����¼����ƿ�
    EN_KNL_EVTT_FREE,             //��ͼʹ�ÿ����¼����Ϳ��ƿ�
    EN_KNL_EVENTID_LIMIT,         //�¼�idԽ��
    EN_KNL_EVTTID_LIMIT,          //�¼�����idԽ��
    EN_KNL_EVTT_UNREGISTER,       //�¼�����δ�Ǽ�
    EN_KNL_EVENT_SYNC_EXIT,       //�¼�ͬ���У���ͬ�����¼��쳣����
    EN_KNL_MEMORY_OVER,           //�ڴ治��
    EN_KNL_STACK_OVERFLOW         //ջ������վ���
};

//�¼����ȼ����ƶ���
#define CN_PRIO_CRITICAL    (100)
#define CN_PRIO_REAL        (130)
#define CN_PRIO_RRS         (200)
#define CN_PRIO_WDT         (1)
#define CN_PRIO_SYS_SERVICE (250)
#define CN_PRIO_INVALID     (255)   //�Ƿ����ȼ�
#define CFG_GUI_RUN_PRIO    (131)
struct ProcessVm       //����
{
    u8  res;
};

//��mmu�Ļ�����,��ַ����:0~1G����ϵͳ,1~2G���̹�����,2~4G����˽�пռ�.
//�ر�ע��:���ṹҪ�������ʣ����Ա˳���ܸı䣬Ҳ�����������ӳ�Ա
struct ThreadVm          //�߳����ݽṹ
{
    //�߳�ջָ��,���̱߳���ռʱ����sp,����ʱ������̬����sp�仯
    u32    *stack;
    u32    *stack_top;          //�̵߳�ջ��ָ��
    u32    *stack_used;         //�Ѿ�ʹ�õ�ջ�ײ�
    struct ThreadVm *next;      //���ڰ�evtt�����п����߳�����һ�����򿪿�����
                                //��������evtt��my_free_vmָ������
    u32    stack_size;          //ջ���
    struct ProcessVm *host_vm;  //�������̣���si��dlspģʽ��ΪNULL
};

//�¼���������̬��ԭ��.
//�¼�Ӧ�����ڱյ�,djyos���ṩ��ѯ�����¼���ǰ״̬�Ĺ���.��ѯ���˵�״̬,�и���
//����֮��.
//ʹ�÷���:���¼���������̬ʱ,���ݽ���ǰ״̬�����¼���wakeup_from.
//��ѯ��������wakeup_fromֵ�ж��¼��Ӻ���״̬��������̬.
//�����ȼ��Ͷ��ȴ��Ĳ��迼��,����,ĳ�¼���ʱ��������Ϊ���ȼ��Ͷ��ȴ�һ��ʱ���
//������,��ѯ���Ϊ"������"
#define CN_STS_EVENT_READY      (u32)0
#define CN_STS_EVENT_DELAY      (u32)(1<<0)     //��ʱ����������Djy_DelayUs
#define CN_STS_SYNC_TIMEOUT     (u32)(1<<1)     //��ʱ
#define CN_STS_WAIT_EVENT_DONE  (u32)(1<<2)     //�¼�ͬ��
#define CN_STS_EVENT_EXP_EXIT   (u32)(1<<3) //�¼�ͬ���б�ͬ����Ŀ���¼��쳣�˳�
#define CN_STS_WAIT_EVTT_POP    (u32)(1<<4)     //�¼����͵���ͬ��
#define CN_STS_WAIT_EVTT_DONE   (u32)(1<<5)     //�¼��������ͬ��
#define CN_STS_WAIT_MEMORY      (u32)(1<<6)     //��ϵͳ�ѷ����ڴ�ͬ��
#define CN_STS_WAIT_SEMP        (u32)(1<<7)     //�ź���ͬ��
#define CN_STS_WAIT_MUTEX       (u32)(1<<8)     //������ͬ��
#define CN_STS_WAIT_ASYN_SIGNAL (u32)(1<<9)     //�첽�ź�ͬ��
#define CN_STS_WAIT_CPIPE       (u32)(1<<10)    //����pipe
#define CN_STS_WAIT_VPIPE       (u32)(1<<11)    //��̬����pipe
#define CN_STS_WAIT_MSG_SENT    (u32)(1<<12)    //�ȴ���Ϣ����
#define CN_STS_WAIT_PARA_USED   (u32)(1<<13)    //�ȴ���Ϣ�������
#define CN_STS_EVTTSYNC_DELETED (u32)(1<<14)    //�¼�������ص�ͬ������Ŀ������
                                                //��ɾ�������ͬ����
#define CN_STS_EVENT_RESET      (u32)(1<<15)    //��λ���״���������
#define CN_STS_EVENT_NORUN      (u32)(1<<16)    //�¼���δ��ʼ����

#define CN_BLOCK_PRIO_SORT      (u32)(1<<17)    //�Ƿ������ȼ����������������

 //˵��:
//1�������¼�ʱ�����Я��������ϵͳ�������������ƿ�(tagParaPCB)��¼�ò�����
//2����������ߴ�С��cn_para_limited����ֱ��copy���������ƿ��static_para��Ա�С�
//3���������cn_para_limited�����ӵ���djy_event_pop����ʱ��para_options����������
//   ���djy_event_pop������ע�͡�
//struct ParaPCB
//{
//    struct ParaPCB *next,*previous;
//    struct EventECB *sync;     //�ȴ���������������¼�������ɵ��¼����У�
//                                //��ΪNULL,�򲻵ȴ�����ֱ�ӷ���
////#if(CN_CFG_DEBUG_INFO == 1)
////    s64 ParaStartTime;             //����������ʱ�䣬uS
////#endif
//    bool_t dynamic_mem;         //true=����������ڴ��Ƕ�̬�����
//    void *event_para;
//    //�¼������������ߴ�����32�ֽڣ�ֱ�Ӵ��������ʽ�ɵ����ʹ����¼���˫��Э��
//    //ȷ��.�Ѱ����ϸ�Ķ��룬Ӧ�ó������ֱ�Ӱ���ǿ������ת�����κ���Ҫ�����͡�
//    align_type static_para[32/sizeof(align_type)];
//};
struct EventInfo
{
    s64    EventStartTime;      //�¼�����ʱ�䣬uS
    s64    consumed_time;       //�¼����ĵ���ʱ��
    u32    consumed_time_second;//���1�����ĵ�ʱ��
    u32    StackSize;
    u32*   Stack;
    u32*   StackUsed;
    u32*   StackBottom;
    u32*   StackTop;
    char  *EvttName;

    u32    error_no;            //���¼�ִ�в��������һ�������
    ptu32_t event_result;       //������¼�����ʱ�������¼������ҵȴ�������
                                //(������pop����ʱ��timeout !=0)�����������أ���
                                //�ﱣ����¼���������
};
struct EventECB
{
    //�¼���ָ��,���ڹ�����������
    //pg_event_free: ���⣬�μ��ĵ�
    //g_ptEventReady:���������μ��ĵ�
    //g_ptEventDelay:˫��ѭ��
    struct EventECB *next,*previous;
    //�๦������ָ�룬����������������:
    //1����ͬ�����У������¼�ͬ�����¼����͵���ͬ����
    //2���������������ȼ��������У�˫��ѭ��������ʵ����ѯ��y_event_ready��
    //   ��O(1)�㷨��
    struct EventECB *multi_next,*multi_previous;      //����ͬ������
    struct ThreadVm  *vm;               //�����¼����߳�ָ��
    ptu32_t param1,param2;              //�¼�����,ֻ�������һ�ε�������Ĳ���
    ptu32_t userdata;                   //�ṩ���û�ʹ�õ�����
//    struct ParaPCB *para_high_prio;   //�����ȼ���������
//    struct ParaPCB *para_low_prio;    //�����ȼ���������
//    struct ParaPCB *para_current;     //��ǰ��Ҫ�����ڴ���Ĳ�����
    struct EventECB *sync;              //ͬ���ڱ��¼��Ķ��У������¼���ɺ�
                                        //��������е��¼�
                                        //�����ͬ����һ��������ͬ���ǵ���ʱ�õ�
    struct EventECB **sync_head;        //��ס�Լ�����һ��ͬ�������У��Ա㳬ʱ
                                        //����ʱ�Ӹ�ͬ������ȡ���¼�

#if CFG_OS_TINY == false
    s64    EventStartTime;              //�¼�����ʱ�䣬uS
    s64    consumed_time;               //�¼����ĵ���ʱ��
    u32    consumed_time_second;        //���1�����ĵ�ʱ��
    u32    consumed_time_record;        //�ϴ�����ʱ�����ĵ�ʱ�����
#endif  //CFG_OS_TINY == false
    s64    delay_start_tick;    //�趨����ʱ��
    s64    delay_end_tick;      //������ʱ��
    u32    error_no;            //���¼�ִ�в��������һ�������
    ptu32_t event_result;       //������¼�����ʱ�������¼������ҵȴ�������
                                //(������pop����ʱ��timeout !=0)�����������أ���
                                //�ﱣ����¼���������
    u32    wait_mem_size;       //�ȴ�������ڴ�����.
    u32    HeapSize;            //���̴߳�heap�з�����ڴ�����
    u32    HeapSizeMax;         //���̴߳�heap�з�����ڴ���������ʷ���ֵ
    //�������вμ�CN_STS_EVENT_READYϵ�ж���
    u32 wakeup_from;            //���ڲ�ѯ�¼��������̬��ԭ��,todo,ֱ�ӷ���״̬
    u32 event_status;           //��ǰ״̬,�������ɲ���ϵͳ�ڲ�ʹ��,
    u32 prio_raise_cnt;         //���ȼ��̳м���
    u8  prio_base;         //��ʱ�������ȼ�ʱ�������ı�prio_base
    u8  prio;              //�¼����ȼ�
//  ufast_t  prio_new;       //���ȼ����ݣ������޸Ĵ�������̬���¼����ȼ�ʱ��
                                //�ݴ������ȼ���

    u16    evtt_id;             //�¼�����id��0~16383
    u16    sync_counter;        //ͬ������
//    u16    paras;             //�¼���Ϣ���е�ǰ����

    //�¼�id��Χ:0~32767(CN_EVENT_ID_LIMIT)
    u16    event_id;            //�¼����б��,��ͬ���¼����¼��������е�ƫ��λ��
    u32    local_memory;        //���¼��������������ľֲ��ڴ����
                                //���ջؽ������ڴ�й©.
};


//�¼����Զ����
struct EvttStatus
{
    u16 correlative:1;  //0=��ʾ�����¼�,�¼������п���ͬʱ���ڶ��������¼�
                        //1=ͬһ���͵Ķ���¼������й����ģ�����Э������
                        //���¼�������ֻ�������һ�����¼�
    u16 registered:1;   //0=���¼����ͻ�û��ע��,ϵͳ���ܾ�pop������¼
                        //1=���¼������Ѿ�ע��,����pop�������¼�
    u16 inuse:1;        //0=���ж����ж�û�и����͵��¼�
                        //1=������(�����ȴ���)������һ�������͵��¼�
    u16 deleting:1;     //0=����״̬��1=�ȴ�ע��״̬��
};

enum enEventRelation
{
    EN_INDEPENDENCE=0,//�߼��ϣ��ɳ����������¼�,ÿ�ε����������¼�����������
                        //�������¼����ƿ飻�Ӵ�����̿����ɳ����ǲ����¼���ÿ��
                        //�����������¼����������߳̽��д���
    EN_CORRELATIVE,   //�߼��ϣ��ɳ����������¼���ͬһ���͵Ķ���¼������й���
                        //�ģ�����Э��������¼�������ֻ�������һ�����¼�������
                        //���̿����Ǵ����¼���ͬһ�����¼���ε��������봮�д���
                        //ÿ���¼���
};

enum enSwitchType
{
    EN_SWITCH_IN=0,     //�������л�ʱ����ĳ�¼���
    EN_SWITCH_OUT,      //�������л�ʱ����ĳ�¼���
};
typedef void (*SchHookFunc)(ucpu_t SchType);

#define CN_EVTTNAME_LIMIT   32
struct EventType
{
    //ͬһ���͵��¼������ж�������ִ�л�ȴ�����,����Щ�¼�����ͬ������.
    struct EvttStatus    property;
    //�����߳�ָ��,�����̸߳��¼�ʱ�����ȴ��������
    struct ThreadVm  *my_free_vm;
    // �¼����Ȼص�����,�ڱ������¼����������������ʱ����.
    // ������ʹ�õ�ջ,�������������,���������뻹������,��ʹ�������¼���ջ.
    // ������ж������л�,��ʹ���ж�ջ.
    // ����:
    // SchType: EN_SWITCH_IN = ����,EN_SWITCH_OUT=����
    // event_id: �����������¼�ID
    SchHookFunc SchHook;
#if CFG_OS_TINY == false
    char evtt_name[CN_EVTTNAME_LIMIT]; //�¼���������û�����֣���ֻҪ�����֣��Ͳ�����ͬ��
                        //���һ������ֻ��ģ���ڲ�ʹ�ã����Բ������֡�
                        //��ģ�����Ҫ���浯���¼��������ַ��ʡ�
#endif  //CFG_OS_TINY == false
    //���ȼ�С��0x80Ϊ�������ȼ�,��Ӱ���̵߳Ĺ���.�������ȼ��ڳ�ʼ��ʱ�趨,
    u16    default_prio;    //�¼��������ȼ�.��ͬ���¼����ȼ�,1~255,0�Ƿ�.
    u16    correlativeID;   //������:��Ч
                            //������:�Ѿ��������¼�ID
    u16    events;          //������¼�����
    u16    vpus_res;        //ϵͳΪ�������¼������Ŀ����߳����ޣ���������Ч
    u16    vpus_limit;      //������:�������¼�����ͬʱ�������̸߳���
                            //������:��Ч
    u16    vpus;            //������:�������¼��Ѿ�ӵ�е��̸߳���
                            //������:��Ч
//    u16    para_limit; //�������¼��������ƿ���г���
    ptu32_t (*thread_routine)(void);//����ָ��,��������ѭ��.
    u32 stack_size;              //thread_routine�����ջ��С
    u32 pop_times;     //�������¼���ʷ�ۼƵ�������������0xffffffff������0

//  struct EventECB *mark_event;
    //�������¼�����Ч��ָ���¼������еı������¼�
    //���ڶ������¼����ö��н��������ڴ����Щû�з��䵽VM��ECB,�������ȼ��Ӹ�
    //���͵�˳��

    //�������ж�����ʣ������Ŷӵ�˫��ѭ������
    struct EventECB *done_sync,*pop_sync;//����ͬ�������ͬ������ͷָ��,
};

//��������(���ȼ�����),ʼ��ִ�ж���ͷ�����¼�,���ж�����ȼ���ͬ,����ִ��
extern struct EventECB  *g_ptEventReady;
extern struct EventECB  *g_ptEventRunning;   //��ǰ����ִ�е��¼�
extern bool_t g_bScheduleEnable;
void DJY_ScheduleIsr(u32 inc_ticks);
void DJY_SetRRS_Slice(u32 slices);
u32 DJY_GetRRS_Slice(void);
void DJY_CreateProcessVm(void);
u16 DJY_EvttRegist(enum enEventRelation relation,
                        ufast_t default_prio,
                        u16 vpu_res,
                        u16 vpus_limit,
//                        u16 para_limit,
                        ptu32_t (*thread_routine)(void),
                        void *Stack,
                        u32 stack_size,
                        char *evtt_name);
u32 DJY_GetRunMode(void);
u16 DJY_GetEvttId(char *evtt_name);
bool_t DJY_EvttUnregist(u16 evtt_id);
bool_t DJY_QuerySch(void);
bool_t DJY_IsMultiEventStarted(void);
bool_t DJY_SetEventPrio(u16 event_id,ufast_t new_prio);
bool_t Djy_SetEventPrio(u16 event_id,ufast_t new_prio);//�޸ĳ�DJY_SetEventPrio�������C���г�ͻ��copy��һ�ݣ�����C��֮��ɾ��
bool_t DJY_RaiseTempPrio(u16 event_id);
bool_t DJY_RestorePrio(void);
u32 DJY_EventDelay(u32 u32l_uS);
s64 DJY_EventDelayTo(s64 s64l_uS);
u32 DJY_WaitEventCompleted(u16 event_id,u32 timeout);
u32 DJY_WaitEvttCompleted(u16 evtt_id,u16 done_times,u32 timeout);
u32 DJY_WaitEvttPop(u16 evtt_id,u32 *base_times, u32 timeout);
u16 DJY_EventPop(  u16  hybrid_id,
                    u32 *pop_result,
                    u32 timeout,
                    ptu32_t PopPrarm1,
                    ptu32_t PopPrarm2,
                    ufast_t prio);
u32 DJY_GetEvttPopTimes(u16 evtt_id);
ptu32_t DJY_GetEventResult(void);
void DJY_GetEventPara(ptu32_t *Param1,ptu32_t *Param2);
void __SetEventPara(ptu32_t *Param1,ptu32_t *Param2);
ptu32_t DJY_GetEventUserdata(void);
void DJY_SetEventUserdata(ptu32_t userdata);
void __DJY_EventExit(struct EventECB *event, u32 exit_code,enum EN_BlackBoxAction action);
void DJY_EventComplete(ptu32_t result);
u32 DJY_WakeUpFrom(void);
u16 DJY_GetMyEvttId(void);
u16 DJY_GetMyEventId(void);
u16 Djy_MyEventId(void); //�޸ĳ�DJY_GetMyEventId�������C���г�ͻ��copy��һ�ݣ�����C��֮��ɾ��
void DJY_ApiStart(u32 api_no);
void DJY_DelayUs(u32 time);
void DJY_DelayNano(u32 time);

bool_t DJY_GetEventInfo(u16 id, struct EventInfo *info);
bool_t DJY_GetEvttName(u16 evtt_id, char *dest, u32 len);
bool_t DJY_RegisterHook(u16 EvttID,SchHookFunc HookFunc);

#ifdef __cplusplus
}
#endif
#endif //__DJYOS_H__
