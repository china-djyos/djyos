/*
 * silan_pwm.c
 */

//#define __DEBUG__

#include "silan_pwm.h"
#include "ap1508.h"
#include "silan_pwm_regs.h"
#include "silan_types.h"
#include "silan_printf.h"
#include "silan_syscfg.h"
#include "silan_iomux.h"

 inline int silan_pwm_channel_open(u8 channel_num)
{
	if(channel_num > 7)
		return -1;
	PWMCON |=  (1<<(channel_num + 4 ));
	return 0;
}
 inline int silan_pwm_channel_close(u8 channel_num)
{
	if(channel_num > 7)
		return -1;
	PWMCON &= ~(1<<(channel_num + 4));
	return 0;
}
static inline int silan_pwm_duty_set(u8 channel_num ,u32 para)
{
	if(channel_num > 7)
		return -1;
	__REG32(SILAN_PWM_REGS_BASE+0x04+channel_num*4) = (para-1);
	return 0;
}	
static inline int silan_pwm_period_set(u8 channel_num_2 ,u32 para)
{
	if(channel_num_2 > 7)
		return -1;
	__REG32(SILAN_PWM_REGS_BASE+0x24+channel_num_2/2*4 ) = (para-1);
	return 0;
}

inline int silan_pwm_prediv_set(u32 para)
{
	if((para < 1) || (para > 256))
		return -1;
	PWMPSC = para-1;
	return 0;
}

int silan_pwm_config(u8 open_num,u32 x_duty, u32 x_period)
{
	if(open_num > 7)
		return -1;
	if(!x_duty)
		return -1;
	if(!x_period)
		return -1;

	silan_pwm_duty_set(open_num,x_duty);
	silan_pwm_period_set(open_num,x_period);

	return 0;
}

int silan_io_pwm_config(u32 io, u32 x_duty, u32 x_period)
{
	u32 open_num;

	open_num = IO_TO_PWM(io);
	silan_pwm_config(open_num, x_duty, x_period);
	silan_pwm_channel_open(open_num);

	silan_io_func_config(io, IO_FUNC_PWM);
}

