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
//#include "TW6448s.h"
#include "djyos.h"

#include "cpu_peri.h"
#include "systime.h"
#include "spi_pub.h"

#include <ascii8x16song.h>

#if 1
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void ModuleInstall_GC9307(void);
//  ModuleInstall_GC9307();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"LCD driver TW6448"//LCD显示驱动
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
#if ( CFG_MODULE_ENABLE_LCD_DRIVER_TW6448 == false )
//#warning  " keyboard_hard_driver  组件参数未配置，使用默认配置"
#define CFG_MODULE_ENABLE_LCD_DRIVER_TW6448    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   64             //"LCD宽度-像素数",
#define CFG_LCD_YSIZE   48             //"LCD高度-像素数",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   36720            //"LCD宽度-微米数",
#define CFG_LCD_YSIZE_UM   48960            //"LCD高度-微米数",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_TW6448_DISPLAY_NAME        "lcdTW6448"    //"显示器名称",配置液晶显示的名称
#define CFG_TW6448_HEAP_NAME           "extram"        //"驱动使用堆名",配置液晶驱动所使用的堆名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#endif

//#define X_WIDTH     64
//#define Y_WIDTH     48
#define SIZE 16
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  CFG_LCD_XSIZE  //X_WIDTH
#define Max_Row     CFG_LCD_YSIZE  //Y_WIDTH
#define Brightness  0xFF
#define page_size CFG_LCD_YSIZE/8  //Y_WIDTH/8

#define COL_OFFSET 32  //column offset setting


#define SPI_RST(x)   djy_gpio_write(GPIO19,(x))
#define SPI_RS(x)    djy_gpio_write(GPIO15,(x))
#define SPI_CS(x)    djy_gpio_write(GPIO3,(x))
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
   SPI_CS(0);
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
   SPI_CS(1);
   return 0;
}

void WriteDataBytes(unsigned char *data, int len)
{
    u32 param;
    SPI_CS(0);
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

    SPI_CS(1);
    //return 0;
}


void WriteComm(unsigned char i)
{
    SPI_CS(0);
    SPI_RS(0);
    SendDataSPI(i);
    SPI_CS(1);
}
void WriteData(unsigned char i)
{
    SPI_CS(0);
    SPI_RS(1);
    SendDataSPI(i);
    SPI_CS(1);
}
void WriteDispData(unsigned char DataH,unsigned char DataL)
{
     SPI_CS(0);
     SPI_RS(1);
     SendDataSPI(DataH);
     SendDataSPI(DataL);
     SPI_CS(1);

}
void WriteOneDot(unsigned long int color)
{
    SPI_CS(0);
    SPI_RS(1);
    SendDataSPI(color>>16);
    SendDataSPI(color>>8);
    SendDataSPI(color);
    SPI_CS(1);
}

void __TW6448s_write_cmd(unsigned char cmd)
{
    SPI_CS(0);
    SPI_RS(0);
    SendDataSPI(cmd);
    SPI_CS(1);
}
void __TW6448s_write_data(unsigned char data)
{
    SPI_CS(0);
    SPI_RS(1);
    SendDataSPI(data);
    SPI_CS(1);
}


void spi_init_extral_gpio(void)
{
    bk_gpio_config_output(GPIO15);
    bk_gpio_output(GPIO15, GPIO_INT_LEVEL_HIGH);

    bk_gpio_config_output(GPIO19);
    bk_gpio_output(GPIO19, GPIO_INT_LEVEL_HIGH);

    bk_gpio_config_output(GPIO3);
    bk_gpio_output(GPIO3, GPIO_INT_LEVEL_HIGH);

//    bk_gpio_config_output(GPIO36);
//    bk_gpio_output(GPIO36, GPIO_INT_LEVEL_LOW);
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


void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        WriteComm (0xb0+i);
        WriteComm (0x00);
        WriteComm (0x10);
        for(n=0;n<128;n++)WriteData(0);
    }
}


