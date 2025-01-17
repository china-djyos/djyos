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
//所属模块:图形模块
//作者：HM
//版本：V1.0.0
//文件描述: MCB1700所带LCD驱动程序,DJYGUI图形模块下所定义宏
//-----------------------------------------------------------------------------
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
#include "cpu_peri.h"
#include "djyos.h"
#include "gkernel.h"
#include <gui/gk_display.h>
#include "lcd.h"
#include "lcd_spidrv.h"
#include "heap.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  ptu32_t LCD_ModuleInit(ptu32_t para);
//  LCD_ModuleInit(CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"board configure of mcb1700net"//组件名
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                      //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                   //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                     //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                   //初始化时机，可选值：early，medium，later, pre-main。
                                     //表示初始化时间，分别是早期、中期、后期
//dependence:"gkernel"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件将强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"              //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件不会被强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                     //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BOARD_CONFIGURE_OF_MCB1700NET == false )
//#warning  " board_configure_of_mcb1700net  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BOARD_CONFIGURE_OF_MCB1700NET    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   240             //"LCD宽度",
#define CFG_LCD_YSIZE   128             //"LCD高度",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   36500            //"LCD宽度-微米数",
#define CFG_LCD_YSIZE_UM   48600            //"LCD高度-微米数",
//%$#@num,0,100,
//%$#@enum,ture,false,
//%$#@string,1,10,
#define CFG_LCD_HEAP_NAME         "sys"      //"堆名称",配置LCD模块分配内存使用的堆
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

// =============================================================================
#define cn_coordinates      0       //0=正常坐标，1=翻转坐标，即原点=(239,319)
struct DisplayObj tg_lcd_display;

#define LANDSCAPE   0                   /* 1 for landscape, 0 for portrait    */
#define ROTATE180   0                   /* 1 to rotate the screen for 180 deg */

static unsigned char Himax;
// =============================================================================

#define DELAY_2N 18
static void delay (int cnt)     //TODO 这个函数必须拿掉,改用OS提供的函数.
{
    cnt <<= DELAY_2N;
    while (cnt--);
}

//以下是mcbqvga的操作函数
/*******************************************************************************
* Write a command the LCD controller                                           *
*   Parameter:    cmd:    command to be written                                *
*   Return:                                                                    *
*******************************************************************************/
static __inline void wr_cmd (unsigned char cmd)
{
//    LCD_CS(0);
//    spi_tran(SPI_START | SPI_WR | SPI_INDEX);   /* Write : RS = 0, RW = 0       */
//    spi_tran(0);
//    spi_tran(cmd);
//    LCD_CS(1);
    u8 data[3];
    data[0] = (u8)(SPI_START | SPI_WR | SPI_INDEX);
    data[1] = 0;
    data[2] = cmd;
    LCD_TxRx(data,3,NULL,0);
}

/*******************************************************************************
* Write data to the LCD controller                                             *
*   Parameter:    dat:    data to be written                                   *
*   Return:                                                                    *
*******************************************************************************/
static __inline void wr_dat (unsigned short dat)
{
//    LCD_CS(0);
//    spi_tran(SPI_START | SPI_WR | SPI_DATA);    /* Write : RS = 1, RW = 0       */
//    spi_tran((dat >>   8));                     /* Write D8..D15                */
//    spi_tran((dat & 0xFF));                     /* Write D0..D7                 */
//    LCD_CS(1);
    u8 data[3];
    data[0] = (u8)(SPI_START | SPI_WR | SPI_DATA);
    data[1] = (u8)(dat >>   8);
    data[2] = (u8)(dat & 0xFF);
    LCD_TxRx(data,3,NULL,0);
}

/*---------------------------------------------------------------------------
功能：  写lcd内部寄存器
---------------------------------------------------------------------------*/
void __mcbqvga_write_reg(u32 reg_index, u32 data)
{
      wr_cmd(reg_index);
      wr_dat(data);
}

/*---------------------------------------------------------------------------
功能：  写命令
---------------------------------------------------------------------------*/
void __mcbqvga_write_cmd(u32 cmd)
{
    wr_cmd(cmd);
}

