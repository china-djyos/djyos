// =============================================================================
// Copyright (C) 2012-2020 长园继保自动化有限公司 All Rights Reserved
// 文件名     ：gd25q16c.c
// 模块描述: Qflash，使用QSPI通信
// 模块版本: V1.00
// 创建人员: wangxy
// 创建时间: 2018.08.30
// =============================================================================


#include "gd25q16c.h"
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
#include <device/unit_media.h>
#include <device.h>
#include <djyfs/filesystems.h>
#include <include/spi_pub.h>
#include "dbug.h"
#include "cpu_peri_spi.h"

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_gd25q16c(void);
//    ModuleInstall_gd25q16c();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"nor flash gd25q16c"//spi接口的norflash
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
#warning   gd25q16c组件参数未配置，使用默认值
//%$#@num,-1,1024,
//%$#@enum,true,false,
#define CFG_GD25_PART_FORMAT               false      //分区选项,是否需要擦除该芯片。
//%$#@select
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//*****************************************************************//
//QFLASH说明：目前配置下，本芯片：flash容量2M，一个扇区16页，一页256字节数据
//在读写操作前需根据芯片手册中每个指令的时序图使用QSPI_Send_CMD进行配置，例如：
//QPI,四线读数据,地址为ReadAddr,1线传输指令_1线传输24位地址_8空周期_4线传输数据,NumByteToRead个数据
//随后使用QSPI_Receive或QSPI_Transmit
//*****************************************************************//


//struct SemaphoreLCB pgd25q16c_Lock;   //芯片互斥访问保护
struct MutexLCB *pgd25q16c_Lock;           //芯片互斥访问保护
bool_t gd25q16c_Init_Flag=false;
u16 gd25q16c_TYPE          = 0xc814;
u8 gd25q16c_QPI_MODE       = 0;            //QSPI模式标志:0,SPI模式;1,QPI模式.

                                            //flash容量16M
u32 gd25q16c_SectorSize    = 16 * 256;    //一个扇区4k字节数据
u32 gd25q16c_PageSize      = 256;          //一页256字节数据

u32 gd25q16c_OP_TIMEOUT= 1800000; // 30分钟
static char *flash_name = "gd25q16c";
struct NorDescr *qflashdescription = NULL;
struct umedia *gd25_umedia;
extern struct Object *s_ptDeviceRoot;

s32 gd25q16c_Write_Enable(void);
bool_t gd25q16c_WaitReady(u32 timeout);
//=====================================================================
//函数名： 片选拉低
//参数：
//返回值：
//功能：片选拉低，选中器件
//=====================================================================
static void gd25q16c_CsActive(void)
{
    u32 param;
    param = 0x2;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
}
//=====================================================================
//函数名：片选拉高
//参数：
//返回值：
//功能：片选拉高，结束操作
//=====================================================================
static void gd25q16c_CsInActive(void)
{
    u32 param;
    param = 0x3;
    spi_ctrl(CMD_SPI_SET_NSSID, (void *)&param);
}

//=====================================================================
//函数名：SPI发送接收函数
//参数：发送的数据地址、数据长度、接受的数据地址、数据长度、偏移量（发送几个数之后再开始发一个收一个，前面是空读）
//返回值：0：成功 ； -1失败
//功能：SPI发送接收
//=====================================================================
static s32 gd25q16c_TxRx(u8* sdata,u32 slen,u8* rdata, u32 rlen,u32 RecvOff)
{
    if(SPI_TxRx(sdata, slen, rdata, rlen, RecvOff) == false)
        return -1;
    return 0;
}

//=====================================================================
//函数名：读寄存器指令
//参数：想读的寄存器枚举量
//返回值：-1:失败 ； 其它 ：寄存器的值
//功能：
//=====================================================================
s32 gd25q16c_ReadSR(u8 regno)
{
    u8 command[1];
    u8 byte;
    switch(regno)
    {
        case StatusReg1:
            command[0] = gd25q16c_ReadStatusReg1;           //读状态寄存器1指令
            break;
//        case ConfigurationReg1:
//           command[0] = gd25q16c_ReadConfigurationReg1;    //读状态寄存器1指令
//            break;
        case StatusReg2:
            command[0] = gd25q16c_ReadStatusReg2;           //读状态寄存器2指令
            break;
        default:
            command[0] = gd25q16c_ReadStatusReg1;
            break;
    }

    gd25q16c_CsActive();
    if(gd25q16c_TxRx(command,1,&byte,1,1) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }
    gd25q16c_CsInActive();

    return byte;
}


