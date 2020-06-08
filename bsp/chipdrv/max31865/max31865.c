// =============================================================================

// 文件名     ：max31865.c
// 模块描述: 温度检测芯片max31865的驱动
// 模块版本: V1.00
// 创建人员:
// 创建时间: 1/20.2017
// =============================================================================

#include "string.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max31865.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Max31865(void);
//    ModuleInstall_Max31865();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"max31865"//spi接口的测温芯片
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
#if ( CFG_MODULE_ENABLE_MAX31865 == false )
//#warning  " max31865  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MAX31865    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,100000,5000000,
#define CFG_MAX3_SPI_SPEED                 (200*1000)//"时钟速度"，配置max31865所使用的spi总线速度
//%$#@num,100000,0xFFFFFFFF,
//%$#@enum,MAX31865_CONTINOUS_MODE,MAX31865_SINGLE_MODE,
#define CFG_MAX31865_SAM_MODE              MAX31865_CONTINOUS_MODE   //"采样模式",连续采样或单次采样
//%$#@string,1,10,
#define CFG_MAX31865_BUS_NAME              "SPI0"       //"SPI总线",MAX31865所使用SPI总线名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_MAX3_TIMEOUT                   CN_TIMEOUT_FOREVER//"超时时间"，即等待超时间
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static struct SPI_Device *s_ptMax3_Dev;
static u32 s_Max3_Timeout = CFG_MAX3_TIMEOUT;
//#define CFG_MAX3_SPI_SPEED      (200*1000)
static bool_t sMax3Inited = false;


static struct SemaphoreLCB s_tMax3_Semp;    //芯片互斥访问保护
static u8 MaxSampleMode = MAX31865_CONTINOUS_MODE;

//extern bool_t Max31865_DataReady(void);         //与板件相关，在board.c中
//max31865  温度
//static const Pin Max3_pin[] = {
//      {GPIO_A,PIN4,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//cs
//      {GPIO_A,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_A,PIN6,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_B,PIN5,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_PU,AF5},
//      {GPIO_I,PIN11,GPIO_MODE_IN,GPIO_OTYPE_PP,GPIO_SPEED_H,GPIO_PUPD_NONE,AF_NUll},//data ready
//};

__attribute__((weak)) bool_t Max31865_DataReady(void)
{
    return true;//Max31865数据Ready 信号读取
}
static bool_t Max31865_Ready(u32 WaitTime)
{
    u32 timecnt = 0;
    while(timecnt < WaitTime)
    {
        DJY_DelayUs(10);
//      if(!PIO_Get(&Max3_pin[4]))
        if(!Max31865_DataReady())
            return true;
        timecnt += 10;
    }
    return false;
}

static void Max31865_CsActive(void)
{
    SPI_CsActive(s_ptMax3_Dev,s_Max3_Timeout);
//  PIO_Clear(&Max3_pin[0]);
}
static void Max31865_CsInActive(void)
{
    SPI_CsInactive(s_ptMax3_Dev);
//  PIO_Set(&Max3_pin[0]);
}
static u32 Max31865_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = RecvOff;
    data.SendBuf = sdata;
    data.SendLen = slen;

    result = SPI_Transfer(s_ptMax3_Dev,&data,true,s_Max3_Timeout);
    if(result != CN_SPI_EXIT_NOERR)
        return 0;
    return 1;
}

// =============================================================================
// 函数功能：触发一次采样
// 输入参数：无
// 输出参数：无
// 返回值  ：true，配置成功；false，配置不成功
// =============================================================================
static bool_t Max31865_TrigSingleSample(void)
{
    u8 cfgData[] = {0x80,0xA0};
    u8 cfgRead = 0x00,cfgcnt = 0;
    bool_t ret = false;

    do
    {
        cfgData[0] = 0x80;
        cfgData[1] = 0xA0;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        DJY_EventDelay(15*1000);
        Max31865_CsInActive();

        cfgData[0] = 0x00;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,1,&cfgRead,1,1);
        Max31865_CsInActive();
        if(cfgRead == 0xA0)
        {
            ret = true;
            break;
        }
        cfgcnt ++;
        if(cfgcnt == 20)
        {
            break;
        }
        DJY_EventDelay(1*mS);
    }while(1);

    return ret;
}

