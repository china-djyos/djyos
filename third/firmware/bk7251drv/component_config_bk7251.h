
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"bk7251"       //原厂固件库
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"none"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"bk7251V3.0"            //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BK7251 == false )
//#warning  " bk7251  组件参数未配置，使用默认配置"
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BK7251    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select
//%$#@SYMBOL
#define CN_BEKEN_SDK_USE    1
#define CN_BEKEN_SDK_V2     1
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//./common;
//./driver/ble;./driver/codec;./driver/entry;./driver/fft;
//./driver/i2c;./driver/i2s;./driver/irda;./driver/jpeg;./driver/rc_beken;
//./driver/sdio;./driver/spidma;./driver/usb;
//./driver/driver.c;./driver/SConscript;./driver/pwm/timer.c;
//./func/bk7011_cal/bk7011_cal.c;./func/bk7011_cal/bk7231_cal.c;./func/bk7011_cal/bk7231U_cal.c;
//./func/bk7011_cal/manual_cal_bk7231.c;./func/hostapd-2.5/src/crypto/crypto_ali.c;
//./func/lwip_intf/lwip-2.0.2/doc;./func/lwip_intf/lwip-2.0.2/src/apps;
//./func/lwip_intf/lwip-2.0.2/test;//./func/lwip_intf/platform_stub.c;
//./func/rttos/ate;./func/rttos/source;./func/rttos/os_clock.c;./func/rttos/target_util.c;
//./func/user_driver/BkDriverGpio.c;./func/user_driver/BkDriverPwm.c;
//./func/user_driver/BkDriverUart.c;./func/user_driver/BkDriverWdg.c;./func/mem_arch.c;
//./func/wlan_ui/lsig_monitor.c;./func/voice_transfer;./func/vad;
//./func/saradc_intf;./func/misc;./func/music_player;./app/app_demo;
//./app/airkiss;./app/config;
//./ip;
//%$#@end exclude
//%$#@API include path
//./;
//./app;
//./app/standalone-ap;./app/standalone-station;
//./common;
//./config;
//./demo;
//./driver;
//./driver/audio;./driver/common;./driver/common/reg;./driver/demo;
//./driver/dma;./driver/flash;./driver/entry;./driver/include;
//./driver/phy;./driver/pwm;./driver/sys_ctrl;./driver/uart;./driver/qspi;
//./func/ethernet_intf;./func/hostapd-2.5;./func/hostapd-2.5/bk_patch;
//./func/hostapd-2.5/hostapd;./func/hostapd-2.5/src;./func/hostapd-2.5/src/ap;
//./func/hostapd-2.5/src/common;./func/hostapd-2.5/src/drivers;./func/hostapd-2.5/src/utils;
//./ip/umac/src/apm;./func/include;./func/joint_up;./func/lwip_intf/lwip-2.0.2/port;
//./func/lwip_intf/lwip-2.0.2/port/arch;./func/lwip_intf/lwip-2.0.2/src/include;
//./func/os/include;./func/rttos/include;./func/rwnx_intf;./func/temp_detect;
//./func/uart_debug;./func/user_driver;
//./ip/common;
//./ip/ke;
//./ip/lmac;./ip/lmac/src/rwnx;./ip/lmac/src/tx;./ip/lmac/src/tx/txl;
//./ip/lmac/src/hal;./ip/lmac/src/scan;./ip/lmac/src/mm;./ip/lmac/src/bfr;
//./ip/lmac/src/chan;./ip/lmac/src/p2p;./ip/lmac/src/rd;./ip/lmac/src/rx;
//./ip/umac/src/bam;./ip/lmac/src/sta;./ip/lmac/src/tpc;./ip/lmac/src/tdls;
//./ip/lmac/src/td;./ip/lmac/src/vif;./ip/lmac/src/ps;./ip/lmac/src/rx/rxl;
//./ip/mac;
//./ip/umac;./ip/umac/src/me;./ip/umac/src/mesh;
//./ip/umac/src/mfp;./ip/umac/src/rc;./ip/umac/src/rxu;./ip/umac/src/scanu;
//./ip/umac/src/llc;./ip/umac/src/sm;./ip/umac/src/txu;
//%$#@end API include path

//%$#@self include path
//%$#@end self include path

//%$#@library path
//./ip/libip_7221u.a
//%$#@end library path

//@#$%component end configure
