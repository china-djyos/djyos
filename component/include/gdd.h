//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  LiuWei.
//�汾��V1.0.0
//�ļ�����: GDD����ͷ�ļ�(��GDD�ں˼��û�ʹ��)
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2011-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_H__
#define __GDD_H__

#if __cplusplus
extern "C" {
#endif

#include "stddef.h"
#include "gkernel.h"
#include <list.h>

#define LO16(a)     (a&0xFFFF)
#define HI16(a)     ((a>>16)&0xFFFF)

#define RGB(r,g,b)  ((u8)r<<16)|((u8)g<<8)|((u8)b)

typedef wchar_t WCHAR;
typedef const WCHAR* LPCWSTR;

typedef struct WINDOW*  HWND;
typedef struct DC*      HDC;
typedef struct FontObj*     HFONT;

typedef struct PointCdn      POINT;
typedef struct Rectangle     RECT;
typedef struct RectBitmap    BITMAP;

/*============================================================================*/
//// BMP������ݽṹ

#pragma pack(1)

typedef struct
{
    u16 bfType;      /* �ļ�����,   ����Ϊ   "BM "   (0x4D42)   */
    u32 bfSize;      /* �ļ��Ĵ�С(�ֽ�)   */
    u16 bfRsvd1;     /* ����,   ����Ϊ   0   */
    u16 bfRsvd2;     /* ����,   ����Ϊ   0   */
    u32 bfOffBits;   /* λͼ����������ļ�ͷ��ƫ����(�ֽ�)   */

    u32 biSize;       /* size   of   BITMAPINFOHEADER   */
    u32 biWidth;      /* λͼ����(����)   */
    u32 biHeight;     /* λͼ�߶�(����)   */
    u16 biPlanes;     /* Ŀ���豸��λƽ����,   ������Ϊ1   */
    u16 biBitCount;   /* ÿ�����ص�λ��,   1,4,8��24   */
    u32 biCompress;   /* λͼ���е�ѹ������,0=��ѹ��   */
    u32 biSizeImage;  /* ͼ���С(�ֽ�)   */
    u32 biXPelsPerMeter;  /* Ŀ���豸ˮƽÿ�����ظ���   */
    u32 biYPelsPerMeter;  /* Ŀ���豸��ֱÿ�����ظ���   */
    u32 biColorUsed;      /* λͼʵ��ʹ�õ���ɫ������ɫ��   */
    u32 biColorImportant; /* ��Ҫ��ɫ�����ĸ���   */
    u32 biRedMask;
    u32 biGreenMask;
    u32 biBlueMask;
    u32 biAlphaMask;

}tagBMP_HEADER;

typedef struct
{
    u16 Format;      /* ��ɫ��ʽ  */
    u16 Width;       /* ͼ�����(����ֵ) */
    u16 Height;      /* ͼ��߶�(����ֵ) */
    u16 LineBytes;   /* ÿ��ͼ�����ݵ��ֽ���  */
}tagBMP_INFO;

#pragma pack()




/*============================================================================*/
//// ��Ϣ���ݽṹ
struct WindowMsg
{
    HWND hwnd;      //��Ϣ��������.
    u32 Code;       //��Ϣ����.
    ptu32_t Param1;     //��Ϣ����1,��ͬ��Ϣ����,���岻ͬ.
    ptu32_t Param2;     //��Ϣ����2,��ͬ��Ϣ����,���岻ͬ.
    const void* ExData;     //��Ϣ��չ����

};

//�������ڶ���ؼ������ڣ�����Ϣ���������б�
struct MsgProcTable
{
    u32 MsgCode;
    ptu32_t (*MsgProc)(struct WindowMsg *pMsg);
};

// ������Ҫ��������Ϣ������ʵ��ʱע�⣬������Head�ڵ���������Ҫ�����ģ�ֱ��
// LinkNext == NULL�Ž���������
struct MsgTableLink
{
    struct MsgTableLink *LinkPrev;
    struct MsgTableLink **pLinkNext;
    struct MsgTableLink *LinkAdd;
//    list_t TableLink;
    struct MsgProcTable *myTable;
    u32 MsgNum;
};




/*============================================================================*/
//// DrawText flag
#define DT_VCENTER      (0<<0)  //���Ĵ�ֱ����.
#define DT_TOP          (1<<0)  //���Ķ��˶���.
#define DT_BOTTOM       (2<<0)  //���ĵײ�����.
#define DT_ALIGN_V_MASK (3<<0)

#define DT_CENTER       (0<<2)  //ʹ�����ھ�����ˮƽ����.
#define DT_LEFT         (1<<2)  //���������.
#define DT_RIGHT        (2<<2)  //�����Ҷ���.
#define DT_ALIGN_H_MASK (3<<2)





#define DT_BORDER       (1<<4)  //���Ʊ߿�(ʹ��DrawColor)
#define DT_BKGND        (1<<5)  //��䱳��(ʹ��FillColor)

//// �������ģʽ���μ�gkernel.h




//// ��갴��״̬
#define MK_RBUTTON  (1<<0)
#define MK_MBUTTON  (1<<1)
#define MK_LBUTTON  (1<<2)

//// �����¼�����
typedef enum{
    KEY_EVENT_DOWN =1,  //��������.
    KEY_EVENT_UP   =2,  //��������.
}EN_GDD_KEY_EVENT;

//��Ϣ�붨�巽����
//bit0~23�����ڶ�����Ϣ������0��ʹ�ã�1~65535��ϵͳ�Ϳؼ�ʹ�ã�65536�������û�
//         ʹ��
//bit24~31�����ڶ���һЩ��Ϣ�����򣬽�������Ϣ����������ʱʹ�ã������ͽ��յ���
//          ��Ϣ������������Щ���ݡ�
//  bit31���������Ϣ�ļ̳з�ʽ��
//      0��none��ʽ����ִ����ͼ��ӽڵ��ϵ���Ϣ�������������û�ж��壬�Ŵ�
//         ���ڵ���ң�ֱ������ϵͳĬ�ϡ�
//      1���̳�ʽ����ִ�и��ڵ㶨�����Ϣ�������������ڵ�û�ж���Ļ�������ǰ
//        �ң�ֱ������ϵͳĬ�ϣ����ִ���Լ���
#define MSG_CONTROL_MSK         0xff000000      //��Ϣ����������
#define MSG_ADOPT_MSK           0x80000000      //��Ϣ�����̳з�ʽ����
#define MSG_ADOPT_NONE          0x00000000      //���̳и���
#define MSG_ADOPT_NORMAL        0x80000000      //�̳и��ദ������

#define MSG_BODY_MASK           0xffffff        //��Ϣ������
// ͨ�ô�����Ϣ����
#define MSG_CREATE              0x0001  //���ڴ�����Ϣ; Param1:��WindowCreate��pdata����;Param2:����.
#define MSG_ERASEBKGND          0x0002  //���ڱ�������; Param1:��ͼ������; Param2:����.
#define MSG_NCPAINT             0x0003  //���ڷǿͻ�������; Param1:����; Param2:����.
#define MSG_PAINT               0x0004  //���ڿͻ�������; Param1:����; Param2:����.
#define MSG_TIMER               0x0005  //��ʱ����Ϣ: Param1:��ʱId; Param2:��ʱ������.
#define MSG_SETTEXT             0x0006  //���ô�������
#define MSG_GETTEXT             0x0007  //��ô�������
#define MSG_GETTEXTFLENGTH      0x0008  //��ô������ֳ���

//close��Ϣ��hwnd��param2�Ƚ����⣺MSG_CLOSE�Դ��̳����ԣ���ô�����û�����Ϣ����
//����ʱ�����ڱ����Ѿ������٣���Ϣ��hwnd��Ա�������
//����MSG_CLOSE��Ϣʱ��param2���ÿ��ǣ��������û��������Ϣ�����������ú����յ�
//����Ϣ�У�param2 == hwnd->Private������ϵͳ���ӵġ�
#define MSG_CLOSE       (MSG_ADOPT_NORMAL+0x0009)  //���ڹر���Ϣ��Param1:����;
//#define MSG_DESTROY             0x000A  //����������Ϣ
//#define MSG_QUIT                0x000B  //�����˳���Ϣ
#define MSG_NOTIFY              0x000C  //֪ͨ��Ϣ; Param1:�ؼ�Id(L16),֪ͨ��(H16); Param2:�ؼ�HWND.
#define MSG_SETFOCUS            0x000D  //���ڻ�ý���; Param1:����; Param2:����;
#define MSG_KILLFOCUS           0x000E  //����ʧȥ����; Param1:����; Param2:����;
#define MSG_LBUTTON_DOWN        0x0100  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_LBUTTON_UP          0x0101  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_DOWN        0x0102  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_UP          0x0103  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_DOWN        0x0104  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_UP          0x0105  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MOUSE_MOVE          0x0106  //����ƶ�; Param1:��갴��״̬; Param2:x����(L16),y����(H16).
#define MSG_NCLBUTTON_DOWN      0x0107  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCLBUTTON_UP        0x0108  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_DOWN      0x0109  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_UP        0x010A  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_DOWN      0x010B  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_UP        0x010C  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMOUSE_MOVE        0x010D  //�ǿͻ�������ƶ�; Param1:��갴��״̬; Param2:x����(L16),y����(H16).
#define MSG_SETATTR             0x010E  //�����������ã�

#define MSG_KEY_DOWN            0x0120  //���̰���; ��16λ(H16)WinID,Param1:����ֵ(L16); Param2:�¼�������ʱ��(���뵥λ).
#define MSG_KEY_UP              0x0121  //���̵���; Param1:����ֵ(L16); Param2:�¼�������ʱ��(���뵥λ).
#define MSG_KEY_PRESS           0x0122

#define MSG_TOUCH_DOWN          0x0130   //�����������´�����
#define MSG_TOUCH_UP            0x0131   //�������뿪������
#define MSG_TOUCH_MOVE          0x0132
#define MSG_NCTOUCH_DOWN        0x0133
#define MSG_NCTOUCH_UP          0x0134
#define MSG_NCTOUCH_MOVE        0x0135

#define MSG_TIMER_START         0x0140  //��ʱ����Ϣ: Param1:��ʱId; Param2:��ʱ������.
#define MSG_TIMER_STOP          0x0141  //��ʱ����Ϣ: Param1:��ʱId; Param2:��ʱ������.
//#define   MSG_GET_POS         0xF000

//// ��������Ϣ
#define MSG_ProcessBar_SETDATA     0x2100  //�������ݣ�Param1�������������ṹָ�룻Param2�����ԣ����أ�����.
#define MSG_ProcessBar_GETDATA     0x2101  //������ݣ�Param1�����������ݽṹָ�룻Param2�����ԣ����أ�����.
#define MSG_ProcessBar_SETRANGE    0x2102  //��������: Param1:����ֵ; Param2:����; ����:����.
#define MSG_ProcessBar_GETRANGE    0x2103  //�������: Param1:����; Parame2:����; ����:����ֵ.
#define MSG_ProcessBar_SETPOS      0x2104  //���õ�ǰλ��: Param1:��ǰλ��; Param2:����; ����:����.
#define MSG_ProcessBar_GETPOS      0x2105  //��õ�ǰλ��: Param1:����; Param1:����; ����:��ǰλ��.

//// ListBox��Ϣ.
#define MSG_ListBox_ADDSTRING       0x2200  //����һ���ַ���; Param1:��Ŀ����; Param2:�ַ�ָ��; ����:ʵ����Ŀ����.
#define MSG_ListBox_DELSTRING       0x2201  //ɾ��һ���ַ���; Param1:��Ŀ����; Param2:����; ����:����.
#define MSG_ListBox_SETCURSEL       0x2202  //���õ�ǰѡ����; Param1:��Ŀ����; Param2:����; ����:����.
#define MSG_ListBox_GETCURSEL       0x2203  //��õ�ǰѡ����; Param1:����; Param2:����; ����:��ǰѡ��������.
#define MSG_ListBox_SETTOPINDEX     0x2204  //���ö����׸��ɼ���; Param1:��Ŀ����; Param2:����; ����:����.
#define MSG_ListBox_GETTOPINDEX     0x2205  //��ö����׸��ɼ���; Param1:����; Param2:����; ����:�����׸��ɼ�������.
#define MSG_ListBox_GETCOUNT        0x2206  //��������б������Ŀ����; Param1:����; Param2:����; ����:��Ŀ����.
#define MSG_ListBox_RESETCONTENT    0x2207  //ɾ���б���������Ŀ; Param1:����; Param2:����; ����:����.
#define MSG_ListBox_GETTEXTLEN      0x2208  //���ָ����Ŀ�������ֽ���; Param1:��Ŀ����; Param2:����; ����:����.
#define MSG_ListBox_GETTEXT         0x2209  //���ָ����Ŀ������; Param1:��Ŀ����; Param2:���������; ����:����.
#define MSG_ListBox_SETITEMHEIGHT   0x220A  //�����б�����Ŀ�߶�; Param1:�߶�ֵ(���ص�λ); Param2:����; ����:����.
#define MSG_ListBox_GETITEMHEIGHT   0x220B  //����б�����Ŀ�߶�; Param1:����; Param2:����; ����:�߶�ֵ(���ص�λ).
#define MSG_ListBox_SETITEMDATA     0x220C  //�����б�����Ŀ����; Param1:��Ŀ����; Param2:��Ŀ��ֵ; ����:����.
#define MSG_ListBox_GETITEMDATA     0x220D  //����б�����Ŀ����; Param1:��Ŀ����; Param2:����; ����:��Ŀ��ֵ.

#define MSG_WM_USER                 0x10000 //�û��������Ϣ��ʼ����
/*============================================================================*/
//// �ؼ�֪ͨ��

//Button
#define MSG_BTN_DOWN    1    //����
#define MSG_BTN_UP      2    //����

//ListBox
//#define LBN_SELCHANGE   1   //��ǰѡ����Ŀ���ı�

//Listview
#define LVN_CLICK   1   //��"���"

/*============================================================================*/
// ���ڹ��������ؼ�����Լ��ؼ�������ϳ�32λ�֣����д��ڹ������ʹ�ø�16λ
// �ؼ����ʹ��ʣ��16λ��
#define WS_CHILD        (1<<16) //�Ӵ��ڱ�־,�ؼ����ڱ���ָ���ñ�־.
#define WS_VISIBLE      (1<<17) //�����Ƿ�ɼ�
#define WS_DISABLE      (1<<18) //�����Ƿ�Ϊ��ֹ״̬,ָ���ñ�־,���ڽ�����Ӧ������Ϣ
#define WS_BORDER       (1<<19) //�����Ƿ��б߿�
#define WS_DLGFRAME     (1<<20) //�����Ƿ��жԻ��߿�
#define WS_CAPTION      (1<<21) //�����Ƿ��б�����
#define WS_SYSMENU      (1<<22) //ָ���ñ�־ʱ,������������"�ر�"��ť,
                                //���ڱ���ָ���� WS_CAPTION��־ʱ,����Ч.
#define WS_SHOW_CURSOR  (1<<23) //���ڹ���Ƿ���ʾ
#define WS_CAN_FOCUS    (1<<24) //�����Ƿ�������Ϊfocus����

//#define WS_MAIN_WINDOW  (WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU)


/*============================================================================*/

#define CN_WINDOW_ZPRIO_MOUSE       (-101)
#define CN_WINDOW_ZPRIO_CURSOR      (-100)

HDC     CreateDC(struct GkWinObj *gk_win,const RECT *prc);
bool_t    DeleteDC(HDC hdc);

struct RopGroup SetRopCode(HDC hdc,struct RopGroup RopCode);
struct RopGroup GetRopCode(HDC hdc);
void   MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt);

