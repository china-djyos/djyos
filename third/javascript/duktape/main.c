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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
//author:zhangqf
//date  :����3:49:48/2017��2��28��
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



