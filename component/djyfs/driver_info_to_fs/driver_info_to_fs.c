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
#include "stdint.h"
#include "stddef.h"
#include "cpu_peri.h"
#include <dbug.h>
#include "project_config.h"     //本文件由IDE中配置界面生成，存放在APP的工程目录中。
                                //允许是个空文件，所有配置将按默认值配置。

// ============================================================================
// 功能：写文件信息，文件如果到长度上限会自动删除，如果写的信息和文件最后的信息相同则不会写入
//      写数据每次都往文件最后写。
// 参数：name -- 文件名；val -- 待写数据；len -- 长度
// 返回： 0 -- 失败； 1 -- 成功
// 备注：
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
        //这里如果用的是efs文件系统存mac信息时，当文件已经容不下这次需要写的数据时，则删除该文件，重新创建
        //CFG_EFS_FILE_SIZE_LIMIT是设置的efs文件的最大大小
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
// 功能：获取文件信息，从文件末尾开始读
// 参数：name -- 文件名；val -- 待写数据；len -- 长度
// 返回： 0 -- 失败； > 0 -- 成功,实际读取的有效数据长度
// 备注：
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
// 功能：删除文件
// 参数：name -- 文件名
// 返回： 0 -- 失败； 1 -- 成功
// 备注：
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