/*---------------------------------------------------------------------------
功能：  写数据
---------------------------------------------------------------------------*/
//#define __mcbqvga_write_data(data) (LCD_CMD=data)
void __mcbqvga_write_data(u32 data)
{
    wr_dat((u16)data);
}

/*---------------------------------------------------------------------------
功能：  读数据
---------------------------------------------------------------------------*/
u32 __mcbqvga_read_data(void)
{
//  return LCD_CMD;
//      unsigned short val = 0;

//      LCD_CS(0);
//      spi_tran(SPI_START | SPI_RD | SPI_DATA);    /* Read: RS = 1, RW = 1         */
//      spi_tran(0);                                /* Dummy read 1                 */
//      val   = spi_tran(0);                        /* Read D8..D15                 */
//      val <<= 8;
//      val  |= spi_tran(0);                        /* Read D0..D7                  */
//      LCD_CS(1);
//      return (val);

    u8 dout[2],din[2];
    dout[0] = (u8)(SPI_START | SPI_RD | SPI_DATA);
    dout[1] = (u8)(0);
    LCD_TxRx(dout,2,din,2);
    return (u16)((din[0]<<8) + din[1]);
}

void __mcbqvga_set_pos(u32 x,u32 y)
{
    __mcbqvga_write_reg(0x02, x >>    8);            /* Column address start MSB           */
    __mcbqvga_write_reg(0x03, x &  0xFF);            /* Column address start LSB           */
    __mcbqvga_write_reg(0x04, x >>    8);            /* Column address end MSB             */
    __mcbqvga_write_reg(0x05, x &  0xFF);            /* Column address end LSB             */

    __mcbqvga_write_reg(0x06, y >>    8);            /* Row address start MSB              */
    __mcbqvga_write_reg(0x07, y &  0xFF);            /* Row address start LSB              */
    __mcbqvga_write_reg(0x08, y >>    8);            /* Row address end MSB                */
    __mcbqvga_write_reg(0x09, y &  0xFF);            /* Row address end LSB                */
}
/*---------------------------------------------------------------------------
功能：  设置一个像素
---------------------------------------------------------------------------*/
void __mcbqvga_set_pixel(u32 x, u32 y, u16 color)
{
    __mcbqvga_set_pos(x,y);

    __mcbqvga_write_cmd(0x0022);
    __mcbqvga_write_data(color);
}

/*---------------------------------------------------------------------------
功能：  读取一个像素
---------------------------------------------------------------------------*/
u16 __mcbqvga_get_pixel(u32 x, u32 y)
{
    u16 col = 0;//, rgb = 0

    __mcbqvga_set_pos(x,y);

    __mcbqvga_write_cmd(0x0022);
    __mcbqvga_read_data();//dummy read

    col = __mcbqvga_read_data();//BGR 排列
//  rgb = (col & 0x001f)<<11;
//  rgb |= (col & 0xf800)>>11;
//  rgb |= col & 0x07e0;
    return col;
}

/*---------------------------------------------------------------------------
功能：  设置光标位置
        row     :0--319
        column  :0--239
---------------------------------------------------------------------------*/
void __mcbqvga_set_cursor(u32 x, u32 y)
{
    __mcbqvga_set_pos(x,y);
}

/*---------------------------------------------------------------------------
功能：  开窗
---------------------------------------------------------------------------*/
void __mcbqvga_set_window(u32 x,u32 y, u32 wide,u32 high)
{
    unsigned int xe, ye;
    xe = x+wide-1;
    ye = y+high-1;

    __mcbqvga_write_reg(0x02, x  >>    8);           /* Column address start MSB           */
    __mcbqvga_write_reg(0x03, x  &  0xFF);           /* Column address start LSB           */
    __mcbqvga_write_reg(0x04, xe >>    8);           /* Column address end MSB             */
    __mcbqvga_write_reg(0x05, xe &  0xFF);           /* Column address end LSB             */

    __mcbqvga_write_reg(0x06, y  >>    8);           /* Row address start MSB              */
    __mcbqvga_write_reg(0x07, y  &  0xFF);           /* Row address start LSB              */
    __mcbqvga_write_reg(0x08, ye >>    8);           /* Row address end MSB                */
    __mcbqvga_write_reg(0x09, ye &  0xFF);           /* Row address end LSB                */

}

