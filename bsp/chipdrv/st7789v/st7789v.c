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
#include "ST7789V.h"
#include "djyos.h"
#include <dbug.h>
#include "cpu_peri.h"
#include "systime.h"
#include "spi_pub.h"

#if 1
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern ptu32_t ModuleInstall_ST7789V(const char *DisplayName,const char* HeapName);
//  ModuleInstall_ST7789V(CFG_ST7789V_DISPLAY_NAME,CFG_ST7789V_HEAP_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"LCD driver ST7789V"//LCD��ʾ����
//parent:"graphical kernel"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main            //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
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
#if ( CFG_MODULE_ENABLE_LCD_DRIVER_ST7789V == false )
//#warning  " keyboard_hard_driver  �������δ���ã�ʹ��Ĭ������"
#define CFG_MODULE_ENABLE_LCD_DRIVER_ST7789V    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,24,32,
#define  CFG_LCD_SIZE  32     //"LCD�ߴ�",
//%$#@enum,true,false,
#define  CFG_LCD_TURN  false            //"�����귴ת",�������Ƿ���Ҫ��ת
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   320             //"LCD���-������",
#define CFG_LCD_YSIZE   240             //"LCD�߶�-������",
//%$#@num,,,
#define CFG_LCD_XSIZE_UM   64800            //"LCD���-΢����",
#define CFG_LCD_YSIZE_UM   48600            //"LCD�߶�-΢����",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_ST7789V_DISPLAY_NAME        "lcdst7789v"    //"��ʾ������",����Һ����ʾ������
#define CFG_ST7789V_HEAP_NAME           "extram"        //"����ʹ�ö���",����Һ��������ʹ�õĶ�����
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define cn_coordinates      0      //0=�������꣬1=��ת���꣬��ԭ��=(239,319)

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
//��������spi��λ����board�ж���
//������
//����ֵ��
//���ܣ�
//=====================================================================
__attribute__((weak)) void SPI_RST(u32 value)
{
    error_printf("ST7789V","board undefined SPI_RST \r\n");
}
//=====================================================================
//�����������ڿ��Ʒ��͸����������ݻ��������board�ж���
//������value��1=��д���ݶ˿ڣ�0=��д����˿�
//����ֵ��
//���ܣ�
//=====================================================================
__attribute__((weak)) void SPI_RS(u32 value)
{
    error_printf("ST7789V","board undefined SPI_RS \r\n");
}

//=====================================================================
//������������SPIƬѡ����board�ж���
//������
//����ֵ��
//���ܣ�
//=====================================================================
__attribute__((weak)) void ST7789V_CsActive(void)
{
    error_printf("ST7789V","board undefined ST7789V_CsActive \r\n");
}
//=====================================================================
//������������ȡ��SPIƬѡ����board�ж���
//������
//����ֵ��
//���ܣ�
//=====================================================================
__attribute__((weak)) void ST7789V_CsInactive(void)
{
    error_printf("ST7789V","board undefined ST7789V_CsInactive \r\n");
}

//=====================================================================
//����������board�ﶨ�壬��ʼ��RST��RS��Ƭѡ3������
//������
//����ֵ��
//���ܣ�
//=====================================================================
__attribute__((weak)) void ST7789V_GpioInit(void)
{
    error_printf("ST7789V","board undefined ST7789V_GpioInit \r\n");
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
   struct spi_message spi_msg;
   ST7789V_CsActive();
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
   bk_spi_master_xfer(&spi_msg);
   ST7789V_CsInactive();
   return 0;
}

void WriteDataBytes(unsigned char *data, int len)
{
    u32 param;
    ST7789V_CsActive();
    SPI_RS(1);
    struct spi_message spi_msg;
    spi_msg.recv_buf = 0;
    spi_msg.recv_len = 0;
    spi_msg.send_buf = data;
    spi_msg.send_len = len;

    param = 1;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, &param);
    bk_spi_master_xfer(&spi_msg);
    param = 0;
    spi_ctrl(CMD_SPI_SET_BITWIDTH, &param);

    ST7789V_CsInactive();
    //return 0;
}


