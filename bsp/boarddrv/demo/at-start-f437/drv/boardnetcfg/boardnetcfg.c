#include <stdint.h>
#include <get_cpuid.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern void ModuleInstall_InitNet( );
//    ModuleInstall_InitNet( );
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"network config"//网络配置
//parent:"tcpip"     //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"tcpip","cpu onchip ETH" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_NETWORK_CONFIG == false )
//#warning  " network_config  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_NETWORK_CONFIG    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,true,false
#define CFG_STATIC_IP       true            //"使用静态IP?",
//%$#@string,1,32
#define CFG_SELECT_NETCARD  "STM32F4_ETH"   //"网卡名称",必须与选中的网卡驱动中配置的名称相同
//%$#@string,7,15
#define CFG_MY_IPV4         "192.168.0.179" //"静态IP",
#define CFG_MY_SUBMASK      "255.255.255.0" //"子网掩码",
#define CFG_MY_GATWAY       "192.168.0.1"   //"网关",
#define CFG_MY_DNS          "192.168.0.1"   //"首选DNS",
#define CFG_MY_DNSBAK       "192.168.0.1"   //"备用DNS",
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//please refers the following function in the module-trim in proper place.
//make sure that that os kernel has been initialize ok and the interrupt system intialize ok
//make sure that that board net hardware has initialized and could work ok
void ModuleInstall_InitNet(void)   //static ip example
{
    u32  signature[3];
#if CFG_STATIC_IP == 1
    u32 hop,subnet,ip,submask,dns,dnsbak;
    tagRouterPara para;
    struct NetDev *NetDev;

    NetDev = NetDev_GetHandle(CFG_SELECT_NETCARD);
    if(NetDev == NULL)          //网卡未安装
    {
        printk("%s netcard not install\r\n",CFG_SELECT_NETCARD);
        return;
    }

    memset(&para,0,sizeof(para));
    ip      = inet_addr(CFG_MY_IPV4);
    submask = inet_addr(CFG_MY_SUBMASK);
    hop     = inet_addr(CFG_MY_GATWAY);
    dns     = inet_addr(CFG_MY_DNS);
    dnsbak  = inet_addr(CFG_MY_DNSBAK);

    subnet = ip & submask;
    para.ver = EN_IPV_4;
    para.ifname = CFG_SELECT_NETCARD;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
//  para.dns = &dns;
//  para.dnsbak = &dnsbak;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.flags = 0;

    NetDev_SetDns(EN_IPV_4,NetDev, &dns, &dnsbak);
    NetDev_SetGateway(EN_IPV_4,NetDev, &hop);

    if(RouterCreate(&para))
    {
        printk("%s CreateRout:%s success\r\n",CFG_SELECT_NETCARD,CFG_MY_IPV4);
    }
    else
    {
        printk("%s CreateRout:%s failed\r\n",CFG_SELECT_NETCARD,CFG_MY_IPV4);
    }

    //下一个路由，用于生产测试用，利用CPU ID 随机生成主机地址，网络地址用 192.168.1
    //WE WILL ADD A ROUT DIFFERENT FOR EACH DEVICE USE THE CPU SIGNATURE
    //USE THE NET:192.168.1.xx
    u8 value8 = 0;
    GetCpuID(&signature[0],&signature[1],&signature[2]);
    value8 = +((u8)signature[0]>>0)+((u8)signature[0]>>8)+((u8)signature[0]>>16)+((u8)signature[0]>>24);
    if((value8==0)||(value8==1)||(value8==255))
    {
        value8=253;
    }
    u32 value32 = 0;
    value32 = inet_addr("192.168.1.0");
    value32 = ntohl(value32);
    value32 =(value32&0xffffff00) + value8;
    ip      = htonl(value32);
    submask = inet_addr("255.255.255.0");
//  hop  = inet_addr("192.168.1.1");

    subnet = ip & submask;
    para.ver = EN_IPV_4;
    para.ifname = CFG_SELECT_NETCARD;
    para.mask = &submask;
    para.net = &subnet;
    para.host = &ip;
//  para.hop = &hop;
    para.prior = CN_ROUT_PRIOR_UNI;
    para.flags = 0;

    if(RouterCreate(&para))
    {
        printk("%s CreateRout success\r\n",CFG_SELECT_NETCARD);
    }
    else
    {
        printk("%s CreateRout failed\r\n",CFG_SELECT_NETCARD);
    }

#else
    //如果重新启动系统后，希望快速连接，或者使用重启前的IP，可每次启动把申请到的IP
    //保存在文件中，利用第二个参数。
    if(DHCP_AddClientTask(CFG_SELECT_NETCARD,0))
    {
       printk("Add %s success\r\n",CFG_SELECT_NETCARD);
    }
    else
    {
        printk("Add %s failed\r\n",CFG_SELECT_NETCARD);
    }

#endif

    return ;
}

