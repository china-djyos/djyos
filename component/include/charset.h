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

//����ģ��: �ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: �ַ�������Դ��ʼ��
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#ifndef __CHARSET_H__
#define __CHARSET_H__

#include "stdint.h"
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CN_CHAR_ENCODING_RSC_TREE       "charset encoding"

#define CN_NLS_CHARSET_ASCII            "ascii"
#define CN_NLS_CHARSET_GB2312           "gb2312"
#define CN_NLS_CHARSET_UTF8             "utf8"
#define CN_NLS_CHARSET_CYRIL            "cyril"     //������ַ���

/* ���ֽ��ַ�������Դ */
struct Charset
{
    struct Object *HostObj;
    u16 max_len;        // �����ַ�������ֽ���
    u16 EOC_Size;       // �ַ������������ֽ���

//----ȡ���ֽ��ַ�-------------------------------------------------------------
//����: �Ӷ��ֽ��ַ�����ȡһ�����ֽ��ַ�����ת��Ϊucs4�ַ���
//����: mbs, ָ���ת���Ķ��ֽ��ַ��ֽ����е�ָ��(�ɵ��ú����ж�s�ĺϷ���)
//      n������ⳤ�ȣ�-1��ʾ���ȫ����
//����: 0: mbs��NULLָ�롣
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ���
//      ��������ת�����ַ����ȣ��ֽ�����
//˵�������صĶ��ֽ��ַ�����mbsָ����ַ���
    s32 (*GetOneMb)(const char* mbs, s32 n);

//----���ֽ��ַ�תΪucs4�ַ�---------------------------------------------------
//����: ��һ�����ֽ��ַ�ת��Ϊucs4�ַ���
//����: pwc������ת�������ָ�룬��ΪNULL,�򲻷���ת�����
//      mbs, ָ���ת���Ķ��ֽ��ַ��ֽ����е�ָ��(�ɵ��ú����ж�s�ĺϷ���)
//      n������ⳤ�ȣ�-1��ʾ���ȫ����
//����: 0: mbs��NULLָ�����mbsָ��մ���
//      -1:mbsָ��Ĳ��ǺϷ����ֽ��ַ���
//      ��������ת�����ַ����ȣ��ֽ�����
//˵����
//      �˺�����C Run-time���mblen��mbtowc��������
    s32 (*MbToUcs4)(u32* pwc, const char* mbs, s32 n);

//----ucs4�ַ�תΪ���ֽ��ַ�---------------------------------------------------
//����: ��һ��ucs4�ַ�ת��Ϊ���ֽ��ַ���
//����: mb������ת�������ָ��(�ɵ��ú����ж�s�ĺϷ���)
//      wc����ת�����ַ�
//���أ�-1,���wc���ܶ�Ӧһ����Ч�Ķ��ֽ��ַ���
//      �ֽ��������ض�Ӧ�Ķ��ֽ��ַ����ֽ�����
//˵����
//      �˺�����C Run-time���wctomb��������
    s32 (*Ucs4ToMb)(char* mb, u32 wc);

//----���ֽ��ַ���תΪucs4��---------------------------------------------------
//����: ��һ�����ֽ��ַ���ת��Ϊucs4�ַ���
//����: pwcs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      mbs��������ֽ��ַ��Ļ�����ָ��
//      n������ⳤ�ȣ�-1��ʾ���ȫ����
//����: 0: mbs��NULLָ��
//      -1:������������ǰ���в���ת�����ַ�
//      ����:�õ����ַ�����=n��ʾԴ���ǲ�������������'\0'��
//-----------------------------------------------------------------------------
    s32 (*MbsToUcs4s)(u32* pwcs, const char* mbs, s32 n);

//----ucs4�ַ���תΪ���ֽ��ַ���-----------------------------------------------
//����: ��һ��ucs4�ַ���ת��Ϊ���ֽ��ַ�����
//����: mbs������ת�������ָ�룬�������ɵ��÷��ṩ�����գ��򱾺���ת��Ϊֻ����
//          ����ת�����������ֽ���
//      pwcs����ת�����ַ�
//      n������ⳤ�ȣ�-1��ʾ���ȫ����ע��ucs4�Ľ�����������4��0x00.
//����: 0: pwcs��NULLָ��
//      -1:������������ǰ���в���ת�����ַ�
//      ����:д��mbs���������ֽ���������������'\0'
//-----------------------------------------------------------------------------
    s32 (*Ucs4sToMbs)(char* mbs, const u32* pwcs, s32 n);
};


ptu32_t ModuleInstall_Charset(void);
bool_t  Charset_NlsInstallCharset(struct Charset *encoding,const char* name);
struct Charset* Charset_NlsGetCurCharset(void);
struct Charset* Charset_NlsSetCurCharset(struct Charset* encoding);
struct Charset* Charset_NlsSearchCharset(const char* name);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // __CHARSET_H__

