
#ifndef CPU_PERI_PWM_H
#define CPU_PERI_PWM_H

#include "stdint.h"

//channel define
#define CN_PWM_CHANNEL0   (0)
#define CN_PWM_CHANNEL1   (1)
#define CN_PWM_CHANNEL2   (2)
#define CN_PWM_CHANNEL3   (3)
#define CN_PWM_CHANNEL4   (4)
#define CN_PWM_CHANNEL5   (5)
#define CN_PWM_CHANNEL6   (6)
#define CN_PWM_CHANNEL7   (7)

//start channel or stop channel
enum
{
    EN_CHANNEL_OFF= 0x0,     //stop channel
	EN_CHANNEL_ON ,             //start channel
};

extern void appPWM_IO_Config(u32 io, u32 x_para, u32 x_period,u8 onOff);
extern void appPWM_ChannelPreDivSet(u32 preDiv);
extern void appPWM_ChannelPeriodSet(u8 channelNum,u32 para);
extern void appPWM_ChannelParaSet(u8 channelNum,u32 para);
extern void appPWM_ChannelOpen(u8 channelNum,u8 onOff);


#endif





















































