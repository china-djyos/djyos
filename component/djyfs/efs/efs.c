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


#include <stdio.h>
#include <dirent.h>
#include <djyfs/filesystems.h>
#include <stdlib.h>
#include <device/unit_media.h>
#include <string.h>
#include <endian.h>
#include <misc/ecc_256.h>
#include <misc/crc.h>
#include <lock.h>
#include <djyfs/efs.h>
#include <dbug.h>
#include <Object.h>
#include <systime.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_EFS(const char *target, u32 opt, u32 config);
//    ModuleInstall_EFS(CFG_EFS_MOUNT_POINT, CFG_EFS_INSTALL_OPTION, 0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始+
//component name:"easy file system"//easy文件系统
//parent:"file system"//填写该组件的父组件名字，none表示没有父组件
//attribute:system                    //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                    //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                      //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                    //初始化时机，可选值：early，medium，later, pre-main。
                                      //表示初始化时间，分别是早期、中期、后期
//dependence:"file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件将强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"               //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                      //选中该组件时，被依赖组件不会被强制选中，
                                      //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                        //该组件的互斥组件名（可以是none，表示无互斥组件），
                                      //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_EASY_FILE_SYSTEM == false )
//#warning  " easy_file_system  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_EASY_FILE_SYSTEM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,1073741823,
#define CFG_EFS_FILE_SIZE_LIMIT           4096       //"单个文件大小的上限"
#define CFG_EFS_MAX_CREATE_FILE_NUM       50         //"允许创建最大文件数"
#define CFG_EFS_MAX_OPEN_FILE_NUM         10         //"允许同时打开最大文件数"
//%$#@string,1,10,
#define CFG_EFS_MOUNT_POINT               "efs"      //"EFS文件系统安装目录"
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#define CFG_EFS_INSTALL_OPTION            MS_INSTALLCREAT      //"安装选项"，MS_INSTALLFORMAT,MS_INSTALLCREAT,MS_INSTALLUSE中一个或多个的“或”
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

#define MUTEX_WAIT_TIME     CN_TIMEOUT_FOREVER  //(2*1000*1000) // 互斥访问等待时间

s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3);
struct EasyFS *s_pEfsList; // 简易文件系统
static u8 tgOpenedSum = 0; // 打开的文件数
static char NameBuf[256];
static void NameECC(char *Name, u8 *Ecc);
static s32 ChkOrRecNameByECC(char *Name, u8 *Ecc);
static s32 Efs_Verify_Install(struct FsCore *pSuper);
static u32 dwFileMaxSize;   //文件尺寸上限
static u32 EfsCgfLimit;     //用于缓存打开的文件的信息所需存储尺寸
static u32 IndexesNum;      //文件分配表项数=比可创建文件数+1
static u32 CreateMax;       //可创建的文件数
static u32 FileInfoList;    //文件分配表所需内存空间=（可创建文件数+1）*256
static u32 Ram_file_info_len;   //内存中保存单个文件信息所需空间，4+256
// =============================================================================
// 功能：通过ECC修复文件名，如果ECC=ffffff，则表示无须ecc，返回“正确”
// 参数：
// 返回：-1 -- ECC校验失败; 0 -- ECC校验成功;
// =============================================================================
static s32 ChkOrRecNameByECC(char *Name, u8 *Ecc)
{
    u32 Ret;

    if( ((Ecc[0] == 0) && (Ecc[1] == 0) && (Ecc[2] == 0)) ||
            ((Ecc[0] == 0xFF) && (Ecc[1] == 0xFF) && (Ecc[2] == 0xFF))    )
        return 0;

    memset(NameBuf, 0x00, 256);
    strcpy(NameBuf, Name);
    Ret = ECC_HammingVerify256x((u8*)NameBuf, 256, Ecc);      //校验文件名
    if (Ret && (Ret != HAMMING_ERROR_SINGLE_BIT))
    {
        printf("\r\nfile name \"%s\" ecc error [0x%x]\r\n", Name, Ret);
        return (-1);
    }
    return (0);
}
// =============================================================================
// 功能：计算文件名ECC
// 参数：Name -- 文件名； Ecc -- 存ecc值的地址
// 返回：
// =============================================================================
static void NameECC(char *Name, u8 *Ecc)
{
    memset(NameBuf, 0x00, 256);
    strcpy(NameBuf, Name);
    ECC_HammingCompute256x((u8*)NameBuf, 256, Ecc);
}
//------------------------------------------------------------------------------
//功能: 计算奇校验位
//参数: odd -- 文件尺寸奇数位或偶数位被清零后的值
//返回: 如果1的个数为奇数个，则返回0，如果1的个数为偶数个，则返回1。
//-----------------------------------------------------------------------------
static u32 __Efs_paritycheck(u32 odd)
{
    u32 val=1;
    u32 flag=1;
    while(odd)
    {
        val ^= (odd & flag);
        odd >>= 1;
    }
    return val & 0x1;
}
//------------------------------------------------------------------------------
//功能: 计算文件大小的奇偶校验,奇数位和偶数位的奇校验分别放在第30位和第31位
//参数: newsize -- 文件大小
//返回: 返回加上了奇偶校验的文件大小
//-----------------------------------------------------------------------------
static u32 __EfsFileSize_odd_even_check(u32 newsize)
{
    u32 ret = newsize,odd,even;

    if(ret > EFS_SUPPORT_MAXFILESIZE)
    {
        ret = EFS_SUPPORT_MAXFILESIZE;
        warning_printf("efs","The file size exceeds the maximum file size supported by efs ");
    }
    odd = ret & 0x15555555;     //获取所有奇数位
    odd = __Efs_paritycheck(odd);   //获取所有偶数数位
    even = ret & 0x2aaaaaaa;
    even = __Efs_paritycheck(even);   //获取所有偶数数位
    ret = (ret | (odd << 30) | (even << 31));
    return ret;
}

//------------------------------------------------------------------------------
//功能: 校验文件大小和校验位是否匹配
//参数: filesize -- 文件大小
//返回: 0 -- 匹配 ； -1 -- 不匹配
//-----------------------------------------------------------------------------
static s32 __EfsFileSize_verify(u32 filesize)
{
    u32 ret = filesize,odd,even;

    odd = ((ret >> 30) & 0x1);
    even = ((ret >> 31) & 0x1);

    if((odd != __Efs_paritycheck(ret & 0x15555555)) || (even != __Efs_paritycheck(ret & 0x2aaaaaaa)))
        return -1;

    return 0;
}

//----检查文件分配表filesize-----------------------------------------------------------
//功能: 检查一个文件分配表的filesize，只检查，不作任何改写
//参数: buf，缓冲区，存有文件的26个大小
//      location，filesize数据的位置，如果filesize出错，则是出错的位置
//返回: 0 = 无错，1=有错没有改正
//-----------------------------------------------------------------------------
static u32 __Efs_CheckFileSize(u8 *buf, u32 *location)
{
    u32 filesize_no, offset;
    u32 ret = 0;

    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM;filesize_no++)
    {
        offset = filesize_no * EFS_FILESIZE_BYTE_NUMBER;
        if(pick_little_32bit(buf + offset,0) == CN_LIMIT_UINT32)//为全1，则该条无错
            break;
        else if(pick_little_32bit(buf + offset,0) == 0)//表示被改写过的大小，为全0,直接跳过
            continue;
        else if(__EfsFileSize_verify(*(u32 *) (buf + offset)))//奇偶校验有错
        {
            ret = 1;
            break;
        }
    }
    *location = filesize_no;
    return ret;
}
//----检查扇区缓冲区-----------------------------------------------------------
//功能: 检查一个文件分配表的扇区缓冲区，有错误则改正之,但不写入芯片。若无错或有
//      错但能改正，同时把该缓冲区包含的文件数增加到chip->files_sum。
//参数: buf，缓冲区
//      index，第几个扇区
//      efs，文件系统核心数据
//返回: 0 = 无错，1=有错并改正，2=有错不能改正
//-----------------------------------------------------------------------------
static u32 __Efs_CheckSingleBuf(u8 *buf, u32 index, struct EasyFS *efs)
{
    u32 offset, location, item_no,item_num;
    u32 temp_files_sum = 0;
    u32 ret = 0;

    item_num = (FileInfoList > efs->block_size)?
            efs->block_size:FileInfoList;
    item_num = item_num / EFS_ITEM_LIMIT;       //一个块中存了多少文件索引
    if(index == efs->start_block)
        item_no = 1;            //如果是efs文件系统的起始块，则从第二条开始检查，第一条存的是文件系统信息
    else
        item_no = 0;

    for (;item_no < item_num;item_no++)
    {
        offset = item_no * EFS_ITEM_LIMIT;
        if( (buf[offset] != 0xFF) && (buf[offset] != 0x00) )//name为非空，表示该条有用
        {
            temp_files_sum++; //该条用到，则文件数加1
            offset += FILE_FILESIZE_OFF;
            if(__Efs_CheckFileSize(buf + offset,&location)) //有错，则将filesize改写成0
            {
                ret = 1;
                memset(buf + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);   //如果文件大小出错，则把错误的大小清除，全写0
            }
        }
    }
    //把本块内所有文件数放到efs->files_sum
    efs->files_sum += temp_files_sum;
    return ret;
}