/*---------------------------------------------------------------------------
功能：  关窗
---------------------------------------------------------------------------*/
void __mcbqvga_close_window(void)
{
    __mcbqvga_write_reg(0x0050, 0);//水平起始地址
    __mcbqvga_write_reg(0x0051, 239);//水平结束地址

    __mcbqvga_write_reg(0x0052, 0);//垂直起始地址
    __mcbqvga_write_reg(0x0053, 319);//垂直结束地址

    __mcbqvga_set_cursor(0, 0);//光标设在左上角
}
void lcd_clear(u32 color)
{
    u32 i = 0;

    __mcbqvga_close_window();

    __mcbqvga_write_cmd(0x0022);                                    //写GRAM之前 要先写0x0022
    for (i = 0; i < 320*240 ; i++)
    {
        __mcbqvga_write_data(color);
    }

}

void lcd_reset(void)
{
}

void lcd_backlight_on(void)
{
}

void lcd_backlight_off(void)
{
}

//以下是djygui要求的接口函数，许多情况下，大多数用空函数即可
/*---------------------------------------------------------------------------
功能：    lcd 初始化
---------------------------------------------------------------------------*/
void __lcd_mcbqvga_init(void)
{
//    SPI_HardDrvInit();

    /* LCD with HX8347-D LCD Controller   */
    Himax = 1;  /* Set Himax LCD controller flag      */
    /* Driving ability settings ----------------------------------------------*/
    __mcbqvga_write_reg(0xEA, 0x00);                 /* Power control internal used (1)    */
    __mcbqvga_write_reg(0xEB, 0x20);                 /* Power control internal used (2)    */
    __mcbqvga_write_reg(0xEC, 0x0C);                 /* Source control internal used (1)   */
    __mcbqvga_write_reg(0xED, 0xC7);                 /* Source control internal used (2)   */
    __mcbqvga_write_reg(0xE8, 0x38);                 /* Source output period Normal mode   */
    __mcbqvga_write_reg(0xE9, 0x10);                 /* Source output period Idle mode     */
    __mcbqvga_write_reg(0xF1, 0x01);                 /* RGB 18-bit interface ;0x0110       */
    __mcbqvga_write_reg(0xF2, 0x10);

    /* Adjust the Gamma Curve ------------------------------------------------*/
    __mcbqvga_write_reg(0x40, 0x01);
    __mcbqvga_write_reg(0x41, 0x00);
    __mcbqvga_write_reg(0x42, 0x00);
    __mcbqvga_write_reg(0x43, 0x10);
    __mcbqvga_write_reg(0x44, 0x0E);
    __mcbqvga_write_reg(0x45, 0x24);
    __mcbqvga_write_reg(0x46, 0x04);
    __mcbqvga_write_reg(0x47, 0x50);
    __mcbqvga_write_reg(0x48, 0x02);
    __mcbqvga_write_reg(0x49, 0x13);
    __mcbqvga_write_reg(0x4A, 0x19);
    __mcbqvga_write_reg(0x4B, 0x19);
    __mcbqvga_write_reg(0x4C, 0x16);

    __mcbqvga_write_reg(0x50, 0x1B);
    __mcbqvga_write_reg(0x51, 0x31);
    __mcbqvga_write_reg(0x52, 0x2F);
    __mcbqvga_write_reg(0x53, 0x3F);
    __mcbqvga_write_reg(0x54, 0x3F);
    __mcbqvga_write_reg(0x55, 0x3E);
    __mcbqvga_write_reg(0x56, 0x2F);
    __mcbqvga_write_reg(0x57, 0x7B);
    __mcbqvga_write_reg(0x58, 0x09);
    __mcbqvga_write_reg(0x59, 0x06);
    __mcbqvga_write_reg(0x5A, 0x06);
    __mcbqvga_write_reg(0x5B, 0x0C);
    __mcbqvga_write_reg(0x5C, 0x1D);
    __mcbqvga_write_reg(0x5D, 0xCC);

    /* Power voltage setting -------------------------------------------------*/
    __mcbqvga_write_reg(0x1B, 0x1B);
    __mcbqvga_write_reg(0x1A, 0x01);
    __mcbqvga_write_reg(0x24, 0x2F);
    __mcbqvga_write_reg(0x25, 0x57);
    __mcbqvga_write_reg(0x23, 0x88);

    /* Power on setting ------------------------------------------------------*/
    __mcbqvga_write_reg(0x18, 0x36);                 /* Internal oscillator frequency adj  */
    __mcbqvga_write_reg(0x19, 0x01);                 /* Enable internal oscillator         */
    __mcbqvga_write_reg(0x01, 0x00);                 /* Normal mode, no scrool             */
    __mcbqvga_write_reg(0x1F, 0x88);                 /* Power control 6 - DDVDH Off        */
    delay(20);
    __mcbqvga_write_reg(0x1F, 0x82);                 /* Power control 6 - Step-up: 3 x VCI */
    delay(5);
    __mcbqvga_write_reg(0x1F, 0x92);                 /* Power control 6 - Step-up: On      */
    delay(5);
    __mcbqvga_write_reg(0x1F, 0xD2);                 /* Power control 6 - VCOML active     */
    delay(5);

    /* Color selection -------------------------------------------------------*/
    __mcbqvga_write_reg(0x17, 0x55);                 /* RGB, System interface: 16 Bit/Pixel*/
    __mcbqvga_write_reg(0x00, 0x00);                 /* Scrolling off, no standby          */

    /* Interface config ------------------------------------------------------*/
    __mcbqvga_write_reg(0x2F, 0x11);                 /* LCD Drive: 1-line inversion        */
    __mcbqvga_write_reg(0x31, 0x00);
    __mcbqvga_write_reg(0x32, 0x00);                 /* DPL=0, HSPL=0, VSPL=0, EPL=0       */

    /* Display on setting ----------------------------------------------------*/
    __mcbqvga_write_reg(0x28, 0x38);                 /* PT(0,0) active, VGL/VGL            */
    delay(20);
    __mcbqvga_write_reg(0x28, 0x3C);                 /* Display active, VGL/VGL            */

    #if (LANDSCAPE == 1)
    #if (ROTATE180 == 0)
     __mcbqvga_write_reg (0x16, 0xA8);
    #else
     __mcbqvga_write_reg (0x16, 0x68);
    #endif
    #else
    #if (ROTATE180 == 0)
     __mcbqvga_write_reg (0x16, 0x08);
    #else
     __mcbqvga_write_reg (0x16, 0xC8);
    #endif
    #endif

    /* Display scrolling settings --------------------------------------------*/
    __mcbqvga_write_reg(0x0E, 0x00);                 /* TFA MSB                            */
    __mcbqvga_write_reg(0x0F, 0x00);                 /* TFA LSB                            */
    __mcbqvga_write_reg(0x10, 320 >> 8);             /* VSA MSB                            */
    __mcbqvga_write_reg(0x11, 320 &  0xFF);          /* VSA LSB                            */
    __mcbqvga_write_reg(0x12, 0x00);                 /* BFA MSB                            */
    __mcbqvga_write_reg(0x13, 0x00);                 /* BFA LSB                            */

//  lcd_backlight_on();
      LPC_GPIO4->FIOSET = 0x10000000;
}

