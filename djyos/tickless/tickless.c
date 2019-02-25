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
#include "tickless.h"
#include "stddef.h"
#include "board-config.h"

struct djytickless_t {
    struct djytickless_op_t *op;
}static djytickless = {
    .op = NULL,
};

#define DJYTICKLESS_DEBUG      (0U)
/*�����õģ����ù�*/
#if     DJYTICKLESS_DEBUG
struct djytickless_debug_t{
    uint64_t pre_cur_cnt;
    uint64_t next_int_cnt;
    uint64_t pre_next_int_cnt;
    uint32_t pre_reload;
    uint8_t  evt;
    bool_t return_flag;
}djytickless_debug={
    0,
    0,
    0,
    0,
    0,
    false,
};
#endif
// =============================================================================
// ���ܣ���ȡ��ʱ���ܶ�ʱ�����cnt��
// ��������
// ���أ����cnt��
// =============================================================================
uint32_t DjyTickless_GetCntMax(void)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_cnt_max();
    }
    return 0;
}

// =============================================================================
// ���ܣ���ȡ��ʱ���ܶ�ʱ����Сcnt��
// ��������
// ���أ���Сcnt��
// =============================================================================
uint32_t DjyTickless_GetCntMin(void)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_cnt_min();
    }
    return 0;
}

// =============================================================================
// ���ܣ�����systick�Ķ�ʱ����
// ������Ҫ���õ�ֵ
// ���أ���
// =============================================================================
uint32_t DjyTickless_GetReload(void)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_reload();
    }
    return 0;
}

// =============================================================================
// ���ܣ�ˢ�µ�ǰ�ۼƵ�cntֵ
// ������������cnt��
// ���أ���ǰ�ۼƵ�cnt��
// =============================================================================
uint64_t DjyTickless_RefreshTotalCnt(uint32_t cnt)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->refresh_total_cnt(cnt);
    }
    return 0;
}

// =============================================================================
// ���ܣ���ȡ��ǰ�ۼƵ�cntֵ
// ��������
// ���أ���ǰ�ۼƵ�cntֵ
// =============================================================================
uint64_t DjyTickless_GetTotalCnt(void)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->get_total_cnt();
    }
    return 0;
}

// =============================================================================
// ���ܣ���λticklessģ��
// ��������
// ���أ���
// =============================================================================
void DjyTickless_Reset(void)
{
    if(djytickless.op != NULL)
    {
        djytickless.op->reset();
    }
    return;
}

// =============================================================================
// ���ܣ�����ticklessģ��
// ��������
// ���أ���
// =============================================================================
void DjyTickless_Start(void)
{
    if(djytickless.op != NULL)
    {
        djytickless.op->start();
    }
    return;
}

// =============================================================================
// ���ܣ���cntֵת��Ϊus��
// ������cntֵ
// ���أ�us��
// =============================================================================
uint64_t DjyTickless_CntToUs(uint64_t cnt)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->cnt_to_us(cnt);
    }
    return 0;
}

// =============================================================================
// ���ܣ���us��ת���ɶ�Ӧ��cntֵ
// ������us��
// ���أ�cntֵ
// =============================================================================
uint64_t DjyTickless_UsToCnt(uint64_t us)
{
    if(djytickless.op != NULL)
    {
        return djytickless.op->us_to_cnt(us);
    }
    return 0;
}

