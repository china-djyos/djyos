// =============================================================================

// 文件名     ：board.c
// 模块描述: 板件相关部分初始化或配置等
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 3/8.2017
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <board-config.h>
#include <stm32L4xx.h>
#include <stm32l4xx_hal_rcc.h>
#include <stm32l4xx_hal_gpio.h>
#include <stm32l4xx_hal_gpio_ex.h>

#include "int.h"
#include "djyos.h"
#include "hard-exp.h"
#include "cpu_peri_lptimer.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                             //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required                           //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                            //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                           //初始化时机，可选值：early，medium，later, pre-main。
                                            //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","stm32L4","cpu onchip gpio","cpu onchip peripheral lowpower control"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件将强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                     //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                            //选中该组件时，被依赖组件不会被强制选中，
                                            //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                            //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header                       //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


void Board_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    //USART2:PA2,PA3
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

#if (CN_USE_TICKLESS_MODE)
#if CN_CFG_USE_USERTIMER
#include "cpu_peri_lptimer.h"
#include "tickless.h"
extern struct IntMasterCtrl  tg_int_global;          //定义并初始化总中断控制结构
extern void __DJY_ScheduleAsynSignal(void);
static uint32_t DjyBsp_UserTimerIsrHandle(uint32_t param);
#define TIME_GLUE           CN_CFG_FINE_US
#define FAST_TIME_GLUE      CN_CFG_FINE_HZ
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(CN_CFG_TIME_PRECISION*TIME_GLUE): \
                                    ((CN_CFG_TIME_PRECISION*CN_CFG_TIME_BASE_HZ)/Mhz))
extern void HardExp_ConnectSystick(void (*tick)(uint32_t inc_ticks));

struct djybsp_user_timer_tickless_t
{
    uint32_t reload_value;
    uint64_t cnt_before;
    uint64_t total_cnt;
}static djybsp_user_timer;

static void Null_Tick(uint32_t inc_ticks)
{
    inc_ticks = *(&inc_ticks);
    return;
}

static void DjyBsp_UserTimerReset(void)
{
    HardExp_ConnectSystick(Null_Tick);
    pg_systick_reg->ctrl &=   ~((1<<bo_systick_ctrl_enable)
                                |(1<<bo_systick_ctrl_tickint)
                                |(1<<bo_systick_ctrl_clksource));
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
    djybsp_user_timer.reload_value = 0;
    djybsp_user_timer.cnt_before = 0;
    djybsp_user_timer.total_cnt = 0;
    /*以上几行是为了兼容旧版本的IBOOT，因为旧版本的IBOOT里面使用了lptimer定时器，打开了该定时器，所以这里要关掉*/
    DjyBsp_LptimerPreInit(0);
}

static void DjyBsp_UserTimerStart(void)
{
    DjyBsp_LptimerInit(0,CN_LIMIT_UINT16,DjyBsp_UserTimerIsrHandle);
    djybsp_user_timer.reload_value = CN_LIMIT_UINT16;
}

static void DjyBsp_UserTimerSetReload(uint32_t cnt)
{
    if(cnt>(CN_LIMIT_UINT16>>1)  || cnt==0)
    {
        //理论上不可能出现此事件
        return;
    }
    DjyBsp_LptimerSetReload(0,(uint16_t)cnt);
    djybsp_user_timer.reload_value = cnt;
}

static uint32_t Djybsp_UserTimerReadCnt(void)
{
    return DjyBsp_LptimerReadCnt(0);
}

static uint64_t DjyBsp_UserTimerGetTotalCntIsr(uint32_t cnt)
{
    return (djybsp_user_timer.total_cnt + cnt);
}

static uint64_t DjyBsp_UserTimerGetTotalCnt(void)
{
    uint64_t temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    temp = djybsp_user_timer.total_cnt + DjyBsp_LptimerReadCnt(0);
    if(temp < djybsp_user_timer.cnt_before)
    {
        temp += CN_LIMIT_UINT16;
    }
    else
        djybsp_user_timer.cnt_before = temp;
    Int_LowAtomEnd(atom_low);
    return temp;
}

static struct djytickless_op_t djyticklss_user_timer_op =
{
    .get_total_cnt = DjyBsp_UserTimerGetTotalCnt,
    .get_total_cnt_isr = DjyBsp_UserTimerGetTotalCntIsr,
    .set_reload = DjyBsp_UserTimerSetReload,
};

static struct djytickless_register_param_t djyticklss_user_timer =
{
     .op = &djyticklss_user_timer_op,
     .freq = CN_CFG_TIME_BASE_HZ,
     .max_reload_value = (CN_LIMIT_UINT16>>1),
     .min_reload_value = TIME_BASE_MIN_GAP,
};

static uint32_t DjyBsp_UserTimerIsrHandle(uint32_t param)
{
    uint32_t flag = 0;
    uint32_t cnt = 0;
    g_bScheduleEnable = false;
    param = *(&param);
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
    flag = DjyBsp_LptimerClearIsrFlag(0);
    switch(flag)
    {
        case CN_LPTIMER_NONE:
            break;
        case CN_LPTIMER_RELOAD:
            djybsp_user_timer.total_cnt += CN_LIMIT_UINT16;
            djybsp_user_timer.cnt_before = djybsp_user_timer.total_cnt;
            break;
        case CN_LPTIMER_CMP:
            cnt = Djybsp_UserTimerReadCnt();
            if(djybsp_user_timer.cnt_before > (djybsp_user_timer.total_cnt + cnt))
                djybsp_user_timer.cnt_before = djybsp_user_timer.total_cnt + cnt + CN_LIMIT_UINT16;
            else
                djybsp_user_timer.cnt_before = djybsp_user_timer.total_cnt + cnt;
            DJY_ScheduleIsr(djybsp_user_timer.cnt_before - djybsp_user_timer.total_cnt);
            break;
        case CN_LPTIMER_RELOAD_AND_CMP:
            djybsp_user_timer.total_cnt += CN_LIMIT_UINT16;
            djybsp_user_timer.cnt_before = djybsp_user_timer.total_cnt;
            DJY_ScheduleIsr(0);
            break;
    }

    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __DJY_ScheduleAsynSignal();       //执行中断内调度
    g_bScheduleEnable = true;
    return 0;
}

///////////////////////////////////////////////djy-api start//////////////////////////////////
/*只需重写__InitTimeBase函数就足够了*/
void __InitTimeBase(void)
{
    DjyTickless_Register(&djyticklss_user_timer);
    DjyBsp_UserTimerReset();
}

void __DjyInitTick(void)
{
    DjyBsp_UserTimerStart();
}
///////////////////////////////////////////////djy-api end//////////////////////////////////
#endif
#endif

