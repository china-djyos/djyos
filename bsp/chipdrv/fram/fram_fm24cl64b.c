// =============================================================================
// Copyright (C) 2012-2020 都江堰操作系统研发团队  All Rights Reserved
// 文件名     ：cpu_peri_iic.c
// 模块描述: FRAM模块的器件驱动程序，由器件驱动程序员完成
// 模块版本: V1.00
// 创建人员: hm
// 创建时间: 20/06.2014
// =============================================================================
#include "stdint.h"
#include "cpu_peri.h"
#include "stdio.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "iicbus.h"
#include "endian.h"
#include "fram_fm24cl64b.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//   extern ptu32_t ModuleInit_Fram(void);
//   ModuleInit_Fram();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"fram fm24c164b"//iic接口的铁电
//parent:"iicbus"    //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                   //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                  //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early                 //初始化时机，可选值：early，medium，later, pre-main。
                                  //表示初始化时间，分别是早期、中期、后期
//dependence:"iicbus","lock","cpu onchip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                  //选中该组件时，被依赖组件将强制选中，
                                  //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"           //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                  //选中该组件时，被依赖组件不会被强制选中，
                                  //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                  //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_FRAM_FM24C164B == false )
//#warning  " fram_fm24c164b  组件参数未配置，使用默认配置"
//%$#@target = header             //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_FRAM_FM24C164B    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_FRAM_BUS_NAME              "IIC0"       //"IIC总线",配置铁电使用的IIC总线名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#define CFG_FRAM_ADDR                 (0x80)             //"地址",铁电的地址（硬件地址）
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define CFG_FRAM_MAX_ADDR             (0x2000) //铁电大小，字节单位
struct MutexLCB *  ptSemID_Fram;       // IIC操作互斥标志
static struct IIC_Device *s_ptFRAM_Dev=NULL;
#define CFG_FM24CL64B_TIMEOUT         (2000*mS)
#define CFG_FM24CL64B_CLK_FRE         (100*1000)      //总线速度，单位Hz
// =============================================================================
// 功能：读铁电指定地址开始Dstlen长度的数据
// 参数：fram_no,铁电号，为0或1
//       Fram_DstAddr，目标存储器预读取数据的地址
//       pDstBuf，存放接收到的数据缓存地址
//       DstLen， 预读取数据的长度
// 返回：ret, 实际读到的字节数
// =============================================================================
u32 FRAM_Read_Data(u32 Fram_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + DstLen> CFG_FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Read(s_ptFRAM_Dev, Fram_DstAddr, pDstBuf, DstLen, CFG_FM24CL64B_TIMEOUT);
    return ret;
}
// =============================================================================
// 功能：写铁电指定地址开始Dstlen长度的数据
// 参数：fram_no,铁电号，为0或1
//      Fram_DstAddr，目标存储器预写数据地址
//      pSrcBuf，发送数据缓存地址
//      SrcLen， 发送数据长度
// 返回：ret, 实际写入的字节数
// =============================================================================
u32 FRAM_Write_Data(u32 Fram_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0;
    if(Fram_DstAddr + SrcLen > CFG_FRAM_MAX_ADDR)
        return 0;
    ret=IIC_Write(s_ptFRAM_Dev, Fram_DstAddr, pSrcBuf, SrcLen,0, CFG_FM24CL64B_TIMEOUT);
    return ret;
}
// =============================================================================
// 功能：写字到铁电。读铁电指定地址双字长度的数据
// 参数：fram_no,铁电号，为0或1
//       Fram_DstAddr，目标存储器预读地址
// 返回：u32data,返回读到的数据
// =============================================================================
u32 FRAM_Read_DWord(u32 Fram_DstAddr)
{
    u32 u32data,buf[4];
    u32  rLen = 4;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //大小端转换
    u32data = pick_little_32bit((u8 *)&buf,0);
    return u32data;
}
// =============================================================================
// 功能：写字到铁电，写双字数据到指定地址的FRAM
// 参数：fram_no,铁电号，为0或1
//       Fram_DstAddr，目标存储器预写地址
// 返回：ret, 操作错误码，无错误为ERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_DWord(u32 Fram_DstAddr,u32 u32data)
{
    u32  rLen = 4;
    u8 buf[4];
    u32 ret = 0;

    fill_little_32bit((u8 *)&buf,0,u32data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}

// =============================================================================
// 功能：读双字，读铁电指定地址双字长度的数据
// 参数：fram_no,铁电号，为0或1
//       Fram_DstAddr，目标存储器预读地址
// 返回：u16data,返回读到的数据
// =============================================================================
u16 FRAM_Read_Word(u32 Fram_DstAddr)
{
    u16 u16data,buf[2];
    u8  rLen = 2;
    u32 ret = 0;

    ret = FRAM_Read_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    if(ret != rLen)
        return 0;
    //从小端存储中读取到的数据，转化为大端模式
    u16data = pick_little_16bit((u8 *)&buf,0);
    return u16data;
}

// =============================================================================
// 功能：写字到铁电，写字数据到指定地址的FRAM
// 参数：fram_no,铁电号，为0或1
//       Fram_DstAddr，目标存储器预写地址
//       u16data,写入的数据
// 返回：ret, 操作错误码，无错误为ERR_N0_ERROR
// =============================================================================
u32 FRAM_Write_Word( u32 Fram_DstAddr,u16 u16data)
{
    u8  rLen = 2,buf[2];
    u32 ret = 0;

    //将大端改成小端 ，以小端方式存储在FRAM
    fill_little_16bit((u8 *)&buf,0,u16data);
    ret = FRAM_Write_Data(Fram_DstAddr,(u8 *)&buf,rLen);
    return ret;
}
// =============================================================================
// 功能：带保护写字到铁电，按单个WORD读取铁电数据
// 参数：读取WORD地址
// 返回：读取后WORD数据
// =============================================================================
u16 FRAM_Read_Word_r(u16 wAddress )
{
    u16    wData = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))  // 获取写权限
    {
        printf("FRAM1_Read_Word_r 获取写权限失败\n");
        return  false;
    }
    wData = FRAM_Read_Word(wAddress );   // 写铁电
    Lock_MutexPost( ptSemID_Fram );
    return    wData;
}

