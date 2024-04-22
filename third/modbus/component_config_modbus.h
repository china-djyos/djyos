#include <mbport.h>
#include <user_mb_app.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Modbus(void);
//    ModuleInstall_Modbus();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"modbus" //内核对象系统
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:third              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable               //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:pre-main                //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","Software Timers"             //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_MODBUS == false )
//#warning  " kernel_object_system  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_MODBUS    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,1,65536
#define CFG_MB_ASCII_TIMEOUT_SEC                    1       //"从机用ASCII时，通信的超时时间",实际的超时时间是CFG_MB_ASCII_TIMEOUT_SEC*20000*50
#define CFG_MB_FUNC_HANDLERS_MAX                    16      //"协议栈应该支持的Modbus功能码的最大数量"
#define CFG_MB_FUNC_OTHER_REP_SLAVEID_BUF           32      //"从机ID数据长度",这个数字限制了从机ID数据的最大大小。
#define CFG_MB_MASTER_DELAY_MS_CONVERT              200     //"等待延迟时间",主站发送一个广播帧后，主站等待延迟时间，时间到后才主站才可以发送其他帧
#define CFG_MB_MASTER_TIMEOUT_MS_RESPOND            100     //"从机响应超时时间"
#define CFG_MB_MASTER_TOTAL_SLAVE_NUM               16      //"从机总数"
//%$#@enum,true,false
#define CFG_MB_FUNC_OTHER_REP_SLAVEID_ENABLED       true    // "是否启用请求从机ID功能"
#define CFG_MB_FUNC_READ_INPUT_ENABLED              true    // "是否启用读输入寄存器功能"
#define CFG_MB_FUNC_READ_HOLDING_ENABLED            true    // "是否启用读保持寄存器功能"
#define CFG_MB_FUNC_WRITE_HOLDING_ENABLED           true    // "是否启用写单个寄存器功能"
#define CFG_MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  true    // "是否启用写多个寄存器功能"
#define CFG_MB_FUNC_READ_COILS_ENABLED              true    // "是否启用读线圈功能"
#define CFG_MB_FUNC_WRITE_COIL_ENABLED              true    // "是否启用写线圈功能"
#define CFG_MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED    true    // "是否启用写多个线圈功能"
#define CFG_MB_FUNC_READ_DISCRETE_INPUTS_ENABLED    true    // "是否启用读离散输入功能"
#define CFG_MB_FUNC_READWRITE_HOLDING_ENABLED       true    // "是否启用读写多个寄存器功能"

//%$#@enum,true,false
#define CFG_MB_MASTER_ENABLED                       false    // "主机使能开关"
    //%$#@enum,true,false
    #define CFG_MB_MASTER_RTU_ENABLED                   false    // "主机RTU使能开关"
    //%$#@num,0,65535
    #define CFG_MB_M_DISCRETE_INPUT_START               1        //"主机离散量输入的起始地址"
    #define CFG_MB_M_DISCRETE_INPUT_NDISCRETES          10       //"主机离散量输入的数量"
    #define CFG_MB_M_COIL_START                         11       //"主机线圈的起始地址"
    #define CFG_MB_M_COIL_NCOILS                        10       //"主机线圈的数量"
    #define CFG_MB_M_REG_INPUT_START                    21       //"主机输入寄存器的起始地址"
    #define CFG_MB_M_REG_INPUT_NREGS                    10       //"主机输入寄存器的数量"
    #define CFG_MB_M_REG_HOLDING_START                  31       //"主机保持寄存器的起始地址"
    #define CFG_MB_M_REG_HOLDING_NREGS                  10       //"主机保持寄存器的数量"
    #define CFG_MB_M_PORT_NUM                           2        //"串口的端口号，例如要用/dev/UART2，就填2"
    //%$#@enum,110,300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200,230400,380400,460800,921600
    #define CFG_MB_M_PORT_BAUDRATE                      115200    // "串口波特率"
    //%$#@enum,MB_PAR_NONE,MB_PAR_ODD,MB_PAR_EVEN
    #define CFG_MB_M_PORT_PARITY                        MB_PAR_NONE    // "校验方式"

//%$#@enum,true,false
#define CFG_MB_SLAVE_ENABLED                       false    // "从机使能开关"
    //%$#@enum,true,false
    #define CFG_MB_SLAVE_ASCII_ENABLED                  false    // "从机ASCII使能开关"
    #define CFG_MB_SLAVE_RTU_ENABLED                    false    // "从机RTU使能开关"
    #define CFG_MB_SLAVE_TCP_ENABLED                    false    // "从机TCP使能开关"

    //%$#@num,1,65536
    #define CFG_MB_S_DISCRETE_INPUT_START               1        //"从机离散量输入的起始地址"
    #define CFG_MB_S_DISCRETE_INPUT_NDISCRETES          10       //"从机离散量输入的数量"
    #define CFG_MB_S_COIL_START                         11       //"从机线圈的起始地址。"
    #define CFG_MB_S_COIL_NCOILS                        10       //"从机线圈的数量"
    #define CFG_MB_S_REG_INPUT_START                    21       //"从机输入寄存器的起始地址"
    #define CFG_MB_S_REG_INPUT_NREGS                    10       //"从机输入寄存器的数量"
    #define CFG_MB_S_REG_HOLDING_START                  31       //"从机保持寄存器的起始地址"
    #define CFG_MB_S_REG_HOLDING_NREGS                  10       //"从机保持寄存器的数量"
    #define CFG_MB_S_ADDR                               2        //"从机地址"
    #define CFG_MB_S_PORT_NUM                           2        //"串口的端口号，例如要用/dev/UART2，就填2"，串口的设置属性，在使用RTU或ASCII模式时才有效
    //%$#@num,0,65536
    #define CFG_MB_S_TCP_PORT_NUM                       0        //"使用TCP时，使用的端口号，0表示使用默认端口（502），使用TCP模式时才有效"
    //%$#@enum,110,300,600,1200,2400,4800,9600,14400,19200,38400,57600,115200,230400,380400,460800,921600
    #define CFG_MB_S_PORT_BAUDRATE                      115200    // "串口波特率"
    //%$#@enum,MB_PAR_NONE,MB_PAR_ODD,MB_PAR_EVEN
    #define CFG_MB_S_PORT_PARITY                        MB_PAR_NONE    // "校验方式"
    //%$#@enum,MB_RTU_MODE,MB_TCP_MODE,MB_ASCII_MODE
    #define CFG_MB_S_TRANSFER_MODE                      MB_RTU_MODE    // "通信方式"
//%$#@string,1,10
//%$#@select
//%$#@free
#endif
//%$#@end configue  ****参数配置结束

//%$#@API include path         ****API头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，应用本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//./include;./ascii;./port;./rtu;./tcp;
//%$#@end API include path

//%$#@self include path         ****编译自身需要的头文件路径列表，用分号“;”隔开，以本文件所在目录为当前目录，允许用通配符，编译本库时，DIDE将自动把本列表中的头文件加到 -I 搜索路径中
//%$#@end self include path

//@#$%component end configure
