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

extern u32 gc_ProductSn;

//----------------------------------------------------------------------------
//功能: 对文件系统里的文件进行校验
//参数: path：文件路径
//返回: true: 成功；false ： 失败.
//-----------------------------------------------------------------------------
bool_t app_check_from_fs(const char *path)
{
    FILE *fp = NULL;
    s8 *apphead = NULL;
    s8 *apphead_back = NULL;
    s8 *buf = NULL;
    u32 file_size = 0, readsize, buf_len = 1024;
    struct stat file_stat;
    bool_t ret = false;

    if(stat(path,&file_stat) == -1)     //看文件是否存在
    {
        printk("update file stat get fail!\r\n");
        return false;
    }

    fp = fopen(path, "r");
    if(fp == NULL)
    {
        printf("update file open fail!\r\n");
        return false;
    }

    buf = (s8 *)malloc(buf_len);      //存读到的文件数据
    if(buf)
    {
        apphead = (s8 *)malloc(sizeof(struct AppHead)+sizeof(struct ProductInfo));    //存文件头信息
        if(apphead)
        {
            apphead_back = (s8 *)malloc(sizeof(struct AppHead)+sizeof(struct ProductInfo));   //备份文件头信息
            if(apphead_back)
            {
                readsize = fread(apphead, 1, sizeof(struct AppHead)+sizeof(struct ProductInfo), fp);    //读文件头
                if(readsize == (sizeof(struct AppHead)+sizeof(struct ProductInfo)))
                {
                    memcpy(apphead_back, apphead, readsize);
                    file_size += readsize;
                    while(1)
                    {
                        readsize = fread(buf, 1, buf_len, fp);
                        if(readsize)
                        {
                            if(XIP_AppFileCheckSubsection(apphead, (u8 *)buf, readsize, NULL) == false)
                            {
                                printf("file check error\r\n");
                                break;
                            }
                            file_size += readsize;
                        }
                        else
                        {       //文件全读完了
                            if(file_size != (u32)file_stat.st_size)
                            {
                                printf("file check : file size error\r\n");
                            }
                            if(XIP_AppFileCheckSubsection(apphead, (u8 *)buf, readsize, apphead_back) == false)   //比较两个文件头里的校验码
                            {
                                printf("file check error\r\n");
                            }
                            else
                                ret = true;
                            break;
                        }
                    }
                }
                else
                    printf("read file %s fail\r\n", path);

                free(apphead_back);
            }
            free(apphead);
        }
        free(buf);
    }
    fclose(fp);

    return ret;
}