// =============================================================================
// 功能：信号保护读数据，按多个u8读取铁电数据
// 参数：读取WORD地址, 保存缓冲区 读取长度
// 返回： 读取数据长度
// =============================================================================
u32 FRAM_Read_Data_r( u16 wAddress, u8 *pbyBuf, u32 dwLen )
{
    register u32      dwStatus = 0;

    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // 获取写权限
    {
        printf("FRAM1_Read_Data 获取写权限失败\n");
        return  false;
    }
    dwStatus = FRAM_Read_Data(wAddress, pbyBuf, dwLen );// 写铁电
    Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// 功能：信号保护读字；按单个WORD写铁电数据
// 参数：读取WORD地址, 写入数据
// 返回：  写入数据长度
// =============================================================================
s32 FRAM_Write_Word_r(u16 wAddress, u16 wData )
{
    s32    Status = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))       // 获取写权限
    {
        printf("FRAM1_Write_Word_r 获取写权限失败\n");
        return  false;
    }
    Status = FRAM_Write_Word(wAddress, wData);   // 写铁电
    Lock_MutexPost( ptSemID_Fram);
    return    Status;
}

// =============================================================================
// 功能：信号保护写数据，按多个u8写铁电数据
// 参数：读取WORD地址, 写入数据
// 返回：  写入数据长度
// =============================================================================
u32 FRAM_Write_Data_r(u16 wAddress, u8 *pbyBuf, u32 dwLen)
{
    register u32      dwStatus = 0;
    if(-1 == Lock_MutexPend( ptSemID_Fram, CN_TIMEOUT_FOREVER))   // 获取写权限
    {
        printf("FRAM1_Write_Data_r 获取写权限失败\n");
        return  false;
    }
    dwStatus = FRAM_Write_Data(wAddress, pbyBuf, dwLen );// 写铁电
    Lock_MutexPost( ptSemID_Fram );

    return    dwStatus;
}

// =============================================================================
// 功能：初始化fram芯片，调用IIC驱动初始化函数
// 参数: para,铁电号，0或1
// 返回: true= 成功，false=失败
// =============================================================================
ptu32_t ModuleInit_Fram(void)
{
    ptSemID_Fram = Lock_MutexCreate("Fram0 Lock");

    s_ptFRAM_Dev = IIC_DevAdd(CFG_FRAM_BUS_NAME,"FRAM_FM24CL64B",CFG_FRAM_ADDR,1,17);
    if(s_ptFRAM_Dev)
    {
        IIC_BusCtrl(s_ptFRAM_Dev,CN_IIC_SET_CLK,CFG_FM24CL64B_CLK_FRE,0);
        IIC_BusCtrl(s_ptFRAM_Dev,CN_IIC_SET_POLL,0,0);
        return true;
    }
    else
    {
        Lock_MutexDelete(ptSemID_Fram);
        return false;
    }
}
