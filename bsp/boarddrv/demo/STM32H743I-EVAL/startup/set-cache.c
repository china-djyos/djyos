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
// Copyright (c) 2018，著作权由都江堰操作系统�?源开发团队所有�?�著作权人保留一切权利�??
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然�?
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明�?
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或�?�其他附
//    于散播包装中的媒介方式，重制上述之版权宣告�?�本条件列表，以及下�?
//    的免责声明�??

// 免责声明：本软件是本软件版权持有人以及贡献�?�以现状�?"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就�?�售性以及特定目
// 的的适用性为默示性担保�?�版权持有人及本软件之贡献�?�，无论任何条件�?
// 无论成因或任何责任主义�?�无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生�?
// 任何直接性�?�间接�?��?�偶发�?��?�特殊�?��?�惩罚�?�或任何结果的损害（包括但不�?
// 于替代商品或劳务之购用�?�使用损失�?�资料损失�?�利益损失�?�业务中断等等）�?
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然�?
// -----------------------------------------------------------------------------
// =============================================================================

// 文件�?     ：sysinit.c
// 模块描述: CPU时钟的初始化和片内片外RAM等的初始�?
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 08/13.2015s
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "arch_feature.h"


#ifndef __MPU_PRESENT
#define __MPU_PRESENT  1
#endif

#ifndef K
#define K  1024
#endif

#ifndef M
#define M  1024*1024
#endif

#ifndef   HAL_CORTEX_MODULE_ENABLED
#define   HAL_CORTEX_MODULE_ENABLED
#endif

#include "stm32h7xx_hal_cortex.h"

#define memory_sync()        __DSB();__ISB();


extern uint32_t gc_ptCacheRam1Size;
extern uint32_t gc_ptCacheRam2Size;
extern uint32_t gc_ptCacheRam3Size;
extern uint32_t gc_ptCacheRam4Size;
extern uint32_t gc_ptCacheRam5Size;
extern uint32_t gc_ptNoCacheRamSize;


void MPU_Disable(void)
{
    SCB->SHCSR&=~(1<<16);   //禁止MemManage
    MPU->CTRL&=~(1<<0);     //禁止MPU
}


void MPU_Enable(void)
{
  MPU->CTRL=(1<<2)|(1<<0);  //使能PRIVDEFENA,使能MPU
  SCB->SHCSR|=1<<16;        //使能MemManage
}


u8 MPU_Convert_Bytes_To_POT(u32 nbytes)
{
    u8 count=0;
    while(nbytes!=1)
    {
        nbytes>>=1;
        count++;
    }
    return count;
}



static u8 Bytes_To_POT(u32 size)
{
    u8 pot=0;
    while(size!=1)
    {
        size>>=1;
        pot++;
    }
    return pot;
}
//                   基址                                大小                    编号                 模式                      指令          共用             cache   缓冲
bool_t MPU_Set(u32 baseaddr,u32 size,u8 rnum,u8 mode,u8 Exec,u8 sen,u8 cen,u8 ben)
{

    u32 tempreg=0;
    u8 rnr=0;
    if((size%32)||size==0) //大小不是32的�?�数,或�?�size�?0,说明参数错误
        return false;
    rnr=Bytes_To_POT(size)-1;       //转换�?2为底的指数�??
    MPU_Disable();                  //设置之前,先禁止MPU保护
    MPU->RNR=rnum;                  //设置保护区域
    MPU->RBAR=baseaddr;             //设置基址
    if(Exec==0)
        tempreg|=1<<28;             //不允许指令访�?
    tempreg|=((u32)mode)<<24;       //设置访问权限,
    tempreg|=0<<19;                 //设置类型扩展域为level0
    tempreg|=((u32)sen)<<18;        //是否允许共用
    tempreg|=((u32)cen)<<17;        //是否允许cache
    tempreg|=((u32)ben)<<16;        //是否允许缓冲
    tempreg|=0<<8;                  //禁止子区�?
    tempreg|=rnr<<1;                //设置保护区域大小
    tempreg|=1<<0;                  //使能该保护区�?
    MPU->RASR=tempreg;              //设置RASR寄存�?
    MPU_Enable();                   //设置完毕,使能MPU保护
    return true;
}
// =============================================================================
// 功能：配置MPU，主要是配置了cache的区域，具体配置信息如下�?
// 注：
//       MPU_REGION_NO_ACCESS,无访问（特权&用户都不可访问）
//       MPU_REGION_PRIV_RW,仅支持特权读写访�?
//       MPU_REGION_PRIV_RW_URO,禁止用户写访问（特权可读写访问）
//       MPU_REGION_FULL_ACCESS,全访问（特权&用户都可访问�?
//       MPU_REGION_PRIV_RO,仅支持特权读访问
//       MPU_REGION_PRIV_RO_URO,只读（特�?&用户都不可以写）
// 返回：无
// =============================================================================

void Cache_config( void )
{

/*=============================================================================
    0x68000000  64M  外部RAM  如：LCD
=============================================================================*/
    MPU_Set(0x68000000,2*1024*1024,
            0,MPU_REGION_FULL_ACCESS,1,0,1,1);
/*=============================================================================
    0x20000000      内部SRAM
=============================================================================*/
    //cashe �?
    MPU_Set(0x24000000,gc_ptCacheRam1Size,
            1,MPU_REGION_FULL_ACCESS,1,0,1,1);
    //cashe �?
    MPU_Set(0x24000000+gc_ptCacheRam1Size,gc_ptCacheRam2Size,
            2,MPU_REGION_FULL_ACCESS,1,0,1,1);
    //非cashe �?
    MPU_Set(0x24000000+gc_ptCacheRam1Size+gc_ptCacheRam2Size,gc_ptNoCacheRamSize,
            3,MPU_REGION_FULL_ACCESS,1,0,0,1);
/*=============================================================================
    0XC0000000   32M SDRAM
=============================================================================*/
    MPU_Set(0xD0000000, 32*1024*1024,
            4,MPU_REGION_FULL_ACCESS,1,0,1,1);
/*=============================================================================
    0X80000000  NAND FLASH   stm32 有art
=============================================================================*/
    MPU_Set(0X80000000,256*1024*1024,
            5,MPU_REGION_FULL_ACCESS,1,0,1,1);

}














