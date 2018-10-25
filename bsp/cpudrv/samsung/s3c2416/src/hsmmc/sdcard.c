#include <device/card/card.h>
#include <stdlib.h>
#include <fs/fat/port/drivers/fat_drivers.h>
#include <fs/fat/ff11/src/integer.h>
#include <fs/fat/ff11/src/diskio.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32  ModuleInstall_SD(const char *ChipName, s32 Clean);
//    ModuleInstall_SD(CFG_SD_CHIP_NAME,CFG_SD_CLEAN);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu_peri_hsmmc"      //CPU的hsmmc驱动
//parent:none                          //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                        //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                       //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                     //初始化时机，可选值：early，medium，later。
                                       //表示初始化时间，分别是早期、中期、后期
//dependence:"djyfs","heap","devfile"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件将强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件不会被强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                         //该组件的依赖组件名（可以是none，表示无依赖组件），
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_SD_CLEAN   //****检查参数是否已经配置好
#warning    cpu_peri_hsmmc组件参数未配置，使用默认值
//%$#@num,0,100,
#define CFG_SD_CLEAN                  (0)     //"配置是否清除SD",
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_SD_CHIP_NAME              ("/dev/sd")   //"SD名称",包括了上级的路径
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


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

//-----------------------------------------------------------------------------
//功能:
//参数: ChipName --
//      Clean -- 器件格式化;"1"--是;"0"--否。
//返回: "0" -- 成功;
//      "-1" -- 输入参数错误;
//      "-2" -- 内存不足;
//      "-3" -- 操作失败;
//备注: todo：抽象一个sd管理结构体
//-----------------------------------------------------------------------------
struct FatDrvFuns SDDrv =
{
    .DrvStatus     = MMC_disk_status,
    .DrvInitialize = MMC_disk_initialize,
    .DrvRead       = MMC_disk_read,
    .DrvWrite      = MMC_disk_write,
    .DrvIoctl      = MMC_ioctl
};
extern struct CardDescription Card;
s32  ModuleInstall_SD(const char *ChipName, s32 Clean)
{
    s32 Ret;
    char *Name;
    (void)Clean;

    if(NULL == ChipName)
        return (-1);

    Name = (char*)malloc(strlen(ChipName)+1);
    if(NULL == Name)
        return (-2);

    strcpy(Name, ChipName);

    Ret = S3c2416_HsmmcInit(1);
    if(0 == Ret)
    {
        if(dev_add(NULL,Name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)&SDDrv)))
        {
            if((0 == Clean) || ((Clean) &&
              (0 == S3c2416_BlkErase(0, ((Card.CapacityInBytes >> Card.BlkLenSettings)-1)))))
                  return (0);// 成功
        }

    }

    free(Name);
    return (-3);// 操作失败
}
