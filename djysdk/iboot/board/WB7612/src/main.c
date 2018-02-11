#include <cpu_peri_gpio.h>
#include <djyos.h>
#include <stdint.h>
#include <stdio.h>
#include <systime.h>
#include "shell.h"
#include "driver.h"
#include "uartctrl.h"
#include "iicbus.h"
#include "djyos.h"
#include "../../../../../djysrc/bsp/arch/arm/arm32_stdint.h"
#include "hmi-input.h"
#include "cpu_peri.h"
#include "timer_hard.h"
#include "stm32f3xx_hal_adc_ex.h"
#include "stm32f3xx_hal_adc.h"
#include "int.h"

////////////////////////////////主函数///////////////////////////////////////
ptu32_t ADC_Sample(void);
static void AT24_test(void);
ptu32_t Uart_Test(void);
void Timer_Test();
u32 Run_IntInit();

ptu32_t djy_main(void)
 {
	ADC_Sample(); //ADC采样
    return 0;
 }

///////////////////////////////////硬件测试///////////////////////////////
void HardDrv_Test(void)
{
	 Timer_Test();
	 Run_IntInit();
	 AT24_test();
	 Uart_Test();
	 printf("uart4 test!!\n\r"); //调试串口
}

///////////////////////////////Run信号 ISR/////////////////////////////
//初始化Run信号对应的中断线,并初始化中断入口函数
u32  Run_Isr();

u32 Run_IntInit()
{
	GPIO_InitTypeDef  GPIO_Initure;

	GPIO_PowerOn(GPIO_C);  //LED配置
	GPIO_CfgPinFunc(GPIO_C,PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,
	                            GPIO_SPEED_H,GPIO_PUPD_NONE);

	GPIO_PowerOn(GPIO_B);  //外部触发源使能
	GPIO_Initure.Pin=GPIO_PIN_4;           //PB4,作为Run信号触发源
	GPIO_Initure.Mode=GPIO_MODE_IT_RISING; //外部中断，上升沿触发
	GPIO_Initure.Pull=GPIO_PULLDOWN;       //默认下拉
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);    //初始化,中断线 0 会自动连接到PB4

    //异步信号,中断
    Int_Register(CN_INT_LINE_EXTI4);            //EXTI Line4 Interrupt,注册中断到中断模块
    Int_SetClearType(CN_INT_LINE_EXTI4,CN_INT_CLEAR_AUTO); //调用前自动清
    Int_IsrConnect(CN_INT_LINE_EXTI4,Run_Isr);  //中断处理函数Run_Isr()
    Int_SettoAsynSignal(CN_INT_LINE_EXTI4);     //中断线设为异步信号
    Int_ClearLine(CN_INT_LINE_EXTI4);           //清中断
    Int_RestoreAsynLine(CN_INT_LINE_EXTI4);     //使能异步信号

    return 0;
}

u32 Run_Isr()
{
	EXTI->PR|=(1<<4); //清中断线
	static u8 flag;
	if(flag)
	{
		GPIO_SettoHigh(GPIO_C,PIN11); //D27亮
		flag=0;
	}
	else
	{
		flag=1;
		GPIO_SettoLow(GPIO_C,PIN11);  //D27灭
	}
	return 0;
}
///////////////////////////////ADC采样//////////////////////////////////////
#if(1)
//WB7612 ADC采样,共9路(包含温度值)
//8路采样+转换时间=8*(7.5clk+12.5clk)=8*20clk=160*0.014us=2.24us
//温度值采样+转换时间=181.5clk+12.5=194clk=194*0.014us=2.7616us
//总采样转换时间t=2.24+2.7616=5.0016us

uint32_t buff1[9];

//存放ADC1采样值,对应I/O口及开入如下:
/*buff1[0]--PA1--KI6
buff1[1]--PA2--KI7
buff1[2]--PA3--KI8
buff1[3]--PC0--KI1
buff1[4]--PC1--KI2
buff1[5]--PC2--KI3
buff1[6]--PC3--KI4
buff1[7]--PF2--KI5
buff1[8]--temperature*/

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);

//ADC采样
ptu32_t ADC_Sample(void)
{
	  HAL_Init();

	  MX_GPIO_Init();
	  MX_DMA_Init();
	  MX_ADC1_Init();

	  HAL_ADCEx_Calibration_Start(&hadc1,2|3|4|6|7|8|9|10|16); //ADC采样校准
	  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&buff1, 9);         //采样开始并DMA传输

	  return 0;
}

