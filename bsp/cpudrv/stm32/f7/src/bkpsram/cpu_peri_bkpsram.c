//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然:
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明:本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：brdexp.c
// 模块描述: 异常模块存储与解读，将AT45的最好1M配置为异常存储
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 12/03.2016
// =============================================================================
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <os.h>
#include <stm32f7xx_hal.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t BkpSramInit(void);
//    BkpSramInit( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip Backup area sram"//电池保持的4K片内ram读写
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM == false )
//#warning  " cpu_onchip_Backup_area_sram  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure


//BKPSRAM:
//1,in stm32f7xx,addr：0X40024000 size：4KB
//2,will retain its data only if the BACKUP POWER IS ON

#define CN_BKPSRAM_SIZE    0X1000               //4KB
#define CN_BKPSRAM_ADDR    ((u8 *)0X40024000)   //BKPSRAM

//enable the access of bkpsram
static void __BkpsramEnable(void)
{
    HAL_PWR_EnableBkUpAccess();
}

static void __BkpsramDisable(void)
{
    HAL_PWR_DisableBkUpAccess();
}
//first to make the bkpsram to could be used
bool_t BkpSramInit(void)
{
    //step 1:Enable the PWR clock
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_SLEEP_ENABLE();

    //step 2:Enable access to the backup domain
    HAL_PWR_EnableBkUpAccess();

    //step 3:Enable backup SRAM Clock
    __HAL_RCC_BKPSRAM_CLK_ENABLE();
    __HAL_RCC_BKPSRAM_CLK_SLEEP_ENABLE();
    //step 4:Enable the Backup SRAM low power Regulator to retain it's content in VBAT mode
    HAL_PWREx_EnableBkUpReg();

    //step 5:check the vbat is OK
    int timeout =100;
    while((!(__HAL_PWR_GET_FLAG(PWR_FLAG_BRR)))&&(timeout > 0))
    {
        timeout--;
        DJY_DelayUs(1*mS);
    }
    if(timeout <=0)
    {
        printf("bkpsram init timeout:vbat ready wait timeout\n\r");
    }
    //step 6:disable the access here to protect the sram
    __BkpsramDisable();
    return true;
}

//use the  enable and disable access to protect the bkpsram
bool_t BkpSramRead(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if((offset < CN_BKPSRAM_SIZE)&&(len <=CN_BKPSRAM_SIZE)&&((offset + len) <= CN_BKPSRAM_SIZE))
    {
        __BkpsramEnable();
        memcpy(buf,CN_BKPSRAM_ADDR+offset,len);
        __BkpsramDisable();
        ret = true;
    }
    return ret;
}
//write the bkpsram len bytes to addr from buf
bool_t BkpSramWrite(u16 offset,void *buf,u16 len)
{
    bool_t ret = false;
    if((offset < CN_BKPSRAM_SIZE)&&(len <=CN_BKPSRAM_SIZE)&&((offset + len) <= CN_BKPSRAM_SIZE))
    {
        __BkpsramEnable();
        memcpy(CN_BKPSRAM_ADDR+offset,buf,len);
        __BkpsramDisable();
    }
    return ret;
}
//zero bkpsram
bool_t  BkpSramFormat(u16 offset,u16 len)
{
    __BkpsramEnable();
    memset(CN_BKPSRAM_ADDR+offset,0,len);
    __BkpsramDisable();
    return true;
}

u32 BkpSramSize(void)
{
    return CN_BKPSRAM_SIZE;
}



