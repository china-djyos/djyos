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
//����ģ��: Ӳ����ʱ��
//����:  ������.
//�汾��V1.0.0
//�ļ�����: ϵͳ��ʼ���ļ�
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-04-24
//   ����:  ������.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾�����ļ�����ԭ���ͼ���������һ��
//------------------------------------------------------
#include "stdint.h"
#include "component_config_core.h"

extern void __InitTimeBase(void);
extern void __InitSys(void);
extern ptu32_t __InitMB(void);
extern void __StartOs(void);
extern ptu32_t __InitLock(void);
extern s32 obj_ModuleInit(void);
extern s32 handle_ModuleInit(void);
extern s32 Lock_CreateObject(void);
extern s32 Mb_CreateObject(void);
extern void Sys_ModuleInit(void);
extern void Board_Init(void);

align_type InitStack[CFG_INIT_STACK_SIZE/sizeof(align_type)] = {'d'};

//----runtime��ʼ��-----------------------------------------------------
//���ܣ�newlib�У����캯���ĳ�ʼ������Ϊpreinit_array��init_array�����֣�������C++
//      ȫ�ֹ��캯��ָ�����飬ǰ�����κ�˵������֪���⡣����֮�䣬������_init�������ú���
//      ��gcc��newlib�о��Ҳ�������ʵ�֣���preinit_array��_int������������ϵͳ�õġ�
//      DJYOS��ʱ�ò������ǣ��ȿ��š�
//��������
//���أ���
//---------------------------------------------------------------------------
void _init(void)
{
}

//----runtime��Ļʽ-----------------------------------------------------
//���ܣ�newlib�е���fini_array�еĺ���ָ��󣬵�����_fini�������ú�����gcc��newlib�о�
//      �Ҳ�������ʵ�֣�������������ϵͳ�õġ�DJYOS��ʱ�ò������ǣ��ȿ��š�
//��������
//���أ���
//---------------------------------------------------------------------------
void _fini(void)
{
}

//----ϵͳ��������-----------------------------------------------------
//���ܣ�ִ�в���ϵͳ���أ�ģ��(�豸)��ʼ�����û���ʼ���Լ���������ϵͳ
//��������
//���أ���
//---------------------------------------------------------------------------
void Sys_Start(void)
{
//  __InitTimeBase();
    __DjyInitTick();
    __InitSys( );
    __InitMB();
    // Mb_ModuleInit������Ҫ�����ź���������ModuleInstall_Lock֮ǰ�������ź���������ģ�
    // �����ܶ�����Post��pend��������ModuleInstall_Lock��Ҫ�����ڴ�أ���ֻ���ڵ���
    __InitLock();
    obj_ModuleInit();
    handle_ModuleInit();          // ��������ϵ��ʼ����
    Lock_CreateObject();    // lock��ϵ��
    Mb_CreateObject();      // memory block��ϵ��
    Board_Init();
    Sys_ModuleInit();
    // �ڿ���ע��,���������䲻Ҫ��ͼ����ʲô����,һ�к���Ը�.
    __StartOs();
}

