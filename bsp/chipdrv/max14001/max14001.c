// =============================================================================

// 文件名     ：max11410.c
// 模块描述: AD芯片max14001的驱动
// 模块版本: V1.00
// 创建人员: yt
// 创建时间:
// =============================================================================
#include "string.h"
#include "stdio.h"
#include "os.h"
#include "spibus.h"
#include "cpu_peri.h"
#include "max14001.h"
#include "math.h"
#include "cpu_peri_gpio.h"
//#include "Threshold.h"
//#include "MAX14001Cfg.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Max14001(void);
//    ModuleInstall_Max14001();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"max14001"//spi接口10bit隔离ADC
//parent                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus","lock","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_MAX14001 == false )
//#warning  " max14001  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MAX14001    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//static struct SemaphoreLCB Max_Semp;       //芯片互斥访问保护
//static struct SPI_Device Max14001_Dev;   //SPI总线器件结构体
//static bool_t sMaxInited = false;
#define Max14001_TIMEOUT        (5*1000)     //Max14001 超时配置

//extern  KI_Threshold    *WB7363_KI_CFG;
//extern  SPI_Device_CFG  *WB7363_SPI_DEV;

// =============================================================================
// 功能：Max14001与CPU进行数据交互的接口函数，采用SPI通信，硬件注意拉高拉低片选
// 参数：   wBuf,写缓冲区
//      wLen,写数据长度，字节单位
//      rBuf,读缓冲区
//      rLen,读字节量
//      port，被操作的port编号，比如要操作GPIOA,则port=GPIO_A
//      mask，port中需要输出高电平的位掩码,比如操作PIOA,PIN5，msk = (1<<5)
// 返回：无
// =============================================================================
u16  Max14001_TxRx(u16 data,u8 addr,u8 rdflag,u32 timeout,struct SPI_Device Max14001_Dev)
{
    struct SPI_DataFrame Data;
    u8 buf[2];
    u8 Rcvbuf[2];

    buf[0] = (u8)(data&0xff);                            //第一个字节
    buf[1] = (u8)(((data>>8)&(0x3))|((addr&0x1f)<<3));   //第二个字节

    if(rdflag)
    {
        buf[1] &=~(1<<2);  //R置0
        Data.RecvLen = 2;  //接收长度2字节
    }
    else
    {
       buf[1] |= (1<<2);   //W置1
       Data.RecvLen = 0;   //不接收
    }

    Data.SendBuf = buf;
    Data.SendLen = 2;
    Data.RecvBuf = Rcvbuf;
    Data.RecvOff = 0;

    SPI_CsActive(&Max14001_Dev,CN_TIMEOUT_FOREVER);
    DJY_DelayUs(10);
    SPI_Transfer(&Max14001_Dev,&Data,true,Max14001_TIMEOUT);  //数据传送函数，完成数据的发送和接收。
    DJY_DelayUs(3);
    SPI_CsInactive(&Max14001_Dev);

    if(rdflag)
    {
        SPI_CsActive(&Max14001_Dev,CN_TIMEOUT_FOREVER);
        DJY_DelayUs(10);
        SPI_Transfer(&Max14001_Dev,&Data,true,Max14001_TIMEOUT);  //数据传送函数，完成数据的发送和接收。
        DJY_DelayUs(3);
        SPI_CsInactive(&Max14001_Dev);

//        printf("addr=0x%x , rcvdata =0x%x \n\r",addr,(u16)(Rcvbuf[0]|((Rcvbuf[1]&(0x3))<<8)));
//        DJY_EventDelay(100*mS);
        return (u16)(Rcvbuf[0]|((Rcvbuf[1]&(0x3))<<8));
    }
    else
        return 0;
}

#if 0
u32 Max14001_TxRx1(u8* sBuf,u16 sLen,u8* rBuf,u16 rLen,u32 recvoff)
{
    struct SPI_DataFrame data;
    s32 result;

    data.SendBuf = sBuf;      //接收数据指针
    data.SendLen = 2;         //接收数据长度，字节
    data.RecvBuf = rBuf;      //发送数据指针
    data.RecvLen = rLen;      //发送数据长度，字节
    data.RecvOff = recvoff;   //接收数据偏移

    SPI_CsActive(&Max14001_Dev,1000);
    result =SPI_Transfer(&Max14001_Dev,&data,true,Max14001_TIMEOUT);  //数据传送函数，完成数据的发送和接收。
    SPI_CsInactive(&Max14001_Dev);
    if(result != CN_SPI_EXIT_NOERR)
            return 0;
    return 1;
}
#endif

