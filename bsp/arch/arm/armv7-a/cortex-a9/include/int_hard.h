#ifndef __INT_HARD_H__
#define __INT_HARD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

typedef ucpu_t atom_high_t;
typedef ucpu_t atom_low_t;

/* ע����Ҫ�뵱Ȼ�ذѺ����ͺ��������ϵ��������ʵ������Ϊ׼��*/

//���������ж�ģ��ĳ�����������Ӳ���йأ�Ҳ����������йء���ʵ���첽�źź�
//ʵʱ�жϾ�ʹ��irq��irq��Ϊ���жϿ��أ����첽�ź���û�ж�����ʹ�ܿ��أ���ֹ�첽
//�ź���ͨ����ֹ���б�����Ϊ�첽�źŵ��жϺ������ʵ�ֵģ���û�ж������첽�ź�
//ʹ�ܿ��ء������ֲ�߾�����fiqʵ��ʵʱ�жϣ���irqʵ���첽�źţ�������ж�����
//�첽�ź�ʹ�ܿ��ء�
#define CN_CFG_ASYN_TURNKEY    0   //�Ƿ�֧�ֶ������첽�ź�ʹ�ܿ���
#define CN_CFG_TRUNK_TURNKEY   1   //�Ƿ�֧�ֶ��������ж�ʹ�ܿ���

#define CN_CFG_REG_ATOM   1   //�����첽�źſ��ؼĴ�����ԭ���ԣ���������д
                                //�첽�źſ���ʱ���Ƿ����һ��ָ������ꡣ
                                //����:arm9��s3c2410,INTMSK��һ��32λ�Ĵ�����
                                //      ��32λcpu�ϣ�һ��ָ���ִ�ж���д��
                                //      ����Ϊ����ԭ���ԣ���������Ϊ1
                                //     arm9��s3c2416��INTMSK��64λ��������
                                //      32λ�Ĵ�������������ָ����ܶ���д��
                                //      �ʲ���ԭ���ԣ���������Ϊ0
                                //     ��cortex-m3�ϣ�ֱ�Ӷ�дBASEPRI�Ĵ�����
                                //      Ҳ��һ��ָ������꣬�߱�ԭ���ԣ���Ϊ1

void __Int_ClearAllLine(void);
void __Int_InitHard(void);
void __Int_EngineReal(ufast_t ufl_line);
void __Int_EngineAsynSignal(ufast_t ufl_line);
void __Int_EngineAll(ufast_t ufl_line);

void int_enable_arm_int(void);
void int_disable_arm_int(void);
void __int_enable_irq(void);
void __int_disable_irq(void);
void __int_enable_fiq(void);
void __int_disable_fiq(void);

#ifdef __cplusplus
}
#endif

#endif //__INT_HARD_H__
