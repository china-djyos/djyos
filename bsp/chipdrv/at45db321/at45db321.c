//----------------------------------------------------
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
//所属模块：SPI FLASH模块
//作者：liq
//版本：V0.1.0
//文件描述：DJYOS的SPI FLASH AT45DB321D底层驱动
//其他说明：
//修订历史：
//1. 日期: 2014-4-15
//   作者：
//   新版本号：V0.1.0
//   修改说明：原始版本
//------------------------------------------------------

#include "stdint.h"
#include "string.h"
#include "cpu_peri.h"
#include "at45db321.h"
#include "spibus.h"
#include "board-config.h"
#include <device/djy_flash.h>
#include "os.h"
#include "systime.h"
#include <math.h>
#include <stdlib.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <device.h>
#include <device/unit_media.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_at45db321(void);
//    ModuleInstall_at45db321();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nor flash at45db321"//spi接口的norflash
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","spi bus","heap","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"yaf2 file system","easy file system"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NOR_FLASH_AT45DB321 == false )
//#warning  " nor_flash_at45db321  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NOR_FLASH_AT45DB321    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@enum,512,528,
#define CFG_AT45_PAGE_SIZE                  512       //配置AT45的页大小，默认为512
//%$#@string,1,10,
#define CFG_AT45_BUSNAME                   "SPI4"     //"SPI总线名称",AT45使用的总线名称
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_AT45_PART_FORMAT               false      //分区选项,是否需要擦除该芯片。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

/*------------------------Time Symbol--------------------------------
Symbol  Parameter                               Typ     Max     Units
fSCK    SCK Frequency                                   66      MHz
fCAR1   SCK Frequency for Continuous Array Read         66      MHz
fCAR2   SCK Frequency for Continuous Array Read
        (Low Frequency)                                 33      MHz
tXFR    Page to Buffer Transfer Time                    200     μs
tcomp   Page to Buffer Compare Time                     200     μs
tEP     Page Erase and Programming Time         17      40      ms
tP      Page Programming Time                   3       6       ms
tPE     Page Erase Time                         15      35      ms
tBE     Block Erase Time                        45      100     ms
-------------------------------------------------------------------*/

//Read Commands
#define AT45_MainMem_Page_Read          0xD2
#define AT45_Continous_Array_Read_LC    0xE8        //(Legacy Command) up to 66M
#define AT45_Continous_Array_Read_LF    0x03        //(Low Frequency) up to 33M
#define AT45_Continous_Array_Read_HF    0x0B        //(High Frequency) up to 66M
#define AT45_Buff1_Read_LF              0xD1        //(Low Frequency)
#define AT45_Buff2_Read_LF              0xD3        //(Low Frequency)
#define AT45_Buff1_Read                 0xD4
#define AT45_Buff2_Read                 0xD6
//Program and Erase Commands
#define AT45_Buff1_Write                0x84
#define AT45_Buff2_Write                0x87
#define AT45_Buff1_to_MainMem_Page_Prog_vsErase     0x83    //Buffer 1 to Main Memory Page Program with Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_vsErase     0x86    //Buffer 2 to Main Memory Page Program with Built-in Erase
#define AT45_Buff1_to_MainMem_Page_Prog_noErase     0x88    //Buffer 1 to Main Memory Page Program without Built-in Erase
#define AT45_Buff2_to_MainMem_Page_Prog_noErase     0x89    //Buffer 2 to Main Memory Page Program without Built-in Erase
#define AT45_Page_Erase_Cmd             0x81
#define AT45_Block_Erase_Cmd            0x50
#define AT45_Sector_Erase               0x7C
#define AT45_Chip_Erase1                0xC7
#define AT45_Chip_Erase2                0x94
#define AT45_Chip_Erase3                0x80
#define AT45_Chip_Erase4                0x9A
#define AT45_MainMem_Page_Prog_Through_Buff1        0x82
#define AT45_MainMem_Page_Prog_Through_Buff2        0x85
//Additional Commands
#define AT45_MainMem_Page_to_Buff1_Transfer         0x53
#define AT45_MainMem_Page_to_Buff2_Transfer         0x55
#define AT45_MainMem_Page_to_Buff1_Compare          0x60
#define AT45_MainMem_Page_to_Buff2_Compare          0x61
#define AT45_Auto_Page_Rewrite_through_Buff1        0x58
#define AT45_Auto_Page_Rewrite_through_Buff2        0x59
#define AT45_Deep_Power_down                        0xB9
#define AT45_Resume_Power_down                      0xAB
#define AT45_Status_Register_Read                   0xD7
#define AT45_Command_ID                             0x9F    //Manufacturer and Device ID Read
//AT45_Status_Register bit define
#define AT45_Status_Reg_Bit_BUSY        0x80    //1=Ready   0=Busy
#define AT45_Status_Reg_Bit_COMP        0x40    //1=not match   0=match
#define AT45_Status_Reg_Bit_PRTC        0x02    //1=En protect  0=Dis protect
#define AT45_Status_Reg_Bit_PGSZ        0x01    //1=page size 512   0=page size 528

