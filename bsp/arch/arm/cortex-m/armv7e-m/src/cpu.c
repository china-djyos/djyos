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
//����ģ��: ������
//���ߣ�lst
//�汾��V1.0.1
//�ļ�����: ����������CPUֱ����صĴ��롣
//����˵��:
//�޶���ʷ:
//3. ����: 2018-05-31
//   ����: chj
//   �°汾��: V1.0.2
//   �޸�˵��: ��reboot���������
//  InitCpu_Addr=*(u32*)0x00000004;�ĳ���InitCpu_Addr = (u32)Init_Cpu;
//  0x00000004��ĳЩCPU�Ͽ��ܻ����
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
#include "dbug.h"
#include <board-config.h>
#include "Iboot_info.h"

extern   uint32_t   msp_top[ ];
extern void __set_PSP(uint32_t topOfProcStack);
extern void __set_MSP(uint32_t topOfProcStack);
extern void Init_Cpu(void);
// void __start_systick(void);

///////////////////////////////////////////////djy-api end//////////////////////////////////

extern void Iboot_LoadPreload(void);
#include <blackbox.h>
#include <Iboot_Info.h>

#define CN_BOOT_SOFTREBOOT_FLAG         (CN_BOOT_LEGALKEY <<1)
#define CN_BOOT_SOFTRELOAD_FLAG         (CN_BOOT_LEGALKEY <<2)
#define CN_BOOT_HARDRST_FLAG            (CN_BOOT_LEGALKEY <<3)
#define CN_BOOT_SOFTRESET_FLAG          (CN_BOOT_LEGALKEY <<4)
typedef struct
{
    union
    {
        u32 value;
        struct
        {
            u32 procrst:1;
            u32 res3:2;
            u32 extrst:1;
            u32 res2:4;
            u32 res1:16;
            u32 key:8;
        }bits;
    }cr;
    union
    {
        u32 value;
        struct
        {
            u32 ursts:1;
            u32 res3:7;
            u32 rsttype:3;
            u32 res2:5;
            u32 nrstl:1;
            u32 srcmp:1;
            u32 res1:14;
        }bits;
    }sr;
    union
    {
        u32 value;
        struct
        {
            u32 ursten:1;
            u32 res3:3;
            u32 urstien:1;
            u32 res2:3;
            u32 erstl:4;
            u32 res1:12;
            u32 key:8;
        }bits;
    }mr;
}tagRstc;
// =============================================================================
// ���ܣ����е�ѡ��ϵͳ���з�ʽǰ������M3/M4��CPU����PC��ת��Init_CPU()
// ��������
// ���أ���
// =============================================================================
void CPU_Reboot(void)
{
    u32 InitCpu_Addr;
    Iboot_SetRebootFlag();
    Iboot_SetPreviouResetFlag();
    InitCpu_Addr = (u32)Init_Cpu;
    ((void (*)(void))(InitCpu_Addr))();

    return;
}
// =============================================================================
// ���ܣ�ResetӲ��CPU���൱���ϵ�����������Ӳ��������õ���λ
// ��������
// ���أ���
// =============================================================================
void CPU_Reset(void)
{
    Iboot_SetSoftResetFlag();
    Iboot_SetPreviouResetFlag();
    pg_scb_reg->AIRCR = (0x05FA << 16)|(0x01 << bo_scb_aircr_sysresetreq);
    return;
}
// =============================================================================
// ���ܣ����е�CPU���ش���ǰ����pre_load()ǰ
// ��������
// ���أ���
// =============================================================================
void CPU_RestartSystem(void)
{
    DJY_DelayUs(10);
    __set_PSP((uint32_t)msp_top);
    __set_MSP((uint32_t)msp_top);
    Iboot_LoadPreload();
    return;
}

enStartMode GetBootMethodSoft(void)
{
    enStartMode result;
    return result;
}
enum
{
    EN_RESET_GENERALRST = 0,
    EN_RESET_BACKUPRST,
    EN_RESET_WDTRST,
    EN_RESET_SOFTRST,
    EN_RESET_USERRST,
    EN_RESET_RES,
}enResetType;
enStartMode GetBootMethodHard(void)
{
    enStartMode result;
    tagRstc rstc;
    rstc.cr.value = *(vu32 *)(0x400e1800);
    rstc.sr.value = *(vu32 *)(0x400e1804);
    rstc.mr.value = *(vu32 *)(0x400e1808);
    switch(rstc.sr.bits.rsttype)
    {
        case EN_RESET_GENERALRST:
            printk("General RESET\n\r");
            result =EN_Start_POWERDOWN;
            break;
        case EN_RESET_BACKUPRST:
            printk("BackUp RESET\n\r");
            result =EN_Start_UNKNOWN;
            break;
        case EN_RESET_WDTRST:
            printk("WDT RESET\n\r");
            result =EN_Start_UNKNOWN;
            break;
        case EN_RESET_SOFTRST:
            printk("SOFT RESET\n\r");
            result =EN_Start_SRST;
            break;
        case EN_RESET_USERRST:
            printk("USER RESET\n\r");
            result =EN_Start_HRST;
            break;
        default:
            printk("UNKOWN MODE\n\r");
            result =EN_Start_UNKNOWN;
            break;
    }
    return result;
}
