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
// ģ������: �����ʱ��ģ��
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:36:18 PM/Apr 1, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:���ļ���Ҫ�ṩһ��Ӳ����ʱ������Ҫһ���������ʱ��64λ��ʱ��
#include "board-config.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"

#include "os.h"

#include "timer.h"
#include "timer_hard.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Timer(void);
//    ModuleInstall_Timer();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"Software Timers"//�����ʱ�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:system              //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"message queue"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_SOFTWARE_TIMERS == false )
//#warning  " Software_Timers  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_SOFTWARE_TIMERS    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,2000,
#define CFG_TIMERS_LIMIT        5       //"��ʱ������",�ɴ����Ķ�ʱ��������������ͼ�ν���Ķ�ʱ����
//%$#@enum,true,false,
#define CFG_TIMER_SOUCE_HARD    true    //"Ӳ����ʱ���ṩʱ��Դ",ѡ��ר��Ӳ������tick��ʱ��Դ
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure


//timer�����ݽṹ
struct Timer
{
    struct Timer  *pre;
    struct Timer  *nxt;
    char          *name;
    u32           cycle;        //��ʱ������ (��λ��΢��)
    fnTimerRecall isr;          //��ʱ����ʱʱ��ڵ㹳�Ӻ���
    u32           stat;         //��ʱ��״̬��־���μ� CN_TIMER_ENCOUNT �ȳ���
    ptu32_t       TimerTag;     //˽�б�ǩ
    s64           deadline;     //��ʱ����ʱʱ��(��λ��΢��)
};

#define CN_TIMER_ALARMNEVER      CN_LIMIT_SINT64             //����ʱ��

// ʱ���ȵ��㷨��
// ��ʱ���׼��tick����ȡ1��tick��Ӧ��uS����
// ����Ӳ����ʱ����ȡ1000����Ƶ���ڶ�Ӧ��uS��ȡ��������Ƶ����1G��ȡ1uS��
// Ӳ����ʱ��ʱ�����ƾ�����Ϊ�����ƶ�ʱ���ж�Ƶ����
// �⣬��ʹʹ����Ӳ����ʱ�������systimeû��ר�õĶ�ʱ������������tick�Ļ���
// ��Ȼ�޷�������辫�ȡ�
static u32 s_u32TimerPrecision;     //��Ƶ1000clk��Ӧ��uS����ȡ��
static u32 s_u32Precision2Tclk;     //ȡ�����s_u32TimerPrecision��Ӧ�Ķ�ʱ����������
static u32 s_u32TimerFreq;          //���õ�Ӳ����ʱ��Ƶ��
//static bool_t  sbUsingHardTimer = false;
static tagTimer* ptTimerHead = NULL;                 //��ʱ������ͷ
static tagTimer* ptTimerTail = NULL;                 //�����ʱ����β
static struct MemCellPool  *ptTimerMemPool = NULL;    //�ڴ��ͷָ�롣
static struct MutexLCB     *ptTimerQSync =NULL;       //API��Դ������

//ʹ�õ�Ӳ����ʱ����ָ��ʹ��Ӳ����ʱ����
static ptu32_t sgHardTimerDefault = (ptu32_t)NULL;           //������ʱ��32λ��ʱ��
//ʹ���߳�ģ��Ӳ����ʱ��
enum __EN_TIMERSOFT_CMD
{
    EN_TIMERSOFT_ADD = EN_TIMER_LASTOPCODE+1,
    EN_TIMERSOFT_REMOVE,
};

