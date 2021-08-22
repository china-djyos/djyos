// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：gd25q64c.c
// 模块描述: Qflash，使用QSPI通信
// 模块版本: V1.00
// 创建人员: wangxy
// 创建时间: 2018.08.30
// =============================================================================


#include "gd25q64c.h"

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
#include "shell.h"
#include <device/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Gd25q64c(void);
//    ModuleInstall_Gd25q64c();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nor flash gd25q64c"//spi接口的norflash
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","heap","spi bus","cpu onchip spi"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"easy file system"     //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_GD25_PART_FORMAT           //****检查参数是否已经配置好
#warning   gd25q64c组件参数未配置，使用默认值
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_GD25_PART_FORMAT               false      //分区选项,是否需要擦除该芯片。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//*****************************************************************//
//QFLASH说明：目前配置下，本芯片：flash容量8M，一个扇区16页，一页256字节数据，一个块16扇区，一共128个块
//在读写操作前需根据芯片手册中每个指令的时序图使用QSPI_Send_CMD进行配置，例如：
//QPI,四线读数据,地址为ReadAddr,1线传输指令_1线传输24位地址_8空周期_4线传输数据,NumByteToRead个数据
//随后使用QSPI_Receive或QSPI_Transmit
//*****************************************************************//


//struct SemaphoreLCB pgd25q64c_Lock;   //芯片互斥访问保护
struct MutexLCB *pgd25q64c_Lock;           //芯片互斥访问保护
bool_t gd25q64c_Init_Flag=false;
u16 gd25q64c_type          = 0xc816;
u8 gd25q64c_MODE       = 0;            //QSPI模式标志:0,SPI模式;1,QPI模式.

//struct FlashDescrible
//{
//    u32 gd25q64c_BlockrNum        = 128;          //一共128块
//    u32 gd25q64c_SectorsPerBlock  = 16;          //一个块16扇区
//    u32 gd25q64c_SectorNum        = 2048;          //一共2048个扇区
//    u32 gd25q64c_SectorSize       = 4096;          //扇区大小4096字节
//    u32 gd25q64c_PagesPerSector   = 16;          //一个扇区16页
//    u32 gd25q64c_PageSize         = 256;          //一页256字节数据
//};

u32 gd25q64c_BlockrNum        = 64;          //一共128块
u32 gd25q64c_SectorsPerBlock  = 16;          //一个块16扇区
u32 gd25q64c_SectorNum        = 1024;          //一共2048个扇区
u32 gd25q64c_SectorSize       = 4096;          //扇区大小4096字节
u32 gd25q64c_PagesPerSector   = 16;          //一个扇区16页
u32 gd25q64c_PageSize         = 256;          //一页256字节数据

u32 gd25q64c_OP_TIMEOUT= 1800000; // 30分钟
static char *flash_name = "gd25q64c";
struct NorDescr *gd25q64_des = NULL;
struct umedia *gd25_umedia;
extern struct Object *s_ptDeviceRoot;

bool_t Gd25q64c_Write_Enable(void);
bool_t Gd25q64c_WaitReady(u32 timeout);
//=====================================================================
//函数名： 片选拉低,在board中定义
//参数：
//返回值：
//功能：片选拉低，选中器件
//=====================================================================
__attribute__((weak)) void Gd25q64c_CsActive(void)
{
//    u32 param;
//    param = 0x2;
//    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    printf("board undefined gd25q64c_CsActive \r\n");
}
//=====================================================================
//函数名：片选拉高，在board中定义
//参数：
//返回值：
//功能：片选拉高，结束操作
//=====================================================================
__attribute__((weak)) void Gd25q64c_CsInactive(void)
{
//    u32 param;
//    param = 0x3;
//    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
    printf("board undefined Gd25q64c_CsInactive \r\n");
}

//=====================================================================
//函数名：初始化用到的IO，在board中定义
//参数：
//返回值：
//功能：初始化片选
//=====================================================================
__attribute__((weak)) void Gd25q64c_GpioInit(void)
{
    printf("board undefined Gd25q64c_GpioInit \r\n");
//    bk_gpio_config_output(GPIO36);
//    bk_gpio_output(GPIO36, GPIO_INT_LEVEL_LOW);
}


