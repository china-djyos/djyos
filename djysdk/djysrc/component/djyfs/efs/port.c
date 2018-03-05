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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "efs_port.h"


struct EFS_Dir{
    const char *Root;
    struct Dirent *CurDirent;
    u32    item; // wenjian
};

extern struct FileOperations EfsFileOps;
//-----------------------------------------------------------------------------
//����:
//����: 
//����: ��NULL -- �ɹ�; NULL -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
struct FileContext *EfsOpen(const char *Path, u32 Mode, const char *Root)
{
    FSRESULT Ret;
    u8 LocalMode = 0;
    struct FileContext *FileCt;
    struct FileBuf *Buf;

    if(Mode & M_LINK)
        return (NULL); // ��ǰ��֧�ֵ�ģʽ

    if((NULL != Path) && (Mode & M_DIR))// ��EFS��Ŀ¼���ܴ������
        return (NULL);
    
    FileCt = AllocContext();
    if(!FileCt)
        return (NULL);
        
    FileCt->FileOps = (void *)&EfsFileOps;
    if(Mode & M_READ)
    {
        LocalMode |= EF_OP_READ;
        FileCt->Property |= P_READ;
    }    
    
    if(Mode & M_WRITE)
    {
        LocalMode |= EF_OP_WRITE;
        FileCt->Property |= P_WRITE;
    }    
    
    if(Mode & M_CREATE)
        LocalMode |= EF_OP_CREATE;

    if(Mode & M_OPEN)
        LocalMode |= EF_OP_OPEN;

    if(Mode & M_APPEND)
        LocalMode |= EF_OP_APPEND;

    if(!(Mode & M_TYPE))// ����δָ������������Ҫ������
        Mode |= M_REG;
        
    if(Mode & M_REG)// ���ļ�
    {
        tagFileRsc *NewFile = malloc(sizeof(tagFileRsc));
        if(NewFile)
        {    
            Ret = Efs_FindFs(NewFile,Root);
            if(FSR_OK == Ret)
            {
                Ret = Efs_Open(NewFile, Path, LocalMode);// ���ļ�
                if(FSR_OK == Ret)
                {
                    if(!AllocFileBuf(&Buf, 0))
                    {
                        FileCt->Property |= P_REG;
                        FileCt->Private = (void *)(NewFile);
                        FileCt->Buf = Buf;
                        return (FileCt);// �ɹ�
                    }
                }
            }
            // ���ļ�ʧ��
            free(NewFile);
        }    
    }

