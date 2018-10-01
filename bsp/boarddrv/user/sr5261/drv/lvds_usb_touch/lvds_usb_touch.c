//----------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 著作权由都江堰操作系统开源团队所有。著作权人保留一切权利。
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


#include "stdint.h"
#include "stdio.h"
#include "stddef.h"
#include "Touch.h"
#include "string.h"
#include "gkernel.h"
#include "shell.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern ptu32_t ModuleInstall_Touch_usb_lvds(struct GkWinObj *desktop,char *touch_dev_name);
//    ModuleInstall_Touch_usb_lvds(GK_GetDesktop(CFG_DISPLAY_NAME),CFG_INPUTDEV_NAME);
//    extern bool_t GDD_AddInputDev(const char *InputDevName);
//    GDD_AddInputDev(CFG_INPUTDEV_NAME);
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"usb_lvds"      //IIC接口的触摸屏
//parent:"touch"                 //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件               //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:later              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"gkernel","rtc","gdd","lock","touch" //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"          //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure

static struct ST_TouchAdjust tg_touch_adjust;
//触摸屏的类型 非零表示步科屏或迪文屏，0表示拓普威 如果是拓普威的屏需要在外部定义这个常量为0
__attribute__((weak))  const u8 lcd_lvds_type = 1;

extern s32 TOUCH_DirectRead(u8 *pButton, u32 *pX, u32 *pY);
extern s32 TOUCH_DeviceReady(void);
//----读取触摸点坐标-----------------------------------------------------------
//功能: 读取usb_lvds采集到的触摸点坐标，如果有多点，则平均之
//参数: touch_data，采集到的坐标
//返回: 1=触摸笔按下，0=触摸笔提起，
//-----------------------------------------------------------------------------
static ufast_t read_touch_usb_lvds(struct SingleTouchMsg *touch_data)
{
    u8 Button;u32 X; u32 Y;
    s32 tch_x,tch_y;
    if(TOUCH_DeviceReady() !=0)//USB未就绪
        return 0;
    if(TOUCH_DirectRead(&Button,&X,&Y))
    {
        if(lcd_lvds_type)
        {
            tch_x = (s32)X;//步科 迪文
            tch_y = (s32)Y;
        }
        else
        {
            tch_x = (s32)Y;//拓普威
            tch_y = (s32)X;
        }

        touch_data->z = (s32)Button;

        if(tg_touch_adjust.ratio_x != 0)
        {
            touch_data->x = ((tch_x << 16)- tg_touch_adjust.offset_x)
                                / tg_touch_adjust.ratio_x;
        }
        else
        {
            touch_data->x = tch_x;
        }
        if(tg_touch_adjust.ratio_y != 0)
        {
            touch_data->y = ((tch_y << 16)- tg_touch_adjust.offset_y)
                                / tg_touch_adjust.ratio_y;
        }
        else
        {
            touch_data->y = tch_y;
        }
        return (ufast_t)touch_data->z;
    }
    return 0;
}