//----检查扇区-----------------------------------------------------------
//功能: 检查一对(主+备份)文件分配表的扇区缓冲区，有错误则改正之,该数据已经在
//      flash中，改正后写进去。同时把该缓冲区包含的文件数增加到chip->files_sum。
//参数: buf_main -- 主文件分配表缓冲区；
//      index_main -- 主文件分配表起始块号
//      buf_bakindex -- 备份文件分配表缓冲区；
//      index_bak -- 备份文件分配表起始块号
//      efs，文件系统核心数据
//返回: 0 = 主备相同；1=主备有差别
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckBlock(u8 *buf_main,u32 index_main, u8 *buf_bak,
                    u32 index_bak, struct EasyFS *efs)
{
    u32 item_no,offset,item_num;
    u32 reflash = 0, location;
    u32 temp_files_sum = 0;
    u8 * correct_filesize;
    item_num = (FileInfoList > efs->block_size)?
            efs->block_size:FileInfoList;
    item_num = item_num / EFS_ITEM_LIMIT;       //文件条目缓冲中存了多少文件索引
    if(index_main == efs->start_block)
        item_no = 1;        //如果是efs文件系统的起始块，则从第二条开始检查，第一条存的是文件系统信息
    else
        item_no = 0;
    for (;item_no < item_num;item_no++)
    {
        offset = item_no * EFS_ITEM_LIMIT;
        if ((buf_main[offset] != 0xFF) || (buf_bak[offset] != 0xFF)) //从名称判断是否有用到该文件配置
        {
            if( (buf_main[offset] != 0x00) && (buf_bak[offset] != 0x00) ) //若为0，则表示该文件被删除
            {
                temp_files_sum++;
            }

            //检查主备分配表name,startblock,maxsize,crc
            if(memcmp(buf_main+offset,buf_bak+offset,FILE_FILESIZE_OFF))//若不相同
            {
                //主分配表错误,则发生擦除或者写入错误，需要要本条item全部复制
                if(memcmp(buf_main + FILE_VERIFITY_OFF, "easyfile", 8))
                {
                    memcpy(buf_main+offset,buf_bak+offset,EFS_ITEM_LIMIT);
                }
                else    //备用分配表错误
                {
                    memcpy(buf_bak+offset,buf_main+offset,EFS_ITEM_LIMIT);
                }
                reflash = 1;
                continue;
            }

            //核对主备文件分配表的filesize......todo:此处如果是nandflash,是否直接改成有错便擦除？
            offset += FILE_FILESIZE_OFF;
            if(memcmp(buf_main+offset,buf_bak+offset,
                    FILE_VERIFITY_OFF - FILE_FILESIZE_OFF))//主备分配表filesize大小不同
            {
                reflash = 1;        //nandflash 有错尽量去纠正，同时擦除该块
                if (__Efs_CheckFileSize(buf_main + offset, &location)) //主分配表有错
                {
                    correct_filesize = buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location;
                    if(pick_little_32bit(correct_filesize,0) == CN_LIMIT_UINT32)//主分配表filesize错误，但是bak还没写
                    {
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//与主分配表保持一致
                        memset(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                    }
                    else if(location == FILE_FILESIZE_NUM)//错误的是最后一条filesize
                    {
                        reflash = 1;
                        memset(buf_main+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset,correct_filesize,EFS_FILESIZE_BYTE_NUMBER);//保存正确filesize到第一条位置
                        memset(buf_bak+offset,0xff,FILE_VERIFITY_OFF-FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset,buf_main+offset,EFS_FILESIZE_BYTE_NUMBER);//保存正确filesize到第一条位置
                    }
                    else////主分配表filesize错误，但是bak已写，即bak文件大小正确
                    {
                        //将写为0的主filesize写到备份中相对应的位置
                        memset(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);//将主分配表中filesize错误条写成0
                        memcpy(buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location + EFS_FILESIZE_BYTE_NUMBER,
                                                        correct_filesize,EFS_FILESIZE_BYTE_NUMBER);//从备份表中拷贝正确的filesize到下一条
                        memcpy(correct_filesize + EFS_FILESIZE_BYTE_NUMBER,correct_filesize,EFS_FILESIZE_BYTE_NUMBER);
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//与主分配表保持一致
                    }
                }
                else//主分配表正确
                {
                    correct_filesize = buf_main + offset + EFS_FILESIZE_BYTE_NUMBER * location;
                    if(pick_little_32bit(correct_filesize,0) == CN_LIMIT_UINT32)//主分配表filesize错误，但是bak还没写
                    {
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER);//与主分配表保持一致
                        memset(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                    }
                    else if(location == FILE_FILESIZE_NUM)//错误的是最后一条filesize
                    {
                        reflash = 1;
                        memset(buf_bak+offset, 0xff, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
                        memcpy(buf_bak+offset, correct_filesize, EFS_FILESIZE_BYTE_NUMBER);//保存正确filesize到第一条位置
                        memset(buf_main+offset, 0xff, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
                        memcpy(buf_main+offset, buf_bak + offset, EFS_FILESIZE_BYTE_NUMBER);//保存正确filesize到第一条位置
                    }
                    else
                    {
                        //将bak该filesize写成0
                        memset(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location, 0, EFS_FILESIZE_BYTE_NUMBER);
                        memcpy(correct_filesize + EFS_FILESIZE_BYTE_NUMBER, correct_filesize, EFS_FILESIZE_BYTE_NUMBER); //下移并拷贝
                        memcpy(buf_bak + offset + EFS_FILESIZE_BYTE_NUMBER * location + EFS_FILESIZE_BYTE_NUMBER,
                                                        correct_filesize, EFS_FILESIZE_BYTE_NUMBER); //拷贝主文件分配表的filesize
                        memset(correct_filesize, 0, EFS_FILESIZE_BYTE_NUMBER); //与备用分配表相持一致
                    }
                }
            }
        }
    }

    efs->files_sum += temp_files_sum;
    return reflash;
}
//-----------------------------------------------------------------------------
//功能: 检查和修复所有文件的文件名
//参数: FileInfoBuf -- 所有文件信息的缓冲区
//返回:
//-----------------------------------------------------------------------------
static bool_t __Efs_CheckAllNameECC(u8 *FileInfoBuf)
{
    u32 offset, loop;
    u8 *buf;

    for (loop = 1; loop < IndexesNum; loop++)
    {
        offset = loop * EFS_ITEM_LIMIT;
        buf = FileInfoBuf + offset;
        if( (buf[0] != 0xFF) && (buf[0] != 0x00) )
        {
            buf[FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)buf,buf+FILENAME_LIMIT+1);  //通过ECC检查和修复文件名
        }
    }
    return true;
}
//-----------------------------------------------------------------------------
//功能: 整理一个flash块中的文件大小空间，文件信息里会有44个文件大小，最后一个才是用的，该函数的作用
//就是把一个块中，所有的文件信息中的文件大小整理一个，例如某个文件信息里有10个文件大小，只有最后一个是有效的
//则把前9个文件大小删除，只留下有效的文件大小。
//参数: core -- efs文件系统管理；file_info_buf某个块中的数据
//返回:
//-----------------------------------------------------------------------------
static void __Efs_Reset_File_Size(struct EasyFS *efs, u8 *file_info_buf)
{
    u8 *hsize, *buf;
    u32 loop, item, block_info_num, file_size = 0;
    if(FileInfoList > efs->block_size)
        block_info_num = efs->block_size / EFS_ITEM_LIMIT;
    else
        block_info_num = FileInfoList / EFS_ITEM_LIMIT;

    for (loop = 0; loop < block_info_num; loop++)
    {
        buf = file_info_buf + (loop * EFS_ITEM_LIMIT);
        if (memcmp(buf, "easyfile", 8) || memcmp(buf + FILE_VERIFITY_OFF, "easyfile", 8))
        {
            if((*buf != 0x00) && (*buf != 0xFF))
            {
                for (item = 0; item < FILE_FILESIZE_NUM; item++)
                {
                    hsize = (buf + FILE_FILESIZE_END - item * EFS_FILESIZE_BYTE_NUMBER);
                    if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                        (pick_little_32bit(hsize,0) == 0))
                        continue;
                    if (__EfsFileSize_verify(*(u32 *)hsize) == 0)
                    {   //获取目标文件的大小
                        file_size = (pick_little_32bit(buf,     //从44个文件大小中，找正确的文件大小，从最后开始找
                                (FILE_FILESIZE_END - item * EFS_FILESIZE_BYTE_NUMBER) >> 2));
                        break;
                    }
                }
                if(item != 43)
                {
                    memset(buf + FILE_FILESIZE_OFF, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
//                    file_size = __EfsFileSize_odd_even_check(file_size); //加上校验后的文件大小
                    fill_little_32bit(buf, FILE_FILESIZE_OFF >> 2, file_size);
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
//功能: 刷新内存里的文件列表
//参数: efs -- efs文件系统管理
//返回:
//-----------------------------------------------------------------------------
static bool_t __Efs_RefreshFileList(struct EasyFS *efs)
{
    char *file_name;
    u8 *file_info_buf;
    u32 i, filesize_no;

    if(efs == NULL)
        return false;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    for(i = 1; i < CFG_EFS_MAX_OPEN_FILE_NUM; i++)
    {
        file_name = (char*)(efs->file_list_buf + (i * Ram_file_info_len));
        if(*file_name != (char)0xff)
        {
            for (filesize_no = 1; filesize_no < IndexesNum; filesize_no++)
            {
                if(!efs->drv->efs_read_media(efs->start_block, filesize_no * EFS_ITEM_LIMIT, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
                    memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
                file_info_buf[FILENAME_LIMIT] = '\0';
                ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
                if (strncmp((const char*)file_info_buf ,file_name, FILENAME_LIMIT) == 0)
                {     //找到目标文件
                    memcpy(efs->file_list_buf + (i * Ram_file_info_len), file_info_buf, EFS_ITEM_LIMIT);
                    break;
                }
            }
        }

    }
    free(file_info_buf);
    return true;
}


//----修改文件尺寸-------------------------------------------------------------
//功能: 修改文件分配表中文件尺寸部分。1、在主分配表中找一个空闲位置写入。2、如果
//      找不到，则读出整块，把整个filesize区填充成ff，然后把文件尺寸更新到第一个
//      空位上，擦除块并写入之。3、在bak上做相同的操作。
//参数: ob，待操作的文件对象指针
//      newsize，新的文件尺寸
//返回: 无
//-----------------------------------------------------------------------------
static void __Efs_ChangeFileSize(struct Object *ob, u32 newsize)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u8 *block_buf;
    u32 fileoffset,sizeoffset, cfg_blocks,temp;
    u32 filesize_no,file_max_size,block_no,block_addr;
    u16 crc16_check;

    if(ob == NULL)
        return ;
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *) fp->private;
    if((efs == NULL) || (fp == NULL) || (fileinfo == NULL))
        return ;

    cfg_blocks = (FileInfoList + efs->block_size - 1) / efs->block_size;  //文件分配表所占块数
    block_buf = efs->file_list_buf + (fileinfo->temp_item * Ram_file_info_len);
    ChkOrRecNameByECC((char *)block_buf,block_buf+FILENAME_LIMIT+1);
    //文件尺寸不能大于文件最大尺寸
    file_max_size = pick_little_32bit(block_buf,FILE_MAXSIZE_OFF >> 2);
    if(newsize > file_max_size)
        return;

    fileoffset = fileinfo->item * EFS_ITEM_LIMIT;
    sizeoffset = FILE_FILESIZE_OFF;
    newsize = __EfsFileSize_odd_even_check(newsize); //加上校验后的文件大小
    for (filesize_no = 0; filesize_no < FILE_FILESIZE_NUM; filesize_no++)
    {
        if (pick_little_32bit(block_buf + sizeoffset, 0) == CN_LIMIT_UINT32) //在主分配表找到空闲位置
        {
            fill_little_32bit(block_buf, sizeoffset >> 2, newsize);
            efs->drv->efs_write_media(efs->start_block, fileoffset + sizeoffset,
                    block_buf + sizeoffset, EFS_FILESIZE_BYTE_NUMBER,EF_WR_NOECC);  //写新的文件大小，文件大小占4字节
            efs->drv->efs_write_media(efs->start_block + cfg_blocks, fileoffset + sizeoffset,
                    block_buf + sizeoffset, EFS_FILESIZE_BYTE_NUMBER,EF_WR_NOECC);
            break;
        }
        sizeoffset += EFS_FILESIZE_BYTE_NUMBER;
    }

    if (filesize_no == FILE_FILESIZE_NUM) //主分配表没有空余位置
    {
        memset(block_buf + FILE_FILESIZE_OFF, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);
        fill_little_32bit(block_buf, FILE_FILESIZE_OFF >> 2, newsize);
        //先读出整块数据到buf,并擦除
        block_buf = NULL;
        block_buf = malloc(FileInfoList);
        if(block_buf == NULL)
        {
            warning_printf("efs", "failed to modify file size \r\n");
            return ;
        }
        efs->drv->efs_read_media(efs->start_block, 0, block_buf, FileInfoList, EF_WR_NOECC);
        __Efs_CheckAllNameECC(block_buf);

        sizeoffset = fileinfo->item * EFS_ITEM_LIMIT + FILE_FILESIZE_OFF;      //该文件，文件大小的位置
        memset(block_buf + sizeoffset, 0xFF, FILE_VERIFITY_OFF - FILE_FILESIZE_OFF);  //把该文件原来存的44个文件大小清除
        fill_little_32bit(block_buf, sizeoffset >> 2, newsize);     //把新的文件大小填进去

        block_no = fileoffset / efs->block_size;
        block_addr = block_no * efs->block_size;

        efs->drv->efs_erase_media(efs->start_block + block_no);       //擦主区中该文件信息所属的块
        __Efs_Reset_File_Size(efs, block_buf + block_addr);
        if((FileInfoList - block_addr) < efs->block_size)   //如果要写的数据小于一块的大小，那就休实际需要写的大小
            efs->drv->efs_write_media(efs->start_block + block_no, 0, block_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
        else
            efs->drv->efs_write_media(efs->start_block + block_no, 0, block_buf + block_addr, efs->block_size, EF_WR_NOECC);
        if((efs->start_block + cfg_blocks - 1) == (efs->start_block + block_no))   //如果被擦除的块，是存了CRC校验码的，则重新写如CRC
        {
            crc16_check = CRC_16(block_buf, 16);
            fill_little_16bit((u8*)&temp, 0, crc16_check);    //转化为小端
            efs->drv->efs_write_media(efs->start_block + cfg_blocks - 1, efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
        }

        efs->drv->efs_erase_media(efs->start_block + block_no + cfg_blocks);        //擦副区中该文件信息所属的块
        if((FileInfoList - block_addr) < efs->block_size)
            efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, block_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
        else
            efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, block_buf + block_addr, efs->block_size, EF_WR_NOECC);
        if((efs->start_block + 2*cfg_blocks - 1) == (efs->start_block + block_no + cfg_blocks))
            efs->drv->efs_write_media(efs->start_block + 2*cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC);

        __Efs_RefreshFileList(efs);
        free(block_buf);
    }
}

// =============================================================================
// 功能：新建并打开文件，初始化文件相关的信息，文件大小为0，最大尺寸为默认
// 参数：fp -- 文件私有数据；ob -- 文件的对象指针；filename -- 文件名； fileinfo -- 文件信息指针
// 返回：fp -- 创建成功；NULL -- 创建失败
// =============================================================================
static struct FileRsc *__Efs_NewFile(struct FileRsc* fp,struct Object *ob,const char *filename,
                        struct EfsFileInfo *fileinfo)
{
    struct EasyFS *efs;
    u8 *file_info_buf;
    static u8 seat = 0;
    u32 block_no, start_block, offset,block_addr;
    u8 cfg_blocks, item, loop, fill = 0;
    u64 index_offset;
    u16 crc16_check, temp;
    if( (ob == NULL) || (strlen(filename) > FILENAME_LIMIT) )//文件名太长
    {
        printf("Create New File Failed For Too Long Name!\r\n");
        return NULL;
    }

    efs = (struct EasyFS*)File_Core(ob);
    if (efs->files_sum >= CreateMax)
        return NULL; //已达可创建文件的上限

    offset = 0;
    //计算文件系统文件分配表主分配所使用的blocks
    cfg_blocks = (FileInfoList + efs->block_size-1) / efs->block_size;
    //查找为全0或全FF的位置
    file_info_buf = malloc(FileInfoList);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return NULL;
    }
    efs->drv->efs_read_media(efs->start_block, 0, file_info_buf, FileInfoList, EF_WR_NOECC);
    __Efs_CheckAllNameECC(file_info_buf);
    for(item = 1; item < IndexesNum; item++)  //第一个文件的索引是在文件分配表的第二个位置，所以这里加1
    {
        offset = item * EFS_ITEM_LIMIT;
        if(file_info_buf[offset] == 0x00)  //若为0，则表示该文件被删除,可用于新建文件
        {
            block_no = offset / efs->block_size;
            block_addr = block_no * efs->block_size;
            memset(file_info_buf+offset,0xFF,EFS_ITEM_LIMIT);
            __Efs_Reset_File_Size(efs, file_info_buf + block_addr);
//            //added,将最后的CRC写到文件系统信息的最后两个字节
            efs->drv->efs_erase_media(efs->start_block + block_no);       //擦主区中该文件信息所属的块
            if((FileInfoList - block_addr) < efs->block_size)   //如果要写的数据小于一块的大小，那就休实际需要写的大小
                efs->drv->efs_write_media(efs->start_block + block_no, 0, file_info_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
            else
                efs->drv->efs_write_media(efs->start_block + block_no, 0, file_info_buf + block_addr, efs->block_size, EF_WR_NOECC);
            if((efs->start_block + cfg_blocks - 1) == (efs->start_block + block_no))//如果被擦除的块，是存了CRC校验码的，则重新写如CRC
            {
                crc16_check = CRC_16(file_info_buf,16);
                fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端
                efs->drv->efs_write_media(efs->start_block + cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2,EF_WR_NOECC);
            }
            efs->drv->efs_erase_media(efs->start_block + block_no + cfg_blocks);       //擦副区中该文件信息所属的块
            if((FileInfoList - block_addr) < efs->block_size)
                efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, file_info_buf + block_addr, FileInfoList - block_addr, EF_WR_NOECC);
            else
                efs->drv->efs_write_media(efs->start_block + block_no + cfg_blocks, 0, file_info_buf + block_addr, efs->block_size, EF_WR_NOECC);
            if((efs->start_block + 2*cfg_blocks - 1) == (efs->start_block + block_no + cfg_blocks))
                efs->drv->efs_write_media(efs->start_block + 2*cfg_blocks - 1, efs->block_size-2, (u8*)&temp, 2,EF_WR_NOECC);
            break;
        }
        if(file_info_buf[offset] == 0xFF) //从名称判断是否有用到该文件配置
        {
            break;          //空闲位置
        }
    }

    if(item == IndexesNum)        //没有找到空闲位置或文件数量达到上限
    {
        free(file_info_buf);
        return NULL;
    }

    //计算新建的文件内容存储位置
    start_block = efs->start_block + 2*cfg_blocks + (item - 1)*((dwFileMaxSize + efs->block_size-1)/efs->block_size);
    //同时需要擦除文件内容所在的块数据
    block_no = (dwFileMaxSize + efs->block_size-1) / efs->block_size;
    for(temp = 0; temp < block_no; temp++)
    {
        efs->drv->efs_erase_media(temp + start_block);
    }
    block_no = efs->start_block;
    //以下初始化文件分配表并写入flash
    memset((char *)file_info_buf + offset, 0xff, EFS_ITEM_LIMIT);
    strcpy((char *)file_info_buf + offset, filename);        //写入文件名
    file_info_buf[FILENAME_LIMIT + offset] = '\0';
    NameECC((char *)filename, file_info_buf + offset + FILENAME_LIMIT+1);        //文件名ECC
    fill_little_32bit(file_info_buf + offset, FILE_STARTBLOCK_OFF >> 2,start_block);      //文件内容起始块
    fill_little_32bit(file_info_buf + offset, FILE_MAXSIZE_OFF >> 2, dwFileMaxSize);      //文件最大大小
    fill_little_32bit(file_info_buf + offset, FILE_FILESIZE_OFF >> 2, 0xc0000000);        //因为是新建的文件，文件大小先写0
                                                                                          //最高两位分别是奇数位和偶数位的奇校验
    memcpy(file_info_buf + offset + FILE_VERIFITY_OFF, "easyfile", 8);              //文件索引末尾写easyfile
    efs->drv->efs_write_media(block_no, offset, file_info_buf + offset, EFS_ITEM_LIMIT, EF_WR_NOECC);
    efs->drv->efs_write_media(block_no+cfg_blocks, offset, file_info_buf + offset, EFS_ITEM_LIMIT, EF_WR_NOECC);
    memcpy(fp->name,filename,FILENAME_LIMIT+1);
    //更新文件信息，创建即打开
    fp->private = (ptu32_t)fileinfo;
    fp->file_size = 0;
    fp->ptr = 0;

    //更新文件信息，创建即打开
    fileinfo->max_size = dwFileMaxSize;
    fileinfo->start_block = start_block;
    fileinfo->item = item;
    fileinfo->filesize = 0;

    __Efs_RefreshFileList(efs);

    for (loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
    {
        index_offset = loop * Ram_file_info_len;
        if(efs->file_list_buf[index_offset] == 0x00)  //如果为空则表示这段文件信息为空
        {
            memcpy((char *)efs->file_list_buf + index_offset, (const char *)(file_info_buf + offset), EFS_ITEM_LIMIT);
            fill = 1;
            fileinfo->temp_item = loop;
            fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, item);
            break;
        }
    }
    if(fill == 0)
    {
        memcpy((char *)efs->file_list_buf + (seat * Ram_file_info_len), (const char *)(file_info_buf + offset), EFS_ITEM_LIMIT);
        fill_little_32bit(efs->file_list_buf + (seat * Ram_file_info_len) + EFS_ITEM_LIMIT, 0, item);
        fileinfo->temp_item = seat;
        seat++;
        if(seat > 9)
            seat = 0;
    }

    free(file_info_buf);
    return fp;
}
// =============================================================================
// 功能：打开文件，若文件不存在，则可根据mode方式，决定是否新建文件
// 参数：ob -- 文件的对象指针
//      flags -- 打开方式
//      uncached -- 路径未匹配的部分
// 返回：文件句柄hdl -- 成功； NULL -- 失败
// =============================================================================
static struct objhandle *Efs_Open(struct Object *ob, u32 flags, char *uncached)
{
    struct EasyFS *efs;
    struct FileRsc *fp=NULL;
    struct EfsFileInfo *fileinfo=NULL;
    u32 loop,item;
    u64 index_offset;
    u8 *hsize,*buf=NULL;
    bool_t found = false;
    struct objhandle *hdl;
//    mode_t mode, property = 0;
    if(ob == NULL)
        return NULL;

    efs = (struct EasyFS*)File_Core(ob);
    if (efs == NULL)
        return NULL;

    if(efs->file_sys_install == false)
    {
        struct Object *current;
        current = ob;
        while( (!File_ObjIsMount(current)) && (current != OBJ_GetRoot()))
        {
            current = OBJ_GetParent(current);
        }
        if(Efs_Verify_Install((struct FsCore *)OBJ_GetPrivate(current)))
        {
            warning_printf("efs", "file system set up fail");
            return (NULL);     //文件系统建立失败
        }
    }

    if((!uncached)&&(File_ObjIsMount(ob)))
    {
        if(!Handle_FlagIsDirectory(flags))  //没有未匹配的路径，并且ob是挂载点，则是打开efs的根目录
            return (NULL);  //如果不是打开目录的操作则直接返回失败
    }

    if(tgOpenedSum >= CFG_EFS_MAX_OPEN_FILE_NUM)
        return NULL;

    if(Handle_FlagIsDirectory(flags))
    {
//        property = S_IFDIR;     // 目录逻辑不做其它操作，直接把obj和hal关联就行了
        if((uncached) || (!File_ObjIsMount(ob)))
            return NULL;
    }
    else
    {
        if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
            return (NULL);

        fp = malloc(sizeof(struct FileRsc));
        fileinfo = malloc(sizeof(struct EfsFileInfo));
        buf = malloc(CN_FILE_BUF_LIMIT);
        if( (NULL == fileinfo) || (NULL == buf) || (efs == NULL) || (fp == NULL))
            goto exit;

        memset(fp, 0, sizeof(struct FileRsc));
        memset(fileinfo, 0, sizeof(struct EfsFileInfo));
        memset(buf, 0xFF, CN_FILE_BUF_LIMIT);
        //扫描固定文件数
//        for (loop = 1; loop < IndexesNum; loop++)
        for (loop = 1; loop <= CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
        {
            index_offset = loop * Ram_file_info_len;
            efs->file_list_buf[index_offset + FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)(efs->file_list_buf + index_offset), efs->file_list_buf + index_offset + FILENAME_LIMIT + 1);
            if (strncmp((const char*)efs->file_list_buf + index_offset, uncached, FILENAME_LIMIT) == 0)
            {
                found = true;
                fileinfo->temp_item = loop;
                fileinfo->item = pick_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0 );
                break;
            }
        }
        if(!found)
        {
            u8 *file_info_buf, fill = 0;
            static u8 seat = 0;
            file_info_buf = malloc(EFS_ITEM_LIMIT);
            if(file_info_buf == NULL)
            {
                warning_printf("efs", "memory request failed \r\n");
                goto exit;
            }
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
            for (loop = 1; loop < IndexesNum; loop++)
            {
                if(!efs->drv->efs_read_media(efs->start_block, loop * EFS_ITEM_LIMIT, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
                {
                    warning_printf("efs", "read file info fail \r\n");
                    goto exit;
                }
                file_info_buf[FILENAME_LIMIT] = '\0';
                ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
                if (strncmp((const char*)file_info_buf, uncached, FILENAME_LIMIT) == 0)
                {
                    found = true;
                    for (item = 0; item < CFG_EFS_MAX_OPEN_FILE_NUM; item++)
                    {
                        index_offset = item * Ram_file_info_len;
                        if(efs->file_list_buf[index_offset] == 0x00)  //如果为空则表示这段文件信息为空
                        {
                            memcpy((char *)efs->file_list_buf + index_offset, (const char *)file_info_buf, EFS_ITEM_LIMIT);
                            fill = 1;
                            fileinfo->temp_item = item;
                            fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, loop);
                        }
                    }
                    if(fill == 0)
                    {
                        memcpy((char *)efs->file_list_buf + (seat * Ram_file_info_len), (const char *)file_info_buf, EFS_ITEM_LIMIT);
                        fill_little_32bit(efs->file_list_buf + (seat * Ram_file_info_len) + EFS_ITEM_LIMIT, 0, loop);
                        index_offset = seat * Ram_file_info_len;
                        fileinfo->temp_item = seat;
                        seat++;
                        if(seat > 9)
                            seat = 0;
                    }
                    fileinfo->item = loop;      //把该文件索引在文件分配表中的位置保存下来
                    break;
                }
            }
            free(file_info_buf);
        }
        if (!found)
        {
            if(Handle_FlagIsCreate(flags))//未找到，判断是否是新建文件
            {
                if(__Efs_NewFile(fp, ob, uncached, fileinfo))
                    efs->files_sum++;
                else
                {
                    if (efs->files_sum >= CreateMax)
                    {
                        warning_printf("efs", "file sum exceed upper limit (%d)",CreateMax);
                    }
                    goto exit;
                }
            }
            else
            {   // 文件不存在，但不要求新建；
                printf("\r\n : dbug : efs    : open \"%s\" failed(do not exist).", uncached);
                goto exit;
            }
        }
        else     //此处 文件存在
        {
            if(Handle_FlagIsOnlyCreate(flags)) // 已存在，但只要求新建；
            {
                printf("\r\n : dbug : efs    : open \"%s\" failed(already exist).", uncached);
                goto exit;
            }
            //以下设置文件资源描述，参照struct file_rsc定义中要求驱动模块维护的部分。
            for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
            {
                hsize = (efs->file_list_buf + index_offset + FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER);
                if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                    (pick_little_32bit(hsize,0) == 0))
                    continue;
//                if (pick_little_32bit(hsize,0) == ~(pick_little_32bit(hsize + 4,0)))
                if(__EfsFileSize_verify(*(u32 *)(hsize)) == 0)
                {
                    fp->file_size = (pick_little_32bit(efs->file_list_buf + index_offset,     //从44个文件大小中，找正确的文件大小，从最后开始找
                            (FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER) >> 2) & EFS_SUPPORT_MAXFILESIZE);
                    break;
                }
            }
            fileinfo->start_block = pick_little_32bit(efs->file_list_buf + index_offset, FILE_STARTBLOCK_OFF >> 2);          //获取文件内容的开始块
            fileinfo->max_size = pick_little_32bit(efs->file_list_buf + index_offset, FILE_MAXSIZE_OFF >> 2);       //获取文件最大大小
            fileinfo->filesize = fp->file_size;     //获取文件当前大小
            memcpy(fp->name, (const char *)(efs->file_list_buf + index_offset), FILENAME_LIMIT+1);
        }

        fp->wr_buf = buf;       //文件写缓存
        fp->buf_off = 0;        //文件写缓存中的偏移
        if(Handle_FlagIsAppend(flags))
            fp->ptr = fileinfo->filesize;       //追加模式，则把文件当前位置设置为文件大小
        else
            fp->ptr = 0;

        fp->private = (ptu32_t)fileinfo;
//        property = S_IFREG;
        tgOpenedSum ++;
//        if(!OBJ_NewChild(ob, e_operations, (ptu32_t)fp, uncached))
//        {
//            printf("\r\n: erro : efs    : new file \"%s\"(virtual).", uncached);
//            goto exit;
//        }
    }

    hdl = Handle_New();
    if(!hdl)
    {
        printf("\r\n : erro : efs    : open failed(memory out).");
        goto exit;
    }

    Handle_Init(hdl, NULL, flags, (ptu32_t)0);
    //继承操作方法，对象的私有成员保存访问模式（即 stat 的 st_mode ）
    ob = OBJ_BuildTempPath(ob, e_operations, (ptu32_t)fp,uncached);
    OBJ_LinkHandle(hdl, ob);

    Lock_MutexPost(efs->block_buf_mutex);
    return hdl;

exit:
    free(buf);
    free(fileinfo);
    free(fp);
    Lock_MutexPost(efs->block_buf_mutex);
    return NULL;
}

// =============================================================================
// 功能：读目录，即读第一个文件或者fname的下一个文件
// 参数：hdl -- 文件句柄；dentry -- 目录结构
// 返回：全部读完（1）；失败（-1）；读了一项（0）；
// =============================================================================
static s32 Efs_DirRead(struct objhandle *hdl,struct dirent *dentry)
{
    s32 result = -1;
    static u8 *file_info_buf = NULL;
    struct EasyFS *efs;
    u32 loop = 1;
    u64 index_offset;
    struct Object *ob = hdl->HostObj;
    if(NULL == dentry)
        return -1;

    efs = (struct EasyFS*)File_Core(ob);
    if(NULL == efs)
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    //判断是否存在该文件
    if(dentry->d_off < 1)
    {
        dentry->d_off = 1;
        file_info_buf = malloc(EFS_ITEM_LIMIT);
    }
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }
    loop = dentry->d_off;   //文件分配表中的偏移
    for(; loop < IndexesNum; loop++ )       //查找fname下一个文件或第一个文件
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);

        file_info_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        //查找下一个有效的文件
        if((*file_info_buf != 0x00) && (*file_info_buf != 0xFF))
        {
            memcpy((char *)dentry->d_name, file_info_buf, FILENAME_LIMIT+1);     //获取文件名
            result = 0;
            break;
        }
    }

    if(loop == IndexesNum)
    {
        dentry->d_off = 1;
        if(result == 0)
        {
            result = 1;  //遍历完所有文件
            free(file_info_buf);
            file_info_buf = NULL;
        }
        else
            result = -1;
    }
    else
        dentry->d_off = loop + 1;
    Lock_MutexPost(efs->block_buf_mutex);
    if(result != -1)
    {
        dentry->d_reclen = sizeof(dentry->d_name);
        dentry->d_type |= DIRENT_IS_REG;        //efs不支持创建目录，所以这里赋值属性始终是文件属性
    }

    return result;
}

// =============================================================================
// 功能：读 文件
// 参数：hdl,文件指针
//     buf,读文件数据存储的缓冲区
//     len,读文件字节数
// 返回：读取的实际长度 -- 成功；0 -- 失败；
// =============================================================================
static u32 Efs_Read (struct objhandle *hdl, u8 *buf, u32 len)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u32 buf_off, copy_len, block, readlen, rdsz = 0, completed = 0;
    u64 rd_point, blk_off;
    u8 rDataBuf[CN_FILE_BUF_LIMIT];
    struct Object *ob = hdl->HostObj;
    //文件所在flash芯片指针
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((len == 0) || (buf == NULL) || (fp == NULL) || (efs == 0) || (fileinfo == NULL))
    {
        printf("\r\nEfs_Read - FSR_INVALID_PARAMETER\r\n");
        return 0;
    }

    rd_point = fp->ptr;     //读的位置
    if((sint64_t)(rd_point + len) > fp->file_size)
        len = fp->file_size - rd_point;     //文件当前读取位置加需要读取的长度大于文件大小的话。则需要读的数据长度为文件大小减去文件当前的读取位置

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
        printf("\r\nEfs_Read - FSR_DENIED- locked\r\n");
        return 0;
    }
    while(((s32)len) > 0)
    {
        block = rd_point / efs->block_size + fileinfo->start_block;
        blk_off =  rd_point % efs->block_size;

        buf_off = blk_off % CN_FILE_BUF_LEN;    //用块偏移计算出在读出来的256字节中的偏移
        if(buf_off)
            blk_off -= buf_off;     //一次读取256字节，这里减去buf_off是为了对齐

        if(rd_point >= fileinfo->filesize)
        {
            memcpy(rDataBuf,fp->wr_buf, CN_FILE_BUF_LEN);    //文件读取位置比文件的实际大小大，则从写缓存里读数据
            readlen = CN_FILE_BUF_LEN;
        }
        else
            readlen = efs->drv->efs_read_media(block, blk_off, rDataBuf, CN_FILE_BUF_LEN,EF_WR_ECC);    //从flash中读数据

        copy_len = CN_FILE_BUF_LEN - buf_off;   //计算可以在本次读出来的256字节中获取多少有效数据
        copy_len = (copy_len > len) ? len : copy_len;   //如果可以获取的数据量大于需要读取的数据量，则把copy_len赋值为需要读取的数据量
        if((buf_off + copy_len) > readlen)
        {   //本次想读的数据大于本次从flash里读到的数据
            if(buf_off >= readlen)    //获从缓存中复制数据的长度
                copy_len = len = 0; //buf偏移都大于本次本次从flash里读到的数据，那么把copy_len和len全清零
            else
                copy_len = len = readlen - buf_off; //从flash里读了多少有效数据就拿多少有效数据
        }
        memcpy(buf + completed,&rDataBuf[buf_off],copy_len);

        completed += copy_len;
        rd_point  += copy_len;
        len       -= copy_len;
    }

    fp->ptr += completed;
    rdsz = completed;
    Lock_MutexPost(efs->block_buf_mutex);
    return rdsz;
}

// =============================================================================
// 功能：写文件，如果超过文件的最大尺寸限制，则返回错误
// 参数：hdl,文件句柄
//      buf,待写入的数据缓冲区
//      len,写入数据的字节数
// 返回：FSRESULT，正确返回是FSR_OK
// =============================================================================
static u32 Efs_Write (struct objhandle *hdl, u8 *buf, u32 len)
{
    struct EfsFileInfo *fileinfo = NULL;
    struct EasyFS *efs = NULL;
    struct FileRsc *fp = NULL;
    struct Object *ob = hdl->HostObj;
    int ret = 1;
    u32 write_sum, write_len, completed = 0, rdsz = 0, block, alignsize;
    u64 wr_point, offset_block;
    //文件所在flash芯片指针
    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *) fp->private;
    if ((fp == NULL) || (len == 0) || (buf == NULL) || (efs == 0) || (fileinfo == NULL))
        return 0;

    if(fp->ptr < fp->file_size)
        return 0;       //不支持改写，当文件的当前位置，小于文件大小时则失败

    //如果超出了最大尺寸，则超出部分会被舍弃
    if (len <= (fileinfo->max_size - fp->ptr))
        write_sum = len;
    else
        write_sum = fileinfo->max_size - fp->ptr;

    if((s32)write_sum <= 0)
        return 0;       //需要写的数据长度小于等0则直接返回0
    wr_point = fp->ptr;

    //计算起始写位置处于文件内第几块和偏移量
    block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
    offset_block = fileinfo->filesize % efs->block_size;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return 0;

    //判断是否改写
    if(!efs->drv->efs_check_block_ready(block,offset_block,(u8*)buf,fp->buf_off + len))
    {
        Lock_MutexPost(efs->block_buf_mutex);
        return 0;
    }

    memset(fp->wr_buf + fp->buf_off, 0xFF, (CN_FILE_BUF_LIMIT - fp->buf_off));
    if( fileinfo->filesize % CN_FILE_BUF_LEN )//需先读出来,只有close,flush后才有
    {
        fp->buf_off = fileinfo->filesize % CN_FILE_BUF_LEN;
        efs->drv->efs_read_media(block, (offset_block & (~0xFF)), fp->wr_buf, fp->buf_off, EF_WR_NOECC); // 回读数据，为了后面256写入时的ECC校验
    }

    alignsize = fileinfo->filesize - fileinfo->filesize % CN_FILE_BUF_LEN;  //获取文件大小减去了不满256字节长度的大小
    //文件当前位置加需要写入的数据大小大于等于alignsize + CN_FILE_BUF_LEN的话，就先更新文件大小。（意味着本次写操作至少有一次会往flash里写数据）
    if(wr_point + write_sum >= alignsize + CN_FILE_BUF_LEN )
    {
        write_len = wr_point + write_sum;
        write_len = write_len - (write_len % CN_FILE_BUF_LEN);      //不满256字节的数据，在文件close时才写入flash
        fileinfo->filesize = write_len;
        __Efs_ChangeFileSize(ob, write_len);    //将filesize写入flash
    }

    while(1)
    {
        fp->buf_off = wr_point % CN_FILE_BUF_LEN;   //获取文件写缓冲里的数据长度
        write_len = CN_FILE_BUF_LEN - fp->buf_off;  //计算文件缓冲里还能写多少数据

        if(write_sum < write_len)
        {   //文件缓冲里能写的数据大于本次需要写的数据量
            write_len = write_sum;
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);   //先写入写缓存
            fp->buf_off += write_len;
            break;
        }
        else//write the buf to flash
        {    //把写缓存的数据写入flash
            memcpy(fp->wr_buf + fp->buf_off,buf+completed,write_len);
            block = (wr_point / efs->block_size) + fileinfo->start_block;
            offset_block = (wr_point % efs->block_size) - fp->buf_off;
            if(CN_FILE_BUF_LEN != efs->drv->efs_write_media(block,offset_block,
                    fp->wr_buf,CN_FILE_BUF_LEN,EF_WR_ECC))
            {
                write_len = 0;
                ret = 0;
                break;
            }
            fp->buf_off = 0;
            memset(fp->wr_buf, 0xFF, CN_FILE_BUF_LIMIT); // 写缓存清除，防止seek操作后会填入一些不可知的值；
        }
        wr_point += write_len;
        write_sum -= write_len;
        completed += write_len;
    }

    fp->ptr += completed + write_len;
    fp->file_size = fp->ptr;
    rdsz = completed + write_len;

    if(ret)
    {
        if(fileinfo->filesize != (u64)(fp->ptr - fp->ptr % CN_FILE_BUF_LEN))
        {
            fileinfo->filesize = fp->ptr - fp->ptr % CN_FILE_BUF_LEN;
            __Efs_ChangeFileSize(ob, fileinfo->filesize);    //写成功更新文件大小
        }
    }

    Lock_MutexPost(efs->block_buf_mutex);
    return rdsz;
}

