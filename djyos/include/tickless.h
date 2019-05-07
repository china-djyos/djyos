#ifndef __DJY_TICKLESS_H__
#define __DJY_TICKLESS_H__

#include "stdint.h"

enum djytickless_event_t{
    ISR_TICK_DELAY_TOO_LONG,
    EVENT_DELAY,
    RESUME_DELAY,
    RRS_ENABLE,
    RRS_CANCLE,
    ISR_TICK_HANDLE,
    SET_INT_TICK_DEFAULT=0xff
};

struct djytickless_op_t
{
    uint64_t (*get_total_cnt)(void);
    uint64_t (*get_total_cnt_isr)(uint32_t cnt);
    void (*set_reload)(uint32_t cnt);
};

struct djytickless_register_param_t
{
    uint32_t freq;
    uint32_t max_reload_value;
    uint32_t min_reload_value;
    struct djytickless_op_t *op;
};

struct djytickless_param
{
    uint64_t cur_cnt;
    uint64_t next_delay_cnt;
    uint64_t next_rrs_cnt;
};


void DjyTickless_Register(struct djytickless_register_param_t* param);
void DjyTickless_SetReload(struct djytickless_param *param,uint8_t evt);
void DjyTickless_CheckCnt(struct djytickless_param *param,uint32_t cnt);
uint64_t DjyTickless_GetTotalCnt(void);
uint64_t DjyTickless_GetTotalCntIsr(uint32_t cnt);
uint64_t DjyTickless_UsToCnt(uint64_t us);
uint64_t DjyTickless_CntToUs(uint64_t cnt);
uint32_t DjyTickless_GetPrecision(void);

#endif
