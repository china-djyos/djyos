//-----------------------------------------------------------------------------
// Copyright (c) 2018,Open source team. All rights reserved.

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
// Copyright (c) 2014 ����Ȩ�ɶ����߲���ϵͳ��Դ�Ŷ����С�����Ȩ�˱���һ��Ȩ����
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
#include <device/flash/flash.h>
#include <cpu_peri.h>// ����"samv71q21.h"
#include "project_config.h"     //���ļ���IDE�����ý������ɣ������APP�Ĺ���Ŀ¼�С�
                                //�����Ǹ����ļ����������ý���Ĭ��ֵ���á�

//@#$%component configure   ****������ÿ�ʼ������ DIDE ��ͼ�λ����ý���
//****���ÿ���﷨��ʹ�÷������μ�Դ���Ŀ¼�µ��ļ���component_config_readme.txt****
//%$#@initcode      ****��ʼ�����뿪ʼ���� DIDE ɾ����//����copy����ʼ���ļ���
//    ModuleInstall_NAND(CFG_NAND_CHIP_NAME,CFG_NAND_FLAG,CFG_NAND_START_BLOCK);
//%$#@end initcode  ****��ʼ���������

//%$#@describe      ****���������ʼ
//component name:"cpu_peri_nand"                                            //CPU��nand����������
//parent:"djyfs"                                                            //��д������ĸ�������֣�none��ʾû�и����
//attribute:bsp���                                                          //ѡ���������������������bsp������û��������������������IDE�з���
//select:��ѡ                                                               //ѡ���ѡ����ѡ������ѡ���������ѡ����Ҫ���ò�������IDE�ü�������Ĭ�Ϲ�ȡ��
                                                                            //����ȡ������ѡ�Ҳ���Ҫ���ò����ģ����ǲ���ѡ�ģ�IDE�ü������в���ʾ��
//init time:early                                                          //��ʼ��ʱ������ѡֵ��early��medium��later��
                                                                            //��ʾ��ʼ��ʱ�䣬�ֱ������ڡ����ڡ�����
//dependence:"djyfs","heap","driver","atsamv7"            //������������������������none����ʾ�������������
                                                                            //ѡ�и����ʱ�������������ǿ��ѡ�У�
                                                                            //����������������������г����á�,���ָ�
//weakdependence:"none"                                                     //��������������������������none����ʾ�������������
                                                                            //ѡ�и����ʱ��������������ᱻǿ��ѡ�У�
                                                                            //����������������������г����á�,���ָ�
//mutex:"none"                                                              //������������������������none����ʾ�������������
                                                                            //����������������������г����á�,���ָ�
//%$#@end describe  ****�����������

//%$#@configue      ****�������ÿ�ʼ
//%$#@target = header           //header = ����ͷ�ļ�,cmdline = �����б�����DJYOS����ģ�����
#ifndef CFG_NAND_START_BLOCK   //****�������Ƿ��Ѿ����ú�
#warning    cpu_peri_mcan�������δ���ã�ʹ��Ĭ��ֵ
//%$#@num,0,100,
#define CFG_NAND_START_BLOCK        (0)         //"��ʼ��",nand�ļ�ϵͳʹ�õ���ʼ��
#define CFG_NAND_FLAG               (2)         //"���"
//%$#@enum,true,false,
//%$#@string,1,10,
#define CFG_NAND_CHIP_NAME          ("nand")    //"оƬ��",оƬ����
//%$#select,        ***������ֵ�ĺ꣬�����ڵ��������
//%$#@free,
#endif
//%$#@end configue  ****�������ý���
//@#$%component end configure