// =============================================================================
// ���ܣ�������һ�ζ�ʱ��ʱ��
// ������param:Ҫ���õĲ����Լ���ǰ�Ĳ�����evt:�������¼�
// ���أ���
// =============================================================================
void DjyTickless_SetReload(struct djytickless_param *param,uint8_t evt)
{
    static uint64_t next_int_cnt=CN_LIMIT_UINT64; //next_int_cntUs����һ���жϵ�ʱ��
    uint32_t cnt=0;          //ת����US����д��reload���ú���
    uint64_t temp_u64=0;
    if(djytickless.op == NULL)
        return;
#if     DJYTICKLESS_DEBUG
    djytickless_debug.pre_next_int_cnt = next_int_cnt;
#endif
    switch(evt)
    {
        case ISR_TICK_DELAY_TOO_LONG:
        //����DJY_ISR_TICK���������⵽�������е�ʱ�ӵ�reloadֵΪ
        //���ֵʱ����������������������
        //1.��ʱϵͳû����ʱ�¼��Լ���ת�¼���ϵͳ�����ֵ��Ϊreloadֵ����
        //2.��ʱϵͳ����ʱ�¼�������ת�¼�����������ʱʱ���ϵͳʱ�ӵ�һ�����ڵ���ʱʱ�仹����
        //��ʱҲ���reloadֵ��Ϊ���ֵ
            if(next_int_cnt==CN_LIMIT_UINT64 || \
                ((next_int_cnt-(param->cur_cnt))>(djytickless.op->get_cnt_max())))
                //�ж�next_int_cnt�뵱ǰʱ��Ĳ�ֵ�Ƿ񻹱������ʱֵ����
                return; //������ǵڶ������������switch�����ж�next_int_cntUsֵ�Ƿ񻹱������ʱֵ����
                        //ֱ���˳�
            break;
        case ISR_TICK_HANDLE:
        //���¼���isr_tick�������汻����
        //case RESUME_DELAY:
        //���¼�����__Djy_ResumeDelay�����ﱻ��������ʱ����ͷ��delay_end_cnt����Ϊ
        //*(param->next_delay_cnt)��������������ʱ�����ѿգ���*(param->next_delay_cnt)=0����Ҫȷ��һ�´�ʱnext_int_cntUs��
        //����*NextRRS������ȣ������趯���������ȣ���˵����ʱ�Ѿ�û����Ҫ��ʱ���¼��ˣ�ֱ�Ӱ�
        //next_int_cntUsֵ��Ϊ���ֵ
        case EVENT_DELAY:
        //���¼�����DJY_EVENT_DELAY�����ﱻ��������ʱ����ͷ��delay_end_cnt����Ϊ
        //*(param->next_delay_cnt)��������������ʱnext_int_cntUs��Ϊ���ֵ������Ҫ��*(param->next_delay_cnt)�Ƚϣ�ȡ��Сֵ��
        //����ֱ�Ӹ�ֵ
        case RRS_ENABLE:
            //���¼���DJY_SELECT_EVENT_TO_RUN�е���ǰ�ľ������߳����ȼ�������һ���̵߳�
            //���ȼ���ͬʱ�����˴�
            //��֮ǰ��ʱ��ʱ������ʱʱ�䲢�ұ�*NextRRSСʱ��ֱ�ӷ��أ�����ȡ���ǵ���Сֵ
        case RESUME_DELAY:
//      //���¼�����__Djy_ResumeDelay�����ﱻ��������ʱ����ͷ��delay_end_cnt����Ϊ
//      //*(param->next_delay_cnt)��������������ʱ�����ѿգ���*(param->next_delay_cnt)=0����Ҫȷ��һ�´�ʱnext_int_cntUs��
//      //����*NextRRS������ȣ������趯���������ȣ���˵����ʱ�Ѿ�û����Ҫ��ʱ���¼��ˣ�ֱ�Ӱ�
//      //next_int_cntUsֵ��Ϊ���ֵ
        case RRS_CANCLE:
            //ȡ��RRS��ת����ǰ�������е���������ȼ�������һ����������ȼ���һ��������»ᱻ����
            //˵����ʱ�Ѿ�����Ҫ������ת���ȣ���DJY_SELECT_EVENT_TO_RUN�лᱻ�жϴ���
            temp_u64 = ((param->next_delay_cnt)>(param->next_rrs_cnt))?(param->next_rrs_cnt):(param->next_delay_cnt);
            if((next_int_cnt>=temp_u64 && ((next_int_cnt - temp_u64)<(djytickless.op->get_cnt_min()))) || \
                (next_int_cnt<temp_u64 && ((temp_u64 - next_int_cnt)<(djytickless.op->get_cnt_min()))))
            return;
            next_int_cnt = temp_u64;
            break;
        default:
            //��������²������˴�������ִ��RESUME_DELAY֮ǰ�ж��Ѿ������ʱ�п��ܻ����˴�
            //���ߵײ����ö�ʱʱ���ʱ��ֱ��return�˵�ʱ��Ҳ�������
        //    if( ((next_int_cnt==*(param->next_delay_cnt)) && (*(param->next_delay_cnt)<=*NextRRS)) || \
        //            ((next_int_cnt==*NextRRS) && (*NextRRS<=*(param->next_delay_cnt))) )
        //      return;
            next_int_cnt=((param->next_delay_cnt)>(param->next_rrs_cnt))?(param->next_rrs_cnt):(param->next_delay_cnt);
            break;
//            next_int_cnt = ((param->cur_cnt)) + __Djy_TimeBaseUsToCnt(CN_CFG_TICK_US);
//            break;
    }
    //��systick�ж��Ѿ����ˣ�����ϵͳȴ�Ѿ��ر����жϵ�����£�
    //�п��ܻ����next_int_cntUs<(param->cur_cnt)Us��������п��ܻ���DJY_SELECT_EVENT_TO_RUN
    //���������˴�����ʱ����Ҫ��ʲô��ֱ�ӷ��ؼ��ɣ���isr_tick���������ȷ����
    /*�ǲ��ǻ���һ�������������һ��ͷ�ڵ�ʱ��*/
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
    //������Ҫ���õĶ�ʱʱ�䳤�ȣ������ֵ���ܱȶ�ʱ�������ʱʱ�仹��
    cnt=((next_int_cnt-(param->cur_cnt))>(djytickless.op->get_cnt_max()))?    \
            (djytickless.op->get_cnt_max()):(next_int_cnt-(param->cur_cnt));
    if(cnt<(djytickless.op->get_cnt_min()))
        cnt = djytickless.op->get_cnt_min();
#if     DJYTICKLESS_DEBUG
    djytickless_debug.pre_cur_cnt = param->cur_cnt;
    djytickless_debug.pre_reload = cnt;
    djytickless_debug.next_int_cnt = next_int_cnt;
    djytickless_debug.evt = evt;
#endif
    djytickless.op->set_reload(cnt);
}

