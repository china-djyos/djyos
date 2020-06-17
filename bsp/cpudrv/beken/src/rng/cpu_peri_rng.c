// =============================================================================

// 文件名     ：cpu_peri_rng.c
// 模块描述: GMAC模块底层硬件驱动，为DJYIP的协议栈特定的驱动
// 模块版本: V1.00
// 创建人员: HM
// 创建时间: 02/13.2017
// =============================================================================
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include "wlan_ui_pub.h"
#include "arm_arch.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern void ModuleInstall_random(void);
//    ModuleInstall_random();
//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"cpu onchip random"//CPU的随机数发生器驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:early               //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM == false )
//#warning  " cpu_onchip_random  组件参数未配置，使用默认配置"
//%$#@target = header    //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_CPU_ONCHIP_RANDOM    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,255,
//%$#@num,1000,65535,
//%$#@enum,true,false,
//%$#@string,1,32,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束

//%$#@exclude       ****编译排除文件列表
//%$#@end exclude   ****组件描述结束

//@#$%component end configure
// =============================================================================

#define TRNG_BASE (0X00802480)
#define TRNG_CTRL (TRNG_BASE + 0*4)
#define TRNG_EN (0x01UL << 0)
#define TRNG_DATA ((TRNG_BASE + 1*4))

static void trang_active(u8 enable)
{
    u32 value;
    value = REG_READ(TRNG_CTRL);

    if(enable)
    {
        value |= TRNG_EN;
    }
    else
    {
        value &= ~TRNG_EN;
    }

    REG_WRITE(TRNG_CTRL,value);
}

u32 trng_get_random(void)
{
    trang_active(1);
    return REG_READ(TRNG_DATA);
}


u16 rand_port (void)
{
    u32 xx =  trng_get_random();
    u16 xx_h = xx>>16;
    u16 xx_l = xx&0xFFFF;
//    printf("xx=%d!\r\n", (int)(xx_h^xx_l));
    return (u16)(xx_h^xx_l);
}


void ModuleInstall_random(void)
{
  u32 crc,crc2;
  u8 i=8;
  trang_active(1);
  while(i--)
  {
      crc2 =trng_get_random();
      crc ^= crc2;

      printf("\n\r\n\r random_init NUm ====== 0x%x,0x%x\n\r\n\r",crc,crc2);
  }
  srand(crc);
}
