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
#include <typedef.h>
#include <stdlib.h>
#include <device.h>
#include <sys_config.h>
#include <sys_ctrl_pub.h>
#include <djyos.h>
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/diskio.h>
#include <sdcard/sdcard.h>
#include <sdcard/sdio_driver.h>
#include <gpio_pub.h>
#include "cpu_peri.h"
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <string.h>
#include <filesystems.h>
#include <sdcard/sdio_driver.h>
#include "project_config.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern int ModuleInstall_SDCARD(void);
//    ModuleInstall_SDCARD();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip sdcard"    //片内sdcard驱动
//parent:"none"                         //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                         //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                      //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                        //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                      //初始化时机，可选值：early，medium，later, pre-main。
                                        //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","fat file system" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件将强制选中，
                                        //如果依赖多个组件，则依次列出
//weakdependence:"none"                 //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                        //选中该组件时，被依赖组件不会被强制选中，
                                        //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                          //该组件的互斥组件名（可以是none，表示无互斥组件），
                                        //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD == false )
//#warning  " cpu_onchip_uart  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_SDCARD    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,512,
//%$#@enum,true,false
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// ============================================================================

extern void sdcard_power_on(void);

int SDCARD_Status(void);
int SDCARD_Initialize(void);
int SDCARD_Read(BYTE *Buff, DWORD Sector, UINT Count);
int SDCARD_Write(BYTE *Buff, DWORD Sector, UINT Count);
int SDCARD_Ioctl( BYTE Cmd, void *Buff);
extern UINT32 sddev_control(char *dev_name, UINT32 cmd, VOID *param);
extern void sdcard_uninitialize(void);
extern SDIO_Error sdcard_initialize(void);

static uint16_t evtt_sd_install;
static uint16_t evtt_sd_uninstall;

extern struct Object *s_ptDeviceRoot;

#define RETRY_COUNT     100

struct FatDrvFuns SDCARD_Drv =
{
    .DrvStatus     = SDCARD_Status,
    .DrvInitialize = SDCARD_Initialize,
    .DrvRead       = SDCARD_Read,
    .DrvWrite      = SDCARD_Write,
    .DrvIoctl      = SDCARD_Ioctl
};

