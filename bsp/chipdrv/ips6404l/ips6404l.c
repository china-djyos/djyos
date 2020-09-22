// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：gd25q64c.c
// 模块描述: Qflash，使用QSPI通信
// 模块版本: V1.00
// 创建人员: wangxy
// 创建时间: 2018.08.30
// =============================================================================

#include "stdint.h"
#include "cpu_peri.h"
#include "systime.h"
#include "stddef.h"
#include <string.h>
#include <stdlib.h>
#include <djyos.h>
#include <math.h>
#include "lock.h"
#include "spibus.h"
#include "djybus.h"
#include "driver.h"
#include <device/include/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "ips6404l.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern s32 ModuleInstall_Ips6404l(void);
//    ModuleInstall_Ips6404l();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"Ips6404l"//SPI和QSPI接口的ram芯片
//parent:"none"//填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none",//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"cpu onchip spi","cpu onchip qspi"        //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NORFLASH_IPS6404 == false )
//#warning  " SPIRAM_IPS6404  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NORFLASH_IPS6404    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@string,1,20,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

struct MutexLCB *ips6404l_lock;           //芯片互斥访问保护
u16 ips6404l_type          = 0x0d5d;
//=====================================================================
//函数名： 片选控制,在board中定义
//参数：  0 -- 拉低；1 -- 拉高。
//返回值：
//功能：片选拉低，选中器件
//=====================================================================
__attribute__((weak)) void Ips6404l_Cs(u8 flag)
{
//    gpio_output(GPIO26, flag);
    printf("board undefined Ips6404l_Cs \r\n");
}

//=====================================================================
//函数名：初始化用到的IO，在board中定义
//参数：
//返回值：
//功能：初始化片选
//=====================================================================
__attribute__((weak)) void Ips6404l_GpioInit(void)
{
    printf("board undefined Ips6404l_GpioInit \r\n");
//    gpio_config(GPIO26, GMODE_OUTPUT);
//    gpio_output(GPIO26, 1);
}


//=====================================================================
//函数名：SPI发送接收函数
//参数：发送的数据地址、数据长度、接受的数据地址、数据长度、偏移量（发送几个数之后再开始发一个收一个，前面是空读）
//返回值：true：成功 ； false:失败
//功能：SPI发送接收
//=====================================================================
static bool_t Ips6404l_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return false;
    return true;
}

//=====================================================================
//函数名：写一页
//参数：发送缓冲区，页号
//返回值：true -- 成功； false -- 失败
//功能：
//=====================================================================
bool_t Ips6404l_Write(u32 WriteAddr, u8* pBuffer, u32 len)
{
    u8 *sndbuf = NULL;
    Lock_MutexPend(ips6404l_lock,CN_TIMEOUT_FOREVER);

    sndbuf = malloc(4 + len);
    if(sndbuf)
    {
        sndbuf[0] = Ips6404l_WriteData;
        sndbuf[3] = WriteAddr & 0xff;
        sndbuf[2] = (WriteAddr >> 8) & 0xff;
        sndbuf[1] = (WriteAddr >> 16) & 0xff;
        memcpy(sndbuf + 4, pBuffer, len);

        Ips6404l_Cs(0);

        if(Ips6404l_TxRx(sndbuf, 4 + len ,NULL,0,0) == false)
        {
            Ips6404l_Cs(1);
            free(sndbuf);
            Lock_MutexPost(ips6404l_lock);
            error_printf("Ips64"," write fail.\r\n");
            return false;
        }

        Ips6404l_Cs(1);
        free(sndbuf);
    }
    else
    {
        Lock_MutexPost(ips6404l_lock);
        error_printf("Ips64"," sndbuf malloc fail.\r\n");
        return false;
    }

    Lock_MutexPost(ips6404l_lock);
    return true;

}


//=====================================================================
//函数名：读一页
//参数：接收缓冲区，页号
//返回值： true:成功；false：失败
//功能：
//=====================================================================
bool_t Ips6404l_Read(u32 ReadAddr, u8* pBuffer, u32 len)
{
    u8 sndbuf[4];
    Lock_MutexPend(ips6404l_lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = Ips6404l_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    Ips6404l_Cs(0);

    if(Ips6404l_TxRx(sndbuf,4,pBuffer,len,4) == false)
    {
        Ips6404l_Cs(1);
        Lock_MutexPost(ips6404l_lock);
        error_printf("Ips64"," read fail.\r\n");
        return false;
    }

    Ips6404l_Cs(1);

    Lock_MutexPost(ips6404l_lock);
    return true;
}
//=====================================================================
//函数名：读芯片ID
//参数：
//返回值：其它:成功；-1：失败
//功能：
//=====================================================================
u16 Ips6404l_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[2];
    u16 deviceid;

    sndbuf[0]=Ips6404l_ManufactDeviceID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    Ips6404l_Cs(0);
    if(Ips6404l_TxRx(sndbuf,4,rcvbuf,2,4) == false)
    {
        Ips6404l_Cs(1);
        return -1;
    }
    Ips6404l_Cs(1);

    deviceid=(rcvbuf[0]<<8)|rcvbuf[1];
    return deviceid;
}


// =============================================================================
// 功能：初始化QSPI FLASH模块，校验芯片ID是否正确
// 参数：无
// 返回：成功（0）；失败（-1）；
// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
// =============================================================================
s32 ModuleInstall_Ips6404l(void)
{
    u16 temp;

    Ips6404l_GpioInit();
    temp=Ips6404l_ReadID();

    if(temp==ips6404l_type)
   {
        printf("Ips6404l Read ID Success,ID:%x\r\n",ips6404l_type);
    }
    else
    {
       printf("Ips6404l Read ID Error,True ID:%x",ips6404l_type);
       printf("    Read ID:%x\r\n",temp);
       return -1;
    }
    ips6404l_lock = Lock_MutexCreate("gd25q64c Lock");
    if(!ips6404l_lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create Ips6404l flash lock.");
        return -1;
    }
    return 0;
}



