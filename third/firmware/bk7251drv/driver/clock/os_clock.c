#include "typedef.h"
#include "pwm_pub.h"
#include "icu_pub.h"
#include "drv_model_pub.h"
#include "arch.h"
#include "sys_config.h"

#define FCLK_PWM_ID           PWM0
#define FCLK_DURATION_MS      1
#define FCLK_SECOND           (1000/FCLK_DURATION_MS)
extern void Exp_SystickTickHandler(void);

static void fclk_hdl(UINT8 param)
{
	GLOBAL_INT_DECLARATION();
	GLOBAL_INT_DISABLE();

    /*rt_tick_increase();*/
    Exp_SystickTickHandler();
	GLOBAL_INT_RESTORE();
}


UINT32 fclk_cal_endvalue(UINT32 mode)
{
	UINT32 value = 1;
	
	if(PWM_CLK_32K == mode)
	{	/*32k clock*/
		value = FCLK_DURATION_MS * 32;
	}
	else if(PWM_CLK_26M == mode)
	{	/*26m clock*/
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
//	ASSERT(PWM_SUCCESS == ret);
}

// eof

