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
#include <device/unit_media.h>
#include <djyfs/filesystems.h>
#include <math.h>
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yaffsfs.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/direct/yportenv.h"
#include "../yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h"
//#include "./drivers/yaffs_drv_generic.h"
#include "yaf2port.h"

static s32 __yaf2install(struct FsCore *pSuper, u32 dwOpts, void *data);
static s32 YAF2_Ops(void *opsTarget, u32 cmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3);

// ============================================================================
// 功能：打开YAF的文件或目录
// 参数：ob -- YAF文件对象(可能不是需要打开的文件)；
//      flags -- 文件标记；
//      pPath -- 文件路径；
// 返回：成功（YAF文件）；失败（NULL）;
// 备注：
// ============================================================================
struct objhandle *__yaf2open(struct Object *ob, u32 flags, char *uncached)
{
    char *path;
    s32 res;
    s32 yafcx;
    mode_t mode, property = 0;
    struct objhandle *hdl = NULL;
    char entirepath[DJYFS_PATH_BUFFER_SIZE];

    memset(entirepath, 0, DJYFS_PATH_BUFFER_SIZE);
    File_GetEntirePath(ob,uncached,entirepath,DJYFS_PATH_BUFFER_SIZE);

    res = strlen(entirepath) + 1;
    path = malloc(res);
    if(!path)
        return (NULL);
    memset(path, 0, res);
    memcpy(path, entirepath, res);
    do
    {
        if(Handle_FlagIsDirectory(flags))
        {
            if(Handle_FlagIsCreate(flags)) // 创建或者打开逻辑，则先尝试创建
            {
                res = yaffs_mkdir(path, (S_IREAD | S_IWRITE));
                if((res<0)&&(Handle_FlagIsOnlyCreate(flags))) // 只是要创建
                    break; // 失败
            }

            yafcx = (s32)yaffs_opendir(path);
            if(!yafcx)
                break; // 失败
            property = S_IFDIR;
        }
        else
        {
            yafcx = yaffs_open(path, flags, (S_IREAD | S_IWRITE)); // TODO: 需要确认下目录的操作是不是和文件一致的
            if(-1 == yafcx)
                break; // 失败
            property = S_IFREG;
        }

        hdl = Handle_New();
        if(hdl)
            Handle_Init(hdl, NULL, flags, (ptu32_t)yafcx);

    }while(0);


    free(path);
    if(hdl)
    {
        //TODO：从yaffs2中读取权限等，暂时赋予全部权限。
        mode = S_IALLUGO | S_IFDIR;     //建立的路径，属性是目录。
        //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
        ob = OBJ_BuildTempPath(ob, YAF2_Ops, mode,uncached);
        mode = S_IALLUGO | property;     //最末端的也许是文件
        if(!File_ObjIsMount(ob))
        {
            OBJ_SetPrivate(ob, mode);
        }
        OBJ_LinkHandle(hdl, ob);
        return (hdl);
    }
    else
        return NULL;
}