    // easy nor ��֧�ִ�Ŀ¼����Ϊֻ��һ����Ŀ¼,�������Ŀ¼��ʧ��
    if((Mode & M_DIR) && (!(Mode & M_CREATE)) )// ��Ŀ¼
    {
        struct EFS_Dir *RootDir = malloc(sizeof(*RootDir));
        if(RootDir)
        {
            RootDir->Root = Root;
            RootDir->CurDirent = NULL;
            FileCt->Property |= P_DIR;
            FileCt->Buf = NULL;
            FileCt->Private = (void *)(RootDir);
            return (FileCt);// �ɹ�
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
s32 EfsClose(struct FileContext *FileCt)
{
    FSRESULT Ret;
    
    switch(FileCt->Property & P_TYPE)
    {
        case P_REG:
        {
            tagFileRsc *File = (tagFileRsc *)(FileCt->Private);
            Ret = Efs_Close(File);
            if(FSR_OK == Ret)
            {
                FreeFileBuf(FileCt->Buf);
                break;
            }
            return (-2);// ʧ��
        }

        case P_DIR:
        {
            struct EFS_Dir *RootDir = (struct EFS_Dir *)(FileCt->Private);
            if(RootDir->CurDirent)
                free(RootDir->CurDirent);
            free(RootDir);
            break;
        }
        default:
            return (-2);// ʧ��
    }
    
    FreeContext(FileCt);
    return (0);
    
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -3 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsSync(struct FileContext *FileCt)
{
    return (0);
}
//-----------------------------------------------------------------------------
//����:
//����: 
//����: 0 -- �ɹ�; -1 -- ����ʧ��; -2 -- ��������;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsSeek(struct FileContext *FileCt, s64 Offset, int Whence)
{
    FSRESULT Ret;
    u32 NewPos;
    tagFileRsc *File = (tagFileRsc*)FileCt->Private;
    
    switch(Whence)
    {
        case V_SEEK_SET: NewPos = Offset;break;
        case V_SEEK_CUR: NewPos = Offset + File->ptr;break;
        case V_SEEK_END: NewPos = Offset + File->file_size; break;
        default: return (-2);// ��������
    }

    Ret = Efs_Seek(File,NewPos);
    if( FSR_OK != Ret)
        return (-1);
    return (0);
}

//-----------------------------------------------------------------------------
//����: nor ֻ֧��ɾ���ļ��б�����һ��˭��
//����: 
//����: 
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsDelete(const char *Path, const char *Root)
{
    FSRESULT Ret;

    Ret = Efs_Remove(Path,Root);
    if(FSR_OK != Ret)
        return (-3);
    return (0);    
}

//-----------------------------------------------------------------------------
//����:
//����: 
//����: !0 -- �ɹ�; 0 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsWrite(const void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{
    u32 Result;
    tagFileRsc *File = (tagFileRsc*)FileCt->Private;
    u32 Length = Size * Nmemb;
    
    Efs_Write(File, Buf, Length, &Result);
    
    return((s32)Result);
}

//-----------------------------------------------------------------------------
//����:
//����: 
//����: !0 -- �ɹ�; 0 -- ʧ��;
//��ע: 
//-----------------------------------------------------------------------------
s32 EfsRead(void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{
    u32 Result;
    tagFileRsc *File = (tagFileRsc*)FileCt->Private;
    u32 Length = Size * Nmemb;
    
    Efs_Read(File, Buf, Length, &Result);
    
    return((s32)Result);
}

//-----------------------------------------------------------------------------
//����:
//����:
//����: 0 -- �ɹ�; -1 -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 EfsStat(struct FileContext *FileCt, const char *Path,
                    struct Stat *Buf, const char *Root)
{
    tagFileRsc *File;
    u32 FileSize = 0;
    FSRESULT Ret;

    if(FileCt)
    {
        File = (tagFileRsc *)FileCt->Private;
        Buf->Property = FileCt->Property;
        Buf->Size = File->file_size;
        return (0);
    }

    if(Path && Root)
    {
        Ret = Efs_Stat(Path, Root, &FileSize);
        if(FSR_OK != Ret)
            return (-1);

        Buf->Size = FileSize;
        Buf->Property = P_READ | P_WRITE | P_REG;
    }
    //��Ŀ¼�����ǣ��Ѿ������ڴ�֮����
    return (0);
}
//-----------------------------------------------------------------------------
//����: ��Ŀ¼����
//����:
//����: 1 -- ����,ȫ������;
//      -1 -- ��������; -2 -- ������֧��; -3 -- ��������;
//      0 -- �ɹ���ȡ����;
//��ע: ÿ��һ��,����һ��Ŀ¼��
//-----------------------------------------------------------------------------
s32 EfsDirRead(struct FileContext *FileCt, struct Dirent *Content)
{
    FSRESULT ret;
    struct EFS_Dir *RootDir = (struct EFS_Dir *)(FileCt->Private);

    if( (NULL == FileCt) || (NULL == Content) )
        return (-1);
    
    if(NULL == RootDir->CurDirent)
    {
        RootDir->CurDirent = (struct Dirent *)malloc(sizeof(struct Dirent));
        if(NULL == RootDir->CurDirent)
            return (-3);

        memset(RootDir->CurDirent, 0x0, sizeof(struct Dirent));
        RootDir->item = 1;    //��ʾ�ӵ�һ���ļ���ʼɨ��
    }

    ret = Efs_DirRead(RootDir->CurDirent->Name, RootDir->Root, &(RootDir->item));
    if(FSR_OK == ret)
    {
        RootDir->CurDirent->Property |= P_REG;
        *Content = *(RootDir->CurDirent);
        return (0);
    }

    if(FSR_NO_FILE == ret)
        return (1);

    if(FSR_INVALID_PARAMETER == ret)
        return (-1);
    return (0);
}
