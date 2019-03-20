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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下二条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
/*
 * M0 核启动CK 核或者DSP核
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
// 功能:copy
// 参数：
// 返回：
// 备注： 
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
// 功能：M0 核启动Risc 核
// 参数：
// 返回：
// 备注： 
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
// 功能：M0 核启动Dsp核
// 参数：
// 返回：
// 备注： 
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
// 功能：M0 核启动Risc核
// 参数：
// 返回：
// 备注： 
// ============================================================================

void M0RebootRisc(void)
{
    silan_risc_reboot();
}

// ============================================================================
// 功能：M0 核启动Dsp核
// 参数：
// 返回：
// 备注： 
// ============================================================================

void M0RebootDsp(void)
{
    silan_dsp_reboot();
}

// ============================================================================
// 功能：判断是否有数据，有则启动，无则不启动
// 参数：遍历前面1024k字节
// 返回：
// 备注：
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

    //检查1024字节
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


//启动M0 以及CK

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

//shell 运行Risc


bool_t RunRisc(char *Param)
{
    if(isToBoot(EN_BOOT_RISC))
    {
         M0BootRisc();
    }
}

//shell 运行Dsp


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

//复位 Risc


bool_t ResetRisc(char *Param)
{
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1);
}


//复位Dsp


bool_t ResetDsp(char *Param)
{
    __REG32(SILAN_SYSCFG_REG11) &= ~(0x1<<2);
}


//shell 增加调试控制

#define Reg11CtrCfg    *((volatile u32*)(0x42010000 + 0x2C))

//关闭M0调试

bool_t closem0debug(char *Param)
{
    Reg11CtrCfg &= ~(1<<20);
}

//打开M0调试

bool_t openm0debug(char *Param)
{
    Reg11CtrCfg |= (1<<20);
}

//关闭ck 调试

bool_t closeriscdebug(char *Param)
{
    Reg11CtrCfg &= ~(1<<21);
}

//打开risc 调试

bool_t openriscdebug(char *Param)
{
    Reg11CtrCfg |= (1<<21);
}

//关闭dsp 调试

bool_t closedspdebug(char *Param)
{
    Reg11CtrCfg &= ~(1<<22);
}


//打开Dsp 调试

bool_t opendspdebug(char *Param)
{
    Reg11CtrCfg |= (1<<22);
}
ADD_TO_ROUTINE_SHELL(opendspdebug,opendspdebug,"打开riscDebug    命令格式: opendspdebug");
ADD_TO_ROUTINE_SHELL(RunRisc,RunRisc,"运行Risc    命令格式: RunRisc");
ADD_TO_ROUTINE_SHELL(RunDsp,RunDsp,"运行Dsp    命令格式: RunDsp");
ADD_TO_ROUTINE_SHELL(RebootRisc,RebootRisc,"RebootRisc    命令格式: RebootRisc");
ADD_TO_ROUTINE_SHELL(ReBootDsp,ReBootDsp,"ReBootDsp    命令格式: ReBootDsp");
ADD_TO_ROUTINE_SHELL(ResetRisc,ResetRisc,"ResetRisc    命令格式: ResetRisc");
ADD_TO_ROUTINE_SHELL(ResetDsp,ResetDsp,"ResetDsp    命令格式: ResetDsp");
ADD_TO_ROUTINE_SHELL(closem0debug,closem0debug,"关闭M0Debug    命令格式: closem0debug");
ADD_TO_ROUTINE_SHELL(closeriscdebug,closeriscdebug,"关闭RiscDebug    命令格式: closeriscdebug");
ADD_TO_ROUTINE_SHELL(openriscdebug,openriscdebug,"打开riscDebug    命令格式: openriscdebug");
ADD_TO_ROUTINE_SHELL(closedspdebug,closedspdebug,"关闭DspDebug    命令格式: closedspdebug");
ADD_TO_ROUTINE_SHELL(openm0debug,openm0debug,"打开M0Debug    命令格式: openm0debug");

#endif


































































































































































































































