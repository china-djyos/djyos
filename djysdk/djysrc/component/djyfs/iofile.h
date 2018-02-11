//-----------------------------------------------------------------------------
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

#ifndef __IOFILE_H__
#define __IOFILE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "vfile.h"


//
// �˽��Ʊ�ʾ(������linux)
//
#ifndef O_ACCMODE
 #define O_ACCMODE                  (00000003)/* ģʽ����λ */
#else
 #warning "O_ACCMODE" duplicate definition!
#endif
#ifndef O_RDONLY
 #define O_RDONLY                   (00000001)
#else
 #warning "O_RDONLY" duplicate definition!
#endif
#ifndef O_WRONLY
 #define O_WRONLY                   (00000002)
#else
 #warning "O_WRONLY" duplicate definition!
#endif
#ifndef O_RDWR
 #define O_RDWR                     (O_RDONLY|O_WRONLY)
#else
 #warning "O_RDWR" duplicate definition!
#endif
#ifndef O_CREAT
 #define O_CREAT                    (00000100)
#else
 #warning "O_CREATE" duplicate definition!
#endif
#ifndef O_EXCL
 #define O_EXCL                     (00000200)
#else
 #warning "O_EXCL" duplicate definition!
#endif
#ifndef O_NOCTTY
 #define O_NOCTTY                   (00000400)
#else
 #warning "O_NOCTTY" duplicate definition!
#endif
#ifndef O_TRUNC
 #define O_TRUNC                    (00001000)
#else
 #warning "O_TRUNC" duplicate definition!
#endif
#ifndef O_APPEND
 #define O_APPEND                   (00002000)
#else
 #warning "O_APPEND" duplicate definition!
#endif
#ifndef O_NONBLOCK
 #define O_NONBLOCK                 (00004000)
#else
 #warning "O_NONBLOCK" duplicate definition!
#endif
#ifndef O_DSYNC
 #define O_DSYNC                    (00010000)
#else
 #warning "O_DSYNC" duplicate definition!
#endif
#ifndef O_DIRECT
 #define O_DIRECT                   (00040000)
#else
 #warning "O_DIRECT" duplicate definition!
#endif
#ifndef O_LARGEFILE
 #define O_LARGEFILE                (00100000)
#else
 #warning "O_LARGEFILE" duplicate definition!
#endif
#ifndef O_DIRECTORY
#define O_DIRECTORY                 (00200000)
#else
 #warning "O_DIRECTORY" duplicate definition!
#endif
#ifndef O_NOFOLLOW
 #define O_NOFOLLOW                 (00400000)
#else
 #warning "O_NOFOLLOW" duplicate definition!
#endif
#ifndef O_NOATIME
#define O_NOATIME                   (01000000)
#else
 #warning "O_NOATIME" duplicate definition!
#endif
#ifndef O_CLOEXEC
 #define O_CLOEXEC                  (02000000)
#else
 #warning "O_CLOEXEC" duplicate definition!
#endif
#ifndef S_IFMT
 #define S_IFMT                     (00170000)// 1111-0000-0000-0000 �ļ���������λ
#else
 #warning "S_IFMT" duplicate definition!
#endif
#ifndef S_IFSOCK
 #define S_IFSOCK                   (00140000)// 1100-0000-0000-0000
#else
 #warning "S_IFSOCK" duplicate definition!
#endif
#ifndef S_IFLNK
 #define S_IFLNK                    (00120000)// 1010-0000-0000-0000
#else
 #warning "S_IFlINK" duplicate definition!
#endif
#ifndef S_IFREG
 #define S_IFREG                    (00100000)// 1000-0000-0000-0000
#else
 #warning "S_IFREG" duplicate definition!
#endif
#ifndef S_IFBLK
 #define S_IFBLK                    (00060000)// 0110-0000-0000-0000
#else
 #warning "S_IFBLK" duplicate definition!
#endif
#ifndef S_IFDIR
 #define S_IFDIR                    (00040000)// 0100-0000-0000-0000
#else
 #warning "S_IFDIR" duplicate definition!
#endif
#ifndef S_IFCHR
 #define S_IFCHR                    (00020000)// 0010-0000-0000-0000
