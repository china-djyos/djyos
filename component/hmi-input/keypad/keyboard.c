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
//所属模块:键盘
//作者:  罗侍田.
//版本：V1.1.0
//文件描述: 提供键盘设备管理功能的公共部分，移植需要修改的见key_xxx.c文件
//其他说明:
//修订历史:
//3. 日期:20120324
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.按照新的stdin模块要求重写
//      2.增加key_read函数和key_read_direct函数
//2. 日期:20090131
//   作者:  罗侍田.
//   新版本号：v1.1.0
//   修改说明:
//      1.把硬件相关的函数key_scan_hard移到key_xxx.c中.
//      2.增加key_read函数和key_read_direct函数
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#include "stdint.h"
#include "keyboard.h"
#include "djyos.h"
#include "object.h"
#include "hmi-input.h"
#include "systime.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_KeyBoard(void);
//    ModuleInstall_KeyBoard();
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"keyboard"     //键盘
//parent:"HmiInput"             //填写该组件的父组件名字，none表示没有父组件
//attribute:system              //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable              //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium              //初始化时机，可选值：early，medium，later。
                                //表示初始化时间，分别是早期、中期、后期
//dependence:"HmiInput"         //该组件的依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件将强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"         //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                //选中该组件时，被依赖组件不会被强制选中，
                                //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                  //该组件的互斥组件名（可以是none，表示无互斥组件），
                                //如果与多个组件互斥，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header           //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
//%$#@num,0,100,
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***从列出的选项中选择若干个定义成宏
//%$#@free,
//%$#@end configue  ****参数配置结束
//@#$%component end configure


ptu32_t KeyBoard_Scan(void);
//----初始化键盘模块----------------------------------------------------------
//功能: 初始化键盘模块，包括:安装键盘标准输入设备；登记键盘扫描事件
//      类型；pop键盘扫描事件以启动扫描(实际扫描工作要等多线程启动后才开始)
//参数: 无
//返回: false=失败，true=成功
//----------------------------------------------------------------------------
bool_t ModuleInstall_KeyBoard(void)
{
    s16 evtt_key;
    if(!obj_search_child(obj_root(),"stdin input device"))   //标准输入设备未初始化
        return false;
    evtt_key = Djy_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                    KeyBoard_Scan,NULL,512,"keyboard");
    if(evtt_key == CN_EVTT_ID_INVALID)
    {
        return false;
    }
    if(Djy_EventPop(evtt_key,NULL,0,0,0,0)
                        == (uint16_t)CN_EVENT_ID_INVALID)
    {
        Djy_EvttUnregist(evtt_key);
        return false;
    }
    return true;
}
//----安装键盘-----------------------------------------------------------------
//功能: 由硬件驱动程序调用，用来初始化键盘输入设备，可安装多个输入设备
//参数:
//返回: 输入设备ID,-1表示失败
//-----------------------------------------------------------------------------
s32 Keyboard_InstallDevice(char *keyboard_name,struct KeyBoardPrivate *keyboard_pr)
{
    if(-1 ==HmiIn_InstallDevice(keyboard_name,EN_HMIIN_KEYBOARD,keyboard_pr))
        return -1;
    else
    {
        keyboard_pr->vtime_count = 0;
        return 0;
    }
}

