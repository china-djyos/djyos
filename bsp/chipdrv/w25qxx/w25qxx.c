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
#include "w25qxx.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_W25qxx(void);
//    ModuleInstall_W25qxx();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"norflash W25QXX"//SPI和QSPI接口的nor flash芯片
//parent:"none"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none",//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"cpu onchip spi"       //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NORFLASH_W25QXX == false )
//#warning  " norflash_W25QXX  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NORFLASH_W25QXX    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_W25QXX_PART_ERASE               false      //"分区选项,是否需要擦除该芯片。"
#define CFG_W25QXX_QSPI_ENABLE              false      //"是否使用QSPI模式"
//%$#@num,0,0x7fffffff,
#define CFG_W25QXX_SPI_CLK                  100000000    //"SPI时钟频率"
//%$#@num,0,65535,
#define CFG_W25QXX_BYTES_PAGE               256        //"每页的字节数"
#define CFG_W25QXX_PAGES_SECTOR             16         //"每个扇区有多少页"
#define CFG_W25QXX_SECTORS_BLOCK            16         //"每个块有多少扇区"
#define CFG_W25QXX_SECTORS_NUM              2048       //"总的扇区数"
#define CFG_W25QXX_CHIP_ID                  0xEF18     //"芯片ID"
//%$#@string,1,10,
#define CFG_W25_SPI_NAME                    "SPI1"      //"用的SPI设备名字，spi模式下有效"
//%$#@string,1,20,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
#define CFG_W25QXX_SPI_CLK                  100000000    //"SPI时钟频率"

