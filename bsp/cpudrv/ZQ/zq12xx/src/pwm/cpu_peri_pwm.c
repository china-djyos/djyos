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
// =============================================================================

// �ļ���     ��
// ģ������:
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��:
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "cpu_peri_pwm.h"
#include "djyos.h"
#include "stdio.h"

//#include "silan_pwm_regs.h"
//#include "silan_types.h"
#include "silan_syscfg.h"
#include "silan_iomux.h"
#include "silan_iomux.h"

//Reg define
typedef struct
{
    vu32  PWMPSC;    //0x00
    vu32  PWM0D;      //0x04
    vu32  PWM1D;     //0x08
    vu32  PWM2D;     //0x0c
    vu32  PWM3D;     //0x10
    vu32  PWM4D;     //0x14
    vu32  PWM5D;     //0x18
    vu32  PWM6D;         //0x1c
    vu32  PWM7D;         //0x20
    vu32  PWMP01;    //0x24
    vu32  PWMP23;    //0x28
    vu32  PWMP45;    //0x2c
    vu32  PWMP67;    //0x30
    vu32  PWMCON;    //0x34
}tagPWMReg;

#define CN_PWM_CHANNEL_START     ( CN_PWM_CHANNEL0 )
#define CN_PWM_CHANNEL_END       ( CN_PWM_CHANNEL7 )

//channel number select
#define CHANNEL_NUM_GET(channelNum)     (1<< (channelNum+4) )

//base PWM base addr
#define BASE_PWM_ADDR  ( 0x410D0000 )

static tagPWMReg volatile *const sPWMReg = (tagPWMReg*) BASE_PWM_ADDR;

// =============================================================================
//����: �򿪶�Ӧ��ͨ��
//������
//���أ���
// =============================================================================
static void __CPU_PWM_ChannelOpen(u8 channelNum)
{
    sPWMReg->PWMCON  |= CHANNEL_NUM_GET(channelNum);
    return ;
}

// =============================================================================
//����: ����ռ�ձ�
//������
//���أ���
// =============================================================================

static void __CPU_PWM_ChannelDutySet(u8 channelNum,u32 para)
{
    switch(channelNum)
    {
            case CN_PWM_CHANNEL0: sPWMReg->PWM0D = (para-1);break;
            case CN_PWM_CHANNEL1: sPWMReg->PWM1D = (para-1);break;
            case CN_PWM_CHANNEL2: sPWMReg->PWM2D = (para-1);break;
            case CN_PWM_CHANNEL3: sPWMReg->PWM3D = (para-1);break;
            case CN_PWM_CHANNEL4: sPWMReg->PWM4D = (para-1);break;
            case CN_PWM_CHANNEL5: sPWMReg->PWM5D = (para-1);break;
            case CN_PWM_CHANNEL6: sPWMReg->PWM6D = (para-1);break;
            case CN_PWM_CHANNEL7: sPWMReg->PWM7D = (para-1);break;
            default:printk("__CPU_ChannelParaSet func arg channelNum Err\r\n");
            return;
    }

}

// =============================================================================
//����: ����ռ�ձ�����
//������
//���أ���
// =============================================================================

static void __CPU_PWM_ChannelPeriodSet(u8 channelNum,u32 para)
{
    u8 channelSet;
    channelSet = (u8)(channelNum/2);

    switch(channelSet)
    {
        case CN_PWM_CHANNEL0:
             sPWMReg->PWMP01 = (para-1);
             break;
        case CN_PWM_CHANNEL1:
             sPWMReg->PWMP23 = (para-1);
             break;
        case CN_PWM_CHANNEL2:
             sPWMReg->PWMP45 = (para-1);
             break;
        case CN_PWM_CHANNEL3:
             sPWMReg->PWMP67 = (para-1);
             break;
        default:printk("__CPU_ChannelPeriodSet func arg channelNum Err\r\n");
        return;
    }
}

