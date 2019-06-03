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
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROcur_cntEMENT OF
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
#include "tickless.h"
#include "stddef.h"
#include "board-config.h"

static struct djytickless_register_param_t
djytickless =
{
    .op = NULL,
    .freq = 0,
    .max_reload_value = 0,
    .min_reload_value = 0,
};

#define DJYTICKLESS_DEBUG      (0U)
/*调试用的，不用管*/
#if     DJYTICKLESS_DEBUG
struct djytickless_debug_t
{
    uint64_t pre_cur_cnt;
    uint64_t next_int_cnt;
    uint64_t pre_next_int_cnt;
    uint32_t pre_reload;
    uint8_t  evt;
    bool_t return_flag;
};
struct djytickless_debug_t djytickless_debug={
    0,
    0,
    0,
    0,
    0,
    false,
};
#endif

// =============================================================================
// 功能：获取定时器能定时的最小cnt数
// 参数：无
// 返回：最小cnt数
// =============================================================================
uint32_t Djytickless_GetPrecision(void)
{
    return djytickless.min_reload_value;
}

// =============================================================================
// 功能：在中断里获取累计的cnt值
// 参数：cnt：中断时传进来的cnt值
// 返回：当前累计的cnt值
// =============================================================================

uint64_t Djytickless_GetTotalCntIsr(uint32_t cnt)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_total_cnt_isr(cnt);
    }
    return 0;
}

// =============================================================================
// 功能：获取当前累计的cnt值
// 参数：无
// 返回：当前累计的cnt值
// =============================================================================
uint64_t Djytickless_GetTotalCnt(void)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_total_cnt();
    }
    return 0;
}

// =============================================================================
// 功能：把cnt值转化为us数
// 参数：cnt值
// 返回：us数
// =============================================================================
uint64_t DjyTickless_CntToUs(uint64_t cnt)
{
    uint64_t temp = 0;
    uint32_t freq = djytickless.freq;
    if(freq!=0)
    {
        if(freq>Mhz)
        {
            temp = cnt/(freq/Mhz);
        }
        else
        {
            temp = cnt*(Mhz/freq);
        }
    }
    return temp;
}

// =============================================================================
// 功能：把us数转化成对应的cnt值
// 参数：us数
// 返回：cnt值
// =============================================================================
uint64_t DjyTickless_UsToCnt(uint64_t us)
{
    uint64_t temp = 0;
    uint32_t freq = djytickless.freq;
    if(freq!=0)
    {
        if(freq>Mhz)
        {
            temp = us*(freq/Mhz);
        }
        else
        {
            temp = us/(Mhz/freq);
        }
        if(temp <= djytickless.min_reload_value)
            temp = djytickless.min_reload_value;
    }
    return temp;
}