//
//
//
#define BOARD_NAND_CS               (3)// NAND Ƭѡ�ܽ�
#define NAND_COMMAND_CYCLE(x)       ((*(volatile u8*)0x63400000) = ((u8)(x)))// todo
#define NAND_ADDRESS_CYCLE(x)       ((*(volatile u8*)0x63200000) = ((u8)(x)))
#define NAND_WR_DATA_CYCLE(x)       ((*(volatile u8*)0x63000000) = ((u8)(x)))
#define NAND_RD_DATA_CYCLE()        (*(volatile u8*)0x63100000)
//
//
//
//static struct NandDescr *s_ptNandInfo;
struct NandDescr *s_ptNandInfo;// ��ʱ
static u8 s_u8SizeofHammingCode; // ECCУ�����ݴ�С
static u8 *s_pu8HammingCode; // ECCУ����
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

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(SpareOffset);
    NAND_ADDRESS_CYCLE((SpareOffset>>8) & 0xF);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    for(i = 0; i < (s_ptNandInfo->OOB_Size); i++)
        NAND_WR_DATA_CYCLE(Data[i]);

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE2);

    Djy_EventDelay(700);// �г�
    __NandWaitReady(); // ʱ��Ҫ��

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

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

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(SpareOffset);
    NAND_ADDRESS_CYCLE((SpareOffset>>8) & 0xF);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);
    //NAND_ADDRESS_CYCLE((PageNo>>16) & 0x1);

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE2);

    Djy_EventDelay(25);
    __NandWaitReady(); // ʱ��Ҫ��

    for(i = 0; i < s_ptNandInfo->OOB_Size; i++)
        Data[i] = NAND_RD_DATA_CYCLE();

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

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
            hamming_compute_256x(Data, s_ptNandInfo->BytesPerPage, s_pu8HammingCode);
            for(i=0; i<s_u8SizeofHammingCode; i++)
            {
                EccSpace[i] = s_pu8HammingCode[i];
            }
            break;
        }
        default :return (-1);
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

    Djy_EventDelay(700);// �г�
    __NandWaitReady();// ʱ��Ҫ��

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

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


    if(((SW_ECC & Flags) && (HW_ECC & Flags)) ||
       ((NO_ECC & Flags) && (HW_ECC & Flags)) ||
       ((NO_ECC & Flags) && (SW_ECC & Flags)))
        return (-1);

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

    Djy_EventDelay(25);
    __NandWaitReady(); // ʱ��Ҫ��

    for(i = 0; i < RdLen; i++)
        Data[i] = NAND_RD_DATA_CYCLE();

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

    if (!((SPARE_REQ & Flags) || (SW_ECC & Flags)))
        return (s_ptNandInfo->BytesPerPage); // ֻ��ҳ,�����˳�

    if(SW_ECC & Flags)
    {
        EccOffset = s_ptNandInfo->BytesPerPage + s_ptNandInfo->OOB_Size - s_u8SizeofHammingCode;
        for(i = 0; i < s_u8SizeofHammingCode; i++)
        {
            s_pu8HammingCode[i] = Data[EccOffset+i];
        }

        EccRet = hamming_verify_256x(Data, s_ptNandInfo->BytesPerPage, s_pu8HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
//            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            debug_printf("null","cannot be fixed");
            return (-3);
        }
    }
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

    __NandOn();

    NAND_COMMAND_CYCLE(BLOCK_ERASE_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(BlkNo << 6); // 3����ַ���ڱ�ʾ����ҳ��
    NAND_ADDRESS_CYCLE(BlkNo >> 2);
    //NAND_ADDRESS_CYCLE(BlkNo >> 10);

    NAND_COMMAND_CYCLE(BLOCK_ERASE_CMD_BYTE2);

    Djy_EventDelay(3000); // todo
    __NandWaitReady();

    Ret = __NandStatus();

    __NandOff();

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


    if(DecodeONFI((const char*)OnfiBuf, Descr, 0))
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
//����: ��װ"nand"ģ��
//����: ChipName -- ������
//      Flags -- ������ʽ��;"1"--��;"0"--��;
//      StartBlk -- ��ʼ��;
//����: "0" -- �ɹ�;
//      "-1" -- �����������;
//      "-2" -- �ڴ治��;
//      "-3" -- �豸��Ϣ��ȡʧ�ܡ�
//��ע:
//-----------------------------------------------------------------------------
s32 ModuleInstall_NAND(const char *ChipName, u32 Flags, u16 StartBlk)
{
    //�߼�:
    //   1.оƬ�ܽŵȻ�������;
    //   2.(�����ж��߼�)��ȡоƬ����Ϣ;
    //   3.��dev��֧�½���NAND�ڵ�;
    //   4.��ʼ��NAND�ڵ�,����FLASH�Ĳ�������;

    u32 Len;
    struct FlashChip *Chip;
    struct NandDescr ChipDesrc = {0};

    if (NULL == ChipName)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "unavailable param! \r\n");
        error_printf("null","unavailable param! \r\n");
        return (-1);
    }

    SAMv7_NAND_Config();// оƬ�ܽŵȻ�������

    // ��ȡNAND��Ϣ
    if(SAMv7_GetNandDescr(&ChipDesrc))
    {
//        TraceDrv(FLASH_TRACE_ERROR, "����NAND��Ϣʧ��\r\n");
        error_printf("null","����NAND��Ϣʧ��\r\n");
        return (-3);
    }

    if(StartBlk >= ChipDesrc.BlksPerLUN * ChipDesrc.LUNs)
        return (-1);

    ChipDesrc.ReservedBlks = StartBlk;
    ChipDesrc.Controller = SW_ECC_SUPPORTED;

    Len = strlen (ChipName) + 1;

    Chip = (struct FlashChip*)malloc(sizeof(struct FlashChip) + Len);
    if (NULL == Chip)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        return (-2);
    }

    memset (Chip, 0x00, sizeof(*Chip));

    s_u8SizeofHammingCode = (ChipDesrc.BytesPerPage >> 8) * 3; // ((ҳ��С/256)*3)
    ChipDesrc.BadMarkOffset = ChipDesrc.OOB_Size - s_u8SizeofHammingCode - 1;
    s_pu8HammingCode = malloc(s_u8SizeofHammingCode);
    if(NULL == s_pu8HammingCode)
    {
//        TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        free(Chip);
        return (-2);
    }

    Chip->Type                    = F_NAND;
    Chip->Descr.Nand              = ChipDesrc;
    Chip->Ops.RdPage              = SAMv7_PageRead;
    Chip->Ops.WrPage              = SAMv7_PageProgram;
    Chip->Ops.ErsBlk              = SAMv7_BlockErase;
    Chip->Ops.Special.Nand.ChkBlk = SAMv7_BadChk;
    Chip->Ops.Special.Nand.MrkBad = SAMv7_BadMark;

    strcpy(Chip->Name, ChipName); // �豸��

    s_ptNandInfo = &(Chip->Descr);

    Chip->Buf = (u8*)malloc(s_ptNandInfo->OOB_Size + s_ptNandInfo->BytesPerPage); // NAND�ײ㻺��
    if(NULL == Chip->Buf)
    {
 //       TraceDrv(FLASH_TRACE_ERROR, "out of memory!\r\n");
        error_printf("null","out of memory!\r\n");
        free(s_pu8HammingCode);
        free(Chip);
        return (-2);
    }

    if(!dev_add(NULL,Chip->Name, NULL, NULL, NULL, NULL, NULL, (ptu32_t)Chip)) // �豸����"/dev"
    {
        free(s_pu8HammingCode);
        free(Chip);
        error_printf("null","device  add nand flash failed.");
    }

    if(Flags & FLASH_ERASE_ALL)
        EarseWholeChip(Chip);

    return (0);// �ɹ�;
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
        Djy_EventDelay(25);
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

