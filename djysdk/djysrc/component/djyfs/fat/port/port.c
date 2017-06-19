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
#include <string.h>
#include "fat_port.h"

extern struct FileOperations g_tFATFileOps; // ./mount.c�ļ��ж���
extern char *PathCopy(const char *Root, const char *RelPath, char(*Rebuild)(char *Path));

//-----------------------------------------------------------------------------
//����:
//����: 
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
struct FileContext *FATOpen(const char *Path, u32 Mode, const char *Root)
{
	char *LocalPath;
	struct FileContext *FileCt;
	struct FileBuf *Buf;
	FRESULT Res = FR_OK;
	u8 LocalMode = 0;
	
	// ���ų���֧�ֹ���
	if((Mode & M_APPEND) || (Mode & M_LINK))
		return (NULL);// ��ǰ��֧�ֵ�ģʽ,(Mode & M_TRUNCATE)��û�д���
	
	LocalPath = PathCopy(Root, Path, NULL);// todo:���Ƿ��������
	
	FileCt = AllocContext();
	if(!FileCt)
		return (NULL);

	FileCt->FileOps = (void*)&g_tFATFileOps;

	// todo: ��д���Զ�û����ô����
	if(Mode & M_READ)
	{
		LocalMode |= FA_READ;
		FileCt->Property |= P_READ;
	}	
	
	if(Mode & M_WRITE)
	{
		LocalMode |= FA_WRITE;
		FileCt->Property |= P_WRITE;
	}	
	
	if(Mode & M_CREATE)
	{
		if((Mode & M_OPEN))
			LocalMode |= FA_OPEN_ALWAYS;// ����ļ�������,�򴴽�һ���ٴ�.��˱��ܱ�֤fopen��һ���Դ�һ���Ѵ����ļ�
		else
			LocalMode |= FA_CREATE_NEW;	
	}	
	
	if(!(Mode & M_TYPE))// ����δָ������������Ҫ������
		Mode |= M_REG | M_DIR;
		
	if(Mode & M_REG)// ���ļ�
	{
		FIL *NewFile = malloc(sizeof(FIL));
		if(NewFile)
		{	
			Res = f_open(NewFile, LocalPath, LocalMode);// ���ļ�
			if((FR_OK == Res) || 
			   ((FR_EXIST == Res) && (Mode & M_OPEN)))//���Ѵ����ļ�
			{
				if(0 == AllocFileBuf(&Buf, 0)) // Ϊ�ļ�����������
				{
					FileCt->Property |= P_REG;
					FileCt->Private = (void *)(NewFile);
					FileCt->Buf = Buf;
					return (FileCt);// �ɹ�
				}
				else
				{
					f_close(NewFile); // ʧ��
					free(NewFile);
					FreeContext(FileCt);
					if((Mode & M_CREATE) && (!(Mode & M_OPEN)))
						f_unlink(LocalPath);
					return (NULL);
				}
			}
			// ���ļ�ʧ��
			free(NewFile);
		}	
	}
	
	if(Mode & M_DIR)// ��Ŀ¼
	{
		_DIR *NewDir;
		
		if(LocalMode & FA_CREATE_NEW)// �贴��������ͼ����Ŀ¼
		{
			Res = f_mkdir(LocalPath);
		}
		if(FR_OK == Res)
		{
			NewDir = malloc(sizeof(_DIR));
			if(NewDir)
			{
				Res = f_opendir(NewDir, LocalPath);
				if(FR_OK == Res)
				{
					FileCt->Property |= P_DIR;
					FileCt->Private = (void *)(NewDir);
					FileCt->Buf = NULL;
					return (FileCt);// �ɹ�
				}
				// ��Ŀ¼ʧ��
				free(NewDir);
			}
		}
	}
	
