#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <os.h>
#include <cpu_peri.h>
#include <uartctrl.h>

#include "project_config.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//板件特性驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                       //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                     //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","at32f43x","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
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
//%$#@enum,true,flase,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#include "at32f435_437.h"

/* 
 * https://www.arterytek.com/download/RM/RM_AT32F435_437_CH_V2.03.pdf
 */

void Board_Init(void)
{
	gpio_init_type hw_gpio = {
		.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3 | GPIO_PINS_1 | GPIO_PINS_9 | GPIO_PINS_10 | GPIO_PINS_12,
		.gpio_out_type = GPIO_OUTPUT_PUSH_PULL,
		.gpio_pull = GPIO_PULL_NONE,
		.gpio_mode = GPIO_MODE_MUX,
		.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER
	};

	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK,  TRUE);
	crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);

	gpio_init(GPIOA, &hw_gpio);

	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE9,  GPIO_MUX_7);	/* USART2_TX     */
	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE10, GPIO_MUX_7);	/* USART2_RX     */
	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE12, GPIO_MUX_7);	/* USART2_RTS_DE */

	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_7);	/* USART2_TX     */
	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_7);	/* USART2_RX     */
	gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE1, GPIO_MUX_7);	/* USART2_RTS_DE */
}
