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
//�ļ�����: �б��ؼ���ض���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  lst.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_LISTVIEW_H__
#define __GDD_LISTVIEW_H__

#if __cplusplus
extern "C" {
#endif
#include    "gdd.h"
/*=========================================================================================*/

//LVITEM Mask
#define LVCF_FORMAT         (1<<0)
#define LVCF_TEXT           (1<<1)
#define LVCF_TEXTCOLOR      (1<<2)
#define LVCF_BACKCOLOR      (1<<3)
#define LVCF_BORDERCOLOR    (1<<4)
#define LVCF_DATA           (1<<5)
#define LVCF_ALL            (LVCF_FORMAT|LVCF_TEXT|LVCF_TEXTCOLOR|LVCF_BACKCOLOR|LVCF_BORDERCOLOR|LVCF_DATA)

//LVITEM Format
#define LVF_TEXT            (1<<0)  //��ʾ����
#define LVF_IMAGE           (1<<1)  //��ʾͼ��

#define LVF_ALIGN_V_MASK    (3<<2)
#define LVF_BOTTOM          (0<<2)  //��ֱ�ײ�����
#define LVF_VCENTER         (1<<2)  //��ֱ���ж���
#define LVF_TOP             (2<<2)  //��ֱ��������

#define LVF_ALIGN_H_MASK    (3<<4)
#define LVF_LEFT            (0<<4)  //ˮƽ�������
#define LVF_CENTER          (1<<4)  //ˮƽ���ж���
#define LVF_RIGHT           (2<<4)  //ˮƽ���Ҷ���

#define LVF_DEFAULT         (LVF_TEXT|LVF_VCENTER|LVF_CENTER)


typedef struct  __LVCELL
{
    u16 Mask;
    u16 Format;

    u32 cbTextMax;
    const char *pszText;
    u32 TextColor,BackColor,BorderColor;
    const void *pData;
}LVCELL;

#define MAKE_LVCELL_IDX(row,col) ((row<<16)|col)

/*=========================================================================================*/

#define LVM_FIRST   0x7000

#define LVM_INSERTCOLUMN    (LVM_FIRST+0)   //������
#define LVM_INSERTROW       (LVM_FIRST+1)   //�����С�
#define LVM_SETCELL         (LVM_FIRST+2)   //����һ����Ԫ
#define LVM_GETCELL         (LVM_FIRST+3)   //���һ����Ԫ
#define LVM_DELETEROW       (LVM_FIRST+4)   //ɾ��һ��
#define LVM_DELETEALLROWS   (LVM_FIRST+5)   //ɾ��������
#define LVM_SETFIRSTCOLUMN  (LVM_FIRST+6)
#define LVM_SETFIRSTROW     (LVM_FIRST+7)

/*=========================================================================================*/

HWND Widget_CreateListView(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink);


/*=========================================================================================*/

#if __cplusplus
}
#endif
#endif  /*__GDD_LISTVIEW_H__*/
