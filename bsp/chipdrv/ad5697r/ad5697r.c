// =============================================================================

// 文件名     ：ad5697r.c
// 模块描述: 温度检测芯片max31865的驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "iicbus.h"
#include "cpu_peri.h"
#include "ad5697r.h"
//#include "ad5697r_config.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t AD5697R_ModuleInit(void)
//    AD5697R_ModuleInit();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ADC chip ad5697r"//12bit DAC转换
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","iicbus","cpu onchip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ADC_CHIP_AD5697R == false )
//#warning  " ADC_chip_ad5697r  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ADC_CHIP_AD5697R    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0，0xFFFFFFFF,
#define CFG_AD5697R_TIMEOUT        (-1)   //"超时时间",-1表示无穷
//%$#@num,,,
#define CFG_AD5697R_CLK_FRE        (50*1000)            //"总线速度",单位Hz
//%$#@enum,0x01,0x08,0x0D
#define CFG_AD5697R_ADDR           0x0D                 //"设备地址",
//%$#@string,1,10,
#define CFG_AD5697R_BUS_NAME              "I2C0"        //"name",所挂载的总线名称,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
//#define CFG_AD5697R_ADDR           0x0D            //设备地址
//#define CFG_AD5697R_CLK_FRE        (50*1000)      //总线速度，单位Hz
//#define CFG_AD5697R_TIMEOUT        CN_TIMEOUT_FOREVER

static struct SemaphoreLCB AD5697R_Semp;   //芯片互斥访问保护

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *s_ptAD5697R_Dev = NULL;
static u32 s_AD5697R_Timeout = CN_TIMEOUT_FOREVER;
static bool_t sAD5697RInited = false;

//static const Pin AD5697R_LDAC_pin[] = {
//      {PIO_PD25, PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_DEFAULT},
//};

// =============================================================================
void __AD5697R_GpioInit(void)
{
    //一般会将LDAC的引脚配置为低电平，在board.c里面配置
//  PIO_Configure(AD5697R_LDAC_pin,1);
}

// =============================================================================
// 功能：DA2输出
// 参数：DacValue2，DA通道2的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_Output1(u16 DacValue1)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x31;
    DacValue1 = (DacValue1 << 4) & 0xFFF0;
    value[0] = (DacValue1 & 0xFF00) >> 8;
    value[1] = (DacValue1 & 0x00FF) >> 0;

    IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：DA1输出
// 参数：DacValue1，DA通道1的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_Output2(u16 DacValue2)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    cmd = 0x38;
    DacValue2 = (DacValue2 << 4) & 0xFFF0;
    value[0] = (DacValue2 & 0xFF00) >> 8;
    value[1] = (DacValue2 & 0x00FF) >> 0;

    IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：DA输出，两通道同时配置
// 参数：DacValue1，DA通道1的DA值
//      DacValue2，DA通道2的DA值
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_OutputAll(u16 DacValue1,u16 DacValue2)
{
    u8 cmd,value[2];

    if(!sAD5697RInited)
    {
        return false;
    }

    if(false == Lock_SempPend(&AD5697R_Semp,25*mS))
    {
        return false;
    }

    if(DacValue1 == DacValue2)
    {
        cmd = 0x39;
        DacValue2 = (DacValue2 << 4) & 0xFFF0;
        value[0] = (DacValue2 & 0xFF00) >> 8;
        value[1] = (DacValue2 & 0x00FF) >> 0;

        IIC_Write(s_ptAD5697R_Dev,cmd,value,2,true,s_AD5697R_Timeout);
    }
    else
    {
        AD5697R_Output1(DacValue1);
        AD5697R_Output2(DacValue2);
    }

    Lock_SempPost(&AD5697R_Semp);
    return true;
}

// =============================================================================
// 功能：芯片初始化
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t AD5697R_ModuleInit(void)
{
    //GPIO初始化，SDA、SCL已经在IIC中初始化了，此处只需初始化WP即可
    __AD5697R_GpioInit();

    if(NULL == Lock_SempCreate_s(&AD5697R_Semp,1,1,CN_BLOCK_FIFO,"AD5697R_semp"))
        return false;

    //添加FM24CL64到IIC0总线
    s_ptAD5697R_Dev = IIC_DevAdd(CFG_AD5697R_BUS_NAME,"AD5697R",CFG_AD5697R_ADDR,0,8);
    if(s_ptAD5697R_Dev)
    {
        IIC_BusCtrl(s_ptAD5697R_Dev,CN_IIC_SET_CLK,CFG_AD5697R_CLK_FRE,0);
        IIC_BusCtrl(s_ptAD5697R_Dev,CN_IIC_SET_POLL,0,0);

        sAD5697RInited = true;
    }
    else
    {
        Lock_SempDelete_s(&AD5697R_Semp);
        sAD5697RInited = false;
    }
    return sAD5697RInited;
}
