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

void DjyTickless_RegisterOp(struct djytickless_op_t* op);
void DjyTickless_Reset(void);
void DjyTickless_Reset(void);
void DjyTickless_SetReload(struct djytickless_param *param,uint8_t evt);
void DjyTickless_CheckCnt(struct djytickless_param *param,uint32_t cnt);
uint64_t DjyTickless_GetTotalCnt(void);
uint64_t DjyTickless_RefreshTotalCnt(uint32_t cnt);
uint64_t DjyTickless_UsToCnt(uint64_t us);
uint64_t DjyTickless_CntToUs(uint64_t cnt);
uint32_t DjyTickless_GetReload(void);
uint32_t DjyTickless_GetCntMin(void);
uint32_t DjyTickless_GetCntMax(void);

#endif
