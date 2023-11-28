#ifndef __SYS_GPIO_H__
#define __SYS_GPIO_H__

typedef struct{
   vu32 CFG[4];
   vu32 DAT;
   vu32 DRV[2];
   vu32 PUL[2];

}GPIO_TypeDef;

typedef enum
{
    GPIO_Mode_IN   = 0x0, /*!< GPIO Input Mode              */
    GPIO_Mode_OUT  = 0x1, /*!< GPIO Output Mode             */
    GPIO_Mode_010  = 0x2,
    GPIO_Mode_011  = 0x3,
    GPIO_Mode_100  = 0x4,
    GPIO_Mode_101  = 0x5,
    GPIO_Mode_110  = 0x6,
    GPIO_Mode_111  = 0x7,
}GPIOMode_TypeDef;
//PA没有上下拉
typedef enum
{
    GPIO_PuPd_NOPULL = 0x00,
    GPIO_PuPd_UP     = 0x01,
    GPIO_PuPd_DOWN   = 0x02
}GPIOPuPd_TypeDef;
/** @defgroup GPIO_pins_define
  * @{
  */
#define GPIO_Pin_0                 0
#define GPIO_Pin_1                 1
#define GPIO_Pin_2                 2
#define GPIO_Pin_3                 3
#define GPIO_Pin_4                 4
#define GPIO_Pin_5                 5
#define GPIO_Pin_6                 6
#define GPIO_Pin_7                 7
#define GPIO_Pin_8                 8
#define GPIO_Pin_9                 9
#define GPIO_Pin_10                10
#define GPIO_Pin_11                11
#define GPIO_Pin_12                12
#define GPIO_Pin_13                13
#define GPIO_Pin_14                14
#define GPIO_Pin_15                15
#define GPIO_Pin_16                16
#define GPIO_Pin_17                17
#define GPIO_Pin_18                18
#define GPIO_Pin_19                19
#define GPIO_Pin_20                20
#define GPIO_Pin_21                21
#define GPIO_Pin_22                22
#define GPIO_Pin_23                23
#define GPIO_Pin_24                24
#define GPIO_Pin_25                25
#define GPIO_Pin_26                26
#define GPIO_Pin_27                27
#define GPIO_Pin_28                28
#define GPIO_Pin_29                29
#define GPIO_Pin_30                30
#define GPIO_Pin_31                31

#define GPIO_Pin_All               0xff

#define CPU_PORT_BASE 0x01C20800
#define GPIOA_BASE  (CPU_PORT_BASE + 0x24*0)
#define GPIOB_BASE  (CPU_PORT_BASE + 0x24*1)
#define GPIOC_BASE  (CPU_PORT_BASE + 0x24*2)
#define GPIOD_BASE  (CPU_PORT_BASE + 0x24*3)
#define GPIOE_BASE  (CPU_PORT_BASE + 0x24*4)
#define GPIOF_BASE  (CPU_PORT_BASE + 0x24*5)


#define GPIOA       ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB       ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC       ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD       ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE       ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOF       ((GPIO_TypeDef *) GPIOF_BASE)

#define GPIODI      0
#define GPIOEI      1
#define GPIOFI      2

//External INTn Mode (n = 0~7)
#define Positive_Edge 0x0
#define Negative_Edge 0x1
#define High_Level    0x2
#define Low_Level     0x3
#define Double_Edge   0x4 //(Positive/ Negative)

/*
GPIO 初始化
*/
void GPIO_Congif
  (
    GPIO_TypeDef * GPIOx, //GPIO
    unsigned int GPIO_Pin, //Pin
    GPIOMode_TypeDef GPIOMode,//模式
    GPIOPuPd_TypeDef GPIO_PuPd//上下拉
  );
void GPIO_SettoHigh(GPIO_TypeDef * GPIOx,unsigned int GPIO_Pin );
void GPIO_SettoLow(GPIO_TypeDef * GPIOx, unsigned int GPIO_Pin );
u8 GPIO_READ(GPIO_TypeDef * GPIOx, unsigned int GPIO_Pin);
/*
GPIO驱动能力设置
Multi_Driving=0-3，0最低，3最高
*/
void GPIO_Multi_Driving
  (
    GPIO_TypeDef * GPIOx, //GPIO
    unsigned int GPIO_Pin, //Pin
    unsigned int Multi_driving //驱动能力
  );
//中断相关
void GPIO_External_Inerrupt_Config(int GPIOi,unsigned int GPIO_Pin,int mode);
void GPIO_External_Inerrupt_Enable(int GPIOi,unsigned int GPIO_Pin);
void GPIO_External_Inerrupt_Disable(int GPIOi,unsigned int GPIO_Pin);
unsigned int GPIO_External_Inerrupt_Status(int GPIOi);


#endif

