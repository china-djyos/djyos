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
//�ļ�����: GDD�ں�˽��ͷ�ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_PRIVATE_H__
#define __GDD_PRIVATE_H__

#if __cplusplus
extern "C" {
#endif

#include    "stddef.h"
#include    "stdio.h"
#include    "stdlib.h"
#include    "string.h"
#include    "math.h"
#include    "list.h"
#include    "systime.h"
#include    "lock.h"
#include    "djyos.h"
#include    "hmi-input.h"
#include    "gkernel.h"

#include    "gdd.h"

/*============================================================================*/

#define DEF_BORDER_SIZE     1   //Ĭ�ϵı߿��С
#define DEF_DLGFRAME_SIZE   3   //Ĭ�ϵĶԻ��߿��С
#define DEF_CAPTION_SIZE    28  //Ĭ�ϵı�������С


//#define CN_DEF_FILL_COLOR           RGB(255,255,255)   //Ĭ�ϴ��������ɫ
//#define CN_DEF_DRAW_COLOR           RGB(0, 0, 0)  //Ĭ�ϴ��ڻ�����ɫ
//#define CN_DEF_TEXT_COLOR           RGB(0, 0, 0)  //Ĭ�ϴ����ı���ɫ

#define CN_DEF_FILL_COLOR           RGB(100, 20, 100)   //Ĭ�ϴ��������ɫ
#define CN_DEF_DRAW_COLOR           RGB(255, 255, 255)  //Ĭ�ϴ��ڻ�����ɫ
#define CN_DEF_TEXT_COLOR           RGB(255, 255, 255)  //Ĭ�ϴ����ı���ɫ


#define WINDOW_BORDER_COLOR         RGB(200,200,200)    //���ڱ߿���ɫ
#define WINDOW_DLGFRAME_COLOR1      RGB(70,70,100)      //���ڶԻ��߿���ɫ1
#define WINDOW_DLGFRAME_COLOR2      RGB(70,70,220)      //���ڶԻ��߿���ɫ2
#define WINDOW_DLGFRAME_COLOR3      RGB(70,70,90)       //���ڶԻ��߿���ɫ3
#define WINDOW_CAPTION_TEXT_COLOR   RGB(255,255,255)    //���ڱ�����������ɫ


typedef struct DC       DC;
struct WinMsgQueueCB;

/*============================================================================*/

#define HDC2DC(hdc) (struct DC*)(hdc)
#define DC2HDC(dc)  (HDC)dc


/*============================================================================*/

#define DC_TYPE_PAINT   0
#define DC_TYPE_CLIENT  1
#define DC_TYPE_WINDOW  2

struct DC
{
    struct GkWinObj *pGkWin;
    struct FontObj *pFontDef;   //Ĭ������,���ֵ�ڴ���DCʱָ��,���������ɸ���
    struct FontObj *pFont;      //��ǰ����,���ֵ��DCʹ�ù�����,���Ա�����
    struct Charset *pCharset;   //��ǰ�ַ���
    HWND hwnd;                  //DC��������
    u32 DCType;                 //DC����
    s32 CurX,CurY;              //��ǰλ��
    u32 DrawColor;              //������ɫ
    u32 FillColor;              //�����ɫ
    u32 TextColor;              //������ɫ
    u32 SyncTime;               //ͬ��ʱ��
    u32 HyalineColor;           //͸����ʾ͸��ɫ��RopCode����͸��ɫʱ��,RGB888
    struct RopGroup RopCode;    //��դ��

};

#define WF_NCPAINT      (1<<0)  //�ǿͻ�������
#define WF_PAINT        (1<<1)  //�ͻ�������
#define WF_ERASEBKGND   (1<<2)  //��������

struct WINDOW
{
    //���ڣ�һ���ǿؼ�����˽����Ϣ��������
    //����Ա��ʵ�ִ�����Ϣ�̳л��ƵĹؼ�
    struct MsgTableLink **MyMsgTableLink;
//  list_t MsgProcFuncTable;
    char    *Text;
    struct MutexLCB *mutex_lock;        //���ڻ�����
    struct WinMsgQueueCB *pMsgQ;        //������Ϣ����
    struct GkWinObj *pGkWin;   //�̳�GkWin
    ptu32_t PrivateData;      //����˽������

    u16     EventID;        //���������¼�ID
    u16     WinId;          //����ID

    u8      BorderSize;     //�߿��С
    u8      CaptionSize;    //��������С
    u16     Flag;           //���ڱ�ǣ����磺WF_ERASEBKGND
    u32     Style;          //���ڷ��,��16λϵͳʹ�ã����磺WS_CHILD
                            //0~7 bit=�ؼ����ͣ�8~15 bit = �ؼ����
    RECT    CliRect;        //���ڿͻ���(ʹ����Ļ����)
    u32     DrawColor;      //������ɫ�������Ӵ���ʱ�����̳�
    u32     FillColor;      //�����ɫ�������Ӵ���ʱ�����̳�
    u32     TextColor;      //������ɫ�������Ӵ���ʱ�����̳�

    list_t  list_timer;         //���ڶ�ʱ������
    list_t  node_msg_close;     //����CLOSE��Ϣ����ڵ�
    list_t  node_msg_ncpaint;   //����NCPAINT��Ϣ����ڵ�
    list_t  node_msg_paint;     //����PAINT��Ϣ����ڵ�
};

struct WinTimer
{
//    u32 Tag ;
    HWND hwnd;              //��ʱ����������.
    u16 Id;                 //��ʱ��Id.
//    u16 Flag;               //��ʱ����ǣ��ο�TMR_START�ȶ���
    u32 Interval;           //��ʱ���(��λ:mS).
    s64 Alarm;             //��һ�ε���ʱ�䣨uS��.
    list_t  node_sys;       //ϵͳ��ʱ������ڵ�.
    list_t  node_hwnd;      //�������ڵĶ�ʱ������ڵ�.
    list_t  node_msg_timer; //TIMER��Ϣ�ڵ�,������MSG_TIMER��Ϣʱ,�ýڵ���뵽
                            //�������ڵ���Ϣ�����е�list_timer��.
};

#define CN_HMIINPUT_TIMER_ID     0xff01         //HMI����ɨ�趨ʱ��
/*============================================================================*/
void    GDD_Init(void);

bool_t  __HWND_Lock(HWND hwnd);
void    __HWND_Unlock(HWND hwnd);
bool_t  __GDD_Lock(void);
void    __GDD_Unlock(void);
bool_t GDD_AdoptWin(HWND Hwnd ,HWND NewParent);
void    GDD_InitDC(DC *pdc,struct GkWinObj *gk_win,HWND hwnd,s32 dc_type);
void    __GDD_OffsetRect(RECT *prc,s32 dx,s32 dy);
void    __GDD_InflateRect(RECT *prc,s32 dx,s32 dy);
void    __GDD_InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b);
bool_t  __PtInRect(const RECT *prc,const POINT *pt);


bool_t  __GDD_PostMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,u32 msg,u32 param1,ptu32_t param2);
void    __GDD_PostTimerMessage(struct WinMsgQueueCB *pMsgQ,HWND hwnd,struct WinTimer *ptmr);

void    __GDD_InvalidateWindow(HWND hwnd,bool_t bErase);
struct WinMsgQueueCB*   __GUI_CreateMsgQ(s32 size);
void    __GUI_DeleteMsgQ(struct WinMsgQueueCB *pMsgQ);
void    __GDD_RemoveWindowTimer(HWND hwnd);
void    __GDD_DeleteMainWindowData(HWND hwnd);
void __GDD_DeleteChildWindowData(HWND hwnd);

struct WinMsgQueueCB *__GDD_GetWindowMsgQ(HWND hwnd);
bool_t __GDD_WinMsgProc(struct WindowMsg *pMsg);
u32         __GDD_GetWindowEvent(HWND hwnd);
ptu32_t GDD_GetGK_Message(void);

/*============================================================================*/


#if __cplusplus
}
#endif
#endif  /*__WDD_PRIVATE_H__*/