// =============================================================================
// ���ܣ�����Ƿ���ϵͳ���У�Ҳ���ǲ���Ҫ��һ���ж�ʱ��ʱ��������жϣ�������һ������
// ������param:ϵͳ������cnt:�ж�ʱ��reloadֵ
// ���أ���
// =============================================================================
void DjyTickless_CheckCnt(struct djytickless_param *param,uint32_t cnt)
{
    /*������߼��ǣ���cur_cnt����min_cnt��С�ڵ�����һ���жϵ�ʱ��ʱ��˵��������ʱ��δ��*/
    /*���������djyos.c����Ķ�ʱ���жϺ����ﱻ���ã���ע����ǣ����߼��붨ʱ�жϺ��������*/
    /*�����ж���һ��delayʱ���Լ���һ����תʱ���Ƿ������߼��ǻ���ģ�������С�ڵ��ڣ��Ǳ��Ǵ���*/
    /*Ҳ����˵����������Ǿ��Խ������Ǳ�������жϣ�����Ͳ���*/
    if(cnt==djytickless.op->get_cnt_max() && \
            (param->cur_cnt)<=((param->next_delay_cnt) - (djytickless.op->get_cnt_min())) && \
            (param->cur_cnt)<=((param->next_rrs_cnt) - (djytickless.op->get_cnt_min())) )
    {
        DjyTickless_SetReload(param,ISR_TICK_DELAY_TOO_LONG);
        return;
    }
}

// =============================================================================
// ���ܣ���ticklessģ��ע��ops
// ������op
// ���أ���
// =============================================================================
void DjyTickless_RegisterOp(struct djytickless_op_t* op)
{
    djytickless.op = op;
}


