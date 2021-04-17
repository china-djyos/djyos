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
//所属模块:图形
//作者：lst
//版本：V1.0.0
//文件描述: ili9325驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-03-10
//   作者: lst
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "stdlib.h"
#include "string.h"

#include "gkernel.h"
#include <gui/gk_display.h>
//#include "board.h"
//#include "GC9307.h"
#include "djyos.h"

#include "cpu_peri.h"
#include "systime.h"
#include "spi_pub.h"

#if 1
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern ptu32_t ModuleInstall_GC9307(const char *DisplayName,const char* HeapName);
//  ModuleInstall_GC9307(CFG_GC9307_DISPLAY_NAME,CFG_GC9307_HEAP_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"LCD driver GC9307"//LCD显示驱动
//parent:"graphical kernel"   //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","heap","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#if ( CFG_MODULE_ENABLE_LCD_DRIVER_GC9307 == false )
//#warning  " keyboard_hard_driver  组件参数未配置，使用默认配置"
#define CFG_MODULE_ENABLE_LCD_DRIVER_GC9307    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   240             //"LCD宽度-像素数",
#define CFG_LCD_YSIZE   240             //"LCD高度-像素数",
//%$#@num,0,0,
#define CFG_LCD_XSIZE_UM   36720            //"LCD宽度-微米数",
#define CFG_LCD_YSIZE_UM   48960            //"LCD高度-微米数",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_GC9307_DISPLAY_NAME        "lcdGC9307"    //"显示器名称",配置液晶显示的名称
#define CFG_GC9307_HEAP_NAME           "extram"        //"驱动使用堆名",配置液晶驱动所使用的堆名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#endif

#define cn_coordinates      0      //0=正常坐标，1=翻转坐标，即原点=(239,319)

#define cn_lcd_line_size        (CFG_LCD_XSIZE<<1)
#define cn_frame_buffer_size    (cn_lcd_line_size * CFG_LCD_YSIZE)
#define CN_LCD_PIXEL_FORMAT     CN_SYS_PF_RGB565

struct DisplayObj tg_lcd_display;

#define ROW_TEMP 50
#define COLO_DEP 2
//#define ROW_BUFFER   40*240*2
#if 0
//unsigned char screen_2[CFG_LCD_XSIZE][COLO_DEP];
//#else
unsigned char screen[ROW_TEMP][240][COLO_DEP];
#endif

//#define LED_ON(x)    djy_gpio_write(GPIO36,!(x))
#if 0
void SPI_CS (int stat)
{
    u32 param;
    if (stat == 0) {
        param = 0x2;
    }
    else {
        param = 0x3;
    }
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
}
#endif

//=====================================================================
//函数名：spi复位，在board中定义
//参数：
//返回值：
//功能：
//=====================================================================
__attribute__((weak)) void SPI_RST(u32 value)
{
    error_printf("GC9307","board undefined SPI_RST \r\n");
}
//=====================================================================
//函数名：用于控制发送给屏的是数据还是命令，在board中定义
//参数：
//返回值：
//功能：
//=====================================================================
__attribute__((weak)) void SPI_RS(u32 value)
{
    error_printf("GC9307","board undefined SPI_RS \r\n");
}

//=====================================================================
//函数名：用于SPI片选，在board中定义
//参数：
//返回值：
//功能：
//=====================================================================
__attribute__((weak)) void GC9307_CsActive(void)
{
    error_printf("GC9307","board undefined GC9307_CsActive \r\n");
}
//=====================================================================
//函数名：用于取消SPI片选，在board中定义
//参数：
//返回值：
//功能：
//=====================================================================
__attribute__((weak)) void GC9307_CsInactive(void)
{
    error_printf("GC9307","board undefined GC9307_CsInactive \r\n");
}

//=====================================================================
//函数名：在board里定义，初始化RST、RS和片选3个引脚
//参数：
//返回值：
//功能：
//=====================================================================
__attribute__((weak)) void GC9307_GpioInit(void)
{
    error_printf("GC9307","board undefined GC9307_GpioInit \r\n");
//    bk_gpio_config_output(GPIO36);
//    bk_gpio_output(GPIO36, GPIO_INT_LEVEL_LOW);
}