//=============================================================================
//功能：获取某一引脚电平状态
//参数：port
//    pin
//返回：1-高电平
//    0-低电平
//=============================================================================
u16 Get_Data(u32 port,u16 pin)
{
    u16 data;
    data=GPIO_GetData(port);
    data=(data>>(pin-1))&1;   //从右往左数（低位往高位数）
    return data;
}
//=============================================================================
//功能：设置Max14001默认初始值
//参数：参数：
//返回：true = 成功初始化，false = 初始化失败
//=============================================================================
u16 max_data[0x20];
bool_t Max14001_Config(u16 thl,u16 thu,struct SPI_Device Max14001_Dev)
{
        u16 readdate;
        bool_t result;
        DJY_EventDelay(100*mS);

        //1，READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
        DJY_EventDelay(10);

        //2，WRITEN WEN
        Max14001_TxRx(0x294,0x0c,0,10000,Max14001_Dev);     //写WEN=0x294
        DJY_EventDelay(10);

        //3，DISABLE FAULT MV
        Max14001_TxRx(0x0ff,0x03,0,10000,Max14001_Dev);     //写FLTEN=0x0ff  EMV=0 对应fault不输出
        DJY_EventDelay(10);

        //4，WRITE CONFIGURATION REGISTERS
        Max14001_TxRx(0x0ff,0x03,0,10000,Max14001_Dev);     //写FLTEN 0x0ff
        DJY_EventDelay(10);
        Max14001_TxRx(thl,0x04,0,10000,Max14001_Dev);       //写THL
        DJY_EventDelay(10);
        Max14001_TxRx(thu,0x05,0,10000,Max14001_Dev);       //写THU
        DJY_EventDelay(10);
        Max14001_TxRx(0x20d,0x09,0,10000,Max14001_Dev);     //写CFG
        DJY_EventDelay(10);
        Max14001_TxRx(0x0c0,0x06,0,10000,Max14001_Dev);     //写INRR
        DJY_EventDelay(10);
        Max14001_TxRx(0x180,0x07,0,10000,Max14001_Dev);     //写INRT
        DJY_EventDelay(10);
        Max14001_TxRx(0x1d8,0x08,0,10000,Max14001_Dev);     //写INRP
        DJY_EventDelay(10);

        //5，WRITE VERIFICATION REGISTERS
        Max14001_TxRx(0x0ff,0x13,0,10000,Max14001_Dev);     //写FLTV=FLTEN
        DJY_EventDelay(10);
        Max14001_TxRx(thl,0x14,0,10000,Max14001_Dev);       //写THLV=THL
        DJY_EventDelay(10);
        Max14001_TxRx(thu,0x15,0,10000,Max14001_Dev);       //写THUV=THU
        DJY_EventDelay(10);
        Max14001_TxRx(0x20d,0x19,0,10000,Max14001_Dev);     //写CFGV=CFG
        DJY_EventDelay(10);
        Max14001_TxRx(0x0c0,0x16,0,10000,Max14001_Dev);     //写INRRV=INRR
        DJY_EventDelay(10);
        Max14001_TxRx(0x180,0x17,0,10000,Max14001_Dev);     //写INRTV=INRT
        DJY_EventDelay(10);
        Max14001_TxRx(0x1d8,0x18,0,10000,Max14001_Dev);     //写INRPV=INRP
        DJY_EventDelay(10);

        Max14001_TxRx(0x010,0x0a,0,10000,Max14001_Dev);     //写ENBL
        Max14001_TxRx(0x010,0x1a,0,10000,Max14001_Dev);     //写ENBLV

        //6，ENABLE FALUT MV(WRITE FLTEN AND FLTV)
        Max14001_TxRx(0x1ff,0x03,0,10000,Max14001_Dev);   //写FLTEN=0x1ff
        DJY_EventDelay(10);
        Max14001_TxRx(0x1ff,0x13,0,10000,Max14001_Dev);   //写FLTV=0x1ff
        DJY_EventDelay(10);

        //7，WRITE WEN
        Max14001_TxRx(0x000,0x0c,0,10000,Max14001_Dev);   //写WEN=0x000  之后不能写
        DJY_EventDelay(10);

        //8，READ BACK CONFIGURATION REGISTERS
        readdate=Max14001_TxRx(0x000,0x03,1,10000,Max14001_Dev);     //读FLTEN
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x04,1,10000,Max14001_Dev);     //读THL
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x05,1,10000,Max14001_Dev);     //读THU
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x09,1,10000,Max14001_Dev);     //读CFG=0x203
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x06,1,10000,Max14001_Dev);     //读INRR
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x07,1,10000,Max14001_Dev);     //读INRT
        DJY_EventDelay(10);
        readdate=Max14001_TxRx(0x000,0x08,1,10000,Max14001_Dev);     //读INRP
        DJY_EventDelay(10);

