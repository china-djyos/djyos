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
// �����Ȩ�����ʹ���߷������¶������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�����������У���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�����������У��Լ�����
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
//����ģ��: C��
//���ߣ�������
//�汾��V1.0.0
//�ļ�����: stat��غ���������
//����˵��:
//�޶���ʷ:
//2. ...
//1. ����  : 2014/12/31
//   �޸���: ������
//   �°汾��:
//   �޸�˵��: �½�
//------------------------------------------------------
#ifndef __STAT_H__
#define __STAT_H__

#include <stdint.h>
#include <types.h>
#ifdef __cplusplus
extern "C"{
#endif

//�����������ֲ���ڲ��߼�δʵ�֣���Ӧstruct stat��st_mode��Ա��0-11bit
//open ���õĵ���������
#define   S_ISUID    0004000   //set-user-ID bit
#define   S_ISGID    0002000   //set-group-ID bit (see below)
#define   S_ISVTX    0001000   //sticky bit (see below)
#define   S_IRWXU    00700     //mask for file owner permissions
#define   S_IRUSR    00400     //owner has read permission
#define   S_IWUSR    00200     //owner has write permission
#define   S_IXUSR    00100     //owner has execute permission
#define   S_IRWXG    00070     //mask for group permissions
#define   S_IRGRP    00040     //group has read permission

#define   S_IWGRP    00020     //group has write permission
#define   S_IXGRP    00010     //group has execute permission
#define   S_IRWXO    00007     //mask for permissions for others (not in group)
#define   S_IROTH    00004     //others have read permission
#define   S_IWOTH    00002     //others have write permission
#define   S_IXOTH    00001     //others have execute permission

// ��LINUX�߼�
struct stat
{
    mode_t st_mode; // �ļ���Ӧ��ģʽ���ļ���Ŀ¼��(Ŀǰֻ���˵�16λ��15-12Ϊ�ļ����ͣ���S_IFSOCK��11-0ΪȨ��)
    ino_t st_ino; // inode�ڵ��
    dev_t st_dev; // �豸��
    dev_t st_rdev; // �����豸��
    nlink_t st_nlink; // �ļ���������
    uid_t st_uid; // �ļ�������
    gid_t st_gid; // �ļ������߶�Ӧ����
    off_t st_size; // ��ͨ�ļ�����Ӧ���ļ��ֽ���
    blksize_t st_blksize; // �ļ����ݶ�Ӧ�Ŀ��С
    blkcnt_t st_blocks; // ΰ�����ݶ�Ӧ�Ŀ�����
    time_t st_atime; // �ļ���󱻷��ʵ�ʱ��
    time_t st_mtime; // �ļ���������޸ĵ�ʱ��
    time_t st_ctime; // �ļ�״̬�ı�ʱ��
};

#define S_ISREG(x)      ((x&S_IFMT)==S_IFREG?true:false) // is it a regular file
#define S_ISDIR(x)      ((x&S_IFMT)==S_IFDIR?true:false) // directory?
#define S_ISCHR(x)      ((x&S_IFMT)==S_IFCHR?true:false) // character device?
#define S_ISBLK(x)      ((x&S_IFMT)==S_IFBLK?true:false) // block device?
#define S_ISFIFO(x)     ((x&S_IFMT)==S_IFIFO?true:false) // FIFO (named pipe)?
#define S_ISLNK(x)      ((x&S_IFMT)==S_IFLNK?true:false) // symbolic link?  (Not in POSIX.1-1996.)
#define S_ISSOCK(x)     ((x&S_IFMT)==S_IFSOCK?true:false) // socket?  (Not in POSIX.1-1996.)
// ˽�ж���
#define S_ISFLOW(x)       ((x&S_IFMT)==S_IFFLOW?true:false) // ��ʾ������ΪC�ļ����������壻

int   _EXFUN(stat, (const char *restrict, struct stat *restrict));
int   _EXFUN(fstat, (int, struct stat *restrict));
int   _EXFUN(lstat, (const char *restrict, struct stat *restrict));

#ifdef __cplusplus
}
#endif

#endif      //for __STAT_H__