	// ʧ��
	FreeContext(FileCt);
	return (NULL);

}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -2 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 FATClose(struct FileContext *FileCt)
{
	FRESULT Res;
	
	switch(FileCt->Property & P_TYPE)
	{
		case P_REG:
		{
			FIL *File = (FIL *)(FileCt->Private);
			Res = f_close(File);
			if(FR_OK == Res)
			{
				FreeFileBuf(FileCt->Buf);
				break;
			}
			return (-2);// ʧ��
		}
		case P_DIR:
		{
			_DIR *Dir = (_DIR*)(FileCt->Private);
			Res = f_closedir(Dir);
			if(FR_OK == Res)
				break;
			return (-2);// ʧ��
		}
		default:
			return (-2);// ʧ��
	}
	
	free(FileCt->Private);// �ͷ�"FIL"��"_DIR"
	FreeContext(FileCt);
	return (0);
	
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -3 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 FATSync(struct FileContext *FileCt)
{
	FRESULT Res;
    FIL *File = (FIL*)(FileCt->Private);
	
	Res = f_sync(File);
    if (FR_OK != Res)
        return (-3);
		
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -1 -- ����ʧ��; -2 -- ��������;
//��ע: 
//-----------------------------------------------------------------------------
s32 FATSeek(struct FileContext *FileCt, s64 Offset, int Whence)
{
	FRESULT Res;
    DWORD NewPos;
    FIL *File = (FIL*)FileCt->Private;
	
	switch(Whence)
	{
		case V_SEEK_SET: NewPos = Offset;break;
		case V_SEEK_CUR: NewPos = Offset + File->fptr;break;
		case V_SEEK_END: NewPos = Offset + File->fsize; break;
		default: return (-2);// ��������
	}
	
	Res = f_lseek(File, NewPos);
    if (FR_OK != Res)
        return (-1);
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 
//��ע: 
//-----------------------------------------------------------------------------
s32 FATDelete(const char *Path, const char *Root)
{
	FRESULT Res;
	char *LocalPath = PathCopy(Root, Path, NULL);
	
	Res = f_unlink(LocalPath);
	if(FR_OK != Res)
		return (-3);
	return (0);	
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: !0 -- �ɹ�; 0 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 FATWrite(const void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{

	u32 Result;
    FIL *File = (FIL*)FileCt->Private;	
	u32 Length = Size * Nmemb;
	
	f_write(File, Buf, Length, &Result);
    
	return((s32)Result);
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: !0 -- �ɹ�; 0 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 FATRead(void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{

	u32 Result;
    FIL *File = (FIL*)FileCt->Private;	
	u32 Length = Size * Nmemb;
	
	f_read(File, Buf, Length, &Result);
	
	return((s32)Result);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 FATStat(struct FileContext *FileCt, const char *Path, struct Stat *Buf, const char *Root)
{
	FIL *File;
	char *LocalPath;
	FILINFO FileInfo = {0};// Ҫ��ʼ��������Դ������ܷ�
	FRESULT Res;

	if(FileCt)
	{
		if(FileCt->Property & P_REG)
		{
			File = (FIL*)FileCt->Private;
			Buf->Size = File->fsize;
		}
		else
		{
			Buf->Size = 0;
		}

		Buf->Property = FileCt->Property;
		return (0);
	}

	if(Path && Root)
	{
		LocalPath = PathCopy(Root, Path, NULL);
		Res = f_stat(LocalPath, &FileInfo);
		if(FR_OK != Res)
			return (-1);

		Buf->Size = FileInfo.fsize;

		if(FileInfo.fattrib & AM_RDO) //ֻ��
			Buf->Property = P_READ;
		else
			Buf->Property = P_READ | P_WRITE;

		if(FileInfo.fattrib & AM_DIR)// Ŀ¼
			Buf->Property |= P_DIR;

		if(FileInfo.fattrib & AM_ARC)// �ĵ�
			Buf->Property |= P_REG;

		return (0);
	}

	return (-1);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע: todo: off_t���ʹ���
//-----------------------------------------------------------------------------
s32 FATTruncate(struct FileContext *FileCt, off_t NewSize)
{
	FIL *File;
	FRESULT Res;

	File = (FIL*)(FileCt->Private);

	File->fptr = (DWORD)NewSize; // todo

	Res = f_truncate(File);
	if(FR_OK == Res)
		return (0);
	return (-1);
}

//-----------------------------------------------------------------------------
//����: ��Ŀ¼����
//����:
//����: 1 -- ����,ȫ������;
//      -1 -- ��������; -2 -- ������֧��; -3 -- ��������;
//      0 -- �ɹ���ȡ����;
//��ע: ÿ��һ��,����һ��Ŀ¼��
//-----------------------------------------------------------------------------
s32 FATDirRead(struct FileContext *pFileCt, struct Dirent *pContent)
{
	FILINFO info;
	char lfname[256];
	FRESULT res;
	char *des, *src;
	u8 len, i;
	s32 ret = 0;
	_DIR *dir = (_DIR *)pFileCt->Private;

#if _USE_LFN
	memset(lfname, 0x0, 256);
	info.lfname = lfname;
#endif
	res = f_readdir(dir, &info);
	if(FR_OK !=res)
		return (-3);
		
	if(0 == info.fname[0])
		return (1);

#if _USE_LFN
	if(0x7E == info.fname[6])
	{
		if(0x0 != info.lfname[0])
		{
			strcpy(pContent->Name, info.lfname); // ���ļ���
		}
		else
		{
			strcpy(pContent->Name, info.fname);
			ret = -3; // ���ļ��������޷���������ǰ�ַ�����֧�֣�
		}
	}
	else
#endif
	{
		strcpy(pContent->Name, info.fname); // 8.3�ļ���
	}

	switch(info.fattrib & 0x30)
	{
		case AM_ARC: pContent->Property = P_REG; break;
		case AM_DIR: pContent->Property = P_DIR; break;
		default: return (-3); // ��δ֧�ֵ��ļ���ʽ
	}
	return (ret);
}