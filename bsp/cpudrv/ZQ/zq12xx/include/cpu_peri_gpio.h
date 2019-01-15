

#ifndef CPU_PERI_GPIO_H
#define CPU_PERI_GPIO_H

//Gpio 外部中断处理函数

typedef u32 (*fnGpioHandle_t)(u32);

//触发方式的选择

enum EN_TRIGLE_METHOD
{
    EN_EDGE = 0x00, //上升沿或者下降沿触发
    EN_LEVEL,       //电平触发
};

//杈规部瑙﹀彂鏂瑰紡

enum EN_EDGE_TRIGLE_METHOD
{
	EN_LOW_LEVEL_EDGE,   //低电平触发
	EN_HIGHT_LAVEL_EDGE, //高电平触发
    EN_BOTH_EDGE,   	 //双边沿触发
    EN_RISING_EDGE, 	 //上升沿触发
    EN_FALLING_EDGE,	 //下降沿触发
};


//all pins listed as follow - io

#define    IO_PIN_PA0         0
#define    IO_PIN_PA1         1
#define    IO_PIN_PA2         2
#define    IO_PIN_PA3         3
#define    IO_PIN_PA4         4
#define    IO_PIN_PA5         5
#define    IO_PIN_PA6         6
#define    IO_PIN_PA7         7
#define    IO_PIN_PB0         8
#define    IO_PIN_PB1         9
#define    IO_PIN_PB2        10
#define    IO_PIN_PB3        11
#define    IO_PIN_PB4        12
#define    IO_PIN_PB5        13
#define    IO_PIN_PB6        14
#define    IO_PIN_PB7        15
#define    IO_PIN_PB8        16
#define    IO_PIN_PC0        17
#define    IO_PIN_PC1        18
#define    IO_PIN_PC2        19
#define    IO_PIN_PC3        20
#define    IO_PIN_PC4        21
#define    IO_PIN_PC5        22
#define    IO_PIN_PC6        23
#define    IO_PIN_PC7        24
#define    IO_PIN_PC8        25
#define    IO_PIN_PC9        26
#define    IO_PIN_PC10       27
#define    IO_PIN_PC11       28
#define    IO_PIN_PC12       29
#define    IO_PIN_PC13       30
#define    IO_PIN_PD0        31
#define    IO_PIN_PD1        32
#define    IO_PIN_PD2        33
#define    IO_PIN_PD3        34
#define    IO_PIN_PD4        35
#define    IO_PIN_PD5        36
#define    IO_PIN_PD6        37
#define    IO_PIN_PD7        38
#define    IO_PIN_PD8        39
#define    IO_PIN_PD9        40
#define    IO_PIN_PD10       41

//Config io above to input mode

extern void Gpio_Input_Config(u8 io);

//Config io above to output mode

extern void Gpio_Output_Config(u8 io);

//set io to Hight Level

extern void Gpio_Set_Value_Hight(u8 io);

//set io to Low Level

extern void Gpio_Set_Value_Low(u8 io);

//get io Level

extern u8 Gpio_Get_Level(u8 io);

//enbale io int

extern void Gpio_Enable_Int(u8 io);

//disable io int

extern void Gpio_Disable_Int(u8 io);

//clear io irq

extern void Gpio_Clear_Irq(u8 io);

//set io irq mode

extern void Gpio_Set_Iqr_Mode(u8 io,u8 trugleMethod,u8 edgeTrugleMethod);


//设置io 虑波分频

extern  void Gpio_Io_Filter_Config_Div(u8 io,u8 div);

//启动io 虑波功能

extern void Gpio_io_Filter_Open(u8 io);

//不使用IO虑波功能

extern void Gpio_io_Filter_Close(u8 io);

//io 外部中断注册申请

extern void Register_Gpio_Irq(u8 io,fnGpioHandle_t IsrHandle);

#endif
































