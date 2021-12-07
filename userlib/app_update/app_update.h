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

enum UpdateResult
{
    NoUpdateFile,                   //无须更新
    UpdateSuccess,                  //升级成功
    UpdateDownLoadError,            //下载失败，含超时
    UpdateParamError,               //参数错误
    VerifyFail,                     //下载的文件校验错误
    VerifySuccess,                  //下载的文件校验通过
};

void *psram_malloc (u32 size);
enum UpdateResult CheckUpdate(void);	//此函数由用户实现

//s32 set_upgrade_data(struct update_app_info *info);
int to_update_app(u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
