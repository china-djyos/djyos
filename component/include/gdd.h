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
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
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

//ע��biHeight>0��ʾ����λͼ����ԭ�������½ǣ�����Ϊ����λͼ����ԭ�������Ͻǡ�
typedef struct
{
    u16 bfType;      /* �ļ�����,   ����Ϊ   "BM "   (0x4D42)   */
    u32 bfSize;      /* �ļ��Ĵ�С(�ֽ�)   */
    u16 bfRsvd1;     /* ����,   ����Ϊ   0   */
    u16 bfRsvd2;     /* ����,   ����Ϊ   0   */
    u32 bfOffBits;   /* λͼ����������ļ�ͷ��ƫ����(�ֽ�)   */

    u32 biSize;       /* size   of   BITMAPINFOHEADER   */
    u32 biWidth;      /* λͼ���(����)   */
    s32 biHeight;     /* λͼ�߶�(����)����Ϊ��������������λͼ   */
    u16 biPlanes;     /* Ŀ���豸��λƽ����,   ������Ϊ1   */
    u16 biBitCount;   /* ÿ�����ص�λ��,   1,4,8,16,24,32   */
    u32 biCompress;   /* λͼ���е�ѹ������,0=��ѹ��   */
    u32 biSizeImage;  /* ͼ���С(�ֽ�)   */
    u32 biXPelsPerMeter;  /* Ŀ���豸ˮƽÿ�����ظ���   */
    u32 biYPelsPerMeter;  /* Ŀ���豸��ֱÿ�����ظ���   */
    u32 biColorUsed;      /* λͼʵ��ʹ�õ���ɫ�����ɫ��   */
    u32 biColorImportant; /* ��Ҫ��ɫ�����ĸ���   */
    u32 biRedMask;
    u32 biGreenMask;
    u32 biBlueMask;
    u32 biAlphaMask;

}tagBMP_HEADER;

#pragma pack()

/*============================================================================*/
//// ��Ϣ���ݽṹ
struct WindowMsg
{
    HWND hwnd;          //��Ϣ��������.
    u32 Code;           //��Ϣ����.
    ptu32_t Param1;     //��Ϣ����1,��ͬ��Ϣ����,���岻ͬ.
    ptu32_t Param2;     //��Ϣ����2,��ͬ��Ϣ����,���岻ͬ.
    const void* ExData; //��Ϣ��չ����

};

//�������ڶ���ؼ������ڣ�����Ϣ�������б�
struct MsgProcTable
{
    u32 MsgCode;
    ptu32_t (*MsgProc)(struct WindowMsg *pMsg);
};

// ������Ҫ�������Ϣ����ʵ��ʱע�⣬�����Head�ڵ���������Ľڵ㣬head->prev��
// �������Ľڵ�
struct MsgTableLink
{
//    struct MsgTableLink *PrevMsg;
//    struct MsgTableLink *NextMsg;
    struct MsgProcTable *myTable;
    u32 MsgNum;
//    struct MsgProcTable myTable[];
};




/*============================================================================*/
// GDD_DrawText flag
#define DT_VCENTER      (0<<0)  //���Ĵ�ֱ����.
#define DT_TOP          (1<<0)  //���Ķ��˶���.
#define DT_BOTTOM       (2<<0)  //���ĵײ�����.
#define DT_ALIGN_V_MASK (3<<0)  //ˮƽ�������λmsk

#define DT_CENTER       (0<<2)  //ʹ�����ھ�����ˮƽ����.
#define DT_LEFT         (1<<2)  //���������.
#define DT_RIGHT        (2<<2)  //�����Ҷ���.
#define DT_ALIGN_H_MASK (3<<2)  //��ֱ�������λmsk

#define DT_BORDER       (1<<4)  //���Ʊ߿�(ʹ��DrawColor)
#define DT_BKGND        (1<<5)  //��䱳��(ʹ��FillColor)

//��Ϣ�붨�巽����
//bit0~23�����ڶ�����Ϣ������0��ʹ�ã�1~65535��ϵͳ�Ϳؼ�ʹ�ã�65536�������û�
//         ʹ��
//bit24~31�����ڶ���һЩ��Ϣ�����򣬽�������Ϣ��������ʱʹ�ã������ͽ��յ���
//          ��Ϣ������������Щ���ݡ�
//  bit31���������Ϣ�ļ̳з�ʽ��
//      0��none��ʽ����ִ����ͼ��ӽڵ��ϵ���Ϣ�����������û�ж��壬�Ŵ�
//         ���ڵ���ң�ֱ������ϵͳĬ�ϡ�
//      1���̳�ʽ����ִ�и��ڵ㶨�����Ϣ�����������ڵ�û�ж���Ļ�������ǰ
//        �ң�ֱ������ϵͳĬ�ϣ����ִ���Լ���
#define MSG_CONTROL_MSK         0xff000000      //��Ϣ����������
#define MSG_ADOPT_MSK           0x80000000      //��Ϣ����̳з�ʽ����
#define MSG_ADOPT_NONE          0x00000000      //���̳и���
#define MSG_ADOPT_NORMAL        0x80000000      //�̳и��ദ����

#define MSG_BODY_MASK           0xffffff        //��Ϣ������
// ͨ�ô�����Ϣ����
#define MSG_CREATE              0x0001  //���ڴ�����Ϣ; Param1:��WindowCreate��pdata����;Param2:�û�����
#define MSG_ERASEBKGND          0x0002  //���ڱ�������; Param1:��ͼ������; Param2:�û�����
#define MSG_NCPAINT             0x0003  //���ڷǿͻ�������; Param1:�û�����; Param2:�û�����
#define MSG_PAINT               0x0004  //���ڿͻ�������; Param1:�û�����; Param2:�û�����
#define MSG_TIMER               0x0005  //��ʱ����Ϣ: Param1:��ʱId; Param2:��ʱ������.
#define MSG_SETTEXT             0x0006  //���ô��ڱ���
#define MSG_GETTEXT             0x0007  //��ô��ڱ���
#define MSG_GETTEXTLEN          0x0008  //��ô��ڱ��ⳤ��
#define MSG_SYNC_DISPLAY        0x0009  //ͬ�����ڣ�����ʾ����ˢ�µ���ʾ���ϡ�
//close��Ϣ��hwnd��param2�Ƚ����⣺MSG_CLOSE�Դ��̳����ԣ���ô�����û�����Ϣ����
//����ʱ�����ڱ����Ѿ������٣���Ϣ��hwnd��Ա�������
//����MSG_CLOSE��Ϣʱ��param2���ÿ��ǣ��������û��������Ϣ���������ú����յ�
//����Ϣ�У�param2 == hwnd->PrivateData������ϵͳ��ӵģ������û����Щ�ض�������
#define MSG_CLOSE       (MSG_ADOPT_NORMAL+0x000A)  //���ڹر���Ϣ��Param1:����;
//#define MSG_DESTROY             0x000A  //����������Ϣ
//#define MSG_QUIT                0x000B  //�����˳���Ϣ
#define MSG_NOTIFY              0x000B  //֪ͨ��Ϣ; Param1:�ؼ�Id(L16),֪ͨ��(H16); Param2:�ؼ�HWND.
#define MSG_SETFOCUS            0x000C  //���ڻ�ý���; Param1:����; Param2:����;
#define MSG_KILLFOCUS           0x000D  //����ʧȥ����; Param1:����; Param2:����;
#define MSG_LBUTTON_DOWN        0x0100  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_LBUTTON_UP          0x0101  //����������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_DOWN        0x0102  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_RBUTTON_UP          0x0103  //����Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_DOWN        0x0104  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MBUTTON_UP          0x0105  //����м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_MOUSE_MOVE          0x0106  //����ƶ�; Param1��X����1mS������um��(L16)��Y����1mS������um��(H16),
                                        //               param2��x����(L16),y����(H16)
#define MSG_NCLBUTTON_DOWN      0x0107  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCLBUTTON_UP        0x0108  //�ǿͻ�������������; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_DOWN      0x0109  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCRBUTTON_UP        0x010A  //�ǿͻ�������Ҽ�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_DOWN      0x010B  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMBUTTON_UP        0x010C  //�ǿͻ�������м�����; Param1:����; Param2:x����(L16),y����(H16).
#define MSG_NCMOUSE_MOVE        0x010D  //�ǿͻ�������ƶ�; Param1��X����1mS������um��(L16)��Y����1mS������um��(H16),
                                        //               param2��x����(L16),y����(H16)
#define MSG_SETATTR             0x010E  //�����������ã�

#define MSG_KEY_DOWN            0x0120  //���̰���;Param1: ����ֵ; Param2:�¼�������ʱ��(���뵥λ).
#define MSG_KEY_UP              0x0121  //���̵���;Param1: ����ֵ; Param2:�¼�������ʱ��(���뵥λ).
#define MSG_KEY_PRESS           0x0122

