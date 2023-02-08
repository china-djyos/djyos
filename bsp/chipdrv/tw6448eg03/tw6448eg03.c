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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//����ģ��:ͼ��
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ili9325��������
//����˵��:
//�޶���ʷ:
//1. ����: 2009-03-10
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
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
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern void ModuleInstall_GC9307(void);
//  ModuleInstall_GC9307();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"LCD driver TW6448"//LCD��ʾ����
//parent:"graphical kernel"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","heap","cpu onchip spi"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"          //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#if ( CFG_MODULE_ENABLE_LCD_DRIVER_TW6448 == false )
//#warning  " keyboard_hard_driver  �������δ���ã�ʹ��Ĭ������"
#define CFG_MODULE_ENABLE_LCD_DRIVER_TW6448    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   64             //"LCD���-������",
#define CFG_LCD_YSIZE   48             //"LCD�߶�-������",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   36720            //"LCD���-΢����",
#define CFG_LCD_YSIZE_UM   48960            //"LCD�߶�-΢����",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_TW6448_DISPLAY_NAME        "lcdTW6448"    //"��ʾ������",����Һ����ʾ������
#define CFG_TW6448_HEAP_NAME           "extram"        //"����ʹ�ö���",����Һ��������ʹ�õĶ�����
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
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

//������djyguiҪ��Ľӿں������������£�������ÿպ�������
/*---------------------------------------------------------------------------
���ܣ�    lcd ��ʼ��
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

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
void ModuleInstall_GC9307(void)
{
    __lcd_TW6448s_init( );
}

