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
// =============================================================================

// �ļ���     ��cpu_peri_lcd.c
// ģ������: MIMXRT1052���ڲ�lcd����
// ģ��汾:
// ������Ա:    chj
// ����ʱ��: 5/4.2018
// =============================================================================
#include "cpu_peri_lcd.h"
#include "fsl_elcdif.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "board.h"
#include "heap.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,\
//                                          const char* HeapName);
//    ModuleInstall_LCD(CFG_LCD_DISPLAY_NAME,CFG_LCD_HEAP_NAME);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip lcd"//CPU����LCD����������
//parent:"graphical kernel"   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                  //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable               //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                 //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                 //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","heap"//������������������������none����ʾ�������������
                                 //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//weakdependence:none            //��������������������������none����ʾ�������������
                                 //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                 //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                 //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_LCD == false )
//#warning  " cpu_onchip_lcd  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_LCD    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,32,512,

//%$#@enum,true,false,
//%$#@string,1,30,
#define CFG_LCD_DISPLAY_NAME      "LCD_DISPLAY"//"��ʾ������",
#define CFG_LCD_HEAP_NAME         "sys"//"��ʾʹ�õĶ�����",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

#define FRAME_BUFFER_ALIGN  64U

#define lcd_swap(a, b){a^=b; b^=a; a^=b;}

static struct LCD_ConFig lcd;                      //����LCD LTDC����Ҫ����
static struct DisplayObj lcd_display;

AT_NONCACHEABLE_SECTION_ALIGN( uint16_t s_frameBuffer[CN_LCD_YSIZE][CN_LCD_XSIZE], FRAME_BUFFER_ALIGN);

void delay_lcd(void)
{
    volatile uint32_t i = 0;
    for (i = 0; i < 16000000; ++i)
    {
        __asm("NOP"); /* delay */
    }
}
//-----------------------------------------------------------------------
//LCD��λ������Ƴ�ʼ��
//-----------------------------------------------------------------------
static void Lcd_Reset(void)
{
    gpio_pin_config_t config = {
        kGPIO_DigitalOutput, 0,
    };

    /* Reset the LCD. */
    GPIO_PinInit(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, &config);
    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);
    delay_lcd();
    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 0);
    delay_lcd();
    GPIO_PinWrite(LCD_DISP_GPIO, LCD_DISP_GPIO_PIN, 1);
    delay_lcd();
    delay_lcd();
    /* Backlight. */
    config.outputLogic = 1;
    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
    delay_lcd();
}