/******************************************************************************
                         �����ļ�ϵͳ����
******************************************************************************/
#ifdef EFS_ON
//-----------------------------------------------------------------------------
//����: дС��һҳ������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
#define __WritePage(a,b,c) SAMv7_PageProgram(a,b,c)
#define __ReadPage(a,b,c) SAMv7_PageRead(a,b,c)
static s32 __WriteFragment(u32 PageNo, u32 Offset, const u8 *Buf, u32 Size)
{
    u32 i;
    s32 Ret;

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(Offset);
    NAND_ADDRESS_CYCLE(Offset>>8);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    for(i = 0; i < Size; i++)
        NAND_WR_DATA_CYCLE(Buf[i]);

    NAND_COMMAND_CYCLE(PAGE_PROGRAM_CMD_BYTE2);// д��Main�������

    Djy_EventDelay(700);// �г�
    __NandWaitReady();// ʱ��Ҫ��

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

    return (Size);
}
//-----------------------------------------------------------------------------
//����: ��С��һҳ������
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static s32 __ReadFragment(u32 PageNo, u32 Offset, u8 *Buf, u32 Size)
{
    u32 i;
    s32 Ret;

    __NandWaitReady(); // todo����ֹ��ͻ

    __NandOn();

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE1);

    NAND_ADDRESS_CYCLE(Offset);
    NAND_ADDRESS_CYCLE(Offset>>8);
    NAND_ADDRESS_CYCLE(PageNo);
    NAND_ADDRESS_CYCLE(PageNo>>8);

    NAND_COMMAND_CYCLE(PAGE_READ_CMD_BYTE2);

    Djy_EventDelay(25);
    __NandWaitReady(); // ʱ��Ҫ��

    for(i = 0; i < Size; i++)
        Buf[i] = NAND_RD_DATA_CYCLE();

    Ret = __NandStatus();

    __NandOff();

    if(Ret)
        return (-2);

    return (Size);
}
//-----------------------------------------------------------------------------
//����: д����
//����: BlkNo -- ���;
//      Offset -- ����ƫ��;
//      Buf -- д����;
//      Size -- д���ݴ�С
//      Flags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
//����:
//��ע:
//-----------------------------------------------------------------------------
u32 EFS_IF_WriteData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 WrLen = Size;

    if(!Buf)
        return (0);

    // ECC������ԴΪ256�ֽڣ��������ݺ��,3���ֽ�
    if(1 == Flags)
    {
        hamming_compute_256x(Buf, 256, Buf+256);
        WrLen = Size + 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (WrLen >= s_ptNandInfo->BytesPerPage))
            Ret = __WritePage(PageNo, Buf, NO_ECC);
        else if((WrLen >= s_ptNandInfo->BytesPerPage) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((WrLen+PageOffset) > s_ptNandInfo->BytesPerPage) && (!Flags))) // д����С��һҳ,�����ݿ�ҳ(ֻ�п��Ƿ�ECC�����)
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, (s_ptNandInfo->BytesPerPage - PageOffset));
        else
            Ret = __WriteFragment(PageNo, PageOffset, (const u8*)Buf, WrLen);

        if(Ret <= 0)
            break;// ���������д��

        WrLen -= Ret;
        if(WrLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }

    if(Ret > 0)
        return (Size);
    else
        return (0);
}

