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
#include <string.h>
#include "fat_port.h"

extern struct FileOperations g_tFATFileOps; // ./mount.c文件中定义
extern char *PathCopy(const char *Root, const char *RelPath, char(*Rebuild)(char *Path));

//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 非NULL -- 成功; NULL -- 失败;
//备注: 
//-----------------------------------------------------------------------------
struct FileContext *FATOpen(const char *Path, u32 Mode, const char *Root)
{
    char *LocalPath;
    struct FileContext *FileCt;
    struct FileBuf *Buf;
    FRESULT Res = FR_OK;
    u8 LocalMode = 0;
    
    // 先排除不支持功能
    if((Mode & M_APPEND) || (Mode & M_LINK))
        return (NULL);// 当前不支持的模式,(Mode & M_TRUNCATE)还没有处理
    
    LocalPath = PathCopy(Root, Path, NULL);// todo:考虑放入虚拟层

    FileCt = AllocContext();
    if(!FileCt)
        return (NULL);

    FileCt->FileOps = (void*)&g_tFATFileOps;

    // todo: 读写属性都没有怎么处理？
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
            LocalMode |= FA_OPEN_ALWAYS;// 如果文件不存在,则创建一个再打开.如此便能保证fopen打开一次性打开一个已存在文件
        else
            LocalMode |= FA_CREATE_NEW;    
    }    
    
    if(!(Mode & M_TYPE))// 参数未指定，则两个都要尝试下
        Mode |= M_REG | M_DIR;

    if(Mode & M_REG)// 打开文件
    {
        FIL *NewFile = malloc(sizeof(FIL));
        if(NewFile)
        {    
            Res = f_open(NewFile, LocalPath, LocalMode);// 打开文件
            if((FR_OK == Res) || 
               ((FR_EXIST == Res) && (Mode & M_OPEN)))//打开已存在文件
            {
                if(0 == AllocFileBuf(&Buf, 0)) // 为文件创建缓冲区
                {
                    FileCt->Property |= P_REG;
                    FileCt->Private = (void *)(NewFile);
                    FileCt->Buf = Buf;
                    return (FileCt);// 成功
                }
                else
                {
                    f_close(NewFile); // 失败
                    free(NewFile);
                    FreeContext(FileCt);
                    if((Mode & M_CREATE) && (!(Mode & M_OPEN)))
                        f_unlink(LocalPath);
                    return (NULL);
                }
            }
            // 打开文件失败
            free(NewFile);
        }    
    }
    
    if(Mode & M_DIR)// 打开目录
    {
        _DIR *NewDir;
        
        if(LocalMode & FA_CREATE_NEW)// 需创建，先试图创建目录
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
                    return (FileCt);// 成功
                }
                // 打开目录失败
                free(NewDir);
            }
        }
    }
    
    // 失败
    FreeContext(FileCt);
    return (NULL);

}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 0 -- 成功; -2 -- 失败;
//备注: 
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
            return (-2);// 失败
        }
        case P_DIR:
        {
            _DIR *Dir = (_DIR*)(FileCt->Private);
            Res = f_closedir(Dir);
            if(FR_OK == Res)
                break;
            return (-2);// 失败
        }
        default:
            return (-2);// 失败
    }
    
    free(FileCt->Private);// 释放"FIL"或"_DIR"
    FreeContext(FileCt);
    return (0);
    
}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 0 -- 成功; -3 -- 失败;
//备注: 
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
//功能:
//参数: 
//返回: 0 -- 成功; -1 -- 操作失败; -2 -- 参数错误;
//备注: 
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
        default: return (-2);// 参数错误
    }
    
    Res = f_lseek(File, NewPos);
    if (FR_OK != Res)
        return (-1);
    return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数: 
