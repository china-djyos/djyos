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
//����ģ��:IO��ʼ��
//���ߣ�Liuwei
//�汾��V1.0.0
//�ļ�����:IO��ʼ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-04-22
//   ����: Liuwei
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GPIO_H__
#define __GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

struct GPIO_REG{

    vu32 GPACON;                  // Port A - offset 0
    vu32 GPADAT;                  // Data

    vu32 PAD1[2];

    vu32 GPBCON;                  // Port B - offset 0x10
    vu32 GPBDAT;                  // Data
    vu32 GPBUDP;                   // Pull-up disable
    vu32 PAD2;

    vu32 GPCCON;                  // Port C - offset 0x20
    vu32 GPCDAT;                  // Data
    vu32 GPCUDP;                   // Pull-up disable
    vu32 PAD3;

    vu32 GPDCON;                  // Port D - offset 0x30
    vu32 GPDDAT;                  // Data
    vu32 GPDUDP;                   // Pull-up disable
    vu32 PAD4;

    vu32 GPECON;                  // Port E - offset 0x40
    vu32 GPEDAT;                  // Data
    vu32 GPEUDP;                   // Pull-up disable
    vu32 PAD5;

    vu32 GPFCON;                  // Port F - offset 0x50
    vu32 GPFDAT;
    vu32 GPFUDP;
    vu32 PAD6;

    vu32 GPGCON;                  // Port G - offset 0x60
    vu32 GPGDAT;
    vu32 GPGUDP;
    vu32 PAD7;

    vu32 GPHCON;                  // Port H - offset 0x70
    vu32 GPHDAT;
    vu32 GPHUDP;
    vu32 PAD8;

    vu32 MISCCR;                  // misc control reg - offset 0x80
    vu32 DCLKCON;                 // DCLK0/1 control reg

    vu32 EXTINT0;                 // external interrupt control reg 0
    vu32 EXTINT1;                 // external interrupt control reg 1
    vu32 EXTINT2;                 // external interrupt control reg 2

    vu32 EINTFLT0;                // reserved
    vu32 EINTFLT1;                // reserved
    vu32 EINTFLT2;                // external interrupt filter reg 2
    vu32 EINTFLT3;                // external interrupt filter reg 3

    vu32 EINTMASK;                // external interrupt mask reg
    vu32 EINTPEND;                // external interrupt pending reg

    vu32 GSTATUS0;                // external pin status
    vu32 GSTATUS1;                // chip ID
    vu32 GSTATUS2;                // reset status
    vu32 GSTATUS3;                // inform register
    vu32 GSTATUS4;                // inform register

    vu32 DSC0;                  // C0 - added by simon
    vu32 DSC1;
    vu32 DSC2;
    vu32 MSLCON;

    vu32 GPJCON;                    // D0
    vu32 GPJDAT;
    vu32 GPJUDP;
    vu32 PDA9;

    vu32 GPKCON;                    // E0
    vu32 GPKDAT;

    vu32 GPKUDP;

    vu32 PDA10;

    vu32 GPLCON;                    // F0
    vu32 GPLDAT;
    vu32 GPLUDP;
    vu32 PDA11;

    vu32 GPMCON;                    // 100
    vu32 GPMDAT;
    vu32 GPMUDP;
    vu32 PDA12;
};

#define GPIO_REG_BASE   ((struct GPIO_REG*)0x56000000)

#ifdef __cplusplus
}
#endif

#endif /*__GPIO_H__*/