u32 SetDrawColor(HDC hdc,u32 color);
u32 GetDrawColor(HDC hdc);
u32 SetFillColor(HDC hdc,u32 color);
u32 GetFillColor(HDC hdc);
u32 SetTextColor(HDC hdc,u32 color);
u32 GetTextColor(HDC hdc);
u32 SetSyncTime(HDC hdc,u32 sync_time);
u32 GetSyncTime(HDC hdc);

s32  GetStrLineCount(const char *str);
void AdjustTextRect(HDC hdc,const char *text,int count, RECT *prc,u32 flag);

HFONT   SetFont(HDC hdc,HFONT hFont);
HFONT   GetFont(HDC hdc);


void    SetPixel(HDC hdc,s32 x,s32 y,u32 color);
void    DrawLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1);
void    DrawDottedLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1);
void    DrawLineEx(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,u32 color);
void    DrawLineTo(HDC hdc,s32 x,s32 y);
bool_t  TextOut(HDC hdc,s32 x,s32 y,const char *text,s32 count);
bool_t  DrawText(HDC hdc,const char *text,s32 count,const RECT *prc,u32 flag);
void    DrawRect(HDC hdc,const RECT *prc);;
void    FillRect(HDC hdc,const RECT *prc);
void    FillRectEx(HDC hdc,const RECT *prc,u32 color);
void    GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode);
void    Fill3DRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2);