// =============================================================================
//����: PWM Ԥ��Ƶ����
//������ preDiv ��Ƶֵ0~255
//���أ���
// =============================================================================

static void __CPU_PWM_PreDivSet(u8 preDiv)
{
    sPWMReg->PWMPSC = preDiv -1;
}

// =============================================================================
//����: PWM ʱ��Դ�Լ�����ѡ������
//������
//���أ���
// =============================================================================

static void __CPU_PWM_ClkConfig(u8 clkSelect,u8 onOff)
{

    if(clkSelect > EN_PWM_RCL_CLK_DIV2)
    {
        printk("__CPU_PWM_ClkConfig clkSelect Err\r\n");
        return;
    }

    silan_pwm_cclk_config(clkSelect, onOff);
}

// =============================================================================
//����: ����IO��������
//������pin:�߱�����Ϊpwm ����Ĺܽź� drvLevel ��������
//
//���أ���
// =============================================================================

static void __CPU_ConfigIODrvLevel(u32 pin,u32 drvLevel)
{
    silan_io_driver_config(pin,drvLevel);
}


// =============================================================================
//����: ����IO��������
//������pin:�߱�����Ϊpwm ����Ĺܽź� drvLevel ��������
//
//���أ���
// =============================================================================

static void __CPU_PWM_IO_Config(u32 io, u32 x_para, u32 x_period)
{
    u32 channelNo =  io;
    channelNo = channelNo % 8;                          //�ܽŶ�Ӧ��ͨ����

    if(x_para == 0 || x_period == 0)
    {
        printk("__CPU_PWM_IO_Config func arg x_para or x_period Err\r\n");
        return;
    }else
    {
        __CPU_PWM_ChannelDutySet  (channelNo,x_para);   //���� ռ�ձ�

        __CPU_PWM_ChannelPeriodSet(channelNo,x_period); //��������

        __CPU_PWM_ChannelOpen(channelNo);         //�򿪶�Ӧ��PWM ͨ��

        silan_io_func_config(io, IO_FUNC_PWM);          //����PWM���ù���
    }

}


// =============================================================================
//����: PWM ģ�鰲װ
//���жԸ�ģ���һЩĬ�ϳ�ʼ��
//
//���أ���
// =============================================================================

#define CN_DEFAULT_PRE (60)

void PWM_ModuleInstall(void)
{
    __CPU_PWM_ClkConfig(EN_PWM_PLL_REF_CLK_DIV2,EN_PWM_CLK_ON);//Ĭ������PWMʱ��Դ�Լ���Ƶϵ��
    __CPU_PWM_PreDivSet(CN_DEFAULT_PRE);                       //����Ĭ��Ԥ��ƵΪ60
    //����Ĭ������ܽ�,�Լ���Ӧ����������
    __CPU_ConfigIODrvLevel(IO_CONFIG_PB8,EN_IO_DRV_8MS);
    __CPU_PWM_IO_Config(IO_CONFIG_PB8, 5, 100);
}

// =============================================================================
//����: PWM ģ�鰲װ
//���жԸ�ģ���һЩĬ�ϳ�ʼ��
//
//���أ���
// =============================================================================

bool_t PWM_Ctrl(u32 cmd,u32 para1,u32 para2,u32 para3)
{
    bool_t ret = false;

    switch(cmd)
    {
        case EN_PWM_PRE_DIV_SET:
            if(para1 > 255)
            {
                printk("para1 out of range\r\n");
                ret = false ;
            }else
                __CPU_PWM_PreDivSet((u8)para1);
            break;
        case EN_PWM_CLK_SET:
            __CPU_PWM_ClkConfig((u8)para1,(u8) para2);
            break;
        case EN_IO_DRV_SET:
            __CPU_ConfigIODrvLevel(para1,para2);
            break;
        case EN_PWM_PARA:
            __CPU_PWM_IO_Config(para1, para2, para3);
            break;
        default:;break;
    }

    return ret;
}

#endif



































































































































































































































