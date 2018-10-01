// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
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
//    extern void Board_GpioInit(void);
//    Board_GpioInit();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"nucleo_l476rg_board"      //�����������
//parent:"none"                             //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                         //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                               //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                          //��ʼ��ʱ������ѡֵ��early��medium��later��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"cpu_peri_gpio","cpu_peri_lowpower"                //������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                     //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                              //������������������������none����ʾ�������������
                                            //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header                       //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure


extern u32 SystemCoreClock;
#if CN_CFG_USE_USERTIMER
extern struct IntMasterCtrl  tg_int_global;          //?����?2��3?��??������?D???????��11
extern void __Djy_ScheduleAsynSignal(void);
static void __DjyIsrTimeBase(u32 param);
#define CN_TIME_ROUNDING    (32768U)//���������ֵ
#define TIME_GLUE           (0x1E849CU)
#define FAST_TIME_GLUE      (0x863U)
#define TIME_BASE_MIN_GAP   (CN_CFG_TIME_BASE_HZ>Mhz?(100*TIME_GLUE):((200*CN_CFG_TIME_BASE_HZ)/Mhz))
static u64 g_time_base_tick=0;
static u64 g_per_sys_cnt = 0;
extern void HardExp_ConnectSystick(void (*tick)(u32 inc_ticks));

static void Null_Tick(u32 inc_ticks)
{
    return;
}

void __InitTimeBase(void)
{
    HardExp_ConnectSystick(Null_Tick);
    pg_systick_reg->ctrl &=   ~((1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource));//���ں�ʱ��
    pg_systick_reg->reload = 0;
    pg_systick_reg->current = 0;
    /*���ϼ�����Ϊ�˼��ݾɰ汾��IBOOT����Ϊ�ɰ汾��IBOOT����ʹ����SYSTICK��ʱ�������˸ö�ʱ������������Ҫ�ص�*/
    Lptimer1_PreInit();
}

void __DjyStartTimeBase(void)
{
    Lptimer1_Init(CN_LIMIT_UINT16,__DjyIsrTimeBase);
}

//��ȡ���Զ�ʱ�����ֵ
u32 __Djy_GetDelayMaxCnt(void)
{
    return (CN_LIMIT_UINT16>>1);
}

u32 __Djy_GetTimeBaseGap(void)
{
    return TIME_BASE_MIN_GAP;
}

void __Djy_SetTimeBaseCnt(u32 cnt)
{
    if(cnt>CN_LIMIT_UINT16 || cnt==0)
    {
        //����??��?2??��?��3???��?��??t
        return;
    }
    Lptimer1_set_period((u16)cnt);
}

static u32 __Djy_GetTimeBaseRealCnt(void)
{
    return Lptimer1_read_cnt();
}

u32 __Djy_GetTimeBaseReload(void)
{
    return Lptimer1_read_reload();
}

u64 __Djy_TimeBaseUsToCnt(u64 us)
{
    u64 temp = 0;
    temp = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (us*TIME_GLUE):
            ((us*FAST_TIME_GLUE + CN_TIME_ROUNDING))>>16);
    if( temp < TIME_BASE_MIN_GAP )
        temp = TIME_BASE_MIN_GAP;
    return temp;
}

u32 __Djy_TimeBaseCntToUs(u64 cnt)
{
    return ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (cnt/(u32)TIME_GLUE):
            (u32)(((u64)(cnt*TIME_GLUE))>>16));
}

u64 __Djy_GetTimeBaseCnt(u32 cnt)
{
    return (g_time_base_tick + cnt);
}

u64 __DjyGetSysCnt(void)
{
    u64 temp = 0;
    atom_low_t atom_low;
    atom_low = Int_LowAtomStart();
    temp = g_time_base_tick + __Djy_GetTimeBaseRealCnt();
    if(temp < g_per_sys_cnt)
    {
        temp += CN_LIMIT_UINT16;
    }
    else
        g_per_sys_cnt = temp;
    Int_LowAtomEnd(atom_low);
    return temp;
}

u64 __DjyGetSysTime(void)
{
    u64 time;
    u64 temp=0;
    temp = __DjyGetSysCnt();
    time = ((CN_CFG_TIME_BASE_HZ>Mhz)?
            (temp/(u32)TIME_GLUE)://����û�а취��ֻ��ֱ��ʹ�ó��������������ۼ�����������  --chj
            ((u64)(temp*TIME_GLUE))>>16);
    return time;
}

static void __DjyIsrTimeBase(u32 param)
{
    u8 flag = 0;
    u32 tick=0;
    g_bScheduleEnable = false;
    tg_int_global.en_asyn_signal_counter = 1;
    tg_int_global.nest_asyn_signal = 1;
    flag = Lptimer1_ClearISR();
    switch(flag)
    {
        case CN_LPTIMER_NONE:
            break;
        case CN_LPTIMER_RELOAD:
            g_time_base_tick += CN_LIMIT_UINT16;
            g_per_sys_cnt = g_time_base_tick;
            break;
        case CN_LPTIMER_CMP:
            tick=__Djy_GetTimeBaseRealCnt();
            g_per_sys_cnt = g_time_base_tick + tick;
            Djy_IsrTimeBase(tick);
            break;
        case CN_LPTIMER_RELOAD_AND_CMP:
            g_time_base_tick += CN_LIMIT_UINT16;
            g_per_sys_cnt = g_time_base_tick;
            //tick=__Djy_GetTimeBaseRealCnt();
            Djy_IsrTimeBase(0);
            break;
    }

    tg_int_global.nest_asyn_signal = 0;
    tg_int_global.en_asyn_signal_counter = 0;
    if(g_ptEventReady != g_ptEventRunning)
        __Djy_ScheduleAsynSignal();       //?��DD?D???���̡�?��
    g_bScheduleEnable = true;
}

#endif


void Board_GpioInit(void)
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

    SystemCoreClock = CN_CFG_MCLK;
}