void WriteComm(unsigned char i)
{
    ST7789V_CsActive();
    SPI_RS(0);
    SendDataSPI(i);
    ST7789V_CsInactive();
}
void WriteData(unsigned char i)
{
    ST7789V_CsActive();
    SPI_RS(1);
    SendDataSPI(i);
    ST7789V_CsInactive();
}
void WriteDispData(unsigned char DataH,unsigned char DataL)
{
     ST7789V_CsActive();
     SPI_RS(1);
     SendDataSPI(DataH);
     SendDataSPI(DataL);
     ST7789V_CsInactive();

}
void WriteOneDot(unsigned long int color)
{
    ST7789V_CsActive();
    SPI_RS(1);
    SendDataSPI(color>>16);
    SendDataSPI(color>>8);
    SendDataSPI(color);
    ST7789V_CsInactive();
}

void __ST7789V_write_cmd(unsigned char cmd)
{
    ST7789V_CsActive();
    SPI_RS(0);
    SendDataSPI(cmd);
    ST7789V_CsInactive();
}
void __ST7789V_write_data(unsigned char data)
{
    ST7789V_CsActive();
    SPI_RS(1);
    SendDataSPI(data);
    ST7789V_CsInactive();
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

static int __ST7789V_read_reg()
{
    return 0x7796;
}


//������djyguiҪ��Ľӿں������������£�������ÿպ�������
/*---------------------------------------------------------------------------
���ܣ�    lcd ��ʼ��
---------------------------------------------------------------------------*/
void __lcd_ST7789V_init(void)
{
//    spi_init();
//    spi_flash_enable_voltage();
//    Djy_DelayUs(300);
//    bk_spi_master_init(30*1000*1000, SPI_DEF_MODE);
    ST7789V_GpioInit();
    SPI_RST(1);
    DJY_DelayUs(100);
    SPI_RST(0);
    DJY_DelayUs(500);
    SPI_RST(1);
    DJY_DelayUs(300);

    if( __ST7789V_read_reg() != 0x7796)
    {
        return;
    }
#if (CFG_LCD_SIZE == 32)
    //*************  **********//
    WriteComm(0x11);

    WriteComm(0x36);

#if (CFG_LCD_XSIZE < CFG_LCD_YSIZE)
    #if  (CFG_LCD_TURN == false)
        WriteData(0x00);
    #else
        WriteData(0xc0);
    #endif
#else
    #if  (CFG_LCD_TURN == false)
        WriteData(0xa0);
    #else
        WriteData(0x60);
    #endif
#endif

    WriteComm(0x3A);
    WriteData(0x55);


    WriteComm(0xb2);
    WriteData(0x0C);
    WriteData(0x0C);
    WriteData(0x00);
    WriteData(0x33);
    WriteData(0x33);

    WriteComm(0xb7);
    WriteData(0x35);

    WriteComm(0xbb);
    WriteData(0x28);

    WriteComm(0xc0);
    WriteData(0x2c);

    WriteComm(0xc2);
    WriteData(0x01);

    WriteComm(0xc3);
    WriteData(0x10);

    WriteComm(0xc4);
    WriteData(0x20);

    WriteComm(0xc6);
    WriteData(0x0F);

    WriteComm(0xd0);
    WriteData(0xa4);
    WriteData(0xa1);

    WriteComm(0xe0);
    WriteData(0xd0);
    WriteData(0x00);
    WriteData(0x02);
    WriteData(0x07);
    WriteData(0x0A);
    WriteData(0x28);
    WriteData(0x32);
    WriteData(0x44);
    WriteData(0x42);
    WriteData(0x06);
    WriteData(0x0E);
    WriteData(0x12);
    WriteData(0x14);
    WriteData(0x17);

    WriteComm(0xe1);
    WriteData(0xd0);
    WriteData(0x00);
    WriteData(0x02);
    WriteData(0x07);
    WriteData(0x0A);
    WriteData(0x28);
    WriteData(0x31);
    WriteData(0x54);
    WriteData(0x47);
    WriteData(0x0E);
    WriteData(0x1C);
    WriteData(0x17);
    WriteData(0x1B);
    WriteData(0x1E);

    WriteComm(0x29);
    WriteComm(0x2C);
#elif (CFG_LCD_SIZE == 24)
    WriteComm(0x11);   //sleep out
//    delay_ms(200);

    WriteComm(0x36);   //memory access control

#if (CFG_LCD_XSIZE < CFG_LCD_YSIZE)
    #if  (CFG_LCD_TURN == false)
        WriteData(0x00);
    #else
        WriteData(0xc0);   //MY MX MV ML MH=0,BGR=1
    #endif
#else
    #if  (CFG_LCD_TURN == false)
        WriteData(0xa0);
    #else
        WriteData(0x60);
    #endif
#endif

    WriteComm(0x3A);
    WriteData(0x05);

    WriteComm(0x21);   //IPS

//    WriteComm(0xB0);  //RAM CONTROL
//    WriteData(0x11);
//    WriteData(0xF0);

//    WriteComm(0xB1);  //RGB CONTROL
//    WriteData(0x40);
//    WriteData(0x02);
//    WriteData(0x14);

    WriteComm(0xB2);
    WriteData(0x05);
    WriteData(0x05);
    WriteData(0x00);
    WriteData(0x33);
    WriteData(0x33);

    WriteComm(0xB7);
    WriteData(0x35);

    WriteComm(0xB8);
    WriteData(0x2F);
    WriteData(0x2B);
    WriteData(0x2F);

    WriteComm(0xBb);
    WriteData(0x20);

    WriteComm(0xc0);
    WriteData(0x2c);

    WriteComm(0xc2);
    WriteData(0x01);

    WriteComm(0xc3);
    WriteData(0x0B);

    WriteComm(0xc4);
    WriteData(0x20);

    WriteComm(0xc6);
    WriteData(0x11);

    WriteComm(0xd0);
    WriteData(0xa4);
    WriteData(0xa1);

    WriteComm(0xE8);
    WriteData(0x03);

    WriteComm(0xE9);
    WriteData(0x0D);
    WriteData(0x12);
    WriteData(0x00);

    WriteComm(0xE0);   //positive gamma correction
    WriteData(0xd0);
    WriteData(0x06);
    WriteData(0x0B);
    WriteData(0x0A);
    WriteData(0x09);
    WriteData(0x05);
    WriteData(0x2E);
    WriteData(0x43);
    WriteData(0x44);
    WriteData(0x09);
    WriteData(0x16);
    WriteData(0x15);
    WriteData(0x23);
    WriteData(0x27);

    WriteComm(0xE1);   //negative gamma correction
    WriteData(0xd0);
    WriteData(0x06);
    WriteData(0x0B);
    WriteData(0x09);
    WriteData(0x08);
    WriteData(0x06);
    WriteData(0x2E);
    WriteData(0x44);
    WriteData(0x44);
    WriteData(0x3A);
    WriteData(0x15);
    WriteData(0x15);
    WriteData(0x23);
    WriteData(0x26);

    WriteComm(0x29);   //display on

    WriteComm(0x2A);   //column address set
    WriteData(0x00);
    WriteData(0x00);
    WriteData(0x00);
    WriteData(0xEF);

    WriteComm(0x2B);   //page address set
    WriteData(0x00);
    WriteData(0x00);
    WriteData(0x01);
    WriteData(0x3F);

    //WriteComm(0x2C);     //memory write



//    delay_ms(50);
    WriteComm(0x11);       //Exit Sleep
//    delay_ms(120);
    WriteComm(0x29);       //Displa
#endif
}

/*---------------------------------------------------------------------------
���ܣ�  ����һ������
---------------------------------------------------------------------------*/
void __ST7789V_set_pixel(u32 x, u32 y, u16 color)
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
���ܣ�  ��ȡһ������
---------------------------------------------------------------------------*/
u16 __ST7789V_get_pixel(u32 x, u32 y)
{
    printf("__ST7789V_get_pixel\r\n");
    return 0;
}

/*---------------------------------------------------------------------------
���ܣ�  ���ù��λ��
        row     :0--319
        column  :0--239
---------------------------------------------------------------------------*/
void __ST7789V_set_cursor(u32 x, u32 y)
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
���ܣ�  ����
---------------------------------------------------------------------------*/
void __ST7789V_set_window(u32 x,u32 y, u32 wide,u32 high)
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
���ܣ�  �ش�
---------------------------------------------------------------------------*/
void __ST7789V_close_window(void)
{
    __ST7789V_set_cursor(0, 0);//����������Ͻ�
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
        WriteDataBytes((u8*)screen, sizeof(screen));
    }
    WriteDataBytes((u8*)screen, (CFG_LCD_YSIZE%ROW_TEMP)*CFG_LCD_XSIZE*COLO_DEP);
    if (screen) free(screen);
}

