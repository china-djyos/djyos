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

// 文件名     ：cpu_peri_gpio.h
// 模块描述: STM32F7xx的GPIO驱动
// 模块版本:
// 创建人员:
// 创建时间: 10/28.2016
// =============================================================================
#ifdef CFG_CPU_ZQ12XX_M0
#include "stdint.h"
#include "silan_iomux.h"
#include "cpu_peri_isr.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"cpu onchip gpio"//gpio操作函数集
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                 //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence                   //该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件不会被强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                 //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO == false )
//#warning  " cpu_onchip_gpio  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

/*Gpio Module*/

typedef struct
{
    volatile u32 GPIO_DATA;//0x000
}tagRegGpioData;


typedef struct
{
   volatile u32 GPIO_DIR; //0x400
   volatile u32 GPIO_IS;  //0x404
   volatile u32 GPIO_IBE; //0x408
   volatile u32 GPIO_IEV; //0x40C
   volatile u32 GPIO_IE;  //0x410
   volatile u32 GPIO_RIS; //0x414
   volatile u32 GPIO_MIS; //0x418
   volatile u32 GPIO_IC;  //0x41C
   volatile u32 GPIOAFSEL;//0x420
   volatile u32 GPIOFILSEL;//0x424
   volatile u32 GPIODIV;  //0x428

}tagRegGpio;

#define  GPIO1_REG_BASE ((volatile tagRegGpio*)(0x41070000 + 0x400))
#define  GPIO1_REG_DATA ((volatile u32*)(0x41070000 + 0x00))

#define  GPIO2_REG_BASE (volatile tagRegGpio*)(0x41080000 + 0x400)
#define  GPIO2_REG_DATA ((volatile u32 *)(0x41080000 + 0x00))

typedef struct
{
   tagRegGpioData *pRegGpioData;//data Register
   tagRegGpio     *pRegGpioBase;
}tagRegBaseInfo;

#define CN_GPIO_GROUP_NUM   (2)

#define CN_GPIO1_MAX_NUM    22
#define CN_GPIO2_MAX_NUM    10

/*get gpio pin group location*/

#define GET_GPIO_PIN_GROUP(gpio)    (gpio / (CN_GPIO1_MAX_NUM+1))

/*get gpio pin register location*/

#define GET_GPIO_REG_OFFSET(gpio)    (gpio % (CN_GPIO1_MAX_NUM+1))

/*turn to gpio pin*/

#define IO_TO_GPIO_PIN(x)    ((x>=8) ? (x-8) : (34+x))


/*interrupt Control Register 6 and 7*/

#define REG_INT_CTRL_INT_LINE6 (0x42030000 + 0x618)
#define REG_INT_CTRL_INT_LINE7 (0x42030000 + 0x61C)

static volatile u32 *spg_RegIntEnable[2] = {
    (volatile u32*)(REG_INT_CTRL_INT_LINE6),
    (volatile u32*)(REG_INT_CTRL_INT_LINE7),
};


/*Gpio Module Register Info*/

static volatile tagRegBaseInfo s_RegGpioInfo[CN_GPIO_GROUP_NUM] =
{
    {   //Gpio Group 1
        (volatile u32 *)GPIO1_REG_DATA,
        (volatile tagRegGpio*)GPIO1_REG_BASE,
    },
       //Gpio Group 2
    {
        (volatile u32 *)GPIO2_REG_DATA,
        (volatile tagRegGpio*)GPIO2_REG_BASE,
    }
};



// =============================================================================
// Function Describe:
// Config io to input
//
// 配置对应 的IO为输入模式
// =============================================================================

void Gpio_Input_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

   if(GpioRegBase != NULL)
   {
       GpioRegBase->GPIO_DIR &= ~(1 << byPinRegLoc);
   }

}


// =============================================================================
// Function Describe:
// Config io to output
//
//
// =============================================================================

void Gpio_Output_Config(u8 io)
{
   u8 byPinRegLoc;
   u8 byPinGroupLoc;
   u8 byGpio;
   volatile tagRegGpio* GpioRegBase  = NULL;

   //turn to gpio
   byGpio = IO_TO_GPIO_PIN(io);

   //get Pin which Group
   byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

   //get offence
   byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

   GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

   if(GpioRegBase != NULL)
   {
       GpioRegBase->GPIO_DIR |= (1 << byPinRegLoc);
   }
}


