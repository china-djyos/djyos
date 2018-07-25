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

#include <errno.h>
#include <object.h>
#include <objfile.h>
#include <multiplex.h>

// 泛设备模块出错代码，本enum常量从enum_drv_no_error开始依序增1.
enum _DRV_ERROR_CODE_
{
    EN_DRV_NO_ERROR = CN_DEV_NO_ERROR, //没有错误
    EN_DRV_HANDLE_ERROR, //句柄错误
    EN_DRV_HOMONYMY, //设备重名错误
    EN_DRV_DEV_DEL, //删除设备失败
};

#define CN_DEV_NAME_LIMIT   255 // 设备名长度不能超过255字符

#ifndef O_DBC // 设备文件的操作逻辑，block after complete，表示写入数据时，必须等数据发送完，才能返回 (目前仅用于了UART)
 #define O_DBC                  0x00000004 // 需要保证不会与标准O_XXX发送冲突
#else
 #error "O_DBC" duplicate definition!
#endif

#ifndef O_DRAW // 裸操作，不考虑文件属性（offset不叠加）
 #define O_DRAW                 0x00000008 // 需要保证不会与标准O_XXX发送冲突
#else
 #error "O_DRAW" duplicate definition!
#endif

//常用设备控制命令
#define CN_DEV_CTRL_START           (CN_OBJ_CMD_USER+0)      // 启动设备，有些能控制电源的设备需要
#define CN_DEV_CTRL_STOP            (CN_OBJ_CMD_USER+1)      // 启动设备，有些能控制电源的设备需要
#define CN_DEV_CTRL_SHUTDOWN        (CN_OBJ_CMD_USER+2)      // 关闭电源,断电前需要特定处理的设备需要
#define CN_DEV_CTRL_SLEEP           (CN_OBJ_CMD_USER+3)      // 设备进入低功耗状态,用于电源管理
#define CN_DEV_CTRL_RESUME          (CN_OBJ_CMD_USER+4)      // 设备恢复,从低功耗状态唤醒
#define CN_DEV_CTRL_CHECK           (CN_OBJ_CMD_USER+5)      // 检查设备状态
#define CN_DEV_CTRL_SET_FUNC        (CN_OBJ_CMD_USER+6)      // 设置有输入/输出/错误时回调函数
#define CN_DEV_CTRL_BLOCK_BUFFER    (CN_OBJ_CMD_USER+7)      // Write的完成条件是发送到缓冲区
#define CN_DEV_CTRL_BLOCK_COPLETE   (CN_OBJ_CMD_USER+8)      // Write的完成条件是传输完成
#define CN_DEV_CTRL_USER            (CN_OBJ_CMD_USER+256)    //自定义的设备控制命令，从0x100开始
//用户定义控制命令常数，请从0x80开始，例如:
//#define CN_XXX_CTRL_MYCMD1        (CN_DEV_CTRL_USER+0)
//#define CN_XXX_CTRL_MYCMD2        (CN_DEV_CTRL_USER+1)

//操纵设备时，通过这些指针，间接调用设备驱动提供的函数。

//打开设备，如设备无须打开即可工作，置空即可
typedef s32 (*fntDevOpen)(tagOFile *of, u32 mode, u32 timeout);
//关闭设备，如果设备部需要关闭操作，置空即可
typedef s32 (*fntDevClose)(tagOFile *of);
//返回值:成功写入的字节数。
//以串口为例，如果调用dev_WriteDevice时BlockOption = CN_BLOCK_BUFFER，则返回值表
//示函数返回时成功写入到设备缓冲区的数据量，并不确定是否已经从物理串口传输出去了。
//如果BlockOption = CN_BLOCK_COMPLETE，则能确保传输完成才返回。
//这个特性对485通信特别有用，因为RS485需要控制收发切换，
typedef s32 (*fntDevWrite)(tagOFile *of, u8 *data, u32 size, u32 offset, u32 timeout);
//返回值:成功读取的字节数
typedef s32 (*fntDevRead) (tagOFile *of, u8 *data, u32 size, u32 offset, u32 timeout);
//返回值:收到不支持的命令，返回-1，0表示成功执行，其他返回值的含义自定
typedef s32 (*fntDevCntl) (tagOFile *of, u32 cmd, ptu32_t data1, ptu32_t data2);

s32            DevOpen(const char *name, s32 flags,u32 timeout);
s32            DevClose(s32 handle);
s32            DevRead(s32 handle, void *buf, u32 len, u32 offset, u32 timeout);
s32            DevWrite(s32 handle, void *buf, u32 len, u32 offset, u32 timeout);
s32            DevCntl(s32 handle, u32 cmd, ptu32_t data1, ptu32_t data2);

s32            israw(tagOFile *pOF);
s32            isbc(u32 flags);
const char    *dev_nameo(struct Object *devo);
struct Object *dev_grpaddo(char *name);
s32            dev_grpadd(char *name);
s32            dev_grpdelo(struct Object *grp);
s32            dev_grpdel(char *name);
struct Object *dev_addo(struct Object *grp, const char *name,
                        fntDevOpen dopen, fntDevClose dclose,
                        fntDevWrite dwrite, fntDevRead dread,
                        fntDevCntl dcntl, ptu32_t dtag);
s32            dev_add(const char *grp, const char *name, fntDevOpen dopen, fntDevClose dclose,
                       fntDevWrite dwrite, fntDevRead dread,
                       fntDevCntl dcntl, ptu32_t dtag);
s32            dev_delo(struct Object *devo);
s32            dev_del(const char *grp, const char *name);
ptu32_t        devfileutag(tagOFile *of);
ptu32_t        devfiledtag(tagOFile *of);

#ifdef __cplusplus
}
#endif
#endif //__DEVICE_H___