//-----------------------------------------------------------------------------
//����: ������
//����: BlkNo -- ���;
//      Offset -- ����ƫ��;
//      Buf -- ������;
//      Size -- �����ݴ�С
//      Flags -- 0x0 -- �޹���; 0x1 - ECC���ܿ���(���ݴ�СΪ����256)
//����:
//��ע:
//-----------------------------------------------------------------------------
u32 EFS_IF_ReadData(u32 BlkNo, u32 Offset, u8 *Buf, u32 Size, u8 Flags)
{
    s32 Ret;
    u32 PageNo = (BlkNo << 6) + (Offset >> 11);
    u32 PageOffset = Offset & 0x7FF;
    u32 RdLen = Size;

    if(!Buf)
        return (0);

    if(1 == Flags)
    {
        RdLen += 3;
        PageOffset += 3 * (PageOffset >> 8);
    }

    for(;;)
    {
        if((!PageOffset) && (RdLen >= s_ptNandInfo->BytesPerPage))
            Ret = __ReadPage(PageNo, Buf, NO_ECC);
        else if((RdLen >= s_ptNandInfo->BytesPerPage) || // д���ݴ������һҳ,�����ݿ�ҳ
                (((RdLen+PageOffset) > s_ptNandInfo->BytesPerPage) && (!Flags))) // ������С��һҳ,�����ݿ�ҳ(ֻ�з�ECC�������Ҫ����)
            Ret = __ReadFragment(PageNo, PageOffset, (const u8*)Buf, (s_ptNandInfo->BytesPerPage - PageOffset));
        else
            Ret = __ReadFragment(PageNo, PageOffset, (const u8*)Buf, RdLen);

        if(Ret <= 0)
            break;// ���������д��

        RdLen -= Ret;
        if(RdLen <= 0)
            break;
        Buf += Ret;
        PageNo++;
        if(PageOffset)
            PageOffset = 0;
    }


    if(Ret <= 0)
        return (0);

    if((1 == Flags) && (256 == Size))
    {
        u8 *HammingCode;
        u32 EccRet;

        HammingCode = Buf + 256;
        EccRet = hamming_verify_256x(Buf, 256, HammingCode);
        if (EccRet && (EccRet != HAMMING_ERROR_SINGLE_BIT))
        {
//            TraceDrv(FLASH_TRACE_DEBUG, "cannot be fixed");
            debug_printf("null","cannot be fixed");
            return (0);
        }
    }

    return (Size);
}