//----------------------------------------------------------------------
void  SendDataSPI(unsigned char dat)
{

   struct spi_message spi_msg;
   spi_msg.recv_buf = 0;
   spi_msg.recv_len = 0;
   spi_msg.send_buf = &dat;
   spi_msg.send_len = 1;

   bk_spi_master_xfer(&spi_msg);

   //return 0;
}

int  SendRecvDataSPI(unsigned char *send_buf, int slen, unsigned char *recv_buf, int rlen)
{
   int ret = 0;
   struct spi_message spi_msg;
   GC9307_CsActive();
   SPI_RS(0);
   spi_msg.recv_buf = 0;
   spi_msg.recv_len = 0;
   spi_msg.send_buf = send_buf;
   spi_msg.send_len = slen;
   bk_spi_master_xfer(&spi_msg);
   SPI_RS(1);
   spi_msg.recv_buf = recv_buf;
   spi_msg.recv_len = rlen;
   spi_msg.send_buf = 0;
   spi_msg.send_len = 0;
   ret = bk_spi_master_xfer(&spi_msg);
   GC9307_CsInactive();
   return 0;
}

void WriteDataBytes(unsigned char *data, int len)
{
    u32 param;
    GC9307_CsActive();
    SPI_RS(1);
    s32 result;
    struct spi_message spi_msg;
    spi_msg.recv_buf = 0;
    spi_msg.recv_len = 0;
    spi_msg.send_buf = data;
    spi_msg.send_len = len;

    param = 1;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, &param);
    result = bk_spi_master_xfer(&spi_msg);
    param = 0;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, &param);

    GC9307_CsInactive();
    //return 0;
}


void WriteComm(unsigned char i)
{
    GC9307_CsActive();
    SPI_RS(0);
    SendDataSPI(i);
    GC9307_CsInactive();
}
void WriteData(unsigned char i)
{
    GC9307_CsActive();
    SPI_RS(1);
    SendDataSPI(i);
    GC9307_CsInactive();
}
void WriteDispData(unsigned char DataH,unsigned char DataL)
{
     GC9307_CsActive();
     SPI_RS(1);
     SendDataSPI(DataH);
     SendDataSPI(DataL);
     GC9307_CsInactive();

}
void WriteOneDot(unsigned long int color)
{
    GC9307_CsActive();
    SPI_RS(1);
    SendDataSPI(color>>16);
    SendDataSPI(color>>8);
    SendDataSPI(color);
    GC9307_CsInactive();
}

void __GC9307_write_cmd(unsigned char cmd)
{
    GC9307_CsActive();
    SPI_RS(0);
    SendDataSPI(cmd);
    GC9307_CsInactive();
}
void __GC9307_write_data(unsigned char data)
{
    GC9307_CsActive();
    SPI_RS(1);
    SendDataSPI(data);
    GC9307_CsInactive();
}


static void spi_flash_enable_voltage(void)
{
    UINT32 param;

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &param);

    param = QSPI_IO_3_3V;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_IO_VOLTAGE, &param);

    param = PSRAM_VDD_3_3V_DEF;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_VDDRAM_VOLTAGE, &param);
}

static int __GC9307_read_reg()
{
    return 0x7796;
}


