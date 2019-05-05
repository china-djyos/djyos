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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#ifndef _NRSEC3000_H_
#define _NRSEC3000_H_

#include "stdint.h"
typedef struct   //加密芯片结构体
{
  u8   TxBuffer[1024];
  u8   TxBuffer1[500];//指令发送缓冲区
  u8   TxBuffer2[5000];//数据发送缓冲区
  u8   RxBuffer[5000];//共用的接收缓冲区

  u8   SM1_Key[16];//存放要导入的SM1对称密钥
  u8   SM1_IV[16];//导入初始向量IV
  u8   SM1_TrueNumber[2048];//存放SM1未加密数据
  u8   SM1_EncryptNumber[2048];//存放SM1加密后数据

  u8   Client_PrivateKey[32];
  u8   SM2_00_PublicKey[64];//SM2 密钥索引00处公钥
  u8   SM2_01_PublicKey[64];//SM2 密钥索引01处公钥
  u8   SM2_TrueNumber[128];//存放SM2未加密数据
  u8   SM2_EncryptNumber[128];//存放SM2加密后数据

  u8   SM2_Certificate[500];//存放SM2请求的证书
  u8   Hash_Number[32];//存放SM3_Hash值
  u8   sign[64];//存放SM2签名

  u8   * Client_Cer;
  u8   PucID[16];
  u32  Client_Length;

  u8   Auth[32];//安全认证因子
  u8   Auth_Number[146];//存放安全认证数据
  u8   Version[64];//存放版本信息
  u8   Random[255];//存放随机数
  u16  len;//接收缓冲区中接收INS命令处的偏移量（后面+2/+3（取决于接收长度len是一字节还是两字节）就是加密芯片送出来的数据）

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