//-----------------------------------------------------------------------------
//����: ����
//����: BlkNo -- ���;
//����:
//��ע:
//-----------------------------------------------------------------------------
bool_t EFS_IF_Erase(u32 BlkNo)
{
    if(SAMv7_BlockErase(BlkNo))
        return (FALSE);

    return (TRUE);
}
//-----------------------------------------------------------------------------
//����: ����ļ��Ƿ��д
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
static u8 TempBuf[259];
s32 EFS_IF_IsFragmentWritten(u32 BlkNo, u32 Offset)
{
    u16 i;
    u32 PageOffset = Offset & 0x7FF;
    u8 EccOffset = 3 * (PageOffset >> 8);

    if(0 == EFS_IF_ReadData(BlkNo, (Offset+EccOffset), TempBuf, 259, 0))
        return (-1);

    for(i = 0; i < 259; i++)
    {
        if(0xFF != TempBuf[i])
            return (-1);
    }

    return (0);
}
//-----------------------------------------------------------------------------
//����: δʵ��
//����:
//����:
//��ע:
//-----------------------------------------------------------------------------
bool_t EFS_IF_CheckBlockReady(u32 block,u32 offset,
        u8 *buf,u32 size)
{
    u32 CurBlock,CurBlockOffset;
    u32 BlockSize;
    bool_t result = TRUE;

    if(NULL == s_ptNandInfo)
    {
        return FALSE;
    }

    CurBlock = block;
    CurBlockOffset = offset;
    BlockSize = s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk;

    while(size)
    {
        if(-1 == EFS_IF_IsFragmentWritten(CurBlock,CurBlockOffset))
        {
            result = FALSE;
            break;
        }
        CurBlockOffset += 256;
        CurBlock += CurBlockOffset / BlockSize;
        CurBlockOffset = CurBlockOffset % BlockSize;
        size = (size > 256) ? (size - 256) :0;
    }
    return result;// �ļ�д����������ڸ�д�߼�
}