/*---------------------------------------------------------------------------
���ܣ�    display on
---------------------------------------------------------------------------*/
void lcd_display_on(void)
{

}

/*---------------------------------------------------------------------------
���ܣ�    display off
---------------------------------------------------------------------------*/
void lcd_display_off(void)
{

}

//----λͼ�л�����-------------------------------------------------------------
//����: ��λͼ�л�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ���ơ�
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ��ء�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x��y������
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: ��
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
    printf("__lcd_set_pixel_bm\r\n");
    return false;
}

//----λͼ�л���---------------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�������(x2,y2)�㣬ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
//    printf("__lcd_line_bm\r\n");
    return false;
}

//----λͼ�л���(���˵�)-------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
//    printf("__lcd_line_bm_ie\r\n");
    return false;
}

//----λͼ��������----------------------------------------------------------------
//Bitmap�о������,֧�ֽ���ɫ
//ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
//������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
//����:
// DstBitmap: ���Ƶ�Ŀ��λͼ
// Target: Ŀ���������,����ɫ���ʱ������������������ɫ�������յ�
// Focus: ʵ���������,gkernelȷ������Target�����ڲ�
// Color0,Color1: Target���ϽǺ����½���ɫֵ,������ǽ������,Color1��������,
//              ���ظ�ʽ��CN_SYS_PF_ERGB8888
//Mode: ���ģʽ,CN_FILLRECT_MODE_N�峣��
//����: true=�ɹ����ƣ�false=ʧ��
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

