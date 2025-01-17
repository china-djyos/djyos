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
//    extern struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,\
//                                          const char* HeapName);
//    ModuleInstall_LCD(CFG_LCD_DISPLAY_NAME,CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip lcd"//CPU内置LCD控制器驱动
//parent:"graphical kernel"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                  //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                 //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                 //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","heap"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件将强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:none            //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                 //选中该组件时，被依赖组件不会被强制选中，
                                 //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                 //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束
    //%$#@num,0,65536,
#define CFG_LCD_XSIZE   240             //"LCD宽度",
#define CFG_LCD_YSIZE   128             //"LCD高度",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   36500            //"LCD宽度-微米数",
#define CFG_LCD_YSIZE_UM   48600            //"LCD高度-微米数",
//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LCD == false )
//#warning  " cpu_onchip_lcd  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LCD    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,32,512,

//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_LCD_DISPLAY_NAME      "LCD_DISPLAY"//"显示器名称",
#define CFG_LCD_HEAP_NAME         "sys"//"显示使用的堆名称",
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define M5D(n)          ((n) & 0x1fffff)    //获得低21位


////////////////////////////////////////////////////////

static struct DisplayObj lcd_display;

static  u16 *pFrameBufferFG;    //前台显示缓冲区.

#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

static struct GkWinObj   frame_win;
static struct RectBitmap frame_bm;

static u16 lcd_buffer_bk[CFG_LCD_XSIZE*CFG_LCD_YSIZE];    //后台显示缓冲区.

#endif

//----初始化lcd硬件------------------------------------------------------------
//功能: 初始化lcd硬件。
//参数: 无
//返回: 无
//备注: 本函数是移植关键函数
//----------------------------------------------------------------------------
static void __lcd_hard_init(u32 lcd_buffer_addr)
{

    struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
    struct LCD_REG *lcd =LCD_REG_BASE;

    //端口 C 的上拉使能寄存器 ,0,使能附加上拉功能到相应端口引脚
    gpio->GPCUP = 0x00000000;
    //GPC4 ,VM;GPC3 VFRAME;GPC2  VLINE;GPC1 VLINE;GPC0 输出。
    //Initialize VD[7:0]，16-31位,VD[23:0] ,（LCD 像素数据输出端口）
    gpio->GPCCON = 0xaaaa02a9;
    gpio->GPDUP = 0x00000000;
    gpio->GPDCON = 0xaaaaaaaa; //Initialize VD[23:8]，0-31位。

    lcd->LCDCON1 = (LCDCON1_CLKVAL<<8)|(LCDCON1_MMODE<<7)|(3<<5)
                            |(12<<1)|0;
    lcd->LCDCON2 = (LCDCON2_VBPD<<24)|((CFG_LCD_XSIZE-1)<<14)|(LCDCON2_VFPD<<6)
                            |(LCDCON2_VSPW);
    lcd->LCDCON3 = (LCDCON3_HBPD<<19)|((CFG_LCD_YSIZE-1)<<8)|(LCDCON3_HFPD);
    lcd->LCDCON4 = (LCDCON4_MVAL<<8)|(LCDCON4_HSPW);
    lcd->LCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7)
                 | (0<<6) | (1<<3)  |(LCDCON5_BSWP<<1) | (LCDCON5_HWSWP);
    lcd->LCDSADDR1 = (((u32)lcd_buffer_addr>>22)<<21)
                            |M5D((uint32_t)lcd_buffer_addr>>1);
    lcd->LCDSADDR2 = M5D(((u32)lcd_buffer_addr
                            +(CFG_LCD_XSIZE*CFG_LCD_YSIZE*2))>>1);
    lcd->LCDSADDR3 = (((CFG_LCD_XSIZE-CFG_LCD_XSIZE)/1)<<11)
                                |(CFG_LCD_XSIZE/1);
    lcd->LCDINTMSK |= (3);         //LCD中断屏蔽寄存器
    lcd->TCONSEL &= (~7) ;       //TCON控制寄存器
    lcd->TPAL = 0;                 //临时调色板寄存器

    lcd->LCDCON1 |= 1;          //允许视频输出和 LCD控制信号

    //LCD PWR
    gpio->GPGUP  = (gpio->GPGUP & (~(1<<4))) | (1<<4); //上拉使能
    gpio->GPGCON = (gpio->GPGCON & (~(3<<8))) | (3<<8);
    gpio->GPGDAT = gpio->GPGDAT | (1<<4) ;

    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<3))) | (1<<3);
    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<5))) |(1<<5);


}

