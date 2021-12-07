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
// Copyright (c) 2018������Ȩ�ɶ����߲���ϵͳ��Դ�����Ŷ����С�����Ȩ�˱���һ��Ȩ����
//
// �����Ȩ�����ʹ���߷������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
// �ļ���     ��
// ģ������:
// ģ��汾:
// ������Ա: czz
// ����ʱ��:
// =============================================================================

#ifndef __SI_INFO_H__
#define __SI_INFO_H__
#include "stdio.h"
#include <project_config.h>

enum runibootmode{
    HARD_SET_RUN_IBOOT,     //Ӳ����־ǿ������iboot
    RAM_SET_RUN_IBOOT,      //�����־ǿ������iboot
    UPDATE_APP_RUN_IBOOT,   //Ϊ������app������iboot
    CHACK_ERROR,            //����appʧ�ܣ�ת������iboot
};

enum runappmode{
    RUN_APP_FROM_FILE,      //���ļ��м���app
    RUN_APP_FROM_DIRECT,    //app��Ƭ��flash�У�ֱ������app
    RUN_APP_FROM_UPDATE,
};

enum hardflag
{
    POWER_ON_FLAG,      //�ϵ縴λӲ����־�� NO_HARD_FLAG =�޴�Ӳ����NO_FLAG=�д�Ӳ�������ޱ�־��
                        //FLAG_ONLY_READ_ONE=�б�־���ĺ󼴷٣�FLAG_READ_MANY=�У����ĺ󼴷�
    HARD_RESET_FLAG,    //Ӳ����λ��־
    HARD_WDT_RESET,     //Ӳ�����Ź���λ��־
    LOW_POWER_WAKEUP,   //�͹��Ļ��ѱ�־�������߻��ѣ�
    POWER_ON_RESET,     //Ӳ���д��ϵ縴λ��־��

};

enum power_on_reset_flag{
    RESET_FLAG_NO_HARD_FLAG,       //�޴�Ӳ��
    RESET_FLAG_NO_FLAG,            //�д�Ӳ�������ޱ�־���������ϵ縴λ
    RESET_FLAG_READ_AND_CLR,       //���ϵ縴λ���ñ�־�ǡ����������
    RESET_FLAG_READ_NORMAL,        //���ϵ縴λ���ñ�־�ǡ����������
};


enum productinfo    //��ȡ��Ʒ��Ϣ��������
{
    APP_HEAD_VERSION_NUM,        //APP�汾xx.xx.xx��
    APP_HEAD_RAW_VERSION_NUM,    //APP�汾�ţ���ֱ�ӻ�ȡ����������֣�
    APP_HEAD_MANUFACTURER,       //������
    APP_HEAD_CLASSIFY,           //��Ʒ����
    APP_HEAD_TYPE,               //��Ʒ�ͺ�
    APP_HEAD_TYPE_CODE,          //��Ʒ�ͺ����ֱ���
    APP_HEAD_PRODUCTION_WEEK,    //����ʱ��
    APP_HEAD_PRODUCTION_NUM,     //��Ʒ���
    APP_HEAD_BOARD_TYPE,         //����ͺ�
    APP_HEAD_CPU_TYPE,           //cpu�ͺ�

    APP_HEAD_FINGER,                 //��ȡָ�ƺ�
};
//�����ļ���Դ,ԭ����update_source
//enum AppStoreMedia
//{
////  CN_STORE_IN_FILE = 0,         //���ļ�ϵͳ
////  CN_STORE_IN_ADDRMEM,          //�ӿ�Ѱַ�ڴ棨������RAM��Ҳ������flash��
////  CN_STORE_IN_USER1,            //����������û�а�װ�ļ�ϵͳ��spiflash
////  CN_STORE_IN_USER2             //����������û�а�װ�ļ�ϵͳ��spiflash
//};
#define CN_STORE_IN_FILE        0   //ota����洢���ļ���
#define CN_STORE_IN_ADDRMEM     1   //ota����洢�ڿ�Ѱַ�ڴ棨������RAM��Ҳ������flash��
#define CN_STORE_IN_USER1       2   //����������û�а�װ�ļ�ϵͳ��spiflash
#define CN_STORE_IN_USER2       3   //����������û�а�װ�ļ�ϵͳ��spiflash

