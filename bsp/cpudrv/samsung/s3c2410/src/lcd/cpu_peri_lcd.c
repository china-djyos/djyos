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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------


#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

#include "board-config.h"
#include "gkernel.h"
#include <gui/gk_display.h>
#include "cpu_peri.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,const char* HeapName);
//    ModuleInstall_LCD(CFG_LCD_DISP_NAME,CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip lcd"//CPU内置LCD控制器驱动
//parent:"graphical kernel"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LCD == false )
//#warning  " cpu_onchip_lcd  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LCD    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
    //%$#@num,0,65536,
#define CFG_LCD_XSIZE   240             //"LCD宽度",
#define CFG_LCD_YSIZE   128             //"LCD高度",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   36500            //"LCD宽度-微米数",
#define CFG_LCD_YSIZE_UM   48600            //"LCD高度-微米数",

//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_LCD_DISP_NAME              "LCD_S3C2416"   //"显示器名称",
#define CFG_LCD_HEAP_NAME               "extram"       //"显示器使用堆的名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#if 1

#if CN_LCD_PIXEL_FORMAT ==CN_SYS_PF_RGB565
#define LCD_BPP     16
#elif   CN_LCD_PIXEL_FORMAT ==CN_SYS_PF_ERGB8888
#define LCD_BPP     24
#else
#error LCD_PIXEL_FORMAT不能支持!
#endif


#define LCD_FRAME_RATE  50

#define VS_INV      1
#define HS_INV      1
#define VCLK_INV    1
#define VDEN_INV    1

#define VFPD    2
#define VBPD    2
#define VSPW    10

#define HFPD    2
#define HBPD    2
#define HSPW    41


//* VIDCON0

#define VIDOUT      (0)
#define L1_DATA16   (4)
#define L0_DATA16   (4)
#define PNRMODE     (0)
#define CLKVALUP    (1)
#define CLKVAL_F    (6)
#define CLKDIR      (1)
#define CLKSEL_F    (0)



//* WINCON1to4
#if(LCD_BPP==16)
#define BITSWP      (0) //shift on basis of half-word swap
#define BYTSWP      (0) //shift on basis of half-word swap
#define HAWSWP      (1) //shift on basis of half-word swap
#endif

#if(LCD_BPP==24)
#define BITSWP      (0) //shift on basis of half-word swap
#define BYTSWP      (0) //shift on basis of half-word swap
#define HAWSWP      (0) //shift on basis of half-word swap
#endif

#define WINCONx_4WORD_BURST     (2)
#define WINCONx_8WORD_BURST     (1)
#define WINCONx_16WORD_BURST        (0)
#define WINCONx_PLANE_BLENDING      (0)
#define WINCONx_PIXEL_BLENDING      (1)
#define WINCONx_1BPP_PALLET     (0)
#define WINCONx_2BPP_PALLET     (1)
#define WINCONx_4BPP_PALLET     (2)
#define WINCONx_8BPP_PALLET     (3)
#define WINCONx_8BPP_NO_PALLET      (4)
#define WINCONx_16BPP_565       (5)
#define WINCONx_16BPP_A555      (6)
#define WINCONx_16BPP_1555      (7)
#define WINCONx_18BPP_666       (8)
#define WINCONx_18BPP_A665      (9)
#define WINCONx_19BPP_A666      (10)
#define WINCONx_24BPP_888       (11)
#define WINCONx_24BPP_A887      (12)
#define WINCONx_25BPP_A888      (13)
#define WINCONx_ALPHA_MODE_0        (0)
#define WINCONx_ALPHA_MODE_1        (1)


static  u16 *pFrameBufferFG;    //前台显示缓冲区.

