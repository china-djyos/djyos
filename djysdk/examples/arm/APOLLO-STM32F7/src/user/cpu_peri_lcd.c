//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.
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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "gkernel.h"
#include "gui/gkernel/gk_display.h"
#include "stdint.h"
#include "stdlib.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal_dma2d.h"
#include "stm32f7xx_hal_ltdc.h"
#include "stm32f7xx_hal_rcc_ex.h"
#include "string.h"
#include "cpu_peri_lcd.h"

#define lcd_swap(a, b){a^=b; b^=a; a^=b;}
typedef struct
{
    u16 hsw;            //水平同步宽度
    u16 vsw;            //垂直同步宽度
    u16 hbp;            //水平后廊
    u16 vbp;            //垂直后廊
    u16 hfp;            //水平前廊
    u16 vfp;            //垂直前廊
    u16 * pFrameBuffe;  //当前层缓冲区指针
    u32 pixsize;        //每个像素所占字节数
}_ltdc_dev;

static _ltdc_dev lcdltdc;                      //管理LCD LTDC的重要参数
static LTDC_HandleTypeDef  LTDC_Handler;       //LTDC句柄
static u16 *pLTDCBufferFG1 =NULL;//缓冲区起始位置
static u16 *pLTDCBufferFG2 =NULL;//缓冲区起始位置
static struct DisplayRsc lcd_display;
static struct MutexLCB Dma2dMutex;//Dma2D互斥访问。

#define  LCD_WIDTH    ((uint16_t)1024)             /* LCD PIXEL WIDTH */
#define  LCD_HEIGHT   ((uint16_t)600)             /* LCD PIXEL HEIGHT*/
#define CN_LCD_XSIZE  LCD_WIDTH
#define CN_LCD_YSIZE  LCD_HEIGHT

#define PIXSIZE  2    //每个像素占2个字节
#define CN_LCD_PIXEL_FORMAT     CN_SYS_PF_RGB565  //系统颜色格式定义
#define CN_DMA2D_PIXEL_FORMAT  DMA2D_OUTPUT_RGB565//固件库颜色额格式定义

#define DMA2D_timeout   50000


static  void __lcd_backlight_onoff(u8 onoff)
{
  if(onoff)
     GPIO_SettoHigh(GPIO_B,PIN5);
  else
     GPIO_SettoLow(GPIO_B,PIN5);

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
//备注:CLK=(pllsain/pllsair)/(2^pllsaidivr)
//----------------------------------------------------------------------------
static bool_t LTDC_Clk_Set(u32 pllsain,u32 pllsair,u32 pllsaidivr)
{
    RCC_PeriphCLKInitTypeDef PeriphClkIniture;

    //LTDC输出像素时钟，需要根据自己所使用的LCD数据手册来配置！
    PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;   //LTDC时钟
    PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;
    PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;
    PeriphClkIniture.PLLSAIDivR=pllsaidivr;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture)==HAL_OK) //配置像素时钟，这里配置为时钟为18.75MHZ
    {
        return 0;   //成功
    }
    else return 1;  //失败
}

