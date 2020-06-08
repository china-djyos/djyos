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
#include "cpu_peri.h"
#include "djyos.h"
#include "stdlib.h"
#include "stm32h7xx.h"
#include "stm32h7xx_hal_dma2d.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_rcc_ex.h"
#include "cpu_peri_lcd.h"
#include "string.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,const char* HeapName);
//  ModuleInstall_LCD(CFG_DISPLAY_NAME,CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip lcd"//CPU内置LCD控制器驱动
//parent:"graphical kernel"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"heap","graphical kernel"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:none           //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LCD == false )
//#warning  " cpu_onchip_lcd  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LCD    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,1,2048,
#define CFG_LCD_XSIZE                   600 //"LCD水平像素宽度",
#define CFG_LCD_YSIZE                   480 //"LCD竖直像素宽度",
//%$#@enum,true,false,
//%$#@string,1,32,
#define CFG_DISPLAY_NAME        "LCD_F7"    //"显示器LCD名称",
#define CFG_LCD_HEAP_NAME       "extram"    //"LCD使用堆名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure


#define lcd_swap(a, b){a^=b; b^=a; a^=b;}

static struct LCD_ConFig lcd;                      //管理LCD LTDC的重要参数
static LTDC_HandleTypeDef  LTDC_Handler;       //LTDC句柄
static u8 *pLTDCBufferFG1 =NULL;//缓冲区起始位置
//static u8 *pLTDCBufferFG2 =NULL;//缓冲区起始位置
static struct DisplayObj lcd_display;
static struct MutexLCB Dma2dMutex;//Dma2D互斥访问。

//不同的屏有些参数是不同的，不同那个的屏需要实现该函，数完成该函数参数的配置
extern bool_t Ltdc_Lcd_Config(struct LCD_ConFig *lcd);
//----------------------------------------------------------------
//功能: 开关背光
//参数:  0关背光 非零开背光
//----------------------------------------------------------------
__attribute__((weak))   void Lcd_BackLight_OnOff(u8 onoff)
{
//  if(onoff)
//      GPIO_SettoLow(GPIO_A,PIN8);
//  else
//      GPIO_SettoHigh(GPIO_A,PIN8);
}

//----------------------------------------------------------------
//功能: 开关LTDC
//参数:0/1
//----------------------------------------------------------------------------
void LTDC_Switch(u8 sw)
{
    if(sw==1) __HAL_LTDC_ENABLE(&LTDC_Handler);
    else if(sw==0)__HAL_LTDC_DISABLE(&LTDC_Handler);
}

//----------------------------------------------------------------
//功能: 开关指定层Layer
//参数:  层layer 0,第一层; 1,第二层
//      开关sw 1/0
//返回值:ture/false
//----------------------------------------------------------------------------
bool_t LTDC_Layer_Switch(u8 layerx,u8 sw)
{
    if(sw==1) __HAL_LTDC_LAYER_ENABLE(&LTDC_Handler,layerx);
    else if(sw==0) __HAL_LTDC_LAYER_DISABLE(&LTDC_Handler,layerx);
    __HAL_LTDC_RELOAD_CONFIG(&LTDC_Handler);
    return true;
}

//----------------------------------------------------------------
//功能: LTDC时钟设置。
//参数: pllsaidivr:LCD时钟分频系数,取值范围:0~3,对应分频2^(pllsaidivr+1)
//返回值:0,成功;1,失败。
//备注:CLK=(pllsain/pllsair)/(pllsaidivr)
//----------------------------------------------------------------------------
static bool_t LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;
    PeriphClkIniture.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkIniture.PLL3.PLL3M = pllsain;
    PeriphClkIniture.PLL3.PLL3N = pllsair;
    PeriphClkIniture.PLL3.PLL3P = 2;
    PeriphClkIniture.PLL3.PLL3Q = 2;
    PeriphClkIniture.PLL3.PLL3R = pllsaidivr;

    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK)
    {
        return 0;   //成功
    }
    else return 1;  //失败
}

