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
// ģ������: �쳣��Ϣ�洢�����ӿ�
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 10:27:22 AM/Mar 26, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:�û�ע����쳣��Ϣ������

#include "stdint.h"
#include "stddef.h"
#include "exp.h"
#include "exp_record.h"

static struct ExpRecordOperate s_tExpRecordOpt = {NULL};
static struct ExpRecordOperate s_tExpRecordDefaultOpt = {NULL};

// =============================================================================
// �������ܣ��ڴ濽��,��ʱc����δ���أ��������ã����ܵ���memcpy
// ���������
// ���������
// ����ֵ  ����_EN_EXP_RECORDRESULT����
// ˵��      �����쳣ģ��ĳ�ʼ�������У�C�⻹û�б����أ�������Ҫ���˵�ƽ̨���ԣ�������memcpy��������
// =============================================================================
void __memcpyByteByByte(u8 *dest, u8 *src, u32 len)
{
    u32 i;
    for(i = 0;i < len; i++)
    {
        dest[i] = src[i];
    }
}

// =============================================================================
// �������ܣ�Exp_Record
//           ��¼һ֡�쳣��Ϣ
// ���������recordpara����Ҫ��¼���쳣��Ϣ
// ���������
// ����ֵ  ����enum EN_ExpDealResult����
// =============================================================================
enum EN_ExpDealResult Exp_Record(struct ExpRecordPara *recordpara)
{
    u32 result;
    if(NULL != s_tExpRecordOpt.fnExpRecordSave)
    {
        result = (u32)(s_tExpRecordOpt.fnExpRecordSave(recordpara));
    }
    else
    {
        result = (u32)EN_EXP_RESULT_RECORD_NOMETHOD;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordClear
//          ������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��true �ɹ��� falseʧ��
// =============================================================================
bool_t Exp_RecordClear(void)
{
    if(NULL != s_tExpRecordOpt.fnExpRecordClean)//������ע���
    {
        return s_tExpRecordOpt.fnExpRecordClean();
    }
    else//Ĭ�Ϸ�ʽ����
    {
        return false;
    }
}
// =============================================================================
// �������ܣ�Exp_RecordCheckNum
//          �鿴һ���洢�˶������쳣��Ϣ
// �����������
// ���������recordnum,���ص��쳣��Ϣ��Ŀ��
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckNum(u32 *recordnum)
{
    if(NULL != s_tExpRecordOpt.fnExpRecordCheckNum)//������ע���
    {
        return s_tExpRecordOpt.fnExpRecordCheckNum(recordnum);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        *recordnum = 0;
        return false;
    }
}

// =============================================================================
// �������ܣ�Exp_RecordCheckLen
//          �鿴ָ����Ŀ���쳣��Ϣ�ĳ���
// ���������assignedno,ָ�����쳣֡��Ŀ
// ���������recordlen,���ڴ洢ָ���쳣��Ŀ�ĳ���
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t Exp_RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    bool_t result;
    if(NULL != s_tExpRecordOpt.fnExpRecordCheckLen)//������ע���
    {
        result = s_tExpRecordOpt.fnExpRecordCheckLen(assignedno, recordlen);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RecordGet
//          �Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ(�ڼ����쳣��Ŀ)
//          buflenlimit,ϵͳ�쳣֡�ĳ��ȣ����Ƴ�������Ϊ���⿽��Խ��
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t Exp_RecordGet(u32 assignedno, u32 buflenlimit, u8 *buf, \
                     struct ExpRecordPara  *recordpara)
{
    bool_t result;
    if(NULL != s_tExpRecordOpt.fnExpRecordGet)//������ע���
    {
        result = s_tExpRecordOpt.fnExpRecordGet(assignedno, buflenlimit,buf,recordpara);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_RegisterRecordOpt
//          ע���쳣��Ϣ������
// ���������opt,��Ҫע����쳣��Ϣ������
// �����������
// ����ֵ  ��false,ʧ��  true�ɹ�
// ע��    ��opt�ṹ����ָ���Ĵ�������Ӧ���ṩ������Ļ���ע�᲻�ɹ�
// =============================================================================
bool_t  Exp_RegisterRecorder(struct ExpRecordOperate *opt)
{
    bool_t result;
    if(NULL == opt)
    {
        result = false;
    }
    else
    {
        if((NULL == opt->fnExpRecordScan) ||\
           (NULL == opt->fnExpRecordSave)||\
           (NULL == opt->fnExpRecordClean)||\
           (NULL == opt->fnExpRecordCheckNum)||\
           (NULL == opt->fnExpRecordCheckLen)||\
           (NULL == opt->fnExpRecordGet))
        {
            result = false;
        }
        else
        {
            //ע��
            __memcpyByteByByte((u8 *)&s_tExpRecordOpt, \
                               (u8 *)opt, sizeof(s_tExpRecordOpt));
            //��һ�εĻ��������ݣ��Ա��ָ�
            if(NULL == s_tExpRecordDefaultOpt.fnExpRecordSave)
            {
                __memcpyByteByByte((u8 *)&s_tExpRecordDefaultOpt, \
                                   (u8 *)&s_tExpRecordOpt, \
                                   sizeof(s_tExpRecordOpt));
            }
            //�洢������ʼ��ɨ��
            if(NULL != s_tExpRecordOpt.fnExpRecordScan)
            {
                s_tExpRecordOpt.fnExpRecordScan();
            }
            result = true;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�Exp_UnRegisterRecordOpt
//           ע���쳣��Ϣ������
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// =============================================================================
bool_t Exp_UnRegisterRecorder(void)
{
    //�ָ���Ĭ�ϵĴ洢����
    __memcpyByteByByte((u8 *)&s_tExpRecordOpt, \
                       (u8 *)&s_tExpRecordDefaultOpt, \
                       sizeof(s_tExpRecordOpt));
    //�洢������ʼ��ɨ��
    if(NULL != s_tExpRecordOpt.fnExpRecordScan)
    {
        s_tExpRecordOpt.fnExpRecordScan();
    }
    return true;
}

