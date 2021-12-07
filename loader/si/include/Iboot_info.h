//----------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
// Copyright (c) 2018，著作权由都江堰操作系统开源开发团队所有。著作权人保留一切权利。
//
// 这份授权条款，在使用者符合下列条件的情形下，授予使用者使用及再散播本
// 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
//
// 1. 对于本软件源代码的再散播，必须保留上述的版权宣告、本条件列表，以
//    及下述的免责声明。
// 2. 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
//    于散播包装中的媒介方式，重制上述之版权宣告、本条件列表，以及下述
//    的免责声明。

// 免责声明：本软件是本软件版权持有人以及贡献者以现状（"as is"）提供，
// 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
// 的的适用性为默示性担保。版权持有人及本软件之贡献者，无论任何条件、
// 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
// 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
// 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
// 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
// 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。
//-----------------------------------------------------------------------------
// 文件名     ：
// 模块描述:
// 模块版本:
// 创建人员: czz
// 创建时间:
// =============================================================================

#ifndef __SI_INFO_H__
#define __SI_INFO_H__
#include "stdio.h"
#include <project_config.h>

enum runibootmode{
    HARD_SET_RUN_IBOOT,     //硬件标志强行运行iboot
    RAM_SET_RUN_IBOOT,      //软件标志强行运行iboot
    UPDATE_APP_RUN_IBOOT,   //为了升级app，运行iboot
    CHACK_ERROR,            //运行app失败，转而运行iboot
};

enum runappmode{
    RUN_APP_FROM_FILE,      //从文件中加载app
    RUN_APP_FROM_DIRECT,    //app在片内flash中，直接运行app
    RUN_APP_FROM_UPDATE,
};

enum hardflag
{
    POWER_ON_FLAG,      //上电复位硬件标志， NO_HARD_FLAG =无此硬件；NO_FLAG=有此硬件，但无标志；
                        //FLAG_ONLY_READ_ONE=有标志，阅后即焚；FLAG_READ_MANY=有，非阅后即焚
    HARD_RESET_FLAG,    //硬件复位标志
    HARD_WDT_RESET,     //硬件看门狗复位标志
    LOW_POWER_WAKEUP,   //低功耗唤醒标志，（休眠唤醒）
    POWER_ON_RESET,     //硬件有带上电复位标志的

};

enum power_on_reset_flag{
    RESET_FLAG_NO_HARD_FLAG,       //无此硬件
    RESET_FLAG_NO_FLAG,            //有此硬件，但无标志，代表不是上电复位
    RESET_FLAG_READ_AND_CLR,       //是上电复位，该标志是“读清除”的
    RESET_FLAG_READ_NORMAL,        //是上电复位，该标志非“读清除”的
};


enum productinfo    //获取产品信息的命令码
{
    APP_HEAD_VERSION_NUM,        //APP版本xx.xx.xx，
    APP_HEAD_RAW_VERSION_NUM,    //APP版本号，（直接获取数组里的数字）
    APP_HEAD_MANUFACTURER,       //厂商名
    APP_HEAD_CLASSIFY,           //产品分类
    APP_HEAD_TYPE,               //产品型号
    APP_HEAD_TYPE_CODE,          //产品型号数字编码
    APP_HEAD_PRODUCTION_WEEK,    //生产时间
    APP_HEAD_PRODUCTION_NUM,     //产品序号
    APP_HEAD_BOARD_TYPE,         //板件型号
    APP_HEAD_CPU_TYPE,           //cpu型号

    APP_HEAD_FINGER,                 //获取指纹号
};
//升级文件来源,原名：update_source
//enum AppStoreMedia
//{
////  CN_STORE_IN_FILE = 0,         //从文件系统
////  CN_STORE_IN_ADDRMEM,          //从可寻址内存（可能是RAM，也可能是flash）
////  CN_STORE_IN_USER1,            //其他，例如没有安装文件系统的spiflash
////  CN_STORE_IN_USER2             //其他，例如没有安装文件系统的spiflash
//};
#define CN_STORE_IN_FILE        0   //ota代码存储在文件中
#define CN_STORE_IN_ADDRMEM     1   //ota代码存储在可寻址内存（可能是RAM，也可能是flash）
#define CN_STORE_IN_USER1       2   //其他，例如没有安装文件系统的spiflash
#define CN_STORE_IN_USER2       3   //其他，例如没有安装文件系统的spiflash