#if CFG_W25QXX_QSPI_ENABLE == FALSE
static struct SPI_Device *s_ptSpiPort; // 器件使用的SPI端口
#endif
struct MutexLCB *W25qxx_Lock;           //芯片互斥访问保护
struct NorDescr *W25qxx_description = NULL;
bool_t W25qxx_InitFlag = false;
const char *W25qFlashName = "w25qxx";      //该flash在obj在的名字
struct umedia *w25q_flash_um;
extern struct Object *s_ptDeviceRoot;
static bool_t Enable4B_AddrOpt = false;
//-----------------------------------------------------------------------------
//功能: 读状态寄存器
//参数: regno：状态寄存器序号
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_ReadSR(u8 regno, u8 *sta)
{
    u8 Command[1];
    bool_t ret = false;
    switch(regno)
    {
        case 1:
            Command[0] = W25X_ReadStatusReg1;    //读状态寄存器1
            break;
        case 2:
            Command[0] = W25X_ReadStatusReg2;    //读状态寄存器2
            break;
        case 3:
            Command[0] = W25X_ReadStatusReg3;    //读状态寄存器3
            break;
        default:
            Command[0] = W25X_ReadStatusReg1;
            break;
    }
#if CFG_W25QXX_QSPI_ENABLE == TRUE
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
            error_printf("W25q","spi cs inactive failed.\r\n");
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
bool_t W25QXX_WaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(W25QXX_ReadSR(1, &sta) == false)
    {
        return false;
    }
    while((sta & 0x01) == 0x01)   //等待空闲
    {
        if(W25QXX_ReadSR(1, &sta) == false)
            return false;
        DJY_EventDelay(1000);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("W25q","Wait busy timeput.\r\n");
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
bool_t W25QXX_WriteSR(u8 regno, u8 data)
{
    bool_t ret = false;
    u8 Command[2];
    switch(regno)
    {
        case 1:
            Command[0] = W25X_WriteStatusReg1;    //写状态寄存器1
            break;
        case 2:
            Command[0] = W25X_WriteStatusReg2;    //写状态寄存器2
            break;
        case 3:
            Command[0] = W25X_WriteStatusReg3;    //写状态寄存器3
            break;
        default:
            Command[0] = W25X_WriteStatusReg1;
            break;
    }

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_W25QXX_QSPI_ENABLE == TRUE
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
            error_printf("W25q","spi cs inactive failed.\r\n");
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
bool_t W25QXX_WriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(W25QXX_ReadSR(1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            W25QXX_ReadSR(1, &sta);
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
bool_t W25QXX_WriteEnable(void)
{
    
    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_W25QXX_QSPI_ENABLE == TRUE
    //QSPI，地址位0，无数据，8位地址，无地址，4线传输指令，无空周期，0字节数据
    if(QSPI_Send_CMD(W25X_WriteEnable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
    {
        return false;
    }
#else
    //SPI，
    struct SPI_DataFrame CommandFrame;
    bool_t Ret = false;
    u8 Command[1] = {W25X_WriteEnable};

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
            error_printf("W25q","spi cs inactive failed.\r\n");
        }
    }

    if (false == Ret)
    {
        return false;
    }
#endif
    return W25QXX_WriteEnableWait();
}

//-----------------------------------------------------------------------------
//功能: 写失能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteDisable(void)
{
    bool_t ret = false;

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_W25QXX_QSPI_ENABLE == TRUE
    //QSPI，地址位0，无数据，8位地址，无地址，4线传输指令，无空周期，0字节数据
    ret = QSPI_Send_CMD(W25X_WriteDisable,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command[1] = {W25X_WriteDisable};

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
            error_printf("W25q","spi cs inactive failed.\r\n");
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
u16 W25QXX_ReadID(void)
{
    u8 temp[2];
    u16 deviceid;
    bool_t ret = false;

    if(W25QXX_WaitBusy(5000) == false)
    {
        return false;
    }

#if CFG_W25QXX_QSPI_ENABLE == TRUE
    //QSPI，地址位0，4线传输数据，24位地址，4线传输地址，4线传输指令，无空周期，2字节数据
    ret = QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);

    if(ret)
    {
        QSPI_Receive(temp,2);
        deviceid=(temp[0] << 8) | temp[1];
    }
    else
    {
        deviceid = 0;
    }
#else
    //SPI
    struct SPI_DataFrame CommandFrame;
    u8 Command[4];

    Command[0] = W25X_ManufactDeviceID;
    Command[1] = 0x0;
    Command[2] = 0x0;
    Command[3] = 0x0;

    CommandFrame.RecvBuf = temp;
    CommandFrame.RecvLen = 2;
    CommandFrame.RecvOff = 4;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    if (SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER))
    {
        if(CN_SPI_EXIT_NOERR ==
                SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
        {
            deviceid=(temp[0] << 8) | temp[1];
        }

        if (false == SPI_CsInactive(s_ptSpiPort))
        {
            error_printf("W25q","spi cs inactive failed.\r\n");
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
bool_t W25QXX_EraseChip(void)
{
    bool_t res = false;
    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        if(W25QXX_WriteEnable())
        {
            if(W25QXX_WaitBusy(5000))
            {
#if CFG_W25QXX_QSPI_ENABLE == TRUE
                //QSPI，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期
                res = QSPI_Send_CMD(W25X_ChipErase,0,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
#else
                // SPI
                struct SPI_DataFrame CommandFrame;
                u8 Command[1] = {W25X_ChipErase};

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
                        error_printf("W25q","spi cs inactive failed.\r\n");
                    }
                }
#endif
                if ((res) && (W25QXX_WaitBusy(500000)))
                {
                    res = true;
                }
            }
        }
        Lock_MutexPost(W25qxx_Lock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 扇区擦除
//参数: addr：扇区地址
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_EraseSector(u32 addr)
{
    bool_t res = false;
    addr *= 4096;
    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        if(W25QXX_WriteEnable())
        {
#if CFG_W25QXX_QSPI_ENABLE == TRUE
            //QSPI，地址为addr，无数据，32位地址，4线传输地址，4线传输指令，无空周期
            res = QSPI_Send_CMD(W25X_SectorErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                    QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = W25X_SectorErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = W25X_SectorErase;
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
                    error_printf("W25q","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(W25qxx_Lock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 块区擦除
//参数: addr：扇区地址
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_EraseBlock(u32 addr)
{
    bool_t res = false;
    addr *= 65536;
    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        if(W25QXX_WriteEnable())
        {
#if CFG_W25QXX_QSPI_ENABLE == TRUE
            //QSPI，地址为addr，无数据，32位地址，4线传输地址，4线传输指令，无空周期
            res = QSPI_Send_CMD(W25X_BlockErase,addr,0,QSPI_INSTRUCTION_4_LINES,
                                QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_NONE);
#else
            // SPI
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = W25X_BlockErase_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = W25X_BlockErase;
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
                    error_printf("W25q","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(W25qxx_Lock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 读flash
//参数: buf：数据缓冲区，addr：读的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_Read(u8* buf,u32 addr,u32 len)
{
    bool_t ret = false;
    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        if(W25QXX_WaitBusy(5000) == false)
        {
            ret = false;
        }
#if CFG_W25QXX_QSPI_ENABLE == TRUE
        //QSPI，快速读数据，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，2个空周期，
        if(QSPI_Send_CMD(W25X_FastReadData,addr,2,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
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
            Command[0] = W25X_ReadData_4B_Addr;
            Command[1] = ((addr >> 24) & 0xFF);
            Command[2] = ((addr >> 16) & 0xFF);
            Command[3] = ((addr >> 8) & 0xFF);
            Command[4] = (addr & 0xFF);
            CommandFrame.RecvOff = 5;
            CommandFrame.SendLen = 5;
        }
        else
        {
            Command[0] = W25X_ReadData;
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
                error_printf("W25q","spi cs inactive failed.\r\n");
            }
        }

#endif
        Lock_MutexPost(W25qxx_Lock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写一页flash
//参数: buf：数据缓冲区，addr：写的地址，len：写的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_WritePage(u8* buf,u32 addr,u32 len)
{
    bool_t res = false;
    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        if(W25QXX_WriteEnable())
        {
#if CFG_W25QXX_QSPI_ENABLE == TRUE
            //QSPI，页写，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，0个空周期，
            if(QSPI_Send_CMD(W25X_PageProgram,addr,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES))
            {
                res = QSPI_Transmit(buf, len);
            }
#else
            struct SPI_DataFrame CommandFrame;
            u8 Command[5];

            if (Enable4B_AddrOpt)
            {
                Command[0] = W25X_PageProgram_4B_Addr;
                Command[1] = ((addr >> 24) & 0xFF);
                Command[2] = ((addr >> 16) & 0xFF);
                Command[3] = ((addr >> 8) & 0xFF);
                Command[4] = (addr & 0xFF);
                CommandFrame.SendLen = 5;
            }
            else
            {
                Command[0] = W25X_PageProgram;
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
                    error_printf("W25q","spi cs inactive failed.\r\n");
                }
            }
#endif
        }
        Lock_MutexPost(W25qxx_Lock);
    }
    return res;
}

//-----------------------------------------------------------------------------
//功能: 不带擦除功能的写，写之前需要确保所写的地址范围内的数据全为0xff，否则会写入失败
//参数: buf：数据缓冲区，addr：写的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_WriteNoErase(u8* buf,u32 addr,u32 len)
{
    u32 remain = 0;
    bool_t ret = false;

    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        remain = CFG_W25QXX_BYTES_PAGE - addr % CFG_W25QXX_BYTES_PAGE;  //单页还剩余的字节数
        if(len <= remain)
            remain = len;
        while(1)
        {
            if(W25QXX_WritePage(buf, addr, remain))
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
                    if(len > CFG_W25QXX_BYTES_PAGE)
                        remain = CFG_W25QXX_BYTES_PAGE;
                    else
                        remain = len;
                }
            }
            else
                break;

        }
        Lock_MutexPost(W25qxx_Lock);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写flash
//参数: buf：数据缓冲区，addr：写的地址，len：读取的字节数
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
u8 W25QXX_BUFFER[4096];
bool_t W25QXX_Write(u8* buf,u32 addr,u32 len)
{
    u32 sec;
    u32 sec_off;
    u32 sec_remain;
    u32 i;
    bool_t ret = true;

    if(Lock_MutexPend(W25qxx_Lock,CN_TIMEOUT_FOREVER))
    {
        sec = addr / 4096; //扇区地址
        sec_off = addr % 4096;//扇区偏移
        sec_remain = 4096- sec_off; //扇区剩余大小

        if(len <= sec_remain)
            sec_remain = len;
        while(1)
        {
            if(W25QXX_Read(W25QXX_BUFFER, sec*4096, 4096))
            {
                for(i=0; i < sec_remain; i++)//
                {
                    if ((W25QXX_BUFFER[sec_off + i] != 0XFF) && (W25QXX_BUFFER[sec_off + i] != buf[i]))
                        break;
                }
                if(i < sec_remain)    //需要擦除
                {
                    if(W25QXX_EraseSector(sec))
                    {
                        for(i=0; i<sec_remain; i++)
                        {
                            W25QXX_BUFFER[i + sec_off] = buf[i];
                        }
                        if(W25QXX_WriteNoErase(W25QXX_BUFFER, sec*4096, 4096) == false)
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
                    if(W25QXX_WriteNoErase(buf, addr, sec_remain) == false)
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
        Lock_MutexPost(W25qxx_Lock);
    }
    return ret;
}

#if CFG_W25QXX_QSPI_ENABLE == TRUE
//-----------------------------------------------------------------------------
//功能: W25QXX_Qspi使能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_QspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(W25QXX_ReadSR(2, &sta2) == false)       //读状态寄存器2的值
        return false;
    if((sta2 & 0X02)==0)           //QE位未使能
    {
        W25QXX_WriteEnable();      //写使能
        sta2 |= 1<<1;              //使能QE位
        if(W25QXX_WriteSR(2, sta2) == false) //写状态寄存器2
            ret = false;

        //写使能QSPI指令，地址为0，无数据，8位地址，无地址，单线传输指令，无空周期，0个字节数据
//        QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
        if(QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
        {
            if(W25QXX_ReadSR(2, &sta2))       //读状态寄存器2的值
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
        ret = QSPI_Send_CMD(W25X_EnterQPIMode,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE);
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: W25QXX_Qspi失能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_QspiDisable(void)
{
    //写失能QSPI指令，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
    if(QSPI_Send_CMD(W25X_ExitQPIMode,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        return true;
    }
    else
        return false;
}

#endif
//-----------------------------------------------------------------------------
//功能: W25QXX初始化
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t W25QXX_Init(void)
{
    u16 chip_id;
    bool_t ret = false;

#if CFG_W25QXX_QSPI_ENABLE == TRUE
    u8 temp;
    if (false == W25QXX_QspiEnable())
    {
        return false;
    }
#else
    s_ptSpiPort = SPI_DevAdd(CFG_W25_SPI_NAME,"w25qxx",0,8,SPI_MODE_0,SPI_SHIFT_MSB,CFG_W25QXX_SPI_CLK,false);
    if(s_ptSpiPort)
    {
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_POLL,0,0);
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_CLK, CFG_W25QXX_SPI_CLK, 0);
    }
    else
    {
        error_printf("W25q","Add spi dev failed.\n\r");
        return false;
    }
#endif

    chip_id = W25QXX_ReadID();
    if(chip_id == CFG_W25QXX_CHIP_ID)
    {
        if (chip_id == 0xEF18)
        {
            Enable4B_AddrOpt = true;
        }

 #if CFG_W25QXX_QSPI_ENABLE == TRUE       
        if(W25QXX_ReadSR(3, &temp))      //读状态寄存器3
        {
            if((temp & 0X01) == 0)          //判断当前是否为4地址模式，0是3地址模式，1是4地址模式
            {
                if(W25QXX_WriteEnable())  //写使能
                {
                    //写使能4字节地址指令，地址为0，无数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
                    if(QSPI_Send_CMD(W25X_Enable4ByteAddr,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
                        return ret;
                }
                else
                    return ret;
            }
            if(W25QXX_WriteEnable())
            {
                //写设置读参数指令，地址为0，4线传输数据，8位地址，无地址，4线传输指令，无空周期，0个字节数据
                if(QSPI_Send_CMD(W25X_SetReadParam,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_4_LINES))
                {
                    temp = 0<<4;                  //设置P4&P5=00,2个dummy clocks,44M
                    if(QSPI_Transmit(&temp,1))
                        ret = true;
                }
            }
        }
#else
        ret = true;
#endif
    }
    else
    {
        error_printf("W25q","chip ID error.\r\n");
    }
    return ret;
}


// ============================================================================
// 功能：QFLASH 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：0 -- 成功；-1 -- 失败
// 备注：
// ============================================================================
s32 __W25qxx_Req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = *unit / W25qxx_description->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  W25qxx_description->BlockNum;
            break;
        }
        case totalSectors:
        {
            *((u32*)args) =  W25qxx_description->SectorNum;
            break;
        }


        case blockunits:
        {
            // args = &units
            *((u32*)args) = W25qxx_description->PagesPerBlock;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = W25qxx_description->BytesPerPage;
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
                end = W25qxx_description->SectorNum;
// else if (start)
//     end += start;

            do
            {
                if(W25QXX_EraseBlock(--end) == false)
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
s32 ModuleInstall_W25qxx(void)
{
    W25qxx_Lock = Lock_MutexCreate("gd25q16c Lock");
    if(!W25qxx_Lock)
    {
        error_printf("W25q","MODULE INSTALL : error : cannot create w25qxx flash lock.");
        return false;
    }

    if(W25QXX_Init() == false)
    {
        error_printf("W25q","Install fail.\r\n");
        return false;
    }

    if(!W25qxx_description)
    {
        W25qxx_description = malloc(sizeof(struct NorDescr));
        if(!W25qxx_description)
        {
            error_printf("W25q","erro : device : memory out.\r\n");
            return false;
        }
        memset(W25qxx_description, 0x0, (sizeof(struct NorDescr)));
        W25qxx_description->PortType = NOR_SPI;
        W25qxx_description->Port = NULL;
        W25qxx_description->BytesPerPage = CFG_W25QXX_BYTES_PAGE;
        W25qxx_description->SectorNum = CFG_W25QXX_SECTORS_NUM;
        W25qxx_description->BlockNum = CFG_W25QXX_SECTORS_NUM / CFG_W25QXX_SECTORS_BLOCK;
        W25qxx_description->BlocksPerSector = 0;
        W25qxx_description->PagesPerSector = CFG_W25QXX_PAGES_SECTOR;
        W25qxx_description->SectorsPerBlock = CFG_W25QXX_SECTORS_BLOCK;
        W25qxx_description->PagesPerBlock = CFG_W25QXX_PAGES_SECTOR * CFG_W25QXX_SECTORS_BLOCK;
        W25qxx_description->ReservedBlks = 0;
    }

    if(CFG_W25QXX_PART_ERASE)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __W25qxx_Req(format, 0 , -1, &sz))
        {
            warning_printf("W25q"," Format failure.\r\n");
        }
    }

    w25q_flash_um = malloc(sizeof(struct umedia));
    if(!w25q_flash_um)
    {
        return (-1);
    }
    w25q_flash_um->mreq = __W25qxx_Req;
    w25q_flash_um->type = nor;
    w25q_flash_um->ubuf = NULL;

    if(!Device_Create((const char*)W25qFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)w25q_flash_um)))
    {
        printf("\r\n: erro : device : %s addition failed.", W25qFlashName);
        free(w25q_flash_um);
        return (-1);
    }

    W25qxx_InitFlag = true;

    return true;

}

// =============================================================================
// 功能：判断w25q是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t W25qxx_is_install(void)
{
    return W25qxx_InitFlag;
}

// ============================================================================
// 功能：初始化片内flash
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __w25qxx_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
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
    super->MediaInfo = w25q_flash_um;
    super->MediaDrv = mediadrv;

     if(-1 == (s32)endblock)
     {
         endblock = bend = W25qxx_description->BlockNum; // 最大块号
     }

     do
     {
         if(__W25qxx_Req(blockunits, (ptu32_t)&units, --endblock))
         {
             return (-1);
         }

         total += units;
     }
     while(endblock!=bstart);

    super->AreaSize = W25qxx_description->BytesPerPage * total;
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

    super->MediaStart = bstart * W25qxx_description->PagesPerBlock; // 起始unit号

    res = strlen(W25qFlashName) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,W25qFlashName);   //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);

}



#if 0   //注释掉的是SPI访问芯片的方式，代码太老了，以后要用这种方式的时候再整理
//
//  开发板源码,BITBAND逻辑
/*
#define BITBAND(addr, bitnum)         ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)              *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)       MEM_ADDR(BITBAND(addr, bitnum))
#define GPIOA_ODR_Addr                (GPIOA_BASE+20)
#define GPIOB_ODR_Addr                (GPIOB_BASE+20) //0x40020414
#define PAout(n)                       BIT_ADDR(GPIOA_ODR_Addr,n)  //输出
#define PBout(n)                       BIT_ADDR(GPIOB_ODR_Addr,n)  //输出
*/
static struct SPI_Device *s_ptSpiPort; // 器件使用的SPI端口
static struct FlashChip *s_pChip; // 测试用

//-----------------------------------------------------------------------------
//功能: Nor专用逻辑，写使能
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
static s32 __WriteEnable(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0x06};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: 获取Nor状态
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
static s32 __Status1(u8 *Status)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0x05};

    CommandFrame.RecvBuf = Status;
    CommandFrame.RecvLen = 1;
    CommandFrame.RecvOff = 1;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: 检查操作是否完成
//参数:
//返回:  -1 -- 未完成; 0 -- 完成;
//备注:
//-----------------------------------------------------------------------------
static s32 __Done(void)
{
    u8 Status;

    if(__Status1(&Status))
        return ( -1);

    if(Status & 0x1)
        return (-1);

    return (0);
}

//-----------------------------------------------------------------------------
//功能: 获取Nor ID信息
//参数:
//返回: 非零 -- ID信息; 0 -- 失败;
//备注:
//-----------------------------------------------------------------------------
static u16 __ID(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u16 ID;
    u8 Command[4];

    Command[0] = 0x90;
    Command[1] = 0x0;
    Command[2] = 0x0;
    Command[3] = 0x0;

    CommandFrame.RecvBuf = (u8*)(&ID);
    CommandFrame.RecvLen = 2;
    CommandFrame.RecvOff = 4;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
    if(CN_SPI_EXIT_NOERR != Ret)
    {
        ID = 0;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (ID);
}

//-----------------------------------------------------------------------------
//功能: 擦除一个sector
//参数: SectorNo -- 扇号
//返回: 0 -- 成功; 1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 w25qxx_SectorErase(u32 SectorNo)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[4];

    SectorNo = (SectorNo << 4); // SectorNo转PageNo
    Command[0] = 0x20;
    Command[1] = ((SectorNo >> 8) & 0xFF);
    Command[2] = (SectorNo & 0xFF);
    Command[3] = 0x0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    DJY_EventDelay(400000);// 切出//延时切出.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}
//-----------------------------------------------------------------------------
//功能: 擦除一个block
//参数: BlkNo -- 块号
//返回: 0 -- 成功; 1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 w25qxx_BlkErase(u32 BlkNo)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[4];

    Command[0] = 0xD8;
    Command[1] = (BlkNo & 0xFF);
    Command[2] = 0x0;
    Command[3] = 0x0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    DJY_EventDelay(1000000);// 切出//延时切出.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: 擦除整个芯片
//参数:
//返回: 0 -- 成功; 1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 w25qxx_ChipErase(void)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = 0;
    u8 Command[1] = {0xC7};

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 1;

    if(__WriteEnable())
        return (-1);

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR !=
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = -1;
    }

    SPI_CsInactive(s_ptSpiPort);

    DJY_EventDelay(40000000);// 切出//延时切出.

    if((0 == Ret) && (__Done()))
    {
        Ret = -1;
    }

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: 读一页
//参数: PageNo -- 页号;
//      Data -- 读缓冲;
//      Flags -- 保留;
//返回: >0 -- 读出字节数; -1 -- 参数错误; -2 -- 读失败
//备注:
//-----------------------------------------------------------------------------
s32 w25qxx_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret = -2;
    u8 Command[4];

    if(NULL == Data)
        return (-1);

    Command[0] = 0x03;
    Command[1] = ((PageNo >> 8) & 0xFF);
    Command[2] = (PageNo & 0xFF);
    Command[3] = 0;

    CommandFrame.RecvBuf = Data;
    CommandFrame.RecvLen = 256;
    CommandFrame.RecvOff = 4;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);

    if(CN_SPI_EXIT_NOERR ==
            SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER))
    {
        Ret = 256;
    }

    SPI_CsInactive(s_ptSpiPort);

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: 写一页
//参数: PageNo -- 页号;
//      Data -- 写缓冲;
//      Flags -- 保留;
//返回: >0 -- 写入字节数; -1 -- 参数错误; -2 -- 写入失败;
//备注:
//-----------------------------------------------------------------------------
s32 w25qxx_PageWrite(u32 PageNo, u8 *Data, u32 Flags)
{
    struct SPI_DataFrame CommandFrame;
    s32 Ret;
    u8 Command[4];

    if(NULL == Data)
        return (-1);

    if(__WriteEnable())
        return (-2);

    Command[0] = 0x02;
    Command[1] = ((PageNo >> 8) & 0xFF);
    Command[2] = (PageNo & 0xFF);
    Command[3] = 0;

    CommandFrame.RecvBuf = NULL;
    CommandFrame.RecvLen = 0;
    CommandFrame.RecvOff = 0;
    CommandFrame.SendBuf = Command;
    CommandFrame.SendLen = 4;

    SPI_CsActive(s_ptSpiPort, CN_TIMEOUT_FOREVER);
    Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
    if(CN_SPI_EXIT_NOERR == Ret)
    {
        CommandFrame.RecvBuf = NULL;
        CommandFrame.RecvLen = 0;
        CommandFrame.RecvOff = 0;
        CommandFrame.SendBuf = Data;
        CommandFrame.SendLen = 256;
        Ret = SPI_Transfer(s_ptSpiPort, &CommandFrame, TRUE, CN_TIMEOUT_FOREVER);
        if(CN_SPI_EXIT_NOERR == Ret)
        {
            Ret = 256;
        }
        else
        {
            Ret = -2;
        }
    }
    else
    {
        Ret = -2;
    }

    SPI_CsInactive(s_ptSpiPort);

    DJY_EventDelay(3000);// 延时切出.

    if(__Done())
        Ret = -2;// 未完成,写失败

    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: Nor设备初始化
//参数: Nor -- 设备结构体
//返回: -1 -- 失败; 0 -- 成功;
//备注:
//-----------------------------------------------------------------------------

s32 Nor_Init(struct FlashChip *Nor)
{
    struct SPI_Device *SpiPort;


    if(s_ptSpiPort = SPI_DevAdd("SPI1","w25qxx",0,8,SPI_MODE_0,SPI_SHIFT_MSB,1000*1000,false))
    {
        SPI_BusCtrl(s_ptSpiPort,CN_SPI_SET_POLL,0,0);
    }
    else
    {
        printf("w25qxx init failed.\n\r");
        return false;
    }

    Nor->Descr.Nor.PortType = NOR_SPI;
    Nor->Descr.Nor.Port = (void*)s_ptSpiPort;

    Nor->Descr.Nor.BytesPerPage = 256;
    Nor->Descr.Nor.PagesPerSector = 16;
    Nor->Descr.Nor.SectorsPerBlk = 16;
    Nor->Descr.Nor.Blks = 256;

    Nor->Ops.RdPage = w25qxx_PageRead;
    Nor->Ops.WrPage = w25qxx_PageWrite;
    Nor->Ops.ErsBlk = w25qxx_BlkErase;
    Nor->Ops.Special.Nor.ErsChip = w25qxx_ChipErase;

    return (0);
}
//-----------------------------------------------------------------------------
//功能: Nor模块安装
//参数:
//返回: 0 -- 成功; -1 -- 参数失败; -2 -- 内存不足;
//备注:
//-----------------------------------------------------------------------------

s32 ModuleInstall_W25qxx(void)
{
    u32 Len;
    struct FlashChip *Chip;

    if(NULL == CFG_W25QXX_BUS_NAME)
        return (-1);

    Len = strlen(CFG_W25QXX_BUS_NAME) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(*Chip) + Len);
    if(NULL == Chip)
    {
        error_printf("w25qxx","out of memory!\r\n");
        return (-2);
    }
    memset(Chip, 0x0, sizeof(*Chip));
    s_pChip = Chip;
    strcpy(Chip->Name, CFG_W25QXX_BUS_NAME); // 设备别名
    Chip->Type = F_NOR;
    Chip->Descr.Nor.ReservedBlks = CFG_W25QXX_START_BLK; //

    if(Nor_Init(Chip))
    {
        free(Chip);
        return (-2);
    }

    Chip->Buf = (u8*)malloc(Chip->Descr.Nor.BytesPerPage);// NAND底层缓冲
    if(NULL == Chip->Buf)
    {
        error_printf("w25qxx","out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    Device_Create(Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip);// 设备接入"/dev"

    return (0);

}
#endif
//-----------------------------------------------------------------------------
//功能: 驱动测试
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void W25qxx_LocalTest(void)
{
    u32 i, j, k;
    u16 temp;
    u8 WriteBuf[CFG_W25QXX_BYTES_PAGE];
    u8 ReadBuf[CFG_W25QXX_BYTES_PAGE];
    temp = W25QXX_ReadID();
    debug_printf("W25q"," chip id is 0x%4x.\r\n", temp);

    for(i=0; i<W25qxx_description->BytesPerPage; i++)
    {
        WriteBuf[i] = i;
    }
    // 测读写块擦除
    if(false == W25QXX_EraseChip())
    {
        error_printf("W25q"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<W25qxx_description->BlockNum; k++)
        {
            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }


            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                if (false == W25QXX_WritePage(WriteBuf, 
                        (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("W25q"," write block %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }

            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("W25q"," page %d data error.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }
            if (false == W25QXX_EraseBlock(k))
            {
                error_printf("W25q"," erase block %d failed.\r\n", k);
            }

            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }
        }
    }

    // 先写满flash
    for(k=0; k<W25qxx_description->BlockNum; k++)
    {
        for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
            }
        }


        for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
        {
            if (false == W25QXX_WritePage(WriteBuf, 
                    (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("W25q"," write block %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
            }
        }

        for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("W25q"," page %d data error.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
            }
        }
    }

    // 测整片擦除
    if(false == W25QXX_EraseChip())
    {
        error_printf("W25q"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<W25qxx_description->BlockNum; k++)
        {
            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }

        }
    }

    // 测扇区擦除
    for(k=0; k<W25qxx_description->SectorNum; k++)
    {
        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }


        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            if (false == W25QXX_WritePage(WriteBuf, 
                    (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("W25q"," write sector %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }

        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("W25q"," page %d data error.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }
        if (false == W25QXX_EraseSector(k))
        {
            error_printf("W25q"," erase block %d failed.\r\n", k);
        }

        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }
    }

    // 擦除芯片后写满2个块
    if(false == W25QXX_EraseChip())
    {
        error_printf("W25q"," erase chip failed.\r\n");
    }
    else
    {
        for(k=0; k<2; k++)
        {
            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != 0xFF)
                        {
                            error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }


            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                if (false == W25QXX_WritePage(WriteBuf, 
                        (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(WriteBuf)))
                {
                    error_printf("W25q"," write block %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }

            for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
            {
                memset(ReadBuf, 0, sizeof(ReadBuf));
                if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerBlock)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
                {
                    for(j=0; j<W25qxx_description->BytesPerPage; j++)
                    {
                        if(ReadBuf[j] != WriteBuf[j])
                        {
                            error_printf("W25q"," page %d data error.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                            break;
                        }
                    }
                }
                else
                {
                    error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerBlock))));
                }
            }
        }
        // 擦除第一个块
        if (false == W25QXX_EraseBlock(0))
        {
            error_printf("W25q"," erase block %d failed.\r\n", 0);
        }
        // 第一个块是全ff
        for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, i*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("W25q"," page erase %d failed.\r\n", i);
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", i);
            }
        }
        // 第二个块还有数据
        for(i=0; i<(W25qxx_description->PagesPerBlock); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+W25qxx_description->PagesPerBlock)*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("W25q"," page %d data error.\r\n", (i+W25qxx_description->PagesPerBlock));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+W25qxx_description->PagesPerBlock));
            }
        }
    }

    // 测写2个扇区擦一个扇区
    for(k=0; k<2; k++)
    {
        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != 0xFF)
                    {
                        error_printf("W25q"," page erase %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }


        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            if (false == W25QXX_WritePage(WriteBuf, 
                    (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(WriteBuf)))
            {
                error_printf("W25q"," write block %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }

        for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
        {
            memset(ReadBuf, 0, sizeof(ReadBuf));
            if (W25QXX_Read(ReadBuf, (i+(k*(W25qxx_description->PagesPerSector)))*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
            {
                for(j=0; j<W25qxx_description->BytesPerPage; j++)
                {
                    if(ReadBuf[j] != WriteBuf[j])
                    {
                        error_printf("W25q"," page %d data error.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
                        break;
                    }
                }
            }
            else
            {
                error_printf("W25q"," read page %d failed.\r\n", (i+(k*(W25qxx_description->PagesPerSector))));
            }
        }
    }
    // 擦除第一个块
    if (false == W25QXX_EraseSector(0))
    {
        error_printf("W25q"," erase block %d failed.\r\n", 0);
    }
    // 第一个块是全ff
    for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (W25QXX_Read(ReadBuf, i*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<W25qxx_description->BytesPerPage; j++)
            {
                if(ReadBuf[j] != 0xFF)
                {
                    error_printf("W25q"," page erase %d failed.\r\n", i);
                    break;
                }
            }
        }
        else
        {
            error_printf("W25q"," read page %d failed.\r\n", i);
        }
    }
    // 第二个扇区还有数据
    for(i=0; i<(W25qxx_description->PagesPerSector); ++i)
    {
        memset(ReadBuf, 0, sizeof(ReadBuf));
        if (W25QXX_Read(ReadBuf, (i+W25qxx_description->PagesPerSector)*W25qxx_description->BytesPerPage, sizeof(ReadBuf)))
        {
            for(j=0; j<W25qxx_description->BytesPerPage; j++)
            {
                if(ReadBuf[j] != WriteBuf[j])
                {
                    error_printf("W25q"," page %d data error.\r\n", (i+W25qxx_description->PagesPerSector));
                    break;
                }
            }
        }
        else
        {
            error_printf("W25q"," read page %d failed.\r\n", (i+W25qxx_description->PagesPerSector));
        }
    }

}