//�ڽ�����Ϣ�����ڴ洢������APP��Ϣ����󳤶�
#define CN_APP_STORE_INFO_LIMIT 31
//�ڽ�����Ϣ�д�������ļ�·������󳤶�,���� up_info �ĳ���
//����ԭ���� MutualPathLen
#define CN_UPDATE_PATH_LIMIT    CN_APP_STORE_INFO_LIMIT
//union update_info{
//    struct ram_update_info {    //�ӿ�Ѱַ�ڴ�����ʱ����Ϣ�ṹ
//        s8 *start_add;
//        u32 file_size;
////      s8 production_time[4];     //����ʱ�䣬BCD�룬��+���ڣ�3�ֽڣ�.
////      s8 production_num[5];   //��Ʒ��ţ�
////      s8 file_name[23];
//    }ram;
//    struct file_update_info {   //���ļ�ϵͳ����ʱ����Ϣ�ṹ
////      s8 file_path[31];
////      s8 production_time[4];     //����ʱ�䣬BCD�룬��+���ڣ�3�ֽڣ�.
////      s8 production_num[5];   //��Ʒ��ţ�
//    }file;
//    char file_path[CN_UPDATE_PATH_LIMIT];
//    char info[CN_UPDATE_PATH_LIMIT];    //
//    char pad[40];    //ʹ�ṹ�ĳߴ����40�ֽ�
//};
union app_stored_info{
    struct stored_inram {    //�ӿ�Ѱַ�ڴ�����ʱ����Ϣ�ṹ
        s8 *start_add;
        u32 app_size;
    }ram;
    char file_path[CN_UPDATE_PATH_LIMIT];
    char pads[CN_APP_STORE_INFO_LIMIT];    //ʹ�ṹ�ĳߴ����31�ֽ�
};

//#define IBOOT_APP_INFO_VER                 (1)
struct IbootAppInfo
{
    #define PREVIOURESET_IBOOT   (0x12345678)//��λǰ����iboot
    #define PREVIOURESET_APP     (0x87654321)//��λǰ����APP
                                             //����ֵ��ʾ�ϵ��λ
    u32 previou_reset;                       //��λǰ����ģʽ
    struct{
        u32 hard_set_run_iboot    :1;   //0Ӳ��Ҫ��ǿ������iboot
        u32 restart_run_iboot     :1;   //1ָʾ����������Iboot
        u32 restart_run_app       :1;   //2ָʾ����������APP
        u32 runmode_iboot         :1;   //3��ǰ����ģʽ��iboot
        u32 runmode_app           :1;   //4��ǰ����ģʽΪapp
        u32 Before_run_iboot      :1;   //5֮ǰ����ģʽΪiboot
        u32 Before_run_app        :1;   //6֮ǰ����ģʽΪapp
        u32 run_app_form_file     :1;   //7���ļ��м���app
        u32 run_iboot_and_update_app  :1;   //8������Iboot�����Զ�����APP
        u32 run_app_and_update_iboot  :1;   //9��app������iboot����
        u32 update_from           :2;   //10~11�����ļ���Դ:0�ļ�,1:�ڴ�,2��3�û��Զ���
        u32 after_update_runmode  :1;   //12������ɺ�����0.iboot --  1.app
        u32 error_app_check       :1;   //13У�����
        u32 error_app_no_file     :1;   //14û������ļ����ļ���ʽ����
        u32 error_app_size        :1;   //15 app�ļ���С����
        //�ϵ縴λӲ����־0=�޴�Ӳ�� 1=�д�Ӳ�������ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷٣�
        u32 power_on_flag         :2;   //16~17
        u32 hard_wdt_reset        :1;   //18���Ź���λ��־
        u32 soft_reset_flag       :1;   //19���������ڲ���λ
        u32 reboot_flag           :1;   //20 CPU_Reboot ��־
        u32 restart_system_flag   :1;   //21 restart_system��־
        u32 hard_reset_flag       :1;   //22�ⲿӲ����λ��־
        u32 low_power_wakeup      :1;   //23�͹�����������жϻ��ѱ�־
        u32 call_fun_reset        :1;   //24 1=�ڲ���λ/����������������غ��������ģ�0=�쳣����
        u32 power_on_reset_flag   :1;   //25�ϵ縴λ��־�����b16~17�Լ����ϵ��־�����ж�
    }runflag;                           //���б�־
    u32  UserTag;                       //�û���־
    u32  reserved;                      //����
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
    u8   iboot_ver_small;               //iboot �汾 xx.xx.__
    u8   iboot_ver_medium;              //iboot �汾 xx.__.xx
    u8   iboot_ver_large;               //iboot �汾 __.xx.xx
    u8   app_ver_small;                 //app �汾 xx.xx.__
    u8   app_ver_medium;                //app �汾 xx.__.xx
    u8   app_ver_large;                 //app �汾 __.xx.xx
    char board_name[20];                //�����
    s8   production_week[4];            //����ʱ�䣬���ֽ���+���ֽ�������ɣ����
    s8   production_serial[5];          //һ���������Ĳ�Ʒ���ţ�34���ƣ����ĵ�
//  union update_info  up_info;
    union app_stored_info stored;       //����APP�Ĳ�����������ʼ��ַ���ļ�·����
};

