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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// MenuInitGeneral.h
// 模块描述:
// 模块版本:
// 创建时间: 2012-12-31
// 作者    : Administrator
// ========================================================================================================
// 程序修改记录(最新的放在最前面):
// <版本号> <修改日期>, <修改人员>: <修改功能概述>
// =========================================================================================================

#ifndef MENUINITGENERAL_H_
#define MENUINITGENERAL_H_


#include "MenuDisplay.h"

//应用程序应该调用此函数实现菜单的初始化并显示
struct menu_displaypara* menuappinit_general(\
        struct menu_item *menuitem,struct GkWinObj *Appwin, u8 mydismode);

//菜单显示退出
bool_t menu_exit(struct menu_displaypara *dispara);

//执行显示参数的显示函数指针
void   menu_display(struct menu_displaypara *dispara);


//执行显示参数的外设相应指针函数，一般的是具体外设,其实外设通用函数已经给外设提供了
//操作菜单项的接口，因此后续考虑删除这样的具体外设，和外设的耦合性太大了，外设应该只
//使用通用外设接口即可。--TODO
struct menu_displaypara*  menuperipheral_respond(\
        struct menu_displaypara *operatingDispara, void* para );

//执行显示参数的通用外设函数，para是具体的命令参见"MenuDisplay.h"
struct menu_displaypara*  menupeiripheralrespondgeneral(\
        struct menu_displaypara *operatingDispara, void* para );


struct menu_displaypara*  menukeyboard_respond(\
        struct menu_displaypara *operatingDispara, void* para );

struct menu_displaypara*  menumousetouch_respond(\
        struct menu_displaypara *operatingDispara, void* para );

#endif /* MENUINITGENERAL_H_ */