//----初始化lcd硬件------------------------------------------------------------
//功能: 初始化lcd硬件。
//参数: 无
//返回: 无
//备注: 本函数是移植关键函数
//----------------------------------------------------------------------------
void HAL_LTDC_MspInit(LTDC_HandleTypeDef* hltdc)
{
    u32 Pin;
    __HAL_RCC_LTDC_CLK_ENABLE();                //使能LTDC时钟
    GPIO_PowerOn(GPIO_B);//背光控制引脚
    GPIO_PowerOn(GPIO_F);
    GPIO_PowerOn(GPIO_G);
    GPIO_PowerOn(GPIO_H);
    GPIO_PowerOn(GPIO_I);

    Pin=PIN5;//控制背光的引脚
    GPIO_CfgPinFunc(GPIO_B,Pin,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_PU);

    Pin=PIN10;
    GPIO_AFSet(GPIO_F,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN10;
    GPIO_AFSet(GPIO_F,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_F,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN6|PIN7|PIN11;
    GPIO_AFSet(GPIO_G,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_G,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN9|PIN10|PIN11|PIN12|PIN13|PIN14|PIN15;
    GPIO_AFSet(GPIO_H,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_H,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);

    Pin=PIN0|PIN1|PIN2|PIN3|PIN4|PIN5|PIN6|PIN7|PIN8|PIN9|PIN10;
    GPIO_AFSet(GPIO_I,Pin,AF14);
    GPIO_CfgPinFunc(GPIO_I,Pin,GPIO_MODE_AF,GPIO_OTYPE_PP,GPIO_SPEED_VH,GPIO_PUPD_NONE);
}

static bool_t Dma2d_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();               //使能DMA2D时钟

    return true;
}

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
void LTDC_Layer_Parameter_Config(u8 layerx,u32 bufaddr,u8 pixformat,u8 alpha,u8 alpha0,u8 bfac1,u8 bfac2,u32 bkcolor)
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
//LTDC,层颜窗口设置,窗口以LCD面板坐标系为基准
//注意:此函数必须在LTDC_Layer_Parameter_Config之后再设置.
//layerx:层值,0/1.
//sx,sy:起始坐标
//width,height:宽度和高度
void LTDC_Layer_Window_Config(u8 layerx,u16 sx,u16 sy,u16 width,u16 height)
{
    HAL_LTDC_SetWindowPosition(&LTDC_Handler,sx,sy,layerx);  //设置窗口的位置
    HAL_LTDC_SetWindowSize(&LTDC_Handler,width,height,layerx);//设置窗口大小
}

//选择层
//layerx:层号;0,第一层;1,第二层;
bool_t LTDC_Select_Layer(u8 layerx)
{
    switch (layerx)
    {
        case 0:
            lcdltdc.pFrameBuffe=pLTDCBufferFG1;
            return true;
        case 1:
            lcdltdc.pFrameBuffe=pLTDCBufferFG2;
            return true;
        default:  return false;
    }
}

//LTDC填充矩形,DMA2D填充
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色
bool_t LTDC_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
    u32 timeout=0;
    u32 addr;
    bool_t flag=true;
    u32 height,width;


    width=ex-sx;
    height=ey-sy;

    addr=((u32)lcdltdc.pFrameBuffe+lcdltdc.pixsize*(CN_LCD_XSIZE*sy+sx));
    if(false==Lock_MutexPend(&Dma2dMutex,DMA2D_timeout))
        return false;

    RCC->AHB1ENR|=1<<23;
    DMA2D->CR&=~(1<<0);
    DMA2D->CR=3<<16;
    DMA2D->OPFCCR=CN_DMA2D_PIXEL_FORMAT;
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
//在指定区域内填充指定颜色块,DMA2D填充
//此函数仅支持u16,RGB565格式的颜色数组填充.
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//注意:sx,ex,不能大于lcddev.width-1;sy,ey,不能大于lcddev.height-1!!!
//color:要填充的颜色数组首地址
bool_t  LTDC_BitMap_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *src_addr)
{
    u32 timeout=0;
    u16 offline;
    u32 addr;
    bool_t flag=true;
    //坐标系转换
    offline=lcd_display.width-(ex-sx);
    addr=((u32)(lcdltdc.pFrameBuffe+PIXSIZE*(lcd_display.width*sy+sx)));

    if(false==Lock_MutexPend(&Dma2dMutex,DMA2D_timeout))
        return false;
    RCC->AHB1ENR|=1<<23;            //使能DM2D时钟
    DMA2D->CR=0<<16;                //存储器到存储器模式
    DMA2D->FGPFCCR=CN_DMA2D_PIXEL_FORMAT;   //设置颜色格式
    DMA2D->FGOR=0;                  //前景层行偏移为0
    DMA2D->OOR=offline;             //设置行偏移
    DMA2D->CR&=~(1<<0);             //先停止DMA2D
    DMA2D->FGMAR=(u32)src_addr;        //源地址
    DMA2D->OMAR=addr;               //输出存储器地址
    DMA2D->NLR=(ey-sy+1)|((ex-sx+1)<<16);   //设定行数寄存器
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


static void LTDC_Init(u16 *pFrameBufferFG)
{
    lcdltdc.hsw=20;                 //水平同步宽度
    lcdltdc.vsw=3;                  //垂直同步宽度
    lcdltdc.hbp=140;                //水平后廊
    lcdltdc.vbp=20;                 //垂直后廊
    lcdltdc.hfp=160;                //水平前廊
    lcdltdc.vfp=12;                 //垂直前廊
    LTDC_Clk_Set(360,2,RCC_PLLSAIDIVR_4);//设置像素时钟  45Mhz
    lcdltdc.pixsize=PIXSIZE;              //每个像素占2个字节
    LTDC_Handler.Instance=LTDC;
    LTDC_Handler.Init.HSPolarity=LTDC_HSPOLARITY_AL;         //水平同步极性
    LTDC_Handler.Init.VSPolarity=LTDC_VSPOLARITY_AL;         //垂直同步极性
    LTDC_Handler.Init.DEPolarity=LTDC_DEPOLARITY_AL;         //数据使能极性
    LTDC_Handler.Init.PCPolarity=LTDC_PCPOLARITY_IPC;        //像素时钟极性
    LTDC_Handler.Init.HorizontalSync=lcdltdc.hsw-1;          //水平同步宽度
    LTDC_Handler.Init.VerticalSync=lcdltdc.vsw-1;            //垂直同步宽度
    LTDC_Handler.Init.AccumulatedHBP=lcdltdc.hsw+lcdltdc.hbp-1; //水平同步后沿宽度
    LTDC_Handler.Init.AccumulatedVBP=lcdltdc.vsw+lcdltdc.vbp-1; //垂直同步后沿高度
    LTDC_Handler.Init.AccumulatedActiveW=lcdltdc.hsw+lcdltdc.hbp+lcd_display.width-1;//有效宽度
    LTDC_Handler.Init.AccumulatedActiveH=lcdltdc.vsw+lcdltdc.vbp+lcd_display.height-1;//有效高度
    LTDC_Handler.Init.TotalWidth=lcdltdc.hsw+lcdltdc.hbp+lcd_display.width+lcdltdc.hfp-1;//总宽度
    LTDC_Handler.Init.TotalHeigh=lcdltdc.vsw+lcdltdc.vbp+lcd_display.height+lcdltdc.vfp-1;//总高度
    LTDC_Handler.Init.Backcolor.Red=0;           //屏幕背景层红色部分
    LTDC_Handler.Init.Backcolor.Green=0;         //屏幕背景层绿色部分
    LTDC_Handler.Init.Backcolor.Blue=0;          //屏幕背景色蓝色部分
    LTDC_Handler.State = HAL_LTDC_STATE_RESET;
    HAL_LTDC_Init(&LTDC_Handler);

    //层配置
    LTDC_Layer_Parameter_Config(0,(u32)pFrameBufferFG,LTDC_PIXEL_FORMAT_RGB565,255,0,6,7,0X000000);//层参数配置
    LTDC_Layer_Window_Config(0,0,0,lcd_display.width,lcd_display.height);
    LTDC_Select_Layer(0);           //选择第1层
    __lcd_backlight_onoff(1);

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
bool_t __lcd_disp_ctrl(struct DisplayRsc *disp)
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
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
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
bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
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
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----位图中填充矩形-----------------------------------------------------------
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
      if(Mode != CN_FILLRECT_MODE_N)
          return false;
  //todo DMA2D支持不同颜色格式传递待添加
      if(dst_bitmap->PixelFormat != CN_LCD_PIXEL_FORMAT)
          return false;

      pixel = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,Color0);

      dst = (u16*)((u32)dst_bitmap->bm_bits
                        + Focus->top * dst_bitmap->linebytes);
      dst += Focus->left;
      width=Focus->right-Focus->left;
      height=Focus->top-Focus->bottom;
      if(false==Lock_MutexPend(&Dma2dMutex,DMA2D_timeout))
          return false;
      RCC->AHB1ENR|=1<<23;            //使能DM2D时钟
      DMA2D->CR&=~(1<<0);             //先停止DMA2D
      DMA2D->CR=3<<16;                //寄存器到存储器模式
      DMA2D->OPFCCR=CN_DMA2D_PIXEL_FORMAT;  //设置输出颜色格式
      DMA2D->OOR  = lcd_display.width-width;  //设置行偏移
      DMA2D->OMAR=(u32)dst;               //输出存储器地址
      DMA2D->NLR  = height|(width<<16);   //设定行数寄存器
      DMA2D->OCOLR=pixel;                //设定输出颜色寄存器
      DMA2D->CR|=1<<0;                //启动DMA2D
      timeout =0;
      while((DMA2D->ISR&(1<<1))==0)   //等待传输完成
      {
          timeout+=10;
          Djy_DelayUs(10);
          if(timeout>DMA2D_timeout)
          {
              flag=false;
               break;  //超时退出
          }
      }
      DMA2D->IFCR|=1<<1;              //清除传输完成标志
      bool_t Lock_MutexDelete(struct MutexLCB *mutex);
      return flag;
}
//--------------bitmap中位块传送-----------------------------------------------
//功能: 把一个位图中的一个矩形区域传送到另一个区域，两个位图的像素格式
//参数:  dst_bitmap，目标位图
//      dst_rect，目标矩形
//      src_bitmap，源位图
//      xsrc、ysrc，源位图中被传送的区域左上角坐标
//      pat，pattern指针
//      bitmsk，rop4操作需要的掩码
//      rop_code，三元光栅操作码
//      transparentcolor，透明色
//返回: true=成功,false=失败
//-----------------------------------------------------------------------------

bool_t __lcd_blt_bm_to_bm(struct RectBitmap *dst_bitmap,
						  struct Rectangle *DstRect,
						  struct RectBitmap *src_bitmap,
						  struct Rectangle *SrcRect,
						  struct RopGroup RopCode,u32 transparentcolor)
{
    u16 *src_offset;    //源位图点阵缓冲区可能不对齐!!!
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };

    if((dst_bitmap->PixelFormat != CN_LCD_PIXEL_FORMAT)
             ||(src_bitmap->PixelFormat != CN_LCD_PIXEL_FORMAT)
             ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))!=0))
    {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //源位图缓冲区非对齐模型
            return false;
        else
        {
            src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                         + SrcRect->top * src_bitmap->linebytes);
            src_offset += SrcRect->left;
            LTDC_BitMap_Fill(DstRect->left,DstRect->top,DstRect->right-1,DstRect->bottom-1,src_offset);
        }
    }
    return true;
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
    byteoffset = (y*CN_LCD_XSIZE + x);
    color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);
    dest = (u32)lcdltdc.pFrameBuffe[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);
    lcdltdc.pFrameBuffe[byteoffset] = (u16)color;
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
    color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);

    for(i=0;i<n;i++)
    {
        offset = PixelGroup[i].y*CN_LCD_XSIZE + PixelGroup[i].x;
        dest = (u32)lcdltdc.pFrameBuffe[offset];
        dest = GK_BlendRop2(dest,color,r2_code);

        lcdltdc.pFrameBuffe[offset] = dest;
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
        color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);
        return LTDC_Color_Fill( x1,y1,x2,y2+1,color);

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
        color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);
        return LTDC_Color_Fill( x1,y1,x2+1,y2,color);
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
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
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
    pen = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,Color0);
    flag=LTDC_Color_Fill(Focus->left,Focus->top,Focus->right,Focus->bottom,pen);
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
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
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

    color = (u32)lcdltdc.pFrameBuffe[y*CN_LCD_XSIZE+x];
    e = 0;
    r = (color>>11) &0x1f;
    g = (color>>5) & 0x3f;
    b = color & 0x1f;
    return ((e<<28)|(r<<20) | (g<<12) | (b<<4));
}

