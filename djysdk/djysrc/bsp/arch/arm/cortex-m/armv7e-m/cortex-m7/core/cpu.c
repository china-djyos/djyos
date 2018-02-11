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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//2. ����: 2009-04-24
//   ����: lst
//   �°汾��: V1.0.1
//   �޸�˵��: ɾ����һЩΪdlsp�汾׼���Ķ���
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------

#include "align.h"
#include "stdint.h"
#include "stdlib.h"
#include "int.h"
#include "hard-exp.h"
#include "string.h"
#include "arch_feature.h"
#include "cpu.h"
#include "djyos.h"
#include "cpu_peri.h"
#include <board-config.h>

#include "core_cm7.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_MSP(uint32_t topOfProcStack);
// void __start_systick(void);


extern s64  g_s64OsTicks;             //����ϵͳ����ticks��
u32 g_u32CycleSpeed; //for(i=j;i>0;i--);ÿѭ��������*1.024
u32 g_u32HundreUsfor;

//todo �ú����ǲ��ǲ���Ҫ��.
//----����ָ��ָ����ʱ����-----------------------------------------------------
//����: ����ָ����ʱ����,ʹ�����ú��ֱ������ͱ����Ż�ѡ��,djy_delay_us����׼ȷ��ʱ��
//��������
//����: ��
//��ע: ��������ֲ����
//-----------------------------------------------------------------------------
void __DjySetDelay(void)
{
    uint32_t counter,u32_fors=10000,clksum;
    volatile uint32_t u32loops;

    clksum = 0xFFFFFF;  //�˴�����ȡ���������ֵ��M3��M4�ں�SYSTICK��24λ������

    pg_systick_reg->reload = clksum;
    pg_systick_reg->current = 0;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                            |(1<<bo_systick_ctrl_tickint)   //��������ж�
                            |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
    for(u32loops=u32_fors;u32loops>0;u32loops--);       //ѭ��u32_fors��

    //�˴����Խ׶���ȷ�������ݼ�����������0ʱ��˵��һ�������������޷��������ѭ��
    //���Сu32_forsֵ�������systick�жϷ����������ж��ٸ���������
    if(pg_systick_reg->ctrl & bm_systick_ctrl_countflag)
    {
        while(1);
    }
    //ֹͣ��ʱ��
    pg_systick_reg->ctrl =   (0<<bo_systick_ctrl_enable)    //��ʹ��
                            |(1<<bo_systick_ctrl_tickint)   //��������ж�
                            |(1<<bo_systick_ctrl_clksource);//���ں�ʱ��
    counter = pg_systick_reg->current;      //��ȡѭ��u32_fors������ʱ��

    counter = clksum - counter;    //ȡʵ����������
    g_u32CycleSpeed = ((uint64_t)counter * (u64)10E8)/CN_CFG_FCLK/u32_fors;//���������64λ
    g_u32CycleSpeed = (g_u32CycleSpeed << 10) / 1000;     //����1.024��

    g_u32HundreUsfor = (1024<<7)/g_u32CycleSpeed;
}

//----��ʼ��tick---------------------------------------------------------------
//����: ��ʼ����ʱ��,������tick�жϺ���,������ʱ��.
//����: ��
//����: ��
//��ע: ����������ֲ���к���.
//-----------------------------------------------------------------------------
__attribute__((weak)) void __DjyInitTick(void)
{
    HardExp_ConnectSystick(Djy_IsrTick);
    pg_systick_reg->reload = CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->current =CN_CFG_FCLK/CN_CFG_TICK_HZ;
    pg_systick_reg->ctrl =   (1<<bo_systick_ctrl_enable)    //ʹ��
                                |(1<<bo_systick_ctrl_tickint)   //��������ж�
                                |(1<<bo_systick_ctrl_clksource);//��ʹ���ⲿʱ��

}

//----��ȡ��ǰʱ��(uS)---------------------------------------------------------
//���ܣ���ȡ��ǰʱ��(uS),�Ӽ������������������us����64λ��Ĭ�ϲ������
//      g_s64OsTicks Ϊ64λ��������64λϵͳ�У���ȡ g_s64OsTicks ��Ҫ����1��
//      ����,��Ҫʹ��ԭ�Ӳ�����
//��������
//���أ���ǰʱ��
//˵��: ����һ��׮����,��systime.c�ļ��� DjyGetSysTime �������á�
//      ���systime��ʹ��ticks��Ϊʱ�����������ɱ��ֿպ�����
//-----------------------------------------------------------------------------
__attribute__((weak)) s64 __DjyGetSysTime(void)
{
    s64 time;
    static s64 BakTime = 0;
    u32 temp;
    atom_low_t atom_low;
    temp =CN_CFG_FCLK/CN_CFG_TICK_HZ - pg_systick_reg->current;

    atom_low = Int_LowAtomStart();
    time = g_s64OsTicks;
    Int_LowAtomEnd(atom_low);
    time = time*CN_CFG_TICK_US + (temp*CN_CFG_FINE_US >>16);
    if(time < BakTime)
        time += CN_CFG_TICK_US;
    BakTime = time;

    return time;
}



extern void Load_Preload(void);
#include <exp.h>
#include <osboot.h>
#include <IAP_Inner.h>
extern tagIapVar pg_IapVar;

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG         (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)

// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void reboot(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_REBOOT);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTREBOOT_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        SCB_DisableDCache();
        u32 InitCpu_Addr;
        InitCpu_Addr = *(u32*)0x00000004;
        ((void (*)(void))(InitCpu_Addr))();
    }
    return;
}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void reset(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_SRST);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRESET_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
    }
    return;
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void restart_system(u32 key)
{
    vu32 *addr;
    vu32 value;

    if(key != CN_BOOT_LEGALKEY)
    {
        printf("IllegalKey:0x%08x Recorded\n\r",key);
        ThrowOsBootInfo(EN_BOOT_RELOAD);
    }
    else
    {
        addr = (vu32 *)&pg_IapVar.IbootFlag[12];
        value = CN_BOOT_SOFTRELOAD_FLAG;
        *addr = value;
        value = *addr;
        Djy_DelayUs(10);
        __set_PSP((uint32_t)msp_top);
        __set_MSP((uint32_t)msp_top);
        Load_Preload();
    }
    return;
}

enBootMode GetBootMethodSoft(void)
{
    vu32 *addr;
    vu32 value;
    enBootMode result;
    addr = (u32*)&pg_IapVar.IbootFlag[12];
    value = *addr;
    if(value == CN_BOOT_SOFTRESET_FLAG)
    {
        result = EN_BOOT_SRST;
    }
    else if(value == CN_BOOT_SOFTREBOOT_FLAG)
    {
        result = EN_BOOT_REBOOT;
    }
    else if(value == CN_BOOT_SOFTRELOAD_FLAG)
    {
        result = EN_BOOT_RELOAD;
    }
    else if(value == CN_BOOT_HARDRST_FLAG)
    {
        result = EN_BOOT_HRST;
    }
    else
    {
        result = EN_BOOT_POWERDOWN;
    }
    value = CN_BOOT_HARDRST_FLAG;
    *addr = value;
    return result;
}
