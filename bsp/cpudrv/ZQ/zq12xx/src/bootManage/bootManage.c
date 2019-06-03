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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
/*
 * M0 ������CK �˻���DSP��
 *
 *  Created on: 2014-5-28
 *      Author: huzb
 */
#ifdef CFG_CPU_ZQ12XX_M0
#include "silan_types.h"
#include "silan_syscfg.h"
#include "silan_syscfg_regs.h"
#include "int_hard.h"
#include "shell.h"



/*boot  risc infomation*/

#define CN_RISC_RESET_ADDR    (0x400)          //risc reset addr
#define CN_RISC_FLASH_ADDR    (0x100000)       //risc load  addr in flash
#define CN_RISC_CODE_LEN      (0x80000)        //risc code  len 512k
#define CN_RISC_RUN_ADDR      (0x02160100)     //risc running addr

/*boot  dsp information*/

//0x100000 + 0x40000 = 0x140000
#define CN_DSP_LOAD_ADDR      (CN_RISC_FLASH_ADDR + CN_RISC_CODE_LEN)       //put dsp code here
#define CN_DSP_CODE_LEN       (0x80000)        //code of dsp
#define CN_DSP_RUN_ADDR       (0x02000000)     //dsp run in sdram

enum EN_DspBootMedia
{
    EN_BOOT_FROM_SDRAM = 0x11,
    EN_BOOT_FROM,
};


// ============================================================================
// ����:copy
// ������
// ���أ�
// ��ע�� 
// ============================================================================
void codesCopy(u8* dst,u8* scr,u32 len)
{
    u32 tmpLen   = 0;
    u8  *optDst  = NULL;
    u8  *optScr  = NULL;
    
    if(dst == NULL || scr == NULL || len == 0)
    {
        printf("arg err\r\n");
        return;
    }
    else
    {
        tmpLen = len;
        optDst = dst;
        optScr = scr;
    }
    
    while(tmpLen)
    {
    	*optDst++ = *optScr++;
    	tmpLen--;
    }
    printk("code Copy Over\r\n");
    
    return;
}

// ============================================================================
// ���ܣ�M0 ������Risc ��
// ������
// ���أ�
// ��ע�� 
// ============================================================================

void M0BootRisc(void)
{
    int  inst;
    int  tmpSts;
    u32  *resetAddr = NULL;
    
   // silan_risc_reset();
   // __REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
    
    //codesCopy((u8*)CN_RISC_RUN_ADDR,(u8*)CN_RISC_FLASH_ADDR,CN_RISC_CODE_LEN);
    memcpy((uint8_t *) (CN_RISC_RUN_ADDR & 0xFFFFF000), (char *)CN_RISC_FLASH_ADDR, CN_RISC_CODE_LEN);

    inst    = (*(volatile u32*)(CN_RISC_RESET_ADDR));
    tmpSts  = (inst & 0xFF000000) >> 24;
    
    u8 iLoop;
    for(iLoop = 0; iLoop < 10;iLoop++)
    printk("risc isbooting ,please wait...\r\n");

    if( (tmpSts == 0x22) || (tmpSts == 0x2))
    {
        printk("risc boot success\r\n");
    }
    else
    {
    	printk("risc boot failed\r\n");
    	return;
    }
    
//    silan_risc_unreset();
    __REG32(SILAN_SYSCFG_REG11) |= (0x1);

}


// ============================================================================
// ���ܣ�M0 ������Dsp��
// ������
// ���أ�
// ��ע�� 
// ============================================================================

void M0BootDsp(u32 bootMedia)
{
     volatile u32*dspLoadAddr = NULL;
     u32 inst;
     
     dspLoadAddr = (volatile u32*)CN_DSP_LOAD_ADDR;
     inst 	     = (*(volatile u32*)(dspLoadAddr));
     
     if(bootMedia == EN_BOOT_FROM_SDRAM)
     {
         if(inst == 0x0000AF02)
     	 {
             memcpy((uint8_t *)CN_DSP_RUN_ADDR, (char *)CN_DSP_LOAD_ADDR, CN_DSP_CODE_LEN);
     	 }
         else
     	 {
     	     printk("Dsp Boot Failed\r\n");
     	     return;
     	 }
     }
     
     if(bootMedia == EN_BOOT_FROM_SDRAM)
	__REG32(SILAN_SYSCFG_SOC0) |=  (0x1<<1);
     else
	__REG32(SILAN_SYSCFG_SOC0) &= ~(0x1<<1);

    inst =  __REG32(0x02000000);
		                               
    if(inst == 0x0000AF02)
    {
    	printk("dsp boot successs\r\n");
    }
    else
    {
	    printk("dsp boot failed\r\n");
	    return;
    }


    //silan_dsp_unreset();
    __REG32(SILAN_SYSCFG_REG11) |= (0x1<<2);
     
}

// ============================================================================
// ���ܣ�M0 ������Risc��
// ������
// ���أ�
// ��ע�� 
// ============================================================================

void M0RebootRisc(void)
{
    silan_risc_reboot();
}

