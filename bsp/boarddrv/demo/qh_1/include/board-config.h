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

#define DJY_BOARD   "QH_1"

//��ʱ��������ص�����,��������
#define Mhz 1000000
#define CN_CFG_MCLK (72*Mhz)  //��Ƶ���ں�Ҫ�ã����붨��
#define CN_CFG_FCLK CN_CFG_MCLK  //cm3������������ʱ��
#define CN_CFG_AHBCLK CN_CFG_MCLK  //����ʱ��
#define CN_CFG_PCLK2 CN_CFG_MCLK  //��������ʱ��
#define CN_CFG_PCLK1 (CN_CFG_MCLK/2)  //��������ʱ��
#define CN_CFG_EXTCLK 8000000  //�ⲿʱ��=8M

/*____���¶���tick����____*/
#define CN_CFG_TICK_US 1000  //tick�������usΪ��λ��
#define CN_CFG_TICK_HZ 1000  //�ں�ʱ��Ƶ�ʣ���λΪhz��
#define CN_USE_TICKLESS_MODE    (0U)
#if (!CN_USE_TICKLESS_MODE)
#define CN_CFG_FINE_US 0x0000038e  //1/72M,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ CN_CFG_MCLK  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ�������λHz��
#else
#define CN_CFG_USE_USERTIMER        (0U)//�Ƿ�ʹ��LPTIMER��Ϊϵͳʱ��
#define CN_CFG_TIME_BASE_HZ         CN_CFG_MCLK//(32000U)//(8000U)
#if (!CN_CFG_USE_USERTIMER)
#define CN_CFG_USE_BYPASSTIMER      (0U)//0��ʾ��ʹ����·��ʱ����1��ʾʹ����·��ʱ��
#define CN_CFG_TIME_PRECISION       (500U)/*���ȵ�λ��US*/
#else
#define CN_CFG_USERTIMER_PRESC      (1U)//����ʹ��SYSTICK��ʱ������ָ���û���ʱ���ķ�Ƶ��
#define CN_CFG_FINE_US (0x1F4000U)  //1/32000,tick����ʱ�����ڣ���uSΪ��λ��32λ������������С����ռ16λ����Ҳ������ticks�������65535uS
#define CN_CFG_FINE_HZ (0x831U)  //tick����ʱ��Ƶ�ʣ���CN_CFG_FINE_US�ĵ���
#define CN_CFG_TIME_PRECISION       (200U)/*���ȵ�λ��US*/
#endif
#endif

//CPU�ܹ��������,��ѡ������������,�����С��,�ǿ�ѡ��,����������,
//��CPU�ֳ��̶�,���ֳ���BSP��arch_feature.h�ļ��ж���
//�洢����С�˵�����
#define CN_CFG_LITTLE_ENDIAN        0
#define CN_CFG_BIG_ENDIAN           1
#define CN_CFG_BYTE_ORDER          CN_CFG_LITTLE_ENDIAN

#define CN_LCD_XSIZE 240
#define CN_LCD_YSIZE 320

//����CPU�ͺ�,����ST��˾�Ĺ̼���ʱ��Ҫ----for cube ��
//��stm32f1xx.h�ļ����������ͷ�ļ�
//#define STM32F100xB     //STM32F100C4, STM32F100R4, STM32F100C6, STM32F100R6, STM32F100C8, STM32F100R8, STM32F100V8, STM32F100CB, STM32F100RB and STM32F100VB
//#define STM32F100xE     //STM32F100RC, STM32F100VC, STM32F100ZC, STM32F100RD, STM32F100VD, STM32F100ZD, STM32F100RE, STM32F100VE and STM32F100ZE
//#define STM32F101x6     //STM32F101C4, STM32F101R4, STM32F101T4, STM32F101C6, STM32F101R6 and STM32F101T6 Devices
//#define STM32F101xB     //STM32F101C8, STM32F101R8, STM32F101T8, STM32F101V8, STM32F101CB, STM32F101RB, STM32F101TB and STM32F101VB
//#define STM32F101xE     //STM32F101RC, STM32F101VC, STM32F101ZC, STM32F101RD, STM32F101VD, STM32F101ZD, STM32F101RE, STM32F101VE and STM32F101ZE
//#define STM32F101xG     //STM32F101RF, STM32F101VF, STM32F101ZF, STM32F101RG, STM32F101VG and STM32F101ZG
//#define STM32F102x6     //STM32F102C4, STM32F102R4, STM32F102C6 and STM32F102R6
//#define STM32F102xB     //STM32F102C8, STM32F102R8, STM32F102CB and STM32F102RB
//#define STM32F103x6     //STM32F103C4, STM32F103R4, STM32F103T4, STM32F103C6, STM32F103R6 and STM32F103T6
//#define STM32F103xB     //STM32F103C8, STM32F103R8, STM32F103T8, STM32F103V8, STM32F103CB, STM32F103RB, STM32F103TB and STM32F103VB
//#define STM32F103xE     //STM32F103RC, STM32F103VC, STM32F103ZC, STM32F103RD, STM32F103VD, STM32F103ZD, STM32F103RE, STM32F103VE and STM32F103ZE
//#define STM32F103xG     //STM32F103RF, STM32F103VF, STM32F103ZF, STM32F103RG, STM32F103VG and STM32F103ZG
//#define STM32F105xC     //STM32F105R8, STM32F105V8, STM32F105RB, STM32F105VB, STM32F105RC and STM32F105VC
//#define STM32F107xC     //STM32F107RB, STM32F107VB, STM32F107RC and STM32F107VC

//����CPU�ͺ�,����ST��˾�Ĺ̼���ʱ��Ҫ----for ��ͳ ��
//��stm32f10x.h�ļ����������ͷ�ļ�
//#define STM32F10X_LD      //!< STM32F10X_LD: STM32 Low density devices
//#define STM32F10X_LD_VL   //!< STM32F10X_LD_VL: STM32 Low density Value Line devices
//#define STM32F10X_MD      //!< STM32F10X_MD: STM32 Medium density devices
//#define STM32F10X_MD_VL   //!< STM32F10X_MD_VL: STM32 Medium density Value Line devices
//#define STM32F10X_HD      //!< STM32F10X_HD: STM32 High density devices
//#define STM32F10X_HD_VL   //!< STM32F10X_HD_VL: STM32 High density value line devices
//#define STM32F10X_XL      //!< STM32F10X_XL: STM32 XL-density devices
//#define STM32F10X_CL      //!< STM32F10X_CL: STM32 Connectivity line devices

#ifdef __cplusplus
}
#endif
#endif // __BOARD_CONFIG_H__