//#ifndef CFG_AT45_PAGE_SIZE
////CFG_AT45_PAGE_SIZE默认值为528，如不使用默认值，请在config-prj.h中定义
//#define CFG_AT45_PAGE_SIZE      528
//#endif

#define AT45_Block_Size     CFG_AT45_PAGE_SIZE*8

//buff num define
#define AT45_Buff1  1
#define AT45_Buff2  2
//page erase define
#define vs_Erase    1
#define no_Erase    2

struct SPI_Device *s_ptAT45_Dev;
u32 AT45_OP_TIMEOUT = 1800000; // 30分钟
#define AT45_SPI_SPEED      (10*1000*1000)
static bool_t sAT45Inited = false;

//Command指令缓存区
u8 _at45db321_Command[10]={0};
//SPI FLASH数据缓存区
u8 _at45db321_buff[CFG_AT45_PAGE_SIZE] = {0};
//当前可用于写数据的Buff,代表另一Buff有可能正处于向FLASH写数据的阶段
u8 _at45db321_Ready_Buff = AT45_Buff1;

struct MutexLCB *pAT45_Lock;   //芯片互斥访问保护
struct NorDescr *nordescription;
char *At45Name = "AT45DB321E";
struct umedia *at45_umedia;
extern struct Object *s_ptDeviceRoot;

s32 __at45_req(enum ucmd cmd, ptu32_t args, ...);
s32 __at45_erase(u32 unit, struct uesz sz);
bool_t at45db321_Wait_Ready(u32 Time_Out);
/*---------------------test use only----------------------
#define test_buff_size  10240
section("seg_int_data") u32 Data_write[test_buff_size]={0};
section("seg_int_data") u32 Data_read[test_buff_size]={0};
u32 buff1_cnt=0,buff2_cnt=0;
---------------------test use only----------------------*/
bool_t _at45db321_cs_active(void)
{
    return SPI_CsActive(s_ptAT45_Dev,AT45_OP_TIMEOUT);
}
void _at45db321_cs_inactive(void)
{
    SPI_CsInactive(s_ptAT45_Dev);
    DJY_DelayUs(20);
}
bool_t _at45db321_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen)
{
    struct SPI_DataFrame data;

    data.RecvBuf = rdata;
    data.RecvLen = rlen;
    data.RecvOff = slen;
    data.SendBuf = sdata;
    data.SendLen = slen;

    return SPI_Transfer(s_ptAT45_Dev,&data,true,AT45_OP_TIMEOUT);
}

//note: 应用程序中的Address实际为虚拟地址，需要经过转换才能成为AT45中的实际地址
//      AT45用地址线A0-A9代表Page内的偏移地址，A10-A23代表Page地址，而因为AT45的Page大小是528bytes
//      所以用此表示的实际物理地址是不连续的，所以应用程序中使用的连续的虚拟地址需要经过变换方可成为实际物理地址

//----通过虚拟地址计算Page内偏移地址-------------------------------------------
//功能：通过虚拟地址计算Page内偏移地址
//      Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//      1 page = 528 bytes
//参数：Address 虚拟地址
//返回：Page内偏移地址地址byte_offset_addr
//-----------------------------------------------------------------------------
u32 _at45db321_Offset_Caculate(u32 Address)
{
#if CFG_AT45_PAGE_SIZE==512
    return ( (Address%CFG_AT45_PAGE_SIZE)&0x1FF );
#elif CFG_AT45_PAGE_SIZE==528
    return ( (Address%CFG_AT45_PAGE_SIZE)&0x3FF );
#endif
}

//----通过虚拟地址计算Page地址------------------------------------------------
//功能：通过虚拟地址计算Page地址
//      Address(23bit) = page_addr(13bit) + byte_offset_addr(10bit for 528)
//      1 page = 528 bytes
//参数：Address 虚拟地址
//返回：Page地址
//-----------------------------------------------------------------------------
u32 _at45db321_Page_Caculate(u32 Address)
{
    return (u32)( Address/CFG_AT45_PAGE_SIZE );
}

//----通过Page地址计算Block地址------------------------------------------------
//功能：通过Page地址计算Block地址
//      page_addr = block_addr(10bit) + page_offset_addr(3bit)
//      1 block = 8 pages
//参数：page_addr   Page地址
//返回：Block地址
//-----------------------------------------------------------------------------
u32 _at45db321_Block_Caculate(u32 page_addr)
{
    return ( page_addr>>3 );
}

//----写入数据长度计算--------------------------------------------------------
//功能：通过比较Page的大小确定本次可以写入的数据长度
//参数：byte_offset_addr    Page内的偏移地址
//      data_len            期望写入的数据长度
//返回：本次实际能够写入的数据长度
//-----------------------------------------------------------------------------
u32 _at45db321_Written_Caculate(u32 byte_offset_addr,u32 data_len)
{
    u32 written_data_len;
    written_data_len = CFG_AT45_PAGE_SIZE-byte_offset_addr;
    if( written_data_len>data_len )
        written_data_len = data_len;
    return written_data_len;
}

