/*
 * upgrade_app.c
 *
 *  Created on: 2020��12��14��
 *      Author: CK
 */

#include "app_update.h"
#include <iboot_info.h>
#include <string.h>
#include <stdlib.h>
#include <dbug.h>
#include <filesystems.h>
#include <xip.h>
#include <djyos.h>
#include <shell.h>
#include "project_config.h"

//----------------------------------------------------------------------------
//����: ׼�������������ݴ����Ĳ�����У��̼�����ȷ�ԣ����ú�APP��iboot֮��Ĺ���RAM��
//      ����Ϊ��ͬƽ̨��λ��ʽ��ͬ����λǰ��׼��������ͬ������������׼����RAM������
//      ִ�и�λ��
//      �����ļ��洢��ʽ�ܶ࣬����ֻʵ���˿�Ѱַ�ڴ���ļ����֣������洢��ʽ����APP
//      ��ʵ�֡�
//����: info�����������Ϣ��
//����: 0: �ɹ��� -1 �� ʧ��.
//ԭ���� app_update_info_to_iboot
//-----------------------------------------------------------------------------
//s32 set_upgrade_data(struct update_app_info *info)
//{
//    struct AppHead *apphead;
//    u32 file_size;
//    union update_info  up_info;
//
//    if((info->time_buf_len > sizeof(up_info.ram.production_week))
//        || (info->prod_num_buf_len > sizeof(up_info.ram.production_num)))
//    {
//        info_printf("up", "param len overlong \r\n");
//        return -1;
//    }
//
//    if(info->updsrc == CN_STORE_IN_ADDRMEM)
//    {
//        if(info->file_name_len > sizeof(up_info.ram.file_name))
//        {
//            info_printf("up", "file name len overlong \r\n");
//            return -1;
//        }
//        apphead = (struct AppHead *)info->start_addr;
//        file_size = info->end_addr - info->start_addr;  //�����ļ���С
//        if(apphead->file_size != file_size)
//        {
//            printk("ERROR: file size diff.apphead_appbinsize = %d, apphead_filesize = %d, file_size = %d!\r\n",
//                            apphead->app_bin_size, apphead->file_size, file_size);
//            return -1;
//        }
//
//        if(XIP_CheckAppInMemory(info->start_addr))
//        {
//            printf("File check correct\r\n");
//
//            memset(&up_info, 0, sizeof(up_info));
//            up_info.ram.start_add = info->start_addr;
//            up_info.ram.file_size = file_size;
//            memcpy(up_info.ram.production_week, info->time_buf, info->time_buf_len);   //����ʱ��
//            memcpy(up_info.ram.production_num, info->prod_num_buf, info->prod_num_buf_len);   //��Ʒ���
////          memcpy(up_info.ram.file_name, info->start_addr, info->file_name_len);
//
//            Iboot_SetUpdateSource(CN_STORE_IN_ADDRMEM);  //����������ʽΪ����ram��ȡapp����
//        }
//        else
//        {
//            printk("File check fail\r\n");
//            return -1;
//        }
//    }
//    else if(info->updsrc == CN_STORE_IN_FILE)
//    {
//        if(info->file_name_len > sizeof(up_info.file_path))
//        {
//            info_printf("up", "file path len overlong \r\n");
//            return -1;
//        }
//        if(XIP_CheckAppInFile((const char *)info->start_addr))
//        {
//            printk("File check correct\r\n");
//
//            memcpy(up_info.file.production_week, info->time_buf, info->time_buf_len);   //����ʱ��
//            memcpy(up_info.file.production_num, info->prod_num_buf, info->prod_num_buf_len);   //��Ʒ���
//            memcpy(up_info.file.file_path, info->start_addr, info->file_name_len);
//
//            Iboot_SetUpdateSource(CN_STORE_IN_FILE);  //����������ʽΪ����ram��ȡapp����
//        }
//        else
//        {
//            printk("File check fail\r\n");
//            return -1;
//        }
//    }
//    set_upgrade_info((char *)&up_info, sizeof(up_info));
//    Iboot_SetRunIbootAndUpdateApp();
//    Iboot_SetRunIbootFlag();
//    return 0;
//
//}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

static u16 UpdateAppEvtt = CN_EVTT_ID_INVALID;

//------------------------------------------------------------------------------
//���ܣ����û��ṩ��ota����APP�ĺ������û���ota��ȡ����󣬳������Ǵ��ڿ�Ѱַ�ڴ��У�
//      �����ļ�ϵͳ�У�������������������Ѿ��ṩ�˽����������������֮�⣬���û�
//      ʵ�֡�
//������updsrc�����·���ָʾ��ȡֵSOURCE_OTHER1��SOURCE_OTHER2
//      pdcttime,����ʱ�䣬����finger���������ֽ����+���ֽ�������ɡ�
//      pdctnum��������ţ�����finger������5�ֽ�34��������
//���أ��û��ṩ�ĺ����У�true=��ȷ���³���xip��false=ʧ�ܣ���weak�������Ƿ���false��
//------------------------------------------------------------------------------
__attribute__((weak)) bool_t UserUpdateApp(u32 updsrc,u8 *pdcttime,u8 *pdctnum)
{
    printf("appӦ���ṩ��������ʵ��\r\n");
    return false;
}
#pragma GCC diagnostic pop