//在交互信息中用于存储待升级APP信息的最大长度
#define CN_APP_STORE_INFO_LIMIT 31
//在交互信息中存待升级文件路径的最大长度,就是 up_info 的长度
//符号原名： MutualPathLen
#define CN_UPDATE_PATH_LIMIT    CN_APP_STORE_INFO_LIMIT
//union update_info{
//    struct ram_update_info {    //从可寻址内存升级时的信息结构
//        s8 *start_add;
//        u32 file_size;
////      s8 production_time[4];     //生产时间，BCD码，年+星期（3字节）.
////      s8 production_num[5];   //产品序号，
////      s8 file_name[23];
//    }ram;
//    struct file_update_info {   //从文件系统升级时的信息结构
////      s8 file_path[31];
////      s8 production_time[4];     //生产时间，BCD码，年+星期（3字节）.
////      s8 production_num[5];   //产品序号，
//    }file;
//    char file_path[CN_UPDATE_PATH_LIMIT];
//    char info[CN_UPDATE_PATH_LIMIT];    //
//    char pad[40];    //使结构的尺寸等于40字节
//};
union app_stored_info{
    struct stored_inram {    //从可寻址内存升级时的信息结构
        s8 *start_add;
        u32 app_size;
    }ram;
    char file_path[CN_UPDATE_PATH_LIMIT];
    char pads[CN_APP_STORE_INFO_LIMIT];    //使结构的尺寸等于31字节
};

//#define IBOOT_APP_INFO_VER                 (1)
struct IbootAppInfo
{
    #define PREVIOURESET_IBOOT   (0x12345678)//复位前运行iboot
    #define PREVIOURESET_APP     (0x87654321)//复位前运行APP
                                             //其他值表示上电或复位
    u32 previou_reset;                       //复位前运行模式
    struct{
        u32 hard_set_run_iboot    :1;   //0硬件要求强制运行iboot
        u32 restart_run_iboot     :1;   //1指示启动后运行Iboot
        u32 restart_run_app       :1;   //2指示启动后运行APP
        u32 runmode_iboot         :1;   //3当前运行模式是iboot
        u32 runmode_app           :1;   //4当前运行模式为app
        u32 Before_run_iboot      :1;   //5之前运行模式为iboot
        u32 Before_run_app        :1;   //6之前运行模式为app
        u32 run_app_form_file     :1;   //7从文件中加载app
        u32 run_iboot_and_update_app  :1;   //8启动（Iboot）后，自动升级APP
        u32 run_app_and_update_iboot  :1;   //9启app后升级iboot自身
        u32 update_from           :2;   //10~11升级文件来源:0文件,1:内存,2—3用户自定义
        u32 after_update_runmode  :1;   //12升级完成后运行0.iboot --  1.app
        u32 error_app_check       :1;   //13校验出错
        u32 error_app_no_file     :1;   //14没有这个文件或文件格式错误
        u32 error_app_size        :1;   //15 app文件大小错误
        //上电复位硬件标志0=无此硬件 1=有此硬件，但无标志；2=有标志，阅后即焚；3=有，非阅后即焚；
        u32 power_on_flag         :2;   //16~17
        u32 hard_wdt_reset        :1;   //18看门狗复位标志
        u32 soft_reset_flag       :1;   //19软件引起的内部复位
        u32 reboot_flag           :1;   //20 CPU_Reboot 标志
        u32 restart_system_flag   :1;   //21 restart_system标志
        u32 hard_reset_flag       :1;   //22外部硬件复位标志
        u32 low_power_wakeup      :1;   //23低功耗深度休眠中断唤醒标志
        u32 call_fun_reset        :1;   //24 1=内部复位/重启是主动调用相关函数引发的；0=异常重启
        u32 power_on_reset_flag   :1;   //25上电复位标志，结合b16~17以及“上电标志”字判定
    }runflag;                           //运行标志
    u32  UserTag;                       //用户标志
    u32  reserved;                      //保留
    u16  iboot_build_year;              // years since 1900
    u8   iboot_build_mon;               // months since January [0-11
    u8   iboot_build_day;               // day of the month [1-31]
    u8   iboot_build_hour;              // hours since midnight [0-23]
    u8   iboot_build_min;               // minutes after the hour [0-59]
    u8   iboot_build_sec;               // seconds after the minute [0-60]
    u16  app_build_year;                // years since 1900
    u8   app_build_mon;                 // months since January [0-11]
    u8   app_build_day;                 // day of the month [1-31]
    u8   app_build_hour;                // hours since midnight [0-23]
    u8   app_build_min;                 // minutes after the hour [0-59]
    u8   app_build_sec;                 // seconds after the minute [0-60]
    u8   iboot_ver_small;               //iboot 版本 xx.xx.__
    u8   iboot_ver_medium;              //iboot 版本 xx.__.xx
    u8   iboot_ver_large;               //iboot 版本 __.xx.xx
    u8   app_ver_small;                 //app 版本 xx.xx.__
    u8   app_ver_medium;                //app 版本 xx.__.xx
    u8   app_ver_large;                 //app 版本 __.xx.xx
    char board_name[20];                //板件名
    s8   production_week[4];            //生产时间，两字节年+两字节星期组成，大端
    s8   production_serial[5];          //一周内生产的产品串号，34进制，见文档
//  union update_info  up_info;
    union app_stored_info stored;       //保存APP的参数，例如起始地址、文件路径等
};

