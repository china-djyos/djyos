//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

// 文件名     ：ft5406.c
// 模块描述: 触摸屏芯片ft5406的驱动
// 模块版本: V1.00
// 创建人员: chj
// 创建时间: 05/15.2018
// =============================================================================
#include "ft5406.h"
#include "iicbus.h"
#include "systime.h"
#include "Touch.h"
#include "board.h"
#include "stdio.h"

#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//  extern bool_t ModuleInstall_FT5406(void);
//  ModuleInstall_FT5406();
//#if(CFG_MODULE_ENABLE_GRAPHICAL_DECORATE_DEVELOPMENT == true)
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_FT5406_TOUCH_NAME);
//#endif
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"touchscreen ft5406"//iic接口的触摸屏
//parent:"none"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:bsp                 //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later               //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"lock","iicbus","graphical decorate development","file system","graphical kernel","touch"//该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TOUCHSCREEN_FT5406 == false )
//#warning  " touchscreen_ft5406  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TOUCHSCREEN_FT5406    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0x30,0x3F,
#define CFG_FT5406_RT_I2C_ADDRESS   0x38                //"设备地址",硬件配置IIC设备地址
//%$#@enum,0x38,
//%$#@enum,100000,400000
#define CFG_FT5406_RT_I2C_CLK_FRE   (100*1000)           //"总线速度",单位Hz

//%$#@string,1,16,
#define CFG_FT5406_BUS_NAME          "I2C0"             //"IIC总线名称",触摸芯片使用的IIC总线名称
#define CFG_FT5406_TOUCH_NAME        "TOUCH_FT5406"     //"触摸屏名称",触摸芯片的名称
#define CFG_FT5406_DISPLAY_NAME      "desktop"          //"桌面名称",触摸屏所使用的桌面的名称
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

/*! @brief FT5406_RT I2C address. */
//#define CFG_FT5406_RT_I2C_ADDRESS (0x38)

/*! @brief FT5406_RT maximum number of simultaneously detected touches. */
#define FT5406_RT_MAX_TOUCHES (5U)

/*! @brief FT5406_RT register address where touch data begin. */
#define FT5406_RT_TOUCH_DATA_SUBADDR (1)

/*! @brief FT5406_RT raw touch data length. */
#define FT5406_RT_TOUCH_DATA_LEN (0x20)

//FT5406 部分寄存器定义
#define FT_DEVIDE_MODE          0x00//FT5406模式控制寄存器
#define FT_REG_NUM_FINGER       0x02//触摸状态寄存器

#define FT_TP1_REG              0X03//第一个触摸点数据地址
#define FT_TP2_REG              0X09//第二个触摸点数据地址
#define FT_TP3_REG              0X0F//第三个触摸点数据地址
#define FT_TP4_REG              0X15//第四个触摸点数据地址
#define FT_TP5_REG              0X1B//第五个触摸点数据地址


#define FT_ID_G_LIB_VERSION     0xA1//版本
#define FT_ID_G_MODE            0xA4//FT5206中断模式控制寄存器
#define FT_ID_G_THGROUP         0x80//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE    0x88//激活状态周期设置寄存器

#define TOUCH_POINT_GET_EVENT(T) ((touch_event_t)((T).XH >> 6))
#define TOUCH_POINT_GET_ID(T) ((T).YH >> 4)
#define TOUCH_POINT_GET_X(T) ((((T).XH & 0x0f) << 8) | (T).XL)
#define TOUCH_POINT_GET_Y(T) ((((T).YH & 0x0f) << 8) | (T).YL)

typedef struct _ft5406_touch_point
{
    uint8_t XH;
    uint8_t XL;
    uint8_t YH;
    uint8_t YL;
    uint8_t RESERVED[2];
} ft5406_touch_point_t;

typedef struct _ft5406_touch_data
{
    uint8_t DEVIDE_MODE;
    uint8_t GEST_ID;
    uint8_t TD_STATUS;
    ft5406_touch_point_t TOUCH;
} ft5406_touch_data_t;

typedef enum _touch_event
{
    kTouch_Down = 0,    /*!< The state changed to touched. */
    kTouch_Up = 1,      /*!< The state changed to not touched. */
    kTouch_Contact = 2, /*!< There is a continuous touch being detected. */
    kTouch_Reserved = 3 /*!< No touch information available. */
} touch_event_t;

//定义IICBUS架构下的IIC设备结构
static struct IIC_Device *ps_FT5406_Dev = NULL;
static u32 s_FT5406_Timeout = CN_TIMEOUT_FOREVER;
static struct SingleTouchPrivate FT5406;
static struct ST_TouchAdjust tg_touch_adjust;

__attribute__((weak))  void Board_FT5406_RST(void)
{
    return;
}

__attribute__((weak))  void Board_FT5406_Int_Gpio(void)
{
    return;
}

// =============================================================================
// 功能：FT5406芯片初始化，
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
static bool_t FT5406_Init(void)
{
    u8 data = 0;
    u8 id=0;
    s32 result=0;
    ft5406_touch_data_t touch_data;

    Board_FT5406_Int_Gpio();
    Board_FT5406_RST();

    result=IIC_Write(ps_FT5406_Dev,FT_DEVIDE_MODE,&data,1,true,s_FT5406_Timeout);//进入正常操作模式
    if(result!=1)
    {
        goto FT5406_INIT_FALSE;
    }
    else
    {
        result=IIC_Read(ps_FT5406_Dev,data,(u8*)&touch_data,sizeof(touch_data),s_FT5406_Timeout);
        if(result!=sizeof(touch_data))
            goto FT5406_INIT_FALSE;

        id=TOUCH_POINT_GET_ID(touch_data.TOUCH);
        printf("Touch FT5406 Init success,id:%02X",id);
        result = *(&result);    //消除警告
        return true;
    }

FT5406_INIT_FALSE:
    printf("Touch FT5406 Init ERROR.FILE:%s,LINE:%d\r\n",__FILE__,__LINE__);
    return false;
}