// =============================================================================
// 函数功能：配置Max采样为连续采样模式
// 输入参数：无
// 输出参数：无
// 返回值  ：true，配置成功；false，配置不成功
// =============================================================================
static bool_t Max31865_ContinousModeCfg(void)
{
    u8 cfgData[] = {0x80,0xC1};
    u8 cfgRead = 0x00,cfgcnt = 0;
    bool_t ret = false;

    do
    {
        cfgData[0] = 0x80;
        cfgData[1] = 0xC1;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        DJY_EventDelay(15*mS);
        Max31865_CsInActive();

        cfgData[0] = 0x00;
        Max31865_CsActive();
        Max31865_TxRx(cfgData,1,&cfgRead,1,1);
        Max31865_CsInActive();
        if(cfgRead == 0xC1)
        {
            ret = true;
            break;
        }
        cfgcnt ++;
        if(cfgcnt == 20)
        {
            break;
        }
        DJY_EventDelay(10*mS);
    }while(1);

    return ret;
}

// =============================================================================
// 函数功能：判断是否有错误，并清错误标记
// 输入参数：无
// 输出参数：无
// 返回值  ：true，有错误；false，无错误
// =============================================================================
static bool_t Max31865_FaultAndClear(u8 *Reg)
{
    bool_t ret = false;
    u8 cfgData[2];

    if(Reg[7] & 0x04)                   //错误标记
    {
        cfgData[0] = 0x80;
        cfgData[1] = Reg[0] & (~((1<<5) | (3<<2))); //清标记
        cfgData[1] |= (1<<1);
        Max31865_CsActive();
        Max31865_TxRx(cfgData,2,NULL,0,0);
        Max31865_CsInActive();
        ret = true;
    }
    return ret;
}
// =============================================================================
// 函数功能：读温度(注意：函数类型为float，必须声名，否则返回值出错)
// 输入参数：无
// 返回值  ：温度值，为float类型；-1，表示出错
// =============================================================================
float Max31865_TemperatureGet(void)
{
    float temp;
    u8 RegMatrix[8] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07};
    u8 Data[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

    if(!sMax3Inited)
    {
        return -1;
    }

    if(false == Lock_SempPend(&s_tMax3_Semp,25*mS))
    {
        return -1;
    }

    if(MaxSampleMode == MAX31865_SINGLE_MODE)
    {
        if(false == Max31865_TrigSingleSample())
        {
            Lock_SempPost(&s_tMax3_Semp);
            return -1;
        }
    }

    if(false == Max31865_Ready(500000))       //查忙，若超时则返回false
    {
        Lock_SempPost(&s_tMax3_Semp);
        return -1;
    }

    Max31865_CsActive();
    Max31865_TxRx(RegMatrix,8,Data,8,1);
    Max31865_CsInActive();

    if(Max31865_FaultAndClear(Data))
    {
        Lock_SempPost(&s_tMax3_Semp);
        return -1;
    }

    Lock_SempPost(&s_tMax3_Semp);

    //计算温度
    temp = ((Data[1] <<8) + Data[2]) >> 1;
    temp = (temp / 32 - 256);

    return temp;
}

// =============================================================================
//功能：初始化Max31865芯片，若采用连续读模式，则配置其模式为连续采样
//参数：参数：
//     BusName,总结名称，如“SPI1”
//     Mode，连续模式或单次采样模式,
//返回：true = 成功初始化，false = 初始化失败
// =============================================================================
bool_t ModuleInstall_Max31865(void)
{
//  PIO_Configure(Max3_pin,PIO_LISTSIZE(Max3_pin));
//  PIO_Set(&Max3_pin[0]);

    if(NULL == Lock_SempCreate_s(&s_tMax3_Semp,1,1,CN_BLOCK_FIFO,"MAX31865_semp"))
        return false;

    if(s_ptMax3_Dev = SPI_DevAdd(CFG_MAX31865_BUS_NAME,"MAX31865",0,8,SPI_MODE_1,SPI_SHIFT_MSB,CFG_MAX3_SPI_SPEED,false))
    {
        SPI_BusCtrl(s_ptMax3_Dev,CN_SPI_SET_POLL,0,0);
        if(CFG_MAX31865_SAM_MODE == MAX31865_CONTINOUS_MODE)
        {
            MaxSampleMode = MAX31865_CONTINOUS_MODE;
            if(Max31865_ContinousModeCfg())
            {
                sMax3Inited = true;
            }
        }
        else
        {
            MaxSampleMode = MAX31865_SINGLE_MODE;
            sMax3Inited = true;
        }
        return true;
    }
    else
    {
        printf("MAX31865 init failed.\n\r");
        return false;
    }

    return sMax3Inited;
}