// =============================================================================
// 功能：设置下一次定时的时间
// 参数：param:要设置的参数以及当前的参数，evt:发生的事件
// 返回：无
// =============================================================================
void DjyTickless_SetReload(struct djytickless_param *param,uint8_t evt)
{
    static uint64_t next_int_cnt=CN_LIMIT_UINT64; //next_int_cntUs：下一次中断的时间
    uint32_t cnt=0;          //转换成US数后写入reload设置函数
    uint64_t temp_u64=0;
    if(djytickless.op == NULL)
        return;
#if     DJYTICKLESS_DEBUG
    djytickless_debug.pre_next_int_cnt = next_int_cnt;
#endif
    switch(evt)
    {
        case ISR_TICK_DELAY_TOO_LONG:
        //当在DJY_ISR_TICK函数里面检测到正在运行的时钟的reload值为
        //最大值时，则会进入这里，有两种情况：
        //1.此时系统没有延时事件以及轮转事件，系统以最大值作为reload值运行
        //2.此时系统有延时事件或者轮转事件，但是其延时时间比系统时钟的一个周期的延时时间还长，
        //此时也会把reload值设为最大值
            if(next_int_cnt==CN_LIMIT_UINT64 || \
                ((next_int_cnt-(param->cur_cnt))>(djytickless.max_reload_value)))
                //判断next_int_cnt与当前时间的差值是否还比最大延时值还大
                return; //否则就是第二种情况，需在switch外面判断next_int_cntUs值是否还比最大延时值还大
                        //直接退出
            break;
        case ISR_TICK_HANDLE:
        //此事件在isr_tick函数里面被触发
        //case RESUME_DELAY:
        //此事件会在__Djy_ResumeDelay函数里被触发，延时队列头的delay_end_cnt会作为
        //*(param->next_delay_cnt)被传进来，若此时队列已空，则*(param->next_delay_cnt)=0，需要确定一下此时next_int_cntUs是
        //等于*NextRRS，若相等，则无需动作，若不等，则说明此时已经没有需要延时的事件了，直接把
        //next_int_cntUs值设为最大值
        case EVENT_DELAY:
        //此事件会在DJY_EVENT_DELAY函数里被触发，延时队列头的delay_end_cnt会作为
        //*(param->next_delay_cnt)被传进来，若此时next_int_cntUs不为最大值，则需要和*(param->next_delay_cnt)比较，取最小值，
        //否则，直接赋值
        case RRS_ENABLE:
            //此事件在DJY_SELECT_EVENT_TO_RUN中当当前的就绪的线程优先级和其下一个线程的
            //优先级相同时会进入此处
            //当之前定时的时间是延时时间并且比*NextRRS小时，直接返回，否则取他们的最小值
        case RESUME_DELAY:
//      //此事件会在__Djy_ResumeDelay函数里被触发，延时队列头的delay_end_cnt会作为
//      //*(param->next_delay_cnt)被传进来，若此时队列已空，则*(param->next_delay_cnt)=0，需要确定一下此时next_int_cntUs是
//      //等于*NextRRS，若相等，则无需动作，若不等，则说明此时已经没有需要延时的事件了，直接把
//      //next_int_cntUs值设为最大值
        case RRS_CANCLE:
            //取消RRS轮转，当前正在运行的任务的优先级和他下一个任务的优先级不一样的情况下会被触发
            //说明此时已经不需要进行轮转调度，在DJY_SELECT_EVENT_TO_RUN中会被判断触发
            temp_u64 = ((param->next_delay_cnt)>(param->next_rrs_cnt))?(param->next_rrs_cnt):(param->next_delay_cnt);
            if((next_int_cnt>=temp_u64 && ((next_int_cnt - temp_u64)<(djytickless.min_reload_value))) || \
                (next_int_cnt<temp_u64 && ((temp_u64 - next_int_cnt)<(djytickless.min_reload_value))))
            return;
            next_int_cnt = temp_u64;
            break;
        default:
            //正常情况下不会进入此处，但当执行RESUME_DELAY之前中断已经到达，此时有可能会进入此处
            //或者底层设置定时时间的时候直接return了的时候也会进这里
        //    if( ((next_int_cnt==*(param->next_delay_cnt)) && (*(param->next_delay_cnt)<=*NextRRS)) || \
        //            ((next_int_cnt==*NextRRS) && (*NextRRS<=*(param->next_delay_cnt))) )
        //      return;
            next_int_cnt=((param->next_delay_cnt)>(param->next_rrs_cnt))?(param->next_rrs_cnt):(param->next_delay_cnt);
            break;
//            next_int_cnt = ((param->cur_cnt)) + __Djy_TimeBaseUsToCnt(CN_CFG_TICK_US);
//            break;
    }
    //当systick中断已经到了，但是系统却已经关闭了中断的情况下，
    //有可能会出现next_int_cntUs<(param->cur_cnt)Us的情况，有可能会在DJY_SELECT_EVENT_TO_RUN
    //函数里进入此处，此时不需要做什么，直接返回即可，在isr_tick里面会做正确处理
    /*是不是还有一种情况，当插入一个头节点时，*/
    if(next_int_cnt<=(param->cur_cnt))
    {
#if     DJYTICKLESS_DEBUG
        djytickless_debug.return_flag = true;
#endif
        return;
    }
#if     DJYTICKLESS_DEBUG
    djytickless_debug.return_flag = false;
#endif
    //计算需要设置的定时时间长度，其最大值不能比定时器的最大定时时间还长
    cnt=((next_int_cnt-(param->cur_cnt))>(djytickless.max_reload_value))?    \
            (djytickless.max_reload_value):(next_int_cnt-(param->cur_cnt));
    if(cnt<(djytickless.min_reload_value))
        cnt = djytickless.min_reload_value;
#if     DJYTICKLESS_DEBUG
    djytickless_debug.pre_cur_cnt = param->cur_cnt;
    djytickless_debug.pre_reload = cnt;
    djytickless_debug.next_int_cnt = next_int_cnt;
    djytickless_debug.evt = evt;
#endif
    djytickless.op->set_reload(cnt);
}

// =============================================================================
// 功能：检查是否是系统空闲（也就是不需要下一次中断时间时）进入的中断，再做进一步处理
// 参数：param:系统参数，cnt:中断时的reload值
// 返回：无
// =============================================================================
void DjyTickless_CheckCnt(struct djytickless_param *param,uint32_t cnt)
{
    /*这里的逻辑是：当cur_cnt加上min_cnt都小于等于下一次中断的时间时，说明，唤醒时候未到*/
    /*这个函数在djyos.c里面的定时器中断函数里被调用，需注意的是，此逻辑与定时中断函数里面的*/
    /*下面判断下一次delay时间以及下一次轮转时间是否到来的逻辑是互斥的，这里是小于等于，那边是大于*/
    /*也就是说，进了这里，是绝对进不了那边下面的判断，否则就不对*/
    if(cnt==djytickless.max_reload_value && \
            (param->cur_cnt)<=((param->next_delay_cnt) - (djytickless.min_reload_value)) && \
            (param->cur_cnt)<=((param->next_rrs_cnt) - (djytickless.min_reload_value)) )
    {
        DjyTickless_SetReload(param,ISR_TICK_DELAY_TOO_LONG);
        return;
    }
}

// =============================================================================
// 功能：给tickless模块注册ops
// 参数：op
// 返回：无
// =============================================================================
void DjyTickless_Register(struct djytickless_register_param_t* param)
{
    djytickless.op = param->op;
    djytickless.freq = param->freq;
    djytickless.max_reload_value = param->max_reload_value;
    djytickless.min_reload_value = param->min_reload_value;
}


