
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中


//#define ADD_TO_ROUTINE_SHELL(cmdname,fun,help) \
//                __attribute__((section(".ro_shell_cmd")))\
//                const struct shell_cmd djysh_##cmdname = \
//                        {.shell_fun_addr  = fun,.shell_help_addr = help,}
//#if BUILDING_SFTPC
//bool_t sftp_client(char *Param);
//ADD_TO_ROUTINE_SHELL(sftpc,sftp_client,"start the sftp client,usage: sftpc -h host -p port -u user -P password\n example:  sftpc -h 192.168.0.1 -p 2323 -u user -P user");
//#endif
//#if BUILDING_SFTPD
//bool_t sftp_server(char *Param);
//ADD_TO_ROUTINE_SHELL(sftpd,sftp_server,"start the sftp server,usage: sftpd -p port\n example:  sftpd -p 22222");
//#endif

//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"wolfssh130"  //组件名称
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"wolfssl3157"      //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_WOLFSSH130 == false )
//#warning  " wolfssh130  组件参数未配置，使用默认配置"
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_WOLFSSH130    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,65535
#define SFTP_SERVER_DEFAULT_LISTEN_PORT        22222      //"sftpd默认监听端口"
//%$#@enum,true,false
#define BUILDING_SFTPC          false
#define BUILDING_SFTPD          false
//%$#@select
//%$#@string,1,32
#define serverKeyRsaPemFile "/efs/server-key-rsa.pem"   //"rsa.pem file"，不使用文件系统则不需要配置
#define serverKeyEccDerFile "/efs/server-key-ecc.der"   //"ecc.der file"
#define serverKeyRsaDerFile "/efs/server-key-rsa.der"   //"rsa.der file"

#define SFTP_SERVER_USERNAME_PASSWORD_STRINGS  "jill:upthehill\n jack:fetchapail\n" //"用户名密码，格式为: username:password\n"
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
//./examples;./src/keygen.c;./src/misc.c;./tests;

//%$#@endexclude
//%$#@self include path
//./;
//%$#@end self include path
//@#$%component end configure

