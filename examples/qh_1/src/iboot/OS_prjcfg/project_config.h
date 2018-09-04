/****************************************************
 *  Automatically-generated file. Do not edit!	*
 ****************************************************/

#ifndef __PROJECT_CONFFIG_H__
#define __PROJECT_CONFFIG_H__

#include "stdint.h"

//*******************************  Configure DevFile  ******************************************//
#define CFG_DEVFILE_LIMIT       20      //定义设备数量
//*******************************  Configure lock  ******************************************//
#define CFG_LOCK_LIMIT          100      //定义锁的数量包含信号量和互斥量
//*******************************  Configure kernel  ******************************************//
#define CFG_INIT_STACK_SIZE     4096    //定义初始化过程使用的栈空间一般按默认值就可以了
#define CFG_EVENT_LIMIT         50      //事件数量
#define CFG_EVENT_TYPE_LIMIT    50      //事件类型数
#define CFG_MAINSTACK_LIMIT     4096    //main函数运行所需的栈尺寸
#define CFG_IDLESTACK_LIMIT     1024    //IDLE事件处理函数运行的栈尺寸一般按默认值就可以了
#define CFG_OS_TINY             false   //true=用于资源紧缺的场合内核会裁剪掉：事件类型名字事件处理时间统计。
//*******************************  Configure heap  ******************************************//
#define CFG_DYNAMIC_MEM true  //即使选false也允许使用malloc-free分配内存但使用有差别详见 《……用户手册》内存分配章节
//*******************************  Configure MemoryPool  ******************************************//
#define CFG_MEMPOOL_LIMIT       20      //允许建立10个内存池
//*******************************  Configure time  ******************************************//
#define CFG_LOCAL_TIMEZONE      8      //北京时间是东8区
//*******************************  Configure loader  ******************************************//
#define  CFG_APP_RUNMODE  EN_DIRECT_RUN     //EN_DIRECT_RUN=直接从flash中运行；EN_FORM_FILE=从文件系统加载到内存运行，
#define  CFG_IBOOT_CRC   EN_NO_APP_CRC      //是否对APP程序进行CRC校验，需要极快速启动才不需要CRC校验
#define CFG_APP_FILENAME        ""              //CFG_APP_RUNMODE==EN_FORM_FILE，这里配置文件名
#define CFG_IBOOT_VERSION       "V1.0.0"        //Iboot发布版本号
#define CFG_IBOOT_UPDATE_NAME   "iboot.bin"     //iboot更新文件名，此二配置，以后改掉
#define CFG_IBOOT_UPDATE_PATH   "\\"     //iboot更新路径名，此二配置，以后改掉
//*******************************  Configure cpu_peri_hal  ******************************************//
#define CFG_STM32_HAL_TIMER     TIM6//配置HAL库使用的定时器
//*******************************  Configure NlsCharset  ******************************************//
#define CFG_LOCAL_CHARSET       "gb2312"        //
//*******************************  Configure font  ******************************************//
#define CFG_FONT_DEFAULT  CN_FONT_GB2312_SONG_16      //字体名在include/font目录中找
//*******************************  Configure ascii_font  ******************************************//
#define CFG_ASCII_8X8         0         //8*8点阵ascii字体
#define CFG_ASCII_6X12        0         //6*12点阵ascii字体
#define CFG_ASCII_8X16_SONG   1         //8*16点阵ascii字体_宋体
#define CFG_ASCII_8X16_YUAN   0         //8*16点阵ascii字体_圆体
#define CFG_ASCII_8X16_KAI    0         //8*16点阵ascii字体_楷体
#define CFG_ASCII_8X16_HEI    0         //8*16点阵ascii字体_黑体
#define CFG_ASCII_8X16_FANG   0         //8*16点阵ascii字体_仿宋
#define CFG_ASCII_12X24_SONG  0         //12*24点阵ascii字体_宋体
#define CFG_ASCII_12X24_YUAN  0         //12*24点阵ascii字体_圆体
#define CFG_ASCII_12X24_KAI   0         //12*24点阵ascii字体_楷体
#define CFG_ASCII_12X24_HEI   0         //12*24点阵ascii字体_黑体
#define CFG_ASCII_12X24_FANG  0         //12*24点阵ascii字体_仿宋
#define CFG_ASCII_16X32_YUAN  0         //16*32点阵ascii字体_圆体
#define CFG_ASCII_16X32_KAI   0         //16*32点阵ascii字体_楷体
#define CFG_ASCII_16X32_HEI   0         //16*32点阵ascii字体_黑体
#define CFG_ASCII_16X32_FANG  0         //16*32点阵ascii字体_仿宋
//*******************************  Configure fatfilesystem  ******************************************//
#define CFG_OPTIONS             0       //FAT文件系统文件安装逻辑
#define CFG_MOUNT_POINT         ""      //FAT文件系统安装目录
#define CFG_MOUNT_DEV           ""      //FAT文件系统所在设备路径
//*******************************  Configure gkernel  ******************************************//
#define CFG_GKERNEL_CMD_DEEP        1024    //gdd向gkernel传递命令的缓冲区长度（字节数）
//*******************************  Configure HmiInput  ******************************************//
#define CFG_HMIIN_DEV_LIMIT     2       //人机交互输入设备数量如键盘、鼠标等
//*******************************  Configure gdd  ******************************************//
#define     CFG_DESKTOP_WIDTH                0                 //桌面尺寸（像素数）宽度，0=显示器宽度
#define     CFG_DESKTOP_HEIGHT               0                 //桌面尺寸（像素数）高度，0=显示器高度
#define     CFG_DISPLAY_NAME                 "lcdili9325"      //须与bsp中显示器驱动模块配置的显示器名字相同
#define     CFG_DESKTOP_NAME                 "Desktop"         //
#define     CFG_INPUTDEV_NAME                "TOUCH_STMPE811"  //使用bsp中输入设备所配置的名字，多输入设备的话，每个设备间用逗号隔开
#define     CFG_DESKTOP_FORMAT               CN_SYS_PF_RGB565  //桌面窗口像素格式，常数在gkernel.h中定义，一般使用与显示器相同颜色
#define     CFG_GRAY_BASE_COLOR              CN_COLOR_WHITE    //像素格式设为灰度时才需要设置的“最亮”色，可在gkernel.h中找到常用颜色定义
#define     CFG_FILL_COLOR                   CN_COLOR_BLUE     //创建桌面时的填充色，用888格式，可在gkernel.h中找到常用颜色定义
//*******************************  Configure stmpe811  ******************************************//
#define CFG_CRT_CLK_FRE                 (100*1000)      //总线速度单位Hz
#define CFG_STMPE811_DEVADDR            0x41            //IIC总线上的设备地址
#define CFG_STMPE811_TOUCH_DEV_NAME      "TOUCH_STMPE811"     //触摸设备的名称
#define CFG_STMPE811_DESKTOP_NAME  "LCD_DESKTOP_STMPE811"     //触摸屏所在桌面的名称
//*******************************  Configure stdio  ******************************************//
#define     CFG_STDIO_RUN_MODE               (CN_STDIO_STDIN_MULTI|CN_STDIO_STDOUT_FOLLOW)//
#define     CFG_STDIO_IN_NAME                "/dev/UART2"      //
#define     CFG_STDIO_OUT_NAME               "/dev/UART2"      //
#define     CFG_STDIO_ERR_NAME               "/dev/UART2"      //
//*******************************  Configure cpu_peri_uart  ******************************************//
#define     CFG_UART1_SENDBUF_LEN            32                //
#define     CFG_UART1_RECVBUF_LEN            32                //
#define     CFG_UART1_DMABUF_LEN             32                //
#define     CFG_UART2_SENDBUF_LEN            32                //
#define     CFG_UART2_RECVBUF_LEN            32                //
#define     CFG_UART2_DMABUF_LEN             32                //
#define     CFG_UART3_SENDBUF_LEN            32                //
#define     CFG_UART3_RECVBUF_LEN            32                //
#define     CFG_UART3_DMABUF_LEN             32                //
#define     CFG_UART4_SENDBUF_LEN            32                //
#define     CFG_UART4_RECVBUF_LEN            32                //
#define     CFG_UART5_SENDBUF_LEN            32                //
#define     CFG_UART5_RECVBUF_LEN            32                //
#define     CFG_UART1_ENABLE                 false             //
#define     CFG_UART2_ENABLE                 true              //
#define     CFG_UART3_ENABLE                 false             //
#define     CFG_UART4_ENABLE                 false             //
#define     CFG_UART5_ENABLE                 false             //
//*******************************  Configure timer  ******************************************//
#define CFG_TIMER_SOUCE     1       //1=由硬件计时器提供时钟源0=由tick提供时钟源
#define CFG_TIMERS_LIMIT    5       //可创建的定时器数量（不包含图形界面的定时器）
//*******************************  Configure wdt  ******************************************//
#define CFG_WDT_LIMIT           10      //允许养狗数量
#define CFG_WDTMSG_LIMIT        3       //操作看门狗的消息队列的最大长度
//*******************************  Configure ili9325  ******************************************//
#define CFG_ILI9325_DISPLAY_NAME              "lcdili9325"    //配置液晶显示的名称
#define CFG_ILI9325_HEAP_NAME                 "extram"        //配置液晶驱动所使用的堆名称
//*******************************  Configure cpu_peri_iic  ******************************************//
#define     CFG_I2C1_BUF_LEN                 128               //
#define     CFG_I2C2_BUF_LEN                 128               //
#define     CFG_I2C1_ENABLE                  true              //
#define     CFG_I2C2_ENABLE                  true              //
//******************************* Core Clock ******************************************//
#define  CFG_CORE_MCLK                   (8*Mhz)           //主频，内核要用，必须定义

#endif
