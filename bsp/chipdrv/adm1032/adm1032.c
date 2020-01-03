/*******************************************************************************/
/*                                                                             */
/*   本温度传感器驱动改编至VDSP ezboard post例程中的adm1032驱动                */
/*                                                                             */
/*    FILE:     adm1032.c                                                      */
/*                                                                             */
/*    PURPOSE:  This file tests the ADM1032 Temperature Monitor interface      */
/*               on the EZ-Board.                                              */
/*                                                                             */
/*******************************************************************************/

/* includes */
#include <cdef21469.h>
#include <def21469.h>

#include "stdint.h"
#include "cpu_peri.h"
#include "adm1032.h"
#include "iic.h"

#include <sru.h>
#include <sysreg.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    module_init_adm1032(0);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"temperature transmit adm1032"//iic接口温度传感器
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","iicbus","cpu onchip iic"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TEMPERATURE_TRANSMIT_ADM1032 == false )
//#warning  " temperature_transmit_adm1032  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TEMPERATURE_TRANSMIT_ADM1032    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_ADM1032_BUS         "iic1"//"name",配置adm1032的总线
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


struct MutexLCB *  ptSemID_adm1032;       // IIC操作互斥标志

static djy_handle_t pg_adm1032_lhdl;
static struct IIC_Param pg_adm1032_param;

static u32 timeout = 10000;


// ------读铁电--------------------------------------------------------
// 功能：读铁电指定地址开始Dstlen长度的数据
// 参数：Adm1032_DstAddr，目标存储器预读取数据的地址
//         pDstBuf，存放接收到的数据缓存地址
//         DstLen， 预读取数据的长度
// 返回：ret, 实际读到的字节数
// -------------------------------------------------------------------
u32 ADM1032_Read_Data(u32 Adm1032_DstAddr,u8 *pDstBuf,u32 DstLen)
{
    u32 ret = 0;

    pg_adm1032_param.memory_addr = Adm1032_DstAddr;
    pg_adm1032_param.data_len    = 0;

    //发送存储器地址
    Driver_DevWriteLeft(pg_adm1032_lhdl,(ptu32_t)pDstBuf,
                (ptu32_t)&pg_adm1032_param,timeout);

    //读数据
    pg_adm1032_param.data_len    = DstLen;
    ret = Driver_DevReadLeft(pg_adm1032_lhdl,(ptu32_t)pDstBuf,
                    (ptu32_t)&pg_adm1032_param,timeout);
    return ret;
}
// -------写铁电------------------------------------------------------
// 功能：写铁电指定地址开始Dstlen长度的数据
// 参数：Adm1032_DstAddr，目标存储器预写数据地址
//         pSrcBuf，发送数据缓存地址
//         SrcLen， 发送数据长度
// 返回：ret, 实际写入的字节数
// -------------------------------------------------------------------
u32 ADM1032_Write_Data(u32 Adm1032_DstAddr,u8 *pSrcBuf,u32 SrcLen)
{
    u32 ret = 0;


    pg_adm1032_param.memory_addr = Adm1032_DstAddr;
    pg_adm1032_param.data_len    = SrcLen;

    ret = Driver_DevWriteLeft(pg_adm1032_lhdl,(ptu32_t)pSrcBuf,
                        (ptu32_t)&pg_adm1032_param,timeout);

    return ret;
}


//----初始化ADM1032芯片----------------------------------------------------
//功能：初始化ADM1032芯片
//参数: 无
//返回: true= 成功，false=失败
//--------------------------------------------------------------------------
bool_t module_init_adm1032(ptu32_t para)
{
    ptSemID_adm1032 = Lock_MutexCreate(NULL);// adm1032操作互斥标志

    pg_adm1032_param.device_addr  = TEMP_SENSOR_ADDRESS;  //ADM1032的物理地址
    pg_adm1032_param.memory_addr  = 0;
    pg_adm1032_param.mem_addr_len = 1;             //ADM1032的地址空间为两字节
    pg_adm1032_param.data_len     = 0;

    IIC0_Init();
    pg_adm1032_lhdl = Driver_DevOpenLeft("iic1",0);
    if(pg_adm1032_lhdl != NULL)
    {
        //配置传输速度为100KHz
       Driver_DevCtrlLeft(pg_adm1032_lhdl,cn_iic_set_clk,I2C_CLOCK_RATE_100K,0);
       return true;
    }
    else
        return false;
}
