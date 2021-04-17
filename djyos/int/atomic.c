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
//����ģ��:���ܺ�����
//���ߣ�����
//�汾��V1.0.0
//�ļ�����:ԭ�ӱ�����������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: lst
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#include "stdint.h"
#include "int.h"

#include "component_config_int.h"

//-----------------------------------------------------------------------------
//����: 32λ�޷�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_Set32(u32 *base, u32 value)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base = value;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 32λ�޷�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_Set64(u64 *base, u64 value)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base = value;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 32λ�޷�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_UADD32(u32 *base, u32 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 32λ�޷�����ԭ�Ӽ���
//����: base: ����������ַ
//      sub: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_USUBb2(u32 *base, u32 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 32λ�з�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_SADD32(s32 *base, s32 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 32λ�з�����ԭ�Ӽ���
//����: base: ����������ַ
//      sub: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_SSUB32(s32 *base, s32 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 64λ�޷�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_UADD64(u64 *base, u64 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 64λ�޷�����ԭ�Ӽ���
//����: base: ����������ַ
//      sub: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_USUB64(u64 *base, u64 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 64λ�з�����ԭ�Ӽӷ�
//����: base: ����������ַ
//      inc: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_SADD64(s64 *base, s64 inc)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base +=inc;
    Int_LowAtomEnd(atom);
}

//-----------------------------------------------------------------------------
//����: 64λ�з�����ԭ�Ӽ���
//����: base: ����������ַ
//      sub: ����
//����: ��
//ע��������ֻ�ܷ�ֹ�̻߳���ͨ�ж�(�첽�ź�)�����ͻ���������ʵʱ�ж��з���base
//    ���¹����ͻ������Ҳ�Ȳ�����
//-----------------------------------------------------------------------------
void Atom_SSUB64(s64 *base, s64 sub)
{
    atom_low_t atom;
    atom = Int_LowAtomStart();
    *base -= sub;
    Int_LowAtomEnd(atom);
}


