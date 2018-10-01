//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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


#ifndef __YMODEM_H__
#define __YMODEM_H__
#include <stdint.h>
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

// YMODEM����
#define CN_YMODEM_SOH     1         //128�ֽ���ʼ��
#define CN_YMODEM_STX     2         //1028�ֽ���ʼ��
#define CN_YMODEM_EOT     4
#define CN_YMODEM_ACK     6
#define CN_YMODEM_NAK     0x15
#define CN_YMODEM_C       0x43          //��д��ĸC
#define CN_YMODEM_CAN     0x18

// YMODEM���ݰ�����
#define CN_YMODEM_SOH_SIZE  128        //SOH���ݰ���СΪ128�ֽ�
#define CN_YMODEM_STX_SIZE  1024       //STX���ݰ���СΪ1024

typedef enum
{
     YMODEM_OK= 0,              //��������
     YMODEM_PARAM_ERR,          //��������
     YMODEM_FILE_ERR,           //�ļ���������
     YMODEM_TIMEOUT,            //��ʱ����
     YMODEM_MEM_ERR,            //�ڴ治��
     YMODEM_CAN_TRANS,          //ȡ������
     YMODEM_MEDIA_ERR,          //Ӳ������
     YMODEM_UNKNOW_ERR
}YMRESULT;

typedef enum
{
    ENUM_YMODEM_STA_INFO = 0x80,        //YMODEM��Ϣ��
    ENUM_YMODEM_STA_SOH,                //128�ֽڵ����ݰ�
    ENUM_YMODEM_STA_STX,                //1024�ֽڵ����ݰ�
    ENUM_YMODEM_STA_EOT,                //������
    ENUM_YMODEM_STA_ZERO,               //ȫZERO��
}YMSTATUS;

typedef enum
{
    YMODEM_FILE_NOOPS= 0xF0,            //���ļ�����
    YMODEM_FILE_OPEN ,                  //���ļ����
    YMODEM_FILE_WRITE,                  //д�ļ����
    YMODEM_FILE_READ,                   //���ļ����
    YMODEM_FILE_STAT,
    YMODEM_FILE_CLOSE,                  //�ر��ļ����
}YMFILEOPS;


typedef struct __Ymodem
{
    FILE    *File;                      //file ptr for file op
    const char    *Path;                //�ļ����ڵ�·����δ���ļ�����
    char    *FileName;                  //�ļ�����
    struct MutexLCB *pYmodemMutex;      //�������
    YMFILEOPS FileOps;                  //�ļ�����
    u32     FileSize;                   //��ǰ�����ļ���С
    u32     FileCnt;                    //д���ļ���С����
    u32     PkgNo;                      //����
    u32     PkgSize;                    //��ǰ������
    s64     StartTime;                  //start time
    u32     TimeOut;                    //time out
    YMSTATUS Status;                    //ymodem status
    u8      *FileBuf;                   //file buf
    u32     FileBufCnt;                 //�ļ���������С����
    u8      *PkgBuf;
    u32     PkgBufCnt;                  //PKG��С����
}tagYmodem;

ptu32_t ModuleInstall_Ymodem( void );
bool_t Ymodem_UploadFile(char *Param);
bool_t Ymodem_DownloadFile(char *Param);
bool_t Ymodem_PathSet(const char *Path);

#ifdef __cplusplus
}
#endif

#endif //__YMODEM_H__

