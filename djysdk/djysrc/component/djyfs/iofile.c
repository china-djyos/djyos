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

//
//
//
#include <stdarg.h>
#include <systime.h>
#include <lock.h>
#include <stdlib.h>
#include <string.h>
#include "tricks.h"
#include "iofile.h"
#include "mount.h"
#include "file.h"


static void __Tostat(struct Stat *Src, struct stat *Dest);
//-----------------------------------------------------------------------------
//����: ���ļ�
//����: pathname -- �ļ�·��;
//      flags -- ������־;
//      mode -- ����ģʽ;
//����: -1 -- ʧ��; ���� -- �ļ������;
//��ע: 
//-----------------------------------------------------------------------------
int open(const char *pathname, int fmt, ...)
{
	va_list Params;
	u32 mode = 0; // ��ʱ������
	struct VFile *Fp;
	u32 Mode = 0;
	s32 oflag = fmt;

	if((NULL == pathname)  || ('\0' == *pathname))
		return (-1);

	va_start(Params, fmt);
	mode = va_arg(Params, int);
	va_end(Params);

	switch (oflag & O_ACCMODE) // �򿪷�ʽ
	{
		case O_RDONLY: Mode |= M_READ; break; // ȱʡֵ
		case O_WRONLY: Mode |= M_WRITE; break;
		case O_RDWR: Mode |= M_WRITE | M_READ; break;
		default: return (-1); // ������ʽ����
	}
	oflag &= ~O_ACCMODE;
	
	if(oflag & O_TRUNC)// �ض�
	{
		Mode |= M_TRUNCATE; 
		oflag &= ~O_TRUNC;
	}
	
	if(oflag & O_DIRECTORY)// Ŀ¼
	{
		Mode |= M_DIR;
		oflag &= ~O_DIRECTORY;
	}
	else
	{
		Mode |= M_REG; // Ĭ�ϴ��ļ�
	}
	
	if(oflag & O_APPEND)// ׷��
	{
		Mode |= M_APPEND; 
		oflag &= ~O_APPEND;
	}

	if((oflag & O_CREAT) && (oflag & O_EXCL))
	{
		Mode |= M_CREATE; // ������
		oflag &= ~O_CREAT;
		oflag &= ~O_EXCL;
	}
	else if(oflag & O_CREAT)
	{
		Mode |= (M_CREATE | M_OPEN); // �������
		oflag &= ~O_CREAT;
	}
	else
	{
		Mode |= M_OPEN; // ���򿪲�����
		if(oflag & O_EXCL)
			 oflag &= ~O_EXCL; // ����֪�߼�
	}
	
	if(oflag)
		return (-1);// ������־, Ŀǰ��֧��
		
	Fp = __Open(pathname, Mode);// ���ļ�
	if(!Fp)
		return (-1);// �ļ���ʧ��
	
	return (VFileToHandle(Fp));
}
//-----------------------------------------------------------------------------
//����: �ر��ļ�.
//����: fd -- �ļ������;
//����: 0 -- �ɹ�;
//      -1 -- ��������; -2 -- �ļ����ʹ���; ����;  
//��ע: 
//-----------------------------------------------------------------------------
int close(int fd)
{
	s32 Ret;
	struct VFile *Fp;
	
	Fp = HandleToVFile(fd);
	if(NULL == Fp)
		return (-1);

	Ret = Fp->VOps->VClose(Fp->Private);
	if(!Ret)
		FreeVFile(Fp);// �ɹ����ͷž��

	return (Ret);
}
//-----------------------------------------------------------------------------
//����: ���ļ�.
//����: fd -- �ļ������;
//      buf -- ������;
//      count -- ��Ҫ�������ݴ�С��Byte�ƣ�;
//����: >= 0 -- �ɹ�; -1 -- ʧ��;    
//��ע: 
//-----------------------------------------------------------------------------
int read(int fd, void *buf, size_t count)
{
	struct VFile *Fp;
	s32 Ret;
	
	if(NULL == buf)
		return (-1);// ��������
		
	if(0 == count)
		return (0);
		
	Fp = HandleToVFile(fd);
	if(NULL == Fp)
		return (-1);

	Ret = Fp->VOps->VRead(buf, count, 1, Fp->Private);
	if(0 == Ret)
		Ret = -1;
	
	return (Ret);		
}
//-----------------------------------------------------------------------------
//����: д�ļ�.
//����: fd -- �ļ������;
//      buf -- д����;
//      count -- ��Ҫд�����ݴ�С��Byte�ƣ�;
//����:  >= 0 -- �ɹ�; -1 -- ʧ��; 
//��ע: 
//-----------------------------------------------------------------------------
int write(int fd, const void *buf, size_t count)
{
	struct VFile *Fp;
	s32 Ret;
	
	if(NULL == buf)
		return (-1);// ��������
		
	if(0 == count)
		return (0);
		
	Fp = HandleToVFile(fd);
	if(NULL == Fp)
		return (-1);
	
	Ret = Fp->VOps->VWrite(buf, count, 1, Fp->Private);
	if(0 == Ret)
		Ret = -1;
	
	return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����:  
//��ע: todo: cntl()
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//����: ��ѯ�ļ�״̬
//����: file_name -- �ļ�·����
//      buf -- �ļ�״̬;
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
int fstat(int fd, struct stat *buf)
{
	s32 Ret;
	struct FileContext *FileCt;
	struct FileOperations *FileOps;
	struct Stat FileStat;
	struct VFile *Fp = NULL;

	if(NULL == buf)
		return(-1);

	Lock_MutexPend(g_ptVFMutex, CN_TIMEOUT_FOREVER);// ������,��Ϊ����һ���ļ�������
	Fp = HandleToVFile(fd);
	if(NULL == Fp)
	{
		Ret = -1; // δ�ҵ������
		goto FAIL;
	}

	FileCt = (struct FileContext*)Fp->Private;
	if(FileCt->Property & P_DEV)
	{
		Ret = -1; // todo: �豸�ļ��ݲ�֧��
		goto FAIL;
	}

	FileOps = (struct FileOperations*)(FileCt->FileOps);
	if((NULL == FileOps) || (NULL == FileOps->FileStat))
	{
		Ret = -1; // �ļ�ϵͳ��֧�ָú���
		goto FAIL;
	}

	Ret = FileOps->FileStat(FileCt, NULL, &FileStat, NULL);

FAIL:
	Lock_MutexPost(g_ptVFMutex);// ������
	if(0 == Ret)
		__Tostat(&FileStat, buf);

	return (Ret);
}

//-----------------------------------------------------------------------------
//����: ��ѯ�ļ�״̬
//����: file_name -- �ļ�·����
//      buf -- �ļ�״̬;
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע: todo: �Ƿ���Ҫ����
//-----------------------------------------------------------------------------
int stat(const char *filename, struct stat *buf)
{
	struct PathWalk PathContext;
	u8 IsFound;
	struct FileOperations *FileOps;
	s32 Ret;
	struct Stat FileStat;

	if((NULL == buf) || (NULL == filename) || ('\0' == *filename))
		return(-1);

	Lock_MutexPend(g_ptVFMutex, CN_TIMEOUT_FOREVER);// ������

	__PathInit(&PathContext, filename);

	IsFound = __VPathLookup(&PathContext, 0);

	if(!(PathContext.End->Type & OB_FILE))
	{
		Ret = -1; // Ŀǰֻ֧���ļ�
		goto FAIL;
	}

	FileOps = (struct FileOperations*)__FileOps(&PathContext);

	if((NULL == FileOps) || (NULL == FileOps->FileStat))
	{
		Ret = -1;// ���ļ�ϵͳ���Ͳ�֧��
		goto FAIL;
	}

	Ret = FileOps->FileStat(NULL, PathContext.Path, &FileStat, PathContext.Root);

FAIL:
	Lock_MutexPost(g_ptVFMutex);// ������

	if(0 == Ret)
		__Tostat(&FileStat, buf);

	return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����: file_name -- �ļ�·����
//      buf -- �ļ�״̬;
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
int ftruncate(int fd, off_t length)
{
	struct VFile *Fp;
	struct FileContext *FileCt;
//	struct VCommon *FileCom;
	struct FileOperations *FileOps;

	Fp = HandleToVFile(fd);
	if(NULL == Fp)
		return (-1); // (EBADF)�ļ������Ч����ļ��ѱ��ر�

	FileCt = (struct FileContext*)Fp->Private;
	if(0 == (FileCt->Property & P_REG) || (0 == (FileCt->Property & P_WRITE)))
		return (-1); // ֻ֧���ļ�

	FileOps = (struct FileOperations*)(FileCt->FileOps);
	if((NULL == FileOps) || (NULL == FileOps->FileTruncate))
		return (-1); // ����δע��

	return(FileOps->FileTruncate(FileCt, length));
}

//-----------------------------------------------------------------------------
//����: �ļ������ṹ��ת�ļ������.
//����: Fp -- �ļ������ṹ��;
//����: �ļ������.
//��ע: 
//-----------------------------------------------------------------------------
int VFileToHandle(struct VFile *Fp)
{
	int Fd;
	struct VFilePool *Pool;
	
	Pool = PoolOf(Fp);

	Fd = ((VFILE_NUM - 1) - (Fp - Pool->Space)) & 0xFFFF;// note: ����������ļ��������ǵ�ַ����
	Fd |= ((Pool->ID << 16) & 0xFFFF0000);
	
	return (Fd);
}
//-----------------------------------------------------------------------------
//����: �ļ������ת�ļ������ṹ��
//����: fd -- �ļ������;
//����: �ļ������ṹ��
//��ע: 
//-----------------------------------------------------------------------------
struct VFile *HandleToVFile(int Fd)
{
	u16 ID, Offset;
	struct VFilePool *Pool;
	
	ID = (Fd >> 16) & 0xFFFF;// ���ڵ�VFilePool��ID
	
	Offset = ((VFILE_NUM - 1) - (Fd & 0xFFFF));// ��Pool�е�ƫ��
	
	Pool = g_ptVFilePool;
	
	while(Pool)// ����VFile���ڵ�pool
	{
		if(Pool->ID != ID)
			Pool = Pool->Next;
		else
			break;
	}
	
	if(Pool)
		return (Pool->Space + Offset);
	else
		return (NULL);
}
//-----------------------------------------------------------------------------
//����: ɾ���ļ���Ŀ¼
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע: todo
//-----------------------------------------------------------------------------
int unlink(const char *pathname)
{
    if(remove(pathname))
    	return (-1);
    return (0);
}
//-----------------------------------------------------------------------------
//����: ��Ŀ¼
//����:
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע: todo����ʱ�߼�
//-----------------------------------------------------------------------------
DIR *opendir(const char *name)
{
	DIR *DirD;
    s32 Handle;

    if((NULL == name)  || ('\0' == *name))
    	return (NULL);

    Handle = open(name, O_DIRECTORY | O_RDWR, 0);
    if(-1 == Handle)
    {
    	return (NULL);
    }

    DirD = malloc(sizeof(*DirD));
    if(NULL == DirD)
    {
    	close(Handle);
    }
    memset(DirD, 0, sizeof(*DirD));

    DirD->__fd = (void*)Handle;
    return (DirD);
}
//-----------------------------------------------------------------------------
//����: ��Ŀ¼��
//����:
//����:
//��ע: todo,��ʱ�߼�
//-----------------------------------------------------------------------------
struct dirent *readdir(DIR *dir)
{
	s32 handle, res;
	struct VFile *Fp;
	struct Dirent DirentInfo;
	struct dirent *ret;
	u8 i;

	if(NULL == dir)
		return (NULL);

	handle = (s32)dir->__fd;

	Fp = HandleToVFile(handle);
	if(NULL == Fp)
		return (NULL);

	res = ReadDir(Fp, &DirentInfo);
	if(1 == res)
		return (NULL); // ��ȫ��������

	ret = (struct dirent *)dir->__data;
	if(NULL == ret)
	{
		ret = malloc(sizeof(*ret));
		if(NULL == ret)
			return (NULL);
		memset(ret, 0, sizeof(*ret));
		dir->__data = (void*)ret;
	}

	for(i = 0; i < NAME_MAX; i++)
	{
		ret->d_name[i] = DirentInfo.Name[i];
		if('\0' == DirentInfo.Name[i])
			break;
	}

	return (ret);
}
//-----------------------------------------------------------------------------
//����: ����Ŀ¼
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��
//��ע: todo,��ʱ�߼�
//-----------------------------------------------------------------------------
int mkdir (const char *filename, mode_t mode)
{
    s32 Ret;

	Ret = open(filename, O_DIRECTORY | O_CREAT | O_EXCL | O_RDWR, mode);
	if(-1 != Ret)
	{
		close(Ret);
		return (0);
	}

	return (-1);

}
//-----------------------------------------------------------------------------
//����: �ر�Ŀ¼
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��
//��ע: todo,��ʱ�߼�
//-----------------------------------------------------------------------------
int closedir(DIR *dir)
{
    s32 Ret;

	if(NULL == dir)
    	return (0);

    if(dir->__data)
    	free((void*)dir->__data);

    Ret = close((s32)dir->__fd);
    if(Ret < 0)
    	return (-1);

    free(dir);
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __Tostat(struct Stat *Src, struct stat *Dest)
{
	memset(Dest, 0, sizeof(*Dest));

	Dest->st_size = (off_t)Src->Size;
	if(Src->Property & P_DIR)
		Dest->st_mode = S_IFDIR;
	else if(Src->Property & P_REG)
		Dest->st_mode = S_IFREG;
	else if(Src->Property & P_LINK)
		Dest->st_mode = S_IFLNK;
	// ����������ʱ��֧��
}
