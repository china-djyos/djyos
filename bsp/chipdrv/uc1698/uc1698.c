//----------------------------------------------------
//Copyright (C), 2004-2011,  ������.
//��Ȩ���� (C), 2004-2011,   ������.
//����ģ��:ʱ��ģ��
//����:  ������
//�汾��V1.0.0
//�ļ�����: windows����ģ��Һ������
//����˵��:
//�޶���ʷ:
// 1. ����: 2012-10-09
//   ����:  ������
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include <string.h>
#include <stdlib.h>
#include "gkernel.h"
#include "cpu_peri.h"
#include "os.h"
#include "board-config.h"
#include <gui/gk_display.h>

//����ϵ˵��,�������Դ�ƫ�Ƶ�ַ���������ص������ӳ���ϵ,ע�ⵥɫ�ͻҶ���ʾ��
//ÿ�ֽڿ��ܰ�����ֹһ�����ص�,ͼ��û�м�������.
//��ͬ��ϵͳ�в�ͬ�Ķ�Ӧ��ϵ,���ֱ仯Ӱ�� ������ʾ�����ı�д����.
//Ӧ�ó����Լ�gui�������������������ӳ���ϵ.
//�������,���������������Ļֱ��ӳ��ķ�����ʾbmpͼ��,��Ҫ�˽�����ӳ���ϵ.
//----------------------------------------------------->x   ���Ͻ�
// |0x0000,0x0001,0x0002--------------------------xmax-1
// |xmax,xmax+1,xmax+2---------------------------2*max-1
// |----------------------------------------------------
// |----------------------------------------------------
// |----------------------------------------------------
// |(ymax-1)*xmax,(ymax-1)*xmax+1------------ymax*xmax-1    ���½�
//\/
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern ptu32_t ModuleInstall_UC1698(void);
//  ModuleInstall_UC1698();
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"uc1698"      //CPU��������������
//parent:"touch"     //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                        //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                     //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                       //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium                     //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                       //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"graphical kernel","touch"//������������������������none����ʾ�������������
                                       //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//weakdependence:"gdd"                 //��������������������������none����ʾ�������������
                                       //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                       //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                       //�������������⣬�������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_UC1698 == false )
//#warning  " uc1698  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_UC1698    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@num,0,65536,
#define CFG_LCD_XSIZE   240             //"LCD���",
#define CFG_LCD_YSIZE   128             //"LCD�߶�",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_DISPLAY_NAME               "UC1698"   //"��ʾ������",
//%$#select,        ***���г���ѡ����ѡ�����ɸ�����ɺ�
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

struct DisplayObj tg_lcd_display;
u16 *pg_frame_buffer;
u8 *volatile CmdPort = (u8*)0x60000000;
u8 *volatile DataPort = (u8*)0x60000001;
u8 ContrastLevel;
#define CN_TONTRAST_LEVEL_DEFAULT   0x26


#define LCD_STATUS_GE_BIT     1<<7
#define LCD_STATUS_MX_BIT     1<<6
#define LCD_STATUS_MY_BIT     1<<5
#define LCD_STATUS_WA_BIT     1<<4
#define LCD_STATUS_DE_BIT     1<<5
#define LCD_STATUS_WS_BIT     1<<6
#define LCD_STATUS_MD_BIT     1<<1
#define LCD_STATUS_MS_BIT     1<<0

extern const Pin LCD_BLK[1];
extern const Pin LCD_RST[1];



u8 LCD_GetStatus(void)
{
    u8 s0,s1,s2;
    s0 = *CmdPort;
    s1 = *CmdPort;
    s2 = *CmdPort;
    return s0;
}


