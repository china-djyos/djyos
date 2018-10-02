
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

//PWM 时钟源以及分频配置
enum EN_PWM_Clk{
    EN_PWM_SYS_CLK_DIV2 = 0,      //系统时钟二分频
    EN_PWM_SYS_CLK_DIV4,          //系统时钟4分频
    EN_PWM_PLL_REF_CLK_DIV2,      //REF 时钟2分频
    EN_PWM_RCL_CLK_DIV2,          //RCL
};

//PWM 时钟开关配置
enum EN_PWM_ClkSwi{
    EN_PWM_CLK_OFF = 0,
    EN_PWM_CLK_ON,
};

//配置在PWM 输出的管脚
enum EN_PWM_PinDrv{
    EN_IO_DRV_2MS  = 0,
    EN_IO_DRV_4MS,
    EN_IO_DRV_8MS,
    EN_IO_DRV_24MS,
};

//start channel or stop channel
enum
{
    EN_CHANNEL_OFF= 0x0,        //stop channel
	EN_CHANNEL_ON ,             //start channel
};

enum EN_Cmd_Ctrl{
    EN_PWM_PRE_DIV_SET = 0X11,  //设置预分频
    EN_PWM_CLK_SET,             //设置PWM时钟
    EN_IO_DRV_SET,              //设置驱动能力
    EN_PWM_PARA,                //设置占空比、管脚、周期
};

extern bool_t PWM_Ctrl(u32 cmd,u32 para1,u32 para2,u32 para3);
extern void PWM_ModuleInstall(void);

#endif





















