struct AppHead
{
    char djy_flag[3];        //"djy"标志，固定标志
    u8   app_ver;            //信息块的版本
    u32  file_size;          //文件系统读到的文件大小，在线升级时，由文件系统填充，编译时由外部工具填充

    //以下定义不能修改，添加和修改都需要同步修改addver.exe 
    #define VERIFICATION_NULL     0
    #define VERIFICATION_CRC      1
    #define VERIFICATION_MD5      2
    #define VERIFICATION_SSL      3  //SSL安全证书
    u32  verification;    //校验方法默认校验方法为不校验，由外部工具根据配置修改
    u32  app_bin_size;      //app bin文件大小，由外部工具填充
#if(CN_PTR_BITS < 64)
    u32  virt_addr;        //运行地址
    u32  reserved32;      //保留
#else
    u64  virt_addr;        //运行地址
#endif
    u16  app_head_size;     //信息块的大小
    u8 start_app_is_verify;  //启动App之前是否进行校验
    char reserved[5];        //保留
    char app_name[96];      //app的文件名 由外部工具填充该bin文件的文件名
    char verif_buf[128];     //校验码与校验方法对应的具体内容 由工具填充
};

struct ProductInfo
{
#if(CN_PTR_BITS < 64)
    u32  ManufacturerNameAddr;        //厂商名地址
    u32  ManufacturerNamereserved32;      //保留
#else
    u64  ManufacturerNameAddr;        //厂商名地址
#endif
    const char ProductClassify[9];    //产品分类字符串，厂商在DIDE中配置
    const char ProductType[9];        //产品型号字符串，厂商在DIDE中配置
    const char TypeCode[6];           //产品型号数字编码
    char ProductionTime[4];     //生产时间，BCD码，年+星期（3字节）.源码中填'*'，生产时服务器下发，iboot写入
    char ProductionNumber[5];   //产品序号，源码中填'*'，生产时服务器下发，iboot写入
    char reserved8;             //保留
    char VersionNumber[3];      //APP版本xx.xx.xx，
    const char BoardType[16];   //板件型号
    const char CPU_Type[16];   //cpu型号
    char Reserved[179];         //保留
};

//bool_t Set_RunIbootUpdateIboot();
bool_t Iboot_SetRebootFlag();
bool_t Iboot_SetSoftResetFlag();
bool_t Iboot_SetRestartAppFlag();
bool_t Iboot_ClearResetFlag();
bool_t Iboot_SiIbootAppInfoInit();
void Iboot_GetAppInfo(struct IbootAppInfo *get_info);
//bool_t XIP_APPIsDebug(void );
bool_t Iboot_SetAppVerFlag(u8 small, u8 medium, u8 large);
void Iboot_SetUserTag(u32 UserTag);
u32 Iboot_GetUserTag(void);
void Iboot_SetWeek(s8 *week);
bool_t Iboot_GetWeek(s8 *week);
void Iboot_SetSerial(s8 *serial);
bool_t Iboot_GetSerial(s8 *serial);
void Iboot_SetOtaAddr(s8 *address, ptu32_t size);
void Iboot_GetOtaAddr(s8 **address, ptu32_t *size);
void Iboot_SetOtaUserInfo(s8 *address, ptu32_t size);
void Iboot_GetOtaUserInfo(s8 *address, ptu32_t size);
void Iboot_SetOtaFilename(s8 *filename, ptu32_t size);
void Iboot_GetOtaFilename(s8 *filename, ptu32_t size);
bool_t Iboot_SetAppBulidTime(u16 pyear,u8 pmon,u8 pday,u8 phour,u8 pmin,u8 psec);
bool_t Iboot_GetPowerOnResentFlag(void);
bool_t Iboot_GetCallFunResent();
bool_t Iboot_GetSoftResetFlag();
char Iboot_GetPowerOnFlag(void);
bool_t Iboot_GetHeadWdtReset(void);
bool_t Iboot_GetRebootFlag();
bool_t Iboot_GetRestartAppFlag();
bool_t Iboot_GetHeadResetFlag();
bool_t Iboot_GetLowPowerWakeup();