/*---------------------------------------------------------------------------
功能：    display on
---------------------------------------------------------------------------*/
void lcd_display_on(void)
{
    __mcbqvga_write_reg(0x0007, 0x0173);
}

/*---------------------------------------------------------------------------
功能：    display off
---------------------------------------------------------------------------*/
void lcd_display_off(void)
{
    __mcbqvga_write_reg(0x0007, 0x0000);
}


bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    u32 y;
    u16 pixel;
    u16 *dst_offset;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);

    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                              + Focus->top * dst_bitmap->linebytes);
    dst_offset += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {
        memset(dst_offset,pixel,(Focus->right-Focus->left)<<1);
        dst_offset += dst_bitmap->linebytes >> 1;
    }
    return true;
}

//本函数用于在最通常的情况下，加速图形绘制。
//绝大多数位图传输操作，源和目标位图格式都是cn_sys_pf_r5g6b5，
bool_t __lcd_blt_bm_to_bm( struct RectBitmap *dst_bitmap,
                            struct Rectangle *DstRect,
                            struct RectBitmap *src_bitmap,
                            struct Rectangle *SrcRect,
                            struct RopGroup RopCode, u32 HyalineColor)
{
    u16 *src_offset,*dst_offset;    //源位图点阵缓冲区可能不对齐!!!
    u32 y;
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0 };

    if((dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))==0))
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
                                      +SrcRect->top * src_bitmap->linebytes);
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
#if (cn_coordinates == 0)   //正常坐标
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __mcbqvga_set_pixel(x,y,pixel);
    }else
    {
        dest = __mcbqvga_get_pixel(x,y);
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __mcbqvga_set_pixel(x,y,pixel);
    }
    return true;
}
//在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//在screen中画一条任意直线，含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen中矩形填充，如硬件加速不支持在screen上矩形填充，或者有frame_buffer，
//driver可以简化，直接返回false即可
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 i,width,height,loop;
    u16 pixel;
    u8 data[3];
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    width = Focus->right-Focus->left;
    height = Focus->bottom-Focus->top;
    __mcbqvga_set_window(Focus->left,Focus->top,width,height);
    __mcbqvga_write_cmd(0x0022);
