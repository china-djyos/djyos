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
#include <cpu_peri_flash.h>

//----------------------------------------------------------------------------
//功能: 准备好升级，根据传来的参数，校验固件的正确性，设置好APP和iboot之间的共享RAM，
//      但因为不同平台复位方式不同，复位前的准备工作不同，本函数这是准备好RAM，并不
//      执行复位。
//参数: info：升级相关信息，
//返回: 0: 成功； -1 ： 失败.
//原名： app_update_info_to_iboot
//-----------------------------------------------------------------------------
s32 set_upgrade_data(struct update_app_info *info)
{
    struct AppHead *apphead;
    u32 file_size;
    union update_info  up_info;

    if((info->time_buf_len > sizeof(up_info.ram.production_time))
        || (info->prod_num_buf_len > sizeof(up_info.ram.production_num)))
    {
        info_printf("up", "param len overlong \r\n");
        return -1;
    }

    if(info->start_addr != NULL)       //看是否有给出起始地址，有则是从内存里升级，无则不是
    {
        if(info->file_name_len > sizeof(up_info.ram.file_name))
        {
            info_printf("up", "file name len overlong \r\n");
            return -1;
        }
        apphead = (struct AppHead *)info->start_addr;
        file_size = info->end_addr - info->start_addr;  //计算文件大小
        if(apphead->file_size != file_size)
        {
            printk("ERROR: file size diff.apphead_appbinsize = %d, apphead_filesize = %d, file_size = %d!\r\n",
                            apphead->app_bin_size, apphead->file_size, file_size);
            return -1;
        }

        if(XIP_CheckAppInMemory(info->start_addr))
        {
            printf("File check correct\r\n");

            memset(&up_info, 0, sizeof(up_info));
            up_info.ram.start_add = info->start_addr;
            up_info.ram.file_size = file_size;
            memcpy(up_info.ram.production_time, info->time_buf, info->time_buf_len);   //生产时间
            memcpy(up_info.ram.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
            memcpy(up_info.ram.file_name, info->file_name, info->file_name_len);

            Iboot_SetUpdateSource(SOURCE_ADDR_MEMORY);  //设置升级方式为，从ram获取app数据
        }
        else
        {
            printk("File check fail\r\n");
            return -1;
        }
    }
    else if(info->file_name != 0)   //如果没给出start_addr，但是给出了file_name 则是从文件系统升级
    {
        if(info->file_name_len > sizeof(up_info.file.file_path))
        {
            info_printf("up", "file path len overlong \r\n");
            return -1;
        }
        if(XIP_CheckAppInFile((const char *)info->file_name))
        {
            printk("File check correct\r\n");

            memcpy(up_info.file.production_time, info->time_buf, info->time_buf_len);   //生产时间
            memcpy(up_info.file.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
            memcpy(up_info.file.file_path, info->file_name, info->file_name_len);

            Iboot_SetUpdateSource(SOURCE_FILE);  //设置升级方式为，从ram获取app数据
        }
        else
        {
            printk("File check fail\r\n");
            return -1;
        }
    }
    set_upgrade_info((char *)&up_info, sizeof(up_info));
    Iboot_SetRunIbootUpdateApp();
    Set_RunIbootFlag();
    return 0;

}


//----------------------------------------------------------------------------
//功能: 把app从可寻址内存写入到xip 文件系统中
//参数: dir：待写入的目录，一般是/xpi-app，
//      addr：内存起始地址，
//      file_size：文件大小，
//      file_name：文件名
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
static bool_t app_ram_to_fs(s8 *dir, s8 *addr,u32 file_size, s8 *file_name)
{
    bool_t ret = false;
    FILE *xipapp = 0;
    u32 file_name_led,res, residue_size = (s32)file_size;
    char *xipapppath;
    char percentage_last = 0, percentage = 0;
    file_name_led = strlen((const char *)file_name)+ strlen((const char *)dir) + 2;
    xipapppath = (char *)malloc(file_name_led);
    if(xipapppath == NULL)
        return ret;
    sprintf(xipapppath, "%s/%s", dir, file_name);
    xipapp = fopen(xipapppath, "w+");
    if(xipapp)
    {
        printf("open file success   %s ,\r\n", xipapppath);
        printf("\r\nloading       ");
        while(1)
        {
            percentage = 100 - ((char)((residue_size * 100)/ file_size));
            if(percentage != percentage_last)
            {
                printf("\b\b\b%2d%%",percentage);
                percentage_last = percentage;
            }
            if(residue_size > 1024)
            {
                res = fwrite(addr, 1, 1024, xipapp);
                if(res != 1024)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            else
            {
                res = fwrite(addr, 1, residue_size, xipapp);
                if(res != residue_size)
                    printf("write file xip-app error  \r\n ");
                addr += res;
                residue_size -= res;
            }
            if(residue_size == 0)
            {
                info_printf("IAP","app update success.\r\n");
                ret = true;
                break;
            }
            if((s32)residue_size < 0)
            {
                info_printf("IAP","app update fail.\r\n");
                break;
            }
        }
        fclose(xipapp);
        free(xipapppath);
    }
    else
        printf("open file fail,\r\n");

    return ret;
}

//----------------------------------------------------------------------------
//功能: 去执行升级app
//参数: app_max_size：app文件的最大大小
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
s32 to_update_app(u32 app_max_size)
{
    s32 ret = false;
    union update_info  up_info;
    s8 file_name[sizeof(up_info.ram.file_name)];
    struct ProductInfo p_productinfo;
    s8 *start_addr = 0;
//    u8 app_info_offset = 0;
    u32 file_size = 0;
//    u32 iboot_sn_addr = 0;
    struct stat file_state;
    char production_num[sizeof(p_productinfo.ProductionNumber)];
    char production_time[sizeof(p_productinfo.ProductionTime)];
    u8 finger[sizeof(p_productinfo.ProductionNumber) + \
                  sizeof(p_productinfo.ProductionTime) + sizeof(p_productinfo.TypeCode)];
    if(Iboot_GetUpdateApp() == true)        //todo,此判断可以去掉
    {
        if(!Iboot_GetMutualUpdatePath((char *)&up_info, sizeof(up_info)))
        {
            printk("app file info get fail\r\n");
            return false;
        }

        read_finger_from_iboot(finger, sizeof(finger));
        finger[sizeof(p_productinfo.TypeCode)] = 0;
        if((finger[0] != 0xff) && (strcmp(finger, PRODUCT_PRODUCT_MODEL_CODE) != 0))
        {
            printk("p_productinfo type code error \r\n");
            return false;
        }       //todo：这里设置指纹空的标志

        Iboot_GetProductInfo(&p_productinfo);   //获取还没升级前的文件头里的产品信息
        //如果版本号是0.0.0就把从服务器上获取到的生产时间和产品序号写到app文件头中，
        //否则就直接用原来app文件头里面的
        if((p_productinfo.VersionNumber[0] == 0) &&     //todo:考虑全擦掉的情形
                (p_productinfo.VersionNumber[1] == 0) && (p_productinfo.VersionNumber[2] == 0))
        {
            if(Iboot_GetUpdateSource() == SOURCE_FILE)
            {
                if((sizeof(up_info.file.production_time) > sizeof(p_productinfo.ProductionTime))
                || (sizeof(up_info.file.production_num) > sizeof(p_productinfo.ProductionNumber)))
                {
                    printk("param len overlong\r\n");
                    return false;
                }
                memcpy(&production_time, up_info.file.production_time, sizeof(up_info.ram.production_time));
                printk("production_time  = %4.4s,\r\n",production_time);

                memcpy(&production_num, up_info.file.production_num, sizeof(up_info.ram.production_num));
                printk("production_num   = %5.5s,\r\n",production_num);
            }
            else if(Iboot_GetUpdateSource() == SOURCE_ADDR_MEMORY)
            {
                if((sizeof(up_info.ram.production_time) > sizeof(p_productinfo.ProductionTime))
                || (sizeof(up_info.ram.production_num) > sizeof(p_productinfo.ProductionNumber)))
                {
                    printk("param len overlong\r\n");
                    return false;
                }
                memcpy(&production_time, up_info.ram.production_time, sizeof(up_info.ram.production_time));
                printk("production_time  = %4.4s,\r\n",production_time);

                memcpy(&production_num, up_info.ram.production_num, sizeof(up_info.ram.production_num));
                printk("production_num   = %5.5s,\r\n",production_num);
            }
        }
        else
        {   //使用本地文件的生产时间和生产序号
            memcpy(&production_time, p_productinfo.ProductionTime, sizeof(production_time));
            printk("production_time  = %4.4s,\r\n",production_time);

            memcpy(&production_num, p_productinfo.ProductionNumber, sizeof(production_num));
            printk("production_num   = %5.5s,\r\n",production_num);
        }
        if(write_finger_to_iboot((s8 *)production_time, sizeof(production_time), (s8 *)production_num, sizeof(production_num)) == false)
            info_printf("up", "iboot sn write fail\r\n");

        if(Iboot_GetUpdateSource() == SOURCE_FILE)
        {
            if(up_info.file.file_path[0] != '/')
            {
                printk(" file path error \r\n");
                return false;
            }
            memset(&file_state, 0, sizeof(struct stat));
            if(stat((const char *)up_info.file.file_path,&file_state))
            {
                info_printf("up", "get file fail\r\n");
                return false;
            }

            if(file_state.st_size  > app_max_size)
            {
                info_printf("up", "file size is overlong\r\n");
            }
            else
            {
                printk("Start erase flash\r\n");
                if(!File_Format((const char *)CFG_APP_SAVE_DIR))
                {
                    Iboot_UpdateApp();
                }
            }

        }
        else if(Iboot_GetUpdateSource() == SOURCE_ADDR_MEMORY)
        {
            start_addr = up_info.ram.start_add;
            file_size = up_info.ram.file_size;
            printk("psram_update_addr   = %x,\r\n", start_addr);
            printk("file_size   = %d,\r\n",file_size);
            memcpy(file_name, up_info.ram.file_name, sizeof(up_info.ram.file_name));
            printk("file_name   = %s,\r\n",file_name);
            if(file_size <= app_max_size)
            {
                if(XIP_CheckAppInMemory(start_addr))
                {
                    printk("File check success\r\n");
                    printf("Start erase flash\r\n");
                    if(!File_Format((const char *)CFG_APP_SAVE_DIR))
                    {
                        printf("Erase flash succeed\r\n");
                        //把产品序号和文件头，写入app文件头中
                        Iboot_RewriteProductInfoNumTime(start_addr + sizeof(struct AppHead), production_time, production_num);
                        if(app_ram_to_fs(CFG_APP_SAVE_DIR, start_addr, file_size, file_name))
                            ret = true;
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
        else
        {
            printk("app file info get fail\r\n");
        }
    }
    else
    {
        printk("No update flags\r\n");
    }
    return ret;
}

