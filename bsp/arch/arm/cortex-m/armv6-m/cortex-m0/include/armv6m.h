//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:  ����
//���ߣ�     lst
//�汾��      V1.0.0
//��ʼ�汾������ڣ�2009-08-05
//�ļ�����:  cortex-m0�Ĵ�����ͷ�ļ�
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-05
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#ifndef __ARMV6M_H__
#define __ARMV6M_H__

#ifdef __cplusplus
extern "C" {
#endif

extern struct ScbReg volatile * const pg_scb_reg;
extern struct SystickReg volatile * const pg_systick_reg;

//����scb_reg.ICSR �жϿ��Ƽ�״̬�Ĵ�����0xE000_ED04 ,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_scb_icsr_nmipendset  (1<<31)     //rw д1����nmi,��1=nmi����
#define bm_scb_icsr_pendsvset   (1<<28)     //rw д1����pendsv�������ع���״̬
#define bm_scb_icsr_pendsvclr   (1<<27)     //w д1���pendsv����״̬
#define bm_scb_icsr_pendstset   (1<<26)     //rw д1����systick�������ع���״̬
#define bm_scb_icsr_pendstclr   (1<<25)     //w д1���systick����״̬
#define bm_scb_icsr_isrpending  (1<<22)     //r ���жϹ��𣨲���nmi��
#define bm_scb_icsr_vectpending (0x3f<<12)  //r ������ж������ȼ���ߵ��жϺ�
#define bm_scb_icsr_vectactive  (0x3f<<0)   //r ���ڷ�����쳣��
#define bo_scb_icsr_nmipendset  31
#define bo_scb_icsr_pendsvset   28
#define bo_scb_icsr_pendsvclr   27
#define bo_scb_icsr_pendstset   26
#define bo_scb_icsr_pendstclr   25
#define bo_scb_icsr_isrpending  22
#define bo_scb_icsr_vectpending 12
#define bo_scb_icsr_vectactive  0

//AIRCR Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c ,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_scb_aircr_vectkey        (0xffff<<16)
#define bm_scb_aircr_endianess      (1<<15)
#define bm_scb_aircr_sysresetreq    (1<<2)
#define bm_scb_aircr_vectclractive  (1<<1)
#define bo_scb_aircr_vectkey        16  //rw д���Ĵ���������ͬʱ�Դ���д0x05fa
#define bo_scb_aircr_endianess      15  //r 1=��ˣ�0=С��
#define bo_scb_aircr_sysresetreq    2   //w д1�߼���λоƬ
#define bo_scb_aircr_vectclractive  1   //w �������ڵ���

//����scb_reg.SCR ϵͳ���ƼĴ���,0xE000_ED10 ,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_scb_scr_sevonpend    (1<<4)
#define bm_scb_scr_sleepdeep    (1<<2)
#define bm_scb_scr_sleeponexit  (1<<1)
#define bo_scb_scr_sevonpend    4      //rw 1=�κ��жϺ��¼������Ի��Ѵ�����
                                       //   0=ֻ��ʹ�ܵ��жϺ��¼����Ի��Ѵ�����
#define bo_scb_scr_sleepdeep    2      //rw 0=��ͨ˯�ߣ�1=���˯��
#define bo_scb_scr_sleeponexit  1      //rw 1=���жϷ���ʱ����sleep

//����scb_reg.CCR ��������ƼĴ���,0xE000_ED14 ,bm_Ϊλ���룬bo_Ϊλƫ��
#define bm_scb_ccr_stkalign         (1<<9)
#define bm_scb_ccr_unaligntrp       (1<<3)
#define bo_scb_ccr_stkalign         9   //rw �쳣��Ӧ�Զ���ջʱǿ��dp˫�ֶ���
#define bo_scb_ccr_unaligntrp       3   //rw 1=δ������ʽ������÷�fault

//�ر�ע��:m0��scb�Ĵ�����m3��ͬ��m3�����ֽڷ��ʣ���m0�����ֽڷ���
struct ScbReg
{
    vuc32 CPUID;
    vu32 ICSR;          //�жϿ��Ƽ�״̬�Ĵ�����0xE000_ED04
    vu32 res0;
    vu32 AIRCR;         //Ӧ�ó����жϼ���λ���ƼĴ�����0xE000_ED0c
    vu32 SCR;           //ϵͳ���ƼĴ���,0xE000_ED10
    vu32 CCR;           //��������ƼĴ���,0xE000_ED14
    vu32 res1;
    vu32 pri8_11;       //�쳣�����ȼ�,0xE000_ED1c,pri11=svc
    vu32 pri12_15;      //�쳣�����ȼ�,0xE000_ED20��pri14=pendsv,pri15=systick
    vu32 SHCSR;         //datasheet���ޣ���nxp�Ĵ�������?
    vu32 res4[2];
    vu32 DFSR;          //���� fault ״̬�Ĵ���(DFSR) 0xE000_ED30
                        //datasheet���ޣ���nxp�Ĵ�������?
};

#ifdef __cplusplus
}
#endif

#endif /*__ARMV6M_H__*/