//以下是djygui要求的接口函数，许多情况下，大多数用空函数即可
/*---------------------------------------------------------------------------
功能：    lcd 初始化
---------------------------------------------------------------------------*/
void __lcd_GC9307_init(void)
{
//    spi_init();
//    spi_flash_enable_voltage();
//    Djy_DelayUs(300);
//    bk_spi_master_init(30*1000*1000, SPI_DEF_MODE);
    GC9307_GpioInit();
    SPI_RST(1);
    DJY_DelayUs(100);
    SPI_RST(0);
    DJY_DelayUs(500);
    SPI_RST(1);
    DJY_DelayUs(300);

    WriteComm(0xfe);
    WriteComm(0xef);

    WriteComm(0x36);
    WriteData(0x48);
    WriteComm(0x3a);
    WriteData(0x05);



    WriteComm(0x85);
    WriteData(0x80);
    WriteComm(0x86);
    WriteData(0xf8);
    WriteComm(0x87);
    WriteData(0x79);
    WriteComm(0x89);
    WriteData(0x13);

    WriteComm(0x8b);
    WriteData(0x80);

    WriteComm(0x8d);
    WriteData(0x33);
    WriteComm(0x8e);
    WriteData(0x8f);
    WriteComm(0x8f);
    WriteData(0x73);

    //inversion
    WriteComm(0xe8);
    WriteData(0x12);
    WriteData(0x00);

    WriteComm(0xec);
    WriteData(0x13);
    WriteData(0x02);
    WriteData(0x88);

    WriteComm(0xbf);
    WriteData(0x1c);

    WriteComm(0xad);
    WriteData(0x4a);

    WriteComm(0xae);
    WriteData(0x44);

    WriteComm(0xac);
    WriteData(0x44);


    WriteComm(0xc3);
    WriteData(0x58);//58

    WriteComm(0xc4);
    WriteData(0x4E);//4E

    WriteComm(0xc9);
    WriteData(0x08);


    WriteComm(0xff);
    WriteData(0x62);


    WriteComm(0x99);
    WriteData(0x3e);
    WriteComm(0x9d);
    WriteData(0x4b);
    WriteComm(0x98);
    WriteData(0x3e);
    WriteComm(0x9c);
    WriteData(0x4b);


    WriteComm(0xf0);
    WriteData(0x45);
    WriteData(0x0A);
    WriteData(0x0A);
    WriteData(0x06);
    WriteData(0x05);
    WriteData(0x2E);

    WriteComm(0xf2);
    WriteData(0x45);
    WriteData(0x09);
    WriteData(0x0A);
    WriteData(0x0b);
    WriteData(0x05);
    WriteData(0x2E);

    WriteComm(0xf1);
    WriteData(0x45);
    WriteData(0x8F);
    WriteData(0x8f);
    WriteData(0x3B);
    WriteData(0x3F);
    WriteData(0x7f);

    WriteComm(0xf3);
    WriteData(0x45);
    WriteData(0x8f);
    WriteData(0x8f);
    WriteData(0x3B);
    WriteData(0x3F);
    WriteData(0x7f);


    WriteComm(0x11);
//    Delay(120);
    WriteComm(0x29);
    WriteComm(0x2c);

}

/*---------------------------------------------------------------------------
功能：  设置一个像素
---------------------------------------------------------------------------*/
void __GC9307_set_pixel(u32 x, u32 y, u16 color)
{
    unsigned int Xstart = x;
    unsigned int Xend = x;
    unsigned int Ystart = y;
    unsigned int Yend = y;
    unsigned int xx[2];

    WriteComm(0x2a);
    WriteData(Xstart>>8);
    WriteData(Xstart&0xff);
    WriteData(Xend>>8);
    WriteData(Xend&0xff);

    WriteComm(0x2b);
    WriteData(Ystart>>8);
    WriteData(Ystart&0xff);
    WriteData(Yend>>8);
    WriteData(Yend&0xff);

    WriteComm(0x2c);
//    color = (color>>8)|(color&0xFF00);
    xx[0] = (unsigned char)((color>>8) & 0xff);
    xx[1] = (unsigned char)(color & 0xff);

    WriteDataBytes(xx, 2);
}

void BlockWrite(unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend)
{
    WriteComm(0x2a);
    WriteData(Xstart>>8);
    WriteData(Xstart&0xff);
    WriteData(Xend>>8);
    WriteData(Xend&0xff);

    WriteComm(0x2b);
    WriteData(Ystart>>8);
    WriteData(Ystart&0xff);
    WriteData(Yend>>8);
    WriteData(Yend&0xff);

    WriteComm(0x2c);

}

/*---------------------------------------------------------------------------
功能：  读取一个像素
---------------------------------------------------------------------------*/
u16 __GC9307_get_pixel(u32 x, u32 y)
{
    printf("__GC9307_get_pixel\r\n");
    return 0;
}