//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����.
//----------------------------------------------------------------------------
void InitUC1698(void)
{
    PatchF7Bug_ClearA0;
    ContrastLevel = CN_TONTRAST_LEVEL_DEFAULT;
    *CmdPort = 0xE2;                    // �����¶Ȳ���ϵ��-0.05%/C
    DJY_EventDelay(100*mS);
    *CmdPort=0x31;                     //set advanced program control
    *CmdPort=0x08;
    *CmdPort=0x2b;                     //internal power control  (13nF<LCD<=22nF)
    *CmdPort=0x25;                     //set TC=-0.15%
    *CmdPort=0xa0;                     //set line rate (00b:8.5Klps)
    *CmdPort=0xeb;                    //0XEA//set bias     ��Ϊ1/12ʱ��VLCDwr_command(0xeB);
    *CmdPort=0xf1;
    *CmdPort=0x7f;
    *CmdPort=0x81;
    *CmdPort=0x26;
    *CmdPort=0xc4;
    *CmdPort=0x89;
    *CmdPort=0xd1;
    *CmdPort=0xd5;
    *CmdPort=0xc8;
    *CmdPort=0x18;
    *CmdPort=0xad;

//        *CmdPort = 0x25;                            // �����¶Ȳ���ϵ��-0.05%/C
//        *CmdPort = 0x2b;                            // �ڲ�DC-DC
//        *CmdPort = 0xc4;                            // LCDӳ��MY=1��MX=0��LC0=0
//        *CmdPort = 0xa3;                            // ������ɨ��Ƶ�� FR=263Hz
//        *CmdPort = 0xd1;                            // ��ɫ���ݸ�ʽR-G-B
//        *CmdPort = 0xd5;                            // ��������λΪ12λRRRR-GGGG-BBBB
//        *CmdPort = 0xc8; *CmdPort = 0x18;           // ����M�ź�Ϊ֡��ת
//        *CmdPort = 0xeb;                            // ����ƫѹ��1/12
//        *CmdPort = 0xa7;                            // ������ʾ
//        *CmdPort = 0xa4;                            // ������ʾ
//        *CmdPort = 0x81; *CmdPort = ContrastLevel;  // ���öԱȶ�  ��Ӧ��ɫͼ��
//        *CmdPort = 0xdf;                            // ����ɨ��ģʽ ����FRC
//        *CmdPort = 0xad;                            // ����ʾ  �����Ҷ���ʾ





    *CmdPort = 0x70;
    *CmdPort = 0x60;
    *CmdPort = 0x11;
    *CmdPort = 0x08;
    *CmdPort = 0xf4;
    *CmdPort = 0x18;
    *CmdPort = 0xf6;
    *CmdPort = 0x67;
    *CmdPort = 0xf5;
    *CmdPort = 0x00;
    *CmdPort = 0xf7;
    *CmdPort = 0x7f;
    *CmdPort = 0xf8;




//    *CmdPort = 0x25;                            // �����¶Ȳ���ϵ��-0.05%/C
//    *CmdPort = 0x2b;                            // �ڲ�DC-DC
//    *CmdPort = 0xc4;                            // LCDӳ��MY=1��MX=0��LC0=0
//    *CmdPort = 0xa3;                            // ������ɨ��Ƶ�� FR=263Hz
//    *CmdPort = 0xd1;                            // ��ɫ���ݸ�ʽR-G-B
//    *CmdPort = 0xd5;                            // ��������λΪ12λRRRR-GGGG-BBBB
//    *CmdPort = 0xc8; *CmdPort = 0x18;           // ����M�ź�Ϊ֡��ת
//    *CmdPort = 0xeb;                            // ����ƫѹ��1/12
//    *CmdPort = 0xa7;                            // ������ʾ
//    *CmdPort = 0xa4;                            // ������ʾ
//    *CmdPort = 0x81; *CmdPort = ContrastLevel;  // ���öԱȶ�  ��Ӧ��ɫͼ��
//    *CmdPort = 0xdf;                            // ����ɨ��ģʽ ����FRC
//    *CmdPort = 0xad;                            // ����ʾ  �����Ҷ���ʾ
//    *CmdPort = 0xf4; *CmdPort = 0x00;           // ���ò���������߽�
//    *CmdPort = 0xf5; *CmdPort = 0x00;           // ���ò��������ϱ߽�
//    *CmdPort = 0xf6; *CmdPort = 0x4f;           // ���ò��������ұ߽�
//    *CmdPort = 0xf7; *CmdPort = 0x7f;           // ���ò��������±߽�
//    *CmdPort = 0xf8;                            // ���ô��ڲ���ʹ��
//    *CmdPort = 0x00; *CmdPort = 0x10;           // ������ʼ�е�ַ
//    *CmdPort = 0x60; *CmdPort = 0x70;           // ������ʼ�е�ַ

    DJY_EventDelay(50*mS);
}


