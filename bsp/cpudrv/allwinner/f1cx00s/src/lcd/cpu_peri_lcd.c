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
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
#if (defined LCD_TYPE_TV_PAL_720_576)||(defined LCD_TYPE_TV_NTSC_720_480)
    #include "cpu_peri_tve.h"
#endif

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern ptu32_t ModuleInstall_F1CLCD(const char *DisplayName,const char* HeapName);
//  ModuleInstall_F1CLCD(CFG_ONCHIP_DISPLAY_NAME,NULL);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"F1C-LCD"      //LCD显示驱动
//parent:"graphical kernel"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main            //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"graphical kernel","heap" //该组件的依赖组件名（可以是none，表示无依赖组件），
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
#if ( CFG_MODULE_ENABLE_LCD_DRIVER_ST7789V == false )
//#warning  " keyboard_hard_driver  组件参数未配置，使用默认配置"
#define CFG_MODULE_ENABLE_LCD_DRIVER_ST7789V    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
//%$#@num,,,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_ONCHIP_DISPLAY_NAME        "F1C-LCD"    //"显示器名称",配置液晶显示的名称
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
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#ifdef  LCD_TYPE_RGB43_480_272
    #define CFG_LCD_XSIZE 480             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 272             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   39806       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   29854       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB43_800_480
    #define CFG_LCD_XSIZE 800             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 480             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   39806       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   29854       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_RGB70_1024_600
    #define CFG_LCD_XSIZE 1024             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 600              //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   64800       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   48600       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_MCU_320_240               //3.2寸
    #define CFG_LCD_XSIZE 320             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 240             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   29623       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   22217       //"LCD高度-微米数",
#endif
//-------------------------------------------
//---VGA
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define CFG_LCD_XSIZE 1024             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 768              //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_60HZ
    #define CFG_LCD_XSIZE 640             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 480             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_640_480_75HZ
    #define CFG_LCD_XSIZE 640             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 480             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1024_768_60HZ
    #define CFG_LCD_XSIZE 1024             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 768              //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1280_720_60HZ
    #define CFG_LCD_XSIZE 1280             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 720              //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_Vga_1360_768_60HZ
    #define CFG_LCD_XSIZE 1360             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 768              //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
//---TV (TV输出时LCD与VGA不能输出)
#ifdef LCD_TYPE_TV_NTSC_720_480
    #define CFG_LCD_XSIZE 720             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 480             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif
//-------------------------------------------
#ifdef LCD_TYPE_TV_PAL_720_576
    #define CFG_LCD_XSIZE 720             //"LCD宽度-像素数",
    #define CFG_LCD_YSIZE 576             //"LCD高度-像素数",
    #define CFG_LCD_XSIZE_UM   1       //"LCD宽度-微米数",
    #define CFG_LCD_YSIZE_UM   1       //"LCD高度-微米数",
#endif

#define delay_ms(x) DJY_DelayUs(x * 1000)
#define u64_t u64
#define u32_t u32
#define u16_t u16
#define u8_t  u8
/************************************************************************************
当boot已经初始化了LCD，应用再次初始化LCD时解决显示过度的方法：
1.不初始化，直接使用第一次初始化的显存。
    f1c100s_lcd_set_addr();  //设置lcd地址
  显存地址=f1c100s_debe_get_address_x(0);    //获取显存地址

2.不初始化，直接设置新的显存地址。
  lcd_wait_vertical_blanking_interrupt();//帧中断
    f1c100s_lcd_set_addr();  //设置lcd地址
    delay_ms(1);//跳过本帧-留时间给GUI绘图
  f1c100s_debe_set_address_x(0,显存地址);    //设置新的显存地址

3.再次初始化-当第一次初始化的IO与第二次初始化的IO不一样时，Sys_LCD_Init函数里需要先关闭时钟。
    Sys_LCD_Init(W,H,显存1,显存2);
************************************************************************************/
//LCD时序
struct __lcd_timing lcd_timing=
{
/*模式 ----水平时序------,     -----垂直时序------,   -极性控制--,     串并行  CPU_MODE     PCKL=(24MHz*N)/M/F  [5<F<96]*/
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
//------------------------------MCU 8位2串行65K色
#ifdef LCD_TYPE_MCU_320_240
     0,      320,  1, 1,  1,      240,  1,  1,   1,    1, 1, 0, 1,      2,        7,        65, 4, 12 //PCLK=MHZ
#endif

};
//全局结构体
struct fb_f1c100s_pdata_t __lcd_pdat;
struct fb_f1c100s_pdata_t * lcd_pdat;

