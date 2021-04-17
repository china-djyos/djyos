//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//所属模块:标准输入--触摸屏模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 触摸屏硬件驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "Touch.h"
#include "string.h"
#include "gkernel.h"
#include "stmpe811.h"
#include "cpu_peri_iic.h"
#include "iicbus.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    struct GkWinObj;
//    extern ptu32_t ModuleInstall_Touch_Stmpe811(void);
//    ModuleInstall_Touch_Stmpe811(stmpe811_desktop);
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_STMPE811_TOUCH_DEV_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"stmpe811"//IIC接口的触摸屏
//parent:"touch"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","rtc","lock","touch","iicbus","graphical decorate development"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_STMPE811 == false )
//#warning  " stmpe811  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_STMPE811    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,100000,400000,
#define CFG_CRT_CLK_FRE                 (100*1000)      //"IIC总线速度"，总线速度，单位Hz
//%$#@enum,0x41,0x44,
#define CFG_STMPE811_DEVADDR            0x41            //"设备地址"，IIC总线上的设备地址
//%$#@string,1,30,
#define CFG_STMPE811_TOUCH_DEV_NAME   "TOUCH_STMPE811"       //"触摸屏名称",配置触摸屏名称
#define CFG_STMPE811_DISPLAY_NAME        "LCD_DESKTOP_STMPE811"      //"显示器名称",配置触摸屏所在显示器的名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static struct ST_TouchAdjust tg_touch_adjust;
static struct IIC_Device *s_ptCRT_Dev = NULL;//定义IICBUS架构下的IIC设备结构


//#define CRT_ADDRESS         0x49            //设备地址
//#define CFG_STMPE811_DEVADDR    0x41            /* 7-bit I2C address */
/* =============================================================================
 功能：stmpe811芯片初始化，初始化和加载设备到对应的IIC总线.
 参数：无
 返回：true,成功;false,失败
 =============================================================================*/

static bool_t STMPE811_Init(char *BusName)
{

//    static struct  IIC_Device s_CRT_Dev;
//    s_CRT_Dev.DevAddr=CRT_ADDRESS;
//    s_CRT_Dev.BitOfMemAddr=8;
//    s_CRT_Dev.BitOfMemAddrInDevAddr=0;

    s_ptCRT_Dev = IIC_DevAdd(BusName,"STMPE811",CFG_STMPE811_DEVADDR,7,23);
    if(s_ptCRT_Dev)
    {
        IIC_BusCtrl(s_ptCRT_Dev,CN_IIC_SET_CLK,CFG_CRT_CLK_FRE,0);//设置时钟大小
        IIC_BusCtrl(s_ptCRT_Dev,CN_IIC_SET_POLL,0,0);       //使用中断方式发送
        return true;
    }

    return false;
}
// =============================================================================
// 功能：    读函数，
// 参数：    RegAddr 寄存器地址  num 数据长度
// 返回：接收的字
// =============================================================================
static u32 TS_Read ( u8 RegAddr, u8 num)
{

   static u8 buf[4];
   num = IIC_Read(s_ptCRT_Dev,RegAddr,buf,num,0xffffffff);
    switch (num)
    {   case 1:
            return buf[0];

        case 2:
            return ((buf[1]) | buf[0]<<8);

        case 3:
            return (buf[2]|(buf[1]<<8)|(buf[0]<<16));

        default:
            return(buf[3]|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24));
    }

}
// =============================================================================
// 功能：写寄存器
// 参数：DelDev,器件结构体指针
//       addr,发送地址，即指存储地址
//       buf,发送数据缓冲区指针
//       len,发送数据长度，字节单位
//       block_option,阻塞选项,true表示阻塞发送，false为非阻塞发送
//       timeout,超时时间，us
// =============================================================================
static  void TS_Write (u8 reg, u8 num, u32 val)
{
    u8 buf[4];
    buf[0]=(u8)(val & 0x000000ff);
    IIC_Write( s_ptCRT_Dev , reg,buf,num, true,0xffffffff );

}
//---------------------------------------------------------------------------
//功能: 触摸屏硬件初始化
//参数: 无
//返回: 键盘设备号，-1表示按照键盘设备失败
//-----------------------------------------------------------------------------
static bool_t touch_hard_init(void)
{
    u16 chipid;

    chipid = TS_Read (CHIP_ID, 2);                      //测试通信是否正常
    if(chipid ==0x811)
    {
        TS_Write(SYS_CTRL2, 1, 0x0C);                   //打开TSC及ADC的时钟
        DJY_DelayUs(100);
        TS_Write(INT_EN, 1, 0x07);                      //使能中断
        //ADC分辨率（12bit），参考电压（内部）及采样时间（124）
        DJY_DelayUs(100);
        TS_Write(ADC_CTRL1 , 1, 0x68);
        DJY_DelayUs(100);
        TS_Write(ADC_CTRL2 , 1, 0x01);                  //ADC采样频率（3.25Mhz）
        DJY_DelayUs(100);
        TS_Write(GPIO_AF , 1, 0x00);                    //端口选择为触摸屏使用
        DJY_DelayUs(100);
        TS_Write(TSC_CFG, 1, 0x92);                     //TSC_CFG
        DJY_DelayUs(100);
        TS_Write (FIFO_TH, 1, 16);             //触摸屏的触发门限1（测量的是单击）
        //TS_Write (FIFO_TH, 1, 0x05);         //触摸屏的触发门限5（测量的是轨迹）
        DJY_DelayUs(100);
        TS_Write(FIFO_STA, 1, 0x01);                    //FIFO复位
        DJY_DelayUs(100);
        TS_Write(FIFO_STA, 1, 0x00);
        DJY_DelayUs(100);
        TS_Write(TSC_FRACT_XYZ, 1, 0x07);
        DJY_DelayUs(100);
        TS_Write(TSC_I_DRIVE, 1, 0x01);
        DJY_DelayUs(100);

        TS_Write(TSC_CTRL, 1, 0x01 | stmpe811_opmode<<1); //使能TSC
        DJY_DelayUs(100);
        TS_Write(INT_STA, 1, 0xFF);                       //清除所有中断
        DJY_DelayUs(100);
        TS_Write(INT_CTRL, 1, 0x01);                      //不使能TSC
        DJY_DelayUs(100);


        return true;
    }
    else
    {
        return false;
    }

}