//=====================================================================
//函数名：SPI发送接收函数
//参数：sdata,待发送的数据地址、
//      slen，数据长度、
//      rdata，接受的数据地址、
//      rlen，接收数据长度、
//      RecvOff，接收偏移量（发送多少个数之后再开始发一个收一个，前面是空读）
//返回值：true：成功 ； false:失败
//功能：SPI发送接收
//=====================================================================
static bool_t Gd25q64c_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return false;
    return true;
}

//=====================================================================
//函数名：读状态寄存器指令
//参数：regno，想读的状态寄存器，如 gd25q64c_ReadStatusReg1
//返回值：true：成功 ； false失败
//功能：
//=====================================================================
bool_t Gd25q64c_ReadSR(u8 regno, u8 *sta)
{
    u8 command;
//    u8 byte;
    switch(regno)
    {
        case StatusReg1:
            command = gd25q64c_ReadStatusReg1;           //读状态寄存器1指令
            break;
        case StatusReg2:
            command = gd25q64c_ReadStatusReg2;           //读状态寄存器2指令
            break;
        case StatusReg3:
            command = gd25q64c_ReadStatusReg3;           //读状态寄存器3指令
            break;
        default:
            command = gd25q64c_ReadStatusReg1;
            break;
    }

    Gd25q64c_CsActive();
    if(Gd25q64c_TxRx(&command,1,sta,1,1) == false)
    {
        Gd25q64c_CsInactive();
        return false;
    }
    Gd25q64c_CsInactive();
    return true;
//    return byte;
}

//=====================================================================
//函数名：写状态寄存器指令
//参数：regno -- 寄存器号，data -- 写的数据
//返回值：true：成功 ； false失败
//功能：
//=====================================================================
bool_t Gd25q64c_WriteSR(u8 regno, u8 data)
{
    u8 command=0;
    u8 sndbuf[2];
    switch(regno)
    {
        case 1:
            command=gd25q64c_WriteRegister1;    //写状态寄存器1
            break;
        case 2:
            command=gd25q64c_WriteRegister2;    //写状态寄存器2
            break;
        case 3:
            command=gd25q64c_WriteRegister3;    //写状态寄存器3
            break;
        default:
            command=gd25q64c_WriteRegister1;
            break;
    }

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Write Reg.");
        return -1;
    }

    if(Gd25q64c_Write_Enable() != true)                  //写使能
    {
        return -1;
    }

    sndbuf[0] = command;
    sndbuf[1] = data;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,2,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        return -1;
    }

    Gd25q64c_CsInactive();

    return 0;
}

////=====================================================================
////函数名：写使能等待
////参数：
////返回值：true：成功 ； false：失败
////功能：
////=====================================================================
//bool_t Gd25q64c_WriteEnableWait(u32 timeout)
//{
//    u8 sr1=1;
//    u32 timeoutcnt=0;
//
//    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//        return false;
//    while(1)   // 等待写使能位置位
//    {
//        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//            return false;
//        if( (sr1 & 0x02) == 0x02)
//            return true;
//        DJY_EventDelay(10*mS);
//        timeoutcnt += 10*mS;
//        if(timeoutcnt >= timeout)
//        {
//            return false;
//        }
//    }
//    return true;
//}
//
//
////=====================================================================
////函数名：写失能等待
////参数：
////返回值：true：成功 ； false：失败
////功能：
////=====================================================================
//bool_t Gd25q64c_WriteDisableWait(u32 timeout)
//{
//    u8 sr1=1;
//    u32 timeoutcnt=0;
//
//    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//        return false;
//    while(1)   // 等待BUSY位清空
//    {
//        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
//            return false;
//        if( (sr1 & 0x02) != 0x02)
//            return true;
//        DJY_EventDelay(10*mS);
//        timeoutcnt += 10*mS;
//        if(timeoutcnt >= timeout)
//        {
//            return false;
//        }
//    }
//    return true;
//}

