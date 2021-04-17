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


//思路:
//   采用一个buf表示页和oob区域,而不是将他们分开表示。原因是读部分数据时,ECC校验
//   却是需要整个页和OOB数据。因此带ECC校验时,每次都需读写的整个页和OOB的数据才行

#include <string.h>
#include <stdlib.h>
#include <djyos.h>
#include <MKxxF12.h>
#include <device/djy_flash.h>
#include <include/device.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <math.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern s32 ModuleInstall_NAND(u32 doformat);
//   ModuleInstall_NAND(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip nand"//CPU的nand驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"device file system","file system"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_NAND == false )
//#warning  " cpu_onchip_nand  组件参数未配置，使用默认配置"
//%$#@target = header   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_NAND    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,true,false,
#define CFG_NFLASH_PART_FORMAT     false      //是否需要擦除该芯片。
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure


//上面的分区起始和分区结束我写的是-1到10000，具体数值可根据具体芯片参数填写
//
// @brief  NFC预定义
//
#define NFCDIV                               0x05// 时钟分频
#define NFCFRAC                              0x00// 时钟分频

#define NANDFLASH_STATUS_PASS                0x00
#define NANDFLASH_STATUS_ERR                 0x01

// ECC STATUS
#define ECC_STATUS_MASK                     ((u8)0x80)
#define ECC_ERR_COUNT                       ((u8)0x3F)

//
// @brief  NFC CMD
//
#define NFC_READ_PAGE_CMD_CODE              0x7EF8
#define NFC_PROGRAM_PAGE_CMD_CODE           0x7FD8
#define NFC_DMA_PROGRAM_PAGE_CMD_CODE       0xFFC0
#define NFC_BLOCK_ERASE_CMD_CODE            0x4ED8
#define NFC_READ_ID_CMD_CODE                0x4804
#define NFC_RESET_CMD_CODE                  0x4040
#define NFC_READ_STATUS_CODE                0x4880

//
// @brief  本地全局变量
//
s32 __nand_read(s64 unit, void *data, struct uopt opt);
s32 __nand_write(s64 unit, void *data, struct uopt opt);
s32 __nand_erase(s64 unit, struct uesz sz);
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...);
static u32 *badstable;
static u32 badslocation = 0;
static bool_t sNandflashInited = false;
extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);
static struct NandDescr *s_ptNandInfo;//
extern struct Object *s_ptDeviceRoot;
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static const char *NandFlashName = "nand";      //该flash在obj在的名字
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10
static u32 *badstable = NULL;
static u32 badslocation = 0;
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
static s32 __nand_init(void);

