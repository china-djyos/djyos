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
//����:  luost.
//�汾��V1.0.0
//�ļ�����: gdd�����ʾ
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2017-01-14
//   ����:  luost.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include <stdint.h>
#include <gui/gdd/gdd_private.h>
#include <gui\gdd_cursor.h>


#define CN_CURSOR_FLASH_TIME     800            //�����˸ʱ�䣨ms��
HWND g_ptCursorHwnd=NULL;                         //��괰��
struct WinTimer* sg_tpCursorTimer;

//----��괴������-------------------------------------------------------------
//���ܣ����ǰ�ť�ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static bool_t Cursor_Create(struct WindowMsg *pMsg)
{
    HWND hwnd;
    s32 x,y;
    RECT rc;
    HDC hdc;
    if(pMsg==NULL)
       return false;
    hwnd=pMsg->hwnd;
    if(hwnd==NULL)
         return false;
    hdc =BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GetClientRect(hwnd,&rc);
        x=RectW(&rc);
        y=RectH(&rc);
        SetFillColor(hdc,RGB(255,0,0));
        FillRect(hdc,&rc);
        SetDrawColor(hdc,RGB(1,1,1));
        DrawLine(hdc,x/2,0,x/2,y);
        EndPaint(hwnd,hdc);
    }
    return true;
}

//----���������-------------------------------------------------------------
//���ܣ����һ�������Ƿ���ʾ���
//������hwnd�������Ĵ���
//���أ�true = ��ʾ��꣬false = ����ʾ���
//-----------------------------------------------------------------------------
bool_t Cursor_CheckStatus(HWND hwnd)
{
   if(hwnd->Style & WS_SHOW_CURSOR)
       return true;
   else
       return false;
}

//----�����˸����-------------------------------------------------------------
//���ܣ���Ӧ��괰�ڵĶ�ʱ����Ϣ
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static bool_t Cursor_Flash(struct WindowMsg *pMsg)
{
    if(IsWindowVisible(g_ptCursorHwnd))
    {
         SetWindowHide(g_ptCursorHwnd);
         UpdateDisplay(CN_TIMEOUT_FOREVER);
    }
    else
    {
         SetWindowShow(g_ptCursorHwnd);
         UpdateDisplay(CN_TIMEOUT_FOREVER);
    }
    return true;
}

//----��ʾ���-----------------------------------------------------------------
//���ܣ����ù�괦�ڿ���ʾ״̬
//������
//���أ�
//-----------------------------------------------------------------------------
void Cursor_SetShow( void )
{
    SetWindowShow(g_ptCursorHwnd);
    GDD_StartTimer( sg_tpCursorTimer );
}

//----���ع��-----------------------------------------------------------------
//���ܣ����ù�괦������״̬
//������
//���أ�
//-----------------------------------------------------------------------------
void Cursor_SetHide( void )
{
    SetWindowHide(g_ptCursorHwnd);
    GDD_StopTimer( sg_tpCursorTimer );
}

//----�ƶ����-----------------------------------------------------------------
//���ܣ����λ������ֵ�޸ģ����ӻ���٣���
//������deltaX��deltaY����������ֵ��
//���أ���
//-----------------------------------------------------------------------------
void Cursor_Offset( s32 deltaX , s32 deltaY )
{
    OffsetWindow(g_ptCursorHwnd, deltaX, deltaY);
}

//----�ƶ����-----------------------------------------------------------------
//���ܣ��ƶ���굽��λ�ã���������ڸ�����
//������absX��absY���µ�����
//���أ���
//-----------------------------------------------------------------------------
void Cursor_Move( s32 absX , s32 absY )
{
    MoveWindow(g_ptCursorHwnd, absX, absY);
}

//----���ù�����������-------------------------------------------------------
//���ܣ����⣬�������һ���潹�㴰�ڸı䡣
//������HostWin���µ���������
//���أ���
//-----------------------------------------------------------------------------
void Cursor_SetHost(HWND HostWin)
{
//    GK_AdoptWin(g_ptCursorHwnd->pGkWin, HostWin->pGkWin);
    GDD_AdoptWin( g_ptCursorHwnd , HostWin);
}
//�����Ϣ������������
static struct MsgProcTable s_gCursorMsgProcTable[]=
{
    {MSG_CREATE,Cursor_Create},
    {MSG_TIMER,Cursor_Flash},
};

static struct MsgTableLink  s_gCursorMsgLink;

bool_t Cursor_Init(void)
{
    struct RopGroup RopCode;

    s_gCursorMsgLink.MsgNum = sizeof(s_gCursorMsgProcTable) / sizeof(struct MsgProcTable);
    s_gCursorMsgLink.myTable = (struct MsgProcTable *)&s_gCursorMsgProcTable;
    g_ptCursorHwnd = CreateWindow("Cursor",WS_CHILD,0,0,
                        2, 12,NULL, 0, CN_WINBUF_PARENT,NULL,&s_gCursorMsgLink);
    if(g_ptCursorHwnd!=NULL)
    {
         GK_SetPrio(g_ptCursorHwnd->pGkWin,CN_WINDOW_ZPRIO_CURSOR , CN_TIMEOUT_FOREVER);
         RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_XORPEN, 0, 0, 0  };
         GK_SetRopCode(g_ptCursorHwnd->pGkWin, RopCode, CN_TIMEOUT_FOREVER);

         sg_tpCursorTimer =GDD_CreateTimer(g_ptCursorHwnd,0,CN_CURSOR_FLASH_TIME);
         GDD_StartTimer(sg_tpCursorTimer);
         if(sg_tpCursorTimer == NULL)
         {
             DestroyWindow(g_ptCursorHwnd);
             return false;
         }
         SetWindowHide(g_ptCursorHwnd);
         return true;
     }
    return false;
}

