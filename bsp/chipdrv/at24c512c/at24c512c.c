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
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

// 文件名     ：at24c512C.c
// 模块描述: EEPROM芯片驱动，调用了IICBUS总线驱动模型
// 模块版本:
// 创建人员: YT
// 创建时间: 2018/5/17
// =============================================================================

#include "stdint.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern bool_t AT24_ModuleInit(void);
//    AT24_ModuleInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"EEPROM at24c512c"//iic接口的eeprom
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus","lock","cpu oncip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_EEPROM_AT24C512C == false )
//#warning  " EEPROM_at24c512c  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_EEPROM_AT24C512C    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,0xFFFFFFFF,
#define CFG_AT24_TIMEOUT           (-1)   //"超时时间",-1表示无穷
//%$#@enum,0xA0,0xA2,0xA4,0xA6,0xA8,0xAC,0xAA,0xAE
#define CFG_AT24_ADDRESS            0xA0                //"设备地址",硬件配置AT24的IIC设备地址
//%$#@enum,100000,400000
#define CFG_AT24_CLK_FRE           (100*1000)           //"总线速度",单位Hz
//%$#@string,1,10,
#define CFG_AT24C512_BUS_NAME              "I2C0"       //"name",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
#define CN_AT24_CHIP_SIZE       (64*1024)           //芯片大小64K Bytes
#define CN_AT24_PAGE_SIZE       (64)                //芯片页大小8 Bytes
#define CN_AT24_PAGE_SUM        (CN_AT24_CHIP_SIZE/CN_AT24_PAGE_SIZE)

//#define CFG_AT24_ADDRESS           0x50            //设备地址
//#define CFG_AT24_CLK_FRE           (100*1000)      //总线速度，单位Hz
//#define CFG_AT24_TIMEOUT           CN_TIMEOUT_FOREVER

static struct SemaphoreLCB AT24_Semp;   //芯片互斥访问保护

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *s_ptAT24_Dev = NULL;
static u32 s_AT24_Timeout = CFG_AT24_TIMEOUT;

// =============================================================================
// 功能：AT24芯片WP写保护引脚初始化，配置为低时，允许写，为高时，写保护
// 参数：无
// 返回：无
// =============================================================================
void __AT24_GpioInit(void)
{
    //AT24C的WP引脚配置为低，允许写
//    extern void Board_AT24GpioInit(void);
//    Board_AT24GpioInit();
    //已经在board.c里面Board_GpioInit完成
}

__attribute__((weak))  void AT24_WPEn(u8 En)
{

}

// =============================================================================
// 功能：AT24芯片按页写,地址范围只局限在该页内
// 参数：wAddr:操作芯片的片内地址
//       pbyBuf:缓冲区
//       wBytesNum:数据字节数
// 返回：true,成功;false,失败
// =============================================================================
u32 __AT24_PageProgram(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 DelayMs;

    if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
        return 0;

    IIC_Write(s_ptAT24_Dev,wAddr,pbyBuf,wBytesNum,true,s_AT24_Timeout);

    //AT24内部写时间，最大为5mS
    DelayMs = 1000 / CFG_AT24_CLK_FRE;
    if(DelayMs >= 6)
        DelayMs = DelayMs + 1;
    else
        DelayMs = 6;

    DJY_EventDelay(DelayMs*mS);

    return wBytesNum;
}

// =============================================================================
// 功能：AT24芯片按页读,地址范围只局限在该页内
// 参数：wAddr:操作芯片的片内地址
//       pbyBuf:缓冲区
//       wBytesNum:数据字节数
// 返回：true,成功;false,失败
// =============================================================================
u32 __AT24_PageRead(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 DelayMs;

    if(((wAddr%CN_AT24_PAGE_SIZE) + wBytesNum) > CN_AT24_PAGE_SIZE)
        return 0;

    IIC_Read(s_ptAT24_Dev,wAddr,pbyBuf,wBytesNum,s_AT24_Timeout);

    //AT24内部写时间，最大为5mS
    DelayMs = 1000 / CFG_AT24_CLK_FRE;
    if(DelayMs >= 6)
        DelayMs = DelayMs + 1;
    else
        DelayMs = 6;

    DJY_EventDelay(DelayMs*mS);

    return wBytesNum;
}

