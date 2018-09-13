
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"atsamv7"      //原厂固件库
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:第三方组件           //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:none                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
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
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//#ifndef CFG_STATIC_IP   //****检查参数是否已经配置好
//#warning    “网络配置”组件参数未配置，使用默认值
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select,1
//#defined __SAMV71J19__          //只能选中其中一个
//#defined __SAMV71J20__
//#defined __SAMV71J21__
//#defined __SAMV71N19__
//#defined __SAMV71N20__
//#defined __SAMV71N21__
//#defined __SAMV71Q19__
//#defined __SAMV71Q20__
#defined __SAMV71Q21__
//%$#@free
//#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//../source/gmac.c
//../source/gmacd.c
//../source/iso7816_4.c
//../source/mcan.c
//../source/mcid_dma.c
//../source/pio_capture.c
//../source/pio_it.c
//../source/pio.c
//../source/qspi_dma.c
//../source/qspi.c
//../source/sdramc.c
//../source/smc.c
//../source/twi.c
//../source/twid.c
//../source/uart_dma.c
//../source/uart.c
//../source/usart_dma.c
//../source/usart.c
//../source/xdma_hardware_interface.c
//../source/xdmac.c
//../source/xdmad.c
//%$#@end exclude
//%$#@include path
//..;..\include;
//..;..\include\samv7;
//%$#@end include path

