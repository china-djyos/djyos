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
#include <djyos.h>
#include "dbug.h"
#include <device/djy_flash.h>
#include <math.h>
#include <djyfs/filesystems.h>
#include <device/unit_media.h>
#include <board.h>
#include <libc/misc/ecc/ecc_256.h>
#include <cpu_peri.h>// ����"samv71q21.h"
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
//
//����ķ�����ʼ�ͷ���������д����-1��10000��������ֵ�ɸ��ݾ���оƬ������д
//
#define BOARD_NAND_CS               (3)// NAND Ƭѡ�ܽ�
#define NAND_COMMAND_CYCLE(x)       ((*(volatile u8*)0x63400000) = ((u8)(x)))// todo
#define NAND_ADDRESS_CYCLE(x)       ((*(volatile u8*)0x63200000) = ((u8)(x)))
#define NAND_WR_DATA_CYCLE(x)       ((*(volatile u8*)0x63000000) = ((u8)(x)))
#define NAND_RD_DATA_CYCLE()        (*(volatile u8*)0x63100000)

s32 __nand_FsInstallInit(const char *fs, s32 bstart, s32 bend, void *mediadrv);
static s32 __nand_init(void);
s32 __nand_req(enum ucmd cmd, ptu32_t args, ...);
s32 __nand_read(s64 unit, void *data, struct uopt opt);
s32 __nand_write(s64 unit, void *data, struct uopt opt);
s32 __nand_erase(s64 unit, struct uesz sz);
static u32 *badstable;
static u32 badslocation = 0;
//
extern s32 deonfi(const char *data, struct NandDescr *onfi, u8 little);
#define NFlashLockTimeOut     CN_CFG_TICK_US * 1000 * 10
struct NandDescr *s_ptNandInfo;// ��ʱ
static struct MutexLCB *NandFlashLock;
struct umedia *nand_umedia;
static const char *NandFlashName = "nand";      //��flash��obj�ڵ�����
static u8 s_u8SizeofHammingCode; // ECCУ�����ݴ�С
static u8 *s_pu8HammingCode; // ECCУ����
extern struct Object *s_ptDeviceRoot;
static bool_t sNandflashInited = false;

