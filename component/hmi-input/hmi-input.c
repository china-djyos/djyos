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
//所属模块：标准IO设备管理
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 鼠标、键盘等输入设备管理
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2011-01-24
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 创建文件
//------------------------------------------------------

#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "object.h"
#include "pool.h"
#include "hmi-input.h"
#include "msgqueue.h"
#include "djyos.h"
#include "dbug.h"
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

//@#$%component configure   ****组件配置开始，用于 DIDE 中图形化配置界面
//****配置块的语法和使用方法，参见源码根目录下的文件：component_config_readme.txt****
//%$#@initcode      ****初始化代码开始，由 DIDE 删除“//”后copy到初始化文件中
//    extern bool_t ModuleInstall_HmiIn(void);
//    ModuleInstall_HmiIn();      //初始化人机界面输入模块
//%$#@end initcode  ****初始化代码结束

//%$#@describe      ****组件描述开始
//component name:"HmiInput"               //人机界面输入接口
//parent:"none"                           //填写该组件的父组件名字，none表示没有父组件
//attribute:system                        //选填“third、system、bsp、user”，本属性用于在IDE中分组
//select:choosable                        //选填“required、choosable、none”，若填必选且需要配置参数，则IDE裁剪界面中默认勾取，
                                          //不可取消，必选且不需要配置参数的，或是不可选的，IDE裁剪界面中不显示，
//init time:medium                        //初始化时机，可选值：early，medium，later。
                                          //表示初始化时间，分别是早期、中期、后期
//dependence:"MsgQueue","gkernel"         //该组件的依赖组件名（可以是none，表示无依赖组件），
                                          //选中该组件时，被依赖组件将强制选中，
                                          //如果依赖多个组件，则依次列出，用“,”分隔
//weakdependence:"none"                   //该组件的弱依赖组件名（可以是none，表示无依赖组件），
                                          //选中该组件时，被依赖组件不会被强制选中，
                                          //如果依赖多个组件，则依次列出，用“,”分隔
//mutex:"none"                            //该组件的依赖组件名（可以是none，表示无依赖组件），
                                          //如果依赖多个组件，则依次列出，用“,”分隔
//%$#@end describe  ****组件描述结束

//%$#@configue      ****参数配置开始
//%$#@target = header                      //header = 生成头文件,cmdline = 命令行变量，DJYOS自有模块禁用
#ifndef CFG_HMIIN_DEV_LIMIT                //****检查参数是否已经配置好
#warning    HmiInput组件参数未配置，使用默认值
//%$#@num,0,100,
#define CFG_HMIIN_DEV_LIMIT     2       //"name",人机交互输入设备数量，如键盘、鼠标等
//%$#@enum,true,false,
//%$#@string,1,10,
//%$#select,        ***定义无值的宏，仅用于第三方组件
//%$#@free,
#endif
//%$#@end configue  ****参数配置结束
//@#$%component end configure

//所有未设置焦点的输入设备，默认以此为焦点
tpInputMsgQ tg_pDefaultFocusMsgQ = NULL;
struct MemCellPool *g_ptHmiInDevicePool;
struct HMI_InputDeviceObj sg_ptStdinDeviceMem[CFG_HMIIN_DEV_LIMIT];
s32 g_s32NextId = 0;       //每次安装输入设备时，以本变量为设备ID，然后本变量++
                            //删除设备，ID不收回。
static struct obj *s_ptHmiInDeviceDir;
tpInputMsgQ tg_pHmiInputMsgQ;    //  标准输入设备的消息队列