#else
 #warning "S_IFCHR" duplicate definition!
#endif
#ifndef S_IFIFO
 #define S_IFIFO                    (00010000)// 0001-0000-0000-0000
#else
 #warning "S_IFIFO" duplicate definition!
#endif


//�����������ֲ���ڲ��߼�δʵ��
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

#define S_ISREG(x)      ((x&S_IFMT)==S_IFREG?true:false) //is it a regular file
#define S_ISDIR(x)      ((x&S_IFMT)==S_IFDIR?true:false) //directory?
#define S_ISCHR(x)      ((x&S_IFMT)==S_IFCHR?true:false) //character device?
#define S_ISBLK(x)      ((x&S_IFMT)==S_IFBLK?true:false) //block device?
#define S_ISFIFO(x)     ((x&S_IFMT)==S_IFIFO?true:false) //FIFO (named pipe)?
#define S_ISLNK(x)      ((x&S_IFMT)==S_IFLNK?true:false) //symbolic link?  (Not in POSIX.1-1996.)
#define S_ISSOCK(x)      ((x&S_IFMT)==S_IFSOCK?true:false) //socket?  (Not in POSIX.1-1996.)


#ifndef NAME_MAX
#define NAME_MAX  64
#endif
//
// todo
//
struct dirent
{
   long d_ino; /* inode number �����ڵ�� */
   off_t d_off; /* offset to this dirent ��Ŀ¼�ļ��е�ƫ�� */
   unsigned short d_reclen; /* length of this d_name �ļ����� */
   unsigned char d_type; /* the type of d_name �ļ����� */
   char d_name [NAME_MAX+1]; /* file name (null-terminated) �ļ������255�ַ� */
};

//
// todo
//
struct __dirstream
{
    void *__fd; /* `struct hurd_fd' pointer for descriptor.   */
    char *__data; /* Directory block.   */
    int __entry_data; /* Entry number `__data' corresponds to.   */
    char *__ptr; /* Current pointer into the block.   */
    int __entry_ptr; /* Entry number `__ptr' corresponds to.   */
    size_t __allocation; /* Space allocated for the block.   */
    size_t __size; /* Total valid data in the block.   */
};

typedef struct __dirstream DIR;



//
//
//
struct stat
{
    //added by zqf for the comportable for the free software--todo
    mode_t     st_mode;       //�ļ���Ӧ��ģʽ���ļ���Ŀ¼��

    ino_t      st_ino;       //inode�ڵ��

    dev_t      st_dev;        //�豸����

    dev_t      st_rdev;       //�����豸����

    nlink_t    st_nlink;      //�ļ���������

    uid_t      st_uid;        //�ļ�������

    gid_t      st_gid;        //�ļ������߶�Ӧ����

    off_t      st_size;       //��ͨ�ļ�����Ӧ���ļ��ֽ���

    blksize_t st_blksize;    //�ļ����ݶ�Ӧ�Ŀ��С

    blkcnt_t   st_blocks;     //ΰ�����ݶ�Ӧ�Ŀ�����

    time_t     st_atime;      //�ļ���󱻷��ʵ�ʱ��

    time_t     st_mtime;      //�ļ���������޸ĵ�ʱ��

    time_t     st_ctime;      //�ļ�״̬�ı�ʱ��
};



//
//  ��������
//
int VFileToHandle(struct VFile *Fp);
struct VFile *HandleToVFile(int Fd);

//
// ��ʵ�ֵĺ�������
//
//int open(const char *pathname, int oflag, unsigned int mode);
int open(const char *pathname, int fmt, ...);
int close(int fd);
int read(int fd, void *buf, size_t count);
int write(int fd, const void *buf, size_t count);
int fstat(int fd, struct stat *buf);
int stat(const char *filename, struct stat *buf);
int ftruncate(int fd, off_t length);
DIR *opendir(const char *name); // todo
struct dirent *readdir(DIR *dir); // todo
int closedir(DIR *dir); // todo
int mkdir (const char *filename, mode_t mode); // todo

#ifdef __cplusplus
}
#endif

#endif/* __IOFILE_H__ */