// =============================================================================
// Function Describe:
// Config io input and output derection config
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Derection_Config(u8 io,bool_t bDerection)
{
    if(bDerection)
    {
        Gpio_Output_Config(io);
    }else
    {
        Gpio_Input_Config(io);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Hight
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Hight(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        GpioRegData->GPIO_DATA |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

void Gpio_Set_Value_Low(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpioData*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        GpioRegData->GPIO_DATA &= ~(1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// Set io Level Low
// arg0:io
// arg1:bDerection TRUE - output False :input
// =============================================================================

u8 Gpio_Get_Level(u8 io)
{
    u8 byRet;
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpioData* GpioRegData  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegData = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioData;

    if(GpioRegData != NULL)
    {
        if(GpioRegData->GPIO_DATA & (1 << byPinRegLoc))

           byRet = 1;
        else
           byRet = 0;

    }

    return byRet;
}

// =============================================================================
// Function Describe:
// Enable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Enable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE |= (1 << byPinRegLoc);
        //int Ctrl Register
        *(spg_RegIntEnable[byPinRegLoc]) = 1 << byPinRegLoc;
    }


}


// =============================================================================
// Function Describe:
// Disable Int
// arg0:io
// arg1:
// =============================================================================

void Gpio_Disable_Int(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IE &= ~(1 << byPinRegLoc);
        *(spg_RegIntEnable[byPinRegLoc]) &= ~(1 << byPinRegLoc);
    }
}



// =============================================================================
// Function Describe:
// Clear Irq
// arg0:io
// arg1:
// =============================================================================

void Gpio_Clear_Irq(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    if(GpioRegBase != NULL)
    {
        GpioRegBase->GPIO_IC |= (1 << byPinRegLoc);
    }
}


// =============================================================================
// Function Describe:
// 设置外部中断触发模式
// io:需要配置的io
// trugleMethod:    边沿触发 还是 电平触发
// edgeTrugleMethod:具体是高、低电平触发还是上升沿或者下降沿触发
// =============================================================================

void Gpio_Set_Iqr_Mode(u8 io,u8 trugleMethod,u8 edgeTrugleMethod)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    switch(trugleMethod)
    {
        case EN_EDGE:   //边沿触发
             GpioRegBase->GPIO_IS &= ~(1<<byPinRegLoc);
             if(edgeTrugleMethod == EN_BOTH_EDGE)           //双边沿触发
             {
                GpioRegBase->GPIO_IBE |= (1<<byPinRegLoc);  //双边沿触发
             }
             else if(edgeTrugleMethod == EN_RISING_EDGE)    //上升沿触发
             {
                 //选择上升沿或者下降沿触发
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //选择高电平或者上升沿触发
                 GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc);
             }else if(edgeTrugleMethod == EN_FALLING_EDGE)  //下降沿触发
             {
                 //选择上升沿或者下降沿触发
                 GpioRegBase->GPIO_IBE &= ~(1<<byPinRegLoc);
                 //选择低电平或者下降沿触发
                 GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);
             }
             break;
        case EN_LEVEL:                                      //电平触发
             GpioRegBase->GPIO_IS |= (1<<byPinRegLoc);

             if(edgeTrugleMethod == EN_HIGHT_LAVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV |= (1<<byPinRegLoc); //高电平触发
             }else if(edgeTrugleMethod == EN_LOW_LEVEL_EDGE)
             {
                GpioRegBase->GPIO_IEV &= ~(1<<byPinRegLoc);//低电平触发
             }
             break;
    }

}


// =============================================================================
// Function Describe:
// 婊ゆ尝鍒嗛閫夋嫨
//
//
// =============================================================================

void Gpio_Io_Filter_Config_Div(u8 io,u8 div)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIODIV |= ((div&0xf)<<1)|0x1;

}


// =============================================================================
// Function Describe:
// Gpio 打开虑波配置
//
//
// =============================================================================


void Gpio_io_Filter_Open(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL |= 1<<byPinRegLoc ;

}



// =============================================================================
// Function Describe:
// 关闭 Gpio 滤波配置
//
//
// =============================================================================

void Gpio_io_Filter_Close(u8 io)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    GpioRegBase->GPIOFILSEL &= ~(1<<byPinRegLoc) ;

}


// =============================================================================
// Function Describe:
// Gpio 外部中断请求
//
//
// =============================================================================

enum EN_GPIO_INTLINE{
    EN_GPIO1_INT_LINE = 0x00,
    EN_GPIO2_INT_LINE,
};

void Register_Gpio_Irq(u8 io,fnGpioHandle_t IsrHandle)
{
    u8 byPinRegLoc;
    u8 byPinGroupLoc;
    u8 byGpio;
    volatile tagRegGpio* GpioRegBase  = NULL;
    u8 intLine;      //中断线
    u8 subIntLine;   //子中断线

    //turn to gpio
    byGpio = IO_TO_GPIO_PIN(io);

    //get Pin which Group
    byPinGroupLoc = GET_GPIO_PIN_GROUP(byGpio);

    //get offence
    byPinRegLoc = GET_GPIO_REG_OFFSET(byGpio);

    GpioRegBase = (volatile tagRegGpio*)s_RegGpioInfo[byPinGroupLoc].pRegGpioBase;

    //Gpio1 为6号中断线 Gpio2 为7号 中断线
    switch(byPinGroupLoc)
    {
        case EN_GPIO1_INT_LINE:
             intLine    = 6;
             subIntLine = byPinRegLoc;
             break;
        case EN_GPIO2_INT_LINE:
             intLine = 7;
             subIntLine = byPinRegLoc;
             break;
        default:;break;
    }

    if(IsrHandle != NULL)
    {
        djybsp_isr_hdl_register(intLine, subIntLine,IsrHandle,0);
    }

}

#endif