//=====================================================================
//函数名：写使能
//参数：
//返回值：true：成功 ； false：失败
//功能：想往QFLASH里写东西前都要先使能
//=====================================================================
bool_t Gd25q64c_Write_Enable(void)
{
    u8 sndbuf[1];
    u8 sr1=1;
    u32 timeoutcnt=0;

    sndbuf[0] = gd25q64c_WriteEnable;

    while(1)   // 等待写使能位置位
    {
        Gd25q64c_CsActive();

        Gd25q64c_TxRx(sndbuf,1,NULL,0,0);

        Gd25q64c_CsInactive();
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x02) == 0x02)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= 80000*mS)
        {
            printf("\r\n gd25q64c Write Enable fail.\r\n");
            return false;
        }
    }
    return true;

}


//=====================================================================
//函数名：写失能
//参数：
//返回值：true：成功 ； false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_Write_Disable(void)

{
    u8 sndbuf[1];
    u8 sr1=1;
    u32 timeoutcnt=0;

    sndbuf[0] = gd25q64c_WriteDisable;

    while(1)   // 等待写使能位清除
    {
        Gd25q64c_CsActive();

        Gd25q64c_TxRx(sndbuf,1,NULL,0,0);

        Gd25q64c_CsInactive();
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x02) != 0x02)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= 80000*mS)
        {
            printf("\r\n gd25q64c Write Enable fail.\r\n");
            return false;
        }
    }
    return true;

}
//=====================================================================
//函数名：等待QFLASH的busy位清空
//参数：
//返回值：true ：成功；false ：失败
//功能：
//=====================================================================
bool_t Gd25q64c_WaitReady(u32 timeout)
{
    u8 sr1=1;
    u32 timeoutcnt=0;

    if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
        return false;
    while(1)   // 等待BUSY位清空
    {
        if(Gd25q64c_ReadSR(StatusReg1, &sr1) == false)
            return false;
        if( (sr1 & 0x01) != 0x01)
            return true;
        DJY_EventDelay(10*mS);
        timeoutcnt += 10*mS;
        if(timeoutcnt >= timeout)
        {
            return false;
        }
    }
    return true;
}