//----初始化lcd硬件------------------------------------------------------------
//功能: 初始化lcd硬件。
//参数: 无
//返回: 无
//备注: 本函数是移植关键函数
//----------------------------------------------------------------------------
static void __lcd_hard_init(u32 lcd_buffer_addr)
{
    u8 clk_div=0;
    u16 hsync_cnt,vclk_cnt;
    u32 clk_mhz,clk_lcd_src;

    struct GPIO_REG *gpio =GPIO_REG_BASE;
    struct LCD_REG *lcd =LCD_REG_BASE;

    gpio->MISCCR |= (1<<28);

    gpio->GPCCON = 0xAAAAAAAA;
    gpio->GPCUDP = 0xAAAAAAAA;

    gpio->GPDCON = 0xAAAAAAAA;
    gpio->GPDUDP = 0xAAAAAAAA;

    hsync_cnt = (HFPD+HBPD+HSPW+ CFG_LCD_XSIZE);
    vclk_cnt  = (VFPD+VBPD+VSPW+ CFG_LCD_YSIZE);

    lcd->WINCON0 &= ~0x01;
    lcd->WINCON1 &= ~0x01;
    lcd->VIDCON0 &= (~3); // ENVID Off
    //LCD_CLK =HCLK
    clk_lcd_src = (133*1000000);
   // clk_div = (u8)(clk_lcd_src/(hsync_cnt*vclk_cnt*LCD_FRAME_RATE))-1;
    clk_div = 7;

    lcd->VIDCON0 = (VIDOUT<<22)|(L1_DATA16<<19)|(L0_DATA16<<16)|(PNRMODE<<13)|(CLKVALUP<<12)|(clk_div<<6)|(0<<5)|(CLKDIR<<4)|(0<<2)|(0<<0);
    lcd->VIDCON1 = (VCLK_INV<<7)|(HS_INV<<6)|(VS_INV<<5)|(VDEN_INV<<4);
    lcd->VIDTCON0 = ((VBPD-1)<<16)|((VFPD-1)<<8)|(VSPW-1);
    lcd->VIDTCON1 = ((HBPD-1)<<16)|((HFPD-1)<<8)|(HSPW-1);
    lcd->VIDTCON2 = ((CFG_LCD_YSIZE-1)<<11)|(CFG_LCD_XSIZE-1);

    lcd->VIDOSD0A = (0<<11)|(0);
    lcd->VIDOSD0B = (CFG_LCD_XSIZE-1)<<11|(CFG_LCD_YSIZE-1);

    #if(LCD_BPP==16)
    lcd->WINCON0 = (BITSWP<<18)|(BYTSWP<<17)|(HAWSWP<<16)|(WINCONx_16WORD_BURST<<9)|(WINCONx_16BPP_565<<2); // 16word burst, 16bpp,

    lcd->VIDW00ADD0B0 = (u32)lcd_buffer_addr;
    lcd->VIDW00ADD1B0 = (u32)lcd_buffer_addr + CFG_LCD_XSIZE*CFG_LCD_YSIZE*2;
    lcd->VIDW00ADD2B0 = (0<<13)|(CFG_LCD_XSIZE*2);
    #endif

    #if(LCD_BPP==24)
    lcd->rWINCON0 = (BITSWP<<18)|(BYTSWP<<17)|(HAWSWP<<16)|(WINCONx_16WORD_BURST<<9)|(WINCONx_24BPP_888<<2); // 16word burst, 24bpp,

    lcd->VIDW00ADD0B0 = (u32)lcd_buffer_addr;
    lcd->VIDW00ADD1B0 = (u32)lcd_buffer_addr + CFG_LCD_XSIZE*CFG_LCD_YSIZE*4;
    lcd->VIDW00ADD2B0 = (0<<13)|(CFG_LCD_XSIZE*4);
    #endif


    lcd->DITHMODE = (1<<5)|(1<<3)|(1<<1)|(1<<0);

    lcd->WINCON0 |= 0x1;
    lcd->VIDCON0 |= (1<<5)|0x3;

}

static  void __lcd_backlight_onoff(bool_t onoff)
{
  // BKLIGHT: GPC0(tq2416)
  struct GPIO_REG *gpio =GPIO_REG_BASE;

  gpio->GPCCON &= ~(0x3<<0);
  gpio->GPCCON |=  (0x1<<0);

  if(onoff!=false)
  { //ON
      gpio->GPCDAT |=  (1<<0);

  }
  else
  { //OFF
      gpio->GPCDAT &= ~(1<<0);
  }

}