// =============================================================================
// 功能：AT24芯片写操作，EEPROM按页操作，每次读写不能超过页大小
// 参数：wAddr:操作芯片的片内地址
//       pbyBuf:缓冲区
//       wBytesNum:数据字节数
// 返回：true,成功;false,失败
// =============================================================================
u32 AT24_WriteBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u32 offset = 0,wSize = 0;

    //判断是否超出芯片地址范围
    if((wAddr + wBytesNum-1) >= CN_AT24_CHIP_SIZE)
        return false;

    if(false == Lock_SempPend(&AT24_Semp,2500*mS))// TODO --- 10*mS
    {
        return false;
    }
    AT24_WPEn(0);
    //分页操作
    while(wBytesNum)
    {
        offset = wAddr % CN_AT24_PAGE_SIZE;             //本页偏移字节数
        wSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?
                (CN_AT24_PAGE_SIZE - offset): wBytesNum;//本页写字节数
        __AT24_PageProgram(wAddr,pbyBuf,wSize);         //写本页
        wAddr += wSize;                                 //写地址增加
        pbyBuf += wSize;                                //缓冲区增加
        wBytesNum -= wSize;                             //写长度递减
    }
    AT24_WPEn(1);
    Lock_SempPost(&AT24_Semp);

    return true;
}

// =============================================================================
// 功能：AT24芯片读操作，EEPROM按页操作，每次读写不能超过页大小
// 参数：wAddr:操作芯片的片内地址
//       pbyBuf:缓冲区
//       wBytesNum:数据字节数
// 返回：true,成功;false,失败
// =============================================================================
u32 AT24_ReadBytes(u32 wAddr, u8 *pbyBuf, u32 wBytesNum)
{
    u16 offset = 0,rSize = 0;

    //判断是否超出芯片地址范围
    if((wAddr + wBytesNum-1) >= CN_AT24_CHIP_SIZE)
        return false;

    if(false == Lock_SempPend(&AT24_Semp,2500*mS))//TODO
    {
        return false;
    }

    //分页操作
    while(wBytesNum)
    {
        offset = wAddr % CN_AT24_PAGE_SIZE;             //本页偏移字节数
        rSize = (wBytesNum > CN_AT24_PAGE_SIZE - offset)?
                (CN_AT24_PAGE_SIZE - offset): wBytesNum;//本页读字节数
        __AT24_PageRead(wAddr,pbyBuf,rSize);            //读本页
        wAddr += rSize;                                 //读地址增加
        pbyBuf += rSize;                                //缓冲区增加
        wBytesNum -= rSize;                             //读长度递减
    }
    Lock_SempPost(&AT24_Semp);
    return true;
}

// =============================================================================
// 功能：写字到指定地址
// 参数：wAddr,地址
//       wValue,数值，16比特数值
// 返回：true,成功;false,失败
// =============================================================================
u16 AT24_WriteWord(u32 wAddr,u16 wValue)
{
    u8 pbyBuf[2];
    pbyBuf[0]=wValue;
    pbyBuf[1]=wValue>>8;
    if(true == AT24_WriteBytes(wAddr,pbyBuf,2))
        return true;
    else
        return false;
}

// =============================================================================
// 功能：读字
// 参数：wAddr,地址
// 返回：wValue,成功;-1,失败
// =============================================================================
u16 AT24_ReadWord(u32 wAddr)
{
    u8 pbyBuf[2];
    u16 wValue;

    if(true == AT24_ReadBytes(wAddr, pbyBuf,2))
    {
        wValue=pbyBuf[0]+(pbyBuf[1]<<8);
        return wValue;
    }
    else
        return -1;
}

// =============================================================================
// 功能：AT24芯片初始化，包括WP写保护功能引脚初始化和加载设备到对应的IIC总线
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t AT24_ModuleInit(void)
{
    bool_t result = false;
    //GPIO初始化，SDA、SCL已经在IIC中初始化了，此处只需初始化WP即可
    __AT24_GpioInit();

    if(NULL == Lock_SempCreate_s(&AT24_Semp,1,1,CN_BLOCK_FIFO,"AT24 Semp"))
        return false;

    //添加AT24到IIC0总线
    s_ptAT24_Dev = IIC_DevAdd(CFG_AT24C512_BUS_NAME,"AT24C512C",CFG_AT24_ADDRESS, 0, 16);
    if(s_ptAT24_Dev)
    {
        IIC_BusCtrl(s_ptAT24_Dev,CN_IIC_SET_CLK,CFG_AT24_CLK_FRE,0);
        IIC_BusCtrl(s_ptAT24_Dev,CN_IIC_SET_POLL,0,0);

//        IIC_BusCtrl(ps_FM24CL64_Dev,CN_IIC_DMA_USED,0,0);
        result = true;
    }
    else
    {
        Lock_SempDelete_s(&AT24_Semp);
        result = false;
    }

    return result;
}