//RGB转YUV系数
static unsigned int csc_tab_rgb2yuv[4][12] =
{
 {0x0204,0x0107,0x0064,0x0100,0x1ed6,0x1f68,0x01c1,0x0800,0x1e87,0x01c1,0x1fb7,0x0800},//bt601 rgb2yuv
 {0x0274,0x00bb,0x003f,0x0100,0x1ea5,0x1f98,0x01c1,0x0800,0x1e67,0x01c1,0x1fd7,0x0800},//bt709 rgb2yuv
 {0x0258,0x0132,0x0075,0x0000,0x1eac,0x1f53,0x0200,0x0800,0x1e53,0x0200,0x1fac,0x0800},//DISP_YCC rgb2yuv
 {0x0274,0x00bb,0x003f,0x0100,0x1ea5,0x1f98,0x01c1,0x0800,0x1e67,0x01c1,0x1fd7,0x0800} //xvYCC rgb2yuv
};
/*YUV使能输出*/
void YUV_OUT_Enable(char mode)
{
    int i;
    for(i=0x950;i<=0x97c;i+=4)
        write32(DEBE_Base_Address+i,csc_tab_rgb2yuv[mode][(i-0x950)/4]<<16);
    S_BIT(DEBE_Base_Address+0x800,5);
}
/*关TCON时钟*/
void Tcon_Clk_Close(void)
{
    C_BIT(CCU_BUS_CLK_GATING_REG1,4);
    C_BIT(CCU_TCON_CLK_REG,31);
    C_BIT(CCU_BUS_SOFT_RST_REG1,4);
}
/*开TCON时钟*/
void Tcon_Clk_Open(void)
{
    S_BIT(CCU_BUS_CLK_GATING_REG1,4);
    S_BIT(CCU_TCON_CLK_REG,31);
    S_BIT(CCU_BUS_SOFT_RST_REG1,4);

}
/*开Debe时钟*/
void Debe_Clk_Open(void)
{
    S_BIT(CCU_BUS_CLK_GATING_REG1,12);
    S_BIT(CCU_BUS_SOFT_RST_REG1,12);
}
/*关Debe时钟*/
void Debe_Clk_Close(void)
{
    C_BIT(CCU_BUS_CLK_GATING_REG1,12);
    C_BIT(CCU_BUS_SOFT_RST_REG1,12);
}
/*------------------------
设置层窗口大小
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
使能层视频通道
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
失能层视频通道
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
设置数据格式
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
设置视频层号
------------------------*/
void set_video_layer_inx(int layer_inx)
{
    video_layer_inx=layer_inx;
}
/*------------------------
设置视频窗口位置与大小
------------------------*/
void set_video_window(int x,int y,int w,int h)
{
    Set_LayerX_window(video_layer_inx,x,y,w,h);
}
/*------------------------
使能视频通道
------------------------*/
void Enable_Layer_Video(void)
{
    Enable_LayerX_Video(video_layer_inx);
}
/*------------------------
失能视频通道
------------------------*/
void Disable_Layer_Video(void)
{
    Disable_LayerX_Video(video_layer_inx);
}
/*------------------------
设置层优先级
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
/*TCON初始化*/
void Tcon_Init(void)
{
    struct f1c100s_tcon_reg_t *tcon=((struct f1c100s_tcon_reg_t *)lcd_pdat->virttcon);
    struct fb_f1c100s_pdata_t * pdat = lcd_pdat;
    int val;
    int bp, total;
/*设置VIDEO PLL时钟*/
    C_BIT(CCU_Base_Address+0x010,31);
    write32(CCU_Base_Address+0x010,((lcd_timing.n-1)<<8)|((lcd_timing.m-1)<<0)|(3<<24));
    S_BIT(CCU_Base_Address+0x010,31);
    delay_ms(1);
/*设置TCON时钟与复位*/
    S_BIT(CCU_BUS_CLK_GATING_REG1,4);
    S_BIT(CCU_TCON_CLK_REG,31);
    S_BIT(CCU_BUS_SOFT_RST_REG1,4);
    delay_ms(1);
    if(lcd_timing.mode!=7)//非TV
    {
    /*TCON配置*/
        C_BIT(TCON_Base_Address+0x40,0);
        val = (lcd_timing.v_front_porch + lcd_timing.v_back_porch + lcd_timing.v_sync_len);
        write32(TCON_Base_Address+0x40,((u64_t)0x1 << 31) |(val & 0x1f) << 4);
        val = lcd_timing.f; // 5< DCLKDIV <96 时钟除数
        write32(TCON_Base_Address+0x44, ((u64_t)0xf << 28) | (val << 0));
    /*设置宽高*/
        write32((virtual_addr_t)&tcon->tcon0_timing_active, ((lcd_timing.width - 1) << 16) | ((lcd_timing.height - 1) << 0));
    /*设置HT+HBP*/
        bp = lcd_timing.h_sync_len + lcd_timing.h_back_porch;
        total = (lcd_timing.width*lcd_timing.serial) + lcd_timing.h_front_porch + bp;
        write32((virtual_addr_t)&tcon->tcon0_timing_h, ((total - 1) << 16) | ((bp - 1) << 0));
    /*设置VT+VBP*/
        bp = lcd_timing.v_sync_len + lcd_timing.v_back_porch;
        total = lcd_timing.height + lcd_timing.v_front_porch + bp;
        write32((virtual_addr_t)&tcon->tcon0_timing_v, ((total * 2) << 16) | ((bp - 1) << 0));
    /*设置时钟宽度*/
        write32((virtual_addr_t)&tcon->tcon0_timing_sync, ((lcd_timing.h_sync_len - 1) << 16) | ((lcd_timing.v_sync_len - 1) << 0));
    /*设置RB交换-方便布线*/
    #if 0
        S_BIT(TCON_Base_Address+0x40,23);
    #endif
    /*设置模式*/
        if(lcd_timing.mode>0)//rgb
        {
            write32((virtual_addr_t)&tcon->tcon0_hv_intf, 0);
            write32((virtual_addr_t)&tcon->tcon0_cpu_intf, 0);
        }
        else//cpu
        {
            //设置为8080模式
            write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(1)<<24);
            //CPU模式
            write32(TCON_Base_Address+0x60,(u64_t)(lcd_timing.cpu_mode)<<29 | (u64_t)(1)<<28);
        }
        //设置输入源
        write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(0)<<0);//[3-白色数据][2-DMA][0-DE]
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
        //极性控制
        val = (1 << 28);
        if(!lcd_timing.h_sync_active)   val |= (1 << 25);
        if(!lcd_timing.v_sync_active)   val |= (1 << 24);
        if(!lcd_timing.den_active)      val |= (1 << 27);
        if(!lcd_timing.clk_active)      val |= (1 << 26);
        write32((virtual_addr_t)&tcon->tcon0_io_polarity, val);
        //触发控制关
        write32((virtual_addr_t)&tcon->tcon0_io_tristate, 0);
    #if 0    //中断配置参考数据手册183-184页
        //中断配置
        IRQ_Init(IRQ_LEVEL_1,IRQ_TCON,TCON_ISR ,3);
    #if 1   //消隐中断
        S_BIT(TCON_Base_Address+0x04,31);//TCON0 vertical blanking interrupt Enable
    #endif
    #if 0   //线中断
        write32(TCON_Base_Address+0x08,(pdat->height)<<16);//set line
        S_BIT(TCON_Base_Address+0x04,29);//TCON0 line trigger interrupt enable
    #endif
        sysSetLocalInterrupt(ENABLE_IRQ);//开IRQ中断
    #endif
    }else
    { //TV
        write32(TCON_Base_Address+0x40,read32(TCON_Base_Address+0x40)|(3)<<0);//[3-白色数据][2-DMA][0-DE]
    }
}
/*debe配置*/
void Debe_Init(void)
{
    struct f1c100s_debe_reg_t * debe = (struct f1c100s_debe_reg_t *)lcd_pdat->virtdebe;
    struct fb_f1c100s_pdata_t * pdat = lcd_pdat;
//时钟与复位
    S_BIT(CCU_BUS_CLK_GATING_REG1,12);
    S_BIT(CCU_BUS_SOFT_RST_REG1,12);
    delay_ms(1);
//使能DEBE
    S_BIT((virtual_addr_t)&debe->mode,0);
    write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
//设置层0参数
    write32((virtual_addr_t)&debe->layer0_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
    write32((virtual_addr_t)&debe->layer0_stride, ((pdat->width) << 4));//[RGB565=4][ARGB8888=5]
    write32((virtual_addr_t)&debe->layer0_addr_low32b, (u32)(pdat->vram[0]) << 3);
    write32((virtual_addr_t)&debe->layer0_addr_high4b, (u32)(pdat->vram[0]) >> 29);
    write32((virtual_addr_t)&debe->layer0_attr1_ctrl, 0x5 << 8);//[RGB565=0x5][ARGB8888=0xA]
    S_BIT((virtual_addr_t)&debe->mode,8);//层0使能
  if((pdat->vram[1]!=0)&&(pdat->vram[0]!=pdat->vram[1]))
    {
//设置层1参数
        write32((virtual_addr_t)&debe->disp_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
        write32((virtual_addr_t)&debe->layer1_size, (((pdat->height) - 1) << 16) | (((pdat->width) - 1) << 0));
        write32((virtual_addr_t)&debe->layer1_stride, ((pdat->width) <<5));//[RGB565=4][ARGB8888=5]
        write32((virtual_addr_t)&debe->layer1_addr_low32b, (u32)(pdat->vram[1]) << 3);
        write32((virtual_addr_t)&debe->layer1_addr_high4b, (u32)(pdat->vram[1]) >> 29);
        write32((virtual_addr_t)&debe->layer1_attr1_ctrl, 0x0A << 8);//[RGB565=0x5][ARGB8888=0xA]

        debe_set_layer_priority(0,1);//设置优先级 层0>层1
        S_BIT((virtual_addr_t)&debe->layer1_attr0_ctrl,15);//1: select Pipe 1 需要透明叠加时需要输入不同管道  层0=pipe0 层1=pipe1

        S_BIT((virtual_addr_t)&debe->mode,9);//层1使能
    }
//加载
    S_BIT((virtual_addr_t)&debe->reg_ctrl,0);
//DEBE 开始
    S_BIT((virtual_addr_t)&debe->mode,1);
}
/*debe设置层0显存地址*/
void f1c100s_debe_set_address(void * vram)
{
    f1c100s_debe_set_address_x(0,vram);
}
/*debe返回层0显存地址*/
unsigned int f1c100s_debe_get_address(void)
{
    return f1c100s_debe_get_address_x(0);
}
/*debe设置层显存地址-会在场消隐时完成切换
vram=显存
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
/*debe返回层显存地址
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
/*tcon使能*/
void f1c100s_tcon_enable(void)
{
    struct f1c100s_tcon_reg_t * tcon = (struct f1c100s_tcon_reg_t *)lcd_pdat->virttcon;
    u32_t val;

    val = read32((virtual_addr_t)&tcon->ctrl);
    val |= ((u64_t)1 << 31);
    write32((virtual_addr_t)&tcon->ctrl, val);
}
/*tcon失能*/
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
/*设置地址*/
void f1c100s_lcd_set_addr(void)
{
    lcd_pdat=&__lcd_pdat;
    //设置地址
    lcd_pdat->virtdebe = F1C100S_DEBE_BASE;
    lcd_pdat->virttcon = F1C100S_TCON_BASE;
}
/*LCD配置*/
void F1C100S_LCD_Config(int width,int height,unsigned int *buff1,unsigned int *buff2)
{
    //设置地址
  f1c100s_lcd_set_addr();
    //LCD宽高
    lcd_pdat->width = width;
    lcd_pdat->height = height;
    //FRM-抖动功能-当显存为RGB888,IO定义为RGB565或RGB666时输出接近RGB888的效果
    if(lcd_timing.mode==1)lcd_pdat->frm_pixel = 16;
    else if(lcd_timing.mode==2)lcd_pdat->frm_pixel = 18;
    else lcd_pdat->frm_pixel = 0;/*其它为0*/
    //设置缓存
    lcd_pdat->vram[0] = buff1;
    lcd_pdat->vram[1] = buff2;
#if 1
    //清寄存器
    if((read32(TCON_Base_Address+0x00)&((u64_t)1<<31))==0) //判断LCD开启
    {
        for(int i = 0x0800; i < 0x1000; i += 4)
        write32(F1C100S_DEBE_BASE + i, 0);
    }
#endif
    //debe+tcon配置
#if 0
    f1c100s_tcon_disable(lcd_pdat);
#endif

  Debe_Init();
    Tcon_Init();

    //使能TV，TV与LCD输出只能2选1
#if (defined LCD_TYPE_TV_PAL_720_576)||(defined LCD_TYPE_TV_NTSC_720_480)
    TVE_Init();
    YUV_OUT_Enable(0);
#endif
    f1c100s_tcon_enable();
}
/*LCD IO初始化*/
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
        else if(lcd_timing.mode==4)//RGB8位串行
        {
            for(i=18;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
        else if(lcd_timing.mode==5)//CCIR656
        {
            GPIO_Congif(GPIOD,GPIO_Pin_18,GPIO_Mode_010,GPIO_PuPd_NOPULL);//只需要一个时钟线
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
    }else//MCU
    {
        for(i=18;i<=21;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        if((lcd_timing.cpu_mode==6)||(lcd_timing.cpu_mode==7))//MCU 8位
        {
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
        else if((lcd_timing.cpu_mode==1)||(lcd_timing.cpu_mode==2)||(lcd_timing.cpu_mode==3)||(lcd_timing.cpu_mode==4))//MCU 16位
        {
            for(i=1;i<=8;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
            for(i=10;i<=17;i++)GPIO_Congif(GPIOD,GPIO_Pin_0+i,GPIO_Mode_010,GPIO_PuPd_NOPULL);
        }
    }
}
/*等待帧中断*/
void lcd_wait_vertical_blanking_interrupt(void)
{
    if(read32(TCON_Base_Address+0x00)&((u64_t)1<<31)) //判断LCD开启
    {
        //sysprintf("The LCD has been initialized.\r\n");
        C_BIT(TCON_Base_Address+0x04,15);
        while(1)
        {
            if(read32(TCON_Base_Address+0x04)&(1<<15))break;
        }
    }
}
/*LCD初始化*/
void Sys_LCD_Init(int width,int height,unsigned int *buff1,unsigned int *buff2)
{
#if 0 //多次初始化为了时钟连续性不用关时钟
    Tcon_Clk_Close();
    Debe_Clk_Close();
#endif
  lcd_wait_vertical_blanking_interrupt();//等待帧中断
    LCD_IO_Init();//IO初始化-如果第一次初始化IO与第二次初始化IO不一样，需要先关闭时钟再初始化
    F1C100S_LCD_Config(width,height,buff1,buff2);//TCON DEBE 初始化
}
/*TCON中断*/
void TCON_ISR(void)
{
    //这里TCON_INT_REG0需要写0清除中断标志-参考数据手册183页
    C_BIT(TCON_Base_Address+0x04,15);//清除TCON0消隐中断

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

//以下是djygui要求的接口函数，许多情况下，大多数用空函数即可

//----位图中画像素-------------------------------------------------------------
//功能: 在位图中画一个像素，只有在bitmap的像素格式为cn_custom_pf时，才需要绘制。
//      如果显卡不打算支持自定义格式，本函数直接返回。
//参数: bitmap，目标位图
//      limit，限制矩形，只绘制在该矩形内部的部分
//      x、y，坐标
//      color，绘图用的颜色，CN_SYS_PF_ERGB8888格式
//      r2_code，二元光栅操作码
//返回: 无
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                         s32 x,s32 y,u32 color,u32 r2_code)
{
//  printf("__lcd_set_pixel_bm\r\n");
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
//    printf("__lcd_line_bm\r\n");
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
    else//圆角矩形
    {
        s32 OutConst, Sum, SumY;
        s32 move_x, x, move_y, flag, flag_x;
        s32 cx, cy, dy, dx, arc_ry, arc_rx, _max, min_y;
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
                if (move_y >= Focus->top && move_y < Focus->bottom)//上边弧start
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

                }//上边弧end
                move_y = cy + y + dy - flag;
                if (move_y < Focus->bottom&&move_y >= Focus->top)//下边弧start
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
                }//下边弧end
            }

        }
        //*********中间矩形部分start
        if (cy - dy <= Focus->top) y = Focus->top;//矩形高度
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

        }//*********中间矩形部分end
        return true;
    }//圆角矩形
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
                else if(src_bitmap->reversal == 2)  //用于特定用户180°旋转图形的操作
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
                else if(src_bitmap->reversal == 3)  //用于特定用户180°旋转图形的操作
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

//----screen中画像素-----------------------------------------------------------
//功能: 在screen中画一个像素.
//参数: x、y，像素坐标
//      color，颜色
//      r2_code，二元光栅操作码
//返回: true=成功绘制，false=没有绘制，或无需绘制
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

    return false;
}

//从内存缓冲区到screen位块传送，只支持块拷贝，不支持rop操作。
//镜像显示器的driver须提供这个函数
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
            struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    return false;
}

//从screen中取一像素，并转换成CN_SYS_PF_ERGB8888
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
//  printf("__lcd_get_pixel_screen\r\n");
    return GK_ConvertColorToRGB24(CN_SYS_PF_RGB565,
                                    u16g_FrameBuffer[x + y * CFG_LCD_XSIZE],0);
}

//把screen内矩形区域的内容复制到bitmap，调用前，先设置好dest的pf_type
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
//  printf("__lcd_get_rect_screen\r\n");
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
#pragma GCC diagnostic pop

//----初始化lcd设备------------------------------------------------------------
//功能: 如名
//参数: 无
//返回: 显示器资源指针
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

    tg_lcd_display.DisplayHeap = NULL;
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    GK_InstallDisplay(&tg_lcd_display,DisplayName);
    return (ptu32_t)&tg_lcd_display;
}