//----键盘扫描任务-------------------------------------------------------------
//功能: 周期性地调用硬件扫描程序，获取用户敲击信息，合成标准键盘码，送到标准
//      输入设备中。扫描码有两种不同的类型：通码(make code)和断码(break code)。
//      当一个键被按下或持续按住时,就产生一个通码;而当一个键被释放时,就产生一个
//      断码。每个按键由两个字节组成，通码为0+键码，断码为0xF0+键码。键码值参照
//      windows的虚拟键码，如A键,通码为0x00,0x41,断码为0xF0，0x41。
//参数: 无
//返回: 无
//----------------------------------------------------------------------------
ptu32_t KeyBoard_Scan(void)
{
    struct HMI_InputDeviceObj *KeyboardObj,*StdinObj;
    struct KeyBoardPrivate *keyboard_pr;
    struct obj *ob;
    struct KeyBoardMsg key_msg;
    u32 keyvalue;
    ob = obj_search_child(obj_root(),"stdin input device");
    StdinObj = (struct HMI_InputDeviceObj *)obj_GetPrivate(ob);
    while(1)
    {
        KeyboardObj = StdinObj;
        while(1)
        {
            ob = obj_foreach_scion(StdinObj->HostObj,KeyboardObj->HostObj);
            KeyboardObj = (struct HMI_InputDeviceObj*)obj_GetPrivate(ob);
            if(KeyboardObj == NULL)
                break;
            if(KeyboardObj->input_type != EN_HMIIN_KEYBOARD)
                continue;
            keyboard_pr = (struct KeyBoardPrivate*)KeyboardObj->stdin_private;
            keyvalue = keyboard_pr->read_keyboard();
            if(keyvalue != keyboard_pr->key_now)
            {
                keyboard_pr->key_now = keyvalue;
                keyboard_pr->vtime_count = 0;
            }
            else
            {
                if(keyboard_pr->vtime_count < keyboard_pr->vtime_limit)
                    keyboard_pr->vtime_count += 10;
            }

            if((keyboard_pr->key_bak != keyboard_pr->key_now)
                && (keyboard_pr->vtime_count >= keyboard_pr->vtime_limit))
            {
                u32 loop;
                u8 key,key_byte[4];
                //特注: 此处如果使用union，将带来可移植性的问题。
                key_byte[0] = keyboard_pr->key_bak & 0xff;
                key_byte[1] = (keyboard_pr->key_bak >> 8) & 0xff;
                key_byte[2] = (keyboard_pr->key_bak >> 16) & 0xff;
                key_byte[3] = (keyboard_pr->key_bak >> 24) & 0xff;
                for(loop = 0; loop < 4; loop++)
                {
                    key = (keyboard_pr->key_now >> (loop<<3)) & 0xff;
                    if(key == 0)
                        continue;
                    //查看是不是新按下的按键
                    if((key_byte[0] != key)
                        && (key_byte[1] != key)
                        && (key_byte[2] != key)
                        && (key_byte[3] != key) )
                    {
                        key_msg.time = DjyGetSysTime();
                        key_msg.key_value[1] = 0;
                        key_msg.key_value[0] = key;
                        HmiIn_InputMsg(KeyboardObj->device_id,
                                            (u8*)&key_msg,sizeof(key_msg));
                    }
                }

                //特注: 此处如果使用union，将带来可移植性的问题。
                key_byte[0] = keyboard_pr->key_now & 0xff;
                key_byte[1] = (keyboard_pr->key_now >> 8) & 0xff;
                key_byte[2] = (keyboard_pr->key_now >> 16) & 0xff;
                key_byte[3] = (keyboard_pr->key_now >> 24) & 0xff;
                for(loop = 0; loop < 4; loop++)
                {
                    key = (u8)(keyboard_pr->key_bak >> (loop<<3)) & 0xff;
                    if(key == 0)
                        continue;
                    //查看有没有按键释放
                    if((key_byte[0] != key)
                        && (key_byte[1] != key)
                        && (key_byte[2] != key)
                        && (key_byte[3] != key) )
                    {

                        key_msg.time = DjyGetSysTime();
                        key_msg.key_value[1] = CN_BREAK_CODE;
                        key_msg.key_value[0] = key;
                        HmiIn_InputMsg(KeyboardObj->device_id,
                                            (u8*)&key_msg,sizeof(key_msg));
                    }
                }
                keyboard_pr->key_bak = keyboard_pr->key_now;
            }
        }
        Djy_EventDelay(10*mS);
    }
}

