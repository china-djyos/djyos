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


#include "stdint.h"
#include "stddef.h"
#include <int.h>
#include <shell.h>
#include "cpu_peri.h"
#include "board.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIG == false )
//#warning  " board_config  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure



void OpenBackLight()
{
}

void CloseBackLight()
{
}

void OpenScreen()
{
}

void CloseScreen()
{
}

void CloseSpeaker()
{
}
void OpenSpeaker()
{
}

//===============================================================
// TP复位引脚控制，RST和INT引脚时序配合，设置GT911的IIC地址为0x5d
//  0=设置RESET为低电平，1=高电平
//==============================================================
void LcdTp_Reset_OnOff(u8 onoff)
{
  if(onoff)
  {
      GPIO_SettoHigh(GPIOA,GPIO_Pin_3 );
  }
  else
  {
      GPIO_SettoLow(GPIOA,GPIO_Pin_3);
  }

}

//===============================================================
// TP INT引脚控制，RST和INT引脚时序配合，设置GT911的IIC地址为0x5d
//  0=设置 INT为低电平；1=高阻（CPU的INT引脚被设定为OD）
//==============================================================
void LcdTp_Int_OnOff(u8 onoff)
{
  if(onoff)
  {
      GPIO_SettoHigh(GPIOA,GPIO_Pin_1 );
  }
  else
  {
      GPIO_SettoLow(GPIOA,GPIO_Pin_1);
  }

}

//===============================================================
// TP INT引脚设为高阻态
//==============================================================
void LcdTp_Int_Input(void)
{
    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_IN,  GPIO_PuPd_NOPULL);   //int
}

u32 IIC_IoCtrlFunc(enum IIc_Io IO,u32 tag)
{
    switch(tag)
    {
        case 1 :
            switch(IO)
            {
            case scl_set_High : CT_IIC_SCL(1); break;
            case scl_set_Low  : CT_IIC_SCL(0); break;
            case scl_set_out  : CT_SCL_OUT();  break;
            case sda_set_High : CT_IIC_SDA(1); break;
            case sda_get      : return CT_READ_SDA;
            case sda_set_Low  : CT_IIC_SDA(0); break;
            case sda_set_out  : CT_SDA_OUT();  break;
            case sda_set_in   : CT_SDA_IN();   break;
            default:
                break;
            }
        break;
    }
    return 0;
}

#define SPI1_PA_EN  0
void Board_Init(void)
{
    u32 i=0;
    //uart PIN初始化
    GPIO_Congif(GPIOE,GPIO_Pin_0,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX0
    GPIO_Congif(GPIOE,GPIO_Pin_1,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX0
//    GPIO_Congif(GPIOA,GPIO_Pin_2,GPIO_Mode_101,GPIO_PuPd_NOPULL);//RX1
//    GPIO_Congif(GPIOA,GPIO_Pin_3,GPIO_Mode_101,GPIO_PuPd_NOPULL);//TX1
    GPIO_Congif(GPIOE,GPIO_Pin_7,GPIO_Mode_011,GPIO_PuPd_NOPULL);//TX2
    GPIO_Congif(GPIOE,GPIO_Pin_8,GPIO_Mode_011,GPIO_PuPd_NOPULL);//RX2

    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_OUT, GPIO_PuPd_UP);   //rst
    GPIO_SettoHigh(GPIOA,GPIO_Pin_1 );

    //iic0 PIN初始化,io模拟IIC
    GPIO_Congif(GPIOA, GPIO_Pin_0, GPIO_Mode_OUT, GPIO_PuPd_UP);   //scl
    GPIO_Congif(GPIOA, GPIO_Pin_1, GPIO_Mode_OUT,  GPIO_PuPd_NOPULL);   //int
    GPIO_Congif(GPIOA, GPIO_Pin_2, GPIO_Mode_OUT, GPIO_PuPd_UP);   //sda
    GPIO_Congif(GPIOA, GPIO_Pin_3, GPIO_Mode_OUT, GPIO_PuPd_UP);   //rst

    //以下是SPI PIN初始化
    for(i=0;i<4;i++)    //SPI0
    {
        GPIO_Congif(GPIOC,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOC,GPIO_Pin_0+i,3);//增加驱动力
    }
    for(i=0;i<4;i++)    //SPI1
    {
#if SPI1_PA_EN == 1      //SPI1使用PA0~3
        GPIO_Congif(GPIOA,GPIO_Pin_0+i,GPIO_Mode_110,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOA,GPIO_Pin_0+i,3);//增加驱动力
#else                       //SPI1使用PE7~10
        GPIO_Congif(GPIOE,GPIO_Pin_7+i,GPIO_Mode_100,GPIO_PuPd_NOPULL);
        GPIO_Multi_Driving(GPIOE,GPIO_Pin_7+i,3);//增加驱动力
#endif
    }
}

void Init_Cpu(void);
void SoftReset(void)
{
    Init_Cpu();
}
