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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

#include <string.h>
#include <stdlib.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device.h>
#include <filesystems.h>
#include <device/djy_flash.h>
#include <spibus.h>
#include <unit_media.h>
#include <dbug.h>
#include "spi_flash.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_SpiFlash(void);
//    ModuleInstall_SpiFlash();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"spi flash"    //SPI和QSPI接口的flash芯片
//parent:"none"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"spi bus",//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"       //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH == false )
//#warning  " spi flash  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NORFLASH_SPI_FLASH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_SPI_FLASH_PART_ERASE               false      //"分区选项,是否需要擦除该芯片。"
#define CFG_SPI_FLASH_QSPI_ENABLE              false      //"是否使用QSPI模式"
//%$#@string,1,10,
#define CFG_SPI_FLASH_SPI_NAME                 "SPI1"      //"用的SPI设备名字，spi模式下有效"
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#if CFG_SPI_FLASH_QSPI_ENABLE == FALSE
static struct SPI_Device *s_ptSpiPort; // 器件使用的SPI端口
#endif
struct MutexLCB *SpiFlashLock;           //芯片互斥访问保护
struct NorDescr *SpiFlashdescription = NULL;
bool_t SpiFlashInitFlag = false;
const char *SpiFlashName = "spi flash dev";      //该flash在obj在的名字
struct umedia *SpiFlashUm;
extern struct Object *s_ptDeviceRoot;
static bool_t Enable4B_AddrOpt = false;


static u32 FlashPageSize = 0;        //"每页的字节数"
static u32 FlashPageInSector = 0;         //"每个扇区有多少页"
static u32 FlashSectorInBolck = 0;         //"每个块有多少扇区"
static u32 FlashSectorNum =0;       //"总的扇区数"


#define CMD_SpiFlashWriteEnable        0x06
#define CMD_SpiFlashWriteDisable       0x04
#define CMD_SpiFlashReadStatusReg1     0x05
#define CMD_SpiFlashReadStatusReg2     0x35
#define CMD_SpiFlashReadStatusReg3     0x15
#define CMD_SpiFlashWriteStatusReg1    0x01
#define CMD_SpiFlashWriteStatusReg2    0x31
#define CMD_SpiFlashWriteStatusReg3    0x11
#define CMD_SpiFlashReadData           0x03
#define CMD_SpiFlashReadData_4B_Addr   0x13
#define CMD_SpiFlashFastReadData       0x0B
#define CMD_SpiFlashFastReadDual       0x3B
#define CMD_SpiFlashPageProgram        0x02
#define CMD_SpiFlashPageProgram_4B_Addr 0x12
#define CMD_SpiFlashBlockErase         0xD8
#define CMD_SpiFlashBlockErase_4B_Addr 0xDC
#define CMD_SpiFlashSectorErase        0x20
#define CMD_SpiFlashSectorErase_4B_Addr 0x21
#define CMD_SpiFlashChipErase          0xC7
#define CMD_SpiFlashPowerDown          0xB9
#define CMD_SpiFlashReleasePowerDown   0xAB
#define CMD_SpiFlashDeviceID           0xAB
#define CMD_SpiFlashManufactDeviceID   0x90
#define CMD_SpiFlashJedecDeviceID      0x9F
#define CMD_SpiFlashEnable4ByteAddr    0xB7
#define CMD_SpiFlashExit4ByteAddr      0xE9
#define CMD_SpiFlashSetReadParam       0xC0
#define CMD_SpiFlashEnterQPIMode       0x38
#define CMD_SpiFlashExitQPIMode        0xFF

