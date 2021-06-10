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
//所属模块:标准输入--触摸屏模块
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 触摸屏设备驱动程序
//其他说明:
//修订历史:
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include <stdio.h>
#include "hmi-input.h"
#include "object.h"
#include "djyos.h"
#include "Touch.h"
#include <gdd.h>
#include <gui/gk_display.h>
#include "string.h"
#include "systime.h"

#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_Touch(void);
//    ModuleInstall_Touch();    //初始化人机界面输入模块
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"touch"        //触摸屏模块
//parent:"human machine interface"      //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later, pre-main。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"human machine interface"  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"graphical decorate development"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
#if ( CFG_MODULE_ENABLE_TOUCH == false )
//#warning  " touch  组件参数未配置，使用默认配置"
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#define CFG_MODULE_ENABLE_TOUCH    false //如果勾选了本组件，将由DIDE在project_config.h或命令行中定义为true
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

u16 Touch_BinSqrt(u32 value)
{
    u16 root;
    u16 rem;
    s32 i;
    for(root = rem = i = 0; i < 16; ++i)
    {
        int if1;
        root <<= 1;
        rem = (rem << 2) | (value >> 30);
        value <<= 2;
        if1 = (root << 1) + 1;
        if(rem >= if1)
        {
            root |= 1;
            rem -= if1;
        }
    }
    return root;
}

//----触摸屏扫描任务-----------------------------------------------------------
//功能: 周期性地调用硬件扫描程序，获取用户用户触摸信息，并把获得的信息输入到标准
//      输入缓冲区(调用函数stdin_input_msg)，注意，抬起触摸笔后，还要发送一条触
//      摸笔离开的信息
//参数: my_event，键盘服务事件类型。
//返回: 无
//----------------------------------------------------------------------------
ptu32_t Touch_Scan(void)
{
    struct HMI_InputDeviceObj *TouchObj,*StdinObj;
    struct SingleTouchPrivate *touch_pr;
    struct Object *ob;
    struct SingleTouchMsg touch_temp = {0,0,0,0,0,0,0};
    struct DisplayObj *display;

    ob = OBJ_SearchChild(OBJ_GetRoot(),"hmi input device");
    StdinObj = (struct HMI_InputDeviceObj *)OBJ_GetPrivate(ob);
    while(1)
    {
        TouchObj = StdinObj;
        while(1)
        {
            ob = OBJ_ForeachScion(StdinObj->HostObj,TouchObj->HostObj);
            if(ob == NULL)
                break;
            TouchObj = (struct HMI_InputDeviceObj*)OBJ_GetPrivate(ob);

            if(TouchObj->input_type != EN_HMIIN_SINGLE_TOUCH)
                continue;
            //注意，可能有多个触摸屏，对应多个显示器
            touch_pr = (struct SingleTouchPrivate*)TouchObj->stdin_private;
            display = touch_pr->touch_loc.display;
            if(touch_pr->read_touch(&touch_temp) != 0)
            {
                if(touch_pr->TouchStatus == CN_NO_TOUCH)   //刚刚接触，且慢发消息，等下要判一下移动
                {
                    //记下初始触摸点
                    touch_pr->touch_loc.x = touch_temp.x;
                    touch_pr->touch_loc.y = touch_temp.y;
                    touch_pr->touch_loc.z = touch_temp.z;
//                  touch_pr->touch_loc.time = DJY_GetSysTime();
                    touch_pr->TouchStatus = CN_TOUCHING;
                }
                else   //已经接触，判断是否滑动
                {
                    s32 DeltaX,DeltaY,Distance;
                    DeltaX = touch_temp.x - touch_pr->touch_loc.x;  //计算像素距离
                    DeltaY = touch_pr->touch_loc.y - touch_temp.y;  //计算像素距离，y坐标从上到下，反之。
                    Distance = Touch_BinSqrt(DeltaX*DeltaX + DeltaY*DeltaY);
                    //计算物理距离，这里假设像素是正方形的，且x和y方向等间距。
                    Distance = Distance * display->width_um / display->width;
                    touch_temp.time = DJY_GetSysTime();
                    if(Distance < 1000)     //如果小于1mm，则认为没有移动,发按下消息
                    {
                        if(touch_pr->TouchStatus != CN_GOT_TOUCH)
                        {
                            touch_pr->TouchStatus = CN_GOT_TOUCH;
                            touch_temp.MoveX = 0;
                            touch_temp.MoveY = 0;
//                            printf("touch: x = %d ,y = %d \r\n",
//                                touch_temp.x, touch_temp.y);
                            //发滑动数据=0的消息，即按下消息
                            HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp);
                        }
                    }
                    else
                    {
                        touch_pr->TouchStatus = CN_MOVING;
                        touch_pr->touch_loc.x = touch_temp.x;
                        touch_pr->touch_loc.y = touch_temp.y;
                        touch_pr->touch_loc.z = touch_temp.z;
                        touch_temp.MoveX = DeltaX;
                        touch_temp.MoveY = DeltaY;
//                        printf("move: x = %d ,y = %d ,movx = %d ,movy = %d\r\n",
//                            touch_temp.x, touch_temp.y, touch_temp.MoveX, touch_temp.MoveY);
                        //发包含滑动数据的消息，即滑动消息
                        HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp);
                        //检测到滑动，200mS内不检测，以免连续快速发滑动消息
                        DJY_EventDelay(200*mS);
                    }
                }
            }
            else
            {
               if(touch_pr->TouchStatus != CN_NO_TOUCH)   //刚刚抬起手指
                {
                    touch_pr->touch_loc.time = DJY_GetSysTime();
                    touch_pr->touch_loc.z = 0;
                    touch_pr->touch_loc.MoveX = 0;
                    touch_pr->touch_loc.MoveY = 0;
                    touch_pr->TouchStatus = CN_NO_TOUCH;
//                    printf("touch left: x = %d ,y = %d \r\n",
//                        touch_pr->touch_loc.x, touch_pr->touch_loc.y);
                    HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_pr->touch_loc);
                }
            }
        }
        DJY_EventDelay(20*mS);
    }
}


s32 Touch_InstallDevice(char *touch_name,struct SingleTouchPrivate *touch_pr)
{
    s32  ret = 0;

    ret =  HmiIn_InstallDevice(touch_name, EN_HMIIN_SINGLE_TOUCH, touch_pr);
    if(-1 == ret)
        touch_pr->TouchStatus = false;
    return ret;
}
//----初始化触摸屏模块---------------------------------------------------------
//功能: 初始化触摸屏模块，包括:安装触摸屏标准输入设备；登记触摸屏扫描事件
//      类型；pop触摸屏扫描事件以启动扫描(实际扫描工作要等多线程启动后才开始)
//参数: 无
//返回: false=失败，true=成功
//----------------------------------------------------------------------------
bool_t ModuleInstall_Touch(void)
{
    u16 touch_scan_evtt;
    if(!OBJ_SearchChild(OBJ_GetRoot( ),"hmi input device"))      //标准输入设备未初始化
        return false;
    touch_scan_evtt = DJY_EvttRegist(EN_CORRELATIVE,CFG_GUI_RUN_PRIO,0,0,
                            Touch_Scan,NULL,2048,"touch");
    if(touch_scan_evtt == CN_EVTT_ID_INVALID)
    {
        return false;
    }
    if(DJY_EventPop(touch_scan_evtt, NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        DJY_EvttUnregist(touch_scan_evtt);
        return false;
    }
    return true;
}