//----SPI FLASH连续数据读取----------------------------------------------------
//功能：从AT45的地址Address处开始，连续读取data_len长度的数据，存入data
//      若地址累计到FLASH末尾则返回开头处继续读取
//参数：page_addr   Page地址
//      byte_offset_addr    Page内的偏移地址
//      *data       数据存储首地址
//      data_len    数据长度
//返回：实际读取的数据长度
//-----------------------------------------------------------------------------
u32 _at45db321_Continuous_Array_Read(u32 page_addr,u32 byte_offset_addr,u8 *data,u32 data_len)
{
    //array_addr
    _at45db321_Command[0] = AT45_Continous_Array_Read_HF;
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = ((page_addr<<1)|(byte_offset_addr>>8))&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = ((page_addr<<2)|(byte_offset_addr>>8))&0xFF;
#endif
    _at45db321_Command[3] = byte_offset_addr & 0xFF;
    _at45db321_Command[4] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,5,data,data_len))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    _at45db321_cs_inactive();
    return data_len;
}

//----SPI FLASH写数据到缓存区--------------------------------------------------
//功能：SPI FLASH写数据到缓存区
//参数：buff_num    目标缓存区选择
//      buff_addr   缓存区内偏移地址
//      *data       数据获取首地址
//      data_len    数据长度
//返回：实际写入的数据长度
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Write(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_Buff1_Write;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_Buff2_Write;
    else
    {
        return 0;
    }
    //buff_addr
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = (buff_addr>>8)&0xFF;
    _at45db321_Command[3] = (buff_addr)&0xFF;

    if(FALSE ==_at45db321_cs_active())
    {
        return 0;
    }

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    if(_at45db321_TxRx(data,data_len,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }
    _at45db321_cs_inactive();

    return data_len;
}

//----SPI FLASH读缓存区数据----------------------------------------------------
//功能：SPI FLASH读缓存区数据
//参数：buff_num    目标缓存区选择
//      buff_addr   缓存区内偏移地址
//      *data       数据接收首地址
//      data_len    数据长度
//返回：实际读取的数据长度
//-----------------------------------------------------------------------------
u32 _at45db321_Buff_Read(u32 buff_num,u32 buff_addr,u8 *data,u32 data_len)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_Buff1_Read;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_Buff2_Read;
    else
    {
        return 0;
    }
    //buff_addr
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = (buff_addr>>8)&0xFF;
    _at45db321_Command[3] = (buff_addr)&0xFF;
    _at45db321_Command[4] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    _at45db321_TxRx(_at45db321_Command,5,data,data_len);

    _at45db321_cs_inactive();

    return data_len;
}

//----SPI FLASH将Page数据读到缓存区--------------------------------------------
//功能：SPI FLASH将Page数据读到缓存区
//参数：buff_num    目标缓存区选择
//      page_addr   目标Page地址
//返回：false:  失败
//      true:   成功
//-----------------------------------------------------------------------------
bool_t _at45db321_Page_to_Buff(u32 buff_num,u32 page_addr)
{
    if(buff_num == AT45_Buff1)
        _at45db321_Command[0] = AT45_MainMem_Page_to_Buff1_Transfer;
    else if(buff_num == AT45_Buff2)
        _at45db321_Command[0] = AT45_MainMem_Page_to_Buff2_Transfer;
    else
    {
        return 0;
    }
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0x0;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }

    _at45db321_cs_inactive();
    return true;
}

//----SPI FLASH将缓存区数据写到Page--------------------------------------------
//功能：SPI FLASH将缓存区数据写到Page
//参数：buff_num    目标缓存区选择
//      page_addr   目标Page地址
//      With_Erase  写Page之前是否需要擦除
//返回：false:  失败
//      true:   成功
//-----------------------------------------------------------------------------
bool_t _at45db321_Buff_to_Page(u32 buff_num,u32 page_addr,u32 With_Erase)
{
    if(buff_num == AT45_Buff1)
    {
        if(With_Erase == no_Erase)
            _at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_noErase;
        else if(With_Erase == vs_Erase)
            _at45db321_Command[0] = AT45_Buff1_to_MainMem_Page_Prog_vsErase;
        else
        {
            return 0;
        }
    }
    else if(buff_num == AT45_Buff2)
    {
        if(With_Erase == no_Erase)
            _at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_noErase;
        else if(With_Erase == vs_Erase)
            _at45db321_Command[0] = AT45_Buff2_to_MainMem_Page_Prog_vsErase;
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }

#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0x0;

    if(false == _at45db321_cs_active())
        return 0;

    if(_at45db321_TxRx(_at45db321_Command,4,NULL,0))
    {
        _at45db321_cs_inactive();
        return 0;
    }

    _at45db321_cs_inactive();
    return true;
}

