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
#include <Iboot_info.h>

enum UpdateResult
{
    NoUpdateFile,                   //�������
    UpdateSuccess,                  //�����ɹ�
    UpdateDownLoadError,            //����ʧ�ܣ�����ʱ
    UpdateParamError,               //��������
    VerifyFail,                     //���ص��ļ�У�����
    VerifySuccess,                  //���ص��ļ�У��ͨ��
};

void *psram_malloc (u32 size);
enum UpdateResult CheckUpdate(void);	//�˺������û�ʵ��

//s32 set_upgrade_data(struct update_app_info *info);
int to_update_app(u32 app_max_size);

#ifdef __cplusplus
}
#endif

#endif /* UPGRADE_APP_H_ */