// ============================================================================
// 功能: SdCard状态
// 参数:
// 输出:
// 返回:
// ============================================================================
s32 __SDCARD_Status(void)
{
    s32 res = 1; //RES_ERROR;

    if(get_sdcard_is_ready ())
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
// 功能: USB初始化
// 参数:
// 返回:
// 备注: 可以将设备初始化放在这里;
// ============================================================================
s32 __SDCARD_Initialize(void)
{
    static BYTE Done = 0;

    if(1 == Done)
        return (0);

    Done = 1;
    return (0);
}

// ============================================================================
// 功能: USB读数据
// 参数: buff -- 读数据缓存。
//      sector -- 目标页号。
//      count -- 页数量。
// 返回: 0 -- 正确; 1 -- 错误; 3 -- 设备未准备好
// 备注:
// ============================================================================
s32 __SDCARD_Read(u8 *buff, u32 sector, u32 count)
{
    s32 res = 1; // RES_ERROR;
    u8 i = 0;

    if(count)
    {
        do
        {
            if(sdcard_read((char *)buff, count, sector))
                break;
            else
                i ++;   //偶尔可能会因为等待SD卡的响应超时而失败一次。如果连续读3次都失败，则认为是失败。
        }while(i < RETRY_COUNT);
        if(i >= RETRY_COUNT)
        {
            if(get_sdcard_is_ready ())
                res = 1; // RES_ERROR;
            else
            {
                printf("\r\n: info : sdcard : card is not ready! \r\n");
                res = 3; // RES_NOTRDY;
            }
        }
        else
            res = 0; // RES_OK;
    }
    else
        res = 0; // RES_OK;

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
s32 __SDCARD_Write(u8 *buff, u32 sector, u32 count)
{
    s32 res = 1; // RES_ERROR;
    u8 i = 0;

    if(count)
    {
        do
        {
            if(!sdcard_write((char *)buff, count, sector))
                break;
            else
                i ++;   //偶尔可能会因为等待SD卡的响应超时而失败一次。如果连续读3次都失败，则认为是失败。
        }while(i < RETRY_COUNT);
        if(i >= RETRY_COUNT)
        {
            if(get_sdcard_is_ready ())
                res = 1; // RES_ERROR;
            else
            {
                printf("\r\n: info : sdcard : card is not ready! \r\n");
                res = 3; // RES_NOTRDY;
            }
        }
        else
            res = 0; // RES_OK;
    }
    else
        res = 0; // RES_OK;

    return (res);
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
s32 __SDCARD_Ioctl( u8 cmd, void *buff)
{
    DRESULT res = 0; // RES_OK;
    SDCARD_S card_info;

    switch (cmd) {
        /* Make sure that no pending write process */
        case CTRL_SYNC:
            res = 0; // RES_OK;
            break;

        /* Get number of sectors on the disk (DWORD) */
        case GET_SECTOR_COUNT:
            sdcard_get_card_info(&card_info);
            if(card_info.total_block)
            {
                *(u32*)buff = card_info.total_block;
                res =  0; // RES_OK;
            }
            else
                res =  1; // RES_ERROR;
            break;

        case GET_SECTOR_SIZE: /* Get R/W sector size (WORD) */
            sdcard_get_card_info(&card_info);
            if(card_info.block_size)
            {
                *(u32*)buff = card_info.block_size;
                res =  0; // RES_OK;
            }
            else
                res =  1; // RES_ERROR;
            break;

        /* Get erase block size in unit of sector (DWORD) */
        case GET_BLOCK_SIZE:
            sdcard_get_card_info(&card_info);
            if(card_info.block_size)
            {
                *(u32*)buff = card_info.block_size;
                res =  0; // RES_OK;
            }
            else
                res =  1; // RES_ERROR;
            break;

        default:
            res = 4; // RES_PARERR;
    }

    return res;
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int SDCARD_Ioctl( u8 Cmd, void *Buff)
{
    return(__SDCARD_Ioctl((BYTE)Cmd, Buff));
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int SDCARD_Status(void)
{
    return (__SDCARD_Status());
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int SDCARD_Initialize(void)
{
    return (__SDCARD_Initialize());
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int SDCARD_Read(BYTE *Buff, DWORD Sector, UINT Count)
{
    if(!Buff)
    {
        printf("\r\n: erro : sdcard    : read buffer address is empty.\r\n");
        return (1);
    }

    return(__SDCARD_Read((u8*)Buff, (u32)Sector, (u32)Count));
}

// ============================================================================
// 功能:
// 参数:
// 返回:
// 备注:
// ============================================================================
int SDCARD_Write(BYTE *Buff, DWORD Sector, UINT Count)
{
    if(!Buff)
    {
        printf("\r\n: erro : sdcard    : write buffer address is empty.\r\n");
        return (1);
    }

    return(__SDCARD_Write((u8*)Buff, (u32)Sector, (u32)Count));
}

// ============================================================================
// 功能: 注册USB设备MSC
// 参数: fs -- 需要挂载的文件系统
// 返回: 成功（0）；失败（-1）；
// 备注:
// ============================================================================
s32 InstallDevSdcard(const char *fs)
{
    static char *name = "sdcard";
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    if(!Device_Create((const char*)name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)name))
    {
        error_printf("sdcard"," register device(SDCARD) failed.\r\n");
        return (-1); // register failure
    }
    if(fs != NULL)
    {
        targetobj = OBJ_MatchPath(fs, &notfind);
        if(notfind)
        {
            error_printf("sdcard"," not found need to install file system.\r\n");
            return -1;
        }
        super = (struct FsCore *)OBJ_GetPrivate(targetobj);
        super->MediaInfo = name;
        if(strcmp(super->pFsType->pType, "FAT") == 0)      //这里的"FAT"为文件系统的类型名，在文件系统的filesystem结构中
        {
            super->MediaDrv = &SDCARD_Drv;
        }
        else
        {
            super->MediaDrv = 0;
            error_printf("sdcard","  install file system type not FAT.\r\n");
            return -1;
        }

        FullPath = malloc(strlen(name)+strlen(s_ptDeviceRoot->name) + 1);  //获取msc的完整路径
        sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,name);
        File_BeMedia(FullPath,fs);     //在msc上挂载文件系统
        free(FullPath);
    }
    else
    {
        warning_printf("sdcard", "  No file system is installed.\r\n");
    }
    info_printf("sdcard", "  install device(SDCARD).\r\n");
    return (0); //
}
// ============================================================================
// 功能: 注销sdcard设备
// 参数:
// 返回: 成功（0）；失败（-1）；
// 备注:
// ============================================================================
s32 UninstallDevSdcard(void)
{
    static char *name = "sdcard";

    if(Device_DeleteByName(name))
    {
        error_printf("sdcard"," uninstall device(SDCARD) failed.\r\n");
        return (-1); // register failure
    }

    info_printf("sdcard", "  uninstall device(SDCARD).\r\n");
    return (0); //
}
// ============================================================================
// 功能: sdcard的热插中断响应
// 参数:
// 返回:
// 备注:
// ============================================================================
static void sdcard_isr_uninitialize(void);
static void sdcard_isr_initialize(void)
{
    DJY_EventPop(evtt_sd_install, NULL, 0, 0, 0, 0);
}
// ============================================================================
// 功能: sdcard的热拔中断响应
// 参数:
// 返回:
// 备注:
// ============================================================================
static void sdcard_isr_uninitialize(void)
{
    DJY_EventPop(evtt_sd_uninstall, NULL, 0, 0, 0, 0);
}
// ============================================================================
// 功能: sdcard的安装任务
// 参数:
// 返回:
// 备注:
// ============================================================================
static ptu32_t sdcard_install_event(void)
{
    extern s32 ModuleInstall_FAT(const char *dir_name, u32 opt, void *data);

    if(!get_sdcard_is_ready())
    {
        if(sdcard_initialize() != SD_OK)
            return -1;
    }
    if(OBJ_SearchChild(OBJ_GetRoot(), "SD") == NULL)
        ModuleInstall_FAT("SD", MS_INSTALLUSE, "SD");
    if(InstallDevSdcard("SD"))
        return -1;

    djy_gpio_irq_enable(GPIO12, 0);
    djy_gpio_attach_irq(GPIO12, PIN_IRQ_MODE_RISING, (void *)sdcard_isr_uninitialize, 0);   //设置卡被拔掉时的中断响应函数
    djy_gpio_irq_enable(GPIO12, 1);
    return 0;
}
// ============================================================================
// 功能: sdcard的卸载任务
// 参数:
// 返回:
// 备注:
// ============================================================================
static ptu32_t sdcard_uninstall_event(void)
{
    extern void sdcard_uninitialize(void);
    extern s32 UnfileSystem_FAT(const char *dir);
    OBJ_SetPwd("/");
    UnfileSystem_FAT("/SD");
    UninstallDevSdcard();
    DJY_EventDelay(20*mS);

    djy_gpio_irq_enable(GPIO12, 0);
    djy_gpio_attach_irq(GPIO12, PIN_IRQ_MODE_FALLING, (void *)sdcard_isr_initialize, 0);//设置卡被插上时的中断响应函数
    djy_gpio_irq_enable(GPIO12, 1);
    sdcard_uninitialize();
    return 0;
}

// =============================================================================
// 功能：初始化SPI模块
// 参数：无
// 返回：1=成功，0=失败
// =============================================================================
int ModuleInstall_SDCARD(void)
{
    sdcard_init();
    sdcard_power_on();
    evtt_sd_install = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            sdcard_install_event,NULL,CFG_MAINSTACK_LIMIT, "sd install");
    if (CN_EVTT_ID_INVALID == evtt_sd_install)
    {
        error_printf("sdcard"," sd install register failed.\r\n");
        return 0;
    }

    evtt_sd_uninstall = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
            sdcard_uninstall_event,NULL,CFG_MAINSTACK_LIMIT, "sd uninstall");
    if (CN_EVTT_ID_INVALID == evtt_sd_uninstall)
    {
        if(DJY_EvttUnregist(evtt_sd_install) == false)
            error_printf("sdcard","sd install unregister failed.\r\n");
        error_printf("sdcard"," sd uninstall register failed.\r\n");
        return 0;
    }

    gpio_config(12, GMODE_INPUT_PULLUP);
    djy_gpio_attach_irq(GPIO12, PIN_IRQ_MODE_FALLING, (void *)sdcard_isr_initialize, 0);//设置卡被插上时的中断响应函数
    djy_gpio_irq_enable(GPIO12, 1);

    return 1;
}

// =============================================================================
// 功能：    开sd卡电源，
// 参数：    无
// 返回：无
// =============================================================================
void sdcard_power_on(void)
{
    u32 param;
    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;                 //先使能电源
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_ENABLE, &param);
//    param = QSPI_IO_3_3V;
//    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_IO_VOLTAGE, &param);
    param = PSRAM_VDD_3_3V_DEF;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_VDDRAM_VOLTAGE, &param);    //再设置电压
}
// =============================================================================
// 功能：    关sd卡电源，
// 参数：    无
// 返回：无
// =============================================================================
void sdcard_power_off(void)
{
    u32 param;

    sdcard_uninitialize();

    param = BLK_BIT_MIC_QSPI_RAM_OR_FLASH;
    sddev_control(SCTRL_DEV_NAME, CMD_SCTRL_BLK_DISABLE, &param);
//    param = QSPI_IO_1_8V;
//    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_IO_VOLTAGE, &param);
    param = PSRAM_VDD_1_8V;
    sddev_control(SCTRL_DEV_NAME, CMD_QSPI_VDDRAM_VOLTAGE, &param);
}
// =============================================================================
// 功能：    掉电复位sd卡
// 参数：    无
// 返回：无
// =============================================================================
void pwoer_off_sdcard_reset(void)
{
    sdcard_power_off();
    DJY_EventDelay(3000*1000);
    sdcard_power_on();
    DJY_EventDelay(2000*1000);
}

// =============================================================================
// 功能： 软件复位SD卡，并且安装好fat文件系统
// 参数：    无
// 返回：无
// =============================================================================
bool_t soft_sdcard_reset(void)
{
    atom_high_t atom;
    bool_t ret = false;
    u32 len = strlen(CFG_FAT_MOUNT_POINT);
    char *path = malloc(len + 1);
    if(path)
    {
        sprintf(path,"%s%s", "/", CFG_FAT_MOUNT_POINT);
        OBJ_SetPwd("/");    //卸载SD卡
        UnfileSystem_FAT(path);
        UninstallDevSdcard();
        DJY_EventDelay(20*1000);
        sdcard_uninitialize();

        atom = Int_HighAtomStart();
    //            sdcard_power_off();
    //            Djy_DelayUs(500 * 1000);
    //            sdcard_power_on();
        DJY_DelayUs(200 * 1000);

        if(!get_sdcard_is_ready())  //重新安装SD卡
        {
            if(sdcard_initialize() != SD_OK)
            {
                printf("SD initialize fail.\r\n");
                Int_HighAtomEnd(atom);
                free(path);
                return ret;
            }
        }
        if(OBJ_SearchChild(OBJ_GetRoot(), CFG_FAT_MOUNT_POINT) == NULL)
            ModuleInstall_FAT(CFG_FAT_MOUNT_POINT, CFG_FAT_MS_INSTALLUSE, CFG_FAT_MEDIA_KIND);
        if(InstallDevSdcard(CFG_FAT_MOUNT_POINT) == 0)
        {
            ret = true;
        }
        Int_HighAtomEnd(atom);
        free(path);
    }

    return ret;
}
