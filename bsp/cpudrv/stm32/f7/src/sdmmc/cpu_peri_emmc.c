//-----------------------------------------------------------------------------
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
// ������װԭʼ�뼰����λ��ִ����ʽ��Ȩ�������۴˰�װ�Ƿ񾭸�����Ȼ��
//
// 1. ���ڱ�����Դ�������ɢ�������뱣�������İ�Ȩ���桢�������б�����
//    ������������������
// 2. ���ڱ��׼�����λ��ִ����ʽ����ɢ���������������ļ��Լ�������������
//    ��ɢ����װ�е�ý�鷽ʽ����������֮��Ȩ���桢�������б����Լ�����
//    ������������

// �����������������Ǳ�������Ȩ�������Լ�����������״��"as is"���ṩ��
// ��������װ�����κ���ʾ��Ĭʾ֮�������Σ������������ھ��������Լ��ض�Ŀ
// �ĵ�������ΪĬʾ�Ե�������Ȩ�����˼�������֮�����ߣ������κ�������
// ���۳�����κ��������塢���۴�����Ϊ���Լ��ϵ���޹�ʧ������������Υ
// Լ֮��Ȩ��������ʧ������ԭ��ȣ����𣬶����κ���ʹ�ñ�������װ��������
// �κ�ֱ���ԡ�����ԡ�ż���ԡ������ԡ��ͷ��Ի��κν�����𺦣�����������
// �������Ʒ������֮���á�ʹ����ʧ��������ʧ��������ʧ��ҵ���жϵȵȣ���
// �����κ����Σ����ڸ���ʹ���ѻ���ǰ��֪���ܻ���ɴ����𺦵���������Ȼ��
//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stddef.h>
#include <fcntl.h>
#include <cpu_peri_gpio.h>
#include <stm32f7xx_hal.h>
#include <stdlib.h>
#include <device.h>
#include <fs/fat/port/drivers/fat_drivers.h>
#include <fs/fat/ff11/src/integer.h>
#include <fs/fat/ff11/src/diskio.h>
#include <stm32f7xx_hal_mmc.h>
#include <systime.h>

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�
//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//  extern s32 ModuleInstall_MMC(u32 dwArgC, ...);
//  if(0 != ModuleInstall_MMC(CFG_EMMC_ARGC,CFG_EMMC_NAME,CFG_EMMC_OPTION,CFG_EMMC_SPEED))
//  {
//      printf("cpu_peri_emmc init error ����");
//      while(1);
//  }
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_emmc"     //emmc����������
//parent:"devfile"                   //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                      //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable                   //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                     //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                    //��ʼ��ʱ������ѡֵ��early��medium��later��
                                     //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"devfile","lock"        //������������������������none����ʾ�������������
                                     //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                     //����������������������г�
//weakdependence:"none"              //��������������������������none����ʾ�������������
                                     //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                     //����������������������г����á�,���ָ�
//mutex:"none"                       //������������������������none����ʾ�������������
                                     //����������������������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header    //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_EMMC_ARGC   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_emmc �������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_EMMC_ARGC            3       //"EMMC��������",
#define CFG_EMMC_SPEED           0       //"EMMC�ٶ�",0���߷���
#define CFG_EMMC_OPTION          0       //"EMMC����",1��������������
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_EMMC_NAME            "emmc"    //"EMMC��ע������",
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������
//@#$%component end configure
// ============================================================================

s32 MMC_HardInit(void);
extern uint32_t SDMMC_CmdSendStatus(SDMMC_TypeDef *SDMMCx, uint32_t Argument);
extern const Pin MMC[6]; // ��board.c֮�ж���

