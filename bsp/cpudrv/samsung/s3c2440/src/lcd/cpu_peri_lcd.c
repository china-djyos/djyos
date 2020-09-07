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
#include "stdint.h"
#include "stdlib.h"
#include "stddef.h"
#include "string.h"

#include "board-config.h"
#include "gkernel.h"
#include <gui/gk_display.h>
#include "cpu_peri.h"
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

#define M5D(n)          ((n) & 0x1fffff)    //��õ�21λ


////////////////////////////////////////////////////////

static struct DisplayObj lcd_display;

static  u16 *pFrameBufferFG;    //ǰ̨��ʾ������.

#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

static struct GkWinObj   frame_win;
static struct RectBitmap frame_bm;

static u16 lcd_buffer_bk[CN_LCD_XSIZE*CN_LCD_YSIZE];    //��̨��ʾ������.

#endif

//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ��ʼ��lcdӲ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����
//----------------------------------------------------------------------------
static void __lcd_hard_init(u32 lcd_buffer_addr)
{

    struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
    struct LCD_REG *lcd =LCD_REG_BASE;

    //�˿� C ������ʹ�ܼĴ��� ,0,ʹ�ܸ����������ܵ���Ӧ�˿�����
    gpio->GPCUP = 0x00000000;
    //GPC4 ,VM;GPC3 VFRAME;GPC2  VLINE;GPC1 VLINE;GPC0 �����
    //Initialize VD[7:0]��16-31λ,VD[23:0] ,��LCD ������������˿ڣ�
    gpio->GPCCON = 0xaaaa02a9;
    gpio->GPDUP = 0x00000000;
    gpio->GPDCON = 0xaaaaaaaa; //Initialize VD[23:8]��0-31λ��

    lcd->LCDCON1 = (LCDCON1_CLKVAL<<8)|(LCDCON1_MMODE<<7)|(3<<5)
                            |(12<<1)|0;
    lcd->LCDCON2 = (LCDCON2_VBPD<<24)|((CN_LCD_XSIZE-1)<<14)|(LCDCON2_VFPD<<6)
                            |(LCDCON2_VSPW);
    lcd->LCDCON3 = (LCDCON3_HBPD<<19)|((CN_LCD_YSIZE-1)<<8)|(LCDCON3_HFPD);
    lcd->LCDCON4 = (LCDCON4_MVAL<<8)|(LCDCON4_HSPW);
    lcd->LCDCON5 = (1<<11) | (1<<10) | (1<<9) | (1<<8) | (0<<7)
                 | (0<<6) | (1<<3)  |(LCDCON5_BSWP<<1) | (LCDCON5_HWSWP);
    lcd->LCDSADDR1 = (((u32)lcd_buffer_addr>>22)<<21)
                            |M5D((uint32_t)lcd_buffer_addr>>1);
    lcd->LCDSADDR2 = M5D(((u32)lcd_buffer_addr
                            +(CN_LCD_XSIZE*CN_LCD_YSIZE*2))>>1);
    lcd->LCDSADDR3 = (((CN_LCD_XSIZE-CN_LCD_XSIZE)/1)<<11)
                                |(CN_LCD_XSIZE/1);
    lcd->LCDINTMSK |= (3);         //LCD�ж����μĴ���
    lcd->TCONSEL &= (~7) ;       //TCON���ƼĴ���
    lcd->TPAL = 0;                 //��ʱ��ɫ��Ĵ���

    lcd->LCDCON1 |= 1;          //������Ƶ����� LCD�����ź�

    //LCD PWR
    gpio->GPGUP  = (gpio->GPGUP & (~(1<<4))) | (1<<4); //����ʹ��
    gpio->GPGCON = (gpio->GPGCON & (~(3<<8))) | (3<<8);
    gpio->GPGDAT = gpio->GPGDAT | (1<<4) ;

    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<3))) | (1<<3);
    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<5))) |(1<<5);


}

static void __lcd_backlight_onoff(bool_t onoff)
{

}