void    DrawCircle(HDC hdc,s32 cx,s32 cy,s32 r);
void    FillCircle(HDC hdc,s32 cx,s32 cy,s32 r);
void    DrawEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry);
void    FillEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry);
void    DrawSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2);
void    FillSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2);
void    DrawBezier3(HDC hdc,const POINT *pt,s32 cnt);

void    DrawPolyLine(HDC hdc,const POINT *pt,s32 count);
void    DrawGroupBox(HDC hdc,const RECT *prc,const char *Text);
bool_t  DrawBitmap(HDC hdc,s32 x,s32 y,struct RectBitmap *bitmap,u32 key_color,
                        struct RopGroup RopCode);

bool_t  GetBMPInfo(tagBMP_INFO *bm_info,tagBMP_HEADER *pBmpHdr);
bool_t  DrawBMP(HDC hdc,s32 x,s32 y,const void *bmp_data);

//��Ϣ��غ���
u32     DispatchMessage(struct WindowMsg *pMsg);
u32     SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
bool_t  PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
bool_t  GetMessage(struct WindowMsg *pMsg,HWND hwnd,bool_t *SyncMsg);


HWND    GetDesktopWindow(void);
u32     GetWindowStyle(HWND hwnd);
void*   GetWindowPrivateData(HWND hwnd);
void    SetWindowPrivateData(HWND hwnd,void *data);
HWND    InitGddDesktop(struct GkWinObj *desktop);

