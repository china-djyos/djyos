#include <device/card/card.h>
#include <stdlib.h>
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/integer.h>
#include <fat/ff11/src/diskio.h>
#include <systime.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#include <string.h>
#include <device.h>
#include <stdbool.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32  ModuleInstall_SD(const char *targetfs,s32 doformat);
//    ModuleInstall_SD(CFG_SDCARD_FSMOUNT_NAME,CFG_SDCARD_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip hsmmc"//CPU的hsmmc驱动
//parent                       //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                        //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                       //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                     //初始化时机，可选值：early，medium，later, pre-main。
                                       //表示初始化时间，分别是早期、中期、后期
//dependence:"file system","heap","device file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件将强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件不会被强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                       //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_HSMMC == false )
//#warning  " cpu_onchip_hsmmc  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_HSMMC    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
#define CFG_SDCARD_FORMAT            false    //是否需要器件格式化。
//%$#@string,1,10,
#define CFG_SDCARD_FSMOUNT_NAME      "fat"    //需安装的文件系统的mount的名字
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


extern struct Object *s_ptDeviceRoot;
//-----------------------------------------------------------------------------
//功能:
//参数:
//输出:
//返回:
//-----------------------------------------------------------------------------
int MMC_disk_status(void)
{
    return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注: 可以将设备初始化放在这里;
//-----------------------------------------------------------------------------
int MMC_disk_initialize(void)
{
    static BYTE Done = 0;

    if(1 == Done)
        return (0);

    Done = 1;
    return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
int MMC_disk_read(BYTE *buff, DWORD sector, UINT count)
{
    u32 *Buf = (u32*)buff;
    extern s32 S3c2416_BlkRead(const u32 *Buf, u32 BlkNo, u32 Count);

    if(S3c2416_BlkRead(Buf, sector, count))
        return (-1);// 失败

    return (0);// 正确
}
//-----------------------------------------------------------------------------
//功能:
//参数: buff -- 写入内容。
//     sector -- 目标页号。
//     count -- 页数量。
//返回: 0 -- 正确;-1 -- 错误;
//备注:
//-----------------------------------------------------------------------------
int MMC_disk_write(BYTE *buff, DWORD sector, UINT count)
{

    u32 *Buf = (u32*)buff;
    extern s32 S3c2416_BlkWrite(const u32 *Buf, u32 BlkNo, u32 Count);

    if(S3c2416_BlkWrite(Buf, sector, count))
        return (-1);// 失败
    return (0);// 正确
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
int MMC_ioctl( BYTE cmd, void *buff)
{

    extern struct CardDescription Card;
    switch(cmd)
    {
        case GET_SECTOR_SIZE:
            *(DWORD *)(buff) = (1 << Card.BlkLenSettings);//
            break;
        case GET_SECTOR_COUNT:
            *(DWORD*)(buff) = Card.CapacityInBytes >> Card.BlkLenSettings;//
            break;
        case GET_BLOCK_SIZE:
            *(DWORD*)(buff) = 1;// todo
            break;
       default:
           break;
    }
    return (RES_OK);
}


struct FatDrvFuns SDDrv =
{
    .DrvStatus     = MMC_disk_status,
    .DrvInitialize = MMC_disk_initialize,
    .DrvRead       = MMC_disk_read,
    .DrvWrite      = MMC_disk_write,
    .DrvIoctl      = MMC_ioctl
};
extern struct CardDescription Card;
//-----------------------------------------------------------------------------
//功能:
//参数: targetfs -- 要挂载文件系统的mount点名字
//      Clean -- 器件格式化;"1"--是;"0"--否。
//返回: "0" -- 成功;
//      "-1" -- 输入参数错误;
//      "-2" -- 内存不足;
//      "-3" -- 操作失败;
//备注: todo：抽象一个sd管理结构体
//-----------------------------------------------------------------------------
s32  ModuleInstall_SD(const char *targetfs,u8 doformat)
{
    s32 Ret;
    const char *ChipName = "sdcard";    // 设备名;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if(NULL == ChipName)
        return (-1);

    Ret = S3c2416_HsmmcInit(1);
    if(0 == Ret)
    {
        if(Device_Create(ChipName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)ChipName)))
        {
            if((false == doformat) || ((doformat) &&
              (0 != S3c2416_BlkErase(0, ((Card.CapacityInBytes >> Card.BlkLenSettings)-1)))))
            {
                printk("\r\nMODULE : INSTALL : \"%s\" format failed during initialization.\r\n", defaultName);
                return (-3);// 失败
            }

            if(targetfs != NULL)
            {
                targetobj = OBJ_MatchPath(targetfs, &notfind);
                if(notfind)
                {
                    error_printf("SDCARD"," not found need to install file system.\r\n");
                    return -1;
                }
                super = (struct FsCore *)OBJ_GetPrivate(targetobj);
                super->MediaInfo = ChipName;
                if(strcmp(super->pFsType->pType, "FAT") == 0)      //这里的"FAT"为文件系统的类型名，在文件系统的filesystem结构中
                {
                    super->MediaDrv = &SDDrv;
                }
                else
                {
                    super->MediaDrv = 0;
                    error_printf("SDCARD","  install file system type not FAT\r\n");
                    return -1;
                }

                FullPath = malloc(strlen(ChipName)+strlen(s_ptDeviceRoot->name));  //获取msc的完整路径
                sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,ChipName);
                File_BeMedia(FullPath,targetfs);     //在msc上挂载文件系统
                free(FullPath);
            }
            else
            {
                warning_printf("SDCARD", "  No file system is installed\r\n");
            }
            return (0);
        }

    }

    return (-3);// 操作失败
}
