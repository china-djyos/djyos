
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu_peri.h"
#include "lan8720.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern bool_t LAN8720_ResetInit(void);
//  LAN8720_RESET( );
//  LAN8720_ResetInit( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"ethernet phy lan8720"//RMII接口的10/100M PHY
//parent:"tcpip"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720 == false )
//#warning  " ethernet_phy_lan8720  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_ETHERNET_PHY_LAN8720    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,255,
#define CFG_LAN8720_PHY_ADDRESS     0   //phy的物理地址，硬件一般设计为 0
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
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

//extern uint32_t HAL_GetTick(void);
//bool_t lan8720Init(void)
//{
//    u16   phyreg = 0;
//    u32 delay = 0;
////  uint32_t tickstart = 0;
////  uint32_t tickNow ;
//    /*-------------------- PHY initialization and configuration ----------------*/
//    /* Put the PHY in reset mode */
//    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_RESET)) != 1)
//    {
//        if(sg_NeedMacDriver == true)
//            printf("\r\n LAN8720驱动需要mac驱动");
//        return false;
//    }
//     DJY_EventDelay(CN_RESET_DELAY);
////   tickstart=HAL_GetTick();
//    do
//    {
//        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
//        if(delay > 1000)
//            return false;
//        else
//        {
//            DJY_DelayUs(1000);
//            delay++;
//        }
////      tickNow=HAL_GetTick();
////      if((tickNow - tickstart ) > 1000)
////          return false;
//
//    } while (((phyreg & PHY_LINKED_STATUS) != PHY_LINKED_STATUS));
//    /* Enable Auto-Negotiation */
//    if((GMAC_MdioW(CFG_LAN8720_PHY_ADDRESS, PHY_BCR, PHY_AUTONEGOTIATION)) != 1)
//    {
//      return false;
//    }
//    /* Get tick */
////  tickstart = HAL_GetTick();
//    delay = 0;
//    /* Wait until the auto-negotiation will be completed */
//    do
//    {
//        GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_BSR, &phyreg);
//        if(delay > 1000)
//            return false;
//        else
//        {
//            DJY_DelayUs(1000);
//            delay++;
//        }
////      tickNow=HAL_GetTick();
////      if((tickNow - tickstart ) > 10000)
////          return false;
//    } while (((phyreg & PHY_AUTONEGO_COMPLETE) != PHY_AUTONEGO_COMPLETE));
//
//    /* Read the result of the auto-negotiation */
//    if((GMAC_MdioR(CFG_LAN8720_PHY_ADDRESS, PHY_SR, &phyreg)) != 1)
//    {
//        return false;
//    }
//    return true;
//}

static lan8720_Object_t LAN8720_Obj;

/**
  * @brief  Register IO functions to component object
  * @param  pObj: device object  of LAN8742_Object_t.
  * @param  ioctx: holds device IO functions.
  * @retval LAN8742_STATUS_OK  if OK
  *         LAN8742_STATUS_ERROR if missing mandatory function
  */
int32_t  LAN8720_RegisterBusIO(lan8720_IOCtx_t *ioctx)
{
  if(!ioctx->ReadReg || !ioctx->WriteReg )
  {
    return -1;
  }

  LAN8720_Obj.IO.Init = ioctx->Init;
  LAN8720_Obj.IO.DeInit = ioctx->DeInit;
  LAN8720_Obj.IO.ReadReg = ioctx->ReadReg;
  LAN8720_Obj.IO.WriteReg = ioctx->WriteReg;

  return 0;
}

//读取PHY寄存器值
//reg要读取的寄存器地址
//返回值:0 读取成功，-1 读取失败
int32_t LAN8720_ReadPHY(u16 reg,u32 *regval)
{
    if(LAN8720_Obj.IO.ReadReg==NULL)
        return -1;
    if(LAN8720_Obj.IO.ReadReg(LAN8720_ADDR,reg,regval)!=0)
        return -1;
    return 0;
}

//向LAN8720指定寄存器写入值
//reg:要写入的寄存器
//value:要写入的值
//返回值:0 写入正常，-1 写入失败
int32_t LAN8720_WritePHY(u16 reg,u16 value)
{
    u32 temp=value;
    if(LAN8720_Obj.IO.WriteReg==NULL)
        return -1;
    if(LAN8720_Obj.IO.WriteReg(LAN8720_ADDR,reg,temp)!=0)
        return -1;
    return 0;
}