MMC_HandleTypeDef handleMMC;
static u32 SDMMC_BUS_CLK_DIV;
void *pOperationMutex;
// ============================================================================
// ���ܣ���ʼ��EMMC IO
// ������
// ���أ�
// ��ע��IO����
// ============================================================================
void HAL_MMC_MspInit(MMC_HandleTypeDef *hmmc)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
//  __HAL_RCC_GPIOG_CLK_ENABLE();
//  __HAL_RCC_GPIOH_CLK_ENABLE();
    PIO_Configure(MMC, PIO_LISTSIZE(MMC));

    PIO_Set(&MMC[6]); // SDMMC1_RSTn����
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��IO����
// ============================================================================
void HAL_MMC_MspDeInit(MMC_HandleTypeDef *hmmc)
{
    ;
}

// ============================================================================
// ���ܣ�EMMC����sleepģʽ
// ������
// ���أ�
// ��ע��Ŀǰδʹ��
// ============================================================================
void EMMC_Sleep(void)
{
    u32 res;
    extern u32 EMMC_CMD_Sleep(SDMMC_TypeDef *SDMMCx, u32 wRCA);
    extern uint32_t SDMMC_CmdSelDesel(SDMMC_TypeDef *SDMMCx, uint64_t Addr);

    SDMMC_CmdSelDesel(handleMMC.Instance, 0);

    res = EMMC_CMD_Sleep(handleMMC.Instance, handleMMC.MmcCard.RelCardAdd);
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : entry sleep mode failed.\r\n");
    }
}

// ============================================================================
// ���ܣ�EMMC�˳�sleepģʽ
// ������
// ���أ�
// ��ע��Ŀǰδֻ��
// ============================================================================
void EMMC_Wakeup(void)
{
    u32 res;
    extern u32 EMMC_CMD_Awake(SDMMC_TypeDef *SDMMCx, u32 wRCA);
    extern uint32_t SDMMC_CmdSelDesel(SDMMC_TypeDef *SDMMCx, uint64_t Addr);

    res = EMMC_CMD_Awake(handleMMC.Instance, handleMMC.MmcCard.RelCardAdd);
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : awake failed.\r\n");
        return ;
    }

    Djy_EventDelay(1000); // �ȴ�����

    res = SDMMC_CmdSelDesel(handleMMC.Instance, (uint32_t)(((uint32_t)handleMMC.MmcCard.RelCardAdd) << 16));
    if(res != HAL_MMC_ERROR_NONE)
    {
        printk("EMMC : error : select failed(entry transfer mode).\r\n");
    }
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int _MMC_Status(void)
{
    return (0);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 _MMC_Initialize(void)
{
    return (0);
}

// ============================================================================
// ���ܣ�MMC��
// ������
// ���أ�
// ��ע��
// ============================================================================
int _MMC_Read(BYTE *pBuf, DWORD qwSector, UINT dwCount)
{
    HAL_StatusTypeDef res;
    u32 response;
    u32 block = (u32)qwSector;
    u8 *buf = (u8*)pBuf;
    u32 count = (u32)dwCount;
    int ret = 0;

    Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);

    res = HAL_MMC_ReadBlocks(&handleMMC, buf, block, count, 0xFFFF);
    if(HAL_OK != res)
    {
        printk("\r\nEMMC : debug : read <%xH> <%xH> <%xH> <%xH>\r\n", block, count, res, handleMMC.ErrorCode);
        ret = -1;
    }
    else
    {   // ��ѯ�豸��ɣ����Ƿ����
        while(1)
        {
            Djy_EventDelay(1);
            if(count++ == SDMMC_MAX_TRIAL)
            {
                ret = -1;
                printk("\r\nEMMC : debug : read timeout\r\n");
                break;
            }

            res = SDMMC_CmdSendStatus(handleMMC.Instance, (u32)(handleMMC.MmcCard.RelCardAdd << 16));
            if(HAL_MMC_ERROR_NONE != res)
            {
                ret = -1;
                printk("\r\nEMMC : debug : read status <%xH>\r\n", res);
                break;
            }

            response = SDMMC_GetResponse(handleMMC.Instance, SDMMC_RESP1);
            if((response & 1 << 8) && ((response & (0xF << 9)) != (0x7 << 9)))
                break;
        }
    }

    Lock_MutexPost(pOperationMutex);
    return (ret);
}

