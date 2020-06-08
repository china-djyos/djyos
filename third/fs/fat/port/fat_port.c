//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
#include <djyfs/file.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#define SORTLEN    20      //媒体类型名的最大长度

s32 __fat_operations(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);
static s32 __fat_install(struct FsCore *super, u32 opt, void *pData);

// ============================================================================
// 功能：格式化fat文件系统
// 参数：
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __fatformat(void *core)
{
    debug_printf("fat","Formatting is not supported.");
    return -1;
}

// ============================================================================
// 功能：安装FAT文件系统
// 参数：super -- 文件系统管理信息；opt -- 文件系统的安装方式；data -- 文件系统的私有配置
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __fat_install(struct FsCore *super, u32 opt, void *data)
{
    const char *name;
    char *volume, *temp;
    s32 volumeNum;
    FATFS *structFAT;
    FRESULT res;
    u32 immediately = 1;

    name = data;
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

    if(FatDrvInitialize(LD2PD(volumeNum), (struct FatDrvFuns*)(super->MediaDrv)))
    {
        free(volume);
        return (-1); // 安装驱动失败
    }

    if(opt & MS_INSTALLUSE)
        immediately = 0;

    res = f_mount(structFAT, volume, immediately); // 挂载
    if ((FR_NO_FILESYSTEM == res) && (opt & MS_INSTALLCREAT))
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
// 功能：注销FAT文件系统
// 参数：super -- 文件系统管理信息；opt -- 文件系统的安装方式；data -- 文件系统的私有配置
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __fat_uninstall(void *data)
{
    const char *name;
    char *volume, *temp;
    FRESULT res;
    s32 volumeNum;

    name = data;
    temp = volume = malloc(strlen(name)+2);
    sprintf(volume, "%s:", name);

    volumeNum = get_ldnumber((const char**)&temp); // 需要ffconf.h中定义"_VOLUME_STRS"
    if (volumeNum < 0)
    {
        printf("\r\n: debug : fatfs : mount failed, can not find the predefined volume (%s).", volume);
        free(volume);
        return (-1);
    }
    res = f_mount(NULL, volume, 0); // 注销
    if(res != FR_OK)
    {
        free(volume);
        return (-1);
    }
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

    if(Handle_FlagIsCreate(flags) == 1)
    {
        if(Handle_FlagIsOnlyCreate(flags))
            mode |= FA_CREATE_NEW;
        else if(Handle_FlagIsTrunc(flags))
            mode |= FA_CREATE_ALWAYS;
        else
            mode |= FA_OPEN_ALWAYS ;

    }
    else
        mode |= FA_OPEN_EXISTING;

    if(Handle_FlagIsReadable(flags))
        mode |= FA_READ;

    if(Handle_FlagIsWriteable(flags))
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
static struct objhandle *__fat_open(struct Object *ob, u32 flags, char *full)
{
    char *path, *part_path, *mount_name = NULL;
    void *context;
    u8 mode;
    mode_t obj_mode, property = 0;
    struct objhandle *hdl = NULL;
    struct Object *temp =  ob;
    FRESULT res = FR_OK;
    char *volume = (char*)File_Core(ob);
    char entirepath[DJYFS_PATH_BUFFER_SIZE];

    while(temp != OBJ_GetRoot())
    {
        if(File_ObjIsMount(temp))
        {
            mount_name = (char*)OBJ_GetName(temp);    //找出mount点的名字
            break;
        }
        temp = OBJ_GetParent(temp);
    }
    if((!volume) && (!mount_name))
        return (NULL);

//    if(!full)
//        full = "/"; // 根目录
    memset(entirepath, 0, DJYFS_PATH_BUFFER_SIZE);
    File_GetEntirePath(ob, full, entirepath, DJYFS_PATH_BUFFER_SIZE); //获取文件的完整路径
    res = strlen(entirepath) + strlen(volume) + 1;
    path = malloc(res);
    if(!path)
        return (NULL);
    memset(path, 0, res);
    part_path = strstr(entirepath, mount_name);     //找出mount点名字的所在位置
    part_path += strlen(mount_name);
    //用volume中的设备名替换entirepath中的mount点名，因为fat只识别几种特定的设备
    sprintf(path,"%s%s", volume, part_path);
    res = FR_OK;
    mode = __deflags(flags);
    do
    {
        if(Handle_FlagIsDirectory(flags))// 目录操作逻辑
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
            property = S_IFDIR;
        }
        else// 文件操作逻辑
        {
            context = malloc(sizeof(FIL));
            if(context)
            {
                res = f_open(context, path, mode);
                if(FR_EXIST == res)
                    res = FR_OK; // 这个返回值也是操作成功的
                if(res == FR_OK)
                {
                    if(Handle_FlagIsAppend(flags))
                        res = f_lseek(context, ((FIL *)context)->fsize);
                }
            }
            property = S_IFREG;
        }
    }while(0);

    if(FR_OK != res)
    {
        free(context); // 打开失败；
    }
    else
    {
        hdl = Handle_New();
        if(hdl)
        {
            Handle_Init(hdl, NULL, flags, (ptu32_t)context);
            //TODO：从yaffs2中读取权限等，暂时赋予全部权限。
            obj_mode = S_IALLUGO | S_IFDIR;     //建立的路径，属性是目录。
            //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
            ob = OBJ_BuildTempPath(ob, __fat_operations, obj_mode,full);
            obj_mode = S_IALLUGO | property;     //最末端的也许是文件
            OBJ_LinkHandle(hdl, ob);
        }
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
    void *context = (void*)Handle_GetContext(hdl);

    if(File_IsDirectory(hdl))
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
//  Handle_Delete(hdl);
    return (0);
}

// ============================================================================
// 功能：同步FAT文件；
// 参数：hdl -- 内部句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __fat_sync(struct objhandle *hdl)
{
    FRESULT res;
    void *context = (void*)Handle_GetContext(hdl);

    if(Handle_FlagIsDirectory(hdl->flags))
    {
        return (-1);
    }
    else
    {
        res = f_sync((FIL*)context);
    }

    if(FR_OK != res)
        return (-1);

    return (0);
}

// ============================================================================
// 功能：
// 参数：hdl -- 内部句柄；
// 返回：
// 备注：
// ============================================================================
static off_t __fat_seek(struct objhandle *hdl, off_t *offset, s32 whence)
{
    DWORD position = -1;
    FRESULT res;
    FIL *context = (FIL*)Handle_GetContext(hdl);

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

    return ((off_t)position);
}

// ============================================================================
// 功能：
// 参数：ob -- 安装点对象；
// 返回：
// 备注：
// ============================================================================
static s32 __fat_remove(struct Object *ob, char *full)
{
    FRESULT res;
    char *path, *part_path, *mount_name = NULL;
    char *volume = (char*)File_Core(ob);
    struct Object *temp =  ob;
    char entirepath[DJYFS_PATH_BUFFER_SIZE];
    while(temp != OBJ_GetRoot())
    {
        if(File_ObjIsMount(temp))
        {
            mount_name = (char*)OBJ_GetName(temp);    //找出mount点的名字
            break;
        }
        temp = OBJ_GetParent(temp);
    }
    if((!volume) && (!mount_name))
        return (-1);

    memset(entirepath, 0, DJYFS_PATH_BUFFER_SIZE);
    File_GetEntirePath(ob,full,entirepath,DJYFS_PATH_BUFFER_SIZE); //获取文件的完整路径
    res = strlen(entirepath) + strlen(volume) + 1;
    path = malloc(res);
    if(!path)
        return (-1);

    memset(path, 0, res);
    part_path = strstr(entirepath, mount_name);     //找出mount点名字的所在位置
    part_path += strlen(mount_name);
    //用volume中的设备名替换entirepath中的mount点名，因为fat只识别几种特定的设备
    sprintf(path,"%s%s", volume, part_path);

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
    FIL *context = (FIL*)Handle_GetContext(hdl);

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
    FIL *context = (FIL*)Handle_GetContext(hdl);

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
    _DIR *context = (_DIR*)Handle_GetContext(hdl);

    memset(lfname, 0x0, 256);
    info.lfname = lfname;
    info.lfsize = 256;
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
static s32 __fat_stat(struct Object *ob, struct stat *data, char *uncached)
{
    FILINFO fatstat = {0};// 要初始化，否则源程序会跑飞；
    struct objhandle *myhandle;
    char *mount_name = NULL, *part_path;
    struct Object *temp =  ob;

    while(temp != OBJ_GetRoot())
    {
        if(File_ObjIsMount(temp))
        {
            mount_name = (char*)OBJ_GetName(temp);    //找出mount点的名字
            break;
        }
        temp = OBJ_GetParent(temp);
    }

    myhandle = OBJ_ForeachHandle(NULL, ob);     //取该文件其中一个句柄
    if((uncached)                   //文件对象未缓存，依靠路径查询文件；
        ||(myhandle == NULL))       //ob（是目录）虽在路径中，但未打开，也依靠路径查询
    {
        char *path, *root;
        char entirepath[DJYFS_PATH_BUFFER_SIZE];

        root = (char*)File_Core(ob);
        if(!root)
            return (-1);
        memset(entirepath, 0, DJYFS_PATH_BUFFER_SIZE);
        File_GetEntirePath(ob,uncached,entirepath,DJYFS_PATH_BUFFER_SIZE);
        part_path = strstr(entirepath, mount_name);     //找出mount点名字的所在位置
        part_path += strlen(mount_name);
        path = malloc(strlen(root)+strlen(entirepath)+1);
        if(!path)
            return (-1);

        sprintf(path, "%s%s", root, part_path);

        if(strcmp(path, root) == 0)
        {
            // 根目录目前访问不了,直接处理
            data->st_size = 0;
            data->st_mode = S_IFDIR;
            free(path);
            return (0);
        }
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
            data->st_mode = S_IFREG|S_IRWXUGO;

        free(path);
    }
    else // 文件对象已缓存，依靠文件上下文查询；
    {
        if(File_IsDirectory(myhandle))
        {
            data->st_size = 0;
            data->st_mode = S_IFDIR;
        }
        else
        {
            FIL *fatcx = (FIL*)Handle_GetContext(myhandle);
            data->st_size = fatcx->fsize;
            data->st_mode = S_IFREG|S_IRWXUGO;
        }
    }

    return (0);
}

// ============================================================================
// 功能：更改文件名称或位置
// 参数：oldpath -- 原路径；
//      newpath -- 新路径；
// 返回：成功 -- 0，失败 -- -1。
// 备注：
// ============================================================================
static s32 __fat_rename(const char *oldpath, const char *newpath)
{
    s32 res;
    struct Object *ob;
    char *OPath = NULL, *NPath = NULL, *uncached, *volume = NULL, *mount_name = NULL;

    ob = OBJ_MatchPath(oldpath, &uncached);     //找出原路径的OBJ
    volume = (char*)File_Core(ob);

    while(ob != OBJ_GetRoot())
    {
        if(File_ObjIsMount(ob))
        {
            mount_name = (char*)OBJ_GetName(ob);       //找出mount的名字
            break;
        }
        ob = OBJ_GetParent(ob);
    }
    if((!volume) && (!mount_name))
        return (-1);
    oldpath = strstr(oldpath, mount_name);      ////找出mount点名字的所在位置
    newpath = strstr(newpath, mount_name);
    OPath = malloc(DJYFS_PATH_BUFFER_SIZE);
    NPath = malloc(DJYFS_PATH_BUFFER_SIZE);
    if((!OPath) && (!NPath))
        return (-1);
    memset(OPath, 0, DJYFS_PATH_BUFFER_SIZE);
    memset(NPath, 0, DJYFS_PATH_BUFFER_SIZE);
    //用volume中的设备名替换entirepath中的mount点名，因为fat只识别几种特定的设备
    sprintf(OPath,"%s%s", volume, oldpath + strlen(mount_name));
    sprintf(NPath,"%s%s", volume, newpath + strlen(mount_name));
    res = f_rename(OPath, NPath);

    free(OPath);
    free(NPath);
    return res;
}


// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 __fat_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
//    va_list list;
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = __fat_open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)__fat_readdentry(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __fat_read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __fat_write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(__fat_close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = __fat_seek((struct objhandle *)opsTarget,
                                        (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(__fat_remove((struct Object*)opsTarget, (char *)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            if(__fat_stat((struct Object*)opsTarget, (struct stat *)OpsArgs1,
                            (char*)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if((ptu32_t)__fat_sync((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_RENAME:
        {
            *(s32*)OpsArgs1 = __fat_rename((const char *)OpsArgs2,(const char *)OpsArgs3);

            break;
        }

        default:
        {
            return CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// 功能：安装FAT文件系统
// 参数：  dir -- fat文件系统所挂载的目录的名字；缺省为“fat”
//      opt -- 文件系统设置选项；如MS_INSTALLCREAT
//      data -- 媒体所属类别（"RAM","NAND","CF","SD", "MSC", "EMMC"）
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_FAT(const char *dir_name, u32 opt, void *data)
{
    s32 res;
    char *mountpoint = "fat";
    struct Object * mountobj;
    static struct filesystem *typeFAT = NULL;
    if(dir_name)
        mountpoint = (char*)dir_name;

    if(typeFAT == NULL)
    {
        typeFAT = malloc(sizeof(*typeFAT));

        typeFAT->fileOps = __fat_operations;
        typeFAT->install = __fat_install;
        typeFAT->pType = "FAT";
        typeFAT->format = __fatformat;
        typeFAT->uninstall = __fat_uninstall;
    }
    res = File_RegisterFs(typeFAT);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"FAT\"<file system type>.");
        return (-1); // 失败;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, mountpoint);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"FAT\" failed, cannot create \"%s\"(target).", mountpoint);
        return (-1);
    }

//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;          //直接挂载不用备份
    res = File_Mount(NULL, mountpoint, "FAT", opt, data);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"FAT\" failed, cannot install.");
        OBJ_Delete(mountobj);
        return (-1);// 失败
    }
    return (0);
}

// ============================================================================
// 功能：注销FAT文件系统
// 参数：dir_path -- 文件系统的安装路径
// 备注：
// ============================================================================
s32 UnfileSystem_FAT(const char *dir_path)
{
    struct Object *targetobj;
    char *mountpoint = "/fat";
    char *notfind;
    if(dir_path)
        mountpoint = (char*)dir_path;

    targetobj = OBJ_MatchPath(mountpoint, &notfind);
    if(notfind)
    {
        error_printf("fat"," not found \"%s\".\r\n",mountpoint);
        return (-1); // 安装点必须准备好。
    }
    OBJ_DutyDown(targetobj);
    if(OBJ_IsOnDuty(targetobj))
    {
        OBJ_DutyUp(targetobj);
        error_printf("fat"," \"%s\" device not logged off.\r\n",mountpoint);
        return (-1);
    }
    else
    {
        File_UnMount(mountpoint, "FAT");
        if(OBJ_Delete(targetobj) != 0)
        {
            OBJ_DutyUp(targetobj);
            error_printf("fat"," fat obj delete fail.\r\n");
            return (-1); // 删除节点失败。
        }
    }

    return (0);
}