//打开LAN8720 Power Down模式
void LAN8720_EnablePowerDownMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_POWER_DOWN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//关闭LAN8720 Power Down模式
void LAN8720_DisablePowerDownMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval&=~LAN8720_BCR_POWER_DOWN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//开启LAN8720的自协商功能
void LAN8720_StartAutoNego(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_AUTONEGO_EN;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//使能回测模式
void LAN8720_EnableLoopbackMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval|=LAN8720_BCR_LOOPBACK;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//关闭LAN8720的回测模式
void LAN8720_DisableLoopbackMode(void)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    readval&=~LAN8720_BCR_LOOPBACK;
    LAN8720_WritePHY(LAN8720_BCR,readval);
}

//使能中断，中断源可选:LAN8720_ENERGYON_IT
//                     LAN8720_AUTONEGO_COMPLETE_IT
//                     LAN8720_REMOTE_FAULT_IT
//                     LAN8720_LINK_DOWN_IT
//                     LAN8720_AUTONEGO_LP_ACK_IT
//                     LAN8720_PARALLEL_DETECTION_FAULT_IT
//                     LAN8720_AUTONEGO_PAGE_RECEIVED_IT
void LAN8720_EnableIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_IMR,&readval);
    readval|=interrupt;
    LAN8720_WritePHY(LAN8720_IMR,readval);
}

//关闭中断，中断源可选:LAN8720_ENERGYON_IT
//                     LAN8720_AUTONEGO_COMPLETE_IT
//                     LAN8720_REMOTE_FAULT_IT
//                     LAN8720_LINK_DOWN_IT
//                     LAN8720_AUTONEGO_LP_ACK_IT
//                     LAN8720_PARALLEL_DETECTION_FAULT_IT
//                     LAN8720_AUTONEGO_PAGE_RECEIVED_IT
void LAN8720_DisableIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_IMR,&readval);
    readval&=~interrupt;
    LAN8720_WritePHY(LAN8720_IMR,readval);
}

//清除中断标志位，读寄存器ISFR就可清除中断标志位
void LAN8720_ClearIT(u32 interrupt)
{
    u32 readval=0;
    LAN8720_ReadPHY(LAN8720_ISFR,&readval);
}

//获取中断标志位
//返回值，1 中断标志位置位，
//        0 中断标志位清零
u8 LAN8720_GetITStatus(u32 interrupt)
{
    u32 readval=0;
    u32 status=0;
    LAN8720_ReadPHY(LAN8720_ISFR,&readval);
    if(readval&interrupt) status=1;
    else status=0;
    return status;
}

