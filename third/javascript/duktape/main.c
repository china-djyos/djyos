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
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
//author:zhangqf
//date  :下午3:49:48/2017年2月28日
//usage :
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "duktape.h"




#define JS_QUIT     "quit"
#define CN_BUFLEN   128

/* Being an embeddable engine, Duktape doesn't provide I/O
 * bindings by default.  Here's a simple one argument print()
 * function.
 */
static duk_ret_t native_print(duk_context *ctx) {
  printf("%s\n", duk_to_string(ctx, 0));
  return 0;  /* no return value (= undefined) */
}

/* Adder: add argument values. */
static duk_ret_t native_adder(duk_context *ctx) {
  int i;
  int n = duk_get_top(ctx);  /* #args */
  double res = 0.0;

  for (i = 0; i < n; i++) {
    res += duk_to_number(ctx, i);
  }

  duk_push_number(ctx, res);
  return 1;  /* one return value */
}

char *TextLine[]=
{
    "print(\'2+3=\' + adder(2, 3));",
    "print(\'Hello world from the Js\');",
    "var a = 5;",
    "var b = 6;",
    "var c = 7;",
    "print(\'a=\'+a);",
    "print(\'b=\'+b);",
    "c = a+b;"
    "print(\'c=a+b=\'+c);"
};

#define CN_TEXT_SIZE    (sizeof(TextLine)/(sizeof(char *)))

bool_t DukTapeShell(char *param)
{
    char buf[CN_BUFLEN];
    duk_context *ctx = duk_create_heap_default();
    if(NULL == ctx)
    {
        printf("%s:ctx create error\n\r",__FUNCTION__);
        goto EXIT_SHELL;
    }
    printf("js engine start!,if you want to exit,please input:%s\n\r",JS_QUIT);
    //add some function
    duk_push_c_function(ctx, native_print, 1 /*nargs*/);
    duk_put_global_string(ctx, "print");
    duk_push_c_function(ctx, native_adder, DUK_VARARGS);
    duk_put_global_string(ctx, "adder");
    //make a eval
//  duk_eval_string(ctx, "print('Hello world from the duktape!');");

    int i =0;
    for(i = 0; i <CN_TEXT_SIZE;i++)
    {
        duk_eval_string(ctx,TextLine[i]);
    }
    while(1)
    {
        memset(buf,0,CN_BUFLEN);
        gets(buf);
        if(strlen(buf) > 0)
        {
            //do the process
            if(0 == strcmp(buf,JS_QUIT))
            {
                break;
            }
            else
            {
                duk_eval_string(ctx,buf);
            }
        }
    }
    duk_destroy_heap(ctx);
    printf("js engine stop!\n\r");
EXIT_SHELL:
    return true;
}

static duk_context  *gMyCtx = NULL;

int duktest1(char *param) {

    if(NULL == gMyCtx)
    {
        gMyCtx = duk_create_heap_default();
    }
    duk_eval_string(gMyCtx, "1+2");
    printf("1+2=%d\n", (int) duk_get_int(gMyCtx, -1));
    return 0;
}

int duktest2(char *param) {
    if(NULL != gMyCtx)
    {
        duk_destroy_heap(gMyCtx);
        gMyCtx = NULL;
    }
    return 0;
}
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <os.h>
#include <shell.h>

struct shell_debug  gJsDukTapeDebug[] =
{
    {
        "duktape",
        DukTapeShell,
        "usage:duktape",
        "usage:duktape",
    },
    {
        "duktest1",
        duktest1,
        "usage:duktest1",
        "usage:duktest1",
    },
    {
        "duktest2",
        duktest2,
        "usage:duktest2",
        "usage:duktest2",
    },
};
#define CN_JsDukTapeDebug_NUM  ((sizeof(gJsDukTapeDebug))/(sizeof(struct shell_debug)))
//static struct ShellCmdRsc gJsDukTapeDebugCmdRsc[CN_JsDukTapeDebug_NUM];

// =============================================================================
// FUNCTION:this is duktape install function
// PARA  IN:
// PARA OUT:
// RETURN  :
// INSTRUCT:you could use the js in the shell
// =============================================================================
int Duktape_main(int argc, char *argv[])
{
    shell_debug_add(gJsDukTapeDebug, CN_JsDukTapeDebug_NUM);
    printf("###300KB ROM and 1KB RAM need, and the stack must be more than 10KB\n\r");
    return 0;
}



