//----------------------------------------------------
//Copyright (C), 2013-2020,  luoshitian.
//版权所有 (C), 2013-2020,   罗侍田.
//所属模块: 88E1512 phy芯片函数
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 88E1512 phy芯片驱动函数
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2013-11-14
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
//#include "cpu_peri_tsec.h"
#include "phy_88e1512.h"
#include "stdio.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"phy 88e1512"//网络phy驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_PHY_88E1512 == false )
//#warning  " phy_88e1512  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_PHY_88E1512    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define SET_PAGE_REG      (0x16)

#define PAGE_NUM(x)       (x)
#define REGISTER(x)       (x)

#define CON_PHY_RGMII_MODE(x)   ((x & 0xFFF8) | 0x2)

#define MARVEL_RESET      (0x8000)

 //----读取是否链接-----------------------------------------------------------------
//功能：读取Marvell PHY的状态寄存器，获取链接状态
//参数：int addr，phy芯片地址
//返回：返回link = 0,没有链接
//      返回link = 1,已经链接
//-----------------------------------------------------------------------------
s32 genphy_update_link_88E1512(u32 addr)
{
    u32 reg,linked;
    s32 timeout = 500;
    s32 devad = -1;

    phy_write(addr, devad, SET_PAGE_REG, PAGE_NUM(1));
    reg = phy_read(addr, devad, REGISTER(17));
    while (timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(17));
        if(reg & 0x0400)        //是否连接上
            break;              //连接上了
        DJY_DelayUs(1000);
    }
    if(reg & 0x0400)
        linked = 1;
    else
        linked = 0;

    return linked;
}

//----获取phy网络参数-------------------------------------------------------
//功能：获取网络参数，主要包括传输速度和是否为全双工
//参数：int addr，phy芯片地址
//返回：结构体phy_parameter,包含传输的速度和全/半双工
//-----------------------------------------------------------------------------
struct phy_parameter genphy_parse_link__88E1512(u32 addr)
{
    u32 reg,temp;
    s32 devad = -1;
    struct phy_parameter priv;

    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);
    if(reg != PAGE_NUM(1))
    {
        tsec_phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1));
    }

    reg = tsec_phy_read(addr, devad, REGISTER(17));
    temp = reg >> 14;
    switch(temp)
    {
        case 2:
            priv.speed = 1000;
            break;
        case 1:
            priv.speed = 100;
            break;
        case 0:
            priv.speed = 10;
            break;
        default:
            break;
    }
    if(reg & 0x2000)
        priv.duplex = 0x01;      //full_duplex
    else
        priv.duplex = 0x00;      //half_duplex

    return priv;
}

//----phy初始化函数-----------------------------------------------------------------
//功能：配置Marvell PHY寄存器，针对芯片88E1512
//参数：int addr，phy芯片地址，int supported，phy芯片支持的数据传输模式设置
//返回：返回0
//-----------------------------------------------------------------------------
s32 phy_config_88e1512(s32 addr,s32 supported)
{
    s32 reg;
    s32 timeout = 500;
    s32 devad = -1;

    //将PHY页配置为18
    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);
    if(reg != PAGE_NUM(18))
    {
        if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(18)) < 0)
        {
            printf("PHY reset failed\r\n");
            return -1;
        }
    }

    //配置为RGMII模式，18页，第20个寄存器
    reg = tsec_phy_read(addr, devad, REGISTER(20));
    reg = CON_PHY_RGMII_MODE(reg);//RGMII 1000M
    tsec_phy_write(addr,devad,REGISTER(20),reg);

    //update the PHY's MODE
    reg = tsec_phy_read(addr, devad, REGISTER(20));
    reg |= MARVEL_RESET;
    tsec_phy_write(addr,devad,REGISTER(20),reg);

    //等待PHY's MODE 完成更新
    while ((reg & MARVEL_RESET) && timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(20));
        DJY_DelayUs(1000);
    }
    if (reg & MARVEL_RESET)
    {
        printf("PHY reset timed out\n");
        return -1;
    }

    //读取连接状态，page1,register1
    if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1)) < 0)
    {
        printf("PHY reset failed\r\n");
        return -1;
    }
    reg = tsec_phy_read(addr, devad, REGISTER(0));
    reg &= 0xEFFF;//禁止自适应
    tsec_phy_write(addr,devad,REGISTER(0),reg);
    reg = tsec_phy_read(addr, devad, REGISTER(26));
    reg |= 0x4000;//link noisy
    tsec_phy_write(addr, devad, REGISTER(26),reg);

    return 0;
}

//----phy芯片的复位---------------------------------------------------------
//功能：配置Marvell PHY寄存器，针对芯片88E1512，88e1512芯片的配置相对不同，
//      不仅需要硬件上配置，软件也需要配置，配置时需先设置操作的页
//参数：int addr，phy芯片地址，
//返回：返回0
//--------------------------------------------------------------------------
void phy_reset_88e1512(s32 addr)
{
    s32 reg;
    s32 timeout = 500;
    s32 devad = -1;

    reg = tsec_phy_read(addr, devad, SET_PAGE_REG);

    if(reg != PAGE_NUM(1))
    {
        //SET PAGE1
        if (phy_write(addr, devad, SET_PAGE_REG,PAGE_NUM(1)) < 0)
        {
            printf("PHY reset failed\r\n");
            return;
        }
    }

    reg = tsec_phy_read(addr, devad, REGISTER(0));
    reg |= MARVEL_RESET;
    if (phy_write(addr, devad, REGISTER(0),reg) < 0)
    {
        printf("PHY reset failed\r\n");
        return;
    }

    /*
     * Poll the control register for the reset bit to go to 0 (it is
     * auto-clearing).  This should happen within 0.5 seconds per the
     * IEEE spec.
     */
    while ((reg & MARVEL_RESET) && timeout--)
    {
        reg = tsec_phy_read(addr, devad, REGISTER(0));

        if (reg < 0)
        {
            printf("PHY status read failed\r\n");
            return;
        }
        DJY_DelayUs(1000);
    }

    if (reg & MARVEL_RESET)
    {
        printf("PHY reset timed out\n");
        return;
    }

    return;
}


