#include "typedef.h"
#include "pwm_pub.h"
#include "icu_pub.h"
#include "drv_model_pub.h"
#include "driver/entry/arch.h"      //lst
#include "sys_config.h"
#include "power_save_pub.h"

#if CFG_BK7221_MDM_WATCHDOG_PATCH
void rc_reset_patch(void);
#endif

#define FCLK_PWM_ID           PWM0
#define FCLK_DURATION_MS      10
#define FCLK_SECOND           (1000/FCLK_DURATION_MS)

static volatile UINT64 current_clock = 0;
//static volatile UINT32 current_seconds = 0;

extern void Exp_SystickTickHandler(void);

static void fclk_hdl(UINT8 param)
{
    GLOBAL_INT_DECLARATION();
    GLOBAL_INT_DISABLE();

    /*rt_tick_increase();*/
    current_clock += FCLK_DURATION_MS; //PWM 10ms


    Exp_SystickTickHandler();
    GLOBAL_INT_RESTORE();

}





UINT32 fclk_get_tick(void)
{
    return (uint32_t)current_clock;
}

UINT32 fclk_get_second(void)
{
    return current_clock/1000;
}


UINT32 fclk_get_millisecond(void)
{
    return current_clock%1000;
}

UINT32 fclk_cal_endvalue(UINT32 mode)
{
    UINT32 value = 1;

    if(PWM_CLK_32K == mode)
    {   /*32k clock*/
        value = FCLK_DURATION_MS * 32;
    }
    else if(PWM_CLK_26M == mode)
    {   /*26m clock*/
        value = FCLK_DURATION_MS * 26000;
    }

    return value;
}

void os_clk_init(void)
{
    UINT32 ret;
    pwm_param_t param;

    /*init pwm*/
    param.channel         = FCLK_PWM_ID;
    param.cfg.bits.en     = PWM_ENABLE;
    param.cfg.bits.int_en = PWM_INT_EN;
    param.cfg.bits.mode   = PMODE_TIMER;

#if(CFG_RUNNING_PLATFORM == FPGA_PLATFORM)  // FPGA:PWM0-2-32kCLK, pwm3-5-24CLK
    param.cfg.bits.clk    = PWM_CLK_32K;
#else
    param.cfg.bits.clk    = PWM_CLK_26M;
#endif

    param.p_Int_Handler   = fclk_hdl;
    param.duty_cycle      = 0;
    param.end_value       = fclk_cal_endvalue((UINT32)param.cfg.bits.clk);

    ret = sddev_control(PWM_DEV_NAME, CMD_PWM_INIT_PARAM, &param);
    ASSERT(PWM_SUCCESS == ret);
}

// eof
#include "pwm.h"

//BK7251无法读出定时器当前值，无论定时器还是PWM
//u32 Get_SysTickCnt()
//{
//    return REG_READ(REG_APB_BK_PWMn_END_ADDR(FCLK_PWM_ID));
//}

void Set_SysTickEnd(u32 value)
{
    REG_WRITE(REG_APB_BK_PWMn_END_ADDR(FCLK_PWM_ID), value);
}
