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


#ifndef __CPU_PERI_LCD_H_
#define __CPU_PERI_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include    "stdint.h"
#include    "gkernel.h"
#include    <gui/gk_display.h>

/*===========================================================================*/
struct LCD_ConFig
{
    u16 hsw;            //ˮƽͬ�����
    u16 vsw;            //��ֱͬ�����
    u16 hbp;            //ˮƽ����
    u16 vbp;            //��ֱ����
    u16 hfp;            //ˮƽǰ��
    u16 vfp;            //��ֱǰ��
    u8 * pFrameBuffe;  //��ǰ�㻺����ָ��
    u32  pixsize;         //ÿ��������ռ�ֽ���
    u32 LcdPixelFormat;   //ϵͳ��ɫ��ʽ����
    u32 Dma2dPixelFormat; //DMA2D�����ɫ��ʽ
    u32 LtdcPixelFormat;  //LTDC��ɫ��ʽ
    u32 Dma2dTimeOut;     //DMA2D���䳬ʱʱ��us
    u32 LtdcClk ;         //ʱ�ӵ�λMhz
    u32 LtdcHSPolarity; //ˮƽͬ������
    u32 LtdcVSPolarity; //��ֱͬ������
    u32 LtdcDEPolarity; //����ʹ�ܼ���
    u32 LtdcPCPolarity; //����ʱ�Ӽ���

};
typedef u32 (* LcdConfigFun)(struct LCD_ConFig *lcd);
struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,\
        const char* HeapName);

#ifdef __cplusplus
}
#endif

#endif   //__LCDDRV_H_