//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取stmpe811采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
//-----------------------------------------------------------------------------
static ufast_t read_touch_stmpe811(struct SingleTouchMsg *touch_data)
{
    s32 i, n, tch_data;
    s32 tch_x,tch_y,tch_z;
    u8 tmp,tch_int;
    tch_int = TS_Read (INT_STA, 1);     //Read Touch-screen interrupt status

    if ((tch_int & 0x02))                 //过了门限,或FIFO满，或FIFO溢出
    {
        tmp = TS_Read(INT_EN, 1);
        TS_Write(INT_EN, 1, tmp &( ~(tch_int & 0x0E)));  //读取数据时需要禁止中断

        TS_Write(INT_STA, 1, 0xff);  //读取数据时需要禁止中断

        i = TS_Read (FIFO_SIZE, 1);        //FIFO中的采样次数
        n=i;
        tch_x=0;
        tch_y=0;
        tch_z=0;

#if (stmpe811_opmode == 0)

        i <<= 2;
        while (i--)                       //需要将这些字节读出来，直到小于门限
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data;
            tch_data |= TS_Read(TSC_DATA, 1);
            touch_data->x <<= 4;
            touch_data->x |= tch_data>>4;

            touch_data->y = tch_data & 0x0F;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y <<= 4;
            touch_data->y |= tch_data;
            touch_data->z = TS_Read(TSC_DATA, 1);

            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z +=touch_data->z;
        }

#elif (stmpe811_opmode == 1)
        while (i--)
        {
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x = tch_data<<4;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->x |= tch_data>>4;
            touch_data->y = (tch_data & 0x0F)<<8;
            tch_data = TS_Read(TSC_DATA, 1);
            touch_data->y |= tch_data;

            tch_x +=touch_data->x;
            tch_y +=touch_data->y;
            tch_z++;
        }
#endif

        //还有采样点未读完或FIFO满或FIFO溢出
        if (TS_Read(FIFO_SIZE, 1) || (tch_int & 0x0C))
        {
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            TS_Write(FIFO_STA, 1, 0x00);
        }

        TS_Write(INT_EN, 1, tmp | (tch_int & 0x0E));    //重新使能中断
        TS_Write(INT_STA, 1, 0x02);
        if(n!=0)
        {
            tch_x /=n;       //平均多次座标值
            tch_y /=n;
            tch_z /=n;
        }


        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }

        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        touch_data->z = tch_z;
        if (TS_Read(FIFO_SIZE, 1))
        {
            TS_Write(INT_STA, 1, 0xFF);                //清除所有中断
            DJY_EventDelay(100);
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            DJY_EventDelay(100);
            TS_Write(FIFO_STA, 1, 0x00);
            DJY_EventDelay(100);
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)//用静态变量存储每次开机校准一次//
//返回: 无
//-----------------------------------------------------------------------------
static void touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1,touch_xyz2;
//    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    u8 sta;

