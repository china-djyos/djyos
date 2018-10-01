//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��: Զ������
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: Զ������
//����˵��:
//�޶���ʷ:
//1. ����: 2017-08-07
//   ����:  zhb.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#if 0
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_peri.h"
#include "IAP_Inner.h"
#if(CN_CPU_OPTIONAL_CACHE==1)
#include "set-cache.h"
#endif



extern tagIapVar pg_IapVar;
const char updateflag[]="update";//ҪŪ��const�����Ǿֲ��������������������ROM

bool_t HAL_SetUpdateFlag(void)
{
    pg_IapVar.UpdateFlag[0]=0x75;
    pg_IapVar.UpdateFlag[1]=0x70;
    pg_IapVar.UpdateFlag[2]=0x64;
    pg_IapVar.UpdateFlag[3]=0x61;
    pg_IapVar.UpdateFlag[4]=0x74;
    pg_IapVar.UpdateFlag[5]=0x65;
    pg_IapVar.UpdateFlag[6]=0x0;
    return true;
}

bool_t HAL_ClrUpdateFlag(void)
{
    pg_IapVar.UpdateFlag[0]=0x64;
    pg_IapVar.UpdateFlag[1]=0x64;
    pg_IapVar.UpdateFlag[2]=0x64;
    pg_IapVar.UpdateFlag[3]=0x64;
    pg_IapVar.UpdateFlag[4]=0x64;
    pg_IapVar.UpdateFlag[5]=0x64;
    pg_IapVar.UpdateFlag[6]=0x64;
    return true;

}

bool_t HAL_CheckUpdateFlag(void)
{
    u8 i;
    for(i=0;i<7;i++)
    {
        if(pg_IapVar.UpdateFlag[i]!=updateflag[i])
        {
            return false;
        }
    }
    return true;


}

#endif