//----SPI FLASH Page擦除需求判断-----------------------------------------------
//功能：判断FLASH中已有的数据与需要写入的数据段之间的匹配度，判断是否需要擦除
//      FLASH相应PAGE后再写入需要写入的数据已经写入Buff，Flash中读出的数据在_at45db321_buff中
//参数：*data       需要写入的数据的起始地址
//      data_len    需要写入的数据长度
//返回：false:  需要擦除
//      true:   不需要擦除
//-----------------------------------------------------------------------------
bool_t _at45db321_Need_Erase_orNot(u8 *data,u32 data_len)
{
    u32 i;

    for(i=0;i<data_len;i++)
    {
        if(_at45db321_buff[i] != 0xFF)
            return false;
    }

    return true;
}

//----SPI FLASH Page擦除-------------------------------------------------------
//功能：SPI FLASH Page擦除
//参数：无
//返回：无
//-----------------------------------------------------------------------------

bool_t at45db321_Wait_Ready_erase(u32 Time_Out);
bool_t AT45_Page_Erase(u32 Address)
{
    u32 page_addr;

    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }

    if(false == at45db321_Wait_Ready_erase(500000))       //查忙，若超时则返回false
        return false;//超时，退出

    page_addr = _at45db321_Page_Caculate(Address);

    _at45db321_Command[0] = AT45_Page_Erase_Cmd;
#if CFG_AT45_PAGE_SIZE==512
    _at45db321_Command[1] = (page_addr>>7)&0xFF;
    _at45db321_Command[2] = (page_addr<<1)&0xFF;
#elif CFG_AT45_PAGE_SIZE==528
    _at45db321_Command[1] = (page_addr>>6)&0xFF;
    _at45db321_Command[2] = (page_addr<<2)&0xFF;
#endif
    _at45db321_Command[3] = 0xFF;

    if(false == _at45db321_cs_active())
        return 0;

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    DJY_EventDelay(50000);
    Lock_MutexPost(pAT45_Lock);
    return true;
}

//----SPI FLASH Block擦除------------------------------------------------------
//功能：SPI FLASH Block擦除
//参数：无
//返回：无
//-----------------------------------------------------------------------------
s32 AT45_Block_Erase(u32 Address)
{
    u32 block_addr;
    s32 res;
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 ,0 ,0};
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return -1;
    }

    block_addr = _at45db321_Page_Caculate(Address);
    block_addr = _at45db321_Block_Caculate(block_addr);

    command[0] = AT45_Block_Erase_Cmd;
    command[1] = (block_addr>>4)&0x3F;
    command[2] = (block_addr<<4)&0xF0;
    command[3] = 0x0;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
    {
        Lock_MutexPost(pAT45_Lock);
        return -1;//超时，退出
    }

    if(FALSE == _at45db321_cs_active())
    {
        Lock_MutexPost(pAT45_Lock);
        return -1;//超时，退出
    }
    res = SPI_Transfer(s_ptAT45_Dev, &frame, TRUE, AT45_OP_TIMEOUT);

    _at45db321_cs_inactive();

    Lock_MutexPost(pAT45_Lock);

    return res;
}

//----SPI FLASH全片擦除--------------------------------------------------------
//功能：SPI FLASH全片擦除
//参数：无
//返回：无
//-----------------------------------------------------------------------------
bool_t AT45_Chip_Erase(void)
{
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }

    _at45db321_Command[0] = AT45_Chip_Erase1;
    _at45db321_Command[1] = AT45_Chip_Erase2;
    _at45db321_Command[2] = AT45_Chip_Erase3;
    _at45db321_Command[3] = AT45_Chip_Erase4;

    if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
        return false;//超时，退出

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

    DJY_EventDelay(100000000);
    Lock_MutexPost(pAT45_Lock);

    return true;
}

//----SPI FLASH Page Size设置--------------------------------------------------
//功能：SPI FLASH Page Size设置为512。调用本函数后需要重新上电，否则操作FLASH会得到错误数据
//      注意：该过程不可逆！！AT45默认PageSize为528bytes，调用本函数可以修改为512
//参数：无
//返回：无
//-----------------------------------------------------------------------------
void _at45db321_Binary_Page_Size_512(void)
{
    _at45db321_Command[0] = 0x3D;
    _at45db321_Command[1] = 0x2A;
    _at45db321_Command[2] = 0x80;
    _at45db321_Command[3] = 0xA6;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,4,NULL,0);

    _at45db321_cs_inactive();

//  while(1);
}

//----SPI FLASH读取Status Register---------------------------------------------
//功能：SPI FLASH读取Status Register值
//参数：无
//返回：Status Register值
//-----------------------------------------------------------------------------
u32 _at45db321_Read_Status(void)
{
    u8 Data;

    _at45db321_Command[0] = AT45_Status_Register_Read;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,1,&Data,1);

    _at45db321_cs_inactive();

    return Data;
}