//----控制显示器---------------------------------------------------------------
//功能: 这是由driver提供商编写的，用于设置显示器各种参数的函数，属于应用程序的
//      范畴，功能由设备厂商定义。
//参数: disp，显示器指针
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct Displayobj *disp, s32 cmd, ...)
{
    return true;
}
//----格式化pattern------------------------------------------------------------
//功能: 把用户提交的作为pattern的bitmap格式化成driver能识别的格式。
//参数: pattern，保存结果的指针
//      bitmap，用户提供的位图
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_format_pattern(u8 *pattern,struct RectBitmap *bitmap)
{
    s32 x,y;
    u32 color,bytes;

    if(bitmap->PixelFormat == CN_LCD_PIXEL_FORMAT)
        return true;
    bytes = 0;
    for(y = 0;y < bitmap->height;y++)
    {
        for(x = 0;x < bitmap->width;x++)
        {
            color = GK_GetPixelBm(bitmap,x,y);
            color = GK_ConvertPF2PF(CN_LCD_PIXEL_FORMAT,bitmap->PixelFormat,color,NULL);
            pattern[bytes] = (u8)color;
            pattern[bytes+1] = (u8)(color>>8);
            bytes += 2;
        }
    }
    return true;
}
//----位图中画像素-------------------------------------------------------------
//功能: 在位图中画一个像素，只有在bitmap的像素格式为cn_custom_pf时，才需要绘制。
//      如果显卡不打算支持自定义格式，本函数直接返回。
//参数: bitmap，目标位图
//      limit，限制矩形，只绘制在该矩形内部的部分
//      x、y，坐标
//      color，绘图用的颜色，CN_SYS_PF_ERGB8888格式
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
//      color，绘图用的颜色，CN_SYS_PF_ERGB8888格式
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
//      color，绘图用的颜色，CN_SYS_PF_ERGB8888格式
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----位图中填充矩形-----------------------------------------------------------
//功能: 把位图中的矩形用color颜色填充。
//参数: bitmap，目标位图
//      dst_rect，待填充的矩形
//      color，填充颜色，CN_SYS_PF_ERGB8888格式
//返回: true=成功绘制，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode,s32 radius)
{
    s32 x, y;
    u16 pixel;
    u16 *dst,*p;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;

    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    if (radius == 0)
    {
        dst = (u16*)((u32)dst_bitmap->bm_bits
            + Focus->top * dst_bitmap->linebytes);
        dst += Focus->left;

        for (y = Focus->top; y < Focus->bottom; y++)
        {
            p = dst;
            for (x = Focus->left; x < Focus->right; x++)
            {
                *p++ = pixel;
            }
            dst += dst_bitmap->linebytes >> 1;
        }
        return true;
    }
    else//圆角矩形
    {
        s32 OutConst, Sum, SumY;
        s32 move_x, move_y, flag, flag_x;
        s32 cx, cy, dy, dx, arc_ry, arc_rx, _max;
        cx = ((Target->right - Target->left) / 2) + Target->left;//中心点坐标
        cy = ((Target->bottom - Target->top) / 2) + Target->top;

        if (radius * 2 <= (Target->bottom - Target->top))
        {
            arc_ry = radius;
            dy = cy - Target->top - arc_ry;
        }
        else
        {
            arc_ry = (Target->bottom - Target->top) >> 1;
            dy = 0;
        }
        if (radius * 2 <= (Target->right - Target->left))
        {
            arc_rx = radius;
            dx = cx - Target->left - arc_rx;
        }
        else
        {
            arc_rx = (Target->right - Target->left) >> 1;
            dx = 0;
        }
        u32 _rx = arc_rx;
        u32 _ry = arc_ry;
        flag = ((Target->bottom - Target->top) % 2 == 0 ? 1 : 0);
        flag_x = ((Target->right - Target->left) % 2 == 0 ? 0 : 1);
        OutConst = _rx * _rx*_ry*_ry
            + (_rx*_rx*_ry >> 1);
        move_x = arc_rx;
        for (y = 0; y <= arc_ry; y++)
        {
            SumY = ((s32)(arc_rx*arc_rx))*((s32)(y*y));
            while (Sum = SumY + ((s32)(arc_ry*arc_ry))*((s32)(move_x*move_x)),
                (move_x > 0) && (Sum > OutConst)) {move_x--;}
            if (y)
            {
                move_y = cy - y - dy;
                if (move_y >= Focus->top && move_y < Focus->bottom)//上边弧start
                {
                    dst = (u16*)((u32)dst_bitmap->bm_bits
                        + move_y * dst_bitmap->linebytes);

                    x = cx - move_x - dx;
                    if (x <= Focus->left) x = Focus->left;
                    else if (x >= Focus->right) x = Focus->right;

                    _max = cx + move_x + dx + flag_x;
                    if (_max >= Focus->right) _max = Focus->right;
                    else if (_max <= Focus->left) _max = Focus->left;

                    dst += x;
                    p = dst;
                    for (; x < _max; x++)
                    {
                        *p++ = pixel;
                    }

                }//上边弧end
                move_y = cy + y + dy - flag;
                if (move_y < Focus->bottom&&move_y >= Focus->top)//下边弧start
                {
                    dst = (u16*)((u32)dst_bitmap->bm_bits
                        + move_y * dst_bitmap->linebytes);

                    x = cx - move_x - dx;
                    if (x <= Focus->left) x = Focus->left;
                    else if (x >= Focus->right) x = Focus->right;

                    _max = cx + move_x + dx + flag_x;
                    if (_max >= Focus->right) _max = Focus->right;
                    else if (_max <= Focus->left) _max = Focus->left;

                    dst += x;
                    p = dst;
                    for (; x < _max; x++)
                    {
                        *p++ = pixel;
                    }
                }//下边弧end
            }

        }
        //*********中间矩形部分start
        if (cy - dy <= Focus->top) y = Focus->top;//矩形高度
        else y = cy - dy;
        if (cy + dy + 1 >= Focus->bottom) _max = Focus->bottom;
        else _max = cy + dy + 1;
        dst = (u16*)((u32)dst_bitmap->bm_bits
            + y * dst_bitmap->linebytes);
        dst += Focus->left;
        for (; y < _max; y++)
        {
            p = dst;
            for (x = Focus->left; x < Focus->right; x++)
            {
                *p++ = pixel;
            }
            dst += dst_bitmap->linebytes >> 1;

        }//*********中间矩形部分end

        return true;
    }//圆角矩形
}
//----bitmap中位块传送---------------------------------------------------------
//功能: 把一个位图中的一个矩形区域传送到另一个区域，两个位图的像素格式不必相同，
//      源和目的位图可以相同，
//参数: dst_bitmap，目标位图
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
                        struct RopGroup RopCode,u32 HyalineColor)
{
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    u16 *src_offset,*dst_offset;    //源位图点阵缓冲区可能不对齐!!!
    u32 y;
    if((dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
      ||(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
      ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))!=0))
    {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //源位图缓冲区非对齐模型
        {
        }else
        {
            dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                                      + DstRect->top * dst_bitmap->linebytes);
            dst_offset += DstRect->left;

            src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                                      + SrcRect->top * src_bitmap->linebytes);
            src_offset += SrcRect->left;

            for(y = DstRect->top; y < DstRect->bottom; y++)
            {
                memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<1);
                dst_offset += dst_bitmap->linebytes >> 1;
                src_offset += src_bitmap->linebytes >> 1;
            }
        }
    }
    return true;
}
//----读取位图中的像素---------------------------------------------------------
//功能: 取位图中的一个像素，只有在bitmap的像素格式为cn_custom_pf时，才需要读取。
//      如果显卡不打算支持自定义格式，本函数直接返回false。
//参数: bitmap，目标位图
//      x、y，坐标
//返回: 读取的像素值，CN_SYS_PF_ERGB8888格式
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_bm(struct RectBitmap *bitmap,s32 x,s32 y)
{
    return 0;
}
//----读取bitmap中矩形块-------------------------------------------------------
//功能: 把一个位图中的矩形读取到另一个位图中。
//参数: src，源位图
//      rect，欲读取的矩形
//      dest，保存矩形的位图，其长宽必须与rect相同
//返回: true=成功读取，false=失败，原因可能是提供了不支持的像素格式
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_bm(struct RectBitmap *src,struct Rectangle *rect,
                     struct RectBitmap *dest)
{
    return false;
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
    byteoffset = y*CFG_LCD_XSIZE + x;

