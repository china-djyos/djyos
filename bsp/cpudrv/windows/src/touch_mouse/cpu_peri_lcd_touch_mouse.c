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
#include <windows.h>
#include <tchar.h>
#include <string.h>
//#include <process.h>
#include <winGDI.h>
#include "stdint.h"
#include "Lcd_Touch_Key.h"
#include "gkernel.h"
#include "hmi-input.h"
#include "cpu_peri.h"
#include "cpu.h"
#include <gui/gk_display.h>
#include "touch.h"

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


HDC     lcd_hdc ;
BITMAPINFO *lcdbmi;
TCHAR szLcdDriver[]       =_TEXT("�����߲���ϵͳ");    // �������ı�

LRESULT CALLBACK    WndProc_lcd(HWND, UINT, WPARAM, LPARAM);

struct DisplayObj tg_lcd_display;

bool_t touched = false;
s32 touch_x=0,touch_y=0,touch_z=0;
RECT tg_WindowRect;
s32 s32g_window_height,s32g_window_width;

extern HWND tg_keyboard_wnd;

u16 *pg_frame_buffer;

u16* getframebuff()
{
   return pg_frame_buffer;

}

bool_t __lcd_line_bm_ie(struct RectBitmap *bitmap,struct Rectangle *limit,
                        s32 x1,s32 y1,s32 x2,s32 y2,u32 color,u32 r2_code);
bool_t __lcd_blt_bm_to_bm( struct RectBitmap *dst_bitmap,
                            struct Rectangle *DstRect,
                            struct RectBitmap *src_bitmap,
                            struct Rectangle *SrcRect,
                            struct RopGroup RopCode, u32 HyalineColor);