//    for(y = 0; y < height; y++)
//    {
//        for(x = 0; x < width; x++)
//        {
//            __mcbqvga_write_data(pixel);
//        }
//    }
//    rect_trans_to_screen(height,width,pixel);
    loop = width*height;

    data[0] = (u8)(SPI_START | SPI_WR | SPI_DATA);
    data[1] = (u8)(pixel >>   8);
    data[2] = (u8)(pixel & 0xFF);

    LCD_CS(0);
    spi_tran(data[0]);
    for(i = 0; i <loop;i++)
    {
        spi_tran(data[1]);
        spi_tran(data[2]);
    }
    LCD_CS(1);

    __mcbqvga_close_window();
    return true;
}

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //正常坐标，
    lineoffset = (u16*)((u32)src_bitmap->bm_bits + ysrc*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //正常坐标，
    __mcbqvga_set_window(dst_rect->left,dst_rect->top,width,height);
    __mcbqvga_write_cmd(0x0022);
//    for(y = 0; y < height; y++)
//    {
//        for(x = 0; x < width; x++)
//        {
//            __mcbqvga_write_data(lineoffset[x]);
//        }
//        lineoffset += (src_bitmap->linebytes)>>1;
//    }

    u8 data[3];
    data[0] = (u8)(SPI_START | SPI_WR | SPI_DATA);

    LCD_CS(0);
    spi_tran(data[0]);

    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
//            __mcbqvga_write_data(lineoffset[x]);
            data[1] = (u8)(lineoffset[x] >>   8);
            data[2] = (u8)(lineoffset[x] & 0xFF);
            spi_tran(data[1]);
            spi_tran(data[2]);
        }
        lineoffset += (src_bitmap->linebytes)>>1;
    }
    LCD_CS(1);
    __mcbqvga_close_window();
    return true;
}

//从screen中取一像素，并转换成CN_SYS_PF_ERGB8888
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                        __mcbqvga_get_pixel(x,y),0);
}
#else
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __mcbqvga_set_pixel(CFG_LCD_XSIZE-x,CFG_LCD_YSIZE-y,pixel);   //转换坐标
    }else
    {
        dest = __mcbqvga_get_pixel(CFG_LCD_XSIZE-1-x,CFG_LCD_YSIZE-1-y);  //转换坐标
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __mcbqvga_set_pixel(CFG_LCD_XSIZE-1-x,CFG_LCD_YSIZE-1-y,pixel);   //转换坐标
    }
    return true;
}
//在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}