//=====================================================================
//函数名：扇区擦除
//参数：扇区号
//返回值：true：成功 ； false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_Erase_Sector(u32 SectorNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)    //超时等于最大操作时间
    {
        Lock_MutexPost(pgd25q64c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Dst_Addr = SectorNum * gd25q64c_SectorSize;

    sndbuf[0] = gd25q64c_SectorErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();


//  Gd25q64c_WaitReady(200*mS);    //扇区擦除时间是200mS
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//函数名：块擦除
//参数：块号
//返回值：true:成功；false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_Erase_Block(u32 BlockNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)    //超时等于最大操作时间
    {
        Lock_MutexPost(pgd25q64c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Dst_Addr = BlockNum * gd25q64_des->SectorsPerBlock * gd25q64c_SectorSize;

    sndbuf[0] = gd25q64c_BlockErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();


//  Gd25q64c_WaitReady(2000*mS);   //32K block擦除时间是1S，64K的是1.5S
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//函数名：芯片擦除
//参数：
//返回值：true:成功；false：失败
//功能：擦出全部内容
//=====================================================================
bool_t Gd25q64c_Erase_Chip(void)
{
    u8 status = 0;
    u32 time = 0;
    u8 sndbuf[1];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    if(Gd25q64c_WaitReady(80000*mS) == false)   //4Mbytes的芯片擦除时间最大80S
    {
        printf("\r\n FLASH : debug : device is busy before Chip Erase.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    sndbuf[0] = gd25q64c_ChipErase;

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,1,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    printf("\r\n gd25q64c Erase, Please Wait.");
    Gd25q64c_WaitReady(80000*mS);   //4Mbytes的芯片擦除时间最大80S

    printf("\r\ngd25q64c Erase Chip Succeed\r\n");
    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}



//=====================================================================
//函数名：写一页，写之前确保无须擦除
//参数：pBuffer，待写入的数据
//      PageNum，页号
//返回值：true -- 成功； false -- 失败
//功能：
//=====================================================================
bool_t Gd25q64c_WritePage(u8* pBuffer,u32 PageNum)
{
    u8 sndbuf[4 + 256];
    u32 WriteAddr;
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    WriteAddr = PageNum * gd25q64c_PageSize;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Write.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    if(Gd25q64c_Write_Enable() != true)                  //写使能
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    sndbuf[0] = gd25q64c_PageProgram;
    sndbuf[3] = WriteAddr & 0xff;
    sndbuf[2] = (WriteAddr >> 8) & 0xff;
    sndbuf[1] = (WriteAddr >> 16) & 0xff;
    memcpy(sndbuf + 4, pBuffer, gd25q64c_PageSize);

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf, 4 + 256 ,NULL,0,0) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;

}

//=====================================================================
//函数名：不带擦除功能的写，写之前需要确保所写的地址范围内的数据全为0xff，否则会写入失败
//参数：pBuffer，待写入的数据
//      WriteAddr，写入地址
//      NumByteToWrite，数据长度
//返回值：true:成功；false：失败
//功能：地址就是从QFLASH的第N个字节开始
//=====================================================================
bool_t Gd25q64c_WriteNoErase(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite)
{
    u32 remain = 0, page_num = 0;
    bool_t ret = false;
    u8 write_buf[256];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    page_num = WriteAddr / 256;
    remain = 256 - WriteAddr % 256;  //单页还剩余的字节数
    if(NumByteToWrite <= remain)
        remain = NumByteToWrite;
    while(1)
    {
        if(remain == 256)
        {
            if(Gd25q64c_WritePage(pBuffer, page_num))
            {
                if(remain == NumByteToWrite)
                {
                    ret = true;
                    break;
                }
                else
                {
                    pBuffer += remain;
                    page_num ++;
                    NumByteToWrite -= remain;
                    if(NumByteToWrite > 256)
                        remain = 256;
                    else
                        remain = NumByteToWrite;
                }
            }
            else
                break;
        }
        else
        {
            memset(write_buf, 0xff, sizeof(write_buf));
            memcpy(write_buf, pBuffer, remain);
            if(Gd25q64c_WritePage(write_buf, page_num))
            {
                ret = true;
                break;
            }
            else
                break;
        }


    }
    Lock_MutexPost(pgd25q64c_Lock);
    return ret;

}

//=====================================================================
//函数名：从某一地址开始写,写前先判断待写入的内容是否存在把原有内容从0改为1的比特，
//      若有则先擦除整扇区，否则直接写入
//参数：发送缓冲区，写地址，数据长度
//返回值： true:成功；false：失败
//功能：
//=====================================================================
u8 Gd25Q64C_BUFFER[4096];
bool_t Gd25q64c_Write(u8* pBuffer,u32 WriteAddr,u32 len)
{
    u32 sec;
    u16 sec_off;
    u16 sec_remain;
    u16 i;
    u8 data;
    bool_t ret = true,temp;
    u32 t,t1,t2;

    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    sec = WriteAddr / gd25q64c_SectorSize;          //扇区地址
    sec_off = WriteAddr % gd25q64c_SectorSize;      //扇区偏移
    sec_remain = gd25q64c_SectorSize- sec_off;      //扇区剩余大小

    if(len <= sec_remain)
        sec_remain = len;

    while(1)
    {
//        extern u32 timesectorread,timesectorerase,timesectorwrite;       //toto,lst dbg
//        t = DJY_GetSysTime();
        temp = Gd25q64c_Read(Gd25Q64C_BUFFER, sec * gd25q64c_SectorSize, gd25q64c_SectorSize);
//        t1 = DJY_GetSysTime();
//        timesectorread += gd25q64c_SectorSize;
        if(temp)
        {
            for(i=0; i < sec_remain; i++)
            {
                data = Gd25Q64C_BUFFER[sec_off + i];
                if((data | pBuffer[i]) != data)     //检查是否有从0改为1的bit
                    break;
            }
            if(i < sec_remain)    //需要擦除
            {
                temp = Gd25q64c_Erase_Sector(sec);
//                t = DJY_GetSysTime();
//                timesectorerase ++;
                if(temp)
                {
                    for(i=0; i<sec_remain; i++)
                    {
                        Gd25Q64C_BUFFER[i + sec_off] = pBuffer[i];
                    }
                    temp = Gd25q64c_WriteNoErase(Gd25Q64C_BUFFER, sec*4096, 4096);
//                    t1 = DJY_GetSysTime();
//                    timesectorwrite += t1 - t;
                    if(temp == false)
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
                temp = Gd25q64c_WriteNoErase(pBuffer, WriteAddr, sec_remain);
//                t = DJY_GetSysTime();
//                timesectorwrite +=sec_remain;
                if(temp == false)
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

                pBuffer += sec_remain;
                WriteAddr += sec_remain;
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

    Lock_MutexPost(pgd25q64c_Lock);
    return ret;
}

//=====================================================================
//函数名：读一页
//参数：pBuffer，接收缓冲区
//      PageNum，页号
//返回值： true:成功；false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_ReadPage(u8* pBuffer,u32 PageNum)
{
    u32 ReadAddr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    ReadAddr = PageNum * gd25q64c_PageSize;

    sndbuf[0] = gd25q64c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Read.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,pBuffer,256,4) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;
}

//=====================================================================
//函数名：从某一地址开始读
//参数：PBuffer，接收缓冲区，
//      ReadAddr，起始地址，
//      NumByteToRead，读取数量
//返回值：true:成功；false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_Read(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead)
{
    u8 sndbuf[4];

//    u32 t;
//    extern u32 timewait;       //toto,lst dbg
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = gd25q64c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

//    t = DJY_GetSysTime();
    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Memory Read.");
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }
//    timewait += DJY_GetSysTime() - t;
    Gd25q64c_CsActive();

    if(Gd25q64c_TxRx(sndbuf,4,pBuffer,NumByteToRead,4) == false)
    {
        Gd25q64c_CsInactive();
        Lock_MutexPost(pgd25q64c_Lock);
        return false;
    }

    Gd25q64c_CsInactive();

    Lock_MutexPost(pgd25q64c_Lock);
    return true;

#if 0
    Lock_MutexPend(pgd25q64c_Lock,CN_TIMEOUT_FOREVER);
    if(QSPI_Send_CMD(gd25q64c_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE) != 0)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return -1;
    }
    //QPI,快速读数据,地址为ReadAddr,1线传输数据_24位地址_1线传输地址_1线传输指令,8空周期,NumByteToRead个数据
    if(QSPI_Receive(pBuffer,NumByteToRead) != 0)
    {
        Lock_MutexPost(pgd25q64c_Lock);
        return -1;
    }
    Lock_MutexPost(pgd25q64c_Lock);
    return 0;
#endif
}

//=====================================================================
//函数名：读芯片ID
//参数：
//返回值：其它:成功；-1：失败
//功能：
//=====================================================================
u32 Gd25q64c_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[3];
    u32 deviceid;

    sndbuf[0]=gd25q64c_JedecID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    if(Gd25q64c_WaitReady(80000*mS) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Read ID.");
        return -1;
    }

    Gd25q64c_CsActive();
//  if(Gd25q64c_TxRx(sndbuf,4,rcvbuf,2,4) == false)
    if(Gd25q64c_TxRx(sndbuf,1,rcvbuf,3,1) == false)
    {
        Gd25q64c_CsInactive();
        return -1;
    }
    Gd25q64c_CsInactive();

    deviceid=(rcvbuf[0]<<16)|(rcvbuf[1]<<8)|(rcvbuf[2]);
    return deviceid;
}

//=====================================================================
//函数名：使能四线模式
//参数：
//返回值：true:成功；false：失败
//功能：
//=====================================================================
bool_t Gd25q64c_Qspi_Enable(void)
{
    u8 sr2;

    if(Gd25q64c_ReadSR(StatusReg2, &sr2) == false)
        return false;
    if((sr2 & 0X02)==0)           //QE位未使能
    {
        sr2 |= 1<<1;              //使能QE位
        if(Gd25q64c_WriteSR(StatusReg2, sr2) == false)
            return false;
    }
    return true;
}

// ============================================================================
// 功能：擦除gd25q64c的页或块
// 参数：unit -- 页或者块号；
//      sz -- 页或者扇区；
// 返回：
// 备注：
// ============================================================================
s32 __gd25q64c_erase(u32 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
        block = unit / gd25q64_des->PagesPerBlock; // 看看这一页在哪个块（手册中的扇区）中
    else if (sz.block)
        block = (u32)unit;

    return Gd25q64c_Erase_Block(block);
}

// ============================================================================
// 功能：QFLASH 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __gd25q64c_req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = *unit / gd25q64_des->PagesPerBlock;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  gd25q64_des->BlockNum;
            break;
        }

        case totalSectors:
        {
            // args = &blocks
            *((u32*)args) =  gd25q64_des->SectorNum;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = gd25q64_des->PagesPerSector;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = gd25q64_des->BytesPerPage;
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

            if(!sz->block)
                return (-1);

            if(-1 == (s32)end)
                end = gd25q64_des->BlockNum;
            else if (start)
                end += start;

            do
            {
                if(__gd25q64c_erase((s64)--end, *sz) == false)
                {
                    res = -1;
                    break;
                }
            }
            while(end!=start);

            break;
        }

        case checkbad: break; // 检查坏块
        default: res = -1; break;
    }

    return (res);
}

// =============================================================================
// 功能：初始化QSPI FLASH模块，校验芯片ID是否正确
// 参数：无
// 返回：成功（0）；失败（-1）；
// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
// =============================================================================
s32 ModuleInstall_Gd25q64c(void)
{
    u32 temp;

    pgd25q64c_Lock = Lock_MutexCreate("gd25q64c Lock");
    if(!pgd25q64c_Lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create gd25q64c flash lock.");
        return false;
    }
    Gd25q64c_GpioInit();
    temp=Gd25q64c_ReadID();

    printf("gd25q64c Read ID Success,ID:%x\r\n",temp);
//  if(temp==gd25q64c_type)
//  {
//      printf("gd25q64c Read ID Success,ID:%x\r\n",gd25q64c_type);
//  }
//  else
//  {
//     printf("gd25q64c Read ID Error,True ID:%x",gd25q64c_type);
//     printf("    Read ID:%x\r\n",temp);
//     return false;
//  }

    if(!gd25q64_des)
    {
        gd25q64_des = malloc(sizeof(struct NorDescr));
        if(!gd25q64_des)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }
        memset(gd25q64_des, 0x0, (sizeof(struct NorDescr)));
        gd25q64_des->PortType = NOR_SPI;
        gd25q64_des->Port = NULL;
        gd25q64_des->BytesPerPage = gd25q64c_PageSize;
        gd25q64_des->SectorNum = gd25q64c_BlockrNum * gd25q64c_SectorsPerBlock;
        gd25q64_des->BlockNum = gd25q64c_BlockrNum;
        gd25q64_des->BlocksPerSector = 0;
        gd25q64_des->PagesPerSector = gd25q64c_PagesPerSector;
        gd25q64_des->SectorsPerBlock = gd25q64c_SectorsPerBlock;
        gd25q64_des->PagesPerBlock = gd25q64c_PagesPerSector * gd25q64c_SectorsPerBlock;
        gd25q64_des->ReservedBlks = 0;
    }

    if(CFG_GD25_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __gd25q64c_req(format, 0 , -1, &sz))
        {
            warning_printf("gd25"," Format failure.");
        }
    }

    gd25_umedia = malloc(sizeof(struct umedia)+gd25q64_des->BytesPerPage);
    if(!gd25_umedia)
        return false;

    gd25_umedia->mreq = __gd25q64c_req;
    gd25_umedia->type = nor;
    gd25_umedia->ubuf = (u8*)gd25_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)gd25_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(gd25_umedia);
        return false;
    }

    gd25q64c_Init_Flag = TRUE;
    return TRUE;
}