struct AppHead
{
    char djy_flag[3];        //"djy"��־���̶���־
    u8   app_ver;            //��Ϣ��İ汾
    u32  file_size;          //�ļ�ϵͳ�������ļ���С����������ʱ�����ļ�ϵͳ��䣬����ʱ���ⲿ�������

    //���¶��岻���޸ģ���Ӻ��޸Ķ���Ҫͬ���޸�addver.exe 
    #define VERIFICATION_NULL     0
    #define VERIFICATION_CRC      1
    #define VERIFICATION_MD5      2
    #define VERIFICATION_SSL      3  //SSL��ȫ֤��
    u32  verification;    //У�鷽��Ĭ��У�鷽��Ϊ��У�飬���ⲿ���߸��������޸�
    u32  app_bin_size;      //app bin�ļ���С�����ⲿ�������
#if(CN_PTR_BITS < 64)
    u32  virt_addr;        //���е�ַ
    u32  reserved32;      //����
#else
    u64  virt_addr;        //���е�ַ
#endif
    u16  app_head_size;     //��Ϣ��Ĵ�С
    u8 start_app_is_verify;  //����App֮ǰ�Ƿ����У��
    char reserved[5];        //����
    char app_name[96];      //app���ļ��� ���ⲿ��������bin�ļ����ļ���
    char verif_buf[128];     //У������У�鷽����Ӧ�ľ������� �ɹ������
};

struct ProductInfo
{
#if(CN_PTR_BITS < 64)
    u32  ManufacturerNameAddr;        //��������ַ
    u32  ManufacturerNamereserved32;      //����
#else
    u64  ManufacturerNameAddr;        //��������ַ
#endif
    const char ProductClassify[9];    //��Ʒ�����ַ�����������DIDE������
    const char ProductType[9];        //��Ʒ�ͺ��ַ�����������DIDE������
    const char TypeCode[6];           //��Ʒ�ͺ����ֱ���
    char ProductionTime[4];     //����ʱ�䣬BCD�룬��+���ڣ�3�ֽڣ�.Դ������'*'������ʱ�������·���ibootд��
    char ProductionNumber[5];   //��Ʒ��ţ�Դ������'*'������ʱ�������·���ibootд��
    char reserved8;             //����
    char VersionNumber[3];      //APP�汾xx.xx.xx��
    const char BoardType[16];   //����ͺ�
    const char CPU_Type[16];   //cpu�ͺ�
    char Reserved[179];         //����
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
