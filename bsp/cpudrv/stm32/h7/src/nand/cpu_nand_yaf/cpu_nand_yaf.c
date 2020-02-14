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
#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/flash/flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <math.h>
#include "stm32h7xx_hal_conf.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device/include/unit_media.h>
#include <board.h>
#include <fs/yaf2/yaffs2-583dbd9/yaffs2-583dbd9/yaffs_guts.h>

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern bool_t ModuleInstall_NandInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat);
//   ModuleInstall_NandInstallYaf(CFG_NFLASH_YAFFSMOUNT_NAME, CFG_NFLASH_YAF_PART_START,
//                                                  CFG_NFLASH_YAF_PART_END, CFG_NFLASH_YAF_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nand_install_yaf"//nand安装yaf
//parent:"cpu onchip nand"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"yaf2 file system","cpu onchip nand"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NAND_INSTALL_YAF == false )
//#warning  " nand_install_yaf  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NAND_INSTALL_YAF    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@string,1,10,
#define CFG_NFLASH_YAFFSMOUNT_NAME     "yaf2" //需安装的文件系统的mount的名字
//%$#@num,-1,2048
#define CFG_NFLASH_YAF_PART_START      0      //分区起始，填写块号，块号从0开始计算
#define CFG_NFLASH_YAF_PART_END        -1     //分区结束，-1表示最后一块
//%$#@enum,true,false,
#define CFG_NFLASH_YAF_PART_FORMAT     false      //分区选项,是否需要格式化该分区。
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure

extern struct NandDescr *__nandescription; // NAND器件描述;
extern struct umedia *nand_umedia;
extern bool_t Nandflash_is_install(void);
extern s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
int yaf_nand_write(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len);
int yaf_nand_read(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out);
int yaf_nand_eraseblock(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_markbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_checkbad(struct yaffs_dev *yaf2dev, int block_no);
int yaf_nand_initialize(struct yaffs_dev *dev);
int yaf_nand_deinitialize(struct yaffs_dev *dev);

struct yaffs_driver YAF_NAND_DRV =
{
    .drv_write_chunk_fn = yaf_nand_write,
    .drv_read_chunk_fn = yaf_nand_read,
    .drv_erase_fn = yaf_nand_eraseblock,
    .drv_mark_bad_fn = yaf_nand_markbad,
    .drv_check_bad_fn = yaf_nand_checkbad,
    .drv_initialise_fn = yaf_nand_initialize,
    .drv_deinitialise_fn = yaf_nand_deinitialize
};

// ============================================================================
// 功能：判断该区域是否擦除
// 参数：buf -- 读到该区域的数据；datalen -- 数据长度；taglen -- oob长度
// 返回：擦除 -- 1；未擦除 -- 0；
// 备注：
// ============================================================================
s32 yaf_nand_is_erased(const u8 *buf, s32 datalen, s32 taglen)
{
    s32 alllen = datalen + taglen;
    while (alllen--)
    {
        if (*buf != 0xff)
            return 0;
        buf++;
    }
    return 1;
}

// ============================================================================
// 功能：通过yaf文件系统往flash里写数据
// 参数：yaf2dev -- yaf的驱动结构； chunk -- flash的页号； data -- 待写数据的缓存； data_len -- 待写数据长度
//     oob -- ecc校验码； oob_len -- ecc校验码长度
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
int yaf_nand_write(struct yaffs_dev *yaf2dev, int chunk,
                        const u8 *data, int data_len,
                        const u8 *oob, int oob_len)
{
    struct uopt opt = {0};
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
   {
       printf("\r\n: erro : yaffs2 : bad driver logic when write");
       return (YAFFS_FAIL);
   }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif

    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = __nandescription->BytesPerPage;
    __nand_req(sparebytes, (ptu32_t)&spare);
    __nand_req(lock, -1);
    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        memset(nand_umedia->ubuf, 0xFF, main+spare);
        if(data)
        {
            opt.main = 1;
            memcpy(nand_umedia->ubuf, data, data_len);
            data += main;
        }

        if(oob)
        {
            opt.spare = 1;
            memcpy(nand_umedia->ubuf+main, oob, oob_len);
        }

        if(__nand_write((s64)chunk, (void*)nand_umedia->ubuf, opt) != 0)
        {
            res = YAFFS_FAIL;
            break;
        }
    }

    __nand_req(unlock, 0);
    return (res);
}

// ============================================================================
// 功能：通过yaf文件系统读flash里的数据
// 参数：yaf2dev -- yaf的驱动结构； chunk -- flash的页号； data -- 待读数据的缓存； data_len -- 待写数据长度
//     oob -- ecc校验码； oob_len -- ecc校验码长度
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：intag模式与oob是不会同时存在的，如果初始化时时intag模式，oob上不会存在内容；
// ============================================================================
int yaf_nand_read(struct yaffs_dev *yaf2dev, int chunk,
                   u8 *data, int data_len,
                   u8 *oob, int oob_len,
                   enum yaffs_ecc_result *ecc_result_out)
{
    struct uopt opt = {0};
    s32 i, spare = 0, res = YAFFS_OK, main;

