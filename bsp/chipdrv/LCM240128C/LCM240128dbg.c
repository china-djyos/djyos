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
// =============================================================================

// 文件名     ：LCM240128dbg.c
// 模块描述:  LCM240128调试信息
// 模块版本: V1.00
// 创建人员: zhb
// 创建时间: 09/03.2017
// =============================================================================
/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/
#include <assert.h>
#include "cpu_peri.h"
#include "ring.h"
#include "os.h"
#include "stdlib.h"
#include "shell.h"
#include "stdint.h"
#include "stddef.h"
#include "stdio.h"
#include "string.h"
#include <shell.h>


static bool_t lcdrst(void);
static bool_t lcdbl(char *param);
static bool_t gtav(void);
static bool_t etav(void);
static bool_t gtp(void);
static bool_t lcdsta(void);

extern void LCD_Reset(void);
extern void LCD_BackLight(bool_t OnOff);
extern void Touch_GetAdjustVal(void);
extern bool_t Touch_EraseAdjustVal(void);
extern void Touch_GetTouchPoint(void);
extern void InitLCM240128C(void);
extern u8 LCD_GetStatus(void);

//static void Sh_LCD_Reset(void)
bool_t lcdrst(void)
{
    LCD_Reset();
    InitLCM240128C( );
    return true;
}

//static void Sh_LCD_Status(void)
bool_t lcdsta(void)
{
   u8 status;
   status=LCD_GetStatus();
   printf("status:%x\r\n",status);
   return true;
}
//static bool_t Sh_LCD_BackLight(char *param)
 bool_t lcdbl(char *param)
{
    char *word_OnOff,*word_trail,*next_param;
    uint8_t byOnOff;
    //提取1个参数
    extern char *shell_inputs(char *buf,char **next);
    word_OnOff = shell_inputs(param,&next_param);
    word_trail = shell_inputs(next_param,&next_param);
    if((word_OnOff == NULL)||(word_trail != NULL))
    {
      printf("\r\n格式错误，正确格式是：\r\n>d 1:Light LCD/0:Off Light.\r\n");
      return false;
    }
    byOnOff = strtol(word_OnOff, (char **)NULL, 0);
    if(byOnOff==1)
    {
        LCD_BackLight(1);
    }
    else if(byOnOff==0)
    {
        LCD_BackLight(0);
    }
    else
    {
        printf("Para is invalid,1:Light LCD/0:Off Light.\r\n");
    }
    return true;
}
//static void Sh_Get_AdjustVal(void)
bool_t gtav(void)
{
    Touch_GetAdjustVal();
    return true;
}
//static void Sh_Earse_AdjustVal(void)
bool_t etav(void)
{
    Touch_EraseAdjustVal();
    return true;
}
//static void Sh_Get_TouchPoint(void)
bool_t gtp(void)
{
    Touch_GetTouchPoint();
    return true;
}
ADD_TO_ROUTINE_SHELL(gtp,gtp,"get the location of touch point  COMMAND:gtp+enter");
ADD_TO_ROUTINE_SHELL(etav,etav,"erase touch lcd adjust val  COMMAND:etav+enter");
ADD_TO_ROUTINE_SHELL(gtav,gtav,"get touch lcd adjust val  COMMAND:gtav+enter");
ADD_TO_ROUTINE_SHELL(lcdbl,lcdbl,"On/Off lcd back light  COMMAND:lcdbl+1/0(1:On 0:Off)+enter");
ADD_TO_ROUTINE_SHELL(lcdsta,lcdsta,"get lcd status  COMMAND:lcdsta+enter");
ADD_TO_ROUTINE_SHELL(lcdrst,lcdrst,"reset lcd  COMMAND:lcdrst+enter");
