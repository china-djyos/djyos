//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

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
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
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
// 无论成因或任何责任主体、无论此责任为因合约关系、无过失责任主体或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------

//所属模块: GDD
//作者:  lst.
//版本：V1.0.0
//文件描述: 进度条(progressbar)相关定义
//其他说明:
//修订历史:
//2. ...
//1. 日期: 2014-11-10
//   作者:  lst.
//   新版本号：V1.0.0
//   修改说明: 原始版本
//------------------------------------------------------
#ifndef __GDD_PROGRESSBAR_H__
#define __GDD_PROGRESSBAR_H__

#if __cplusplus
extern "C" {
#endif
#include    "gdd.h"

//进度条Flag
#define PBF_ORG_LEFT    (0<<0)              //未用到
#define PBF_ORG_RIGHT   (1<<0)              //ProgressBar_Paint中用到
#define PBF_ORG_TOP     PBF_ORG_LEFT        //未用到
#define PBF_ORG_BOTTOM  PBF_ORG_RIGHT       //ProgressBar_Paint中用到
#define PBF_SHOWTEXT    (1<<1)
//进度条数据结构
typedef struct
{
    u32 Flag;//进度条风格，2bit used
    u32 Range;//进度条范围
    u32 Pos;  //是否移动及移动方向
    u32 FGColor;//进度条颜色
    u32 BGColor;//进度条背景色
    u32 TextColor;//字体颜色
    u32 DrawTextFlag;//字体风格
    char * text;//进度条显示文本

}PROGRESSBAR_DATA;


// 进度条风格，使用struct WINDOW结构Style的低16位，高16位用于窗口公共风格。
#define PBS_HOR     (0<<0)  //水平进度条
#define PBS_VER     (1<<0)  //垂直进度条
#define PBS_FLAT    (1<<1)  //平面风格

//// 进度条模式
typedef enum{
    PBM_LEFT = 0,   //起点在左边
    PBM_RIGHT = 1,  //起点在右边
    PBM_TOP = 2,    //起点在上边
    PBM_BOTTOM = 3, //起点在底边

}EN_PB_MODE;

HWND Widget_CreateProgressBar(  const char *Text,u32 Style,
                    s32 x,s32 y,s32 w,s32 h,
                    HWND hParent,u32 WinId,ptu32_t pdata,
                    struct MsgTableLink *UserMsgTableLink);

bool_t Widget_MakeProgressRect(RECT *dst,const RECT *src,u32 Range,u32 Val,EN_PB_MODE mode);
#if __cplusplus
}
#endif
#endif  /*__GDD_PROGRESSBAR_H__*/

