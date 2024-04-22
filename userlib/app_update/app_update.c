/*
 * upgrade_app.c
 *
 *  Created on: 2020年12月14日
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
#include "project_config.h"

//----------------------------------------------------------------------------
//功能: 准备好升级，根据传来的参数，校验固件的正确性，设置好APP和iboot之间的共享RAM，
//      但因为不同平台复位方式不同，复位前的准备工作不同，本函数这是准备好RAM，并不
//      执行复位。
//      升级文件存储方式很多，这里只实现了可寻址内存和文件两种，其他存储方式可在APP
//      中实现。
//参数: info：升级相关信息，
//返回: 0: 成功； -1 ： 失败.
//原名： app_update_info_to_iboot
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
//        file_size = info->end_addr - info->start_addr;  //计算文件大小
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
//            memcpy(up_info.ram.production_week, info->time_buf, info->time_buf_len);   //生产时间
//            memcpy(up_info.ram.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
////          memcpy(up_info.ram.file_name, info->start_addr, info->file_name_len);
//
//            Iboot_SetUpdateSource(CN_STORE_IN_ADDRMEM);  //设置升级方式为，从ram获取app数据
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
//            memcpy(up_info.file.production_week, info->time_buf, info->time_buf_len);   //生产时间
//            memcpy(up_info.file.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
//            memcpy(up_info.file.file_path, info->start_addr, info->file_name_len);
//
//            Iboot_SetUpdateSource(CN_STORE_IN_FILE);  //设置升级方式为，从ram获取app数据
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
//------------------------------------------------------------------------------
//功能：须用户提供的ota更新APP的函数，用户从ota获取代码后，常见的是存在可寻址内存中，
//      或者文件系统中，这两种情况，库里面已经提供了解决方案，除此两种之外，须用户
//      实现。
//参数：updsrc，更新方法指示，取值SOURCE_OTHER1或SOURCE_OTHER2
//      pdcttime,生产时间，根据finger规则，是两字节年份+两字节星期组成。
//      pdctnum，生产序号，根据finger规则，是5字节34进制数。
//返回：用户提供的函数中，true=正确更新程序到xip，false=失败，本weak函数总是返回false。
//------------------------------------------------------------------------------
__attribute__((weak)) bool_t UserUpdateApp(u32 updsrc,u8 *pdcttime,u8 *pdctnum)
{
    printf("app应该提供本函数的实例\r\n");
    return false;
}
#pragma GCC diagnostic pop

//----------------------------------------------------------------------------
//功能: 去执行升级app，本函数在iboot中调用
//参数: 无
//返回: true: 成功； false ： 失败.
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
        return false;   //不需要升级app
    }
    memset(finger, 0xff, sizeof(finger));
    read_finger_from_iboot((s8 *)finger, sizeof(finger));
    finger[sizeof(p_productinfo->TypeCode)] = 0;
    if((finger[0] != 0xff) && (strcmp((char *)finger, PRODUCT_PRODUCT_MODEL_CODE) != 0))
    {
        printk("p_productinfo type code error \r\n");
        return false;
    }       //todo：这里设置指纹空的标志

    memset(production_week, '*', sizeof(production_week));
    memset(production_num, '*', sizeof(production_num));
    if(Iboot_GetWeek(production_week) == true)  //说明ota时收到了指纹信息
    {
        Iboot_GetSerial(production_num);
    }
    else        //使用本地文件的生产时间和生产序号
    {
        //获取还没升级前的文件头里的产品信息
        Iboot_GetProductInfo(APP_HEAD_PRODUCTION_WEEK,(char *)production_week,sizeof(production_week));
        Iboot_GetProductInfo(APP_HEAD_PRODUCTION_NUM,(char *)production_num,sizeof(production_num));
    }
    if ((production_week[0] == '*') || (production_num[0] == '*'))
    {
        if (finger[0] != 0xff)
        {
            read_finger_from_iboot((s8 *)finger, sizeof(finger));
            memcpy(production_week, &finger[6], sizeof(production_week));
            memcpy(production_num, &finger[10], sizeof(production_num));
        }
        else
        {
            error_printf("boot","finger is error, not update .\r\n");
            return false;
        }
    }
    if (finger[0] == 0xff)
    {
        //如果指纹已经存在，则不会覆盖。
        if(write_finger_to_iboot(production_week,production_num) == false)
            info_printf("up", "iboot sn write fail\r\n");
    }

    updsrc = Iboot_GetUpdateSource();
    if(updsrc == CN_STORE_IN_FILE)
    {
        if(WriteAppFromFile((u8 *)production_week, (u8 *)production_num))
            ret = true;
        else
            DJY_EventDelay(100*1000);      //延时一下，让升级过程中的信息能打印出来
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
                    //把产品序号和文件头，写入app文件头中
                    p_apphead = (struct AppHead *)start_addr;
                    p_productinfo = (struct ProductInfo *)(p_apphead+1);
                    memcpy(p_productinfo->ProductionNumber, production_num, sizeof(p_productinfo->ProductionNumber));
                    memcpy(p_productinfo->ProductionTime, production_week, sizeof(p_productinfo->ProductionTime));
                    if(WriteAppFromRam(start_addr, file_size, (s8 *)p_apphead->app_name))
                        ret = true;
                    else
                        DJY_EventDelay(100*1000);      //延时一下，让升级过程中的信息能打印出来
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
    else        //APP用其他存储方式，用weak函数，有APP提供实际函数替换
    {
         //updsrc的值为SOURCE_OTHER1或SOURCE_OTHER2
        ret = UserUpdateApp(updsrc, (u8 *)production_week, (u8 *)production_num);
    }

    return ret;
}

