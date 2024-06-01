#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <os.h>
#include <cpu_peri.h>
#include <uartctrl.h>
#include <cpu_peri_gpio.h>
#include "project_config.h"

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
//dependence:"kernel","stm32f7","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
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



#include "at32f435_437.h"

/* 
 * https://www.arterytek.com/download/RM/RM_AT32F435_437_CH_V2.03.pdf
 */

//DEBUG口
static const Pin uart1_pin[] = {
        {GPIO_A,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //RX
        {GPIO_A,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //TX
};

static const Pin uart2_pin[] = {
        {GPIO_D,PIN5,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //TX
        {GPIO_A,PIN3,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},    //RX
};

static const Pin uart3_pin[] = {
        {GPIO_B,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //TX
        {GPIO_B,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_7},   //RX
};

static const Pin uart4_pin[] = {
        {GPIO_C,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        {GPIO_C,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //RX
};

static const Pin uart5_pin[] = {
        {GPIO_B,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        {GPIO_B,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
        // {GPIO_C,PIN12,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        // {GPIO_D,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_PU,MUX_8},   //RX
};

static const Pin uart6_pin[] = {
        {GPIO_A,PIN4,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
        {GPIO_A,PIN5,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        // {GPIO_C,PIN6,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //TX
        // {GPIO_C,PIN7,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},   //RX
};

static const Pin uart7_pin[] = {
        {GPIO_E,PIN7,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
        {GPIO_E,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX
};

static const Pin uart8_pin[] = {
        {GPIO_C,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //TX 
        {GPIO_C,PIN3,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_8},    //RX
};

static const Pin EthRmii_pins[] = {
        {GPIO_A,PIN2,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //mdio
        {GPIO_C,PIN1,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //mdc
        {GPIO_G,PIN11,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_d0
        {GPIO_G,PIN13,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_d1
        {GPIO_G,PIN14,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //tx_en
        {GPIO_D,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_dv
        {GPIO_D,PIN9,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_d0
        {GPIO_D,PIN10,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //rx_d1
        {GPIO_A,PIN1,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_11},  //clk
        {GPIO_A,PIN8,GPIO_MODE_MUX,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_0},   //应该是给phy芯片提供时钟用的
};

static const Pin ResetPHY_pins[] = {
        {GPIO_E,PIN15,GPIO_MODE_OUTPUT,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_NUll},
        {GPIO_G,PIN15,GPIO_MODE_OUTPUT,GPIO_OTYPE_PP,GPIO_DRIVE_STRENGTH_STRONGER,GPIO_PUPD_NONE,MUX_NUll},
};

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功发送功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexSend(u8 SerialNo)
{

}

// ============================================================================
// 功能：设置串口编号为SerialNo的串口为半双功接收功能
// 参数：SerialNo,串口号
// 返回：无
// ============================================================================
void Board_UartHalfDuplexRecv(u8 SerialNo)
{

}

// ============================================================================
// 功能：复位PHY芯片
// 参数：无
// 返回：无
// ============================================================================
void Board_ResetPHY(void)
{
   /* exit power down mode */
   PIO_Clear(&ResetPHY_pins[1]);

   /*reset phy */
   PIO_Clear(&ResetPHY_pins[0]);
   DJY_DelayUs(20000);
   PIO_Set(&ResetPHY_pins[0]);
   DJY_DelayUs(20000);
}



void Board_Init(void)
{
    PIO_Configure(uart1_pin, PIO_LISTSIZE(uart1_pin));
    PIO_Configure(uart2_pin, PIO_LISTSIZE(uart2_pin));
    PIO_Configure(uart3_pin, PIO_LISTSIZE(uart3_pin));
    PIO_Configure(uart4_pin, PIO_LISTSIZE(uart4_pin));
    PIO_Configure(uart5_pin, PIO_LISTSIZE(uart5_pin));
    PIO_Configure(uart6_pin, PIO_LISTSIZE(uart6_pin));
    PIO_Configure(uart7_pin, PIO_LISTSIZE(uart7_pin));
    PIO_Configure(uart8_pin, PIO_LISTSIZE(uart8_pin));
    PIO_Configure(EthRmii_pins, PIO_LISTSIZE(EthRmii_pins));
    PIO_Configure(ResetPHY_pins, PIO_LISTSIZE(ResetPHY_pins));

}