static const Pin NAND_PINS[] = {
{PIO_PC0A_D0,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D0 �ܽ�
{PIO_PC1A_D1,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D1 �ܽ�
{PIO_PC2A_D2,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D2 �ܽ�
{PIO_PC3A_D3,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D3 �ܽ�
{PIO_PC4A_D4,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D4 �ܽ�
{PIO_PC5A_D5,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D5 �ܽ�
{PIO_PC6A_D6,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D6 �ܽ�
{PIO_PC7A_D7,           PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND D7 �ܽ�

{PIO_PC16A_NANDALE,     PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND ALE �ܽ�
{PIO_PC17A_NANDCLE,     PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND CLE �ܽ�
{PIO_PC9A_NANDOE,       PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND OE �ܽ�
{PIO_PC10A_NANDWE,      PIOC, ID_PIOC, PIO_PERIPH_A, PIO_PULLUP },  // NAND WE �ܽ�
{PIO_PD19A_NCS3,        PIOD, ID_PIOD, PIO_OUTPUT_1, PIO_DEFAULT},  // NAND CS �ܽ�PIO_PD19A_NCS3
{PIO_PE2,               PIOE, ID_PIOE, PIO_INPUT,    PIO_PULLUP},   // NAND ready/busy �ܽ�

{PIO_PD27,              PIOD, ID_PIOD, PIO_OUTPUT_0, PIO_PULLUP}    // NAND WP�ܽ�
};
//
// ���غ�������
//
static void __NandOff(void);
static void __NandOn(void);
static void __NandWaitReady(void);
static s32 __NandStatus(void);
static void __NandReset(void);
static u32 __NandID(void);

//-----------------------------------------------------------------------------
//����: дNANDĳҳ��OOB��
//����: PageNo -- ҳ��;
//      Data --  д����
//���: ">0" -- д�ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
s32 SAMv7_SpareProgram(u32 PageNo, const u8 *Data)
{
    u32 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }
    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(SpareOffset);
    NAND_ADDRESS_CYCLE((SpareOffset>>8) & 0xF);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    for(i = 0; i < (s_ptNandInfo->OOB_Size); i++)
        NAND_WR_DATA_CYCLE(Data[i]);

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE2);

    DJY_EventDelay(700);// �г�
    __NandWaitReady(); // ʱ��Ҫ��

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->OOB_Size);
}
//-----------------------------------------------------------------------------
//����: ��NANDĳҳ��OOB��
//����: PageNo -- ҳ��;
//      Data --  ������
//���: ">0" -- ���ɹ�; "-2" -- дʧ��;
//����:
//-----------------------------------------------------------------------------
s32 SAMv7_SpareRead(u32 PageNo, u8 *Data)
{
    u8 i;
    s32 Ret;
    u32 SpareOffset = s_ptNandInfo->BytesPerPage;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(SpareOffset);
    NAND_ADDRESS_CYCLE((SpareOffset>>8) & 0xF);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);
    //NAND_ADDRESS_CYCLE((PageNo>>16) & 0x1);

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);
    __NandWaitReady(); // ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = NAND_RD_DATA_CYCLE();

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    return (s_ptNandInfo->OOB_Size);
}

//-----------------------------------------------------------------------------
//����: дNANDĳҳ
//����: PageNo -- ҳ��;
//      Data -- д����;
//      Flags -- д��������λ;
//����: ">0" -- ������д����ֽ���;
//      "-1" -- ��������;
//      "-2" -- дʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 SAMv7_PageProgram(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i, EccOffset, WrLen = s_ptNandInfo->BytesPerPage;
    s32 Ret;
    u8 *EccSpace;//ָ����ECCУ�����ݵĿռ�

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    // ECC ����
    switch (Flags & MASK_ECC)
    {
        case NO_ECC :
        case HW_ECC :
        {
            break;//��֧��HW_ECC
        }
        case SW_ECC :
        {
            EccOffset = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;
            EccSpace = (u8*)Data + EccOffset;
            WrLen = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
            memset(s_pu8HammingCode, 0xFF, s_u8SizeofHammingCode);
            ECC_HammingCompute256x(Data, s_ptNandInfo->BytesPerPage, s_pu8HammingCode);
            for(i=0; i<s_u8SizeofHammingCode; i++)
            {
                EccSpace[i] = s_pu8HammingCode[i];
            }
            break;
        }
        default :
            Lock_MutexPost(NandFlashLock);
            return (-1);
    }

    if(SPARE_REQ & Flags)
        WrLen = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(0x0);
    NAND_ADDRESS_CYCLE(0x0);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    for(i = 0; i < WrLen; i++)
        NAND_WR_DATA_CYCLE(Data[i]);

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE2);// д��Main�������

    DJY_EventDelay(700);// �г�
    __NandWaitReady();// ʱ��Ҫ��

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    Lock_MutexPost(NandFlashLock);
    return (WrLen);

}
//-----------------------------------------------------------------------------
//����: ��NANDĳҳ
//����: PageNo -- ҳ��;
//      Data -- ������;
//      Flags -- ����������λ;
//����: ">0" -- �������������ֽ�����
//      "-1" -- �����������;
//      "-2" -- ��ʧ��;
//      "-3" -- ECC����ʧ��;
//��ע: ���ܶ�д�Ƿ���ȷ���������ݻش�
//-----------------------------------------------------------------------------
s32 SAMv7_PageRead(u32 PageNo, u8 *Data, u32 Flags)
{
    //�߼�:
    //
    u32 i, RdLen;
    s32 Ret;
    u32 EccRet, EccOffset;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-2);
    }

    if(((SW_ECC & Flags) && (HW_ECC & Flags)) ||
       ((NO_ECC & Flags) && (HW_ECC & Flags)) ||
       ((NO_ECC & Flags) && (SW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (-1);
    }

    if((SPARE_REQ & Flags) || (SW_ECC & Flags))
        RdLen = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size;
    else
        RdLen = s_ptNandInfo->BytesPerPage;

    __NandWaitReady(); // todo����ֹ��ͻ

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(0x0);
    NAND_ADDRESS_CYCLE(0x0);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE2);

    DJY_EventDelay(25);
    __NandWaitReady(); // ʱ��Ҫ��

    for(i = 0; i < RdLen; i++)
        Data[i] = NAND_RD_DATA_CYCLE();

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
    {
        Lock_MutexPost(NandFlashLock);
        return (-2);
    }

    if (!((SPARE_REQ & Flags) || (SW_ECC & Flags)))
    {
        Lock_MutexPost(NandFlashLock);
        return (s_ptNandInfo->BytesPerPage); // ֻ��ҳ,�����˳�
    }

    if(SW_ECC & Flags)
    {
        EccOffset = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;
        for(i = 0; i < s_u8SizeofHammingCode; i++)
        {
            s_pu8HammingCode[i] = Data[EccOffset+i];
        }

        EccRet = ECC_HammingVerify256x(Data, s_ptNandInfo->BytesPerPage, s_pu8HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
//            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            debug_printf("null","cannot be fixed");
            Lock_MutexPost(NandFlashLock);
            return (-3);
        }
    }
    Lock_MutexPost(NandFlashLock);
    return (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size);

}

//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//����: "0" -- �ɹ�;"-1" -- ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 SAMv7_BlockErase(u32 BlkNo)
{
    s32 Ret;

    if(Lock_MutexPend(NandFlashLock, NFlashLockTimeOut) == false)
    {
        return (-1);
    }

    __NandOn();

    NAND_COMMAND_CYCLE(BLOCK_ERASE_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(BlkNo << 6); // 3����ַ���ڱ�ʾ����ҳ��
    NAND_ADDRESS_CYCLE(BlkNo >> 2);
    //NAND_ADDRESS_CYCLE(BlkNo >> 10);

    NAND_COMMAND_CYCLE(BLOCK_ERASE_CMD_BYTE2);

    DJY_EventDelay(3000); // todo
    __NandWaitReady();

    Ret = __NandStatus();

    __NandOff();
    Lock_MutexPost(NandFlashLock);
    return (Ret);
}

//-----------------------------------------------------------------------------
//����: ����״�����û���
//����: BlkNo -- ���;
//����: "0" -- �ÿ�;
//      "-1" -- ����;
//      "-2" -- �ڴ治��
//      "-3" -- ��ʧ��;
//��ע:
//-----------------------------------------------------------------------------
s32 SAMv7_BadChk(u32 BlkNo)
{
    u8 *Spare, i;
    s32 Ret = 0;
    u32 PageNo = BlkNo * s_ptNandInfo->PagesPerBlk;

    Spare = malloc (s_ptNandInfo->OOB_Size);
    if (NULL == Spare)
        return (-2);

    for (i = 0; i < 2; i++)// ֻ�ж�ÿ�������ҳ
    {
        if(-2 == SAMv7_SpareRead(PageNo + i, Spare))
        {
            Ret = -3;
            break;
        }

        if (0xFF != Spare[s_ptNandInfo->BadMarkOffset])
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
s32 SAMv7_BadMark(u32 BlkNo)
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

    for (i = 0; i < 2; i++)
    {
        if(-2 == SAMv7_SpareProgram(PageNo + i, Spare))
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
static s32 SAMv7_GetNandDescr(struct NandDescr *Descr)
{
    u16 i;
    s32 Ret = 0;
    char *OnfiBuf;

    OnfiBuf = (char*)malloc(786);
    if (NULL == OnfiBuf)
        return (-1);

    memset(OnfiBuf, 0, 786);

    __NandOn();

    NAND_COMMAND_CYCLE(PARAMETER_PAGE_READ_CMD_BYTE);

    NAND_ADDRESS_CYCLE(0);

    __NandWaitReady();// ʱ��Ҫ��

    for(i = 0; i < 786; i++)
        OnfiBuf[i] = NAND_RD_DATA_CYCLE();

    __NandOff();

//  PrintBuf(OnfiBuf, 786);//����
//  {
//      char j;
//      for(i = 0; i < 786; i++)
//          j = OnfiBuf[i];
//  }


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
#define NCS_RD_SETUP(x)     (((u32)((x)<<24)) & 0xFF000000)
#define NRD_SETUP(x)        (((u32)((x)<<16)) & 0xFF0000)
#define NCS_WR_SETUP(x)     (((u32)((x)<<8)) & 0xFF00)
#define NWE_SETUP(x)        (((u32)(x)) & 0xFF)
#define NCS_RD_PULSE(x)     (((u32)((x)<<24)) & 0xFF000000)
#define NRD_PULSE(x)        (((u32)((x)<<16)) & 0xFF0000)
#define NCS_WR_PULSE(x)     (((u32)((x)<<8)) & 0xFF00)
#define NWE_PULSE(x)        (((u32)(x)) & 0xFF)
#define NRD_CYCLE(x)        (((u32)((x)<<16)) & 0xFFFF0000)
#define NWE_CYCLE(x)        (((u32)(x)) & 0xFFFF)

static void  SAMv7_NAND_Config(void)
{
    //�߼�:
    //
    PMC_EnablePeripheral(ID_SMC);
    PMC_EnablePeripheral(ID_PIOC);

    MATRIX->CCFG_SMCNFCS = (MATRIX->CCFG_SMCNFCS | (0x1u << BOARD_NAND_CS));
    // MLKΪ150MHz,ͬʱΪʱ��ʱ�ӣ����ȵ�λΪ6.7ns
    SMC->SMC_CS_NUMBER[BOARD_NAND_CS].SMC_SETUP = NCS_RD_SETUP(1) | NRD_SETUP(0) |
                                                  NCS_WR_SETUP(1) | NWE_SETUP(0);
    SMC->SMC_CS_NUMBER[BOARD_NAND_CS].SMC_PULSE = NCS_RD_PULSE(4) | NRD_PULSE(4) |
                                                  NCS_WR_PULSE(3) | NWE_PULSE(2);
    SMC->SMC_CS_NUMBER[BOARD_NAND_CS].SMC_CYCLE = NRD_CYCLE(7) | NWE_CYCLE(4);
    SMC->SMC_CS_NUMBER[BOARD_NAND_CS].SMC_MODE = (SMC_MODE_READ_MODE | SMC_MODE_WRITE_MODE | SMC_MODE_DBW_8_BIT);

    PIO_Configure(NAND_PINS, PIO_LISTSIZE(NAND_PINS));

    PIO_Set(&NAND_PINS[14]); // ȥ��д����

    __NandReset();

    __NandID();
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
    nand_umedia->ubuf = (u8*)nand_umedia + sizeof(struct umedia);

    if(!Device_Create((const char*)NandFlashName, NULL, NULL, NULL, NULL, NULL, ((ptu32_t)nand_umedia)))
    {
        printf("\r\n: erro : device : %s addition failed.", NandFlashName);
        free(nand_umedia);
        return (-1);
    }
    sNandflashInited = true;
    return (0);// �ɹ�;
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
//����: ���NAND�����Ƿ����
//����:
//����: 0 -- ����; -1 -- ����;
//��ע: δ�ж�busy��״̬
//-----------------------------------------------------------------------------
static s32 __NandStatus(void)
{
    u8 Status = 0x1;
    u32 Retry = 0;

    do
    {
        NAND_COMMAND_CYCLE(STATUS_READ_CMD_BYTE);
        Status = NAND_RD_DATA_CYCLE();
    }
    while((Status & 0x1) && (Retry++ < 10));

    if(Retry > 10)
        __NandReset();// todo: ���������оƬ

    return(0);
}
//-----------------------------------------------------------------------------
//����: NANDʱ���߼�
//����:
//����:
//��ע: ʵ�������ж�NANDоƬ��R/Bn���ţ������ж��Ǳز����ٵġ�
//-----------------------------------------------------------------------------
static void __NandWaitReady(void)
{
    u32 Retry = 0;
    while(0 == PIO_Get(&NAND_PINS[13]))
    {
        if(Retry++ > 100)
            break;
    }

    while(0 == PIO_Get(&NAND_PINS[13]))
    {
        DJY_EventDelay(25);
        if(Retry++ > 100)
            break;
    }
}
//-----------------------------------------------------------------------------
//����: ��NANDƬѡ��CS��Ϊ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __NandOn(void)
{
    PIO_Clear(&NAND_PINS[12]);
}
//-----------------------------------------------------------------------------
//����: ��NANDƬѡ��CS��Ϊ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __NandOff(void)
{
    PIO_Set(&NAND_PINS[12]);
}
//-----------------------------------------------------------------------------
//����: ����NANDоƬ
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static void __NandReset(void)
{
    __NandOn();

    NAND_COMMAND_CYCLE(RESET_CMD_BYTE);

    __NandWaitReady();

    __NandOff();

}
//-----------------------------------------------------------------------------
//����: ��NANDоƬID����
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static u32 __NandID(void)
{
    u32 NAND_ID;
    volatile u32 delay = 0;

    __NandOn();

    NAND_COMMAND_CYCLE(ID_READ_CMD_BYTE);

    NAND_ADDRESS_CYCLE(0);

    for(;;)
    {
        if(delay++ > 0xFFFF)
            break;
    }
    NAND_ID = NAND_RD_DATA_CYCLE();
    NAND_ID |= (NAND_RD_DATA_CYCLE() << 8);
    NAND_ID |= (NAND_RD_DATA_CYCLE() << 16);
    NAND_ID |= (NAND_RD_DATA_CYCLE() << 24);
    //__NandWaitReady();

    __NandOff();

    return (NAND_ID);
}

#if 0
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
    Buf[1] = 0;
    Chip->Ops.WrPage(PageNo, Buf, NO_ECC);
    memset(Buf, 0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
    Ret = Chip->Ops.RdPage(PageNo, Buf, ECC_Flags | SPARE_REQ);
    for(i=0; i<(s_ptNandInfo->BytesPerPage); i++)
    {
        if(((u8)i) != Buf[i])
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
            // ����ʧ��
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
        memset(Buf, 0x0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));

        Chip->Ops.ErsBlk(TestBlocks);

        for(TestPages = 0; TestPages < s_ptNandInfo->PagesPerBlk; TestPages++)
        {
            u32 CurPage = TestPages + (TestBlocks * s_ptNandInfo->PagesPerBlk);
            memset(Buf, 0x0, (s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size));
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

    ContinuityTest(Chip);
    while(1);
}

#else // �½ӿ�

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
                res = SAMv7_BadChk((u32)args); // args = block

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
            res = SAMv7_BadMark((u32)args); // args = block
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

                if(-1==(s32)end)
                    end = s_ptNandInfo->BlksPerLUN * s_ptNandInfo->LUNs;

                do
                {
                    if((badslocation==(--end))&&escape) // ������ڵ�һҳ
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

        res = SAMv7_PageRead((u32)unit, (u8*)data, flags);
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
        res = SAMv7_SpareRead((u32)unit, (u8*)data);
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

        res = SAMv7_PageProgram((u32)unit, (u8*)data, flags);
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
        res = SAMv7_SpareProgram((u32)unit, (u8*)data);
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

    return (SAMv7_BlockErase(block));
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

    SAMv7_NAND_Config();// оƬ�ܽŵȻ�������
    if(SAMv7_GetNandDescr(s_ptNandInfo))
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