//返回: 
//备注: 
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
//功能:
//参数: 
//返回: !0 -- 成功; 0 -- 失败;
//备注: 
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
//功能:
//参数: 
//返回: !0 -- 成功; 0 -- 失败;
//备注: 
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
//功能:
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 FATStat(struct FileContext *FileCt, const char *Path, struct Stat *Buf, const char *Root)
{
    FIL *File;
    char *LocalPath;
    FILINFO FileInfo = {0};// 要初始化，否则源程序会跑飞
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

        if(FileInfo.fattrib & AM_RDO) //只读
            Buf->Property = P_READ;
        else
            Buf->Property = P_READ | P_WRITE;

        if(FileInfo.fattrib & AM_DIR)// 目录
            Buf->Property |= P_DIR;

        if(FileInfo.fattrib & AM_ARC)// 文档
            Buf->Property |= P_REG;

        return (0);
    }

    return (-1);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注: todo: off_t类型待议
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
//功能: 读目录内容
//参数:
//返回: 1 -- 结束,全部读完;
//      -1 -- 参数错误; -2 -- 函数不支持; -3 -- 其他错误;
//      0 -- 成功读取返回;
//备注: 每读一次,返回一个目录项
//-----------------------------------------------------------------------------
s32 FATDirRead(struct FileContext *pFileCt, struct Dirent *pContent)
{
    FILINFO info;
    char lfname[256];
    FRESULT res;
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
            strcpy(pContent->Name, info.lfname); // 长文件名
        }
        else
        {
            strcpy(pContent->Name, info.fname);
            ret = -3; // 长文件名，但无法解析（当前字符集不支持）
        }
    }
    else
#endif
    {
        strcpy(pContent->Name, info.fname); // 8.3文件名
    }

    switch(info.fattrib & 0x30)
    {
        case AM_ARC: pContent->Property = P_REG; break;
        case AM_DIR: pContent->Property = P_DIR; break;
        default: return (-3); // 尚未支持的文件格式
    }
    return (ret);
}
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <djyos.h>
#include <dirent.h>
#include <objhandle.h>
#include "./drivers/fat_drivers.h"
#include "../ff11/src/ff.h"
#include "../../file.h"
#include <fs/filesystems.h>

static ptu32_t __fat_operations(enum objops ops, ptu32_t o_hdl, ptu32_t args,  ...);
static s32 __fat_install(tagFSC *super, u32 opt, void *pData);

tagFST typeFAT = {
        __fat_operations,
        __fat_install,
        NULL,
        NULL,
        "FAT"
};

// ============================================================================
// 功能：安装FAT文件系统
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __fat_install(tagFSC *super, u32 opt, void *data)
{
    const char *name;
    char *volume, *temp;
    s32 volumeNum;
    FATFS *structFAT;
    FRESULT res;
    u32 immediately = 1;

    data = data;
    name = devo2name(super->pDev);
    temp = volume = malloc(strlen(name)+2);
    sprintf(volume, "%s:", name);

    volumeNum = get_ldnumber((const char**)&temp); // 需要ffconf.h中定义"_VOLUME_STRS"
    if (volumeNum < 0)
    {
        printf("\r\n: debug : fatfs : mount failed, can not find the predefined volume (%s).", volume);
        free(volume);
        return (-1);
    }

    structFAT = malloc(sizeof(FATFS));
    if(!structFAT)
    {
        free(volume);
        return (-1);
    }

    if(FatDrvInitialize(LD2PD(volumeNum), (struct FatDrvFuns*)(devo2drv(super->pDev))))
    {
        free(volume);
        return (-1); // 安装驱动失败
    }

    if(opt & INSTALL_USE)
        immediately = 0;

    res = f_mount(structFAT, volume, immediately); // 挂载
    if ((FR_NO_FILESYSTEM == res) && (opt & INSTALL_CREAT))
        res = f_mkfs(volume, 1, 0); // 设备上不存在文件系统，则新建FAT

    if(FR_OK != res) // 失败
    {
        free(structFAT);
        free(volume);
        return (-1);
    }

    super->pCore = (void*)volume;
    return (0);
}

