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
#include "sam4e.h"
//#include "core_cmFunc.h"
#include "arm32_feature.h"
#ifndef __CHECK_DEVICE_DEFINES
#define __CHECK_DEVICE_DEFINES
#endif

#include "core_cm4.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_PRIMASK(uint32_t priMask);
extern void __set_FAULTMASK(uint32_t faultMask);
extern void __set_CONTROL(uint32_t control);

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
const u32 gc_u32StartupExpTable[256] __attribute__ ((section(".StartupExpTbl")))=
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

    #if (_D_FPU_USED  == 1)
    startup_scb_reg->CPACR = (3UL << 20)|(3UL << 22);    //ʹ��FPU
    startup_scb_reg->FPCCR = (1UL << 31);                //�ر�lazy stacking
    #endif
    switch(startup_scb_reg->CPUID)
    {
//        case cn_revision_r0p1://todo
//            break;    //����ûʲôҪ����
    }

    //�ع�
    WDT->WDT_MR = WDT_MR_WDDIS;

    void System_ClkInit(void);
    System_ClkInit();

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

extern void Iboot_LoadPreload(void);
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

#define EEFC_FMR_Val    0x00000500      // 0x00000000

#define PMC_SETUP       1               // Reset values:
#define PMC_SCER_Val    0x00000000      // WO register (0x00000001)
#define PMC_PCER0_Val   0x00000000      // WO register (0x00000000)
#define PMC_PCER1_Val   0x00000000      // WO register (0x00000000)
#define CKGR_MOR_Val    0x01003F29      //

// PLL A Freq = (Main CLOCK Freq / DIVA) * (MULA + 1)
// ���£� MULA = 0x1F, DIVA = 0x04, PLLA = 12M / 0x04 * (0x27+1) = 120M
#define CKGR_PLLAR_Val  0x00270604      // 0x00003F00
#define FILL_0          0x00000000      // 0x00000000
#define PMC_MCKR_Val    0x00000002      // ѡ�� Master Clock Source ΪPLLA_CLK
#define PMC_USB_Val     0x00000100      // 0x00000000
#define PMC_PCK0_Val    0x00000001      // 0x00000000
#define PMC_PCK1_Val    0x00000004      // 0x00000000
#define PMC_PCK2_Val    0x00000000      // 0x00000000

void System_ClkInit(void)
{
    EFC->EEFC_FMR = EEFC_FMR_Val;

    PMC->PMC_WPMR = 0x504D4300;                 /* Disable write protect             */

    /* before we change the clocksetup we switch Master Clock Source
     to MAIN_CLK and set prescaler to one
     */
    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

    PMC->PMC_MCKR = (PMC->PMC_MCKR & ~ PMC_MCKR_PRES_Msk) | PMC_MCKR_PRES_CLK_1;
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));     /* Wait for MCKRDY                   */

#if (CKGR_MOR_Val & (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN))    /* If MOSCRCEN/MOSCXTEN set  */
    PMC->CKGR_MOR  = ((PMC->CKGR_MOR &  CKGR_MOR_MOSCSEL)    |    /* Keep the current MOSCSEL  */
                    (CKGR_MOR_Val  & ~CKGR_MOR_MOSCSEL)    |    /* Set value except MOSCSEL  */
                    (CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN)|    /* and enable bothe OSCs     */
                    (CKGR_MOR_KEY(0x37))                      );
#if (CKGR_MOR_Val & CKGR_MOR_MOSCRCEN)
    while (!(PMC->PMC_SR & PMC_SR_MOSCRCS));   /* Wait for MOSCRCS                   */
#endif
#if (CKGR_MOR_Val & CKGR_MOR_MOSCXTEN)
    while (!(PMC->PMC_SR & PMC_SR_MOSCXTS));   /* Wait for MOSCXTS                   */
#endif
    PMC->CKGR_MOR  =  ((CKGR_MOR_Val      ) |  /* set the desired selection          */
                     (CKGR_MOR_KEY(0x37))  );
    while (!(PMC->PMC_SR & PMC_SR_MOSCSELS));  /* Wait for MOSCSELS                  */
#endif

  /* write PLLBDIV2, PLLADIV2 */
    PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~(PMC_MCKR_PLLADIV2)) |
                   (PMC_MCKR_Val  &  (PMC_MCKR_PLLADIV2))  );
    while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                    */

#if (CKGR_PLLAR_Val & CKGR_PLLAR_MULA_Msk)   /* If PLL is activated                */
    PMC->CKGR_PLLAR = (CKGR_PLLAR_Val | CKGR_PLLAR_ONE);
    while (!(PMC->PMC_SR & PMC_SR_LOCKA));     /* Wait for LOCKA                     */
#endif

    if ((PMC_MCKR_Val & PMC_MCKR_CSS_Msk) >= 2)
    {
        /* Write PRES field only                                                       */
        PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_PRES_Msk)  );
        while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */

        /* Write CSS field only                                                        */
        PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_CSS_Msk)  );
        while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */
    }
    else
    {
        /* Write CSS field only                                                        */
        PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_CSS_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_CSS_Msk)  );
        while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */

        /* Write PRES field only                                                       */
        PMC->PMC_MCKR = ((PMC->PMC_MCKR & ~PMC_MCKR_PRES_Msk) |
                     (PMC_MCKR_Val  &  PMC_MCKR_PRES_Msk)  );
        while (!(PMC->PMC_SR & PMC_SR_MCKRDY));    /* Wait for MCKRDY                  */
    }

    PMC->PMC_SCER  = PMC_SCER_Val;
    PMC->PMC_PCER0 = PMC_PCER0_Val;
    PMC->PMC_PCER1 = PMC_PCER1_Val;

#if (PMC_SCER_Val & PMC_SCER_PCK0)
    PMC->PMC_PCK[0] = PMC_PCK0_Val;            /* Write PCK0                         */
    while (!(PMC->PMC_SR & PMC_SR_PCKRDY0));   /* Wait for PCKRDY0                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK1)
    PMC->PMC_PCK[1] = PMC_PCK1_Val;            /* Write PCK1                         */
    while (!(PMC->PMC_SR & PMC_SR_PCKRDY1));   /* Wait for PCKRDY1                   */
#endif
#if (PMC_SCER_Val & PMC_SCER_PCK2)
    PMC->PMC_PCK[2] = PMC_PCK2_Val;            /* Write PCK2                         */
    while (!(PMC->PMC_SR & PMC_SR_PCKRDY2));   /* Wait for PCKRDY2                   */
#endif


  PMC->PMC_WPMR = 0x504D4301;                /* Enable write protect               */
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
