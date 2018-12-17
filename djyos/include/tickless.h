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
    uint32_t (*get_cnt_max)(void);
    uint32_t (*get_cnt_min)(void);
    uint32_t (*get_reload)(void);
    uint64_t (*refresh_total_cnt)(uint32_t cnt);
    uint64_t (*get_total_cnt)(void);
    uint64_t (*us_to_cnt)(uint64_t us);
    uint64_t (*cnt_to_us)(uint64_t cnt);
    void (*reset)(void);
    void (*start)(void);
    void (*set_reload)(uint32_t cnt);
};

struct djytickless_param
{
    uint64_t cur_cnt;
    uint64_t next_delay_cnt;
    uint64_t next_rrs_cnt;
};

void djytickless_register_op(struct djytickless_op_t* op);
void djytickless_start(void);
void djytickless_reset(void);
void djytickless_set_reload(struct djytickless_param *param,uint8_t evt);
void djytickless_check_cnt(struct djytickless_param *param,uint32_t cnt);
uint64_t djytickless_get_total_cnt(void);
uint64_t djytickless_refresh_total_cnt(uint32_t cnt);
uint64_t djytickless_us_to_cnt(uint64_t us);
uint64_t djytickless_cnt_to_us(uint64_t cnt);
uint32_t djytickless_get_reload(void);
uint32_t djytickless_get_cnt_min(void);
uint32_t djytickless_get_cnt_max(void);

#endif
