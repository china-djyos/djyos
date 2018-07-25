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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#if 0
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: !NULL -- 成功; NULL -- 失败;
//备注: 
//-----------------------------------------------------------------------------
struct FileContext *YAF2Open(const char *Path, u32 Mode, const char *Root)
{
	int YFlag = 0;
    int YMode = S_IREAD | S_IWRITE;
	char *YPath = PathCopy(Root, Path, NULL);
	struct FileContext *FileCt;
	struct FileBuf *Buf;
	
	FileCt = AllocContext();
	if(!FileCt)
		return (NULL);
		
	FileCt->FileOps = (void *)&g_tYAF2FileOps;

	if((Mode & M_READ) && (Mode & M_WRITE))
	{
		YFlag |= O_RDWR;
		FileCt->Property = P_READ | P_WRITE;
	}
	else if(Mode & M_READ)
	{
		YFlag |= O_RDONLY;
		FileCt->Property = P_READ;
	}
	else if(Mode & M_WRITE)
	{
		YFlag |= O_WRONLY;
		FileCt->Property = P_WRITE;
	}
	else
		goto FAIL;// 参数错误
	
	if(Mode & M_CREATE)
	{
		if(Mode & M_OPEN)
			YFlag |= O_CREAT;// 创建或打开已存在
		else
			YFlag |= O_CREAT | O_EXCL;// 仅创建
	}
		
	if(Mode & M_TRUNCATE)
		YFlag |= O_TRUNC;
		
	if(Mode & M_APPEND)
		YFlag |= O_APPEND;	

	if(!(Mode & M_TYPE))// 参数未指定，则两个都要尝试下
		Mode |= M_REG | M_DIR;
		
	if(Mode & M_REG)// 文件类型
	{
		int Handle;
		Handle = yaffs_open(YPath, YFlag, YMode);
		if(Handle < 0)
			goto FAIL;//todo: 已存在文件是何结果待检验

		if(AllocFileBuf(&Buf, 0))// 为文件创建缓冲区
		{
			yaffs_close(Handle);
			if((Mode & M_CREATE) && (!(Mode & M_OPEN))) // 失败则删除已创建的文件
				yaffs_unlink(YPath);
			goto FAIL;
		}
		
		FileCt->Property |= P_REG;
		FileCt->Private = (void *)Handle;
		FileCt->Buf = Buf;
		return (FileCt);// 成功
	}