//获取LAN8720的连接状态
//返回值：LAN8720_STATUS_LINK_DOWN              连接断开
//        LAN8720_STATUS_AUTONEGO_NOTDONE       自动协商完成
//        LAN8720_STATUS_100MBITS_FULLDUPLEX    100M全双工
//        LAN8720_STATUS_100MBITS_HALFDUPLEX    100M半双工
//        LAN8720_STATUS_10MBITS_FULLDUPLEX     10M全双工
//        LAN8720_STATUS_10MBITS_HALFDUPLEX     10M半双工
u32 LAN8720_GetLinkState(void)
{
    u32 readval=0;

    //读取两遍，确保读取正确！！！
    LAN8720_ReadPHY(LAN8720_BSR,&readval);
    LAN8720_ReadPHY(LAN8720_BSR,&readval);

    //获取连接状态(硬件，网线的连接，不是TCP、UDP等软件连接！)
    if((readval&LAN8720_BSR_LINK_STATUS)==0)
        return LAN8720_STATUS_LINK_DOWN;

    //获取自动协商状态
    LAN8720_ReadPHY(LAN8720_BCR,&readval);
    if((readval&LAN8720_BCR_AUTONEGO_EN)!=LAN8720_BCR_AUTONEGO_EN)  //未使能自动协商
    {
        if(((readval&LAN8720_BCR_SPEED_SELECT)==LAN8720_BCR_SPEED_SELECT)&&
                ((readval&LAN8720_BCR_DUPLEX_MODE)==LAN8720_BCR_DUPLEX_MODE))
            return LAN8720_STATUS_100MBITS_FULLDUPLEX;
        else if((readval&LAN8720_BCR_SPEED_SELECT)==LAN8720_BCR_SPEED_SELECT)
            return LAN8720_STATUS_100MBITS_HALFDUPLEX;
        else if((readval&LAN8720_BCR_DUPLEX_MODE)==LAN8720_BCR_DUPLEX_MODE)
            return LAN8720_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720_STATUS_10MBITS_HALFDUPLEX;
    }
    else                                                            //使能了自动协商
    {
        LAN8720_ReadPHY(LAN8720_PHYSCSR,&readval);
        if((readval&LAN8720_PHYSCSR_AUTONEGO_DONE)==0)
            return LAN8720_STATUS_AUTONEGO_NOTDONE;
        if((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_100BTX_FD)
            return LAN8720_STATUS_100MBITS_FULLDUPLEX;
        else if ((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_100BTX_HD)
            return LAN8720_STATUS_100MBITS_HALFDUPLEX;
        else if ((readval&LAN8720_PHYSCSR_HCDSPEEDMASK)==LAN8720_PHYSCSR_10BT_FD)
            return LAN8720_STATUS_10MBITS_FULLDUPLEX;
        else
            return LAN8720_STATUS_10MBITS_HALFDUPLEX;
    }
}


//设置LAN8720的连接状态
//参数linkstate：LAN8720_STATUS_100MBITS_FULLDUPLEX 100M全双工
//               LAN8720_STATUS_100MBITS_HALFDUPLEX 100M半双工
//               LAN8720_STATUS_10MBITS_FULLDUPLEX  10M全双工
//               LAN8720_STATUS_10MBITS_HALFDUPLEX  10M半双工
void LAN8720_SetLinkState(u32 linkstate)
{

    u32 bcrvalue=0;
    LAN8720_ReadPHY(LAN8720_BCR,&bcrvalue);
    //关闭连接配置，比如自动协商，速度和双工
    bcrvalue&=~(LAN8720_BCR_AUTONEGO_EN|LAN8720_BCR_SPEED_SELECT|LAN8720_BCR_DUPLEX_MODE);
    if(linkstate==LAN8720_STATUS_100MBITS_FULLDUPLEX)       //100M全双工
        bcrvalue|=(LAN8720_BCR_SPEED_SELECT|LAN8720_BCR_DUPLEX_MODE);
    else if(linkstate==LAN8720_STATUS_100MBITS_HALFDUPLEX)  //100M半双工
        bcrvalue|=LAN8720_BCR_SPEED_SELECT;
    else if(linkstate==LAN8720_STATUS_10MBITS_FULLDUPLEX)   //10M全双工
        bcrvalue|=LAN8720_BCR_DUPLEX_MODE;

    LAN8720_WritePHY(LAN8720_BCR,bcrvalue);
}

//初始化LAN8720
int32_t LAN8720_ResetInit(void)
{
    u32 timeout=0;
    u32 regval=0;
    int32_t status=LAN8720_STATUS_OK;
    if(LAN8720_WritePHY(LAN8720_BCR,LAN8720_BCR_SOFT_RESET)>=0) //LAN8720软件复位
    {
        //等待软件复位完成
        if(LAN8720_ReadPHY(LAN8720_BCR,&regval)>=0)
        {
            while(regval&LAN8720_BCR_SOFT_RESET)
            {
                if(LAN8720_ReadPHY(LAN8720_BCR,&regval)<0)
                {
                    status=LAN8720_STATUS_READ_ERROR;
                    break;
                }
                DJY_DelayUs(10*1000);
                timeout++;
                if(timeout>=LAN8720_TIMEOUT) break; //超时跳出,5S
            }

        }
        else
        {
            status=LAN8720_STATUS_READ_ERROR;
        }
    }
    else
    {
        status=LAN8720_STATUS_WRITE_ERROR;
    }
    LAN8720_StartAutoNego();                //开启自动协商功能

    if(status==LAN8720_STATUS_OK)           //如果前面运行正常就延时1s
        DJY_DelayUs(200*1000);                     //等待1s

    //等待网络连接成功
    timeout=0;
    while(LAN8720_GetLinkState()<=LAN8720_STATUS_LINK_DOWN)
    {
        DJY_DelayUs(10*1000);
        timeout++;
        if(timeout>=LAN8720_TIMEOUT)
        {
            status=LAN8720_STATUS_LINK_DOWN;
            break; //超时跳出,5S
        }
    }
    return status;
}





