//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

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
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <cpu_peri_gpio.h>
#include <stm32f7xx_hal.h>
#include <stdlib.h>
#include <device.h>
#include <fat/port/drivers/fat_drivers.h>
#include <fat/ff11/src/integer.h>
#include <fat/ff11/src/diskio.h>
#include <stm32f7xx_hal_mmc.h>
#include <systime.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#include <string.h>
#include <stdbool.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern s32 ModuleInstall_MMC(const char *targetfs, u8 doformat, u32 speed);
//  ModuleInstall_MMC(CFG_EMMC_FSMOUNT_NAME,CFG_EMMC_FORMAT,CFG_EMMC_SPEED);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip emmc controler"//emmc控制器驱动
//parent:"device file system"  //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                      //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                   //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                     //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                    //初始化时机，可选值：early，medium，later, pre-main。
                                     //表示初始化时间，分别是早期、中期、后期
//dependence:"fat file system","device file system","lock"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件将强制选中，
                                     //如果依赖多个组件，则依次列出
//weakdependence:"none"              //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                     //选中该组件时，被依赖组件不会被强制选中，
                                     //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                     //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_EMMC_CONTROLER == false )
//#warning  " cpu_onchip_emmc_controler  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_EMMC_CONTROLER    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,255,
#define CFG_EMMC_SPEED           1       //"EMMC速度",指定SDMMC控制器的时钟频率。
//%$#@enum,true,false,
#define CFG_EMMC_FORMAT          false    //是否需要器件格式化。
//%$#@string,1,10,
#define CFG_EMMC_FSMOUNT_NAME    "fat"    //需安装的文件系统的mount的名字
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束
//@#$%component end configure
// ============================================================================

s32 MMC_HardInit(void);
extern uint32_t SDMMC_CmdSendStatus(SDMMC_TypeDef *SDMMCx, uint32_t Argument);
extern struct Object *s_ptDeviceRoot;
extern const Pin MMC[6]; // 在board.c之中定义

MMC_HandleTypeDef handleMMC;
static u32 SDMMC_BUS_CLK_DIV;
void *pOperationMutex;
// ============================================================================
// 功能：初始化EMMC IO
// 参数：
// 返回：
// 备注：IO设置
// ============================================================================
void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
    PIO_Configure(MMC, PIO_LISTSIZE(MMC));

    PIO_Set(&MMC[6]); // SDMMC1_RSTn拉高--todo：下标越界
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：IO重置
// ============================================================================
void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{
    ;
}

// ============================================================================
// 功能：EMMC进入sleep模式
// 参数：
// 返回：
// 备注：目前未使用
// ============================================================================
void EMMC_Sleep(void)
{
    u32 res;
    extern u32 EMMC_CMD_Sleep(SDMMC_TypeDef *SDMMCx, u32 wRCA);
    extern uint32_t SDMMC_CmdSelDesel(SDMMC_TypeDef *SDMMCx, uint64_t Addr);

    SDMMC_CmdSelDesel(handleMMC.Instance, 0);

    res = EMMC_CMD_Sleep(handleMMC.Instance, handleMMC.MmcCard.RelCardAdd);
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : entry sleep mode failed.\r\n");
    }
}