// ============================================================================
// ���ܣ�MMCд
// ������
// ���أ�
// ��ע��
// ============================================================================
int _MMC_Write(BYTE *pBuf, DWORD qwSector, UINT dwCount)
{
    HAL_StatusTypeDef res;
    u32 response;
    u32 block = (u32)qwSector;
    u8 *buf = (u8*)pBuf;
    u32 count = (u32)dwCount;
    int ret = 0;

    Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);

    res = HAL_MMC_WriteBlocks(&handleMMC, buf, block, count, 0xFFFF);
    if(HAL_OK != res)
    {
        printk("\r\nEMMC : debug : write <%xH> <%xH> <%xH> <%xH>\r\n", block, count, res, handleMMC.ErrorCode);
        ret = -1;
    }
    else
    {
        // ��ѯ�豸�Ƿ�ready�����Ƿ����
        while(1)
        {
            Djy_EventDelay(1);
            if(count++ == SDMMC_MAX_TRIAL)
            {
                ret = -1;
                printk("\r\nEMMC : debug : write timeout\r\n");
                break;
            }

            res = SDMMC_CmdSendStatus(handleMMC.Instance, (u32)(handleMMC.MmcCard.RelCardAdd << 16));
            if(HAL_MMC_ERROR_NONE != res)
            {
                ret = -1;
                printk("\r\nEMMC : debug : write status <%xH>\r\n", res);
                break;
            }

            response = SDMMC_GetResponse(handleMMC.Instance, SDMMC_RESP1);
            if((response & 1 << 8) && ((response & (0xF << 9)) != (0x7 << 9)))
                break;
        }
    }

    Lock_MutexPost(pOperationMutex);
    return (ret);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