// ============================================================================
// 功能：将系统的flags逻辑转非FAT的mode逻辑
// 参数：
// 返回：
// 备注：
// ============================================================================
static u8 __deflags(u32 flags)
{
    u8 mode = 0;

    if(test_creat(flags))
    {
        if(test_onlycreat(flags))
            mode |= FA_CREATE_NEW;
        else
            mode |= FA_OPEN_ALWAYS;
    }
    else
    {
        mode |= FA_OPEN_ALWAYS;
    }

    if(test_readable(flags))
        mode |= FA_READ;

    if(test_writeable(flags))
        mode |= FA_WRITE;

    return (mode);
}

// ============================================================================
// 功能：打开FAT的文件或目录；
// 参数：ob -- FAT文件对象(可能不是需要打开的文件)；
//      flags -- 文件标记；
//      full -- 文件路径；
// 返回：成功（FAT文件）；失败（NULL）;
// 备注：
// ============================================================================
static struct objhandle *__fat_open(struct obj *ob, u32 flags, char *full)
{
    char *path;
    void *context;
    u8 mode;
    struct objhandle *hdl = NULL;
    FRESULT res = FR_OK;
    char *volume = (char*)corefs(ob);

    if(!volume)
        return (NULL);

    if(!full)
        full = "/"; // 根目录

    path = malloc(strlen(volume) + strlen(full) + 1);
    sprintf(path,"%s%s", volume, full);
    mode = __deflags(flags);
    do
    {
        if(test_directory(flags))// 目录操作逻辑
        {
            context = malloc(sizeof(_DIR));
            if(mode & FA_OPEN_ALWAYS) //  打开或新建，则先尝试打开
            {
                res = f_opendir(context, path);
                if(FR_OK == res)
                    break;
            }

            if((FR_OK != res) || (mode & FA_CREATE_NEW)) // 上面打开失败了或者新建
            {
                res = f_mkdir(path);
            }

            if(FR_OK == res)
            {
                res = f_opendir(context, path);
            }
        }
        else// 文件操作逻辑
        {
            context = malloc(sizeof(FIL));
            if(context)
            {
                res = f_open(context, path, mode);
                if(FR_EXIST == res)
                {
                    res = FR_OK; // 这个返回值也是操作成功的
                }
            }
        }
    }while(0);

    if(FR_OK != res)
    {
        free(context); // 打开失败；
    }
    else
    {
        hdl = handle_new();
        if(hdl)
            handle_init(hdl, NULL, flags, (ptu32_t)context);
    }

    free(path);
    return (hdl);
}

// ============================================================================
// 功能：关闭打开的FAT文件；
// 参数：hdl -- 内部句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __fat_close(struct objhandle *hdl)
{
    FRESULT res;
    void *context = (void*)handle_context(hdl);

    if(isdirectory(hdl))
    {
        res = f_closedir((_DIR*)context);
    }
    else
    {
        res = f_close((FIL*)context);
    }

    if(FR_OK != res)
        return (-1);

    free(context);
    handle_free(hdl);
    return (0);
}

// ============================================================================
// 功能：
// 参数：hdl -- 内部句柄；
// 返回：
// 备注：
// ============================================================================
static s32 __fat_seek(struct objhandle *hdl, s64 *offset, s32 whence)
{
    DWORD position;
    FRESULT res;
    FIL *context = (FIL*)handle_context(hdl);

    switch(whence)
    {
        case SEEK_SET: position = *offset;break;
        case SEEK_CUR: position = *offset + context->fptr;break;
        case SEEK_END: position = *offset + context->fsize; break;
        default: return (-1);
    }

    res = f_lseek(context, position);
    if(FR_OK != res)
        return (-1);

    *offset = position;
    return (0);
}