//----------------------------------------------------------------------------
//功能: app发送，app的相关升级信息给iboot，
//参数: info：升级相关信息，
//返回: 0: 成功； -1 ： 失败.
//-----------------------------------------------------------------------------
s32 app_update_info_to_iboot(struct update_app_info *info)
{
    struct AppHead *apphead;
    u32 file_size;
    union update_info  up_info;
    // s8 app_info[MutualPathLen]; //app和iboot的共享内存只留了40个字节来存升级相关信息
//    u8 app_info_size = 0;

    if((info->time_buf_len > sizeof(up_info.ram.production_time)) 
        || (info->prod_num_buf_len > sizeof(up_info.ram.production_num)))
    {
        info_printf("up", "param len overlong \r\n");
        return -1;
    }
        
    if(info->start_addr != 0)       //看是否有给出起始地址，有则是从内存里升级，无则不是
    {
        if(info->file_name_len > sizeof(up_info.ram.file_name))
        {
            info_printf("up", "file name len overlong \r\n");
            return -1;
        }
        apphead = (struct AppHead *)info->start_addr;
        file_size = info->end_addr - info->start_addr;  //计算文件大小
        if((apphead->app_bin_size != apphead->file_size) || (apphead->file_size != file_size))
        {
            printk("ERROR: file size diff.apphead_appbinsize = %d, apphead_filesize = %d, file_size = %d!\r\n",apphead->app_bin_size, apphead->file_size, file_size);
            return -1;
        }

        if(XIP_AppFileCheck(info->start_addr))
        {
            printf("File check correct\r\n");

            memset(up_info.info, 0, sizeof(up_info.info));
            up_info.ram.start_add = info->start_addr;
            up_info.ram.file_size = file_size;
            memcpy(up_info.ram.production_time, info->time_buf, info->time_buf_len);   //生产时间
            memcpy(up_info.ram.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
            memcpy(up_info.ram.file_name, info->file_name, info->file_name_len);


            // memcpy(app_info + app_info_size, &info->start_addr, sizeof(info->start_addr));  //app在ram的起始地址
            // app_info_size += sizeof(info->start_addr);

            // memcpy(app_info + app_info_size, &file_size, sizeof(file_size));    //app文件大小
            // app_info_size += sizeof(file_size);

            // memcpy(app_info + app_info_size, info->time_buf, info->time_buf_len);   //生产时间
            // app_info_size += info->time_buf_len;

            // memcpy(app_info + app_info_size, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
            // app_info_size += info->prod_num_buf_len;

            // if((app_info_size + info->file_name_len) > MutualPathLen)
            // {
            //     printk("ERROR: file_namd length exceeded!\r\n");    //判断app_info还够不够存文件名。
            //     return -1;
            // }
            // memcpy(app_info + app_info_size, info->file_name, info->file_name_len);

            // Iboot_FillMutualUpdatePath((char *)&up_info, sizeof(up_info));
            Iboot_SetUpdateSource(SOURCE_ADDR_MEMORY);  //设置升级方式为，从ram获取app数据
            //
            // Iboot_SetRunIbootUpdateApp();
            // Set_RunIbootFlag();
        }
        else
        {
            printk("File check fail\r\n");
            return -1;
        }
    }
    else if(info->file_name != 0)   //如果没给出start_addr，但是给出了file_namd则是从文件系统升级
    {
        if(info->file_name_len > sizeof(up_info.file.file_path))
        {
            info_printf("up", "file path len overlong \r\n");
            return -1;
        }
        if(app_check_from_fs((const char *)info->file_name))
        {
            printk("File check correct\r\n");

            memcpy(up_info.file.production_time, info->time_buf, info->time_buf_len);   //生产时间
            memcpy(up_info.file.production_num, info->prod_num_buf, info->prod_num_buf_len);   //产品序号
            memcpy(up_info.file.file_path, info->file_name, info->file_name_len);

            // Iboot_FillMutualUpdatePath((char *)&up_info, sizeof(up_info));
            Iboot_SetUpdateSource(SOURCE_FILE);  //设置升级方式为，从ram获取app数据
            // Iboot_SetRunIbootUpdateApp();
            // Set_RunIbootFlag();
        }
        else
        {
            printk("File check fail\r\n");
            return -1;
        }
    }
    Iboot_FillMutualUpdatePath((char *)&up_info, sizeof(up_info));
    Iboot_SetRunIbootUpdateApp();
    Set_RunIbootFlag();
    return 0;

}


//----------------------------------------------------------------------------
//功能: 把app从可寻址内存写入到文件系统中
//参数: dir：待写入的目录，一般是/xpi-app，addr：内存起始地址，file_size：文件大小，file_name：文件名
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
//功能: 把app从可寻址内存写入到文件系统中
//参数: target_dir：app文件待待写入的目录，app_max_size：app文件的最大大小
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
static bool_t write_sn_to_iboot(s8 *time, u32 time_len, s8 *num, u32 num_len)
{
    u8 *iboot_sn_buf = 0;
    u32 iboot_sn_addr = 0;

    iboot_sn_buf = malloc(time_len + num_len + 1);
    if(iboot_sn_buf == NULL)
        return false;

    iboot_sn_addr = (u32)(&gc_ProductSn) / 32 * 34;
    if(iboot_sn_addr)
    {
        //直接带CRC的读出来，因为这个SN号里面不会存在CRC
        djy_flash_read_crc(iboot_sn_addr, iboot_sn_buf, time_len + num_len);
        if((iboot_sn_buf[0] == 0xff) && ((u8)time[0] != 0xff))
        {   //iboot里没SN，程序里给出SN号，现在写SN
            printf("write SN in iboot.\r\n");
            memcpy(iboot_sn_buf, time, time_len);
            memcpy(iboot_sn_buf + time_len, num, num_len);

//            flash_protection_op(0,FLASH_PROTECT_NONE);
            djy_flash_write(iboot_sn_addr, iboot_sn_buf, time_len + num_len);
//            flash_protection_op(0,FLASH_PROTECT_ALL);
        }
    }

    free(iboot_sn_buf);
    return true;
}


//----------------------------------------------------------------------------
//功能: 把app从可寻址内存写入到文件系统中
//参数: target_dir：app文件待待写入的目录，app_max_size：app文件的最大大小
//返回: true: 成功； false ： 失败.
//-----------------------------------------------------------------------------
int to_update_app(s8 *target_dir, u32 app_max_size)
{
    int ret = -1;
//    char app_info[MutualPathLen];
    union update_info  up_info;
    s8 file_name[sizeof(up_info.ram.file_name)];
    struct ProductInfo p_productinfo;
    s8 *start_addr = 0;
//    u8 app_info_offset = 0;
    u32 file_size = 0;
//    u32 iboot_sn_addr = 0;
    struct stat file_state;
    char production_num[sizeof(p_productinfo.ProductionNumber)],production_time[sizeof(p_productinfo.ProductionTime)];
//    u8 iboot_sn_buf[sizeof(p_productinfo.ProductionNumber) + sizeof(p_productinfo.ProductionTime)];
    if(Iboot_GetUpdateApp() == true)
    {
        if(!Iboot_GetMutualUpdatePath((char *)&up_info, sizeof(up_info)))
        {
            printk("app file info get fail\r\n");
            return false;
        }
        Iboot_GetProductInfo(&p_productinfo);   //获取还没升级前的文件头里的产品信息
        //如果版本号是0.0.0就把从服务器上获取到的生产时间和产品序号写到app文件头中，
        //否则就直接用原来app文件头里面的
        if((p_productinfo.VersionNumber[0] == 0) &&
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
        {
            memcpy(&production_time, p_productinfo.ProductionTime, sizeof(production_time));
            printk("production_time  = %4.4s,\r\n",production_time);

            memcpy(&production_num, p_productinfo.ProductionNumber, sizeof(production_num));
            printk("production_num   = %5.5s,\r\n",production_num);
        }
        if(write_sn_to_iboot((s8 *)production_time, sizeof(production_time), (s8 *)production_num, sizeof(production_num)) == false)
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
                if(!File_Format((const char *)target_dir))
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
                if(XIP_AppFileCheck(start_addr))
                {
                    printk("File check success\r\n");
                    printf("Start erase flash\r\n");
                    if(!File_Format((const char *)target_dir))
                    {
                        printf("Erase flash succeed\r\n");
                        //把产品序号和文件头，写入app文件头中
                        Iboot_RewriteProductInfoNumTime(start_addr + sizeof(struct AppHead), production_time, production_num);
                        if(app_ram_to_fs(target_dir, start_addr, file_size, file_name))
                            ret = 0;
                    }
                }
                else
                {
                    printk("File check fail\r\n");
                }
            }
            else
                info_printf("up", "file size is overlong\r\n");

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