void _at45db321_Check_SP(void)
{
    u8 Data[64],i;

    if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
        return ;//超时，退出

    _at45db321_Command[0] = 0x32;
    _at45db321_Command[1] = 0xFF;
    _at45db321_Command[2] = 0xFF;
    _at45db321_Command[3] = 0xFF;


    _at45db321_cs_active();
    _at45db321_TxRx(_at45db321_Command,4,Data,64);
    _at45db321_cs_inactive();

    for(i = 0; i < 64; i ++)
    {
        if(Data[i] != 0)
            break;
    }

    if(i != 64)
    {

        if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
            return ;//超时，退出

        //erase sector protect
        _at45db321_Command[0] = 0x3D;
        _at45db321_Command[1] = 0x2A;
        _at45db321_Command[2] = 0x7F;
        _at45db321_Command[3] = 0xCF;



        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,NULL,0);
        _at45db321_cs_inactive();

        //programe sector protect
        memset(Data,0x00,64);
        if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
            return ;//超时，退出
        _at45db321_Command[0] = 0x3D;
        _at45db321_Command[1] = 0x2A;
        _at45db321_Command[2] = 0x7F;
        _at45db321_Command[3] = 0xFC;

        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,NULL,0);
        _at45db321_TxRx(Data,64,NULL,0);
        _at45db321_cs_inactive();

        if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
            return ;//超时，退出
        //check it
        _at45db321_Command[0] = 0x32;
        _at45db321_Command[1] = 0xFF;
        _at45db321_Command[2] = 0xFF;
        _at45db321_Command[3] = 0xFF;

        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,Data,64);
        _at45db321_cs_inactive();
    }

}

void _at45db321_Check_Lock(void)
{
    u8 Data[64];

    _at45db321_Command[0] = AT45_Status_Register_Read;
    _at45db321_cs_active();
    _at45db321_TxRx(_at45db321_Command,1,Data,2);
    _at45db321_cs_inactive();

    if(Data[1] & (0x08))//means locked down
    {
        //then read it
        _at45db321_Command[0] = 0x35;
        _at45db321_Command[1] = 0xFF;
        _at45db321_Command[2] = 0xFF;
        _at45db321_Command[3] = 0xFF;


        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,4,Data,64);
        _at45db321_cs_inactive();
    }
}
//----SPI FLASH校验芯片ID------------------------------------------------------
//功能：SPI FLASH校验芯片ID
//参数：无
//返回：true = 校验正常，false = 校验不成功
//-----------------------------------------------------------------------------
bool_t _at45db321_Check_ID(void)
{
    u8 Data[5];

    _at45db321_Command[0] = AT45_Command_ID;

    _at45db321_cs_active();

    _at45db321_TxRx(_at45db321_Command,1,Data,5);

    _at45db321_cs_inactive();

    if( (Data[0] != 0x1F) ||            //Manufacturer_ID
        (Data[1] != 0x27) ||            //Device_ID1
        (Data[2] != 0x01) ||            //Device_ID2
        (Data[3] != 0x01) ||
        (Data[4] != 0x00))              //Extended_Info
    {
        return false;
    }

    return true;    //Match SPI Flash ID successful
}

/*--------------------应用程序调用接口函数定义由此开始------------------------*/

//----SPI FLASH模块查忙--------------------------------------------------------
//功能：SPI FLASH模块查忙，读取AT45_Status，返回BUSY标志位的当前值
//参数：无
//返回：true = FLASH空闲，false = FLASH忙
//-----------------------------------------------------------------------------
bool_t at45db321_Check_Busy(void)
{
    u8 Data;

    Data = _at45db321_Read_Status();

    if( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data) )
        return false;
    else
        return true;
}
//----等待FLASH模块空闲--------------------------------------------------------
//功能：等待FLASH模块空闲，函数内部判断是否超时
//参数：无
//返回：true = FLASH空闲，false = FLASH超时异常
//-----------------------------------------------------------------------------
bool_t at45db321_Wait_Ready(u32 Time_Out)
{
    u8 Data[2],result = true;


    _at45db321_Command[0] = AT45_Status_Register_Read;

    do
    {
        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,1,Data,2);
        _at45db321_cs_inactive();

        Time_Out -= 2;
        DJY_DelayUs(2);
        if(Time_Out == 0)
        {
            result = false;
            break;
        }
    }while( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data[0]) );

    return result;
}

bool_t at45db321_Wait_Ready_erase(u32 Time_Out)
{
    u8 Data[2],result = true;

    if(Time_Out < 1000)
        Time_Out = 1000;
    _at45db321_Command[0] = AT45_Status_Register_Read;

    do
    {
        _at45db321_cs_active();
        _at45db321_TxRx(_at45db321_Command,1,Data,2);
        _at45db321_cs_inactive();

        Time_Out -= 1000;
        DJY_EventDelay(1*mS);
        if(Time_Out == 0)
        {
            result = false;
            break;
        }
    }while( AT45_Status_Reg_Bit_BUSY != (AT45_Status_Reg_Bit_BUSY & Data[0]) );

    return result;
}

