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
#include <lock.h>
#include <gui/gk_display.h>
#include <hmi-input.h>
#include    "gdd_private.h"
#include "dbug.h"
static tpInputMsgQ sg_ptGddMsgQ;


//-----------------------------------------------------------------------------
//���ܣ���������豸�����ڽ�������HMI�豸��������Ϣ��
//����������GDD�������豸����ע�⣬���������GDD�������豸�����������
//���أ�true = �ɹ���ʼ����false =ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t GDD_AddInputDev(const char *InputDevName)
{
    bool_t flag;
    enum _STDIN_INPUT_TYPE_ DevType;
    flag = HmiIn_SetFocus((char*)InputDevName,sg_ptGddMsgQ);
    DevType = HmiIn_CheckDevType(InputDevName);
    switch(DevType)
    {
        case EN_HMIIN_MOUSE_2D:
            GDD_CreateMouseIcon( );
            break;
        case EN_HMIIN_MOUSE_3D:
            break;
        case EN_HMIIN_SINGLE_TOUCH:
            break;
        case EN_HMIIN_MUTI_TOUCH:
            break;
        case EN_HMIIN_AREA_TOUCH:
            break;
        default:break;
    }
    return flag;
}

//-----------------------------------------------------------------------------
//���ܣ������豸�����뽹��� �豸������ɾ���� ����ΪĬ��ֵ
//����������GDD�������豸����
//���أ�true = �ɹ���ʼ����false =ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t GDD_DeleteInputDev(const char *InputDevName)
{
    bool_t flag;
    flag = HmiIn_SetFocus((char*)InputDevName,HmiIn_GetFocusDefault());
    return flag;
}

//----�����豸��ʼ��-----------------------------------------------------------
//���ܣ����������豸���У����ڽ�������HMI�豸��������Ϣ��
//����������GDD�������豸���б�ע�⣬���������GDD�������豸�����������
//���أ�true = �ɹ���ʼ����false =ʧ�ܡ�
//-----------------------------------------------------------------------------
bool_t GDD_InputDevInit(void)
{
    GDD_StartTimer(GDD_CreateTimer(GDD_GetDesktopWindow( ), CN_HMIINPUT_TIMER_ID, 30));

    sg_ptGddMsgQ = HmiIn_CreatInputMsgQ(20);

    if(sg_ptGddMsgQ == NULL)
    {
        debug_printf("gdd","create HmiInput Msgqueue error\n\r");
        return false;
    }
    else
    {
        return true;
    }
}

void GDD_HmiInput(void)
{
    HWND hwnd;
    struct InputDeviceMsg msg;

    if(HmiIn_ReadMsg(sg_ptGddMsgQ, &msg, 0) == true)
    {
        //���㴥������Ϣ��ģ��������Ϣ
        if(msg.input_type == EN_HMIIN_SINGLE_TOUCH)
        {
            struct SingleTouchMsg *TouchMsg;
            static s32 z=0;
            u16 Touch_Msg;
            bool_t NC;
            POINT pt;
            RECT rc;
            TouchMsg = &msg.input_data.SingleTouchMsg;
            pt.x = TouchMsg->x;
            pt.y = TouchMsg->y;
            __GDD_Lock();
            //TouchDown���ڴ���.
            if(TouchMsg->display != NULL)
                hwnd = GDD_GetWindowFromPoint(TouchMsg->display->desktop, &pt);
            else
                hwnd = GDD_GetWindowFromPoint(GDD_GetDesktopWindow()->pGkWin, &pt);
            //GDD_MoveWindow(sg_pMouseHwnd, pt.x-4, pt.y-4);
//          GDD_UpdateDisplay(CN_TIMEOUT_FOREVER);
            if(hwnd != NULL)
            {
                GDD_GetClientRectToScreen(hwnd,&rc);
                if(GDD_PtInRect(&rc, &pt))
                    NC = false;
                else
                    NC = true;

                z = TouchMsg->z;
                if(z>0)     //touch���»��߻���
                {
                    if((TouchMsg->MoveX ==0) && (TouchMsg->MoveY ==0))
                    {
                        if(NC)
                            Touch_Msg = MSG_NCTOUCH_DOWN;
                        else
                            Touch_Msg = MSG_TOUCH_DOWN;
                    }
                    else
                    {
                        if(NC)
                            Touch_Msg = MSG_NCTOUCH_MOVE;
                        else
                            Touch_Msg = MSG_TOUCH_MOVE;
                    }
                }
                else        //touch�뿪
                {
                    if(NC)
                        Touch_Msg = MSG_NCTOUCH_UP;
                    else
                        Touch_Msg = MSG_TOUCH_UP;
                }
                GDD_PostMessage(hwnd, Touch_Msg,
                            ((u16)(TouchMsg->MoveY) << 16) | (u16)(TouchMsg->MoveX),
                            ((u16)(pt.y) << 16) | (u16)(pt.x));

            }
            __GDD_Unlock();
        }

        if(msg.input_type == EN_HMIIN_KEYBOARD)
        {
            u8* key;
            u8 val,event;
            u32 KeyTime;

            hwnd = GDD_GetFocusWindow();
            if(NULL != hwnd)
            {
                key  = msg.input_data.key_board.key_value;
                KeyTime = msg.input_data.key_board.time;

                val  =key[0];
                event =key[1];

                if(event==0x00)
                {
                    GDD_PostMessage(hwnd, MSG_KEY_DOWN, val, KeyTime);
                }

                if(event==0xF0)
                {
                    GDD_PostMessage(hwnd, MSG_KEY_UP, val, KeyTime);
                }
            }
        }


    }

    return;
}