static struct SPI_FlashInfo  FlashInfo[] ={
    {"W25Q64",              0xEF4017, 256, 16, 16, 8 * 1024 * 1024},
    {"W25Q128",             0xEF4018, 256, 16, 16, 16 * 1024 * 1024},
    {"W25Q256",             0xEF4019, 256, 16, 16, 32 * 1024 * 1024},

//  =======================还未验证==========================================
    // {"W25X10",              0xEF3011, 256, 16, 16, 128 * 1024},
    // {"W25X20",              0xEF3012, 256, 16, 16, 256 * 1024},
    // {"W25X40",              0xEF3013, 256, 16, 16, 512 * 1024},
    // {"W25X80",              0xEF3014, 256, 16, 16, 1 * 1024 * 1024},
    // {"W25X16",              0xEF3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"W25X32",              0xEF3016, 256, 16, 16, 4 * 1024 * 1024},
    // {"W25X64",              0xEF3017, 256, 16, 16, 8 * 1024 * 1024},
    // {"W25X128",             0xEF3018, 256, 16, 16, 16 * 1024 * 1024},
    // {"W25Q40",              0xEF4013, 256, 16, 16, 512 * 1024},
    // {"W25Q80",              0xEF4014, 256, 16, 16, 1 * 1024 * 1024},
    // {"W25Q16",              0xEF4015, 256, 16, 16, 2 * 1024 * 1024},
    // {"W25Q32",              0xEF4016, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P040",             0x514013, 256, 16, 16, 512 * 1024},
    // {"AT25DF321",           0xC2201A, 256, 16, 16, 100 * 1024 * 1024},
    // {"MX25L3205",           0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"A25L05PU",            0x372010, 256, 16, 16, 64 * 1024},
    // {"A25L05PT",            0x372020, 256, 16, 16, 64 * 1024},
    // {"A25L10PU",            0x372011, 256, 16, 16, 128 * 1024},
    // {"A25L05PT",            0x372021, 256, 16, 16, 128 * 1024},
    // {"A25L20PU",            0x372012, 256, 16, 16, 256 * 1024},
    // {"A25L20PT",            0x372022, 256, 16, 16, 256 * 1024},
    // {"A25L512",             0x373010, 256, 16, 16, 64 * 1024},
    // {"A25L010",             0x373011, 256, 16, 16, 128 * 1024},
    // {"A25L020",             0x373012, 256, 16, 16, 256 * 1024},
    // {"A25L40P",             0x372013, 256, 16, 16, 512 * 1024},
    // {"A25L040",             0x373013, 256, 16, 16, 512 * 1024},
    // {"A25L80P",             0x372014, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L80P",             0x7F3720, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L080",             0x373015, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L080",             0x373014, 256, 16, 16, 1 * 1024 * 1024},
    // {"A25L16P(bottom)",     0x372015, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L16P(otp)",        0x372025, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L016",             0x373015, 256, 16, 16, 2 * 1024 * 1024},
    // {"A25L032",             0x373016, 256, 16, 16, 4 * 1024 * 1024},
    // {"A25LQ32A",            0x374016, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25FS010",           0x1F6601, 256, 16, 16, 128 * 1024},
    // {"AT25FS040",           0x1F6604, 256, 16, 16, 512 * 1024},
    // {"AT25F512",            0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F512B",           0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F512A",           0x1F6500, 256, 16, 16, 64 * 1024},
    // {"AT25F1024",           0x1F, 256, 16, 16, 128 * 1024},
    // {"AT25F2048",           0x1F, 256, 16, 16, 256 * 1024},
    // {"AT25F4096",           0x1F, 256, 16, 16, 512 * 1024},
    // {"AT25DF021",           0x1F4300, 256, 16, 16, 256 * 1024},
    // {"AT25DF041A",          0x1F4401, 256, 16, 16, 512 * 1024},
    // {"AT25DF081A",          0x1F4501, 256, 16, 16, 1 * 1024 * 1024},
    // {"AT25DF161A",          0x1F4602, 256, 16, 16, 2 * 1024 * 1024}
    // {"AT25DF321",           0x1F4700, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25DF321A",          0x1F4701, 256, 16, 16, 4 * 1024 * 1024},
    // {"AT25DF641",           0x1F4800, 256, 16, 16, 8 * 1024 * 1024},
    // {"AT26F004",            0x1F0400, 256, 16, 16, 512 * 1024},
    // {"AT26DF041",           0x1F4400, 256, 16, 16, 512 * 1024},
    // {"AT26DF081A",          0x1F4501, 256, 16, 16, 1 * 1024 * 1024},
    // {"AT26DF161A",          0x1F4601, 256, 16, 16, 2 * 1024 * 1024},
    // {"AT26DF161",           0x1F4600, 256, 16, 16, 2 * 1024 * 1024},
    // {"AT26DF321",           0x1F4700, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B05",             0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25B05T",            0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25F05",             0x1c3110, 256, 16, 16, 64 * 1024},
    // {"EN25P05",             0x1c2010, 256, 16, 16, 64 * 1024},
    // {"EN25LF05",            0x1c3110, 256, 16, 16, 64 * 1024},
    // {"EN25F10",             0x1c3111, 256, 16, 16, 128 * 1024},
    // {"EN25P10",             0x1c2011, 256, 16, 16, 128 * 1024},
    // {"EN25LF10",            0x1c3111, 256, 16, 16, 128 * 1024},
    // {"EN25LF20",            0x1c3112, 256, 16, 16, 256 * 1024},
    // {"EN25F20",             0x1c3112, 256, 16, 16, 256 * 1024},
    // {"EN25F40",             0x1c3113, 256, 16, 16, 512 * 1024},
    // {"EN25LF40",            0x1c3113, 256, 16, 16, 512 * 1024},
    // {"EN25Q80",             0x1c3014, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25P80",             0x1c2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25T80",             0x1c5114, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25F80",             0x1c3114, 256, 16, 16, 1 * 1024 * 1024},
    // {"EN25D16",             0x1c3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B16",             0x1c2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B16T",            0x1c2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25F16",             0x1c3115, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25Q16",             0x1c3015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25QH16",            0x1c7015, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25T16",             0x1c5115, 256, 16, 16, 2 * 1024 * 1024},
    // {"EN25B32",             0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B32T",            0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25P32",             0x1c2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25F32",             0x1c3116, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25Q32",             0x1c3316, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25QH32",            0x1c7016, 256, 16, 16, 4 * 1024 * 1024},
    // {"EN25B64",             0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25B64T",            0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25P64",             0x1c2017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25Q64",             0x1c3017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25QH64",            0x1c7017, 256, 16, 16, 8 * 1024 * 1024},
    // {"EN25Q128",            0x1c3018, 256, 16, 16, 16 * 1024 * 1024},
    // {"ES25P40",             0x4A2013, 256, 16, 16, 256 * 1024},
    // {"ES25P80",             0x4A2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"ES25P16",             0x4A2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L04UA",            0x8C8C8C, 256, 16, 16, 256 * 1024},
    // {"F25L004A",            0x8C2013, 256, 16, 16, 256 * 1024},
    // {"F25L08PA",            0x8C2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"F25L008A",            0x8C2014, 256, 16, 16, 1 * 1024 * 1024},
    // {"F25L016A",            0x8C2015, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L16PA",            0x8C1414, 256, 16, 16, 2 * 1024 * 1024},
    // {"F25L32PA",            0x8C2016, 256, 16, 16, 4 * 1024 * 1024},
    // {"F25L32QA",            0x8C4016, 256, 16, 16, 4 * 1024 * 1024},
    // {"F25L64QA",            0x8C4117, 256, 16, 16, 8 * 1024 * 1024},
    // {"SST25VF040",          0xBF258d, 256, 16, 16, 512 * 1024},
    // {"GD25Q512",            0xC84010, 256, 16, 16, 64 * 1024},
    // {"GD25Q10",             0xC84011, 256, 16, 16, 128 * 1024},
    // {"GD25Q20B",            0xC84012, 256, 16, 16, 256 * 1024},
    // {"GD25Q21B",            0xC84112, 256, 16, 16, 256 * 1024},
    // {"GD25Q40B",            0xC84013, 256, 16, 16, 512 * 1024},
    // {"GD25Q41B",            0xC84013, 256, 16, 16, 512 * 1024},
    // {"GD25LQ40",            0xC86013, 256, 16, 16, 512 * 1024},
    // {"GD25F40",             0xC82013, 256, 16, 16, 512 * 1024},
    // {"GD25F80",             0xC82014, 256, 16, 16, 1 * 1024 * 1024},
    // {"GD25Q80",             0xC84014, 256, 16, 16, 1 * 1024 * 1024},
    // {"GD25Q16",             0xC84015, 256, 16, 16, 2 * 1024 * 1024},
    // {"GD25Q32B",            0xC84016, 256, 16, 16, 4 * 1024 * 1024},
    // {"GD25Q64",             0xC84017, 256, 16, 16, 8 * 1024 * 1024},
    // {"GD25Q128",            0xC84018, 256, 16, 16, 16 * 1024 * 1024},
    // {"GD25Q256",            0xC84019, 256, 16, 16, 32 * 1024 * 1024},
    // {"GD25Q512",            0xC84020, 256, 16, 16, 64 * 1024 * 1024},
    // {"KH25L4006E",          0xC2013 , 256, 16, 16, 512 * 1024},
    // {"KH25L8006E",          0xC2014 , 256, 16, 16, 1 * 1024 * 1024},
    // {"KH25L8036",           0xC22615, 256, 16, 16, 1 * 1024 * 1024},
    // {"KH25L1606E",          0xC2015 , 256, 16, 16, 2 * 1024 * 1024},
    // {"MS25X40",             0x373013, 256, 16, 16, 512 * 1024},
    // {"MX25V512",            0xC22010, 256, 16, 16, 64 * 1024},
    // {"MX25L512",            0xC22010, 256, 16, 16, 64 * 1024},
    // {"MX25V1005",           0xC22011, 256, 16, 16, 128 * 1024},
    // {"MX25V2005",           0xC22012, 256, 16, 16, 256 * 1024},
    // {"MX25L2005",           0xC22012, 256, 16, 16, 256 * 1024},
    // {"MX25U4035",           0xC22533, 256, 16, 16, 512 * 1024},
    // {"MX25V4035",           0xC22553, 256, 16, 16, 512 * 1024},
    // {"MX25V4005",           0xC22013, 256, 16, 16, 512 * 1024},
    // {"MX25L4005",           0xC22013, 256, 16, 16, 512 * 1024},
    // {"MX25V8005",           0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L8005",           0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25V8035",           0xC22554, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25U8035",           0xC22534, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L8006E",          0xC22014, 256, 16, 16, 1 * 1024 * 1024},
    // {"MX25L1606E",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1605A",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1635D",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1633E",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1635E",          0xC22515, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1636D",          0xC22415, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1636E",          0xC22515, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1608D",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L1608E",          0xC22015, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25U1635E",          0xC22535, 256, 16, 16, 2 * 1024 * 1024},
    // {"MX25L3225D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3237D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3236D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3206E",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3235D",          0xC25E16, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25U3235F",          0xC22536, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25U3235E",          0xC22536, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3208D",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3208E",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L3205D",          0xC22016, 256, 16, 16, 4 * 1024 * 1024},
    // {"MX25L6455E",          0xC22617, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6465E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6408D",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6408E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6406E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6445E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6405D",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L6436E",          0xC22017, 256, 16, 16, 8 * 1024 * 1024},
    // {"MX25L12805D",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12835E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12835F",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25U12835F",         0xC22538, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12845E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12836E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12865E",         0xC22018, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L12855E",         0xC22618, 256, 16, 16, 16 * 1024 * 1024},
    // {"MX25L25635E",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25635F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25639F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25735E",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25L25735F",         0xC22019, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX25U25635F",         0xC22539, 256, 16, 16, 32 * 1024 * 1024},
    // {"MX66U51235F",         0xC2253A, 256, 16, 16, 64 * 1024 * 1024},
    // {"MX66L51235F",         0xC2201A, 256, 16, 16, 64 * 1024 * 1024},
    // {"PM25LV512A",          0x7F9D7B, 256, 16, 16, 64 * 1024},
    // {"PM25LV010A",          0x7F9D7C, 256, 16, 16, 128 * 1024},
    // {"PM25LV020",           0x7F9D7D, 256, 16, 16, 256 * 1024},
    // {"PM25LV040",           0x7F9D7E, 256, 16, 16, 512 * 1024},
    // {"PM25LV080",           0x7F9D7F, 256, 16, 16, 64 * 1024},
    // {"S25FL001",            0x010210, 256, 16, 16, 128 * 1024},
    // {"S25FL002",            0x010211, 256, 16, 16, 256 * 1024},
    // {"S25FL004",            0x010212, 256, 16, 16, 512 * 1024},
    // {"S25FL008",            0x010213, 256, 16, 16, 1 * 1024 * 1024},
    // {"S25FL016",            0x010214, 256, 16, 16, 2 * 1024 * 1024},
    // {"S25FL032",            0x010215, 256, 16, 16, 4 * 1024 * 1024},
    // {"S25FL064",            0x010216, 256, 16, 16, 8 * 1024 * 1024},
    // {"S25FL128",            0x012017, 256, 16, 16, 16 * 1024 * 1024},
    // {"SST25WF512",          0xBF2501, 256, 16, 16, 64 * 1024},
    // {"SST25WF010",          0xBF2502, 256, 16, 16, 128 * 1024},
    // {"SST25WF020",          0xBF2503, 256, 16, 16, 256 * 1024},
    // {"SST25WF040",          0xBF2504, 256, 16, 16, 512 * 1024},
    // {"SST25VF040B",         0xBF258D, 256, 16, 16, 512 * 1024},
    // {"SST25VF080B",         0xBF258E, 256, 16, 16, 1 * 1024 * 1024},
    // {"SST25VF016B",         0xBF2541, 256, 16, 16, 2 * 1024 * 1024},
    // {"SST25VF032B",         0xBF254A, 256, 16, 16, 4 * 1024 * 1024},
    // {"SST25VF064B",         0xBF254B, 256, 16, 16, 8 * 1024 * 1024},
    // {"SST26VF016 ",         0xBF2601, 256, 16, 16, 2 * 1024 * 1024},
    // {"SST26VF032 ",         0xBF2602, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P05A",             0x202010, 256, 16, 16, 64 * 1024},
    // {"M25P10A",             0x202011, 256, 16, 16, 128 * 1024},
    // {"M25P20",              0x202012, 256, 16, 16, 256 * 1024},
    // {"M25P40",              0x202013, 256, 16, 16, 512 * 1024},
    // {"M25P80",              0x202014, 256, 16, 16, 1 * 1024 * 1024},
    // {"M25P16",              0x202015, 256, 16, 16, 2 * 1024 * 1024},
    // {"M25P32",              0x202016, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25P64",              0x202017, 256, 16, 16, 8 * 1024 * 1024},
    // {"M25P128",             0x202018, 256, 16, 16, 16 * 1024 * 1024},
    // {"M25PX16",             0x207115, 256, 16, 16, 2 * 1024 * 1024},
    // {"M25PX32",             0x207116, 256, 16, 16, 4 * 1024 * 1024},
    // {"M25PX64",             0x207117, 256, 16, 16, 8 * 1024 * 1024},
    // {"M25PE10",             0x208011, 256, 16, 16, 128 * 1024},
    // {"M25PE20",             0x208012, 256, 16, 16, 256 * 1024},
    // {"M25PE40",             0x208013, 256, 16, 16, 512 * 1024},
    // {"M25PE80",             0x208014, 256, 16, 16, 1 * 1024 * 1024},
    // {"M25PE16",             0x208015, 256, 16, 16, 2 * 1024 * 1024}
//  ========================================================================
};

