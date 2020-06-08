//-----------------------------------------------------------------------------
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
#include <stddef.h>
#include <stdlib.h>
#include <device.h>
#include <stdio.h>
#include "usb.h"


extern s32 USBH_HID_TouchGetData(USBH_HandleTypeDef *pHost, u8 *pButton, u32 *pX, u32 *pY);
USBH_HandleTypeDef *pTouch;
static char *touchname = "usb touch";
// ============================================================================
// ���ܣ����������豸�ļ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __TOUCH_DriRead(struct objhandle *pUSB, u8 *data, u32 dwLen, u32 wdOffset, u32 dwTimeout)
{
    s32 res;
    u8 *pBuf = data;

    if((9 != dwLen) || (0 != wdOffset) || (!pUSB)) // ����touch������ý�壬ÿ�ζ��Ĵ�С���ǹ̶���
        return (0);

    res = USBH_HID_TouchGetData((void*)Device_GetDrvTag(Handle2fd(pUSB)), pBuf, (u32*)(pBuf+1), (u32*)(pBuf+5));
    if(-1 == res)
        return (0);

    return (9);
}

// ============================================================================
// ���ܣ���������
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 TOUCH_DirectRead(u8 *pButton, u32 *pX, u32 *pY)
{
    s32 res;

    if(!pTouch)
    {
        printf("\r\n: erro : usb    : touch device do not exist.");
        return (0);
    }

    res = USBH_HID_TouchGetData(pTouch, pButton, pX, pY);
    if(-1 == res)
        return (0);

    return (9);
}

// ============================================================================
// ���ܣ��������豸�Ƿ����
// ������
// ���أ�0 -- �豸������ -1 -- �豸δ������
// ��ע��
// ============================================================================
s32 TOUCH_DeviceReady(void)
{
    return (USBH_Query(pTouch));
}

// ============================================================================
// ���ܣ�ע�ᴥ����
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 USBH_ResigerDevTouch(void *pParam)
{

    if(NULL == Device_Create((const char*)touchname, NULL, NULL,NULL,
            __TOUCH_DriRead, NULL, (ptu32_t)pParam))
    {
        printf("\r\n: erro : usb    : register touch device failed.");
        return (-1); // register failure
    }

    return (0); // register failure
}

// ============================================================================
// ���ܣ�ע��������
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 USBH_UnResigerDevTouch(void)
{
    if(Device_DeleteByName(touchname))
    {
        printf("\r\n: erro : usb    : unregister touch device failed.\r\n");
        return (-1); // register failure
    }

    return (0);
}

// ============================================================================
// ���ܣ����ô�����
// ������
// ���أ�
// ��ע��
// ============================================================================
void USBH_SetTouch(void *pHost)
{
    if((pTouch) || (!pHost))
    {
        printf("\r\n: erro : usb    : cannot set touch device.\r\n");
        return ;
    }

    if(!USBH_ResigerDevTouch(pHost))
        pTouch = pHost;
}

// ============================================================================
// ���ܣ����ô�����
// ������
// ���أ�
// ��ע��
// ============================================================================
void USBH_ResetTouch(void *pHost)
{
    if(pHost == pTouch)
    {
        if(!USBH_UnResigerDevTouch())// ע���豸
            pTouch = NULL;
    }
}
