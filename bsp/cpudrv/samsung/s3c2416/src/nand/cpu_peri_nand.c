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
//-----------------------------------------------------------------------------

//˼·:
//   ����һ��buf��ʾҳ��oob����,�����ǽ����Ƿֿ���ʾ��ԭ���Ƕ���������ʱ,ECCУ��
//   ȴ����Ҫ����ҳ��OOB���ݡ���˴�ECCУ��ʱ,ÿ�ζ����д������ҳ��OOB�����ݲ���
//

#include <string.h>
#include <stdlib.h>
#include <device.h>
#include <device/djy_flash.h>
#include <cpu_peri.h>
#include <djyos.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>
#include <dbug.h>
#include <djyfs/filesystems.h>
#include <math.h>
#include "stm32f7xx_hal_conf.h"

#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//   extern s32 ModuleInstall_NAND(u32 doformat);
//   ModuleInstall_NAND(CFG_NFLASH_PART_FORMAT);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu onchip nand"//CPU��nand����
//parent:"none"                 //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp                 //ѡ�third��system��bsp��user����������������IDE�з���
//select:choosable              //ѡ�required��choosable��none���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early               //��ʼ��ʱ������ѡֵ��early��medium��later, pre-main��
                                //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"device file system","file system"//������������������������none����ʾ�������������
                                //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                //����������������������г�
//weakdependence:"none"         //��������������������������none����ʾ�������������
                                //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                //����������������������г����á�,���ָ�
//mutex:"none"                  //������Ļ����������������none����ʾ�޻����������
                                        //�������������⣬�������г�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
#if ( CFG_MODULE_ENABLE_CPU_ONCHIP_NAND == false )
//#warning  " cpu_onchip_nand  �������δ���ã�ʹ��Ĭ������"
//%$#@target = header   //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#define CFG_MODULE_ENABLE_CPU_ONCHIP_NAND    false //�����ѡ�˱����������DIDE��project_config.h���������ж���Ϊtrue
//%$#@enum,true,false,
#define CFG_NFLASH_PART_FORMAT     false      //�Ƿ���Ҫ������оƬ��
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���

//%$#@exclude       ****�����ų��ļ��б�
//%$#@end exclude   ****�����������

//@#$%component end configure

//����ķ�����ʼ�ͷ���������д����-1��10000��������ֵ�ɸ��ݾ���оƬ������д

#define NAND_CONTROLLER_REG                 (pg_nand_reg)
#define NandChipOn()                        (NAND_CONTROLLER_REG->NFCONT &= ~(1<<1))
#define NandChipOff()                       (NAND_CONTROLLER_REG->NFCONT |= (1<<1))
#define SetNandCommand(cmd)                 (NAND_CONTROLLER_REG->NFCMD = cmd)
#define SetNandAddr(i, addr)                (NAND_CONTROLLER_REG->NFADDR = addr)
#define SetNandData(data)                   (NAND_CONTROLLER_REG->NFDATA = data)
#define GetNandData()                       (NAND_CONTROLLER_REG->NFDATA)

s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...);
s32 __nand_read(s64 unit, void *data, struct uopt opt);
s32 __nand_write(s64 unit, void *data, struct uopt opt);
s32 __nand_erase(s64 unit, struct uesz sz);
static u32 *badstable = NULL;
static u32 badslocation = 0;
static bool_t sNandflashInited = false;
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10
extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);
extern struct Object *s_ptDeviceRoot;
static struct NandDescr *s_ptNandInfo;
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static const char *NandFlashName = "nand";      //��flash��obj�ڵ�����

static void ResetNand(void);
static s32 StatusOfNand(void);
static void WaitNandReady(void);
//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- д�ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
s32 S3C2416_SpareProgram(u32 PageNo, const u8 *Data)
{

    u32 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    NandChipOn();

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)SpareOffset;
    NAND_CONTROLLER_REG->NFADDR = (u8)((SpareOffset>>8) & 0xF);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)((PageNo>>16) & 0x1);

    for(i = 0; i < (s_ptNandInfo->OOB_Size); i++)
        SetNandData(Data[i]);

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE2);// д��Main�������

    DJY_EventDelay(700);// �г�
    WaitNandReady();//ʱ��Ҫ��

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//���: ">0" -- ���ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
s32 S3C2416_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    NandChipOn();

    SetNandCommand(PAGE_READ_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)(SpareOffset);
    NAND_CONTROLLER_REG->NFADDR = (u8)((SpareOffset>>8) & 0xF);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    SetNandCommand(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);// �г�
    WaitNandReady();//ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = GetNandData();

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->OOB_Size);
}

