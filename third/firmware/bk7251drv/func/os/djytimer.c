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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdlib.h>
#include "djytimer.h"
#include "djyq.h"
#include "djyos.h"
#include "lock.h"
#include "systime.h"
#include "int.h"

static struct djytimer_info_t
{
    struct djyq_t timerlist;
    struct SemaphoreLCB *sem;
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
    struct djytimer_t *tmp_timer = NULL;
    atom_low_t low = 0;
    timer->node.addr = timer->interval + (uint32_t)(DJY_GetSysTime()/1000);
    tmp_timer = djytimer.timerlist.head;
    djytimer_stop(timer);

    low = Int_LowAtomStart();
    djyq_enqueue(&djytimer.timerlist, &timer->node);
    timer->status = DJYTIMER_RUNNIG;
    if((timer==(struct djytimer_t *)djytimer.timerlist.head) && (tmp_timer!=timer))
        Lock_SempPost(djytimer.sem);
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
    uint32_t cur_tick = 0;
    uint32_t delay_ms = 0;
    struct djytimer_t *timer = NULL;
    while(1)
    {
        cur_tick = (uint32_t)(DJY_GetSysTime()/1000);
        timer = (struct djytimer_t *)djytimer.timerlist.head;
        while(timer != NULL)
        {
            if(djytimer_should_trigger(cur_tick, timer->node.addr))
            {
                timer->status = DJYTIMER_ISR;
                if(timer->cb != NULL)
                {
                    timer->cb(timer->param);
                }
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
                    if(timer->status == DJYTIMER_ISR)
                        djytimer_stop(timer);
                }
                timer = (struct djytimer_t *)djytimer.timerlist.head;
            }
            else
            {
                break;
            }
        }
        timer = (struct djytimer_t *)djytimer.timerlist.head;
        if(timer==NULL)
        {
            Lock_SempPend(djytimer.sem,CN_TIMEOUT_FOREVER);
            timer = (struct djytimer_t *)djytimer.timerlist.head;
        }
        cur_tick = (uint32_t)(DJY_GetSysTime()/1000);
        delay_ms = timer->node.addr > cur_tick ?
                (timer->node.addr - cur_tick) : (CN_LIMIT_SINT32 - cur_tick + timer->node.addr);
        Lock_SempPend(djytimer.sem,delay_ms*mS);
    }
    return 0;
}

void djytimer_init(void)
{
    uint16_t evtt_timer;
    djyq_init(&djytimer.timerlist);
    djytimer.sem = Lock_SempCreate(1,0,CN_BLOCK_FIFO,NULL);
    if(djytimer.sem==NULL)
        goto SEM_INIT_ERR;
    evtt_timer = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            djytimer_thread,NULL,4096, "timer function");
    if(evtt_timer==CN_EVTT_ID_INVALID)
        goto EVTT_REGIST_ERR;
    if(DJY_EventPop(evtt_timer,NULL,0,NULL,0,0)==CN_EVTT_ID_INVALID)
        goto EVENT_POP_ERR;
    else
        return;
EVENT_POP_ERR:
    DJY_EvttUnregist(evtt_timer);
EVTT_REGIST_ERR:
    Lock_SempDelete(djytimer.sem);
SEM_INIT_ERR:
    return;
}