// =============================================================================
// 功能：关闭文件,主要工作是释放文件指针的空间和打开文件空间
// 参数：hdl,文件句柄
// 返回：成功（0）；失败（-1）；
// =============================================================================
static s8 Efs_Close (struct objhandle *hdl)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block;
    u64 offset;
    struct FileRsc *fp;
    struct Object *ob = hdl->HostObj;
    s8 ret = 0;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((fp == NULL) || (efs == 0) || (fileinfo == NULL))
    {
        printk("close file param error\r\n");
        return -1;
    }
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
    {
        printk("close file timeout\r\n");
       return -1;
    }

    if(!Handle_FlagIsDirectory(hdl->flags)) // 非目录逻辑
    {
        //若wr_buf有数据，则先将其写入flash，不带ECC
        if(fp->buf_off)
        {
            block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
            offset = (fileinfo->filesize % efs->block_size) & (~0xFF); // 256对齐，fp->buf_off可能存在回读数据，而filesize不一定是256对齐的

            if(!efs->drv->efs_check_block_ready(block, offset, fp->wr_buf, fp->buf_off))
            {
                ret = -1;
                goto exit;
            }

            __Efs_ChangeFileSize(ob, fp->file_size);
            //此处写入大小没有256，写入EF_WR_ECC只是为了底层更好做一点
            if(fp->buf_off !=
                    efs->drv->efs_write_media(block,offset,fp->wr_buf,fp->buf_off,EF_WR_ECC))
            {
                __Efs_ChangeFileSize(ob, fileinfo->filesize);   //写入失败，则更正filesize
                ret = -1;
                goto exit;
            }
        }

        free(fileinfo);
        free(fp->wr_buf);
        free(fp);//todo ---- 不应该在此处释放，在efs/port.c里面释放，因为在那里malloc
        tgOpenedSum --;
    }