//----初始化DMA2D硬件------------------------------------------------------------
//功能: 初始化DMA2D硬件
//参数: 无
//返回: 无
//备注:
//----------------------------------------------------------------------------
static bool_t Dma2d_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();               //使能DMA2D时钟

    return true;
}
//----初始化LTDC设备------------------------------------------------------------
//LTDC,基本参数设置.
//注意:此函数,必须在LTDC_Layer_Window_Config之前设置.
//layerx:层值,0/1.
//bufaddr:层颜色帧缓存起始地址
//pixformat:颜色格式.0,ARGB8888;1,RGB888;2,RGB565;3,ARGB1555;4,ARGB4444;5,L8;6;AL44;7;AL88
//alpha:层颜色Alpha值,0,全透明;255,不透明
//alpha0:默认颜色Alpha值,0,全透明;255,不透明
//bfac1:混合系数1,4(100),恒定的Alpha;6(101),像素Alpha*恒定Alpha
//bfac2:混合系数2,5(101),恒定的Alpha;7(111),像素Alpha*恒定Alpha
//bkcolor:层默认颜色,32位,低24位有效,RGB888格式
//返回值:无
//-----------------------------------------------------------------------
static void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
{
    LTDC_LayerCfgTypeDef pLayerCfg;

    pLayerCfg.WindowX0=0;                       //窗口起始X坐标
    pLayerCfg.WindowY0=0;                       //窗口起始Y坐标
    pLayerCfg.WindowX1=CN_LCD_XSIZE;          //窗口终止X坐标
    pLayerCfg.WindowY1=CN_LCD_YSIZE;         //窗口终止Y坐标
    pLayerCfg.PixelFormat=pixformat;            //像素格式
    pLayerCfg.Alpha=alpha;                      //Alpha值设置，0~255,255为完全不透明
    pLayerCfg.Alpha0=alpha0;                    //默认Alpha值
    pLayerCfg.BlendingFactor1=(u32)bfac1<<8;    //设置层混合系数
    pLayerCfg.BlendingFactor2=(u32)bfac2<<8;    //设置层混合系数
    pLayerCfg.FBStartAdress=bufaddr;            //设置层颜色帧缓存起始地址
    pLayerCfg.ImageWidth=CN_LCD_XSIZE;        //设置颜色帧缓冲区的宽度
    pLayerCfg.ImageHeight=CN_LCD_YSIZE;      //设置颜色帧缓冲区的高度
    pLayerCfg.Backcolor.Red=(u8)(bkcolor&0X00FF0000)>>16;   //背景颜色红色部分
    pLayerCfg.Backcolor.Green=(u8)(bkcolor&0X0000FF00)>>8;  //背景颜色绿色部分
    pLayerCfg.Backcolor.Blue=(u8)bkcolor&0X000000FF;        //背景颜色蓝色部分
    HAL_LTDC_ConfigLayer(&LTDC_Handler,&pLayerCfg,layerx);   //设置所选中的层
}

//-----------------------------------------------------------------------
//LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
//----初始化LTDC设备------------------------------------------------------------
static void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //设置窗口的位置
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//设置窗口大小
}

//-----------------------------------------------------------------------
//选择层
//layerx:层号;0,第一层;1,第二层;
//目前只用一层
//-----------------------------------------------------------------------
static bool_t LTDC_Select_Layer(u8 layerx)
{
    switch (layerx)
    {
        case 0:
            lcd.pFrameBuffe=pLTDCBufferFG1;
            return true;
        case 1:
//            lcd.pFrameBuffe=pLTDCBufferFG2;
            return true;
        default:  return false;
    }
}