/*---------------------------------------------------------------------------
功能：  设置光标位置
        row     :0--319
        column  :0--239
---------------------------------------------------------------------------*/
void __GC9307_set_cursor(u32 x, u32 y)
{
    unsigned int Xstart = x;
    unsigned int Xend = x;
    unsigned int Ystart = y;
    unsigned int Yend = y;
    WriteComm(0x2a);
    WriteData(Xstart>>8);
    WriteData(Xstart&0xff);
    WriteData(Xend>>8);
    WriteData(Xend&0xff);

    WriteComm(0x2b);
    WriteData(Ystart>>8);
    WriteData(Ystart&0xff);
    WriteData(Yend>>8);
    WriteData(Yend&0xff);

    WriteComm(0x2c);
}

/*---------------------------------------------------------------------------
功能：  开窗
---------------------------------------------------------------------------*/
void __GC9307_set_window(u32 x,u32 y, u32 wide,u32 high)
{
    unsigned int Xstart = x;
    unsigned int Xend = x+wide-1;
    unsigned int Ystart = y;
    unsigned int Yend = y+high-1;

    WriteComm(0x2a);
    WriteData(Xstart>>8);
    WriteData(Xstart&0xff);
    WriteData(Xend>>8);
    WriteData(Xend&0xff);

    WriteComm(0x2b);
    WriteData(Ystart>>8);
    WriteData(Ystart&0xff);
    WriteData(Yend>>8);
    WriteData(Yend&0xff);

    WriteComm(0x2c);
}

/*---------------------------------------------------------------------------
功能：  关窗
---------------------------------------------------------------------------*/
void __GC9307_close_window(void)
{
    __GC9307_set_cursor(0, 0);//光标设在左上角
}

void lcd_clear(u32 color)
{
    u32 i = 0, j = 0, n = 0;

    unsigned short* screen = (unsigned short*)malloc(ROW_TEMP*CFG_LCD_YSIZE*CFG_LCD_XSIZE*COLO_DEP); //RGB565--2, RGB666-3
    if (screen==0) return ;

    for(i=0;i<ROW_TEMP;i++)
    {
        for(j=0;j<CFG_LCD_XSIZE;j++)
        {
            screen[(i%ROW_TEMP)*CFG_LCD_XSIZE+j] = color&0xFFFF;
        }
    }
    BlockWrite(0,CFG_LCD_XSIZE-1,0,CFG_LCD_YSIZE-1);
    n = CFG_LCD_YSIZE / ROW_TEMP;
    for (i=0; i<n; i++) {
        WriteDataBytes(screen, sizeof(screen));
    }
    WriteDataBytes(screen, (CFG_LCD_YSIZE%ROW_TEMP)*CFG_LCD_XSIZE*COLO_DEP);
    if (screen) free(screen);
}

/*---------------------------------------------------------------------------
功能：    display on
---------------------------------------------------------------------------*/
void lcd_display_on(void)
{

}