//        readdate1=Max14001_TxRx(0x000,0x13,1,10000);     //读FLTEN
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x14,1,10000);     //读THL
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x15,1,10000);     //读THU
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x19,1,10000);     //读CFG=0x203
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x16,1,10000);     //读INRRV
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x17,1,10000);     //读INRTV
//        DJY_EventDelay(10);
//        readdate1=Max14001_TxRx(0x000,0x18,1,10000);     //读INRPV
//        DJY_EventDelay(10);

        //9，READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
        DJY_EventDelay(10);
        //10，READ FLAGS
        readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
        DJY_EventDelay(10);

        if(readdate==0x000)
        {
            //11,WRITE WEN
            Max14001_TxRx(0x294,0x0c,0,10000,Max14001_Dev);   //写WEN=0x294
            //12,WRITE ENBL:ENA=1
            Max14001_TxRx(0x010,0x0a,0,10000,Max14001_Dev);   //写ENBL
            Max14001_TxRx(0x010,0x1a,0,10000,Max14001_Dev);   //写ENBLV
            //13，READ FLAGS
            readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
            //14，READ FLAGS
            readdate=Max14001_TxRx(0,0x02,1,10000,Max14001_Dev);   //读FLAGS
            if(readdate==0x000)
            {
                //15，WRITE WEN
                Max14001_TxRx(0x000,0x0c,0,10000,Max14001_Dev);   //写WEN=0x000  之后不能写
                result=true;
            }
        }
        else
        {
            printf("Max14001初始化失败\r\n");
            result=false;
        }

        //把寄存器全读出来检查一遍
        for(u8 i=0;i<0x1b;i++)
        {
            max_data[i] = Max14001_TxRx(0x000,i,1,10000,Max14001_Dev);
            printf("addr=0x%x , rcvdata =0x%x \n\r",i,(u16)max_data[i]);
        }
        return result;
}

//=============================================================================
//功能：更新采样值至全局数组
//参数：
//返回：
//=============================================================================
extern u16 COUT_GPIO_CFG[18];
extern u16 COUT_PIN_CFG[18];

u16 MAX14001_ADC_BUFF[18];   //存放一次18路COUT值

void ADC_SAMPLE_UPDATA(void)
{
    for(int i=0;i<18;i++)
    {
        MAX14001_ADC_BUFF[i]=Get_Data(COUT_GPIO_CFG[i],COUT_PIN_CFG[i]);
    }
}

//=============================================================================
//功能：初始化Max14001芯片
//参数：参数：
//    BusName,总结名称，如“SPI1”
//返回：true = 成功初始化，false = 初始化失败
//=============================================================================
extern struct SPI_Device Max14001_Dev[18];

extern u16 WB7363_KI_THL_CFG[18];
extern u16 WB7363_KI_THU_CFG[18];

extern u16 WB7363_SPI_DEV_AUTOCS[18];
extern u16 WB7363_SPI_DEV_LEN[18];
extern u16 WB7363_SPI_DEV_SPI_MODE[18];
extern u16 WB7363_SPI_DEV_SPI_SHIFT[18];
extern u32 WB7363_SPI_DEV_SPI_FREQ[18];

extern char *WB7363_SPI_DEV_SPI_NAME[18];

bool_t ModuleInstall_Max14001(void)
{
    u32 gNUM=0;
    u16 THL_Value;
    u16 THU_Value;

    if(18==18)
    {
        for(u32 t=0;t<18;t++)
        {
           THL_Value=WB7363_KI_THL_CFG[t];
           THU_Value=WB7363_KI_THU_CFG[t];

           Max14001_Dev[t].AutoCs = WB7363_SPI_DEV_AUTOCS[t];       //非自动片选，发数据时通过自己手动片选
           Max14001_Dev[t].CharLen = WB7363_SPI_DEV_LEN[t];         //数据长度8位
           Max14001_Dev[t].Mode = WB7363_SPI_DEV_SPI_MODE[t];       //clk空闲为低电平第一个跳变沿采集/输出数据
           Max14001_Dev[t].ShiftDir = WB7363_SPI_DEV_SPI_SHIFT[t];  //低位优先，不同于其他芯片
           Max14001_Dev[t].Freq = WB7363_SPI_DEV_SPI_FREQ[t];       //速率
           Max14001_Dev[t].Cs = t%6 ;

           if((gNUM>=0)&&(gNUM<=5))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI1",WB7363_SPI_DEV_SPI_NAME[t]);     //1-6 挂在SPI1下，器件名不能重名
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }

           if((gNUM>=6)&&(gNUM<=11))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI2",WB7363_SPI_DEV_SPI_NAME[t]);     //6-12挂在SPI2下
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }

           if((gNUM>=12)&&(gNUM<=17))
           {
                 SPI_DevAdd_s(&Max14001_Dev[t],"SPI3",WB7363_SPI_DEV_SPI_NAME[t]);     //12-18挂在SPI3下
                 SPI_BusCtrl(&Max14001_Dev[t],CN_SPI_SET_POLL,0,0);
                 Max14001_Config(THL_Value,THU_Value,Max14001_Dev[t]);
           }
           gNUM++;
         }
         return true;
    }
    else
    {
        printf("KI通道数与AD芯片数不一致\r\n");
        return false;
    }
}
