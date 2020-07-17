/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include <stdint.h>
#include <stddef.h>
//manual config start
//此处填写手动配置，DIDE生成配置文件时，不会被修改
//manual config end

#define    CN_RUNMODE_IBOOT                0                //IBOOT模式运行
#define    CN_RUNMODE_APP                2                //由IBOOT加载的APP
#define    CFG_RUNMODE                CN_RUNMODE_APP                //由IBOOT加载的APP
//*******************************  Configure black box  ******************************************//
#define    CFG_MODULE_ENABLE_BLACK_BOX     true
//*******************************  Configure int  ******************************************//
#define    CFG_MODULE_ENABLE_INT           true
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8    // "对象数初始值"，用完会自动扩充
#define CFG_HANDLE_LIMIT        8    // "句柄数初始值"，用完会自动扩充
#define    CFG_MODULE_ENABLE_KERNEL_OBJECT_SYSTEM  true
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40       // "锁的数量",定义锁的数量，包含信号量和互斥量
#define    CFG_MODULE_ENABLE_LOCK          true
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10       // "内存池数量限值",
#define    CFG_MODULE_ENABLE_MEMORY_POOL   true
//*******************************  Configure message queue  ******************************************//
#define    CFG_MODULE_ENABLE_MESSAGE_QUEUE true
//*******************************  Configure multiplex  ******************************************//
#define    CFG_MODULE_ENABLE_MULTIPLEX     true
//*******************************  Configure cpu onchip gpio  ******************************************//
#define    CFG_MODULE_ENABLE_CPU_ONCHIP_GPIO  true
//*******************************  Configure STM32 commond code  ******************************************//
#define    CFG_MODULE_ENABLE_STM32_COMMOND_CODE  true
//*******************************  Configure djybus  ******************************************//
#define    CFG_MODULE_ENABLE_DJYBUS        true
//*******************************  Configure can bus  ******************************************//
#define CFG_CAN_BUS_MSGQ_NUM        200      // "消息队列长度",
#define CFG_CAN_BUS_TIMEOUT         200      // "总线超时(ms)",
#define CFG_CAN_BUS_RCV_RING_LEN    100      // "接收buffer消息数",
#define    CFG_MODULE_ENABLE_CAN_BUS       true
//*******************************  Configure iicbus  ******************************************//
#define    CFG_MODULE_ENABLE_IICBUS        true
//*******************************  Configure io analog iic bus  ******************************************//
#define    CFG_MODULE_ENABLE_IO_ANALOG_IIC_BUS  true
//*******************************  Configure kernel  ******************************************//
#define    CFG_INIT_STACK_SIZE             4096              //定义初始化过程使用的栈空间一般按默认值就可以了
#define    CFG_EVENT_LIMIT                 15                //事件数量
#define    CFG_EVENT_TYPE_LIMIT            15                //事件类型数
#define    CFG_MAINSTACK_LIMIT             4096              //main函数运行所需的栈尺寸
#define    CFG_IDLESTACK_LIMIT             1024              //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define    CFG_IDLE_MONITOR_CYCLE          30                //监视IDLE事件持续低于1/16 CPU占比的时间秒数，0=不监视
#define    CFG_OS_TINY                     false             //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字，事件处理时间统计。
#define    CFG_MODULE_ENABLE_KERNEL        true
//*******************************  Configure board config  ******************************************//
#define    CFG_MODULE_ENABLE_BOARD_CONFIG  true
//*******************************  Configure misc  ******************************************//
#define    CFG_MODULE_ENABLE_MISC          true
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
#define    CFG_MODULE_ENABLE_TIME          true
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024     // "命令缓冲区长度",上层应用（例如gdd）向gkernel传递命令的缓冲区长度（字节数）
#define CFG_USER_REQUEST_DEEP       128     //gkernel向上层请求命令的缓冲区长度（字节数）
#define    CFG_MODULE_ENABLE_GRAPHICAL_KERNEL  true
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true   // "全功能动态分配",即使选false，也允许使用malloc-free分配内存，但使用有差别，详见 《……用户手册》内存分配章节
#define    CFG_MODULE_ENABLE_HEAP          true
//*******************************  Core Clock  ******************************************//
#define  CFG_CORE_MCLK                   (400*Mhz)         //主频，内核要用，必须定义
//*******************************  DjyosProduct Configuration  ******************************************//
#define    PRODUCT_MANUFACTURER_NAME       ""                //厂商名称
#define    PRODUCT_PRODUCT_CLASSIFY        ""                //产品分类
#define    PRODUCT_PRODUCT_MODEL           ""                //产品型号
#define    PRODUCT_VERSION_LARGE           0                 //版本号,__.xx.xx
#define    PRODUCT_VERSION_MEDIUM          0                 //版本号,xx.__.xx
#define    PRODUCT_VERSION_SMALL           0                 //版本号,xx.xx.__
#define    PRODUCT_PRODUCT_MODEL_CODE      ""                //产品型号编码
#define    PRODUCT_PASSWORD                ""                //产品秘钥
#define    PRODUCT_OTA_ADDRESS             ""                //OTA服务器地址
#define    PRODUCT_BOARD_TYPE              ""                //板件类型
#define    PRODUCT_CPU_TYPE                ""                //CPU类型



#endif