//在screen中画一条任意直线，含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//screen中矩形填充，如硬件加速不支持在screen上矩形填充，或者有frame_buffer，
//driver可以简化，直接返回false即可
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 x,y,width,height;
    u16 pixel;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    width = Focus->right-Focus->left;
    height = Focus->bottom-Focus->top;
    //转换坐标
    __mcbqvga_set_window(CFG_LCD_XSIZE-Focus->right,
                         CFG_LCD_YSIZE-Focus->bottom,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = 0; x < width; x++)
        {
            __mcbqvga_write_data(pixel);
        }
    }
    __mcbqvga_close_window();
    return true;
}

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 x,y,width,height;
    u16 *lineoffset;
    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    //转换坐标
    lineoffset = (u16*)((u32)src_bitmap->bm_bits
                        + (ysrc+height-1)*src_bitmap->linebytes);
    lineoffset +=xsrc;
    //转换坐标
    __mcbqvga_set_window(CFG_LCD_XSIZE-dst_rect->right,
                         CFG_LCD_YSIZE-dst_rect->bottom,width,height);
    __mcbqvga_write_cmd(0x0022);
    for(y = 0; y < height; y++)
    {
        for(x = width-1; x >= 0; x--)
        {
            __mcbqvga_write_data(lineoffset[x]);
        }
        lineoffset -= (src_bitmap->linebytes)>>1;
    }
    __mcbqvga_close_window();
    return true;
}

//从screen中取一像素，并转换成CN_SYS_PF_ERGB8888
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                    __mcbqvga_get_pixel(CFG_LCD_XSIZE-1-x,CFG_LCD_YSIZE-1-y),0);
}

#endif
//把screen内矩形区域的内容复制到bitmap，调用前，先设置好dest的pf_type
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}

//----控制显示器---------------------------------------------------------------
//功能: 这是由driver提供商编写的，用于设置显示器各种参数的函数，属于应用程序的
//      范畴，功能由设备厂商定义。在ut2416中，本函数为空。
//参数: disp，显示器指针
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct DisplayObj *disp, s32 cmd, ...)
{
    return true;
}

//----初始化lcd设备------------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------
ptu32_t LCD_ModuleInit(ptu32_t para)
{
    LCD_HardInit();
    __lcd_mcbqvga_init( );
    tg_lcd_display.frame_buffer = NULL;

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    tg_lcd_display.width = CFG_LCD_XSIZE;
    tg_lcd_display.height = CFG_LCD_YSIZE;
    tg_lcd_display.pixel_format = CN_SYS_PF_RGB565;
    tg_lcd_display.framebuf_direct = false;
    //无须初始化frame_buffer和desktop，z_topmost三个成员

    tg_lcd_display.draw.SetPixelToBitmap = __lcd_set_pixel_bm;
    tg_lcd_display.draw.LineToBitmap = __lcd_line_bm;
    tg_lcd_display.draw.LineToBitmapIe = __lcd_line_bm_ie;
    tg_lcd_display.draw.FillRectToBitmap = __lcd_fill_rect_bm;
    tg_lcd_display.draw.BltBitmapToBitmap = __lcd_blt_bm_to_bm;
    tg_lcd_display.draw.SetPixelToScreen = __lcd_set_pixel_screen;
    tg_lcd_display.draw.LineToScreen = __lcd_line_screen;
    tg_lcd_display.draw.LineToScreenIe = __lcd_line_screen_ie;
    tg_lcd_display.draw.FillRectToScreen = __lcd_fill_rect_screen;
    tg_lcd_display.draw.CopyBitmapToScreen = __lcd_bm_to_screen;
    tg_lcd_display.draw.GetPixelFromScreen = __lcd_get_pixel_screen;
    tg_lcd_display.draw.GetRectFromScreen = __lcd_get_rect_screen;

    tg_lcd_display.DisplayHeap = (struct HeapCB *)Heap_FindHeap("sys");
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,(char*)para);
    return (ptu32_t)&tg_lcd_display;
}
