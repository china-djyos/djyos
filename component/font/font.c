//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include "dbug.h"
#include "../include/font/font.h"
#include "../include/font/font_ascii.h"
#include "../include/font/font_gb2312.h"
#include "../include/font/gb2312.h"
#include "string.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_Font(void);
//    ModuleInstall_Font ( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"font"         //��������
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������             //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                   //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:medium              //��ʼ��ʱ������ѡֵ��early��medium��later��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"none"             //������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������������������������none����ʾ�������������
                                //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_FONT_DEFAULT   //****�������Ƿ��Ѿ����ú�
#warning    font�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16      //"Ĭ������",��������include/fontĿ¼����
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

static struct FontObj *s_ptCurFont;
struct obj *pFontRoot;

//----��ȡ������Դ-------------------------------------------------------------
//����: �����������ƻ�ȡ������Դ��"C"��Ĭ������Ĵ��š�
//����: ��������
//����: ƥ������壬��δ��ƥ����룬ʹ��Ĭ�ϵ�ASCII����
//-----------------------------------------------------------------------------
struct FontObj* Font_GetLocFont(const char* font_name)
{
    struct FontObj* font;

    if(strcmp(font_name, "C") == 0){

        font = Font_SearchFont(CFG_FONT_DEFAULT);

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
bool_t Font_InstallFont(struct FontObj *font,const char* name)
{
    if(!pFontRoot)
        return (FALSE);       //������Դ�����δ����

    font->HostObj = obj_newchild(pFontRoot, (fnObjOps)-1, 0, 0, name);
    if(!font->HostObj)
        return (FALSE);

    if(!s_ptCurFont)
        s_ptCurFont = font;

    return (TRUE);
}

//----��������ģ���ʼ��-------------------------------------------------------
//����: ��ʼ����������ģ��
//����: para: ��Ч
//����: true = �ɹ���false = ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_Font(void)
{
    pFontRoot = obj_newchild_set(objsys_root(), "font", NULL, 0, O_RDWR);
    if(pFontRoot)
    {
        info_printf("module","font install ");
        return true;
    }else
    {
        error_printf("module","font install failed");
        return false;
    }
}

//----��ȡ��ǰ����---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ�����
//����: ��ǰ����
//-----------------------------------------------------------------------------
struct FontObj* Font_GetCurFont(void)
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
struct FontObj* Font_SetCurFont(struct FontObj* font)
{
    struct obj *rsc;
    char *Name;
    if(font == NULL)
        return NULL;

    rsc = obj_search_child(objsys_root(), CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //������Դ��δ����
    Name = obj_name(font->HostObj);
    rsc = obj_search_child(rsc,(const char*)Name);
    if(rsc != NULL)
    {
        s_ptCurFont = (struct FontObj*)rsc;
    }
    return s_ptCurFont;
}

//----����������Դ---------------------------------------------------------
//����: ������������������Դ
//����: name��ָ������������
//����: NULL���޴�������Դ
//      Ҫ�ҵ�������Դ
//-----------------------------------------------------------------------------
struct FontObj* Font_SearchFont(const char* name)
{
    struct obj *rsc;

    rsc = OBJ_SearchTree(CN_FONT_RSC_TREE);
    if(rsc == NULL)
        return NULL;       //������Դ��δ����

    return (struct FontObj*)obj_search_child(rsc, name);
}

//----ȡ��������и�-----------------------------------------------------------
//����: ��ȡ�������ַ��ĵ����и�,������������ߵ��Ǹ��ַ��ĸ߶�.
//����: font, ����ѯ������.
//����: ���ֿ���ߵ��Ǹ��ַ��ĸ߶�(����ֵ)
//-----------------------------------------------------------------------------
s32 Font_GetFontLineHeight(struct FontObj* font)
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
s32 Font_GetFontLineWidth(struct FontObj* font)
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
s32 Font_GetFontAttr(struct FontObj* font)
{
    if(font == NULL)
        return 0;
    return font->Attr;
}