//-----------------------------------------------------------------------
//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
//-----------------------------------------------------------------------
static bool_t LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
    u32 timeout=0;
    u32 addr;
    bool_t flag=true;
    u32 height,width;


    width=ex-sx+1;
    height=ey-sy+1;
    addr=((u32)lcd.pFrameBuffe+lcd.pixsize*(CN_LCD_XSIZE*sy+sx));
    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut)
            ||(ex+1>CN_LCD_XSIZE)
            ||(ey+1>CN_LCD_YSIZE))
        return false;

    DMA2D->CR&=~(1<<0);
    DMA2D->CR=3<<16;
    DMA2D->OPFCCR=lcd.Dma2dPixelFormat;
    DMA2D->OOR  = lcd_display.width-width;
    DMA2D->OMAR =  addr;
    DMA2D->NLR  = height|(width<<16);
    DMA2D->OCOLR=color;
    DMA2D->CR|=1<<0;
    while((DMA2D->ISR&(1<<1))==0)   //等待传输完成
    {
        timeout++;
        if(timeout>0X1FFFFF)
            {
                break;  //超时退出
                flag=false;
            }
    }
    DMA2D->IFCR|=1<<1;              //清除传输完成标志
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}

//------------------------------------------------------------------
//功能: LTDC 初始化
//参数: true/false
//返回:
//-----------------------------------------------------------------------------
static void LTDC_Init(u8 *pFrameBufferFG)
{
    LTDC_Clk_Set(5,160,18);//设置像素时钟  50Mhz
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=lcd.LtdcHSPolarity;     //水平同步极性
    LTDC_Handler.Init.VSPolarity=lcd.LtdcVSPolarity;     //垂直同步极性
    LTDC_Handler.Init.DEPolarity= lcd.LtdcDEPolarity;    //数据使能极性
    LTDC_Handler.Init.PCPolarity=lcd.LtdcPCPolarity;     //像素时钟极性
    LTDC_Handler.Init.HorizontalSync=lcd.hsw-1;          //水平同步宽度
    LTDC_Handler.Init.VerticalSync=lcd.vsw-1;            //垂直同步宽度
    LTDC_Handler.Init.AccumulatedHBP=lcd.hsw+lcd.hbp-1;  //水平同步后沿宽度
    LTDC_Handler.Init.AccumulatedVBP=lcd.vsw+lcd.vbp-1;  //垂直同步后沿高度
    LTDC_Handler.Init.AccumulatedActiveW=lcd.hsw+lcd.hbp+lcd_display.width-1;//有效宽度
    LTDC_Handler.Init.AccumulatedActiveH=lcd.vsw+lcd.vbp+lcd_display.height-1;//有效高度
    LTDC_Handler.Init.TotalWidth=lcd.hsw+lcd.hbp+lcd_display.width+lcd.hfp-1;//总宽度
    LTDC_Handler.Init.TotalHeigh=lcd.vsw+lcd.vbp+lcd_display.height+lcd.vfp-1;//总高度
    LTDC_Handler.Init.Backcolor.Red  =0;           //屏幕背景层红色部分
    LTDC_Handler.Init.Backcolor.Green=0;         //屏幕背景层绿色部分
    LTDC_Handler.Init.Backcolor.Blue =0;          //屏幕背景色蓝色部分
    LTDC_Handler.State = HAL_LTDC_STATE_RESET;
    __HAL_RCC_LTDC_CLK_ENABLE();     //使能LTDC时钟
    HAL_LTDC_Init(&LTDC_Handler);

    //层配置
    LTDC_Layer_Parameter_Config(0,(u32)pFrameBufferFG,lcd.LtdcPixelFormat,255,0,6,7,0X000000);//层参数配置
    LTDC_Layer_Window_Config(0,0,0,lcd_display.width,lcd_display.height);
    LTDC_Select_Layer(0);           //选择第1层
    Lcd_BackLight_OnOff(1);

}
//------------------------------------------------------------------
//功能: 背光开关
//参数: true/false
//返回:
//-----------------------------------------------------------------------------


//----控制显示器---------------------------------------------------------------
//功能: 这是由driver提供商编写的，用于设置显示器各种参数的函数，属于应用程序的
//      范畴，功能由设备厂商定义。
//参数: disp，显示器指针
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
static bool_t __lcd_disp_ctrl(struct DisplayObj *disp)
{
    return true;
}