void RAM_ADDRESS(void)
{
    *CmdPort=0x70;  //set row MSB address
    *CmdPort=0x60;  //set row LSB address
    *CmdPort=0x11;  //set column MSB address
    *CmdPort=0x08;  //set column LSB address
    *CmdPort=0xf4;  //set oolumn start address
    *CmdPort=0x18;  //start address=0
    *CmdPort=0xf6;  //set column end address
    *CmdPort=0x67;  //(0x4f+1)*3=240
    *CmdPort=0xf5;  //set row start address
    *CmdPort=0x00;  //start address=0x00
    *CmdPort=0xf7;  //set row end address
    *CmdPort=0x7f;  //row end address=7f
    *CmdPort=0xf8;  //inside mode
}


//----LCD�������------------------------------------------------------------
//����: ��
//����: OnOff��true=������false=Ϩ��
//����: ��
//-----------------------------------------------------------------------------
void LCD_BackLight(bool_t OnOff)
{
    if(OnOff)
        PIO_Clear(LCD_BLK);
    else
        PIO_Set(LCD_BLK);
}

//----LCD��λ------------------------------------------------------------------
//����: ��
//����: OnOff��true=������false=Ϩ��
//����: ��
//-----------------------------------------------------------------------------
void LCD_Reset(void)
{
     PIO_Clear(LCD_RST);
     DJY_EventDelay(100*mS);
     PIO_Set(LCD_RST);
     DJY_EventDelay(500*mS);
}


//----LCD��ʾʹ��------------------------------------------------------------
//����: ��
//����: OnOff��true=ʹ����ʾ��false=�ر���ʾ
//����: ��
//-----------------------------------------------------------------------------
void __lcd_envid_of(bool_t OnOff)
{
    GPIO_SettoLow(GPIO_F,1);
    if(OnOff)
        *CmdPort = 0xaf;                // ����ʾ  �����Ҷ���ʾ
    else
        *CmdPort = 0xae;                // ����ʾ  �����Ҷ���ʾ
}

