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

HWND HWND_Focus=NULL;

//���㴰���㷨��1����ʼ����focus = desktop�����ж��desktop�����һ����
//2��ɾ������ʱ�����㽻����һ���ɻ�ý�����ֵܴ��ڣ������ÿա�
//3��������ΪNULL����cursor���ڼĴ��������أ���Ϊ���ز��ŵ�desktop�ϡ�
//----���ý��㴰��-------------------------------------------------------------
//����: ��һ��������Ϊ���㴰�ڣ�����Ϊ��괰�ڵĸ����ڡ�
//������hwnd: �µĽ��㴰��
//���أ�true = success, false = failure.
//-----------------------------------------------------------------------------
bool_t GDD_SetFocusWindow(HWND hwnd)
{
    HWND wnd=NULL;
    if(! GDD_IsFocusEnable(hwnd))
        return false;
    if(__GDD_Lock())
    {
        wnd =HWND_Focus;
        HWND_Focus =hwnd;
        GDD_CursorSetHost(hwnd);
        if( GDD_CursorCheckStatus(hwnd) )
            GDD_CursorSetShow();
        else
            GDD_CursorSetHide();
        __GDD_Unlock();
    }
    else
    {
        return false;
    }

    if(wnd!=NULL)
    {
        GDD_SendMessage(wnd,MSG_KILLFOCUS,0,0);
    }
    if(hwnd!=NULL)
    {
        GDD_SendMessage(hwnd,MSG_SETFOCUS,0,0);
    }

    return true;

}

//----��ý��㴰��--------------------------------------------------------------
//����: ��.
//��������.
//���أ����㴰��.
//------------------------------------------------------------------------------
HWND    GDD_GetFocusWindow(void)
{
    HWND wnd=NULL;

    if(__GDD_Lock())
    {
        wnd =HWND_Focus;
        __GDD_Unlock();
    }
    return wnd;
}

//----�ж��Ƿ�������Ϊ����-----------------------------------------------------
//���ܣ��ж�һ�������Ƿ�������Ϊ���뽹��
//������hwnd�����ھ��
//���أ�true = ����, false = ������.
//-----------------------------------------------------------------------------
bool_t GDD_IsFocusEnable(HWND hwnd)
{
    if((hwnd->Style & WS_CAN_FOCUS) == WS_CAN_FOCUS)
        return true;
    else
        return false;
}

//----�ж��Ƿ�Ϊ���㴰��-------------------------------------------------------
//����: ��.
//��������.
//���أ����ָ���Ĵ����ǽ��㴰��,������TRUE,���򷵻�FALSE.
//------------------------------------------------------------------------------
bool_t  GDD_IsFocusWindow(HWND hwnd)
{
    bool_t res=FALSE;

    if(__GDD_Lock())
    {
        if(hwnd == HWND_Focus)
        {
            res=TRUE;
        }
        __GDD_Unlock();
    }
    return res;

}

//----�Ƿ񽹵�����-------------------------------------------------------------
//���ܣ����һ�������Ƿ񽹵㴰�ڻ������ȴ���
//������hwnd�������Ĵ���
//���أ�true = �����ȴ��ڣ�false = ����
//-----------------------------------------------------------------------------
bool_t GDD_IsFocusAncestor(HWND hwnd)
{
    HWND desktop,current;
    bool_t result = false;
    if(hwnd == NULL)
       return false;
    current = GDD_GetFocusWindow();
    desktop = GDD_GetDesktopWindow( );
    while(1)
    {
        if(hwnd == current)
        {
            result = true;
            break;
        }
        if(current == desktop)
        {
            result = false;
            break;
        }
        current = GDD_GetWindowParent(current);
    }
    return result;
}



