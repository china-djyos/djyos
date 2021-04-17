// =============================================================================

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 3/8.2017
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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                             //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                           //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                           //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","stm32L4","cpu onchip gpio","cpu onchip peripheral lowpower control"//������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                     //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                            //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header                       //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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
extern struct IntMasterCtrl  tg_int_global;          //���岢��ʼ�����жϿ��ƽṹ
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
    /*���ϼ�����Ϊ�˼��ݾɰ汾��IBOOT����Ϊ�ɰ汾��IBOOT����ʹ����lptimer��ʱ�������˸ö�ʱ������������Ҫ�ص�*/
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
        //�����ϲ����ܳ��ִ��¼�
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
        __DJY_ScheduleAsynSignal();       //ִ���ж��ڵ���
    g_bScheduleEnable = true;
    return 0;
}

///////////////////////////////////////////////djy-api start//////////////////////////////////
/*ֻ����д__InitTimeBase�������㹻��*/
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

