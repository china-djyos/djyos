/*
 * silan_pwm.h
 */

#ifndef __SILAN_PWM_H__
#define __SILAN_PWM_H__

#include "silan_types.h"

#define IO_TO_PWM(x)		(x % 8)

/*
 * open_num : 0,1,...7
 * x_duty : 0-0xffff
 * x_period : 0-0xffff
 */
int silan_pwm_config(u8 open_num,u32 x_duty, u32 x_period);

int silan_pem_uninit(u8 close_num);

inline int silan_pwm_channel_open(u8 channel_num);
inline int silan_pwm_channel_close(u8 channel_num);

int silan_io_pwm_config(u32 io, u32 x_duty, u32 x_period);
inline int silan_pwm_prediv_set(u32 para);
 
#endif  //__SILAN_PWM_H__

