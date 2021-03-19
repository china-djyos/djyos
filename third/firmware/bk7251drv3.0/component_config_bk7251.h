
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"bk7251V3.0"       //原厂固件库
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
//mutex:"bk7251"                //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_BK7251V3P0 == false )
//#warning  " bk7251  组件参数未配置，使用默认配置"
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_BK7251V3P0    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select
//%$#@SYMBOL
#define CN_BEKEN_SDK_USE    1
#define CN_BEKEN_SDK_V3     1
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//./common;./alios;./os;
//./driver/codec;./driver/fft;
//./driver/i2c;./driver/irda;./driver/jpeg;./driver/rc_beken;
//./driver/sdio;./driver/spidma;./driver/usb;
//./driver/driver.c;./driver/SConscript;./driver/pwm/timer.c;
//./func/bk7011_cal/bk7011_cal.c;./func/bk7011_cal/bk7231_cal.c;./func/bk7011_cal/bk7231U_cal.c;
//./func/bk7011_cal/manual_cal_bk7231.c;./func/hostapd-2.5/src/crypto/crypto_ali.c;
//./func/lwip_intf;
//./func/user_driver/BkDriverGpio.c;./func/user_driver/BkDriverPwm.c;
//./func/user_driver/BkDriverUart.c;./func/user_driver/BkDriverWdg.c;./func/mem_arch.c;
//./func/wlan_ui/lsig_monitor.c;./func/voice_transfer;./func/vad;
//./func/saradc_intf;./func/misc;./func/music_player;./app/app_demo;
//./app/airkiss;./driver/entry;
//./ip;
//%$#@end exclude
//%$#@API include path         ****API头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，应用本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//.;
//./app;./app/config;
//./app/standalone-ap;./app/standalone-station;
//./common;
//./demo;
//./driver;./djyos/include;
//./driver/ble;./driver/ble/ble_pub/modules/rwip/api;./driver/ble/ble_lib/ip/ble/ll/src/rwble;
//./driver/ble/ble_lib/ip/ble/hl/inc;./driver/ble/ble_lib/ip/ble/hl/api;
//./driver/ble/ble_pub/ip/ble/hl/inc;./driver/ble/ble_pub/plf/refip/src/arch;
//./driver/ble/ble_pub/modules/app/api;./driver/ble/ble_pub/plf/refip/src/arch/compiler;
//./driver/ble/ble_pub/plf/refip/src/build/ble_full/reg/fw;./driver/ble/ble_pub/plf/refip/src/arch/ll;
//./driver/ble/ble_pub/plf/refip/src/driver/reg;./driver/ble/ble_pub/modules/common/api;
//./driver/ble/ble_lib/ip/em/api;./driver/ble/ble_lib/ip/ble/ll/src/em;
//./driver/ble/ble_lib/modules/h4tl/api;./driver/ble/ble_pub/modules/dbg/api;
//./driver/ble/ble_lib/ip/ea/api;./driver/ble/ble_pub/modules/rf/api;
//./driver/ble/ble_pub/plf/refip/src/driver/ble_icu;./driver/ble/ble_lib/modules/ke/api;
//./driver/ble/ble_lib/ip/hci/api;./driver/ble/ble_pub/modules/ecc_p256/api;
//./driver/ble/ble_lib/ip/ble/ll/src/llc;./driver/ble/ble_lib/ip/ble/ll/src/llm;
//./driver/ble/ble_lib/ip/ble/ll/src/lld;./driver/ble/ble_lib/modules/ke/src;
//./driver/ble/ble_lib/ip/ahi/api;./driver/ble/ble_pub/ip/ble/profiles/sdp/api;
//./driver/ble/ble_pub/ip/ble/profiles/comm/api;./driver/ble/ble_lib/ip/ble/hl/src/gatt/atts;
//./driver/audio;./driver/common;./driver/common/reg;./driver/demo;./driver/sdcard;./driver/icu;
//./driver/dma;./driver/flash;./driver/include;./driver/gpio;./driver/intc;
//./driver/phy;./driver/pwm;./driver/sys_ctrl;./driver/uart;./driver/qspi;
//./func/ethernet_intf;./func/hostapd-2.5;./func/hostapd-2.5/bk_patch;
//./func/hostapd-2.5/hostapd;./func/hostapd-2.5/src;./func/hostapd-2.5/src/ap;
//./func/hostapd-2.5/src/common;./func/hostapd-2.5/src/drivers;./func/hostapd-2.5/src/utils;
//./ip/umac/src/apm;./func/include;./func/joint_up;
//./func/rwnx_intf;./func/temp_detect;
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

//%$#@self include path         ****编译自身需要的头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，编译本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//%$#@end self include path

//%$#@library path             ****列出本组件引用的.a库文件列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符
//./ip/libip_7221u_sta.a;./driver/ble/libble.a
//%$#@end library path

//@#$%component end configure