//GPIO时钟使能
void MX_GPIO_Init(void)
{
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
}

//DMA使能
void MX_DMA_Init(void)
{
	__DMA1_CLK_ENABLE();
}

//ADC1控制器初始化
void MX_ADC1_Init(void)
{
	  ADC_ChannelConfTypeDef sConfig;
	  //ADC1配置
	  hadc1.Instance = ADC1;
	  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV1;
	  hadc1.Init.Resolution = ADC_RESOLUTION12b;
	  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	  hadc1.Init.ContinuousConvMode = ENABLE;
	  hadc1.Init.DiscontinuousConvMode = DISABLE;
	  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  hadc1.Init.NbrOfConversion = 9;
	  hadc1.Init.DMAContinuousRequests = ENABLE;
	  hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
	  hadc1.Init.LowPowerAutoWait = DISABLE;
	  hadc1.Init.Overrun = OVR_DATA_OVERWRITTEN;

	  HAL_ADC_Init(&hadc1);
	  //采样通道配置
	  sConfig.Channel = ADC_CHANNEL_2;
	  sConfig.Rank = 1;
	  sConfig.SingleDiff = ADC_SINGLE_ENDED;
	  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5 ;
	  sConfig.OffsetNumber = ADC_OFFSET_NONE;
	  sConfig.Offset = 0;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_3;
	  sConfig.Rank = 2;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_4;
	  sConfig.Rank = 3;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_6;
	  sConfig.Rank = 4;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_7;
	  sConfig.Rank = 5;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_8;
	  sConfig.Rank = 6;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_9;
	  sConfig.Rank = 7;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_10;
	  sConfig.Rank = 8;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	  sConfig.Rank = 9;
	  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
	  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

//采样GPIO口及DMA配置
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hadc->Instance==ADC1)
    {
      __ADC12_CLK_ENABLE();
      //PA1 PA2 PA3
      GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
      //PC0 PC1 PC2 PC3
      GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      //PF2
      GPIO_InitStruct.Pin = GPIO_PIN_2;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
      //DMA配置
      hdma_adc1.Instance = DMA1_Channel1;
      hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
      hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
      hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
      hdma_adc1.Init.Mode = DMA_CIRCULAR;
      hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
      HAL_DMA_Init(&hdma_adc1);

      __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
    }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

	  if(hadc->Instance==ADC1)
	  {
		__ADC12_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
		HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2);
		HAL_DMA_DeInit(hadc->DMA_Handle);
	  }
}

#else

//WB7312 ADC采样,共15路(包含一路温度值）
//14路采样+转换时间=14*(7.5clk+12.5clk)=14*20clk=280*0.014us=3.92us
//温度值采样+转换时间=181.5clk+12.5=194clk=194*0.014us=2.7616us
//总采样转换时间t=3.92+2.7616=6.6816us

uint32_t  buff1[10];  //存放ADC1采样值,9路+温度
uint32_t  buff2[5];   //存放ADC2采样值,5路

//存放ADC1采样值,对应I/O口及开入如下：
/*buff1[0]--PA1--KI6
buff1[1]--PA2--KI7
buff1[2]--PA3--KI8
buff1[3]--PC0--KI1
buff1[4]--PC1--KI2
buff1[5]--PC2--KI3
buff1[6]--PC3--KI4
buff1[7]--PF2--KI5
buff1[8]--PF4--KI9
buff1[9]--temperature

buff2[0]--PA4--KI10
buff2[1]--PA5--KI11
buff2[2]--PA6--KI12
buff2[3]--PC7--KI13
buff2[4]--PC4--KI14*/

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_ADC1_Init(void);
void MX_ADC2_Init(void);

void MX_GPIO_Init(void)
{
  __GPIOA_CLK_ENABLE();
  __GPIOC_CLK_ENABLE();
  __GPIOF_CLK_ENABLE();
}

void MX_DMA_Init(void)
{
  __DMA2_CLK_ENABLE();
  __DMA1_CLK_ENABLE();
}

void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION12b;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 10;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = OVR_DATA_OVERWRITTEN;

  HAL_ADC_Init(&hadc1);

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5 ;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 6;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 7;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 8;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 9;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 10;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}