//
// @brief  本地函数声明
//
static void  __SetRowCol(u32 PageNo, u32 Offset);
static void __Buf2Mem(u8 BufNo, u32 Offset, u8 *Mem, u32 Size);
static void __Mem2Buf(u8 BufNo, u32 Offset, const u8 *Mem, u32 Size);
static s32 __Status(void);
static s32 __Ready(void);
static void  K70_MFC_Config(void);
static s32 K70_GetNandDescr(struct NandDescr *Descr);

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void  K70_MFC_Config(void)
{
    //逻辑:
    //   配置IO管脚
    //   配置NFC寄存器

    // Enable the NFC clock gate in the SIM
    SIM->SCGC3 |= SIM_SCGC3_NFC_MASK;

    // Disable the MPU to allow NFC to access memory
    MPU->CESR &= 0xFFFFFFFE;

    // Set the NFC clock divder according to defines
    SIM->CLKDIV4 |= SIM_CLKDIV4_NFCDIV(NFCDIV) |SIM_CLKDIV4_NFCFRAC(NFCFRAC);

    // NAND管脚配置
    PORTC->PCR[17] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable NFC_CE0 pad */
    PORTC->PCR[16] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_RB pad */
    PORTD->PCR[8]  = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_CLE pad */
    PORTD->PCR[9]  = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_ALE pad */
    PORTD->PCR[10] = PORT_PCR_MUX(6) | PORT_PCR_DSE_MASK;/* Enable the NFC_RE pad */
    PORTC->PCR[11] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_WE pad */

    PORTB->PCR[20] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA15 pad */
    PORTB->PCR[21] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA14 pad */
    PORTB->PCR[22] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA13 pad */
    PORTB->PCR[23] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA12 pad */
    PORTC->PCR[0]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA11 pad */
    PORTC->PCR[1]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA10 pad */
    PORTC->PCR[2]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA9 pad */
    PORTC->PCR[4]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA8 pad */
    PORTC->PCR[5]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA7 pad */
    PORTC->PCR[6]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA6 pad */
    PORTC->PCR[7]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA5 pad */
    PORTC->PCR[8]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA4 pad */
    PORTC->PCR[9]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA3 pad */
    PORTC->PCR[10] = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA2 pad */
    PORTD->PCR[4]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA1 pad */
    PORTD->PCR[5]  = PORT_PCR_MUX(5) | PORT_PCR_DSE_MASK;/* Enable the NFC_DATA0 pad */

    // 设置NFC寄存器
    // 设置CFG寄存器
    NFC->CFG = NFC_CFG_STOPWERR_MASK |// 写错误立即停止
               NFC_CFG_IDCNT(5) |
               NFC_CFG_TIMEOUT(6) |
               NFC_CFG_BITWIDTH_MASK |// 16位宽
               NFC_CFG_PAGECNT(1);
    // 设置RAI寄存器
    NFC->RAI = 0;// 不自动增加地址
    // 设置SWAP寄存器
    NFC->SWAP  = NFC_SWAP_ADDR1(0x7FF) | NFC_SWAP_ADDR2(0x7FF);
    // 设置ISR寄存器
    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);
    NFC->RAR |= (NFC_RAR_CS0_MASK | NFC_RAR_RB0_MASK);// 使能NAND选片
}
//-----------------------------------------------------------------------------
//功能:
//参数: Data -- 数据空间。不能为NULL。
//      Flags --
//
//返回: ">0" -- 正常，写入的字节数;
//      "-1" -- 参数错误;
//      "-2" -- 写失败;
//备注:
//-----------------------------------------------------------------------------
s32  K70_PageProgram(u32 PageNo, const u8 *Data, u32 Flags)
{
    /*逻辑:
        K70下，带ECC写入，ECC的计算值放入spare区域尾部。因此不管是否设置
        SPARE_REQ，带ECC写入都需要写spare。可能会担忧不带SPARE_REQ却需要HW_ECC
        时,会把spare内容覆盖的问题。但实际上，K70的带ECC写入前，必须先擦除，否
        则ECC自动写入值可能不正确。
    */

    u8 *Spare;
    u8 SpareReq = 1;
    u8 BufNo = 2;// 写固定使用buffer 2
    u32 Size = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
    struct FlashChip *Nand;

    if (!Data)
        return (-1);

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    __Ready();

    // 将页的main区域放入buffer
    __Mem2Buf(BufNo, 0, Data, s_ptNandInfo->BytesPerPage);

    if (SPARE_REQ & Flags)
        Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;
    else if (HW_ECC & Flags)
    {
        // 需要ECC校验,但是又不需要写spare时,才需要
        Nand = dListEntry(s_ptNandInfo, struct FlashChip, Descr);
        Spare = Nand->Buf + s_ptNandInfo->BytesPerPage;// spare空间 */
        if (NULL == Spare)
        {
            Lock_MutexPost(NandFlashLock);
            error_printf("nand driver", "chip buffer do not exist!\r\n");
            return (-1);
        }
    }
    else
    {
        SpareReq = 0;
        Size = s_ptNandInfo->BytesPerPage;
    }

    // 将页的spare区域放入buffer
    if (SpareReq)
        __Mem2Buf(BufNo, s_ptNandInfo->BytesPerPage, Spare, s_ptNandInfo->OOB_Size);


    // ecc 功能项
    switch (MASK_ECC & Flags)
    {
        case HW_ECC :
            /* 4 error correct 产生 8bytes ECC */
            NFC->CFG &= (~NFC_CFG_ECCMODE_MASK);
            NFC->CFG |= NFC_CFG_ECCMODE(1);
            break;
        case SW_ECC :
        case NO_ECC :
            NFC->CFG &= (~NFC_CFG_ECCMODE_MASK);
            break;
    }

    __SetRowCol(PageNo, 0);

    NFC->SECSZ = NFC_SECSZ_SIZE(Size + 1);/* 因为是16位 */

    /* Write the NFC_CMD2 register with the command byte and code for an erase */
    NFC->CMD2 = NFC_CMD2_BYTE1(PAGE_PROGRAM_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE) |
                NFC_CMD2_BUFNO(BufNo);

    /* Write the NFC_CMD1 register with the command byte2 and byte3 for an erase */
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_PROGRAM_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);


    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

