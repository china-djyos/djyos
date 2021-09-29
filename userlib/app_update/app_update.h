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
#include <Iboot_info.h>
//struct update_app_info
//{
//    enum AppStoreMedia updsrc;  //升级文件来源，其中other1/2由用户解析
//    s8 *start_addr;     //app在可寻址内存中的起始地址，或文件名地址
//    s8 *end_addr;
//    s8 *time_buf;       //生产时间
//    u8 time_buf_len;
//    s8 *prod_num_buf;   //产品序号
//    u8 prod_num_buf_len;
////    s8 *file_name;     //如果start_addr != 0，表示升级APP保存在文件里面，将copy到xip-app文件系统里
//    u8 file_name_len;
//};

//s32 set_upgrade_data(struct update_app_info *info);
int to_update_app(u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
