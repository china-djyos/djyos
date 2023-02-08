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
#include    "gdd_private.h"
#include <gui\gdd_focus.h>


static HWND sg_pMouseHwnd;

//----�����ƺ���-------------------------------------------------------------
//���ܣ����ǰ�ť�ؼ���MSG_PAINT��Ϣ��Ӧ����
//������pMsg����Ϣָ��
//���أ��̶�true
//-----------------------------------------------------------------------------
static  bool_t __GDD_MousePaint(struct WindowMsg *pMsg)
{
    HWND hwnd;
    HDC hdc;
    RECT rc;

    hwnd=pMsg->hwnd;
    hdc =GDD_BeginPaint(hwnd);
    if(NULL!=hdc)
    {
        GDD_GetClientRect(hwnd,&rc);
        GDD_SetFillColor(hdc,RGB(255,0,0));
        GDD_FillRect(hdc,&rc);

        GDD_SetDrawColor(hdc,RGB(1,1,1));
        GDD_DrawLine(hdc,0,4,8,4);
        GDD_DrawLine(hdc,4,0,4,8);

        GDD_EndPaint(hwnd,hdc);
    }
    return true;
}

//�����Ϣ�������������û���������û�д������Ϣ��
static struct MsgProcTable s_gMouseMsgProcTable[] =
{
    {MSG_CREATE,__GDD_MousePaint}
};

static struct MsgTableLink  s_gMouseWinMsgLink;

bool_t GDD_CreateMouseIcon( void )
{
    HWND desktop;
    struct Rectangle rc ;
    struct PointCdn sz;
    s32 x,y;
    struct RopGroup RopCode;
    desktop = GDD_GetDesktopWindow(NULL );
    GK_GetDspSize(desktop->pGkWin, &sz);
    GK_GetScreenArea(desktop->pGkWin, &rc);
    x = sz.x / 2 - rc.left;
    y = sz.y / 2 -rc.top;
    s_gMouseWinMsgLink.MsgNum = sizeof(s_gMouseMsgProcTable) / sizeof(struct MsgProcTable);
    s_gMouseWinMsgLink.myTable = (struct MsgProcTable *)&s_gMouseMsgProcTable;
    sg_pMouseHwnd = GDD_CreateWindow("Mouse_Cursor", 0, x,y, 8, 8, NULL, 0,
                                  CN_WINBUF_BUF, 0, CN_SYS_PF_DISPLAY, CN_COLOR_WHITE,
                                  &s_gMouseWinMsgLink);
    if(sg_pMouseHwnd != NULL)
    {
       GK_SetPrio(sg_pMouseHwnd->pGkWin,CN_WINDOW_ZPRIO_MOUSE , CN_TIMEOUT_FOREVER);
//         GK_SetHyalineColor(sg_pMouseHwnd->pGkWin,RGB(0, 0, 0));
       RopCode = (struct RopGroup){ 0, 0, 0, CN_R2_XORPEN, 0, 0, 0  };
       GK_SetRopCode(sg_pMouseHwnd->pGkWin, RopCode, CN_TIMEOUT_FOREVER);
       return true;
    }
    else
        return false;
}


