#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu peripheral register definition"//CPU外设数据结构定义
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION == false )
//#warning  " cpu_peripheral_register_definition  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_PERIPHERAL_REGISTER_DEFINITION    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
//dma相关寄存器
struct DMA_StReg volatile * const pg_dma1_reg
                        = (struct DMA_StReg *)0x40020000;
struct DMA_StReg volatile * const pg_dma2_reg
                        = (struct DMA_StReg *)0x40020400;

//dma1的各通道寄存器
struct DMA_StChannelReg volatile * const pg_dma1_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020008;
struct DMA_StChannelReg volatile * const pg_dma1_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002001c;
struct DMA_StChannelReg volatile * const pg_dma1_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020030;
struct DMA_StChannelReg volatile * const pg_dma1_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020044;
struct DMA_StChannelReg volatile * const pg_dma1_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020058;
struct DMA_StChannelReg volatile * const pg_dma1_channel6_reg
                        = (struct DMA_StChannelReg *)0x4002006c;
struct DMA_StChannelReg volatile * const pg_dma1_channel7_reg
                        = (struct DMA_StChannelReg *)0x40020080;
//dma2的各通道寄存器
struct DMA_StChannelReg volatile * const pg_dma2_channel1_reg
                        = (struct DMA_StChannelReg *)0x40020408;
struct DMA_StChannelReg volatile * const pg_dma2_channel2_reg
                        = (struct DMA_StChannelReg *)0x4002041c;
struct DMA_StChannelReg volatile * const pg_dma2_channel3_reg
                        = (struct DMA_StChannelReg *)0x40020430;
struct DMA_StChannelReg volatile * const pg_dma2_channel4_reg
                        = (struct DMA_StChannelReg *)0x40020444;
struct DMA_StChannelReg volatile * const pg_dma2_channel5_reg
                        = (struct DMA_StChannelReg *)0x40020458;

//gpio相关寄存器定义
struct GPIO_StReg volatile * const pg_gpio_rega
                        = (struct GPIO_StReg *)0x40010800;
struct GPIO_StReg volatile * const pg_gpio_regb
                        = (struct GPIO_StReg *)0x40010c00;
struct GPIO_StReg volatile * const pg_gpio_regc
                        = (struct GPIO_StReg *)0x40011000;
struct GPIO_StReg volatile * const pg_gpio_regd
                        = (struct GPIO_StReg *)0x40011400;
struct GPIO_StReg volatile * const pg_gpio_rege
                        = (struct GPIO_StReg *)0x40011800;
struct GPIO_StReg volatile * const pg_gpio_regf
                        = (struct GPIO_StReg *)0x40011c00;
struct GPIO_StReg volatile * const pg_gpio_regg
                        = (struct GPIO_StReg *)0x40012000;

struct AFIO_StReg volatile * const pg_afio_reg
                        = (struct AFIO_StReg *)0x40010000;
struct GPIO_StReg volatile * const pg_gpio_reg [7] = {
                                (struct GPIO_StReg *)0x40010800,
                                (struct GPIO_StReg *)0x40010c00,
                                (struct GPIO_StReg *)0x40011000,
                                (struct GPIO_StReg *)0x40011400,
                                (struct GPIO_StReg *)0x40011800,
                                (struct GPIO_StReg *)0x40011c00,
                                (struct GPIO_StReg *)0x40012000,
                               };

//串口相关寄存器
struct st_usart_reg volatile * const pg_uart1_reg
                        = (struct st_usart_reg *)0x40013800;
struct st_usart_reg volatile * const pg_uart2_reg
                        = (struct st_usart_reg *)0x40004400;
struct st_usart_reg volatile * const pg_uart3_reg
                        = (struct st_usart_reg *)0x40004800;
struct st_usart_reg volatile * const pg_uart4_reg
                        = (struct st_usart_reg *)0x40004c00;
struct st_usart_reg volatile * const pg_uart5_reg
                        = (struct st_usart_reg *)0x40005000;

//rcc(复位和时钟控制寄存器)
struct st_rcc_reg volatile * const pg_rcc_reg  = (struct st_rcc_reg *)0x40021000;

//片内flash控制寄存器
struct st_inflash_fpec_reg volatile * const pg_inflash_fpec_reg
                                = (struct st_inflash_fpec_reg *)0x40022000;
struct st_inflash_ob_reg volatile * const pg_inflash_ob_reg
                                = (struct st_inflash_ob_reg *)0x1FFFF800;

