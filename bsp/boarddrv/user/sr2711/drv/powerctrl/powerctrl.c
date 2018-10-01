//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
//所属模块: SR2711板件模块电源控制
//作者:  zhb.
//版本：V1.0.0
//文件描述: 远程升级
//其他说明:
//修订历史:
//1. 日期: 2017-08-12
//   作者:  zhb.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "cpu_peri.h"
#include "board-config.h"
#include "powerctrl.h"
#include "shell.h"
#include <stm32f7xx_hal_mmc.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"powerctrl"    //板件特性配置
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:必选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"cpu_peri_eth","cpu_peri_emmc"    //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure

void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag);
extern void ADC_LowPowerCfg(bool_t Flag);


//**************************************************************************
//static void Sh_PeriStatus(void);
//static void Sh_SDRAM_PowerOn(void);
//static void Sh_SDRAM_PowerOff(void);
//static void Sh_EMMC_PowerOn(void);
//static void Sh_EMMC_PowerOff(void);
//static void Sh_EMAC_PowerOn(void);
//static void Sh_EMAC_PowerOff(void);
//static void Sh_IMAGE_PowerOn(void);
//static void Sh_IMAGE_PowerOff(void);
//static void Sh_CAMERA_PowerOn(void);
//static void Sh_CAMERA_PowerOff(void);
//static void Sh_30W_PowerOn(void);
//static void Sh_30W_PowerOff(void);
//static void Sh_10W_PowerOn(void);
//static void Sh_10W_PowerOff(void);
//static void Sh_SENSOR1_PowerOn(void);
//static void Sh_SENSOR1_PowerOff(void);
//static void Sh_SENSOR2_PowerOn(void);
//static void Sh_SENSOR2_PowerOff(void);
//static void Sh_SENSOR3_PowerOn(void);
//static void Sh_SENSOR3_PowerOff(void);
//static void Sh_SENSOR4_PowerOn(void);
//static void Sh_SENSOR4_PowerOff(void);
//static void Sh_SENSOR5_PowerOn(void);
//static void Sh_SENSOR5_PowerOff(void);
//static void Sh_SENSOR6_PowerOn(void);
//static void Sh_SENSOR6_PowerOff(void);
//static void Sh_SENSOR7_PowerOn(void);
//static void Sh_SENSOR7_PowerOff(void);
//static void Sh_SENSOR8_PowerOn(void);
//static void Sh_SENSOR8_PowerOff(void);
//static void Sh_SENSOR9_PowerOn(void);
//static void Sh_SENSOR9_PowerOff(void);
//static void Sh_SENSOR10_PowerOn(void);
//static void Sh_SENSOR10_PowerOff(void);
//static void Sh_SENSOR11_PowerOn(void);
//static void Sh_SENSOR11_PowerOff(void);
//static void Sh_SENSOR12_PowerOn(void);
//static void Sh_SENSOR12_PowerOff(void);
//static void Sh_SENSOR13_PowerOn(void);
//static void Sh_SENSOR13_PowerOff(void);
//static void Sh_RF1_PowerOn(void);
//static void Sh_RF1_PowerOff(void);
//static void Sh_RTC_PowerOn(void);
//static void Sh_RTC_PowerOff(void);
//static void Sh_ENCRYPTION_PowerOn(void);
//static void Sh_ENCRYPTION_PowerOff(void);
//static void Sh_RF2_PowerOn(void);
//static void Sh_RF2_PowerOff(void);
//static void Sh_BAT_PowerOn(void);
//static void Sh_BAT_PowerOff(void);
//static void Sh_5V_CheckPowerOn(void);
//static void Sh_5V_CheckPowerOff(void);

static bool_t gs_PeriStaus[EN_PERI_END];
static char * gs_PeriName[EN_PERI_END];