void OLED_On(void)
{
    u8 i,n;
    for(i=0;i<page_size;i++)
    {
        WriteComm (0xb0+i);
        WriteComm(((COL_OFFSET&0xf0)>>4)|0x10);
        WriteComm((COL_OFFSET&0x0f));
        for(n=0;n<Max_Column/2;n++)
        {
            if(n%2==0)
            {
                WriteData(0xaa);
                WriteData(0x88);
            }
            else
            {
                WriteData(0x00);
                WriteData(0x00);
            }
        }
    }
}

void OLED_Set_Pos(unsigned char x, unsigned char y)
{
    WriteComm(0xb0+y);
    WriteComm((((x+COL_OFFSET)&0xf0)>>4)|0x10);
    WriteComm(((x+COL_OFFSET)&0x0f));
}

void OLED_ShowChar(u8 x,u8 y,u8 chr)
{
    unsigned char c=0,i=0;
    c=chr-' ';
    if(x>Max_Column-1)
    {
        x=0;
        y=y+2;
    }
    if(SIZE ==16)
    {
        OLED_Set_Pos(x,y);
        for(i=0;i<8;i++)
            WriteData(ascii_8x16[c*16+i]);
        OLED_Set_Pos(x,y+1);
        for(i=0;i<8;i++)
            WriteData(ascii_8x16[c*16+i+8]);
    }
}

void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {
        OLED_ShowChar(x,y,chr[j]);
        x+=8;
        if(x>Max_Column-8)
        {
            x=0;
            y+=2;
        }
        j++;
    }
}

void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
    unsigned int j=0;
    unsigned char x,y;

//    if(y1%8==0)
//        y=y1/8;
//    else
//        y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
        for(x=x0;x<x1;x++)
        {
            WriteData(BMP[j++]);
        }
    }
}

//以下是djygui要求的接口函数，许多情况下，大多数用空函数即可
/*---------------------------------------------------------------------------
功能：    lcd 初始化
---------------------------------------------------------------------------*/
void __lcd_TW6448s_init(void)
{
    spi_init();
    spi_flash_enable_voltage();
    spi_init_extral_gpio();
    Djy_DelayUs(300);
    bk_spi_master_init(30*1000*1000, SPI_DEF_MODE);
    spi_init_extral_gpio();
    SPI_RST(1);
    Djy_DelayUs(1000);
    SPI_RST(0);
    Djy_DelayUs(1000);
    SPI_RST(1);
    Djy_DelayUs(1000);

    WriteComm(0xae);//Display OFF
    WriteComm(0x40);//Set Display Start Line
    WriteComm(0x81);//Set Contrast Control
    WriteComm(0x7d);
    WriteComm(0xa0);//Set Segment Re-map
    WriteComm(0xa4);// Resume to RAM content display
    WriteComm(0xa6);// Set Normal Display
    WriteComm(0xa8);//Set Multiplex Ratio
    WriteComm(0x2f);
    WriteComm(0xc0);//Set COM Output Scan Direction
    WriteComm(0xd3);//Set Display Offset
    WriteComm(0x00);
    WriteComm(0xd5);//Set Display Clock Divide Ratio/Oscillator Frequency
    WriteComm(0x10);
    WriteComm(0xd9);//Set Pre-charge Period
    WriteComm(0x22);
    WriteComm(0xda);//Set COM Pins Hardware Configuration
    WriteComm(0x12);
    WriteComm(0xdb);//Set VCOMH Deselect Level
    WriteComm(0x00);
    WriteComm(0x8d);//Charge Pump Setting
    WriteComm(0x14);//Enable Charge Pump
    OLED_Clear();
    WriteComm(0xAf); /*display ON*/

}

//----初始化lcd设备------------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 显示器资源指针
//-----------------------------------------------------------------------------
void ModuleInstall_GC9307(void)
{
    __lcd_TW6448s_init( );
}

