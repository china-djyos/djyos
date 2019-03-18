#ifndef _SCM6XX_TIMER_H_
#define _SCM6XX_TIMER_H_

#include "SCM6XX_ALL.h"

#define TC0    0
#define TC1    1
#define TC2    2
#define TC3    3


//������16λ��ʱģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT16 matchVal;
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_16BitTimerCFG;

//������16λ����ģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //�����ء��½��ء�����/�½���   
	UINT16 matchVal;
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_16BitCNTCFG;

//������32λ����ģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //�����ء��½��ء�����/�½���   
	UINT16 matchValL;  //��16λƥ��ֵ
	UINT16 matchValH;  //��16λƥ��ֵ
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_32BitCNTCFG;

//�����ڲ�׽ģʽʱ������
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




