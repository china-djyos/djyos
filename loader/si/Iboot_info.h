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
#define MutualPathLen   40   //�ڽ�����Ϣ�д�������ļ�·������󳤶�
enum runibootmode{
    HEARD_SET_RUN_IBOOT,
    RAM_SET_RUN_IBOOT,
    UPDATE_APP_RUN_IBOOT,
    CHACK_ERROR,
};

enum runappmode{
    RUN_APP_FROM_FILE,
    RUN_APP_FROM_DIRECT,
    RUN_APP_FROM_UPDATE,
};

enum hardflag
{
    POWER_ON_FLAG,
    HEAD_RESET_FLAG,
    HEAD_WDT_RESET,
    LOW_POWER_WAKEUP,
    POWER_ON_RESET,

};
//#define IBOOT_APP_INFO_VER                 (1)
struct IbootAppInfo
{
    #define PREVIOURESET_IBOOT   (0x12345678)//��λǰ����iboot
    #define PREVIOURESET_APP     (0x87654321)//��λǰ����APP
    u32 PreviouReset;//��λǰ����ģʽ
    struct{
        u32 heard_set_run_iboot   :1;//Ӳ����������iboot
        u32 restart_run_iboot     :1;//ָʾ����������Iboot
        u32 restart_run_app       :1;//ָʾ����������APP
        u32 runmode_iboot         :1;//��ǰ����ģʽ��iboot
        u32 runmode_app           :1;//��ǰ����ģʽΪapp
        u32 Before_run_iboot      :1;//֮ǰ����ģʽΪiboot
        u32 Before_run_app        :1;//֮ǰ����ģʽΪapp
        u32 run_app_form_file     :1;//���ļ��м���app
        u32 run_iboot_update_app  :1;//������Iboot�����Զ�����APP
        u32 run_app_update_iboot  :1;//��app������iboot����
        u32 update_from           :2;//�����ļ���Դ0�ļ� 1����3������
        u32 update_runmode        :1;//������ɺ�����0.iboot --  1.app
        u32 error_app_check       :1;//У�����
        u32 error_app_no_file     :1;//û������ļ����ļ���ʽ����
        u32 error_app_size        :1;//app�ļ���С����
        //�ϵ縴λӲ����־0=�޴�Ӳ�� 1=�д�Ӳ�������ޱ�־��2=�б�־���ĺ󼴷٣�3=�У����ĺ󼴷٣�
        u32 power_on_flag         :2;
        u32 head_wdt_reset        :1;//���Ź���λ��־
        u32 soft_reset_flag       :1;//���������ڲ���λ
        u32 reboot_flag           :1;//reboot ��־
        u32 restart_system_flag      :1;//restart_system��־
        u32 head_reset_flag       :1;//�ⲿӲ����λ��־
        u32 low_power_wakeup      :1;//�͹�����������жϻ��ѱ�־
        u32 call_fun_resent       :1;//1=�ڲ���λ/����������������غ��������ģ�0=�쳣����
        u32 power_on_resent_flag  :1;//�ϵ縴λ��־�����b18~19�Լ����ϵ��־�����ж�
    }runflag; //���б�־
    u64  reserved;//����
    u16  iboot_buildyear;    /* years since 1900 */
    u8   iboot_buildmon;     /* months since January [0-11] */
    u8   iboot_buildmday;    /* day of the month [1-31] */
    u8   iboot_buildhour;    /* hours since midnight [0-23] */
    u8   iboot_buildmin;     /* minutes after the hour [0-59] */
    u8   iboot_buildsec;     /* seconds after the minute [0-60] */
    u16  app_buildyear;    /* years since 1900 */
    u8   app_buildmon;     /* months since January [0-11] */
    u8   app_buildmday;    /* day of the month [1-31] */
    u8   app_buildhour;    /* hours since midnight [0-23] */
    u8   app_buildmin;     /* minutes after the hour [0-59] */
    u8   app_buildsec;     /* seconds after the minute [0-60] */
    u8   ibootVer_small;         //iboot �汾 xx.xx.__
    u8   ibootVer_medium;        //iboot �汾 xx.__.xx
    u8   ibootVer_large;         //iboot �汾 __.xx.xx
    u8   appVer_small;           //app �汾 xx.xx.__
    u8   appVer_medium;          //app �汾 xx.__.xx
    u8   appVer_large;           //app �汾 __.xx.xx
    char boardname[20];    //�����
    char update_path[40];    //�������ļ�·��
};

struct AppHead
{
    char djyflag[3];        //"djy"��־                    �̶���־
    u8   AppVer;            //��Ϣ��İ汾
    u32  filesize;          //�ļ�ϵͳ�������ļ���С   ��������ʱ ���ļ�ϵͳ������ʱ���ⲿ�������

    #define VERIFICATION_NULL     0
    #define VERIFICATION_CRC      1
    #define VERIFICATION_MD5      2
    #define VERIFICATION_SSL      3  //SSL��ȫ֤��
    u32  Verification;    //У�鷽��Ĭ��У�鷽��Ϊ��У�飬���ⲿ���߸��������޸�
    u32  appbinsize;      //app bin�ļ���С ���ⲿ�������
#if(CN_PTR_BITS < 64)
    u32  VirtAddr;        //���е�ַ
    u32  reserved32;      //����
#else
    u64  VirtAddr;        //���е�ַ
#endif
    u16  appheadsize;     //��Ϣ��Ĵ�С
    char VersionNumber[3];      //APP�汾xx.xx.xx��
    char reserved[3];          //����
    char appname[96];      //app���ļ��� ���ⲿ��������bin�ļ����ļ���
    char VerifBuf[128];     //У������У�鷽����Ӧ�ľ������� �ɹ������

//    const char ManufacturerName[65];  //�������ƣ� DIDE������
#if(CN_PTR_BITS < 64)
    u32  ManufacturerNameAddr;        //��������ַ
    u32  ManufacturerNamereserved32;      //����
#else
    u64  ManufacturerNameAddr;        //��������ַ
#endif
//    const char DjyosTag[6];          //"djyos"��
    const char ProductClassify[9];    //��Ʒ�����ַ�����������DIDE������
    const char ProductType[9];        //��Ʒ�ͺ��ַ�����������DIDE������
    const char TypeCode[6];           //��Ʒ�ͺ����ֱ���
    char ProductionTime[4];     //����ʱ�䣬BCD�룬��+���ڣ�3�ֽڣ�.Դ������FF������ʱ�������·���ibootд��
    char ProductionNumber[5];   //��Ʒ��ţ�Դ������FF������ʱ�������·���ibootд��
    char reserved8;             //����
    const char PanelType[16];   //����ͺ�
    const char CPU_Type[16];   //cpu�ͺ�
//    u32 ProductionNumber;   //Դ������FF������ʱ�������·���ibootд��
    char Reserved[182];         //����
};


//bool_t Set_RunIbootUpdateIboot();
bool_t Set_RebootFlag();
bool_t Set_SoftResetFlag();
bool_t Set_RestartAppFlag();
bool_t clear_resetflag();
bool_t Si_IbootAppInfoInit();
void Get_IbootAppInfo(struct IbootAppInfo *get_info);
//bool_t XIP_APPIsDebug(void );
bool_t Set_AppVerFlag(u8 small, u8 medium, u8 large);
bool_t Set_AppBulid_Time(u16 pyear,u8 pmon,u8 pday,u8 phour,u8 pmin,u8 psec);
bool_t Get_PowerOnResentFlag(void);
bool_t Get_CallFunResent();
bool_t Get_SoftResetFlag();
char Get_PowerOnFlag(void);
bool_t Get_HeadWdtReset(void);
bool_t Get_RebootFlag();
bool_t Get_RestartAppFlag();
bool_t Get_HeadResetFlag();
bool_t Get_LowPowerWakeup();

#if (CFG_RUNMODE_BAREAPP == 0)
bool_t Rewrite_AppHead_FileInfo(void * apphead,const char*name,u32 filesize);
bool_t Rewrite_AppHead_NumTime(void * apphead,const char* time,char *num);
bool_t XIP_AppFileCheck_Easy(void * apphead);
bool_t XIP_AppFileCheck(void * apphead);
void * XIP_GetAPPStartAddr(void * apphead);
u32  XIP_GetAPPSize(void * apphead);
u32 Get_AppSize(void * apphead);
char*  Get_AppName(void * apphead);
bool_t XIP_IsRamIbootFlag();
bool_t Fill_MutualUpdatePath(char* Path,int len);
bool_t Run_Iboot(enum runibootmode mode);
bool_t Run_App(enum runappmode mode);
bool_t Update_ToRun();
bool_t Set_PreviouResetFlag();
bool_t Set_RunIbootFlag();
bool_t Set_RunAppFlag();
bool_t Set_RunIbootUpdateApp();
bool_t Set_RunAppUpdateIboot();
bool_t Clear_RunIbootUpdateApp();
bool_t Clear_RunAppUpdateIboot();
bool_t Set_UpdateSource(char *param);
u32 Get_UpdateSource(void);
bool_t Get_RestartRunApp();
char Get_RunAppFormFile();
char Get_RunMode(void);
char Get_LastRunMode(void);
bool_t Get_HeardSetRunIboot(void);
char * Get_MutualUpdatePath(void);
bool_t Get_UpdateApp(void);
bool_t Get_Updateiboot(void);
bool_t Set_UpdateRunModet(u8 mode);
char Get_UpdateRunModet(void);
u32  XIP_GetAPPSize(void * apphead);
u32  Get_AppHeadSize(void);
bool_t Get_AppHead(void * apphead);
bool_t Get_ErrorAppCheck(void);
bool_t Get_ErrorAppNoFile(void);
bool_t Get_ErrorAppSize(void);
bool_t Get_HeardSetRunIboot(void);
char * Get_ProductSN(void);
char * Get_ProductVersion(void * apphead);
const char * Get_ManufacturerName(void);

bool_t  runiboot(char *param);
bool_t  runapp(char *param);
#endif
#endif /* __IICBUS_H__ */