//----SPI FLASH模块读函数------------------------------------------------------
//功能：从SPI FLASH的Address地址处开始，读出data_len长度的数据，数据存储首地址为data
//      若地址累加到FLASH的末尾，则会返回FLASH开头处继续读取，直到读满data_len个数据为止
//参数：Address     需要写入的地址（注意，此地址为应用程序所使用的虚拟地址，需要经过变换成为物理地址：page_addr和byte_offset_addr）
//      *data       读取数据存储位置的起始地址
//      data_len    读取数据长度
//返回：data_len = 实际读取的数据长度，false = 读取失败
//-----------------------------------------------------------------------------
u32 AT45_FLASH_Read(u32 Address,u8 *data,u32 data_len)
{
    u32 page_addr,byte_offset_addr;

    ////判断是否需要初始化SPI，以防module_init_at45db321未被调用
    if(!sAT45Inited)
    {
        return 0;
    }

    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return 0;
    }

    page_addr=_at45db321_Page_Caculate(Address);
    byte_offset_addr=_at45db321_Offset_Caculate(Address);

    if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
    {
        Lock_MutexPost(pAT45_Lock);
        return false;//超时，退出
    }

    _at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,data,data_len);

//    DJY_EventDelay(10000);
    Lock_MutexPost(pAT45_Lock);

    return data_len;
}

//----SPI FLASH模块写函数------------------------------------------------------
//功能：从SPI FLASH的Address地址处开始，写入data_len长度的数据，数据获取首地址为data
//      本函数在写入连续的、数据长度为多个Page时可以使FLASH的效率达到最优。
//      应用程序无需关心FLASH是否需要擦除后再写入
//      注意：应用程序希望写入的数据长度为data_len，但本程序不保证实际写入data_len长度的数据，
//      实际写入的数据长度会通过written_data_len返回给应用程序（根据Page大小决定的）
//参数：Address     需要写入的地址（注意，此地址为应用程序所使用的虚拟地址）
//      *data       写入数据源的起始地址
//      data_len    写入数据长度
//返回：written_data_len = 实际写入的数据长度，false = 写入失败
//-----------------------------------------------------------------------------
u32 _at45_flash_write(u32 Address,u8 *data,u32 data_len)
{
    u32 written_data_len=0,Erase_orNot=vs_Erase;
    u32 page_addr,byte_offset_addr;

    //计算Page地址及数据在Page中的偏移地址
    page_addr = _at45db321_Page_Caculate(Address);
    byte_offset_addr = _at45db321_Offset_Caculate(Address);

    //确认本次可以写入的数据长度
    written_data_len = _at45db321_Written_Caculate(byte_offset_addr,data_len);

    //1.首先，将需要写入的数据写入处于空闲状态的Buff中
    if( false == at45db321_Check_Busy() )
    {//当前Buff正在执行写入FLASH的操作，换一块Buff写入数据
        if( _at45db321_Ready_Buff==AT45_Buff1 )
            _at45db321_Ready_Buff = AT45_Buff2;
        else
            _at45db321_Ready_Buff = AT45_Buff1;
    }
    _at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);

    if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
        return false;//超时，退出

    //2.其次，从FLASH区中读出已经存在的数据,判断是否需要擦除
    _at45db321_Continuous_Array_Read(page_addr,byte_offset_addr,_at45db321_buff,written_data_len);//读出并存入_at45db321_buff
    if( false == _at45db321_Need_Erase_orNot(data,written_data_len))
    {//需要擦除
        Erase_orNot = vs_Erase;
    }
    else
    {//不需要擦除
        Erase_orNot = no_Erase;
    }

    //3.最后，完成写入操作
    //写入时，必须将flash中的数据读入到片内RAM，否则，将会将RAM中未确定的数据刷入到FLASH
//  if((( byte_offset_addr != 0 )||( written_data_len != CFG_AT45_PAGE_SIZE ))&&(Erase_orNot == vs_Erase))
    {//非整个Page的写入，擦除前需要保存
        _at45db321_Page_to_Buff(_at45db321_Ready_Buff,page_addr);
        if(false == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
            return false;//超时，退出
        _at45db321_Buff_Write(_at45db321_Ready_Buff,byte_offset_addr,data,written_data_len);
    }

    _at45db321_Buff_to_Page(_at45db321_Ready_Buff,page_addr,Erase_orNot);
    //此处，不需要等待完成。在进行下一轮FLASH操作时再判断

    return written_data_len;
}

// =============================================================================
// 功能：写flash，分 页写,将data_len的数据写入到Address的起始地址
// 参数：Address，起始地址
//      data，数据指针
//      data_len，数据长度，字节数
// 返回：实际写入的数据长度
// =============================================================================
u32 AT45_FLASH_Write(u32 Address,u8 *data,u32 data_len)
{
    u32 wsize,temp;
    ////判断是否需要初始化SPI，以防module_init_at45db321未被调用
    if(!sAT45Inited)
    {
        return false;
    }
    if(false == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        return false;
    }
    temp = data_len;
    while(temp)
    {
        wsize = _at45_flash_write(Address,data,temp);
        if(!wsize)
            break;
        Address += wsize;
        data    += wsize;
        temp -= wsize;
    }
    DJY_EventDelay(10000);
    Lock_MutexPost(pAT45_Lock);
    return data_len - temp;
}