/******************************************************************************
                                 ���ز���
******************************************************************************/
void EFS_DriverTest(void)
{
    u8 *Size256, *Size256Check, *SizeBig;
    u32 i, TestSize;
    u32 BlkNo, BlkOffset;

    if(ModuleInstall_NAND("nand", 0, 0))
        while(1);

    Size256 = malloc(256+3);
    if(!Size256)
        while(1);

    Size256Check = malloc(256+3);
    if(!Size256Check)
        while(1);

    debug_printf("null","---------EFS DRIVER TEST---------\r\n");

    //
    EFS_IF_Erase(0);

    if(0 == EFS_IF_ReadData(0, 0, Size256, 259, 0))
        debug_printf("null","Block 0 offset 0x0 cannot be read!\r\n");
    else
        PrintBuf(Size256, 259);

    for(i = 0; i < 256; i++)
        Size256[i] = i;

    if(0 == EFS_IF_WriteData(0, 0, Size256, 256, 1))
        debug_printf("null","Block 0 offset 0x0 cannot be write!\r\n");

    if(0 == EFS_IF_ReadData(0, 0, Size256, 256, 1))
        debug_printf("null","Block 0 offset 0x0 cannot be read!\r\n");

    PrintBuf(Size256, 259);


    for(BlkNo = 0; BlkNo < 10; BlkNo++)
    {
        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 256)
        {
            if(EFS_IF_IsFragmentWritten(BlkNo, BlkOffset))
            {
                if(FALSE == EFS_IF_Erase(BlkNo))
                    debug_printf("null","Block %d cannot be erased!\r\n", BlkNo);
                break;
            }
        }

        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 256)
        {
            if(0 == EFS_IF_WriteData(BlkNo, BlkOffset, Size256, 256, 1))
                debug_printf("null","Block %d offset 0x%x cannot be write!\r\n", BlkNo, BlkOffset);
        }

        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 256)
        {
            if(0 == EFS_IF_ReadData(BlkNo, BlkOffset, Size256, 256, 1))
                debug_printf("null","Block %d offset 0x%x cannot be read!\r\n", BlkNo, BlkOffset);

            for(i = 0; i < 256; i++)
            {
                if((u8)i != Size256[i])
                {
                    debug_printf("null","Block %d offset 0x%x read is not right!\r\n", BlkNo, BlkOffset);
                    while(1);
                }
            }
        }
    }

    TestSize = 3000;
    SizeBig = malloc(TestSize);
    if(!SizeBig)
        while(1);

    for(i = 0; i < TestSize; i++)
        SizeBig[i] = (u8)i;

    for(BlkNo = 10; BlkNo < 20; BlkNo++)
    {
        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 256)
        {
            if(EFS_IF_IsFragmentWritten(BlkNo, BlkOffset))
            {
                if(FALSE == EFS_IF_Erase(BlkNo))
                    debug_printf("null","Block %d cannot be erased!\r\n", BlkNo);
                break;
            }
        }

        for(BlkOffset = 0; (BlkOffset+TestSize) < (s_ptNandInfo->BytesPerPage*s_ptNandInfo->PagesPerBlk); BlkOffset += TestSize)
        {
            if(0 == EFS_IF_WriteData(BlkNo, BlkOffset, SizeBig, TestSize, 0))
                debug_printf("null","Block %d offset 0x%x cannot be write!\r\n", BlkNo, BlkOffset);
        }

        for(BlkOffset = 0; (BlkOffset+TestSize) < (s_ptNandInfo->BytesPerPage*s_ptNandInfo->PagesPerBlk); BlkOffset += TestSize)
        {
            if(0 == EFS_IF_ReadData(BlkNo, BlkOffset, SizeBig, TestSize, 0))
                debug_printf("null","Block %d offset 0x%x cannot be read!\r\n", BlkNo, BlkOffset);

            for(i = 0; i < TestSize; i++)
            {
                if((u8)i != SizeBig[i])
                {
                    debug_printf("null","Block %d offset 0x%x read is not right!\r\n", BlkNo, BlkOffset);
                    while(1);
                }
            }
        }
    }

    // ECC����
    for(BlkNo = 0; BlkNo < 1; BlkNo++)
    {
        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 259)
        {
            if(0 == EFS_IF_ReadData(BlkNo, BlkOffset, Size256, 259, 0))
                debug_printf("null","Block %d offset 0x%x cannot be read!\r\n", BlkNo, BlkOffset);
            Size256[1] = 0;
            if(0 == EFS_IF_WriteData(BlkNo, BlkOffset, Size256, 259, 0))
                debug_printf("null","Block %d offset 0x%x cannot be write!\r\n", BlkNo, BlkOffset);
        }

        for(BlkOffset = 0; BlkOffset < s_ptNandInfo->BytesPerPage * s_ptNandInfo->PagesPerBlk; BlkOffset += 256)
        {
            if(0 == EFS_IF_ReadData(BlkNo, BlkOffset, Size256, 256, 1))
                debug_printf("null","Block %d offset 0x%x cannot be read!\r\n", BlkNo, BlkOffset);

            for(i = 0; i < 256; i++)
            {
                if((u8)i != Size256[i])
                {
                    debug_printf("null","Block %d offset 0x%x read is not right!\r\n", BlkNo, BlkOffset);
                    while(1);
                }
            }
        }
    }
}
#endif
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
