//-----------------------------------------------------------------------------
// Copyright (c) 2018, Djyos Open source Development team. All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 3. As a constituent part of djyos,do not transplant it to other software
//    without specific prior written permission.

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
// �����Ȩ�����ʹ���߷��������������������£�����ʹ����ʹ�ü���ɢ����
// �����װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б���
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б��Լ�����
//    ������������
// 3. �������Ϊ�����߲���ϵͳ����ɲ��֣�δ����ǰȡ�õ�������ɣ���������ֲ����
//    �����߲���ϵͳ���������С�

// ����������������Ǳ������Ȩ�������Լ�����������״��"as is"���ṩ��
// �������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
#if 0
#include <stdio.h>
#include <string.h>
#include <djyos.h>
#include <wdt_soft.h>
#include <dirent.h>
#include <stdlib.h>
#include <blackbox.h>

#define RW_SIZE                      512
#define APPLICATION_FILE_NAME        "PRS-7573.bin"

typedef enum{
    INIT = 0,
    RUN,
    DONE,
    ERROR
}USB_IAP_STATUS;

static USB_IAP_STATUS gRunningStatus;
static u32 dwDeamonInspect;
#define __IAP_USB_BUFFER_SIZE                         (1024*8)
//extern void USB_UDiskReset(void);
extern void Amend_IBootFlag_RunIBoot(void);
extern void Amend_IBootFlag_RunAPP(void);
extern enStartMode GetBootMethodSoft(void);
extern s32 MSC_DeviceReady(u8 bID);
extern bool_t  Sh_RunAPP(char *param);
extern bool_t HAL_ClrUpdateFlag(void);
extern bool_t HAL_SetUpdateFlag(void);
extern bool_t HAL_ClrUpdateFlag(void);
extern void RunIboot(void);
extern void CPU_Reset(u32 key);
extern s32 IAP_SetMethod(u32 dwMethod);
extern s32 IAP_SetPath(char *pPath);
extern u32 IAP_GetMethod(void);
extern char *IAP_GetPath(void);
const tagVmMemItem *gMemTab[]={
    {0x20000000,(512-32)*1024},\
    NULL,\
};

// ============================================================================
// ���ܣ� ����
// ������
// ���أ�
// ��ע�� ������ʵ��,TODO,��Ϊfcntl�߼���
// ============================================================================
__attribute__((weak)) void USB_IAP_TurnOnLED(void)
{
    ;
}

// ============================================================================
// ���ܣ� ����
// ������
// ���أ�
// ��ע�� ������ʵ��,TODO,��Ϊfcntl�߼���
// ============================================================================
__attribute__((weak)) void USB_IAP_TurnOffLED(void)
{
    ;
}


