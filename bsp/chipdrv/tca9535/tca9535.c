#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "os.h"
#include "cpu_peri.h"
#include "iicbus.h"
#include "tca9535.h"
#include "board-config.h"
#include <time.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t TCA9535_Init(void);
//    TCA9535_Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"tca9535"//IIC接口的16口IO扩展
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
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
#if ( CFG_MODULE_ENABLE_TCA9535 == false )
//#warning  " tca9535  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TCA9535    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,100000,400000,
#define CFG_TCA9535_CLK_FRE             (100*1000)      //"总线速度"，TCA9535的总线速度
//%$#@enum,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,
#define CFG_TCA9535_ADDR                (0x20)          //"设备地址",设备的总线地址
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_TCA9535_BUS_NAME              "IIC0"            //"总线名称",设备所在总线的名称
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


static struct IIC_Device *pg_TCA9535_Dev=NULL;
struct MutexLCB *  ptSemID_TCA9535;       // IIC操作互斥标志
//#define CFG_TCA9535_CLK_FRE             (100*1000)      //总线速度，单位Hz

//#define CFG_TCA9535_ADDR (0x20) //这和板件硬件连接有关，到时候放到板件里面定义

//芯片8个寄存器的内部地址
#define TCA9535_REG_InputPort0  0x00
#define TCA9535_REG_InputPort1  0x01
#define TCA9535_REG_OutputPort0 0x02
#define TCA9535_REG_OutputPort1 0x03
#define TCA9535_REG_PolarityInversion0  0x04
#define TCA9535_REG_PolarityInversion1  0x05
#define TCA9535_REG_Configuration0  0x06
#define TCA9535_REG_Configuration1  0x07

static s32 timeout = 1000*mS;                 //1000*mS;

//全局变量，对寄存器设置用（因为每次只改一个IO口，而写入寄存器需要写8位）
static u8 p0conf=255;//1111 1111八个p0口初始全设定为输入
static u8 p1conf=255;
static u8 p0out=255;//1111 1111八个p0口初始值全输出高电平
static u8 p1out=255;




// =============================================================================
// 功能：读指定地址开始Dstlen长度的数据
// 参数：
//       TCA9535_DstAddr，目标存储器预读取数据的地址
//       pDstBuf，存放接收到的数据缓存地址
//       DstLen， 预读取数据的长度,为一个字节
// 返回：ret, 实际读到的字节数
// =============================================================================
static s32 TCA9535_Read_Data(u32 TCA9535_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    s32 ret = 0;

    ret=IIC_Read(pg_TCA9535_Dev, TCA9535_DstAddr, pDstBuf, DstLen, timeout);
    return ret;
}

// =============================================================================
// 功能：写指定地址开始Dstlen长度的数据
// 参数：
//      TCA9535_DstAddr，目标存储器预写数据地址
//      pSrcBuf，发送数据缓存地址
//      SrcLen， 发送数据长度，一般为一个字节
// 返回：ret, 实际写入的字节数
// =============================================================================
static s32 TCA9535_Write_Data(u32 TCA9535_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    s32 ret = 0;

    ret=IIC_Write(pg_TCA9535_Dev, TCA9535_DstAddr, pSrcBuf, SrcLen,0, timeout);
    return ret;
}


// =============================================================================
// 功能：设置每个IO口的输入输出
// 参数：
//      portid，IO口的名字，.h文件中的枚举量，如port03，port04
//      mode，0为输出（output），1为输入（input）
// 返回：true or false
// =============================================================================
bool_t TCA9535_PortConfigure(u8 portid,bool_t mode)
{

    s32 ret=0;
    u8 *p0;
    u8 *p1;

    p0=&p0conf;//指向8位数值的指针
    p1=&p1conf;

    if(portid<8)//说明是P0端口
    {
        if(mode==0)//模式为输出
        {
            p0conf&=(~(1<<portid));//8位全局变量对应8个IO口，对应的端口值置0
        }
        if(mode==1)//输入
        {
            p0conf|=(1<<portid);//对应的端口值置1
        }
        ret = TCA9535_Write_Data(TCA9535_REG_Configuration0,p0,1);//配置寄存器，配置IO引脚的方向
    }
    if(portid>9)//说明是P1端口
    {
        portid-=10;
        if(mode==0)//模式为输出
        {
            p1conf&=(~(1<<portid));//对应的端口值置0
        }
        if(mode==1)//输入
        {
            p1conf|=(1<<portid);//对应的端口值置1
        }
        portid+=10;
        ret = TCA9535_Write_Data(TCA9535_REG_Configuration1,p1,1);
    }

    if(ret==-1)
    return false;
    return true;
}


