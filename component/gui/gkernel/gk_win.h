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

//����ģ��: gui
//����:  ������.
//�汾��V1.0.0
//�ļ�����: gui kernel���岿��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-11-21
//   ����:  ������.
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GK_WIN_H__
#define __GK_WIN_H__
#include <errno.h>
#include <ring.h>
#include "gk_syscall.h"
#ifdef __cplusplus
extern "C" {
#endif

struct DisplayObj;

struct GkChunnel       //�û�����(ͨ����gui windows) �� gui kernel��ͨ�Źܵ�
{
    struct RingBuf ring_syscall;         //gui ϵͳ���û��λ�����
//  u8 *syscall_buf;                        //Ϊϵͳ���û��λ����������ڴ�

    struct MutexLCB *syscall_mutex;      //������,���ڷ�ֹ����д
//  struct SemaphoreLCB *syscall_semp;   //�ź�����дʱ�������������Ⱥ�

    struct SemaphoreLCB *usercall_semp;  //�ź�����дʱ�������������Ⱥ�
    struct MsgQueue *usercall_msgq;      //usercall��Ϣ����

};

struct GkWinObj *__GK_GetZsectionStart(struct GkWinObj *gkwin);
struct GkWinObj *__GK_GetZsectionEnd(struct GkWinObj *gkwin);
struct GkWinObj *__GK_CreateDesktop(struct GkscParaCreateDesktop *para);
struct GkWinObj *__GK_CreateWin(struct GkscParaCreateGkwin *para);
void __GK_AdoptWin(struct GkscParaAdoptWin *para);
void __GK_MoveWin(struct GkscParaMoveWin *para);
void __GK_SetBound(struct GkWinObj *gkwin);
void __GK_SetBoundMode(struct GkscParaSetBoundMode *para);
void __GK_SetPrio(struct GkscParaSetPrio *para);
bool_t __GK_SetRopCode(struct GkscParaSetRopCode *para);
void __GK_SetVisible(struct GkscParaSetVisible *para);
bool_t __GK_SetHyalineColor(struct GkscParaSetHyalineColor *para);
void __GK_SetDirectScreen(struct GkWinObj *gkwin);
void __GK_SetUnDirectScreen(struct GkWinObj *gkwin);
void __GK_DestroyWinChild(struct GkWinObj *gkwin);
void __GK_DestroyWin(struct GkWinObj *gkwin);
bool_t __GK_ChangeWinArea(struct GkscParaChangeWinArea *para);
void __GK_OutputRedraw(struct DisplayObj *display);
u16 __GK_SyscallChunnel(u16 command,u32 sync_time,void *param1,u16 size1,
                                                void *param2,u16 size2);
bool_t  __GK_UsercallChunnel(void *pdata,u16 size,u32 timeout);

ptu32_t __GK_UsercallServer(void);
ptu32_t __GK_Server(void);

#ifdef __cplusplus
}
#endif

#endif //__GK_WIN_H__