//----��ʼ��lcdӲ��------------------------------------------------------------
//����: ����
//����: ��
//����: ��
//��ע: ����������ֲ�ؼ�����.
//----------------------------------------------------------------------------
void __lcd_hard_init(void)
{
    RECT ClientRect;
    HWND lcd_wnd;
//    HBITMAP hbmi;

    lcdbmi = malloc(sizeof(BITMAPINFO)+64);
    lcdbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    lcdbmi->bmiHeader.biWidth = CN_LCD_XSIZE;
    lcdbmi->bmiHeader.biHeight = CN_LCD_YSIZE;
    lcdbmi->bmiHeader.biPlanes = 1;
    lcdbmi->bmiHeader.biBitCount = 16;
//    lcdbmi->bmiHeader.biBitCount = 4;
//  lcdbmi->bmiHeader.biCompression = BI_BITFIELDS;
    lcdbmi->bmiHeader.biCompression = BI_RGB;
    lcdbmi->bmiHeader.biSizeImage = 0;
    lcdbmi->bmiHeader.biXPelsPerMeter = 1;
    lcdbmi->bmiHeader.biYPelsPerMeter = 1;
    lcdbmi->bmiHeader.biClrUsed = 0;
    lcdbmi->bmiHeader.biClrImportant = 0;
    lcdbmi->bmiColors[0].rgbBlue  = 0;
    lcdbmi->bmiColors[0].rgbGreen = 0;
    lcdbmi->bmiColors[0].rgbRed   = 0;
    lcdbmi->bmiColors[0].rgbReserved = 0;
    lcdbmi->bmiColors[1].rgbBlue  = 0x10;
    lcdbmi->bmiColors[1].rgbGreen = 0x10;
    lcdbmi->bmiColors[1].rgbRed   = 0x10;
    lcdbmi->bmiColors[1].rgbReserved = 0;
    lcdbmi->bmiColors[2].rgbBlue  = 0x20;
    lcdbmi->bmiColors[2].rgbGreen = 0x20;
    lcdbmi->bmiColors[2].rgbRed   = 0x20;
    lcdbmi->bmiColors[2].rgbReserved = 0;
    lcdbmi->bmiColors[3].rgbBlue  = 0x30;
    lcdbmi->bmiColors[3].rgbGreen = 0x30;
    lcdbmi->bmiColors[3].rgbRed   = 0x30;
    lcdbmi->bmiColors[3].rgbReserved = 0;
    lcdbmi->bmiColors[4].rgbBlue  = 0x40;
    lcdbmi->bmiColors[4].rgbGreen = 0x40;
    lcdbmi->bmiColors[4].rgbRed   = 0x40;
    lcdbmi->bmiColors[4].rgbReserved = 0;
    lcdbmi->bmiColors[5].rgbBlue  = 0x50;
    lcdbmi->bmiColors[5].rgbGreen = 0x50;
    lcdbmi->bmiColors[5].rgbRed   = 0x50;
    lcdbmi->bmiColors[5].rgbReserved = 0;
    lcdbmi->bmiColors[6].rgbBlue  = 0x60;
    lcdbmi->bmiColors[6].rgbGreen = 0x60;
    lcdbmi->bmiColors[6].rgbRed   = 0x60;
    lcdbmi->bmiColors[6].rgbReserved = 0;
    lcdbmi->bmiColors[7].rgbBlue  = 0x70;
    lcdbmi->bmiColors[7].rgbGreen = 0x70;
    lcdbmi->bmiColors[7].rgbRed   = 0x70;
    lcdbmi->bmiColors[7].rgbReserved = 0;
    lcdbmi->bmiColors[8].rgbBlue  = 0x80;
    lcdbmi->bmiColors[8].rgbGreen = 0x80;
    lcdbmi->bmiColors[8].rgbRed   = 0x80;
    lcdbmi->bmiColors[8].rgbReserved = 0;
    lcdbmi->bmiColors[9].rgbBlue  = 0x90;
    lcdbmi->bmiColors[9].rgbGreen = 0x90;
    lcdbmi->bmiColors[9].rgbRed   = 0x90;
    lcdbmi->bmiColors[9].rgbReserved = 0;
    lcdbmi->bmiColors[10].rgbBlue  = 0xa0;
    lcdbmi->bmiColors[10].rgbGreen = 0xa0;
    lcdbmi->bmiColors[10].rgbRed   = 0xa0;
    lcdbmi->bmiColors[10].rgbReserved = 0;
    lcdbmi->bmiColors[11].rgbBlue  = 0xb0;
    lcdbmi->bmiColors[11].rgbGreen = 0xb0;
    lcdbmi->bmiColors[11].rgbRed   = 0xb0;
    lcdbmi->bmiColors[11].rgbReserved = 0;
    lcdbmi->bmiColors[12].rgbBlue  = 0xc0;
    lcdbmi->bmiColors[12].rgbGreen = 0xc0;
    lcdbmi->bmiColors[12].rgbRed   = 0xc0;
    lcdbmi->bmiColors[12].rgbReserved = 0;
    lcdbmi->bmiColors[13].rgbBlue  = 0xd0;
    lcdbmi->bmiColors[13].rgbGreen = 0xd0;
    lcdbmi->bmiColors[13].rgbRed   = 0xd0;
    lcdbmi->bmiColors[13].rgbReserved = 0;
    lcdbmi->bmiColors[14].rgbBlue  = 0xe0;
    lcdbmi->bmiColors[14].rgbGreen = 0xe0;
    lcdbmi->bmiColors[14].rgbRed   = 0xe0;
    lcdbmi->bmiColors[14].rgbReserved = 0;
    lcdbmi->bmiColors[15].rgbBlue  = 0xf0;
    lcdbmi->bmiColors[15].rgbGreen = 0xf0;
    lcdbmi->bmiColors[15].rgbRed   = 0xf0;
    lcdbmi->bmiColors[15].rgbReserved = 0;
//  lcdbmi->bmiColors[0].rgbBlue = 0;
//  lcdbmi->bmiColors[0].rgbGreen = 0xf8;
//  lcdbmi->bmiColors[0].rgbRed = 0;
//  lcdbmi->bmiColors[0].rgbReserved = 0;
//  lcdbmi->bmiColors[1].rgbBlue = 0xe0;
//  lcdbmi->bmiColors[1].rgbGreen = 0x07;
//  lcdbmi->bmiColors[1].rgbRed = 0;
//  lcdbmi->bmiColors[1].rgbReserved = 0;
//  lcdbmi->bmiColors[2].rgbBlue = 0x1f;
//  lcdbmi->bmiColors[2].rgbGreen = 0;
//  lcdbmi->bmiColors[2].rgbRed = 0;
//  lcdbmi->bmiColors[2].rgbReserved = 0;
    CreateDIBSection(0,lcdbmi,DIB_RGB_COLORS,(void**)&pg_frame_buffer,NULL,0);


    RegisterWinClass(hInstance,WndProc_lcd,szLcdDriver);
    lcd_wnd = CreateWindow(szLcdDriver, szLcdDriver, WS_CAPTION | WS_MINIMIZEBOX,
                       CW_USEDEFAULT,CW_USEDEFAULT, 0, 0,
                       NULL, NULL, hInstance, NULL);

    if (!lcd_wnd)
    {
        return;
    }
    GetWindowRect(lcd_wnd, &tg_WindowRect);
    GetClientRect(lcd_wnd, &ClientRect);
    tg_WindowRect.right += (cn_lcd_window_xsize - ClientRect.right);
    tg_WindowRect.bottom += (cn_lcd_window_ysize - ClientRect.bottom);
    s32g_window_height =  tg_WindowRect.bottom-tg_WindowRect.top;
    s32g_window_width = tg_WindowRect.right-tg_WindowRect.left;
    MoveWindow(lcd_wnd, tg_WindowRect.left, tg_WindowRect.top, s32g_window_width, s32g_window_height, false);

    ShowWindow(lcd_wnd, SW_SHOW);
    UpdateWindow(lcd_wnd);

    lcd_hdc = GetDC (lcd_wnd) ;
}