    if(((s32)(yaf2dev->driver_context) > 1)&&(oob))
    {
        printf("\r\n: erro : yaffs2 : bad driver logic when read");
        *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;
        return (YAFFS_FAIL);
    }

#if 0// todo: yaffs调用时,存在data==NULL,data_len不为零的情况。
    if (((!data) && (data_len > 0)) || ((!oob) && (oob_len > 0)))
        return (YAFFS_FAIL);
#endif



    if(yaf2dev->param.use_nand_ecc)
    {
        opt.hecc = 1;
        opt.secc = 1;
    }

    main = __nandescription->BytesPerPage;;
    __nand_req(sparebytes, (ptu32_t)&spare);
    __nand_req(lock, -1);
    opt.main = 1;
    if(oob)
        opt.spare = 1;

    for(i = 0; i < (s32)(yaf2dev->driver_context); i++)
    {
        if(__nand_read((s64)chunk, (void*)nand_umedia->ubuf, opt) != 0)
        {
            if(!yaf_nand_is_erased(nand_umedia->ubuf, main, oob_len))
                *ecc_result_out = YAFFS_ECC_RESULT_UNFIXED;/* ECC校验失败(保证不是被擦除的块中的页) */
            else
                *ecc_result_out = YAFFS_ECC_RESULT_UNKNOWN;/* 读错误等,或者无错误（情景：ECC读一个刚被擦除的页） */

            printf("\r\n: erro : yaffs2 : bad when read");
            res = YAFFS_FAIL;
            break;
        }

        if(data)
        {
            memcpy(data, nand_umedia->ubuf, main);
            data += main;
        }

        if(oob)
            memcpy(oob, (nand_umedia->ubuf+main), oob_len);
    }

    __nand_req(unlock, 0);
    return (res);
}

// ============================================================================
// 功能：通过yaf文件系统擦除flash里某个块的数据
// 参数：yaf2dev -- yaf的驱动结构； block_no -- 需要擦除的块号
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：
// ============================================================================
int yaf_nand_eraseblock(struct yaffs_dev *yaf2dev, int block_no)
{
    struct uesz sz = {0};

    sz.block = 1;
    if(__nand_erase(block_no, sz) != 0)
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);
}

// ============================================================================
// 功能：标记某块是坏块
// 参数：yaf2dev -- yaf的驱动结构； block_no -- 需要标记的块号
// 返回：成功 -- YAFFS_OK；失败 -- YAFFS_FAIL
// 备注：
// ============================================================================
int yaf_nand_markbad(struct yaffs_dev *yaf2dev, int block_no)
{
    __nand_req(markbad, block_no);
    return (YAFFS_OK);
}
// ============================================================================
// 功能：检查某块是否是坏块
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf_nand_checkbad(struct yaffs_dev *yaf2dev, int block_no)
{
    if(__nand_req(checkbad, block_no))
        return (YAFFS_FAIL);
    else
        return (YAFFS_OK);

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf_nand_initialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int yaf_nand_deinitialize(struct yaffs_dev *yaf2dev)
{
    return (YAFFS_OK);
}

//-----------------------------------------------------------------------------
// 功能：在nand安装yaf
// 参数：  TargetFs -- 要挂载的文件系统
//      parts -- 分区数；
//      TargetPart -- 指定要挂到哪个分区下，分区从0开始
//      分区数据 -- 起始块，结束块数（如果结束块是6，起始块是0，则该分区使用的块为0，1，2，3，4，5块，不包括第六块），是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：如果还不知道要安装什么文件系统，或者不安装文件系统TargetFs填NULL，TargetPart填-1；
//-----------------------------------------------------------------------------
bool_t ModuleInstall_NandInstallYaf(const char *TargetFs,s32 bstart, s32 bend, u32 doformat)
{
    struct FsCore *super;
    char *notfind;
    struct Object *targetobj;
    if(Nandflash_is_install() == true)
    {
        if((TargetFs != NULL) && (bstart != bend))
        {
            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __nand_req(format, bstart , bend, &sz))
                {
                    warning_printf("yaf2"," Format failure.");
                }
            }
            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("nand"," not found need to install file system.");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            if(strcmp(super->pFsType->pType, "YAF2") == 0)
            {
                if(__nand_FsInstallInit(TargetFs,bstart,bend,&YAF_NAND_DRV) == 0)
                    return true;
            }
            error_printf("nand"," need to install file system not YAF2.");
        }
    }
    return false;
}
