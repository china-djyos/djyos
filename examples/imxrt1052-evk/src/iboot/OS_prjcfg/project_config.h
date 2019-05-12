/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure component Nls Charset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure component lock  ******************************************//
#define CFG_LOCK_LIMIT          100      //定义锁的数量包含信号量和互斥量
//*******************************  Configure component heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure device file system  ******************************************//
#define CFG_DEVFILE_LIMIT       20      //定义设备数量
//*******************************  Configure component file system  ******************************************//
#define     CFG_CLIB_BUFFERSIZE              512               //
//*******************************  Configure component font  ******************************************//
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16      //字体名在include/font目录中找
//*******************************  Configure component gb2312 dot  ******************************************//
#define     CFG_GB2312_12_SONG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_12_SONG_FILENAME      "zk_gb2316_12song.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_16_SONG               from_array        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_16_SONG_FILENAME      "zk_gb2316_16song.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_16_YUAN               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_16_YUAN_FILENAME      "zk_gb2316_16yuan.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_16_KAI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_16_KAI_FILENAME       "zk_gb2316_16kai.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_16_HEI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_16_HEI_FILENAME       "zk_gb2316_16hei.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_16_FANG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_16_FANG_FILENAME      "zk_gb2316_16fang.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_24_SONG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_24_SONG_FILENAME      "zk_gb2316_24song.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_24_YUAN               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_24_YUAN_FILENAME      "zk_gb2316_24yuan.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_24_KAI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_24_KAI_FILENAME       "zk_gb2316_24kai.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_24_HEI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_24_HEI_FILENAME       "zk_gb2316_24hei.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_24_FANG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_24_FANG_FILENAME      "zk_gb2316_24fang.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_32_SONG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_32_SONG_FILENAME      "zk_gb2316_32song.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_32_YUAN               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_32_YUAN_FILENAME      "zk_gb2316_32yuan.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_32_KAI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_32_KAI_FILENAME       "zk_gb2316_32kai.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_32_HEI                zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_32_HEI_FILENAME       "zk_gb2316_32hei.bin"//若从文件读取则配置文件名
#define     CFG_GB2312_32_FANG               zk_disable        //GB2312字体zk_disable：不需要from_array：从数组读取from_file：从文件读
#define     CFG_GB2312_32_FANG_FILENAME      "zk_gb2316_32fang.bin"//若从文件读取则配置文件名
//*******************************  Configure component gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure component gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                480                 //桌面尺寸（像素数）宽度0=显示器宽度
#define     CFG_DESKTOP_HEIGHT               272                  //桌面尺寸（像素数）高度0=显示器高度
#define     CFG_DISPLAY_NAME                 "LCD_DISPLAY"    //须与bsp中显示器驱动模块配置的显示器名字相同
#define     CFG_DESKTOP_NAME                 "Desktop"    //"         //",
#define     CFG_INPUTDEV_NAME                "TOUCH_FT5406"   //使用bsp中输入设备所配置的名字多输入设备的话每个设备间用逗号隔开
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565  //桌面窗口像素格式常数在gkernel.h中定义一般使用与显示器相同颜色
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE    //像素格式设为灰度时才需要设置的“最亮”色可在gkernel.h中找到常用颜色定义
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE     //创建桌面时的填充色用888格式可在gkernel.h中找到常用颜色定义
//*******************************  Configure third hm input  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
//*******************************  Configure component stdio  ******************************************//
#define CFG_STDIO_RUN_MODE            (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW) //
#define CFG_STDIO_IN_NAME              "/dev/UART1"    //
#define CFG_STDIO_OUT_NAME             "/dev/UART1"    //
#define CFG_STDIO_ERR_NAME             "/dev/UART1"    //
//*******************************  Configure cpu_peri_component uart  ******************************************//
#define CFG_UART1_SENDBUF_LEN    64             //
#define CFG_UART1_RECVBUF_LEN    64             //
#define CFG_UART2_SENDBUF_LEN    64             //
#define CFG_UART2_RECVBUF_LEN    64             //
#define CFG_UART3_SENDBUF_LEN    64             //
#define CFG_UART3_RECVBUF_LEN    64             //
#define CFG_UART4_SENDBUF_LEN    64             //
#define CFG_UART4_RECVBUF_LEN    64             //
#define CFG_UART5_SENDBUF_LEN    64             //
#define CFG_UART5_RECVBUF_LEN    64             //
#define CFG_UART6_SENDBUF_LEN    64             //
#define CFG_UART6_RECVBUF_LEN    64             //
#define CFG_UART7_SENDBUF_LEN    64             //
#define CFG_UART7_RECVBUF_LEN    64             //
#define CFG_UART8_SENDBUF_LEN    64             //
#define CFG_UART8_RECVBUF_LEN    64             //
#define CFG_UART1_ENABLE         true           //
#define CFG_UART2_ENABLE         false          //
#define CFG_UART3_ENABLE         false          //
#define CFG_UART4_ENABLE         false          //
#define CFG_UART5_ENABLE         false          //
#define CFG_UART6_ENABLE         false          //
#define CFG_UART7_ENABLE         false          //
#define CFG_UART8_ENABLE         false          //
//*******************************  Configure shell  ******************************************//
#define     CFG_ADD_ROUTINE_SHELL            true              //
#define     CFG_ADD_EXPAND_SHELL             true              //
#define     CFG_ADD_GLOBAL_FUN               false             //
#define     CFG_SHOW_ADD_SHEELL              true              //
//*******************************  Configure component kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         50      //事件数量
#define CFG_EVENT_TYPE_LIMIT    50      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure os memory poll  ******************************************//
#define CFG_MEMPOOL_LIMIT       20      //允许建立10个内存池
//*******************************  Configure component time  ******************************************//
#define     CFG_LOCAL_TIMEZONE               8                 //北京时间是东8区
//*******************************  Configure component loader  ******************************************//
#define CFG_IBOOT_VERSION       01        //Iboot发布版本号
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_APP_VERIFICATION   VERIFICATION_NULL      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
//*******************************  Configure third lib ft5406  ******************************************//
#define     CFG_FT5406_RT_I2C_ADDRESS        0x38              //硬件配置IIC设备地址
#define     CFG_FT5406_RT_I2C_CLK_FRE        (100*1000)        //单位Hz
#define     CFG_FT5406_BUS_NAME              "IIC1"            //触摸芯片使用的IIC总线名称
#define     CFG_FT5406_TOUCH_NAME            "TOUCH_FT5406"    //触摸芯片的名称
#define     CFG_FT5406_DESKTOP_NAME          "Desktop"                //触摸屏所使用的桌面的名称
//*******************************  Configure cpu driver iic  ******************************************//
#define     CFG_IIC1_BUF_LEN                 128               //
#define     CFG_IIC2_BUF_LEN                 128               //
#define     CFG_IIC3_BUF_LEN                 128               //
#define     CFG_IIC4_BUF_LEN                 128               //
#define     CFG_IIC1_ENABLE                  true             //
#define     CFG_IIC2_ENABLE                  false             //
#define     CFG_IIC3_ENABLE                  false             //
#define     CFG_IIC4_ENABLE                  false             //
//*******************************  Configure cpu driver lcd  ******************************************//
#define     CFG_LCD_DISPLAY_NAME             "LCD_DISPLAY"     //
#define     CFG_LCD_HEAP_NAME                "sdram"             //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (600*Mhz)         //主频，内核要用，必须定义

#endif