//----����LCD��Դ--------------------------------------------------------------
//����: LCD ��Դ��������ʹ��
//����: OnOff��true=��������Դ��false=�رյ�Դ
//����: ��
//-----------------------------------------------------------------------------
void __lcd_power_enable(bool_t OnOff)
{
    GPIO_SettoLow(GPIO_F,1);
    if(OnOff)
        *CmdPort = 0x28;                // ����ʾ  �����Ҷ���ʾ
    else
        *CmdPort = 0x2b;                // ����ʾ  �����Ҷ���ʾ
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
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ��֡�
//      x��y������
//      color����ͼ�õ���ɫ
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
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ��֡�
//      x1,y1,x2,y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code���ߡ�
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ�л���(���˵�)-------------------------------------------------------
//����: ��λͼ�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ���
//����: bitmap��Ŀ��λͼ
//      limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ��֡�
//      x1,y1,x2,y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code���ߡ�
//-----------------------------------------------------------------------------
bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----λͼ��������-----------------------------------------------------------
//����: ��λͼ�еľ�����color��ɫ���
//����: bitmap��Ŀ��λͼ
//      dst_rect,�����ľ���
//      color�������ɫ
//����:  true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_bm(struct RectBitmap *dst_bitmap,
                          struct Rectangle *Target,
                          struct Rectangle *Focus,
                          u32 Color0,u32 Color1,u32 Mode)
{
    return false;
}
//----bitmap��λ�鴫��---------------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ
//����: dst_bitmap��Ŀ��λͼ
//      dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc,ysrc��Դλͼ�б����͵��������Ͻ�����
//      pat��patternָ��
//      mask��rop4������Ҫ������
//      rop_code����Ԫ��դ������
//      transparentcolor���ؼ���ɫ
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_blt_bm_to_bm( struct RectBitmap *dst_bitmap,
                            struct Rectangle *DstRect,
                            struct RectBitmap *src_bitmap,
                            struct Rectangle *SrcRect,
                            struct RopGroup RopCode, u32 HyalineColor)
{
    return false;
}
//----��ȡλͼ�е�����---------------------------------------------------------
//����: ȡλͼ�е�һ�����أ�ֻ����bitmap�����ظ�ʽΪcn_custom_pfʱ������Ҫ��ȡ��
//      ����Կ�������֧���Զ����ʽ��������ֱ�ӷ���false��
//����: bitmap��Ŀ��λͼ
//      x��y������
//����:  true=�ɹ���ȡ��false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_get_pixel_bm(struct RectBitmap *bitmap,s32 x,s32 y)
{
    return false;
}
//----screen�л�����-----------------------------------------------------------
//����: ��screen�л�һ�����أ���frame buffer��driver��ֱ�ӷ��ؼ���
//����: x,y,��������
//      color����ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=û�л��ƣ����������
//-----------------------------------------------------------------------------
bool_t __lcd_set_pixel_screen(s32 x,s32 y,u32 color,u32 r2_code)
{
    return false;
}
//----screen�л���---------------------------------------------------------------
//����: ��screen�л�һ������ֱ�ߣ������˵㣬ֻ������limit�޶��������ڵĲ��֡���
//      ��֧�ֻ��ߵ�Ӳ�����٣�ֱ�ӷ���false��
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ��֡�
//      x1,y1,x2,y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code���ߡ�
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen(struct Rectangle *limit,
                    s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen�л���(���˵�)------------------------------------------------------
//����: ��screen�л�һ��ֱ�ߣ�ֻ������limit�޶��������ڵĲ���
//����: limit�����ƾ��Σ�ֻ�����ڸþ����ڲ��Ĳ��֡�
//      x1,y1,x2,y2������յ�����
//      color����ͼ�õ���ɫ
//      r2_code����Ԫ��դ������
//����: true=�ɹ����ƣ�false=ʧ�ܣ���Ӳ�����ٻ�֧�ְ�r2_code���ߡ�
//-----------------------------------------------------------------------------
bool_t __lcd_line_screen_ie(struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code)
{
    return false;
}
//----screen��������-----------------------------------------------------------
//����: ��screen�еľ�����color��ɫ��䣬color�ĸ�ʽ��cn_sys_pf_e8r8g8b8
//����: dst_rect,�����ľ���
//      color�������ɫ
//����:  true=�ɹ����ƣ�false=ʧ��
//-----------------------------------------------------------------------------
bool_t __lcd_fill_rect_screen(struct Rectangle *Target,
                              struct Rectangle *Focus,
                              u32 Color0,u32 Color1,u32 Mode)
{
    return false;
}

//----bitmap��screenλ�鴫��---------------------------------------------------
//����: ��һ��λͼ�е�һ�����������͵���һ����������λͼ�����ظ�ʽ������ͬ��
//      Դ��Ŀ��λͼ������ͬ
//����: dst_rect��Ŀ�����
//      src_bitmap��Դλͼ
//      xsrc,ysrc��Դλͼ�б����͵��������Ͻ�����
//����: true=�ɹ�,false=ʧ��
//-----------------------------------------------------------------------------
#pragma GCC push_options
#pragma GCC optimize("O0")
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
                struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
    s32 xstart,xend;
    s32 realxsrc;
    s32 loopx,loopy;
    u32 offset;
    u8 c,s0,s1,s2;

    //����LCDģ���Ҫ��x��start��end��Ҫ����Ϊ6�ı���
    xstart = (dst_rect->left/6)*6;
    xend = ((dst_rect->right-1)/6)*6;
    realxsrc = xsrc - (dst_rect->left-xstart);

    PatchF7Bug_ClearA0;

    *CmdPort=0x70;  //set row MSB address
    *CmdPort=0x60;  //set row LSB address
    *CmdPort=0x11;  //set column MSB address
    *CmdPort=0x08;  //set column LSB address

    *CmdPort = 0xf4; *CmdPort =0x18+xstart/3;           // ���ò���������߽�
    *CmdPort = 0xf5; *CmdPort = (u8)dst_rect->top;  // ���ò��������ϱ߽�
    *CmdPort = 0xf6; *CmdPort = 0x18+xend/3+1;             // ���ò��������ұ߽�
    *CmdPort = 0xf7; *CmdPort = (u8)(dst_rect->bottom-1);  // ���ò��������±߽�
    *CmdPort = 0xf8;                                // ���ô��ڲ���ʹ��
    *CmdPort = (0x18+xstart/3) & 0x0f;
        *CmdPort = 0x10|((0x18+xstart/3) >> 4);            // ������ʼ�е�ַ

    *CmdPort = 0x60 + ((u8)dst_rect->top & 0x0f);
        *CmdPort = 0x70 + ((u8)(dst_rect->top)>> 4);// ������ʼ�е�ַ

    if(realxsrc%2 == 0)
    {
        offset = ysrc * src_bitmap->linebytes + realxsrc/2;
        DJY_DelayUs(1);
        PatchF7Bug_SetA0;
        for(loopy = 0; loopy < dst_rect->bottom - dst_rect->top; loopy++)
        {
            for(loopx = 0; loopx < (xend-xstart+6)/2; loopx++ )
            {
                c = src_bitmap->bm_bits[offset+loopx];
                *DataPort = c;
            }
            offset += src_bitmap->linebytes;
        }
        PatchF7Bug_ClearA0;
        s0 = *CmdPort;
        s1 = *CmdPort;
        s2 = *CmdPort;
        while(s0 != 0xba)
        {
//            *CmdPort = 0xE2;                            // �����¶Ȳ���ϵ��-0.05%/C
//            *CmdPort = 0x25;                            // �����¶Ȳ���ϵ��-0.05%/C
//            *CmdPort = 0x2b;                            // �ڲ�DC-DC
//            *CmdPort = 0xc4;                            // LCDӳ��MY=1��MX=0��LC0=0
//            *CmdPort = 0xa3;                            // ������ɨ��Ƶ�� FR=263Hz
//            *CmdPort = 0xd1;                            // ��ɫ���ݸ�ʽR-G-B
//            *CmdPort = 0xd5;                            // ��������λΪ12λRRRR-GGGG-BBBB
//            *CmdPort = 0xc8; *CmdPort = 0x18;           // ����M�ź�Ϊ֡��ת
//            *CmdPort = 0xeb;                            // ����ƫѹ��1/12
//            *CmdPort = 0xa7;                            // ������ʾ
//            *CmdPort = 0xa4;                            // ������ʾ
//            *CmdPort = 0x81; *CmdPort = ContrastLevel;  // ���öԱȶ�  ��Ӧ��ɫͼ��
//            *CmdPort = 0xdf;                            // ����ɨ��ģʽ ����FRC
//            *CmdPort = 0xad;






            *CmdPort = 0xE2;                    // �����¶Ȳ���ϵ��-0.05%/C
            DJY_EventDelay(100*mS);
            *CmdPort=0x31;                     //set advanced program control
            *CmdPort=0x08;
            *CmdPort=0x2b;  //ok                   //internal power control  (13nF<LCD<=22nF)
            *CmdPort=0x25;  //ok                    //set TC=-0.15%
            *CmdPort=0xa0;   //ok                  //set line rate (00b:8.5Klps)
            *CmdPort=0xeb;    //ok                 //0XEA//set bias     ��Ϊ1/12ʱ��VLCDwr_command(0xeB);
            *CmdPort=0xf1;
            *CmdPort=0x7f;
            *CmdPort=0x81;  //ok
            *CmdPort=0x26;  //ok
            *CmdPort=0xc4;  //ok
            *CmdPort=0x89;
            *CmdPort=0xd1;  //ok
            *CmdPort=0xd5;  //ok
            *CmdPort=0xc8;  //ok
            *CmdPort=0x18;  //ok
            *CmdPort=0xad;  //ok

//           *CmdPort = 0x25;   //ok                       // �����¶Ȳ���ϵ��-0.05%/C
//         *CmdPort = 0x2b;   //ok                          // �ڲ�DC-DC
//         *CmdPort = 0xc4;   //ok                         // LCDӳ��MY=1��MX=0��LC0=0
//         *CmdPort = 0xa0;   //no same    ok                        // ������ɨ��Ƶ�� FR=263Hz
//         *CmdPort = 0xd1;   //ok                           // ��ɫ���ݸ�ʽR-G-B
//         *CmdPort = 0xd5;    //ok                        // ��������λΪ12λRRRR-GGGG-BBBB
//         *CmdPort = 0xc8; *CmdPort = 0x18;  //ok          // ����M�ź�Ϊ֡��ת
//         *CmdPort = 0xeb;   //ok                          // ����ƫѹ��1/12
//         *CmdPort = 0xa7;    //no same todo                          // ������ʾ
//         *CmdPort = 0xa4;    //nosame todo                          // ������ʾ
//         *CmdPort = 0x81; *CmdPort = ContrastLevel;  // ���öԱȶ�  ��Ӧ��ɫͼ��
//         *CmdPort = 0xdf;   //nosame todo                          // ����ɨ��ģʽ ����FRC
//         *CmdPort = 0xad;      //ok                       // ����ʾ  �����Ҷ���ʾ






         // ����ʾ  �����Ҷ���ʾ

//            *CmdPort = 0xf4; *CmdPort = xstart/3;           // ���ò���������߽�
//         *CmdPort = 0xf5; *CmdPort = (u8)dst_rect->top;  // ���ò��������ϱ߽�
//         *CmdPort = 0xf6; *CmdPort = xend/3+1;             // ���ò��������ұ߽�
//         *CmdPort = 0xf7; *CmdPort = (u8)(dst_rect->bottom-1);  // ���ò��������±߽�
//         *CmdPort = 0xf8;                                // ���ô��ڲ���ʹ��
//         *CmdPort = xstart/3 & 0x0f;
//         *CmdPort = 0x10|(xstart/3 >> 4);            // ������ʼ�е�ַ
//           *CmdPort = 0x60 + ((u8)dst_rect->top & 0x0f);
//           *CmdPort = 0x70 + ((u8)(dst_rect->top)>> 4);// ������ʼ�е�ַ

             *CmdPort=0x70;  //set row MSB address
            *CmdPort=0x60;  //set row LSB address
            *CmdPort=0x11;  //set column MSB address
            *CmdPort=0x08;  //set column LSB address



          *CmdPort = 0xf4; *CmdPort =0x18+xstart/3;           // ���ò���������߽�
          *CmdPort = 0xf5; *CmdPort = (u8)dst_rect->top;  // ���ò��������ϱ߽�
          *CmdPort = 0xf6; *CmdPort =0x18+xend/3+1;             // ���ò��������ұ߽�
          *CmdPort = 0xf7; *CmdPort = (u8)(dst_rect->bottom-1);  // ���ò��������±߽�
          *CmdPort = 0xf8;                                // ���ô��ڲ���ʹ��
          *CmdPort = (0x18+xstart/3) & 0x0f;
          *CmdPort = 0x10|((0x18+xstart/3) >> 4);            // ������ʼ�е�ַ
          *CmdPort = 0x60 + ((u8)dst_rect->top & 0x0f);
          *CmdPort = 0x70 + ((u8)(dst_rect->top)>> 4);// ������ʼ�е�ַ







            offset = ysrc * src_bitmap->linebytes + realxsrc/2;
            DJY_DelayUs(1);
            PatchF7Bug_SetA0;
            for(loopy = 0; loopy < dst_rect->bottom - dst_rect->top; loopy++)
            {
                for(loopx = 0; loopx < (xend-xstart+6)/2; loopx++ )
                {
                    c = src_bitmap->bm_bits[offset+loopx];
                    *DataPort = c;
                }
                offset += src_bitmap->linebytes;
            }
            PatchF7Bug_ClearA0;
            s0 = *CmdPort;
            s1 = *CmdPort;
            s2 = *CmdPort;
        }
    }
    else
    {

    }

    return true;
}