//----位图中画像素-------------------------------------------------------------
//功能: 在位图中画一个像素，只有在bitmap的像素格式为cn_custom_pf时，才需要绘制。
//      如果显卡不打算支持自定义格式，本函数直接返回。
//参数: bitmap，目标位图
//      limit，限制矩形，只绘制在该矩形内部的部分
//      x、y，坐标
//      color，绘图用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: 无
//-----------------------------------------------------------------------------
static bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                     s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
//----位图中画线---------------------------------------------------------------
//功能: 在位图中画一条直线，不绘制(x2,y2)点，只绘制在limit限定的区域内的部分。
//参数: bitmap，目标位图
//      limit，限制矩形，只绘制在该矩形内部的部分
//      x1、y1、x2、y2，起点终点坐标
//      color，绘图用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
static bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----位图中画线(含端点)-------------------------------------------------------
//功能: 在位图中画一条直线，只绘制在limit限定的区域内的部分。
//参数: bitmap，目标位图
//      limit，限制矩形，只绘制在该矩形内部的部分
//      x1、y1、x2、y2，起点终点坐标
//      color，绘图用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
static bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----位图中填充矩形----------------------------------------------------------------
//Bitmap中矩形填充,支持渐变色
//什么情况下应该实现本函数的功能,参见本结构前面的注释
//镜像显示器无须实现本函数,直接返回false即可
//参数:
// DstBitmap: 绘制的目标位图
// Target: 目标填充区域,渐变色填充时用它的坐标做计算颜色的起点和终点
// Focus: 实际填充区域,gkernel确保其在Target矩形内部
// Color0,Color1: Target左上角和右下角颜色值,如果不是渐变填充,Color1将被忽略,
//              像素格式是CN_SYS_PF_ERGB8888
//Mode: 填充模式,CN_FILLRECT_MODE_N族常数
//返回: true=成功绘制，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    u16 pixel;
    u16 *dst;
    s32 height,width;
    u32 timeout;
    bool_t flag;

    if((Mode != CN_FILLRECT_MODE_N)  \
        ||(dst_bitmap->PixelFormat != lcd.LcdPixelFormat))
      return false;

    pixel = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,Color0);

    dst = (u16*)((u32)dst_bitmap->bm_bits+Focus->left*lcd.pixsize+\
                     Focus->top * dst_bitmap->linebytes);

    width=Focus->right - Focus->left;
    height=Focus->bottom - Focus->top;
    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut))
      return false;
    DMA2D->CR&=~(1<<0);             //先停止DMA2D
    DMA2D->CR=3<<16;                //寄存器到存储器模式
    DMA2D->OPFCCR=lcd.Dma2dPixelFormat;  //设置输出颜色格式
    DMA2D->OOR  = dst_bitmap->width-width;  //设置行偏移
    DMA2D->OMAR=(u32)dst;               //输出存储器地址
    DMA2D->NLR  = height|(width<<16);   //设定行数寄存器
    DMA2D->OCOLR=pixel;                //设定输出颜色寄存器
    DMA2D->CR|=1<<0;                //启动DMA2D
    timeout =0;
    while((DMA2D->ISR&(1<<1))==0)   //等待传输完成
    {
      timeout+=10;
      DJY_DelayUs(10);
      if(timeout>lcd.Dma2dTimeOut)
      {
          flag=false;
           break;  //超时退出
      }
    }
    DMA2D->IFCR|=1<<1;              //清除传输完成标志
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}
//--------------bitmap中位块传送-----------------------------------------------
//在两个矩形位图中位块传送,如果矩形尺寸不相等,则要实现拉伸或压缩
//什么情况下应该实现本函数的功能,参见本结构前面的注释
//镜像显示器无须实现本函数,直接返回false即可
//参数:
// dst_bitmap: 目标位图
// DstRect: 目标矩形
// src_bitmap: 源位图
// SrcRect: 源矩形
// RopCode: 显示效果,参见 gkernel.h文件中 CN_ROP_ALPHA_SRC_MSK 族常量的说明
// HyalineColor: 透明色,如果参数RopCode中设定了 CN_ROP_KEYCOLOR_EN,则
//                  本参数指定透明色,CN_SYS_PF_ERGB8888格式
//返回: true=成功,false=失败
//-----------------------------------------------------------------------------

