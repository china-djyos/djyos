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
//����ģ��: BSPģ��
//����:  Liuwei
//�汾��V1.0.0
//�ļ�����: ������������ļ���
//        CPU�ͺ�:S3C2416

//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2015-05-11
//   ����:  Liuwei
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define DJY_BOARD        "TQ2440"

/*____��Ӳ����ص�����____*/
#define Mhz 1000000
#define CN_CFG_MCLK (532*Mhz)  //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_HCLK (CN_CFG_MCLK/4)  //��������ʱ��
#define CN_CFG_PCLK (CN_CFG_MCLK/8)  //��������ʱ��
#define CN_CFG_TIMER_CLK CN_CFG_PCLK  //��ʱ��ʱ��Դ

/*____���¶���tick����____*/
#define CN_CFG_TICK_US 1000  //tick������Ժ���Ϊ��λ��
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define CN_CFG_FINE_US 0x00000a3d  //40nS,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ (25*Mhz)  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ�����

//CPU�ܹ��������,��ѡ������������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch_feature.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

//LCD��������
#define CN_LCD_XSIZE        320     //LCD�������ؿ��.
#define CN_LCD_YSIZE        240     //LCD�������ظ߶�.
#define CN_LCD_PIXEL_FORMAT CN_SYS_PF_RGB565    //LCD��ɫ��ʽ.

#undef  CN_LCD_DRV_FRAME_BUFFER_EN  //����ú���ʹ��FrameBuffer,����ֱ����Ļ���.
#undef  CN_LCD_DRV_REMOTE_EN        //����ú���LCD����ʹ��Զ����ʾ����

#ifdef __cplusplus
}
#endif
#endif // __BOARD_CONFIG_H__