// =============================================================================
// 功能：判断gd25是否安装
// 参数：  无
// 返回：已成功安装（true）；未成功安装（false）；
// 备注：
// =============================================================================
bool_t Gd25_is_install(void)
{
    return gd25q64c_Init_Flag;
}

// ============================================================================
// 功能：初始化SPI FLASH模块，用做文件系统
// 参数：   fs -- 该媒体所要安装文件系统mount点名字；
//      mediadrv -- 媒体驱动
//      dwStart -- 起始块；
//      dwEnd -- 结束块数（擦除时不包括该块，只擦到该块的上一块）；
// 返回：成功初始化（true）；初始化失败（false）；
// 备注：分区逻辑用于文件系统，直接访问逻辑不用设置分区。
// ============================================================================
bool_t __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;

    if(mediadrv == NULL)
        return false;

    targetobj = OBJ_MatchPath(fs, &notfind);    //根据mount点名字找mount点的obj
    if(notfind)
    {
        error_printf("gd25"," not found need to install file system.");
        return false;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj); //获取obj的私有数据
    super->MediaInfo = gd25_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = gd25q64_des->BlockNum;
        BlockNum = dwEnd - dwStart;     //获取文件系统一共用了多少块
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * gd25q64_des->SectorsPerBlock * gd25q64_des->PagesPerSector * gd25q64_des->BytesPerPage ;
    super->MediaStart = dwStart * gd25q64_des->PagesPerBlock; // 起始unit号

    res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);      //获取设备的全路径
    File_BeMedia(FullPath,fs);     //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (true);
}