bool_t __lcd_blt_bm_to_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *DstRect,
                          struct RectBitmap *src_bitmap,
                          struct Rectangle *SrcRect,
                          struct RopGroup RopCode,u32 transparentcolor)
{

    u16 *src_offset,*dst_offset;    //源位图点阵缓冲区可能不对齐!!!
    u32 width,height;
    u32 timeout=0;
    bool_t flag=true;
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };

    width = SrcRect->right-SrcRect->left;
    height= SrcRect->bottom-SrcRect->top;

    if((dst_bitmap->PixelFormat != lcd.LcdPixelFormat) \
             ||(src_bitmap->PixelFormat != lcd.LcdPixelFormat)\
             ||((DstRect->bottom-DstRect->top) != height)\
             ||((DstRect->right-DstRect->left) != width)\
             ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))!=0)
             ||((ptu32_t)src_bitmap->bm_bits & 1))
        return false;

    src_offset = (u16*)((ptu32_t)(src_bitmap->bm_bits+\
              SrcRect->top * src_bitmap->linebytes)+\
              SrcRect->left*lcd.pixsize);

    dst_offset = (u16*)((ptu32_t)(dst_bitmap->bm_bits+\
             DstRect->top * dst_bitmap->linebytes)+\
             DstRect->left*lcd.pixsize);

    if(false==Lock_MutexPend(&Dma2dMutex,lcd.Dma2dTimeOut))
        return false;
    DMA2D->CR=0<<16;                //存储器到存储器模式
    DMA2D->FGPFCCR=lcd.Dma2dPixelFormat;   //设置颜色格式
    DMA2D->FGOR=(u32)(src_bitmap->width - width); //前景层行偏移为0
    DMA2D->OOR= (u32)(dst_bitmap->width - width); //设置行偏移
    DMA2D->CR&=~(1<<0);             //先停止DMA2D
    DMA2D->FGMAR=(u32)src_offset;        //源地址
    DMA2D->OMAR=(u32)dst_offset;        //输出存储器地址
    DMA2D->NLR=(height)|((width)<<16);   //设定行数寄存器
    DMA2D->CR|=1<<0;                //启动DMA2D
    while((DMA2D->ISR&(1<<1))==0)   //等待传输完成
    {
        timeout++;
        if(timeout>0X1FFFFF)
        {
            flag=false;
            break;  //超时退出
        }
    }
    DMA2D->IFCR|=1<<1;              //清除传输完成标志
    Lock_MutexPost(&Dma2dMutex);
    return flag;
}

//----screen中画像素-----------------------------------------------------------
//功能: 在screen中画一个像素.
//参数: x、y，像素坐标
//      color，颜色
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=没有绘制，或无需绘制
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 rop2_code)
{

    u32 dest;
    u32 byteoffset;
    if(((x+1)>CN_LCD_XSIZE)||((y+1)>CN_LCD_YSIZE))
        return false;
    byteoffset = (y*CN_LCD_XSIZE + x)*lcd.pixsize;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
    dest = (u32)lcd.pFrameBuffe[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);

    switch (lcd.pixsize)
    {
    case 2:
        *(u16 *)(&lcd.pFrameBuffe[byteoffset]) = (u16)color;
        break;
    case 3:
        lcd.pFrameBuffe[byteoffset] = (u8)color>>16;
        lcd.pFrameBuffe[byteoffset+1] = (u8)color>>8;
        lcd.pFrameBuffe[byteoffset+2] = (u8)color;
        break;
    default:            break;
    }
    return true;

}