//扫描触摸屏(采用查询方式)
//参数:touch_data：,存储XYZ值
//返回值:是否有触摸
// 1=触摸笔按下，0 触摸笔提起
static ufast_t FT5406_Scan(struct SingleTouchMsg *touch_data)
{
    ft5406_touch_data_t data;
    touch_event_t touch_event;
    s32 result=0;
    u32 io_s=1;
    u32 touch_x=0,touch_y=0;

    io_s = GPIO_ReadPinInput(FT5406_INT_GPIO, FT5406_INT_GPIO_PIN);
    if(io_s==0)
    {
        result=IIC_Read(ps_FT5406_Dev,0x00,(u8*)&data,sizeof(data),s_FT5406_Timeout);
        if(result!=sizeof(data))
            return  0;

        touch_x = TOUCH_POINT_GET_X(data.TOUCH);
        touch_y = TOUCH_POINT_GET_Y(data.TOUCH);

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->y = ((touch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }
        else
        {
            touch_data->y = touch_x;
        }

        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->x = ((touch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        else
        {
            touch_data->x = touch_y;
        }

        touch_data->z = 1;

        touch_event = TOUCH_POINT_GET_EVENT(data.TOUCH);
        if(touch_event==kTouch_Down)
            return 1;
    }
    return 0;
}

//----触摸屏模块校准---------------------------------------------------------
//功能: 校准触摸屏模块，并把结果存起来
//参数: desktop：当前桌面
//返回: 0=失败，1=成功
//----------------------------------------------------------------------------
bool_t Touch_Adjust(struct GkWinObj *desktop,struct ST_TouchAdjust* touch_adjust)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1;
    FILE *touch_init;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    if((touch_init = fopen("/yaffs2/touch_init.dat","r")) != NULL)
    {

        fread(touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    }
    else
    {
        limit_left = desktop->limit_left;
        limit_top = desktop->limit_top;
        limit_right = desktop->limit_right;
        limit_bottom = desktop->limit_bottom;

        GK_FillWin(desktop,CN_COLOR_WHITE,0);

        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "触摸屏矫正", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "请准确点击十字交叉点", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(FT5406_Scan(&touch_xyz0)!=1);//等待触摸
        printf("采集坐标1:(%d,%d)\n\r",touch_xyz0.x,touch_xyz0.y);

        GK_FillWin(desktop,CN_COLOR_WHITE,0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                       "触摸屏矫正", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                       "再次准确点击十字交叉点", 21, CN_COLOR_RED, CN_R2_COPYPEN, 0);
        GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                      limit_right,limit_bottom-20,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                      limit_right-20,limit_bottom,
                      CN_COLOR_RED,CN_R2_COPYPEN,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        while(FT5406_Scan(&touch_xyz1)!=1);//等待触摸
        printf("采集坐标2:(%d,%d)\n\r",touch_xyz1.x,touch_xyz1.y);

        GK_FillWin(desktop,CN_COLOR_GREEN,0);
        touch_adjust->ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                        /(limit_right - limit_left -40);
        touch_adjust->offset_x = (touch_xyz0.x<<16) - 20*touch_adjust->ratio_x;
        touch_adjust->ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                        /(limit_bottom- limit_top-40);
        touch_adjust->offset_y= (touch_xyz0.y<<16) - 20*touch_adjust->ratio_y;
        GK_FillWin(desktop,CN_COLOR_BLUE,0);
        GK_SyncShow(CN_TIMEOUT_FOREVER);

        touch_init = fopen("/yaffs2/touch_init.dat","w+");
        if(touch_init)
            fwrite(touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        return true;
    }
    return false;
}

// =============================================================================
// 功能：FT5406模块初始化，
// 参数：无
// 返回：true,成功;false,失败
// =============================================================================
bool_t ModuleInstall_FT5406(void)
{
    struct GkWinObj *desktop;
    bool_t result = false;
    static struct IIC_Device* s_FT5406_Dev;

    //添加FT5406到IIC总线
    s_FT5406_Dev = IIC_DevAdd(CFG_FT5406_BUS_NAME,"IIC_Dev_FT5406",CFG_FT5406_RT_I2C_ADDRESS,0,8);
    if(NULL != s_FT5406_Dev)
    {
        desktop = GK_GetDesktop(CFG_FT5406_DISPLAY_NAME);
        ps_FT5406_Dev = s_FT5406_Dev;
        result=FT5406_Init();
        if(!result)
            return false;

        result=Touch_Adjust(desktop,&tg_touch_adjust);
        if(!result)
            return false;

        FT5406.read_touch = FT5406_Scan;//读触摸点的坐标函数
        FT5406.touch_loc.display = GK_GetDisplay(CFG_FT5406_DISPLAY_NAME);
        result=Touch_InstallDevice(CFG_FT5406_TOUCH_NAME,&FT5406);//添加驱动到Touch
        if(!result)
            return false;

        result = true;
    }

    return result;
}

