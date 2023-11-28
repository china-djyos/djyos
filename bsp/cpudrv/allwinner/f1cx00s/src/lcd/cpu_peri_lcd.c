#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "gkernel.h"
#include <gui/gk_display.h>
#include "djyos.h"
#include "systime.h"
#include "cpu_peri_io.h"
#include "cpu_peri_gpio.h"
#include "cpu_peri_lcd.h"
#include "cpu_peri_debe.h"
#include "cpu_peri_tcon.h"
#include <stdlib.h>
//-----------------------
#include "sys_lcd_conf.h"
//-----------------------
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
#if (defined LCD_TYPE_TV_PAL_720_576)||(defined LCD_TYPE_TV_NTSC_720_480)
    #include "cpu_peri_tve.h"
#endif

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern ptu32_t ModuleInstall_F1CLCD(const char *DisplayName,const char* HeapName);
//  ModuleInstall_F1CLCD(CFG_ONCHIP_DISPLAY_NAME,NULL);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"F1C-LCD"      //LCD��ʾ����
//parent:"graphical kernel"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:pre-main            //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","heap" //������������������������none����ʾ�������������
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
//%$#@enum,true,false,
//%$#@num,,,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_ONCHIP_DISPLAY_NAME        "F1C-LCD"    //"��ʾ������",����Һ����ʾ������
//%$#@select,1
//#define       LCD_TYPE_RGB43_480_272
#define       LCD_TYPE_RGB43_800_480
//#define       LCD_TYPE_RGB70_1024_600
//#define       LCD_TYPE_MCU_320_240
//#define       LCD_TYPE_Vga_640_480_60HZ
//#define       LCD_TYPE_Vga_640_480_75HZ
//#define       LCD_TYPE_Vga_1024_768_60HZ
//#define       LCD_TYPE_Vga_1280_720_60HZ
//#define       LCD_TYPE_Vga_1360_768_60HZ
//#define       LCD_TYPE_TV_NTSC_720_480
//#define       LCD_TYPE_TV_PAL_720_576
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#ifdef  LCD_TYPE_RGB43_480_272
    #define CFG_LCD_XSIZE 480             //"LCD���-������",
    #define CFG_LCD_YSIZE 272             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   39806       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   29854       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB43_800_480
    #define CFG_LCD_XSIZE 800             //"LCD���-������",
    #define CFG_LCD_YSIZE 480             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   39806       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   29854       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB70_1024_600
    #define CFG_LCD_XSIZE 1024             //"LCD���-������",
    #define CFG_LCD_YSIZE 600              //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   64800       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   48600       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_MCU_320_240               //3.2��
    #define CFG_LCD_XSIZE 320             //"LCD���-������",
    #define CFG_LCD_YSIZE 240             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   29623       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   22217       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
//---VGA
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define CFG_LCD_XSIZE 1024             //"LCD���-������",
    #define CFG_LCD_YSIZE 768              //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_60HZ
    #define CFG_LCD_XSIZE 640             //"LCD���-������",
    #define CFG_LCD_YSIZE 480             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_75HZ
    #define CFG_LCD_XSIZE 640             //"LCD���-������",
    #define CFG_LCD_YSIZE 480             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define CFG_LCD_XSIZE 1024             //"LCD���-������",
    #define CFG_LCD_YSIZE 768              //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1280_720_60HZ
    #define CFG_LCD_XSIZE 1280             //"LCD���-������",
    #define CFG_LCD_YSIZE 720              //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1360_768_60HZ
    #define CFG_LCD_XSIZE 1360             //"LCD���-������",
    #define CFG_LCD_YSIZE 768              //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
//---TV (TV���ʱLCD��VGA�������)
#ifdef LCD_TYPE_TV_NTSC_720_480
    #define CFG_LCD_XSIZE 720             //"LCD���-������",
    #define CFG_LCD_YSIZE 480             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_TV_PAL_720_576
    #define CFG_LCD_XSIZE 720             //"LCD���-������",
    #define CFG_LCD_YSIZE 576             //"LCD�߶�-������",
    #define CFG_LCD_XSIZE_UM   1       //"LCD���-΢����",
    #define CFG_LCD_YSIZE_UM   1       //"LCD�߶�-΢����",
