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
    s8 *file_name;     //���start_addr != 0����ʾ����APP�������ļ����棬��copy��xip-app�ļ�ϵͳ��
    u8 file_name_len;
};

s32 set_upgrade_data(struct update_app_info *info);
int to_update_app(u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
