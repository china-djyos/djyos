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

//����ģ��: ASCII�ַ���������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: ASCII�ַ��������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "ascii.h"
#include "charset.h"
#include "gkernel.h"
#include "dbug.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_myname.h****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    extern bool_t ModuleInstall_CharsetAscii(void);
//    ModuleInstall_CharsetAscii ( );
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"ascii_charset"      //��д�����������
//parent:"NlsCharset"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:�������                   //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                         //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                      //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//grade:init                          //��ʼ��ʱ������ѡֵ��none��init��main��none��ʾ�����ʼ����
                                      //init��ʾ�ڵ���main֮ǰ��main��ʾ��main�����г�ʼ��
//dependence:"NlsCharset"             //������������������������none����ʾ�������������
                                      //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//weakdependence:"none"               //��������������������������none����ʾ�������������
                                      //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                      //����������������������г����á�,���ָ�
//mutex:"charset_gb2312"              //������������������������none����ʾ�������������
                                      //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header             //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
//%$#@end configue  ****�������ý���
//@#$%component end configure

s32 AsciiMbToUcs4(u32* pwc, const char* s, s32 n);
s32 AsciiMbsToUcs4s(u32* pwcs, const char* mbs, s32 n);
s32 AsciiUcs4ToMb(char* s, u32 wc);
s32 AsciiUcs4sToMbs(char* mbs, const u32* pwcs, s32 n);

// ע�Ͳ��� charset.h-> struct Charset -> GetOneMb
s32 AsciiGetOneMb(const char* mbs,s32 n)
{
    if (mbs == NULL)
    {
        return 0;
    }
    else if(*mbs >= 0x80)       //��������Ҳ�ǺϷ��ַ�
        return -1;
    else
        return 1;
}

// ע�Ͳ��� charset.h-> struct Charset -> MbToUcs4
s32 AsciiMbToUcs4(u32* pwc, const char* mbs,s32 n)
{
    u8 c;
    s32 result;
    if (mbs == NULL)
    {
        return 0;
    }
    c = *mbs;

    if(c == 0)
    {
        result = 0;
    }else if(c < 0x80)
    {
        result = 1;
    }
    else
        result = -1;
    if(pwc != NULL)
        *pwc = c;
    return result;
}

// ע�Ͳ��� charset.h-> struct Charset -> MbsToUcs4s
//todo����չascii����ô��?
s32 AsciiMbsToUcs4s(u32* pwcs, const char* mbs, s32 n)
{
    s32 wcn,len;
    if(mbs == NULL)
        return 0;
    if(n != -1)
        len = n;
    else
        len = CN_LIMIT_SINT32;
    for(wcn = 0; wcn < len; wcn++)
    {
        if( (mbs[wcn] == 0) || (mbs[wcn] >= 0x80) )
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)0;
            return wcn;
        }
        else
        {
            if(pwcs != NULL)
                pwcs[wcn] = (u32)mbs[wcn];
        }
    }
    return wcn;
}

// ע�Ͳ��� charset.h-> struct Charset -> Ucs4ToMb
s32 AsciiUcs4ToMb(char* mbs, u32 wc)
{

    if(wc < 0x80){

        *mbs = wc;
        return 1;
    }

    return -1;
}

// ע�Ͳ��� charset.h-> struct Charset -> Ucs4sToMbs
s32 AsciiUcs4sToMbs(char* mbs, const u32* pwcs, s32 n)
{
    s32 asciin,len;
    if(pwcs == NULL)
        return 0;
    if(n != -1)
        len = n;
    else
        len = CN_LIMIT_SINT32;
    for(asciin = 0; asciin < len; asciin++)
    {
        if( (pwcs[asciin] == 0) || (pwcs[asciin] >= 0x80) )
        {
            if(mbs != NULL)
                mbs[asciin] = '\0';
            break;
        }
        else
        {
            if(mbs != NULL)
                mbs[asciin] = (u8)pwcs[asciin];
        }
    }
    return asciin;
}

//----��װascii�ַ���----------------------------------------------------------
//����: ��װascii�ַ�������ϵͳʹ�������ַ�����ʱ��ʹ������ַ������ر�ע�⣬
//      gb2312�Ѿ�������Ӣ���ַ�����ʹ�����Ļ���Ӣ�Ļ�Ͻ���ģ�����Ҫ��װascii
//      �ַ��������ǣ�����GB2312���ֿ�ֻ������ȫ�ǵ�Ӣ���ַ����ʻ���Ҫ��װascii
//      ��������Դ���ߴ�(8*8��8*16)��ѡ��
//����: ������
//����: 1=�ɹ���0=ʧ��
//-----------------------------------------------------------------------------
bool_t ModuleInstall_CharsetAscii(void)
{
    static struct Charset encoding;

    encoding.max_len = 1;
    encoding.EOC_Size = 1;
    encoding.GetOneMb = AsciiGetOneMb;
    encoding.MbToUcs4 = AsciiMbToUcs4;
    encoding.Ucs4ToMb = AsciiUcs4ToMb;
    encoding.MbsToUcs4s = AsciiMbsToUcs4s;
    encoding.Ucs4sToMbs = AsciiUcs4sToMbs;
    if( Charset_NlsInstallCharset(&encoding, CN_NLS_CHARSET_ASCII))
    {
        debug_printf("ascii","ASCII encoding install sucess\r\n");
        return true;
    }else
    {
        Djy_SaveLastError(EN_GK_CHARSET_INSTALL_ERROR);
        debug_printf("ascii","ASCII encoding install fail\n\r");
        return false;
    }
}

