
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"wolfssl3157"//组件名称
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
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_WOLFSSL3157 == false )
//#warning  " wolfssl3157  组件参数未配置，使用默认配置"
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_WOLFSSL3157    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@SYMBOL
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//./wolfcrypt/benchmark;./wolfcrypt/test;./wolfcrypt/user-crypto;./tirtos;./testsuite;./tests;
//./swig;./sslSniffer;./mqx;./mplabx;./mcapi;./IDE;./examples;./ctaocrypt;
//./src/bio.c;./src/crl.c;./src/ocsp.c;./src/sniffer.c;./src/tls13.c;
//./wolfcrypt/src/port;./wolfcrypt/src/aes_asm.S;./wolfcrypt/src/arc4.c;./wolfcrypt/src/asm.c;
//./wolfcrypt/src/async.c;./wolfcrypt/src/blake2b.c;./wolfcrypt/src/camellia.c;./wolfcrypt/src/cmac.c;
//./wolfcrypt/src/compress.c;./wolfcrypt/src/cpuid.c;./wolfcrypt/src/cryptodev.c;
//./wolfcrypt/src/curve25519.c;./wolfcrypt/src/des3.c;./wolfcrypt/src/dsa.c;
//./wolfcrypt/src/ecc_fp.c;./wolfcrypt/src/ed25519.c;./wolfcrypt/src/evp.c;
//./wolfcrypt/src/fe_low_mem.c;./wolfcrypt/src/fe_operations.c;./wolfcrypt/src/fips.c;
//./wolfcrypt/src/fips_test.c;./wolfcrypt/src/ge_low_mem.c;./wolfcrypt/src/ge_operations.c;
//./wolfcrypt/src/hc128.c;./wolfcrypt/src/idea.c;./wolfcrypt/src/integer.c;./wolfcrypt/src/md2.c;
//./wolfcrypt/src/md4.c;./wolfcrypt/src/misc.c;./wolfcrypt/src/pkcs7.c;./wolfcrypt/src/pkcs12.c;
//./wolfcrypt/src/pwdbased.c;./wolfcrypt/src/rabbit.c;./wolfcrypt/src/ripemd.c;./wolfcrypt/src/selftest.c;
//./wolfcrypt/src/sp_arm32.c;./wolfcrypt/src/sp_arm64.c;./wolfcrypt/src/sp_armthumb.c;
//./wolfcrypt/src/sp_c32.c;./wolfcrypt/src/sp_c64.c;./wolfcrypt/src/sp_int.c;./wolfcrypt/src/sp_x86_64.c;
//./wolfcrypt/src/srp.c;./wolfcrypt/src/wc_pkcs11.c;./wolfcrypt/src/wolfcrypt_first.c;
//./wolfcrypt/src/wolfcrypt_last.c;./wolfcrypt/src/wolfevent.c

//%$#@endexclude
//%$#@self include path
//./;
//%$#@end self include path
//@#$%component end configure