// ============================================================================
// 功能：
// 参数：ob -- 安装点对象；
// 返回：
// 备注：
// ============================================================================
static s32 __fat_remove(struct obj *ob, char *full)
{
    FRESULT res;
    char *path;
    char *volume = (char*)corefs(ob);

    path = malloc(strlen(volume) + strlen(full) + 1);
    if(!path)
        return (-1);

    sprintf(path,  "%s%s", (char*)volume, full);
    res = f_unlink(path);
    free(path);
    if(FR_OK != res)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：写FAT文件
// 参数：hdhdl -- 内部句柄；l -- FAT文件；
//      buf -- 需写入数据；
//      size -- 需写入数据的数量；
// 返回：实际写入的数据的数量；
// 备注：
// ============================================================================
static s32 __fat_write(struct objhandle *hdl, u8 *buf, u32 size)
{
    s32 writen = 0;
    FIL *context = (FIL*)handle_context(hdl);

    f_write(context, buf, size, (u32*)(&writen));
    return (writen);
}

// ============================================================================
// 功能：读FAT文件
// 参数：hdl -- 内部句柄；
//      buf -- 需读出数据的缓存；
//      size -- 需读出数据的数量；
// 返回：实际读出的数据的数量。
// 备注：
// ============================================================================
static s32 __fat_read(struct objhandle *hdl, u8 *buf, u32 size)
{
    s32 read = 0;
    FIL *context = (FIL*)handle_context(hdl);

    f_read(context, buf, size, (u32*)(&read));
    return (read);
}

// ============================================================================
// 功能：读FAT目录项
// 参数：hdl -- 内部句柄；
//      dentry -- 目录项；
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// 备注：
// ============================================================================
static s32 __fat_readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    FILINFO info;
    char lfname[256];
    FRESULT res;
    _DIR *context = (_DIR*)handle_context(hdl);

    memset(lfname, 0x0, 256);
    info.lfname = lfname;

    res = f_readdir(context, &info);
    if(FR_OK !=res)
        return (-1);

    if(0==info.fname[0]) // TODO，是不是应该通过返回值
        return (1); // 全部已经读完

    if(('~'==info.fname[6])||('~'==info.fname[3])) // 通过文件名中的'~'去区分，[3]可能是个BUG
    {
        strcpy(dentry->d_name, info.lfname); // 长文件名
    }
    else
    {
        strcpy(dentry->d_name, info.fname); // 8.3文件名
    }

    switch(info.fattrib & 0x30)
    {
        case AM_ARC: dentry->d_type = DIRENT_IS_REG; break;
        case AM_DIR: dentry->d_type = DIRENT_IS_DIR; break;
        default: dentry->d_type = DIRENT_IS_REG;
    }

    return (0);
}

