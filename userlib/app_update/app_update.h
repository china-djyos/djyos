/*
 * upgrade_app.h
 *
 *  Created on: 2020年12月14日
 *      Author: CK
 */

#ifndef UPGRADE_APP_H_
#define UPGRADE_APP_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

struct update_app_info
{
    s8 *start_addr;     //app在可寻址内存中的起始地址，只要它不为0，则表示是SOURCE_ADDR_MEMORY升级
    s8 *end_addr;
    s8 *time_buf;       //生产时间
    u8 time_buf_len;
    s8 *prod_num_buf;   //产品序号
    u8 prod_num_buf_len;
    s8 *file_name;     //这个文件名是待升级文件，存在xip-app里时使用的文件名，start_addr为0，它不空时则是SOURCE_FILE
    u8 file_name_len;
};

bool_t app_check_from_fs(const char *path);
s32 app_update_info_to_iboot(struct update_app_info *info);
int to_update_app(s8 *target_dir, u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
