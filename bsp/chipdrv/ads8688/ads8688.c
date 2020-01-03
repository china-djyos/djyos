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
// =============================================================================

// 文件名     ：ADS8668.c
// 模块描述: ADS8688采集模块的底层驱动，调用了SPI接口通信函数读取AD采集值
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 05/10.2017
// =============================================================================
#include <harddrv_ad.h>     //


//#define SPI_BASE         SPI2_BASE
//s32 SPI_Read(void);
//s32 SPI_Write(uint32_t dwNpcs, uint16_t wData );
//bool_t AD_HardInit(uint8_t spi_port);
//bool_t AD_GetCPChannel(uint8_t num);
//bool_t AD_GetALLChannel(uint16_t *pAinBuf);

#include "stdint.h"
#include "stddef.h"
#include "endian.h"
#include "djyos.h"
#include "cpu_peri.h"
#include "shell.h"
#include "ads8688.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void ADS8688_SpiInit(uint8_t port);
//    ADS8688_SpiInit(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ADC chip ads 8688"//spi接口的16bitADC
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"sr5333_board"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ADC_CHIP_ADS_8688 == false )
//#warning  " ADC_chip_ads_8688  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ADC_CHIP_ADS_8688    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,0,1,2,3
#define CFG_ADS8688_CS        0//"片选",片选号
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure



extern const Pin ADCS[];
// =============================================================================
//#define CFG_ADS8688_CS           (0)
#define WRITE                 1
#define READ                  0

extern s32 SPI_Read(void);
extern s32 SPI_Write(uint32_t dwNpcs, uint16_t wData);

// =============================================================================
// 功能：ADS8688对应的SPI控制器初始化
// 参数：port, SPI控制器编号，在sr5333-v2.0中ADS8688采用的是SPI2;
// 返回：空.
// =============================================================================
void ADS8688_SpiInit(uint8_t port)
{
    //已经在board.c里面初始化了SPI2的驱动
     Spi *Reg;
     Reg = (Spi *)SPI_BASE;
     //配置SPI使用GPIO引脚
     RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; // 使能SPI2
     RCC->APB1RSTR |= RCC_APB1RSTR_SPI2RST; // 复位SPI2
     RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST; // 停止复位SPI2

     Reg->CR1 |=SPI_CR1_BR_0;//波特率设置  108M/4=27MHz
     Reg->CR1 &= ~SPI_CR1_CPOL;                      // 空闲模式下SCK为1 CPOL=0
     Reg->CR1 |= SPI_CR1_CPHA;                      // 数据采样从第2个时间边沿开始,CPHA=1
     Reg->CR1 &= ~SPI_CR1_RXONLY;//全双工通讯模式
     Reg->CR1 &= ~SPI_CR1_LSBFIRST;//数据帧格式
     Reg->CR1 &= ~ SPI_CR1_SSM;         // 禁止软件nss管理
     Reg->CR1 |= SPI_CR1_SSI;  //SSM
     Reg->CR1 |= SPI_CR1_MSTR;       // SPI主机

     Reg->CR2 |=SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2|SPI_CR2_DS_3 ;//数据帧格式为16bit
     Reg->CR2 |=SPI_CR2_SSOE;//SS 输出使能
     Reg->CR2 |=SPI_CR2_NSSP;//生成 NSS 脉冲
     Reg->CR2 &=~SPI_CR2_FRXTH;//16字节fifo
     Reg->I2SCFGR &= (uint16_t)(~SPI_I2SCFGR_I2SMOD);// 选择SPI模式
     Reg->CR1 |= SPI_CR1_SPE;
}

// =============================================================================
// 功能：ADS8688对应的SPI控制器初始化
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t __ADS8688_SpiTrans(uint16_t *wBuf, uint32_t wlen, uint16_t *rBuf,uint32_t rlen)
{
    uint32_t i;
    s32 ret;
    bool_t result = false;
    Spi *Reg;
    Reg = (Spi *)SPI_BASE;
    PIO_Clear(ADCS);
    for(i = 0; i < wlen; i++)
    {
        ret=SPI_Write(CFG_ADS8688_CS,wBuf[i]);
        if(ret==-1)
            goto SPI_EXIT;
        ret=SPI_Read();
        if(ret==-1)
            goto SPI_EXIT;
    }
    for(i = 0; i < rlen; i ++)
    {
        ret=SPI_Write(CFG_ADS8688_CS,0x0000);
        if(ret==-1)
            goto SPI_EXIT;
         ret = SPI_Read();
         if(ret==-1)
            goto SPI_EXIT;

         rBuf[i] = (uint16_t)(ret & 0xFFFF);
    }
    result = true;
SPI_EXIT:
    PIO_Set(ADCS);
    return result;
}

// =============================================================================
void ADS8688_WriteCmdReg(uint16_t command)//写ADS8688命令寄存器
{
    uint16_t tmp=0;
    __ADS8688_SpiTrans(&command,1,&tmp,1);
}

void ADS8688_EnterAutoRstMode(void)    //进入自动扫描模式
{
    ADS8688_WriteCmdReg(AUTO_RST);
}

void ADS8688_EnterRstMode(void)
{
    ADS8688_WriteCmdReg(RST);
}

// =============================================================================
// 功能：Program Register写操作
// 参数：addr, Program Register地址;
//       data，仅对写指令有效，为写进行寄存器的值.
// 返回：
// =============================================================================
static bool_t ADS8688_WRProgReg(uint8_t addr,uint8_t data)
{
    uint16_t w_dat=0x0000;
    uint16_t r_dat=0x0000;
    bool_t ret=true;
    uint8_t temp=0x00;
    temp=addr<<1;
    temp=temp|WRITE;
    w_dat=temp;
    w_dat=w_dat<<8;
    w_dat=w_dat|data;
    ret=__ADS8688_SpiTrans(&w_dat,1,&r_dat,1);
    if(ret==false)
        return ret;
    r_dat=r_dat>>8;
    if(r_dat!=data)
        return false;
    return true;
}
// =============================================================================
// 功能：设置通道输入范围
// 参数：seq
// 返回：
// =============================================================================
bool_t ADS8688_SetChInRange(uint8_t ch,uint8_t range)
{
    bool_t result;
    result=ADS8688_WRProgReg(ch,range);
    return result;
}


// =============================================================================
// 功能：读取扫描通道序列的AD转换数据
// 参数：seq
// 返回：
// =============================================================================
bool_t ADS8688_GetRstModeData(uint8_t seq,uint16_t *pdata,uint8_t chnum)
{
    uint8_t i;
    uint16_t bData = 0x0000;
    bool_t ret=true;
    uint16_t temp=0;
    ret=ADS8688_WRProgReg(AUTO_SEQ_EN,seq);
    if(!ret)
        return false;
    ADS8688_EnterRstMode();
    for (i = 0; i < chnum; i ++)
    {
        ret=__ADS8688_SpiTrans(&bData,1,pdata,1);
        if(ret==false)
            return ret;
        temp=*pdata;
        *pdata=temp;
        pdata++;
    }
    return true;
}

// =============================================================================
// 功能：读取手动通道的AD转换数据
// 参数：无。
// 返回：
// =============================================================================
uint16_t ADS8688_GetManModeData(uint16_t ch)
{
    uint16_t dat=0;
    bool_t ret=true;
    uint16_t bData = 0x0000;
    //设置指定通道为手动模式
    ADS8688_WriteCmdReg(ch);
    ret=__ADS8688_SpiTrans(&bData,1,&dat,1);
    if(ret==false)
        return 0;
    return dat;
}


