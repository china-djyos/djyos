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
//�ļ�����: �������㺯����
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  LiuWei.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include    "gdd_private.h"

//----ȡ���εĿ�----------------------------------------------------------------
//���ܣ�ȡһ�����εĿ��
//������prc������ָ��
//���أ����εĿ��
//-----------------------------------------------------------------------------
s32 GDD_RectW(const RECT *prc)
{
    return (prc->right - prc->left);
}

//----ȡ���εĸ�----------------------------------------------------------------
//���ܣ�ȡһ�����εĸ߶�
//������prc������ָ��
//���أ����εĸ߶�
//-----------------------------------------------------------------------------
s32 GDD_RectH(const RECT *prc)
{
    return (prc->bottom - prc->top);
}

//----���þ��γߴ�-----------------------------------------------------------
//����: ��
//������x,y������λ��
//      w,h�����ο��
//���أ����ڶ����ڴ��ڴ�ָ��
//------------------------------------------------------------------------------
void    GDD_SetRect(RECT *prc,s32 x,s32 y,s32 w,s32 h)
{
    if(prc!=NULL)
    {
        prc->left   = x;
        prc->top    = y;
        if(w < 0)
            prc->right = x;
        else
            prc->right  = x+w;
        if(h < 0)
            prc->bottom = y;
        else
            prc->bottom = y+h;
    }
}

//----��վ��γߴ�-----------------------------------------------------------
//����: ��
//��������
//���أ����ڶ����ڴ��ڴ�ָ��
//------------------------------------------------------------------------------
void    GDD_SetRectEmpty(RECT *prc)
{
    if(prc!=NULL)
    {
        prc->left   = 0;
        prc->top    = 0;
        prc->right  = 0;
        prc->bottom = 0;
    }
}

//------------------------------------------------------------------------------
//���ܣ��������Ƿ�գ����=0��
//������prc�������ľ���
//���أ�true = �����Ϊ0��false= ���Ϊ0
bool_t    GDD_IsRectEmpty(const RECT *prc)
{
    if(prc!=NULL)
    {
        if((prc->right - prc->left)<=0)
        {
            return FALSE;
        }
        if((prc->bottom - prc->top)<=0)
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
//���ܣ�����ƫ�ƣ���һ�����ε������ƶ���dx��dy��
//������prc����һ���ľ���
//      dx,dy��ƫ����
//���أ�true= �ɹ���false=ʧ��
//------------------------------------------------------------------------------
bool_t    GDD_OffsetRect(RECT *prc,s32 dx,s32 dy)
{
    if(NULL!=prc)
    {
        prc->left += dx;
        prc->top  += dy;
        prc->right += dx;
        prc->bottom  += dy;
        return TRUE;
    }
    return FALSE;
}

//-----------------------------------------------------------------------------
//���ܣ������ƶ�����һ�����ε������ƶ���Ŀ�����꣨x��y��
//������prc���������ľ��Ρ�
//      x,y��Ŀ������
//���أ�true= �ɹ���false=ʧ��
//------------------------------------------------------------------------------
bool_t    GDD_MoveRect(RECT *prc,s32 x,s32 y)
{
    if(NULL!=prc)
    {
        prc->right += x - prc->left;
        prc->bottom  += y - prc->top;
        prc->left = x;
        prc->top  = y;
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
//���ܣ����α߿�Ŵ�dx��dy������dx��dy������һ���Ǹ�������Ӧ�߿�����С
//������prc���������ľ��Ρ�
//      dx��x����Ŵ���������ͬ���Ŵ�ʵ�ʷŴ�����2dx
//      dy��y����Ŵ���������ͬ���Ŵ�ʵ�ʷŴ�����2dy
//���أ�true= �ɹ���false=ʧ��
//------------------------------------------------------------------------------
bool_t    GDD_InflateRect(RECT *prc,s32 dx,s32 dy)
{
    if(NULL!=prc)
    {
        prc->left   -= dx;
        prc->top    -= dy;
        prc->right  += dx;
        prc->bottom += dy;
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
//���ܣ������ĸ��߿�ֱ�Ŵ�������һ���Ŵ����Ǹ�������Ӧ�߿�����С
//������prc���������ľ��Ρ�
//      l����߿�Ŵ���
//      t���ϱ߿�Ŵ���
//      r���ұ߿�Ŵ���
//      b���±߿�Ŵ���
//���أ�true= �ɹ���false=ʧ��
//------------------------------------------------------------------------------
bool_t GDD_InflateRectEx(RECT *prc,s32 l,s32 t,s32 r,s32 b)
{
    if(NULL!=prc)
    {
        prc->left   -= l;
        prc->top    -= t;
        prc->right  += r;
        prc->bottom += b;
        return TRUE;
    }
    return FALSE;
}

//------------------------------------------------------------------------------
//���ܣ���������Ƿ��ھ����ڲ�
//������prc���������ľ��Ρ�
//      pt����������������
//���أ�true = ���ڲ���false = �����ڲ�
//------------------------------------------------------------------------------
bool_t    GDD_PtInRect(const RECT *prc,const POINT *pt)
{
    if((NULL==prc) || (NULL==pt))
        return FALSE;
    else
    {
        if(pt->x < prc->left)
            return FALSE;
        if(pt->x >= prc->right)
            return FALSE;

        if(pt->y < prc->top)
            return FALSE;
        if(pt->y >= prc->bottom)
            return FALSE;
        return TRUE;
    }
}

/*============================================================================*/