// ============================================================================
// 功能：EMMC退出sleep模式
// 参数：
// 返回：
// 备注：目前未只用
// ============================================================================
void EMMC_Wakeup(void)
{
    u32 res;
    extern u32 EMMC_CMD_Awake(SDMMC_TypeDef *SDMMCx, u32 wRCA);
    extern uint32_t SDMMC_CmdSelDesel(SDMMC_TypeDef *SDMMCx, uint64_t Addr);

    res = EMMC_CMD_Awake(handleMMC.Instance, handleMMC.MmcCard.RelCardAdd);
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : awake failed.\r\n");
        return ;
    }

    DJY_EventDelay(1000); // 等待唤醒

    res = SDMMC_CmdSelDesel(handleMMC.Instance, (uint32_t)(((uint32_t)handleMMC.MmcCard.RelCardAdd) << 16));
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : select failed(entry transfer mode).\r\n");
    }
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int _MMC_Status(void)
{
    return (0);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 _MMC_Initialize(void)
{
    return (0);
}

// ============================================================================
// 功能：MMC读
// 参数：
// 返回：
// 备注：
// ============================================================================
int _MMC_Read(BYTE *pBuf, DWORD qwSector, UINT dwCount)
{
    HAL_StatusTypeDef res;
    u32 response;
    u32 block = (u32)qwSector;
    u8 *buf = (u8*)pBuf;
    u32 count = (u32)dwCount;
    int ret = 0;

    Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);

    res = HAL_MMC_ReadBlocks(&handleMMC, buf, block, count, 0xFFFF);
    if(HAL_OK != res)
    {
        printk("\r\nEMMC : debug : read <%xH> <%xH> <%xH> <%xH>\r\n", block, count, res, handleMMC.ErrorCode);
        ret = -1;
    }
    else
    {   // 查询设备完成，即是否完成
        while(1)
        {
            DJY_EventDelay(1);
            if(count++ == SDMMC_MAX_TRIAL)
            {
                ret = -1;
                printk("\r\nEMMC : debug : read timeout\r\n");
                break;
            }

            res = SDMMC_CmdSendStatus(handleMMC.Instance, (u32)(handleMMC.MmcCard.RelCardAdd << 16));
            if(HAL_MMC_ERROR_NONE != res)
            {
                ret = -1;
                printk("\r\nEMMC : debug : read status <%xH>\r\n", res);
                break;
            }

            response = SDMMC_GetResponse(handleMMC.Instance, SDMMC_RESP1);
            if((response & 1 << 8) && ((response & (0xF << 9)) != (0x7 << 9)))
                break;
        }
    }

    Lock_MutexPost(pOperationMutex);
    return (ret);
}

// ============================================================================
// 功能：MMC写
// 参数：
// 返回：
// 备注：
// ============================================================================
int _MMC_Write(BYTE *pBuf, DWORD qwSector, UINT dwCount)
{
    HAL_StatusTypeDef res;
    u32 response;
    u32 block = (u32)qwSector;
    u8 *buf = (u8*)pBuf;
    u32 count = (u32)dwCount;
    int ret = 0;

    Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);

    res = HAL_MMC_WriteBlocks(&handleMMC, buf, block, count, 0xFFFF);
    if(HAL_OK != res)
    {
        printk("\r\nEMMC : debug : write <%xH> <%xH> <%xH> <%xH>\r\n", block, count, res, handleMMC.ErrorCode);
        ret = -1;
    }
    else
    {
        // 查询设备是否ready，即是否完成
        while(1)
        {
            DJY_EventDelay(1);
            if(count++ == SDMMC_MAX_TRIAL)
            {
                ret = -1;
                printk("\r\nEMMC : debug : write timeout\r\n");
                break;
            }

            res = SDMMC_CmdSendStatus(handleMMC.Instance, (u32)(handleMMC.MmcCard.RelCardAdd << 16));
            if(HAL_MMC_ERROR_NONE != res)
            {
                ret = -1;
                printk("\r\nEMMC : debug : write status <%xH>\r\n", res);
                break;
            }

            response = SDMMC_GetResponse(handleMMC.Instance, SDMMC_RESP1);
            if((response & 1 << 8) && ((response & (0xF << 9)) != (0x7 << 9)))
                break;
        }
    }

    Lock_MutexPost(pOperationMutex);
    return (ret);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
