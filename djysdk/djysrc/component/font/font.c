//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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

//����ģ��: ��������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: ������Դ��ʼ��
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2009-11-21
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "object.h"
#include "../include/font/font.h"
#include "../include/font/ascii8x8.h"
#include "../include/font/ascii8x16.h"
#include "../include/font/gb2312_16.h"
#include "string.h"
#include <cfg/local_config.h>

static struct FontRsc *s_ptCurFont;


//----��ȡ������Դ-------------------------------------------------------------
//����: �����������ƻ�ȡ������Դ��"C"��Ĭ������Ĵ��š�
//����: ��������
//����: ƥ������壬��δ��ƥ����룬ʹ��Ĭ�ϵ�ASCII����
//-----------------------------------------------------------------------------
struct FontRsc* Font_GetLocFont(const char* font_name)
{
    struct FontRsc* font;

    if(strcmp(font_name, "C") == 0){

        font = Font_SearchFont(gc_pCfgDefaultFontName);

    }else
    {
        font = Font_SearchFont(font_name);
    }

    return font;
}

//----��װ����-----------------------------------------------------------------
//����: �������尲װ��ϵͳ��Դ������
//����: font��������������Դָ��
//      name������������
//����: true
//-----------------------------------------------------------------------------
bool_t Font_InstallFont(struct FontRsc *font,const char* name)
{
    struct Object *rsc;
    rsc = OBJ_SearchTree(CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return false;       //������Դ�����δ����

    OBJ_AddChild(rsc,&font->node,sizeof(struct FontRsc),RSC_FONT,name);
    if(s_ptCurFont == NULL)
        s_ptCurFont = font;

    return true;
}

//----��������ģ���ʼ��-------------------------------------------------------
//����: ��ʼ����������ģ��
//����: para: ��Ч
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Font(ptu32_t para)
{
    static struct Object font_root_rsc;    // ������Դ

    s_ptCurFont = NULL;
    // ���������Դ���ڵ�
    if(OBJ_AddTree(&font_root_rsc,
                   sizeof(struct Object),RSC_RSCNODE,
                   CN_FONT_RSC_TREE))
    {
        return 1;
    }else
    {
        printf("install font module fail\r\n");
        return 0;
    }
}

//----��ȡ��ǰ����---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ�����
//����: ��ǰ����
//-----------------------------------------------------------------------------
struct FontRsc* Font_GetCurFont(void)
{
    return s_ptCurFont;
}

//----�趨��ǰ����---------------------------------------------------------
//����: ����������Ϊ��ǰʹ�õ����壬������������Ȱ�װ��ϵͳ�У����
//      ��������Դ�ڵ�δ������������ʲô������������ԭ���ĵ�ǰ���塣
//����: font��ָ�������塣
//����: NULL���趨ʧ��
//      �趨֮ǰ�����塣
//-----------------------------------------------------------------------------
struct FontRsc* Font_SetCurFont(struct FontRsc* font)
{
    struct Object *rsc;
    char *Name;
    if(font == NULL)
        return NULL;
    rsc = OBJ_SearchTree(CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //������Դ��δ����
    Name = OBJ_Name(&font->node);
    rsc = OBJ_SearchChild(rsc,(const char*)Name);
    if(rsc != NULL)
    {
        s_ptCurFont = (struct FontRsc*)rsc;
    }
    return s_ptCurFont;
}

//----����������Դ---------------------------------------------------------
//����: ������������������Դ
//����: name��ָ������������
//����: NULL���޴�������Դ
//      Ҫ�ҵ�������Դ
//-----------------------------------------------------------------------------
struct FontRsc* Font_SearchFont(const char* name)
{
    struct Object *rsc;

    rsc = OBJ_SearchTree(CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //������Դ��δ����

    return (struct FontRsc*)OBJ_SearchChild(rsc, name);
}

//----ȡ��������и�-----------------------------------------------------------
//����: ��ȡ�������ַ��ĵ����и�,������������ߵ��Ǹ��ַ��ĸ߶�.
//����: font, ����ѯ������.
//����: ���ֿ���ߵ��Ǹ��ַ��ĸ߶�(����ֵ)
//-----------------------------------------------------------------------------
s32 Font_GetFontLineHeight(struct FontRsc* font)
{
    if(font == NULL)
        return 0;
    return font->MaxHeight;
}

//----ȡ����������п�---------------------------------------------------------
//����: ��ȡ�������ַ��ĵ������п��,���������������Ǹ��ַ��Ŀ��.
//����: font, ����ѯ������.
//����: ���ֿ������Ǹ��ַ��Ŀ��(����ֵ)
//-----------------------------------------------------------------------------
s32 Font_GetFontLineWidth(struct FontRsc* font)
{
    if(font == NULL)
        return 0;
    return font->MaxWidth;
}

//----ȡ����������-------------------------------------------------------------
//����: ��ȡ�����������
//����: font, ����ѯ������.
//����: ����������,font.cģ�鲢��������������
//-----------------------------------------------------------------------------
s32 Font_GetFontAttr(struct FontRsc* font)
{
    if(font == NULL)
        return 0;
    return font->Attr;
}

