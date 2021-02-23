/*
 * upgrade_app.h
 *
 *  Created on: 2020��12��14��
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
    s8 *start_addr;     //app�ڿ�Ѱַ�ڴ��е���ʼ��ַ��ֻҪ����Ϊ0�����ʾ��SOURCE_ADDR_MEMORY����
    s8 *end_addr;
    s8 *time_buf;       //����ʱ��
    u8 time_buf_len;
    s8 *prod_num_buf;   //��Ʒ���
    u8 prod_num_buf_len;
    s8 *file_name;     //����ļ����Ǵ������ļ�������xip-app��ʱʹ�õ��ļ�����start_addrΪ0��������ʱ����SOURCE_FILE
    u8 file_name_len;
};

bool_t app_check_from_fs(const char *path);
s32 app_update_info_to_iboot(struct update_app_info *info);
int to_update_app(s8 *target_dir, u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