//----读取bitmap中矩形块-------------------------------------------------------
//功能: 把一个位图中的矩形读取到另一个位图中。
//参数: rect，欲读取的矩形
//      dest，保存矩形的位图，其长宽必须与rect相同
//返回: true=成功读取，false=失败，原因可能是提供了不支持的像素格式
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}

//----初始化lcd设备------------------------------------------------------------
//功能: 初始化lcd设备。
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------

struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,const char* HeapName)
{
    struct HeapCB *heap;
    u32 Addr;
    heap =M_FindHeap(HeapName);
    if(heap==NULL)
        return NULL;
    //多申请64字节如果显存不是64字节对齐描点的时候会有闪屏的现象
    pLTDCBufferFG1 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*PIXSIZE+0x40,heap,0);
//    pLTDCBufferFG2 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*PIXSIZE,heap,0);//先用一层
    if(0x3f&(u32)pLTDCBufferFG1)
        pLTDCBufferFG1+=(0x40-(0x3f&(u32)pLTDCBufferFG1))/2;

    lcd_display.frame_buffer =GK_GetRootWin(&lcd_display);
    lcd_display.framebuf_direct = true;

    lcd_display.xmm = 0;
    lcd_display.ymm = 0;
    lcd_display.width = CN_LCD_XSIZE;
    lcd_display.height = CN_LCD_YSIZE;
    lcd_display.pixel_format = CN_LCD_PIXEL_FORMAT;


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