int _MMC_Ioctl( BYTE bCommand, void *pReturn)
{
    u8 command = (u8)bCommand;

    switch(command)
    {
    case GET_SECTOR_SIZE:
        *((DWORD *)pReturn) = handleMMC.MmcCard.BlockSize; //
        break;

    case GET_SECTOR_COUNT:
        *((DWORD*)pReturn) =  handleMMC.MmcCard.BlockNbr; //
        break;

    case GET_BLOCK_SIZE:
        *((DWORD*)pReturn) = 1; // TODO
        break;

    default:
       break;
    }

    return (RES_OK);
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
struct FatDrvFuns MMC_Drv =
{
    .DrvStatus     = _MMC_Status,
    .DrvInitialize = _MMC_Initialize,
    .DrvRead       = _MMC_Read,
    .DrvWrite      = _MMC_Write,
    .DrvIoctl      = _MMC_Ioctl
};

// ============================================================================
// 功能：MMC初始化
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 MMC_HardInit(void)
{
    s32 res;

    handleMMC.Instance = SDMMC1;
    handleMMC.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    handleMMC.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
    handleMMC.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    handleMMC.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    handleMMC.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    handleMMC.Init.ClockDiv            = SDMMC_BUS_CLK_DIV;

    res = (s32)HAL_MMC_Init(&handleMMC);

    return (res);
}

// ============================================================================
// 功能：安装MMC类设备
// 参数： targetfs -- 要挂载文件系统的mount点名字
//      doformat -- 器件格式化;"1"--是;"0"--否。
//      speed -- SDMMC控制器的时钟频率；
// 返回：0 -- 成功； 其他 -- 失败；
// 备注：
// ============================================================================
s32 ModuleInstall_MMC(const char *targetfs, u8 doformat, u32 speed)
{
    char *defaultName = "emmc"; // 设备缺省名;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;

    SDMMC_BUS_CLK_DIV = speed;

    pOperationMutex = (void*)Lock_MutexCreate("MMC");
    if(!pOperationMutex)
    {
        printf("\r\nMODULE : INSTALL : \"%s\" create mutex failed.\r\n", defaultName);
        return (-1);
    }

    if(MMC_HardInit())
    {
        printk("\r\nMODULE : INSTALL : \"%s\" initialization failed.\r\n", defaultName);
        return (-1);
    }

    if(!Device_Create((const char*)defaultName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)defaultName)))
    {
        printk("\r\nMODULE : INSTALL : \"%s\" initialization failed.\r\n", defaultName);
        return (-1); // 失败
    }

    if(true == doformat) // 擦除整个设备
    {
        if(HAL_MMC_Erase(&handleMMC, 0, handleMMC.MmcCard.LogBlockNbr-1))
        {
            printk("\r\nMODULE : INSTALL : \"%s\" format failed during initialization.\r\n", defaultName);
        }
        DJY_EventDelay(3000000); // 等待一段时间
    }

    if(targetfs != NULL)
    {
        targetobj = OBJ_MatchPath(targetfs, &notfind);
        if(notfind)
        {
            error_printf("mmc"," not found need to install file system.");
            return -1;
        }
        super = (struct FsCore *)OBJ_GetPrivate(targetobj);
        super->MediaInfo = defaultName;
        if(strcmp(super->pFsType->pType, "FAT") == 0)      //这里的"FAT"为文件系统的类型名，在文件系统的filesystem结构中
        {
            super->MediaDrv = &MMC_Drv;
        }
        else
        {
            super->MediaDrv = 0;
            error_printf("mmc","  install file system type not FAT");
            return -1;
        }

        FullPath = malloc(strlen(defaultName)+strlen(s_ptDeviceRoot->name));  //获取msc的完整路径
        sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,defaultName);
        File_BeMedia(FullPath,targetfs);     //在msc上挂载文件系统
        free(FullPath);
    }
    else
    {
        warning_printf("mmc", "  No file system is installed");
    }
    return (0);
}
// ============================================================================
// 功能：擦除整个设备
// 参数：dwArgC -- 参数个数；第一个参数
// 返回：
// 备注：
// ============================================================================
s32 MMC_Erase(s32 dwArgC, ...)
{
    s32 range = -1, i;
    s32 res = -1;
    va_list ap;

    va_start(ap, dwArgC);
    for(i = 0; i < dwArgC; i++)
    {
        switch(i)
        {
            case 0 : range = va_arg(ap, s32); break;
            default : break;
        }
    }
    va_end(ap);

    if(-1 == range) // 擦除整个设备
    {
        Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);
        res = HAL_MMC_Erase(&handleMMC, 0, handleMMC.MmcCard.LogBlockNbr-1);
        if(res)
        {
            printk("\r\nEMMC : debug : erase failed.\r\n");
        }
        Lock_MutexPost(pOperationMutex);
        DJY_EventDelay(3000000); // 等待一段时间
    }

    return (res);
}