//-----------------------------------------------------------------------------
//����:
//����: Data -- ���ݿռ䡣����ΪNULL��
//      Flags --
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32  S3C2416_PageProgram(u32 PageNo, const u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i, EccOffset;
    s32 Ret;
    u8 *Spare;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    // ECC ����
    Ret = NAND_CONTROLLER_REG->NFMECC0;//����
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT &= (~(1<<7));// ����(����)Main ECC
            NAND_CONTROLLER_REG->NFCONT |= (1<<5);// ��ʼ��Main ECC
            break;
        }
        case SW_ECC :
        case NO_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT |= (1<<7);// �ر�Main ECC
            break;
        }
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }

    NandChipOn();

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        SetNandData(Data[i]);

    SetNandCommand(PAGE_PROGRAM_CMD_BYTE2);// д��Main�������

    DJY_EventDelay(700);// �г�
    WaitNandReady();// ʱ��Ҫ��

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (s_ptNandInfo->BytesPerPage);// ֻдҳ,�����˳�
    }

    Spare = (u8*)Data + s_ptNandInfo->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�

    if(HW_ECC & Flags)
    {
        NAND_CONTROLLER_REG->NFCONT |= (1<<7);// �ر�(��)Mian ECC
        EccOffset = s_ptNandInfo->OOB_Size - 4;// 4���ֽ�(1-bit ECCУ��)
        Ret = NAND_CONTROLLER_REG->NFMECC0;//����
        *(u32*)(Spare + EccOffset) = NAND_CONTROLLER_REG->NFMECC0;//
    }

    if(!(SPARE_REQ & Flags))
        memset(Spare, 0xFF, EccOffset);// δҪ��дspare,��Ĭ��д0xFF

    if(-2 == S3C2416_SpareProgram(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: ">0" -- �������������ֽ�����
//      "-1" -- �����������;
//      "-2" -- ��ʧ��;
//      "-3" -- ECC����ʧ��;
//��ע: ���ܶ�д�Ƿ���ȷ���������ݻش�
//-----------------------------------------------------------------------------
s32  S3C2416_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i;
    s32 Ret;
    u8 *Spare;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
    // ecc ����
    switch (Flags & MASK_ECC)
    {
        case HW_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT &= (~(1<<7));// ����(����)Main ECC
            NAND_CONTROLLER_REG->NFCONT |= (1<<5);// ��ʼ��Main ECC
            break;
        }
        case SW_ECC :
        case NO_ECC :
        {
            NAND_CONTROLLER_REG->NFCONT |= (1<<7);// �ر�Main ECC
            break;
        }
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }

    NandChipOn();

    SetNandCommand(PAGE_READ_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = 0x0;
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>8);
    NAND_CONTROLLER_REG->NFADDR = (u8)(PageNo>>16) & 0x1;

    SetNandCommand(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);// �г�
    WaitNandReady();// ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Data[i] = GetNandData();

    Ret = StatusOfNand();

    NandChipOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if (!((SPARE_REQ & Flags) || (HW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (s_ptNandInfo->BytesPerPage);// ֻ��ҳ,�����˳�
    }

    Spare = Data + s_ptNandInfo->BytesPerPage;// ע�⣺�����ǻ�����������ͳһ�Ļ�����߼�

    if(HW_ECC & Flags)
        NAND_CONTROLLER_REG->NFCONT |= (1<<7);// �ر�(��)Mian ECC

    if(-2 == S3C2416_SpareRead(PageNo, Spare))
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if(HW_ECC & Flags)
    {
        u32 EccRes, EccOffset, EccStatus;

        EccOffset = s_ptNandInfo->OOB_Size - 4;//
        EccRes = *(u32*)(Spare + EccOffset);
        // ���YAFFS2���߼�,HW_ECC��һ���²������ҳ��ECCУ����Ϊ0xFFFFFFFF(-1),
        // ����if����߼���ִ�о����޸Ķ��ص�����, �轫���ε��������
        if(EccRes != -1)
        {
            NAND_CONTROLLER_REG->NFMECCD0 = ((u32)((EccRes & 0xFF00) << 8) | (u32)(EccRes & 0xFF));
            NAND_CONTROLLER_REG->NFMECCD1 = ((u32)((EccRes & 0xFF000000) >> 8) | (u32)(EccRes & 0xFF0000) >> 16);

            EccStatus = NAND_CONTROLLER_REG->NFECCERR0;
            switch(EccStatus & 0x3)
            {
                case 0: break;// �޴�
                case 1:// 1λ���ɾ���
                {
                    Data[(EccStatus >> 7) & 0x7ff] ^= (1 << ((EccStatus >> 4) & 0x7));
                    debug_printf("nand driver"��"fix: bit 0x%x in Byte 0x%x\r\n",
                            ((EccStatus >> 4) & 0x7), ((EccStatus >> 7) & 0x7ff));
                    break;
                }
                case 2:// ����λ̫��, ���ɾ���
                case 3:// ECC�������
                    debug_printf("nand driver", "cannot be fixed");
                    Lock_MutexPost(NandFlashLock);
                    return (-3);
            }
        }
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);

}

//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 S3C2416_BlockErase(u32 BlkNo)
{
    s32 Ret;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }

    NandChipOn();

    SetNandCommand(BLOCK_ERASE_CMD_BYTE1);

    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo << 6);// 3����ַ���ڱ�ʾ����ҳ��
    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo >> 2);
    NAND_CONTROLLER_REG->NFADDR = (u8)(BlkNo >> 10);

    SetNandCommand(BLOCK_ERASE_CMD_BYTE2);

    DJY_EventDelay(3000);// �г�
    WaitNandReady();

    Ret = StatusOfNand();

    NandChipOff();
    Lock_MutexPost(NandFlashLock);
    return (Ret);
}