//----LCD�����ź�------------------------------------------------------------
//����: LCD ��Ƶ������߼�ʹ��/��ֹ��1=������Ƶ����� LCD�����źţ�
//      0=��ֹ��Ƶ����� LCD�����źš�
//����: onoff�������ź�
//����: ��
//-----------------------------------------------------------------------------
static void __lcd_envid_of(int onoff)
{
    struct LCD_REG *lcd =LCD_REG_BASE;

    if(onoff == 1)
        lcd->LCDCON1 |= 1;   //������Ƶ����� LCD�����ź�
    else                        //��ֹ��Ƶ����� LCD�����ź�
        lcd->LCDCON1 = lcd->LCDCON1 & 0x3fffe;
}
//----����LCD��Դ--------------------------------------------------------------
//����: LCD��Դ��������ʹ�ܡ�
//����: invpwren������PWREN�źż���
//      pwren��LCD_PWREN����ź�ʹ��/��ֹ
//����: ��
//-----------------------------------------------------------------------------
static void __lcd_power_enable(int invpwren,int pwren)
{
    struct gpio_reg *gpio =(struct gpio_reg*)GPIO_REG_BASE;
    struct LCD_REG *lcd =LCD_REG_BASE;

    gpio->GPGUP  = (gpio->GPGUP & (~(1<<4))) | (1<<4); //����ʹ��
    gpio->GPGCON = (gpio->GPGCON & (~(3<<8))) | (3<<8);
    gpio->GPGDAT = gpio->GPGDAT | (1<<4) ;

    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<3))) | (pwren<<3);
    lcd->LCDCON5 = (lcd->LCDCON5 & (~(1<<5))) |(invpwren<<5);
}
//----������ʾ��---------------------------------------------------------------
//����: ������driver�ṩ�̱�д�ģ�����������ʾ�����ֲ����ĺ���������Ӧ�ó����
//      ���룬�������豸���̶��塣
//����: disp����ʾ��ָ��
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_disp_ctrl(struct DisplayObj *disp)
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
bool_t __lcd_set_pixel_bm(struct RectBitmap *bitmap,
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
bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
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
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ��������-----------------------------------------------------------
//����: ��λͼ�еľ�����color��ɫ��䡣
//����: bitmap��Ŀ��λͼ
//      dst_rect�������ľ���
//      color�������ɫ��cn_sys_pf_e8r8g8b8��ʽ
//����: true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    s32 x, y;
    u16 pixel;
    u16 *dst,*p;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;

    if(dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
        return false;

    pixel = GK_ConvertRGB24ToPF(CN_SYS_PF_RGB565,Color0);

    dst = (u16*)((u32)dst_bitmap->bm_bits
                      + Focus->top * dst_bitmap->linebytes);
    dst += Focus->left;

    for(y = Focus->top; y < Focus->bottom; y++)
    {
        p =dst;
        for(x=Focus->left; x<Focus->right;x++)
        {
            *p++ =pixel;
        }
        dst += dst_bitmap->linebytes >> 1;
    }
    return true;
}
//----bitmap��λ�鴫��---------------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_bitmap��Ŀ��λͼ
//      dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����
//      pat��patternָ��
//      bitmsk��rop4������Ҫ������
//      rop_code����Ԫ��դ������
//      transparentcolor��͸��ɫ
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_blt_bm_to_bm(struct RectBitmap *dst_bitmap,
                        struct Rectangle *DstRect,
                        struct RectBitmap *src_bitmap,
                        struct Rectangle *SrcRect,
                        struct RopGroup RopCode,u32 HyalineColor)
{
    struct RopGroup Rop = { 0, 0, 0, CN_R2_COPYPEN, 0, 0, 0  };
    u16 *src_offset,*dst_offset;    //Դλͼ���󻺳������ܲ�����!!!
    u32 y;
    if((dst_bitmap->PixelFormat != CN_SYS_PF_RGB565)
      ||(src_bitmap->PixelFormat != CN_SYS_PF_RGB565)
      ||(memcmp(&RopCode, &Rop ,sizeof(struct RopGroup))!=0))
    {
        return false;
    }
    else
    {
        if((ptu32_t)src_bitmap->bm_bits & 1)    //Դλͼ�������Ƕ���ģ��
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
    byteoffset = y*CN_LCD_XSIZE + x;

    color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);

    dest = (u32)pFrameBufferFG[byteoffset];
    dest = GK_BlendRop2(dest,color,rop2_code);
    pFrameBufferFG[byteoffset] = dest;
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
    color = GK_ConvertRGB24ToPF(CN_LCD_PIXEL_FORMAT,color);

    for(i=0;i<n;i++)
    {
        offset = PixelGroup[i].y*CN_LCD_XSIZE + PixelGroup[i].x;
        dest = (u32)pFrameBufferFG[offset];
        dest = GK_BlendRop2(dest,color,r2_code);

        pFrameBufferFG[offset] = dest;
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
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
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
    int x,y;
    u32 pen;
    u16 *p;
    return false;

    if(Mode != CN_FILLRECT_MODE_N)
        return false;
    pen = __GK_ConvertColorToPF(CN_LCD_PIXEL_FORMAT,Color0);

    for(y=Focus->top;y<Focus->bottom;y++)
    {
        p =&pFrameBufferFG[y*CN_LCD_XSIZE];

        for(x=Focus->left;x<Focus->right;x++)
        {
            *p++ =pen;
        }
    }
    return true;
}

//----bitmap��screenλ�鴫��---------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ��
//����: dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc��ysrc��Դλͼ�б����͵��������Ͻ�����(��Դλͼ���Ͻ�����Ϊԭ��)
//����: true=�ɹ���false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
                struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    u32 size,double_width_src;
    u32 byteoffset_rect,byteoffset_bitmap;
    s32 y;

    if(src_bitmap->PixelFormat!=CN_LCD_PIXEL_FORMAT)
    {   //���Դλͼ��LCD��ʽ����ͬ
        return false;
    }

    double_width_src = src_bitmap->width*2;     //Դλͼһ����ռ���ֽ���
    //ÿһ��Ҫ�����͵��ֽ���
    size = (dst_rect->right-dst_rect->left)*(CN_LCD_PIXEL_FORMAT>>8)/8;
    //Դλͼ���Ͻ�������ֽ�ƫ����
    byteoffset_bitmap = ysrc*double_width_src + xsrc*2;
    //Ŀ��������Ͻ������˫�ֽ�ƫ��������Ϊpg_video_bufΪu16��ָ��
    //��Ϊ�Ǵ��͵�screen�ϣ�����ÿ��˫�ֽ���ȡcn_lcd_xsize
    byteoffset_rect = dst_rect->top*CN_LCD_XSIZE + dst_rect->left;

    //bitmap��screenλ�鴫��
    for(y = dst_rect->top;y < dst_rect->bottom;y++)
    {
        memcpy(&(pFrameBufferFG[byteoffset_rect]),
                    &src_bitmap->bm_bits[byteoffset_bitmap],size);
        byteoffset_bitmap += double_width_src;
        byteoffset_rect += CN_LCD_XSIZE;
    }
    return true;
}
//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8��cn_sys_pf_a8r8g8b8��ʽ��
//����: x��y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    u32 r,g,b,color,offset;

    offset = (u32)pFrameBufferFG;
    offset += y*CN_LCD_XSIZE*2;
    offset += x*2;
    color = (u32)(*(u16*)offset);
    r = (color>>11) &0x1f;
    g = (color>>5) & 0x3f;
    b = color & 0x1f;
    return (r<<19) | (g<<10) | (b<<3);
}

//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ�С�
//����: rect������ȡ�ľ���
//      dest��������ε�λͼ���䳤�������rect��ͬ
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
}



//----��ʼ��lcd�豸------------------------------------------------------------
//����: ��ʼ��lcd�豸��
//����: ��
//����: ��ʾ����Դָ��
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


    pFrameBufferFG =M_MallocHeap(CN_LCD_XSIZE*CN_LCD_YSIZE*2,heap,0);

    __lcd_hard_init((u32)pFrameBufferFG);

    //�����ʼ��frame_buffer��desktop��z_topmost������Ա
#ifdef  CN_LCD_DRV_FRAME_BUFFER_EN

    frame_bm.PixelFormat = CN_LCD_PIXEL_FORMAT;
    frame_bm.width = CN_LCD_XSIZE;
    frame_bm.height = CN_LCD_YSIZE;
    frame_bm.linebytes = CN_LCD_XSIZE*2;
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
    lcd_display.width = CN_LCD_XSIZE;
    lcd_display.height = CN_LCD_YSIZE;
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