    color = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);

    dest = (u32)pFrameBufferFG[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);
    pFrameBufferFG[byteoffset] = (u16)dest;
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
    return false;
}
//----screen中画线(含端点)------------------------------------------------------
//功能: 在screen中画一条直线，只绘制在limit限定的区域内的部分。
//参数: limit，限制矩形，只绘制在该矩形内部的部分
//      x1、y1、x2、y2，起点终点坐标
//      color，绘图用的颜色，CN_SYS_PF_ERGB8888格式
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=失败，无硬件加速或不支持按r2_code画线
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen中填充矩形-----------------------------------------------------------
//功能: 把screen中的矩形用color颜色填充，color的格式是CN_SYS_PF_ERGB8888。
//参数: dst_rect，待填充的矩形
//      color，填充颜色
//返回: true=成功绘制，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    int x,y;
    u32 pen;
    u16 *p;
    return false;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = __GK_ConvertColorToPF(CN_SYS_PF_RGB565,Color0);

    for(y=Focus->top;y<Focus->bottom;y++)
    {
        p =&pFrameBufferFG[y*CFG_LCD_XSIZE];

        for(x=Focus->left;x<Focus->right;x++)
        {
            *p++ =pen;
        }
    }
    return true;
}

//----bitmap到screen位块传送---------------------------------------------------
//功能: 把一个位图中的一个矩形区域传送到另一个区域，两个位图的像素格式必须相同，
//      源和目的位图可以相同。
//参数: dst_rect，目标矩形
//      src_bitmap，源位图
//      xsrc、ysrc，源位图中被传送的区域左上角坐标(以源位图左上角坐标为原点)
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
                struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 size,double_width_src;
    u32 byteoffset_rect,byteoffset_bitmap;
    s32 y;

    if(src_bitmap->PixelFormat!=CN_LCD_PIXEL_FORMAT)
    {   //如果源位图与LCD格式不相同
        return false;
    }

    double_width_src = src_bitmap->width*2;     //源位图一行所占的字节数
    //每一行要被传送的字节数
    size = (dst_rect->right-dst_rect->left)*(CN_LCD_PIXEL_FORMAT>>8)/8;
    //源位图左上角坐标的字节偏移量
    byteoffset_bitmap = ysrc*double_width_src + xsrc*2;
    //目标矩形左上角坐标的双字节偏移量，因为pg_video_buf为u16的指针
    //因为是传送到screen上，所以每行双字节数取cn_lcd_xsize
    byteoffset_rect = dst_rect->top*CFG_LCD_XSIZE + dst_rect->left;

    //bitmap到screen位块传送
    for(y = dst_rect->top;y < dst_rect->bottom;y++)
    {
        memcpy(&(pFrameBufferFG[byteoffset_rect]),
                    &src_bitmap->bm_bits[byteoffset_bitmap],size);
        byteoffset_bitmap += double_width_src;
        byteoffset_rect += CFG_LCD_XSIZE;
    }
    return true;
}
//----从screen中取像素---------------------------------------------------------
//功能: 从screen中取一像素，并转换成CN_SYS_PF_ERGB8888或cn_sys_pf_a8r8g8b8格式。
//参数: x、y，坐标
//返回: 像素颜色值
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 r,g,b,color,offset;

    offset = (u32)pFrameBufferFG;
    offset += y*CFG_LCD_XSIZE*2;
    offset += x*2;
    color = (u32)(*(u16*)offset);
    r = (color>>11) &0x1f;
    g = (color>>5) & 0x3f;
    b = color & 0x1f;
    return (r<<19) | (g<<10) | (b<<3);
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

