//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������
// 3. ��������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ��������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------

//����ģ��: GDD
//����:  zhb.
//�汾��V1.0.0
//�ļ�����: ��ť�ؼ�ʵ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2016-10-14
//   ����:  zhb.
//   �°汾�ţ�V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __GDD_TEXTBOX_H__
#define __GDD_TEXTBOX_H__

#if __cplusplus
extern "C" {
#endif
#include    "gdd.h"
#include    "gdd_widget.h"


typedef enum{
	EN_R_W =0,   //�ɶ���д
    EN_R_O ,     //read only.
    EN_W_O ,     //write only.
}EN_TEXTBOX_EDIT_PROPERTY;



#define WS_TEXTBOX_R_W       (1<<8) //�Ӵ��ڱ�־,�ؼ����ڱ���ָ���ñ�־.
#define WS_TEXTBOX_R_O       (1<<9) //�����Ƿ�ɼ�
#define WS_TEXTBOX_W_O       (1<<10) //�����Ƿ�Ϊ��ֹ״̬,���ָ���ñ�־,���ڽ�����Ӧ������Ϣ


//�ı���Text���ݲ���������
enum TextCtrlCmd
{
   EN_GET_TEXT=0,
   EN_SET_TEXT,
   EN_DELETE_TEXT,
};


//�ı���˽�����ݽṹ
typedef struct
{
   u16 ChNum;                 //�ַ���
   u8 CharNumLimit;          //�û������ı����м����ճ��������ַ���
   u16 CharWidthSum;        //��ǰ�ܵ��ַ�����
   EN_TEXTBOX_EDIT_PROPERTY EditProperty;          //�ı��������Ƿ�ֻ�ɶ�
   bool_t Visible;           //�ı����Ƿ�ɼ�
   bool_t IsMultiLines;       //�Ƿ�֧�ֶ���
   u16    MaxLines;           //�������
   u8 CursorLoc;          //��ǰ�������λ��,0~CN_STR_LEN_MAX,��ǰ�ı���Ϊ���㴰��ʱ�����ֵĬ��Ϊ0xff
}TextBox;


#define BORDER_FIXEDSINGLE   (1<<0)
#define BORDER_FIXED3D       (1<<1)
// =============================================================================
// ��������: TextBox�ؼ�����������
// �������: Text:�ı��򴰿�Text;
//           Style:�ı����񣬲μ�gdd.h;
//           x:�ı�����ʼλ��x��������(��λ������);
//           y:�ı�����ʼλ��y��������(��λ������);
//           w:�ı������(��λ������);
//           h:�ı���߶�(��λ������);
//           hParent:�ı��򸸴��ھ��;
//           WinId:�ı���ؼ�Id;
//           pdata:�ı���ؼ�˽�����ݽṹ;
//           UserMsgTableLink:�ı���ؼ��û���Ϣ�б��ṹָ�롣
// �������: �ޡ�
// ����ֵ  :�ɹ��򷵻��ı�������ʧ���򷵻�NULL��
// =============================================================================
HWND CreateTextBox(const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,void *pdata,
                    struct MsgTableLink *UserMsgTableLink);

// =============================================================================
// ��������: TextBox�ؼ���ʾ���ݿ��ƺ���
// �������: hwnd,TextBox�ؼ����ھ��;
//          ctrlcmd,����μ�enum TextCtrlCmd
//          para1:����EN_GET_TEXT��EN_SET_TEXT��EN_DELETE_TEXT�ò�����Ч,��ֱ����0��
//          para2:����EN_DELETE_TEXT�ò�����Ч,��ֱ����0,����EN_GET_TEXT��EN_SET_TEXT
//          �����ַ���ָ��.
// ����ֵ  :�ɹ��򷵻�true��ʧ���򷵻�false.
// =============================================================================
bool_t TextBox_TextCtrl(HWND hwnd,u8 ctrlcmd,u32 para1,ptu32_t para2);


#if __cplusplus
}
#endif
#endif  /*__GDD_TEXTBOX_H__*/