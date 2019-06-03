#ifndef _SCM6XX_TIMER_H_
#define _SCM6XX_TIMER_H_

#include "SCM6XX_ALL.h"

#define TC0    0
#define TC1    1
#define TC2    2
#define TC3    3


//工作于16位定时模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT16 matchVal;
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_16BitTimerCFG;

//工作于16位计数模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //上升沿、下降沿、上升/下降沿   
	UINT16 matchVal;
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_16BitCNTCFG;

//工作于32位计数模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //上升沿、下降沿、上升/下降沿   
	UINT16 matchValL;  //低16位匹配值
	UINT16 matchValH;  //高16位匹配值
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_32BitCNTCFG;

//工作于捕捉模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT16 capSrc;    
	UINT16 capEdge;
}TC_CAPTCFG;


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void TC_Reset_Reg(SGCC_TC_TypeDef *TCx);
void TC_Enable_Int(SGCC_TC_TypeDef *TCx, FunctionalState opt);
void TC_Enable_Int_Src(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src, FunctionalState opt);
void TC_Clear_Int_Flag(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src);
UINT8 TC_Get_Int_Status(SGCC_TC_TypeDef *TCx, UINT8 Timer_Int_Src);
void TC_Timer_Set_16Bit(SGCC_TC_TypeDef *TCx, FunctionalState Match_En, TC_16BitTimerCFG TC_Timer_Cfg, FunctionalState Int_En);
void TC_Timer_Set_32Bit(SGCC_TC_TypeDef *TCx, UINT16 Prescale, FunctionalState Match_En, UINT16 Match_Val_L, UINT16 Match_Val_H);
void TC_Counter_Set_16Bit(SGCC_TC_TypeDef *TCx,FunctionalState Match_En, TC_16BitCNTCFG TC_Counter_Cfg, FunctionalState Int_En);
void TC_Counter_Set_32Bit(SGCC_TC_TypeDef *TCx,TC_32BitCNTCFG TC_Counter_Cfg);
void TC_Capture_Set_16Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG TC_Capture_Cfg,FunctionalState Int_En);
void TC_Capture_Set_32Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG TC_Capture_Cfg);

#endif 




