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

#ifndef __NAND_H__
#define __NAND_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//
//JEDEC标准命令
//
#define PAGE_READ_CMD_BYTE1                 (0x00)
#define PAGE_READ_CMD_BYTE2                 (0x30)
#define PAGE_PROGRAM_CMD_BYTE1              (0x80)
#define PAGE_PROGRAM_CMD_BYTE2              (0x10)
#define BLOCK_ERASE_CMD_BYTE1               (0x60)
#define BLOCK_ERASE_CMD_BYTE2               (0xD0)
#define ID_READ_CMD_BYTE                    (0x90)
#define STATUS_READ_CMD_BYTE                (0x70)
#define PARAMETER_PAGE_READ_CMD_BYTE        (0xEC)
#define RESET_CMD_BYTE                      (0xFF)

//
// @brief  NAND设备信息
//
struct NandDescr{
    u16      Type;                      // 是否支持ONFI
#define ONFI                            ((u16)0x0001)
#define RAW                             ((u16)0x0000)
    u16      Revision;                  // ONFI版本号
    char     Manufacturer[13];          // 芯片制造商
    char     DeviceModel[21];           // 芯片型号
    u16      OOB_Size;                  // Out Of Band空间尺寸,字节为单位
    u32      BytesPerPage;              // 页中包含的字节数
    u32      PagesPerBlk;               // 块中包含的页数
    u32      BlksPerLUN;                // LUN中包含的块数
    u8       LUNs;                      // LUN数量
#if 0
    u8       SafeBlkNo;                 // 厂商确保有用的块号
#endif
    u8       BadMarkOffset;             // 坏块标记在spare中的偏置,数组方式计位
    u16      ReservedBlks;              // 保留的块数，从第0块开始计算
    u8       Controller;                // NAND控制器属性见下面
#define NO_ECC_SUPPORTED                ((u8)0x0)
#define HW_ECC_SUPPORTED                ((u8)0x1)
#define SW_ECC_SUPPORTED                ((u8)0x2)
};


//
// @brief  ONFI
//
struct NandONFI
{
    //修订信息和特征块
    s8 signature[4];             //ONFI签名
    s16 revision_num;            //支持的ONFI版本号
    s16 features_supp;           //功能支持
    s16 opt_cmd_supp;            //支持可选的命令
    s8 adv_cmd_supp;             //ONFI-JEDEC JTG主要高级命令支持
    s8 tar_cmd_supp;             //训练命令支持
    s16 ext_param_page_len;      //扩展参数页长
    s8 param_page_num;           //参数页数
    s8 feat_reserved[17];        //修订信息和特征块中保留的位
    //制造商信息块
    s8 dev_manufacturer[12];         //设备制造商
    s8 dev_model[20];            //设备型号
    s8 manufacturer_id;          //JEDEC 制造商ID
    s16 data_code;               //数据代码
    s8 manuf_reserved[13];       //制造商信息块中保留的位
    //内存结构块
    s32 bytes_per_page;          //每页的字节数
    s16 spare_bytes_per_page;    //每页的备用字节数
    s32 obsolete_bytes_per_partial_page;            //每部分页的字节数(已废弃)
    s16 obsolete_spare_bytes_per_partial_page;      //每部分页的备用字节数(已废弃)
    s32 pages_per_block;          //每块的页数
    s32 blocks_per_LUN;           //每个LUN的块数
    s8 LUNs;                      //LUN数
    s8 address_cycles;            //地址周期数，0-3：行地址周期数，4-7：列地址周期数
    s8 bits_per_cell;             //每个单元的比特数
    s16 bad_blk_max_per_LUN;      //每个LUN的最大坏块数
    s16 block_endurance;          //块的耐久性
    s8 blocks_begin_valid;        //在目标开始时保证有效块
    s16 valid_block_endurance;    //保证有效块的块持久性
    s8 programs_per_page;         //每页的程序数
    s8 obsolete_Part_programs_attri;       //部分编程属性(已废弃)
    s8 ECC_corr_bits;             //ECC可纠正位数
    s8 plane_address_bits;        //面地址位数
    s8 multi_plane_oper_attri;    //多面操作属性
    s8 EZ_nand_support;           //EZ nand支持
    s8 memory_reserved[12];       //内存结构块的保留位
    //电气参数块
    s8 IO_capacitance_max;        //I/O引脚电容量，最大值
    s16 SDR_timing_support;       //SDR计时模式支持
    s16 obsolete_SDR_program_cache;     //支持SDR程序缓存定时模式(已废弃)
    s16 max_page_program_time;    //tPROG最大页面程序时间
    s16 max_block_erase_time;     //tBERS最大块擦除时间
    s16 max_page_read_time;       //tR最大页读取时间
    s16 min_change_colu_set_time; //最小更改列设置时间
    s8 NV_DDR_timing_mode_supp;   //NV-DDR定时模式支持
    s8 NV_DDR2_timing_mode_supp;   //NV-DDR2定时模式支持
    s8 DDR_DDR2_features;          //NV-DDR - NV-DDR2功能
    s16 CLK_input_capacitance;     //时钟输入引脚电容量，典型值
    s16 IO_capacitance_typical;    //I/O引脚电容量，典型值
    s16 input_capacitance_typical; //输入引脚电容量，典型值
    s8 input_capacitance_max;      //输入引脚电容量，最大值
    s8 driver_strength_supp;       //驱动力量支持
    s16 multi_plane_page_read_time_max;     //最大多面也读时间
    s16 program_page_reg_clear;    //程序页寄存器清除增强tADL值
    s16 typical_page_read_time;    //典型的EZ NAND的页面读取时间
    s8 NV_DDR2_3_features;         //NV-DDR2/3特性
    s8 NV_DDR2_3_warmup_cycles;    //NV-DDR2/3预热周期
    s16 NV_DDR3_timing_supp;       //NV-DDR3定时模式支持
    s8 NV_DDR2_timing_supp;        //NV-DDR2定时模式支持
    s8 electrical_reserved;        //电气参数块，保留位
    //供应商的块
    s16 vendor_specific_revision;  //供应商特定版本号
    s8 vendor_specific[88];        //特定于供应商的
    s16 integrity_CRC;             //完整CRC
    //剩余的参数页
    s8 redundant1[256];
    s8 redundant2[256];

}__attribute__((packed));

//
//
//
struct NandSpecialOps{
    s32 (*ChkBlk)(u32 BlkNo);           // 检查块是否已坏
    s32 (*MrkBad)(u32 BlkNo);           // 标记坏块
};
#ifdef __cplusplus
}
#endif

#endif //ifndef __NAND_H__
