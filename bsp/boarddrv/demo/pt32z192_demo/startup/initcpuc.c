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
//��ʼ�汾������ڣ�2009-08-05
//�ļ�����:  cpuʱ�ӳ�ʼ��
//����˵��:  ��
//�޶���ʷ:
//2. ...
//1. ����: 2009-08-05
//   ����: lst
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "arch_feature.h"
#include "stdint.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "hard-exp.h"
#include "lowpower.h"
#include "djyos.h"
#include "pt32z192xx.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

extern void SRAM_Init(void);
extern void Iboot_IAP_SelectLoadProgam(void);

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
    switch(startup_scb_reg->CPUID)
    {
        case CN_M3_REVISION_R0P0:
            break;    //�г�û�а汾0��оƬ
        case CN_M3_REVISION_R1P0:
            startup_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case CN_M3_REVISION_R1P1:
            startup_scb_reg->CCR |= 1<<bo_scb_ccr_stkalign;
            break;
        case CN_M3_REVISION_R2P0:break;    //����ûʲôҪ����
    }
    SystemInit();

//    __LP_BSP_HardInit( );   //�����������ӵ�startup

//  if( __LP_BSP_GetSleepLevel() == CN_SLEEP_L3)    //�����������ӵ�startup
//  {
//      __LP_BSP_RestoreRamL3();    //�˺��ڴ��Ѿ��ָ������Ե���ȫ�������ˡ�
//
//      HardExp_Init();
//      Int_ContactTrunk();
//      cm_cpsie_f();
//
//      __asm_start_thread(g_ptEventRunning->vm);
//  }
//  else
//      Iboot_LoadPreload( );

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

extern void Iboot_LoadPreload(void);
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
//    u32 flag;
//    IAP_GpioPinInit( );
//    flag=pg_gpio_regc->IDR&(1<<10);
//    if(flag==0)
//        return false;
//    return true;

    return false;

}

