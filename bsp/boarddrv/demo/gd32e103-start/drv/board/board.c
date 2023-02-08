#include <stdint.h>
#include <stddef.h>

#include <gd32e103r_start.h>
#include <gd32e10x.h>

#include <os.h>
#include <cpu_peri.h>

#include "uartctrl.h"
#include "pcf8574.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board config"//board featrue and drivers
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                       //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:required                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                     //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"kernel","gd32e10x","cpu onchip gpio"//该组件的依赖组件名（可以是none，表示无依赖组件），
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

// =============================================================================
// 功能：根据具体的板件配置串口的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      串口驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和串口时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_UartGpioInit(u8 SerialNo)
{
	rcu_periph_enum periph;
	uint32_t gpio_periph;
	uint32_t mode;
	uint32_t speed;
	uint32_t pin;

	uint32_t com = 0;

	com = SerialNo;
	if (com > CN_UART4) return false;
	
	/* debug: use usart3 for debug
		PC10 - White wire
		PC11 - Green wire */
	
	com = UART3;
	gpio_periph = GPIOC;

	/* enable GPIO clock */
	rcu_periph_clock_enable(RCU_GPIOC);

	/* enable USART clock */
	rcu_periph_clock_enable(RCU_UART3);

	/* connect port to USARTx_Tx */
	gpio_init(gpio_periph, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

	/* connect port to USARTx_Rx */
	gpio_init(gpio_periph, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_11);

	/* USART configure */
	usart_deinit(com);
	usart_baudrate_set(com, 115200U);
	usart_receive_config(com, USART_RECEIVE_ENABLE);
	usart_transmit_config(com, USART_TRANSMIT_ENABLE);
	usart_enable(com);

        while (1) {

                /* transmit */
                uint32_t usart_periph = UART3;
                static uint32_t data = 'U';
                /* receive */
                uint16_t ch;

                usart_data_transmit(usart_periph, data);
                while (usart_flag_get(usart_periph, USART_FLAG_TBE) == RESET);

                if (usart_flag_get(usart_periph, USART_FLAG_RBNE) == SET) {
                        ch = usart_data_receive(usart_periph);
                        data = ch;
                }
                /* 测试串口收发 */
                break;
        };


	return true;
}

// =============================================================================
// 功能：根据具体的板件配置IIC的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      IIC驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和外设时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_IicGpioInit(u8 I2Cx)
{

    return true;
}
// =============================================================================
// 功能：网络相关的gpio配置
// 参数：无
// 返回：true
// =============================================================================

bool_t Board_EthGpioInit(void)
{
//    GPIO_PowerOn(GPIO_A);
//    GPIO_PowerOn(GPIO_C);
//    GPIO_PowerOn(GPIO_D);
//    GPIO_PowerOn(GPIO_G);
//    RCC->APB2ENR|=1<<14;    //使能SYSCFG时钟
//    SYSCFG->PMC|=1<<23;     //使用RMII接口


#if 0

    GPIO_CfgPinFunc(GPIO_A,PIN1|PIN2|PIN7,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_D,PIN3,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_NONE);
    GPIO_CfgPinFunc(GPIO_C,PIN1|PIN4|PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);
    GPIO_CfgPinFunc(GPIO_G,PIN11|PIN13|PIN14,GPIO_MODE_AF,GPIO_OTYPE_PP,
                    GPIO_SPEED_100M,GPIO_PUPD_PU);

    GPIO_AFSet(GPIO_A,1,11);    //PA1,AF11
    GPIO_AFSet(GPIO_A,2,11);    //PA2,AF11
    GPIO_AFSet(GPIO_A,7,11);    //PA7,AF11

    GPIO_AFSet(GPIO_C,1,11);    //PC1,AF11
    GPIO_AFSet(GPIO_C,4,11);    //PC4,AF11
    GPIO_AFSet(GPIO_C,5,11);    //PC5,AF11

    GPIO_AFSet(GPIO_G,11,11);   //PG11,AF11
    GPIO_AFSet(GPIO_G,13,11);   //PG13,AF11
    GPIO_AFSet(GPIO_G,14,11);   //PG14,AF11

    GPIO_SettoLow(GPIO_D,PIN3);
    DJY_EventDelay(10*1000);
    GPIO_SettoHigh(GPIO_D,PIN3);
#endif
    return true;
}
// =============================================================================
// 功能：根据具体的板件配置SPI的GPIO的引脚功能，这是与板件相关，所以该函数放在该文件，CPU
//      SPI驱动直接调用该函数来初始化串口的GPIO引脚
//      主要包括GPIO端口和外设时钟使能、GPIO配置、重映射、时钟等
// 参数：无
// 返回：true
// =============================================================================
bool_t Board_SpiGpioInit(u8 SPIx)
{
    if(SPIx == CN_SPI1)
    {
//        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // 使能PORTB时钟
//        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; // 使能SPI1时钟
//
//        RCC->AHB1ENR |= 1<<6; // 使能PORTG时钟

//##WIP##        RCC->APB2RSTR |= RCC_APB2RSTR_SPI1; // 复位SPI1
//##WIP##        RCC->APB2RSTR &= ~RCC_APB2RSTR_SPI1; // 停止复位SPI1

#if 0
        GPIO_CfgPinFunc(GPIO_B,PIN4|PIN5|PIN3,GPIO_MODE_AF,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB3、4和5为SCK、MISO和MOSI
        GPIO_AFSet(GPIO_B,3,5);
        GPIO_AFSet(GPIO_B,4,5);
        GPIO_AFSet(GPIO_B,5,5);
// CS由设备确定
        GPIO_CfgPinFunc(GPIO_B,PIN14,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_50M,GPIO_PUPD_PU); // GPB14为CS用于NOR

        GPIO_CfgPinFunc(GPIOG,PIN7,GPIO_MODE_OUT,GPIO_OTYPE_PP,
                GPIO_SPEED_100M,GPIO_PUPD_PU); // GPG7为CS用于无线模式
        GPIOG->ODR|=1<<7;

        GPIO_SettoHigh(GPIO_B,PIN14);// CS为高
#endif
    }
    else
    {
        return false;
    }
    return true;
}

// =============================================================================
// 功能：根据具体的板件485所需的控制管脚，控制485为发送状态该板件由硬件电路实现无需控制
// 参数：串口号如：CN_UART1
// 返回：无
// =============================================================================

void Board_UartHalfDuplexSend(u8 SerialNo)
{
    switch(SerialNo)
    {
        case CN_UART1:break;
        case CN_UART2:break;
        default:      break;
    }
}

// ============================================================================
// 功能：获取端口的类型RS_485/RS_232 RS485 返回false
// 参数：SerialNo,串口号
// 返回：true/false
// ============================================================================
u8   Board_CheckPortIsRS232(u8 SerialNo)
{

    if(SerialNo==CN_UART1||SerialNo==CN_UART2)
        return false;
    return true;
}

 // =============================================================================
 // 功能：根据具体的板件485所需的控制管脚，控制485为接受收状态
 // 参数：串口号如：CN_UART1
 // 返回：无
 // =============================================================================

void Board_UartHalfDuplexRecv(u8 SerialNo)
{

    switch(SerialNo)
    {
    case CN_UART1: break;
    case CN_UART2: break;
    default:       break;
    }
}

// =============================================================================
// 功能：根据端口号控制片选
// 参数：SPIPort端口号  level==0使能使能或  1失能
// 返回：true
// =============================================================================
#define BITBAND(addr, bitnum)       ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)      MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOB_ODR_Addr                (GPIOB+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //输出

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

bool_t Board_SpiCsCtrl(u8 SPIPort,u8 cs,u8 level)
{
    switch(SPIPort)
    {
        case CN_SPI1:
            if(level)
                PBout(14) = 1;
            else
                PBout(14) = 0;
            break;
        default:  return false;
    }
    return true;
}
#pragma GCC diagnostic pop

static void Board_NORGpioInit (void )
{
#if 0
    Board_SpiGpioInit(CN_SPI1);
    GPIO_CfgPinFunc(GPIO_B, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_50M,
                    GPIO_PUPD_PU); // GPB14为CS用于NOR

    // RCC->AHB1ENR |= 1<<6; // 使能PORTG时钟
    GPIO_CfgPinFunc(GPIOG,PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_100M,
                    GPIO_PUPD_PU); // GPG7为CS用于无线模式
    // GPIOG->ODR |= 1<<7;

    GPIO_SettoHigh(GPIO_B,PIN14);// CS为高
#endif
}

void board_rest_init(void)
{
    /* enable the led clock */
    rcu_periph_clock_enable(RCU_GPIOC);

    /* configure led GPIO port */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_bit_set(GPIOC,GPIO_PIN_6);
}
void Board_RestInit  (void) __attribute__ ((weak, alias ("board_rest_init")));
void Board_Rest_Init (void) __attribute__ ((weak, alias ("board_rest_init")));


// =============================================================================
// 功能：Board_GpioInit板上所有用到的GPIO初始化
//  在这里将所有可能用到的复用模式的gpio定义在这里需要用取消注释即可
// 参数：无
// 返回：无
// =============================================================================

void Board_Init(void)
{
    Board_EthGpioInit();//网络
    Board_UartGpioInit(CN_UART3);
    Board_NORGpioInit();
    Board_RestInit();
}

////网口的 PHY 芯片：LAN8720复位
bool_t LAN8720_RESET(void)
{
//  PCF8574_WriteBit(ETH_RESET_IO,1);
//  DJY_DelayUs(100*mS);
//  PCF8574_WriteBit(ETH_RESET_IO,0);
//  DJY_DelayUs(100*mS);
    //PD3上输出一个100mS的高电平脉冲
    return true;
}

