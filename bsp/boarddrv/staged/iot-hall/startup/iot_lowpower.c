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
//����ģ��: CPU��ص͹��Ĺ���
//����:  ������.
//�汾��V1.0.0
//�ļ�����: cpu��صĵ͹��Ĺ���,ʵ��lowpower.h�ļ��й涨bsp��Ҫ�ṩ�ĺ���,����
//          ʵ�ָ������ص�,��CPU��صĺ���,��cpudrvĿ¼����.
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "stm32f4xx.h"

//stm32�ϵĵ͹���Level����(�ο�lowpower.h�ļ��еĶ���)
//L0:��ִ��wfeָ��,����������.��L1һ��,������Ǵ�������˯��ģʽ.
//L1:wfeָ��,�ں�ִ�еĲ�����L0һ��,���û��Ļص���������ͣ������ʱ��
//L2:������stm32��ֹͣģʽ,����SRAM�ͼĴ���ֵ,�˳����������
//L3/L4:����stm32�Ĵ���ģʽ,SRAM�ͼĴ���ֵ����ʧֻ�б��ݼĴ���ά�ֹ���.��������
//�˳���,L3����ʱҪ����SRAM�ͼĴ���,�˳�ʱҪ�ָ������˳�������ִ��.

extern   uint32_t   msp_top[ ];
#define  CN_LP_STACK_SIZE  0x100

//64K��Ƭ��RAM������flash���洢����64k��Χ��
//���洢��Ϊ2K/Page
static void LP_RamFlashErase(void)
{
    u8 page;
    u32 addr;

    for(page = 255; page > 255-32; page--)
    {
        addr = page*0x800 + 0x08000000;
        FLASH_Unlock();
//      FLASH_ErasePage(addr);todo
    }
}

static u32 LP_RamFlashProgram(u32 addr,u32 *buf,u32 len)
{
    u32 i;
    FLASH_Unlock();
    for(i = 0; i < len; i++)
    {
        FLASH_ProgramWord(addr+4*i,buf[i]);
    }

    return len;
}

static u32 LP_RamFlashRead(u32 addr,u32 *buf,u32 len)
{
    u32 i,temp;

    for(i = 0; i < len; )
    {
        temp = *(u32*)(addr+4*i);
        buf[i] = temp;
        i++;
    }
    return len;
}
//----�ָ�RAM------------------------------------------------------------------
//����: �ӷ���ʧ�洢���лָ�RAM������,�Ա�ӽ���͹��Ĵ��������г���,������L3��
//      �͹���.������������֧��L3���͹���,��������ֱ�ӷ���false.
//����: ��
//����: true=�ɹ��ָ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_RestoreRamL3(void)
{
    u32 i,temp;
    u32 RamAddr,FlashAddr,len;

    //�ڲ�SRAM�ֳ�������
    //1.0x20000000 ~ msp_top-100 ,��ָ�
    //2.msp_top-100 ~ msp_top ,���ָܻ�����Ϊ��ʱ��flash��������ʹ��
    //3.msp_top - 0x20004000, ��ָ�
    RamAddr = 0x20000000;
    FlashAddr =  224*0x800 + 0x08000000;
    len = ((uint32_t)msp_top-CN_LP_STACK_SIZE - 0x20000000)/4;

    LP_RamFlashRead(FlashAddr,RamAddr,len);

    RamAddr = (uint32_t)msp_top;
    FlashAddr = FlashAddr +len*4 + CN_LP_STACK_SIZE;
    len = (64*1024 - ((uint32_t)msp_top - 0x20000000))/4;

    LP_RamFlashRead(FlashAddr,RamAddr,len);

    return true;
}

//----����RAM------------------------------------------------------------------
//����: ����L3���͹���ǰ,��RAM�����ݱ��浽����ʧ�洢����,�Ա��˳��͹���״̬ʱ,
//      �ָ��ڴ沢�ӳ����жϴ���������.������������֧��L3���͹���,��������
//      ֱ�ӷ���false.
//����: ��
//����: true=�ɹ��ָ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __LP_BSP_SaveRamL3(void)
{
    u32 i,RamAddr,FlashAddr,len;

    RamAddr = 0x20000000;
    FlashAddr =  224*0x800 + 0x08000000;
    len = 64*1024/4;

    LP_RamFlashErase();

    LP_RamFlashProgram(FlashAddr,(u32*)RamAddr,len);

   return true;
}


