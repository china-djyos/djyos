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
// ģ������:P1020rdb�����MMUӳ���
// ģ��汾:
// ������Ա: zhangqf
// ����ʱ��: 9:14:46 AM/Sep 17, 2013
// =======================================================================
// �����޸ļ�¼(���µķ�����ǰ��):
// <�汾��> <�޸�����>, <�޸���Ա>: <�޸Ĺ��ܸ���>
// =======================================================================
#include "stdint.h"
#include "E500v2RegBits.h"
#include "SysSpace.h"
#include "TLB.h"
#include "LAW.h"
#include "ELBC.h"

const tagTlbConfig tgTlbConfig[] = {
    //ddr
    SET_TLB_ENTRY(1, CN_DDR_ADDR_V, CN_DDR_ADDR_P,
                MAS3_SX|MAS3_SW|MAS3_SR, MAS2_W | MAS2_G |MAS2_M,
                0, 0, BOOKE_PAGESZ_1G, 1),     //DDR WILL BE WRITE THROUGH
    //other device route
    SET_TLB_ENTRY(1, CN_SRAM_ADDR_V, CN_SRAM_ADDR_P,
                MAS3_SW|MAS3_SR|MAS3_SX, MAS2_I| MAS2_G |MAS2_M ,
                0, 7, BOOKE_PAGESZ_256K, 1),    //SRAM

    //ccsr
    SET_TLB_ENTRY(1, CN_CCSR_NEWADDR_V, CN_CCSR_NEWADDR_PL,
                MAS3_SW|MAS3_SR, MAS2_I | MAS2_G|MAS2_M,
                0, 11, BOOKE_PAGESZ_1M, 1),

    //nor flash  16MB--storage
    SET_TLB_ENTRY(1, CN_NORFLASH_EXTEND_V, CN_NORFLASH_EXTEND_P,
                MAS3_SX|MAS3_SW|MAS3_SR,MAS2_I |MAS2_G|MAS2_M,
                0, 12, BOOKE_PAGESZ_16M, 1),
    //nor flash  16MB--code
    SET_TLB_ENTRY(1, CN_NORFLASH_ADDR_V, CN_NORFLASH_ADDR_P,
                MAS3_SX|MAS3_SW|MAS3_SR,MAS2_G|MAS2_M,
                0, 13, BOOKE_PAGESZ_16M, 1),

    //nand flash
    SET_TLB_ENTRY(1, CN_NANDFLASH_ADDR_V, CN_NANDFLASH_ADDR_P,
                MAS3_SX|MAS3_SW|MAS3_SR,  MAS2_I |MAS2_G|MAS2_M,
                0, 14, BOOKE_PAGESZ_4K, 1),
};
const int gTlbCfgItem = ARRAY_SIZE(tgTlbConfig);
//LAW config
//0x00000000--0x3fffffff     1GB reserved for the mem
const tagLawConfig tgLawConfig[] = {
    //DDR
    {0x00000000,0x80F0001d},
    //ELBC
    {0x000fe000,0x80400018},
};
const int gLawCfgItem = ARRAY_SIZE(tgLawConfig);

//elbc config
const tagElbcCfg  tgP1020RDB_ElbcCfg[] =
{
        {0xFE001001, 0xFE000F87},   //NOR FLASH BOOT,GPCM-CS0
        {0,0},
        {0xC0000C21, 0xF80003AE},   //NAND FLASH-CS2
        {0xEFA00801, 0xFFFE09F7},   //CPLD-CS3
};
const int gElbcCfgItem = ARRAY_SIZE(tgLawConfig);

//do the cache init

#define CN_CACHEL2_BASEADDR          (CN_CCSR_NEWADDR_V + 0x20000)
#define CN_CACHEL2_L2CTL             (CN_CACHEL2_BASEADDR + 0x00)
#define CN_CACHEL2_L2CTL_L2E         (1<<31)
#define CN_CACHEL2_L2CTL_L2I         (1<<30)
#define CN_CACHEL2_L2CTL_NOSRAM      (0x0<<16)
#define CN_CACHEL2_L2CTL_ALLSRAM     (0X01<<16)

#define CN_CACHEL2_L2ERRDIS          (CN_CACHEL2_BASEADDR+0xE44)
#define CN_CACHEL2_L2ERRDIS_SBECC    (1<<2)
#define CN_CACHEL2_L2ERRDIS_MBECC    (1<<3)

#define CN_CACHEL2_L2SRBAR0          (CN_CACHEL2_BASEADDR + 0X100)
#define CN_CACHEL2_L2SRBAREA0        (CN_CACHEL2_BASEADDR + 0x104)
#define CN_CACHEL2_SRAM_ADDRL        (0x40000000)
#define CN_CACHEL2_SRAM_ADDRH        (0x0)

/*
@-------------------------------------------------------------------------------
@function:initial the l2 cache
@para:void
@return:void
@-------------------------------------------------------------------------------
*/
void Cache_InitL2(void)
{
    u32 value;
    u32 *addr;

    //DISABLE THE SB AND MB ERROR
    addr = (u32 *)CN_CACHEL2_L2ERRDIS;
    value = *addr;
    value |=(CN_CACHEL2_L2ERRDIS_SBECC|CN_CACHEL2_L2ERRDIS_MBECC);
    *addr = value;

    //DISABLE THE L2CACHE AND SET TO THE 256KB CACHE
    addr = (u32 *)CN_CACHEL2_L2CTL;
    value = CN_CACHEL2_L2CTL_ALLSRAM;
    *addr = value;

    //SET THE  BASE ADDR
    addr = (u32 *)CN_CACHEL2_L2SRBAR0;
    value = CN_CACHEL2_SRAM_ADDRL;
    *addr = value;

    addr = (u32 *)CN_CACHEL2_L2SRBAREA0;
    value = CN_CACHEL2_SRAM_ADDRH;
    *addr = value;

    //ENABLE THE SRAM
    addr = (u32 *)CN_CACHEL2_L2CTL;
    value = *addr;
    value |= CN_CACHEL2_L2CTL_L2E;
    *addr = value;

    return;
}