void MX_ADC2_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV1;
  hadc2.Init.Resolution = ADC_RESOLUTION12b;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 5;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = EOC_SINGLE_CONV;
  hadc2.Init.LowPowerAutoWait = DISABLE;
  hadc2.Init.Overrun = OVR_DATA_OVERWRITTEN;

  HAL_ADC_Init(&hadc2);

  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 2;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 3;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 4;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);

  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 5;
  HAL_ADC_ConfigChannel(&hadc2, &sConfig);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(hadc->Instance==ADC1)
    {
      __ADC12_CLK_ENABLE();

      GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4;
      GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

      hdma_adc1.Instance = DMA1_Channel1;
      hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
      hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
      hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
      hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
      hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
      hdma_adc1.Init.Mode = DMA_CIRCULAR;
      hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
      HAL_DMA_Init(&hdma_adc1);

      __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc1);
    }
  if(hadc->Instance==ADC2)
  {
    __ADC12_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    hdma_adc2.Instance = DMA2_Channel1;
    hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_adc2.Init.Mode = DMA_CIRCULAR;
    hdma_adc2.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_adc2);

    __HAL_LINKDMA(hadc,DMA_Handle,hdma_adc2);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {
	__ADC12_CLK_DISABLE();

	HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	HAL_GPIO_DeInit(GPIOF, GPIO_PIN_2|GPIO_PIN_4);
	HAL_DMA_DeInit(hadc->DMA_Handle);
  }
	if(hadc->Instance==ADC2)
  {
    __ADC12_CLK_DISABLE();

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }
}

ptu32_t ADC_Sample(void)
{
  HAL_Init();

  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();

  HAL_ADCEx_Calibration_Start(&hadc1,2|3|4|6|7|8|9|10|5|16);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&buff1, 10);

  HAL_ADCEx_Calibration_Start(&hadc2,1|2|3|4|5);
  HAL_ADC_Start_DMA(&hadc2, (uint32_t*)&buff2, 5);

  return 0;
}

#endif
///////////////////////////////////硬件定时器//////////////////////////////
u32 LED_OnOff();

void Timer_Test()
{
	ptu32_t TimerHandle;
	u32 CrystalFreq;

	GPIO_PowerOn(GPIO_C);  //LED配置
	GPIO_CfgPinFunc(GPIO_C,PIN9,GPIO_MODE_OUT,GPIO_OTYPE_PP,
	                            GPIO_SPEED_H,GPIO_PUPD_NONE);

	TimerHandle = HardTimer_Alloc(LED_OnOff);              //定时器分配,分配的定时器的中断服务函数,中断中调用
    CrystalFreq = HardTimer_GetFreq(TimerHandle);          //获取定时器频率,72MHZ
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETCYCLE,CrystalFreq*1); //设置定时周期,待设置的周期（计数值）,1s
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETRELOAD,true);   //reload模式
    HardTimer_Ctrl(TimerHandle,EN_TIMER_ENINT,true);       //中断使能
    HardTimer_Ctrl(TimerHandle,EN_TIMER_SETINTPRO,true);   //中断属性设置,true代表实时信号
    HardTimer_Ctrl(TimerHandle,EN_TIMER_STARTCOUNT,0);     //使能计数
}

u32 LED_OnOff()
{
	static bool_t flag; //中断服务函数,每隔1s进一次中断服务函数,LED交替亮灭
	if(flag==0)
	{
		flag=1;
		GPIO_SettoLow(GPIO_C,PIN9);  //D28,灭
	}
	else
	{
		GPIO_SettoHigh(GPIO_C,PIN9); //D28,亮
		flag=0;
	}
    return 0;
}

///////////////////////////////////IIC测试//////////////////////////////////
///////////////////////////////////EEPROM测试///////////////////////////////
#define CN_PAGE_NUM        1
#define CN_PAGE_SIZE      64
static u8 eeprom_wbuf[CN_PAGE_SIZE];
static u8 eeprom_rbuf[CN_PAGE_SIZE];
//EEPROM测试
static void AT24_test(void)
{
    u32 ret,w_addr,r_addr,i;
    u8 j;
    //一共256页
    for(i=0;i<CN_PAGE_NUM;i++)
    {
        w_addr=CN_PAGE_SIZE*i;
        r_addr=w_addr;
        for(j=0;j<CN_PAGE_SIZE;j++)
        {
            eeprom_wbuf[j]=j;
        }
        //每次读写不能超过页大小  一页为64bytes
        extern u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
        ret=AT24_WriteBytes(w_addr, eeprom_wbuf, CN_PAGE_SIZE);
        if(!ret)
        {
            printf("EEPROM write error.\r\n");
            return;
        }
        else
        {
            extern u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum);
            ret=AT24_ReadBytes(r_addr,eeprom_rbuf,CN_PAGE_SIZE);
            if(!ret)
            {
                printf("EEPROM read error.\r\n");
                return;
            }
            for(j=0;j<CN_PAGE_SIZE;j++)
            {
                if(eeprom_rbuf[j]!=eeprom_wbuf[j])   //写的每个数与读到的每个数不一致
                {
                    printf("EEPROM test error.\r\n");
                }
            }
        }
    }
    printf("EEPROM test success.\r\n");
}

