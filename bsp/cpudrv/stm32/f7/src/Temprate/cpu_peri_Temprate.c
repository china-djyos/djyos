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
// =============================================================================

// 文件名     ：cpu_peri_uart.c
// 模块描述: DJYOS串口模块的底层驱动部分，主要实现寄存器级别的操作，如中断等
// 模块版本: V1.10
// 创建人员:
// 创建时间:
// =============================================================================

#include <djyos.h>
#include <stdint.h>
#include "cpu_peri.h"
#include "cpu_peri_temprate.h"
// =============================================================================
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern  bool_t Temprate_Init(void);
//    Temprate_Init( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip temprate"//CPU的测温外设驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                          //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                       //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                         //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                       //初始化时机，可选值：early，medium，later, pre-main。
                                         //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件将强制选中，
                                         //如果依赖多个组件，则依次列出
//weakdependence:"none"                  //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                         //选中该组件时，被依赖组件不会被强制选中，
                                         //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                         //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_TEMPRATE == false )
//#warning  " cpu_onchip_temprate  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_TEMPRATE    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define ADC_CH_TEMP     18

//AD时钟分频系数2,4,6,8.  PCLK2/ADC_CLK_DIV=CLk
#define ADC_CLK_DIV   4

// =============================================================================
// 功能: 初始化CPU内部温度传感器
// 参数: 无
// 返回: 无
// =============================================================================

bool_t Temprate_Init(void)
{

    RCC->APB2ENR|=1<<8;     //使能ADC1时钟
    RCC->APB2RSTR|=1<<8;    //ADCs复位
    RCC->APB2RSTR&=~(1<<8); //复位结束
    ADC->CCR=((ADC_CLK_DIV>>1)-1)<<16;//AD时钟设置
    ADC->CCR|=1<<23;        //使能内部温度传感器

    ADC1->CR1=0;            //CR1设置清零
    ADC1->CR2=0;            //CR2设置清零
    ADC1->CR1|=0<<24;       //12位模式
    ADC1->CR1|=0<<8;        //非扫描模式

    ADC1->CR2&=~(1<<1);     //单次转换模式
    ADC1->CR2&=~(1<<11);    //右对齐
    ADC1->CR2|=0<<28;       //软件触发

    ADC1->SQR1&=~(0XF<<20);
    ADC1->SQR1|=0<<20;      //1个转换在规则序列中 也就是只转换规则序列1

    ADC1->SMPR1&=~(7<<(3*(18-10))); //清除通道18原来的设置
    ADC1->SMPR1|=7<<(3*(18-10));    //通道18 480周期,提高采样时间可以提高精确度

    ADC1->CR2|=1<<0;                //开启AD转换器
    return true;
}

// =============================================================================
// 功能: 获取AD采样的值
// 参数: 通道号
// 返回: AD值
// =============================================================================
u16 Get_Adc(u8 ch)
{
    u16 timeout=0;
    //设置转换序列
    ADC1->SQR3&=0XFFFFFFE0;//规则序列1 通道ch
    ADC1->SQR3|=ch;
    ADC1->CR2|=1<<30;       //启动规则转换通道
    while(!(ADC1->SR&1<<1))//等待转换结束
    {
        timeout++;
        if(timeout>=1000000)
            return 0;//超时返回0
    }
    return ADC1->DR;
}

// =============================================================================
// 功能:获取AD采样均值
// 参数: AD 通道号
//      采样次数
// 返回: 无
// =============================================================================
static u16 Get_Adc_Average(u8 ch,u8 times)
{
    u32 temp_val=0;
    u8 i;
    for(i=0;i<times;i++)
    {
        temp_val+=Get_Adc(ch);
        DJY_DelayUs(5);
    }
    return temp_val/times;
}

// =============================================================================
// 功能: 获取温度的值
// 参数: 空
// 注 :函数直接返回单精度或者双精度的参数返回后数值会出错,
//   所以在这里不能采用直接返回的方式获取温度
// 返回: 温度值
// =============================================================================
float Get_Temprate(void)
{
    u32 adcx;
    float temperate;

    adcx=Get_Adc_Average(ADC_CH_TEMP,Temprate_num);

    temperate=(float)adcx*(3.3/4096);

    temperate=(temperate-0.76)/0.0025+25;

    return temperate;
}





