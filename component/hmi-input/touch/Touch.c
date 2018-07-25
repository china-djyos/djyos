//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

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
// Copyright (c) 2014 著作权由深圳鹏瑞软件有限公司所有。著作权人保留一切权利。
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
#include "hmi-input.h"
#include "object.h"
#include "djyos.h"
#include "Touch.h"
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
//parent:"HmiInput"             //填写该组件的父组件名字，none表示没有父组件
//attribute:核心组件             //选填“第三方组件、核心组件、bsp组件、用户组件”，本属性用于在IDE中分组
//select:可选                   //选填“必选、可选、不可选”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//grade:init                    //初始化时机，可选值：none，init，main。none表示无须初始化，
                                //init表示在调用main之前，main表示在main函数中初始化
//dependence:"HmiInput"         //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


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
    struct SingleTouchMsg touch_temp = {0,0,0,0,0};

    StdinObj = (struct HMI_InputDeviceObj *)OBJ_SearchChild(OBJ_Root(),"stdin input device");
    while(1)
    {
        TouchObj = StdinObj;
        while(1)
        {
            TouchObj = (struct HMI_InputDeviceObj*)
                            OBJ_TraveScion(StdinObj->HostObj,TouchObj->HostObj);
            if(TouchObj == NULL)
                break;
            if(TouchObj->input_type != EN_HMIIN_SINGLE_TOUCH)
                continue;
            touch_pr = (struct SingleTouchPrivate*)TouchObj->stdin_private;
            if(touch_pr->read_touch(&touch_temp) != 0)
            {
                if((touch_temp.x != touch_pr->touch_loc.x)
                    ||(touch_temp.y != touch_pr->touch_loc.y)
                    ||(touch_temp.z != touch_pr->touch_loc.z))
                {
                    touch_temp.time = DjyGetSysTimeBase();
                    HmiIn_InputMsg(TouchObj->device_id,(u8*)&touch_temp,
                                                        sizeof(touch_temp));
                    touch_pr->touch_loc = touch_temp;
                    touch_pr->status = true;
                }
            }
            else
            {
                if(touch_pr->status == true)
                {
                    touch_pr->touch_loc.time = DjyGetSysTimeBase();
                    touch_pr->touch_loc.z = 0;
                    HmiIn_InputMsg(TouchObj->device_id,
                                    (u8*)&touch_pr->touch_loc,
                                    sizeof(touch_pr->touch_loc));
                    touch_pr->status = false;
                }
            }
        }
        Djy_EventDelay(10*mS);
    }
}


s32 Touch_InstallDevice(char *touch_name,struct SingleTouchPrivate *touch_pr)
{
    s32  ret = 0;
    ret =  HmiIn_InstallDevice(touch_name, EN_HMIIN_SINGLE_TOUCH, touch_pr);
    if(-1 == ret)
        touch_pr->status = false;
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
    s16 touch_scan_evtt;
    if(!OBJ_SearchChild(OBJ_Root( ),"stdin input device"))      //标准输入设备未初始化
        return false;
    touch_scan_evtt = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_REAL,0,0,
                            Touch_Scan,NULL,2048,"touch");
    if(touch_scan_evtt == CN_EVTT_ID_INVALID)
    {
        return false;
    }
    if(Djy_EventPop(touch_scan_evtt, NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(touch_scan_evtt);
        return false;
    }
    return true;
}

