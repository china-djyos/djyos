//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
// ģ������: �׳��쳣��Ϣ������ע�ἰ���ڲ�����ʵ��
// ģ��汾: V1.00
// ������Ա: Administrator
// ����ʱ��: 3:16:13 PM/May 8, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:

#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "int.h"
#include "string.h"
#include "list.h"
#include "exp_decoder.h"
#include "dbug.h"
struct dListNode ExpDecoderListHead;
// =============================================================================
// ��������: �����쳣�����Ƿ��Ѿ���ע��,���ؽ������ṹ,δ��ע���򷵻�NULL
// �������: name,����������
// �������:
// ����ֵ  :�������ṹָ��,û�ҵ��ͷ���NULL
// =============================================================================
struct ExpInfoDecoder *  __Exp_FindDecoderNoByName(const char *name)
{
    struct ExpInfoDecoder *result = NULL;
    struct ExpInfoDecoder *Decoder;
    struct dListNode *Pos;
    int cmpresult;
    dListForEach(Pos,&ExpDecoderListHead)
    {
        Decoder = dListEntry(Pos,struct ExpInfoDecoder,DecoderList);
        cmpresult = strcmp(Decoder->DecoderName,name);
        if(0 == cmpresult)
        {
            result = Decoder;
            break;
        }
    }
    return result;
}

// =============================================================================
// ��������:Exp_ThrowInfodecode
//          �׳��쳣��Ϣ����
// �������:parahead,�׳��쳣ʱ����
//          endian, ��Ϣ�Ĵ�С��
// ���������
// ����ֵ  :true,�ɹ�����;false,����ʧ�ܣ���Ҫ����Ϊ��������
// ˵��    :�ڲ�����
// =============================================================================
bool_t  Exp_ThrowInfodecode(struct ExpThrowPara *parahead,u32 endian)
{
    bool_t result;
    u32 n;
    struct ExpInfoDecoder *Decoder;

    if((parahead->DecoderName == NULL) || (strlen(parahead->DecoderName) == 0))
    {
        debug_printf("exp","%s",parahead->ExpInfo);
        debug_printf("exp","\n\r");
        result = true;
    }
    else
    {
        Decoder = __Exp_FindDecoderNoByName(parahead->DecoderName);
        if(Decoder != NULL) //ע���У���ô�ͽ�����
        {
            result = Decoder->MyDecoder(parahead,endian);
        }
        else                //DecoderName�ǿգ���ȴ�Ҳ���������
        {
            debug_printf("exp","�쳣��Ϣ������δ�ҵ�,��ʾ16������Ϣ");
            for(n = 0; n < parahead->ExpInfoLen; n++)
            {
                debug_printf("exp","%x ",parahead->ExpInfo[n]);
                if((n & 0xf) == 0)
                    debug_printf("exp","\n\r");
            }
            result = false;
        }
    }
    return result;
}
// =============================================================================
//���ܣ�ע���쳣��Ϣ������,����쳣����ʱ�ռ�����Ϣ�Ƚ϶���Ҹ���,��ע��һ��
//      ������,���ڰѸ���Ϣ�����ɿɶ���Ϣ�Թ�����������쳣��Ϣ�������һ���ַ���,
//      ������ע�������.
//      ���洢���쳣������,��ȴδע��,����16������ʾ.
//������decoder,�쳣����������ָ��
//�������
//���أ�true,�ɹ�ע��;false,ע��ʧ�ܣ�
//˵����Ϊʲôʹ�����ֶ����ú���ָ��, ԭ������:
//      1.�쳣�����豸��������ʱ��¼��,�����Ͳ�������ʱ,��������һ��ר�ŵ����
//        ����,��ʱ���ڽ����쳣�ĺ���,��һ�����¼�쳣ʱ�ĵ�ַ��һ��.
//      2.�쳣��¼�п��ܱ�copy������������Ϸ���,������������ַ�Ӷ���һ����.
// =============================================================================
bool_t Exp_RegisterThrowInfoDecoder(struct ExpInfoDecoder *Decoder)
{
    bool_t  result;
    struct ExpInfoDecoder *dcd;
    atom_low_t  atom2operate;

    if(Decoder == NULL)
    {
        printk("Exp_RegisterThrowInfoDecoder:Invalid parameter!\n\r");
        return false;
    }
    atom2operate = Int_LowAtomStart();

    dcd = __Exp_FindDecoderNoByName(Decoder->DecoderName);
    if(dcd == NULL)//������δ��ע��
    {
        dListInsertAfter(&ExpDecoderListHead,&(Decoder->DecoderList));
        result = true;
    }
    else
    {
        result = false;
    }
    Int_LowAtomEnd(atom2operate);

    return result;
}

