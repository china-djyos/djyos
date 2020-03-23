#include <shell.h>

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern ptu32_t ModuleInstall_LS88e6060(void);
//  ModuleInstall_LS88e6060();
//%$#@end initcode  ****初始化代码结束
//%$#@describe      ****组件描述开始
//component name:"ls88e6060"//5口交换机驱动
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"network config"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_LS88E6060 == false )
//#warning  " ls88e6060  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_LS88E6060    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,255,
#define CFG_VLAN_INIT_ADDR0      32           //"VLAN ADDR0"
#define CFG_VLAN_INIT_ADDR1      32           //"VLAN ADDR1"
#define CFG_VLAN_INIT_ADDR2      32           //"VLAN ADDR2"
#define CFG_VLAN_INIT_ADDR3      32           //"VLAN ADDR3"
#define CFG_VLAN_INIT_ADDR4      32           //"VLAN ADDR4"
#define CFG_VLAN_INIT_ADDR5      31           //"VLAN ADDR5"
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure


//IEEE defined Registers (8846060)
#define PHY_BMCR        0x0   // Basic Mode Control Register
#define PHY_BMSR        0x1   // Basic Mode Status Register
#define PHY_PHYID1R     0x2   // PHY Identifier Register 1
#define PHY_PHYID2R     0x3   // PHY Identifier Register 2
#define PHY_ANAR        0x4   // Auto_Negotiation Advertisement Register
#define PHY_ANLPAR      0x5   // Auto_negotiation Link Partner Ability Register
#define PHY_ANER        0x6   // Auto-negotiation Expansion Register
#define PHY_ANNPR       0x7   // Auto-negotiation Next Page Register
#define PHY_ANLPNPAR    0x8   // Auto_negotiation Link Partner Next Page Ability Register

#define PHY_SPC1R       0x10  // Specific Control Register 1
#define PHY_SPS1        0x11  // Specific Status Register
#define PHY_IER         0x12  // Interrupt Enable Register
#define PHY_ISR         0x13  // Interrupt Status Register
#define PHY_IPR         0x14  // Interrupt Port Summary
#define PHY_REC         0x15  // Receive Error Counter


// PHY ID Identifier Register
#define PHY_LSB_MASK           (0X3FF)
// definitions: MII_PHYID1
#define PHY_OUI_MSB            0x0141
// definitions: MII_PHYID2
#define PHY_OUI_LSB            (0x3 << 10)

// Basic Mode Control Register (BMCR)
// Bit definitions: MII_BMCR
#define PHY_RESET             (1 << 15) // 1= Software Reset; 0=Normal Operation
#define PHY_LOOPBACK          (1 << 14) // 1=loopback Enabled; 0=Normal Operation
#define PHY_SPEED_SELECT_LSB  (1 << 13) // 0,1=100Mbps; 0,0=10Mbps
#define PHY_AUTONEG           (1 << 12) // Auto-negotiation Enable
#define PHY_POWER_DOWN        (1 << 11) // 1=Power down 0=Normal operation
#define PHY_ISOLATE           (1 << 10) // 1 = Isolates 0 = Normal operation
#define PHY_RESTART_AUTONEG   (1 << 9)  // 1 = Restart auto-negotiation 0 = Normal operation
#define PHY_DUPLEX_MODE       (1 << 8)  // 1 = Full duplex operation 0 = Normal operation
//      Reserved                7        // Read as 0, ignore on write
#define PHY_SPEED_SELECT_MSB  (1 << 6)  //
//      Reserved                5 to 0   // Read as 0, ignore on write


