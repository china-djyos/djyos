//----------------------------------------------------
// Copyright (c) 2014, SHENZHEN PENGRUI SOFT CO LTD. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。
// 3. 本软件作为都江堰操作系统的组成部分，未获事前取得的书面许可，不允许移植到非
//    都江堰操作系统环境下运行。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  zhb.
//版本：V1.0.0
//文件描述: 按钮控件实现
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2017-12-15
//   作者:  zhb.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------

#include <gdd.h>
#include <gdd_widget.h>
#include <gkernel.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "dbug.h"
#include "../../../../djysrc/bsp/arch/arm/arm32_stdint.h"

static bool_t Widget_SetFillColor(HWND hwnd,uint32_t color)
{
	SetWindowFillColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetFillColor(HWND hwnd,uint32_t *pcolor)
{
	return (GetWindowFillColor(hwnd,pcolor));
}

static bool_t Widget_SetHyalineColor(HWND hwnd,uint32_t color)
{
	SetWindowHyalineColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetHyalineColor(HWND hwnd,uint32_t *pcolor)
{

}


static bool_t Widget_SetTextColor(HWND hwnd,uint32_t color)
{
	SetWindowTextColor(hwnd,color);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_GetTextColor(HWND hwnd,uint32_t *pcolor)
{
	return (GetWindowTextColor(hwnd,pcolor));
}

static bool_t Widget_SetRopCode(HWND hwnd,struct RopGroup *ptRopCode)
{
	struct RopGroup RopCode;
	memcpy(&RopCode,ptRopCode,sizeof(struct RopGroup));
	SetWindowRopCode(hwnd,RopCode);
	InvalidateWindow(hwnd,true);
	return true;
}

static bool_t Widget_EnableHyalineColor(HWND hwnd)
{
	struct RopGroup RopCode={0, 0, 0, CN_R2_COPYPEN, 0, 1, 0};
    return (Widget_SetRopCode(hwnd,&RopCode));
}



//----设置控件属性-------------------------------------------------------------
//功能：设置控件属性
//参数：pMsg，消息指针
//返回：固定true
//---------------------------------------------------------------------------
bool_t Widget_SetAttr(HWND hwnd,uint8_t attrid,ptu32_t param)
{
	 if(hwnd==NULL)
	 {
	     debug_printf("gdd","Function:%s para invalid.\r\n",__FUNCTION__);
	    return false;
	 }
     switch(attrid)
     {
	   case ENUM_WIDGET_FILL_COLOR:
		   Widget_SetFillColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_DRAW_COLOR:
//		   Button_SetDrawColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_TEXT_COLOR:
		   Widget_SetTextColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_HYALINE_COLOR:
		   Widget_SetHyalineColor(hwnd,param);
		   break;
	   case ENUM_WIDGET_ROP_CODE:
		   Widget_SetRopCode(hwnd,(struct RopGroup *)param);
		   break;
	   case ENUM_WIDGET_ENABLE_HYALINE:
		   Widget_EnableHyalineColor(hwnd);
		   break;
	   default:
		  break;

     }
     return true;
}

bool_t Widget_GetAttr(HWND hwnd,uint8_t attrid,ptu32_t *param)
{
	 if(hwnd==NULL)
	 {
	     debug_printf("gdd","Function:%s para invalid.\r\n",__FUNCTION__);
	     return false;
	 }
	 switch(attrid)
	 {
	   case ENUM_WIDGET_FILL_COLOR:
		   Widget_GetFillColor(hwnd,(u32)param);
		   break;
	   case ENUM_WIDGET_TEXT_COLOR:
		   Widget_GetTextColor(hwnd,(u32)param);
		   break;
//	   case ENUM_WIDGET_DRAW_COLOR:
////		   Button_SetDrawColor(hwnd,param);
//		   break;
//	   case ENUM_WIDGET_HYALINE_COLOR:
//		   Widget_SetHyalineColor(hwnd,param);
//		   break;
//	   case ENUM_WIDGET_ROP_CODE:
//		   Widget_SetRopCode(hwnd,(struct RopGroup *)param);
//		   break;
//	   case ENUM_WIDGET_ENABLE_HYALINE:
//		   Widget_EnableHyalineColor(hwnd);
//		   break;
	   default:
		  break;

	 }
	 return true;
}
