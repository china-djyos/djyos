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

//����ģ��: GDD
//����:  lst.
//�汾��V1.0.0
//�ļ�����: ���������ض���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-10-10
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_VIR_KEYBOARD_H__
#define __GDD_VIR_KEYBOARD_H__

#if __cplusplus
extern "C" {
#endif
#include    "gdd.h"
#include    "keyboard.h"




#define VK_DEL_CHAR           0x20
#define VK_ENTER_CHAR         161
#define VK_UP                 85
#define VK_DOWN               68

#define VK_CHAR_START         0x21
#define VK_CHAR_END           0x7e

#define VK_DECIMAL_POINT      46   //С����

#define VK_NUM_0              48
#define VK_NUM_1              49
#define VK_NUM_2              50
#define VK_NUM_3              51
#define VK_NUM_4              52
#define VK_NUM_5              53
#define VK_NUM_6              54
#define VK_NUM_7              55
#define VK_NUM_8              56
#define VK_NUM_9              57




struct VirKeyBoardPara
{
    u8 rows;            //��������
    u8 cols;             //��������
    u8 keyspace;     //�����������̵ļ������
    u8 id;           //����id���
    //char *keysets;   //���̵ļ�ֵ����
    struct KeyBoardPrivate *pKBP;
};

typedef struct VirKeyBoardPara  VirKeyBoard;
typedef struct VirKeyBoardPara *VBK;
// =============================================================================
// ��������: ������̿ؼ�����������
// �������: Text:������̴���Text;
//           Style:������̷�񣬲μ�gdd.h;
//           x:���������ʼλ��x��������(��λ������);
//           y:���������ʼλ��y��������(��λ������);
//           w:������̿��(��λ������);
//           h:������̸߶�(��λ������);
//           hParent:������̸����ھ��;
//           WinId:������̿ؼ�Id;
//           pdata:������̿ؼ�˽�����ݽṹ;
//           UserMsgTableLink:������̿ؼ��û���Ϣ�б�ṹָ�롣
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻��ı�������ʧ���򷵻�NULL��
// =============================================================================
HWND Widget_CreateVirKeyBoard(const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink);

#if __cplusplus
}
#endif
#endif  /*__GDD_VIR_KEYBOARD_H__*/