//DC��������
HDC     GetWindowDC(HWND hwnd);
HDC     GetDC(HWND hwnd);
bool_t    ReleaseDC(HWND hwnd,HDC hdc);
HDC     BeginPaint(HWND hwnd);
bool_t    EndPaint(HWND hwnd,HDC hdc);

//���ڲ�������
void AddProcFuncTable(HWND hwnd,struct MsgTableLink *pNewMsgTableLink);
HWND    CreateWindow(const char *Text,u32 Style,
                     s32 x,s32 y,s32 w,s32 h,
                     HWND hParent,u32 WinId,
                     u32 BufProperty, void *pdata,
                     struct MsgTableLink *pUserMsgTableLink);
void    DestroyWindow(HWND hwnd);
void    DestroyAllChild(HWND hwnd);
bool_t    MoveWindow(HWND hwnd,s32 x,s32 y);
bool_t    OffsetWindow(HWND hwnd,s32 dx,s32 dy);
bool_t    IsWindowVisible(HWND hwnd);
bool_t    InvalidateWindow(HWND hwnd,bool_t bErase);
bool_t    SetWindowShow(HWND hwnd);
bool_t    SetWindowHide(HWND hwnd);
bool_t    EnableWindow(HWND hwnd,bool_t bEnable);
HWND    Gdd_GetWindowParent(HWND hwnd);
HWND GetWindowChild(HWND hwnd);
HWND GetWindowPrevious(HWND hwnd);
HWND __GetWindowTwig(HWND hwnd);
HWND GetWindowNext(HWND hwnd);
HWND GetWindowFirst(HWND hwnd);
HWND GetWindowLast(HWND hwnd);
char    *GetWindowText(HWND hwnd);
HWND    GetDlgItem(HWND hwnd,u16 id);
void    SetWindowText(HWND hwnd,const char *text,s32 max_len);
HWND    GetWindowFromPoint(struct GkWinObj *desktop, POINT *pt);