static struct MsgQueue    *ptTimerMsgQ = NULL;
typedef struct
{
    tagTimer*     timer;
    u32               type;
    u32               para;
}tagTimerMsg;
#define CN_TIMERSOFT_MSGLEN  0X10
// =============================================================================
// �������ܣ�__Timer_Get64Time
//           ��ȡS64timer��ʱ��
// �������: ��
// ���������time,��λΪ΢��
// ����ֵ  ��true �ɹ�  falseʧ��
// ˵��    ���ڲ����ú���,ע��ú�������ֲ���к�����S64timer,����Ϊ��ͣ������ϵ�
//           64λ��ʱ����ÿ��ϵͳ����CPU�����õĶ���һ��
// =============================================================================
bool_t __Timer_Get64Time(s64 *time)
{
    *time = DJY_GetSysTime();
    return true;
}
// =============================================================================
// �������ܣ�__Timer_ExeAlarmHandler
//          ִ��timersoft�Ĺ��Ӻ���
// ���������timersoft,��ִ�еĶ�ʱ��.
// �����������
// ����ֵ  ��true �ɹ�  falseʧ��
// ˵��    ���ڲ����ú�����û��HOOK��ִ��
// =============================================================================
bool_t __Timer_ExeAlarmHandler(tagTimer *timer)
{
    if(NULL != timer->isr)
    {
        timer->isr(timer);
    }
    return true;
}