//��������������ͨ��������£�����ͼ�λ��ơ�
//�������λͼ���������Դ��Ŀ��λͼ��ʽ����cn_sys_pf_r5g6b5��
bool_t __lcd_blt_bm_to_bm( struct RectBitmap *dst_bitmap,
                            struct Rectangle *DstRect,
                            struct RectBitmap *src_bitmap,
                            struct Rectangle *SrcRect,
                            struct RopGroup RopCode, u32 HyalineColor)
{
    u16 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
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
        if((ptu32_t)src_bitmap->bm_bits & 1)    //Դλͼ�������Ƕ���ģ��
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
                if(src_bitmap->reversal == 1)
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
                else if(src_bitmap->reversal == 2)  //�����ض��û�180����תͼ�εĲ���
                {
                    src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                                              +SrcRect->top * src_bitmap->linebytes);
                    src_offset += SrcRect->left;
                    for(y = DstRect->top; y < DstRect->bottom; y++)
                    {
                        u32 n,m;
//                      memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<1);
                        m = DstRect->right-DstRect->left - 1;
                        for(n = 0; n <= m; n++)
                        {
                            dst_offset[n] = src_offset[m-n];
                        }
                        dst_offset += dst_bitmap->linebytes >> 1;
                        src_offset += src_bitmap->linebytes >> 1;
                    }
                }
                else if(src_bitmap->reversal == 3)  //�����ض��û�180����תͼ�εĲ���
                {
                    src_offset = (u16*)((ptu32_t)src_bitmap->bm_bits
                        +(src_bitmap->height - SrcRect->top-1) * src_bitmap->linebytes);
                    src_offset += SrcRect->left;
                    for(y = DstRect->top; y < DstRect->bottom; y++)
                    {
                        u32 n,m;
//                      memcpy(dst_offset,src_offset,(DstRect->right-DstRect->left)<<1);
                        m = DstRect->right-DstRect->left - 1;
                        for(n = 0; n <= m; n++)
                        {
                            dst_offset[n] = src_offset[m-n];
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

//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ������.
//����: x��y����������
//      color����ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=û�л��ƣ����������
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    u16 dest,pixel;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    if(CN_R2_COPYPEN == r2_code)
    {
        __ST7789V_set_pixel(x,y,pixel);
    }else
    {
        printf("__ST7789V_get_pixel\r\n");
        dest = __ST7789V_get_pixel(x,y);
        pixel = GK_BlendRop2(dest, pixel, r2_code);
        __ST7789V_set_pixel(x,y,pixel);
    }
    return true;
}

//��screen�л�һ������ֱ�ߣ������˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    printf("__lcd_line_screen\r\n");
    return false;
}

//��screen�л�һ������ֱ�ߣ����˵㣬��Ӳ�����ٲ�֧����screen�ϻ��ߣ�������
//frame_buffer��driver���Լ򻯣�ֱ�ӷ���false����
//������ʾ����driver���ṩ�������
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                       s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
//    printf("__lcd_line_screen_ie\r\n");
    return false;
}