// =============================================================================
// 功能：返回AT45DB321是否已经初始化
// 参数：无
// 返回：初始化状态
// =============================================================================
bool_t AT45_FLASH_Ready(void)
{
    return sAT45Inited;
}

// =============================================================================
// 功能：安装at45驱动
// 参数：
// 返回：成功（true）；失败（false）；
// 备注：
// =============================================================================
bool_t ModuleInstall_at45db321(void)
{
    pAT45_Lock = Lock_MutexCreate("AT45 Lock");
    if(!pAT45_Lock)
    {
        printf("\r\n: error : device : cannot create AT45 flash lock.");
        return false;
    }

    s_ptAT45_Dev = SPI_DevAdd(CFG_AT45_BUSNAME,At45Name,0,8,SPI_MODE_0,SPI_SHIFT_MSB,AT45_SPI_SPEED,false);
    if(s_ptAT45_Dev != NULL)
    {
        SPI_BusCtrl(s_ptAT45_Dev, CN_SPI_SET_POLL, 0, 0);
    }
    else
    {
        printf("\r\n: error  : device : AT45DB321E init failed.\n\r");
        return false;
    }

    if(false == _at45db321_Check_ID())  //校验芯片ID
        return false;

    if( !(_at45db321_Read_Status() & AT45_Status_Reg_Bit_PGSZ) )//转换成512字节
    {
        _at45db321_Binary_Page_Size_512();//不可逆，且需重新上电
        printf("\r\n: info  : device : AT45DB321 page size 改变，请重新上电\n\r");
    }

    if(!nordescription) //初始化nor的信息
    {
        nordescription = malloc(sizeof(struct NorDescr));
        if(!nordescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }

        memset(nordescription, 0x0, (sizeof(struct NorDescr)));

        // AT45的sector比block大，而且sector的大小不一致。这里逻辑上就将sector等于page，
        // 忽然sector,block最大。
        nordescription->PortType = NOR_SPI;
        nordescription->Port = s_ptAT45_Dev;
        nordescription->BytesPerPage = CFG_AT45_PAGE_SIZE;
        nordescription->PagesPerSector = 1;
        nordescription->SectorsPerBlk = 8;
        nordescription->Blks = 1024; // 全部器件的容量
        nordescription->ReservedBlks = 0;
    }

    if(CFG_AT45_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __at45_req(format, 0 , -1, &sz))
        {
            warning_printf("at45"," Format failure.");
        }
    }

    at45_umedia = malloc(sizeof(struct umedia)+nordescription->BytesPerPage);
    if(!at45_umedia)
        return false;

    at45_umedia->mreq = __at45_req;
    at45_umedia->type = nor;
    at45_umedia->ubuf = (u8*)at45_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)At45Name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)at45_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", At45Name);
        free(at45_umedia);
        return false;
    }

    sAT45Inited = true;
    return true;
}
// =============================================================================
// 功能：判断at45是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t At45_is_install(void)
{
    return sAT45Inited;
}

//文件系统使用的相关接口
#if 1
// ============================================================================
// 功能：at45命令
// 参数：cmd -- 命令；
//      args -- 可变参，命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __at45_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 *unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            *block = *unit / nordescription->SectorsPerBlk;     //获取当前页属于哪一块
            break;
        }

        case totalblocks:
        {
            *((u32*)args) =  nordescription->Blks;      //1个扇区里有多少块
            break;
        }

        case blockunits:
        {
            *((u32*)args) = nordescription->SectorsPerBlk;      //1个块里有多少页
            break;
        }

        case unitbytes:
        {
            *((u32*)args) = nordescription->BytesPerPage;   //每页的字节数
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            struct uesz *sz;

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            if(-1==end)
                end = nordescription->Blks;

            do
            {
                if(__at45_erase((s64)--end, *sz))
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);      //擦除指定区域

            break;
        }

        case remain:
        {
            va_list list;
            u32 *left;
            s64 *unit;

            left = (u32*)args;
            va_start(list, args);
            unit = (s64*)va_arg(list, u32);
            va_end(list);
            //当前页所属的块在还中还有多少页没用
            *left = (nordescription->SectorsPerBlk - 1)- (*unit % nordescription->SectorsPerBlk);

            break;
        }

        case checkbad: break; // 检查坏块
        default: res = -1; break;
    }

    return (res);
}
// ============================================================================
// 功能：差AT45的页或块
// 参数：unit -- 页或者块；
//      sz -- 页或者块；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __at45_erase(u32 unit, struct uesz sz)
{
    u32 Address;

    if(sz.unit)
        Address = unit / 8; // 当前页属于第几块
    else if (sz.block)
        Address = unit;

    Address = (Address << 3) * CFG_AT45_PAGE_SIZE;      //获取擦除块的首地址
    return (AT45_Block_Erase(Address));
}
// ============================================================================
// 功能：读AT45
// 参数：unit -- 页号；data --存读到数据的缓存；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：
// ============================================================================
s32 __at45_read(u32 unit, void *data, struct uopt opt)
{
    s32 res;
    u32 Address;
    Address = unit * CFG_AT45_PAGE_SIZE;    //算出当前页的首地址
    res = AT45_FLASH_Read(Address, (u8*)data, CFG_AT45_PAGE_SIZE);
    if(res != CFG_AT45_PAGE_SIZE)
    {
        return -1;
    }
    return 0;
}