//  Handle_Delete(hdl);    //是目录的话啥也不干直接删除句柄
exit:
    Lock_MutexPost(efs->block_buf_mutex);
    return (ret);
}
// =============================================================================
// 功能：seek函数，两个功能，在EFS中，对于刚新建的文件，该函数实现配置最大尺寸，另外的情况
//      该函数实现移动读写指针（不能实现动态扩大的功能）
// 参数：hdl,文件句柄；file_offset,偏移多少位置；whence，从哪里开始偏移
// 返回：FSRESULT，正确返回是FSR_OK
// =============================================================================
static off_t Efs_Seek(struct objhandle *hdl, off_t *file_offset, s32 whence)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    u32 block, offset, newpos = 0, fsizeoff, alignsize;
    struct FileRsc *fp;
    struct Object *ob = hdl->HostObj;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if ((fp == NULL) || (efs == 0) || (fileinfo == NULL))
        return -1;

    switch(whence)
    {
        case SEEK_SET: newpos = (*file_offset);break;    //从文件开始出偏移
        case SEEK_CUR: newpos = (*file_offset + fp->ptr);break;     //从文件当前位置开始偏移
        case SEEK_END: newpos = (*file_offset + fp->file_size); break;     //从文件结束位置开始偏移
        default: return (-1);// 参数错误
    }

    if(newpos > fileinfo->max_size)   //文件位置超过文件大小则失败
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
        return -1;

    fsizeoff = fileinfo->filesize % CN_FILE_BUF_LEN;
    alignsize = fileinfo->filesize - fsizeoff;
    //向后seek > CN_FILE_BUF_LEN,则将wr_buf写下去
    if( (newpos - alignsize >= CN_FILE_BUF_LEN) && (newpos > alignsize) )
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;
        if(!efs->drv->efs_check_block_ready(block,offset,fp->wr_buf,CN_FILE_BUF_LEN))
        {
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }

        if(fsizeoff)        //判断是否需要读出来再write
        {
            offset = alignsize % efs->block_size;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
            efs->drv->efs_read_media(block ,offset, fp->wr_buf, fsizeoff, EF_WR_NOECC);
        }

        __Efs_ChangeFileSize(ob, newpos - newpos % CN_FILE_BUF_LEN);//修改文件大小

        if(CN_FILE_BUF_LEN != efs->drv->efs_write_media(block, alignsize, fp->wr_buf,
                CN_FILE_BUF_LEN,EF_WR_ECC))
        {
            __Efs_ChangeFileSize(ob, fileinfo->filesize);    //如果写入失败则把文件大小恢复
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }
        else
        {
            fileinfo->filesize = newpos - newpos % CN_FILE_BUF_LEN;    //文件实际大小
            fp->file_size = newpos;     //文件含写缓冲里数据的大小
            fp->buf_off = 0;
            memset(fp->wr_buf,0xFF,CN_FILE_BUF_LEN);
        }
    }

    fp->ptr = newpos;
    Lock_MutexPost(efs->block_buf_mutex);
    return newpos;
}

