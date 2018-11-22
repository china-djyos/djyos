
//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"stm32f3"      //原厂固件库
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third               //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:none                //初始化时机，可选值：early，medium，later。
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
//%$#@target = cmdline   //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//#ifndef CFG_STATIC_IP   //****检查参数是否已经配置好
//#warning    “网络配置”组件参数未配置，使用默认值
//%$#@num,0,100
//%$#@enum,true,false
//%$#@string,1,16
//%$#@select,2
#define     USE_HAL_DRIVER          //是否使用hal库
//#define STM32F301x8             //只能选中其中一个
//#define STM32F302x8
//#define STM32F302xC
//#define STM32F302xE
//#define STM32F303x8
//#define STM32F303xC
//#define STM32F303xE
//#define STM32F373xC
//#define STM32F334x8
//#define STM32F318xx
//#define STM32F328xx
//#define STM32F358xx
//#define STM32F378xx
//#define STM32F398xx
//%$#@free
//#endif
//%$#@end configue  ****参数配置结束
//%$#@exclude
//%$#@end exclude
//%$#@include path
//..;..\inc;
//%$#@end include path