//低电平有效
const Pin VDD3_3CtrlPin[4]={
    {GPIO_K, PIN3, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO1
    {GPIO_K, PIN4, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO2
    {GPIO_K, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO3
    {GPIO_K, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},   //VDD3.3_KO4
    //SR2711-V1.1.0删除该引脚  20170929
//  {GPIO_H, PIN12, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD3.3_KO5
};


//高电平有效

//SR2711-V1.1.0改为低电平有效  20170929
static const Pin VDD12_EN_Pin[]=
{
    {GPIO_H, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD12_EN1
    {GPIO_H, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD12_EN2
};


//高电平有效
static const Pin VDD12CtrlPin[]={
    {GPIO_J, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO1
    {GPIO_J, PIN6, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO2
    {GPIO_J, PIN7, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO3
    {GPIO_J, PIN8, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO4
    {GPIO_J, PIN9, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO5
    {GPIO_J, PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO6
    {GPIO_J, PIN11, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO7
    {GPIO_J, PIN12, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO8
    {GPIO_J, PIN13, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO9
    {GPIO_J, PIN14, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO10
    {GPIO_J, PIN15, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE}, //VDD_KO11
    {GPIO_K, PIN0, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO12
    {GPIO_K, PIN1, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO13
    {GPIO_K, PIN2, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO14
    {GPIO_H, PIN10, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},  //VDD_KO15
};

//低电平有效  控制电池电压采样
static const Pin BATKoPin[]=
{
        {GPIO_A, PIN5, GPIO_MODE_OUT, GPIO_OTYPE_PP, GPIO_SPEED_VH,GPIO_PUPD_NONE},
};



void Board_PowerCtrlInit(void)
{
     u8 cnt,i;
     PIO_Configure(VDD12_EN_Pin,PIO_LISTSIZE(VDD12_EN_Pin));    //VDD12_EN
     cnt=PIO_LISTSIZE(VDD12_EN_Pin);
     //初始状态下全部使能电平
     for(i=0;i<cnt;i++)
     {
         //SR2711-V1.1.0改为低电平有效  20170929
//       PIO_Set(&VDD12_EN_Pin[i]);
         PIO_Clear(&VDD12_EN_Pin[i]);
     }

     PIO_Configure(VDD12CtrlPin,PIO_LISTSIZE(VDD12CtrlPin));    //VDD12_KO
     cnt=PIO_LISTSIZE(VDD12CtrlPin);
     //初始状态下全部使能电平
     for(i=0;i<cnt;i++)
     {
         PIO_Set(&VDD12CtrlPin[i]);
     }

     PIO_Configure(VDD3_3CtrlPin,PIO_LISTSIZE(VDD3_3CtrlPin));  //VDD3.3
     cnt=PIO_LISTSIZE(VDD3_3CtrlPin);
     for(i=0;i<cnt;i++)
     {
         PIO_Clear(&VDD3_3CtrlPin[i]);
     }
     PIO_Configure(BATKoPin,PIO_LISTSIZE(BATKoPin));    //VDD3.3
     PIO_Set(&BATKoPin[0]);


     for(i=0;i<EN_PERI_END;i++)
     {
        gs_PeriStaus[i]=true;
     }
}






//**************************************************************************
//struct shell_debug const shell_cmd_power_ctrl_table[]=
//{
//  {"perista",(bool_t(*)(char*))Sh_PeriStatus,    "板件所有外设状态",            NULL},
//  {"sdramon",(bool_t(*)(char*))Sh_SDRAM_PowerOn,   "使能SDRAM及电源",           NULL},
//  {"sdramoff",(bool_t(*)(char*))Sh_SDRAM_PowerOff, "禁能SDRAM及电源",           NULL},
//  {"emmcon",(bool_t(*)(char*))Sh_EMMC_PowerOn,     "使能EMMC及电源",            NULL},
//  {"emmcoff",(bool_t(*)(char*))Sh_EMMC_PowerOff,   "禁能EMMC及电源",            NULL},
//  {"emacron",(bool_t(*)(char*))Sh_EMAC_PowerOn,    "使能EMAC及电源",            NULL},
//  {"emacoff",(bool_t(*)(char*))Sh_EMAC_PowerOff,   "禁能EMAC及电源",            NULL},
//  {"imageon",(bool_t(*)(char*))Sh_IMAGE_PowerOn,   "使能模拟图像处理芯片电源及对应外设",NULL},
//  {"imageoff",(bool_t(*)(char*))Sh_IMAGE_PowerOff, "禁能模拟图像处理芯片电源及对应外设",NULL},
//  {"cameon",(bool_t(*)(char*))Sh_CAMERA_PowerOn,   "使能摄像头球机",             NULL},
//  {"cameoff",(bool_t(*)(char*))Sh_CAMERA_PowerOff, "禁能摄像头球机",             NULL},
//  {"30won",(bool_t(*)(char*))Sh_30W_PowerOn,       "开启30W电源",              NULL},
//  {"30woff",(bool_t(*)(char*))Sh_30W_PowerOff,     "断电30W电源",              NULL},
//  {"10won",(bool_t(*)(char*))Sh_10W_PowerOn,       "开启10W电源",              NULL},
//  {"10woff",(bool_t(*)(char*))Sh_10W_PowerOff,     "断电10W电源",              NULL},
//  {"sen1on",(bool_t(*)(char*))Sh_SENSOR1_PowerOn,  "使能传感器组1电源",           NULL},
//  {"sen1off",(bool_t(*)(char*))Sh_SENSOR1_PowerOff,"禁能传感器组1电源",           NULL},
//  {"sen2on",(bool_t(*)(char*))Sh_SENSOR2_PowerOn,  "使能传感器组2电源",           NULL},
//  {"sen2off",(bool_t(*)(char*))Sh_SENSOR2_PowerOff,"禁能传感器组2电源",           NULL},
//  {"sen3on",(bool_t(*)(char*))Sh_SENSOR3_PowerOn,  "使能传感器组3电源",           NULL},
//  {"sen3off",(bool_t(*)(char*))Sh_SENSOR3_PowerOff,"禁能传感器组3电源",           NULL},
//  {"sen4on",(bool_t(*)(char*))Sh_SENSOR4_PowerOn,  "使能传感器组4电源",           NULL},
//  {"sen4off",(bool_t(*)(char*))Sh_SENSOR4_PowerOff,"禁能传感器组4电源",           NULL},
//  {"sen5on",(bool_t(*)(char*))Sh_SENSOR5_PowerOn,  "使能传感器组5电源",           NULL},
//  {"sen5off",(bool_t(*)(char*))Sh_SENSOR5_PowerOff,"禁能传感器组5电源",           NULL},
//  {"sen6on",(bool_t(*)(char*))Sh_SENSOR6_PowerOn,  "使能传感器组6电源",           NULL},
//  {"sen6off",(bool_t(*)(char*))Sh_SENSOR6_PowerOff,"禁能传感器组6电源",           NULL},
//  {"sen7on",(bool_t(*)(char*))Sh_SENSOR7_PowerOn,  "使能传感器组7电源",           NULL},
//  {"sen7off",(bool_t(*)(char*))Sh_SENSOR7_PowerOff,"禁能传感器组7电源",           NULL},
//  {"sen8on",(bool_t(*)(char*))Sh_SENSOR8_PowerOn,  "使能传感器组8电源",           NULL},
//  {"sen8off",(bool_t(*)(char*))Sh_SENSOR8_PowerOff,"禁能传感器组8电源",           NULL},
//  {"sen9on",(bool_t(*)(char*))Sh_SENSOR9_PowerOn,  "使能传感器组9电源",           NULL},
//  {"sen9off",(bool_t(*)(char*))Sh_SENSOR9_PowerOff,"禁能传感器组9电源",           NULL},
//  {"sen10on",(bool_t(*)(char*))Sh_SENSOR10_PowerOn,  "使能传感器组10电源",        NULL},
//  {"sen10off",(bool_t(*)(char*))Sh_SENSOR10_PowerOff,"禁能传感器组10电源",        NULL},
//  {"sen11on",(bool_t(*)(char*))Sh_SENSOR11_PowerOn,  "使能传感器组11电源",        NULL},
//  {"sen11off",(bool_t(*)(char*))Sh_SENSOR11_PowerOff,"禁能传感器组11电源",        NULL},
//  {"sen12on",(bool_t(*)(char*))Sh_SENSOR12_PowerOn,  "使能传感器组12电源",        NULL},
//  {"sen12off",(bool_t(*)(char*))Sh_SENSOR12_PowerOff,"禁能传感器组12电源",        NULL},
//  {"sen13on",(bool_t(*)(char*))Sh_SENSOR13_PowerOn,  "使能传感器组13电源",        NULL},
//  {"sen13off",(bool_t(*)(char*))Sh_SENSOR13_PowerOff,"禁能传感器组13电源",        NULL},
//  {"rf1on",(bool_t(*)(char*))Sh_RF1_PowerOn,         "使能RF模块电源及对应外设",    NULL},
//  {"rf1off",(bool_t(*)(char*))Sh_RF1_PowerOff,       "禁能RF模块电源及对应外设",    NULL},
//  {"rtcon",(bool_t(*)(char*))Sh_RTC_PowerOn,         "使能RTC芯片及对应外设",      NULL},
//  {"rtcoff",(bool_t(*)(char*))Sh_RTC_PowerOff,       "禁能RTC芯片及对应外设",      NULL},
//  {"ecton",(bool_t(*)(char*))Sh_ENCRYPTION_PowerOn,  "使能加密芯片及对应外设",      NULL},
//  {"ectoff",(bool_t(*)(char*))Sh_ENCRYPTION_PowerOff,"禁能加密芯片及对应外设",      NULL},
//  {"rf2on",(bool_t(*)(char*))Sh_RF2_PowerOn,         "使能无线射频模块及对应外设",   NULL},
//  {"rf2off",(bool_t(*)(char*))Sh_RF2_PowerOff,       "禁能无线射频模块及对应外设",   NULL},
//  {"baton",(bool_t(*)(char*))Sh_BAT_PowerOn,     "使能电池采样及对应外设",          NULL},
//  {"batoff",(bool_t(*)(char*))Sh_BAT_PowerOff,    "禁能电池采样及对应外设",         NULL},
//  {"5vcheckon",(bool_t(*)(char*))Sh_5V_CheckPowerOn,  "使能5V自检采样及对应外设",   NULL},
//  {"5vcheckoff",(bool_t(*)(char*))Sh_5V_CheckPowerOff,"禁能5V自检采样及对应外设",   NULL},
//
//};
////**************************************************************************
//#define CN_POWER_CTRL_SHELL_NUM  sizeof(shell_cmd_power_ctrl_table)/sizeof(struct shell_debug)
//static struct ShellCmdRsc tg_power_ctrl_shell_cmd_rsc[CN_POWER_CTRL_SHELL_NUM];



void Peri_NameInit(void)
{
    gs_PeriName[0]="SDRAM";
    gs_PeriName[1]="EMMC";
    gs_PeriName[2]="EMAC";
    gs_PeriName[3]="IMAGE";
    gs_PeriName[4]="CAMERA";
    gs_PeriName[5]="30W";
    gs_PeriName[6]="10W";
    gs_PeriName[7]="SENSOR1";
    gs_PeriName[8]="SENSOR2";
    gs_PeriName[9]="SENSOR3";
    gs_PeriName[10]="SENSOR4";
    gs_PeriName[11]="SENSOR5";
    gs_PeriName[12]="SENSOR6";
    gs_PeriName[13]="SENSOR7";
    gs_PeriName[14]="SENSOR8";
    gs_PeriName[15]="SENSOR9";
    gs_PeriName[16]="SENSOR10";
    gs_PeriName[17]="SENSOR11";
    gs_PeriName[18]="SENSOR12";
    gs_PeriName[19]="SENSOR13";
    gs_PeriName[20]="RF1";
    gs_PeriName[21]="RTC";
    gs_PeriName[22]="ENCRYPTION";
    gs_PeriName[23]="RF2";
    gs_PeriName[25]="BATTERY";
    gs_PeriName[26]="5V_Check";
    gs_PeriName[27]="SAMPLE_RESERVED";
    gs_PeriName[28]="USB_EXPORT";
    gs_PeriName[29]="4G_USB";
    gs_PeriName[30]="4G_UART";
}
/*******************************************************************************
功能:CAN控制器操作shell模块
参数:无.
返回值:1。
*********************************************************************************/
//ptu32_t Power_Ctrl_Shell_Module_Install(void)
//{
//  Sh_InstallCmd(shell_cmd_power_ctrl_table,tg_power_ctrl_shell_cmd_rsc,CN_POWER_CTRL_SHELL_NUM);
//  return 1;
//}


//**************************************************************************
//static void Sh_PeriStatus(void)
ADD_TO_SHELL_HELP(perista,"板件所有外设状态");
ADD_TO_IN_SHELL  bool_t perista(void)
{
    u8 i;
    printf("CPU MCLK(MHz):%d.\r\n",CN_CFG_MCLK/Mhz);
    for(i=0;i<EN_PERI_END;i++)
   {
      if(gs_PeriStaus[i]==true)
      {
        printf("%s Power on.\r\n",gs_PeriName[i]);
      }
      else
      {
        printf("%s Power off.\r\n",gs_PeriName[i]);
      }
   }
   printf("The end........................\r\n");
   return true;
}




//static void Sh_SDRAM_PowerOn(void)
ADD_TO_SHELL_HELP(sdramon,"使能SDRAM及电源");
ADD_TO_IN_SHELL  bool_t sdramon(void)
{
    Peri_LowPowerCfg(EN_PERI_SDRAM,true);
    printf("SDRAM Power on.\r\n");
    return true;

}

//static void Sh_SDRAM_PowerOff(void)
ADD_TO_SHELL_HELP(sdramoff,"禁能SDRAM及电源");
ADD_TO_IN_SHELL  bool_t sdramoff(void)
{
    Peri_LowPowerCfg(EN_PERI_SDRAM,false);
    printf("SDRAM Power off.\r\n");
    return true;
}



//static void Sh_EMMC_PowerOn(void)
ADD_TO_SHELL_HELP(emmcon,"使能EMMC及电源");
ADD_TO_IN_SHELL  bool_t emmcon(void)
{
    bool_t ret;
    Peri_LowPowerCfg(EN_PERI_EMMC,true);
    ret=MMC_HardInit();
    if(!ret)
    {
        printf("EMMC Power on failed.\r\n");
        return true;
    }
    printf("EMMC Power on.\r\n");
    return true;
}

//static void Sh_EMMC_PowerOff(void)
ADD_TO_SHELL_HELP(emmcoff,"禁能EMMC及电源");
ADD_TO_IN_SHELL  bool_t emmcoff(void)
{
    Peri_LowPowerCfg(EN_PERI_EMMC,false);
    printf("EMMC Power off.\r\n");
    return true;
}





//static void Sh_EMAC_PowerOn(void)
ADD_TO_SHELL_HELP(emacron,"使能EMAC及电源");
ADD_TO_IN_SHELL  bool_t emacron(void)
{
    Peri_LowPowerCfg(EN_PERI_EMAC,true);
    MacReset(0);
    printf("EMAC Power on.\r\n");
    return true;
}

//static void Sh_EMAC_PowerOff(void)
ADD_TO_SHELL_HELP(emacoff,"禁能EMAC及电源");
ADD_TO_IN_SHELL  bool_t emacoff(void)
{
    Peri_LowPowerCfg(EN_PERI_EMAC,false);
    printf("EMAC Power off.\r\n");
    return true;
}



//static void Sh_IMAGE_PowerOn(void)
ADD_TO_SHELL_HELP(imageon,"使能模拟图像处理芯片电源及对应外设");
ADD_TO_IN_SHELL  bool_t imageon(void)
{
    Peri_LowPowerCfg(EN_PERI_IMAGE_PROCESS,true);
    printf("IMAGE Power on.\r\n");
    return true;
}

//static void Sh_IMAGE_PowerOff(void)
ADD_TO_SHELL_HELP(imageoff,"禁能模拟图像处理芯片电源及对应外设");
ADD_TO_IN_SHELL bool_t imageoff(void)
{
    Peri_LowPowerCfg(EN_PERI_IMAGE_PROCESS,false);
    printf("IMAGE Power off.\r\n");
    return true;
}

//static void Sh_CAMERA_PowerOn(void)
ADD_TO_SHELL_HELP(cameon,"使能摄像头球机");
ADD_TO_IN_SHELL  bool_t cameon(void)
{
    Peri_LowPowerCfg(EN_PERI_CAMERA_HEADER,true);
    printf("CAMERA Power on.\r\n");
    return true;
}

//static void Sh_CAMERA_PowerOff(void)
ADD_TO_SHELL_HELP(cameoff,"禁能摄像头球机");
ADD_TO_IN_SHELL bool_t cameoff(void)
{
    Peri_LowPowerCfg(EN_PERI_CAMERA_HEADER,false);
    printf("CAMERA Power off.\r\n");
    return true;
}


//static void Sh_30W_PowerOn(void)
ADD_TO_SHELL_HELP(on30w,"开启30W电源");
ADD_TO_IN_SHELL bool_t on30w(void)
{
    Peri_LowPowerCfg(EN_PERI_30W_POWER,true);
    extern void UART4_GpioInit(void);
    UART4_GpioInit();
    UART_HardInit(CN_UART4);
    printf("30W Power on.\r\n");
    return true;
}

//static void Sh_30W_PowerOff(void)
ADD_TO_SHELL_HELP(off30w,"断电30W电源");
ADD_TO_IN_SHELL  bool_t off30w(void)
{
    Peri_LowPowerCfg(EN_PERI_30W_POWER,false);
    printf("30W Power off.\r\n");
    return true;
}



//static void Sh_10W_PowerOn(void)
ADD_TO_SHELL_HELP(on10w,"开启10W电源");
ADD_TO_IN_SHELL bool_t on10w(void)
{
    Peri_LowPowerCfg(EN_PERI_10W_POWER,true);
    extern void UART3_GpioInit(void);
    extern void UART6_GpioInit(void);
    UART3_GpioInit();
    UART6_GpioInit();
    UART_HardInit(CN_UART6);
    UART_HardInit(CN_UART3);
    printf("10W Power on.\r\n");
    return true;
}

//static void Sh_10W_PowerOff(void)
ADD_TO_SHELL_HELP(off10w,"断电10W电源");
ADD_TO_IN_SHELL bool_t off10w(void)
{
    Peri_LowPowerCfg(EN_PERI_10W_POWER,false);
    printf("10W Power off.\r\n");
    return true;
}



//static void Sh_SENSOR1_PowerOn(void)
ADD_TO_SHELL_HELP(sen1on,"使能传感器组1电源");
ADD_TO_IN_SHELL bool_t sen1on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR1,true);
    printf("Sensor1 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR1_PowerOff(void)
ADD_TO_SHELL_HELP(sen1off,"禁能传感器组1电源");
 ADD_TO_IN_SHELL bool_t sen1off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR1,false);
    printf("Sensor1 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR2_PowerOn(void)
 ADD_TO_SHELL_HELP(sen2on,"使能传感器组2电源");
 ADD_TO_IN_SHELL bool_t sen2on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR2,true);
    printf("Sensor2 Power on.\r\n");
    return true;
}


//static void Sh_SENSOR2_PowerOff(void)
 ADD_TO_SHELL_HELP(sen2off,"禁能传感器组2电源");
 ADD_TO_IN_SHELL bool_t sen2off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR2,false);
    printf("Sensor2 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR3_PowerOn(void)
 ADD_TO_SHELL_HELP(sen3on,"使能传感器组3电源");
 ADD_TO_IN_SHELL bool_t sen3on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR3,true);
    printf("Sensor3 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR3_PowerOff(void)
 ADD_TO_SHELL_HELP(sen3off,"禁能传感器组3电源");
 ADD_TO_IN_SHELL bool_t sen3off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR3,false);
    printf("Sensor3 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR4_PowerOn(void)
 ADD_TO_SHELL_HELP(sen4on,"使能传感器组4电源");
 ADD_TO_IN_SHELL bool_t sen4on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR4,true);
    printf("Sensor4 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR4_PowerOff(void)
 ADD_TO_SHELL_HELP(sen4off,"禁能传感器组4电源");
 ADD_TO_IN_SHELL bool_t sen4off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR4,false);
    printf("Sensor4 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR5_PowerOn(void)
 ADD_TO_SHELL_HELP(sen5on,"使能传感器组5电源");
 ADD_TO_IN_SHELL bool_t sen5on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR5,true);
    printf("Sensor5 Power on.\r\n");
    return true;
}


//static void Sh_SENSOR5_PowerOff(void)
 ADD_TO_SHELL_HELP(sen5off,"禁能传感器组5电源");
 ADD_TO_IN_SHELL  bool_t sen5off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR5,false);
    printf("Sensor5 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR6_PowerOn(void)
 ADD_TO_SHELL_HELP(sen6on,"使能传感器组6电源");

 ADD_TO_IN_SHELL bool_t sen6on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR6,true);
    printf("Sensor6 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR6_PowerOff(void)
 ADD_TO_SHELL_HELP(sen6off,"禁能传感器组6电源");

 ADD_TO_IN_SHELL bool_t sen6off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR6,false);
    printf("Sensor6 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR7_PowerOn(void)
 ADD_TO_SHELL_HELP(sen7on,"使能传感器组7电源");

 ADD_TO_IN_SHELL bool_t sen7on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR7,true);
    printf("Sensor7 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR7_PowerOff(void)
 ADD_TO_SHELL_HELP(sen7off,"禁能传感器组7电源");

 ADD_TO_IN_SHELL bool_t sen7off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR7,false);
    printf("Sensor7 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR8_PowerOn(void)
 ADD_TO_SHELL_HELP(sen8on,"使能传感器组8电源");

 ADD_TO_IN_SHELL bool_t sen8on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR8,true);
    printf("Sensor8 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR8_PowerOff(void)
 ADD_TO_SHELL_HELP(sen8off,"禁能传感器组8电源");

 ADD_TO_IN_SHELL bool_t sen8off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR8,false);
    printf("Sensor8 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR9_PowerOn(void)
 ADD_TO_SHELL_HELP(sen9on,"使能传感器组9电源");

 ADD_TO_IN_SHELL bool_t sen9on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR9,true);
    printf("Sensor9 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR9_PowerOff(void)
 ADD_TO_SHELL_HELP(sen9off,"禁能传感器组9电源");

 ADD_TO_IN_SHELL bool_t sen9off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR9,false);
    printf("Sensor9 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR10_PowerOn(void)
 ADD_TO_SHELL_HELP(sen10on,"使能传感器组10电源");

 ADD_TO_IN_SHELL bool_t sen10on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR10,true);
    printf("Sensor10 Power on.\r\n");
    return true;
}

//static void Sh_SENSOR10_PowerOff(void)

 ADD_TO_SHELL_HELP(sen10off,"禁能传感器组10电源");
 ADD_TO_IN_SHELL  bool_t sen10off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR10,false);
    printf("Sensor10 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR11_PowerOn(void)
 ADD_TO_SHELL_HELP(sen11on,"使能传感器组11电源");

 ADD_TO_IN_SHELL bool_t sen11on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR11,true);
    printf("Sensor11 Power on.\r\n");
    return true;
}


//static void Sh_SENSOR11_PowerOff(void)

 ADD_TO_SHELL_HELP(sen11off,"禁能传感器组11电源");
 ADD_TO_IN_SHELL  bool_t sen11off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR11,false);
    printf("Sensor11 Power off.\r\n");
    return true;
}


//static void Sh_SENSOR12_PowerOn(void)

 ADD_TO_SHELL_HELP(sen12on,"使能传感器组12电源");
 ADD_TO_IN_SHELL  bool_t sen12on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR12,true);
    printf("Sensor12 Power on.\r\n");
    return true;
}
//static void Sh_SENSOR12_PowerOff(void)

 ADD_TO_SHELL_HELP(sen12off,"禁能传感器组12电源");
 ADD_TO_IN_SHELL  bool_t sen12off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR12,false);
    printf("Sensor12 Power off.\r\n");
    return true;
}

//static void Sh_SENSOR13_PowerOn(void)

 ADD_TO_SHELL_HELP(sen13on,"使能传感器组13电源");
 ADD_TO_IN_SHELL  bool_t sen13on(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR13,true);
    printf("Sensor13 Power on.\r\n");
    return true;
}
//static void Sh_SENSOR13_PowerOff(void)

 ADD_TO_SHELL_HELP(sen13off,"禁能传感器组13电源");
 ADD_TO_IN_SHELL  bool_t sen13off(void)
{
    Peri_LowPowerCfg(EN_PERI_SENSOR13,false);
    printf("Sensor13 Power off.\r\n");
    return true;
}

//static void Sh_RF1_PowerOn(void)

 ADD_TO_SHELL_HELP(rf1on,"使能RF模块电源及对应外设");
 ADD_TO_IN_SHELL  bool_t rf1on(void)
{
    Peri_LowPowerCfg(EN_PERI_RF1,true);
    printf("RF1 Power on.\r\n");
    return true;
}
//static void Sh_RF1_PowerOff(void)

 ADD_TO_SHELL_HELP(rf1off,"禁能RF模块电源及对应外设");
 ADD_TO_IN_SHELL  bool_t rf1off(void)
{
    Peri_LowPowerCfg(EN_PERI_RF1,false);
    printf("RF1 Power off.\r\n");
    return true;
}


//static void Sh_RTC_PowerOn(void)

 ADD_TO_SHELL_HELP(rtcon,"使能RTC芯片及对应外设");
 ADD_TO_IN_SHELL  bool_t rtcon(void)
{
    Peri_LowPowerCfg(EN_PERI_RTC,true);
    printf("RTC Power on.\r\n");
    return true;
}
//static void Sh_RTC_PowerOff(void)

 ADD_TO_SHELL_HELP(rtcoff,"禁能RTC芯片及对应外设");
 ADD_TO_IN_SHELL  bool_t rtcoff(void)
{
    Peri_LowPowerCfg(EN_PERI_RTC,false);
    printf("RTC Power off.\r\n");
    return true;
}

//static void Sh_ENCRYPTION_PowerOn(void)

 ADD_TO_SHELL_HELP(ecton,"使能加密芯片及对应外设");
 ADD_TO_IN_SHELL  bool_t ecton(void)
{
    Peri_LowPowerCfg(EN_PERI_ENCRYPTION,true);
    printf("ENCRYPTION Power on.\r\n");
    return true;
}
//static void Sh_ENCRYPTION_PowerOff(void)

 ADD_TO_SHELL_HELP(ectoff,"禁能加密芯片及对应外设");
 ADD_TO_IN_SHELL  bool_t ectoff(void)
{
    Peri_LowPowerCfg(EN_PERI_ENCRYPTION,false);
    printf("ENCRYPTION Power off.\r\n");
    return true;
}

//static void Sh_RF2_PowerOn(void)

 ADD_TO_SHELL_HELP(rf2on,"使能无线射频模块及对应外设");
 ADD_TO_IN_SHELL  bool_t rf2on(void)
{
    Peri_LowPowerCfg(EN_PERI_RF2,true);
    printf("RF2 Power on.\r\n");
    return true;
}
//static void Sh_RF2_PowerOff(void)

 ADD_TO_SHELL_HELP(rf2off,"禁能无线射频模块及对应外设");
 ADD_TO_IN_SHELL  bool_t rf2off(void)
{
    Peri_LowPowerCfg(EN_PERI_RF2,false);
    printf("RF2 Power off.\r\n");
    return true;
}


//static void Sh_BAT_PowerOn(void)

 ADD_TO_SHELL_HELP(baton,"使能电池采样及对应外设");
 ADD_TO_IN_SHELL  bool_t baton(void)
{
    Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,true);
    printf("BAT Power on.\r\n");
    return true;

}
//static void Sh_BAT_PowerOff(void)

 ADD_TO_SHELL_HELP(batoff,"禁能电池采样及对应外设");
 ADD_TO_IN_SHELL  bool_t batoff(void)
{
    Peri_LowPowerCfg(EN_PERI_BATTERY_SAMPLE,false);
    printf("BAT Power off.\r\n");
    return true;
}

//static void Sh_5V_CheckPowerOn(void)

 ADD_TO_SHELL_HELP(checkon5v,"使能5V自检采样及对应外设");
 ADD_TO_IN_SHELL  bool_t checkon5v(void)
{
    Peri_LowPowerCfg(EN_PERI_5V_SAMPLE,true);
    printf("5V_Check Power on.\r\n");
    return true;
}
//static void Sh_5V_CheckPowerOff(void)

 ADD_TO_SHELL_HELP(checkoff5v,"禁能5V自检采样及对应外设");
 ADD_TO_IN_SHELL  bool_t checkoff5v(void)
{
    Peri_LowPowerCfg(EN_PERI_5V_SAMPLE,false);
    printf("5V_Check Power off.\r\n");
    return true;
}





void Module_PeriCtrlInit(void)
{
//  Power_Ctrl_Shell_Module_Install();
    Peri_NameInit();
}

// =============================================================================
// 功能：板件模块各独立电源控制函数
// 参数：PeriIdx,外设编号;
//     EnableFlag,使能标识符,true，使能;false,禁能.
// 返回：无
// =============================================================================
void Peri_LowPowerCfg(enum BoardPeriCode PeriIdx,bool_t Flag)
{
    switch(PeriIdx)
    {
      case EN_PERI_SDRAM:
          if(Flag==OutLowPower)
          {
//            PIO_Clear(&VDD3_3CtrlPin[0]);
              SDRAM_LowPowerCfg(OutLowPower);
              gs_PeriStaus[EN_PERI_SDRAM]=true;
          }
          else
          {
              SDRAM_LowPowerCfg(InLowPower);
//            PIO_Set(&VDD3_3CtrlPin[0]);      //SDRAM电源不能关闭，关闭的话SDRAM上内容不能保持
              gs_PeriStaus[EN_PERI_SDRAM]=false;
          }
          break;
      case EN_PERI_EMMC:
//        if(Flag)
//        {
//            PIO_Clear(&VDD3_3CtrlPin[3]);
//            RCC->APB2ENR |=(1<<11);  //使能SDMMC1时钟
//            EMMC_Wakeup();
//            gs_PeriStaus[EN_PERI_EMMC]=true;
//        }
//        else
//        {
//            RCC->APB2ENR &=~(1<<11);  //禁能SDMMC1时钟
//            EMMC_Sleep();
//            PIO_Set(&VDD3_3CtrlPin[3]);
//            gs_PeriStaus[EN_PERI_EMMC]=false;
//        }
          break;
      case EN_PERI_EMAC:    //ok
          if(Flag==OutLowPower)
          {
              PIO_Clear(&VDD3_3CtrlPin[1]);
              GMAC_LowPowerConfig(OutLowPower);
              gs_PeriStaus[EN_PERI_EMAC]=true;
          }
          else
          {
              GMAC_LowPowerConfig(InLowPower);
              PIO_Set(&VDD3_3CtrlPin[1]);
              gs_PeriStaus[EN_PERI_EMAC]=false;

          }
          break;
          //SR2711-V1.1.0板删除该功能  20170929
      case EN_PERI_IMAGE_PROCESS:
//        if(Flag)
//        {
//            PIO_Clear(&VDD3_3CtrlPin[4]); //VDD3.3_KO5
//            UART_LowPowerConfig(CN_UART7,UART_OUT_LOWPOWER);
//            gs_PeriStaus[EN_PERI_IMAGE_PROCESS]=true;
//        }
//        else
//        {
//            UART_LowPowerConfig(CN_UART7,UART_IN_LOWPOWER);
//            PIO_Set(&VDD3_3CtrlPin[4]); //VDD3.3_KO5
//            gs_PeriStaus[EN_PERI_IMAGE_PROCESS]=false;
//        }
          break;
      case EN_PERI_CAMERA_HEADER:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[13]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART4,OutLowPower);
              gs_PeriStaus[EN_PERI_CAMERA_HEADER]=true;
          }
          else
          {
              UART_LowPowerConfig(CN_UART4,InLowPower);
              PIO_Clear(&VDD12CtrlPin[13]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_CAMERA_HEADER]=false;
          }
          break;
      case EN_PERI_30W_POWER:
          if(Flag==OutLowPower)
          {
              //SR2711-V1.1.0改为低电平有效  20170929
//            PIO_Set(&VDD12_EN_Pin[1]); //VDD12_EN2
              PIO_Clear(&VDD12_EN_Pin[1]); //VDD12_EN2
              gs_PeriStaus[EN_PERI_30W_POWER]=true;
          }
          else
          {
              //SR2711-V1.1.0改为低电平有效  20170929
//            PIO_Clear(&VDD12_EN_Pin[1]); //VDD12_EN2
              PIO_Set(&VDD12_EN_Pin[1]); //VDD12_EN2
              gs_PeriStaus[EN_PERI_30W_POWER]=false;
          }
          break;
      case EN_PERI_10W_POWER:
          if(Flag==OutLowPower)
          {
              //SR2711-V1.1.0改为低电平有效  20170929
//            PIO_Set(&VDD12_EN_Pin[0]); //VDD12_EN1
              PIO_Clear(&VDD12_EN_Pin[0]); //VDD12_EN
              gs_PeriStaus[EN_PERI_10W_POWER]=true;
          }
          else
          {
              //SR2711-V1.1.0改为低电平有效  20170929
//            PIO_Clear(&VDD12_EN_Pin[0]); //VDD12_EN1
              PIO_Set(&VDD12_EN_Pin[0]); //VDD12_EN1
              gs_PeriStaus[EN_PERI_10W_POWER]=false;
          }
          break;
      case EN_PERI_SENSOR1:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[0]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR1]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[0]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR1]=false;
          }
          break;

      case EN_PERI_SENSOR2:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[1]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR2]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[1]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR2]=false;
          }
          break;

      case EN_PERI_SENSOR3:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[2]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR3]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[2]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR3]=false;
          }
          break;

       case EN_PERI_SENSOR4:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[3]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR4]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[3]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR4]=false;
          }
          break;


       case EN_PERI_SENSOR5:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[4]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR5]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[4]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR5]=false;
          }
          break;

        case EN_PERI_SENSOR6:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[5]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR6]=true;
          }
          else
          {
//            RCC->APB2ENR &=~(1<<5);  //禁能UART6时钟
              PIO_Clear(&VDD12CtrlPin[5]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR6]=false;
          }
          break;

        case EN_PERI_SENSOR7:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[6]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR7]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[6]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR7]=false;
          }
          break;

       case EN_PERI_SENSOR8:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[7]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR8]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[7]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR8]=false;
          }
          break;


       case EN_PERI_SENSOR9:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[8]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR9]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[8]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR9]=false;
          }
          break;

        case EN_PERI_SENSOR10:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[9]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR10]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[9]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR10]=false;
          }
          break;

        case EN_PERI_SENSOR11:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[10]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR11]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[10]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR11]=false;
          }
          break;

       case EN_PERI_SENSOR12:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[11]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR12]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[11]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR12]=false;
          }
          break;

       case EN_PERI_SENSOR13:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[12]); //VDD12_KO14
              UART_LowPowerConfig(CN_UART6,OutLowPower);
              gs_PeriStaus[EN_PERI_SENSOR13]=true;
          }
          else
          {
              PIO_Clear(&VDD12CtrlPin[12]); //VDD12_KO14
              gs_PeriStaus[EN_PERI_SENSOR13]=false;
          }
          break;

      case EN_PERI_RF1:
          if(Flag==OutLowPower)
          {
              PIO_Set(&VDD12CtrlPin[14]); //VDD12_KO15
              UART_LowPowerConfig(CN_UART3,OutLowPower);
              gs_PeriStaus[EN_PERI_RF1]=true;
          }
          else
          {
              UART_LowPowerConfig(CN_UART3,InLowPower);
              PIO_Clear(&VDD12CtrlPin[14]); //VDD12_KO15
              gs_PeriStaus[EN_PERI_RF1]=false;
          }
          break;
      case EN_PERI_RTC:
          if(Flag==OutLowPower)
          {
//              IIC_LowPowerConfig(CN_IIC2,OutLowPower);
              gs_PeriStaus[EN_PERI_RTC]=true;
          }
          else
          {
//              IIC_LowPowerConfig(CN_IIC2,InLowPower);
              gs_PeriStaus[EN_PERI_RTC]=false;
          }
          break;

      case EN_PERI_ENCRYPTION:
          if(Flag==OutLowPower)
          {
              RCC->APB2ENR |=(1<<12);   //使能SPI1时钟
              gs_PeriStaus[EN_PERI_ENCRYPTION]=true;
          }
          else
          {
              RCC->APB2ENR &=~(1<<12);     //禁能SPI1时钟
              gs_PeriStaus[EN_PERI_ENCRYPTION]=false;
          }
          break;
      case EN_PERI_BATTERY_SAMPLE:
          if(Flag==OutLowPower)
          {
              PIO_Set(&BATKoPin[0]);    //BAT_KO
              ADC_LowPowerCfg(OutLowPower);
              gs_PeriStaus[EN_PERI_BATTERY_SAMPLE]=true;
          }
          else
          {
              ADC_LowPowerCfg(InLowPower);
              PIO_Clear(&BATKoPin[0]);   //BAT_KO
              gs_PeriStaus[EN_PERI_BATTERY_SAMPLE]=false;
          }
          break;
      case EN_PERI_5V_SAMPLE:
          if(Flag==OutLowPower)
          {
              ADC_LowPowerCfg(OutLowPower);
              gs_PeriStaus[EN_PERI_5V_SAMPLE]=true;
          }
          else
          {
              ADC_LowPowerCfg(InLowPower);
              gs_PeriStaus[EN_PERI_5V_SAMPLE]=false;
          }
          break;

      case EN_PERI_RF2:
          if(Flag==OutLowPower)
          {
              PIO_Clear(&VDD3_3CtrlPin[2]); //VDD3.3_KO5
              UART_LowPowerConfig(CN_UART7,OutLowPower);
              gs_PeriStaus[EN_PERI_RF2]=true;
          }
          else
          {
              UART_LowPowerConfig(CN_UART7,InLowPower);
              PIO_Set(&VDD3_3CtrlPin[2]); //VDD3.3_KO5
              gs_PeriStaus[EN_PERI_RF2]=false;
          }
          break;
      case EN_PERI_DEBUG_UART:
          if(Flag==OutLowPower)
          {
              UART_LowPowerConfig(CN_UART1,OutLowPower);
              gs_PeriStaus[EN_PERI_DEBUG_UART]=true;
          }
          else
          {
              UART_LowPowerConfig(CN_UART1,InLowPower);
              gs_PeriStaus[EN_PERI_DEBUG_UART]=false;
          }
          break;

      default:
          break;
    }
}