//���κ������������
bool_t    GetWindowRect(HWND hwnd,RECT *prc);
bool_t    GetClientRect(HWND hwnd,RECT *prc);
bool_t    GetClientRectToScreen(HWND hwnd,RECT *prc);
s32     RectW(const RECT *prc);
s32     RectH(const RECT *prc);
void    SetRect(RECT *prc,s32 x,s32 y,s32 w,s32 h);
void    SetRectEmpty(RECT *prc);
bool_t    CopyRect(RECT *dst,const RECT *src);
bool_t    IsRectEmpty(const RECT *prc);
bool_t    OffsetRect(RECT *prc,s32 dx,s32 dy);
bool_t    MoveRect(RECT *prc,s32 x,s32 y);
bool_t    InflateRect(RECT *prc,s32 dx,s32 dy);
bool_t    InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b);
bool_t    PtInRect(const RECT *prc,const POINT *pt);
bool_t    ScreenToClient(HWND hwnd,POINT *pt,s32 count);
bool_t    ClientToScreen(HWND hwnd,POINT *pt,s32 count);
bool_t    ScreenToWindow(HWND hwnd,POINT *pt,s32 count);
bool_t    WindowToScreen(HWND hwnd,POINT *pt,s32 count);

/*===========================================================================*/
void    ModuleInstall_GDD(struct GkWinObj *desktop);

u32     AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha);
void    UpdateDisplay(u32 timeout);
void GDD_WaitGuiAppExit(char *AppName);
HWND GDD_CreateGuiApp(char *AppName,struct MsgTableLink  *MyMsgLink,
                      u32 MemSize, u32 WinBuf,u32 Style);

/*===========================================================================*/
#include <gui\gdd_timer.h>
#include <gui\gdd_focus.h>
#include <gui\gdd_cursor.h>
#include <gui\gdd_mouse.h>
#include <gui\gdd_gif.h>


#if __cplusplus
}
#endif
#endif  /*__GDD_H__*/