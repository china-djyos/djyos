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
// AND ANY BLACKBOXRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
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
#include "blackbox.h"
#include "blackbox_record.h"

static struct BlackBoxRecordOperate s_tBlackBoxRecordOpt = {NULL};
static struct BlackBoxRecordOperate s_tBlackBoxRecordDefaultOpt = {NULL};

// =============================================================================
// �������ܣ��ڴ濽��,��ʱc����δ���أ��������ã����ܵ���memcpy
// ���������
// ���������
// ����ֵ  ����_EN_BLACKBOX_RECORDRESULT����
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
// �������ܣ���¼һ֡�쳣��Ϣ
// ���������recordpara����Ҫ��¼���쳣��Ϣ
// ���������
// ����ֵ  ����enum EN_BlackBoxDealResult����
// =============================================================================
enum EN_BlackBoxDealResult __BlackBox_Record(struct BlackBoxRecordPara *recordpara)
{
    u32 result;
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordSave)
    {
        result = (u32)(s_tBlackBoxRecordOpt.fnBlackBoxRecordSave(recordpara));
    }
    else
    {
        result = (u32)EN_BLACKBOX_RESULT_RECORD_NOMETHOD;
    }
    return result;
}
// =============================================================================
// �������ܣ�������е��쳣��Ϣ������쳣��Ϣ�洢����
// �����������
// �����������
// ����ֵ  ��true �ɹ��� falseʧ��
// =============================================================================
bool_t BlackBox_RecordClear(void)
{
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordClean)//������ע���
    {
        return s_tBlackBoxRecordOpt.fnBlackBoxRecordClean();
    }
    else//Ĭ�Ϸ�ʽ����
    {
        return false;
    }
}
// =============================================================================
// �������ܣ��鿴һ���洢�˶������쳣��Ϣ
// �����������
// ���������recordnum,���ص��쳣��Ϣ��Ŀ��
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t BlackBox_RecordCheckNum(u32 *recordnum)
{
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordCheckNum)//������ע���
    {
        return s_tBlackBoxRecordOpt.fnBlackBoxRecordCheckNum(recordnum);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        *recordnum = 0;
        return false;
    }
}

// =============================================================================
// �������ܣ��鿴ָ����Ŀ���쳣��Ϣ�ĳ���
// ���������assignedno,ָ�����쳣֡��Ŀ
// ���������recordlen,���ڴ洢ָ���쳣��Ŀ�ĳ���
// ����ֵ  ��false,ʧ�� true�ɹ�
// =============================================================================
bool_t BlackBox_RecordCheckLen(u32 assignedno, u32 *recordlen)
{
    bool_t result;
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordCheckLen)//������ע���
    {
        result = s_tBlackBoxRecordOpt.fnBlackBoxRecordCheckLen(assignedno, recordlen);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ��Ӵ洢�����л�ȡָ����Ŀ���쳣֡��Ϣ
// ���������assignedno,ָ�����쳣֡��Ŀ(�ڼ����쳣��Ŀ)
//          buflenlimit,����������
// ���������buf,���ڴ洢��ȡָ����Ŀ���쳣��Ϣ
//          recordpara,�쳣��Ϣ�洢ʱ�Ĳ������ڴ��Ƕ�buf�ĸ������ֵĶ���
// ����ֵ  ��true �ɹ� falseʧ��
// =============================================================================
bool_t BlackBox_RecordGet(u32 assignedno, u32 buflenlimit, u8 *buf, \
                     struct BlackBoxRecordPara  *recordpara)
{
    bool_t result;
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordGet)//������ע���
    {
        result = s_tBlackBoxRecordOpt.fnBlackBoxRecordGet(assignedno, buflenlimit,buf,recordpara);
    }
    else//Ĭ�Ϸ�ʽ����
    {
        result = false;
    }
    return result;
}
// =============================================================================
// �������ܣ�ע���쳣��Ϣ�洢����
// ���������opt,��Ҫע����쳣��Ϣ������
// �����������
// ����ֵ  ��false,ʧ��  true�ɹ�
// ע��    ��opt�ṹ����ָ���Ĵ�������Ӧ���ṩ������Ļ���ע�᲻�ɹ�
// =============================================================================
bool_t  BlackBox_RegisterRecorder(struct BlackBoxRecordOperate *opt)
{
    bool_t result;
    if(NULL == opt)
    {
        result = false;
    }
    else
    {
        if((NULL == opt->fnBlackBoxRecordScan) ||\
           (NULL == opt->fnBlackBoxRecordSave)||\
           (NULL == opt->fnBlackBoxRecordClean)||\
           (NULL == opt->fnBlackBoxRecordCheckNum)||\
           (NULL == opt->fnBlackBoxRecordCheckLen)||\
           (NULL == opt->fnBlackBoxRecordGet))
        {
            result = false;
        }
        else
        {
            //ע��
            __memcpyByteByByte((u8 *)&s_tBlackBoxRecordOpt, \
                               (u8 *)opt, sizeof(s_tBlackBoxRecordOpt));
            //��һ�εĻ��������ݣ��Ա��ָ�
            if(NULL == s_tBlackBoxRecordDefaultOpt.fnBlackBoxRecordSave)
            {
                __memcpyByteByByte((u8 *)&s_tBlackBoxRecordDefaultOpt, \
                                   (u8 *)&s_tBlackBoxRecordOpt, \
                                   sizeof(s_tBlackBoxRecordOpt));
            }
            //�洢������ʼ��ɨ��
            if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordScan)
            {
                s_tBlackBoxRecordOpt.fnBlackBoxRecordScan();
            }
            result = true;
        }
    }
    return result;
}
// =============================================================================
// �������ܣ�ע���쳣��Ϣ������
// �����������
// �����������
// ����ֵ  ��true�ɹ�  falseʧ��
// =============================================================================
bool_t BlackBox_UnRegisterRecorder(void)
{
    //�ָ���Ĭ�ϵĴ洢����
    __memcpyByteByByte((u8 *)&s_tBlackBoxRecordOpt, \
                       (u8 *)&s_tBlackBoxRecordDefaultOpt, \
                       sizeof(s_tBlackBoxRecordOpt));
    //�洢������ʼ��ɨ��
    if(NULL != s_tBlackBoxRecordOpt.fnBlackBoxRecordScan)
    {
        s_tBlackBoxRecordOpt.fnBlackBoxRecordScan();
    }
    return true;
}