// ============================================================================
// 功能：关闭打开的YAF文件；
// 参数：hdl -- YAF文件的对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __yaf2close(struct objhandle *hdl)
{
    s32 res;
    ptu32_t yafcx = Handle_GetContext(hdl);

    if(Handle_FlagIsDirectory(hdl->flags))
    {
        res = yaffs_closedir((yaffs_DIR*)yafcx);
    }
    else
    {
        res = yaffs_close(yafcx);
    }

    if(!res)
    {
//      Handle_Delete(hdl); // 成功；
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：写YAF文件；
// 参数：hdl -- YAF文件的对象句柄；
//      data -- 需写入数据；
//      size -- 需写入数据的数量；
// 返回：实际写入的数据的数量。
// 备注：
// ============================================================================
static s32 __yaf2write(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 res, yafcx = (s32)Handle_GetContext(hdl);

    res = yaffs_write(yafcx, data, size);
    if(-1 == res)
        return (0); // 失败

    return (size);
}

// ============================================================================
// 功能：读YAF文件；
// 参数：hdl -- YAF文件的对象句柄；
//      data -- 需读出数据的缓存；
//      size -- 需读出数据的数量；
// 返回：实际读出的数据的数量。
// 备注：
// ============================================================================
static s32 __yaf2read(struct objhandle *hdl, u8 *data, u32 size)
{
    s32 res, yafcx = (s32)Handle_GetContext(hdl);

    res = yaffs_read(yafcx, data, size); // 返回实际读到的数据
    if(-1 == res)
        return (0);// 失败

    return (res);
}

// ============================================================================
// 功能：同步YAF文件；
// 参数：hdl -- YAF文件的对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __yaf2fsync(struct objhandle *hdl)
{
    s32 res;
    ptu32_t yafcx = Handle_GetContext(hdl);

    if(Handle_FlagIsDirectory(hdl->flags))
    {
        return (-1);
    }
    else
    {
        res = yaffs_fsync(yafcx);
    }

    if(!res)
    {
        return (0);
    }

    return (-1);
}

// ============================================================================
// 功能：更改文件名称或位置
// 参数：oldpath -- 原路径；
//      newpath -- 新路径；
// 返回：成功 -- 0，失败 -- -1。
// 备注：
// ============================================================================
static s32 __yaf2rename(const char *oldpath, const char *newpath)
{
    s32 res;

    res = yaffs_rename(oldpath, newpath);
    return (res);
}

// ============================================================================
// 功能：读目录项；
// 参数：hdl -- YAF文件的对象句柄；
//      dentry -- 目录项；
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
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
static s32 __yaf2readdentry(struct objhandle *hdl, struct dirent *dentry)
{
    u32 type;
    struct yaffs_dirent *yafDirent;
    struct yaffsfs_DirSearchContext *yafDSC;
    yaffs_DIR *yafDir = (yaffs_DIR*)Handle_GetContext(hdl);

     yafDSC = (struct yaffsfs_DirSearchContext *)yafDir;
     if(yafDSC->inUse && yafDSC->nextReturn)
     {
         type = yaffs_get_obj_type((struct yaffs_obj *)yafDSC->nextReturn);
         switch(type)
         {
             case DT_REG: dentry->d_type = DIRENT_IS_REG; break;
             case DT_DIR: dentry->d_type = DIRENT_IS_DIR; break;
             case DT_LNK: dentry->d_type = DIRENT_IS_LNK; break;
             default : return (-1); // 其他类型不支持
         }
     }

     yafDirent = yaffs_readdir(yafDir);
     if(!yafDirent)
         return (1);

     strcpy(dentry->d_name, yafDirent->d_name);
     return (0);
}

// ============================================================================
// 功能：删除文件；
// 参数：ob -- YAF安装点对象；
//      path -- 目标文件的全路径；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 __yaf2remove(struct Object *ob, char *full)
{
    s32 res;
    char *path;
    extern int __yaffs_unlink(const YCHAR *path);
    char entirepath[DJYFS_PATH_BUFFER_SIZE];

    memset(entirepath, 0, DJYFS_PATH_BUFFER_SIZE);
    File_GetEntirePath(ob,full,entirepath,DJYFS_PATH_BUFFER_SIZE);

    res = strlen(entirepath) + 1;
    path = malloc(res);
    if(!path)
        return (-1);
    memset(path, 0, res);
    memcpy(path, entirepath, res);
    res = __yaffs_unlink(path);
    free(path);
    return (res);
}

// ============================================================================
// 功能：YAF文件重定位当前位置；
// 参数：hdl -- YAF文件对象句柄；
//      qwOffset -- 移动量；
//      dwWhence -- 移动的起点；
// 返回：成功（当前位置）；失败（-1）；
// 备注：
// ============================================================================
static off_t __yaf2seek(struct objhandle *hdl, off_t *offset, s32 whence)
{
    s32 yafcx = (s32)Handle_GetContext(hdl);

    return (yaffs_lseek(yafcx, *offset, whence));
}

// ============================================================================
// 功能：查询文件信息；
// 参数：ob -- YAF文件对象；
//      data -- 文件信息；
//      uncached -- 为缓存文件路径；
//      cached -- 已缓存文件的对象句柄；
// 返回：成功（0）；失败（-1）；
// 备注：当查询安装点时，uncached为NULL；
// ============================================================================
static s32 __yaf2stat(struct Object *ob, struct stat *data, char *uncached)
{
    struct yaffs_stat yafstat = {0};
    struct objhandle *myhandle;
    s32 res;

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

        res = strlen(entirepath) + 1;

        path = malloc(res);
        if(!path)
            return -1;
        memset(path, 0, res);
        memcpy(path, entirepath, res);
        res = strlen(path);
        while(res--)
        {
            if((path[res] == '/') || (path[res] == '\\'))    //去掉路径最后多余的"/"或"\\"
                path[res] = '\0';
            else
                break;
        }
        if(-1==yaffs_stat(path, &yafstat))
        {
            free(path);
            return (-1);
        }

        memset(data, 0x0, sizeof(*data));
        data->st_size = yafstat.st_size;
        data->st_mode = yafstat.st_mode;
        free(path);
    }
    else    // 对象已打开，依靠文件上下文查询
    {
        s32 yafcx = (s32)Handle_GetContext(myhandle);

        if(-1==yaffs_fstat(yafcx, &yafstat)) // TODO:目录是否可以？
            return (-1);

        data->st_size = yafstat.st_size;
        data->st_mode = yafstat.st_mode;
    }

    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 YAF2_Ops(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                        ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = __yaf2open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)__yaf2readdentry(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __yaf2read(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            ssize_t len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = __yaf2write(devfile, (u8*)OpsArgs2, (ssize_t)OpsArgs3);
            *(ssize_t *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(__yaf2close((struct objhandle*)opsTarget)== 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = __yaf2seek((struct objhandle *)opsTarget,
                                        (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(__yaf2remove((struct Object*)opsTarget, (char *)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            if(__yaf2stat((struct Object*)opsTarget, (struct stat *)OpsArgs1,
                                (char*)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if((ptu32_t)__yaf2fsync((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_RENAME:
        {
            *(s32*)OpsArgs1 = __yaf2rename((const char *)OpsArgs2,(const char *)OpsArgs3);

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
// 功能：初始化efs对media的驱动
// 参数：yaf2dev -- yaf的驱动结构；；  drv -- media的操作函数集；  splices -- 拼了多少页
// 返回：YAFFS_OK -- 成功；  YAFFS_FAIL --失败
// 备注：
// ============================================================================
static int yaf2_install_drv(struct yaffs_dev *yaf2dev, struct yaffs_driver *drv, u32 splices)
{
    if (!yaf2dev || !drv)
         return (YAFFS_FAIL);

    yaf2dev->drv.drv_write_chunk_fn = drv->drv_write_chunk_fn;
    yaf2dev->drv.drv_read_chunk_fn = drv->drv_read_chunk_fn;
    yaf2dev->drv.drv_erase_fn = drv->drv_erase_fn;
    yaf2dev->drv.drv_mark_bad_fn = drv->drv_mark_bad_fn;
    yaf2dev->drv.drv_check_bad_fn = drv->drv_check_bad_fn;
    yaf2dev->drv.drv_initialise_fn = drv->drv_initialise_fn;
    yaf2dev->drv.drv_deinitialise_fn = drv->drv_deinitialise_fn;

    if(!splices)
        yaf2dev->driver_context = 1;
    else
        yaf2dev->driver_context = splices;

    return (YAFFS_OK);
}

// ============================================================================
// 功能：格式化yaf2文件系统
// 参数：core -- 文件 系统核心数据
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __yaf2format(void *core)
{
    if(core == NULL)
        return -1;
    return yaffs_format_reldev((struct yaffs_dev *)core,1,1,1);
}
// ============================================================================
// 功能：安装yaf2文件系统
// 参数：pSuper -- 文件系统管理信息；dwOpts -- 文件系统的安装方式；data -- 文件系统的私有配置
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 __yaf2install(struct FsCore *pSuper, u32 dwOpts, void *data)
{
    struct yaffs_dev *yaf2Dev = NULL;
    struct yaffs_param *params;
    struct umedia *media;
    u32 pieces,splice = 0,config = *(u32*)data,flash_block_size,flash_page_size,block_page_number;
    s32 res = YAFFS_FAIL;

    media = (struct umedia*)pSuper->MediaInfo;              //获取flash的信息
    media->mreq(blockunits,(ptu32_t)&block_page_number);             //获取flash中一块有多少页
    media->mreq(unitbytes,(ptu32_t)&flash_page_size);                //获取flash的页大小
    flash_block_size = block_page_number * flash_page_size;  //获取flash的块大小
    if((flash_block_size == 0) || (block_page_number == 0) || (flash_page_size == 0))
        return (-1);

    if(dwOpts & MS_INSTALLFORMAT)                           //根据安装方式来决定是否要格式化安装文件系统的区域
    {
        struct uesz sz = {0};
        sz.block = 1;

        if(media->mreq(format, (pSuper->MediaStart/block_page_number), -1, &sz))
            return (-1);
    }

    yaffs_start_up();
    yaf2Dev = malloc(sizeof(struct yaffs_dev)); // "yaffs_dev"类似于superblock作用
    if(NULL == yaf2Dev)
        return (-1); // 错误;

    memset(yaf2Dev, 0, sizeof(*yaf2Dev));
    params = &yaf2Dev->param;
    params->name = Device_GetName(pSuper->pTarget);
    params->n_reserved_blocks = 6;
    params->is_yaffs2 = 1;
    params->n_caches = 0; // 0 -- 不使用cache逻辑
    params->use_nand_ecc = 0; // 挂载过程未使用ECC，这个对一个新芯片很重要的逻辑
    params->disable_summary = 1; // 除去summary功能，这个功能对于随时断电的设备作用不大；

    // yaf文件系统的文件头结构体最大是512Bytes另外还有Tags是16Bytes
    params->total_bytes_per_chunk = flash_page_size;
    if(params->total_bytes_per_chunk<512) // unit过小,需要拼接
    {
        splice = 1024 / flash_page_size;
        params->total_bytes_per_chunk = 1024;
    }
    if(splice)
        params->chunks_per_block = block_page_number / splice;
    else
        params->chunks_per_block = block_page_number;

    params->start_block = pSuper->MediaStart / block_page_number;
    pieces = (u32)(pSuper->MediaStart % params->chunks_per_block); // 对齐操作
    if(pieces)
        params->start_block += 1;

    params->end_block = (pSuper->AreaSize - (pieces * flash_page_size)) / flash_block_size;
    params->end_block += (params->start_block - 1); // 减一是块的需要从零开始；
    if((nor==media->type)||(splice))
        params->inband_tags = 1; // nand页过小或非nand设备，tag存放于内页;

    res = yaf2_install_drv(yaf2Dev, (struct yaffs_driver *)pSuper->MediaDrv, splice); // 初始化YAFFS2驱动层接口
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

    pSuper->pCore = (void*)yaf2Dev;
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
// 参数：target -- YAF2文件系统所挂载的目录；缺省为“yaf2”
//      opt -- 文件系统配置选项；  true ：格式化整个文件系统；false ：不格式化整个文件系统
//      data -- 传递给YAF2安装逻辑的数据；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 ModuleInstall_YAF2(const char *target, u32 opt, u32 data)
{
    static struct filesystem *typeYAF2 = NULL;
    struct Object * mountobj;
    s32 res;

    if(!target)
    {
        printf("\r\n: dbug : module : cannot input \"YAF2\" file system mount name.");
        return (-1);
    }

    if(typeYAF2 == NULL)
    {
        typeYAF2 = malloc(sizeof(*typeYAF2));

        typeYAF2->fileOps = YAF2_Ops;
        typeYAF2->install = __yaf2install;
        typeYAF2->pType = "YAF2";
        typeYAF2->format = __yaf2format;
        typeYAF2->uninstall = NULL;
    }
    res = File_RegisterFs(typeYAF2);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"YAF2\"<file system type>.");
        return (-1); // 失败;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, target);
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"YAF2\" failed, cannot create \"%s\"<mount point>.", target);
        return (-1); // 失败;
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;              //直接挂载不用备份
    res = File_Mount(NULL, target, "YAF2", opt, (void *)data);
   if(res == -1)
   {
       printf("\r\n: dbug : module : mount \"YAF2\" failed, cannot install.");
       OBJ_Delete(mountobj);
       return (-1);// 失败
   }
   return (0);
}

// ============================================================================
// 功能：挂载YAFFS2文件系统到目录/yaffs2下
// 参数：pDevPath -- 文件系统所挂载的设备路径
//      dwOpt -- 文件系统配置选项：(0x1) -- 启用ECC;(0x2) -- 格式化文件系统
// 返回：成功（0）；失败（-1）;
// 备注：不再使用
// ============================================================================
//s32 ModuleInstall_YAFFS2(const char *pDevPath, u32 dwOpt)
//{
//    u32 data;
//    u32 opt;
//
//    if(dwOpt & 0x1)
//        data = YAF2_ENABLE_DEVICE_ECC;
//
//    if(dwOpt & 0x2)
//        opt = MS_INSTALLFORMAT;
//    else
//        opt = MS_INSTALLCREAT;
//
//    return (ModuleInstall_YAF2("/yaffs2", pDevPath, opt, &data));
//}

#endif