#if 0
        PrintBuf((u8 *)Data, (2112));/* 测试 */
#endif

    /* Wait for start/busy bit to clear indicating command is done */
    if(__Status())
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    if ((HW_ECC & Flags) && !(SPARE_REQ & Flags))
      // 仅需要ECC写时的返回值。
      // 尽管实际上其写入了spare区域的,这里处理是为了让用户感觉上是让写多少就是多少
        return (s_ptNandInfo->BytesPerPage);
    else
        return (Size);

}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: ">0" -- 正常，读出的字节数。
//      "-1" -- 输入参数错误;
//      "-2" -- 读失败;
//      "-3" -- ECC纠错失败;
//备注: 不管读写是否正确，都将数据回传
//-----------------------------------------------------------------------------
s32  K70_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    /*逻辑:
        K70下，带ECC读，默认获取spare区域。因为ECC值存放在spare区域尾部。
    */
    u32 Size;
    u8 BufNo = 0;// 读固定使用buffer 0
    u32 EccStatusAddr = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;// ECC status存放的位置
    s32 Ret = 0;

    if (!Data)
        return (-1);

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    __Ready();

    // 需要从设备上取出的数据大小
    if ((SPARE_REQ & Flags) || (HW_ECC & Flags))
        Size = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
    else
        Size = s_ptNandInfo->BytesPerPage;

    // ECC 功能项
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
            NFC->CFG &= ~(NFC_CFG_ECCMODE_MASK | NFC_CFG_ECCAD_MASK );
            NFC->CFG |= (NFC_CFG_ECCMODE(1) |// 4 error correct 产生 8bytes ECC
                         NFC_CFG_ECCAD(EccStatusAddr >> 3) |// ECC STATUS地址
                         NFC_CFG_ECCSRAM_MASK);// 使能ECC STATUS
            break;
        case SW_ECC :
        case NO_ECC :
            NFC->CFG &= ~(NFC_CFG_ECCMODE_MASK |
                          NFC_CFG_ECCAD_MASK );
            break;
    }

    __SetRowCol(PageNo, 0);

    NFC->SECSZ =  NFC_SECSZ_SIZE(Size + 1);

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1) |
                 NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE) |
                 NFC_CMD2_BUFNO(BufNo));

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR |= (NFC_ISR_DONECLR_MASK | NFC_ISR_DONEEN_MASK |
                 NFC_ISR_IDLECLR_MASK | NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    if(__Status())
        Ret = -2;// 错误,都失败;

    // ECC校验
    if ( HW_ECC & Flags)
    {
        // buffer 0的空间取出的。小端SOC，其ECC status的地址增4
        u8 EccStatus = *(u8 *)(NFC_BASE + EccStatusAddr + 0x4);

        if (EccStatus & ECC_STATUS_MASK)
        {
            error_printf("nand driver", "Page %d ECC failed\r\n", PageNo);
#if 0
        PrintBuf((u8 *)NFC_BASE, (2300));/* 测试 */
        printk("\r\nECC STATUS: %x\r\n", EccStatus);/* 测试 */
#endif
            Ret = -3;
        }
        else
        {
            u8 EccCnt;
            EccCnt = EccStatus & ECC_ERR_COUNT;
            if (0 != EccCnt)
                debug_printf("nand driver", "Page %d: ECC: %d bit fixed\r\n", PageNo, EccCnt);
        }
    }

    // 从buffer中取出的数据大小
    if ((HW_ECC & Flags) && !(SPARE_REQ & Flags))
        Size = s_ptNandInfo->BytesPerPage;

    __Buf2Mem(BufNo, 0, Data, Size);

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (Ret);// 失败;
    }
    Lock_MutexPost(NandFlashLock);
    return (Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功;"-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32 K70_BlockErase(u32 BlkNo)
{
    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }

    __Ready();

    // Make sure the column address is cleared - not needed for block erase
    NFC->CAR = 0x0;
    NFC->RAR &= (u32)0xFF000000;// Row的地址为清零
    NFC->RAR |= BlkNo << 6; // Row地址中是忽略PAx[5..0]部分的

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// 擦除操作需要关闭ECC

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = NFC_CMD2_BYTE1(BLOCK_ERASE_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_BLOCK_ERASE_CMD_CODE);

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(BLOCK_ERASE_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    while (NFC->CMD2 & NFC_CMD2_BUSY_START_MASK);
    Lock_MutexPost(NandFlashLock);
    return (__Status());
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 好块;
//      "-1" -- 坏块;
//      "-2" -- 内存不足
//      "-3" -- 读失败;
//备注:
//-----------------------------------------------------------------------------
s32 K70_BadChk(u32 BlkNo)
{
    /*逻辑:
       因为器件不同,bad mark的地址可能不一样,所以该函数放在驱动中
    */

    u8 BadMark[2];
    u32 PageNo = (BlkNo * s_ptNandInfo->PagesPerBlk);// (块号)转(页号)
    //u32 BadMarkOffet = (s_ptNandInfo->BytesPerPage >> 1) + 1;// 16位
    u32 BadMarkOffet = ((s_ptNandInfo->BytesPerPage + s_ptNandInfo->BadMarkOffset) >> 1) + 1;
    u32 Size = 2;// 16位下BAD MARK位是2bytes
    u8 BufNo = 0;// 读固定使用BUF 0

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// 关闭ECC

    __SetRowCol(PageNo, BadMarkOffet);

    NFC->SECSZ =  NFC_SECSZ_SIZE(Size + 1);

    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = (NFC_CMD2_BYTE1(PAGE_READ_CMD_BYTE1) |
                 NFC_CMD2_CODE(NFC_READ_PAGE_CMD_CODE) |
                 NFC_CMD2_BUFNO(BufNo));

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_READ_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);

    NFC->ISR  |= (NFC_ISR_DONECLR_MASK |
                  NFC_ISR_DONEEN_MASK  |
                  NFC_ISR_IDLECLR_MASK |
                  NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    // Wait for start/busy bit to clear indicating command is done
    if(__Status())
        return (-3);

    // 判断坏块标志位
    __Buf2Mem(BufNo, 2, BadMark, Size);// 这里偏置量为2与CPU结构有关
    if (0xAAAA == *(u16*)BadMark)
        return (-1);

    return (0);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功; "-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
s32  K70_BadMark(u32 BlkNo)
{
    /*逻辑:
        不管BAD MARK位原来是多少以及是否有ECC校验问题, 只管将该位标志为坏块
    */

    u8 BufNo = 2;// 写固定使用buffer 2
    u32 Size = 2;
    u32 PageNo = (BlkNo * s_ptNandInfo->PagesPerBlk);// (块号)转(页号)
    //u32 BadMarkOffet = (s_ptNandInfo->BytesPerPage >> 1) + 1;// 16位宽
    u32 BadMarkOffet = ((s_ptNandInfo->BytesPerPage + s_ptNandInfo->BadMarkOffset) >> 1) + 1;
    u8 BadMark[2] = {0xAA, 0xAA};

    NFC->CFG &= ~NFC_CFG_ECCMODE_MASK;// 关闭ECC

    NFC->SECSZ = NFC_SECSZ_SIZE(Size + 1);

    __Mem2Buf(BufNo, 2, BadMark, Size);// 这里设置跟CPU结构有关

    __SetRowCol(PageNo, BadMarkOffet);// 这里的BadMarkOffet与CPU结构有关

    // 将标记了坏块的页写回
    // Write the NFC_CMD2 register with the command byte and code for an erase
    NFC->CMD2 = NFC_CMD2_BYTE1(PAGE_PROGRAM_CMD_BYTE1) |
                NFC_CMD2_CODE(NFC_PROGRAM_PAGE_CMD_CODE) |
                NFC_CMD2_BUFNO(BufNo);

    // Write the NFC_CMD1 register with the command byte2 and byte3 for an erase
    NFC->CMD1 = NFC_CMD1_BYTE2(PAGE_PROGRAM_CMD_BYTE2) |
                NFC_CMD1_BYTE3(STATUS_READ_CMD_BYTE);


    NFC->ISR |= (NFC_ISR_DONECLR_MASK |
                 NFC_ISR_DONEEN_MASK |
                 NFC_ISR_IDLECLR_MASK |
                 NFC_ISR_IDLEEN_MASK);

    // Set Start Bit to send command to the NAND flash
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    return (__Status());
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功; "-1" -- 解析失败.
//备注:
//-----------------------------------------------------------------------------
static s32 K70_GetNandDescr(struct NandDescr *Descr)
{
    u32 CfgBack;
    u8 *OnfiBuf;
    u32 OnfiSize = 768;
    u8 BufNo = 0; /* 读固定使用buffer 0 */

    OnfiBuf = malloc (OnfiSize);
    if (NULL == OnfiBuf)
    {
        error_printf("nand driver", "内存不足\r\n");
        return (-1);
    }

    __Ready();

    /* 读ONFI时,强制使用8位宽。原寄存器值先保存 */
    CfgBack = NFC->CFG;

    /* 使用8Bit宽度 */
    NFC->CFG = NFC_CFG_STOPWERR_MASK |
               NFC_CFG_TIMEOUT(6) |
               NFC_CFG_PAGECNT(1);

    /* Write the NFC_CMD2 register with the command byte and code for a
       reset */
    NFC->CMD2 = NFC_CMD2_BYTE1(PARAMETER_PAGE_READ_CMD_BYTE) |
                NFC_CMD2_CODE(0x4860) |
                NFC_CMD2_BUFNO(BufNo);

    /* 重置ISR */
    NFC->ISR |= (NFC_ISR_DONECLR_MASK |
                 NFC_ISR_DONEEN_MASK  |
                 NFC_ISR_IDLECLR_MASK |
                 NFC_ISR_IDLEEN_MASK);

    /* Set Start Bit to send command to the NAND flash */
    NFC->CMD2 |= NFC_CMD2_BUSY_START_MASK;

    /* 等待操作完成，不要读STATUS.未使用DONE_MASK,因为还要取数据 */
    while (!(NFC->ISR & NFC_ISR_IDLE_MASK));

    NFC->CFG = CfgBack;

    __Buf2Mem(BufNo, 0, OnfiBuf, OnfiSize);
#if 0
    PrintBuf(OnfiBuf, OnfiSize);/* 测试 */
#endif
    if(deonfi((const char*)OnfiBuf, Descr, 1))
    {
        free (OnfiBuf);
        return (-1);
    }

    Descr->Type = ONFI;
    free (OnfiBuf);
    return (0);
}
//-----------------------------------------------------------------------------
// 功能：安装nand驱动
// 参数：  doformat -- 是否格式化；
// 返回：成功（0）；失败（-1）；
// 备注：
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(u32 doformat)
{
    if(!s_ptNandInfo)
    {
        if(__nand_init())
        {
            printf("\r\n: erro : device : nand initialization failed(init).");
            return (-1);
        }
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __nand_req(format, 0 , -1, &sz);
    }

    if(!badstable)
    {
        badstable = nandbuildbads(__nand_req);
        if(!badstable)
        {
            printf("\r\n: erro : device : nand initialization failed(bad table).");
            return (-1);
        }
    }

    nand_umedia = malloc(sizeof(struct umedia)+s_ptNandInfo->BytesPerPage+s_ptNandInfo->OOB_Size);
    if(!nand_umedia)
        return (-1);

    nand_umedia->mreq = __nand_req;
    nand_umedia->type = nand;
    nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }
    sNandflashInited = true;
    return (0);// 成功;
}
// =============================================================================
// 功能：判断nandflash是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t Nandflash_is_install(void)
{
    return sNandflashInited;
}
/******************************************************************************
                         PRIVATE FUNCTION(本地私有函数)
******************************************************************************/
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void  __SetRowCol(u32 PageNo, u32 Offset)
{
    NFC->CAR = Offset & 0x7FF;

    NFC->RAR &= 0xFF000000;//ROW的地址为清零
    NFC->RAR |= (PageNo & 0x1FFFF);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void __Buf2Mem(u8 BufNo, u32 Offset, u8 *Mem, u32 Size)
{
    u32 BufAddr;

    BufAddr = BufNo << 12;//buffer的地址
    memcpy(Mem, (u8 *)(NFC_BASE + BufAddr + Offset), Size);
}

//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
static void __Mem2Buf(u8 BufNo, u32 Offset,  const u8 *Mem, u32 Size)
{
    u32 BufAddr;

    BufAddr = BufNo << 12;//buffer的地址
    memcpy((u8 *)(NFC_BASE + BufAddr + Offset), Mem, Size);
}
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回: "0" -- 成功; "-1" -- 失败;
//备注:
//-----------------------------------------------------------------------------
static s32 __Status(void)
{
    u32 ISR;
    ISR = NFC->ISR;// 测试
    while (!(NFC->ISR & NFC_ISR_IDLE_MASK));

    ISR = NFC->ISR;// 测试
    if (NFC->SR2 & NANDFLASH_STATUS_ERR)
    {
        error_printf("nand driver", "NAND操作失败\r\n");
        return (-1);
    }
    else
    {
        /* 清ISR */
        NFC->ISR |= (NFC_ISR_IDLECLR_MASK | NFC_ISR_DONECLR_MASK);
        return (0);
    }

}
//-----------------------------------------------------------------------------
//功能: todo:
//参数:
//返回: "1" -- 准备好; "0" -- 未准备好。
//备注:
//-----------------------------------------------------------------------------
static s32 __Ready(void)
{
    /* 检查R/B_n管脚 */
    //return (StructEntry(s_ptNandInfo, struct NandChip, Lock));/* Chip->Lock */
    return (1);
}


#if 0
//-----------------------------------------------------------------------------
//功能:
//参数:
//返回:
//备注:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init)
{
    volatile u8 *Buf;
    u32 i, Ret;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //安装nand设备
            while(1);
    }

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)0xFF;

    Chip->Ops.WrPage(PageNo, Buf, ECC_Flags | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        //if(((u8)i) != Buf[i])
        if(((u8)0xFF) != Buf[i])
            while(1);
    }

    // ECC一位纠错
    Buf[1] = 0xF0;

    Chip->Ops.WrPage(PageNo, Buf, NO_ECC);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
//      if(((u8)i) != Buf[i])
        if(((u8)0xFF) != Buf[i])
            while(1);
    }

}

void ContinuityTest(struct FlashChip *Chip)
{
     u32 i, j, Len;
    u32 Blks, Pages;
    s32 BadBlks = 0;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* 擦除失败 */
//          TraceDrv(NAND_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
            while(1);
        }
    }

    // 擦除后的校验
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        while(1);// 内存不足
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
            while(1);
        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
                while(1);
        }
    }
}