// =============================================================================
// �������ܣ�__Timer_Remove
//          ��ָ���Ķ�ʱ���Ӷ�ʱ���������Ƴ�
// ���������timer,���Ƴ��Ķ�ʱ��
// �����������
// ����ֵ  �������Ķ�ʱ��
// ˵��    ��
// =============================================================================
void __Timer_Remove(tagTimer *timer)
{
    if((timer == ptTimerHead)&&(timer == ptTimerTail)) //only one
    {
        ptTimerHead = NULL;
        ptTimerTail = NULL;
    }
    else if(timer == ptTimerHead)  //head to remove
    {
        ptTimerHead = ptTimerHead->nxt;
        ptTimerHead->pre = NULL;
    }
    else if(timer == ptTimerTail) //tail to remove
    {
        ptTimerTail = ptTimerTail->pre;
        ptTimerTail->nxt = NULL;
    }
    else //normal to remove
    {
        timer->pre->nxt = timer->nxt;
        timer->nxt->pre = timer->pre;
    }
    return;
}
// =============================================================================
// �������ܣ�__Timer_Add
//          ��Ӷ�ʱ������ʱ������
// ���������timer,����ӵĶ�ʱ��
// �����������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __Timer_Add(tagTimer *timer)
{
    tagTimer *tmp;
    if(NULL == ptTimerHead) //the queue is empty
    {
        ptTimerHead = timer;
        ptTimerTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else        //find proper place to add
    {
        tmp = ptTimerHead;
        while((NULL != tmp)&&(tmp->deadline < timer->deadline))
        {
            tmp = tmp->nxt;
        }
        if(NULL == tmp) //add the timer to the tail
        {
            timer->pre = ptTimerTail;
            timer->nxt = NULL;
            ptTimerTail->nxt = timer;
            ptTimerTail = timer;
        }
        else
        {
            timer->pre = tmp->pre;
            timer->nxt = tmp;
            if(NULL != tmp->pre)
            {
                tmp->pre->nxt = timer;
            }
            else
            {
                ptTimerHead = timer;
            }
            tmp->pre = timer;
        }
    }
    return;
}

// =============================================================================
// �������ܣ�__Timer_AddLast
//          ��Ӷ�ʱ������ʱ������β
// ���������timer,����ӵĶ�ʱ��
// �����������
// ����ֵ  ��
// ˵��    ��
// =============================================================================
void __Timer_AddLast(tagTimer *timer)
{
    if(NULL == ptTimerHead) //the queue is empty
    {
        ptTimerHead = timer;
        ptTimerTail = timer;
        timer->pre = NULL;
        timer->nxt = NULL;
    }
    else
    {
        timer->pre = ptTimerTail;
        timer->nxt = NULL;
        ptTimerTail->nxt = timer;
        ptTimerTail = timer;
    }
    return;
}

// =============================================================================
// �������ܣ�__Timer_ChkTimeout
// �����������
// �����������
// ����ֵ  ��true���ö�ʱ����ʱ������ö�ʱ������ʱ
// ˵��     :
// =============================================================================
bool_t  __Timer_ChkTimeout(tagTimer *timer, s64 timenow)
{
    bool_t result = false;
    if((timer->deadline -s_u32TimerPrecision)<timenow)
    {
        result = true;
    }
    return result;
}
// =============================================================================
// �������ܣ�__Timer_DealTimeout
//          ����ʱ�Ķ�ʱ������
// ���������
// ���������
// ����ֵ  ���´εȴ���ʱ��
// ˵��    ���ڲ����ú���
// =============================================================================
u32 __Timer_DealTimeout(void)
{
    u32           result;
    s64           timenow;
    tagTimer *timer;

    result = CN_TIMEOUT_FOREVER;
    timer = ptTimerHead;
    while(timer) //ִ�������е�TIMER ALARM
    {
        if(timer->stat &CN_TIMER_ENCOUNT)
        {
            timenow = DJY_GetSysTime();     //ʹ��ϵͳ64λ��ͣ��Ķ�ʱ����������
                                        //��ʱ���ж���֮ͣ������Ļ������
            if(__Timer_ChkTimeout(timer, timenow))
            {
                __Timer_Remove(timer);
                if(timer->stat & CN_TIMER_RELOAD)
                {
                    timer->deadline = timenow + timer->cycle;
                    __Timer_Add(timer);
                }
                else
                {
                    timer->stat &= (~CN_TIMER_ENCOUNT);
                    timer->deadline = CN_TIMER_ALARMNEVER;
                    __Timer_AddLast(timer);
                }
                __Timer_ExeAlarmHandler(timer); //execute the hook

                timer = ptTimerHead;
            }
            else
            {
                //compute the wait time
                result = (u32)(timer->deadline - timenow);
                timer = NULL;
            }
        }
        else
        {
            timer = NULL; //����ѭ����֤�����е�TIMER������PAUSE����û�ж�ʱ��״̬
        }
    }

    return result;
}

u32 __GetTclkCycle(u32 waittime)
{
    u32 result;
    result = waittime *(s_u32TimerFreq / 1E6);
    if(result < s_u32Precision2Tclk)      //�жϼ��С����С���
        result = s_u32Precision2Tclk;
    return result;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// =============================================================================
// �������ܣ�Timer_ISR
//          ��ʱ���жϷ���HOOK
// ���������irq_no,�жϺ�
// �����������
// ����ֵ     ����ʱδ����
// =============================================================================
u32 Timer_ISR(ptu32_t irq_no)
{
    u32 waittime;
    //��ʱ��ֹͣ����
    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

    waittime = __Timer_DealTimeout();
    if(waittime != CN_TIMEOUT_FOREVER)
    {
        waittime = __GetTclkCycle(waittime);
        HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
        HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
    }
    else
    {
        //all the timer is paused or no timer, so no need to start the counter
    }

    return 0;
}
#pragma GCC diagnostic pop

// =============================================================================
// �������ܣ�Timer_Create_s
//          ����һ����ʱ��(�Լ��ṩ��Դ)
// ���������name,��ʱ������
//          cycle,��ʱ������,��λ΢��
//          isr,��ʱ����ʱʱ�䵽ִ��HOOk���ж��п��ܱ�����
//          timer,�ṩ�����ⶨʱ������Դ�ռ�
// �����������
// ����ֵ  �� NULL����ʧ��  ���򷵻ش����ɹ��Ķ�ʱ�����
//           �����Ķ�ʱ��Ĭ�ϵ�reloadģʽ�������Ҫ�ֶ��Ļ�����ô����֮���Լ����ã�
//           �����Ķ�ʱ�����Ǵ���pause״̬����Ҫ�ֶ������ö�ʱ��
// =============================================================================
tagTimer*  Timer_Create_s(tagTimer *timer,const char *name,
                                  u32 cycle, fnTimerRecall isr)
{
    tagTimer*      result = NULL;
    u32                waittime;
    if(NULL == timer)  //�������
    {
        result =NULL;
    }
    else
    {
#if CFG_TIMER_SOUCE_HARD == true        //��Ӳ����ʱ���ṩʱ��Դ
//      if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER))
            {
                //��ͣ���ӣ������жϻ���
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
                //��ʼִ��ָ������Ĳ���
                //���ⶨʱ���ĳ�ʼ��
                timer->name = (char*)name;
                timer->cycle = cycle;
                timer->isr = isr;
                timer->deadline = CN_TIMER_ALARMNEVER;
                timer->stat = CN_TIMER_RELOAD;
                //�����������ⶨʱ�����timer��ʱ��������
                __Timer_AddLast(timer);
                waittime = __Timer_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    __GetTclkCycle(waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerQSync);
                result = timer;
            }
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg    msg;
            timer->name = (char*)name;
            timer->cycle = cycle;
            timer->isr = isr;
            timer->deadline = CN_TIMER_ALARMNEVER;
            timer->stat = CN_TIMER_RELOAD;
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_ADD;
            if(MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }
    return result;
}
// =============================================================================
// �������ܣ�Timer_Delete_s
//           ɾ��һ����ʱ��(�Լ��ṩ��Դ�Ķ�ʱ��)
// ���������timersoft,��ɾ���Ķ�ʱ��
// �����������
// ����ֵ  ����ɾ���Ķ�ʱ����NULLɾ�����ɹ�
// =============================================================================
tagTimer* Timer_Delete_s(tagTimer* timer)
{
    tagTimer*      result = NULL;
    u32                waittime;

    if(NULL == timer)  //�������
    {
        result =NULL;
    }
    else
    {
#if CFG_TIMER_SOUCE_HARD == true        //��Ӳ����ʱ���ṩʱ��Դ
//      if(sbUsingHardTimer)
        {
           if(Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER))
            {
                //��ͣ���ӣ������жϻ���
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);

                //�����������ⶨʱ�����timer��ʱ��������
                __Timer_Remove(timer);
                waittime = __Timer_DealTimeout();
                if(waittime != CN_TIMEOUT_FOREVER)
                {
                    __GetTclkCycle(waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                    HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
                }
                Lock_MutexPost(ptTimerQSync);
                result = (tagTimer*)timer;
            }
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg    msg;
            msg.timer = timer;
            msg.type = EN_TIMERSOFT_REMOVE;
            if(MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL))
            {
                result = timer;
            }
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }
    return result;
}
// =============================================================================
// �������ܣ�Timer_Create
//          ����һ����ʱ��
// ���������name,��ʱ������
//          cycle,��ʱ�����ڣ���λ΢��
//          isr,��ʱ����ʱʱ�䵽ִ��HOOk���ж��п��ܱ�����
// �����������
// ����ֵ  ��NULL����ʧ��  ���򷵻ط��䵽�Ķ�ʱ�����
//           �����Ķ�ʱ��Ĭ�ϵ�reloadģʽ�������Ҫ�ֶ��Ļ�����ô����֮���Լ����ã�
//           �����Ķ�ʱ�����Ǵ���pause״̬����Ҫ�ֶ������ö�ʱ��
// =============================================================================
tagTimer* Timer_Create(const char *name, u32 cycle,fnTimerRecall isr)
{
    tagTimer *timer;
    tagTimer *result;

    result = NULL;
    timer = Mb_Malloc(ptTimerMemPool,CN_TIMEOUT_FOREVER);
    if(NULL != timer)
    {
        result = Timer_Create_s(timer,name, cycle, isr);
        if(NULL == result)
        {
            Mb_Free(ptTimerMemPool,timer);
        }
    }
    return result;
}

//-----------------------------------------------------------------------------
//���ܣ��鿴��ʱ���Ƿ�������״̬
//������Timer�����鿴�Ķ�ʱ��
//���أ�true = running��false= stop
//-----------------------------------------------------------------------------
bool_t Timer_IsRunning(tagTimer *Timer)
{
    if(Timer == NULL)
    {
        return false;
    }
    else
    {
        if(Timer->stat & CN_TIMER_ENCOUNT)
            return true;
        else
            return false;
    }
}

// =============================================================================
// �������ܣ�Timer_Delete
//           ɾ��һ����ʱ��
// ���������timersoft,��ɾ���Ķ�ʱ��
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t Timer_Delete(tagTimer* timer)
{
    tagTimer* result;

    result = Timer_Delete_s(timer);
    if(result != (tagTimer*)NULL)
    {
        Mb_Free(ptTimerMemPool, (void *)timer);
        return true;
    }
    else
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�Timer_Ctrl
//          ���ö�ʱ����ʶ��״̬
// ���������timersoft,���趨�Ķ�ʱ��.
//          opcode,��ʱ���Ĳ�����
//          inoutpara,���ݲ���������Ĳ������ȷ��趨cycle��ʱ��inoutpara����cyle
// �����������
// ����ֵ  ��true �ɹ� falseʧ��
// ˵����opcode��Ӧ��para�����Զ����enum TimerCmdCode˵��
// =============================================================================
bool_t Timer_Ctrl(tagTimer* timer,u32 opcode, u32 para)
{
    bool_t  result = false;
    u32     waittime;
    s64     timenow;               //��ǰʱ��

    if(timer)                      //�������
    {
#if CFG_TIMER_SOUCE_HARD == true        //��Ӳ����ʱ���ṩʱ��Դ
//      if(sbUsingHardTimer)
        {
            Lock_MutexPend(ptTimerQSync,CN_TIMEOUT_FOREVER);
            //��ͣ���ӣ������жϻ���
            HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_PAUSECOUNT,(ptu32_t)NULL);
            result = true;
            switch(opcode)
            {
                case EN_TIMER_SOFT_START:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))    //����δʹ��
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        timenow = DJY_GetSysTime(); //ʹ��ϵͳ64λ��ͣ��Ķ�ʱ����������
                                                //��ʱ���ж���֮ͣ������Ļ������
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_STOP:
                    if(CN_TIMER_ENCOUNT & timer->stat)          //����������̬
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __Timer_Remove(timer);
                        __Timer_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        timenow = DJY_GetSysTime(); //ʹ��ϵͳ64λ��ͣ��Ķ�ʱ����������
                                                //��ʱ���ж���֮ͣ������Ļ������
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                default:
                    result = false;
                    break;
            }
            //�����������ܻ��ж�ʱ����ʱ,Ҫ����ʱ�����еĳ�ʱ����
            waittime = __Timer_DealTimeout();
            if(waittime != CN_TIMEOUT_FOREVER)
            {
                __GetTclkCycle(waittime);
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_SETCYCLE,(ptu32_t)waittime);
                HardTimer_Ctrl(sgHardTimerDefault, EN_TIMER_STARTCOUNT,(ptu32_t)NULL);
            }
            Lock_MutexPost(ptTimerQSync);
        }
#else       //for #if CFG_TIMER_SOUCE_HARD == true
        {
            tagTimerMsg msg;
            msg.timer = timer;
            msg.type = opcode;
            msg.para = para;
            result = MsgQ_Send(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),\
                               CN_TIMEOUT_FOREVER, CN_MSGQ_PRIO_NORMAL);
        }