//=====================================================================
//函数名：写寄存器指令
//参数：保存三个寄存器值得数组指针
//返回值：0：成功 ； -1失败
//功能：按顺序写三个寄存器
//=====================================================================
s32 gd25q16c_Write_SR1_SR2(u8 *sr)
{
    u8 sndbuf[3];

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Write Reg.");
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //写使能
    {
        return -1;
    }

    sndbuf[0] = gd25q16c_WriteRegister;
    sndbuf[1] = *sr;
    sndbuf[2] = *(sr + 1);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,3,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }

    gd25q16c_CsInActive();

    return 0;
}

//=====================================================================
//函数名：写使能等待
//参数：
//返回值：0：成功 ； -1失败
//功能：
//=====================================================================
s16 gd25q16c_WriteEnableWait(void)
{
    u8 sndbuf[1];
    u8 rcvbuf[1];
    u32 wait = 20000;

    sndbuf[0] = gd25q16c_ReadStatusReg1;

    do
    {
        gd25q16c_CsActive();

        gd25q16c_TxRx(sndbuf,1,rcvbuf,1,1);

        gd25q16c_CsInActive();
//        DJY_DelayUs(100);
        wait --;
        if(wait == 0)
        {
            return -1;
        }
    }while((rcvbuf[0] & 0x02) != 0x02);

    return 0;
}

//=====================================================================
//函数名：写失能等待
//参数：
//返回值：0：成功 ； -1失败
//功能：
//=====================================================================
s16 gd25q16c_WriteDisableWait(void)
{
    u8 sndbuf[1];
    u8 rcvbuf[1];
    u32 wait = 20000;

    sndbuf[0] = gd25q16c_ReadStatusReg1;

    do
    {
        gd25q16c_CsActive();

        gd25q16c_TxRx(sndbuf,1,rcvbuf,1,1);

        gd25q16c_CsInActive();
//        DJY_DelayUs(100);
        wait --;
        if(wait == 0)
        {
            return -1;
        }
    }while((rcvbuf[0] & 0x02) == 0x02);

    return 0;
}

//=====================================================================
//函数名：写使能
//参数：
//返回值：0：成功 ； -1失败
//功能：想往QFLASH里写东西前都要先使能
//=====================================================================
s32 gd25q16c_Write_Enable(void)
{
    u8 sndbuf[1];

    sndbuf[0] = gd25q16c_WriteEnable;

    gd25q16c_CsActive();

    gd25q16c_TxRx(sndbuf,1,NULL,0,0);

    gd25q16c_CsInActive();

    if(gd25q16c_WriteEnableWait() == -1)
    {
        printf("\r\n gd25q16c Write Enable fail.\r\n");
        return -1;
    }

    return 0;

}


//=====================================================================
//函数名：写失能
//参数：
//返回值：0：成功 ； -1失败
//功能：
//=====================================================================
s32 gd25q16c_Write_Disable(void)
{
    u8 sndbuf[1];

    sndbuf[0] = gd25q16c_WriteDisable;

    gd25q16c_CsActive();

    gd25q16c_TxRx(sndbuf,1,NULL,0,0);

    gd25q16c_CsInActive();

    if(gd25q16c_WriteDisableWait() == -1)
    {
        printf("\r\n gd25q16c Write Disable fail.\r\n");
        return -1;
    }

    return 0;

}

//=====================================================================
//函数名：等待QFLASH的busy位清空
//参数：
//返回值：true ：成功；false ：失败
//功能：
//=====================================================================
bool_t gd25q16c_WaitReady(u32 timeout)
{
    u8 sr1=1;
    u32 timeoutcnt=0;

    sr1 = gd25q16c_ReadSR(StatusReg1);

    while( (sr1 & 0x01) == 0x01)   // 等待BUSY位清空
    {
        sr1 = gd25q16c_ReadSR(StatusReg1);

//        DJY_EventDelay(100);
        timeoutcnt++;
        if(timeoutcnt == timeout)
        {
            return false;
        }
    }
    return true;
}