#define MSG_TOUCH_DOWN          0x0130  //���ڿͻ�����Ӵ���������param1�����ԣ�param2��x����(L16),y����(H16)
#define MSG_TOUCH_UP            0x0131  //���ڿͻ������뿪������
#define MSG_TOUCH_MOVE          0x0132  //��ָ�����ϴ��ڿͻ����򻬶���Param1��X����1mS������um��(L16)��Y����1mS������um��(H16),
                                        //               param2��x����(L16),y����(H16)
#define MSG_NCTOUCH_DOWN        0x0133  //�����������´����㣬param1�����ԣ�param2��x����(L16),y����(H16)
#define MSG_NCTOUCH_UP          0x0134
#define MSG_NCTOUCH_MOVE        0x0135  //ͬ MSG_TOUCH_MOVE ��Ϣ

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
#define MSG_ListBox_GETCOUNT        0x2206  //��������б�����Ŀ����; Param1:����; Param2:����; ����:��Ŀ����.
#define MSG_ListBox_RESETCONTENT    0x2207  //ɾ���б��������Ŀ; Param1:����; Param2:����; ����:����.
#define MSG_ListBox_GETTEXTLEN      0x2208  //���ָ����Ŀ�������ֽ���; Param1:��Ŀ����; Param2:����; ����:����.
#define MSG_ListBox_GETTEXT         0x2209  //���ָ����Ŀ������; Param1:��Ŀ����; Param2:���������; ����:����.
#define MSG_ListBox_SETITEMHEIGHT   0x220A  //�����б����Ŀ�߶�; Param1:�߶�ֵ(���ص�λ); Param2:����; ����:����.
#define MSG_ListBox_GETITEMHEIGHT   0x220B  //����б����Ŀ�߶�; Param1:����; Param2:����; ����:�߶�ֵ(���ص�λ).
#define MSG_ListBox_SETITEMDATA     0x220C  //�����б����Ŀ����; Param1:��Ŀ����; Param2:��Ŀ��ֵ; ����:����.
#define MSG_ListBox_GETITEMDATA     0x220D  //����б����Ŀ����; Param1:��Ŀ����; Param2:����; ����:��Ŀ��ֵ.

#define MSG_WM_USER                 0x10000 //�û��������Ϣ��ʼ����
/*============================================================================*/
//// �ؼ�֪ͨ��

//Button
#define MSG_BTN_DOWN        1   //����
#define MSG_BTN_UP          2   //����
#define MSG_BTN_PEN_MOVE    3   //��������
#define MSG_KEY_CHANGE      4   //����״̬�ı�
//ListBox
//#define LBN_SELCHANGE   1   //��ǰѡ����Ŀ���ı�

//Listview
#define LVN_CLICK   1   //��"���"

/*============================================================================*/
// ���ڹ��������ؼ�����Լ��ؼ�������ϳ�32λ�֣����д��ڹ������ʹ�ø�16λ
// �ؼ����ʹ��ʣ��16λ��
//#define WS_WIDGET      (1<<16) //��ɾ�������ڷ������Ҫ�˱�־��ʹ����ֱ��ɾ��
#define WS_VISIBLE      (1<<17) //�����Ƿ�ɼ�
#define WS_DISABLE      (1<<18) //�����Ƿ�Ϊ��ֹ״̬,ָ���ñ�־,���ڽ�����Ӧ������Ϣ
#define WS_BORDER       (1<<19) //�����Ƿ��б߿�
#define WS_DLGFRAME     (1<<20) //�����Ƿ��жԻ��߿�
#define WS_CAPTION      (1<<21) //�����Ƿ��б�����
#define WS_SYSMENU      (1<<22) //ָ���ñ�־ʱ,������������"�ر�"��ť,
                                //���ڱ���ָ���� WS_CAPTION��־ʱ,����Ч.
#define WS_SHOW_CURSOR  (1<<23) //���ڹ���Ƿ���ʾ
#define WS_CAN_FOCUS    (1<<24) //�����Ƿ������Ϊfocus����
#define WS_UNFILL       (1<<25) //��������ʱ�Ƿ���䣬0=��䣬1=�����

//#define WS_MAIN_WINDOW  (WS_BORDER|WS_DLGFRAME|WS_CAPTION|WS_SYSMENU)


/*============================================================================*/

#define CN_WINDOW_ZPRIO_MOUSE       (-101)
#define CN_WINDOW_ZPRIO_CURSOR      (-100)

HDC     CreateDC(struct GkWinObj *gk_win,const RECT *prc);
bool_t    GDD_DeleteDC(HDC hdc);

