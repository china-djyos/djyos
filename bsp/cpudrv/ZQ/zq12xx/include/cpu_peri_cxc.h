

//����ж��Լ��˼�ͨ��ģ��

#ifndef CPU_PERI_CXC_H
#define CPU_PERI_CXC_H

#ifdef __cplusplus
extern "C"
#endif

#include "cpu_peri_isr.h"
//M0 ����������Դ

enum EN_M0RevSource
{
    EN_M0_REV_FROM_RISC = 0x00, //���ݷ��ͷ�ΪRISC
    EN_M0_REV_FROM_DSP,         //���ݷ��ͷ�ΪDSP
    EN_RISC_REV_FROM_M0,        //Risc����M0  ����
    EN_RISC_REV_FROM_DSP,       //Risc����DSP ����
};

//Soft Int Start

//��������ж���

extern u8 SoftIntIrqRequst(void);

//�ͷ�����ж���

extern bool_t SoftIntIrqFree(u8 irqID);

//��������ж�

extern void SoftIntStart(u8 irqID);

//ע������ж�Usr������

extern bool_t RegisterSoftInt(u8 irqID,djybsp_isr_hdl_t cxc_hdl,u32 para);


/*****************************************************
 * M0 SoftInt Usage Example:
 * M0��
 * 	  softIrq = SoftIntIrqRequst();
 * 	  RegisterSoftInt(softIrq,UsrIsr);
 * 	  where need to softInt call :
 *	  SoftIntStart(softIrq);
 *
 *****************************************************/

//Soft Int End


//����Ϊ�˼�ͨ��

//M0�˽������ݳ�ʼ��
//fromObj = EN_M0_REV_FROM_RISC M0 �� Risc���� ��ʼ��
//fromObj = EN_M0_REV_FROM_DSP  M0 �� Dsp ���� ��ʼ��

extern void M0ToRevMessageInit(u8 fromObj);

//M0 �� Risc �˽��� ���� - ��û ���յ�������߳� ����

extern u8 M0ToRevRiscData(u32 *buf,u8 len);

//Risc �� M0���� ����

extern void RiscSendToM0(u32 *buf,u8 len);

//M0 ���� Dsp ����

extern u8 M0ToRevDspData(u32 *buf,u8 len);

/*****************************************************
 * M0 Rev Usage Example:
 * Risc Send To M0��
 * Risc:
 *	    RiscSendToM0(buf,len);
 *
 * M0 :
 *	    M0ToRevMessageInit(EN_M0_REV_FROM_RISC);
 *	    M0ToRevRiscData(buf,len);
 *
 *****************************************************/


//Risc ���� ��ʼ��
//fromObj = EN_RISC_REV_FROM_M0  ���� M0  ���ݳ�ʼ��
//fromObj = EN_RISC_REV_FROM_DSP ����Dsp  ���ݳ�ʼ��

extern void RiscToRevMessageInit(u8 fromObj);

//M0 ��Risc ��������

extern u8 M0SendToRisc(u32 *buf,u8 len);

//Risc ����M0 ����

extern u8 RiscToRevM0Data(u32 *buf,u8 len);

//Risc ����Dsp ����

extern u8 RiscToRevDspData(u32 *buf,u8 len);

/*****************************************************
 * Risc Rev Usage Example:
 * M0 Send To Risc��
 * M0 :
 * 		M0SendToRisc(buf,len);
 * Risc:
 *      RiscToRevMessageInit(EN_RISC_REV_FROM_M0);
 *		RiscToRevM0Data(buf,len);
 *
 *****************************************************/

#ifdef __cplusplus
}
#endif

#endif




















