//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ÿ�;
//      "-1" -- ����;
//      "-2" -- �ڴ治��
//      "-3" -- ��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 S3C2416_BadChk(u32 BlkNo)
{
    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// ֻ�ж�ÿ�������ҳ
    {
        if(-2 == S3C2416_SpareRead(PageNo + i, Spare))
        {
            Ret = -3;
            break;
        }

        if ((0xAA == Spare[s_ptNandInfo->BadMarkOffset]) &&
            (0xAA == Spare[s_ptNandInfo->BadMarkOffset-1]))
        {
            Ret = -1;
            break;
        }
    }

    free(Spare);

    return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�;
//      "-1" -- ʧ��;
//      "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
s32  S3C2416_BadMark(u32 BlkNo)
{
    //�߼�:
    //    ����BAD MARKλԭ���Ƕ����Լ��Ƿ���ECCУ������, ֻ�ܽ���λ��־Ϊ����

    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);


    memset(Spare, 0xFF, s_ptNandInfo->OOB_Size);
    Spare[s_ptNandInfo->BadMarkOffset] = 0xAA;
    Spare[s_ptNandInfo->BadMarkOffset-1] = 0xAA;

    for (i = 0; i < 2; i++)
    {
        if(-2 == S3C2416_SpareProgram(PageNo + i, Spare))
            Ret = -1;
    }

    free(Spare);

    return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����: "0" -- �ɹ�; "-1" -- ����ʧ��; "-2" -- �ڴ治��;
//��ע:
//-----------------------------------------------------------------------------
static s32 S3C2416_GetNandDescr(struct NandDescr *Descr)
{
    u16 i;
    s32 Ret = 0;
    char *OnfiBuf;

    OnfiBuf = (char*)malloc(786);
    if (NULL == OnfiBuf)
        return (-1);

    memset(OnfiBuf, 0, 786);

    NandChipOn();

    SetNandCommand(PARAMETER_PAGE_READ_CMD_BYTE);

    NAND_CONTROLLER_REG->NFADDR = 0;

    WaitNandReady();// ʱ��Ҫ��

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = GetNandData();

    NandChipOff();

//  PrintBuf(OnfiBuf, 786);//����

    if(deonfi((const char*)OnfiBuf, Descr, 0))
        Ret = -1;

    free (OnfiBuf);

    return (Ret);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void  S3C2416_NAND_ControllerConfig(void)
{
    //�߼�:
    //

    //nand config register
    NAND_CONTROLLER_REG->NFCONF = (0<<23)|// ECCType       [24:23]    0:1-bit ECC
                                  (1<<12)|// TACLS         [14:12]    CLE&ALE duration = HCLK*TACLS.
                                  (3<<8)| // TWRPH0        [10:8]     TWRPH0 duration = HCLK*(TWRPH0+1)
                                  (1<<4)| // TWRPH1        [6:4]      TWRPH1 duration = HCLK*(TWRPH1+1)
                                  (1<<3)| // AdvFlash(R)   [3]        Advanced NAND, 0:256/512, 1:1024/2048
                                  (0<<2)| // PageSize(R)   [2]        NAND memory page size
                                  (1<<1)| // AddrCycle(R)  [1]        NAND flash addr size
                                  (0<<0); // BusWidth(R/W) [0]        NAND bus width. 0:8-bit, 1:16-bit.

    //nand control register
    NAND_CONTROLLER_REG->NFCONT = (0<<13)| // Lock-tight   [13]    0:Disable lock, 1:Enable lock.
                                  (0<<12)| // Soft Lock    [12]    0:Disable lock, 1:Enable lock.
                                  (0<<10)| // EnablillegalAcINT[10] Illegal access interupt control.0:Disable,1:Enable
                                  (0<<9)|  // EnbRnBINT    [9] RnB interrupt. 0:Disable, 1:Enable
                                  (0<<8)|  // RnB_TrandMode[8] RnB transition detection config.0:Low->High,1:High->Low
                                  (1<<6)|  // SpareECCLock [6]     0:Unlock, 1:Lock
                                  (1<<5)|  // MainECCLock  [5]     0:Unlock, 1:Lock
                                  (1<<4)|  // InitECC(W)   [4]     1:Init ECC decoder/encoder.
                                  (1<<1)|  // Reg_nCE      [1]     0:nFCE=0, 1:nFCE=1.
                                  (1<<0);  // NANDC Enable [0]     operating mode. 0:Disable, 1:Enable.

    ResetNand();
}
//-----------------------------------------------------------------------------
// ���ܣ���װnand����
// ������  doformat -- �Ƿ��ʽ����
// ���أ��ɹ���0����ʧ�ܣ�-1����
// ��ע��
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(u32 doformat)
{
    if(!s_ptNandInfo)
    {
        if(__nand_init())
        {
            printf("\r\n: erro : device : nand initialization failed(init).");
            return (-1);
        }
    }

    if(doformat)
    {
        struct uesz sz;
        sz.unit = 0;
        sz.block = 1;
        __nand_req(format, 0 , -1, &sz);
    }

    if(!badstable)
    {
        badstable = nandbuildbads(__nand_req);
        if(!badstable)
        {
            printf("\r\n: erro : device : nand initialization failed(bad table).");
            return (-1);
        }
    }

    nand_umedia = malloc(sizeof(struct umedia)+s_ptNandInfo->BytesPerPage+s_ptNandInfo->OOB_Size);
    if(!nand_umedia)
        return (-1);

    nand_umedia->mreq = __nand_req;
    nand_umedia->type = nand;
    nand_umedia->ubuf = (u8*)um + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }
    sNandflashInited = true;
    return (0);
}
// =============================================================================
// ���ܣ��ж�nandflash�Ƿ�װ
// ������  ��
// ���أ��ѳɹ���װ��true����δ�ɹ���װ��false����
// ��ע��
// =============================================================================
bool_t Nandflash_is_install(void)
{
    return sNandflashInited;
}
/******************************************************************************
                         PRIVATE FUNCTION(����˽�к���)
******************************************************************************/
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void ResetNand(void)
{
    NandChipOn();

    SetNandCommand(RESET_CMD_BYTE);

    WaitNandReady();

    NandChipOff();
}
//-----------------------------------------------------------------------------
//����: ���NAND�����Ƿ����
//����:
//����: 0 -- ����; -1 -- ����;
//��ע: δ�ж�busy��״̬
//-----------------------------------------------------------------------------
static s32 StatusOfNand(void)
{
    u8 Status = 0x1;

    do
    {
        SetNandCommand(STATUS_READ_CMD_BYTE);
        Status = GetNandData();
    }
    while(Status & 0x1);

    return(0);
}

//-----------------------------------------------------------------------------
//����: Nandʱ���߼�
//����:
//����:
//��ע: ʵ�������ж�NANDоƬ��R/Bn���ţ������ж��Ǳز����ٵ�
//-----------------------------------------------------------------------------
static void WaitNandReady(void)
{
    while(0 == (NAND_CONTROLLER_REG->NFSTAT & 0x1));
}

//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __WaitEccDone(void)
{
    ;//1-bit ECC û���жϵ�λ
}
#if 0
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void RawTest(void)
{
    struct FlashChip *Chip;
    u32 i;
    u8 *Buf;
    u32 TestPage = 640;

    ModuleInstall_NAND("nand", 0, 0);//��װnand�豸

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    memset(Buf, 0x0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.ErsBlk(TestPage/s_ptNandInfo->PagesPerBlk);

    Chip->Ops.RdPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    //����ECC����,1λ�������
    // 1��
    Buf[1] = 0;

    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
    // 2��
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[2] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    // 3��
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 1;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    //����ECC����,2λ�������,�ǲ��ܾ�����
    TestPage++;
    Chip->Ops.WrPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    Buf[3] = 0;
    Chip->Ops.WrPage(TestPage, Buf, NO_ECC | SPARE_REQ);

    memset(Buf, 0xFF, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Chip->Ops.RdPage(TestPage, Buf, HW_ECC | SPARE_REQ);

    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
    while(1);
}
//-----------------------------------------------------------------------------
//����:
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
void PageTest(const u32 PageNo, const u8 Init)
{
    volatile u8 *Buf;
    u32 i, Ret;
    struct FlashChip *Chip;
    u32 ECC_Flags = 0;

    if(Init)
    {
        if(ModuleInstall_NAND("nand", 0, 0)) //��װnand�豸
            while(1);
    }

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(i = 0; i < s_ptNandInfo->BytesPerPage; i++)
        Buf[i] = (u8)i;

    Chip->Ops.WrPage(PageNo, Buf, ECC_Flags | SPARE_REQ);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }

    // ECCһλ����
    Buf[1] = 0xF0;

    Chip->Ops.WrPage(PageNo, Buf, NO_ECC);

    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
            while(1);
    }

}
void BadMarkFunctionCheck(struct FlashChip *Chip)
{
    u32 i;
    u32 Blks;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = 0; i < Blks; i++)
    {
        if(Chip->Ops.Special.Nand.MrkBad(i))
            while(1);
    }

    for(i = 0; i < Blks; i++)
    {
        if(-1 != Chip->Ops.Special.Nand.ChkBlk(i))
            while(1);
    }
}
void ContinuityTest(struct FlashChip *Chip)
{
    u32 i, j, Len;
    u32 Blks, Pages;
    s32 BadBlks = 0;
    u8 *Temp;

    if(NULL == Chip)
        while(1);

    Blks = Chip->Descr.Nand.BlksPerLUN * Chip->Descr.Nand.LUNs;

    for(i = Chip->Descr.Nand.ReservedBlks; i < Blks; i++)
    {
        if(0 != Chip->Ops.ErsBlk(i))
        {
            /* ����ʧ�� */
//          TraceDrv(NAND_TRACE_ERROR, "block %d cannot be erased!\r\n", i);
            while(1);
        }
    }

    // �������У��
    Len = (Chip->Descr.Nand.BytesPerPage + Chip->Descr.Nand.OOB_Size);
    Temp = (u8*)malloc(Len);
    if(NULL == Temp)
        while(1);// �ڴ治��
    for(i = (Chip->Descr.Nand.ReservedBlks * Chip->Descr.Nand.PagesPerBlk); i < (Blks * Chip->Descr.Nand.PagesPerBlk); i++)
    {
        if(Len != Chip->Ops.RdPage(i, Temp, SPARE_REQ | NO_ECC))
            while(1);
        for(j = 0; j < Len; j++)
        {
            if(0xFF != Temp[j])
                while(1);
        }
    }
}

