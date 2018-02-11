#include "cpu_peri.h"?
#include <djyos.h>
#include <stdint.h>
#include <stdio.h>
#include <systime.h>
#include "shell.h"
#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"
// =============================================================================
// 备注：LED1-  GPIOF   GPIO_PIN_10
//     LED3-  GPIOA   GPIO_PIN_4
// 参数:无
// 返回: 无
// =============================================================================
ptu32_t Led_Test(void)
{
	for(;;)
	{
	    HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_10);
	    Djy_EventDelay(500*mS);
		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_4 );
        Djy_EventDelay(500*mS);
	}
	return 0;
}
// =============================================================================
// 功能:led初始化
// 参数:无
// 返回: 无
// =============================================================================
void led_init(void)
{
	u16 evtt_led;

    evtt_led = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
    						Led_Test,NULL,1000,"Led_Test");
    if(evtt_led!=CN_EVTT_ID_INVALID)
    	Djy_EventPop(evtt_led,(u32 *)NULL,0,(ptu32_t)NULL,0,0);
}
// =============================================================================
// 功能:main 函数
// 参数:无
// 返回: 无
// =============================================================================
u8 flag = 0;
void djy_main(void)
{
    led_init();
    for(;;)
    {
        if(flag^=1)
        {
            if(0)
                printf("RUN MODE Iboot\n\r");
        }
    	Djy_EventDelay(500*mS);
    }
}