void GDD_SetDrawArea(HDC hdc, struct Rectangle *DrawArea);
struct RopGroup GDD_SetRopCode(HDC hdc,struct RopGroup RopCode);
struct RopGroup GDD_GetRopCode(HDC hdc);
void   GDD_MoveTo(HDC hdc,s32 x,s32 y,POINT *old_pt);

u32 GDD_SetDrawColor(HDC hdc,u32 color);
u32 GDD_GetDrawColor(HDC hdc);
u32 GDD_SetFillColor(HDC hdc,u32 color);
u32 GDD_GetFillColor(HDC hdc);
u32 GDD_SetTextColor(HDC hdc,u32 color);
u32 GDD_GetTextColor(HDC hdc);
u32 GDD_SetSyncTime(HDC hdc,u32 sync_time);
u32 GDD_GetSyncTime(HDC hdc);

s32  GDD_GetStrLineCount(struct Charset *myCharset, const char *str);
void GDD_AdjustTextRect(HDC hdc,const char *text,int count, RECT *prc,u32 flag);

struct FontObj *GDD_SetFont(HDC hdc,HFONT hFont);
struct FontObj *GDD_GetFont(HDC hdc);
struct Charset *GDD_SetCharset(HDC hdc,struct Charset *pCharset);
struct Charset *GDD_GetCharset(HDC hdc);


void    GDD_SetPixel(HDC hdc,s32 x,s32 y,u32 color);
void    GDD_DrawLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1);
void    GDD_DrawDottedLine(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,s32 temp_draw,s32 temp_past);
void    GDD_DrawLineEx(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,u32 color);
void    GDD_DrawLineTo(HDC hdc,s32 x,s32 y);
bool_t  GDD_TextOut(HDC hdc,s32 x,s32 y,const char *text,s32 count);
bool_t  GDD_DrawText(HDC hdc,const char *text,s32 count,const RECT *prc,u32 flag);
void    GDD_FillTrangle(HDC hdc,s32 x0,s32 y0,s32 x1,s32 y1,s32 x2,s32 y2);
void    GDD_DrawRect(HDC hdc,const RECT *prc);;
void    GDD_FillRect(HDC hdc,const RECT *prc);
void    GDD_FillRectEx(HDC hdc,const RECT *prc,u32 color);
void    GDD_GradientFillRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2,u32 mode);
void    GDD_Fill3DRect(HDC hdc,const RECT *prc,u32 Color1,u32 Color2);

void    GDD_DrawCircle(HDC hdc,s32 cx,s32 cy,s32 r);
void    GDD_FillCircle(HDC hdc,s32 cx,s32 cy,s32 r);
void    GDD_DrawEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry);
void    GDD_FillEllipse(HDC hdc,s32 cx, s32 cy, s32 rx, s32 ry);
void    GDD_DrawSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2);
void    GDD_FillSector(HDC hdc, s32 xCenter, s32 yCenter, s32 radius,s32 angle1,s32 angle2);
void    GDD_DrawBezier3(HDC hdc,const POINT *pt,s32 cnt);

void    GDD_DrawPolyLine(HDC hdc,const POINT *pt,s32 count);
void    GDD_DrawGroupBox(HDC hdc,const RECT *prc,const char *Text);
bool_t  GDD_DrawBitmap(HDC hdc,s32 x,s32 y,struct RectBitmap *bitmap,u32 HyalineColor);

bool_t GDD_DrawWinBMPArray(HDC hdc,s32 x,s32 y,const void *bmp_data,u32 *palette,u32 HyalineColor);