//=====================================================================
//函数名：扇区擦除
//参数：块号
//返回值：0:成功；-1：失败
//功能：
//=====================================================================
s32 gd25q16c_Erase_Sector(u32 SectorNum)
{
    u32 Dst_Addr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    if(gd25q16c_WaitReady(200000) == false)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        printf("\r\n QFLASH : debug : device is busy before Sector Erase.");
        return -1;
    }


    if(gd25q16c_Write_Enable() != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    Dst_Addr = SectorNum * gd25q16c_SectorSize;

    sndbuf[0] = gd25q16c_SectorErase;
    sndbuf[3] = Dst_Addr & 0xff;
    sndbuf[2] = (Dst_Addr >> 8) & 0xff;
    sndbuf[1] = (Dst_Addr >> 16) & 0xff;

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();


    if(gd25q16c_WaitReady(200000) == false)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        printf("\r\n QFLASH : debug : device is busy after Sector Erase.");
        return -1;
    }
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}

//=====================================================================
//函数名：芯片擦除
//参数：
//返回值：0：成功;-1：失败
//功能：擦出全部内容
//=====================================================================
s32 gd25q16c_Erase_Chip(void)
{
    u8 status = 0;
    u32 time = 0;
    u8 sndbuf[1];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Chip Erase.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    sndbuf[0] = gd25q16c_ChipErase;

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,1,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    printf("\r\n gd25q16c Erase, Please Wait.");
    do
    {
        status = gd25q16c_ReadSR(StatusReg1);
        DJY_EventDelay(100);
        if(time++ > 1000)
        {
            printf(".");
            time = 0;
        }
    }while((status & 0x01) == 0x01);

    printf("\r\ngd25q16c Erase Chip Succeed\r\n");
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}



//=====================================================================
//函数名：写一页
//参数：发送缓冲区，页号
//返回值：0 ：成功；-1：失败
//功能：
//=====================================================================
s32 gd25q16c_WritePage(u8* pBuffer,u32 PageNum)
{
    u8 sndbuf[4 + 256];
    u32 WriteAddr;
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    WriteAddr = PageNum * gd25q16c_PageSize;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Write.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //写使能
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    sndbuf[0] = gd25q16c_PageProgram;
    sndbuf[3] = WriteAddr & 0xff;
    sndbuf[2] = (WriteAddr >> 8) & 0xff;
    sndbuf[1] = (WriteAddr >> 16) & 0xff;
    memcpy(sndbuf + 4, pBuffer, gd25q16c_PageSize);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf, 4 + 256 ,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

//    DJY_EventDelay(4000);// 延时切出. 4ms。不能去除

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;

}

//=====================================================================
//函数名：从某一地址开始写
//参数：发送缓冲区，写地址，数据长度
//返回值： 0:成功；-1：失败
//功能：地址就是从QFLASH的第N个字节开始
//=====================================================================
s32 gd25q16c_WriteMemory(u8* pBuffer,u32 WriteAddr,u32 NumByteToWrite)
{
    u8 command[4];
    u8 *SendBuf;

    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Memory Write.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    if(gd25q16c_Write_Enable() != 0)                  //写使能
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    SendBuf = (u8*) malloc(4 + NumByteToWrite);

    command[0] = gd25q16c_PageProgram;
    command[3] = WriteAddr & 0xff;
    command[2] = (WriteAddr >> 8) & 0xff;
    command[1] = (WriteAddr >> 16) & 0xff;

    memcpy(SendBuf, command, 4);
    memcpy(SendBuf + 4, pBuffer, NumByteToWrite);

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(SendBuf, 4 + NumByteToWrite ,NULL,0,0) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    gd25q16c_CsInActive();

//    DJY_EventDelay(4000);// 延时切出. 4ms。不能去除

    Lock_MutexPost(pgd25q16c_Lock);
    free(SendBuf);
    return 0;

}

//=====================================================================
//函数名：读一页
//参数：接收缓冲区，页号
//返回值： 0:成功；-1：失败
//功能：
//=====================================================================
s32 gd25q16c_ReadPage(u8* pBuffer,u32 PageNum)
{
    u32 ReadAddr;
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    ReadAddr = PageNum * gd25q16c_PageSize;

    sndbuf[0] = gd25q16c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Page Read.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,pBuffer,256,4) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
}