//在screen中画一个像素，有frame buffer的情况下，正常显示gui不会调用这个函数，
//如果窗口direct_screen==true，则可能调用本函数，无论是否有frame buffer，
//driver都必须提供并且必须实现本函数
//镜像显示器必须实现本函数的功能,不能直接返回false
//参数:
// PixelGroup: 像素坐标数组指针.
// color: 颜色值,CN_SYS_PF_ERGB8888格式
// n: 坐标点的数量
// r2_code: 显示效果,详见gkernel.h中的CN_R2_BLACK族常数定义
bool_t __lcd_set_pixel_group_screen(struct PointCdn *PixelGroup,u32 color,u32 n, u32 r2_code)
{
    u32 dest,i;
    u32 offset;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);

    for(i=0;i<n;i++)
    {
        offset = PixelGroup[i].y*CN_LCD_XSIZE + PixelGroup[i].x;
        dest = *(u16 *)&(lcd.pFrameBuffe[offset]);
        dest = GK_BlendRop2(dest,color,r2_code);

        *(u16 *)&(lcd.pFrameBuffe[offset]) = dest;
    }


    return true;
}

//----screen中画线---------------------------------------------------------------
//功能: 在screen中画一条任意直线，不含端点，只绘制在limit限定的区域内的部分。如
//      不支持画线的硬件加速，直接返回false。
//参数: limit，限制矩形，只绘制在该矩形内部的部分
//      x1、y1、x2、y2，起点终点坐标
//      color，绘图用的颜色
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    if(y1 == y2)    //绘水平线
    {
        if(x1 > x2)
            lcd_swap(x1,x2);
        if((y1 < limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return false;         //所绘直线在limit之外
        if(x1 < limit->left)
            x1 = limit->left;
        if(x2 > limit->right)
            x2 = limit->right;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return LTDC_Color_Fill( x1,y1,x2-1,y2+1,color);

    }else if(x1 == x2)   //绘垂直线
    {
        if(y1 > y2)
            lcd_swap(y1,y2);
        if((x1 < limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return false;         //所绘直线在limit之外
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return LTDC_Color_Fill( x1,y1,x2+1,y2-1,color);
    }
    return false;
}
//----screen中画线(含端点)------------------------------------------------------
//功能: 在screen中画一条直线，只绘制在limit限定的区域内的部分。
//参数: limit，限制矩形，只绘制在该矩形内部的部分
//      x1、y1、x2、y2，起点终点坐标
//      color，绘图用的颜色，cn_sys_pf_e8r8g8b8格式
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
static bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{

    return false;
}
//----screen中填充矩形-----------------------------------------------------------
//功能: 把screen中的矩形用color颜色填充，color的格式是cn_sys_pf_e8r8g8b8。
//参数: dst_rect，待填充的矩形
//      color，填充颜色
//返回: true=成功绘制，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 pen;
    bool_t flag;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,Color0);
    flag=LTDC_Color_Fill(Focus->left,Focus->top,Focus->right-1,Focus->bottom-1,pen);
    return flag;
}

//----bitmap到screen位块传送---------------------------------------------------
//功能: 把一个位图中的一个矩形区域传送到另一个区域，
//      源和目的位图可以相同。
//参数: dst_rect，目标矩形
//      src_bitmap，源位图
//      xsrc、ysrc，源位图中被传送的区域左上角坐标(以源位图左上角坐标为原点)
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
static bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
        struct RectBitmap *src_bitmap,s32 x,s32 y)
{
    return false;
}
//----从screen中取像素---------------------------------------------------------
//功能: 从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8或cn_sys_pf_a8r8g8b8格式。
//参数: x、y，坐标
//返回: 像素颜色值
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 e,r,g,b,color;
    u32 byteoffset;
    byteoffset = (y*CN_LCD_XSIZE + x)*lcd.pixsize;

    switch (lcd.pixsize)
    {
    case 2:
        color = *(u16 *)(&lcd.pFrameBuffe[byteoffset]);
        r = (color>>11) &0x1f;
        g = (color>>5) & 0x3f;
        b = color & 0x1f;
        break;
    case 3:
        color = lcd.pFrameBuffe[byteoffset]|\
        lcd.pFrameBuffe[byteoffset+1] <<8|\
        lcd.pFrameBuffe[byteoffset+2] <<16;
        b = lcd.pFrameBuffe[byteoffset];
        g = lcd.pFrameBuffe[byteoffset+1];
        r = lcd.pFrameBuffe[byteoffset+2];
        break;
    default:            break;
    }
    e = 0;
    return ((e<<28)|(r<<20) | (g<<12) | (b<<4));
}

