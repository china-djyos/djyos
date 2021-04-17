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
#include <cpu_peri.h>
#include <djyos.h>
#include <device.h>
#include <device/djy_flash.h>
#include <spibus.h>
#include <djyfs/filesystems.h>
#include <dbug.h>
#include <device/nand.h>
#include <gd5f1gq5x.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_Gd5f1g(void);
//    ModuleInstall_Gd5f1g();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"gd5f1gq5x"	//SPI和QSPI接口的flash芯片
//parent:"none"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu onchip qspi",//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"        //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NORFLASH_GD5F1G == false )
//#warning  " nandflash_GD5F1G  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NORFLASH_GD5F1G    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@string,1,20,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

								
								

#define ECC_ENABLE_SPARE_WRITE_LEN          64          //默认是使能了芯片的硬件ECC的，后64字节用于硬件ECC了，软件不允许操作

struct MutexLCB *gd5f1g_lock;           //芯片互斥访问保护
struct umedia *nand_umedia = NULL;
struct NandDescr __nandescription; // NAND器件描述

static u32 *bads_table = NULL;
static s32 bads_location = 0;
static bool_t is_bads_table = false;
static bool_t Gd5f1gInited = false;
static const char *NandFlashName = "nand";      //该flash在obj在的名字
static u8 *Spare_BadChk = NULL;
static u8 *Spare_BadMark = NULL;

extern struct Object *s_ptDeviceRoot;
extern s32 deonfi(const struct NandONFI *data, struct NandDescr *onfi, u8 little);

s32 Gd5f1g_Ctrl(enum ucmd cmd, ptu32_t args, ...);
//-----------------------------------------------------------------------------
//功能: 读状态寄存器
//参数: regno：状态寄存器序号
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
u8 Gd5f1g_ReadSR(enum GetFeatures regno, u8 *sta)
{
    u8 command = 0;
    switch(regno)
    {
        case Get_Protection:
            command=Gd5f1g_RdFeaturesReg_Protection;
            break;
        case Get_Feature1:
            command=Gd5f1g_RdFeaturesReg_Feature1;
            break;
        case Get_Status1:
            command=Gd5f1g_RdFeaturesReg_Status1;
            break;
        case Get_Feature2:
            command=Gd5f1g_RdFeaturesReg_Feature2;
            break;
        case Get_Status2:
            command=Gd5f1g_RdFeaturesReg_Status2;
            break;
        default:
            command=Gd5f1g_RdFeaturesReg_Feature1;
            break;
    }
    if(QSPI_Send_CMD(Gd5f1g_ReadFeaturesReg,command,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE) == false)
        return false;

    return QSPI_Receive(sta, 1);
}

//-----------------------------------------------------------------------------
//功能: 写状态寄存器
//参数: regno：状态寄存器序号，data: 待写入的数据
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
u8 Gd5f1g_WriteSR(enum SetFeatures regno, u8 data)
{
    u8 command = 0;
    switch(regno)
    {
        case Set_Protection:
            command=Gd5f1g_WrFeaturesReg_Protection;
            break;
        case Set_Feature1:
            command=Gd5f1g_WrFeaturesReg_Feature1;
            break;
        case Set_Feature2:
            command=Gd5f1g_WrFeaturesReg_Feature2;
            break;
        default:
            command=Gd5f1g_WrFeaturesReg_Feature1;
            break;
    }

    if(QSPI_Send_CMD(Gd5f1g_WriteFeaturesReg,command,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE) == false)
        return false;
    return QSPI_Transmit(&data, 1);
}