//----初始化标准输入模块-------------------------------------------------------
//功能: 初始化标准输入模块，执行初始化后，可以创建输入设备。
//参数: 无
//返回: true = 成功,false= 失败.
//-----------------------------------------------------------------------------
bool_t ModuleInstall_HmiIn(void)
{
    static struct HMI_InputDeviceObj root;
    struct HMI_InputDeviceObj *StdinDeviceMem;

    tg_pHmiInputMsgQ = HmiIn_CreatInputMsgQ(10,"StdInDev");
    if(tg_pHmiInputMsgQ == NULL)
        goto ExitMsgQ;
    s_ptHmiInDeviceDir = obj_newchild(objsys_root(), (fnObjOps)-1, 0, (ptu32_t)&root, "stdin input device");
    if(s_ptHmiInDeviceDir == NULL)
    {
        goto ExitDir;
    }
    root.HostObj = s_ptHmiInDeviceDir;
    //初始化设备控制块内存池
    StdinDeviceMem = M_Malloc(sizeof( struct HMI_InputDeviceObj),0);
    if(StdinDeviceMem == NULL)
    {
        goto ExitDir;
    }
    g_ptHmiInDevicePool = Mb_CreatePool((void*)StdinDeviceMem,
                                    CFG_HMIIN_DEV_LIMIT,
                                    sizeof(struct HMI_InputDeviceObj),
                                    0,0,
                                    "输入设备控制块内存池");
    if(s_ptHmiInDeviceDir == NULL)
    {
        goto ExitPool;
    }
    HmiIn_SetFocusDefault(tg_pHmiInputMsgQ);
    return true;

ExitPool:
    obj_del(s_ptHmiInDeviceDir);
ExitDir:
    free(StdinDeviceMem);
ExitMem:
    HmiIn_DeleteInputMsgQ(tg_pHmiInputMsgQ);
ExitMsgQ:
    return false;
}

//----安装输入设备-------------------------------------------------------------
//功能: 把输入设备登记到资源结点中
//参数: device_name，目标设备的资源名
//返回: 输入设备ID,-1表示失败
//-----------------------------------------------------------------------------
s32 HmiIn_InstallDevice(const char *device_name,enum _STDIN_INPUT_TYPE_ stdin_type,
                         void *myprivate)
{
    struct HMI_InputDeviceObj *Djy_HmiIn;

    if(obj_search_child(s_ptHmiInDeviceDir,device_name))
    {
        return -1;
    }
    else
    {
//        Djy_HmiIn = Mb_Malloc(g_ptHmiInDevicePool,0);
        Djy_HmiIn = M_Malloc(sizeof(struct HMI_InputDeviceObj),0);
        if(Djy_HmiIn != NULL)
        {
            Djy_HmiIn->HostObj = obj_newchild(s_ptHmiInDeviceDir, (fnObjOps)-1, 0, (ptu32_t)Djy_HmiIn, device_name);
            if(!Djy_HmiIn->HostObj)
            {
                Mb_Free(g_ptHmiInDevicePool, Djy_HmiIn);
                return (-1);
            }
            Djy_HmiIn->input_type = stdin_type;
            Djy_HmiIn->stdin_private = (ptu32_t)myprivate;
            Djy_HmiIn->input_counter = 0;
            Djy_HmiIn->FocusMsgQ = tg_pDefaultFocusMsgQ;
            g_s32NextId += 1;
            Djy_HmiIn->device_id = g_s32NextId;
            return Djy_HmiIn->device_id;
        }
        else
        {
            Djy_SaveLastError(EN_MEM_TRIED);
            debug_printf("hmi-input","function %s 内存不足\n\r", __FUNCTION__);
            return -1;
        }

    }
}

//----创建输入消息队列--------------------------------------------------------
//功能: 创建输入消息队列, 应用程序从队列中接收输入事件消息.
//参数: MsgNum,消息队列中可容纳的输入消息数量
//      Name, 输入消息队列的名字
//返回: 新创建的输入消息队列指针
//----------------------------------------------------------------------------
tpInputMsgQ HmiIn_CreatInputMsgQ(u32 MsgNum,const char *Name)
{
    return MsgQ_Create(MsgNum,sizeof(struct InputDeviceMsg),CN_MSGQ_TYPE_FIFO);
}