bool_t showpage(char *param)
{
    s32 temp;
    u8 buf[256];
    temp = atoi(param);
    Gd25q64c_ReadPage(buf, temp);
    for(temp = 0;temp<256;temp+=8)
    {
        printf("0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x 0x%2x\r\n", buf[temp], buf[temp + 1], buf[temp + 2], buf[temp + 3], buf[temp + 4], buf[temp + 5], buf[temp + 6], buf[temp + 7]);
    }
    return true;
}

u32  data[256],rd[256];
bool_t test25(char *param)
{
    u32 loop,num;
    s32 temp;
    temp = atoi(param);

    for(num = 0; num < 4096; num++)
    {
        for(loop = 0; loop < 256; loop++)
        {
            data[loop] = num*256+loop+temp;
        }
        Gd25q64c_Write(data, num*1024,1024);
        Gd25q64c_Read(rd, num*1024,1024);
        for(loop = 0; loop < 256; loop++)
        {
            if(data[loop] != rd[loop])
            {
                printf("********w25q test error,n = %d\r\n",num);
                break;
            }
        }
        if(num % 100 == 0)
        {
            printf("-------------writed %d pages\r\n",num);
        }
      DJY_EventDelay(1*1000);
    }
    return true;
}

bool_t testread(char *param)
{
    char *page_addr,*next_param;
    u32 page,num,loop,n;

    page_addr = shell_inputs(param,&next_param);

    page = atoi(page_addr);
    num = atoi(next_param);

    Gd25q64c_Read(data, page*1024,1024);
    for(loop = 0; loop < num; loop++)
    {
        Gd25q64c_Read(rd, page*1024,1024);
        for(n = 0; n < 256; n++)
        {
            if(data[n] != rd[n])
            {
                printf("********w25q test error,n = %d\r\n",loop);
                break;
            }
        }
        if(loop % 100 == 0)
        {
            printf("-------------writed %d pages\r\n",loop);
        }
      DJY_EventDelay(1*1000);
    }
    return true;
}
ADD_TO_ROUTINE_SHELL(erase25, Gd25q64c_Erase_Chip,"擦除25系列spi norflash");
ADD_TO_ROUTINE_SHELL(showpage, showpage,"显示一页内容");
ADD_TO_ROUTINE_SHELL(test25, test25,"测试芯片");
ADD_TO_ROUTINE_SHELL(testread, testread,"测试芯片");

