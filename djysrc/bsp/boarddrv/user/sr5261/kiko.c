// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：KiKo.c
// 模块描述: 开入开出驱动
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 10/05.2016
// =============================================================================

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "os.h"
#include "cpu_peri.h"
#include "kiko.h"
#include "tca9535.h"

// BIT位定义
#define DB0                 (0x01)
#define DB1                 (0x02)
#define DB2                 (0x04)
#define DB3                 (0x08)
#define DB4                 (0x10)
#define DB5                 (0x20)
#define DB6                 (0x40)
#define DB7                 (0x80)
#define DB8                 (0x100)
#define DB9                 (0x200)
#define DB10                (0x400)
#define DB11                (0x800)
#define DB12                (0x1000)
#define DB13                (0x2000)
#define DB14                (0x4000)
#define DB15                (0x8000)
#define DB16                (0x10000)
#define DB17                (0x20000)
#define DB18                (0x40000)
#define DB19                (0x80000)
#define DB20                (0x100000)
#define DB21                (0x200000)
#define DB22                (0x400000)
#define DB23                (0x800000)
#define DB24                (0x1000000)
#define DB25                (0x2000000)
#define DB26                (0x4000000)
#define DB27                (0x8000000)
#define DB28                (0x10000000)
#define DB29                (0x20000000)
#define DB30                (0x40000000)
#define DB31                (0x80000000)

static const uint32_t Bits[]={
		DB0,
		DB1,
		DB2,
		DB3,
		DB4,
		DB5,
		DB6,
		DB7,
		DB8,
		DB9,
		DB10,
		DB11,
		DB12,
		DB13,
		DB14,
		DB15,
		DB16,
		DB17,
		DB18,
		DB19,
		DB20,
		DB21,
		DB22,
		DB23,
		DB24,
		DB25,
		DB26,
		DB27,
		DB28,
		DB29,
		DB30,
		DB31
};

#define KO1    {GPIO_H, PIN11, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO2    {GPIO_G, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO3    {GPIO_G, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO4    {GPIO_J, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO5    {GPIO_G, PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO6    {GPIO_I, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO7    {GPIO_I, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO8    {GPIO_J, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO9    {GPIO_K, PIN1, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO10    {GPIO_K, PIN4, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO11    {GPIO_C, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define KO12    {GPIO_H, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}

#define RUN_DBG_LED   {GPIO_I, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}
#define RUN_LED       {GPIO_I, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}


const uint8_t KI_Pins[] = {
		port00,
		port01,
		port02,
		port03,
		port04,
		port05,
		port06,
		port07,
		port10,
};

 const Pin KO_Pins[] = {
		KO1,
		KO2,
		KO3,
		KO4,
		KO5,
		KO6,
		KO7,
		KO8,
		KO9,
		KO10,
		KO11,
		KO12,
};

const uint8_t LED_KO_Pins[]={
		port11,
		port12,
		port13,
		port14,
		port15,
		port16,
};

const Pin CPU_CTRL_LED_Pins[]={
		RUN_LED,
		RUN_DBG_LED,
};

#define MAX_KO_NUM       12     //最大开出量
#define MAX_KI_NUM       9      //最大开入量
#define LED_KO_NUM       6


// =========================================================================
// 函数功能：开出引脚初始化
// 输入参数：无
// 输出参数：无
// 返回值  ：无
// =========================================================================
void KO_Init(void)
{
	PIO_Configure(KO_Pins,  PIO_LISTSIZE(KO_Pins));
	PIO_Configure(CPU_CTRL_LED_Pins, PIO_LISTSIZE(CPU_CTRL_LED_Pins));
    PIO_Set(&KO_Pins[0]);  //电源不导通   todo  
}

// =========================================================================
// 函数功能：写开出值，开出值从低到高在一个32位的数组里面
// 输入参数：无
// 输出参数：无
// 返回值  ：无
// =========================================================================
void KO_Write(uint32_t dwKoValue)
{
	uint8_t i;
	for(i=0;i<MAX_KO_NUM;i++)
	{
       if(dwKoValue&Bits[i])
       {
    	   PIO_Set(&KO_Pins[i]);
       }
       else
       {
    	   PIO_Clear(&KO_Pins[i]);
       }
	}
}

// =========================================================================
// 函数功能：CPU板led控制
// 输入参数：打开 关闭
// 输出参数：无
// 返回值  ：无
// =========================================================================
void Led_KO_Ctrl(uint8_t idx,uint8_t flag)
{
	TCA9535_PortWrite(10+idx,flag);
}

// =========================================================================
// 函数功能：初始化IO引脚
// 输入参数：
// 输出参数：
// 返回值  ：
// =========================================================================
void KI_Init(void)
{
    PIO_Configure (KI_Pins, PIO_LISTSIZE(KI_Pins));   //总线和片选初始化
}

// =============================================================================
// 功能：读对应槽号的开往值
// 参数：SlotNo,槽号
// 返回：读到的开入值，放入32bit的低9位
// =============================================================================
void KI_Read(uint32_t *buf)
{
	uint8_t i;
	uint32_t temp=0;
	uint32_t Buf=0;

	for(i=0;i<MAX_KI_NUM;i++)
	{
		temp=0;
		temp=TCA9535_PortRead(KI_Pins[i]);
		temp=temp<<i;
		Buf|=temp;
	}
	Buf=0xFFFFFFFF-Buf;
	*buf=Buf;
}

// =============================================================================
// 功能：RUN LED控制，点亮或熄灭
// 参数：flag,1点亮；0,熄灭.
// 返回：无.
// =============================================================================
void CPU_Led_Ctrl(uint8_t idx, uint8_t flag)
{
	if( flag )
	{
		PIO_Set(&CPU_CTRL_LED_Pins[idx]);
	}
	else
	{
		PIO_Clear(&CPU_CTRL_LED_Pins[idx]);
	}
}

// =========================================================================
// 函数功能：开出使能。
// 输入参数：打开 关闭
// 输出参数：无
// 返回值  ：无
// =========================================================================
void KO_StartRly(uint8_t flag)
{
//	if( flag )
//	{
//		PIO_Set(&EN_KO_Pin);
//	}
//	else
//	{
//		PIO_Clear(&EN_KO_Pin);
//	}
}


void ExKiKo_Init(void)
{
	TCA9535_Init(TAC9535_IIC_BUS_NAME);
	TCA9535_PortConfigure(port00,enum_in_type);
	TCA9535_PortConfigure(port01,enum_in_type);
	TCA9535_PortConfigure(port02,enum_in_type);
	TCA9535_PortConfigure(port03,enum_in_type);
	TCA9535_PortConfigure(port04,enum_in_type);
	TCA9535_PortConfigure(port05,enum_in_type);
	TCA9535_PortConfigure(port06,enum_in_type);
	TCA9535_PortConfigure(port07,enum_in_type);
	TCA9535_PortConfigure(port10,enum_in_type);

	TCA9535_PortConfigure(port11,enum_out_type);
	TCA9535_PortConfigure(port12,enum_out_type);
	TCA9535_PortConfigure(port13,enum_out_type);
	TCA9535_PortConfigure(port14,enum_out_type);
	TCA9535_PortConfigure(port15,enum_out_type);
	TCA9535_PortConfigure(port16,enum_out_type);
}