//----销毁输入消息队列--------------------------------------------------------
//功能: 销毁输入消息队列, 应用程序不再需要输入消息时,删除输入队列.
//      删除操作也将清空队列中已有的消息.
//参数: MsgNum,消息队列中可容纳的输入消息数量
//      Name, 输入消息队列的名字
//返回: 新创建的输入消息队列指针
//----------------------------------------------------------------------------
bool_t HmiIn_DeleteInputMsgQ(tpInputMsgQ InputMsgQ)
{
    return MsgQ_Delete(InputMsgQ);
}

//----设置输入焦点------------------------------------------------------------
//功能: 设置输入设备的输入焦点，即把输入消息(例如键盘击键、鼠标点击)传给哪个输入
//      消息队列
//参数: device_name，目标设备的资源名
//      FocusMsgQ，新的焦点
//返回: true =成功，false=输入设备找不到
//----------------------------------------------------------------------------
bool_t HmiIn_SetFocus(const char *device_name, tpInputMsgQ FocusMsgQ)
{
    struct HMI_InputDeviceObj *result;
    struct obj *focus;

    focus = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(focus != NULL)
    {
        result = (struct HMI_InputDeviceObj *)obj_GetPrivate(focus);
        result->FocusMsgQ = FocusMsgQ;
        return true;
    }
    else
    {
        return false;
    }
}

//----查询设备类型------------------------------------------------------------
//功能: 查询名为name的设备的类型。
//参数: device_name，目标设备的资源名
//返回: 设备的类型
//----------------------------------------------------------------------------
enum _STDIN_INPUT_TYPE_ HmiIn_CheckDevType(const char *device_name)
{
    struct HMI_InputDeviceObj *InputDev;
    struct obj *input;

    input = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(input != NULL)
    {
        InputDev = (struct HMI_InputDeviceObj *)obj_GetPrivate(input);
        return InputDev->input_type;
    }
    else
    {
        return EN_HMIIN_INVALID;
    }
}

//----设置默认输入焦点---------------------------------------------------------
//功能: 所有输入设备，都有焦点，即设备有数据输入时，给哪个事件类型发消息。未设定
//      焦点的设备，使用默认焦点，如果默认焦点是CN_INVALID_EVTT_ID，则未设置焦点
//      的设备便没有焦点
//参数: focus_evtt，默认焦点，
//返回: 无
//-----------------------------------------------------------------------------
void HmiIn_SetFocusDefault(tpInputMsgQ FocusMsgQ)
{
    tg_pDefaultFocusMsgQ = FocusMsgQ;
}

//----取默认消息队列-----------------------------------------------------------
//功能：获取默认消息队列，对于没有指定消息队列的输入设备，其输入消息将用默认消息
//      队列。
//参数：无
//返回：默认消息对哦指针。
//-----------------------------------------------------------------------------
tpInputMsgQ HmiIn_GetFocusDefault(void)
{
   return tg_pDefaultFocusMsgQ;
}

//----设置所有设备输入焦点-----------------------------------------------------
//功能: 把所有输入设备的焦点设为指定值
//参数: focus_evtt，新的焦点焦点
//返回: 无
//-----------------------------------------------------------------------------
//void HmiIn_SetFocusAll(u16 focus_evtt)
//{
//    //在这里遍历所有输入设备，即"input device"资源的子孙资源，设置他们的focus_evtt
//    struct  obj  *current,*start,*target;
//    start = &(s_ptHmiInDeviceDir->stdin_device_node);
//    current = start;
//    while((target =
//        obj_foreach_scion(start,current))!=NULL)
//    {
//        ((struct StdinDeviceRsc *)target)->focus_evtt = focus_evtt;
//        current = target;
//    }
//}

