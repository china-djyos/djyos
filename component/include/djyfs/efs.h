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
#ifndef __EFS_H__
#define __EFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EFS_NAME_LIMIT            64 // ��������󳤶�,���һ���ֽ�'\0'

// ˵�����ļ�������Ϊ64�ֽڣ�61�ֽ�Ϊ'\0'�������ֽ�Ϊ�ļ�����ECC
#define FILENAME_LIMIT            60 // ��������󳤶�,���һ���ֽ�'\0'
#define ECC_LIMIT                 3
#define EFS_ITEM_LIMIT            256 // ÿ��������ĳ���

//�ļ���������Աƫ����
#define FILE_NAME_OFF             (0 ) // �ļ�����size=64
#define FILE_STARTBLOCK_OFF       (64) // �ļ���ʼ�飬size=4
#define FILE_MAXSIZE_OFF          (68) // �ļ���󳤶�
#define FILE_FILESIZE_OFF         (72) // �ļ��ߴ磬size=4����44
#define FILE_FILESIZE_END         (244) // ���һ�����ļ��ߴ硱
#define FILE_VERIFITY_OFF         (248) // У�����="easyfile"
#define FILE_FILESIZE_NUM         (44)  // �ļ��ߴ����

#define EFS_SUPPORT_MAXFILESIZE   0x3FFFFFFF            //efs�ļ�ϵͳ��֧�ֵ�����ļ���С
#define EFS_FILESIZE_BYTE_NUMBER   4                    //�ļ���С�Ĵ洢ռ�����ֽ�

#define EFS_RAM_ITEM_BYTE          4    //��RAM�д洢�ļ������λ����ռ���ֽ���
//
// efs�ļ�ϵͳ��flash�Ĳ���������
//
struct __efs_media_drv{
    u32 (*efs_write_media) (u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
    u32 (*efs_read_media) (u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize, u8 bFlags);
    bool_t (*efs_erase_media) (u32 dwBlock);
    bool_t (*efs_check_block_ready) (u32 dwBlock, u32 dwOffset, u8 *pBuf, u32 dwSize);
};

struct EfsFileTableItem
{
    char filename[FILENAME_LIMIT+1];
    u32 StartBlock;         //�ļ���ʼ���
    u32 FileSizeLimit;      //�ļ����ߴ�
    u32 FileSize[44];       //�ļ��ߴ�
    char FlagBuffer[8];     //��־��"easyfile",û��'\0'
};
struct EasyFS
{
    u32 block_sum; // �ܿ���
    u64 block_size; // ��ߴ�
    u32 start_block; // ���ڴ洢�ļ�ϵͳ���׿���
    u32 files_sum; // �ļ���
    u8 *file_list_buf; // �ļ������Ļ�������size= �ļ�������С

    bool_t file_sys_install;  //�ļ�ϵͳ�Ƿ��Ѿ���װ����װΪtrue��Ϊ��װΪfalse
    u32 install_options;
    struct MutexLCB *block_buf_mutex; // ����������
    char name[EFS_NAME_LIMIT+1]; // ���ص������

    // �����ǵ��͵�flash��������ָ�룬��дһ������оƬ������ʱ����Ϊ����оƬ��
    // ��������,��Щ����������ÿһ��оƬ����Ҫ������Ҫ����NULL��
    // ���Ѳ����涨������д��flashǰ���Ƿ���Ҫ���������buf=NULL������offset
    // ��size�趨�������Ƿ񱻲���
    // true = ��׼���ã�����Ҫ����(��տ�),false = ��Ҫ������
    struct umedia *media;
    struct __efs_media_drv *drv;
};

struct EfsFileInfo
{
    u32 start_block; // �洢���ļ����׿�š�
    u64 max_size; // ���ļ���󳤶ȣ�filesize���ܳ���������ȡ�
    u32 item; // ���ļ�������ڷ�����е�λ��
    u32 temp_item; //���ڴ��л���ķ�����е�λ��
    u64 filesize; // �ļ�ʵ�ʴ�С
};


#define EF_WR_NOECC         (0x00)
#define EF_WR_ECC           (0x01)

#define     CN_FILE_BUF_LEN          (256)
#define     CN_FILE_BUF_LIMIT        (CN_FILE_BUF_LEN + 3)
struct FileRsc
{
    ptu32_t private; // ��ý����صı�ǣ��京�����ض��ļ�ϵͳ��driver���ͣ���flash file driver)�У�
                    //������ļ�(Ŀ¼)��FDT��Ŀ�š�
    sint64_t file_size; // �ļ��ߴ�,�ֽ����������ڻ�����������
    u8 *wr_buf; // д��������ecc��Ҫ
    u32  buf_off; // д������ָ��
                 // ģ��ά��
    sint64_t ptr; // ��ǰ��ָ�룬������ά����ʹ�á�
    char     name[EFS_NAME_LIMIT];    // �ļ�����Ŀ¼����,������֧�ֵ��ļ�����
                                      //��������ģ������ģ���������ģ��ά����
};


#ifdef __cplusplus
}
#endif

#endif // __EASY_FLASHFILE_H__