#pragma GCC pop_options
//----��screen��ȡ����---------------------------------------------------------
//����: ��screen��ȡһ���أ���ת����cn_sys_pf_e8r8g8b8��cn_sys_pf_a8r8g8b8��ʽ
//����: x,y������
//����: ������ɫֵ
//-----------------------------------------------------------------------------
u32 __lcd_get_pixel_screen(s32 x,s32 y)
{
    //x86_windows�£���������windows��ȡ������ȡ��
    return 0;
}

//----��ȡbitmap�о��ο�-------------------------------------------------------
//����: ��һ��λͼ�еľ��ζ�ȡ����һ��λͼ��
//����: rect������ȡ�ľ��Ρ�
//      dest��������ε�λͼ���䳤�������rect��ͬ��
//����: true=�ɹ���ȡ��false=ʧ�ܣ�ԭ��������ṩ�˲�֧�ֵ����ظ�ʽ��
//-----------------------------------------------------------------------------
bool_t __lcd_get_rect_screen(struct Rectangle *rect,struct RectBitmap *dest)
{
    return false;
 }

//----��ʼ��lcd�豸------------------------------------------------------------
//����: ����
//����: ��
//����: ��ʾ����Դָ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_UC1698(void)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;

    DJY_EventDelay(10*mS);
    LCD_BackLight(1);
    LCD_Reset();
    InitUC1698( );

    pg_frame_buffer = malloc(CFG_LCD_XSIZE*CFG_LCD_YSIZE/2);
    memset(pg_frame_buffer,0,CFG_LCD_XSIZE*CFG_LCD_YSIZE/2);
    FrameBitmap.bm_bits = (u8*)pg_frame_buffer;
    FrameBitmap.width = CFG_LCD_XSIZE;
    FrameBitmap.height = CFG_LCD_YSIZE;
    FrameBitmap.PixelFormat = CN_SYS_PF_GRAY4;
    FrameBitmap.linebytes = CFG_LCD_XSIZE/2;
    FrameBitmap.ExColor = 0xffffffff;
    frame_win.wm_bitmap = &FrameBitmap;
    tg_lcd_display.frame_buffer = &frame_win;

    tg_lcd_display.width_um = 0;
    tg_lcd_display.height_um = 0;
    tg_lcd_display.width = CFG_LCD_XSIZE;
    tg_lcd_display.height = CFG_LCD_YSIZE;
    tg_lcd_display.pixel_format = CN_SYS_PF_GRAY4;
    tg_lcd_display.framebuf_direct = false;
    //�����ʼ��frame_buffer��desktop��z_topmost������Ա

    tg_lcd_display.draw.SetPixelToBitmap = __lcd_set_pixel_bm;
    tg_lcd_display.draw.FillRectToBitmap = __lcd_fill_rect_bm;
    tg_lcd_display.draw.LineToBitmap = __lcd_line_bm;
    tg_lcd_display.draw.LineToBitmapIe = __lcd_line_bm_ie;
    tg_lcd_display.draw.BltBitmapToBitmap = __lcd_blt_bm_to_bm;
//    tg_lcd_display.draw.check_raster = __lcd_check_raster;
    tg_lcd_display.draw.SetPixelToScreen = __lcd_set_pixel_screen;
    tg_lcd_display.draw.LineToScreen = __lcd_line_screen;
    tg_lcd_display.draw.LineToScreenIe = __lcd_line_screen_ie;
    tg_lcd_display.draw.FillRectToScreen = __lcd_fill_rect_screen;
    tg_lcd_display.draw.CopyBitmapToScreen = __lcd_bm_to_screen;
    tg_lcd_display.draw.GetPixelFromScreen = __lcd_get_pixel_screen;
    tg_lcd_display.draw.GetRectFromScreen = __lcd_get_rect_screen;

    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    if(GK_InstallDisplay(&tg_lcd_display,CFG_DISPLAY_NAME))
        return (ptu32_t)&tg_lcd_display;
    else
        return 0;
}