// Basic Mode Status Register (BMSR)
// Bit definitions: MII_BMSR
#define PHY_100BASE_T4        (1 << 15) // 100BASE-T4 Capable
#define PHY_100BASE_TX_FD     (1 << 14) // 100BASE-TX Full Duplex Capable
#define PHY_100BASE_T4_HD     (1 << 13) // 100BASE-TX Half Duplex Capable
#define PHY_10BASE_T_FD       (1 << 12) // 10BASE-T Full Duplex Capable
#define PHY_10BASE_T_HD       (1 << 11) // 10BASE-T Half Duplex Capable
//      Reserved                10 to 9  // Read as 0, ignore on write
#define PHY_EXTEND_STATUS     (1 << 8)  // 1 = Extend Status Information In Reg 15
//      Reserved                7
#define PHY_MF_PREAMB_SUPPR   (1 << 6)  // MII Frame Preamble Suppression
#define PHY_AUTONEG_COMP      (1 << 5)  // Auto-negotiation Complete
#define PHY_REMOTE_FAULT      (1 << 4)  // Remote Fault
#define PHY_AUTONEG_ABILITY   (1 << 3)  // Auto Configuration Ability
#define PHY_LINK_STATUS       (1 << 2)  // Link Status
#define PHY_JABBER_DETECT     (1 << 1)  // Jabber Detect
#define PHY_EXTEND_CAPAB      (1 << 0)  // Extended Capability

// Auto-negotiation Advertisement Register (ANAR)
// Auto-negotiation Link Partner Ability Register (ANLPAR)
// Bit definitions: MII_ANAR, MII_ANLPAR
#define PHY_NP               (1 << 15) // Next page Indication
//      Reserved               7
#define PHY_RF               (1 << 13) // Remote Fault
//      Reserved               12       // Write as 0, ignore on read
#define PHY_PAUSE_MASK       (3 << 11) // 0,0 = No Pause 1,0 = Asymmetric Pause(link partner)
                                        // 0,1 = Symmetric Pause 1,1 = Symmetric&Asymmetric Pause(local device)
#define PHY_T4               (1 << 9)  // 100BASE-T4 Support
#define PHY_TX_FDX           (1 << 8)  // 100BASE-TX Full Duplex Support
#define PHY_TX_HDX           (1 << 7)  // 100BASE-TX Support
#define PHY_10_FDX           (1 << 6)  // 10BASE-T Full Duplex Support
#define PHY_10_HDX           (1 << 5)  // 10BASE-T Support
//      Selector                 4 to 0   // Protocol Selection Bits
#define PHY_AN_IEEE_802_3      0x00001

// Phy Interrupt Enable Register
#define PHY_IER_LINK         (1 << 10)  // Link Status Changed Interrupt Enable

#define PHY_ISR_LINK         (1 << 10)  // Link Status Changed


#define CN_88E6060_TIMEOUT   100
#define CN_DEV_NUM           0X06

#define  CN_A4_HIGH           1

#ifdef  CN_A4_HIGH
#define CN_PHY_DEVBASE        0X10
#define CN_SWITCH_DEVBASE     0X18
#define CN_SWITCH_GLOBAL      0X1F
#else
#define CN_PHY_DEVBASE        0X00
#define CN_SWITCH_DEVBASE     0X08
#define CN_SWITCH_GLOBAL      0X0F

#endif
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//this function must be supplied by the mac device
//extern u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value);
//extern u8 GMAC_MdioW(u8 dev,u8 reg, u16 value);