//----输入消息-----------------------------------------------------------------
//功能: 输入设备获得新的输入后(例如新的键盘击键、鼠标点击等)，调用本函数，把新的
//      消息传入系统。弹出事件，如果in_device->focus_evtt已初始化，则事件类型是
//      in_device->focus_evtt，如果未初始化，则事件类型是u16g_default_focus，
//      如果u16g_default_focus也没有初始化，则不弹出事件
//参数: in_device，目标输入设备的资源接点
//      msg_data，包含输入的数据
//      msg_size，输入消息的长度，字节数
//返回: true=成功，false=失败，一般是因为输入设备未安装。
//-----------------------------------------------------------------------------
bool_t HmiIn_InputMsg(s32 stdin_id,u8 *msg_data, u32 msg_size)
{
    struct  obj  *current;
    struct HMI_InputDeviceObj *InputDevice;
    struct InputDeviceMsg input_msg;
    tpInputMsgQ InputMsgQ;

    current = s_ptHmiInDeviceDir;

    //在资源队列中查找stdin_id对应的输入设备
    while((current = obj_foreach_child(s_ptHmiInDeviceDir, current)) != NULL)
    {
        InputDevice = (struct HMI_InputDeviceObj *)obj_GetPrivate(current);
        if(InputDevice->device_id == stdin_id)
            break;
    }
    if(current == NULL)        //未找到输入设备
        return false;

    InputMsgQ = InputDevice->FocusMsgQ;
    InputDevice->input_counter++;
    if(InputMsgQ == NULL)      //该设备的输入焦点未初始化
    {
        InputMsgQ = tg_pDefaultFocusMsgQ;      //取默认焦点(可能也未初始化)
    }

    if(InputMsgQ != NULL)
    {
        input_msg.input_data = *(union un_input_data *)msg_data;
        input_msg.input_type = InputDevice->input_type;
        input_msg.device_id = InputDevice->device_id;
        MsgQ_Send(InputMsgQ,(u8*)&input_msg,sizeof(struct InputDeviceMsg),
                    CN_TIMEOUT_FOREVER,CN_MSGQ_PRIO_NORMAL);
    }
    return true;
}

//----读取消息-----------------------------------------------------------------
//功能: 从输入消息队列中读取输入消息
//参数: InputMsgQ, 接收消息的消息队列
//      MsgBuf, 保存接收到的消息的缓冲区
//      TimeOut, 如果消息队列空, 等候消息的时限
//返回: true=成功，false=失败，一般是因为输入设备未安装。
//-----------------------------------------------------------------------------
bool_t HmiIn_ReadMsg(tpInputMsgQ InputMsgQ,
                     struct InputDeviceMsg *MsgBuf,u32 TimeOut)
{
    return MsgQ_Receive(InputMsgQ,(u8*)MsgBuf,sizeof(struct InputDeviceMsg),TimeOut);
}

//----从默认队列读取消息-------------------------------------------------------
//功能: 从默认输入消息队列中读取输入消息
//参数: MsgBuf, 保存接收到的消息的缓冲区
//      TimeOut, 如果消息队列空, 等候消息的时限
//返回: true=成功，false=失败，一般是因为输入设备未安装。
//-----------------------------------------------------------------------------
bool_t HmiIn_ReadDefaultMsg(struct InputDeviceMsg *MsgBuf,u32 TimeOut)
{
    return MsgQ_Receive(tg_pHmiInputMsgQ,(u8*)MsgBuf,sizeof(struct InputDeviceMsg),TimeOut);
}

//----卸载输入设备-------------------------------------------------------------
//功能: 把输入设备从资源接点删除，从"input device"资源的子资源中，找到
//      "device_name"资源接点，删除之，并把内存释放到pg_stdin_device_pool
//      内存池中
//参数: device_name，目标设备的资源名
//返回: true = 成功删除，0=失败。
//-----------------------------------------------------------------------------
bool_t HmiIn_UnInstallDevice(const char *device_name)
{
    struct  obj  *current;
    struct HMI_InputDeviceObj *Djy_HmiIn;
    current = obj_search_child(s_ptHmiInDeviceDir,device_name);
    if(current == NULL)
        return false;

    Djy_HmiIn = (struct HMI_InputDeviceObj *)obj_GetPrivate(current);
    if(!obj_del(current))
    {
        Mb_Free(g_ptHmiInDevicePool,Djy_HmiIn);
        return true;
    }
    else
        return false;
}

