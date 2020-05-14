/*
 * w25qxx_fat.c
 *
 *  Created on: 2020年5月9日
 *      Author: CK
 */

#include <stdlib.h>
#include <device.h>
#include <fs/fat/port/drivers/fat_drivers.h>
#include <fs/fat/ff11/src/diskio.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <string.h>
#include "cpu_peri.h"
#include "../w25qxx.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_W25qxxInstallFat(const char *TargetFs, s32 bend, u32 doformat);
//    ModuleInstall_W25qxxInstallFat(CFG_W25_FAT_MOUNT_NAME,CFG_W25_FAT_PART_END,CFG_W25_FAT_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"W25QXX FAT"//SPI和QSPI接口的nor flash芯片
//parent:"norflash W25QXX"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"fat file system",//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"        //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NORFLASH_W25QXX_FAT == false )
//#warning  " norflash_W25QXX  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NORFLASH_W25QXX_FAT    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@string,1,10,
#define CFG_W25_FAT_MOUNT_NAME            "fat"      //"文件系统mount点名字",需要挂载的efs文件系统mount点名字
//%$#@num,-1,512,
#define CFG_W25_FAT_PART_END                   -1        //分区结束，-1表示最后一块,起始分区固定从0开始。如果不是-1的话，不会包括当前块。例如start=0,end=6,那使用的范围为0~5
//%$#@enum,true,false,
#define CFG_W25_FAT_PART_FORMAT               false      //分区选项,是否需要格式化该分区。
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


u32 sector_num = 0;
#define     SECTOR_SIZE     512         //文件系统配置的是512,这里对flash的操作也得按512来，不能按256来，（其实就是把页大小换成512）

int w25qxx_status(void);
int w25qxx_initialize(void);
int fat_w25qxx_read(BYTE *Buff, DWORD Sector, UINT Count);
int fat_w25qxx_write(BYTE *Buff, DWORD Sector, UINT Count);
int w25qxx_ioctl( BYTE Cmd, void *Buff);

extern struct Object *s_ptDeviceRoot;

struct FatDrvFuns W25QXX_Drv =
{
    .DrvStatus     = w25qxx_status,
    .DrvInitialize = w25qxx_initialize,
    .DrvRead       = fat_w25qxx_read,
    .DrvWrite      = fat_w25qxx_write,
    .DrvIoctl      = w25qxx_ioctl
};

// ============================================================================
// 功能: W25qxx状态
// 参数: 无
// 输出: 0 -- OK，1 -- ERROR
// 返回:
// ============================================================================
s32 w25qxx_status(void)
{
    s32 res = 1; //RES_ERROR;

    if(W25qxx_is_install())
    {
        res = 0; //RES_OK;
    }
    else
    {
        res = 1; //RES_ERROR;
    }

    return res;
}

// ============================================================================
// 功能: W25qxx初始化
// 参数:
// 返回:
// 备注: 可以将设备初始化放在这里;
// ============================================================================
s32 w25qxx_initialize(void)
{
    return (0);
}

// ============================================================================
// 功能: w25qxx读数据
// 参数: buff -- 读数据缓存。
//      sector -- 目标页号。
//      count -- 页数量。
// 返回: 0 -- 正确; 1 -- 错误; 3 -- 设备未准备好
// 备注:
// ============================================================================
s32 fat_w25qxx_read(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(W25QXX_Read(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(w25qxx_status())
            res = 3;
        else
            res = 0;
    }
    return res;
}

// ============================================================================
// 功能: 写数据
// 参数: buff -- 写入内容。
//      sector -- 目标页号。
//      count -- 页数量。
// 返回: 0 -- 正确; 1 -- 错误; 3 -- 设备未准备好
// 备注:
// ============================================================================
s32 fat_w25qxx_write(u8 *buff, DWORD sector, u32 count)
{
    s32 res = 1; // RES_ERROR;

    if(W25QXX_Write(buff, ((u32)sector) * SECTOR_SIZE, count * SECTOR_SIZE))
        res = 0;
    else
    {
        if(w25qxx_status())
            res = 3;
        else
            res = 0;
    }
    return res;

}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 w25qxx_ioctl( u8 cmd, void *buff)
{
    DRESULT res = 0; // RES_OK;

    switch (cmd) {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = 0; // RES_OK;
            break;

        /* Get number of sectors on the disk (DWORD) */
        case GET_SECTOR_COUNT:

            *(u32*)buff = sector_num;
            res =  0; // RES_OK;
            break;

        case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
            *(u32*)buff = SECTOR_SIZE;
            res =  0; // RES_OK;
            break;

        /* Get erase block size in unit of sector (DWORD) */
        case GET_BLOCK_SIZE:
            *(u32*)buff = 8;    //相当于一个扇区里的页数
            res =  0; // RES_OK;
            break;

        default:
            res = 4; // RES_PARERR;
    }

    return res;
}

// =============================================================================
// 功能：在w25q中安装fat文件系统
// 参数： TargetFs -- 要挂载的文件系统
//      分区数据 -- 起始块，结束块数（擦除时不包括该块，只擦到该块的上一块），是否格式化；
// 返回：成功（true）；失败（false）；
// 备注：
// =============================================================================
bool_t ModuleInstall_W25qxxInstallFat(const char *TargetFs,s32 bend, u32 doformat)
{
    static char *name = "w25qxx";
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if((TargetFs != NULL) && (0 != bend))
    {
        if(W25qxx_is_install())
        {
            if(bend == -1)
                __W25qxx_Req(totalblocks, &bend);
            sector_num = bend * 65536 / 512;

            if(doformat)
            {
                struct uesz sz;
                sz.unit = 0;
                sz.block = 1;
                if(-1 == __W25qxx_Req(format, 0 , bend, &sz))
                {
                    warning_printf("w25q"," Format failure.");
                    return false;
                }
            }

            if(!dev_Create((const char*)name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)name))
            {
                error_printf("w25q"," register device(w25q) failed.\r\n");
                return false; // register failure
            }

            targetobj = obj_matchpath(TargetFs, &notfind);
            if(notfind)
            {
                error_printf("w25q"," not found need to install file system.\r\n");
                return false;
            }
            super = (struct FsCore *)obj_GetPrivate(targetobj);
            super->MediaInfo = name;
            if(strcmp(super->pFsType->pType, "FAT") == 0)      //这里的"FAT"为文件系统的类型名，在文件系统的filesystem结构中
            {
                super->MediaDrv = &W25QXX_Drv;
            }
            else
            {
                super->MediaDrv = 0;
                error_printf("w25q","  install file system type not FAT.\r\n");
                return false;
            }
            FullPath = malloc(strlen(name)+strlen(s_ptDeviceRoot->name) + 1);  //获取msc的完整路径
            sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name, name);
            FsBeMedia(FullPath, TargetFs);     //在w25q上挂载文件系统
            free(FullPath);
        }
    }
    return true;
}

