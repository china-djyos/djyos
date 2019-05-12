// =============================================================================

// �ļ���     ��cpu_peri_hal.c
// ģ������: HAL��Ҫʹ�õ��Ľӿں���
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 2/20.2017
// =============================================================================

#include <stdio.h>
#include <stddef.h>
#include "cpu_peri.h"
#include <os.h>
#include <board-config.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

#ifdef USE_HAL_DRIVER
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������
//%$#@describe      ****���������ʼ
//component name:"cpu driver hal"//stm32��hal�ⲹ��
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:none                //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header              //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

//==============================================================================
//HAL_RCC���õ���������ΪDJYOSʹ���Լ���ʱ�ӳ�ʼ�����̣���ˣ���ʹ��HAL���ʱ����ЩHAL
//�ڲ�����ø����������˴���ֵ
//����ETH���������У�stm32f7xx_hal_eth.c�л����
uint32_t SystemCoreClock = CN_CFG_MCLK;         //HAL_RCC���õ�����

#if(CFG_STM32_HAL_TIMER == 1)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM1;
#elif(CFG_STM32_HAL_TIMER == 2)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM2;
#elif(CFG_STM32_HAL_TIMER == 3)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM3;
#elif(CFG_STM32_HAL_TIMER == 4)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM4;
#elif(CFG_STM32_HAL_TIMER == 5)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM5;
#elif(CFG_STM32_HAL_TIMER == 6)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM6;
#elif(CFG_STM32_HAL_TIMER == 7)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM7;
#elif(CFG_STM32_HAL_TIMER == 8)
    TIM_TypeDef* const STM32_HAL_TIMER = TIM8;
#else
#error "error �� û�����ö�ʱ������";
#endif
//static TIM_HandleTypeDef HalTimHandle;
//HAL�������øú�����ʱ���������ˣ����õ�HAL�⣬����Ҫ�ṩ�ú����ṩ��ʱ
//return ms
uint32_t HAL_GetTick(void)
{
    static u32 sTicks = 0;
    static u16 sTimCntLast = 0;
    u16 TimCntCurrent;

//    TimCntCurrent = (__HAL_TIM_GET_COUNTER(&HalTimHandle))&0xFFFF;//0.5ms
    TimCntCurrent = (STM32_HAL_TIMER->CNT)&0xFFFF;
    if( sTimCntLast > TimCntCurrent)
    {
        sTicks += 32768;    //ʱ�ӷֱ���0.5mS��16bit��32768mS
    }
    sTimCntLast = TimCntCurrent;
    return sTicks + (TimCntCurrent>>1);
}

void HAL_SuspendTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Stop(&HalTimHandle);
}
void HAL_ResumeTick(void)
{
    TIM_HandleTypeDef HalTimHandle;
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HAL_TIM_Base_Start(&HalTimHandle);
}
HAL_StatusTypeDef HAL_InitTick (uint32_t TickPriority)
{
    return HAL_OK;
}

__attribute__((weak)) void Hal_Timer_Clk_Enable(void)
{
    //�˴��ĺ�����board.c��ʵ�֣�����ʹ�ܶ�Ӧ��TIM��ʱ��
    //������TIM6Ϊ��
#if(CFG_STM32_HAL_TIMER == 1)
    __HAL_RCC_TIM1_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 2)
    __HAL_RCC_TIM2_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 3)
    __HAL_RCC_TIM3_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 4)
    __HAL_RCC_TIM4_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 5)
    __HAL_RCC_TIM5_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 6)
    __HAL_RCC_TIM6_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 7)
    __HAL_RCC_TIM7_CLK_ENABLE();
#elif(CFG_STM32_HAL_TIMER == 8)
    __HAL_RCC_TIM8_CLK_ENABLE();
#else
#error "error �� û�����ö�ʱ������";
#endif
}
//�˴���TIM6
void HAL_TickInit(void)
{
    u32 uwPrescalerValue;
    TIM_HandleTypeDef HalTimHandle;
    Hal_Timer_Clk_Enable();

    uwPrescalerValue = ((CN_CFG_MCLK/4) / 1000) - 1;    //Counter Clock = 2K
    HalTimHandle.Instance = STM32_HAL_TIMER;
    HalTimHandle.Init.Period        = 0xFFFFFFFF;
    HalTimHandle.Init.Prescaler     = uwPrescalerValue;
    HalTimHandle.Init.ClockDivision = 0;
    HalTimHandle.Init.CounterMode   = TIM_COUNTERMODE_UP;

#if defined(TIM_AUTORELOAD_PRELOAD_ENABLE)
    HalTimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
#endif
    HAL_TIM_Base_DeInit(&HalTimHandle);
    HAL_TIM_Base_Init(&HalTimHandle);
//    MODIFY_REG(STM32_HAL_TIMER->CR1, TIM_CR1_ARPE, TIM_CR1_ARPE);
    HAL_TIM_Base_Start(&HalTimHandle);
}
#endif

