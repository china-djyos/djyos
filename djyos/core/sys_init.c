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
//所属模块: 硬件定时器
//作者:  罗侍田.
//版本：V1.0.0
//文件描述: 系统初始化文件
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2009-04-24
//   作者:  罗侍田.
//   新版本号：V1.0.0
//   修改说明: 原始版本，本文件内容原来和加载器放在一起
//------------------------------------------------------
#include "stdint.h"
#include "component_config_core.h"

extern void __InitTimeBase(void);
extern void __InitSys(void);
extern ptu32_t __InitMB(void);
extern void __StartOs(void);
extern ptu32_t __InitLock(void);
extern s32 obj_ModuleInit(void);
extern s32 handle_ModuleInit(void);
extern s32 Lock_CreateObject(void);
extern s32 Mb_CreateObject(void);
extern void Sys_ModuleInit(void);
extern void Board_Init(void);

align_type InitStack[CFG_INIT_STACK_SIZE/sizeof(align_type)] = {'d'};

//----runtime初始化-----------------------------------------------------
//功能：newlib中，构造函数的初始化被分为preinit_array和init_array两部分，后者是C++
//      全局构造函数指针数组，前者无任何说明，不知何意。两者之间，调用了_init函数，该函数
//      在gcc和newlib中均找不到具体实现，猜preinit_array和_int都是留给操作系统用的。
//      DJYOS暂时用不到它们，先空着。
//参数：无
//返回：无
//---------------------------------------------------------------------------
void _init(void)
{
}

//----runtime闭幕式-----------------------------------------------------
//功能：newlib中调用fini_array中的函数指针后，调用了_fini函数，该函数在gcc和newlib中均
//      找不到具体实现，猜是留给操作系统用的。DJYOS暂时用不到它们，先空着。
//参数：无
//返回：无
//---------------------------------------------------------------------------
void _fini(void)
{
}

//----系统启动程序-----------------------------------------------------
//功能：执行操作系统加载，模块(设备)初始化、用户初始化以及启动操作系统
//参数：无
//返回：无
//---------------------------------------------------------------------------
void Sys_Start(void)
{
//  __InitTimeBase();
    __DjyInitTick();
    __InitSys( );
    __InitMB();
    // Mb_ModuleInit函数需要创建信号量，调用ModuleInstall_Lock之前，创建信号量是允许的，
    // 但不能对其做Post和pend操作。而ModuleInstall_Lock需要创建内存池，则只能在调用
    __InitLock();
    obj_ModuleInit();
    handle_ModuleInit();          // 对象句柄体系初始化；
    Lock_CreateObject();    // lock体系；
    Mb_CreateObject();      // memory block体系；
    Board_Init();
    Sys_ModuleInit();
    // 黑客们注意,此两函数间不要企图插入什么代码,一切后果自负.
    __StartOs();
}

