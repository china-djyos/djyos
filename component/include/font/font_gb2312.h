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

//����ģ��: GB2312-80��׼��������
//���ߣ�mjh
//�汾��V1.0.0
//�ļ�����: GB2312-80�������
//����˵��:
//�޶���ʷ:
//    2. ...
//    1. ����:2011-05-17
//       ����:mjh
//       �°汾�ţ�
//       �޸�˵��:��ʼ�汾
//------------------------------------------------------
#ifndef __FONTGB2312_H__
#define __FONTGB2312_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CN_FONT_GB2312_SONG_12    "gb2312_song_12"  //ascii:6*12,����12*12������

#define CN_FONT_GB2312_YAHEI_16   "gb2312_yahei_16" //ascii:8*16,����16*16��΢���ź�
#define CN_FONT_GB2312_SONG_16    "gb2312_song_16"  //ascii:8*16,����16*16������
#define CN_FONT_GB2312_FANG_16    "gb2312_fang_16"  //ascii:8*16,����16*16������
#define CN_FONT_GB2312_HEI_16     "gb2312_hei_16"   //ascii:8*16,����16*16������
#define CN_FONT_GB2312_KAI_16     "gb2312_kai_16"   //ascii:8*16,����16*16������
#define CN_FONT_GB2312_YUAN_16    "gb2312_yuan_16"  //ascii:8*16,����16*16��Բ��

#define CN_FONT_GB2312_YAHEI_24  "gb2312_yahei_24"  //ascii:12*24,����24*24��΢���ź�
#define CN_FONT_GB2312_SONG_24   "gb2312_song_24"   //ascii:12*24,����24*24������
#define CN_FONT_GB2312_FANG_24   "gb2312_fang_24"   //ascii:12*24,����24*24������
#define CN_FONT_GB2312_HEI_24    "gb2312_hei_24"    //ascii:12*24,����24*24������
#define CN_FONT_GB2312_KAI_24    "gb2312_kai_24"    //ascii:12*24,����24*24������
#define CN_FONT_GB2312_YUAN_24   "gb2312_yuan_24"   //ascii:12*24,����24*24��Բ��

#define CN_FONT_GB2312_SONG_32   "gb2312_song_32"   //ascii:16*32,����32*32������
#define CN_FONT_GB2312_FANG_32   "gb2316_fang_32"   //ascii:16*32,����32*32������
#define CN_FONT_GB2312_HEI_32    "gb2316_hei_32"    //ascii:16*32,����32*32������
#define CN_FONT_GB2312_KAI_32    "gb2316_kai_32"    //ascii:16*32,����32*32������
#define CN_FONT_GB2312_YUAN_32   "gb2316_yuan_32"   //ascii:16*32,����32*32��Բ��

#ifdef __cplusplus
}
#endif

#endif //__FONTGB2312_H__

