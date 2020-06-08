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
//�ļ�����: ������(progressbar)��ض���
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2014-11-10
//   ����:  lst.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_PROGRESSBAR_H__
#define __GDD_PROGRESSBAR_H__

#if __cplusplus
extern "C" {
#endif
#include    "gdd.h"

//������Flag
#define PBF_ORG_LEFT    (0<<0)              //δ�õ�
#define PBF_ORG_RIGHT   (1<<0)              //ProgressBar_Paint���õ�
#define PBF_ORG_TOP     PBF_ORG_LEFT        //δ�õ�
#define PBF_ORG_BOTTOM  PBF_ORG_RIGHT       //ProgressBar_Paint���õ�
#define PBF_SHOWTEXT    (1<<1)
//���������ݽṹ
typedef struct
{
    u32 Flag;//���������2bit used
    u32 Range;//��������Χ
    u32 Pos;  //�Ƿ��ƶ����ƶ�����
    u32 FGColor;//��������ɫ
    u32 BGColor;//����������ɫ
    u32 TextColor;//������ɫ
    u32 DrawTextFlag;//������
    char * text;//��������ʾ�ı�

}PROGRESSBAR_DATA;


// ���������ʹ��struct WINDOW�ṹStyle�ĵ�16λ����16λ���ڴ��ڹ������
#define PBS_HOR     (0<<0)  //ˮƽ������
#define PBS_VER     (1<<0)  //��ֱ������
#define PBS_FLAT    (1<<1)  //ƽ����

//// ������ģʽ
typedef enum{
    PBM_LEFT = 0,   //��������
    PBM_RIGHT = 1,  //������ұ�
    PBM_TOP = 2,    //������ϱ�
    PBM_BOTTOM = 3, //����ڵױ�

}EN_PB_MODE;

HWND Widget_CreateProgressBar(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink);

bool_t Widget_MakeProgressRect(RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode);
#if __cplusplus
}
#endif
#endif  /*__GDD_PROGRESSBAR_H__*/