// ============================================================================
// ���ܣ� ����
// ������
// ���أ�
// ��ע�� ������ʵ��,TODO,��Ϊfcntl�߼���
// ============================================================================
__attribute__((weak)) void USB_UDiskReset(void)
{
    ;
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע�� ����500΢�뱻����һ��
// ============================================================================
u32 USB_IAP_StatusDeamon(struct Wdt *reg)
{
    static u8 toggle;

    switch(gRunningStatus)
    {
    case INIT:
            USB_IAP_TurnOffLED();

            break;

    case RUN:
            // 500΢�����ڽ�������
            if(toggle)
            {
                toggle = 0;
                USB_IAP_TurnOnLED();
            }
            else
            {
                toggle = 1;
                USB_IAP_TurnOffLED();
            }

            if(dwDeamonInspect++ > 20)
            {
                dwDeamonInspect = 0;
                printf("\r\nIAP : DEAMON : unknown state during USB update, reset all.\r\n");
                DJY_EventDelay(1000000);
                CPU_Reset(CN_BOOT_LEGALKEY);
            }
            break;

    case DONE:
            // ����
            USB_IAP_TurnOnLED();
            break;

    case ERROR:
            // һ�����̵�����
            if(toggle < 4) USB_IAP_TurnOnLED();
            else if(toggle < 5) USB_IAP_TurnOffLED();
            else if(toggle < 6) USB_IAP_TurnOnLED();
            else if(toggle < 7) USB_IAP_TurnOffLED();
            else if(toggle < 8) USB_IAP_TurnOnLED();
            else if(toggle < 9) USB_IAP_TurnOffLED();
            else if(toggle < 10) toggle = 0;

            toggle++;
//          �������10�룬��Ϊ�����������У��ε�U�̵Ļ���USBЭ��ջ����10�����������ݷŵ磬��Ҫ�����ʱ�����USB�������½���IDLE״̬��
//          ���������10�룬��USBЭ��ջ���10����ʱ����֮����������HUB��
//          ���û����10�룬������HUB�Ļ�����ô��HUB�����ʱ���жϼ�⵽HUB�Ͽ���USBЭ��ջ���ڵȴ���10��ĵ��ݷŵ磬
//          USBЭ��ջ��û���ü���USBģ�����IDLE״̬������ʱHUB�ϵ�Ļ����жϼ�⵽�˿�״̬�����仯���������ӡ�����USBģ���״̬����disconnected
//          ״̬��δ����IDLE״̬����ʱ����HUB��ʧ�ܡ�������Ҫ�ڶ�������HUB���������ɹ���
            if(dwDeamonInspect++ > 20)
            {
                dwDeamonInspect = 0;
                if(!MSC_DeviceReady(0))
                {
                    printf("\r\nIAP : DEAMON : unknown state during USB update, reset all.\r\n");
                    DJY_EventDelay(1000000);
                    USB_UDiskReset();
                }
                else
                {
                    printf("\r\nIAP : DEAMON : Enter the error state and do not detect the usb drive.\r\n");
                }
            }
            break;
    default:
        break;
    }

    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void USB_IAP_StatusInit(void)
{

    if(!Wdt_Create("usb iap status", 500000, USB_IAP_StatusDeamon, EN_BLACKBOX_DEAL_IGNORE, 0, 0))
    {
        printf("\r\n: erro : iapusb : status initial failed\r\n");
        gRunningStatus = ERROR;
        do
        {
            USB_IAP_StatusDeamon(NULL);   //������ñ����ѭ���ɣ���һ��û����ɹ������������������

        }while(1);
    }

    gRunningStatus = INIT;
}
// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 __UpdateThroughUSB(char *pAPP, u32 dwOpt)
{
    DIR *dir;
    u8 updatestate = 1;
    struct dirent *structDirent;
    char *path;
    char *buffer;
    u32 bit[2];
    s32 res;
    struct stat sourceInfo;
    u8 i = 0;
    u8 found = 0;
    s32 source = -1;
    s32 destination = -1;
    u32 resetflag = 0;
    char *program = pAPP;

    u32 dbgCount = 0;

    for(i=0; i<2; i++)
    {
        bit[i] = dwOpt & 0x1;
        dwOpt = dwOpt >> 1;
    }

    gRunningStatus = INIT;

    do{
        if(!MSC_DeviceReady(0))
            break; // USB�豸��׼����

        DJY_EventDelay(1000000); // 1s��һ��

        if((bit[0]) && (++i > 60))
        {
            if(resetflag)
            {
                printf("\r\n: info : iapusb : no U disk was found, switch to application mode in 10 seconds.\r\n\r\n\r\n\r\n\r\n");
                DJY_EventDelay(10000000); // ��ʱ10s���Ա�֤��ӡ�����
                CPU_Reset(CN_BOOT_LEGALKEY);
                return (-1); // δ��⵽USB�豸��
            }
            resetflag = 1;
            i = 0;
            USB_UDiskReset();
        }
    }while(1);

    buffer = malloc(__IAP_USB_BUFFER_SIZE);
    if(!buffer)
    {
        printf("\r\nIAP : USB : memory out.");
        return (-1);
    }

    gRunningStatus = RUN;
    dwDeamonInspect = 0;
    dir = opendir("/fat");
    if(!dir)
    {
        printf("\r\n: info : iapusb : directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");
        gRunningStatus = ERROR;
        return (-1);
    }

    dwDeamonInspect = 0;
    while((structDirent = readdir(dir)) != NULL)
    {
        if(0 == strcmp(structDirent->d_name, program))
        {
            printf("\r\n: info : iapusb : file \"%s\" will be programmed.",
                    structDirent->d_name);
            found = 1;
            break;
        }
    }

    if(!found)
    {
        printf("\r\n: info : iapusb : file \"%s\" is not found.", program);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    path = buffer;
    strcpy(path, "/fat/");
    strcpy(path+5, structDirent->d_name);
    dwDeamonInspect = 0;
    source = open(path, O_RDONLY);
    if(-1 == source)
    {
        printf("\r\n: info : iapusb : cannot open source file \"%s\".", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    dwDeamonInspect = 0;
    res = fstat(source, &sourceInfo);
    if(res)
    {
        printf("\r\n: info : iapusb : cannot statistics source file \"%s\".", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    strcpy(path, "/iboot/");
    strcpy(path+7, structDirent->d_name);
    dwDeamonInspect = 0;
    destination = open(path, O_RDWR | O_CREAT);
    if(-1 == destination)
    {
        printf("\r\n: info : iapusb : cannot open destination file \"%s\"!", path);
        gRunningStatus = ERROR;
        updatestate = 0;
        goto __UPGRADE_ERROR;
    }

    printf("\r\nIAP : USB : application is updating   ");
    while(1)
    {
        dwDeamonInspect = 0;
        dbgCount++;
        if(9 == dbgCount)
        {
            DJY_EventDelay(1000);
        }

        res = read(source, buffer, __IAP_USB_BUFFER_SIZE);
        if((!res) || (-1 == res))
        {
            printf("\b\b\b error<read source, %d>.", dbgCount);
            dwDeamonInspect = 0;
            gRunningStatus = ERROR;
            updatestate = 0;
            break; // û�����ݿɶ�
        }

        sourceInfo.st_size -= res; // ʣ��

        dwDeamonInspect = 0;
        if(res != write(destination, buffer, res))
        {
            printf("\b\b\b error<write destination>.");
            gRunningStatus = ERROR;
            updatestate = 0;
            break;
        }

        if(!sourceInfo.st_size)
        {
            printf("\b\b\b\b\b\bed successfully.");
            gRunningStatus = DONE;
            HAL_ClrUpdateFlag();
            break; // ȫ������
        }

        switch(i)
        {
        case 0 : printf("\b\b\b.  "); i = 1; break;
        case 1 : printf("\b\b\b.. "); i = 2; break;
        case 2 : printf("\b\b\b..."); i = 0; break;
        default: i = 0;break;
        }

    }

__UPGRADE_ERROR:
    closedir(dir);
    printf("\r\n: info : iapusb : thread exit ...");
    if(-1 != source)
    {
        res = close(source);
        if(res)
        {
            printf("\r\n: info : iapusb : close source file failed.");
        }
    }

    if(-1 != destination)
    {
        res = close(destination);
        if(res)
        {
            printf("\r\n: info : iapusb : close destination file failed.");
        }
    }

    printf("\r\n");

    if((bit[1]) && (gRunningStatus == DONE))
    {
        printf("\r\nIAP : USB : application update succeed, switch to application mode in 10 seconds.\r\n\r\n\r\n\r\n\r\n");
        DJY_EventDelay(10000000); // ��ʱ10s���Ա�֤��ӡ�����
        Sh_RunAPP(NULL);
        return (0); // δ��⵽USB�豸
    }
    if(updatestate)
    {
        return (0);
    }
    else
    {
        return (-1);
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void UpdateThroughUSB_T(void)
{
    char *file;
    u32 bits;

    DJY_GetEventPara((ptu32_t*)&file, (ptu32_t*)&bits);

    __UpdateThroughUSB(file, bits);
}

// ============================================================================
// ���ܣ�����U���Ƿ���ϣ�������϶���U�����������ļ����IbootFlag�޸�ΪRuniboot,����IbootFlagΪRunapp
//     ���Ұ������ķ�ʽ��Ŀ���ļ��������Iboot
// ������
// ���أ�
// ��ע��
// ============================================================================
void __SniffUSB(char *pFile,u32 dwOpt)
{
    DIR *dir;
    u8 UDisk_Flag = 1;
    u32 methodBackup;
    char *pathBackup;
    struct dirent *structDirent;
    char *program = pFile;

    methodBackup = IAP_GetMethod();
    pathBackup = IAP_GetPath();
    HAL_ClrUpdateFlag();
    while(1)
    {
        if((UDisk_Flag == 1) && (!MSC_DeviceReady(0)))
        {
            Amend_IBootFlag_RunAPP();
            dir = opendir("/fat");
            if(!dir)
            {
                printf("USB : directory \"fat\" is not exist! maybe \"FAT\" is not installed!\r\n");
            }

            while((!MSC_DeviceReady(0)) && (structDirent = readdir(dir)) != NULL)
            {
                if(0 == strcmp(structDirent->d_name, program))
                {
                    HAL_SetUpdateFlag();
                    IAP_SetMethod(3);
                    IAP_SetPath(pFile);
                    UDisk_Flag = 0;
                    if(dwOpt == 1)
                    {
                        ModuleInstall_OsBoot(gMemTab,NULL,GetBootMethodSoft);
                        Amend_IBootFlag_RunIBoot();
                        printf("\r\nIf you need to upgrade, please enter \"reset 0xaa55aa55\".\r\n");
                    }
                    else
                    {
                        RunIboot();
                    }

                    break;
                }
            }

        }

        if((UDisk_Flag == 0) && (MSC_DeviceReady(0) != 0))
        {
            // û��ִ�������߼���������״̬��ԭ��
            Amend_IBootFlag_RunAPP();
            HAL_ClrUpdateFlag();
            IAP_SetMethod(methodBackup);
            IAP_SetPath(pathBackup);
            UDisk_Flag = 1;
        }

        DJY_EventDelay(1000000);
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void SniffUSB_T(void)
{
    char *file;
    u32 bits;

    DJY_GetEventPara((ptu32_t*)&file, (ptu32_t*)&bits);

    __SniffUSB(file,bits);
}

// ============================================================================
// ���ܣ���װU����������
// ������bArgC -- ����������
//     ��1������ -- ����ѡ�1�� USB������0��USB���ӣ���
//     ��2������ -- ��������ӵ��ļ�����
//     ��3������ -- USB���ӹ��ܣ�1(��ʾ��Ҫ�ֶ����������ִ������)��0����ʾ����Ҫ�ֶ��������������U�̺���Զ�ִ����������
//              USB�������ܣ�bit1(��ʾiboot60��û��⵽U�̵Ļ�������)��bit2(��ʾ������ɺ��Զ�����APP)��
// ���أ� �ɹ���0��; ʧ�ܣ�-1��;
// ��ע��
// ============================================================================
s32 ModuleInstall_USB_IAP(u8 bArgC, ...)
{
    u8 i;
    u16 thread;
    va_list ap;
    u32 arg = 0, func = 0;
    char *file = NULL;
    const char *func0 = "IAP: sniff USB";
    const char *func1 = "IAP: update through USB";

    va_start(ap, bArgC);
    for(i = 0; i < bArgC; i++)
    {
        switch(i)
        {
            case 0: func = va_arg(ap, u32);break;
            case 1: file = (char*)va_arg(ap, u32);break;
            case 2: arg = va_arg(ap, u32);break;
            default: break;
        }
    }
    va_end(ap);

    if(0 == func)
    {
        thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                SniffUSB_T, NULL, 0x1000, (char*)func0);

        if(thread != CN_EVTT_ID_INVALID)
        {
            DJY_EventPop(thread, NULL, 0, (ptu32_t)file, arg, 0);
            return (0);
        }
    }
    else if(1 == func)
    {
        USB_IAP_StatusInit();
        thread = DJY_EvttRegist(EN_CORRELATIVE, CN_PRIO_RRS, 0, 0,
                                UpdateThroughUSB_T, NULL, 0x1000, (char*)func1);

        if(thread != CN_EVTT_ID_INVALID)
        {
            DJY_EventPop(thread, NULL, 0, (ptu32_t)file, arg, 0);
            return (0);
        }
    }

    return (-1);
}
#endif
