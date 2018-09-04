/*
 * silan_rtc.h
 *
 * Modify Date: 2016-4-28 18:10
 * MaintainedBy: yerenfeng <yerenfeng@silan.com.cn>
 */
 
#ifndef __SILAN_RTC_H__
#define __SILAN_RTC_H__

#include "silan_types.h"

/*
 * set rtc register
 * offset:register offset 
 */
void silan_rtc_reg_set(u32 offset, u8 value);
/*
 *read rtc register
 */
u32 silan_rtc_reg_get(u32 offset);
void silan_soc_ldo_config(u8 level);
void silan_rtc_init(void);


#endif //__SILAN_RTC_H__


