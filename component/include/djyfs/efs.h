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
//所属模块:文件系统
//作者：lst
//版本：V1.0.0
//文件描述:easynor文件系统主模块
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者: 贺敏
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __EFS_H__
#define __EFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define EFS_NAME_LIMIT            64 // 分区名最大长度,最后一个字节'\0'

// 说明：文件名区域为64字节，61字节为'\0'，后三字节为文件名的ECC
#define FILENAME_LIMIT            60 // 分区名最大长度,最后一个字节'\0'
#define ECC_LIMIT                 3
#define EFS_ITEM_LIMIT            256 // 每个配置项的长度

//文件分配表各成员偏移量
#define FILE_NAME_OFF             (0 ) // 文件名，size=64
#define FILE_STARTBLOCK_OFF       (64) // 文件起始块，size=4
#define FILE_MAXSIZE_OFF          (68) // 文件最大长度
#define FILE_FILESIZE_OFF         (72) // 文件尺寸，size=4，共44
#define FILE_FILESIZE_END         (244) // 最后一个“文件尺寸”
#define FILE_VERIFITY_OFF         (248) // 校验符，="easyfile"
#define FILE_FILESIZE_NUM         (44)  // 文件尺寸个数

#define EFS_SUPPORT_MAXFILESIZE   0x3FFFFFFF            //efs文件系统所支持的最大文件大小
#define EFS_FILESIZE_BYTE_NUMBER   4                    //文件大小的存储占几个字节

#define EFS_RAM_ITEM_BYTE          4    //在RAM中存储文件分配表位置所占的字节数
//
// efs文件系统对flash的操作函数集
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
    u32 StartBlock;         //文件起始块号
    u32 FileSizeLimit;      //文件最大尺寸
    u32 FileSize[44];       //文件尺寸
    char FlagBuffer[8];     //标志，"easyfile",没有'\0'
};
struct EasyFS
{
    u32 block_sum; // 总块数
    u64 block_size; // 块尺寸
    u32 start_block; // 用于存储文件系统的首块块号
    u32 files_sum; // 文件数
    u8 *file_list_buf; // 文件分配表的缓冲区，size= 文件分配表大小

    bool_t file_sys_install;  //文件系统是否已经安装，安装为true，为安装为false
    u32 install_options;
    struct MutexLCB *block_buf_mutex; // 缓冲区互斥
    char name[EFS_NAME_LIMIT+1]; // 挂载点的名字

    // 以下是典型的flash操作函数指针，编写一个具体芯片的驱动时，设为具体芯片的
    // 操作函数,这些函数并不是每一种芯片都需要，不需要的用NULL。
    // 检查把参数规定的内容写入flash前，是否需要擦除。如果buf=NULL则检查由offset
    // 和size设定的区域是否被擦除
    // true = 已准备好，不需要擦除(或空块),false = 需要擦除；
    struct umedia *media;
    struct __efs_media_drv *drv;
};

struct EfsFileInfo
{
    u32 start_block; // 存储本文件的首块号。
    u64 max_size; // 本文件最大长度，filesize不能超过这个长度。
    u32 item; // 该文件分配表在分配表中的位置
    u32 temp_item; //在内存中缓存的分配表中的位置
    u64 filesize; // 文件实际大小
};


#define EF_WR_NOECC         (0x00)
#define EF_WR_ECC           (0x01)

#define     CN_FILE_BUF_LEN          (256)
#define     CN_FILE_BUF_LIMIT        (CN_FILE_BUF_LEN + 3)
struct FileRsc
{
    ptu32_t private; // 与媒体相关的标记，其含义由特定文件系统，driver解释，如flash file driver)中，
                    //保存该文件(目录)的FDT项目号。
    sint64_t file_size; // 文件尺寸,字节数，含仍在缓冲区的数据
    u8 *wr_buf; // 写缓冲区，ecc需要
    u32  buf_off; // 写缓冲区指针
                 // 模块维护
    sint64_t ptr; // 当前读指针，由驱动维护和使用。
    char     name[EFS_NAME_LIMIT];    // 文件名（目录名）,具体所支持的文件名长
                                      //度是驱动模块决定的，故由驱动模块维护。
};


#ifdef __cplusplus
}
#endif

#endif // __EASY_FLASHFILE_H__