int _MMC_Ioctl( BYTE bCommand, void *pReturn)
{
    u8 command = (u8)bCommand;

    switch(command)
    {
    case GET_SECTOR_SIZE:
        *((DWORD *)pReturn) = handleMMC.MmcCard.BlockSize; //
        break;

    case GET_SECTOR_COUNT:
        *((DWORD*)pReturn) =  handleMMC.MmcCard.BlockNbr; //
        break;

    case GET_BLOCK_SIZE:
        *((DWORD*)pReturn) = 1; // TODO
        break;

    default:
       break;
    }

    return (RES_OK);
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
struct FatDrvFuns MMC_Drv =
{
    .DrvStatus     = _MMC_Status,
    .DrvInitialize = _MMC_Initialize,
    .DrvRead       = _MMC_Read,
    .DrvWrite      = _MMC_Write,
    .DrvIoctl      = _MMC_Ioctl
};

// ============================================================================
// ���ܣ�MMC��ʼ��
// ������
// ���أ�
// ��ע��
// ============================================================================
s32 MMC_HardInit(void)
{
    s32 res;

    handleMMC.Instance = SDMMC1;
    handleMMC.Init.ClockEdge           = SDMMC_CLOCK_EDGE_RISING;
    handleMMC.Init.ClockBypass         = SDMMC_CLOCK_BYPASS_DISABLE;
    handleMMC.Init.ClockPowerSave      = SDMMC_CLOCK_POWER_SAVE_DISABLE;
    handleMMC.Init.BusWide             = SDMMC_BUS_WIDE_4B;
    handleMMC.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
    handleMMC.Init.ClockDiv            = SDMMC_BUS_CLK_DIV;

    res = (s32)HAL_MMC_Init(&handleMMC);

    return (res);
}

// ============================================================================
// ���ܣ���װMMC���豸
// ������dwArgC -- ������������ѡ���� -- �豸�����豸�������ٶ�ģʽ��
// ���أ�0 -- �ɹ��� ���� -- ʧ�ܣ�
// ��ע��
// ============================================================================
s32 ModuleInstall_MMC(u32 dwArgC, ...)
{
    s32 res;
    va_list ap;
    u32 options = 0; // �豸����ѡ��;
    char *name = NULL; // �豸��;
    const char *defaultName = "emmc"; // �豸ȱʡ��;
    u32 speed = 0;
    u8 i;

    va_start(ap, dwArgC);
    for(i = 0; i < dwArgC; i++)
    {
        switch(i)
        {
            case 0 : name = va_arg(ap, char*); break;
            case 1 : options = va_arg(ap, u32); break;
            case 2 : speed = va_arg(ap, u32); break;
            default : break;
        }
    }
    va_end(ap);

    if(!name)
        name = (char*)defaultName;

    if(speed)
        SDMMC_BUS_CLK_DIV = 0;
    else
        SDMMC_BUS_CLK_DIV = 1;

    pOperationMutex = (void*)Lock_MutexCreate("MMC");
    if(!pOperationMutex)
    {
        printf("\r\nMODULE : INSTALL : \"%s\" create mutex failed.\r\n", name);
        return (-1);
    }

    res = MMC_HardInit();
    if(res)
    {
        printk("\r\nMODULE : INSTALL : \"%s\" initialization failed.\r\n", name);
        return (-1);
    }

    res = dev_add(NULL, (const char*)name, NULL, NULL, NULL, NULL,NULL, ((ptu32_t)&MMC_Drv));
    if(res)
    {
        printk("\r\nMODULE : INSTALL : \"%s\" initialization failed.\r\n", name);
        return (-1); // ʧ��
    }

    if(1 == options) // ���������豸
    {
        res = HAL_MMC_Erase(&handleMMC, 0, handleMMC.MmcCard.LogBlockNbr-1);
        if(res)
        {
            printk("\r\nMODULE : INSTALL : \"%s\" format failed during initialization.\r\n", name);
        }
        Djy_EventDelay(3000000); // �ȴ�һ��ʱ��
    }

    return (res);
}
// ============================================================================
// ���ܣ����������豸
// ������dwArgC -- ������������һ������
// ���أ�
// ��ע��
// ============================================================================
s32 MMC_Erase(u32 dwArgC, ...)
{
    u32 range = -1, i;
    s32 res = -1;
    va_list ap;

    va_start(ap, dwArgC);
    for(i = 0; i < dwArgC; i++)
    {
        switch(i)
        {
            case 0 : range = va_arg(ap, u32); break;
            default : break;
        }
    }
    va_end(ap);

    if(-1 == range) // ���������豸
    {
        Lock_MutexPend(pOperationMutex, CN_TIMEOUT_FOREVER);
        res = HAL_MMC_Erase(&handleMMC, 0, handleMMC.MmcCard.LogBlockNbr-1);
        if(res)
        {
            printk("\r\nEMMC : debug : erase failed.\r\n");
        }
        Lock_MutexPost(pOperationMutex);
        Djy_EventDelay(3000000); // �ȴ�һ��ʱ��
    }

    return (res);
}

#if 1
// ============================================================================
// ���ܣ����ز�������
// ������
// ���أ�
// ��ע��
// ============================================================================
#include <string.h>
void LOCAL_Test(void)
{
    u8 buf[512];
    u8 check[512];
    s32 res;
    u32 i, block;
    u32 error = 0;
    u32 error2 = 0;

    res = ModuleInstall_MMC(0);
    if(res)
    {
        printk("\r\nLOCAL TEST : install failed.\r\n");
        while(1);
    }

    printk("\r\nLOCAL TEST : start to run...\r\n");
    printk("LOCAL TEST : strategy No.1\r\n");
    for(block = 0; block <  handleMMC.MmcCard.BlockNbr; block++)
    {
        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if(0xFF != check[i])
                break;
        }

        if(512 > i)
        {
            for(i = 0; i < 512; i++)
            {
                buf[i] = 0xFF;
            }

            res = _MMC_Write(buf, block, 1);
            if(res)
            {
                printk("LOCAL TEST : write %d failed.\r\n", block);
                error++;
                continue;
            }

            memset(check, 0x0, 512);
            res = _MMC_Read(check, block, 1);
            if(res)
            {
                printk("LOCAL TEST : read %d failed.\r\n", block);
                error++;
                continue;
            }

            for(i = 0; i < 512; i++)
            {
                if(0xFF != check[i])
                {
                    printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                    printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0xFF.\r\n", i, check[i]);
                    goto NEXT_BLOCK;
                }
            }
        }

        for(i = 0; i < 512; i++)
        {
            buf[i] = i;
        }

        res = _MMC_Write(buf, block, 1);
        if(res)
        {
            printk("LOCAL TEST : write %d failed.\r\n", block);
            error++;
            continue;
        }

        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error2++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if((u8)i != check[i])
            {
                printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0x%02x.\r\n", i, check[i], (u8)i);
                error2++;
                goto NEXT_BLOCK;
            }

        }

    NEXT_BLOCK:
        continue;

    }

    printk("LOCAL TEST : strategy No.2\r\n");
    for(block = 0; block <  handleMMC.MmcCard.BlockNbr; block++)
    {
        memset(check, 0x0, 512);
        res = _MMC_Read(check, block, 1);
        if(res)
        {
            printk("LOCAL TEST : read %d failed.\r\n", block);
            error++;
            continue;
        }

        for(i = 0; i < 512; i++)
        {
            if((u8)i != check[i])
            {
                printk("LOCAL TEST : read & write are not the same in block %d.\r\n", block);
                printk("LOCAL TEST : offset <%d>, data <%xH>, should be 0x%02x.\r\n", i, check[i], (u8)i);
                break;
            }

        }
    }

    {
    Djy_EventDelay(30000000);
    printk("LOCAL TEST : result : strategy 1 has %d errors\r\n", error);
    printk("LOCAL TEST : result : strategy 2 has %d errors\r\n", error2);

    }

}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
void LOCAL_Test2(void)
{
    s32 res;
    s32 fd;
    u8 buf[512], check[512];
    u32 i;
#if 0
    res = ModuleInstall_MMC(0);
    if(res)
    {
        printk("\r\nLOCAL TEST : install failed.\r\n");
        while(1);
    }

    res = ModuleInstall_FAT("/dev/emmc", 1);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT install failed.\r\n");
        while(1);
    }