void ChipRawTest(void)
{
    struct FlashChip *Chip;
    u32 i, Ret;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//安装nand设备
        while(1);

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Ret = Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                    //while(1);
                }
            }

            PageTest(CurPage, 0);
        }
    }


    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }

    ContinuityTest(Chip);
    while(1);
}
#else // 新接口

// ============================================================================
// 功能：nand 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = unit / s_ptNandInfo->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = s_ptNandInfo->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptNandInfo->BytesPerPage;
            break;
        }

        case checkbad:
        {
            if(badslocation==(u32)args)
                res = 1;
            else
                res = K70_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            struct uopt opt = {0};
            opt.main = 1;
            if(0 != __nand_write(badslocation, (void*)args, opt)) // 坏块表放置在第0页
                res = -1;

            break;
        }

        case getbads:
        {
            struct uopt opt = {0};
            u32 **table = (u32**)args;

            opt.main = 1;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = __nand_read(badslocation, (void*)(*table), opt); // 坏块表放置在第0页
            }

            break;
        }

        case markbad:
        {
            res = K70_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;
            struct uesz *sz;
            struct uopt opt = {0};

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            tmp = malloc(s_ptNandInfo->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(0 ==__nand_read(badslocation, (void*)(tmp), opt)) // 读坏块表
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // 存在坏块表，不擦除；

                if(-1==(s32)end)
                    end = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;

                do
                {
                    if((badslocation==(--end)) && escape) // 坏块表在第一页
                        continue;

                    if(__nand_erase((s64)end, *sz))
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

// ============================================================================
// 功能：nand 读；
// 参数：unit -- 读的序号（页）；
//      data -- 读的数据；
//      opt -- 读的方式；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __nand_read(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
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

        res = K70_PageRead((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)s_ptNandInfo->BytesPerPage)
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
//    else
//    {
//        res = K70_PageRead((u32)unit, (u8*)data);
//        if(res != (s32)s_ptNandInfo->OOB_Size)
//        {
//            return (-1);
//        }
//    }

    return (0);
}

// ============================================================================
// 功能：nand 写；
// 参数：unit -- 写的序号（页）；
//      data -- 写的数据；
//      opt -- 写的方式；
// 返回：成功 -- （0）；失败 -- （-1）
// 返回：
// 备注：
// ============================================================================
s32 __nand_write(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
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

        res = K70_PageProgram((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage))
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
//    else
//    {
//        res = K70_PageProgram((u32)unit, (u8*)data);
//        if(res != (s32)s_ptNandInfo->OOB_Size)
//        {
//            return (-1);
//        }
//    }

    return (0);
}

// ============================================================================
// 功能：nand 擦除
// 参数：unit -- 擦除的序号；
//      sz -- 擦除的单位（unit或block）
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __nand_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        nandbadfreeunit(badstable, &unit, __nand_req);
        block = (u32)(unit / s_ptNandInfo->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (K70_BlockErase(block));
}

// ============================================================================
// 功能：
// 参数：
// 返回：
// 备注：
// ============================================================================
static s32 __nand_init(void)
{
    s_ptNandInfo = malloc(sizeof(*s_ptNandInfo));
    if(!s_ptNandInfo)
        return (-1);

    K70_MFC_Config();// 芯片管脚等基本配置
    if(K70_GetNandDescr(s_ptNandInfo))
    {
        free(s_ptNandInfo);
        s_ptNandInfo = NULL;
        return (-1);
    }
    NandFlashLock = Lock_MutexCreate("Nand Flash Lock");        //创建nand的互斥锁
    s_ptNandInfo->ReservedBlks = 0;
    s_ptNandInfo->Controller = HW_ECC_SUPPORTED;
    s_ptNandInfo->BadMarkOffset = s_ptNandInfo->OOB_Size - 4 - 1;
    return (0);
}

// ============================================================================
// 功能：把nand和文件系统关联起来
// 参数：fs -- 需要挂载的文件系统，mediadrv -- 媒体驱动，
//       bstart -- 起始块，bend -- 结束块（不包括该块，只到该块的上一块）
// 返回：0 -- 成功， -1 -- 失败
// 备注：
// ============================================================================
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;
    super->MediaDrv = mediadrv;

    if(-1 == (s32)bend)
    {
        bend = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk * BlockNum;
    super->MediaStart = bstart*s_ptNandInfo->PagesPerBlk; // 起始unit号

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,NandFlashName); //获取该设备的全路径
    File_BeMedia(FullPath,fs); //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}