#endif

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8
/************************************************************************************
��boot�Ѿ���ʼ����LCD��Ӧ���ٴγ�ʼ��LCDʱ�����ʾ���ȵķ�����
1.����ʼ����ֱ��ʹ�õ�һ�γ�ʼ�����Դ档
    f1c100s_lcd_set_addr();  //����lcd��ַ
  �Դ��ַ=f1c100s_debe_get_address_x(0);    //��ȡ�Դ��ַ

2.����ʼ����ֱ�������µ��Դ��ַ��
  lcd_wait_vertical_blanking_interrupt();//֡�ж�
    f1c100s_lcd_set_addr();  //����lcd��ַ
    delay_ms(1);//������֡-��ʱ���GUI��ͼ
  f1c100s_debe_set_address_x(0,�Դ��ַ);    //�����µ��Դ��ַ

3.�ٴγ�ʼ��-����һ�γ�ʼ����IO��ڶ��γ�ʼ����IO��һ��ʱ��Sys_LCD_Init��������Ҫ�ȹر�ʱ�ӡ�
    Sys_LCD_Init(W,H,�Դ�1,�Դ�2);
************************************************************************************/
//LCDʱ��
struct __lcd_timing lcd_timing=
{
/*ģʽ ----ˮƽʱ��------,     -----��ֱʱ��------,   -���Կ���--,     ������  CPU_MODE     PCKL=(24MHz*N)/M/F  [5<F<96]*/
/*MODE WIDTH,HFP,HBP,HSPW,  HEIGHT,VFP,VBP,VSPW,  HA,VA,DA,CA,      s,            0,        N, M, F,                    */
#ifdef  LCD_TYPE_RGB43_480_272
     1,     480,  8, 43,  1,      272,  4, 12,  10,   0, 0, 1, 0,      1,         0,        65, 4,40 //PCLK=9.75MHZ
#endif
#ifdef LCD_TYPE_RGB43_800_480
     1,     800, 40, 87,  1,      480, 13, 31,   1,   0, 0, 1, 0,      1,           0,        65, 4,13 //PCLK=30MHZ
#endif
#ifdef LCD_TYPE_RGB70_1024_600
     2,    1024,160,160,  1,      600,  2, 23,  12,   0, 0, 1, 0,      1,         0,        65, 4, 8 //PCLK=48.75MHZ
#endif
//------------------------------VGA
#ifdef LCD_TYPE_Vga_640_480_60HZ
     2,     640, 16, 48, 96,      480, 10, 33,   2,   0, 0, 1, 0,      1,         0,        23, 2, 11//PCLK=25.09MHZ
#endif
#ifdef LCD_TYPE_Vga_640_480_75HZ
     2,     640, 16,120, 64,      480,  1, 16,   3,   0, 0, 1, 0,      1,         0,        63, 8, 6 //PCLK=31.5MHZ
#endif
#ifdef LCD_TYPE_Vga_1024_768_60HZ
     2,    1024, 24,160,136,      768,  3, 29,   6,   0, 0, 1, 0,      1,         0,        65, 4, 6 //PCLK=65MHZ
#endif
#ifdef LCD_TYPE_Vga_1280_720_60HZ
     2,    1280,110,220, 40,      720,  5, 20,   5,   0, 0, 1, 0,      1,         0,        99, 4, 8 //PCLK=74.25MHZ
#endif
#ifdef LCD_TYPE_Vga_1360_768_60HZ
     2,    1360, 64,256,112,      768,  3, 18,   6,   0, 0, 1, 0,      1,         0,        86, 4, 6 //PCLK=85.5MHZ
#endif
//------------------------------TV
#if (defined LCD_TYPE_TV_PAL_720_576)||(defined LCD_TYPE_TV_NTSC_720_480)
     7,    0,     0,  0,  0,        0,  0,  0,   0,   0, 0, 0, 0,      0,         0,        65, 4,13
#endif
//------------------------------MCU 8λ2����65Kɫ
#ifdef LCD_TYPE_MCU_320_240
     0,      320,  1, 1,  1,      240,  1,  1,   1,    1, 1, 0, 1,      2,        7,        65, 4, 12 //PCLK=MHZ
#endif

};
//ȫ�ֽṹ��
struct fb_f1c100s_pdata_t __lcd_pdat;
struct fb_f1c100s_pdata_t * lcd_pdat;

