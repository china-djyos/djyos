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
// Copyright (c) 2014 ����Ȩ����������������޹�˾���С�����Ȩ�˱���һ��Ȩ����
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
//����ģ��:�ļ�ϵͳ
//���ߣ�lst
//�汾��V1.0.0
//�ļ�����:easynor�ļ�ϵͳ��ģ��
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����: 2009-01-04
//   ����: ����
//   �°汾��: V1.0.0
//   �޸�˵��: ԭʼ�汾
//------------------------------------------------------
#ifndef __NOR_FLASHFILE_H__
#define __NOR_FLASHFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EFS_NAME_LIMIT            63 // ��������󳤶�,���һ���ֽ�'\0'

// ˵�����ļ�������Ϊ64�ֽڣ�61�ֽ�Ϊ'\0'�������ֽ�Ϊ�ļ�����ECC
#define FILENAME_LIMIT            60 // ��������󳤶�,���һ���ֽ�'\0'
#define ECC_LIMIT                 3
#define EFS_CFG_LIMIT             17568 // ������Ϣ����
#define EFS_ITEM_LIMIT            288 // ÿ��������ĳ���

//�ļ���������Աƫ����
#define FILE_NAME_OFF             (0 ) // �ļ�����size=32+32
#define FILE_STARTBLOCK_OFF       (32+32) // �ļ���ʼ�飬size=4
#define FILE_MAXSIZE_OFF          (36 +32) // �ļ���󳤶�
#define FILE_FILESIZE_OFF         (40+32) // �ļ��ߴ磬�����룬size=8����26��
#define FILE_FILESIZE_END         (240+32) // ���һ���ļ��ߴ�
#define FILE_VERIFITY_OFF         (248+32) // У�����="easyfile"
#define FILE_FILESIZE_NUM         26

typedef struct EasyFS
{
    u32 block_sum; // �ܿ���
    u32 block_size; // ��ߴ�
    u32 start_block; // ���ڴ洢�ļ����׿���
    u32 files_sum; // �ļ���
    u8 *block_buf; // �����ṩ�Ļ�������size= block_size
    struct MutexLCB *block_buf_mutex; // ����������
    char name[EFS_NAME_LIMIT+1]; // �������ķ�������

    // �����ǵ��͵�flash��������ָ�룬��дһ������оƬ������ʱ����Ϊ����оƬ��
    // ��������,��Щ����������ÿһ��оƬ����Ҫ������Ҫ����NULL��
    // ���Ѳ����涨������д��flashǰ���Ƿ���Ҫ���������buf=NULL������offset
    // ��size�趨�������Ƿ񱻲���
    // true = ��׼���ã�����Ҫ����(��տ�),false = ��Ҫ������
    bool_t   (*query_block_ready)(u32 block,u32 offset,
                                  u8 *buf,u32 size);
    // ����1�飬����: true = �ɹ�������false = ����
    bool_t (*erase_block)(u32 block);
    // ��һ��֮�ڵ����ⳤ�����ݣ�����У�������ȷ����
    // ����У��Ķ����������� ʵ�ʶ�ȡ�������������������򷵻�cn_limit_uint32
    u32 (*read_data)(u32 block,u32 offset,u8 *buf,u32 size,u8 eccflg);
    // ����У��Ķ����������أ�cn_limit_uint32=������������ֵ��ʵ��д������
    u32 (*write_data)(u32 block,u32 offset,u8 *buf,u32 size,u8 eccflg);
}tagEFS;

typedef struct EfsFileInfo
{
    u32 start_block; // �洢���ļ����׿�š�
    u32 max_size; // ���ļ���󳤶ȣ�filesize���ܳ���������ȡ�
    u32 item; // ���ļ�������ڷ�����е�λ��
    u32 filesize; // �ļ���¼��С
}tagEfsFileInfo;



#define EF_OP_READ          (0x01)
#define EF_OP_WRITE         (0x02)
#define EF_OP_CREATE        (0X04)
#define EF_OP_OPEN          (0X08)
#define EF_OP_APPEND        (0X10)
#define EF_WR_NOECC         (0x00)
#define EF_WR_ECC           (0x01)

/* File function return code (FRESULT) */

typedef enum {
    FSR_OK = 0,              /* (0) Succeeded */
    FSR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
    FSR_INT_ERR,             /* (2) Assertion failed */
    FSR_NOT_READY,           /* (3) The physical drive cannot work */
    FSR_NO_FILE,             /* (4) Could not find the file */
    FSR_NO_PATH,             /* (5) Could not find the path */
    FSR_INVALID_NAME,        /* (6) The path name format is invalid */
    FSR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
    FSR_EXIST,               /* (8) Access denied due to prohibited access */
    FSR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
    FSR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
    FSR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
    FSR_NOT_ENABLED,         /* (12) The volume has no work area */
    FSR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
    FSR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any parameter error */
    FSR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
    FSR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
    FSR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
    FSR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_SHARE */
    FSR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
} FSRESULT;

// ��ԭ����file.h���޸Ķ�����
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
// =============================================================================
//��ô���ĵ����ļ�����Ƕ��ʽϵͳ���޶�����壬Ϊ���ݴ�������ļ���ϵͳ��
#define     CN_FILE_NAME_LIMIT       255
#define     CN_FILE_BUF_LEN          (256)
#define     CN_FILE_BUF_LIMIT        (CN_FILE_BUF_LEN + 3)
typedef struct FileRsc
{
    struct EasyFS *efs;
    ptu32_t private; // ��ý����صı�ǣ��京�����ض��ļ�ϵͳ
                     // driver���ͣ���flash file driver)�У�����
                     // ���ļ�(Ŀ¼)��FDT��Ŀ�š�
    sint64_t file_size; // �ļ��ߴ�,�ֽ����������ڻ�����������
    u8 *wr_buf; // д��������ecc��Ҫ
    u8  buf_off; // д������ָ��
                 // ģ��ά��
    sint64_t ptr; // ��ǰ��ָ�룬������ά����ʹ�á�
    char     name[CN_FILE_NAME_LIMIT+1]; // �ļ�����Ŀ¼����,������֧�ֵ��ļ�����                                  //��������ģ������ģ���������ģ��ά����
}tagFileRsc;



FSRESULT Efs_Open(tagFileRsc* fp,const char* path,u8 mode);
FSRESULT Efs_Mount (tagEFS* fs, const char* path,u8 opt);
FSRESULT Efs_Mkfs(tagEFS* fs,const char* path,u8 sfd,u32 au);
FSRESULT Efs_Close (tagFileRsc *fp);
FSRESULT Efs_Remove(const char *filename,const char *fsname);
FSRESULT Efs_Seek(tagFileRsc* fp,u32 newpos);
FSRESULT Efs_Write (tagFileRsc* fp,const void *buf,u32 len,u32* bw);
FSRESULT Efs_Read (tagFileRsc* fp,void* buf,u32 len,u32* br);
FSRESULT Efs_DirRead(const char *fname,const char *fsname,u32 *item);


#ifdef __cplusplus
}
#endif

#endif // __EASY_FLASHFILE_H__