void ChipRawTest(void)
{
    struct FlashChip *Chip;
    u32 i, Ret;
    u8 *Buf;
    u32 TestPages;
    u32 TestBlocks;
    u32 ECC_Flags = 0;
    u32 ErrorCount = 0;

    if(ModuleInstall_NAND("nand", 0, 0))//��װnand�豸
        while(1);

    if(HW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= HW_ECC;

    if(SW_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= SW_ECC;

    if(NO_ECC_SUPPORTED & s_ptNandInfo->Controller)
        ECC_Flags |= NO_ECC;

    Chip = dListEntry(s_ptNandInfo, struct FlashChip, Descr);

    Buf = Chip->Buf;

    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        Chip->Ops.ErsBlk(TestBlocks);

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0xAA, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
            Ret = Chip->Ops.RdPage(CurPage, Buf, NO_ECC | SPARE_REQ);
            for(i=0; i<(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size); i++)
            {
                if(0xFF != Buf[i])
                {
                    ErrorCount += 1;
                    //PrintBuf(Buf, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
                    while(1);
                }
            }

            PageTest(CurPage, 0);
        }
    }


    for(TestBlocks = 0; TestBlocks < s_ptNandInfo->BlksPerLUN; TestBlocks++)
    {
        if(Chip->Ops.ErsBlk(TestBlocks))
            while(1);
    }


    BadMarkFunctionCheck(Chip);

    ContinuityTest(Chip);

    while(1);
}
#else