#if (CFG_RUNMODE_BAREAPP == 0)
bool_t Iboot_GetProductInfo(enum productinfo type, char *date_buf, u32 buf_len);
bool_t write_finger_to_iboot(s8 *time, s8 *num);
bool_t read_finger_from_iboot(s8 *finger, u32 buf_len);
bool_t Iboot_RewriteAppHeadFileInfo(void * apphead,const char*name,u32 filesize);
bool_t XIP_AppFileCheckEasy(void * apphead);
bool_t XIP_CheckAppInMemory(void * apphead);
bool_t XIP_CheckAppInFile(const char *path);
void * XIP_GetAppStartAddr(void * apphead);
u32  XIP_GetAppSize(void * apphead);
u32 Iboot_GetAppSize(void * apphead);
char*  Iboot_GetAppName(void * apphead);
bool_t XIP_IsRamIbootFlag();
//bool_t set_upgrade_info(char* Path,int len);
bool_t Run_Iboot(enum runibootmode mode);
bool_t Run_App(enum runappmode mode);
bool_t Iboot_UpdateToRun();
bool_t Iboot_SetPreviouResetFlag();
bool_t Iboot_SetRunIbootFlag();
bool_t Iboot_SetRunAppFlag();
bool_t Iboot_SetRunIbootAndUpdateApp();
bool_t Iboot_SetRunAppAndUpdateIboot();
bool_t Iboot_ClearRunIbootAndUpdateApp();
bool_t Iboot_ClearRunAppAndUpdateIboot();
bool_t Iboot_SetUpdateSource(u32 source);
u32 Iboot_GetUpdateSource(void);
bool_t Iboot_GetRestartRunApp();
bool_t Iboot_IsLoadAppFromFile();
char Iboot_GetRunMode(void);
char Iboot_GetLastRunMode(void);
bool_t Iboot_GetHeardSetRunIboot(void);
//bool_t Iboot_GetMutualUpdatePath(char *buf, u32 buf_len);
bool_t Iboot_GetUpdateApp(void);
bool_t Iboot_GetUpdateIboot(void);
bool_t Iboot_SetAfterUpdateRunMode(u8 mode);
char Iboot_GetAfterUpdateRunModet(void);
u32  XIP_GetAppSize(void * apphead);
u32  Iboot_GetAppHeadSize(void);
bool_t Iboot_GetAppHead(void * apphead);
bool_t Iboot_GetErrorAppCheck(void);
bool_t Iboot_GetErrorAppNoFile(void);
bool_t Iboot_GetErrorAppSize(void);
bool_t Iboot_GetHeardSetRunIboot(void);
//char * Get_ProductSN(void);
//char * Get_ProductVersion(void * apphead);
//const char * Get_ManufacturerName(void);
void Iboot_CheckAppComplete(void * apphead);
void Iboot_CheckAppBody(void * apphead, u8 * buf, u32 len);
bool_t Iboot_CheckAppHead(void *apphead);
bool_t  Iboot_CheckAppCompare(void *apphead,void *apphead_back);

void CPU_Reset(void);
void CPU_Reboot(void);
void CPU_RestartSystem(void);

bool_t  runiboot(char *param);
bool_t  runapp(char *param);

bool_t WriteAppFromRam(s8 *addr,u32 file_size, s8 *file_name);
bool_t WriteAppFromFile(u8 *production_time,u8* production_num);
#endif
#endif /* __IICBUS_H__ */
