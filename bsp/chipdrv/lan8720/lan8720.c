
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  bool_t lan8720Init(void);
//  lan8720Init();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"lan8720"      //RMII接口的10/100M PHY
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"cpu_peri_eth"     //该组件的依赖组件名（可以是none，表示无依赖组件），
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
//#ifndef CFG_VLAN_VALUE0         //****检查参数是否已经配置好
//#warning    lan8720组件参数未配置，使用默认值
//%$#@num,0,255,
#define CFG_LAN8720_PHY_ADDRESS     0   //phy的物理地址，硬件一般设计为 0
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

#define CN_RESET_DELAY      255000      //uS
static bool_t sg_NeedMacDriver = false;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
__attribute__((weak)) u8 GMAC_MdioR(u8 dev, u8 reg, u16 *value)
{
    sg_NeedMacDriver = true;
    return 0;
}

__attribute__((weak)) u8 GMAC_MdioW(u8 dev,u8 reg, u16 value)
{
    sg_NeedMacDriver = true;
    return 0;
}

#pragma GCC diagnostic pop

extern uint32_t HAL_GetTick(void);
bool_t lan8720Init(void)
{
    u16   phyreg = 0;
    u32 delay = 0;
//  uint32_t tickstart = 0;
//  uint32_t tickNow ;
    /*-------------------- PHY initialization and configuration ----------------*/
    /* Put the PHY in reset mode */
    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
    {
        if(sg_NeedMacDriver == true)
            printf("\r\n LAN8720驱动需要mac驱动");
        return false;
    }
     Djy_EventDelay(CN_RESET_DELAY);
//   tickstart=HAL_GetTick();
    do
    {
        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
        if(delay > 1000)
            return false;
        else
        {
            Djy_DelayUs(1000);
            delay++;
        }
//      tickNow=HAL_GetTick();
//      if((tickNow - tickstart ) > 1000)
//          return false;

    } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
    /* Enable Auto-Negotiation */
    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
    {
      return false;
    }
    /* Get tick */
//  tickstart = HAL_GetTick();
    delay = 0;
    /* Wait until the auto-negotiation will be completed */
    do
    {
        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
        if(delay > 1000)
            return false;
        else
        {
            Djy_DelayUs(1000);
            delay++;
        }
//      tickNow=HAL_GetTick();
//      if((tickNow - tickstart ) > 10000)
//          return false;
    } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));

    /* Read the result of the auto-negotiation */
    if((GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
    {
        return false;
    }
    return true;
}