//=====================================================================
//函数名：从某一地址开始读
//参数：接收缓冲区，开始读取地址，读取长度
//返回值： 0:成功；-1：失败
//功能：
//=====================================================================
s32 gd25q16c_ReadMemory(u8* pBuffer,u32 ReadAddr,u32 NumByteToRead)
{
    u8 sndbuf[4];
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);

    sndbuf[0] = gd25q16c_ReadData;
    sndbuf[3] = ReadAddr & 0xff;
    sndbuf[2] = (ReadAddr >> 8) & 0xff;
    sndbuf[1] = (ReadAddr >> 16) & 0xff;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n FLASH : debug : device is busy before Memory Read.");
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,pBuffer,NumByteToRead,4) == -1)
    {
        gd25q16c_CsInActive();
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }

    gd25q16c_CsInActive();

    Lock_MutexPost(pgd25q16c_Lock);
    return 0;

#if 0
    Lock_MutexPend(pgd25q16c_Lock,CN_TIMEOUT_FOREVER);
    if(QSPI_Send_CMD(gd25q16c_FastReadData,ReadAddr,8,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_24_BITS,QSPI_DATA_1_LINE) != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    //QPI,快速读数据,地址为ReadAddr,1线传输数据_24位地址_1线传输地址_1线传输指令,8空周期,NumByteToRead个数据
    if(QSPI_Receive(pBuffer,NumByteToRead) != 0)
    {
        Lock_MutexPost(pgd25q16c_Lock);
        return -1;
    }
    Lock_MutexPost(pgd25q16c_Lock);
    return 0;
#endif
}

//=====================================================================
//函数名：读芯片ID
//参数：
//返回值：0:成功；-1：失败
//功能：
//=====================================================================
u16 gd25q16c_ReadID(void)
{
    u8 sndbuf[4];
    u8 rcvbuf[2];
    u16 deviceid;

    sndbuf[0]=gd25q16c_ManufactDeviceID;
    sndbuf[1]=0x00;
    sndbuf[2]=0x00;
    sndbuf[3]=0x00;

    if(gd25q16c_WaitReady(200000) == false)
    {
        printf("\r\n QFLASH : debug : device is busy before Read ID.");
        return -1;
    }

    gd25q16c_CsActive();

    if(gd25q16c_TxRx(sndbuf,4,rcvbuf,2,4) == -1)
    {
        gd25q16c_CsInActive();
        return -1;
    }


    gd25q16c_CsInActive();

    deviceid=(rcvbuf[0]<<8)|rcvbuf[1];
    return deviceid;
}

//=====================================================================
//函数名：使能四线模式
//参数：
//返回值：
//功能：
//=====================================================================
void gd25q16c_Qspi_Enable(void)
{
    u8 sr1,sr2;
    u8 reg_w_buf[2];

    sr2=gd25q16c_ReadSR(StatusReg2);

    if((sr2 & 0X02)==0)           //QE位未使能
    {
        gd25q16c_Write_Enable();   //写使能
        if(gd25q16c_WriteEnableWait() == -1)
        {
            printf("\r\n gd25q16c Write Enable fail.\r\n");
        }
        sr1=gd25q16c_ReadSR(StatusReg1);       //先读出配置寄存器1的原始值
        reg_w_buf[0]=sr1 ;
        sr2 |= 1<<1;              //使能QE位
        reg_w_buf[1]=sr2 ;

        gd25q16c_Write_SR1_SR2(reg_w_buf); //写状态寄存器
    }
    return;
}

// ============================================================================
// 功能：读gd25q16c
// 参数：
// 返回：0 -- 成功； -1 -- 失败
// 备注：
// ============================================================================
s32 __gd25q16c_read(u32 unit, void *data, struct uopt opt)
{
    return gd25q16c_ReadPage((u8*)data,(u32)unit);
}

// ============================================================================
// 功能：写gd25q16c
// 参数：
// 返回：
// 备注：
// ============================================================================
s32 __gd25q16c_write(u32 unit, void *data, struct uopt opt)
{
    return gd25q16c_WritePage((u8*)data,(u32)unit);
}

// ============================================================================
// 功能：擦除gd25q16c的页或扇区
// 参数：unit -- 页或者扇区；
//      sz -- 页或者扇区；
// 返回：
// 备注：
// ============================================================================
s32 __gd25q16c_erase(u32 unit, struct uesz sz)
{
    u32 Sector;

    if(sz.unit)
        Sector = unit / qflashdescription->PagesPerSector; // 看看这一页在哪个块（实际芯片的扇区）中
    else if (sz.block)
        Sector = (u32)unit;

    return gd25q16c_Erase_Sector(Sector);
}