//RGBתYUVϵ��
static unsigned int csc_tab_rgb2yuv[4][12] =
{
 {0x0204,0x0107,0x0064,0x0100,0x1ed6,0x1f68,0x01c1,0x0800,0x1e87,0x01c1,0x1fb7,0x0800},//bt601 rgb2yuv
 {0x0274,0x00bb,0x003f,0x0100,0x1ea5,0x1f98,0x01c1,0x0800,0x1e67,0x01c1,0x1fd7,0x0800},//bt709 rgb2yuv
 {0x0258,0x0132,0x0075,0x0000,0x1eac,0x1f53,0x0200,0x0800,0x1e53,0x0200,0x1fac,0x0800},//DISP_YCC rgb2yuv
 {0x0274,0x00bb,0x003f,0x0100,0x1ea5,0x1f98,0x01c1,0x0800,0x1e67,0x01c1,0x1fd7,0x0800} //xvYCC rgb2yuv
};
/*YUVʹ�����*/
void YUV_OUT_Enable(char mode)
{
    int i;
    for(i=0x950;i<=0x97c;i+=4)
        write32(DEBE_Base_Address+i,csc_tab_rgb2yuv[mode][(i-0x950)/4]<<16);
    S_BIT(DEBE_Base_Address+0x800,5);
}
/*��TCONʱ��*/
void Tcon_Clk_Close(void)
{
    C_BIT(CCU_BUS_CLK_GATING_REG1,4);
    C_BIT(CCU_TCON_CLK_REG,31);
    C_BIT(CCU_BUS_SOFT_RST_REG1,4);
}
/*��TCONʱ��*/
void Tcon_Clk_Open(void)
{
    S_BIT(CCU_BUS_CLK_GATING_REG1,4);
    S_BIT(CCU_TCON_CLK_REG,31);
    S_BIT(CCU_BUS_SOFT_RST_REG1,4);

}
/*��Debeʱ��*/
void Debe_Clk_Open(void)
{
    S_BIT(CCU_BUS_CLK_GATING_REG1,12);
    S_BIT(CCU_BUS_SOFT_RST_REG1,12);
}
/*��Debeʱ��*/
void Debe_Clk_Close(void)
{
    C_BIT(CCU_BUS_CLK_GATING_REG1,12);
    C_BIT(CCU_BUS_SOFT_RST_REG1,12);
}
/*------------------------
���ò㴰�ڴ�С
------------------------*/
void Set_LayerX_window(int Layer,int x,int y,int w,int h)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    switch(Layer)
    {
        case 0:
            write32((virtual_addr_t)&debe->layer0_pos, (y<< 16) | (x << 0));
            write32((virtual_addr_t)&debe->layer0_size, ((h - 1) << 16) | ((w - 1) << 0));
            break;
        case 1:
            write32((virtual_addr_t)&debe->layer1_pos, (y<< 16) | (x << 0));
            write32((virtual_addr_t)&debe->layer1_size, ((h - 1) << 16) | ((w - 1) << 0));
            break;
        case 2:
            write32((virtual_addr_t)&debe->layer2_pos, (y<< 16) | (x << 0));
            write32((virtual_addr_t)&debe->layer2_size, ((h - 1) << 16) | ((w - 1) << 0));
            break;
        case 3:
            write32((virtual_addr_t)&debe->layer3_pos, (y<< 16) | (x << 0));
            write32((virtual_addr_t)&debe->layer3_size, ((h - 1) << 16) | ((w - 1) << 0));
            break;
    }
}
/*------------------------
ʹ�ܲ���Ƶͨ��
------------------------*/
void Enable_LayerX_Video(int Layer)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    switch(Layer)
    {
        case 0:
            S_BIT((virtual_addr_t)&debe->layer0_attr0_ctrl,1);
            break;
        case 1:
            S_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,1);
            break;
        case 2:
            S_BIT((virtual_addr_t)&debe->layer2_attr0_ctrl,1);
            break;
        case 3:
            S_BIT((virtual_addr_t)&debe->layer3_attr0_ctrl,1);
            break;
    }
}
/*------------------------
ʧ�ܲ���Ƶͨ��
------------------------*/
void Disable_LayerX_Video(int Layer)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    switch(Layer)
    {
        case 0:
            C_BIT((virtual_addr_t)&debe->layer0_attr0_ctrl,1);
            break;
        case 1:
            C_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,1);
            break;
        case 2:
            C_BIT((virtual_addr_t)&debe->layer2_attr0_ctrl,1);
            break;
        case 3:
            C_BIT((virtual_addr_t)&debe->layer3_attr0_ctrl,1);
            break;
    }
}
/*------------------------
�������ݸ�ʽ
[forma RGB565=0x5,stride=4][forma ARGB8888=0xA,stride=5]
------------------------*/
void Set_Layer_format(int Layer,int format,int stride)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    switch(Layer)
    {
        case 0:
            write32((virtual_addr_t)&debe->layer0_attr1_ctrl, format << 8);
            write32((virtual_addr_t)&debe->layer0_stride, ((lcd_timing.width) << stride));
            break;
        case 1:
            write32((virtual_addr_t)&debe->layer1_attr1_ctrl, format << 8);
            write32((virtual_addr_t)&debe->layer1_stride, ((lcd_timing.width) << stride));
            break;
        case 2:
            write32((virtual_addr_t)&debe->layer2_attr1_ctrl, format << 8);
            write32((virtual_addr_t)&debe->layer2_stride, ((lcd_timing.width) << stride));
            break;
        case 3:
            write32((virtual_addr_t)&debe->layer3_attr1_ctrl, format << 8);
            write32((virtual_addr_t)&debe->layer3_stride, ((lcd_timing.width) << stride));
            break;
    }
}
static int video_layer_inx=0;
/*------------------------
������Ƶ���
------------------------*/
void set_video_layer_inx(int layer_inx)
{
    video_layer_inx=layer_inx;
}
/*------------------------
������Ƶ����λ�����С
------------------------*/
void set_video_window(int x,int y,int w,int h)
{
    Set_LayerX_window(video_layer_inx,x,y,w,h);
}
/*------------------------
ʹ����Ƶͨ��
------------------------*/
void Enable_Layer_Video(void)
{
    Enable_LayerX_Video(video_layer_inx);
}
/*------------------------
ʧ����Ƶͨ��
------------------------*/
void Disable_Layer_Video(void)
{
    Disable_LayerX_Video(video_layer_inx);
}
/*------------------------
���ò����ȼ�
Priority=0-3    [3>2>1>0]
------------------------*/
void debe_set_layer_priority(int layer,int Priority)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    unsigned int val=read32((virtual_addr_t)&debe->layer1_attr0_ctrl);
    val&=~(0x3<<10);
    val|=(Priority<<10);
    write32((virtual_addr_t)&debe->layer1_attr0_ctrl,val);
}
/*TCON��ʼ��*/
void Tcon_Init(void)
{
    struct f1c100s_tcon_reg_t *tcon=((struct f1c100s_tcon_reg_t *)lcd_pdat->virttcon);
    struct fb_f1c100s_pdata_t * pdat = lcd_pdat;
    int val;
    int bp, total;
/*����VIDEO PLLʱ��*/
    C_BIT(CCU_Base_Address+0x010,31);
    write32(CCU_Base_Address+0x010,((lcd_timing.n-1)<<8)|((lcd_timing.m-1)<<0)|(3<<24));
    S_BIT(CCU_Base_Address+0x010,31);
    delay_ms(1);
/*����TCONʱ���븴λ*/
    S_BIT(CCU_BUS_CLK_GATING_REG1,4);
    S_BIT(CCU_TCON_CLK_REG,31);
    S_BIT(CCU_BUS_SOFT_RST_REG1,4);
    delay_ms(1);
    if(lcd_timing.mode!=7)//��TV
    {
    /*TCON����*/
        C_BIT(TCON_Base_Address+0x40,0);
        val = (lcd_timing.v_front_porch + lcd_timing.v_back_porch + lcd_timing.v_sync_len);
        write32(TCON_Base_Address+0x40,((u64_t)0x1 << 31) |(val & 0x1f) << 4);
        val = lcd_timing.f; // 5< DCLKDIV <96 ʱ�ӳ���
        write32(TCON_Base_Address+0x44, ((u64_t)0xf << 28) | (val << 0));
    /*���ÿ��*/
        write32((virtual_addr_t)&tcon->tcon0_timing_active, ((lcd_timing.width - 1) << 16) | ((lcd_timing.height - 1) << 0));
    /*����HT+HBP*/
        bp = lcd_timing.h_sync_len + lcd_timing.h_back_porch;
        total = (lcd_timing.width*lcd_timing.serial) + lcd_timing.h_front_porch + bp;
        write32((virtual_addr_t)&tcon->tcon0_timing_h, ((total - 1) << 16) | ((bp - 1) << 0));
    /*����VT+VBP*/
        bp = lcd_timing.v_sync_len + lcd_timing.v_back_porch;
        total = lcd_timing.height + lcd_timing.v_front_porch + bp;
        write32((virtual_addr_t)&tcon->tcon0_timing_v, ((total * 2) << 16) | ((bp - 1) << 0));
    /*����ʱ�ӿ��*/
        write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((lcd_timing.h_sync_len - 1) << 16) | ((lcd_timing.v_sync_len - 1) << 0));
    /*����RB����-���㲼��*/
    #if 0
        S_BIT(TCON_Base_Address+0x40,23);
    #endif
    /*����ģʽ*/
        if(lcd_timing.mode>0)//rgb
        {
            write32((virtual_addr_t)&tcon->tcon0_hv_intf, 0);
            write32((virtual_addr_t)&tcon->tcon0_cpu_intf, 0);
        }
        else//cpu
        {
            //����Ϊ8080ģʽ
            write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(1)<<24);
            //CPUģʽ
            write32(TCON_Base_Address+0x60,(u64_t)(lcd_timing.cpu_mode)<<29 | (u64_t)(1)<<28);
        }
        //��������Դ
        write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(0)<<0);//[3-��ɫ����][2-DMA][0-DE]
        //FRM
        if(pdat->frm_pixel == 18 || pdat->frm_pixel == 16)
        {
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[0], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[1], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[2], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[3], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[4], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_seed[5], 0x11111111);
            write32((virtual_addr_t)&tcon->tcon0_frm_table[0], 0x01010000);
            write32((virtual_addr_t)&tcon->tcon0_frm_table[1], 0x15151111);
            write32((virtual_addr_t)&tcon->tcon0_frm_table[2], 0x57575555);
            write32((virtual_addr_t)&tcon->tcon0_frm_table[3], 0x7f7f7777);
            write32((virtual_addr_t)&tcon->tcon0_frm_ctrl, (pdat->frm_pixel == 18) ? (((u64_t)1 << 31) | (0 << 4)) : (((u64_t)1 << 31) | (5 << 4)));
        }
        //���Կ���
        val = (1 << 28);
        if(!lcd_timing.h_sync_active)   val |= (1 << 25);
        if(!lcd_timing.v_sync_active)   val |= (1 << 24);
        if(!lcd_timing.den_active)      val |= (1 << 27);
        if(!lcd_timing.clk_active)      val |= (1 << 26);
        write32((virtual_addr_t)&tcon->tcon0_io_polarity, val);
        //�������ƹ�
        write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0);
    #if 0    //�ж����òο������ֲ�183-184ҳ
        //�ж�����
        IRQ_Init(IRQ_LEVEL_1,IRQ_TCON,TCON_ISR ,3);
    #if 1   //�����ж�
        S_BIT(TCON_Base_Address+0x04,31);//TCON0 vertical blanking interrupt Enable
    #endif
    #if 0   //���ж�
        write32(TCON_Base_Address+0x08,(pdat->height)<<16);//set line
        S_BIT(TCON_Base_Address+0x04,29);//TCON0 line trigger interrupt enable
    #endif
        sysSetLocalInterrupt(ENABLE_IRQ);//��IRQ�ж�
    #endif
    }else
    { //TV
        write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(3)<<0);//[3-��ɫ����][2-DMA][0-DE]
    }
}
/*debe����*/
void Debe_Init(void)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    struct fb_f1c100s_pdata_t * pdat = lcd_pdat;
//ʱ���븴λ
    S_BIT(CCU_BUS_CLK_GATING_REG1,12);
    S_BIT(CCU_BUS_SOFT_RST_REG1,12);
    delay_ms(1);
