//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// =============================================================================

// �ļ���     ��
// ģ������:
// ģ��汾: V1.00
// ������Ա:
// ����ʱ��:
// =============================================================================
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "os.h"

typedef struct _PIC_TYPE_
{
    struct _PIC_TYPE_ *Next;
    u32 IntLine;
    u32 IntSub;
    u32 *IntEn;
    u32 *IntSts;
    u32 (*isr)(ptu32_t);
}tagPic;

tagPic *sPicHead = NULL;

static void Pic_NodeInsert(tagPic *node)
{
    tagPic *temp = sPicHead;
    while(temp->Next != NULL)
    {
        temp = temp->Next;
    }
    temp->Next = node;
    node->Next = NULL;
}

static tagPic* Pic_FindLineNode(tagPic *pic,u8 intline)
{
    tagPic *temp = pic;
    while(temp != NULL)
    {
        if(temp->IntLine == intline)
        {
            break;
        }
        else
        {
            temp = temp->Next;
        }
    }
    return temp;
}
static tagPic* Pic_FindSubInt(tagPic *pic,u8 intline,u8 subint)
{
    tagPic *temp = pic;
    while(temp != NULL)
    {
        if((temp->IntLine == intline) && \
                (temp->IntSub == subint))
        {
            break;
        }
        else
        {
            temp = temp->Next;
        }
    }
    return temp;
}

static bool_t Pic_IsrHandle(u8 intline)
{
    tagPic *pic;

    pic = sPicHead;
    while(1)
    {
        pic = Pic_FindLineNode(pic,intline);
        if(pic == NULL)
        {
            break;
        }

        if(*(pic->IntSts) & (1<<pic->IntSub))
        {
            pic->isr(pic->IntSub);
        }
        pic = pic->Next;
    }

    return true;
}
bool_t Pic_IntRegister(u8 intline,u8 subint,u32* EnAddr,\
                        u32* StsAddr,u32 (*isr)(ptu32_t))
{
    bool_t Ret = false;
    tagPic *pic,*temp;

    if(NULL == isr)
    {
        return false;
    }

    temp = Pic_FindLineNode(sPicHead,intline);
    if(NULL == temp)
    {
        Int_Register(intline);
        Int_SetClearType(intline,CN_INT_CLEAR_AUTO);
        Int_IsrConnect(intline,Pic_IsrHandle);
        Int_SettoAsynSignal(intline);
        Int_ClearLine(intline);
        Int_RestoreAsynLine(intline);
    }

    pic = malloc(sizeof(tagPic));
    if(NULL != pic)
    {
        pic->IntLine = intline;
        pic->IntSub  = subint;
        pic->IntEn   = EnAddr;
        pic->IntSts  = StsAddr;
        pic->isr     = isr;
        pic->Next    = NULL;

        if(sPicHead == NULL)
        {
            sPicHead = pic;
        }
        else
        {
            Pic_NodeInsert(pic);
        }
        Ret = true;
    }
    return true;
}

bool_t Pic_IntEnable(u8 intline,u8 subint)
{
    bool_t Ret = false;
    tagPic *pic;

    pic = Pic_FindSubInt(sPicHead,intline,subint);
    if(NULL != pic)
    {
        *(pic->IntEn) |= 1<<subint;
        Ret = true;
    }
    return Ret;
}

bool_t Pic_IntDisable(u8 intline,u8 subint)
{
    bool_t Ret = false;
    tagPic *pic;

    pic = Pic_FindSubInt(sPicHead,intline,subint);
    if(NULL != pic)
    {
        *(pic->IntEn) &= ~(1<<subint);
        Ret = true;
    }
    return Ret;
}