//-----------------------------------------------------------------------------
//功能: 读状态寄存器
//参数: regno：状态寄存器序号
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashReadSR(u8 regno, u8 *sta)
{
    u8 Command[1];
    bool_t ret = false;
    switch(regno)
    {
        case 1:
            Command[0] = CMD_SpiFlashReadStatusReg1;    //读状态寄存器1
            break;
        case 2:
            Command[0] = CMD_SpiFlashReadStatusReg2;    //读状态寄存器2
            break;
        case 3:
            Command[0] = CMD_SpiFlashReadStatusReg3;    //读状态寄存器3
            break;
        default:
            Command[0] = CMD_SpiFlashReadStatusReg1;
            break;
    }
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI，地址位0，4线传数据，8位地址，无地址，4线传输指令，无空周期，1字节数据
    ret = QSPI_Send_CMD(Command[0],0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    if (ret)
    {
        ret = QSPI_Receive(sta, 1);
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    CommandFrame.RecvBuf = sta;
    CommandFrame.RecvLen = 1;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif

    return ret;
}


//-----------------------------------------------------------------------------
//功能: 等待空闲
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(SpiFlashReadSR(1, &sta) == false)
    {
        return false;
    }
    while((sta & 0x01) == 0x01)   //等待空闲
    {
        if(SpiFlashReadSR(1, &sta) == false)
            return false;
        DJY_EventDelay(1000);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("SpiFlash","Wait busy timeput.\r\n");
            break;
        }
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写状态寄存器
//参数: regno：状态寄存器序号，data: 待写入的数据
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteSR(u8 regno, u8 data)
{
    bool_t ret = false;
    u8 Command[2];
    switch(regno)
    {
        case 1:
            Command[0] = CMD_SpiFlashWriteStatusReg1;    //写状态寄存器1
            break;
        case 2:
            Command[0] = CMD_SpiFlashWriteStatusReg2;    //写状态寄存器2
            break;
        case 3:
            Command[0] = CMD_SpiFlashWriteStatusReg3;    //写状态寄存器3
            break;
        default:
            Command[0] = CMD_SpiFlashWriteStatusReg1;
            break;
    }

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI，地址位0，4线传数据，8位地址，无地址，4线传输指令，无空周期，1字节数据
    ret = QSPI_Send_CMD(Command[0],0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES);
    if (ret)
    {
        ret = QSPI_Transmit(&data, 1);
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    Command[1] = data;
    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 2;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写使能等待
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(SpiFlashReadSR(1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            SpiFlashReadSR(1, &sta);
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
//功能: 写使能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteEnable(void)
{
    
    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI，地址位0，无数据，8位地址，无地址，4线传输指令，无空周期，0字节数据
    if(QSPI_Send_CMD(CMD_SpiFlashWriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
    {
        return false;
    }
#else
    //SPI，
    struct SPI_DataFrame CommandFrame;
    bool_t Ret = false;
    u8 Command[1] = {CMD_SpiFlashWriteEnable};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            Ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }

    if (false == Ret)
    {
        return false;
    }
#endif
    return SpiFlashWriteEnableWait();
}

//-----------------------------------------------------------------------------
//功能: 写失能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteDisable(void)
{
    bool_t ret = false;

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI，地址位0，无数据，8位地址，无地址，4线传输指令，无空周期，0字节数据
    ret = QSPI_Send_CMD(CMD_SpiFlashWriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command[1] = {CMD_SpiFlashWriteDisable};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            ret = true;
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 读ID
//参数: 无
//返回: 芯片ID
//备注:
//-----------------------------------------------------------------------------
u32 SpiFlashReadID(void)
{
    u8 temp[3];
    u32 deviceid = 0;
    bool_t ret = false;

    if(SpiFlashWaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    //QSPI，地址位0，4线传输数据，24位地址，4线传输地址，4线传输指令，无空周期，2字节数据
    ret = QSPI_Send_CMD(CMD_SpiFlashJedecDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);

    if(ret)
    {
        QSPI_Receive(temp,3);
        deviceid=(temp[0] << 16) | (temp[1] << 8) | temp[2];
    }
    else
    {
        deviceid = 0;
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command = CMD_SpiFlashJedecDeviceID;

    CommandFrame.RecvBuf = temp;
    CommandFrame.RecvLen = 3;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = &Command;
    CommandFrame.SendLen = 1;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            deviceid = (temp[0] << 16) | (temp[1] << 8) | temp[2];
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("SpiFlash","spi cs inactive failed.\r\n");
        }
    }
#endif
    return deviceid;
}
//-----------------------------------------------------------------------------
//功能: 芯片擦除
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseChip(void)
{
    bool_t res = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
            if(SpiFlashWaitBusy(5000))
            {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
                //QSPI，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期
                res = QSPI_Send_CMD(CMD_SpiFlashChipErase,0,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
                // SPI
                struct SPI_DataFrame CommandFrame;
                u8 Command[1] = {CMD_SpiFlashChipErase};

                CommandFrame.RecvBuf = NULL;
                CommandFrame.RecvLen = 0;
                CommandFrame.RecvOff = 0;
                CommandFrame.SendBuf = Command;
                CommandFrame.SendLen = 1;

                if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
                {
                    if(CN_SPI_EXIT_NOERR ==
                            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                    {
                        res = true;
                    }

                    if (false == SPI_CsInactive(s_ptSpiPort))
                    {
                        error_printf("SpiFlash","spi cs inactive failed.\r\n");
                    }
                }
#endif
                if ((res) && (SpiFlashWaitBusy(500000)))
                {
                    res = true;
                }
            }
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 扇区擦除
//参数: addr：扇区地址
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseSector(u32 addr)
{
    bool_t res = false;
    addr *= 4096;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI，地址为addr，无数据，32位地址，4线传输地址，4线传输指令，无空周期
            res = QSPI_Send_CMD(CMD_SpiFlashSectorErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashSectorErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashSectorErase;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR ==
                        SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    res = true;
                }

                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 块区擦除
//参数: addr：扇区地址
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashEraseBlock(u32 addr)
{
    bool_t res = false;
    addr *= 65536;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI，地址为addr，无数据，32位地址，4线传输地址，4线传输指令，无空周期
            res = QSPI_Send_CMD(CMD_SpiFlashBlockErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashBlockErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashBlockErase;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR ==
                        SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    res = true;
                }

                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 读flash
//参数: buf：数据缓冲区，addr：读的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashRead(u8* buf,u32 addr,u32 len)
{
    bool_t ret = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWaitBusy(5000) == false)
        {
            ret = false;
        }
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
        //QSPI，快速读数据，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，2个空周期，
        if(QSPI_Send_CMD(CMD_SpiFlashFastReadData,addr,2,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
        {
            if(QSPI_Receive(buf, len))
            {
                ret = true;
            }
            else
            {
                ret = false;
            }
        }
#else
        struct SPI_DataFrame CommandFrame;
        u8 Command[5];

        if (Enable4B_AddrOpt)
        {
            Command[0] = CMD_SpiFlashReadData_4B_Addr;
            Command[1] = ((addr >> 24) & 0xFF);
            Command[2] = ((addr >> 16) & 0xFF);
            Command[3] = ((addr >> 8) & 0xFF);
            Command[4] = (addr & 0xFF);
            CommandFrame.RecvOff = 5;
            CommandFrame.SendLen = 5;
        }
        else
        {
            Command[0] = CMD_SpiFlashReadData;
            Command[1] = ((addr >> 16) & 0xFF);
            Command[2] = ((addr >> 8) & 0xFF);
            Command[3] = (addr & 0xFF);
            CommandFrame.RecvOff = 4;
            CommandFrame.SendLen = 4;
        }

        CommandFrame.RecvBuf = buf;
        CommandFrame.RecvLen = len;
        CommandFrame.SendBuf = Command;

        if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
        {
            if(CN_SPI_EXIT_NOERR ==
                    SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
            {
                ret = true;
            }

            if (false == SPI_CsInactive(s_ptSpiPort))
            {
                error_printf("SpiFlash","spi cs inactive failed.\r\n");
            }
        }

#endif
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写一页flash
//参数: buf：数据缓冲区，addr：写的地址，len：写的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWritePage(u8* buf,u32 addr,u32 len)
{
    bool_t res = false;
    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        if(SpiFlashWriteEnable())
        {
#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
            //QSPI，页写，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，0个空周期，
            if(QSPI_Send_CMD(CMD_SpiFlashPageProgram,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
            {
                res = QSPI_Transmit(buf, len);
            }
#else
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = CMD_SpiFlashPageProgram_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = CMD_SpiFlashPageProgram;
                Command[1] = ((addr >> 16) & 0xFF);
                Command[2] = ((addr >> 8) & 0xFF);
                Command[3] = (addr & 0xFF);
                CommandFrame.SendLen = 4;
            }

            CommandFrame.RecvBuf = NULL;
            CommandFrame.RecvLen = 0;
            CommandFrame.RecvOff = 0;
            CommandFrame.SendBuf = Command;

            if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
            {
                if(CN_SPI_EXIT_NOERR == SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                {
                    CommandFrame.RecvBuf = NULL;
                    CommandFrame.RecvLen = 0;
                    CommandFrame.RecvOff = 0;
                    CommandFrame.SendBuf = buf;
                    CommandFrame.SendLen = len;
                    if(CN_SPI_EXIT_NOERR == SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
                    {
                        res = true;
                    }
                }
                if (false == SPI_CsInactive(s_ptSpiPort))
                {
                    error_printf("SpiFlash","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 不带擦除功能的写，写之前需要确保所写的地址范围内的数据全为0xff，否则会写入失败
//参数: buf：数据缓冲区，addr：写的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashWriteNoErase(u8* buf,u32 addr,u32 len)
{
    u32 remain = 0;
    bool_t ret = false;

    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        remain = FlashPageSize - addr % FlashPageSize;  //单页还剩余的字节数
        if(len <= remain)
            remain = len;
        while(1)
        {
            if(SpiFlashWritePage(buf, addr, remain))
            {
                if(remain == len)
                {
                    ret = true;
                    break;
                }
                else
                {
                    buf += remain;
                    addr += remain;
                    len -= remain;
                    if(len > FlashPageSize)
                        remain = FlashPageSize;
                    else
                        remain = len;
                }
            }
            else
                break;

        }
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写flash
//参数: buf：数据缓冲区，addr：写的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
u8 SPI_FLASH_BUFFER[4096];
bool_t SpiFlashWrite(u8* buf,u32 addr,u32 len)
{
    u32 sec;
    u32 sec_off;
    u32 sec_remain;
    u32 i;
    bool_t ret = true;

    if(Lock_MutexPend(SpiFlashLock,CN_TIMEOUT_FOREVER))
    {
        sec = addr / 4096; //扇区地址
        sec_off = addr % 4096;//扇区偏移
        sec_remain = 4096- sec_off; //扇区剩余大小

        if(len <= sec_remain)
            sec_remain = len;
        while(1)
        {
            if(SpiFlashRead(SPI_FLASH_BUFFER, sec*4096, 4096))
            {
                for(i=0; i < sec_remain; i++)//
                {
                    if ((SPI_FLASH_BUFFER[sec_off + i] != 0XFF) && (SPI_FLASH_BUFFER[sec_off + i] != buf[i]))
                        break;
                }
                if(i < sec_remain)    //需要擦除
                {
                    if(SpiFlashEraseSector(sec))
                    {
                        for(i=0; i<sec_remain; i++)
                        {
                            SPI_FLASH_BUFFER[i + sec_off] = buf[i];
                        }
                        if(SpiFlashWriteNoErase(SPI_FLASH_BUFFER, sec*4096, 4096) == false)
                        {
                            ret = false;
                            break;
                        }
                    }
                    else
                    {
                        ret = false;
                        break;
                    }

                }
                else
                {
                    if(SpiFlashWriteNoErase(buf, addr, sec_remain) == false)
                    {
                        ret = false;
                        break;
                    }
                }
                if(len == sec_remain)
                    break;  //写完了
                else
                {
                    sec++;
                    sec_off=0;

                    buf += sec_remain;
                    addr += sec_remain;
                    len -= sec_remain;
                    if(len > 4096)
                        sec_remain=4096;
                    else
                        sec_remain=len;
                }
            }
            else
            {
                ret = false;
                break;
            }
        }
        Lock_MutexPost(SpiFlashLock);
    }
    return ret;
}

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
//-----------------------------------------------------------------------------
//功能: flash Qspi使能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashQspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(SpiFlashReadSR(2, &sta2) == false)       //读状态寄存器2的值
        return false;
    if((sta2 & 0X02)==0)           //QE位未使能
    {
        SpiFlashWriteEnable();      //写使能
        sta2 |= 1<<1;              //使能QE位
        if(SpiFlashWriteSR(2, sta2) == false) //写状态寄存器2
            ret = false;

        //写使能QSPI指令，地址为0，无数据，8位地址，无地址，单线传输指令，无空周期，0个字节数据
//        QSPI_Send_CMD(CMD_SpiFlashExitQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        if(QSPI_Send_CMD(CMD_SpiFlashEnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
        {
            if(SpiFlashReadSR(2, &sta2))       //读状态寄存器2的值
            {
                if((sta2 & 0X02)==0)           //QE位未使能
                    ret = false;
            }
            else
                ret = false;
        }
        else
            ret = false;
    }
    else
    {
        ret = QSPI_Send_CMD(CMD_SpiFlashEnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: SpiFlashQspi失能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashQspiDisable(void)
{
    //写失能QSPI指令，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
    if(QSPI_Send_CMD(CMD_SpiFlashExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        return true;
    }
    else
        return false;
}

#endif
//-----------------------------------------------------------------------------
//功能: SpiFlash初始化
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t SpiFlashInit(void)
{
    u32 chip_id;
    u32 i = 0;
    bool_t ret = false;

#if CFG_SPI_FLASH_QSPI_ENABLE == TRUE
    u8 temp;
    if (false == SpiFlashQspiEnable())
    {
        return false;
    }
#else
    s_ptSpiPort = SPI_DevAdd(CFG_SPI_FLASH_SPI_NAME,SpiFlashName,0,8,SPI_MODE_0,SPI_SHIFT_MSB,1000*1000,false);
    if(s_ptSpiPort)
    {
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_POLL,0,0);
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_CLK, 30 * 1000 * 1000, 0);
    }
    else
    {
        error_printf("SpiFlash","Add spi dev failed.\n\r");
        return false;
    }
#endif

    chip_id = SpiFlashReadID();

    for (i = 0; i < sizeof(FlashInfo) / sizeof(struct SPI_FlashInfo); i++)
    {
        if (chip_id == FlashInfo[i].ChipID)
        {
            if (FlashInfo[i].Capacity > (16 * 1024 * 1024))
            {
                Enable4B_AddrOpt = true;    //超过16M的话，3字节地址就不能访问了，要用4字节地址命令操作
            }
            FlashPageSize = FlashInfo[i].PageSize;
            FlashPageInSector = FlashInfo[i].PageNumInSector;
            FlashSectorInBolck = FlashInfo[i].SectorNumInBlock;
            FlashSectorNum = FlashInfo[i].Capacity / (FlashPageSize * FlashPageInSector);
            ret = true;
            break;
        }
    }
    
 #if CFG_SPI_FLASH_QSPI_ENABLE == TRUE       
    if(SpiFlashReadSR(3, &temp))      //读状态寄存器3
    {
        if((temp & 0X01) == 0)          //判断当前是否为4地址模式，0是3地址模式，1是4地址模式
        {
            if(SpiFlashWriteEnable())  //写使能
            {
                //写使能4字节地址指令，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
                if(QSPI_Send_CMD(SpiFlashEnable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
                    return ret;
            }
            else
                return ret;
        }
        if(SpiFlashWriteEnable())
        {
            //写设置读参数指令，地址为0，4线传输数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
            if(QSPI_Send_CMD(SpiFlashSetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES))
            {
                temp = 0<<4;                  //设置P4&P5=00,2个dummy clocks,44M
                if(QSPI_Transmit(&temp,1))
                    ret = true;
            }
        }
    }
#endif
    return ret;
}


// ============================================================================
// 功能：QFLASH 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：0 -- 成功；-1 -- 失败
// 备注：
// ============================================================================
s32 __SpiFlashReq(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock://本页在第几扇区
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / SpiFlashdescription->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  SpiFlashdescription->BlockNum;
            break;
        }
        case totalSectors:
        {
            *((u32*)args) =  SpiFlashdescription->SectorNum;
            break;
        }

        case sectorsunits:
        {
            // args = &units
            *((u32*)args) = SpiFlashdescription->PagesPerSector;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = SpiFlashdescription->PagesPerBlock;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = SpiFlashdescription->BytesPerPage;
            break;
        }

        case format:
        {
            va_list list;
            u32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(-1 == (s32)end)
                end = SpiFlashdescription->SectorNum;
// else if (start)
//     end += start;

            do
            {
                if(SpiFlashEraseBlock(--end) == false)
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);
            break;
        }
        default: res = -1;
        break;
    }

    return (res);
}

//-----------------------------------------------------------------------------
//功能: Nor模块安装
//参数:
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_SpiFlash(void)
{
    SpiFlashLock = Lock_MutexCreate("spi flash lock");
    if(!SpiFlashLock)
    {
        error_printf("SpiFlash","MODULE INSTALL : error : cannot create spi flash lock.");
        return false;
    }

    if(SpiFlashInit() == false)
    {
        error_printf("SpiFlash","Install fail.\r\n");
        return false;
    }

    if(!SpiFlashdescription)
    {
        SpiFlashdescription = malloc(sizeof(struct NorDescr));
        if(!SpiFlashdescription)
        {
            error_printf("SpiFlash","erro : device : memory out.\r\n");
            return false;
        }
        memset(SpiFlashdescription, 0x0, (sizeof(struct NorDescr)));
        SpiFlashdescription->PortType = NOR_SPI;
        SpiFlashdescription->Port = NULL;
        SpiFlashdescription->BytesPerPage = FlashPageSize;
        SpiFlashdescription->SectorNum = FlashSectorNum;
        SpiFlashdescription->BlockNum = FlashSectorNum / FlashSectorInBolck;
        SpiFlashdescription->BlocksPerSector = 0;
        SpiFlashdescription->PagesPerSector = FlashPageInSector;
        SpiFlashdescription->SectorsPerBlock = FlashSectorInBolck;
        SpiFlashdescription->PagesPerBlock = FlashPageInSector * FlashSectorInBolck;
        SpiFlashdescription->ReservedBlks = 0;
    }

    if(CFG_SPI_FLASH_PART_ERASE)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __SpiFlashReq(format, 0 , -1, &sz))
        {
            warning_printf("SpiFlash"," Format failure.\r\n");
        }
    }

    SpiFlashUm = malloc(sizeof(struct umedia));
    if(!SpiFlashUm)
    {
        return (-1);
    }
    SpiFlashUm->mreq = __SpiFlashReq;
    SpiFlashUm->type = nor;
    SpiFlashUm->ubuf = NULL;

    if(!Device_Create((const char*)SpiFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)SpiFlashUm)))
    {
        printf("\r\n: erro : device : %s addition failed.", SpiFlashName);
        free(SpiFlashUm);
        return (-1);
    }

    SpiFlashInitFlag = true;

    return true;

}

// =============================================================================
// 功能：判断flash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t SpiFlashIsInstall(void)
{
    return SpiFlashInitFlag;
}

// ============================================================================
// 功能：初始化片内flash
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __SpiFlashFsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
     u32 units, total = 0,endblock = bend;
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("embed"," not found need to install file system.\r\n");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = SpiFlashUm;
    super->MediaDrv = mediadrv;

     if(-1 == (s32)endblock)
     {
         endblock = bend = SpiFlashdescription->BlockNum; // 最大块号
     }

     do
     {
         if(__SpiFlashReq(blockunits, (ptu32_t)&units, --endblock))
         {
             return (-1);
         }

         total += units;
     }
     while(endblock!=bstart);

    super->AreaSize = SpiFlashdescription->BytesPerPage * total;
    // endblock = 0;
    // total = 0;
    // while(endblock<bstart)
    // {
    //     if(__embed_req(blockunits, (ptu32_t)&units, endblock++))
    //     {
    //         return (-1);
    //     }
    //     total += units;
    // }

    super->MediaStart = bstart * SpiFlashdescription->PagesPerBlock; // 起始unit号

    res = strlen(SpiFlashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,SpiFlashName);   //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}

//-----------------------------------------------------------------------------
//功能: 驱动测试
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void SpiFlashLocalTest(void)
{
    u32 i, j, k;
    u16 temp;
    u8 WriteBuf[FlashPageSize];
    u8 ReadBuf[FlashPageSize];
    temp = SpiFlashReadID();
    debug_printf("SpiFlash"," chip id is 0x%4x.\r\n", temp);

    for(i=0; i<SpiFlashdescription->BytesPerPage; i++)
    {
        WriteBuf[i] = i;
    }
    // 测读写块擦除
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<SpiFlashdescription->BlockNum; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }


            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                if (false == SpiFlashWritePage(WriteBuf, 
                        (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
            if (false == SpiFlashEraseBlock(k))
            {
                error_printf("SpiFlash"," erase block %d failed.\r\n", k);
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
        }
    }

    // 先写满flash
    for(k=0; k<SpiFlashdescription->BlockNum; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
            }
        }
    }

    // 测整片擦除
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<SpiFlashdescription->BlockNum; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

        }
    }

    // 测扇区擦除
    for(k=0; k<SpiFlashdescription->SectorNum; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write sector %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
        if (false == SpiFlashEraseSector(k))
        {
            error_printf("SpiFlash"," erase block %d failed.\r\n", k);
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
    }

    // 擦除芯片后写满2个块
    if(false == SpiFlashEraseChip())
    {
        error_printf("SpiFlash"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<2; k++)
        {
            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }


            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                if (false == SpiFlashWritePage(WriteBuf, 
                        (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }

            for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerBlock)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerBlock))));
                }
            }
        }
        // 擦除第一个块
        if (false == SpiFlashEraseBlock(0))
        {
            error_printf("SpiFlash"," erase block %d failed.\r\n", 0);
        }
        // 第一个块是全ff
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, i*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", i);
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", i);
            }
        }
        // 第二个块还有数据
        for(i=0; i<(SpiFlashdescription->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+SpiFlashdescription->PagesPerBlock)*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+SpiFlashdescription->PagesPerBlock));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+SpiFlashdescription->PagesPerBlock));
            }
        }
    }

    // 测写2个扇区擦一个扇区
    for(k=0; k<2; k++)
    {
        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("SpiFlash"," page erase %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }


        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            if (false == SpiFlashWritePage(WriteBuf, 
                    (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("SpiFlash"," write block %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }

        for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (SpiFlashRead(ReadBuf, (i+(k*(SpiFlashdescription->PagesPerSector)))*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("SpiFlash"," page %d data error.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("SpiFlash"," read page %d failed.\r\n", (i+(k*(SpiFlashdescription->PagesPerSector))));
            }
        }
    }
    // 擦除第一个块
    if (false == SpiFlashEraseSector(0))
    {
        error_printf("SpiFlash"," erase block %d failed.\r\n", 0);
    }
    // 第一个块是全ff
    for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (SpiFlashRead(ReadBuf, i*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
            {
                if(ReadBuf[j] != 0xFF)
                {
                    error_printf("SpiFlash"," page erase %d failed.\r\n", i);
                    break;
                }
            }
        }
        else
        {
            error_printf("SpiFlash"," read page %d failed.\r\n", i);
        }
    }
    // 第二个扇区还有数据
    for(i=0; i<(SpiFlashdescription->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (SpiFlashRead(ReadBuf, (i+SpiFlashdescription->PagesPerSector)*SpiFlashdescription->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<SpiFlashdescription->BytesPerPage; j++)
            {
                if(ReadBuf[j] != WriteBuf[j])
                {
                    error_printf("SpiFlash"," page %d data error.\r\n", (i+SpiFlashdescription->PagesPerSector));
                    break;
                }
            }
        }
        else
        {
            error_printf("SpiFlash"," read page %d failed.\r\n", (i+SpiFlashdescription->PagesPerSector));
        }
    }

}