// ============================================================================
// ���ܣ�nand ����
// ������ucmd -- ���
//      ���� -- ���������
// ���أ�
// ��ע��
// ============================================================================
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...)
{
    s32 res = 0;

    switch(cmd)
    {
        case whichblock:
        {
            va_list list;
            u32 *block;
            s64 unit;

            block = (u32*)args;
            va_start(list, args);
            unit = (s64)va_arg(list, u32);
            va_end(list);
            *block = unit / s_ptNandInfo->PagesPerBlk;
            break;
        }

        case totalblocks:
        {
            // args = &blocks
            *((u32*)args) =  s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
            break;
        }

        case blockunits:
        {
            // args = &units
            *((u32*)args) = s_ptNandInfo->PagesPerBlk;
            break;
        }

        case unitbytes:
        {
            // args = &bytes
            *((u32*)args) = s_ptNandInfo->BytesPerPage;
            break;
        }

        case checkbad:
        {
            if(badslocation==(u32)args)
                res = 1;
            else
                res = S3C2416_BadChk((u32)args); // args = block

            break;
        }

        case savebads:
        {
            struct uopt opt = {0};
            opt.main = 1;
            if(0 != __nand_write(badslocation, (void*)args, opt)) // ���������ڵ�0ҳ
                res = -1;

            break;
        }

        case getbads:
        {
            struct uopt opt = {0};
            u32 **table = (u32**)args;

            opt.main = 1;
            if(!(*table))
            {
                res = 1;
            }
            else
            {
                res = __nand_read(badslocation, (void*)(*table), opt); // ���������ڵ�0ҳ
            }

            break;
        }

        case markbad:
        {
            res = S3C2416_BadMark((u32)args); // args = block
            break;
        }

        case format:
        {
            va_list list;
            s32 start, end;
            u8 *tmp, escape = 0;
            struct uesz *sz;
            struct uopt opt = {0};

            start = (u32)args;
            va_start(list, args);
            end = va_arg(list, u32);
            sz = (struct uesz*)va_arg(list, u32);
            va_end(list);

            if(!sz->block)
                return (-1);

            tmp = malloc(s_ptNandInfo->BytesPerPage);
            if(!tmp)
                return (-1);

            opt.main = 1;
            if(0 ==__nand_read(badslocation, (void*)(tmp), opt)) // �������
            {
                if(nandvalidbads((u32*)tmp))
                    escape = 1; // ���ڻ������������

                if(-1 == (s32)end)
                    end = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;

                do
                {
                    if((badslocation == (--end)) && escape) // ������ڵ�һҳ
                        continue;

                    if(__nand_erase((s64)end, *sz))
                    {
                        res = -1;
                        break;
                    }
                }
                while(end!=start);
            }
            else
            {
                res = -1;
            }

            free(tmp);
            break;
        }

        default: res = -1; break;
    }

    return (res);
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ������ţ�ҳ����
//      data -- �������ݣ�
//      opt -- ���ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __nand_read(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = S3C2416_PageRead((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)s_ptNandInfo->BytesPerPage)
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = S3C2416_SpareRead((u32)unit, (u8*)data);
        if(res != (s32)s_ptNandInfo->OOB_Size)
        {
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// ���ܣ�nand д��
// ������unit -- д����ţ�ҳ����
//      data -- д�����ݣ�
//      opt -- д�ķ�ʽ��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ���أ�
// ��ע��
// ============================================================================
s32 __nand_write(s64 unit, void *data, struct uopt opt)
{
    u32 flags = 0;
    s32 res;
    nandbadfreeunit(badstable, &unit, __nand_req);
    if(opt.hecc)
        flags |= HW_ECC;
    else if(opt.secc)
        flags |= SW_ECC;
    else
        flags |= NO_ECC;

    if(opt.main)
    {
        if(opt.spare)
            flags |= SPARE_REQ;

        res = S3C2416_PageProgram((u32)unit, (u8*)data, flags);
        if (!((SPARE_REQ & flags) || (HW_ECC & flags)))
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage))
            {
                return (-1);
            }
        }
        else
        {
            if(res != (s32)(s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size))
            {
                return (-1);
            }
        }
    }
    else
    {
        res = S3C2416_SpareProgram((u32)unit, (u8*)data);
        if(res != (s32)s_ptNandInfo->OOB_Size)
        {
            return (-1);
        }
    }

    return (0);
}