/*---------------------------------------------------------------------------
功能：    display off
---------------------------------------------------------------------------*/
void lcd_display_off(void)
{

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
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
    printf("__lcd_set_pixel_bm\r\n");
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
//    printf("__lcd_line_bm\r\n");
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
//    printf("__lcd_line_bm_ie\r\n");
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
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    u32 y;
    u16 pixel;
    u16 *dst_offset;

//    printf("__lcd_fill_rect_bm\r\n");
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
//    printf("pixel = %4x\r\n",pixel);
    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                              + Focus->top * dst_bitmap->linebytes);
    dst_offset += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {

        memset16(dst_offset,pixel,(Focus->right-Focus->left));
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
    struct RopGroup Rop0 = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    struct RopGroup Rop1 = { 0, 0, 0, CN_R2_COPYPEN, 0, 1, 0  };
    u32 x,y;
    bool_t RopSupport;

    RopSupport = (memcmp(&RopCode,&Rop0,sizeof(struct RopGroup)) == 0)
                    || (memcmp(&RopCode,&Rop1,sizeof(struct RopGroup)) == 0);
//    printf("__lcd_blt_bm_to_bm\r\n");
    if((dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
             ||! RopSupport)
     {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //源位图缓冲区非对齐模型
        {
            return false;
        }else
        {
            dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                                      + DstRect->top * dst_bitmap->linebytes);
            dst_offset += DstRect->left;

            if(RopCode.HyalineEn)
            {
                HyalineColor = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,HyalineColor);
                if(src_bitmap->reversal == true)
                {
                    src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                        +(src_bitmap->height - SrcRect->top-1) * src_bitmap->linebytes);
                    src_offset += SrcRect->left;
                    for(y = DstRect->top; y < DstRect->bottom; y++)
                    {
                        for(x = 0; x < (DstRect->right-DstRect->left); x++)
                        {
                            if(src_offset[x] != HyalineColor)
                                dst_offset[x] = src_offset[x];
                        }
                        dst_offset += dst_bitmap->linebytes >> 1;
                        src_offset -= src_bitmap->linebytes >> 1;
                    }
                }
                else
                {
                    src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                                              +SrcRect->top * src_bitmap->linebytes);
                    src_offset += SrcRect->left;
                    for(y = DstRect->top; y < DstRect->bottom; y++)
                    {
                        for(x = 0; x < (DstRect->right-DstRect->left); x++)
                        {
                            if(src_offset[x] != HyalineColor)
                                dst_offset[x] = src_offset[x];
                        }
                        dst_offset += dst_bitmap->linebytes >> 1;
                        src_offset += src_bitmap->linebytes >> 1;
                    }
                }
            }
            else
            {
                if(src_bitmap->reversal == true)
                {
                    src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                        +(src_bitmap->height - SrcRect->top-1) * src_bitmap->linebytes);
                    src_offset += SrcRect->left;
                    for(y = DstRect->top; y < DstRect->bottom; y++)
                    {
                        memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<1);
                        dst_offset += dst_bitmap->linebytes >> 1;
                        src_offset -= src_bitmap->linebytes >> 1;
                    }
                }
                else
                {
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
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __GC9307_set_pixel(x,y,pixel);
    }else
    {
        printf("__GC9307_get_pixel\r\n");
        dest = __GC9307_get_pixel(x,y);
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __GC9307_set_pixel(x,y,pixel);
    }
    return true;
}

//在screen中画一条任意直线，不含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    printf("__lcd_line_screen\r\n");
    return false;
}

//在screen中画一条任意直线，含端点，如硬件加速不支持在screen上画线，或者有
//frame_buffer，driver可以简化，直接返回false即可
//镜像显示器的driver须提供这个函数
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
//    printf("__lcd_line_screen_ie\r\n");
    return false;
}

//screen中矩形填充，如硬件加速不支持在screen上矩形填充，或者有frame_buffer，
//driver可以简化，直接返回false即可
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{

    u32 width,height;
    u16 pixel;
//    unsigned int i,j;
//    u32 x,y;

    if(Mode != CN_FILLRECT_MODE_N)
    {
        printf("Mode = %d!\r\n",Mode);
        return false;
    }
    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
    width = Focus->right-Focus->left;
    height = Focus->bottom-Focus->top;
    __GC9307_set_window(Focus->left,Focus->top,width,height);
    printf("Focus->right = %d,Focus->left = %d\r\n",Focus->right,Focus->left);
    printf("Focus->bottom = %d,Focus->top = %d\r\n",Focus->bottom,Focus->top);
}

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 width,height;
    u32 pixel,use=0,linelen;
    u32 j,y;
    u16 *line;
    u8 x0,x1;

    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    __GC9307_set_window(xsrc,ysrc,width,height);

    linelen = src_bitmap->linebytes;
    line = (u16*)(src_bitmap->bm_bits + ysrc*linelen + xsrc*2);
    for(y = 0;y < height;y++)
    {
        WriteDataBytes(line,width*2);
        line += linelen/2;
    }
    return true;
}

