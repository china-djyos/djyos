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
#if 0
struct IntReg
{
    vu32 SRCPND   ;    //0x4a000000 ���жϲ�������1
    vu32 INTMOD   ;    //0x4a000004 Interrupt mode control
    vu32 INTMSK   ;    //0x4a000008 Interrupt mask control
    vu32 PRIORITY ;    //0x4a00000c IRQ priority control
    vu32 INTPND   ;    //0x4a000010 SRCPND=1��λ������������1
    vu32 INTOFFSET;    //0x4a000014 Interruot request source offset
    vu32 SUBSRCPND;    //0x4a000018 Sub source pending
    vu32 INTSUBMSK;    //0x4a00001c Interrupt sub mask
};
#endif


struct INTC_REG{

    vu32 SRCPND1;                   // interrupt request status reg
    vu32 INTMOD1;                   // interrupt mode reg
    vu32 INTMSK1;                   // interrupt mask reg
    vu32 PAD0;

    vu32 INTPND1;                   // interrupt pending reg
    vu32 INTOFFSET1;                // interrupt offset reg
    vu32 SUBSRCPND;                 // SUB source pending reg
    vu32 INTSUBMSK;                 // interrupt SUB mask reg


    vu32 PAD1[4];

    vu32 PRIORITY_MODE1;            // priority reg
    vu32 PRIORITY_UPDATE1;          // priority reg
    vu32 PAD2[2];


    vu32 SRCPND2;           // interrupt request status reg
    vu32 INTMOD2;           // interrupt mode reg
    vu32 INTMSK2;           // interrupt mask re
    vu32 PAD3;


    vu32 INTPND2;           // interrupt pending reg
    vu32 INTOFFSET2;        // interrupt offset reg
    vu32 PAD4[2];

    vu32 PAD5[4];


    vu32 PRIORITY_MODE2;    // priority reg
    vu32 PRIORITY_UPDATE2;  // priority reg
    vu32 PAD6[2];


};

#define INTC_REG_BASE   ((struct INTC_REG*)0x4A000000)


#define cn_noirq    0x80
#define cn_nofiq    0x40
#define cn_noint    0xc0

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