//initialize the vlan,you could call the vlan function to set
static u16   gVlanInitialize[CN_DEV_NUM]=
{0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
//which means.port 0,1,2,3,4could send frame to port 5, but frame could not
//transmit between them;port 5 could snd frame to port 0,1,2,3,4


__attribute__((weak))  u8 GMAC_MdioR(u8 dev,u8 reg, u16 *value)
{
    return 0;
}

__attribute__((weak))  u8 GMAC_MdioW(u8 dev,u8 reg, u16 *value)
{
    return 0;
}
static u8 DevReset(u8 dev)
{
    u32 timeout;
    u16    bmcr = PHY_RESET;
    bmcr = PHY_RESET;
    GMAC_MdioW(dev, PHY_BMCR, bmcr);

    timeout = CN_88E6060_TIMEOUT;
    do {
        GMAC_MdioR(dev, PHY_BMCR, &bmcr);
        timeout--;
    } while ((bmcr & PHY_RESET) && timeout);
    GMAC_MdioW(dev-CN_PHY_DEVBASE + CN_SWITCH_DEVBASE, 0x06, gVlanInitialize[dev-CN_PHY_DEVBASE]);
    return  0;
}


static u8 VlanSet(u8 port)
{
    u8 dev;
    dev = port + CN_SWITCH_DEVBASE;
    GMAC_MdioW(dev, 0x06, gVlanInitialize[port]);
    return 0;
}

static u8  DevIntSet(u8 dev)
{

    u16  ier;

    GMAC_MdioR(dev,PHY_IER,&ier);
    ier|=(PHY_IER_LINK);
    GMAC_MdioW(dev,PHY_IER,ier);

    return 0;
}

static u8   DevGIntSet(void)
{
    u16  ier;

    GMAC_MdioR(CN_SWITCH_GLOBAL,0x04,&ier);
    ier|=(1<<1);
    GMAC_MdioW(CN_SWITCH_GLOBAL,0x04,ier);

    return 0;
}



//check whether the link is up
static bool_t DevLinkCheck(u8 port)
{
    u8 i;
    u16 bmsr;
    bool_t result;
    u8 dev;

    dev = port + CN_PHY_DEVBASE;
    i = 2;
    while(i--)  //连续读两次读出link状态
    {
        GMAC_MdioR(dev, PHY_BMSR,&bmsr);
    }
    if( bmsr & PHY_LINK_STATUS )
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}


static bool_t  PhyCheckLink(u8 port)
{
    bool_t result;
    u8 dev;
    u16 bmsr;

    dev = port + CN_PHY_DEVBASE;
    GMAC_MdioR(dev, PHY_BMSR,&bmsr);
    if( bmsr & PHY_LINK_STATUS )
    {
        result = true;
    }
    else
    {
        result = false;
    }
    return result;
}


/**
 * \brief Issue a Auto Negotiation of the PHY
 * \param pMacb Pointer to the MACB instance
 * \return 1 if successfully, 0 if timeout.
 */
enum  __EN_NEGOTIATION
{
    EN_LS_NEGOTIATION_FAILED = 0,
    EN_LS_NEGOTIATION_10M_H,   //10m half duplex
    EN_LS_NEGOTIATION_10M_F,   //10m full duplex
    EN_LS_NEGOTIATION_100M_H,  //100m half duplex
    EN_LS_NEGOTIATION_100M_F,  //100m full duplex
};

const char *gNegotiateComment[]={
  "NEGOTIATION FAILED",
  "NEGOTIATION 10M   HALF DUPLEX",
  "NEGOTIATION 10M   FULL DUPLEX",
  "NEGOTIATION 100M  HALF DUPLEX",
  "NEGOTIATION 100M  FULL DUPLEX",
};

#define CN_NEGOTIATION_TIMES  (10)
static u8 PhyLinkConfig(u8 port)
{
    u8   result = EN_LS_NEGOTIATION_FAILED;
    u16  value;
    u32  trytimes;
    u8   dev;

    dev = port + CN_PHY_DEVBASE;
    /** Set the Auto_negotiation Advertisement Register, MII advertising for
    Next page 100BaseTxFD and HD, 10BaseTFD and HD, IEEE 802.3 */
    //use the default advitisement to negotiation with the phy
//    value = PHY_TX_FDX | PHY_AN_IEEE_802_3;
    value = PHY_TX_FDX ;
    GMAC_MdioW(dev,PHY_ANAR, value);

    //set the phy to do the negotiation
    GMAC_MdioR(dev, PHY_BMCR, &value);
    value |=  PHY_AUTONEG | PHY_RESTART_AUTONEG;
    GMAC_MdioW(dev, PHY_BMCR, value);

//    //set the fixed speed
//    GMAC_MdioR(dev, PHY_BMCR, &value);
//    value = PHY_SPEED_SELECT_LSB|PHY_DUPLEX_MODE;
//    GMAC_MdioW(dev, PHY_BMCR, value);


    //reset the phy
    GMAC_MdioR(dev, PHY_BMCR, &value);
    value |=  PHY_RESET ;
    GMAC_MdioW(dev, PHY_BMCR, value);
    //check the link state
    printk("Dev %02d Negotiate and link wait...",dev);
    trytimes = 0;
    while(trytimes <CN_NEGOTIATION_TIMES)
    {
        GMAC_MdioR(dev, PHY_BMSR, &value);
        if(value &PHY_LINK_STATUS)
        {
            break;
        }
        else
        {
            if(0 == ((++trytimes)%10000))
            {
                printk(".");
            }
        }
    }
    if(trytimes <CN_NEGOTIATION_TIMES)
    {
        GMAC_MdioR(dev, PHY_ANLPAR, &value);
        /* Set up the GMAC link speed */
        if (value&PHY_TX_FDX)
        {
            /* set RGMII for 1000BaseTX and Full Duplex */
            result = EN_LS_NEGOTIATION_100M_F;
        } else if (value&PHY_10_FDX)
        {
            /* set RGMII for 1000BaseT and Half Duplex*/
            result = EN_LS_NEGOTIATION_10M_F;
        } else if (value& PHY_TX_HDX)
        {
            /* set RGMII for 100BaseTX and half Duplex */
            result = EN_LS_NEGOTIATION_100M_H;
        } else if (value&PHY_10_HDX)
        {
            // set RGMII for 10BaseT and half Duplex
            result = EN_LS_NEGOTIATION_10M_H;
        }
        printk("done--trytimes:0x%08x--speed:%s\n\r",trytimes,gNegotiateComment[result]);

    }
    else
    {
        printk("timeout--trytimes:0x%08x\n\r",trytimes);
    }

    return  result;
}


#include <shell.h>
const unsigned char  gPhyReg[]={
  0,1,2,3,4,5,6,7,8,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c
};
#define CN_PHYREGSIZE   sizeof(gPhyReg)
const unsigned char gSwitchReg[]={0,3,4,6,0x0b,0x10,0x11};
#define CN_SWITCH_SIZE   sizeof(gSwitchReg)

//static bool_t DevPhyRegPrint(char *param)
bool_t preg(char *param)
{
    int i =0;
    unsigned char     dev;

    u16  value;

    dev = (unsigned char )strtol(param,NULL,0);

    if(dev >= CN_DEV_NUM)
    {
        printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
        return true;
    }
    else
    {
        dev +=CN_PHY_DEVBASE;
    }

    printf("%s:phyaddr:0x%02x  RegsList\n\r",__FUNCTION__,dev);

    for(i =0;i <CN_PHYREGSIZE;i++ )
    {
        GMAC_MdioR(dev,gPhyReg[i],&value);
        printf("Reg:0x%02x :  value：0x%04x\n\r",gPhyReg[i],value);
    }

    printf("%s:phyaddr:0x%02x  End\n\r",__FUNCTION__,dev);

    return true;
}

//static bool_t DevSwitchRegPrint(char *param)
bool_t sreg(char *param)
{
    int i =0;
    unsigned char     dev;
    unsigned short    value;

    dev = (unsigned char )strtol(param,NULL,0);
    if(dev >= CN_DEV_NUM)
    {
        printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
        return true;
    }
    else
    {
        dev +=CN_SWITCH_DEVBASE;
    }

    printf("%s:switchaddr:0x%02x  RegsList\n\r",__FUNCTION__,dev);

    for(i =0;i <CN_SWITCH_SIZE;i++ )
    {
        GMAC_MdioR(dev,gSwitchReg[i],&value);
        printf("Reg:0x%02x :  value：0x%04x\n\r",gSwitchReg[i],value);
    }

    printf("%s:switchaddr:0x%02x  End\n\r",__FUNCTION__,dev);

    return true;
}

//static bool_t DevSetPhyReg(char *param)
bool_t setphy(char *param)
{

    u8 dev;
    u8 reg;
    u32 data[3];
    u16 value;

    int argc=3;
    char *argv[3];

    string2arg(&argc,argv,param);

    data[0]=strtol(argv[0],NULL,0);
    data[1]=strtol(argv[1],NULL,0);
    data[2]=strtol(argv[2],NULL,0);

    dev = (u8)data[0];
    reg = (u8)data[1];
    value =(u16)data[2];

    if(dev >= CN_DEV_NUM)
    {
        printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
        return true;
    }
    else
    {
        dev +=CN_PHY_DEVBASE;
    }

    printf("paranum:0x%02x phyaddr:0x%02x reg:0x%02x value:0x%04x\n\r",\
            argc,dev,reg,value);

    if(argc == 3)
    {
        GMAC_MdioW(dev,reg,value);
    }

    return true;

}

//static bool_t DevSetSwitchReg(char *param)
bool_t setswitch(char *param)
{

    u8 dev;
    u8 reg;
    u32 data[3];
    u16 value;

    int argc=3;
    char *argv[3];

    string2arg(&argc,argv,param);

    data[0]=strtol(argv[0],NULL,0);
    data[1]=strtol(argv[1],NULL,0);
    data[2]=strtol(argv[2],NULL,0);

    dev = (u8)data[0];
    reg = (u8)data[1];
    value =(u16)data[2];

    if(dev >= CN_DEV_NUM)
    {
        printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
        return true;
    }
    else
    {
        dev +=CN_SWITCH_DEVBASE;
    }

    printf("paranum:0x%02x switchaddr:0x%02x reg:0x%02x value:0x%04x\n\r",\
            argc,dev,reg,value);

    if(argc == 3)
    {
        GMAC_MdioW(dev,reg,value);
    }

    return true;

}


//static bool_t DevSetPhyLoop(char *param)
bool_t phyloop(char *param)
{
    int argc;
    char *argv[2];
    u8 dev;
    u8 onoff;
    u8 reg;
    u16 value;

    argc =2;

    string2arg(&argc,argv,param);

    if(argc == 2)
    {
        dev = (u8)strtol(argv[0],NULL,0);
        onoff = (u8)strtol(argv[1],NULL,0);
        reg = PHY_BMCR;

        if(dev >= CN_DEV_NUM)
        {
            printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
            return true;
        }
        else
        {
            dev +=CN_PHY_DEVBASE;
        }

        DevReset(dev);
        if(onoff)
        {
            GMAC_MdioR(dev,reg,&value);
            value &=(~(PHY_AUTONEG|PHY_RESTART_AUTONEG));
            GMAC_MdioW(dev,reg,value);

            GMAC_MdioR(dev,reg,&value);
            value |= (PHY_LOOPBACK|PHY_DUPLEX_MODE|PHY_SPEED_SELECT_LSB);
            GMAC_MdioW(dev,reg,value);
        }
        else
        {
            GMAC_MdioR(dev,reg,&value);
            value &= (~PHY_LOOPBACK);
            GMAC_MdioW(dev,reg,value);

            GMAC_MdioR(dev,reg,&value);
            value |=((PHY_AUTONEG|PHY_RESTART_AUTONEG|PHY_DUPLEX_MODE|PHY_SPEED_SELECT_LSB));
            GMAC_MdioW(dev,reg,value);
        }
        printf("%s:phyloop:%d devaddr:0x%02x reg:0x%02x value:0x%04x\n\r",\
                __FUNCTION__,onoff,dev,reg,value);

    }
    else
    {
        printf("%s:parameter %d is not enough!\n\r",__FUNCTION__,argc);
    }



    return true;
}
//static bool_t DevResetCmd(char *param)
bool_t phyreset(char *param)
{
    int argc;
    char *argv[2];
    u8 dev;


    argc =1;

    string2arg(&argc,argv,param);

    if(argc == 1)
    {
        dev = (u8)strtol(argv[0],NULL,0);

        if(dev >= CN_DEV_NUM)
        {
            printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
            return true;
        }
        else
        {
            dev +=CN_PHY_DEVBASE;
        }
        DevReset(dev);
    }
    else
    {
        printf("%s:parameter %d is not enough!\n\r",__FUNCTION__,argc);
    }
    return true;
}

//static bool_t DevVlanSet(char *param)
 bool_t vlan(char *param)
{
    int argc;

    char *argv[2];
    u8   dev;
    u16  vlan;

    argc =2;

    string2arg(&argc,argv,param);

    if(argc == 2)
    {
        dev = (u8)strtol(argv[0],NULL,0);
        vlan = (u16)strtol(argv[1],NULL,0);

        if(dev >= CN_DEV_NUM)
        {
            printf("%s：please input dev between 0~%d\n\r",__FUNCTION__,CN_DEV_NUM-1);
            return true;
        }
        else
        {
            gVlanInitialize[dev]=vlan;
            dev +=CN_SWITCH_DEVBASE;
        }

        DevReset(dev);
        GMAC_MdioW(dev,0x06,vlan);
    }
    else
    {
        printf("%s:parameter %d is not enough!\n\r",__FUNCTION__,argc);
    }
    return true;
}

static bool_t DevDebugInstall(void)
{
        return (TRUE);
}

void LS88e6060ShellCmdInit(void)
{
    DevDebugInstall();
}

//this is the 88e6060  initialize
ptu32_t ModuleInstall_LS88e6060(void)
{
    u8 i =0;

//    memcpy(gVlanInitialize,vlan,sizeof(gVlanInitialize));
    gVlanInitialize[0] = CFG_VLAN_INIT_ADDR0;
    gVlanInitialize[1] = CFG_VLAN_INIT_ADDR1;
    gVlanInitialize[2] = CFG_VLAN_INIT_ADDR2;
    gVlanInitialize[3] = CFG_VLAN_INIT_ADDR3;
    gVlanInitialize[4] = CFG_VLAN_INIT_ADDR4;
    gVlanInitialize[5] = CFG_VLAN_INIT_ADDR5;

    //check all the phy
    for(i =0; i< CN_DEV_NUM;i++)
    {
        if(i != (CN_DEV_NUM-1))
        {
            PhyLinkConfig(i);
        }
        VlanSet(i);
    }

    DevGIntSet();   //open the master interrupt
//    printf("autoconfig!\n\r");

    LS88e6060ShellCmdInit();
    return 0;
}


//this is the 88e6060 int handler
void   LS_IntHandler(void)
{

    u16 isr,ips,bmsr;
    u16 i;
    u16 value;
    u8 dev;
    u8 linkread;

    //读是哪个PORT的中断
    GMAC_MdioR(CN_PHY_DEVBASE, PHY_IPR,&ips);
    for(i = 0; i < CN_DEV_NUM; i++)
    {
        if(ips & (1<<i))
        {
            //读ISR，等于是清对应port的中断标记
            dev = CN_PHY_DEVBASE+i;
            GMAC_MdioR(dev, PHY_ISR,&isr);
            //对该phy进行处理
            linkread = 2;
            while(linkread--)  //连续读两次读出link状态
            {
                GMAC_MdioR(dev, PHY_BMSR,&bmsr);
            }
            if( bmsr & PHY_LINK_STATUS )
            {
                GMAC_MdioR(dev, PHY_BMCR,&value);

                if(0 == (value&PHY_LOOPBACK))
                {
                    //改phy需要重新配置
                    //复位该phy
                    DevReset(dev);
                    //设置中断
                    DevIntSet(dev);
                    //自动协商
                }

            }

            GMAC_MdioR(dev, PHY_ISR,&isr);
        }
    }

    return ;
}
ADD_TO_ROUTINE_SHELL(vlan,vlan,"usage:set the vlan map:vlan + port +value");
ADD_TO_ROUTINE_SHELL(phyreset,phyreset,"usage:reset the dev:phyreset+port");
ADD_TO_ROUTINE_SHELL(preg,preg,"usage:print the phyregs");
ADD_TO_ROUTINE_SHELL(sreg,sreg,"usage:print the switch regs");
ADD_TO_ROUTINE_SHELL(setphy,setphy,"usage:set the phy regs:setphy +port +reg +value");
ADD_TO_ROUTINE_SHELL(setswitch,setswitch,"usage:set the switch regs:setswitch +port +reg +value");
ADD_TO_ROUTINE_SHELL(phyloop,phyloop,"usage:reset the phy loop::phyloop+port");