//-----------------------------------------------------------------------
//LCDʱ�ӳ�ʼ��
//-----------------------------------------------------------------------
static void Lcd_ClockInit(void)
{
    /*
     * The desired output frame rate is 60Hz. So the pixel clock frequency is:
     * (480 + 41 + 4 + 18) * (272 + 10 + 4 + 2) * 60 = 9.2M.
     * Here set the LCDIF pixel clock to 9.3M.
     */

    /*
     * Initialize the Video PLL.
     * Video PLL output clock is OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 93MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 31, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 4);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}
//-----------------------------------------------------------------------
//LCD��ʼ������
//������lcd���洢Ҫ���õĲ���
//-----------------------------------------------------------------------
void Lcd_Init( struct LCD_ConFig* plcd)
{
    elcdif_rgb_mode_config_t config;
    config.hsw = plcd->hsw;
    config.hfp = plcd->hfp;
    config.hbp = plcd->hbp;
    config.vsw = plcd->vsw;
    config.vfp = plcd->vfp;
    config.vbp = plcd->vbp;
    config.polarityFlags = plcd->HSPolarity | plcd->VSPolarity |
                                    plcd->DEPolarity | plcd->PCPolarity;
    config.panelHeight = plcd->height;
    config.panelWidth = plcd->width;
    config.bufferAddr = (u32)s_frameBuffer;
    config.pixelFormat = plcd->PixelFormat;

    switch(plcd->PixelFormat)
    {
        case kELCDIF_PixelFormatRGB565:
            config.dataBus = kELCDIF_DataBus16Bit;
            break;
        case kELCDIF_PixelFormatRGB666:
            config.dataBus = kELCDIF_DataBus18Bit;
            break;
        case kELCDIF_PixelFormatXRGB8888:
        case kELCDIF_PixelFormatRGB888:
            config.dataBus = kELCDIF_DataBus24Bit;
            break;
    }

    Lcd_ClockInit();
    Lcd_Reset();
    ELCDIF_RgbModeInit(LCDIF, &config);
    ELCDIF_RgbModeStart(LCDIF);
}

//-----------------------------------------------------------------------
//LTDC������
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//ע��:sx,ex,���ܴ���lcddev.width-1;sy,ey,���ܴ���lcddev.height-1!!!
//color:Ҫ������ɫ
//-----------------------------------------------------------------------
static bool_t Lcd_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u32 color)
{
    u32 i=0,j=0;
    u16* addr;
    u32 height,width;


    width=ex-sx+1;
    height=ey-sy+1;
    addr=(u16*)((u32)lcd.pFrameBuffe+lcd.pixsize*(lcd.width*sy+sx));
    if((ex+1>lcd.width)
            ||(ey+1>lcd.height))
        return false;

    for(i=0;i<width;i++)
    {
        for(j=0;j<height;j++)
        {
            *(addr+i*lcd.width+j) = (u16)color;
        }
    }
    return true;
}

//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
static bool_t __lcd_disp_ctrl(struct DisplayRsc *disp)
{
    return true;
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
//-----------------------------------------------------------------------------
static bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
                     s32 x,s32 y,u32 color,u32 r2_code)
{
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
static bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
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
static bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
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
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    return false;
}

//--------------bitmap��λ�鴫��-----------------------------------------------
//����������λͼ��λ�鴫��,������γߴ粻���,��Ҫʵ�������ѹ��
//ʲô�����Ӧ��ʵ�ֱ������Ĺ���,�μ����ṹǰ���ע��
//������ʾ������ʵ�ֱ�����,ֱ�ӷ���false����
//����:
// dst_bitmap: Ŀ��λͼ
// DstRect: Ŀ�����
// src_bitmap: Դλͼ
// SrcRect: Դ����
// RopCode: ��ʾЧ��,�μ� gkernel.h�ļ��� CN_ROP_ALPHA_SRC_MSK �峣����˵��
// HyalineColor: ͸��ɫ,�������RopCode���趨�� CN_ROP_KEYCOLOR_EN,��
//                  ������ָ��͸��ɫ,CN_SYS_PF_ERGB8888��ʽ
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------

bool_t __lcd_blt_bm_to_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *DstRect,
                          struct RectBitmap *src_bitmap,
                          struct Rectangle *SrcRect,
                          struct RopGroup RopCode,u32 transparentcolor)
{
    return false;
}

//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ������.
//����: x��y����������
//      color����ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=û�л��ƣ����������
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 rop2_code)
{

    u32 dest;
    u32 byteoffset;
    if(((x+1)>lcd.width)||((y+1)>lcd.height))
        return false;
    byteoffset = (y*lcd.width + x)*lcd.pixsize;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
    dest = (u32)lcd.pFrameBuffe[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);

    switch (lcd.pixsize)
    {
    case 2:
        *(u16 *)(&lcd.pFrameBuffe[byteoffset]) = (u16)color;
        break;
    case 3:
        lcd.pFrameBuffe[byteoffset] = (u8)color>>16;
        lcd.pFrameBuffe[byteoffset+1] = (u8)color>>8;
        lcd.pFrameBuffe[byteoffset+2] = (u8)color;
        break;
    default:            break;
    }
    return true;

}

//��screen�л�һ�����أ���frame buffer������£�������ʾgui����������������
//�������direct_screen==true������ܵ��ñ������������Ƿ���frame buffer��
//driver�������ṩ���ұ���ʵ�ֱ�����
//������ʾ������ʵ�ֱ������Ĺ���,����ֱ�ӷ���false
//����:
// PixelGroup: ������������ָ��.
// color: ��ɫֵ,CN_SYS_PF_ERGB8888��ʽ
// n: ����������
// r2_code: ��ʾЧ��,���gkernel.h�е�CN_R2_BLACK�峣������
bool_t __lcd_set_pixel_group_screen(struct PointCdn *PixelGroup,u32 color,u32 n, u32 r2_code)
{
    u32 dest,i;
    u32 offset;
    color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);

    for(i=0;i<n;i++)
    {
        offset = PixelGroup[i].y*CN_LCD_XSIZE + PixelGroup[i].x;
        dest = *(u16 *)&(lcd.pFrameBuffe[offset]);
        dest = GK_BlendRop2(dest,color,r2_code);

        *(u16 *)&(lcd.pFrameBuffe[offset]) = dest;
    }


    return true;
}

//----screen�л���---------------------------------------------------------------
//����: ��screen�л�һ������ֱ�ߣ������˵㣬ֻ������limit�޶��������ڵĲ��֡���
//      ��֧�ֻ��ߵ�Ӳ�����٣�ֱ�ӷ���false��
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    if(y1 == y2)    //��ˮƽ��
    {
        if(x1 > x2)
            lcd_swap(x1,x2);
        if((y1 < limit->top) || (y1 >= limit->bottom)
            ||(x2 <= limit->left) || (x1 >= limit->right))
            return false;         //����ֱ����limit֮��
        if(x1 < limit->left)
            x1 = limit->left;
        if(x2 > limit->right)
            x2 = limit->right;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return Lcd_Color_Fill( x1,y1,x2-1,y2+1,color);

    }else if(x1 == x2)   //�洹ֱ��
    {
        if(y1 > y2)
            lcd_swap(y1,y2);
        if((x1 < limit->left) || (x1 >= limit->right)
            ||(y2 <= limit->top) || (y1 >= limit->bottom))
            return false;         //����ֱ����limit֮��
        if(y1 < limit->top)
            y1 = limit->top;
        if(y2 > limit->bottom)
            y2 = limit->bottom;
        color = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,color);
        return Lcd_Color_Fill( x1,y1,x2+1,y2-1,color);
    }
    return false;
}

//----screen�л���(���˵�)------------------------------------------------------
//����: ��screen�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ��֡�
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ���
//      x1��y1��x2��y2������յ�����
//      color����ͼ�õ���ɫ��cn_sys_pf_e8r8g8b8��ʽ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code����
//-----------------------------------------------------------------------------
static bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{

    return false;
}

//----screen��������-----------------------------------------------------------
//����: ��screen�еľ�����color��ɫ��䣬color�ĸ�ʽ��cn_sys_pf_e8r8g8b8��
//����: dst_rect�������ľ���
//      color�������ɫ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    u32 pen;
    bool_t flag;
    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = GK_ConvertRGB24ToPF(lcd.LcdPixelFormat,Color0);
    flag=Lcd_Color_Fill(Focus->left,Focus->top,Focus->right-1,Focus->bottom-1,pen);
    return flag;
}

//----bitmap��screenλ�鴫��---------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ������
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����(��Դλͼ���Ͻ�����Ϊԭ��)
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
static bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
        struct RectBitmap *src_bitmap,s32 x,s32 y)
{
    return false;
}

//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8��cn_sys_pf_a8r8g8b8��ʽ��
//����: x��y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 e,r,g,b,color;
    u32 byteoffset;
    byteoffset = (y*lcd.width + x)*lcd.pixsize;

    switch (lcd.pixsize)
    {
    case 2:
        color = *(u16 *)(&lcd.pFrameBuffe[byteoffset]);
        r = (color>>11) &0x1f;
        g = (color>>5) & 0x3f;
        b = color & 0x1f;
        break;
    case 3:
        color = lcd.pFrameBuffe[byteoffset]|\
        lcd.pFrameBuffe[byteoffset+1] <<8|\
        lcd.pFrameBuffe[byteoffset+2] <<16;
        b = lcd.pFrameBuffe[byteoffset];
        g = lcd.pFrameBuffe[byteoffset+1];
        r = lcd.pFrameBuffe[byteoffset+2];
        break;
    default:            break;
    }
    e = 0;
    return ((e<<28)|(r<<20) | (g<<12) | (b<<4));
}

//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ�С�
//����: rect������ȡ�ľ���
//      dest��������ε�λͼ���䳤�������rect��ͬ
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ
//-----------------------------------------------------------------------------
static bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}

//��ʼ��LCD���ò���
static bool_t Lcd_Config(struct LCD_ConFig *lcd)
{
    lcd->hsw              =LCD_HSW;
    lcd->hbp              =LCD_HBP;
    lcd->hfp              =LCD_HFP;
    lcd->vsw              =LCD_VSW;
    lcd->vbp              =LCD_VBP;
    lcd->vfp              =LCD_VFP;

    switch(LCD_RGB_MODE)
    {
        case CN_SYS_PF_RGB565:
            lcd->LcdPixelFormat   =CN_SYS_PF_RGB565;
            lcd->PixelFormat      =kELCDIF_PixelFormatRGB565;
            lcd->pixsize          =2;
            break;
        case CN_SYS_PF_RGB888:
            lcd->LcdPixelFormat   =CN_SYS_PF_RGB888;
            lcd->PixelFormat      =kELCDIF_PixelFormatRGB888;
            lcd->pixsize          =3;
            break;
        case CN_SYS_PF_ERGB8888:
            lcd->LcdPixelFormat   =CN_SYS_PF_RGB888;
            lcd->PixelFormat      =kELCDIF_PixelFormatXRGB8888;
            lcd->pixsize          =3;
            break;
        default:
            return false;
    }

    lcd->HSPolarity   =kELCDIF_HsyncActiveLow;
    lcd->VSPolarity   =kELCDIF_VsyncActiveLow;
    lcd->DEPolarity   =kELCDIF_DataEnableActiveHigh;
    lcd->PCPolarity   =kELCDIF_DriveDataOnRisingClkEdge;

    lcd->width = CN_LCD_XSIZE;
    lcd->height = CN_LCD_YSIZE;
    return true;
}

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ��ʼ��lcd�豸��
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
struct DisplayRsc* ModuleInstall_LCD(const char *DisplayName,\
                                            const char* HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;

    struct HeapCB *heap;
    Lcd_Config(&lcd);
    heap =Heap_FindHeap(HeapName);
    if(heap==NULL)
        return NULL;

    FrameBitmap.bm_bits=(u8 *)s_frameBuffer;
    FrameBitmap.width = CN_LCD_XSIZE;
    FrameBitmap.height = CN_LCD_YSIZE;
    FrameBitmap.PixelFormat = lcd.LcdPixelFormat;
    FrameBitmap.linebytes = CN_LCD_XSIZE*lcd.pixsize;
    FrameBitmap.ExColor = 0xffffffff;
    frame_win.wm_bitmap = &FrameBitmap;


    lcd_display.frame_buffer =&frame_win;
    lcd_display.framebuf_direct = true;

    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    lcd_display.width = CN_LCD_XSIZE;
    lcd_display.height = CN_LCD_YSIZE;
    lcd_display.pixel_format = lcd.LcdPixelFormat;

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

    Lcd_Init(&lcd);
    //LTDC_Init(pLTDCBufferFG1);

    if(GK_InstallDisplay(&lcd_display,DisplayName))
        return &lcd_display;
    return NULL;
}