// ============================================================================
// ���ܣ�nand ����
// ������unit -- ��������ţ�
//      sz -- �����ĵ�λ��unit��block��
// ���أ��ɹ� -- ��0����ʧ�� -- ��-1��
// ��ע��
// ============================================================================
s32 __nand_erase(s64 unit, struct uesz sz)
{
    u32 block;

    if(sz.unit)
    {
        nandbadfreeunit(badstable, &unit, __nand_req);
        block = (u32)(unit / s_ptNandInfo->PagesPerBlk);
    }
    else
    {
        block = unit;
        nandbadfreeblock(badstable, &block, __nand_req);
    }

    return (S3C2416_BlockErase(block));
}

// ============================================================================
// ���ܣ�
// ������
// ���أ�
// ��ע��
// ============================================================================
static s32 __nand_init(void)
{
    s_ptNandInfo = malloc(sizeof(*s_ptNandInfo));
    if(!s_ptNandInfo)
        return (-1);

    S3C2416_NAND_ControllerConfig();// оƬ�ܽŵȻ�������()
    if(S3C2416_GetNandDescr(s_ptNandInfo))
    {
        free(s_ptNandInfo);
        s_ptNandInfo = NULL;
        return (-1);
    }
    NandFlashLock = Lock_MutexCreate("Nand Flash Lock");        //����nand�Ļ�����
    s_ptNandInfo->ReservedBlks = 0;
    s_ptNandInfo->Controller = HW_ECC_SUPPORTED;
    s_ptNandInfo->BadMarkOffset = s_ptNandInfo->OOB_Size - 4 - 1;
    return (0);
}

