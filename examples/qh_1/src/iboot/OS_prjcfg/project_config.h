/****************************************************
 *  Automatically-generated file. Do not edit!  *
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          40      //定义锁的数量包含信号量和互斥量
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure kernel object system  ******************************************//
#define CFG_OBJECT_LIMIT        8   //用完会自动扩充
#define CFG_HANDLE_LIMIT        8   //用完会自动扩充
//*******************************  Configure memory pool  ******************************************//
#define CFG_MEMPOOL_LIMIT       10      //
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         15      //事件数量
#define CFG_EVENT_TYPE_LIMIT    15      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure cpu onchip iic  ******************************************//
#define    CFG_I2C1_BUF_LEN                128               //
#define    CFG_I2C2_BUF_LEN                128               //
#define    CFG_I2C1_ENABLE                 false             //
#define    CFG_I2C2_ENABLE                 false             //
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       10      //定义设备数量
//*******************************  Configure cpu onchip uart  ******************************************//
#define    CFG_UART1_SENDBUF_LEN           32                //
#define    CFG_UART1_RECVBUF_LEN           32                //
#define    CFG_UART1_DMABUF_LEN            32                //
#define    CFG_UART2_SENDBUF_LEN           32                //
#define    CFG_UART2_RECVBUF_LEN           32                //
#define    CFG_UART2_DMABUF_LEN            32                //
#define    CFG_UART3_SENDBUF_LEN           32                //
#define    CFG_UART3_RECVBUF_LEN           32                //
#define    CFG_UART3_DMABUF_LEN            32                //
#define    CFG_UART4_SENDBUF_LEN           32                //
#define    CFG_UART4_RECVBUF_LEN           32                //
#define    CFG_UART5_SENDBUF_LEN           32                //
#define    CFG_UART5_RECVBUF_LEN           32                //
#define    CFG_UART1_ENABLE                true              //
#define    CFG_UART2_ENABLE                false             //
#define    CFG_UART3_ENABLE                false             //
#define    CFG_UART4_ENABLE                false             //
#define    CFG_UART5_ENABLE                false             //
//*******************************  Configure graphical kernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure graphical decorate development(gdd)  ******************************************//
#define CFG_DESKTOP_WIDTH       0           //桌面尺寸（像素数）宽度0=显示器宽度
#define CFG_DESKTOP_HEIGHT      0           //桌面尺寸（像素数）高度0=显示器高度
#define CFG_DISPLAY_NAME        "DISPLAY_NAME"         //须与bsp中显示器驱动模块配置的显示器名字相同
#define CFG_DESKTOP_NAME        "DESKTOP_NAME"         //
#define CFG_INPUTDEV_NAME       "INPUTDEV_NAME"         //使用bsp中输入设备所配置的名字多输入设备的话每个设备间用逗号隔开
#define CFG_DESKTOP_FORMAT      CN_SYS_PF_RGB565    //桌面窗口像素格式常数在gkernel.h中定义一般使用与显示器相同颜色
#define CFG_GRAY_BASE_COLOR     CN_COLOR_WHITE      //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define CFG_FILL_COLOR          CN_COLOR_BLUE       //创建桌面时的填充色用888格式可在gkernel.h中找到常用颜色定义
//*******************************  Configure human machine interface  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
//*******************************  Configure stmpe811  ******************************************//
#define    CFG_CRT_CLK_FRE                 (100*1000)        //总线速度单位Hz
#define    CFG_STMPE811_DEVADDR            0x41              //IIC总线上的设备地址
#define    CFG_STMPE811_TOUCH_DEV_NAME     "TOUCH_STMPE811"  //触摸设备的名称
#define    CFG_STMPE811_DESKTOP_NAME       "LCD_DESKTOP_STMPE811"//触摸屏所在桌面的名称
//*******************************  Configure LCD driver ili9325  ******************************************//
#define    CFG_LCD_XSIZE                   240               //
#define    CFG_LCD_YSIZE                   128               //
#define    CFG_ILI9325_DISPLAY_NAME        "lcdili9325"      //配置液晶显示的名称
#define    CFG_ILI9325_HEAP_NAME           "extram"          //配置液晶驱动所使用的堆名称
//*******************************  Configure font  ******************************************//
#define    CFG_FONT_DEFAULT                "gb2312_song_16"  //字体名在include/font目录中找
//*******************************  Configure ascii dot font  ******************************************//
#define    CFG_ASCII_8X8                   false             //8*8点阵ascii字体
#define    CFG_ASCII_6X12                  false             //6*12点阵ascii字体
#define    CFG_ASCII_8X16_SONG             true              //8*16点阵ascii字体_宋体
#define    CFG_ASCII_8X16_YUAN             false             //8*16点阵ascii字体_圆体
#define    CFG_ASCII_8X16_KAI              false             //8*16点阵ascii字体_楷体
#define    CFG_ASCII_8X16_HEI              false             //8*16点阵ascii字体_黑体
#define    CFG_ASCII_8X16_FANG             false             //8*16点阵ascii字体_仿宋
#define    CFG_ASCII_12X24_SONG            false             //12*24点阵ascii字体_宋体
#define    CFG_ASCII_12X24_YUAN            false             //12*24点阵ascii字体_圆体
#define    CFG_ASCII_12X24_KAI             false             //12*24点阵ascii字体_楷体
#define    CFG_ASCII_12X24_HEI             false             //12*24点阵ascii字体_黑体
#define    CFG_ASCII_12X24_FANG            false             //12*24点阵ascii字体_仿宋
#define    CFG_ASCII_16X32_YUAN            false             //16*32点阵ascii字体_圆体
#define    CFG_ASCII_16X32_KAI             false             //16*32点阵ascii字体_楷体
#define    CFG_ASCII_16X32_HEI             false             //16*32点阵ascii字体_黑体
#define    CFG_ASCII_16X32_FANG            false             //16*32点阵ascii字体_仿宋
//*******************************  Configure loader  ******************************************//
#define    CFG_APP_RUNMODE                 CN_DIRECT_RUN     //CN_DIRECT_RUN=直接从flash中运行；CN_FORM_FILE=从文件系统加载到内存运行，
#define    CFG_APP_VERIFICATION            VERIFICATION_NULL //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define    CFG_IBOOT_VERSION               01                //Iboot发布版本号
#define    CFG_IBOOT_UPDATE_NAME           "/yaf2/iboot.bin" //
//*******************************  Configure Nls Charset  ******************************************//
#define    CFG_LOCAL_CHARSET               "gb2312"          //
//*******************************  Configure stdio  ******************************************//
#define CFG_STDIO_STDIN_MULTI      true         //
#define CFG_STDIO_STDOUT_FOLLOW    true         //
#define CFG_STDIO_STDERR_FOLLOW    true         //
#define CFG_STDIO_FLOAT_PRINT      true         //
#define CFG_STDIO_STDIOFILE        true         //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
//*******************************  Configure shell  ******************************************//
#define    CFG_SHELL_STACK                 0x1000            //
#define    CFG_ADD_ROUTINE_SHELL           true              //
#define    CFG_ADD_EXPAND_SHELL            true              //
#define    CFG_ADD_GLOBAL_FUN              false             //
#define    CFG_SHOW_ADD_SHEELL             true              //
#define  CFG_MODULE_ENABLE_SHELL         true
//*******************************  Configure time  ******************************************//
#define    CFG_LOCAL_TIMEZONE              8                 //北京时间是东8区
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (168.0*Mhz)       //主频，内核要用，必须定义

#endif