// ============================================================================
// 功能：QFLASH 命令
// 参数：ucmd -- 命令；
//      其他 -- 命令参数；
// 返回：
// 备注：
// ============================================================================
s32 __gd25q16c_req(enum ucmd cmd, ptu32_t args, ...)
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
            *block = *unit / qflashdescription->PagesPerBlock;
            break;
        }

        case totalblocks://这枚举量和注释里说的意思不一样啊
        {
            // args = &blocks
            *((u32*)args) =  qflashdescription->SectorNum;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = qflashdescription->PagesPerSector;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = qflashdescription->BytesPerPage;
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
                end = qflashdescription->BlockNum;
            else if (start)
                end += start;

            do
            {
                if(__gd25q16c_erase((s64)--end, *sz))
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
s32 ModuleInstall_gd25q16c(void)
{
    u16 temp;

    pgd25q16c_Lock = Lock_MutexCreate("gd25q16c Lock");
    if(!pgd25q16c_Lock)
    {
        printf("\r\nMODULE INSTALL : error : cannot create gd25q16c flash lock.");
        return false;
    }

    temp=gd25q16c_ReadID();        //读取FLASH ID.目前只能用中断方式，在initPrj.c文件中初始化中断还没起来

    if(temp==gd25q16c_TYPE)
   {
        printf("gd25q16c Read ID Success,ID:%x\r\n",gd25q16c_TYPE);
    }
    else
    {
       printf("gd25q16c Read ID Error,True ID:%x",gd25q16c_TYPE);
       printf("    Read ID:%x\r\n",temp);
       return false;
    }

    if(!qflashdescription)
    {
        qflashdescription = malloc(sizeof(struct NorDescr));
        if(!qflashdescription)
        {
            printf("\r\n: erro : device : memory out.\r\n");
            return false;
        }
        memset(qflashdescription, 0x0, (sizeof(struct NorDescr)));
        qflashdescription->PortType = NOR_SPI;
        qflashdescription->Port = NULL;
        qflashdescription->BytesPerPage = gd25q16c_PageSize;
        qflashdescription->SectorNum = 512;
        qflashdescription->BlockNum = 32;
        qflashdescription->BlocksPerSector = 0;
        qflashdescription->PagesPerSector = 16;
        qflashdescription->SectorsPerBlock = 16;
        qflashdescription->PagesPerBlock = 256;
        qflashdescription->ReservedBlks = 0;
    }

    if(CFG_GD25_PART_FORMAT)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        if(-1 == __gd25q16c_req(format, 0 , -1, &sz))
        {
            warning_printf("at45"," Format failure.");
        }
    }

    gd25_umedia = malloc(sizeof(struct umedia)+qflashdescription->BytesPerPage);
    if(!gd25_umedia)
        return false;

    gd25_umedia->mreq = __gd25q16c_req;
    gd25_umedia->type = nor;
    gd25_umedia->ubuf = (u8*)gd25_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)flash_name, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)gd25_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", flash_name);
        free(gd25_umedia);
        return false;
    }

    gd25q16c_Init_Flag = TRUE;
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
    return gd25q16c_Init_Flag;
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
s32 __GD25_FsInstallInit(const char *fs, s32 dwStart, s32 dwEnd, void *mediadrv)
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
    super->MediaInfo = gd25_umedia;
    super->MediaDrv = mediadrv;

    if((s32)dwEnd == -1)
    {
        dwEnd = qflashdescription->SectorNum;
        BlockNum = dwEnd - dwStart;     //获取文件系统一共用了多少块
    }
    else
    {
        BlockNum = dwEnd - dwStart;
    }
    super->AreaSize = BlockNum * (qflashdescription->BytesPerPage * qflashdescription->PagesPerSector);
    super->MediaStart = dwStart * qflashdescription->PagesPerSector; // 起始unit号

    res = strlen(flash_name) + strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,flash_name);      //获取设备的全路径
    File_BeMedia(FullPath,fs);     //往该设备挂载文件系统
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, dwStart, dwEnd);
    return (0);
}

