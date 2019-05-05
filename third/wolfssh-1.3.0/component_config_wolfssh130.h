
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"wolfssh130"   //组件名称
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"wolfssl3157"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef BUILDING_WOLFSSL   //****检查参数是否已经配置好
//#warning    “网络配置”组件参数未配置，使用默认值
//%$#@num,0,65535
#define SFTP_SERVER_DEFAULT_LISTEN_PORT        22222      //"ssh server listen port"
//%$#@enum,true,false
//%$#@select
//%$#@string,1,16
#define serverKeyRsaPemFile "/efs/server-key-rsa.pem"   //"rsa.pem file"，不使用文件系统则不需要配置
#define serverKeyEccDerFile "/efs/server-key-ecc.der"   //"ecc.der file"
#define serverKeyRsaDerFile "/efs/server-key-rsa.der"   //"rsa.der file"

#define SFTP_SERVER_USERNAME_PASSWORD_STRINGS  "jill:upthehill\n jack:fetchapail\n" //"server username and password"
//%$#@SYMBOL
#define SIZEOF_LONG_LONG 8
#define NO_WRITEV
#define BUILDING_WOLFSSL
#define WOLFSSL_PUBLIC_MP
#define _POSIX_THREADS
#define HAVE_THREAD_LS
#define TFM_TIMING_RESISTANT
#define ECC_TIMING_RESISTANT
#define WC_RSA_BLINDING
#define HAVE_AESGCM
#define WOLFSSL_SHA512
#define WOLFSSL_SHA384
#define NO_DSA
#define HAVE_ECC
#define TFM_ECC256
#define ECC_SHAMIR
#define WOLFSSL_BASE64_ENCODE
#define NO_RC4
#define NO_HC128
#define NO_RABBIT
#define WOLFSSL_SHA224
#define WOLFSSL_SHA3
#define HAVE_POLY1305
#define HAVE_ONE_TIME_AUTH
#define HAVE_CHACHA
#define HAVE_HASHDRBG
#define HAVE_TLS_EXTENSIONS
#define HAVE_SUPPORTED_CURVES
#define HAVE_EXTENDED_MASTER
#define NO_PSK
#define NO_MD4
#define NO_PWDBASED
#define USE_FAST_MATH
#define WOLFSSL_X86_64_BUILD
#define WC_NO_ASYNC_THREADING
#define NO_DES3
#define HAVE___UINT128_T
#define BUILDING_WOLFSSH
#define HAVE_CYASSL_OPTIONS
#define WOLFSSH_SFTP
#define HAVE_CONFIG_H
#define NDEBUG

//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
./examples;./src/keygen.c;./src/misc.c;./tests;

//%$#@endexclude
//%$#@includepath
//./;
//%$#@endincludepath

