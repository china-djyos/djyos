//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��: BSPģ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ������������ļ���
//        CPU�ͺ�:STM32F103ZE
//        ����ͺ�:QH_1
//        ������ҵ:����������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-10-31
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cpu-optional.h"

#define CN_CFG_TICK_US 10000  //tick�������usΪ��λ��
#define CN_CFG_TICK_HZ 100    //�ں�ʱ��Ƶ�ʣ���λΪhz��

//FslKxxϵ���в���֧��MPU��FPU���ʲ���ͳһ��cpu-optional.h������
#define CN_CPU_OPTIONAL_MPU         1       //�Ƿ�֧��mpu
#define CN_CPU_OPTIONAL_FPU         1       //�Ƿ�֧��fpu

#define Mhz             1000000
#define CN_CFG_MCLK     (24*Mhz)           //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_FCLK     CN_CFG_MCLK
#define CN_CFG_AHBCLK   CN_CFG_MCLK         //����ʱ��
#define CN_CFG_PCLK2    (CN_CFG_MCLK/2)     //��������ʱ��
#define CN_CFG_PCLK1    (CN_CFG_MCLK/4)     //��������ʱ��
#define CN_CFG_EXTCLK   (10*Mhz)             //�ⲿʱ��=8M

#define CN_CFG_FINE_US 0x00000AAA  //1/24M,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ�����

//�ں��������
#define CN_CFG_DYNAMIC_MEM 1  //�Ƿ�֧�ֶ�̬�ڴ���䣬��ʹ��֧�֣�Ҳ����ʹ��malloc-free�����ڴ棬
                              //��ʹ���в������������û��ֲᡷ�ڴ�����½�

//CPU�ܹ��������,��ѡ���߿��ܿ�ѡ�Ĳ�����������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

#ifdef __cplusplus
}
#endif
#endif // __CPU_OPTIONAL_H__


