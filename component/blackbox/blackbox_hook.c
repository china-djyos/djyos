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
// ģ������: �쳣�Ĺ��Ӵ�����
// ģ��汾: V1.00
// ������Ա: zhangqf_cyg
// ����ʱ��: 2:29:24 PM/Apr 17, 2014
// =============================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =============================================================================
// ��ע:��Ҫ�����ӵ�ע��͵���


#include "stdint.h"
#include "stddef.h"
#include "blackbox.h"
#include "blackbox_hook.h"

static fnBlackBox_Hook   s_fnBlackBoxHookDealer = NULL;
static fnBlackBox_HookParse  s_fnBlackBoxHookInfoDecoder = NULL;

// =============================================================================
// ��������: ִ��HOOKDealer
// �������:throwpara,�쳣�׳����׳����쳣����
// �������:infoaddr, �洢�쳣��Ϣ�ĵ�ַ
//          infolen,�洢�Ѽ���Ϣ����
//          BlackBoxAction,���洦��������û��HOOKʱ����ֵ��Ч
// ����ֵ  :true�ɹ�  false ʧ�ܣ�û��ע������أ����ӽ�����ò��ɣ�
// =============================================================================
bool_t __BlackBox_HookDealer(struct BlackBoxThrowPara *throwpara,\
                          ptu32_t *infoaddr,u32 *infolen,enum EN_BlackBoxAction *BlackBoxAction)
{
    if(NULL != s_fnBlackBoxHookDealer)
    {
        *BlackBoxAction = s_fnBlackBoxHookDealer(throwpara,infoaddr,infolen);
        return true;
    }
    else
    {
        *infoaddr = (ptu32_t)NULL;
        *infolen = 0;
        *BlackBoxAction = EN_BLACKBOX_DEAL_DEFAULT;
        return true;
    }
}
// =============================================================================
// ��������: ִ��HOOKDecoder
// �������:throwpara, �쳣������hook�ܽ�ͽ⣬���ܽ������
//          infoaddr, ��Ϣ�洢��ַ
//          infolen, ��Ϣ��Ч����
//          endian,�洢ʱ��Ĵ�С��
// �������:��
// ����ֵ  :true�ɹ�  false ʧ�ܣ�û��ע������أ�
// =============================================================================
bool_t __BlackBox_HookInfoDecoder(struct BlackBoxThrowPara *throwpara,\
                           ptu32_t infoaddr, u32 infolen,u32 endian)
{
    if(NULL != s_fnBlackBoxHookInfoDecoder)
    {
         return  s_fnBlackBoxHookInfoDecoder(throwpara,infoaddr,infolen,endian);
    }
    else
    {
        return false;
    }
}


// =============================================================================
// �������ܣ�ע��APP�ṩ���쳣����HOOK�����쳣�����󣬽�����fnHookFunc������
//           ��� fnBlackBox_Hook���Ͷ����ע�͡�fnHookFunc��������ͨ��infoaddrָ��
//           ����һЩ��Ϣ����Щ��Ϣ��������ַ��������Խ�һ���ṩfnHookParse��
//           ���ڰ���Ϣ�����ɿɶ���Ϣ��
// �������:fnHookFunc���쳣������
//          fnHookParse���쳣��Ϣ������
// �������:��
// ����ֵ  ��true�ɹ�  falseʧ��
// =============================================================================
bool_t BlackBox_RegisterHook(fnBlackBox_Hook  fnHookFunc,
                             fnBlackBox_HookParse fnHookParse)
{
    s_fnBlackBoxHookDealer = fnHookFunc;
    s_fnBlackBoxHookInfoDecoder = fnHookParse;
    return true;
}

// =============================================================================
// ��������:BlackBox_UnRegisterHook
//          ע��APP�ṩ���쳣����HOOK�Լ��쳣��Ϣ������
// �������:��
// �������:��
// ����ֵ  :true�ɹ�  falseʧ��
// =============================================================================
bool_t BlackBox_UnRegisterHook(void)
{
    s_fnBlackBoxHookDealer = NULL;
    s_fnBlackBoxHookInfoDecoder = NULL;

    return true;
}

