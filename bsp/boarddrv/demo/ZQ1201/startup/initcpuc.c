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
#ifdef CFG_CPU_ZQ12XX_M0
#include "arch_feature.h"
#include "stdint.h"
#include "int.h"
#include "cpu.h"
#include "cpu_peri.h"
#include "hard-exp.h"
#include "lowpower.h"
#include "djyos.h"
//#include "core_cmFunc.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

extern   uint32_t   msp_top[ ];
//extern void __set_PSP(uint32_t topOfProcStack);
//extern void __set_PRIMASK(uint32_t priMask);
//extern void __set_CONTROL(uint32_t control);

extern void Iboot_IAP_SelectLoadProgam(void);
//static void __set_PSP(uint32_t topOfProcStack)
//{
//  __asm volatile ("MSR psp, %0\n" : : "r" (topOfProcStack) : "sp");
//}
//static void __set_MSP(uint32_t topOfMainStack)
//{
//  __asm volatile ("MSR msp, %0\n" : : "r" (topOfMainStack) : "sp");
//}
//
//static void __set_PRIMASK(uint32_t priMask)
//{
//  __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
//}
//static void __set_CONTROL(uint32_t control)
//{
//  __asm volatile ("MSR control, %0" : : "r" (control) : "memory");
//}
void Startup_NMI(void)
{
    while(1);
}
void Startup_Hardfault(void)
{
    while(1);
}

#include <silan_pmu.h>
#include <silan_syscfg.h>
#include <silan_m0_cache.h>
#include <silan_bootspi.h>
#include <silan_sdram.h>
#include "silan_syscfg_regs.h"


static void SystemInit(void)
{
    silan_pmu_wdt_reset_disable();
    silan_dsp_ldo_config(LDO_OUT_12V);
    silan_pwr_iso_disable();

    silan_vlsp_cclk_config(CLK_ON);
    silan_soc_ldo_config(LDO_OUT_12V);

    silan_dsp_dcache_cfg(DCACHE_MODE_DC);

    // LQFP64 Close MCU DEBUG
    //silan_mcu_debug_close();
    silan_risc_debug_close();
    silan_dsp_debug_close();
}

static void ResetInit(void)
{
     __REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<2);
    silan_audiopll_init();
    silan_syspll_init(SYSPLL_SSCG_OFF);
    silan_sysclk_change(SYSCLK_FROM_PLL, SYSPLL_160M);
    silan_timer_cclk_config(TIMER_CCLK_CLKSYS_DIV2, CLK_ON);

//  iomux_init();

//  silan_module_clkon(CLK_UART2, CLK_ON);
/// silan_uart_sys_init(UART_2, BAUDRATE_115200);

//  SL_LOG("\n\n\n");
//  SL_LOG("CLK_SYS = %dM", get_sysclk_val_settled()/1000000);

    m0_cache_init(CACHE_ENABLE, 0, 0, 0);
//  m0_cache_init(CACHE_DISABLE, 0, 0, 0);
//  cache_spi_cmd_sel(SF_CMD_FRD);

    silan_mcu_cclksel(MCU_CCLK_DIV1);

//  int5ms_f = 0;
//  int1s_f = 0;
    silan_pic_init();
}

static void SDRAM_Init(void)
{
    silan_sdram_init();
}
void Init_Cpu(void)
{
    __set_PSP((uint32_t)msp_top);
    __set_PRIMASK(1);
    __set_CONTROL(0);

    SystemInit();
    ResetInit();

    SDRAM_Init();

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
#endif

#ifdef CFG_CPU_ZQ12XX_CK
#include "csi_core.h"
#include "arch_feature.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

extern void Iboot_LoadPreload(void);

void Init_Cpuc(void)
{
    /* Here we may setting exception vector, MGU, cache, and so on. */
//  csi_cache_set_range(0, 0x00000000, CACHE_CRCR_4M, 1);
//  csi_cache_set_range(1, 0x22000000, CACHE_CRCR_8M, 1);
//  csi_dcache_enable();

#if (CFG_RUNMODE_BAREAPP == 1)
    Iboot_LoadPreload();
#else
    Iboot_IAP_SelectLoadProgam();
#endif
}

void IAP_GpioPinInit(void)
{

}

bool_t Iboot_IAP_IsForceIboot(void)
{
    return false;
}

void epc_test(uint32_t r_epc)
{
    if(r_epc>=0x021e0100)
    {
        return;
    }
    else if(r_epc<=0x02160000)
    {
        return;
    }
}

#endif