///////////////////////////////////数据帧格式////////////////////////////////////////
//发送数据帧格式
struct sendframe
{
    u8 tongbutou;
    u8 mubiaoaddr;
    u8 appxieyi;
    u8 yuanaddr;
    u16 baozhangdu;
    u8 jiaoyanma;
    u8 baohupeizhi;
    u16 kairudianya;
    u8 U0;
    u8 U1;
    u8 U2;
    u8 U3;
    u8 U4;
    u8 U5;
    u8 U6;
    u8 U7;
    u8 U8;
    u8 U9;
    u8 U10;
    u8 U11;
    u8 U12;
    u8 U13;
    u8 U14;
    u8 U15;
    u8 U16;
    u8 U17;
    u8 U18;
    u8 U19;
    u8 U20;
    u8 U21;
    u8 U22;
    u8 U23;
    u8 U24;
    u8 U25;
    u8 U26;
    u8 U27;
    u8 U28;
    u8 U29;
    u8 U30;
    u8 U31;
    u16 leijiajiaoyan;
    u8 baowenwei;
}sendframe;
//接收数据帧格式
struct recframe
{
    u8 tongbutou;
    u8 mubiaoaddr;
    u8 appxieyi;
    u8 yuanaddr;
    u16 baozhangdu;
    u8 jiaoyanma;
    u8 baohupeizhi;
    u16 kairudianya;
    u8 U0;
    u8 U1;
    u8 U2;
    u8 U3;
    u8 U4;
    u8 U5;
    u8 U6;
    u8 U7;
    u8 U8;
    u8 U9;
    u8 U10;
    u8 U11;
    u8 U12;
    u8 U13;
    u8 U14;
    u8 U15;
    u8 U16;
    u8 U17;
    u8 U18;
    u8 U19;
    u8 U20;
    u8 U21;
    u8 U22;
    u8 U23;
    u8 U24;
    u8 U25;
    u8 U26;
    u8 U27;
    u8 U28;
    u8 U29;
    u8 U30;
    u8 U31;
    u16 leijiajiaoyan;
    u8 baowenwei;
}recframe;
///////////////////////////////////////Uart_Test////////////////////////////////////
ptu32_t Uart_Test(void)
{
    struct DjyDevice *dev_485a,*dev_485b;
    u32 t;
    u32 ret;
    u32 rlen=0;
    //发送数据帧赋值
    sendframe.tongbutou=0xeb;
    sendframe.mubiaoaddr=0x01;
    sendframe.appxieyi=0xec;
    sendframe.yuanaddr=0x01;
    sendframe.baozhangdu=0x2101;
    sendframe.jiaoyanma=0x80;
    sendframe.baohupeizhi=0xaa;
    sendframe.kairudianya=0x0101;
    sendframe.U0=0x00;
    sendframe.U1=0x01;
    sendframe.U2=0x02;
    sendframe.U3=0x03;
    sendframe.U4=0x04;
    sendframe.U5=0x05;
    sendframe.U6=0x06;
    sendframe.U7=0x07;
    sendframe.U8=0x08;
    sendframe.U9=0x09;
    sendframe.U10=0x0a;
    sendframe.U11=0x0b;
    sendframe.U12=0x0c;
    sendframe.U13=0x0d;
    sendframe.U14=0x0e;
    sendframe.U15=0x0f;
    sendframe.U16=0x10;
    sendframe.U17=0x11;
    sendframe.U18=0x12;
    sendframe.U19=0x13;
    sendframe.U20=0x14;
    sendframe.U21=0x15;
    sendframe.U22=0x16;
    sendframe.U23=0x17;
    sendframe.U24=0x18;
    sendframe.U25=0x19;
    sendframe.U26=0x1a;
    sendframe.U27=0x1b;
    sendframe.U28=0x1c;
    sendframe.U29=0x1d;
    sendframe.U30=0x1e;
    sendframe.U31=0x1f;
    sendframe.leijiajiaoyan=0x2120;
    sendframe.baowenwei=0xc3;

    void Led_init(void);
    Led_init();
    GPIO_SettoLow(GPIO_C,PIN9);
    GPIO_SettoLow(GPIO_C,PIN11);

//    //测试uart1,9M自环传输时,只需打开UART1即可
//    dev_485a=Driver_OpenDevice("UART1",D_RDWR,CN_TIMEOUT_FOREVER);   //打开设备
//    ret=Driver_CtrlDevice(dev_485a,CN_UART_SET_BAUD,9000000,0);      //设置波特率
//    ret=Driver_CtrlDevice(dev_485a, CN_UART_DMA_USED,0,0);           //DMA传输
//    ret=Driver_CtrlDevice(dev_485a,CN_UART_START,0,0);               //启动串口
//
//    do{
//        rlen=Driver_ReadDevice(dev_485a,(u8 *)&recframe,sizeof(struct sendframe),0,10000); //485a读数据
//      }while(rlen);     //如果读到reln为0则结束循环,不为0 继续传输
//
//    ret=Driver_WriteDevice(dev_485a,(u8 *)&sendframe,sizeof(struct recframe),  //485a写数据
//                    0,CN_BLOCK_COMPLETE,CN_TIMEOUT_FOREVER);
//    rlen=Driver_ReadDevice(dev_485a,(u8 *)&recframe,sizeof(struct sendframe),0,10000); //485a读数据

    dev_485a=Driver_OpenDevice("UART1",D_RDWR,CN_TIMEOUT_FOREVER);   //打开设备
    ret=Driver_CtrlDevice(dev_485a,CN_UART_SET_BAUD,9000000,0);      //设置波特率
    ret=Driver_CtrlDevice(dev_485a, CN_UART_DMA_USED,0,0);           //DMA传输
    ret=Driver_CtrlDevice(dev_485a,CN_UART_START,0,0);               //启动串口
    ret=Driver_CtrlDevice(dev_485a,CN_UART_HALF_DUPLEX_SEND,0,0);    //485a发送数据

    dev_485b=Driver_OpenDevice("UART2",D_RDWR,CN_TIMEOUT_FOREVER);
    ret=Driver_CtrlDevice(dev_485b,CN_UART_SET_BAUD,9000000,0);
    ret=Driver_CtrlDevice(dev_485b, CN_UART_DMA_USED,0,0);
    ret=Driver_CtrlDevice(dev_485b,CN_UART_START,0,0);
    ret=Driver_CtrlDevice(dev_485b,CN_UART_HALF_DUPLEX_RECV,0,0);    //485b接收数据
//    extern s64 __DjyGetSysTime(void);
    while(1)
    {
        ret=Driver_WriteDevice(dev_485a,(u8 *)&sendframe,sizeof(struct recframe),  //485a写数据
                        0,CN_BLOCK_COMPLETE,CN_TIMEOUT_FOREVER);

        rlen=Driver_ReadDevice(dev_485b,(u8 *)&recframe,sizeof(struct sendframe),0,10000); //485b读数据
        GPIO_SettoHigh(GPIO_C,PIN11); //D27,运行
        Sh_ShowEvent(); //time1处打断点,可测CPU占用率
    }
    return 0;
}
///////////////////////////////////////LED初始化//////////////////////////
 void Led_init(void)
{
    GPIO_PowerOn(GPIO_C);
    GPIO_CfgPinFunc(GPIO_C,PIN9|PIN11,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                            GPIO_SPEED_H,GPIO_PUPD_NONE);
}
 ///////////////////////////////////////KO测试//////////////////////////
void KO_test()
{
	GPIO_PowerOn(GPIO_C);
	GPIO_PowerOn(GPIO_E);

	GPIO_CfgPinFunc(GPIO_E,PIN2|PIN4|PIN6|PIN11|PIN13,GPIO_MODE_OUT,GPIO_OTYPE_PP,
	                            GPIO_SPEED_H,GPIO_PUPD_NONE);
	while(1)
	{
		GPIO_SettoHigh(GPIO_E,PIN2);
	    Djy_EventDelay(1000);
	    GPIO_SettoHigh(GPIO_E,PIN4);
	    Djy_EventDelay(1000);
	  	GPIO_SettoHigh(GPIO_E,PIN6);
		Djy_EventDelay(1000);
		GPIO_SettoHigh(GPIO_E,PIN11);
		Djy_EventDelay(1000);
		GPIO_SettoHigh(GPIO_E,PIN13);
	}
}