//----读取bitmap中矩形块-------------------------------------------------------
//功能: 把一个位图中的矩形读取到另一个位图中。
//参数: rect，欲读取的矩形
//      dest，保存矩形的位图，其长宽必须与rect相同
//返回: true=成功读取，false=失败，原因可能是提供了不支持的像素格式
//-----------------------------------------------------------------------------
static bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}

//----初始化lcd设备------------------------------------------------------------
//功能: 初始化lcd设备。
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------
struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,\
        const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;

    struct HeapCB *heap;
    Ltdc_Lcd_Config(&lcd);
    heap =Heap_FindHeap(HeapName);
    if(heap==NULL)
        return NULL;
    //多申请64字节如果显存不是64字节对齐描点的时候会有闪屏的现象
    pLTDCBufferFG1 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize+0x40,heap,0);
//    pLTDCBufferFG2 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize,heap,0);//先用一层
    if(0x3f&(u32)pLTDCBufferFG1)
        pLTDCBufferFG1+=(0x40-(0x3f&(u32)pLTDCBufferFG1));

    FrameBitmap.bm_bits=(u8 *)pLTDCBufferFG1;
    FrameBitmap.width = CN_LCD_XSIZE;
    FrameBitmap.height = CN_LCD_YSIZE;
    FrameBitmap.PixelFormat = lcd.LcdPixelFormat;
    FrameBitmap.linebytes = CN_LCD_XSIZE*lcd.pixsize;
    FrameBitmap.ExColor = 0xffffffff;
    frame_win.wm_bitmap = &FrameBitmap;


    lcd_display.frame_buffer =&frame_win;
    lcd_display.framebuf_direct = true;

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    lcd_display.width = CFG_LCD_XSIZE;
    lcd_display.height = CFG_LCD_YSIZE;
    lcd_display.pixel_format = lcd.LcdPixelFormat;

    lcd_display.draw.SetPixelToBitmap = __lcd_set_pixel_bm;
    lcd_display.draw.FillRectToBitmap = __lcd_fill_rect_bm;
    lcd_display.draw.LineToBitmap = __lcd_line_bm;
    lcd_display.draw.LineToBitmapIe = __lcd_line_bm_ie;
    lcd_display.draw.BltBitmapToBitmap = __lcd_blt_bm_to_bm;
    lcd_display.draw.SetPixelToScreen = __lcd_set_pixel_screen;
    lcd_display.draw.SetPixelGroupToScreen = __lcd_set_pixel_group_screen;
    lcd_display.draw.LineToScreen = __lcd_line_screen;
    lcd_display.draw.LineToScreenIe = __lcd_line_screen_ie;
    lcd_display.draw.FillRectToScreen = __lcd_fill_rect_screen;
    lcd_display.draw.CopyBitmapToScreen = __lcd_bm_to_screen;
    lcd_display.draw.GetPixelFromScreen = __lcd_get_pixel_screen;
    lcd_display.draw.GetRectFromScreen = __lcd_get_rect_screen;

    lcd_display.DisplayHeap =  heap;
    lcd_display.disp_ctrl = __lcd_disp_ctrl;

    Dma2d_Init();
    Lock_MutexCreate_s(&Dma2dMutex,"DMA2D");
    LTDC_Init(pLTDCBufferFG1);

    if(GK_InstallDisplay(&lcd_display,DisplayName))
        return &lcd_display;
    return NULL;
}