//从screen中取一像素，并转换成cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    printf("__lcd_get_pixel_screen\r\n");
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                        __GC9307_get_pixel(x,y),0);
}

//把screen内矩形区域的内容复制到bitmap，调用前，先设置好dest的pf_type
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    printf("__lcd_get_rect_screen\r\n");
    return false;
}

//----控制显示器---------------------------------------------------------------
//功能: 这是由driver提供商编写的，用于设置显示器各种参数的函数，属于应用程序的
//      范畴，功能由设备厂商定义。在ut2416中，本函数为空。
//参数: disp，显示器指针
//返回: true=成功，false=失败
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct DisplayObj *disp)
{
    printf("__lcd_disp_ctrl\r\n");
    return true;
}

void DispMainInterface(unsigned char *pic)
{
//    struct Rectangle dst_rect = {0,0,CFG_LCD_XSIZE,CFG_LCD_YSIZE};
//    struct RectBitmap src_bitmap;
//
//    src_bitmap.bm_bits = pic;
//    src_bitmap.width = CFG_LCD_XSIZE;
//    src_bitmap.height = CFG_LCD_YSIZE;
//    src_bitmap.linebytes = CFG_LCD_XSIZE*2;
//    src_bitmap.reversal = 0;
//    src_bitmap.PixelFormat = CN_SYS_PF_RGB565;
//    __lcd_bm_to_screen(&dst_rect,  &src_bitmap, 0, 0);

    struct RopGroup RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
       struct GkWinObj *desktop;
       struct RectBitmap   bitmap;
       desktop = GK_GetDesktop(CFG_DISPLAY_NAME);
       bitmap.bm_bits = pic+70;
       bitmap.linebytes = CFG_LCD_XSIZE*2;
       bitmap.PixelFormat = CN_SYS_PF_RGB565;
       bitmap.ExColor = 0;
       bitmap.height=CFG_LCD_YSIZE;
       bitmap.width=CFG_LCD_XSIZE;
       bitmap.reversal = true;
       GK_DrawBitMap(desktop,&bitmap,0,0,0,RopCode,100000);
       GK_SyncShow(100000);
}

//----初始化lcd设备------------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_GC9307(const char *DisplayName,const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;
    //u8 *pLTDCBufferFG1 =NULL;//缓冲区起始位置
    static u8 u8g_frame_buffer[cn_frame_buffer_size] __attribute__((section(".lcdram")));


    //struct HeapCB *heap;

    __lcd_GC9307_init( );

#if 0
    heap =Heap_FindHeap(HeapName);
    if(heap==NULL){
        printf("M_FindHeapd  ERROR!\r\n");
        return NULL;
    }

    pLTDCBufferFG1 = M_MallocHeap(cn_frame_buffer_size,heap,0);

    printf("M_FindHeapd  pLTDCBufferFG1!\r\n");
//    pLTDCBufferFG2 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize,heap,0);//先用一层
//    if(0x3f&(u32)pLTDCBufferFG1)
//        pLTDCBufferFG1+=(0x40-(0x3f&(u32)pLTDCBufferFG1));
    FrameBitmap.bm_bits = (u8 *)pLTDCBufferFG1;
#else
    FrameBitmap.bm_bits = u8g_frame_buffer;
//  heap =Heap_FindHeap("PSRAM");
//  FrameBitmap.bm_bits = M_MallocHeap(cn_frame_buffer_size,heap,0);;
#endif
    FrameBitmap.width = CFG_LCD_XSIZE;
    FrameBitmap.height = CFG_LCD_YSIZE;
    FrameBitmap.PixelFormat = CN_LCD_PIXEL_FORMAT;
    FrameBitmap.linebytes = cn_lcd_line_size;
    FrameBitmap.ExColor = 0;
    frame_win.wm_bitmap = &FrameBitmap;
    tg_lcd_display.frame_buffer = &frame_win;

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

//    tg_lcd_display.bmmalloc = lcd_bmmalloc;

//  tg_lcd_display.DisplayHeap = heap;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,DisplayName);
    return (ptu32_t)&tg_lcd_display;
}