// =============================================================================
// 功能：读取期望的IO口值
// 参数：portid，IO口的名字，在.h文件中的枚举量，如port03，port04
// 返回：true or -1
// =============================================================================
s32 TCA9535_PortRead(u8 portid)
{
    u8 data;
    s32 ret;
    u8 *buf=&data;

    if(portid<8)//说明是P0端口
    {
        if(0==(p0conf&(1<<portid)))//该IO口设置为输出模式，无法读取
            return -1;
        ret=TCA9535_Read_Data(TCA9535_REG_InputPort0,buf,1);//读取相关寄存器，值存在buf指针指向的data中
        if(ret==-1)
            return -1;
        data&=(1<<portid);//只看想看的那一个IO口
        if(data>0)
        return 1;
        if(data==0)
        return 0;
    }
    if(portid>9)//说明是P1端口
    {
        if(0==(p1conf&(1<<(portid-10))))//该IO口设置为输出模式，无法读取
            return -1;
        ret=TCA9535_Read_Data(TCA9535_REG_InputPort1,buf,1);//读取相关寄存器，值存在buf指针指向的data中
        if(ret==-1)
            return -1;
        data&=(1<<(portid-10));//只看想看的那一个IO口
        if(data>0)
        return 1;
        if(data==0)
        return 0;
    }
    return -1;
}

// =============================================================================
// 功能：命令指定IO口输出期望的电平
// 参数：portid，IO口的名字，在.h文件中的枚举量，如port03，port04
//      value，0为低电平，1为高电平
// 返回：true or -1
// =============================================================================
s32 TCA9535_PortWrite(u8 portid,bool_t value)
{
    s32 ret;


    if(portid<8)//说明是P0端口
    {
        if(1==(p0conf&(1<<portid)))//该位置为1说明是输入端口，无法令其输出
            return -1;
        if(value==1)//输出高电平
            p0out|=(1<<portid);//相应的位置置1
        if(value==0)//输出低电平
            p0out&=(~(1<<portid));//相应的位置置0
        ret=TCA9535_Write_Data(TCA9535_REG_OutputPort0,&p0out,1);//把设置好的8位数值写入对应寄存器
        if(ret==-1)
            return -1;
    }
    if(portid>9)//说明是P0端口
    {
        portid-=10;
        if(1==(p1conf&(1<<portid)))//该位置为1说明是输入端口，无法令其输出
            return -1;
        if(value==1)//输出高电平
            p1out|=(1<<portid);//相应的位置置1
        if(value==0)//输出低电平
            p1out&=(~(1<<portid));//相应的位置置0
        ret=TCA9535_Write_Data(TCA9535_REG_OutputPort1,&p1out,1);//把设置好的8位数值写入对应寄存器
        if(ret==-1)
            return -1;

    }
    return true;
}


bool_t text(void)
{
//  s8 value;
    u8 p0=0;
    u8 p1=255;

//  TCA9535_Write_Data(TCA9535_REG_Configuration0,&p1,8);
//  TCA9535_Write_Data(TCA9535_REG_OutputPort0,&p0,8);

    TCA9535_Write_Data(TCA9535_REG_Configuration1,&p0,1);

//  IIC_Write(pg_TCA9535_Dev, 0x07, &p0, 1,0, timeout);
//    TCA9535_PortWrite(port14,1);

    TCA9535_Write_Data(TCA9535_REG_OutputPort1,&p1,1);



    return 1;
}
// =============================================================================
// 功能：初始化tca9535芯片，调用IIC驱动初始化函数
// 参数:
// 返回: true= 成功，false=失败
// =============================================================================
/* =============================================================================
 功能：tca9535芯片初始化，初始化和加载设备到对应的IIC总线.
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/

bool_t TCA9535_Init(void)
{
    ptSemID_TCA9535 = Lock_MutexCreate("TCA9535 Lock");
    if(!ptSemID_TCA9535)
    {
        printf("\r\n: error : device : TAC9535 initialization failed<lock>.");
        return (FALSE);
    }

    pg_TCA9535_Dev = IIC_DevAdd(CFG_TCA9535_BUS_NAME, "TCA9535", CFG_TCA9535_ADDR, 0, 8);
    if(!pg_TCA9535_Dev)
    {
        Lock_MutexDelete(ptSemID_TCA9535);
        printf("\r\n: error : device : TAC9535 initialization failed<device>.");
        return (FALSE);
    }

    IIC_BusCtrl(pg_TCA9535_Dev,CN_IIC_SET_CLK,CFG_TCA9535_CLK_FRE,0);//设置时钟大小
    IIC_BusCtrl(pg_TCA9535_Dev,CN_IIC_SET_POLL,0,0);       //使用方式发送
    printf("\r\n: error : device : TAC9535 added.");
    return (TRUE);
}