// ============================================================================
// ���ܣ�M0 ������Dsp��
// ������
// ���أ�
// ��ע�� 
// ============================================================================

void M0RebootDsp(void)
{
    silan_dsp_reboot();
}

// ============================================================================
// ���ܣ��ж��Ƿ������ݣ�������������������
// ����������ǰ��1024k�ֽ�
// ���أ�
// ��ע��
// ============================================================================

typedef enum BOOT_METHOD{
   EN_BOOT_RISC = 0x88,
   EN_BOOT_DSP,
};
#include "stddef.h"

bool_t isToBoot(u8 Method)
{
    u16 loop,ZeroCount = 0,FCount = 0;
    u8  dat;
    u32 addr;
    bool_t ret = false;

    switch(Method)
    {
        case EN_BOOT_RISC:
            addr = CN_RISC_FLASH_ADDR;
            break;
        case EN_BOOT_DSP:
            addr = CN_DSP_LOAD_ADDR;
            break;
        default:printf("no this Boot Meathod\r\n");break;
    }

    //���1024�ֽ�
    for(loop = 0;loop < 1024;loop++)
    {
        dat = *((u8*)(addr+loop));
        if(dat == 0)
        {
            ZeroCount++;
        }else if(dat == 0xff)
        {
            FCount++;
        }
    }

    if(ZeroCount == 1024 || FCount == 1024)
         ret = false;
    else ret = true;

    return ret;
}


//����M0 �Լ�CK

void BootManage(void)
{
    u32 dwBoot_Delay = 800;

    while(dwBoot_Delay--);

    if(isToBoot(EN_BOOT_RISC))
    {
        M0BootRisc();
    }

    if(isToBoot(EN_BOOT_DSP))
    {
        M0BootDsp(0x11);
    }

}

//shell ����Risc


bool_t RunRisc(char *Param)
{
    if(isToBoot(EN_BOOT_RISC))
    {
         M0BootRisc();
    }
}

//shell ����Dsp


bool_t RunDsp(char *Param)
{
    if(isToBoot(EN_BOOT_DSP))
    {
        M0BootDsp(0x11);
    }
}

//Reboot Risc


bool_t RebootRisc(char *Param)
{
    M0RebootRisc();
}

//Reboot Dsp


bool_t ReBootDsp(char *Param)
{
    M0RebootDsp();
}

//��λ Risc


bool_t ResetRisc(char *Param)
{
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
}


//��λDsp


bool_t ResetDsp(char *Param)
{
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<2);
}


//shell ���ӵ��Կ���

#define Reg11CtrCfg    *((volatile u32*)(0x42010000 + 0x2C))

//�ر�M0����

bool_t closem0debug(char *Param)
{
    Reg11CtrCfg &= ~(1<<20);
}

//��M0����

bool_t openm0debug(char *Param)
{
    Reg11CtrCfg |= (1<<20);
}

//�ر�ck ����

bool_t closeriscdebug(char *Param)
{
    Reg11CtrCfg &= ~(1<<21);
}

//��risc ����

bool_t openriscdebug(char *Param)
{
    Reg11CtrCfg |= (1<<21);
}

//�ر�dsp ����

bool_t closedspdebug(char *Param)
{
    Reg11CtrCfg &= ~(1<<22);
}


//��Dsp ����

bool_t opendspdebug(char *Param)
{
    Reg11CtrCfg |= (1<<22);
}
ADD_TO_ROUTINE_SHELL(opendspdebug,opendspdebug,"��riscDebug    �����ʽ: opendspdebug");
ADD_TO_ROUTINE_SHELL(RunRisc,RunRisc,"����Risc    �����ʽ: RunRisc");
ADD_TO_ROUTINE_SHELL(RunDsp,RunDsp,"����Dsp    �����ʽ: RunDsp");
ADD_TO_ROUTINE_SHELL(RebootRisc,RebootRisc,"RebootRisc    �����ʽ: RebootRisc");
ADD_TO_ROUTINE_SHELL(ReBootDsp,ReBootDsp,"ReBootDsp    �����ʽ: ReBootDsp");
ADD_TO_ROUTINE_SHELL(ResetRisc,ResetRisc,"ResetRisc    �����ʽ: ResetRisc");
ADD_TO_ROUTINE_SHELL(ResetDsp,ResetDsp,"ResetDsp    �����ʽ: ResetDsp");
ADD_TO_ROUTINE_SHELL(closem0debug,closem0debug,"�ر�M0Debug    �����ʽ: closem0debug");
ADD_TO_ROUTINE_SHELL(closeriscdebug,closeriscdebug,"�ر�RiscDebug    �����ʽ: closeriscdebug");
ADD_TO_ROUTINE_SHELL(openriscdebug,openriscdebug,"��riscDebug    �����ʽ: openriscdebug");
ADD_TO_ROUTINE_SHELL(closedspdebug,closedspdebug,"�ر�DspDebug    �����ʽ: closedspdebug");
ADD_TO_ROUTINE_SHELL(openm0debug,openm0debug,"��M0Debug    �����ʽ: openm0debug");

#endif


































































































































































































































