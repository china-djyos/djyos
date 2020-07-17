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

// �ļ���     ��board.c
// ģ������: �����ز��ֳ�ʼ�������õ�
// ģ��汾: V1.00
// ������Ա: HM
// ����ʱ��: 5/11.2015
// =============================================================================

#include "stdint.h"
#include "stddef.h"
#include "os.h"
#include "cpu_peri.h"

#include <silan_iomux.h>
#include <silan_gpio.h>
#include <silan_iomux_regs.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"board config"//�����������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                             //ѡ�third��system��bsp��user����������������IDE�з���
//select:required                           //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                           //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"kernel","cpu onchip gpio"//������������������������none����ʾ�������������
                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                     //��������������������������none����ʾ�������������
                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                            //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                            //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header                       //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure
// =============================================================================
// =============================================================================
// ���ܣ����ݾ���İ�����ô��ڵ�GPIO�����Ź��ܣ�����������أ����Ըú������ڸ��ļ���CPU
//      ��������ֱ�ӵ��øú�������ʼ�����ڵ�GPIO����
//      ��Ҫ����GPIO�˿ںʹ���ʱ��ʹ�ܡ�GPIO���á���ӳ�䡢ʱ�ӵ�
// ��������
// ���أ�true
// =============================================================================
void Board_Init(void)
{
    //UART3
    silan_io_func_config(IO_CONFIG_PD7, PD7_FUNC_UART3);
    silan_io_func_config(IO_CONFIG_PD8, PD8_FUNC_UART3);

    // UART2 to PA4/5
    silan_io_func_config(IO_CONFIG_PA4, PA4_FUNC_UART2);
    silan_io_func_config(IO_CONFIG_PA5, PA5_FUNC_UART2);

    // IIS1 IN 2.0
    //  silan_io_func_config(IO_CONFIG_PB4, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PB5, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PB6, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PB7, IO_FUNC_GPIO);
    // IIS3 IN 2.0

    silan_io_func_config(IO_CONFIG_PC3, PC3_FUNC_I3_I2S);
    silan_io_func_config(IO_CONFIG_PC4, PC4_FUNC_I3_I2S);
    silan_io_func_config(IO_CONFIG_PC5, PC5_FUNC_I3_I2S);
    silan_io_func_config(IO_CONFIG_PC6, PC6_FUNC_I3_I2S);

    //I2_IIS IN
    //  silan_io_func_config(IO_CONFIG_PB8, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PC0, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PC1, IO_FUNC_GPIO);
    //  silan_io_func_config(IO_CONFIG_PC2, IO_FUNC_GPIO);

    silan_io_func_config(IO_CONFIG_PA6,PA6_FUNC_SPDIF_IN);

    // SPDIF
    silan_io_func_config(IO_CONFIG_PA3,PA3_FUNC_SPDIF_O);
    silan_io_func_config(IO_CONFIG_PA7,IO_FUNC_GPIO);
    silan_io_func_config(CTRL_LED1,IO_FUNC_GPIO);
    //  silan_io_func_config(CTRL_LED2,IO_FUNC_GPIO);
    silan_io_func_config(CTRL_LED3,IO_FUNC_GPIO);
    //  silan_io_func_config(CTRL_MUTE,IO_FUNC_GPIO);
    silan_io_func_config(CTRL_IR,IO_FUNC_GPIO);

    /////////////////////////////////////////////////
    silan_io_func_config(IO_CONFIG_PB6,IO_FUNC_GPIO); ////�������MIC
    silan_io_pullup_config(IO_CONFIG_PB6,IO_ATTR_INPUT_EN);
    silan_io_direction(IO_CONFIG_PB6,IO_INPUT);

    silan_io_func_config(IO_CONFIG_PB7,IO_FUNC_GPIO); ////�������MIC
    silan_io_pullup_config(IO_CONFIG_PB7,IO_ATTR_INPUT_EN);
    silan_io_direction(IO_CONFIG_PB7,IO_INPUT);

    ///////////////////////////////////////////////////////
    silan_io_func_config(IO_CONFIG_PD9,IO_FUNC_GPIO); //PD9
    silan_io_driver_config(IO_CONFIG_PD9, IO_ATTR_DS_8mA);
    silan_io_output(IO_CONFIG_PD9);
    silan_io_set_low(IO_CONFIG_PD9);

    //silan_io_output(CTRL_MUTE);

    //  silan_io_output(CTRL_LED2);
    //silan_io_output(CTRL_LED3);
    //silan_io_input(CTRL_IR);

    //  silan_io_set_high(CTRL_LED2);
    //silan_io_set_high(CTRL_LED3);
    //  silan_io_set_low(CTRL_MUTE);
}

