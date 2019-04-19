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
#ifndef _NRSEC3000_H_
#define _NRSEC3000_H_

#include "stdint.h"
typedef struct   //����оƬ�ṹ��
{
  u8   TxBuffer[1024];
  u8   TxBuffer1[500];//ָ��ͻ�����
  u8   TxBuffer2[5000];//���ݷ��ͻ�����
  u8   RxBuffer[5000];//���õĽ��ջ�����

  u8   SM1_Key[16];//���Ҫ�����SM1�Գ���Կ
  u8   SM1_IV[16];//�����ʼ����IV
  u8   SM1_TrueNumber[2048];//���SM1δ��������
  u8   SM1_EncryptNumber[2048];//���SM1���ܺ�����

  u8   Client_PrivateKey[32];
  u8   SM2_00_PublicKey[64];//SM2 ��Կ����00����Կ
  u8   SM2_01_PublicKey[64];//SM2 ��Կ����01����Կ
  u8   SM2_TrueNumber[128];//���SM2δ��������
  u8   SM2_EncryptNumber[128];//���SM2���ܺ�����

  u8   SM2_Certificate[500];//���SM2�����֤��
  u8   Hash_Number[32];//���SM3_Hashֵ
  u8   sign[64];//���SM2ǩ��

  u8   * Client_Cer;
  u8   PucID[16];
  u32  Client_Length;

  u8   Auth[32];//��ȫ��֤����
  u8   Auth_Number[146];//��Ű�ȫ��֤����
  u8   Version[64];//��Ű汾��Ϣ
  u8   Random[255];//��������
  u16  len;//���ջ������н���INS�����ƫ����������+2/+3��ȡ���ڽ��ճ���len��һ�ֽڻ������ֽڣ����Ǽ���оƬ�ͳ��������ݣ�

}tagEncryptionCtrl;

extern tagEncryptionCtrl  g_tEncryptionCtrl;


bool_t NRSEC3000_Init(void);
bool_t Import_SM1_Key();
bool_t Import_IV();
bool_t SM1_Encrypt(u8 *Txbuffer,u16 InLenth);
bool_t SM1_Decrypt(u8 *Txbuffer,u16 InLenth);
bool_t Key_Product();
bool_t Export_SM2_PublicKey(u8 p2);
bool_t Export_SM2_PrivateKey(u8 p2);
bool_t Import_SM2_PublicKey(u8 p2);
bool_t Import_SM2_PrivateKey(u8 p2);
int NRSEC3000_sm3(u8 *Txbuffer,u16 Lenth, u8 *Rxbuffer);
int sm3(unsigned char *in,int inl,unsigned char *out, unsigned char *pubkey, unsigned char
*pucID, int idl);
bool_t SM2_Sign(u8 p2);
bool_t SM2_Sign_Verif(u8 p2);
bool_t SM2_Encrypt(u8 p2);
bool_t SM2_Decrypt(u8 p2);
bool_t Cer_Request(char *byP3Info,u8 p3);
bool_t Random_Numer(u8 p3);
u8 NRSEC3000_RdVersion();
bool_t Safety_Cert();


#endif
