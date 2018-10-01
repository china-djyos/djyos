// =============================================================================
// Copyright (C) 2012-2020 ��԰�̱��Զ������޹�˾ All Rights Reserved
// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: zhb
// ����ʱ��: 3/8.2018
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <cpu_peri.h>
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
//component name:"stm32l475_demo_board"      //�����������
//parent:"none"                              //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                          //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                             //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                           //��ʼ��ʱ������ѡֵ��early��medium��later��
                                             //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"                          //������������������������none����ʾ�������������
                                             //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                             //����������������������г����á�,���ָ�
//weakdependence:"none"                      //��������������������������none����ʾ�������������
                                             //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                             //����������������������г����á�,���ָ�
//mutex:"none"                               //������������������������none����ʾ�������������
                                             //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header                        //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
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
    Lptimer1_PreInit();
}

void __DjyStartTimeBase(void)
{
    Lptimer1_Init(CN_LIMIT_UINT16,__DjyIsrTimeBase);
}

//??��??����??��������?��?�䨮?��
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
    __HAL_RCC_GPIOD_CLK_ENABLE();

    //UART4:PA0,PA1,SIMCOM��GPRS��
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART4;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //USART2:PA2,PA3
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //USART1:PA9,PA10,RF
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //USART3:PC4,PC5,SIM28ML��GPS��
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    //UART5:PC12,PD2,debug print
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_UART5;
    GPIO_InitStruct.Pin = GPIO_PIN_12;                  //UART5_TX
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    GPIO_InitStruct.Pin = GPIO_PIN_2;                   //UART5_RX
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    //SPI1:PA5,PA6,PA7,SPIFLASH & ����
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    //SPI1:CS1(PA4),CS2(PB0)
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);

    //FLASH_RST:PD3 (IO_CTL8)
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, SET);

    //AT24,IIC:PB6,PB7
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //AT24,EP:PB8 (IO_CTL5)
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //SPI2:PB13,PB14,PB15
    GPIO_InitStruct.Mode  = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    //SPI2:CS1(PB11),CS2(PB12)
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);

    //AD0~AD1: PC0~PC1
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    //CFG0

    //COM_CTL0~COM_CTL6

    //IO0~IO8

    SystemCoreClock = CN_CFG_MCLK;
}


bool_t Board_SpiCsCtrl(u8 port,u8 cs,u8 state)
{
    switch(port)
    {
    case CN_SPI1:
        if(cs==0)       //AT45 FLASH
        {
            if(state)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, RESET);
            }
        }
        else
        {
            if(state)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
            }
        }
        break;
    case CN_SPI2:
        if(cs==0)
        {
            if(state)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, RESET);
            }
        }
        else
        {
            if(state)
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);
            }
        }
        break;
    default:
        break;
    }
    return true;
}


void AT24_WPEn(u8 en)
{
    if(en)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, RESET);
    }
}


// =============================================================================
// ���Ź���ʼ��
void Wdt_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Wdt_FeedDog(void)
{
    HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}