#endif       //for #if CFG_TIMER_SOUCE_HARD == true
    }

    return result;
}
// =============================================================================
// �������ܣ�Timer_VMTask �߳�ģ��Ӳ����ʱ��
// �����������
// �����������
// ����ֵ  ��
// ˵��    :��ʹ���߳���Ϊģ�ⶨʱ����ʱ�򣬳�����Ӻ�ɾ�����⣬���еĶԶ�ʱ��
//          ���еĲ��������ڸ��߳������
// =============================================================================
ptu32_t  Timer_VMTask(void)
{
    u32              waittime;
    u32              opcode;
    u32              para;
    s64              timenow;
    tagTimerMsg  msg;
    tagTimer     *timer;

    while(1)
    {
        //any way, we scan the queue to chk if any timer is timeout
        waittime = __Timer_DealTimeout();
        if(MsgQ_Receive(ptTimerMsgQ,(u8 *)&msg, sizeof(msg),waittime))
        {
            //has got some msg from the api
            opcode = msg.type;
            timer = msg.timer;
            para = msg.para;
            switch(opcode)
            {
                case EN_TIMER_SOFT_START:
                    if(0 ==(CN_TIMER_ENCOUNT & timer->stat))    //����δʹ��
                    {
                        timer->stat |= CN_TIMER_ENCOUNT;
                        timenow = DJY_GetSysTime(); //ʹ��ϵͳ64λ��ͣ��Ķ�ʱ����������
                                                //���tick�жϲ����Ļ������
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_STOP:
                    if(CN_TIMER_ENCOUNT & timer->stat)      //����������̬
                    {
                        timer->stat &= (~CN_TIMER_ENCOUNT);
                        timer->deadline = CN_TIMER_ALARMNEVER;
                        __Timer_Remove(timer);
                        __Timer_AddLast(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETCYCLE:
                    timer->cycle = para;
                    if(CN_TIMER_ENCOUNT&timer->stat)
                    {
                        timenow = DJY_GetSysTime(); //ʹ��ϵͳ64λ��ͣ��Ķ�ʱ����������
                                                //���tick�жϲ����Ļ������
                        timer->deadline = timenow + timer->cycle;
                        __Timer_Remove(timer);
                        __Timer_Add(timer);
                    }
                    break;
                case EN_TIMER_SOFT_SETRELOAD:
                    if(para)
                    {
                        timer->stat |= CN_TIMER_RELOAD;
                    }
                    else
                    {
                        timer->stat &= (~CN_TIMER_RELOAD);
                    }
                    break;
                case EN_TIMERSOFT_ADD:
                    __Timer_Add(timer);
                    break;
                case EN_TIMERSOFT_REMOVE:
                    __Timer_Remove(timer);
                    break;
                default:
                    break;
            }

        }

    }
    return 0;
}

//-----------------------------------------------------------------------------
//���ܣ����ö�ʱ��˽�б�ǩ
//������timer,��ʱ��ָ��.
//      Tag���µı�ǩ
//���أ���ʱ����˽�б�ǩ
//-----------------------------------------------------------------------------
bool_t Timer_SetTag(tagTimer* timer,ptu32_t Tag)
{
    if(timer != NULL)
    {
        timer->TimerTag = Tag;
        return true;
    }
    else
        return false;
}

//-----------------------------------------------------------------------------
//���ܣ�ȡ��ʱ��˽�б�ǩ
//������timer,��ʱ��ָ��.
//���أ���ʱ����˽�б�ǩ
//-----------------------------------------------------------------------------
ptu32_t Timer_GetTag(tagTimer* timer)
{
    if(timer != NULL)
        return timer->TimerTag;
    else
        return 0;
}

//-----------------------------------------------------------------------------
//���ܣ�ȡ��ʱ������
//������timersoft,��ʱ��ָ��.
//���أ���ʱ������
//-----------------------------------------------------------------------------
char *Timer_GetName(tagTimer* timer)
{
    if(timer != NULL)
        return timer->name;
    else
        return NULL;
}


// =============================================================================
// �������ܣ�ModuleInstall_Timer
//          ���ⶨʱ����ʼ��ģ��
// ���������para,��ʾ��ʱ��ģ��ʹ��Ӳ����ʱ������tick����ʱ��׼��ȡֵΪ
//              CN_TIMER_SOURCE_TICK��CN_TIMER_SOURCE_HARD
// �����������
// ����ֵ  ��true = �ɹ�  false =ʧ��
// =============================================================================
bool_t ModuleInstall_Timer(void)
{
    static tagTimer ptTimerMem[CFG_TIMERS_LIMIT];
    printk("Timer:Init Start....\n\r");
    ptTimerMemPool = Mb_CreatePool(ptTimerMem,CFG_TIMERS_LIMIT,
                                sizeof(tagTimer),0,0,"Timer");
    if(NULL ==ptTimerMemPool)
    {
        goto EXIT_POOLFAILED;
    }
#if CFG_TIMER_SOUCE_HARD == true        //��Ӳ����ʱ���ṩʱ��Դ
        //ʹ��Ӳ����ʱ����ʱ��Ż�ʹ�ø�ͬ�����
        ptTimerQSync = Lock_MutexCreate("Timer");
        if(NULL == ptTimerQSync)
        {
            goto EXIT_TIMERFAILED;
        }
        sgHardTimerDefault = HardTimer_Alloc(Timer_ISR);
        if((ptu32_t)NULL == sgHardTimerDefault)
        {
            Lock_MutexDelete(ptTimerQSync);
            goto EXIT_TIMERFAILED;
        }
        s_u32TimerPrecision = 1E9/CN_CFG_MCLK;     //����1000��CPU���ڶ�Ӧ��uS����ȡ��
        if(s_u32TimerPrecision == 0)               //�����Ƶ����1G
            s_u32TimerPrecision = 1;
        s_u32TimerFreq = s_u32TimerPrecision*CN_CFG_MCLK/1000000;    //ȡ����s_u32TimerPrecision uS��Ӧ����Ƶ������
        s_u32Precision2Tclk = HardTimer_GetFreq(sgHardTimerDefault);
        //���Ӧ�Ķ�ʱ��������������timerģ���ж�ʱ����Ƶ���С��ʱ���
        s_u32Precision2Tclk =s_u32Precision2Tclk*(s_u32TimerFreq/ 1E6);
        //ʹ�ܶ�ʱ���жϣ�����û��ʹ�ܶ�ʱ��,����API�ĵ���
        HardTimer_Ctrl(sgHardTimerDefault,EN_TIMER_ENINT,(ptu32_t)NULL);
        HardTimer_Ctrl(sgHardTimerDefault,EN_TIMER_SETRELOAD,(ptu32_t)false);
#else   //CFG_TIMER_SOUCE_HARD == true      ��tick�ṩʱ��Դ
        u16 u16EvttId;
        u16 u16EventId;

        //����ͨ���õ���Ϣ����
        ptTimerMsgQ = MsgQ_Create(CN_TIMERSOFT_MSGLEN, \
                                      sizeof(tagTimerMsg),CN_MSGQ_TYPE_FIFO);
        if(NULL == ptTimerMsgQ)
        {
            goto EXIT_TIMERFAILED;
        }
        else
        {
            u16EvttId = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS-2,0,0,
                                   Timer_VMTask,NULL,0x1000,NULL);
            if(CN_EVTT_ID_INVALID == u16EvttId)
            {
                MsgQ_Delete(ptTimerMsgQ);
                ptTimerMsgQ = NULL;
                goto EXIT_TIMERFAILED;
            }
            else
            {
                s_u32TimerPrecision = CN_CFG_TICK_US;   //����=1��tick
                u16EventId = DJY_EventPop(u16EvttId,NULL,0,0,0,0);
                if(CN_EVENT_ID_INVALID == u16EventId)
                {
                    MsgQ_Delete(ptTimerMsgQ);
                    ptTimerMsgQ = NULL;
                    DJY_EvttUnregist(u16EventId);
                    goto EXIT_TIMERFAILED;
                }
            }
        }
#endif  //CFG_TIMER_SOUCE_HARD == true

    printk("Timer:Init Success\n\r");
    return true;

EXIT_TIMERFAILED:
    Mb_DeletePool(ptTimerMemPool);
EXIT_POOLFAILED:
    free(ptTimerMem);
    printk("Timer:Init Failed\n\r");
    return false;
}