// ============================================================================
// 功能：写AT45
// 参数：unit -- 页号；data -- 要写的数据；
// 返回：成功 -- （0）；失败 -- （-1）
// 备注：用于文件系统的at45写操作没有直接用上面的接口，而是重新写了。这是因为文件系统设计的对flash的写操作和
// AT45_FLASH_Write函数存在冲突。文件系统写flash时每次往里面写的数据长短不一，并且会写多次，当1页满后换
// 下一页，所以会出现往1页中多次写数据的情况，并且文件系统每次都是按页来操作的，当文件系统对1页写第二次时。
// 会自动偏移到第一次写完数据的后面，不会覆盖第一次写的数据，第一次写的数据位置会自动填充0XFF（flash的位只能由1变为0）.
// 而AT45_FLASH_Write接口在写数据时，会从指定的地址开始写，写之前会从指定的地址开始读数据，读指定长度，当发现
// 这段地址范围里的数据不是全FF时会把该范围擦除，然后写入数据。
// 这样就会产生一个问题文件系统会往一个页中写多次，页的地址是固定的，当文件系统第二次写该页的时候，然后用AT45_FLASH_Write
// 的话，AT45_FLASH_Write会先把数据读出来，发现里面不是全FF（因为有第一次写的数据），所有会擦除一遍，这样会导致第一次
// 写的内容没了。
// ============================================================================
s32 __at45_write(u32 unit, void *data, struct uopt opt)
{
    s32 res;
    struct SPI_DataFrame frame;
    u8 command[] = {0, 0 ,0 ,0};

    command[0] = JEDEC_PAGE_WRITE_DIRECT_BY_BUF1;
    command[1] = (unit >> 0x7) & 0x3F;
    command[2] = (unit << 0x1) & 0xFE;
    command[3] = 0x00;

    frame.RecvBuf = NULL;
    frame.RecvLen = 0;
    frame.RecvOff = 0;
    frame.SendBuf = command;
    frame.SendLen = 4;

    if(true == Lock_MutexPend(pAT45_Lock, AT45_OP_TIMEOUT))
    {
        if(true == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
        {
            if(true == _at45db321_cs_active())
            {
                res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);    //先发送命令
                if(CN_SPI_EXIT_NOERR == res)
                {
                    frame.RecvBuf = NULL;
                    frame.RecvLen = 0;
                    frame.RecvOff = 0;
                    frame.SendBuf = data;
                    frame.SendLen = nordescription->BytesPerPage;

                    res = SPI_Transfer(nordescription->Port, &frame, TRUE, AT45_OP_TIMEOUT);    //再发送要想的数据
//                    if(CN_SPI_EXIT_NOERR == res)
//                        res = nordescription->BytesPerPage;
                }
                _at45db321_cs_inactive();
//                DJY_EventDelay(4000);// 延时切出. 4ms
//                if(true == at45db321_Wait_Ready(500000))       //查忙，若超时则返回false
//                {
                Lock_MutexPost(pAT45_Lock);
                if(res != CN_SPI_EXIT_NOERR)
                {
                    return -1;
                }
                return 0;
//                }
            }
            Lock_MutexPost(pAT45_Lock);
        }
    }
    return -1;
}

// ============================================================================
// 功能：初始化SPI FLASH模块，用做文件系统
// 参数：   fs -- 该媒体所要安装文件系统mount点名字；
//      mediadrv -- 媒体驱动
//      dwStart -- 起始块；
//      dwEnd -- 结束块数（擦除时不包括该块，只擦到该块的上一块）；
// 返回：成功初始化（0）；初始化失败（-1）；
// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
// ============================================================================
s32 __AT45_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return -1;

    targetobj = OBJ_MatchPath(fs, &notfind);    //根据mount点名字找mount点的obj
    if(notfind)
    {
        error_printf("at45"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj); //获取obj的私有数据
    super->MediaInfo = at45_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = nordescription->Blks;
        BlockNum = dwEnd - dwStart;     //获取文件系统一共用了多少块
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * (nordescription->BytesPerPage * nordescription->SectorsPerBlk);
    super->MediaStart = dwStart * nordescription->SectorsPerBlk; // 起始unit号

    res = strlen(At45Name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,At45Name);      //获取设备的全路径
    File_BeMedia(FullPath,fs);     //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}
#endif