static void __lcd_backlight_onoff(bool_t onoff)
{

}

//----LCD控制信号------------------------------------------------------------
//功能: LCD 视频输出和逻辑使能/禁止，1=允许视频输出和 LCD控制信号，
//      0=禁止视频输出和 LCD控制信号。
//参数: onoff，控制信号
//返回: 无
//-----------------------------------------------------------------------------
static void __lcd_envid_of(int onoff)
{
    struct LCD_REG *lcd =LCD_REG_BASE;

    if(onoff == 1)
        lcd->LCDCON1 |= 1;   //允许视频输出和 LCD控制信号
    else                        //禁止视频输出和 LCD控制信号
        lcd->LCDCON1 = lcd->LCDCON1 & 0x3fffe;
}
//----控制LCD电源--------------------------------------------------------------
//功能: LCD电源控制引脚使能。
//参数: invpwren，表明PWREN信号极性
//      pwren，LCD_PWREN输出信号使能/禁止
//返回: 无
//-----------------------------------------------------------------------------
static void __lcd_power_enable(int invpwren,int pwren)
{
    struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
    struct LCD_REG *lcd =LCD_REG_BASE;

    gpio->GPGUP  = (gpio->GPGUP & (~(1<<4))) | (1<<4); //上拉使能
    gpio->GPGCON = (gpio->GPGCON & (~(3<<8))) | (3<<8);
    gpio->GPGDAT = gpio->GPGDAT | (1<<4) ;

    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<3))) | (pwren<<3);
    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<5))) |(invpwren<<5);
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
                (move_x > 0) && (Sum > OutConst)) {
                move_x--;
            }
            if (y)
            {
                move_y = cy - y - dy;
                if (move_y >= Focus->top&&move_y < Focus->bottom)//上边弧start
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
                    for (; x < Focus->right; x++)
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
                    for (; x < Focus->right; x++)
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

    color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);

    dest = (u32)pFrameBufferFG[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);
    pFrameBufferFG[byteoffset] = dest;
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
        offset = PixelGroup[i].y*CFG_LCD_XSIZE + PixelGroup[i].x;
        dest = (u32)pFrameBufferFG[offset];
        dest = GK_BlendRop2(dest,color,r2_code);

        pFrameBufferFG[offset] = dest;
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
    pen = __GK_ConvertColorToPF(CN_LCD_PIXEL_FORMAT,Color0);

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


    pFrameBufferFG =M_MallocHeap(CFG_LCD_XSIZE*CFG_LCD_YSIZE *2,heap,0);

    __lcd_hard_init((u32)pFrameBufferFG);

    //无须初始化frame_buffer和desktop，z_topmost三个成员
#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

    frame_bm.PixelFormat = CN_LCD_PIXEL_FORMAT;
    frame_bm.width = CFG_LCD_XSIZE;
    frame_bm.height = CFG_LCD_YSIZE;
    frame_bm.linebytes = CFG_LCD_XSIZE *2;
    frame_bm.bm_bits = (u8*)lcd_buffer_bk;
    frame_win.wm_bitmap =&frame_bm;

    lcd_display.frame_buffer = &frame_win;
    lcd_display.framebuf_direct = false;

#else

    lcd_display.frame_buffer =NULL;
    lcd_display.framebuf_direct = true;

#endif

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    lcd_display.width = CFG_LCD_XSIZE;
    lcd_display.height = CFG_LCD_YSIZE;
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






