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
#include <stdlib.h>
#include "djytimer.h"
#include "djyq.h"
#include "djyos.h"
#include "systime.h"
#include "int.h"

static struct djytimer_info_t
{
    struct djyq_t timerlist;
}djytimer;

enum djytimer_status_e djytimer_get_status(struct djytimer_t *timer)
{
    if(timer==NULL)
        return DJYTIMER_ERROR;
    else
        return timer->status;
}

void djytimer_stop(struct djytimer_t *timer)
{
    atom_low_t low = 0;
    low = Int_LowAtomStart();
    djyq_remove(&djytimer.timerlist, &timer->node);
    timer->status = DJYTIMER_STOP;
    Int_LowAtomEnd(low);
}

void djytimer_start(struct djytimer_t *timer)
{
    atom_low_t low = 0;
    timer->node.addr = timer->interval + (uint32_t)(DjyGetSysTime()/1000);
    djytimer_stop(timer);

    low = Int_LowAtomStart();
    djyq_enqueue(&djytimer.timerlist, &timer->node);
    timer->status = DJYTIMER_RUNNIG;
    Int_LowAtomEnd(low);
}

struct djytimer_t *djytimer_create(uint32_t interval, int16_t trigger_cnt,
                                        void (*cb)(void *), void *param)
{
    atom_low_t low = 0;
    low = Int_LowAtomStart();
    struct djytimer_t *timer = malloc(sizeof(struct djytimer_t));
    Int_LowAtomEnd(low);

    if (NULL == timer)
    {
        return NULL;
    }
    timer->interval = interval;
    timer->trigger_cnt = trigger_cnt;
    timer->cb = cb;
    timer->param = param;
    timer->status = DJYTIMER_INIT;
    return timer;
}

void djytimer_free(struct djytimer_t *timer)
{
    atom_low_t low = 0;
    djytimer_stop(timer);
    low = Int_LowAtomStart();
    free(timer);
    Int_LowAtomEnd(low);
}

static bool_t djytimer_should_trigger(uint32_t cur_tick, uint32_t timer)
{
    return cur_tick >= timer ?
            (cur_tick - timer) < (1UL << (sizeof(uint32_t) * 8 - 1)) :
            (timer - cur_tick) >= (1UL << (sizeof(uint32_t) * 8 - 1));
}

static ptu32_t djytimer_thread(void)
{
    uint32_t cur_tick = (uint32_t)(DjyGetSysTime()/1000);
    struct djytimer_t *timer = NULL;
    while(1)
    {
        timer = (struct djytimer_t *)djytimer.timerlist.head;
        while(timer != NULL)
        {
            if(djytimer_should_trigger(cur_tick, timer->node.addr))
            {
                if (timer->trigger_cnt > 0)
                {
                    timer->trigger_cnt--;
                }
                if (timer->trigger_cnt != 0)
                {
                    djytimer_start(timer);
                }
                else
                {
                    djytimer_free(timer);
                }
                if(timer->cb != NULL)
                {
                    timer->cb(timer->param);
                }
                timer = (struct djytimer_t *)djytimer.timerlist.head;
            }
            else
            {
                break;
            }
        }
        Djy_EventDelay(1*mS);
        cur_tick++;
    }
    return 0;
}

void djytimer_init(void)
{
    uint16_t evtt_timer;
    djyq_init(&djytimer.timerlist);
    evtt_timer = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            djytimer_thread,NULL,4096, "timer function");
    Djy_EventPop(evtt_timer,NULL,0,NULL,0,0);
}

