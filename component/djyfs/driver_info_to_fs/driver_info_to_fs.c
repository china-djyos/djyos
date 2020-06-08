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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <dbug.h>
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

// ============================================================================
// ���ܣ�д�ļ���Ϣ���ļ�������������޻��Զ�ɾ�������д����Ϣ���ļ�������Ϣ��ͬ�򲻻�д��
//      д����ÿ�ζ����ļ����д��
// ������name -- �ļ�����val -- ��д���ݣ�len -- ����
// ���أ� 0 -- ʧ�ܣ� 1 -- �ɹ�
// ��ע��
// ============================================================================
bool_t File_SetNameValueFs(char *name, char *val, int len)
{
    FILE *fd = NULL;
    struct stat file_state;
    u8 *read_buf = NULL;
    u8 i;
    bool_t res = false;

    read_buf = malloc(len);
    if((read_buf) && (val))
    {
//        flash_protection_op(0,FLASH_PROTECT_NONE);
        memset(&file_state, 0, sizeof(struct stat));
        memset(read_buf, 0, len);
        stat(name,&file_state);
        //��������õ���efs�ļ�ϵͳ��mac��Ϣʱ�����ļ��Ѿ��ݲ��������Ҫд������ʱ����ɾ�����ļ������´���
        //CFG_EFS_FILE_SIZE_LIMIT�����õ�efs�ļ�������С
#if CFG_MODULE_ENABLE_EASY_FILE_SYSTEM
        if((file_state.st_size + len) > CFG_EFS_FILE_SIZE_LIMIT)
        {
            if(remove(name))
                info_printf("app_flash"," file full . remove %s fail\r\n.",name);
            else
                info_printf("app_flash"," file full . remove %s success\r\n.",name);
        }
#endif
        fd = fopen(name,"a+");
        if(fd)
        {
            if(file_state.st_size > len)
                fseek(fd, file_state.st_size - len, SEEK_SET);
            else
                fseek(fd, 0, SEEK_SET);

            fread(read_buf, 1, len, fd);
            for(i = 0; i < len; i++)
            {
                if(read_buf[i] != val[i])
                {
                    if(fwrite(val, 1, len, fd) != (size_t)len)
                           error_printf("app_flash"," write file %s fail\r\n.",name);
                    else
                        res = true;
                    break;
                }
            }
            if(i == len)
                res = true;
            fclose(fd);
        }
        else
            error_printf("app_flash"," open file %s fail\r\n.",name);

//        flash_protection_op(0,FLASH_PROTECT_ALL);
//        free(read_buf);
    }
    else
        error_printf("app_flash"," memory allocation fail\r\n.");

    if(read_buf != NULL)
        free(read_buf);
    return res;
}
// ============================================================================
// ���ܣ���ȡ�ļ���Ϣ�����ļ�ĩβ��ʼ��
// ������name -- �ļ�����val -- ��д���ݣ�len -- ����
// ���أ� 0 -- ʧ�ܣ� > 0 -- �ɹ�,ʵ�ʶ�ȡ����Ч���ݳ���
// ��ע��
// ============================================================================
int File_GetNameValueFs(char *name, char *val, int len)
{
    FILE *fd = NULL;
    int res = 0;
    struct stat file_state;
    if(val)
    {
        memset(&file_state, 0, sizeof(struct stat));
        stat(name,&file_state);
//        flash_protection_op(0,FLASH_PROTECT_NONE);
        fd = fopen(name,"a+");
        if(fd)
        {
            if(file_state.st_size > len){
                fseek(fd, file_state.st_size - len, SEEK_SET);
            } else{
                fseek(fd, 0, SEEK_SET);
                len = file_state.st_size;
            }
            if(fread(val, 1, len, fd) != (size_t)len){
                error_printf("app_flash"," read  %s fail\r\n.",name);
                res = 0;
            }
            else {
                //hex_dump(name, val, len);
                res = len;
            }
            fclose(fd);
        }
        else
            error_printf("app_flash"," open file %s fail\r\n.",name);
//        flash_protection_op(0,FLASH_PROTECT_ALL);
    }
    else
        error_printf("app_flash"," memory allocation fail\r\n.");

    return res;
}
// ============================================================================
// ���ܣ�ɾ���ļ�
// ������name -- �ļ���
// ���أ� 0 -- ʧ�ܣ� 1 -- �ɹ�
// ��ע��
// ============================================================================
bool_t File_RmNameValueFs(char *name)
{
    bool_t res = false;
//    flash_protection_op(0,FLASH_PROTECT_NONE);
    if(remove(name))
        info_printf("app_flash"," remove %s fail\r\n.",name);
    else
    {
        info_printf("app_flash"," remove %s success\r\n.",name);
        res = true;
    }
//    flash_protection_op(0,FLASH_PROTECT_ALL);
    return res;
}