//ʹ��DEBE
    S_BIT((virtual_addr_t)&debe->mode,0);
    write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
//���ò�0����
    write32((virtual_addr_t)&debe->layer0_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
    write32((virtual_addr_t)&debe->layer0_stride, ((pdat->width) << 4));//[RGB565=4][ARGB8888=5]
    write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32)(pdat->vram[0]) << 3);
    write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32)(pdat->vram[0]) >> 29);
    write32((virtual_addr_t)&debe->layer0_attr1_ctrl, 0x5 << 8);//[RGB565=0x5][ARGB8888=0xA]
    S_BIT((virtual_addr_t)&debe->mode,8);//��0ʹ��
  if((pdat->vram[1]!=0)&&(pdat->vram[0]!=pdat->vram[1]))
    {
//���ò�1����
        write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
        write32((virtual_addr_t)&debe->layer1_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
        write32((virtual_addr_t)&debe->layer1_stride, ((pdat->width) <<5));//[RGB565=4][ARGB8888=5]
        write32((virtual_addr_t)&debe->layer1_addr_low32b, (u32)(pdat->vram[1]) << 3);
        write32((virtual_addr_t)&debe->layer1_addr_high4b, (u32)(pdat->vram[1]) >> 29);
        write32((virtual_addr_t)&debe->layer1_attr1_ctrl, 0x0A << 8);//[RGB565=0x5][ARGB8888=0xA]

        debe_set_layer_priority(0,1);//�������ȼ� ��0>��1
        S_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,15);//1: select Pipe 1 ��Ҫ͸������ʱ��Ҫ���벻ͬ�ܵ�  ��0=pipe0 ��1=pipe1

        S_BIT((virtual_addr_t)&debe->mode,9);//��1ʹ��
    }
