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
//所属模块:泛设备管理模块
//作者:  罗侍田.
//版本：V1.0.0
//文件描述:提供泛设备管理功能
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-01-04
//   作者:  罗侍田.
//   新版本号: V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include <fcntl.h>
#include <errno.h>
#include <object.h>
#include <objhandle.h>
#include <multiplex.h>
#include <stdarg.h>
// 设备模块出错代码，本enum常量从enum_drv_no_error开始依序增1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DEV_NO_ERROR, //没有错误
    EN_DRV_HANDLE_ERROR, //句柄错误
    EN_DRV_HOMONYMY, //设备重名错误
    EN_DRV_DEV_DEL, //删除设备失败
};

#define CN_DEV_NAME_LIMIT   255 // 设备名长度不能超过255字符

//常用设备控制命令（为了与旧代码兼容）
#define CN_DEV_CTRL_START           (F_DSTART)              // 启动设备，有些能控制电源的设备需要
#define CN_DEV_CTRL_STOP            (F_DSTOP)               // 启动设备，有些能控制电源的设备需要
#define CN_DEV_CTRL_SHUTDOWN        (F_DSHUTDOWN)           // 关闭电源,断电前需要特定处理的设备需要
#define CN_DEV_CTRL_SLEEP           (F_DSLEEP)              // 设备进入低功耗状态,用于电源管理
#define CN_DEV_CTRL_RESUME          (F_DRESUME)             // 设备恢复,从低功耗状态唤醒
#define CN_DEV_CTRL_CHECK           (F_DCHECK)              // 检查设备状态
#define CN_DEV_CTRL_SET_FUNC        (F_DHOOK)               // 设置有输入/输出/错误时回调函数
#define CN_DEV_CTRL_BLOCK_BUFFER    (F_DBLOCK_BUFFER)       // Write的完成条件是发送到缓冲区
#define CN_DEV_CTRL_BLOCK_COPLETE   (F_DBLOCK_COPLETE)      // Write的完成条件是传输完成

//#define CN_DEV_CTRL_USER            (CN_OBJ_CMD_USER+256)    //自定义的设备控制命令，从0x100开始
//用户定义控制命令常数，请从0x80开始，例如:
//#define CN_XXX_CTRL_MYCMD1        (CN_DEV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DEV_CTRL_USER+1)

//操纵设备时，通过这些指针，间接调用设备驱动提供的函数。
//打开设备，如设备无须打开即可工作，置空即可
typedef s32 (*fnDevOpen)(struct objhandle *hdl, u32 mode, u32 timeout);
//关闭设备，如果设备部需要关闭操作，置空即可
typedef s32 (*fnDevClose)(struct objhandle *hdl);
//返回值:成功写入的字节数。
//以串口为例，如果调用dev_WriteDevice时BlockOption = CN_BLOCK_BUFFER，则返回值表
//示函数返回时成功写入到设备缓冲区的数据量，并不确定是否已经从物理串口传输出去了。
//如果BlockOption = CN_BLOCK_COMPLETE，则能确保传输完成才返回。
typedef s32 (*fnDevWrite)(struct objhandle *hdl, u8 *data, u32 size, u32 offset, u32 timeout);
//返回值:成功读取的字节数
typedef s32 (*fnDevRead) (struct objhandle *hdl, u8 *data, u32 size, u32 offset, u32 timeout);
//返回值: CN_OBJ_CMD_UNSUPPORT 或 CN_OBJ_CMD_EXECUTED
typedef s32 (*fnDevCntl) (struct objhandle *hdl, u32 cmd, va_list *args);


s32 handle_IsBlockComplete(u32 flags);
const char *Device_GetName(struct Object *devo);
//ptu32_t dev_GetDrvTagFromObj(struct obj *devo);
struct Object *Device_Create(const char *name, fnDevOpen dopen, fnDevClose dclose,
                        fnDevWrite dwrite, fnDevRead dread, fnDevCntl dcntl,
                        ptu32_t DrvTag);
s32 dev_Delete(struct Object *dev);
s32 Device_DeleteByName(const char *name);
void Device_SetDrvTag(s32 fd,ptu32_t DrvTag);
ptu32_t Device_SetUserTag(s32 fd,ptu32_t UserTag);
ptu32_t Device_GetUserTag(s32 fd);
ptu32_t Device_GetDrvTag(s32 fd);

s32 Device_Open(const char *name, s32 flags,u32 timeout);
s32 Device_Close(s32 fd);
s32 Device_Read(s32 fd, void *buf, u32 len, u32 offset, u32 timeout);
s32 Device_Write(s32 fd, void *buf, u32 len, u32 offset, u32 timeout);
s32 Device_Ctrl(s32 fd, u32 cmd, ptu32_t data1, ptu32_t data2);
#ifdef __cplusplus
}
#endif
#endif //__DEVICE_H___