	if(Mode & M_DIR)// 目录类型
	{
		yaffs_DIR *Dir;
		if(YFlag & O_CREAT)
		{
			int Ret;
			Ret = yaffs_mkdir(YPath, YMode);
			if(Ret < 0)// 文件已存在,则表示创建错误;或没有空间
				goto FAIL;
		}

		Dir = yaffs_opendir(YPath);
		if(NULL == Dir)
			goto FAIL;

		FileCt->Property |= P_DIR;
		FileCt->Private = (void *)Dir;
		FileCt->Buf = NULL;
		return (FileCt);// 成功
	}

FAIL: //失败
	FreeContext(FileCt);
	return (NULL);
		

}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Close(struct FileContext *FileCt)
{
	int Ret;
	
	switch(FileCt->Property & P_TYPE)
	{
		case P_REG:
		{
			int File = (int)FileCt->Private;
			Ret = yaffs_close(File);
			if(!Ret)
			{
				FreeFileBuf(FileCt->Buf);
				break;// 成功
			}
			return (-2);// 失败
		}
		case P_DIR:
		{
			yaffs_DIR *Dir = (yaffs_DIR*)FileCt->Private;
			Ret = yaffs_closedir(Dir);
			if(!Ret)
				break;// 成功
			return (-2);// 失败
		}
		default:
			return (-2);// 失败
	}
	
	free(FileCt);
	return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Seek(struct FileContext *FileCt, s64 Offset, s32 Whence)
{
	Y_LOFF_T Ret;
	int Handle = (int)(FileCt->Private);
	
	Ret = yaffs_lseek(Handle, Offset, Whence);
    if (-1 == Ret)
		return (-1);
		
	return (0);
	
}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 0 -- 成功; -3 -- 失败;
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Sync(struct FileContext *FileCt)
{
	int Ret;
	int Handle = (int)FileCt->Private;

	Ret = yaffs_flush(Handle);
	if(Ret)
		return (-3);
	return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 0 -- 成功; -3 -- 错误;
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Delete(const char *Path, const char *Root)
{
	int Ret;
	char *YPath = PathCopy(Root, Path, NULL);
	u32 Length;
	Length = strlen(Path);
	
	// 检查路径是否说明,删除的是目录还是文件;如果'/'结尾，表示删除的是目录
	if(Path[Length] != '/')
	{
		Ret = yaffs_unlink(YPath);
		if(!Ret)
			return (0);// 成功
	}
	
	Ret = yaffs_rmdir(YPath);
	if(!Ret)
		return (0);// 成功
		 
	return (-3);
}

//-----------------------------------------------------------------------------
//功能:
//参数: Size -- 保证不为零
//返回: >0 -- 成功; 0 -- 失败;
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Write(const void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{
	int Ret;
    int File = (int)FileCt->Private;
    u32 AllSize = Size * Nmemb;

    Ret = yaffs_write(File, Buf, AllSize);
	if(-1 == Ret)
		return (0);// 失败

    return (Ret/Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数: Size -- 保证不为零
//返回: >0 -- 成功; 0 -- 失败;
//备注: 
//-----------------------------------------------------------------------------
s32 YAF2Read(void *Buf, u32 Size, u32 Nmemb, struct FileContext *FileCt)
{
	int Ret;
    int File = (int)FileCt->Private;
    u32 AllSize = Size * Nmemb;

    Ret = yaffs_read(File, Buf, AllSize);
	if(-1 == Ret)
		return (0);// 失败

    return (Ret/Size);
}

//-----------------------------------------------------------------------------
//功能: 查询文件状态
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 YAF2Stat(struct FileContext *FileCt, const char *Path, struct Stat *Buf, const char *Root)
{
	int Ret;
	int File;
	struct yaffs_stat FileInfo;
	char *YPath;

	if(FileCt)// 文件已经在缓冲中
	{
		Buf->Property = FileCt->Property;
		if(FileCt->Property & P_REG)
		{
			File = (int)FileCt->Private;
			Ret = yaffs_fstat(File, &FileInfo);
			if(-1 == Ret)
				return (-1);
			Buf->Size = (long long)FileInfo.st_size;
		}
		else
		{
			Buf->Property = FileCt->Property;
			Buf->Size = 0;
		}
		return (0);

	}

	// 文件不在缓冲中
	YPath = PathCopy(Root, Path, NULL);
	Ret = yaffs_stat(YPath, &FileInfo);
	if(-1 == Ret)
		return (-1);

	Buf->Size = (long long)FileInfo.st_size;

	Buf->Property = P_READ | P_WRITE;
	if(FileInfo.st_mode & S_IFDIR)
	{
		Buf->Property |= P_DIR;
		Buf->Size = 0; // yaffs2的目录大小不为零
	}
	else if(FileInfo.st_mode & S_IFREG)
		Buf->Property |= P_REG;
	else if(FileInfo.st_mode & S_IFLNK)
		Buf->Property |= P_LINK;

	return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注: todo: off_t类型待议
//-----------------------------------------------------------------------------
s32 YAF2Truncate(struct FileContext *FileCt, off_t NewSize)
{
	int File;

	File = (int)FileCt->Private;
	return(yaffs_ftruncate(File, (Y_LOFF_T)NewSize));// todo
}
//-----------------------------------------------------------------------------
//功能: 读目录内容
//参数:
//返回: 1 -- 结束,全部读完;
//      -1 -- 参数错误; -2 -- 函数不支持; -3 -- 其他错误;
//      0 -- 成功读取返回;
//备注: 每读一次,返回一个目录项
//-----------------------------------------------------------------------------
// 由YAFFS2控制的结构体
struct yaffsfs_DirSearchContext {
    struct yaffs_dirent de;
    YCHAR name[NAME_MAX + 1];
    struct yaffs_obj *dirObj;
    struct yaffs_obj *nextReturn;
    struct list_head others;
    s32 offset:20;
    u8 inUse:1;
};
s32 YAF2DirRead(struct FileContext *FileCt, struct Dirent *Content)
{
	 u32 FileType;
	 struct yaffs_dirent *De;
	 struct yaffsfs_DirSearchContext *Dsc;
	 yaffs_DIR *Dir = (yaffs_DIR*)FileCt->Private;

	 Dsc = (struct yaffsfs_DirSearchContext *)Dir;
	 if(Dsc->inUse && Dsc->nextReturn)
	 {
		 FileType = yaffs_get_obj_type((struct yaffs_obj *)Dsc->nextReturn);
		 switch(FileType)
		 {
		 	 case DT_REG: Content->Property = P_REG; break;
		 	 case DT_DIR: Content->Property = P_DIR; break;
		 	 case DT_LNK: Content->Property = P_LINK; break;
		 	 default : return (-3); // 其他类型不支持
		 }
	 }

	 De = yaffs_readdir(Dir);
	 if(NULL == De)
		 return (1);

	 strcpy(Content->Name, De->d_name);

	 return (0);
}
#else
#include <device.h>
#include <stddef.h>
#include <dirent.h>
#include <device/flash/flash.h>
#include "../../filesystems.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yaffsfs.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yportenv.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h"
#include "./drivers/yaffs_drv_generic.h"
#include "yaf2port.h"

static s32 __yaf2install(tagFSC *pSuper, u32 dwOpts, void *data);
static ptu32_t __yaf2opertaions(u32 dwCMD, ptu32_t oof, ptu32_t args,  ...);

//
// IAP的文件系统类型
//
tagFST typeYAF2 = {
        __yaf2opertaions,
        __yaf2install,
        NULL,
        NULL,
        "YAF2"
};
// ============================================================================
// 功能：打开YAF的文件或目录
// 参数：pObj -- YAF文件对象(可能不是需要打开的文件)；
//      dwFlags -- 文件标记；
//      pPath -- 文件路径；
// 返回：成功（YAF文件）；失败（NULL）;
// 备注：
// ============================================================================
tagOFile *__yaf2open(struct Object *pObj, u32 dwFlags, char *pPath)
{
    char *path;
    s32 res;
    ptu32_t context;
    tagOFile *new = NULL;
    char *root = (char*)FS_Core(pObj);

    path = malloc(strlen(root)+strlen(pPath)+1);
    if(!path)
        return (NULL);

    sprintf(path, "%s%s", root, pPath);
    do
    {
        if(testdirectory(dwFlags))
        {
            if(testcreat(dwFlags)) // 创建或者打开逻辑，则先尝试创建
            {
                res = yaffs_mkdir(path, (S_IREAD | S_IWRITE));
                if((res < 0) && (testonlycreat(dwFlags))) // 只是要创建
                    break; // 失败
            }

            context = (ptu32_t)yaffs_opendir(path);
            if(!context)
                break; // 失败
        }
        else
        {
            context = yaffs_open(path, dwFlags, (S_IREAD | S_IWRITE)); // TODO: 需要确认下目录的操作是不是和文件一致的
            if(-1 == context)
                break; // 失败
        }

        new = of_new();
        if(new)
            of_init(new, NULL, dwFlags, context);

    }while(0);


    free(path);
    return (new);
}

// ============================================================================
// 功能：关闭打开的YAF文件；
// 参数：pOF -- YAF文件；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __yaf2close(tagOFile *pOF)
{
    s32 res;
    ptu32_t context = of_context(pOF);

    if(IS_DIRECTORY(pOF))
    {
        res = yaffs_closedir((yaffs_DIR*)pOF);
    }
    else
    {
        res = yaffs_close(context);
    }

    if(!res)
    {
        of_free(pOF); // 成功；
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：写YAF文件；
// 参数：pOF -- YAF文件；
//      pBuf -- 需写入数据；
//      dwSize -- 需写入数据的数量；
// 返回：实际写入的数据的数量。
// 备注：
// ============================================================================
static s32 __yaf2write(tagOFile *pOF, u8 *pBuf, u32 dwSize)
{
    s32 res, context = (s32)of_context(pOF);

    res = yaffs_write(context, pBuf, dwSize);
    if(-1 == res)
        return (0); // 失败

    return (dwSize);
}

// ============================================================================
// 功能：读YAF文件；
// 参数：pOF -- YAF文件；
//      pBuf -- 需读出数据的缓存；
//      dwSize -- 需读出数据的数量；
// 返回：实际读出的数据的数量。
// 备注：
// ============================================================================
static s32 __yaf2read(tagOFile *pOF, u8 *pBuf, u32 dwSize)
{
    s32 res, context = (s32)of_context(pOF);

    res = yaffs_read(context, pBuf, dwSize);
    if(-1 == res)
        return (0);// 失败

    return (dwSize);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
// 由YAFFS2控制的结构体
struct yaffsfs_DirSearchContext {
    struct yaffs_dirent de;
    YCHAR name[NAME_MAX + 1];
    struct yaffs_obj *dirObj;
    struct yaffs_obj *nextReturn;
    struct list_head others;
    s32 offset:20;
    u8 inUse:1;
};
static s32 __yaf2readdentry(tagOFile *pOF, struct dirent *pDirent)
{
    u32 type;
    struct yaffs_dirent *yafDirent;
    struct yaffsfs_DirSearchContext *yafDSC;
    yaffs_DIR *yafDir = (yaffs_DIR*)of_context(pOF);

     yafDSC = (struct yaffsfs_DirSearchContext *)yafDir;
     if(yafDSC->inUse && yafDSC->nextReturn)
     {
         type = yaffs_get_obj_type((struct yaffs_obj *)yafDSC->nextReturn);
         switch(type)
         {
             case DT_REG: pDirent->d_type = DIRENT_IS_REG; break;
             case DT_DIR: pDirent->d_type = DIRENT_IS_DIR; break;
             case DT_LNK: pDirent->d_type = DIRENT_IS_LNK; break;
             default : return (-1); // 其他类型不支持
         }
     }

     yafDirent = yaffs_readdir(yafDir);
     if(!yafDirent)
         return (1);

     strcpy(pDirent->d_name, yafDirent->d_name);
     return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __yaf2remove(struct Object *pObj, char *pPath)
{
    s32 res;
    char *path, *root;
    extern int __yaffs_unlink(const YCHAR *path);

    root = (char*)FS_Core(pObj);
    path = malloc(strlen(root)+strlen(pPath)+1);
    if(!path)
        return (-1);

    res = __yaffs_unlink(path);
    free(path);
    return (res);
}

// ============================================================================
// 功能：YAF文件重定位当前位置；
// 参数：pOF -- YAF文件；
//      qwOffset -- 移动量；
//      dwWhence -- 移动的起点；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __yaf2seek(tagOFile *pOF, s64 qwOffset, s32 dwWhence)
{
    s32 context = (s32)of_context(pOF);

    if(-1 == yaffs_lseek(context, qwOffset, dwWhence))
        return (-1);

    return (0);
}
// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static ptu32_t __yaf2opertaions(u32 dwCMD, ptu32_t oof, ptu32_t args,  ...)
{
    va_list list;

    switch(dwCMD)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct Object *obj = (struct Object*)oof;
            u32 flags = (u32)args;
            u32 discard;
            char *full;

            va_start(list, args);
            discard = va_arg(list, u32);
            full = (char*)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__yaf2open(obj, flags, full));
        }

        case CN_OBJ_CMD_CLOSE:
        {
            tagOFile *of = (tagOFile*)oof;
            return ((ptu32_t)__yaf2close(of));
        }

        case CN_OBJ_CMD_READDIR:
        {
            tagOFile *of = (tagOFile*)oof;
            struct dirent *ret = (struct dirent *)args;

            return(__yaf2readdentry(of, ret));
        }

        case CN_OBJ_CMD_READ:
        {
            tagOFile *of;
            u8 *buf;
            u32 len;

            of = (tagOFile *)oof;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__yaf2read(of, buf, len));
        }

        case CN_OBJ_CMD_WRITE:
        {
            tagOFile *of;
            u8 *buf;
            u32 len;

            of = (tagOFile *)oof;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__yaf2write(of, buf, len));
        }

        case CN_OBJ_CMD_DELETE:
        {
            char *path;
            struct Object *obj = (struct Object*)oof;

            va_start(list, args);
            path = (char *)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__yaf2remove(obj, path));
        }

        case CN_OBJ_CMD_SEEK:
        {
            s32 whence;
            tagOFile *of = (tagOFile *)oof;
            s64 *offset = (s64*)args;


            va_start(list, args);
            whence = (s32)va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__yaf2seek(of, offset, whence));
        }

        default:
        {
        	debug_printf("todo   do not support this operation now.");
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：
// 参数：
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __yaf2install(tagFSC *pSuper, u32 dwOpts, void *data)
{
    struct yaffs_dev *yaf2Dev;
    struct yaffs_param *params;
    struct FlashChip *flash;
    s32 res = YAFFS_FAIL;
    u32 index, config = (u32)data;

    flash = (struct FlashChip*)dev_dtago(pSuper->pDev);

    if(dwOpts & INSTALL_FORMAT)
    {
        if(-1 == EarseWholeChip(flash))
            return (-1);
    }

    yaffs_start_up();
    yaf2Dev = malloc(sizeof(struct yaffs_dev)); // "yaffs_dev"类似于superblock作用
    if(NULL == yaf2Dev)
        return (-1); // 错误;

    memset(yaf2Dev, 0, sizeof(*yaf2Dev));
    params = &yaf2Dev->param;
    params->name = __DEV_Name(pSuper->pDev);
    params->n_reserved_blocks = 6;
    params->is_yaffs2 = 1;
    params->n_caches = 0; // 0 -- 不使用cache逻辑
    params->use_nand_ecc = 0; // 挂载过程未使用ECC，这个对一个新芯片很重要的逻辑
    params->disable_summary = 1; // 除去summary功能，todo
    if(F_NAND == flash->Type)
    {
        params->total_bytes_per_chunk = flash->Descr.Nand.BytesPerPage; // todo: 考虑nand页小于512的情况
        params->chunks_per_block = flash->Descr.Nand.PagesPerBlk;
        params->start_block = flash->Descr.Nand.ReservedBlks;
        params->end_block = (flash->Descr.Nand.BlksPerLUN * flash->Descr.Nand.LUNs) - 1; // 例如总块数2048,则范围"0-2047"
        res = yaffs_nand_install_drv(yaf2Dev, flash); // 初始化YAFFS2驱动层接口
    }
    else if(F_NOR == flash->Type)
    {
        // 文件头结构体最大是512Bytes另外还有Tags是16Bytes,
        if(flash->Descr.Nor.BytesPerPage > 512+16)
        {
            params->total_bytes_per_chunk = flash->Descr.Nor.BytesPerPage;
            params->chunks_per_block = flash->Descr.Nor.PagesPerSector * flash->Descr.Nor.PagesPerSector;
        }
        else
        {
            params->total_bytes_per_chunk = 1024;
            index = params->total_bytes_per_chunk / flash->Descr.Nor.BytesPerPage; // todo: 此处转变为位运算
            params->chunks_per_block = flash->Descr.Nor.PagesPerSector * flash->Descr.Nor.PagesPerSector / index;
        }

        params->start_block = flash->Descr.Nor.ReservedBlks;
        params->end_block = flash->Descr.Nor.Blks - 1;
        params->inband_tags = 1; // tag存放于内页
        res = yaffs_nor_install_drv(yaf2Dev, flash); // 初始化YAFFS2驱动层接口
    }

    if (YAFFS_FAIL == res) // 其他设备暂不支持或者上面逻辑执行失败
    {
        free(yaf2Dev);
        return (-1);
    }

    yaffs_add_device(yaf2Dev); // 将"yaffs_dev"注册进yaffs2

    res = yaffs_mount_reldev(yaf2Dev); // 挂载
    if(-1 == res)
    {
        yaffs_remove_device(yaf2Dev);
        free(yaf2Dev);
        return (-1);
    }

    // 如果驱动并不支持ECC，在驱动层会有一次判断
    if(config & YAF2_ENABLE_DEVICE_ECC)
        params->use_nand_ecc = 1;// 使用ECC，只能放到挂载完成后

    pSuper->pCore = (void*)__DEV_Name(pSuper->pDev);

    return (0);
}

// ============================================================================
// 功能：YAFS文件系统内部机制需要OS相关的一些逻辑;
// 参数：
// 返回：成功（0）；失败（-1）。
// 备注:
// ============================================================================
int yaffs_start_up(void)
{
    static int startupCalled = 0;

    if(startupCalled)
        return (0);

    startupCalled = 1;

    yaffsfs_OSInitialisation();

    return (0);
}

// ============================================================================
// 功能：挂载YAFFS2文件系统到目录/yaffs2下
// 参数：pTarget -- YAF2文件系统所挂载的目录；缺省为“/yaf2”
//      pSource -- YAF2文件系统所处于的设备；
//      dwOpt -- 文件系统配置选项；
//      dwData -- 传递给YAF2安装逻辑的数据；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_YAF2(const char *pTarget, const char *pSource, u32 dwOpt, void *data)
{
    s32 res, fd;
    char *target = "/yaf2";

    if(NULL == pSource)
    {
    	debug_printf("module   file system \"YAF2\" install failed<no device>.");
        return (-1); // 参数失败
    }

    if(!pTarget)
    {
        target = (char*)pTarget;
    }

    res = FS_Register(&typeYAF2);
    if(res)
    {
    	debug_printf("module   cannot register \"YAF2\"<file system type>.");
        return (-1); // 失败;
    }

    fd = open("/yaf2", O_DIRECTORY | O_CREAT | O_EXCL | O_RDWR, 0);
    if(-1 == fd)
    {
    	debug_printf("module   mount \"YAF2\" failed, cannot create \"%s\"<mount point>.", target);
        return (-1); // 失败;
    }

    res = FS_Mount(pSource, target, "YAF2", dwOpt, data);
   if(res < 0)
   {
	   debug_printf("module   mount \"FAT\" failed, cannot install.");
       close(fd);
       remove(target);
       return (-1);// 失败
   }

   close(fd);
   info_printf("module","\"FAT\" file system installed.");
   return (0);
}

// ============================================================================
// 功能：挂载YAFFS2文件系统到目录/yaffs2下
// 参数：pDevPath -- 文件系统所挂载的设备路径
//      dwOpt -- 文件系统配置选项：(0x1) -- 启用ECC;(0x2) -- 格式化文件系统
// 返回：成功（0）；失败（-1）;
// 备注：不再使用
// ============================================================================
s32 ModuleInstall_YAFFS2(const char *pDevPath, u32 dwOpt)
{
    u32 data;
    u32 opt;

    if(dwOpt & 0x1)
        data = YAF2_ENABLE_DEVICE_ECC;

    if(dwOpt & 0x2)
        opt = INSTALL_FORMAT;
    else
        opt = INSTALL_CREAT;

    return (ModuleInstall_YAF2("/yaffs2", pDevPath, opt, data));
}

#endif