//��Ϣ��غ���
u32     GDD_DispatchMessage(struct WindowMsg *pMsg);
u32     GDD_SendMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
bool_t  GDD_PostMessage(HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
bool_t  GDD_GetMessage(struct WindowMsg *pMsg,HWND hwnd,bool_t *SyncMsg);


HWND    GDD_GetDesktopWindow(char *display);
u32     GDD_GetWindowStyle(HWND hwnd);
ptu32_t   GDD_GetWindowPrivateData(HWND hwnd);
void    GDD_SetWindowPrivateData(HWND hwnd,ptu32_t data);
HWND    GDD_InitGddDesktop(struct GkWinObj *desktop);

//DC��������
HDC     GDD_GetWindowDC(HWND hwnd);
HDC     GDD_GetClientDC(HWND hwnd);
bool_t    GDD_ReleaseDC(HDC hdc);
HDC     GDD_BeginPaint(HWND hwnd);
bool_t    GDD_EndPaint(HWND hwnd,HDC hdc);

//���ڲ�������
void GDD_AddProcFuncTable(HWND hwnd,struct MsgTableLink *pNewMsgTableLink);
HWND    GDD_CreateWindow(const char *Text,u32 Style,
                     s32 x,s32 y,s32 w,s32 h,
                     HWND hParent,u16 WinId,
                     u32 BufProperty, ptu32_t pdata,
                     u16 PixelFormat,u32 BaseColor,
                     struct MsgTableLink *pUserMsgTableLink);
void    GDD_DestroyWindow(HWND hwnd);
void    GDD_DestroyAllChild(HWND hwnd);
bool_t    GDD_MoveWindow(HWND hwnd,s32 x,s32 y);
bool_t    GDD_OffsetWindow(HWND hwnd,s32 dx,s32 dy);
bool_t    GDD_IsWindowVisible(HWND hwnd);
bool_t    GDD_InvalidateWindow(HWND hwnd,bool_t bErase);
bool_t    GDD_SetWindowShow(HWND hwnd);
bool_t    GDD_SetWindowHide(HWND hwnd);
bool_t GDD_SetWindowRopCode(HWND hwnd, struct RopGroup RopCode);
bool_t GDD_SetWindowHyalineColor(HWND hwnd,u32 HyalineColor);
bool_t GDD_SetWindowFillColor(HWND hwnd,u32 FillColor);
bool_t GDD_GetWindowFillColor(HWND hwnd,u32 *pFillColor);
bool_t GDD_SetWindowTextColor(HWND hwnd,u32 TextColor);
bool_t GDD_GetWindowTextColor(HWND hwnd,u32 *pTextColor);
bool_t    EnableWindow(HWND hwnd,bool_t bEnable);
HWND    GDD_GetWindowParent(HWND hwnd);
HWND GDD_GetWindowChild(HWND hwnd);
HWND GDD_GetWindowPrevious(HWND hwnd);
HWND __GDD_GetWindowTwig(HWND hwnd);
HWND GDD_GetWindowNext(HWND hwnd);
HWND GDD_GetWindowFirst(HWND hwnd);
HWND GDD_GetWindowLast(HWND hwnd);
char    *GDD_GetWindowText(HWND hwnd);
HWND    GDD_GetDlgItem(HWND hwnd,u16 id);
void    SetWindowText(HWND hwnd,const char *text,s32 max_len);
HWND    GDD_GetWindowFromPoint(struct GkWinObj *desktop, POINT *pt);

//���κ������������
bool_t    GDD_GetWindowRect(HWND hwnd,RECT *prc);
bool_t    GDD_GetClientRect(HWND hwnd,RECT *prc);
bool_t    GDD_GetClientRectToScreen(HWND hwnd,RECT *prc);
s32     GDD_RectW(const RECT *prc);
s32     GDD_RectH(const RECT *prc);
void    GDD_SetRect(RECT *prc,s32 x,s32 y,s32 w,s32 h);
void    GDD_SetRectEmpty(RECT *prc);
bool_t    GDD_CopyRect(RECT *dst,const RECT *src);
bool_t    GDD_IsRectEmpty(const RECT *prc);
bool_t    GDD_OffsetRect(RECT *prc,s32 dx,s32 dy);
bool_t    GDD_MoveRect(RECT *prc,s32 x,s32 y);
bool_t    GDD_InflateRect(RECT *prc,s32 dx,s32 dy);
bool_t    GDD_InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b);
bool_t    GDD_PtInRect(const RECT *prc,const POINT *pt);
bool_t    GDD_ScreenToClient(HWND hwnd,POINT *pt,s32 count);
bool_t    GDD_ClientToScreen(HWND hwnd,POINT *pt,s32 count);
bool_t    GDD_ScreenToWindow(HWND hwnd,POINT *pt,s32 count);
bool_t    GDD_WindowToScreen(HWND hwnd,POINT *pt,s32 count);

/*===========================================================================*/
void    ModuleInstall_GDD(struct GkWinObj *desktop);

u32     GDD_AlphaBlendColor(u32 bk_c,u32 fr_c,u8 alpha);
void    GDD_SyncShow(HWND hwnd);//����ԭ�� �� GDD_UpdateDisplay
void GDD_WaitGuiAppExit(char *AppName);
HWND GDD_CreateGuiApp(char *AppName,struct MsgTableLink  *MyMsgLink,
                      s32 x,s32 y,s32 w,s32 h,
                      u32 MemSize, u32 WinBuf,u32 Style,u16 PixelFormat,u32 BaseColor);

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