//screen�о�����䣬��Ӳ�����ٲ�֧����screen�Ͼ�����䣬������frame_buffer��
//driver���Լ򻯣�ֱ�ӷ���false����
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
    __ST7789V_set_window(Focus->left,Focus->top,width,height);
    printf("Focus->right = %d,Focus->left = %d\r\n",Focus->right,Focus->left);
    printf("Focus->bottom = %d,Focus->top = %d\r\n",Focus->bottom,Focus->top);
    return true;
}

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 width,height;
    u32 linelen;
    u32 y;
    u16 *line;

    if(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;
    width = dst_rect->right-dst_rect->left;
    height = dst_rect->bottom-dst_rect->top;
    __ST7789V_set_window(xsrc,ysrc,width,height);

    linelen = src_bitmap->linebytes;
    line = (u16*)(src_bitmap->bm_bits + ysrc*linelen + xsrc*2);
    for(y = 0;y < height;y++)
    {
        WriteDataBytes(line,width*2);
        line += linelen/2;
    }
    return true;
}

//��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    printf("__lcd_get_pixel_screen\r\n");
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                        __ST7789V_get_pixel(x,y),0);
}

//��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    printf("__lcd_get_rect_screen\r\n");
    return false;
}

//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣��ut2416�У�������Ϊ�ա�
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct DisplayObj *disp)
{
    printf("__lcd_disp_ctrl\r\n");
    return true;
}
#pragma GCC diagnostic pop

//void DispMainInterface(unsigned char *pic)
//{
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
//}
//
//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_ST7789V(const char *DisplayName,const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;
    //u8 *pLTDCBufferFG1 =NULL;//��������ʼλ��
    static u8 u8g_frame_buffer[cn_frame_buffer_size] __attribute__((section(".lcdram")));


    //struct HeapCB *heap;

    __lcd_ST7789V_init( );

#if 0
    heap =Heap_FindHeap(HeapName);
    if(heap==NULL){
        printf("M_FindHeapd  ERROR!\r\n");
        return NULL;
    }

    pLTDCBufferFG1 = M_MallocHeap(cn_frame_buffer_size,heap,0);

    printf("M_FindHeapd  pLTDCBufferFG1!\r\n");
//    pLTDCBufferFG2 =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*lcd.pixsize,heap,0);//����һ��
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
    //�����ʼ��frame_buffer��desktop��z_topmost������Ա

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

