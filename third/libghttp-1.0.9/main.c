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
//date  :����7:42:26/2017��2��24��
//usage :
//-----------------------------------------------------------------------------
#include "ghttp.h"
#include <stdio.h>
#include <stdlib.h>
//�첽����ʵ��
void test001()
{
    char *uri = "http://www.hao123.com";
    ghttp_request *request = NULL;
    ghttp_status status;
    char *buf;
    int bytes_read;
    request = ghttp_request_new();
    if(ghttp_set_uri(request, uri) == -1)
        exit(-1);
    if(ghttp_set_type(request, ghttp_type_get) == -1)
        exit(-1);
    /* NOTE: Set async request */
    ghttp_set_sync(request, ghttp_async);
    ghttp_prepare(request);

    while(1) {
        status = ghttp_process(request);
        if(status == ghttp_error)
            break;
        /* NOTE: buf may NULL, notice it */
        buf = ghttp_get_body(request);


        bytes_read = ghttp_get_body_len(request);
        printf("%d\n %s\n",bytes_read,buf);
        if(status == ghttp_done) {
            /* NOTE: Ok, done */
            break;
        }
    }
    ghttp_clean(request);
    ghttp_request_destroy(request);
    system("pause");
}
//ͬ������ʵ��
int test002()
{
    char *uri = "http://www.hao123.com";
    ghttp_request *request = NULL;
    ghttp_status status;
    char *buf;
    int bytes_read;
    request = ghttp_request_new();
    if(ghttp_set_uri(request, uri) == -1)
        return (-1);
    if(ghttp_set_type(request, ghttp_type_get) == -1)
        return(-1);
    ghttp_prepare(request);
    status = ghttp_process(request);
    if(status == ghttp_error)
        return(-1);
    /* OK, done */
    printf("Status code -> %d\n", ghttp_status_code(request));
    buf = ghttp_get_body(request);
    printf("%s\n",buf);
    bytes_read = ghttp_get_body_len(request);
    ghttp_clean(request);
    ghttp_request_destroy(request);
    system("pause");
    return 0;
}

static u16 gIeEvttID;

int IeTask(void)
{
    char *url;
    char *uri;
    ghttp_request *request = NULL;
    ghttp_status status;
    char *buf;
    int bytes_read;

    while(1)
    {
        DJY_WaitEvttPop(gIeEvttID,NULL,CN_TIMEOUT_FOREVER);
        DJY_GetEventPara(&url,NULL);
        if(NULL == url)
        {
            goto ONECE_MORE;
        }

        uri = url;
        request = ghttp_request_new();
        if(NULL == request)
        {
            printf("%s:create request failed\n\r",__FUNCTION__);
            goto ONECE_MORE;
        }
        if(ghttp_set_uri(request, uri) == -1)
        {
            printf("%s:set uri failed\n\r",__FUNCTION__);
            goto CLEAN_REQUEST;
        }
        if(ghttp_set_type(request, ghttp_type_get) == -1)
        {
            printf("%s:set type failed\n\r",__FUNCTION__);
            goto CLEAN_REQUEST;
        }
        ghttp_prepare(request);
        status = ghttp_process(request);
        if(status == ghttp_error)
        {
            printf("%s:process failed\n\r",__FUNCTION__);
            goto CLEAN_REQUEST;
        }
        /* OK, done */
        printf("Status code -> %d\n", ghttp_status_code(request));
        bytes_read = ghttp_get_body_len(request);
        printf("BodyLen:%d\n\r",bytes_read);
        buf = ghttp_get_body(request);
        printf("%s\n",buf);

CLEAN_REQUEST:
        ghttp_clean(request);
        ghttp_request_destroy(request);
ONECE_MORE:
        printf("%s:once more!\n\r",__FUNCTION__);

    }
    return 0;
}
//
//ptu32_t IeTask(void)
//{
//
//  int result = 0;
//  result = UrlGet();
//  printk("IeResult:%d\n\r",result);
//  return 0;
//}



bool_t IeShell(char *param)
{
    if(NULL != param)
    {
        DJY_EventPop(gIeEvttID, NULL, 0, param, 0, 0);

        DJY_EventDelay(1000*mS);
    }
    return true;
}


#include <shell.h>
#include <os.h>

//usage:use this function to add the url debug to the kernel
int Ghttp_main(int argc, char *argv[])
{
    gIeEvttID = DJY_EvttRegist(EN_CORRELATIVE, 200, 0, 1,IeTask,NULL, 0X2000,"ie");
    if(gIeEvttID == CN_EVTT_ID_INVALID)
    {
        return -1;
    } 
    return 0;
}

ADD_TO_ROUTINE_SHELL (ie,IeShell,"usage:ie + http://host:port/dir?param");