////////////////////////////////////////////////////////

static struct DisplayObj lcd_display;
static struct GkWinObj   frame_win;

static  u16 lcd_buffer_fg[CFG_LCD_XSIZE*CFG_LCD_YSIZE];


#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

static struct RectBitmap frame_bm;


static  u16 lcd_buffer_bk[CFG_LCD_XSIZE*CFG_LCD_YSIZE];
static  u16 *pFrameBufferBK;    //后台显示缓冲区.
#endif

//----初始化lcd设备------------------------------------------------------------
//功能: 初始化lcd设备。
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------

struct DisplayObj* ModuleInstall_LCD(const char *DisplayName,const char* HeapName)
{
    struct HeapCB *heap;

    __lcd_backlight_onoff(false);

    heap =Heap_FindHeap(HeapName);
    if(heap==NULL)
    {
        return NULL;
    }

   // pFrameBufferFG =lcd_buffer_fg;
    pFrameBufferFG =M_MallocHeap(CFG_LCD_XSIZE*CFG_LCD_YSIZE *2,heap,0);

    __lcd_hard_init(pFrameBufferFG);

    //无须初始化frame_buffer和desktop，z_topmost三个成员
#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

    pFrameBufferBK =lcd_buffer_bk;
    frame_bm.PixelFormat = CN_LCD_PIXEL_FORMAT;
    frame_bm.width = CFG_LCD_XSIZE;
    frame_bm.height = CFG_LCD_YSIZE;
    frame_bm.linebytes = CFG_LCD_XSIZE *2;
    frame_bm.bm_bits = (u8*)pFrameBufferBK;
    frame_win.wm_bitmap =&frame_bm;

