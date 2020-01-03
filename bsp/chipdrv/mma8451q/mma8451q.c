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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// =============================================================================

// 文件名     ：MMA8451Q.c
// 模块描述: 加速度传感器驱动，采用的是IIC的接口
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 18/09.2014
// =============================================================================

#include "stdint.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include <mma8451q_config.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t MMA8541Q_ModuleInit(void);
//    MMA8541Q_ModuleInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"3 axis accelerometer mma8451q"//IIC接口的3轴加速度传感器
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","iicbus","cpu onchip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_3_AXIS_ACCELEROMETER_MMA8451Q == false )
//#warning  " 3_axis_accelerometer_mma8451q  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_3_AXIS_ACCELEROMETER_MMA8451Q    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
#define CFG_MMA_ADDRESS                0x50      //"设备地址",IIC总线设备地址
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_MMA_BUS_NAME              "IIC0"    //"总线名称",mma8451使用的IIC总线名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//寄存器定义（部分）
#define F_STATUS        0x00
#define OUT_X_MSB       0x01
#define OUT_X_LSB       0x02
#define OUT_Y_MSB       0x03
#define OUT_Y_LSB       0x04
#define OUT_Z_MSB       0x05
#define OUT_Z_LSB       0x06
#define WHO_AM_I        0x0D

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *ps_MMA_Dev = NULL;
static u32 s_MMA_Timeout = CN_TIMEOUT_FOREVER;

//#define CFG_MMA_ADDRESS         0x50            //设备地址
#define MMA_CLK_FRE         (100*1000)      //总线速度，单位Hz

// =============================================================================
// 功能：读加速度器的寄存器（内部调用）
// 参数：RegAddr,寄存器地址（8位）
//       pbyBuf,读取结果存储空间
//       wBytesNum,读取数据的长度
// 返回：true，读取成功;false，读取失败
// =============================================================================
bool_t __MMA8541Q_ReadReg(u8 RegAddr, u8 *pbyBuf, u16 wBytesNum)
{
    if(wBytesNum == IIC_Read(ps_MMA_Dev,RegAddr,pbyBuf,wBytesNum,s_MMA_Timeout))
        return true;
    else
        return false;
}

// =============================================================================
// 功能：读加速度器的ID
// 参数：无
// 返回：读取结果
// =============================================================================
u8 MMA8541Q_ID_Read(void)
{
    u8 pbyBuf,RegAddr,wBytesNum;

    pbyBuf = 0;
    RegAddr = WHO_AM_I;
    wBytesNum = 1;

    if(true == __MMA8541Q_ReadReg(RegAddr,&pbyBuf,wBytesNum))
        return pbyBuf;
    else
        return 0;
}

// =============================================================================
// 功能：读加速度器的状态寄存器
// 参数：无
// 返回：读取结果
// =============================================================================
u8 MMA8541Q_StatusRead(void)
{
    u8 pbyBuf,RegAddr,wBytesNum;

    pbyBuf = 0;
    RegAddr = F_STATUS;
    wBytesNum = 1;

    if(true == __MMA8541Q_ReadReg(RegAddr,&pbyBuf,wBytesNum))
        return pbyBuf;
    else
        return 0;
}

// =============================================================================
// 功能：读加速度器的X轴方向值
// 参数：无
// 返回：读取结果
// =============================================================================
u16 MMA8541Q_X_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_X_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}

// =============================================================================
// 功能：读加速度器的Y轴方向值
// 参数：无
// 返回：读取结果
// =============================================================================
u16 MMA8541Q_Y_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_Y_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}
// =============================================================================
// 功能：读加速度器的Z轴方向值
// 参数：无
// 返回：读取结果
// =============================================================================
u16 MMA8541Q_Z_Read(void)
{
    u8 pbyBuf[2],RegAddr,wBytesNum;

    pbyBuf[0] = 0;
    pbyBuf[1] = 0;
    RegAddr = OUT_Z_MSB;
    wBytesNum = 2;

    if(true == __MMA8541Q_ReadReg(RegAddr,pbyBuf,wBytesNum))
        return (u16)(pbyBuf[1]<<8 + pbyBuf[0]);
    else
        return 0;
}
// =============================================================================
// 功能：MMA芯片初始化，包括WP写保护功能引脚初始化和加载设备到对应的IIC总线
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================

ptu32_t MMA8541Q_ModuleInit(void)
{
    bool_t result = false;
    //GPIO初始化，SDA、SCL已经在IIC中初始化了，此处只需初始化WP即可
//  __MMA_GpioInit();

    //添加MMA8451到IIC0总线
    if(ps_MMA_Dev = IIC_DevAdd(CFG_MMA_BUS_NAME,"IIC_Dev_MMA8451Q",CFG_MMA_ADDRESS,0,8))
    {
        IIC_BusCtrl(ps_MMA_Dev,CN_IIC_SET_CLK,MMA_CLK_FRE,0);
        result = true;
    }

    return result;
}