//----------------------------------------------------------------------------
//����: ȥִ������app����������iboot�е���
//����: ��
//����: true: �ɹ��� false �� ʧ��.
//-----------------------------------------------------------------------------
s32 to_update_app(void)
{
    struct AppHead *p_apphead;
    struct ProductInfo *p_productinfo;

    s32 ret = false;
    u32 updsrc;
    s8 *start_addr = 0;
    u32 file_size = 0;
    s8 production_num[sizeof(p_productinfo->ProductionNumber)];
    s8 production_week[sizeof(p_productinfo->ProductionTime)];
    u8 finger[sizeof(p_productinfo->ProductionNumber) + \
                  sizeof(p_productinfo->ProductionTime) + sizeof(p_productinfo->TypeCode)];

    if (false == Iboot_GetUpdateApp())
    {
        return false;   //����Ҫ����app
    }

    read_finger_from_iboot((s8 *)finger, sizeof(finger));
    finger[sizeof(p_productinfo->TypeCode)] = 0;
    if((finger[0] != 0xff) && (strcmp((char *)finger, PRODUCT_PRODUCT_MODEL_CODE) != 0))
    {
        printk("p_productinfo type code error \r\n");
        return false;
    }       //todo����������ָ�ƿյı�־

    updsrc = Iboot_GetUpdateSource();
    if(Iboot_GetWeek(production_week) == true)  //˵��otaʱ�յ���ָ����Ϣ
    {
        Iboot_GetSerial(production_num);
    }
    else        //ʹ�ñ����ļ�������ʱ����������
    {
        //��ȡ��û����ǰ���ļ�ͷ��Ĳ�Ʒ��Ϣ
        Iboot_GetProductInfo(APP_HEAD_PRODUCTION_WEEK,(char *)production_week,sizeof(production_week));
        Iboot_GetProductInfo(APP_HEAD_PRODUCTION_NUM,(char *)production_num,sizeof(production_num));
    }
    //���ָ���Ѿ����ڣ��򲻻Ḳ�ǡ�
    if(write_finger_to_iboot(production_week,production_num) == false)
        info_printf("up", "iboot sn write fail\r\n");

    if(updsrc == CN_STORE_IN_FILE)
    {
        if(WriteAppFromFile((u8 *)production_week, (u8 *)production_num))
            ret = true;
        else
            DJY_EventDelay(100*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
    }
    else if(updsrc == CN_STORE_IN_ADDRMEM)
    {
        Iboot_GetOtaAddr(&start_addr,&file_size);
        printk("psram_update_addr   = %x,\r\n", start_addr);
        printk("file_size   = %d,\r\n",file_size);
        if(file_size <= CFG_APP_UPDATE_MAX_FILE_SIZE)
        {
            if(XIP_CheckAppInMemory(start_addr))
            {
                printk("File check success\r\n");
                printf("Start erase flash\r\n");
                if(!File_Format((const char *)CN_XIP_APP_PATH))
                {
                    printf("Erase flash succeed\r\n");
                    //�Ѳ�Ʒ��ź��ļ�ͷ��д��app�ļ�ͷ��
                    p_apphead = (struct AppHead *)start_addr;
                    p_productinfo = (struct ProductInfo *)(p_apphead+1);
                    memcpy(p_productinfo->ProductionNumber, production_num, sizeof(p_productinfo->ProductionNumber));
                    memcpy(p_productinfo->ProductionTime, production_week, sizeof(p_productinfo->ProductionTime));
                    if(WriteAppFromRam(start_addr, file_size, (s8 *)p_apphead->app_name))
                        ret = true;
                    else
                        DJY_EventDelay(100*1000);      //��ʱһ�£������������е���Ϣ�ܴ�ӡ����
                }
            }
            else
            {
                printk("File check fail\r\n");
            }
        }
        else
            printk("file size is overlong\r\n");

    }
    else        //APP�������洢��ʽ����weak��������APP�ṩʵ�ʺ����滻
    {
         //updsrc��ֵΪSOURCE_OTHER1��SOURCE_OTHER2
        ret = UserUpdateApp(updsrc, (u8 *)production_week, (u8 *)production_num);
    }

    return ret;
}


static ptu32_t update_app(void)
{
    if (false == to_update_app())
    {
        error_printf("modbus", "update app failed\n\r");
    }
    else
    {
        error_printf("modbus", "update app success.\n\r");
        error_printf("modbus", "Run the app in 3 seconds\n\r");
        DJY_EventDelay(3000*1000);
        runapp("1");
    }

    return 0;
}

static bool_t update_app_shell(char *Param)
{
    if (CN_EVTT_ID_INVALID == UpdateAppEvtt)
    {
        UpdateAppEvtt = DJY_EvttRegist(EN_CORRELATIVE,CN_PRIO_RRS,0,0,
                                update_app,NULL,0x1000, "update app");
        if (CN_EVTT_ID_INVALID == UpdateAppEvtt)
        {
            printk("update app evtt regsit fail\r\n");
            return false;
        }
    }

    if (CN_EVENT_ID_INVALID == DJY_EventPop(UpdateAppEvtt,NULL,0,0,0,0))
    {
        printk("update app event pop fail\r\n");
        return false;
    }


    return (TRUE);
}

ADD_TO_ROUTINE_SHELL(update_app,update_app_shell,"����app.");