// =============================================================================
// 功能：删除文件
// 参数：ob,文件对象地址； uncached,路径中未匹配的部分
// 返回：成功  -- 0； 失败 -- -1
// =============================================================================
static s32 Efs_Remove(struct Object *ob, char *uncached)
{
    struct EasyFS *efs;
    u64 index_offset=0,ram_offset;
    u8 cfg_blocks,loop;
    u8 *file_info_buf;
    char *fname;
    if(ob == NULL)
        return -1;

    efs = (struct EasyFS*)File_Core(ob);
    if(NULL == efs)
    {
        printf("cannot fine efs root");
        return -1;
    }
    if(uncached)
        fname = uncached;
    else
        fname = ob->name;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    cfg_blocks = (FileInfoList+ efs->block_size-1) / efs->block_size;      //文件分配表所占的块数
    for (loop = 1; loop < IndexesNum; loop++)
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);

        file_info_buf[index_offset + FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        if (strncmp((const char*)file_info_buf, fname, FILENAME_LIMIT) == 0)
        {
            break;//找到该文件
        }
    }

    if(loop == IndexesNum)
    {
        printf("cannot find :%s\r\n", fname);
        free(file_info_buf);
        Lock_MutexPost(efs->block_buf_mutex);
        return -1;
    }

    if(loop < IndexesNum)
    {
        for (loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
        {
            ram_offset = loop * Ram_file_info_len;
            efs->file_list_buf[ram_offset + FILENAME_LIMIT] = '\0';
            ChkOrRecNameByECC((char *)(efs->file_list_buf + ram_offset), efs->file_list_buf + ram_offset + FILENAME_LIMIT + 1);
            if (strncmp((const char*)efs->file_list_buf + ram_offset, fname, FILENAME_LIMIT) == 0)
                memset(efs->file_list_buf + ram_offset, 0x0, Ram_file_info_len);
        }

        //删除的文件信息，写全0
        memset(file_info_buf, 0x0, EFS_ITEM_LIMIT);
        efs->drv->efs_write_media(efs->start_block, index_offset, file_info_buf,
                EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->drv->efs_write_media(efs->start_block+cfg_blocks, index_offset,
                file_info_buf,EFS_ITEM_LIMIT,EF_WR_NOECC);
        efs->files_sum --;
#if 1
{       //判断文件索引在主/备是否都写为了0
        u16 CountTemp;
        efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(file_info_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                break;
            }
        }
        efs->drv->efs_read_media(efs->start_block+cfg_blocks, index_offset, file_info_buf,
                EFS_ITEM_LIMIT, EF_WR_NOECC);
        for(CountTemp = 0; CountTemp < EFS_ITEM_LIMIT; CountTemp++)
        {
            if(file_info_buf[CountTemp] != 0x0)
            {
                printf("\r\nremove error in \"%s\"\r\n", fname);
                break;
            }
        }
}
#endif
    }
    free(file_info_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}

// =============================================================================
// 功能：获取文件状态
// 参数：ob,文件对象地址；data，存文件状态的结构；uncached,路径中未匹配的部分
// 返回：成功  -- 0；失败  -- -1
// =============================================================================
static s32 Efs_Stat(struct Object *ob, struct stat *data, char *uncached)
{
    struct EasyFS *efs;
    struct FileRsc* fp;
    u32 loop;
    u64 index_offset;
    u8 found = false;
    char *fname;
    u8 *hsize;
    u8 *file_info_buf;
    if(ob == NULL)
        return -1;

    if((uncached == NULL) || ((*uncached == '\0') ))
    {
        if(File_ObjIsMount(ob))
        {
            data->st_size = 0; // 安装点；
            data->st_mode = S_IFDIR;
        }
        else
        {
            fp = (struct FileRsc *)OBJ_GetPrivate(ob);      //获取以打开文件的状态
            data->st_size = fp->file_size;
            data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;
        }
        return 0;
    }

    efs = (struct EasyFS*)File_Core(ob);
//    if(uncached)
        fname = uncached;
//    else
//        fname = ob->name;

    if(NULL == efs)
        return -1;

    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    file_info_buf = malloc(EFS_ITEM_LIMIT);
    if(file_info_buf == NULL)
    {
        warning_printf("efs", "memory request failed \r\n");
        return -1;
    }
    //判断是否存在该文件
    for (loop = 1; loop < IndexesNum; loop++)
    {
        index_offset = loop * EFS_ITEM_LIMIT;
        if(!efs->drv->efs_read_media(efs->start_block, index_offset, file_info_buf, EFS_ITEM_LIMIT, EF_WR_NOECC))
            memset(file_info_buf, 0xFF, EFS_ITEM_LIMIT);
        file_info_buf[FILENAME_LIMIT] = '\0';
        ChkOrRecNameByECC((char *)file_info_buf, file_info_buf + FILENAME_LIMIT + 1);
        if (strncmp((const char*)file_info_buf ,fname,FILENAME_LIMIT) == 0)
        {
            found = true;       //找到目标文件
            break;
        }
    }

    if(found)
    {
        for (loop = 0; loop < FILE_FILESIZE_NUM; loop++)
        {
            hsize = (file_info_buf + FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER);
            if ((pick_little_32bit(hsize,0) == CN_LIMIT_UINT32) ||
                (pick_little_32bit(hsize,0) == 0))
                continue;
            if (__EfsFileSize_verify(*(u32 *)hsize) == 0)
            {   //获取目标文件的大小
                data->st_size = (pick_little_32bit(file_info_buf,     //从44个文件大小中，找正确的文件大小，从最后开始找
                        (FILE_FILESIZE_END - loop * EFS_FILESIZE_BYTE_NUMBER) >> 2) & EFS_SUPPORT_MAXFILESIZE);
                break;
            }
        }
        data->st_mode = S_IFREG|S_IRUGO|S_IWUGO;    //efs里只有文件没有目录，所有里面找到的文件，属性全都赋值为文件
        free(file_info_buf);
        Lock_MutexPost(efs->block_buf_mutex);
        return 0;
    }
    free(file_info_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return -1;
}

// =============================================================================
// 功能：将缓存的数据写入flash
// 参数：hdl,文件句柄
// 返回：成功 -- 0；失败 -- -1
// =============================================================================
static s32 Efs_Sync (struct objhandle *hdl)
{
    struct EfsFileInfo *fileinfo;
    struct EasyFS *efs;
    struct FileRsc *fp;
    u32 block;
    u64 offset;
    struct Object *ob = hdl->HostObj;

    fp = (struct FileRsc *)OBJ_GetPrivate(ob);
    efs = (struct EasyFS *)File_Core(ob);
    fileinfo = (struct EfsFileInfo *)fp->private;
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    //若wr_buf有数据，则先将其写入flash，不带ECC
    if(fp->buf_off)
    {
        block = (fileinfo->filesize / efs->block_size) + fileinfo->start_block;
        offset = fileinfo->filesize % efs->block_size;
        if(!efs->drv->efs_check_block_ready(block, offset, fp->wr_buf, fp->buf_off))
        {
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }

        __Efs_ChangeFileSize(ob, fp->file_size);        //修改文件大小（加上写缓存里的数据）
        //此处写入大小没有256，写入EF_WR_ECC只是为了底层更好做一点
        if(fp->buf_off !=
                efs->drv->efs_write_media(block, offset, fp->wr_buf, fp->buf_off, EF_WR_ECC))
        {
            __Efs_ChangeFileSize(ob, fileinfo->filesize);   //写入失败，则更正filesize
            Lock_MutexPost(efs->block_buf_mutex);
            return -1;
        }
        else
        {
            fileinfo->filesize = fp->ptr;
            fp->buf_off = 0;
        }
    }
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}
// ============================================================================
// 功能：初始化efs对media的驱动
// 参数：core -- efs文件系统管理；  drv -- media的操作函数集
// 返回：0 -- 成功；  -1 --失败
// 备注：
// ============================================================================
int efs_install_drv(struct EasyFS *core, struct __efs_media_drv *drv)
{
    if (!core || !drv)
         return (-1);
    core->drv = malloc(sizeof(*drv));
    core->drv->efs_erase_media = drv->efs_erase_media;
    core->drv->efs_read_media = drv->efs_read_media;
    core->drv->efs_write_media = drv->efs_write_media;
    core->drv->efs_check_block_ready = drv->efs_check_block_ready;
    return (0);
}
// ============================================================================
// 功能：初始化efs的信息
// 参数：core -- efs文件系统管理；  pSuper -- 文件系统核心数据
// 返回：其他 -- 成功； NULL --失败
// 备注：
// ============================================================================
struct EasyFS *EfsInfo(struct FsCore *pSuper, u32 opts)
{
    struct umedia *media;
    struct EasyFS *core;
    u32 flash_block_size,flash_page_size,blocks_per_page,filedatablocks,allblocknum,num,filelistblock;

    if(pSuper == NULL)
        return NULL;

    media = (struct umedia*)pSuper->MediaInfo;              //获取flash的信息
    media->mreq(blockunits,(ptu32_t)&blocks_per_page);      //获取flash中一块有多少页
    media->mreq(unitbytes,(ptu32_t)&flash_page_size);       //获取flash的页大小
    flash_block_size = blocks_per_page * flash_page_size;   //获取flash的块大小
    allblocknum = pSuper->AreaSize / flash_block_size;      //总块数
    // EFS单个文件实际数据所需要的块数
    filedatablocks = (CFG_EFS_FILE_SIZE_LIMIT + flash_block_size - 1) / flash_block_size;
    num = allblocknum / filedatablocks-1;     //预计可以创建的最大文件数
    //计算文件列表所需要的块数
    filelistblock = (((num + 1) * EFS_ITEM_LIMIT) + flash_block_size - 1) / flash_block_size;
    //计算实际能创建的最大文件数
    while(((num * filedatablocks) + (filelistblock * 2)) > allblocknum)
    {
        num--;
        filelistblock = (((num + 1) * EFS_ITEM_LIMIT) + flash_block_size - 1) / flash_block_size;
    }
    if(num > CFG_EFS_MAX_CREATE_FILE_NUM)
        num = CFG_EFS_MAX_CREATE_FILE_NUM;
    CreateMax = num;
    IndexesNum = num + 1;
    FileInfoList = IndexesNum * EFS_ITEM_LIMIT;
    if((CreateMax == 0) || (IndexesNum == 0) || (EfsCgfLimit == 0))
    {
        error_printf("efs","file system core parameters calculated incorrectly.\r\n");
        return NULL;
    }
    notice_printf("efs","create max file number %d .\r\n",num);

    core = malloc(sizeof(*core) + EfsCgfLimit);
    if(!core)
    {
        printf("\r\n: erro : efs    : install failed(memory out).");
        return NULL;
    }
    memset(core, 0x0, sizeof(*core) + EfsCgfLimit);
    core->block_buf_mutex = Lock_MutexCreate("EFS_BLOCK_BUF_SEMP");
    if(!core->block_buf_mutex)
    {
        free(core);
        printf("\r\n: erro : efs    : install failed(cannot create lock).");
        return NULL;
    }
    core->install_options = opts;
    pSuper->pCore = (void*)core;
    core->media = media;
    efs_install_drv(core,pSuper->MediaDrv);
    core->file_list_buf = (u8*)core + sizeof(*core);
    core->block_size = flash_block_size;
    core->block_sum = allblocknum;
    core->files_sum = 0;
    memcpy(core->name, pSuper->pTarget->name, EFS_NAME_LIMIT+1);
    core->start_block = pSuper->MediaStart / blocks_per_page;

    return core;
}
// =============================================================================
// 功能：创建文件系统，在mount不成功时，会调用，调用后原有文件信息将不存在
// 参数：efs,文件系统指针
//      pSuper -- 文件系统核心数据
// 返回：0 -- 成功； -1 --失败
// =============================================================================
static s32 Efs_Mkfs(struct EasyFS* efs,struct FsCore *pSuper)
{
    u32 loop, blocks, cfg_end_block;
    u8 *block_buf;
    u16 crc16_check,temp;

    if(!efs->block_sum) // 说明该chip未初始化
    {
        error_printf("efs","file system was not initialized.\r\n");
        return -1;
    }

    blocks = (FileInfoList + efs->block_size-1) / efs->block_size;
    cfg_end_block = efs->start_block + blocks;
//    block_buf = efs->file_list_buf;
    if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
       return -1;

    block_buf = M_MallocLc(FileInfoList, 0);
    if(!block_buf)
    {
        Lock_MutexPost(efs->block_buf_mutex);
        return -1;
    }

    //写好efs文件系统的核心信息到flash里
    memset(block_buf, 0xff, FileInfoList);
    memcpy(block_buf, "easyfile", 8);
    memcpy(block_buf + FILE_VERIFITY_OFF, "easyfile", 8);
    fill_little_32bit(block_buf, 2, efs->block_sum);
    fill_little_32bit(block_buf, 3, efs->block_size);

    crc16_check = CRC_16(block_buf,16);
    fill_little_16bit((u8*)&temp,0,crc16_check); // 转化为小端
    for (loop = efs->start_block; loop < cfg_end_block; loop++)
    {
        efs->drv->efs_erase_media(loop);       //efs用到的整个区域擦一遍
        efs->drv->efs_erase_media(loop + blocks);       //efs用到的整个区域擦一遍
    }
    efs->drv->efs_write_media(efs->start_block, 0, block_buf, FileInfoList ,EF_WR_NOECC);
    efs->drv->efs_write_media(cfg_end_block, 0, block_buf, FileInfoList ,EF_WR_NOECC);
    //added,将最后的CRC写到文件系统信息的最后两个字节
    efs->drv->efs_write_media(efs->start_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);
    efs->drv->efs_write_media(cfg_end_block + blocks - 1,
            efs->block_size-2, (u8*)&temp,2,EF_WR_NOECC);

    efs->files_sum = 0;

    if(pSuper)
    {
        //每次新建都将fs存放到全局变量 s_pEfsList，用于后面操作
        s_pEfsList = efs;
        memcpy(s_pEfsList->name,pSuper->pTarget->name,EFS_NAME_LIMIT+1);
    }
    efs->file_sys_install = true;
    free(block_buf);
    Lock_MutexPost(efs->block_buf_mutex);
    return 0;
}

// ============================================================================
// 功能：文件系统接口；
// 参数：
// 返回：根据各命令要求
// 备注：
// ============================================================================
s32 e_operations(void *opsTarget, u32 objcmd, ptu32_t OpsArgs1,
                 ptu32_t OpsArgs2, ptu32_t OpsArgs3)
{
    s32 result = CN_OBJ_CMD_EXECUTED;

    switch(objcmd)
    {
        case CN_OBJ_CMD_OPEN:
        {
            struct objhandle *hdl;
            hdl = Efs_Open((struct Object *)opsTarget, (u32)(*(u64*)OpsArgs2), (char*)OpsArgs3);
            *(struct objhandle **)OpsArgs1 = hdl;
            break;
        }

      case CN_OBJ_CMD_READDIR:
      {
          struct objhandle *hdl = (struct objhandle*)OpsArgs3;
          struct dirent *ret = (struct dirent *)OpsArgs1;

          if((ptu32_t)Efs_DirRead(hdl, ret) == 0)
              result = CN_OBJ_CMD_TRUE;
          else
              result = CN_OBJ_CMD_FALSE;
          break;
      }

        case CN_OBJ_CMD_READ:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = Efs_Read(devfile, (u8*)OpsArgs2, (u32)OpsArgs3);
            *(u32 *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_WRITE:
        {
            u32 len;
            struct objhandle *devfile = (struct objhandle*)opsTarget;

            len = Efs_Write(devfile, (u8*)OpsArgs2, (u32)OpsArgs3);
            *(u32 *)OpsArgs1 = len;
            break;
        }

        case CN_OBJ_CMD_CLOSE:
        {
            if(Efs_Close((struct objhandle*)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SEEK:
        {
            *(off_t*)OpsArgs1 = Efs_Seek((struct objhandle *)opsTarget, (off_t*)OpsArgs2, (s32)OpsArgs3);
            break;
        }

        case CN_OBJ_CMD_DELETE:
        {
            if(Efs_Remove((struct Object*)opsTarget, (char *)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_STAT:
        {
            if(Efs_Stat((struct Object*)opsTarget, (struct stat *)OpsArgs1, (char*)OpsArgs3) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        case CN_OBJ_CMD_SYNC:
        {
            if(Efs_Sync((struct objhandle *)opsTarget) == 0)
                result = CN_OBJ_CMD_TRUE;
            else
                result = CN_OBJ_CMD_FALSE;
            break;
        }

        default:
        {
            result = CN_OBJ_CMD_UNSUPPORT;
            break;
        }
    }

    return result;
}

// ============================================================================
// 功能：格式化efs文件系统
// 参数：core -- 文件 系统核心数据
// 返回：0 -- 成功; -1 -- 失败;
// 备注：
// ============================================================================
static s32 Efs_Format(void *core)
{
    if(core == NULL)
        return -1;

    struct EasyFS *efs = (struct EasyFS *)core;
    return Efs_Mkfs(efs,NULL);
}
// ============================================================================
// 功能：校验或安装文件系统；
// 参数：core -- 文件系统的核心数据结构
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
static s32 Efs_Verify_Install(struct FsCore *pSuper)
{
    u32 loop, blocks, end_block, fileInfoSize;
    u8 *bakbuf, *block_buf;
    u8 mainblockerr = 0,bakblockerr = 0;
    u16 crc16_check,temp;
    u64 index_offset;

    if(pSuper == NULL)
        return -1;

    struct EasyFS *efs = (struct EasyFS *)pSuper->pCore;

    if(efs->install_options & MS_INSTALLFORMAT)
    {
        if(Efs_Mkfs(efs,pSuper) == 0)    //安装时会格式化整个文件系统
            return 0;
        else
        {
            error_printf("efs","format fail.\r\n");
            free(efs);
            return -1;
        }
    }

    bakbuf = M_MallocLc(FileInfoList, 0);//只需文件信息大小
    if(bakbuf == NULL)
    {
        free(efs);
        return -1;
    }
    block_buf = M_MallocLc(FileInfoList, 0);
    if(block_buf == NULL)
    {
        free(bakbuf);
        free(efs);
        return -1;
    }
    blocks = (FileInfoList + efs->block_size-1) / efs->block_size; // EFS文件分配表所需要使用的块数
    end_block = efs->start_block + blocks; //
    fileInfoSize = FileInfoList;
//    block_buf = efs->file_list_buf;

   if(false == Lock_MutexPend(efs->block_buf_mutex,MUTEX_WAIT_TIME))
   {
       free(bakbuf);
       free(efs);
       return -1;
   }

    //检查文件系统是否已经建立
    if(!efs->drv->efs_read_media(efs->start_block, 0, block_buf, fileInfoSize,EF_WR_NOECC))
        goto fail;
    if(!efs->drv->efs_read_media(end_block, 0, bakbuf, fileInfoSize,EF_WR_NOECC))
        goto fail;
    //主备区都被破坏，则需要重新建立文件系统
    if ((memcmp(bakbuf, "easyfile", 8) ||
            memcmp(bakbuf + FILE_VERIFITY_OFF, "easyfile", 8)) &&
        (memcmp(block_buf, "easyfile", 8)||
            memcmp(block_buf + FILE_VERIFITY_OFF, "easyfile", 8)) )
    {
        printf("文件系统未建立，创建之!\r\n");
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
    }

    //检查文件系统信息区最后两字节CRC是否正确
    if(!efs->drv->efs_read_media(efs->start_block + blocks - 1, efs->block_size-2,
            block_buf+fileInfoSize-2, 2,EF_WR_NOECC))
        goto fail;
    if(!efs->drv->efs_read_media(end_block + blocks - 1, efs->block_size-2,
            bakbuf+fileInfoSize-2, 2,EF_WR_NOECC))
        goto fail;

    crc16_check = pick_little_16bit(block_buf,(fileInfoSize/2)-1);
    if(crc16_check != CRC_16(block_buf,16))
        mainblockerr = 1;

    crc16_check = pick_little_16bit(bakbuf,(fileInfoSize/2)-1);
    if(crc16_check != CRC_16(bakbuf,16)) //bak校验错
        bakblockerr = 1;

    //以下检查数据正确性
    loop = efs->start_block;

    switch((mainblockerr << 1) + (bakblockerr << 0))
    {
    case 0x03:      //两个缓冲区校验均出错，表明文件系统已经损坏，报错
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
        break;
    case 0x02:      //mainbuf error, bak校验正确
        temp = __Efs_CheckSingleBuf(bakbuf, loop, efs);
        if (temp == 0)
        {
            crc16_check = CRC_16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //擦主区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(efs->start_block, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else if (temp == 1)
        {
            crc16_check = CRC_16(bakbuf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //擦主区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(efs->start_block, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //擦备份区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(loop, 0, bakbuf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else
        {
            Lock_MutexPost(efs->block_buf_mutex);
            goto exit;
        }
        break;
    case 0x01:          //主校验正确, bakbuf error
        temp = __Efs_CheckSingleBuf(block_buf, loop,efs);
        if (temp == 0)
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //擦备份区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else if (temp == 1)
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //擦主区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(efs->start_block, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //擦备份区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        else
        {
            Lock_MutexPost(efs->block_buf_mutex);
            goto exit;
        }
        break;
    case 0x00:          //主/备区域全对
        if(1 == __Efs_CheckBlock(block_buf,loop, bakbuf, loop+blocks, efs) )    //主备区域不一致
        {
            crc16_check = CRC_16(block_buf,16);
            fill_little_16bit((u8*)&temp,0,crc16_check);    //转化为小端
            //分配表信息错或filesize满，则写入flash
            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop);       //擦主区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(efs->start_block, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;

            for (loop = efs->start_block; loop < end_block; loop++)
                    efs->drv->efs_erase_media(loop + blocks);       //擦备份区，然后重新写入所有文件索引;
            if(!efs->drv->efs_write_media(loop, 0, block_buf, fileInfoSize, EF_WR_NOECC))
                goto fail;
            if(!efs->drv->efs_write_media(loop + blocks - 1, efs->block_size-2, (u8*)&temp, 2, EF_WR_NOECC))
                goto fail;
        }
        break;
    default:
        Lock_MutexPost(efs->block_buf_mutex);
        goto exit;
        break;
    }
    for(loop = 0; loop < CFG_EFS_MAX_OPEN_FILE_NUM; loop++)
    {
        index_offset = loop * Ram_file_info_len;
        if(!efs->drv->efs_read_media(efs->start_block, loop*EFS_ITEM_LIMIT, efs->file_list_buf + index_offset, EFS_ITEM_LIMIT,EF_WR_NOECC))       //读文件分配表到缓存中
            goto fail;
        fill_little_32bit(efs->file_list_buf + index_offset + EFS_ITEM_LIMIT, 0, loop + 0);
    }
    s_pEfsList = efs;
    memcpy(s_pEfsList->name,pSuper->pTarget->name,EFS_NAME_LIMIT+1);
    Lock_MutexPost(efs->block_buf_mutex);
    free(bakbuf);
    free(block_buf);
//    pSuper->pCore = (void*)efs;
    efs->file_sys_install = true;
    return 0;
exit:
    free(bakbuf);
    free(block_buf);
    if(efs->install_options & MS_INSTALLCREAT)
    {
        if(Efs_Mkfs(efs,pSuper) == 0)//设备上不存在文件系统，则新建
            return 0;
    }
    free(efs);
    return -1;
fail:
    free(bakbuf);
    free(block_buf);
    free(efs);
    Lock_MutexPost(efs->block_buf_mutex);
    return -1;
}
// ============================================================================
// 功能：安装文件系统；
// 参数：super -- 文件系统结构；
//      opts -- 安装方式；
//      config -- 文件系统配置；
// 返回：成功（0）；失败（-1）；
// 备注：
// ============================================================================
s32 e_install(struct FsCore *pSuper, u32 opts, void *config)
{
    struct EasyFS *core;

    dwFileMaxSize = CFG_EFS_FILE_SIZE_LIMIT; // 文件大小上限
    if(dwFileMaxSize > EFS_SUPPORT_MAXFILESIZE)
    {
        dwFileMaxSize = EFS_SUPPORT_MAXFILESIZE;
        warning_printf("efs","The file size exceeds the maximum file size supported by efs "
                             "and now forces the maximum file size to be 0x3fffffff");
    }
    Ram_file_info_len = EFS_RAM_ITEM_BYTE + EFS_ITEM_LIMIT;
    EfsCgfLimit = Ram_file_info_len * CFG_EFS_MAX_OPEN_FILE_NUM;  //efs,在缓存中存文件信息的大小
    core = EfsInfo(pSuper, opts);
    if(core == NULL)
    {
        free(core);
        return -1;
    }

    if(opts & MS_INSTALLUSE)
        return 0;
    else
        return Efs_Verify_Install(pSuper);
}
// ============================================================================
// 功能：安装EASY文件系统
// 参数：target -- 安装目录；
//      opt -- 安装选项；
//      config -- 特殊逻辑（暂时未使用）；
// 返回：失败(-1)； 成功(0)。
// 备注:
// ============================================================================
s32 ModuleInstall_EFS(const char *target, u32 opt, u32 config)
{
    struct Object * mountobj;
    static struct filesystem *typeEFS = NULL;
    s32 res;

    if(!target)
    {
        return (-1);
    }

    res = sizeof(*typeEFS);
    if(typeEFS == NULL)
    {
        typeEFS = malloc(res);

        typeEFS->fileOps = e_operations;
        typeEFS->install = e_install;
        typeEFS->pType = "EFS";
        typeEFS->format = Efs_Format;
        typeEFS->uninstall = NULL;
    }
    res = File_RegisterFs(typeEFS);
    if(-1==res)
    {
        printf("\r\n: dbug : module : cannot register \"EFS\"<file system type>.");
        return (-1); // 失败;
    }

    mountobj = OBJ_NewChild(OBJ_GetRoot(), __File_MountOps, 0, target); // 创建安装目录
    if(NULL == mountobj)
    {
        printf("\r\n: dbug : module : mount \"EFS\" failed, cannot create \"%s\"<group point>.", target);
        return (-1);
    }
//    OBJ_DutyUp(mountobj);
    opt |= MS_DIRECTMOUNT;
//  opt |= CFG_EFS_INSTALL_OPTION_APPEND;
    res = File_Mount(NULL, target, "EFS", opt, (void *)config);
    if(res == -1)
    {
        printf("\r\n: dbug : module : mount \"%s\" failed, cannot install.", "EFS");
        OBJ_Delete(mountobj);
        return (-1);
    }

    return (0);
}