#if 1
// ============================================================================
// 功能：本地测试样例
// 参数：
// 返回：
// 备注：
// ============================================================================
#include <string.h>
void LOCAL_Test(void)
{
    u8 buf[512];
    u8 check[512];
    s32 res;
    u32 i, block;
    u32 error = 0;
    u32 error2 = 0;

    res = ModuleInstall_MMC(NULL,0,0);
    if(res)
    {
        printk("\r\nLOCAL TEST : install failed.\r\n");
        while(1);
    }

    printk("\r\nLOCAL TEST : start to run...\r\n");
    printk("LOCAL TEST : strategy No.1\r\n");
    for(block = 0; block <  handleMMC.MmcCard.BlockNbr; block++)
    {
        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if(0xFF != check[i])
                break;
        }

        if(512 > i)
        {
            for(i = 0; i < 512; i++)
            {
                buf[i] = 0xFF;
            }

            res = _MMC_Write(buf, block, 1);
            if(res)
            {
                printk("LOCAL TEST : write %d failed.\r\n", block);
                error++;
                continue;
            }

            memset(check, 0x0, 512);
            res = _MMC_Read(check, block, 1);
            if(res)
            {
                printk("LOCAL TEST : read %d failed.\r\n", block);
                error++;
                continue;
            }

            for(i = 0; i < 512; i++)
            {
                if(0xFF != check[i])
                {
                    printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                    printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0xFF.\r\n", i, check[i]);
                    goto NEXT_BLOCK;
                }
            }
        }

        for(i = 0; i < 512; i++)
        {
            buf[i] = i;
        }

        res = _MMC_Write(buf, block, 1);
        if(res)
        {
            printk("LOCAL TEST : write %d failed.\r\n", block);
            error++;
            continue;
        }

        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error2++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if((u8)i != check[i])
            {
                printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0x%02x.\r\n", i, check[i], (u8)i);
                error2++;
                goto NEXT_BLOCK;
            }

        }

    NEXT_BLOCK:
        continue;

    }

    printk("LOCAL TEST : strategy No.2\r\n");
    for(block = 0; block <  handleMMC.MmcCard.BlockNbr; block++)
    {
        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if((u8)i != check[i])
            {
                printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0x%02x.\r\n", i, check[i], (u8)i);
                break;
            }

        }
    }

    {
    DJY_EventDelay(30000000);
    printk("LOCAL TEST : result : strategy 1 has %d errors\r\n", error);
    printk("LOCAL TEST : result : strategy 2 has %d errors\r\n", error2);

    }

}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
void LOCAL_Test2(void)
{
    s32 res;
    s32 fd;
    u8 buf[512], check[512];
    u32 i;
#if 0
    res = ModuleInstall_MMC(0);
    if(res)
    {
        printk("\r\nLOCAL TEST : install failed.\r\n");
        while(1);
    }

    res = ModuleInstall_FAT("/dev/emmc", 1);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT install failed.\r\n");
        while(1);
    }
#endif
    for(i = 0; i < 512; i++)
    {
        buf[i] = i;
    }


    fd = open("/fat/test1", O_CREAT | O_RDWR);
    if(-1 == fd)
    {
        printk("\r\nLOCAL TEST : FAT open failed.\r\n");
        while(1);
    }

    res = write(fd, buf, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT write failed.\r\n");
        while(1);
    }

    res = write(fd, buf, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT write failed.\r\n");
        while(1);
    }

    res = close(fd);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT close failed.\r\n");
        while(1);
    }

    fd = open("/fat/test1", O_CREAT | O_RDWR);
    if(-1 == fd)
    {
        printk("\r\nLOCAL TEST : FAT open failed.\r\n");
        while(1);
    }

    res = read(fd, check, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT read failed.\r\n");
        while(1);
    }

    for(i = 0; i < 512; i++)
    {
        if(buf[i] != check[i])
        {
            printk("\r\nLOCAL TEST : FAT write & read failed, %d, %xH, %xH.\r\n", i, buf[i], check[i]);
            while(1);
        }
    }

    res = read(fd, check, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT read failed.\r\n");
        while(1);
    }

    for(i = 0; i < 512; i++)
    {
        if(buf[i] != check[i])
        {
            printk("\r\nLOCAL TEST : FAT write & read failed, %d, %xH, %xH.\r\n", i, buf[i], check[i]);
            while(1);
        }
    }

    res = close(fd);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT close failed.\r\n");
        while(1);
    }

    res = remove("/fat/test1");
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT remove failed.\r\n");
        while(1);
    }

}
#endif