//����
    S_BIT((virtual_addr_t)&debe->reg_ctrl,0);
//DEBE ��ʼ
    S_BIT((virtual_addr_t)&debe->mode,1);
}
/*debe���ò�0�Դ��ַ*/
void f1c100s_debe_set_address(void * vram)
{
    f1c100s_debe_set_address_x(0,vram);
}
/*debe���ز�0�Դ��ַ*/
unsigned int f1c100s_debe_get_address(void)
{
    return f1c100s_debe_get_address_x(0);
}
/*debe���ò��Դ��ַ-���ڳ�����ʱ����л�
vram=�Դ�
layer=0-3
*/
void f1c100s_debe_set_address_x(int layer,void * vram)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    switch(layer)
    {
        case 0:
            write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32_t)vram << 3);
            write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32_t)vram >> 29);
            break;
        case 1:
            write32((virtual_addr_t)&debe->layer1_addr_low32b, (u32_t)vram << 3);
            write32((virtual_addr_t)&debe->layer1_addr_high4b, (u32_t)vram >> 29);
            break;
        case 2:
            write32((virtual_addr_t)&debe->layer2_addr_low32b, (u32_t)vram << 3);
            write32((virtual_addr_t)&debe->layer2_addr_high4b, (u32_t)vram >> 29);
            break;
        case 3:
            write32((virtual_addr_t)&debe->layer3_addr_low32b, (u32_t)vram << 3);
            write32((virtual_addr_t)&debe->layer3_addr_high4b, (u32_t)vram >> 29);
            break;
    }
}
/*debe���ز��Դ��ַ
layer=0-3
*/
unsigned int f1c100s_debe_get_address_x(int layer)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    unsigned int l32=0,h4=0;
    switch(layer)
    {
        case 0:
            l32=read32((virtual_addr_t)&debe->layer0_addr_low32b);
            h4=read32((virtual_addr_t)&debe->layer0_addr_high4b);
            break;
        case 1:
            l32=read32((virtual_addr_t)&debe->layer1_addr_low32b);
            h4=read32((virtual_addr_t)&debe->layer1_addr_high4b);
            break;
        case 2:
            l32=read32((virtual_addr_t)&debe->layer2_addr_low32b);
            h4=read32((virtual_addr_t)&debe->layer2_addr_high4b);
            break;
        case 3:
            l32=read32((virtual_addr_t)&debe->layer3_addr_low32b);
            h4=read32((virtual_addr_t)&debe->layer3_addr_high4b);
            break;
    }
    unsigned int addr=(l32>>3)|(h4<<29);
    return addr;
}
/*tconʹ��*/
void f1c100s_tcon_enable(void)
{
    struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)lcd_pdat->virttcon;
    u32_t val;

    val = read32((virtual_addr_t)&tcon->ctrl);
    val |= ((u64_t)1 << 31);
    write32((virtual_addr_t)&tcon->ctrl, val);
}
/*tconʧ��*/
void f1c100s_tcon_disable(void)
{
    struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)lcd_pdat->virttcon;
    u32_t val;

    write32((virtual_addr_t)&tcon->ctrl, 0);
    write32((virtual_addr_t)&tcon->int0, 0);

    val = read32((virtual_addr_t)&tcon->tcon0_dclk);
    val &= ~((u64_t)0xf << 28);
    write32((virtual_addr_t)&tcon->tcon0_dclk, val);

    write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0xffffffff);
    write32((virtual_addr_t)&tcon->tcon1_io_tristate, 0xffffffff);
}
/*���õ�ַ*/
void f1c100s_lcd_set_addr(void)
{
    lcd_pdat=&__lcd_pdat;
    //���õ�ַ
    lcd_pdat->virtdebe = F1C100S_DEBE_BASE;
    lcd_pdat->virttcon = F1C100S_TCON_BASE;
}
/*LCD����*/
void F1C100S_LCD_Config(int width,int height,unsigned int *buff1,unsigned int *buff2)
{
    //���õ�ַ
  f1c100s_lcd_set_addr();
    //LCD���
    lcd_pdat->width = width;
    lcd_pdat->height = height;
    //FRM-��������-���Դ�ΪRGB888,IO����ΪRGB565��RGB666ʱ����ӽ�RGB888��Ч��
    if(lcd_timing.mode==1)lcd_pdat->frm_pixel = 16;
    else if(lcd_timing.mode==2)lcd_pdat->frm_pixel = 18;
    else lcd_pdat->frm_pixel = 0;/*����Ϊ0*/
    //���û���
    lcd_pdat->vram[0] = buff1;
    lcd_pdat->vram[1] = buff2;
#if 1
    //��Ĵ���
    if((read32(TCON_Base_Address+0x00)&((u64_t)1<<31))==0) //�ж�LCD����
    {
        for(int i = 0x0800; i < 0x1000; i += 4)
        write32(F1C100S_DEBE_BASE + i, 0);
    }
#endif
    //debe+tcon����
#if 0
    f1c100s_tcon_disable(lcd_pdat);
#endif

  Debe_Init();
    Tcon_Init();

    //ʹ��TV��TV��LCD���ֻ��2ѡ1
#if (defined LCD_TYPE_TV_PAL_720_576)||(defined LCD_TYPE_TV_NTSC_720_480)
    TVE_Init();
    YUV_OUT_Enable(0);
#endif
    f1c100s_tcon_enable();
}
/*LCD IO��ʼ��*/
void LCD_IO_Init(void)
{
    int i=0;
    //
    if(lcd_timing.mode>0)//RGB
    {
        if(lcd_timing.mode==1)//RGB565
        {
            for(i=0;i<=21;i++)
            {
                if((i==0)||(i==12))continue;
                GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            }
        }
        else if(lcd_timing.mode==2)//RGB666
        {
            for(i=0;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
        else if(lcd_timing.mode==3)//RGB888
        {
            for(i=0;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            for(i=0;i<=5;i++)GPIO_Congif(GPIOE,GPIO_Pin_0+i,GPIO_Mode_011,GPIO_PuPd_NOPULL);
        }
        else if(lcd_timing.mode==4)//RGB8λ����
        {
            for(i=18;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
        else if(lcd_timing.mode==5)//CCIR656
        {
            GPIO_Congif(GPIOD,GPIO_Pin_18,GPIO_Mode_010,GPIO_PuPd_NOPULL);//ֻ��Ҫһ��ʱ����
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
    }else//MCU
    {
        for(i=18;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        if((lcd_timing.cpu_mode==6)||(lcd_timing.cpu_mode==7))//MCU 8λ
        {
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
        else if((lcd_timing.cpu_mode==1)||(lcd_timing.cpu_mode==2)||(lcd_timing.cpu_mode==3)||(lcd_timing.cpu_mode==4))//MCU 16λ
        {
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            for(i=10;i<=17;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
    }
}
/*�ȴ�֡�ж�*/
void lcd_wait_vertical_blanking_interrupt(void)
{
    if(read32(TCON_Base_Address+0x00)&((u64_t)1<<31)) //�ж�LCD����
    {
        //sysprintf("The LCD has been initialized.\r\n");
        C_BIT(TCON_Base_Address+0x04,15);
        while(1)
        {
            if(read32(TCON_Base_Address+0x04)&(1<<15))break;
        }
    }
}
/*LCD��ʼ��*/
void Sys_LCD_Init(int width,int height,unsigned int *buff1,unsigned int *buff2)
{
#if 0 //��γ�ʼ��Ϊ��ʱ�������Բ��ù�ʱ��
    Tcon_Clk_Close();
    Debe_Clk_Close();
#endif
  lcd_wait_vertical_blanking_interrupt();//�ȴ�֡�ж�
    LCD_IO_Init();//IO��ʼ��-�����һ�γ�ʼ��IO��ڶ��γ�ʼ��IO��һ������Ҫ�ȹر�ʱ���ٳ�ʼ��
    F1C100S_LCD_Config(width,height,buff1,buff2);//TCON DEBE ��ʼ��
}
/*TCON�ж�*/
void TCON_ISR(void)
{
    //����TCON_INT_REG0��Ҫд0����жϱ�־-�ο������ֲ�183ҳ
    C_BIT(TCON_Base_Address+0x04,15);//���TCON0�����ж�

}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#define cn_lcd_line_size        (CFG_LCD_XSIZE<<1)
#define cn_frame_buffer_size    (cn_lcd_line_size * CFG_LCD_YSIZE)
#define CN_LCD_PIXEL_FORMAT     CN_SYS_PF_RGB565
u16 u16g_FrameBuffer[CFG_LCD_XSIZE*CFG_LCD_YSIZE] __attribute__((aligned(4)));

struct DisplayObj tg_lcd_display;

#define ROW_TEMP 50
#define COLO_DEP 2

//������djyguiҪ��Ľӿں������������£�������ÿպ�������

//----λͼ�л�����-------------------------------------------------------------
//����: ��λͼ�л�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ���ơ�
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ��ء�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x��y������
//      color����ͼ�õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
//      r2_code����Ԫ��դ������
//����: ��
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
//  printf("__lcd_set_pixel_bm\r\n");
    return false;
}

//----λͼ�л���---------------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�������(x2,y2)�㣬ֻ������limit�޶��������ڵĲ��֡�
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
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
//      color����ͼ�õ���ɫ��CN_SYS_PF_ERGB8888��ʽ
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
                          u32 Color0,u32 Color1,u32 Mode,s32 radius)
{
    u32 y;
    u16 pixel;
    u16 *dst_offset;
    u32 i = 0, len = 0;
//    printf("__lcd_fill_rect_bm\r\n");
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);
//    printf("pixel = %4x\r\n",pixel);
    if (radius == 0)
    {
        dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
            + Focus->top * dst_bitmap->linebytes);
        dst_offset += Focus->left;

        for (y = Focus->top; y < Focus->bottom; y++)
        {
            len = (Focus->right - Focus->left);
            for (i = 0; i < len; i++)
            {
                dst_offset[i] = pixel;
            }
            dst_offset += dst_bitmap->linebytes >> 1;

        }
        return true;
    }
    else//Բ�Ǿ���
    {
        s32 OutConst, Sum, SumY;
        s32 move_x, x, move_y, flag, flag_x;
        s32 cx, cy, dy, dx, arc_ry, arc_rx, _max, min_y;
        cx = ((Target->right - Target->left) / 2) + Target->left;//���ĵ�����
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
                if (move_y >= Focus->top && move_y < Focus->bottom)//�ϱ߻�start
                {
                    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                        + move_y * dst_bitmap->linebytes);

                    x = cx - move_x - dx;
                    if (x <= Focus->left) x = Focus->left;
                    else if (x >= Focus->right) x = Focus->right;

                    _max = cx + move_x + dx + flag_x;
                    if (_max >= Focus->right) _max = Focus->right;
                    else if (_max <= Focus->left) _max = Focus->left;

                    dst_offset += x;
                    len = _max - x;
                    for (i = 0; i < len; i++)
                    {
                        dst_offset[i] = pixel;
                    }

                }//�ϱ߻�end
                move_y = cy + y + dy - flag;
                if (move_y < Focus->bottom&&move_y >= Focus->top)//�±߻�start
                {
                    dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
                        + move_y * dst_bitmap->linebytes);
                    x = cx - move_x - dx;
                    if (x <= Focus->left) x = Focus->left;
                    else if (x >= Focus->right) x = Focus->right;

                    _max = cx + move_x + dx + flag_x;
                    if (_max >= Focus->right) _max = Focus->right;
                    else if (_max <= Focus->left) _max = Focus->left;

                    dst_offset += x;
                    len = _max - x;
                    for (i = 0; i < len; i++)
                    {
                        dst_offset[i] = pixel;
                    }
                }//�±߻�end
            }

        }
        //*********�м���β���start
        if (cy - dy <= Focus->top) y = Focus->top;//���θ߶�
        else y = cy - dy;
        if (cy + dy + 1 >= Focus->bottom) _max = Focus->bottom;
        else _max = cy + dy + 1;
        dst_offset = (u16*)((ptu32_t)dst_bitmap->bm_bits
            + y * dst_bitmap->linebytes);
        dst_offset += Focus->left;

        for (; y < _max; y++)
        {
            len = (Focus->right - Focus->left);
            for (i = 0; i < len; i++)
            {
                dst_offset[i] = pixel;
            }
            dst_offset += dst_bitmap->linebytes >> 1;

        }//*********�м���β���end
        return true;
    }//Բ�Ǿ���
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
    u16 dest;
    u32 pixel;

    if(CN_R2_COPYPEN == r2_code)
    {
        dest = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,color);
    }else
    {
        printf("__ST7789V_get_pixel\r\n");
        pixel = GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                                        u16g_FrameBuffer[x + y * CFG_LCD_XSIZE],0);
        pixel = GK_BlendRop2(color, pixel, r2_code);
        dest = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,pixel);
    }
    u16g_FrameBuffer[x + y * CFG_LCD_XSIZE] = dest;
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

    return false;
}

//���ڴ滺������screenλ�鴫�ͣ�ֻ֧�ֿ鿽������֧��rop������
//������ʾ����driver���ṩ�������
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    return false;
}

//��screen��ȡһ���أ���ת����CN_SYS_PF_ERGB8888
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
//  printf("__lcd_get_pixel_screen\r\n");
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                                    u16g_FrameBuffer[x + y * CFG_LCD_XSIZE],0);
}

//��screen�ھ�����������ݸ��Ƶ�bitmap������ǰ�������ú�dest��pf_type
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
//  printf("__lcd_get_rect_screen\r\n");
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

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_F1CLCD(const char *DisplayName,const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;

    Sys_LCD_Init(CFG_LCD_XSIZE, CFG_LCD_YSIZE, u16g_FrameBuffer, NULL);

    FrameBitmap.bm_bits = u16g_FrameBuffer;
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
    tg_lcd_display.framebuf_direct = true;
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

    tg_lcd_display.DisplayHeap = NULL;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,DisplayName);
    return (ptu32_t)&tg_lcd_display;
}