//
//  ����: WndProc_lcd(HWND, unsigned, WORD, LONG)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
LRESULT CALLBACK WndProc_lcd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT         ps ;
    switch (message)
    {
        case WM_PAINT:
            BeginPaint (hWnd, &ps) ;
//            StretchDIBits (lcd_hdc,    0, 0,cn_lcd_window_xsize, cn_lcd_window_ysize,
//                       0, CN_LCD_YSIZE, CN_LCD_XSIZE,-CN_LCD_YSIZE,pg_frame_buffer,lcdbmi,DIB_RGB_COLORS,SRCCOPY) ;
            EndPaint (hWnd, &ps) ;
            break;
        case WM_MOVE:
//            StretchDIBits (lcd_hdc,    0, 0,cn_lcd_window_xsize, cn_lcd_window_ysize,
//               0, CN_LCD_YSIZE, CN_LCD_XSIZE,-CN_LCD_YSIZE,pg_frame_buffer,lcdbmi,DIB_RGB_COLORS,SRCCOPY) ;
            GetWindowRect(hWnd, &tg_WindowRect);
            MoveWindow(tg_keyboard_wnd, tg_WindowRect.left+s32g_window_width + 5,
                       tg_WindowRect.top, s32g_window_width, s32g_window_height,
                       true);
            break ;
        case WM_ACTIVATE:
//            StretchDIBits (lcd_hdc,    0, 0,cn_lcd_window_xsize, cn_lcd_window_ysize,
//               0, CN_LCD_YSIZE, CN_LCD_XSIZE,-CN_LCD_YSIZE,pg_frame_buffer,lcdbmi,DIB_RGB_COLORS,SRCCOPY) ;

            break ;
        case WM_LBUTTONDOWN:
            touch_x = LOWORD(lParam)/CN_LCD_X_SCALE;
            touch_y = HIWORD(lParam)/CN_LCD_Y_SCALE;
            touch_z = 1;
            touched = true;
            break;
        case WM_LBUTTONUP:
            touch_x = LOWORD(lParam)/CN_LCD_X_SCALE;
            touch_y = HIWORD(lParam)/CN_LCD_Y_SCALE;
            touch_z = 0;
            touched = false;
            break;
        case WM_MOUSEMOVE:
            touch_x = LOWORD(lParam)/CN_LCD_X_SCALE;
            touch_y = HIWORD(lParam)/CN_LCD_Y_SCALE;
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


//----LCD�����ź�------------------------------------------------------------
//����: LCD ��Ƶ������߼�ʹ��/��ֹ,1=������Ƶ����� LCD�����ź�,
//      0=��ֹ��Ƶ����� LCD�����ź�
//����: onoff�������ź�
//����: ��
//-----------------------------------------------------------------------------
void __lcd_envid_of(int onoff)
{

}
//----����LCD��Դ--------------------------------------------------------------
//����: LCD ��Դ��������ʹ��
//����: invpwren��
//      pwren,
//����: ��
//-----------------------------------------------------------------------------
void __lcd_power_enable(int invpwren,int pwren)
{

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
bool_t __lcd_bm_to_screen(struct Rectangle *dst_rect,
                struct RectBitmap *src_bitmap,s32 xsrc,s32 ysrc)
{
#if 1
    StretchDIBits (lcd_hdc,   dst_rect->left*CN_LCD_X_SCALE, dst_rect->top*CN_LCD_Y_SCALE,
                            (dst_rect->right - dst_rect->left)*CN_LCD_X_SCALE,
                            (dst_rect->bottom - dst_rect->top)*CN_LCD_Y_SCALE,
                            xsrc,ysrc + dst_rect->bottom - dst_rect->top,
                            dst_rect->right - dst_rect->left,
                            -(dst_rect->bottom - dst_rect->top),
                            pg_frame_buffer,lcdbmi,DIB_RGB_COLORS,SRCCOPY) ;
#else
    StretchDIBits (lcd_hdc,    0, 0,320, 240,
               0, 240, 320,-240,pg_frame_buffer,lcdbmi,DIB_RGB_COLORS,SRCCOPY) ;
#endif
    return true;
}
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
ptu32_t ModuleInstall_Lcd(char *DisplayName,char *HeapName)
{
    static struct GkWinObj frame_win;
    static struct RectBitmap FrameBitmap;
    __lcd_hard_init();
//  __lcd_power_enable(0,1);
//  __lcd_envid_of(1);

    FrameBitmap.bm_bits = (u8*)pg_frame_buffer;
    FrameBitmap.width = CN_LCD_XSIZE;
    FrameBitmap.height = CN_LCD_YSIZE;
    FrameBitmap.PixelFormat = CN_LCD_PIXEL_FORMAT;
//    FrameBitmap.PixelFormat = CN_SYS_PF_GRAY4;
    FrameBitmap.linebytes = cn_lcd_line_size;
//  FrameBitmap.linebytes = 160;
    FrameBitmap.ExColor = 0xffffff;
    frame_win.wm_bitmap = &FrameBitmap;
    tg_lcd_display.frame_buffer = &frame_win;


    tg_lcd_display.width_um = CFG_LCD_XSIZE_UM;
    tg_lcd_display.height_um = CFG_LCD_YSIZE_UM;
    tg_lcd_display.width = CN_LCD_XSIZE;
    tg_lcd_display.height = CN_LCD_YSIZE;
//  tg_lcd_display.pixel_format = CN_SYS_PF_RGB565;
    tg_lcd_display.pixel_format = CN_LCD_PIXEL_FORMAT;
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

    tg_lcd_display.DisplayHeap = Heap_FindHeap(HeapName);
    tg_lcd_display.disp_ctrl = __lcd_disp_ctrl;

    if(GK_InstallDisplay(&tg_lcd_display,DisplayName))
        return (ptu32_t)&tg_lcd_display;
    else
        return 0;
}

//�����Ǵ��������֣���Ϊwindowsֻ��һ����ֻ꣬��ģ��һ����λ�豸�������������
//�ǲ���ͬʱʹ�ܵ�

//----��ȡ����������-----------------------------------------------------------
//����: ��ȡstmpe811�ɼ����Ĵ��������꣬����ж�㣬��ƽ��֮
//����: touch_data���ɼ���������
//����: 1=�����ʰ��£�0=����������
//-----------------------------------------------------------------------------
ufast_t read_touch_windows(struct SingleTouchMsg *touch_data)
{
    touch_data->display = NULL;
    touch_data->z = touch_z;
    if(touched == true)
    {
        touch_data->x = touch_x;
        touch_data->y = touch_y;
        return 1;
    }
    else
    {
        return 0;
    }
}

//----��ʼ��������ģ��---------------------------------------------------------
//����:
//����: display_name,����������Ӧ����ʾ����(��Դ��)
//����: ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_TouchWindows(struct GkWinObj *desktop,char * TouchName)
{
    static struct SingleTouchPrivate windows_touch;
    windows_touch.read_touch = read_touch_windows;
    windows_touch.touch_loc.display = NULL;     //NULL��ʾ��Ĭ������
    Touch_InstallDevice(TouchName,&windows_touch);
    return 1;
}

