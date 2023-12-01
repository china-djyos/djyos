#include "cpu_peri_io.h"
#include "cpu_peri_gpio.h"
#include "reg_ccu.h"

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


/*
GPIO 初始化
*/
void GPIO_Congif(
        GPIO_TypeDef * GPIOx, //GPIO
        unsigned int GPIO_Pin, //Pin
        GPIOMode_TypeDef GPIOMode,//模式
        GPIOPuPd_TypeDef GPIO_PuPd//上下拉
    )
{
    GPIOx->CFG[GPIO_Pin/8] &=~  (0xF<<(GPIO_Pin%8*4));//清0

    if(GPIOMode==GPIO_Mode_IN)
        GPIOx->CFG[GPIO_Pin/8] &= ~ (0x1<<(GPIO_Pin%8*4));//输入模式
    else if(GPIOMode==GPIO_Mode_OUT)
        GPIOx->CFG[GPIO_Pin/8] |=   (0x1<<(GPIO_Pin%8*4));//输出模式
    else
        GPIOx->CFG[GPIO_Pin/8] |=   (GPIOMode<<(GPIO_Pin%8*4));//其它模式

    //上下拉
    GPIOx->PUL[GPIO_Pin/16] &= ~ (0x3<<(GPIO_Pin%16*2));//清0
    GPIOx->PUL[GPIO_Pin/16] |=  (GPIO_PuPd<<(GPIO_Pin%16*2));//写入
}

/*
GPIO输出1
*/
void GPIO_SettoHigh(GPIO_TypeDef * GPIOx,unsigned int GPIO_Pin )
{
    GPIOx->DAT |=   (0x1<<GPIO_Pin);
}
/*
GPIO输出0
*/
void GPIO_SettoLow(GPIO_TypeDef * GPIOx,unsigned int GPIO_Pin )
{
    GPIOx->DAT &= ~ (0x1<<GPIO_Pin);
}
/*
GPIO读 返回0-1
*/
u8 GPIO_READ(GPIO_TypeDef * GPIOx, unsigned int GPIO_Pin )
{
    if(GPIOx->DAT&((1)<<GPIO_Pin))return 1;
    else return 0;
}
/*
GPIO驱动能力设置
Multi_Driving=0-3，0最低，3最高
*/
void GPIO_Multi_Driving
    (
        GPIO_TypeDef * GPIOx, //GPIO
        unsigned int GPIO_Pin, //Pin
        unsigned int Multi_driving //驱动能力
    )
{
    if(Multi_driving>3)Multi_driving=3;
    GPIOx->DRV[GPIO_Pin/16]&= ~ (0x3<<(GPIO_Pin%16*2));//清0
    GPIOx->DRV[GPIO_Pin/16]|=  (Multi_driving<<(GPIO_Pin%16*2));//写入
}
/*
GPIO中断配置
*/
#define PIO_Base_Address (0x01C20800)
void GPIO_External_Inerrupt_Config(int GPIOi,unsigned int GPIO_Pin,int mode)
{
    unsigned int n=GPIO_Pin/8;
    unsigned int y=GPIO_Pin%8;
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+(n*0x4);

    // External Interrupt Configure Register
    write32(addr,read32(addr)&(~(0xf<<(y*4))));//清0
    write32(addr,read32(addr)|(mode<<(y*4)));//写入
//  sysprintf("GPIO_%08x...\r\n",addr);
//  sysprintf("GPIO_%08x...\r\n",read32(addr));
}
/*
GPIO中断使能
*/
void GPIO_External_Inerrupt_Enable(int GPIOi,unsigned int GPIO_Pin)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x10;

    //External Interrupt Control Register
    S_BIT(addr,GPIO_Pin);
//  sysprintf("GPIO_%08x...\r\n",addr);
//  sysprintf("GPIO_%08x...\r\n",read32(addr));
}
/*
GPIO中断失能
*/
void GPIO_External_Inerrupt_Disable(int GPIOi,unsigned int GPIO_Pin)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x10;

    //External Interrupt Control Register
    C_BIT(addr,GPIO_Pin);
}

//获取gpio的中断状态并清中断
// 为1的bit代表该gpio捕获到中断
unsigned int GPIO_External_Inerrupt_Status(int GPIOi)
{
    unsigned int addr=PIO_Base_Address+0x200+(GPIOi*0x20)+0x14;
    unsigned int val=read32(addr);
    write32(addr,val);//Write ‘1’ to clear
    return val;
}


