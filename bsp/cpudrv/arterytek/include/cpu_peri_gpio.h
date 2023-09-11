#ifndef __cpu_peri_gpio_h_
#define __cpu_peri_gpio_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <at32f435_437_gpio.h>

#define PIO_LISTSIZE(pPins)    (sizeof(pPins) / sizeof( Pin))


//GPIO端口设置
enum GPIO_PORT_NUM
{
    GPIO_A = 0,
    GPIO_B = 1,
    GPIO_C = 2,
    GPIO_D = 3,
    GPIO_E = 4,
    GPIO_F = 5,
    GPIO_G = 6,
    GPIO_H = 7,

    GPIO_NULL = 8,  //无效端口号
};

//GPIO引脚定义
enum GPIO_PIN_NUM
{
    PIN0 = GPIO_PINS_0, 
    PIN1 = GPIO_PINS_1, 
    PIN2 = GPIO_PINS_2, 
    PIN3 = GPIO_PINS_3, 
    PIN4 = GPIO_PINS_4, 
    PIN5 = GPIO_PINS_5, 
    PIN6 = GPIO_PINS_6, 
    PIN7 = GPIO_PINS_7, 
    PIN8 = GPIO_PINS_8, 
    PIN9 = GPIO_PINS_9, 
    PIN10 = GPIO_PINS_10,
    PIN11 = GPIO_PINS_11,
    PIN12 = GPIO_PINS_12,
    PIN13 = GPIO_PINS_13,
    PIN14 = GPIO_PINS_14,
    PIN15 = GPIO_PINS_15,

    PIN_NULL = GPIO_PINS_15 + 1,    //无效引脚号
};

enum GPIO_MODE
{
    GPIO_IN_MODE      = GPIO_MODE_INPUT,        //普通输入模式
    GPIO_OUT_MODE     = GPIO_MODE_OUTPUT,        //普通输出模式
    GPIO_MUX_MODE     = GPIO_MODE_MUX ,        //MUX功能模式
    GPIO_ANALOG_MODE  = GPIO_MODE_ANALOG,        //模拟输入/输出模式

    GPIO_NULL_MODE    = 0xff,         //无效的工作模式
};

enum GPIO_PUPD
{
    GPIO_PUPD_NONE = GPIO_PULL_NONE,       //不带上下拉
    GPIO_PUPD_PU   = GPIO_PULL_UP,          //上拉
    GPIO_PUPD_PD   = GPIO_PULL_DOWN,       //下拉

    GPIO_PUPD_NULL = 0xff,                  //无效的上下拉
};

enum GPIO_DRIVE
{
    GPIO_DRIVE_SS = GPIO_DRIVE_STRENGTH_STRONGER,       //更高强度
    GPIO_DRIVE_SM = GPIO_DRIVE_STRENGTH_MODERATE,       //中等强度

    GPIO_DRIVE_NULL = 0xff,                             //无效的强度
};

enum GPIO_OTYPE
{
    GPIO_OTYPE_PP = GPIO_OUTPUT_PUSH_PULL,       //推挽输出
    GPIO_OTYPE_OD = GPIO_OUTPUT_OPEN_DRAIN,      //开漏输出

    GPIO_OTYPE_NULL = 0xff,                      //无效的输出类型
};

enum GPIO_MUX
{
    MUX_0   = GPIO_MUX_0,
    MUX_1   = GPIO_MUX_1,
    MUX_2   = GPIO_MUX_2,
    MUX_3   = GPIO_MUX_3,
    MUX_4   = GPIO_MUX_4,
    MUX_5   = GPIO_MUX_5,
    MUX_6   = GPIO_MUX_6,
    MUX_7   = GPIO_MUX_7,
    MUX_8   = GPIO_MUX_8,
    MUX_9   = GPIO_MUX_9,
    MUX_10  = GPIO_MUX_10,
    MUX_11  = GPIO_MUX_11,
    MUX_12  = GPIO_MUX_12,
    MUX_13  = GPIO_MUX_13,
    MUX_14  = GPIO_MUX_14,
    MUX_15  = GPIO_MUX_15,
    MUX_NUll= 0xff,        //复用模式为空作为普通I/O口使用。
};


//O_开头的是输出需要初始化的   I_  开头的是输入需要初始化化的
typedef struct PIN
{
   //GPIO_A....
   u32 PORT;
   //如PIN1....
   u32 Pinx;
   // GPIO 端口模式寄存器如 GPIO_MODE_IN
   u32 MODER;
   //输出模式开漏或者 推挽输出
   u32 O_TYPER;
   //强度
   u32 O_STRENGTH;
   //上拉下拉等
   u32 PUPD;
   //MUX mode
   u32 MUX;
}Pin;


// =============================================================================
// @brief: GPIO引脚配置，包括引脚的模式、输入类型、速度、上下拉类型等
// @param: port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
// @param: Msk，操作的掩码，如操作的是GPIOA,PIN5，则msk = (1<<5).
// @param: Mode,模式，分为输入、输出、模拟输入、AF复用功能
// @param: OutType,推挽输出或开漏输出
// @param: Speed,速度，如GPIO_SPEED_50M
// @param: PUPD,上拉或下拉
// @retval: 无
// =============================================================================
bool_t GPIO_CfgPinFunc(u32 port, u32 Msk, u32 Mode, u32 OutType, u32 Strength, u32 PUPD);

// =============================================================================
// @brief: 设置利用功能号
// @param: port，被操作的port编号，比如要操作GPIOA,PIN5,则port=GPIO_A
// @param: pinnum,IO号，如GPIOA,PIN5,则pinnum = 5
// @param: af_no，利用功能号
// @retval: 无
// =============================================================================
u32 GPIO_AFSet(u32 port, u32 pinnum, u32 af_no);

// =============================================================================
// @brief: 把某gpio port整体读入
// @param: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
// @retval: 读得的数据
// =============================================================================
u32 GPIO_GetData(u32 port);

// =============================================================================
// @brief: 把数据整体输出到某gpio port
// @param: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
// @param: data，待输出的数据
// @retval: 无
// =============================================================================
void GPIO_OutData(u32 port, u32 data);

// =============================================================================
// @brief: 在port中msk中是1的bit对应的位输出高电平
// @param: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
// @param: mask，port中需要输出高电平的位掩码，比如操作GPIOA,PIN5，msk = (1<<5)
// @retval: 无
// =============================================================================
void GPIO_SettoHigh(u32 port, u32 msk);

// =============================================================================
// @brief: 在port中msk中是1的bit对应的位输出低电平
// @param: port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
// @param: mask，port中需要输出高电平的位掩码,比如操作PIOA,PIN5，msk = (1<<5)
// @retval: 无
// =============================================================================
void GPIO_SettoLow(u32 port, u32 msk);

// =============================================================================
// @brief: 打开GPIOA-GPIOI的时钟输出
// @param: port,端口号，如GPIO_A
// @retval: 无
// =============================================================================
void GPIO_PowerOn(u32 port);

// =============================================================================
// @brief: 为了节能，降低功耗，直接关闭GPIOA-GPIOI的时钟输出
// @param: port,端口号，如GPIO_A
// @retval: 无
// =============================================================================
void GPIO_PowerOff(u32 port);
// =============================================================================
// 功能: 初始化GPIO 引脚
// 参数：struct PIN
// 返回：true/false
// =============================================================================
bool_t PIO_Configure(const Pin *Pin, u32 num);
void PIO_Set(const Pin *Pin);
void PIO_Clear(const Pin *Pin);
#ifdef __cplusplus
}
#endif

#endif /*__cpu_peri_gpio_h_*/