// ============================================================================
// ���ܣ���nand���ļ�ϵͳ��������
// ������fs -- ��Ҫ���ص��ļ�ϵͳ��mediadrv -- ý��������
//       bstart -- ��ʼ�飬bend -- �����飨�������ÿ飬ֻ���ÿ����һ�飩
// ���أ�0 -- �ɹ��� -1 -- ʧ��
// ��ע��
// ============================================================================
s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv)
{
    char *FullPath,*notfind;
    struct Object *targetobj;
    struct FsCore *super;
    s32 res,BlockNum;
    targetobj = OBJ_MatchPath(fs, &notfind);
    if(notfind)
    {
        error_printf("nand"," not found need to install file system.");
        return -1;
    }
    super = (struct FsCore *)OBJ_GetPrivate(targetobj);
    super->MediaInfo = nand_umedia;
    super->MediaDrv = mediadrv;
    if(-1 == (s32)bend)
    {
        bend = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;
        BlockNum = bend - bstart;
    }
    else
    {
        BlockNum = bend - bstart;
    }
    super->AreaSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk * BlockNum;
    super->MediaStart = bstart*s_ptNandInfo->PagesPerBlk; // ��ʼunit��

    res = strlen(NandFlashName)+strlen(s_ptDeviceRoot->name) + 1;
    FullPath = malloc(res);
    memset(FullPath, 0, res);
    sprintf(FullPath, "%s/%s", s_ptDeviceRoot->name,NandFlashName); //��ȡ���豸��ȫ·��
    File_BeMedia(FullPath,fs); //�����豸�����ļ�ϵͳ
    free(FullPath);

    printf("\r\n: info : device : %s added(start:%d, end:%d).", fs, bstart, bend);
    return (0);
}