//    if((touch_init = fopen("/sys/touch_init.dat","r")) != NULL)
//    {
//
//        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
//        while(1);// added by jzl for test!
//    }
//    else
    {
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;
    //    GK_CreateWin(desktop,desktop,limit_left,limit_top,limit_right,limit_bottom,
    //                      CN_COLOR_WHITE,CN_WINBUF_BUF,"&tg_touch_adjust",CN_R3_SRCCOPY,0);
    //    GK_SetPrio(desktop,-1,CN_GK_SYNC);
        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "touch", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "TOUCH", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
        while(!read_touch_stmpe811(&touch_xyz0));           //记录触摸屏第一点校正值
        DJY_DelayUs(300);
//这里的松手检测是通过读检查fifo中是否有坐标点数据来实现的，
//有数据清空并延时0.1s再检测，软件可以不用考虑防抖。
        do
        {
            if (TS_Read(FIFO_SIZE, 1))
            {
                TS_Write(INT_STA, 1, 0xFF);                //清除所有中断
                DJY_DelayUs(100);
                TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
                DJY_DelayUs(100);
                TS_Write(FIFO_STA, 1, 0x00);
                DJY_DelayUs(100);
            }

            DJY_DelayUs(1000*100);
            sta=TS_Read (INT_STA, 1);
        }while(sta&2);//fifo中有数据则重读，无数据则认为松手


        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "touch", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "TOUCH", 21, CN_COLOR_BLACK+1, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        if (TS_Read(FIFO_SIZE, 1))
        {
            TS_Write(INT_STA, 1, 0xFF);                //清除所有中断
            DJY_DelayUs(100);
            TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
            DJY_DelayUs(100);
            TS_Write(FIFO_STA, 1, 0x00);
            DJY_DelayUs(100);
        }

        while(!read_touch_stmpe811(&touch_xyz1));           //记录触摸屏第二点校正值

        do
        {
            if (TS_Read(FIFO_SIZE, 1))
            {
                TS_Write(INT_STA, 1, 0xFF);                //清除所有中断
                DJY_DelayUs(100);
                TS_Write(FIFO_STA, 1, 0x01);                //FIFO复位
                DJY_DelayUs(100);
                TS_Write(FIFO_STA, 1, 0x00);
                DJY_DelayUs(100);
            }

            DJY_DelayUs(1000*100);
            sta=TS_Read (INT_STA, 1);
        }while(sta&2);//等待松手


        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
        tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);
    //    GK_DestroyWin(desktop);
//        touch_init = fopen("/sys/touch_init.dat","w+");
//        if(touch_init)
//            fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    }
//    fclose(touch_init);

}
//----初始化触摸屏模块---------------------------------------------------------
//功能:
//参数: display_name,本触摸屏对应的显示器名(资源名)
//返回: true,成功;false,失败
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch_Stmpe811(void)
{
    struct GkWinObj *desktop;
    static struct SingleTouchPrivate stmpe811;

    desktop = GK_GetDesktop(CFG_STMPE811_DISPLAY_NAME);
    if(!STMPE811_Init("IIC2" ))//将器件挂到IIC2总线上
        return false;
    if(!touch_hard_init())//触摸屏初始化
         return false;
    touch_ratio_adjust(desktop);          //屏幕校准
    stmpe811.read_touch = read_touch_stmpe811;//读触摸点的坐标函数
    stmpe811.touch_loc.display = GK_GetDisplay(CFG_STMPE811_DISPLAY_NAME);
    Touch_InstallDevice(CFG_STMPE811_TOUCH_DEV_NAME,&stmpe811);//添加驱动到Touch
    return true;
}
