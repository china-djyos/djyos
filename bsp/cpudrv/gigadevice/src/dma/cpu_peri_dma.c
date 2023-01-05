#include <stddef.h>

#include <cpu_peri.h>
#include <systime.h>
#include <djyos.h>
#include <cpu_peri_dma.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip dma"//CPU的dma驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_DMA == false )
//#warning  " cpu_onchip_dma  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_DMA    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
// =============================================================================

#define DMA_WAIT_TIME (2*mS)
//DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMA通道选择,范围:0~7
//par:外设地址
//mar:存储器地址

typedef void DMA_Stream_TypeDef;
// =============================================================================
// 功能：DMAx的各通道配置
// 参数：DMA_Streamx,DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      chx:DMA通道选择,范围:0~7
//      par:外设地址
//      mar:存储器地址
//      dir,方向，从外设到内存、内存到内存，外设到外设
//      msize,
//      psize,
//      ndtr,
// 返回：
// =============================================================================
u32 DMA_Config(DMA_Stream_TypeDef *DMA_Streamx,u8 chx,u32 par,u32 mar,
        u8 dir,u8 msize,u8 psize,u16 ndtr)
{
    return true;
}

// =============================================================================
// 功能：开启一次DMA传输
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//       ndtr:数据传输量
// 返回：无
// =============================================================================
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u32 mar,u16 ndtr)
{
}

// =============================================================================
// 功能：禁止使用相应的DMA流
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：无
// =============================================================================
void DMA_Disable(DMA_Stream_TypeDef *DMA_Streamx)
{
}

// =============================================================================
// 功能：清除相应流上的所有中断标志位
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// 返回：无
// =============================================================================
void DMA_ClearIntFlag(DMA_Stream_TypeDef *DMA_Streamx)
{
}

// =============================================================================
// 功能：使能相应流上的中断
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE，为(1<<3)
// 返回：
// =============================================================================
void DMA_IntEnable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
}

// =============================================================================
// 功能：判断相应流的中断
// 参数：DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
//      SrcInt,中断位掩码，如DMA_INT_HTIE，为(1<<3)
// 返回：无
// =============================================================================
void DMA_IntDisable(DMA_Stream_TypeDef *DMA_Streamx,u8 SrcInt)
{
}

