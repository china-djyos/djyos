//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.DGTD

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

//����ģ��: �ַ�������
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����: �ַ�����Դ��ʼ��
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:lst
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "object.h"
#include "stdio.h"
#include "dbug.h"
#include "charset.h"
#include "component_config_NlsCharset.h"

static struct Charset *s_ptCurCharset;      // ��ǰ�ַ���
static struct Object *s_ptCharsetDir;       // �ַ���Ŀ¼
static bool_t g_bUserSetCharset = false;
//----��װ�ַ���-------------------------------------------------------------
//����: �����ַ�����װ���ַ���Ŀ¼��
//����: encoding���������ַ���ָ��
//      name�������ַ�����
//����: true
//-----------------------------------------------------------------------------
bool_t  Charset_NlsInstallCharset(struct Charset *encoding,const char* name)
{
    if(s_ptCharsetDir == NULL)
        return (FALSE);       // �ַ���Ŀ¼δ����

    encoding->HostObj = obj_newchild(s_ptCharsetDir,(fnObjOps)-1,(ptu32_t)encoding,name);
    if(!encoding->HostObj)
    {
        return (FALSE);
    }

    if(!s_ptCurCharset)     //�����ǰ�ַ���δ����
        s_ptCurCharset = encoding;
    else
    {
        //��ǰ�ַ�������ǵ���API���õģ���ά�֣���������°�װ���ַ����Ǳ����ַ�����
        //��ѵ�ǰ�ַ�������Ϊ�����ַ���
        if( ! g_bUserSetCharset)
        {
            if(strcmp(name,CFG_LOCAL_CHARSET) == 0)
                s_ptCurCharset = encoding;
        }
    }

    return (TRUE);
}

//----�ַ���ģ���ʼ��-------------------------------------------------------
//����: ��ʼ���ַ�������ģ��
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Charset(ptu32_t para)
{
    s_ptCurCharset = NULL;
    // �����ַ���Ŀ¼
    s_ptCharsetDir = obj_newchild(obj_root(),(fnObjOps)-1,0,CN_CHAR_ENCODING_RSC_TREE);
    if(s_ptCharsetDir)
    {
        return 1;
    }else
    {
        debug_printf("charset","install charset module fail\r\n");
        return 0;
    }
}

//----��ȡ��ǰ�ַ���---------------------------------------------------------
//����: ��ȡ��ǰʹ�õ��ַ���
//����: ��ǰ�ַ���
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsGetCurCharset(void)
{
    return s_ptCurCharset;
}

//----�趨��ǰ�ַ���---------------------------------------------------------
//����: �����ַ�����Ϊ��ǰʹ�õ��ַ���,���ַ����������Ȱ�װ��ϵͳ�У����
//      encoding�ַ���δ��װ��������ʲô������������ԭ���ĵ�ǰ�ַ���
//����: encoding��ָ�����ַ���
//����: NULL���趨ʧ��
//      �趨֮ǰ���ַ���
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsSetCurCharset(struct Charset* encoding)
{
    struct Object *Me;
    if(encoding == NULL)
        return NULL;
    if(encoding == s_ptCurCharset)
        return encoding;
    if(s_ptCharsetDir == NULL)
        return NULL;       //�ַ���Ŀ¼δ����
    Me = encoding->HostObj;
    if(s_ptCharsetDir==obj_parent(Me)) // �ַ���ȷ�Ѱ�װ��s_ptCharsetDirĿ¼
    {
        s_ptCurCharset = (struct Charset*)encoding;
        g_bUserSetCharset = true;
    }
    return s_ptCurCharset;
}

//----�����ַ���---------------------------------------------------------
//����: �������������ַ���
//����: name��ָ�����ַ�������
//����: Ҫ�ҵ��ַ���ָ�룬NULL=�޴��ַ���
//
//-----------------------------------------------------------------------------
struct Charset* Charset_NlsSearchCharset(const char* name)
{
    struct Object *CharsetObj;
    if(s_ptCharsetDir == NULL)
        return NULL;       //�ַ���Դ��δ����

    CharsetObj = obj_search_child(s_ptCharsetDir,name);
    if(CharsetObj != NULL)
        return (struct Charset*)obj_GetPrivate(CharsetObj);
    else
        return NULL;
}