//-----------------------------------------------------------------------------
//功能: 等待空闲
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_WaitBusy(u32 timeout)
{
    bool_t ret = true;
    u32 time = 0;
    u8 sta = 0;
    if(Gd5f1g_ReadSR(Get_Status1, &sta) == false)
        return false;
    while((sta & 0x01) == 0x01)   //等待空闲
    {
        DJY_EventDelay(100);
        time ++;
        if(time > timeout)
        {
            ret = false;
            error_printf("W25q","Wait busy timeput.\r\n");
            break;
        }
        if(Gd5f1g_ReadSR(Get_Status1, &sta) == false)
            return false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 写使能等待
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_WriteEnableWait(void)
{
    u8 sta = 0;
    u32 wait = 40;
    if(Gd5f1g_ReadSR(Get_Status1, &sta))
    {
        while((sta & 2) != 2)
        {
            wait --;
            if(wait == 0)
                return false;
            sta = 0;
            Gd5f1g_ReadSR(Get_Status1, &sta);
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
bool_t Gd5f1g_SetWriteEnable(void)
{
    if(QSPI_Send_CMD(Gd5f1g_WriteEnable,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE) == false)
        return false;

    return Gd5f1g_WriteEnableWait();
}


//-----------------------------------------------------------------------------
//功能: 块区擦除
//参数: addr：扇区地址
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 Gd5f1g_EraseBlock(u32 block_no)
{
    u8 sta = 0;

    if(block_no >= (__nandescription.BlksPerLUN * __nandescription.LUNs))
    {
        error_printf("gd5f1g", "erase : block_no = %d. exceed max limit\r\n", block_no);
        return (-1);
    }

    block_no = block_no << 6;
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    if(Gd5f1g_WaitBusy(5000) == false)
    {
        Lock_MutexPost(gd5f1g_lock);
        return -1;
    }

    if(Gd5f1g_SetWriteEnable())
    {
        if(QSPI_Send_CMD(Gd5f1g_BlockErase,block_no,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            if(Gd5f1g_WaitBusy(5000) == false)
            {
                Lock_MutexPost(gd5f1g_lock);
                return -1;
            }
            if(Gd5f1g_ReadSR(Get_Status1, &sta))
            {
                if((sta & 4) == 4)
                {
                    Lock_MutexPost(gd5f1g_lock);
                    return -1;
                }
                else
                {
                    Lock_MutexPost(gd5f1g_lock);
                    return 0;
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return -1;
}

//-----------------------------------------------------------------------------
//功能: 读flash页的备用区
//参数: buf：数据缓冲区，PageNo：读的地址，len：读取的字节数
//返回: 读到的字节数
//备注:
//-----------------------------------------------------------------------------
u32 Gd5f1g_ReadSpare(u32 PageNo, u8* buf)
{
    u8 ecc_1 = 0;
    s32 res = 0;

    s64 t1,t2,t3;

    t1 = DJY_GetSysTime();
    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "read spare param fail. buf = %x, PageNo =%d.\r\n", buf, PageNo);
        return res;
    }
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);
    t2 = DJY_GetSysTime();
    t3 = t2 - t1;

    t1 = DJY_GetSysTime();
    if(Gd5f1g_WaitBusy(5000))
    {
        t2 = DJY_GetSysTime();
        t3 = t2 - t1;

        t1 = DJY_GetSysTime();
        if(QSPI_Send_CMD(Gd5f1g_ReadData,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            t2 = DJY_GetSysTime();
            t3 = t2 - t1;

            t1 = DJY_GetSysTime();
            if(Gd5f1g_WaitBusy(5000))
            {
                t2 = DJY_GetSysTime();
                t3 = t2 - t1;

                t1 = DJY_GetSysTime();
                //__nandescription.BytesPerPage << 8,按理来说是不要左移8位的，但是因为地址长度我也从12位写成了24位，所以我现在得左移8位才正常
                if(QSPI_Send_CMD(Gd5f1g_ReaddaraCachex4, __nandescription.BytesPerPage << 8,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES))
                {
                    t2 = DJY_GetSysTime();
                    t3 = t2 - t1;

                    t1 = DJY_GetSysTime();
                    if(QSPI_Receive(buf, __nandescription.OOB_Size))
                    {
                        t2 = DJY_GetSysTime();
                        t3 = t2 - t1;

                        t1 = DJY_GetSysTime();
                        if(Gd5f1g_ReadSR(Get_Status1, &ecc_1))
                        {
                            t2 = DJY_GetSysTime();
                            t3 = t2 - t1;
                            ecc_1 = ecc_1 >> 4;
                            ecc_1 &= 3;
                            if(ecc_1 != 2)
                                res = __nandescription.OOB_Size;
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//功能: 写flash的页备用区
//参数: buf：数据缓冲区，addr：写的地址，len：写的字节数
//返回: 写进去的字节数
//备注:
//-----------------------------------------------------------------------------
u32 Gd5f1g_WriteSpare(u32 PageNo, u8* buf)
{
    u8 sta = 0;
    s32 res = 0;

    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "write spare param fail. buf = %x, PageNo =%d.\r\n", buf, PageNo);
        return res;
    }
    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    if(Gd5f1g_WaitBusy(5000))
    {
        if(QSPI_Send_CMD(Gd5f1g_PageProgramx4,__nandescription.BytesPerPage, 0, QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_16_BITS,QSPI_DATA_4_LINES))
        {
            if(QSPI_Transmit(buf, __nandescription.OOB_Size))
            {
                if(Gd5f1g_SetWriteEnable())
                {
                    if(QSPI_Send_CMD(Gd5f1g_PageProgramExecute,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                    {
                        if(Gd5f1g_WaitBusy(5000))
                        {
                            if(Gd5f1g_ReadSR(Get_Status1, &sta) == true)
                            {
                                if((sta & 8) != 8)
                                    res = __nandescription.OOB_Size;
                            }
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//功能: 读flash页
//参数: buf：数据缓冲区，PageNo：读的地址，len：读取的字节数
//返回: 读到的字节数;
//备注:
//-----------------------------------------------------------------------------
u32 Gd5f1g_ReadPage(u32 PageNo, u8* buf, u32 Flags)
{
    u8 ecc_1 = 0;
    s32 res = 0;
    u8 ecc = 0;
    u32 len = 0;

    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "read page param fail. buf = %x, PageNo = %d.\r\n", buf, PageNo);
        return res;
    }

    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);

    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            ecc = 1;
            break;
        case SW_ECC :
        case NO_ECC :
            ecc = 0;
            break;
        default :
            ecc = 0;
            break;
    }

    if(Gd5f1g_WaitBusy(5000))
    {
        //QSPI，快速读数据，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，2个空周期，
        if(QSPI_Send_CMD(Gd5f1g_ReadData,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
        {
            if(Gd5f1g_WaitBusy(5000))
            {
                if(QSPI_Send_CMD(Gd5f1g_ReaddaraCachex4,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES))
                {
                    if(SPARE_REQ & Flags)
                        len = __nandescription.BytesPerPage + __nandescription.OOB_Size;
                    else
                        len = __nandescription.BytesPerPage;
                    if(QSPI_Receive(buf, len))
                    {
                        if((ecc) && (Gd5f1g_ReadSR(Get_Status1, &ecc_1)))
                        {
                            ecc_1 = ecc_1 >> 4;
                            ecc_1 &= 3;
                            if(ecc_1 != 2)
                                res = len;
                        }
                        else
                            res = len;
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//功能: 写一页flash
//参数: buf：数据缓冲区，addr：写的地址，len：写的字节数
//返回: 写的字节数;
//备注:
//-----------------------------------------------------------------------------
u32 Gd5f1g_WritePage(u32 PageNo, u8* buf, u32 Flags)
{
    u8 sta = 0;
    s32 res = 0;
    u32 len = 0;
    if((buf == NULL) || (PageNo >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk)))
    {
        error_printf("gd5f1g", "write page param fail. buf = %x, PageNo = %d.\r\n", buf, PageNo);
        return res;
    }

    Lock_MutexPend(gd5f1g_lock,CN_TIMEOUT_FOREVER);
    if(Gd5f1g_WaitBusy(5000))
    {
        if(QSPI_Send_CMD(Gd5f1g_PageProgramx4,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_16_BITS,QSPI_DATA_4_LINES))
        {
            if(SPARE_REQ & Flags)
                len = __nandescription.BytesPerPage + __nandescription.OOB_Size;
            else
                len = __nandescription.BytesPerPage;
            if(QSPI_Transmit(buf, len))
            {
                if(Gd5f1g_SetWriteEnable())
                {
                    //QSPI，页写，地址为addr，4线传输数据，32位地址，4线传输地址，4线传输指令，0个空周期，
                    if(QSPI_Send_CMD(Gd5f1g_PageProgramExecute,PageNo,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                    {
                        if(Gd5f1g_WaitBusy(5000))
                        {
                            if(Gd5f1g_ReadSR(Get_Status1, &sta) == true)
                            {
                                if((sta & 8) != 8)
                                    res = len;
                            }
                        }
                    }
                }
            }
        }
    }
    Lock_MutexPost(gd5f1g_lock);
    return res;
}

//-----------------------------------------------------------------------------
//功能: 带坏块管理的读
// 参数：unit -- 读的序号（页）；
//      data -- 读的数据；
//      opt -- 读的方式；
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 Gd5f1g_ReadPage_BadBlk(s32 page, void *data, struct uopt opt)
{
    u32 flags = 0;
    u32 res;
    s64 offset_page = (s64)page;

    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "read bads_table = NULL\r\n");
        return (-1);
    }

    if(data == NULL)
    {
        error_printf("gd5f1g", "ReadPage_BadBlk param fail data = %x.\r\n",data);
        return (-1);
    }

    nandbadfreeunit(bads_table, &offset_page, Gd5f1g_Ctrl);

    if(offset_page >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk))
    {
        error_printf("gd5f1g", "read :offset_page = %d. exceed max limit\r\n", offset_page);
        return (-1);
    }

    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = Gd5f1g_ReadPage((u32)offset_page, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != __nandescription.BytesPerPage)
            {
                error_printf("gd5f1g", "read fail\r\n");
                return (-1);
            }
        }
        else
        {
            if(res != (__nandescription.BytesPerPage + __nandescription.OOB_Size))
            {
                error_printf("gd5f1g", "read fail and OOB\r\n");
                return (-1);
            }
        }
    }
    else
    {
        res = Gd5f1g_ReadSpare((u32)offset_page, (u8*)data);
        if(res != __nandescription.OOB_Size)
        {
            error_printf("gd5f1g", "read OOB fail.\r\n");
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// 功能：带坏块管理的写；
// 参数：unit -- 写的序号（页）；
//      data -- 写的数据；
//      opt -- 写的方式；
// 返回：成功 -- （0）；失败 -- （-1）
// 返回：
// 备注：
// ============================================================================
s32 Gd5f1g_WritePage_BadBlk(s32 page, void *data, struct uopt opt)
{
    u32 flags = 0;
    u32 res;
    s64 offset_page = (s64)page;

    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "write bads_table = NULL\r\n");
        return (-1);
    }

    if(data == NULL)
    {
        error_printf("gd5f1g", "WritePage_BadBlk param fail data = %x.\r\n",data);
        return (-1);
    }

    nandbadfreeunit(bads_table, &offset_page, Gd5f1g_Ctrl);

    if(offset_page >= (__nandescription.BlksPerLUN * __nandescription.LUNs * __nandescription.PagesPerBlk))
    {
        error_printf("gd5f1g", "write : offset_page = %d. exceed max limit\r\n", offset_page);
        return (-1);
    }

    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = Gd5f1g_WritePage((u32)offset_page, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (__nandescription.BytesPerPage))
            {
                error_printf("gd5f1g", "write fail\r\n");
                return (-1);
            }
        }
        else
        {
            if(res != (__nandescription.BytesPerPage + __nandescription.OOB_Size))
            {
                error_printf("gd5f1g", "write fail and OOB\r\n");
                return (-1);
            }
        }
    }
    else
    {
        warning_printf("gd5f1g", "opt.main = %d, opt.spare =%d.\r\n",opt.main,opt.spare);
    }

    return (0);
}

// ============================================================================
// 功能：带坏块管理的擦除
// 参数：unit -- 擦除的序号；
//      sz -- 擦除的单位（unit或block）
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 Gd5f1g_Erase_BadBlk(s32 block)
{
    u32 offset_block = (u32)block;
    if(bads_table == NULL)
    {
        error_printf("gd5f1g", "erase bads_table = NULL\r\n");
        return (-1);
    }

    nandbadfreeblock(bads_table, &offset_block, Gd5f1g_Ctrl);

    if(offset_block >= (__nandescription.BlksPerLUN * __nandescription.LUNs))
    {
        error_printf("gd5f1g", "erase : offset_block = %d. exceed max limit\r\n", offset_block);
        return (-1);
    }

    return (Gd5f1g_EraseBlock(offset_block));
}

//-----------------------------------------------------------------------------
//功能: Gd5f1g_Qspi使能
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_QspiEnable(void)
{
    u8 sta2;
    bool_t ret = true;

    if(Gd5f1g_ReadSR(Get_Feature1, &sta2) == false)       //读状态寄存器2的值
        return false;
    if((sta2 & 0X01)==0)           //QE位未使能
    {
        sta2 |= 1<<0;              //使能QE位
        if(Gd5f1g_WriteSR(Set_Feature1, sta2) == false) //写状态寄存器2
            ret = false;

        sta2 = 0;
        if(Gd5f1g_ReadSR(Get_Feature1, &sta2))       //读状态寄存器2的值
        {
            if((sta2 & 0X01)==0)           //QE位未使能
                ret = false;
        }
        else
            ret = false;
    }
    return ret;
}

//-----------------------------------------------------------------------------
//功能: 读ID
//参数: 无
//返回: 芯片ID
//备注:
//-----------------------------------------------------------------------------
u16 Gd5f1g_ReadID(void)
{
    u8 temp[2];
    u16 deviceid;
    bool_t ret = false;

    if(Gd5f1g_WaitBusy(5000) == false)
    {
        return false;
    }

//    if(W25QXX_QPI_MODE)
//    {
//        //QSPI，地址位0，4线传输数据，24位地址，4线传输地址，4线传输指令，无空周期，2字节数据
//        ret = QSPI_Send_CMD(W25X_ManufactDeviceID,0,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
//    }
//    else
//    {
        //SPI，地址位0，单线传输数据，24位地址，单线传输地址，单线传输指令，无空周期，2字节数据
        ret = QSPI_Send_CMD(Gd5f1g_DeviceID,0,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_1_LINE);
//    }
    if(ret)
    {
        QSPI_Receive(temp,2);
        deviceid=(temp[0] << 8) | temp[1];
    }
    else
        deviceid = 0;
    return deviceid;
}

static bool_t Gd5f1g_ReadParamPage(struct NandONFI *onfi)
{
    u8 sta;
    bool_t res = false;
    if(Gd5f1g_WaitBusy(5000))
    {
        if(Gd5f1g_ReadSR(Get_Feature1, &sta))
        {
            if(!(sta & (1 << 6)))
            {
                sta |= 1 << 6;
                if(Gd5f1g_WriteSR(Get_Feature1, sta))
                {   sta = 0;
                    Gd5f1g_ReadSR(Get_Feature1, &sta);
                }
            }

            if(sta & (1 << 6))
            {
                if(QSPI_Send_CMD(Gd5f1g_ReadData,ParameterPage_Addr,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_NONE))
                {
                    if(Gd5f1g_WaitBusy(5000))
                    {
                        if(QSPI_Send_CMD(Gd5f1g_ReaddaraCache,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE))
                        {
                            if(QSPI_Receive((u8 *)onfi, sizeof(struct NandONFI)))
                            {
                                res = true;
                            }
                        }
                    }
                }
                sta &= ~(1 << 6);
                if(Gd5f1g_WriteSR(Get_Feature1, sta))
                {
                    if(Gd5f1g_ReadSR(Get_Feature1, &sta))
                    {
                        if(sta & (1 << 6))
                            res = false;
                    }
                }
            }
        }
    }

    return res;
}

//-----------------------------------------------------------------------------
//功能: 标记坏块
//参数:
//返回: "0" -- 成功;
//      "-1" -- 失败;
//      "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_BadMark(u32 BlkNo)
{
    //逻辑:
    //    不管BAD MARK位原来是多少以及是否有ECC校验问题, 只管将该位标志为坏块

    u8 i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * __nandescription.PagesPerBlk;

    memset(Spare_BadMark, 0xFF, __nandescription.OOB_Size);
    Spare_BadMark[__nandescription.BadMarkOffset] = 0xAA;
    Spare_BadMark[__nandescription.BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(0 == Gd5f1g_WriteSpare(PageNo + i, Spare_BadMark))
            Ret = -1;
    }
    return (Ret);
}

//-----------------------------------------------------------------------------
//功能:检查块的好坏
//参数:块好
//返回: "0" -- 好块;
//      "-1" -- 坏块;
//      "-2" -- 内存不足
//      "-3" -- 读失败;
//备注:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_BadChk(u32 BlkNo)
{
    u8 i;
    s32 Ret = 0,ReadState = 0;
    u32 PageNo = BlkNo * __nandescription.PagesPerBlk;

    if(is_bads_table)
    {
        for (i = 0; i < 2; i++)// 只判断每块的首两页
        {
            ReadState = Gd5f1g_ReadSpare(PageNo + i, Spare_BadChk);
            if(ReadState != __nandescription.OOB_Size)
            {
                Ret = -3;
                break;
            }
            if ((0xAA == Spare_BadChk[__nandescription.BadMarkOffset]) &&
                (0xAA == Spare_BadChk[__nandescription.BadMarkOffset-1]))
            {
                Ret = -1;
                break;
            }
        }
    }
    else
    {
        if(Gd5f1g_ReadSpare(PageNo, Spare_BadChk) != 0)
        {
            for(i = 0; i < __nandescription.OOB_Size; i++)
            {
                if(Spare_BadChk[i] != 0xff)
                    break;
            }
            if(i != __nandescription.OOB_Size)
            {
                Gd5f1g_BadMark(BlkNo);
                Ret = -1;
            }
        }
    }
    return (Ret);
}

// ============================================================================
// 功能：nand 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：根据不同的命令，有不同的返回值
// 备注：
// ============================================================================
s32 Gd5f1g_Ctrl(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);
            va_end(list);
            *block = unit / __nandescription.PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  __nandescription.BlksPerLUN * __nandescription.LUNs;
            break;
        }

        case blockunits:
        {
            *((u32*)args) = __nandescription.PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            *((u32*)args) = __nandescription.BytesPerPage;
            break;
        }
        case sparebytes:
        {
            *((u32*)args) = __nandescription.OOB_Size;
            break;
        }

        case checkbad:
        {
            if(bads_location == (s32)args)
                res = 1;
            else
                res = Gd5f1g_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            if(0 == Gd5f1g_WritePage(bads_location, (void*)args, 0)) // 坏块表放置在第0页
                res = -1;

            break;
        }

        case getbads:
        {
            u32 **table = (u32**)args;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = Gd5f1g_ReadPage(bads_location, (void*)(*table), HW_ECC);
            }

            break;
        }

        case markbad:
        {
            res = Gd5f1g_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            va_end(list);

            tmp = malloc(__nandescription.BytesPerPage);
            if(!tmp)
                return (-1);

            if(Gd5f1g_ReadPage(bads_location, (void*)(tmp), HW_ECC)) // 读坏块表
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // 存在坏块表，不擦除；

                if(-1 == end)
                    end = __nandescription.BlksPerLUN * __nandescription.LUNs;

                do
                {
                    if((bads_location == (--end)) && escape) //坏块表在第一页
                        continue;

                    if(Gd5f1g_EraseBlock((u32)end))
                    {
                        res = -1;
                        break;
                    }
                }
                while(end!=start);
            }
            else
            {
                res = -1;
            }

            free(tmp);
            break;
        }

        default: res = -1; break;
    }

    return (res);
}


//-----------------------------------------------------------------------------
//功能:获取nand flash 的内部信息
//参数:
//返回: "0" -- 成功; "-1" -- 解析失败; "-2" -- 内存不足;
//备注:
//-----------------------------------------------------------------------------
static s32 Gd5f1g_GetNandDescr(struct NandDescr *Descr)
{
    s32 Ret = -1;
    struct NandONFI *onfi_buf;

    onfi_buf = (struct NandONFI *)malloc(sizeof(struct NandONFI));
    if (NULL == onfi_buf)
        return -2;

    memset(onfi_buf, 0, sizeof(struct NandONFI));
    if(Gd5f1g_ReadParamPage(onfi_buf))
    {
        if(deonfi((const struct NandONFI *)onfi_buf, Descr, 0) == 0)
            Ret = 0;
    }
    free (onfi_buf);
    return (Ret);
}

//-----------------------------------------------------------------------------
//功能: Gd5f1g初始化
//参数: 无
//返回: true -- 成功; false -- 失败;
//备注:
//-----------------------------------------------------------------------------
bool_t Gd5f1g_Init(void)
{
    u16 chip_id;
    u8 sta;
    bool_t ret = false;

    if(Gd5f1g_GetNandDescr(&__nandescription))
    {
        return false;
    }
    __nandescription.ReservedBlks = 0;
    __nandescription.Controller = HW_ECC_SUPPORTED;
    __nandescription.OOB_Size = ECC_ENABLE_SPARE_WRITE_LEN;
    __nandescription.BadMarkOffset = 0x30;          //备份区中，0x30~0x33，是不受ECC保护的。写在这里面，不会影响读其他数据的操作
    gd5f1g_lock = Lock_MutexCreate("gd25q16c Lock");
    if(!gd5f1g_lock)
    {
        error_printf("gd5f1g"," create gd5f1g flash lock fali \r\n.");
        return false;
    }
    if(QSPI_Send_CMD(Gd5f1g_SoftReset,0,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_ADDRESS_8_BITS,QSPI_DATA_NONE))
    {
        DJY_EventDelay(1000);

        chip_id = Gd5f1g_ReadID();
        if(chip_id != 0Xc851)
            error_printf("gd5f1g","chip ID error.\r\n");
        else
        {
            if(Gd5f1g_QspiEnable())
            {
                if(Gd5f1g_ReadSR(Get_Protection,&sta))
                {
                    if(sta & (7 << 3))
                    {
                        sta &= ~(7 << 3);
                        if(Gd5f1g_WriteSR(Set_Protection, sta))
                        {
                            if(Gd5f1g_ReadSR(Get_Protection,&sta))
                            {
                                if(!(sta & (7 << 3)))
                                    ret = true;
                            }
                        }
                    }
                    else
                        ret = true;
                }
            }
        }
    }
    return ret;
}
//-----------------------------------------------------------------------------
//功能: nand_flash模块安装
//参数:
//返回: 0 -- 成功; -1 -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 ModuleInstall_Gd5f1g(void)
{
    u8 *buf;
    if(Gd5f1g_Init() == false)
    {
        error_printf("gd5f1g", "device : initialization failed(init).\r\n");
        return -1;
    }

    buf = malloc(__nandescription.BytesPerPage);
    if (NULL == buf)
        return -1;
    if(Gd5f1g_ReadPage(0, buf, HW_ECC))
    {
        if(nandvalidbads((u32 *)buf))
            is_bads_table = true;
        else
            is_bads_table = false;

        free(buf);

        Spare_BadChk = malloc(__nandescription.OOB_Size);
        if (NULL == Spare_BadChk)
        {
            return (-1);
        }

        Spare_BadMark = malloc(__nandescription.OOB_Size);
        if (NULL == Spare_BadChk)
        {
            free(Spare_BadChk);
            return (-1);
        }

        if(!bads_table)
        {
            bads_table = nandbuildbads(Gd5f1g_Ctrl);
            if(!bads_table)
            {
                error_printf("gd5f1g", "device : initialization failed(bad table).\r\n");
                free(Spare_BadMark);
                free(Spare_BadChk);
                return (-1);
            }
        }

        nand_umedia = malloc(sizeof(struct umedia)+__nandescription.BytesPerPage+__nandescription.OOB_Size);
        if(!nand_umedia)
            return (-1);
        memset(nand_umedia,0,sizeof(struct umedia));

        nand_umedia->mreq = Gd5f1g_Ctrl;
        nand_umedia->type = nand;
        nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

        if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
        {
            error_printf("gd5f1g", " device : %s addition failed.\r\n", NandFlashName);
            free(nand_umedia);
            free(Spare_BadMark);
            free(Spare_BadChk);
            return (-1);
        }

        Gd5f1gInited = true;
    }
    else
        free(buf);
    return 0;

}

// =============================================================================
// 功能：判断nandflash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t Gd5f1g_IsInstall(void)
{
    return Gd5f1gInited;
}


// ============================================================================
// 功能：把nand和文件系统关联起来
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __Gd5f1g_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return -1;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("gd5f1g"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;             //把nand的信息放到文件系统的核心数据结构当中
    super->MediaDrv = mediadrv;

    if(-1 == (s32)bend)
    {
        bend = __nandescription.BlksPerLUN * __nandescription.LUNs;  //计算文件系统所有区域的结束块
        BlockNum = bend - bstart;       //计算一共有多少块
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = __nandescription.BytesPerPage * __nandescription.PagesPerBlk * BlockNum;        //计算文件系统所用区域一共有多少字节
    super->MediaStart = bstart * __nandescription.PagesPerBlk; // 起始unit号

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name, NandFlashName); //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}
