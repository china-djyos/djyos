//-----------------------------------------------------------------------------

// 文件名     ：cpu_peri_busram.c
// 模块描述: CPU片内backup 1K SRAM读写操作
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 09/11.2015
// =============================================================================

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "os.h"
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"cpu onchip Backup area sram"//电池保持的4K片内ram读写
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                        //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                     //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                       //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                       //初始化时机，可选值：early，medium，later, pre-main。
                                       //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件将强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                       //选中该组件时，被依赖组件不会被强制选中，
                                       //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                       //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM == false )
//#warning  " cpu_onchip_Backup_area_sram  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_BACKUP_AREA_SRAM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure
#define BACKUPSRAM_ADDR     (0x40074000)        //片内的1K Backup SRAM起始地址
#define BACKUPSRAM_SIZE     (0x400)             //1K字节的数据

// =============================================================================
// 功能：读片内的1K Bytes地址空间的数据，该区域数据掉电保护
// 参数：rBuffer，存储数据的缓冲区
//      offset，相对存储起始位置偏移，32位对齐
//      len，存储的数据长度
// 返回：写入的数据DWORD（4字节）数
// =============================================================================
u32 BackUpSram_Read(u8 *rBuffer,u32 offset,u32 len)
{
    u32 i,addr,dw;
    if( (NULL == rBuffer) || (offset + len >= BACKUPSRAM_SIZE) || (offset%4 != 0))
        return 0;

    addr = BACKUPSRAM_ADDR + offset;
    for(i = 0; i < len/4; i++)      //copy 整4倍数的部分
    {
        dw = *((u32*)(addr + i*4));
        rBuffer[4*i] = dw&0xff;
        rBuffer[4*i+1] = (dw>>8)&0xff;
        rBuffer[4*i+2] = (dw>>16)&0xff;
        rBuffer[4*i+3] = (dw>>24)&0xff;
    }

    dw = *((u32*)(addr + i*4));
    for(i = i*4; i < len; i++)      //copy剩余部分
    {
        rBuffer[i] = dw & 0xff;
        dw >>= 8;
    }
    return i;
}

// =============================================================================
// 功能：写片内的1K Bytes地址空间的数据，该区域数据掉电保护
// 参数：rBuffer，存储数据的缓冲区
//      offset，相对存储起始位置偏移，32位对齐
//      len，存储的数据长度
// 返回：写入的数据DWORD（4字节）数
// =============================================================================
u32 BackUpSram_Write(u8 *wBuffer,u32 offset,u32 len)
{
    u32 i,addr,dw;
    if( (NULL == wBuffer) || (offset + len >= BACKUPSRAM_SIZE) || (offset%4 != 0))
        return 0;

    addr = BACKUPSRAM_ADDR + offset;
    for(i = 0; i < len/4; i++)      //copy 整4倍数的部分
    {
        dw =  wBuffer[4*i]
            + (wBuffer[4*i+1]<<8)
            + (wBuffer[4*i+2]<<16)
            + (wBuffer[4*i+3]<<24);
        *(u32*)(addr+4*i) = dw;
    }

    for(i = i*4; i < len; i++)      //copy剩余部分
    {
        dw = wBuffer[i]<<(i*8);
    }
    *(u32*)(addr+(i&~0x3)) = dw;
    return i;
}