#endif
    for(i = 0; i < 512; i++)
    {
        buf[i] = i;
    }


    fd = open("/fat/test1", O_CREAT | O_RDWR);
    if(-1 == fd)
    {
        printk("\r\nLOCAL TEST : FAT open failed.\r\n");
        while(1);
    }

    res = write(fd, buf, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT write failed.\r\n");
        while(1);
    }

    res = write(fd, buf, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT write failed.\r\n");
        while(1);
    }

    res = close(fd);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT close failed.\r\n");
        while(1);
    }

    fd = open("/fat/test1", O_CREAT | O_RDWR);
    if(-1 == fd)
    {
        printk("\r\nLOCAL TEST : FAT open failed.\r\n");
        while(1);
    }

    res = read(fd, check, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT read failed.\r\n");
        while(1);
    }

    for(i = 0; i < 512; i++)
    {
        if(buf[i] != check[i])
        {
            printk("\r\nLOCAL TEST : FAT write & read failed, %d, %xH, %xH.\r\n", i, buf[i], check[i]);
            while(1);
        }
    }

    res = read(fd, check, 512);
    if(512 != res)
    {
        printk("\r\nLOCAL TEST : FAT read failed.\r\n");
        while(1);
    }

    for(i = 0; i < 512; i++)
    {
        if(buf[i] != check[i])
        {
            printk("\r\nLOCAL TEST : FAT write & read failed, %d, %xH, %xH.\r\n", i, buf[i], check[i]);
            while(1);
        }
    }

    res = close(fd);
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT close failed.\r\n");
        while(1);
    }

    res = remove("/fat/test1");
    if(res)
    {
        printk("\r\nLOCAL TEST : FAT remove failed.\r\n");
        while(1);
    }

}
#endif