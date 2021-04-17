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
//����ģ��:  CPU��ʼ��
//���ߣ�     lst
//�汾��      V1.0.0
//��ʼ�汾������ڣ�2013-05-27
//�ļ�����:  cpuʱ�ӳ�ʼ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2014-01-20
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: �����ǳ���,������֤Ŀ��
//      ��һ���������ж�ʱ���Ƿ��Ѿ�����Ĵ���
//------------------------------------------------------
#include "stdint.h"
#include "cpu_peri.h"
#include "hard-exp.h"
#include "arch_feature.h"
#include "stm32f7xx.h"
//#include "core_cmFunc.h"
#include "cpu-optional.h"
#include "core_cm7.h"
#include "arm32_feature.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

#ifndef __CHECK_DEVICE_DEFINES
#define __CHECK_DEVICE_DEFINES
#endif
//#define VECT_TAB_SRAM
#define VECT_TAB_OFFSET  0x0

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

extern bool_t SysClockInit(void);
extern void Iboot_LoadPreload(void);

struct ScbReg volatile * const startup_scb_reg
                        = (struct ScbReg *)0xe000ed00;

void Startup_NMI(void)
{
    while(1);
}
void Startup_Hardfault(void)
{
    while(1);
}
void Init_Cpu(void);
//��ʼ���쳣��������ʱ�����ж��Լ��������쳣���ǹرյģ���ֻ��Ҫ�ṩ4��������
// ջ������λ��NMI��hard fault�����Ҿ����պ������ɡ�
//ϵͳ����ʱʹ�õ��쳣�����쳣ģ�鴦���أ���ram�С�
const u32 gc_u32StartupExpTable[4] __attribute__ ((section(".StartupExpTbl")))=
{
    (u32)msp_top,
    (u32)Init_Cpu,
    (u32) Startup_NMI,
    (u32) Startup_Hardfault
};
void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_FAULTMASK(1);
    __set_CONTROL(0);

    #if (_D_FPU_USED == 1)      //_D_FPU_USED������gcc����
        startup_scb_reg->CPACR = (3UL << 20)|(3UL << 22);    //ʹ��FPU
        startup_scb_reg->FPCCR = (1UL << 31);                //�ر�lazy stacking
    #endif
    switch(startup_scb_reg->CPUID)
    {
    }

#ifdef USE_HAL_DRIVER
    HAL_TickInit();
#endif
    while(false==SysClockInit());

    extern void SRAM_Init(void);
    SRAM_Init();        //Ƭ��RAM ��ʼ��

#if (CN_CPU_OPTIONAL_CACHE == 1)
    SCB_EnableICache();         //ָ�����cacheʹ��
    SCB_EnableDCache();
#endif

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

//-----------------------------------------------------------------
//���ܣ�APPӦ�ó������ں�����iboot�����в����ã�app��lds�ļ��У��뽫�ú���
//     �����ӵ�ַ����IbootSize + 512����ʼλ�á��ú�������ջָ�벢���س���
//��������
//���أ��ޡ�
//-----------------------------------------------------------------
void AppStart(void)
{
    __set_MSP((uint32_t)msp_top);
    __set_PSP((uint32_t)msp_top);
    Iboot_LoadPreload();
}

//-----------------------------------------------------------------
//���ܣ�IAP�����������ģʽ�����GPIO���ų�ʼ�������ڴ�ʱϵͳ��û�м��أ�ֻ��ʹ
//      ��ֱ�ӵ�ַ���������ܵ���gpio��صĿ⺯����
//      �������ʹ��gpio����־�����������Ǳ��룬��ɾ����
//��������
//���أ��ޡ�
//-----------------------------------------------------------------
void IAP_GpioPinInit(void)
{
    GPIO_PowerOn(GPIO_A);
    GPIO_CfgPinFunc(GPIO_A,PIN0,GPIO_MODE_IN,GPIO_OTYPE_OD,\
            GPIO_SPEED_VH,GPIO_PUPD_PD);

}

//-----------------------------------------------------------------
//���ܣ���Ӳ�������Ƿ�ǿ�ƽ���Iboot�����˺�������TRUE����ǿ������Iboot��ͨ����ʹ
//      ��һ��gpio��ͨ�����߾�����
//      ��������£������������APP���ǿ�����runiboot�����л���Iboot״̬�ģ�����
//      ��Ӳ����Ŀ���ж���
//     1���������쳣���󣬲�����shell�л�ʱ���ṩһ�����ȴ�ʩ��
//     2�����ڰ�ȫ���ǣ�APP��û�а����л����룬����������Դ�Ĺ�ϵ���õ���shell��
//��������
//���أ��ޡ�
//˵�������������漰����Ӳ�������ڱ��ļ��г�ʼ�����ر���Ҫע����ǣ����������δ
//      ���صĺ������ر��ǿ⺯����
//      �����������ṩ�����û��������ӦӲ�������Լ򵥷���false��
//-----------------------------------------------------------------
bool_t Iboot_IAP_IsForceIboot(void)
{
    u32 flag;
    IAP_GpioPinInit( );
    flag=GPIO_GetData(GPIO_A)&PIN0;
    if(flag==0)
        return false;
    return true;
}