//----触摸屏校准---------------------------------------------------------------
//功能: 触摸屏的尺寸可能与液晶可显示区域不完全一致，安装也可能有偏差，需要计算
//      校准系数和偏移量。为获得更高精度，使用定点小数。
//参数: display_name,本触摸屏对应的显示器名(资源名)//用静态变量存储每次开机校准一次//
//返回: 无
//-----------------------------------------------------------------------------
static void touch_ratio_adjust(struct GkWinObj *desktop)
{
    struct SingleTouchMsg touch_xyz0,touch_xyz1,touch_adjust;
    FILE *touch_init; u8 i;
    s32 limit_left,limit_top,limit_right,limit_bottom;

    printf("Wait Touch Device Ready...\n\r");
    while(1)
    {
        if(TOUCH_DeviceReady() ==0)//等待枚举完成
            break;
        Djy_EventDelay(10*mS);
    }
    printf("Touch Device Ready...!!!! \n\r");
    limit_left = desktop->limit_left;
    limit_top = desktop->limit_top;
    limit_right = desktop->limit_right;
    limit_bottom = desktop->limit_bottom;

    GK_FillWin(desktop,CN_COLOR_WHITE,0);
    GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+50,
                   "触摸屏矫正", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_DrawText(desktop,NULL,NULL,limit_left+10,limit_top+70,
                   "请准确点击十字交叉点", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_Lineto(desktop,0,20,40,20,CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_Lineto(desktop,20,0,20,40,CN_COLOR_RED,CN_R2_COPYPEN,CN_TIMEOUT_FOREVER);
    GK_SyncShow(CN_TIMEOUT_FOREVER);

    while(!read_touch_usb_lvds(&touch_adjust));    //等待触摸
    Djy_EventDelay(20*mS);                            //防抖延时
    while(!read_touch_usb_lvds(&touch_xyz0));    //记录触摸屏第一点校正值

    GK_FillWin(desktop,CN_COLOR_WHITE,0);

    GK_DrawText(desktop,NULL,NULL,limit_right-190,limit_bottom-80,
                   "触摸屏矫正", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_DrawText(desktop,NULL,NULL,limit_right-190,limit_bottom-60,
                   "再次准确点击十字交叉点", 21, CN_COLOR_BLACK, CN_R2_COPYPEN, 0);
    GK_Lineto(desktop,limit_right-40,limit_bottom-20,
                  limit_right,limit_bottom-20,
                  CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_Lineto(desktop,limit_right-20,limit_bottom-40,
                  limit_right-20,limit_bottom,
                  CN_COLOR_RED,CN_R2_COPYPEN,0);
    GK_SyncShow(CN_TIMEOUT_FOREVER);
    for(i=0;i<5;i++)
    {
        Djy_EventDelay(20*mS);
        if(read_touch_usb_lvds(&touch_adjust))//判断松手
            i=0;
    }

    while(!read_touch_usb_lvds(&touch_adjust));    //等待触摸
    Djy_EventDelay(20*mS);                            //防抖延时
    while(!read_touch_usb_lvds(&touch_xyz1));    //记录触摸屏第二点校正值

    GK_FillWin(desktop,CN_COLOR_WHITE,0);
    for(i=0;i<5;i++)
    {
        Djy_EventDelay(20*mS);
        if(read_touch_usb_lvds(&touch_adjust))//判断松手
            i=0;
    }

    tg_touch_adjust.ratio_x = ((touch_xyz1.x - touch_xyz0.x)<<16)
                    /(limit_right - limit_left -40);
    tg_touch_adjust.offset_x = (touch_xyz0.x<<16) - 20*tg_touch_adjust.ratio_x;
    tg_touch_adjust.ratio_y = ((touch_xyz1.y - touch_xyz0.y)<<16)
                    /(limit_bottom- limit_top-40);
    tg_touch_adjust.offset_y= (touch_xyz0.y<<16) - 20*tg_touch_adjust.ratio_y;
    GK_FillWin(desktop,CN_COLOR_BLUE,0);
    GK_SyncShow(CN_TIMEOUT_FOREVER);

    touch_init = fopen("/yaffs2/touch_init.dat","w+");
    if(touch_init)
        fwrite(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
    fclose(touch_init);

}

//-----------------------------------------------------------------------------
//功能:初始化触摸屏模块
//参数:
//  desktop 显示器句柄
//  touch_dev_name,本触摸屏对应的名字
//返回: true,成功;false,失败
//-----------------------------------------------------------------------------
ptu32_t ModuleInstall_Touch_usb_lvds(struct GkWinObj *desktop,char *touch_dev_name)
{
    static struct SingleTouchPrivate usb_lvds;
    FILE *touch_init;

    if((touch_init = fopen("/yaffs2/touch_init.dat","r")) != NULL)
    {
        fread(&tg_touch_adjust,sizeof(struct ST_TouchAdjust),1,touch_init);
        fclose(touch_init);
    }
    else
    {
        touch_ratio_adjust(desktop);          //屏幕校准
    }
    usb_lvds.read_touch = read_touch_usb_lvds;//读触摸点的坐标函数
    usb_lvds.touch_loc.display = NULL;        //NULL表示用默认桌面
    Touch_InstallDevice(touch_dev_name,&usb_lvds);//添加驱动到Touch

    return true;
}