    lcd_display.frame_buffer = &frame_win;
    lcd_display.framebuf_direct = false;
    //lcd_display.reset_clip = false;
#else

    lcd_display.frame_buffer =NULL;
    lcd_display.framebuf_direct = true;
    //lcd_display.reset_clip = true;

#endif

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    lcd_display.width = CFG_LCD_XSIZE;
    lcd_display.height = CFG_LCD_YSIZE;
    lcd_display.pixel_format = CN_LCD_PIXEL_FORMAT;


    lcd_display.draw.set_pixel_bm = __lcd_set_pixel_bm;
    lcd_display.draw.fill_rect_bm = __lcd_fill_rect_bm;
    lcd_display.draw.get_pixel_bm = __lcd_get_pixel_bm;
    lcd_display.draw.line_bm = __lcd_line_bm;
    lcd_display.draw.line_bm_ie = __lcd_line_bm_ie;
    lcd_display.draw.blt_bm_to_bm = __lcd_blt_bm_to_bm;
    lcd_display.draw.get_rect_bm = __lcd_get_rect_bm;
//    lcd_display.draw.check_raster = __lcd_check_raster;
    lcd_display.draw.set_pixel_screen = __lcd_set_pixel_screen;
    lcd_display.draw.line_screen = __lcd_line_screen;
    lcd_display.draw.line_screen_ie = __lcd_line_screen_ie;
    lcd_display.draw.fill_rect_screen = __lcd_fill_rect_screen;
    lcd_display.draw.bm_to_screen = __lcd_bm_to_screen;
    lcd_display.draw.get_pixel_screen = __lcd_get_pixel_screen;
    lcd_display.draw.get_rect_screen = __lcd_get_rect_screen;


    lcd_display.DisplayHeap =  heap;
    lcd_display.disp_ctrl = __lcd_disp_ctrl;

    if(GK_InstallDisplay(&lcd_display,DisplayName))
    {
        __lcd_backlight_onoff(true);
        return &lcd_display;
    }
    else
    {

        return NULL;
    }
}

#endif