// ============================================================================
// 功能：文件查询
// 参数：ob -- FAT文件对象；
//      data -- 文件信息；
//      path -- 未缓存文件路径；
//      cached -- 已缓存文件的内部句柄
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __fat_stat(struct obj *ob, struct stat *data, char *fullpath, struct objhandle *cached)
{
    FILINFO fatstat = {0};// 要初始化，否则源程序会跑飞；

    if(!cached) // 文件对象未缓存，依靠路径查询文件；
    {
        char *path, *root;

        root = (char*)corefs(ob);
        if(!root)
            return (-1);

        if(*fullpath=='\0')
        {
            // 根目录目前访问不了,直接处理
            data->st_size = 0;
            data->st_mode = S_IFDIR;
            return (0);
        }

        path = malloc(strlen(root)+strlen(fullpath)+1);
        if(!path)
            return (-1);

        sprintf(path, "%s%s", root, fullpath);
        if(FR_OK != f_stat(path, &fatstat))
        {
            free(path);
            return (-1);
        }

        memset(data, 0x0, sizeof(*data));
        data->st_size = fatstat.fsize;
        if(fatstat.fattrib & AM_DIR)// 目录
            data->st_mode = S_IFDIR;
        else if(fatstat.fattrib & AM_ARC)// 文档
            data->st_mode = S_IFREG;

        free(path);
    }
    else // 文件对象已缓存，依靠文件上下文查询；
    {
        if(isdirectory(cached))
        {
            data->st_size = 0;
            data->st_mode = S_IFDIR;
        }
        else
        {
            FIL *fatcx = (FIL*)handle_context(cached);
            data->st_size = fatcx->fsize;
            data->st_mode = S_IFREG;
        }
    }

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static ptu32_t __fat_operations(enum objops ops, ptu32_t o_hdl, ptu32_t args,  ...)
{
    va_list list;

    switch(ops)
    {
        case OBJOPEN:
        {
            struct obj *ob = (struct obj*)o_hdl;
            u32 flags = (u32)args;
            u32 discard;
            char *full;

            va_start(list, args);
            discard = va_arg(list, u32);
            discard = va_arg(list, u32);
            full = (char*)va_arg(list, u32);
            va_end(list);
            discard = discard; // 去报警

            return ((ptu32_t)__fat_open(ob, flags, full));
        }

        case OBJCLOSE:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            return ((ptu32_t)__fat_close(hdl));
        }

        case OBJCHILDS:
        {
            struct objhandle *hdl = (struct objhandle*)o_hdl;
            struct dirent *ret = (struct dirent *)args;

            return((ptu32_t)__fat_readdentry(hdl, ret));
        }

        case OBJREAD:
        {
            struct objhandle *hdl;
            u8 *buf;
            u32 len;

            hdl = (struct objhandle *)o_hdl;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__fat_read(hdl, buf, len));
        }

        case OBJWRITE:
        {
            struct objhandle *hdl;
            u8 *buf;
            u32 len;

            hdl = (struct objhandle *)o_hdl;
            buf = (u8*)args;
            va_start(list, args);
            len = va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__fat_write(hdl, buf, len));
        }

        case OBJDEL:
        {
            char *path;
            struct obj *ob = (struct obj*)o_hdl;

            va_start(list, args);
            path = (char *)va_arg(list, u32);
            va_end(list);

            return ((ptu32_t)__fat_remove(ob, path));
        }

        case OBJSEEK:
        {
            s32 whence;
            struct objhandle *hdl = (struct objhandle *)o_hdl;
            s64 *offset = (s64*)args;


            va_start(list, args);
            whence = (s32)va_arg(list, u32);
            va_end(list);

            return((ptu32_t)__fat_seek(hdl, offset, whence));
        }

        case OBJSTAT:
        {
            char *path;
            struct stat *data = (struct stat*)args;
            struct obj *ob = (struct obj*)o_hdl;
            struct objhandle *cached = NULL;

            va_start(list, args);
            path = (char *)va_arg(list, u32);
            if(path)
            {
                path = (char *)va_arg(list, u32); // 获取全路径；
            }
            else
            {
                cached = (struct objhandle*)va_arg(list, u32); // 获取已缓存文件的对象句柄；
            }

            va_end(list);
            return ((ptu32_t)__fat_stat(ob, data, path, cached));
        }

        default:
        {
            printf("\r\n: debug : fatfs  : do not support this operation now.");
            break;
        }
    }

    return (-1);
}

// ============================================================================
// 功能：安装FAT文件系统
// 参数：dev -- 文件系统所在设备；
//      opt -- 文件系统设置选项；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_FAT(const char *dir, const char *dev, u32 opt)
{
    s32 res, fd;
    char *mountpoint = "/fat";

    if(dir)
        mountpoint = (char*)dir;

    if(NULL==dev)
    {
        printf("\r\n: dbug : module : bad parameters, \"FAT\" file system installation is failed.");
        return (-1);
    }

    res = regfs(&typeFAT);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"FAT\"<file system type>.");
        return (-1); // 失败;
    }

    fd = open(mountpoint, O_DIRECTORY | O_CREAT | O_EXCL | O_RDWR, 0); // 创建/fat目录
    if(-1 == fd)
    {
        printf("\r\n: dbug : module : mount \"FAT\" failed, cannot create \"fat\"(mount point).");
        return (-1);// 失败;
    }


    res = mountfs(dev, mountpoint, "FAT", opt, NULL);
    if(res < 0)
    {
        printf("\r\n: dbug : module : mount \"FAT\" failed, cannot install.");
        close(fd);
        remove("/fat");
        return (-1);// 失败
    }

    close(fd);
    printf("\r\n: info : module : file system \"FAT\" installed on \"%s\".", dev);
    return (0);
}
